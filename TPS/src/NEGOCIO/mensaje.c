#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <alarma.h> 
#include <b_mens.h> 
#include <cr.h> 
#include <getch.h> 
#include <mensaje.h> 
#include <pausa.h> 
#include <tkt.h> 
#include <cmemory.h>
#include <ini.h>
#include <log.h>
#include <deftables.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
extern int _GRANDE;
extern int TIPO_DE_LETRA;
extern int ESTACION_SELECCIONADA;
extern char _ATRIBUTO;
int ALERTA_EN_VENTANA2( char *mensaje,char *titulo, unsigned int iconos, char* titulo_venta_activa, int pregunta );
/******************************************************************************/
void MENSAJE( char *cadena )
/******************************************************************************/
{
    unsigned long pausa;
	char buff[1024];

	memset( buff, 0, sizeof( buff ) );    
    ALARMA( 1 );
    pausa = PAUSA_EN_MENSAJES;
    if( pausa > 127 || pausa < 0 ) {
        pausa = 127;
    }
    pausa = pausa * 100;
	//pausa = pausa * 1000;// es demasiado tiempo de exposicion y los sleep y ciclos posteriores retrasan mucho en el funcionamiento de la caja, al solucion requiere sincronizarse
	_snprintf( buff, 1023, "MSG: Mensaje %s", cadena );
	buff[1023] = '\0';
	
	GRABAR_LOG_SISTEMA_CONFIG( buff, LOG_VARIOS,3);
    MENSAJE_SIN_SONIDO( cadena,pausa );
    //PAUSA( pausa );
}

/******************************************************************************/
void MENSAJE_SIN_PAUSA( char *cadena )
/******************************************************************************/
{
	char buff[1024];

	memset( buff, 0, sizeof( buff ) );
	MENSAJE_SIN_SONIDO( cadena,NO );
	_snprintf( buff, 1023, "MSG: Mensaje Sin Pausa %s", cadena );
	buff[1023] = '\0';
	GRABAR_LOG_SISTEMA_CONFIG( buff, LOG_VARIOS,4);
	//ALARMA( 1 );
}

/******************************************************************************/
void MENSAJE_CON_PAUSA( char *cadena, int pausa )
/******************************************************************************/
{
	char buff[1024];

	memset( buff, 0, sizeof( buff ) );
    MENSAJE( cadena );
	_snprintf( buff, 1023, "MSG: Mensaje Con Pausa %s", cadena );
	buff[1023] = '\0';
	GRABAR_LOG_SISTEMA_CONFIG( buff, LOG_VARIOS,2);
    //PAUSA( pausa );
}

/******************************************************************************/
void MENSAJE_CON_PAUSA_CAJA( char *cadena, int pausa )
/******************************************************************************/
{
	char buff[1024];

	memset( buff, 0, sizeof( buff ) );	
	MENSAJE_SIN_SONIDO( cadena,NO );
	_snprintf( buff, 1023, "MSG: Mensaje Con Pausa Caja %s", cadena );
	buff[1023] = '\0';
	GRABAR_LOG_SISTEMA_CONFIG( buff, LOG_VARIOS,2);
    PAUSA( pausa );
}

/******************************************************************************/
void MENSAJE_CON_TECLA( char *cadena, int tecla )
/******************************************************************************/
{
	char buff[1024];

	memset( buff, 0, sizeof( buff ) );
	_snprintf( buff, 1023, "MSG: Mensaje Con Tecla %s", cadena );
	buff[1023] = '\0';
	ALARMA( 1 );	
	MENSAJE_SIN_SONIDO( cadena,NO );
	while( GETCH() != tecla );
	BORRAR_MENSAJE();
	GRABAR_LOG_SISTEMA_CONFIG( buff, LOG_VARIOS,2);
    
}
/******************************************************************************/
void MENSAJE_TECLA( char *cadena )
/******************************************************************************/
{
	char buff[1024];
	
	memset( buff, 0, sizeof( buff ) );
    MENSAJE_SIN_PAUSA( cadena );
    while( GETCH() == -999 );
	_snprintf( buff, 1023, "MSG: Mensaje Tecla %s", cadena );
	buff[1023] = '\0';
    BORRAR_MENSAJE();
	GRABAR_LOG_SISTEMA_CONFIG( buff, LOG_VARIOS,2);

}
/******************************************************************************/
int MENSAJE_CON_TECLA_S_N( char *cadena, int tecla_si, int tecla_no )
/******************************************************************************/
{
	char buff[1024];
    int tecla = 0;

	memset( buff, 0, sizeof( buff ) );
    MENSAJE( cadena );
    while( (tecla = GETCH()) != tecla_si && tecla != tecla_no );
	_snprintf( buff, 1023, "MSG: Mensaje Con Teca S/N %s", cadena );
	buff[1023] = '\0';
	GRABAR_LOG_SISTEMA_CONFIG( buff, LOG_VARIOS,2);
    BORRAR_MENSAJE();

    return tecla;

}

/******************************************************************************/
int MENSAJE_CON_TECLA2_S_N( char *cadena, int tecla_si, int tecla_no )
/******************************************************************************/
{
	char buff[1024];
    int tecla = 0;
    int respu =0;

	memset( buff, 0, sizeof( buff ) );
//hmm notificaciones este va por zenity
	//'s' = 115 //'S' = 83 //'n' = 110 //'N' = 78
	 respu= ALERTA_EN_VENTANA2( cadena,"Mensaje Informativo", 3, "tps - v",1, tecla_si,tecla_no );
	 
	 if(respu == 6)
		 tecla = tecla_si;
	 else
		 tecla = tecla_no;
	  //0 ok SI  , 256 CANCEL, NO
	_snprintf( buff, sizeof(buff), "MSG: Mensaje Con Tecla S/N %s", cadena );

	buff[sizeof(buff)-1]='\0';
	GRABAR_LOG_SISTEMA_CONFIG( buff, LOG_VARIOS,2);

    return tecla;

}

