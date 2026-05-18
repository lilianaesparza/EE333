#ifndef CONFIG_H
#define CONFIG_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#pragma config FEXTOSC = OFF
#pragma config RSTOSC = HFINTOSC_1MHZ
#pragma config WDTE = OFF
#pragma config LVP = OFF
#pragma config MCLRE = EXTMCLR

#define _XTAL_FREQ 1000000UL

#endif
