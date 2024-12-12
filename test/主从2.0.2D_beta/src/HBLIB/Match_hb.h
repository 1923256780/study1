/*
 * @Author: LHB
 * @Date: 2024-04-02 17:17:28
 * @LastEditors: LHB
 * @LastEditTime: 2024-04-12 10:56:14
 * @Description: 
 * 让花成花,让树成树
 */


#ifndef Match_hb_H__
#define Match_hb_H__

#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"

#define BLE_SLAVE_BOX_Scan 0 //HB_临时注释 扫描蓝牙盒子从机
#define BLE_SLAVE_Heart_Scan 0 //HB_临时注释 扫描心率手环

enum{
    step_box_need = 1,
    step_heart_need,
    step_box_search,
    step_box_connect,
    step_heart_search,
    step_heart_connect,
    
    step_master_AD,
};

typedef enum {
    No_Ready = 0,
    Yes_Ready = 1,
    Readying = 2,
    step_2_ready = 7,

    Saved_Ready = 3,
    Saved_Readying = 5,

    None_Device_Ready = 1>>1,
    Heart_Device_Ready = 1<<0,
    BOX_Device_Ready = 1<<1,
    BOX_Device_SavedReady = 3,
    BOX_Device_STEP_2_Ready = 7,
}BLE_SLAVE_Ready_t;

typedef struct SaveMacAddr
{
    uint16_t usSUM_Check;      

    char arrMAC_Address[6];

} SaveMacAddr_t; 

void GetReadValData(const unsigned lenth,const unsigned char *data);
unsigned char Check_FitShowPro_CB(unsigned char step);
char Get_BoxAddr_NVDS(void) ;
void Del_BoxAddr_NVDS(void);
bool get_nvds_box_addr_CB(void);
void Master_SlaveConnectTaskList(void);
void DelMatchInfo_CB(void);

#endif
