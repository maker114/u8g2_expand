#include <U8g2_Expand.h> //���붯����չ

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
void Move_Cursor(int GoalValue, float *CurrentValue)
{
    float Error = GoalValue - *CurrentValue; // ������Ŀ��ֵ��ȥʵ��ֵ
    if (fabs(Error) > 1)                     // ������1
    {
        *CurrentValue += Error / 4; // ʵ��ֵ����������10
    }
    else
        *CurrentValue = GoalValue; // ���С��1 ֱ�ӵ���Ŀ��ֵ
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
    int X_Cursor = x, Y_Cursor = y + u8g2.getMaxCharHeight();           // ���λ��
    for (uint8_t i = 0; i < strlen(str); i++)                           // ���������ַ���
    {
        // FIXME:���к��һ���ַ���һ��
        if (X_Cursor > max_width + x || *text == '\n')
        {
            X_Cursor = x;
            Y_Cursor += u8g2.getMaxCharHeight();
            if (*text == ' ')
            {
                text++;
                continue; // ���������ո�ֱ������
            }
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
    int X_Cursor = x, Y_Cursor = 0;                                     // ���λ��
    for (uint8_t i = 0; i < strlen(str); i++)                           // ���������ַ���
    {
        if (X_Cursor > max_width || *text == '\n')
        {
            X_Cursor = x;
            Y_Cursor += u8g2.getMaxCharHeight();
            if (*text == ' ')
            {
                text++;
                continue; // ���������ո�ֱ������
            }
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
        Move_Cursor(15, &y_data);
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
        Move_Cursor(5, &x_data);
        u8g2.sendBuffer();
        delay(20);
    } while (x_data > 5);
    u8g2.setMaxClipWindow();
    delay(200);
}

/**
 * @brief ������ʾ
 *
 * @param str1 ��ʾ����
 * @note ������ʾ������ɵ�����������С�������볤�ȱ任
 */
void U8G2E_PromptWindow(const char *str1)
{
    // TODO: ����ѡ��λ��ʹ��ʱ������ʱ�˳���ʧ��ʱ���������˳�
    uint8_t MaxWight = 80; // �ַ��������
    uint8_t Window_W = MaxWight + 20;
    uint8_t Window_X = (int)((128 - Window_W) / 2);
    uint8_t Window_H = U8G2E_StrHight(str1, MaxWight, Window_X + 5) + 10;
    uint8_t Window_Y = (int)((64 - Window_H) / 2);
    float y_Cursor = 64;
    // FIXME:�����������һ�лᳬ���߿�
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
        Move_Cursor(Window_Y, &y_Cursor);
        delay(10);
        u8g2.sendBuffer();
    } while (y_Cursor > Window_Y);
    delay(2000);
    Window_Y = 65;
    do // �½�
    {
        u8g2.clearBuffer();
        U8G2E_CoverBuffer();
        u8g2.setDrawColor(0);
        u8g2.drawBox(Window_X, y_Cursor, Window_W, Window_H);
        u8g2.setDrawColor(1);
        u8g2.drawFrame(Window_X, y_Cursor, Window_W, Window_H);
        U8G2E_DrawWrappedText(Window_X + 7, y_Cursor + 3, str1, MaxWight);
        Move_Cursor(Window_Y, &y_Cursor);
        delay(10);
        u8g2.sendBuffer();
    } while (y_Cursor < Window_Y);
}
