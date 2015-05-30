//
// Universidade Federal do Rio Grande do Sul
// Escola de Engenharia
// Departamento de Engenharia Elétrica
// Microprocessadores I
//
// Laboratório 5
// Autores: Gustavo Dal Molin, Gustavo dos Anjos e Rogiel Sulzbach
//

#include "parser.h"

#define is_number(ascii)\
	(ascii >= 48 && ascii <= 57)

// private
PWMCommandOperation parse_operation(SerialByte byte);
int parse_ascii_number(unsigned char character);

// public
bool parse_command(PWMCommand* command, SerialByte byte) {
	if(command->operation == PWM_COMMAND_OPERATION_NONE) {
		command->operation = parse_operation(byte);

		// comandos do tipo SET possuem diretamente o número necessário
		if(command->operation == PWM_COMMAND_OPERATION_SET) {
			command->dutyCycle = (DutyCycle) parse_ascii_number(byte);
			return true;
		} else if(command->operation == PWM_COMMAND_OPERATION_UNK) {
			return true;
		}

		return false;
	} else {
		int parsed = parse_ascii_number(byte);

		if(parsed == -1) {
			command->operation = PWM_COMMAND_OPERATION_UNK;
		} else {
			command->dutyCycle = (DutyCycle) parsed;
		}

		return true;
	}
}

void reset_command(PWMCommand* command) {
	command->operation = PWM_COMMAND_OPERATION_NONE;
	command->dutyCycle = 0;
}

// private implementation
PWMCommandOperation parse_operation(SerialByte byte) {
	switch(byte) {
		case '+': return PWM_COMMAND_OPERATION_ADD;
		case '-': return PWM_COMMAND_OPERATION_SUBTRACT;

		default:
			if(is_number(byte)) {
				return PWM_COMMAND_OPERATION_SET;
			} else {
				return PWM_COMMAND_OPERATION_UNK;
			}
	}
}

int parse_ascii_number(unsigned char character) {
	if(is_number(character)) {
		return character - 48;
	}
	return -1;
}