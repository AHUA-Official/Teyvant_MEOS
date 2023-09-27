

#include "MSP430G2553.h"

//-----在main()函数前提前申明子函数-----
void P1_IODect();//P1口的外部中断事件检测函数
void P13_Onclick();//P1.3按键的中断事件处理函数
void GPIO_Init();//GPIO初始化函数


void main(void) 
{
	WDTCTL = WDTPW + WDTHOLD;//关闭看门狗定时器
	GPIO_Init();//初始化GPIO
	_BIS_SR(CPUOFF+GIE);//等同_EINT，使能总中断

	__delay_cycles(1000000);//与CPU时钟相关的长延时
//	__delay_cycles(100);//与CPU时钟相关的长延时
        while(1)
        {
	  P1OUT ^= BIT0+BIT6;//LED亮灭状态改变
        }
//LED亮灭状态改变

}
/******************************************************************************************************
 * 名       称：GPIO_Init()
 * 功       能：设定按键和LED控制IO的方向，启用按键IO的上拉电阻
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
void GPIO_Init()
{
	//-----设定P1.0和P1.6的输出初始值-----
	P1DIR |= BIT0+BIT6;//设定P1.0为输出
	P1OUT |= BIT0+BIT6;//设定P1.0初值
	//-----配合机械按键，启用内部上拉电阻-----
	P1REN |= BIT3;//启用P1.3内部上下拉电阻
	P1OUT |= BIT3;//将电阻设置为上拉
	//-----配置P1.3中断参数-----
	P1DIR &= ~BIT3;// P1.3设为输入(可省略)
	P1IES |= BIT3; // P1.3设为下降沿中断
	P1IE  |= BIT3; // 允许P1.3中断
        
}
/******************************************************************************************************
 * 名       称：PORT1_ISR()
 * 功       能：响应P1口的外部中断服务
 * 入口参数：无
 * 出口参数：无
 * 说       明：P1.0~P1.8共用了PORT1中断，所以在PORT1_ISR()中必须查询标志位P1IFG才能知道
 * 		具体是哪个IO引发了外部中断。P1IFG必须手动清除，否则将持续引发PORT1中断。
 * 范       例：无
 ******************************************************************************************************/
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	//-----启用Port1事件检测函数-----
	P1_IODect();//检测通过，则会调用事件处理函数
   	P1IFG=0;//退出中断前必须手动清除IO口中断标志
}

/******************************************************************************************************
 * 名       称：P1_IODect()
 * 功       能：判断具体引发中断的IO，并调用相应IO的中断事件处理函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：该函数兼容所有8个IO的检测，请根据实际输入IO激活“检测代码”。
 * 					 本例中，仅有P1.3被用作输入IO，所以其他7个IO的“检测代码”没有被“激活”。
 * 范       例：无
 ******************************************************************************************************/
void P1_IODect()
{
	unsigned int Push_Key=0;
	//-----排除输出IO的干扰后，锁定唯一被触发的中断标志位-----
	Push_Key=P1IFG&(~P1DIR);//??????
	//-----延时一段时间，避开机械抖动区域-----
	__delay_cycles(10000);	//消抖延时
	//----判断按键状态是否与延时前一致-----
	if((P1IN&Push_Key)==0)//如果该次按键确实有效
        {
	 //----判断具体哪个IO被按下，调用该IO的事件处理函数-----
	 switch(Push_Key)
         {
//	 case BIT0:	P10_Onclick();		break;
//	 case BIT1: 	P11_Onclick();		break;
//	 case BIT2: 	P12_Onclick();		break;
	 case BIT3: 	P13_Onclick();		break;
//	 case BIT4: 	P14_Onclick();		break;
//	 case BIT5: 	P15_Onclick();		break;
//	 case BIT6: 	P16_Onclick();		break;
//	 case BIT7: 	P17_Onclick();		break;
	 default:				break;		//任何情况下均加上default
	 }
     }
}
/******************************************************************************************************
 * 名    称：P13_Onclick()
 * 功    能：P1.3的中断事件处理函数，即当P1.3键被按下后，下一步干什么
 * 入口参数：无
 * 出口参数：无
 * 说    明：使用事件处理函数的形式，可以增强代码的移植性和可读性
 * 范    例：无
 ******************************************************************************************************/
void P13_Onclick()
{
	//-----Freq仅在P13_Onclick()中使用，但是又需要退出函数时不被清除-----
	static unsigned int Freq=0;// 静态全局变量的典型应用场合
	//-----变量从0~3循环移位-----
	Freq++;                           
	if (Freq>3) Freq=0;
	//-----根据Freq的值，改变DCO设定频率-----
	switch(Freq)
        {
	case 0:	DCOCTL = CALDCO_1MHZ; BCSCTL1 = CALBC1_1MHZ; break;
	case 1: DCOCTL = CALDCO_8MHZ; BCSCTL1 = CALBC1_8MHZ; break;
	case 2:	DCOCTL = CALDCO_12MHZ;BCSCTL1 = CALBC1_12MHZ;break;
	case 3:	DCOCTL = CALDCO_16MHZ;BCSCTL1 = CALBC1_16MHZ;break;
	default: 				             break;
	}
}

