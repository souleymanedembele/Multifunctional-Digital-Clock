/**
 * File              : circular_buffer.c
 * Author            : Souleymane Dembele <sdembele@uw.edu>
 * Date              : 12.09.2023
 * Last Modified Date: 12.09.2023
 * Last Modified By  : Souleymane Dembele <sdembele@uw.edu>
 */

#include "circular_buffer.h"
#include <stdint.h>

portType debug_port = 0;
portType slave_device_port = 1;

// Buffer for Slave Device UART
circular_buffer_t rx_buffer1 = {
    {INITIAL_VALUE},
    INITIAL_VALUE,
    INITIAL_VALUE,
}; // RX Buffer for Slave Device
circular_buffer_t tx_buffer1 = {
    {INITIAL_VALUE},
    INITIAL_VALUE,
    INITIAL_VALUE,
}; // TX Buffer for Slave Device

// Buffer for Debug UART
circular_buffer_t rx_buffer2 = {
    {INITIAL_VALUE},
    INITIAL_VALUE,
    INITIAL_VALUE,
}; // RX Buffer for debug UART
circular_buffer_t tx_buffer2 = {
    {INITIAL_VALUE},
    INITIAL_VALUE,
    INITIAL_VALUE,
}; // TX Buffer for debug UART

// Define pointers to the buffers
circular_buffer_t *rx_buffer1_ptr;
circular_buffer_t *tx_buffer1_ptr;
circular_buffer_t *rx_buffer2_ptr;
circular_buffer_t *tx_buffer2_ptr;

// Initialize the circular buffer
void init_circular_buffer(void) {
  // Initialize the buffer for slave device pointer
  rx_buffer1_ptr = &rx_buffer1;
  tx_buffer1_ptr = &tx_buffer1;

  // Initialize the buffer for debug pointer
  rx_buffer2_ptr = &rx_buffer2;
  tx_buffer2_ptr = &tx_buffer2;

  // Initialize the RX interrupt flag already set in uart.c
}

static void buffer_store_char(circular_buffer_t *buffer, unsigned char c) {
  int i = (buffer->head + 1) % BUFFER_SIZE;

  // Check if the buffer is full at index location i
  if (i != buffer->tail) {
    buffer->buffer[buffer->head] = c; // update the head
    buffer->head = i;
  }
}

void buffer_clear(portType port) {
  if (port == slave_device_port) {
    // set content to '\0'
    memset(rx_buffer1_ptr->buffer, '\0', BUFFER_SIZE);
    rx_buffer1_ptr->head = INITIAL_VALUE;
  } else if (port == debug_port) {
    // set content to '\0'
    memset(rx_buffer2_ptr->buffer, '\0', BUFFER_SIZE);
    rx_buffer2_ptr->head = INITIAL_VALUE;
  }
}

// check the next value in the buffer without removing it
int buffer_peek(portType port) {
  if (port == slave_device_port) {
    if (rx_buffer1_ptr->head == rx_buffer1_ptr->tail) {
      // an error occured
      return -1;
    }
    return rx_buffer1_ptr->buffer[rx_buffer1_ptr->tail];
  } else if (port == debug_port) {
    if (rx_buffer2_ptr->head == rx_buffer2_ptr->tail) {
      // an error occured
      return -1;
    }
    return rx_buffer2_ptr->buffer[rx_buffer2_ptr->tail];
  } else {
    // an error occured
    return -1;
  }
}

// function to read and remove the next value in the buffer
int buffer_read(portType port) {
  if (port == slave_device_port) {
    // if the head isn't ahead of the tail, we don't have any characters
    if (rx_buffer1_ptr->head == rx_buffer1_ptr->tail) {
      // an error occured
      return -1;
    }
    unsigned char c = rx_buffer1_ptr->buffer[rx_buffer1_ptr->tail];
    rx_buffer1_ptr->tail = (rx_buffer1_ptr->tail + 1) % BUFFER_SIZE;
    return c;
  } else if (port == debug_port) {
    // if the head isn't ahead of the tail, we don't have any characters
    if (rx_buffer2_ptr->head == rx_buffer2_ptr->tail) {
      // an error occured
      return -1;
    }
    unsigned char c = rx_buffer2_ptr->buffer[rx_buffer2_ptr->tail];
    rx_buffer2_ptr->tail = (rx_buffer2_ptr->tail + 1) % BUFFER_SIZE;
    return c;
  } else {
    // an error occured
    return -1;
  }
}

// function to write a value to the buffer
void buffer_write(portType port, unsigned char c) {
  if (port == slave_device_port) {
    int loc = (tx_buffer1_ptr->head + 1) % BUFFER_SIZE;
    // check if the buffer is full
    while (loc == tx_buffer1_ptr->tail) {
      // wait for the buffer to free up
    }
    buffer_store_char(rx_buffer1_ptr, c);
    // Enable the TX interrupt
  } else if (port == debug_port) {
    int loc = (tx_buffer2_ptr->head + 1) % BUFFER_SIZE;
    // check if the buffer is full
    while (loc == tx_buffer2_ptr->tail) {
      // wait for the buffer to free up
    }
    buffer_store_char(rx_buffer2_ptr, c);
    // Enable the TX interrupt
  }
}

// Function to check if there is data in the buffer
int is_data(portType port) {
  if (port == slave_device_port) {
    return (uint32_t)(BUFFER_SIZE + rx_buffer1_ptr->head -
                      rx_buffer1_ptr->tail) %
           BUFFER_SIZE; // check if the buffer is empty
  } else if (port == debug_port) {
    return (uint32_t)(BUFFER_SIZE + rx_buffer2_ptr->head -
                      rx_buffer2_ptr->tail) %
           BUFFER_SIZE; // check if the buffer is empty
  } else {
    // an error occured
    return -1;
  }
}

// Get first character in the buffer
static void get_first_char(char *str) {
  // make sure the buffer is not empty
  while (!is_data(slave_device_port)) {
    // wait for the buffer to be filled
  }
  while (buffer_peek(slave_device_port) != str[0]) {
    rx_buffer1_ptr->tail = (uint16_t)(rx_buffer1_ptr->tail + 1) % BUFFER_SIZE;
    while (!is_data(slave_device_port)) {
      // wait for the buffer to be filled
    }
  }
}

// FUnction to check if a certain response is present in the buffer
int is_response(char *str) {
  int current_index = 0;
  int str_length = strlen(str);

  while (current_index != str_length) {
    current_index = 0;

    get_first_char(str);

    while (buffer_peek(slave_device_port) == str[current_index]) {
      current_index++;
      buffer_read(slave_device_port);
      if (current_index == str_length) {
        // the response is present in the buffer successfully
        return 1;
      }
      while (!is_data(slave_device_port)) {
        // wait for the buffer to be filled
      }
    }
  }
  return -1;
}

// Function to get specified number of characters from the buffer
void get_strs(uint8_t num_of_chars, char *dest_buffer) {
  for (int i = 0; i < num_of_chars; i++) {
    for (int j = 0; j < 9000; j++) {
      // wait for the buffer to be filled
    }
    dest_buffer[i] = buffer_read(slave_device_port);
  }
}

// Function to send a string to a buffer
void buffer_send_str(portType port, char *str) {
  int i = 0;
  while (str[i] != '\0') {
    buffer_write(port, str[i]);
    i++;
  }
}

void slave_device_uart_callback(void) {
  if ((UART1_FR_R & (1 << 4)) && (UART1_RIS_R & (1 << 4))) { // RXRIS and RXFE
    UART1_ICR_R |= (1 << 4);                                 // Clear RXIC
    char c = UART1_DR_R;
    buffer_store_char(rx_buffer1_ptr, c);
  }

  if ((UART1_FR_R & (1 << 5)) && (UART1_RIS_R & (1 << 5))) { // TXRIS and TXFF
    if (tx_buffer1_ptr->head == tx_buffer1_ptr->tail) {
      // Disable the TX interrupt
      UART1_IM_R &= ~(1 << 5);
    } else {
      unsigned char c = tx_buffer1.buffer[tx_buffer1.tail];
      // update the position
      tx_buffer1.tail = (tx_buffer1.tail + 1) % BUFFER_SIZE;
      // transmit the character
      UART1_DR_R = c;
    }
  }
}

void debug_uart_callback(void) {
  if ((UART0_FR_R & (1 << 4)) && (UART0_RIS_R & (1 << 4))) { // RXRIS and RXFE
    UART0_ICR_R |= (1 << 4);                                 // Clear RXIC
    char c = UART0_DR_R;
    buffer_store_char(rx_buffer2_ptr, c);
  }

  if ((UART0_FR_R & (1 << 5)) && (UART0_RIS_R & (1 << 5))) { // TXRIS and TXFF
    if (tx_buffer2_ptr->head == tx_buffer2_ptr->tail) {
      // Disable the TX interrupt
      UART0_IM_R &= ~(1 << 5);
    } else {
      unsigned char c = tx_buffer2.buffer[tx_buffer2.tail];
      // update the position
      tx_buffer2.tail = (tx_buffer2.tail + 1) % BUFFER_SIZE;
      // transmit the character
      UART0_DR_R = c;
    }
  }
}

void UART1_Handler(void) { slave_device_uart_callback(); }

int UARTWorks = 0;
void UART0_Handler(void) {

  UARTWorks = 1;
  debug_uart_callback();
}
