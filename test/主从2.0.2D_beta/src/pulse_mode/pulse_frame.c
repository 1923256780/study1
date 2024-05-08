#include "pulse_frame.h"
#include "timer_task_frame.h"
#include "icu.h"
#include "uart.h"
#include "device.h"
#include "nvds.h"
#include "app.h"
#include <string.h>

/** 全局变量声明 **/
Pulse_Para_Config_t Pulse_Para;
Pulse_Sport_Para_t Sport_Para;

extern FitShowAllData_t xFitShowAllData;
static void GPIO_InterruptCallBack(void);

/** 函数声明 **/

// 清除中断标志及使能
void GPIO_INT_ENABLE(void)
{
    // 清除GPIO中断标志位
    REG_GPIO_WUATOD_STATUS = 0xffffffff;
    // 使能GPIO中断
    SYS_REG0X10_INT_EN |= (0x01 << POS_SYS_REG0X10_INT_EN_GPIO);
}

// 获取GPIO指定引脚的中断标志位
uint32_t GetGPIO_Interrupt_Flag(uint8_t Pin)
{
    return REG_GPIO_WUATOD_STATUS & (0x01 << Pin);
}

// 脉冲模式配置函数
static void PulseModeConfig(void)
{
    // 配置脉冲模式参数
    
    // 设置工作模式为脉冲模式
    Pulse_Para.Work_Mode = Pulse_Mode;
    
    // 设置计数模式为单脉冲模式
    Pulse_Para.Count_Mode = Single_Pulse_Mode;  // Single_Pulse_Mode, Double_Pulse_Mode
    
    // 配置速度传感器引脚参数
    Pulse_Para.Speed_Pin_A.pin_number = 0x02;
    Pulse_Para.Speed_Pin_B.pin_number = 0x03;
    Pulse_Para.Speed_Pin_A.Pin_Pull_Type = PULL_LOW;
    Pulse_Para.Speed_Pin_A.Int_Mode = RISE_EDGE;
    Sport_Para.Speed_Interval.max_Interval = 65535;
    Sport_Para.Speed_Interval.min_Interval = 50;
    
    // 配置心率传感器引脚参数
    Pulse_Para.Heart_Pin.pin_number = 0x04;
    Pulse_Para.Heart_Pin.Pin_Pull_Type = PULL_LOW;
    Pulse_Para.Heart_Pin.Int_Mode = RISE_EDGE;
    Sport_Para.Heart_Interval.max_Interval = 4000;
    Sport_Para.Heart_Interval.min_Interval = 50;
    
    // 配置输出引脚A和B参数
    Pulse_Para.Output_Pin_A.pin_number = 0x11;
    Pulse_Para.Output_Pin_A.Default_Level = Low_Level;
    Pulse_Para.Output_Pin_B.pin_number = 0x12;
    Pulse_Para.Output_Pin_B.Default_Level = High_Level;
    
    // 设置单位为公制
    Pulse_Para.Unit = Metric; // Metric, Imperial  
    
    // 设置计数参数
    Pulse_Para.Count_Para.pulse_per_count = 10;  // 单位0.1，例：数值10 -> 1次脉冲计一个次数
    Pulse_Para.Count_Para.distance_per_count = 8000;  // 单位0.001米，例：数值1000 -> 1米
    
    // 设置卡路里计算方法及对应的参数
    Pulse_Para.Calories_Calculation_Method = Base_On_Distance;  // 基于距离和基于次数计算卡路里。 Base_On_Distance, Base_On_Count
    Pulse_Para.Calories_Calculation_Para.calories_distance_rate = 178;  // 单位0.1kcal/km
    Pulse_Para.Calories_Calculation_Para.calories_count_rate = 1000;  //单位0.1cal/cnt, 即1000 = 0.1kcal/cnt
    
    // 设置设备代码
    Pulse_Para.Device_Code.type = 3;
    Pulse_Para.Device_Code.factory = 888;
    Pulse_Para.Device_Code.model = 888;

    // 更新全局设备数据类型参数
    xFitShowAllData.xDeviceDataTypePara.Unit = Pulse_Para.Unit;
    xFitShowAllData.xDeviceDataTypePara.Model = Pulse_Para.Device_Code.model;
    xFitShowAllData.xDeviceDataTypePara.Factory = Pulse_Para.Device_Code.factory;
    xFitShowAllData.xDeviceDataTypePara.Type = Pulse_Para.Device_Code.type;
}

// 初始化脉冲帧处理
void Pulse_frame_Init(void)
{
    // 清除运动信息，保留其它数据
    ClearSportInfoOnly();
    
    // 配置脉冲模式参数
    PulseModeConfig();
    
    // 初始化脉冲计数为0
    Pulse_Para.Count_Para.count = 0;
    
    // 初始化速度和心率时间戳为0
    Sport_Para.Speed_Interval.cur_pulse_time_stamp = 0;
    Sport_Para.Heart_Interval.cur_pulse_time_stamp = 0;
    
    // 配置速度传感器A引脚为输入，设置上拉/下拉类型，设置中断模式
    gpio_config(Pulse_Para.Speed_Pin_A.pin_number, INPUT, Pulse_Para.Speed_Pin_A.Pin_Pull_Type);
    gpio_set_int_mode(Pulse_Para.Speed_Pin_A.pin_number, Pulse_Para.Speed_Pin_A.Int_Mode); 
    
    // 配置心率传感器引脚为输入，设置上拉/下拉类型，设置中断模式
    gpio_config(Pulse_Para.Heart_Pin.pin_number, INPUT, Pulse_Para.Heart_Pin.Pin_Pull_Type);
    gpio_set_int_mode(Pulse_Para.Heart_Pin.pin_number, Pulse_Para.Heart_Pin.Int_Mode); 
    
    // 使能GPIO中断并注册中断回调函数
    GPIO_INT_ENABLE();
    gpio_cb_register(GPIO_InterruptCallBack);
    
    // 如果计数模式不是双脉冲模式，则返回
    if (Pulse_Para.Count_Mode != Double_Pulse_Mode) {
        return;
    }
    
    // 配置速度传感器B引脚为输入，设置上拉/下拉类型，设置中断模式
    gpio_config(Pulse_Para.Speed_Pin_B.pin_number, INPUT, Pulse_Para.Speed_Pin_A.Pin_Pull_Type);
    gpio_set_int_mode(Pulse_Para.Speed_Pin_B.pin_number, Pulse_Para.Speed_Pin_A.Int_Mode); 
    
    // 初始化双脉冲模式状态为IDLE
    Sport_Para.Dounble_Mode_Para.Double_Pulse_Mode_State = IDLE;
}

// 时间间隔计算函数
// 若返回值为0，表示脉冲间隔不符合，过滤掉
// 若返回值为1，表示脉冲间隔正常
uint8_t Pulse_Interval_Cal(Interval_Para_t* Interval_Para)
{
    static uint32_t time_diff;
    
    // 更新上一次脉冲时间戳为当前时间戳
    Interval_Para->pre_pulse_time_stamp = Interval_Para->cur_pulse_time_stamp;
    // 获取当前系统时间
    Interval_Para->cur_pulse_time_stamp = system_time;
    // 计算时间差
    time_diff = system_time - Interval_Para->pre_pulse_time_stamp;

    // 如果时间差不在最小和最大时间间隔范围内，则返回0
    if (time_diff * 10 < Interval_Para->min_Interval || time_diff * 10 > Interval_Para->max_Interval) {
        return 0;
    }
    
    // 更新时间间隔为时间差
    Interval_Para->interval = (uint16_t)(time_diff);
    return 1;
}

// 将时间间隔转换为RPM
uint16_t Interval_To_RPM(uint16_t interval)
{
    // 如果时间间隔为0，则返回0。否则计算RPM值
    if (interval == 0) {return 0;}
    // 如果时间间隔不为0，则正常计算RPM值
    else {return ((60000 / interval) + 5) / 10;}  // +5再除10目的为四舍五入
}

// 将RPM转换为速度
uint16_t RPM_To_Speed(uint16_t RPM, uint16_t distance_per_count)
{
    // 计算速度值
    return (uint16_t)((uint32_t)RPM * distance_per_count * 6 / 1000);
}

// 计算数组元素之和，并将输入参数放于首位数据，剔除末尾数据
uint16_t sum_cal(uint16_t *array, uint8_t len, uint16_t cur_value)
{
    uint16_t sum = cur_value;
    // 移动数组元素并计算总和
    for (uint8_t i = 0; i < len - 1; i++)
    {
        array[i] = array[i + 1];
        sum += array[i];
    }
    array[len - 1] = cur_value;
    return sum;
}

// 计算数组元素平均值
uint16_t avg_cal(uint16_t *array, uint8_t len, uint16_t cur_value)
{
    // 返回数组元素之和除以数组长度的结果
    return (sum_cal(array, len, cur_value) / len);
}

//数据测试
void TestDeviceData(void)
{
    uart0_printf("心率=%d, 时间=%d, 次数=%d, 距离=%d, 卡路里=%d, 频率=%d, 速度=%d, 功率=%d, ",xFitShowAllData.Heart, \
                        xFitShowAllData.Time, xFitShowAllData.Count,xFitShowAllData.Distance, \
                        xFitShowAllData.Calories, xFitShowAllData.Freq, xFitShowAllData.Speed, xFitShowAllData.Power);
}

// 单脉冲模式数据采集函数
static void Single_Pulse_Acquire(void)
{
    // 计算脉冲间隔时间，如果不在范围内则返回
    if (!Pulse_Interval_Cal(&Sport_Para.Speed_Interval)) {return;}
    
    // 增加次数并更新速度引脚标志为活跃状态
    Pulse_Para.Count_Para.count++;
    Sport_Para.Flag.Speed = Active;
    
    // 重新启动脉冲检测定时器
    Restart_Timer(Pulse_Detection);
}

// 双脉冲模式引脚设置函数
static void DoubleModePinSet(void)
{
    // 当检测到第一个脉冲时，设置双脉冲模式状态为一号引脚第一个脉冲的状态
    Sport_Para.Dounble_Mode_Para.Double_Pulse_Mode_State = First_Pin_Pulse_1;
    
    // 如果速度传感器A引脚有中断标志，则设置第一引脚为A，第二引脚为B
    if (GetGPIO_Interrupt_Flag(Pulse_Para.Speed_Pin_A.pin_number))
    {
        Sport_Para.Dounble_Mode_Para.first_pin = Pulse_Para.Speed_Pin_A.pin_number;
        Sport_Para.Dounble_Mode_Para.second_pin = Pulse_Para.Speed_Pin_B.pin_number;
    }
    // 如果速度传感器B引脚有中断标志，则设置第一引脚为B，第二引脚为A
    else if (GetGPIO_Interrupt_Flag(Pulse_Para.Speed_Pin_B.pin_number))
    {
        Sport_Para.Dounble_Mode_Para.first_pin = Pulse_Para.Speed_Pin_B.pin_number;
        Sport_Para.Dounble_Mode_Para.second_pin = Pulse_Para.Speed_Pin_A.pin_number;
    }
}

/**  
 * 双感波形示例  ABBA
 *     ___          ___  
 *  __|   |________|   |__       A  
 *        ___     ___      
 *  _____|   |___|   |____       B
 */ 
// 双脉冲模式数据采集函数
static void Double_Pulse_Acquire(void)
{
    switch (Sport_Para.Dounble_Mode_Para.Double_Pulse_Mode_State)
    {
        // 空闲状态
        case IDLE:
            DoubleModePinSet();
        break;
        
        // 一号速度引脚第一个脉冲
        case First_Pin_Pulse_1:
            // 如果检测到二号速度引脚触发中断，则更新状态进下一阶段
            if (GetGPIO_Interrupt_Flag(Sport_Para.Dounble_Mode_Para.second_pin))
            {
                Sport_Para.Dounble_Mode_Para.Double_Pulse_Mode_State = Second_Pin_Pulse_1;
                break;
            }
            // 否则重新设置引脚
            DoubleModePinSet();
        break;
        
        // 二号速度引脚第一个脉冲
        case Second_Pin_Pulse_1:
            // 如果检测到二号速度引脚触发中断，则更新状态进下一阶段
            if (GetGPIO_Interrupt_Flag(Sport_Para.Dounble_Mode_Para.second_pin)) 
            {
                Sport_Para.Dounble_Mode_Para.Double_Pulse_Mode_State = Second_Pin_Pulse_2;
                break;
            }
            // 否则重新设置引脚
            DoubleModePinSet();
        break;
        
        // 二号速度引脚第二个脉冲
        case Second_Pin_Pulse_2:
            // 如果检测到一号速度引脚触发中断，则表示一组双感脉冲完成
            if (GetGPIO_Interrupt_Flag(Sport_Para.Dounble_Mode_Para.first_pin))
            {
                // 此时一组双感脉冲完成，重置状态位
                Sport_Para.Dounble_Mode_Para.Double_Pulse_Mode_State = IDLE;
                
                // 计算脉冲间隔时间，如果不在范围内则返回
                if (!Pulse_Interval_Cal(&Sport_Para.Speed_Interval)) {
                    return;
                }
                
                // 增加次数并更新速度引脚标志为活跃状态
                Pulse_Para.Count_Para.count++;
                Sport_Para.Flag.Speed = Active;
                
                // 重新启动脉冲检测定时器
                Restart_Timer(Pulse_Detection);
            }
            // 否则重新设置引脚
            DoubleModePinSet();
        break;  
    }
}


// 脉冲数据计算函数
void Pulse_Data_Cal(void)
{
    // 静态数组用于存储RPM数据
    static uint16_t RPM_array[SPEED_BUF_SIZE] = {0};
    
    // 将时间转换为一秒为单位
    xFitShowAllData.Time = system_time / 100;
    
    // 如果速度引脚标志为未激活，则返回
    if (Sport_Para.Flag.Speed == In_Active)
    {
        // 如果频率为0，则返回
        if (xFitShowAllData.Freq == 0) {return;}
        
        // 计算平均频率、速度和功率，并返回
        xFitShowAllData.Freq = avg_cal(RPM_array, SPEED_BUF_SIZE, 0);
        xFitShowAllData.Speed = RPM_To_Speed(xFitShowAllData.Freq, Pulse_Para.Count_Para.distance_per_count);
        xFitShowAllData.Power = xFitShowAllData.Speed / 20;
        return;
    }
    
    // 计算当前脉冲的频率、平均频率、速度和功率
    xFitShowAllData.Freq = Interval_To_RPM(Sport_Para.Speed_Interval.interval);
    xFitShowAllData.Freq = avg_cal(RPM_array, SPEED_BUF_SIZE, xFitShowAllData.Freq);
    xFitShowAllData.Speed = RPM_To_Speed(xFitShowAllData.Freq, Pulse_Para.Count_Para.distance_per_count);
    xFitShowAllData.Power = xFitShowAllData.Speed / 20;
    
    // 如果每次脉冲记一次，则直接使用脉冲计数器的值，否则进行转换
    if (Pulse_Para.Count_Para.pulse_per_count == 10) {xFitShowAllData.Count = Pulse_Para.Count_Para.count;} 
    else {xFitShowAllData.Count = ((uint32_t)Pulse_Para.Count_Para.count * 100 / Pulse_Para.Count_Para.pulse_per_count + 5) / 10;}
    
    // 计算运动距离
    xFitShowAllData.Distance = ((uint32_t)Pulse_Para.Count_Para.distance_per_count * Pulse_Para.Count_Para.count / 1000);
    
    // 根据卡路里计算方式计算卡路里
    if (Pulse_Para.Calories_Calculation_Method == Base_On_Distance) 
    {xFitShowAllData.Calories = (uint16_t)(xFitShowAllData.Distance * Pulse_Para.Calories_Calculation_Para.calories_distance_rate / 1000);} 
    else 
    {xFitShowAllData.Calories = (uint16_t)((uint32_t)Pulse_Para.Count_Para.count * Pulse_Para.Calories_Calculation_Para.calories_count_rate / 1000);}

    // 如果设备类型为10（跑步机），则速度需除以10
    if (Pulse_Para.Device_Code.type == 10) {xFitShowAllData.Speed /= 10;}
    
    // 如果单位为英制，则将速度和距离进行转换
    if (Pulse_Para.Unit == Imperial)
    {
        xFitShowAllData.Speed = (uint16_t)((float)xFitShowAllData.Speed * 0.6214 + 0.5);
        xFitShowAllData.Distance = (uint16_t)((float)xFitShowAllData.Distance * 0.6214 + 0.5);
    }
    
    // 如果距离超过32000且设备类型不为10，则进行处理
    if (xFitShowAllData.Distance >= 32000 && Pulse_Para.Device_Code.type != 10) 
    {
        xFitShowAllData.Distance = ((xFitShowAllData.Distance + 5) / 10) | 0x8000;  // +5是为了四舍五入，使数据变化更平滑
    }
}

// 心率采集函数
void Heart_Rate_Acquire(void)
{
    // 如果心率间隔时间不在范围内，则返回
    if (!Pulse_Interval_Cal(&Sport_Para.Heart_Interval)) {return;}
    
    // 更新心率引脚标志为激活并重新启动心率检测定时器
    Sport_Para.Flag.Heart = Active;
    Restart_Timer(Heart_Detection);
}

// 心率计算函数
void Heart_Data_Cal(void)
{
    // 静态数组用于存储心率数据
    static uint16_t heart_array[HEART_BUF_SIZE] = {0};
    
    // 如果心率标志为未激活，则清零数组并返回
    if (Sport_Para.Flag.Heart == In_Active)
    {
        memset(heart_array, 0, HEART_BUF_SIZE);
        xFitShowAllData.Heart = 0;
        return;
    }
    
    // 计算当前心率
	uint16_t heart_temp = Interval_To_RPM(Sport_Para.Heart_Interval.interval);
    
    // 如果心率不在有效范围内，则返回
    if (heart_temp < 50 || heart_temp > 180) {return;}
    
    // 计算平均心率
    heart_temp = avg_cal(heart_array, HEART_BUF_SIZE, heart_temp);
    
    // 如果心率不在有效范围内，则返回
    if (heart_temp < 50 || heart_temp > 180) {return;}
    
    // 将平均心率保存到设备数据中
    xFitShowAllData.Heart = heart_temp;
}

// GPIO中断回调函数
static void GPIO_InterruptCallBack(void)
{
    // 如果心率引脚触发了中断
    if (GetGPIO_Interrupt_Flag(Pulse_Para.Heart_Pin.pin_number))
    {
        // 调用心率采集函数并重新使能GPIO中断，然后返回
        Heart_Rate_Acquire();
        GPIO_INT_ENABLE();
        return;
    }
    
    // 根据脉冲计数模式进行不同处理
    if (Pulse_Para.Count_Mode == Single_Pulse_Mode)
    {
        // 如果是单脉冲模式，调用单脉冲采集函数
        Single_Pulse_Acquire();
    }
    else if (Pulse_Para.Count_Mode == Double_Pulse_Mode)
    {
        // 如果是双脉冲模式，调用双脉冲采集函数
        Double_Pulse_Acquire();
    }
    else if (Pulse_Para.Count_Mode == Long_Short_Wave_Mode)
    {
        // 如果是长短波模式，则
    }
    
    // 重新使能GPIO中断
    GPIO_INT_ENABLE();
}

// APP连接部分
void Sport_Starting(void)
{
    // 启动运行时间定时器和数据处理定时器
    Start_Timer(Running_Time);
    Start_Timer(Data_Process);
}

// APP断开连接部分
void Sport_Stoping(void)
{
    // 停止运行时间定时器和数据处理定时器，并清除运动信息
    Stop_Timer(Running_Time);
    Stop_Timer(Data_Process);
    ClearSportInfoOnly();
}


// 测试
void nvds_test1(void)
{
    uint8_t ret = 0;
    uint8_t len = sizeof(Pulse_Para_Config_t);

    Pulse_Para_Config_t Pulse_Para_Config_Test;
    Pulse_Para_Config_t Pulse_Para_Config_Test2;
    
    Pulse_Para_Config_Test.Device_Code.factory = 666;
    Pulse_Para_Config_Test.Device_Code.model = 666;
    
    // 判断flash是否有配置好的参数，如果有就用它的，没有的话就正常初始化
//    ret = nvds_get(NVDE_TAG_PULSE_CONFIG, &len, (uint8_t *)&Pulse_Para_Config_Test2);
//    uart0_printf("ret = %d \r\n", ret);
    
    // 保存配置参数
    ret = nvds_put(NVDE_TAG_PULSE_CONFIG, len, (uint8_t *)&Pulse_Para_Config_Test);
    uart0_printf("ret = %d \r\n", ret);
    
    // 读取配置参数，并更新到对应的数据结构
    ret = nvds_get(NVDE_TAG_PULSE_CONFIG, &len, (uint8_t *)&Pulse_Para_Config_Test2);

    Pulse_Para.Device_Code.factory = Pulse_Para_Config_Test2.Device_Code.factory;
    Pulse_Para.Device_Code.model = Pulse_Para_Config_Test2.Device_Code.model;
    
    uart0_printf("Test2.factory = %d \r\n", Pulse_Para_Config_Test2.Device_Code.factory);
    uart0_printf("Test2.model = %d \r\n", Pulse_Para_Config_Test2.Device_Code.model);
    
    uart0_printf("Device_Code.factory = %d \r\n", Pulse_Para.Device_Code.factory);
    uart0_printf("Device_Code.model = %d \r\n", Pulse_Para.Device_Code.model);
}

