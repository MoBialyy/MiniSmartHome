#include "GPTM.h"
#include "tm4c123gh6pm.h"
#define CPU_FREQ 16000000

void timer0A_delay(uint32 time){
  
  SysCtlPeripheralEnable(1); // enable clock
  Timer0Disable(); // disable timer 
  Timer0Configure(0); // 32bit       16bit -->0x04,  32bit --> 0x00
  TIMER0_TAMR_R = 0x01; // oneshot mode down
  Timer0LoadSet(time); // timer A load value      PS. 16000 not 16000000
  TIMER0_ICR_R = 0x1; // clear
  Timer0Enable(); // enable timer A
  while ((TIMER0_RIS_R & 0x1) == 0) ; // wait
  
}

void SysCtlPeripheralEnable(uint32 ui32Peripheral) {
    SYSCTL_RCGCTIMER_R |= ui32Peripheral;  // Enable
    while ((SYSCTL_RCGCTIMER_R & ui32Peripheral) == 0); // Wait
}

bool SysCtlPeripheralReady(uint32 ui32Peripheral) {
    return (SYSCTL_RCGCTIMER_R & ui32Peripheral) != 0;
}

void Timer0Configure(uint32 ui32Config) {
    TIMER0_CFG_R = ui32Config;  
}

void Timer0LoadSet(uint32 ui32Value) {
    TIMER0_TAILR_R = 16000 * ui32Value - 1;
}

void Timer0Enable(void) {
    TIMER0_CTL_R |= 0x01;  
}

void Timer0Disable(void) {
    TIMER0_CTL_R |= 0x00;  
}
