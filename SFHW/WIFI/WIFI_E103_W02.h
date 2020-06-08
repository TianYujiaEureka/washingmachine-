#ifndef __WIFI_E103_W02_H
#define __WIFI_E103_W02_H

#include "stm32f4xx.h" 

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_UART5_RX 			1		//ʹ�ܣ�1��/��ֹ��0������5����
	  	
extern u8  WIFI_USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 WIFI_USART_RX_STA;         		//����״̬���	

void E103_W02_SendData( uint8_t *Data,int l);
void init_E103_w02(void);
char link_server(void);
char restE103_W02(void);
u8 getLinkStatus(void);
#endif

