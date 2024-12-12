/*
 * @Author: LHB
 * @Date: 2023-08-15 09:37:31
 * @LastEditors: LHB
 * @LastEditTime: 2024-04-12 10:50:48
 * @Description: 
 * 1
 */
#include <string.h>
#include "Match_hb.h"
#include "nvds.h"
#include "app_init.h"
#include "app_scan.h"
#include "uart.h"

extern unsigned short  Heart_SRC_ID ,BOX_SRC_ID ;
extern unsigned short HB_FFF2_Handle_val,HB_Read2A29_Handle_val,HB_Read2A24_Handle_val,HB_Read2A2A_Handle_val,HB_Read2ACC_Handle_val,HB_Read2A37_Handle_val;
extern uint8_t HB_ADDR_TempBuffer[6];

unsigned char step_flag = 0,Disconnect_will_flag = 0;
unsigned char Con_Slave_Step = 0;
extern bool Checker_Box_Flag,Get_NVDS_SuccessFlag;

extern unsigned char BLE_SLAVE_Box_ReadyFlag,BLE_SLAVE_Heart_ReadyFlag;
bool Disconnect_slave_afterSave = false;

SaveMacAddr_t BoxAddr;

uint8_t Get_BoxADDR_Val[6];

bool Step_Box_ScanEnabled,Step_Heart_ScanEnabled,Box_ScanBusy_Flag = false,Slave_ConnectComplete = false, ResetAD_Flag = true,resetAD_bit = false;
uint8_t Master_SlaveScanStep = step_box_need;

extern uint8_t UartInit_Flag;
//================================================================================================================================================//HB-注释
extern void reset_mcu(void);
//================================================================================================================================================//HB-注释

static void prvvBoxAddrSave(void)
{
    uint8_t ret = 0;

    BoxAddr.usSUM_Check = usSumUp((uint8_t*)&BoxAddr.arrMAC_Address[0], sizeof(SaveMacAddr_t) - 2);
    ret = nvds_put(NVDE_TAG_BOXADDR_DATA, sizeof(SaveMacAddr_t), (uint8_t *)&BoxAddr);
    uart_printf("-->ret:%d\r\n",ret);//HB-注释
}

static void prvvBoxAddrDel(void)
{
    uint8_t ret = 0;

    ret = nvds_del(NVDE_TAG_BOXADDR_DATA);
    uart_printf("-->ret:%d\r\n",ret);//HB-注释
}

void DelMatchInfo_CB(void){
    prvvBoxAddrDel();
    for(uint16_t i=0;i<600;i++)
    {
        uart_printf(".");
    }
    reset_mcu();
}

char Get_BoxAddr_NVDS(void) 
{
	char res = 1;
	nvds_tag_len_t Len = sizeof(SaveMacAddr_t);
	uint8_t data[sizeof(SaveMacAddr_t)];
	memset(data, 0, sizeof(SaveMacAddr_t));
	
	uint8_t ret = nvds_get(NVDE_TAG_BOXADDR_DATA, &Len, data);
	
    
	SaveMacAddr_t *pADDR_Buff = (SaveMacAddr_t *)data;
	
	uint16_t CheckValue = 0x0000;
	CheckValue = usSumUp((uint8_t*)&pADDR_Buff->arrMAC_Address, sizeof(SaveMacAddr_t) - 2);

    
	if (CheckValue != pADDR_Buff->usSUM_Check)
	{      
			return res;
	}
	
    CheckValue = 0;

    for(uint8_t i=0;i<6;i++)
    {

        CheckValue += pADDR_Buff->arrMAC_Address[5-i];
    }


    
	if(CheckValue>0)
	{
        for(uint8_t i=0;i<6;i++)
        {
            Get_BoxADDR_Val[i] = pADDR_Buff->arrMAC_Address[i];
        }
		res = 0;
	}

	return res;
}

void Del_BoxAddr_NVDS(void)
{
    uint8_t ret = 0;
			
    ret = nvds_del(NVDE_TAG_BOXADDR_DATA);
    uart_printf("-->ret:%d\r\n",ret);//HB-注释

}

bool get_nvds_box_addr_CB(void){
    if(!Get_BoxAddr_NVDS()){

        Get_NVDS_SuccessFlag = true;
        BLE_SLAVE_Box_ReadyFlag = Saved_Ready;
    }else{
        
        Get_NVDS_SuccessFlag = false;
        BLE_SLAVE_Box_ReadyFlag = No_Ready;
    }
    return Get_NVDS_SuccessFlag;
}
//================================================================================================================================================//HB-注释

unsigned char Check_FitShowPro_CB(unsigned char step){
    if(Checker_Box_Flag == true){step_flag = 9;return 0;}
    switch (step)
    {
    case 1:
        appm_read_info_req(0,HB_Read2A29_Handle_val);

        
        break;
    case 3:

        appm_read_info_req(0,HB_Read2A24_Handle_val);
        
        break;
    case 5:

        appm_read_info_req(0,HB_Read2ACC_Handle_val);
        
        break;
    case 7:

        appm_read_info_req(0,HB_Read2A2A_Handle_val);
        
        break;

    default:
        break;
    }

    return 0;
}

void Transform_SportData_CB(void){

}

void GetReadValData(const unsigned lenth,const unsigned char *data){

    if(step_flag == 1 && Disconnect_will_flag==0){
        step_flag = 3;



        if(data[4] == 0x48 && data[5] == 0x4F && data[6] == 0x57){
       
            
        }else {
         
            // Disconnect_will_flag++;
        }
        
        
    }else if(step_flag == 3 && Disconnect_will_flag==0){
        step_flag = 5;
        
        if(data[1] == 0x73){
         
        }else {
          
            // Disconnect_will_flag++;
        }
        
        
    }else if(step_flag == 5 && Disconnect_will_flag==0){
        step_flag = 7;
       

        if(data[5] == 0x1C){
         
        }else if(data[5] != 0x1C){
          
            Disconnect_will_flag++;
        }
        
        
    }else if(step_flag == 7 && Disconnect_will_flag==0){
        step_flag = 9;
       

        if(lenth == 6){
        
            if(data[0] == 0x39 && data[1] == 0x38 && data[2] == 0x31 && data[3] == 0x32 && data[4] == 0x30 && data[5] == 0x39){
            
                if(Disconnect_will_flag == 0){
                    memcpy(&BoxAddr.arrMAC_Address[0],&HB_ADDR_TempBuffer[0],6);
                    memcpy(&Get_BoxADDR_Val[0],&HB_ADDR_TempBuffer[0],6);
                    prvvBoxAddrSave();
                    
                    if(get_nvds_box_addr_CB() == true){
                        BLE_SLAVE_Box_ReadyFlag = Yes_Ready;
                        // Disconnect_slave_afterSave = true;
                        // Box_ScanBusy_Flag = false;
                    }else{
                        
                    }
                }
                
            }
            else{
             
                Disconnect_will_flag++;
            }
        }else{
        
            Disconnect_will_flag++;
        }
        
        
    }
    else{
        // return 9;
    }

    
}
//================================================================================================================================================//HB-注释


void Master_SlaveConnectTaskList(void){
    bool print_enable = false;
    switch(Master_SlaveScanStep){
        case step_box_need://HB_临时注释 box need
        if(print_enable == true)
            
            if(BLE_SLAVE_BOX_Scan == 1){
                Step_Box_ScanEnabled = true;//      
            }else{
                Step_Box_ScanEnabled = false;
                
            }
        case step_heart_need://HB_临时注释 heart need
        
            if(BLE_SLAVE_Heart_Scan == 1){
                if(Box_ScanBusy_Flag ==true){
                    Step_Heart_ScanEnabled = false;//      
                }else{
                    if(BLE_SLAVE_Heart_ReadyFlag == Yes_Ready){
                        Step_Heart_ScanEnabled = false;
                    }else{
                        Step_Heart_ScanEnabled = true;//
                    }
                }               
            }else{
                Step_Heart_ScanEnabled = false;//   
            }
            if(print_enable == true)
            uart_printf("-->Master_SlaveScanStep:%d,Step_Heart_ScanEnabled:%d\r\n",step_heart_need,Step_Heart_ScanEnabled);//HB-注释
        //-----------------------------------------------------------Box-------------------------------------------------------------------------------------//HB-注释
        case step_box_search://HB_临时注释 box ad search
        if(print_enable == true)
        uart_printf("-->Master_SlaveScanStep:%d\r\n",step_box_search);//HB-注释
            if(Step_Box_ScanEnabled == true){
                if(BLE_SLAVE_Box_ReadyFlag != Yes_Ready)
                Box_ScanBusy_Flag = true;//      %%%%%%%%%%%%%%%%%% 
            }else{
                Master_SlaveScanStep = step_heart_search;//
                break;
            }
        case step_box_connect://HB_临时注释 box connect 
        if(print_enable == true)
        uart_printf("-->Master_SlaveScanStep:%d\r\n",step_box_connect);//HB-注释
            if(BLE_SLAVE_Box_ReadyFlag == Yes_Ready){
                Box_ScanBusy_Flag = false;//   
            }else{break;}
        //-----------------------------------------------------------Heart-------------------------------------------------------------------------------------//HB-注释
        case step_heart_search://HB_临时注释 heart ad search
        if(print_enable == true)
        uart_printf("-->Master_SlaveScanStep:%d\r\n",step_heart_search);//HB-注释
            if(Step_Heart_ScanEnabled == true ){
                //
            }else{
                Master_SlaveScanStep = step_master_AD;//
                break;
            }
        
        case step_heart_connect://HB_临时注释 heart connect
        if(print_enable == true)
        uart_printf("-->Master_SlaveScanStep:%d\r\n",step_heart_connect);//HB-注释
            if(BLE_SLAVE_Heart_ReadyFlag == Yes_Ready){
            
                }else{break;}
        
        //------------------------------------------------------------------------------------------------------------------------------------------------//HB-注释
        case step_master_AD://HB_临时注释 master AD
        if(print_enable == true)
        uart_printf("-->Master_SlaveScanStep:%d\r\n",step_master_AD);//HB-注释
            if(BLE_SLAVE_BOX_Scan == 1 && BLE_SLAVE_Heart_Scan == 1)
            {
                if((BLE_SLAVE_Heart_ReadyFlag == Yes_Ready)  && (BLE_SLAVE_Box_ReadyFlag == Yes_Ready)){
                    // uart_printf("-************appm_stop_scaning----------------------*****************->line:%d\r\n",__LINE__);//HB-注释
                    appm_stop_scaning();
                    Slave_ConnectComplete = true;
                    if(ResetAD_Flag == true){

                    resetAD_bit = true;
                    UartInit_Flag = 0;
                    
                    ResetAD_Flag = false;
                }
                }
            }else if(BLE_SLAVE_BOX_Scan == 1 && BLE_SLAVE_Heart_Scan == 0)
            {
                if((BLE_SLAVE_Box_ReadyFlag == Yes_Ready)){
                    appm_stop_scaning();
                    Slave_ConnectComplete = true;
                    if(ResetAD_Flag == true){

                    resetAD_bit = true;
                    UartInit_Flag = 0;

                    ResetAD_Flag = false;
                }
                    // uart_printf("-************appm_stop_scaning----------------------*****************->line:%d\r\n",__LINE__);//HB-注释
                }
            }else if(BLE_SLAVE_BOX_Scan == 0 && BLE_SLAVE_Heart_Scan == 1)//
            {
                if((BLE_SLAVE_Heart_ReadyFlag == Yes_Ready)){
                    appm_stop_scaning();
                    Slave_ConnectComplete = true;
                    // uart_printf("-************appm_stop_scaning----------------------*****************->line:%d\r\n",__LINE__);//HB-注释
                }
            }else{
                appm_stop_scaning();
                Slave_ConnectComplete = true;
                
            }

            Master_SlaveScanStep = step_box_need;
        break;

        default:
        break;
    }
}













