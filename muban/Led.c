
#include "Led.h"
#include <stc15f2k60s2.h>
 
/* LED显示函数 */
void LED_Show(unsigned char addr, enable)
{
    static unsigned char temp_new_LED = 0x00;//全亮 
    static unsigned char temp_old_LED = 0xFF;//全灭
    
    if (enable)
        temp_new_LED |= 0x01 << addr; //先按位左移，把最右边的1移到addr的位置，比如addr=0，则还是在最右边；
		                                  //再按位异或，只有最右边的1保留了下来；
    else                              //temp_new变成了0x01
                                         			
        temp_new_LED &= ~(0x01 << addr);
    
    if (temp_new_LED != temp_old_LED)
    {
			
        P0 = ~temp_new_LED; //0xfe     1111 1110
			
        P2 = P2 & 0x1f | 0x80; // P2 相关的这几段代码共同实现了将 P0 端口的数据锁存到 LED 控制端口的功能。
//0x1f 0001 1111                       //先将数据放到 P0 端口，然后通过设置 P2 端口选择合适的锁存器，让 P0 数据传输过去，最后关闭锁存器，
			
        P2 &= 0x1f;//100x xxxx & 0001 1111
			            // 又把高三位给清除掉了，无事发生，把Latch重新锁好
        temp_old_LED = temp_new_LED;//temp_old=0x01记录下来此时LED的状态，检测下次是否有变化
    }
}
 
 
/* 蜂鸣器继电器专用 */
unsigned char temp_new = 0x00;
unsigned char temp_old = 0xFF;
 
//  0-关闭   1-开启
/* 蜂鸣器控制函数 */   
void Beep(unsigned char flag)
{
    if (flag)
        temp_new |= 0x40;//0000 0000 | 0100 0000 =1000 0000
    else
        temp_new &= ~(0x40);//0100 0000 按位取反 1011 1111 再按位& 则等于0000 0000 
    
    if (temp_new != temp_old)
    {
        P0 = temp_new;
			
        P2 = P2 & 0x1f | 0xa0;// 清除低5位，保留高三位，0xa0=101       0 0000
			                              //1010 0000
			           
        P2 &= 0x1f;                //0001
			
			
        temp_old = temp_new;
    }
}
/* 继电器控制函数 */
void Relay(unsigned char flag)//relay作名词时，除了表示继电器，还有接力赛、接力的一群人
{
    if (flag)
        temp_new |= 0x10; //0001 0000
    else
        temp_new &= ~(0x10);
    
    if (temp_new != temp_old)
    {
        P0 = temp_new;
			
			
			
        P2 = P2 & 0x1f | 0xa0;
        P2 &= 0x1f;
			
			
			
			
			
		
        temp_old = temp_new;
    }
}
