#pragma once
#include <FreeRTOS.h>
#include <stdint.h>
#include <semphr.h>

#define LORA_MAX_PAYLOAD_LENGTH	20

typedef struct lora_payload {
	uint8_t port_no; /**< Port_no the data is received on, or to transmit to */
	uint8_t len; /**< Length of the payload (no of bytes) */
	uint8_t bytes[LORA_MAX_PAYLOAD_LENGTH]; /**< Array to hold the payload to be sent, or that has been received */
} lora_payload_t;

typedef struct LoraDriver* LoraDriver_t;

LoraDriver_t lora_create(QueueHandle_t p_queue_handle, SemaphoreHandle_t pPrintfSemaphore);
void loraDriver_sent_upload_message(lora_payload_t lora_payload, LoraDriver_t lora_driver);
void loraDriver_destroy(LoraDriver_t* self);
void loraDriver_Task(void* pvParameters);
void loraDriver_inLoop(LoraDriver_t self, lora_payload_t lora_payload);