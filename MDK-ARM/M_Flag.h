#ifndef M_FLAG_H
#define M_FLAG_H

#include "main.h"
#include "ADS1247.h"
#include "stdio.h"
#include "stdlib.h"
#include "usart.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifndef ON
#define ON (1)
#endif

#ifndef OFF
#define OFF (0)
#endif
	
typedef union {
	struct {
	uint8_t sysTim_1ms :1;
	uint8_t sysTim_10ms :1;
	uint8_t sysTim_100ms :1;
	uint8_t sysTim_500ms :1;
	uint8_t sysTim_1000ms :1;
	}bytes;
}User_Time_t;

extern volatile User_Time_t UserTim; 

extern ADS1247_Handle_t ads1247;

extern uint32_t ADS_adc;

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !M_FLAG_H
