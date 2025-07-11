#include <U8g2_Expand.h> //引入动画扩展
#include <string.h>

/*************************************部分支持与底层实现***************************************/
U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, 14, 12, 13); // 定义U8g2库

/**
 * @brief 初始化OLED
 * @param Enable 是否启用开机动画
 * @param str1 开机动画第一行文字(主标题)
 * @param str2 开机动画第二行文字(副标题）
 */
void U8G2E_Init(bool Enable, const char *str1, const char *str2)
{
    u8g2.begin();                    // 启动OLED
    u8g2.enableUTF8Print();          // 开启中文字符支持
    u8g2.setFont(u8g2_font_6x12_mf); // 设置初始字体
    if (Enable)
    {
        U8G2E_StartAnimation(str1, str2);
    }
}

/**
 * @brief 数字平滑移动
 *
 * @param GoalValue 目标值
 * @param CurrentValue 待更改的当前值
 * @param Mode 移动模式（Slow或Elasticity）
 * @note 函数不作延时，需要在每一帧的后面独立按照需求延时
 */
void U8G2E_MoveCursor(int GoalValue, float *CurrentValue, uint8_t Mode)
{
    const float max_speed = 12.0f; // 限速
    if (Mode == Slow)
    {
        float Error = GoalValue - *CurrentValue;
        if (fabs(Error) > 1)
        {
            float delta = Error / 4.0f;
            // 应用限速
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
        static float Velocity = 0.0f; // 内部存储速度（静态变量）
        const float Stiffness = 0.3f; // 弹性系数（刚度，控制回弹力度）
        const float Damping = 0.4f;   // 阻尼系数（控制减震效果）

        float Error = GoalValue - *CurrentValue;         // 目标差值
        float SpringForce = Error * Stiffness;           // 弹性力（拉向目标）
        float DampingForce = -Velocity * Damping;        // 阻尼力（减速）
        float Acceleration = SpringForce + DampingForce; // 总加速度

        Velocity += Acceleration;
        // 应用限速
        if (max_speed > 0.0f)
        {
            if (Velocity > max_speed)
                Velocity = max_speed;
            else if (Velocity < -max_speed)
                Velocity = -max_speed;
        }
        *CurrentValue += Velocity;
        // 如果接近目标且速度足够小，直接设为目标值
        if (fabs(Error) < 1 && fabs(Velocity) < 0.1f)
        {
            *CurrentValue = GoalValue;
            Velocity = 0.0f;
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

void U8G2E_CountDigits(double number, int *integer_digits, int *decimal_digits)
{
    char buf[32];
    *integer_digits = 0;
    *decimal_digits = 0;

    // 处理特殊情况：0、NaN、Infinity
    if (number == 0.0)
    {
        *integer_digits = 1;
        return;
    }
    if (isnan(number) || isinf(number))
    {
        return;
    }

    // 使用%g格式，但先检查是否会使用科学计数法
    double abs_num = fabs(number);
    if (abs_num >= 1e6 || abs_num <= 1e-4)
    {
        // 可能会使用科学计数法，改用%f格式
        snprintf(buf, sizeof(buf), "%.8f", number);
    }
    else
    {
        snprintf(buf, sizeof(buf), "%.8g", number);
    }

    // 检查科学计数法(e或E)
    char *e_pos = strchr(buf, 'e');
    if (!e_pos)
        e_pos = strchr(buf, 'E');

    if (e_pos)
    {
        // 处理科学计数法的情况
        *e_pos = '\0'; // 截断指数部分

        // 计算有效数字部分的整数和小数位数
        char *dot = strchr(buf, '.');
        if (dot)
        {
            *integer_digits = (int)(dot - buf) - (*buf == '-');
            *decimal_digits = (int)strlen(dot + 1);
        }
        else
        {
            *integer_digits = (int)strlen(buf) - (*buf == '-');
        }
        return;
    }

    // 非科学计数法的情况
    char *dot = strchr(buf, '.');
    if (dot)
    {
        *integer_digits = (int)(dot - buf) - (*buf == '-');

        // 计算小数位数，去除末尾的0
        char *decimal_start = dot + 1;
        char *p = decimal_start + strlen(decimal_start) - 1;
        while (p >= decimal_start && *p == '0')
        {
            p--;
        }
        *decimal_digits = (int)(p - decimal_start + 1);
    }
    else
    {
        *integer_digits = (int)strlen(buf) - (*buf == '-');
    }
}

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
        delay(12);
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

/**
 * @brief 丝滑移动菜单
 *
 * @param MenuOption_ARR 菜单结构体数组
 * @param valid_num 有效选项数量
 */
void U8G2E_MenuDisplay(U8G2E_MenuOption MenuOption_ARR[], uint8_t valid_num)
{
    // 变量声明
    int8_t MenuOption_NUM = 0;          // 当前选中菜单项索引
    int8_t t = 0;                       // 边界弹性动画参数
    uint8_t y_skew = 3;                 // 光标垂直偏移量
    uint8_t TOP_Flag = 0;               // 顶部边界标志
    uint8_t BASE_Flag = 0;              // 底部边界标志
    uint8_t Key_result;                 // 按键结果
    float origin_y_Coordinate = 2;      // 当前Y轴基准坐标
    float goal_origin_y_Coordinate = 2; // 目标Y轴基准坐标
    float Cursor_y = 0;                 // 光标Y坐标

    // 初始化显示设置
    u8g2.setFont(u8g2_font_questgiver_tr);
    const uint8_t DisplayWidth = u8g2.getDisplayWidth();
    const uint8_t DisplayHeight = u8g2.getDisplayHeight();
    const uint8_t CharHeight = u8g2.getMaxCharHeight();
    const uint8_t CharWidth = u8g2.getMaxCharWidth();
    const uint8_t MENU_ITEM_COUNT = 5; // 进入动画菜单项数量

    /******************** 进入动画部分 ********************/
    U8G2E_SaveBuffer();
    for (uint8_t i = 0; i < MENU_ITEM_COUNT; i++) // 初始化菜单项坐标
    {
        const uint8_t Skew_Coefficient = 40; // 动画偏移系数(值越大间隔越明显)
        MenuOption_ARR[i].X_Coordinate = DisplayWidth + Skew_Coefficient * i;
        MenuOption_ARR[i].Y_Coordinate = CharHeight * (i + 1) + 2; // 垂直间距+2像素偏移
    }
    do // 执行进入动画
    {
        u8g2.clearBuffer();
        U8G2E_CoverBuffer();
        U8G2E_MoveCursor(5, &MenuOption_ARR[0].X_Coordinate, Slow);
        U8G2E_MoveCursor(MenuOption_ARR[0].Y_Coordinate, &Cursor_y, Slow);
        u8g2.setDrawColor(0);
        u8g2.drawBox(MenuOption_ARR[0].X_Coordinate - 5, MenuOption_ARR[0].Y_Coordinate - CharHeight - 2, DisplayWidth, CharHeight + 3); // +3像素垂直偏移
        u8g2.setDrawColor(1);
        u8g2.drawBox(1, Cursor_y - CharHeight + y_skew, 2, CharHeight - y_skew);
        U8G2E_MenuOptionDisplay(MenuOption_ARR[0]);
        for (uint8_t i = 1; i < MENU_ITEM_COUNT; i++)
        {
            // 水平移动动画
            U8G2E_MoveCursor(0, &MenuOption_ARR[i].X_Coordinate, Slow);
            // 清除旧内容
            u8g2.setDrawColor(0);
            u8g2.drawBox(MenuOption_ARR[i].X_Coordinate, MenuOption_ARR[i].Y_Coordinate - CharHeight + 3, DisplayWidth, CharHeight); // +3像素垂直偏移
            // 绘制新内容
            u8g2.setDrawColor(1);
            U8G2E_MenuOptionDisplay(MenuOption_ARR[i]);
        }
        delay(10);
        u8g2.sendBuffer();
    } while (MenuOption_ARR[MENU_ITEM_COUNT - 1].X_Coordinate > 0);

    /******************** 主交互循环 ********************/
    do
    {
        u8g2.clearBuffer();
        Key_result = (goal_origin_y_Coordinate == origin_y_Coordinate) ? KEY_Scan() : 0; // 只在坐标稳定时检测按键
        switch (Key_result)                                                              // 处理按键输入
        {
        case KEY_UP:
            if (MenuOption_NUM > 0)
                MenuOption_NUM--;
            if (MenuOption_NUM <= 0) // 顶部边界检查
            {
                TOP_Flag = 1;
                MenuOption_NUM = 0;
            }
            if (MenuOption_ARR[MenuOption_NUM].Y_Coordinate - CharHeight < 2) // 需要上滚的情况
            {
                goal_origin_y_Coordinate += CharHeight;
            }
            break;

        case KEY_DOWN:
            if (MenuOption_NUM < valid_num - 1)
                MenuOption_NUM++;
            if (MenuOption_NUM >= valid_num - 1) // 底部边界检查
            {
                BASE_Flag = 1;
                MenuOption_NUM = valid_num - 1;
            }
            if (MenuOption_ARR[MenuOption_NUM].Y_Coordinate > DisplayHeight - 2) // 需要下滚的情况
            {
                goal_origin_y_Coordinate -= CharHeight;
            }
            break;
        }
        // 处理边界弹性动画
        if (TOP_Flag)
            t = (t < 10) ? t + 2 : (TOP_Flag = 0, 0); // 顶部回弹效果
        if (BASE_Flag)
            t = (t > -10) ? t - 2 : (BASE_Flag = 0, 0); // 底部回弹效果
        // 更新整体位置
        U8G2E_MoveCursor(goal_origin_y_Coordinate + t, &origin_y_Coordinate, Slow);

        // 绘制所有菜单项
        for (uint8_t i = 0; i < valid_num; i++)
        {
            // 更新菜单项Y坐标,以便整体移动
            MenuOption_ARR[i].Y_Coordinate = origin_y_Coordinate + CharHeight * (i + 1);

            // 处理选中项的特殊效果
            if (MenuOption_NUM == i)
            {
                U8G2E_MoveCursor(5, &MenuOption_ARR[i].X_Coordinate, Slow);              // 水平弹性效果
                U8G2E_MoveCursor(MenuOption_ARR[i].Y_Coordinate, &Cursor_y, Slow);       // 光标Y坐标跟随选中项
                u8g2.drawBox(1, Cursor_y - CharHeight + y_skew, 2, CharHeight - y_skew); // 绘制选中光标s
            }
            else
            {
                U8G2E_MoveCursor(0, &MenuOption_ARR[i].X_Coordinate, Slow); // 非选中项无弹性
            }
            // 显示菜单内容
            U8G2E_MenuOptionDisplay(MenuOption_ARR[i]);
        }
        // 执行菜单动作
        if (Key_result == KEY_CONFIRM) // 确认键
            U8G2E_MenuExecute(&MenuOption_ARR[MenuOption_NUM]);
        u8g2.setFont(u8g2_font_questgiver_tr);
        delay(10);
        u8g2.sendBuffer(); // 刷新此帧
    } while (Key_result != KEY_EXIT);

    //**********退出动画**********/
    do
    {
        u8g2.clearBuffer();
        // 绘制所有菜单项
        for (uint8_t i = 0; i < valid_num; i++)
        {
            // 更新菜单项Y坐标,以便整体移动
            MenuOption_ARR[i].Y_Coordinate = origin_y_Coordinate + CharHeight * (i + 1);

            // 处理选中项的特殊效果
            if (MenuOption_NUM == i)
            {
                U8G2E_MoveCursor(130, &MenuOption_ARR[i].X_Coordinate, Slow);            // 水平弹性效果
                U8G2E_MoveCursor(0, &Cursor_y, Slow);                                    // 光标Y坐标跟随选中项
                u8g2.drawBox(1, Cursor_y - CharHeight + y_skew, 2, CharHeight - y_skew); // 绘制选中光标
            }
            else
            {
                U8G2E_MoveCursor(130, &MenuOption_ARR[i].X_Coordinate, Slow); // 非选中项无弹性
            }
            // 显示菜单内容
            U8G2E_MenuOptionDisplay(MenuOption_ARR[i]);
        }
        delay(6);
        u8g2.sendBuffer(); // 刷新此帧
    } while (MenuOption_ARR[0].X_Coordinate != 130);
}

/**
 * @brief 绘制菜单项
 *
 * @param MenuOption_Member 菜单选项结构体
 */
void U8G2E_MenuOptionDisplay(U8G2E_MenuOption MenuOption_Member)
{
    uint8_t y_skew = 0;
    u8g2.setCursor(MenuOption_Member.X_Coordinate, MenuOption_Member.Y_Coordinate);
    u8g2.print(MenuOption_Member.Title);
    switch (MenuOption_Member.Kind)
    {
    case U8G2E_OPTION_TEXT:
        break;

    case U8G2E_OPTION_KEY:
        u8g2.drawFrame(MenuOption_Member.X_Coordinate + 100, MenuOption_Member.Y_Coordinate - u8g2.getMaxCharHeight() + 2 + y_skew, 21, u8g2.getMaxCharHeight() - y_skew);
        if (MenuOption_Member.Value == 1)
        {
            u8g2.setCursor(MenuOption_Member.X_Coordinate + 95 - 1 * u8g2.getMaxCharWidth(), MenuOption_Member.Y_Coordinate);
            u8g2.print("ON");
            u8g2.drawBox(MenuOption_Member.X_Coordinate + 102, MenuOption_Member.Y_Coordinate - u8g2.getMaxCharHeight() + 4 + y_skew, 8, u8g2.getMaxCharHeight() - 4 - y_skew);
        }
        else
        {
            u8g2.setCursor(MenuOption_Member.X_Coordinate + 95 - 1 * u8g2.getMaxCharWidth() - 3, MenuOption_Member.Y_Coordinate);
            u8g2.print("OFF");
            u8g2.drawBox(MenuOption_Member.X_Coordinate + 111, MenuOption_Member.Y_Coordinate - u8g2.getMaxCharHeight() + 4 + y_skew, 8, u8g2.getMaxCharHeight() - 4 - y_skew);
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
        if (MenuOption_Member.Value > 999)
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

/**
 * @brief 执行菜单项动作
 *
 * @param MenuOption_Member 菜单结构体
 */
void U8G2E_MenuExecute(U8G2E_MenuOption *MenuOption_Member)
{
    void (*Packaging_Function)(void);
    switch (MenuOption_Member->Kind)
    {
    case U8G2E_OPTION_KEY:
        MenuOption_Member->Value = !MenuOption_Member->Value;
        break;

    case U8G2E_OPTION_PCT:
        U8G2E_PCT_ACTION(MenuOption_Member); // 处理百分比选项
        break;

    case U8G2E_OPTION_NUM:
        U8G2E_NUM_ACTION(MenuOption_Member); // 处理数字选项
        break;

    case U8G2E_OPTION_FUNC:
        Packaging_Function = (void (*)(void))(intptr_t)MenuOption_Member->Value;
        Packaging_Function();
        break;
    }
}

/**
 * @brief 处理百分比选项
 *
 * @param MenuOption_Member 菜单结构体
 */
void U8G2E_PCT_ACTION(U8G2E_MenuOption *MenuOption_Member)
{
    uint8_t key_result = 0;
    uint8_t Digital_bit = 1;
    float Base_Y_Coordinate = 64;        // 基准Y坐标
    float Goal_Y_Coordinate = 0;         // 目标Y坐标
    float ProgressBar_ExternalFarme = 0; // 进度条外框长度
    float ProgressBar_InsideFarme = 0;   // 进度条内框长度
    bool Exit_tigger = false;
    bool Exit_Flag = false;
    U8G2E_SaveBuffer();
    do
    {
        key_result = (Exit_tigger == true) ? KEY_EXIT : KEY_Scan(); // 检测按键,在返回时停止检测
        u8g2.clearBuffer();
        U8G2E_CoverBuffer();
        U8G2E_Blurring(); // 背景虚化
        // 计算坐标
        Goal_Y_Coordinate = (key_result == KEY_EXIT) ? 64 : 12;
        Exit_tigger = (key_result == KEY_EXIT) ? true : false;
        U8G2E_MoveCursor(Goal_Y_Coordinate, &Base_Y_Coordinate, Slow);
        Exit_Flag = (Base_Y_Coordinate == 64) ? true : false;
        // 绘制窗口
        u8g2.setDrawColor(0);
        u8g2.drawBox(10 - 3, Base_Y_Coordinate - 3, 128 - 10 * 2 + 6, 38 + 6);
        u8g2.setDrawColor(1);
        u8g2.drawFrame(10, Base_Y_Coordinate, 128 - 10 * 2, 38); // 绘制窗口
        // 绘制进度条
        U8G2E_MoveCursor(98, &ProgressBar_ExternalFarme, Slow);                             // 外框
        U8G2E_MoveCursor(MenuOption_Member->Value * 0.94f, &ProgressBar_InsideFarme, Slow); // 内框
        u8g2.drawFrame(15, Base_Y_Coordinate + 18, ProgressBar_ExternalFarme, 15);          // 外框
        u8g2.drawBox(17, Base_Y_Coordinate + 20, ProgressBar_InsideFarme, 11);              // 内框
        // 文字部分
        u8g2.setFont(u8g2_font_6x13_mf);
        u8g2.setCursor(15, Base_Y_Coordinate + u8g2.getMaxCharHeight());
        u8g2.print("Percent:");
        char buffer[20];
        sprintf(buffer, "%.2f%%", MenuOption_Member->Value);
        u8g2.setCursor(113 - u8g2.getStrWidth(buffer), Base_Y_Coordinate + u8g2.getMaxCharHeight());
        u8g2.print(buffer);
        // 操作部分
        Digital_bit = (key_result == KEY_CONFIRM) ? Digital_bit + 1 : Digital_bit;
        Digital_bit = (Digital_bit > 4) ? 1 : Digital_bit;
        float Operand = 0.001 * pow(10, Digital_bit);
        MenuOption_Member->Value = (key_result == KEY_UP) ? MenuOption_Member->Value + Operand : MenuOption_Member->Value;
        MenuOption_Member->Value = (key_result == KEY_DOWN) ? MenuOption_Member->Value - Operand : MenuOption_Member->Value;
        uint8_t skew = (Digital_bit == 3 || Digital_bit == 4) ? 1 : 0;
        u8g2.drawHLine(113 - (Digital_bit + 1 + skew) * u8g2.getMaxCharWidth(), Base_Y_Coordinate + u8g2.getMaxCharHeight() + 2, 6);
        MenuOption_Member->Value = MenuOption_Member->Value > 100 ? 100 : MenuOption_Member->Value;
        MenuOption_Member->Value = MenuOption_Member->Value < 0 ? 0 : MenuOption_Member->Value;
        //  帧刷新
        u8g2.setFont(u8g2_font_questgiver_tr);
        delay(10);
        u8g2.sendBuffer();

    } while (Exit_Flag != true); // 退出标志
}

/**
 * @brief 数字处理选项
 *
 * @param MenuOption_Member 菜单结构体
 */
void U8G2E_NUM_ACTION(U8G2E_MenuOption *MenuOption_Member)
{
    uint8_t key_result = 0;
    uint8_t Digital_bit = 1;
    float Base_Y_Coordinate = 64; // 基准Y坐标
    float Goal_Y_Coordinate = 0;  // 目标Y坐标
    bool Exit_tigger = false;
    bool Exit_Flag = false;
    U8G2E_SaveBuffer();
    do
    {
        key_result = (Exit_tigger == true) ? KEY_EXIT : KEY_Scan(); // 检测按键,在返回时停止检测
        u8g2.clearBuffer();
        U8G2E_CoverBuffer();
        U8G2E_Blurring(); // 背景虚化
        // 计算坐标
        Goal_Y_Coordinate = (key_result == KEY_EXIT) ? 64 : 12;
        Exit_tigger = (key_result == KEY_EXIT) ? true : false;
        U8G2E_MoveCursor(Goal_Y_Coordinate, &Base_Y_Coordinate, Slow);
        Exit_Flag = (Base_Y_Coordinate == 64) ? true : false;
        // 绘制窗口
        u8g2.setDrawColor(0);
        u8g2.drawBox(10 - 3, Base_Y_Coordinate - 3, 128 - 10 * 2 + 6, 38 + 6);
        u8g2.setDrawColor(1);
        u8g2.drawFrame(10, Base_Y_Coordinate, 128 - 10 * 2, 38); // 绘制窗口
        // 文字部分
        u8g2.setFont(u8g2_font_6x13_mf);
        u8g2.setCursor(15, Base_Y_Coordinate + u8g2.getMaxCharHeight());
        u8g2.print("NUM:");
        char buffer[20];

        // sprintf(buffer, "%.8f", MenuOption_Member->Value);
        double abs_num = fabs(MenuOption_Member->Value);
        if (abs_num >= 1e6 || abs_num <= 1e-4)
        {
            snprintf(buffer, sizeof(buffer), "%.8f", MenuOption_Member->Value);
        }
        else
        {
            snprintf(buffer, sizeof(buffer), "%.8g", MenuOption_Member->Value);
        }
        u8g2.setCursor(113 - u8g2.getStrWidth(buffer), Base_Y_Coordinate + u8g2.getMaxCharHeight());
        u8g2.print(buffer);
        // 计算整数与小数
        int Interger = 0;                                                 // 整数位数
        int Decimal = 0;                                                  // 小数位数
        U8G2E_CountDigits(MenuOption_Member->Value, &Interger, &Decimal); // 计算整数与小数位数
        // 操作部分
        Digital_bit = (key_result == KEY_CONFIRM) ? Digital_bit + 1 : Digital_bit;
        Digital_bit = (Digital_bit > Interger + Decimal) ? 1 : Digital_bit;
        float Operand = 0;
        if (Digital_bit >= Interger) // 位于整数位
            Operand = pow(10, Digital_bit - Decimal - 1);
        else
            Operand = pow(10, -(Decimal - Digital_bit + 1));
        MenuOption_Member->Value = (key_result == KEY_UP) ? MenuOption_Member->Value + Operand : MenuOption_Member->Value;
        MenuOption_Member->Value = (key_result == KEY_DOWN) ? MenuOption_Member->Value - Operand : MenuOption_Member->Value;
        uint8_t skew = 0;
        if (Decimal != 0)
            skew = (Digital_bit > Decimal) ? 1 : 0; // 如果位于整数位则向前跳一格以跳过小数点
        else
            skew = 0;
        u8g2.drawHLine(113 - (Digital_bit + skew) * u8g2.getMaxCharWidth(), Base_Y_Coordinate + u8g2.getMaxCharHeight() + 2, 6);
        //  帧刷新
        printf("Interger:%d,Decimal:%d,Digital_bit:%d \n", Interger, Decimal, Digital_bit);
        u8g2.setFont(u8g2_font_questgiver_tr);
        delay(10);
        u8g2.sendBuffer();

    } while (Exit_Flag != true); // 退出标志
}
