#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "DMA_t.h"

//const uint8_t data1[4] = {0x01, 0x02, 0x03, 0x04};//将数据存在Flash闪存中，验证将Flash中的数据搬运到RAM运行内存中
uint8_t data1[4] = {0x01, 0x02, 0x03, 0x04};//存储在RAM运行内存中
uint8_t data2[4] = {0};//存储在RAM运行内存中

int main()
{
	OLED_Init();
	DMA_t_Init((uint32_t)data1, (uint32_t)data2, 4);
	
	OLED_ShowString(1, 1, "data1:");
	OLED_ShowHexNum(1, 7, (uint32_t)data1, 8);
	OLED_ShowString(3, 1, "data2:");
	OLED_ShowHexNum(3, 7, (uint32_t)data2, 8);
	
	OLED_ShowHexNum(2, 1, data1[0], 2);
	OLED_ShowHexNum(2, 4, data1[1], 2);
	OLED_ShowHexNum(2, 7, data1[2], 2);
	OLED_ShowHexNum(2, 10, data1[3], 2);
	
	OLED_ShowHexNum(4, 1, data2[0], 2);
	OLED_ShowHexNum(4, 4, data2[1], 2);
	OLED_ShowHexNum(4, 7, data2[2], 2);
	OLED_ShowHexNum(4, 10, data2[3], 2);
	
	
	while(1)
	{
		DMA_t_Transfer();
		
		Delay_ms(1000);
		
		OLED_ShowHexNum(4, 1, data2[0], 2);
		OLED_ShowHexNum(4, 4, data2[1], 2);
		OLED_ShowHexNum(4, 7, data2[2], 2);
		OLED_ShowHexNum(4, 10, data2[3], 2);
		
		Delay_ms(1000);
	
		//若数组data1用const修饰则不能++，这四行代码要注释掉
		data1[0]++;
		data1[1]++;
		data1[2]++;
		data1[3]++;
		
		OLED_ShowHexNum(2, 1, data1[0], 2);
		OLED_ShowHexNum(2, 4, data1[1], 2);
		OLED_ShowHexNum(2, 7, data1[2], 2);
		OLED_ShowHexNum(2, 10, data1[3], 2);
	}
}
