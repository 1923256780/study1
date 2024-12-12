#include "timer_task_frame.h"
#include "pulse_frame.h"
#include "timer0.h"
#include "uart.h"
#include "gpio.h"
#include "icu.h"
#include <stddef.h>
#include "device.h"

/** 全局变量声明 **/
Timer_Task_t *Running_Time, *Data_Process, *Pulse_Detection, *Heart_Detection;  // 声明定时器任务结构体

/** 函数声明 **/

// 创建定时器任务，返回指向该任务的指针
Timer_Task_t* Create_Timer_Task(uint8_t is_looping, uint32_t duration, Timer_CallBack Callback)
{
    // 为定时器任务分配内存空间
    Timer_Task_t* Timer_Task = (Timer_Task_t*) ke_malloc(sizeof(Timer_Task_t), KE_MEM_NON_RETENTION);
    
    // 如果内存分配失败，返回空指针
    if (Timer_Task == NULL) {return NULL;}
    
    // 初始化定时器任务的属性
    Timer_Task->is_looping = is_looping;   // 是否循环执行
    Timer_Task->duration = duration;       // 定时器间隔
    Timer_Task->current_timer_count = 0;   // 当前计数
    Timer_Task->Callback = Callback;       // 定时器回调函数
    Timer_Task->TimerState = Timer_Idle;   // 定时器状态为闲置

    return Timer_Task;  // 返回定时器任务指针
}

// 启动定时器
void Start_Timer(Timer_Task_t* Timer_Task) 
{
    // 如果定时器任务为空，直接返回
    if (Timer_Task == NULL) {return;}
    
    // 设置定时器状态为运行
    Timer_Task->TimerState = Timer_Running;
}

// 停止定时器
void Stop_Timer(Timer_Task_t* Timer_Task) 
{
    // 如果定时器任务为空，直接返回
    if (Timer_Task == NULL) {return;}
    
    // 将当前计数重置为0，并将定时器状态设置为闲置
    Timer_Task->current_timer_count = 0;
    Timer_Task->TimerState = Timer_Idle;
}

// 重启定时器
void Restart_Timer(Timer_Task_t* Timer_Task) 
{
    // 如果定时器任务为空，直接返回
    if (Timer_Task == NULL) {return;}

    // 将当前计数重置为0，并将定时器状态设置为运行
    Timer_Task->current_timer_count = 0;
    Timer_Task->TimerState = Timer_Running;
}

// 销毁定时器任务
void Destroy_Timer_Task(Timer_Task_t* Timer_Task) 
{
    // 如果定时器任务不为空，释放内存空间
    if (Timer_Task != NULL) {ke_free(Timer_Task);}
}

// 处理定时器任务计数并执行回调函数
void Timer_Tick(Timer_Task_t* Timer_Task) 
{
    // 如果定时器任务为空或者定时器状态不是运行状态，直接返回
    if (Timer_Task == NULL || Timer_Task->TimerState != Timer_Running) {return;}

    // 定时器计数加1
    Timer_Task->current_timer_count++;
    
    // 如果定时器计数未达到定时器间隔，直接返回
    if (Timer_Task->current_timer_count < Timer_Task->duration) {return;}

    // 执行定时器回调函数，并将当前计数重置为0
    Timer_Task->Callback();
    Timer_Task->current_timer_count = 0;
    
    // 如果定时器为循环模式，则直接返回
    if (Timer_Task->is_looping != 0) {return;}
    
    // 如果定时器不是循环模式，则将定时器状态设置为闲置
    Timer_Task->TimerState = Timer_Idle;
}

// 系统时间，用于记录系统运行时间，单位为10ms
uint32_t system_time = 0;

// 运行时间任务，用于每10ms更新系统运行时间
void Running_Time_Task(void) 
{
    system_time ++; // 系统时间递增
}

// 数据处理任务，用于处理心率和脉冲数据
void Data_Process_Task(void) 
{
    Heart_Data_Cal();  // 计算心率数据
    Pulse_Data_Cal();  // 计算脉冲数据
}

// 脉冲检测任务，用于检测脉冲是否活跃
void Pulse_Detection_Task(void)
{
    Sport_Para.Flag.Speed = In_Active;  // 将脉冲活跃标志设置为不活跃
}

// 心率检测任务，用于检测心率是否活跃
void Heart_Detection_Task(void)
{
    Sport_Para.Flag.Heart = In_Active;  // 将心率活跃标志设置为不活跃
}

// 定时器任务列表初始化函数，用于创建并初始化各个定时器任务
static void TimerList_Init(void) 
{
    Running_Time = Create_Timer_Task(1, 1, Running_Time_Task);  // 创建并初始化运行时间任务，10ms执行一次
    Data_Process = Create_Timer_Task(1, 50, Data_Process_Task);  // 创建并初始化数据处理任务，500ms执行一次
    Heart_Detection = Create_Timer_Task(0, 200, Heart_Detection_Task);  // 创建并初始化心率检测任务，2s执行一次
    
    // 根据设备类型创建并初始化脉冲检测任务，设备类型为3时，3s执行一次，否则5s执行一次
    if (Pulse_Para.Device_Code.type == 3)
    {Pulse_Detection = Create_Timer_Task(0, 400, Pulse_Detection_Task);}
    else
    {Pulse_Detection = Create_Timer_Task(0, 200, Pulse_Detection_Task);}
    
    Sport_Starting();  // 启动运动状态
}

// 定时器0中断处理函数，用于处理定时器任务的计时和执行
static void Timer0_Int_Handler(unsigned char ucChannel)
{
    // 处理运行时间任务
    Timer_Tick(Running_Time);
    // 处理数据处理任务
    Timer_Tick(Data_Process);
    // 处理脉冲检测任务
    Timer_Tick(Pulse_Detection);
    // 处理心率检测任务
    Timer_Tick(Heart_Detection);
}

// 定时器初始化
void Timer_Init(void)
{
    // 定时任务列表初始化
    TimerList_Init();
    
	//ch通道0，div时钟分频0~15，restart为是否重新开始标志位，0表示不重新开始，timer总定时器单位1us,子定时器单位10ms，最后一个是中断函数
	timer0_init(0, 0, 1, 10000, Timer0_Int_Handler);			
}





