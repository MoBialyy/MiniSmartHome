#include "bitwise_operation.h"

// Bit manipulation
void set_Bit(volatile ulong32 *Register, uint32 Bit) {
    *Register |= (1 << Bit);
}

void clear_Bit(volatile ulong32  *Register, uint32 Bit) {
    *Register &= ~(1 << Bit);
}

int get_Bit(volatile ulong32 Register, uint32 Bit) {
    return (Register >> Bit) & 1;
}

void toggle_Bit(volatile ulong32 *Register, uint32 Bit) {
    *Register ^= (1 << Bit);
}