/**
 ****************************************************************************************
 *
 * @file uart.h
 *
 * @brief UART Driver for HCI over UART operation.
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef _UART2_H_
#define _UART2_H_

#include <stdbool.h>          // standard boolean definitions
#include <stdint.h>           // standard integer functions
#include "uart.h"

#define BASEADDR_UART2                                      0x00806a00
//addUART2_Reg0x0
#define UART2_REG0X0                                        *((volatile unsigned long *) (BASEADDR_UART2+0x0*4))
#define POS_UART2_REG0X0_TX_ENABLE                          0
#define POS_UART2_REG0X0_RX_ENABLE                          1
#define POS_UART2_REG0X0_IRDA                               2
#define POS_UART2_REG0X0_LEN                                3
#define POS_UART2_REG0X0_PAR_EN                             5
#define POS_UART2_REG0X0_PAR_MODE                           6
#define POS_UART2_REG0X0_STOP_LEN                           7
#define POS_UART2_REG0X0_CLK_DIVID                          8

//addUART2_Reg0x1
#define UART2_REG0X1                                         *((volatile unsigned long *) (BASEADDR_UART2+0x1*4))
#define POS_UART2_REG0X1_TX_FIFO_THRESHOLD                   0
#define POS_UART2_REG0X1_RX_FIFO_THRESHOLD                   8
#define POS_UART2_REG0X1_RX_STOP_DETECT_TIME                 16

//addUART2_Reg0x2
#define UART2_REG0X2                                         *((volatile unsigned long *) (BASEADDR_UART2+0x2*4))
#define POS_UART2_REG0X2_TX_FIFO_COUNT                       0
#define POS_UART2_REG0X2_RX_FIFO_COUNT                       8
#define POS_UART2_REG0X2_TX_FIFO_FULL                        16
#define POS_UART2_REG0X2_TX_FIFO_EMPTY                       17
#define POS_UART2_REG0X2_RX_FIFO_FULL                        18
#define POS_UART2_REG0X2_RX_FIFO_EMPTY                       19
#define POS_UART2_REG0X2_FIFO_WR_READY                       20
#define POS_UART2_REG0X2_FIFO_RD_READY                       21

//addUART2_Reg0x3
#define UART2_REG0X3                                         *((volatile unsigned long *) (BASEADDR_UART2+0x3*4))
#define POS_UART2_REG0X3_UART_TX_FIFO_DIN                    0
#define POS_UART2_REG0X3_UART_RX_FIFO_DOUT                   8

//addUART2_Reg0x4
#define UART2_REG0X4                                         *((volatile unsigned long *) (BASEADDR_UART2+0x4*4))
#define POS_UART2_REG0X4_TX_FIFO_NEED_WRITE_MASK             0
#define POS_UART2_REG0X4_RX_FIFO_NEED_READ_MASK              1
#define POS_UART2_REG0X4_RX_FIFO_OVER_FLOW_MASK              2
#define POS_UART2_REG0X4_UART_RX_PARITY_ERR_MASK             3
#define POS_UART2_REG0X4_UART_RX_STOP_ERR_MASK               4
#define POS_UART2_REG0X4_UART_TX_STOP_END_MASK               5
#define POS_UART2_REG0X4_UART_RX_STOP_END_MASK               6
#define POS_UART2_REG0X4_UART_RXD_WAKEUP_MASK                7

//addUART2_Reg0x5
#define UART2_REG0X5                                         *((volatile unsigned long *) (BASEADDR_UART2+0x5*4))
#define POS_UART2_REG0X5_TX_FIFO_NEED_WRITE                  0
#define POS_UART2_REG0X5_RX_FIFO_NEED_READ                   1
#define POS_UART2_REG0X5_RX_FIFO_OVER_FLOW                   2
#define POS_UART2_REG0X5_UART_RX_PARITY_ERROR                3
#define POS_UART2_REG0X5_UART_RX_STOP_ERROR                  4
#define POS_UART2_REG0X5_UART_TX_STOP_END                    5
#define POS_UART2_REG0X5_UART_RX_STOP_END                    6
#define POS_UART2_REG0X5_UART_RXD_WAKEUP                     7

//addUART2_Reg0x6
#define UART2_REG0X6                                         *((volatile unsigned long *) (BASEADDR_UART2+0x6*4))
#define POS_UART2_REG0X6_FLOW_CTL_LOW_CNT                    0
#define POS_UART2_REG0X6_FLOW_CTL_HIGH_CNT                   8
#define POS_UART2_REG0X6_FLOW_CONTROL_ENA                    16
#define POS_UART2_REG0X6_RTS_POLARITY_SEL                    17
#define POS_UART2_REG0X6_CTS_POLARITY_SEL                    18

//addUART2_Reg0x7
#define UART2_REG0X7                                         *((volatile unsigned long *) (BASEADDR_UART2+0x7*4))
#define POS_UART2_REG0X7_UART_WAKE_COUNT                     0
#define POS_UART2_REG0X7_UART_TXD_WAIT_CNT                   10
#define POS_UART2_REG0X7_UART_RXD_WAKE_EN                    20
#define POS_UART2_REG0X7_UART_TXD_WAKE_EN                    21
#define POS_UART2_REG0X7_RXD_NEGEDGE_WAKE_EN                 22




#define UART2_FIFO_MAX_COUNT   128



int uart2_printf(const char *fmt,...);
int uart2_printf_null(const char *fmt,...);


void uart2_init(uint32_t baudrate);
void uart2_isr(void);

void uart2_send(void *buff, int len);



/// @} UART
#endif /* _UART_H_ */
