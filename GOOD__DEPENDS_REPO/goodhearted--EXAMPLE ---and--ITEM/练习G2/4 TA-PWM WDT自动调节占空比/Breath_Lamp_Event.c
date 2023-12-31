/*
 *  呼吸灯事件处理程序库
*/
#include "msp430g2553.h"    /*单片机寄存器头文件*/
#include "Breath_Lamp_Event.h"
//#include "Breath_Lamp_Global.h"	/*系统全局变量*/
#include "TA_PWM.h"
/******************************************************************************************************
 * 名       称：WDT_Ontime()
 * 功       能：WDT定时中断事件处理函数，即当WDT定时中断发生后后，下一步干什么
 * 入口参数：无
 * 出口参数：无
 * 说       明：使用事件处理函数的形式，可以增强代码的移植性和可读性
 * 范       例：无
 ******************************************************************************************************/
void WDT_Ontime()
{
	static int Bright_Delta=0;
	TA0_PWM_SetPermill(1,Bright);	//更新PWM占空比
        //TA1_PWM_SetPermill(2,Bright);
        //TA1_PWM_SetPermill(1,Bright);
	if(Bright>=400)	//占空比最大40%，更亮的区间视觉变化不明显
		Bright_Delta=-3;
	if(Bright<=10)	//变亮
		Bright_Delta=3;
	Bright=Bright+Bright_Delta;
	_NOP();	//用于插入断点调试程序用
}
