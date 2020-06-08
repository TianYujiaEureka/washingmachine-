#ifndef __LED_DISPLAY_H
#define __LED_DISPLAY_H
#include "sys.h"
	


//LED端口定义
#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1	 

void LED_DISPLAY_Init(void);//初始化		 	
void show_num(int num);
void select(int id);
#endif


