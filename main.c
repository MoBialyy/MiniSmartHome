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
    UART_InitE();                                       // initialize UART5 and port E
    ADC1_Init();                                        // initialize the temperature related ports
    DIO_Init();                                         // initialize main ports used and clocks
    door_init();                                        // initialize the magnetic door ports and its interrupt related things

    float32 temperature;
    uint32 prevSwitchState  = DIO_ReadPin(5, 4);        // Initialize previous switch state
    uint32 switcPlug;                                   // will keep track to the manual switch that turns the system on/off
    uint32 enable = 1;                                  // will keep track to the app switch that turns the system on/off
    
    while (1){
      switcPlug = DIO_ReadPin(5,0);                     // read the manual switch status
      if(enable ^ switcPlug){                           // 'xor-ing' the manual and app switch, if '1' --> the system is on
         {    
          if (isNewSwitchSignal(&prevSwitchState)) {    // Check if a new signal from the switch is detected
              toggleLamp();                             // Toggle lamp based on switch
          }
          else if (isNewMessageReceived()) {            // Check if a new message is received from Bluetooth
              char c = UART5_ReadChar();                // read the char received from the app
              if (c == 't') {                           // 't' command to toggle lamp 
                  toggleLamp();
              }
              else if (c == 'u') {                      // 'u' command to toggle switch plug
                  enable =!enable;
              }
          }
          temperature = ADC1_ReadValue();               // read the sensor's temperature
          if (temperature != -1) {
                UART5_WriteFloat(temperature);          // send the temperature to the app
            if (temperature >= 41) {                    // check if temperature is above a given value
                ToggleBuzzerOn();                       // toggle the hardware buzzer
            }
            else {                                      // else here is used to turn off the senso
                ToggleBuzzerOff();                      // buzzer keeps on 'buzzing' till temperature gets low again
            }
          } 
          systicInit(16000000);                         // Wait and then rerun
        }
      }
      else{                                             // the system is off
        if (isNewMessageReceived()) {                   // check for message's from the app
              char c = UART5_ReadChar();                // read the message received
              if (c == 'u') {                           // 'u' command to toggle switch plug via app
                  enable = !enable;                     // same as above
              }
        }
        GPIO_PORTF_DATA_R = (1 << 1) | (1 << 2) | (1 << 3);           // system is Off
        UART5_WriteString("k");                                       // sends 'k' to the app to alert that the system is off
        ToggleBuzzerOff();                                            // toggle the buzzer off in case it was on while the system was on
      }
    timer0A_delay(500);                                               // Wait and then rerun
   }
}