#include <stc15f2k60s2.h>
#include "Seg.h"
 
code unsigned char Seg_Table[] =
{
    0xc0, //0 1010 0000
    0xf9, //1
    0xa4, //2
    0xb0, //3
    0x99, //4
    0x92, //5
    0x82, //6
    0xf8, //7
    0x80, //8
    0x90, //9
    0xff, //熄灭
    0xbf, //-
    0x88, //A
    0x83, //b
    0xc6, //C
    0xa1, //d
    0x86, //E
    0x8e  //F
};
 
void Seg_Show(unsigned char addr, Table, Point)
{
    P0 = 0xff;                  //消影
    P2 = P2 & 0x1f | 0xe0;
    P2 &= 0x1f;
    
    P0 = 0x01 << addr;          //位选
    P2 = P2 & 0x1f | 0xc0;
    P2 &= 0x1f;
    
    P0 = Seg_Table[Table];      //段选
    if (Point)
        P0 &= 0x7f;             //小数点
    P2 = P2 & 0x1f | 0xe0;
    P2 &= 0x1f;
}
