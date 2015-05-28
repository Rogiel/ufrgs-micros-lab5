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

void main() {
	pwm_init();
	pwm_set_duty_cycle(10);

	serial_init(9600);

	while(1) {
		if(flags_global & FLAG_SERIAL_RX) {
			unset_flag(FLAG_SERIAL_RX);

			static unsigned int operation;
			if(operation == 0) {
				operation = serial_read_byte();
			} else {
				SerialByte number = parse_ascii_number(serial_read_byte());
				switch(operation) {
					case '+':
						pwm_set_duty_cycle(pwm_get_duty_cycle() + number*10);
						break;

					case '-':
						pwm_set_duty_cycle(pwm_get_duty_cycle() - number*10);
						break;
				}
				operation = 0;
			}

		} else if(flags_global & FLAG_LCD) {
			unset_flag(FLAG_LCD);
		}
	}
}

SERIAL_INTERRUPT_HANDLER({
	set_flag(FLAG_SERIAL_RX);
}, { /* ignore */});