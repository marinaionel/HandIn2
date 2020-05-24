#include "packageHandler.h"

static int16_t co2;
static int16_t temperature;

void packageHandler_setCo2(int16_t p_co2)
{
	co2 = p_co2;
}

void packageHandler_setTemperature(int16_t p_temperature)
{
	temperature = p_temperature;
}

lora_payload_t packageHandler_getLoraPayload()
{
	lora_payload_t _payload = { .len = 4, .bytes = {0}, .port_no = 1 };
	_payload.bytes[0] = co2 >> 8;
	_payload.bytes[1] = co2 & 0xFF;
	_payload.bytes[2] = temperature >> 8;
	_payload.bytes[3] = temperature & 0xFF;
	return _payload;
}