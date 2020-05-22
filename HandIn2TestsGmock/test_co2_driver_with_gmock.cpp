#include "FreeRTOS_mock.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

extern "C" {
#include "../HandIn2/co2driver.h"
}

extern freertos::FreeRTOSMock FreeRTOSMockObj;

class Co2DriverTest : public ::testing::Test
{
public:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

// TEST_F(Co2DriverTest, TestCreate)
// {
// 	EXPECT_CALL(FreeRTOSMockObj, pvPortMalloc(80)).Times(1);
// 	co2Driver_create(1, FreeRTOSMockObj.xEventGroupCreate(), FreeRTOSMockObj.xEventGroupCreate(), FreeRTOSMockObj.xQueueCreateMutex(queueQUEUE_TYPE_MUTEX));
// }