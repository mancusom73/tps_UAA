#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cobros_esp.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <ftoc.h> 
#include <getch.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <pago_tc.h> 
#include <pant.h> 
#include <pedir.h> 
#include <p_float.h> 
#include <scanner.h> 
#include <trim.h> 
#include <valida.h> 
#include <cmemory.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#endif
static char _ACEPTAR_SIGNO = 0;
double pesos[] = {100.0,50.0,20.0,10.0,5.0,2.0,1.0,.5};
int INDICE_PESOS( int k );
void format_float(char *buff, char *source,int decimals)
{
	int i = 1,j = 0,decimal = 0,length = strlen(source);
	buff[0] = ',';
	while( j < length ){
		if( length - j <= decimals && !decimal ){
			buff[i] = '.';
			decimal = 1;
			i++;
		}else{
			if ( !( ( length - j - decimals ) %3 ) && i > 1 ){
				buff[i] = ',';
				i++;
			}
		}
		buff[i] = source[j];
		i++;
		j++;
	}
	buff[i] = 0;
	if(!length)
		strcpy(buff,",.");
}

/******************************************************************************/
double PEDIR_FLOTANTE( int x, int y, int enteros, int decimales )
/******************************************************************************/
{
    char cadena[] = {"               "},
		cadena_pantalla[16],
		cadena_aux[16],
         seguir = 1, asignar = 1, mostrar = 1;
    char signed posicion;
    int h, punto = 0, k, pos_pesos = -1;
    double rta;
    posicion = 0;

	memset(cadena_aux,0,sizeof(cadena_aux));
	memset(cadena_pantalla,0,sizeof(cadena_pantalla));

    if( decimales && !PUNTO_DECIMAL_OBLIGATORIO ) {
        punto = 1;
        cadena[enteros] = '.';
	}
    INIT_PEDIR( x, y, enteros + decimales + punto );
    cadena[enteros + decimales + punto] = 0;

    while( seguir ) {
        if( mostrar ) {
			
			if(PUNTO_DECIMAL_OBLIGATORIO && USAR_PUNTO_EN_PANTALLA){
				LTRIM(cadena);
				TRIM(cadena);
				format_float( cadena_aux , cadena , decimales );
				memset( cadena_pantalla , ' ' , 15 );
				strcpy( &cadena_pantalla[ ( enteros + div(enteros,3).quot + 1*(enteros%3 != 0) ) - ((strlen(cadena_aux) > 4) ? strlen(cadena_aux) - decimales - 1 : 1) ] , cadena_aux );
			}else{
				strcpy(cadena_pantalla,cadena);
			}

            MOSTRAR_INPUT_CADENA( cadena_pantalla , posicion );
            mostrar = 0;
        };
        k = GETCH();
        switch( k ) {
            case 27:
                /* ESC */
                seguir = 0;
                asignar = 0;
                break;
            case '-':
                /* SIGNO */
                if( _ACEPTAR_SIGNO ) {
                    cadena[posicion++] = k;
                    if( posicion == enteros + decimales + punto ) {
                        posicion--;
                    }
                    if( posicion == enteros && punto ) {
                        posicion++;
                    }
                    mostrar = 1;
                }
                else {
                    seguir = 0;
                    // asignar=0;
                }
                break;
            case 13:
                /* ENTER */
            case '+':
            case '*':
			case -80:					  // flecha abajo
			case -72:					  // flecha abajo
			/*-Discrimina Submedios por Numero- Agrego Comando P_ATRAS Y ADELANTE */
			case -73:
			/*-Discrimina Submedios por Numero- Agrego Comando P_ATRAS Y ADELANTE */
			case -81:
			//-Tecla anulacion total.Discrimina Submedios por Numero
			case -62:
			seguir = 0;
			break;
                seguir = 0;
                break;
            case -400:
            case -401:
            case -402:
            case -403:
            case -404:
            case -405:
            case -406:
            case -407:
                SET_LASTKEY( 43 );                // +
                seguir = 0;
                pos_pesos = INDICE_PESOS( k );
                break;
            case 8:
                /* BACKSPACE */
				
				if( posicion == enteros + 1 && punto ) {
                    cadena[enteros] = '.';
                    posicion--;
                }
				
				if( posicion > 0 ){
					posicion--;
					cadena[posicion] = ' ';
				}
				                
				/*if( posicion < 0 ) {
                    posicion = 0;
                }*/

                if( posicion == enteros + 1 && punto ) {
                    cadena[enteros] = '.';
                    posicion--;
                }
                mostrar = 1;
                break;
            case '.':
                /* PUNTO DECIMAL */
                if( posicion < enteros && punto ) {
                    for( h = enteros - 1;h >= 0;h-- ) {
                        if( posicion > 0 ) {
                            cadena[h] = cadena[--posicion];
                        }
                        else {
                            cadena[h] = ' ';
                        }
                    }
                    posicion = enteros + 1;
                    mostrar = 1;
                }
                break;

            default:
                if( k >= '0' && k <= '9' ) {

                    /*cadena[posicion++] = k;
                    if( posicion == enteros + decimales + punto ) {
                        posicion--;
                    }*/
					if( posicion < (enteros + decimales + punto) ) {
                        cadena[posicion++] = k;
                    }
                    if( posicion == enteros && punto ) {
                        posicion++;
                    }
                    mostrar = 1;
                }
                break;
        }
    }
    if( asignar ) {
        if( VALOR_OBLIGATORIO() ) {
            char tmp_cad[] = {"               "};
            double tmp = 0.0;
            strcpy( tmp_cad, cadena );
            TRIM( tmp_cad );
            tmp = atof( tmp_cad );
            if( tmp == 0.0 ) {
                if( !strrchr( tmp_cad, '0' ) ) {
                    rta = -1;
                    asignar = 0;
                }
            }
        }
    }
    if( asignar ) {
        rta = atof( cadena );
        if( pos_pesos >= 0 ) {
            rta *= pesos[pos_pesos];
        }
        if( PUNTO_DECIMAL_OBLIGATORIO ) {
            for( h = 0;h < decimales;h++ ) {
                rta = rta / 10;
            }
        }
        MOSTRAR_FLOTANTE( x, y, enteros, decimales, _ACEPTAR_SIGNO, NO, rta );
    }
    else {
        rta = ( _ACEPTAR_SIGNO ) ? 0 : -1;
    }
    CLOSE_PEDIR();
    return ( rta );
}
/******************************************************************************/
double PEDIR_FLOTANTE_COD_BARRA( char x, char y, char enteros, char decimales, double _default,
                                 char **cod_barra )
/******************************************************************************/
{
    char cadena[] = {"               "},
		cadena_pantalla[16],
		cadena_aux[16],
         seguir = 1, asignar = 1, mostrar = 1;
    char signed posicion;
    int h, punto = 0, k, pos_pesos = -1;
    double rta;
    static char codigo_barra[100];
    if( cod_barra != NULL ) {
        *cod_barra = NULL;
        SET_ESPERA_GETCH( 2 );
    }
    if( _default != 0 ) {
        if( !PUNTO_DECIMAL_OBLIGATORIO ) {
            _snprintf(cadena, sizeof(cadena)-1, "%.2lf", _default );
        }
        else {
            char temp[32];
            char *p_cadena = cadena;
            char *p_temp = temp;
            _snprintf(temp, sizeof(temp)-1, "%.2lf", _default );
            for( ;*p_temp;p_temp++ ) {
                if( *p_temp != '.' ) {
                    *p_cadena = *p_temp;
                    p_cadena++;
                }
            }
        }
    }
    posicion = 0;
	
	memset(cadena_aux,0,sizeof(cadena_aux));
	memset(cadena_pantalla,0,sizeof(cadena_pantalla));

    if( decimales && !PUNTO_DECIMAL_OBLIGATORIO ) {
        punto = 1;
        cadena[( int )enteros] = '.';
    }
    INIT_PEDIR( x, y, enteros + decimales + punto );
    cadena[( int )enteros + decimales + punto] = 0;
    while( seguir ) {
        if( mostrar ) {

            if(PUNTO_DECIMAL_OBLIGATORIO && USAR_PUNTO_EN_PANTALLA){
				LTRIM(cadena);
				TRIM(cadena);
				format_float( cadena_aux , cadena , decimales );
				memset( cadena_pantalla , ' ' , 15 );
				strcpy( &cadena_pantalla[ ( enteros + div(enteros,3).quot + 1*(enteros%3 != 0) ) - ((strlen(cadena_aux) > 4) ? strlen(cadena_aux) - decimales - 1 : 1) ] , cadena_aux );
			}else{
				strcpy(cadena_pantalla,cadena);
			}

            MOSTRAR_INPUT_CADENA( cadena_pantalla , posicion );
            mostrar = 0;
        };
        k = GETCH();
        switch( k ) {
            case 27:
                /* ESC */
                seguir = 0;
                asignar = 0;
                break;
            case '-':
                /* SIGNO */
                if( _ACEPTAR_SIGNO ) {
                    cadena[posicion++] = k;
                    if( posicion == enteros + decimales + punto ) {
                        posicion--;
                    }
                    if( posicion == enteros && punto ) {
                        posicion++;
                    }
                    mostrar = 1;
                }
                else {
                    seguir = 0;
                    // asignar=0;
                }
                break;
            case 13:
                /* ENTER */
            case '+':
            case '*':
            case -80:
                // flecha abajo
            case -72:
                // flecha abajo
            case -73:
            case -81:
                seguir = 0;
                break;
            case -400:
            case -401:
            case -402:
            case -403:
            case -404:
            case -405:
            case -406:
            case -407:
                SET_LASTKEY( 43 );                // +
                seguir = 0;
                pos_pesos = INDICE_PESOS( k );
                break;
            case 8:
                /* BACKSPACE */
                cadena[posicion--] = ' ';
                if( posicion < 0 ) {
                    posicion = 0;
                }
                if( posicion == enteros && punto ) {
                    cadena[( int )enteros] = '.';
                    posicion--;
                }
                mostrar = 1;
                break;
            case '.':
                /* PUNTO DECIMAL */
                if( posicion < enteros && punto ) {
                    for( h = enteros - 1;h >= 0;h-- ) {
                        if( posicion > 0 ) {
                            cadena[h] = cadena[--posicion];
                        }
                        else {
                            cadena[h] = ' ';
                        }
                    }
                    posicion = enteros + 1;
                    mostrar = 1;
                }
                break;
            case -999:
                if( cod_barra != NULL ) {
                    if( IS_SCANNER() && LEER_SCANNER( (unsigned char *)codigo_barra, sizeof( codigo_barra ) ) ) {
						BEEP( 2000, 1000 );
                        *cod_barra = codigo_barra;
                        seguir = 0;
                    }
                }
                break;
            default:
                if( k >= '0' && k <= '9' ) {
                    cadena[posicion++] = k;
                    if( posicion == enteros + decimales + punto ) {
                        posicion--;
                    }
                    if( posicion == enteros && punto ) {
                        posicion++;
                    }
                    mostrar = 1;
                }
                break;
        }
    }
    if( asignar ) {
        rta = atof( cadena );
        if( pos_pesos >= 0 ) {
            rta *= pesos[pos_pesos];
        }
        if( PUNTO_DECIMAL_OBLIGATORIO ) {
            for( h = 0;h < decimales;h++ ) {
                rta = rta / 10;
            }
        }
        MOSTRAR_FLOTANTE( x, y, enteros, decimales, _ACEPTAR_SIGNO, NO, rta );
    }
    else {
        rta = ( _ACEPTAR_SIGNO ) ? 0 : -1;
    }
    CLOSE_PEDIR();
    SET_ESPERA_GETCH( 0 );
    return ( rta );
}
/******************************************************************************/
int INDICE_PESOS( int k )
/******************************************************************************/
{
    return ( -k - 400 );
}
/******************************************************************************/
double PEDIR_FLOTANTE_CON_SIGNO( char x, char y, char enteros, char decimales )
/******************************************************************************/
{
    double rta;
    char aux;
    aux = _ACEPTAR_SIGNO;
    _ACEPTAR_SIGNO = SI;
    rta = PEDIR_FLOTANTE( x, y, enteros, decimales );
    _ACEPTAR_SIGNO = aux;
    return ( rta );
}
double OBTENER_IMPORTE_T_COMPRA_DESDE_SCANNER( char *cadena_scanner, char cod_t_compra );
/******************************************************************************/
double PIDE_IMPORTE_T_COMPRA( char x, char y, char enteros, char decimales, char cod_t_compra,
                              char pos, char **output_cadena_scanner )
/******************************************************************************/
{
    char cadena[] = {"               "},
		cadena_pantalla[16],
		cadena_aux[16],
         seguir = 1, asignar = 1, mostrar = 1;
    static char cadena_scanner[30];
    char signed posicion;
    int h, punto = 0, k, pos_pesos = -1;
    double rta = 0;
    char leido_por_scanner = 0;
    if( output_cadena_scanner != NULL ) {
        *output_cadena_scanner = NULL;
    }
    memset( cadena_scanner, 0, sizeof( cadena_scanner ) );
    posicion = 0;

	memset(cadena_aux,0,sizeof(cadena_aux));
	memset(cadena_pantalla,0,sizeof(cadena_pantalla));

    INIT_PEDIR( x, y, enteros + decimales + punto );
    if( decimales && !PUNTO_DECIMAL_OBLIGATORIO ) {
        punto = 1;
        cadena[( int )enteros] = '.';
    }
    cadena[enteros + decimales + punto] = 0;
    SET_ESPERA_GETCH( 2 );
    while( seguir ) {
        if( mostrar ) {

            if(PUNTO_DECIMAL_OBLIGATORIO && USAR_PUNTO_EN_PANTALLA){
				LTRIM(cadena);
				TRIM(cadena);
				format_float( cadena_aux , cadena , decimales );
				memset( cadena_pantalla , ' ' , 15 );
				strcpy( &cadena_pantalla[ ( enteros + div(enteros,3).quot + 1*(enteros%3 != 0) ) - ((strlen(cadena_aux) > 4) ? strlen(cadena_aux) - decimales - 1 : 1) ] , cadena_aux );
			}else{
				strcpy(cadena_pantalla,cadena);
			}

            MOSTRAR_INPUT_CADENA( cadena_pantalla , posicion );

            mostrar = 0;
        };
        k = GETCH();
        switch( k ) {
            case 27:
                /* ESC */
                seguir = 0;
                asignar = 0;
                break;
            case '-':
                /* SIGNO */
                if( _ACEPTAR_SIGNO ) {
                    cadena[posicion++] = k;
                    if( posicion == enteros + decimales + punto ) {
                        posicion--;
                    }
                    if( posicion == enteros && punto ) {
                        posicion++;
                    }
                    mostrar = 1;
                }
                else {
                    seguir = 0;
                    // asignar=0;
                }
                break;
            case 13:
                /* ENTER */
            case '+':
            case '*':
            case -80:
                // flecha abajo
            case -72:
                // flecha abajo
                seguir = 0;
                break;
            case -400:
            case -401:
            case -402:
            case -403:
            case -404:
            case -405:
            case -406:
            case -407:
                SET_LASTKEY( 43 );                // +
                seguir = 0;
                pos_pesos = INDICE_PESOS( k );
                break;
            case 8:
                /* BACKSPACE */
				if( posicion == enteros + 1 && punto ) {
                    cadena[enteros] = '.';
                    posicion--;
                }
				
				if( posicion > 0 ){
					posicion--;
					cadena[posicion] = ' ';
				}

				/*if( posicion < 0 ) {
                    posicion = 0;
                }*/
                
				if( posicion == enteros + 1 && punto ) {
                    cadena[enteros] = '.';
                    posicion--;
                }
                mostrar = 1;
                break;
            case '.':
                /* PUNTO DECIMAL */
                if( posicion < enteros && punto ) {
                    for( h = enteros - 1;h >= 0;h-- ) {
                        if( posicion > 0 ) {
                            cadena[h] = cadena[--posicion];
                        }
                        else {
                            cadena[h] = ' ';
                        }
                    }
                    posicion = enteros + 1;
                    mostrar = 1;
                }
                break;
            case -999:
                /*------------- Sale por espera -------------------*/
                if( IS_SCANNER() ) {
                    CURSOR_OFF();
                    if( LEER_SCANNER( (unsigned char *)cadena_scanner, sizeof( cadena_scanner ) ) ) {
						BEEP( 2000, 1000 );
                        cadena_scanner[sizeof( cadena_scanner ) - 1] = 0;
                        rta = OBTENER_IMPORTE_T_COMPRA_DESDE_SCANNER( cadena_scanner,
                                                                      cod_t_compra );
                        if( rta ) {
                            SET_LASTKEY( '+' );
                            leido_por_scanner = 1;
                            asignar = 1;
                            seguir = 0;
                        }
                        else {
                            SET_LASTKEY( 13 );
                        }
                    }
                    else {
                        MENSAJE_STRING( S_ERROR_LECTURA_SCANNER );
                        memset( cadena_scanner, ' ', L_CODIGO );
                    }
                    CURSOR_ON();
                }
                break;
            default:
                if( k >= '0' && k <= '9' ) {

                    /*cadena[posicion++] = k;
                    if( posicion == enteros + decimales + punto ) {
                        posicion--;
                    }*/
					if( posicion < (enteros + decimales + punto) ) {
                        cadena[posicion++] = k;
                    }
                    if( posicion == enteros && punto ) {
                        posicion++;
                    }
					
                    mostrar = 1;
                }
                break;
        }
    }
    if( asignar ) {
        if( !leido_por_scanner ) {
            rta = atof( cadena );
            if( pos_pesos >= 0 ) {
                rta *= pesos[pos_pesos];
            }
            if( PUNTO_DECIMAL_OBLIGATORIO ) {
                for( h = 0;h < decimales;h++ ) {
                    rta = rta / 10;
                }
            }
        }
        else {
            if( output_cadena_scanner != NULL ) {
                *output_cadena_scanner = cadena_scanner;
            }
        }
        MOSTRAR_FLOTANTE( x, y, enteros, decimales, _ACEPTAR_SIGNO, NO, rta );
    }
    else {
        rta = ( _ACEPTAR_SIGNO ) ? 0 : -1;
    }
    CLOSE_PEDIR();
    SET_ESPERA_GETCH( 0 );
    return ( rta );
}
/******************************************************************************/
double PEDIR_FLOTANTE_CON_VALOR( int x, int y, int enteros, int decimales, double defecto )
/******************************************************************************/
{
    char cadena[] = {"               "},
		cadena_pantalla[16],
		cadena_aux[16],
         seguir = 1, asignar = 1, mostrar = 1;
    char signed posicion, borrar = 0;
    int h, punto = 0, k, pos_pesos = -1, signo = 0;
    double rta;

	if( defecto < 0 ) 
		signo = 1;
    if( defecto ) {
        FTOC( defecto, cadena, enteros, decimales, signo, 0 );
        borrar = 1;
		if( strstr( cadena, "." ) ) 
			//El parametro "defecto" viene con importe con punto de manera que 
			//corta cadena antes. "defecto" seteamos en 0 para que setee punto = 1 
			defecto	= 0;
    }
	
    posicion = 0;

	memset(cadena_aux,0,sizeof(cadena_aux));
	memset(cadena_pantalla,0,sizeof(cadena_pantalla));

    if( decimales && !PUNTO_DECIMAL_OBLIGATORIO && !defecto ) {
        punto = 1;
        cadena[enteros] = '.';
    }
    INIT_PEDIR( x, y, enteros + decimales + punto );
    cadena[enteros + decimales + punto] = 0;
    while( seguir ) {
        if( mostrar ) {

            if(PUNTO_DECIMAL_OBLIGATORIO && USAR_PUNTO_EN_PANTALLA){
				LTRIM(cadena);
				TRIM(cadena);
				format_float( cadena_aux , cadena , decimales );
				memset( cadena_pantalla , ' ' , 15 );
				strcpy( &cadena_pantalla[ ( enteros + div(enteros,3).quot + 1*(enteros%3 != 0) ) - ((strlen(cadena_aux) > 4) ? strlen(cadena_aux) - decimales - 1 : 1) ] , cadena_aux );
			}else{
				strcpy(cadena_pantalla,cadena);
			}

            MOSTRAR_INPUT_CADENA( cadena_pantalla , posicion );
            //mostrar = 0;
        };
        k = GETCH();
        switch( k ) {
            case 27:
                /* ESC */
                seguir = 0;
                asignar = 0;
                break;
            case '-':
                /* SIGNO */
                if( _ACEPTAR_SIGNO ) {
                    cadena[posicion++] = k;
                    if( posicion == enteros + decimales + punto ) {
                        posicion--;
                    }
                    if( posicion == enteros && punto ) {
                        posicion++;
                    }
                    mostrar = 1;
                }
                else {
                    seguir = 0;
                    //asignar=0;
                }
                break;
            case 13:
                /* ENTER */
            case '+':
            case '*':
			case -80:					  // flecha abajo
			case -72:					  // flecha abajo
			/*-Discrimina Submedios por Numero- Agrego Comando P_ATRAS Y ADELANTE */
			case -73:
			/*-Discrimina Submedios por Numero- Agrego Comando P_ATRAS Y ADELANTE */
			case -81:
			//-Tecla anulacion total.Discrimina Submedios por Numero
			case -62:
			seguir = 0;
                break;
            case -400:
            case -401:
            case -402:
            case -403:
            case -404:
            case -405:
            case -406:
            case -407:
                SET_LASTKEY( 43 );                // +
                seguir = 0;
                pos_pesos = INDICE_PESOS( k );
                break;
            case 8:
                /* BACKSPACE */
                cadena[posicion--] = ' ';
                if( posicion < 0 ) {
                    posicion = 0;
                }
                if( posicion == enteros && punto ) {
                    cadena[enteros] = '.';
                    posicion--;
                }
                mostrar = 1;
                break;
            case '.':
                /* PUNTO DECIMAL */
				if( posicion < enteros && punto ) {
                    for( h = enteros - 1;h >= 0;h-- ) {
                        if( posicion > 0 ) {
                            cadena[h] = cadena[--posicion];
                        }
                        else {
                            cadena[h] = ' ';
                        }
                    }
                    posicion = enteros + 1;
					cadena[enteros] = '.';
                    mostrar = 1;
                }
                break;
            default:
                if( k >= '0' && k <= '9' ) {
                    if( borrar ) {
                        borrar = 0;
                        memset( cadena, ' ', sizeof( cadena ) );
                        cadena[enteros + decimales + punto] = 0;
                    }
                    cadena[posicion++] = k;
                    if( posicion == enteros + decimales + punto ) {
                        posicion--;
                    }
                    if( posicion == enteros && punto ) {
                        posicion++;
                    }
                    mostrar = 1;
                }
                break;
        }
    }
    if( asignar ) {
        rta = atof( cadena );
        if( pos_pesos >= 0 ) {
            rta *= pesos[pos_pesos];
        }
        if( PUNTO_DECIMAL_OBLIGATORIO ) {
            for( h = 0;h < decimales;h++ ) {
                rta = rta / 10;
            }
        }
        MOSTRAR_FLOTANTE( x, y, enteros, decimales, _ACEPTAR_SIGNO, NO, rta );
    }
    else {
        rta = ( _ACEPTAR_SIGNO ) ? 0 : -1;
    }
    CLOSE_PEDIR();
    return ( rta );
}
/*****************************************************************************/
double OBTENER_FLOTANTE_DE_CADENA( char *buffer )
/*****************************************************************************/
{
    int len = strlen( buffer ), i;
    for( i = 0;i < len;i++ ) {
        if( buffer[i] == ',' ) {
            buffer[i] = '.';
        }
    }
    return( atof( buffer ) );
}

