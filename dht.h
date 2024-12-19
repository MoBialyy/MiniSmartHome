#ifndef ADC_H
#define ADC_H
#include <stdint.h>
#include "Lab1.h"

void ADC1_Init();
float32 ADC1_ReadValue();
void ToggleBuzzerOn(void);
void ToggleBuzzerOff(void);


#endif