
/*
 * Timer_Key_LED.c
 * 硬件描述： G2553开发板上P1.3接了一个按键，P1.0和P1.6接了LED
 * 功能描述： 两个LED永远1亮1灭，每次按P1.3后，两LED交换亮灭状态。
 * 备    注： 使用WDT定时节拍实现按键的非阻塞判断
 *  Created on: 2013-4-8
 *  Author: Administrator
 */
#include "MSP430G2553.h"

//-----在main函数前提前申明函数-----
void P1_IODect()	;
void P13_Onclick();
void GPIO_init();
void WDT_init();

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;//关狗
	GPIO_init();
	WDT_init();
        _BIS_SR(LPM3_bits + GIE);//使能总中断
	//_enable_interrupts();	//开总中断
	//_bis_SR_register(LPM3_bits);//LPM3休眠
}

/******************************************************************************************************
 * 名       称：GPIO_Init()
 * 功       能：设定按键和LED控制IO的方向，启用按键IO的上拉电阻
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
void GPIO_init()
{
	//-----设定P1.0和P1.6的输出初始值-----
	P1DIR |= BIT0+BIT6;//设定P1.0和P1.6为输出
	P1OUT |= BIT0;	//设定P1.0初值
	P1OUT &= ~BIT6;	//设定P1.6初值
	//-----配合机械按键，启用内部上拉电阻-----
	P1REN |= BIT3;	//启用P1.3内部上下拉电阻
	P1OUT |= BIT3;  //将电阻设置为上拉
	//-----不再使用P1.3中断功能-----
//	P1DIR &= ~BIT3;                    // P1.3设为输入(可省略)
//	P1IES |= BIT3;                     // P1.3设为下降沿中断
//	P1IE  |= BIT3 ;                    // 允许P1.3中断
}

/******************************************************************************************************
 * 名       称：WDT_init()
 * 功       能：设定WDT定时中断为16ms，开启WDT定时中断使能
 * 入口参数：无
 * 出口参数：无
 * 说       明：WDT定时中断的时钟源选择ACLK，可以用LPM3休眠。
 * 范       例：无
 ******************************************************************************************************/
void WDT_init()
{
	//-----设定WDT为16ms中断-----
	WDTCTL=WDT_ADLY_16;
	//-----WDT中断使能-----
        IE1|=WDTIE;
}

/******************************************************************************************************
 * 名       称：WDT_ISR()
 * 功       能：响应WDT定时中断服务
 * 入口参数：无
 * 出口参数：无
 * 说       明：WDT定时中断独占中断向量，所以无需进一步判断中断事件，也无需人工清除标志位。
 *                  所以，在WDT定时中断服务子函数中，直接调用WDT事件处理函数就可以了。
 * 范       例：无
 ******************************************************************************************************/
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	//-----启用Port1事件检测函数-----
	P1_IODect();		//检测通过，则会调用事件处理函数
}

/******************************************************************************************************
 * 名       称：P1_IODect()
 * 功       能：判断是否有键被按下，哪个键被按下，并调用相应IO的中断事件处理函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：必须用最近两次扫描的结果，才知道按键是否被按下
 * 范       例：无
 ******************************************************************************************************/
void P1_IODect()
{
	static unsigned char KEY_Now=0;	//变量值出函数时需保留
	unsigned char KEY_Past=0;
	KEY_Past=KEY_Now;
	//-----查询IO的输入寄存器-----
	if(P1IN&BIT3) 	KEY_Now=1;
	else 	        KEY_Now=0;
	//-----前一次高电平、后一次低电平，说明按键按下-----
	if((KEY_Past==1)&&(KEY_Now==0))
	P13_Onclick();
}
/******************************************************************************************************
 * 名       称：P13_Onclick()
 * 功       能：P1.3的中断事件处理函数，即当P1.3键被按下后，下一步干什么
 * 入口参数：无
 * 出口参数：无
 * 说       明：使用事件处理函数的形式，可以增强代码的移植性和可读性
 * 范       例：无
 ******************************************************************************************************/
void P13_Onclick()					//P1.3的事件处理函数
{
	//----翻转IO电平-----
	P1OUT ^= BIT0;
	P1OUT ^= BIT6;
}
