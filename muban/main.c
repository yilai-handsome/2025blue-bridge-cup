#include <stc15f2k60s2.h>
#include <intrins.h>
#include "Init.h"
#include "Key.h"
#include "Seg.h"
#include "LED.h"
#include "ds1302.h"
#include "iic.h"
#include "onewire.h"
#include "ultrasonic.h"
/* �������������� */
unsigned int Slow_Down;                                                 //����ɨ��ר��
bit Key_Slow_Flag, Seg_Slow_Flag, LED_Slow_Flag, Infor_Slow_Flag,Distance_Slow_Flag;       //���������LED��Ϣ������ٱ�־λ
unsigned char Seg_Buf[8] = {1, 1, 1, 1, 1, 1, 1, 1};            //�������ʾ������
unsigned char Seg_Point[8] = {0, 1, 0, 0, 0, 0, 0, 0};                  //�����С������ʾ������
unsigned char LED_Buf[8] = {0, 0, 0, 0, 0, 0, 0, 0};                    //LED��ʾ������
unsigned char i;                                                        //ѭ��ר�ñ���
unsigned int decimal_part;
unsigned int int_part;
/* ���������� */
float temp; 
unsigned char rtc[3] = {12, 30, 0}; // ��ʼ��ʱ��Ϊ12:30:00

/* ���������� */
void Key_Deal()
{
    static unsigned char Key_New = 0, Key_Old = 0;    //������ȡ
    static unsigned char Key_UP = 0, Key_Down = 0;    //ר�ñ���
    
    if (Key_Slow_Flag) return;                //����
    Key_Slow_Flag = 1;
    
    Key_New = Key_Read();                        //��ȡ����
    Key_UP = ~Key_New & (Key_New ^ Key_Old);     //����������
    Key_Down = Key_New & (Key_New ^ Key_Old);    //�����½���
    Key_Old = Key_New;                           //����ɨ��
    
    switch (Key_Down)
    {
       
				 case 3:
           Seg_Buf[1]=2;
        break;
				 case 4:
					 Seg_Buf[3]=4;
        break;
    }
}
 
/* ����ܴ����� */
void Seg_Deal()
{
 if(Seg_Slow_Flag) return;
	Seg_Slow_Flag=1;
	
   temp=DS18B20_Read_T();
	
   int_part = (unsigned int)temp;  // ��ȡ��������
   decimal_part = (unsigned int)((temp - int_part) * 100);  // ��ȡС������

  		Delay12us();
    Seg_Buf[0] = int_part / 10;  // ʮλ
    Seg_Buf[1] = int_part % 10;  // ��λ
    Seg_Buf[2] = decimal_part / 10;  // ʮ��λ
    Seg_Buf[3] = decimal_part % 10;  // �ٷ�λ
		Delay12us();
	DS1302_Read_Rtc(rtc);
	
	
	
	Seg_Buf[4] = rtc[1] / 10;
	Seg_Buf[5] = rtc[1] % 10;
	Seg_Buf[6] = rtc[2] / 10;
    Seg_Buf[7] = rtc[2] % 10;
	
	Delay12us();
	

}
 
/* LED������ */
void LED_Deal()
{
    if (LED_Slow_Flag) return;        //LED���٣����Բ�Ҫ������Ŀ���������ˮ��֮���
    LED_Slow_Flag = 1;                //������Ӧ�䣬�Ѽ���ȥ��
    LED_Show(1,1);
	LED_Show(3,1);
	LED_Show(5,1);
	LED_Show(7,1);
}
 
/* ��Ϣ������ */
void Infor_Deal()
{
    /* ��Ϣ����� */
 
 
    /* ���� */
    if (Infor_Slow_Flag) return;
    Infor_Slow_Flag = 1;
    
    /* ��Ϣ������ */
    
}
 
/* ��ʱ����ʼ������ */
void Timer1_Init(void)      //1����@12.000MHz
{
   
    TMOD &= 0x0F;       //���ö�ʱ��ģʽ
    TL1 = 0x18;         //���ö�ʱ��ֵ
    TH1 = 0xFC;         //���ö�ʱ��ֵ
    TF1 = 0;            //���TF1��־
    TR1 = 1;            //��ʱ��1��ʼ��ʱ
    EA  = 1;
    ET1 = 1;
}
 
/* �жϷ����� */
void Timer1_Serve() interrupt 3
{
	
//	  if(++chaoshengbo==10) distance=Uwave_Read();
    if (++Slow_Down == 400) Slow_Down = Infor_Slow_Flag = 0;
    if (Slow_Down % 10 == 0) Key_Slow_Flag = 0;
    if (Slow_Down % 20 == 0) LED_Slow_Flag = 0;
    if (Slow_Down % 100 == 0) Seg_Slow_Flag = 0;
	 if (Slow_Down % 50 == 0)Distance_Slow_Flag=0;
    Seg_Show(Slow_Down%8, Seg_Buf[Slow_Down%8], Seg_Point[Slow_Down%8]);
    LED_Show(Slow_Down%8, LED_Buf[Slow_Down%8]);
    
}
 //�ⲿ�ж�0��ʼ������
void IT0_Init(void)
{
    EX0 = 1;    //�����ⲿ�ж�0����
    EA=1;       //�����ж�������
    IT0=1;      //���ô�����ʽΪ�½��ش�����Ϊ0ʱ������ʽΪ�½��غ������ض�����
    PX0 = 0;    //���õ����ȼ���Ϊ1ʱΪ�����ȼ�
}


/* ������ */
void main()
{
    System_Init();
    Timer1_Init();
	  IT0_Init();//�ⲿ�ж�
	 DS1302_Write_Rtc(rtc);
	
    while (1)
    {
	
        Infor_Deal();
        Key_Deal();
        Seg_Deal();
        LED_Deal();
				Delay12us();
		
		}
			
}

/*void Time_Gone()//�ⲿ�жϵĴ����¼�������������ʱ��P3ģ���½��ش����ⲿ�ж�
{
    unsigned char temp1 = rtc[2];
	Delay12us();
	 
    if(temp1!=rtc[2])
    {
        // ģ���½��ش����ⲿ�ж�
        P3 &= 0xFE;  // P3.2����
      Delay12us();
			Delay12us();
        P3 |= 0x01;  // P3.2����
    }
}
*/
void External_Hander0() interrupt 0
{
    //��������
		LED_Show(0,1);//�������ⲿ�жϵ�����ʱ��LED������
}



