#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <actual.h> 
#include <btrv.h> 
#include <cio.h> 
#include <clave.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <cr_disco.h> 
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <descuent.h> 
#include <eventos.h> 
#include <ext_tick.h> 
#include <imp_item.h> 
#include <imp_tick.h> 
#include <log.h> 
#include <malloc.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <pant.h> 
#include <print.h> 
#include <recup.h> 
#include <round.h> 
#include <stringt.h> 
#include <tkt.h> 
#include <trim.h> 
#include <t_pago.h> 
#include <_cr1.h> 
#include <actions.h>
#include <typeacti.h>
#ifdef INVEL_W
#include <io.h>
#else
#include <sys/io.h>
#endif
#include <math.h>
#include <aarch.h>
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef INVEL_W
#endif
#ifdef INVEL_L
#endif
#if defined(INVEL_W) || defined(INVEL_L)
#endif
static int _AGREGAR_DESCUENTO( long cod_art, int cod_iva, struct _acu_iva *acum, double porc,
                               char origen, int tipo, double imp_descuento, double porc_original );
struct _acu_iva *CALCULAR_IMPORTE_IMPUESTOS( struct _acu_iva *acum, double porc );
static int filtro_origen_articulo = 0;
static int cod_concepto = 0;
static int por_defecto = 0;
struct _motivos *p_motivo_sic;
static double tot_bruto = 0.00, tot_neto = 0.00;
static int desc_on[] = {0,0,0,0,0};
#define ACUMULAR_EXENTOS   0x1
#define ACUMULAR_IVA       0x2
#define ACUMULAR_ILA_IHA   0x8
struct _ind_motivo
{
    INT16 mostrar;
    INT16 cod_motivo;
};
#ifdef COMPILAR_DESCUENTOS
/*****************************************************************************/
void ACUMULA_IMPUESTOS_ITEM( int h, struct _acu_iva *acu, double cantidad, int flags_impuestos,
                             int cod_iva )
/*****************************************************************************/
{
    double recargo_neto,imp_iva1,imp_iva2,imp[4];
    int i;
    imp_iva1 = _TICKET_IVA1_PRECIO( h ) + _TICKET_IVA1_REC( h );
    imp_iva2 = _TICKET_IVA2_PRECIO( h ) + _TICKET_IVA2_REC( h );
    imp[0] = _TICKET_IMPUESTO1_PRECIO( h );
    imp[1] = _TICKET_IMPUESTO2_PRECIO( h );
    imp[2] = _TICKET_IMPUESTO3_PRECIO( h );
    imp[3] = _TICKET_IMPUESTO4_PRECIO( h );
    recargo_neto = _TICKET_RECARGO( h );
    if( flags_impuestos & ACUMULAR_EXENTOS ) {
        if( _TICKET_EXENTO( h ) * cantidad ) {
            acu->exento += _TICKET_EXENTO( h ) * cantidad;
        }
    }
    if( flags_impuestos & ACUMULAR_IVA ) {
        if( cod_iva == -1 || _TICKET_COD_IVA( h ) == cod_iva ) {
            if( _TICKET_GRAVADO( h ) || ( imp_iva1 * cantidad + imp_iva2 * cantidad ) ) {
                acu->iva1 += imp_iva1 * cantidad;
                recargo_neto -= _TICKET_IVA1_REC( h );
                acu->iva2 += imp_iva2 * cantidad;
                recargo_neto -= _TICKET_IVA2_REC( h );
                acu->importe_sin_iva += ( _TICKET_IMPORTE_SIN_IVA( h ) + recargo_neto ) * cantidad;
            }
        }
    }
    if( flags_impuestos & ACUMULAR_ILA_IHA ) {
        if( imp[0] * cantidad + imp[1] * cantidad + imp[2] * cantidad + imp[3] * cantidad ) {
            for( i = 0;i < 4;i ++ ) {
                acu->monto_impuesto[i] += imp[i] * cantidad;
            }
        }
        //acu->alicuota += _TICKET_ALICUOTA_PRECIO( h ) * cantidad;
    }
}
/***************************************************************************/
struct _acu_iva * ACUMULAR_IMPUESTOS_TICKET( int disc_departamentos, unsigned desc,
                                             int flags_impuestos, int cod_iva,
                                             int disc_articulo_habilitado )
/***************************************************************************/
{
    unsigned char mask,mask_desc;
    int h;
    int acumular = 0;
    static struct _acu_iva acum_impuestos;
    memset( &acum_impuestos, 0, sizeof( acum_impuestos ) );
    mask_desc = 1 << desc;
    for( h = 0;h < RAM_P_TICKET;h++ ) {
        if( !disc_articulo_habilitado ) {
            if( !filtro_origen_articulo || _TICKET_ORIGEN( h ) == filtro_origen_articulo ) {
                if( disc_departamentos ) {
                    mask = _TICKET_ATRIB_DESCUENTOS( h );
                    if( mask & 0x80 ) {
                        if( mask & mask_desc ) {
                            acumular = 1;
                        }
                    }
                    else {
                        mask = ATRIB_DESC_DPTO[_TICKET_DEPTO( h )];
                        if( mask & 0x80 ) {
                            if( mask & mask_desc ) {
                                acumular = 1;
                            }
                        }
                        else {
                            acumular = 1;
                        }
                    }
                }
                else {
                    acumular = 1;
                }
                if( acumular ) {
                    ACUMULA_IMPUESTOS_ITEM( h, &acum_impuestos,
                                            _TICKET_CANTIDAD( h )
                                          * ( ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1 ),
                                            flags_impuestos, cod_iva );
                    acumular = 0;
                }
            }
        }
        else if( _TICKET_HAB_DESC_CLIENTE( h ) ) {
            ACUMULA_IMPUESTOS_ITEM( h, &acum_impuestos,
                                    _TICKET_CANTIDAD( h )
                                  * ( ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1 ), flags_impuestos,
                                    cod_iva );
        }
    }
    return &acum_impuestos;
}
#endif
/***************************************************************************/
int AGREGAR_DESCUENTO_IMPORTE( int id )
/***************************************************************************/
{
    #ifdef COMPILAR_DESCUENTOS
    long codigo;
    double importe,importe_ticket;
    double porc;
    struct _acu_iva *acumulados;
    int i;
	int desc_fijo = 0, j;

	for( j = 0;j < 5;j++ ) {
		if( _GET_MEMORY_CHAR( __ram_desc_porc_pendientes, j ) ){
			desc_fijo = 1;
		}
	}
    if( desc_fijo || ( RAM_DESCUENTO_IMPORTE && !RAM_ANULACION ) ) {
        return ( 0 );
    }
    SET_MEMORY_LONG( __ram_codigo_supervisor, 1 );
    if( SOLICITAR_CLAVES( 29, 19, NO, SI, PEDIR_SUPERVISOR_DESC_IMP, ST( S_DESCUENTO_IMPORTE ),
                          ST( S_DESCUENTO_IMPORTE ), RAM_COD_FUNCION, SI ) ) {
        SET_MEMORY_INT( __ram_motivo_descuento, 0 );
        for( i = 0;i < 5;i++ ) {
            _SET_MEMORY_CHAR( __ram_motivo_descuento_pendiente, i, 0 );
        }
        if( PEDIR_MOTIVO_EN_DESCUENTOS && !RAM_ANULACION ) {
            SET_MEMORY_INT( __ram_motivo_descuento, ELEGIR_MOTIVO( NULL, _MOTIVO_DESCUENTO ) );
        }
        if( id == 1 ) {
            codigo = config.cod_descuento_importe1;
        }
        else if( id == 2 ) {
            codigo = config.cod_descuento_importe2;
        }
        else {
            codigo = config.cod_descuento_importe3;
        }
        if( RAM_ANULACION ) {
            long id_evento = RAM_ID_EVENTO;
            long position;
            struct _acu_iva impuestos;
            //SELECT( AREA_E_TICKET );
            //SET_ORDER( 1 );
            //GET_LAST();
			SELECT_TABLE( T_EV_CONT, TT_ORIG );
			SET_WHERE("");
			SET_ORDER2("ID_EVENTO");
			RUN_QUERY(NO);
			GO2(BOTTOM);
            memset( &impuestos, 0, sizeof( impuestos ) );
            while( event_e_ticket.id_evento == id_evento && !BOF() && RAM_DESCUENTO_IMPORTE ) {
                if( event_e_ticket.origen == _ORIGEN_DESC_PESOS && event_e_ticket.tipo == id ) {
                    if( config.pais == COLOMBIA ) {
                        impuestos.exento = -event_e_ticket.total;
                    }
                    else {
                        impuestos.importe_sin_iva = -event_e_ticket.importe_sin_iva;
                        impuestos.exento = -event_e_ticket.exento;
                        impuestos.iva1 = -event_e_ticket.iva1;
                        impuestos.iva2 = -event_e_ticket.iva2;
                        for( i = 0;i < 4;i++ ) {
                            impuestos.monto_impuesto[i] = -event_e_ticket.monto_impuesto[i];
                        }
                    }
                    position = GET_POSITION_REG();
                    if( _AGREGAR_DESCUENTO( codigo, 0, &impuestos, 0, _ORIGEN_DESC_PESOS, id, 0.0,
                                            0.0 ) ) {
                        ELIMINA_BASE_DTO( 6 );  //Descuento porcentual
                    }
                    //GO2( position );
                    GET_DIRECT_REG( position , 0 );
                    SET_MEMORY_CHAR( __ram_descuento_importe, 0 );
                    break;
                }
                SKIP2( -1 );
            }
            SET_MEMORY_INT( __ram_anulacion, 0 );
            LIMPIAR_RECUADRO_PETICION( 0 );
        }
        else {
            importe = PEDIR_FLOAT_EN_RECUADRO( ST( S__IMPORTE_A_DESCONTAR_ ), SI, ENTEROS,
                                               DECIMALES );
            if( importe > 0 ) {
                if( importe <= config.tope_descuento_imp && importe <= RAM_TOTAL ) {
                    if( RAM_NEGATIVO || RAM_MODO_DEVOLUCION ) {
                        importe = -importe;
                    }
                    if( config.pais == COLOMBIA ) {
                        SET_FILTRO_ORIGEN_ARTICULO( _ORIGEN_TICKET );
                    }
                    acumulados = ACUMULAR_IMPUESTOS_TICKET( NO, 0, -1, -1, NO );
                    importe_ticket = acumulados->importe_sin_iva + acumulados->exento
                                   + acumulados->iva1 + acumulados->iva2
                                   + acumulados->monto_impuesto[0] + acumulados->monto_impuesto[1]
                                   + acumulados->monto_impuesto[2] + acumulados->monto_impuesto[3];
                    if( importe_ticket ) {
                        porc = ( importe * 100.00 ) / importe_ticket;
                        CALCULAR_BASE_DESCUENTOS( 6, porc, SI, -1, NO );
                        if( RAM_FACTURA || IMPRESORA_FISCAL == FISCAL ) {
                            int cod_iva;
                            _AGREGAR_DESCUENTO( codigo, 0,
                                                ACUMULAR_IMPUESTOS_TICKET( NO, 0, ACUMULAR_EXENTOS,
                                                                           0, NO ), porc,
                                                _ORIGEN_DESC_PESOS, id, 0.0, 0.0 );
                            _AGREGAR_DESCUENTO( codigo, 0,
                                                ACUMULAR_IMPUESTOS_TICKET( NO, 0, ACUMULAR_ILA_IHA,
                                                                           0, NO ), porc,
                                                _ORIGEN_DESC_PESOS, id, 0.0, 0.0 );
                            for( cod_iva = 0;cod_iva < MAX_COD_IVA;cod_iva++ ) {
                                if( iva[cod_iva].porc_iva1 > .009 ) {
                                    _AGREGAR_DESCUENTO( codigo, cod_iva,
                                                        ACUMULAR_IMPUESTOS_TICKET( NO, 0,
                                                                                   ACUMULAR_IVA,
                                                                                   cod_iva, NO ),
                                                        porc, _ORIGEN_DESC_PESOS, id, 0.0, 0.0 );
                                }
                            }
                        }
                        else {
                            _AGREGAR_DESCUENTO( codigo, 0,
                                                ACUMULAR_IMPUESTOS_TICKET( NO, 0, -1, -1, NO ),
                                                porc, _ORIGEN_DESC_PESOS, id, 0.0, 0.0 );
                        }
                    }
                    else {
                        acumulados->importe_sin_iva = ( importe
                                                      / ( 1 + ( iva[0].porc_iva1 / 100 ) ) );
                        acumulados->iva1 = ( importe - acumulados->importe_sin_iva );
                        acumulados->importe_sin_iva *= -1;
                        acumulados->iva1 *= -1;
                        _AGREGAR_DESCUENTO( codigo, 0, acumulados, 0, _ORIGEN_DESC_PESOS, id, 0.0,
                                            0.0 );
                    }
                    SET_MEMORY_CHAR( __ram_descuento_importe, id );//Posibles valores 1, 2 y 3
                    if( config.pais == COLOMBIA ) {
                        SET_FILTRO_ORIGEN_ARTICULO( NO );
                    }
                }
                else {
					MENSAJE_ID_CON_PAUSA_CONF( S_IMPORTE_DE_DESCUENTO_EXCEDIDO );
                    //MENSAJE_STRING( S_IMPORTE_DE_DESCUENTO_EXCEDIDO );
                }
            }
        }
    }
    else {
        SET_MEMORY_LONG( __ram_codigo_supervisor, 0 );
    }
	MOSTRAR_DESCUENTOS_EN_PANTALLA();
    #endif
    return ( 1 );
}
/***************************************************************************/
void AGREGAR_DESCUENTO_PORCENTUAL()
/***************************************************************************/
{
    #ifdef COMPILAR_DESCUENTOS
    double porc,porc_new,importe_ticket,importe_base;
    struct _acu_iva *acumulados;
    int i;
    if( RAM_DESCUENTO_PORCENTUAL && !RAM_ANULACION ) {
        MENSAJE_STRING( S_PUEDE_REALIZAR_SOLO_UN_DESCUENTO_POR_TICKET );
    }
    else {
        SET_MEMORY_LONG( __ram_codigo_supervisor, 1 );
        if( SOLICITAR_CLAVES( 29, 19, NO, SI, PEDIR_SUPERVISOR_DESC_PORC, ST( S__DESCUENTO___ ),
                              ST( S_DESCUENTO__ ), RAM_COD_FUNCION, SI ) ) {
            SET_MEMORY_INT( __ram_motivo_descuento, 0 );
            if( PEDIR_MOTIVO_EN_DESCUENTOS && !RAM_ANULACION ) {
                SET_MEMORY_INT( __ram_motivo_descuento, ELEGIR_MOTIVO( NULL, _MOTIVO_DESCUENTO ) );
            }
            if( RAM_ANULACION ) {
                long id_evento = RAM_ID_EVENTO;
                long position;
                struct _acu_iva impuestos;
                //SELECT( AREA_E_TICKET );
                //SET_ORDER( 1 );
                //GET_EQUAL( ( char* )&id_evento );
				SELECT_TABLE( T_EV_CONT, TT_ORIG );
				SET_WHERE("ID_EVENTO = '%ld'",id_evento);
				SET_ORDER2("ID_EVENTO");
				RUN_QUERY(NO);
                memset( &impuestos, 0, sizeof( impuestos ) );
                while( event_e_ticket.id_evento == id_evento && !dbEOF()
                    && RAM_DESCUENTO_PORCENTUAL ) {
                    if( event_e_ticket.origen == _ORIGEN_DESC_PORC && event_e_ticket.tipo == 0 ) {
                        if( config.pais != COLOMBIA ) {
                            impuestos.importe_sin_iva = -event_e_ticket.importe_sin_iva;
                            impuestos.exento = -event_e_ticket.exento;
                            impuestos.iva1 = -event_e_ticket.iva1;
                            impuestos.iva2 = -event_e_ticket.iva2;
                            for( i = 0;i < 4;i++ ) {
                                impuestos.monto_impuesto[i] = -event_e_ticket.monto_impuesto[i];
                            }
                        }
                        else {
                            impuestos.exento = -event_e_ticket.total;
                        }
                        position = GET_POSITION_REG();
                        if( _AGREGAR_DESCUENTO( config.cod_descuento_porc, event_e_ticket.cod_iva,
                                                &impuestos, 0, _ORIGEN_DESC_PORC, 0, 0.0, 0.0 ) ) {
                            SET_MEMORY_CHAR( __ram_descuento_porcentual, 0 );
                            ELIMINA_BASE_DTO( 5 );  //Descuento porcentual
                        }
                        //GO2( position );
                        GET_DIRECT_REG( position, 0);
                        break;
                    }
                    SKIP2( 1 );
                }
                SET_MEMORY_INT( __ram_anulacion, 0 );
                LIMPIAR_RECUADRO_PETICION( 0 );
            }
            else {
                porc = PEDIR_FLOAT_EN_RECUADRO( ST( S__PORCENTAJE_A_DESCONTAR_ ), SI, 2, 2 );
                if( porc > 0 ) {
                    if( porc <= config.tope_descuento_porc ) {
                        if( config.pais == COLOMBIA || config.pais == CHILE
                         || config.pais == BRASIL ) {
                            acumulados = ACUMULAR_IMPUESTOS_TICKET( NO, 0, -1, -1, NO );
                            importe_ticket = acumulados->importe_sin_iva + acumulados->exento
                                           + acumulados->iva1 + acumulados->iva2
                                           + acumulados->monto_impuesto[0]
                                           + acumulados->monto_impuesto[1]
                                           + acumulados->monto_impuesto[2]
                                           + acumulados->monto_impuesto[3];
                            SET_FILTRO_ORIGEN_ARTICULO( _ORIGEN_TICKET );
                            acumulados = ACUMULAR_IMPUESTOS_TICKET( NO, 0, -1, -1, NO );
                            importe_base = acumulados->importe_sin_iva + acumulados->exento
                                         + acumulados->iva1 + acumulados->iva2
                                         + acumulados->monto_impuesto[0]
                                         + acumulados->monto_impuesto[1]
                                         + acumulados->monto_impuesto[2]
                                         + acumulados->monto_impuesto[3];
                            porc_new = porc * ( 1 + ( importe_ticket - importe_base )
                                              / importe_ticket );
                            porc = porc_new;
                        }
                        CALCULAR_BASE_DESCUENTOS( 5, porc, SI, -1, NO );
                        if( RAM_FACTURA || RAM_TICKET_FACTURA || (IMPRESORA_FISCAL == FISCAL )) { //para la tm220 debe prorratear el descuento
                            int cod_iva;
                            _AGREGAR_DESCUENTO( config.cod_descuento_porc, 0,
                                                ACUMULAR_IMPUESTOS_TICKET( NO, 0, ACUMULAR_EXENTOS,
                                                                           0, NO ), porc,
                                                _ORIGEN_DESC_PORC, 0, 0.0, 0.0 );
                            _AGREGAR_DESCUENTO( config.cod_descuento_porc, 0,
                                                ACUMULAR_IMPUESTOS_TICKET( NO, 0, ACUMULAR_ILA_IHA,
                                                                           0, NO ), porc,
                                                _ORIGEN_DESC_PORC, 0, 0.0, 0.0 );
                            for( cod_iva = 0;cod_iva < MAX_COD_IVA;cod_iva++ ) {
                                if( iva[cod_iva].porc_iva1 > .009 ) {
                                    _AGREGAR_DESCUENTO( config.cod_descuento_porc, cod_iva,
                                                        ACUMULAR_IMPUESTOS_TICKET( NO, 0,
                                                                                   ACUMULAR_IVA,
                                                                                   cod_iva, NO ),
                                                        porc, _ORIGEN_DESC_PORC, 0, 0.0, 0.0 );
                                }
                            }
                        }
                        else {
                            _AGREGAR_DESCUENTO( config.cod_descuento_porc, 0,
                                                ACUMULAR_IMPUESTOS_TICKET( NO, 0, -1, -1, NO ),
                                                porc, _ORIGEN_DESC_PORC, 0, 0.0, 0.0 );
                        }
                        SET_MEMORY_CHAR( __ram_descuento_porcentual, 1 );
                        if( config.pais == COLOMBIA ) {
                            SET_FILTRO_ORIGEN_ARTICULO( NO );
                        }
                    }
                    else {
                        MENSAJE_STRING( S_PORCENTAJE_DE_DESCUENTO_EXCEDIDO );
                    }
                }
            }
        }
        else {
            SET_MEMORY_LONG( __ram_codigo_supervisor, 0 );
        }
    }
    #endif
}
#ifdef COMPILAR_DESCUENTOS
/***************************************************************************/
struct _acu_iva * CALCULAR_IMPORTE_IMPUESTOS( struct _acu_iva *acu, double porc )
{
    /***************************************************************************/
    static struct _acu_iva total_imp_item;
    int i;
    memset( &total_imp_item, 0, sizeof( total_imp_item ) );
    porc = ( porc / 100 ) * -1;
    total_imp_item.importe_sin_iva = ROUND( porc * acu->importe_sin_iva, ENTEROS, DECIMALES );
    total_imp_item.exento = ROUND( porc * acu->exento, ENTEROS, DECIMALES );
    total_imp_item.iva1 = ROUND( porc * acu->iva1, ENTEROS, DECIMALES );
    total_imp_item.iva2 = ROUND( porc * acu->iva2, ENTEROS, DECIMALES );
    for( i = 0;i < 4;i++ ) {
        total_imp_item.monto_impuesto[i] = ROUND( porc * acu->monto_impuesto[i], ENTEROS,
                                                  DECIMALES );
    }
    return &total_imp_item;
}
/***************************************************************************/
static int _AGREGAR_DESCUENTO( long cod_art, int cod_iva, struct _acu_iva *acum, double porc,
                               char origen, int tipo, double imp_descuento, double porc_original )
/***************************************************************************/
{
    char nombre[41],nombre_desc[20];
    double importe, importe_s_iva, tasa_iva;
    int iOk;
    struct _acu_iva *impuestos;

	iOk = ( cod_art ) ? 1 : 0;
	/*if( !iOk ) {
      MENSAJE_STRING( S_ARTICULO_DESCUENTO_NO_DEFINIDO );
	}*/

    if( porc > 0.009 && iOk ) {
        impuestos = CALCULAR_IMPORTE_IMPUESTOS( acum, porc );
    }
    else {
        impuestos = acum;
    }

    if( !RAM_ANULACION && config.pais == COLOMBIA && tipo
     && ( origen != _ORIGEN_DESC_PESOS && !( origen == _ORIGEN_DESC_PORC && tipo == 9 ) ) ) {
        importe = -imp_descuento;
    }
    else {
        importe = impuestos->importe_sin_iva + impuestos->exento + impuestos->iva1 + impuestos->iva2
                + impuestos->monto_impuesto[0] + impuestos->monto_impuesto[1]
                + impuestos->monto_impuesto[2] + impuestos->monto_impuesto[3];
    }

    if( importe ) {
        ASIGNA_CODIGO_ESPECIAL( cod_art );
        if( config.pais == ECUADOR && origen == _ORIGEN_DESC_PORC && ( tipo == 9 || tipo == 0 ) ) {
            strcpy( nombre, ITEM_NOMBRE_LARGO );
            TRIM( nombre );
            _snprintf(nombre_desc, sizeof(nombre_desc)-1, " %5.2lf%%", porc_original );
            strcat( nombre, nombre_desc );
            nombre[30] = 0;
            SET_MEMORY( __item_nombre_largo, nombre );
            SET_MEMORY_DOUBLE( __item_costo, porc_original );
        }
		if( !impuestos->exento ) {
   			importe_s_iva = impuestos->importe_sin_iva;
   			tasa_iva = impuestos->iva1;
   		} else {
   			importe_s_iva = impuestos->exento;
   			tasa_iva = 0.0;
   		}
		SET_MEMORY_DOUBLE( __item_precio_sin_iva, importe_s_iva );
		SET_MEMORY_DOUBLE( __item_iva1_precio, tasa_iva );
        SET_MEMORY_DOUBLE( __item_precio, importe );
		SET_MEMORY_DOUBLE( __item_imp_int, 0 );
        SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
		ADD_MEMORY_DOUBLE( __ram_imp_descuento, importe );

		//agregado para nacionales fiscales
		if( fabs(impuestos->exento) > 0.01 )
   			SET_MEMORY_CHAR( __item_gravado, 0 );
   		else {
   			SET_MEMORY_CHAR( __item_cod_iva, cod_iva );
   			SET_MEMORY_CHAR( __item_gravado, 1 );
   		}

        AGREGA_ITEM_AL_TICKET( RAM_P_TICKET, 0, NO, NO, origen );
        if( config.pais != ECUADOR ) {
            _SET_MEMORY_CHAR( __ticket_cod_iva, RAM_P_TICKET, cod_iva );
        }

		_SET_MEMORY_DOUBLE( __ticket_precio, RAM_P_TICKET, ITEM_PRECIO );
        _SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, RAM_P_TICKET, impuestos->importe_sin_iva );
        _SET_MEMORY_DOUBLE( __ticket_exento, RAM_P_TICKET, impuestos->exento );
        _SET_MEMORY_DOUBLE( __ticket_iva1_precio, RAM_P_TICKET, impuestos->iva1 );
        _SET_MEMORY_DOUBLE( __ticket_iva2_precio, RAM_P_TICKET, impuestos->iva2 );

        _SET_MEMORY_FLOAT( __ticket_impuesto1_precio, RAM_P_TICKET, impuestos->monto_impuesto[0] );
        _SET_MEMORY_FLOAT( __ticket_impuesto2_precio, RAM_P_TICKET, impuestos->monto_impuesto[1] );
        _SET_MEMORY_FLOAT( __ticket_impuesto3_precio, RAM_P_TICKET, impuestos->monto_impuesto[2] );
        _SET_MEMORY_FLOAT( __ticket_impuesto4_precio, RAM_P_TICKET, impuestos->monto_impuesto[3] );
        _SET_MEMORY_DOUBLE( __ticket_recargo, RAM_P_TICKET, 0 );
        _SET_MEMORY_DOUBLE( __ticket_imp_int, RAM_P_TICKET, 0 );
        _SET_MEMORY_DOUBLE( __ticket_iva1_rec, RAM_P_TICKET, 0 );
        _SET_MEMORY_DOUBLE( __ticket_iva2_rec, RAM_P_TICKET, 0 );
        _SET_MEMORY_CHAR( __ticket_hab_desc_cliente, RAM_P_TICKET, 0 );
        _SET_MEMORY_CHAR( __ticket_tipo2, RAM_P_TICKET, ( char )tipo );
        DISCRIMINA_IMPUESTOS_EN_ARTICULO( -porc / 100.0, NULL );
        ADD_MEMORY_INT( __ram_p_ticket, 1 );
        SET_MEMORY_DOUBLE( __ram_total, ROUND( RAM_TOTAL + ITEM_PRECIO, 10, DECIMALES ) );
        if( DEBE_IMPRIMIR_ITEM() ) {
            IMPRIME_ITEM( RAM_P_TICKET - 1 );
        }
 //       MUESTRA_SUBTOTAL();
        MUESTRA_ITEM_EN_PANTALLA( RAM_P_TICKET - 1 );
        ACTUALIZA_ACUMULADORES_RAM( RAM_P_TICKET - 1 );
        GRABAR_EVENT_E_TICKET( RAM_P_TICKET - 1, NO, SI, origen, tipo, NO );
        GRABAR_TICKET( RAM_P_TICKET - 1 );
        SET_MEMORY_CHAR( __ram_prohibir_mas, 1 );
        iOk = 1;
        SET_MEMORY_INT( __ram_motivo_descuento, 0 );
		MUESTRA_SUBTOTAL(NO);
    }
    else {
        iOk = 1;
    }

    return ( iOk );
}
#endif
/***************************************************************************/
void APLICAR_DESCUENTOS_PENDIENTES()
/***************************************************************************/
{
    #ifdef COMPILAR_DESCUENTOS
    unsigned i;
    for( i = 0;i < 5;i++ ) {
        if( _GET_MEMORY_CHAR( __ram_desc_porc_pendientes, i ) == 1 ) {
            if( ( config.mask_descuentos_empleados & ( 1 << i ) )
             && !_RAM_STRING_LEGAJO_EMPLEADO( 0 ) ) {
                MENSAJE_STRING( S_DEBE_IDENTIFICAR_EL_EMPLEADO );
                _SET_MEMORY_CHAR( __ram_desc_porc_pendientes, i, 0 );
            }
            else {
                SET_MEMORY_INT( __ram_motivo_descuento,
                                _GET_MEMORY_INT( __ram_motivo_descuento_pendiente, i ) );
                APLICAR_DESCUENTO( ( char )i );
                SET_MEMORY_INT( __ram_motivo_descuento, 0 );
            }
        }
    }
    #endif
}
/***************************************************************************/
void REVERTIR_DESCUENTOS_FINAL_TICKET( void )
/***************************************************************************/
{
    #ifdef COMPILAR_DESCUENTOS
    char i;
    int anulaciones_verif = VERIFICA_ANULACIONES_AUTORIZADAS();
    if( CALCULAR_DESC_AL_FINAL_TICKET ) {
        for( i = 0;i < 5;i++ ) {
            if( _GET_MEMORY_CHAR( __ram_desc_porc_pendientes, i ) == 2 ) {
                SET_MEMORY_INT( __ram_anulacion, 1 );
                SET_MEMORY_CHAR( __ram_correccion, 1 );
                APLICAR_DESCUENTO( ( char )i );
                SET_MEMORY_CHAR( __ram_anulacion, 0 );
                SET_MEMORY_CHAR( __ram_correccion, 0 );
                // Lo Setea en Seleccionado.
                _SET_MEMORY_CHAR( __ram_desc_porc_pendientes, i, 1 );
                // La vuelve a seleccionar
                if( anulaciones_verif ) {
                    SET_MEMORY_INT( __ram_item_supervisado, RAM_P_TICKET - 1 );
                }
            }
        }
        REVERTIR_EVENTO_DESC_PROMO( MOTIVO_DESC_MANUAL_PESOS, 0 );
        REVERTIR_EVENTO_DESC_PROMO( MOTIVO_DESC_MANUAL_PORC, 0 );
        REVERTIR_EVENTO_DESC_PROMO( MOTIVO_DESC_PERFIL, 0 );
    }
    #endif
}
/***************************************************************************/
void MOSTRAR_DESCUENTOS_EN_PANTALLA( void )
/***************************************************************************/
{
    #ifdef COMPILAR_DESCUENTOS
    int i, desc_fijo = 0, desc_importe = 0;
    #if defined(INVEL_W) || defined(INVEL_L)
    char aux[10],aux1[20];
    int send = 0;
	memset( aux1, 0, sizeof( aux1 ) );
	
	for( i = 0;i < 5;i++ ) {
		if( _GET_MEMORY_CHAR( __ram_desc_porc_pendientes, i ) )
		{
			desc_fijo = 1;
			_itoa( i + 1, aux, 10 );
			_snprintf(aux1, sizeof(aux1)-1,"desc: %s", aux );
		}
		if( RAM_DESCUENTO_IMPORTE && RAM_DESCUENTO_IMPORTE != 9 )
		{
			//Posibles valores 1, 2 y 3. Seteamos con 9 para que entre solo una vez por aqui
			desc_importe = 1;
			_snprintf(aux1, sizeof(aux1)-1,"desc Importe %d", RAM_DESCUENTO_IMPORTE );
			SET_MEMORY_CHAR( __ram_descuento_importe, 9 );
			i = 0;//Reutilizamos el flag descuento.gif para el descuento por importe
		}
        if( desc_fijo || desc_importe ) {
            if( desc_on[i] == 0 ) {
                switch( i + 1 ) {
                    case 1:
                        send = T_FLAGS_DESCUENTO_1_AGREGAR;
                        break;
                    case 2:
                        send = T_FLAGS_DESCUENTO_2_AGREGAR;
                        break;
                    case 3:
                        send = T_FLAGS_DESCUENTO_3_AGREGAR;
                        break;
                    case 4:
                        send = T_FLAGS_DESCUENTO_4_AGREGAR;
                        break;
                    case 5:
                        send = T_FLAGS_DESCUENTO_5_AGREGAR;
                        break;
                }
                ExecuteSSAction( A_MUESTRA_FLAGS, send, aux1, " ", NULL, " " );
                desc_on[i] = 1;
            }
        }
        else {
            if( desc_on[i] == 1 ) {
                switch( i + 1 ) {
                    case 1:
                        send = T_FLAGS_DESCUENTO_1_BORRAR;
                        break;
                    case 2:
                        send = T_FLAGS_DESCUENTO_2_BORRAR;
                        break;
                    case 3:
                        send = T_FLAGS_DESCUENTO_3_BORRAR;
                        break;
                    case 4:
                        send = T_FLAGS_DESCUENTO_4_BORRAR;
                        break;
                    case 5:
                        send = T_FLAGS_DESCUENTO_5_BORRAR;
                        break;
                }
                ExecuteSSAction( A_MUESTRA_FLAGS, send, " ", aux1, NULL, " " );
                desc_on[i] = 0;
            }
        }
	}
    #else
    char aux[50];
    for( i = 0;i < 5;i++ ) {
        if( _GET_MEMORY_CHAR( __ram_desc_porc_pendientes, i ) ) {
            _ATRIBUTO = 120;
            MOSTRAR_ENTERO( 20 + i - ancho_panel / 2, 24, "9", i + 1 );
        }
        else {
            _ATRIBUTO = 7;
            MOSTRAR_CADENA( 20 + i - ancho_panel / 2, 24, " " );
        }
    }
    #endif      
    #endif
}
/***************************************************************************/
void TECLA_DESCUENTO_PORCENTUAL( unsigned desc )
/***************************************************************************/
{
    #ifdef COMPILAR_DESCUENTOS
    int ok = 1;

	if( RAM_P_TICKET || CALCULAR_DESC_AL_FINAL_TICKET ) {
		desc--;
		if( RAM_ANULACION ) {
			if( _GET_MEMORY_CHAR( __ram_desc_porc_pendientes, desc ) != 2 ) {
				MENSAJE_ID_CON_PAUSA_CONF( S_DESCUENTO_NO_REALIZADO_EN_EL_TICKET );
				ok = 0;
			}
		}
		else {
			int  i = 0;
			char desc_pend = 0;
			// Controla la clase de descuento
			desc_pend = _RAM_DESC_PORC_PENDIENTES( desc );
			switch( desc_pend ) {
				case 1:
					_SET_MEMORY_CHAR( __ram_desc_porc_pendientes, desc, 0 );
					ok = 0;
					break;
				case 2:
				case 3:
				   MENSAJE_ID_CON_PAUSA_CONF( S_DESCUENTO_YA_APLICADO );
				   ok = 0;
				   break;
				default:
					for( i = 0; i < 5 && ok; i++ ) {
					  if( _RAM_DESC_PORC_PENDIENTES( i ) && config.grupo_dto[desc] == config.grupo_dto[i] ) {
						 ok = 0;
					  }
					}
					if( !ok ) {
						MENSAJE_ID_CON_PAUSA_CONF( S_NO_SE_ACEPTAN_DOS_DESCUENTOS_DEL_MISMO_GRUPO );
					} else {
						if( ( config.mask_descuentos_empleados & ( 1 << desc ) )
						 && !_RAM_STRING_LEGAJO_EMPLEADO( 0 ) ) {
							MENSAJE_STRING( S_DEBE_IDENTIFICAR_EL_EMPLEADO );
							ok = 0;
						}
					}
					break;
			}
		}
		if( ok ) {
			char val_1 = 1;
			if( SOLICITAR_CLAVES( 29, 19, NO, SI, ( ( val_1 << desc ) & config.sup_descuento )
															&& PEDIR_SUPERVISOR_DESC_PORC,
															   ST( S__DESCUENTO___ ),
															   ST( S_DESCUENTO__ ), RAM_COD_FUNCION,
															   SI ) ) {
				SET_MEMORY_INT( __ram_motivo_descuento, 0 );
				if( PEDIR_MOTIVO_EN_DESCUENTOS && !RAM_ANULACION ) {
					SET_MEMORY_INT( __ram_motivo_descuento, ELEGIR_MOTIVO( NULL, _MOTIVO_DESCUENTO ) );
				}
				if( CALCULAR_DESC_AL_FINAL_TICKET && !RAM_ANULACION ) {
					_SET_MEMORY_CHAR( __ram_desc_porc_pendientes, desc, 1 );
					MENSAJE_ID_CON_PAUSA_CONF( S_EL_DESCUENTO_SERA_APLICADO_AL_FINAL_DEL_TIC );
					_SET_MEMORY_CHAR( __ram_motivo_descuento_pendiente, desc, RAM_MOTIVO_DESCUENTO );
					SET_MEMORY_INT( __ram_motivo_descuento, 0 );
				}
				else {
					APLICAR_DESCUENTO( ( char )desc );
				}
			}
		}
	}
    MOSTRAR_DESCUENTOS_EN_PANTALLA();
    #endif
}
/***************************************************************************/
void APLICAR_DESCUENTO( char desc )
{
    /***************************************************************************/
    #ifdef COMPILAR_DESCUENTOS
    double porc,importe;
    int i;
    porc = config.descuento_porc_fijo[desc];
    if( porc > 0 ) {
        if( RAM_ANULACION ) {
            long id_evento = RAM_ID_EVENTO;
            long position;
            struct _acu_iva impuestos;
            importe = 0;
            //SELECT( AREA_E_TICKET );
            //SET_ORDER( 1 );
            //GET_LAST();           //(char*)&id_evento
			SELECT_TABLE( T_EV_CONT, TT_ORIG );
			SET_WHERE("");
			SET_ORDER2("ID_EVENTO");
			RUN_QUERY(NO);
			GO2(BOTTOM);
            if( event_e_ticket.id_evento != id_evento ) {
                while( !dbBOF() && event_e_ticket.id_evento != id_evento ) {
                    SKIP2( -1 );
                }
            }
            memset( &impuestos, 0, sizeof( impuestos ) );
            while( event_e_ticket.id_evento == id_evento && !BOF() ) {
                if( event_e_ticket.origen == _ORIGEN_DESC_PORC && event_e_ticket.tipo == desc + 1 ) {
                    if( config.pais != COLOMBIA ) {
                        impuestos.importe_sin_iva = -event_e_ticket.importe_sin_iva;
                        impuestos.exento = -event_e_ticket.exento;
                        impuestos.iva1 = -event_e_ticket.iva1;
                        impuestos.iva2 = -event_e_ticket.iva2;
                        for( i = 0;i < 4;i++ ) {
                            impuestos.monto_impuesto[i] = -event_e_ticket.monto_impuesto[i];
                        }
                    }
                    else {
                        impuestos.exento = -event_e_ticket.total;
                    }
                    position = GET_POSITION_REG();
                    if( _AGREGAR_DESCUENTO( config.cod_desc_porc_fijo[desc], event_e_ticket.cod_iva,
                                            &impuestos, 0, _ORIGEN_DESC_PORC, desc + 1, 0.0, 0.0 ) ) {
                        SET_MEMORY_CHAR( __ram_descuento_porcentual, 0 );
                        _SET_MEMORY_CHAR( __ram_desc_porc_pendientes, desc, 0 );
                        ELIMINA_BASE_DTO( desc );
                    }
                    //GO2( position );
                    GET_DIRECT_REG( position, 0 );
                    break;
                }
                GET_PREVIOUS();
            }
            SET_MEMORY_INT( __ram_anulacion, 0 );
            LIMPIAR_RECUADRO_PETICION( 0 );
        }
        else {
            if( DESCUENTOS_FIJOS_SELECTIVOS ) {
                importe = CALCULAR_BASE_DESCUENTOS( desc, porc, NO, -1, NO );
            }
            else {
                importe = RAM_TOTAL;
            }
            if( importe ) {
                char descuento_base[100];
                double importe_porc = ROUND( importe *porc / 100, ENTEROS, DECIMALES );
                sprintf( descuento_base, "%s $%.2f", ST( S_BASE_DEL_DESCUENTO ), importe );
                if( config.pais == COLOMBIA ) {
                    _AGREGAR_DESCUENTO( config.cod_desc_porc_fijo[desc], 0,
                                        ACUMULAR_IMPUESTOS_TICKET( DESCUENTOS_FIJOS_SELECTIVOS,
                                                                   desc, -1, -1, NO ), porc,
                                        _ORIGEN_DESC_PORC, desc + 1, importe_porc, 0.0 );
                }
                else {
					if( RAM_FACTURA || IMPRESORA_FISCAL == FISCAL ) { //se agrega para la tm220
                        int cod_iva;
                        _AGREGAR_DESCUENTO( config.cod_desc_porc_fijo[desc], 0,
                                            ACUMULAR_IMPUESTOS_TICKET( DESCUENTOS_FIJOS_SELECTIVOS,
                                                                       desc, ACUMULAR_EXENTOS, 0,
                                                                       NO ), porc,
                                            _ORIGEN_DESC_PORC, desc + 1, 0.0, 0.0 );
                        _AGREGAR_DESCUENTO( config.cod_desc_porc_fijo[desc], 0,
                                            ACUMULAR_IMPUESTOS_TICKET( DESCUENTOS_FIJOS_SELECTIVOS,
                                                                       desc, ACUMULAR_ILA_IHA, 0,
                                                                       NO ), porc,
                                            _ORIGEN_DESC_PORC, desc + 1, 0.0, 0.0 );
                        for( cod_iva = 0;cod_iva < MAX_COD_IVA;cod_iva++ ) {
                            if( iva[cod_iva].porc_iva1 > .009 ) {
                                _AGREGAR_DESCUENTO( config.cod_desc_porc_fijo[desc], cod_iva,
                                                    ACUMULAR_IMPUESTOS_TICKET(
                                DESCUENTOS_FIJOS_SELECTIVOS, desc, ACUMULAR_IVA, cod_iva, NO ),
                                                    porc, _ORIGEN_DESC_PORC, desc + 1, 0.0, 0.0 );
                            }
                        }
                    }
                    else {
                        _AGREGAR_DESCUENTO( config.cod_desc_porc_fijo[desc], 0,
                                            ACUMULAR_IMPUESTOS_TICKET( DESCUENTOS_FIJOS_SELECTIVOS,
                                                                       desc, -1, -1, NO ), porc,
                                            _ORIGEN_DESC_PORC, desc + 1, 0.0, 0.0 );
                    }
                }
                if( DEBE_IMPRIMIR_ITEM() && config.pais != ECUADOR ) {
                    MOSTRAR_CADENA( 1, PRN, descuento_base );
                    LF( 1 );
                }
                SET_MEMORY_CHAR( __ram_descuento_porcentual, desc + 1 );
                _SET_MEMORY_CHAR( __ram_desc_porc_pendientes, desc, 2 );
            }
            else {
                MENSAJE_STRING( S_NO_HAY_ARTICULOS_HABILITADOS_PARA_DICHO_DESCUENTO );
            }
        }
    }
    #endif
}
/***************************************************************************/
double CALCULAR_BASE_DESCUENTOS( unsigned desc, double porc, int incondicional, int posicion,
                                 int acumulativo )
/***************************************************************************/
{
    double total = 0;
    #ifdef COMPILAR_DESCUENTOS
    int h = 0, handle_desc = 0, j = 0, k = 0;
    double tot_aux = 0;
    double *bases_descuento = NULL;
    char archivo[10];
    double porc_aux;
    int i, ultimo_item, origen = 0, iva_anterior;
    porc_aux = porc;
    //---- Se recorren todos los items del ticket para ver cuales se incluyen
    //---- en el descuento y cuales no.
    if( porc != 0 ) {
        bases_descuento = MALLOC( sizeof( double ) * _LARGO_TICKET );
        if( bases_descuento ) {
            memset( bases_descuento, 0, sizeof( double ) * _LARGO_TICKET );
        }
        //---Acumulamos los descuentos calculados con anterioridad.
        if( acumulativo ) {
            sprintf( archivo, "dp%d.dat", desc );
            NRO_ARCHIVO = 0;
            handle_desc = _OPEN( archivo, S_IFREG, __LINE__, __FILE__ );
            if( handle_desc > 0 ) {
                _READ( handle_desc, ( char* )bases_descuento, sizeof( double ) * _LARGO_TICKET,
                       __LINE__, __FILE__ );
                _CLOSE( handle_desc, __LINE__, __FILE__ );
            }
        }
    }
    i = RAM_P_TICKET - 1;
    origen = _TICKET_ORIGEN( i );
    iva_anterior = MAX_COD_IVA + 1;
    while( _TICKET_ORIGEN( i ) == origen && ( _TICKET_PRECIO( i ) < 0 )
        && ( _TICKET_COD_IVA( i ) < iva_anterior ) ) {
        iva_anterior = _TICKET_COD_IVA( i );
        i--;
    }
    ultimo_item = i;
    if( posicion == -1 ) {
        tot_bruto = 0.00;
        tot_neto = 0.00;
        for( k = 0;k <= ultimo_item;k++ ) {
            tot_bruto += ( _TICKET_ORIGEN( k ) == _ORIGEN_TICKET )
                       ? _TICKET_PRECIO( k ) * _TICKET_CANTIDAD( k ) : 0.00;
            tot_neto += _TICKET_PRECIO( k ) * _TICKET_CANTIDAD( k );
        }
        for( h = 0;h < RAM_P_TICKET;h++ ) {
            if( _CALCULAR_BASE_DESCUENTOS( desc, h, incondicional, porc, &total, &tot_aux,
                                           bases_descuento, acumulativo ) ) {
                j = h;
            }
        }
    }
    else {
        _CALCULAR_BASE_DESCUENTOS( desc, posicion, incondicional, porc, &total, &tot_aux,
                                   bases_descuento, acumulativo );
        j = posicion;
    }
    porc = porc_aux;
    if( bases_descuento && ( porc != 0 ) ) {
        if( tot_aux
         != ( ROUND( total * porc / 100.0, ENTEROS, DECIMALES )
            * ( RAM_MODO_DEVOLUCION ? -1 : 1 ) ) ) {
            bases_descuento[j] += ROUND( total * porc / 100.0, ENTEROS, DECIMALES )
                                * ( RAM_MODO_DEVOLUCION ? -1 : 1 ) - tot_aux;
        }
        sprintf( archivo, "dp%d.dat", desc );
        NRO_ARCHIVO = 0;
        handle_desc = _CREAT( archivo, S_IFREG | S_IWRITE | S_IREAD, __LINE__, __FILE__ );
        if( handle_desc ) {
            _WRITE( handle_desc, ( char* )bases_descuento, sizeof( double ) * _LARGO_TICKET,
                    __LINE__, __FILE__ );
            _CLOSE( handle_desc, __LINE__, __FILE__ );
        }
        FREEMEM( ( void* )bases_descuento );
    }
    #endif
    return ( total );
}
/***************************************************************************/
int _CALCULAR_BASE_DESCUENTOS( int desc, int h, int incondicional, double porc, double *total,
                               double *tot_aux, double *bases_descuento, int acumulativo )
/***************************************************************************/
{
    int acumula = 0;
    #ifdef COMPILAR_DESCUENTOS
    unsigned char mask,mask_desc;
    double importe,descuento;
    if( filtro_origen_articulo && _TICKET_ORIGEN( h ) != filtro_origen_articulo ) {
        return ( 0 );
    }
    acumula = 0;
    importe = _TICKET_PRECIO( h ) * _TICKET_CANTIDAD( h );
    if( _TICKET_TIPO( h ) & _RESTA ) {
        importe *= -1;
    }
    if( incondicional ) {
        acumula = 1;
        *total += importe;
    }
    else {
        mask_desc = 1 << desc;
        mask = _TICKET_ATRIB_DESCUENTOS( h );
        if( mask & 0x80 ) {
            if( mask & mask_desc ) {
                *total += importe;
                acumula = 1;
            }
        }
        else {
            mask = ATRIB_DESC_DPTO[_TICKET_DEPTO( h )];
            if( mask & 0x80 ) {
                if( mask & mask_desc ) {
                    *total += importe;
                    acumula = 1;
                }
            }
            else {
                *total += importe;
                acumula = 1;
            }
        }
    }
    if( acumula && ( porc != 0 ) ) {
        if( bases_descuento ) {
            if( RAM_MODO_DEVOLUCION ) {
                importe *= ( -1 );
            }
            descuento = ROUND( importe * porc / 100.0, ENTEROS, DECIMALES );
            if( acumulativo ) {
                bases_descuento[h] += descuento;
            }
            else {
                bases_descuento[h] = descuento;
            }
            if( importe ) {
                //*tot_aux += bases_descuento[ h ];
                *tot_aux += descuento;
            }
        }
    }
    else {
        acumula = 0;
    }
    #endif
    return ( acumula );
}
/***************************************************************************/
void IMPRIMIR_BASE_DESCUENTO( unsigned desc )
/***************************************************************************/
{
    #ifdef COMPILAR_DESCUENTOS
    double importe = 0;
    char descuento_base[100];
    //double porc;
    if( DESCUENTOS_FIJOS_SELECTIVOS ) {
        importe = CALCULAR_BASE_DESCUENTOS( desc - 1, 0, NO, -1, NO );
    }
    else {
        importe = RAM_TOTAL;
    }
    //porc = config.descuento_porc_fijo[desc-1];
    //importe_porc = ROUND(importe * porc / 100, ENTEROS, DECIMALES);
    sprintf( descuento_base, "%s %d $%.2f", ST( S_BASE_DEL_DESCUENTO ), desc, importe );
    if( DEBE_IMPRIMIR_ITEM() && config.pais != ECUADOR ) {
        MOSTRAR_CADENA( 1, PRN, descuento_base );
        LF( 1 );
    }
    #endif
}
/***************************************************************************/
void ELIMINA_BASE_DTO( int desc )
/***************************************************************************/
{
    #ifdef COMPILAR_DESCUENTOS
    char archivo[15];
    switch( desc ) {
        case 96:
            strcpy( archivo, "dp_rec.dat" );
            break;
        case 97:
            strcpy( archivo, "dp_per.dat" );
            break;
        case 98:
            strcpy( archivo, "dp_imp.dat" );
            break;
        case 99:
            strcpy( archivo, "dp_pro.dat" );
            break;
        default:
            sprintf( archivo, "dp%d.dat", desc );
            break;
    }
    _unlink( archivo );
    #endif
}
/*****************************************************************************/
int CARGA_BASE_DESCUENTO( int desc, double *base_aux )
/*****************************************************************************/
{
    int rta = 0;
    #ifdef COMPILAR_DESCUENTOS
    char archivo[15];
    int handle_desc;
    switch( desc ) {
        case 96:
            //Recargos Financieros desde Perfiles
            strcpy( archivo, "dp_rec.dat" );
            break;
        case 97:
            //Perfiles
            strcpy( archivo, "dp_per.dat" );
            break;
        case 98:
            //Importaciones
            strcpy( archivo, "dp_imp.dat" );
            break;
        case 99:
            //Promociones
            strcpy( archivo, "dp_pro.dat" );
            break;
        default:
            //Otros
            sprintf( archivo, "dp%d.dat", desc );
            break;
    }
    if( access( archivo, 0 ) == 0 ) {
        NRO_ARCHIVO = 0;
        handle_desc = _OPEN( archivo, S_IFREG, __LINE__, __FILE__ );
        if( handle_desc > 0 ) {
            _READ( handle_desc, ( char* )base_aux, sizeof( double ) * _LARGO_TICKET, __LINE__,
                   __FILE__ );
            _CLOSE( handle_desc, __LINE__, __FILE__ );
            rta = 1;
        }
    }
    #endif
    return rta;
}
/***************************************************************************/
void AGREGAR_DESCUENTO_ULTIMO_ARTICULO( int descuento_importe )
/***************************************************************************/
{
    #ifdef COMPILAR_DESCUENTOS
    int h,j,seguir,anular,pos_des;
    long codigo;
    double importe,importe_ticket,porc_original;
    double porc;
    static struct _acu_iva acum_impuestos;
    struct _acu_iva *acumulados;
    if( RAM_P_TICKET ) {
        if( ( ( _TICKET_ORIGEN( RAM_P_TICKET - 1 ) == _ORIGEN_TICKET )
           && !( _TICKET_TIPO( RAM_P_TICKET - 1 ) & _RESTA ) ) || RAM_ANULACION ) {
            if( SOLICITAR_CLAVES( 29, 19, NO, SI, PEDIR_SUPERVISOR_DESC_IMP,
                                  ( descuento_importe ) ? ST( S__DESCUENTO_I_ULT_ART__ )
                                : ST( S__DESCUENTO___ULT_ART__ ),
                                  ( descuento_importe ) ? ST( S_DESCUENTO___UTL_ART_ )
                                : ST( S_DESCUENTO___UTL_ART_ ), RAM_COD_FUNCION, SI ) ) {
                SET_MEMORY_INT( __ram_motivo_descuento, 0 );
                if( PEDIR_MOTIVO_EN_DESCUENTOS && !RAM_ANULACION ) {
                    SET_MEMORY_INT( __ram_motivo_descuento, ELEGIR_MOTIVO( NULL , _MOTIVO_DESCUENTO ) );
                }
                if( descuento_importe ) {
                    codigo = config.cod_descuento_importe3;
                }
                else {
                    codigo = config.cod_descuento_porc;
                }
                if( RAM_ANULACION ) {
                    importe = PEDIR_FLOAT_EN_RECUADRO( ST( S__IMPORTE_A_ANULAR_ ), SI, ENTEROS,
                                                       DECIMALES );
                    if( importe > 0 ) {
                        if( RAM_NEGATIVO || RAM_MODO_DEVOLUCION ) {
                            importe = -importe;
                        }
                        pos_des = 0;
                        for( h = 1, seguir = 1;h < RAM_P_TICKET && seguir;h++ ) {
                            if( ( ( _TICKET_ORIGEN( h ) == _ORIGEN_DESC_PESOS && descuento_importe )
                               || ( _TICKET_ORIGEN( h ) == _ORIGEN_DESC_PORC
                                 && !descuento_importe ) ) && _TICKET_TIPO2( h ) == 9
                             && ( fabs( _TICKET_PRECIO( h ) + importe ) < 0.01 ) ) {
                                anular = h;
                                //--- Verificamos que no este anulado el descuento.
                                for( j = ( pos_des ) ? pos_des + 1 : h + 1;
                                     j < RAM_P_TICKET && anular;j++ ) {
                                    if( ( ( _TICKET_ORIGEN( j ) == _ORIGEN_DESC_PESOS
                                         && descuento_importe )
                                       || ( _TICKET_ORIGEN( j ) == _ORIGEN_DESC_PORC
                                         && !descuento_importe ) ) && _TICKET_TIPO2( j ) == 9
                                     && ( fabs( _TICKET_PRECIO( j ) - importe ) < 0.01 ) ) {
                                        pos_des = j;
                                        anular = 0;
                                    }
                                }
                                if( anular ) {
                                    seguir = 0;
                                    memset( &acum_impuestos, 0, sizeof( acum_impuestos ) );
                                    j = anular - 1;
                                    ACUMULA_IMPUESTOS_ITEM( j, &acum_impuestos,
                                                            _TICKET_CANTIDAD( j )
                                                          * ( ( _TICKET_TIPO( j ) & _RESTA ) ? -1
                                                            : 1 ), -1, -1 );
                                    acumulados = &acum_impuestos;
                                    importe_ticket = acumulados->importe_sin_iva
                                                   + acumulados->exento + acumulados->iva1
                                                   + acumulados->iva2
                                                   + acumulados->monto_impuesto[0]
                                                   + acumulados->monto_impuesto[1]
                                                   + acumulados->monto_impuesto[2]
                                                   + acumulados->monto_impuesto[3];
                                    if( importe_ticket ) {
                                        porc = -( importe * 100.00 ) / importe_ticket;
                                        CALCULAR_BASE_DESCUENTOS( 7, porc, SI, j, SI );
                                        _AGREGAR_DESCUENTO( codigo, 0, acumulados, porc,
                                                            ( char )( ( descuento_importe )
                                                                    ? _ORIGEN_DESC_PESOS
                                                                    : _ORIGEN_DESC_PORC ), 9, 0.0,
                                                            0.0 );
                                    }
                                }
                            }
                        }
                        if( seguir ) {
                            MENSAJE_STRING( S_DESCUENTO_A_ANULAR_NO_ENCONTRADO );
                        }
                    }
                    SET_MEMORY_INT( __ram_anulacion, 0 );
                    LIMPIAR_RECUADRO_PETICION( 0 );
                }
                else {
                    if( descuento_importe ) {
                        importe = PEDIR_FLOAT_EN_RECUADRO( ST( S__IMPORTE_A_DESCONTAR_ ), SI,
                                                           ENTEROS, DECIMALES );
                    }
                    else {
                        importe = PEDIR_FLOAT_EN_RECUADRO( ST( S__PORCENTAJE_A_DESCONTAR_ ), SI, 2,
                                                           2 );
                    }
                    if( importe > 0 ) {
                        if( ( importe <= config.tope_descuento_ult_item_porc && !descuento_importe )
                         || ( importe <= config.tope_descuento_utl_item_imp && descuento_importe ) ) {
                            h = RAM_P_TICKET - 1;
                            porc_original = ( descuento_importe ) ? 0.0 : importe;
                            if( !descuento_importe ) {
                                importe = ROUND( fabs( _TICKET_PRECIO( h ) * _TICKET_CANTIDAD( h ) )
                                               * importe / 100.0, ENTEROS, DECIMALES );
                            }
                            if( importe <= fabs( _TICKET_PRECIO( h ) * _TICKET_CANTIDAD( h ) ) ) {
                                if( RAM_NEGATIVO || RAM_MODO_DEVOLUCION ) {
                                    importe = -importe;
                                }
                                memset( &acum_impuestos, 0, sizeof( acum_impuestos ) );
                                ACUMULA_IMPUESTOS_ITEM( h, &acum_impuestos,
                                                        _TICKET_CANTIDAD( h )
                                                      * ( ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1 ),
                                                        -1, -1 );
                                acumulados = &acum_impuestos;
                                importe_ticket = acumulados->importe_sin_iva + acumulados->exento
                                               + acumulados->iva1 + acumulados->iva2
                                               + acumulados->monto_impuesto[0]
                                               + acumulados->monto_impuesto[1]
                                               + acumulados->monto_impuesto[2]
                                               + acumulados->monto_impuesto[3];
                                if( importe_ticket ) {
                                    porc = ( importe * 100.00 ) / importe_ticket;
                                    CALCULAR_BASE_DESCUENTOS( 7, porc, SI, h, SI );
                                    _AGREGAR_DESCUENTO( codigo, 0, acumulados, porc,
                                                        ( char )( ( descuento_importe )
                                                                ? _ORIGEN_DESC_PESOS
                                                                : _ORIGEN_DESC_PORC ), 9, 0.0,
                                                        porc_original );
                                }
                            }
                            else {
                                MENSAJE_STRING( S_EL_IMPORTE_SUPERA_EL_MONTO_ORIGINAL );
                            }
                        }
                        else {
                            if( descuento_importe ) {
                                MENSAJE_STRING( S_IMPORTE_DE_DESCUENTO_EXCEDIDO );
                            }
                            else {
                                MENSAJE_STRING( S_PORCENTAJE_DE_DESCUENTO_EXCEDIDO );
                            }
                        }
                    }
                }
            }
        }
        else {
            MENSAJE_STRING( S_NO_SE_PUEDE_APLICAR_EL_DESCUENTO );
        }
        
    }
    #endif
}
/***************************************************************************/
void SET_FILTRO_ORIGEN_ARTICULO( int origen )
/***************************************************************************/
{
    #ifdef COMPILAR_DESCUENTOS
    filtro_origen_articulo = origen;
    #endif
}
/*****************************************************************************/
_elegir_motivo( int comando, char *cadena, int dato, struct _browse *b, double monto )
/*****************************************************************************/
{
    int rta = 0;
    int cod,cntSkip = 0;
    switch( comando ) {
        case _BUSCAR:
            MOSTRAR_CADENA( 29, 20, cadena );
            cod = atoi( cadena );
			SET_WHERE( "CONCEPTO = '%i' AND COD_MOTIVO >= '%i'",cod_concepto, cod );
			RUN_QUERY( NO );
            break;
        case _MOSTRAR:
            /*while ( p_motivo_sic->mostrar == (short) 1 )
               SKIP( 1 );*/
            {
                char xx[10];
                if( ( p_motivo_sic->mostrar == ( short )0 
                            && p_motivo_sic->concepto == cod_concepto )
                        || ( MODO_NEGOCIO == SERVICIOS && p_motivo_sic->mostrar != 0
                        && p_motivo_sic->mostrar == cobros.codigo ) ) {
                _itoa( p_motivo_sic->cod_motivo, xx, 10 );
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, p_motivo_sic->des_motivo );
                MOSTRAR_ENTERO( 29, 10 + dato, "99", p_motivo_sic->cod_motivo );
                MOSTRAR_CADENA( 32, 10 + dato, p_motivo_sic->des_motivo );
                } else { 
                    setFieldValuepChar( b->index, 0, "0" );
                    setFieldValuepChar( b->index, 1, ST( S_VALOR_POR_DEFECTO ) );
                    por_defecto = 1;
                }
            }
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
            //Desde las interfaces, mostrar = SIEMPRE = 0
            if( p_motivo_sic->mostrar != ( short )0
             || ( MODO_NEGOCIO == SERVICIOS && p_motivo_sic->mostrar != 0
               && p_motivo_sic->mostrar != cobros.codigo ) ) {
                SKIP2( -dato );
                rta = 1;
             } else {//Desde las interfaces, mostrar = 0 = SIEMPRE
                 if( p_motivo_sic->concepto != cod_concepto  ) {
                    SKIP2( -dato );
                    rta = 1;
                 }
             }
            break;
        case _VALIDAR:
            if( dato >= '0' && dato <= '9' ) {
                //rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
            rta = 1;
            if( por_defecto ) {
                rta = -1;
                por_defecto = 0;
            }
            break;
        case _TECLA:
            if( b->tecla == 27 ) {
                b->tecla = 0;
                rta = 1;
            }
            break;
    }
    return ( rta );
}
/***************************************************************************/
int ELEGIR_MOTIVO( int *cod_solicitud, int concepto )
/***************************************************************************/
{
    int rta = 0;
    struct _motivos motivo_sic;
    p_motivo_sic = &motivo_sic;
    /*---------------------------- Abre el archivo ---------------------------*/
	if( OPEN_TABLE( T_MOTIVOS, TT_ORIG, ( char* )&motivo_sic, sizeof( struct _motivos ) )  == 0 ) {
        /*-------------------------- Prepara la pantalla -------------------------*/
        /*#ifdef COMPILAR_DESCUENTOS
		SET_ORDER2( "MOSTRAR, COD_MOTIVO" );
		SET_WHERE( "MOSTRAR = '%i' AND COD_MOTIVO >= '%i'", cobros.codigo, 0 );
        #else*/
        //Se reutiliza ind_motivo.mostrar porque tiene igual tipo de dato que concepto
		//SET_ORDER2("CONCEPTO, COD_MOTIVO");
		//SET_WHERE( "CONCEPTO = '%i' AND COD_MOTIVO >= '%i'", concepto, 0 );
        //#endif
		//RUN_QUERY( NO );
        //Se usa en la busqueda del browser
        cod_concepto = /*ind_motivo.mostrar*/ concepto; 
        rta = Consulta_elegir_motivo( _elegir_motivo );
        if( rta > 0 ) {
            rta = motivo_sic.cod_motivo;
            if( cod_solicitud != NULL ) {
                *cod_solicitud = motivo_sic.cod_solicitud;
            }
        }
		CLOSE_TABLE( T_MOTIVOS, TT_ORIG );
    }

    return ( rta );
}
/***************************************************************************/
void AGREGAR_DESCUENTO_PORC_CLIENTE( float porc_desc )
/***************************************************************************/
{
    #ifdef COMPILAR_DESCUENTOS
    double importe_ticket,porcentaje,porc;
    struct _acu_iva *acumulados;
    acumulados = ACUMULAR_IMPUESTOS_TICKET( NO, 0, -1, -1, SI );
    importe_ticket = acumulados->importe_sin_iva + acumulados->exento + acumulados->iva1
                   + acumulados->iva2 + acumulados->monto_impuesto[0]
                   + acumulados->monto_impuesto[1] + acumulados->monto_impuesto[2]
                   + acumulados->monto_impuesto[3];
    porcentaje = ROUND( importe_ticket * porc_desc / 100, 10, 2 );
    if( importe_ticket ) {
        porc = 100 * porcentaje / importe_ticket;
    }
    else {
        porc = porc_desc;
    }
    _AGREGAR_DESCUENTO( config.cod_descuento_cliente, 0,
                        ACUMULAR_IMPUESTOS_TICKET( NO, 0, -1, -1, SI ), porc, _ORIGEN_DESC_PORC, 0,
                        0.0, 0.0 );
    #endif
}
/***************************************************************************/
int VERIFICAR_MOTIVO( int tipo_verificacion, int *cod_solicitud, short *supervisor, int cod_motivo )
/***************************************************************************/
{
    int rta = 0;
    int area_ant = SELECTED2( );
    int tipo_ant = SELECTED_TIPO();

    struct _motivos motivo_sic;
    p_motivo_sic = &motivo_sic;
    *supervisor = ( short )0;
	if( !OPENED( T_MOTIVOS, TT_ORIG ) ) {
		if( OPEN_TABLE( T_MOTIVOS, TT_ORIG, ( char* )&motivo_sic, sizeof( struct _motivos ) ) ) {
			SET_WHERE("COD_MOTIVO = '%d'", cod_motivo );
			RUN_QUERY( NO );
			if( FOUND2() ) {
                rta = 1;
                switch( tipo_verificacion ) {
                    case _MOTIVO_OBTENER_CODSOLICITUD:
                        *cod_solicitud = 0;
                        if( cod_solicitud != NULL ) {
                            *cod_solicitud = motivo_sic.cod_solicitud;
                        }
                        break;
                    case _MOTIVO_OBTENER_SUPERVISOR:
                        if( supervisor != NULL ) {
                            *supervisor = motivo_sic.supervisor;
                        }
                        break;
                    case _MOTIVO_OBTENER_CODSOLYSUPER:
                        *cod_solicitud = 0;
                        if( cod_solicitud != NULL ) {
                            *cod_solicitud = motivo_sic.cod_solicitud;
                        }
                        if( supervisor != NULL ) {
                            *supervisor = motivo_sic.supervisor;
                        }
                        break;
                }
            }
			CLOSE_TABLE( T_MOTIVOS, TT_ORIG );
        }
    }
    else {
        GRABAR_LOG_SISTEMA( "ERROR EN AREA AUX PARA MOTIVO" , LOG_ERRORES,2 );
    }
	SELECT_TABLE( area_ant, tipo_ant );

    return ( rta );
}

