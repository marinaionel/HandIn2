#include "freertos-mock.h"
#include "gtest/gtest.h"

extern "C" {
#include "../HandIn2/co2driver.h"
}

freertos::FreeRTOSMock freertosmock;

TEST(CO2_DRIVER_TEST, TestCreate_calls_pvPortMalloc)
{
	EXPECT_CALL(freertosmock, pvPortMalloc());
}

TEST(CO2_DRIVER_TEST, TestCreate) {
	// Co2Driver_t co2driver = co2Driver_create();


	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

