#include "led.h"
#include "WIFI_E103_W02.h"
#include "string.h"
#include "key.h"
#include "oled.h"
#include "delay.h"
#include "sht11.h" 
#include "Data_.h" 
#include "stdio.h" 
#include "LED_DISPLAY.h"
#include "sys.h"
#include "USB_To_Usart.h"	
	void GetSHTDataToGlobParams(uint32_t* sendData)
{
	int sendDataLen=0;
	int sendDataLen1=0;
	
	u8 	buffer[128];
	uint8_t 	ServerUartBufferTx[128];
	
	u16 humi_val, temp_val;
	u8 err = 0, checksum = 0;
	float humi_val_real = 0.0; 
	float temp_val_real = 0.0;
	err += SHT11_Measure(&temp_val, &checksum, TEMP);                  //获取温度
	err += SHT11_Measure(&humi_val, &checksum, HUMI);                  //获取湿度
	if(err != 0){
		
		SHT11_ConReset();
	}else
		{
			SHT11_Calculate(temp_val, humi_val, &temp_val_real, &humi_val_real); //实际温度
			} 
			sprintf((char *)buffer,"temp:%2.2f hum:%2.2f", temp_val_real, humi_val_real);
			
			OLED_ShowString(0,38, buffer,12);
			
			sendData[0]=((int)temp_val_real<<8)+(int)(temp_val_real*10)%10;
			sendData[1]=((int)humi_val_real<<8)+(int)(humi_val_real*10)%10;		
			sendDataLen=getData(ServerUartBufferTx,&sendDataLen1,sendData,3);
			if(!getLinkStatus()){
				E103_W02_SendData(ServerUartBufferTx,sendDataLen);
			}
	
}

int main(void)
{ int num=0;
	uint32_t sendData[3];
	u8 	buffer[128];
  u8 key;           //保存键值
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口	
 	OLED_Init();				//初始化OLED 
	delay_ms(11);
	uart_init(115200);
	SHT11_Config();
	init_E103_w02();//初始化WIFI
	initDataFrame(0xDC20100166146533,0x123456);//初始化数据协议，并设置协议中用到的设备id和登录密码
	LED_DISPLAY_Init();//初始化数码管I/O
	OLED_ShowString(0,0, (unsigned char *)"KEY_UP: Link_Server",12);
	OLED_ShowString(0,14, (unsigned char *)"KEY_1 : Rest_WIFI",12);
	OLED_ShowString(0,26, (unsigned char *)"Link State:",12);
	OLED_Refresh_Gram();//更新显示到OLED	 
 /*
	*/
	select(0);
	show_num(num);
	KEY_Init();       //初始化与按键连接的硬件接口
	while(1)
	{	if(WIFI_USART_RX_STA>0x8000){
				//SendDataToDevice_a(WIFI_USART_RX_BUF, WIFI_USART_RX_STA^0x8000);//打印指令包
				delay_ms(11);
				if(ParseServerData(WIFI_USART_RX_BUF, WIFI_USART_RX_STA^0x8000)){
					if(dataFrame.mOrderConnect[0]<=1){
						if(dataFrame.mOrderConnect[0]==0){
						LED0=1;
						LED0=1;
						OLED_ShowString(0,50, (unsigned char *)"   admin state   ",12);
					}
						else{LED1=0;
						LED0=1;
						OLED_ShowString(0,50, (unsigned char *)"   already pay   ",12);
						}
					}
					else{
						LED0=0;
						LED1=1;
						OLED_ShowString(0,50, (unsigned char *)"   not pay yet   ",12);
					}
					sprintf((char *)buffer,"指令数值:%d\r\n", dataFrame.mOrderConnect[0]);
					SendDataToDevice_a(buffer, 20);
				}
				WIFI_USART_RX_STA = 0;
		}
		key=KEY_Scan(0);		//得到键值
		select(0);
		show_num(num);
	   	if(key)
		{						
			switch(key)
			{				 
				case WKUP_PRES:	//连接到服务器
					//SendDataToDevice_a("连接",4);
				
					OLED_ShowString(0,50, (unsigned char *)"WIFI Set start...",12);
					OLED_Refresh_Gram();//更新显示到OLED	 		
					if(link_server()==0){
						OLED_ShowString(0,50, (unsigned char *)"WIFI Set Success",12);
					}else{
						OLED_ShowString(0,50, (unsigned char *)"WIFI Set Fail",12);
					}
				
					break;
				case KEY1_PRES:	//恢复出厂化设置
					sendData[2]=0;
					//SendDataToDevice_a("重置",4);
					delay_ms(100);	
					OLED_ShowString(0,50, (unsigned char *)"WIFI Rest start...",12);	
				/**/
					OLED_Refresh_Gram();//更新显示到OLED	 	
					if(restE103_W02()==0){
						OLED_ShowString(0,50, (unsigned char *)"WIFI Rest Success",12);
					}else{
						OLED_ShowString(0,50, (unsigned char *)"WIFI Rest Fail",12);
					}
				
					break;
				case KEY0_PRES:
					OLED_ShowString(0,50, (unsigned char *)"   add num ing   ",12);
					num++;
					num=num%4;
					break;
				case KEY2_PRES:	//
					LED0=0;
					LED1=0;
					OLED_ShowString(0,50, (unsigned char *)"   wait for pay   ",12);
					sendData[2]=num;
					break;
			}show_num(num);
		}else{ delay_ms(10); }
		
		if(getLinkStatus()){
			OLED_ShowString(6*11,26, (unsigned char *)"NO",12);
		}else{
			OLED_ShowString(6*11,26, (unsigned char *)"OK",12);
		}
		
		GetSHTDataToGlobParams(sendData);		
		OLED_Refresh_Gram();//更新显示到OLED	 	
	}
}
 



