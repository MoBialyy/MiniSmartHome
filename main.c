#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "DIO.h"
#include "bitwise_operation.h"
#include "systic.h"
#include "UART.h"
#include "GPTM.h"
#include "dht.h"
#include "door.h"


#define LED_RED    (1U << 1)  
#define LED_BLUE   (1U << 2)  
#define LED_GREEN  (1U << 3) 


void main(void){
  
    // Initialize UART, ADC(Temp sensor related), GPIOs and the magnetic door switch 
    UART_InitE();
    ADC1_Init();  
    DIO_Init();
    door_init();

    float32 temperature;
    uint32 prevSwitchState  = DIO_ReadPin(5, 4);  // Initialize previous switch state
    uint32 switcPlug;
    
    while (1){
      //GPIOPortB_Handler();
      //uint32 door = DIO_ReadPin(1,2);
      //printf("door is %d", door);
      switcPlug = DIO_ReadPin(5,0);
      //printf("switchPlug is %d", switcPlug); 
      if( switcPlug == 1){
         {    
          //printf("o");
          // Check if a new signal from the switch is detected
          if (isNewSwitchSignal(&prevSwitchState)) {
              printf("Switch toggled\n");
              //printf("switch is %d \n", DIO_ReadPin(5,4));
              toggleLamp();  // Toggle lamp based on switch
          }
          // Check if a new message is received from Bluetooth
          else if (isNewMessageReceived()) {
              printf("Bluetooth message received\n");
              char c = UART5_ReadChar();
              if (c == 't') {  // 't' command to toggle lamp via Bluetooth
                  toggleLamp();
              }
          }
          temperature = ADC1_ReadValue();
          if (temperature != -1) {
            UART5_WriteFloat(temperature);
            if (temperature >= 41) {
                //UART5_WriteString("H");
                //printf("temp is %f \n",temperature);
                ToggleBuzzerOn(); 
            } else {
                //UART5_WriteString("k");
                //printf("temp is %f \n",temperature);
                ToggleBuzzerOff();
              }
            } 
          else {
            printf("Error reading DHT11 data\n");
        }
          systicInit(16000000);  // Wait 1 second before the next reading
        }
      }
      else{
        GPIO_PORTF_DATA_R = (1 << 1) | (1 << 2) | (1 << 3);  // Ssystem is Off
        UART5_WriteString("k");
        ToggleBuzzerOff();
      }
      timer0A_delay(1000);
    }
}