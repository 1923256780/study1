#include "pulse_frame.h"
#include "timer_task_frame.h"
#include "icu.h"
#include "uart.h"
#include "device.h"
#include "nvds.h"
#include "app.h"
#include <string.h>

/** ȫ�ֱ������� **/
Pulse_Para_Config_t Pulse_Para;
Pulse_Sport_Para_t Sport_Para;

extern FitShowAllData_t xFitShowAllData;
static void GPIO_InterruptCallBack(void);

/** �������� **/

// ����жϱ�־��ʹ��
void GPIO_INT_ENABLE(void)
{
    // ���GPIO�жϱ�־λ
    REG_GPIO_WUATOD_STATUS = 0xffffffff;
    // ʹ��GPIO�ж�
    SYS_REG0X10_INT_EN |= (0x01 << POS_SYS_REG0X10_INT_EN_GPIO);
}

// ��ȡGPIOָ�����ŵ��жϱ�־λ
uint32_t GetGPIO_Interrupt_Flag(uint8_t Pin)
{
    return REG_GPIO_WUATOD_STATUS & (0x01 << Pin);
}

// ����ģʽ���ú���
static void PulseModeConfig(void)
{
    // ��������ģʽ����
    
    // ���ù���ģʽΪ����ģʽ
    Pulse_Para.Work_Mode = Pulse_Mode;
    
    // ���ü���ģʽΪ������ģʽ
    Pulse_Para.Count_Mode = Single_Pulse_Mode;  // Single_Pulse_Mode, Double_Pulse_Mode
    
    // �����ٶȴ��������Ų���
    Pulse_Para.Speed_Pin_A.pin_number = 0x02;
    Pulse_Para.Speed_Pin_B.pin_number = 0x03;
    Pulse_Para.Speed_Pin_A.Pin_Pull_Type = PULL_LOW;
    Pulse_Para.Speed_Pin_A.Int_Mode = RISE_EDGE;
    Sport_Para.Speed_Interval.max_Interval = 65535;
    Sport_Para.Speed_Interval.min_Interval = 50;
    
    // �������ʴ��������Ų���
    Pulse_Para.Heart_Pin.pin_number = 0x04;
    Pulse_Para.Heart_Pin.Pin_Pull_Type = PULL_LOW;
    Pulse_Para.Heart_Pin.Int_Mode = RISE_EDGE;
    Sport_Para.Heart_Interval.max_Interval = 4000;
    Sport_Para.Heart_Interval.min_Interval = 50;
    
    // �����������A��B����
    Pulse_Para.Output_Pin_A.pin_number = 0x11;
    Pulse_Para.Output_Pin_A.Default_Level = Low_Level;
    Pulse_Para.Output_Pin_B.pin_number = 0x12;
    Pulse_Para.Output_Pin_B.Default_Level = High_Level;
    
    // ���õ�λΪ����
    Pulse_Para.Unit = Metric; // Metric, Imperial  
    
    // ���ü�������
    Pulse_Para.Count_Para.pulse_per_count = 10;  // ��λ0.1��������ֵ10 -> 1�������һ������
    Pulse_Para.Count_Para.distance_per_count = 8000;  // ��λ0.001�ף�������ֵ1000 -> 1��
    
    // ���ÿ�·����㷽������Ӧ�Ĳ���
    Pulse_Para.Calories_Calculation_Method = Base_On_Distance;  // ���ھ���ͻ��ڴ������㿨·� Base_On_Distance, Base_On_Count
    Pulse_Para.Calories_Calculation_Para.calories_distance_rate = 178;  // ��λ0.1kcal/km
    Pulse_Para.Calories_Calculation_Para.calories_count_rate = 1000;  //��λ0.1cal/cnt, ��1000 = 0.1kcal/cnt
    
    // �����豸����
    Pulse_Para.Device_Code.type = 3;
    Pulse_Para.Device_Code.factory = 888;
    Pulse_Para.Device_Code.model = 888;

    // ����ȫ���豸�������Ͳ���
    xFitShowAllData.xDeviceDataTypePara.Unit = Pulse_Para.Unit;
    xFitShowAllData.xDeviceDataTypePara.Model = Pulse_Para.Device_Code.model;
    xFitShowAllData.xDeviceDataTypePara.Factory = Pulse_Para.Device_Code.factory;
    xFitShowAllData.xDeviceDataTypePara.Type = Pulse_Para.Device_Code.type;
}

// ��ʼ������֡����
void Pulse_frame_Init(void)
{
    // ����˶���Ϣ��������������
    ClearSportInfoOnly();
    
    // ��������ģʽ����
    PulseModeConfig();
    
    // ��ʼ���������Ϊ0
    Pulse_Para.Count_Para.count = 0;
    
    // ��ʼ���ٶȺ�����ʱ���Ϊ0
    Sport_Para.Speed_Interval.cur_pulse_time_stamp = 0;
    Sport_Para.Heart_Interval.cur_pulse_time_stamp = 0;
    
    // �����ٶȴ�����A����Ϊ���룬��������/�������ͣ������ж�ģʽ
    gpio_config(Pulse_Para.Speed_Pin_A.pin_number, INPUT, Pulse_Para.Speed_Pin_A.Pin_Pull_Type);
    gpio_set_int_mode(Pulse_Para.Speed_Pin_A.pin_number, Pulse_Para.Speed_Pin_A.Int_Mode); 
    
    // �������ʴ���������Ϊ���룬��������/�������ͣ������ж�ģʽ
    gpio_config(Pulse_Para.Heart_Pin.pin_number, INPUT, Pulse_Para.Heart_Pin.Pin_Pull_Type);
    gpio_set_int_mode(Pulse_Para.Heart_Pin.pin_number, Pulse_Para.Heart_Pin.Int_Mode); 
    
    // ʹ��GPIO�жϲ�ע���жϻص�����
    GPIO_INT_ENABLE();
    gpio_cb_register(GPIO_InterruptCallBack);
    
    // �������ģʽ����˫����ģʽ���򷵻�
    if (Pulse_Para.Count_Mode != Double_Pulse_Mode) {
        return;
    }
    
    // �����ٶȴ�����B����Ϊ���룬��������/�������ͣ������ж�ģʽ
    gpio_config(Pulse_Para.Speed_Pin_B.pin_number, INPUT, Pulse_Para.Speed_Pin_A.Pin_Pull_Type);
    gpio_set_int_mode(Pulse_Para.Speed_Pin_B.pin_number, Pulse_Para.Speed_Pin_A.Int_Mode); 
    
    // ��ʼ��˫����ģʽ״̬ΪIDLE
    Sport_Para.Dounble_Mode_Para.Double_Pulse_Mode_State = IDLE;
}

// ʱ�������㺯��
// ������ֵΪ0����ʾ�����������ϣ����˵�
// ������ֵΪ1����ʾ����������
uint8_t Pulse_Interval_Cal(Interval_Para_t* Interval_Para)
{
    static uint32_t time_diff;
    
    // ������һ������ʱ���Ϊ��ǰʱ���
    Interval_Para->pre_pulse_time_stamp = Interval_Para->cur_pulse_time_stamp;
    // ��ȡ��ǰϵͳʱ��
    Interval_Para->cur_pulse_time_stamp = system_time;
    // ����ʱ���
    time_diff = system_time - Interval_Para->pre_pulse_time_stamp;

    // ���ʱ������С�����ʱ������Χ�ڣ��򷵻�0
    if (time_diff * 10 < Interval_Para->min_Interval || time_diff * 10 > Interval_Para->max_Interval) {
        return 0;
    }
    
    // ����ʱ����Ϊʱ���
    Interval_Para->interval = (uint16_t)(time_diff);
    return 1;
}

// ��ʱ����ת��ΪRPM
uint16_t Interval_To_RPM(uint16_t interval)
{
    // ���ʱ����Ϊ0���򷵻�0���������RPMֵ
    if (interval == 0) {return 0;}
    // ���ʱ������Ϊ0������������RPMֵ
    else {return ((60000 / interval) + 5) / 10;}  // +5�ٳ�10Ŀ��Ϊ��������
}

// ��RPMת��Ϊ�ٶ�
uint16_t RPM_To_Speed(uint16_t RPM, uint16_t distance_per_count)
{
    // �����ٶ�ֵ
    return (uint16_t)((uint32_t)RPM * distance_per_count * 6 / 1000);
}

// ��������Ԫ��֮�ͣ������������������λ���ݣ��޳�ĩβ����
uint16_t sum_cal(uint16_t *array, uint8_t len, uint16_t cur_value)
{
    uint16_t sum = cur_value;
    // �ƶ�����Ԫ�ز������ܺ�
    for (uint8_t i = 0; i < len - 1; i++)
    {
        array[i] = array[i + 1];
        sum += array[i];
    }
    array[len - 1] = cur_value;
    return sum;
}

// ��������Ԫ��ƽ��ֵ
uint16_t avg_cal(uint16_t *array, uint8_t len, uint16_t cur_value)
{
    // ��������Ԫ��֮�ͳ������鳤�ȵĽ��
    return (sum_cal(array, len, cur_value) / len);
}

//���ݲ���
void TestDeviceData(void)
{
    uart0_printf("����=%d, ʱ��=%d, ����=%d, ����=%d, ��·��=%d, Ƶ��=%d, �ٶ�=%d, ����=%d, ",xFitShowAllData.Heart, \
                        xFitShowAllData.Time, xFitShowAllData.Count,xFitShowAllData.Distance, \
                        xFitShowAllData.Calories, xFitShowAllData.Freq, xFitShowAllData.Speed, xFitShowAllData.Power);
}

// ������ģʽ���ݲɼ�����
static void Single_Pulse_Acquire(void)
{
    // ����������ʱ�䣬������ڷ�Χ���򷵻�
    if (!Pulse_Interval_Cal(&Sport_Para.Speed_Interval)) {return;}
    
    // ���Ӵ����������ٶ����ű�־Ϊ��Ծ״̬
    Pulse_Para.Count_Para.count++;
    Sport_Para.Flag.Speed = Active;
    
    // �������������ⶨʱ��
    Restart_Timer(Pulse_Detection);
}

// ˫����ģʽ�������ú���
static void DoubleModePinSet(void)
{
    // ����⵽��һ������ʱ������˫����ģʽ״̬Ϊһ�����ŵ�һ�������״̬
    Sport_Para.Dounble_Mode_Para.Double_Pulse_Mode_State = First_Pin_Pulse_1;
    
    // ����ٶȴ�����A�������жϱ�־�������õ�һ����ΪA���ڶ�����ΪB
    if (GetGPIO_Interrupt_Flag(Pulse_Para.Speed_Pin_A.pin_number))
    {
        Sport_Para.Dounble_Mode_Para.first_pin = Pulse_Para.Speed_Pin_A.pin_number;
        Sport_Para.Dounble_Mode_Para.second_pin = Pulse_Para.Speed_Pin_B.pin_number;
    }
    // ����ٶȴ�����B�������жϱ�־�������õ�һ����ΪB���ڶ�����ΪA
    else if (GetGPIO_Interrupt_Flag(Pulse_Para.Speed_Pin_B.pin_number))
    {
        Sport_Para.Dounble_Mode_Para.first_pin = Pulse_Para.Speed_Pin_B.pin_number;
        Sport_Para.Dounble_Mode_Para.second_pin = Pulse_Para.Speed_Pin_A.pin_number;
    }
}

/**  
 * ˫�в���ʾ��  ABBA
 *     ___          ___  
 *  __|   |________|   |__       A  
 *        ___     ___      
 *  _____|   |___|   |____       B
 */ 
// ˫����ģʽ���ݲɼ�����
static void Double_Pulse_Acquire(void)
{
    switch (Sport_Para.Dounble_Mode_Para.Double_Pulse_Mode_State)
    {
        // ����״̬
        case IDLE:
            DoubleModePinSet();
        break;
        
        // һ���ٶ����ŵ�һ������
        case First_Pin_Pulse_1:
            // �����⵽�����ٶ����Ŵ����жϣ������״̬����һ�׶�
            if (GetGPIO_Interrupt_Flag(Sport_Para.Dounble_Mode_Para.second_pin))
            {
                Sport_Para.Dounble_Mode_Para.Double_Pulse_Mode_State = Second_Pin_Pulse_1;
                break;
            }
            // ����������������
            DoubleModePinSet();
        break;
        
        // �����ٶ����ŵ�һ������
        case Second_Pin_Pulse_1:
            // �����⵽�����ٶ����Ŵ����жϣ������״̬����һ�׶�
            if (GetGPIO_Interrupt_Flag(Sport_Para.Dounble_Mode_Para.second_pin)) 
            {
                Sport_Para.Dounble_Mode_Para.Double_Pulse_Mode_State = Second_Pin_Pulse_2;
                break;
            }
            // ����������������
            DoubleModePinSet();
        break;
        
        // �����ٶ����ŵڶ�������
        case Second_Pin_Pulse_2:
            // �����⵽һ���ٶ����Ŵ����жϣ����ʾһ��˫���������
            if (GetGPIO_Interrupt_Flag(Sport_Para.Dounble_Mode_Para.first_pin))
            {
                // ��ʱһ��˫��������ɣ�����״̬λ
                Sport_Para.Dounble_Mode_Para.Double_Pulse_Mode_State = IDLE;
                
                // ����������ʱ�䣬������ڷ�Χ���򷵻�
                if (!Pulse_Interval_Cal(&Sport_Para.Speed_Interval)) {
                    return;
                }
                
                // ���Ӵ����������ٶ����ű�־Ϊ��Ծ״̬
                Pulse_Para.Count_Para.count++;
                Sport_Para.Flag.Speed = Active;
                
                // �������������ⶨʱ��
                Restart_Timer(Pulse_Detection);
            }
            // ����������������
            DoubleModePinSet();
        break;  
    }
}


// �������ݼ��㺯��
void Pulse_Data_Cal(void)
{
    // ��̬�������ڴ洢RPM����
    static uint16_t RPM_array[SPEED_BUF_SIZE] = {0};
    
    // ��ʱ��ת��Ϊһ��Ϊ��λ
    xFitShowAllData.Time = system_time / 100;
    
    // ����ٶ����ű�־Ϊδ����򷵻�
    if (Sport_Para.Flag.Speed == In_Active)
    {
        // ���Ƶ��Ϊ0���򷵻�
        if (xFitShowAllData.Freq == 0) {return;}
        
        // ����ƽ��Ƶ�ʡ��ٶȺ͹��ʣ�������
        xFitShowAllData.Freq = avg_cal(RPM_array, SPEED_BUF_SIZE, 0);
        xFitShowAllData.Speed = RPM_To_Speed(xFitShowAllData.Freq, Pulse_Para.Count_Para.distance_per_count);
        xFitShowAllData.Power = xFitShowAllData.Speed / 20;
        return;
    }
    
    // ���㵱ǰ�����Ƶ�ʡ�ƽ��Ƶ�ʡ��ٶȺ͹���
    xFitShowAllData.Freq = Interval_To_RPM(Sport_Para.Speed_Interval.interval);
    xFitShowAllData.Freq = avg_cal(RPM_array, SPEED_BUF_SIZE, xFitShowAllData.Freq);
    xFitShowAllData.Speed = RPM_To_Speed(xFitShowAllData.Freq, Pulse_Para.Count_Para.distance_per_count);
    xFitShowAllData.Power = xFitShowAllData.Speed / 20;
    
    // ���ÿ�������һ�Σ���ֱ��ʹ�������������ֵ���������ת��
    if (Pulse_Para.Count_Para.pulse_per_count == 10) {xFitShowAllData.Count = Pulse_Para.Count_Para.count;} 
    else {xFitShowAllData.Count = ((uint32_t)Pulse_Para.Count_Para.count * 100 / Pulse_Para.Count_Para.pulse_per_count + 5) / 10;}
    
    // �����˶�����
    xFitShowAllData.Distance = ((uint32_t)Pulse_Para.Count_Para.distance_per_count * Pulse_Para.Count_Para.count / 1000);
    
    // ���ݿ�·����㷽ʽ���㿨·��
    if (Pulse_Para.Calories_Calculation_Method == Base_On_Distance) 
    {xFitShowAllData.Calories = (uint16_t)(xFitShowAllData.Distance * Pulse_Para.Calories_Calculation_Para.calories_distance_rate / 1000);} 
    else 
    {xFitShowAllData.Calories = (uint16_t)((uint32_t)Pulse_Para.Count_Para.count * Pulse_Para.Calories_Calculation_Para.calories_count_rate / 1000);}

    // ����豸����Ϊ10���ܲ����������ٶ������10
    if (Pulse_Para.Device_Code.type == 10) {xFitShowAllData.Speed /= 10;}
    
    // �����λΪӢ�ƣ����ٶȺ;������ת��
    if (Pulse_Para.Unit == Imperial)
    {
        xFitShowAllData.Speed = (uint16_t)((float)xFitShowAllData.Speed * 0.6214 + 0.5);
        xFitShowAllData.Distance = (uint16_t)((float)xFitShowAllData.Distance * 0.6214 + 0.5);
    }
    
    // ������볬��32000���豸���Ͳ�Ϊ10������д���
    if (xFitShowAllData.Distance >= 32000 && Pulse_Para.Device_Code.type != 10) 
    {
        xFitShowAllData.Distance = ((xFitShowAllData.Distance + 5) / 10) | 0x8000;  // +5��Ϊ���������룬ʹ���ݱ仯��ƽ��
    }
}

// ���ʲɼ�����
void Heart_Rate_Acquire(void)
{
    // ������ʼ��ʱ�䲻�ڷ�Χ�ڣ��򷵻�
    if (!Pulse_Interval_Cal(&Sport_Para.Heart_Interval)) {return;}
    
    // �����������ű�־Ϊ��������������ʼ�ⶨʱ��
    Sport_Para.Flag.Heart = Active;
    Restart_Timer(Heart_Detection);
}

// ���ʼ��㺯��
void Heart_Data_Cal(void)
{
    // ��̬�������ڴ洢��������
    static uint16_t heart_array[HEART_BUF_SIZE] = {0};
    
    // ������ʱ�־Ϊδ������������鲢����
    if (Sport_Para.Flag.Heart == In_Active)
    {
        memset(heart_array, 0, HEART_BUF_SIZE);
        xFitShowAllData.Heart = 0;
        return;
    }
    
    // ���㵱ǰ����
	uint16_t heart_temp = Interval_To_RPM(Sport_Para.Heart_Interval.interval);
    
    // ������ʲ�����Ч��Χ�ڣ��򷵻�
    if (heart_temp < 50 || heart_temp > 180) {return;}
    
    // ����ƽ������
    heart_temp = avg_cal(heart_array, HEART_BUF_SIZE, heart_temp);
    
    // ������ʲ�����Ч��Χ�ڣ��򷵻�
    if (heart_temp < 50 || heart_temp > 180) {return;}
    
    // ��ƽ�����ʱ��浽�豸������
    xFitShowAllData.Heart = heart_temp;
}

// GPIO�жϻص�����
static void GPIO_InterruptCallBack(void)
{
    // ����������Ŵ������ж�
    if (GetGPIO_Interrupt_Flag(Pulse_Para.Heart_Pin.pin_number))
    {
        // �������ʲɼ�����������ʹ��GPIO�жϣ�Ȼ�󷵻�
        Heart_Rate_Acquire();
        GPIO_INT_ENABLE();
        return;
    }
    
    // �����������ģʽ���в�ͬ����
    if (Pulse_Para.Count_Mode == Single_Pulse_Mode)
    {
        // ����ǵ�����ģʽ�����õ�����ɼ�����
        Single_Pulse_Acquire();
    }
    else if (Pulse_Para.Count_Mode == Double_Pulse_Mode)
    {
        // �����˫����ģʽ������˫����ɼ�����
        Double_Pulse_Acquire();
    }
    else if (Pulse_Para.Count_Mode == Long_Short_Wave_Mode)
    {
        // ����ǳ��̲�ģʽ����
    }
    
    // ����ʹ��GPIO�ж�
    GPIO_INT_ENABLE();
}

// APP���Ӳ���
void Sport_Starting(void)
{
    // ��������ʱ�䶨ʱ�������ݴ���ʱ��
    Start_Timer(Running_Time);
    Start_Timer(Data_Process);
}

// APP�Ͽ����Ӳ���
void Sport_Stoping(void)
{
    // ֹͣ����ʱ�䶨ʱ�������ݴ���ʱ����������˶���Ϣ
    Stop_Timer(Running_Time);
    Stop_Timer(Data_Process);
    ClearSportInfoOnly();
}


// ����
void nvds_test1(void)
{
    uint8_t ret = 0;
    uint8_t len = sizeof(Pulse_Para_Config_t);

    Pulse_Para_Config_t Pulse_Para_Config_Test;
    Pulse_Para_Config_t Pulse_Para_Config_Test2;
    
    Pulse_Para_Config_Test.Device_Code.factory = 666;
    Pulse_Para_Config_Test.Device_Code.model = 666;
    
    // �ж�flash�Ƿ������úõĲ���������о������ģ�û�еĻ���������ʼ��
//    ret = nvds_get(NVDE_TAG_PULSE_CONFIG, &len, (uint8_t *)&Pulse_Para_Config_Test2);
//    uart0_printf("ret = %d \r\n", ret);
    
    // �������ò���
    ret = nvds_put(NVDE_TAG_PULSE_CONFIG, len, (uint8_t *)&Pulse_Para_Config_Test);
    uart0_printf("ret = %d \r\n", ret);
    
    // ��ȡ���ò����������µ���Ӧ�����ݽṹ
    ret = nvds_get(NVDE_TAG_PULSE_CONFIG, &len, (uint8_t *)&Pulse_Para_Config_Test2);

    Pulse_Para.Device_Code.factory = Pulse_Para_Config_Test2.Device_Code.factory;
    Pulse_Para.Device_Code.model = Pulse_Para_Config_Test2.Device_Code.model;
    
    uart0_printf("Test2.factory = %d \r\n", Pulse_Para_Config_Test2.Device_Code.factory);
    uart0_printf("Test2.model = %d \r\n", Pulse_Para_Config_Test2.Device_Code.model);
    
    uart0_printf("Device_Code.factory = %d \r\n", Pulse_Para.Device_Code.factory);
    uart0_printf("Device_Code.model = %d \r\n", Pulse_Para.Device_Code.model);
}

