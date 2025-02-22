
#include "Led.h"
#include <stc15f2k60s2.h>
 
/* LED��ʾ���� */
void LED_Show(unsigned char addr, enable)
{
    static unsigned char temp_new_LED = 0x00;//ȫ�� 
    static unsigned char temp_old_LED = 0xFF;//ȫ��
    
    if (enable)
        temp_new_LED |= 0x01 << addr; //�Ȱ�λ���ƣ������ұߵ�1�Ƶ�addr��λ�ã�����addr=0�����������ұߣ�
		                                  //�ٰ�λ���ֻ�����ұߵ�1������������
    else                              //temp_new�����0x01
                                         			
        temp_new_LED &= ~(0x01 << addr);
    
    if (temp_new_LED != temp_old_LED)
    {
			
        P0 = ~temp_new_LED; //0xfe     1111 1110
			
        P2 = P2 & 0x1f | 0x80; // P2 ��ص��⼸�δ��빲ͬʵ���˽� P0 �˿ڵ��������浽 LED ���ƶ˿ڵĹ��ܡ�
//0x1f 0001 1111                       //�Ƚ����ݷŵ� P0 �˿ڣ�Ȼ��ͨ������ P2 �˿�ѡ����ʵ����������� P0 ���ݴ����ȥ�����ر���������
			
        P2 &= 0x1f;//100x xxxx & 0001 1111
			            // �ְѸ���λ��������ˣ����·�������Latch��������
        temp_old_LED = temp_new_LED;//temp_old=0x01��¼������ʱLED��״̬������´��Ƿ��б仯
    }
}
 
 
/* �������̵���ר�� */
unsigned char temp_new = 0x00;
unsigned char temp_old = 0xFF;
 
//  0-�ر�   1-����
/* ���������ƺ��� */   
void Beep(unsigned char flag)
{
    if (flag)
        temp_new |= 0x40;//0000 0000 | 0100 0000 =1000 0000
    else
        temp_new &= ~(0x40);//0100 0000 ��λȡ�� 1011 1111 �ٰ�λ& �����0000 0000 
    
    if (temp_new != temp_old)
    {
        P0 = temp_new;
			
        P2 = P2 & 0x1f | 0xa0;// �����5λ����������λ��0xa0=101       0 0000
			                              //1010 0000
			           
        P2 &= 0x1f;                //0001
			
			
        temp_old = temp_new;
    }
}
/* �̵������ƺ��� */
void Relay(unsigned char flag)//relay������ʱ�����˱�ʾ�̵��������н�������������һȺ��
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
