
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

// Include any necessary header files here (assuming NRF24 library is included)
#include "NRF24L01.h"

uint64_t RxpipeAddrs = 0x11223344AA; // This is the address for the receiving pipe (modify as needed)
char myRxData[32];                 // Array to store received data (32 bytes)

void app_main(void) {
  // Initialize the NRF24 transceiver
  NRF24_Init();

  printf("**** RECIVER - NO ACK ****\n\r");  // Print message indicating receiver mode with no acknowledgements

  printf("________________________After change Setting________________________\n\r");
  // Configure NRF24 settings:
  NRF24_set_AutoAck(false);              // Disable automatic acknowledgements
  NRF24_set_Channel(10);                 // Set the channel to 10 (modify if needed)
  NRF24_set_PayloadSize(32);              // Set payload size to 32 bytes
  NRF24_openReadingPipe(1, RxpipeAddrs);   // Open reading pipe 1 with the specified address
  printRadioSettings();                  // Call a function to print the current radio settings (if available)
  NRF24_startListening();                // Start listening for data on the specified pipe

  while (1) {
    // Continuously loop to check for incoming data
    if (NRF24_available()) {
      // Check if there's data available in the receive buffer
      NRF24_read(myRxData, 32);          // Read 32 bytes of data into the myRxData array
      printf("%s\n\r", myRxData);         // Print the received data
    } else {
      printf("No Data !!\r\n");           // Print message if no data is available
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay for 1 second (assuming this is within an RTOS environment)
  }
}

