#ifndef DIO_H_INCLUDED
#define DIO_H_INCLUDED
#include "Lab1.h"

void Delay(unsigned long counter);
void DIO_Init(void);
void DIO_WritePin(sint16 port, sint16 pin, sint16 value);
void DIO_WritePort(sint16 port, uint32 value);
uint32 DIO_ReadPin(sint16 port, sint16 pin); 
uint32 DIO_ReadPort(sint16 port); 

#endif