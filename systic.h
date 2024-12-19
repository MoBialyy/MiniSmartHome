#ifndef SYSTIC_H_INCLUDED
#define SYSTIC_H_INCLUDED
#include "stdbool.h"
#include "lab1.h"
#define LED_RED    (1U << 1)  
#define LED_BLUE   (1U << 2)  
#define LED_GREEN  (1U << 3) 

void systicInit(ulong32 reloadValue);
void systicInitTime(float32 time);
void systicWait(void);
void systicEnable(void);
void systicDisable(void);
void systicPeriodSet(uint32 period);
uint32 getSysticPeriod(void);
uint32 getSysticPeriodTime(void);
uint32 getSysticValue(void);
bool systicTimeOut(void);
void toggle500ms(void);

#endif