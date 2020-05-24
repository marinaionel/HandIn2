#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>
#include <stdio.h>

#include <projdefs.h>

#include "loraDriver.h"

#define LORA_DRIVER_TASK_PRIORITY (configMAX_PRIORITIES-3)
#define LORA_DRIVER_TAG "LORA DRIVER"

typedef struct LoraDriver
{
	TaskHandle_t task_handle;
	SemaphoreHandle_t xPrintfSemaphore;
	QueueHandle_t xQueue;
}LoraDriver;

void loraDriver_inLoop(LoraDriver_t self, lora_payload_t lora_payload)
{
	if (self->xQueue != NULL)
	{
		if (xQueueReceive(self->xQueue,
			&lora_payload,
			portMAX_DELAY) == pdPASS)
		{
			loraDriver_sent_upload_message(lora_payload, self);
		}
	}
}

void loraDriver_Task(void* pvParameters)
{
	static LoraDriver_t _ld1 = NULL;
	_ld1 = (LoraDriver_t)pvParameters;
	static lora_payload_t _lorapayload;
	for (;;)
	{
		loraDriver_inLoop(_ld1, _lorapayload);
	}
	vTaskDelete(_ld1->task_handle);
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
		loraDriver_Task								/* Task function. */
		, (const portCHAR*)"Lora Driver Task"	/* String with name of task. */
		, configMINIMAL_STACK_SIZE					/* Stack size in words. */
		, (void*)_ld								/* Parameter passed as input of the task */
		, LORA_DRIVER_TASK_PRIORITY		/* Priority of the task. */
		, &_ld->task_handle);						/* Task handle. */

	return _ld;
}

void loraDriver_sent_upload_message(lora_payload_t p_lora_payload, LoraDriver_t lora_driver)
{
	if (lora_driver != NULL) {
		if (lora_driver->xPrintfSemaphore != NULL) {
			xSemaphoreTake(lora_driver->xPrintfSemaphore, portMAX_DELAY);
			printf("%s :: SENDING DATA PACKET :: ", LORA_DRIVER_TAG);
			printf("data packet{ portNo %d, len %d, bytes ", p_lora_payload.port_no, p_lora_payload.len);

			for (int i = 0; i < p_lora_payload.len; i++)
			{
				if (i > 0) printf(":");
				printf("%02X", p_lora_payload.bytes[i]);
			}
			printf(" }\n");

			xSemaphoreGive(lora_driver->xPrintfSemaphore);
		}
	}
}

void loraDriver_destroy(LoraDriver_t* self)
{
	if (self == NULL || *self == NULL) return;
	vTaskDelete((*self)->task_handle);
	vPortFree(*self);
	*self = NULL;
}
