/**
 * @file main.cpp
 * @author maker114
 * @brief U8g2�⶯����չ
 * @version 0.1
 * @date 2025-04-18
 */
#include <Arduino.h>     //����Arduino��
#include <U8g2lib.h>     //����U8g2��
#include <SPI.h>         //����SPI��
#include <U8g2_Expand.h> //���붯����չ��
#include "driver/gpio.h"

/* **************************************************************************************** */
/*                                           ���岿��                                        */
/* **************************************************************************************** */

// ������������
const int LED_B = 15; // ����
const int LED_R = 2;  // ���
#define Button1 21    // ����1
#define Button2 5     // ����2
#define Button3 4     // ����3

// ���庯��
int Key_Scan(void);

// ʵ��������
U8G2E_MenuOption MenuOption_ARR[7]{
    {"Text Option1", 0, U8G2E_OPTION_TEXT},
    {"KEY Option2", 1, U8G2E_OPTION_KEY},
    {"PCT Option3", 0, U8G2E_OPTION_PCT},
    {"NUM Option4", 123.123456, U8G2E_OPTION_NUM},
    {"NUM Option5", 134045, U8G2E_OPTION_NUM},
    {"Text Option6", 1, U8G2E_OPTION_KEY},
    {"Text Option7", 0, U8G2E_OPTION_KEY}};

void setup()
{
  // ��ʼ������
  Serial.begin(9600);             // ��ʼ������ͨ��
  Serial2.begin(9600);            // ��ʼ������ͨ��
  pinMode(LED_B, OUTPUT);         // ������������Ϊ���ģʽ
  pinMode(LED_R, OUTPUT);         // ���ú������Ϊ���ģʽ
  pinMode(Button1, INPUT_PULLUP); // ���ð���1����Ϊ����ģʽ
  pinMode(21, INPUT_PULLUP);      // ���ð���2����Ϊ����ģʽ
  pinMode(Button3, INPUT_PULLUP); // ���ð���2����Ϊ����ģʽ

  U8G2E_Init(true, "U8G2 Expand", "Design by maker114");
  U8G2E_SignKeyFun(Key_Scan);

  // ��������
  digitalWrite(LED_B, HIGH); // ������
  digitalWrite(LED_R, HIGH); // �����
  delay(500);                // ��ʱ����
  digitalWrite(LED_B, LOW);  // ������
  digitalWrite(LED_R, LOW);  // �����
}

float change_H1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
float change_H2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int Time = 0, A = 0;
void loop()
{

  // uint8_t Key_result = Key_Scan();
  U8G2E_MenuDisplay(MenuOption_ARR, 7);

  /*u8g2.setFont(u8g2_font_6x10_mf);
  U8G2E_PromptWindow("Please put button 1", true);
  delay(200);
  u8g2.setFont(u8g2_font_5x8_mf);
  U8G2E_PromptWindow("Just wait", false);
  delay(200);
  u8g2.setFont(u8g2_font_4x6_mf);
  U8G2E_PromptWindow("Please put button 1digitalRead(Button2) again", true);
  delay(200);*/
  //  u8g2.clearBuffer();
  //  u8g2.setFont(u8g2_font_6x10_mf);
  //  u8g2.drawVLine(90, 0, U8G2E_StrHight("warning:\nChannel 1 has been disconnected", 80, 10));
  //  U8G2E_DrawWrappedText(10, 0, "warning:\nChannel 1 has been disconnected", 80);
  //  u8g2.sendBuffer();
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
    uint8_t result = (Count >= 20) ? 4 : 1; // ��Ŀ��������ж�
    Count = 0;                              // ͳһ���ü�����
    return result;
  }

  if (digitalRead(Button1) == HIGH && digitalRead(Button2) == HIGH && digitalRead(Button3) == HIGH)
    Button_Hold = false;
  return 0;
}