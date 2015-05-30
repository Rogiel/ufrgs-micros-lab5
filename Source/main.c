//
// Universidade Federal do Rio Grande do Sul
// Escola de Engenharia
// Departamento de Engenharia Elétrica
// Microprocessadores I
//
// Laboratório 5
// Autores: Gustavo Dal Molin, Gustavo dos Anjos e Rogiel Sulzbach
//

#include "pwm.h"
#include "serial.h"
#include "parser.h"
#include "lcd.h"

#include "flags.h"

static PWMCommand command;
static volatile SerialByte serialByte;

void serial_rx();

void pwm_setup_increase_duty_cycle(DutyCycle dc);
void pwm_setup_decrease_duty_cycle(DutyCycle dc);
void pwm_setup_set_duty_cycle(DutyCycle dc);

void update_display();

#define reply_ok()\
	serial_write_string("OK\n", 3);

#define reply_nok()\
	serial_write_string("NOK\n", 4);

void main() {
	// configuração das globais
	reset_command(&command);
	flags_global = 0;
	serialByte = 0;

	// configuração do PWM e Serial
	pwm_init();
	serial_init(9600);

	// loop infinito
	while(1) {
		if(flags_global & FLAG_SERIAL_RX) {
			unset_flag(FLAG_SERIAL_RX);
			serial_rx();
		} else if(flags_global & FLAG_LCD) {
			unset_flag(FLAG_LCD);
			lcd_periodic();
		}
	}
}

void serial_rx() {
	bool parse = parse_command(&command, serialByte);
	if(parse) {
		switch(command.operation) {
			case PWM_COMMAND_OPERATION_ADD: {
				pwm_setup_increase_duty_cycle(command.dutyCycle);
				break;
			}

			case PWM_COMMAND_OPERATION_SUBTRACT: {
				pwm_setup_decrease_duty_cycle(command.dutyCycle);
				break;
			}

			case PWM_COMMAND_OPERATION_SET: {
				pwm_setup_set_duty_cycle(command.dutyCycle);
				break;
			}

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
	update_display();

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
	update_display();

	// responde com OK
	reply_ok();
}

void pwm_setup_set_duty_cycle(DutyCycle number) {
	// seta o duty cycle
	pwm_set_duty_cycle(number * 10);

	// atualiza o display
	update_display();

	// responde com OK
	reply_ok();
}

void update_display() {
	DutyCycle dc;

	lcd_init();
	lcd_put_string("   DUTY-CYCLE", 0x0D);
	lcd_put_cmd(0xC0);
	lcd_put_string(":     ", 0x06);

	dc = pwm_get_duty_cycle();
	if(dc == 100) {
		lcd_put_string("10", 0x02);
	} else {
		lcd_put_string("0", 0x01);
		lcd_put_char((unsigned char)(dc + 48));
	}

	lcd_put_string("0%", 0x02);
	lcd_put_cmd(0x0C);//desliga o cursor
	lcd_start_writing();//indica iniciar a escerita do display
}

SERIAL_INTERRUPT_HANDLER({
	serialByte = serial_read_byte();
	set_flag(FLAG_SERIAL_RX);
}, { /* ignore */})

