//
// Universidade Federal do Rio Grande do Sul
// Escola de Engenharia
// Departamento de Engenharia Elétrica
// Microprocessadores I
//
// Laboratório 5
// Autores: Gustavo Dal Molin, Gustavo dos Anjos e Rogiel Sulzbach
//

#ifndef		__FLAGS__
#define		__FLAGS__

/**
 * Flags de execução
 */
extern volatile unsigned char flags_global;
#define		FLAG_LCD					(1 << 0)
#define		FLAG_KEY					(1 << 1)
#define		FLAG_SERIAL_RX				(1 << 2)

void set_flag(unsigned char f) {
	DISABLE_GLOBAL_INT();
	flags_global |= f;
	ENABLE_GLOBAL_INT();
}

void unset_flag(unsigned char f) {
	DISABLE_GLOBAL_INT();
	flags_global &= ~f;
	ENABLE_GLOBAL_INT();
}

#endif