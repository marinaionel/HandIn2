#include <fff.h>
#include <gtest/gtest.h>
DEFINE_FFF_GLOBALS

extern "C" {
#include <FreeRTOS.h>
#include <queue.h>
#include <event_groups.h>
#include "../HandIn2/co2driver.h"
}

FAKE_VALUE_FUNC1(void*, pvPortMalloc, size_t)
FAKE_VALUE_FUNC6(BaseType_t, xTaskCreate, TaskFunction_t, const char*, configSTACK_DEPTH_TYPE, void*, UBaseType_t, TaskHandle_t*)
FAKE_VALUE_FUNC2(BaseType_t, xQueueSemaphoreTake, QueueHandle_t, TickType_t)
FAKE_VALUE_FUNC4(portBASE_TYPE, xQueueGenericSend, xQueueHandle, const void*, portTickType, portBASE_TYPE)
FAKE_VALUE_FUNC5(EventBits_t, xEventGroupWaitBits, EventGroupHandle_t, EventBits_t, BaseType_t, BaseType_t, TickType_t)
FAKE_VALUE_FUNC2(EventBits_t, xEventGroupSetBits, EventGroupHandle_t, EventBits_t)
FAKE_VOID_FUNC1(vTaskDelete, TaskHandle_t)
FAKE_VOID_FUNC1(vPortFree, void*);

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

TEST_F(Co2DriverTest, TestCreate_callsMalloc)
{
	co2Driver_create(2, nullptr, nullptr, nullptr);
	EXPECT_EQ(1u, pvPortMalloc_fake.call_count);
	// sizeof(Co2Driver) = 20
	EXPECT_EQ(20u, pvPortMalloc_fake.arg0_val);
}

TEST_F(Co2DriverTest, TestCreate_callsxTaskCreate)
{
	co2Driver_create(2, nullptr, nullptr, nullptr);
	EXPECT_EQ(1u, 1u);
	EXPECT_EQ(co2sensor_Task, xTaskCreate_fake.arg0_val);
}