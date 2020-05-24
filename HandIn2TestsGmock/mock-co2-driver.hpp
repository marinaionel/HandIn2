#include "gmock/gmock.h"

extern "C" {
#include <FreeRTOS.h>
#include "../HandIn2/co2driver.h"
}

namespace drivers
{
	class Co2DriverMock
	{
	public:
		virtual ~Co2DriverMock() {}

		MOCK_METHOD4(co2Driver_create, Co2Driver_t(uint8_t pPortNo, EventGroupHandle_t measure_event_group_handle, EventGroupHandle_t new_data_event_group_handle, SemaphoreHandle_t pPrintfSemaphore));
		MOCK_METHOD1(co2Driver_destroy, void(Co2Driver_t* self));
		MOCK_METHOD1(co2Driver_getCo2Ppm, uint16_t(Co2Driver_t co2driver));
	};
}