#include <U8g2_Expand.h> //引入动画扩展

/*************************************部分支持与底层实现***************************************/

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
typedef enum
{
    Slow = 1,
    Elasticity = 2,
} Mode;
void U8G2E_MoveCursor(int GoalValue, float *CurrentValue, uint8_t Mode)
{
    if (Mode == Slow)
    {
        float Error = GoalValue - *CurrentValue; // 误差等于目标值减去实际值
        if (fabs(Error) > 1)                     // 误差大于1
        {
            *CurrentValue += Error / 4; // 实际值加上误差除以10
        }
        else
            *CurrentValue = GoalValue; // 误差小于1 直接等于目标值
    }
    else if (Mode == Elasticity)
    {
        static float Velocity = 0.0f; // 内部存储速度（静态变量）
        const float Stiffness = 0.4f; // 弹性系数（刚度，控制回弹力度）
        const float Damping = 0.5f;   // 阻尼系数（控制减震效果）

        float Error = GoalValue - *CurrentValue;         // 目标差值
        float SpringForce = Error * Stiffness;           // 弹性力（拉向目标）
        float DampingForce = -Velocity * Damping;        // 阻尼力（减速）
        float Acceleration = SpringForce + DampingForce; // 总加速度

        Velocity += Acceleration;  // 更新速度
        *CurrentValue += Velocity; // 更新位置

        // 如果接近目标且速度足够小，直接设为目标值
        if (fabs(Error) < 1 && fabs(Velocity) < 0.1f)
        {
            *CurrentValue = GoalValue;
            Velocity = 0.0f; // 重置速度
        }
    }
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
    max_width -= max_width % u8g2.getMaxCharWidth();                    // 最大宽度取整
    int X_Cursor = x, Y_Cursor = y + u8g2.getMaxCharHeight();           // 光标位置
    for (uint8_t i = 0; i < strlen(str); i++)                           // 遍历整个字符串
    {
        if (X_Cursor >= max_width + x)
        {
            X_Cursor = x;
            Y_Cursor += u8g2.getMaxCharHeight();
            if (*text == ' ')
            {
                text++;
                continue; // 换行遇到空格直接跳过
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
    max_width -= max_width % u8g2.getMaxCharWidth();                    // 最大宽度取整
    int X_Cursor = x, Y_Cursor = u8g2.getMaxCharHeight();               // 光标位置
    for (uint8_t i = 0; i < strlen(str); i++)                           // 遍历整个字符串
    {
        if (X_Cursor >= max_width + x)
        {
            X_Cursor = x;
            Y_Cursor += u8g2.getMaxCharHeight();
            if (*text == ' ')
            {
                text++;
                continue; // 换行遇到空格直接跳过
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
 * @brief 注册按键函数
 * @note  使用例：U8G2E_SignKeyFun(key2_scan_alone);
 */
typedef int (*Key_scan_fun)(void);
Key_scan_fun KEY_Scan;
void U8G2E_SignKeyFun(int Put_in_fun(void))
{
    KEY_Scan = Put_in_fun;
}
typedef enum
{
    KEY_NULL = 0,    // 无返回
    KEY_DOWN = 3,    // 返回向下
    KEY_UP = 2,      // 返回向上
    KEY_CONFIRM = 1, // 返回确认
} Key_mapping;       // 按键映射

/*************************************动画部分***************************************/

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
 * @brief 弹窗提示
 *
 * @param str1 提示内容
 * @note 弹窗提示，字体可调整，弹窗大小随字体与长度变换
 */
void U8G2E_PromptWindow(const char *str1, bool Key_trigger_enable)
{
    uint8_t MaxWight = 80; // 字符串最大宽度
    uint8_t Window_W = MaxWight - MaxWight % u8g2.getMaxCharWidth() + 14;
    uint8_t Window_X = (int)((128 - Window_W) / 2);
    uint8_t Window_H = U8G2E_StrHight(str1, MaxWight, Window_X + 5) + 10;
    uint8_t Window_Y = (int)((64 - Window_H) / 2);
    float y_Cursor = 64;
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
        U8G2E_MoveCursor(Window_Y, &y_Cursor, Elasticity);
        delay(15);
        u8g2.sendBuffer();
    } while (y_Cursor != Window_Y);

    if (Key_trigger_enable)
        while (KEY_Scan() == KEY_NULL) // 按任意键退出
            ;
    else
        delay(1000);

    Window_Y = 100;
    do // 下降
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
 * @brief 丝滑数字显示函数
 *
 * @param num 待显示的数字
 * @param x 数字的x坐标
 * @param y 数字的y坐标
 * @param change 数字对应的数组
 * @param W 数字的宽度
 * @param H 数字的高度
 */
void U8G2E_NUMDisplay(int num, int x, int y, float change[], int W, int H)
{
    static const uint8_t digit_patterns[10] = {0x7D, 0x50, 0x37, 0x57, 0x5A, 0x4F, 0x6F, 0x51, 0x7F, 0x5F};
    if (num < 0 || num > 9)
        return; // 超出范围，不显示
    uint8_t pattern = digit_patterns[num];
    for (int i = 0; i < 7; i++)
    {
        if (i >= 0 && i <= 2)
            U8G2E_MoveCursor((pattern & (1 << i)) ? W : 0, &change[i], Slow);
        if (i >= 3 && i <= 6)
            U8G2E_MoveCursor((pattern & (1 << i)) ? H : 0, &change[i], Slow);
    }
    U8G2E_MoveCursor(H, &change[7], Slow);           // 计数用
    u8g2.drawHLine(x + 1, y, change[0]);             // 上横线
    u8g2.drawHLine(x + 1, y + H, change[1]);         // 中横线
    u8g2.drawHLine(x + 1, y + 2 * H - 1, change[2]); // 下横线
    u8g2.drawVLine(x + 1, y, change[3]);             // 左上竖线
    u8g2.drawVLine(x + W, y, change[4]);             // 右上竖线
    u8g2.drawVLine(x + 1, y + H, change[5]);         // 左下竖线
    u8g2.drawVLine(x + W, y + H, change[6]);         // 右下竖线
}
