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

void UART_InitE(void){   //UART5
    SYSCTL_RCGCUART_R |= 0x20;
    SYSCTL_RCGCGPIO_R |= (0x10) | (0x20);
    
    unsigned int i;
    for(i = 0; i < 100; i++);
    
    UART5_CTL_R = 0;
    UART5_IBRD_R = 104;
    UART5_FBRD_R = 11;
    UART5_CC_R = 0;
    UART5_LCRH_R = 0x60;
    UART5_CTL_R = 0x301;

    GPIO_PORTE_DEN_R = 0x30;
    GPIO_PORTE_AFSEL_R = 0x30;
    GPIO_PORTE_AMSEL_R = 0x00;
    GPIO_PORTE_PCTL_R = 0x00110000;
    
    //PE4 --> TXD HC-05
    //PE5 --> RXD HC-05
    //SWITCH --> PF4
    //IN1 --> PF1
    //IN2 --> PF2
    
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
            //Delay(100000);
            break;
        default:
            //UART5_WriteString("turn off all blue \n");
            //GPIO_PORTF_DATA_R = 0;  // Turn off all LEDs
            break;
  }
}

char UART5_ReadChar(void)  {
    char data;
    while((UART5_FR_R & (1<<4)) != 0);
    data = UART5_DR_R;
    return data; 
}

void UART5_WriteChar(unsigned char data){
    while((UART5_FR_R & (1<<5)) != 0);
    UART5_DR_R = data;
}

void UART5_WriteString(char *str){
    while(*str)
    {
        UART5_WriteChar(*(str++));
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
    if ((UART5_FR_R & (1 << 4)) == 0) {
        return true;  // New message received
    }
    return false;  // No new message
}

bool isNewSwitchSignal(uint32* prevState) {
    uint32 currentState = DIO_ReadPin(5, 4);  // Read current state of the switch

    // If the state changes, toggle the lamp
    if (currentState != *prevState) {
        *prevState = currentState;  // Update previous state
        return true;  // Signal detected
    }

    return false;  // No change in state
}

void controlLED(uint32 state) {
    if (state == 1) {
        GPIO_PORTF_DATA_R = LED_RED;  // Turn ON LED
    } else {
        GPIO_PORTF_DATA_R = 0;        // Turn OFF LED
    }
}

void toggleLamp(void) {
    lampState = !lampState;  // Toggle the lamp state
    controlLED(lampState);   // Update LED based on new lamp state
}

void UART5_WriteFloat(float32 data) {
    char buffer[32];  // A buffer to hold the string

    snprintf(buffer, sizeof(buffer), "%.2f", data);

    UART5_WriteString(buffer);
}