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

#ifndef		__SERIAL__
#define		__SERIAL__

#include <AT89X52.h>

#include "config.h"

/**
 * Definição do tipo de byte
 */
typedef unsigned char SerialByte;

/**
 * Definiçao do tipo da taxa de bits
 */
typedef unsigned int  SerialBaudRate;

/**
 * Configura a macro de interrupção
 */
#define SERIAL_INTERRUPT interrupt 4
/**
 * Inicializa a comunicação serial
 *
 * @param baudRate a taxa de bits da comunicação
 */
void serial_init(SerialBaudRate baudRate);

/**
 * Lê um byte da porta serial
 *
 * @return o byte lido na porta serial
 */
SerialByte serial_read_byte();

/**
 * Lê um byte da porta serial. Espera até que um byte seja recebido.
 *
 * @return o byte lido na porta serial
 */
SerialByte serial_read_byte_wait();

/**
 * Verifica se a leitura na porta serial está completa
 *
 * @return true se a leitura foi completada
 */
bool serial_read_complete();

/**
 * Escreve um byte na porta serial
 *
 * @param byte o byte para ser escrito na porta
 */
void serial_write_byte(SerialByte byte);

/**
 * Escreve um byte na porta serial. Espera até que o byte tenha sido escrito com sucesso.
 *
 * @param byte o byte para ser escrito na porta
 */
void serial_write_byte_wait(SerialByte byte);

/**
 * Escreve uma string na porta serial. Espera até que o byte tenha sido escrito com sucesso.
 *
 * @param str a string para ser escrita na porta
 * @param size o tamanho da string (em bytes)
 */
void serial_write_string(const char* str, unsigned int size);

/**
 * Verifica se a escrita na porta serial está completa
 *
 * @return true se a escrita foi completada
 */
bool serial_write_complete();

/**
 * Macro que cria um handler para a interrupção serial
 *
 * @param ReadHandler o bloco para ser executado ao ler um byte da serial
 * @param WriteHandler o bloco para ser executado ao escrever um byte na serial
 *
 * @example SERIAL_INTERRUPT_HANDLER(readByte = serial_read_byte(), writeComplete = true);
 */
#define SERIAL_INTERRUPT_HANDLER(ReadHandler, WriteHandler)			\
void serial_interrupt_handler() SERIAL_INTERRUPT {					\
	if(RI == 1) ReadHandler;										\
	if(TI == 0) WriteHandler;										\
}

#endif