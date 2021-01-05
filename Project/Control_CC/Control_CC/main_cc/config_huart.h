#include "inclusiones.h"

#ifndef UART_H_
#define  UART_H_


int mi_putc(char,FILE *stream); //Estandarizacion de mi funcion propia
int mi_getc(FILE *stream);

FILE uart_io=FDEV_SETUP_STREAM(mi_putc,mi_getc,_FDEV_SETUP_RW); //???
#define fgetc() mi_getc(&uart_io)
#define fputc(x) mi_putc(x,&uart_io)

void mi_UART_Init(uint32_t brate, int trx, int rrx){
	UBRR0=F_CPU/16/(brate-1);
	UCSR0A=0;
	UCSR0B=(1<<RXEN0)|(1<<TXEN0);
	
	UCSR0C=(1<<USBS0)|(1<<UCSZ00)|(1<<UCSZ01);//8 BITS
	
	
	stdout = stdin = &uart_io;
	
	if(trx){
		UCSR0A &=~(1<<TXC0); //Apaga el flag de interrupcion por Transmicion
		UCSR0B |=(1<<RXCIE0); //Habilita la interrupcion TX
		
	}
	
	if(rrx){
		UCSR0A &=~(1<<RXC0); //Apaga flag de interrupcion por recepcion
		UCSR0B |=(1<<RXCIE0); //Habilita la interrupcion
		
		
	}
}

int mi_putc (char c,FILE *uart_io){
	while(!(UCSR0A&(1<<UDRE0)));
	UDR0=c;
	return 0;
}

int mi_getc(FILE *uart_io){
	while(!(UCSR0A&(1<<RXC0)));
	return UDR0;
}

#endif /* UART_H_ */