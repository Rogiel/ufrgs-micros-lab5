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

#ifndef		__FLAGS__
#define		__FLAGS__

/**
 * Flags de execução
 */
static volatile unsigned char flags_global;

/**
 * Flag que indica que a função periódica do LCD deve ser executada
 */
#define		FLAG_LCD					(1 << 0)

/**
 * Flag que indica que a tela LCD deve ser atualizada
 */
#define		FLAG_LCD_UPDATE				(1 << 1)

/**
 * Flag que indica que um byte foi recebido na entrada serial
 */
#define		FLAG_SERIAL_RX				(1 << 2)

/**
 * Define a flag "f"
 * @param f a flag "f" que deve ser setada
 */
#define set_flag(f) \
	DISABLE_GLOBAL_INT();\
	flags_global |= f;\
	ENABLE_GLOBAL_INT();

/**
 * Reseta a flag "f"
 * @param f a flag "f" que deve ser resetada
 */
#define unset_flag(f) \
	DISABLE_GLOBAL_INT();\
	flags_global &= ~f;\
	ENABLE_GLOBAL_INT();

#endif