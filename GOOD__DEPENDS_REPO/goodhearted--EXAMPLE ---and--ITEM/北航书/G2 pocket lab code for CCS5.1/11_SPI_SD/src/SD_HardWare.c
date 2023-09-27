/*
 * SD_HardWare.c
 *说明：此函数库文件为SD卡底层相关的读写和操作函数，也是与最底层G2硬件SPI唯一的衔接函数库。
 * 			若是需要在其他硬件微处理器上使用上层SD_SPI.c库函数，只需要更改此函数库中带有
 * 			“此处，调用底层G2硬件接口函数”注释的4处底层SPI函数，和SD_High_Speed()、
 * 			SD_Low_Speed()这两个函数即可完成移植。
 *  Created on: 2013-4-3
 *      Author: WangJingbing
 */
#include"MSP430G2553.h"
#include"USCI_SPI.h"
/****************************************************************************
* 名    称：SD_High_Speed()
* 功    能：SD卡在SPI读取模式下，使能SPI的时钟为高速传输模式
* 入口参数：无
* 出口参数：无
* 说    明：  该函数可以更改当前SPI工作的速度。 一般在初始化完SD卡后
* 					需要将SPI的速度提高，加快读写速度
* 使用范例：SD_High_Speed();
****************************************************************************/
void SD_High_Speed()
{
    UCA0CTL1 |= UCSWRST;
    UCA0BR0 = 2;                                // f_UCxCLK = 12MHz/2 = 6MHz
    UCA0BR1 = 0;
    UCA0MCTL = 0;
    UCA0CTL1 &= ~UCSWRST;
}
/****************************************************************************
* 名    称：SD_Low_Speed()
* 功    能：SD卡在SPI读取模式下，使能SPI的时钟为低速（300K左右）
* 入口参数：无
* 出口参数：无
* 说    明： 由于在初始化完SD卡时，需要的速度很低，此函数更改当前
* 					SPI工作的速度。使其在300K左右
* 使用范例：SD_Low_Speed();
****************************************************************************/
void SD_Low_Speed()
{
    UCA0CTL1 |= UCSWRST;
    UCA0BR0 =50;                          // f_UCxCLK = 12MHz/50 = 240KHz
    UCA0BR1 = 0;
    UCA0MCTL = 0;
    UCA0CTL1 &= ~UCSWRST;
}
/****************************************************************************
* 名    称：SD_CS_High()
* 功    能：SD卡在SPI读取模式下，控制使能CS管脚为高电平
* 入口参数：无
* 出口参数：无
* 说    明：  此处的CS管脚可以根据硬件的需要，任意指定管脚作CS均可。
* 使用范例：SD_CS_High();
****************************************************************************/
void SD_CS_High()
{
	//-----此处，调用底层G2硬件接口函数-----
	SPI_CS_High();
}
/****************************************************************************
* 名    称：SD_CS_Low()
* 功    能：SD卡在SPI读取模式下，控制使能CS管脚为低电平
* 入口参数：无
* 出口参数：无
* 说    明:：  此处的CS管脚可以根据硬件的需要，任意指定管脚作CS均可。
* 使用范例：SD_CS_Low();
****************************************************************************/
void SD_CS_Low()
{
	//-----此处，调用底层G2硬件接口函数-----
	SPI_CS_Low();
}
/****************************************************************************
* 名    称：SD_Write_Byte()
* 功    能：SPI模式下，向SD卡中写入一个字节数据
* 入口参数：value：当前要写入的数据
* 出口参数：无
* 说    明：使用该函数可以向SD卡中写入一个字节
* 使用范例：SD_Write_Byte();// 将value写入SD 卡中
****************************************************************************/
void SD_Write_Byte(unsigned char value)
{
	unsigned char temp=0;
	do{
		//-----此处，调用底层G2硬件接口函数-----
		temp=SPI_SendFrame(&value,1);
	}while(temp==0);
}
/****************************************************************************
* 名    称：SD_Write_Frame()
* 功    能：SPI模式下，向SD卡中写入size个字节数据
* 入口参数：pBuffer：当前要写入的数据头指针
* 					size：计划要写入的数据个数
* 出口参数：无
* 说    明：使用该函数可以向SD卡中写入size个字节
* 使用范例：无
****************************************************************************/
void SD_Write_Frame(unsigned char  *pBuffer,unsigned int size)
{
	unsigned char temp=0;
	do{
		//-----此处，调用底层G2硬件接口函数-----
		temp=SPI_SendFrame(pBuffer,size);
	}while(temp==0);
}
/****************************************************************************
* 名    称：SD_Write_Frame()
* 功    能：SPI模式下，从SD卡中读出size个字节数据
* 入口参数：pBuffer：存储的读SD数据的头指针
* 					size：计划要读取的数据个数
* 出口参数：无
* 说    明：使用该函数可以从SD卡中读出size个字节
* 使用范例：无
****************************************************************************/
void SD_Read_Frame(unsigned char  *pBuffer, unsigned int size)
{
	unsigned char temp=0;
	do{
		//-----此处，调用底层G2硬件接口函数-----
		temp=SPI_RecieveFrame(pBuffer,size);
	}while(temp==0);
}
/****************************************************************************
* 名    称：SD_Read_Byte()
* 功    能：SPI模式下，读取SD卡中的一个字节
* 入口参数：无
* 出口参数：value：当前读取出的数据
* 说    明：使用该函数可以读取SD卡中的一个字节
* 使用范例：tempt=SD_Read_Byte();// 读取一个字节，并赋给tempt变量
****************************************************************************/
unsigned char SD_Read_Byte()
{
	unsigned char value=0;
	unsigned char temp=0;
	do{
		//-----此处，调用底层G2硬件接口函数-----
		temp=SPI_RecieveFrame(&value,1);
	}while(temp==0);
	return value;
}





