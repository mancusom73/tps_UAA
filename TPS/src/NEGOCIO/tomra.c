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
#include <carga.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <ini.h>
#include <cr_disco.h> 
#include <dbrouter.h> 
#include <eventos.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <imp_item.h> 
#include <imp_tick.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <print.h> 
#include <recup.h> 
#include <round.h> 
#include <tkt.h> 
#include <tomra.h> 
#include <valida.h> 
#include <_cr1.h> 
#include <math.h>
#include <aarch.h>
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#endif
#undef MK_FP
struct _aux_envases aux_envases;
struct _dev_envases dev_envases;
//static int cant_tipos_env_total = 4;
//static int cant_grupos_total = 4;
static struct _grupo_env
{
    int cant;
    long cod_interno;
    long tipoenv;
    int cod_grupo;
} pend_env[5], fact_env[5];
extern int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos;
extern double DECIMAL_COMPARATIVO( int decimal );
double QUEDAN_ENVASES_PARA_DEVOLVER();
/*****************************************************************************/
int ES_CODIGO_TOMRA( char *cadena )
/*****************************************************************************/
{
    int rta = 0, dig_prefijo_tomra = 0;

	dig_prefijo_tomra = strlen( PREFIJO_TOMRA );
	
    if( dig_prefijo_tomra && strlen( cadena ) == 13 && memcmp( cadena, PREFIJO_TOMRA, dig_prefijo_tomra ) == 0 ) {
        rta = 1;
    }

	return ( rta );
}
/*****************************************************************************/
long TOMAR_CODIGO_TOMRA( char *cadena )
/*****************************************************************************/
{
    long nro_ticket_tomra;
    strcpy( cadena, &cadena[strlen( PREFIJO_TOMRA )] );
    strcpy( cadena_scanner, &cadena[L_TICKET_TOMRA] );
    // Elimina digito
    cadena_scanner[strlen( cadena_scanner ) - 1] = 0;
    // verificador
    cadena[L_TICKET_TOMRA - 1] = 0;
    nro_ticket_tomra = atol( cadena );
    _snprintf(cadena, sizeof(cadena)-1, "%13li", config.cod_dev_envase );
    return ( nro_ticket_tomra );
}
/*****************************************************************************/
void IMPRIMIR_CUPONES_TOMRA()
/*****************************************************************************/
{
    if( RAM_CUPON_ENVASES ) {
        IMPRIME_CUPON_PROMOCION( config.val_dev_envase, 0, -1, 0, 0, NULL,0 );
    }
}
/****************************************************************************/
int BUSCAR_VOUCHER_TOMRA( long cod_tomra, int en_pago )
/****************************************************************************/
{
    int rta = 0;
    //int area_ant;
    //area_ant = SELECTED();
	int tabla_ant = SELECTED2();
	int tipo_ant = SELECTED_TIPO();
    //---PENDIENTE-------------------------
    //---Hay que cambiar esto por un acceso atraves de DBRouter a la tabla Dev_env.SIC
    //if( USE_DB( AREA_ENVASES, _DEV_ENV_SIC, ( char* )&dev_envases, sizeof( struct _dev_envases ), NULL, 0, 0 ) == 0 ) {
	if( OPEN_TABLE(T_DEV_ENVASE, TT_ORIG,( char* )&dev_envases, sizeof( struct _dev_envases))== 0 ) {
        //GO( TOP );
		GO2( TOP );
        while( /*!BtrvEOF()*/ !dbEOF() && dev_envases.cod_transaccion != cod_tomra ) {
            //SKIP( 1 );
			SKIP2( 1 );
        }
        if( dev_envases.cod_transaccion == cod_tomra ) {
            //1 = Pendiente
            if( dev_envases.estado == 1 ) {
                rta = 1;
                if( en_pago ) {
                    /*if( USE_DB( AREA_AUX_ENV, _AUX_ENV_SIC, ( char* )&aux_envases,
                                sizeof( struct _aux_envases ), NULL, 0, 0 ) == 0 ) {*/
					if( OPEN_TABLE(T_AUX_ENVASE, TT_ORIG,( char* )&aux_envases, sizeof( struct _aux_envases))== 0 ) {
                        if( EMPTY() ) {
                            aux_envases.cantidad_voucher = dev_envases.cantidad;
                            aux_envases.cod_interno = dev_envases.cod_interno;
                            aux_envases.cantidad_facturados = 0;
                            #ifdef INVEL_L
                            BEGIN_TRANSACTION();
                            #endif
                            //INSERT();
							INSERT2(NO);
                            #ifdef INVEL_L
                            END_TRANSACTION();
                            #endif
                            //SELECT( AREA_ENVASES );
							SELECT_TABLE( T_DEV_ENVASE, TT_ORIG );
                            //SKIP( 1 );
							SKIP2( 1 );
                        }
                        //SELECT( AREA_ENVASES );
						SELECT_TABLE( T_DEV_ENVASE, TT_ORIG );
                        while( dev_envases.cod_transaccion == cod_tomra && /*!BtrvEOF()*/ !dbEOF() ) {
                            //SELECT( AREA_AUX_ENV );
							SELECT_TABLE( T_AUX_ENVASE, TT_ORIG );
                            //GET_EQUAL( ( char* )&dev_envases.cod_interno );
							SET_WHERE("COD_INTERNO = '%ld' ", dev_envases.cod_interno );
							RUN_QUERY(NO);
                            if( /*FOUND()*/FOUND2() )       // acumula cantidades del mismo tipo de envase.
                            {
                                aux_envases.cantidad_voucher += dev_envases.cantidad;
                                #ifdef INVEL_L
                                BEGIN_TRANSACTION();
                                #endif
                                //UPDATE();
								UPDATE2();
                                #ifdef INVEL_L
                                END_TRANSACTION();
                                #endif
                            }
                            else {
                                aux_envases.cod_interno = dev_envases.cod_interno;
                                aux_envases.cantidad_voucher = dev_envases.cantidad;
                                aux_envases.cantidad_facturados = 0;
                                #ifdef INVEL_L
                                BEGIN_TRANSACTION();
                                #endif
                                //INSERT();
								INSERT2(NO);
                                #ifdef INVEL_L
                                END_TRANSACTION();
                                #endif
                            }
                            //SELECT( AREA_ENVASES );
							CLOSE_TABLE( T_DEV_ENVASE, TT_ORIG );
                            //SKIP( 1 );
							SKIP2( 1 );
                        }
                        //CLOSE_DATABASE( AREA_AUX_ENV );
						CLOSE_TABLE( T_AUX_ENVASE, TT_ORIG );
                    }
                    else {
                        MENSAJE_STRING( S_ERROR_EN_AREA_AUXILIAR_ENVASES );
                        rta = 0;
                        return ( rta );
                    }
                }
            }
            else {
                //2 = Cerrado
                if( dev_envases.estado == 2 ) {
                    MENSAJE_STRING( S_TICKET_DE_ENVASES_CERRADO );
                }
                rta = 0;
            }
        }
        else {
            MENSAJE_STRING( S_TICKET_DE_ENVASES_NO_ENCONTRADO );
            rta = 0;
        }
        //CLOSE_DATABASE( AREA_ENVASES );
		CLOSE_TABLE( T_DEV_ENVASE, TT_ORIG );
    }
    else {
        MENSAJE_STRING( S_ERROR_EN_AREA_ENVASES );
    }
    //SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );
    return ( rta );
}
/****************************************************************************/
int VOUCHER_REPETIDO( long cod_tomra )
/****************************************************************************/
{
    int rta = 0, i = 0;
    while( _RAM_NUMERO_TICKET_TOMRA( i ) ) {
        if( _RAM_NUMERO_TICKET_TOMRA( i ) == cod_tomra ) {
            rta = 1;
        }
        i++;
    }
    return ( rta );
}
/****************************************************************************/
void VACIAR_TABLA_AUXILIAR()
/****************************************************************************/
{
    /*if( USE_DB( AREA_AUX_ENV, _AUX_ENV_SIC, ( char* )&aux_envases, sizeof( struct _aux_envases ),
                NULL, 0, 0 ) == 0 ) {*/
	if( OPEN_TABLE(T_AUX_ENVASE, TT_ORIG,( char* )&aux_envases, sizeof( struct _aux_envases))== 0 ) {
        //GO( TOP );
		GO2( TOP );
        #ifdef INVEL_L
        BEGIN_TRANSACTION();
        #endif
        while( /*!BtrvEOF()*/ !dbEOF() ) {
            //_DELETE();
			DELETE2();
//            GO( TOP );
			GO2( TOP );
            //SKIP( 1 );
        }
        #ifdef INVEL_L
        END_TRANSACTION();
        #endif
        //CLOSE_DATABASE( AREA_AUX_ENV );
		CLOSE_TABLE( T_AUX_ENVASE, TT_ORIG );
    }
    else {
        MENSAJE_STRING( S_ERROR_EN_AREA_AUXILIAR_ENVASES );
    }
}
/****************************************************************************/
void BUSCAR_ENVASES_FACTURADOS()
/***************************************************************************/
{
    AGRUPAR_ENVASES();
}
/****************************************************************************
void BUSCAR_ENVASES_FACTURADOS(  )
***************************************************************************
{
   int       area_ant, cantidad = 0, h, signo = 1;
   long      codigo = 0;
   int       ind_env = 0, ind_dev = 0;
   area_ant = SELECTED(  );
   if( USE_DB( AREA_AUX_ENV, _AUX_ENV_SIC, ( char * ) &aux_envases,
         sizeof( struct _aux_envases ), NULL, 0, 0 ) == 0 ) {
GO( TOP );
while( !BtrvEOF(  ) ) {
aux_envases.cantidad_facturados = 0;
UPDATE(  );
SKIP( 1 );
}
for( h = 0; h < RAM_P_TICKET; h++ ) {
if( _TICKET_TIPO( h ) & _ENVASE || _TICKET_TIPO( h ) & _DEV_ENVASE ) {
cantidad = ( int ) _TICKET_CANTIDAD( h );
codigo = _TICKET_COD_ESPECIAL( h );
signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
if( MODO_DEVOLUCION && ( _TICKET_TIPO( h ) & _DEV_ENVASE ) )
signo *= -1;
cantidad *= signo;
SELECT( AREA_AUX_ENV );
GO( TOP );
GET_EQUAL( ( char * ) &codigo );
if( FOUND(  ) ) {
//if( _TICKET_PRECIO( h ) < 0 ) { // caso devolucion de envases
if( signo < 0 ) {
if( cantidad < 0 ) {
aux_envases.cantidad_facturados += cantidad;
} else {
aux_envases.cantidad_facturados -= cantidad;
}
} else {         // caso envase facturado
aux_envases.cantidad_facturados += cantidad;
}
UPDATE(  );
} else if( MODALIDAD_TOMRA == POR_TIPO ) {
aux_envases.cantidad_voucher = 0;
aux_envases.cod_interno = codigo;
aux_envases.cantidad_facturados = cantidad;
aux_envases.tipo = _TICKET_TIPO_ENVASE( h );
INSERT(  );
}
}
}
CLOSE_DATABASE( AREA_AUX_ENV );
} else {
MENSAJE_STRING( S_ERROR_EN_AREA_AUXILIAR_ENVASES );
}
SELECT( area_ant );
}*/
/****************************************************************************
int RECORRER_ENVASES( int devolver )
***************************************************************************
{
   int       area_ant, area_ant1, diferencia = 0, rta = 0;
   char      cad_val[60], desc[20];
   area_ant = SELECTED(  );
   if( USE_DB( AREA_AUX_ENV, _AUX_ENV_SIC, ( char * ) &aux_envases,
         sizeof( struct _aux_envases ), NULL, 0, 0 ) == 0 ) {
      SELECT( AREA_AUX_ENV );
GO( TOP );
while( !BtrvEOF(  ) ) {
diferencia = ( aux_envases.cantidad_voucher - aux_envases.cantidad_facturados );
switch ( devolver ) {  // caso devolver envases
case 0:
if( diferencia > 0 ) {  // caso imprimir cantidad sobrante en voucher
area_ant1 = SELECTED(  );
memset( desc, 0, 20 );
SELECT( GET_AREA_ARTICULOS(  ) );
SET_ORDER( 3 );
GO( TOP );
GET_EQUAL( ( char * ) &aux_envases.cod_interno );
if( FOUND(  ) ) {
strcpy( desc, articulo.nombre );
}
SELECT( area_ant1 );
sprintf( cad_val, " %9li  %-20s  %d ", aux_envases.cod_interno, desc,
diferencia );
MOSTRAR_CADENA( 1, PRN, cad_val );
LF( 1 );
}
break;
case 1:
if( aux_envases.cantidad_facturados != 0 ) {
if( diferencia <= 0 ) { // caso devolver cantidad de envases facturados
DEVOLVER_ENVASE( aux_envases.cod_interno,
aux_envases.cantidad_facturados );
} else {         // caso devolver cantidad de envases en voucher
DEVOLVER_ENVASE( aux_envases.cod_interno,
aux_envases.cantidad_voucher );
}
}
break;
case 2:
if( diferencia > 0 ) {
rta = 1;
}
break;
}
SELECT( AREA_AUX_ENV );
SKIP( 1 );
}
CLOSE_DATABASE( AREA_AUX_ENV );
} else {
MENSAJE_STRING( S_ERROR_EN_AREA_AUXILIAR_ENVASES );
}
SELECT( area_ant );
return ( rta );
}*/
/****************************************************************************/
int RECORRER_ENVASES( int devolver )
/***************************************************************************/
{
    int area_ant, tipo_ant = 0, diferencia = 0, rta = 0;
    //char      cad_val[60], desc[20];
    //area_ant = SELECTED();
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

    /*if( USE_DB( AREA_AUX_ENV, _AUX_ENV_SIC, ( char* )&aux_envases, sizeof( struct _aux_envases ),
                NULL, 0, 0 ) == 0 ) {*/
	if( OPEN_TABLE(T_AUX_ENVASE, TT_ORIG,( char* )&aux_envases, sizeof( struct _aux_envases))== 0 ) {
        //SELECT( AREA_AUX_ENV );
        //GO( TOP );
		GO2( TOP );
        while( !BtrvEOF() ) {
            diferencia = ( aux_envases.cantidad_voucher - aux_envases.cantidad_facturados );
            switch( devolver )                   // caso devolver envases
            {
                case 1:
                    /* 
                                if( aux_envases.cantidad_facturados != 0 ) {
                                   if( diferencia <= 0 ) { // caso devolver cantidad de envases facturados
                                      DEVOLVER_ENVASE( aux_envases.cod_interno,
                                         aux_envases.cantidad_facturados );
                                   }
                                }
                    */
                    if( aux_envases.cantidad_facturados > 0 ) {
                        DEVOLVER_ENVASE( aux_envases.cod_interno, aux_envases.cantidad_facturados );
                    }
                    break;
                case 2:
                    if( aux_envases.cantidad_voucher > 0 ) {
                        rta = 1;
                    }
                    break;
            }
            //SELECT( AREA_AUX_ENV );
			SELECT_TABLE( T_AUX_ENVASE, TT_ORIG );
            //SKIP( 1 );
			SKIP2( 1 );
        }
        //CLOSE_DATABASE( AREA_AUX_ENV );
		CLOSE_TABLE( T_AUX_ENVASE, TT_ORIG );
    }
    else {
        MENSAJE_STRING( S_ERROR_EN_AREA_AUXILIAR_ENVASES );
    }
    //SELECT( area_ant );
	SELECT_TABLE( area_ant, tipo_ant );

    return ( rta );
}
/****************************************************************************/
void DEVOLVER_ENVASE( long codigo, int cantidad )
/***************************************************************************/
{
    int area_ant, signo = ( cantidad > 0 ) ? 1 : -1;
    area_ant = SELECTED();
    //SELECT( AREA_E_TICKET );
	SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
    ASIGNA_CODIGO_ESPECIAL( codigo );
    if( RAM_NOTA_CR ) {
        SET_MEMORY_DOUBLE( __ram_cantidad, cantidad * signo * -1.00 );
    }
    else {
        SET_MEMORY_DOUBLE( __ram_cantidad, cantidad * signo );
    }
    //SET_MEMORY_DOUBLE( __ram_cantidad, cantidad );
    //SET_MEMORY_INT( __ram_anulacion, ( signo > 0 ) ? 1 : 0 );
    SET_MEMORY_INT( __ram_anulacion, ( signo > 0 ) ? 0 : 1 );
    //if( signo > 0 ) {
    articulo.precio_sin_iva = -articulo.precio_sin_iva;
    ASIGNA_ITEM( NO );
    //}
    SET_MEMORY_CHAR( __item_dev_envase, 1 );
    AGREGA_ITEM_AL_TICKET( RAM_P_TICKET, RAM_ANULACION, NO, NO, _ORIGEN_TICKET );
    //SET_MEMORY_INT( __ram_item_supervisado, RAM_P_TICKET - 1 );
    SET_MEMORY_INT( __ram_item_supervisado, RAM_P_TICKET );
    _SET_MEMORY( __ticket_cod_barra_original, RAM_P_TICKET, ITEM_COD_BARRA );
    ADD_MEMORY_INT( __ram_p_ticket, 1 );
    //SET_MEMORY_DOUBLE( __ram_total, ROUND( RAM_TOTAL + ( ITEM_PRECIO * RAM_CANTIDAD * signo * ( -1 ) ), 10,
    //    DECIMALES ) );
    SET_MEMORY_DOUBLE( __ram_total,
                       ROUND( RAM_TOTAL + ( ITEM_PRECIO * RAM_CANTIDAD * signo ), 10, DECIMALES ) );
    if( DEBE_IMPRIMIR_ITEM() ) {
        IMPRIME_ITEM( RAM_P_TICKET - 1 );
    }
    ACTUALIZA_ACUMULADORES_RAM( RAM_P_TICKET - 1 );
    //GRABAR_EVENT_E_TICKET( RAM_P_TICKET - 1, NO, SI, _ORIGEN_TICKET, 0 ); //tipo = 0
    //tipo = 0
    GRABAR_EVENT_E_TICKET( RAM_P_TICKET - 1, NO, SI, _ORIGEN_TICKET,
                           _TICKET_TIPO( RAM_P_TICKET - 1 ), NO );
    GRABAR_TICKET( RAM_P_TICKET - 1 );
    SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
    SELECT( area_ant );
}
/****************************************************************************/
int MARCAR_TABLA_ENVASES()
/***************************************************************************/
{
    int tabla_ant = 0, tipo_ant = 0, i = 0, rta = 0;
    char cad[60];
    
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    /*if( USE_DB( AREA_ENVASES, _DEV_ENV_SIC, ( char* )&dev_envases, sizeof( struct _dev_envases ),
                NULL, 0, 0 ) == 0 ) {*/
	if( OPEN_TABLE(T_DEV_ENVASE, TT_ORIG,( char* )&dev_envases, sizeof( struct _dev_envases))== 0 ) {
        while( _RAM_NUMERO_TICKET_TOMRA( i ) ) {
            //SELECT( AREA_ENVASES );
            //GO( TOP );
			GO2( TOP );
            while( /*!BtrvEOF()*/ !dbEOF() && dev_envases.cod_transaccion != _RAM_NUMERO_TICKET_TOMRA( i ) ) {
                //SKIP( 1 );
				SKIP2( 1 );
            }
            if( dev_envases.cod_transaccion == _RAM_NUMERO_TICKET_TOMRA( i ) ) {
                if( dev_envases.estado == 1 ) {
                    while( dev_envases.cod_transaccion == _RAM_NUMERO_TICKET_TOMRA( i )
                        && !BtrvEOF() ) {
                        dev_envases.caja = NRO_CAJA;
                        dev_envases.cajero = NRO_CAJERO;
                        dev_envases.nro_ticket = NRO_TICKET;
                        dev_envases.fecha_proc = GET_FECHA_DOS();
                        dev_envases.hora_proc = GET_HORA_DOS();
                        dev_envases.estado = 2;
                        #ifdef INVEL_L
                        BEGIN_TRANSACTION();
                        #endif
                        //UPDATE();
						UPDATE2();
                        #ifdef INVEL_L
                        END_TRANSACTION();
                        #endif
                        //SKIP( 1 );
						SKIP2( 1 );
                    }
                    rta = 1;
                }
                else {
                    _snprintf(cad, sizeof(cad)-1, ST( S_LA_ET__TOMRA_N___LI_YA_FUE_PROCESADA ),
                             _RAM_NUMERO_TICKET_TOMRA( i ) );
                    MENSAJE( cad );
                    rta = 0;
                }
            }
            else {
                _snprintf(cad, sizeof(cad)-1, ST( S_NO_SE_ENCONTRO_LA_ET__TOMRA_N___LI ),
                         _RAM_NUMERO_TICKET_TOMRA( i ) );
                MENSAJE( cad );
                rta = 0;
            }
            i++;
        }
        //CLOSE_DATABASE( AREA_ENVASES );
		CLOSE_TABLE( T_DEV_ENVASE, TT_ORIG );
    }
    else {
        MENSAJE_STRING( S_ERROR_EN_AREA_ENVASES );
    }
    //SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );

    return ( rta );
}
/****************************************************************************/
void REVERTIR_ENVASES()
/***************************************************************************/
{
    int tabla_ant, tipo_ant, diferencia = 0;
    //area_ant = SELECTED();
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

    /*if( USE_DB( AREA_AUX_ENV, _AUX_ENV_SIC, ( char* )&aux_envases, sizeof( struct _aux_envases ),
                NULL, 0, 0 ) == 0 ) {*/
	if( OPEN_TABLE(T_AUX_ENVASE, TT_ORIG,( char* )&aux_envases, sizeof( struct _aux_envases))== 0 ) {
        //SELECT( AREA_AUX_ENV );
		SELECT_TABLE( T_AUX_ENVASE, TT_ORIG );
        //GO( TOP );
		GO2( TOP );
        while( /*!BtrvEOF()*/ !dbEOF()  ) {
            diferencia = ( aux_envases.cantidad_voucher - aux_envases.cantidad_facturados );
            /*
                     if( aux_envases.cantidad_facturados != 0 && aux_envases.cantidad_voucher != 0 ) {
                        if( diferencia >= 0 ) { // caso revertir cantidad de envases facturados
                           DEVOLVER_ENVASE( aux_envases.cod_interno,
                              -aux_envases.cantidad_facturados );
                        } else {            // caso revertir cantidad de envases en voucher
                           DEVOLVER_ENVASE( aux_envases.cod_interno, -aux_envases.cantidad_voucher );
                        }
                     }
              */
            if( aux_envases.cantidad_facturados > 0 ) {
                DEVOLVER_ENVASE( aux_envases.cod_interno, -aux_envases.cantidad_facturados );
            }
            //SELECT( AREA_AUX_ENV );
			SELECT_TABLE( T_AUX_ENVASE, TT_ORIG );
            //SKIP( 1 );
			SKIP2( 1 );
        }
        //CLOSE_DATABASE( AREA_AUX_ENV );
		CLOSE_TABLE( T_AUX_ENVASE, TT_ORIG );
    }
    else {
        MENSAJE_STRING( S_ERROR_EN_AREA_AUXILIAR_ENVASES );
    }
    //SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );

}

/*****************************************************************************/
double QUEDAN_ENVASES_PARA_DEVOLVER()
/* Revisa si en el ticket estan los envases que se quieren devolver,
para no permitir devolver de mas*/
/*****************************************************************************/
{
    int h;
    double vendidos = 0, comprados = 0, importe =0,importe_min =0.00, importe_min_ant =0.00, saldo = 0;

    for( h = 0;h < RAM_P_TICKET;h++ ) {
        if( _TICKET_COD_MOVIMIENTO( h ) != _CAMBIO_DE_MERCADERIA_MENOS ) {
            importe = _TICKET_PRECIO( h ) * _TICKET_CANTIDAD( h );

            if( ( _TICKET_TIPO( h ) & _DEV_ENVASE || _TICKET_TIPO( h ) & _ENVASE ) &&
                ( fabs(_TICKET_PRECIO( h )) < fabs(importe_min) || importe_min == 0.00  ) ){
                /* veamos el menor importe de envases para controlar el 10% del mismo*/
                importe_min =_TICKET_PRECIO( h );
                importe_min_ant = importe_min;
            }

            if( _TICKET_TIPO( h ) & _RESTA ) {
                importe *= -1;
            }
            if( _TICKET_TIPO( h ) & _ENVASE ) {
                vendidos += importe;
            }
            if( _TICKET_TIPO( h ) & _DEV_ENVASE ) {
                if( _TICKET_TIPO2( h ) & _PEDIR_PRECIO )
                {    //Esto es porque en la funcion CR1_PEDIR_PRECIO cambia el
                    //signo del importe (de manera que ya viene el importe en negativo) para ITEM_DEV_ENVASE
                    comprados += importe;
                }
                else
                {
                    comprados += -importe;
                }
            }
        }
    }
    if( RAM_MODO_DEVOLUCION || RAM_NOTA_CR ) {
        saldo = vendidos - comprados;
        /* veamos el 10% del menor precio del envase
        para tener el cuenta si hay o no para devolver, esto evita que los controles sean afectados por el redondeo*/
        if( fabs( saldo ) < fabs(importe_min)*0.1) //if( fabs( saldo ) < 0.04 ) //por redondeo puede fallar la difencia por eso el limite es 0.02
            saldo = 0;
    } else {
        saldo = vendidos + comprados;
        if( fabs( saldo ) < fabs(importe_min)*0.1) //if( fabs( saldo ) < 0.04 ) //por redondeo puede fallar la difencia por eso el limite es 0.02
            saldo = 0;

    }
   
    //return ( vendidos - comprados );
    return ( saldo );
}
/*****************************************************************************/
int VALIDA_ENVASES_ENTREGADOS()
/*****************************************************************************/
{
    int rta = 1;
    double saldo;
	double comp_decimal = 0.00;

    if( VERIFICAR_IMPORTE_ENVASES && RAM_MODO_EJECUCION != INVENTARIO ) {
        saldo = QUEDAN_ENVASES_PARA_DEVOLVER();//SALDO_ENVASES();
		comp_decimal = DECIMAL_COMPARATIVO( DECIMALES );//??quito el +2 decimal comparativo un saldo vs 0.0001???
        if( RAM_MODO_DEVOLUCION || RAM_NOTA_CR ) {
            rta = ( saldo < comp_decimal ) ? 1 : 0;
        }
        else {
            rta = ( saldo > -comp_decimal ) ? 1 : 0;
        }
    }

    return ( rta );//1
}
/*****************************************************************************/
double SALDO_ENVASES()
/*****************************************************************************/
{
    int h;
    double vendidos = 0, comprados = 0, importe =0, saldo = 0;

    for( h = 0;h < RAM_P_TICKET;h++ ) {
		if( _TICKET_COD_MOVIMIENTO( h ) != _CAMBIO_DE_MERCADERIA_MENOS ) {
			importe = _TICKET_PRECIO( h ) * _TICKET_CANTIDAD( h );
			if( _TICKET_TIPO( h ) & _RESTA ) {
				importe *= -1;
			}
			if( _TICKET_TIPO( h ) & _ENVASE ) {
				vendidos += importe;
			}
			if( _TICKET_TIPO( h ) & _DEV_ENVASE ) {
				if( _TICKET_TIPO2( h ) & _PEDIR_PRECIO )
				{	//Esto es porque en la funcion CR1_PEDIR_PRECIO cambia el 
					//signo del importe (de manera que ya viene el importe en negativo) para ITEM_DEV_ENVASE
					comprados += importe;
				}
				else 
				{
					comprados += -importe;
				}
			}
		}
    }
	if( RAM_MODO_DEVOLUCION || RAM_NOTA_CR ) {
		saldo = vendidos - comprados;
		if( fabs( saldo ) < 0.04 ) //por redondeo puede fallar la difencia por eso el limite es 0.02
			saldo = 0;
	} else {
		saldo = vendidos + comprados;
		if( fabs( saldo ) < 0.04 ) //por redondeo puede fallar la difencia por eso el limite es 0.02
			saldo = 0;

	}
    //return ( vendidos - comprados );
	return ( saldo );
}
/****************************************************************************/
int CARGAR_ENVASES_POR_TIPO( char *cadena )
/****************************************************************************/
{
    int cant[4], total, tot_aux = 0, i, rta = 1, p;
    //   cadena tiene 13 digitos..
    //   1 a 2 Cabecera.
    //   3 a 7 Nmero serie que emite la TOMRA.
    //   8  Cantidad de envases totales.
    //   9 a 12   Cantidad por tipos de envases. El tipo de envases se toma de acuerdo a la posicin.
    //   13 D�ito verificador.
    total = ( int )( cadena[7] - '0' );
    for( i = 0;i < 4;i++ ) {
        p = i + 8;
        cant[i] = ( int )( cadena[p] - '0' );
        tot_aux += cant[i];
    }
    if( total != tot_aux ) {
        MENSAJE_STRING( S_TICKET_DE_ENVASES_INVALIDO );
        rta = 0;
    }
    else {
        for( i = 0;i < 4;i++ ) {
            _ADD_MEMORY_INT( __ram_cant_envases, i, cant[i] );
        }
    }
    return( rta );
}
/****************************************************************************/
int CARGAR_VOUCHER_ENVASE()
/****************************************************************************/
{
    int rta = 1, i, j;
    int tabla_ant = 0, tipo_ant = 0, cant[5], insertar = 0;
    for( i = 0;i < 4;i++ ) {
        cant[i + 1] = _RAM_CANT_ENVASES( i );
    }
    for( i = 1;i < 5;i++ ) {
        int envases = pend_env[i].cant;
        pend_env[i].cant -= cant[i];
        fact_env[i].cant = ( pend_env[i].cant == 0 ) ? envases
                         : ( ( pend_env[i].cant > 0 ) ? cant[i]
                           : ( ( envases > 0 ) ? envases : 0 ) );
        //fact_env[i].cod_interno = tenvase[i-1].cod_interno;
    }
    //agrupo por tipo
    for( i = 1;i < 5;i++ ) {
        if( pend_env[i].cant > 0 ) {
            for( j = 1;j < 5;j++ ) {
                if( j != i && ( pend_env[j].cant < 0 )
                 && ( pend_env[i].cod_grupo == pend_env[j].cod_grupo ) ) {
                    int sobrantes = ( int )fabs( pend_env[j].cant );
                    int envases = pend_env[i].cant;
                    pend_env[i].cant = ( sobrantes > envases ) ? 0 : envases - sobrantes;
                    pend_env[j].cant = ( sobrantes > envases )
                                     ? ( ( sobrantes - envases ) * ( -1 ) ) : 0;
                    //fact_env[i].cant += ( sobrantes > envases ) ? envases : sobrantes;
                    //fact_env[i].cod_interno = pend_env[i].cod_interno;
                    fact_env[j].cant += ( sobrantes > envases ) ? envases : sobrantes;
                }
            }
        }
    }
    //agrego los envases facturados y los envases a sobrantes
    //area_ant = SELECTED();
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

    /*if( USE_DB( AREA_AUX_ENV, _AUX_ENV_SIC, ( char* )&aux_envases, sizeof( struct _aux_envases ),
                NULL, 0, 0 ) == 0 ) {*/
	if( OPEN_TABLE(T_AUX_ENVASE, TT_ORIG,( char* )&aux_envases, sizeof( struct _aux_envases))== 0 ) {
		for( i = 1;i < 5;i++ ) {
            insertar = 0;
            aux_envases.cantidad_voucher = 0;
            if( pend_env[i].cant != 0 ) {
                if( MODALIDAD_TOMRA == _POR_TIPO ) {
                    //aux_envases.cod_interno = pend_env[i].cod_interno;
                    aux_envases.cod_interno = tenvase[i - 1].cod_interno;
                    aux_envases.tipo = i;
                    aux_envases.cantidad_facturados = 0;
                    aux_envases.cantidad_voucher = pend_env[i].cant * ( -1 );
                    insertar = 1;
                }
            }
            if( fact_env[i].cant != 0 ) {
                if( MODALIDAD_TOMRA == _POR_TIPO ) {
                    //aux_envases.cod_interno = fact_env[i].cod_interno;
                    aux_envases.cod_interno = tenvase[i - 1].cod_interno;
                    aux_envases.tipo = i;
                    aux_envases.cantidad_facturados = fact_env[i].cant;
                    insertar = 1;
                }
            }
            if( insertar ) {
                #ifdef INVEL_L
                BEGIN_TRANSACTION();
                #endif
                //INSERT();
				INSERT2(NO);
                #ifdef INVEL_L
                END_TRANSACTION();
                #endif
            }
        }
        //CLOSE_DATABASE( AREA_AUX_ENV );
		CLOSE_TABLE( T_AUX_ENVASE, TT_ORIG );
    }
    else {
        MENSAJE_STRING( S_ERROR_EN_AREA_AUXILIAR_ENVASES );
    }
    //SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );

    return ( rta );
}
/****************************************************************************/
void AGRUPAR_ENVASES()
/****************************************************************************/
{
    int i,h;
    int cantidad,signo;
    long codigo = 0;
    struct _grupo_env grupo_env[5],grupo_dev_env[5];
    for( i = 0;i < 5;i++ ) {
        memset( &fact_env[i], 0, sizeof( struct _grupo_env ) );
        memset( &pend_env[i], 0, sizeof( struct _grupo_env ) );
        memset( &grupo_env[i], 0, sizeof( struct _grupo_env ) );
        memset( &grupo_dev_env[i], 0, sizeof( struct _grupo_env ) );
    }
    for( h = 0;h < RAM_P_TICKET;h++ ) {
        if( _TICKET_TIPO( h ) & _ENVASE || _TICKET_TIPO( h ) & _DEV_ENVASE ) {
            cantidad = ( int )_TICKET_CANTIDAD( h );
            codigo = _TICKET_COD_ESPECIAL( h );
            signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
            if( MODO_DEVOLUCION && ( _TICKET_TIPO( h ) & _DEV_ENVASE ) ) {
                signo *= -1;
            }
            cantidad *= signo;
            for( i = 0;i < 4;i++ ) {
                if( _TICKET_TIPO_ENVASE( h ) == tenvase[i].cod_envase ) {
                    //if( signo > 0 ) {
                    grupo_env[tenvase[i].cod_envase].cant += cantidad;
                    grupo_env[tenvase[i].cod_envase].cod_interno = tenvase[i].cod_interno;
                    grupo_env[tenvase[i].cod_envase].tipoenv = _TICKET_TIPO_ENVASE( h );
                    //} else {
                    //  grupo_dev_env[tenvase[i].cod_envase].cant += cantidad;
                    // grupo_dev_env[tenvase[i].cod_envase].cod_interno = tenvase[i].cod_interno;
                    // grupo_dev_env[tenvase[i].cod_envase].tipoenv = _TICKET_TIPO_ENVASE( h );
                    //}
                }
                grupo_env[tenvase[i].cod_envase].cod_grupo = tenvase[i].cod_grupo;
                grupo_dev_env[tenvase[i].cod_envase].cod_grupo = tenvase[i].cod_grupo;
            }
        }
    }
    for( i = 1;i < 5;i++ ) {
        pend_env[i].cant = grupo_env[i].cant - grupo_dev_env[i].cant;
        pend_env[i].cod_interno = grupo_env[i].cod_interno;
        pend_env[i].tipoenv = grupo_env[i].tipoenv;
        pend_env[i].cod_grupo = grupo_env[i].cod_grupo;
    }
}

