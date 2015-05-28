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

/**
 * Inicializa a comunicação serial
 *
 * @param baudRate a taxa de bits da comunicação
 */
void serial_init(SerialBaudRate baudRate) {
	SCON  = 0x50;                   /* SCON: mode 1, 8-bit UART, enable rcvr    */
	TMOD |= 0x20;                   /* TMOD: timer 1, mode 2, 8-bit reload      */
	TH1 = (unsigned char) (256 - (XTAL / (16L * 12L * baudRate))); /* TH1:  reload value for 2400 baud         */
	TR1   = 1;                      /* TR1:  timer 1 run                        */
	TI    = 1; 						/* TI:   set TI to send first char of UART  */
}

/**
 * Lê um byte da porta serial
 *
 * @return o byte lido na porta serial
 */
SerialByte serial_read_byte() {
	while(!serial_read_complete()) { /* espera até terminar a leitura */ }
	RI = 0;
	return SBUF;
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
	// reseta o registrador de controle do serial
	TI = 0;

	// coloca o byte no buffer de escrita
	SBUF = byte;

	// espera até a escrita complete
	while(!serial_write_complete()) { }
}

/**
 * Verifica se a escrita na porta serial está completa
 *
 * @return true se a escrita foi completada
 */
bool serial_write_complete() {
	return TI;
}