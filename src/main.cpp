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

/* **************************************************************************************** */
/*                                           定义部分                                        */
/* **************************************************************************************** */

// 定义外设引脚
const int LED_B = 15;   // 蓝灯
const int LED_R = 2;    // 红灯
const int Button1 = 4;  // 按键1
const int Button2 = 16; // 按键2
const int Button3 = 17; // 按键3

// 定义函数
int Key_Scan(void);

// 实例化对象

void setup()
{
  // 初始化外设
  Serial.begin(9600);             // 初始化串口通信
  pinMode(LED_B, OUTPUT);         // 设置蓝灯引脚为输出模式
  pinMode(LED_R, OUTPUT);         // 设置红灯引脚为输出模式
  pinMode(Button1, INPUT_PULLUP); // 设置按键1引脚为输入模式
  pinMode(Button2, INPUT_PULLUP); // 设置按键2引脚为输入模式
  pinMode(Button3, INPUT_PULLUP); // 设置按键3引脚为输入模式
  U8G2E_Init(true);
  U8G2E_SignKeyFun(Key_Scan);

  // 测试外设
  digitalWrite(LED_B, HIGH); // 蓝灯亮
  digitalWrite(LED_R, HIGH); // 红灯亮
  delay(500);                // 延时半秒
  digitalWrite(LED_B, LOW);  // 蓝灯灭
  digitalWrite(LED_R, LOW);  // 红灯灭
  printf("Downloaded Successfully\r\n");
  // u8g2.clear();
}

float change_H1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
float change_H2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int Time = 0, A = 0;
void loop()
{

  u8g2.setFont(u8g2_font_6x10_mf);
  U8G2E_PromptWindow("Please put button 1", true);
  delay(200);
  u8g2.setFont(u8g2_font_5x8_mf);
  U8G2E_PromptWindow("Just wait", false);
  delay(200);
  u8g2.setFont(u8g2_font_4x6_mf);
  U8G2E_PromptWindow("Please put button 1 again", true);
  delay(200);

  //  u8g2.clearBuffer();
  //  u8g2.setFont(u8g2_font_6x10_mf);
  //  u8g2.drawVLine(90, 0, U8G2E_StrHight("warning:\nChannel 1 has been disconnected", 80, 10));
  //  U8G2E_DrawWrappedText(10, 0, "warning:\nChannel 1 has been disconnected", 80);
  //  u8g2.sendBuffer();
}

int Key_Scan(void)
{
  static bool Button_Hold = false;
  if ((digitalRead(Button1) == LOW || digitalRead(Button2) == LOW || digitalRead(Button3) == LOW) && Button_Hold == false)
  {
    Button_Hold = true;
    if (digitalRead(Button1) == LOW)
      return 1;
    if (digitalRead(Button2) == LOW)
      return 2;
    if (digitalRead(Button3) == LOW)
      return 3;
  }
  if (digitalRead(Button1) == HIGH && digitalRead(Button2) == HIGH && digitalRead(Button3) == HIGH)
    Button_Hold = false;
  return 0;
}