#include "LED_DISPLAY.h"

// 
//LED_DISPLAY IO初始化
void LED_DISPLAY_Init(void)
{    	 
  GPIO_InitTypeDef GPIO_InitStructure;
	//使能时钟A、B、C、E、G
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB
	|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG,ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8;
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化I/O PA6,PA8

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; 
	GPIO_Init(GPIOB,&GPIO_InitStructure);//初始化I/O PB6,PB7
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_8|GPIO_Pin_9;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);//初始化I/O PC3,PC8,PC9
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
	GPIO_Init(GPIOE,&GPIO_InitStructure);//初始化I/O PE5,PE6
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_15;
	GPIO_Init(GPIOG,&GPIO_InitStructure);//初始化I/O PG6,PG7,PG15
	
		//关掉数码管位开关和熄灭数码管所有led灯
		GPIO_SetBits(GPIOA,GPIO_Pin_6|GPIO_Pin_8);
		GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);
		GPIO_SetBits(GPIOC,GPIO_Pin_3|GPIO_Pin_8|GPIO_Pin_9);
		GPIO_SetBits(GPIOE,GPIO_Pin_5|GPIO_Pin_6);
		GPIO_SetBits(GPIOG,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_15);

}
/**
控制位开关
id代表开启哪个位
*/
void select(int id){
	if(id>3||id<0){
		return;
	}
	//置高，关掉4位数码管
		GPIO_SetBits(GPIOB,GPIO_Pin_6);
		GPIO_SetBits(GPIOE,GPIO_Pin_5|GPIO_Pin_6);
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
	switch (id){
			case 0:
				GPIO_ResetBits(GPIOB,GPIO_Pin_6);
				break;
			case 1:
				GPIO_ResetBits(GPIOE,GPIO_Pin_5);
				break;
			case 2:
				GPIO_ResetBits(GPIOA,GPIO_Pin_6);
				break;
			case 3:
				GPIO_ResetBits(GPIOE,GPIO_Pin_6);
				break;

		}
}
/**
在数码管显示数字
num显示的数字0--9
*/
void show_num(int num){
	if(num<0||num>9){
		return;
	}
	//熄灭数码管所有led灯
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
		GPIO_SetBits(GPIOB,GPIO_Pin_7);
		GPIO_SetBits(GPIOC,GPIO_Pin_3|GPIO_Pin_8|GPIO_Pin_9);
		GPIO_SetBits(GPIOG,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_15);
	switch (num % 10)

		{

			case 0:
				GPIO_ResetBits(GPIOG,GPIO_Pin_7);//A
				GPIO_ResetBits(GPIOA,GPIO_Pin_8);//B
				GPIO_ResetBits(GPIOC,GPIO_Pin_9);//C
				GPIO_ResetBits(GPIOB,GPIO_Pin_7);//D
				GPIO_ResetBits(GPIOG,GPIO_Pin_15);//E
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);//F
				break;

			case 1:

				GPIO_ResetBits(GPIOC,GPIO_Pin_3);//F
				GPIO_ResetBits(GPIOG,GPIO_Pin_15);//E
				break;

			case 2:

		
				GPIO_ResetBits(GPIOG,GPIO_Pin_7);//A
				GPIO_ResetBits(GPIOA,GPIO_Pin_8);//B
				GPIO_ResetBits(GPIOG,GPIO_Pin_6);//G
				GPIO_ResetBits(GPIOG,GPIO_Pin_15);//E
				GPIO_ResetBits(GPIOB,GPIO_Pin_7);//D
				break;

			case 3:
				GPIO_ResetBits(GPIOG,GPIO_Pin_7);//A
				GPIO_ResetBits(GPIOA,GPIO_Pin_8);//B
				GPIO_ResetBits(GPIOG,GPIO_Pin_6);//G
				GPIO_ResetBits(GPIOC,GPIO_Pin_9);//C
				GPIO_ResetBits(GPIOB,GPIO_Pin_7);//D

				break;

			case 4:
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);//F
				GPIO_ResetBits(GPIOA,GPIO_Pin_8);//B
				GPIO_ResetBits(GPIOG,GPIO_Pin_6);//G
				GPIO_ResetBits(GPIOC,GPIO_Pin_9);//C
				break;

			case 5:
				GPIO_ResetBits(GPIOG,GPIO_Pin_7);//A
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);//F
				GPIO_ResetBits(GPIOG,GPIO_Pin_6);//G
				GPIO_ResetBits(GPIOC,GPIO_Pin_9);//C
				GPIO_ResetBits(GPIOB,GPIO_Pin_7);//D
				break;

			case 6:
				GPIO_ResetBits(GPIOG,GPIO_Pin_7);//A
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);//F
				GPIO_ResetBits(GPIOG,GPIO_Pin_15);//E
				GPIO_ResetBits(GPIOB,GPIO_Pin_7);//D
				GPIO_ResetBits(GPIOC,GPIO_Pin_9);//C
				GPIO_ResetBits(GPIOG,GPIO_Pin_6);//G
				break;

			case 7:
				GPIO_ResetBits(GPIOG,GPIO_Pin_7);//A
				GPIO_ResetBits(GPIOA,GPIO_Pin_8);//B
				GPIO_ResetBits(GPIOC,GPIO_Pin_9);//C
				break;

			case 8:
				GPIO_ResetBits(GPIOG,GPIO_Pin_7);//A
				GPIO_ResetBits(GPIOA,GPIO_Pin_8);//B
				GPIO_ResetBits(GPIOC,GPIO_Pin_9);//C
				GPIO_ResetBits(GPIOB,GPIO_Pin_7);//D
				GPIO_ResetBits(GPIOG,GPIO_Pin_15);//E
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);//F
				GPIO_ResetBits(GPIOG,GPIO_Pin_6);//G
				break;

			case 9:
				GPIO_ResetBits(GPIOG,GPIO_Pin_7);//A
				GPIO_ResetBits(GPIOA,GPIO_Pin_8);//B
				GPIO_ResetBits(GPIOC,GPIO_Pin_9);//C
				GPIO_ResetBits(GPIOB,GPIO_Pin_7);//D
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);//F
				GPIO_ResetBits(GPIOG,GPIO_Pin_6);//G
				break;
		}

}



