/**
\file lcd.c
Este arquivo contém as funções de acesso ao LCD.
*/

#include		<AT89X52.h>
#include		"lcd.h"

#define			LCD_DATA	P1		// porta 1 onde esta os pinos de dados do LCD
#define			LCD_CTRL	P3		// porta 3 onde estão os bits de comando
#define			LCD_EN		7			// porta 3, bit 7
#define			LCD_RS		6			// porta 3, bit 6

#define			TIMEOUT_LCD_DATA		0
#define			TIMEOUT_LCD_CMD			4

#define			BUFFER_LCD_SIZE		35

int buffer_lcd[BUFFER_LCD_SIZE];
char index_lcd = 0;
char buffer_lcd_counter = 0;
char state_lcd;
unsigned char timeout_lcd = 0;


// prototipos das privadas (utilizadas apenas neste módulo)
void lcd_write_command (unsigned char cmd);

void lcd_write_data (char data_lcd);

void lcd_pulso_en	(void);


// definição das funções privadas
void lcd_write_command (unsigned char cmd)
{
	LCD_DATA = cmd;
	LCD_CTRL	&= ~(1 << LCD_RS);
	lcd_pulso_en();
	return;
}

void lcd_write_data (char data_lcd)
{
	LCD_DATA = data_lcd;
	LCD_CTRL |= (1 << LCD_RS);
	lcd_pulso_en();
	return;
}

void lcd_pulso_en	(void)
{
	LCD_CTRL	|= (1 << LCD_EN);
	// como a frequencia é de 11.0592 MHz, o tempo de um pulso atende aos requisitos.
	LCD_CTRL	&= ~(1 << LCD_EN);
	return;
}


// Funções públicas (utilizadas por outros módulos)
void lcd_init (void)
{
	// inicialização do LCD
	timeout_lcd = 15; // 15 ms para VCC
	
	buffer_lcd_counter = 0;
	index_lcd = 0;
	
	lcd_put_cmd(0x38);  // 3 vezes, como no manual
	lcd_put_cmd(0x38);
	lcd_put_cmd(0x38);  
	
	lcd_put_cmd(0x06);	// desloca cursor para direita quando insere caracteres
	lcd_put_cmd(0x0E);	// liga cursor
	lcd_put_cmd(0x01);	// limpa tela e volta cursor para 'home'
	
	lcd_start_writing();
	
	return;
}


void lcd_periodic (void) 
{
	int x;
	unsigned char tuchar;
	
	if (timeout_lcd) {
		timeout_lcd--;
		return;
	}
	
	if (!state_lcd)
		return;
			
	if (buffer_lcd_counter > 0) {
		x = buffer_lcd[index_lcd];
		index_lcd++;
		if (index_lcd >= buffer_lcd_counter) {
			buffer_lcd_counter = 0;
			index_lcd = 0;
			state_lcd = 0;
		}
			
		if (x >= 256) {
			// é dado
			lcd_write_data ((char)(x & 0x00FF));
			timeout_lcd = TIMEOUT_LCD_DATA;
		}
		else {
			// é comando
			tuchar = (unsigned char)(x & 0x00FF);
			lcd_write_command (tuchar);
			timeout_lcd = TIMEOUT_LCD_CMD;
		}
	}
	else {
		state_lcd = 0;
	}
}

void lcd_start_writing (void)
{
	index_lcd = 0;
	state_lcd = 1;
}


void lcd_put_char (char x)
{
	buffer_lcd[index_lcd] = (256 + (int)x);
	index_lcd++;
	buffer_lcd_counter++;
}


void lcd_put_cmd (unsigned char cmd)
{
	buffer_lcd[index_lcd] = (int)cmd;
	index_lcd++;
	buffer_lcd_counter++;
	
}

char lcd_put_string (char * pstr, char len)
{
	char i;
	
	if (len >= (BUFFER_LCD_SIZE - buffer_lcd_counter))
		return -1;  // se não cabe mais no buffer de lcd...
	
	for (i = 0; i < len; i++) {
		lcd_put_char (pstr[i]);
	}
	return 0;
}

char lcd_busy (void)
{
	char ret;
	
	DISABLE_GLOBAL_INT();  // necessário caso "lcd_periodic" seja chamado de dentro da ISR de timer0.
	if (state_lcd)
		ret = -1;
	else
		ret = 0;
	ENABLE_GLOBAL_INT();
	
	return ret;
}
