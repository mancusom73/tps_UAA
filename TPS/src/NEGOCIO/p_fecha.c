#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <b_mens.h> 
#include <cadafech.h> 
#include <cr.h> 
#include <getch.h> 
#include <mensaje.h> 
#include <m_cadena.h> 
#include <pant.h> 
#include <pedir.h> 
#include <p_fecha.h> 
#include <tkt.h> 
#include <trim.h> 
#include <actions.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
static int dd, mm, yy;//Seba
char YY[5],MM[5],DD[5];//Seba
/******************************************************************************/
int VALIDA_FECHA( char *buffer, int ddmmaa )
/******************************************************************************/
{
    int dia,mes,anio;
    int ok = 1;

	if( ddmmaa ) {
    sscanf( buffer, "%d-%d-%d", &dia, &mes, &anio );
	} else {
		sscanf( buffer, "%d-%d", &mes, &anio );
	}
    if( ( anio < 0 ) || ( anio >= 100 ) ) {
        ok = 0;
    }
	if( ok && ( ( mes < 1 ) || ( mes > 12 ) ) ) {
        ok = 0;
    }

	if( ddmmaa && ok ) 
	{
		if( mes == 2 ) {
			if( anio % 4 ) {
				ok = ( dia > 0 ) && ( dia <= 28 );
			}
			else {
				ok = ( dia > 0 ) && ( dia <= 29 );
			}
		}
		else if( ( mes >= 1 ) && ( mes <= 7 ) ) {
			if( mes % 2 ) {
				ok = ( dia > 0 ) && ( dia <= 31 );
			}
			else {
				ok = ( dia > 0 ) && ( dia <= 30 );
			}
		}
		else if( ( mes > 7 ) && ( mes <= 12 ) ) {
			if( ( mes - 1 ) % 2 ) {
				ok = ( dia > 0 ) && ( dia <= 31 );
			}
			else {
				ok = ( dia > 0 ) && ( dia <= 30 );
			}
		}
	}

    return ok;
}
/******************************************************************************/
int PEDIR_FECHA( int x, int y, char *str )
/******************************************************************************/
{
    char cadena[9], seguir = 1, asignar = 1, mostrar = 1, separador;
    char signed posicion = 0;
    int k;
    //int rta = -1;
	int rta = 0;
    int cnt = 0;
    /*------------------- inicializa cadena ---------------------*/
    strcpy( cadena, "        " );
    separador = '-';
    if( str ) {
        str[0] = 0;
    }
    INIT_PEDIR( x, y, 8 );
    while( seguir ) {
        if( mostrar ) {
            MOSTRAR_INPUT_CADENA( cadena, posicion );
            mostrar = 0;
            if( !cnt ) {
                strcpy( cadena, "  -  -  " );
            }
            cnt = 1;
            #if defined(INVEL_W) || defined(INVEL_L)
            //MOSTRAR_INPUT_CADENA( cadena, posicion );
            ExecuteSAction( A_SET_INPUT_TEXT, cadena );
            #endif
        }        
        k = GETCH();
        switch( k ) {
            case 27:
                /* ESC */
                seguir = 0;
                asignar = 0;
                break;
            case 13:
                /* ENTER */
                seguir = !VALIDA_FECHA( cadena, SI );
                if( seguir ) {
                    MENSAJE( "FECHA INVALIDA" );
                    BORRAR_MENSAJE();
                    mostrar = 1;
                    strcpy( cadena, "  -  -  " );
                    posicion = 0;
                }
                break;
            case 8:
                /* BACKSPACE */
                cadena[posicion--] = ' ';
                if( posicion < 0 ) {
                    posicion = 0;
                }
                if( posicion == 2 || posicion == 5 ) {
                    cadena[posicion] = separador;
                    posicion--;
                }
                mostrar = 1;
                break;
            case -72:
                // Flecha Arriba
                seguir = 0;
                asignar = 0;
                rta = -3;
                break;
            default:
                if( k >= '0' && k <= '9' ) {
                    cadena[posicion++] = k;
                    if( posicion == 8 ) {
                        posicion--;
                    }
                    if( posicion == 2 || posicion == 5 ) {
                        posicion++;
                    }
                    mostrar = 1;
                }
                break;
        }
    }
    if( asignar ) {
        if( str ) {
            strncpy( str, cadena, 9 );
            TRIM( str );
        }
        rta = CADENA_A_FECHA( cadena );
    }
    CLOSE_PEDIR();
    return ( rta );
}
/******************************************************************************/
int PEDIR_FECHA_MMAA( int x, int y )
/******************************************************************************/
{
    char cadena[6], seguir = 1, asignar = 1, mostrar = 1, separador, aux[9] = "10-";
    char signed posicion = 0;
    int k;
    unsigned rta;
    /*------------------- inicializa cadena ---------------------*/
    if( _GRANDE ) {
        strcpy( cadena, "  -  " );
        separador = '-';
    }
    else {
        strcpy( cadena, "  /  " );
        separador = '/';
    }
    INIT_PEDIR( x, y, 5 );
    while( seguir ) {
        if( mostrar ) {
            ExecuteSAction( A_SET_INPUT_TEXT, cadena );
            //MOSTRAR_INPUT_CADENA( cadena, posicion );
            mostrar = 0;
        }
        k = GETCH();
        switch( k ) {
            case 27:
                /* ESC */
                seguir = 0;
                asignar = 0;
                break;
            case 13:
                /* ENTER */
                seguir = !VALIDA_FECHA( cadena, NO );
                if( seguir ) {
                    MENSAJE( "FECHA INVALIDA" );
                    BORRAR_MENSAJE();
                    mostrar = 1;
                    if( _GRANDE ) {
						strcpy( cadena, "  -  " );
						separador = '-';
					}
					else {
						strcpy( cadena, "  /  " );
						separador = '/';
					}
                    posicion = 0;
                }
                break;
            case 8:
                /* BACKSPACE */
                cadena[posicion--] = ' ';
                if( posicion < 0 ) {
                    posicion = 0;
                }
                if( posicion == 2 ) {
                    cadena[posicion] = separador;
                    posicion--;
                }
                mostrar = 1;
                break;
            case -72:
                // Flecha Arriba
                seguir = 0;
                asignar = 0;
                rta = -3;
                break;
            default:
                if( k >= '0' && k <= '9' ) {
                    cadena[posicion++] = k;
                    if( posicion == 5 ) {
                        posicion--;
                    }
                    if( posicion == 2 ) {
                        posicion++;
                    }
                    mostrar = 1;
                }
                break;
        }
    }
    strcpy( &aux[3], cadena );
	sscanf(aux, "%d-%d-%d", &dd, &mm, &yy );//Seba
    rta = ( asignar ) ? CADENA_A_FECHA( aux ) : -1;
    CLOSE_PEDIR();
    return ( rta );
}
/******************************************************************************/
long PEDIR_HORA_LARGA( int x, int y, char *str )
/******************************************************************************/
{
    char cadena[9], seguir = 1, asignar = 1, mostrar = 1, separador;
    char signed posicion = 0;
    int k;
    int rta = -1;
    int cnt = 0;
    //------------------- inicializa cadena ---------------------
    strcpy( cadena, "        " );
	separador = ':';
    
    if( str ) {
        str[0] = 0;
    }
    INIT_PEDIR( x, y, 8 );
    while( seguir ) {
        if( mostrar ) {
            MOSTRAR_INPUT_CADENA( cadena, posicion );
            mostrar = 0;
            if( !cnt ) {
				strcpy( cadena, "  :  :  " );
            }
            cnt = 1;
            #if defined(INVEL_W) || defined(INVEL_L)
            //MOSTRAR_INPUT_CADENA( cadena, posicion );
            ExecuteSAction( A_SET_INPUT_TEXT, cadena );
            #endif
        }        
        k = GETCH();
        switch( k ) {
            case 27:
                // ESC 
                seguir = 0;
                asignar = 0;
                break;
            case 13:
                // ENTER
                seguir = ( char ) !VALIDA_HORA_LARGA( cadena );
                if( seguir ) {
                    MENSAJE( "HORA INVALIDA" );
                    BORRAR_MENSAJE(  );
                    mostrar = 1;
					strcpy( cadena, "  :  :  " );
                    posicion = 0;
                }
                break;
            case 8:
                // BACKSPACE 
                cadena[posicion--] = ' ';
                if( posicion < 0 ) {
                    posicion = 0;
                }
                if( posicion == 2 || posicion == 5 ) {
                    cadena[posicion] = separador;
                    posicion--;
                }
                mostrar = 1;
                break;
            case -72:
                // Flecha Arriba
                seguir = 0;
                asignar = 0;
                rta = -3;
                break;
            default:
                if( k >= '0' && k <= '9' ) {
                    cadena[posicion++] = k;
                    if( posicion == 8 ) {
                        posicion--;
                    }
                    if( posicion == 2 || posicion == 5 ) {
                        posicion++;
                    }
                    mostrar = 1;
                }
                break;
        }
    }
    if( asignar ) {
        if( str ) {
            strncpy( str, cadena, 9 );
            TRIM( str );
        }
        rta = CADENA_A_HORA( cadena );
    }
    CLOSE_PEDIR(  );
    return ( rta );
}
/******************************************************************************/
long CADENA_A_HORA( char *cadena )
/******************************************************************************/
{
    char  cadena2[20];
    long  hora = 0, minutos = 0, segundos = 0;
    strcpy( cadena2, cadena );
    if( strlen( cadena ) < 8 ) {
        hora = 0;
    }
    else {
        cadena2[2] = 0;
        cadena2[5] = 0;
        hora = atoi( cadena2 );
        minutos = atoi( cadena2 + 3 );
        segundos = atoi( cadena2 + 6 );
        hora = ( hora * 10000 ) + ( minutos * 100 ) + segundos;
    }
    return ( hora );
}
/******************************************************************************/
int VALIDA_HORA_LARGA( char *buffer )
/******************************************************************************/
{
    long  hora = 0, minutos = 0, segundos = 0;
    int   ok = 1;
    
	sscanf( buffer, "%d:%d:%d", &hora, &minutos, &segundos );
    if( ( hora < 0 ) || ( hora > 23 ) ) {
        ok = 0;
    }
    if( ( minutos < 0 ) || ( minutos > 59 ) ) {
        ok = 0;
    }
	if( ( segundos < 0 ) || ( segundos > 59 ) ) {
        ok = 0;
    }
    return ok;
}
/******************************************************************************/
void OBTIENE_FECHA_VENCIMIENTO(char *fecha)//Seba
/******************************************************************************/
{

	memset(YY,0,strlen(YY));
	memset(MM,0,strlen(MM));
	memset(DD,0,strlen(DD));
	_snprintf(YY,sizeof(YY)-1,"%d",yy);
	_snprintf(MM,sizeof(MM)-1,"%d",mm);
	_snprintf(DD,sizeof(DD)-1,"%d",dd);
	
	if(yy<10)
		{
			memset(YY,0,strlen(YY));		
			_snprintf(YY,sizeof(YY)-1,"0%d",(int)yy);
		}
	if(mm<10)
		{
			memset(MM,0,strlen(MM));
			_snprintf(MM,sizeof(MM)-1,"0%d",(int)mm);
		}
	if(dd<10)
		{
			memset(DD,0,strlen(DD));
			_snprintf(DD,sizeof(DD)-1,"0%d",(int)dd);
	}

	sprintf(fecha,"%s%s%s",YY,MM,DD);
	//sprintf(fecha,"%s%s%s","09","11","02");

}
