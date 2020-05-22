#include <FreeRTOS.h>
#include <queue.h>
#include <event_groups.h>

#include "gmock/gmock.h"

namespace freertos {

	class FreeRTOSMock
	{
	public:
		virtual ~FreeRTOSMock() {}

		//queue
		MOCK_METHOD3(xQueueGenericCreate,
			QueueHandle_t(const UBaseType_t uxQueueLength, const UBaseType_t uxItemSize, const uint8_t
				ucQueueType));
		MOCK_METHOD4(xQueueGenericSend, BaseType_t(QueueHandle_t xQueue, const void* const pvItemToQueue, TickType_t xTicksToWait, const BaseType_t xCopyPosition));
		MOCK_METHOD1(vQueueDelete, void(QueueHandle_t xQueue));

		//event group
		MOCK_METHOD0(xEventGroupCreate, EventGroupHandle_t(void));
		MOCK_METHOD2(xEventGroupSetBits, EventBits_t(EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet));
		MOCK_METHOD5(xEventGroupWaitBits, EventBits_t(EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToWaitFor, const BaseType_t xClearOnExit, const BaseType_t xWaitForAllBits, TickType_t xTicksToWait));
		MOCK_METHOD1(vEventGroupDelete, void(EventGroupHandle_t xEventGroup));

		//task
		MOCK_METHOD6(xTaskCreate, BaseType_t(TaskFunction_t pvTaskCode,
			const char* const pcName,
			configSTACK_DEPTH_TYPE usStackDepth,
			void* pvParameters,
			UBaseType_t uxPriority,
			TaskHandle_t* pxCreatedTask));
		MOCK_METHOD1(vTaskDelay, void(const TickType_t xTicksToDelay));
		MOCK_METHOD1(vTaskDelete, void(TaskHandle_t xTask));

		//semaphore
		MOCK_METHOD1(xQueueCreateMutex, QueueHandle_t(const uint8_t ucQueueType)); //param - queueQUEUE_TYPE_MUTEX
		MOCK_METHOD2(xQueueSemaphoreTake, BaseType_t(QueueHandle_t xQueue, TickType_t xTicksToWait));
		//xSemaphoreGive is xQueueGenericSend - xQueueGenericSend( ( QueueHandle_t ) ( xSemaphore ), NULL, semGIVE_BLOCK_TIME, queueSEND_TO_BACK )
		//vSemaphoreDelete is vQueueDelete

		//memory allocation
		MOCK_METHOD1(vPortFree, void(void* pv));
		MOCK_METHOD1(pvPortMalloc, void* (size_t xWantedSize));
	};
}