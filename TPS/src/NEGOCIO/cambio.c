#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <global.h>
#include <tpv_dat.h>
#include <aarch.h>
#include <btrv.h>
#include <cambio.h>
#include <clave.h>
#include <cmemory.h>
#include <comprob.h>
#include <cr.h>
#include <cr_pant.h>
#include <dbrouter.h>
#include <getch.h>
#include <import_t.h>
#include <imp_tick.h>
#include <log.h>
#include <menu.h>
#include <mstring.h>
#include <_cr1.h>
#include <deftables.h>
#include <db.h>
#include <trim.h>
#include <cr_disco.h>
#include <mensaje.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_TECLA_CAMBIO
struct _tipos_de_comprob {
    BINARY status;
    char descripcion[25];
    INT16 codigo;
    INT16 tipo_evento_comp;
    INT16 pais;
    INT16 validacion_cab;
	INT16 validacion_cpo;
	INT16 validacion_pie;
	INT16 tipo_comprobante;
	BINARY fin;
};

double IMPORTE_TOTAL_ITEM( void );
int INVERTIR_SIGNO_PRECIO( void );
int ram_diferencia_precio;
extern long item_cod_enganche;
/***************************************************************************/
void TECLA_CAMBIO()
/***************************************************************************/
{
    int error = 0, ok = 1;
    int caja = 0, limpiar_all = 0;
    long secuencia = 0L;
    int fecha = 0;
    switch( RAM_CAMBIO ) {
        case 0:
            /*--------- Inicio del modo cambio ---------*/
            if( !RAM_P_TICKET
             && SOLICITAR_CLAVES( 29, 19, NO, SI, PEDIR_SUPERVISOR_EN_CAMBIO, ST( S__CAMBIO_ ),
                                  ST( S_CAMBIO ), RAM_COD_FUNCION, SI ) ) {
                if( PEDIR_CAJA_Y_SECUENCIA ) {
                    ok = INGRESAR_CAJA_Y_SECUENCIA( &caja, &secuencia, &fecha );
                    if( ok && DEBE_PEDIR_BOLETA_ANTERIOR ) {
                        ok = PEDIR_BOLETA_ANTERIOR( NO );
                    }
                    if( ok ){
                        SET_MEMORY_INT( __ram_caja_anterior, caja );
                        SET_MEMORY_INT( __ram_fecha_anterior, fecha );
                        if( !RAM_TIPO_COMP_ANTERIOR ) {
                            SET_MEMORY_CHAR( __ram_tipo_comp_anterior, 1 );//ticket común
                        }
                    }
                } else {
                    ok = PEDIR_BOLETA_ANTERIOR( NO );
                     if( ok && REFERENCIAR_POR_SECUENCIA ) {
                        secuencia = RAM_BOLETA_ANTERIOR;
                        SET_MEMORY_LONG( __ram_boleta_anterior, 0 );
                     }

                }
                if( ok && RAM_BOLETA_ANTERIOR >= 0 ) {
                    SET_MEMORY_CHAR( __ram_cambio, 1 );
                    //SET_MEMORY_CHAR( __ram_pedir_precio, 0 );
                    SET_MEMORY_CHAR( __ram_negativo, 1 );
                    SET_MEMORY_CHAR( __ram_control_cambio, 0 );

                       if( REFERENCIAR_COMP_ORIGINAL_EN_CAM ) {
                           int tipo_comp_ant = 0;

                           tipo_comp_ant = TIPO_EVENTO( RAM_TIPO_COMP_ANTERIOR, NO );

                           error = OBTENER_EVENTOS_TEMPORALES( tipo_comp_ant, RAM_CAJA_ANTERIOR,
                               RAM_BOLETA_ANTERIOR, secuencia, RAM_FECHA_ANTERIOR, 0, 0, SI, NULL, NO );
                           if( !error ) {
                               if( VERIFICA_DATOS_IMPORTACION( SI, tipo_comp_ant, RAM_FECHA_ANTERIOR,
                                   RAM_BOLETA_ANTERIOR, RAM_CAJA_ANTERIOR, secuencia ) ) {
                                   if( IMPORTAR_ITEMS() == 0 ) {
                                       MUESTRA_COMPROBANTE( );
                                       SET_MEMORY_CHAR( __ram_control_cambio, 1 );
                                       SET_MEMORY_CHAR(__ram_en_importacion_de_ticket, 3 );
                                   }
                               }
                               else {
                                   limpiar_all = 1;
                               }
                           } else {
                               limpiar_all = 1;
                           }
                       }
                   }
               }
               break;
           case 1:
            /*--------- Cambia -/+ o anula el cambio ---------*/
			   SET_MEMORY_CHAR( __ram_negativo, 0 );
            if( !RAM_P_TICKET ) {
                SET_MEMORY_CHAR( __ram_cambio, 0 );
                SET_MEMORY_LONG( __ram_boleta_anterior, 0L );
                limpiar_all = 1;
            }
            else {
                SET_MEMORY_CHAR( __ram_cambio, 2 );
				SET_MEMORY_CHAR( __ram_negativo, 1 );
            }
            SET_MEMORY_CHAR( __ram_pedir_precio, 0 );
            
            break;
        case 2:
            /*--------- Cambia +/- en el cambio ---------*/

            /*----No se permite volver al negativo---*/
		    /*
		    * SET_MEMORY_CHAR(__ram_cambio, 1);
		    * SET_MEMORY_CHAR(__ram_pedir_precio, 1);
		    * SET_MEMORY_CHAR(__ram_negativo, 1);
		    * IMPRIME_SUBTOTAL(RAM_TOTAL);
		    */
            break;
       }
       if( limpiar_all ) {
           CRR_INICIALIZAR_VAR_TICKET_RAM_NOVOL();
           MUESTRA_COMPROBANTE();
       }

       MOSTRAR_MODO_CAMBIO();
}

#endif
/***************************************************************************/
int PEDIR_BOLETA_ANTERIOR( int operacion )
/***************************************************************************/
{
    int rta = 1, fecha = 0, iter = 0, elementos = 0, menu_ok = 1;
    char tmp[7];
	/*struct _menu items[] = {{"1) TICKET", '1', NC_ORIGEN_TICKET},
								{"2) FACTURA \"A\"", '2', NC_ORIGEN_FACT_A},
								{"3) FACTURA \"B\"", '3', NC_ORIGEN_FACT_B}};
	struct _menu items1[] = {{"1) TICKET", '1', NC_ORIGEN_TICKET},
								{"2) TICKET FACTURA", '2', NC_ORIGEN_TF}};*/

    struct _menu *items;

    struct _tipos_de_comprob tipos_com;

    struct _cabmenu *cab;
    int columnas;
    SET_MEMORY_LONG( __ram_boleta_anterior, 0L );
    SET_MEMORY_INT( __ram_sucursal_anterior, 0 );
    SET_MEMORY_INT( __ram_caja_anterior, 0 );
    SET_MEMORY_INT( __ram_fecha_anterior, 0 );
    SET_MEMORY_CHAR( __ram_tipo_comp_anterior, 0 );

	if( ( operacion == _NC_LIBRE ) && FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
		/* Para ElSalvador las notas de créditos se realizan solo para comprobantes
		originales del tipo Credito Fiscal */
		SET_MEMORY_CHAR( __ram_tipo_comp_anterior, _EVENT_FAC_A );
		menu_ok = 1;
	} else {
		if( operacion >= _NC_LIBRE){
			if ( OPENED( T_TIPOS_COMPROB, TT_ORIG )
				|| OPEN_TABLE( T_TIPOS_COMPROB, TT_ORIG, (char *)&tipos_com,
				sizeof( struct _tipos_de_comprob )) == 0 ) {
			SET_WHERE( "PAIS = %d", config.pais );
			SET_ORDER2( "CODIGO" );
			RUN_QUERY( NO );
			GO2( TOP );
			elementos = RECCOUNT2( NULL, NULL);
			items = calloc( sizeof( struct _menu ), elementos );
			while( !dbEOF() && iter < elementos ) {
				items[iter].nom = malloc( sizeof ( tipos_com.descripcion ) );
				strncpy( items[iter].nom, tipos_com.descripcion, sizeof ( tipos_com.descripcion ) );
				_snprintf( tmp, sizeof( tmp ), "%d", tipos_com.codigo );
				items[iter].tecla = ( char ) tmp[0];
				items[iter].rta = tipos_com.codigo;
				iter++;
				SKIP2( 1 );
			}
			CLOSE_TABLE( T_TIPOS_COMPROB, TT_ORIG );
			if( elementos <= 0 ) {
				menu_ok = 0;
			}
			//rta = 0;
			#if defined(INVEL_W) || defined(INVEL_L)
			columnas = 2;
			cab = ( struct _cabmenu * )calloc( columnas, sizeof( struct _cabmenu ) );
			if( menu_ok && cab != NULL ) {
				cab[0].nombre = "COD";
				cab[0].align = ALIGN_LEFT;
				cab[0].width = 8;
				cab[1].nombre = "DESCRIPCION";
				cab[1].align = ALIGN_LEFT;
				cab[1].width = 20;
					SET_MEMORY_CHAR( __ram_tipo_comp_anterior,
							MENU( ST( S_TIPO_DE_COMP_ANTERIOR ),
								/*( config.pais != MEXICO )?*/ items /*: items1*/,
								elementos /*( config.pais != MEXICO )? 3 : 2*/, cab, columnas ) );
			}
			//ESC -> -1 - TECLA CAMBIO
			if( RAM_TIPO_COMP_ANTERIOR < 0 ){
				rta = 0;
			}
			free( cab );
			for( iter = 0; iter < elementos; iter++ ){
				free( items[iter].nom );
			}
			free( items );
			#else
			SET_MEMORY_CHAR( __ram_tipo_comp_anterior,
								MENU( 0, 17, ST( S_TIPO_DE_COMP_ANTERIOR ), items, 3, 120, 52 ) );
			#endif
			} else {
				GRABAR_LOG_SISTEMA( "ERROR ABRIENDO TABLA: TIPOS COMPROB" ,LOG_ERRORES,2);
				menu_ok = 0;
				rta = 0;
			}
		}
	}
	if( !( PEDIR_CAJA_Y_SECUENCIA && DEBE_PEDIR_BOLETA_ANTERIOR ) ) {
		if( menu_ok && LASTKEY() != 27 ) {
			if( REFERENCIAR_POR_SECUENCIA ) {
				SET_MEMORY_LONG( __ram_boleta_anterior,
						PEDIR_ENTERO_EN_RECUADRO( "SECUENCIA ANTERIOR", SI, 8 ) );
			} else {
				SET_MEMORY_LONG( __ram_boleta_anterior,
						PEDIR_ENTERO_EN_RECUADRO( ST( S_NRO_COMPROBANTE_ANTERIOR ), SI, 8 ) );
			}
			if( LASTKEY() != 27 ) {
				if( config.pais != MEXICO ) {
					SET_MEMORY_INT( __ram_sucursal_anterior,
							( int )PEDIR_ENTERO_EN_RECUADRO( ST( S_SUCURSAL_ANTERIOR ), SI, 3 ) );
				} else {
					SET_MEMORY_INT( __ram_sucursal_anterior, config.sucursal );
				}
				if( LASTKEY() != 27 ) {
					SET_MEMORY_INT( __ram_caja_anterior,
									( int )PEDIR_ENTERO_EN_RECUADRO( ST( S_CAJA_ANTERIOR ), SI,
																	 3 ) );
					if( LASTKEY() != 27 ) {
						fecha = PEDIR_FECHA_EN_RECUADRO( ST( S_FECHA_ANTERIOR ), SI, NULL );
						if( fecha > 0 ) {
							SET_MEMORY_INT( __ram_fecha_anterior, fecha );
						}
						if( LASTKEY() != 27 && fecha > 0 ) {
							rta = 1;
						}
					}
				}
			}
		}
	}

    return ( rta );

}

/***************************************************************************/
int TIPO_EVENTO( INT16 codigo, INT16 tipo_comprobante )
/***************************************************************************/
{
    int resultado = -1;
   struct _tipos_de_comprob tipos_com;

   if( OPENED( T_TIPOS_COMPROB, TT_ORIG )
        || OPEN_TABLE( T_TIPOS_COMPROB, TT_ORIG, (char *)&tipos_com, sizeof( struct _tipos_de_comprob )) == 0 ) {
        SELECT_TABLE( T_TIPOS_COMPROB, TT_ORIG );
		if( codigo ) {
			SET_WHERE( "CODIGO = %d AND PAIS = %d", codigo, config.pais );
		} else {
			SET_WHERE( "TIPO_COMPROBANTE = %d AND PAIS = %d", tipo_comprobante, config.pais );
		}
        if( RUN_QUERY( NO ) == 0 ) {
            resultado = tipos_com.tipo_evento_comp;
        }
        CLOSE_TABLE( T_TIPOS_COMPROB, TT_ORIG );
    }
    return resultado;
}

/***************************************************************************/
int VALIDACION_DEL_COMPROBANTE( int tipo_comprobante,int tipo_evento_comp,int bloque )
/***************************************************************************/
{
    int resultado = -1;
   struct _tipos_de_comprob tipos_com;

   tipos_com.tipo_evento_comp = -1;

   if( OPENED( T_TIPOS_COMPROB, TT_ORIG )
        || OPEN_TABLE( T_TIPOS_COMPROB, TT_ORIG, (char *)&tipos_com, sizeof( struct _tipos_de_comprob )) == 0 ) {
        SELECT_TABLE( T_TIPOS_COMPROB, TT_ORIG );
		SET_WHERE( "TIPO_COMPROBANTE = %d AND TIPO_EVENTO_COMP = %d AND PAIS = %d", tipo_comprobante, tipo_evento_comp, config.pais );
        RUN_QUERY( NO ) ;
		if( FOUND2( ) ) {
			switch(bloque){
			case  /*_CABECERA	*/1:
				resultado = tipos_com.validacion_cab;
				break;
			case  /*_CUERPO	*/ 2:
				resultado = tipos_com.validacion_cpo;
				break;
			case /* _PIE		*/ 3:
				resultado = tipos_com.validacion_pie;
				break;

			default:
				resultado = 0;
			break;
			}
	        }
        CLOSE_TABLE( T_TIPOS_COMPROB, TT_ORIG );
    }
    return resultado;
}

/***************************************************************************/
int IMPORTAR_ITEMS()
/***************************************************************************/
{
    int encontro, error = 0, skip = NO;
    struct _ev_cont ctrl_nc,ev_cont;
    struct _eventos eventos;
    struct _ev_medios ev_pago;
    struct _ev_despro ev_despro;
    struct _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
    struct _ev_bpromo ev_b_promo;

	if( OPEN_TABLE( T_CTRL_NC, TT_ORIG, ( char* )&ctrl_nc, sizeof( event_e_ticket ) ) == 0 ) {
        SELECT_TABLE( T_CTRL_NC, TT_ORIG );
        #ifdef INVEL_L
        #else
		ZAP2();
        #endif
        if( ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago, &ev_despro, &ev_dpago,
                                      &ev_cod_barra, &ev_b_promo ) ) {
			SELECT_TABLE( T_EVC_TMP, TT_ORIG );
			SET_WHERE("");
			RUN_QUERY(NO);
            while( !dbEOF() ) {
                //se saltean los articulos de descarga
                if( ev_cont.origen == _ORIGEN_PROMOCION && ( RAM_NOTA_CR || MODO_DEVOLUCION ) ) {
                    skip = SI;
                }
                SELECT_TABLE( T_CTRL_NC, TT_ORIG );
                encontro = 0;
                GO2( TOP );
                while( !dbEOF() && !encontro && !skip ) {
                    if( ctrl_nc.cod_articulo == ev_cont.cod_articulo
						&& ( strcmp( ctrl_nc.cod_barra, ev_cont.cod_barra ) == 0 )
						&& !RAM_REFERENCIA_NC
                        && !( RAM_CAMBIO && ev_cont.cod_movimiento == _DIFERENCIA_PRECIO ) ) {
                        encontro = 1;
                    }
                    else {
                        SKIP2( 1 );
                    }
                }
                if( encontro && !skip ) {
                    ctrl_nc.cantidad += ev_cont.cantidad;
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
                    UPDATE2();
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                }
                else if( !skip ) {
                    memcpy( &ctrl_nc, &ev_cont, sizeof( struct _ev_cont ) );
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
                    INSERT2( NO );
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                }
                SELECT_TABLE( T_EVC_TMP, TT_ORIG );
                SKIP2( 1 );
            }
            CERRAR_EVENTOS_TEMPORALES();
        }
        CLOSE_TABLE( T_CTRL_NC, TT_ORIG );
    }
    else {
        MENSAJE_STRING( S_ERROR_AL_ABRIR_CONTROL_NC );
    }
    if( error ) {
        MENSAJE_STRING( S_ERROR_EN_EVENTOS_TEMPORALES );
    }
    return error;
}
/*****************************************************************************/
int TIPOS_CAMBIO()
/*****************************************************************************/
{
    int rta = 1;
    if( RAM_CAMBIO == 1 ) {
        switch( RAM_CONTROL_CAMBIO ) {
            case 1:
                if( !RAM_ANULACION ) {
                    rta = EXISTE_ARTICULO_TICKET_ORIGINAL();
                    if( !rta ) {
                        SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
                        CR1_MUESTRA_MULTIPLICACION();
                    }
                }
                break;
        }
    }
    return ( rta );
}
/*****************************************************************************/
double IMPORTE_TOTAL_ITEM( void )
/*****************************************************************************/
{
	int       i;
	double    imp = 0.0;

   imp = ITEM_PRECIO;

	for( i = 0; i < RAM_P_TICKET; i++ ) {
		if( _TICKET_COD_ESPECIAL( i ) == ITEM_COD_ESPECIAL
		   && ( strcmp( _TICKET_COD_BARRA( i ), ITEM_COD_BARRA ) == 0 )
		   && _TICKET_ORIGEN( i ) == _ORIGEN_TICKET ) {
		       /* Se le cambia el signo a los anulados */
               if( _TICKET_TIPO( i ) & _RESTA && !( _TICKET_TIPO2( i ) & _CORRECCION ) ) {
                   imp -= /*fabs(*/ _TICKET_PRECIO( i ) /*)*/;
               }
               else {
                   imp += /*fabs(*/ _TICKET_PRECIO( i ) /*)*/;
               }
		}
	}

	return ( imp );
}
/*****************************************************************************/
int INVERTIR_SIGNO_PRECIO( void )
/*****************************************************************************/
{
   SET_MEMORY_DOUBLE( __item_precio, -ITEM_PRECIO );
   SET_MEMORY_DOUBLE( __item_imp_int, -ITEM_IMP_INT );
	SET_MEMORY_DOUBLE( __item_precio_sin_iva, -ITEM_PRECIO_SIN_IVA );
	SET_MEMORY_DOUBLE( __item_recargo, 0 );
   SET_MEMORY_DOUBLE( __item_iva1_precio, -ITEM_IVA1_PRECIO );
	SET_MEMORY_DOUBLE( __item_iva1_rec, 0.0 );
	SET_MEMORY_DOUBLE( __item_iva2_precio, -ITEM_IVA2_PRECIO );
	SET_MEMORY_DOUBLE( __item_iva2_rec, 0.0 );
	SET_MEMORY_FLOAT( __item_ila_precio, -ITEM_ILA_PRECIO );
	SET_MEMORY_FLOAT( __item_ila_rec, 0.0 );
	SET_MEMORY_FLOAT( __item_iha_precio, -ITEM_IHA_PRECIO );
	SET_MEMORY_FLOAT( __item_iha_rec, 0.0 );
	/*if( ITEM_GRAVADO && EXENTO_PARA_DIPLOMATICOS(  ) && config.pais == ELSALVADOR ) {
	   SET_MEMORY_DOUBLE( __ram_cantidad, -RAM_CANTIDAD );
	} */
	return ( 1 );
}

/*****************************************************************************
int EXISTE_ARTICULO_TICKET_ORIGINAL_vieja()
/*****************************************************************************
{
    int rta = 0, excedido;
    double cantidad,cantidad_pesables,_precio;
    struct _ev_cont ctrl_nc;
    OPEN_TABLE( T_CTRL_NC, TT_ORIG, ( char* )&ctrl_nc, sizeof( event_e_ticket ) );
	excedido = 0;
    cantidad = CANTIDAD_TOTAL_ITEM();
    cantidad_pesables = 0;
    while( !dbEOF() && !rta && !excedido ) {
        if( ctrl_nc.cod_articulo == ITEM_COD_ESPECIAL ) {
            if( ITEM_PEDIR_PRECIO ) {
                if( ES_FACTURA_A( clientes.cond_iva )
                 && ( CR1_ITEM_GRAVADO() || EXENTO_COMO_GRAVADO( ITEM_GRAVADO ) ) ) {
                    _precio = ( 1 + ( iva[ITEM_COD_IVA].porc_iva1 / 100.0 ) ) * ITEM_PRECIO;
                }
                else {
                    _precio = ITEM_PRECIO;
                    if ( RAM_REFERENCIA_NC ) {
                        //_precio += ITEM_IVA1_PRECIO + ITEM_IVA2_PRECIO;
                        for( iter = 0; iter < 4; iter ++ ) {
                            _precio += _ITEM_MONTO_IMPUESTO( iter );
                        }
                    }
                }
                {
                    char mens[80];

                    sprintf( mens, "%f (%f) %f %f %f", ITEM_PRECIO, _precio, ctrl_nc.total,
                             cantidad, cantidad_pesables );
                    GRABAR_LOG_SISTEMA( mens );
                }
                if( fabs( fabs( _precio ) - ( fabs( ctrl_nc.total )/ctrl_nc.cantidad ) ) < 0.01 ) {
                    cantidad_pesables += 1;
                    if( fabs( cantidad_pesables - cantidad ) < 0.01 ) {
                        rta = 1;
                    }
                }
            }
            else {
                if( ctrl_nc.cantidad - cantidad > -0.005 ) {
                    rta = 1;
                }
                else {
                    excedido = 1;
                }
            }
        }
        if( !rta ) {
            SKIP2( 1 );
        }
    }
    if( !rta ) {
        if( !excedido && cantidad_pesables > 0.01 ) {
            excedido = 1;
        }
        if( excedido ) {
            MENSAJE_STRING( S_LA_CANTIDAD_SUPERA_EL_TIKET_ORIGINAL );
        }
        else {
            MENSAJE_STRING( S_ARTICULO_NO_VENDIDO_EN_EL_TIKET_ORIGINAL );
        }
        SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
    }
    else {
        ASIGNAR_PRECIO_TICKET_ORIGINAL( ctrl_nc );
    }
    CLOSE_TABLE( T_CTRL_NC, TT_ORIG );
    return ( rta );
}*/

/*****************************************************************************/
int EXISTE_ARTICULO_TICKET_ORIGINAL(  )
/*****************************************************************************/
{
	int       rta = 0, excedido=0, i = 0, hay_diferencia = 0;
    double    cantidad, importe, cantidad_pesables, _precio = 0.0, dif, cant_nc = 0.0;
	struct    _ev_cont ctrl_nc;
	double    ctrl_imp = 0.0, monto_impuesto = 0.0;

	if( !OPENED( T_CTRL_NC, TT_ORIG ) ) {
		OPEN_TABLE( T_CTRL_NC, TT_ORIG, ( char* )&ctrl_nc, sizeof( event_e_ticket ) );
	    SET_WHERE("");
		SET_ORDER2("POSICION");
	    RUN_QUERY(NO); 
	    excedido = 0;
	    //cantidad = CANTIDAD_TOTAL_ITEM(  );
	    cantidad_pesables = 0;
		while( !dbEOF() ) {
            TRIM( ctrl_nc.cod_barra );
            if( ctrl_nc.cod_articulo == ITEM_COD_ESPECIAL &&
					( strcmp( ctrl_nc.cod_barra, ITEM_COD_BARRA ) == 0 ) &&
					( ctrl_nc.origen == _ORIGEN_TICKET
                 || ( RAM_CAMBIO && ctrl_nc.origen == _ORIGEN_DIF_PRECIO )
				 || ( RAM_CAMBIO && ctrl_nc.origen == _ORIGEN_PROMOCION )) ) {
                if( ctrl_nc.cod_movimiento != _DIFERENCIA_PRECIO ) {
                    cant_nc += ctrl_nc.cantidad;
                } else {
                    hay_diferencia = 1;
                }
                if ( PRECALCULAR_IMPUESTOS ) {
                    for( i = 0;i < 4;i++ ) {
                        monto_impuesto += ctrl_nc.monto_impuesto[i];
                    }
                }
				ctrl_imp += ctrl_nc.importe_sin_iva + ctrl_nc.imp_int + ctrl_nc.exento + ctrl_nc.iva1 + ctrl_nc.iva2 + monto_impuesto;
	        }
            SKIP2( 1 );
        }
		SET_WHERE("");
		SET_ORDER2("POSICION");
	    RUN_QUERY(NO); 
	    while( !dbEOF() && !rta && !excedido ) {
	        TRIM( ctrl_nc.cod_barra );
		    if( ctrl_nc.cod_articulo == ITEM_COD_ESPECIAL
		        && ( strcmp( ctrl_nc.cod_barra, ITEM_COD_BARRA ) == 0 )
				&& ( ctrl_nc.origen == _ORIGEN_TICKET ) ) {
		        cantidad = CANTIDAD_TOTAL_ITEM(  );
				if( ITEM_PEDIR_PRECIO && !FUNCION_PIDE_PRECIO && !RAM_REFERENCIA_NC && !hay_diferencia ) {
					if( RAM_FACTURA ) {
                        if( ES_FACTURA_A( clientes.cond_iva ) && ( CR1_ITEM_GRAVADO(  )
                            || EXENTO_COMO_GRAVADO( ITEM_GRAVADO ) ) ) {
                            _precio = ( 1 + ( iva[ITEM_COD_IVA].porc_iva1 / 100.0 ) ) * ITEM_PRECIO;
                        }
                    } else {
					    _precio = ITEM_PRECIO;
                    }
                    dif = fabs( _precio ) - fabs( ctrl_nc.total );
                    if( dif < 0.01 ) {
					    cantidad_pesables += 1;
                        if( ( cantidad_pesables - cantidad ) < 0.01 ) {
                            rta = 1;
                        }
				    }
				} else if( ITEM_PEDIR_PRECIO && !FUNCION_PIDE_PRECIO ) {
			        importe = IMPORTE_TOTAL_ITEM(  );
                    if( hay_diferencia ) {
                        SET_MEMORY_DOUBLE( __item_precio, ctrl_imp );
                        RECALCULA_ITEM_POR_PRECIO( ITEM_PRECIO );
                    }
                    if( fabs( ctrl_imp ) - fabs( importe ) > -0.0005 ) {
                        if( cant_nc - cantidad > -0.0005 ) {
                            rta = 1;
                        }
                   } else {
                      excedido = 1;
                   }
                } else {
                    if( hay_diferencia ) {
                        SET_MEMORY_DOUBLE( __item_precio, ctrl_imp );
                        RECALCULA_ITEM_POR_PRECIO( ITEM_PRECIO );
                    }
                    if( cant_nc - cantidad > -0.0005 ) {
                        rta = 1;
                    } else {
                        excedido = 1;
                    }
			    }
		    }
            if( !rta ) {
                SKIP2( 1 );
            }
	    }
		if( !rta ) {
            if( !excedido && cantidad_pesables > 0.01 ) {
                excedido = 1;
            }
			if( excedido ) {
				//Se realiza este control para casos donde 1ro se ingresa el articulo enganche B
				//y luego se ingresa el articulo A con enganche trayendo su enganche B otra vez.
				if( !item_cod_enganche ) {
					if(!(RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL &&  ctrl_nc.tipo & _ENVASE &&  ctrl_nc.cantidad < 0))
					MENSAJE_STRING( S_LA_CANTIDAD_SUPERA_EL_TIKET_ORIGINAL );
				}
            } else {
                MENSAJE_STRING( S_ARTICULO_NO_VENDIDO_EN_EL_TIKET_ORIGINAL );
            }
            SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
		    //SET_MEMORY_INT( __ram_cant_bultos, 1 );
		    CR1_MUESTRA_MULTIPLICACION( );
	    } else if( !ITEM_PEDIR_PRECIO || !RAM_REFERENCIA_NC ) {
			if( !( RAM_CAMBIO && hay_diferencia ) ) {
					//Ver con Motor de Promociones
					//Se restringe el cambio de precio para articulos por importe ya que el motor
					//de promociones no puede identificar un mismo articulo en un tiquet con
					//precios unitarios diferentes y esto proboca errores al aplicarse una promoción
					//&& !( RAM_CAMBIO == 1 && ITEM_PEDIR_PRECIO ) ) {
                ASIGNAR_PRECIO_TICKET_ORIGINAL( ctrl_nc );
            }
        } else if( !RAM_NOTA_CR && !RAM_MODO_DEVOLUCION ){
            if( ( !( articulo.gravado == 'S' && EXENTO_PARA_DIPLOMATICOS(  )
					&& config.pais == ELSALVADOR )
					|| !FUNCION_PIDE_PRECIO ) ) {
				//Ver con Motor de Promociones
				//Se restringe el cambio de precio para articulos por importe ya que el motor
				//de promociones no puede identificar un mismo articulo en un tiquet con
				//precios unitarios diferentes y esto proboca errores al aplicarse una promoción
				//&& !( RAM_CAMBIO == 1 && ITEM_PEDIR_PRECIO ) ){
                INVERTIR_SIGNO_PRECIO(  );
            }
        }
        //CLOSE_DATABASE( AREA_AUX4 );
        CLOSE_TABLE( T_CTRL_NC, TT_ORIG );
    } else {
        MENSAJE( "ERROR AL CONTROLAR" );
    }
	return ( rta );
}


/*****************************************************************************/
double CANTIDAD_TOTAL_ITEM_vieja()
/*****************************************************************************/
{
    int i;
    double cant;
    cant = ( ITEM_PEDIR_PRECIO ) ? 1 : RAM_CANTIDAD;
    for( i = 0;i < RAM_P_TICKET;i++ ) {
        if( _TICKET_COD_ESPECIAL( i ) == ITEM_COD_ESPECIAL ) {
            if( ITEM_PEDIR_PRECIO ) {
                if( fabs( ITEM_PRECIO - _TICKET_PRECIO( i ) ) < 0.01 ) {
                    cant += ( _TICKET_TIPO( i ) & _RESTA ) ? -1 : 1;
                }
            }
            else {
                cant += ( _TICKET_TIPO( i ) & _RESTA ) ? -_TICKET_CANTIDAD( i )
                      : _TICKET_CANTIDAD( i );
            }
        }
    }
    return ( cant );
}

/*****************************************************************************/
double CANTIDAD_TOTAL_ITEM( void )
/*****************************************************************************/
{
	int       i;
	double    cant = 0.0;
	long aux_cod_espcial=0;
	long aux_cod_espcial2=0;
	char barraA[30],barraB[30];
   cant = ( ITEM_PEDIR_PRECIO && !FUNCION_PIDE_PRECIO ) ? 1 : RAM_CANTIDAD;

	for( i = 0; i < RAM_P_TICKET; i++ ) {
		
		aux_cod_espcial=ITEM_COD_ESPECIAL;
		aux_cod_espcial2=_TICKET_COD_ESPECIAL( i ) ;
		memset(barraA,0,sizeof(barraA));
		memset(barraB,0,sizeof(barraB));
		sprintf(barraA,"%s",_TICKET_COD_BARRA( i ));
		sprintf(barraB,"%s",ITEM_COD_BARRA);
		

		if( _TICKET_COD_ESPECIAL( i ) == ITEM_COD_ESPECIAL && 
			( strcmp( barraA, barraB ) == 0 )
			//( strcmp( _TICKET_COD_BARRA( i ), ITEM_COD_BARRA ) == 0 )
		   && _TICKET_ORIGEN( i ) == _ORIGEN_TICKET ) {

			if( ITEM_PEDIR_PRECIO ) {
					cant += ( ( FUNCION_PIDE_PRECIO ) ? _TICKET_CANTIDAD( i ) : 1 )
					   * ( ( _TICKET_TIPO( i ) & _RESTA ) ? -1 : 1 );
			} else {
				cant +=
					( _TICKET_TIPO( i ) & _RESTA ) ? -_TICKET_CANTIDAD( i ) :
					_TICKET_CANTIDAD( i );
		   }
		   if( RAM_CAMBIO == 1 && !RAM_ANULACION )
		      if( _TICKET_COD_MOVIMIENTO( i ) == _CAMBIO_DE_MERCADERIA_MAS )
		      // LOS ingresados nuevos (CAMBIO+) no los cuento COMO CANTIDAD
		         cant--;

		}
	}

	return ( cant );
}

/*****************************************************************************/
int ASIGNAR_PRECIO_TICKET_ORIGINAL( struct _ev_cont ctrl_nc )
/*****************************************************************************/
{
    int i = 0, h = 0, signo = 0, rta = 0;
	float div_cantidad;
	double tot_imp = 0;
   struct _ev_cont ctrl_nc_2, ctrl_nc_3 ;
   int exento_x_cant = NO, sin_iva_x_cant = NO;
   ram_diferencia_precio = 0;

	/*Para devolucion referenciada porque acumulo los items junto con sus importes*/
   if( RAM_REFERENCIA_NC && ctrl_nc.cantidad != RAM_CANTIDAD
	   && !( ( RAM_MODO_DEVOLUCION == _DEVOL_PARCIAL
					|| RAM_MODO_DEVOLUCION ==  _NC_RF_PARCIAL )
					&& IMPORTACION_CON_SELECCION_ART ) ) {
		if( fabs( fmod( ctrl_nc.cantidad, 1.00 ) ) > 0.0005
			|| ( ctrl_nc.tipo3 & _PESABLE )
			|| ( ctrl_nc.tipo2 & _PEDIR_PRECIO ) ) {
			div_cantidad = 1;
		} else {
			div_cantidad = ctrl_nc.cantidad;
		}
	} else {
		div_cantidad = 1;
	}
	for( h = 0; h < 4; h++ ) {
		tot_imp += ctrl_nc.monto_impuesto[h];
	}
    signo = ( RAM_CAMBIO )? 1 : -1 ;

   /* si al item se le aplicó una diferencia de precio en el ticket original, se debe emplear el precio al q se lo vendio */
   if( RAM_REFERENCIA_NC ) {
      if( ctrl_nc.excluir_promocion == 2 && ctrl_nc.precio_original ) {
         /* este item tuvo una diferencia de precio
          * busco el mismo item con origen dif precio
          */
         unsigned long pos_reg = -1;
         long codarticulo = 0;
         char codbarra[17];

         //pos_reg = GET_POSITION();
         pos_reg = GET_POSITION_REG();
         codarticulo = ctrl_nc.cod_articulo;
         strncpy( codbarra, ctrl_nc.cod_barra, sizeof( codbarra ) );

         if( OPENED( T_CTRL_NC, TT_ORIG ))
            CLOSE_TABLE( T_CTRL_NC, TT_ORIG );
         OPEN_TABLE( T_CTRL_NC, TT_ORIG, ( char* )&ctrl_nc_2, sizeof( struct _ev_cont ) );
         SET_ORDER2("CAJA_Z, ID_EVENTO, POSICION");
         SET_WHERE("");
         RUN_QUERY(NO);
         while( !dbEOF() && !rta ) {
			 TRIM(ctrl_nc_2.cod_barra);
            if( ( ctrl_nc_2.cod_articulo == codarticulo )
               && ( strncmp( ctrl_nc_2.cod_barra, codbarra, sizeof( codbarra ) ) == 0 )
               && ( ctrl_nc_2.origen == _ORIGEN_DIF_PRECIO ) ) {
                rta = 1;

            }
            if( !rta ) {
                SKIP2( 1 );
            }
         }
         ///* hago esto por posibles problemas de btrieve, pero es redundante */
         //CLOSE_DATABASE( AREA_AUX4 );
         //USE_DB( AREA_AUX4, _CTRL_NC_BTR, ( char* )&ctrl_nc, sizeof( event_e_ticket ), NULL, 0, 0 );
         //GET_DIRECT( pos_reg );
         if( rta )
           memcpy( &ctrl_nc_3, &ctrl_nc_2, sizeof(struct _ev_cont) );

         OPEN_TABLE( T_CTRL_NC, TT_ORIG, ( char* )&ctrl_nc, sizeof( struct _ev_cont ) );

         GET_DIRECT_REG( pos_reg, 0 );
         if( rta )
           memcpy( &ctrl_nc_2, &ctrl_nc_3, sizeof(struct _ev_cont) );
         if( rta ) {
            ram_diferencia_precio = 1;
            ctrl_nc.importe_sin_iva = ctrl_nc.importe_sin_iva - ( ctrl_nc_2.importe_sin_iva * (-1) );
            ctrl_nc.exento = ctrl_nc.exento - ( ctrl_nc_2.exento * (-1) );
            ctrl_nc.iva1 = ctrl_nc.iva1 - ( ctrl_nc_2.iva1 * (-1) );
            ctrl_nc.iva2 = ctrl_nc.iva2 - ( ctrl_nc_2.iva2 * (-1) );
         }
      }
   }

    SET_MEMORY_DOUBLE( __item_precio,
			signo * ( ctrl_nc.importe_sin_iva + ctrl_nc.imp_int + ctrl_nc.exento + ctrl_nc.iva1 + ctrl_nc.iva2
                        + tot_imp ) /*/ div_cantidad*/ );
    SET_MEMORY_DOUBLE( __item_imp_int, -ctrl_nc.imp_int /*/ div_cantidad*/ );
    if( ctrl_nc.origen == _ORIGEN_TICKET ) {
      if( ctrl_nc.importe_sin_iva !=0 ) {
			double diff = fabs( articulo.precio_sin_iva ) - fabs(ctrl_nc.importe_sin_iva) + ctrl_nc.imp_int;
			diff = fabs( diff);
			//ojo que en una NC el articulo.precio_sin_iva es negativo ?¿?¿
			if(( RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL || RAM_MODO_DEVOLUCION == _NC_RF_INTERME )
				&&( diff > 0.0004 && diff < 0.03  && ctrl_nc.origen != _ORIGEN_PROMOCION  ) ) {
				SET_MEMORY_DOUBLE( __item_precio_sin_iva, articulo.precio_sin_iva + ctrl_nc.imp_int);

			}else
         SET_MEMORY_DOUBLE( __item_precio_sin_iva, -(ctrl_nc.importe_sin_iva + ctrl_nc.imp_int)/*/ div_cantidad */);
      }
      else if( ctrl_nc.exento != 0){
         SET_MEMORY_DOUBLE( __item_precio_sin_iva, -ctrl_nc.exento/* / div_cantidad*/ );
      }
    } else {
       SET_MEMORY_DOUBLE( __item_precio_sin_iva, -(ctrl_nc.importe_sin_iva + ctrl_nc.imp_int)/*/ div_cantidad*/ );
    }
    SET_MEMORY_DOUBLE( __item_costo, ctrl_nc.costo /*/ div_cantidad */);
    SET_MEMORY_DOUBLE( __item_recargo, 0 );
    /*---Recupero la bandera de gravado ---*/
    SET_MEMORY_CHAR( __item_gravado, ( ctrl_nc.tipo & _GRAVADO ) ? 1 : 0 );

    if( !ITEM_GRAVADO && ( ctrl_nc.exento > 0.01 ) && !RAM_REFERENCIA_NC && !RAM_CAMBIO ) {
        SET_MEMORY_CHAR( __item_gravado, 1 );
    }
    if( ITEM_PEDIR_PRECIO ) {
        SET_MEMORY_DOUBLE( __ram_cantidad, ctrl_nc.cantidad / div_cantidad );
    }
    /*--- Fin ---*/
    SET_MEMORY_DOUBLE( __item_iva1_precio, -ctrl_nc.iva1 /*/ div_cantidad*/ );
    SET_MEMORY_DOUBLE( __item_iva1_rec, 0.0 );
    SET_MEMORY_DOUBLE( __item_iva2_precio, -ctrl_nc.iva2 /*/ div_cantidad*/ );
    SET_MEMORY_DOUBLE( __item_iva2_rec, 0.0 );

    for( i = 0;i < 4;i++ ) {
        _SET_MEMORY_FLOAT( __item_monto_impuesto, i, -ctrl_nc.monto_impuesto[i] /*/ div_cantidad*/ );
    }
  // }
 //  else
 //  {
	//SET_MEMORY_DOUBLE( __item_precio,
 //                      signo * ( ctrl_nc.importe_sin_iva + ctrl_nc.exento + ctrl_nc.iva1 + ctrl_nc.iva2
 //                       + tot_imp ) /*/ div_cantidad*/ );
 //   SET_MEMORY_DOUBLE( __item_imp_int, -ctrl_nc.imp_int /*/ div_cantidad*/ );
 //   if( ctrl_nc.origen == _ORIGEN_TICKET ) {
 //     if( ctrl_nc.importe_sin_iva !=0 ) {
 //        SET_MEMORY_DOUBLE( __item_precio_sin_iva, -ctrl_nc.importe_sin_iva /*/ div_cantidad*/ );
 //     }
 //     else if( ctrl_nc.exento != 0){
 //        SET_MEMORY_DOUBLE( __item_precio_sin_iva, -ctrl_nc.exento /*/ div_cantidad*/ );
 //     }
 //   } else {
 //      SET_MEMORY_DOUBLE( __item_precio_sin_iva, -ctrl_nc.importe_sin_iva /*/ div_cantidad*/);
 //   }
 //   SET_MEMORY_DOUBLE( __item_costo, ctrl_nc.costo /*/ div_cantidad*/ );
 //   SET_MEMORY_DOUBLE( __item_recargo, 0 );
 //   ///---Recupero la bandera de gravado ---
 //   SET_MEMORY_CHAR( __item_gravado, ( ctrl_nc.tipo & _GRAVADO ) ? 1 : 0 );

 //   if( !ITEM_GRAVADO && ( ctrl_nc.exento > 0.01 ) && !RAM_REFERENCIA_NC && !RAM_CAMBIO ) {
 //       SET_MEMORY_CHAR( __item_gravado, 1 );
 //   }
 //   if( ITEM_PEDIR_PRECIO ) {
 //       SET_MEMORY_DOUBLE( __ram_cantidad, ctrl_nc.cantidad /*/ div_cantidad*/ );
 //   }
 //   //--- Fin ---
 //   SET_MEMORY_DOUBLE( __item_iva1_precio, -ctrl_nc.iva1 /*/ div_cantidad*/ );
 //   SET_MEMORY_DOUBLE( __item_iva1_rec, 0.0 );
 //   SET_MEMORY_DOUBLE( __item_iva2_precio, -ctrl_nc.iva2 /*/ div_cantidad*/ );
 //   SET_MEMORY_DOUBLE( __item_iva2_rec, 0.0 );
 //
 //   for( i = 0;i < 4;i++ ) {
 //       _SET_MEMORY_FLOAT( __item_monto_impuesto, i, -ctrl_nc.monto_impuesto[i] /*/ div_cantidad*/ );
 //   }
 //  }
    return ( 1 );
}
/*****************************************************************************/
int INGRESAR_CAJA_Y_SECUENCIA( int *caja, long *secuencia, int *fecha )
/*****************************************************************************/
{
    *caja = ( int )PEDIR_ENTERO_EN_RECUADRO( BUSCA_MENSAJE_STRING( S_CAJA_ANTERIOR ), SI, 3 );
    if( LASTKEY() == 27 ) {
        return 0;
    }
    *secuencia = PEDIR_ENTERO_EN_RECUADRO( BUSCA_MENSAJE_STRING( S_NRO_SECUENCIA_ANTERIOR ), SI, 8 );
    if( LASTKEY() == 27 ) {
        return 0;
    }
    *fecha = PEDIR_FECHA_EN_RECUADRO( BUSCA_MENSAJE_STRING( S_FECHA_ANTERIOR ), SI, NULL );
    if( LASTKEY() == 27 ) {
        return 0;
    }
    return 1;
}
