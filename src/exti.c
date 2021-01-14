#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "beep.h"
#include "ZLG7289_STM32.h"
char key_val;

void EXTIX_Init(void)
{

    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //使能复用功能时钟

    //GPIOE.14	  中断线以及中断初始化配置  下降沿触发	//ZLG7289的INT引脚
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource14);
    EXTI_InitStructure.EXTI_Line = EXTI_Line14; 
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure); //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;             //使能按键KEY0所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;        //子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);                              //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}


//外部中断4服务程序
void EXTI15_10_IRQHandler(void)
{
   // delay_ms(10);  //消抖
    key_val = ZLG7289_Key();
    EXTI_ClearITPendingBit(EXTI_Line14); //清除LINE4上的中断标志位
}
