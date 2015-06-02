/**
\file lcd.h
Este arquivo contém os protótipos das funções publicas de lcd.
*/

#ifndef		__LCD_H_
#define		__LCD_H_

void lcd_init (void);

/**
* Esta função foi planejada para ser executada a cada 1ms (aproximadamente);
*
*/
void lcd_periodic (void);

void lcd_start_writing (void);

void lcd_put_char (char x);

void lcd_put_cmd (unsigned char cmd);

char lcd_put_string (char * pstr, char len);

char lcd_busy (void);

#endif
