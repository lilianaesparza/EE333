/*
 * ============================================================================
 * Project Title : Closed-Loop Smart Irrigation System
 * Microcontroller: PIC18F47K42
 * Compiler       : XC8 Compiler
 * IDE            : MPLAB X IDE
 *
 * Description:
 * This project implements a closed-loop smart irrigation system using
 * a capacitive soil moisture sensor, PIC18F47K42 microcontroller,
 * relay-controlled water pump, and 16x2 LCD display.
 *
 * Features:
 * - Real-time soil moisture monitoring
 * - ADC-based sensor acquisition
 * - Automatic pump control using feedback thresholds
 * - Moisture percentage calculation
 * - LCD display of system status
 * - ADC averaging for noise reduction
 *
 * Control Strategy:
 * - ON/OFF feedback controller
 * - Dual-threshold control
 * - Active-high relay operation
 *
 * Author:
 * Liliana Esparza
 *
 * Course:
 * EE333 Control Systems
 *
 * ============================================================================
 */

#include "config.h"
#include "lcd.h"
#include "adc.h"

#define PUMP_LAT LATBbits.LATB0
#define PUMP_TRIS TRISBbits.TRISB0
#define PUMP_ANSEL ANSELBbits.ANSELB0

#define DRY_THRESHOLD 2800
#define WET_THRESHOLD 1800

#define ADC_SAMPLES 10

void System_Init(void);
uint8_t MoisturePercent(uint16_t adcValue);
uint16_t ADC_Read_Average(uint8_t samples);

void main(void)
{
    uint16_t adcValue;
    uint8_t moisturePercent;

    System_Init();

    LCD_Clear();
    LCD_SetCursor(1, 1);
    LCD_String("Smart Irrigation");
    LCD_SetCursor(2, 1);
    LCD_String("PIC18F47K42");
    __delay_ms(2000);

    while(1)
    {
        adcValue = ADC_Read_Average(ADC_SAMPLES);
        moisturePercent = MoisturePercent(adcValue);

        // Active-high relay:
        // PUMP_LAT = 1 means pump ON
        // PUMP_LAT = 0 means pump OFF
        if(adcValue >= DRY_THRESHOLD)
        {
            PUMP_LAT = 1;   // Dry soil -> pump ON
        }
        else if(adcValue <= WET_THRESHOLD)
        {
            PUMP_LAT = 0;   // Wet soil -> pump OFF
        }

        LCD_Clear();

        LCD_SetCursor(1, 1);
        LCD_String("Moisture: ");
        LCD_PrintNumber(moisturePercent);
        LCD_String("%");

        LCD_SetCursor(2, 1);
        LCD_String("ADC:");
        LCD_PrintNumber(adcValue);

        LCD_SetCursor(2, 11);
        if(PUMP_LAT == 1)
        {
            LCD_String("P:ON ");
        }
        else
        {
            LCD_String("P:OFF");
        }

        __delay_ms(1000);
    }
}

void System_Init(void)
{
    ANSELB = 0x00;
    ANSELC = 0x00;

    PUMP_ANSEL = 0;
    PUMP_TRIS = 0;
    PUMP_LAT = 0;   // Relay OFF at startup for active-high relay

    ADC_Init();
    LCD_Init();
}

uint16_t ADC_Read_Average(uint8_t samples)
{
    uint32_t sum = 0;
    uint8_t i;

    for(i = 0; i < samples; i++)
    {
        sum += ADC_Read_AN0();
        __delay_ms(20);
    }

    return (uint16_t)(sum / samples);
}

uint8_t MoisturePercent(uint16_t adcValue)
{
    const uint16_t ADC_WET = 1420;
    const uint16_t ADC_DRY = 3030;

    uint32_t percent;

    if(adcValue <= ADC_WET)
        return 100;

    if(adcValue >= ADC_DRY)
        return 0;

    percent = ((uint32_t)(ADC_DRY - adcValue) * 100UL) / (ADC_DRY - ADC_WET);

    return (uint8_t)percent;
}
