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

// Define a 64-bit unsigned integer to store the RX pipe address
uint64_t RxpipeAddrs = 0x11223344AA;

// Declare character arrays to store received data and acknowledgement payload
char myRxData[32];
char myAckPayload[32] = "Ack by ESP32S3 !!";

void app_main(void) {

  // Initialize the NRF24 transceiver
  NRF24_Init();

  // Print a message indicating receiver mode with no acknowledgements by default
  printf("**** RECEIVER - ACK ****\n\r");

  printf("________________________After change Setting________________________\n\r");

  // Configure NRF24 settings:
  // - Enable automatic acknowledgements (modified from the original code)
  NRF24_set_AutoAck(true);  

  // Set the channel to 10 (modify if needed)
  NRF24_set_Channel(10);

  // Set payload size to 32 bytes
  NRF24_set_PayloadSize(32);

  // Open reading pipe 1 with the specified address
  NRF24_openReadingPipe(1, RxpipeAddrs);

  // Enable dynamic payloads (which allows for variable payload sizes)
  NRF24_enableDynamicPayloads();

  // Enable acknowledgements for received data (modified from the original code)
  NRF24_enable_AckPayload(); 

  // Print the current radio settings (if available)
  printRadioSettings();

  // Start listening for data on the specified pipe
  NRF24_startListening();

  while (1) {
    // Continuously loop to check for incoming data
    if (NRF24_available()) {
      // Check if there's data available in the receive buffer
      NRF24_read(myRxData, 32);  // Read 32 bytes of data into the myRxData array

      // Send acknowledgement payload back to the sender
      NRF24_write_AckPayload(1, myAckPayload, 32);

      // Print the received data
      printf("%s\n\r", myRxData);
    } else {
      // Print message if no data is available
      printf("No Data !!\r\n");
    }

    // Delay for 1 second (assuming this is within an RTOS environment)
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}


