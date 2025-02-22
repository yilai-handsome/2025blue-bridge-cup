#ifndef _IIC_H
#define _IIC_H

void I2CStart(void); 
void I2CStop(void); 
unsigned char I2CWaitAck(void); 
void I2CSendAck(unsigned char ackbit);
void I2CSendByte(unsigned char byt);
unsigned char I2CReceiveByte(void); 

#endif
