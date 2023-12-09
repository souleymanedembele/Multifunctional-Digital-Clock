/**
 * File              : circular_buffer.h
 * Author            : Souleymane Dembele <sdembele@uw.edu>
 * Date              : 12.09.2023
 * Last Modified Date: 12.09.2023
 * Last Modified By  : Souleymane Dembele <sdembele@uw.edu>
 */

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include "uart.h"

typedef uint8_t portType;

#define BUFFER_SIZE 100
#define INITIAL_VALUE 0
#define DEBUG_PORT 0
#define SLAVE_DEVICE_PORT 1

typedef struct {
  unsigned char buffer[BUFFER_SIZE];
  volatile uint32_t head;
  volatile uint32_t tail;
} circular_buffer_t;

void init_circular_buffer(void);
void buffer_send_str(portType port, char *str);

#endif /* CIRCULAR_BUFFER_H */
