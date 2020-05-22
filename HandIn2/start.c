#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>

#include "AppController.h"

AppController_t app_controller;

static const uint8_t TEMPERATURE_PORT_NO = 1;
static const uint8_t CO2_PORT_NO = 2;

main_()
{
	printf("Started the application...\n");

	app_controller = appController_create(CO2_PORT_NO, TEMPERATURE_PORT_NO);

	vTaskStartScheduler();

	for (;;);
}