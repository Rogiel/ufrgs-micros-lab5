//
// Universidade Federal do Rio Grande do Sul
// Escola de Engenharia
// Departamento de Engenharia Elétrica
// Microprocessadores I
//
// Laboratório 5
// Autores: Gustavo Dal Molin, Gustavo dos Anjos e Rogiel Sulzbach
//

#ifndef		__PWM__
#define		__PWM__

#include <AT89X52.h>

/**
 * Inicializa os timers do PWM
 */
void pwm_init();

/**
 * Seta o duty cycle do pwm
 *
 * @param dc o duty cycle do pwm
 */
void pwm_set_duty_cycle(unsigned int dc);

/**
 * Retorna o duty cycle do pwm
 *
 * @return o duty cycle do pwm
 */
unsigned int pwm_get_duty_cycle();

#endif