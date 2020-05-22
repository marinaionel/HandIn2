#include "mock-lora-driver.hpp"

drivers::LoraDriverMock lora_driver_mock;

LoraDriver_t lora_create(QueueHandle_t p_queue_handle, SemaphoreHandle_t pPrintfSemaphore)
{
	return lora_driver_mock.lora_create(p_queue_handle, pPrintfSemaphore);
}

void loraDriver_destroy(LoraDriver_t self)
{
	lora_driver_mock.loraDriver_destroy(self);
}