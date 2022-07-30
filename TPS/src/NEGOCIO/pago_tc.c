#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <boletin.h> 
//#include <browse.h> 
#include <btrv.h> 
#include <b_mens.h> 
#include <cio.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <get_fech.h> 
#include <mensaje.h> 
#include <m_cadena.h> 
#include <pagos.h> 
#include <pago_tc.h> 
#include <pant.h> 
#include <stringt.h> 
#include <tkt.h> 
#include <trim.h> 
#include <_cr1.h> 
#include <aarch.h>
#include <ftoc.h>
#include <round.h>
#ifdef INVEL_W
#include <io.h>
#else
#include <sys/io.h>
#endif
#include <path.h>
#include <deftables.h>
#include <db.h>
#include <clave.h>
#include <recupero.h>
#include <motor.h>

/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifdef INVEL_L
#else
#endif
#if defined(INVEL_W) || defined(INVEL_L)
#endif
#undef EOF
struct _t_compra  *t_compra_sic;
int cod_medio_padre = 0;
/*****************************************************************************/
int _elegir_t_compra( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0;
    char cod_tcompra;
	char xx[20];
	double monto_2 = 0.0;
    char tmp2[40];
	long reg;

    switch( comando ) {
        case _BUSCAR:
            cod_tcompra = atoi( cadena );
			if( cod_tcompra == 0 && cod_medio > 0 ){
				SET_WHERE("COD_T_COMPRA >= '%d'AND COD_MEDIO_PADRE = %u", cod_tcompra, cod_medio );
			}else
			SET_WHERE("COD_T_COMPRA >= '%d'",cod_tcompra);
			SET_ORDER2( "COD_T_COMPRA" );
			RUN_QUERY(NO);
			b->posicionar=1;
            break;
        case _MOSTRAR:
			memset( xx, 0, sizeof( xx ) );
            _itoa( t_compra_sic->cod_t_compra, xx, 10 );
            setFieldValuepChar( b->index, 0, xx );
            setFieldValuepChar( b->index, 1, t_compra_sic->des_t_compra );

            if( UTILIZA_PROMOCIONES && UTILIZA_MODULO_PROMOCIONES && !RAM_NOTA_CR ) {
                if( t_compra_sic->exclusivo_promocion == 'S' && MEDIO_ACTIVADO_PROMO( t_compra_sic->cod_t_compra, 0, cod_medio )){
				    monto_2 = MONTO_MEDIO_ACTIVADO_PROMO( t_compra_sic->cod_t_compra, 0, cod_medio );
				    if( monto_2 < RAM_IMPORTE ){					
					    FTOC( monto_2, tmp2, ENTEROS + 1, DECIMALES, 0, 0 );
				    } else {
					    FTOC( RAM_RESTO, tmp2, ENTEROS + 1, DECIMALES, 0, 0 );
                    }
				    LTRIM( tmp2 );				
				    sprintf( xx, "%s", tmp2 );
                }
			}
			//setFieldValuepChar( b->index, 2, xx );
            //MOSTRAR_ENTERO( 8, 10 + dato, "99", t_compra_sic->cod_t_compra );
            //MOSTRAR_CADENA( 11, 10 + dato, t_compra_sic->des_t_compra );
            break;
		case _SKIP:
			reg = GET_POSITION_REG();
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
            while(!rta 
					&& t_compra_sic->exclusivo_promocion == 'S' 
					&& !MEDIO_ACTIVADO_PROMO( t_compra_sic->cod_t_compra, 0, cod_medio ) ) {                
                SKIP2( ( !dato ) ? 1 : dato );
                rta = dbEOF() || dbBOF();
                if( rta ){
					GET_DIRECT_REG( reg, 0 );
                }
            }
			if( dato != 0 && !rta && cod_medio_padre != 0 &&t_compra_sic->cod_medio_padre != cod_medio_padre ) {
				SKIP2( -dato );
				rta = 1;
			}	
            break;
        case _VALIDAR:
            if( dato >= '0' && dato <= '9' ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
            /*Validar si el plan seleccionado es exclusivo de promocion*/
            if( CANTIDAD_REGISTROS_REC(ARCH_REC_ACT_MEDIO_MOTOR, sizeof(struct activacion_medios ) ) ) {
                //HAY Promocion Activada 
                if( !MEDIO_ACTIVADO_PROMO( t_compra_sic->cod_t_compra, 0, cod_medio ) ) {
                    //El Submedio seleccionado NO Activa la Promoción
                    int teclado_alfabetico;
                    char car;

                    MENSAJE_SIN_SONIDO( ST( S_NO_UTILIZA_PROMO_ACTIVADA_S_N ), NO );
                    teclado_alfabetico = TECLADO_ALFABETICO;
                    TECLADO_ALFABETICO = 1;
                    do {
                        car = GETCH();
                    }
                    while( car != 'S' && car != 'N' && car != 's' && car != 'n' );
                    if( car == 'N' || car == 'n' ) {
                        BORRAR_MENSAJE();
                        TECLADO_ALFABETICO = teclado_alfabetico;
                        break;
                    }
                    BORRAR_MENSAJE();
                    TECLADO_ALFABETICO = teclado_alfabetico;
                }
            }
			/*Si el plan es exclusivo de promocion, verifica si fue activado por alguna promocion*/
            if( t_compra_sic->exclusivo_promocion == 'S' && 
                ( ( monto_2 = ROUND( MONTO_MEDIO_ACTIVADO_PROMO( t_compra_sic->cod_t_compra, 0, cod_medio ),
                    ENTEROS + 1, DECIMALES ) ) < RAM_RESTO ) ){
                FTOC( monto_2, tmp2, ENTEROS + 1, DECIMALES, 0, 0 );
			}
			rta = 1;
            break;
		case _TECLA:
		if( dato == -73 || dato == -81 /*|| ( dato >= '0' && dato <= '9' )*/ )
			b->tecla = 0;
		break;
		case _LIMIT:
            if( dato > 0 ) {
                SET_LIMIT( dato, SI );
            } else {
                SET_LIMIT( NO, NO );
            }
            break;
    }
    return ( rta );
}
/*****************************************************************************/
int PIDE_TICKET_COMPRA( int *id_registro, long *autorizacion, long *datos_ad,
                        char *cod_medio_vuelto, char *cod_submedio_vuelto, char *tipo_vuelto,
                        int *datos_adicionales, int cod_medio, int origen_ventas, 
                        int *cod_restriccion_art, int solicitar_submedio )
/*****************************************************************************/
{
    int rta = -1, ok = 0, found = 0;
    struct _t_compra  t_compra;
	/*struct
	{
		int cod_medio_padre;
		char cod_t_compra;
	} indice3;*/

    /*---------------------------- Abre el archivo ---------------------------*/
    t_compra_sic = &t_compra;
    if( /*USE_DB( AREA_T_COMPRA, _t_compra , ( char* )t_compra_sic, sizeof( struct _t_compra  ),
                NULL, 0, 0 )*/
		OPEN_TABLE(T_COMPRA, TT_ORIG,( char* )t_compra_sic,sizeof( struct _t_compra  ))== 0 ) {
        /*-------------------------- Prepara la pantalla -------------------------*/
		SELECT_TABLE( T_COMPRA, TT_ORIG );

        if( cod_medio != 0 ) {
			//indice3.cod_medio_padre = cod_medio;
			//indice3.cod_t_compra = 0;
			//SET_ORDER( 3 );//COD_MEDIO_PADRE + COD_T_COMPRA 
			//GET_GREATER_E( ( char* )&indice3 ); 
			//SET_WHERE("COD_MEDIO_PADRE = '%i' AND COD_T_COMPRA >'%i'", cod_medio, 0 );
			SET_WHERE("COD_MEDIO_PADRE = '%i'", cod_medio);
			RUN_QUERY(NO);
            cod_medio_padre = cod_medio;
			found = FOUND2();
        }
        else {
            //SET_ORDER( 1 ); //COD_MEDIO NUEVO CAMPO
			SET_WHERE("COD_T_COMPRA = '0'");
            //GO( TOP );
			RUN_QUERY(NO);
            found = FOUND2();
        }
        if( found ) {
            if( solicitar_submedio ) {
			    ok = Consulta_t_compra( _elegir_t_compra, (unsigned short)cod_medio );
                if(ok)
                    rta = t_compra_sic->cod_t_compra;
            } else {
                ok = 1;
                rta = 1; //se almacena todo en 1 - ver si causa problemas
            }
		} else {
			MENSAJE("NO SE ENCONTRARON MEDIOS ASOCIADOS");
		}
		if( ok ) {
			if( t_compra_sic->pide_supervisor == 'S' && origen_ventas ) {
				if( !( SOLICITAR_CLAVES( 30 - ancho_panel, 20, NO, SI, SI, "AUTORIZACION MEDIO",
					"AutorizÂ¢ t_compra", _F_SUP_MEDIO_DE_PAGO, SI ) ) ) {
					ok = 0;
					MENSAJE( "NO SE AUTORIZA EL MEDIO" );
				}
			}
		}
        if( ok ) {
            rta = t_compra_sic->cod_t_compra;
            if( id_registro != NULL && t_compra_sic->id_registro ) {
                *id_registro = t_compra_sic->id_registro;
            }
            if( cod_medio_vuelto != NULL ) {
				*cod_medio_vuelto =((t_compra_sic->cod_medio_vuelto > 0 )? t_compra_sic->cod_medio_vuelto:0);
            }
            if( cod_submedio_vuelto != NULL ) {
                *cod_submedio_vuelto = ((t_compra_sic->cod_submedio_vuelto > 0 )? t_compra_sic->cod_submedio_vuelto:0);
            }
            if( tipo_vuelto != NULL ) {
                *tipo_vuelto = t_compra_sic->vuelto;
            }
            if( datos_adicionales != NULL ) {
                *datos_adicionales = t_compra_sic->cod_solicitud;
            }
			if( cod_restriccion_art != NULL ) {
				*cod_restriccion_art = t_compra_sic->cod_restriccion_art;
			}
        }
        CLOSE_TABLE( T_COMPRA, TT_ORIG );
    }
    if( rta > 0 ) {
        if( t_compra_sic->autorizacion == 'S' && autorizacion ) {
			//*autorizacion = PAGOS_PIDE_AUTORIZACION( ST( S_AUTORIZACION ) );
			*autorizacion = 1;
        }
        if( t_compra_sic->datos_adicionales == 'S' && datos_ad ) {
            //
            // *datos_ad = PAGOS_PIDE_AUTORIZACION( ST( S_DOCUMENTO ) );
        }
    }
    BORRAR_MENSAJE();
    return ( rta );
}
/*****************************************************************************/
double OBTENER_IMPORTE_T_COMPRA_DESDE_SCANNER( char *cadena_scanner, char cod_t_compra )
/*****************************************************************************/
{
    struct _t_compra  t_compra;
    char *letra_scanner = cadena_scanner;
    char *letra_mascara = t_compra.mascara_scanner;
    double importe_scanner = 0;
    char arma_importe[21];
    int iError = 0, i, seguir;
    char cadena_entera[24];
    char cadena_decimal[24];
    memset( cadena_entera, 0, sizeof( cadena_entera ) );
    memset( cadena_decimal, 0, sizeof( cadena_entera ) );
    if( /*USE_DB( AREA_AUX, _t_compra , ( char* )&t_compra, sizeof( t_compra ), NULL, 0, 0 )*/
		OPEN_TABLE(T_COMPRA, TT_ORIG,( char* )&t_compra,sizeof( t_compra )) == 0 ) {
		SELECT_TABLE( T_COMPRA, TT_ORIG );
        //SET_ORDER( 1 );
		SET_WHERE("COD_T_COMPRA = '%i'",cod_t_compra);
        //GET_EQUAL( &cod_t_compra );
		RUN_QUERY(NO);
        if( FOUND2() ) {
            if( t_compra.habilitado_scanner == 'S' ) {
                // Controla el largo de la barra
                for( i = 0, seguir = 1;letra_scanner[i] && seguir;i++ ) {
                    if( letra_scanner[i] < '0' || letra_scanner[i] > '9' ) {
                        letra_scanner[i] = 0;
                        seguir = 0;
                    }
                }
                TRIM( letra_mascara );
                if( strlen( letra_scanner ) != strlen( letra_mascara ) ) {
                    iError = 1;
                }
                while( *letra_mascara && !iError ) {
                    if( *letra_mascara >= '0' && *letra_mascara <= '9' ) {
                        if( *letra_mascara != *letra_scanner ) {
                            iError = 1;
                            break;
                        }
                    }
                    else if( *letra_mascara == 'E' ) {
                        cadena_entera[strlen( cadena_entera )] = *letra_scanner;
                        //} else if (*letra_mascara == 'D') {
                    }
                    else if( *letra_mascara == 'K' ) {
                        cadena_decimal[strlen( cadena_decimal )] = *letra_scanner;
                    }
                    letra_mascara++;
                    letra_scanner++;
                }
                if( !iError ) {
                    sprintf( arma_importe, "%s.%s", cadena_entera, cadena_decimal );
                    importe_scanner = atof( arma_importe );
                }
            }
            /* else {
             * * char debug[100];
             * * sprintf(debug, "NO HABILI MED %d", t_compra.habilitado_scanner);
             * * } */
        }
        CLOSE_TABLE( T_COMPRA, TT_ORIG );
    }
    return importe_scanner;
}
/*****************************************************************************/
int PIDE_CANTIDAD_CUPONES()
/*****************************************************************************/
{
    int cant;
    cant = ( int )PEDIR_GRAN_ENTERO( 35, 23, 4 );
    return ( cant );
}
/*****************************************************************************/
double PIDE_VALOR_CUPONES()
/*****************************************************************************/
{
    double importe;
    importe = PEDIR_GRAN_FLOTANTE( 30, 20, ENTEROS + 1, DECIMALES );
    return ( importe );
}
#ifdef COMPILAR_TECLA_REPROCESAR
/*****************************************************************************/
void REPROCESAR_TICKETS_DE_COMPRA( char tipo_pago )
/*****************************************************************************/
{
    int h;
    for( h = 0;h < RAM_P_PAGO;h++ ) {
        if( medios[_PAGO_MODO( h )].ticket_compra ) {
            if( tipo_pago == 1 ) {
                _ADD_MEMORY_DOUBLE( __x_cobros_importe, _PAGO_MODO( h ), -( _PAGO_IMPORTE( h ) ) );
                _ADD_MEMORY_INT( __x_cobros_cantidad, _PAGO_MODO( h ), -1 );
                if( _PAGO_VUELTO_MEDIO( h ) ) {
                    _ADD_MEMORY_DOUBLE( __x_cobros_importe, _PAGO_COD_MEDIO_VUELTO( h ) - 1,
                                        _PAGO_VUELTO_MEDIO( h ) );
                }
            }
            else {
                _ADD_MEMORY_DOUBLE( __x_ventas_importe, _PAGO_MODO( h ), -( _PAGO_IMPORTE( h ) ) );
                _ADD_MEMORY_INT( __x_ventas_cantidad, _PAGO_MODO( h ), -1 );
                if( _PAGO_VUELTO_MEDIO( h ) ) {
                    _ADD_MEMORY_DOUBLE( __x_ventas_importe, _PAGO_COD_MEDIO_VUELTO( h ) - 1,
                                        _PAGO_VUELTO_MEDIO( h ) );
                }
            }
        }
    }
}
#endif
/*****************************************************************************/
void IMPRIME_DATOS_TICKET_COMPRA( int nro )
/*****************************************************************************/
{
    struct _t_compra  t_compra;
    if( /*USE_DB( AREA_AUX, _t_compra , ( char* )&t_compra, sizeof( t_compra ), NULL, 0, 0 )*/
		OPEN_TABLE(T_COMPRA, TT_ORIG,( char* )&t_compra,sizeof( t_compra )) == 0 ) {
		SELECT_TABLE( T_COMPRA, TT_ORIG );
        //GET_EQUAL( ( char* )&nro );
		SET_WHERE("COD_T_COMPRA = '%i'",nro);
		RUN_QUERY(NO);
        MOSTRAR_CADENA( 21, PRN, t_compra.des_t_compra );
    }
    CLOSE_TABLE( T_COMPRA, TT_ORIG );
}
/*****************************************************************************/
int COD_VALIDA_T_COMPRA( char marca )
/*****************************************************************************/
{
    int rta = 0;
    struct _t_compra  t_compra;
    //---------------------------- Abre el archivo ---------------------------//
    if( /*USE_DB( AREA_AUX, _t_compra , ( char* )&t_compra, sizeof( t_compra ), NULL, 0, 0 )*/
		OPEN_TABLE(T_COMPRA, TT_ORIG,( char* )&t_compra,sizeof( t_compra )) == 0 ) {
        //GET_EQUAL( &marca );
		SELECT_TABLE( T_COMPRA, TT_ORIG );
		SET_WHERE("COD_T_COMPRA = '%i'",marca);
		RUN_QUERY(NO);
        if( FOUND2() ) {
            rta = t_compra.cod_validacion;
        }
        CLOSE_TABLE( T_COMPRA, TT_ORIG );
    }
    return ( rta );
}
/*****************************************************************************/
int AGREGA_A_POSICION( char pos, char *codigo_barra, double importe )
/*****************************************************************************/
{
    struct
    {
        char codigo_barra[32];
        double importe;
    } codigo_en_bin;
    char archivo[16];
    int handle;
    int error = 1;
    sprintf( archivo, "t_tmp.%d", pos );
    // sprintf( archivo, "tcomptmp.%d", pos);
    memset( &codigo_en_bin, 0, sizeof( codigo_en_bin ) );
    if( codigo_barra != NULL ) {
        memcpy( codigo_en_bin.codigo_barra, codigo_barra, sizeof( codigo_en_bin.codigo_barra ) );
    }
    codigo_en_bin.importe = importe;
    handle = _OPEN_O_CREAT( archivo, O_WRONLY, __LINE__, __FILE__ );
    if( handle >= 0 ) {
        LSEEK( handle, 0, SEEK_END, __LINE__, __FILE__ );
        error = _WRITE( handle, ( char* )&codigo_en_bin, sizeof( codigo_en_bin ), __LINE__,
                        __FILE__ );
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    return !error;
}
/*****************************************************************************/
int AGREGA_BIN_EV_T_COMPRA_A_BTRV( void )
/*****************************************************************************/
{
    struct
    {
        char codigo_barra[32];
        double importe;
    } codigo_en_bin;
    char archivo[30];
    int handle;
    int salir = 0;
    int posicion_codigo_barra = 1;
    int codigos_en_archivo = 0;
    char nom_aux[16];
    if( medios[event_pago.modo_pago - 1].ticket_compra ) {
        //sprintf( archivo, "t_tmp.%d", event_pago.posicion );
        sprintf( nom_aux, "t_tmp.%d", event_pago.posicion );
        sprintf( archivo, "%s%s", PATH_TMP_DADIC, nom_aux );
        if( access( archivo, 0 ) == 0 ) {
            //handle = _OPEN( archivo, O_RDONLY, __LINE__, __FILE__ );

			#ifdef INVEL_L
			handle = _OPEN( archivo, O_WRONLY, __LINE__, __FILE__ );
			#else
			handle = _OPEN( archivo, O_WRONLY | O_BINARY, __LINE__, __FILE__ );
			#endif
            //SELECT( AREA_COD_BARRA );
			SELECT_TABLE( T_EV_COD_BARRA, TT_ORIG );
            if( handle >= 0 ) {
                codigos_en_archivo = filelength( handle ) / sizeof( codigo_en_bin );
                while( !salir && codigos_en_archivo > 0 ) {
                    if( !_READ( handle, ( char* )&codigo_en_bin, sizeof( codigo_en_bin ), __LINE__,
                                __FILE__ ) ) {
                        memset( &event_cod_barra, 0, sizeof( struct _ev_cod_barra ) );
                        event_cod_barra.caja = event_pago.caja;
                        event_cod_barra.nro_z = event_pago.nro_z;
                        event_cod_barra.id_evento = event_pago.id_evento;
                        event_cod_barra.posicion_medio = event_pago.posicion;
                        event_cod_barra.posicion_cod_barra = posicion_codigo_barra++;
                        event_cod_barra.cuenta = ( char )event_pago.numero_id;
                        event_cod_barra.importe = codigo_en_bin.importe;
                        memcpy( event_cod_barra.codigo_barra, &codigo_en_bin,
                                sizeof( codigo_en_bin ) );
                        event_cod_barra.cod_sucursal = config.sucursal;
						if( !NRO_FECHA_OPERACION )
						SET_MEMORY_INT( __nro_fecha_operacion, GET_FECHA_DOS() );

						event_cod_barra.fecha_ticket = NRO_FECHA_OPERACION;
                        event_cod_barra.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
                        #ifdef INVEL_L
                        //BEGIN_TRANSACTION();
                        #endif
                        //INSERT();
						INSERT2(NO);
                        #ifdef INVEL_L
                        //END_TRANSACTION();
                        #endif
                        codigos_en_archivo--;
                    }
                    else {
                        salir = 1;
                    }
                }
                _CLOSE( handle, __LINE__, __FILE__ );
            }
        }
    }
    return 1;
}
/*****************************************************************************/
int CONTROLA_IMPORTE_TICKET_COMPRA( char marca, char *numero_bono, double importe )
/*****************************************************************************/
{
    int rta = 1, ok = 0;
    struct _t_compra  t_compra;
    char tipo_boletin = 0;
    int nro_archivo;
    /*---------------------------- Abre el archivo ---------------------------*/
    t_compra_sic = &t_compra;
    if( /*USE_DB( AREA_AUX, _t_compra , ( char* )t_compra_sic, sizeof( struct _t_compra  ),
                NULL, 0, 0 )*/
		OPEN_TABLE(T_COMPRA, TT_ORIG,( char* )t_compra_sic,sizeof( struct _t_compra  )) == 0 ) {
		SELECT_TABLE( T_COMPRA, TT_ORIG );
		SET_WHERE("COD_T_COMPRA = '%i'",marca);
		RUN_QUERY(NO);
        //GET_EQUAL( &marca );
        if( FOUND2() ) {
            tipo_boletin = t_compra_sic->tipo_boletin;
            nro_archivo = t_compra_sic->nro_archivo;
            if( rta && tipo_boletin ) {
                if( PIDE_NRO_BONO( numero_bono ) ) {
                    ok = BUSCAR_MEDIO_EN_BOLETIN( tipo_boletin, nro_archivo, numero_bono, &importe,
                                                  SI, SI, SI );
                }
                if( !ok ) {
                    rta = 0;
                }
            }
        }
        CLOSE_TABLE( T_COMPRA, TT_ORIG );
    }
    return ( rta );
}
/*****************************************************************************/
void NOMBRE_TICKET_COMPRA( int nro, char *cadena )
/*****************************************************************************/
{
    struct _t_compra  t_compra;
    strcpy( cadena, "DESCONOCIDO" );
    if( /*USE_DB( AREA_AUX, _t_compra , ( char* )&t_compra, sizeof( t_compra ), NULL, 0, 0 )*/
		OPEN_TABLE(T_COMPRA, TT_ORIG,( char* )&t_compra,sizeof( t_compra )) == 0 ) {
        //GET_EQUAL( ( char* )&nro );
		SELECT_TABLE( T_COMPRA, TT_ORIG );
		SET_WHERE("COD_T_COMPRA = '%i'",nro);
		RUN_QUERY(NO);
        if( FOUND2() ) {
            strcpy( cadena, t_compra.des_t_compra );
        }
        CLOSE_TABLE( T_COMPRA, TT_ORIG );
    }
}

/*****************************************************************************/
int VALIDA_T_COMPRA( char marca )
/*****************************************************************************/
{
	int       rta = 0;
	struct _t_compra  t_compra;

   //---------------------------- Abre el archivo ---------------------------//	
   //if( USE_DB( AREA_AUX, _t_compra , ( char* )&t_compra, sizeof( t_compra ), NULL, 0, 0 ) == 0 ) {
   if( OPEN_TABLE(T_COMPRA, TT_ORIG,( char* )&t_compra,sizeof( t_compra )) == 0 ) {
		//GET_EQUAL( &marca );
	    SELECT_TABLE( T_COMPRA, TT_ORIG );
		SET_WHERE("COD_T_COMPRA = '%i'", marca);
		RUN_QUERY(NO);
		if( FOUND2(  ) ) {
			//if( t_compra_sic->exclusivo_promocion == 'S' ){ 
			if( t_compra.exclusivo_promocion == 'S' ){ 
				rta = 1;				
			}
		}
		//CLOSE_DATABASE( AREA_AUX );
		CLOSE_TABLE( T_COMPRA, TT_ORIG );
	}
	return ( rta );
}
