# U8G2_Expand 库文档

## 1. 简介
`U8G2_Expand` 是一个基于 U8g2 图形库的扩展库，专为 Arduino 平台开发，为著名 OLED 驱动库 `u8g2` 提供包括菜单在内的丰富拓展，主要特性包括：
- 平滑动画系统（缓动/弹性效果）
- 预置开机动画
- 自动换行文本渲染
- 弹窗提示系统
- 动态数字显示（七段数码管风格）
- 交互式菜单系统（支持多种选项类型）
- 背景虚化效果
- 缓冲区压缩备份与恢复
- 按键事件处理

---

## 2. 函数列表

| 函数 | 说明 |
|------|------|
| `U8G2E_Init()` | 初始化 OLED 并可选启动开机动画 |
| `U8G2E_MoveCursor()` | 数字平滑移动（缓动/弹性模式） |
| `U8G2E_Blurring()` | 背景虚化效果 |
| `U8G2E_DrawWrappedText()` | 自动换行文本渲染 |
| `U8G2E_StrHeight()` | 计算文本在自动换行后的渲染高度 |
| `U8G2E_SaveBuffer()` | 压缩并保存当前显示缓冲区 |
| `U8G2E_CoverBuffer()` | 恢复之前保存的显示缓冲区 |
| `U8G2E_SignKeyFun()` | 注册按键扫描函数 |
| `U8G2E_CountDigits()` | 计算浮点数的整数/小数位数（自动去尾零） |
| `U8G2E_StartAnimation()` | 开机动画（带双行标题） |
| `U8G2E_PromptWindow()` | 弹窗提示（支持按键触发） |
| `U8G2E_NUMDisplay()` | 丝滑数字显示（七段数码管风格） |
| `U8G2E_MenuDisplay()` | 丝滑移动菜单主控函数 |
| `U8G2E_MenuOptionDisplay()` | 渲染单个菜单项 |
| `U8G2E_MenuExecute()` | 执行菜单项动作 |
| `U8G2E_PCT_ACTION()` | 百分比选项编辑器 |
| `U8G2E_NUM_ACTION()` | 数字选项编辑器 |

---

## 3. 结构体与枚举

### **菜单选项结构体 (U8G2E_MenuOption)**

```cpp
typedef struct {
    String Title;       // 选项标题（支持 UTF?8 字符串，需字体支持）
    double Value;       // 选项数值（用于存储开关状态/百分比/数值等）
    uint8_t Kind;       // 选项类型（见 U8G2E_OptionType 枚举）
    float X_Coordinate; // 动态 X 坐标（用于实现弹性动画）
    float Y_Coordinate; // 动态 Y 坐标（用于实现滚动效果）
} U8G2E_MenuOption;
```

#### 成员说明
| 成员           | 类型      | 说明                                                         |
| -------------- | --------- | ------------------------------------------------------------ |
| `Title`        | `String`  | 菜单项显示文本，支持 UTF?8 编码（需配合支持中文的字体）。    |
| `Value`        | `double`  | 多功能数值：<br> 开关类型：0/1<br> 百分比：0.0~100.0<br> 数值：任意浮点数 |
| `Kind`         | `uint8_t` | 决定选项行为，需使用 `U8G2E_OptionType` 枚举值。             |
| `X_Coordinate` | `float`   | 动态 X 坐标（配合 `U8G2E_MoveCursor` 实现横向弹性效果）。    |
| `Y_Coordinate` | `float`   | 动态 Y 坐标（配合菜单滚动系统使用）。                        |

---

### **选项类型枚举 (U8G2E_OptionType)**

```cpp
typedef enum {
    U8G2E_OPTION_KEY = 0,  // 开关类型（显示 ON/OFF 状态框）
    U8G2E_OPTION_NUM = 1,  // 数值类型（可编辑整数/小数）
    U8G2E_OPTION_PCT = 2,  // 百分比类型（自动限制 0~100 范围）
    U8G2E_OPTION_TEXT = 3, // 纯文本类型（仅显示，不可交互）
    U8G2E_OPTION_FUNC = 4  // 函数类型（点击执行回调函数）
} U8G2E_OptionType;
```

#### 枚举值说明
| 值                  | 说明                                              |
| ------------------- | ------------------------------------------------- |
| `U8G2E_OPTION_KEY`  | 显示为开关控件，`Value` 字段应为 0 (OFF) 或 1 (ON) |
| `U8G2E_OPTION_NUM`  | 进入数值编辑模式（调用 `U8G2E_NUM_ACTION`）       |
| `U8G2E_OPTION_PCT`  | 进入百分比编辑模式（调用 `U8G2E_PCT_ACTION`）     |
| `U8G2E_OPTION_TEXT` | 不可选文本，通常用于菜单分类标题或信息表达        |
| `U8G2E_OPTION_FUNC` | 点击后执行 `Value` 字段指向的函数（需强制类型转换）|

**示例数组：**

```c
U8G2E_MenuOption MenuOption_ARR[7]{
    {"Text Option1", (int)Packaging_1, U8G2E_OPTION_FUNC},
    {"KEY Option2", 1, U8G2E_OPTION_KEY},
    {"PCT Option3", 0, U8G2E_OPTION_PCT},
    {"NUM Option4", 123.123456, U8G2E_OPTION_NUM},
    {"NUM Option5", 134045, U8G2E_OPTION_NUM},
    {"Text Option6", 1, U8G2E_OPTION_KEY},
    {"Text Option7", 0, U8G2E_OPTION_KEY}};
```

```c
void Packaging_1(void)
{
  u8g2.setFont(u8g2_font_6x10_mf);
  U8G2E_PromptWindow("Put button 2 to restart menu\n     OK", true);
  u8g2.setFont(u8g2_font_5x7_mf);
  U8G2E_PromptWindow("Put button 1 to restart menu\n     OK", true);
}
```

---

### **动画模式枚举 (Mode)**

```cpp
typedef enum {
    Slow = 1,       // 缓动模式（平滑减速）
    Elasticity = 2  // 弹性模式（带回弹效果）
} Mode;
```

#### 模式对比
| 模式         | 特性                                            | 适用场景               |
| ------------ | ----------------------------------------------- | ---------------------- |
| `Slow`       | 线性插值<br>自动限速<br>稳定无振荡              | 菜单滚动、进度条动画   |
| `Elasticity` | 弹簧物理模型<br>可配置刚度/阻尼<br>边界回弹效果 | 弹窗弹出、特殊强调动画 |

---

### **按键映射枚举 (Key_mapping)**

```cpp
typedef enum {
    KEY_NULL = 0,    // 无按键输入
    KEY_DOWN = 3,    // 下键（菜单向下/数值减少）
    KEY_UP = 2,      // 上键（菜单向上/数值增加）
    KEY_CONFIRM = 1, // 确认键（进入编辑/执行函数）
    KEY_EXIT = 4     // 退出键（返回上级菜单）
} Key_mapping;
```

#### 硬件适配说明
- 需通过 `U8G2E_SignKeyFun` 注册外部扫描函数。
- 扫描函数应返回上述枚举值之一。
- 按键检测参考（Arduino 环境）：
  ```cpp
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
      uint8_t result = (Count >= 20) ? 4 : 1; // 三目运算符简化判断
      Count = 0;
      return result;
    }
  
    if (digitalRead(Button1) == HIGH && digitalRead(Button2) == HIGH && digitalRead(Button3) == HIGH)
      Button_Hold = false;
    return 0;
  }
  ```

---

## 4. 函数详解

### **U8G2E_Init**

```c
void U8G2E_Init(bool Enable, const char *str1, const char *str2)
```

#### 输入参数
| 参数    | 类型         | 说明                          |
|---------|--------------|-------------------------------|
| Enable  | bool         | 是否启用开机动画              |
| str1    | const char*  | 开机动画主标题（最多10字符）  |
| str2    | const char*  | 开机动画副标题（最多17字符）  |

#### 输出参数
无

#### 函数功能
- 启动 OLED 设备并设置基础显示参数。
- 设置初始字体为 `u8g2_font_6x12_mf`。
- 根据 `Enable` 参数决定是否显示开机动画。

#### NOTE
- 如需更换屏幕型号或引脚，需根据实际 OLED 显示器实例化 `u8g2` 对象（默认为 7 线硬件 SPI，引脚在头文件中定义）。

---

### **U8G2E_MoveCursor**

```c
void U8G2E_MoveCursor(int GoalValue, float *CurrentValue, uint8_t Mode)
```

#### 输入参数
| 参数         | 类型     | 说明                          |
|:------------:|:--------:|:-----------------------------:|
| GoalValue    | int      | 目标位置值                   |
| CurrentValue | float*   | 当前值指针（会被修改）       |
| Mode         | uint8_t  | 移动模式（`Slow` 或 `Elasticity`） |

#### 输出参数
通过指针修改 `CurrentValue` 的值。

#### 函数功能
- 实现数值平滑过渡效果。
- 提供两种动画模式：
  - `Slow`：缓动效果（线性减速）。
  - `Elasticity`：弹性效果（带阻尼震荡）。

#### NOTE
- 函数内部不包含延时，需在外部控制帧率。

---

### **U8G2E_Blurring**

```c
void U8G2E_Blurring(void)
```

#### 输入参数
无

#### 输出参数
无

#### 函数功能
- 创建背景虚化效果，通过绘制棋盘格图案实现半透明遮挡感。
- 对整个屏幕生效。

#### NOTE
- 通常配合 `U8G2E_SaveBuffer` / `U8G2E_CoverBuffer` 使用以恢复背景。

---

### **U8G2E_DrawWrappedText**

```c
void U8G2E_DrawWrappedText(u8g2_uint_t x, u8g2_uint_t y, const char *str, u8g2_uint_t max_width)
```

#### 输入参数
| 参数      | 类型         | 说明                  |
|-----------|--------------|-----------------------|
| x         | u8g2_uint_t  | 起始 X 坐标           |
| y         | u8g2_uint_t  | 起始 Y 坐标（顶部）   |
| str       | const char*  | 要显示的字符串        |
| max_width | u8g2_uint_t  | 单行最大宽度（像素）  |

#### 输出参数
无

#### 函数功能
- 自动换行显示文本。
- 支持识别 `\n` 强制换行。
- 自动跳过行首空格。

#### NOTE
- 需在调用前设置好字体。

---

### **U8G2E_StrHeight**

```c
uint8_t U8G2E_StrHeight(const char *str, uint8_t max_width, uint8_t x)
```

#### 输入参数
| 参数      | 类型         | 说明                        |
|-----------|--------------|-----------------------------|
| str       | const char*  | 要计算的字符串              |
| max_width | uint8_t      | 单行最大宽度（像素）        |
| x         | uint8_t      | 起始 X 坐标（用于换行计算） |

#### 输出参数
无

#### 返回值
- 返回字符串在自动换行后的总高度（像素）。

#### 函数功能
- 模拟 `U8G2E_DrawWrappedText` 的换行逻辑，计算所需总高度。
- 便于动态调整窗口大小。

#### NOTE
- 需在调用前设置字体，结果基于 `u8g2.getMaxCharHeight()`。

---

### **U8G2E_SaveBuffer**

```c
void U8G2E_SaveBuffer(void)
```

#### 输入参数
无

#### 输出参数
无

#### 函数功能
- 使用字节级 RLE 编码压缩当前显示缓冲区，并保存到内部静态备份数组。

#### NOTE
- 备份缓冲区大小为 2048 字节（`U8G2E_BACKUP_BUF_MAX`），适用于 128×64 单色屏的 RLE 压缩存储。
- 若缓冲区内容复杂，可能无法完整保存（RLE 最坏情况编码后大小可能超过限制）。

---

### **U8G2E_CoverBuffer**

```c
void U8G2E_CoverBuffer(void)
```

#### 输入参数
无

#### 输出参数
无

#### 函数功能
- 从备份缓冲区解压数据并覆盖当前显示缓冲区，恢复到之前保存的状态。

#### NOTE
- 需与 `U8G2E_SaveBuffer` 配对使用。
- 若备份数据损坏或长度异常，恢复可能不完整。

---

### **U8G2E_SignKeyFun**

```c
void U8G2E_SignKeyFun(int Put_in_fun(void))
```

#### 输入参数
| 参数        | 类型            | 说明                      |
|-------------|-----------------|---------------------------|
| Put_in_fun  | int (*)(void)   | 按键扫描函数指针（无参，返回 int） |

#### 输出参数
无

#### 函数功能
- 注册外部按键扫描函数，供菜单和弹窗模块使用。

#### NOTE
- 必须在调用菜单系统前注册。
- 扫描函数应返回 `Key_mapping` 枚举值，也可自行映射。

---

### **U8G2E_CountDigits**

```c
void U8G2E_CountDigits(double number, int *integer_digits, int *decimal_digits)
```

#### 输入参数
| 参数            | 类型     | 说明                  |
|-----------------|----------|-----------------------|
| number          | double   | 要分析的数值          |
| integer_digits  | int*     | 输出整数部分位数指针  |
| decimal_digits  | int*     | 输出小数部分位数指针（去尾零后） |

#### 输出参数
通过指针返回整数位数和小数位数。

#### 函数功能
- 智能识别数值的整数/小数位数。
- 特殊值处理：`0.0` 视为整数 1 位；`NaN` 或 `Inf` 直接返回，不修改输出。
- 自动忽略小数末尾的无效零。

#### NOTE
- 内部使用 `snprintf` 格式化，缓冲区 32 字节足够常规双精度数。

---

### **U8G2E_StartAnimation**

```c
void U8G2E_StartAnimation(const char *str1, const char *str2)
```

#### 输入参数
| 参数 | 类型        | 说明               |
|------|-------------|--------------------|
| str1 | const char* | 主标题（居中显示） |
| str2 | const char* | 副标题（底部显示） |

#### 输出参数
无

#### 函数功能
- 播放开机动画，流程为：
  1. 中心线展开动画。
  2. 文字滑动入场效果（主标题与副标题使用不同字体）。

#### NOTE
- 总时长约 1 秒（含延时）。

---

### **U8G2E_PromptWindow**

```c
void U8G2E_PromptWindow(const char *str1, bool Key_trigger_enable)
```

#### 输入参数
| 参数               | 类型        | 说明                          |
|--------------------|-------------|-------------------------------|
| str1               | const char* | 提示内容（自动换行）          |
| Key_trigger_enable | bool        | true：按任意键退出；false：延时 1 秒退出 |

#### 输出参数
无

#### 函数功能
- 弹出提示窗口，包含背景虚化、弹性弹出动画。
- 自动计算窗口大小（基于文本高度）。
- 可根据参数选择按键触发关闭或自动关闭。

#### NOTE
- 弹窗前自动保存背景，退出后恢复。
- 需提前设置字体。

---

### **U8G2E_NUMDisplay**

```c
void U8G2E_NUMDisplay(int num, int x, int y, float change[], int W, int H)
```

#### 输入参数
| 参数    | 类型    | 说明                          |
|---------|---------|-------------------------------|
| num     | int     | 要显示的数字（0-9）           |
| x       | int     | 左上角 X 坐标                 |
| y       | int     | 左上角 Y 坐标                 |
| change  | float[] | 长度为 8 的动画参数数组       |
| W       | int     | 数字宽度（像素）              |
| H       | int     | 数字高度（像素）              |

#### 输出参数
无（但 `change` 数组会被函数内部修改以驱动动画）。

#### 函数功能
- 七段数码管风格的丝滑数字显示。
- 根据段码控制横竖线条的长度，实现平滑过渡动画。

#### NOTE
- `change` 数组长度必须为 8，否则越界访问可能导致程序崩溃。
- 数组需在调用前初始化（通常静态分配）。

**使用例（数字时钟）**：

```c
float change_H1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
float change_H2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
float change_M1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
float change_M2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
float change_S1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
float change_S2[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void Display_Mode4(void)
{
  int Hour = rtc.getHour(true);
  int Minute = rtc.getMinute();
  int Second = rtc.getSecond();
  do
  {
    u8g2.clearBuffer();

    NUM_Display(Hour / 10, 14, 20, change_H1, 10, 15);
    NUM_Display(Hour % 10, 14 + 15, 20, change_H2, 10, 15);

    u8g2.drawVLine(46, 25, 5);
    u8g2.drawVLine(46, 40, 5);

    NUM_Display(Minute / 10, 51, 20, change_M1, 10, 15);
    NUM_Display(Minute % 10, 51 + 15, 20, change_M2, 10, 15);

    u8g2.drawVLine(83, 25, 5);
    u8g2.drawVLine(83, 40, 5);

    NUM_Display(Second / 10, 88, 20, change_S1, 10, 15);
    NUM_Display(Second % 10, 88 + 15, 20, change_S2, 10, 15);

    u8g2.sendBuffer();
    delay(15);
    Refresh_Time++;
  } while (Refresh_Time < 15);
  Refresh_Time = 0;
}
```

---

### **U8G2E_MenuDisplay**

```c
void U8G2E_MenuDisplay(U8G2E_MenuOption MenuOption_ARR[], uint8_t valid_num)
```

#### 输入参数
| 参数       | 类型                | 说明                         |
|------------|---------------------|------------------------------|
| MenuOption_ARR | U8G2E_MenuOption[] | 菜单选项结构体数组           |
| valid_num  | uint8_t             | 有效的菜单选项数量           |

#### 输出参数
无（但数组中的坐标值会被函数动态修改）。

#### 函数功能
- 显示交互式菜单，支持上下导航、确认和退出。
- 包含进入/退出动画、弹性滚动和边界回弹效果。
- 根据选项类型调用相应的编辑界面或执行函数。

#### NOTE
- 需提前调用 `U8G2E_Init` 初始化显示设备，并注册按键扫描函数（`U8G2E_SignKeyFun`）。
- 菜单项数量应适配显示区域高度（推荐不超过 5 项）。

---

### **U8G2E_MenuOptionDisplay**

```c
void U8G2E_MenuOptionDisplay(U8G2E_MenuOption MenuOption_Member)
```

#### 输入参数
| 参数 | 类型 | 说明 |
|------|------|------|
| MenuOption_Member | U8G2E_MenuOption | 单个菜单选项结构体 |

#### 输出参数
无

#### 函数功能
- 根据菜单项类型渲染不同的显示样式（开关、百分比、数值、文本、函数）。
- 处理 ON/OFF 状态框、百分号格式、数值格式化等。

#### NOTE
- 由 `U8G2E_MenuDisplay` 内部调用，通常无需直接使用。
- 字体由调用方设置，本函数不修改字体。

---

### **U8G2E_MenuExecute**

```c
void U8G2E_MenuExecute(U8G2E_MenuOption *MenuOption_Member)
```

#### 输入参数
| 参数 | 类型 | 说明 |
|------|------|------|
| MenuOption_Member | U8G2E_MenuOption* | 指向菜单选项结构体的指针 |

#### 输出参数
无（但会修改结构体中的 `Value` 字段，或执行函数）。

#### 函数功能
- 根据选项类型执行相应操作：
  - `U8G2E_OPTION_KEY`：切换 `Value` 的 0/1 状态。
  - `U8G2E_OPTION_PCT`：调用 `U8G2E_PCT_ACTION` 编辑百分比。
  - `U8G2E_OPTION_NUM`：调用 `U8G2E_NUM_ACTION` 编辑数值。
  - `U8G2E_OPTION_FUNC`：将 `Value` 强制转换为函数指针并执行。

#### NOTE
- 由 `U8G2E_MenuDisplay` 自动调用，无需手动调用。

---

### **U8G2E_PCT_ACTION**

```c
void U8G2E_PCT_ACTION(U8G2E_MenuOption *MenuOption_Member)
```

#### 输入参数
| 参数 | 类型 | 说明 |
|------|------|------|
| MenuOption_Member | U8G2E_MenuOption* | 指向菜单选项结构体的指针 |

#### 输出参数
无（但会修改结构体中的 `Value`，限制在 0~100 范围内）。

#### 函数功能
- 弹出百分比编辑界面，包含进度条和数值显示。
- 支持按键调整数值（上/下键增减，确认键切换精度）。
- 背景虚化，窗口弹性弹出/退出。

#### NOTE
- 该函数会阻塞直到用户退出。
- 内部自动保存并恢复背景。

---

### **U8G2E_NUM_ACTION**

```c
void U8G2E_NUM_ACTION(U8G2E_MenuOption *MenuOption_Member)
```

#### 输入参数
| 参数 | 类型 | 说明 |
|------|------|------|
| MenuOption_Member | U8G2E_MenuOption* | 指向菜单选项结构体的指针 |

#### 输出参数
无（但会修改结构体中的 `Value`）。

#### 函数功能
- 弹出数值编辑界面，自动识别整数/小数部分。
- 支持按键逐位调整（上/下键增减，确认键切换位）。
- 对于极大或极小的数值，自动切换显示格式（使用 `%f` 或 `%g`）。

#### NOTE
- 该函数会阻塞直到用户退出。
- 背景虚化，内部使用 `U8G2E_CountDigits` 确定小数位。

---

> **注意**：所有函数均需在已初始化 `u8g2` 对象的前提下调用，且部分功能依赖按键扫描函数的注册。请参考示例代码进行配置。