/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include <esp_system.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "NRF24L01.h"

// Define the transmission pipe address (64-bit)
uint64_t TxpipeAddrs = 0x11223344AA;

// Define the data to be transmitted (32 bytes)
char myTxData[32] = "Hello World!";

// Define a buffer to store the received acknowledgement payload (32 bytes)
char AckPayload[32];

void app_main(void) {

  // Initialize the NRF24L01 module
  NRF24_Init();

  printf("                   //**** TRANSMIT - ACK ****//  \n\r");
  printf("________________________After change Setting________________________ \n\r");

  // Stop listening for incoming data (we're transmitting)
  NRF24_stopListening();

  // Set the transmission pipe address
  NRF24_openWritingPipe(TxpipeAddrs);

  // Enable automatic acknowledgements (expecting ACK from receiver)
  NRF24_set_AutoAck(true);

  // Set the radio channel (frequency)
  NRF24_set_Channel(10);

  // Set the payload size for transmission (32 bytes)
  NRF24_set_PayloadSize(32);

  // Enable dynamic payloads (allows variable payload sizes)
  NRF24_enableDynamicPayloads();

  // Enable receiving acknowledgement payloads
  NRF24_enable_AckPayload();

  // Print the current radio settings for debugging (optional)
  printRadioSettings();

  while (1) {
    /* code */

    // Attempt to transmit the data
    if (NRF24_write(myTxData, 32)) {
      // Read the received acknowledgement payload (if any)
      NRF24_read(AckPayload, 32);
      printf("AckPayload: %s \n\r", AckPayload);
      printf("Send Ok !!\r\n");
    } else {
      printf("Send Not Ok !!\r\n");
    }

    // Delay for 1 second before next attempt
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}


