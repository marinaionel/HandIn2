#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>
#include <semphr.h>

#include "constants.h"
#include "co2driver.h"

#define CO2_TASK_PRIORITY (configMAX_PRIORITIES-4)
#define CO2_DRIVER_TASK_NAME "CO2"
#define CO2_DRIVER_TAG "CO2 DRIVER"

typedef struct Co2Driver {
	uint8_t portNo;
	uint16_t lastMeasurement;
	SemaphoreHandle_t xPrintfSemaphore;
	EventGroupHandle_t event_group_handle_measure;
	EventGroupHandle_t event_group_handle_new_data;
	TaskHandle_t task_handle;
}Co2Driver;

void co2Driver_inLoop(Co2Driver_t pDriver)
{
	EventBits_t uxBits = xEventGroupWaitBits(
		pDriver->event_group_handle_measure,
		CO2_BIT_0,
		pdTRUE,
		pdTRUE,
		portMAX_DELAY);
	if ((uxBits & CO2_BIT_0) == CO2_BIT_0)
	{
		co2Driver_takeMeasuring(pDriver);
	}
}

void co2sensor_Task(void* pvParameters) {
	Co2Driver_t _passedDriver = (Co2Driver_t)pvParameters;

	for (;;) {
		co2Driver_inLoop(_passedDriver);
	}
	vTaskDelete(_passedDriver->task_handle);
}

Co2Driver_t co2Driver_create(uint8_t pPortNo, EventGroupHandle_t measure_event_group_handle, EventGroupHandle_t new_data_event_group_handle, SemaphoreHandle_t pPrintfSemaphore)
{
	Co2Driver_t _co2driver = pvPortMalloc(1 * sizeof(Co2Driver));
	if (_co2driver == NULL) return NULL;

	_co2driver->portNo = pPortNo;
	_co2driver->lastMeasurement = 0;
	_co2driver->xPrintfSemaphore = pPrintfSemaphore;
	_co2driver->event_group_handle_measure = measure_event_group_handle;
	_co2driver->event_group_handle_new_data = new_data_event_group_handle;
	_co2driver->task_handle = NULL;

	//Create task
	xTaskCreate(
		co2sensor_Task									/* Task function. */
		, (const portCHAR*)CO2_DRIVER_TASK_NAME			/* String with name of task. */
		, configMINIMAL_STACK_SIZE						/* Stack size in words. */
		, (void*)_co2driver								/* Parameter passed as input of the task */
		, CO2_TASK_PRIORITY					/* Priority of the task. */
		, &_co2driver->task_handle);					/* Task handle. */

	return _co2driver;
}

co2_return_code_enum co2Driver_takeMeasuring(Co2Driver_t co2driver)
{
	if (co2driver == NULL) return CO2_DRIVER_NOT_CREATED;

	if (co2driver->xPrintfSemaphore != NULL) {
		xSemaphoreTake(co2driver->xPrintfSemaphore, portMAX_DELAY);
		printf("CO2 DRIVER :: Taking co2 measurement...\n");
		xSemaphoreGive(co2driver->xPrintfSemaphore);
	}

	//rand() % (max_number + 1 - minimum_number) + minimum_number
	co2driver->lastMeasurement = rand() % (5000 + 1 - 0) + 0;

	xEventGroupSetBits(co2driver->event_group_handle_new_data, CO2_BIT_0);
	return CO2_DRIVER_OK;
}

int16_t co2Driver_getCo2Ppm(Co2Driver_t co2driver)
{
	if (co2driver == NULL) return -1;
	return co2driver->lastMeasurement;
}

void co2Driver_destroy(Co2Driver_t* self)
{
	if (self == NULL || *self == NULL) return;
	vTaskDelete((*self)->task_handle);
	vPortFree(*self);
	*self = NULL;
}