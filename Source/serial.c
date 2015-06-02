//
// Universidade Federal do Rio Grande do Sul
// Escola de Engenharia
// Departamento de Engenharia Elétrica
// Microprocessadores I
//
// Laboratório 5
// Autores: Gustavo Dal Molin, Gustavo dos Anjos e Rogiel Sulzbach
//

#include "serial.h"
#include <AT89X52.h>

/**
 * Inicializa a comunicação serial
 *
 * @param baudRate a taxa de bits da comunicação
 */
void serial_init(SerialBaudRate baudRate) {
	unsigned int divisor = (unsigned int) (65536 - XTAL / (32L * baudRate)) - 1;

	// SCON em modo 1 (SM0=0, SM1=1)
	SM0 = 0;
	SM1 = 1;

	// ativa o recebimento
	REN = 1;

	// configura o timer 2
	T2CON = 0x34;

	// configure os bytes altos e baixos do timer 2
	RCAP2H = (unsigned char) (divisor / 256);
	RCAP2L = divisor - RCAP2H;

	// ativa o serial
	ES = 1;

	// defaults
	TI = 0;
	RI = 0;
}

/**
 * Lê um byte da porta serial
 *
 * @return o byte lido na porta serial
 */
SerialByte serial_read_byte() {
	RI = 0;
	return SBUF;
}

/**
 * Escreve um byte na porta serial
 *
 * @param byte o byte para ser escrito na porta
 */
void serial_write_byte(SerialByte byte) {
	DISABLE_GLOBAL_INT();

	// coloca o byte no buffer de escrita
	SBUF = byte;
	TI = 0;

	ENABLE_GLOBAL_INT();
}

/**
 * Escreve um byte na porta serial. Espera até que o byte tenha sido escrito com sucesso.
 *
 * @param byte o byte para ser escrito na porta
 */
void serial_write_byte_wait(SerialByte byte) {
	// escreve o byte
	serial_write_byte(byte);

	// espera até a escrita complete
	while(!serial_write_complete()) { }
}

/**
 * Escreve uma string na porta serial. Espera até que o byte tenha sido escrito com sucesso.
 *
 * @param str a string para ser escrita na porta
 * @param size o tamanho da string (em bytes)
 */
void serial_write_string(const char* str, unsigned int size) {
	unsigned int i;
	for(i = 0; i< size; i++) {
		serial_write_byte_wait((SerialByte) str[i]);
	}
}

/**
 * Verifica se a escrita na porta serial está completa
 *
 * @return true se a escrita foi completada
 */
bool serial_write_complete() {
	return TI;
}