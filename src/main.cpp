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

/* **************************************************************************************** */
/*                                           ���岿��                                        */
/* **************************************************************************************** */

// ������������
const int LED_B = 15;   // ����
const int LED_R = 2;    // ���
const int Button1 = 4;  // ����1
const int Button2 = 16; // ����2
const int Button3 = 17; // ����3

// ���庯��
int Key_Scan(void);

// ʵ��������

void setup()
{
  // ��ʼ������
  Serial.begin(9600);             // ��ʼ������ͨ��
  pinMode(LED_B, OUTPUT);         // ������������Ϊ���ģʽ
  pinMode(LED_R, OUTPUT);         // ���ú������Ϊ���ģʽ
  pinMode(Button1, INPUT_PULLUP); // ���ð���1����Ϊ����ģʽ
  pinMode(Button2, INPUT_PULLUP); // ���ð���2����Ϊ����ģʽ
  pinMode(Button3, INPUT_PULLUP); // ���ð���3����Ϊ����ģʽ
  U8G2E_Init(true);
  U8G2E_SignKeyFun(Key_Scan);

  // ��������
  digitalWrite(LED_B, HIGH); // ������
  digitalWrite(LED_R, HIGH); // �����
  delay(500);                // ��ʱ����
  digitalWrite(LED_B, LOW);  // ������
  digitalWrite(LED_R, LOW);  // �����
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