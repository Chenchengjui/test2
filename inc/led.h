#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED0 PEout(0)
#define LED1 PEout(1)	
#define LED2 PEout(2)	
#define LED3 PEout(3)	
#define LED4 PEout(4)	
#define LED5 PEout(5)	
#define LED6 PEout(6)	
#define LED7 PCout(13)


void LED_Init(void);//��ʼ��
void LED_Clear(void);
		 				    
#endif
