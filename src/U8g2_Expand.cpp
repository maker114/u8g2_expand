#include <U8g2_Expand.h> //���붯����չ

/*************************************����֧����ײ�ʵ��***************************************/

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, 14, 12, 13); // ����U8g2��

/**
 * @brief ��ʼ��OLED
 * @param Enable �Ƿ����ÿ�������
 */
void U8G2E_Init(bool Enable)
{
    u8g2.begin();                    // ����OLED
    u8g2.enableUTF8Print();          // ���������ַ�֧��
    u8g2.setFont(u8g2_font_6x12_mf); // ���ó�ʼ����
    if (Enable)
    {
        U8G2E_StartAnimation("U8G2 Expand", "Design by maker114");
    }
}

/**
 * @brief ����ƽ���ƶ�
 *
 * @param GoalValue Ŀ��ֵ
 * @param CurrentValue �����ĵĵ�ǰֵ
 * @note ����������ʱ����Ҫ��ÿһ֡�ĺ����������������ʱ
 */
typedef enum
{
    Slow = 1,
    Elasticity = 2,
} Mode;
void U8G2E_MoveCursor(int GoalValue, float *CurrentValue, uint8_t Mode)
{
    if (Mode == Slow)
    {
        float Error = GoalValue - *CurrentValue; // ������Ŀ��ֵ��ȥʵ��ֵ
        if (fabs(Error) > 1)                     // ������1
        {
            *CurrentValue += Error / 4; // ʵ��ֵ����������10
        }
        else
            *CurrentValue = GoalValue; // ���С��1 ֱ�ӵ���Ŀ��ֵ
    }
    else if (Mode == Elasticity)
    {
        static float Velocity = 0.0f; // �ڲ��洢�ٶȣ���̬������
        const float Stiffness = 0.4f; // ����ϵ�����նȣ����ƻص����ȣ�
        const float Damping = 0.5f;   // ����ϵ�������Ƽ���Ч����

        float Error = GoalValue - *CurrentValue;         // Ŀ���ֵ
        float SpringForce = Error * Stiffness;           // ������������Ŀ�꣩
        float DampingForce = -Velocity * Damping;        // �����������٣�
        float Acceleration = SpringForce + DampingForce; // �ܼ��ٶ�

        Velocity += Acceleration;  // �����ٶ�
        *CurrentValue += Velocity; // ����λ��

        // ����ӽ�Ŀ�����ٶ��㹻С��ֱ����ΪĿ��ֵ
        if (fabs(Error) < 1 && fabs(Velocity) < 0.1f)
        {
            *CurrentValue = GoalValue;
            Velocity = 0.0f; // �����ٶ�
        }
    }
}

/**
 * @brief �����黯
 *
 */
void U8G2E_Blurring(void)
{
    u8g2.setDrawColor(0);
    for (uint8_t i = 0; i < 64; i++)
    {
        for (uint8_t j = i % 2; j < 128; j += 2)
        {
            u8g2.drawPixel(j, i);
        }
    }
    u8g2.setDrawColor(1);
}

/**
 * @brief �Զ���������ַ���
 *
 * @param x x�������
 * @param y y�������
 * @param str �ַ���
 * @param max_width �����
 * @note ��Ҫ���ӳ����ⶨ������
 */
void U8G2E_DrawWrappedText(u8g2_uint_t x, u8g2_uint_t y, const char *str, u8g2_uint_t max_width)
{
    const char *text = str;
    uint8_t Display_width = u8g2.getDisplayWidth();                     // ��ȡ��Ļ���
    max_width = max_width >= Display_width ? Display_width : max_width; // �ж�������Ƿ񳬹���Ļ���
    max_width -= max_width % u8g2.getMaxCharWidth();                    // �����ȡ��
    int X_Cursor = x, Y_Cursor = y + u8g2.getMaxCharHeight();           // ���λ��
    for (uint8_t i = 0; i < strlen(str); i++)                           // ���������ַ���
    {
        if (X_Cursor >= max_width + x)
        {
            X_Cursor = x;
            Y_Cursor += u8g2.getMaxCharHeight();
            if (*text == ' ')
            {
                text++;
                continue; // ���������ո�ֱ������
            }
        }
        if (*text == '\n')
        {
            X_Cursor = x;
            Y_Cursor += u8g2.getMaxCharHeight();
            text++;
        }

        u8g2.setCursor(X_Cursor, Y_Cursor);
        u8g2.printf("%c", *text);
        text++;
        X_Cursor += u8g2.getMaxCharWidth();
    }
}

/**
 * @brief ��ȡ�ַ����ڵ�ǰ����ͻ��г����µĸ߶�
 *
 * @param str �ַ���
 * @param max_width �����
 * @return uint8_t �ַ����߶�
 */
uint8_t U8G2E_StrHight(const char *str, uint8_t max_width, uint8_t x)
{
    const char *text = str;
    uint8_t Display_width = u8g2.getDisplayWidth();                     // ��ȡ��Ļ���
    max_width = max_width >= Display_width ? Display_width : max_width; // �ж�������Ƿ񳬹���Ļ���
    max_width -= max_width % u8g2.getMaxCharWidth();                    // �����ȡ��
    int X_Cursor = x, Y_Cursor = u8g2.getMaxCharHeight();               // ���λ��
    for (uint8_t i = 0; i < strlen(str); i++)                           // ���������ַ���
    {
        if (X_Cursor >= max_width + x)
        {
            X_Cursor = x;
            Y_Cursor += u8g2.getMaxCharHeight();
            if (*text == ' ')
            {
                text++;
                continue; // ���������ո�ֱ������
            }
        }
        if (*text == '\n')
        {
            X_Cursor = x;
            Y_Cursor += u8g2.getMaxCharHeight();
            text++;
        }
        text++;
        X_Cursor += u8g2.getMaxCharWidth();
    }
    return Y_Cursor;
}

/**
 * @brief ���滺����
 *
 */
uint8_t Buffer_Backups[8 * 64 * 128]; // ���������ݣ���СΪ8 * u8g2.getBufferTileHeight() * u8g2.getBufferTileWidth()
void U8G2E_SaveBuffer(void)
{
    uint8_t *p = u8g2.getBufferPtr();
    for (size_t i = 0; i < 8 * u8g2.getBufferTileHeight() * u8g2.getBufferTileWidth(); i++)
    {
        Buffer_Backups[i] = *p;
        p++;
    }
}

/**
 * @brief �ָ�������
 *
 */
void U8G2E_CoverBuffer(void)
{
    uint8_t *p = u8g2.getBufferPtr();
    for (size_t i = 0; i < 8 * u8g2.getBufferTileHeight() * u8g2.getBufferTileWidth(); i++)
    {
        *p = Buffer_Backups[i];
        p++;
    }
}

/**
 * @brief ע�ᰴ������
 * @note  ʹ������U8G2E_SignKeyFun(key2_scan_alone);
 */
typedef int (*Key_scan_fun)(void);
Key_scan_fun KEY_Scan;
void U8G2E_SignKeyFun(int Put_in_fun(void))
{
    KEY_Scan = Put_in_fun;
}
typedef enum
{
    KEY_NULL = 0,    // �޷���
    KEY_DOWN = 3,    // ��������
    KEY_UP = 2,      // ��������
    KEY_CONFIRM = 1, // ����ȷ��
} Key_mapping;       // ����ӳ��

/*************************************��������***************************************/

/**
 * @brief ��������
 *
 * @param str1 �����⣬���10���ַ�
 * @param str2 �����⣬���17���ַ�
 *
 */
void U8G2E_StartAnimation(const char *str1, const char *str2)
{
    float y_data = 0, x_data = 64;
    do
    {
        u8g2.clearBuffer();
        u8g2.drawLine(64, 32, 64, 32 + y_data);
        u8g2.drawLine(64, 32, 64, 32 - y_data);
        U8G2E_MoveCursor(15, &y_data, Slow);
        u8g2.sendBuffer();
        delay(20);
    } while (y_data < 15);
    delay(200);
    do
    {
        u8g2.clearBuffer();
        u8g2.setClipWindow(x_data - 1, 32 - 15, 128 - x_data + 1, 32 + 15);
        u8g2.drawLine(x_data, 32 + 15, x_data, 32 - 15);
        u8g2.setFont(u8g2_font_10x20_t_arabic);
        u8g2.setCursor(x_data + 5, 32 - 15 + 15);
        u8g2.printf(str1);
        u8g2.setFont(u8g2_font_profont12_mf);
        u8g2.setCursor(x_data + 6, 32 + 13);
        u8g2.printf(str2);
        U8G2E_MoveCursor(5, &x_data, Slow);
        u8g2.sendBuffer();
        delay(20);
    } while (x_data != 5);
    u8g2.setMaxClipWindow();
    delay(200);
}

/**
 * @brief ������ʾ
 *
 * @param str1 ��ʾ����
 * @note ������ʾ������ɵ�����������С�������볤�ȱ任
 */
void U8G2E_PromptWindow(const char *str1, bool Key_trigger_enable)
{
    uint8_t MaxWight = 80; // �ַ��������
    uint8_t Window_W = MaxWight - MaxWight % u8g2.getMaxCharWidth() + 14;
    uint8_t Window_X = (int)((128 - Window_W) / 2);
    uint8_t Window_H = U8G2E_StrHight(str1, MaxWight, Window_X + 5) + 10;
    uint8_t Window_Y = (int)((64 - Window_H) / 2);
    float y_Cursor = 64;
    U8G2E_SaveBuffer();
    do // ����
    {
        u8g2.clearBuffer();
        U8G2E_CoverBuffer();
        U8G2E_Blurring();
        u8g2.setDrawColor(0);
        u8g2.drawBox(Window_X, y_Cursor, Window_W, Window_H);
        u8g2.setDrawColor(1);
        u8g2.drawFrame(Window_X, y_Cursor, Window_W, Window_H);
        U8G2E_DrawWrappedText(Window_X + 7, y_Cursor + 3, str1, MaxWight);
        U8G2E_MoveCursor(Window_Y, &y_Cursor, Elasticity);
        delay(15);
        u8g2.sendBuffer();
    } while (y_Cursor != Window_Y);

    if (Key_trigger_enable)
        while (KEY_Scan() == KEY_NULL) // ��������˳�
            ;
    else
        delay(1000);

    Window_Y = 100;
    do // �½�
    {
        u8g2.clearBuffer();
        U8G2E_CoverBuffer();
        u8g2.setDrawColor(0);
        u8g2.drawBox(Window_X, y_Cursor, Window_W, Window_H);
        u8g2.setDrawColor(1);
        u8g2.drawFrame(Window_X, y_Cursor, Window_W, Window_H);
        U8G2E_DrawWrappedText(Window_X + 7, y_Cursor + 3, str1, MaxWight);
        U8G2E_MoveCursor(Window_Y, &y_Cursor, Slow);
        delay(10);
        u8g2.sendBuffer();
    } while (y_Cursor != Window_Y);
}

/**
 * @brief ˿��������ʾ����
 *
 * @param num ����ʾ������
 * @param x ���ֵ�x����
 * @param y ���ֵ�y����
 * @param change ���ֶ�Ӧ������
 * @param W ���ֵĿ��
 * @param H ���ֵĸ߶�
 */
void U8G2E_NUMDisplay(int num, int x, int y, float change[], int W, int H)
{
    static const uint8_t digit_patterns[10] = {0x7D, 0x50, 0x37, 0x57, 0x5A, 0x4F, 0x6F, 0x51, 0x7F, 0x5F};
    if (num < 0 || num > 9)
        return; // ������Χ������ʾ
    uint8_t pattern = digit_patterns[num];
    for (int i = 0; i < 7; i++)
    {
        if (i >= 0 && i <= 2)
            U8G2E_MoveCursor((pattern & (1 << i)) ? W : 0, &change[i], Slow);
        if (i >= 3 && i <= 6)
            U8G2E_MoveCursor((pattern & (1 << i)) ? H : 0, &change[i], Slow);
    }
    U8G2E_MoveCursor(H, &change[7], Slow);           // ������
    u8g2.drawHLine(x + 1, y, change[0]);             // �Ϻ���
    u8g2.drawHLine(x + 1, y + H, change[1]);         // �к���
    u8g2.drawHLine(x + 1, y + 2 * H - 1, change[2]); // �º���
    u8g2.drawVLine(x + 1, y, change[3]);             // ��������
    u8g2.drawVLine(x + W, y, change[4]);             // ��������
    u8g2.drawVLine(x + 1, y + H, change[5]);         // ��������
    u8g2.drawVLine(x + W, y + H, change[6]);         // ��������
}
