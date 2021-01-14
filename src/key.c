#include "stm32f10x.h"
#include "key.h"
#include "sys.h"
#include "delay.h"

//按键初始化函数
void KEY_Init(void) //IO初始化
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); //使能GPIOD的时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;     //KEY0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成下拉输入
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下
//4，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1; //按键按松开标志
    if (mode)
        key_up = 1; //支持连按
    if (key_up && (KEY1 == 0 || KEY2 == 0))
    {
        delay_ms(10); //去抖动
        key_up = 0;
        if (KEY1 == 0)
            return KEY1_PRES;
        else if (KEY2 == 0)
            return KEY2_PRES;
    }

    else if (KEY1 == 1 && KEY2 == 1)
        key_up = 1;
    return 0; // 无按键按下
}
