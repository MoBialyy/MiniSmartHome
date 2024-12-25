#include "DIO.h"
#include "bitwise_operation.h"
#include "Lab1.h"
#include "tm4c123gh6pm.h"
#include "stdbool.h"


void Delay(unsigned long counter) {
    unsigned long i;
    for(i = 0; i < counter; i++);
}

void DIO_Init(void) {
    
    //0x024U --> 0010 0100
    //SYSCTL_RCGCGPIO_R |= 0x00000024U; 
    set_Bit(&SYSCTL_RCGCGPIO_R, 5U);
    set_Bit(&SYSCTL_RCGCGPIO_R, 1U);  // Port B
    set_Bit(&SYSCTL_RCGCGPIO_R, 2U);  //port c
    
    while ( (SYSCTL_PRGPIO_R & (1U << 5)) == 0) {}      // Port F
    while ((SYSCTL_PRGPIO_R & (1U << 1)) == 0) {}       // Port B
    while ( (SYSCTL_PRGPIO_R & (1U << 2)) == 0) {}      // Port C
    
    // This wil be very annoying to write in set_Bit
    GPIO_PORTF_LOCK_R = 0X4C4F434B;
    
    //0X1F --> 0001 1111
    //GPIO_PORTF_CR_R = 0X1F;
    set_Bit(&GPIO_PORTF_CR_R, 0U);
    set_Bit(&GPIO_PORTF_CR_R, 1U);
    set_Bit(&GPIO_PORTF_CR_R, 2U);
    set_Bit(&GPIO_PORTF_CR_R, 3U);
    set_Bit(&GPIO_PORTF_CR_R, 4U);
    
    //0X0E --> 0b00001110, 1U << 1 and 1U << 2 and 1U << 3
    //GPIO_PORTF_DIR_R = 0X0E;
    set_Bit(&GPIO_PORTF_DIR_R, 1U);
    set_Bit(&GPIO_PORTF_DIR_R, 2U);
    set_Bit(&GPIO_PORTF_DIR_R, 3U);
    
    //0X1F --> 0001 1111
    //PIO_PORTF_DEN_R = 0X1F;
    set_Bit(&GPIO_PORTF_DEN_R, 0U);
    set_Bit(&GPIO_PORTF_DEN_R, 1U);
    set_Bit(&GPIO_PORTF_DEN_R, 2U);
    set_Bit(&GPIO_PORTF_DEN_R, 3U);
    set_Bit(&GPIO_PORTF_DEN_R, 4U);
    
    
    //0X11 --> 0001 0001
    //GPIO_PORTF_PUR_R = 0X11;
    set_Bit(&GPIO_PORTF_PUR_R, 0U);
    set_Bit(&GPIO_PORTF_PUR_R, 4U);
}

void DIO_WritePin(sint16 port, sint16 pin, sint16 value){
  switch (port) {
        case 0:  // Port A
            if (value) {
                set_Bit(&GPIO_PORTA_DATA_R, pin);  
            } else {
                clear_Bit(&GPIO_PORTA_DATA_R, pin); 
            }
            break;
        case 1:  // Port B
            if (value) {
                set_Bit(&GPIO_PORTB_DATA_R, pin);  
            } else {
                clear_Bit(&GPIO_PORTB_DATA_R, pin); 
            }
            break;
        case 2:  // Port C
            if (value) {
                set_Bit(&GPIO_PORTC_DATA_R, pin);  
            } else {
                clear_Bit(&GPIO_PORTC_DATA_R, pin); 
            }
            break;
        case 3:  // Port D
            if (value) {
                set_Bit(&GPIO_PORTD_DATA_R, pin);  
            } else {
                clear_Bit(&GPIO_PORTD_DATA_R, pin);
            }
            break;
        case 4:  // Port E
            if (value) {
                set_Bit(&GPIO_PORTE_DATA_R, pin); 
            } else {
                clear_Bit(&GPIO_PORTE_DATA_R, pin);
            }
            break;
        case 5:  // Port F
            if (value) {
                set_Bit(&GPIO_PORTF_DATA_R, pin);  
            } else {
                clear_Bit(&GPIO_PORTF_DATA_R, pin);
            }
            break;
        default:
            break;
    }
}

void DIO_WritePort(sint16 port, uint32 value){
   switch (port) {
        case 0:  // Port A
            GPIO_PORTA_DATA_R = value;
            break;
        case 1:  // Port B
            GPIO_PORTB_DATA_R = value;
            break;
        case 2:  // Port C
            GPIO_PORTC_DATA_R = value;
            break;
        case 3:  // Port D
            GPIO_PORTD_DATA_R = value;
            break;
        case 4:  // Port E
            GPIO_PORTE_DATA_R = value;
            break;
        case 5:  // Port F
            GPIO_PORTF_DATA_R = value;
            break;
        default:
            break;
    }
}

uint32 DIO_ReadPin(sint16 port, sint16 pin){
    
    //There's an alternate where we can directly return the value,
    //but we like this one more as it's more 'clean'
    uint32 pinValue;

    switch (port) {
        case 0:  // Port A
            pinValue = get_Bit(GPIO_PORTA_DATA_R, pin);
            break;
        case 1:  // Port B
            pinValue = get_Bit(GPIO_PORTB_DATA_R, pin);
            break;
        case 2:  // Port C
            pinValue = get_Bit(GPIO_PORTC_DATA_R, pin);
            break;
        case 3:  // Port D
            pinValue = get_Bit(GPIO_PORTD_DATA_R, pin);
            break;
        case 4:  // Port E
            pinValue = get_Bit(GPIO_PORTE_DATA_R, pin);
            break;
        case 5:  // Port F
            pinValue = get_Bit(GPIO_PORTF_DATA_R, pin);
            break;
        default:
            break;
    }
    return pinValue;
}

uint32 DIO_ReadPort(sint16 port){
  
    uint32 portValue = 0;

    switch (port) {
        case 0: 
            portValue = GPIO_PORTA_DATA_R;
            break;
        case 1: 
            portValue = GPIO_PORTB_DATA_R;
            break;
        case 2: 
            portValue = GPIO_PORTC_DATA_R;
            break;
        case 3:  
            portValue = GPIO_PORTD_DATA_R;
            break;
        case 4:   
            portValue = GPIO_PORTE_DATA_R;
            break;
        case 5:   
            portValue = GPIO_PORTF_DATA_R;
            break;
        default:
            portValue = 0;
            break;
    }

    return portValue;
}
  
