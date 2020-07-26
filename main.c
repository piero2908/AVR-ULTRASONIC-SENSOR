#define F_CPU 12000000UL
#include<avr/io.h>
#include<util/delay.h>
#include"lcd.h"
#include<stdio.h>

//#define PIND2 TRIG
//#define PIND3 ECHO

uint16_t tiempo;
float distancia;
char mensaje[16];

void pin_config(){
	DDRD |= (1<<PORTD2)|(0<<PORTD3);
}

void tmr1_config(){
	TCCR1B |= (1<<CS11)|(1<<CS10); //PRESCALER DE 64, T = 5.33 uS,  349.30 mS para alcanzar 65536 (0-65535)
	
	TCNT1 = 0;
}

int main(){
	
	pin_config();
	tmr1_config();
	lcd_init(LCD_DISP_ON);
	
	while (1) {
		
		PORTD = (1<<PORTD2);
		_delay_us(10);
		PORTD = (0<<PORTD2);
		while( !(PIND &(1<<PIND3)) ); // {}
		TCNT1 = 0;
		while( (PIND &(1<<PIND3)) ); // {}
		
		//obtener timer1 y hacer calculos
		tiempo = TCNT1;
		distancia = (float)(tiempo*5.33*0.0343)/2.0;
		
		//Convertir distancia a char*
		snprintf(mensaje,sizeof(mensaje),"%3.2f",distancia);	
		//sprintf(mensaje,"%f",distancia);
		
		//Imprimir distancia en pantalla
		lcd_clrscr();
		lcd_puts(mensaje);
				
		//Tiempo de espera
		_delay_ms(200);
	}
	return 0;
}
