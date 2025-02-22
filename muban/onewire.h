#ifndef __ONEWIRE_H
#define __ONEWIRE_H

#include <stc15f2k60s2.h>

bit init_ds18b20(void);
void Write_DS18B20(unsigned char dat);
unsigned char Read_DS18B20(void);
float DS18B20_Read_T();
#endif
