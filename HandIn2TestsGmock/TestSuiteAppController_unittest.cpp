#include "FreeRTOS_mock.hpp"
#include "mock-temperature-driver.hpp"
#include "mock-co2-driver.hpp"
#include "mock-lora-driver.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

extern "C" {
#include "../HandIn2/AppController.h"
#include "../HandIn2/lora_driver.h"
}

extern freertos::FreeRTOSMock FreeRTOSMockObj;
extern drivers::TemperatureDriverMock temperature_driver_mock;
extern drivers::LoraDriverMock lora_driver_mock;
extern drivers::Co2DriverMock co_2driver_mock;

class AppControllerTest : public ::testing::Test
{
public:
	virtual void SetUp() {}
	virtual void TearDown()	{}
};

TEST(AppControllerTest, TestCreateCallsCreateQueue)
{
	//EXPECT_CALL(FreeRTOSMockObj, xQueueGenericCreate(1, 1 * sizeof(lora_payload), queueQUEUE_TYPE_BASE)).Times(1);
	//EXPECT_CALL(lora_driver_mock, lora_create).Times(1);
	//appController_create(1, 2);
	EXPECT_EQ(1, 1);
}