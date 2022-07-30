/*******************************************************************************
*   Sistema: TPV 1000 - IV
*
*   Descripcion :   Manejador de funcionalidades de Cajero Flotante
*   Autor: Davalle Sebastián
*
********************************************************************************/

#include <caj_flotante.h>
#include <path.h>
#include <deftables.h>
#include <aarch.h>
#include <DB.h>
#include <cr.h>
#include <cmemory.h>
#include <recupero.h>
#include <import_t.h>
#include <log.h>
#include <round.h>
#include <malloc.h>
#include <get_hora.h>
#include <fecha.h>
#pragma pack(1)

	struct _ev_cierre cierre;
	struct _ev_cierre cierre_ant;
	struct _eventos events;
    struct _ev_medios pagos;
	struct _ev_cont cont;
    struct _ev_despro despro;
    struct _ev_dmedios dpago;
    struct _ev_cod_barra cod_barra;
    struct _ev_bpromo ev_b_promo;
	struct __sub_medios subTmp;

	struct _v_cierres_y 
	{
		BINARY status; //1
		long cod_cajero;//5
		long caja_z ; //9
		long id_evento ; //13
		long secuencia ; //17
		long cod_sucursal ; //21
		INT16 fecha_ticket; //23
		INT16 cant_cierres_y; //25
	} *cierres_y;
	
int	nro_caja, nro_caja_ant;
extern int tipo_t_pagos, tipo_t_eventos, tipo_t_ev_cont, tipo_t_ev_cierre;
/*---------------------------------------------------------------------------------------*/
int IMPORTAR_DATOS_CAJA_ANTERIOR(int cajero)
/*---------------------------------------------------------------------------------------*/
{
	//char where[100];
    int error = 0;
	int ok = 0;
	
    nro_caja = NRO_CAJA; 
	nro_caja_ant = 0;
	flotando = 0;
	//COMENTO HASTA QUE SE ARREGLE		
    /*if (!( cierres_y = MALLOC( sizeof( struct _v_cierres_y ) ) ) ) return (0);
	
	DIRECT_DB_QUERY("delete from v_cierres_y"); // Borramos todos los registros de la tabla por si tiene basura

	_snprintf( where,sizeof(where), " WHERE COD_CAJERO = %d", cajero);
	
	cierres_y->id_evento=0; //cereo la estructura por si tiene basura

	if( UTILIZA_DBROUTER ) 
	{
        if( !OBTENER_TABLA_SERVIDOR( V_CIERRES_Y, TT_ORIG, "v_cierres_y", where, SI, NULL )) 
		{
			if( !OPENED(V_CIERRES_Y, TT_ORIG) && OPEN_TABLE(V_CIERRES_Y, TT_ORIG, (char *)cierres_y, sizeof( struct _v_cierres_y )) == 0 ) 
			{
				SELECT_TABLE( V_CIERRES_Y, TT_ORIG );
				SET_WHERE("");
				SET_ORDER2( "CANT_CIERRES_Y" );
				RUN_QUERY( NO );
				GO2( BOTTOM ); // Selecciono el último Cierre (el mayor)
				if(cierres_y->id_evento > 0)//si existe algun evento en la vista, indica que el cajero tiene dinero sin arquear
				{
					nro_caja_ant = (cierres_y->caja_z - (cierres_y->caja_z % 100000))/100000;
					cant_cierres_y = cierres_y->cant_cierres_y;
					if( cierres_y->cod_cajero == NRO_CAJERO )//si es el mismio cajero
					{
						OBTENER_TABLAS_TEMPORALES(cierres_y->secuencia, nro_caja_ant, cierres_y->fecha_ticket, cierres_y->id_evento);
						flotando = 1;
						ok=1;
					}
				}
				else
				{
					GRABAR_LOG_SISTEMA("EL CAJERO NO TIENE DINERO SIN ARQUEAR");
					flotando = 0;
					ok=0;
					SELECT( AREA_CIERRE );//Me fijo si es el primer cierre del cajero para poner la cantidad de cierres Y en cero
					if( !RECCOUNT() ) 
					{
						GRABAR_LOG_SISTEMA("PRIMER INGRESO DEL CAJERO");		
						cant_cierres_y = 0;
						ok=0;
					}
				}
			}
			else
			{
				ok=0;	
			}
		}
	if(ok)
	{
		UPDATE_VARIABLES__X_();
		
		UPDATE_SUBMEDIOS_REC();

		GUARDAR_DATOS_IMPORTADOS();
	}
	 
	FREEMEM( ( char* )cierres_y );  
	CLOSE_TABLE(V_CIERRES_Y, TT_ORIG);
	CERRAR_EVENTOS_TEMPORALES();//cierro todas las tablas para poder seguirlas usando
	return (ok);
	}*/
return(ok);

}
/*---------------------------------------------------------------------------------------*/
void ENVIAR_SUBMEDIOS_REC()
/*---------------------------------------------------------------------------------------*/
{
    struct __sub_medios subTmp;
	int cnt=0,cant=0,rta=0,error=0, /*area*/ tabla_ant, tipo_ant;
	//area =SELECTED();
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	//SELECT( AREA_TICKET );
	SELECT_TABLE( T_EVENTOS, tipo_t_ev_cont );
   
	if((cant = CANTIDAD_REGISTROS_REC(ARCH_REC_SUBMEDIOS,sizeof( struct __sub_medios ))) > 0)// Recupero lo de submedios.rec y lo pongo en una struct
	{
		while(cnt < cant)
		{
			RECUPERAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS, ( char* )&subTmp, sizeof( struct __sub_medios ), cnt );
	       
			event_pago.id_evento = event_ticket.id_evento;
			event_pago.modo_pago = subTmp.medios; 
			event_pago.cod_sub_medio = subTmp.submedios;
			event_pago.importe = subTmp.importe;
			event_pago.posicion = cnt;
			
		    
				//SELECT( AREA_PAGO );
				SELECT_TABLE( T_PAGOS, tipo_t_pagos );
				#ifdef INVEL_L
				//BEGIN_TRANSACTION();
				#endif
				rta = INSERT2( NO );
				#ifdef INVEL_L
				//END_TRANSACTION();
				#endif
				if(rta) error++;
				cnt++;
		}
	}
	//SELECT(area);
	SELECT_TABLE( tabla_ant, tipo_ant );
}

/*---------------------------------------------------------------------------------------*/
void CALCULA_TOTAL_DE_CAJERO()//Realiza una resta de lo actual menos lo importado antes de compiar las variable __x_ a las __Z_.
/*---------------------------------------------------------------------------------------*/
{
	int cnt=0,cant=0, i,/* area*/ tabla_ant, tipo_ant;

	if((cant = CANTIDAD_REGISTROS_REC(ARCH_REC_CIERRE_ANT,sizeof( struct _ev_cierre ))) > 0)
	{
	RECUPERAR_ARCHIVO_REC( ARCH_REC_CIERRE_ANT, ( char* )&cierre_ant, sizeof( struct _ev_cierre ), cnt );
	
    //area = SELECTED();
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	
	//SELECT( AREA_TICKET );        
    //GET_LAST();
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
	SET_WHERE("");
	RUN_QUERY(NO);
	GO2( BOTTOM );

				event_cierre->caja = event_ticket.caja;
                event_cierre->nro_z = event_ticket.nro_z;
                event_cierre->caja_z = event_ticket.caja_z;
                event_cierre->id_evento = event_ticket.id_evento;
                event_cierre->gran_total = 0.0;
                event_cierre->nro = NRO_Y;
				
				for(i=0; i<10; i++) 
				{
					event_cierre->imp_ventas[i] = - cierre_ant.imp_ventas[i] + _X_VENTAS_IMPORTE( i );
					event_cierre->can_ventas[i] = - cierre_ant.can_ventas[i] + _X_VENTAS_CANTIDAD( i );
					event_cierre->imp_cobros[i] = - cierre_ant.imp_cobros[i] + _X_COBROS_IMPORTE( i );
					event_cierre->can_cobros[i] = - cierre_ant.can_cobros[i] + _X_COBROS_CANTIDAD( i );
					event_cierre->imp_fondo_fijo_multiple[i] = - cierre_ant.imp_fondo_fijo_multiple[i] + _X_FONDO_FIJO_MULTIPLE_IMPORTE( i );
					event_cierre->imp_fondo_fijo_multiple_me[i] = - cierre_ant.imp_fondo_fijo_multiple_me[i] + _X_FONDO_FIJO_MULTIPLE_IMP_ME( i );
					event_cierre->can_fondo_fijo_multiple[i] = - cierre_ant.can_fondo_fijo_multiple[i] + _X_FONDO_FIJO_MULTIPLE_CANTIDAD( i );
					event_cierre->importes_iva[i] = - cierre_ant.importes_iva[i] + _X_IMPORTE_IVA( i );
					event_cierre->sobretasas_iva[i] = - cierre_ant.sobretasas_iva[i] + _X_IMPORTE_SOBRETASA_IVA( i );
					event_cierre->imp_retiros[i]  = - cierre_ant.imp_retiros[i] + _X_RETIROS_IMPORTE( i );
					event_cierre->imp_retiros_me[i] = - cierre_ant.imp_retiros_me[i] + _X_RETIROS_IMPORTE_ME( i );
					event_cierre->can_retiros[i] = - cierre_ant.can_retiros[i] + _X_RETIROS_CANTIDAD( i );
					event_cierre->imp_cambios[i] = - cierre_ant.imp_cambios[i] + _X_CAMBIOS_IMPORTE( i );
					event_cierre->can_cambios[i] = - cierre_ant.can_cambios[i] + _X_CAMBIOS_CANTIDAD( i );             
					event_cierre->devoluciones_medio[i] = - cierre_ant.devoluciones_medio[i] + _X_DEVOLUCIONES_MEDIO( i );
					event_cierre->imp_pagosesp[i] = - cierre_ant.imp_pagosesp[i] + _X_MOZOS_IMPORTE( i );
					event_cierre->can_pagosesp[i] = - cierre_ant.can_pagosesp[i] + ( short )_X_MOZOS_CANTIDAD( i );
					event_cierre->ventas_importe_sin_redondeo[i] = - cierre_ant.ventas_importe_sin_redondeo[i] + _X_VENTAS_IMPORTE_SIN_REDONDEO(i); //new
					event_cierre->retiro_fondo_importe_pago[i] = - cierre_ant.retiro_fondo_importe_pago[i] + _X_COBROS_IMPORTE_ME(i); //new
					event_cierre->cobros_importe_me[i] = - cierre_ant.cobros_importe_me[i] + _X_RETIRO_FONDO_IMPORTE_PAGOS(i); //new
				}    

				event_cierre->adelantos.imp = - cierre_ant.adelantos.imp + X_ADELANTOS_IMPORTE;
                event_cierre->adelantos.can = - cierre_ant.adelantos.can + X_ADELANTOS_CANTIDAD;
                event_cierre->intereses.imp = - cierre_ant.intereses.imp + X_INTERESES_IMPORTE;
                event_cierre->intereses.can = - cierre_ant.intereses.can + X_INTERESES_CANTIDAD;
                event_cierre->fondo_fijo.imp = - cierre_ant.fondo_fijo.imp + X_FONDO_FIJO_IMPORTE;
                event_cierre->fondo_fijo.can = - cierre_ant.fondo_fijo.can + X_FONDO_FIJO_CANTIDAD;
                event_cierre->anulaciones.imp = - cierre_ant.anulaciones.imp + X_ANULACIONES_IMPORTE;
                event_cierre->anulaciones.can = - cierre_ant.anulaciones.can + X_ANULACIONES_CANTIDAD;
                event_cierre->dev_envases.imp = - cierre_ant.dev_envases.imp + X_DEV_ENVASES_IMPORTE;
                event_cierre->dev_envases.can = - cierre_ant.dev_envases.can + X_DEV_ENVASES_CANTIDAD;
                event_cierre->ventas_gravadas = - cierre_ant.ventas_gravadas + X_VENTAS_GRAVADAS;
                event_cierre->ventas_exentas = - cierre_ant.ventas_exentas + X_VENTAS_EXENTAS;
                event_cierre->imp_int = - cierre_ant.imp_int + X_IMP_INT;
                event_cierre->tot_dinero = - cierre_ant.tot_dinero + X_TOT_DINERO;
                event_cierre->iva1 = - cierre_ant.iva1 + X_IVA1;
                event_cierre->iva2 = - cierre_ant.iva2 + X_IVA2;
                event_cierre->retiro_fondo.imp = - cierre_ant.retiro_fondo.imp + X_RETIRO_FONDO_IMPORTE;
                event_cierre->retiro_fondo.can = - cierre_ant.retiro_fondo.can + X_RETIRO_FONDO_CANTIDAD;
                event_cierre->pagos_por_caja.imp = - cierre_ant.pagos_por_caja.imp + X_PAGOS_POR_CAJA_IMPORTE;
                event_cierre->pagos_por_caja.can = - cierre_ant.pagos_por_caja.can + X_PAGOS_POR_CAJA_CANTIDAD;
                event_cierre->facturas_a.imp = - cierre_ant.facturas_a.imp + X_FACTURAS_A_IMPORTE;
                event_cierre->facturas_a.can = - cierre_ant.facturas_a.can + X_FACTURAS_A_CANTIDAD;
                event_cierre->facturas_b.imp = - cierre_ant.facturas_b.imp + X_FACTURAS_B_IMPORTE;
				event_cierre->facturas_b.can = - cierre_ant.facturas_b.can + X_FACTURAS_B_CANTIDAD;
                event_cierre->vueltos.imp = - cierre_ant.vueltos.imp + X_VUELTOS_IMPORTE;
                event_cierre->vueltos.can = - cierre_ant.vueltos.can + X_VUELTOS_CANTIDAD;
                event_cierre->bonos_entregados = - cierre_ant.bonos_entregados + X_BONOS_ENTREGADOS;
                event_cierre->diferencia = ROUND((X_DIFERENCIA - cierre_ant.diferencia), ENTEROS, DECIMALES );//PROBAR
                event_cierre->nc_a.imp = - cierre_ant.nc_a.imp + X_NC_A_IMPORTE;
                event_cierre->nc_a.can = - cierre_ant.nc_a.can + X_NC_A_CANTIDAD;
                event_cierre->nc_b.imp = - cierre_ant.nc_b.imp + X_NC_B_IMPORTE;
                event_cierre->nc_b.can = - cierre_ant.nc_b.can + X_NC_B_CANTIDAD;
                event_cierre->retencion_dgr = - cierre_ant.retencion_dgr + X_RETENCION_DGR;
                event_cierre->percepdion_dgi = - cierre_ant.percepdion_dgi + X_PERCEPCION_DGI;
                event_cierre->imp_correccion = - cierre_ant.imp_correccion + X_CORRECCION_IMPORTE;
                event_cierre->can_correcion = - cierre_ant.can_correcion + X_CORRECCION_CANTIDAD;
                event_cierre->primer_ticket = X_NRO_PRIMER_TICKET;//es el de la sesion actual
                event_cierre->cantidad_tickets = - cierre_ant.cantidad_tickets + X_CANTIDAD_TICKETS;
				event_cierre->cobros_cantidad_servicios = - cierre_ant.cobros_cantidad_servicios + X_COBROS_CANTIDAD_SERVICIOS;
				event_cierre->cobros_importe_servicios = - cierre_ant.cobros_importe_servicios + X_COBROS_IMPORTES_SERVICIOS;
                event_cierre->ret_rg_212_1 = - cierre_ant.ret_rg_212_1 + X_PERCEPCION_212_IVA1;
                event_cierre->ret_rg_212_2 = - cierre_ant.ret_rg_212_2 + X_PERCEPCION_212_IVA2;
                event_cierre->redondeo = - cierre_ant.redondeo + X_DIFERENCIA_REDONDEO;
                event_cierre->devoluciones.imp = - cierre_ant.devoluciones.imp + X_DEVOLUCIONES_IMPORTE;
                event_cierre->devoluciones.can = - cierre_ant.devoluciones.can + X_DEVOLUCIONES_CANTIDAD;
                event_cierre->cant_articulos = - cierre_ant.cant_articulos + X_CANT_ARTICULOS;
                event_cierre->cant_art_escaneados = - cierre_ant.cant_art_escaneados + X_CANT_ART_ESCANEADOS;
                event_cierre->tmpo_reg_articulo = - cierre_ant.tmpo_reg_articulo + X_TMPO_REG_ARTICULO; 
                event_cierre->tmpo_pago = - cierre_ant.tmpo_pago + X_TMPO_PAGO;
                event_cierre->tmpo_standby = - cierre_ant.tmpo_standby + X_TMPO_STANDBY;
                event_cierre->tmpo_ocio = - cierre_ant.tmpo_ocio + X_TMPO_OCIO;
                event_cierre->tmpo_no_venta = - cierre_ant.tmpo_no_venta + X_TMPO_NO_VENTA;
                event_cierre->anulados.imp = - cierre_ant.anulados.imp + X_IMP_CANCELADOS;
                event_cierre->anulados.can = - cierre_ant.anulados.can + X_CANT_CANCELADOS;
                event_cierre->cod_sucursal = config.sucursal;
				if( !NRO_FECHA_OPERACION )
				      SET_MEMORY_INT( __nro_fecha_operacion, GET_FECHA_DOS() );
				event_cierre->fecha_ticket = NRO_FECHA_OPERACION;
				event_cierre->donaciones.imp = - cierre_ant.donaciones.imp + X_DONACIONES_IMPORTE;
                event_cierre->donaciones.can = - cierre_ant.donaciones.can + X_DONACIONES_CANTIDAD;
				event_cierre->factura_a_importe_exento = - cierre_ant.factura_a_importe_exento + X_FACTURA_A_IMPORTE_EXENTO;
				event_cierre->factura_a_importe_gravado = - cierre_ant.factura_a_importe_gravado + X_FACTURA_A_IMPORTE_GRAVADO;
				event_cierre->nc_a_importe_exento = - cierre_ant.nc_a_importe_exento + X_NC_A_IMPORTE_EXENTO;
                event_cierre->nc_a_importe_gravado = - cierre_ant.nc_a_importe_gravado + X_NC_A_IMPORTE_GRAVADO;
                event_cierre->cons_final_importe_exent_fact = - cierre_ant.cons_final_importe_exent_fact + X_CONS_FINAL_IMPORTE_EXENT_FACT;
                event_cierre->cons_final_importe_grav_fact = - cierre_ant.cons_final_importe_grav_fact + X_CONS_FINAL_IMPORTE_GRAV_FACT;
                event_cierre->cons_final_cantidad_fact = - cierre_ant.cons_final_cantidad_fact + X_CONS_FINAL_CANTIDAD_FACT;
                event_cierre->dev_cons_f_importe_exent_fact = - cierre_ant.dev_cons_f_importe_exent_fact + X_DEV_CONS_F_IMPORTE_EXENT_FACT;
                event_cierre->dev_cons_f_importe_grav_fact = - cierre_ant.dev_cons_f_importe_grav_fact + X_DEV_CONS_F_IMPORTE_GRAV_FACT;
                event_cierre->dev_cons_f_cantidad_fact = - cierre_ant.dev_cons_f_cantidad_fact + X_DEV_CONS_F_CANTIDAD_FACT;
                event_cierre->ventas_diplom_importe = - cierre_ant.ventas_diplom_importe + X_VENTAS_DIPLOM_IMPORTE;
                event_cierre->ventas_diplom_cantidad = - cierre_ant.ventas_diplom_cantidad + X_VENTAS_DIPLOM_CANTIDAD;
                event_cierre->devoluciones_diplom_importe = - cierre_ant.devoluciones_diplom_importe + X_DEVOLUCIONES_DIPLOM_IMPORTE;
                event_cierre->devoluciones_diplom_cantidad = - cierre_ant.devoluciones_diplom_cantidad + X_DEVOLUCIONES_DIPLOM_CANTIDAD;
				event_cierre->nro_transaccion_inicial = - cierre_ant.nro_transaccion_inicial + X_NRO_TRANSACCION_INICIAL;
				event_cierre->nro_transaccion_final = - cierre_ant.nro_transaccion_final + X_NRO_TRANSACCION_FINAL;
                event_cierre->devoluciones_export_importe = - cierre_ant.devoluciones_export_importe + X_DEVOLUCIONES_EXPORT_IMPORTE;
                event_cierre->devoluciones_export_cantidad = - cierre_ant.devoluciones_export_cantidad + X_DEVOLUCIONES_EXPORT_CANTIDAD;
                event_cierre->cons_final_importe_exent_tkt = - cierre_ant.cons_final_importe_exent_tkt + X_CONS_FINAL_IMPORTE_EXENT_TKT;
                event_cierre->cons_final_importe_grav_tkt = - cierre_ant.cons_final_importe_grav_tkt + X_CONS_FINAL_IMPORTE_GRAV_TKT;
                event_cierre->cons_final_cantidad_tkt = - cierre_ant.cons_final_cantidad_tkt + X_CONS_FINAL_CANTIDAD_TKT;
                event_cierre->dev_cons_f_importe_exent_tkt = - cierre_ant.dev_cons_f_importe_exent_tkt + X_DEV_CONS_FINAL_IMPORTE_EXENT_TKT;
                event_cierre->dev_cons_f_importe_grav_tkt = - cierre_ant.dev_cons_f_importe_grav_tkt + X_DEV_CONS_F_IMPORTE_GRAV_TKT;
                event_cierre->dev_cons_f_cantidad_tkt = - cierre_ant.dev_cons_f_cantidad_tkt + X_DEV_CONS_F_CANTIDAD_TKT;
                event_cierre->iva_factura_a = - cierre_ant.iva_factura_a + X_IVA_FACTURA_A;
                event_cierre->iva_cons_final = - cierre_ant.iva_cons_final + X_IVA_CONS_FINAL;
                event_cierre->ventas_export_importe = - cierre_ant.ventas_export_importe + X_VENTAS_EXPORT_IMPORTE;
                event_cierre->ventas_export_cantidad = - cierre_ant.ventas_export_cantidad + X_VENTAS_EXPORT_CANTIDAD;
                event_cierre->ofertas_importe = - cierre_ant.ofertas_importe + X_OFERTAS_IMPORTE;
                event_cierre->iva_nc_a = - cierre_ant.iva_nc_a + X_IVA_NC_A;
                event_cierre->iva_devol_consumidor_final = - cierre_ant.iva_devol_consumidor_final + X_IVA_DEVOL_CONSUMIDOR_FINAL;
				
				event_cierre->cant_cierres_y = cant_cierres_y + 1; // empieza el conteo nuevamente!
				
				for(i=0;i<4;i++)
					event_cierre->impuestos[i] = - cierre_ant.impuestos[i] + _X_IMPUESTOS(i);//new
				for(i=0;i<11;i++)
					event_cierre->descuentos_medio[i] = - cierre_ant.descuentos_medio[i]+ _X_DESCUENTOS_MEDIO(i); //new 
	}	
UPDATE_VARIABLES__Z_(); //actualizo las variables __Z_
BORRAR_ARCHIVO_REC( ARCH_REC_CIERRE_ANT );
//SELECT( area );
SELECT_TABLE( tabla_ant, tipo_ant );

}

/*---------------------------------------------------------------------------------------*/
void CHECK_SUBMEDIOS_REC()//Debuging puedo chequear los datos grabado en el archvo submedios.rec 
/*---------------------------------------------------------------------------------------*/
{
	int cnt=0,cant=0;

	if((cant = CANTIDAD_REGISTROS_REC(ARCH_REC_SUBMEDIOS,sizeof( struct __sub_medios ))) > 0)// Recupero lo de submedios.rec y lo pongo en una struct
	{
		while(cnt < cant)
		{
			RECUPERAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS, ( char* )&subTmp, sizeof( struct __sub_medios ), cnt );
			subTmp.importe;
			subTmp.importe_dec;
			subTmp.medios;
			subTmp.submedios;
			cnt++;
		}
	}
}
/*---------------------------------------------------------------------------------------*/
void UPDATE_SUBMEDIOS_REC()
/*---------------------------------------------------------------------------------------*/
{
    int cnt = 0;
	int error =0;
	
	BORRAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS );//Lo borro por las dudas que esté y tenga basura
	GUARDAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS, (char*)&subTmp, sizeof( struct __sub_medios ) ); 

	if(POSICIONA_PAGO(cierres_y->id_evento))
	{
		while( /*!BtrvEOF()*/ !dbEOF() )
			{
			subTmp.importe = pagos.importe;
			subTmp.medios = pagos.modo_pago;
			subTmp.submedios = pagos.cod_sub_medio;
			subTmp.importe_dec = pagos.imp_pago;
			
			ACTUALIZAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS, ( char* )&subTmp, sizeof( struct __sub_medios ), cnt );
			cnt++;
			
			//SELECT( AREA_AUX_MED );
			//GET_NEXT();
			SELECT_TABLE( T_EVM_TMP, TT_ORIG );
			SKIP2( 1 );

			}
	}
		

	CHECK_SUBMEDIOS_REC();
}
/*---------------------------------------------------------------------------------------*/
void GUARDAR_DATOS_IMPORTADOS()
/*---------------------------------------------------------------------------------------*/
{
	int /*area*/ tabla_ant, tipo_ant;

	//area = SELECTED();
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	
	
	SELECT(AREA_CAJ_FLOTANTE);

	GUARDAR_ARCHIVO_REC( ARCH_REC_CIERRE_ANT, (char*)&cierre, sizeof( struct _ev_cierre ) ); 
	
	GRABAR_LOG_SISTEMA( "SE GUARDA ARCHIVO ev_cierre_ant.rec" ,LOG_ERRORES,4);

	//SELECT(area);
	SELECT_TABLE( tabla_ant, tipo_ant );

}
/*---------------------------------------------------------------------------------------*/
void UPDATE_VARIABLES__X_()
/*---------------------------------------------------------------------------------------*/
{
int i, area;

area = SELECTED();

SELECT( AREA_CAJ_FLOTANTE );

for(i=0; i<10; i++) 
{
	_SET_MEMORY_DOUBLE( __x_ventas_importe, i, cierre.imp_ventas[i]);
	_SET_MEMORY_INT( __x_ventas_cantidad, i, cierre.can_ventas[i]);
	_SET_MEMORY_DOUBLE( __x_cobros_importe, i, cierre.imp_cobros[i]);
    _SET_MEMORY_INT( __x_cobros_cantidad, i, cierre.can_cobros[i]);
	_SET_MEMORY_DOUBLE( __x_fondo_fijo_multiple_importe, i, cierre.imp_fondo_fijo_multiple[i]);
    _SET_MEMORY_DOUBLE( __x_fondo_fijo_multiple_imp_me, i, cierre.imp_fondo_fijo_multiple_me[i]);
	_SET_MEMORY_INT( __x_fondo_fijo_multiple_cantidad, i, cierre.can_fondo_fijo_multiple[i]);
    _SET_MEMORY_DOUBLE( __x_importe_iva, i, cierre.importes_iva[i]);
	_SET_MEMORY_DOUBLE( __x_importe_sobretasa_iva, i, cierre.sobretasas_iva[i]);
	_SET_MEMORY_DOUBLE( __x_retiros_importe, i, cierre.imp_retiros[i]);
	_SET_MEMORY_DOUBLE( __x_retiros_importe_me, i, cierre.imp_retiros_me[i]);
	_SET_MEMORY_INT( __x_retiros_cantidad, i, cierre.can_retiros[i]);
	_SET_MEMORY_DOUBLE( __x_cambios_importe, i, cierre.imp_cambios[i]);
	_SET_MEMORY_INT( __x_cambios_cantidad, i, cierre.can_cambios[i]);       
    _SET_MEMORY_DOUBLE( __x_devoluciones_medio, i, cierre.devoluciones_medio[i]);
	_SET_MEMORY_DOUBLE( __x_mozos_importe, i, cierre.imp_pagosesp[i]);
	_SET_MEMORY_DOUBLE( __x_mozos_cantidad, i, cierre.can_pagosesp[i]);
	_SET_MEMORY_DOUBLE( __x_ventas_importe_sin_redondeo, i, cierre.ventas_importe_sin_redondeo[i]); //new
	_SET_MEMORY_DOUBLE( __x_retiro_fondo_importe_pagos, i, cierre.retiro_fondo_importe_pago[i]); //new
	_SET_MEMORY_DOUBLE( __x_cobros_importe_me, i, cierre.cobros_importe_me[i]); //new
}               
SET_MEMORY_DOUBLE( __x_adelantos_importe, cierre.adelantos.imp);                
SET_MEMORY_INT( __x_adelantos_cantidad, cierre.adelantos.can);                

SET_MEMORY_DOUBLE( __x_intereses_importe, cierre.intereses.imp);                                
SET_MEMORY_INT( __x_intereses_cantidad, cierre.intereses.can);                                

SET_MEMORY_DOUBLE( __x_fondo_fijo_importe, cierre.fondo_fijo.imp);                                
SET_MEMORY_INT( __x_fondo_fijo_cantidad, cierre.fondo_fijo.can);                 

SET_MEMORY_DOUBLE( __x_anulaciones_importe, cierre.anulaciones.imp);   
SET_MEMORY_INT( __x_anulaciones_cantidad, cierre.anulaciones.can);  
                
SET_MEMORY_DOUBLE( __x_dev_envases_importe, cierre.dev_envases.imp);   
SET_MEMORY_INT( __x_dev_envases_cantidad, cierre.dev_envases.can);  

SET_MEMORY_DOUBLE( __x_ventas_gravadas, cierre.ventas_gravadas);   
SET_MEMORY_DOUBLE( __x_ventas_exentas, cierre.ventas_exentas);   

SET_MEMORY_DOUBLE( __x_imp_int, cierre.imp_int);   
SET_MEMORY_DOUBLE( __x_tot_dinero, cierre.tot_dinero); 

SET_MEMORY_DOUBLE( __x_iva1, cierre.iva1);   
SET_MEMORY_DOUBLE( __x_iva2, cierre.iva2); 

SET_MEMORY_DOUBLE( __x_retiro_fondo_importe, cierre.retiro_fondo.imp);   
SET_MEMORY_INT( __x_retiro_fondo_cantidad, cierre.retiro_fondo.can); 

SET_MEMORY_DOUBLE( __x_pagos_por_caja_importe, cierre.pagos_por_caja.imp);   
SET_MEMORY_INT( __x_pagos_por_caja_cantidad, cierre.pagos_por_caja.can); 

SET_MEMORY_DOUBLE( __x_facturas_a_importe, cierre.facturas_a.imp);   
SET_MEMORY_INT( __x_facturas_a_cantidad, cierre.facturas_a.can); 

SET_MEMORY_DOUBLE( __x_facturas_b_importe, cierre.facturas_b.imp);   
SET_MEMORY_INT( __x_facturas_b_cantidad, cierre.facturas_b.can); 

SET_MEMORY_DOUBLE( __x_vueltos_importe, cierre.vueltos.imp);   
SET_MEMORY_INT( __x_vueltos_cantidad, cierre.vueltos.can); 

SET_MEMORY_FLOAT(__x_bonos_entregados, cierre.bonos_entregados); // UNSIGNED
SET_MEMORY_DOUBLE(__x_diferencia, cierre.diferencia); //ROUND( X_DIFERENCIA, ENTEROS, DECIMALES );

SET_MEMORY_DOUBLE(__x_nc_a_importe, cierre.nc_a.imp); //X_NC_A_IMPORTE;
SET_MEMORY_INT(__x_nc_a_cantidad, cierre.nc_a.can); // = X_NC_A_CANTIDAD;

SET_MEMORY_DOUBLE(__x_nc_b_importe, cierre.nc_b.imp); // = X_NC_B_IMPORTE;
SET_MEMORY_INT(__x_nc_b_cantidad, cierre.nc_b.can); // = X_NC_B_CANTIDAD;                                   

SET_MEMORY_DOUBLE(__x_retencion_dgr, cierre.retencion_dgr); // = X_RETENCION_DGR;
SET_MEMORY_DOUBLE(__x_percepcion_dgi, cierre.percepdion_dgi); // = X_PERCEPCION_DGI;                    

SET_MEMORY_DOUBLE(__x_correccion_importe, cierre.imp_correccion); // = X_CORRECCION_IMPORTE;
SET_MEMORY_INT(__x_correccion_cantidad, cierre.can_correcion); // = X_CORRECCION_CANTIDAD;

SET_MEMORY_LONG(__x_nro_primer_ticket, cierre.primer_ticket); // = X_NRO_PRIMER_TICKET;
SET_MEMORY_LONG(__x_cantidad_tickets, cierre.cantidad_tickets); // = X_CANTIDAD_TICKETS;                

SET_MEMORY_INT(__x_cobros_cantidad_servicios, cierre.cobros_cantidad_servicios); // = X_COBROS_CANTIDAD_SERVICIOS;
SET_MEMORY_DOUBLE(__x_cobros_importes_servicios, cierre.cobros_importe_servicios); // = X_COBROS_CANTIDAD_SERVICIOS;
SET_MEMORY_DOUBLE(__x_percepcion_212_iva1, cierre.ret_rg_212_1); // = X_PERCEPCION_212_IVA1;
SET_MEMORY_DOUBLE(__x_percepcion_212_iva2, cierre.ret_rg_212_2); // = X_PERCEPCION_212_IVA2;
SET_MEMORY_DOUBLE(__x_diferencia_redondeo, cierre.redondeo); // = X_DIFERENCIA_REDONDEO;

SET_MEMORY_DOUBLE(__x_devoluciones_importe, cierre.devoluciones.imp); // = X_DEVOLUCIONES_IMPORTE;
SET_MEMORY_INT(__x_devoluciones_cantidad, cierre.devoluciones.can); // = X_DEVOLUCIONES_CANTIDAD;

SET_MEMORY_INT(__x_cant_articulos, cierre.cant_articulos); // = X_CANT_ARTICULOS;
SET_MEMORY_INT(__x_cant_art_escaneados, cierre.cant_art_escaneados); // = X_CANT_ART_ESCANEADOS;

SET_MEMORY_FLOAT(__x_tmpo_reg_articulo, cierre.tmpo_reg_articulo); // = X_TMPO_REG_ARTICULO;
SET_MEMORY_FLOAT(__x_tmpo_pago, cierre.tmpo_pago); // = X_TMPO_PAGO;
SET_MEMORY_FLOAT(__x_tmpo_standby, cierre.tmpo_standby); // = X_TMPO_STANDBY;
SET_MEMORY_FLOAT(__x_tmpo_ocio, cierre.tmpo_ocio); // = X_TMPO_OCIO;
SET_MEMORY_FLOAT(__x_tmpo_no_venta, cierre.tmpo_no_venta); // = X_TMPO_NO_VENTA;

SET_MEMORY_DOUBLE(__x_imp_cancelados, cierre.anulados.imp); // = X_IMP_CANCELADOS;
SET_MEMORY_INT(__x_cant_cancelados, cierre.anulados.can); // = X_CANT_CANCELADOS;
                
//cierre.cod_sucursal = config.sucursal;

//cierre.fecha_ticket = ( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_HORA_DOS();
                
SET_MEMORY_DOUBLE(__x_donaciones_importe, cierre.donaciones.imp);// = X_DONACIONES_IMPORTE;
SET_MEMORY_INT(__x_donaciones_cantidad, cierre.donaciones.can);// = X_DONACIONES_CANTIDAD;

SET_MEMORY_DOUBLE(__x_factura_a_importe_exento, cierre.factura_a_importe_exento);// = X_FACTURA_A_IMPORTE_EXENTO;
SET_MEMORY_DOUBLE(__x_factura_a_importe_gravado, cierre.factura_a_importe_gravado);// = X_FACTURA_A_IMPORTE_GRAVADO;

SET_MEMORY_DOUBLE(__x_nc_a_importe_exento, cierre.nc_a_importe_exento);// = X_NC_A_IMPORTE_EXENTO;
SET_MEMORY_DOUBLE(__x_nc_a_importe_gravado, cierre.nc_a_importe_gravado);// = X_NC_A_IMPORTE_GRAVADO;

SET_MEMORY_DOUBLE(__x_cons_final_importe_exent_fact, cierre.cons_final_importe_exent_fact);// = X_CONS_FINAL_IMPORTE_EXENT_FACT;
SET_MEMORY_DOUBLE(__x_cons_final_importe_grav_fact, cierre.cons_final_importe_grav_fact);// = X_CONS_FINAL_IMPORTE_GRAV_FACT;
SET_MEMORY_INT(__x_cons_final_cantidad_fact, cierre.cons_final_cantidad_fact);// = X_CONS_FINAL_CANTIDAD_FACT;

SET_MEMORY_DOUBLE(__x_dev_cons_f_importe_exent_fact, cierre.dev_cons_f_importe_exent_fact);// = X_DEV_CONS_F_IMPORTE_EXENT_FACT;
SET_MEMORY_DOUBLE(__x_dev_cons_f_importe_grav_fact, cierre.dev_cons_f_importe_grav_fact);// = X_DEV_CONS_F_IMPORTE_GRAV_FACT;
SET_MEMORY_INT(__x_dev_cons_f_cantidad_fact, cierre.dev_cons_f_cantidad_fact);// = X_DEV_CONS_F_CANTIDAD_FACT;

SET_MEMORY_DOUBLE(__x_ventas_diplom_importe, cierre.ventas_diplom_importe);// = X_VENTAS_DIPLOM_IMPORTE;
SET_MEMORY_INT(__x_ventas_diplom_cantidad, cierre.ventas_diplom_cantidad);// = X_VENTAS_DIPLOM_CANTIDAD;

SET_MEMORY_DOUBLE(__x_devoluciones_diplom_importe, cierre.devoluciones_diplom_importe);// = X_DEVOLUCIONES_DIPLOM_IMPORTE;
SET_MEMORY_INT(__x_devoluciones_diplom_cantidad, cierre.devoluciones_diplom_cantidad);// = X_DEVOLUCIONES_DIPLOM_CANTIDAD;

SET_MEMORY_LONG(__x_nro_transaccion_inicial, cierre.nro_transaccion_inicial);// = X_NRO_TRANSACCION_INICIAL;
SET_MEMORY_LONG(__x_nro_transaccion_final, cierre.nro_transaccion_final);// = X_NRO_TRANSACCION_FINAL;

SET_MEMORY_DOUBLE(__x_devoluciones_export_importe, cierre.devoluciones_export_importe);// = X_DEVOLUCIONES_EXPORT_IMPORTE;
SET_MEMORY_INT(__x_devoluciones_export_cantidad, cierre.devoluciones_export_cantidad);// = X_DEVOLUCIONES_EXPORT_CANTIDAD;

SET_MEMORY_DOUBLE(__x_cons_final_importe_exent_tkt, cierre.cons_final_importe_exent_tkt);// = X_CONS_FINAL_IMPORTE_EXENT_TKT;
SET_MEMORY_DOUBLE(__x_cons_final_importe_grav_tkt, cierre.cons_final_importe_grav_tkt);// = X_CONS_FINAL_IMPORTE_GRAV_TKT;
SET_MEMORY_INT(__x_cons_final_cantidad_tkt, cierre.cons_final_cantidad_tkt);// = X_CONS_FINAL_CANTIDAD_TKT;

SET_MEMORY_DOUBLE(__x_dev_cons_final_importe_exent_tkt, cierre.dev_cons_f_importe_exent_tkt);// = X_DEV_CONS_FINAL_IMPORTE_EXENT_TKT;
SET_MEMORY_DOUBLE(__x_dev_cons_f_importe_grav_tkt, cierre.dev_cons_f_importe_grav_tkt);// = X_DEV_CONS_F_IMPORTE_GRAV_TKT;
SET_MEMORY_INT(__x_dev_cons_f_cantidad_tkt, cierre.dev_cons_f_cantidad_tkt);// = X_DEV_CONS_F_CANTIDAD_TKT;

SET_MEMORY_DOUBLE(__x_iva_factura_a, cierre.iva_factura_a);// = X_IVA_FACTURA_A;
SET_MEMORY_DOUBLE(__x_iva_cons_final, cierre.iva_cons_final);// = X_IVA_CONS_FINAL;

SET_MEMORY_DOUBLE(__x_ventas_export_importe, cierre.ventas_export_importe);// = X_VENTAS_EXPORT_IMPORTE;
SET_MEMORY_INT(__x_ventas_export_cantidad, cierre.ventas_export_cantidad);// = X_VENTAS_EXPORT_CANTIDAD;
SET_MEMORY_DOUBLE(__x_ofertas_importe, cierre.ofertas_importe);// = X_OFERTAS_IMPORTE;
SET_MEMORY_DOUBLE(__x_iva_nc_a, cierre.iva_nc_a);// = X_IVA_NC_A;
SET_MEMORY_DOUBLE(__x_iva_devol_consumidor_final, cierre.iva_devol_consumidor_final);// = X_IVA_DEVOL_CONSUMIDOR_FINAL;

for(i=0;i<4;i++)
	_SET_MEMORY_DOUBLE(__x_impuestos, i, cierre.impuestos[i] );//= _X_IMPUESTOS(i); //New
for(i=0;i<11;i++)
	_SET_MEMORY_DOUBLE(__x_descuentos_medio, i , cierre.descuentos_medio[i]);//= _X_DESCUENTOS_MEDIO(i); //New

SELECT(area);

}
/*---------------------------------------------------------------------------------------*/
void UPDATE_VARIABLES__Z_()
/*---------------------------------------------------------------------------------------*/
{
int i/*, area*/, tabla_ant, tipo_ant;
//area = SELECTED();
tabla_ant = SELECTED2();
tipo_ant = SELECTED_TIPO();

//SELECT(AREA_CIERRE);
SELECT_TABLE( T_EV_CIERRE, tipo_t_ev_cierre );

for(i=0; i<10; i++) 
{
	_SET_MEMORY_DOUBLE( __z_ventas_importe, i, event_cierre->imp_ventas[i]);
	_SET_MEMORY_INT( __z_ventas_cantidad, i, event_cierre->can_ventas[i]);
	_SET_MEMORY_DOUBLE( __z_cobros_importe, i, event_cierre->imp_cobros[i]);
    _SET_MEMORY_INT( __z_cobros_cantidad, i, event_cierre->can_cobros[i]);
	_SET_MEMORY_DOUBLE( __z_fondo_fijo_multiple_importe, i, event_cierre->imp_fondo_fijo_multiple[i]);
	_SET_MEMORY_INT( __z_fondo_fijo_multiple_cantidad, i, event_cierre->can_fondo_fijo_multiple[i]);
    _SET_MEMORY_DOUBLE( __z_importe_iva, i, event_cierre->importes_iva[i]);
	_SET_MEMORY_DOUBLE( __z_importe_sobretasa_iva, i, event_cierre->sobretasas_iva[i]);
	_SET_MEMORY_DOUBLE( __z_retiros_importe, i, event_cierre->imp_retiros[i]);
	_SET_MEMORY_INT( __z_retiros_cantidad, i, event_cierre->can_retiros[i]);
	_SET_MEMORY_DOUBLE( __z_cambios_importe, i, event_cierre->imp_cambios[i]);
	_SET_MEMORY_INT( __z_cambios_cantidad, i, event_cierre->can_cambios[i]);       
    _SET_MEMORY_DOUBLE( __z_devoluciones_medio, i, event_cierre->devoluciones_medio[i]);

	_SET_MEMORY_DOUBLE( __z_descuentos_medio, i, event_cierre->descuentos_medio[i]);
	_SET_MEMORY_DOUBLE( __z_ventas_importe_sin_redondeo, i, event_cierre->ventas_importe_sin_redondeo[i]);
}               
SET_MEMORY_DOUBLE( __z_adelantos_importe, event_cierre->adelantos.imp);                
SET_MEMORY_INT( __z_adelantos_cantidad, event_cierre->adelantos.can);                
SET_MEMORY_DOUBLE( __z_intereses_importe, event_cierre->intereses.imp);                                
SET_MEMORY_INT( __z_intereses_cantidad, event_cierre->intereses.can);                                
SET_MEMORY_DOUBLE( __z_fondo_fijo_importe, event_cierre->fondo_fijo.imp);                                
SET_MEMORY_INT( __z_fondo_fijo_cantidad, event_cierre->fondo_fijo.can);                 
SET_MEMORY_DOUBLE( __z_anulaciones_importe, event_cierre->anulaciones.imp);   
SET_MEMORY_INT( __z_anulaciones_cantidad, event_cierre->anulaciones.can);                 
SET_MEMORY_DOUBLE( __z_dev_envases_importe, event_cierre->dev_envases.imp);   
SET_MEMORY_INT( __z_dev_envases_cantidad, event_cierre->dev_envases.can);  
SET_MEMORY_DOUBLE( __z_ventas_gravadas, event_cierre->ventas_gravadas);   
SET_MEMORY_DOUBLE( __z_ventas_exentas, event_cierre->ventas_exentas);   
SET_MEMORY_DOUBLE( __z_imp_int, event_cierre->imp_int);   
SET_MEMORY_DOUBLE( __z_tot_dinero, event_cierre->tot_dinero); 
SET_MEMORY_DOUBLE( __z_iva1, event_cierre->iva1);   
SET_MEMORY_DOUBLE( __z_iva2, event_cierre->iva2); 
SET_MEMORY_DOUBLE( __z_retiro_fondo_importe, event_cierre->retiro_fondo.imp);   
SET_MEMORY_INT( __z_retiro_fondo_cantidad, event_cierre->retiro_fondo.can); 
SET_MEMORY_DOUBLE( __z_pagos_por_caja_importe, event_cierre->pagos_por_caja.imp);   
SET_MEMORY_INT( __z_pagos_por_caja_cantidad, event_cierre->pagos_por_caja.can); 
SET_MEMORY_DOUBLE( __z_facturas_a_importe, event_cierre->facturas_a.imp);   
SET_MEMORY_DOUBLE( __z_factura_a_importe_gravado, event_cierre->factura_a_importe_gravado);   
SET_MEMORY_INT( __z_facturas_a_cantidad, event_cierre->facturas_a.can); 
SET_MEMORY_DOUBLE( __z_facturas_b_importe, event_cierre->facturas_b.imp);   
SET_MEMORY_INT( __z_facturas_b_cantidad, event_cierre->facturas_b.can); 
SET_MEMORY_DOUBLE( __z_vueltos_importe, event_cierre->vueltos.imp);   
SET_MEMORY_INT( __z_vueltos_cantidad, event_cierre->vueltos.can); 
SET_MEMORY_FLOAT(__z_bonos_entregados, event_cierre->bonos_entregados); 
SET_MEMORY_DOUBLE(__z_diferencia, event_cierre->diferencia); 
SET_MEMORY_DOUBLE(__z_nc_a_importe, event_cierre->nc_a.imp); 
SET_MEMORY_INT(__z_nc_a_cantidad, event_cierre->nc_a.can); 
SET_MEMORY_DOUBLE(__z_nc_b_importe, event_cierre->nc_b.imp); 
SET_MEMORY_INT(__z_nc_b_cantidad, event_cierre->nc_b.can);                                
SET_MEMORY_DOUBLE(__z_retencion_dgr, event_cierre->retencion_dgr); 
SET_MEMORY_DOUBLE(__z_percepcion_dgi, event_cierre->percepdion_dgi);                  
SET_MEMORY_DOUBLE(__z_correccion_importe, event_cierre->imp_correccion); 
SET_MEMORY_INT(__z_correccion_cantidad, event_cierre->can_correcion); 
SET_MEMORY_LONG(__z_nro_primer_ticket, event_cierre->primer_ticket); 
SET_MEMORY_LONG(__z_cantidad_tickets, event_cierre->cantidad_tickets);                 
SET_MEMORY_DOUBLE(__z_percepcion_212_iva1, event_cierre->ret_rg_212_1); 
SET_MEMORY_DOUBLE(__z_percepcion_212_iva2, event_cierre->ret_rg_212_2);
SET_MEMORY_DOUBLE(__z_diferencia_redondeo, event_cierre->redondeo); 
SET_MEMORY_DOUBLE(__z_devoluciones_importe, event_cierre->devoluciones.imp); 
SET_MEMORY_INT(__z_devoluciones_cantidad, event_cierre->devoluciones.can); 
SET_MEMORY_INT(__z_cant_articulos, event_cierre->cant_articulos); 
SET_MEMORY_INT(__z_cant_art_escaneados, event_cierre->cant_art_escaneados); 
SET_MEMORY_FLOAT(__z_tmpo_reg_articulo, event_cierre->tmpo_reg_articulo); 
SET_MEMORY_FLOAT(__z_tmpo_pago, event_cierre->tmpo_pago); 
SET_MEMORY_FLOAT(__z_tmpo_standby, event_cierre->tmpo_standby); 
SET_MEMORY_FLOAT(__z_tmpo_ocio, event_cierre->tmpo_ocio); 
SET_MEMORY_FLOAT(__z_tmpo_no_venta, event_cierre->tmpo_no_venta); 
SET_MEMORY_DOUBLE(__z_imp_cancelados, event_cierre->anulados.imp); 
SET_MEMORY_INT(__z_cant_cancelados, event_cierre->anulados.can); 
SET_MEMORY_INT(__z_cobros_cantidad_servicios, event_cierre->cobros_cantidad_servicios);                      
SET_MEMORY_DOUBLE(__z_cobros_importes_servicios, event_cierre->cobros_importe_servicios);                      
SET_MEMORY_DOUBLE(__z_donaciones_importe, event_cierre->donaciones.imp);
SET_MEMORY_INT(__z_donaciones_cantidad, event_cierre->donaciones.can);

SET_MEMORY_DOUBLE( __z_factura_a_importe_exento, event_cierre->factura_a_importe_exento);   
SET_MEMORY_DOUBLE( __z_nc_a_importe_gravado, event_cierre->nc_a_importe_gravado);   
SET_MEMORY_DOUBLE( __z_nc_a_importe_exento, event_cierre->nc_a_importe_exento);
SET_MEMORY_DOUBLE( __z_cons_final_importe_exent_fact, event_cierre->cons_final_importe_exent_fact );
SET_MEMORY_DOUBLE( __z_cons_final_importe_grav_fact, event_cierre->cons_final_importe_grav_fact );
SET_MEMORY_INT(__z_cons_final_cantidad_fact, event_cierre->cons_final_cantidad_fact );
SET_MEMORY_DOUBLE( __z_cons_final_importe_exent_tkt, event_cierre->cons_final_importe_exent_tkt );
SET_MEMORY_DOUBLE( __z_cons_final_importe_grav_tkt, event_cierre->cons_final_importe_grav_tkt );
SET_MEMORY_INT(__z_cons_final_cantidad_tkt, event_cierre->cons_final_cantidad_tkt );
SET_MEMORY_DOUBLE( __z_dev_cons_f_importe_exent_tkt, event_cierre->dev_cons_f_importe_exent_tkt );
SET_MEMORY_DOUBLE( __z_dev_cons_f_importe_grav_tkt, event_cierre->dev_cons_f_importe_grav_tkt );
SET_MEMORY_INT(__z_dev_cons_f_cantidad_tkt, event_cierre->dev_cons_f_cantidad_tkt );
SET_MEMORY_DOUBLE( __z_iva_factura_a, event_cierre->iva_factura_a );
SET_MEMORY_DOUBLE( __z_iva_cons_final, event_cierre->iva_cons_final);
SET_MEMORY_DOUBLE( __z_oferta_importe, event_cierre->ofertas_importe);
SET_MEMORY_DOUBLE( __z_oferta_importe, event_cierre->ofertas_importe);
SET_MEMORY_DOUBLE( __z_iva_nc_a, event_cierre->iva_nc_a);
SET_MEMORY_DOUBLE( __z_iva_devol_consumidor_final, event_cierre->iva_devol_consumidor_final);
//SELECT(area);
SELECT_TABLE( tabla_ant, tipo_ant );
}
/*---------------------------------------------------------------------------------------*/
void OBTENER_TABLAS_TEMPORALES(long secuencia, int caja, int fecha, long id_evento)
/*---------------------------------------------------------------------------------------*/

{
	int error=0; 

	error = OBTENER_TEMPORALES(_EVENT_Y, caja, 0, secuencia, fecha, _ORIGEN_CAJERO_FLOTANTE, SI, id_evento);
	
	if(!error)//Si no hay error al obtener las tablas, cargo las structs con los datos correspondientes
	{
		if(!ABRIR_EVENTOS_TEMPORALES( &events, &cont, &pagos, &despro, &dpago, &cod_barra, &ev_b_promo))
			GRABAR_LOG_SISTEMA( "Error abriendo las tablas temporales" ,LOG_ERRORES,2);
		if(!ABRIR_EV_CIERRE(&cierre))
			GRABAR_LOG_SISTEMA( "Error abriendo la tabla EV_CIERRE",LOG_ERRORES,2 );
	}
}
/*---------------------------------------------------------------------------------------*/
int ABRIR_EV_CIERRE( struct _ev_cierre *ev_cierre)
/*---------------------------------------------------------------------------------------*/
{
    int error = 0;

	if(  OPENED( T_EVCI_TMP, TT_ORIG ) /*USED( AREA_CAJ_FLOTANTE )*/ ) 
       GRABAR_LOG_SISTEMA( "Error abriendo AREA_CAJ_FLOTANTE" ,LOG_ERRORES,2);

	//error = USE_DATABASE( AREA_CAJ_FLOTANTE, EVCIERRE_TMP, ( char* )ev_cierre, sizeof( struct _ev_cierre ), NULL, 0, 0 );
	error = OPEN_TABLE( T_EVCI_TMP, TT_ORIG, ( char * ) &cobros, sizeof( struct _cobros ) ); 
	if( error == 0 ) {
	   char buff[100];
	   _snprintf(buff,100,"E:R(%i)",RECCOUNT2( NULL, NULL )/*RECCOUNT(  )*/);
	   GRABAR_LOG_SISTEMA(buff,LOG_ERRORES,2);
	}
    
    if( !error ) {
        //SELECT( AREA_CAJ_FLOTANTE );
		SELECT_TABLE( T_EVCI_TMP, TT_ORIG );
        if( !RECCOUNT2( NULL, NULL )/*!RECCOUNT()*/ ) {
            MENSAJE_STRING( S_LA_TRANSACCION_ESPECIFICADA_NO_EXISTE );
            error = 1;
        }
	} else {
		GRABAR_LOG_SISTEMA("ERROR abriendo ev_cierre",LOG_ERRORES,2);
	}

    return !error;

}
/*---------------------------------------------------------------------------------------*/
int OBTENER_TEMPORALES( int ev_requerido, int caja, long nro_ticket, long secuencia,
	int fecha, char origen, int online, long id_evento)
/*---------------------------------------------------------------------------------------*/
{
	char _where[150];
    int error = 0;
	int dd,mm,aa;

	if( origen == _ORIGEN_CAJERO_FLOTANTE) 
	{
		if( ev_requerido ) 
			_snprintf( _where,sizeof(_where), " caja = %d AND nro_secuencia = %ld AND tipo_evento = %d", caja, secuencia, ev_requerido );
		else 
			_snprintf( _where,sizeof(_where), " caja = %d AND nro_secuencia = %ld", caja, secuencia );
	}
	if(fecha)
	{
		FECHA__( fecha, &dd, &mm, &aa );
		_snprintf( _where,sizeof(_where), " caja = %ld AND nro_secuencia = %ld AND tipo_evento = %ld AND fecha_ticket = '%04ld%02ld%02ld'", caja,
		secuencia, ev_requerido, aa, mm, dd );
	}
	
	GRABAR_LOG_DBROUTER(_where,LOG_DEBUG,2);
	
	/*Obtengo la tabla eventos*/
	if( OBTENER_TABLA_SERVIDOR( T_EV_TMP, TT_ORIG, "eventos", _where, SI, NULL ) ) 
		error = 1;
    
	/*Si no hay error al obtenerla y la puedo usar*/
    //if( error && ( USE_DATABASE( AREA_AUX3, EV_TMP, ( char* )&events, sizeof( struct _event_ticket ), NULL, 0, 0 ) != 0 ) ) 
	if( error && ( OPEN_TABLE( T_EV_TMP, TT_ORIG, ( char* )&events, sizeof( struct _eventos ) ) != 0 ) ) 
		error += 1;

	_snprintf( _where,sizeof(_where), " caja = %ld AND id_evento = %ld AND fecha_ticket = '%04ld%02ld%02ld'", caja, id_evento, aa, mm, dd );
	GRABAR_LOG_DBROUTER(_where,LOG_DEBUG,2);

	/*Obtengo la tabla medios*/ 
	if( OBTENER_TABLA_SERVIDOR( T_EVM_TMP, TT_ORIG, "ev_medios", _where, SI, NULL ) ) 
		error += 1;
        
	/*Si no hay error al obtenerla y la puedo usar*/
	//if( error && ( USE_DATABASE( AREA_AUX_MED, EVM_TMP, ( char* )&pagos, sizeof( struct _event_pago ), NULL, 0, 0 ) != 0 ) ) 
	if( error && ( OPEN_TABLE( T_EVM_TMP, TT_ORIG, ( char* )&pagos, sizeof( struct _ev_medios ) ) != 0 ) ) 
		error += 1;

	/*Obtengo la tabla ev_cierre*/
	if( OBTENER_TABLA_SERVIDOR( EVCIERRE_TMP, TT_ORIG, "ev_cierre", _where, SI, NULL ) ) 
		error += 1;

		/*Si no hay error al obtenerla y la puedo usar*/
        if( error && ( USE_DATABASE( AREA_CAJ_FLOTANTE, EVCIERRE_TMP, ( char* )&cierre, sizeof( struct _ev_cierre ), NULL, 0, 0 ) != 0 ) ) 
			error += 1;

		

	return error;
}
/*****************************************************************************/
int POSICIONA_PAGO(long id_evento )
/******************************************************************************/
{
    //USE_DATABASE( AREA_AUX_MED, EVM_TMP, ( char* )&pagos, sizeof( struct _event_pago ), NULL, 0, 0 ) ;  
	OPEN_TABLE( T_EVM_TMP, TT_ORIG, ( char* )&pagos, sizeof( struct _ev_medios ) ) ;  
	//SELECT( AREA_AUX_MED );
	SELECT_TABLE( T_EVM_TMP, TT_ORIG );

        //SET_ORDER( 1 );
        //GET_EQUAL( ( char* )&id_evento );
		SET_WHERE("ID_EVENTO = '%ld'", id_evento);
		RUN_QUERY(NO);

	    return FOUND2();
}
