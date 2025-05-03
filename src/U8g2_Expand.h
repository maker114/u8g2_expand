#ifndef _U8G2EXPAND_
#define _U8G2EXPAND_
#include <Arduino.h>
#include <U8g2lib.h>

/*
U8G2字体后缀命名规则：
<purpose>	字体用途描述
t	透明字体，不使用背景色
h	所有字形具有相同高度
m	所有字形具有相同高度和宽度（等宽字体）
8	所有字形适配8x8像素框

<char set>	字符集描述
f	字体包含最多256个字形（完整字符集）
r	字体仅包含ASCII码32至127范围的字符（标准ASCII字符集）
u	字体仅包含ASCII码32至95范围的字符（大写字母字符集）
n	字体仅包含数字及用于编写日期和时间字符串的额外字形
...	其他自定义字符列表

(推荐后缀：mf)
*/

extern U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2; // 定义U8g2库

void U8G2E_Init(bool Enable);
uint8_t U8G2E_StrHight(const char *str, uint8_t max_width, uint8_t x);
void U8G2E_SaveBuffer(void);
void U8G2E_CoverBuffer(void);
void U8G2E_SignKeyFun(int Put_in_fun(void));
void U8G2E_StartAnimation(const char *str1, const char *str2);
void U8G2E_PromptWindow(const char *str1, bool Key_trigger_enable);
void U8G2E_NUMDisplay(int num, int x, int y, float change[], int W, int H);
void U8G2E_Blurring(void);
void U8G2E_DrawWrappedText(u8g2_uint_t x, u8g2_uint_t y, const char *str, u8g2_uint_t max_width);
void U8G2E_MoveCursor(int GoalValue, float *CurrentValue, uint8_t Mode);

#endif