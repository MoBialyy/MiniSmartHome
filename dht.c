#include "dht.h"
#include "tm4c123gh6pm.h"

void ADC1_Init(){  
  
    //Using PD1 as input for temp sensor, AIN6

    // Enable the ADC clock 
    SYSCTL_RCGCADC_R |= (1 << 1);
    volatile int delay = 0; 
    delay++;

    //Enable the clock to the GPIO D
    SYSCTL_RCGCGPIO_R |= (1 << 3);

    //PD1 as an analog input
    GPIO_PORTD_AFSEL_R |= (1 << 1);     // Enable alternate function for PD1
    GPIO_PORTD_DEN_R &= ~(1 << 1);      // Disable digital function for PD1
    GPIO_PORTD_AMSEL_R |= (1 << 1);     // Enable analog function for PD1
    
    //PD6 AS OUTPUT
    GPIO_PORTD_DIR_R |= (1 << 6);       // Set PD6 as output for buzzer
    GPIO_PORTD_DEN_R |= (1 << 6);       // Enable digital function for PD6
     
    //Disable sample sequencer 3 during configuration
    ADC1_ACTSS_R &= ~(1 << 3);

    //Configure the trigger event for sample sequencer 3
    ADC1_EMUX_R = (0xF << 12);          // Always trigger

    //Configure input source for sample sequencer 3
    ADC1_SSMUX3_R = 6; // AIN6

    //Configure sample control: end of sequence and interrupt enable
    ADC1_SSCTL3_R = (1 << 1) | (1 << 2); // Enable END and IE bits

    //Disable interrupts
    ADC1_IM_R &= ~(1 << 3);

    //Enable sample sequencer 3
    ADC1_ACTSS_R |= (1 << 3);
}

float32 ADC1_ReadValue(void){
    ADC1_PSSI_R |= (1 << 3);                    // Start SS3 Conversion
    while ((ADC1_RIS_R & (1 << 3)) == 0);       // Wait for completion
    float32 adc_value = ADC1_SSFIFO3_R;         // Read ADC value
    ADC1_ISC_R = (1 << 3);                      // Clear interrupt
    
    // Sanity check to avoid negative values
    if (adc_value > 4095) 
    {
        adc_value = 0;                          // Reset to 0 if reading is invalid
    }
    
    // Convert ADC value to voltage, 3.3 --> 3.3V source
    float32 voltage = (adc_value / 4095.0) * 3.3;

    // Convert voltage to temperature
    float32 temperature = voltage * 100;        // LM35: 10mV per °C

    return temperature;
}

void ToggleBuzzerOn(void){
    GPIO_PORTD_DATA_R = (1 << 6);               // Toggle buzzer on on PD6
}

void ToggleBuzzerOff(void){
    GPIO_PORTD_DATA_R = (0 << 6);               // Toggle buzzer off on PD6
}