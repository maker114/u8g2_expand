

# U8G2_Expand ���ĵ�

## 1. ���
`U8G2_Expand` ��һ������ U8g2 ͼ�ο����չ�⣬רΪ Arduino ƽ̨����,Ϊ����OLED������ `u8g2`�ṩ�����˵����ڵ���չ���ṩ�ḻ�Ķ���Ч���͸߼� UI �������Ҫ���԰�����
- ƽ������ϵͳ������/����Ч����
- Ԥ�ÿ�������
- �Զ������ı���Ⱦ
- ������ʾϵͳ
- ��̬������ʾ
- ����ʽ�˵�ϵͳ��֧�ֶ���ѡ�����ͣ�
- �����黯Ч��
- ����������
- �����¼�����

---

## 2. �����б�

### ���Ĺ���
| ���� | ˵�� |
|------|------|
| `U8G2E_Init()` | ��ʼ�� OLED ����ѡ������������ |
| `U8G2E_MoveCursor()` | ����ƽ���ƶ�������/����ģʽ�� |
| `U8G2E_Blurring()` | �����黯Ч�� |
| `U8G2E_DrawWrappedText()` | �Զ������ı���Ⱦ |
| `U8G2E_StrHight()` | �����ı���Ⱦ�߶� |
| `U8G2E_SaveBuffer()` | ������ʾ������ |
| `U8G2E_CoverBuffer()` | �ָ���ʾ������ |
| `U8G2E_SignKeyFun()` | ע�ᰴ��ɨ�躯�� |

### �������
| ���� | ˵�� |
|------|------|
| `U8G2E_StartAnimation()` | ������������˫�б��⣩ |
| `U8G2E_PromptWindow()` | ������ʾ��֧�ְ��������� |
| `U8G2E_NUMDisplay()` | ˿��������ʾ���߶�����ܷ�� |

### �˵�ϵͳ
| ���� | ˵�� |
|------|------|
| `U8G2E_MenuDisplay()` | ˿���ƶ��˵����غ��� |
| `U8G2E_MenuOptionDisplay()` | ��Ⱦ�����˵��� |
| `U8G2E_MenuExecute()` | ִ�в˵���� |
| `U8G2E_PCT_ACTION()` | �ٷֱ�ѡ��༭�� |
| `U8G2E_NUM_ACTION()` | ����ѡ��༭�� |
| `U8G2E_CountDigits()` | �������ֵ�����/С��λ�� |

---

## 3. �ṹ����ö��

### ����ӳ��ö��
```cpp
typedef enum {
    KEY_NULL = 0,    // �ް���
    KEY_DOWN = 3,    // ���¼�
    KEY_UP = 2,      // ���ϼ�
    KEY_CONFIRM = 1, // ȷ�ϼ�
    KEY_EXIT = 4     // �˳���
} Key_mapping;

```

�����ǲ���� **3. �ṹ����ö��** �������ݣ�����ԭ���ĵ���ʽ�ͷ���д��

---

## 3. �ṹ����ö��

### **�˵�ѡ��ṹ��(U8G2E_MenuOption)**  
```cpp
typedef struct {
    String Title;       // ѡ����⣨֧�ֶ�̬�ַ�����
    double Value;       // ѡ����ֵ�����ڴ洢����״̬/�ٷֱ�/��ֵ�ȣ�
    uint8_t Kind;       // ѡ�����ͣ���U8G2E_OptionTypeö�٣�
    float X_Coordinate; // ��̬X���꣨����ʵ�ֵ��Զ�����
    float Y_Coordinate; // ��̬Y���꣨����ʵ�ֹ���Ч����
} U8G2E_MenuOption;
```

#### ��Ա˵��
| ��Ա           | ����      | ˵��                                                         |
| -------------- | --------- | ------------------------------------------------------------ |
| `Title`        | `String`  | �˵�����ʾ�ı�,�ı��ݲ�֧�ֳ�Ӣ������ַ�                    |
| `Value`        | `double`  | �๦����ֵ��<br>? �������ͣ�0/1<br>? �ٷֱȣ�0.0~100.0<br>? ��ֵ�����⸡���� |
| `Kind`         | `uint8_t` | ����ѡ����Ϊ����ʹ��`U8G2E_OptionType`ö��ֵ                 |
| `X_Coordinate` | `float`   | ��̬X���꣨���`U8G2E_MoveCursor`ʵ�ֺ�����Ч����          |
| `Y_Coordinate` | `float`   | ��̬Y���꣨��ϲ˵�����ϵͳʹ�ã�                            |

---

### **ѡ������ö��(U8G2E_OptionType)**  
```cpp
typedef enum {
    U8G2E_OPTION_KEY = 0,  // �������ͣ���ʾON/OFF״̬��
    U8G2E_OPTION_NUM = 1,  // ��ֵ���ͣ��ɱ༭����/С����
    U8G2E_OPTION_PCT = 2,  // �ٷֱ����ͣ��Զ�����0~100��Χ��
    U8G2E_OPTION_TEXT = 3, // ���ı����ͣ�����ʾ�����ɽ�����
    U8G2E_OPTION_FUNC = 4  // �������ͣ����ִ�лص�������
} U8G2E_OptionType;
```

#### ö��ֵ˵��
| ֵ                  | ˵��                                              |
| ------------------- | ------------------------------------------------- |
| `U8G2E_OPTION_KEY`  | ��ʾΪ���ؿؼ���`Value`�ֶ�ӦΪ0(OFF)��1(ON)      |
| `U8G2E_OPTION_NUM`  | ������ֵ�༭ģʽ                                  |
| `U8G2E_OPTION_PCT`  | ��ʾ�ٷֺţ���ֵ�Զ�������0~100��Χ               |
| `U8G2E_OPTION_TEXT` | ����ѡ�ı���ͨ�����ڲ˵�����������Ϣ���        |
| `U8G2E_OPTION_FUNC` | �����ִ��`Value`�ֶ�ָ��ĺ�������ǿ������ת���� |

**ʾ�����飺**

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

### **����ģʽö��**(Mode)  
```cpp
typedef enum {
    Slow = 1,       // ����ģʽ��ƽ�����٣�
    Elasticity = 2  // ����ģʽ�����ص�Ч����
} Mode;
```

#### ģʽ�Ա�
| ģʽ         | ����                                                  | ���ó���               |
| ------------ | ----------------------------------------------------- | ---------------------- |
| `Slow`       | ? ���Բ�ֵ<br>? �Զ�����<br>? �ȶ�����              | �˵�����������������   |
| `Elasticity` | ? ��������ģ��<br>? �����øն�/����<br>? �߽�ص�Ч�� | ��������������ǿ������ |

---

### **����ӳ��ö��**(Key_mapping)  
```cpp
typedef enum {
    KEY_NULL = 0,    // �ް�������
    KEY_DOWN = 3,    // �¼����˵�����/��ֵ���٣�
    KEY_UP = 2,      // �ϼ����˵�����/��ֵ���ӣ�
    KEY_CONFIRM = 1, // ȷ�ϼ�������༭/ִ�к�����
    KEY_EXIT = 4     // �˳����������ϼ��˵���
} Key_mapping;
```

#### Ӳ������˵��
- ��ͨ��`U8G2E_SignKeyFun`ע���ⲿɨ�躯��
- ɨ�躯��Ӧ��������ö��ֵ֮һ
- �������ο���Arduino��������
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
      uint8_t result = (Count >= 20) ? 4 : 1; // ��Ŀ��������ж�
      Count = 0;                              // ͳһ���ü�����
      return result;
    }
  
    if (digitalRead(Button1) == HIGH && digitalRead(Button2) == HIGH && digitalRead(Button3) == HIGH)
      Button_Hold = false;
    return 0;
  }
  ```

---

##  4.�������

### **U8G2E_Init**

```c
void U8G2E_Init(bool Enable, const char *str1, const char *str2)
```

#### �������
| ����    | ����         | ˵��                          |
|---------|--------------|-------------------------------|
| Enable  | bool         | �Ƿ����ÿ�������              |
| str1    | const char*  | �������������⣨���10�ַ���  |
| str2    | const char*  | �������������⣨���17�ַ���  |

#### ��������
- ����OLED�豸�����û�����ʾ����
- ���ó�ʼ����Ϊ`u8g2_font_6x12_mf`
- ���ݲ��������Ƿ���ʾ��������

#### NOTE
- ��Ҫ����ʵ��OLED��ʾ���ͺ�ʵ����`u8g2`����

---

### **U8G2E_MoveCursor**

```c
void U8G2E_MoveCursor(int GoalValue, float *CurrentValue, uint8_t Mode)
```

#### �������
| ����         | ����     | ˵��                          |
|:------------:|:--------:|:-----------------------------:|
| GoalValue    | int      | Ŀ��λ��ֵ                   |
| CurrentValue | float*   | ��ǰֵָ�루�ᱻ�޸ģ�       |
| Mode         | uint8_t  | �ƶ�ģʽ��Slow/Elasticity�� |

#### �������
ͨ��ָ���޸�CurrentValue��ֵ

#### ��������
- ʵ����ֵƽ������Ч��
- �ṩ���ֶ���ģʽ(����ö��ֵMode)��
  - `Slow`������Ч�������Լ��٣�
  - `Elasticity`������Ч�����������𵴣�

#### NOTE
- ��������ʱ�������ⲿ����֡��

---

### **U8G2E_Blurring**

```c
void U8G2E_Blurring(void)
```

#### ��������
- ���������黯Ч��
- ͨ���������̸�ͼ��ʵ�ְ�͸��Ч��

#### NOTE
- ���������Ļ�����黯Ч��

---

### **U8G2E_DrawWrappedText**

```c
void U8G2E_DrawWrappedText(u8g2_uint_t x, u8g2_uint_t y, const char *str, u8g2_uint_t max_width)
```

#### �������
| ����      | ����         | ˵��                  |
|-----------|--------------|-----------------------|
| x         | u8g2_uint_t  | ��ʼX����            |
| y         | u8g2_uint_t  | ��ʼY����            |
| str       | const char*  | Ҫ��ʾ���ַ���       |
| max_width | u8g2_uint_t  | ����п����أ�     |

#### ��������
- �Զ������ı���ʾ
- ֧��ʶ��`\n`���з�
- �Զ��������׿ո�

#### NOTE
- ���ڵ���ǰ���ú�����

---

### **U8G2E_StartAnimation**

```c
void U8G2E_StartAnimation(const char *str1, const char *str2)
```

#### �������
| ���� | ����        | ˵��               |
|------|-------------|--------------------|
| str1 | const char* | �����⣨������ʾ�� |
| str2 | const char* | �����⣨�ײ���ʾ�� |

#### ��������
1. ������չ������
2. ���ֻ����볡Ч��
3. ʹ�ò�ͬ����������������

#### NOTE
- ��ʱ��Լ1�루����ʱ��

---

### **U8G2E_PromptWindow**

```c
void U8G2E_PromptWindow(const char *str1, bool Key_trigger_enable)
```

#### �������
| ����               | ����        | ˵��                          |
|--------------------|-------------|-------------------------------|
| str1               | const char* | ��ʾ����                      |
| Key_trigger_enable | bool        | �Ƿ����ð��������ر�          |

#### ����Ч��
- ���Ե�������
- �����黯
- �Զ����㴰�ڴ�С
- ֧�ְ����رջ��Զ��ر�

---

### **U8G2E_MenuDisplay**

```c
void U8G2E_MenuDisplay(U8G2E_MenuOption MenuOption_ARR[], uint8_t valid_num)
```

#### �������
| ����       | ����                | ˵��                |
|------------|---------------------|---------------------|
| MenuOption_ARR | U8G2E_MenuOption[] | �˵�ѡ������        |
| valid_num  | uint8_t             | ��Ч�˵�������      |

#### �����ص�
- ˿������/�˳�����
- ���Թ���Ч��
- ֧��5�ֲ˵����ͣ�
  - ����ѡ��
  - ��ֵ����
  - �ٷֱȵ���
  - ���ı�
  - ����ִ��

#### NOTE
- ����`KEY_Scan()`������ȡ����

---

### **U8G2E_MenuDisplay**

**����ԭ��**  
```c
void U8G2E_MenuDisplay(U8G2E_MenuOption MenuOption_ARR[], uint8_t valid_num)
```

#### **�������**
| ������ | ���� | ˵�� |
|--------|------|------|
| MenuOption_ARR | U8G2E_MenuOption[] | �˵�ѡ��ṹ������ |
| valid_num | uint8_t | ��Ч�Ĳ˵�ѡ������ |


#### **��������**
- ʵ�ֶ�̬����ʽ�˵���ʾϵͳ����Ҫ���ܰ�����
  - ƽ���Ľ���/�˳�����Ч��
  - ֧��5�ֲ˵������ͣ�����/��ֵ/�ٷֱ�/�ı�/������
  - ���Ա߽��⣨����/�ײ��ص�Ч����
  - �����涯��
  - �Զ����ݹ���

### **U8G2E_MenuDisplay**

```c
void U8G2E_MenuDisplay(U8G2E_MenuOption MenuOption_ARR[], uint8_t valid_num)
```

#### �������

| ���� | ���� | ˵�� |
|------|------|------|
| MenuOption_ARR | U8G2E_MenuOption[] | �˵�ѡ��ṹ������ |
| valid_num | uint8_t | ��Ч�Ĳ˵�ѡ������ |

#### �������
����

#### ��������
- ��ʾ����ʽ�˵����棬֧�����µ�����ѡ�����
- ʵ�ֲ˵����ƽ�������͵��Ա߽�Ч��
- ֧�ֶ��ֲ˵������ͣ�����/��ֵ/�ٷֱ�/�ı�/������
- �ṩѡ����ĸ�����ʾ�Ͳ�������

#### NOTE
- ��Ҫ��ǰ����U8G2E_Init��ʼ����ʾ�豸
- �˵��������е�����ֵ�ᱻ������̬�޸�
- ����Ԥ��ע�ᰴ��ɨ�躯��U8G2E_SignKeyFun
- �˵�������Ӧ������ʾ����߶�

---

### **U8G2E_MenuOptionDisplay**

```c
void U8G2E_MenuOptionDisplay(U8G2E_MenuOption MenuOption_Member)
```

#### �������

| ���� | ���� | ˵�� |
|------|------|------|
| MenuOption_Member | U8G2E_MenuOption | �����˵�ѡ��ṹ�� |

#### �������
����

#### ��������
- ���ݲ˵���������Ⱦ��ͬ����ʾ��ʽ
- ������״̬��ON/OFF��ʾ
- ��ʽ����ʾ��ֵ�Ͱٷֱ�����
- ���ֲ˵���Ķ���Ͳ���һ����

#### NOTE
- ��U8G2E_MenuDisplay�ڲ�����
- ��Ҫ����ʵ�����������ʾ��ʽ

---

### **U8G2E_MenuExecute**

```c
void U8G2E_MenuExecute(U8G2E_MenuOption *MenuOption_Member)
```

#### �������

| ���� | ���� | ˵�� |
|------|------|------|
| MenuOption_Member | U8G2E_MenuOption* | ָ��˵�ѡ��ṹ���ָ�� |

#### �������
����

#### ��������
- ִ�в˵����Ӧ�Ĳ���
- �л�����״̬
- ������ֵ/�ٷֱȱ༭����
- ִ��ע��Ļص�����

#### NOTE
- ͨ��U8G2E_MenuDisplay�Զ�����
- ����ָ����Ҫǿ������ת��

###  **U8G2E_MenuExecute**

```c
void U8G2E_MenuExecute(U8G2E_MenuOption *MenuOption_Member)
```

#### �������

|      ����      |        ����         |           ˵��           |
| :------------: | :-----------------: | :----------------------: |
| MenuOption_Member | U8G2E_MenuOption* | ָ��˵�ѡ��ṹ���ָ�� |

#### �������
����

#### ��������
- ���ݲ˵�ѡ�������ִ����Ӧ�Ķ���
- ���������ͣ�U8G2E_OPTION_KEY����ѡ��л���ֵ
- ����ٷֱ����ͣ�U8G2E_OPTION_PCT����ѡ�����U8G2E_PCT_ACTION����
- �����������ͣ�U8G2E_OPTION_NUM����ѡ�����U8G2E_NUM_ACTION����
- ���������ͣ�U8G2E_OPTION_FUNC����ѡ�ִ�а󶨵ĺ���

#### NOTE
- �ú���ͨ����U8G2E_MenuDisplay���ã�����ֱ�ӵ���
- ��������ѡ���Value��ԱӦ�洢�����ĵ�ַ

---

###  **U8G2E_PCT_ACTION**

```c
void U8G2E_PCT_ACTION(U8G2E_MenuOption *MenuOption_Member)
```

#### �������

|      ����      |        ����         |           ˵��           |
| :------------: | :-----------------: | :----------------------: |
| MenuOption_Member | U8G2E_MenuOption* | ָ��˵�ѡ��ṹ���ָ�� |

#### �������
����

#### ��������
- ����ٷֱ����͵Ĳ˵�ѡ��
- �ṩ�����������ڵ����ٷֱ�ֵ
- ֧��ͨ����������/���ٰٷֱ�ֵ
- ֧���л������ľ��ȣ�С����λ����
- �ṩ����Ч�����Ӿ�����

#### NOTE
- �ú���������ֱ���û��˳�
- �������黯��ͻ����ǰ��������

---

###  **U8G2E_NUM_ACTION**

```c
void U8G2E_NUM_ACTION(U8G2E_MenuOption *MenuOption_Member)
```

#### �������

|      ����      |        ����         |           ˵��           |
| :------------: | :-----------------: | :----------------------: |
| MenuOption_Member | U8G2E_MenuOption* | ָ��˵�ѡ��ṹ���ָ�� |

#### �������
����

#### ��������
- �����������͵Ĳ˵�ѡ��
- �ṩ�����������ڵ�����ֵ
- �Զ�ʶ�����ֵ�������С������
- ֧��ͨ����������/������ֵ
- ֧���л������ľ��ȣ�λ����
- �ṩ����Ч�����Ӿ�����

#### NOTE
- �ú���������ֱ���û��˳�
- �������黯��ͻ����ǰ��������
- ���ڷǳ����ǳ�С�����֣����Զ��л���ʾ��ʽ

---

### **U8G2E_StrHight**

```c
uint8_t U8G2E_StrHight(const char *str, uint8_t max_width, uint8_t x)
```

#### ����˵��
| ����      | ����         | ˵��                  |
|-----------|--------------|-----------------------|
| str       | const char*  | Ҫ������ַ���       |
| max_width | uint8_t      | ����ȣ����أ�     |
| x         | uint8_t      | ��ʼX����            |

#### ����ֵ
- �����ַ�����ָ������µ���Ⱦ�߶ȣ����أ�

#### ��������
- �����ַ������Զ����к���ܸ߶�
- ֧��ʶ��`\n`���з�
- �Զ��������׿ո�

#### NOTE
- ���ڵ���ǰ��������

---

### **U8G2E_SaveBuffer**

```c
void U8G2E_SaveBuffer(void)
```

#### ��������
- ���浱ǰ��ʾ���������ݵ����ݻ�����
- ����ʵ�ֽ���ָ������Ч��

#### NOTE
- ���ݻ�������СĬ��Ϊ`8 * 64 * 128`�ֽڣ��ɸ����Լ�����ʾ�����е���

---

### **U8G2E_CoverBuffer**

```c
void U8G2E_CoverBuffer(void)
```

#### ��������
- �ӱ��ݻ������ָ���ʾ����
- �����`U8G2E_SaveBuffer`ʹ��

#### NOTE
- �ָ�ǰ��ȷ������������Ч

---

### **U8G2E_SignKeyFun**

```c
void U8G2E_SignKeyFun(int Put_in_fun(void))
```

#### ����˵��
| ����        | ����            | ˵��                  |
|-------------|-----------------|-----------------------|
| Put_in_fun  | int (*)(void)   | ����ɨ�躯��ָ��      |

#### ��������
- ע���ⲿ����ɨ�躯��
- ��������`Key_mapping`ö��ֵ

#### NOTE
- �����ڵ��ò˵�ϵͳǰע����ʹ�ð���
- ���Ը��ݴ��ص��������ö������ӳ�书��

---

### **U8G2E_CountDigits**

```c
void U8G2E_CountDigits(double number, int *integer_digits, int *decimal_digits)
```

#### ����˵��
| ����            | ����     | ˵��                  |
|-----------------|----------|-----------------------|
| number          | double   | Ҫ��������ֵ          |
| integer_digits  | int*     | �������λ��ָ��      |
| decimal_digits  | int*     | ���С��λ��ָ��      |

#### ��������
- ����ʶ����ֵ������/С��λ��
- �Զ�����ĩβ��Ч��0

---

### **U8G2E_NUMDisplay**

```c
void U8G2E_NUMDisplay(int num, int x, int y, float change[], int W, int H)
```

#### ����˵��
| ����    | ����    | ˵��                  |
|---------|---------|-----------------------|
| num     | int     | Ҫ��ʾ�����֣�0-9��   |
| x       | int     | ��ʼX����            |
| y       | int     | ��ʼY����            |
| change  | float[] | �����������飨����8�� |
| W       | int     | ���ֿ��             |
| H       | int     | ���ָ߶�             |

#### ��������
- ʵ���߶�����ܵ�˿����ʾЧ��
- ֧�ֶ�̬��չ/��������
- �ڲ�ʹ��`Slow`����ģʽ

#### NOTE
- ��Ҫ��������

#### ʹ����

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
    u8g2.clearBuffer(); // ��ջ�����

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

    u8g2.sendBuffer(); // ���ͻ���������

    delay(15);
    Refresh_Time++;
  } while (Refresh_Time < 15); // ǿ��ˢ��15�Σ�������ֿ�������
  Refresh_Time = 0;
}

```

