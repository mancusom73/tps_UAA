/**----------------------------------------------------------------------------------
 * Implementacion temporal para devoluciones de articulos afecatados por promocion
 * "Activacion de Medios". Particularmente para el caso de tarjetas (MSI)
 *----------------------------------------------------------------------------------*/

#include <cr.h>
#include <stdlib.h>
#include <btrv.h>
#include <tpv_dat.h>
#include <motor.h>
#include <math.h>
#include <recupero.h>
#include <path.h>
#include <DB.h>
#include <DefTables.h>
#include <import_t.h>
#include <log.h>
#include <cmemory.h>
#include <round.h>
#include <tarjetas.h>
#include <pagos.h>
#define MAX_GRUPOS 100

INT16 gruposAM[MAX_GRUPOS];
int gruposAMInsert = 0;
int cantPagosAM = 0;
struct _medios_activacion {
    char codMedio;
    char codSubmedio;
    INT16 modoPlanPago;
    char nroTarjeta[21];
    INT16 idGrupo;
    double importe;
    long reg;
    char activado;
} *pagosAM;

void initVars( int max )
{
    int iter;
    memset( gruposAM, '0', MAX_GRUPOS - 1 );
    pagosAM = calloc( max, sizeof( struct _medios_activacion ) );
    cantPagosAM = max;
    for( iter = 0; iter < max; iter++ ){
        //Para no confundir con los idGrupos=0, los cuales son posibles.
        pagosAM[iter].idGrupo = -1;
    }
}

void agregarGrupoAM( INT16 grupo ) 
{
    int iter;
    for( iter = 0; iter < gruposAMInsert; iter++ ) {
        if( gruposAM[iter] == grupo ) {
            return; //ya fue agregado
        }
    }
    gruposAM[iter] = grupo;
    if( gruposAMInsert < MAX_GRUPOS - 1 ) {
        gruposAMInsert++;
    }
    else {
        GRABAR_LOG_SISTEMA("Error: demasiados grupos ACTIVAR MEDIOS", LOG_ERRORES,2 );
    }
}

int perteneceGrupoAM( INT16 grupo ) 
{
    int iter;
    for( iter = 0; iter < gruposAMInsert; iter++ ) {
        if( gruposAM[iter] == grupo ) {
            return 1;
        }
    }
    return 0;
}

int ubicarMedioAM( medio, submedio ) {
    int pos = 0, iter;
    
    for( iter = 0; iter < cantPagosAM; iter++ ) {
        if ( medio == pagosAM[pos].codMedio && submedio == pagosAM[pos].codSubmedio ) {
            return pos;
        }
        pos ++;
    }
    return -1;
}

void agregarPagoAM( /*struct _event_b_promo ev_b_promo*/ INT16 idGrupo, double importe_art ) 
{
    //Se asume q la tabla ev_b_promo esta parada en un articulo que
    //genero' una promo MSI. Se busca un medio del grupo del articulo
    //para agregarlo como medio de devolucion.
    int pos = 0;
    while( pos < cantPagosAM ) {
        if( pagosAM[pos].idGrupo == idGrupo ) {
            //Se marca para ser incluido
            pagosAM[pos].activado = 1;
            if( pagosAM[pos].importe - importe_art > -0.01 ) {
                pagosAM[pos].importe -= importe_art;
                break;
            }
        }
        pos++;
    }
}

void cargarRecMediosAM( ) 
{
    int iter = 0;
    BORRAR_ARCHIVO_REC( ARCH_REC_DEVOL_AM );
    while( iter < cantPagosAM && -1 != pagosAM[iter].idGrupo  ) {
        if( pagosAM[iter].activado ) {
            GUARDAR_ARCHIVO_REC( ARCH_REC_DEVOL_AM, ( char* )&pagosAM[iter],
                                 sizeof( struct _medios_activacion ) );
        }
        iter++;
    }
}

/**
 * Determina los medios e importes de los mismos, que seben ser cargados
 * como pagos para una devolucion de articulos afectados por la promocion
 * "Activacion de Medios"
 */
void CARGAR_DEVOLUCION_AM( ) 
{
    int pos = 0, h, ok = 0;
    struct _eventos eventos;
    struct _ev_cont ev_cont;
    struct _ev_medios ev_pago;
    struct _ev_despro ev_despro;
    struct _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
    struct _ev_bpromo ev_b_promo;

    ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago,
                                &ev_despro, &ev_dpago,
                                &ev_cod_barra, &ev_b_promo );

    //Se carga informacion de los medios MSI de la ev_b_promo
    //SELECT( AREA_AUX10 ); //ev_b_promo
	SELECT_TABLE( T_EVBP_TMP, TT_ORIG );
    initVars( RECCOUNT2(NO, NULL) );
	SET_WHERE("");
	RUN_QUERY(NO);
    GO2( TOP );
    pos = 0;
    while( /*!BtrvEOF( )*/ !dbEOF() ) {
        if( BENEF_ACTIVACION_MEDIO == ev_b_promo.b_promo 
            && ( 'S' == ev_b_promo.aplicado || 's' == ev_b_promo.aplicado ) ) {
            //Se encontro medio que fue usado en promo MSI
            pagosAM[pos].codMedio = ev_b_promo.cod_medio;
            pagosAM[pos].codSubmedio = ev_b_promo.cod_submedio;
            pagosAM[pos].modoPlanPago = ev_b_promo.modo_plan_pago;
            strncpy( pagosAM[pos].nroTarjeta, 
                        ev_b_promo.nro_tarjeta_string, 
                        sizeof( pagosAM[pos].nroTarjeta ) );
            pagosAM[pos].idGrupo = ev_b_promo.id_grupo;
            //Se agrega el grupo a la lista de grupos de promo MSI
            agregarGrupoAM( pagosAM[pos].idGrupo );
            pos++;
        }
        SKIP2( 1 );
    }
    //Se carga informacion de los medios anteriores obtenida de ev_pagos
    pos = 0;
    //SELECT( AREA_AUX5 ); //ev_pago
    //GO( TOP );
	SELECT_TABLE( T_EVM_TMP, TT_ORIG );
	SET_WHERE("");
	RUN_QUERY(NO);
	GO2( TOP );
    while( /*!BtrvEOF( )*/ !dbEOF() ) {
        pos = ubicarMedioAM( ev_pago.modo_pago, ev_pago.cod_sub_medio );
        if( pos > -1 ) {
            //Se asume que se entra aca por cada medio&submedio a lo sumo una vez
            pagosAM[pos].importe = ev_pago.importe;
            pagosAM[pos].reg = GET_POSITION_REG();
        }
        SKIP2( 1 );
    }
    //Se determinan los pagos, segun los articulos a devolver
    //SELECT( AREA_AUX10 ); //b_promo
	SELECT_TABLE( T_EVBP_TMP, TT_ORIG );
    for( h = 0; h < RAM_P_TICKET; h++ ) {
        GO2( TOP );
        while( /*!BtrvEOF( )*/ !dbEOF() ) {
            if( _TICKET_COD_ESPECIAL( h ) == ev_b_promo.cod_arti         //ITEM??
                &&( strcmp( _TICKET_COD_BARRA( h ), ev_b_promo.cod_barra ) == 0 )
                && perteneceGrupoAM( ev_b_promo.id_grupo ) ) {
                //Tenemos un articulo q se devuelve y activo' una MSI
                    agregarPagoAM( /*ev_b_promo*/ ev_b_promo.id_grupo, 
                                   -1 * ROUND( _TICKET_PRECIO( h ), ENTEROS, DECIMALES ) );
                //Saltamos del while, ya que el articulo fue procesado
                break;
            }
            SKIP2( 1 );
        }
    }
    CERRAR_EVENTOS_TEMPORALES();
    //Se cargan los resultado en el REC que sera usado en Pagos.
    cargarRecMediosAM();
    free( pagosAM );
}

void CARGAR_MEDIOS_AM() 
{
    int cant = 0, iter;
    struct _medios_activacion pAM;
    struct _eventos eventos;
    struct _ev_cont ev_cont;
    struct _ev_medios ev_pago;
    struct _ev_despro ev_despro;
    struct _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
    struct _ev_bpromo ev_b_promo;

    ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago,
                                &ev_despro, &ev_dpago,
                                &ev_cod_barra, &ev_b_promo );
    //SELECT( AREA_AUX5 ); //ev_pago
	SELECT_TABLE( T_EVM_TMP, TT_ORIG );
	SET_WHERE("");
	RUN_QUERY(NO);
    
    cant = CANTIDAD_REGISTROS_REC( ARCH_REC_DEVOL_AM, 
                                   sizeof( struct _medios_activacion ) );
    
    for( iter = 0; iter < cant; iter++ ) {
        RECUPERAR_ARCHIVO_REC( ARCH_REC_DEVOL_AM, ( char* )&pAM, 
                            sizeof( struct _medios_activacion ), iter );
        GET_DIRECT_REG( pAM.reg, 0 );

        SET_MEMORY_CHAR( __var_tarjetas_tarjeta, ev_pago.tarjeta );
	    NUMERO_DE_COMERCIO_TARJETA(  );
        SET_MEMORY_CHAR( __var_tarjetas_cuotas, ev_pago.cuotas );
        SET_MEMORY( __var_tarjetas_tabla, ev_pago.tabla );
        SET_MEMORY_CHAR( __var_tarjetas_status, 2 );                    
        SET_MEMORY( __var_tarjetas_nro_comercio, ev_pago.nro_comercio );
        SET_NUMERO_TARJETA_PRIVADO(ev_pago.nro_tarjeta);//SET_MEMORY( __var_tarjetas_numero, ev_pago.nro_tarjeta );
        SET_FECHA_VTO_PRIVADA( DECREMENTAR_MES( ev_pago.fecha_vto ) );
        SET_MEMORY( __var_tarjetas_tabla, ev_pago.tabla );                    
        SET_MEMORY_CHAR( __var_tarjetas_cuotas, ev_pago.cuotas );        
        //pAM.importe contiene el importe del dinero que no se va a devolver,
        //es decir que si pAM.importe es mayor a cero, se estan devolviendo menos
        //articulos que los vendidos con ese medio en la transaccion original.
        PAGOS_AGREGAR_PAGO( -1, ( char )( ev_pago.modo_pago - 1 ),
                            ev_pago.importe - pAM.importe - ev_pago.vuelto_efectivo
                            - ev_pago.vuelto_medio, ev_pago.buffer1_long, ev_pago.numero_id,
                            ev_pago.tarjeta, ev_pago.numero_id, ev_pago.nro_tarjeta,
                            ev_pago.banco, ev_pago.fecha_vto, ev_pago.cantidad_cupones,
                            ev_pago.valor_cupones, ev_pago.cod_post_cheq,
                            ev_pago.cod_hora_acre, ev_pago.banco, ev_pago.cuit,
                            ev_pago.telefono, ev_pago.nro_comercio, (int)ev_pago.id_registro, (char)0,
                            (char)0, (char)0, ev_pago.importe_me, (int)0, (int)0, "", 0 );
    }
    CERRAR_EVENTOS_TEMPORALES();
    BORRAR_ARCHIVO_REC( ARCH_REC_DEVOL_AM );
}
