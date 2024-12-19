#ifndef DOOR_H_INCLUDED
#define DOOR_H_INCLUDED
#include "Lab1.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "bitwise_operation.h"
#include "DIO.h"

void door_init(void);
void GPIOPortB_Handler(void);
void enable_interrupts(void) ;
void disable_interrupts(void);

#endif