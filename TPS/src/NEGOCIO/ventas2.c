#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <btrv.h> 
#include <b_mens.h> 
#include <cio.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <dbmem.h> 
#include <dbrouter.h> 
#include <get_fech.h> 
#include <log.h> 
#include <malloc.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <round.h> 
#include <stack.h> 
#include <tkt.h> 
#include <ventas2.h> 
#include <_cr1.h> 
#include <deftables.h>
#include <db.h>
#include <ini.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <math.h>
#include <aarch.h>
//#include "actions.h"
int longitud_cod_clasificacion = 9;
int ACUMULAR_VENTAS_POR_CLASIFICACION( float cantidad, double importe, long cod_clasificacion );
void CALCULA_FACTOR_DESPLAZAMIENTO( void );
int ACUMULAR_VENTAS_POR_MEDIOS( float cantidad, double importe, char cod_medio, int cod_submedio );
void ACUMULAR_VENTAS_XYZ( char modo );
int _ACTUALIZA_EVENTO_ANTERIOR();
int ACTUALIZA_EVENTO_ANTERIOR( int cant );
int EVENTO_A_PROCESAR( struct _eventos *ticket );
#define  CODIGO_COBRO_UNIPAGO 102
#define UTILIZA_CODIGO_COBRO

struct _vta_clas {
   BINARY status;					//0
   unsigned short fecha;		//1
   unsigned short cajero;		//3
   unsigned short caja;			//5
   long cod_clasificacion;      //7
   long cod_interno;			//11
   char cod_barra[17];			//15
   double cantidad;				//32
   double importe;				//40
   BINARY fin;			// 73
};

struct _datos_recorrer
{
    char cod_clasif1;
    char digito_fin;
    char digito_ini;
};

struct _vta_med {
   BINARY status;					//0
   unsigned short fecha;		//1
   unsigned short cajero;		//3
   unsigned short caja;			//5
   char cod_medio;				//7
   unsigned short cod_submedio; //8
   double cantidad;				//10
   double importe;				//18
   BINARY fin;			
};

struct _vta_xyz {
   BINARY status;				//0
   unsigned short fecha;    //1
   unsigned short cajero;   //3
   unsigned short caja;     //5
   char tipo_informe;		//7
   char estructura[2646];   //8
   BINARY fin;
};

static struct _vta_xyz *vta_xyz;
static struct _vta_clas vta_clas;
static struct _vta_med vta_med;
static long factor_desplazamiento_vta_clas = 0;
static long factor_despl = 0;
int _ACTUALIZA_VENTAS2( long id_evento, int reprocesar );
int RECORRER_CLASIF1_CALCULA_FACTOR( struct _clasif1 *clasif1, struct _datos_recorrer *datos );
extern int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos;
/*****************************************************************************/
void ACTUALIZA_VENTAS2( int reproceso )
/*****************************************************************************/
{
    //  char mens[80];
    int error = 0;
    long id_evento;
    if( USAR_VENTA_POR_CLASIFICACION || USAR_VENTA_POR_MEDIOS ) {
            if( USAR_VENTA_POR_CLASIFICACION ) {
                /*error = USE_DB( AREA_VTA_CLAS, _VTA_CLAS_SIC, ( char* )&vta_clas,
                                sizeof( vta_clas ), NULL, 0, 0 );*/
				error = OPEN_TABLE(T_VTA_CLAS, TT_ORIG,( char* )&vta_clas,
						sizeof( struct _vta_clas ) );

                // SET_LOCKS( 1, 1, 0 );
                if( !error ) {
                    //SET_ORDER( 1 );
					SET_ORDER2( "FECHA, CAJERO, COD_CLASIFICACION" );
                }
            }
            if( !error && USAR_VENTA_POR_MEDIOS ) {
                /*error = USE_DB( AREA_VTA_MED, _VTA_MED_SIC, ( char* )&vta_med, sizeof( vta_med ),
                                NULL, 0, 0 );*/
				error = OPEN_TABLE(T_VTA_MED, TT_ORIG,( char* )&vta_med,
						sizeof( struct _vta_med ) );

                // SET_LOCKS( 1, 1, 0 );
                if( !error ) {
                    //SET_ORDER( 1 );
					SET_ORDER2( "FECHA, CAJERO, COD_MEDIO, COD_SUBMEDIO" );
                }
            }
            //SELECT( AREA_TICKET );
            //SET_ORDER( 1 );
            id_evento = RAM_ID_EVENTO;
            //GET_EQUAL( ( char* )&id_evento );
			SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
			SET_ORDER2("ID_EVENTO");
			SET_WHERE("ID_EVENTO = '%ld'",id_evento);
			RUN_QUERY(NO);
            if( FOUND2() ) {
                if( !error ) {
                    if( !NRO_EVENTO_A_PROCESAR ) {
                        SET_MEMORY_LONG( __nro_evento_a_procesar, event_ticket.id_evento );
                    }
                    if( EVENTO_A_PROCESAR( &event_ticket ) || reproceso ) {
                        error = _ACTUALIZA_VENTAS2( event_ticket.id_evento, reproceso );
                    }
                    if( !error ) {
                        if( event_ticket.id_evento != NRO_EVENTO_A_PROCESAR ) {
                            MENSAJE_SIN_SONIDO_STRING( S_ACTUALIZANDO_VENTAS );
                            ACTUALIZA_EVENTO_ANTERIOR( 1 );
                            BORRAR_MENSAJE();
                        }
                        else {
                            SET_MEMORY_LONG( __nro_evento_a_procesar, 0L );
                        }
                    }
                }
                else if( !NRO_EVENTO_A_PROCESAR ) {
                    SET_MEMORY_LONG( __nro_evento_a_procesar, event_ticket.id_evento );
                }
            }
            else {
                SET_MEMORY_LONG( __nro_evento_a_procesar, event_ticket.id_evento );
            }
            //CLOSE_DATABASE( AREA_VTA_CLAS );
			CLOSE_TABLE( T_VTA_CLAS, TT_ORIG );
            //CLOSE_DATABASE( AREA_VTA_MED );
			CLOSE_TABLE( T_VTA_MED, TT_ORIG );
    }
}
/*****************************************************************************/
int ACTUALIZA_EVENTO_ANTERIOR( int cant )
/*****************************************************************************/
{
    int i,error;
    for( i = 0, error = 0;i < cant && !error && NRO_EVENTO_A_PROCESAR;i++ ) {
        error = _ACTUALIZA_EVENTO_ANTERIOR();
    }
    return ( error );
}
/*****************************************************************************/
int _ACTUALIZA_EVENTO_ANTERIOR()
/*****************************************************************************/
{
    int error = 0;
    long nro_evento;
    //SELECT( AREA_TICKET );
    nro_evento = NRO_EVENTO_A_PROCESAR;
    //GET_EQUAL( ( char* )&nro_evento );
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
	SET_ORDER2("ID_EVENTO");
	SET_WHERE("ID_EVENTO = '%ld'",nro_evento);
	RUN_QUERY(NO);
    if( EVENTO_A_PROCESAR( &event_ticket ) ) {
        error = _ACTUALIZA_VENTAS2( event_ticket.id_evento, NO );
    }
    //--------- Pasamos al proximo registro ------------//
    //SELECT( AREA_TICKET );
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
    if( !error ) {
        while( !EVENTO_A_PROCESAR( &event_ticket ) && !dbEOF() ) {
            SKIP2( 1 );
        }
        if( EVENTO_A_PROCESAR( &event_ticket ) ) {
            SET_MEMORY_LONG( __nro_evento_a_procesar, event_ticket.id_evento );
        }
        else {
            SET_MEMORY_LONG( __nro_evento_a_procesar, 0L );
        }
    }
    return ( error );
}
/*****************************************************************************/
int EVENTO_A_PROCESAR( struct _eventos *ticket )
/*****************************************************************************/
{
    int rta = 0;
	
	//Valores para jsincro
	//ticket->status == 0 -> No fue enviado a la base 
	//ticket->status == 1 -> Fue enviado a la base 
	//ticket->status == 2 -> En uso, es para indicarle al jsincro que no toque el evento
	//por que quiza se pise con otra operacion de la caja.
	if( ( ticket->status == 0 ) && TIPO_EVENTO_A_PROCESAR( ticket->tipo_evento ) ) {
        // new Fondo Fijo
        rta = 1;                          // AGREGO EL TEMA DEL REPROCESO
    }
    if( ticket->status == 0 && ticket->reproceso == EVENTO_REPROCESAR_NO_ENVIADO ) {
        rta = 1;
    }
    return ( rta );
}
/*****************************************************************************/
int TIPO_EVENTO_A_PROCESAR( char tipo_evento )
/*****************************************************************************/
{
    return (	tipo_evento == _EVENT_TICKET                          /*0  */|| 
				tipo_evento == _EVENT_COBRO                           /*1  */|| 
				tipo_evento == _EVENT_PAGO                            /*2  */|| 
				tipo_evento == _EVENT_FAC_A                           /*3  */|| 
				tipo_evento == _EVENT_FAC_B                           /*4  */|| 
				tipo_evento == _EVENT_NC_A                            /*5  */|| 
				tipo_evento == _EVENT_NC_B                            /*6  */|| 
				tipo_evento == _EVENT_RETIRO                          /*7  */|| 
				tipo_evento == _EVENT_INFORME						  /*8  */||
				tipo_evento == _EVENT_Y                               /*9  */|| 
				tipo_evento == _EVENT_Z                               /*10 */|| 
				tipo_evento == _EVENT_REMITO                          /*11 */|| 
				tipo_evento == _EVENT_ANULA_COMPROBANTE               /*12 */|| 
				tipo_evento == _EVENT_INVENTARIO                      /*13 */||                              
				tipo_evento == _EVENT_FACTURA_A_REMITO                /*14 */||                                 
				tipo_evento == _EVENT_FACTURA_B_REMITO                /*15 */||                                 
				tipo_evento == _EVENT_FAC_A_REMOTA                    /*16 */||                                 
				tipo_evento == _EVENT_FAC_B_REMOTA                    /*17 */||                                 
				/*---------------------------------------*/
				tipo_evento == _EVENT_NC_A_REMOTA                     /*19 */||                                 
				tipo_evento == _EVENT_NC_B_REMOTA                     /*20 */||                                 
				tipo_evento == _EVENT_DEVOLUCION                      /*21 */||                                 
				/*---------------------------------------*/
				tipo_evento == _EVENT_FONDO_FIJO                      /*25 */||                                 
				tipo_evento == _EVENT_INICIO_CAJERO                   /*26 */||                                 
				tipo_evento == _EVENT_FIN_CAJERO                      /*27 */||                                 
				tipo_evento == _EVENT_INICIO_STANDBY                  /*28 */||                                 
				tipo_evento == _EVENT_FIN_STANDBY                     /*29 */||                                 
				tipo_evento == _EVENT_TICKET_ESPERA                   /*30 */||
				/*---------------------------------------*/
				tipo_evento == _EVENT_TICKET_FACTURA_A                /*35 */||                                 
				tipo_evento == _EVENT_TICKET_FACTURA_B                /*36 */||                                 
				tipo_evento == _EVENT_REIMPRESION_COMPROBANTE         /*37 */||
				/*---------------------------------------*/
				tipo_evento == _EVENT_CAMBIO_MEDIO					  /*40 */||
				/*---------------------------------------*/
				tipo_evento == _EVENTO_TRANSFERENCIAS                 /*49 */||                                 
				tipo_evento == _EVENTO_ANULACION_DE_COBRO             /*50 */||
				/*---------------------------------------*/
				tipo_evento == _EVENT_FONDO_FIJO_AUTOMATICO           /*57 */||
				/*---------------------------------------*/
				tipo_evento == _EVENT_DATA_ENTRY					  /*59*/ ||
				tipo_evento == _EVENTO_COBRO_PAGO_FACIL        		  /*60 */||                                 
				tipo_evento == _EVENTO_ANULA_COBRO_PAGO_FACIL         /*61 */||
				/*---------------------------------------*/
				tipo_evento == _EVENTO_COBRO_ESPECIAL				  /*70*/||
				tipo_evento == _EVENTO_DEVOLUCION_COBRO_ESPECIAL	  /*71*/||
				tipo_evento == _EVENT_PAGO_ESPECIAL					  /*72*/||
				tipo_evento == _EVENT_REPROCESO_COBRO_ESPECIAL		  /*73*/||
				tipo_evento == _EVENT_VALORES_EN_SUCURSAL			  /*74*/||
				/*---------------------------------------*/		
				tipo_evento == _EVENT_ANULADO      					  /*99*/ ) ? SI : NO ;
}
/*****************************************************************************/
int _ACTUALIZA_VENTAS2( long id_evento, int reprocesar )
/*****************************************************************************/
{
    int error = 0;
    //SELECT( AREA_VTA_CLAS );
	SELECT_TABLE( T_VTA_CLAS, TT_ORIG );
    if( con_request ) {
        BEGIN_TRANSACTION_CONCURRENTE();
    }
    if( USAR_VENTA_POR_CLASIFICACION && !reprocesar ) {
        error = ACTUALIZA_VENTAS_POR_CLASIFICACION( id_evento );
    }
    if( !error && USAR_VENTA_POR_MEDIOS ) {
        error = ACTUALIZA_VENTAS_POR_MEDIOS( id_evento );
    }
    //SELECT( AREA_TICKET );
    //GET_EQUAL( ( char* )&id_evento );
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
	SET_ORDER2("ID_EVENTO");
	SET_WHERE("ID_EVENTO = '%ld'",id_evento);
	RUN_QUERY(NO);
    event_ticket.status = 1;
    if( !error ) {
        #ifdef INVEL_L
        //BEGIN_TRANSACTION();
        #endif
        error = UPDATE2();
        #ifdef INVEL_L
        //END_TRANSACTION();
        #endif
    }
    if( error ) {
        //SELECT( AREA_VTA_CLAS );
		SELECT_TABLE( T_VTA_CLAS, TT_ORIG );
        if( con_request ) {
            ABORT_TRANSACTION();
        }
    }
    else {
        //SELECT( AREA_VTA_CLAS );
		SELECT_TABLE( T_VTA_CLAS, TT_ORIG );
        if( con_request ) {
            error = END_TRANSACTION();
        }
    }
    return ( error );
}
/*  VENTAS POR CLASIFICACION  */
/*****************************************************************************/
void ABRIR_VTA_CLASIF()
/*****************************************************************************/
{
    CALCULA_FACTOR_DESPLAZAMIENTO();
}
/*****************************************************************************/
int ACTUALIZA_VENTAS_POR_CLASIFICACION( long id_evento )
/*****************************************************************************/
{
    int error = 0;
    //SELECT( AREA_E_TICKET );
    //GET_EQUAL( ( char* )&id_evento );
	SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
	SET_WHERE("ID_EVENTO = '%ld'",id_evento);
    SET_ORDER2( "ID_EVENTO");
	RUN_QUERY(NO);
    while( event_e_ticket.id_evento == id_evento && /*!BtrvEOF()*/ !dbEOF() && !error ) {
        error = _ACTUALIZA_VENTAS_POR_CLASIFICACION();
        //SELECT( AREA_E_TICKET );
		SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
        SKIP2( 1 );
    }
    return ( error );
}
/*****************************************************************************/
int _ACTUALIZA_VENTAS_POR_CLASIFICACION()
/*****************************************************************************/
{
    double importe,cantidad;
    int error;
    long cod_clasificacion;
    long aux;
    /*------------------- Calcula los importes ---------------------------*/
    cantidad = event_e_ticket.cantidad;
    cod_clasificacion = event_e_ticket.cod_clasificacion_old;
    if( factor_despl ) {
        aux = cod_clasificacion / factor_despl;
        cod_clasificacion = cod_clasificacion - aux * factor_despl;
    }
    if( factor_desplazamiento_vta_clas ) {
        cod_clasificacion = cod_clasificacion / factor_desplazamiento_vta_clas;
        cod_clasificacion = cod_clasificacion * factor_desplazamiento_vta_clas;
    }
    importe = event_e_ticket.total;
    if( event_ticket.tipo_evento == 5 || event_ticket.tipo_evento == 6
     || event_ticket.tipo_evento == 19 || event_ticket.tipo_evento == 20
     || event_ticket.tipo_evento == 21 ) {
        cantidad = -cantidad;
    }
    /*-------------------- Acumula los art¡culos vendidos --------------------*/
    error = ACUMULAR_VENTAS_POR_CLASIFICACION( ( float )cantidad, importe, cod_clasificacion );
    return ( error );
}
/*****************************************************************************/
int ACUMULAR_VENTAS_POR_CLASIFICACION( float cantidad, double importe, long cod_clasificacion )
/*****************************************************************************/
{
    int nuevo = 0;
    struct
    {
        unsigned fecha;
        unsigned cajero;
        long cod_clasificacion;
    } indice;
    int error;
    /*------------------------- Busca registro ---------------------------*/
    if( event_ticket.nro_dia_operativo ) {
        indice.fecha = event_ticket.nro_dia_operativo;
    }
    else {
        indice.fecha = event_ticket.fecha;
    }
    indice.cajero = event_ticket.cajero;
    indice.cod_clasificacion = cod_clasificacion;
    //SELECT( AREA_VTA_CLAS );
	SELECT_TABLE( T_VTA_CLAS, TT_ORIG );
    //SET_ORDER( 1 );
	SET_ORDER2( "FECHA, CAJERO, COD_CLASIFICACION" );
    //GET_EQUAL( ( char* )&indice );
	SET_WHERE("FECHA = '%i' AND CAJERO = '%i' AND COD_CLASIFICACION = '%i'", indice.fecha,indice.cajero, indice.cod_clasificacion );
	RUN_QUERY(NO);
    if( /*!FOUND()*/ !FOUND2()) {
        memset( &vta_clas, 0, sizeof( struct _vta_clas ) );
        nuevo = 1;
    }
    vta_clas.cantidad = ROUND( vta_clas.cantidad + cantidad, 10, 3 );
    vta_clas.importe = ROUND( vta_clas.importe + importe, 10, 3 );
    vta_clas.caja = NRO_CAJA;
    vta_clas.cajero = event_ticket.cajero;
    if( event_ticket.nro_dia_operativo ) {
        vta_clas.fecha = event_ticket.nro_dia_operativo;
    }
    else {
        vta_clas.fecha = event_ticket.fecha;
    }
    vta_clas.cod_clasificacion = cod_clasificacion;
    if( nuevo ) {
        #ifdef INVEL_L
        BEGIN_TRANSACTION();
        #endif
        //error = INSERT();
		error = INSERT2(NO);
        #ifdef INVEL_L
        END_TRANSACTION();
        #endif
        if( error != 0 ) {
            char auxi[100];
            _snprintf(auxi, sizeof(auxi)-1, ST( S_FALLO_AL_INSETAR__D_ ), error );
            MENSAJE_TECLA( auxi );
        }
    }
    else {
        #ifdef INVEL_L
        BEGIN_TRANSACTION();
        #endif
        //error = UPDATE();
		error = UPDATE2();
        #ifdef INVEL_L
        END_TRANSACTION();
        #endif
    }
    return ( error );
}
/***********************************************************************/
int RECORRER_CLASIF1_CALCULA_FACTOR( struct _clasif1 *clasif1, struct _datos_recorrer *datos )
/***********************************************************************/
{
    if( clasif1->cod_clasif1 == datos->cod_clasif1 ) {
        datos->digito_fin = clasif1->digito_fin;
    }
    if( clasif1->cod_clasif1 == 1 ) {
        datos->digito_ini = clasif1->digito_ini;
    }
    return 0;
}
/*****************************************************************************/
void CALCULA_FACTOR_DESPLAZAMIENTO( void )
/*****************************************************************************/
{
    struct _datos_recorrer datos;
    int j;
    //char auxi[100];
    datos.cod_clasif1 = CODIGO_DE_VENTA_POR_CLASIFICACIO;
    datos.digito_fin = 0;
    datos.digito_ini = 0;
    DBMEM_RECORRER( _CLASIF1_SIC, RECORRER_CLASIF1_CALCULA_FACTOR, ( char* )&datos );
    if( datos.digito_fin ) {
        factor_desplazamiento_vta_clas = 1;
        for( j = 1;j < longitud_cod_clasificacion;j++ ) {
            factor_desplazamiento_vta_clas *= 10;
        }
        for( j = 1;j < datos.digito_fin;j++ ) {
            factor_desplazamiento_vta_clas /= 10;
        }
    }
    if( datos.digito_ini ) {
        factor_despl = 1;
        for( j = datos.digito_ini - 1;j < longitud_cod_clasificacion;j++ ) {
            factor_despl *= 10;
        }
    }
}
/*****************************************************************************/
int ACTUALIZA_VENTAS_POR_MEDIOS( long id_evento )
/*****************************************************************************/
{
    int error = 0;
    double vuelto_efectivo = 0.0;
    //SELECT( AREA_PAGO );
    //GET_EQUAL( ( char* )&id_evento );
	SELECT_TABLE( T_PAGOS, tipo_t_pagos );
	SET_WHERE("ID_EVENTO = '%ld'",id_evento);
	SET_ORDER2("ID_EVENTO");
	RUN_QUERY(NO);
    while( event_pago.id_evento == id_evento && !dbEOF() && !error ) {
        // Si es reproceso y el evento no fue procesado no se acumula el
        // medio
        if( !( event_pago.reproceso && event_ticket.status == 0 ) ) {
            error = _ACTUALIZA_VENTAS_POR_MEDIOS();
            if( event_pago.reproceso ) {
                vuelto_efectivo -= event_pago.vuelto_efectivo;
            }
            else {
                vuelto_efectivo += event_pago.vuelto_efectivo;
            }
        }
        //SELECT( AREA_PAGO );
		SELECT_TABLE( T_PAGOS, tipo_t_pagos );
        SKIP2( 1 );
    }
    if( !error && vuelto_efectivo ) {
        // error = ACUMULAR_VENTAS_POR_MEDIOS(
        // cant_vuelto_efectivo-cant_vuelto_efec_reproc, -vuelto_efectivo, 1,
        // 0 );
        error = ACUMULAR_VENTAS_POR_MEDIOS( 0, -vuelto_efectivo, 1, 0 );
    }
    return ( error );
}
/*****************************************************************************/
int _ACTUALIZA_VENTAS_POR_MEDIOS()
/*****************************************************************************/
{
    double importe;
    int cod_medio, cod_submedio = 0, error = 0;
    float cantidad = 1.0;
    /*------------------- Calcula los importes ---------------------------*/
    importe = event_pago.importe - event_pago.vuelto_propio;
    cod_medio = event_pago.modo_pago + 1;
    if( medios[cod_medio - 1].tarjeta ) {
        cod_submedio = event_pago.tarjeta + 1;
    }
    else if( medios[cod_medio - 1].ticket_compra || medios[cod_medio - 1].mutual ) {
        cod_submedio = event_pago.numero_id;
    }
    if( event_pago.reproceso ) {
        importe = -importe;
        cantidad = -cantidad;
    }
    /*-------------------- Acumula  --------------------*/
    error = ACUMULAR_VENTAS_POR_MEDIOS( cantidad, importe, ( char )cod_medio, cod_submedio );
    return ( error );
}
/*****************************************************************************/
int ACUMULAR_VENTAS_POR_MEDIOS( float cantidad, double importe, char cod_medio, int cod_submedio )
/*****************************************************************************/
{
    int nuevo = 0, error;
    struct
    {
        unsigned fecha;
        unsigned cajero;
        char cod_medio;
        int cod_submedio;
    } indice;
    if( event_ticket.nro_dia_operativo ) {
        indice.fecha = event_ticket.nro_dia_operativo;
    }
    else {
        indice.fecha = event_ticket.fecha;
    }
    indice.cajero = event_ticket.cajero;
    indice.cod_medio = cod_medio;
    indice.cod_submedio = cod_submedio;
    //SELECT( AREA_VTA_MED );
	SELECT_TABLE( T_VTA_MED, TT_ORIG );
    //SET_ORDER( 1 );
	SET_ORDER2( "FECHA, CAJERO, COD_MEDIO, COD_SUBMEDIO" );
    //GET_EQUAL( ( char* )&indice );
	SET_WHERE("FECHA = '%i' AND CAJERO = '%i' AND COD_MEDIO = '%i' AND COD_SUBMEDIO = '%i' ",  indice.fecha, indice.cajero, indice.cod_medio, indice.cod_submedio );
	RUN_QUERY(NO);
    if( /*!FOUND()*/ !FOUND2()) {
        memset( &vta_med, 0, sizeof( struct _vta_med ) );
        nuevo = 1;
    }
    vta_med.cantidad = ROUND( vta_med.cantidad + cantidad, 10, 3 );
    vta_med.importe = ROUND( vta_med.importe + importe, 10, 3 );
    vta_med.caja = NRO_CAJA;
    vta_med.cajero = event_ticket.cajero;
    if( event_ticket.nro_dia_operativo ) {
        vta_med.fecha = event_ticket.nro_dia_operativo;
    }
    else {
        vta_med.fecha = event_ticket.fecha;
    }
    vta_med.cod_medio = cod_medio;
    vta_med.cod_submedio = cod_submedio;
    if( nuevo ) {
        #ifdef INVEL_L
        BEGIN_TRANSACTION();
        #endif
        //error = INSERT();
		error = INSERT2(NO);
        #ifdef INVEL_L
        END_TRANSACTION();
        #endif
    }
    else {
        #ifdef INVEL_L
        BEGIN_TRANSACTION();
        #endif
        //error = UPDATE();
		error = UPDATE2();
        #ifdef INVEL_L
        END_TRANSACTION();
        #endif
    }
    return ( error );
}
/*  VENTAS POR XYZ  */
/*****************************************************************************/
void ACTUALIZA_VENTAS_XYZ( char modo )
/*****************************************************************************/
{
    int _p,_h,_estado;
    int area, a_aux = 0, error = 0;
    if( !GET_ERROR_EN_RED() && USAR_VENTA_POR_XYZ ) {
        OPEN_RECUPERO( &_p, &_h );
        //      _estado = STACK_INS( sizeof( int ) );
        _estado = STACK_INS( sizeof( short ) );
        /*------------------- Calcula los importes ---------------------------*/
        //area = SELECTED();
		area = SELECTED2();
		a_aux = SELECTED_TIPO();
        vta_xyz = (struct _vta_xyz *)MALLOC( sizeof( struct _vta_xyz ) );
        if( vta_xyz ) {
            /*error = USE_DB( AREA_AUX, _VTA_XYZ_SIC, ( char* )vta_xyz, sizeof( struct _vta_xyz ),
                            NULL, 0, 0 );*/
			error = OPEN_TABLE(T_VTA_XYZ, TT_ORIG,( char* )&vta_xyz,
							sizeof( struct _vta_xyz ) );

            if( !error ) {
                SET_LOCKS( 1, 1, 0 );
                //SET_ORDER( 1 );
				SET_ORDER2("FECHA, CAJERO, CAJA, TIPO_INFORME");
                /*-------------------- Acumula los art¡culos vendidos --------------------*/
                if( _ESTADO == 0 ) {
                    ACUMULAR_VENTAS_XYZ( modo );
                    _SET_ESTADO( 1 );
                }
                //CLOSE_DATABASE( AREA_AUX );
				CLOSE_TABLE( T_VTA_XYZ, TT_ORIG );
            }
            //SELECT( area );
			SELECT_TABLE(area, a_aux);
            free( ( char* )vta_xyz );
        }
        CLOSE_RECUPERO( _p, _h );
    }
}
/*****************************************************************************/
void ACUMULAR_VENTAS_XYZ( char modo )
/*****************************************************************************/
{
    int nuevo = 0;
    struct
    {
        unsigned short fecha;
        unsigned short cajero;
        unsigned short caja;
        char tipo_informe;
    } indice;
    int est;
    //char auxi[100];
    if( modo == 'Z' ) {
        indice.cajero = (unsigned short)NRO_SUPERVISOR_ANT;
    }
    else {
        indice.cajero = (unsigned short)NRO_CAJERO_ANT;
    }
    indice.fecha = ( NRO_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS();
    indice.caja = (unsigned short)NRO_CAJA;
    indice.tipo_informe = modo;

    //SELECT( AREA_AUX );
	SELECT_TABLE( T_VTA_XYZ, TT_ORIG );
    //SET_ORDER( 1 );
	SET_ORDER2("FECHA, CAJERO, CAJA, TIPO_INFORME");
    switch( modo ) {
        case 'X':
            /*----------- X ------------*/
            est = _x;
            //GET_EQUAL( ( char* )&indice );
			SET_WHERE("FECHA = '%i' AND CAJERO = '%i' AND CAJA = '%i' AND TIPO_INFORME = '%i' ",  indice.fecha, indice.cajero, indice.caja, indice.tipo_informe );
			RUN_QUERY(NO);
            if( /*!FOUND()*/ !FOUND2()) {
                memset( vta_xyz, 0, sizeof( struct _vta_xyz ) );
                nuevo = 1;
            }
            break;
        case 'Y':
            /*----------- Y ------------*/
            memset( vta_xyz, 0, sizeof( struct _vta_xyz ) );
            est = _x;
            nuevo = 1;
            break;
        case 'Z':
            /*----------- Z ------------*/
            memset( vta_xyz, 0, sizeof( struct _vta_xyz ) );
            est = _z;
            nuevo = 1;
            break;
    }
    COPY_STRUCT_TO_BUFFER( vta_xyz->estructura, est );
    vta_xyz->caja = NRO_CAJA;
    if( modo == 'Z' ) {
        vta_xyz->cajero = NRO_SUPERVISOR_ANT;
    }
    else {
        vta_xyz->cajero = NRO_CAJERO_ANT;
    }
    vta_xyz->fecha = (unsigned short)indice.fecha;
    vta_xyz->tipo_informe = modo;
    if( nuevo ) {
        #ifdef INVEL_L
        BEGIN_TRANSACTION();
        #endif
        //INSERT();
		INSERT2(NO);
        #ifdef INVEL_L
        END_TRANSACTION();
        #endif
    }
    else {
        #ifdef INVEL_L
        BEGIN_TRANSACTION();
        #endif
        //UPDATE();
		UPDATE2();
        #ifdef INVEL_L
        END_TRANSACTION();
        #endif
    }
}
/*****************************************************************************/
void ACTUALIZA_VENTAS2_ANTERIORES( int actualizar_si_o_si )
/*****************************************************************************/
{
    //char  mens[80];
    int error = 1;
    long id_evento;
    if( USAR_VENTA_POR_CLASIFICACION || USAR_VENTA_POR_MEDIOS ) {
        while( error ) {
            if( !GET_ERROR_EN_RED() ) {
                if( USAR_VENTA_POR_CLASIFICACION ) {
                    /*error = USE_DB( AREA_VTA_CLAS, _VTA_CLAS_SIC, ( char* )&vta_clas,
                                    sizeof( vta_clas ), NULL, 0, 0 );*/
					error = OPEN_TABLE(T_VTA_CLAS, TT_ORIG,( char* )&vta_clas,
						sizeof( struct _vta_clas ) );
                    // SET_LOCKS( 1, 1, 0 );
                    if( !error ) {
                        SET_ORDER( 1 );
                    }
                }
                if( !error && USAR_VENTA_POR_MEDIOS ) {
                    /*error = USE_DB( AREA_VTA_MED, _VTA_MED_SIC, ( char* )&vta_med,
                                    sizeof( vta_med ), NULL, 0, 0 );*/
					error = OPEN_TABLE(T_VTA_MED, TT_ORIG,( char* )&vta_med,
						sizeof( struct _vta_med ) );
                    // SET_LOCKS( 1, 1, 0 );
                    if( !error ) {
                        SET_ORDER( 1 );
                    }
                }
                if( !error && NRO_EVENTO_A_PROCESAR ) {
                    //SELECT( AREA_TICKET );
                    //SET_ORDER( 1 );
                    id_evento = NRO_EVENTO_A_PROCESAR;
                    //GET_EQUAL( ( char* )&id_evento );
					SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
					SET_ORDER2("ID_EVENTO");
					SET_WHERE("ID_EVENTO = '%ld'",id_evento);
					RUN_QUERY(NO);
                    if( FOUND2() ) {
                        MENSAJE_SIN_SONIDO_STRING( S_ACTUALIZANDO_VENTAS );
                        error = ACTUALIZA_EVENTO_ANTERIOR( 9999 );
                        BORRAR_MENSAJE();
                        if( !error ) {
                            SET_MEMORY_LONG( __nro_evento_a_procesar, 0L );
                        }
                    }
                }
                //CLOSE_DATABASE( AREA_VTA_CLAS );
				CLOSE_TABLE( T_VTA_CLAS, TT_ORIG );
                //CLOSE_DATABASE( AREA_VTA_MED );
				CLOSE_TABLE( T_VTA_MED, TT_ORIG );
            }
            else {
                _VERIFICAR_REINTENTO();
                if( GET_ERROR_EN_RED() ) {
                    MENSAJE_STRING( S_SE_PERDIO_CONECC_AL_ACT_VTAS_RECONECC_FALLO );
                    GRABAR_LOG_SISTEMA_STRING( S_SE_PERDIO_CONECC_AL_ACT_VTAS_RECONECC_FALLO ,LOG_CON_SERVIDOR,5);
                    if( !actualizar_si_o_si ) {
                        break;
                    }
                }
            }
        }
        MENSAJE_SIN_SONIDO_STRING( S_TERMINO_ACTUALIZAR_VENTAS );
        GRABAR_LOG_SISTEMA( ST( S_TERMINO_ACTUALIZAR_VENTAS ),LOG_VENTAS,4 );
    }
}

