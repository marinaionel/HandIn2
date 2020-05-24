#include "setup-fff.hpp"

extern "C" {
#include "../HandIn2/constants.h"
}

//-------------------------------------TESTS CO2 DRIVER-------------------------------------------------------------------
TEST_F(Co2DriverTest, TestCreate_calls_create_without_crashing)
{
	//act
	co2Driver_create(1, NULL, NULL, NULL);
}

TEST_F(Co2DriverTest, TestCreate_callsMallocWithArgument20)
{
	//act
	co2Driver_create(2, nullptr, nullptr, nullptr);
	EXPECT_EQ(1u, pvPortMalloc_fake.call_count);
	//assert
	// sizeof(Co2Driver) = 20
	EXPECT_EQ(20u, pvPortMalloc_fake.arg0_val);
}

TEST_F(Co2DriverTest, TestCreate_returned_co_2_driver_pointer_is_not_null)
{
	//arrange
	pvPortMalloc_fake.return_val = malloc(20);
	//act
	Co2Driver_t _tmp = co2Driver_create(2, nullptr, nullptr, nullptr);
	//assert
	EXPECT_TRUE(NULL != _tmp);
}

TEST_F(Co2DriverTest, TestCreate_callsxTaskCreateWithTheRequiredArguments)
{
	//arrange
	pvPortMalloc_fake.return_val = malloc(20);
	//act
	Co2Driver_t _co2Driver = co2Driver_create(2, nullptr, nullptr, nullptr);
	//assert
	EXPECT_EQ((TaskFunction_t)co2sensor_Task, xTaskCreate_fake.arg0_val);
	EXPECT_EQ(0, strcmp("CO2", xTaskCreate_fake.arg1_val));
	EXPECT_EQ(configMINIMAL_STACK_SIZE, xTaskCreate_fake.arg2_val);
	EXPECT_EQ(_co2Driver, Co2Driver_t(xTaskCreate_fake.arg3_val));
	EXPECT_EQ(configMAX_PRIORITIES - 4, xTaskCreate_fake.arg4_val);
	EXPECT_FALSE(NULL == xTaskCreate_fake.arg5_val);
}

TEST_F(Co2DriverTest1, TestCreate_has_last_measurement_as_0)
{
	//assert
	EXPECT_EQ(0, co2Driver_getCo2Ppm(co2driver));
}

TEST_F(Co2DriverTest, TestDestroy_can_call_destroy_without_crashing)
{
	//arrange
	pvPortMalloc_fake.return_val = malloc(20);
	Co2Driver_t _tmp = co2Driver_create(1, nullptr, nullptr, nullptr);
	//act
	co2Driver_destroy(&_tmp);
}

TEST_F(Co2DriverTest, TestDestroy_can_pass_a_null_pointer_without_crashing)
{
	//act
	co2Driver_destroy(NULL);
}

TEST_F(Co2DriverTest, TestDestroy_after_calling_destroy_the_pointer_passed_as_an_argument_is_null)
{
	//arrange
	pvPortMalloc_fake.return_val = malloc(20);
	Co2Driver_t _tmp = co2Driver_create(1, nullptr, nullptr, nullptr);
	//act
	co2Driver_destroy(&_tmp);
	//assert
	EXPECT_EQ(nullptr, _tmp);
}

TEST_F(Co2DriverTest, TestDestroy_calls_v_task_delete)
{
	//Can't check if it is really the task handler of the passed pointer because
	//I'll have to make a getter for it for no reason other than this test,
	//but I can check that it calls vTaskDelete with a non null argument and
	//because the passed argument is mandatory to be of type TaskHandle_t
	//we can suppose that it is the task handle of the passed pointer

	//arrange
	pvPortMalloc_fake.return_val = malloc(20);
	Co2Driver_t _tmp = co2Driver_create(1, nullptr, nullptr, nullptr);
	//act
	co2Driver_destroy(&_tmp);
	//assert
	EXPECT_EQ(1u, vTaskDelete_fake.call_count);
}

TEST_F(Co2DriverTest, TestDestroy_calls_v_port_free_with_the_argument_as_the_passed_pointer)
{
	//arrange
	pvPortMalloc_fake.return_val = malloc(20);
	Co2Driver_t _tmp = co2Driver_create(1, nullptr, nullptr, nullptr);
	const size_t _int_address = size_t(_tmp);
	//act
	co2Driver_destroy(&_tmp);
	//assert
	EXPECT_EQ(1, vPortFree_fake.call_count);
	EXPECT_NE(nullptr, vPortFree_fake.arg0_val);
	EXPECT_TRUE(_int_address == size_t(vPortFree_fake.arg0_val));
}

TEST_F(Co2DriverTest1, TestGetCo2Ppm_can_call_get_co_2_ppm_without_crashing)
{
	//act
	co2Driver_getCo2Ppm(co2driver);
}

TEST_F(Co2DriverTest, TestGetCo2Ppm_returns_negative_1_if_the_passed_pointer_parameter_is_nul)
{
	//act
	//assert
	EXPECT_EQ(-1, co2Driver_getCo2Ppm(NULL));
}

TEST_F(Co2DriverTest1, TestGetCo2Ppm_returns_the_last_measurement_parameter_of_the_pointer_in_range_0)
{
	//act
	auto _t = co2Driver_getCo2Ppm(co2driver);
	//assert
	EXPECT_TRUE(_t <= 5000 && _t >= 0);
}

TEST_F(Co2DriverTest1, TestTakeMeasuring_can_call_take_measuring_without_crashing)
{
	//act
	co2Driver_takeMeasuring(co2driver);
}

TEST_F(Co2DriverTest1, TestTakeMeasuring_returns_co_2_driver_not_created_if_the_passed_parameter_is_null)
{
	//act
	//assert
	EXPECT_EQ(CO2_DRIVER_NOT_CREATED, co2Driver_takeMeasuring(NULL));
}

TEST_F(Co2DriverTest1, TestTakeMeasuring_calls_x_event_group_set_bits_with_the_parameters_event_group_handle_new_data_of_the_passed_pointer_and_co_2_bit_0)
{
	//act
	co2Driver_takeMeasuring(co2driver);
	//assert
	EXPECT_EQ(1u, xEventGroupSetBits_fake.call_count);
	EXPECT_TRUE(nullptr == xEventGroupSetBits_fake.arg0_val);
	EXPECT_EQ(CO2_BIT_0, xEventGroupSetBits_fake.arg1_val);
}

TEST_F(Co2DriverTest1, TestTakeMeasuring_generates_a_new_random_value_for_last_measurement_between_0_and_5000)
{
	//act
	auto _t = co2Driver_getCo2Ppm(co2driver);
	//assert
	EXPECT_TRUE(_t <= 5000 && _t >= 0);
	//act
	_t = co2Driver_getCo2Ppm(co2driver);
	//assert
	EXPECT_TRUE(_t <= 5000 && _t >= 0);
	//act
	_t = co2Driver_getCo2Ppm(co2driver);
	//assert
	EXPECT_TRUE(_t <= 5000 && _t >= 0);
	//act
	_t = co2Driver_getCo2Ppm(co2driver);
	//assert
	EXPECT_TRUE(_t <= 5000 && _t >= 0);
	//act
	_t = co2Driver_getCo2Ppm(co2driver);
	//assert
	EXPECT_TRUE(_t <= 5000 && _t >= 0);
	//act
	_t = co2Driver_getCo2Ppm(co2driver);
	//assert
	EXPECT_TRUE(_t <= 5000 && _t >= 0);
	//act
	_t = co2Driver_getCo2Ppm(co2driver);
	//assert
	EXPECT_TRUE(_t <= 5000 && _t >= 0);
}

TEST_F(Co2DriverTest1, TestTakeMeasuring_returns_co_2_driver_ok_if_everything_went_ok)
{
	//act
	//assert
	EXPECT_EQ(CO2_DRIVER_OK, co2Driver_takeMeasuring(co2driver));
}

TEST_F(Co2DriverTest1, TestInLoop_can_call_in_loop_without_crashing)
{
	//act
	co2Driver_inLoop(co2driver);
}

TEST_F(Co2DriverTest1, TestInLoop_calls_x_event_group_wait_bits_with_the_required_arguments)
{
	//act
	co2Driver_inLoop(co2driver);
	//assert
	EXPECT_EQ(1u, xEventGroupWaitBits_fake.call_count);
	EXPECT_TRUE(NULL == xEventGroupWaitBits_fake.arg0_val);
	EXPECT_EQ(CO2_BIT_0, xEventGroupWaitBits_fake.arg1_val);
	EXPECT_EQ(pdTRUE, xEventGroupWaitBits_fake.arg2_val);
	EXPECT_EQ(pdTRUE, xEventGroupWaitBits_fake.arg3_val);
	EXPECT_EQ(portMAX_DELAY, xEventGroupWaitBits_fake.arg4_val);
}

TEST_F(Co2DriverTest1, TestInLoop_calls_take_measuring_after_a_valid_result)
{
	//arrange
	auto _t = co2Driver_getCo2Ppm(co2driver);
	printf("%d", _t);
	xEventGroupWaitBits_fake.return_val = CO2_BIT_0;
	//act
	co2Driver_inLoop(co2driver);
	//assert
	EXPECT_TRUE(_t != co2Driver_getCo2Ppm(co2driver)); //not a good way of testing, but works
													   //because of low probability to generate
													   //the same number twice in a row
}

//-------------------------------------TESTS TEMPERATURE DRIVER-----------------------------------------------------------
TEST_F(TemperatureDriverTest, TestCreate_calls_create_without_crashing)
{
	//act
	temperatureDriver_create(1, NULL, NULL, NULL);
}

TEST_F(TemperatureDriverTest, TestCreate_callsMallocWithArgument20)
{
	//act
	temperatureDriver_create(2, nullptr, nullptr, nullptr);
	EXPECT_EQ(1u, pvPortMalloc_fake.call_count);
	//assert
	// sizeof(TemperatureDriver) = 20
	EXPECT_EQ(20u, pvPortMalloc_fake.arg0_val);
}

TEST_F(TemperatureDriverTest, TestCreate_returned_co_2_driver_pointer_is_not_null)
{
	//arrange
	pvPortMalloc_fake.return_val = malloc(20);
	//act
	TemperatureDriver_t _tmp = temperatureDriver_create(2, nullptr, nullptr, nullptr);
	//assert
	EXPECT_TRUE(NULL != _tmp);
}

TEST_F(TemperatureDriverTest, TestCreate_callsxTaskCreateWithTheRequiredArguments)
{
	//arrange
	pvPortMalloc_fake.return_val = malloc(20);
	//act
	TemperatureDriver_t _temperatureDriver = temperatureDriver_create(2, nullptr, nullptr, nullptr);
	//assert
	EXPECT_EQ((TaskFunction_t)temperatureSensor_Task, xTaskCreate_fake.arg0_val);
	EXPECT_EQ(0, strcmp("Temperature Task", xTaskCreate_fake.arg1_val));
	EXPECT_EQ(configMINIMAL_STACK_SIZE, xTaskCreate_fake.arg2_val);
	EXPECT_EQ(_temperatureDriver, TemperatureDriver_t(xTaskCreate_fake.arg3_val));
	EXPECT_EQ(configMAX_PRIORITIES - 4, xTaskCreate_fake.arg4_val);
	EXPECT_FALSE(NULL == xTaskCreate_fake.arg5_val);
}

TEST_F(TemperatureDriverTest1, TestCreate_has_last_measurement_as_0)
{
	//assert
	EXPECT_EQ(0, temperatureDriver_getMeasure(temperature_driver));
}