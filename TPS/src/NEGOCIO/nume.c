/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Esta funcion recibe un numero y devuelve una cadena compuesta por el
 * importe en letras.
 * El segundo parametro es logico y nos informa si la conversion es con
 * decimales
 * ------------------------------------------------------------------------------
 */
#include <string.h>
#include <stdlib.h>
#include <math.h>
#pragma pack(1)
#include <cr.h>
#include <bridge.h>
#include <cmemory.h>
#include <ftoc.h>
#include <cerear.h>

static char *Lista1[] = {"UN","DOS","TRES","CUATRO","CINCO","SEIS","SIETE","OCHO","NUEVE"};
static char *Lista2[] = {"DIEZ","VEINTE","TREINTA","CUARENTA","CINCUENTA","SESENTA","SETENTA",
                         "OCHENTA","NOVENTA"};
static char *Lista3[] = {"CIEN","DOSCIENTOS","TRESCIENTOS","CUATROCIENTOS","QUINIENTOS",
                         "SEICIENTOS","SETECIENTOS","OCHOCIENTOS","NOVECIENTOS"};
static char *Lista4[] = {"ONCE","DOCE","TRECE","CATORCE","QUINCE","DIECISEIS","DIECISIETE",
                         "DIECIOCHO","DIECINUEVE"};
static char *Lista5[] = {"VEINTIUN","VEINTIDOS","VEINTITRES","VEINTICUATRO","VEINTICINCO",
                         "VEINTISEIS","VEINTISIETE","VEINTIOCHO","VEINTINUEVE"};
static char *Lista6[] = {"","MIL","MILLON","MIL MILLON","BILLON"};
static int procesar_modulo( char *digitos, char modu, char modulos, char *buffer );
/****************************************************************************/
void NUM_A_LETRAS( double importe, char *texto )
/****************************************************************************/
{
    char cadena_decimal[5],sub;
	char cadena_decimal1[5];
    char cadena[16] = {"               "};
    char modu,modulos;
    unsigned valor_modulo;
    /*---------------------- Transforma a cadena -----------------------*/
    sub = 15 - ENTEROS - 3 - 4;
    FTOC( importe, &cadena[( int )sub], ENTEROS + 4, DECIMALES, 0, 0 );
    CEREAR( cadena, 15 );
    cadena_decimal[0] = 0;
    if( DECIMALES ) {
        cadena_decimal[0] = '0';
        strcpy( &cadena_decimal[1], &cadena[13] );
    }
    /*-------------------------- Inicializaciones ---------------------*/
    cadena[12] = 0;
    modulos = 4;
    texto[0] = 0;
    /*---------------- Creamos la parte entera del importe ------------*/
    for( modu = 0;modu < modulos;modu++ ) {
        valor_modulo = procesar_modulo( &cadena[( modu * 3 )], ( char )( modu + 1 ), modulos,
                                        texto );
        if( valor_modulo ) {
            strcat( texto, Lista6[modulos - modu - 1] );
            if( modulos - modu > 2 && valor_modulo > 1 ) {
                strcat( texto, "ES " );
            }
            else {
                strcat( texto, " " );
            }
        }
    }
	if( MODO_NEGOCIO != SERVICIOS && strcmp( texto, "" ) && config.pais != ELSALVADOR ) { 
		strcat( texto, "PESOS " );
	}
    /*------------ Creamos la parte decimal del importe si la hay -----------*/
    valor_modulo = atoi( cadena_decimal );
	if( MODO_NEGOCIO != SERVICIOS && config.pais != ELSALVADOR ) { 
		if( DECIMALES && valor_modulo > 0 ) {
			strcpy( &cadena_decimal1[0], &cadena_decimal[1] );
			cadena_decimal1[DECIMALES] = 0;
			strcat( texto, cadena_decimal1 );
			strcat( texto, "/100" );
		}
	} else {
		if( DECIMALES && valor_modulo > 0 ) {
			strcat( texto, "CON " );
			//procesar_modulo( &cadena_decimal, 1, 3, &texto );
			procesar_modulo( cadena_decimal, 1, 3, texto );
			if( valor_modulo > 1 ) {
				strcat( texto, "CENTAVOS" );
			}
			else {
				strcat( texto, "CENTAVO" );
			}
		} 
	}
	if( MODO_NEGOCIO != SERVICIOS && strcmp( texto, "" ) && config.pais != ELSALVADOR ) { 
		strcat( texto, " M.N." );
	}

CEREAR( cadena, 15 );
CEREAR( cadena_decimal1, 4 );
CEREAR( cadena_decimal, 4 );

}
/***************************************************************************/
static int procesar_modulo( char *digitos, char modu, char modulos, char *buffer )
/***************************************************************************/
{
    unsigned valor_m,valor_d,i,valor_s;
    char modulo[4],digito;
    char palabra[50],seguir;
    memcpy( &modulo, digitos, 3 );
    modulo[3] = 0;
    valor_m = atoi( modulo );
    for( i = 0, seguir = 1;i < 3 && seguir;i++ ) {
        digito = modulo[i];
        valor_d = digito - '0';
        if( valor_d ) {
            switch( i ) {
                case 0:
                    /** 9xx **/
                    strcpy( palabra, Lista3[valor_d - 1] );
                    if( valor_d == 1 && valor_m != 100 ) {
                        strcat( palabra, "TO" );
                    }
                    break;
                case 1:
                    /** x9x **/
                    strcpy( palabra, Lista2[valor_d - 1] );
                    valor_s = modulo[2] - '0';
                    if( valor_d == 1 && valor_s != 0 ) {
                        strcpy( palabra, Lista4[valor_s - 1] );
                        seguir = 0;
                    }
                    else if( valor_d == 2 && valor_s != 0 ) {
                        strcpy( palabra, Lista5[valor_s - 1] );
                        seguir = 0;
                    }
                    else if( valor_d > 2 && valor_s != 0 ) {
                        strcat( palabra, " Y" );
                    }
                    break;
                case 2:
                    /** xx9 **/
                    strcpy( palabra, Lista1[valor_d - 1] );
                    if( valor_d == 1 && modu < modulos ) {
                        if( valor_m == 1 && ( modulos - modu ) == 1 ) {
                            palabra[0] = 0;
                        }
                        else {
                            strcpy( palabra, "UN" );
                        }
                    }
                    break;
            }
            strcat( palabra, " " );
            strcat( buffer, palabra );
        }
    }
    return ( valor_m );
}

