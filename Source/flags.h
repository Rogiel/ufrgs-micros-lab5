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
extern unsigned char flags_global;
#define		FLAG_LCD					(1 << 0)
#define		FLAG_KEY					(1 << 1)
#define		FLAG_SERIAL_RX				(1 << 2)

inline void set_flag(unsigned char f) {
	flags_global |= f;
}

inline void unset_flag(unsigned char f) {
	flags_global &= ~f;
}

#endif