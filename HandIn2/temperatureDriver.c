#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <FreeRTOS.h>
#include <event_groups.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>

#include "temperatureDriver.h"
#include "constants.h"

#define TEMPERATURE_TASK_PRIORITY (configMAX_PRIORITIES-4)

typedef struct TemperatureDriver
{
	uint8_t portNo;
	uint16_t lastValue;
	SemaphoreHandle_t xPrintfSemaphore;
	EventGroupHandle_t event_group_handle_measure;
	EventGroupHandle_t event_group_handle_new_data;
	TaskHandle_t task_handle;
}TemperatureDriver;

static void inLoop(TemperatureDriver_t pDriver)
{
	EventBits_t uxBits = xEventGroupWaitBits(
		pDriver->event_group_handle_measure,
		TEMPERATURE_BIT_1,
		pdTRUE,
		pdTRUE,
		portMAX_DELAY);
	if ((uxBits & TEMPERATURE_BIT_1) == TEMPERATURE_BIT_1)
	{
		TemperatureDriverReturnCode_t result = temperatureDriver_takeMeasuring(pDriver);
		if (result != TEMPERATURE_DRIVER_OK)
		{
			if (pDriver->xPrintfSemaphore != NULL) {
				xSemaphoreTake(pDriver->xPrintfSemaphore, portMAX_DELAY);
				printf("TEMPERATURE SENSOR :: take measuring failed :: return code %d", result);
				xSemaphoreGive(pDriver->xPrintfSemaphore);
			}
		}
	}
}

static void temperatureSensor_Task(void* pvParameters) {
	// avoid compiler warning about unused parameters
	TemperatureDriver_t _passedDriver = (TemperatureDriver_t)pvParameters;

	for (;;) {
		inLoop(_passedDriver);
	}
	vTaskDelete(_passedDriver->task_handle);
}

static void createTemperatureTask(TemperatureDriver_t driver)
{
	//Create tasks	

	xTaskCreate(
		temperatureSensor_Task						/* Task function. */
		, (const portCHAR*)"Temperature Task"					/* String with name of task. */
		, configMINIMAL_STACK_SIZE					/* Stack size in words. */
		, (void*)driver								/* Parameter passed as input of the task */
		, TEMPERATURE_TASK_PRIORITY		/* Priority of the task. */
		, &(driver->task_handle));					/* Task handle. */
}

TemperatureDriver_t temperatureDriver_create(uint8_t pPortNo, EventGroupHandle_t measure_event_group_handle, EventGroupHandle_t new_data_event_group_handle, SemaphoreHandle_t pPrintfSemaphore)
{
	TemperatureDriver_t _t = calloc(1, sizeof(TemperatureDriver));
	if (_t == NULL)return NULL;
	_t->portNo = pPortNo;
	_t->lastValue = 0;
	_t->event_group_handle_new_data = new_data_event_group_handle;
	_t->event_group_handle_measure = measure_event_group_handle;
	_t->xPrintfSemaphore = pPrintfSemaphore;
	_t->task_handle = NULL;
	createTemperatureTask(_t);
	return _t;
}

TemperatureDriverReturnCode_t temperatureDriver_takeMeasuring(TemperatureDriver_t temperature_driver)
{
	if (temperature_driver == NULL) return TEMPERATURE_DRIVER_NOT_CREATED;

	if (temperature_driver->xPrintfSemaphore != NULL) {
		xSemaphoreTake(temperature_driver->xPrintfSemaphore, portMAX_DELAY);
		printf("TEMPERATURE DRIVER :: Taking temperature measurement...\n");
		xSemaphoreGive(temperature_driver->xPrintfSemaphore);
	}

	//rand() % (max_number + 1 - minimum_number) + minimum_number
	temperature_driver->lastValue = rand() % (35 + 1 - 10) + 10;
	xEventGroupSetBits(temperature_driver->event_group_handle_new_data, TEMPERATURE_BIT_1);
	taskYIELD();
	return TEMPERATURE_DRIVER_OK;
}

void temperatureDriver_destroy(TemperatureDriver_t self)
{
	if (self == NULL) return;
	vTaskDelete(self->task_handle);
	vPortFree(self);
	self = NULL;
}

uint16_t temperatureDriver_getMeasure(TemperatureDriver_t temperature_driver)
{
	if (temperature_driver == NULL) return -1;
	return temperature_driver->lastValue;
}

uint8_t temperatureDriver_getPortNo(TemperatureDriver_t temperature_driver)
{
	if (temperature_driver == NULL)
	{
		return -1;
	}
	return temperature_driver->portNo;
}