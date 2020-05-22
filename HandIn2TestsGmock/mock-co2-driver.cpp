#include "mock-co2-driver.hpp"

drivers::Co2DriverMock co_2driver_mock;

Co2Driver_t co2Driver_create(uint8_t pPortNo, EventGroupHandle_t measure_event_group_handle, EventGroupHandle_t new_data_event_group_handle, SemaphoreHandle_t pPrintfSemaphore)
{
	return co_2driver_mock.co2Driver_create(pPortNo, measure_event_group_handle, new_data_event_group_handle, pPrintfSemaphore);
}
uint16_t co2Driver_getCo2Ppm(Co2Driver_t co2driver)
{
	return co_2driver_mock.co2Driver_getCo2Ppm(co2driver);
}
void co2Driver_destroy(Co2Driver_t self)
{
	co_2driver_mock.co2Driver_destroy(self);
}
