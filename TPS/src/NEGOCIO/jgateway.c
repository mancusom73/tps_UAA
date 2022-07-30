//******************************************************************************
//M¢dulo : Comunicacion con JGateway
//******************************************************************************
#pragma pack(1)
#include <string.h>
#include <cmemory.h>
#include <tcp_ip.h>
#include <mensaje.h>
#include <cr.h>
#include <aarch.h>
#include <pagatodo.h>
#include "path.h"
#include <actions.h>
#include <DB.h>
#include <DefTables.h>
#include <recupero.h>
#include <cobros.h>
#include <pausa.h>
#include <b_mens.h>
#include <cr_pant.h>
#include <_cr1.h>
#include <log.h>
#include <round.h>
#include <get_fech.h>
#include <trim.h>
static char canal_afinidad_abierto = 0;
extern int posicionTransaccionKinacu;
/*****************************************************************************/
int ABRIR_CANAL_JGATEWAY( void )
/*****************************************************************************/
{
	char ip[40];
	int rta = 1, canal;
	if( !canal_afinidad_abierto ) {
		memset(ip, 0, sizeof( ip ) );
		strncpy( ip, DIRECCION_IP_JGATEWAY, sizeof( ip ) - 1 );
		canal = _CANAL_JGATEWAY;
		if( ABRIR_CANAL_TCP_IP( canal, ip, _FORMATO_MENSAJE_INVEL ) ) {
			canal_afinidad_abierto = 1;
		} else {
			rta = 0;
		}
	}
	return( rta );
}
/*****************************************************************************/
void CERRAR_CANAL_JGATEWAY( void )
/*****************************************************************************/
{
    CERRAR_CANAL_TCP_IP( _CANAL_JGATEWAY );
    canal_afinidad_abierto = 0;
}
/****************************************************************************************************************************/
ENVIAR_COMANDO_JGATEWAY( char *buffer, int l_buffer, char reintentos, char servicio )
//! Ver que buffer, siempre tenga como tamaño SIZE_BUFFER_TCP, para que no se
//  produzcan pisadas de memoria al copiar la respuesta !!!
//! Presupone que esta abierto un canal al JGateway...
/****************************************************************************************************************************/
{
   int rta = 0, err = 0, seguir = 1, intentos = 0;
   char buffer_rta[SIZE_BUFFER_TCP];
   memset( buffer_rta, 0x00, sizeof( buffer_rta ) );
   while( seguir && intentos <= reintentos  ) {
	  err = _ENVIAR_PAQUETE_TCP_IP( buffer, l_buffer, buffer_rta, SIZE_BUFFER_TCP, TIMEOUT_JGATEWAY, SI, _CANAL_JGATEWAY );
      if( err ) {
         CERRAR_CANAL_JGATEWAY(  );
         if( !ABRIR_CANAL_JGATEWAY(  ) )
            seguir = 0;
      } else
         seguir = 0;
      intentos++;
   }
   if( !err ){
      memcpy( buffer, buffer_rta, SIZE_BUFFER_TCP ); //!Verificar los tamaños de los dos buffers
      rta = 1;
   } else {
      CERRAR_CANAL_JGATEWAY(  );
   }
   buffer = NULL;
   if( !rta ) {
       MENSAJE_TECLA( "ERROR EN TRANSACCION - PRESIONE UNA TECLA" );
   }
   return( rta );
}
/*****************************************************************************/
int RESERVAR_PRODUCTO_JGATEWAY( int origen )
/*****************************************************************************/
{
    int	 rta = 1, error, seguir = 1, intentos = 0, jg_offline = 0, cod_validacion;
    int    cod, /*rea_ant, area_aux*/ tabla_ant, tipo_ant;
    char   nro_linea[16];
    struct _cobros cobros;
       
    memset( nro_linea, 0, sizeof( nro_linea ) );
    //area_ant = SELECTED(  );
	tabla_ant = SELECTED2();
	tipo_ant= SELECTED_TIPO();
    //area_aux = AREA_DISPONIBLE(  );
    //if( USE_DB( area_aux, _COBROS_SIC, ( char * ) &cobros, sizeof( struct _cobros ), NULL, 0, 0 ) == 0 ) {
	cod = articulo.cod_cobro;
	if( cod && OPEN_TABLE( T_COBROS, TT_ORIG, ( char * ) &cobros, sizeof( struct _cobros ) ) == 0 ) {
		//GET_EQUAL( ( char* )&cod );
		SET_WHERE("CODIGO='%d'", cod);
		RUN_QUERY(NO);
		if( FOUND2( ) ) {
            //Cod_especial = 0 -> Cobro Normal, Cod_especial = 1 -> Cobro Pin y si = 2 es Cobro Recarga
			if( ( cobros.cod_especial == 1 || cobros.cod_especial == 2 ) ) {
               if( MODO_DEVOLUCION || RAM_NOTA_CR ) {
                    MENSAJE( "ARTICULO NO HABILITADO PARA DEVOLUCION");
		            rta = 0;
               } else {
		            cod_validacion = cobros.cod_valida;
   		            if( cobros.cod_especial == 2 ) { //si es recarga de saldo
                        rta = PEDIR_CADENA_EN_RECUADRO( " NRO LINEA ", SI, 15, nro_linea, NULL );
   		            }
		        }
		    } else {
		        rta = 2;
		    }
		} else {
		    rta = 2;
		}
		//CLOSE_DATABASE( area_aux );
	} else {
		rta = 2;
	}
    //SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );
	if( rta == 1 ) {
        MENSAJE( "ENVIANDO RESERVA PRODUCTO" );
        SET_MEMORY_DOUBLE( __item_precio, ITEM_PRECIO * RAM_CANTIDAD );
        SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
        RECALCULA_ITEM_POR_PRECIO( ITEM_PRECIO );
        error = ENVIAR_TRANSACCION_KINACU( KINACU_RESERVA, cod_validacion, nro_linea ); 
        rta = !error;
        if( error ) {
            char aux[40];

            memset( aux, 0, sizeof( aux ) );
            strcpy( aux, "NO SE PUDO REALIZAR RESERVA"  );
            GRABAR_LOG_SISTEMA( aux , LOG_ERRORES,2);
		}
    }
    if( !rta ) {
        SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
    }
    /*------------------------- Deshabilita Panel PAGATODO -------------------------*/
    PEDIR_DATOS_COBROS_KINACU( NULL, NO, origen );
	
	if( OPENED( T_COBROS, TT_ORIG ) ) {
		CLOSE_TABLE( T_COBROS, TT_ORIG );
	}

    BORRAR_MENSAJE(  );

    return( rta );
}
/******************************************************************************/
void ELIMINAR_ARTICULOS_RESERVADOS( void )
/******************************************************************************/
{
    int	     error, rta = 0;
    //int      a_ant, a_aux;
	int tabla_ant, tipo_ant;
    char     cancelado = 0;
    //long     id_transaccion;
    struct   _lin_mens lin_mens;
    struct {
        long id_transaccion;
        char nro_linea;
    } indice_lin;
   
    if( RAM_NRO_PEDIDO_PIN ) {
        posicionTransaccionKinacu = 0;
        while( RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_KINACU,(char *)&datos_cobro_kinacu, 
                sizeof( struct _datos_cobro_kinacu ), posicionTransaccionKinacu ) == 0 ) {
            //id_transaccion = datos_cobro_kinacu.id_transaccion;
            //Si id_transaccion = 0 -> Transaccion Cancelada
            cancelado = ( datos_cobro_kinacu.id_transaccion )? 0 : 1;
            if( !cancelado ) {
                MENSAJE( "ANULANDO PRODUCTOS RESERVADOS" );
                error = ENVIAR_TRANSACCION_KINACU( KINACU_ANULACION, datos_cobro_kinacu.id_transaccion, NULL );
                rta = !error;
                if( !error ) {//Sin error en el envio de la transaccion
                //a_ant = SELECTED(  );
				tabla_ant = SELECTED2();
				tipo_ant = SELECTED_TIPO();
                //a_aux = AREA_DISPONIBLE(  );				
                //if( USE_DB( a_aux, _LIN_MENS_TMP, ( char * ) &lin_mens, sizeof( struct _lin_mens ), NULL, 0, 0 ) == 0 ) {
				if( OPEN_TABLE(T_LIN_MENS_TMP, TT_TEMP,( char* )&lin_mens, sizeof( struct _lin_mens))== 0 ) {
                    //SET_ORDER( 1 );
					SET_ORDER2("ID_TRANSACCION, NRO_LINEA");
						indice_lin.id_transaccion = datos_cobro_kinacu.id_transaccion;
		            indice_lin.nro_linea = 0;
		            //GET_EQUAL( ( char * ) &indice_lin );
					SET_WHERE("ID_TRANSACCION = '%ld' AND NRO_LINEA = '%i' ",  indice_lin.id_transaccion, indice_lin.nro_linea );
					RUN_QUERY(NO);
                    while( /*!BtrvEOF()*/ !dbEOF() && lin_mens.id_transaccion == datos_cobro_kinacu.id_transaccion/*indice_lin.id_transaccion*/ ) {
				        //_DELETE(  );
						DELETE2(  );
			        }
			        //CLOSE_DATABASE( a_aux );
					CLOSE_TABLE( T_LIN_MENS_TMP, TT_TEMP );
		        }
                //------------------------------- DELETE -------------------------------
				memset( (char*)&datos_cobro_kinacu, 0, sizeof( struct _datos_cobro_kinacu ) );
		        ACTUALIZAR_ARCHIVO_REC( ARCH_REC_COBROS_KINACU, (char*)&datos_cobro_kinacu,
		                sizeof( struct _datos_cobro_kinacu ), 0 );
                } else {
                    char aux[100];

                    memset( aux, 0, sizeof( aux ) );
                    strcpy( aux, "NO SE PUDO REALIZAR LA ANULACION DE LA RESERVA" );
                    GRABAR_LOG_SISTEMA( aux , LOG_VENTAS,2 );
			    }
                //SELECT( a_ant );
				SELECT_TABLE( tabla_ant, tipo_ant );
            }
            posicionTransaccionKinacu ++;
        }
    }
    BORRAR_MENSAJE(  );      

}
/*****************************************************************************/
int CANCELAR_PRODUCTO_JGATEWAY( char *cod_barra, double importe )
/*****************************************************************************/
{
    int	 rta = 0, error = 0;
    int  a_ant = 0, a_aux = 0;
    //long  id_transaccion;
    char     cancelado = 0;
    struct _lin_mens lin_mens;
    /*struct {
        long id_transaccion;
        char nro_linea;
    } indice_lin;*/

    if( RAM_NRO_PEDIDO_PIN ) {
        posicionTransaccionKinacu = 0;
        while( !rta && RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_KINACU,(char *)&datos_cobro_kinacu, 
                sizeof( struct _datos_cobro_kinacu ), posicionTransaccionKinacu ) == 0 ) {
            if( strcmp( cod_barra, datos_cobro_kinacu.cod_barra ) == 0  &&
                    ROUND( datos_cobro_kinacu.monto, 5, 3 ) == ROUND( importe, 5, 3 ) ) {
                //id_transaccion = datos_cobro_kinacu.id_transaccion;
				cancelado = ( datos_cobro_kinacu.id_transaccion )? 0 : 1;
				if( !cancelado ) {
                    //Articulo ha cancelar 
                    rta = 1;
                } else {
                    //Articulo ya cancelado 
                    rta = 2;
                }
                if( rta == 1 ) {
					MENSAJE( "CANCELANDO PRODUCTO" );
					SET_MEMORY_DOUBLE( __item_precio, ITEM_PRECIO * RAM_CANTIDAD );
					SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
					RECALCULA_ITEM_POR_PRECIO( ITEM_PRECIO );
					error = ENVIAR_TRANSACCION_KINACU( KINACU_ANULACION, datos_cobro_kinacu.id_transaccion, NULL );
					rta = !error;
					if( !error ) {
                    //a_ant = SELECTED(  );
					a_ant = SELECTED2();
                    //a_aux = AREA_DISPONIBLE(  );	
					a_aux = SELECTED_TIPO();
                    /*if( USE_DB( a_aux,  _LIN_MENS_TMP, ( char * ) &lin_mens,
                            sizeof( struct _lin_mens ), NULL, 0, 0 ) == 0 ) {*/
					if( OPEN_TABLE(T_LIN_MENS_TMP, TT_TEMP,( char* )&lin_mens,
							sizeof( struct _lin_mens))== 0 ) {
						//SET_ORDER( 1 );
						SET_ORDER2("ID_TRANSACCION, NRO_LINEA");
		                /*indice_lin.id_transaccion = id_transaccion;
		                indice_lin.nro_linea = 0;
		                GET_EQUAL( ( char * ) &indice_lin );*/
						SET_WHERE("ID_TRANSACCION = '%ld' AND NRO_LINEA = '%i' ",  datos_cobro_kinacu.id_transaccion, 0 );
						RUN_QUERY(NO);
						while( /*!BtrvEOF()*/ !dbEOF() && lin_mens.id_transaccion == datos_cobro_kinacu.id_transaccion/*indice_lin.id_transaccion*/ ) {
							//_DELETE(  );
							DELETE2(  );
						}
						//CLOSE_DATABASE( a_aux );
						CLOSE_TABLE( T_LIN_MENS_TMP, TT_TEMP );

				    }
                    //------------------------------- DELETE -------------------------------
					datos_cobro_kinacu.estado = 1;
		            ACTUALIZAR_ARCHIVO_REC( ARCH_REC_COBROS_KINACU, (char*)&datos_cobro_kinacu,
			                sizeof( struct _datos_cobro_kinacu ), posicionTransaccionKinacu );
					//SELECT( a_ant );
					SELECT_TABLE(a_ant, a_aux);
					} else {
                char aux[100];

                memset( aux, 0, sizeof( aux ) );
                strcpy( aux, "NO SE PUDO REALIZAR LA CANCELACION DE LA RESERVA" );
                GRABAR_LOG_SISTEMA( aux , LOG_VENTAS,2 );
		    }
        }
            }
            posicionTransaccionKinacu ++;
        }

        BORRAR_MENSAJE(  );
    }
    /*------------------------- Deshabilita Panel PAGATODO -------------------------*/
    PEDIR_DATOS_COBROS_KINACU( NULL, NO, 0 );

       
    return( rta );

}
//*****************************************************************************
int ENVIAR_TRANSACCION_KINACU( int modo, long numero, char *nro_linea )

//*****************************************************************************
{

    int    ok = 0, procesar = 1, l_buffer = 0;
    char   buffer[SIZE_BUFFER_TCP];
    char   buffer_kinacu[SIZE_BUFFER_TCP];
    char aux[40];

    memset( aux, 0, sizeof( aux ) );
    memset( &buffer,  0x00, sizeof( buffer ) );  //Inicializo el buffer
    memset( &buffer_kinacu,  0x00, sizeof( buffer_kinacu ) );  //Inicializo el buffer
    if( ABRIR_CANAL_JGATEWAY(  ) ) {
        //Preparo el paquete a comunicar.
        PREPARAR_ENVIO_KINACU( modo, buffer, &l_buffer, buffer_kinacu, numero, nro_linea );
		if( ENVIAR_COMANDO_JGATEWAY( buffer, l_buffer, REINTENTOS_JGATEWAY, 0 ) ) {
            ok = PROCESAR_RESPUESTA_KINACU( modo, buffer, buffer_kinacu, numero );
        } else {
		    ok = -2;
        }
    } else {
        strcpy( aux, "ERROR AL CONECTAR CON JGATEWAY" );
        MENSAJE( aux );
        GRABAR_LOG_SISTEMA( aux , LOG_VENTAS,2 );
		ok = -1; //Abortar operacion por falta de comunicacion - no necesita reverso
	}

    return ok;
}
//*****************************************************************************
int PREPARAR_ENVIO_KINACU( int comando, char *buffer, int *l_buffer, char *buffer_kinacu,
                            long valor, char *nro_linea )
//*****************************************************************************
{
    int ok = 1;
    int	 rta = 1;

	switch( comando ) {
        case KINACU_RESERVA:
		    {
                struct _kinacu_reserva_req kinacu_reserva_aux;

                memset( &kinacu_reserva_aux, 0x00, sizeof( struct _kinacu_reserva_req ) );
                kinacu_reserva_aux.cab.producto   = KINACU;
                kinacu_reserva_aux.cab.servicio   = KINACU_RESERVA;
                kinacu_reserva_aux.cab.version    = V_KINACU_RESERVA_PRODUCTO_REQ;
                kinacu_reserva_aux.gral.fecha     = GET_FECHA_DOS(  );
                kinacu_reserva_aux.gral.sucursal  = config.sucursal;
                kinacu_reserva_aux.gral.id_evento = RAM_ID_EVENTO;
                kinacu_reserva_aux.gral.caja_z    = ( long ) ( NRO_CAJA ) * 100000L
                    + ( NRO_Z % 100000L );
                strncpy( kinacu_reserva_aux.cod_barra, articulo.cod_barra,
                    sizeof( kinacu_reserva_aux.cod_barra ) );
                kinacu_reserva_aux.monto          = ITEM_PRECIO;
                strncpy( kinacu_reserva_aux.customer, nro_linea, strlen( nro_linea ) );
                kinacu_reserva_aux.commit_sale    = 0;

				*l_buffer = sizeof( struct _kinacu_reserva_req );
				memcpy( buffer, &kinacu_reserva_aux, *l_buffer );
                memcpy(  buffer_kinacu, &kinacu_reserva_aux, *l_buffer );
			}
			break;
       case KINACU_CONFIRMACION:
            {
                struct   _kinacu_confirma_req kinacu_confirma_aux;

                memset( &kinacu_confirma_aux, 0x00, sizeof( struct _kinacu_confirma_req ) );
                kinacu_confirma_aux.cab.producto = KINACU;
                kinacu_confirma_aux.cab.servicio = KINACU_CONFIRMACION;
                kinacu_confirma_aux.cab.version  = V_KINACU_CONFIRMA_PRODUCTO_REQ;
                kinacu_confirma_aux.gral.fecha     = GET_FECHA_DOS(  );
                kinacu_confirma_aux.gral.sucursal  = config.sucursal;
                kinacu_confirma_aux.gral.id_evento = RAM_ID_EVENTO;
                kinacu_confirma_aux.gral.caja_z    = ( long ) ( NRO_CAJA ) * 100000L
                    + ( NRO_Z % 100000L );
                kinacu_confirma_aux.id_transaccion = valor;

                *l_buffer = sizeof( struct _kinacu_confirma_req );
			    memcpy( buffer, &kinacu_confirma_aux, *l_buffer );
                memcpy( buffer_kinacu, &kinacu_confirma_aux, *l_buffer );

            }
            break;
        case KINACU_ANULACION:
            {
            struct   _kinacu_confirma_req kinacu_confirma_aux;

            memset( &kinacu_confirma_aux, 0x00, sizeof( struct _kinacu_confirma_req ) );
            kinacu_confirma_aux.cab.producto = KINACU;
            kinacu_confirma_aux.cab.servicio = KINACU_ANULACION;
            kinacu_confirma_aux.cab.version = V_KINACU_ANULA_PRODUCTO_REQ;
            kinacu_confirma_aux.gral.fecha = GET_FECHA_DOS(  );
            kinacu_confirma_aux.gral.sucursal = config.sucursal;
            kinacu_confirma_aux.gral.id_evento = RAM_ID_EVENTO;
            kinacu_confirma_aux.id_transaccion = valor;
            kinacu_confirma_aux.gral.caja_z = ( long ) ( NRO_CAJA ) * 100000L +
                ( NRO_Z % 100000L );

            *l_buffer = sizeof( struct _kinacu_confirma_req );
			memcpy( buffer, &kinacu_confirma_aux, *l_buffer );
            memcpy( buffer_kinacu, &kinacu_confirma_aux, *l_buffer );
            }
            break;
		default:
			ok = 0;
			break;
   }

   return( ok );
}

//***************************************************************************************************
int PROCESAR_RESPUESTA_KINACU( int comando, char *buffer, char *buffer_kinacu,
                              int valor )
//***************************************************************************************************
{
    int    rta = 0;
	char   mensaje[73];

    switch( comando ) {
        case KINACU_RESERVA:
            {
                struct _kinacu_reserva_rta kinacu_reserva_rta;
                struct _kinacu_reserva_req kinacu_reserva;
                    
                memcpy( &kinacu_reserva_rta, buffer, sizeof( struct _kinacu_reserva_rta ) );
                memcpy( &kinacu_reserva, buffer_kinacu, sizeof( struct _kinacu_reserva_req ) );
                if( kinacu_reserva_rta.rta == 0 ) {
                   GUARDAR_DATOS_PROD_RESERVADOS( kinacu_reserva_rta.id_transaccion ,
                        kinacu_reserva.customer, kinacu_reserva.monto, valor );
                    SET_MEMORY_CHAR( __item_hab_desc_cliente, SI );
                } else {
                    TRIM( kinacu_reserva_rta.mensaje );
                    if( strlen( kinacu_reserva_rta.mensaje ) > 0 ) {
                        strncpy( mensaje, kinacu_reserva_rta.mensaje, sizeof( mensaje ) );
                        GRABAR_LOG_SISTEMA( kinacu_reserva_rta.mensaje , LOG_VENTAS,2 );
                    }
                    rta = 1;
                }
		    }
            break;
        case KINACU_CONFIRMACION:
            {
                int   i /*a_ant, a_aux*/;
                struct   _kinacu_confirma_rta rta_confirmacion;
                struct _lin_mens lin_mens;

                memcpy( &rta_confirmacion, buffer, sizeof( struct _kinacu_confirma_rta ) );
                
                if( rta_confirmacion.rta == 0 ) {
                    //a_ant = SELECTED(  );
					int tabla_ant = SELECTED2();
					int tipo_ant = SELECTED_TIPO();
                    //a_aux = AREA_DISPONIBLE(  );
                    /*if( USE_DB( a_aux, _LIN_MENS_TMP, ( char * ) &lin_mens,
                            sizeof( struct _lin_mens ), NULL, 0, 0 ) == 0 ) {*/
					if( OPEN_TABLE(T_LIN_MENS_TMP, TT_TEMP,( char* )&lin_mens,
							sizeof( struct _lin_mens))== 0 ) {
                        char  cantidad_total_lineas = 0;
                        char  nro_cuerpo = 0;

                        cantidad_total_lineas = rta_confirmacion.lineas_voucher_encabezado +
                            rta_confirmacion.lineas_voucher_cuerpo + rta_confirmacion.lineas_voucher_pie;
                       
                        for( i = 0; i < cantidad_total_lineas ; i++ ) {
                            if( ( nro_cuerpo == 0 &&
                                    ( i > ( rta_confirmacion.lineas_voucher_encabezado - 1 ) ) )
                                    || ( nro_cuerpo == 1 &&
                                    ( i > ( rta_confirmacion.lineas_voucher_cuerpo - 1 ) ) ) ) {
                                nro_cuerpo++;
                            }
                                lin_mens.nro_cuerpo = nro_cuerpo;
                                lin_mens.id_transaccion = datos_cobro_kinacu.id_transaccion;
                                lin_mens.nro_linea = i;
                                memcpy( lin_mens.descripcion, &buffer[sizeof( rta_confirmacion ) +
                                    ( i * sizeof( lin_mens.descripcion ) )],
                                    sizeof( lin_mens.descripcion ) );
                                //INSERT(  );
								INSERT2( NO );

                            }
                            //CLOSE_DATABASE( a_aux );
							CLOSE_TABLE( T_LIN_MENS_TMP, TT_TEMP );
                        }
                        //SELECT( a_ant );
                        SELECT_TABLE( tabla_ant, tipo_ant);
                } else {
                    if( rta_confirmacion.rta > 0 ) {
                        rta = 1;
                        /*if( rta_confirmacion.rta == 1 ) { //Error kinacu
                            rta = 2;
                        } else if( rta_confirmacion.rta == 2 ) { //Error Jgateway
                            rta = 0;
                        }*/
                        TRIM( rta_confirmacion.mensaje );
                        if( strlen( rta_confirmacion.mensaje ) > 0 ) {
                            strncpy( mensaje, rta_confirmacion.mensaje, sizeof( mensaje ) );
                            GRABAR_LOG_SISTEMA( rta_confirmacion.mensaje, LOG_VENTAS,2  ); 
                        }
                    }
                }
            }
            break;
        case KINACU_ANULACION: 
            {
                struct   _kinacu_confirma_rta rta_confirmacion;

                memcpy( &rta_confirmacion, buffer, sizeof( struct _kinacu_confirma_rta ) );
                if( rta_confirmacion.rta > 0 ) {
                    TRIM( rta_confirmacion.mensaje );
                    if( strlen( rta_confirmacion.mensaje ) > 0 ) {
                        strncpy( mensaje, rta_confirmacion.mensaje, sizeof( mensaje ) );
                        GRABAR_LOG_SISTEMA( rta_confirmacion.mensaje, LOG_VENTAS,2  );   
                    }
                    rta = 1;       
                } 
            }
            break;
    }
	if( rta && strlen( mensaje ) ) {
		MENSAJE( mensaje );
        PAUSA( 50 );
	}

	return rta;
}
