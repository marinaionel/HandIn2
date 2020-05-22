#include "gmock/gmock.h"

extern "C" {
#include <FreeRTOS.h>
#include "../HandIn2/lora_driver.h"
}

namespace drivers
{
	class LoraDriverMock
	{
	public:
		virtual ~LoraDriverMock() {}

		MOCK_METHOD2(lora_create, LoraDriver_t(QueueHandle_t p_queue_handle, SemaphoreHandle_t pPrintfSemaphore));
		MOCK_METHOD1(loraDriver_destroy, void(LoraDriver_t self));
	};
}