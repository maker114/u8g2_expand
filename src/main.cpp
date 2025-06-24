/**
 * @file main.cpp
 * @author maker114
 * @brief U8g2库动画拓展
 * @version 0.1
 * @date 2025-04-18
 */
#include <Arduino.h>     //引入Arduino库
#include <U8g2lib.h>     //引入U8g2库
#include <SPI.h>         //引入SPI库
#include <U8g2_Expand.h> //引入动画扩展库
#include "driver/gpio.h"
#include "esp_now.h"
#include "WiFi.h"

/* **************************************************************************************** */
/*                                           定义部分                                        */
/* **************************************************************************************** */

// 定义外设引脚
const int LED_B = 15; // 蓝灯
const int LED_R = 2;  // 红灯
#define Button1 21    // 按键1
#define Button2 5     // 按键2
#define Button3 4     // 按键3

// 定义函数
int Key_Scan(void);
void Packaging_1(void);

// 实例化对象
U8G2E_MenuOption MenuOption_ARR[7]{
    {"Text Option1", (int)Packaging_1, U8G2E_OPTION_FUNC},
    {"KEY Option2", 1, U8G2E_OPTION_KEY},
    {"PCT Option3", 0, U8G2E_OPTION_PCT},
    {"NUM Option4", 123.123456, U8G2E_OPTION_NUM},
    {"NUM Option5", 134045, U8G2E_OPTION_NUM},
    {"Text Option6", 1, U8G2E_OPTION_KEY},
    {"Text Option7", 0, U8G2E_OPTION_KEY}};

void setup()
{
  // 初始化外设
  Serial.begin(9600);             // 初始化串口通信
  Serial2.begin(9600);            // 初始化串口通信
  pinMode(LED_B, OUTPUT);         // 设置蓝灯引脚为输出模式
  pinMode(LED_R, OUTPUT);         // 设置红灯引脚为输出模式
  pinMode(Button1, INPUT_PULLUP); // 设置按键1引脚为输入模式
  pinMode(21, INPUT_PULLUP);      // 设置按键2引脚为输入模式
  pinMode(Button3, INPUT_PULLUP); // 设置按键2引脚为输入模式

  U8G2E_Init(true, "U8G2 Expand", "Design by maker114");
  U8G2E_SignKeyFun(Key_Scan);

  // 测试外设
  digitalWrite(LED_B, HIGH); // 蓝灯亮
  digitalWrite(LED_R, HIGH); // 红灯亮
  delay(500);                // 延时半秒
  digitalWrite(LED_B, LOW);  // 蓝灯灭
  digitalWrite(LED_R, LOW);  // 红灯灭
}

float change_H1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
float change_H2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int Time = 0, A = 0;
void loop()
{

  // uint8_t Key_result = Key_Scan();
  U8G2E_MenuDisplay(MenuOption_ARR, 7);
  // u8g2.setFont(u8g2_font_6x10_mf);
  // U8G2E_PromptWindow("Put button 2 to restart menu\n     OK", true);
  // 输出函数指针数据
  // int p1 = (int)Key_Scan;
}

int Key_Scan(void)
{
  static bool Button_Hold = false;
  static int Count = 0;
  if ((digitalRead(Button2) == LOW || digitalRead(Button3) == LOW) && Button_Hold == false)
  {
    Button_Hold = true;
    if (digitalRead(Button2) == LOW)
      return 2;
    if (digitalRead(Button3) == LOW)
      return 3;
  }

  if (digitalRead(Button1) == LOW && Button_Hold == false)
  {
    Button_Hold = true;
    while (digitalRead(Button1) != HIGH && ++Count < 25)
      delay(25);
    uint8_t result = (Count >= 20) ? 4 : 1; // 三目运算符简化判断
    Count = 0;                              // 统一重置计数器
    return result;
  }

  if (digitalRead(Button1) == HIGH && digitalRead(Button2) == HIGH && digitalRead(Button3) == HIGH)
    Button_Hold = false;
  return 0;
}

void Packaging_1(void)
{
  u8g2.setFont(u8g2_font_6x10_mf);
  U8G2E_PromptWindow("Put button 2 to restart menu\n     OK", true);
  u8g2.setFont(u8g2_font_5x7_mf);
  U8G2E_PromptWindow("Put button 1 to restart menu\n     OK", true);
}