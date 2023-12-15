#ifndef I2C0_H_
#define I2C0_H_

#include <stdint.h>
void I2C0_Init(void);
void I2C0_ByteRead(char slaveAddr, char regAddr, char *data);
void I2C0_BurstRead(char slaveAddr, char regAddr, char *data, int length);
void I2C0_ByteWrite(char slaveAddr, char regAddr, char data);
void I2C0_BurstWrite(char slaveAddr, char regAddr, char *data, int length);
#endif /* I2C0_H_ */