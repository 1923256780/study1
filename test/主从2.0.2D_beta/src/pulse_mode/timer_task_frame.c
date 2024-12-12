#include "timer_task_frame.h"
#include "pulse_frame.h"
#include "timer0.h"
#include "uart.h"
#include "gpio.h"
#include "icu.h"
#include <stddef.h>
#include "device.h"

/** ȫ�ֱ������� **/
Timer_Task_t *Running_Time, *Data_Process, *Pulse_Detection, *Heart_Detection;  // ������ʱ������ṹ��

/** �������� **/

// ������ʱ�����񣬷���ָ��������ָ��
Timer_Task_t* Create_Timer_Task(uint8_t is_looping, uint32_t duration, Timer_CallBack Callback)
{
    // Ϊ��ʱ����������ڴ�ռ�
    Timer_Task_t* Timer_Task = (Timer_Task_t*) ke_malloc(sizeof(Timer_Task_t), KE_MEM_NON_RETENTION);
    
    // ����ڴ����ʧ�ܣ����ؿ�ָ��
    if (Timer_Task == NULL) {return NULL;}
    
    // ��ʼ����ʱ�����������
    Timer_Task->is_looping = is_looping;   // �Ƿ�ѭ��ִ��
    Timer_Task->duration = duration;       // ��ʱ�����
    Timer_Task->current_timer_count = 0;   // ��ǰ����
    Timer_Task->Callback = Callback;       // ��ʱ���ص�����
    Timer_Task->TimerState = Timer_Idle;   // ��ʱ��״̬Ϊ����

    return Timer_Task;  // ���ض�ʱ������ָ��
}

// ������ʱ��
void Start_Timer(Timer_Task_t* Timer_Task) 
{
    // �����ʱ������Ϊ�գ�ֱ�ӷ���
    if (Timer_Task == NULL) {return;}
    
    // ���ö�ʱ��״̬Ϊ����
    Timer_Task->TimerState = Timer_Running;
}

// ֹͣ��ʱ��
void Stop_Timer(Timer_Task_t* Timer_Task) 
{
    // �����ʱ������Ϊ�գ�ֱ�ӷ���
    if (Timer_Task == NULL) {return;}
    
    // ����ǰ��������Ϊ0��������ʱ��״̬����Ϊ����
    Timer_Task->current_timer_count = 0;
    Timer_Task->TimerState = Timer_Idle;
}

// ������ʱ��
void Restart_Timer(Timer_Task_t* Timer_Task) 
{
    // �����ʱ������Ϊ�գ�ֱ�ӷ���
    if (Timer_Task == NULL) {return;}

    // ����ǰ��������Ϊ0��������ʱ��״̬����Ϊ����
    Timer_Task->current_timer_count = 0;
    Timer_Task->TimerState = Timer_Running;
}

// ���ٶ�ʱ������
void Destroy_Timer_Task(Timer_Task_t* Timer_Task) 
{
    // �����ʱ������Ϊ�գ��ͷ��ڴ�ռ�
    if (Timer_Task != NULL) {ke_free(Timer_Task);}
}

// ����ʱ�����������ִ�лص�����
void Timer_Tick(Timer_Task_t* Timer_Task) 
{
    // �����ʱ������Ϊ�ջ��߶�ʱ��״̬��������״̬��ֱ�ӷ���
    if (Timer_Task == NULL || Timer_Task->TimerState != Timer_Running) {return;}

    // ��ʱ��������1
    Timer_Task->current_timer_count++;
    
    // �����ʱ������δ�ﵽ��ʱ�������ֱ�ӷ���
    if (Timer_Task->current_timer_count < Timer_Task->duration) {return;}

    // ִ�ж�ʱ���ص�������������ǰ��������Ϊ0
    Timer_Task->Callback();
    Timer_Task->current_timer_count = 0;
    
    // �����ʱ��Ϊѭ��ģʽ����ֱ�ӷ���
    if (Timer_Task->is_looping != 0) {return;}
    
    // �����ʱ������ѭ��ģʽ���򽫶�ʱ��״̬����Ϊ����
    Timer_Task->TimerState = Timer_Idle;
}

// ϵͳʱ�䣬���ڼ�¼ϵͳ����ʱ�䣬��λΪ10ms
uint32_t system_time = 0;

// ����ʱ����������ÿ10ms����ϵͳ����ʱ��
void Running_Time_Task(void) 
{
    system_time ++; // ϵͳʱ�����
}

// ���ݴ����������ڴ������ʺ���������
void Data_Process_Task(void) 
{
    Heart_Data_Cal();  // ������������
    Pulse_Data_Cal();  // ������������
}

// �������������ڼ�������Ƿ��Ծ
void Pulse_Detection_Task(void)
{
    Sport_Para.Flag.Speed = In_Active;  // �������Ծ��־����Ϊ����Ծ
}

// ���ʼ���������ڼ�������Ƿ��Ծ
void Heart_Detection_Task(void)
{
    Sport_Para.Flag.Heart = In_Active;  // �����ʻ�Ծ��־����Ϊ����Ծ
}

// ��ʱ�������б��ʼ�����������ڴ�������ʼ��������ʱ������
static void TimerList_Init(void) 
{
    Running_Time = Create_Timer_Task(1, 1, Running_Time_Task);  // ��������ʼ������ʱ������10msִ��һ��
    Data_Process = Create_Timer_Task(1, 50, Data_Process_Task);  // ��������ʼ�����ݴ�������500msִ��һ��
    Heart_Detection = Create_Timer_Task(0, 200, Heart_Detection_Task);  // ��������ʼ�����ʼ������2sִ��һ��
    
    // �����豸���ʹ�������ʼ�������������豸����Ϊ3ʱ��3sִ��һ�Σ�����5sִ��һ��
    if (Pulse_Para.Device_Code.type == 3)
    {Pulse_Detection = Create_Timer_Task(0, 400, Pulse_Detection_Task);}
    else
    {Pulse_Detection = Create_Timer_Task(0, 200, Pulse_Detection_Task);}
    
    Sport_Starting();  // �����˶�״̬
}

// ��ʱ��0�жϴ����������ڴ���ʱ������ļ�ʱ��ִ��
static void Timer0_Int_Handler(unsigned char ucChannel)
{
    // ��������ʱ������
    Timer_Tick(Running_Time);
    // �������ݴ�������
    Timer_Tick(Data_Process);
    // ��������������
    Timer_Tick(Pulse_Detection);
    // �������ʼ������
    Timer_Tick(Heart_Detection);
}

// ��ʱ����ʼ��
void Timer_Init(void)
{
    // ��ʱ�����б��ʼ��
    TimerList_Init();
    
	//chͨ��0��divʱ�ӷ�Ƶ0~15��restartΪ�Ƿ����¿�ʼ��־λ��0��ʾ�����¿�ʼ��timer�ܶ�ʱ����λ1us,�Ӷ�ʱ����λ10ms�����һ�����жϺ���
	timer0_init(0, 0, 1, 10000, Timer0_Int_Handler);			
}





