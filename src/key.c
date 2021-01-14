#include "stm32f10x.h"
#include "key.h"
#include "sys.h"
#include "delay.h"

//������ʼ������
void KEY_Init(void) //IO��ʼ��
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); //ʹ��GPIOD��ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;     //KEY0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2����
//4��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1; //�������ɿ���־
    if (mode)
        key_up = 1; //֧������
    if (key_up && (KEY1 == 0 || KEY2 == 0))
    {
        delay_ms(10); //ȥ����
        key_up = 0;
        if (KEY1 == 0)
            return KEY1_PRES;
        else if (KEY2 == 0)
            return KEY2_PRES;
    }

    else if (KEY1 == 1 && KEY2 == 1)
        key_up = 1;
    return 0; // �ް�������
}
