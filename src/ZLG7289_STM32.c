#include "ZLG7289_STM32.h"
static void ZLG7289_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;                                         //始使化ZLG7289用的四个IO�?口，PB11，PB10，PE15，PE14
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE); //Enable the clock of PE and PB
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);                         //Enable the clock of AFIO
                                                                                 //remap ************************************/
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);                     //重映射关�?下载�?
    //CS-PB1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //CLK-PB10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //DIO-PE5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //Firstly, set the pin as output
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    //INT-PE14
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

static void ZLG7289_DATA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
static void ZLG7289_DATA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
static void ZLG7289_Delay_1us(uint16_t x) //1us
{
    x += x;
    while (x--)
        ;
}

/*ZLG7289�?*/
void ZLG7289_SPI_Write(char dat)
{
    unsigned char i;
    ZLG7289_DATA_OUT();
    ZLG7289_Delay_1us(200);
    for (i = 0; i < 8; i++)
    {
        if ((dat & 0x80) != 0)

            ZLG7289WriteDAT(1);
        else
            ZLG7289WriteDAT(0);

        dat = dat << 1;
        ZLG7289_Delay_1us(100);
        ZLG7289CLK(1);
        ZLG7289_Delay_1us(100);
        ZLG7289CLK(0);
        ZLG7289_Delay_1us(100);
    }
    ZLG7289_Delay_1us(100);
}

/*
函数：ZLG7289_SPI_Read()
功能：从SPI 总线读取1 �?字节的数�?
返回�?
读取到的数据
*/
char ZLG7289_SPI_Read() //reentrant //   去除�?重入属性，否则程序不能正常运�??
{
    char dat;
    unsigned char t = 8;
    ZLG7289_DATA_IN();
    do
    {
        ZLG7289CLK(1);
        ZLG7289_Delay_1us(100);
        dat <<= 1;
        if (ZLG7289ReadDAT)
            dat++;
        ZLG7289CLK(0);
        ZLG7289_Delay_1us(100);
    } while (--t != 0);
    ZLG7289_DATA_OUT();
    return dat;
}
/*ZLG7289B�?指令*/
void ZLG7289_cmd(char cmd)
{
    ZLG7289_DATA_OUT();
    ZLG7289CS(0);
    ZLG7289_Delay_1us(200);
    ZLG7289_SPI_Write(cmd);
    ZLG7289CS(1);
    ZLG7289_Delay_1us(100);
}

/*ZLG7289B带数�?指令*/
void ZLG7289_cmd_dat(char cmd, char dat)
{
    ZLG7289_DATA_OUT();
    ZLG7289CS(0);
    ZLG7289_Delay_1us(200);
    ZLG7289_SPI_Write(cmd);
    ZLG7289_Delay_1us(100);
    ZLG7289_SPI_Write(dat);
    ZLG7289CS(1);
    ZLG7289_Delay_1us(100);
}

/*ZLG7289B下载数据*/
void ZLG7289_Download(unsigned char mod, char x, int dp, char dat)
{
    char ModDat[3] = {0x80, 0xC8, 0x90};
    char d1;
    char d2;
    if (mod > 2)
        mod = 2;
    d1 = ModDat[mod];
    x &= 0x07;
    d1 |= x;
    d2 = dat & 0x7F;
    if (dp)
        d2 |= 0x80;
    ZLG7289_cmd_dat(d1, d2);
}
/*
功能：执行ZLG7289 �?盘命�?
返回�?
返回读到的按�?值：0�?63
如果返回0xFF 则表示没有键按下
*/
char ZLG7289_Key(void) //reentrant  //去除�?重入属性，否则程序不能正常运�??
{
    char key;
    ZLG7289CS(0);             //片选低有效
    ZLG7289_Delay_1us(200);   //延时
    ZLG7289_SPI_Write(0x15);  //写入读指�?
    ZLG7289_Delay_1us(100);   //延时
    key = ZLG7289_SPI_Read(); //读取�?盘指�?
    ZLG7289CS(1);             //关闭片�?
    ZLG7289_Delay_1us(100);   //延时
    return key;               //返回读出字节�?0x00~0x3F�?
}
void ZLG7289_Init(void)
{

    ZLG7289_GPIO_Config();
    ZLG7289CS(1);       //片选低电平有效
    ZLG7289CLK(0);      //时钟、上升沿有效
    ZLG7289WriteDAT(1); //IO口置�?
}

void ZLG7289_Show(int result) //显示整数
{
    char num[8];
    int i;
    num[7] = result % 10;
    num[6] = (result % 100) / 10;
    num[5] = (result % 1000) / 100;
    num[4] = (result % 10000) / 1000;
    num[3] = (result % 100000) / 10000;
    num[2] = (result % 1000000) / 100000;
    num[1] = (result % 10000000) / 1000000;
    num[0] = result / 10000000;
    if (num[0] != 0)
    {
        for (i = 0; i < 8; i++)
            ZLG7289_Download(1, i, 0, num[i]);
    }
    else if (num[1] != 0)
    {
        for (i = 1; i < 8; i++)
            ZLG7289_Download(1, i, 0, num[i]);
    }
    else

        if (num[2] != 0)
    {
        for (i = 2; i < 8; i++)
            ZLG7289_Download(1, i, 0, num[i]);
    }
    else

        if (num[3] != 0)
    {
        for (i = 3; i < 8; i++)
            ZLG7289_Download(1, i, 0, num[i]);
    }
    else

        if (num[4] != 0)
    {
        for (i = 4; i < 8; i++)
            ZLG7289_Download(1, i, 0, num[i]);
    }
    else

        if (num[5] != 0)
    {
        for (i = 5; i < 8; i++)
            ZLG7289_Download(1, i, 0, num[i]);
    }
    else

        if (num[6] != 0)
    {
        for (i = 6; i < 8; i++)
            ZLG7289_Download(1, i, 0, num[i]);
    }
    else

        if (num[7] != 0)
    {
        for (i = 7; i < 8; i++)
            ZLG7289_Download(1, i, 0, num[i]);
    }
    else

        ZLG7289_Download(1, 7, 0, 0);
}
