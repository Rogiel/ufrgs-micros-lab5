//
// Universidade Federal do Rio Grande do Sul
// Escola de Engenharia
// Departamento de Engenharia Elétrica
// Microprocessadores I
//
// Laboratório 5
// Autores: Gustavo Dal Molin, Gustavo dos Anjos e Rogiel Sulzbach
//

#include <AT89X52.h>
#include "pwm.h"
#include "config.h"

#define TIMER_COUNTER 255
#ifdef COMPILE_FOR_8051
#define TIMER_INTERRUPT_FLAG interrupt 1
#else
#define TIMER_INTERRUPT_FLAG
#endif

static unsigned int pwm;

#define PWM_OUT_PORT pwm
#define PWM_OUT_PIN 1

static unsigned int dutyCycle;
static unsigned int dutyCycleCompleted;

void pwm_init() {
	// seta T0M1 e T0M0
	TMOD = (TMOD & 0xFC) | (1 << 1);

	ET0 = 1;                      /* Enable Timer 0 Interrupts */
	TR0 = 1;                      /* Start Timer 0 Running */

	EA = 1;                       /* Global Interrupt Enable */
	TH0 = TIMER_COUNTER / 10;	  /* Valor inicial do timer */
}

void pwm_set_duty_cycle(unsigned int dc) {
	dutyCycle = dc;
}

unsigned int pwm_get_duty_cycle() {
	return dutyCycle;
}

void pwm_interrupt() TIMER_INTERRUPT_FLAG {
	if(dutyCycleCompleted < dutyCycle) {
		PWM_OUT_PORT |= (1 << PWM_OUT_PIN);
	} else {
		PWM_OUT_PORT &= ~(1 << PWM_OUT_PIN);
	}
	dutyCycleCompleted++;

	if(dutyCycleCompleted == 9) {
		dutyCycleCompleted = 0;
	}
}