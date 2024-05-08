#ifndef __TIMER_TEST_
#define __TIMER_TEST_

#include <stdint.h>
#include "ke_mem.h"

/** 变量类型声明 **/

// 回调函数类型
typedef void (*Timer_CallBack)(void);

// 定时器的运行状态
typedef enum
{
    Timer_Idle,     // 定时器空闲状态
    Timer_Running,  // 定时器运行状态
} TimerState_e;

// 定时器的参数
typedef struct 
{
    uint8_t is_looping;           // 是否循环执行定时器任务，1表示循环执行，0表示执行一次
    TimerState_e TimerState;      // 定时器的当前状态，可以是空闲状态或运行状态
    uint32_t duration;            // 定时器执行的时间间隔，单位为10ms
    uint32_t current_timer_count; // 定时器当前已经计时的时间，单位为10ms
    Timer_CallBack Callback;      // 定时器回调函数，用于执行定时器任务
} Timer_Task_t;

/** 全局变量声明 **/
extern uint32_t system_time;  // 系统运行时间
extern Timer_Task_t *Running_Time, *Data_Process, *Pulse_Detection, *Heart_Detection;  // 声明定时器任务结构体

/** 函数声明 **/
Timer_Task_t* Create_Timer_Task(uint8_t is_looping, uint32_t duration, Timer_CallBack Callback);  // 创建定时任务
void Start_Timer(Timer_Task_t* Timer_Task);  // 启动定时器任务
void Stop_Timer(Timer_Task_t* Timer_Task);  // 停止定时器任务
void Restart_Timer(Timer_Task_t* Timer_Task);  // 重新启动定时器任务
void Destroy_Timer_Task(Timer_Task_t* Timer_Task);  // 销毁定时器任务

void Timer_Init(void);  // 定时器初始化

#endif
