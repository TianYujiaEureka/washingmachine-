#ifndef __LED_DISPLAY_H
#define __LED_DISPLAY_H
#include "sys.h"
	


//LED�˿ڶ���
#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1	 

void LED_DISPLAY_Init(void);//��ʼ��		 	
void show_num(int num);
void select(int id);
#endif


