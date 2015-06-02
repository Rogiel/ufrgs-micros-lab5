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

#ifndef		__PARSER__
#define		__PARSER__

#include <AT89X52.h>

#include "config.h"

#include "pwm.h"
#include "serial.h"

/**
 * A operação a ser executada no comando
 */
typedef enum {
	/**
	 * Executa uma adição de duty cycle
	 */
	PWM_COMMAND_OPERATION_ADD = 1,

	/**
	 * Executa uma subtração de duty cycle
	 */
	PWM_COMMAND_OPERATION_SUBTRACT = 2,

	/**
	 * Executa uma definição de duty cycle
	 */
	PWM_COMMAND_OPERATION_SET = 3,

	/**
	 * Nenhuma operação
	 */
	PWM_COMMAND_OPERATION_NONE = 0,

	/**
	 * Operação desconhecida
	 */
	PWM_COMMAND_OPERATION_UNK = 0xFF
} PWMCommandOperation;

/**
 * Representa a estrutura de um comando
 */
typedef struct {
	/**
	 * A operação que deve ser executada
	 */
	PWMCommandOperation operation;

	/**
	 * O duty cycle que deve ser utilizado na operação
	 */
	DutyCycle dutyCycle;
} PWMCommand;

/**
 * Faz o parse do byte recebido na serial. Se o parse está completo, "true" é retornado.
 *
 * Caso false seja retornado, mais dados são necessários para completar o processo.
 *
 * @param command um ponteiro apontando para o endereço de memória para armazenar o comando
 * @param byte o byte recebido na entrada serial
 *
 * @return true caso o parse esteja completo
 */
bool parse_command(PWMCommand* command, SerialByte byte);

/**
 * Reinicializa o estado do comando
 *
 * @param command o comando para ter o seu estado reiniciado
 */
void reset_command(PWMCommand* command);

#endif