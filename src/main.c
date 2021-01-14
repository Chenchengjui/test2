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

int main(void)
{

    delay_init();                                   //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级s
    ZLG7289_Init();
    EXTIX_Init();
    LED_Init();            //初始化与LED连接的硬件接口
    TIM3_Int_Init(99, 71); //定时器中断的时间为
    ZLG7289_Hide(0xFF);
    LED0 = 1;
    while (1)
    {
        LED0 = !LED0;
        if (key_val != 0xFF)
        {
            switch (key_val)
            {
            case 0x0F: 

                break;

            case 0x0E:
                break;

            case 0x0D:

            default:
                break;
            }
        }
    }
}
