
#include <stc15f2k60s2.h>
#include <intrins.h>
#include <Init.h>
sbit TX = P1^0;
sbit RX = P1^1;


// 产生8个40kHz超声波信号
void Send_wave() {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        TX = 1;
        Delay12us();
        TX = 0;
        Delay12us();
    }
}

// 超声波测距函数
unsigned int Measure_Distance() {
    unsigned int time = 0;
    TMOD &= 0x0F;  // 定时器1模式0，13位，最大8192个计数脉冲
    TH1 = 0x00;
    TL1 = 0x00;

    Send_wave();  // 发送超声波信号
    TR1 = 1;      // 启动定时器

    while ((RX == 1) && (TF1 == 0));  // 等待超声波信号返回或者等到测量超出范围
    TR1 = 0;  // 停止定时器

    if (TF1 == 0) {  // 正常测量范围
        time = TH1;
        time = (time << 8) | TL1;
        return ((time / 10) * 17) / 100 + 3;
    } else {  // 超出测量范围
        TF1 = 0;
        return 999;
    }
}
