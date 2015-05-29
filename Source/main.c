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

#include "flags.h"

void serial_rx();

void pwm_increase_duty_cycle(unsigned int dc);
void pwm_decrease_duty_cycle(unsigned int dc);

void reply_ok();
void reply_nok();

void main() {
	flags_global = 0;
	
	pwm_init();
	serial_init(9600);
	
	serial_write_string("Serial test\n", 12);
	
	while(1) {
		if(flags_global & FLAG_SERIAL_RX) {
			unset_flag(FLAG_SERIAL_RX);
			serial_rx();
		} else if(flags_global & FLAG_LCD) {
			unset_flag(FLAG_LCD);
			// ????
		}
	}
}

static unsigned char SERIAL_RX_OPERATION = 0;

void serial_rx() {
	if(SERIAL_RX_OPERATION == 0) {
		int parsed;
		
		SERIAL_RX_OPERATION = serial_read_byte();
		parsed = parse_ascii_number(SERIAL_RX_OPERATION);
		
		if(parsed != -1) {
			pwm_set_duty_cycle(parsed*10); 
			reply_ok();
			SERIAL_RX_OPERATION = 0;
		} else {
			switch(SERIAL_RX_OPERATION) {
			case '+':
			case '-':
				// wait for more
				break;
			
			default:
				reply_nok();
				break;
			}
		}
	} else {
		int number = parse_ascii_number(serial_read_byte());
		if(number == -1) {
			reply_nok();
			SERIAL_RX_OPERATION = 0;
		}
		
		switch(SERIAL_RX_OPERATION) {
			case '+': {
				pwm_increase_duty_cycle(number);
				break;
			}

			case '-': {
				pwm_decrease_duty_cycle(number);
				break;
			}

			default: {
				// operação não reconhecida. responde com uma negativa
				reply_nok();
			}
		}
		SERIAL_RX_OPERATION = 0;
	}
}

void pwm_increase_duty_cycle(unsigned int number) {
	// se o duty cycle já está em 10 (máximo), responde com uma negativa.
	if(pwm_get_duty_cycle() == 100) {
		reply_nok();
		return;
	}

	// incrementa o duty cycle
	pwm_set_duty_cycle(pwm_get_duty_cycle() + number * 10);

	// responde com OK
	reply_ok();
}

void pwm_decrease_duty_cycle(unsigned int number) {
	// se o duty cycle já está em zero, responde com uma negativa.
	if(pwm_get_duty_cycle() == 0) {
		reply_nok();
		return;
	}

	// decrementa o duty cycle
	pwm_set_duty_cycle(pwm_get_duty_cycle() - number * 10);

	// responde com OK
	reply_ok();
}

void reply_ok() {
	serial_write_string("OK\n", 3);
}

void reply_nok() {
	serial_write_string("NOK\n", 4);
}

SERIAL_INTERRUPT_HANDLER({
	set_flag(FLAG_SERIAL_RX);
}, { /* ignore */})