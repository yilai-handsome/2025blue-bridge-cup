
#include <stc15f2k60s2.h>
#include <intrins.h>
#include <Init.h>
sbit TX = P1^0;
sbit RX = P1^1;


// ����8��40kHz�������ź�
void Send_wave() {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        TX = 1;
        Delay12us();
        TX = 0;
        Delay12us();
    }
}

// ��������ຯ��
unsigned int Measure_Distance() {
    unsigned int time = 0;
    TMOD &= 0x0F;  // ��ʱ��1ģʽ0��13λ�����8192����������
    TH1 = 0x00;
    TL1 = 0x00;

    Send_wave();  // ���ͳ������ź�
    TR1 = 1;      // ������ʱ��

    while ((RX == 1) && (TF1 == 0));  // �ȴ��������źŷ��ػ��ߵȵ�����������Χ
    TR1 = 0;  // ֹͣ��ʱ��

    if (TF1 == 0) {  // ����������Χ
        time = TH1;
        time = (time << 8) | TL1;
        return ((time / 10) * 17) / 100 + 3;
    } else {  // ����������Χ
        TF1 = 0;
        return 999;
    }
}
