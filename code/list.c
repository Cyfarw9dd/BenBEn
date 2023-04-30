#include "list.h"

// 菜单的主体结构体，结构体数组
typedef struct 
{
    char ListIndex;             // 当前菜单索引
    unsigned char *ListName;    // 菜单名称
    void (*list_action)(float *param, char *ListName);
    float *param;
}List;


//主菜单
List MyList[] = 
{
    {1, "Image Mode", NULL, NULL},              // 图像模式
    {11, "Bin Image", NULL, NULL},           // 显示二值化图像
    {12, "Gray Imange", NULL, NULL},         // 显示灰度图像
    {2, "Tuning Mode", NULL, NULL},             // 调参模式
    {21, "Tuning", NULL, NULL},
};

List *current_list_item = MyList;


KeySatateEnum Key1;
KeySatateEnum Key2;
KeySatateEnum Key3;
KeySatateEnum Key4;

void KeyParams_Init(void)
{
    Key1 = nopress;
    Key2 = nopress;
    Key3 = nopress;
    Key4 = nopress;
}
#if 0
unsigned char Key1 = 0;
unsigned char Key2 = 0;
unsigned char Key3 = 0;
unsigned char Key4 = 0;
#endif


// 一般来说同一时间只会按下一个按键，所以多个按键共用同一个按键标志位也是可以的
// 待测试的按键扫描函数
void KeyScan(void)
{
    static unsigned char KeyPressNum = 0;
    static unsigned short KeyPressTime = 0;
    static unsigned char SomeKeyPress_Flag = 0; // 0松开 1按下 2消抖 3长按
    #define AlwaysPressTime 1200
    #define debouncing 5

    if(SomeKeyPress_Flag == 0 && (!gpio_get_level(KEY1) || !gpio_get_level(KEY2) || !gpio_get_level(KEY3) || !gpio_get_level(KEY4)))
    {
        SomeKeyPress_Flag = 1;
    }
    if(SomeKeyPress_Flag > 0)
    {
        KeyPressTime++;
        // 5ms消抖
        if(SomeKeyPress_Flag == 1 && KeyPressTime >= debouncing)
        {
            SomeKeyPress_Flag = 2;
            if(!gpio_get_level(KEY1))
            {
                KeyPressNum = 1;
            }
            if(!gpio_get_level(KEY2))
            {
                KeyPressNum = 2;
            }
            if(!gpio_get_level(KEY3))
            {
                KeyPressNum = 3;
            }
            if(!gpio_get_level(KEY4))
            {
                KeyPressNum = 4;
            }
        }
        // 短按
        if((gpio_get_level(KEY1) && gpio_get_level(KEY2) && gpio_get_level(KEY3) && gpio_get_level(KEY4)) && KeyPressTime < AlwaysPressTime && SomeKeyPress_Flag == 2)
        {
            SomeKeyPress_Flag = 0;
            if(KeyPressNum == 1)
            {
                Key1 = onepress;
            }
            if(KeyPressNum == 2)
            {
                Key2 = onepress;
            }
            if(KeyPressNum == 3)
            {
                Key3 = onepress;
            }
            if(KeyPressNum == 4)
            {
                Key4 = onepress;
            }  
        }
        // 长按
        if(KeyPressTime >= AlwaysPressTime && SomeKeyPress_Flag == 2)
        {
            if(KeyPressNum == 1)
            {
                Key1 = holdpress;
            }
            if(KeyPressNum == 2)
            {
                Key2 = holdpress;
            }
            if(KeyPressNum == 3)
            {
                Key3 = holdpress;
            }
            if(KeyPressNum == 4)
            {
                Key4 = holdpress;
            }
            if(gpio_get_level(KEY1) && gpio_get_level(KEY2) && gpio_get_level(KEY3) && gpio_get_level(KEY4))
            {
                SomeKeyPress_Flag = 0;
                KeyPressTime = 0;
                Key1 = nopress;
                Key2 = nopress;
                Key3 = nopress;
                Key4 = nopress;
            }
        }
    }
}


bool have_sub_list(int ListIndex)     // 查看是否存在子菜单
{
    for (int i = 0; i < sizeof(List) / sizeof(MyList); i++)
    {
        if (MyList[i].ListIndex / 10 == ListIndex)
        {
            return true;
        }
    }
    return false;
}

int show_sub_list(int parent_index, int highlight_col)      // 显示子菜单以及当前高亮菜单
{
    // tft180_clear();
    int item_index = 0;
    for (int i = 0; i < sizeof(List) / sizeof(MyList[0]); i++)
    {
        if (MyList[i].ListIndex / 10 == parent_index)
        {
            if (item_index == highlight_col)
            {
                current_list_item = &MyList[i];
                tft180_set_color(RGB565_RED, RGB565_WHITE);
            }
            else{
                tft180_set_color(RGB565_GREEN, RGB565_WHITE);
            }
            tft180_show_string(1, 30 * item_index, MyList[i].ListName);
            item_index++;
        }
    }
    return item_index;
}


void List_Switch(void)
{
    int parent_list_index = 0;
    int highlight_col = 0;
    int list_item_count = show_sub_list(parent_list_index, highlight_col);
    #if 0
    while (1)
    {   
        if (KeyScan){    // 读取按键函数
    #endif
            if (Key1 == onepress && highlight_col> 0)    // 按下按键1减少当前行数
            {
                highlight_col--;
            }
            else if (Key2 == onepress && highlight_col < list_item_count - 1)    // 按下按键2增加
            {
                highlight_col++;
            }
            else if (Key3 == onepress)
            {
                if (have_sub_list(current_list_item->ListIndex))
                {
                    highlight_col = 0;
                    parent_list_index = current_list_item->ListIndex;
                }
                else if (strcmp(current_list_item->ListName, "Back to Main") == 0)   // 如果当前菜单为Back to Main，则返回主菜单
                {
                    highlight_col = 0;
                    parent_list_index = 0;
                }
                else if (current_list_item->list_action)
                {
                    current_list_item->list_action(current_list_item->param, current_list_item->ListName);
                }
            }
            list_item_count = show_sub_list(parent_list_index, highlight_col);
        }
#if 0
    }
}
#endif