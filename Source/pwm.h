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

#ifndef		__PWM__
#define		__PWM__

#include <AT89X52.h>

/**
 * Definição do tipo do duty cycle do PWM
 */
typedef unsigned char DutyCycle;

/**
 * Inicializa os timers do PWM
 */
void pwm_init();

/**
 * Seta o duty cycle do pwm
 *
 * @param dc o duty cycle do pwm
 */
void pwm_set_duty_cycle(DutyCycle dc);

/**
 * Retorna o duty cycle do pwm
 *
 * @return o duty cycle do pwm
 */
DutyCycle pwm_get_duty_cycle();

#endif