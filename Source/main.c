//
// Universidade Federal do Rio Grande do Sul
// Escola de Engenharia
// Departamento de Engenharia Elétrica
// Microprocessadores I
//
// https://github.com/Rogiel/ufrgs-micros-lab5
//
// Laboratório 5
// Autores: Gustavo Dal Molin, Gustavo dos Anjos e Rogiel Sulzbach
//

#include "pwm.h"
#include "serial.h"
#include "parser.h"
#include "lcd.h"

#include "flags.h"

/**
 * O comando de PWM para executar
 */
static PWMCommand command;

/**
 * O valor do último byte recebido na entrada serial
 */
static volatile SerialByte serialByte;

/**
 * Processa o recebimento de um byte na entrada serial
 */
void serial_rx();

/**
 * Soma um valor do duty cycle atual
 *
 * @param dc o duty cycle para ser somado
 */
void pwm_setup_increase_duty_cycle(DutyCycle dc);

/**
 * Subtrai um valor do duty cycle atual
 *
 * @param dc o duty cycle para ser subtraido
 */
void pwm_setup_decrease_duty_cycle(DutyCycle dc);

/**
 * Configura o duty cycle para um valor absoluto
 *
 * @param dc o duty cycle para ser configurado
 */
void pwm_setup_set_duty_cycle(DutyCycle dc);

/**
 * Atualiza a string exibida no display de acordo com o estado atual do PWM
 */
void update_display();

void init_display();

#define reply_ok()\
	serial_write_string("OK\n", 3);

#define reply_nok()\
	serial_write_string("NOK\n", 4);

void lcd_timer_setup();

/**
 * Entry-point do processador
 */
void main() {
	// configuração das globais
	reset_command(&command);
	flags_global = 0;
	serialByte = 0;

	// configuração do PWM e Serial
	pwm_init();
	serial_init(9600);
	lcd_timer_setup();
	lcd_init();
	
	init_display();

	// loop infinito
	while(1) {
		if(flags_global & FLAG_SERIAL_RX) {
			unset_flag(FLAG_SERIAL_RX);
			serial_rx();
		} else if(flags_global & FLAG_LCD) {
			unset_flag(FLAG_LCD);
			lcd_periodic();
		} else if(flags_global & FLAG_LCD_UPDATE) {
			update_display();
		}
	}
}

void serial_rx() {
	// processa o byte recebido
	bool parse = parse_command(&command, serialByte);

	// se o processo de parsing está complexo, executa o comando
	if(parse) {
		switch(command.operation) {
			// comando +
			case PWM_COMMAND_OPERATION_ADD: {
				pwm_setup_increase_duty_cycle(command.dutyCycle);
				break;
			}

			// comando -
			case PWM_COMMAND_OPERATION_SUBTRACT: {
				pwm_setup_decrease_duty_cycle(command.dutyCycle);
				break;
			}

			// comando de definição absoluta
			case PWM_COMMAND_OPERATION_SET: {
				pwm_setup_set_duty_cycle(command.dutyCycle);
				break;
			}

			// comando não reconhecido
			default: {
				reply_nok();
				break;
			}
		}

		// reinicia o comando para ser reutilizado
		reset_command(&command);
	}
}

void pwm_setup_increase_duty_cycle(DutyCycle number) {
	DutyCycle current = pwm_get_duty_cycle();
	signed int new = 0;

	// se o duty cycle já está em 10 (máximo), responde com uma negativa.
	if(current == 100) {
		reply_nok();
		return;
	}

	// calcula o novo duty cycle e o mantém dentro dos limites
	new = current + number * 10;
	if(new > 100) {
		new = 100;
	}

	// incrementa o duty cycle
	pwm_set_duty_cycle(new);

	// atualiza o display
	set_flag(FLAG_LCD_UPDATE);

	// responde com OK
	reply_ok();
}

void pwm_setup_decrease_duty_cycle(DutyCycle number) {
	DutyCycle current = pwm_get_duty_cycle();
	signed int new = 0;

	// se o duty cycle já está em zero, responde com uma negativa.
	if(current == 0) {
		reply_nok();
		return;
	}

	// calcula o novo duty cycle e o mantém dentro dos limites
	new = current - number * 10;
	if(new < 0) {
		new = 0;
	}

	// decrementa o duty cycle
	pwm_set_duty_cycle(new);

	// atualiza o display
	set_flag(FLAG_LCD_UPDATE);

	// responde com OK
	reply_ok();
}

void pwm_setup_set_duty_cycle(DutyCycle number) {
	// seta o duty cycle
	pwm_set_duty_cycle(number * 10);

	// atualiza o display
	set_flag(FLAG_LCD_UPDATE);

	// responde com OK
	reply_ok();
}

void init_display() {
	// ativa o cursor
	lcd_put_cmd(0x0c);
	lcd_put_cmd(0x83);
	
	lcd_put_string("DUTY-CYCLE   ", 15);

	lcd_put_cmd(0xc6);
	
	// finaliza a escrita da porcentagem
	update_display();
	
	// libera a flag para escrita no display
	lcd_start_writing();
}

void update_display() {
	DutyCycle dc;
	if(lcd_busy()) {
		return;
	}
	
	unset_flag(FLAG_LCD_UPDATE);

	// desativa o cursor
	lcd_put_cmd(0x0c);
	lcd_put_cmd(0x83);
	lcd_put_cmd(0xc6);
	
	// coloca o duty cycle no display
	dc = pwm_get_duty_cycle() / 10;
	if(dc == 10) {
		lcd_put_string("10", 0x02);
	} else {
		lcd_put_string("0", 0x01);
		lcd_put_char((unsigned char)(dc + 48));
	}

	// finaliza a escrita da porcentagem
	lcd_put_string("0%", 0x02);

	// libera a flag para escrita no display
	lcd_start_writing();
}

void lcd_timer_setup() {
	// reseta os bits 4-7
	TMOD &= 0x0F;

	// configura o timer em modo 1
	TMOD |= 0x10;

	// define os valores iniciais do timer
	TH1 = 0xD0;
	TL1 = 0x10;

	ET1=1;							/* enable timer1 interrupt */
	EA=1;							/* enable interrupts */
	TR1=1;							/* timer1 run */
}

void lcd_timer_interrupt() interrupt 3 {
	set_flag(FLAG_LCD);
	
	TH1 = 0xD0;
	TL1 = 0x10;
}

/**
 * Handler de interrupção serial
 */
SERIAL_INTERRUPT_HANDLER({
	serialByte = serial_read_byte();
	set_flag(FLAG_SERIAL_RX);
}, { /* ignore */})

