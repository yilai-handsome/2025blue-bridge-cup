#include <stc15f2k60s2.h>

#include "Key.h"
 
/* ¾ØÕó°´¼üKBD */
unsigned char Key_Read()
{
    unsigned char temp = 0;
    ET0 = 0;
    P34 = 0; P35 = 1; P42 = 1; P44 = 1;
    if (P30 == 0) temp = 3;
    if (P31 == 0) temp = 6;
    if (P32 == 0) temp = 9;
    if (P33 == 0) temp = 12;
    P34 = 1; P35 = 0; P42 = 1; P44 = 1;
    if (P30 == 0) temp = 2;
    if (P31 == 0) temp = 5;
    if (P32 == 0) temp = 8;
    if (P33 == 0) temp = 11;
    P34 = 1; P35 = 1; P42 = 0; P44 = 1;
    if (P30 == 0) temp = 1;
    if (P31 == 0) temp = 4;
    if (P32 == 0) temp = 7;
    if (P33 == 0) temp = 10;
    P34 = 1; P35 = 1; P42 = 1; P44 = 0;
    if (P30 == 0) temp = 7;
    if (P31 == 0) temp = 6;
    if (P32 == 0) temp = 5;
    if (P33 == 0) temp = 4;
    P3 = 0xff;
    ET0 = 1;
    return temp;
}
 
/* ¶ÀÁ¢°´¼üBTN */
/*
unsigned char Key_Read()
{
    unsigned char temp = 0;
    if (P30 == 0) temp = 7;
    if (P31 == 0) temp = 6;
    if (P32 == 0) temp = 5;
    if (P33 == 0) temp = 4;
    return temp;
}
*/