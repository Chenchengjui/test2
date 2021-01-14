#ifndef __ZLG7289_STM32_H
#define __ZLG7289_STM32_H
#include"stm32f10x.h"

void ZLG7289_Show(int result);
void ZLG7289_SPI_Write(char dat);
void ZLG7289_cmd(char cmd);
void ZLG7289_cmd_dat(char cmd,char dat);
void ZLG7289_Download(unsigned char mod, char x, int dp, char dat);
char ZLG7289_Key(void);
void ZLG7289_Init(void);

#define ZLG7289CS(x)        GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_11)|(x ? GPIO_Pin_11 : 0)
#define ZLG7289CLK(x)       GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_10)|(x ? GPIO_Pin_10 : 0)
#define ZLG7289WriteDAT(x)  GPIOE->ODR=(GPIOE->ODR&~GPIO_Pin_15)|(x ? GPIO_Pin_15 : 0)
#define ZLG7289ReadKey      GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14)
#define ZLG7289ReadDAT		GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_15)

#define ZLG7289_Reset() ZLG7289_cmd(0xA4)/*复位*/
#define ZLG7289_Test() ZLG7289_cmd(0xBF)/*测试*/
#define ZLG7289_SHL() ZLG7289_cmd(0xA0)/*左移*/
#define ZLG7289_SHR() ZLG7289_cmd(0xA1)/*右移*/
#define ZLG7289_ROL() ZLG7289_cmd(0xA2)/*循环左移*/
#define ZLG7289_ROR() ZLG7289_cmd(0xA3)/*循环右移*/

#define ZLG7289_Flash(x) ZLG7289_cmd_dat(0x88,(x))/*闪烁*/
#define ZLG7289_Hide(x) ZLG7289_cmd_dat(0x98,(x))/*消隐*/
#define ZLG7289_SegOn(seg) ZLG7289_cmd_dat(0xE0,(seg))/*段点亮*/

#endif
