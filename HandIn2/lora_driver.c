#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>
#include <stdio.h>

#include <projdefs.h>

#include "lora_driver.h"

#define LORA_DRIVER_TASK_PRIORITY (configMAX_PRIORITIES-3)
#define LORA_DRIVER_TAG "LORA DRIVER"
#define LORA_DRIVER_TASK_NAME "Lora Driver Task"

typedef struct LoraDriver
{
	TaskHandle_t task_handle;
	SemaphoreHandle_t xPrintfSemaphore;
	QueueHandle_t xQueue;
}LoraDriver;


static void binary_to_hex(char* destination_string, const char* bytes, int length)
{
	const char hex_digits[] = "0123456789ABCDEF";
	while (--length >= 0) destination_string[length] = hex_digits[(bytes[length >> 1] >> ((1 - (length & 1)) << 2)) & 0xF];
}

static void loraDriverTask(void* pvParameters)
{
	LoraDriver_t _ld = (LoraDriver_t)pvParameters;
	lora_payload_t _lorapayload = NULL;
	for (;;)
	{
		if (_ld->xQueue != NULL)
		{
			if (xQueueReceive((_ld->xQueue),
				&_lorapayload,
				portMAX_DELAY) == pdPASS)
			{
				if (_lorapayload != NULL)
				{
					_ld = (LoraDriver_t)pvParameters;
					loraDriver_sent_upload_message(_lorapayload, _ld);
				}
				else
				{
					if (_ld->xPrintfSemaphore != NULL) {
						xSemaphoreTake(_ld->xPrintfSemaphore, portMAX_DELAY);
						printf("%s :: NULL lora payload", LORA_DRIVER_TAG);
						xSemaphoreGive(_ld->xPrintfSemaphore);
					}
				}
			}
		}
	}
	vTaskDelete(_ld->task_handle);
}

LoraDriver_t lora_create(QueueHandle_t p_queue_handle, SemaphoreHandle_t pPrintfSemaphore)
{
	LoraDriver_t _ld = pvPortMalloc(1 * sizeof(LoraDriver));
	if (_ld == NULL) return NULL;

	_ld->xPrintfSemaphore = pPrintfSemaphore;
	_ld->xQueue = p_queue_handle;
	_ld->task_handle = NULL;

	//Create task
	xTaskCreate(
		loraDriverTask								/* Task function. */
		, (const portCHAR*)LORA_DRIVER_TASK_NAME	/* String with name of task. */
		, configMINIMAL_STACK_SIZE					/* Stack size in words. */
		, (void*)_ld								/* Parameter passed as input of the task */
		, LORA_DRIVER_TASK_PRIORITY		/* Priority of the task. */
		, &(_ld->task_handle));						/* Task handle. */

	return _ld;
}

void loraDriver_sent_upload_message(lora_payload_t p_lora_payload, LoraDriver_t lora_driver)
{
	if (lora_driver != NULL) {
		if (lora_driver->xPrintfSemaphore != NULL) {
			xSemaphoreTake(lora_driver->xPrintfSemaphore, portMAX_DELAY);
			printf("%s :: SENDING DATA PACKET :: ", LORA_DRIVER_TAG);

			char* hexstr = (char*)pvPortMalloc(p_lora_payload->len * sizeof(char));
			binary_to_hex(hexstr, p_lora_payload->bytes, p_lora_payload->len);
			hexstr[p_lora_payload->len] = '\0';
			printf("data packet{ portNo %d, len %d, bytes %s }\n", p_lora_payload->port_no, p_lora_payload->len, hexstr);
			vPortFree(hexstr);
			xSemaphoreGive(lora_driver->xPrintfSemaphore);
			vPortFree(p_lora_payload);
		}
	}
}

void loraDriver_destroy(LoraDriver_t self)
{
	if (self == NULL) return;
	vTaskDelete(self->task_handle);
	vPortFree(self);
	self = NULL;
}
