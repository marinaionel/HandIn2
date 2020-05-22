extern "C" {
#include "../HandIn2/temperatureDriver.h"
}

#include "gmock/gmock.h"

namespace drivers
{
	class TemperatureDriverMock
	{
	public:
		virtual ~TemperatureDriverMock() {}
		MOCK_METHOD1(temperatureDriver_getMeasure, uint16_t(TemperatureDriver_t temperature_driver));
		MOCK_METHOD4(temperatureDriver_create, TemperatureDriver_t(uint8_t pPortNo, EventGroupHandle_t measure_event_group_handle, EventGroupHandle_t new_data_event_group_handle, SemaphoreHandle_t pPrintfSemaphore));
		MOCK_METHOD1(temperatureDriver_destroy, void(TemperatureDriver_t self));
	};
}
