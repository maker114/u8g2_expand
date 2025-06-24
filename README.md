

# U8G2_Expand 库文档

## 1. 简介
`U8G2_Expand` 是一个基于 U8g2 图形库的扩展库，专为 Arduino 平台开发,为著名OLED驱动库 `u8g2`提供包括菜单在内的拓展，提供丰富的动画效果和高级 UI 组件。主要特性包括：
- 平滑动画系统（缓动/弹性效果）
- 预置开机动画
- 自动换行文本渲染
- 弹窗提示系统
- 动态数字显示
- 交互式菜单系统（支持多种选项类型）
- 背景虚化效果
- 缓冲区管理
- 按键事件处理

---

## 2. 函数列表

### 核心功能
| 函数 | 说明 |
|------|------|
| `U8G2E_Init()` | 初始化 OLED 并可选启动开机动画 |
| `U8G2E_MoveCursor()` | 数字平滑移动（缓动/弹性模式） |
| `U8G2E_Blurring()` | 背景虚化效果 |
| `U8G2E_DrawWrappedText()` | 自动换行文本渲染 |
| `U8G2E_StrHight()` | 计算文本渲染高度 |
| `U8G2E_SaveBuffer()` | 保存显示缓冲区 |
| `U8G2E_CoverBuffer()` | 恢复显示缓冲区 |
| `U8G2E_SignKeyFun()` | 注册按键扫描函数 |

### 动画组件
| 函数 | 说明 |
|------|------|
| `U8G2E_StartAnimation()` | 开机动画（带双行标题） |
| `U8G2E_PromptWindow()` | 弹窗提示（支持按键触发） |
| `U8G2E_NUMDisplay()` | 丝滑数字显示（七段数码管风格） |

### 菜单系统
| 函数 | 说明 |
|------|------|
| `U8G2E_MenuDisplay()` | 丝滑移动菜单主控函数 |
| `U8G2E_MenuOptionDisplay()` | 渲染单个菜单项 |
| `U8G2E_MenuExecute()` | 执行菜单项动作 |
| `U8G2E_PCT_ACTION()` | 百分比选项编辑器 |
| `U8G2E_NUM_ACTION()` | 数字选项编辑器 |
| `U8G2E_CountDigits()` | 计算数字的整数/小数位数 |

---

## 3. 结构体与枚举

### 按键映射枚举
```cpp
typedef enum {
    KEY_NULL = 0,    // 无按键
    KEY_DOWN = 3,    // 向下键
    KEY_UP = 2,      // 向上键
    KEY_CONFIRM = 1, // 确认键
    KEY_EXIT = 4     // 退出键
} Key_mapping;

```

以下是补充的 **3. 结构体与枚举** 部分内容，按照原有文档格式和风格编写：

---

## 3. 结构体与枚举

### **菜单选项结构体(U8G2E_MenuOption)**  
```cpp
typedef struct {
    String Title;       // 选项标题（支持动态字符串）
    double Value;       // 选项数值（用于存储开关状态/百分比/数值等）
    uint8_t Kind;       // 选项类型（见U8G2E_OptionType枚举）
    float X_Coordinate; // 动态X坐标（用于实现弹性动画）
    float Y_Coordinate; // 动态Y坐标（用于实现滚动效果）
} U8G2E_MenuOption;
```

#### 成员说明
| 成员           | 类型      | 说明                                                         |
| -------------- | --------- | ------------------------------------------------------------ |
| `Title`        | `String`  | 菜单项显示文本,文本暂不支持除英文外的字符                    |
| `Value`        | `double`  | 多功能数值：<br>? 开关类型：0/1<br>? 百分比：0.0~100.0<br>? 数值：任意浮点数 |
| `Kind`         | `uint8_t` | 决定选项行为，需使用`U8G2E_OptionType`枚举值                 |
| `X_Coordinate` | `float`   | 动态X坐标（配合`U8G2E_MoveCursor`实现横向弹性效果）          |
| `Y_Coordinate` | `float`   | 动态Y坐标（配合菜单滚动系统使用）                            |

---

### **选项类型枚举(U8G2E_OptionType)**  
```cpp
typedef enum {
    U8G2E_OPTION_KEY = 0,  // 开关类型（显示ON/OFF状态框）
    U8G2E_OPTION_NUM = 1,  // 数值类型（可编辑整数/小数）
    U8G2E_OPTION_PCT = 2,  // 百分比类型（自动限制0~100范围）
    U8G2E_OPTION_TEXT = 3, // 纯文本类型（仅显示，不可交互）
    U8G2E_OPTION_FUNC = 4  // 函数类型（点击执行回调函数）
} U8G2E_OptionType;
```

#### 枚举值说明
| 值                  | 说明                                              |
| ------------------- | ------------------------------------------------- |
| `U8G2E_OPTION_KEY`  | 显示为开关控件，`Value`字段应为0(OFF)或1(ON)      |
| `U8G2E_OPTION_NUM`  | 进入数值编辑模式                                  |
| `U8G2E_OPTION_PCT`  | 显示百分号，数值自动限制在0~100范围               |
| `U8G2E_OPTION_TEXT` | 不可选文本，通常用于菜单分类标题或信息表达        |
| `U8G2E_OPTION_FUNC` | 点击后执行`Value`字段指向的函数（需强制类型转换） |

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

### **动画模式枚举**(Mode)  
```cpp
typedef enum {
    Slow = 1,       // 缓动模式（平滑减速）
    Elasticity = 2  // 弹性模式（带回弹效果）
} Mode;
```

#### 模式对比
| 模式         | 特性                                                  | 适用场景               |
| ------------ | ----------------------------------------------------- | ---------------------- |
| `Slow`       | ? 线性插值<br>? 自动限速<br>? 稳定无振荡              | 菜单滚动、进度条动画   |
| `Elasticity` | ? 弹簧物理模型<br>? 可配置刚度/阻尼<br>? 边界回弹效果 | 弹窗弹出、特殊强调动画 |

---

### **按键映射枚举**(Key_mapping)  
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
- 需通过`U8G2E_SignKeyFun`注册外部扫描函数
- 扫描函数应返回上述枚举值之一
- 按键检测参考（Arduino环境）：
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
      Count = 0;                              // 统一重置计数器
      return result;
    }
  
    if (digitalRead(Button1) == HIGH && digitalRead(Button2) == HIGH && digitalRead(Button3) == HIGH)
      Button_Hold = false;
    return 0;
  }
  ```

---

##  4.函数详解

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

#### 函数功能
- 启动OLED设备并设置基础显示参数
- 设置初始字体为`u8g2_font_6x12_mf`
- 根据参数决定是否显示开机动画

#### NOTE
- 需要根据实际OLED显示器型号实例化`u8g2`对象

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
| Mode         | uint8_t  | 移动模式（Slow/Elasticity） |

#### 输出参数
通过指针修改CurrentValue的值

#### 函数功能
- 实现数值平滑过渡效果
- 提供两种动画模式(来自枚举值Mode)：
  - `Slow`：缓动效果（线性减速）
  - `Elasticity`：弹性效果（带阻尼震荡）

#### NOTE
- 不包含延时，需在外部控制帧率

---

### **U8G2E_Blurring**

```c
void U8G2E_Blurring(void)
```

#### 函数功能
- 创建背景虚化效果
- 通过绘制棋盘格图案实现半透明效果

#### NOTE
- 会对整个屏幕进行虚化效果

---

### **U8G2E_DrawWrappedText**

```c
void U8G2E_DrawWrappedText(u8g2_uint_t x, u8g2_uint_t y, const char *str, u8g2_uint_t max_width)
```

#### 输入参数
| 参数      | 类型         | 说明                  |
|-----------|--------------|-----------------------|
| x         | u8g2_uint_t  | 起始X坐标            |
| y         | u8g2_uint_t  | 起始Y坐标            |
| str       | const char*  | 要显示的字符串       |
| max_width | u8g2_uint_t  | 最大行宽（像素）     |

#### 函数功能
- 自动换行文本显示
- 支持识别`\n`换行符
- 自动跳过行首空格

#### NOTE
- 需在调用前设置好字体

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

#### 动画流程
1. 中心线展开动画
2. 文字滑动入场效果
3. 使用不同字体区分主副标题

#### NOTE
- 总时长约1秒（含延时）

---

### **U8G2E_PromptWindow**

```c
void U8G2E_PromptWindow(const char *str1, bool Key_trigger_enable)
```

#### 输入参数
| 参数               | 类型        | 说明                          |
|--------------------|-------------|-------------------------------|
| str1               | const char* | 提示内容                      |
| Key_trigger_enable | bool        | 是否启用按键触发关闭          |

#### 动画效果
- 弹性弹出窗口
- 背景虚化
- 自动计算窗口大小
- 支持按键关闭或自动关闭

---

### **U8G2E_MenuDisplay**

```c
void U8G2E_MenuDisplay(U8G2E_MenuOption MenuOption_ARR[], uint8_t valid_num)
```

#### 输入参数
| 参数       | 类型                | 说明                |
|------------|---------------------|---------------------|
| MenuOption_ARR | U8G2E_MenuOption[] | 菜单选项数组        |
| valid_num  | uint8_t             | 有效菜单项数量      |

#### 功能特点
- 丝滑进入/退出动画
- 弹性滚动效果
- 支持5种菜单类型：
  - 开关选项
  - 数值调整
  - 百分比调整
  - 纯文本
  - 功能执行

#### NOTE
- 依赖`KEY_Scan()`函数获取输入

---

### **U8G2E_MenuDisplay**

**函数原型**  
```c
void U8G2E_MenuDisplay(U8G2E_MenuOption MenuOption_ARR[], uint8_t valid_num)
```

#### **输入参数**
| 参数名 | 类型 | 说明 |
|--------|------|------|
| MenuOption_ARR | U8G2E_MenuOption[] | 菜单选项结构体数组 |
| valid_num | uint8_t | 有效的菜单选项数量 |


#### **函数功能**
- 实现动态交互式菜单显示系统，主要功能包括：
  - 平滑的进入/退出动画效果
  - 支持5种菜单项类型（开关/数值/百分比/文本/函数）
  - 弹性边界检测（顶部/底部回弹效果）
  - 光标跟随动画
  - 自动内容滚动

### **U8G2E_MenuDisplay**

```c
void U8G2E_MenuDisplay(U8G2E_MenuOption MenuOption_ARR[], uint8_t valid_num)
```

#### 输入参数

| 参数 | 类型 | 说明 |
|------|------|------|
| MenuOption_ARR | U8G2E_MenuOption[] | 菜单选项结构体数组 |
| valid_num | uint8_t | 有效的菜单选项数量 |

#### 输出参数
暂无

#### 函数功能
- 显示交互式菜单界面，支持上下导航和选择操作
- 实现菜单项的平滑滚动和弹性边界效果
- 支持多种菜单项类型（开关/数值/百分比/文本/函数）
- 提供选中项的高亮显示和操作反馈

#### NOTE
- 需要提前调用U8G2E_Init初始化显示设备
- 菜单项数组中的坐标值会被函数动态修改
- 必须预先注册按键扫描函数U8G2E_SignKeyFun
- 菜单项数量应适配显示区域高度

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
暂无

#### 函数功能
- 根据菜单项类型渲染不同的显示样式
- 处理开关状态的ON/OFF显示
- 格式化显示数值和百分比数据
- 保持菜单项的对齐和布局一致性

#### NOTE
- 被U8G2E_MenuDisplay内部调用
- 需要根据实际需求调整显示格式

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
暂无

#### 函数功能
- 执行菜单项对应的操作
- 切换开关状态
- 调用数值/百分比编辑界面
- 执行注册的回调函数

#### NOTE
- 通过U8G2E_MenuDisplay自动调用
- 函数指针需要强制类型转换

###  **U8G2E_MenuExecute**

```c
void U8G2E_MenuExecute(U8G2E_MenuOption *MenuOption_Member)
```

#### 输入参数

|      参数      |        类型         |           说明           |
| :------------: | :-----------------: | :----------------------: |
| MenuOption_Member | U8G2E_MenuOption* | 指向菜单选项结构体的指针 |

#### 输出参数
暂无

#### 函数功能
- 根据菜单选项的类型执行相应的动作
- 处理开关类型（U8G2E_OPTION_KEY）的选项，切换其值
- 处理百分比类型（U8G2E_OPTION_PCT）的选项，调用U8G2E_PCT_ACTION函数
- 处理数字类型（U8G2E_OPTION_NUM）的选项，调用U8G2E_NUM_ACTION函数
- 处理函数类型（U8G2E_OPTION_FUNC）的选项，执行绑定的函数

#### NOTE
- 该函数通常由U8G2E_MenuDisplay调用，无需直接调用
- 函数类型选项的Value成员应存储函数的地址

---

###  **U8G2E_PCT_ACTION**

```c
void U8G2E_PCT_ACTION(U8G2E_MenuOption *MenuOption_Member)
```

#### 输入参数

|      参数      |        类型         |           说明           |
| :------------: | :-----------------: | :----------------------: |
| MenuOption_Member | U8G2E_MenuOption* | 指向菜单选项结构体的指针 |

#### 输出参数
暂无

#### 函数功能
- 处理百分比类型的菜单选项
- 提供交互界面用于调整百分比值
- 支持通过按键增加/减少百分比值
- 支持切换调整的精度（小数点位数）
- 提供动画效果和视觉反馈

#### NOTE
- 该函数会阻塞直到用户退出
- 背景会虚化以突出当前操作窗口

---

###  **U8G2E_NUM_ACTION**

```c
void U8G2E_NUM_ACTION(U8G2E_MenuOption *MenuOption_Member)
```

#### 输入参数

|      参数      |        类型         |           说明           |
| :------------: | :-----------------: | :----------------------: |
| MenuOption_Member | U8G2E_MenuOption* | 指向菜单选项结构体的指针 |

#### 输出参数
暂无

#### 函数功能
- 处理数字类型的菜单选项
- 提供交互界面用于调整数值
- 自动识别数字的整数和小数部分
- 支持通过按键增加/减少数值
- 支持切换调整的精度（位数）
- 提供动画效果和视觉反馈

#### NOTE
- 该函数会阻塞直到用户退出
- 背景会虚化以突出当前操作窗口
- 对于非常大或非常小的数字，会自动切换显示格式

---

### **U8G2E_StrHight**

```c
uint8_t U8G2E_StrHight(const char *str, uint8_t max_width, uint8_t x)
```

#### 参数说明
| 参数      | 类型         | 说明                  |
|-----------|--------------|-----------------------|
| str       | const char*  | 要计算的字符串       |
| max_width | uint8_t      | 最大宽度（像素）     |
| x         | uint8_t      | 起始X坐标            |

#### 返回值
- 返回字符串在指定宽度下的渲染高度（像素）

#### 功能描述
- 计算字符串在自动换行后的总高度
- 支持识别`\n`换行符
- 自动跳过行首空格

#### NOTE
- 需在调用前设置字体

---

### **U8G2E_SaveBuffer**

```c
void U8G2E_SaveBuffer(void)
```

#### 功能描述
- 保存当前显示缓冲区内容到备份缓冲区
- 用于实现界面恢复或叠加效果

#### NOTE
- 备份缓冲区大小默认为`8 * 64 * 128`字节，可根据自己的显示器自行调整

---

### **U8G2E_CoverBuffer**

```c
void U8G2E_CoverBuffer(void)
```

#### 功能描述
- 从备份缓冲区恢复显示内容
- 需配合`U8G2E_SaveBuffer`使用

#### NOTE
- 恢复前需确保备份数据有效

---

### **U8G2E_SignKeyFun**

```c
void U8G2E_SignKeyFun(int Put_in_fun(void))
```

#### 参数说明
| 参数        | 类型            | 说明                  |
|-------------|-----------------|-----------------------|
| Put_in_fun  | int (*)(void)   | 按键扫描函数指针      |

#### 功能描述
- 注册外部按键扫描函数
- 函数返回`Key_mapping`枚举值

#### NOTE
- 必须在调用菜单系统前注册以使用按键
- 可以根据传回的数据配合枚举自行映射功能

---

### **U8G2E_CountDigits**

```c
void U8G2E_CountDigits(double number, int *integer_digits, int *decimal_digits)
```

#### 参数说明
| 参数            | 类型     | 说明                  |
|-----------------|----------|-----------------------|
| number          | double   | 要分析的数值          |
| integer_digits  | int*     | 输出整数位数指针      |
| decimal_digits  | int*     | 输出小数位数指针      |

#### 功能描述
- 智能识别数值的整数/小数位数
- 自动忽略末尾无效的0

---

### **U8G2E_NUMDisplay**

```c
void U8G2E_NUMDisplay(int num, int x, int y, float change[], int W, int H)
```

#### 参数说明
| 参数    | 类型    | 说明                  |
|---------|---------|-----------------------|
| num     | int     | 要显示的数字（0-9）   |
| x       | int     | 起始X坐标            |
| y       | int     | 起始Y坐标            |
| change  | float[] | 动画参数数组（长度8） |
| W       | int     | 数字宽度             |
| H       | int     | 数字高度             |

#### 功能描述
- 实现七段数码管的丝滑显示效果
- 支持动态扩展/收缩动画
- 内部使用`Slow`缓动模式

#### NOTE
- 需要配置数组

#### 使用例

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
    u8g2.clearBuffer(); // 清空缓冲区

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

    u8g2.sendBuffer(); // 发送缓冲区数据

    delay(15);
    Refresh_Time++;
  } while (Refresh_Time < 15); // 强制刷新15次，避免出现卡顿现象
  Refresh_Time = 0;
}

```

