/*
 * @Author: LHB
 * @Date: 2023-05-29 11:12:19
 * @LastEditors: LHB
 * @LastEditTime: 2024-04-12 10:53:13
 * @Description: 
 * 1
 */
#include "app.h"

#include <string.h>

#include "co_utils.h"

#include "icu.h"

#include "uart.h"
#include "uart2.h"
#include "pwm.h"
#include "gpio.h"

#include "SystemConfig.h"
#include "Device.h"
#include "app_fff0.h"
#include "app_fee0.h"
#include "app_ftms.h"
#include "app_adv.h"
#include "app_scan.h"
#include "app_init.h"

#include "wdt.h"
#include "adc.h"

#include "list_hb.h"
#include "Match_hb.h"

#include "app_batt.h"

#include "timer_task_frame.h"
#include "pulse_frame.h"

//================================================================================================================================================//HB-注释
#define _arg_n(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,N,...) 	N
#define _get_args_cnt(...)	_arg_n(__VA_ARGS__)
#define _args_seqs() 		20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
#define _argc(...) 			_get_args_cnt(0, __VA_ARGS__, _args_seqs())
#define BIN(...)  			(unsigned char[]){__VA_ARGS__}, _argc(__VA_ARGS__)
#define MAKEWORD(a, b)				(unsigned int)((unsigned int)(a) | ((unsigned int)(b) << 8))

//================================================================================================================================================//HB-注释
extern app_FTMS_Handler_t xApp_FTMS_Handler;
extern unsigned char BLE_SLAVE_Box_ReadyFlag,BLE_SLAVE_Heart_ReadyFlag;
extern uint16_t  Heart_SRC_ID ,BOX_SRC_ID ;
extern unsigned char step_flag,Disconnect_will_flag;
extern unsigned char Con_Slave_Step;
extern uint16_t Heart_Device_Con_conhdl,BOX_Device_Con_conhdl ;
extern uint8_t Heart_Disconnect_ID,BOX_Disconnect_ID,APP_Disconnect_ID;
extern uint16_t HB_FFF2_Handle_val,HB_Read2A29_Handle_val,HB_Read2A24_Handle_val ,HB_Read2A2A_Handle_val ,HB_Read2ACC_Handle_val ,HB_Read2A37_Handle_val ;
extern bool Slave_ConnectComplete,Disconnect_slave_afterSave,ResetAD_Flag,resetAD_bit;

extern uint8_t batt_Voltage,batt_level;
extern bool To_calib_adc_Flag;
//================================================================================================================================================//HB-注释
DeviceInformation_t *Init_deviceBLE_Information;
FitShowAllData_t xFitShowAllData;
static FitShowSystemConfig_t *pFitShowSystemConfig = NULL;
FitShowAllData_t xFitShowAllData;

bool ResetCPU_Flag=false;

uint8_t IO_or_Uart_Mode = FITSHOW_DEVICE_PARAM_DATA_ACQ_UART_MODE;//FITSHOW_DEVICE_PARAM_DATA_ACQ_UART_MODE

volatile uint32_t ulSysTick = 0;
volatile static uint32_t prvulTick_X100us = 0;
volatile uint8_t bSetEnvironmentComplete = false;
volatile uint8_t bBLE_InitStackComplete = false;

uint8_t MacAddr_Info[6];

uint8_t Connect_His_Flag = 0,Connect_CB_Flag = 0,CURRENT_CNT_STA = 0;

uint8_t Heart_clearn_flag = 0;

//================================================================================================================================================//HB-注释
static uint8_t BaudRateIndex = 0,OutTimeCount = 0;
uint8_t UartInit_Flag = 0;
const static uint32_t BaudRate[] = {4096, 4800, 9600, 19200, 38400, 57600, 115200};
//const static uint32_t BaudRate[] = {9600, 9600, 9600, 9600, 9600, 9600}; 

bool prvbNewProtocolFlag = false,Get_NVDS_SuccessFlag = false;
//================================================================================================================================================//HB-注释
sport_t Device_SportInfo;
device_t Device_BaseInfo;
//================================================================================================================================================//HB-注释
static void prvvTimerCallBack(unsigned char ucChannel);
void receive_uart_data(uint8_t * data, uint8_t size);
void fitshow_Decryption(uint8_t *data, uint8_t size);
//================================================================================================================================================//HB-注释
// extern timer_task* timer_test;
timer_task* timer_task_UartInit,*timer_task_Listen,*timer_task_FTMS1,*timer_task_FTMS2,*timer_task_Con,*timer_task_Checker;
timer_task* timer_task_Sync_Data,*timer_task_MasterAD,*timer_task_BATT;
static void prvvTimer_X100ms(void)
{
    timer_tick(timer_task_Checker);
    timer_tick(timer_task_Con);
    timer_tick(timer_task_UartInit);
    timer_tick(timer_task_Listen);
    timer_tick(timer_task_Sync_Data);
    timer_tick(timer_task_MasterAD);

    timer_tick(timer_task_FTMS1);
    timer_tick(timer_task_FTMS2);
    timer_tick(timer_task_BATT);
}

static void prvvTimer_X1ms(void)
{
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------//HB-注释

static void prvvSpeedPinA_SoftFilter(void)
{

}

void delay(uint32_t Delaycnt)
{
	uint32_t i;
	
	for(i = 0; i<Delaycnt; i++)
	{
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	}
}

static void prvvTimerCallBack(unsigned char ucChannel)
{
	if (ucChannel == 0)
	{
		prvulTick_X100us++;

		prvvSpeedPinA_SoftFilter();

		if (prvulTick_X100us % 10 == 0)	// 1ms
		{
			ulSysTick++;
			if (bBLE_InitStackComplete == false)
			{
				if (ulSysTick % 10 == 0)
				{
					DeviceLoop();
				}
			}
			prvvTimer_X1ms();
			if (ulSysTick % 100 == 0)
			{
				prvvTimer_X100ms();
			}
		}

        if(prvulTick_X100us>=50000)
        {
            prvulTick_X100us = 0;
        }
        if(ulSysTick>=50000)
        {
            ulSysTick = 0;
        }
	}
}

static void BLE_Con_Checker(void){
    Master_SlaveConnectTaskList();

    if(BLE_SLAVE_Box_ReadyFlag == Yes_Ready ){
        Check_FitShowPro_CB(step_flag);
    }


}

static void MasterAD_Con_CB(void){
    // 原版
    // if(Slave_ConnectComplete == true){
    //     if(CURRENT_CNT_STA == 0){
    //         Advertising_Start();
    //     }
    // }else{
    //     Advertising_Stop();
    // }

    if(BLE_SLAVE_BOX_Scan == 1){
        if(Slave_ConnectComplete == true){
        if(CURRENT_CNT_STA == 0){
            Advertising_Start();
        }
        }else{
            Advertising_Stop();
        }
    }
    
}

static uint8_t Disconnect_will_cnt = 13;
static void BLE_Con_CB(void){
 
    if(Disconnect_will_flag > 0 && Disconnect_will_cnt > 1){
        Disconnect_will_cnt--;
    }

    if(Disconnect_will_flag > 0 && Disconnect_will_cnt == 1){

        appm_disconnect(BOX_Disconnect_ID);
        Disconnect_will_cnt = 13;
        Disconnect_will_flag = 0;
    }


    if(Heart_clearn_flag > 0){
        uint8_t temp_heartData[2]={0,0};
        fitshow_protocol_forHeartSlave(temp_heartData,2);
        Heart_clearn_flag -- ;
    }
    
}
extern bool Box_ScanBusy_Flag;
uint8_t resetAD_cnt = 0;
uint8_t BOXreset_Ready_cnt = 0,Heartreset_Ready_cnt;

static void Listen_CB(){

    
    if(ResetCPU_Flag == false)
    wdt_feed();
    Update_ConnectFlag();
    if(resetAD_bit == true){
        
        resetAD_cnt++;
        if(resetAD_cnt==1){
            if(UartInit_Flag != Step_Complete){
                resetAD_cnt--;
                return;
            }
            appm_stop_advertising();
            appm_delete_advertising();
        }
        else if(resetAD_cnt == 5){
   
            resetAD_cnt = 0;
            resetAD_bit = false;
            appm_ReSet_AD_HB();
        }
    }

    if(BLE_SLAVE_Heart_ReadyFlag == Readying){
        Heartreset_Ready_cnt++;
        if(Heartreset_Ready_cnt >= 6){
            Heartreset_Ready_cnt = 0;
            BLE_SLAVE_Heart_ReadyFlag = No_Ready;
        }
    }
    if(BLE_SLAVE_Box_ReadyFlag == Readying){
        BOXreset_Ready_cnt++;
        if(BOXreset_Ready_cnt >= 6){
            BOXreset_Ready_cnt = 0;
            BLE_SLAVE_Box_ReadyFlag = No_Ready;
        }
    }

}

uint8_t update_cnt = 0;
static void FTMS_1_CB(void){

    
    if(UartInit_Flag != Step_Complete){
        return;
    }
    update_cnt++;
    if (IO_or_Uart_Mode == FITSHOW_DEVICE_PARAM_DATA_ACQ_UART_MODE)
    {
        if(Connect_CB_Flag==0)
        if(app_ftms_env.Type == TREADMILL_TYPE)
        // if(Device_BaseInfo.info.type == 0)
        {		
            
            uint8_t index = 0;
            uint8_t Data[20];
            Data[index++] = 0x02;
            Data[index++] = 0x51;
            Data[index++] = 0x51;
            Data[index++] = 0x03;
            if(BLE_SLAVE_BOX_Scan == 0)
            BLE_SendToUart0(&Data[0], index);	
            else if(BLE_SLAVE_BOX_Scan == 1){
                
                appc_write_service_data_req(BOX_Disconnect_ID,HB_FFF2_Handle_val,index,Data);	
            }	
        }
        else if(app_ftms_env.Type != TREADMILL_TYPE)
        {
            uint8_t index = 0;
            uint8_t Data[20];
            if(update_cnt%2 == 0)
            {
                
                Data[index++] = 0x02;
                Data[index++] = 0x43;
                Data[index++] = 0x01;
                Data[index++] = 0x42;
                Data[index++] = 0x03;

                
                // app_FFF0S_FFF1_SendData(APP_Disconnect_ID,Data, index);
                // app_fee4_send_ntf(APP_Disconnect_ID,index,Data);

                if(BLE_SLAVE_BOX_Scan == 0)
                BLE_SendToUart0(&Data[0], index);	
                else if(BLE_SLAVE_BOX_Scan == 1){
                
                    appc_write_service_data_req(BOX_Disconnect_ID,HB_FFF2_Handle_val,index,Data);	
                }	
            }else {
                Data[index++] = 0x02;
                Data[index++] = 0x42;
                Data[index++] = 0x42;
                Data[index++] = 0x03;
                if(BLE_SLAVE_BOX_Scan == 0)
                BLE_SendToUart0(&Data[0], index);	
                else if(BLE_SLAVE_BOX_Scan == 1){
             
                    appc_write_service_data_req(BOX_Disconnect_ID,HB_FFF2_Handle_val,index,Data);	
                    
                }	
            }
            
            
        }
    }

    if(CURRENT_CNT_STA == 0 || Connect_CB_Flag>0){
 
        return;
    }

    

    #if FTMS_SumUpdateChannel == 1
	if(app_ftms_env.Type == CROSS_TYPE || app_ftms_env.Type == ROWER_TYPE || app_ftms_env.Type == INDOOR_BIKE_TYPE)
	{
     
        if(app_ftms_env.Type == CROSS_TYPE || app_ftms_env.Type == INDOOR_BIKE_TYPE){
            app_FitShow_To_FTMS_Cross_Trainer_Data_Notify_handler(APP_Disconnect_ID,&xFitShowAllData,
																CrossTrainerDataElapsedTime |
																CrossTrainerDataHeartRate |
																CrossTrainerDataStepCount);
            app_FitShow_To_FTMS_Cross_Trainer_Data_Notify_handler(APP_Disconnect_ID,&xFitShowAllData,
                                                                    CrossTrainerDataMoreData |
                                                                    CrossTrainerDataInstantaneousPower |	
                                                                    CrossTrainerDataExpendedEnergy);	
            app_FitShow_To_FTMS_Indoor_Bike_Data_Notify_handler(APP_Disconnect_ID,&xFitShowAllData,
                                                                IndoorBikeDataInstantaneousCadence |
                                                                    IndoorBikeDataInstantaneousPower |
                                                                    IndoorBikeDataElapsedTime |
                                                                    IndoorBikeDataHeartRate |
                                                                    IndoorBikeDataExpendedEnergy |
                                                                    IndoorBikeDataTotalDistance);
        }else{
            app_FitShow_To_FTMS_Rower_Data_Notify_handler(APP_Disconnect_ID,&xFitShowAllData,
                                                                RowerDataElapsedTime |
                                                                RowerDataExpendedEnergy |
                                                                RowerDataTotalDistance |
                                                                RowerDataHeartRate |
                                                                RowerDataInstantaneousPower |
                                                                RowerDataInstantaneousPace);
        }
	}
    #endif

    
    switch (app_ftms_env.Type)//
	{
	case TREADMILL_TYPE:
	
	//-
	if(1 )
	{
		/* code */
		app_FitShow_To_FTMS_Treadmill_Data_Notify_handler(APP_Disconnect_ID,&xFitShowAllData,
														TreadmillDataMoreData |
														TreadmillDataTotalDistance |
														TreadmillDataInclinationAndRampAngleSetting);
		app_FitShow_To_FTMS_Treadmill_Data_Notify_handler(APP_Disconnect_ID,&xFitShowAllData,
															TreadmillDataExpendedEnergy |
															TreadmillDataHeartRate |
															TreadmillDataElapsedTime);		
	}

		
		break;
#if FTMS_SumUpdateChannel == 0
	case CROSS_TYPE:
		
		if(1 )
		{		
				app_FitShow_To_FTMS_Cross_Trainer_Data_Notify_handler(APP_Disconnect_ID,&xFitShowAllData,
																		CrossTrainerDataElapsedTime |
																			CrossTrainerDataHeartRate |
																			CrossTrainerDataStepCount);
				app_FitShow_To_FTMS_Cross_Trainer_Data_Notify_handler(APP_Disconnect_ID,&xFitShowAllData,
																		CrossTrainerDataMoreData |
																			CrossTrainerDataInstantaneousPower |	
																			CrossTrainerDataExpendedEnergy);					
		}
		
		
		break;
#endif
	case STEP_TYPE:
		
	
		if(1 )
		{
		
		/* code */
		}
		
		break;
	case STAIR_TYPE:
	
		if(1 )
		{
		
		/* code */
		}
		
		break;
#if FTMS_SumUpdateChannel == 0
	case ROWER_TYPE:
	
		if(1 )
		{
	
			app_FitShow_To_FTMS_Rower_Data_Notify_handler(APP_Disconnect_ID,&xFitShowAllData,
																										RowerDataElapsedTime |
																											RowerDataExpendedEnergy |
																											RowerDataTotalDistance |
																											RowerDataHeartRate |
																											RowerDataInstantaneousPower |
																											RowerDataInstantaneousPace);
		}
        break;
#endif		

		
#if FTMS_SumUpdateChannel == 0
	case INDOOR_BIKE_TYPE:

		if(1 )
		{

				app_FitShow_To_FTMS_Indoor_Bike_Data_Notify_handler(APP_Disconnect_ID,&xFitShowAllData,
															IndoorBikeDataInstantaneousCadence |
																IndoorBikeDataInstantaneousPower |
																IndoorBikeDataElapsedTime |
																IndoorBikeDataHeartRate |
																IndoorBikeDataExpendedEnergy |
																IndoorBikeDataTotalDistance);
		}
		
		break;
#endif
	}
    
    
    
    
}

static void FTMS_2_CB(void)
{

}

static void VoltageCheck_CB(void){
    To_calib_adc_Flag = true;
	
	app_batt_send_lvl(batt_level);

	if(batt_Voltage<Save_Voltage){
		cpu_reset();
	}
}

static void Sync_Data_CB(void){
    if(UartInit_Flag != Step_Complete)return;
    
    uint8_t index = 0,fcs = 0;
    uint8_t Data[20];
    
        Data[index++] = 0x02;
        Data[index++] = 0x53;
        Data[index++] = 0x9f;
        Data[index++] = 0x02;
        Data[index++] = 0x00;

        Data[index++] = xFitShowAllData.Heart;

        for (uint8_t i = 1; i < index ; i++)
                fcs ^= Data[i];

        Data[index++] = fcs;
        Data[index++] = 0x03;

        if((BLE_SLAVE_Heart_Scan == 1 && Heart_clearn_flag>0 && BLE_SLAVE_Heart_ReadyFlag != Yes_Ready)||\
        (BLE_SLAVE_Heart_Scan == 1 &&  BLE_SLAVE_Heart_ReadyFlag == Yes_Ready)){
            //HB_临时注释:断开时要做一次清零的同步，后续是否同步可以表上做选择，如果数据都是0，就不同步为0
            BLE_SendToUart0(&Data[0], index);
        }
}

static void Uartinit_CB(){
    if(IO_or_Uart_Mode == FITSHOW_DEVICE_PARAM_DATA_ACQ_UART_MODE){
        if(UartInit_Flag == Step_FactryInformation){
            if(BLE_SLAVE_BOX_Scan == 0){
                send_fomat_uart0(BIN(FC_CMD_Code_SysInfo,sunCMD_Code_SysInfo_model),0);
                uart_init(BaudRate[BaudRateIndex % (sizeof(BaudRate) / sizeof(uint32_t))]);
                BaudRateIndex++;
            }
            
            else{
                send_fomat_fff2_toSlave(BIN(FC_CMD_Code_SysInfo,sunCMD_Code_SysInfo_model),0);
            }

            if(BaudRateIndex>=40){
                // Advertising_Stop();
                Device_BaseInfo.info.type = 2;
                Device_BaseInfo.info.model = 999;
                Device_BaseInfo.info.brand = 208;

                xFitShowAllData.xDeviceDataTypePara.Type = Device_BaseInfo.info.type;
                xFitShowAllData.xDeviceDataTypePara.Model = Device_BaseInfo.info.model;
                xFitShowAllData.xDeviceDataTypePara.Factory = Device_BaseInfo.info.brand;

                UartInit_Flag = Step_OutTime;
            }
        }
        else if(UartInit_Flag == Step_SpeedRange){
            
            if(BLE_SLAVE_BOX_Scan == 0)
            send_fomat_uart0(BIN(FC_CMD_Code_SysInfo,sunCMD_Code_SysInfo_speed),0);
            else{
                send_fomat_fff2_toSlave(BIN(FC_CMD_Code_SysInfo,sunCMD_Code_SysInfo_speed),0);
            }
            OutTimeCount++;

            if(OutTimeCount>=10){
                OutTimeCount = 0;
                Device_BaseInfo.param.max_speed = 200;
                Device_BaseInfo.param.min_speed = 0;

                xFitShowAllData.xSpeedRange.Maximum = Device_BaseInfo.param.max_speed;
                xFitShowAllData.xSpeedRange.Minimum = Device_BaseInfo.param.min_speed;
                xFitShowAllData.xSpeedRange.Increment = 10;
                UartInit_Flag = Step_Complete;
            }
        }
        else if(UartInit_Flag == Step_InclinationRange){
            
            if(BLE_SLAVE_BOX_Scan == 0)
            send_fomat_uart0(BIN(FC_CMD_Code_SysInfo,sunCMD_Code_SysInfo_incline),0);
            else{
                send_fomat_fff2_toSlave(BIN(FC_CMD_Code_SysInfo,sunCMD_Code_SysInfo_incline),0);
            }
            OutTimeCount++;
            if(OutTimeCount>=10){
                OutTimeCount = 0;
                Device_BaseInfo.param.min_incline = 0;
                Device_BaseInfo.param.max_incline = 0;

                xFitShowAllData.xInclinationRange.Maximum = Device_BaseInfo.param.max_incline;
                xFitShowAllData.xInclinationRange.Minimum = Device_BaseInfo.param.min_incline;
                xFitShowAllData.xInclinationRange.Increment = 10;
                UartInit_Flag = Step_Complete;
            }
        }
        else if(UartInit_Flag == Step_ResRange){
            
            if(BLE_SLAVE_BOX_Scan == 0)
            send_fomat_uart0(BIN(FC_CMD_Code_SysInfo2,sunCMD_Code_SysInfo2_Res),0);
            else{
                send_fomat_fff2_toSlave(BIN(FC_CMD_Code_SysInfo2,sunCMD_Code_SysInfo2_Res),0);
            }
            OutTimeCount++;
            if(OutTimeCount>=10){
                OutTimeCount = 0;
                Device_BaseInfo.param.max_level = 0;
                Device_BaseInfo.param.min_level = 0;
                Device_BaseInfo.param.max_incline = 0;
                Device_BaseInfo.param.min_incline = 0;

                
                xFitShowAllData.xResistanceRange.Maximum = Device_BaseInfo.param.max_level;
                xFitShowAllData.xResistanceRange.Minimum = 1;				//
			    xFitShowAllData.xResistanceRange.Increment = 1;			//
                UartInit_Flag = Step_Complete;
            }
            
        }
        else if(UartInit_Flag == Step_Complete){
            // delete_timer_task(timer_task_UartInit);
            
        }
        else if(UartInit_Flag == Step_OutTime){

            

            uart_init(BaudRate[BaudRateIndex % (sizeof(BaudRate) / sizeof(uint32_t))]);
            BaudRateIndex++;

            if(BLE_SLAVE_BOX_Scan == 0)
            send_fomat_uart0(BIN(FC_CMD_Code_SysInfo,sunCMD_Code_SysInfo_model),0);
 
        }
        else if(UartInit_Flag == Step_OutTime_Idle){

        }


        
        
        
    }
    else{}
}
void ClearSportInfoOnly(void){
    xFitShowAllData.AveragePace = 0;
    xFitShowAllData.AverageSpeed = 0;
    xFitShowAllData.Calories = 0;
    xFitShowAllData.Count = 0;
    xFitShowAllData.Distance = 0;
    xFitShowAllData.Freq = 0;
    #if(BLE_SLAVE_Heart_Scan == 0)
    xFitShowAllData.Heart = 0;
    #endif
    xFitShowAllData.Incline = 0;
    xFitShowAllData.InstantaneousPace = 0;
    xFitShowAllData.MetabolicEquivalent = 0;
    xFitShowAllData.Power = 0;
    xFitShowAllData.RemainingTime = 0;
    xFitShowAllData.Resistance = 0;
    xFitShowAllData.Speed = 0;
    xFitShowAllData.TargetIncline = 0;
    xFitShowAllData.TargetPower = 0;
    xFitShowAllData.TargetResistance = 0;
    
}

static void pwm_mode_timer_FS(void) // t:100ms
{
    PWM_DRV_DESC pwm_drv_desc;
    pwm_drv_desc.clk_src = PWM_CLK_XTAL16M;
    pwm_drv_desc.mode = PWM_MODE_TIMER;
    pwm_drv_desc.channel = 0;
    pwm_drv_desc.duty_cycle = 0;
    // pwm_drv_desc.end_value = 16000;	// 1ms
    pwm_drv_desc.end_value = 1600;	// 100us
    pwm_drv_desc.int_en = 1;
    pwm_drv_desc.contiu_mode = 0;
    pwm_drv_desc.en = 1;
    pwm_drv_desc.cpedg_sel = 1;
    pwm_drv_desc.p_Int_Handler = prvvTimerCallBack;
    pwm0_init(&pwm_drv_desc);

}
void gpio_Init_for_FS()
{
    gpio_config(0x11,OUTPUT,PULL_HIGH);
    gpio_config(0x12,OUTPUT,PULL_LOW);
}

//*************************************************************************
//*************************************************************************
//*************************************************************************
//*************************************************************************
//****************************Enter****************************************
//*************************************************************************
//*************************************************************************
//*************************************************************************
//*************************************************************************
//*************************************************************************


void DeviceInit()
{
    Init_deviceBLE_Information = get_Init_deviceBLE_Information();
    pFitShowSystemConfig = pGetFitShowSystemConfig(); 

    IO_or_Uart_Mode = pFitShowSystemConfig->pSystemConfigInformation->eDataAcquisitionMode;

    //------------------------------------------------------------------------------------------------------------------------------------------------//HB-注释
    xFitShowAllData.xDeviceDataTypePara.Model = pFitShowSystemConfig->pSystemConfigInformation->Model;
	xFitShowAllData.xDeviceDataTypePara.Factory = pFitShowSystemConfig->pSystemConfigInformation->Factory;
	xFitShowAllData.xDeviceDataTypePara.Type = pFitShowSystemConfig->pSystemConfigInformation->Type;
	xFitShowAllData.xDeviceDataTypePara.DistancePerPulse = pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse;
	xFitShowAllData.xDeviceDataTypePara.CaloriesRate = pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesRate;
	xFitShowAllData.xDeviceDataTypePara.PulsePerCount = pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount;
	xFitShowAllData.xDeviceDataTypePara.Unit = pFitShowSystemConfig->pSystemConfigInformation->ucUnit; //  Metric  /   Imperial  
	xFitShowAllData.StartCountDownValue = pFitShowSystemConfig->pSystemConfigInformation->ucCountDownValue;

    xFitShowAllData.xSpeedRange.Maximum = 200*100;			//  
    xFitShowAllData.xSpeedRange.Minimum = 0;				//
    xFitShowAllData.xSpeedRange.Increment = 1*10;			//

    // xFitShowAllData.xInclinationRange.Maximum = 3;			
    // xFitShowAllData.xInclinationRange.Minimum = 0;				
    // xFitShowAllData.xInclinationRange.Increment = 0;			

    xFitShowAllData.xResistanceRange.Maximum = 25.5;			//
    xFitShowAllData.xResistanceRange.Minimum = 0;				//
    xFitShowAllData.xResistanceRange.Increment = 1;			//

    xFitShowAllData.xPowerRange.Maximum = 999;			//
    xFitShowAllData.xPowerRange.Minimum = 0;				//
    xFitShowAllData.xPowerRange.Increment = 0;			//

    xFitShowAllData.xHeartRateRange.Maximum = 250;			
    xFitShowAllData.xHeartRateRange.Minimum = 0;			
    xFitShowAllData.xHeartRateRange.Increment = 1;	

    if (pFitShowSystemConfig->pSystemConfigInformation->xAppLink.isEnable == true)//
	{
		gpio_config(pFitShowSystemConfig->pSystemConfigInformation->xAppLink.xAppLinkSignalPin.Pin, OUTPUT, PULL_NONE);
		
		gpio_config(pFitShowSystemConfig->pSystemConfigInformation->xAppLink.xAppLinkSignalPin2.Pin, OUTPUT, PULL_NONE);
	}

    if (pFitShowSystemConfig->pSystemConfigInformation->eDataAcquisitionMode == FITSHOW_DEVICE_PARAM_DATA_ACQ_PULSE_MODE)
	{
        //HB_临时注释:脉冲部分需要初始化的CODE放这边
        Pulse_frame_Init();
        Timer_Init();
        
        
		UartInit_Flag = Step_Complete;
	}

    //------------------------------------------------------------------------------------------------------------------------------------------------//HB-注释

    Device_BaseInfo.info.type = 2;
    xFitShowAllData.xDeviceDataTypePara.Type = Device_BaseInfo.info.type;
    if(Device_BaseInfo.info.type == TREADMILL){
        app_ftms_env.Type = TREADMILL_TYPE;
    }else if(Device_BaseInfo.info.type == CROSS_TRAINER){
        app_ftms_env.Type = CROSS_TYPE;
    }else if(Device_BaseInfo.info.type == INDOORBIKE){
        app_ftms_env.Type = INDOOR_BIKE_TYPE;
    }else if(Device_BaseInfo.info.type == ROWER){
        app_ftms_env.Type = ROWER_TYPE;
    }else{

    }
    //------------------------------------------------------------------------------------------------------------------------------------------------//HB-注释
    if(Get_BLEInformation_NVDS()==0){
        uart_printf("--get nvds success\r\n");//HB-注释
    }else{
        uart_printf("--get nvds fail\r\n");//HB-注释
    }

    //------------------------------------------------------------------------------------------------------------------------------------------------//HB-注释

    memset(app_env.dev_name,0,sizeof(app_env.dev_name));
    memcpy(app_env.dev_name, Init_deviceBLE_Information->arrBLE_Name, strlen(Init_deviceBLE_Information->arrBLE_Name));

    for(uint8_t i=0;i<6;i++)
    {
        MacAddr_Info[i] = co_default_bdaddr.addr[5-i];
    }

    get_nvds_box_addr_CB();

    pwm_mode_timer_FS();

    gpio_Init_for_FS();

    wdt_enable(192000);

    timer_task_Checker = create_timer_task(-1, 5, BLE_Con_Checker);//0.5s
    start_timer(timer_task_Checker);

    timer_task_Con = create_timer_task(-1, 10, BLE_Con_CB);//1s
    start_timer(timer_task_Con);

    timer_task_MasterAD = create_timer_task(-1, 10, MasterAD_Con_CB);//1s
    start_timer(timer_task_MasterAD);

    timer_task_Listen = create_timer_task(-1, 5, Listen_CB);//0.5s
    start_timer(timer_task_Listen);

    timer_task_UartInit = create_timer_task(-1, 5, Uartinit_CB);//0.5s
    start_timer(timer_task_UartInit);

    timer_task_Sync_Data = create_timer_task(-1, 9, Sync_Data_CB);//0.9s
    start_timer(timer_task_Sync_Data);

    timer_task_FTMS1 = create_timer_task(-1, 5, FTMS_1_CB);//0.5s
    start_timer(timer_task_FTMS1);

    timer_task_FTMS2 = create_timer_task(-1, 7, FTMS_2_CB);//0.7s
    start_timer(timer_task_FTMS2);

    timer_task_BATT = create_timer_task(-1, 10, VoltageCheck_CB);//5.7s
    start_timer(timer_task_BATT);
}


//================================================================================================================================================//HB-注释
void BLE_SendToUart0(uint8_t *data,uint16_t len)
{

    // if(BLE_SLAVE_BOX_Scan == 0)
    uart_send(data,len);
}

void BLE_SendToUart2(uint8_t *data,uint16_t len)
{
    uart2_send(data,len);
}

void DeviceLoop(void)
{

}
//================================================================================================================================================//HB-注释
/**
 * @description: fomat cmd
 * @return {*}
 */
void send_fomat_uart0(uint8_t *data,uint8_t size,uint8_t typeChoose)
{
    uint8_t temp_send_data[30];
    uint8_t index = 0;
    uint8_t fcs = 0;
#if typeChoose == 0
#define head_CMD 0x02
#define end_CMD 0x03
#endif

    temp_send_data[index++] = head_CMD;
    for(uint8_t i=0;i<size;i++)
    {
        temp_send_data[index++] = data[i];
    }

    for (uint8_t i = 1; i < index ; i++)
        fcs ^= temp_send_data[i];

    temp_send_data[index++] = fcs;
    temp_send_data[index++] = end_CMD;

    uart_send(temp_send_data,index);
}

void send_fomat_fff1(uint8_t *data,uint8_t size,uint8_t typeChoose)
{
    uint8_t temp_send_data[30];
    uint8_t index = 0;
    uint8_t fcs = 0;
#if typeChoose == 0
#define head_CMD 0x02
#define end_CMD 0x03
#endif

    temp_send_data[index++] = head_CMD;
    for(uint8_t i=0;i<size;i++)
    {
        temp_send_data[index++] = data[i];
    }

    for (uint8_t i = 1; i < index ; i++)
        fcs ^= temp_send_data[i];

    temp_send_data[index++] = fcs;
    temp_send_data[index++] = end_CMD;

    app_fff4_send_ntf(APP_Disconnect_ID,index,temp_send_data);
}

void send_fomat_fff2_toSlave(uint8_t *data,uint8_t size,uint8_t typeChoose)
{
    uint8_t temp_send_data[30];
    uint8_t index = 0;
    uint8_t fcs = 0;
#if typeChoose == 0
#define head_CMD 0x02
#define end_CMD 0x03
#endif

    temp_send_data[index++] = head_CMD;
    for(uint8_t i=0;i<size;i++)
    {
        temp_send_data[index++] = data[i];
    }

    for (uint8_t i = 1; i < index ; i++)
        fcs ^= temp_send_data[i];

    temp_send_data[index++] = fcs;
    temp_send_data[index++] = end_CMD;

    appc_write_service_data_req(BOX_Disconnect_ID,HB_FFF2_Handle_val,index,temp_send_data);	
}
/**
 * @description: 
 * @param {uint8_t *} data
 * @param {uint8_t} size
 * @return {*}
 */
void receive_uart_data(uint8_t * data, uint8_t size)
{
    uint8_t FCS = 0;


    

    if(data[0] == 0x02 && data[size-1] == 0x03){
        FCS = 0x00;
	    for (uint8_t i = 1; i < size - 1; i++)
		FCS ^= data[i];
        if (FCS){return;}

        if(UartInit_Flag == Step_OutTime){UartInit_Flag = Step_OutTime_Idle;}

        if(data[1] == FC_CMD_Code_Config){
            fitshow_protocol_forDevice(data,size);
            return;
        }
        else if(data[1] == 0XAB){
            // fitshow_protocol_forDevice(data,size);
            return;
        }
        else{
            // if(UartInit_Flag != Step_OutTime_Idle)
            fitshow_protocol_forDevice(data,size);
        }

        if(data[1] == 0x42 || data[1] == 0x51){
            if(BLE_SLAVE_Heart_Scan == 1){
                uint8_t fcs = 0;
                if(data[1] == 0x42){
                    data[8] = xFitShowAllData.Heart;
                    for (uint8_t i = 1; i < size-2 ; i++)
                            fcs ^= data[i];
                    data[size-2] = fcs;
                    
                }else if(data[1] == 0x51){
                    data[13] = xFitShowAllData.Heart;
                    for (uint8_t i = 1; i < size-2 ; i++)
                            fcs ^= data[i];
                    data[size-2] = fcs;
                    
                }
            }
        }

        BLE_SEND_DATA(data,size);
    }
    
}

void fitshow_protocol_forHeartSlave(uint8_t * data, uint8_t size){
    uint8_t temp_Heart_bpm = 0;
    if(1){
        temp_Heart_bpm = data[1];
        if(BLE_SLAVE_Heart_Scan == 1){
            Device_SportInfo.heart = temp_Heart_bpm;
            xFitShowAllData.Heart = temp_Heart_bpm;
        }

        Sync_Data_CB();
    }
}

void fitshow_protocol_forBoxSlave(uint8_t * data, uint8_t size){

    if(data[1] == 0x42 || data[1] == 0x43 || data[1] == 0x51  ){
        if(BLE_SLAVE_Heart_Scan == 1){
            uint8_t fcs = 0;
            if(data[1] == 0x42){
                data[8] = xFitShowAllData.Heart;
                for (uint8_t i = 1; i < size-2 ; i++)
                        fcs ^= data[i];
                data[size-2] = fcs;
            }else if(data[1] == 0x51){
                data[13] = xFitShowAllData.Heart;
                for (uint8_t i = 1; i < size-2 ; i++)
                        fcs ^= data[i];
                data[size-2] = fcs;
            }
        }
        
        fitshow_protocol_forDevice(data,size);
        if(BLE_SLAVE_BOX_Scan == 1){
            BLE_SEND_DATA(data,size);
        }
    }else if(data[1] == 0x50){
        fitshow_protocol_forDevice(data,size);
    }
    else if(data[1] == 0x41 && data[2] == 0x02){
        fitshow_protocol_forDevice(data,size);
        // if(BLE_SLAVE_BOX_Scan == 1){
        //     send_fomat_fff1(BIN(FC_CMD_Code_SysInfo2,sunCMD_Code_SysInfo2_Res,Device_BaseInfo.param.max_level,\
        //     Device_BaseInfo.param.Inch_enable,Device_BaseInfo.param.Inch_enable,0),0);
        // }
    }
    else{
        uart_printf("error!!!\r\n");//HB-注释
    }
}

void BLE_SEND_DATA(uint8_t * data, uint8_t size)
{
    if(CURRENT_CNT_STA == 0)return;
    

    app_fff4_send_ntf(APP_Disconnect_ID,size,data);

}

void BLE_REC_DATA(uint8_t * data, uint8_t size)
{
    
    switch (data[1])
    {
    case FC_CMD_Code_Config:
        /* code */
        prvvFitShowSetting(FitShow_AppConfig,data,size);
        break;
    case 0xab:
        fitshow_protocol_forDevice(data,size);
        break;
    case 0x46:
        if(data[2] == 0x01){
            DelMatchInfo_CB();
        }
        
        break;
    default:
        if(BLE_SLAVE_BOX_Scan == 0){
            BLE_SendToUart0(data,size);
        }else{
            send_fomat_fff1(BIN(FC_CMD_Code_SysInfo2,sunCMD_Code_SysInfo2_Res,Device_BaseInfo.param.max_level,\
            Device_BaseInfo.param.Inch_enable,Device_BaseInfo.param.Inch_enable,0),0);
        }
        
        break;
    }
}
//================================================================================================================================================//HB-注释
void Master_Connect_CB(void)
{
    Connect_CB_Flag = 5;
	CURRENT_CNT_STA = 1;
    
#if ConnectStart == true
    xApp_FTMS_Handler.StartOrResume();
#endif
    Connect_His_Flag = 1;
    
}
void Master_Disconnect_CB(void)
{
    Connect_CB_Flag = 5;
	CURRENT_CNT_STA = 0;

    if(Connect_His_Flag == 1)
    {
        Connect_His_Flag = 0;
    
        //HB_临时注释 停止指令
       
        xApp_FTMS_Handler.Stop();
    }
    
}
void Slave_Connect_CB(void)
{
    
}
void Slave_Disconnect_CB(void)
{
    Scanning_Start();//fitshow_protocol_forHeartSlave
    
}

void Advertising_Start(void)
{
    appm_start_advertising();
}
void Advertising_Stop(void)
{
    appm_stop_advertising();
}

void Scanning_Start(void)
{
    appm_start_scaning();
}
void Scanning_Stop(void)
{
    appm_stop_scaning();
}
void reset_mcu(void)
{
    cpu_reset();
}
//================================================================================================================================================//HB-注释
static void fitshow_BLE_Name()
{
	char Send_data[20];
	uint8_t Send_data_index = 0,FCS = 0 ,name_len = 0;

	Send_data[Send_data_index++] = 0x02;
	Send_data[Send_data_index++] = 0x44;
	Send_data[Send_data_index++] = 0xC5;

	name_len = strlen((char *)app_env.dev_name);

	strcpy(&Send_data[3], (char *)app_env.dev_name);
	
	Send_data_index = Send_data_index + name_len;

	for (uint8_t i = 1; i < Send_data_index ; i++)
		FCS ^= Send_data[i];
	Send_data[Send_data_index++] = FCS;

	Send_data[Send_data_index++] = 0x03;

	BLE_SendToUart0((uint8_t *)Send_data, Send_data_index);
	

}

void fitshow_protocol_forDevice(uint8_t * data, uint8_t size)
{
    uint16_t temp_val = 0;
    unsigned int distance = 0;
    uint8_t Data_index = 0,fcs=0;
	uint8_t Data[20];
    switch (data[1])
    {
    case FC_CMD_Code_Config:
        prvvFitShowSetting(FitShow_DeviceConfig,data,size);
        // fitshow_config_CB(data,size);
        
        break;
    case 0xAB:
        fitshow_Decryption(data, size);
    break;
//------------------------------------------------------------------------------------------------------------------------------------------------//HB-注释
    case 0x50:
        if(data[2]==0x00){
            if(size == 9){
                temp_val = MAKEWORD(data[3], data[4]);
                Device_BaseInfo.info.type =  temp_val >> 12;
                Device_BaseInfo.info.brand = temp_val & 0xfff;
                Device_BaseInfo.info.model = MAKEWORD(data[5], data[6]);

                xFitShowAllData.xDeviceDataTypePara.Type = Device_BaseInfo.info.type;
                xFitShowAllData.xDeviceDataTypePara.Model = Device_BaseInfo.info.model;
                xFitShowAllData.xDeviceDataTypePara.Factory = Device_BaseInfo.info.brand;
            }
            else if(size > 5){
                Device_BaseInfo.info.type  = 0;
				Device_BaseInfo.info.brand = data[3];
				Device_BaseInfo.info.model = MAKEWORD(data[4], data[5]);

                xFitShowAllData.xDeviceDataTypePara.Type = Device_BaseInfo.info.type;
                xFitShowAllData.xDeviceDataTypePara.Model = Device_BaseInfo.info.model;
                xFitShowAllData.xDeviceDataTypePara.Factory = Device_BaseInfo.info.brand;
            }
            if(Device_BaseInfo.info.type == TREADMILL){
                app_ftms_env.Type = TREADMILL_TYPE;
            }else if(Device_BaseInfo.info.type == CROSS_TRAINER){
                app_ftms_env.Type = CROSS_TYPE;
            }else if(Device_BaseInfo.info.type == INDOORBIKE){
                app_ftms_env.Type = INDOOR_BIKE_TYPE;
            }else if(Device_BaseInfo.info.type == ROWER){
                app_ftms_env.Type = ROWER_TYPE;
            }else{
                
            }
             

            if(BLE_SLAVE_BOX_Scan == 0)
            get_nvds_device_TypeInfo();
            

            if(Device_BaseInfo.info.type == 0){
                if(UartInit_Flag != Step_OutTime_Idle)
                UartInit_Flag = Step_SpeedRange;
            }
            else{
                if(UartInit_Flag != Step_OutTime_Idle)
                UartInit_Flag = Step_ResRange;
            }

            xFitShowAllData.xPowerRange.Maximum = 999;			//
			xFitShowAllData.xPowerRange.Minimum = 0;				//
			xFitShowAllData.xPowerRange.Increment = 10;			//

            xFitShowAllData.xHeartRateRange.Maximum = 250;			
			xFitShowAllData.xHeartRateRange.Minimum = 0;			
			xFitShowAllData.xHeartRateRange.Increment = 1;	
        }
        else if(data[2]==0x02){
            if(UartInit_Flag != Step_SpeedRange)break;
            if(size>7){
                Device_BaseInfo.param.Inch_enable = data[5] & 0x01;
            }
            
            Device_BaseInfo.param.max_speed = data[3];
            Device_BaseInfo.param.min_speed = data[4];

            xFitShowAllData.xSpeedRange.Maximum = Device_BaseInfo.param.max_speed;
            xFitShowAllData.xSpeedRange.Minimum = Device_BaseInfo.param.min_speed;
            xFitShowAllData.xSpeedRange.Increment = 10;

            if(UartInit_Flag != Step_OutTime_Idle)
            UartInit_Flag = Step_InclinationRange;

            OutTimeCount = 0;
        }
        else if(data[2]==0x03){
            if(UartInit_Flag != Step_InclinationRange)break;
            if (size >= 7){
                if(size>7)
                Device_BaseInfo.param.Inch_enable = data[5] & 0x01;

                if (data[4] == 0 || data[4] & 0x80){
                    Device_BaseInfo.param.max_incline = data[3];
                    Device_BaseInfo.param.min_incline = data[4];
                }
                else{
                    Device_BaseInfo.param.max_incline = data[3];
                    Device_BaseInfo.param.min_incline = 0 - data[4];
                }

                xFitShowAllData.xInclinationRange.Maximum = Device_BaseInfo.param.max_incline;			
                xFitShowAllData.xInclinationRange.Minimum = Device_BaseInfo.param.min_incline;				
                xFitShowAllData.xInclinationRange.Increment = 1;		

                xFitShowAllData.xDeviceDataTypePara.Unit = Device_BaseInfo.param.Inch_enable;

                if(UartInit_Flag != Step_OutTime_Idle)
                UartInit_Flag = Step_Complete;
            }
        }
        break;
    case 0x41:
        if(data[2]==0x02){
            if(UartInit_Flag != Step_ResRange)break;
            if(size>5){
                Device_BaseInfo.param.max_level = data[3];
                Device_BaseInfo.param.min_level = 0;
                Device_BaseInfo.param.max_incline = data[4];
                Device_BaseInfo.param.min_incline = 0;

                
                xFitShowAllData.xResistanceRange.Maximum = Device_BaseInfo.param.max_level;
                xFitShowAllData.xResistanceRange.Minimum = 1;				//
			    xFitShowAllData.xResistanceRange.Increment = 1;			//

                Device_BaseInfo.param.max_speed = 20000;
                Device_BaseInfo.param.min_speed = 0;

                xFitShowAllData.xSpeedRange.Maximum = Device_BaseInfo.param.max_speed;
                xFitShowAllData.xSpeedRange.Minimum = Device_BaseInfo.param.min_speed;
                xFitShowAllData.xSpeedRange.Increment = 10;

                if(size>7)
                Device_BaseInfo.param.Inch_enable = data[5] & 0x01;

                xFitShowAllData.xDeviceDataTypePara.Unit = Device_BaseInfo.param.Inch_enable;

                if(UartInit_Flag != Step_OutTime_Idle)
                UartInit_Flag = Step_Complete;
                
            }
        }
        break;
    //------------------------------------------------------------------------------------------------------------------------------------------------//HB-注释
    case 0x51:
        Device_SportInfo.state = data[2];
        if(Device_SportInfo.state == sunCMD_Code_SysSta_RUNNING ||\
            Device_SportInfo.state == sunCMD_Code_SysSta_STOPPING ||\
            Device_SportInfo.state == sunCMD_Code_SysSta_PAUSED)
        {
            Device_SportInfo.speed    = data[3];
            Device_SportInfo.incline  = data[4];
            Device_SportInfo.runtime  = MAKEWORD(data[5], data[6]);
            Device_SportInfo.distance = MAKEWORD(data[7], data[8]);
            Device_SportInfo.calories = MAKEWORD(data[9], data[10]);
            Device_SportInfo.count    = MAKEWORD(data[11], data[12]);
            if(BLE_SLAVE_Heart_Scan == 0 || (BLE_SLAVE_Heart_Scan == 1 && BLE_SLAVE_Heart_ReadyFlag != Yes_Ready))
            Device_SportInfo.heart    = data[13];

            xFitShowAllData.Speed = Device_SportInfo.speed;
            xFitShowAllData.Incline = Device_SportInfo.incline;
            xFitShowAllData.Time = Device_SportInfo.runtime;
            xFitShowAllData.Distance = Device_SportInfo.distance;
            xFitShowAllData.Calories = Device_SportInfo.calories;
            if(BLE_SLAVE_Heart_Scan == 0 || (BLE_SLAVE_Heart_Scan == 1 && BLE_SLAVE_Heart_ReadyFlag != Yes_Ready))
            xFitShowAllData.Heart = Device_SportInfo.heart;
        }else if(Device_SportInfo.state == sunCMD_Code_SysSta_NORMAL){
            // memset(&Device_SportInfo, 0, sizeof(Device_SportInfo));
            // memset(&xFitShowAllData, 0, sizeof(xFitShowAllData));
            ClearSportInfoOnly();
        }
        
        break;
    case 0x42:
        Device_SportInfo.state = data[2];
        if(Device_SportInfo.state == sunCMD_Code_DevSta_RUNNING ||\
            Device_SportInfo.state == sunCMD_Code_DevSta_PAUSED )
        {
            Device_SportInfo.speed = MAKEWORD(data[3], data[4]);
            // Device_SportInfo.InstantaneousSpeed = sport.speed;
            Device_SportInfo.level = data[5];
            // Device_SportInfo.CrossTrainerDataResistanceLevel = sport.level;
            Device_SportInfo.freq  = MAKEWORD(data[6], data[7]);
            // Device_SportInfo.CrossTrainerDataStepCount_perMin = sport.freq;
            // Device_SportInfo.CrossTrainerDataStepCount_Rate = sport.freq;
            if(BLE_SLAVE_Heart_Scan == 0 || (BLE_SLAVE_Heart_Scan == 1 && BLE_SLAVE_Heart_ReadyFlag != Yes_Ready))
            Device_SportInfo.heart = data[8];

            // Device_SportInfo.CrossTrainerDataHeartRate = sport.heart;
            Device_SportInfo.power = MAKEWORD(data[9], data[10]);
            // Device_SportInfo.CrossTrainerDataInstantaneousPower = sport.power;
            Device_SportInfo.incline = data[11];

            xFitShowAllData.Speed = Device_SportInfo.speed;
            xFitShowAllData.Resistance = Device_SportInfo.level;
            xFitShowAllData.Freq = Device_SportInfo.freq;
            if(BLE_SLAVE_Heart_Scan == 0 || (BLE_SLAVE_Heart_Scan == 1 && BLE_SLAVE_Heart_ReadyFlag != Yes_Ready))
            xFitShowAllData.Heart = Device_SportInfo.heart;
            xFitShowAllData.Power = Device_SportInfo.power;
        }
        else if(Device_SportInfo.state == sunCMD_Code_SysSta_NORMAL){
            // memset(&Device_SportInfo, 0, sizeof(Device_SportInfo));
            // memset(&xFitShowAllData, 0, sizeof(xFitShowAllData));
            ClearSportInfoOnly();
        }
        
        break;
    case 0x43:
        
        if(data[2] == sunCMD_Code_DevSta2){
            Device_SportInfo.runtime  = MAKEWORD(data[3], data[4]);
            // Device_SportInfo.CrossTrainerDataElapsedTime = sport.runtime;
            distance       = MAKEWORD(data[5], data[6]);
            if((distance & 0x8000) )
            {
                distance = (distance & 0x7fff) * 10;
            }
            
            Device_SportInfo.distance = distance;
            Device_SportInfo.calories = MAKEWORD(data[7], data[8]);
            
            Device_SportInfo.count    = MAKEWORD(data[9], data[10]);

            xFitShowAllData.Time = Device_SportInfo.runtime;
            xFitShowAllData.Distance = Device_SportInfo.distance;
            xFitShowAllData.Calories = Device_SportInfo.calories;
            xFitShowAllData.Count = Device_SportInfo.count;
        }

        break;
//------------------------------------------------------------------------------------------------------------------------------------------------//HB-注释
    case 0x44:
        if(data[2]==0xC5){//HB_临时注释: get ble name
            fitshow_BLE_Name();
        }
        else if(data[2]==0xc2){//HB_临时注释: sleep ctrl
            sleep_ctrl();
        }else if(data[2]==0xc3){//HB_临时注释:connect sta
            // Update_ConnectFlag();
            Connect_CB_Flag = 1;
        }else if(data[2]==0xc6){//HB_临时注释:get mac
            Data[Data_index++] = 0x02;
            Data[Data_index++] = 0x44;
            Data[Data_index++] = 0xC6;
            
            memcpy((char*)&Data[Data_index],(char *)&MacAddr_Info[0],6);
            Data_index += 6;

            for (uint8_t i = 1; i < Data_index ; i++)
                    fcs ^= Data[i];

            Data[Data_index++] = fcs;
            Data[Data_index++] = 0x03;
            BLE_SendToUart0(Data, Data_index);
        }else if(data[2]==0xc7){//HB_临时注释:change ble name
            Data[Data_index++] = 0x02;
            Data[Data_index++] = 0x60;
            Data[Data_index++] = 0x01;
            for(uint8_t i = 3; i < size-2 ; i++)
            {
                Data[Data_index++] = data[i];
            }	
            for (uint8_t i = 1; i < Data_index ; i++)
                    fcs ^= Data[i];

            Data[Data_index++] = fcs;
            Data[Data_index++] = 0x03;
            prvvFitShowSetting(FitShow_AppConfig,Data,Data_index);

            memset(Data,0,sizeof(Data));
            Data_index = 0;
            Data[Data_index++] = 0x02;
            Data[Data_index++] = 0x60;
            Data[Data_index++] = 0x00;
            Data[Data_index++] = 0x00;
            Data[Data_index++] = 0x60;
            Data[Data_index++] = 0x03;

            prvvFitShowSetting(FitShow_AppConfig,Data,Data_index);
            ResetCPU_Flag = true;

        }
        break;
    
    default:
        break;
    }
}

void sleep_ctrl(void)
{
    gpio_set(0x11,!PULL_HIGH);
    gpio_set(0x12,!PULL_LOW);

    Advertising_Stop();
    Scanning_Stop();


    vFitShowDeepSleepWakeupSet(0x01, PULL_LOW, RISE_EDGE);
    deep_sleep();
}
//================================================================================================================================================//HB-注释
uint16_t usGetDeviceModel(void)
{
	if (prvbNewProtocolFlag == true)
	{
		// return xDeviceParameter.usModel;
	}
	else
	{
		return xFitShowAllData.xDeviceDataTypePara.Model;
	}
    return 99;
}

uint16_t usGetDeviceFactory(void)
{
	if (prvbNewProtocolFlag == true)
	{
		// return xDeviceParameter.usBrand;
	}
	else
	{
		return xFitShowAllData.xDeviceDataTypePara.Factory;
	}
	return 99;
}

uint16_t usGetDeviceType(void)
{
	if (prvbNewProtocolFlag == true)
	{
		// return xDeviceParameter.usType;
	}
	else
	{
		return xFitShowAllData.xDeviceDataTypePara.Type;
	}
    return 99;
}

//================================================================================================================================================//HB-注释
void fitshow_Decryption(uint8_t *data, uint8_t size)
{
    uint8_t Decryption[20];
    uint8_t len=0;
    uint8_t FCS = 0;
    uint8_t temp = 0;
    uint16_t crc = 0xFFFF;


 //   log_buf(data,size);

    Decryption[len++] = 0x02;
    Decryption[len++] = 0xAB;
    temp = data[2];
    Decryption[len++] = temp;

    FCS = 0;
    for (uint8_t i = 0; i < 3; i++)
        FCS ^= Decryption[i];

    Decryption[len++] = FCS;

    Decryption[len++] = data[4];

    FCS = 0;
    for (uint8_t i = 1; i < 5; i++)
        FCS ^= Decryption[i];

    Decryption[len++] = FCS;

    Decryption[len++] = data[5];

    for (int pos = 2; pos < 7; pos++)
    {
        crc ^= (unsigned int)Decryption[pos]; // XOR byte into least sig. byte of crc
        for (int i = 8; i != 0; i--)   // Loop over each bit
        {
            if ((crc & 0x0001) != 0)   // If the LSB is set
            {
                crc >>= 1; // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else // Else LSB is not set
            {
                crc >>= 1;    // Just shift right
            }
        }
    }

    Decryption[len++] = (crc & 0x00ff);
    Decryption[len++] = data[3];
    Decryption[len++] = (crc & 0xff00) >> 8;

    FCS = 0;
    for (uint8_t i = 1; i < 10; i++)
        FCS ^= Decryption[i];

    Decryption[len++] = FCS;
    Decryption[len++] = 0x03;

    // fitshow_write(Decryption, len);
    BLE_SEND_DATA(Decryption, len);
    // app_FFF0S_FFF1_SendData(APP_Disconnect_ID,Decryption, len);

}
//================================================================================================================================================//HB-注释
void fitshow_config_CB(uint8_t *data, uint8_t size){
    prvvFitShowSetting(FitShow_AppConfig,data,size);
}

void Update_ConnectFlag(void){
    if(Connect_CB_Flag > 0 && IO_or_Uart_Mode == FITSHOW_DEVICE_PARAM_DATA_ACQ_UART_MODE)
	{
		uint8_t index = 0;
		uint8_t Data[10];

		Data[index++] = 0x02;
		Data[index++] = 0x44;
		Data[index++] = 0xC3;
		Data[index++] = CURRENT_CNT_STA;
		if(CURRENT_CNT_STA == 1){Data[index++] = 0x86;}
		else if(CURRENT_CNT_STA == 0){Data[index++] = 0x87;}
		Data[index++] = 0x03;
		
		BLE_SendToUart0(Data, index);
		
		Connect_CB_Flag --;
	}
}

extern uint8_t Read_Code_type;
extern  uint16_t Read_Code_Brand ,Read_Code_Model ;
void get_nvds_device_TypeInfo(void)
{
    if(Get_FitShowCodeInfo()!=0)return;

    Advertising_Stop();
    Device_BaseInfo.info.type = Read_Code_type;
    Device_BaseInfo.info.model = Read_Code_Model;
    Device_BaseInfo.info.brand = Read_Code_Brand;

    xFitShowAllData.xDeviceDataTypePara.Type = Device_BaseInfo.info.type;
    xFitShowAllData.xDeviceDataTypePara.Model = Device_BaseInfo.info.model;
    xFitShowAllData.xDeviceDataTypePara.Factory = Device_BaseInfo.info.brand;

    // appm_set_scan_rsp_data();
    Scanning_Start();
}



//================================================================================================================================================//HB-注释




