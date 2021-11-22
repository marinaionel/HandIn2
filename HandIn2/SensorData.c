#include "SensorData.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct SensorData
{
	uint16_t sensorDataType_t;
	uint16_t value;
}SensorData;

SensorData_t sensorData_create(sensorDataType_t type)
{
	SensorData_t _sensorData = calloc(1, sizeof(SensorData));
	if (_sensorData == NULL) return NULL;
	_sensorData->sensorDataType_t = type;
	_sensorData->value = 0;
	return _sensorData;
}

void sensorData_setValue(SensorData_t sensor_data, uint16_t value)
{
	if (sensor_data == NULL)return;
	sensor_data->value = value;
}

uint16_t sensorData_getValue(SensorData_t sensor_data)
{
	if (sensor_data == NULL)return;
	return sensor_data->value;
}

void sensorData_destroy(SensorData_t sensor_data)
{
	if (sensor_data == NULL) return;
	free(sensor_data);
	sensor_data = NULL;
}