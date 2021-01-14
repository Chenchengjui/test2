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


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //ʹ�ܸ��ù���ʱ��

    //GPIOE.14	  �ж����Լ��жϳ�ʼ������  �½��ش���	//ZLG7289��INT����
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource14);
    EXTI_InitStructure.EXTI_Line = EXTI_Line14; 
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure); //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;             //ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;        //�����ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);                              //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}


//�ⲿ�ж�4�������
void EXTI15_10_IRQHandler(void)
{
   // delay_ms(10);  //����
    key_val = ZLG7289_Key();
    EXTI_ClearITPendingBit(EXTI_Line14); //���LINE4�ϵ��жϱ�־λ
}
