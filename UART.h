#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED
#include "Lab1.h"
#include "stdbool.h"

void UART0_Init(void);
void UART_InitE(void);
void UART0_WriteChar(char c);
void UART0_WriteString(char* string);
void printPrompt(void);
char UART0_ReadChar(void);
void controlRGBLED(char message);
void controlIO(char message);
void controlIOS(uint32 message);
char UART5_ReadChar(void);
void UART5_WriteChar(unsigned char data);
void UART5_WriteString(char *str);
bool isNewMessageReceived(void);
bool isNewSwitchSignal(uint32* prevState);
void controlLED(uint32 state);
void toggleLamp(void);
void UART5_WriteFloat(float32 data);

#endif