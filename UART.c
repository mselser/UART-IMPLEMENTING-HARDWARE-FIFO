#include "UART.h"
#include "PORT.h"

// DECLARACION DEL BUFFER ANILLO
static char ring_buffer[UART_SOFTWARE_BUFFER_SIZE];

// DECLARACION DE PUNTEROS A LA DATA Y A LA DATA ENVIADA DEL BUFFER
static uint16_t data_to_send_index = 0;
static uint16_t data_sent_index = 0;

// FUNCIONES AUXILIARES
static void UART_SetBaudRate (UART_Type *uart, uint32_t baudrate);
static void wait (unsigned long int tiempo);

void UART_Init (void)
{
    /* Clock UART0 module */
    SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;

    /* Use PTB17 as Tx for UART0 */
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
    PORTB_PCR17 = PORT_PCR_MUX(3);

    /* Disable Transmitter and Receptor */
    UART0->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

	//UART0 Baudrate Setup
	UART_SetBaudRate(UART0, UART_HAL_DEFAULT_BAUDRATE);

	/* Enable Tx FIFO feature */
	UART0->PFIFO |= UART_PFIFO_TXFE_MASK;

	// FLUSHEO LA FIFO	SOLO EN EL INIT AL PRINCIPIO
	UART0 ->CFIFO |= UART_CFIFO_TXFLUSH_MASK;

	// SETEO LA WATERMARK
	UART0 ->TWFIFO = 0;

    /* Enable Receiver interrupt, Enable Transmitter and Receptor*/
    UART0_C2 |= (UART_C2_TE_MASK);

    // HABILITO LA INTERRUPCION DE TX_RX
    NVIC_EnableIRQ(UART0_RX_TX_IRQn);

}

void UART_SetBaudRate (UART_Type *uart, uint32_t baudrate)
{
	uint16_t sbr, brfa;
	uint32_t clock;

	clock = ((uart == UART0) || (uart == UART1))?(__CORE_CLOCK__):(__CORE_CLOCK__ >> 1);

	baudrate = ((baudrate == 0)?(UART_HAL_DEFAULT_BAUDRATE):
			((baudrate > 0x1FFF)?(UART_HAL_DEFAULT_BAUDRATE):(baudrate)));

	sbr = clock / (baudrate << 4);               // sbr = clock/(Baudrate x 16)
	brfa = (clock << 1) / baudrate - (sbr << 5); // brfa = 2*Clock/baudrate - 32*sbr

	uart->BDH = UART_BDH_SBR(sbr >> 8);
	uart->BDL = UART_BDL_SBR(sbr);
	uart->C4 = (uart->C4 & (~UART_C4_BRFA_MASK) ) | UART_C4_BRFA(brfa);
}

__ISR__ UART0_RX_TX_IRQHandler (void)
{
	//	INDICE DEL BUFFER
	int i = 0;

	//	RECORRO MIENTRAS SE CUMPLA SIMULTANEAMENTE QUE:
	//	1)	EL INDICE SEA MAS CHICO QUE EL BUFFER DEL HARDWARE (ES DECIR, EN ESTE CASO, MANDO HASTA OCHO POR INTERRUPCION)
	//	2)	EL INDICE DE DATOS ENVIADOS NO ALCANCE AL INDICE DE DATOS A ENVIAR. ES DECIR, SI YA ENVIE TODA LA DATA DEL BUFFER
	//		DEJO DE TRANSMITIR.
	for(i=0; (i < UART_HARDWARE_BUFFER_SIZE) && (data_sent_index != data_to_send_index); i++, data_sent_index++)
	{
		data_sent_index %= UART_SOFTWARE_BUFFER_SIZE;
		UART0->D = ring_buffer[data_sent_index];		//GUARDO LA INFORMACION EN LA FIFO
		wait(TIME);										//ESPERO UN TIEMPO HASTA ENVIAR EL PROXIMO DATO (SI NO, NO ANDA)
	}

	if ((data_sent_index == data_to_send_index) || (i==0))	// SI EL BUFFER ESTÁ VACÍO O SE TRANSMITIÓ TODA LA INFORMACION
		UART0->C2 &= ~UART_C2_TIE_MASK;						// DESHABILITO LA TRANSMISIÓN
}


unsigned char UART_Get_Status(void)
{
	// SI NO ESTA TRANSMITIENDO, DEVUELVE TRUE
	return(((UART0->S1)& UART_S1_TDRE_MASK));
}

void UART_Send_Data(char * data, uint16_t length)
{
	//	DESHABILITO LA TRANSMISION HASTA LLENAR EL BUFFER
	UART0->C2 &= ~UART_C2_TIE_MASK;

	//	INDICE DEL BUFFER A AGREGAR
	int i = 0;

	//	CARGO TODO EL RING BUFFER CON LA INFORMACION A ENVIAR
	for(i = 0; i < length ; i++)
		ring_buffer[(data_to_send_index + i) % UART_SOFTWARE_BUFFER_SIZE] = data[i];

	//INCREMENTO EL PUNTERO A DATA A ENVIAR
	data_to_send_index += i;

	//PARA NO PASARME DEL TAMANO PERMITIDO
	data_to_send_index %= UART_SOFTWARE_BUFFER_SIZE;

	//	REHABILITO LA TRANSMISION
	UART0->C2 |= UART_C2_TIE_MASK;

}

void wait (unsigned long int tiempo)
{
	while(tiempo--);	//	HACER ESTO ES LA UNICA FORMA DE QUE ANDE. BASTANTE FEO.
}
