#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <event_groups.h>
#include <queue.h>
#include <portable.h>

#include "AppController.h"
#include "co2driver.h"
#include "lora_driver.h"
#include "temperatureDriver.h"
#include "constants.h"

#define APP_CONTROLLER_TASK_PRIORITY (configMAX_PRIORITIES-4)
#define APP_CONTROLLER_TAG "APP CONTROLLER"
#define APP_CONTROLLER_TASK_NAME "App Controller Task"

#define TIME_PERIOD_WHEN_TO_MEASURE (6000 / portTICK_PERIOD_MS)

typedef struct AppController
{
	TaskHandle_t task_handle;
	SemaphoreHandle_t xPrintfSemaphore;
	Co2Driver_t co2driver;
	TemperatureDriver_t temperature_driver;
	LoraDriver_t lora_driver;
	EventGroupHandle_t event_group_handle_measure;
	EventGroupHandle_t event_group_handle_new_data;
	QueueHandle_t queue;
}AppController;

static void inLoop(AppController_t app_controller)
{
	//set bits to measure
	xEventGroupSetBits(app_controller->event_group_handle_measure, CO2_BIT_0 | TEMPERATURE_BIT_1);

	//wait for new data bits
	xEventGroupWaitBits(
		app_controller->event_group_handle_new_data,
		CO2_BIT_0 | TEMPERATURE_BIT_1,
		pdTRUE,
		pdTRUE,
		portMAX_DELAY);

	const uint16_t _temperatureResult = temperatureDriver_getMeasure(app_controller->temperature_driver);
	const uint16_t _co2Result = co2Driver_getCo2Ppm(app_controller->co2driver);

	if (app_controller->xPrintfSemaphore != NULL) {
		xSemaphoreTake(app_controller->xPrintfSemaphore, portMAX_DELAY);
		printf("%s :: temperature result = %d\n", APP_CONTROLLER_TAG, _temperatureResult);
		printf("%s :: co2 result = %d\n", APP_CONTROLLER_TAG, _co2Result);
		xSemaphoreGive(app_controller->xPrintfSemaphore);
	}

	lora_payload_t _payload = pvPortMalloc(1 * sizeof(lora_payload));
	_payload->len = 4;
	_payload->port_no = 1;
	_payload->bytes[0] = _co2Result >> 8;
	_payload->bytes[1] = _co2Result & 0xFF;
	_payload->bytes[2] = _temperatureResult >> 8;
	_payload->bytes[3] = _temperatureResult & 0xFF;

	//queue send
	xQueueSend(app_controller->queue, //queue handler
		(void*)&_payload,
		portMAX_DELAY);

	vTaskDelay(TIME_PERIOD_WHEN_TO_MEASURE);
}

static void appController_Task(void* pvParameters)
{
	AppController_t _ac = (AppController_t)pvParameters;
	for (;;)
	{
		inLoop(_ac);
	}
}

AppController_t appController_create(uint8_t co2PortNo, uint8_t temperaturePortNo)
{
	AppController_t _ac = pvPortMalloc(1 * sizeof(AppController));
	if (_ac == NULL) return NULL;
	
	_ac->xPrintfSemaphore = xSemaphoreCreateMutex();
	if (_ac->xPrintfSemaphore != NULL) xSemaphoreGive(_ac->xPrintfSemaphore);

	_ac->event_group_handle_measure = xEventGroupCreate();
	_ac->event_group_handle_measure != NULL ?
		printf("%s :: SUCCESS :: created event group measure successfully\n", APP_CONTROLLER_TAG) :
		printf("%s :: ERROR :: creation of event group measure failed\n", APP_CONTROLLER_TAG);

	_ac->event_group_handle_new_data = xEventGroupCreate();
	_ac->event_group_handle_new_data != NULL ?
		printf("%s :: SUCCESS :: created event group new data successfully\n", APP_CONTROLLER_TAG) :
		printf("%s :: ERROR :: creation of event group new data failed\n", APP_CONTROLLER_TAG);

	_ac->co2driver = co2Driver_create(co2PortNo, _ac->event_group_handle_measure, _ac->event_group_handle_new_data, _ac->xPrintfSemaphore);
	_ac->co2driver != NULL ? printf("%s :: SUCCESS :: created co2 driver successfully\n", APP_CONTROLLER_TAG) :
		printf("%s :: ERROR :: creation of co2 driver failed", APP_CONTROLLER_TAG);

	_ac->temperature_driver = temperatureDriver_create(temperaturePortNo, _ac->event_group_handle_measure, _ac->event_group_handle_new_data, _ac->xPrintfSemaphore);
	_ac->temperature_driver != NULL ?
		printf("%s :: SUCCESS :: created temperature driver successfully\n", APP_CONTROLLER_TAG) :
		printf("%s :: ERROR :: creation of temperature driver failed\n", APP_CONTROLLER_TAG);

	_ac->queue = xQueueCreate(1, sizeof(lora_payload));
	_ac->queue != NULL ?
		printf("%s :: SUCCESS :: created queue\n", APP_CONTROLLER_TAG) :
		printf("%s :: ERROR :: creation of queue\n", APP_CONTROLLER_TAG);

	_ac->lora_driver = lora_create(_ac->queue, _ac->xPrintfSemaphore);
	_ac->lora_driver != NULL ?
		printf("%s :: SUCCESS :: created lora driver successfully\n", APP_CONTROLLER_TAG) :
		printf("%s :: ERROR :: creation of lora diver failed\n", APP_CONTROLLER_TAG);

	//Create task	
	xTaskCreate(
		appController_Task							/* Task function. */
		, (const portCHAR*)APP_CONTROLLER_TASK_NAME	/* String with name of task. */
		, configMINIMAL_STACK_SIZE					/* Stack size in words. */
		, (void*)_ac								/* Parameter passed as input of the task */
		, APP_CONTROLLER_TASK_PRIORITY		/* Priority of the task. */
		, &_ac->task_handle);						/* Task handle. */

	return _ac;
}

void appController_destroy(AppController_t self)
{
	if (self == NULL) return;

	co2Driver_destroy(self->co2driver);
	temperatureDriver_destroy(self->temperature_driver);

	vQueueDelete(self->queue);
	vEventGroupDelete(self->event_group_handle_measure);
	vEventGroupDelete(self->event_group_handle_new_data);
	vSemaphoreDelete(self->xPrintfSemaphore);
	
	vTaskDelete(self->task_handle);
	vPortFree(self);

	self = NULL;
}