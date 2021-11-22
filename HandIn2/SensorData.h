#pragma once
#include <stdint.h>

typedef enum
{
	CO2_DATA, TEMPERATURE_DATA
}sensorDataType_t;

typedef struct SensorData* SensorData_t;

SensorData_t sensorData_create(sensorDataType_t type);
void sensorData_destroy(SensorData_t sensor_data);
uint16_t sensorData_getValue(SensorData_t sensor_data);
void sensorData_setValue(SensorData_t sensor_data, uint16_t value);