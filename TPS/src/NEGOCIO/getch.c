#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <abort.h>
#include <cargacon.h>
#include <cmemory.h>
#include <config.h>
#include <cr.h>
#include <cr_disc2.h>
#include <cr_disco.h>
#include <cr_displ.h>
#include <cr_pant.h>
#include <ctime.h>
#include <dbrouter.h>
#include <dd.h>
#include <driver.h>
#include <e_mens.h>
#include <getch.h>
#include <malloc.h>
#include <omron.h>
#include <panel.h>
#include <pant.h>
#include <power.h>
#include <print.h>
#include <scanner.h>
#include <ssaver.h>
#include <stand_by.h>
#include <status.h>
#include <tar_mag.h>
#include <tcp_ip.h>
#include <tkt.h>
#include <_cr0.h>
#include <path.h>
#include <ini.h>
#include <b_mens.h>
#include <log.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifndef INVEL_L
#include <dos.h>
#include <conio.h>
#endif
#include <stdlib.h>
#if defined(INVEL_W) || defined(INVEL_L)
#include "actions.h"
#include <cargacon.h>
#else
#include <bios.h>
#endif
#include <cr_disc2.h>
#include <time.h> // time_t, time, ctime
/*------------------------ variables estaticas -----------------------*/
static int _ungetch_, ultima_tecla, _pad_numerico = 1, inicializar_demora_set = 1;
static int _ESPERA = 0;
static unsigned long _clock, _clock_cambios = 1, _clock_stand_by_vtas = 0, _clock_stand_by_cob = 0, _clock_otros = 0;
#ifdef COMPILAR_DISPLAY
static int _mostrar_hora;
#endif
#if defined(INVEL_W) || defined(INVEL_L)
/* A este vector se le agrego una columna mas para el touch screen */
static int teclas[151][4];
#else
#define LONGITUD_TABLA_TECLAS 151
static int *teclas;
#endif
int enable_standby_automatico = 0;
extern int RestaurarPrimerPanel;
extern int PANT_VENTAS_ACTIVA( void );
extern int WIN_ACTIVA_ONTOP( void );
extern int PANT_COBROS_ESP_ACTIVA( void );
extern int IsStandbyActivo();
#if defined(INVEL_W) || defined(INVEL_L)
int getData( int action, char *buffer, int len );
#endif
int GET_FUNCION_TEC( int k);
int DEMORA2( unsigned long _clock );
extern unsigned long GET_TIME2(  );
extern BLOQUEAR_DATOS;
int disable_standby_automatico = 0;
extern int IsStandbyActivo();
/******************************************************************************/
int GETCH()
/******************************************************************************/
{
    int k = 0xf000, salir = 0;
    long cntSegArch = 0;
    long cntSegStandBy = 0;
 
	disable_standby_automatico = 0;
	if( _ungetch_ ) {
        k = _ungetch_;
        _ungetch_ = 0;
    }
	else {
        while( k == 0xf000 ) {
			if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
				////CONTROLAR_ESTADO_IMPRESORA(  ); seba
			}


			k = SCANNER_POR_TECLADO( LECTOR_BANDA( _GETCH(  ) ) );
			
			if( k == -999 && !RAM_P_TICKET 
					&& SEGUNDOS_PARA_MODO_STAND_BY 
					&& _clock_stand_by_vtas
					&& ( DEMORA2( _clock_stand_by_vtas ) > SEGUNDOS_PARA_MODO_STAND_BY  ) 
					&& PANT_VENTAS_ACTIVA()) {
				if( WIN_ACTIVA_ONTOP() ) {
					k = 27;
				}
				STAND_BY( 1, _MODO_VENTA );
				_clock_stand_by_vtas = GET_TIME2(  );
			}

			if( k == -999 && _clock_stand_by_vtas
					&& SEGUNDOS_PARA_MODO_STAND_BY
					&&!PANT_VENTAS_ACTIVA())//sino esta activa que comience de nuevo
				_clock_stand_by_vtas = GET_TIME2(  );

			if( k == -999 && !RAM_P_TICKET
					&& SEGUNDOS_PARA_MODO_STAND_BY
					&& _clock_stand_by_cob
					&& ( DEMORA2( _clock_stand_by_cob ) > SEGUNDOS_PARA_MODO_STAND_BY  )
					&& PANT_COBROS_ESP_ACTIVA()) {
				if( WIN_ACTIVA_ONTOP() ) {
					k = 27;
				}

				STAND_BY( 1, _MODO_COBRANZA );
				_clock_stand_by_cob = GET_TIME2(  );
			}

			if( k == -999 && _clock_stand_by_cob
					&& SEGUNDOS_PARA_MODO_STAND_BY
					&& !PANT_COBROS_ESP_ACTIVA())//sino esta activa que comience de nuevo
				_clock_stand_by_cob = GET_TIME2(  );

            if( k == -999 && !RAM_P_TICKET 
					&&_clock_cambios 
					&& SEGUNDOS_VERIFICAR_CAMBIO_ARCHIV 
					&& ( SEGUNDOS_VERIFICAR_CAMBIO_ARCHIV ) <= DEMORA2( _clock_cambios ) ){
			
				if( BLOQUEAR_DATOS != 1 || IsStandbyActivo( ) )
				   VERIFICA_CAMBIO_ARCHIVOS_ACTIVOS( NO );

				_clock_cambios = GET_TIME2(  );
				
			}
			if( k == -999 && !RAM_P_TICKET 
					&& BLOQUEAR_DATOS != 1 
					&& ( DEMORA2( _clock_otros ) > 5 )  ){
				CAJON_CERRADO();
				_clock_otros= GET_TIME2(  );
				if(_CAJON_POR_IMPRESORA_ABIERTO == SI )
					BORRAR_MENSAJE();

			}
		}

 		VERIFICAR_SCREEN_SAVER(&k);

		if( k == -999
				&& PANT_VENTAS_ACTIVA()
				&& WIN_ACTIVA_ONTOP()
				&& enable_standby_automatico != 2 ) {
			//Habilito standby automatico estando en pant venta e input activado
			//sin haber ingresado caracter en el input (Ej clave)
			if( enable_standby_automatico == 0 ) {
				enable_standby_automatico = 1;
			} else {
				enable_standby_automatico = 2;
			}
		}

		if( k != -999 || enable_standby_automatico == 1 ) {
        	ENABLE_STANDBY_AUTOMATICO(  );
        	ENABLE_VERIFICAR_CAMBIO_ARHIVOS(  );
        	if( k != -999 )
        		enable_standby_automatico = 0;
		}

        /*------------------- busca la tecla presionada ------------------*/
		if( k != -999)
			k = GET_FUNCION_TEC(k);

        /*--------------- teclas especiales -------------------*/
        switch( k ) {
            case -1:
                /*------- doble 0 -------*/
                k = '0';
                if( HARD_OMRON || TIPO_TECLADO > 1 ) {
                    UNGETCH( '0' );
                }
                break;
            case -2:
                /*------ LF receipt -----*/
                // LF_RECEIPT( 1 );
                k = 0;
                break;
            case -3:
                /*------ LF journal -----*/
                // LF_JOURNAL( 1 );
                k = 0;
                break;
            case -4:
                /*------ LF journal -----*/
                //MOSTRAR_MEMORIA_DISPONIBLE();
                k = 0;
                break;
            case '!':
                /*------- Mensajes ---------*/
                //RestaurarPrimerPanel=1;
                if( MENSAJES_HABILITADOS ) {
                    SET_MEMORY_CHAR( __mensajes_habilitados, 0 );
                    EMITIR_MENSAJES();
                    SET_MEMORY_CHAR( __mensajes_habilitados, 1 );
                }
                k = 0;
                break;
            case -19:
                /*-------- Reset Pinpad ---------*/
                //RestaurarPrimerPanel=1;
                //RESET_PINPAD();
                //STATUS();
                k = 0;
                break;
        }
	}
ultima_tecla = k;
if( k != -999 ) {
    DEMORA_SET();
	{
		char strTmp[50];
		_snprintf(strTmp, sizeof(strTmp)-1,"Tecla ASC: %i CHR: %c",k, (char)k);
		GRABAR_LOG_SISTEMA_CONFIG(strTmp,LOG_VARIOS,5);
	}
}

return ( k );
}


/*****************************************************************************/
int FAST_GETCH()
/*****************************************************************************/
{
    int k;
    k = SCANNER_POR_TECLADO( LECTOR_BANDA( _GETCH(  ) ) );
    k = GET_FUNCION_TEC(k);
    ultima_tecla = k;
    return k;

}
/*****************************************************************************/
int GET_FUNCION_TEC( int k)
/*****************************************************************************/
{
    int h = 0, salir = 0, rta = 0;
    rta = k;
    if( _TOUCH_SCREEN ){
        for( h = 0; teclas[h][0] != -999 ; h++ ) {
            if( teclas[h][3] == k ) {
                rta = ( TECLADO_ALFABETICO ? teclas[h][2] : teclas[h][1] );
                break;
            }
        }
    }else{
        if( k != 13 ) {
            for( h = 0; teclas[h][0] != -999 && !salir; h++ ) {
                if( teclas[h][0] == k ) {
                    rta = ( TECLADO_ALFABETICO ) ? ( ( teclas[h][2] ) ? teclas[h][2] : k )
                        : teclas[h][1];
                    salir = 1;
                }
            }
        }
    }
    return rta;
}

/*****************************************************************************/
void UNGETCH( int caracter )
/*****************************************************************************/
{
    _ungetch_ = caracter;
}
/*****************************************************************************/
void SET_LASTKEY( int tecla )
/*****************************************************************************/
{
    ultima_tecla = tecla;
}
/*****************************************************************************/
int LASTKEY()
/*****************************************************************************/
{
    return ( ultima_tecla );
}

/*****************************************************************************/
void VACIAR_KEY()
/*****************************************************************************/
{
    int limit = 50;
    while( FAST_GETCH() != -999 && limit-- > 0 );
}

/******************************************************************************/
void SET_ESPERA_GETCH( int ticks )
/******************************************************************************/
{
    _ESPERA = ticks;
}
/******************************************************************************/
int GET_ESPERA_GETCH()
/******************************************************************************/
{
    return ( _ESPERA );
}
/******************************************************************************/
void DEMORA_SET()
/******************************************************************************/
{
    _clock = GET_TIME();
}
/******************************************************************************/
int DEMORA( unsigned long _clock )
/******************************************************************************/
{
    unsigned long t;
    int dif;
    t = GET_TIME();
    if( t < _clock ) {
        dif = ( int )( 0xffffffffL - _clock + t );
    }
    else {
        dif = ( int )( t - _clock );
    }
    return ( dif );
}
/******************************************************************************/
int DEMORA2( unsigned long _clock )
/******************************************************************************/
{
	long  actual;

	time( &actual );
	return( actual - _clock );
}
/******************************************************************************/
int INICIALIZAR_TECLADO()
/******************************************************************************/
{
    int ok = 0;
    memset( teclas, 0, sizeof( teclas ) );
    /* Se cambia el tercer parametro de la funcion para permitir un dato mas correspondiente */
    /* a la configuracion de comandos del touch screen */
    if( TIPO_TECLADO == TECLADO_DRIVER ) {
		ok = CARGAR_ARCHIVO_DE_COMANDOS( TEC_JS_DEF, ( int* )teclas, 4 );
	} else {
		switch ( TIPO_TECLADO ) {
   	 case TECLADO_PC:	          //0 TEC_PC_DEF
   	   ok = CARGAR_ARCHIVO_DE_COMANDOS( TEC_PC_DEF, ( int* )teclas, 4 );
       break;
       case TEC_OM:           //1
		   if( HARD_OMRON )
				ok = CARGAR_ARCHIVO_DE_COMANDOS( TEC_OM_DEF, ( int* )teclas, 4 );
		   else
			   ok = CARGAR_ARCHIVO_DE_COMANDOS( TEC_INV_DEF, ( int* )teclas, 4 );
       break;
       case TEC_MIX:           //2
          ok = CARGAR_ARCHIVO_DE_COMANDOS( TEC_MIX_DEF, ( int* )teclas, 4 );
       break;
		 case TECLADO_KB78:           //3
		    ok = CARGAR_ARCHIVO_DE_COMANDOS( TEC_KB78_DEF, ( int* )teclas, 4 );
		 break;
		 case TECLADO_PKBST_50:           //4
		    ok = CARGAR_ARCHIVO_DE_COMANDOS( PKBST_50_DEF , ( int* )teclas, 4 );
		 break;
		 case TEC_3500:           // 5
		    ok = CARGAR_ARCHIVO_DE_COMANDOS( TEC_3500_DEF, ( int* )teclas, 4 );
		 break;
		 case TECLADO_CARY:           //7
		    ok = CARGAR_ARCHIVO_DE_COMANDOS( TEC_CARY_DEF, ( int* )teclas, 4 );
		 break;
       default :
   		ok = CARGAR_ARCHIVO_DE_COMANDOS( TEC_PC_DEF, ( int* )teclas, 4 );
   		break;
   	}
	}


 //   /* Chequeo si es teclado touch */
 //   if(_TOUCH_SCREEN){
 //       Inicializacion del cliente servidor del touch screen */
 //       if (INICIO_TOUCH()){
 //           ABORT( "ERROR AL INICIALIZAR EL DRIVER DEL TOUCH" );
 //       }
 //   }
    return ( ok );
}
/******************************************************************************/
void SET_NUM_LOCK( int prender )
/******************************************************************************/
{
    SET_LOCK( 32, prender );
}
/******************************************************************************/
void SET_CAPS_LOCK( int prender )
/******************************************************************************/
{
    SET_LOCK( 64, prender );
}
/******************************************************************************/
void SET_LOCK( unsigned char mascara, int prender )
/******************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    if( prender ) {
        pokeb( 0x40, 0x17, peekb( 0x40, 0x17 ) | mascara );
    }
    else {
        pokeb( 0x40, 0x17, peekb( 0x40, 0x17 ) & ( ~mascara ) );
    }
    #endif
}
/******************************************************************************/
int KBHIT()
/******************************************************************************/
{
    int st = 1;
    return ( st );
}
/******************************************************************************/
int _GETCH()
/******************************************************************************/
{
    int tecla = 0;
    int rta = 0;

	if( TIPO_TECLADO == TECLADO_DRIVER ) {
        /*  Pide tecla al JavaServer */
        rta = getData( ACTION_GETCH_KEY, ( char* )&tecla, sizeof( int ) );
		//rta = getData( ACTION_READ, ( char* )&tecla, sizeof( int ) );
		if( tecla == -999 ){
			rta = getData( ACTION_READ, ( char* )&tecla, sizeof( int ) );
			if( rta == 0 ) {
				tecla = -999;
			}
		}

    }
    /* Implementacion de lectura del touch screen */
	else if( _TOUCH_SCREEN ) {
        tecla = GET_TOUCH();
    }
    else {
		rta = getData( ACTION_READ, ( char* )&tecla, sizeof( int ) );
        if( rta == 0 ) {
            tecla = -999;
        }
		if( tecla == 126 && TIPO_TECLADO == 5 )
        #ifndef INVEL_L
        {
            tecla = getch() + 200;
        }
        #else
        ;
        #endif
    }

    return ( tecla );
}
#ifdef COMPILAR_DISPLAY
/******************************************************************************/
void SET_MOSTRAR_HORA_DISPLAY( int status )
/******************************************************************************/
{
    if( MOSTRAR_HORA_DISPLAY ) {
        _mostrar_hora = status;
    }
}
/******************************************************************************/
int GET_MOSTRAR_HORA_DISPLAY()
/******************************************************************************/
{
    return ( _mostrar_hora );
}
#endif
/******************************************************************************/
void ENABLE_VERIFICAR_CAMBIO_ARHIVOS()
/******************************************************************************/
{
    _clock_cambios = GET_TIME2();
}
/******************************************************************************/
void DISABLE_VERIFICAR_CAMBIO_ARHIVOS()
/******************************************************************************/
{
    _clock_cambios = 0L;
}
/******************************************************************************/
void ENABLE_PAD_NUMERICO()
/******************************************************************************/
{
    _pad_numerico = 1;
}
/******************************************************************************/
void DISABLE_PAD_NUMERICO()
/******************************************************************************/
{
    if( ancho_panel ) {
        _pad_numerico = 0;
    }
}
void VERIFICA_CAMBIOS()
{
    if( SEGUNDOS_VERIFICAR_CAMBIO_ARCHIV && _clock_cambios
     && ( DEMORA2( _clock_cambios ) > SEGUNDOS_VERIFICAR_CAMBIO_ARCHIV ) ) {
        _clock_cambios = GET_TIME2();
    }
}
/******************************************************************************/
void ENABLE_STANDBY_AUTOMATICO()
/******************************************************************************/
{
    if( cajero_operando ) {
        ( _clock_stand_by_vtas ) = GET_TIME2();
    }
	if( cajero_operando ) {
		( _clock_stand_by_cob ) = GET_TIME2();
	}
}
/******************************************************************************/
void DISABLE_STANDBY_AUTOMATICO()
/******************************************************************************/
{
	( _clock_stand_by_vtas ) = 0L;
	( _clock_stand_by_cob ) = 0L;
}
/******************************************************************************/
void INICIALIZAR_DEMORA_SET( int valor )
/******************************************************************************/
{
    inicializar_demora_set = valor;
}
/*******************************************************************************
*
*   Funcion     :   INICIO_TOUCH
*   Entrada     :   void.
*   Salida      :   int init.
*   Descripcion :   Esta funcion inicializa el puerto tcp ip de comunicacion con
*                   la interface touch screen.
*
*   Historial de Revisiones:
*   dd/mm/aa    Autor               Incidente   Descripcion
*   --------    -----               ---------   -----------
*   28/06/06    Gavilan_Sebastian       22089   Creacion inicial.
********************************************************************************/
int INICIO_TOUCH( void )
{
    int status = 0;  /* Estado a devolver si ocurrio algun problema */
    /* Inicializo el server */
    DRIVER( _DRV_tcp, _TCP_inicializacion, _CANAL_TOUCH_SCREEN, &status, sizeof( int ) );
    /* Abro el canal    */
    /* Localhost        */
    ABRIR_CANAL_TCP_IP( _CANAL_TOUCH_SCREEN, "localhost", _FORMATO_MENSAJE_INVEL );
    return status;
}
/*******************************************************************************
*
*   Funcion     :   GET_TOUCH
*   Entrada     :   void.
*   Salida      :   int tecla - valor del touch.
*   Descripcion :   Esta funcion le pide a la interface touch screen el valor
*                   de retorno de la tecla oprimida.
*
*   Historial de Revisiones:
*   dd/mm/aa    Autor               Incidente   Descripcion
*   --------    -----               ---------   -----------
*   28/06/06    Gavilan_Sebastian       22089   Creacion inicial.
********************************************************************************/
int GET_TOUCH (void ){
    int     valor   = 0;
	do{
		valor=getch();
	}while(valor==-1 );
    if( valor > 127 ) {
        valor = valor - 256;
    }
    return valor;
}

