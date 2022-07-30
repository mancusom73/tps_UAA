#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <btrv.h> 
#include <b_mens.h> 
#include <clave.h> 
#include <cmemory.h> 
#include <cr.h>  
#include <dbrouter.h> 
#include <eventos.h> 
#include <getch.h> 
#include <llave.h> 
#include <log.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <pant.h> 
#include <pausa.h> 
#include <print.h> 
#include <p_clave.h> 
#include <status.h> 
#include <stringt.h> 
#include <tar_mag.h> 
#include <tkt.h> 
#include <val.h> 
#include <val2.h> 
#include <_cr1.h> 
#include <_cr0.h> 
#include <db.h>
#include <deftables.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef INVEL_L
#include <sys/io.h>
#else
#include <io.h>
#endif
#include <memory.h>
#include <string.h>
#include <aarch.h>
#include <_val.h>
#include <btrv.h>
#include <string.h>
#if defined(INVEL_W) || defined(INVEL_L)
#include <actions.h>
#endif
#define CUALQUIERA    0
#define CAJERO        1
#define SUPERVISOR    2
#define ENTRENAM      3
#define PCOS          0
#define GP416         1
#define CONNAC        2
#define CL_TARJETA    0
#define CL_1903       1
#define CL_MANUAL     2
#define CL_SIN_CLAVE  3
static unsigned _clave_anterior,_ultimo_usuario,_tipo_usuario;
static char _clave_anterior_gp[9], banda_supervisor;
char dato__cajero[80];
int pos_x_cajero;
long LEER_NUMERO_USUARIO_GENERICO( int longitud_a_tomar,char * buffer2 );
/*****************************************************************************/
int PIDE_SUPERVISOR( int x, int y, int tipo, char *titulo )
/*****************************************************************************/
{
    int sup = 999, l_nro = 4, ant_scanner=_PERMITIR_SCANNER, ant_banda=_PERMITIR_BANDA;
	char *barra_usuario=NULL;

    if( MULTI_SUPERVISOR ) {
        MOSTRAR_CADENA( x + 2, y + 1, ST( S_N__SUPERVISOR_ ) );
        
		if( MODO_IDENTIFICACION_SUPERVISOR == IDENTIFICACION_BANDA 
			|| MODO_IDENTIFICACION_GERENTE == IDENTIFICACION_BANDA ) {
				_PERMITIR_BANDA = SI;
				LIMPIA_BUFFER_TARJETA( NO );
		}
		
		if( MODO_IDENTIFICACION_SUPERVISOR == IDENTIFICACION_BARRA 
			|| MODO_IDENTIFICACION_GERENTE == IDENTIFICACION_BARRA ) {
				_PERMITIR_SCANNER=SI;
		}

        //l_nro = ( USAR_LEGAJO == LEGAJO ) ? CANTIDAD_DE_DIGITOS_EN_NRO_LEGAJ : 4;
		/*********************** definimos el largo del campo que usamos par buscar ********/
		if( USAR_LEGAJO == RUT ) {
			//l_nro = 9;
			l_nro = 8;
		} else {
			l_nro = (USAR_LEGAJO == LEGAJO) ? CANTIDAD_DE_DIGITOS_EN_NRO_LEGAJ : 4;
		}
        
		/***********************************************************************************/
        // original ---> sup = ( int )PEDIR_ENTERO( x + 16, y + 1, l_nro, NO );

		sup= (int )PEDIR_ENTERO_COD_BARRA( x + 16, y + 1, l_nro, 0, &barra_usuario, NO, titulo, "" );
		
		if( sup == -1)//ESC en el input
			return ( 0 );
       	
		_PERMITIR_BANDA = NO;
		_PERMITIR_SCANNER = NO;
       
		
		/*     BANDA      */
		if( MODO_IDENTIFICACION_SUPERVISOR == IDENTIFICACION_BANDA 
                || MODO_IDENTIFICACION_GERENTE == IDENTIFICACION_BANDA ) {
            if( sup == -2 ) {
               sup = ( int )LEER_NUMERO_USUARIO(l_nro);
            }else {
                sup = 0;
            }
        }
		/*     BARRA      */
		if( MODO_IDENTIFICACION_SUPERVISOR == IDENTIFICACION_BARRA
                || MODO_IDENTIFICACION_GERENTE == IDENTIFICACION_BARRA ) {
            if( sup == -3 ||(sup == 0 && barra_usuario!=NULL && strlen(barra_usuario)>0)) {
               sup = ( int )LEER_NUMERO_USUARIO_GENERICO( l_nro, barra_usuario );
            }else {
                sup = 0;
            }
        }

        if (USAR_LEGAJO == RUT) {
			sup = BUSCA_CAJERO_X_DNI( sup );
		} else if( USAR_LEGAJO == LEGAJO ) {
            sup = BUSCA_CAJERO_X_LEGAJO( sup );
        }
        else {
            sup = BUSCA_CAJERO_X_CODIGO( sup );
        }
        LEE_CLAVE_CAJERO( sup, NULL );
        if( sup > 0 ) {
            if( tipo == SUPERVISOR && !( ES_SUPERVISOR( sup ) || ES_GERENTE( sup ) ) ) {
                //MENSAJE_STRING( S_NO_NUMERO_SUPERVISOR );
				MENSAJE_ID_CON_PAUSA_CAJA( S_NO_NUMERO_SUPERVISOR );
                BORRAR_MENSAJE();
		        sup = 0;
            } else {
		        if( tipo == GERENTE && !ES_GERENTE( sup ) ) {
		            //MENSAJE("NO ES NUMERO DE GERENTE");
					MENSAJE_ID_CON_PAUSA_CAJA( S_NO_NUMERO_GERENTE );
                    BORRAR_MENSAJE();
		            sup = 0;
		        }
            }
        } else {
             if( tipo == SUPERVISOR ) {
				//MENSAJE_STRING( S_NO_NUMERO_SUPERVISOR );
				MENSAJE_ID_CON_PAUSA_CAJA( S_NO_NUMERO_SUPERVISOR );
				BORRAR_MENSAJE();
				sup = 0;
            } else {
		        if( tipo == GERENTE ) {
		            //MENSAJE("NO ES NUMERO DE GERENTE");
					MENSAJE_ID_CON_PAUSA_CAJA( S_NO_NUMERO_GERENTE );
                    BORRAR_MENSAJE();
		            sup = 0;
		        }
            }
        }
    }

    return ( sup );
}
/*****************************************************************************/
long LEER_NUMERO_USUARIO( int longitud_a_tomar )
/*****************************************************************************/
{
    char buffer2[50],buffer1[80],*p;
    int l;
    long usuario = -1;




	if( LEER_TARJETA_BANDA( (unsigned char *)buffer2, buffer1 ) ) {
		 VACIAR_KEY();
        l = strlen( (const char *)PREFIJO_TARJETA_USUARIO );
        if( l < 1 || l > 10 ) {
            l = 10;
        }
		//para que lo cpia primero para controlar despues?¿?¿?¿?
        //memcpy( buffer2, PREFIJO_TARJETA_USUARIO, l );
        p = PREFIJO_TARJETA_USUARIO;
        if( memcmp( buffer2, p, l ) == 0 ) {
			
			
            p = memchr( &buffer2[l], '=', 50 - l );
            if( p ) {
                *p = 0;
            }
			if( longitud_a_tomar > 0 && longitud_a_tomar < 9 ){
				buffer2[l+longitud_a_tomar]=0;//si es configurable se debe tomar cierta cantidad de digitos.
			}else
				buffer2[l+9]=0;//el maximo long es MAXLONG; 2147483647
			usuario = atol( &buffer2[l] );
        }
        else {
            MENSAJE_STRING( S_NO_TARJETA_USUARIO );
        }
		if( usuario ) {
			 banda_supervisor = 1;
		}
    }
    else {
        MENSAJE_STRING( S_ERROR_LEER_TARJETA );
    }
    return ( usuario );
}
/*****************************************************************************/
int INGRESAR_CLAVE_MANUAL( int x, int y, int tipo, int cajero, int sup )
/*****************************************************************************/
{
    int rta = 0, clave;
    /*--------------------------- clave manual ----------------------------*/
    //MENSAJE_SIN_SONIDO_STRING( S_INGRESE_CLAVE );
	MENSAJE_SIN_SONIDO(ST(S_INGRESE_CLAVE),NO );
    clave = PEDIR_CLAVE( x, y, tipo );
    if( ( TIPO_CLAVE_SUPERVISOR == CL_MANUAL && tipo == SUPERVISOR ) 
            || ( TIPO_CLAVE_CAJERO == CL_MANUAL && tipo == CAJERO ) 
            || ( TIPO_CLAVE_GERENTE == CL_MANUAL && tipo == GERENTE ) ) {
         rta = VERIFICAR_CLAVE_EN_ARCHIVO( clave, tipo, cajero, sup );
   	} else {
        if( clave == 1903 ) {
            rta = 1;
            _ultimo_usuario = cajero;
        }
    }
    return ( rta );
}
#ifdef COMPILAR_PCOS
/*****************************************************************************/
static int INGRESAR_CLAVE_PCOS( int x, int y, int tipo )
/*****************************************************************************/
{
    int rta = 0;
    int clave, salir = 0, nro, seguir = 1;
    unsigned char buffer[8] = {"00000000"};
    /*------------------- espera tarjeta o ESC ------------------------------*/
    SET_ESPERA_GETCH( 1 );
    while( !salir ) {
        if( GETCH() == 27 ) {
            salir = 1;
        }
        if( STATUS() & 0x02 ) {
            salir = 2;
        }
    }
    if( salir == 2 ) {
        /*------------------- resetea la tarjeta ---------------------------*/
        if( GP_RESET() && GP_RESET() ) {
            MENSAJE_STRING( S_NO_DETECTO_TARJETA );
        }
        else {
            /*------------------ pide la clave ------------------------------*/
            MENSAJE_SIN_SONIDO_STRING( S_INGRESE_CLAVE );
            clave = PEDIR_CLAVE( x, y, tipo );
            if( clave >= 0 ) {
                /*-------------------- verifica clave ------------------------*/
                buffer[0] = ( clave & 0xf000 ) >> 12;
                buffer[2] = ( clave & 0x0f00 ) >> 8;
                buffer[4] = ( clave & 0x00f0 ) >> 4;
                buffer[6] = clave & 0x000f;
                nro = tipo + 2;
                if( GP_CHECK_SECRET_CODE( nro, buffer ) ) {
                    MENSAJE_STRING( S_CLAVE_INCORRECTA_RETIRE );
                }
                else {
                    memcpy( _clave_anterior_gp, buffer, 8 );
                    _clave_anterior = clave;
                    MENSAJE_STRING( S_OK_RETIRE );
                    rta = 1;
                }
            }
            else {
                MENSAJE_STRING( S_RETIRE_TARJETA );
            }
        }
        GP_EJECT();
        while( seguir ) {
            seguir = STATUS() & 0x02;
            if( kbhit() ) {
                if( GETCH() == 27 ) {
                    seguir = 0;
                }
            }
        }
    }
    return ( rta );
}
#endif
#ifdef COMPILAR_GP416
/*****************************************************************************/
static int INGRESAR_CLAVE_GP416( int x, int y, int tipo, int cajero, int sup )
/*****************************************************************************/
{
    int rta = 0;
    int clave, salir = 0, seguir = 1, usuario, cl, error, rta1, rta2;
    unsigned char buffer[8] = {"00000000"},
                  byte;
    if( HARD_INVEL == VERSION_1 ) {
        /*----------------- espera tarjeta o ESC ------------------------------*/
        SET_ESPERA_GETCH( 1 );
        while( !salir ) {
            if( GETCH() == 27 ) {
                salir = 1;
            }
            if( STATUS() & 0x02 ) {
                salir = 2;
            }
        }
        if( salir == 2 ) {
            /*------------------- resetea la tarjeta ---------------------------*/
            PAUSA( 2 );
            if( !GP416_DETECT() ) {
                MENSAJE_STRING( S_NO_DETECTO_TARJETA_RETIRE );
            }
            else {
                buffer[0] = 0xaa;
                buffer[1] = 0xaa;
                if( GP416_PRESENT_KEY( buffer ) ) {
                    /*------------- verifica n de usuario --------------------*/
                    PAUSA( 4 );
                    if( GP416_READ( 3, 2, buffer ) == 0 ) {
                        byte = buffer[0] & 0xf0;
                        usuario = ( byte >> 4 ) * 1000 + ( buffer[0] & 0x0f ) * 100;
                        byte = buffer[1] & 0xf0;
                        usuario += ( byte >> 4 ) * 10 + ( buffer[1] & 0x0f );
                        if( !cajero ) {
                            cajero = ( tipo == SUPERVISOR ) ? usuario : NRO_CAJERO;
                        }
                        if( tipo == SUPERVISOR && usuario < config.primer_supervisor ) {
                            MENSAJE_STRING( S_NO_TARJETA_SUP_RETIRE );
                        }
                        else {
                            /*------------------ pide la clave ------------------------------*/
                            MENSAJE_SIN_SONIDO( S_INGRESE_CLAVE,NO );
                            clave = PEDIR_CLAVE( x, y, tipo );
                            if( clave >= 0 ) {
                                if( usuario == cajero ) {
                                    PAUSA( 4 );
                                    if( GP416_READ( 20, 2, &cl ) == 0 ) {
                                        if( cl == clave ) {
                                            if( VERIFICAR_CLAVE_EN_ARCHIVO( clave, tipo, cajero,
                                                                            sup ) ) {
                                                _clave_anterior = clave;
                                                MENSAJE_STRING( S_OK_RETIRE );
                                                rta = 1;
                                                _ultimo_usuario = cajero;
                                            }
                                        }
                                        else {
                                            MENSAJE_STRING( S_CLAVE_INCORRECTA_RETIRE );
                                        }
                                    }
                                    else {
                                        MENSAJE_STRING( S_ERROR_LEER_TARJETA );
                                    }
                                }
                                else {
                                    MENSAJE_STRING( S_TARJETA_INCORRECTA );
                                }
                            }
                        }
                    }
                    else {
                        MENSAJE_STRING( S_ERROR_LEER_TARJETA );
                    }
                }
                else {
                    MENSAJE_STRING( S_ERROR_TARJETA_BLOQUEADA );
                }
            }
        }
        else {
            MENSAJE_STRING( S_RETIRE_TARJETA );
        }
        GP416_SACA_TENSION();
        while( seguir ) {
            seguir = STATUS() & 0x02;
            if( kbhit() ) {
                if( GETCH() == 27 ) {
                    seguir = 0;
                }
            }
        }
    }
    else if( HARD_INVEL == VERSION_2 ) {
        /*----------------- espera tarjeta o ESC ------------------------------*/
        SET_ESPERA_GETCH( 1 );
        while( !salir ) {
            if( GETCH() == 27 ) {
                salir = 1;
            }
            if( VAL_STATUS() & _TARJETA_INGRESADA ) {
                salir = 2;
            }
        }
        if( salir == 2 ) {
            /*------------------- resetea la tarjeta ---------------------------*/
            VAL_SELECT_CARD( _GMP416 );
            VAL_POWER_ON();
            buffer[0] = 0xaa;
            buffer[1] = 0xaa;
            error = VAL_COD_SECRETO_TARJETA( buffer, ( char* )&rta1, ( char* )&rta2 );
            if( !error ) {
                /*------------- verifica n de usuario --------------------*/
                error = VAL_LEER_TARJETA( 3, 2, buffer, ( char* )&rta1, ( char* )&rta2 );
                if( !error ) {
                    byte = buffer[0] & 0xf0;
                    usuario = ( byte >> 4 ) * 1000 + ( buffer[0] & 0x0f ) * 100;
                    byte = buffer[1] & 0xf0;
                    usuario += ( byte >> 4 ) * 10 + ( buffer[1] & 0x0f );
                    if( !cajero ) {
                        cajero = ( tipo == SUPERVISOR ) ? usuario : NRO_CAJERO;
                    }
                    if( tipo == SUPERVISOR && usuario < config.primer_supervisor ) {
                        MENSAJE_STRING( S_NO_TARJETA_SUP_RETIRE );
                    }
                    else {
                        /*------------------ pide la clave ------------------------------*/
                        MENSAJE_SIN_SONIDO_STRING( S_INGRESE_CLAVE );
                        clave = PEDIR_CLAVE( x, y, tipo );
                        if( clave >= 0 ) {
                            if( usuario == cajero ) {
                                error = VAL_LEER_TARJETA( 20, 2, ( char* )&cl, ( char* )&rta1,
                                                          ( char* )&rta2 );
                                if( !error ) {
                                    if( cl == clave ) {
                                        if( VERIFICAR_CLAVE_EN_ARCHIVO( clave, tipo, cajero, sup ) ) {
                                            _clave_anterior = clave;
                                            MENSAJE_STRING( S_OK_RETIRE );
                                            rta = 1;
                                            _ultimo_usuario = cajero;
                                        }
                                    }
                                    else {
                                        MENSAJE_STRING( S_CLAVE_INCORRECTA_RETIRE );
                                    }
                                }
                                else {
                                    MENSAJE_STRING( S_ERROR_LEER_TARJETA );
                                }
                            }
                            else {
                                MENSAJE_STRING( S_TARJETA_INCORRECTA );
                            }
                        }
                    }
                }
                else {
                    MENSAJE_STRING( S_ERROR_LEER_TARJETA );
                }
            }
            else {
                MENSAJE_STRING( S_ERROR_TARJETA_BLOQUEADA );
            }
        }
        else {
            MENSAJE_STRING( S_RETIRE_TARJETA );
        }
        VAL_POWER_OFF();
        while( seguir ) {
            seguir = VAL_STATUS() & _TARJETA_INGRESADA;
            if( kbhit() ) {
                if( GETCH() == 27 ) {
                    seguir = 0;
                }
            }
        }
    }
    return ( rta );
}
#endif
/*****************************************************************************/
void CODIFICA_CLAVE( char *buffer, char *codigo, int extendida )
/*****************************************************************************/
{
    unsigned char byte;
    unsigned int b_alto,b_bajo;
    int       i, k, largo_clave;

    largo_clave = ( extendida ) ? 8 : 4;
    for( i = 0, k = 1; i < largo_clave; i++, k++ ) {
		if( k == 8 )
			k = 1;
        byte = buffer[i];
        b_alto = byte << ( 8 - k );
        b_bajo = byte >> k;
        codigo[i] = ( ( b_alto | b_bajo ) & 0xFF );
    }
}
/*****************************************************************************/
int LEE_CLAVE_CAJERO( int nro, char *buffer )
/*****************************************************************************/
{
    int rta = 0;
   	memset(datos_cajero.nombre,0,26);
	if( nro )
		if( /*USE_DB( AREA_CAJEROS, _CAJEROS_SIC, ( char* )&datos_cajero, sizeof( datos_cajero ), NULL, 0,
                0 )*/
		OPEN_TABLE(T_CAJEROS, TT_ORIG ,( char* )&datos_cajero, sizeof( struct _cajeros ))== 0 ) {
        //GET_EQUAL( ( char* )&nro );
		//SELECT_TABLE( T_CAJEROS, TT_ORIG );
		SET_WHERE("COD_CAJERO = %i",nro);
		RUN_QUERY(1);
        if( FOUND2() ) {
            if( buffer ) {
				if( CLAVE_EXTENDIDA( datos_cajero.tipo ) ) {
                    memcpy( buffer, &datos_cajero.clave_extendida, 8 );
                } else {
				    memcpy( buffer, &datos_cajero.clave, 4 );
                }
            }
            rta = 1;
        }
        else {
            rta = 2;
        }
        CLOSE_TABLE( T_CAJEROS, TT_ORIG );
    }
    else {
        MENSAJE_STRING( S_ERROR_ARCHIVO_CAJEROS );
    }
    return ( rta );
}
/*****************************************************************************/
int BUSCA_CAJERO_X_LEGAJO( long legajo )
/*****************************************************************************/
{
    int rta = 0;
	memset(datos_cajero.nombre,0,26);
    if( /*USE_DB( AREA_CAJEROS, _CAJEROS_SIC, ( char* )&datos_cajero, sizeof( datos_cajero ), NULL, 0,
                0 )*/
		OPEN_TABLE(T_CAJEROS, TT_ORIG ,( char* )&datos_cajero, sizeof( struct _cajeros ))== 0 ) {
        //SET_ORDER( 2 );
		SELECT_TABLE( T_CAJEROS, TT_ORIG );
		SET_WHERE("LEGAJO = %ld",legajo);
        //GET_EQUAL( ( char* )&legajo );
		RUN_QUERY(1);
        if( FOUND2() ) {
            rta = datos_cajero.cod_cajero;
        }
        CLOSE_TABLE( T_CAJEROS, TT_ORIG );
    }
    else {
        MENSAJE_STRING( S_ERROR_ARCHIVO_CAJEROS );
    }
    return ( rta );
}
/*****************************************************************************/
int CLAVE_ANTERIOR()
/*****************************************************************************/
{
    return ( _clave_anterior );
}
/*****************************************************************************/
int TIPO_USUARIO()
/*****************************************************************************/
{
    return ( _tipo_usuario );
}
/*****************************************************************************/
void CLAVE_ANTERIOR_GP( char *buffer )
/*****************************************************************************/
{
    memcpy( buffer, _clave_anterior_gp, 8 );
}
/*****************************************************************************/
int USUARIO_CLAVE_ANTERIOR(void)
/*****************************************************************************/
{
    return ( _ultimo_usuario );
}
/*****************************************************************************/
int SOLICITAR_CLAVES( int x, int y, int cajero, int supervisor, int condicion, char *titulo,
                      char *log, int funcion, int grabar_en_event )
/*****************************************************************************/
{// sino tiene evento grabado para el supervisor usar GRABA_EVENTO_SALIDA_CAJERO(  ); para registrar el evento
    char buffer_journal[100]=" ";
    int ok = 1;
    char log_aux[100];
    SALVA_CURSOR();
    if( condicion ) {
        if( cajero ) {
            ok = 0;
            if( INGRESAR_CLAVE( x + 2, y, ST( S_CAJERO___ ), 1, 0, titulo, funcion, NO,
                                grabar_en_event ) ) {
                ok = 1;
            }
            // Imprimir en Journal
            if( ok ) {
                PRN_SELECT( JOURNAL );
                _snprintf( buffer_journal, sizeof(buffer_journal)-1, ST( S_CAJERO___D ), USUARIO_CLAVE_ANTERIOR() );
                MOSTRAR_CADENA( 1, PRN, buffer_journal );
                LF( 1 );
                PRN_HABILITA_AVANCES();
            }
        }

        if( ok && ( supervisor == SUPERVISOR || supervisor == 1 ) ) {
            ok = 0;
            if( INGRESAR_CLAVE( x, y, ST( S_SUPERVISOR___ ), 2, 0, titulo, funcion, NO, grabar_en_event ) ) {
                if( ESPERAR_LLAVE( 'S' ) ) {
                    ok = 1;
                }
                if( ok ) {
                    PRN_INHABILITA_AVANCE_RECEIPT();
                    if( log != NULL ) {
                        log_aux[0] = /*'\n'*/'}';
                        strcpy( &log_aux[1], log );
						if( strcmp(titulo, ST( S_BOLETIN_VENCIDO_ ))  && titulo != NULL ) {
                            _snprintf( buffer_journal, sizeof(buffer_journal)-1,  ST( S_SUPERVISOR___D__S__S ), USUARIO_CLAVE_ANTERIOR(), titulo,log_aux);
                        }else {
                            _snprintf( buffer_journal, sizeof(buffer_journal)-1,  ST( S_SUPERVISOR___D__S ), USUARIO_CLAVE_ANTERIOR(), log_aux );
                        }
                    }
                    else {
                        if( titulo != NULL ) {
                            _snprintf( buffer_journal, sizeof(buffer_journal)-1,  ST( S_SUPERVISOR___D__S ), USUARIO_CLAVE_ANTERIOR(), titulo );
                        }
                        else {
                            _snprintf( buffer_journal, sizeof(buffer_journal)-1,  ST( S_SUPERVISOR___D ), USUARIO_CLAVE_ANTERIOR() );
                        }
                    }
                    #if defined(INVEL_W) || defined(INVEL_L)
                    if( IMPRIMIR_EN_JOURNAL ) {
                        MOSTRAR_CADENA( 1, PRN, buffer_journal );
                    }
                    #else
                    MOSTRAR_CADENA( 1, PRN, buffer_journal );
                    #endif   
                    LF( 1 );
                    PRN_HABILITA_AVANCES();
                }
            }
        }
        if( ok && supervisor == GERENTE ) {
            ok = 0;
			if( INGRESAR_CLAVE( x, y, "Gerente : ", GERENTE, 0, titulo , RAM_COD_FUNCION, NO, grabar_en_event ) ) {
				ok = 1;
			}
		}
        if( ok && log ) {
            GRABAR_LOG_SUPERVISOR( log, USUARIO_CLAVE_ANTERIOR() ,LOG_VENTAS,2);
        }
    }
    RESTAURA_CURSOR();
    return ( ok );
}
/*****************************************************************************/
int VERIFICAR_CLAVE_EN_ARCHIVO( int clave, int tipo, int cajero, int sup )
/*****************************************************************************/
{
    int rta = 0, extendida = 0, distintos = 1;
    char      clave_cod[9]="", clave_cajero[9]="";
   
    /*Clave extendida*/
	extendida = CLAVE_EXTENDIDA( tipo );
	memset( clave_cod, 0, sizeof(clave_cod) );

    /*------ por disco -------*/
    _itoa( clave, clave_cod, 10 );
    CODIFICA_CLAVE( clave_cod, clave_cod, extendida );
    if( !cajero ) {
        cajero = ( tipo == SUPERVISOR || tipo == GERENTE ) ? sup : NRO_CAJERO;
    }
    switch( LEE_CLAVE_CAJERO( cajero, clave_cajero ) ) {
        case 0:
            /*--- Error ---*/
            MENSAJE_STRING( S_ERROR_LEER_CLAVE );
            break;
        case 1:
            /*--- Normal ----*/
            distintos = strncmp( clave_cajero, clave_cod, extendida ? 8 : 4 );
		    if( !distintos ) {
			    rta = 1;
			    _ultimo_usuario = cajero;
		    } else {
                MENSAJE_STRING( S_CLAVE_INCORRECTA );
            }
            break;
        case 2:
            /*--- Cajero no habilitado ---*/
            MENSAJE_STRING( S_CAJERO_NO_HABILITADO );
            break;
    }
    return ( rta );
}
/*****************************************************************************/
int LEER_TARJETA_CONNAC( int x, int y, int tipo, int usuario_esperado )
/*****************************************************************************/
{
    int rta = 0, usuario, clave;
    if( INGRESA_GP416( x, y ) ) {
        if( PREPARA_GP416() ) {
            usuario = ( int )OBTENER_USUARIO( usuario_esperado );
            if( usuario ) {
                tipo = OBTENER_TIPO( tipo );
            }
            if( usuario && tipo >= 0 ) {
                MENSAJE_SIN_SONIDO_STRING( S_INGRESE_CLAVE );
                clave = PEDIR_CLAVE( x, y, tipo );
                if( OBTENER_CLAVE( clave ) >= 0 ) {
                    if( VERIFICAR_CLAVE_EN_ARCHIVO( clave, tipo, usuario, 0 ) ) {
                        MENSAJE_STRING( S_OK_RETIRE );
                        rta = 1;
                        _clave_anterior = clave;
                        _ultimo_usuario = usuario;
                        _tipo_usuario = tipo;
                    }
                }
            }
        }
    }
    else {
        MENSAJE_STRING( S_RETIRE_TARJETA );
    }
    SACA_GP416();
    return ( rta );
}
/*****************************************************************************/
int BCD_A_INT( unsigned char *buffer )
/*****************************************************************************/
{
    int destino;
    destino = BCD_A_CHAR( buffer[0] ) * 100 + BCD_A_CHAR( buffer[1] );
    return ( destino );
}
/*****************************************************************************/
long BCD_A_NRO_USUARIO( unsigned char *buffer )
/*****************************************************************************/
{
    long destino;
    destino = BCD_A_CHAR( buffer[0] ) * 10000L + BCD_A_CHAR( buffer[1] ) * 100L
            + BCD_A_CHAR( buffer[2] );
    return ( destino );
}
/*****************************************************************************/
int BCD_A_CHAR( unsigned char buffer )
/*****************************************************************************/
{
    int destino;
    destino = ( ( buffer & 0xf0 ) >> 4 ) * 10 + ( buffer & 0x0f );
    return ( destino );
}
/*****************************************************************************/
int PREPARA_GP416()
/*****************************************************************************/
{
    int ok = 1;
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    int rta1,rta2;
    unsigned char buffer[2];
    VAL_SELECT_CARD( _GMP416 );
    VAL_POWER_ON();
    buffer[0] = 0xaa;
    buffer[1] = 0xaa;
    if( VAL_COD_SECRETO_TARJETA( buffer, ( char* )&rta1, ( char* )&rta2 ) ) {
        MENSAJE_STRING( S_ERROR_TARJETA_BLOQUEADA );
        ok = 0;
    }
    #endif
    return ( ok );
}
/*****************************************************************************/
int INGRESA_GP416()
/*****************************************************************************/
{
    int ok = 0;
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    int salir = 0;
    MENSAJE_SIN_SONIDO_STRING( S_INGRESE_TARJ_IDENTIFICACION );
    /*----------------- espera tarjeta o ESC ------------------------------*/
    SET_ESPERA_GETCH( 1 );
    while( !salir ) {
        if( GETCH() == 27 ) {
            salir = 1;
        }
        if( VAL_STATUS() & _TARJETA_INGRESADA ) {
            salir = 2;
        }
    }
    if( salir == 2 ) {
        ok = 1;
    }
    #endif
    return ( ok );
}
/*****************************************************************************/
void SACA_GP416()
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    int seguir = 1;
    VAL_POWER_OFF();
    while( seguir ) {
        seguir = VAL_STATUS() & _TARJETA_INGRESADA;
        if( kbhit() ) {
            if( GETCH() == 27 ) {
                seguir = 0;
            }
        }
    }
    #endif
}
/*****************************************************************************/
long OBTENER_USUARIO( long usuario_esperado )
/*****************************************************************************/
{
    long usuario = 0;
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    int rta1, rta2, ok = 0;
    unsigned char buffer[3];
    if( VAL_LEER_TARJETA( 2, 3, buffer, ( char* )&rta1, ( char* )&rta2 ) ) {
        MENSAJE_STRING( S_ERROR_LEER_TARJETA );
    }
    else {
        usuario = BCD_A_NRO_USUARIO( buffer );
        BUSCA_CAJERO_X_LEGAJO( usuario );
        if( usuario == datos_cajero.nro_legajo ) {
            ok = 1;
        }
        usuario = datos_cajero.cod_cajero;
        if( usuario_esperado && usuario != usuario_esperado ) {
            ok = 0;
        }
        if( usuario_esperado && !ok ) {
            MENSAJE_STRING( S_USUARIO_INCORRECTO_RETIRE );
            usuario = 0;
        }
    }
    #endif
    return ( usuario );
}
/*****************************************************************************/
int OBTENER_TIPO( int tipo_esperado )
/*****************************************************************************/
{
    int tipo = -1;
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    int rta1,rta2;
    unsigned char buffer;
    if( VAL_LEER_TARJETA( 5, 1, ( char* )&buffer, ( char* )&rta1, ( char* )&rta2 ) ) {
        MENSAJE_STRING( S_ERROR_LEER_TARJETA );
    }
    else {
        tipo = BCD_A_CHAR( buffer );
        if( tipo_esperado && tipo != tipo_esperado ) {
            MENSAJE_STRING( S_TARJETA_INCORRECTA_RETIRE );
            tipo = -1;
        }
    }
    #endif
    return ( tipo );
}
/*****************************************************************************/
int OBTENER_CLAVE( int clave )
/*****************************************************************************/
{
    int cl = -1;
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    int rta1,rta2;
    unsigned char buffer[2];
    if( VAL_LEER_TARJETA( 16, 2, buffer, ( char* )&rta1, ( char* )&rta2 ) ) {
        MENSAJE_STRING( S_ERROR_LEER_TARJETA );
    }
    else {
        cl = BCD_A_INT( buffer );
        if( cl != clave ) {
            MENSAJE_STRING( S_CLAVE_INCORRECTA_RETIRE );
            cl = -1;
        }
    }
    #endif
    return ( cl );
}
/*****************************************************************************/
int USUARIO_PUEDE_RETIRAR( int nro )
/*****************************************************************************/
{
    int rta = 0;
	memset(datos_cajero.nombre,0,26);
    if( /*USE_DB( AREA_CAJEROS, _CAJEROS_SIC, ( char* )&datos_cajero, sizeof( datos_cajero ), NULL, 0,
                0 )*/
		OPEN_TABLE(T_CAJEROS, TT_ORIG ,( char* )&datos_cajero, sizeof( struct _cajeros ))== 0 ) {
        //GET_EQUAL( ( char* )&nro );
		SELECT_TABLE( T_CAJEROS, TT_ORIG );
		SET_WHERE("COD_CAJERO = %i",nro);
		RUN_QUERY(1);
        if( FOUND2() ) {
            if( datos_cajero.puede_retirar == 'S' ) {
                rta = 1;
            }
        }
        CLOSE_TABLE( T_CAJEROS, TT_ORIG );
    }
    else {
        MENSAJE_STRING( S_ERROR_ARCHIVO_CAJEROS );
    }
    return ( rta );
}
/*****************************************************************************/
int INGRESAR_CLAVE( int x, int y, char *mensaje, int tipo, int cajero, char *titulo, int funcion,
                    int pant_inicial, int grabar_en_event )
/*****************************************************************************/
{
    int rta = 0, sup = 0, manual = 0, l, lbox, xbox, dif = 0, lt = 0;
    long supl = 0;
    unsigned  pedir_clave = 1;
	char tit[60];
    
	
	memset(tit,0, sizeof(tit));
    l = strlen( titulo );
	strncpy(tit, titulo, l);
	tit[l] = '\0';
    if( ( TIPO_CLAVE_CAJERO == CL_SIN_CLAVE && tipo == CAJERO ) ) {
        return ( 1 );
    }
    /*------------------------- Calcula coordenadas -------------------------*/
    l = strlen( mensaje );
	if( CLAVE_EXTENDIDA( tipo ) ) {
        lbox = l + 11;
    } else {
   	    lbox = l + 7;
    }
    if( titulo ) {
        lt = strlen( titulo );
    }
    if( lt > lbox - 2 ) {
        dif = lt - lbox + 2;
    }
    lbox += dif;
    xbox = x - dif / 2;
    x += l + 2;
    y++;
    manual = ( /*( TIPO_CLAVE_VENDEDOR && tipo == VENDEDOR ) ||*/ ( TIPO_CLAVE_CAJERO
			&& tipo == CAJERO ) || ( TIPO_CLAVE_SUPERVISOR && tipo == SUPERVISOR )
		|| ( TIPO_CLAVE_GERENTE && tipo == GERENTE ) ) ? 1 : 0;
    /*-------------------- pide supervisor si es necesario ------------------*/
    if( manual && tipo != ENTRENAM ) {
        /*---------------- Pide nro de supervisor ---------------*/
		if( !cajero && ( tipo == SUPERVISOR || tipo == GERENTE ) ) {

			if( !( SCANNER_HABILITADO_POR_TECLADO && 
				( MODO_IDENTIFICACION_SUPERVISOR == IDENTIFICACION_BARRA 
				|| MODO_IDENTIFICACION_GERENTE == IDENTIFICACION_BARRA ) ) ) {
            #if defined(INVEL_W) || defined(INVEL_L)
            ExecuteSSAction( ( pant_inicial ) ? A_PEDIR_CL_PRES : A_PEDIR, 1, titulo, mensaje,
                             NULL, " " );
            #endif
			}

            if( USAR_LEGAJO == RUT ) {
                supl = PIDE_SUPERVISOR_LONG( x - l - 6, y - 1, tipo );
                sup = BUSCA_CAJERO_X_DNI( supl );
                if( !sup ) {
				    pedir_clave = 0;
				    rta = 0;
			    }
            }
            else {
                sup = PIDE_SUPERVISOR( x - l - 2, y - 1, tipo, titulo );
            }

			if( !( SCANNER_HABILITADO_POR_TECLADO && 
				( MODO_IDENTIFICACION_SUPERVISOR == IDENTIFICACION_BARRA 
					|| MODO_IDENTIFICACION_GERENTE == IDENTIFICACION_BARRA ) ) ) {
				ExecuteAction( A_FIN_PEDIR );
			}

            if( !sup ) {
                return ( 0 );
            }
        }
        if( tipo == SUPERVISOR && TIPO_CLAVE_SUPERVISOR == CL_SIN_CLAVE ) {
            banda_supervisor = 0;
			_ultimo_usuario = sup;
			pedir_clave = 0;
			rta = 1;
        }
        if( ( tipo == GERENTE && TIPO_CLAVE_GERENTE == CL_SIN_CLAVE ) ) {
			_ultimo_usuario = sup;
            return ( 1 );
        }
    }
    /*------------------------ Muestra el mensaje ---------------------------*/
    if( pedir_clave ) {
		MOSTRAR_CADENA( x - l, y, mensaje );
		ExecuteSSAction( ( pant_inicial ) ? A_PEDIR_CL_PRES : A_PEDIR, 1, tit, mensaje, NULL, " " );
		// printf( "Despues de ExecuteSSAction\n" );
		if( manual ) {
			rta = INGRESAR_CLAVE_MANUAL( x, y, tipo, cajero, sup );
#ifdef COMPILAR_GP416
		}else
			if( TIPO_TARJETA == GP416 && tipo != ENTRENAM ) {
				MENSAJE_SIN_SONIDO_STRING( S_INGRESE_TARJ_IDENTIFICACION );
				rta = INGRESAR_CLAVE_GP416( x, y, tipo, cajero, sup );
#endif
			}else
				if( TIPO_TARJETA == CONNAC && tipo != ENTRENAM ) {
					if( !cajero ) {
						cajero = ( tipo == SUPERVISOR || tipo == GERENTE ) ? sup : NRO_CAJERO;
					}
					rta = LEER_TARJETA_CONNAC( x, y, tipo, cajero );
				}else {
					/*------------------------- clave con tarjeta PCOS --------------------*/
#ifdef COMPILAR_PCOS
					MENSAJE_SIN_SONIDO_STRING( S_INGRESE_TARJ_IDENTIFICACION );
					rta = INGRESAR_CLAVE_PCOS( x, y, tipo );
#endif
				}
				ExecuteAction( ( pant_inicial ) ? A_FIN_PEDIR_CL_PRES : A_FIN_PEDIR );
	}
	BORRAR_MENSAJE();
	if( ( tipo == SUPERVISOR || tipo == GERENTE ) && rta ) {
		GRABAR_EVENT_SUPERVISOR( USUARIO_CLAVE_ANTERIOR(), datos_cajero.nro_legajo, rta, funcion, grabar_en_event );
		//if( rta )
	}
	if( sup && RAM_CODIGO_SUPERVISOR ) {
		if( USAR_LEGAJO == RUT ) {
			SET_MEMORY_LONG( __ram_codigo_supervisor, supl );
		}else {
			SET_MEMORY_LONG( __ram_codigo_supervisor, sup );
		}
	}else {
        SET_MEMORY_LONG( __ram_codigo_supervisor, 0 );
    }
    return ( rta );
}
/*****************************************************************************/
int LEE_CAJERO( int nro, struct _cajeros *datos_cajero )
/*****************************************************************************/
{
    int rta = 0;
    int tabla_ant, tipo_ant;
    tabla_ant = SELECTED2();
    tipo_ant = SELECTED_TIPO();
    
    //int area = SELECTED();
	memset(datos_cajero->nombre,0,26);
    //if( !USED( AREA_AUX5 ) ) {
        if( /*USE_DB( AREA_AUX5, _CAJEROS_SIC, ( char* )datos_cajero, sizeof( struct _cajeros ),
                    NULL, 0, 0 )*/
			OPEN_TABLE(T_CAJEROS, TT_ORIG ,( char* )datos_cajero, sizeof( struct _cajeros ))== 0 ) {
            //GET_EQUAL( ( char* )&nro );
			//SELECT_TABLE( T_CAJEROS, TT_ORIG );
			SET_WHERE("COD_CAJERO = %i",nro);
			RUN_QUERY(1);
            if( FOUND2() ) {
                rta = 1;
            }
            else {
                rta = 2;
            }
            CLOSE_TABLE( T_CAJEROS, TT_ORIG );
        }
        else {
            MENSAJE_STRING( S_ERROR_ARCHIVO_CAJEROS );
        }
    //}
    //else {
    //    MENSAJE_STRING( S_AREA5_USED );
    //}
    //SELECT( area );
    SELECT_TABLE( tabla_ant, tipo_ant );

    return ( rta );
}
/*****************************************************************************/
void MOSTRAR_CAJERO_EN_PANTALLA()
/*****************************************************************************/
{
    struct _cajeros datos_cajero;
    int att,i;
    long nro;
    #if defined(INVEL_W) || defined(INVEL_L)
    char caja_en_pantalla[11];
    char cajero_en_pantalla[37];
    #endif
    if( NRO_CAJERO ) {
        if( LEE_CAJERO( NRO_CAJERO, &datos_cajero ) == 1 ) {
            cajero_operando = 1;
            att = _ATRIBUTO;
            _ATRIBUTO = 7;
            if( USAR_LEGAJO == RUT ) {
                //strcpy( aux, RAM_DOC_CAJERO );
                //nro = atol( aux );
                nro = atol( datos_cajero.documento );
            }
            else {
                nro = ( IMPRIMIR_LEGAJO_CAJERO ) ? NRO_LEGAJO_CAJERO : NRO_CAJERO;
            }
            if( datos_cajero.tipo == SUPERVISOR ) {
                _snprintf(dato__cajero, sizeof(dato__cajero)-1, ST( S_SUPERVISOR___LD__NOMBRE___S ), nro,
                         datos_cajero.nombre );
            }
            else {
                _snprintf(dato__cajero, sizeof(dato__cajero)-1, ST( S_CAJERO___LD__NOMBRE___S ), nro, datos_cajero.nombre );
            }
            for( i = strlen( dato__cajero ) - 1;dato__cajero[i] == ' ';dato__cajero[i--] = 0 ) {
                ;
            }
            pos_x_cajero = 40 - ancho_panel / 2 - strlen( dato__cajero ) / 2;
            MOSTRAR_CADENA( pos_x_cajero, 1, dato__cajero );
            ExecuteSAction( A_MOSTRAR_CAJA_CAJERO_PRESENT,dato__cajero);
            _ATRIBUTO = att;
            #if defined(INVEL_W) || defined(INVEL_L)
            _snprintf(cajero_en_pantalla, sizeof(cajero_en_pantalla)-1, "%ld %s", nro, datos_cajero.nombre );
            _itoa( NRO_CAJA, caja_en_pantalla, 10 );
            ExecuteSSAction( A_MOSTRAR_CAJA_CAJERO, ( datos_cajero.tipo == SUPERVISOR ) ? 1 : 0,
                             cajero_en_pantalla, caja_en_pantalla, NULL, " " );
            #endif
        }
    }else{
        ExecuteSAction( A_MOSTRAR_CAJA_CAJERO_PRESENT," ");
    }
}
/*****************************************************************************/
long PIDE_SUPERVISOR_LONG( int x, int y, int tipo )
/*****************************************************************************/
{
    long sup = 999, ant_scanner=_PERMITIR_SCANNER, ant_banda=_PERMITIR_BANDA;
    int cod_sup,l_nro = 4;
	char *barra_usuario=NULL;

		

    if( MULTI_SUPERVISOR ) {
      switch ( tipo ) {
      	case SUPERVISOR:
      		MOSTRAR_CADENA( x + 2, y + 1, ST( S_N__SUPERVISOR_ ) );
      		break;
      	case GERENTE:
      		MOSTRAR_CADENA( x + 2, y + 1, "Nï¿½ Gerente:" );
      		break;
      }
	  
	 /*********************** definimos el largo del campo que usamos par buscar ********/
	  if (USAR_LEGAJO == RUT) {
			//l_nro = 9;
			l_nro = 8;
		} else {
			l_nro = (USAR_LEGAJO == LEGAJO) ? CANTIDAD_DE_DIGITOS_EN_NRO_LEGAJ : 4;
		}
	/***********************************************************************************/
        if( MODO_IDENTIFICACION_SUPERVISOR == IDENTIFICACION_BARRA )
			_PERMITIR_SCANNER=SI;

		if( MODO_IDENTIFICACION_SUPERVISOR == IDENTIFICACION_BANDA ){
			_PERMITIR_BANDA = SI;
			LIMPIA_BUFFER_TARJETA( NO );
		}
		//ORIGINAL sup =  PEDIR_ENTERO( x + ( tipo == GERENTE ? 13 : 16 ), y + 1, ( USAR_LEGAJO? 6 : 4 ) , NO );
		sup= (int )PEDIR_ENTERO_COD_BARRA( x + ( tipo == GERENTE ? 13 : 16 ), y + 1,l_nro, 0,  &barra_usuario, NO, SI, "" );
		
        _PERMITIR_BANDA = NO;
		_PERMITIR_SCANNER = NO;

      banda_supervisor = 0;
	/*     BANDA SUPERVISOR      */
      if( tipo == SUPERVISOR && ( MODO_IDENTIFICACION_SUPERVISOR == IDENTIFICACION_BANDA ) ) {
		  if( sup == -2 ) {
			  sup = LEER_NUMERO_USUARIO( CANTIDAD_DE_DIGITOS_EN_NRO_LEGAJ );
			} else {
				if( MODO_IDENTIFICACION_SUPERVISOR == IDENTIFICACION_BANDA ) {
					sup = 0;
					MENSAJE_STRING( S_MEDIO_DE_INGRESO_INVALIDO );
				}
		  }
	  } else {
		  if( sup == -2 && tipo == SUPERVISOR && ( MODO_IDENTIFICACION_SUPERVISOR == IDENTIFICACION_MANUAL) ) {
			  sup = 0;
			  MENSAJE_STRING( S_MEDIO_DE_INGRESO_INVALIDO );
		  }
	  }

/*     BANDA GERENTE      */
	  if( tipo == GERENTE && ( MODO_IDENTIFICACION_GERENTE == IDENTIFICACION_BANDA ) ) {
		  if( sup == -2 ) {
			  sup = LEER_NUMERO_USUARIO(l_nro);
		  } else {
			  if( MODO_IDENTIFICACION_GERENTE == IDENTIFICACION_BANDA ) {
				  sup = 0;
				  MENSAJE_STRING( S_MEDIO_DE_INGRESO_INVALIDO );
			  }
		  }
	  } else {
		  if( sup == -2 && tipo == GERENTE && ( MODO_IDENTIFICACION_GERENTE == IDENTIFICACION_MANUAL) ) {
			  sup = 0;
			  MENSAJE_STRING( S_MEDIO_DE_INGRESO_INVALIDO );
		  }
	  }
	  
	  /*     BARRA   SUPERVISOR   */
	  if( tipo == SUPERVISOR && ( MODO_IDENTIFICACION_SUPERVISOR == IDENTIFICACION_BARRA ) ) {
		  if( sup == -3 ||(sup == 0&& barra_usuario!=NULL && strlen(barra_usuario)>0)) {
			  sup = LEER_NUMERO_USUARIO_GENERICO( l_nro, barra_usuario );
		  } else {
			  if( MODO_IDENTIFICACION_SUPERVISOR == IDENTIFICACION_BARRA ) {
				  sup = 0;
				  MENSAJE_STRING( S_MEDIO_DE_INGRESO_INVALIDO );
			  }
		  }
	  } else {
		  if( ( sup == -3 ||(sup == 0 && barra_usuario!=NULL && strlen(barra_usuario)>0)) && tipo == SUPERVISOR && ( MODO_IDENTIFICACION_SUPERVISOR == IDENTIFICACION_MANUAL) ) {
			  sup = 0;
			  MENSAJE_STRING( S_MEDIO_DE_INGRESO_INVALIDO );
		  }
	  }
	  if( tipo == GERENTE && ( MODO_IDENTIFICACION_GERENTE == IDENTIFICACION_BARRA ) ) {
		  if( sup == -3 ||(sup == 0&& barra_usuario!=NULL &&strlen(barra_usuario)>0)) {
			  sup = LEER_NUMERO_USUARIO_GENERICO( l_nro, barra_usuario );
		  } else {
			  if( MODO_IDENTIFICACION_GERENTE == IDENTIFICACION_BARRA ) {
				  sup = 0;
				  MENSAJE_STRING( S_MEDIO_DE_INGRESO_INVALIDO );
			  }
		  }
	  } else {
		  if( ( sup == -3 ||(sup == 0 && barra_usuario!=NULL&& strlen(barra_usuario)>0)) && tipo == GERENTE && ( MODO_IDENTIFICACION_GERENTE == IDENTIFICACION_MANUAL) ) {
			  sup = 0;
			  MENSAJE_STRING( S_MEDIO_DE_INGRESO_INVALIDO );
		  }
	  }
	  /******************************************************************************************/

/* BUSQUEDA DEL USUARIO IDENTIFICADO*/

        if( sup > 0 ) {
            if( USAR_LEGAJO == RUT ) {
                cod_sup = BUSCA_CAJERO_X_DNI( sup );
            }
            else if( USAR_LEGAJO == LEGAJO ) {
                cod_sup = BUSCA_CAJERO_X_LEGAJO( sup );
            }
            else {
                cod_sup = BUSCA_CAJERO_X_CODIGO( sup );
            }
		    LEE_CLAVE_CAJERO(  cod_sup, NULL );
            if( sup > 0 ) {
                if( tipo == SUPERVISOR && !( ES_SUPERVISOR( sup ) || ES_GERENTE( sup ) ) ) {
                    //MENSAJE_STRING( S_NO_NUMERO_SUPERVISOR );
					MENSAJE_ID_CON_PAUSA_CAJA( S_NO_NUMERO_SUPERVISOR );
                    BORRAR_MENSAJE();
		            sup = 0;
                } else {
		            if( tipo == GERENTE && !ES_GERENTE( sup ) ) {
		                //MENSAJE("NO ES NUMERO DE GERENTE");
						MENSAJE_ID_CON_PAUSA_CAJA( S_NO_NUMERO_GERENTE );
                        BORRAR_MENSAJE();
		                sup = 0;
		            }
                }
            } else {
                if( tipo == SUPERVISOR ) {
                    //MENSAJE_STRING( S_NO_NUMERO_SUPERVISOR );
					MENSAJE_ID_CON_PAUSA_CAJA( S_NO_NUMERO_SUPERVISOR );
                    BORRAR_MENSAJE();
		            sup = 0;
                } else {
		            if( tipo == GERENTE ) {
		                //MENSAJE("NO ES NUMERO DE GERENTE");
						MENSAJE_ID_CON_PAUSA_CAJA( S_NO_NUMERO_GERENTE );
                        BORRAR_MENSAJE();
		                sup = 0;
		            }
                }
            }
        } else
            sup = 0;

        }
    return ( sup );
}
/*****************************************************************************/
int BUSCA_CAJERO_X_DNI( long dni )
/*****************************************************************************/
{
    int rta = 0, l_cad;
    char cad_dni[15];
    _ltoa( dni, cad_dni, 10 );
    l_cad = strlen( cad_dni );
    cad_dni[l_cad] = 0;
	memset(datos_cajero.nombre,0,26);

	if( !dni )
		return( rta );

    if( /*USE_DB( AREA_CAJEROS, _CAJEROS_SIC, ( char* )&datos_cajero, sizeof( datos_cajero ), NULL, 0,
                0 )*/
		OPEN_TABLE(T_CAJEROS, TT_ORIG ,( char* )&datos_cajero, sizeof( struct _cajeros ))== 0 ) {
        //Indices Cajero
        //              1 - Codigo
        //              2 - Legajo
        //              3 - Nombre
        //              4 - DNI
        //SET_ORDER( 4 );
        //GET_EQUAL( cad_dni );
		SELECT_TABLE( T_CAJEROS, TT_ORIG );
		SET_ORDER2("DOCUMENTO");
		SET_WHERE("DOCUMENTO = %ld",dni);
		RUN_QUERY(1);
        if( FOUND2() ) {
            rta = datos_cajero.cod_cajero;
        }
        CLOSE_TABLE( T_CAJEROS, TT_ORIG );
    }
    else {
        MENSAJE_STRING( S_ERROR_ARCHIVO_CAJEROS );
    }
    return ( rta );
}
/*****************************************************************************/
int BUSCA_CAJERO_X_CODIGO( int nro )
/*****************************************************************************/
{
    int rta = 0;
    //CLOSE_DATABASE( AREA_CAJEROS );
	memset(datos_cajero.nombre,0,26);
    if( /*USE_DB( AREA_CAJEROS, _CAJEROS_SIC, ( char* )&datos_cajero, sizeof( datos_cajero ), NULL, 0,
                0 )*/
		OPEN_TABLE(T_CAJEROS, TT_ORIG ,( char* )&datos_cajero, sizeof( struct _cajeros ))== 0 ) {
        //SET_ORDER( 1 );
        //GET_EQUAL( ( char* )&nro );
		SELECT_TABLE( T_CAJEROS, TT_ORIG );
		SET_ORDER2("COD_CAJERO");
		SET_WHERE("COD_CAJERO = %i",nro);
		RUN_QUERY(1);
        if( FOUND2() ) {
            rta = nro;
        }
        else {
            rta = -1;
        }
        CLOSE_TABLE( T_CAJEROS, TT_ORIG );
    }
    else {
        MENSAJE_STRING( S_ERROR_ARCHIVO_CAJEROS );
    }
    return ( rta );
}
/*****************************************************************************/
int CLAVE_EXTENDIDA( int tipo )
/*****************************************************************************/
{
	int rta;

   if( ( tipo == SUPERVISOR && config.clave_extendida_supervisor == 'S' ) 
		|| ( tipo == CAJERO && config.clave_extendida_cajero  == 'S') 
		|| ( tipo == GERENTE && config.clave_extendida_supervisor == 'S' ) 
		|| ( tipo == SUPERCAJERO 
			&& ( ( config.clave_extendida_supervisor == 'S' ) 
				|| ( config.clave_extendida_cajero == 'S' ) ) ) ) {
      rta = SI;
   } else {
      rta = NO;
   }

	return( rta );
}
/*****************************************************************************/
long LEER_NUMERO_USUARIO_GENERICO( int longitud_a_tomar,char * buffer2 )
/*****************************************************************************/
{
    char /*buffer2[50],buffer1[80],*/*p;
    int l;
    long usuario = -1;
	l = strlen( (const char *)PREFIJO_TARJETA_USUARIO );
	if( l < 1 || l > 10 ) {
		l = 10;
	}
	p = PREFIJO_TARJETA_USUARIO;
	if( memcmp( buffer2, p, l ) == 0 ) {
		p = memchr( &buffer2[l], '=', 50 - l );
		if( p ) {
			*p = 0;
		}
		if( longitud_a_tomar > 0 && longitud_a_tomar < 9 ){
			buffer2[l+longitud_a_tomar]=0;//si es configurable se debe tomar cierta cantidad de digitos.
		}else
			buffer2[l+9]=0;//el maximo long es MAXLONG; 2147483647
		usuario = atol( &buffer2[l] );
	}else {
		MENSAJE_STRING( S_NO_TARJETA_USUARIO );
	}
	return ( usuario );
}