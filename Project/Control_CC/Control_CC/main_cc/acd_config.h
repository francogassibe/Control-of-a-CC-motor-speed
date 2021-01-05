#include <avr/io.h>
#include <stdint.h>

#define enerAD (PRR &=~(1<<PRADC)) //ENERGIZAR EL MODULO AD
#define flagAD_OFF (ADCSRA &=~(1<ADIF))//BORRAR FLAG DE INTERRUPCION POR FIN DE CONVERSION
#define verif_F_AD (ADCSRA & (1<<ADIF)) //VERIFICACION FLAG DE INTERRIPCION POR FINALIZACION DE CONVERSION
#define init_ConversionAD (ADCSRA |= (1<<ADSC)) //INICIA LA CONVERSION

//CONFIGURACION DE AD
void conf_AD(void);
	//Tension de referencia
	typedef enum
	{
		Aref = 0, //PARA CONFIGURAR BITS 7  y 8
		Avcc = 1,
		Internal1v1 =3
		
		
		}tVref;

//CONFIGURACION DEL CLOCK DEL SISTEMA DE CONVERSION
//EL TIEMPO DE CONVERSION ES APROXIMADAMENTE F_CPU/(PRESCALER * 15)
typedef enum
{
	AD_pres_2 =0,
	AD_pres_4 =2,
	AD_pres_8 =3,
	AD_pres_16 =4,
	AD_pres_32 =5,
	AD_pres_64 =6,
	AD_pres_128 =7
	
}tPres;
	

typedef enum{
	intSI =1,
	intNO =0
}tInterrupt;

//CONFIGURACION DE MODOS DE AC
//PARA CONVERSION SEGUN EL EVENTO INDICADO EN trigSrc atSI
//SINO conversion manual
	
typedef enum{
	atSI =1,
	atNO =0
	}tAutotrig;

//SI atSI entonces

typedef enum{
	free_r = 0, //FREE RUNNING, CUANDO TERMINA DE CONVERTIR COMIENZA UNA NUEVA
	comp_Analogico=1, //Cambio en comparar analogico
	interup_Ext_INT0 =2, //Evento en la interrupcion externa
	Timer0_CompA =3,
	Timer0_OverFlow = 4
	}tTriggerSel;
	
typedef struct{
	uint8_t canal;
	tVref tensionRef;
	tPres prescaler;
	tInterrupt interrupcion;
	tAutotrig autoTrigger;
	tTriggerSel tipoTrigger;
	uint8_t disDIs;
	}tADconfig;


tADconfig pruebaADconfig; //Defino una variable del tipo tADconfig
void iniciarAD(tADconfig * ADconfig){
	DIDR0 |= ADconfig->disDIs;
	ADMUX |= (ADconfig->tensionRef<<REFS0);
	enerAD;
	ADCSRA =0;
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (ADconfig->autoTrigger<<ADATE);
	ADCSRA |= (ADconfig->prescaler<<ADPS0);
	ADCSRA |= (ADconfig->interrupcion<<ADIE);
}

//implementacion de la funcion confAD(), aqui se definen todos los paramentros
//para despues modificar los registros del modulo AD.

void conf_AD(){
	pruebaADconfig.prescaler = AD_pres_128;
	pruebaADconfig.interrupcion = intNO;
	pruebaADconfig.autoTrigger = atNO;
	pruebaADconfig.tensionRef = Avcc;
	pruebaADconfig.disDIs = 0b00111111; //Esto es para deshabilitar los pines digitales de los 6 canales analogicos
	
	
	iniciarAD(&pruebaADconfig);
}


uint16_t ReadAD(uint8_t canal){
	uint8_t aux;
	aux = ADMUX&0xF0;
	aux |= (canal&0x0F);
	ADMUX =aux;
	flagAD_OFF;
	init_ConversionAD;
	while(!verif_F_AD);
	return ADC;
}

float C_corriente(uint16_t auxAD[10]){

	float promedio =(float) ((auxAD[0]+auxAD[1]+auxAD[2]+auxAD[3]+auxAD[4]+auxAD[5]+auxAD[6]+auxAD[7]+auxAD[8]+auxAD[9])/10.0);
		
	return (float) (((promedio*5.0)/1023.0)*10.0);
		
}
