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

/**
 * Verifica se o caractere do argumento é um número na tabela ascii
 */
#define is_number(ascii)\
	(ascii >= 48 && ascii <= 57)

// private
/**
 * Processa a operação do comando
 *
 * @param byte o byte da operação
 * @return a constante respectiva a operação do comando
 */
PWMCommandOperation parse_operation(SerialByte byte);

/**
 * Processa um número ascii em um inteiro
 *
 * @param character o caractere em ascii
 * @return o número respectico ao caractere em ascii
 */
int parse_ascii_number(unsigned char character);

// public
bool parse_command(PWMCommand* command, SerialByte byte) {
	// processa o byte de operação
	command->operation = parse_operation(byte);

	// comandos do tipo SET possuem diretamente o número necessário
	if(command->operation == PWM_COMMAND_OPERATION_SET) {
		if(byte == '*') {
			command->dutyCycle = 10;
		} else {
			command->dutyCycle = (DutyCycle) parse_ascii_number(byte);
		}
	} else if(command->operation == PWM_COMMAND_OPERATION_ADD ||
			  command->operation == PWM_COMMAND_OPERATION_SUBTRACT) {
		command->dutyCycle = 1;
	}

	// parsing completo
	return true;
}

void reset_command(PWMCommand* command) {
	// seta operação para NONE
	command->operation = PWM_COMMAND_OPERATION_NONE;

	// seta o duty cycle em 0
	command->dutyCycle = 0;
}

// private implementation
PWMCommandOperation parse_operation(SerialByte byte) {
	switch(byte) {

		// operação +
		case '+': return PWM_COMMAND_OPERATION_ADD;

		// operação -
		case '-': return PWM_COMMAND_OPERATION_SUBTRACT;
		
		// operação set (máximo)
		case '*': return PWM_COMMAND_OPERATION_SET;

		default:
			// se o byte de operação é um número, a operação é
			// do tipo set
			if(is_number(byte)) {
				return PWM_COMMAND_OPERATION_SET;
			} else {
				return PWM_COMMAND_OPERATION_UNK;
			}
	}
}

int parse_ascii_number(unsigned char character) {
	// da tabela ASCII:
	// 48 (decimal) -> '0'
	if(is_number(character)) {
		return character - 48;
	}

	// não é um número :(
	return -1;
}