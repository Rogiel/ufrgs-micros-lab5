//
// Universidade Federal do Rio Grande do Sul
// Escola de Engenharia
// Departamento de Engenharia Elétrica
// Microprocessadores I
//
// Laboratório 5
// Autores: Gustavo Dal Molin, Gustavo dos Anjos e Rogiel Sulzbach
//

#ifndef		__CONFIG__
#define		__CONFIG__

// #define BUILD_FOR_8051

// Macros para enable/disable da interrupÁ„o global
/**
 * Desabilita as interrupÁıes globalmente.
 */
#define		DISABLE_GLOBAL_INT()	(IE &= ~(1 << 7))

/**
 * Habilita as interrupÁıes globalmente.
 */
#define		ENABLE_GLOBAL_INT()		(IE |= (1 << 7))

#ifndef BUILD_FOR_8051
#include "emulation.h"
#endif

#endif