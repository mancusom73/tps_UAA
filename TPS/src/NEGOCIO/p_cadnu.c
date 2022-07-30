#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cmemory.h> 
#include <cobros_esp.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <getch.h> 
#include <m_cadena.h> 
#include <pant.h> 
#include <pedir.h> 
#include <scanner.h> 
#include <tar_mag.h> 
#include <bridge.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#endif
void ELIMINAR_LETRA_STR( char *str, int pos );
void INSERTAR_LETRA_STR( char *str, int letra, int pos );
extern int  _MASCARA_ ;
/******************************************************************************/
int PEDIR_CADENA_NUMERICA( char x, char y, char largo, char *cadena, char *mascara_scanner,
                           int decimal, int cerear_cadena, int permite_flecha , char caracter_mostrar )
/******************************************************************************/
{
    char seguir = 1, mostrar = 1;
    int asignar = 1;
    char signed posicion;
    int rta = 0, k, l, aux = 0;
	char * cadena2  = (char*) MALLOC((int) ( largo + 1 ) );
	
	memset(cadena2,0, (int) ( largo + 1 ) );

    //    int       *rta1;
    //    char aux1[100];
    //memset( cadena, ' ', largo );

	if( _MASCARA_ == SI ){
		caracter_mostrar = '*';
	}
    if( caracter_mostrar != NULL )
        _MASCARA_ = SI;


    if(cerear_cadena){
        memset( cadena, 0, largo );
        cadena[( int )largo] = 0;
        posicion = 0;
    }else{
        posicion = strlen(cadena);
    }
	

    INIT_PEDIR( x, y, largo );
    while( seguir ) {
        if( mostrar ) {
            MOSTRAR_INPUT_CADENA( cadena, posicion );
			if( _MASCARA_ != 0)
				MOSTRAR_INPUT_CADENA( cadena2, posicion );
            mostrar = 0;
        };
        k = GETCH();
        switch( k ) {
            case 27:
                /* ESC */
                seguir = 0;
                asignar = 0;
				if( _MASCARA_ != 0)
					memset( cadena2, 0, sizeof(cadena2 ));
                break;
            case 13:
                /* ENTER */
                seguir = 0;
                break;
            case 8:
                /* BACKSPACE */
                ELIMINAR_LETRA_STR( cadena, posicion );
				if( _MASCARA_ != 0)
					ELIMINAR_LETRA_STR( cadena2, posicion );
                if( posicion > 0 ) {
                    posicion--;
                }
                mostrar = 1;
                if( decimal > 1 ) {
                    decimal --;
                }
                break;
            case 127:
                /* DEL / SUPR */
                if( posicion < ( int )strlen( cadena ) ) {
                    ELIMINAR_LETRA_STR( cadena, ( posicion + 1 ) );
					if( _MASCARA_ != 0)
						ELIMINAR_LETRA_STR( cadena2, ( posicion + 1 ) );
                }
                mostrar = 1;
                break;
            case -72:
                // Flecha Arriba
			case -80:
				// Flecha Abajo
                seguir = 0;
                asignar = 0;
                if(!permite_flecha)
                    rta = -3;
                else
                    rta = k;
                break;
            case -75:
                /* FLECHA DERECHA */
                if( posicion < ( int )strlen( cadena ) ) {
                    posicion++;
                }
                mostrar = 1;
                break;
            case -74:
                /* FLECHA IZQUIERDA */
                if( posicion > 0 ) {
                    posicion--;
                }
                mostrar = 1;
                break;
            case -76:
                /* TECLA INICIO */
                posicion = 0;
                mostrar = 1;
                break;
            case -77:
                /* TECLA FIN */
                posicion = strlen( cadena );
                mostrar = 1;
                break;
            case -999:
                if( _PERMITIR_SCANNER ) {
                    l = ( ( mascara_scanner && strlen( mascara_scanner ) ) ) ? strlen( mascara_scanner ) : largo;
                    if( LEER_CADENA_SCANNER( cadena, l ) ) {
                        if( mascara_scanner && strlen( mascara_scanner )) {
                            seguir = PEDIR_VALIDAR_MASCARA( cadena, mascara_scanner );
                        }
						else {
							seguir = 0;
						}
						MOSTRAR_INPUT_CADENA( cadena, 0 );
						if( VAR_TARJETAS_LECTURA_BANDA == 0 )
							SET_MEMORY_CHAR( __var_tarjetas_lectura_banda, 2 );
					}
                }
                if( _PERMITIR_BANDA ) {
                    if( LECTOR_TARJETA_LEIDA() ) {
                        seguir = 0;
                        asignar = 0;
                        aux = 1;
                        rta = -2;
                    }
                }
                break;
            case 32:
                /* BARRA ESPACIADORA */
                if( MODO_NEGOCIO == SERVICIOS ) {
                    seguir = 0;
                    asignar = 2;
                }
                break;
            default:
                if( MODO_NEGOCIO == SERVICIOS ) {
                    seguir = TECLAS_COBROS_ESPECIALES( k, ( int* )&rta, seguir, ( int* )&asignar );
                }
                if( ( k >= '0' && k <= '9' ) || ( k == '.' && decimal == 1 ) ) {
                    if( decimal < ( DECIMALES + 2 ) ) {
                        if( ( int )strlen( cadena ) < largo ) {
                            INSERTAR_LETRA_STR( cadena, k, ( int )posicion );
							if( _MASCARA_ != 0)
								INSERTAR_LETRA_STR( cadena2, caracter_mostrar , ( int )posicion );
                            posicion++;
                        }
                        mostrar = 1;
                        if( k == '.' || decimal > 1 ) {
                            decimal ++;
                        }
                    }
                }
                break;
        }
    }
    if( aux ) {
        rta = -2;
    }
    else if( asignar ) {
        rta = 1;
    }
    if( MODO_NEGOCIO == SERVICIOS ) {
        if( asignar == 2 ) {
            rta = 2;
        }
    }
    CLOSE_PEDIR();
	if( cadena2)
         free(cadena2);
    return ( rta );
}
/******************************************************************************/
int PEDIR_VALIDAR_MASCARA( char *cadena, char *mascara_scanner )
/******************************************************************************/
{
    char _cadena[30];
    char *letra_mascara = mascara_scanner;
    char *letra_scanner = cadena;
    int error = 0, iError = 0, p_cadena = 0;
    memset( _cadena, 0, sizeof( _cadena ) );
    while( *letra_mascara && !iError ) {
        if( *letra_mascara >= '0' && *letra_mascara <= '9' ) {
            if( *letra_mascara != *letra_scanner ) {
                iError = 1;
                break;
            }
        }
        else {
            switch( *letra_mascara ) {
                case 'C':
                    // Cadena
                    _cadena[p_cadena++] = *letra_scanner;
                    break;
            }
        }
        letra_mascara++;
        letra_scanner++;
    }
    if( !iError ) {
        strcpy( cadena, _cadena );
    }
    return ( error );
}
/******************************************************************************/
void ELIMINAR_LETRA_STR( char *str, int pos )
/******************************************************************************/
{
    if( pos > 0 ) {
        str[pos - 1] = 0;   //fin de cadena
        strcat( str, &str[pos] ); //mueve
    }
}
/******************************************************************************/
void INSERTAR_LETRA_STR( char *str, int letra, int pos )
/******************************************************************************/
{
    char *p_char;
    p_char = ( char* )malloc( ( size_t )( strlen( str ) + 1 ) * sizeof( char ) );
    strcpy( p_char, str ); // hace una copia de la cadena
    str[pos] = ( char )letra;
    str[pos + 1] = 0;
    strcat( str, &p_char[pos] ); //mueve
    free( p_char );
}

