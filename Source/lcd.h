/**
\file lcd.h
Este arquivo cont�m os prot�tipos das fun��es publicas de lcd.
*/

#ifndef		__LCD__
#define		__LCD__

#include "config.h"

void lcd_init (void);

/**
* Esta fun��o foi planejada para ser executada a cada 1ms (aproximadamente);
*
*/
void lcd_periodic (void);

void lcd_start_writing (void);

void lcd_put_char (char x);

void lcd_put_cmd (unsigned char cmd);

char lcd_put_string (char * pstr, char len);

char lcd_busy (void);

#endif
