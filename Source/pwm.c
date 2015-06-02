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


#include <AT89X52.h>
#include "pwm.h"
#include "config.h"

#define TIMER_COUNTER 255
#define PWM_OUT_PORT P2

static DutyCycle dutyCycle;
static DutyCycle dutyCycleCompleted;

void pwm_init() {
	// seta T0M1 e T0M0
	TMOD = (TMOD & 0xF0) | 0x01;

	ET0 = 1;                      /* Enable Timer 0 Interrupts */
	TR0 = 1;                      /* Start Timer 0 Running */

	EA = 1;                       /* Global Interrupt Enable */
	//TH0 = TIMER_COUNTER / 10;	  /* Valor inicial do timer */
	
	TH0 = 0xFF;
	TL0 = 0;
	
	PWM_OUT_PORT = 0;
	dutyCycle = 0;
}

void pwm_set_duty_cycle(DutyCycle dc) {
	DISABLE_GLOBAL_INT();
	dutyCycle = dc / 10;
	ENABLE_GLOBAL_INT();
}

DutyCycle pwm_get_duty_cycle() {
	return dutyCycle * 10;
}

void pwm_interrupt() interrupt 1 {
	TH0 = 0xFF;
	TL0 = 0;
	
	if(dutyCycleCompleted < dutyCycle) {
		PWM_OUT_PORT = 0xFF;
	} else {
		PWM_OUT_PORT = 0;
	}
	dutyCycleCompleted++;

	if(dutyCycleCompleted == 10) {
		dutyCycleCompleted = 0;
	}
}