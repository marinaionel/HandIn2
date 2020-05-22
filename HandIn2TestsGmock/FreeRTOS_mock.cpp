#include "FreeRTOS_mock.hpp"

freertos::FreeRTOSMock FreeRTOSMockObj;

QueueHandle_t xQueueGenericCreate(const UBaseType_t uxQueueLength, const UBaseType_t uxItemSize, const uint8_t ucQueueType)
{
	return FreeRTOSMockObj.xQueueGenericCreate(uxQueueLength, uxItemSize, ucQueueType);
}

BaseType_t xQueueGenericSend(QueueHandle_t xQueue, const void* const pvItemToQueue, TickType_t xTicksToWait, const BaseType_t xCopyPosition)
{
	return FreeRTOSMockObj.xQueueGenericSend(xQueue, pvItemToQueue, xTicksToWait, xCopyPosition);
}

void vQueueDelete(QueueHandle_t xQueue)
{
	FreeRTOSMockObj.vQueueDelete(xQueue);
}

EventBits_t xEventGroupSetBits(EventGroupHandle_t xEventGroup,
	const EventBits_t uxBitsToSet)
{
	return FreeRTOSMockObj.xEventGroupSetBits(xEventGroup, uxBitsToSet);
}

EventGroupHandle_t xEventGroupCreate()
{
	return FreeRTOSMockObj.xEventGroupCreate();
}

EventBits_t xEventGroupWaitBits(
	const EventGroupHandle_t xEventGroup,
	const EventBits_t uxBitsToWaitFor,
	const BaseType_t xClearOnExit,
	const BaseType_t xWaitForAllBits,
	TickType_t xTicksToWait)
{
	return FreeRTOSMockObj.xEventGroupWaitBits(xEventGroup, uxBitsToWaitFor, xClearOnExit, xWaitForAllBits, xTicksToWait);
}

void vEventGroupDelete(EventGroupHandle_t xEventGroup)
{
	FreeRTOSMockObj.vEventGroupDelete(xEventGroup);
}

BaseType_t xTaskCreate(TaskFunction_t pvTaskCode,
	const char* const pcName,
	configSTACK_DEPTH_TYPE usStackDepth,
	void* pvParameters,
	UBaseType_t uxPriority,
	TaskHandle_t* pxCreatedTask
)
{
	return FreeRTOSMockObj.xTaskCreate(pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask);
}

void vTaskDelete(TaskHandle_t xTask)
{
	FreeRTOSMockObj.vTaskDelete(xTask);
}

void vTaskDelay(const TickType_t xTicksToDelay)
{
	FreeRTOSMockObj.vTaskDelay(xTicksToDelay);
}

QueueHandle_t xQueueCreateMutex(const uint8_t ucQueueType)
{
	return FreeRTOSMockObj.xQueueCreateMutex(ucQueueType);
}

BaseType_t xQueueSemaphoreTake(QueueHandle_t xQueue, TickType_t xTicksToWait)
{
	return FreeRTOSMockObj.xQueueSemaphoreTake(xQueue, xTicksToWait);
}

void vPortFree(void* pv)
{
	FreeRTOSMockObj.vPortFree(pv);
}

void* pvPortMalloc(size_t xWantedSize)
{
	return FreeRTOSMockObj.pvPortMalloc(xWantedSize);
}