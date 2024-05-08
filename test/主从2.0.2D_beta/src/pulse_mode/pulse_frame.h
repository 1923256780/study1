#ifndef __PULSE_FRAME_
#define __PULSE_FRAME_

#include <stdint.h>
#include "device.h"
#include "gpio.h"

#define SPEED_BUF_SIZE 5
#define HEART_BUF_SIZE 6

/** 脉冲模式参数 **/

// 工作模式  
typedef enum  
{  
    Uart_Mode,    // 串口模式  
    Pulse_Mode    // 脉冲模式  
} Work_Mode_e;  

// 外部输出引脚的默认电平状态  
typedef enum  
{  
    Low_Level,   // 低电平  
    High_Level   // 高电平  
} Default_Level_e;  

// 脉冲引脚配置
typedef struct  
{  
    uint8_t pin_number;  // 引脚编号  
    Pull_Type Pin_Pull_Type;  // 引脚上下拉类型  
    int_mode Int_Mode;  // 引脚输出模式的默认电平
} Pulse_Pin_Config_t;  
  
// 外部输出引脚配置
typedef struct  
{  
    uint8_t pin_number;  // 引脚编号
    Default_Level_e Default_Level;  // 引脚输出模式的默认电平  
} Output_Pin_Config_t;  

// 脉冲计数模式  
typedef enum  
{  
    Single_Pulse_Mode,       // 单感模式  
    Double_Pulse_Mode,       // 双感模式  
    Long_Short_Wave_Mode     // 长短波模式  
} Pulse_Count_Mode_e;

// 计数参数 
typedef struct  
{  
    uint16_t count;  // 次数
    uint16_t pulse_per_count;   // 几个脉冲记一次，单位0.1，比如为10，那就一个脉冲记一次
    uint16_t distance_per_count; // 一次多少距离，单位1mm
} Count_Para_t;

// 卡路里计算方式
typedef enum  
{  
    Base_On_Distance,      // 基于距离  
    Base_On_Count          // 基于计数  
} Calories_Calculation_Method_e;  
 
// 卡路里计算系数结构体
typedef struct  
{  
    uint16_t calories_distance_rate;   // 基于距离的卡路里计算系数
    uint16_t calories_count_rate;      // 基于次数的卡路里计算系数
} Calories_Calculation_Para_t;  

// 设备编码结构体
typedef struct  
{  
    uint8_t 	type;     // 设备类型
    uint16_t 	factory;  // 生产厂家
	uint16_t 	model;    // 设备型号
} Device_Code_t;

// 时间间隔参数结构体
typedef struct  
{  
	uint32_t cur_pulse_time_stamp;  // 当前脉冲时间戳
	uint32_t pre_pulse_time_stamp;  // 上一个脉冲时间戳
    uint16_t interval;              // 时间间隔
    uint16_t max_Interval;          // 最大脉冲间隔
    uint16_t min_Interval;          // 最小脉冲间隔
} Interval_Para_t;

// 双感波形状态枚举
typedef enum 
{
    IDLE,               // 空闲状态
    First_Pin_Pulse_1,  // 第一个引脚脉冲1
    Second_Pin_Pulse_1, // 第二个引脚脉冲1
    Second_Pin_Pulse_2, // 第二个引脚脉冲2
    First_Pin_Pulse_2   // 第一个引脚脉冲2
} Double_Pulse_Mode_State_e;

// 双感参数结构体
typedef struct
{
    Double_Pulse_Mode_State_e Double_Pulse_Mode_State;  // 双脉冲模式状态
    uint8_t first_pin;     // 第一个引脚
    uint8_t second_pin;    // 第二个引脚
} Dounble_Mode_Para_t;

// 活跃标志枚举
typedef enum  
{  
    Active,     // 活跃状态
    In_Active   // 非活跃状态
} Active_Flag_e;  

// 标志结构体
typedef struct  
{  
    Active_Flag_e Speed;   // 速度活跃标志
    Active_Flag_e Heart;   // 心率活跃标志
} Flag_t;

// 系统参数相关配置
typedef struct  
{  
    Work_Mode_e 					Work_Mode;  // 工作模式  
    Pulse_Pin_Config_t 				Speed_Pin_A;  // 脉冲引脚1
    Pulse_Pin_Config_t 				Speed_Pin_B;  // 脉冲引脚2             
    Pulse_Pin_Config_t              Heart_Pin;  // 心率引脚
    Output_Pin_Config_t             Output_Pin_A;  //外部输出引脚1
    Output_Pin_Config_t             Output_Pin_B;  //外部输出引脚2
    UnitDimension_e                 Unit;  //公英制单位
    Pulse_Count_Mode_e 				Count_Mode;  // 脉冲计数模式  
	Count_Para_t 					Count_Para;  // 计数参数
    Calories_Calculation_Method_e   Calories_Calculation_Method;  // 卡路里计算方式
    Calories_Calculation_Para_t 	Calories_Calculation_Para;  // 卡路里计算系数
    Device_Code_t                   Device_Code;  //设备编码
} Pulse_Para_Config_t;

// 公英制、设备类型、厂商码、机型码已在其他地方声明，初始化后顺便赋值过去

// 系统参数相关配置
typedef struct  
{ 
    Interval_Para_t 			    Speed_Interval;  // 速度脉冲间隔参数
    Interval_Para_t 			    Heart_Interval;  // 心率脉冲间隔参数
    Dounble_Mode_Para_t             Dounble_Mode_Para;  // 双感模式参数
    Flag_t                          Flag;  // 状态位
}Pulse_Sport_Para_t;

/** 全局变量声明 **/
extern Pulse_Para_Config_t Pulse_Para;  // 脉冲参数配置结构体
extern Pulse_Sport_Para_t Sport_Para;  // 运动参数结构体

/** 函数声明 **/
void Pulse_frame_Init(void);  // 初始化脉冲框架
void Pulse_Data_Cal(void);  // 脉冲数据计算
void Heart_Data_Cal(void);  // 心率数据计算

void Sport_Starting(void);  // 运动开始
void Sport_Stoping(void);  // 运动结束


#endif
