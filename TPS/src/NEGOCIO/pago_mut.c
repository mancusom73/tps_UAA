/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <config.h>
#ifdef COMPILAR_MUTUALES
#include <stdlib.h>
#include <math.h>
#include <aarch.h>
#pragma pack(1)
#include <cr.h>
#include <string.h>
#include <deftables.h>
#include <db.h>
#include <m_cadena.h>
#include <pant.h>
#include <mensaje.h>
#include <clave.h>
#include <pagos.h>
#include <_cr1.h>
#include <cmemory.h>
#include <b_mens.h>
#include <cr_pant.h>
#include <boletin.h>
#include <path.h>
#include <motor.h>
#include <trim.h>
#include <ftoc.h>
#include <round.h>
#include <recupero.h>
#include <pan_pago.h>
struct _t_mutuales *mutuales_sic;
static struct _cuo_mut cuo_mut;
static struct _ind_cuo_mut ind_cuo_mut;
struct _t_mutuales mutuales;
int		      cod_medio_padre;
	struct _indice
{
    char cod_medio;
    char cod_submedio;
};
int venta;
/*****************************************************************************/
int _elegir_mutuales( int comando, char *cadena, int dato, struct _browse *b, double importe, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0;
    char cod_mutual;
	char xx[20];
	double cot;
	double monto = 0.0;
    char tmp2[40];
	long reg;

    switch( comando ) {
		case _BUSCAR:
			cod_mutual = atoi( cadena );
			if( cod_mutual == 0 && cod_medio > 0 ){
				SET_WHERE("COD_MUTUAL >= '%d'AND COD_MEDIO_PADRE = %u", cod_mutual, cod_medio );
			}else
			SET_WHERE("COD_MUTUAL >= '%d'", cod_mutual );
			SET_ORDER2( "COD_MUTUAL");
			RUN_QUERY(NO);
			b->posicionar=1;
			break;
        case _MOSTRAR:
				_itoa( mutuales_sic->cod_mutual, xx, 10 );
				setFieldValuepChar( b->index, 0, xx );
				setFieldValuepChar( b->index, 1, mutuales_sic->des_mutual );
				memset( xx, 0, sizeof( xx ) );
			memset( tmp2, 0, sizeof( tmp2 ) );
			cot = ( mutuales_sic->cotizacion > 0.001 ) ? mutuales_sic->cotizacion : 1.0;
			FTOC( importe / cot, tmp2, ENTEROS + 1, DECIMALES, 0, 0 );			
			
            if( UTILIZA_PROMOCIONES && UTILIZA_MODULO_PROMOCIONES && !RAM_NOTA_CR ) {
                if( mutuales_sic->exclusivo_promocion == 'S' && MEDIO_ACTIVADO_PROMO( mutuales_sic->cod_mutual, 0, cod_medio )){
				    monto = MONTO_MEDIO_ACTIVADO_PROMO( mutuales_sic->cod_mutual, 0, cod_medio );
				    if( monto < ( importe / cot ) ){					
					    FTOC( monto / cot, tmp2, ENTEROS + 1, DECIMALES, 0, 0 );
				    }else{
					    FTOC( importe / cot, tmp2, ENTEROS + 1, DECIMALES, 0, 0 );
				    }				
			    }
            }
			
            LTRIM( tmp2 );				
			sprintf( xx, "%s", tmp2 );
			setFieldValuepChar( b->index, 2, xx );
            break;
        case _SKIP:
			reg = GET_POSITION_REG();
			SKIP2( dato );
            rta = dbEOF() || dbBOF();

            while(!rta && ( mutuales_sic->exclusivo_promocion == 'S' 
					&& !MEDIO_ACTIVADO_PROMO( mutuales_sic->cod_mutual, 0, cod_medio ) )
							|| ( venta && mutuales_sic->hab_ventas == 'N' ) ){
                SKIP2( ( !dato ) ? 1 : dato );
                rta = dbEOF( ) || dbBOF( );
                if( rta ){
                    GET_DIRECT_REG( reg, 0 );
                }
            }

			if( dato != 0 && !rta && cod_medio_padre != 0 && mutuales_sic->cod_medio_padre != cod_medio_padre ) {
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
                if( !MEDIO_ACTIVADO_PROMO( mutuales_sic->cod_mutual, 0, cod_medio ) ) {
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
			cot = ( mutuales_sic->cotizacion > 0.001 ) ? mutuales_sic->cotizacion : 1.0;
            if( mutuales_sic->exclusivo_promocion == 'S' && 
                ( ( monto = ROUND( MONTO_MEDIO_ACTIVADO_PROMO( mutuales_sic->cod_mutual, 0, cod_medio ),
                    ENTEROS + 1, DECIMALES ) ) < ( importe / cot ) ) ){
				FTOC( monto/cot, tmp2, ENTEROS + 1, DECIMALES, 0, 0 );
                LTRIM( tmp2 );	
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
int PIDE_MUTUAL( int *id_registro, long *autorizacion, long *datos_ad, char *cod_medio_vuelto,
                 char *cod_submedio_vuelto, char *tipo_vuelto, int *datos_adicionales, 
				 int cod_medio, double importe, int origen_ventas, int *cod_restriccion_art,
                 int solicitar_submedio )
/*****************************************************************************/
{
    int rta = 0, found = 0;
	char consulta[80];

    /*---------------------------- Abre el archivo ---------------------------*/
    memset(consulta,0,sizeof(consulta));

	mutuales_sic = &mutuales;
	if( OPEN_TABLE(T_MUTUALES, TT_ORIG,( char* )mutuales_sic,sizeof( struct _t_mutuales ))== 0 ) {
        /*-------------------------- Prepara la pantalla -------------------------*/
		if( origen_ventas ) {
			//Verifico que exista submedios habilitados para venta (Tarea 5161)
			if( cod_medio != 0 ) {
				_snprintf(consulta, sizeof(consulta)-1,	
					"COD_MEDIO_PADRE = '%i' and HAB_VENTAS = 'S'", cod_medio );
				cod_medio_padre = cod_medio;
			} else {
				_snprintf(consulta, sizeof(consulta)-1,	
					"COD_MUTUAL = '%i' and HAB_VENTAS = 'S'",cod_medio );
			}
			found = RECCOUNT2( NULL, consulta);
		} else {
			if( cod_medio != 0 ){
				SET_WHERE("COD_MEDIO_PADRE = '%i'",cod_medio);
				RUN_QUERY(NO);
				cod_medio_padre = cod_medio;
			} else {
				SET_WHERE("COD_MUTUAL = '%i'",cod_medio);
				RUN_QUERY(NO);
			}
			found = FOUND2();
		}

		if( found ) {
            if( solicitar_submedio ) {
				venta = origen_ventas;
			    rta = Consulta_elegir_mutuales( _elegir_mutuales, importe, cod_medio  );
            } else {
                rta = 1; //se almacena todo en 1 - ver si causa problemas
            }
		} else {
			MENSAJE("NO SE ENCONTRARON MEDIOS ASOCIADOS O HABILITADOS");
		}

		if( rta ) {
			if( mutuales_sic->pide_supervisor == 'S' && origen_ventas ) {
				if( !( SOLICITAR_CLAVES( 30 - ancho_panel, 20, NO, SI, SI, "AUTORIZACION MEDIO",
					"AutorizÂ¢ mutual", _F_SUP_MEDIO_DE_PAGO, SI ) ) ) {
					rta = 0;
					MENSAJE( "NO SE AUTORIZA EL MEDIO" );
				}
			}
		}
        if( rta ) {
            rta = mutuales_sic->cod_mutual;
			if( mutuales_sic->moneda_extranjera == 'S' 
				|| mutuales_sic->moneda_extranjera == 's' ){
				//Log para robot
				GRABAR_LOG_SISTEMA( "Sub_Medio Seleccionado Moneda Extranjera" ,LOG_ROBOT,1);
			}
			if( mutuales_sic->id_registro && id_registro ) {
                *id_registro = mutuales_sic->id_registro;
            }
            if( cod_medio_vuelto != NULL ) {
				*cod_medio_vuelto = (( mutuales_sic->cod_medio_vuelto > 0 )? mutuales_sic->cod_medio_vuelto:0);
            }
            if( cod_submedio_vuelto != NULL ) {
				*cod_submedio_vuelto = (( mutuales_sic->cod_submedio_vuelto > 0 )? mutuales_sic->cod_submedio_vuelto : 0 );
            }
            if( tipo_vuelto != NULL ) {
                *tipo_vuelto = mutuales_sic->vuelto;
            }
            if( datos_adicionales != NULL ) {
                *datos_adicionales = mutuales_sic->cod_solicitud;
            }
			if( cod_restriccion_art != NULL ) {
				*cod_restriccion_art = mutuales_sic->cod_restriccion_art;
			}
        }
        CLOSE_TABLE( T_MUTUALES, TT_ORIG );
    }
    if( rta ) {
        if( mutuales_sic->autorizacion == 'S' && autorizacion ) {
            *autorizacion = PAGOS_PIDE_AUTORIZACION( ST( S_AUTORIZACION ) );
        }
        /*if( mutuales_sic->datos_adicionales == 'S' && datos_ad )
        {
            *datos_ad = PAGOS_PIDE_AUTORIZACION( ST( S_DOCUMENTO ) );
        }*/
    }
    BORRAR_MENSAJE();

	MOSTRAR_IMPORTE_MUTUAL( importe, rta);
    return ( rta );
}

/*****************************************************************************/
int _pide_cuotas_mutual( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0;
    switch( comando ) {
        case _BUSCAR:
            ind_cuo_mut.nro_orden = atoi( cadena );
			SET_WHERE(/*"CONCAT(COD_MUTUAL,NRO_ORDEN) >= CONCAT(%i,%i)"*/
				"(COD_MUTUAL = %i and NRO_ORDEN >= %i) or COD_MUTUAL > %i",
						ind_cuo_mut.cod_mutual,
						ind_cuo_mut.nro_orden,
						ind_cuo_mut.cod_mutual);
            //GET_GREATER_E( ( char* )&ind_cuo_mut );
            //if( cuo_mut.cod_mutual != ind_cuo_mut.cod_mutual ) {
                //GO_CUO_MUT( BOTTOM );
            //}
            b->pos = 0;
            cadena[0] = ' ';
            break;
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[20];
                _itoa( cuo_mut.nro_orden, xx, 10 );
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, cuo_mut.descripcion );
            }
            #endif
            MOSTRAR_ENTERO( 30 - ancho_panel / 2, 8 + dato, "9", cuo_mut.nro_orden );
            MOSTRAR_CADENA( 33 - ancho_panel / 2, 8 + dato, cuo_mut.descripcion );
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
            if( !rta && cuo_mut.cod_mutual != ind_cuo_mut.cod_mutual ) {
                //if( dato > 0 ) {
                    //GO_CUO_MUT( BOTTOM );
                //}
                //else {
                    //GO_CUO_MUT( TOP );
                //}
                rta = 1;
            }
            break;
        case _VALIDAR:
            if( isdigit( dato ) ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
        case _COMPARAR:
            rta = 1;
            break;
        case _TECLA:
            if( dato == -73 || dato == -81 ) {
                b->tecla = 0;
            }
            break;
    }
    return ( rta );
}
/*****************************************************************************/
int PIDE_CUOTAS_MUTUAL( int codigo )
/*****************************************************************************/
{
    int rta = 0;
    /*---------------------------- Abre el archivo ---------------------------*/
    if( /*USE_DB( AREA_AUX, _CUO_MUT_SIC, ( char* )&cuo_mut, sizeof( cuo_mut ), NULL, 0, 0 )*/
		OPEN_TABLE(T_CUO_MUT, TT_ORIG,( char* )&cuo_mut, sizeof( cuo_mut ))== 0 ) {
		SELECT_TABLE( T_CUO_MUT, TT_ORIG );
        //SET_ORDER( 2 );
        ind_cuo_mut.cod_mutual = (char)codigo;
        //GO_CUO_MUT( TOP );
        rta = Consulta_pide_cuotas_mutual( _pide_cuotas_mutual );
        if( rta ) {
            rta = cuo_mut.nro_cuota;
        }
        CLOSE_TABLE( T_CUO_MUT, TT_ORIG );
    }
    BORRAR_MENSAJE();
    return ( rta );
}
/*****************************************************************************
void GO_CUO_MUT( int modo )
/*****************************************************************************
{
    if( modo == TOP ) {
        ind_cuo_mut.nro_orden = 0;
        //GET_GREATER_E( ( char* )&ind_cuo_mut );
		SET_WHERE("COD_MUTUAL >= '%i'", ind_cuo_mut.cod_mutual);
    }
    else if( modo == BOTTOM ) {
        ind_cuo_mut.nro_orden = 99;
        //GET_GREATER_E( ( char* )&ind_cuo_mut );
		SET_WHERE("COD_MUTUAL >= '%i'", ind_cuo_mut.cod_mutual);
        SKIP2( -1 );		
    }
	RUN_QUERY();
}*/

#ifdef COMPILAR_TECLA_REPROCESAR
/*****************************************************************************/
void REPROCESAR_MUTUALES( char tipo_pago )
/*****************************************************************************/
{
    int h;
    for( h = 0;h < RAM_P_PAGO;h++ ) {
        if( medios[_PAGO_MODO( h )].mutual ) {
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
void IMPRIME_DATOS_MUTUAL( char nro )
/*****************************************************************************/
{
    //struct _t_mutuales mutuales;
    if( /*USE_DB( AREA_AUX, _mutuales, ( char* )&mutuales, sizeof( mutuales ), NULL, 0, 0 )*/
		OPEN_TABLE(T_MUTUALES, TT_ORIG,( char* )&mutuales,sizeof( mutuales ))== 0 ) {
		//SELECT_TABLE( T_MUTUALES, TT_ORIG );
		SET_WHERE("COD_MUTUAL = '%i'",nro);
		RUN_QUERY(NO);
        //GET_EQUAL( &nro );
        MOSTRAR_CADENA( 21, PRN, mutuales.des_mutual );
        MOSTRAR_CADENA( 33, PRN, "(" );
        MOSTRAR_ENTERO( 34, PRN, "00", mutuales.cod_mutual );
        MOSTRAR_CADENA( 36, PRN, ")" );
    }
    CLOSE_TABLE( T_MUTUALES, TT_ORIG );
}
/*****************************************************************************/
int INGRESAR_MUTUAL()
/*****************************************************************************/
{
    int rta = 0;
    SET_MEMORY_INT( __mutual_codigo, PIDE_MUTUAL( NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0.0, SI, NULL, SI ) );
    if( MUTUAL_CODIGO ) {
        SET_MEMORY_INT( __mutual_cuotas, PIDE_CUOTAS_MUTUAL( MUTUAL_CODIGO ) );
        if( MUTUAL_CUOTAS ) {
            //          SET_MEMORY_LONG( __mutual_orden, BOX_PEDIR_ENTERO( 0, 21, " N DE ORDEN ", 9, SI,
            //                  52 ) );
            SET_MEMORY_LONG( __mutual_orden, PEDIR_ENTERO_EN_RECUADRO( " N DE ORDEN ", SI, 9 ) );
            if( MUTUAL_ORDEN > 0 ) {
                SET_MEMORY_DOUBLE( __mutual_recargo, cuo_mut.rec_cuota );
                rta = 1;
            }
        }
    }
    if( !rta ) {
        CEREAR_STRUCT( _mutual );
    }
    MUESTRA_MUTUAL();
    return ( rta );
}

/*****************************************************************************/
int CONTROLA_IMPORTE_MUTUAL( char marca, char *numero_bono, double importe )
/*****************************************************************************/
{
    int rta = 1, ok = 0;
    //struct _t_mutuales mutuales;
    char tipo_boletin = 0;
    int nro_archivo;
    /*---------------------------- Abre el archivo ---------------------------*/
    mutuales_sic = &mutuales;
    if( /*USE_DB( AREA_AUX, _mutuales, ( char* )mutuales_sic, sizeof( struct _t_mutuales ),
                NULL, 0, 0 )*/
		OPEN_TABLE(T_MUTUALES, TT_ORIG,( char* )mutuales_sic,sizeof( struct _t_mutuales ))== 0 ) {
        //GET_EQUAL( &marca );
		//SELECT_TABLE( T_MUTUALES, TT_ORIG );
		SET_WHERE("COD_MUTUAL = '%i'",marca);
		RUN_QUERY(NO);
        if( FOUND2() ) {
            if( rta && CONTROLA_IMPORTE_MEDIOS ) {
                /*
                02/12/2004-- fabiana -- se comenta para poder igualar la base de datos
                if( importe > mutuales_sic->pago_imp_maximo ) {
                   rta = 0;
                   MENSAJE_STRING( S_EL_IMPORTE_EXCEDE_MAXIMO_PERMITIDO );
                }
                if( importe < mutuales_sic->pago_imp_minimo ) {
                   rta = 0;
                   MENSAJE_STRING( S_EL_IMPORTE_NO_LLEGA_MINIMO_EXIGIDO );
                }
                */
            }
            if( config.pais == COLOMBIA ) {
                tipo_boletin = mutuales_sic->tipo_boletin;
                nro_archivo = mutuales_sic->nro_archivo;
                if( rta && tipo_boletin ) {
                    if( PIDE_NRO_BONO( numero_bono ) ) {
                        ok = BUSCAR_MEDIO_EN_BOLETIN( tipo_boletin, nro_archivo, numero_bono,
                                                      &importe, SI, SI, SI );
                    }
                    if( ok <= 0 ) {
                        rta = 0;
                    }
                }
            }
        }
        CLOSE_TABLE( T_MUTUALES, TT_ORIG );
    }
    return ( rta );
}
/*****************************************************************************/
void NOMBRE_MUTUAL( char cod_mutual, char *cadena )
/*****************************************************************************/
{
    //struct _t_mutuales mutuales;
    char codigo;
    strcpy( cadena, "DESCONOCIDA" );
    codigo = cod_mutual;
    if( /*USE_DB( AREA_AUX3, _mutuales, ( char* )&mutual, sizeof( struct _t_mutuales ), NULL, 0,
                0 )*/
		OPEN_TABLE(T_MUTUALES, TT_ORIG,( char* )&mutuales,sizeof( struct _t_mutuales ))== 0 ) {
		//SELECT_TABLE( T_MUTUALES, TT_ORIG );
		SET_WHERE("COD_MUTUAL = '%i'",codigo);
		RUN_QUERY(NO);
        //GET_EQUAL( &codigo );
        if( FOUND2() ) {
            strcpy( cadena, mutuales.des_mutual );
        }
        else {
            MENSAJE( "ERR2" );
        }
        cadena[11] = 0;
        CLOSE_TABLE( T_MUTUALES, TT_ORIG );
    }
}
/*****************************************************************************/
int COD_VALIDA_MUTUAL( char marca )
/*****************************************************************************/
{
	int       rta = 0;
    //struct _t_mutuales mutuales;
   //---------------------------- Abre el archivo ---------------------------//
	mutuales_sic = &mutuales;
   if( /*USE_DB( AREA_AUX, _mutuales, ( char *) mutuales_sic, sizeof( struct _mutuales ),
			NULL, 0, 0 )*/
		OPEN_TABLE(T_MUTUALES, TT_ORIG,( char* )mutuales_sic,sizeof( struct _t_mutuales ))== 0 ) {
		//SELECT_TABLE( T_MUTUALES, TT_ORIG );
		SET_WHERE("COD_MUTUAL = '%i'",marca);
		RUN_QUERY(NO);
		//GET_EQUAL( &marca );
		if( FOUND2(  ) ) {
			rta = mutuales_sic->cod_valida;
		}
		CLOSE_TABLE( T_MUTUALES, TT_ORIG );
	}
	return ( rta );
}
/*****************************************************************************/
double GET_PORC_FROM_VUELTOS_SIC( char medio, char submedio, char medio_vuelto, char submedio_vuelto  )
/*****************************************************************************/
{
    double       rta = 0.0;
	struct _indice indice;
	int  area_ant, tipo_ant;
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
   //---------------------------- Abre el archivo ---------------------------//
  // if( USE_DB( AREA_VUELTOS, _VUELTOS, ( char *) &vueltos_sic, sizeof( struct _vueltos_x_medio ),
  //			NULL, 0, 0 ) == 0 ) {
	if( OPEN_TABLE( T_VUELTOS, TT_ORIG, ( char* )&vueltos_sic, sizeof( struct _vueltos_x_medio ) ) == 0 ) {
	    indice.cod_medio = medio;
	    indice.cod_submedio = submedio;
       // GET_EQUAL( (char *) &indice );
		SELECT_TABLE( T_VUELTOS, TT_ORIG );
		SET_WHERE("COD_MEDIO  = '%d' and COD_SUBMEDIO = '%d'", indice.cod_medio, indice.cod_submedio );
		RUN_QUERY(NO);
	   // if( FOUND(  ) ) {
		 if( FOUND2(  ) ) {
			while( !dbEOF() && medio == vueltos_sic.cod_medio && submedio == vueltos_sic.cod_submedio ) {
				if( medio_vuelto == vueltos_sic.cod_medio_vuelto && submedio_vuelto == vueltos_sic.cod_submedio_vuelto ) {
					rta = vueltos_sic.porc_vuelto;
					break;
				}
				//SKIP( 1 );
				SKIP2( 1 );
			}
	    }
	    //CLOSE_DATABASE( AREA_AUX );
	    CLOSE_TABLE( T_VUELTOS, TT_ORIG );
	}
	SELECT_TABLE( area_ant, tipo_ant );
	return ( rta );
}
/*****************************************************************************/
double GET_MAX_VUELTO_FROM_VUELTOS_SIC( char medio, char submedio, char medio_vuelto, char submedio_vuelto )
/*****************************************************************************/
{
    double       rta = 0.0;
	struct _indice indice;
	int  area_ant, tipo_ant;
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
   //---------------------------- Abre el archivo ---------------------------//
    //if( USE_DB( AREA_VUELTOS, _VUELTOS, ( char *) &vueltos_sic, sizeof( struct _vueltos_x_medio ),
	//		NULL, 0, 0 ) == 0 ) {
	if( OPEN_TABLE( T_VUELTOS, TT_ORIG, ( char* )&vueltos_sic, sizeof( struct _vueltos_x_medio ) ) == 0 ) {
	    indice.cod_medio = medio;
	    indice.cod_submedio = submedio;
      //  GET_EQUAL( (char *) &indice );
		SELECT_TABLE( T_VUELTOS, TT_ORIG );
		SET_WHERE("COD_MEDIO  = '%d' and COD_SUBMEDIO = '%d'", indice.cod_medio, indice.cod_submedio );
		RUN_QUERY(NO);
	    //if( FOUND(  ) ) {
		if( FOUND2(  ) ) {
			while( !dbEOF() && medio == vueltos_sic.cod_medio && submedio == vueltos_sic.cod_submedio ) {
				if( medio_vuelto == vueltos_sic.cod_medio_vuelto && submedio_vuelto == vueltos_sic.cod_submedio_vuelto ) {
					rta = vueltos_sic.max_vuelto;
					break;
				}
				//SKIP( 1 );
				SKIP2( 1 );
			}
	    }
	    //CLOSE_DATABASE( AREA_AUX );
		CLOSE_TABLE( T_VUELTOS, TT_ORIG );
	}
	SELECT_TABLE( area_ant, tipo_ant );
	return ( rta );
}
/***************************************************************************************/
void COD_VUELTO_MEDIO( char medio, char submedio, int *vuelto_medio, int *vuelto_submedio )
/***************************************************************************************/
{
	struct _indice indice;
	int  area_ant, tipo_ant;
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();


   //---------------------------- Abre el archivo ---------------------------//
   //if( USE_DB( AREA_VUELTOS, _VUELTOS, ( char *) &vueltos_sic, sizeof( struct _vueltos_x_medio ),
//		NULL, 0, 0 ) == 0 ) {
	if( OPEN_TABLE( T_VUELTOS, TT_ORIG, ( char* )&vueltos_sic, sizeof( struct _vueltos_x_medio ) ) == 0 ) {
	    indice.cod_medio = medio;
	    indice.cod_submedio = submedio;
        //GET_EQUAL( (char *) &indice );
		SELECT_TABLE( T_VUELTOS, TT_ORIG );
		SET_WHERE("COD_MEDIO  = '%d' and COD_SUBMEDIO = '%d'", indice.cod_medio, indice.cod_submedio );
		RUN_QUERY(NO);
	    //if( FOUND(  ) ) {
		if( FOUND2(  ) ) {
			*vuelto_medio = vueltos_sic.cod_medio_vuelto;
			*vuelto_submedio = vueltos_sic.cod_submedio_vuelto;
	    }
	    //CLOSE_DATABASE( AREA_VUELTOS );
		CLOSE_TABLE( T_VUELTOS, TT_ORIG );
	}
	
	SELECT_TABLE( area_ant, tipo_ant );
}

/*****************************************************************************/
int COD_VALIDA_VUELTO( char medio, char submedio )
/*****************************************************************************/
{
    int       rta = 0;
	struct _indice indice;
	int  area_ant, tipo_ant;
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
   //---------------------------- Abre el archivo ---------------------------//
    // if( USE_DB( AREA_VUELTOS, _VUELTOS, ( char *) &vueltos_sic, sizeof( struct _vueltos_x_medio ),
	//	NULL, 0, 0 ) == 0 ) {
	if( OPEN_TABLE( T_VUELTOS, TT_ORIG, ( char* )&vueltos_sic, sizeof( struct _vueltos_x_medio ) ) == 0 ) {		
	    indice.cod_medio = medio;
	    indice.cod_submedio = submedio;
		SELECT_TABLE( T_VUELTOS, TT_ORIG );
		SET_WHERE("COD_MEDIO  = '%d' and COD_SUBMEDIO = '%d'", indice.cod_medio, indice.cod_submedio );
		RUN_QUERY(NO);
	    //if( FOUND(  ) ) {
		if( FOUND2(  ) ) {
		    rta = vueltos_sic.cod_val_vuelto;
	    }
	    //CLOSE_DATABASE( AREA_VUELTOS );
		CLOSE_TABLE( T_VUELTOS, TT_ORIG );
	}
	
	SELECT_TABLE( area_ant, tipo_ant );
	return ( rta );
}
/*****************************************************************************/
long PIDE_NRO_VALE_MUTUAL()
/*****************************************************************************/
{
    long nro = 0;
    if( PEDIR_NUMERO_DE_VALE ) {
        nro = PEDIR_GRAN_ENTERO( ( 80 - ancho_panel - 16 ) / 2, 23, 8 );
        BORRAR_MENSAJE();
    }
    return ( nro );
}
/*****************************************************************************/
char COTIZACION_MUTUAL( char cod_mutual, double *cotizacion, char *simbolo_me )
/*****************************************************************************/
{
    //struct _t_mutuales mutuales;
   // double cotizacion = 0.00;
	char codigo, rta = 0;
    //strcpy( cadena, "DESCONOCIDA" );
    codigo = cod_mutual;
    if( /*USE_DB( AREA_AUX3, _mutuales, ( char* )&mutual, sizeof( struct _mutuales ), NULL, 0,
                0 )*/
		OPEN_TABLE(T_MUTUALES, TT_ORIG,( char* )&mutuales,sizeof( struct _t_mutuales ))== 0 ) {
		//SELECT_TABLE( T_MUTUALES, TT_ORIG );
		SET_WHERE("COD_MUTUAL = '%i'",codigo);
		RUN_QUERY(NO);
        //GET_EQUAL( &codigo );
        if( FOUND2() ) {
			if( mutuales.moneda_extranjera == 'S' ) {
				*cotizacion = mutuales.cotizacion;
				if( simbolo_me != NULL ) {
					strncpy( simbolo_me, mutuales.simbolo_moneda, sizeof( simbolo_me ) );
				}
				rta = 1;
			}
        }
        else {
            MENSAJE( "ERR2" );
        }
        CLOSE_TABLE( T_MUTUALES, TT_ORIG );
    }
	return rta;
}


/*****************************************************************************/
int VALIDA_MUTUAL( char marca )
/*****************************************************************************/
{
	int       rta = 0;
    //struct _t_mutuales mutuales;

   //---------------------------- Abre el archivo ---------------------------//
   //if( USE_DB( AREA_AUX, _mutuales, ( char *) &mutual, sizeof( struct _mutuales ), NULL, 0, 0 ) == 0 ) {
   if( OPEN_TABLE( T_MUTUALES, TT_ORIG, ( char* )&mutuales, sizeof( struct _t_mutuales ) ) ) {
		//GET_EQUAL( &marca );
		SET_WHERE("COD_MUTUAL = '%i'", marca );
		RUN_QUERY(NO);
		if( FOUND2(  ) ) {
			if( mutuales.exclusivo_promocion == 'S' ) {
				rta = 1;
			}
		}
		//CLOSE_DATABASE( AREA_AUX );
		CLOSE_TABLE( T_MUTUALES, TT_ORIG );
	}

	return ( rta );
}


#endif

