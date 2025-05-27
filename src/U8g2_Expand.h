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

/***********************设置部分**********************/
typedef enum // 定义按键映射
{
    KEY_NULL = 0,    // 无返回
    KEY_DOWN = 3,    // 向下
    KEY_UP = 1,      // 向上
    KEY_CONFIRM = 5, // 确认
    KEY_CANCEL = 4   // 取消
} Key_mapping;

typedef struct // 菜单选项结构体
{
    String Title;       // 选项标题
    float Value;        // 选项数值
    uint8_t Kind;       // 选项种类
    float X_Coordinate; // x轴坐标
    float Y_Coordinate; // y轴坐标
} U8G2E_MenuOption;

typedef enum // 菜单选项类型
{
    U8G2E_OPTION_KEY = 0,  // 开关类型
    U8G2E_OPTION_NUM = 1,  // 数值类型（整数/小数）
    U8G2E_OPTION_PCT = 2,  // 百分比类型（percent）
    U8G2E_OPTION_TEXT = 3, // 文本类型(仅显示文本，没有数据更改)
} U8G2E_OptionType;

typedef enum // 动画显示模式
{
    Slow = 1,
    Elasticity = 2,
} Mode;
/***********************函数声明部分**********************/
uint8_t U8G2E_StrHight(const char *str, uint8_t max_width, uint8_t x);
void U8G2E_SaveBuffer(void);
void U8G2E_CoverBuffer(void);
void U8G2E_SignKeyFun(int Put_in_fun(void));
void U8G2E_StartAnimation(const char *str1, const char *str2);
void U8G2E_PromptWindow(const char *str1, bool Key_trigger_enable);
void U8G2E_NUMDisplay(int num, int x, int y, float change[], int W, int H);
void U8G2E_MenuDisplay(U8G2E_MenuOption MenuOption_ARR[], uint8_t valid_num);
void U8G2E_MenuDisplay(U8G2E_MenuOption MenuOption_Member);
void U8G2E_MenuExecute(U8G2E_MenuOption *MenuOption_Member);
void U8G2E_Blurring(void);
void U8G2E_DrawWrappedText(u8g2_uint_t x, u8g2_uint_t y, const char *str, u8g2_uint_t max_width);
void U8G2E_Init(bool Enable, const char *str1, const char *str2);
void U8G2E_MoveCursor(int GoalValue, float *CurrentValue, uint8_t Mode);

#endif