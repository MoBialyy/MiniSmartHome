#include "door.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "systic.h"
#include "UART.H"

void door_init(void) {
    
    // Configure PB2 as input with pull-up resistor
    GPIO_PORTB_DIR_R &= ~0x04;  // Set PB2 as input
    GPIO_PORTB_DEN_R |= 0x04;   // Enable digital functionality on PB2
    GPIO_PORTB_PUR_R |= 0x04;   // Enable pull-up resistor on PB2

    // Configure PB2 interrupt for falling edge
    GPIO_PORTB_IS_R &= ~0x04;   // Set PB2 to detect edges
    GPIO_PORTB_IBE_R &= ~0x04;  // Disable both edge detection
    GPIO_PORTB_IEV_R &= ~0x04;  // Falling edge trigger on PB2
    GPIO_PORTB_IBE_R |= 0x04;   // use both edges

    GPIO_PORTB_ICR_R = 0x04;    // Clear any prior interrupt on PB2
    GPIO_PORTB_IM_R |= 0x04;    // Unmask interrupt for PB2

    // NVIC Configuration
    NVIC_EN0_R |= (1 << (INT_GPIOB - 16));  // Enable GPIO Port B interrupt in NVIC

    // Enable global interrupts
    enable_interrupts();
}

//enable interrupts globally
void enable_interrupts(void) {
    __asm("CPSIE I");  
}

//disable interrupts globally
void disable_interrupts(void) {
    __asm("CPSID I"); 
}

// GPIO Port B interrupt handler
void GPIOPortB_Handler(void) {
    if (GPIO_PORTB_RIS_R & 0x04) {  // Check if PB2 triggered the interrupt
        GPIO_PORTB_ICR_R = 0x04;    // Clear the interrupt flag for PB2

        // Handle the event (e.g., door status)
        uint32 switch_state = DIO_ReadPin(1, 2);  // Read PB2 state (0 or 1)
        systicInit(16000000);
       
        if (switch_state == 1) {
            printf("Door is OPEN\n");
            UART5_WriteString("O");
        } 
        else {
            printf("Door is CLOSED\n");
            UART5_WriteString("C");
        }  
    }
}


