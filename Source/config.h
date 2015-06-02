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

#ifndef		__CONFIG__
#define		__CONFIG__

// Macros para enable/disable da interrupÁ„o global
/**
 * Desabilita as interrupÁıes globalmente.
 */
#define		DISABLE_GLOBAL_INT()	(IE &= ~(1 << 7))

/**
 * Habilita as interrupÁıes globalmente.
 */
#define		ENABLE_GLOBAL_INT()		(IE |= (1 << 7))

/**
 * O valor de clock do cristal
 */
#define XTAL 11059200

typedef unsigned char bool;
#define true 1
#define false 0

#endif