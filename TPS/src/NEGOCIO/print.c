/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#pragma pack(1)
#include <cr.h>
#include <stdlib.h>
#include <string.h>
#if !defined(INVEL_W) && !defined(INVEL_L)
#include <bios.h>
#endif
#include <fcntl.h>
#undef MK_FP
#ifndef INVEL_L
#include <dos.h>
#include <io.h>
#else
#include <sys/io.h>
#endif
#include <stdio.h>
#include <driver.h>
#include <aarch.h>
#include <memory.h>
#include <sys/stat.h>
#include <btrv.h>
#if defined(INVEL_W) || defined(INVEL_L)
#include "actions.h"
#include <print.h>
#endif
#include <path.h>
#include <malloc.h>
#include <cmemory.h>
#include <print.h>
#include <ctime.h>
#include <comprob.h>
#include <tkt.h>
#include <imp_tick.h>
#include <log.h>
#include <mensaje.h>
#include <b_mens.h>
#include <fiscal.h>
#include <pausa.h>
#include <config.h>
#include <cio.h>
#include <m_cadena.h>
#include <_cr1.h>
#include <b_mens.h>
#include <timer.h>
#include <archivos.h>
#include <export.h>
#include <str.h>
#include <calcular.h>
/*------------------------------- Variables ---------------------------------*/
static int _SLIP = 0, _FILTRAR_CARACTERES_ESPECIALES = 0;
static int IMPRESORA_INSTALADA = 1;
static char SPOOLER;
static char ANCHO_CHAR = 1;
static char _JORNAL = 0;
static char _COPIA_PRN = 0;
char _COPIA_VOUCHER_PRN = 0;
static int handle_copia;
static int handle_ticket_copia;
static int handle_voucher_copia;
static int handle_journal,p_buffer_journal;
static int handle_cierre_z_copia;
static char *buffer_journal;
static int handle_bmp;
int grabar_copia_ticket = 0;
int grabar_copia_cierre_z = 0;
extern int LINEA_IMPRESA;
extern int _SI_CANT_IMPRESION;
extern int *_SI_REPETIR_CADENA;
extern char _SI_CAD_IMPRESION[50];
void CONTINUA_COPIA_VOUCHER_PRN();
#ifdef DEBUG_IMPRESION	
	extern void LF_AUX(int cantidad);
#endif

extern void MOSTRAR_MENSAJE2( char *s, unsigned long pPausa, int pCore );
//#define TO_POS_PRINTER_JOURNAL_RECEIPT  3
//#define TO_POS_PRINTER_RECEIPT          2

#define TO_POS_PRINTER_JOURNAL         1
#define TO_POS_PRINTER_RECEIPT         2
#define TO_POS_PRINTER_JOURNAL_RECEIPT 3
#define TO_POS_PRINTER_JOURNAL_PART    31
#define TO_POS_PRINTER_RECEIPT_PART    32
#define TO_POS_PRINTER_SLIP            4
#define TO_FISCAL_PRINTER_JRN          5
#define TO_FISCAL_PRINTER_REC          6
#define TO_FISCAL_PRINTER_REC_JRN      7
#define TO_FISCAL_PRINTER_SLP          8








#define SIZE_BUFFER_JOURNAL 100
#define SIZE_JOURNAL_FILE 600000
static int __TIPO_IMPRESORA,__PUERTO_PARALELO,__PUERTO_IMPRESORA;
static char _corto_hoja;
static long _clock_corte_hoja;
static char _ESTACION_ACTIVA = RECEIPT;
#define ENVIAR_A_SERIAL
/*--------------------------- Buffer de Impresora ---------------------------*/
#define LARGO_BUFFER 1000
static char *buffer_imp;
static int p_buffer_1 = 0, p_buffer_2 = 0;
/*------------------------- Definiciones de Cdigo --------------------------*/
#define INCREMENTA_CABEZA if(++p_buffer_2 == LARGO_BUFFER ) p_buffer_2 = 0
#define DECREMENTA_CABEZA if(--p_buffer_2 == -1 ) p_buffer_2 = LARGO_BUFFER-1
#define INCREMENTA_COLA   if(++p_buffer_1 == LARGO_BUFFER ) p_buffer_1 = 0
#define ON  1
#define OFF 0
/*------------------------ Comandos de la impresora -------------------------*/
//static int _comandos_prn[40][10];
static int *_comandos_prn = 0;
//if ( !_comandos_prn ) _comandos_prn = malloc ( sizeof(int[40][10]) );
#define _prn_inicializar                 0
#define _prn_corte_total                 1
#define _prn_continua                    2
#define _prn_discontinua                 3
#define _prn_letra_normal                4
#define _prn_letra_chica                 5
#define _prn_letra_grande                6
#define _prn_habilita_deteccion_papel    7
#define _prn_select_receipt_y_journal    8
#define _prn_select_receipt              9
#define _prn_select_journal             10
#define _prn_habilita_slip              11
#define _prn_inhabilita_slip            12
#define _prn_set_espaciado              13
#define _prn_espaciado_normal           14
#define _prn_feed                       15
#define _prn_select                     16
#define _prn_select_para_espaciado      17
#define _prn_inicializar_prn_opcional   18
#define _prn_inicializar_ticket         19
#define _prn_2pasadas                   20
#define _prn_micro_lf                   21
#define _prn_habilitar_cheque           22
#define _prn_leer_cheque                23
#define PUERTO_FISCAL_IBM_4614 2    // COM3
int _IMPRIME_COPIA_PRN( char *archivo, int copiando_a_archivo, int imprimir_cabecera );
int Handle_Fiscal_IBM_4614;
static char redireccionar_print_a_buffer,cadacum[100];
static int ind_acum,redireccion_columna,redireccion_fila,redireccion_cant_columnas,
           redireccion_cant_filas;
char *PRINT_BUFFER;
extern int cantCopias;
extern int cantTotalCopias;
int imprime_copia_voucher = 0;
int printer_status_2 = 0;
extern int TIPO_DE_LETRA;

extern void cio_Write( char *buffer, int cant, char *filename );
//Funcion de IPantalla.cpp (sin .h para incluir)
extern void MOSTRAR_MENSAJE2( char *s, unsigned long pPausa, int pCore );
int TEXTO_EN_IMPRESORA_FISCAL( void );
extern void IMPRIMIR_TEXTO_NO_FISCAL(int station,int tipo, char *texto, int salto_fila );
extern struct _tarjetas_fisc tarjetas_fisc_rec[10];
int GET_HANDLE_VOUCHER ();
int GET_COPIA_VOUCHER ();


/**************************************************************************/
int SLIP_HABILITADO()
/**************************************************************************/
{
    return ( _SLIP ) ? 1 : 0;
}
/**************************************************************************/
int INICIALIZAR_PRINTER()
/**************************************************************************/
{
    int ok = 1, error, no_setear_al_comienzo = 0;
    char nombre[51];
    FILE *p;
    int j;
    if( !_comandos_prn ) {
        _comandos_prn = (int *)MALLOC( sizeof( int[40][10] ) );
    }
    for( j = 0;j < 400;j++ ) {
        _comandos_prn[j] = -1;
    }
    if( SPOOLER ) {
        buffer_imp = malloc( LARGO_BUFFER );
    }
    if( IMPRESORA_INSTALADA ) {
        if( IMPRESORA_FISCAL == FISCAL_IBM_4614 ) {
            p = fopen( "fisc_ibm.dat", "rt" );
            if( !p ) {
                MENSAJE_STRING( S_ERROR_ABRIR_ARCHIVO_FISC_IBM );
                ok = 0;
            }
            else {
                fgets( nombre, 50, p );
                nombre[strlen( nombre ) - 1] = 0;
                fclose( p );
                Handle_Fiscal_IBM_4614 = _open( nombre, O_RDWR );
                if( Handle_Fiscal_IBM_4614 == -1 ) {
                    ok = 0;
                }
                else {
                    ok = 1;
                    // _lseek(Handle_Fiscal_IBM_4614, 0L, SEEK_END);
                }
                ok = ( Handle_Fiscal_IBM_4614 == -1 ) ? 0 : 1;
                no_setear_al_comienzo = 1;
                ok = CARGAR_ARCHIVO_DE_COMANDOS( IBM_4614_DEF, _comandos_prn, 10 );
            }
        }
        else if( IMPRESORA_FISCAL == FISCAL_DITEMPO ) {
            ok = CARGAR_ARCHIVO_DE_COMANDOS( FISCAL_DEF, _comandos_prn, 10 );
        }
        else if( IMPRESORA_FISCAL == FISCAL ) {
            ok = CARGAR_ARCHIVO_DE_COMANDOS( FISCAL_F_DEF, _comandos_prn, 10 );
        }
        else if( IMPRESORA_FISCAL == FISCAL_EPSON ) {
            ok = CARGAR_ARCHIVO_DE_COMANDOS( FISCALEP_DEF, _comandos_prn, 10 );
            #if defined(INVEL_W) || defined(INVEL_L)
            //ok = InitPrinterFiscal(  );
            #endif
        }
        else if( TIPO_IMPRESORA == IMPRESORA_OMRON ) {
            ok = CARGAR_ARCHIVO_DE_COMANDOS( PRN_ON_DEF, _comandos_prn, 10 );
        }
        else if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
            ok = CARGAR_ARCHIVO_DE_COMANDOS( PRINTER_DEF, _comandos_prn, 10 );
            DRIVER( _DRV_printer, _PRN_inicializacion, 0, &error, sizeof( error ) );
            ok = !error;
        }
        else if( TIPO_IMPRESORA == TM930 ) {
            ok = CARGAR_ARCHIVO_DE_COMANDOS( TM930_DEF, _comandos_prn, 10 );
        }
        else if( IMPRESORA_FISCAL == FISCAL_TMT88 ) {
            ok = CARGAR_ARCHIVO_DE_COMANDOS( FISCALE2_DEF, _comandos_prn, 10 );
            memset( cadacum, 0, 100 );
            ind_acum = 0;
        }
        else {
            PRINTER_OFF();
        }
        if( ok && !no_setear_al_comienzo && IMPRESORA_INSTALADA ) {
            SPOOLER = OFF;
            _INICIALIZAR_PRINTER();
            if( TIPO_IMPRESORA != IMPRESORA_DRIVER ) {
                if( IMPRESORA_FISCAL != FISCAL_TMT88 ) {
                    if( IMPRESORA_FISCAL ) {
                        ENVIAR_CODIGOS( _prn_inicializar );
                    }
                    else {
                        ENVIAR_CODIGOS( _prn_inicializar );
                        ENVIAR_CODIGOS( _prn_espaciado_normal );
                        ENVIAR_CODIGOS( _prn_select_receipt_y_journal );
                        ENVIAR_CODIGOS( _prn_habilita_deteccion_papel );
                        if( TIPO_IMPRESORA == TM930 ) {
                            ENVIAR_CODIGOS( _prn_letra_chica );
                        }
                    }
                }
            }
            if( PUERTO_IMPRESORA_OPCIONAL_FACTUR ) {
                PRN_HABILITAR_IMPRESORA_OPCIONAL();
                ENVIAR_CODIGOS( _prn_inicializar_prn_opcional );
                PRN_DESHABILITAR_IMPRESORA_OP();
            }
        }
    }
    return ( ok );
}
/**************************************************************************/
void CLOSE_PRINTER()
/**************************************************************************/
{
    if( IMPRESORA_FISCAL == FISCAL_IBM_4614 ) {
        #ifdef INVEL_L
        close( Handle_Fiscal_IBM_4614 );
        #else
		_close( Handle_Fiscal_IBM_4614 );
        #endif
    }
}
/**************************************************************************/
int STATUS_IBM_4614()
/**************************************************************************/
{
    int status = 0;
    #if !defined(INVEL_W) && !defined(INVEL_L)
    if( ( bioscom( 3, 0, PUERTO_FISCAL_IBM_4614 ) & 0X20 ) == 0 ) {
        status = 1;
    }
    #endif
    return status;
}
/**************************************************************************/
void _INICIALIZAR_PRINTER()
/**************************************************************************/
{
    if( TIPO_IMPRESORA != IMPRESORA_OMRON ) {
        if( TIPO_IMPRESORA != IMPRESORA_DRIVER ) {
            if( PUERTO_PARALELO ) {
                #if !defined(INVEL_W) && !defined(INVEL_L)
                biosprint( 1, 0, PUERTO_IMPRESORA );
            }
            else {
                if( BALANZA_BAUDIOS == 9600 ) {
                    bioscom( 0, 0xE3, PUERTO_IMPRESORA );
                } // 9600
                else {
                    bioscom( 0, 0xC3, PUERTO_IMPRESORA );
                } // 4800
                PAUSA( 18 );
                #endif
            }
        }
    }
    #if !defined(INVEL_W) && !defined(INVEL_L)
    if( PUERTO_IMPRESORA_OPCIONAL_FACTUR ) {
        biosprint( 1, 0, PUERTO_IMPRESORA_OPCIONAL_FACTUR - 1 );
    }
    #endif
}

/***********************************************************************/
void GRABAR_TICKET_PRN( char *cadena ) {
/***********************************************************************/
	//char bufferaso[2048];
	int ie;
 	//memset( bufferaso, 0 , 2048 );
	for(  ie = 0; /*ie < 2048,*/ ie < (int)strlen( cadena ); ie++) {
		if( cadena[ie] == 0x00 )
			cadena[ie] = 20;
	}
	//sprintf( bufferaso, "%s",cadena);
	//ie = strlen(bufferaso);
	//_WRITE( handle_ticket_copia , bufferaso, strlen( bufferaso ), __LINE__, __FILE__ );
	cio_Write(cadena,strlen(cadena),"TICKET.prn");
}
/***********************************************************************/
void GRABAR_VOUCHER_PRN( char *cadena ) {
/***********************************************************************/
	//char bufferaso[2048];
	int ie;
 	//memset( bufferaso, 0 , 2048 );
	for(  ie = 0; /*ie < 2048,*/ ie < (int)strlen( cadena ); ie++) {
		if( cadena[ie] == 0x00 )
			cadena[ie] = 20;
	}
	//sprintf( bufferaso, "%s",cadena);
	//ie = strlen(bufferaso);
	//_WRITE( handle_ticket_copia , bufferaso, strlen( bufferaso ), __LINE__, __FILE__ );
	cio_Write(cadena,strlen(cadena),"voucher.prn");
}

/***********************************************************************/
void GRABAR_Z_PRN( char *cadena ) {
/***********************************************************************/
	int ie;
	for(  ie = 0; ie < (int)strlen( cadena ); ie++) {
		if( cadena[ie] == 0x00 )
			cadena[ie] = 20;
	}
	cio_Write(cadena,strlen(cadena),"CIERRE_Z.prn");
}

/**************************************************************************/
int PRINT( char columna, char modo, char *cadena )
/**************************************************************************/
{
    int station= 0, letra = 0, recupera_tipo_letra = 0, i = 0, salto_fila = 0;
    char _tipo_letra[5];

	CARACTERES_ESPECIALES( cadena, _IMPRIMIR_CADENA);    
    if( MODO_NEGOCIO != SERVICIOS ) {
        unsigned rta = 0;
        int       /*h,*/ error = 1;

        #if defined(INVEL_W) || defined(INVEL_L)
        char buff[256];
		if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
	        _itoa( columna, buff, 10 );
			getData( ACTION_GET_PRINT_STATION, ( char* )&station, 0 );

	        if( CINTA_TESTIGO_ELECTRONICA && ( station == TO_POS_PRINTER_JOURNAL ||
				station == TO_POS_PRINTER_JOURNAL_RECEIPT || station == TO_POS_PRINTER_JOURNAL_PART ) ) {
                if( COLUMNA < columna ) {
                    for( ;COLUMNA < columna;COLUMNA += ANCHO_CHAR ) {
                        GRABAR_JOURNAL_ELECTRONICO2( " " );
                    }
                }
		        GRABAR_JOURNAL_ELECTRONICO2( cadena );
                COLUMNA += strlen(cadena);
	        }
			
            if( _SI_REPETIR_CADENA 
                    && ( station == TO_POS_PRINTER_RECEIPT 
                        || station == TO_POS_PRINTER_JOURNAL_RECEIPT ) ) {
                
                getData( ACTION_GET_PRINT_LETTER, ( char* )&letra, 0 );

                switch( letra ) {
                    case LETRA_CHICA:
                        _snprintf( _tipo_letra, 5, "%d", LETRA_CHICA );
                        _tipo_letra[4] = '\0';
                        break;
                    case LETRA_GRANDE:
                        _snprintf( _tipo_letra, 5, "%d", LETRA_GRANDE );
                        _tipo_letra[4] = '\0';
                        break;
                    case LETRA_NEGRITA:
                        _snprintf( _tipo_letra, 5, "%d", LETRA_NEGRITA );
                        _tipo_letra[4] = '\0';
                        break;
                    case DESHABILITA_FORMATO:
                        _snprintf( _tipo_letra, 5, "%d", DESHABILITA_FORMATO );
                        _tipo_letra[4] = '\0';
                        break;
                }
                
                if( RESOLVER( _SI_REPETIR_CADENA, NULL ) ) {
                    if( LINEA_IMPRESA && _SI_CANT_IMPRESION && LINEA_IMPRESA >= _SI_CANT_IMPRESION ) {
	                    setData( ACTION_SET_DATA_PRINTER, _SI_CAD_IMPRESION, strlen( _SI_CAD_IMPRESION ) );
                        LINEA_IMPRESA = 0;
                        setData( ACTION_PRINT_MODE, _tipo_letra, 0 );
                        recupera_tipo_letra = 1;
                        LF( 1 );
                    } 
                }
            }
			else
			{
				setData( ACTION_SET_PRINT_POS, buff, strlen( buff ) );
				setData( ACTION_SET_DATA_PRINTER, cadena, strlen( cadena ) );
				//while( PAPER_OUT() ) {};
			}
        }
		if( TEXTO_EN_IMPRESORA_FISCAL( ) )
		{
			char auxiliar[100];
			//copia del voucher
			
			memset( auxiliar, 0, sizeof(auxiliar) );
			if( cadena != NULL )
			TRIM(cadena);
			//indica que se hizo un cambio de linea
			if( strcmp( cadena, "" ) == 0 ) {
				salto_fila = 1;
			}
			//indica que se hizo un cambio de linea , debe imprimir el buffer  ?¿
			if( cadena[0]== 0x0a || salto_fila /*IMPRESORA_FISCAL == FISCAL*/ ) { // PARA LA TM220 EL ||
				
				if( strlen( BUFFER_LINEA_PRN ) > 0 /*|| IMPRESORA_FISCAL == FISCAL*/){
					//GRABAR_LOG_BTRIEVE(BUFFER_LINEA_PRN);
					if( TIPO_DE_LETRA < LETRA_NORMAL || TIPO_DE_LETRA > LETRA_SUBRAYADO_NEGRITA )
						TIPO_DE_LETRA = LETRA_NORMAL ;
					
					if(strlen( BUFFER_LINEA_PRN ) > 0)
						IMPRIMIR_TEXTO_NO_FISCAL( 1 , TIPO_DE_LETRA , BUFFER_LINEA_PRN, SI );
					else
						IMPRIMIR_TEXTO_NO_FISCAL( 1 , TIPO_DE_LETRA , cadena, SI );
				}

				memset( BUFFER_LINEA_PRN ,0 ,sizeof( BUFFER_LINEA_PRN ) );
			} else {
				int largo = strlen( BUFFER_LINEA_PRN );

				if( columna < 0 || columna > 199 )
					columna = 0;
				
				if( columna > largo && columna > 1 ) {
					memset(auxiliar,' ',(columna - largo - 1) );
				} 
				strncat( &auxiliar, cadena, sizeof( auxiliar ) - strlen( auxiliar ) - 1 );
				strncat( &BUFFER_LINEA_PRN[largo], auxiliar, sizeof( BUFFER_LINEA_PRN ) - largo - 1 );
			}
		}
        #else
        switch( modo ) {
            case 101:
                PRN_LETRA_NORMAL();
                break;
            case 102:
                PRN_LETRA_GRANDE();
                break;
        }
        // if( !SPOOLER && TIPO_IMPRESORA != IMPRESORA_DRIVER ) {
        if( !SPOOLER ) {
            for( h = 0;error;h++ ) {
                if( IS_PRINTER() ) {
                    error = 0;
                }
                else {
                    if( TIPO_IMPRESORA != TM930 ) {
                        MENSAJE_STRING( S_IMPRESORA_OFF_LINE );
                        PAUSA( 2 );
                    }
                }
                if( h > 30 ) {
                    MENSAJE_STRING( S_ERROR_IMPRESION_ENTER );
                    while( GETCH() != 13 );
                    BORRAR_MENSAJE();
                    h = 0;
                }
            }
        }
        _FILTRAR_CARACTERES_ESPECIALES = 1;
        /*----------- Posiciona el cabezal en la columna apropiada ---------*/
        if( COLUMNA < columna ) {
            for( ;COLUMNA < columna;COLUMNA += ANCHO_CHAR ) {
                SEND_BYTE( ' ' );
            }
        }
        if( ( IMPRESORA_FISCAL == FISCAL_IBM_4610 && MODO_DEBUG == 15 ) || MODO_DEBUG == 50 ) {
            GRABAR_LOG_SISTEMA( cadena );
        }
        /*------------ Envia caracteres ------------*/
        for( i = 0;cadena[i];i++ ) {
            SEND_BYTE( cadena[i] );
        }
        COLUMNA += i * ANCHO_CHAR;
        _FILTRAR_CARACTERES_ESPECIALES = 0;
        #endif
        return ( rta );
    }
    else {
        //modo_negocio
        int len = strlen( cadena );
        if( len > 0 ) {
            setData( ACTION_SET_DATA_PRINTER, cadena, len );
        }
        return ( 0 );
    }
}
/**************************************************************************/
void CR()
/**************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    //setData(ACTION_SET_DATA_RECEIPT, "\r", 1);
    #else
    COLUMNA = 1;
    if( !COMANDO_PRN_DRIVER( _PRN_cr, 0 ) ) {
        SEND_BYTE( 0x0d );
    }
    #endif
}
/**************************************************************************/
void LF( int cantidad )
/**************************************************************************/
{
    char buffer[20];
    int i = 0;
    memset( buffer, 0, 20 );
    COLUMNA = 1;
    if( CINTA_TESTIGO_ELECTRONICA ) {
        for(i=0;i<cantidad;i++){    
            GRABAR_JOURNAL_ELECTRONICO2( "\n" );
        }
    }
	if( IMPRESORA_FISCAL == FISCAL )
	{
		for( i=0; i < cantidad && RAM_IMPRESION_FISCAL !=0 ; i++ )
			IMPRIMIR_TEXTO_NO_FISCAL( 1, TIPO_DE_LETRA,"", SI );
	} else {
		sprintf( buffer, "%d", cantidad );
		setData( ACTION_PRINT_LF, buffer, strlen( buffer ) );
	}
#ifdef  DEBUG_IMPRESION	
	LF_AUX(cantidad);
#endif
}

/**************************************************************************/
void SET_COLUMNA( int col )
/**************************************************************************/
{
    COLUMNA = col;
}
/**************************************************************************/
int GET_COLUMNA()
/**************************************************************************/
{
    return ( ( int )COLUMNA );
}
/**************************************************************************/
void LF_RECEIPT( int cantidad )
/**************************************************************************/
{
    int j;
    j = _JORNAL;
    PRN_INHABILITA_AVANCE_JOURNAL();
    LF( cantidad );
    if( j ) {
        PRN_HABILITA_AVANCES();
    }
}
/**************************************************************************/
void LF_JOURNAL( int cantidad )
/**************************************************************************/
{
    PRN_INHABILITA_AVANCE_RECEIPT();
    LF( cantidad );
    PRN_HABILITA_AVANCES();
}
/**************************************************************************/
void MICRO_LF( int cantidad )
/**************************************************************************/
{
    if( !COMANDO_PRN_DRIVER( _PRN_micro_lf, cantidad ) ) {
        __SEND_BYTE_CODIGO( 0x1b );
        __SEND_BYTE_CODIGO( 0x4a );
        __SEND_BYTE_CODIGO( ( char )cantidad );
        COLUMNA = 1;
        ANCHO_CHAR = 1;
    }
}
/**************************************************************************/
void PRN_LETRA_NORMAL()
/**************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    char buff[5];
    sprintf( buff, "%d", 0 );
    //mmancuso agregado
    TIPO_DE_LETRA = LETRA_NORMAL;
    //
    setData( ACTION_PRINT_MODE, buff, 2 );
    #else
    //   if(IMPRESORA_FISCAL != FISCAL_IBM_4694){
    if( !COMANDO_PRN_DRIVER( _PRN_letra_normal, 0 ) ) {
        if( TIPO_IMPRESORA == 1 ) {
            ENVIAR_CODIGOS( _prn_letra_chica ); //
        }
        else {
            ENVIAR_CODIGOS( _prn_letra_normal );
        }
    }
    //   }
    ANCHO_CHAR = 1;
    #endif
    if( MODO_NEGOCIO == SERVICIOS ) {
        setData( ACTION_NORMAL_LETTER, NULL, 0 );
    }
}
/**************************************************************************/
void PRN_LETRA_GRANDE()
/**************************************************************************/
{
    //   if(IMPRESORA_FISCAL != FISCAL_IBM_4694){
    #if defined(INVEL_W) || defined(INVEL_L)
    char buff[5];
    sprintf( buff, "%d", 1 );
	TIPO_DE_LETRA = LETRA_DOBLE_ANCHO;
    setData( ACTION_PRINT_MODE, buff, 2 );
    #else
    if( !COMANDO_PRN_DRIVER( _PRN_letra_grande, 0 ) ) {
        ENVIAR_CODIGOS( _prn_letra_grande );
    }
    ANCHO_CHAR = 2;
    //   }
    #endif
    if( MODO_NEGOCIO == SERVICIOS ) {
        setData( ACTION_BIG_LETTER, NULL, 0 );
    }
}
/**************************************************************************/
void PRINT_BITMAP( char *buff )
/**************************************************************************/
{	
	//char buff[5];
	if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
		setData( ACTION_PRINT_BITMAP, buff, 2 );
	}
}
/**************************************************************************/
void BAR_CODE(  )
/**************************************************************************/
{	
	char buff[5];
	if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
		setData( ACTION_PRINT_BAR_CODE, buff, 2 );
	}
}
/**************************************************************************/
void EXISTE_PRINTER(  )
/**************************************************************************/
{	
	//char buff[5];
	if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
		setData( ACTION_PRINT_EXISTE_POS_PRINTER, "YES", 2 );
	} else {
		setData( ACTION_PRINT_EXISTE_POS_PRINTER, "NO", 2 );
	}
}
/**************************************************************************/
int SET_BITMAP( char *buff )
/**************************************************************************/
{	
	int rta;
	rta = setData( ACTION_SET_PRINT_BITMAP, buff, 2 );
	return( rta );
}
/**************************************************************************/
void SET_BAR_CODE( char *buff )
/**************************************************************************/
{	
	setData( ACTION_SET_PRINT_BAR_CODE, buff, 2 );
}
/**************************************************************************/
void PRN_LETRA_CHICA()
/**************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    char buff[5];
    sprintf( buff, "%d", 0 );
	TIPO_DE_LETRA = LETRA_NORMAL; 
    if( MODO_NEGOCIO == SERVICIOS ) {
        setData( ACTION_NORMAL_LETTER, NULL, 2 );
    }
    else {
        setData( ACTION_PRINT_MODE, buff, 2 );
    }
    #else
    if( !COMANDO_PRN_DRIVER( _PRN_letra_chica, 0 ) ) {
        ENVIAR_CODIGOS( _prn_letra_chica );
    }
    #endif
}
/**************************************************************************/
void PRN_LETRA_NEGRITA(  )
/**************************************************************************/
{
    char buff[5];

    sprintf( buff, "%d", 2 );
    setData( ACTION_PRINT_MODE, buff, 0 );    

}

/**************************************************************************/
void PRN_DESHABILITA_FORMATOS_LETRAS(  )
/**************************************************************************/
{
    char buff[5];
    
    sprintf( buff, "%d", 10 );
    setData( ACTION_PRINT_MODE, buff, 0 );    

}

/**************************************************************************/
void PRN_LETRA_DOBLE_PASADA()
/**************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        #if defined(INVEL_W) || defined(INVEL_L)
        char buff[5];
        sprintf( buff, "%d", 2 );
        setData( ACTION_PRINT_MODE, buff, 2 );
        #else
        if( !COMANDO_PRN_DRIVER( _PRN_2pasadas, 0 ) ) {
            ENVIAR_CODIGOS( _prn_2pasadas );
        }
        #endif
    }
}
/**************************************************************************/
void PRN_CONTINUA()
/**************************************************************************/
{
    if( !IMPRESORA_FISCAL ) {
        if( !COMANDO_PRN_DRIVER( _PRN_continua, 0 ) ) {
            ENVIAR_CODIGOS( _prn_continua );
        }
    }
    _ESTACION_ACTIVA = RECEIPT;
}
/**************************************************************************/
void PRN_DISCONTINUA()
/**************************************************************************/
{
    if( !IMPRESORA_FISCAL ) {
        if( !COMANDO_PRN_DRIVER( _PRN_discontinua, 0 ) ) {
            ENVIAR_CODIGOS( _prn_discontinua );
            if( IMPRIMIR_EN_JOURNAL ) {
                if( !COMANDO_PRN_DRIVER( _PRN_discontinua, 0 ) ) {
                    ENVIAR_CODIGOS( _prn_discontinua );
                }
            }
        }
    }
    _ESTACION_ACTIVA = RECEIPT | JOURNAL;
}
/**************************************************************************/
void PRN_HABILITA_AVANCES()
/**************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        if( !_SLIP ) {
            PRN_DISCONTINUA();
            if( !IMPRESORA_FISCAL && IMPRIMIR_EN_JOURNAL ) {
                #if defined(INVEL_W) || defined(INVEL_L)
                PRN_SELECT( JOURNAL_RECEIPT );
                #else
                if( !COMANDO_PRN_DRIVER( _PRN_select_receipt_y_journal, 0 ) ) {
                    // ENVIAR_CODIGOS( &_comandos_prn[_prn_select_receipt_y_journal]
                    // );
                    ENVIAR_CODIGOS( _prn_select_receipt_y_journal );
                }
                #endif
            }
            _JORNAL = 1;
        }
    }
}
/**************************************************************************/
void PRN_INHABILITA_AVANCE_JOURNAL()
/**************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        if( !_SLIP ) {
            PRN_CONTINUA();
            #if defined(INVEL_W) || defined(INVEL_L)
            PRN_SELECT( RECEIPT );
            #else
            if( !COMANDO_PRN_DRIVER( _PRN_select_receipt, 0 ) ) {
                ENVIAR_CODIGOS( _prn_select_receipt );
            }
            #endif
            _JORNAL = 0;
            _ESTACION_ACTIVA = RECEIPT;
        }
    }
}
/**************************************************************************/
void PRN_INHABILITA_AVANCE_RECEIPT()
/**************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        if( !_SLIP ) {
            if( IMPRIMIR_EN_JOURNAL ) {
                PRN_CONTINUA();
                #if defined(INVEL_W) || defined(INVEL_L)
                PRN_SELECT( JOURNAL );
                #else
                if( !COMANDO_PRN_DRIVER( _PRN_select_journal, 0 ) ) {
                    ENVIAR_CODIGOS( _prn_select_journal );
                }
                #endif
            }
            _ESTACION_ACTIVA = JOURNAL;
        }
    }
    else {
        PRN_SELECT( JOURNAL );
    }
}
/**************************************************************************/
void PRN_CORTE_TOTAL()
/**************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        setData( ACTION_PRINT, NULL, 0 );
        setData( ACTION_PRINT_CUT_PAPER, NULL, 0 );
        setData( ACTION_PRINT, NULL, 0 );
        _corto_hoja = SI;
        _clock_corte_hoja = GET_TIME();
    }
}
/**************************************************************************/
void PRN_FIN_TICKET(  )
/**************************************************************************/
{
    int lineas;
	if( !IMPRIMIR_CABECERA_AL_FINAL ) {
        if( IMPRESORA_FISCAL != FISCAL )
			LF( 1 );
        if( IMPRESORA_FISCAL == FISCAL ) {
			CERRAR_DOCUMENTO_NO_FISCAL();
        }
        else {
            PRN_INHABILITA_AVANCE_JOURNAL();
            #if !defined(INVEL_W) && !defined(INVEL_L)
            LF( 5 );
            #endif
        }
		if( IMPRESORA_FISCAL != FISCAL ) {
			if( TKT_CONFIGURABLE ) {
				if( MODO_NEGOCIO == SERVICIOS ) {
					char pagina[31];
					//Imprime numero de hoja
					sprintf( pagina, "página %d de %d", cantCopias + 1, cantTotalCopias + 1 );         
					setData( ACTION_SET_POSITION, "150#235", 0 );
					IMPRIMIR_CADENA( 0, pagina );
				}
				TKT_CORTE_PAPEL();
			}
			else {
				PRN_CORTE_TOTAL();
			}
			if( IMPRESORA_FISCAL != FISCAL_IBM_4610 ) {
				if( !COMANDO_PRN_DRIVER( _PRN_inicializar_ticket, 0 ) ) {
					ENVIAR_CODIGOS( _prn_inicializar_ticket );
				}
			}
		}
	}
	else {
        lineas = 2;
        LF( lineas );
        if( !IMPRESORA_FISCAL ) {
            PRN_INHABILITA_AVANCE_JOURNAL();
        }
        _IMPRIME_CABECERA_TICKET( LINEAS_CORTE_RECEIPT, lineas, SI );
    }
    if( IMPRESORA_FISCAL == FISCAL_TMT88 ) {
        //corte de papel
        TKT_CORTE_PAPEL();
    }

}

/**************************************************************************/
void PRN_FIN_TICKET_CONTINUO( int pagina, int tot_paginas )
/**************************************************************************/
{
    int lineas;
    if( !IMPRIMIR_CABECERA_AL_FINAL ) {
        //#if !defined(INVEL_W) && !defined(INVEL_L)
        LF( 1 );
        //#endif
        if( IMPRESORA_FISCAL && IMPRESORA_FISCAL != FISCAL_POS745x ) {
            if( IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
                LF( 4 );
            }
            else
            #if !defined(INVEL_W) && !defined(INVEL_L)
            {
                LF( 9 );
            }
            #else
            LF( 0 );
            #endif
        }
        else {
            PRN_INHABILITA_AVANCE_JOURNAL();
            #if !defined(INVEL_W) && !defined(INVEL_L)
            LF( 5 );
            #endif
        }
        if( TKT_CONFIGURABLE ) {
            if( MODO_NEGOCIO == SERVICIOS ) {
                char cadena[31];
                //Imprime numero de hoja
                sprintf( cadena, "página %d de %d", pagina + 1, tot_paginas + 1 );         
                setData( ACTION_SET_POSITION, "150#235", 0 );
                IMPRIMIR_CADENA( 0, cadena );
            }
            TKT_CORTE_PAPEL();
        }
        else {
            PRN_CORTE_TOTAL();
        }
        if( IMPRESORA_FISCAL != FISCAL_IBM_4610 ) {
            if( !COMANDO_PRN_DRIVER( _PRN_inicializar_ticket, 0 ) ) {
                ENVIAR_CODIGOS( _prn_inicializar_ticket );
            }
        }
    }
    else {
        lineas = 2;
        LF( lineas );
        if( !IMPRESORA_FISCAL ) {
            PRN_INHABILITA_AVANCE_JOURNAL();
        }
        _IMPRIME_CABECERA_TICKET( LINEAS_CORTE_RECEIPT, lineas, SI );
    }
    if( IMPRESORA_FISCAL == FISCAL_TMT88 ) {
        //corte de papel
        TKT_CORTE_PAPEL();
    }
}
/**************************************************************************/
int VERIFICA_CORTE_FIN_TICKET( int lineas, int lineas_corte )
/**************************************************************************/
{
    int rta = 0;
    if( MODO_NEGOCIO != SERVICIOS ) {
        if( lineas >= lineas_corte ) {
            rta = 1;
            if( corte_habilitado ) {
                if( TKT_CONFIGURABLE ) {
                    TKT_CORTE_PAPEL();
                }
                else {
                    PRN_CORTE_TOTAL();
                }
            }
            if( IMPRESORA_FISCAL != FISCAL_IBM_4610 ) {
                if( !COMANDO_PRN_DRIVER( _PRN_inicializar_ticket, 0 ) ) {
                    ENVIAR_CODIGOS( _prn_inicializar_ticket );
                }
            }
        }
    }
    return ( rta );
}
/**************************************************************************/
void ENVIAR_CODIGOS( int op )
/**************************************************************************/
{
    #if !defined(INVEL_W) && !defined(INVEL_L)
    if( MODO_NEGOCIO != SERVICIOS ) {
        int i;
        for( i = 0;_comandos_prn[( op * 10 ) + i] != -1;i++ ) {
            __SEND_BYTE_CODIGO( ( unsigned char ) _comandos_prn[( op * 10 ) + i] );
        }
    }
    #endif
}
/**************************************************************************/
void PRINTER_ON()
/**************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        IMPRESORA_INSTALADA = 1;
    }
}
/**************************************************************************/
void PRINTER_OFF()
/**************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        IMPRESORA_INSTALADA = 0;
    }
}
/**************************************************************************/
void SPOOLER_OFF()
/**************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        SPOOLER = 0;
    }
}
/**************************************************************************/
void SPOOLER_ON()
/**************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        SPOOLER = 1;
    }
}
/**************************************************************************/
int SEND_BYTE( unsigned char byte )
/**************************************************************************/
{
    int extraer = 0, error = 0, seguir = 1;
    if( MODO_NEGOCIO != SERVICIOS ) {
        if( IMPRESORA_INSTALADA ) {
            if( IMPRESORA_FISCAL == FISCAL_TMT88 ) {
                if( byte == 0x0a || byte == 0x0d ) {
                    char cadfinal[10];
                    int i;
                    memset( cadfinal, 0, 10 );
                    {
                        char cadenaacum[100];
                        sprintf( cadenaacum, "Cadacum: %s", cadacum );
                        GRABAR_LOG_SISTEMA( cadenaacum ,LOG_DEBUG,4);
                    }
                    CALCULAR_CHECK_SUM( cadacum, cadfinal );
                    memset( cadacum, 0, 100 );
                    ind_acum = 0;
                    for( i = 0;i < 4;i++ ) {
                        __SEND_BYTE( cadfinal[i], SI );
                    }
                    memset( cadacum, 0, 100 );
                    ind_acum = 0;
                    seguir = 0;
                }
                else {
                    cadacum[ind_acum] = byte;
                    ind_acum++;
                }
            }
            if( seguir ) {
                if( SPOOLER ) {
                    /*---------- Incrementa puntero de cola ----------*/
                    INCREMENTA_CABEZA;
                    /*--- Si no hay lugar en el buffer extraer un byte para hacerlo ---*/
                    if( p_buffer_2 == p_buffer_1 ) {
                        extraer = 1;
                    }
                    /*----------- Coloca en el buffer ------------*/
                    DECREMENTA_CABEZA;
                    if( extraer ) {
                        error = _SEND_BYTE();
                    }
                    buffer_imp[p_buffer_2] = byte;
                    INCREMENTA_CABEZA;
                }
                else {
                    error = __SEND_BYTE( byte, SI );
                }
            }
        }
        return ( error );
    }
    else {
        return ( 0 );
    }
}
/**************************************************************************/
int _SEND_BYTE()
/**************************************************************************/
{
    int error = 0;
    char byte;
    if( MODO_NEGOCIO != SERVICIOS ) {
        if( SPOOLER ) {
            if( p_buffer_1 != p_buffer_2 ) {
                /*------------- Obtiene caracter de la cola -----------*/
                byte = buffer_imp[p_buffer_1];
                INCREMENTA_COLA;
                /*----------- Enva el caracter ------------*/
                error = __SEND_BYTE( byte, SI );
            }
        }
    }
    return ( error );
}
/**************************************************************************/
void PRN_HABILITA_SLIP( int orientacion, int enviar_blanco )
/**************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    PRN_SELECT( SLIP );
    MOSTRAR_CADENA( 0, PRN, "!|h" );
    #else
    #endif
    _SLIP = 1;
}

/**************************************************************************/
void PRN_INHABILITA_SLIP()
/**************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        #if defined( INVEL_W ) || defined ( INVEL_L )
        PRN_SELECT( JOURNAL_RECEIPT );
        #else
        if( IMPRESORA_FISCAL == FISCAL_EPSON ) {
            ENVIAR_COMANDO_FISCAL( "\02" );
        }
        else {
            if( !COMANDO_PRN_DRIVER( _PRN_inhabilita_slip, 0 ) ) {
                //  ENVIAR_CODIGOS( &_comandos_prn[_prn_inhabilita_slip] );
                ENVIAR_CODIGOS( _prn_inhabilita_slip );
            }
        }
        #endif
        _SLIP = 0;
    }
}
/**************************************************************************/
void PRN_ESPACIADO_NORMAL()
/**************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        if( !COMANDO_PRN_DRIVER( _PRN_espaciado_normal, 0 ) ) {
            // ENVIAR_CODIGOS( &_comandos_prn[_prn_espaciado_normal] );
            ENVIAR_CODIGOS( _prn_espaciado_normal );
        }
    }
}
/**************************************************************************/
void PRN_ESPACIADO( int x )
/**************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        if( !COMANDO_PRN_DRIVER( _PRN_set_espaciado, x ) ) {
            // ENVIAR_CODIGOS( &_comandos_prn[_prn_set_espaciado] );
            ENVIAR_CODIGOS( _prn_set_espaciado );
            __SEND_BYTE_CODIGO( ( char )x );
        }
    }
}
/**************************************************************************/
void PRN_FEED( int x )
/**************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        if( !COMANDO_PRN_DRIVER( _PRN_feed, x ) ) {
            // ENVIAR_CODIGOS( &_comandos_prn[_prn_feed] );
            ENVIAR_CODIGOS( _prn_feed );
            __SEND_BYTE_CODIGO( ( char )x );
        }
    }
}
/**************************************************************************/
void PRN_SELECT( int x )
/**************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
	ESTACION_SELECCIONADA = x;
    if( MODO_NEGOCIO != SERVICIOS ) {
        if( IMPRESORA_FISCAL ) {
            char buff[20];
            setData( ACTION_SET_PRINT_STATION, (_itoa( x + 4, buff, 10 )), 0 );
        }
        else {
            char buff[20];
            LF( 0 );
            setData( ACTION_PRINT, NULL, 0 );
            setData( ACTION_SET_PRINT_STATION, _itoa( x, buff, 10 ), 0 );
        }
    }
    else {
        char buff[20];
        setData( ACTION_SET_PRINT_STATION, _itoa( x, buff, 10 ), 0 );
    }
    #else
    if( IMPRESORA_FISCAL == FISCAL_IBM_4694 || IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
        switch( x ) {
            case JOURNAL:
                PRN_INHABILITA_AVANCE_RECEIPT();
                break;
            case RECEIPT:
                PRN_INHABILITA_AVANCE_JOURNAL();
                break;
        }
    }
    else {
        if( IMPRESORA_FISCAL != FISCAL_EPSON && IMPRESORA_FISCAL != FISCAL_IBM_4614
         && IMPRESORA_FISCAL != FISCAL_IBM_4610 && IMPRESORA_FISCAL != FISCAL_TMT88 ) {
            if( IMPRIMIR_EN_JOURNAL || x != JOURNAL ) {
                if( !COMANDO_PRN_DRIVER( _PRN_select, x ) ) {
                    ENVIAR_CODIGOS( _prn_select );
                    __SEND_BYTE_CODIGO( ( char )x );
                }
            }
            _ESTACION_ACTIVA = x;
            if( IMPRESORA_FISCAL ) {
                LF( 1 );
            }
        }
    }
    #endif
}
/**************************************************************************/
void PRN_SELECT_PARA_ESPACIADO( int x )
/**************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        // if( IMPRESORA_FISCAL != FISCAL_EPSON ) {
        if( IMPRESORA_FISCAL != FISCAL_EPSON && IMPRESORA_FISCAL != FISCAL_TMT88 ) {
            if( !COMANDO_PRN_DRIVER( _PRN_select_para_espaciado, x ) ) {
                // ENVIAR_CODIGOS( &_comandos_prn[_prn_select_para_espaciado] );
                ENVIAR_CODIGOS( _prn_select_para_espaciado );
                __SEND_BYTE_CODIGO( ( char )x );
            }
            if( IMPRESORA_FISCAL ) {
                LF( 1 );
            }
        }
    }
}
/**************************************************************************/
void PRN_HABILITAR_IMPRESORA_OPCIONAL()
/**************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        /*------------ Salvamos la configuracion anterior ------------*/
        __TIPO_IMPRESORA = TIPO_IMPRESORA;
        __PUERTO_PARALELO = PUERTO_PARALELO;
        __PUERTO_IMPRESORA = PUERTO_IMPRESORA;
        /*---------- Configuracin para impresora opcional -----------*/
        SET_MEMORY_CHAR( __tipo_impresora, 1 );
        SET_MEMORY_CHAR( __puerto_paralelo, SI );
        SET_MEMORY_CHAR( __puerto_impresora, PUERTO_IMPRESORA_OPCIONAL_FACTUR - 1 );
        /*-------------- Esperamos a que est ON-LINE ----------------*/
        while( !IS_PRINTER() );
    }
}
/**************************************************************************/
void PRN_DESHABILITAR_IMPRESORA_OP()
/**************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        /*------------ Restauramos la configuracion anterior ------------*/
        SET_MEMORY_CHAR( __tipo_impresora, __TIPO_IMPRESORA );
        SET_MEMORY_CHAR( __puerto_paralelo, __PUERTO_PARALELO );
        SET_MEMORY_CHAR( __puerto_impresora, __PUERTO_IMPRESORA );
    }
}
/**************************************************************************/
int IS_PRINTER()
/**************************************************************************/
{
    int ok = 1;
    if( MODO_NEGOCIO != SERVICIOS ) {
        #if defined(INVEL_W) || defined(INVEL_L)
        #else
        if( VERIFICAR_IMPRESORA_ON_LINE && IMPRESORA_INSTALADA ) {
            if( IMPRESORA_FISCAL == FISCAL_IBM_4614 ) {
                ok = STATUS_IBM_4614();
            }
            else if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
                DRIVER( _DRV_printer, _PRN_status, 0, &ok, sizeof( ok ) );
                if( ok & _PRN_online ) {
                    ok = 1;
                }
                else {
                    ok = 0;
                }
            }
            else if( TIPO_IMPRESORA == IMPRESORA_OMRON ) {
                #ifdef COMPILAR_OMRON
                ok = OMRON_IS_PRINTER();
                #endif
            }
            else if( PUERTO_PARALELO ) {
                ok = biosprint( 2, 0, PUERTO_IMPRESORA );
                if( ( ok & 0x90 ) == 0x90 ) {
                    ok = 1;
                }
                else {
                    ok = 0;
                }
            }
            else if( TIPO_IMPRESORA == TM930 ) {
                ok = TM930_IS_PRINTER();
            }
        }
        #endif
    }
    return ( ok );
}

/************************************************************************************************/
void CONTROLAR_ESTADO_IMPRESORA(  ) { 
/************************************************************************************************/
	char mensaje[100];
    static char *retire = '\0', *coloque = '\0', *no_se_pudo = '\0', *inv = '\0', *cambie = '\0';
    int entro = 0;
	//memset( mensaje, 0, sizeof(mensaje) );

    entro = REPETIR_MIENTRAS_IGUALES( ST_MEM( S_POR_FAVOR_RETIRE_COMPROB_DEL_SLIP, &retire ), 
                                      &printer_status_2, 112 );

    entro += REPETIR_MIENTRAS_IGUALES( ST_MEM ( S_COLOQUE_PAPEL_EN_SLIP, &coloque ), 
                                      &printer_status_2, 204 );

    entro += REPETIR_MIENTRAS_IGUALES( ST_MEM( S_NO_SE_PUDO_COMPLETAR_OPERACION_DE_IMPRESION, &no_se_pudo ), 
                                      &printer_status_2, 111 );

    entro += REPETIR_MIENTRAS_IGUALES( ST_MEM( S_OPERACION_DE_IMPRESION_INVALIDA, &inv ), 
                                      &printer_status_2, 106 );

    entro += REPETIR_MIENTRAS_IGUALES( "CIERRE LA TAPA DE LA IMPRESORA", 
                                      &printer_status_2, 201 );

    entro += REPETIR_MIENTRAS_IGUALES( ST_MEM( S_CAMBIE_ROLLO_PAPEL, &cambie ), 
                                      &printer_status_2, 203 );
    
    entro += REPETIR_MIENTRAS_IGUALES( ST_MEM( S_CAMBIE_ROLLO_PAPEL, &cambie ), 
                                      &printer_status_2, 202 );
    
    entro += REPETIR_MIENTRAS_IGUALES( "VERIFIQUE LA IMPRESORA", 
                                      &printer_status_2, 108 );

    entro += REPETIR_MIENTRAS_IGUALES( "IMPRESORA FUERA DE LINEA. VERIFIQUE LA IMPRESORA", 
                                      &printer_status_2, 107 );

	if( entro == 0 && getData( ACTION_DETECT_ERROR_PRINTER, NULL, 0 ) == 1 ) {
        int iteraciones = 50, borrar = 0;
        _snprintf( mensaje, sizeof( mensaje ), "ERROR 1 EN LA IMPRESORA [%d]", printer_status_2 );
		//MOSTRAR_MENSAJE2( mensaje, 2000, NO );
		GRABAR_LOG_SISTEMA( mensaje ,LOG_JAVA_SERVER,2 );
		while( getData( ACTION_DETECT_ERROR_PRINTER, NULL, 0 ) == 1  ) {  
            if( iteraciones == 0 ) {
                MOSTRAR_MENSAJE2( mensaje, 2000, NO );
                borrar = 1;
                iteraciones = 50;
            }
            iteraciones--;
			SLEEP( 100 );
		}
        if( borrar == 1 ) {
		    BORRAR_MENSAJE_( NO );
        }
	}
	if( entro == 0 && printer_status_2 && printer_status_2 != 201 && printer_status_2 != 202 
            && printer_status_2 != 203 && printer_status_2 != 204 && printer_status_2 != 112 
            && printer_status_2 != 108 && printer_status_2 != 107 ) {
        int iteraciones = 50, borrar = 0;
		_snprintf( mensaje, sizeof( mensaje ), "ERROR 2 EN LA IMPRESORA [%d]", printer_status_2 );
		//MOSTRAR_MENSAJE2( mensaje, 2000, NO );
		GRABAR_LOG_SISTEMA( mensaje ,LOG_JAVA_SERVER,2 );
		while( printer_status_2 && printer_status_2 != 201 && printer_status_2 != 202 && printer_status_2 != 203
                && printer_status_2 != 204 && printer_status_2 != 112 && printer_status_2 != 108 && printer_status_2 != 107) {  
            if( iteraciones == 0 ) {
                MOSTRAR_MENSAJE2( mensaje, 2000, NO );
                borrar = 1;
                iteraciones = 50;
            }
            iteraciones --;
			SLEEP( 100 );
        }
        if( borrar == 1 ) {
		    BORRAR_MENSAJE_( NO );
        }
	}
}

/**************************************************************************/
int REPETIR_MIENTRAS_IGUALES( char* mensaje, int* valA, int valB )
/**************************************************************************/
{
	//Se asume que el valA cambia desde otro hilo
    if ( *valA == valB ) {  
        do {  
            MOSTRAR_MENSAJE2( mensaje, 2000, NO );
		    SLEEP( 100 );
	    } while( *valA == valB );
	    BORRAR_MENSAJE_( NO );
        return 1; //(entro)
    }
    return 0; //(no entro)
}

/**************************************************************************/
int PAPER_OUT()
/**************************************************************************/
{
    int ok = 0;
	
	do{
		if( MODO_NEGOCIO != SERVICIOS ) {
			if( IMPRESORA_FISCAL && RAM_P_TICKET < 1 ) {  //en la impresión de items tiramos consulta de papel
				if( /*COMANDO_LEER_STATUS_PRINTER_FISCAL( 1, NULL ) == */GET_ESTADO_FISCAL(_FISCAL_paper_out) ) {
					ok = 1;
				}
				else {
					ok = 0;
				}
			}
			else if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
				getData( ACTION_READ_PRINTER_STATUS, ( char* )&ok, 0 );
			}
			if( ok == 201 ) {
				//MENSAJE_STRING( S_CAMBIE_ROLLO_PAPEL );
                MOSTRAR_MENSAJE2(ST(S_CAMBIE_ROLLO_PAPEL),2000,NO);
			}else if( ok == 204 ) {
				//MENSAJE( "COLOQUE PAPEL EN SLIP" );
                MOSTRAR_MENSAJE2( ST( S_COLOQUE_PAPEL_EN_SLIP ),2000,NO);
			} else if( ok ){
				//MENSAJE( "3- SE DETECTO ERROR EN LA IMPRESORA - LLAME AL SERVICIO TECNICO" );
                MOSTRAR_MENSAJE2("3- SE DETECTO ERROR EN LA IMPRESORA - LLAME AL SERVICIO TECNICO",2000,NO);
				GRABAR_LOG_SISTEMA( "3-SE DETECTO ERROR EN LA IMPRESORA - LLAME SERVICIO TECNICO" ,LOG_JAVA_SERVER,1 );
			}
		}
	} while( ok == 201 || ok == 204 ); // 201: no hay papel en el receipt
    BORRAR_MENSAJE_( NO );
    return ( ok );
}

/**************************************************************************/
int ERROR_IMPRESORA(void )// sumar si otros errores hacen invalidante que siga la impresora.
/**************************************************************************/
{
    int error = NO;
	
	if( MODO_NEGOCIO != SERVICIOS ) {
		if( IMPRESORA_FISCAL && RAM_P_TICKET < 1) {
			COMANDO_LEER_STATUS_PRINTER_FISCAL( 1, NULL ) ;
			if(GET_ESTADO_FISCAL(_FISCAL_mem_llena)||
				GET_ESTADO_FISCAL(_FISCAL_nec_cierre))
				error = SI;

			if(GET_ESTADO_FISCAL(_FISCAL_mem_casi_llena))
				error = NO;
			/*switch( COMANDO_LEER_STATUS_PRINTER_FISCAL( 1, NULL ) ){
				
				case _FISCAL_mem_llena :
				case _FISCAL_nec_cierre :
					error = SI;
				break;
				case _FISCAL_mem_casi_llena :
					error = NO;
				default:
				break;
			}*/
		}
	}
    return ( error );
}
/**************************************************************************/
int TM930_IS_PRINTER()
/**************************************************************************/
{
    char buffer_send[] = {29,5};
    char status[1];
    int ok = 0;
    if( MODO_NEGOCIO != SERVICIOS ) {
        ok = TM930_COMANDO( buffer_send, 2, status, 1 );
        if( ok ) {
            if( status[0] & 4 ) {
                // off line
                MENSAJE_STRING( S_IMPRESORA_ABIERTA );
                ok = 0;
            }
            else {
                if( status[0] & 8 ) {
                    // off line
                    MENSAJE_STRING( S_IMPRESORA_OFF_LINE );
                    ok = 0;
                }
            }
        }
        else {
            MENSAJE_STRING( S_IMPRESORA_ERROR );
        }
    }
    return ( ok );
}
/**************************************************************************/
int TM930_PAPER_OUT()
/**************************************************************************/
{
    char buffer_send[] = {27,118};
    char status[1];
    int out = 0, ok1 = 1;
    char clear_buffer[] = {16};
    if( MODO_NEGOCIO != SERVICIOS ) {
        if( IMPRESORA_INSTALADA ) {
            if( VERIFICAR_IMPRESORA_ON_LINE ) {
                ok1 = TM930_IS_PRINTER();
            }
            if( ok1 ) {
                if( IMPRESORA_FISCAL == FISCAL ) {
                    //ok1 = TM930_COMANDO( clear_buffer, 1, status, 1 );
                    while( PRN_HAY_DATOS() ) {
                        RECIBIR_RESPUESTA_IMPRESORA( (unsigned char *)status, (unsigned char)1 );
                    }
                    ok1 = TM930_COMANDO( clear_buffer, 1, status, 0 );
                }
                ok1 = TM930_COMANDO( buffer_send, 2, status, 1 );
                if( ok1 ) {
                    out = 0;
                    if( IMPRESORA_FISCAL == FISCAL ) {
                        if( status[0] & 4 ) {
                            MENSAJE_STRING( S_RECEIPT_SIN_PAPEL );
                            out = 1;
                        }
                        else if( status[0] & 1 ) {
                            MENSAJE_STRING( S_RECEIPT_SIN_PAPEL );
                            out = 1;
                        }
                        if( status[0] & 8 ) {
                            MENSAJE_STRING( S_RECEIPT_SIN_PAPEL );
                            out = 1;
                        }
                        else if( status[0] & 2 ) {
                            MENSAJE_STRING( S_RECEIPT_SIN_PAPEL );
                            out = 1;
                        }
                    }
                    else {
                        if( status[0] & 4 ) {
                            MENSAJE_STRING( S_JOURNAL_SIN_PAPEL );
                            out = 1;
                        }
                        else if( status[0] & 1 ) {
                            MENSAJE_SIN_SONIDO_STRING( S_JOURNAL_CERCA_FINAL );
                            PAUSA( 7 );
                        }
                        if( status[0] & 8 ) {
                            MENSAJE_STRING( S_RECEIPT_SIN_PAPEL );
                            out = 1;
                        }
                        else if( status[0] & 2 ) {
                            MENSAJE_SIN_SONIDO_STRING( S_RECEIPT_CERCA_FINAL );
                            PAUSA( 7 );
                        }
                    }
                }
                else {
                    MENSAJE_STRING( S_IMPRESORA_ERROR );
                    out = 1;
                }
            }
            else {
                out = 1;
            }
        }
    }
    return ( out );
}
/**************************************************************************/
int TM930_COMANDO( char *buffer_envio, int cantidad_envio, char *buffer_recibir,
                   int cantidad_recibir )
/**************************************************************************/
{
    int ok = 1;
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    int j;
    for( j = 0;j < cantidad_envio;j++ ) {
        SEND_BYTE_SERIAL( buffer_envio[j] );
    }
    if( cantidad_recibir ) {
        if( RECIBIR_RESPUESTA_IMPRESORA( buffer_recibir, cantidad_recibir ) ) {
            ok = 1;
        }
        else {
            ok = 0;
        }
    }
    #endif
    return ( ok );
}
#if defined(INVEL_W) || defined(INVEL_L)
int RECIBIR_RESPUESTA_IMPRESORA( unsigned char *rta, unsigned char cant )
{
    return( 1 );
}
#else
/***************************************************************************/
int RECIBIR_RESPUESTA_IMPRESORA( unsigned char *rta, int cant )
/***************************************************************************/
{
    int ok = 0;
    unsigned status;
    int i = 0, time_out = 0;
    //unsigned long *t = (unsigned long *)0x46c, t_inicial;
    unsigned long *t,t_inicial;
    t = ( void* )MK_FP_BIOS_PROTEGIDO( 0x6c );
    t_inicial = *t;
    while( ( i < cant && !time_out ) || ( cant == 0 && !time_out ) ) {
        status = bioscom( 3, 0, PUERTO_IMPRESORA );
        if( status & 0x100 ) {
            status = bioscom( 2, 0, PUERTO_IMPRESORA );
            rta[i] = status & 0x00ff;
            i++;
        }
        if( *t < t_inicial ) {
            t_inicial = *t;
        }
        else if( *t - t_inicial > 18 ) {
            // 1 segundo
            time_out = 1;
        }
    }
    if( i == cant || cant == 0 ) {
        ok = 1;
    }
    return ( ok );
}
#endif
/**************************************************************************/
int __SEND_BYTE( unsigned char byte, char testear )
/**************************************************************************/
{
    int error = 1, enviar = 1;
    if( MODO_NEGOCIO != SERVICIOS ) {
        #if defined(INVEL_W) || defined(INVEL_L)
        static int i = 0;
        static char fiscal_txt[100];
        #else
        int h;
        #endif
        #ifdef COMPILAR_OMRON
        static int p_buffer = 0;
        #endif
        static char *print_buffer = 0;
        if( !print_buffer ) {
            print_buffer = MALLOC( sizeof( char[200] ) );
        }
        #ifdef COMPILAR_MODO_ENTRENAMIENTO
        if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
            if( byte == ' ' ) {
                byte = '*';
            }
        }
        #endif
        #if defined(INVEL_W) || defined(INVEL_L)
        if( ( _COPIA_PRN )
         && ( !IMPRESORA_FISCAL || ( IMPRESORA_FISCAL && config.pais != ARGENTINA ) ) ) {
            _WRITE( handle_copia, &byte, 1, __LINE__, __FILE__ );
        }
        #else
        if( testear
         && ( SPOOLER || ( TIPO_IMPRESORA == IMPRESORA_DRIVER && ( byte == 13 || byte == 10 ) ) ) ) {
            for( h = 0;error;h++ ) {
                if( IS_PRINTER() ) {
                    error = 0;
                }
                else {
                    if( TIPO_IMPRESORA != TM930 ) {
                        MENSAJE_STRING( S_IMPRESORA_OFF_LINE );
                        PAUSA( 2 );
                    }
                }
                if( h > 30 ) {
                    MENSAJE_STRING( S_ERROR_IMPRESION_ENTER );
                    while( GETCH() != 13 );
                    BORRAR_MENSAJE();
                    h = 0;
                }
            }
        }
        if( _COPIA_PRN ) {
            _WRITE( handle_copia, &byte, 1, __LINE__, __FILE__ );
        }
        #endif
        if( TIPO_IMPRESORA != IMPRESORA_DRIVER ) {
            GRABA_COPIA( 0, byte );
        }
        //*--------------------------- Imprime -------------------------*
        if( TIME_OUT_CORTE_MANUAL && _corto_hoja ) {
            if( YA_PASARON_SEGUNDOS( ( unsigned long* )&_clock_corte_hoja, TIME_OUT_CORTE_MANUAL ) ) {
                _corto_hoja = NO;
            }
            else {
                CORTE_DE_HOJA_MANUAL();
            }
        }
        if( CINTA_TESTIGO_ELECTRONICA && !_SLIP && ( _ESTACION_ACTIVA & JOURNAL ) ) {
            GRABAR_JOURNAL_ELECTRONICO( byte );
            enviar = ( _ESTACION_ACTIVA & RECEIPT ) ? SI : NO;
            if( !enviar ) {
                error = 0;
            }
        }
        if( enviar && redireccionar_print_a_buffer ) {
            //if( byte != 0xa ){
            if( byte == 0xd || byte == 0xa ) {
                redireccion_fila++;
                redireccion_columna = 0;
            }
            else {
                if( redireccion_columna < redireccion_cant_columnas
                 && redireccion_fila < redireccion_cant_filas ) {
                    PRINT_BUFFER[redireccion_columna + redireccion_fila * redireccion_cant_columnas] = 
                    byte;
                }
                else {
                }
                redireccion_columna++;
            }
            //}
            enviar = 0;
        }
        if( enviar ) {
            if( IMPRESORA_FISCAL == FISCAL_IBM_4614 ) {
                if( write( Handle_Fiscal_IBM_4614, &byte, 1 ) == -1 ) {
                    MENSAJE_STRING( S_ERROR_ESCRIBIR_IMPRESORA_FISCAL );
                    error = 1;
                }
            }
            else if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
                GRABA_COPIA( _PRN_enviar_byte, byte );
                DRIVER( _DRV_printer, _PRN_enviar_byte, ( unsigned )byte, &error, sizeof( error ) );
                if( error ) {
                    error = REINTENTAR_impresion();
                }
            }
            else if( TIPO_IMPRESORA == IMPRESORA_OMRON ) {
                #ifdef COMPILAR_OMRON
                /*------------ Elimina acentos y caracteres especiales -----------*/
                if( _FILTRAR_CARACTERES_ESPECIALES ) {
                    switch( byte ) {
                        case 160:
                            byte = 'a';
                            break;
                        case 130:
                            byte = 'e';
                            break;
                        case 161:
                            byte = 'i';
                            break;
                        case 162:
                            byte = 'o';
                            break;
                        case 163:
                            byte = 'u';
                            break;
                        case 164:
                            byte = 'n';
                            break;
                        case 165:
                            byte = 'N';
                            break;
                        case 167:
                            byte = 'o';
                            break;
                    }
                }
                /*---------------------- enva al buffer -------------------------*/
                print_buffer[p_buffer] = byte;
                p_buffer++;
                if( byte == 10 || byte == 0 ) {
                    print_buffer[p_buffer] = 0;
                    OMRON_PRINT( print_buffer );
                    p_buffer = 0;
                }
                #endif
            }
            else if( PUERTO_PARALELO ) {
                #if !defined(INVEL_W) && !defined(INVEL_L)
                biosprint( 0, byte, PUERTO_IMPRESORA );
                #endif
                while( !IS_PRINTER() );
            }
            else {
                #if defined(INVEL_W) || defined(INVEL_L)
                fiscal_txt[i] = 0;
                SET_MEMORY_INT( __varios_imprimiendo_texto_fiscal, 0 );
                i = 0;
                /*          if( RAM_IMPRESION_FISCAL == 1 )
                                //FiscalPrintFiscalText( fiscal_txt );
                            else
                                //FiscalPrintNormal( _SLIP, fiscal_txt );*/
                memset( fiscal_txt, 0, sizeof( fiscal_txt ) );
                #else
                SEND_BYTE_SERIAL( byte );
                #endif
            }
        }
        return ( error );
    }
    else {
        return 0;
    }
}
/**************************************************************************/
int __SEND_BYTE_CODIGO( unsigned char byte )
/**************************************************************************/
{
    int error = 0;
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    #ifdef COMPILAR_OMRON
    static int p_buffer = 0;
    #endif
    static char *print_buffer = 0;
    if( !print_buffer ) {
        print_buffer = MALLOC( sizeof( char[200] ) );
    }
    if( IMPRESORA_INSTALADA ) {
        if( _COPIA_PRN ) {
            _WRITE( handle_copia, &byte, 1, __LINE__, __FILE__ );
        }
        if( TIPO_IMPRESORA != IMPRESORA_DRIVER ) {
            GRABA_COPIA( 0, byte );
        }
        if( IMPRESORA_FISCAL == FISCAL_IBM_4614 ) {
            if( write( Handle_Fiscal_IBM_4614, &byte, 1 ) == -1 ) {
                MENSAJE_STRING( S_ERROR_ESCRIBIR_IMPRESORA_FISCAL );
                error = 1;
            }
        }
        else if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
            GRABA_COPIA( _PRN_enviar_byte, byte );
            DRIVER( _DRV_printer, _PRN_enviar_byte, ( unsigned )byte, &error, sizeof( error ) );
            if( error ) {
                error = REINTENTAR_impresion();
            }
        }
        else if( TIPO_IMPRESORA == IMPRESORA_OMRON ) {
            #ifdef COMPILAR_OMRON
            /*------------ Elimina acentos y caracteres especiales -----------*/
            /*---------------------- enva al buffer -------------------------*/
            print_buffer[p_buffer] = byte;
            p_buffer++;
            if( byte == 10 || byte == 0 ) {
                print_buffer[p_buffer] = 0;
                OMRON_PRINT( print_buffer );
                p_buffer = 0;
            }
            #endif
        }
        else if( PUERTO_PARALELO ) {
            biosprint( 0, byte, PUERTO_IMPRESORA );
        }
        else {
            SEND_BYTE_SERIAL( byte );
        }
    }
    FREEMEM( print_buffer );
    #endif
    return ( error );
}
/**************************************************************************/
void SEND_BYTE_SERIAL( unsigned char byte )
/**************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    #ifdef ENVIAR_A_SERIAL
    int st = 0xe000;
    //  for( h = 0; h < 1500 && ( st & 0xe000 ) == 0xe000; h++ ) {
    //      st = bioscom( 1, byte, PUERTO_IMPRESORA );
    //      PINPAD1_DELAY( 1 );
    //   }
    st = 0;
    while( !( st & 0x6000 ) ) {
        st = bioscom( 3, 0, PUERTO_IMPRESORA );
    }
    bioscom( 1, byte, PUERTO_IMPRESORA );
    #endif
    #endif
}
/**************************************************************************/
void SET_IMPRESORA( int puerto, int paralelo )
/**************************************************************************/
{
    SET_MEMORY_CHAR( __puerto_impresora, puerto );
    SET_MEMORY_CHAR( __puerto_paralelo, paralelo );
}
/**************************************************************************/
void OPEN_COPIA_PRN()
/**************************************************************************/
{
    if( !_COPIA_PRN ) {
        handle_copia = _CREAT( "copia.prn", S_IFREG | S_IWRITE | S_IREAD, __LINE__, __FILE__ );
        if( handle_copia ) {
            _COPIA_PRN = 1;
        }
    }
}
/**************************************************************************/
void CLOSE_COPIA_PRN()
/**************************************************************************/
{
    if( _COPIA_PRN ) {
        _COPIA_PRN = 0;
        _CLOSE( handle_copia, __LINE__, __FILE__ );
		handle_copia = 0;
    }
}
/**************************************************************************/
void IMPRIME_COPIA_PRN()
/**************************************************************************/
{
    char buffer, salir = 0;
    if( !_COPIA_PRN ) {
        #ifdef INVEL_L
        handle_copia = _OPEN( "copia.prn", O_RDONLY, __LINE__, __FILE__ );
        #else       
        //handle_copia = _OPEN( "copia.prn", O_RDONLY | O_BINARY, __LINE__, __FILE__ );
		handle_copia = _OPEN_O_CREAT( "copia.prn", O_RDONLY | O_BINARY, __LINE__, __FILE__ );
        #endif
        if( handle_copia ) {
            while( !salir ) {
                if( !_READ( handle_copia, &buffer, 1, __LINE__, __FILE__ ) ) {
                    SEND_BYTE( buffer );
                }
                else {
                    salir = 1;
                }
            }
            _CLOSE( handle_copia, __LINE__, __FILE__ );
        }
    }
}
/**************************************************************************/
void IMPRIME_GRANDE( int x, char *cadena )
/**************************************************************************/
{
    char c_imp[41];
    int h;
    if( TIPO_IMPRESORA == IMPRESORA_OMRON ) {
        for( h = 0;( unsigned )h < strlen( cadena );h++ ) {
            c_imp[h * 2] = ( char )0xf3;
            c_imp[h * 2 + 1] = cadena[h];
        }
        c_imp[strlen( cadena ) * 2] = 0;
        MOSTRAR_CADENA( x, PRN, c_imp );
    }
    else {
        PRN_LETRA_GRANDE();
        MOSTRAR_CADENA( x, PRN, cadena );
        PRN_LETRA_NORMAL();
    }
}
/*****************************************************************************/
int LEER_STATUS_PRINTER()
/*****************************************************************************/
{
    int rta = 1;
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    int status,h;
    for( h = 0;h < 10;h++ ) {
        status = bioscom( 2, 0, PUERTO_IMPRESORA );
        if( ( status & 0xff00 ) != 0xe000 ) {
            rta = ( status & 0x0010 ) >> 4;
            h = 10;
        }
    }
    #endif
    return ( rta );
}
/**************************************************************************/
int COMANDO_PRN_DRIVER( int comando, int bx )
/**************************************************************************/
{
    int ok, rta = 0;
    if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
        GRABA_COPIA( ( char )comando, bx );
        DRIVER( _DRV_printer, comando, bx, &ok, sizeof( ok ) );
        if( ok != 0 ) {
            ok = REINTENTAR_impresion();
        }
        rta = 1;
    }
    if( IMPRESORA_FISCAL == FISCAL_TMT88 ) {
        rta = 1;
    }
    return ( rta );
}
/**************************************************************************/
int REINTENTAR_impresion()
/**************************************************************************/
{
    int error = 1, h;
    for( h = 0;error;h++ ) {
        DRIVER( _DRV_printer, _PRN_reintentar, 0, &error, sizeof( error ) );
        if( error ) {
            if( h > 20 ) {
                MENSAJE_TECLA_STRING( S_ERROR_IMPRESION_ENTER );
                PAUSA( 7 );
                h = 0;
            }
        }
    }
    return ( error );
}
/**************************************************************************/
void CORTE_DE_HOJA_MANUAL()
/**************************************************************************/
{
    MENSAJE_TECLA_STRING( S_CORTE_EL_PAPEL_Y_PRESIONE_ENTER );
    _corto_hoja = NO;
}
/**************************************************************************/
void OPEN_COPIA_TICKET_PRN()
/**************************************************************************/
{
    int nro;
    nro = NRO_ARCHIVO;
    NRO_ARCHIVO = 0;
    handle_ticket_copia = _CREAT( "TICKET.prn", S_IFREG | S_IWRITE | S_IREAD, __LINE__,
                                    __FILE__ );
	if( !( RAM_FACTURA || RAM_NOTA_CR ) ){
		grabar_copia_ticket = 1;
	} else {
		//Para que no grabe en ticket pero si tiene voucher, si en voucher.prn
		grabar_copia_ticket = 2;
	}
    NRO_ARCHIVO = nro;
}
/**************************************************************************/
void OPEN_COPIA_VOUCHER_PRN()
/**************************************************************************/
{
    int nro;

    nro = NRO_ARCHIVO;
    NRO_ARCHIVO = 0;
    if( HABILITA_COPIA_VOUCHER_PRN ) {
		///quiere decir que ya estaba abierto y debo continuarlo
		if( imprime_copia_voucher == 1){
			CONTINUA_COPIA_VOUCHER_PRN();
			//el handle queda seteado!!!

		}else{
			handle_voucher_copia = _CREAT( "voucher.prn", S_IFREG | S_IWRITE | S_IREAD, __LINE__, __FILE__ );
		}
    }
    NRO_ARCHIVO = nro;
	//imprime_copia_voucher = 0;
	imprime_copia_voucher = 1;
}
/**************************************************************************/
void CONTINUA_COPIA_VOUCHER_PRN()
/**************************************************************************/
{
    int nro;
	char cadena[3];
	memset(cadena,0,sizeof(cadena));
    nro = NRO_ARCHIVO;
    NRO_ARCHIVO = 0;
    if( HABILITA_COPIA_VOUCHER_PRN ) {
        handle_voucher_copia = _OPEN( "voucher.prn", O_RDWR , __LINE__, __FILE__ );
		LSEEK( handle_voucher_copia, 0l, SEEK_END, __LINE__, __FILE__ );
		cadena[0]=0x04;
		cadena[1]=0x01;
		GRABAR_VOUCHER_PRN( cadena ) ;
		LSEEK( handle_voucher_copia, 0l, SEEK_END, __LINE__, __FILE__ );
		/*0000 0001 	1 	01 	SOH 	? 	^A 	Inicio de Encabezado
		0000 0010 	2 	02 	STX 	? 	^B 	Inicio de Texto
		0000 0011 	3 	03 	ETX 	? 	^C 	Fin de Texto
		0000 0100 	4 	04 	EOT 	? 	^D 	Fin de Transmisión*/
    }
    NRO_ARCHIVO = nro;
	imprime_copia_voucher = 2;
}
/**************************************************************************/
int GET_HANDLE_VOUCHER ()
/**************************************************************************/
{
	return(handle_voucher_copia);
}

/**************************************************************************/
int GET_COPIA_VOUCHER ()
/**************************************************************************/
{
	return(_COPIA_VOUCHER_PRN);
}

/**************************************************************************/
void OPEN_Z_PRN()
/**************************************************************************/
{
    int nro;

    nro = NRO_ARCHIVO;
    NRO_ARCHIVO = 0;
    handle_cierre_z_copia = _CREAT( "CIERRE_Z.prn", S_IFREG | S_IWRITE | S_IREAD, __LINE__,
                                    __FILE__ );
	grabar_copia_cierre_z = 1;
    NRO_ARCHIVO = nro;
}

/**************************************************************************/
void CLOSE_COPIA_Z_PRN()
/**************************************************************************/
{
    int nro;
    if(handle_cierre_z_copia){
        nro = NRO_ARCHIVO;
        NRO_ARCHIVO = 0;
        _CLOSE( handle_cierre_z_copia, __LINE__, __FILE__ );
        grabar_copia_cierre_z = 0;
		handle_cierre_z_copia = 0;
        NRO_ARCHIVO = nro;
    }
}
/**************************************************************************/
void CLOSE_COPIA_TICKET_PRN()
/**************************************************************************/
{
    int nro;
    nro = NRO_ARCHIVO;
    NRO_ARCHIVO = 0;
    //if( HABILITA_COPIA_TICKET_PRN ) {
    //    if( VARIOS_COPIA_TICKET_PRN ) {
    //        SET_MEMORY_CHAR( __varios_copia_ticket_prn, 0 );
    _CLOSE( handle_ticket_copia, __LINE__, __FILE__ );
	handle_ticket_copia = 0; 
    //    }
    //}
    NRO_ARCHIVO = nro;
}
/**************************************************************************/
void CLOSE_COPIA_VOUCHER_PRN()
/**************************************************************************/
{
    int nro;
    nro = NRO_ARCHIVO;
    NRO_ARCHIVO = 0;
    if( HABILITA_COPIA_VOUCHER_PRN ) {
        //if( _COPIA_VOUCHER_PRN ) {
           // _COPIA_VOUCHER_PRN = 0;
		_CLOSE( handle_voucher_copia, __LINE__, __FILE__ );
		handle_voucher_copia = 0;
       // }
    }
    NRO_ARCHIVO = nro;
}
/**************************************************************************/
int  IMPRIME_COPIA_VOUCHER( int reimpresion )
/**************************************************************************/
{
	int rta = NO, errorv = 0;
    if( HABILITA_COPIA_VOUCHER_PRN ) {
        if( access( "voucher.prn", 0 ) == 0 && !_COPIA_VOUCHER_PRN ) {
            GRABAR_LOG_SISTEMA( "REIMPRESION VOUCHER" ,LOG_VENTAS,1);
              //lo puse para las fiscales//ojo con un voucher configurable sacado del tkt
			if (IMPRESORA_FISCAL == FISCAL && VOUCHER_TARJETA_FISCAL == SI ) {
				int posicion, cant = 0;
				// veamos cual es el ultimo pago con tarjeta para sacar la reimpresion
				//select posicion,ev_medios.* from ev_medios where  tarjeta > 0 order by id_evento desc
				posicion = POSICION_TARJETA();
				cant = CANT_TARJETAS_REC();
   			    if( cant > 0 ) {
					LEER_TARJETAS_REC();
				} else{
						errorv = 1;
				}
				if( errorv == 0) {
					int i= 0;
					while ( i < cant ) {
						IMPRIME_TKT_DESCRIPCION_TARJETA( posicion, i );
						if( EMITIR_VOUCHER_TARJETA == 2) 
						//aca controlar que haya algo para reimprimir
						// el primer parametro es la posicion de pago del medio y el 2do es la posicion en el vector
							IMPRIME_TKT_DUPLICADO_TARJETA(posicion, i);
						i++;	
					}
				}
			} else {
				PRN_SELECT( RECEIPT );
				imprime_copia_voucher = 0;
				grabar_copia_ticket = 0;
				if( IMPRESORA_FISCAL == FISCAL )
					ABRIR_DOCUMENTO_NO_FISCAL (NO);
				

				if( IMPRESORA_FISCAL == FISCAL){
					rta=  RE_IMPRESION_VOUCHER_POR_VALIDACION( );

				}else{
				
					rta = REIMPRIMIR_TICKET_LOCAL("voucher.prn");
				}
				if( IMPRESORA_FISCAL == FISCAL)
				 	CERRAR_DOCUMENTO_NO_FISCAL();
			}
        }
        else {
			errorv =1;
        }
    }
	if( errorv == 1) 
		MENSAJE_STRING( S_NO_ES_POSIBLE_REIMPRIMIR_EL_VOUCHER );
	return rta;

	//RE_IMPRESION_VOUCHER_POR_VALIDACION//dncx deje aca par aprob arlo hoy no mas
}
/**************************************************************************/
void IMPRIME_COPIA_TICKET( int reimpresion )
/**************************************************************************/
{
    if( HABILITA_COPIA_TICKET_PRN ) {
        if( reimpresion ) {
            GRABAR_LOG_SISTEMA( "REIMPRESION TICKET",LOG_VENTAS,1 );
        }
        //_IMPRIME_COPIA_PRN("TICKET.prn", VARIOS_COPIA_TICKET_PRN, SI);
        _IMPRIME_COPIA_PRN( "TICKET.prn", VARIOS_COPIA_TICKET_PRN, reimpresion );
        // if(_IMPRIME_COPIA_PRN("TICKET.prn",VARIOS_COPIA_TICKET_PRN))
        // IMPRIME_COPIA_VOUCHER();
    }
}
/**************************************************************************/
int _IMPRIME_COPIA_PRN( char *archivo, int copiando_a_archivo, int imprimir_cabecera )
/**************************************************************************/
{
    char buffer[3], salir = 0;
    char comando;
    int handle,bx;
    int error = 1, terminar = 0;
    int nro;
    nro = NRO_ARCHIVO;
    NRO_ARCHIVO = 0;
    if( !copiando_a_archivo ) {
        while( !terminar ) {
            salir = 0;
			if( !IMPRESORA_FISCAL )
            VERIFICAR_IMPRESORA();
            handle = _OPEN( archivo, O_RDONLY, __LINE__, __FILE__ );
            if( handle > 0 ) {
                if( imprimir_cabecera ) {
                    GRABAR_LOG_SISTEMA( "comienza reimpresion",LOG_VENTAS,4 );
                }
                LSEEK( handle, 0L, SEEK_SET, __LINE__, __FILE__ );
                if( IMPRESORA_FISCAL  )
					ABRIR_DOCUMENTO_NO_FISCAL( NO );
				else {
                PRN_HABILITA_AVANCES();
                if( imprimir_cabecera ) {
                    LF( 1 );
                    if( !TKT_CONFIGURABLE ) {
                        _IMPRIME_CABECERA_TICKET( 0, 0, NO );
                        LF( 2 );
                    }
                    //MOSTRAR_CADENA( 1, PRN, "***************REIMPRESION**************" );
                    MOSTRAR_CADENA( 1, PRN, BUSCA_MENSAJE_STRING( S_CADENA_REIMPRESION ) );
                    LF( 2 );
                }
				}
                if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
                    while( !salir ) {
                        if( !_READ( handle, ( char* )buffer, 3, __LINE__, __FILE__ ) ) {
                            comando = buffer[0];
                            bx = ( int )buffer[1];
                            COMANDO_PRN_DRIVER( comando, bx );
                        }
                        else {
                            salir = 1;
                        }
                    }
                }
                else {
                    while( !salir ) {
                        if( !_READ( handle, ( char* )buffer, 1, __LINE__, __FILE__ ) ) {
                            __SEND_BYTE( buffer[0], NO );
                        }
                        else {
                            salir = 1;
                        }
                    }
                }
                _CLOSE( handle, __LINE__, __FILE__ );
                error = 0;
            }
            if( SINCRONIZAR_PRINTER() == 0 ) {
                terminar = 1;
            }
        }
        if( imprimir_cabecera ) {
            GRABAR_LOG_SISTEMA( "fin reimpresion" ,LOG_VENTAS,4);
        }
    }
    NRO_ARCHIVO = nro;
    return !error;
}
/**************************************************************************/
void GRABA_COPIA( char comando, int param )
/**************************************************************************/
{
    char temp[6];
    int nro;
    nro = NRO_ARCHIVO;
    NRO_ARCHIVO = 0;
    if( VARIOS_COPIA_TICKET_PRN ) {
        if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
            temp[0] = comando;
            *( int* )&temp[1] = param;
            //_WRITE( handle_ticket_copia, temp, 3, __LINE__, __FILE__ );
        }
        else {
            _WRITE( handle_ticket_copia, ( char* )&param, 1, __LINE__, __FILE__ );
        }
    }
    if( _COPIA_VOUCHER_PRN ) {
        if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
            temp[0] = comando;
            *( int* )&temp[1] = param;
            //_WRITE( handle_voucher_copia, temp, 3, __LINE__, __FILE__ );
        }
        else {
            _WRITE( handle_voucher_copia, ( char* )&param, 1, __LINE__, __FILE__ );
        }
    }
    NRO_ARCHIVO = nro;
}
/**************************************************************************/
int SINCRONIZAR_PRINTER()
/**************************************************************************/
{
    int rta = 0;
    if( VERIFICAR_SINCRONIZACION_IMPRESO ) {
        if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
            DRIVER( _DRV_printer, _PRN_sincronizar, 0, &rta, sizeof( rta ) );
        }
    }
    return rta;
}
/**************************************************************************/
void VERIFICAR_IMPRESORA()
/**************************************************************************/
{
    int error = 1;
    #define SIN_SINCRONISMO 1
    #define FUERA_DE_LINEA 2
    #define PAPER_OUT 3
    if( VERIFICAR_SINCRONIZACION_IMPRESO ) {
        while( error ) {
            error = SINCRONIZAR_PRINTER();
            if( error ) {
                switch( error ) {
                    case PAPER_OUT:
                        MOSTRAR_MENSAJE2(ST(S_CAMBIE_ROLLO_PAPEL),2000,NO);
                        //MENSAJE_STRING( S_CAMBIE_ROLLO_PAPEL );

                        break;
                    default:
                        //MENSAJE_STRING( S_VERIFIQUE_IMPRESORA );
                        MOSTRAR_MENSAJE2(ST(S_VERIFIQUE_IMPRESORA),2000,NO);
                }
            }
        }
        BORRAR_MENSAJE_( NO );
    }
}
/**************************************************************************/
int TM930_CAJON_CERRADO()
/**************************************************************************/
{
    char buffer_send[] = {29,5};
    char status[1];
    int ok = 0, ok1;
    ok1 = TM930_COMANDO( buffer_send, 2, status, 1 );
    if( ok1 ) {
        if( status[0] & 16 ) {
            // CAJON cerrado
            ok = 1;
        }
        else {
            // cajon abierto
            ok = 0;
        }
    }
    return ( ok );
}
/**************************************************************************/
void INIT_JOURNAL_ELECTRONICO( void )
/**************************************************************************/
{
    /*if( handle_journal == 0 ) {
        buffer_journal = MALLOC( SIZE_BUFFER_JOURNAL + 1 );
        p_buffer_journal = 0;
        if( !NRO_HUBO_OPERACIONES ) {
            SET_MEMORY_LONG( __nro_estado_journal, 1 );
        }
        ABRIR_JOURNAL_ELECTRONICO();
    }*/
	ABRIR_JOURNAL_ELECTRONICO();
}
/**************************************************************************/
void ABRIR_JOURNAL_ELECTRONICO( void )
/**************************************************************************/
{
    int error = 0;
    char nom_destino[80];

    NOM_ARCHIVO( _JOURNAL_DAT, nom_destino );
    #ifdef INVEL_L
    handle_journal = open( nom_destino, O_RDWR, __LINE__, __FILE__ );
    #else
    handle_journal = _open( nom_destino, O_RDWR | O_BINARY/*, __LINE__, __FILE__*/ );
    #endif

    if( handle_journal < 0 ) {
        handle_journal = _CREAT( nom_destino, O_BINARY, __LINE__, __FILE__ );
        if( handle_journal > 0 ) {
            _CLOSE( handle_journal, __LINE__, __FILE__ );
            handle_journal = _OPEN( nom_destino, O_WRONLY | O_BINARY, __LINE__, __FILE__ );
        }
    }
}

/**************************************************************************/
void CERRAR_JOURNAL_ELECTRONICO( void )
/**************************************************************************/
{
    _CLOSE( handle_journal, __LINE__, __FILE__ );
}


/**************************************************************************/
void GRABAR_JOURNAL_ELECTRONICO( unsigned char byte )
/**************************************************************************/
{
    if( byte == 10 ) {
        if( p_buffer_journal && buffer_journal[p_buffer_journal - 1] != 13 ) {
            buffer_journal[p_buffer_journal++] = 13;
        }
    }
    buffer_journal[p_buffer_journal++] = byte;
    if( byte == 10 || p_buffer_journal == SIZE_BUFFER_JOURNAL ) {
        if( ( NRO_ESTADO_JOURNAL + p_buffer_journal ) > SIZE_JOURNAL_FILE ) {
            BACKUP_JOURNAL_ELECTRONICO();
        }
        LSEEK( handle_journal, NRO_ESTADO_JOURNAL, SEEK_SET, __LINE__, __FILE__ );
        _WRITE( handle_journal, buffer_journal, p_buffer_journal, __LINE__, __FILE__ );
        SET_MEMORY_LONG( __nro_estado_journal, NRO_ESTADO_JOURNAL + p_buffer_journal );
        p_buffer_journal = 0;
        DISK_FLUSH();
    }
}
/**************************************************************************/
void GRABAR_JOURNAL_ELECTRONICO2( char *cadena )
/**************************************************************************/
{
	char *cadena_n;
    cadena_n = malloc( strlen( cadena ) + sizeof( char ) * 3 );
    strcpy( cadena_n, cadena );
    lseek( handle_journal, 0, SEEK_END );
	_write( handle_journal, cadena_n, strlen( cadena_n ) );
	 if( cadena_n )
    free( cadena_n );
}

/**************************************************************************/
void BACKUP_JOURNAL_ELECTRONICO( void )
/**************************************************************************/
{
    int h;
    char      fecha_nom[8], remoto[30];
    char      nom[50] = { "C:j" };
    int       handle;
    GRABAR_LOG_SISTEMA( "Backup Journal" ,LOG_VENTAS,4);
    CERRAR_JOURNAL_ELECTRONICO();
    if( config.pais == ELSALVADOR ) {
         /*------ Preparo el Nombre del JOURNAL.BAK  C:jAAMMDD.CCC ------*/
         NOM_CARPETA_ARCHIVO(_JOURNAL_BAK,nom);
         DTOA_INVERSA( GET_FECHA_BACKUP(  ), fecha_nom );
         fecha_nom[6] = 0;
         strcat( nom, "J" );
         strcat( nom, fecha_nom );
         nom[strlen(nom)] = '.';
         strcat( nom, NRO_NOM_CAJA );
         handle = _OPEN_O_CREAT( nom, O_RDWR, __LINE__, __FILE__ );
         _FECHA_ARCHIVO = GET_FECHA_BACKUP(  );
         COPIAR_ARCHIVO( "JOURNAL.DAT", nom, __LINE__, __FILE__ );
         _CLOSE( handle, __LINE__, __FILE__ );
		 strcpy( remoto, "..\\journal\\" );
		 strcat( remoto, &nom[9] );
		 FTP_ARCHIVO_BIN( nom, remoto, PUT );
		 /*-------- Cereo el Journal.DAT -----------*/
    }else{
        BACKUP_ARCHIVO_DE_LA_SEMANA( GET_FECHA_BACKUP(), _JOURNAL_DAT, _JOURNAL_BAK );
    }
    #if defined(INVEL_W) || defined(INVEL_L)
    h = CREAT_( _JOURNAL_DAT, S_IFREG | S_IWRITE | S_IREAD );
    #else
    h = CREAT( _JOURNAL_DAT, O_RDWR );
    #endif
    _CLOSE( h, __LINE__, __FILE__ );
    SET_MEMORY_LONG( __nro_estado_journal, 1 );
    GRABAR_LOG_SISTEMA( "FIN Backup Journal" ,LOG_VENTAS,4);
}
/**************************************************************************/
int IS_SLIP_PRESENT()
/**************************************************************************/
{
    int ok = 1;
    if( IMPRESORA_INSTALADA ) {
        if( IMPRESORA_FISCAL == FISCAL_IBM_4614 ) {
            ok = 1;
        }
        else if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
            DRIVER( _DRV_printer, _PRN_status, 0, &ok, sizeof( ok ) );
            if( ok & _PRN_slip_present ) {
                ok = 1;
            }
            else {
                ok = 0;
            }
        }
        else if( TIPO_IMPRESORA == IMPRESORA_OMRON ) {
            //ok = OMRON_IS_PRINTER();
        }
        else if( PUERTO_PARALELO ) {
            //ok = biosprint(2, 0, PUERTO_IMPRESORA);
            //if ((ok & 0x90) == 0x90)
            //  ok = 1;
            //else
            //  ok = 0;
        }
        else if( TIPO_IMPRESORA == TM930 ) {
            ok = TM930_IS_SLIP_PRESENT();
        }
    }
    return ( ok );
}
/**************************************************************************/
int TM930_IS_SLIP_PRESENT()
/**************************************************************************/
{
    char buffer_send[] = {29,5};
    char status[3];
    int ok = 1;
    status[0] = 0;
    ok = TM930_COMANDO( buffer_send, 2, status, 1 );
    if( ok && ( status[0] & 0x20 ) ) {
        ok = 0;
    }
    return ( ok );
}
/**************************************************************************/
void CALCULAR_CHECK_SUM( unsigned char *cadena, unsigned char *cadena2 )
/**************************************************************************/
{
    int i, suma = 0;
    cadena2[0] = 0x0a;
    cadena2[1] = 0x03;
    for( i = 0;i < ( int )( strlen( (const char *)cadena ) );i++ ) {
        suma += cadena[i];
    }
    suma += cadena2[0];
    suma += cadena2[1];
    suma = ( ~suma ) + 1;
    cadena2[2] = 0;
    cadena2[3] = suma;
}
/**************************************************************************/
void PRN_CAMBIA_SLIP_LANDSCAPE( int landscape )
/**************************************************************************/
{
    if( landscape ) {
        _SLIP = 2;
    }
    else {
        _SLIP = 1;
    }
}
/**************************************************************************/
void IMPRIME_COPIA_COBRO()
/**************************************************************************/
{
    if( HABILITA_COPIA_COBRO_PRN ) {
        GRABAR_LOG_SISTEMA( "REIMPRESION COBRO" ,LOG_VENTAS,2);
        _IMPRIME_COPIA_PRN( "COBRO.prn", VARIOS_COPIA_COBRO_PRN, SI );
    }
}
/**************************************************************************/
void PRN_HABILITAR_LECTURA_CHEQUE()
/**************************************************************************/
{
    int cant = 0;
    char buffer;
    MENSAJE_CON_TECLA_STRING( S_INGRESE_EL_CHEQUE_POR_LA_IMPRESORA, 13 );
    if( TIPO_IMPRESORA != IMPRESORA_DRIVER && !PUERTO_PARALELO ) {
        while( PRN_HAY_DATOS() && cant < 100 ) {
            RECIBIR_RESPUESTA_IMPRESORA( (unsigned char *)&buffer, (unsigned char )1 );
            cant++;
        }
    }
    PRN_HABILITA_SLIP( 1, NO );
    if( !COMANDO_PRN_DRIVER( _PRN_habilitar_cheque, 0 ) ) {
        ENVIAR_CODIGOS( _prn_habilitar_cheque );
    }
}
/**************************************************************************/
void PRN_LEER_CHEQUE()
/**************************************************************************/
{
    if( !COMANDO_PRN_DRIVER( _PRN_leer_cheque, 0 ) ) {
        ENVIAR_CODIGOS( _prn_leer_cheque );
    }
}
/**************************************************************************/
int PRN_HAY_DATOS()
/**************************************************************************/
{
    int ok = 0;
    #if !defined(INVEL_W) && !defined(INVEL_L)
    if( !COMANDO_PRN_DRIVER( _PRN_hay_datos, 0 ) ) {
        if( !PUERTO_PARALELO && ( bioscom( 3, 0, PUERTO_IMPRESORA ) & 0x100 ) ) {
            ok = 1;
        }
    }
    #endif
    return ( ok );
}
/**************************************************************************/
int LEER_DATOS_CHEQUE_IMPRESORA()
/**************************************************************************/
{
    int rta = 0;
    #if !defined(INVEL_W) && !defined(INVEL_L)
    int i;
    char buffer[80];
    struct _driver_cheque
    {
        int rta;
        char *buffer;
        int long_buffer;
    } driver_cheque;
    if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
        if( COMANDO_PRN_DRIVER( _PRN_leer_datos, 0 ) ) {
            driver_cheque.buffer = puntero_memoria_real_driver_invel;
            driver_cheque.long_buffer = 80;
            DRIVER( _DRV_printer, _PRN_leer, 0, &driver_cheque, sizeof( driver_cheque ) );
            if( driver_cheque.rta ) {
                MENSAJE_STRING( S_ERROR_EN_EL_LECTOR_DE_CHEQUES );
            }
            else {
                SET_MEMORY( __ram_cadena_cheque, puntero_memoria_real_driver_invel );
                rta = 1;
            }
        }
    }
    else if( !PUERTO_PARALELO ) {
        memset( buffer, 0, 80 );
        if( RECIBIR_RESPUESTA_IMPRESORA( buffer, 0 ) ) {
            for( i = 0 ;i < 79 && ( buffer[i] < '0' || buffer[i] > '9' );i++ ) {
                ;
            }
            SET_MEMORY( __ram_cadena_cheque, &buffer[i] );
            rta = 1;
        }
    }
    #endif
    return ( rta );
}
/**************************************************************************/
int STATUS_PRINTER_FISCAL()
/**************************************************************************/
{
    char buffer_send[] = {29,114,1};
    char status[1],cadena[37];
    int out = 0, ok1 = 1;
    if( IMPRESORA_INSTALADA ) {
        sprintf( cadena, ST( S_PENDIENTES_EN_SLIP__COLOQUE_EL_PAPEL ) );
        if( VERIFICAR_IMPRESORA_ON_LINE ) {
            ok1 = TM930_IS_PRINTER();
        }
        if( ok1 ) {
            ok1 = TM930_COMANDO( buffer_send, 3, status, 1 );
            if( ok1 ) {
                {
                    char aux[100];
                    sprintf( aux, "S: %x", status[0] );
                    glog( aux ,LOG_DEBUG,4);
                }
                out = 0;
                if( status[0] & 32 ) {
                    MENSAJE( cadena );
                    out = 1;
                }
                if( status[0] & 64 ) {
                    MENSAJE( cadena );
                    out = 1;
                }
            }
            else {
                MENSAJE( cadena );
                out = 1;
            }
        }
        else {
            out = 1;
        }
    }
    return ( out );
}
/**************************************************************************/
int REDIRECCION_PRINTER_A_BUFFER( int cant_columnas, int cant_filas )
/**************************************************************************/
{
    int ok = 0;
    if( PRINT_BUFFER ) {
        free( PRINT_BUFFER );
    }
    PRINT_BUFFER = malloc( cant_filas * cant_columnas ) ;
    if( PRINT_BUFFER ) {
        memset( PRINT_BUFFER, 0, cant_columnas * cant_filas );
        ok = 1;
        redireccionar_print_a_buffer = 1;
        redireccion_columna = redireccion_fila = 0;
        redireccion_cant_columnas = cant_columnas;
        redireccion_cant_filas = cant_filas;
    }
    return( ok );
}
/**************************************************************************/
void FIN_REDIRECCION_PRINTER_A_BUFFER()
/**************************************************************************/
{
    redireccionar_print_a_buffer = 0;
}
/**************************************************************************/
void IMPRIME_BMP_LOGO( char *nomarch )
/**************************************************************************/
{
    char buffer, salir = 0;
    #ifdef INVEL_L
    handle_bmp = _OPEN( nomarch, O_RDONLY, __LINE__, __FILE__ );
    #else   
    handle_bmp = _OPEN( nomarch, O_RDONLY | O_BINARY, __LINE__, __FILE__ );
    #endif
    if( handle_bmp ) {
        while( !salir ) {
            if( !_READ( handle_bmp, &buffer, 1, __LINE__, __FILE__ ) ) {
                SEND_BYTE( buffer );
            }
            else {
                salir = 1;
            }
        }
        _CLOSE( handle_bmp, __LINE__, __FILE__ );
    }
}
/**************************************************************************/
void DISK_FLUSH( void )
/**************************************************************************/
{
    #if !defined(INVEL_W) && !defined(INVEL_L)
    union REGS regs;
    int ret;
    regs.h.ah = 0x0D;
    ret = intdos( &regs, &regs );
    #endif
}

/******************************************************************************/
//void SET_TIPO_LETRA( int tipo_letra )
/******************************************************************************/
/*{
    _tipo_letra_prn = tipo_letra;
}*/
/******************************************************************************/
//void NEXT_LINE_PRN( int next_line )
/******************************************************************************/
/*{
    new_line = next_line;
}*/
/******************************************************************************/
int TEXTO_EN_IMPRESORA_FISCAL( void )
/******************************************************************************/
{return ( IMPRESORA_FISCAL == FISCAL && RAM_IMPRESION_FISCAL == -1 );}

/**************************************************************************/
void CARACTERES_ESPECIALES( char *texto, int origen )
/**************************************************************************/
{
	int k, len = 0;

	len = strlen( texto );
	for( k = 0; texto[k] && k < len; k++ ) {
		//Se limita PROVISORIAMENTE el uso de estas dos teclas porque genera un 
		//exception en el JavaServer cuando se quiere imprimir una cadena que los contenga. 
		switch ( texto[k] ) 
		{
			case 60: /* < */
			case 62: /* > */
			case 38: /* & */
			case -126: 
				if( origen )
					texto[k] = ' ';
				else 
					texto[k] = 38;
				break;
		}
       //if( texto[k] >= 127 )
         //texto[k] = ' ';
   }

}
