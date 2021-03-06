
/*++++++++++++++++++++++++++++++++++++++

主循环
邱毅
IO端口，模拟量端口，通信参数   启停、点动、换轴、滤波等功能
 +++++++++++++++++++++++++++++++++++++++*/
#include "stm32f10x.h"
#include "app_basicdef.h"
#include "app_pid.h"
#include "app_action.h"
#include "app_inout.h"
#include "bsp_dac.h"
#include "bsp_adc.h"
#include "app_kernelregs.h"
#include "bsp_led.h"
#include "bsp_enc.h"
#include "app_Port.h"


#define  Ain1MSpeedZero     0      
				//定义模拟量2输入口的0V输入，

//static u8 err;
void APortInput()				//模拟量输入口
{
	//张力
	if(Port.AIn.Ch1>=Param.Tens.Zero)
	{
		State.Work.InData=Port.AIn.Ch1-Param.Tens.Zero;
	}
	else
	{
	 	State.Work.InData=0;
	}
	State.Work.Tens=State.Work.InData*5;		  //张力值
	//主机速度
	if(Param.Basic.MainSpeedWay==1)		//
	{
		if(Port.AIn.Ch2>= Ain1MSpeedZero)	 //大于中间电压，输入为正
		{
			if(Port.AIn.Ch2>=Param.Basic.MainSpeedZero)
			{
				State.Work.MSpeed = (Port.AIn.Ch2-Param.Basic.MainSpeedZero)*5;			 //主速度百分比

			}
			else
			{
				State.Work.MSpeed=0;
			}
		}
		else	 //输入为负
		{
			if(Port.AIn.Ch2<=Param.Basic.MainSpeedZero)
			{
				State.Work.MSpeed =( Param.Basic.MainSpeedZero-Port.AIn.Ch2)*5;			 //主速度百分比

			}
			else
			{
				State.Work.MSpeed=0;
			}

		}
	}
	if(Param.Basic.SetWay==1)		//设置信号来自于模拟量4

	{
		Param.Auto.SetAim=Port.AIn.Ch4*5;
	}
	if(Param.Dai.Way==1)			//卷径信号来自于模拟量3
	{
  	  	State.Work.DiaPer =Port.AIn.Ch3/2;		  //   卷径百分比	 0.1%单位,  1000= 100%，1 =0.1%
	}

}


void APortOutput()				//模拟量输出口
{
	u32 APortOutputLong;
	if((Param.AOutPort.CH1OutMode==0)||(Param.AOutPort.CH1OutMode==1)||(Param.AOutPort.CH1OutMode==4)||(Param.AOutPort.CH1OutMode==5))
	{ 
		if(Param.AOutPort.CH1OutMode==0)    //0~5V
		{   
			if((State.Out.Data<=5000)&&(State.Out.Data>=0))		//5V以内
			{
				APortOutputLong=State.Out.Data*4095;
    			Port.AOut.Ch1=APortOutputLong/5000;
			}
			else
			{
			   Port.AOut.Ch1=4095;
			}
		}
		else								////0~10V
		{
			if(State.Out.Data<=10000)		//10V以内
			{
				APortOutputLong=State.Out.Data*4095;
    			Port.AOut.Ch1=APortOutputLong/10000;
			}
			else
			{
			   Port.AOut.Ch1=4095;
			}
		}
	}
	else
	{
	 	if(Param.Basic.MMFR==0)		  //正转
		{
			if(Param.AOutPort.CH1OutMode==3)		//-5~+5v
			{   
				if(State.Out.Data<=5000)		//5V以内
				{
					APortOutputLong=State.Out.Data*2047;
		    		Port.AOut.Ch1=2048+APortOutputLong/5000;
				}
				else
				{
					Port.AOut.Ch1=2048+2047;
				}
			}
			else
			{
				if(State.Out.Data<=10000)		//5V以内
				{
					APortOutputLong=State.Out.Data*2047;
		    		Port.AOut.Ch1=2048+APortOutputLong/10000;
				}
				else
				{
					Port.AOut.Ch1=2048+2047;
				}

			}
		}
		else
		{
			if(Param.AOutPort.CH1OutMode==3)		//-5~+5v
			{   
				if(State.Out.Data<=5000)		//5V以内
				{
					APortOutputLong=State.Out.Data*2047;
		    		Port.AOut.Ch1=2048-APortOutputLong/5000;
				}
				else
				{
					Port.AOut.Ch1=2048-2047;
				}
			}
			else
			{
				if(State.Out.Data<=10000)		//5V以内
				{
					APortOutputLong=State.Out.Data*2047;
		    		Port.AOut.Ch1=2048-APortOutputLong/10000;
				}
				else
				{
					Port.AOut.Ch1=2048-2047;
				}
			}
		}
	}
////*********************************通道2****************************************************88
	if((Param.AOutPort.CH2OutMode==0)||(Param.AOutPort.CH2OutMode==1)||(Param.AOutPort.CH2OutMode==4)||(Param.AOutPort.CH2OutMode==5))
	{ 
		if(Param.AOutPort.CH2OutMode==0)    //0~5V
		{   
			if(State.Out.Data2<=5000)		//5V以内
			{
				APortOutputLong=State.Out.Data2*4095;
    			Port.AOut.Ch2=APortOutputLong/5000;
			}
			else
			{
			   Port.AOut.Ch2=4095;
			}
		}
		else								////0~10V
		{
			if(State.Out.Data2<=10000)		//10V以内
			{
				APortOutputLong=State.Out.Data2*4095;
    			Port.AOut.Ch2=APortOutputLong/10000;
			}
			else
			{
			   Port.AOut.Ch2=4095;
			}
		}
	}
	else
	{
	 	if(Param.Basic.MMFR==0)		  //正转
		{
			if(Param.AOutPort.CH2OutMode==3)		//-5~+5v
			{   
				if(State.Out.Data2<=5000)		//5V以内
				{
					APortOutputLong=State.Out.Data2*2047;
		    		Port.AOut.Ch2=2048+APortOutputLong/5000;
				}
				else
				{
					Port.AOut.Ch2=2048+2047;
				}
			}
			else
			{
				if(State.Out.Data2<=10000)		//10V以内
				{
					APortOutputLong=State.Out.Data2*2047;
		    		Port.AOut.Ch2=2048+APortOutputLong/10000;
				}
				else
				{
					Port.AOut.Ch2=2048+2047;
				}

			}
		}
		else
		{
			if(Param.AOutPort.CH2OutMode==3)		//-5~+5v
			{   
				if(State.Out.Data2<=5000)		//5V以内
				{
					APortOutputLong=State.Out.Data2*2047;
		    		Port.AOut.Ch2=2048-APortOutputLong/5000;
				}
				else
				{
					Port.AOut.Ch2=2048-2047;
				}
			}
			else
			{
				if(State.Out.Data2<=10000)		//10V以内
				{
					APortOutputLong=State.Out.Data2*2047;
		    		Port.AOut.Ch2=2048-APortOutputLong/10000;
				}
				else
				{
					Port.AOut.Ch2=2048-2047;
				}
			}
		}
	}
}

uint  InputFilter(uint *InAddress)				//输放滤波
{
uint Filter,FTiems;
uint InAll;
	InAll=0;
	Filter=Param.Basic.ShowFilter/10;
	Filter=Filter+1;
	if(Filter>10)
	{
		Filter=10;						//从1到10
	}
	for(FTiems=1;FTiems<=Filter;FTiems++)
	{
		InAll=InAll+(*InAddress);
		InAddress++;
	}
	InAll=InAll/Filter;
	return(InAll);
}

uint  OutFilter(u16 ChAB)				//输出滤波
{
static u16 OldOutFilter1,OldOutFilter2;
uint Filter,FilterData;
u16 OldOut;
uint OutD,OutData;
uint OUTDAT[10];
unsigned long OutSum;
	if(ChAB==1)
	{
		for(OutD=0;OutD<10;OutD++)
		{
			OUTDAT[OutD]=OUTDATA[OutD];
	//		OutD++;
		}
		OldOut=OldOutFilter1;
	}
	else
	{
		for(OutD=0;OutD<10;OutD++)
		{
			OUTDAT[OutD]=OUTDATB[OutD];

		}
		OldOut=OldOutFilter2;
	}
	Filter=Param.Basic.OutFilter;			//临时
		OutSum=((long)OUTDAT[9])*0x00000064+((long)OUTDAT[8])*0x0000004b +((long)OUTDAT[7])*0x00000041+((long)OUTDAT[6])*0x0000003c +((long)OUTDAT[5])*0x00000037+((long)OUTDAT[4])*0x00000032+((long)OUTDAT[3])*0x00000028+((long)OUTDAT[2])*0x0000001e +((long)OUTDAT[1])*15+OUTDAT[0]*10;	//30％  100/500＝20％
		OutD=OutSum/500;

		FilterData=101-Filter;

	    if(OutD>=OldOut)
		{
	   		if((OutD-OldOut)>FilterData)
			{
			 	OutData=OldOut+FilterData;
			}
			else
			{
			  	OutData=OutD;
			}
		}
		else
		{
 	   		if((OldOut-OutD)>FilterData)
			{
				if(OldOut>FilterData)
				{
				 	OutData=OldOut-FilterData;
				}
				else
				{
				  	OutData=0;
				}
			}
			else
			{
			  	OutData=OutD;
			}
		}
		if(ChAB==1)
		{
			OldOutFilter1=OutData;
		}
		else
		{
			OldOutFilter2=OutData;		
		}	
		return(OutData);
}

//****************************************参数校对********************************************
void ParamCheck()
{
	u16 ParamE=0;
	if(Param.Dai.Test==1)	 //检测卷
	{
		if(Param.Dai.Way==0)   //无方法
		{
			 ParamE=1;
			 State.Basic.ParamERRID=1;
		}
		if(Param.Dai.MostDia< Param.Dai.LeastDia)
		{
			 ParamE=1;
			 State.Basic.ParamERRID=2;	
		}
	}
	if(	Param.Basic.ZDLine==1)	  //锥度
	{
		if(Param.Dai.Test==0)	 //不检测卷
		{
			 ParamE=1;
			 State.Basic.ParamERRID=3;	 //有锥度一定得检测卷径
		}
		if((100<Param.ZDZ.ZDZ10)||(Param.ZDZ.ZDZ10<Param.ZDZ.ZDZ20)||(Param.ZDZ.ZDZ20<Param.ZDZ.ZDZ30)||(Param.ZDZ.ZDZ30<Param.ZDZ.ZDZ40)||(Param.ZDZ.ZDZ40<Param.ZDZ.ZDZ50))
		{
			 ParamE=1;
			 State.Basic.ParamERRID=4;	   //锥度值一定是卷径越小，百分比越大
		}
		if((Param.ZDZ.ZDZ50<Param.ZDZ.ZDZ60)||(Param.ZDZ.ZDZ60<Param.ZDZ.ZDZ70)||(Param.ZDZ.ZDZ70<Param.ZDZ.ZDZ80)||(Param.ZDZ.ZDZ80<Param.ZDZ.ZDZ90)||(Param.ZDZ.ZDZ90<Param.ZDZ.ZDZ100))
		{
			  ParamE=1;
			 State.Basic.ParamERRID=4;	  //锥度值一定是卷径越小，百分比越大
		}
	}
	if(ParamE==1)
	{
	  State.Basic.ParamERR=1;
	}
	else  if(ParamE==0)
	{
	  State.Basic.ParamERR=0;
	}  

}

void   App_Paramload(void)
{
	static u16  StartRead;       //部分参数只在启动时读一次
	union  PLCCommandUnion	PLC_Command_Data;
	u16 LedOnN;				//指示灯

	union  PLCCommandUnion2	PLC_Command_Data2;
	u16 IDIO1,IDIO2,IDIO3,IDIO4;     //ID设置的IO口状态

	KernelRegs[64]=0;

   Param.Auto.OpenOut=KernelRegs[0];				//预输出值
   Param.Auto.StartOut=KernelRegs[1];				//启动增益
   Param.Auto.StartDelayTime=KernelRegs[2];			//启动延时奔�
   Param.Auto.StopOut=KernelRegs[3];				//停车增益
   Param.Auto.StopDelayTime=KernelRegs[4];			//停车延时时间
//5 点动输出值 与9重服，删
	Param.Auto.ChangeOut=KernelRegs[6];
	Param.Auto.ChangeTime=KernelRegs[7];
	Param.Auto.ChangeStopTime=KernelRegs[8];

   Param.Auto.JogOut=KernelRegs[9];					//点动输出值	

   Param.Basic.ZeroTest=KernelRegs[10]*10;	 			//欠张力检测值
   Param.Basic.ZeroTestTiems=KernelRegs[11];	 	//欠张力检测时间
   Param.Basic.OverTest=KernelRegs[12]*10;	 			//过张力检测值
   Param.Basic.OverTestTiems=KernelRegs[13];	 	//过张力检测时间

   Param.Basic.MaxMainSpeedPor=KernelRegs[14];		//卷轴最小时对应主速度的输出比率，（相对主速度的最大输出速度比率）

 // =KernelRegs[15];     				//
   Param.Basic.Mode=KernelRegs[16];  				//1：经典PID 控制2：模糊PID 控制3：开环卷径控制4：带卷径补尝的模糊PID 控制
   Param.Basic.MachMode=KernelRegs[17];				 //机械特性
   Param.Basic.MostOut=KernelRegs[18];				//最大输出值
   Param.Basic.LeastOut=KernelRegs[19];				//最小输出值
	Param.Basic.SetOut=KernelRegs[78];				//设置手动模式下的输出值
   Param.Basic.ZDLine=KernelRegs[20];				//0:无锥度,1:有锥度
 	Param.Basic.OutFilter=KernelRegs[21];// 输出滤波，须加
   Param.Dai.Test=KernelRegs[22];				//0"不检测，1检测
   	if(KernelRegs[23]==1)			////输出控制信号1/2   2：扭矩信号，1 速度信号
   	{
  		 Param.Basic.Out=2;	 				//输出控制信号1，2 1：扭矩信号，2 速度信号
	}
	else  if(KernelRegs[23]==2)
	{
   		 Param.Basic.Out=1;	 				//输出控制信号1，2 1：扭矩信号，2 速度信号
	}

   Param.Basic.Chang=KernelRegs[24]; 				//1:单轴，2：双轴
   Param.Basic.KPid=KernelRegs[25]; 				//PID微调比率，最终客户用
   Pid.Pol=KernelRegs[26];	    					//PID极性
   Pid.CycT=KernelRegs[27];	 //PID周期}
   Pid.KP=KernelRegs[28];	 //PID  KP设置
   Pid.KI=KernelRegs[29];	 //PID周期
   Pid.KD=KernelRegs[30];	 //PID周期
   Pid.KB= KernelRegs[31];	 //PID超调区增益
   Pid.KS= KernelRegs[32];	 //PID死区增益
   Pid.Die= KernelRegs[33];	 //PID死区范围
   Pid.Over= KernelRegs[34];	 //PID死超调区范围
   Pid.Change= KernelRegs[35];		//PID输入跳变率： 0~100% 40％

   Param.Dai.Way=KernelRegs[36];	//卷径检测方法
   Param.Dai.Ply=KernelRegs[37];	 //卷料厚度
   Param.Dai.RollD=KernelRegs[38];	 //卷轴脉冲数
   Param.Dai.MainD=KernelRegs[39];	 //主轴脉冲数
   Param.Dai.LeastDia=KernelRegs[40];	 //最小卷
   Param.Dai.MostDia=KernelRegs[41];	 //最大卷
   Param.Dai.RollMWay=KernelRegs[42];	 //主轴脉冲信号来自于	0：无  1：IO口5   2：编码器口
   Param.Dai.Ratio=KernelRegs[43];	 //	//主轴直径/主轴卷轴比

    Pid.KPV=KernelRegs[46];	 //PID  KP设置
    Pid.KPD=KernelRegs[47];	 //PID  KP设置

    Param.Dai.RollAWay=  KernelRegs[48]; 	  //卷轴A脉冲信号来自于	0：无  1：IO口6   2：编码器口
    Param.Dai.RollBWay =KernelRegs[49]; 	//卷轴b脉冲信号来自于	0：无  1：IO口5   2：编码器口

   Param.Basic.MainSpeedWay=KernelRegs[50];  //主速度来自于		  1：来自模拟输入口2：来自于通信3 来自于编码器4 来自于IO 口6
   Param.Basic.SetWay=KernelRegs[51];  		//设置信号来自于  1：模拟量，2通信

   Param.ZDZ.ZDZ10=KernelRegs[52];  		//10%卷径时的  锥度
   Param.ZDZ.ZDZ20=KernelRegs[53];  		//20%卷径时的  锥度
   Param.ZDZ.ZDZ30=KernelRegs[54];  		//30%卷径时的  锥度
   Param.ZDZ.ZDZ40=KernelRegs[55];  		//40%卷径时的  锥度
   Param.ZDZ.ZDZ50=KernelRegs[56];  		//50%卷径时的  锥度
   Param.ZDZ.ZDZ60=KernelRegs[57];  		//60%卷径时的  锥度
   Param.ZDZ.ZDZ70=KernelRegs[58];  		//70%卷径时的  锥度
   Param.ZDZ.ZDZ80=KernelRegs[59];  		//80%卷径时的  锥度
   Param.ZDZ.ZDZ90=KernelRegs[60];  		//90%卷径时的  锥度
   Param.ZDZ.ZDZ100=KernelRegs[61];  		//100%卷径时的  锥

   Param.Basic.MSPAF=KernelRegs[62];		  //速度前馈
   Param.Basic.AutoClearDia=KernelRegs[68];	 //当RUN信号由1变0时，自动把卷径值清除

    if(StartRead==0)						//以下4个参数只在启动时读一次
   {
   		Param.AOutPort.CH1OutMode=KernelRegs[44];	 //模拟量输出口1模式
   		Param.AOutPort.CH2OutMode=KernelRegs[45];	 //模拟量输出口1模式
   		Param.AOutPort.CH3OutMode=KernelRegs[46];	 //模拟量输出口1模式
   		Param.AOutPort.CH4OutMode=KernelRegs[47];	 //模拟量输出口1模式

		Param.Tens.Zero=KernelRegs[63];//	KernelRegs[63];			  //张力零点
		Param.Basic.MainSpeedZero=KernelRegs[64];// KernelRegs[64];		//速度	零点

//		KernelRegs[67]		  67	卷径修正比储存
		StartRead=1;
		IDIO1=1-GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4);
		IDIO2=1-GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
		IDIO3=1-GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);
		IDIO4=1-GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9);
		State.Basic.ID=(IDIO1+(IDIO2<<1)+(IDIO3<<2)+(IDIO4<<3))+1;
		if(State.Basic.ID<=16)					//小于10以内有效
		{
		   	State.Basic.ID=State.Basic.ID;
		}
		else
		{
	   		State.Basic.ParamERR=1;			//参数错误
			State.Basic.ParamERRID=5;	  //拨码开关错误
		}

		if(Param.Basic.AutoClearDia==0)		 //停机手动保存，上电手动恢复
		{
			if((KernelRegs[65]>=1))	//数据有效	,已有保存的卷径值
			{
			 	State.ToPLC.Bit.Save=1;
			}
		}
		else if(Param.Basic.AutoClearDia==2)		 //停机自动保存，上电自动恢复
		{
			if((KernelRegs[65]>=1))			   //轴卷径已记忆
			{
				State.Work.DiaPer=(KernelRegs[65]&0x00ff-1)*5;  //A轴卷荆百分比
				State.Work.Dia=((Param.Dai.MostDia-Param.Dai.LeastDia)*State.Work.DiaPer)/1000+Param.Dai.LeastDia;
				
				State.Work.DiaPer2=(((KernelRegs[65]&0xff00)>>8)-1)*5;  //B轴卷荆百分比
				State.Work.Dia2=((Param.Dai.MostDia-Param.Dai.LeastDia)*State.Work.DiaPer2)/1000+Param.Dai.LeastDia;
						//	KernelRegs[65]=0;

				if(Param.Dai.Way==2)		//卷料厚度累加法
				{
					if(State.Work.DiaPer>0)
					{
						if(Param.Basic.MachMode==1)				//放卷
						{
							State.Work.RollDT=((Param.Dai.MostDia-State.Work.Dia)*10)/(Param.Dai.Ply*2);
							State.Work.RollDT=State.Work.RollDT*Param.Dai.RollD;
						}
						else if(Param.Basic.MachMode==2)				//收卷
						{
 								State.Work.RollDT=((State.Work.Dia-Param.Dai.LeastDia)*10)/(Param.Dai.Ply*2);
										State.Work.RollDT=State.Work.RollDT*Param.Dai.RollD;
						}
					}
					if	(State.Work.DiaPer2>0)
					{
  						if(Param.Basic.MachMode==1)				//放卷
						{
							State.Work.RollDT2=((Param.Dai.MostDia-State.Work.Dia2)*10)/(Param.Dai.Ply*2);
							State.Work.RollDT2=State.Work.RollDT2*Param.Dai.RollD;
						}
						else if(Param.Basic.MachMode==2)				//收卷
						{
							State.Work.RollDT2=((State.Work.Dia2-Param.Dai.LeastDia)*10)/(Param.Dai.Ply*2);
							State.Work.RollDT2=State.Work.RollDT2*Param.Dai.RollD;
						}
					}
				}
			}
		}
		if(KernelRegs[66]==1) //有异常停机
		{
		 	KernelRegs[66]=0;	 //清除上次异常停机
			State.ToPLC.Bit.Break=1;         //0:正常，1：上次有非正常停机，须用户确认
		}
	}

   	if(Param.Basic.SetWay==2)	  			//  设置值       2：来自于通信
	{
   		Param.Auto.SetAim=KernelRegs[69]*10;		  //设定目标值	   //(0~1000,对应0～10V)
	}
 
	if(Param.Basic.MainSpeedWay==2)	  			//   主速度2：来自于通信
	{
		if(KernelRegs[75]<=1000)
		{
	 		State.Work.MSpeed=(KernelRegs[75]*10);          //(0~1000,对应0～10V)
		}
	}
	if(Param.Dai.Way==6)	 				//卷径信号来自于通信
	{
		if(KernelRegs[76]<=100)			//0%~100%  超出无效
		{
			if(State.Basic.OutCh==0)			//A轴输出模式
			{
	    		State.Work.DiaPer=KernelRegs[76]*10; 
				State.Work.Dia=((Param.Dai.MostDia-Param.Dai.LeastDia)*State.Work.DiaPer)/1000+Param.Dai.LeastDia;
			}
			else
			{
	    		State.Work.DiaPer2=KernelRegs[76]*10; 
				State.Work.Dia2=((Param.Dai.MostDia-Param.Dai.LeastDia)*State.Work.DiaPer2)/1000+Param.Dai.LeastDia;
			}
		}
	}

//**************************状态控制位*************************************************************
	  PLC_Command_Data.All=KernelRegs[79]; 	  //读取数据
	  PLC_Command_Data2.All=KernelRegs[73]; 
	  if(PLC_Command_Data.All!= State.PLC_Command.All)		  //比较是否有变化
	  {				   											//逐位比较			   											//逐位比较
			if(PLC_Command_Data.Bit.Run!=State.PLC_Command.Bit.Run)		 
			{
			    RunSwitch(PLC_Command_Data.Bit.Run);              //运行RUN控制。
	//			PLC_Command_Data.Bit.Run=0;		//清0，表示命令已消费
			}
			if(PLC_Command_Data.Bit.Sampling!=State.PLC_Command.Bit.Sampling)
			{
	//		  	PLC_Command_Data.Bit.Sampling!=State.PLC_Command.Bit.Sampling
			}
			if(PLC_Command_Data.Bit.Refurbish!=State.PLC_Command.Bit.Refurbish)
			{

			}
			if(PLC_Command_Data.Bit.Off_Line!=State.PLC_Command.Bit.Off_Line)
			{

			}
			if(PLC_Command_Data.Bit.JOG!=State.PLC_Command.Bit.JOG)
			{

				IOJog(PLC_Command_Data.Bit.JOG);
		//		PLC_Command_Data.Bit.JOG
			}
			if(PLC_Command_Data.Bit.XZFX!=State.PLC_Command.Bit.XZFX)
			{
				MainMotFR(PLC_Command_Data.Bit.XZFX);		   	//主输出电机正反转控制				
			}
			if(PLC_Command_Data.Bit.ERR!=State.PLC_Command.Bit.ERR)
			{

			}
			if(PLC_Command_Data.Bit.Reset!=State.PLC_Command.Bit.Reset)	   //断点复位
			{
				if(PLC_Command_Data.Bit.Reset==1)
				{
				 	State.ToPLC.Bit.Break=0;         //0:正常，1：上次有非正常停机，须用户确认
					PLC_Command_Data.Bit.Reset=0;
				}
			}
			if(PLC_Command_Data.Bit.Save!=State.PLC_Command.Bit.Save)	   //卷径保存
			{
		   		if(PLC_Command_Data.Bit.Save==1)
				{
				 	PLC_Command_Data.Bit.Save=0;
					DiaSave();
				}
			}
			if(PLC_Command_Data.Bit.ClearSave!=State.PLC_Command.Bit.ClearSave)	  //清除卷径记忆
			{
				if(PLC_Command_Data.Bit.ClearSave==1)
				{
				 	State.ToPLC.Bit.Save=0;
					KernelRegs[65]=0;
					RegsFlag[65] =1;	 //往EEPROM里写
				//	State.Work.DiaPer=0;
					PLC_Command_Data.Bit.ClearSave=0;
				}
			}
			if(PLC_Command_Data.Bit.UseSave!=State.PLC_Command.Bit.UseSave)
			{
				if(PLC_Command_Data.Bit.UseSave==1)
				{
				 	 if((KernelRegs[65]>=1))			   //轴卷径已记忆
					 {
							State.Work.DiaPer=(KernelRegs[65]&0x00ff-1)*5;  //A轴卷荆百分比
						    State.Work.Dia=((Param.Dai.MostDia-Param.Dai.LeastDia)*State.Work.DiaPer)/1000+Param.Dai.LeastDia;
				
							State.Work.DiaPer2=(((KernelRegs[65]&0xff00)>>8)-1)*5;  //B轴卷荆百分比
						    State.Work.Dia2=((Param.Dai.MostDia-Param.Dai.LeastDia)*State.Work.DiaPer2)/1000+Param.Dai.LeastDia;
						//	KernelRegs[65]=0;
							if(Param.Dai.Way==2)		//卷料厚度累加法
							{
								if(State.Work.DiaPer>0)
								{
									if(Param.Basic.MachMode==1)				//放卷
									{
										State.Work.RollDT=((Param.Dai.MostDia-State.Work.Dia)*10)/(Param.Dai.Ply*2);
										State.Work.RollDT=State.Work.RollDT*Param.Dai.RollD;
									}
									else if(Param.Basic.MachMode==2)				//收卷
									{
 										State.Work.RollDT=((State.Work.Dia-Param.Dai.LeastDia)*10)/(Param.Dai.Ply*2);
										State.Work.RollDT=State.Work.RollDT*Param.Dai.RollD;
									}

								}
								if	(State.Work.DiaPer2>0)
								{
  									if(Param.Basic.MachMode==1)				//放卷
									{
										State.Work.RollDT2=((Param.Dai.MostDia-State.Work.Dia2)*10)/(Param.Dai.Ply*2);
										State.Work.RollDT2=State.Work.RollDT2*Param.Dai.RollD;
									}
									else if(Param.Basic.MachMode==2)				//收卷
									{
										State.Work.RollDT2=((State.Work.Dia2-Param.Dai.LeastDia)*10)/(Param.Dai.Ply*2);
										State.Work.RollDT2=State.Work.RollDT2*Param.Dai.RollD;
									}
								}
							}
					 }
					PLC_Command_Data.Bit.UseSave=0;
				}
			}
			if(PLC_Command_Data.Bit.ClearDia!=State.PLC_Command.Bit.ClearDia)
			{
				if(PLC_Command_Data.Bit.ClearDia==1)			//卷径清0
				{
					DaiClear();	  //卷径数据清0
					PLC_Command_Data.Bit.ClearDia=0;	   		//	 //清0，表示命令已消费
				}

			}
			if(PLC_Command_Data.Bit.TensZero!=State.PLC_Command.Bit.TensZero)
			{
				if(PLC_Command_Data.Bit.TensZero==1)
				{
				 	Param.Tens.Zero=Port.AIn.Ch1;	  		//设置当前端口输入值为张力零点
					 KernelRegs[63]=  Param.Tens.Zero;
					 PLC_Command_Data.Bit.TensZero=0;		  //清0，表示命令已消费
				}
			}
			if(PLC_Command_Data.Bit.MainSpeedZero!=State.PLC_Command.Bit.MainSpeedZero)
			{
				if(PLC_Command_Data.Bit.MainSpeedZero==1)
				{
					Param.Basic.MainSpeedZero=Port.AIn.Ch2;	  		//设置当前端口输入值为主轴速度零点
					KernelRegs[64]=	Param.Basic.MainSpeedZero;
					PLC_Command_Data.Bit.MainSpeedZero=0;	 //清0，表示命令已消费
				}
			}
			if(PLC_Command_Data.Bit.ABChange!=State.PLC_Command.Bit.ABChange)
			{
				ChangAction(PLC_Command_Data.Bit.ABChange);
			}
			if(PLC_Command_Data.Bit.DiaSet!=State.PLC_Command.Bit.DiaSet)
			{
			 	if(PLC_Command_Data.Bit.DiaSet==1)
				{
					if(State.Basic.OutCh==0)			//A轴输出模式	
					{
						State.Work.DiaPer=KernelRegs[74]*10; 
						State.Work.Dia=((Param.Dai.MostDia-Param.Dai.LeastDia)*State.Work.DiaPer)/1000+Param.Dai.LeastDia;
						if(Param.Dai.Way==2)		//卷料厚度累加法
						{
								if(State.Work.DiaPer>0)
								{
									if(Param.Basic.MachMode==1)				//放卷
									{
										State.Work.RollDT=((Param.Dai.MostDia-State.Work.Dia)*10)/(Param.Dai.Ply*2);
										State.Work.RollDT=State.Work.RollDT*Param.Dai.RollD;
									}
									else if(Param.Basic.MachMode==2)				//收卷
									{
 										State.Work.RollDT=((State.Work.Dia-Param.Dai.LeastDia)*10)/(Param.Dai.Ply*2);
										State.Work.RollDT=State.Work.RollDT*Param.Dai.RollD;
									}

								}
						}
					}
					else   if(State.Basic.OutCh==1)			//B轴输出模式	
					{
					    State.Work.DiaPer2=KernelRegs[74]*10; 
						State.Work.Dia2=((Param.Dai.MostDia-Param.Dai.LeastDia)*State.Work.DiaPer2)/1000+Param.Dai.LeastDia;
						if(Param.Dai.Way==2)		//卷料厚度累加法
						{
 								if	(State.Work.DiaPer2>0)
								{
  									if(Param.Basic.MachMode==1)				//放卷
									{
										State.Work.RollDT2=((Param.Dai.MostDia-State.Work.Dia2)*10)/(Param.Dai.Ply*2);
										State.Work.RollDT2=State.Work.RollDT2*Param.Dai.RollD;
									}
									else if(Param.Basic.MachMode==2)				//收卷
									{
										State.Work.RollDT2=((State.Work.Dia2-Param.Dai.LeastDia)*10)/(Param.Dai.Ply*2);
										State.Work.RollDT2=State.Work.RollDT2*Param.Dai.RollD;
									}
								}
						}
					}
				}
				PLC_Command_Data.Bit.DiaSet=0;
			}
			//OSMutexPend(RegsWriteMutex,0,&err);
		KernelRegs[79]=	PLC_Command_Data.All;	//反回，表示部分命令已被消费 //新协议，无需返回
			//OSMutexPost(RegsWriteMutex);																				 
			State.PLC_Command.All=PLC_Command_Data.All;	
	  
  }
   if(PLC_Command_Data2.All!= State.PLC_Command2.All)		  //比较是否有变化
   {
 		if(PLC_Command_Data2.Bit.InitParam!=State.PLC_Command2.Bit.InitParam)	//	  //恢复出厂设置
		{
		 	if(	PLC_Command_Data2.Bit.InitParam==1)
			{
				ParamInit();					//恢复出厂设置值
				PLC_Command_Data2.Bit.InitParam=0;
			}

		}
		if(PLC_Command_Data2.Bit.ADCZero!=State.PLC_Command2.Bit.ADCZero)	//	 Bit1:1 模拟量输入校零
		{
		 	if(PLC_Command_Data2.Bit.ADCZero==1)
			{
				ADCZeroS();
				PLC_Command_Data2.Bit.ADCZero=0;
			}
		}
		if(PLC_Command_Data2.Bit.ADCTest!=State.PLC_Command2.Bit.ADCTest)	//	 Bit2:1 模拟量输入校准
		{
		 	if(PLC_Command_Data2.Bit.ADCTest==1)
			{
			 	ADCTsetS();
				PLC_Command_Data2.Bit.ADCTest=0;
			}
		}
 		State.PLC_Command2.All=PLC_Command_Data2.All;	
		KernelRegs[73]=PLC_Command_Data2.All;
   }
	 if(KernelRegs[77]==4001)		  //设置ADC输入口，修正值，临时用
	 {
		 ADCZeroS();
		if(State.Out.Mode==2)	
		{
	   		State.Out.Mode=0;
		}
	}
	else  if(KernelRegs[77]==4002)
	{
			ADCTsetS();
		if(State.Out.Mode==2)	
		{
	   		State.Out.Mode=0;
		}
	}
	else  if((KernelRegs[77]>1000)&&(KernelRegs[77]<=2000))
	{
		if(State.Out.Mode==0)	
		{
	   		State.Out.Mode=2;
		}
	}

	if((KernelRegs[78]>1000)&&(KernelRegs[78]<=2000))
	{
		if(State.Out.Mode2==0)	
		{
	   		State.Out.Mode2=2;
		}
	}
	else
	{
		if(State.Out.Mode2==2)	
		{
	   		State.Out.Mode2=0;
		}
	}
//**********************状态上报至PLC********************************************************
	if(State.Basic.Run!=0)
	{
   		State.ToPLC.Bit.Run=1;
	}
	else
	{
   		State.ToPLC.Bit.Run=0;
	}
   	State.ToPLC.Bit.ZeroFLG= State.Basic.ZeroFLG;
   	State.ToPLC.Bit.OverFLG= State.Basic.OverFLG;
   	State.ToPLC.Bit.JOG= State.Basic.Jog;
   	State.ToPLC.Bit. XZFX=Param.Basic.MMFR;				//0：正转，1反转;    //输出旋转方向，0～10V：0，0～-10V:1
	State.ToPLC.Bit.ERR=State.Basic.Alarm;
	State.ToPLC.Bit.OK=State.Basic.OK;			//已经就绪
//	State.ToPLC.Bit.Save=0;		    //未记忆
//    State.ToPLC.Bit.Break=0;         //0:正常，1：上次有非正常停机，须用户确认
	State.ToPLC.Bit.A_B=State.Basic.OutCh ;  //0:A轴，1：B
	State.ToPLC.Bit.EepromErr= State.Work.EEPROMERR;
	State.ToPLC.Bit.DiaErr=	State.Work.DIAERR;
//	State.ToPLC.Bit.ParamC
	State.ToPLC.Bit.ParamErr= State.Basic.ParamERR;
	State.ToPLC.Bit.USBState= State.Basic.USBState;

	State.ToPLC.Bit.DiaOK=State.Basic.DiaOK;

	KernelRegs[80]=	State.ToPLC.All;
	KernelRegs[81]=	State.Work.Tens/10;		//0~10000	 实际张力
	KernelRegs[82]=	PidOut/10;		//0~10000	 PID控制输出值
	KernelRegs[83]=	(Pid.SetD)/10;	  //PID目标值  =设置值*锥度%
	KernelRegs[84]=	Param.Auto.SetAim/10;	 //	设置值
	KernelRegs[85]=	State.Work.MSpeed/10;		//主速度值
	if(State.Basic.OutCh==0)			//A轴输出模式	
	{
		KernelRegs[86]=	State.Work.Dia;
	}
	else  if(State.Basic.OutCh==1)			//B轴输出模式	
	{
		KernelRegs[86]=	State.Work.Dia2;
	}
	KernelRegs[87]=(* BSP_ENC_CNT);
	KernelRegs[88]=((* BSP_ENC_CNT)&0xffff0000)>>16;
//	KernelRegs[89]=((* BSP_ENC_CNT)&0xffff00000000)>>32;
	KernelRegs[90]=Port.DIn.All;		//IO 输入口总
	KernelRegs[91]=Port.AIn.Ch1;	   //1940
	KernelRegs[92]=Port.AIn.Ch2;		   //927  870
	KernelRegs[93]=Port.AIn.Ch3;							//1790
	KernelRegs[94]=Port.AIn.Ch4;						   //1800
	KernelRegs[99]=	State.Basic.ID;

	ParamCheck();			//参数校对

/*		if(Param.Basic.MachMode==1)		 //放卷
		{
		 	Param.Basic.MMFR=0;				//0：正转，1反转
		}
		else
		{
	   		Param.Basic.MMFR=1;				//0：正转，1反转
		}
   */
											//输出值大小指示灯
	if(State.Basic.OutCh==1)			//B轴输出模式
	{
		if(State.Out.Data2>0)
		{
			LedOnN=((State.Out.Data2-(Param.Basic.MostOut/10))/(Param.Basic.MostOut*12))+1;
		}
		else
		{
		  	LedOnN=0;
		}
	}
	else
	{
		if(State.Out.Data>0)
		{
			LedOnN=((State.Out.Data-(Param.Basic.MostOut/10))/(Param.Basic.MostOut*12))+1;
		}
		else
		{
		  	LedOnN=0;
		}
	}
  	bsp_LedDisplay(LedOnN);
 //*************************************************************************************
}

void AInPortAdjudt()
{
	u16 IICSaveS,SDF;
	u16  CHHH1K,CHHH2K,CHHH3K,CHHH4K,CHHH2Z;
	CHHH1K=2130;   //2053  1940
 	CHHH2K=1236;
	CHHH3K=1810;	  //1790
 	CHHH4K=1790;
 	CHHH2Z=1354;	//927
	IICSaveS=SendWordEEPROM(400,CHHH1K);	   //第一通道系数修正
	while(IICSaveS==0);
	OSTimeDly(1);
	SDF=~  CHHH1K;
	IICSaveS=SendWordEEPROM(500,SDF);
	while(IICSaveS==0);
	OSTimeDly(1);

	IICSaveS=SendWordEEPROM(402,CHHH2K);	   //第2通道系数修正
	while(IICSaveS==0);
	OSTimeDly(1);
	SDF=~  CHHH2K;
	IICSaveS=SendWordEEPROM(502,SDF);
	while(IICSaveS==0);
	OSTimeDly(1);

	IICSaveS=SendWordEEPROM(404,CHHH3K);	   //第3通道系数修正
	while(IICSaveS==0);
	OSTimeDly(1);
	SDF=~  CHHH3K;
	IICSaveS=SendWordEEPROM(504,SDF);
	while(IICSaveS==0);
	OSTimeDly(1);

	IICSaveS=SendWordEEPROM(406,CHHH4K);	   //第4通道系数修正
	while(IICSaveS==0);
	OSTimeDly(1);
	SDF=~CHHH4K;
	IICSaveS=SendWordEEPROM(506,SDF);
	while(IICSaveS==0);
	OSTimeDly(1);
// #if NEWBROD

	IICSaveS=SendWordEEPROM(408,CHHH2Z);	   //第4通道系数修正
	while(IICSaveS==0);
	OSTimeDly(1);
	SDF=~  CHHH2Z;
	IICSaveS=SendWordEEPROM(508,SDF);
	while(IICSaveS==0);
	OSTimeDly(1);
//#endif
}

void UsbState()
{

}


void ADCZeroS()
{
	u16 	Ch2M;
	u16 IICSaveS,SDF;
	Ch2M= (State.ADC.Ch2Buf[0]+State.ADC.Ch2Buf[1]+State.ADC.Ch2Buf[2]+State.ADC.Ch2Buf[3]+State.ADC.Ch2Buf[4])/5;
	if((Ch2M>900)&&(Ch2M<1300))
	{
		Param.Basic.CH2Z=Ch2M;
	 	IICSaveS=SendWordEEPROM(408,Ch2M);	   //第2通道正负输入时的0V中间点设置
		while(IICSaveS==0);
		OSTimeDly(1);
		SDF=~  Ch2M;
		IICSaveS=SendWordEEPROM(508,SDF);
		while(IICSaveS==0);
		OSTimeDly(1);
	}
	else
	{
		State.Basic.ParamERR=1;			//参数错误
	}
}


void ADCTsetS()	   //IO口等于5V的时候测试
{
	u16 IICSaveS,SDF;
	u16  CHHH1K,CHHH2K,CHHH3K,CHHH4K;
//	Port.AIn.Ch1=(State.ADC.Ch1Buf[0]+State.ADC.Ch1Buf[1]+State.ADC.Ch1Buf[2]+State.ADC.Ch1Buf[3]+State.ADC.Ch1Buf[4]-Max-Min)/3;
	//Port.AIn.Ch1=Port.AIn.Ch1*2000/Param.Basic.CH1K;
	//5V  时  对应1000==CH1* 2000/X   X=CH1*2000/1000
	CHHH1K=((State.ADC.Ch1Buf[0]+State.ADC.Ch1Buf[1]+State.ADC.Ch1Buf[2]+State.ADC.Ch1Buf[3]+State.ADC.Ch1Buf[4])/5)*2;   //2053  1940
//			Port.AIn.Ch2=(Port.AIn.Ch2-Param.Basic.CH2Z)*2000/Param.Basic.CH2K;
//			1000=(CH2-CH2Z)*2000/X
 	CHHH2K=(((State.ADC.Ch2Buf[0]+State.ADC.Ch2Buf[1]+State.ADC.Ch2Buf[2]+State.ADC.Ch2Buf[3]+State.ADC.Ch2Buf[4])/5)-Param.Basic.CH2Z)*2;
	CHHH3K=((State.ADC.Ch3Buf[0]+State.ADC.Ch3Buf[1]+State.ADC.Ch3Buf[2]+State.ADC.Ch3Buf[3]+State.ADC.Ch3Buf[4])/5)*2;	  //1790
 	CHHH4K=((State.ADC.Ch4Buf[0]+State.ADC.Ch4Buf[1]+State.ADC.Ch4Buf[2]+State.ADC.Ch4Buf[3]+State.ADC.Ch4Buf[4])/5)*2;

	if((CHHH1K>1700)&&(CHHH1K<2300))
	{
		Param.Basic.CH1K=CHHH1K;
		IICSaveS=SendWordEEPROM(400,CHHH1K);	   //第一通道系数修正
		while(IICSaveS==0);
		OSTimeDly(1);
		SDF=~  CHHH1K;
		IICSaveS=SendWordEEPROM(500,SDF);
		while(IICSaveS==0);
		OSTimeDly(1);
	}
	else
	{
		State.Basic.ParamERR=1;			//参数错误
	}
	if((CHHH2K>700)&&(CHHH2K<1300))
	{
		Param.Basic.CH2K=CHHH2K;
		IICSaveS=SendWordEEPROM(402,CHHH2K);	   //第2通道系数修正
		while(IICSaveS==0);
		OSTimeDly(1);
		SDF=~  CHHH2K;
		IICSaveS=SendWordEEPROM(502,SDF);
		while(IICSaveS==0);
		OSTimeDly(1);
	}
	else
	{
		State.Basic.ParamERR=1;			//参数错误
	}
	if((CHHH3K>1700)&&(CHHH3K<2200))
	{
		Param.Basic.CH3K=CHHH3K;
		IICSaveS=SendWordEEPROM(404,CHHH3K);	   //第3通道系数修正
		while(IICSaveS==0);
		OSTimeDly(1);
		SDF=~  CHHH3K;
		IICSaveS=SendWordEEPROM(504,SDF);
		while(IICSaveS==0);
		OSTimeDly(1);
	}
	else
	{
		State.Basic.ParamERR=1;			//参数错误
	}
	if((CHHH4K>1700)&&(CHHH4K<2200))
	{
		Param.Basic.CH4K=CHHH4K;
		IICSaveS=SendWordEEPROM(406,CHHH4K);	   //第3通道系数修正
		while(IICSaveS==0);
		OSTimeDly(1);
		SDF=~  CHHH4K;
		IICSaveS=SendWordEEPROM(506,SDF);
		while(IICSaveS==0);
		OSTimeDly(1);
	}
	else
	{
		State.Basic.ParamERR=1;			//参数错误
	}
}
