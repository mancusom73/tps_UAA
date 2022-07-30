/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

#pragma pack(1)
#include <cr.h>
#include <cr_disco.h>
#include <promo.h>
#include <config.h>
#include <stdlib.h>
#include <math.h>
#include <aarch.h>
#include <btrv.h>
#include <string.h>
#include <cmemory.h>
#include <_cr1.h>
#include <round.h>
#include <imp_tick.h>
#include <actual.h>
#include <recup.h>
#include <ext_tick.h>
#undef MK_FP
#ifdef INVEL_L
#include <sys/stat.h>
#else
#include <dos.h>
#include <sys\stat.h>
#endif
#include <time.h>
#include <typeacti.h>
#include <actions.h>

static int bonos_on = 0;
static int bonos_ant = 0;
void MUESTRA_BULTOS();
void MUESTRA_SUBTOTAL( int recalcular);
double SUMAR_TOTAL_FAC_CAJA( int recalcular );
void MUESTRA_ITEM_EN_PANTALLA( int nro );

/*****************************************************************************/
void ACUMULA_IVA( int h, struct _acu_iva *acu, double cantidad )
/*****************************************************************************/
{
    //double    recargo_neto, imp_iva1, imp_iva2, imp_ila, imp_iha;
    double recargo_neto,imp_iva1,imp_iva2;

    recargo_neto = _TICKET_RECARGO( h ) - _TICKET_IVA1_REC( h ) - _TICKET_IVA2_REC( h );
    imp_iva1 = _TICKET_IVA1_PRECIO( h ) + _TICKET_IVA1_REC( h );
    imp_iva2 = _TICKET_IVA2_PRECIO( h ) + _TICKET_IVA2_REC( h );
    //imp_ila = _TICKET_ILA_PRECIO( h ) + _TICKET_ILA_REC( h );
    //imp_iha = _TICKET_IHA_PRECIO( h ) + _TICKET_IHA_REC( h );
    acu->importe_sin_iva += ( _TICKET_IMPORTE_SIN_IVA( h ) + recargo_neto ) * cantidad;
    acu->exento += _TICKET_EXENTO( h ) * cantidad;
    acu->iva1 += imp_iva1 * cantidad;
    acu->iva2 += imp_iva2 * cantidad;

    //acu->ila += imp_ila * cantidad;
    //acu->iha += imp_iha * cantidad;
    //acu->alicuota += _TICKET_ALICUOTA_PRECIO( h ) * cantidad;
    acu->monto_impuesto[0] += _TICKET_IMPUESTO1_PRECIO( h ) * cantidad;
    acu->monto_impuesto[1] += _TICKET_IMPUESTO2_PRECIO( h ) * cantidad;
    acu->monto_impuesto[2] += _TICKET_IMPUESTO3_PRECIO( h ) * cantidad;
    acu->monto_impuesto[3] += _TICKET_IMPUESTO4_PRECIO( h ) * cantidad;
}

/*****************************************************************************/
int CARGA_ACU_ART( struct _acu_art *acu_art, int l, int permite_exclusion_art, double *total_cobros,
                   int agrupar_des, int agrupar_precio )
/*****************************************************************************/
{
    char barra1[17],barra2[17];
    int h, i, signo, encontro, p_art = 0, agrupar, ok = 0;
    float cantidad;

    if( acu_art ) {
        memset( acu_art, 0, l );
        if( total_cobros ) {
            *total_cobros = 0.0;
        }
        for( h = 0;h < RAM_P_TICKET;h++ ) {
            signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
            cantidad = _TICKET_CANTIDAD( h ) * signo;
            encontro = 0;

            if( total_cobros && _TICKET_COD_COBRO( h ) ) {
                *total_cobros += _TICKET_PRECIO( h ) * cantidad;
            }
            agrupar = 1;
            if( !agrupar_des ) {
                if( ( _TICKET_ORIGEN( h ) == _ORIGEN_DESC_PESOS
                   || _TICKET_ORIGEN( h ) == _ORIGEN_DESC_PORC
                   || _TICKET_ORIGEN( h ) == _ORIGEN_PROMOCION ) && signo > 0 ) {
                    agrupar = 0;
                }
            }
            if( !agrupar_precio ) {
                //if( ( _TICKET_TIPO2( h ) & _PEDIR_PRECIO ) && signo > 0 ) {
                strncpy( barra1, _TICKET_COD_BARRA( h ), sizeof( barra1 ) );
                strncpy( barra2, _TICKET_COD_BARRA_ORIGINAL( h ), sizeof( barra2 ) );                           
                if( ( ( _TICKET_TIPO2( h ) & _PEDIR_PRECIO )
                   || ( fabs( fmod( _TICKET_CANTIDAD( h ), 1.00 ) ) > 0.0005 )
                   || strcmp( barra1, barra2 ) ) && signo > 0 ) {
                    agrupar = 0;
                }
            }

            for( i = 0;i < p_art && !encontro && agrupar;i++ ) {
                if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
                    if( _TICKET_COD_ESPECIAL( h ) == acu_art[i].cod
                     && _TICKET_PRECIO( h ) == _TICKET_PRECIO( acu_art[i].pos )
                     && ( strcmp( _TICKET_COD_BARRA( h ), acu_art[i].cod_barra ) == 0 ) ) {
                        encontro = 1;
                        if( encontro
                         && ( ( _TICKET_TIPO( h ) & _DEV_ENVASE )
                           && !( _TICKET_TIPO( acu_art[i].pos ) & _DEV_ENVASE ) ) ) {
                            encontro = 0;
                        }
                    }
                }
                else {
                    if( _TICKET_COD_ESPECIAL( h ) == acu_art[i].cod
                     && _TICKET_PRECIO( h ) == _TICKET_PRECIO( acu_art[i].pos ) ) {
                        encontro = 1;
                    }
                }
                if( encontro ) {
                    acu_art[i].cant += cantidad;
                    acu_art[i].descuento_cantidad += _TICKET_DESCUENTO_CANTIDAD( h );
                    if( ( _TICKET_PRECIO( h ) < _TICKET_PRECIO( acu_art[i].pos ) )
                     || ( _TICKET_PRECIO( h ) > _TICKET_PRECIO( acu_art[i].pos ) ) ) {
                        cantidad -= 1;
                        acu_art[i].cant = acu_art[i].cant - 1;
                    }
                }
                /*
                        if( _TICKET_COD_ESPECIAL( h ) == acu_art[i].cod
                            && _TICKET_PRECIO( h ) == _TICKET_PRECIO( acu_art[i].pos ) ) {
                            encontro = 1;
                            acu_art[i].cant += cantidad;
                            acu_art[i].descuento_cantidad += _TICKET_DESCUENTO_CANTIDAD( h );
                        }
                        if( _TICKET_COD_ESPECIAL( h ) == acu_art[i].cod && fabs( _TICKET_PRECIO( h ) ) == fabs( _TICKET_PRECIO( acu_art[i].pos ) ) //){
                            && ( ( _TICKET_PRECIO( h ) < _TICKET_PRECIO( acu_art[i].pos ) )
                                || ( _TICKET_PRECIO( h ) > _TICKET_PRECIO( acu_art[i].pos ) ) ) ) {
                            cantidad -= 1;
                            acu_art[i].cant = acu_art[i].cant - 1;
                        }*/
            }
            if( !encontro ) {
                //para items con otras promociones
                if( RAM_PERFIL_EXCLUIR_ARTICULOS == 'D' && RAM_PERFIL ) {
					cantidad = _TICKET_CANTIDAD( h ) ;/*- _TICKET_CANT_EXCL_DESC( h );*/
                }

                if( cantidad && ( ARTICULO_INCLUIBLE_EN_PROMOCION( h ) || !permite_exclusion_art ) ) {
                    acu_art[p_art].cod = _TICKET_COD_ESPECIAL( h );
                    acu_art[p_art].cant = cantidad;
                    acu_art[p_art].pos = h;
                    acu_art[p_art].importe = _TICKET_PRECIO( h );
                    acu_art[p_art].cod_promocion[0] = _TICKET_COD_PROMOCION( h );
                    acu_art[p_art].cod_promocion[1] = _TICKET_COD_PROMOCION1( h );
                    acu_art[p_art].cod_promocion[2] = _TICKET_COD_PROMOCION2( h );
                    acu_art[p_art].cod_promocion[3] = _TICKET_COD_PROMOCION3( h );
                    acu_art[p_art].descuento_cantidad = _TICKET_DESCUENTO_CANTIDAD( h );
                    memcpy( acu_art[p_art].cod_barra, _TICKET_COD_BARRA( h ), 17 );
                    p_art++;
                }
            }
        }
    }

    return ( p_art );
}


/*****************************************************************************/
void AGREGAR_DESCUENTO_ARTICULO( long cod_art, char* cod_bar, double precio, float cantidad, int id_registro, int origen )
/*****************************************************************************/
{
	int tipo = id_registro;
	if ( cod_bar && cod_art > 0 ) {
		if ( !ASIGNA_COD_ESPECIAL_COD_BARRA( cod_art, cod_bar ) ) {
			glog( "PROMO: AGREGAR_DESCUENTO_ARTICULO: No se encontró el artículo. No se agrega al ticket." , LOG_MOTOR_PROMO,3);
			return;
		}
	} else if ( cod_bar ) {
		if ( !ASIGNA_CODIGO_BARRA( cod_bar ) ) {
			glog( "PROMO: AGREGAR_DESCUENTO_ARTICULO: No se encontró el artículo. No se agrega al ticket." , LOG_MOTOR_PROMO,3);
			return;
		}
	} else {
		if ( !ASIGNA_CODIGO_ESPECIAL( cod_art ) ) {
			glog( "PROMO: AGREGAR_DESCUENTO_ARTICULO: No se encontró el artículo. No se agrega al ticket.", LOG_MOTOR_PROMO,3);
			return;
		}
	}

	if ( cantidad < 0 ) {
		cantidad = - cantidad;
		SET_MEMORY_INT( __ram_anulacion, 1 );
		tipo |= _RESTA;
	} else {
		SET_MEMORY_INT( __ram_anulacion, 0 );
	}

    //SET_MEMORY_DOUBLE( __item_precio, precio );
    SET_MEMORY_DOUBLE( __ram_cantidad, cantidad );
	//SET_MEMORY_INT( __ram_anulacion, ( cantidad >= 0 ? 0 : 1 ) );
	//SET_MEMORY_INT( __ram_anulacion, 0 );
    SET_MEMORY( __ram_cod_barra_original, ITEM_COD_BARRA );
    AGREGA_ITEM_AL_TICKET( RAM_P_TICKET, RAM_ANULACION, NO, NO, origen );
    _SET_MEMORY_CHAR( __ticket_origen, RAM_P_TICKET, origen );
    //_SET_MEMORY_CHAR( __ticket_tipo2, RAM_P_TICKET, ( char )id_registro );
    _SET_MEMORY_CHAR( __ticket_tipo2, RAM_P_TICKET, 0 );
	_SET_MEMORY_CHAR( __ticket_tipo, RAM_P_TICKET, tipo );
	//RECALCULAR_PRECIO_PROMOCION( RAM_P_TICKET, precio );
    ADD_MEMORY_INT( __ram_p_ticket, 1 );
    SET_MEMORY_DOUBLE( __ram_total,
		ROUND( RAM_TOTAL + ITEM_PRECIO * RAM_CANTIDAD * ( RAM_ANULACION ? -1 : 1), 10, DECIMALES ) );
    if( DEBE_IMPRIMIR_ITEM() ) {
        IMPRIME_ITEM( RAM_P_TICKET - 1 );
		if( IMPRESORA_FISCAL == FISCAL) {		
			MUESTRA_BULTOS(NO);
			MUESTRA_SUBTOTAL(NO);
		/*	MUESTRA_ITEM_EN_PANTALLA( RAM_P_TICKET - 1 );
			MUESTRA_DATOS_CONTADOR_PANTALLA( RAM_P_TICKET - 1 , SI );*/
		}

    }
    ACTUALIZA_ACUMULADORES_RAM( RAM_P_TICKET - 1 );
    GRABAR_EVENT_E_TICKET( RAM_P_TICKET - 1, NO, SI, origen, tipo, NO );
    GRABAR_TICKET( RAM_P_TICKET - 1 );

    /*------------------ muestra datos en la pantalla --------------------*/
    //MUESTRA_BULTOS();
    //MUESTRA_SUBTOTAL();
    //MUESTRA_ITEM_EN_PANTALLA( RAM_P_TICKET - 1 );
	//MUESTRA_DATOS_CONTADOR_PANTALLA( RAM_P_TICKET - 1 , SI );

}


/*****************************************************************************/
int ARTICULO_INCLUIBLE_EN_PROMOCION( int h )
/*****************************************************************************/
{
    int rta = 0;
    #ifdef COMPILAR_PROMOCIONES
    if( config.pais == ELSALVADOR ) {
        rta = ( ( _TICKET_TIPO2( h ) & _HABILITADO_PROMO ) ? 1 : 0 )
           && ( ( _TICKET_TIPO( h ) & _CREDITO ) ? 0 : 1 );
    }
    else {
        rta = ( _TICKET_TIPO2( h ) & _HABILITADO_PROMO ) ? 1 : 0;
    }
    #endif
    return ( rta );
}


/*****************************************************************************/
void MOSTRAR_BONOS( void )
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    char cad[50];   

    int continuar = 0;

    if( UTILIZA_MODULO_PROMOCIONES ) {
        //if( bonos_on && ( bonos_ant != RAM_BONOS_A_ENTREGAR )&& RAM_BONOS_A_ENTREGAR ) {
        if( bonos_on && RAM_BONOS_A_ENTREGAR ) {
            continuar = 1;
        }
    }
    else {
        if( bonos_on && ( bonos_ant == RAM_BONOS_A_ENTREGAR ) && RAM_BONOS_A_ENTREGAR ) {
            continuar = 1;
        }
    }

    if( continuar ) {
        sprintf( cad, "BONOS A ENTREGAR: %03d", RAM_BONOS_A_ENTREGAR );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_BONOS_BORRAR, " ", " ", NULL, " " );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_BONOS_AGREGAR, cad, " ", NULL, " " );
        bonos_ant = RAM_BONOS_A_ENTREGAR;
    }
    else if( bonos_on && !RAM_BONOS_A_ENTREGAR ) {
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_BONOS_BORRAR, " ", " ", NULL, " " );
        bonos_ant = RAM_BONOS_A_ENTREGAR;
        bonos_on = 0;
    }
    else if( RAM_BONOS_A_ENTREGAR && !bonos_on ) {
        sprintf( cad, "BONOS A ENTREGAR: %03d", RAM_BONOS_A_ENTREGAR );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_BONOS_AGREGAR, cad, " ", NULL, " " );
        bonos_ant = RAM_BONOS_A_ENTREGAR;
        bonos_on = 1;
    }
    #else
    if( RAM_BONOS_A_ENTREGAR ) {
        _ATRIBUTO = 15;
        MOSTRAR_CADENA( 1, 23, ST( S_BONOS_A_ENTREGAR__ ) );
        MOSTRAR_ENTERO( 20, 23, "999", RAM_BONOS_A_ENTREGAR );
    }
    #endif
}

/*****************************************************************************/
void _AGREGAR_PROMOCION_DESCUENTO( struct _acu_iva *acu, long cod_art, char* cod_bar, float porc_desc,
                                   char *procesar, int id_registro, double **base_descuento,
                                   char origen, int exento, int cod_iva )
/*****************************************************************************/
{
    double porc = 0.0, precio = 0.0, precio_ok = 0.0;
    double tot_i = 0.0;
    int i = 0;

    ADD_MEMORY_INT( __ram_cant_promociones, 1 );

	if ( cod_bar == NULL ) {
		if ( !ASIGNA_CODIGO_ESPECIAL( cod_art ) ) {
			MENSAJE_STRING(S_ARTICULO_DE_DESCARGA_INEXISTE);
			glog( "PROMO: AGREGAR_PROMOCION_DESCUENTO: No se encuentra el articulo de descarga. Descuento NO aplicado" , LOG_MOTOR_PROMO,3);
			BORRAR_MENSAJE();
			return;
		}
	} else {
		if ( !ASIGNA_COD_ESPECIAL_COD_BARRA( cod_art, cod_bar ) ) {
			MENSAJE_STRING(S_ARTICULO_DE_DESCARGA_INEXISTE);
			glog( "PROMO: AGREGAR_PROMOCION_DESCUENTO: No se encuentra el articulo de descarga. Descuento NO aplicado" , LOG_MOTOR_PROMO,3);
			BORRAR_MENSAJE();
			return;
		}
	}

    porc = -1 * porc_desc / 100;

    for( i = 0;i < 4;i++ ) {
        tot_i += acu->monto_impuesto[i];
    }

    precio = acu->importe_sin_iva + acu->exento + acu->iva1 + acu->iva2 + tot_i;

    // Recalculamos el porcentaje para que de exacto el precio final
    precio_ok = ROUND( porc * precio, ENTEROS, DECIMALES );
    if( precio != 0 )
          //porc = ROUND( precio_ok / precio, ENTEROS, DECIMALES );
    {
        porc = precio_ok / precio;
    }
    else {
		glog("PROMO: ERROR aplicando descuentos sobre articulos con precio 0. DIV_BY_ZERO", LOG_MOTOR_PROMO,3);
        porc = -1;
    }
//    SET_MEMORY_DOUBLE( __item_precio, precio_ok );
    SET_MEMORY_DOUBLE( __item_precio, precio_ok );
	//SET_MEMORY_DOUBLE( __item_precio_sin_iva, precio_ok );
    SET_MEMORY_DOUBLE( __item_precio_sin_iva, acu->importe_sin_iva + acu->exento );
	{
		double imp_intX = acu->imp_int ;
		ROUND2(&imp_intX, ENTEROS,2);
		SET_MEMORY_DOUBLE( __item_imp_int, imp_intX );
	//SET_MEMORY_DOUBLE( __item_imp_int, acu->imp_int );
	}

	
	
	
	// Esto se debe pasar por parametro para saber si se trata de un excento o un gravado,
	// en cuyo caso tambien se debera pasar el codigo de iva asociado
	if( exento ) {
	//if( acu->exento ) {
        SET_MEMORY_CHAR( __item_gravado, 0 );
		SET_MEMORY_CHAR( __item_cod_iva, 0 );
    }
    else {
        SET_MEMORY_CHAR( __item_gravado, 1 );
		SET_MEMORY_CHAR( __item_cod_iva, cod_iva );
		//SET_MEMORY_CHAR( __item_cod_iva,INDICE_IVA_ARTICULO());
		//SET_MEMORY_CHAR( __item_cod_iva, 2 ); cambio sugerido por BO para el chek control, hasta que se implemente la solucion completa
    }
    SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
//    SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
    SET_MEMORY_INT( __ram_anulacion, 0 );
    AGREGA_ITEM_AL_TICKET( RAM_P_TICKET, 0, NO, NO, origen );
    _SET_MEMORY_CHAR( __ticket_origen, RAM_P_TICKET, origen );
    _SET_MEMORY_DOUBLE( __ticket_precio, RAM_P_TICKET, ITEM_PRECIO );
	//cuando revierte la promo usa ticket_importe_sin_iva pero cuando lo hace usa item_precio_sin_iva y ahi puede variar los importes

	if( IMPRESORA_FISCAL == FISCAL && (RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1)) {
		int porce;
		if( porc < 0.0)
			porce = -1;
		else
			porce = 1;
		
		porc = porce;
		_SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, RAM_P_TICKET, porc * (acu->importe_sin_iva)) ;	
	}else
    _SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, RAM_P_TICKET, ROUND( porc * acu->importe_sin_iva, ENTEROS, 4 /*DECIMALES*/ ) );
    
    _SET_MEMORY_DOUBLE( __ticket_exento, RAM_P_TICKET, ROUND( porc * acu->exento, ENTEROS, DECIMALES ) );
    _SET_MEMORY_DOUBLE( __ticket_iva1_precio, RAM_P_TICKET, ROUND( porc * acu->iva1 , ENTEROS, 6/*DECIMALES*/ ) );
    _SET_MEMORY_DOUBLE( __ticket_iva2_precio, RAM_P_TICKET, ROUND( porc * acu->iva2 , ENTEROS, 6/*DECIMALES*/ ) );
	if( IMPRESORA_FISCAL == FISCAL && ( RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1))
		_SET_MEMORY_DOUBLE( __ticket_imp_int, RAM_P_TICKET, ROUND( porc * acu->imp_int , ENTEROS,2 ) );
	else
	_SET_MEMORY_DOUBLE( __ticket_imp_int, RAM_P_TICKET, ROUND( porc * acu->imp_int , ENTEROS, 6/*DECIMALES*/ ) );

    //_SET_MEMORY_FLOAT( __ticket_ila_precio, RAM_P_TICKET, porc * acu->ila );
    //_SET_MEMORY_FLOAT( __ticket_iha_precio, RAM_P_TICKET, porc * acu->iha );
    //_SET_MEMORY_FLOAT( __ticket_alicuota_precio, RAM_P_TICKET, porc * acu->alicuota );
    _SET_MEMORY_FLOAT( __ticket_impuesto1_precio, RAM_P_TICKET, ROUND( porc * acu->monto_impuesto[0], ENTEROS, DECIMALES ) );
    _SET_MEMORY_FLOAT( __ticket_impuesto2_precio, RAM_P_TICKET, ROUND( porc * acu->monto_impuesto[1], ENTEROS, DECIMALES ) );
    _SET_MEMORY_FLOAT( __ticket_impuesto3_precio, RAM_P_TICKET, ROUND( porc * acu->monto_impuesto[2], ENTEROS, DECIMALES ) );
    _SET_MEMORY_FLOAT( __ticket_impuesto4_precio, RAM_P_TICKET, ROUND( porc * acu->monto_impuesto[3], ENTEROS, DECIMALES ) );

    _SET_MEMORY_DOUBLE( __ticket_recargo, RAM_P_TICKET, 0 );
    //_SET_MEMORY_DOUBLE( __ticket_imp_int, RAM_P_TICKET, 0 );
    _SET_MEMORY_DOUBLE( __ticket_iva1_rec, RAM_P_TICKET, 0 );
    _SET_MEMORY_DOUBLE( __ticket_iva2_rec, RAM_P_TICKET, 0 );
    //_SET_MEMORY_FLOAT( __ticket_ila_rec, RAM_P_TICKET, 0 );
    //_SET_MEMORY_FLOAT( __ticket_iha_rec, RAM_P_TICKET, 0 );
    _SET_MEMORY_CHAR( __ticket_tipo2, RAM_P_TICKET, ( char )id_registro );
    _SET_MEMORY_DOUBLE( __ticket_costo, RAM_P_TICKET, 0.0 );
    DISCRIMINA_IMPUESTOS_EN_ARTICULO( porc, procesar );
	ADD_MEMORY_INT( __ram_p_ticket, 1 );
//    SET_MEMORY_DOUBLE( __ram_total, ROUND( RAM_TOTAL + ITEM_PRECIO, 10, DECIMALES ) );
    SET_MEMORY_DOUBLE( __ram_total, ROUND( RAM_TOTAL + ITEM_PRECIO, ENTEROS, DECIMALES ) );
    if( DEBE_IMPRIMIR_ITEM() ) {
        IMPRIME_ITEM( RAM_P_TICKET - 1 );
    }
//    #if defined(INVEL_W) || defined(INVEL_L)
//		MUESTRA_SUBTOTAL(  );
//		MUESTRA_ITEM_EN_PANTALLA( RAM_P_TICKET - 1 );
//
//    #endif

	//mmancuso para promos pospago resto total
	/************************************************/ 
	ADD_MEMORY_DOUBLE( __ram_resto, precio_ok );

    ACTUALIZA_ACUMULADORES_RAM( RAM_P_TICKET - 1 );
    GRABAR_EVENT_E_TICKET( RAM_P_TICKET - 1, NO, SI, origen, id_registro, NO );
    GRABAR_TICKET( RAM_P_TICKET - 1 );
	if( IMPRESORA_FISCAL == FISCAL ){
		if( RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR ==1 ) {
			ADD_MEMORY_DOUBLE( __ram_puntos_esp_3, _TICKET_PRECIO( RAM_P_TICKET - 1 ));
		}
	SUMAR_TOTAL_FAC_CAJA( SI); 
	}
	MUESTRA_ITEM_EN_PANTALLA( RAM_P_TICKET - 1 );
    /*if( *base_descuento && config.pais == ARGENTINA ) {
        double precio_acumulado = 0.00;
        UBICAR_DESCUENTO( &precio_acumulado );
        ( *base_descuento )[h_ultimo_descuento] -= ( total_descuentos + precio_acumulado );
    }*/
}

/***************************************************************************************************/


/*****************************************************************************/
void ACUMULA_IVA_ART( int h, long codInt, char* codBar, struct _acu_iva (*acu_gravados)[], struct _acu_iva *acu_exento, double monto )
/*****************************************************************************/
// Acumula impuestos del articulo indicado, de forma proporcional, de modo de acumular 'monto'.
{
    //double    recargo_neto, imp_iva1, imp_iva2, imp_ila, imp_iha;
    double recargo_neto = 0, imp_iva1 = 0, imp_iva2 = 0;
	double porc = 0;
	struct _acu_iva *acu;

    if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
        BUSCA_COD_INTERNO_COD_BARRA( codInt, codBar );
    }
    else {
        BUSCA_CODIGO_ESPECIAL( codInt );
    }        

	if ( articulo.gravado == 'S' ) {
	//if ( articulo.cod_iva != 0 ) {
		acu = &((struct _acu_iva *)acu_gravados)[articulo.cod_iva];
	} else {
		acu = acu_exento;
	}

//	porc = monto / ITEM_PRECIO;
	porc = monto;
/*
    recargo_neto = ITEM_RECARGO - ITEM_IVA1_REC - ITEM_IVA2_REC;
    imp_iva1 = ITEM_IVA1_PRECIO + ITEM_IVA1_REC;
    imp_iva2 = ITEM_IVA2_PRECIO + ITEM_IVA2_REC;
    //imp_ila = ITEM_ILA_PRECIO + ITEM_ILA_REC;
    //imp_iha = ITEM_IHA_PRECIO + ITEM_IHA_REC;
*/
    recargo_neto = _TICKET_RECARGO( h ) - _TICKET_IVA1_REC( h ) - _TICKET_IVA2_REC( h );
    imp_iva1 = _TICKET_IVA1_PRECIO( h ) + _TICKET_IVA1_REC( h );
    imp_iva2 = _TICKET_IVA2_PRECIO( h ) + _TICKET_IVA2_REC( h );
    //imp_ila = _TICKET_ILA_PRECIO( h ) + _TICKET_ILA_REC( h );
    //imp_iha = _TICKET_IHA_PRECIO( h ) + _TICKET_IHA_REC( h );

	if ( articulo.gravado == 'S' ) {
	//if ( articulo.cod_iva != 0 ) {
		//acu->importe_sin_iva += ( ITEM_PRECIO_SIN_IVA + recargo_neto ) * porc;
		//acu->exento += 0;
    }
    else {
		//acu->importe_sin_iva += 0;
		//acu->exento += ( ITEM_PRECIO_SIN_IVA + recargo_neto ) * porc;
    }   
	
    //acu->importe_sin_iva += ( _TICKET_IMPORTE_SIN_IVA( h ) + recargo_neto ) * cantidad;
    //acu->exento += _TICKET_EXENTO( h ) * cantidad;
    acu->importe_sin_iva += ( _TICKET_IMPORTE_SIN_IVA( h ) + recargo_neto ) * porc;
    acu->exento += _TICKET_EXENTO( h ) * porc;
    acu->iva1 += imp_iva1 * porc;
    acu->iva2 += imp_iva2 * porc;	

    //acu->ila += imp_ila * cantidad;
    //acu->iha += imp_iha * cantidad;
    //acu->alicuota += ITEM_ALICUOTA_PRECIO( h ) * cantidad;
    //acu->monto_impuesto[0] += _ITEM_MONTO_IMPUESTO(0) * porc;
    //acu->monto_impuesto[1] += _ITEM_MONTO_IMPUESTO(1) * porc;
    //acu->monto_impuesto[2] += _ITEM_MONTO_IMPUESTO(2) * porc;
    //acu->monto_impuesto[3] += _ITEM_MONTO_IMPUESTO(3) * porc;
    acu->monto_impuesto[0] += _TICKET_IMPUESTO1_PRECIO( h ) * porc;
    acu->monto_impuesto[1] += _TICKET_IMPUESTO2_PRECIO( h ) * porc;
    acu->monto_impuesto[2] += _TICKET_IMPUESTO3_PRECIO( h ) * porc;
    acu->monto_impuesto[3] += _TICKET_IMPUESTO4_PRECIO( h ) * porc;
	acu->imp_int += _TICKET_IMP_INT( h ) * porc;
}


/*

{
    //double    recargo_neto, imp_iva1, imp_iva2, imp_ila, imp_iha;
    double recargo_neto = 0, imp_iva1 = 0, imp_iva2 = 0;
	double porc = 0;
	struct _acu_iva *acu;

    if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
        BUSCA_COD_INTERNO_COD_BARRA( codInt, codBar );
    }
    else {
        BUSCA_CODIGO_ESPECIAL( codInt );
    }        

	if (ITEM_GRAVADO) {
		acu = acu_gravado;
	} else {
		acu = acu_excento;
	}

	porc = monto / ITEM_PRECIO;

    recargo_neto = ITEM_RECARGO - ITEM_IVA1_REC - ITEM_IVA2_REC;
    imp_iva1 = ITEM_IVA1_PRECIO + ITEM_IVA1_REC;
    imp_iva2 = ITEM_IVA2_PRECIO + ITEM_IVA2_REC;

    
    if( ITEM_GRAVADO  ) {
		acu->importe_sin_iva += ( ITEM_PRECIO_SIN_IVA + recargo_neto ) * porc;
		acu->exento += 0;
    }
    else {
		acu->importe_sin_iva += 0;
		acu->exento += ( ITEM_PRECIO_SIN_IVA + recargo_neto ) * porc;
    }   
	
	
	
	
	acu->iva1 += imp_iva1 * porc;
    acu->iva2 += imp_iva2 * porc;

    acu->monto_impuesto[0] = _ITEM_MONTO_IMPUESTO(0) * porc;
    acu->monto_impuesto[1] = _ITEM_MONTO_IMPUESTO(1) * porc;
    acu->monto_impuesto[2] = _ITEM_MONTO_IMPUESTO(2) * porc;
    acu->monto_impuesto[3] = _ITEM_MONTO_IMPUESTO(3) * porc;

}


  */