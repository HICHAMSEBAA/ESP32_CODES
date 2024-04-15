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

// Define the acknowledgement payload (32 bytes)
char myAckPayload[32] = "Ack by ESP32S3 !!";

void app_main(void) {

  // Initialize the NRF24L01 module
  NRF24_Init();
 
  printf("                   //**** TRANSMIT - NO_ACK ****//                   \n\r");
  printf("________________________After change Setting________________________ \n\r");

  // Stop listening for incoming data (we're transmitting)
  NRF24_stopListening();

  // Set the transmission pipe address
  NRF24_openWritingPipe(TxpipeAddrs);

  // Disable automatic acknowledgements (no ACK expected)
  NRF24_set_AutoAck(false);

  // Set the radio channel (frequency)
  NRF24_set_Channel(10);

  // Set the payload size for transmission (32 bytes)
  NRF24_set_PayloadSize(32);

  // Print the current radio settings for debugging (optional)
  printRadioSettings();

  while (1) {
    /* code */

    // Attempt to transmit the data
    if (NRF24_write(myTxData, 32)) {
      printf("Send Ok !! \r\n");
    } else {
      printf("Send Not Ok !!\r\n");
    }

    // Delay for 1 second before next attempt
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

