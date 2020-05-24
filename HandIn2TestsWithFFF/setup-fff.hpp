#pragma once
#include <fff.h>
DEFINE_FFF_GLOBALS
#include <gtest/gtest.h>

extern "C" {
#include <FreeRTOS.h>
#include <queue.h>
#include <event_groups.h>
#include "../HandIn2/co2driver.h"
#include "../HandIn2/temperatureDriver.h"
}

FAKE_VALUE_FUNC1(void*, pvPortMalloc, size_t)
FAKE_VALUE_FUNC6(BaseType_t, xTaskCreate, TaskFunction_t, const char*, configSTACK_DEPTH_TYPE, void*, UBaseType_t, TaskHandle_t*)
FAKE_VALUE_FUNC2(BaseType_t, xQueueSemaphoreTake, QueueHandle_t, TickType_t)
FAKE_VALUE_FUNC4(portBASE_TYPE, xQueueGenericSend, xQueueHandle, const void*, portTickType, portBASE_TYPE)
FAKE_VALUE_FUNC5(EventBits_t, xEventGroupWaitBits, EventGroupHandle_t, EventBits_t, BaseType_t, BaseType_t, TickType_t)
FAKE_VALUE_FUNC2(EventBits_t, xEventGroupSetBits, EventGroupHandle_t, EventBits_t)
FAKE_VOID_FUNC1(vTaskDelete, TaskHandle_t)
FAKE_VOID_FUNC1(vPortFree, void*)

class Co2DriverTest : public::testing::Test
{
protected:
	void SetUp() override
	{
		RESET_FAKE(pvPortMalloc);
		RESET_FAKE(xTaskCreate);
		RESET_FAKE(xQueueSemaphoreTake);
		RESET_FAKE(xQueueGenericSend);
		RESET_FAKE(xEventGroupWaitBits);
		RESET_FAKE(xEventGroupSetBits);
		RESET_FAKE(vTaskDelete);
		RESET_FAKE(vPortFree);
		FFF_RESET_HISTORY();
	}
	void TearDown() override {}
};

class Co2DriverTest1 : public::testing::Test
{
public:
	Co2Driver_t co2driver;
protected:
	void SetUp() override
	{
		RESET_FAKE(pvPortMalloc);
		RESET_FAKE(xTaskCreate);
		RESET_FAKE(xQueueSemaphoreTake);
		RESET_FAKE(xQueueGenericSend);
		RESET_FAKE(xEventGroupWaitBits);
		RESET_FAKE(xEventGroupSetBits);
		RESET_FAKE(vTaskDelete);
		RESET_FAKE(vPortFree);
		FFF_RESET_HISTORY();

		pvPortMalloc_fake.return_val = malloc(20);
		co2driver = co2Driver_create(1, nullptr, nullptr, nullptr);
	}
	void TearDown() override
	{
		co2Driver_destroy(&co2driver);
	}
};

class TemperatureDriverTest : public::testing::Test
{
protected:
	void SetUp() override
	{
		RESET_FAKE(pvPortMalloc);
		RESET_FAKE(xTaskCreate);
		RESET_FAKE(xQueueSemaphoreTake);
		RESET_FAKE(xQueueGenericSend);
		RESET_FAKE(xEventGroupWaitBits);
		RESET_FAKE(xEventGroupSetBits);
		RESET_FAKE(vTaskDelete);
		RESET_FAKE(vPortFree);
		FFF_RESET_HISTORY();
	}
	void TearDown() override {}
};

class TemperatureDriverTest1 : public::testing::Test
{
public:
	TemperatureDriver_t temperature_driver;
protected:
	void SetUp() override
	{
		RESET_FAKE(pvPortMalloc);
		RESET_FAKE(xTaskCreate);
		RESET_FAKE(xQueueSemaphoreTake);
		RESET_FAKE(xQueueGenericSend);
		RESET_FAKE(xEventGroupWaitBits);
		RESET_FAKE(xEventGroupSetBits);
		RESET_FAKE(vTaskDelete);
		RESET_FAKE(vPortFree);
		FFF_RESET_HISTORY();

		pvPortMalloc_fake.return_val = malloc(20);
		temperature_driver = temperatureDriver_create(1, nullptr, nullptr, nullptr);
	}
	void TearDown() override
	{
		temperatureDriver_destroy(&temperature_driver);
	}
};