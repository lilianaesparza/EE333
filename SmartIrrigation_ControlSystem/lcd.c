#include "lcd.h"

static void LCD_EnablePulse(void)
{
    LCD_EN = 1;
    __delay_us(5);
    LCD_EN = 0;
    __delay_us(100);
}

static void LCD_SendNibble(uint8_t nibble)
{
    LCD_D4 = (nibble >> 0) & 0x01;
    LCD_D5 = (nibble >> 1) & 0x01;
    LCD_D6 = (nibble >> 2) & 0x01;
    LCD_D7 = (nibble >> 3) & 0x01;
    LCD_EnablePulse();
    }

void LCD_Command(uint8_t command)
{
    LCD_RS = 0;
    LCD_SendNibble(command >> 4);
    LCD_SendNibble(command & 0x0F);
    __delay_ms(2);
}

void LCD_Char(char data)
{
    LCD_RS = 1;
    LCD_SendNibble(data >> 4);
    LCD_SendNibble(data & 0x0F);
      __delay_us(100);
}

void LCD_Init(void)
{
    // RD0-RD5 as digital outputs
    ANSELD = 0x00;
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD4 = 0;
    TRISDbits.TRISD5 = 0;
    LCD_RS = 0;
    LCD_EN = 0;

    __delay_ms(20);

    // LCD initialization sequence for 4-bit mode
    LCD_SendNibble(0x03);
    __delay_ms(5);
    LCD_SendNibble(0x03);
    __delay_us(150);
    LCD_SendNibble(0x03);
    __delay_us(150);
    LCD_SendNibble(0x02);
  LCD_Command(0x28); // 4-bit mode, 2 lines, 5x8 font
    LCD_Command(0x0C); // Display ON, cursor OFF
    LCD_Command(0x06); // Increment cursor
    LCD_Command(0x01); // Clear display
    __delay_ms(2);
}

void LCD_Clear(void)
{
    LCD_Command(0x01);
    __delay_ms(2);
}

void LCD_String(const char *str)
{
    while(*str)
{
        LCD_Char(*str++);
    }
}

void LCD_SetCursor(uint8_t row, uint8_t column)
{
    uint8_t address;

    if(row == 1)
        address = 0x80 + (column - 1);
    else
        address = 0xC0 + (column - 1);

    LCD_Command(address);
}
void LCD_PrintNumber(uint16_t number)
{
    char buffer[6];
    uint8_t i = 0;
    uint8_t j;

    if(number == 0)
    {
        LCD_Char('0');
        return;
    }

    while(number > 0)
    {
        buffer[i++] = (number % 10) + '0';
        number /= 10;
    }
  for(j = i; j > 0; j--)
    {
        LCD_Char(buffer[j - 1]);
    }
}
