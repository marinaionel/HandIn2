#pragma once
#include <stdint.h>
#include "loraDriver.h"

void packageHandler_setCo2(int16_t p_co2);
void packageHandler_setTemperature(int16_t p_temperature);
lora_payload_t packageHandler_getLoraPayload();