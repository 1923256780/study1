#ifndef __PULSE_FRAME_
#define __PULSE_FRAME_

#include <stdint.h>
#include "device.h"
#include "gpio.h"

#define SPEED_BUF_SIZE 5
#define HEART_BUF_SIZE 6

/** ����ģʽ���� **/

// ����ģʽ  
typedef enum  
{  
    Uart_Mode,    // ����ģʽ  
    Pulse_Mode    // ����ģʽ  
} Work_Mode_e;  

// �ⲿ������ŵ�Ĭ�ϵ�ƽ״̬  
typedef enum  
{  
    Low_Level,   // �͵�ƽ  
    High_Level   // �ߵ�ƽ  
} Default_Level_e;  

// ������������
typedef struct  
{  
    uint8_t pin_number;  // ���ű��  
    Pull_Type Pin_Pull_Type;  // ��������������  
    int_mode Int_Mode;  // �������ģʽ��Ĭ�ϵ�ƽ
} Pulse_Pin_Config_t;  
  
// �ⲿ�����������
typedef struct  
{  
    uint8_t pin_number;  // ���ű��
    Default_Level_e Default_Level;  // �������ģʽ��Ĭ�ϵ�ƽ  
} Output_Pin_Config_t;  

// �������ģʽ  
typedef enum  
{  
    Single_Pulse_Mode,       // ����ģʽ  
    Double_Pulse_Mode,       // ˫��ģʽ  
    Long_Short_Wave_Mode     // ���̲�ģʽ  
} Pulse_Count_Mode_e;

// �������� 
typedef struct  
{  
    uint16_t count;  // ����
    uint16_t pulse_per_count;   // ���������һ�Σ���λ0.1������Ϊ10���Ǿ�һ�������һ��
    uint16_t distance_per_count; // һ�ζ��پ��룬��λ1mm
} Count_Para_t;

// ��·����㷽ʽ
typedef enum  
{  
    Base_On_Distance,      // ���ھ���  
    Base_On_Count          // ���ڼ���  
} Calories_Calculation_Method_e;  
 
// ��·�����ϵ���ṹ��
typedef struct  
{  
    uint16_t calories_distance_rate;   // ���ھ���Ŀ�·�����ϵ��
    uint16_t calories_count_rate;      // ���ڴ����Ŀ�·�����ϵ��
} Calories_Calculation_Para_t;  

// �豸����ṹ��
typedef struct  
{  
    uint8_t 	type;     // �豸����
    uint16_t 	factory;  // ��������
	uint16_t 	model;    // �豸�ͺ�
} Device_Code_t;

// ʱ���������ṹ��
typedef struct  
{  
	uint32_t cur_pulse_time_stamp;  // ��ǰ����ʱ���
	uint32_t pre_pulse_time_stamp;  // ��һ������ʱ���
    uint16_t interval;              // ʱ����
    uint16_t max_Interval;          // ���������
    uint16_t min_Interval;          // ��С������
} Interval_Para_t;

// ˫�в���״̬ö��
typedef enum 
{
    IDLE,               // ����״̬
    First_Pin_Pulse_1,  // ��һ����������1
    Second_Pin_Pulse_1, // �ڶ�����������1
    Second_Pin_Pulse_2, // �ڶ�����������2
    First_Pin_Pulse_2   // ��һ����������2
} Double_Pulse_Mode_State_e;

// ˫�в����ṹ��
typedef struct
{
    Double_Pulse_Mode_State_e Double_Pulse_Mode_State;  // ˫����ģʽ״̬
    uint8_t first_pin;     // ��һ������
    uint8_t second_pin;    // �ڶ�������
} Dounble_Mode_Para_t;

// ��Ծ��־ö��
typedef enum  
{  
    Active,     // ��Ծ״̬
    In_Active   // �ǻ�Ծ״̬
} Active_Flag_e;  

// ��־�ṹ��
typedef struct  
{  
    Active_Flag_e Speed;   // �ٶȻ�Ծ��־
    Active_Flag_e Heart;   // ���ʻ�Ծ��־
} Flag_t;

// ϵͳ�����������
typedef struct  
{  
    Work_Mode_e 					Work_Mode;  // ����ģʽ  
    Pulse_Pin_Config_t 				Speed_Pin_A;  // ��������1
    Pulse_Pin_Config_t 				Speed_Pin_B;  // ��������2             
    Pulse_Pin_Config_t              Heart_Pin;  // ��������
    Output_Pin_Config_t             Output_Pin_A;  //�ⲿ�������1
    Output_Pin_Config_t             Output_Pin_B;  //�ⲿ�������2
    UnitDimension_e                 Unit;  //��Ӣ�Ƶ�λ
    Pulse_Count_Mode_e 				Count_Mode;  // �������ģʽ  
	Count_Para_t 					Count_Para;  // ��������
    Calories_Calculation_Method_e   Calories_Calculation_Method;  // ��·����㷽ʽ
    Calories_Calculation_Para_t 	Calories_Calculation_Para;  // ��·�����ϵ��
    Device_Code_t                   Device_Code;  //�豸����
} Pulse_Para_Config_t;

// ��Ӣ�ơ��豸���͡������롢���������������ط���������ʼ����˳�㸳ֵ��ȥ

// ϵͳ�����������
typedef struct  
{ 
    Interval_Para_t 			    Speed_Interval;  // �ٶ�����������
    Interval_Para_t 			    Heart_Interval;  // ��������������
    Dounble_Mode_Para_t             Dounble_Mode_Para;  // ˫��ģʽ����
    Flag_t                          Flag;  // ״̬λ
}Pulse_Sport_Para_t;

/** ȫ�ֱ������� **/
extern Pulse_Para_Config_t Pulse_Para;  // ����������ýṹ��
extern Pulse_Sport_Para_t Sport_Para;  // �˶������ṹ��

/** �������� **/
void Pulse_frame_Init(void);  // ��ʼ��������
void Pulse_Data_Cal(void);  // �������ݼ���
void Heart_Data_Cal(void);  // �������ݼ���

void Sport_Starting(void);  // �˶���ʼ
void Sport_Stoping(void);  // �˶�����


#endif
