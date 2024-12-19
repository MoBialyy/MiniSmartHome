#ifndef GPTM_H_INCLUDED
#define GPTM_H_INCLUDED
#include "Lab1.h"
#include <stdbool.h>

void timer0A_delay(uint32 time);
void SysCtlPeripheralEnable(uint32 ui32Peripheral);
bool SysCtlPeripheralReady(uint32 ui32Peripheral);
void Timer0Configure(uint32 ui32Config);
void Timer0LoadSet(uint32 ui32Value);
void Timer0Enable(void);
void Timer0Disable(void);

#endif