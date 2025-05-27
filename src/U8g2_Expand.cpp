#include <U8g2_Expand.h> //���붯����չ
#include <string.h>

/*************************************����֧����ײ�ʵ��***************************************/

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, 14, 12, 13); // ����U8g2��

/**
 * @brief ��ʼ��OLED
 * @param Enable �Ƿ����ÿ�������
 */
void U8G2E_Init(bool Enable, const char *str1, const char *str2)
{
    u8g2.begin();                    // ����OLED
    u8g2.enableUTF8Print();          // ���������ַ�֧��
    u8g2.setFont(u8g2_font_6x12_mf); // ���ó�ʼ����
    if (Enable)
    {
        U8G2E_StartAnimation(str1, str2);
    }
}

/**
 * @brief ����ƽ���ƶ�
 *
 * @param GoalValue Ŀ��ֵ
 * @param CurrentValue �����ĵĵ�ǰֵ
 * @param Mode �ƶ�ģʽ��Slow��Elasticity��
 * @note ����������ʱ����Ҫ��ÿһ֡�ĺ����������������ʱ
 */
void U8G2E_MoveCursor(int GoalValue, float *CurrentValue, uint8_t Mode)
{
    const float max_speed = 12.0f; // ����
    if (Mode == Slow)
    {
        float Error = GoalValue - *CurrentValue;
        if (fabs(Error) > 1)
        {
            float delta = Error / 4.0f;
            // Ӧ������
            if (max_speed > 0.0f)
            {
                if (delta > max_speed)
                    delta = max_speed;
                else if (delta < -max_speed)
                    delta = -max_speed;
            }
            *CurrentValue += delta;
        }
        else
            *CurrentValue = GoalValue;
    }
    else if (Mode == Elasticity)
    {
        static float Velocity = 0.0f; // �ڲ��洢�ٶȣ���̬������
        const float Stiffness = 0.3f; // ����ϵ�����նȣ����ƻص����ȣ�
        const float Damping = 0.4f;   // ����ϵ�������Ƽ���Ч����

        float Error = GoalValue - *CurrentValue;         // Ŀ���ֵ
        float SpringForce = Error * Stiffness;           // ������������Ŀ�꣩
        float DampingForce = -Velocity * Damping;        // �����������٣�
        float Acceleration = SpringForce + DampingForce; // �ܼ��ٶ�

        Velocity += Acceleration;
        // Ӧ������
        if (max_speed > 0.0f)
        {
            if (Velocity > max_speed)
                Velocity = max_speed;
            else if (Velocity < -max_speed)
                Velocity = -max_speed;
        }
        *CurrentValue += Velocity;
        // ����ӽ�Ŀ�����ٶ��㹻С��ֱ����ΪĿ��ֵ
        if (fabs(Error) < 1 && fabs(Velocity) < 0.1f)
        {
            *CurrentValue = GoalValue;
            Velocity = 0.0f;
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

void U8G2E_MenuDisplay(U8G2E_MenuOption MenuOption_ARR[], uint8_t valid_num)
{
    // ȫ�־�̬��������
    static int8_t MenuOption_NUM = 0;          // ��ǰѡ�в˵�������
    static int8_t t = 0;                       // �߽絯�Զ�������
    static uint8_t TOP_Flag = 0;               // �����߽��־
    static uint8_t BASE_Flag = 0;              // �ײ��߽��־
    static float origin_y_Coordinate = 2;      // ��ǰY���׼����
    static float goal_origin_y_Coordinate = 2; // Ŀ��Y���׼����
    static float Cursor_y = 0;                 // ���Y����

    // ��ʼ����ʾ����
    u8g2.setFont(u8g2_font_6x12_mf);
    const uint8_t DisplayWidth = u8g2.getDisplayWidth();
    const uint8_t DisplayHeight = u8g2.getDisplayHeight();
    const uint8_t CharHeight = u8g2.getMaxCharHeight();
    const uint8_t CharWidth = u8g2.getMaxCharWidth();
    const uint8_t MENU_ITEM_COUNT = 5; // ���붯���˵�������

    /******************** ���붯������ ********************/
    U8G2E_SaveBuffer();
    for (uint8_t i = 0; i < MENU_ITEM_COUNT; i++) // ��ʼ���˵�������
    {
        const uint8_t Skew_Coefficient = 40; // ����ƫ��ϵ��(ֵԽ����Խ����)
        MenuOption_ARR[i].X_Coordinate = DisplayWidth + Skew_Coefficient * i;
        MenuOption_ARR[i].Y_Coordinate = CharHeight * (i + 1) + 2; // ��ֱ���+2����ƫ��
    }
    do // ִ�н��붯��
    {
        u8g2.clearBuffer();
        U8G2E_CoverBuffer();
        for (uint8_t i = 0; i < MENU_ITEM_COUNT; i++)
        {
            // ˮƽ�ƶ�����
            U8G2E_MoveCursor(0, &MenuOption_ARR[i].X_Coordinate, Slow);
            // ���������
            u8g2.setDrawColor(0);
            u8g2.drawBox(MenuOption_ARR[i].X_Coordinate, MenuOption_ARR[i].Y_Coordinate - CharHeight + 3, DisplayWidth, CharHeight); // +3���ش�ֱƫ��
            // ����������
            u8g2.setDrawColor(1);
            U8G2E_MenuDisplay(MenuOption_ARR[i]);
        }
        delay(10);
        u8g2.sendBuffer();
    } while (MenuOption_ARR[MENU_ITEM_COUNT - 1].X_Coordinate > 0);

    /******************** ������ѭ�� ********************/
    do
    {
        u8g2.clearBuffer();
        uint8_t Key_result = (goal_origin_y_Coordinate == origin_y_Coordinate) ? KEY_Scan() : 0; // ֻ�������ȶ�ʱ��ⰴ��
        switch (Key_result)                                                                      // ����������
        {
        case KEY_UP:
            if (MenuOption_NUM > 0)
                MenuOption_NUM--;
            // �����߽���
            if (MenuOption_NUM <= 0)
            {
                TOP_Flag = 1;
                MenuOption_NUM = 0;
            }
            // ��Ҫ�Ϲ������
            if (MenuOption_ARR[MenuOption_NUM].Y_Coordinate - CharHeight < 2)
            {
                goal_origin_y_Coordinate += CharHeight;
            }
            break;

        case KEY_DOWN:
            if (MenuOption_NUM < valid_num - 1)
                MenuOption_NUM++;
            // �ײ��߽���
            if (MenuOption_NUM >= valid_num - 1)
            {
                BASE_Flag = 1;
                MenuOption_NUM = valid_num - 1;
            }
            // ��Ҫ�¹������
            if (MenuOption_ARR[MenuOption_NUM].Y_Coordinate > DisplayHeight - 2)
            {
                goal_origin_y_Coordinate -= CharHeight;
            }
            break;
        }

        // ����߽絯�Զ���
        if (TOP_Flag)
            t = (t < 10) ? t + 2 : (TOP_Flag = 0, 0); // �����ص�Ч��
        if (BASE_Flag)
            t = (t > -10) ? t - 2 : (BASE_Flag = 0, 0); // �ײ��ص�Ч��

        // ��������λ��
        U8G2E_MoveCursor(goal_origin_y_Coordinate + t, &origin_y_Coordinate, Slow);

        // �������в˵���
        for (uint8_t i = 0; i < valid_num; i++)
        {
            // ���²˵���Y����,�Ա������ƶ�
            MenuOption_ARR[i].Y_Coordinate = origin_y_Coordinate + CharHeight * (i + 1);

            // ����ѡ���������Ч��
            if (MenuOption_NUM == i)
            {
                U8G2E_MoveCursor(5, &MenuOption_ARR[i].X_Coordinate, Slow);        // ˮƽ����Ч��
                U8G2E_MoveCursor(MenuOption_ARR[i].Y_Coordinate, &Cursor_y, Slow); // ���Y�������ѡ����
                u8g2.drawBox(1, Cursor_y - CharHeight + 4, 2, CharHeight - 4);     // ����ѡ�й��
                if (Key_result == KEY_CANCEL)
                    U8G2E_MenuExecute(&MenuOption_ARR[i]);
            }
            else
            {
                U8G2E_MoveCursor(0, &MenuOption_ARR[i].X_Coordinate, Slow); // ��ѡ�����޵���
            }

            // ��ʾ�˵�����
            U8G2E_MenuDisplay(MenuOption_ARR[i]);
            // ִ�в˵���Ӧ�Ĳ���
        }
        delay(10);
        u8g2.sendBuffer(); // ˢ�´�֡
    } while (1);

    //**********�˳�����**********//
}

void U8G2E_MenuDisplay(U8G2E_MenuOption MenuOption_Member)
{
    u8g2.setCursor(MenuOption_Member.X_Coordinate, MenuOption_Member.Y_Coordinate);
    u8g2.print(MenuOption_Member.Title);
    switch (MenuOption_Member.Kind)
    {
    case U8G2E_OPTION_TEXT:
        break;

    case U8G2E_OPTION_KEY:
        u8g2.drawFrame(MenuOption_Member.X_Coordinate + 100, MenuOption_Member.Y_Coordinate - u8g2.getMaxCharHeight() + 4, 21, u8g2.getMaxCharHeight() - 2);
        if (MenuOption_Member.Value == 1)
        {
            u8g2.setCursor(MenuOption_Member.X_Coordinate + 86, MenuOption_Member.Y_Coordinate);
            u8g2.print("ON");
            u8g2.drawBox(MenuOption_Member.X_Coordinate + 102, MenuOption_Member.Y_Coordinate - u8g2.getMaxCharHeight() + 6, 8, u8g2.getMaxCharHeight() - 6);
        }
        else
        {
            u8g2.setCursor(MenuOption_Member.X_Coordinate + 80, MenuOption_Member.Y_Coordinate);
            u8g2.print("OFF");
            u8g2.drawBox(MenuOption_Member.X_Coordinate + 111, MenuOption_Member.Y_Coordinate - u8g2.getMaxCharHeight() + 6, 8, u8g2.getMaxCharHeight() - 6);
        }

        break;

    case U8G2E_OPTION_PCT:
        char buffer[20];
        sprintf(buffer, "%.2f%%", MenuOption_Member.Value);
        u8g2.setCursor(MenuOption_Member.X_Coordinate + 122 - u8g2.getStrWidth(buffer), MenuOption_Member.Y_Coordinate);
        u8g2.print(buffer);
        break;

    case U8G2E_OPTION_NUM:
        char buffer1[20];
        if (MenuOption_Member.Value > 99)
            sprintf(buffer1, "%.1f", MenuOption_Member.Value);
        else if (MenuOption_Member.Value < 9)
            sprintf(buffer1, "%.5f", MenuOption_Member.Value);
        else
            sprintf(buffer1, "%.3f", MenuOption_Member.Value);
        u8g2.setCursor(MenuOption_Member.X_Coordinate + 122 - u8g2.getStrWidth(buffer1), MenuOption_Member.Y_Coordinate);
        u8g2.print(buffer1);

    default:
        break;
    }
}

void U8G2E_MenuExecute(U8G2E_MenuOption *MenuOption_Member)
{
    switch (MenuOption_Member->Kind)
    {
    case U8G2E_OPTION_KEY:
        MenuOption_Member->Value = !MenuOption_Member->Value;
        break;
    }
}