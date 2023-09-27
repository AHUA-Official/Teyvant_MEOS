/****************************************************

*******************硬件连接**************************


             DS18B20的连接
        DS18B20 - (1)-----------------------DS18B20的三极管9013-C（3）
        DQ        (2)-----------------------P1.6  //DS18B20的控制信号脚
        DS18B20 + (3)-----------------------VCC 

           DS18B20的三极管9013的连接
        DS18B20的三极管9013-B(2)------------P2.1   //DS18B20的控制电源
        DS18B20的三极管9013-B(1)------------GND 

             nRF 2401的连接
        MISO -------------------------------P1.0
        MOSI -------------------------------P1.1
        SCK  -------------------------------P1.2
        CE   -------------------------------P1.3
        CSN  -------------------------------P1.4
        IRQ  -------------------------------P1.5
        nRF 2401 + -------------------------VCC
        nRF 2401 - -------------------------nRF 2401的三极管9013-C(3)

            nRF 2401的三极管9013的连接
        nRF 2401的三极管9013(2)-------------P2.0 
        nRF 2401的三极管9013(1)-------------GND

                 按键的连接
        睡眠sleep（LPM4）-------------------P2.3
        唤醒（LPM4_EXIT) -------------------P2.4 

******************************************************/
#include  <msp430g2553.h>
#define uint unsigned int 
#define uchar unsigned char

volatile uchar sleep=0;//睡眠标志位

#define NPN_9013   BIT0           //三极管作为开关控制发射电源
#define NPN_9013_H P2OUT |= BIT0  //三极管置高时，发射电源打开
#define NPN_9013_L P2OUT &= ~BIT0 //三极管置低时，发射电源关闭

#define DS18B20_NPN_9013   BIT1           //三极管作为开关控制DS18B20电源
#define DS18B20_NPN_9013_H P2OUT |= BIT1  //三极管置高时，DS18B20电源打开
#define DS18B20_NPN_9013_L P2OUT &= ~BIT1 //三极管置低时，DS18B20电源关闭


void Delay_us(uint us);



//以下是DS18B20显示宏定义

#define DS18B20      BIT6
#define DS18B20_H    P1OUT |= BIT6
#define DS18B20_L    P1OUT &= ~BIT6

//以下是DS18B20温度检测子程序
/********************************
函数名称: B20_init
功    能: 复位DS18B20
参    数: 无
返回值  : 无
********************************/
void B20_init(void)
{
 P1DIR |= DS18B20;		     //配置为输出			  
 DS18B20_L;			     //拉低
 Delay_us(600);			     //等待600微秒
 DS18B20_H;			     //释放总线
 Delay_us(60);			     //等待60微秒
 P1DIR &= ~DS18B20;		     //配置为输入 
 while((P1IN &(DS18B20)));	     //等待DS18B20拉低
 while(!(P1IN &(DS18B20)));          //等待DS18B20释放总线
}
/*******************************************
函数名称: B20_readB
功    能: 读取一个字节的数据
参    数: 无
返回值  : retd--返回的一个字节数据
********************************************/
uchar B20_readB(void)
{
 uchar i,retd=0;
 for(i=0;i<8;i++)	 	//位计数值
 {
  retd>>=1;			//右移，准备接受新的数据位
  P1DIR |=DS18B20;		//配置为输出
  DS18B20_L;		        //拉低，启动读数据位
  DS18B20_H;			//释放总线
  Delay_us(5);			//等待5微秒
  P1DIR &=~DS18B20;		//配置为输入，开始读取数据位
  if(P1IN&DS18B20)	        //该位是否为高
  {
   retd|=0x80;			//是就将此位置高
  }
  Delay_us(50);			//等待50微秒
 }
 return retd;			//将读到的一个字节返回
}
/*******************************************
函数名称: B20_writeB
功    能: 写入一个字节的数据
参    数: wrd--要写入的数据
返回值  : 无
********************************************/
void B20_writeB(uchar wrd)
{
 uchar i;
 for(i=0;i<8;i++)	  	//位计数值
 {
  P1DIR |=DS18B20;		//配置为输出
  DS18B20_L;		        //拉低，启动写数据位
  Delay_us(1);			//等待1微秒
  if(wrd&0x01)			//此位数据是否为高
  {
   DS18B20_H;		        //是高则将单总线拉高
  }
  else
  {
   DS18B20_L;		        //是低则将单总线拉低
  }
  Delay_us(50);		        //等待50微秒
  DS18B20_H;		        //释放总线
  wrd>>=1;			//右移，为写入新的数据位做准备
 }
  Delay_us(50);			//等待50微秒
}
/*******************************************
函数名称: Read_temp
功    能: 读取温度值
参    数: 无
返回值  : rettemp--返回的温度值
********************************************/
uint Read_temp(void)
{
 uchar templ,temph;
 uint temp;
 B20_init();	   //初始化，每次写命令都从初始化开始
 B20_writeB(0xcc); //跳过ROM
 B20_writeB(0x44); //启动温度转换
 B20_init();	   //初始化，每次写命令都从初始化开始
 B20_writeB(0xcc); //跳过ROM
 B20_writeB(0xbe); //读寄存器
 templ=B20_readB();//读温度低字节
 temph=B20_readB();//读温度高字节
 temp=templ+temph*256;//将温度整理成16位变量
 
  return temp;	   //返回16位变量

}
//以上是DS18B20温度检测子程序




#define TX_ADDR_WITDH 5//发送地址宽度设置为5个字节
#define RX_ADDR_WITDH 5//接收地址宽度设置为5个字节
#define TX_DATA_WITDH 8//发送数据位数
#define RX_DATA_WITDH 8//接收数据位数


//nRF24L01对应引脚

#define MISO   BIT0                //input
#define MISO_H P1OUT |= BIT0
#define MISO_L P1OUT &= ~BIT0

#define MOSI   BIT1
#define MOSI_H P1OUT |= BIT1
#define MOSI_L P1OUT &= ~BIT1

#define SCK    BIT2
#define SCK_H  P1OUT |= BIT2
#define SCK_L  P1OUT &= ~BIT2

#define CE     BIT3
#define CE_H   P1OUT |= BIT3
#define CE_L   P1OUT &= ~BIT3

#define CSN    BIT4
#define CSN_H  P1OUT |= BIT4
#define CSN_L  P1OUT &= ~BIT4

#define IRQ    BIT5          
#define IRQ_H  P1OUT |=BIT5 
#define IRQ_L  P1OUT &= ~BIT5

#define  LED   BIT6
#define  LED_H P1OUT |= BIT6;
#define  LED_L P1OUT &= ~BIT6;

#define R_REGISTER    0x00  // 读寄存器
#define W_REGISTER    0x20  // 写寄存器
#define R_RX_PLOAD    0x61  // 读RX FIFO有效数据，1-32字节，当读数据完成后，数据被清除，应用于接收模式
#define W_TX_PLOAD    0xA0  // 写TX FIFO有效数据，1-32字节，写操作从字节0开始，应用于发射模式
#define FLUSH_TX    0xE1  // 清除TX FIFO寄存器，应用于发射模式
#define FLUSH_RX    0xE2  // 清除RX FIFO寄存器，应用于接收模式
#define REUSE_TX_PL 0xE3  // 重新使用上一包有效数据，当CE为高过程中，数据包被不断的重新发射
#define NOP         0xFF  // 空操作，可以用来读状态寄存器

#define CONFIG      0x00  // 配置寄存器
#define EN_AA       0x01  // “自动应答”功能寄存
#define EN_RX_ADDR  0x02  // 接收通道使能寄存器
#define SETUP_AW    0x03  // 地址宽度设置寄存器
#define SETUP_RETR  0x04  // 自动重发设置寄存器
#define RF_CH       0x05  // 射频通道频率设置寄存器
#define RF_SETUP    0x06  // 射频设置寄存器
#define STATUS      0x07  // 状态寄存器
#define OBSERVE_TX  0x08  // 发送检测寄存器
#define CD          0x09  // 载波检测寄存器
#define RX_ADDR_P0  0x0A  // 数据通道0接收地址寄存器
#define RX_ADDR_P1  0x0B  // 数据通道1接收地址寄存器
#define RX_ADDR_P2  0x0C  // 数据通道2接收地址寄存器
#define RX_ADDR_P3  0x0D  // 数据通道3接收地址寄存器
#define RX_ADDR_P4  0x0E  // 数据通道4接收地址寄存器
#define RX_ADDR_P5  0x0F  // 数据通道5接收地址寄存器
#define TX_ADDR     0x10  // 发送地址寄存器
#define RX_PW_P0    0x11  // 数据通道0有效数据宽度设置寄存器
#define RX_PW_P1    0x12  // 数据通道1有效数据宽度设置寄存器
#define RX_PW_P2    0x13  // 数据通道2有效数据宽度设置寄存器
#define RX_PW_P3    0x14  // 数据通道3有效数据宽度设置寄存器
#define RX_PW_P4    0x15  // 数据通道4有效数据宽度设置寄存器
#define RX_PW_P5    0x16  // 数据通道5有效数据宽度设置寄存器
#define FIFO_STATUS 0x17  // FIFO状态寄存器

uchar sta;                // 状态变量
#define RX_DR  (sta & 0x40)  // 接收成功中断标志
#define TX_DS  (sta & 0x20)  // 发射成功中断标志
#define MAX_RT (sta & 0x10)  // 重发溢出中断标志

uchar  TX_Addr[]={0x34,0x43,0x10,0x10,0x01}; 

//uchar  TX_Buffer[]={3,2,1,0};//待发的数据
uchar RX_Buffer[RX_DATA_WITDH];

void port_init(void)               //端口初始化
{
  P1DIR |= CE+CSN+SCK+IRQ+MOSI+LED+DS18B20;
  P1OUT &= ~LED;
  P1DIR &= ~MISO;
  
  P2DIR |=NPN_9013+DS18B20_NPN_9013;
  NPN_9013_H;          //三极管置高时，发射电源打开
  DS18B20_NPN_9013_H;   //三极管置高时，DS18B20电源打开
  
  P2OUT |=BIT3+BIT4;   //P2.3-4带上拉电阻
  P2IE  |=BIT3+BIT4;   //P2.3-4中断使能
  P2IES |=BIT3+BIT4;   //P2.3-4中断下降沿触发中断
  P2REN |=BIT3+BIT4;   //P2.3-4上拉电阻使能
  P2IFG &=~(BIT3+BIT4);//P2.3-4中断标志位清零
  
}
void int_Clock(void)               //时钟初始化
{
  BCSCTL1 = CALBC1_8MHZ;           //DCO数字振荡器=1MHZ
  DCOCTL  = CALBC1_8MHZ;           //DCO数字振荡器=1MHZ 
}
void int_WDT(void)                 //看门狗设置
{
  WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog
}
void Delay_us(uint us)           // 延时us  前提是f=8MHZ
{    
  uint i;
  for(i=0;i<us;i++)
  __delay_cycles(8);      
}
void Delay_ms(uint ms)           // 延时ms  前提是f=8MHZ
{
  uint j;
  for(j=0;j<ms;j++)
  __delay_cycles(8000); 
}


//nRF24L01初始化
void nRF24L01_Init(void)
{
 CE_L;//待机模式Ⅰ
 CSN_H;
 SCK_L;
 IRQ_H;
}





//SPI时序函数
uchar SPI_RW(uchar byte)
{
 uchar i;
 for(i=0;i<8;i++)//一字节8位循环8次写入
 {
  if(byte&0x80)//如果数据最高位是1
   MOSI_H;//向NRF24L01写1
  else //否则写0
   MOSI_L;
  byte<<=1;//低一位移到最高位
  SCK_H;//SCK拉高，写入一位数据，同时读取一位数据
  if(P1IN & MISO)
   byte|=0x01;
  SCK_L;//SCK拉低
 }
 return byte;//返回读取一字节
}

//SPI写寄存器一字节函数
//reg:寄存器地址
//value:一字节（值）
uchar SPI_W_Reg(uchar reg,uchar value)
{
 uchar status;//返回状态
 CSN_L;//SPI片选
 status=SPI_RW(reg);//写入寄存器地址，同时读取状态
 SPI_RW(value);//写入一字节
 CSN_H;//
 return status;//返回状态
}

//SPI读一字节
uchar SPI_R_byte(uchar reg)
{
 uchar reg_value;
 CSN_L;//SPI片选
 SPI_RW(reg);//写入地址
 reg_value=SPI_RW(0);//读取寄存器的值
 CSN_H;
 return reg_value;//返回读取的值
}

//SPI读取RXFIFO寄存器数据
//reg:寄存器地址
//Dat_Buffer:用来存读取的数据
//DLen:数据长度
uchar SPI_R_DBuffer(uchar reg,uchar *Dat_Buffer,uchar Dlen)
{
 uchar status,i;
 CSN_L;//SPI片选
 status=SPI_RW(reg);//写入寄存器地址，同时状态
 for(i=0;i<Dlen;i++)
 {
  Dat_Buffer[i]=SPI_RW(0);//存储数据
 }
 CSN_H;
 return status;
}

//SPI向TXFIFO寄存器写入数据
//reg:写入寄存器地址
//TX_Dat_Buffer:存放需要发送的数据
//Dlen:数据长度
uchar SPI_W_DBuffer(uchar reg,uchar *TX_Dat_Buffer,uchar Dlen)
{
 uchar status,i;
 CSN_L;//SPI片选，启动时序
 status=SPI_RW(reg);
 for(i=0;i<Dlen;i++)
 {
  SPI_RW(TX_Dat_Buffer[i]);//发送数据
 }
 CSN_H;
 return status; 
}

//设置发送模式
void nRF24L01_Set_TX_Mode(uchar *TX_Data)
{
 CE_L;//待机（写寄存器之前一定要进入待机模式或掉电模式）
 SPI_W_DBuffer(W_REGISTER+TX_ADDR,TX_Addr,TX_ADDR_WITDH);//写寄存器指令+接收节点地址+地址宽度
 SPI_W_DBuffer(W_REGISTER+RX_ADDR_P0,TX_Addr,TX_ADDR_WITDH);//为了接收设备应答信号，接收道0地址与发送地址相同
 SPI_W_DBuffer(W_TX_PLOAD,TX_Data,TX_DATA_WITDH);//写有效数据地址+有效数据+有效数据宽度
 SPI_W_Reg(W_REGISTER+EN_AA,0x01);//接收通道0自动应答
 SPI_W_Reg(W_REGISTER+EN_RX_ADDR,0x01);//使能接收通道0
 SPI_W_Reg(W_REGISTER+SETUP_RETR,0x0a);//自动重发延时250US+86US，重发10次
 SPI_W_Reg(W_REGISTER+RF_CH,0);//2.4GHZ
 SPI_W_Reg(W_REGISTER+RF_SETUP,0x07);//1Mbps速率,发射功率:0DBM,低噪声放大器增益
 SPI_W_Reg(W_REGISTER+CONFIG,0x0e);//发送模式,上电,16位CRC校验,CRC使能
 CE_H;//启动发射
 Delay_ms(5);//CE高电平持续时间最少10US以上
}

//检测应答信号
uchar Check_Ack(void)
{
 sta=SPI_R_byte(R_REGISTER+STATUS);//读取寄存状态
 if(TX_DS||MAX_RT)//如果TX_DS或MAX_RT为1,则清除中断和清除TX_FIFO寄存器的值
 {
  SPI_W_Reg(W_REGISTER+STATUS,0xff);
  CSN_L;
  SPI_RW(FLUSH_TX);       //如果没有这一句只能发一次数据，大家要注意
  CSN_H;
  return 0;
 }
 else
  return 1;
}

void init_devices(void)
{
   port_init();
   int_Clock();
   int_WDT()  ;          //watchdog
}

void stop_IO(void) //进入休眠之前先配置IO口，除按键，开关以外，全部输出，置高
{
  P1DIR =0XFF;
  P1OUT =0XFF;
  P2DIR |= 0XE7;   //将按键设置为输入，以响应中断
  P2OUT |= 0XE6;
  NPN_9013_L;      //将三极管置低，即发射电源关闭
  DS18B20_NPN_9013_L;
}

void main(void)
{
   uint t;
   uchar i;
   uchar TX_Buffer[]={1,1,1,1};//待发的数据
   init_devices();
   Delay_ms(5);
   nRF24L01_Init();                  //NRF24L01初始化
   _EINT();                          //开总中断
   while(1)
 {
    t=Read_temp();                    //读取温度值
    t=t*6.25;
    TX_Buffer[3]=t/1000;              //
    TX_Buffer[2]=t%1000/100;
    TX_Buffer[1]=t%1000%100/10;
    TX_Buffer[0]=t%10;
   
//  for(i=0;i<TX_DATA_WITDH-1;i++)  //发送7次数据
  for(i=0;i<TX_DATA_WITDH-7;i++)    //发送一次数据  
   {
         nRF24L01_Set_TX_Mode(&TX_Buffer[i]);//发送数据
         while(Check_Ack());//等待发送完成
           
        //LED_L;            //检测数据是不是发射完成
        //Delay_ms(2);
        //LED_H;            //检测数据是不是发射完成，若完成LED闪烁一次
        //Delay_ms(2);
       
   }
 
  if(sleep)
   {
         stop_IO();        //进入休眠之前先配置IO口，除按键，开关以外，全部输出，置高
         LPM4;
         
         init_devices();   //从新配置，即初始化
         Delay_ms(5);
         nRF24L01_Init();  //NRF24L01初始化
         sleep=0;          //清睡眠标志
   }
 }   
}

//P2中断服务程序

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{ 
    Delay_ms(10);            //消抖10ms
    if((P2IN & BIT3)==0)
    {
      sleep=1;               //对睡眠标志位置位，准备休眠
    }
    if((P2IN & BIT4)==0)
    {
      sleep=0; 
      LPM4_EXIT;             //退出睡眠LPM3
    }
    P2IFG =0;                //P2中断标志位清零
}  
