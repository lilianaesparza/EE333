#include "adc.h"

void ADC_Init(void)
{
    // RA0 input and analog enabled
    TRISAbits.TRISA0 = 1;
    ANSELAbits.ANSELA0 = 1;

    // ADC positive reference = VDD, negative reference = VSS
    ADREF = 0x00;

    // ADC clock selection
    ADCLK = 0x3F;

    // Select AN0 channel
    ADPCH = 0x00;
 // Right justified result
    ADCON0bits.FM = 1;

    // Enable ADC
    ADCON0bits.ON = 1;
}

uint16_t ADC_Read_AN0(void)
{
    uint16_t result;

    ADPCH = 0x00;       // Select AN0
    __delay_us(10);     // Acquisition delay

    ADCON0bits.GO = 1;  // Start conversion
    while(ADCON0bits.GO);
result = ((uint16_t)ADRESH << 8) | ADRESL;
    return result;
}
