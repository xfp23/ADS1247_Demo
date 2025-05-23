#include "M_Flag.h"

volatile User_Time_t UserTim;

ADS1247_Handle_t ads1247 = NULL; // 句柄

uint32_t ADS_adc = 0;

/*******************************************************************************
Name			:fputc
Syntax			: printf(...,...);
Sync/Async		: -
Reentrancy		: -
Func			: 串口重定向
Parameters(reg)	:  -
Parameters(conf):  -
Return value	:  -
Description		:  此函数不做调用
Call By			:
Date			: 2025/03/04
******************************************************************************/
int fputc(int ch, FILE *f)
{
	static uint16_t delayUtil = 0;

	HAL_UART_Transmit(&hlpuart1, (unsigned char *)&ch, 1, 0xFFFF);

	while (__HAL_UART_GET_FLAG(&hlpuart1, UART_FLAG_TC) != SET)
	{
		if (++delayUtil > 2000)
		{
			delayUtil = 0;
			break;
		}
	};

	return ch;
}
