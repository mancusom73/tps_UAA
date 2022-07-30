#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <alarma.h> 
#include <cr.h> 
#include <ctime.h> 
#include <dbrouter.h> 
#include <driver.h> 
#include <log.h> 
#include <mensaje.h> 
#include <pausa.h> 
#include <pinpad1.h> 
#include <scanner.h> 
#include <cmemory.h>
#include <ssaver.h> 
#include <status.h> 
#include <actions.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#endif
static int port;
int CALCULA_CRC_EAN( char *buffer, int largo, int empieza_impar );
static int ind_buffer_scanner,capturar,codigo_leido;
static char buffer_scanner[80];
static long tick1,tick2;
/*****************************************************************************/
int INIT_SCANNER()
/*****************************************************************************/
{
    int error = 0;
    #if defined(INVEL_W) || defined(INVEL_L)
    if( TIPO_DE_SCANNER == SCANNER_DRIVER ) {
        //error = InitScanner(  );
    }
    #else
    int base,parity,stop_bit,data_bit,baudios;
    if( TIPO_DE_SCANNER == SCANNER_DRIVER ) {
        DRIVER( _DRV_scanner, _SCA_inicializacion, 0, &error, sizeof( error ) );
    }
    else if( TIPO_DE_SCANNER != SCANNER_SPAR ) {
        port = SCANNER_PUERTO;
        base = ( port == 1 ) ? 0x3F8 : 0x2F8;
        data_bit = ( SCANNER_DATA_BITS == 7 ) ? 2 : 3;
        stop_bit = ( SCANNER_STOP_BITS == 1 ) ? 0 : 4;
        switch( SCANNER_PARITY ) {
            case 0:
                /*---- No paridad ----*/
                parity = 0x00;
                break;
            case 1:
                /*---- Impar ----*/
                parity = 0x08;
                break;
            case 2:
                /*---- Par ----*/
                parity = 0x18;
                break;
        }
        baudios = ( int )( 1843200L / ( 16L * SCANNER_BAUDIOS ) );
        outp( base + 3, 0x80 );
        outp( base, baudios );
        outp( base + 1, 0 );
        outp( base + 3, parity | stop_bit | data_bit );
        /* parity, data_bit,
         * stop_bit */
        COMMINIT( port );                         /* Inicializa puerta ........ */
        outp( base + 4, 11 );                     /* DTR y RTS on ............. */
        outp( base + 1, 0 );                      /* Reset interrupts on 8250 . */
        outp( base + 1, 1 );                      /* Set interrupts on 8250 ... */
    }
    #endif
    return ( !error );
}
#define SIZEOF_BUFF_SCANNER 256
char buff_scanner[SIZEOF_BUFF_SCANNER];
int GetDataScanner( int action, char *buffer, int len );
/*****************************************************************************/
int LEER_SCANNER( unsigned char *buffer, int l_buffer )
/*****************************************************************************/
{
    int rta = 0;
    if( SCANNER_HABILITADO_POR_TECLADO && codigo_leido ) {
        rta = 1;
        codigo_leido = 0;
        memcpy( buffer, buffer_scanner, ind_buffer_scanner );
        buffer[ind_buffer_scanner] = 0;
    }
    else if( TIPO_DE_SCANNER == SCANNER_DRIVER ) {
        if( l_buffer > SIZEOF_BUFF_SCANNER ) {
            l_buffer = SIZEOF_BUFF_SCANNER - 1;
        }
		/*if( PRESENTACIONES_EN_DUN14 || L_CODIGO == 14 )
		{
            largo_cod = 14;
   		}*/
		rta = getData( ACTION_READ_DATA_SCANNER, buff_scanner, l_buffer );
			memcpy( buffer, buff_scanner, l_buffer );
    }
	ENABLE_STANDBY_AUTOMATICO(  );
    return ( rta );
}
/*****************************************************************************/
int CRC_SCANNER( char *buffer, int largo, int empieza_impar )
/*****************************************************************************/
{
    int rta = 0;
    if( CALCULA_CRC_EAN( buffer, largo, empieza_impar ) == buffer[largo - 1] ) {
        rta = 1;
    }
    return ( rta );
}
/*****************************************************************************/
int IS_SCANNER()
/*****************************************************************************/
{
    int rta = 0, k = 0;

	if( TIPO_DE_SCANNER == SCANNER_DRIVER ) {
		rta = getData( ACTION_IS_SCANNER, NULL, 0 );
		if( rta == -1 )
			rta = 0;
		if(rta != 0)
			VERIFICAR_SCREEN_SAVER( &k );
	}
	if( SCANNER_HABILITADO_POR_TECLADO && codigo_leido ) {
        rta = 1;
    }
	
    return ( rta );
}
/*****************************************************************************/
void LIMPIAR_SCANNER()
/*****************************************************************************/
{
    
	setHabPing(0);	
#if defined(INVEL_W) || defined(INVEL_L)
	if( TIPO_DE_SCANNER == SCANNER_DRIVER ) {
		getData( ACTION_CLEAN_BUFFER_SCANNER, "", 0 );
	} 
#endif

	#if !defined(INVEL_W) && !defined(INVEL_L)
    char cadena[15];

    #ifdef COMPILAR_SCANNER_SPAR
    if( TIPO_DE_SCANNER == SCANNER_SPAR ) {
        while( STATUS() & 0x10 ) {
            LEER_SCANNER( cadena, sizeof( cadena ) );
            PAUSA( 2 );
        }
    }
    #endif
	if( TIPO_DE_SCANNER != SCANNER_DRIVER ) {
		COMMFLUSH( port );
	}
	else {
		while( !SCANNER_LIBRE() ) {
			LEER_SCANNER( cadena, sizeof( cadena ) );
			PAUSA( 2 );
		}
	}
    #endif
	setHabPing(1);
}

/*void LIMPIAR_BUFFER_SCANNER( void ) {
	if( TIPO_DE_SCANNER == SCANNER_DRIVER ) {
		getData( ACTION_CLEAN_BUFFER_SCANNER, "", 0 );
	}
}*/


/*****************************************************************************/
void SET_RTS_SCANNER( int estado )
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    //  EnableScanner( estado );
    #else
    int base,v;
    if( TIPO_DE_SCANNER != SCANNER_DRIVER ) {
        base = ( port == 1 ) ? 0x3F8 : 0x2F8;
        v = inp( base + 4 ) & 0xfd;
        if( estado ) {
            v |= 0x02;
        }
        outp( base + 4, v );
    }
    #endif
}
/*****************************************************************************/
int CALCULA_CRC_EAN( char *buffer, int largo, int empieza_impar )
/*****************************************************************************/
{
    int par = 0, impar = 0, h, suma;
    div_t division;
    for( h = 0;h < largo - 1;h += 2 ) {
        impar += ( buffer[h] - '0' );
    }
    for( h = 1;h < largo - 1;h += 2 ) {
        par += ( buffer[h] - '0' );
    }
    suma = ( empieza_impar ) ? ( impar + par * 3 ) : ( impar * 3 + par );
    division = div( suma, 10 );
    if( division.rem == 0 ) {
        division.rem = 10;
    }
    return ( 10 - division.rem + '0' );
}
/*****************************************************************************/
void CALCULA_CRC_EAN13( char *buffer )
/*****************************************************************************/
{
    int l;
    char c;
    l = strlen( buffer );
    c = ( char )CALCULA_CRC_EAN( buffer, l, SI );
    buffer[l - 1] = c;
}
/*****************************************************************************/
int LEER_CADENA_SCANNER( char *cadena, int largo )
/*****************************************************************************/
{
    char buffer[31];
    int l, rta = 0;
    memset( buffer, 0, sizeof( buffer ) );
    if( VARIOS_SCANNER_HABILITADO ) {
        if( IS_SCANNER() ) {
            if( LEER_SCANNER( (unsigned char *)buffer, sizeof( buffer ) ) ) {
				BEEP( 2000, 1000 );
                l = strlen( buffer );
                if( l > largo ) {
                    l = largo;
                }
                memcpy( cadena, buffer, l ); 
                rta = 1;
            }
        }
    }
    return ( rta );
}
/*****************************************************************************/
int SCANNER_LIBRE()
/*****************************************************************************/
{
    int rta = 1;
    if( VARIOS_SCANNER_HABILITADO ) {
        if( IS_SCANNER() ) {
            rta = 0;
        }
    }
    return ( rta );
}
/******************************************************************************/
int SCANNER_POR_TECLADO( int tecla )
/******************************************************************************/
{
    if( SCANNER_HABILITADO_POR_TECLADO ) {
        if( capturar ) {
            if( tecla == ( ( TIPO_TECLADO == 1 ) ? -999 : 0xf000 ) ) {
                tick2 = GET_TIME();
                if( tick2 - tick1 > 36 ) {
                    if( ind_buffer_scanner == 0 ) {
                        tecla = PREFIJO_SCANNER;
                    }
                    capturar = 0;
                }
            }
            else {
                if( tecla == SUFIJO_SCANNER ) {
                    capturar = 0;
                    codigo_leido = 1;
                }
                else {
                    if( ind_buffer_scanner < 80 ) {
                        buffer_scanner[ind_buffer_scanner++] = tecla;
                    }
                    else {
                        capturar = 0;
                    }
                }
            }
            tick1 = GET_TIME();
            tecla = ( TIPO_TECLADO == 1 ) ? -999 : 0xf000;
        }
        else {
            if( tecla == PREFIJO_SCANNER ) {
                capturar = 1;
                tick1 = GET_TIME();
                ind_buffer_scanner = 0;
                tecla = ( TIPO_TECLADO == 1 ) ? -999 : 0xf000;
            }
        }
    }
    return ( tecla );
}

