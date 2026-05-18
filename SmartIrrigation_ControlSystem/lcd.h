#ifndef LCD_H
#define LCD_H

#include "config.h"

#define LCD_RS LATDbits.LATD0
#define LCD_EN LATDbits.LATD1
#define LCD_D4 LATDbits.LATD2
#define LCD_D5 LATDbits.LATD3
#define LCD_D6 LATDbits.LATD4
#define LCD_D7 LATDbits.LATD5

void LCD_Init(void);
void LCD_Clear(void);
void LCD_Command(uint8_t command);
void LCD_Char(char data);
void LCD_String(const char *str);
void LCD_SetCursor(uint8_t row, uint8_t column);
void LCD_PrintNumber(uint16_t number);

#endif
