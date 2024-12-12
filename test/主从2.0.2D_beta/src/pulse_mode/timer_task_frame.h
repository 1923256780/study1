#ifndef __TIMER_TEST_
#define __TIMER_TEST_

#include <stdint.h>
#include "ke_mem.h"

/** ������������ **/

// �ص���������
typedef void (*Timer_CallBack)(void);

// ��ʱ��������״̬
typedef enum
{
    Timer_Idle,     // ��ʱ������״̬
    Timer_Running,  // ��ʱ������״̬
} TimerState_e;

// ��ʱ���Ĳ���
typedef struct 
{
    uint8_t is_looping;           // �Ƿ�ѭ��ִ�ж�ʱ������1��ʾѭ��ִ�У�0��ʾִ��һ��
    TimerState_e TimerState;      // ��ʱ���ĵ�ǰ״̬�������ǿ���״̬������״̬
    uint32_t duration;            // ��ʱ��ִ�е�ʱ��������λΪ10ms
    uint32_t current_timer_count; // ��ʱ����ǰ�Ѿ���ʱ��ʱ�䣬��λΪ10ms
    Timer_CallBack Callback;      // ��ʱ���ص�����������ִ�ж�ʱ������
} Timer_Task_t;

/** ȫ�ֱ������� **/
extern uint32_t system_time;  // ϵͳ����ʱ��
extern Timer_Task_t *Running_Time, *Data_Process, *Pulse_Detection, *Heart_Detection;  // ������ʱ������ṹ��

/** �������� **/
Timer_Task_t* Create_Timer_Task(uint8_t is_looping, uint32_t duration, Timer_CallBack Callback);  // ������ʱ����
void Start_Timer(Timer_Task_t* Timer_Task);  // ������ʱ������
void Stop_Timer(Timer_Task_t* Timer_Task);  // ֹͣ��ʱ������
void Restart_Timer(Timer_Task_t* Timer_Task);  // ����������ʱ������
void Destroy_Timer_Task(Timer_Task_t* Timer_Task);  // ���ٶ�ʱ������

void Timer_Init(void);  // ��ʱ����ʼ��

#endif
