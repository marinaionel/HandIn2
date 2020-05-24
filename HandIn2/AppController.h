#pragma once
#include <stdint.h>

typedef struct AppController* AppController_t;

AppController_t appController_create(uint8_t co2PortNo, uint8_t temperaturePortNo);
void appController_destroy(AppController_t* self);
void appController_inLoop(AppController_t app_controller);
void appController_Task(void* pvParameters);