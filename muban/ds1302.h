#ifndef __DS1302_H
#define __DS1302_H

void Write_Ds1302(unsigned  char temp);
void Write_Ds1302_Byte( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302_Byte ( unsigned char address );
void DS1302_Read_Rtc(unsigned char *pRtc);
void DS1302_Write_Rtc(unsigned char *pRtc);
//����������Ҫ���ã���д��ʱ�䣬�ٶ���ʱ��
#endif
