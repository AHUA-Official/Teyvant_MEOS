# AHUA_Teyvat_MSP430_开发进度

目标:

MSP430是一个经典的MCU（Micro Controller Unit）,包含CPU以及若干外设的完整单片系统。MSP430不仅可以作为一个单核使用，也常常被集成到异构多核系统中。

我们要认识、学习MSP430系统的硬件设计和软件开发环境，并在此基础上，掌握MSP430的中断控制、串口、定时器等外设使用，理解和掌握模拟数字转换器（ADC）的工作，完成温度计和动态电压计的应用软件。



具体要求

1.  结合计算机组成原理，认识MCU，包括CPU以及外设。理解MSP430单片机的应用硬件设计、掌握软件设计。(CPU+外设的不同，构建不同的系统！)

2.  通过MSP430的uart串口外设，建立于主机（PC机或外部其它系统）的通讯，可以用于控制MSP430代码运行或接收MSP430代码（打印）输出，即I/O

3.  理解和掌握MSP430 MCU的中断外设控制以及响应机制，包括：中断控制的寄存器以及各个bit位的含义，中断向量表，以及中断服务子程序。

4.  掌握MSP430 MCU的其它外设开发应用，如定时器timer，比较器，等

5.  理解和掌握MSP430自带的ADC（模拟信号到数字信号转换器）的工作，并应用ADC完成温度检测（MSP430G2自带温度传感器）输出（通过UART串口）,以及电压测量（包括平均电压，瞬态电压曲线绘制等）

6.  （可选拓展）所有应用代码没有基于OS！ 是否考虑TI-RTOS？更复杂的外设应用？USB,网络，SPI，…

7.  （可选拓展）MSP430g2 Launchpad可以才声音信号吗？ 硬件接口匹配电路，存贮，传输，恢复检查？

考核内容

**三、考核方式**

课程总成绩 = 平时成绩（20％）＋综合设计报告成绩（40％）＋学生互评贡献成绩（10%）+综合设计答辩成绩（30％）

综合设计报告评价，依据每组工作量体现、解决问题能力体现、软件工程知识点应用、设计思路和方法、实现思路和方法、测试和性能分析等方面给出成绩。报告评价内容针对课程目标AO1、AO2设计。评价标准见本课程报告评分细则。



### 第一周

知道了要干什么,安装下载了CCS软件

AHUA:同上,继续学习暑假时候的51单片机,因为没有板子



### 第2周

拿到了板子,开始检验各自的CCS环境,了解关于单片机的开发学习

### 第三周

CCS环境检验完成,完成烧录LED呼吸灯的实验

建立了大家的Git项目,

AHUA:进行了一些提交,不过大部分是文档和项目日志之类的东西,最有用的东西是学习51单片机的时候写的一些电子笔记,个人认为可能对一些外设模块的快速上手有用.

### 第4周



### 第5周

