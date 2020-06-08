#ifndef __WIFI_E103_W02_H
#define __WIFI_E103_W02_H

#include "stm32f4xx.h" 

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_UART5_RX 			1		//使能（1）/禁止（0）串口5接收
	  	
extern u8  WIFI_USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 WIFI_USART_RX_STA;         		//接收状态标记	

void E103_W02_SendData( uint8_t *Data,int l);
void init_E103_w02(void);
char link_server(void);
char restE103_W02(void);
u8 getLinkStatus(void);
#endif

