#include "stm32f10x.h"                  // Device header

uint16_t size;

void DMA_t_Init(uint32_t sourceAddr1, uint32_t destinationAddr2, uint16_t transferSize)
{
	//将要写入计数器的值拷贝一份，给后续的计数器重新赋值函数使用
	size = transferSize;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitTypeDef DMA_InitStructure;
	//要运输的外设地址（注意，外设地址也可填寄存器地址，实际上无论是外设还是寄存器都是寄存器）
	DMA_InitStructure.DMA_PeripheralBaseAddr = sourceAddr1;
	//外设向外传输数据的数据宽度，以字节为单位传输	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//外设数据搬运后是否执行地址自增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	//要运输的寄存器地址（注意，寄存器地址也可填外设地址，实际上无论是外设还是寄存器都是寄存器）
	//DMA_PeripheralBaseAddr、DMA_InitStructure.DMA_MemoryBaseAddr
	//他们只是传输的站点名字而已
	DMA_InitStructure.DMA_MemoryBaseAddr = destinationAddr2;
	//寄存器的数据宽度
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//寄存器数据搬运后是否执行地址自增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//数据传输方向，指定外设站点地址是源端还是目的地，本程序选择为源端，即数据从外设站点传输到寄存器站点
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	//数据传输计数器，其是16位，取值范围为0~65535
	DMA_InitStructure.DMA_BufferSize = transferSize;
	//选择是否为寄存器到寄存器的数据转运，即选择软件触发或硬件触发，寄存器到寄存器就是软件触发（软件触发条件下一直会有软件触发，无须用代码重复触发）
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
	//传输计数器是否自动重装，由于本程序是软件触发，不可选择这个重装循环模式，
	//选择正常模式，当计数器减为0时DMA不再搬运数据（尽管Cmd开关仍然ENABLE着），即该模式下只搬运一次数据
	//要进行第二次数据搬运，需给计数器重新赋值
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//DMA请求优先级,因为DMA对外联系的总通道只有1个，所以当有多个DMA通道有转运需求时
	//需要有优先级来分配总通道的使用权限
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	//选择DMA1，通道1，由于本程序是软件触发，选择任何的通道都开源）
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);	
	//小细节：调用Init函数之后不马上开启DMA进行转运，等调用下面的转运函数才开始一次转运
	DMA_Cmd(DMA1_Channel1, DISABLE);
}

//调用此函数，给用完（减为0）的计数器重新赋值，进行第二次数据搬运工作
void DMA_t_Transfer()
{
	//重新赋值需要关闭DMA
	DMA_Cmd(DMA1_Channel1, DISABLE);
	//选择要重新写计数值的通道并写入其重新写入的值
	DMA_SetCurrDataCounter(DMA1_Channel1, size);
	//赋值完成后，打开DMA再次进行数据搬运工作
	DMA_Cmd(DMA1_Channel1, ENABLE);
	//等待转运完成
		//循环判断条件中的函数用来获取通道1的搬运完成标志位，搬运完成返回SET
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) != SET);
	//清楚标志位
	DMA_ClearFlag(DMA1_FLAG_TC1);
}

