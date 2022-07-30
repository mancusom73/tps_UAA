#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <getch.h> 
#include <m_cadena.h> 
#include <pant.h> 
#include <pedir.h> 
#include <scanner.h> 
#include <tar_mag.h> 
#include <ctype.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#endif
extern void ELIMINAR_LETRA_STR( char *str, int pos );
extern void INSERTAR_LETRA_STR( char *str, int letra, int pos );
extern void LIMPIA_INDICADOR_TARJETA_LEIDA( void );
/******************************************************************************/
int PEDIR_CADENA( char x, char y, char largo, char *cadena, char *mascara_scanner, int cerear_cadena, int permite_flecha )
/******************************************************************************/
{
    char seguir = 1, asignar = 1, mostrar = 1;
    char signed posicion;
    int rta = 0, k, l, restriccion = 0;
	
	

    //memset( cadena, ' ', largo );
    if(cerear_cadena){
        memset( cadena, 0, largo );
        cadena[( int )largo] = 0;
        posicion = 0;
    }else{
        posicion = strlen(cadena);
    }
	if( _PERMITIR_BANDA )
		LIMPIA_INDICADOR_TARJETA_LEIDA();

    INIT_PEDIR( x, y, largo );
    while( seguir ) {
        if( mostrar ) {
            MOSTRAR_INPUT_CADENA( cadena, posicion );
            mostrar = 0;
        }
        k = GETCH();
		if( k!= -999 )
			CARACTERES_ESPECIALES( &k, _INGRESAR_CADENA );
        switch( k ) {
			//Se limita PROVISORIAMENTE el uso de estas dos teclas porque genera un 
			//exception en el JavaServer cuando se quiere imprimir una cadena(ej nombre) 
			//que contenga estos caracteres. Error al ingresar Datos en Alta de Cliente
			//Ver con Nico el error de java y eliminar limitación 
			case 38: /* & */
				;
			break;
            case 27:
                /* ESC */
                seguir = 0;
                asignar = 0;
                break;
            case 13:
                /* ENTER */
                seguir = 0;
                break;
            case 8:
                /* BACKSPACE */
                ELIMINAR_LETRA_STR( cadena, posicion );
                if( posicion > 0 ) {
                    posicion--;
                }
                mostrar = 1;
                break;
            case 127:
                /* DEL / SUPR */
                if( posicion < ( int )strlen( cadena ) ) {
                    ELIMINAR_LETRA_STR( cadena, ( posicion + 1 ) );
                }
                mostrar = 1;
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

            case -999:
                if( _PERMITIR_SCANNER ) {
                    l = ( ( mascara_scanner && strlen( mascara_scanner ) ) ) ? strlen( mascara_scanner ) : largo;
                    if( LEER_CADENA_SCANNER( cadena, l ) ) {
                        if( mascara_scanner && strlen( mascara_scanner )) {
                            seguir = PEDIR_VALIDAR_MASCARA( cadena, mascara_scanner );
                        }
	                    else{
						   seguir = 0;
						}
						SET_LASTKEY( 13 );
						MOSTRAR_INPUT_CADENA( cadena, 0 );
                    }
                }
                if( _PERMITIR_BANDA ) {
                    if( LECTOR_TARJETA_LEIDA() ) {
                        seguir = 0;
                        asignar = 0;
                        rta = -2;
                    }
                }
                break;
            default:
				restriccion = ( k >= 0 && k < 256 ) ? 1 : 0;
                if( restriccion && ( isalnum( k ) || ispunct( k ) || k == ' ' ) ) {
                    if( ( int )strlen( cadena ) < largo ) {
                        INSERTAR_LETRA_STR( cadena, k, ( int )posicion );
                        posicion++;
                    }
                    mostrar = 1;
                }
                break;
        }
    }
    if( asignar ) {
        rta = 1;
    }
    CLOSE_PEDIR();
    return ( rta );
}

