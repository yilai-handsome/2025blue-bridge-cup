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
/* 基础变量创建区 */
unsigned int Slow_Down;                                                 //减速扫描专用
bit Key_Slow_Flag, Seg_Slow_Flag, LED_Slow_Flag, Infor_Slow_Flag,Distance_Slow_Flag;       //按键数码管LED信息处理减速标志位
unsigned char Seg_Buf[8] = {1, 1, 1, 1, 1, 1, 1, 1};            //数码管显示缓冲区
unsigned char Seg_Point[8] = {0, 1, 0, 0, 0, 0, 0, 0};                  //数码管小数点显示缓冲区
unsigned char LED_Buf[8] = {0, 0, 0, 0, 0, 0, 0, 0};                    //LED显示缓冲区
unsigned char i;                                                        //循环专用变量
unsigned int decimal_part;
unsigned int int_part;
/* 变量创建区 */
float temp; 
unsigned char rtc[3] = {12, 30, 0}; // 初始化时间为12:30:00

/* 按键处理函数 */
void Key_Deal()
{
    static unsigned char Key_New = 0, Key_Old = 0;    //按键获取
    static unsigned char Key_UP = 0, Key_Down = 0;    //专用变量
    
    if (Key_Slow_Flag) return;                //减速
    Key_Slow_Flag = 1;
    
    Key_New = Key_Read();                        //获取键码
    Key_UP = ~Key_New & (Key_New ^ Key_Old);     //捕获上升沿
    Key_Down = Key_New & (Key_New ^ Key_Old);    //捕获下降沿
    Key_Old = Key_New;                           //辅助扫描
    
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
 
/* 数码管处理函数 */
void Seg_Deal()
{
 if(Seg_Slow_Flag) return;
	Seg_Slow_Flag=1;
	
   temp=DS18B20_Read_T();
	
   int_part = (unsigned int)temp;  // 提取整数部分
   decimal_part = (unsigned int)((temp - int_part) * 100);  // 提取小数部分

  		Delay12us();
    Seg_Buf[0] = int_part / 10;  // 十位
    Seg_Buf[1] = int_part % 10;  // 个位
    Seg_Buf[2] = decimal_part / 10;  // 十分位
    Seg_Buf[3] = decimal_part % 10;  // 百分位
		Delay12us();
	DS1302_Read_Rtc(rtc);
	
	
	
	Seg_Buf[4] = rtc[1] / 10;
	Seg_Buf[5] = rtc[1] % 10;
	Seg_Buf[6] = rtc[2] / 10;
    Seg_Buf[7] = rtc[2] % 10;
	
	Delay12us();
	

}
 
/* LED处理函数 */
void LED_Deal()
{
    if (LED_Slow_Flag) return;        //LED减速，可以不要，但题目如果出现流水灯之类的
    LED_Slow_Flag = 1;                //大家灵活应变，把减速去掉
    LED_Show(1,1);
	LED_Show(3,1);
	LED_Show(5,1);
	LED_Show(7,1);
}
 
/* 信息处理函数 */
void Infor_Deal()
{
    /* 信息输出区 */
 
 
    /* 减速 */
    if (Infor_Slow_Flag) return;
    Infor_Slow_Flag = 1;
    
    /* 信息处理区 */
    
}
 
/* 定时器初始化函数 */
void Timer1_Init(void)      //1毫秒@12.000MHz
{
   
    TMOD &= 0x0F;       //设置定时器模式
    TL1 = 0x18;         //设置定时初值
    TH1 = 0xFC;         //设置定时初值
    TF1 = 0;            //清除TF1标志
    TR1 = 1;            //定时器1开始计时
    EA  = 1;
    ET1 = 1;
}
 
/* 中断服务函数 */
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
 //外部中断0初始化函数
void IT0_Init(void)
{
    EX0 = 1;    //开启外部中断0允许
    EA=1;       //开启中断总允许
    IT0=1;      //设置触发方式为下降沿触发，为0时触发方式为下降沿和上升沿都触发
    PX0 = 0;    //设置低优先级，为1时为高优先级
}


/* 主函数 */
void main()
{
    System_Init();
    Timer1_Init();
	  IT0_Init();//外部中断
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

/*void Time_Gone()//外部中断的触发事件，当满足条件时，P3模拟下降沿触发外部中断
{
    unsigned char temp1 = rtc[2];
	Delay12us();
	 
    if(temp1!=rtc[2])
    {
        // 模拟下降沿触发外部中断
        P3 &= 0xFE;  // P3.2拉低
      Delay12us();
			Delay12us();
        P3 |= 0x01;  // P3.2拉高
    }
}
*/
void External_Hander0() interrupt 0
{
    //函数内容
		LED_Show(0,1);//当满足外部中断的条件时，LED灯亮起
}



