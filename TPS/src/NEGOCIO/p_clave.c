#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <b_mens.h> 
#include <cr.h> 
#include <getch.h> 
#include <m_cadena.h> 
#include <pant.h> 
#include <p_clave.h> 
#include <tkt.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#endif
extern void ELIMINAR_LETRA_STR( char *str, int pos );
extern void INSERTAR_LETRA_STR( char *str, int letra, int pos );
/*****************************************************************************/
int PEDIR_CLAVE( int x, int y, int tipo )
/*****************************************************************************
Devuelve la clave ingresada o -1 si se presiona ESC
*****************************************************************************/
{
    char cadena[9], cadena2[9], seguir = 1, mostrar = 1, largo = 4;
    char signed posicion;
    int rta = -1, k, extendida;

	extendida = CLAVE_EXTENDIDA( tipo );
    largo = extendida ? 8 : 4;

    memset( cadena, 0, largo );
    cadena[( int )largo] = 0;
    memset( cadena2, 0, largo );
    cadena2[( int )largo] = 0;
    posicion = 0;
    while( seguir ) {
        if( mostrar ) {
			MOSTRAR_INPUT_CADENA( cadena2, posicion );
            mostrar = 0;
        };
        k = GETCH();
        switch( k ) {
            case 27:
                /* ESC */
                seguir = 0;
				memset( cadena, 0, largo );
				memset( cadena2, 0, largo );
				cadena[( int )largo] = 0;
				posicion = 0;
				MOSTRAR_INPUT_CADENA( cadena2, posicion );
                rta = -1;
                break;
            case 13:
                /* ENTER */
                seguir = 0;
                rta = atoi( cadena );
                memset( cadena2, 0, largo );
                cadena2[( int )largo] = 0;
                MOSTRAR_INPUT_CADENA( cadena2, 0 );
                break;
            case 8:
                /* BACKSPACE */
                ELIMINAR_LETRA_STR( cadena, posicion );
                ELIMINAR_LETRA_STR( cadena2, posicion );
                if( posicion > 0 ) {
                    posicion--;
                }
                mostrar = 1;
                break;
            case 127:
                /* DEL / SUPR */
                if( posicion < ( int )strlen( cadena ) ) {
                    ELIMINAR_LETRA_STR( cadena, ( posicion + 1 ) );
                    ELIMINAR_LETRA_STR( cadena2, ( posicion + 1 ) );
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
            default:
                if( k >= '0' && k <= '9' ) {
                    if( ( int )strlen( cadena ) < largo ) {
                        INSERTAR_LETRA_STR( cadena, k, ( int )posicion );
                        INSERTAR_LETRA_STR( cadena2, '*', ( int )posicion );
                        posicion++;
                    }
                    mostrar = 1;
                };
                break;
        }
    }
    SET_ESTADO_CURSOR( 1 );
    _ATRIBUTO = 7;
    BORRAR_MENSAJE();
    return ( rta );
}

