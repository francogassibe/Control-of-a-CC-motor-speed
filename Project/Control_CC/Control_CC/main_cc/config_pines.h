#include "inclusiones.h"

//Preescaler:
	uint16_t prescalerValue_TC1 =0;
	uint16_t prescalerValue_TC0 =0;




//Configuracion de pin de interrupcion para lectura de pulsos de encoder
void configuracion_interrupcion_Externa(int habl1, int modo1, int habl2, int modo2){
	EICRA = 0;
	EIMSK = 0;
	
	if(habl1){
		switch(modo1){
			case 0: //Interrupe por nivel bajo
			EICRA&=~(3<<ISC00);
			break;
			case 1: // Cualquier cambio
			EICRA|=(1<<ISC00);
			break;
			case 2: //Flanco de bajada
			EICRA|=(2<<ISC00);
			break;
			case 3: //Flanco de subida
			EICRA|=(3<<ISC00);
			break;
			
		}
		EIMSK = (1<<INT0);
		DDRD&=~(1<<DDD2);
	}
	
	if(habl2){
		switch(modo2){
			case 0: //Interrupe por nivel bajo
			EICRA&=~(3<<ISC10);
			break;
			case 1:
			EICRA|=(1<<ISC10);
			break;
			case 2:
			EICRA|=(2<<ISC10);
			break;
			case 3:
			EICRA|=(3<<ISC10);
			break;
			
		}
		EIMSK = (1<<INT1);
		DDRD&=~(1<<DDD3);
	}
	
}

//Configuracion de preescaler Timer 1 (PWM) 16bits
void config_Prescaler_TC1(float T){
	T = (float) (T/1000.0);
	if(T == 0){prescalerValue_TC1=0;}
	float aux = (float) (pow(2,16)/F_CPU);
	if (T <= aux) prescalerValue_TC1=1;
	else if(T<= 8*aux)	prescalerValue_TC1=8;
	else if(T<= 64*aux) prescalerValue_TC1=64;
	else if(T<= 256*aux) prescalerValue_TC1=256;
	else if(T<= 1024*aux) prescalerValue_TC1=1024;
	
	TCCR1B &=~(7<<CS10); //Resetea a 0 bits de conf prescaler
	
	switch(prescalerValue_TC1){
		case 0:
		TCCR1B &=~(7<<CS10);
		break;
		case 1:
		TCCR1B |= (1<<CS10);
		break;
		case 8:
		TCCR1B |= (2<<CS10);
		break;
		case 64:
		TCCR1B |= (3<<CS10);
		break;
		case 256:
		TCCR1B |=(4<<CS10);
		break;
		case 1024:
		TCCR1B |=(5<<CS10);
		break;
	}
}

//Configuracion de preescaler Timer 0 (velocidad) 8bits
void config_Prescaler_TC0(float T){
	T = (float) (T/1000.0);
	float aux = (float) (pow(2,8)/F_CPU);
	if (T <= aux) prescalerValue_TC0=1;
	else if(T<= 8*aux)	prescalerValue_TC0=8;
	else if(T<= 64*aux) prescalerValue_TC0=64;
	else if(T<= 256*aux) prescalerValue_TC0=256;
	else if(T<= 1024*aux) prescalerValue_TC0=1024;
	else prescalerValue_TC0 =1024;
	
	TCCR0B &=~(7<<CS00); //Resetea a 0 bits de conf prescaler
	
	switch(prescalerValue_TC0){
		case 0:
		TCCR0B &=~(7<<CS00);
		break;
		case 1:
		TCCR0B |= (1<<CS00);
		break;
		case 8:
		TCCR0B |= (2<<CS00);
		break;
		case 64:
		TCCR0B |= (3<<CS00);
		break;
		case 256:
		TCCR0B |=(4<<CS00);
		break;
		case 1024:
		TCCR0B |=(5<<CS00);
		break;
	}
}

//Configuracion modo timer 1
void conf_modo_TC1(uint8_t modo){
	TCCR1A &=~(3<<WGM10);
	TCCR1B &=~(3<<WGM12);
	
	switch(modo){
		case 0:
		TCCR1A &=~(3<<WGM10);
		TCCR1B &=~(3<<WGM12);
		break;
		case 1:
		//PWM, PHASE CORRECTED, 8BIT
		TCCR1A |=(1<<WGM10);
		break;
		case 2:
		//PWM, PHASE CORRECTED, 9 BIT
		TCCR1A |=(1<<WGM11);
		break;
		case 3:
		//PWM, PHASE CORRECTED, 10 BIT
		TCCR1A |= (1<<WGM11 | 1<<WGM10);
		break;
		case 4:
		//CTC
		TCCR1B |= (1<<WGM12);
		break;
		case 5:
		//FAST PWM, 8 bit
		TCCR1A |=(1<<WGM10);
		TCCR1B |=(1<<WGM12);
		break;
		case 6:
		//FAST PWM, 9bit
		TCCR1A |=(1<<WGM11);
		TCCR1B |=(1<<WGM12);
		break;
		case 7:
		//FAST PWM, 10 BIT
		TCCR1A |=(1<<WGM10|1<<WGM11);
		TCCR1B |=(1<<WGM12);
		break;
		case 8:
		//PWM, PHASE AND HZ CORRECTED
		
		TCCR1B |=(1<<WGM13);
		break;
		
		case 9:
		TCCR1A |=(1<<WGM10);
		TCCR1B |=(1<<WGM13);
		break;
		case 10:
		TCCR1A |=(1<<WGM11);
		TCCR1B |=(1<<WGM13);
		break;
		case 11:
		TCCR1A |=(1<<WGM10|1<<WGM11);
		TCCR1B |=(1<<WGM13);
		break;
		case 12:
		TCCR1B |=(1<<WGM12|1<<WGM13);
		break;
		case 13:
		TCCR1A |=(1<<WGM10);
		TCCR1B |=(1<<WGM12|1<<WGM13);
		break;
		case 14:
		TCCR1A |=(1<<WGM11);
		TCCR1B |=(1<<WGM12|1<<WGM13);
		break;
		case 15:
		TCCR1A |=(1<<WGM10|1<<WGM11);
		TCCR1B |=(1<<WGM12|1<<WGM13);
		break;

	}

}

void conf_modo_TC0(uint8_t modo0){
	TCCR0A &=~(1<<WGM00);
	TCCR0A &=~(1<<WGM01);
	TCCR0B &=~(1<<WGM02);
	
	switch(modo0){
		case 0:
		TCCR0B &=~(1<<WGM02);
		break;
		case 1:
		//PWM, PHASE CORRECTED, 8BIT
		TCCR0A |=(1<<WGM00);
		break;
		case 2:
		//CTC TOP (OCR0A)
		TCCR0A |=(1<<WGM01);
		break;
		case 3:
		//FAST PWM 
		TCCR0A |= (1<<WGM01 | 1<<WGM00);
		break;
		case 4:
		//RESERVERD
		TCCR0B |= (1<<WGM02);
		break;
		case 5:
		//FAST PWM PC
		TCCR0A |=(1<<WGM00);
		TCCR0B |=(1<<WGM02);
		break;
		case 6:
		//reservado
		TCCR0A |=(1<<WGM01);
		TCCR0B |=(1<<WGM02);
		break;
		case 7:
		//FAST PWM (OCR0A)
		TCCR0A |=(1<<WGM00|1<<WGM01);
		TCCR0B |=(1<<WGM02);
		break;
		
}
}

//Modo de salido de pines timer 1
void conf_modo_salida_TC1(uint8_t outA, uint8_t outB){
	
	TCCR1A &= ~(3<<COM1A0);
	TCCR1A &= ~(3<<COM1B0);
	
	switch(outA){
		case 0:
		TCCR1A &= ~(3<<COM1A0);
		break;
		case 1://toogle
		TCCR1A |= (1<<COM1A0);
		break;
		case 2://clear
		TCCR1A |= (1<<COM1A1);
		break;
		case 3://set
		TCCR1A |= (3<<COM1A0);
		break;
		default:
		printf("Salida OC1A invalida\r\n");
		break;
		
	}
	switch(outB){
		case 0:
		TCCR1A &= ~(3<<COM1B0);
		break;
		case 1:
		TCCR1A |= (1<<COM1B0);
		break;
		case 2:
		TCCR1A |= (1<<COM1B1);
		break;
		case 3:
		TCCR1A |= (3<<COM1B0);
		break;
		default:
		printf("Salida OC1A invalida\r\n");
		break;
		
	}
}


//INTERRUPCIONES33

void interrupciones_TC1(uint8_t InputCapt, uint8_t OutputCaptA, uint8_t OutputCaptB, uint8_t Overflow){
	if(InputCapt){
		TIFR1 &= ~(1<<ICF1); //Apago el flag
		TIMSK1 |=(1<<ICIE1); //Habilita interrupcion
	}
	//PARA MODO 14 USO ESTE Y EL DE ARRIBA
	if(OutputCaptA){
		TIFR1 &= ~(1<<OCF1A);
		TIMSK1 |= (1<<OCIE1A);
	}
	if(OutputCaptB){
		TIFR1 &= ~(1<<OCF1B);
		TIMSK1 |= (1<<OCIE1B);
	}
	if(Overflow){
		TIFR1 &=~(1<<TOV1);
		TIMSK1 |= (1<<TOIE1);
	}
}

void interrupciones_TC0(uint8_t OutputCaptA, uint8_t OutputCaptB, uint8_t Overflow){
	if(OutputCaptA){
		TIFR0 &= ~(1<<OCF0A);
		TIMSK0 |= (1<<OCIE0A);
	}
	if(OutputCaptB){
		TIFR0 &= ~(1<<OCF0B);
		TIMSK0 |= (1<<OCIE0B);
	}
	if(Overflow){
		TIFR0 &=~(1<<TOV0);
		TIMSK0 |= (1<<TOIE0);
	}
	
	
};

//MODO PWM 14
void PWM_14(float T_mili,float Duty_Cycle){
	float aux1 = (float) ((T_mili*Duty_Cycle)/100.0);
	conf_modo_salida_TC1(3, 0);	
    conf_modo_TC1(14);
	interrupciones_TC1(1,1,0,0);
	config_Prescaler_TC1(T_mili);
	ICR1 = (uint16_t) ((T_mili/1000.0)*(F_CPU/prescalerValue_TC1)+1);
	OCR1A = (uint16_t) ((aux1/1000.0)*(F_CPU/prescalerValue_TC1)+1);
	
}

