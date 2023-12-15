#include "i2c.h"
// #include "tm4c123gh6pm.h"
#define SYSCTL_RCGCI2C_R        (*((volatile uint32_t *)0x400FE620))
#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))
#define GPIO_PORTB_AFSEL_R      (*((volatile uint32_t *)0x40005420))
#define GPIO_PORTB_ODR_R        (*((volatile uint32_t *)0x4000550C))
#define GPIO_PORTB_DEN_R        (*((volatile uint32_t *)0x4000551C))
#define GPIO_PORTB_PCTL_R       (*((volatile uint32_t *)0x4000552C))
#define GPIO_PORTB_AMSEL_R      (*((volatile uint32_t *)0x40005528))
#define I2C0_MSA_R              (*((volatile uint32_t *)0x40020000))
#define I2C0_MCS_R              (*((volatile uint32_t *)0x40020004))
#define I2C0_MDR_R              (*((volatile uint32_t *)0x40020008))
#define I2C0_MTPR_R             (*((volatile uint32_t *)0x4002000C))
#define I2C0_MCR_R              (*((volatile uint32_t *)0x40020020))
#define SYSCTL_PRGPIO_R (*((volatile uint32_t *)0x400FEA08))


#define I2C_MCS_ACK             0x00000008  // Data Acknowledge Enable
#define I2C_MCS_DATACK          0x00000008  // Acknowledge Data
#define I2C_MCS_ADRACK          0x00000004  // Acknowledge Address
#define I2C_MCS_STOP            0x00000004  // Generate STOP
#define I2C_MCS_START           0x00000002  // Generate START
#define I2C_MCS_ERROR           0x00000002  // Error
#define I2C_MCS_RUN             0x00000001  // I2C Master Enable
#define I2C_MCS_BUSY            0x00000001  // I2C Busy
#define I2C_MCR_MFE             0x00000010  // I2C Master Function Enable


void I2C0_Init(void) {
  SYSCTL_RCGCI2C_R |= 0x0001;           // activate I2C0
  SYSCTL_RCGCGPIO_R |= 0x0002;          // activate port B
  while((SYSCTL_PRGPIO_R&0x0002) == 0){};// ready?

  GPIO_PORTB_AFSEL_R |= 0x0C;           // 3) enable alt funct on PB2,3
  GPIO_PORTB_ODR_R |= 0x08;             // 4) enable open drain on PB3 only
  GPIO_PORTB_DEN_R |= 0x0C;             // 5) enable digital I/O on PB2,3
                                        // 6) configure PB2,3 as I2C
  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFFF00FF)+0x00003300;
  GPIO_PORTB_AMSEL_R &= ~0x0C;          // 7) disable analog functionality on PB2,3
  I2C0_MCR_R = I2C_MCR_MFE;      // 9) master function enable
  I2C0_MTPR_R = 24;              // 8) configure for 100 kbps clock //((16 Mhz)/(2 * (6 - 4) * (400 kHz)) - 1 = 1
}


void I2C0_ByteRead(char slaveAddr, char regAddr, char *data) {

    // Wait until I2C0 bus is not busy
    while (I2C0_MCS_R & I2C_MCS_BUSY) {}

    // Set slave address with write bit for register address setup
    I2C0_MSA_R = (slaveAddr << 1) & 0xFE;

    // Set register address to read from
    I2C0_MDR_R = regAddr;

    // Start condition with RUN but without STOP to continue transaction
    I2C0_MCS_R = I2C_MCS_START | I2C_MCS_RUN;

    // Wait for the transfer to complete
    while(I2C0_MCS_R & I2C_MCS_BUSY) {}

    // Check for errors (NACK, etc.)
    if(I2C0_MCS_R & (I2C_MCS_DATACK | I2C_MCS_ADRACK | I2C_MCS_ERROR)) {
        I2C0_MCS_R = I2C_MCS_STOP; // Send STOP condition in case of error
        return; // Exit the function if error occurred
    }

    // Send repeated START condition with slave address and read bit
    I2C0_MSA_R = (slaveAddr << 1) | 0x01;

    // Continue transaction to read data
    I2C0_MCS_R = I2C_MCS_STOP | I2C_MCS_START | I2C_MCS_RUN;

    // Wait for the transfer to complete
    while(I2C0_MCS_R & I2C_MCS_BUSY) {}

    // Check for errors (NACK, etc.)
    if(I2C0_MCS_R & (I2C_MCS_DATACK | I2C_MCS_ADRACK | I2C_MCS_ERROR)) {
        return; // Exit the function if error occurred
    }

    // Read data
    *data = I2C0_MDR_R;
}


void I2C0_BurstRead(char slaveAddr, char regAddr, char *data, int length) {
    if(length <= 0) return;

    // Wait until I2C0 bus is not busy
    while (I2C0_MCS_R & I2C_MCS_BUSY) {}

    // Set slave address with write bit for register address setup
    I2C0_MSA_R = (slaveAddr << 1) & 0xFE;

    // Set register address to start reading from
    I2C0_MDR_R = regAddr;

    // Start condition with RUN but without STOP to continue transaction
    I2C0_MCS_R = I2C_MCS_START | I2C_MCS_RUN;

    // Wait for the transfer to complete
    while(I2C0_MCS_R & I2C_MCS_BUSY) {}

    // Check for errors (NACK, etc.)
    if(I2C0_MCS_R & (I2C_MCS_DATACK | I2C_MCS_ADRACK | I2C_MCS_ERROR)) {
        I2C0_MCS_R = I2C_MCS_STOP; // Send STOP condition in case of error
        return; // Exit the function if error occurred
    }

    // Send repeated START condition with slave address and read bit
    I2C0_MSA_R = (slaveAddr << 1) | 0x01;

    for (int i = 0; i < length; i++) {
        // If it's the last byte, send NACK and STOP condition after read
        if (i == (length - 1)) {
            I2C0_MCS_R = I2C_MCS_STOP | I2C_MCS_RUN;
        } else {
            // Else send ACK to continue reading
            I2C0_MCS_R = I2C_MCS_ACK | I2C_MCS_RUN;
        }

        // Wait for the transfer to complete
        while(I2C0_MCS_R & I2C_MCS_BUSY) {}

        // Check for errors (NACK, etc.)
        if(I2C0_MCS_R & (I2C_MCS_DATACK | I2C_MCS_ADRACK | I2C_MCS_ERROR)) {
            I2C0_MCS_R = I2C_MCS_STOP; // Send STOP condition in case of error
            return; // Exit the function if error occurred
        }

        // Read data
        data[i] = I2C0_MDR_R;
    }
}

void I2C0_ByteWrite(char slaveAddr, char regAddr, char data) {
    // Wait until I2C0 bus is not busy
    while(I2C0_MCS_R & I2C_MCS_BUSY) {}

    // Set slave address with write bit
    I2C0_MSA_R = (slaveAddr << 1) & 0xFE;

    // Set register address
    I2C0_MDR_R = regAddr;

    // Start condition with RUN but without STOP to continue transaction
    I2C0_MCS_R = I2C_MCS_START | I2C_MCS_RUN;

    // Wait for the transfer to complete
    while(I2C0_MCS_R & I2C_MCS_BUSY) {}

    // Check for errors (NACK, etc.)
    if(I2C0_MCS_R & (I2C_MCS_DATACK | I2C_MCS_ADRACK | I2C_MCS_ERROR)) {
        I2C0_MCS_R = I2C_MCS_STOP; // Send STOP condition in case of error
        return; // Exit the function if error occurred
    }

    // Set data to write
    I2C0_MDR_R = data & 0xFF;

    // Continue transmission with STOP condition
    I2C0_MCS_R = I2C_MCS_STOP | I2C_MCS_RUN;

    // Wait for transmission done
    while(I2C0_MCS_R & I2C_MCS_BUSY) {}

    // Check for errors (NACK, etc.)
    if(I2C0_MCS_R & (I2C_MCS_DATACK | I2C_MCS_ADRACK | I2C_MCS_ERROR)) {
        I2C0_MCS_R = I2C_MCS_STOP; // Send STOP condition in case of error
        return; // Exit the function if error occurred
    }

    // No return value needed if you don't need to report success/failure
    // You may modify to return an error code or status if necessary
}


void I2C0_BurstWrite(char slaveAddr, char regAddr, char *data, int length) {
    // Wait until I2C0 bus is not busy
    while (I2C0_MCS_R & I2C_MCS_BUSY) {}

    // Transmit slave address with write bit
    I2C0_MSA_R = (slaveAddr << 1) & 0xFE;

    // Set register address to start writing to
    I2C0_MDR_R = regAddr;

    // Start condition with RUN
    I2C0_MCS_R = I2C_MCS_START | I2C_MCS_RUN;

    // Wait for the transfer to complete
    while (I2C0_MCS_R & I2C_MCS_BUSY) {}

    // Check if any error occurred
    if (I2C0_MCS_R & (I2C_MCS_DATACK | I2C_MCS_ADRACK | I2C_MCS_ERROR)) {
        I2C0_MCS_R = I2C_MCS_STOP;  // Send STOP condition
        return;  // Error occurred, exit the function
    }

    // Write each byte in the burst
    for (int i = 0; i < length; i++) {
        // Set data to write
        I2C0_MDR_R = data[i];

        // If it's the last byte, prepare to send STOP condition
        if (i == (length - 1)) {
            I2C0_MCS_R = I2C_MCS_STOP | I2C_MCS_RUN;
        } else {
            I2C0_MCS_R = I2C_MCS_RUN;
        }

        // Wait for the transfer to complete
        while (I2C0_MCS_R & I2C_MCS_BUSY) {}

        // Check if any error occurred
        if (I2C0_MCS_R & (I2C_MCS_DATACK | I2C_MCS_ADRACK | I2C_MCS_ERROR)) {
            I2C0_MCS_R = I2C_MCS_STOP;  // Send STOP condition
            return;  // Error occurred, exit the function
        }
    }
}
