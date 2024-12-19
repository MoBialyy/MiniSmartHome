#include "systic.h"
#include "tm4c123gh6pm.h"
#define CPU_FREQ 16000000

void systicInit(ulong32 reloadValue) {
    NVIC_ST_CTRL_R = 0;                 
    NVIC_ST_RELOAD_R = reloadValue;     
    NVIC_ST_CURRENT_R = 0;              
    NVIC_ST_CTRL_R = 0x05;              
}

void systicInitTime(float32 time) {
    NVIC_ST_CTRL_R = 0;                 
    ulong32 reloadValue = 0;
    reloadValue = (time*CPU_FREQ)-1;
    NVIC_ST_RELOAD_R = reloadValue;     
    NVIC_ST_CURRENT_R = 0;              
    NVIC_ST_CTRL_R = 0x05;              
}

void systicWait(void) {
    while ((NVIC_ST_CTRL_R & 0x10000) == 0);     
    NVIC_ST_CURRENT_R = 0;                      
}

void systicEnable(void){
    NVIC_ST_CTRL_R |= 0x05;
}

void systicDisable(void){
    NVIC_ST_CTRL_R &= ~(0x01);
}

void systicPeriodSet(uint32 period){
    NVIC_ST_RELOAD_R = period - 1;
}

uint32 getSysticPeriod(void){
    return  NVIC_ST_RELOAD_R + 1;
}

uint32 getSysticPeriodTime(void){
    return (NVIC_ST_RELOAD_R + 1) / (CPU_FREQ);
}

uint32 getSysticValue(void){
    return NVIC_ST_CURRENT_R;
}

bool systicTimeOut(void){
  if (NVIC_ST_CTRL_R & 0x10000){        
      NVIC_ST_CURRENT_R = 0;            
      return true;                      
  }
  return false;                         
}

void toggle500ms(void){
    
    //Please note that our register is 24 bits, so max value is 16,777,215 which is roughly one second, if you try to make the reg value
    //more than 1 sec, the register will overflow and will only blink for 1 sec, less than 1 sec will be ok and should work fine
    
    //systicInit(7999999); 
    systicInitTime(0.001);
    printf("Actual Reload Value after setting: %u ticks \n", getSysticPeriod());
    printf("Time to blink: %d second(s)\n", getSysticPeriodTime()); //will print 0 since 0.5 isn't an integer
    
    //turn off the lamp
    GPIO_PORTF_DATA_R &= ~(LED_RED | LED_BLUE | LED_GREEN);
    
    while (1) {
        // Toggle LED
        GPIO_PORTF_DATA_R ^= (LED_GREEN);
        systicWait();
    }
}