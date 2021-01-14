#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "beep.h"
#include "timer.h"
#include "exti.h"
#include "ZLG7289_STM32.h"
extern char key_val;        // 表示ZLG7289的键值
extern u32 timer3_count_ms; //表示定时器3溢出的次数 每次溢出过去0.1s
extern u16 timer3_count_s;  //
u8 state = 0;               //模式选择 0-清0等待计时状态 1-暂停 2-正常定时 在开始的时候强制复位为0
u8 led_pos = 0;
int main(void)
{

    delay_init();                                   //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级s
    ZLG7289_Init();
    EXTIX_Init();
    TIM3_Int_Init(99, 71); //定时器中断的时间为
    LED_Init();            //初始化与LED连接的硬件接口
    ZLG7289_Hide(0xFF);
    LED0 = 1;
    while (1)
    {
        LED0 = !LED0;
        if (key_val != 0xFF)
        {
            switch (key_val)
            {
            case 0x0F: //开始按键
                LED1 = !LED1;
                TIM_Cmd(TIM3, ENABLE); //使能TIMx
                state = 2; //进入计时状态
                key_val = 0xFF;
                break;

            case 0x0E:
                LED2 = !LED2;
                TIM_Cmd(TIM3, DISABLE); //使能TIMx
                state = 1; //进入暂停状态
                key_val = 0xFF;
                break;

            case 0x0D:
                TIM_Cmd(TIM3, DISABLE); //使能TIMx
                ZLG7289_Reset();
                LED2 = 0;
                LED1 = 0;
                timer3_count_ms = 0;
                timer3_count_s = 0;
                state = 0;
                key_val = 0xFF;
                break;

            default:
                break;
            }
        }

        if (state == 2) //表示在正常的计时状态
        {
            if (timer3_count_ms % 300 == 0)
                ZLG7289_Show(timer3_count_ms + timer3_count_s * 1000);
        }

    }
}
