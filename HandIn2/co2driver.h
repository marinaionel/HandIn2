#pragma once
#include <FreeRTOS.h>
#include <event_groups.h>
#include <semphr.h>
#include <stdint.h>

typedef enum
{
	CO2_DRIVER_OK
	, CO2_DRIVER_NO_MEASURING_AVAILABLE
	, CO2_DRIVER_NOT_CREATED
	, CO2_DRIVER_ERROR
} co2_return_code_enum;

typedef struct Co2Driver* Co2Driver_t;

Co2Driver_t co2Driver_create(uint8_t pPortNo, EventGroupHandle_t measure_event_group_handle, EventGroupHandle_t new_data_event_group_handle, SemaphoreHandle_t pPrintfSemaphore);
uint16_t co2Driver_getCo2Ppm(Co2Driver_t co2driver);
void co2Driver_destroy(Co2Driver_t self);
co2_return_code_enum co2Driver_takeMeasuring(Co2Driver_t co2driver);
void co2sensor_Task(void* pvParameters);