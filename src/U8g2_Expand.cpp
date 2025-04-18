#include <U8g2_Expand.h> //引入动画扩展

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, 14, 12, 13); // 定义U8g2库

/**
 * @brief 初始化OLED
 * @param Enable 是否启用开机动画
 */
void U8G2E_Init(bool Enable)
{
    u8g2.begin();                    // 启动OLED
    u8g2.enableUTF8Print();          // 开启中文字符支持
    u8g2.setFont(u8g2_font_6x12_mf); // 设置初始字体
    if (Enable)
    {
        U8G2E_StartAnimation("U8G2 Expand", "Design by maker114");
    }
}

/**
 * @brief 数字平滑移动
 *
 * @param GoalValue 目标值
 * @param CurrentValue 待更改的当前值
 * @note 函数不作延时，需要在每一帧的后面独立按照需求延时
 */
void Move_Cursor(int GoalValue, float *CurrentValue)
{
    float Error = GoalValue - *CurrentValue; // 误差等于目标值减去实际值
    if (fabs(Error) > 1)                     // 误差大于1
    {
        *CurrentValue += Error / 4; // 实际值加上误差除以10
    }
    else
        *CurrentValue = GoalValue; // 误差小于1 直接等于目标值
}

/**
 * @brief 背景虚化
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
 * @brief 自动换行输出字符串
 *
 * @param x x起点坐标
 * @param y y起点坐标
 * @param str 字符串
 * @param max_width 最大宽度
 * @note 需要在子程序外定义字体
 */
void U8G2E_DrawWrappedText(u8g2_uint_t x, u8g2_uint_t y, const char *str, u8g2_uint_t max_width)
{
    const char *text = str;
    uint8_t Display_width = u8g2.getDisplayWidth();                     // 获取屏幕宽度
    max_width = max_width >= Display_width ? Display_width : max_width; // 判断最大宽度是否超过屏幕宽度
    int X_Cursor = x, Y_Cursor = y + u8g2.getMaxCharHeight();           // 光标位置
    for (uint8_t i = 0; i < strlen(str); i++)                           // 遍历整个字符串
    {
        // FIXME:换行后第一个字符空一格
        if (X_Cursor > max_width + x || *text == '\n')
        {
            X_Cursor = x;
            Y_Cursor += u8g2.getMaxCharHeight();
            if (*text == ' ')
            {
                text++;
                continue; // 换行遇到空格直接跳过
            }
        }
        u8g2.setCursor(X_Cursor, Y_Cursor);
        u8g2.printf("%c", *text);
        text++;
        X_Cursor += u8g2.getMaxCharWidth();
    }
}

/**
 * @brief 获取字符串在当前字体和换行长度下的高度
 *
 * @param str 字符串
 * @param max_width 最大宽度
 * @return uint8_t 字符串高度
 */
uint8_t U8G2E_StrHight(const char *str, uint8_t max_width, uint8_t x)
{
    const char *text = str;
    uint8_t Display_width = u8g2.getDisplayWidth();                     // 获取屏幕宽度
    max_width = max_width >= Display_width ? Display_width : max_width; // 判断最大宽度是否超过屏幕宽度
    int X_Cursor = x, Y_Cursor = 0;                                     // 光标位置
    for (uint8_t i = 0; i < strlen(str); i++)                           // 遍历整个字符串
    {
        if (X_Cursor > max_width || *text == '\n')
        {
            X_Cursor = x;
            Y_Cursor += u8g2.getMaxCharHeight();
            if (*text == ' ')
            {
                text++;
                continue; // 换行遇到空格直接跳过
            }
        }
        text++;
        X_Cursor += u8g2.getMaxCharWidth();
    }
    return Y_Cursor;
}

/**
 * @brief 保存缓冲区
 *
 */
uint8_t Buffer_Backups[8 * 64 * 128]; // 缓冲区备份，大小为8 * u8g2.getBufferTileHeight() * u8g2.getBufferTileWidth()
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
 * @brief 恢复缓冲区
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
 * @brief 开机动画
 *
 * @param str1 主标题，最多10个字符
 * @param str2 副标题，最多17个字符
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
 * @brief 弹窗提示
 *
 * @param str1 提示内容
 * @note 弹窗提示，字体可调整，弹窗大小随字体与长度变换
 */
void U8G2E_PromptWindow(const char *str1)
{
    // TODO: 设置选择位，使能时弹窗延时退出，失能时弹窗按键退出
    uint8_t MaxWight = 80; // 字符串最大宽度
    uint8_t Window_W = MaxWight + 20;
    uint8_t Window_X = (int)((128 - Window_W) / 2);
    uint8_t Window_H = U8G2E_StrHight(str1, MaxWight, Window_X + 5) + 10;
    uint8_t Window_Y = (int)((64 - Window_H) / 2);
    float y_Cursor = 64;
    // FIXME:部分字体最后一行会超出边框
    U8G2E_SaveBuffer();
    do // 上升
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
    do // 下降
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
