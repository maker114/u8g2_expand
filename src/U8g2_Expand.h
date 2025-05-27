#ifndef _U8G2EXPAND_
#define _U8G2EXPAND_
#include <Arduino.h>
#include <U8g2lib.h>

/*
U8G2�����׺��������
<purpose>	������;����
t	͸�����壬��ʹ�ñ���ɫ
h	�������ξ�����ͬ�߶�
m	�������ξ�����ͬ�߶ȺͿ�ȣ��ȿ����壩
8	������������8x8���ؿ�

<char set>	�ַ�������
f	����������256�����Σ������ַ�����
r	���������ASCII��32��127��Χ���ַ�����׼ASCII�ַ�����
u	���������ASCII��32��95��Χ���ַ�����д��ĸ�ַ�����
n	������������ּ����ڱ�д���ں�ʱ���ַ����Ķ�������
...	�����Զ����ַ��б�

(�Ƽ���׺��mf)
*/
extern U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2; // ����U8g2��

/***********************���ò���**********************/
typedef enum // ���尴��ӳ��
{
    KEY_NULL = 0,    // �޷���
    KEY_DOWN = 3,    // ����
    KEY_UP = 1,      // ����
    KEY_CONFIRM = 5, // ȷ��
    KEY_CANCEL = 4   // ȡ��
} Key_mapping;

typedef struct // �˵�ѡ��ṹ��
{
    String Title;       // ѡ�����
    float Value;        // ѡ����ֵ
    uint8_t Kind;       // ѡ������
    float X_Coordinate; // x������
    float Y_Coordinate; // y������
} U8G2E_MenuOption;

typedef enum // �˵�ѡ������
{
    U8G2E_OPTION_KEY = 0,  // ��������
    U8G2E_OPTION_NUM = 1,  // ��ֵ���ͣ�����/С����
    U8G2E_OPTION_PCT = 2,  // �ٷֱ����ͣ�percent��
    U8G2E_OPTION_TEXT = 3, // �ı�����(����ʾ�ı���û�����ݸ���)
} U8G2E_OptionType;

typedef enum // ������ʾģʽ
{
    Slow = 1,
    Elasticity = 2,
} Mode;
/***********************������������**********************/
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