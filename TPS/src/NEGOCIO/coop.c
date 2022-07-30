#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <archivos.h> 
//#include <browse.h> 
#include <btrv.h> 
#include <b_mens.h> 
#include <cr_disco.h>
#include <cio.h> 
#include <clave.h> 
#include <clientes.h> 
#include <cmemory.h> 
#include <coop.h> 
#include <cr.h>  
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <get_fech.h> 
#include <log.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <panel.h> 
#include <pant.h> 
#include <perfiles.h> 
#include <stringt.h> 
#include <tkt.h> 
#include <_cr1.h> 
#include <aarch.h>
#include <ctype.h>
#include <bridge.h>
#include <db.h>
#include <deftables.h>
#include <ini.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W)
#include <dos.h>
extern  int ES_IMPRIMIBLE(int dato);
#endif
extern long BUSCAR_CLIENTE_COD_BARRA();
extern int CLIENTE_HABILITADO_PARA_CHEQUES( void  );
/***************************************************************************/
int ELEGIR_CLIENTE_NUMERICO( int cheque, int mostrar )
/***************************************************************************/
{
    int ok = 0;
    int recuadro;
    int _rta = 0;
	if( FACTURACION_HABILITADA || PEDIR_CLIENTE_EN_CHEQUES ) {
        ok = 1;
        if( !archivo_cliente_abierto ) {
            if( !ABRIR_ARCHIVO_CLIENTE() ) {
                ok = 0;
            }
        }
        if( ok ) {
            if( mostrar ) {
                recuadro = 1;
            }
            else {
                recuadro = 0;
            }
			if(config_tps.activarTFpermanente == 1) {
				SET_MEMORY_LONG( __ram_nro_cliente, 0 ); //para que permita buscarlo
			}

            if( RAM_NRO_CLIENTE ) {
                _rta = 1;
            }
            if( !_rta && PEDIR_NRO_CLIENTE( recuadro, NO ) > 0 ) {
                if( !cheque ) {
                    _ELEGIR_CLIENTE();
                }
                else {
					if( cheque && CLIENTE_HABILITADO_PARA_CHEQUES() == NO ){
                    //if( cheque && clientes.cheques == 'N' ) {
                        MENSAJE_STRING( S_CLIENTE_NO_HABILITADO_PARA_TRABAJAR_CHEQUES );
                        SET_MEMORY_LONG( __ram_nro_cliente, 0 );
                        CEREAR_PERFIL_CLIENTE();
                        ok = 0;
                    }
                }
            }
            else {
                if( cheque
                 && SOLICITAR_CLAVES( 30 - ancho_panel / 2, 21, NO, SI, SI,
                                      ST( S__CLIENTE_INEXISTENTE_ ),
                                      ST( S_CHEQUE_DE_CLIENTE_INEXISTENTE ), RAM_COD_FUNCION, SI ) ) {
                    SET_MEMORY_LONG( __ram_nro_cliente, 1 );
                }
                else {
                    ok = 0;
                }
            }
            // if(mostrar)
            MUESTRA_CLIENTE( cheque );
			if( RAM_P_TICKET == 0) {
				if(config_tps.activarTFpermanente == 1) {
					int iva = OBTENER_COND_IVA_CLIENTE( 0);
					if( iva ) {
						if( iva < 2 ) {
							SET_MEMORY_CHAR( __ram_ticket_factura, FACTURA_TIPO_A ); 
						} else {
							SET_MEMORY_CHAR( __ram_ticket_factura, FACTURA_TIPO_B);
						}
					}
				}
			}
            MUESTRA_PERFIL();
        }
    }
    else {
        MENSAJE_STRING( S_NO_ESTA_HABILITADA_FACTURACION_CHEQUES_CLIE );
    }
    if( ok == 0 ) {
		if( RAM_P_TICKET == 0) {
			if(config_tps.activarTFpermanente == 1) {
				MUESTRA_COMPROBANTE();
			}
		}else  {
			SET_MEMORY_LONG( __ram_nro_cliente, 0L );
			CEREAR_PERFIL_CLIENTE();
		 }
    }
    if( !ok ) {
        MUESTRA_CLIENTE( 0 );
    }
    else {
        if( MODO_DEBUG == 15 || MODO_DEBUG == 50 ) {
            char cad_cli[50];
            memset( cad_cli, 0, sizeof( cad_cli ) );
            _snprintf(cad_cli, sizeof(cad_cli)-1, "Cli: %li Desc: %5.2f ", clientes.cod_cliente, clientes.porc_desc );
            GRABAR_LOG_SISTEMA( cad_cli , LOG_DEBUG,4);
        }
    }
    return ok;
}
/*****************************************************************************/
void MOSTRAR_DATOS_CLIENTE( int y, int vacio )
/*****************************************************************************/
{
    if( !vacio ) {
        MOSTRAR_CADENA( 22 - ancho_panel / 2, 4 + y, clientes.nombre );
        MOSTRAR_ENTERO( 53 - ancho_panel / 2, 4 + y, "00000000", clientes.cod_cliente );
    }
}
/*****************************************************************************/
int _consulta_cliente( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0 /*i, cadlen,encontrado,index*/;
    char xx[20];
	//char* ax;
    //   char regexp[31];
    //   char* aux;
    static long pos;
    int salto = 0;
    switch( comando ) {
        case _BUSCAR:
            MOSTRAR_CADENA( 21 - ancho_panel / 2, 21, cadena );
            //GET_GREATER_E( cadena )			
			if(cadena){
				//if(strlen(cadena)>0){
					/*ax = (char*)malloc(strlen(cadena)+1);
					strcpy(ax,cadena);
					ax[strlen(ax)-1] = ax[strlen(ax)-1] + 1;*/
					SET_WHERE("NOMBRE LIKE '%s%%'",cadena);
					//free(ax);
				//}else{
					//SET_WHERE("");
				//}
				SET_ORDER2( "NOMBRE" );
				RUN_QUERY(NO);
			}            
			break;
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            setFieldValuepChar( b->index, 0, clientes.nombre );
            _ltoa( clientes.cod_cliente, xx, 10 );
            setFieldValuepChar( b->index, 1, xx );
            setFieldValuepChar( b->index, 2, clientes.cuit );
            setFieldValuepChar( b->index, 3, clientes.domicilio );
            setFieldValuepChar( b->index, 4, clientes.localidad );
            #else
            MOSTRAR_DATOS_CLIENTE( dato,dbEOF() );
            #endif
            break;
        case _VACIO:
            MOSTRAR_DATOS_CLIENTE( dato, SI );
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
            break;
        case _VALIDAR:
            if( ES_IMPRIMIBLE( dato ) ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? BOF() : dbBOF();
            break;
        case _ENTER:
            rta = 1;
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
/***************************************************************************/
int ELEGIR_CLIENTE_ALFABETICO( int cheque, int validar_cliente )
/***************************************************************************/
{
    int rta = 1;
	int datos_invalidos = 0;

	if( FACTURACION_HABILITADA || PEDIR_CLIENTE_EN_CHEQUES ) {
        /*-------------------- Prepara la pantalla -----------------------*/
        if( !archivo_cliente_abierto ) {
            if( !ABRIR_ARCHIVO_CLIENTE() ) {
                rta = 0;
            }
        }
        if( rta ) {
			/*if( !OPENED( T_CLIENTES, TT_ORIG ) ) {
				OPEN_TABLE( T_CLIENTES, TT_ORIG, ( char* )&clientes, sizeof( struct _clientes ) ); 
			}*/
			SELECT_TABLE( T_CLIENTES, TT_ORIG );
			TECLADO_ALFABETICO = 1;
			rta = Consulta_cliente( _consulta_cliente );
			//   rta1 = rta;
			//  rta  = 0;   //para no cargar los perfiles y demas
			TECLADO_ALFABETICO = 0;
			if( rta ) {
				rta = VALIDA_CLIENTE();
			}
			if( rta ) {
				SET_MEMORY_LONG( __ram_nro_cliente, clientes.cod_cliente );
				SET_MEMORY( __cliente_afinidad_nombre, clientes.nombre );
				CARGAR_PERFIL_CLIENTE( ( int )clientes.cod_perfil );
				if( !cheque ) {
					_ELEGIR_CLIENTE();
				}
			}
			else {
				rta = 0;
			}
            ClearPanel( pPanel );
		}
    }
    else {
        MENSAJE_STRING( S_NO_ESTA_HABILITADA_LA_FACTURACION );
    }

	if( validar_cliente && rta && !FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
		datos_invalidos = !VALIDAR_DATOS_CLIENTE_FISCAL(  );
		if( datos_invalidos )
			rta = 0;
	} 
    if( !rta ) {
		if( RAM_P_TICKET == 0) 
			if(config_tps.activarTFpermanente == 0) 
				SET_MEMORY_LONG( __ram_nro_cliente, 0 );
        CEREAR_PERFIL_CLIENTE();
    }
    if( MODO_NEGOCIO != SERVICIOS ) {
        MUESTRA_CLIENTE( cheque );
        MUESTRA_PERFIL();
		//aqui poner que tome el tipo de comprobante real
		if( RAM_P_TICKET == 0) {
			if(config_tps.activarTFpermanente == 1 && RAM_NOTA_CR == 0 ) {
				int iva = OBTENER_COND_IVA_CLIENTE( 0);
				if( iva ) {
					if( iva < 2 ) {
						SET_MEMORY_CHAR( __ram_ticket_factura, FACTURA_TIPO_A ); 
					} else {
						SET_MEMORY_CHAR( __ram_ticket_factura, FACTURA_TIPO_B);
					}
				}
			}
		}
        MUESTRA_COMPROBANTE();
    }
    return 1;
}
/***************************************************************************/
int _ELEGIR_CLIENTE()
/***************************************************************************/
{
    int rta = 0, cod_cuota = 0;
    float recargo = 0;
    /*--------------- Pedimos codigo de financiacion --------------*/
    if( clientes.ctacte == 'S' ) {
        cod_cuota = PEDIR_COD_FINANCIACION( &recargo );
    }
    if( cod_cuota >= 0 ) {
        SET_MEMORY( __nro_cod_cuota, ( char* )&cod_cuota );
        SET_MEMORY_FLOAT( __nro_recargo_financiacion, recargo );
        rta = 1;
    }
    /*  if( rta && cod_cuota != 0 ) {
            rta = COOP_VERIFICA_CLIENTE_HABILITADO(  );
        }
        if( !rta ) {
            SET_MEMORY_LONG( __ram_nro_cliente, 0 );
            CEREAR_PERFIL_CLIENTE( );
            cod_cuota = 0;
            recargo = 0;
            SET_MEMORY( __nro_cod_cuota, ( char * ) &cod_cuota );
            SET_MEMORY_FLOAT( __nro_recargo_financiacion, recargo );
        }*/
    return ( rta );
}
/*****************************************************************************/
int PEDIR_COD_FINANCIACION( float *recargo )
/*****************************************************************************/
{
    int nro = 0;
    struct _cuo_cta cuo_cta;
    /*-------------- Prepara archivo --------------------*/
    if( PEDIR_CODIGO_DE_FINANCIACION ) {
        nro = 99;
        //USE_DB( AREA_AUX, _CUO_CTA_SIC, ( char* )&cuo_cta, sizeof( cuo_cta ), NULL, 0, 0 );
		OPEN_TABLE(T_CUO_CTA, TT_ORIG ,( char* )&cuo_cta, sizeof( cuo_cta ));
        while( nro == 99 ) {
            if( !COD_FINANCIACION_POR_DEFECTO ) {
                nro = ( int )PEDIR_ENTERO_EN_RECUADRO( ST( S__COD_FIN_ ), SI, 2 );
            }
            else {
                nro = COD_FINANCIACION_POR_DEFECTO;
            }
            if( nro > 0 ) {
				SELECT_TABLE( T_CUO_CTA, TT_ORIG );
				SET_WHERE("CODIGO = '%i'",nro);
				RUN_QUERY(NO);
                //GET_EQUAL( ( char* )&nro );
                if( FOUND2() && cuo_cta.habilitada == 'S' ) {
                    nro = cuo_cta.codigo;
                    *recargo = cuo_cta.recargo;
                }
                else {
                    if( COD_FINANCIACION_POR_DEFECTO ) {
                        nro = 0;
                    }
                    else {
                        MENSAJE_STRING( S_CODIGO_INEXISTENTE );
                        nro = 99;
                    }
                }
            }
            else if( !nro ) {
                *recargo = 0.0;
            }
        }
        /*------------------- Vuelve -------------------*/
        CLOSE_TABLE( T_CUO_CTA, TT_ORIG );
        BORRAR_MENSAJE();
    }
    return ( nro );
}
/*****************************************************************************/
void ACTUALIZA_SOCIOS( double importe )
/*****************************************************************************/
{
    struct _mov_cta mov_cta;
    if( GENERAR_ARCHIVO_DE_SOCIOS && !RAM_FACTURA && NRO_COD_CUOTA ) {
        if( /*USE_DB( AREA_AUX, _MOV_CTA_SIC, ( char* )&mov_cta, sizeof( mov_cta ), NULL, 0, 0 )*/
         OPEN_TABLE(T_MOV_CTA, TT_ORIG ,( char* )&mov_cta, sizeof( mov_cta ))== 0 ) {
			SELECT_TABLE( T_MOV_CTA, TT_ORIG );
            mov_cta.fecha = GET_FECHA_DOS();
            mov_cta.nro_socio = RAM_NRO_CLIENTE;
            mov_cta.nro_cajero = NRO_CAJERO;
            mov_cta.nro_comprobante = NRO_TICKET;
            mov_cta.importe = importe;
            mov_cta.cod_financiacion = NRO_COD_CUOTA;
            #ifdef INVEL_L
            //BEGIN_TRANSACTION();
            #endif
            //INSERT();
			INSERT2(SI);
            #ifdef INVEL_L
            //END_TRANSACTION();
            #endif
            //CLOSE_DATABASE( AREA_AUX );
			CLOSE_TABLE( T_MOV_CTA, TT_ORIG );
        }
    }
}
/*****************************************************************************/
void COOP_FACTURA_SOCIOS()
/*****************************************************************************/
{
    SET_MEMORY_CHAR( __ram_factura, !( RAM_FACTURA ) );
    SET_MEMORY_CHAR( __ram_nota_cr, 0 );
    if( RAM_FACTURA && PEDIR_OC_GD ) {
        SET_MEMORY_CHAR( __ram_factura, PIDE_OC_GD() );
    }
    MUESTRA_COMPROBANTE();
}
/*****************************************************************************/
int COOP_VERIFICA_CLIENTE_HABILITADO()
/*****************************************************************************/
{
    int rta = 1;
    if( clientes.ctacte == 'N' ) {
        MENSAJE_STRING( S_CLIENTE_NO_HABILITADO_PARA_CUENTA_CORRIENTE );
        rta = 0;
    }
    return ( rta );
}
/*****************************************************************************/
int PIDE_OC_GD()
/*****************************************************************************/
{
    int rta;
    char aux[11];
    rta = PEDIR_CADENA_EN_RECUADRO( ST( S__OC___GD_ ), SI, 10, aux, NULL );
    if( rta ) {
        SET_MEMORY( __ram_oc_gd, aux );
    }
    return ( rta );
}
/*****************************************************************************/
int ELEGIR_CLIENTE_COD_BARRA( int cheque )
/*****************************************************************************/
{
    int rta = 1;
    long nro = 0;
    if( FACTURACION_HABILITADA || PEDIR_CLIENTE_EN_CHEQUES ) {
        /*-------------------- Prepara la pantalla -----------------------*/
        if( rta ) {
            nro = BUSCAR_CLIENTE_COD_BARRA();
            if( nro > 0 ) {
                rta = EXISTE_CLIENTE( nro );
                if(!rta){
                    MENSAJE_STRING( S_EL_CLIENTE_NO_EXISTE );
                    BORRAR_MENSAJE();
                }
            }else{
                rta = 0;
            }
            TECLADO_ALFABETICO = 0;
            if( rta ) {
                rta = VALIDA_CLIENTE();
            }
            /*--------------------- finaliza ---------------------*/
            if( rta ) {
                SET_MEMORY_LONG( __ram_nro_cliente, clientes.cod_cliente );
                SET_MEMORY( __cliente_afinidad_nombre, clientes.nombre );
                CARGAR_PERFIL_CLIENTE( ( int )clientes.cod_perfil );
                if( !cheque ) {
                    _ELEGIR_CLIENTE();
                }
            }
            else {
                rta = 0;
            }
            ClearPanel( pPanel );
        }
    }
    else {
        MENSAJE_STRING( S_NO_ESTA_HABILITADA_LA_FACTURACION );
    }
    if( !rta ) {
        SET_MEMORY_LONG( __ram_nro_cliente, 0 );
        CEREAR_PERFIL_CLIENTE();
    }
    if( MODO_NEGOCIO != SERVICIOS ) {
        MUESTRA_CLIENTE( cheque );
        MUESTRA_PERFIL();
        MUESTRA_COMPROBANTE();
    }
    return 1;
}

