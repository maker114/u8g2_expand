/**
 * @file U8g2_Expand.h
 * @author maker114
 * @brief U8G2库扩展头文件
 * @version 0.1
 * @date 2025-06-23
 *
 *
 */
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

#define U8G2E_PIN_CS 14
#define U8G2E_PIN_DC 12
#define U8G2E_PIN_RST 13

/**
 * @brief 按键映射枚举。
 * @details 定义按键扫描函数可能返回的键值，用于菜单交互。
 */
typedef enum
{
    KEY_NULL = 0,    ///< 无按键
    KEY_DOWN = 3,    ///< 向下按键
    KEY_UP = 2,      ///< 向上按键
    KEY_CONFIRM = 1, ///< 确认按键
    KEY_EXIT = 4     ///< 取消按键
} Key_mapping;

/**
 * @brief 菜单选项结构体。
 * @details 存储一个菜单项的所有信息，包括标题、数值、类型和坐标，用于菜单显示和交互。
 * @note 函数初始化使用例：{"Text Option1", (int)Packaging_1, U8G2E_OPTION_FUNC},
 */
typedef struct
{
    String Title;       ///< 选项标题（显示文本）
    double Value;       ///< 选项数值（开关用0/1，百分比/数值为实际值，函数类型存储函数指针的整型值）
    uint8_t Kind;       ///< 选项类型，取值见 U8G2E_OptionType 枚举
    float X_Coordinate; ///< X轴坐标（用于菜单动画和定位，初始化无需赋值）
    float Y_Coordinate; ///< Y轴坐标（用于菜单动画和定位，初始化无需赋值）
} U8G2E_MenuOption;

/**
 * @brief 菜单选项类型枚举。
 * @details 定义菜单项的行为类型，决定其显示样式和交互方式。
 */
typedef enum
{
    U8G2E_OPTION_KEY = 0,  ///< 开关类型（切换ON/OFF）
    U8G2E_OPTION_NUM = 1,  ///< 数值类型（整数/小数可调）
    U8G2E_OPTION_PCT = 2,  ///< 百分比类型（0~100%可调）
    U8G2E_OPTION_TEXT = 3, ///< 纯文本类型（仅显示，无交互）
    U8G2E_OPTION_FUNC = 4, ///< 函数类型（执行回调函数，Value存储函数指针）
} U8G2E_OptionType;

/**
 * @brief 动画移动模式枚举。 
 * @details 定义数字平滑移动的两种运动曲线，用于 U8G2E_MoveCursor 函数。
 */
typedef enum
{
    Slow = 1,       ///< 缓动模式（逐渐减速逼近目标）
    Elasticity = 2, ///< 弹性模式（带回弹效果，模拟弹簧运动）
} Mode;

/***********************函数声明部分**********************/
void U8G2E_Init(bool Enable, const char *str1, const char *str2);
void U8G2E_MoveCursor(int GoalValue, float *CurrentValue, uint8_t Mode);
void U8G2E_Blurring(void);
void U8G2E_DrawWrappedText(u8g2_uint_t x, u8g2_uint_t y, const char *str, u8g2_uint_t max_width);
uint8_t U8G2E_StrHeight(const char *str, uint8_t max_width, uint8_t x);
void U8G2E_SaveBuffer(void);
void U8G2E_CoverBuffer(void);
void U8G2E_SignKeyFun(int Put_in_fun(void));
void U8G2E_CountDigits(double number, int *integer_digits, int *decimal_digits);
void U8G2E_StartAnimation(const char *str1, const char *str2);
void U8G2E_PromptWindow(const char *str1, bool Key_trigger_enable);
void U8G2E_NUMDisplay(int num, int x, int y, float change[], int W, int H);
void U8G2E_MenuDisplay(U8G2E_MenuOption MenuOption_ARR[], uint8_t valid_num);
void U8G2E_MenuOptionDisplay(U8G2E_MenuOption MenuOption_Member);
void U8G2E_MenuExecute(U8G2E_MenuOption *MenuOption_Member);
void U8G2E_PCT_ACTION(U8G2E_MenuOption *MenuOption_Member);
void U8G2E_NUM_ACTION(U8G2E_MenuOption *MenuOption_Member);
#endif