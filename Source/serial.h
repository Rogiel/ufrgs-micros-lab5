//
// Universidade Federal do Rio Grande do Sul
// Escola de Engenharia
// Departamento de Engenharia Elétrica
// Microprocessadores I
//
// Laboratório 5
// Autores: Gustavo Dal Molin, Gustavo dos Anjos e Rogiel Sulzbach
//

#ifndef		__SERIAL__
#define		__SERIAL__

#include <AT89X52.h>
#include <stdbool.h>

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
 * Configura condicionalmente a macro de interrupção
 */
#ifdef BUILD_FOR_8051
#define SERIAL_INTERRUPT interrupt 4
#else
#define SERIAL_INTERRUPT
#endif

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
 * @example SERIAL_INTERRUPT_HANDLER({ readByte = serial_read_byte(); }, { writeComplete = true; });
 */
#define SERIAL_INTERRUPT_HANDLER(ReadHandler, WriteHandler) void serial_interrupt_handler() SERIAL_INTERRUPT {\
	if(RI) ReadHandler;\
	if(TI) WriteHandler;\
}

#endif