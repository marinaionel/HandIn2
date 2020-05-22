#pragma once
#include <stdint.h>

#include <FreeRTOS.h>
#include <event_groups.h>
#include <semphr.h>

typedef enum TemperatureDriverReturnCodes {
	TEMPERATURE_DRIVER_OK
	, TEMPERATURE_DRIVER_OUT_OF_HEAP
	, TEMPERATURE_DRIVER_NOT_CREATED
	, TEMPERATURE_DRIVER_TWI_BUSY
} TemperatureDriverReturnCode_t;

typedef struct TemperatureDriver* TemperatureDriver_t;
TemperatureDriver_t temperatureDriver_create(uint8_t pPortNo, EventGroupHandle_t measure_event_group_handle, EventGroupHandle_t new_data_event_group_handle, SemaphoreHandle_t pPrintfSemaphore);
uint16_t temperatureDriver_getMeasure(TemperatureDriver_t temperature_driver);
TemperatureDriverReturnCode_t temperatureDriver_takeMeasuring(TemperatureDriver_t temperature_driver);
void temperatureDriver_destroy(TemperatureDriver_t self);