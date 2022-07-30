#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cmemory.h> 
#include <cr.h> 
#include <ini.h>
#include <ctime.h> 
#include <dbrouter.h> 
#include <driver.h> 
#include <getch.h> 
#include <mstring.h> 
#include <omron.h> 
#include <pausa.h> 
#include <pinpad1.h> 
#include <status.h> 
#include <tar_mag.h> 
#include <val.h> 
#include <val2.h> 
#include <actions.h>
#include <log.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if !defined(INVEL_W) && !defined(INVEL_L)
#endif
#if defined(INVEL_W) || defined(INVEL_L)
#endif
/*----------------------------- Defines ------------------------------*/
#if !defined(INVEL_W) && !defined(INVEL_L)
#define _TECLA_COMIENZO ';'
#define _TECLA_FIN      13
#define _TECLA_DEFAULT  0xf000
#define _ESPERA_TICK    8
#endif
#ifndef COMPILA_TRACKS_TECLADOS
	#define _TECLA_COMIENZO_T2 ';'
	
#else
	//HMM este para mi teclado kb que no esta bien configurado
	#define _TECLA_COMIENZO_T2 ']'
#endif COMPILA_TRACKS_TECLADOS


#define _TECLA_COMIENZO_T2e 241//teclado español 
#define _TECLA_FIN_T2		13
#define _TECLA_FIN_T2e		'_' //'_'//teclado español
#ifndef COMPILA_TRACKS_TECLADOS
	#define _TECLA_COMIENZO_T1 '%'
	
#else
	//este para mi teclado kb 
	#define _TECLA_COMIENZO_T1 '['
#endif COMPILA_TRACKS_TECLADOS

#ifndef COMPILA_TRACKS_TECLADOS
	#define _TECLA_FIN_T1      '?'
#else
//truchada 
	#define _TECLA_FIN_T1		13
#endif COMPILA_TRACKS_TECLADOS

#define _TECLA_FIN_T1e      '_'//teclado español 
#define _TECLA_DEFAULT  0xf000
#define _ESPERA_TICK    8
#define _TECLA_FIN_T2A  13
#define _TECLA_FIN_T2B  '?'
#define _TECLA_FIN_T2Be  '_'
#define _TECLA_COMIENZO_T1C ';'
#ifndef COMPILA_TRACKS_TECLADOS
	#define _TECLA_COMIENZO_T2C '%'
#else
	//este para mi teclado kb 
	#define _TECLA_COMIENZO_T2C ']'
#endif COMPILA_TRACKS_TECLADOS
#define _TECLA_FIN_TC       13
/*------------------------ variables estaticas -----------------------*/
//static char buf_tarjeta[51];
static int capturar = 0, tarjeta_leida=0;
static long tick1=0 , tick2 = 0;
char buf_track1[81],buf_track2[51],track=0;
#define SIZEOF_BUFF_MSR 256
char buff_msr[SIZEOF_BUFF_MSR];
#if defined(INVEL_W) || defined(INVEL_L)
static int ind_buffer_t1=0,ind_buffer_t2=0;
static char buf_tarjeta_track1[81],buf_tarjeta_track2[51];
int getData( int action, char *buffer, int len );
//int       InitMsr(  );
#endif
int CONTROLAR_CARACTERES( int teclas );
/*****************************************************************************/
int INIT_TARJETA_MAGNETICA()
/*****************************************************************************/
{
    int error = 0;
    if( TIPO_LECTOR_BANDA == LECTOR_BANDA_DRIVER ) {
        #if defined(INVEL_W) || defined(INVEL_L)
        //error = InitMsr(  );
        #else
        DRIVER( _DRV_lector_banda, _LBM_inicializacion, 0, &error, sizeof( error ) );
        #endif
    }
    return ( !error );
}
/*****************************************************************************/
void LEER_TARJETA_MAGNETICA_OMRON( unsigned char *track2, unsigned char *track1 )
/*****************************************************************************/
{
    memcpy( track2, &buf_track2[1], buf_track2[0] );
    memset( track1, 0, 80 );
}
/*****************************************************************************/
int LEER_TARJETA_MAGNETICA( unsigned char *track2, unsigned char *track1 )
/*****************************************************************************/
{
    int st, rta = 1, h;
    #if defined(INVEL_W) || defined(INVEL_L)
    int       longitud = 0;
    #else
    struct _param
    {
        int rta;
        char *track1;
        char *track2;
        int l_track1;
        int l_track2;
    };
    struct _param param;
    #endif
    memset( track2, 0, 50 );
    memset( track1, 0, 80 );
    memset( buf_track2, 0, 50 );
    memset( buf_track1, 0, 80 );
    switch( TIPO_LECTOR_BANDA ) {
        case LECTOR_BANDA_OMRON:
            #ifdef COMPILAR_OMRON
            rta = OMRON_TARJETA_MAGNETICA( buf_track2 );
            if( rta ) {
                memcpy( track2, &buf_track2[1], buf_track2[0] );
            }
            #endif
            break;
        case LECTOR_BANDA_INVEL:
            switch( HARD_INVEL ) {
                case VERSION_1:
                    st = EJECUTAR_COMANDO( 0xF8, 0, 0, 0, 0 );
                    if( BUFFER[0] == '0' || st ) {
                        rta = 0;
                    }
                    if( rta && BUFFER[0] <= 50 ) {
                        for( h = 1;h < BUFFER[0];h++ ) {
                            BUFFER[h] += '0';
                        }
                        memcpy( track2, &BUFFER[1], BUFFER[0] );
                        memcpy( buf_track2, BUFFER, 50 );
                    }
                    else {
                        rta = 0;
                    }
                    break;
                case VERSION_2:
                    st = VAL_LEER_BANDA_TRACK2( (char *)BUFFER );
                    if( st > 0 && st <= 50 ) {
                        memcpy( track2, BUFFER, st );
                        memcpy( &buf_track2[1], BUFFER, 49 );
                        buf_track2[0] = st + 1;
                    }
                    else {
                        rta = 0;
                    }
                    if( rta ) {
                        st = VAL_LEER_BANDA_TRACK1( (char *)BUFFER );
                        if( st > 0 && st <= 80 ) {
                            memcpy( track1, BUFFER, st );
                            memcpy( &buf_track1[1], BUFFER, 79 );
                            buf_track1[0] = st;
                        }
                        else {
                            MENSAJE_STRING( S_EL_NOMBRE_NO_FUE_LEIDO );
                        }
                    }
                    break;
            }
            break;
        case LECTOR_BANDA_TECLADO:
            if( tarjeta_leida ) {
                #if defined(INVEL_W) || defined(INVEL_L)
                memcpy( track1, buf_tarjeta_track1, ind_buffer_t1 );
                memcpy( track2, buf_tarjeta_track2, ind_buffer_t2 );
                memcpy( &buf_track1[1], buf_tarjeta_track1, ind_buffer_t1 );
                memcpy( &buf_track2[1], buf_tarjeta_track2, ind_buffer_t2 );
				//truchada ojo no pasar Solo para simular carga de track1
/*				if(ind_buffer_t1< 200){
					int tecla, i=0;
			//		memcpy( track1, "5275728697807033^ÁÉCUSO HECTRO M          ^1402101010266906370803972", 79 );
					memcpy( buf_tarjeta_track1, "5465535312170576ÂLTAMIRANO ALEXIS J       ^2012101100716382393808119",79);
					//simulemos que viene mal
				//	memcpy( buf_tarjeta_track1, "A5275728697807033^ÁÉÍÓÚO HECTRO M          ^1402101010266906370803972", 80 );//cop 79 sin el primer caracter
					for( i = 0; i < 23; i++){
						tecla= CONTROLAR_CARACTERES(buf_tarjeta_track1[i]);
						buf_tarjeta_track1[i] = tecla;
					}
					//
					memcpy( &buf_track1[1], buf_tarjeta_track1, ind_buffer_t1 );
					ind_buffer_t1 = 79;
				}
*/
                buf_track2[0] = ind_buffer_t2 + 1;
                buf_track1[0] = ind_buffer_t1;
                #else
                memcpy( track2, buf_tarjeta, ind_buffer );
                //MENSAJE_TECLA(track2);
                memcpy( &buf_track2[1], buf_tarjeta, ind_buffer );
                buf_track2[0] = ind_buffer;
                #endif
                tarjeta_leida = 0;
            }
            break;
        case LECTOR_BANDA_DRIVER:
        default:
            {
                char strTmp[200];
            rta = getData( ACTION_READ_DATA_MSR_TRACK1, (char *)track1, 80 );
                _snprintf(strTmp,200,"TRACK1: %s",track1);
                strTmp[199] = 0;
                GRABAR_LOG_SISTEMA(strTmp,LOG_VENTAS,4);
            rta = getData( ACTION_READ_DATA_MSR_TRACK2, (char *)track2, 50 );
                _snprintf(strTmp,200,"TRACK2: %s",track2);
                strTmp[199] = 0;
                GRABAR_LOG_SISTEMA(strTmp,LOG_VENTAS,4);
                track1[79] = '\0';
                track2[49] = '\0';
			if( rta ) {
                int       l1 = 0, l2 = 0;
                l1 = ( char ) strlen( (const char *)track1 );
                l2 = ( char ) strlen( (const char *)track2 );
                if( l2 ) {
                        longitud = ( l2 < 49 ) ? l2 : 48;
                        memcpy( &buf_track2[1], track2, longitud );
                        buf_track2[0] = ( char )longitud + 1;
                }
                if( l1 ) {
					    longitud = ( l1 < 79 ) ? l1 : 78;
					    memcpy( &buf_track1[1], track1, longitud );
					    buf_track1[0] = ( char )longitud + 1;
				}
            }
            }
            break;
    }

    return ( rta );
}


/******************************************************************************/
int LECTOR_BANDA( int tecla )
/******************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    int k = 0;

	if( _PERMITIR_BANDA && TIPO_LECTOR_BANDA == LECTOR_BANDA_TECLADO && tecla != -999 ) {
		
		if( capturar ) {
            if( tecla == ( ( TIPO_TECLADO == 1 ) ? -999 : _TECLA_DEFAULT ) ) {
                tick2 = GET_TIME();
                if( tick2 - tick1 > _ESPERA_TICK ) {
                    if( track == 1 && ind_buffer_t1 == 0 ) {
                        tecla = ( TIPO_TECLADO == TECLADO_CARY ) ? _TECLA_COMIENZO_T1C
                              : _TECLA_COMIENZO_T1;
                    }
                    else if( track == 2 && ind_buffer_t2 == 0 ) {
                        tecla = ( TIPO_TECLADO == TECLADO_CARY ) ? _TECLA_COMIENZO_T2C
                              : _TECLA_COMIENZO_T2;
                    }
                    capturar = 0;
                }
			}else {
				switch( track ) {
                    case 1:
                        // Track 1
						if( ( TIPO_TECLADO == TECLADO_CARY && tecla == _TECLA_FIN_TC ) ||
							( tecla == _TECLA_FIN_T1 ||tecla == _TECLA_FIN_T1e ) )  {
                            capturar = 0;

							if( RAM_PEDIR_CLIENTE_AFINIDAD == 2  ) {
								tarjeta_leida |= 2; //lareina tarjeta no tiene track2
								memset( buf_tarjeta_track2,0, sizeof(buf_tarjeta_track2) );
								 buf_tarjeta_track2[0] = '\0';
							} else
							tarjeta_leida |= 1;
							
                        }
                        else {
                            if( ind_buffer_t1 < 80 ) {
								//Controla de caracteres Á É Í Ó Ú 
								tecla= CONTROLAR_CARACTERES(tecla);
                                buf_tarjeta_track1[ind_buffer_t1++] = tecla;
                            }
                            else {
                                capturar = 0;
                            }
                        }
                        break;
                    case 2:
                        // Track 2
                        if( (track==2)&&
							( tecla == _TECLA_FIN_T2 || 
							tecla == _TECLA_FIN_T2e ||
							tecla == _TECLA_FIN_T2B /*hmm*/ )||
							( tecla == _TECLA_FIN_TC && TIPO_TECLADO == TECLADO_CARY )) {
                            capturar = 0;
							if( RAM_PEDIR_CLIENTE_AFINIDAD == 2  ) {

								buf_tarjeta_track1[0] = '\0';
								 memset( buf_tarjeta_track1,0, sizeof(buf_tarjeta_track1) );
							} 
							tarjeta_leida |= 2;
							//ver si este getch trae problemas..estaba en la parte conmentada
							//k = GETCH();
						}else{
                            if( ind_buffer_t2 < 50 ) {
                                buf_tarjeta_track2[ind_buffer_t2++] = tecla;
                            }
                            else {
                                capturar = 0;
                            }
                        }
                        break;
                }
                tick1 = GET_TIME();
                tecla = ( TIPO_TECLADO == 1 ) ? -999 : _TECLA_DEFAULT;
			}
		}else {//fin capturar!!!
			//verifica si tiene que comenzar a capturar!!!
			
			if( tecla == _TECLA_COMIENZO_T1 ||
				tecla == _TECLA_COMIENZO_T2||
				tecla == _TECLA_COMIENZO_T2e||
				( TIPO_TECLADO == TECLADO_CARY &&
				( tecla == _TECLA_COMIENZO_T1C || tecla == _TECLA_COMIENZO_T2C ) ) ) {
					capturar = 1;
					tick1 = GET_TIME();
					if( tecla== ( ( TIPO_TECLADO == TECLADO_CARY ) ? _TECLA_COMIENZO_T1C
						: _TECLA_COMIENZO_T1 ) ) {
							ind_buffer_t1 = 0;
							track = 1;
							memset( buf_tarjeta_track1,0, sizeof(buf_tarjeta_track1) );
					}else {
						ind_buffer_t2 = 0;
						track = 2;
						memset( buf_tarjeta_track2,0, sizeof(buf_tarjeta_track2) );
					}
					tecla = ( TIPO_TECLADO == 1 ) ? -999 : _TECLA_DEFAULT;
			}

			
        }
		/*se debe verificar si tiene que seguir leyendo o se termino la tarjeta, 
		sino se realiza esto la identificación por banda no funciona correctamente 
		salvo hacer una funcion tipo la de F7 en cr1.*/

		if(tecla != 27 && !(tecla == _TECLA_DEFAULT ||tecla == -999) && track > 0 && tarjeta_leida )
			tecla = ( TIPO_TECLADO == 1 ) ? -999 : _TECLA_DEFAULT;
		
		


    }//---- fin del lector de banda
	
    #else
    if( TIPO_LECTOR_BANDA == LECTOR_BANDA_TECLADO ) {
        if( capturar ) {
            if( tecla == ( ( TIPO_TECLADO == 1 ) ? -999 : _TECLA_DEFAULT ) ) {
                tick2 = GET_TIME();
                if( tick2 - tick1 > _ESPERA_TICK ) {
                    if( ind_buffer == 0 ) {
                        tecla = _TECLA_COMIENZO;
                    }
                    capturar = 0;
                }
            }
            else {
                if( tecla == _TECLA_FIN ) {
                    tarjeta_leida = 1;
                    capturar = 0;
                }
                else {
                    if( ind_buffer < 50 ) {
                        buf_tarjeta[ind_buffer++] = tecla;
                    }
                    else {
                        capturar = 0;
                    }
                    tick1 = GET_TIME();
                }
                tecla = ( TIPO_TECLADO == 1 ) ? -999 : _TECLA_DEFAULT;
            }
        }
        else {
            if( tecla == _TECLA_COMIENZO ) {
                tecla = ( TIPO_TECLADO == 1 ) ? -999 : _TECLA_DEFAULT;
                ind_buffer = 0;
                capturar = 1;
                tick1 = GET_TIME();
            }
        }
    }
    #endif
	
    return ( tecla );
}
/******************************************************************************/
int LECTOR_TARJETA_LEIDA()
/******************************************************************************/
{
    int rta = 0;
    #ifdef COMPILAR_OMRON
    char buffer[50],buffer2[80];
    #endif
    switch( TIPO_LECTOR_BANDA ) {
        case LECTOR_BANDA_OMRON:
            #ifdef COMPILAR_OMRON
            if( LEER_TARJETA_MAGNETICA( buffer, buffer2 ) ) {
                rta = 1;
            }
            #endif
            break;
        case LECTOR_BANDA_INVEL:
            switch( HARD_INVEL ) {
                case VERSION_1:
                    if( STATUS() & 0x04 ) {
                        rta = 1;
                    }
                    break;
                case VERSION_2:
                    if( VAL_STATUS() & _DATOS_TRACK2 ) {
                        rta = 1;
                    }
                    break;
            }
            break;
        case LECTOR_BANDA_TECLADO:
            #if defined(INVEL_W) || defined(INVEL_L)
            rta = ( tarjeta_leida & 2 ) ? 1 : 0;
            #else
            rta = tarjeta_leida;
            #endif
            break;
        case LECTOR_BANDA_DRIVER:
            #if defined(INVEL_W) || defined(INVEL_L)
        default:
            rta = getData( ACTION_IS_MSR, NULL, 0 );
            /*Marcos: Lectura del scanner*/
            #else
            DRIVER( _DRV_lector_banda, _LBM_status, 0, &rta, sizeof( rta ) );
            #endif
            break;
    }
    return ( rta );
}
/****************************************************************/
void LIMPIA_INDICADOR_TARJETA_LEIDA( void )
/****************************************************************/
{tarjeta_leida=0;}
/*****************************************************************************/
void LIMPIA_BUFFER_TARJETA( int cerear )
/*****************************************************************************/
{
    char buffer[50],buffer2[80];
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    #ifdef COMPILAR_OMRON
    int i;
    #endif
    int st;
    #endif
    switch( TIPO_LECTOR_BANDA ) {
        case LECTOR_BANDA_OMRON:
            #if !defined(INVEL_W) && !defined(INVEL_L)
            #ifdef COMPILAR_OMRON
            for( i = 0;i < 4;i++ ) {
                OMRON_TARJETA_MAGNETICA( buffer );
            }
            #endif
            #endif
            break;
        case LECTOR_BANDA_INVEL:
            #if !defined(INVEL_W) && !defined(INVEL_L)
            switch( HARD_INVEL ) {
                case VERSION_1:
                    if( STATUS() & 0x04 ) {
                        PAUSA( 4 );
                        LEER_TARJETA_MAGNETICA( buffer, buffer2 );
                    }
                    break;
                case VERSION_2:
                    st = VAL_STATUS();
                    if( ( st & _DATOS_TRACK2 ) || ( st & _DATOS_TRACK1 ) ) {
                        LEER_TARJETA_MAGNETICA( buffer, buffer2 );
                    }
                    break;
            }
            #endif
            break;
        case LECTOR_BANDA_DRIVER:
            if( LECTOR_TARJETA_LEIDA() ) {
                LEER_TARJETA_MAGNETICA( (unsigned char *)buffer, (unsigned char *)buffer2 );
            }
            break;	
    }
    if( cerear ) {
        CEREAR_MEMORY( __var_tarjetas_track2 );
        CEREAR_MEMORY( __var_tarjetas_track1 );
        
    }
	memset( buf_tarjeta_track2,0, sizeof(buf_tarjeta_track2) );
	memset( buf_tarjeta_track1,0, sizeof(buf_tarjeta_track1) );
	tarjeta_leida = 0;
	VACIAR_KEY();
}
/*****************************************************************************/
int LEER_TARJETA_BANDA( unsigned char *buffer, unsigned char *buffer2 )
/*****************************************************************************/
{
    int ok = 0;
    switch( TIPO_LECTOR_BANDA ) {
        case LECTOR_BANDA_OMRON:
            #ifdef COMPILAR_OMRON
            LEER_TARJETA_MAGNETICA_OMRON( buffer, buffer2 );
            ok = 1;
            #endif
            break;
        case LECTOR_BANDA_INVEL:
        case LECTOR_BANDA_TECLADO:
        case LECTOR_BANDA_DRIVER:
            PAUSA( 4 );
            ok = LEER_TARJETA_MAGNETICA( buffer, buffer2 );
            if( !ok ) {
                MENSAJE_STRING( S_ERROR_LEER_TARJETA );
                /*** a veces queda en el pinpad ***/
                ok = LEER_TARJETA_MAGNETICA( buffer, buffer2 );
            }
            break;
    }
    VACIAR_KEY();
	return ( ok );
}
/*****************************************************************************/
void CARGA_TRACKS_EN_RNV()
/*****************************************************************************/
{
	SET_TRACK_1_PRIVADO( buf_track1 );
    SET_TRACK_2_PRIVADO( buf_track2 );
}

/*****************************************************************************/
int STRLEN( char *men, int cant )
/*****************************************************************************/
{
    int j;
    for( j = 0;men[j] && j < cant;j++ ) {
        ;
    }
    return j;
}
/*****************************************************************************/
int CONTROLAR_CARACTERES( int teclas )
/*****************************************************************************/
{
    //LAS ACENTUADAS 
	int teclaux = teclas;
	switch( teclas ) { //-63 teclas vocales acentuadas
		case -63: 
			 teclas=65;
			 break;
		case -55:
			 teclas=69;
			 break;
		case -51:
			 teclas=73;
			 break;
		case -45:
			 teclas=79;
			 break;
		case -38: //
			 teclas=85;
			 break;
		case 193: //??? y estes?
			 teclas=65;
			 break;
		case 224: // o mayuscula acentuada??
			 teclas=79;
			 break;
		case -31: //cualquiera a minuscula
			 teclas=97;
			 break;
		case -23: //e minuscula acentudad
			 teclas=101;
			 break;
		case -19:  //i acentudada
			 teclas=105;
			 break;
		case -13: //o acentuada 
			 teclas=111;
			 break;
		case -6: //u acentuada
			 teclas=117;
			 break;
		case 243:
			 teclas=111;
			 break;
		case 218: 
			 teclas=85;
			 break;
		case 250:
			 teclas=117;
			 break;
	
	}
	if(teclaux != teclas){ 
		char tec[35];
		sprintf(tec,"Teclas Distintas Entra %i Sale %c",teclaux , teclas);
		glog(tec,1,1);
	}
    return teclas;
}

