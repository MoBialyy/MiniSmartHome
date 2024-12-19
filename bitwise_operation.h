#ifndef BITWISE_OPERATION_H_INCLUDED
#define BITWISE_OPERATION_H_INCLUDED
#include "Lab1.h"

void set_Bit(volatile ulong32  *Register, uint32 Bit);  
void clear_Bit(volatile ulong32 *Register, uint32 Bit);
int get_Bit(volatile ulong32 Register, uint32 Bit);
void toggle_Bit(volatile ulong32 *Register, uint32 Bit);

#endif 


/*
I tried this
void set_Bit(volatile ulong32_ptr  Register, uint32 Bit); 
but my compiler seems not to read/define/allocate the ulong32_ptr although it's defined in Lab1.h 
*/