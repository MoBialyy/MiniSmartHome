#include "UART.h"
#include "tm4c123gh6pm.h"
#include <stdio.h>
#include "DIO.h"
#include <stdio.h>
#define LED_RED    (1U << 1)  

uint32 lampState = 0;  // 0 OFF, 1 ON


void UART0_Init(void){
  
  SYSCTL_RCGCUART_R |= (0x01);
  SYSCTL_RCGCGPIO_R |= (0x01)|(1<<5u);
  
  UART0_CTL_R =0;
  UART0_IBRD_R =104;
  UART0_FBRD_R = 11;
  UART0_CC_R = 0;
  UART0_LCRH_R = 0X60;
  UART0_CTL_R = 0X301;
  
   
  GPIO_PORTA_DEN_R = 0X03;
  GPIO_PORTA_AFSEL_R = 0X03;
  GPIO_PORTA_PCTL_R = 0X11;
}

void UART_InitE(void){             //UART5 is used as UART0 can't be used as it doesn't have any pins on the tiva, only serial connection
    SYSCTL_RCGCUART_R |= 0x20;
    SYSCTL_RCGCGPIO_R |= (0x10) | (0x20);
    
    unsigned int i;
    for(i = 0; i < 100; i++);
    
    UART5_CTL_R = 0;               // Disable UART5 during configuration
    UART5_IBRD_R = 104;            // Integer part of baud rate divisor, this is for 9600 baud rate
    UART5_FBRD_R = 11;             // Fractional part
    UART5_CC_R = 0;                // Use system clock as UART clock source
    UART5_LCRH_R = 0x60;           // 8-bit word length, no parity, one stop bit
    UART5_CTL_R = 0x301;           // Enable UART5, enable TX and RX

    GPIO_PORTE_DEN_R = 0x30;       // Enable digital I/O on PE4 and PE5
    GPIO_PORTE_AFSEL_R = 0x30;     // Enable alternate function on PE4 and PE5
    GPIO_PORTE_AMSEL_R = 0x00;     // Disable analog functionality
    GPIO_PORTE_PCTL_R = 0x00110000;// Configure PE4 and PE5 for UART5 function

    //GPIO_PORTF_DIR_R |= 0x0E;         
    GPIO_PORTF_DEN_R |= 0x1F;         
}

void UART0_WriteChar(char c){
    while((UART0_FR_R & 0X20) != 0); //WAIT
    UART0_DR_R = c;
}

void UART0_WriteString(char* string){
    while(*string){
      UART0_WriteChar(*(string++));
    }
}

void printPrompt(void){
    UART0_WriteString("write r or b or g \n \r");
}

char UART0_ReadChar(void){
    while((UART0_FR_R & 0X10) != 0);
    return (char)(UART0_DR_R & 0XFF);
}

void controlRGBLED(char message) {
    switch(message) {
        case 'r':
            GPIO_PORTF_DATA_R = 1 << 1;  // Red LED on
            break;
        case 'b':
            GPIO_PORTF_DATA_R = 1 << 2;  // Blue LED on
            break;
        case 'g':
            GPIO_PORTF_DATA_R = 1 << 3;  // Green LED on
            break;
        case 'w':  // White LED (Red, Green, Blue on)
            GPIO_PORTF_DATA_R = (1 << 1) | (1 << 2) | (1 << 3);
            break;
        case 'y':  // Yellow (Red and Green on)
            GPIO_PORTF_DATA_R = (1 << 1) | (1 << 3);
            break;
        case 'c':  // Cyan (Green and Blue on)
            GPIO_PORTF_DATA_R = (1 << 2) | (1 << 3);
            break;
        case 'p':  // Purple (Red and Blue on)
            GPIO_PORTF_DATA_R = (1 << 1) | (1 << 2);
            break;
        default:
            // Print "turn off all" message to the terminal if no valid input
            UART5_WriteString("turn off all \n");
            GPIO_PORTF_DATA_R = 0;  // Turn off all LEDs
            break;
    }
}

void controlIO(char message){
  
  switch(message) {
        case '1':  // Turn on red LED, 
            DIO_WritePort(5, LED_RED);  
            Delay(10000);
            break;
        case '0':  // Turn off red LED
            DIO_WritePin(5, 1, 0);  
            break;
        default:
            //UART5_WriteString("turn off all blue \n");
            //GPIO_PORTF_DATA_R = 0;  // Turn off all LEDs
            break;
  }
}

char UART5_ReadChar(void)  {
    char data;
    while((UART5_FR_R & (1<<4)) != 0);                  // Wait until receive FIFO is not empty
    data = UART5_DR_R;                                  // Read the received data
    return data; 
}

void UART5_WriteChar(unsigned char data){
    while((UART5_FR_R & (1<<5)) != 0);                  // Wait until transmit FIFO is not full
    UART5_DR_R = data;                                  // Write the data to be transmitted
}

void UART5_WriteString(char *str){
    while(*str)                                         // Loop until end of string (null character)
    {
        UART5_WriteChar(*(str++));                      // Send each character and increment pointer
    }
}

void controlIOS(uint32 message) {
    if (message == 1) {  
        toggleLamp();    
    } else if (message == 0) {  
        toggleLamp();     
    }
}

bool isNewMessageReceived(void) {
    if ((UART5_FR_R & (1 << 4)) == 0) {                 // New message received
        return true;  
    }
    return false;                                       // No new message
}

bool isNewSwitchSignal(uint32* prevState) {
    uint32 currentState = DIO_ReadPin(5, 4);            // Read current state of the switch
    if (currentState != *prevState) {                   // If the state changes, toggle the lamp
        *prevState = currentState;                      // Update previous state
        return true;                                    // Signal detected, return true
    }
    return false;                                       // No change in state
}

void controlLED(uint32 state) {
    if (state == 1) {
        GPIO_PORTF_DATA_R = LED_RED;                    // Turn ON LED, which in turn turn off the lamp
    } else {
        GPIO_PORTF_DATA_R = 0;                          // Turn OFF LED, which in turn turn on the lamp
    }
}

void toggleLamp(void) {
    lampState = !lampState;                             // Toggle the lamp state
    controlLED(lampState);                              // Update LED based on new lamp state
}

void UART5_WriteFloat(float32 data) {
    char buffer[32];                                    // A buffer to hold the string
    snprintf(buffer, sizeof(buffer), "%.2f", data);     // Convert float to string with 2 decimal places
    UART5_WriteString(buffer);                          // Send the string to the app
}
