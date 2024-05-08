#ifndef __PulseSystemConfig_h__
#define __PulseSystemConfig_h__

//#include "stdint.h"
//#include <stdbool.h>
//#include "FitShow.h"
#include "gpio.h"


//宏定义声明		
#define PULSE_PIN_A 0x01		//脉冲引脚号--Pin01
#define	PULSE_PIN_B 0x02		//脉冲引脚号--Pin02
//#define PinA_Interrupt_Level System_Para_Config.Pulse_Pin_A.Pin_Pull_Type   //中断电平
//#define PinB_Interrupt_Level !!System_Para_Config.Pulse_Pin_B.Pin_Pull_Type

// 引脚输出模式的默认电平状态  
typedef enum  
{  
    Low_Level,  // 高电平  
    High_Level   // 低电平  
} Pin_Output_Mode_Default_Level_e;  
  
// 脉冲的延迟时间  
typedef struct  
{  
    uint16_t max_Pulse_Interval;  // 最大脉冲间隔  
    uint16_t min_Pulse_Interval;  // 最小脉冲间隔  
} Delay_Time_t;  
  
// 脉冲引脚配置
typedef struct  
{  
    uint8_t 			pin_number;  // 引脚编号  
    Dir_Type 			Pin_Dir_Type;  // 引脚方向类型  
    Pull_Type 		Pin_Pull_Type;  // 引脚上下拉类型  
    Pin_Output_Mode_Default_Level_e Pin_Output_Mode_Default_Level;  // 引脚输出模式的默认电平  
    Delay_Time_t 	Delay_Time;  // 延迟时间  
} Pulse_Pin_Config;  
  
// 工作模式  
typedef enum  
{  
    Uart_Mode,    // 串口模式  
    Pulse_Mode    // 脉冲模式  
} Work_Mode_e;  
  
// 脉冲计数方式  
typedef enum  
{  
    Single_Pulse_Mode,       // 单感模式  
    Double_Pulse_Mode,       // 双感模式  
    Long_Short_Wave_Mode     // 长短波模式  
} Pulse_Count_Mode_e;  
  
// 卡路里计算方式
typedef enum  
{  
    Base_On_Distance,      // 基于距离  
    Base_On_Count          // 基于计数  
} Calories_Calculation_Method_e;  
  
// 计数参数 
typedef struct  
{  
    uint16_t pulse_per_count;   // 几个脉冲记一次，单位0.1，比如为10，那就一个脉冲记一次
    uint16_t distance_per_count; // 一次多少距离，单位1mm
} Count_Para_t;

// 卡路里计算系数
typedef struct  
{  
    uint16_t calories_distance_rate;   // 基于距离的卡路里计算系数
    uint16_t calories_count_rate; // 基于次数的卡路里计算系数
} Calories_Calculation_Para_t;  
  
// 系统参数相关配置
typedef struct  
{  
    Pulse_Pin_Config 							Pulse_Pin_A;  // 脉冲引脚1
    Pulse_Pin_Config 							Pulse_Pin_B;  // 脉冲引脚2
    Work_Mode_e 									Work_Mode;  // 工作模式  
    Pulse_Count_Mode_e 						Pulse_Count_Mode;  // 脉冲计数模式  
		Count_Para_t 									Count_Para;  // 计数参数
    Calories_Calculation_Method_e Calories_Calculation_Method;  // 卡路里计算方式
    Calories_Calculation_Para_t 	Calories_Calculation_Para;  // 卡路里计算系数 
    uint16_t 											pulse_interval;  // 每个脉冲之间的间隔
} System_Para_Config_t;



// 公英制、设备类型、厂商码、机型码已在其他地方声明

typedef	enum
{
	Low,
	High
}Pin_Interrupt_Level_e;

typedef struct  
{  
	uint32_t cur_pulse_time_stamp;
	uint32_t pre_pulse_time_stamp;
}Pulse_Time_Stamp_t;









#endif


