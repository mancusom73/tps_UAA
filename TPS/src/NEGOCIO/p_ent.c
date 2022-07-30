#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cobros.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <getch.h> 
#include <m_cadena.h> 
#include <pant.h> 
#include <pedir.h> 
#include <scanner.h> 
#include <tar_mag.h> 
#include <actions.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifndef INVEL_L
#endif
long PEDIR_ENTERO_COD_BARRA( char x, char y, char largo, long _default, char **cod_barra, int busqueda, int identificacion_barra, char *titulo );
extern void ELIMINAR_LETRA_STR( char *str, int pos );
extern void INSERTAR_LETRA_STR( char *str, int letra, int pos );
/******************************************************************************/
long PEDIR_ENTERO( int x, int y, int largo, int busqueda )
/******************************************************************************/
{
    return PEDIR_ENTERO_COD_BARRA( ( char )x, ( char )y, ( char )largo, 0, NULL, busqueda, NO, "" );
}
/******************************************************************************/
long PEDIR_ENTERO_COD_BARRA( char x, char y, char largo, long _default, char **cod_barra, int busqueda, int identificacion_barra, char *titulo )
/******************************************************************************/
{
    char cadena[20], seguir = 1, asignar = 1, mostrar = 1;
    char signed posicion;
    long rta = -1, k;
    int i;
    static char codigo_barra[100];
    int longitud_codigo_barra = 47;
	char modo_identificacion_supervisor = MODO_IDENTIFICACION_SUPERVISOR;
	char modo_identificacion_gerente = MODO_IDENTIFICACION_GERENTE;

	memset(cadena,0,sizeof(cadena));
	memset(codigo_barra,0,sizeof(codigo_barra));

    if( cod_barra != NULL ) {
        *cod_barra = NULL;
        SET_ESPERA_GETCH( 2 );
    }
    if( _default != 0 ) {
        _snprintf(cadena, sizeof(cadena)-1, "%ld", _default );
    }
    else {
        memset( cadena, 0, min(largo,20) );
    }
    cadena[( int )min(largo,19)] = 0;
    posicion = 0;
    INIT_PEDIR( x, y, largo );
    while( seguir ) {
        if( mostrar ) {
            MOSTRAR_INPUT_CADENA( cadena, posicion );
            mostrar = 0;
        }
		
		if( identificacion_barra 
			&& SCANNER_HABILITADO_POR_TECLADO 
			&& ( modo_identificacion_supervisor == IDENTIFICACION_BARRA
					|| modo_identificacion_gerente == IDENTIFICACION_BARRA ) ) {
			k = -65;
		} else {
			k = GETCH(); 
		}
		
        TESTEAR_EXCEPCION( ( int * ) &k );
        switch( k ) {
            case 27:
                /* ESC */
				seguir = 0;
                asignar = 0;
				if( _default != 0 ) {
					_snprintf(cadena, sizeof(cadena)-1, "%ld", _default );
				}
				else {
					memset( cadena, 0, min(largo,20) );
				}
				posicion = 0;
				cadena[( int )min(largo,19)] = 0;
				MOSTRAR_INPUT_CADENA( cadena, posicion );
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
            case -999:
				if( posicion == 0){//si ya empezamos a cargar a mano no usamos lo otros medios de ingreso--> ahorro tiempo
					if( cod_barra != NULL ) {
							if( _PERMITIR_SCANNER && IS_SCANNER() ) {
							if( LEER_SCANNER( (unsigned char *)codigo_barra, sizeof( codigo_barra ) ) ) {
								BEEP( 2000, 1000 );
								*cod_barra = codigo_barra;
								seguir = 0;
								asignar = 0;
								rta = -3;
							}
						}
					}
					else {
						if( _PERMITIR_SCANNER && posicion == 0) {  
							if( LEER_CADENA_SCANNER( cadena, largo ) ) {
								seguir = 0;
							}
						}
					}
					if( _PERMITIR_BANDA ) {
						if( LECTOR_TARJETA_LEIDA() ) {
							seguir = 0;
							asignar = 0;
							rta = -2;
						}
					}
				}
                break;
			case -65 ://usamos F7 por similitud con el pedido de tarjeta manual en lugar de banda
            case 212:
                // scanner por teclado
                memset( codigo_barra, 0, sizeof( codigo_barra ) );
				//desactivar el pedido anterior
				ExecuteAction( A_FIN_PEDIR );
				ExecuteSSAction( A_PEDIR, _DOBLE, ( strlen( titulo ) > 2 ) ? titulo :"INGRESE CODIGO BARRA","Codigo:", NULL, " " );
                if( PEDIR_CADENA_NUMERICA( 3, 16, ( char )longitud_codigo_barra, codigo_barra, NULL,
                                            0, SI, NO,'*' ) != 1 ) {
                    rta = 0;
                }
                else {
                    for( i = 0;i < longitud_codigo_barra;i++ ) {
                        if( cod_barra[longitud_codigo_barra - i - 1] == ( char* )' ' ) {
                            cod_barra[longitud_codigo_barra - i - 1] = 0x00;
                        }
                        else {
                            break;
                        }
                    }
                    *cod_barra = codigo_barra;
                }
                seguir = 0;
                asignar = 0;
				rta = 0;
                ExecuteAction( A_FIN_PEDIR );
                break;
            //case '%':
                /*---------- Consulta ------------*/
                /*if(busqueda){
                    rta = -3;
                    seguir = 0;
                    asignar = 0;
                }
                break;*/

            default:
                if( k >= '0' && k <= '9' ) {
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
        rta = atol( cadena );
    }
    CLOSE_PEDIR();
    SET_ESPERA_GETCH( 0 );
    return ( rta );
}

