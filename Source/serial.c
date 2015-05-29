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
	SCON  = 0x50;                   /* SCON: mode 1, 8-bit UART, enable rcvr    */
	TMOD |= 0x20;                   /* TMOD: timer 1, mode 2, 8-bit reload      */
	TH1 = (unsigned char) (256 - (XTAL / (11L * 12L * baudRate))); /* TH1:  reload value for 2400 baud         */
	TR1   = 1;                      /* TR1:  timer 1 run                        */
	
	REN = 1;
	ES = 1;
	
	TI = 0; 											/* TI:   set TI to send first char of UART  */
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
 * Lê um byte da porta serial. Espera até que um byte seja recebido.
 *
 * @return o byte lido na porta serial
 */
SerialByte serial_read_byte_wait() {
	while(!serial_read_complete()) { /* espera até terminar a leitura */ }
	return serial_read_byte();
}

/**
 * Verifica se a leitura na porta serial está completa
 *
 * @return true se a leitura foi completada
 */
bool serial_read_complete() {
	return RI;
}

/**
 * Escreve um byte na porta serial
 *
 * @param byte o byte para ser escrito na porta
 */
void serial_write_byte(SerialByte byte) {
	// coloca o byte no buffer de escrita
	SBUF = byte;
	
	// reseta o registrador de controle do serial
	TI = 0;
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