#include "mock-temperature-driver.hpp"

drivers::TemperatureDriverMock temperature_driver_mock;

TemperatureDriver_t temperatureDriver_create(uint8_t pPortNo, EventGroupHandle_t measure_event_group_handle, EventGroupHandle_t new_data_event_group_handle, SemaphoreHandle_t pPrintfSemaphore)
{
	return temperature_driver_mock.temperatureDriver_create(pPortNo, measure_event_group_handle, new_data_event_group_handle, pPrintfSemaphore);
}

int16_t temperatureDriver_getMeasure(TemperatureDriver_t temperature_driver)
{
	return temperature_driver_mock.temperatureDriver_getMeasure(temperature_driver);
}

void temperatureDriver_destroy(TemperatureDriver_t* self)
{
	temperature_driver_mock.temperatureDriver_destroy(self);
}