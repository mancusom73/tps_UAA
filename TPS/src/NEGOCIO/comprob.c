#pragma pack(1)
#include <tpv_dat.h>
#include <_dd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <global.h>
#include <tpv_dat.h>
#include <archivos.h> 
#include <aarch.h>
#include <btrv.h> 
#include <b_mens.h> 
#include <calcular.h> 
#include <cio.h> 
#include <config.h>
#include <clave.h> 
#include <clientes.h> 
#include <cmemory.h> 
#include <comprob.h> 
#include <cr.h>  
#include <cr_disco.h> 
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <eventos.h> 
#include <ext_tick.h> 
#include <facrem.h> 
#include <fiscal.h> 
#include <ftoc.h> 
#include <getch.h> 
#include <imp_item.h> 
#include <imp_tick.h> 
#include <lista.h> 
#include <log.h> 
#include <malloc.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <nume.h> 
#include <pagos.h> 
#include <pant.h> 
#include <pausa.h> 
#include <print.h> 
#include <promo.h> 
#include <round.h> 
#include <stack.h> 
#include <tarjetas.h> 
#include <tkt.h> 
#include <trim.h> 
#include <uti-comp.h> 
#include <valida.h> 
#include <_cr1.h> 
#ifdef INVEL_W
#include <io.h>
#else
#include <sys/io.h>
#endif
#include <db.h>
#include <deftables.h>
#include <inc_mes.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W) || defined(INVEL_L)
#include <actions.h>
#include <typeacti.h>
#endif
#define _ORIGINAL    1
#define _REIMPRESION 0
#ifdef DEBUG_IMPRESION	
	char AUX_LINEA[200];
	int AUX_COLUMNA = 0;
	int AUX_FILA = 0;
#endif
#pragma pack(1)
/*--------------------------------- Globales -----------------------------*/
extern struct _ConfTkt *CONFIG_FACTURA;
extern struct _ConfTkt *CONFIG_REMITO;
extern struct _ConfTkt *CONFIG_TKT1;
static struct _ConfTkt *CONFIG_COMPROBANTE1;
static char *ORDEN[] = {"ORIGINAL","DUPLICADO","TRIPLIC.","CUADRUP."};
/*Arrays usados para El Salvador*/
//contiene los tipos de iva respetando el orden, el indice indica el cod
/*char *TIPOS_IVA[]= {"RESP.INSC.","RESP.NO INSC.","NO RESP.","EXENTO",
                       "CONS.FINAL","MONOTRIBUT", "RESP.NO IDEN","LIBERADO","EXTERIOR",
                       "FACTURA DE COMPRA","DIPLOMATICO/EXPORTACION", "GUBERNAMENTAL"};*/
//Contiene los tipos de iva soportados por el Salvador (son los unicos que se muestran)
//char *TIPOS_IVA_MOSTRAR[6] = {"RESP.INSC.","EXENTO","CONS.FINAL","DIPLOMATICO/EXPORTACION","GUBERNAMENTAL",""};
/***************/
char *TIPO_COMP[] = {"FACTURA","NOTA CRED","REMITO","FAC REM","TKT", 
                                "TICKET","CREDITO FISCAL","FACT CONS FINAL"};

static char COMP_INDEP = 0, *P_CAD_CONT, REIMPRESION, CONFIRMAR_IMPRESION = 0;
static int CANT_RENGLONES[5],ind_config,LARGO_COMPROBANTE[5];
static INT16 fila = 0,columna = 0;
static int orden,ind_comienzo[5],ind_comienzo_cuerpo,FILA_CONT,COLUMNA_CONT;
static int renglon,cant_renglones = 1 , cant_pagos,_tipo,_hoja_actual;
static double nro_comp;
float porc_retenc_dgr,porc_percep_dgi, porc_percep_tacis;
static double precio,importe,cantidad,precio_final,percep_212_iva1,percep_212_iva2,cant_total,costo;
static double total,subtotal,exento,imp_int,iva1,iva2,neto,total_descuento;
static double imp[4];
static double total_impuestos[40],min_retenc_dgr,min_percep_dgi,retencion_dgr,percepcion_dgi,
              total_impuestos_factura;
static double percep_212_iva1_backup, percep_212_iva2_backup;
static double total_backup, subtotal_backup, exento_backup, imp_int_backup, iva1_backup;
static double iva2_backup, neto_backup, backup_totales = 0;
static double retencion_dgr_backup, percepcion_dgi_backup, total_impuestos_backup[20];
static double imp_monto_impuesto_back[4];
static char AGREGAR_IVA_NO_INSC, PEDIR_FORMULARIO_CON_TECLA = 1;
static char SI_FACTURA_A;
static char SI_FACTURA_B;
static char SI_NOTA_CREDITO;
static char SI_BIEN_DE_USO;
static char SI_EXENTO;
static char SI_GRAVADO;
static char HAY_GRAVADO;
static char HAY_EXENTO;
static int _COMP_FOR_I,_COMP_FOR_LIMITE,_COMP_FOR_IND_CONFIG;
static char *_SI_P_FORMULA = NULL;
char LARGO_CADENA,OFFSET_CADENA;
extern int nro_cuerpo;
extern int tipo_devolucion;
extern int printer_status_2;
extern struct _comprobante comp;
extern int GET_FECHA_DOS();
extern int NUMERO_CAJA();
static struct _art_fact
{
    long cod_articulo;
    char cod_barra[17];
    float cantidad;
    double precio;
    double neto_gravado;
    double imp_int;
    double exento;
    double iva1;
    double iva2;
    unsigned long pos;
    char cod_iva;
    char gravado;
    double costo;
    INT16 pos_ticket;
    double monto_impuesto[4];
    char cod_movimiento;
    int  origen;
} *art_fact;

struct _art_fact *art_desc;

static struct _art_fact_iva
{
    double neto_gravado;
    double iva1;
    double iva2;
}*art_fact_iva = NULL; 

static struct _art_fact_iva *art_fact_iva_backup = NULL;

int NRO_DOC_FISCAL_COMP;
static int fac_on = 0;
static long nro_ant = 0L;
//extern    HABILITAR_SLIP_FISCAL( int tipo_val, int mens, struct _valida *valida );
static int signo_nc;

char _nro_dato = 0, _impresion_ok = 1;

char imprimiendo_factura_journal = 0;

double diferencia_parcial[100]; //las diferencias entre totales en las promos
int  i_contador = 0;

int OBTENER_DESCRIPCION_COND_IVA( char *c_iva, char con_iva, int largo );

int DEBE_AGRUPAR_ARTICULOS_CON_DECIMALES( int h );
extern int GRABAR_LOG_COMPROBANTES( char *cadena );
extern int VALIDACION_DEL_COMPROBANTE( int tipo_comprobante,int tipo_evento_comp,int bloque ) ;
extern int MON( unsigned fecha );
extern int DIF_DIAS( unsigned fecha1, unsigned fecha2 );
long _secuencia_ant;
extern int recalcular_precio_neto_grupo;

char articulo_exento, articulo_gravado;
extern int tipo_t_ev_cont, tipo_t_ev_despro, tipo_t_pagos, tipo_t_eventos, tipo_t_ev_factu,  tipo_ev_dpago;
#define _INICIALIZA 0
#define _CABECERA  1
#define _CUERPO		 2
#define _PIE			3
double RETORNAR_TOTAL_PRINTER( double variacion );
int RETORNAR_TOTAL_PRINTER_POR_IVA( double variacion );
extern int TIPO_EVENTO( INT16 codigo, INT16 tipo_comprobante );
extern void RESOLVER_FUNCIONES( char *funciones, char *RtaConsulta, int linea );
extern void MUESTRA_PERFIL();
int TIPO_COMPROBANTE_DEPENDIENTE_DEL_CLIENTE(  int cond_iva_cli );
extern int OBTENER_COD_IVA_INICIAL( void );
extern void MODIFICAR_EN_E_TICKET( int h, int exento );
double RETORNAR_PRECIO_IMPRESORA( double cantidad, double precio_siva );
double RETORNAR_IMPORTES_IMPRESORA( double precio_final, double *iva_1 );
/****************************************************************************************************/
int IMPRIMIR_COMPROBANTE( int tipo, int reimpresion, int actualizo_ev ){
/****************************************************************************************************/
	int subtipo = 0, tipo_comprobante = 0, tipo_evento_comp = 0;
	int rta = 0;
	tipo_comprobante = TIPO_COMPROBANTE( &subtipo );
	//subtipo => 1-'a' : 2-'b';
	tipo_evento_comp = TIPO_EVENTO( NO, tipo_comprobante );

	// SI tiene todas configuradas
	if( (VALIDACION_DEL_COMPROBANTE(tipo_comprobante, tipo_evento_comp, 1/*_V_CABECERA*/) > 0 )&& 
		( VALIDACION_DEL_COMPROBANTE(tipo_comprobante, tipo_evento_comp, 2/*_V_CUERPO*/) > 0  )&&
		( VALIDACION_DEL_COMPROBANTE(tipo_comprobante, tipo_evento_comp, 3/*_V_PIE*/)> 0 )){
		rta = IMPRIMIR_COMPROBANTE_VAL( tipo, reimpresion/*, actualizo_ev*/ );
	}else{// SI no del fac-caja correspondiente
		rta = _IMPRIMIR_COMPROBANTE( tipo, reimpresion, actualizo_ev );
	}
	return rta;
}

/***************************************************************************/
int _IMPRIMIR_COMPROBANTE( int tipo, int reimpresion, int actualizo_ev )
/***************************************************************************/
{
    char init_acu;
    int j = 0, rta = 0, h, cant, _ptr, _handle,_estado;
    double _neto = 0.00, control_total = 0.00, tot_imp = 0.00;
    float _neto_percep_iva1 = 0.00, _neto_percep_iva2 = 0.00;
    int renglones_configuracion, i = 0;
    int subtipo, tipo_comprobante = 0, cond_IVA, reimpresion_tmp = 0;
    int reglon_backup = 0, k = 0, salir = 0, esperas = 30;
    double importe_sin_redondear = 0.00, importe_redondeado = 0.00, diferencia_redondeo=0.00, iva_sin_redondear = 0.00;

	memset(&comp,0,sizeof(struct _comprobante));

	OPEN_RECUPERO( &_ptr, &_handle );
    /*_rta = STACK_INS( sizeof( int ) );*/
    _estado = STACK_INS( sizeof( short ) );
   if( _ESTADO == 0 ) {

    fila = 1;
    columna = 1;
    _nro_dato = 0;
    /*------------------ Tipo de comprobante -------------------*/
    APUNTA_CFG( tipo );
	tipo_comprobante = TIPO_COMPROBANTE( &subtipo );
	cond_IVA = OBTENER_COND_IVA_CLIENTE( 0 );
	unlink("D_IMP_2.TMP");
    /*------------------ Inicializaciones ---------------*/
    REIMPRESION = reimpresion;

    if( !reimpresion ) {
        if( !VERIFICAR_EVENTOS_DEL_COMPROBANTE( tipo ) || !EXISTE_CLIENTE( RAM_NRO_CLIENTE ) ) {
            return ( 0 );
        }
		comp.tipo_comp = tipo_comprobante;
		comp.cond_cli = cond_IVA;
        comp.subtipo = subtipo;
        comp.nro_suc = event_ticket.suc_comprobante;
        comp.nro_comp = event_ticket.nro_comprobante_new;
    }

    if( CUERPOS == NULL ) {
        CUERPOS = calloc( MAX_CUERPOS, sizeof( struct _cuerpo ) );
        if( CUERPOS == NULL ) {
            MENSAJE_STRING( S_MEMORIA_INSUFICIENTE_PARA_EMITIR_COMPROBANTE );
            return ( 0 );
        }
    }
	
	if( !AGRUPAR_ARTICULOS( &cant_renglones, &art_fact, &art_fact_iva , &art_fact_iva_backup, &art_desc)){
	//if( !AGRUPAR_ARTICULOS( &j ) ) {
        MENSAJE_STRING( S_MEMORIA_INSUFICIENTE_PARA_EMITIR_COMPROBANTE );
        return ( 0 );
    }
	j = 0;
    comp.fecha = event_ticket.fecha;
    comp.hora = event_ticket.hora;
    comp.cant_formularios = CANTIDAD_FORMULARIOS( cant_renglones );
	cant = ( COMP_INDEP ) ? comp.cant_formularios : 1;
    SET_MEMORY_INT( __ram_cant_comp, cant );
    signo_nc = 1;
    rta = OBTENER_DATOS_COMPROBANTE( tipo, comp.subtipo, cant, &comp.nro_suc, &comp.nro_comp,
                                     &comp.copias, SI, NO );
	if( rta ) {
        if( IMPRESORA_FISCAL && ( tipo != _REMITO ) && !PUERTO_IMPRESORA_OPCIONAL_FACTUR ) {
            if( config.pais != ELSALVADOR ) {
                IMPRIMIR_TICKET_FACTURA( ( COMP_INDEP )
                    ? comp.cant_formularios : 1 );
            }
		if( IMPRESORA_FISCAL == FISCAL)//seba
			{

			ABRIR_NOTA_DE_CREDITO('P',RESP_INSC,cond_IVA,
					CLIENTE_AFINIDAD_NOMBRE, clientes.nombre, __CUIT,clientes.cuit,
					clientes.domicilio,clientes.localidad,clientes.provincia,
					"abc"/*clientes.empresa*/,"abc"/*clientes.telefono*/); //Seba
			}
		}

        if( PUERTO_IMPRESORA_OPCIONAL_FACTUR ) {
            PRN_HABILITAR_IMPRESORA_OPCIONAL();
        }
		for( orden = 0;orden < comp.copias; orden++ ) {
            init_acu = SI;
            nro_comp = comp.nro_comp;
            renglon = 0;
            nro_cuerpo = 0;
            subtotal_backup = 0.00;
   			exento_backup = 0.00;
   			imp_int_backup = 0.00;
   			iva1_backup = 0.00;
   			iva2_backup = 0.00;
   			for( i = 0;i < 4;i++ ) {
                imp_monto_impuesto_back[i] = 0;
            }
   			neto_backup = 0.00;
   			total_backup = 0.00;
   			retencion_dgr_backup = 0.00;
   			percepcion_dgi_backup = 0.00;
   			percep_212_iva1_backup = 0.00;
   			percep_212_iva2_backup = 0.00;
   			memset( total_impuestos_backup, 0, sizeof(total_impuestos_backup));
   			memset( art_fact_iva_backup, 0, sizeof( struct _art_fact_iva )
   			    * MAX_COD_IVA );

            /*----------------------- imprime formulario ----------------------*/
			for( j = 0;j < comp.cant_formularios;j++ ) {
                /*if( IMPRESORA_FISCAL || TIPO_IMPRESORA ) {
                    //Luciano recoradar no se lee TIPO_IMPRESORA RNV
                    #if defined(INVEL_W) || defined(INVEL_L)
                    int rta = 0;
                    /*getData( ACTION_SLP_EMPTY, ( char* )&rta, 0 );
                    while( !rta ) {
                        MENSAJE_TECLA( "RETIRE EL PAPEL DE LA IMPRESORA Y PRESIONE ENTER" );
                        getData( ACTION_SLP_EMPTY, ( char* )&rta, 0 );
                    }*/
                    //MENSAJE_CON_TECLA( ST( S_COLOQUE_FORMULARIO_EN_IMPRESORA_PRES_ENTER ), 13 );
                    
					/*rta = 1;
                    while( rta ) {
                        getData( ACTION_SLP_EMPTY, ( char* )&rta, 0 );
                    }*/
                    //#else

                    /*if( PEDIR_FORMULARIO_CON_TECLA ) {
                         MENSAJE_CON_TECLA( ST( S_COLOQUE_FORMULARIO_EN_IMPRESORA_PRES_ENTER ), 13 );
                    }
                    else {
                        MENSAJE_STRING( S_COLOQUE_EL_FORMULARIO_EN_LA_IMPRESORA );
                    }*/
                    
					//#endif
                //}

                _hoja_actual = j + 1;

                /* if( !PUERTO_IMPRESORA_OPCIONAL_FACTUR )  // Se comenta, para que la habilitacion del slip se por archivo de confg
				{
                    //PRN_HABILITA_SLIP( 1, NO );
                }*/

				if( COMP_INDEP || init_acu ) {
                    if( !backup_totales ) {
                        subtotal = 0.0;
                        exento = 0.0;
                        imp_int = 0.0;
                        iva1 = 0.0;
                        iva2 = 0.0;
                        neto = 0.0;
                        total = 0.0;
                        for( i = 0;i < 4;i++ ) {
                            imp[i] = 0.0;
                        }
                        retencion_dgr = 0.0;
                        percepcion_dgi = 0.0;

                        percep_212_iva1 = 0.0;
                        percep_212_iva2 = 0.0;

                        cant_total = 0.0;
                        costo = 0.0;

                        memset( total_impuestos, 0, sizeof( total_impuestos ) );
                        memset( art_fact_iva, 0, sizeof( struct _art_fact_iva ) * MAX_COD_IVA );
                    }
                    init_acu = NO;
                }

                /*----------- Imprimimos el encabezamiento ---------*/
                ind_config = ind_comienzo[_tipo] - 1;
                /*Se decrementa una posicion porque si bien entra correctamente dentro de 
                COMP_ejecutar_comando, en esta funcion ind_config se incrementa una posicion
                y no valida la primera variable*/
               while( CONFIG_COMPROBANTE1[ind_config].codigo != _COMIENZO_CUERPO ) {
                    COMP_ejecutar_comando( &ind_config );
                }
                //fila_reajuste = fila_reajuste - fila;
                /*---------------- Avanza hasta el inicio del cuerpo ------------*/
                if( tipo == _FAC_REM ) {
                    while( CONFIG_COMPROBANTE1[ind_config].codigo != _COMIENZO_CUERPO_FAC_REM ) {
                        ind_config++;
                    }
                }
                /*Se comenta porque si bien entra dentro de COMP_ejecutar_comando con la posicion 
                correcta luego en esta funcion, ind_config se incrementa una posicion y no valida 
                la primera variable*/
                //ind_config++;
                ind_comienzo_cuerpo = ind_config;

                /*----------------------- Cuerpo de la factura ---------------------*/
                h = 0;
                //art_descuento.precio = 0;
                //renglones_configuracion = ( DESCUENTOS_POR_CUERPO )? CANT_RENGLONES[_tipo] - 1 : CANT_RENGLONES[_tipo];
                if( tipo == _FACTURA )
					renglones_configuracion = CANT_RENGLONES[_FACTURA];
				if( tipo == _NOTA_CREDITO )
					renglones_configuracion = CANT_RENGLONES[_NOTA_CREDITO];
                reglon_backup = renglon;

				while( renglon < cant_renglones && h < renglones_configuracion ) {
                    if( fabs( art_fact[renglon].cantidad ) > 0.001 ) {
                        /*----------------- Resta descuento  ----------------------------*/
                        if( DESCUENTOS_POR_CUERPO ) {
                            CALCULAR_VALOR_CADA_ARTICULO_RESTANDO_DESCUENTOS( renglon );
                        }

                        /*----------------- Barro el art_fact  ----------------------------*/
                        if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
                            BUSCA_COD_INTERNO_COD_BARRA( art_fact[renglon].cod_articulo,
                                                         art_fact[renglon].cod_barra );
                        } else {
                            BUSCA_CODIGO_ESPECIAL( art_fact[renglon].cod_articulo );
                        }
                        //SELECT( AREA_E_TICKET );
                        SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
                        //GET_DIRECT( art_fact[renglon].pos );
                        GET_DIRECT_REG( art_fact[renglon].pos, 0 );

                        cantidad = art_fact[renglon].cantidad;
                        precio = art_fact[renglon].precio;
                        //precio_t =
                        //  art_fact[renglon].neto_gravado + art_fact[renglon].exento +
                        //  art_fact[renglon].imp_int + art_fact[renglon].iva1 +
                        //  art_fact[renglon].iva2 + art_fact[renglon].monto_impuesto[0] +
                        //  art_fact[renglon].monto_impuesto[1] + art_fact[renglon].monto_impuesto[2]
                        //  + art_fact[renglon].monto_impuesto[3];
                        //fabiana 15-02-05 ** nuevo manejo de redondeos. Facturacion especial queda deshabilitado   
                        //if( !MODO_FACTURACION_ESPECIAL(  ) ) {
                        //  precio_t = ROUND( precio_t, ENTEROS, DECIMALES );
                        //}

                        //precio_final =
                        //  ROUND( art_fact[renglon].neto_gravado + art_fact[renglon].exento +
                        //  art_fact[renglon].imp_int + art_fact[renglon].iva1 +
                        //  art_fact[renglon].iva2 + art_fact[renglon].ila +
                        //  art_fact[renglon].iha, ENTEROS, DECIMALES );

						if( ES_FACTURA_A( comp.cond_cli ) && !FACTURA_CON_PRECIO_FINAL ) {
                                for( i = 0;i < 4;i++ ) {
                                    tot_imp = art_fact[renglon].monto_impuesto[i];
                                }
                                precio_final =
                                        ROUND( art_fact[renglon].neto_gravado
                                        + art_fact[renglon].exento
                                        + art_fact[renglon].imp_int, ENTEROS, DECIMALES )
                                        + ROUND( art_fact[renglon].iva1
                                        + art_fact[renglon].iva2 + tot_imp, ENTEROS, DECIMALES );
						} else {
                                precio_final =
                                        ROUND( art_fact[renglon].neto_gravado
                                        + art_fact[renglon].exento
                                        + art_fact[renglon].imp_int + art_fact[renglon].iva1
                                        + art_fact[renglon].iva2, ENTEROS, DECIMALES );
                        }
						if( ES_FACTURA_A( comp.cond_cli ) && !FACTURA_CON_PRECIO_FINAL ) {
                            importe =
                                    art_fact[renglon].neto_gravado
                                    + art_fact[renglon].exento
                                    + art_fact[renglon].imp_int;
							iva_sin_redondear = art_fact[renglon].iva1;

                            #ifdef DEBUG_R                          
                            if( MODO_DEBUG == _DEBUG_REDONDEO ) {
                                memset( cadena_debug, 0, strlen( cadena_debug ) );
                                _snprintf( cadena_debug, 80, "14.1.Comprob, art=%li,pr=%lf,cant=%f,imp=%lf",
                                         art_fact[renglon].cod_articulo, art_fact[renglon].precio,
                                         art_fact[renglon].cantidad, importe );
                                cadena_debug[79] = '\0';
                                //GRABAR_LOG_SISTEMA( cadena_debug );
                                memset( cadena_debug, 0, strlen( cadena_debug ) );
                                _snprintf( cadena_debug, 80, "14.2.Comprob, %lf, pr=%lf,iva=%lf,imp=%lf, dif=%lf",
                                         precio_final, art_fact[renglon].neto_gravado,
                                         art_fact[renglon].iva1,
                                         ROUND( importe, ENTEROS, DECIMALES ),
                                         ROUND( importe, ENTEROS, DECIMALES ) - importe );
                                cadena_debug[79] = '\0';
                                
                                GRABAR_LOG_SISTEMA( cadena_debug );
                            }                       
                            #endif
							//Este IF genera errores de redondeo de 1 centavo 
                            /*if(!reimpresion_tmp){
						        importe_sin_redondear = importe;
                                importe = ROUND( importe, ENTEROS, DECIMALES );
							    importe_redondeado = importe;
							    diferencia_redondeo = ( fabs( importe_sin_redondear ) ) - ( fabs( importe_redondeado ) );
							    //if(RAM_NOTA_CR)
								//    diferencia_redondeo *= -1;
							    if(art_fact[renglon].neto_gravado != 0) // articulo gravado
								    ADD_MEMORY_DOUBLE( __ram_diferencia_redondeo_grav, diferencia_redondeo );
							    if(art_fact[renglon].exento != 0) // articulo exento
								    ADD_MEMORY_DOUBLE( __ram_diferencia_redondeo_exent, diferencia_redondeo );
    							
							    diferencia_redondeo = ( fabs( iva_sin_redondear ) ) - ( fabs( ROUND( iva_sin_redondear, ENTEROS, DECIMALES ) ) );
							    ADD_MEMORY_DOUBLE( __ram_diferencia_redondeo_iva, diferencia_redondeo );
                            }*/

						} else {
                            importe = precio_final;

                            #ifdef DEBUG_R                          
                            if( MODO_DEBUG == _DEBUG_REDONDEO ) {
                                memset( cadena_debug, 0, strlen( cadena_debug ) );
                                sprintf( cadena_debug,
                                         "14.3.Comprob, art=%li,pr=%lf,cant=%f,imp=%lf",
                                         art_fact[renglon].cod_articulo, art_fact[renglon].precio,
                                         art_fact[renglon].cantidad, importe );
                                //GRABAR_LOG_SISTEMA( cadena_debug );
                                memset( cadena_debug, 0, strlen( cadena_debug ) );
                                sprintf( cadena_debug, "14.4.Comprob, pr=%lf,imp=%lf, dif=%lf",
                                         art_fact[renglon].neto_gravado,
                                         ROUND( importe, ENTEROS, DECIMALES ),
                                         ROUND( importe, ENTEROS, DECIMALES ) - importe );
                                GRABAR_LOG_SISTEMA( cadena_debug );
                            }                       
                            #endif                          
                        }
                        /*------------------------- Acumula ----------------------------*/
                        cant_total += cantidad;
                        subtotal += importe;
                        exento += art_fact[renglon].exento;
                        costo += art_fact[renglon].costo;
                        imp_int += art_fact[renglon].imp_int;
                        iva1 += art_fact[renglon].iva1;
                        iva2 += art_fact[renglon].iva2;
                        for( i = 0;i < 4;i++ ) {
                            imp[i] += art_fact[renglon].monto_impuesto[i];
                        }
                        neto += art_fact[renglon].neto_gravado;
                        //total += precio_t;
                        total += precio_final;

						if( art_fact[renglon].cod_iva < MAX_COD_IVA ) {
                            art_fact_iva[art_fact[renglon].cod_iva].neto_gravado += 
                                art_fact[renglon].neto_gravado;
                            art_fact_iva[art_fact[renglon].cod_iva].iva1 +=
                                art_fact[renglon].iva1;
                            art_fact_iva[art_fact[renglon].cod_iva].iva2 +=
                                art_fact[renglon].iva2;
                        } else {
                            MENSAJE_STRING( S_COD_IVA_INVALIDO );
                        }

                        /*------------ Acumula ILA e IHA ------------*/
                        if( event_e_ticket.cod_ext ) {
                                CARGA_TOTAL_IMPUESTOS( total_impuestos, event_e_ticket.cod_ext );
                        } else {
                            for( i = 0;i < 4;i++ ) {
                                if( articulo.cod_impuesto[i] ) {
                                    total_impuestos[articulo.cod_impuesto[i] - 1] += 
                                    art_fact[renglon].monto_impuesto[i];
                                }
                            }
                        }
                        //recalculamos
                        if( art_fact[renglon].origen == 4
                           && (( RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL && RAM_NOTA_CR == 1 )
                                 || ( MODO_DEVOLUCION && RAM_MODO_DEVOLUCION == _DEVOL_PARCIAL ))) {
							            if( fabs( art_fact[renglon].cantidad ) >= 0.01){
								              precio = importe; ;
								              art_fact[renglon].cantidad = 1;
                                      cantidad = art_fact[renglon].cantidad;
						         }
                        }

						/*-----------------Se deben imprimir positivos ------------*/
                        if( DESCUENTOS_POR_CUERPO
                         && ( config.pais == CHILE || config.pais == BRASIL )
                         && ( RAM_NOTA_CR || RAM_REFERENCIA_NC || RAM_MODO_DEVOLUCION ) ) {
                            signo_nc = -1;
                        }
                        
						/*-------------------------- Cabecera ---------------------*/
                        ind_config = ind_comienzo_cuerpo;
                        if( tipo != _FAC_REM ) {
                            while( CONFIG_COMPROBANTE1[ind_config].codigo != _FIN_CUERPO
                                && CONFIG_COMPROBANTE1[ind_config].codigo
                                != _COMIENZO_CUERPO_FAC_REM
                                && CONFIG_COMPROBANTE1[ind_config].codigo != _FIN ) {
                                COMP_ejecutar_comando( &ind_config );
                            }
                        } else {
                            while( CONFIG_COMPROBANTE1[ind_config].codigo != _FIN_CUERPO
                                && CONFIG_COMPROBANTE1[ind_config].codigo != _FIN ) {
                                COMP_ejecutar_comando( &ind_config );
                            }
                        }
                        COMP_SET_FILA( fila + 1 );
                        h++;
                    }
                    renglon++;
                }

                while( CONFIG_COMPROBANTE1[ind_config].codigo != _FIN_CUERPO
                    && CONFIG_COMPROBANTE1[ind_config].codigo != _FIN ) {
                    ind_config++;
                }
                
				/*------------------------- Imprime totales ------------------------*/
				if( j == comp.cant_formularios - 1 || COMP_INDEP ) {
                    /*----- Calculamos retenciones --------*/
                    if( COMP_INDEP ) {
                        _neto += neto;
                    }
                    
					if( j == comp.cant_formularios - 1 ) {
                        if( !COMP_INDEP ) {
                            _neto = neto;
                        }
                        if( CALCULAR_RETENCION_DGR 
								&& REALIZAR_RETENCION_DGR(  )
                                && !RAM_BIEN_DE_USO
                                && ( comp.tipo_comp == _FACTURA 
                                    || comp.tipo_comp == _NOTA_CREDITO )
                                //Salvador aplica la retencion a clientes Gubernamentales tanto
                                //al comprobante A como al B. 
                                && ( comp.subtipo == TIPO_A 
                                    || clientes.cond_iva == INST_GUBERNAMENTAL )
                                && fabs( _neto ) >= min_retenc_dgr
                                && ( clientes.exento_ing_brutos == 'N'
                                    || clientes.exento_ing_brutos == 'n' ) ) {
                            retencion_dgr =
                                ROUND( _neto * porc_retenc_dgr / 100, 10, DECIMALES );
                            total += retencion_dgr;
						}
                        /*Percepcion DGI- Control para cliente dgi = 'N' -----*/
                        /*-----Modalidad bien de Uso -- No calcula Percepcion */
                        if( CALCULAR_PERCEPCION_DGI && REALIZAR_PERCEPCION_DGI(  )&& !RAM_BIEN_DE_USO
                                && ( comp.tipo_comp == _FACTURA || comp.tipo_comp == _NOTA_CREDITO )
                                && comp.subtipo == TIPO_A 
                                && fabs( _neto ) >= min_percep_dgi
                                && clientes.realizar_percepcion_dgi != 'N' ) {
                                percepcion_dgi =
                                ROUND( _neto * porc_percep_dgi / 100, 10, DECIMALES );
                                total += percepcion_dgi;
                        }
                        //---Actualizacion monotributo
                        _neto_percep_iva1 =
                            ( float )( art_fact_iva[0].neto_gravado + art_fact_iva[0].iva1 )
                            * ( ( iva[0].porc_iva1 / 2 ) / 100 );
                        _neto_percep_iva2 =
                            ( float )( art_fact_iva[1].neto_gravado + art_fact_iva[1].iva1 )
                            * ( ( iva[1].porc_iva1 / 2 ) / 100 );
                        if( comp.cond_cli == RESP_NO_IDEN && comp.tipo_comp == _FACTURA ) {
                            percep_212_iva1 = ROUND( _neto_percep_iva1, 10, DECIMALES );
                            percep_212_iva2 = ROUND( _neto_percep_iva2, 10, DECIMALES );
                            total += ( percep_212_iva1 + percep_212_iva2 );
                        }
                            total = ROUND( total, 10, DECIMALES );
                    }

                    if( config.pais == CHILE /*&& comp.cant_formularios > 1*/
                            || config.pais == BRASIL ) {
                        control_total = total - ROUND( neto, ENTEROS, DECIMALES );
                        for( i = 0 ;i < 20 ;i++ ) {
                            control_total -= ROUND( total_impuestos[i], ENTEROS, DECIMALES );
                        }
                        iva1 = control_total;
                        CUERPOS[nro_cuerpo].nro_evento = event_ticket.nro_evento;
                        CUERPOS[nro_cuerpo].nro_sucursal = comp.nro_suc;
                        CUERPOS[nro_cuerpo].nro_comprobante = nro_comp;
                        CUERPOS[nro_cuerpo].exento = exento;
                        CUERPOS[nro_cuerpo].imp_int = imp_int;
                        CUERPOS[nro_cuerpo].iva1 = iva1;
                        CUERPOS[nro_cuerpo].iva2 = iva2;
                        for( i = 0;i < 0;i++ ) {
                            CUERPOS[nro_cuerpo].monto_impuesto[i] = imp[i];
                        }
                        CUERPOS[nro_cuerpo].retencion_dgr = retencion_dgr;
                        CUERPOS[nro_cuerpo].percepcion_dgi = percepcion_dgi;
                        CUERPOS[nro_cuerpo].neto = neto;
                        CUERPOS[nro_cuerpo].subtotal = subtotal;
                        CUERPOS[nro_cuerpo].total = total;
                        CUERPOS[nro_cuerpo].total_descuento = total_descuento;
                        CUERPOS[nro_cuerpo].bultos = cant_total;
                        CUERPOS[nro_cuerpo].costo = costo;
                        CUERPOS[nro_cuerpo].tipo = comp.subtipo;
                        CUERPOS[nro_cuerpo].nro_orden++;
                        for( i = 0 ;i < 20 ;i++ ) {
                            CUERPOS[nro_cuerpo].total_impuestos[i] = total_impuestos[i];
                        }
                        nro_cuerpo++;
                    }
                    //fila = 1;
					//columna = 1;
                    //LF(1);
                    while( CONFIG_COMPROBANTE1[ind_config].codigo != _FIN_CUERPO ) {
                        COMP_ejecutar_comando( &ind_config );
                    }

                    if( tipo == _FACTURA ) {
                        while( CONFIG_COMPROBANTE1[ind_config].codigo != _COMIENZO_CUERPO_FAC_REM
                            && CONFIG_COMPROBANTE1[ind_config].codigo != _FIN_CUERPO ) {
                            COMP_ejecutar_comando( &ind_config );
                        }

                        if( CONFIG_COMPROBANTE1[ind_config].codigo == _COMIENZO_CUERPO_FAC_REM ) {
                            while( CONFIG_COMPROBANTE1[ind_config].codigo != _FIN_CUERPO ) {
                                ind_config++;
                            }
                        }
                    }

                    /*if( fila_reajuste > 0 )
                        fila += fila_reajuste;*/

                    while( CONFIG_COMPROBANTE1[ind_config].codigo != _FIN ) {
                        COMP_ejecutar_comando( &ind_config );
                    }

				} else {
                    COMP_SET_FILA( FILA_CONT );
                    columna = COLUMNA_CONT;
                    MOSTRAR_CADENA( columna, PRN, P_CAD_CONT );
					//Expulso el papel
						LF( 1 );
						setData( ACTION_PRINT, NULL, 0 );
						EJECT();
						LF( 1 );
						setData( ACTION_PRINT, NULL, 0 );
						PRN_INHABILITA_SLIP();
					
					fila = 1;
					columna = 1;
			    }

                if( COMP_INDEP ) {
                    nro_comp++;
                }

                if( HARD_OMRON && !PUERTO_IMPRESORA_OPCIONAL_FACTUR ) {
                    #ifdef COMPILAR_OMRON
                    PRN_INHABILITA_SLIP();
                    OMRON_ESPERAR_QUE_SAQUEN_LA_HOJA();
                    #endif
                }
                /*else {
                    if( config.pais != ELSALVADOR || TIPO_IMPRESORA != IMPRESORA_DRIVER ) {
                        EJECT();
                    }
                }*/
                SET_COLUMNA( 1 );
				COMP_SET_FILA( 1 ); // seba
                
                #if defined( INVEL_W ) || defined( INVEL_L )
                LF( 0 );
                setData( ACTION_PRINT, NULL, 0 );
                #endif
                /*if( PEDIR_FORMULARIO_CON_TECLA ) {
                    MENSAJE( ST( S_COLOQUE_FORMULARIO_EN_IMPRESORA_PRES_ENTER ) );
                    while( FAST_GETCH() != 13 ) {
                        SLEEP( 50 );
                    }
                }*/
                //Confirmamos solo para asistir al usuario, ya que en realidad
                //la impresion ya fue conmpletamente enviada.
                ESPERAR_IMPRESION_SLIP_DRIVER( 'f' );
                if( CONFIRMAR_IMPRESION ) {
                    salir = 0;
                    MENSAJE( ST( S_SE_IMPRIMIO_CORRECTAMENTE_COMPROBANTE_ENTER_ESC ) );
                    while( !salir ) {
                        k = GETCH(  );
                        while( k != 13 && k != 27 ) {
                            k = GETCH();
                        }
                        if( k == 27 || k == 13 ) {
                            salir = 1;
                        }
						if( k == 27 ) {
							fila = 1;
							columna = 1;
                            reimpresion_tmp++;
                            setData( ACTION_CLEAR_PRINTER, ( char* )&rta, 0 );
                            AGREGA_BIN_A_EXTENDIDO( _hoja_actual, D_VALIDACIONES, SI );
							unlink("D_IMP_2.TMP");
                            /* agrego esto para que reinicie los contadores del
                            comprobante cuando se imprime mal.*/
                            if( j == comp.cant_formularios - 1 || COMP_INDEP )
                                init_acu = SI;
                            j--;
                            renglon = reglon_backup;
                            if( !COMP_INDEP ) {
                                subtotal = subtotal_backup;
                                exento = exento_backup;
                                imp_int = imp_int_backup;
                                iva1 = iva1_backup;
                                iva2 = iva2_backup;
                                for( i = 0;i < 4;i++ ) {
                                    imp[i] += imp_monto_impuesto_back[i];
                                }
                                neto = neto_backup;
                                total = total_backup;
                                retencion_dgr = retencion_dgr_backup;
                                percepcion_dgi = percepcion_dgi_backup;
                                percep_212_iva1 = percep_212_iva1_backup;
                                percep_212_iva2 = percep_212_iva2_backup;
                                memcpy( art_fact_iva, art_fact_iva_backup,
                                        sizeof( struct _art_fact_iva ) );
                                memcpy( &total_impuestos, total_impuestos_backup,
                                        sizeof( double ) * 20 );
                                backup_totales = 1;
                                }
                        } else {
                            //poner funcion nueva que pasa de bin a bin_extendido
                            AGREGA_BIN_A_EXTENDIDO(_hoja_actual,D_VALIDACIONES,NO);
                            if( !COMP_INDEP ) {
                                subtotal_backup = subtotal;
                                exento_backup = exento;
                                imp_int_backup = imp_int;
                                iva1_backup = iva1;
                                iva2_backup = iva2;
                                for( i = 0;i < 4;i++ ) {
                                    imp[i] += imp_monto_impuesto_back[i];
                                }
                                neto_backup = neto;
                                total_backup = total;
                                retencion_dgr_backup = retencion_dgr;
                                percepcion_dgi_backup = percepcion_dgi;
                                percep_212_iva1_backup = percep_212_iva1;
                                percep_212_iva2_backup = percep_212_iva2;
                                memcpy( art_fact_iva_backup, art_fact_iva,
                                sizeof( struct _art_fact_iva ) );
                                memcpy( &total_impuestos_backup, total_impuestos,
                                sizeof( double ) * 20 );
                                backup_totales = 0;
                            }
                        }
                    }
                    BORRAR_MENSAJE( );
                }
            }
        }
        /*if( !Z_NRO_PRIMER_TICKET ) {
            SET_MEMORY_LONG( __z_nro_primer_ticket, RAM_NRO_COMP );
        }*/

        /*----------------- actualizo factu.mem ---------*/
        cant = nro_comp - comp.nro_comp - 1;
        OBTENER_DATOS_COMPROBANTE( tipo, comp.subtipo, cant, &comp.nro_suc, &comp.nro_comp,
                                   &comp.copias, SI, NO );
        /*--------------- Vuelve ---------------*/
        if( PUERTO_IMPRESORA_OPCIONAL_FACTUR ) {
            PRN_DESHABILITAR_IMPRESORA_OP();
            //PRN_INHABILITA_SLIP(  );
        } else if( !HARD_OMRON ) {
            if( IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
                if( comp.tipo_comp == _REMITO ) {
                    CERRAR_DOCUMENTO( 0, NO, 0 );
                }
                else {
                    CERRAR_DOCUMENTO( total, NO, 0 );
                }
            }
            PRN_INHABILITA_SLIP();
        }
        if( art_fact ) {
            FREEMEM( ( void* )art_fact );
        }
        if( art_fact_iva ) {
            FREEMEM( ( void* )art_fact_iva );
        }
        if( art_fact_iva_backup ) {
            FREEMEM( ( void * ) art_fact_iva_backup );
        }
        if( art_desc ) {
            FREEMEM( ( void* )art_desc );
        }
    } else {
        MENSAJE_STRING( S_ERROR_LEER_DATOS_PARA_EMITIR_COMPROBANTE );
    }

    SET_COLUMNA( 1 );
	_SET_ESTADO( 1 );
   }

   if( _ESTADO == 1 ) {
   //GRABO A DPAGO
      AGREGAR_EXT_BTRV( actualizo_ev );
      _SET_ESTADO( 2 );
   }
   if( _ESTADO == 2 ) {
      unlink("D_IMP_2.TMP");
      _SET_ESTADO( 3 );
   }
   if( _ESTADO == 3 ) {
      if( config.pais == ELSALVADOR ) {
         IMPRIMIR_COMPROBANTE_JOURNAL( tipo, reimpresion );
      }
   }
   CLOSE_RECUPERO( _ptr, _handle );
    
   return ( rta );
}

/**
/* Se muestran mensaje de acuerdo al estado del JavaServer
/* Se conforma de 5 estados:
/* estado 0 ( todavia se recibio/ejecuto el comando de solicitar slip )
/* estado 204 ( ingrese papel )
/* estado 0 , finish 0 ( imprimiendo )
/* estado 0 , finish 1 ( finalizando impresion y eject del papel )
/* estado 112 ( retire papel )
/*****************************************************************************/
void ESPERAR_IMPRESION_SLIP_DRIVER( char tipo )
/*****************************************************************************/
{
    int resPrinter;
    int contador;
    int salir = 0;

    if( TIPO_IMPRESORA == IMPRESORA_DRIVER ) {
        //Esperamos q se solicite papel (Max 3 segundos)
        contador = 30;
        while( !salir && contador > 0 ) {
		    getData( ACTION_READ_PRINTER_STATUS, ( char* )&resPrinter, 0 );
            if( resPrinter != 0 ) {
                salir = 1;
            }
            SLEEP( 100 );
            contador--;
	    }
        //Cuando se detecta papel se sale de este while
        salir = 0;
        while( !salir ) {
		    getData( ACTION_READ_PRINTER_STATUS, ( char* )&resPrinter, 0 );
            if( resPrinter != 204 ) {
                salir = 1;
            } else {
                if( tipo == 'f' ) {
                    MENSAJE_SIN_PAUSA( ST( S_COLOQUE_EL_FORMULARIO_EN_LA_IMPRESORA ) );
                } else if ( tipo == 'c' ) {
                    MENSAJE_SIN_PAUSA( "COLOQUE EL COMPROBANTE EN LA IMPRESORA" );   
                } else {
                    MENSAJE_SIN_PAUSA( ST( S_COLOQUE_PAPEL_EN_SLIP ) );
                }
                SLEEP( 50 );
            }
	    }
        BORRAR_MENSAJE();
        //Por si no se inserto papel apropiadamente
        CONTROLAR_ESTADO_IMPRESORA();
        //Ahora esperamos la impresion al menos unos 6 segundos
		//ATENCION EN LA CONDICION DEL WHILE SIGUIENTE NO PUEDE USARSE UN || YA QUE NO SALDRIA SINO HAY RESPUESTA DE LA IMPRESORA!!!
		//si se desea esto buscar otra solucion para que no quede la version en un bucle al no usar impresora.
        salir = 0;
        contador = 30;
        while( !salir 
#ifdef COMPILAR_SIN_USAR_IMPRESORA
			||
#else
			&&
#endif
			contador > 0 ) {
            getData( ACTION_PRINT_FINISH, (char *) &resPrinter, 0);
            if( resPrinter != 0 ) {
                salir = 1;
            } else {
                CONTROLAR_ESTADO_IMPRESORA();
                //MOSTRAR_MENSAJE2( "ESPERE...", 100, NO );
                SLEEP( 200 );
                //BORRAR_MENSAJE_(NO);
            }
            contador--;
	    }
        //Ahora queremos esperar a que retire el papel.
        //Esperamos (Max 3 segundos) de status 0, ya que durante la impresion y
        //instantes despues de la misma, no hay errores ni indicios de que esta ocupada
        salir = 0;
        contador = 30;
        while( !salir && contador > 0 ) {
		    getData( ACTION_READ_PRINTER_STATUS, ( char* )&resPrinter, 0 );
            if( resPrinter != 0 ) {
                salir = 1;
            }
            SLEEP( 100 );
            contador--;
	    }
        salir = 0;
        //Antes de salir esperamos a q se retire el papel
        while( !salir ) {
		    getData( ACTION_READ_PRINTER_STATUS, ( char* )&resPrinter, 0 );
            if( resPrinter != 112 ) {
                salir = 1;
            } else {
                CONTROLAR_ESTADO_IMPRESORA();
                //MENSAJE_SIN_PAUSA( ST( S_POR_FAVOR_RETIRE_COMPROB_DEL_SLIP ) );
                SLEEP( 100 );
            }
	    }
    }
}

/*****************************************************************************/
int AGRUPAR_ARTICULOS( int* cantidad_de_items, struct _art_fact **art_fact2, struct _art_fact_iva **art_fact_iva2 , struct _art_fact_iva ** art_fact_iva_backup2,struct _art_fact **art_desc2)
/*****************************************************************************/
{
    char barra1[17],barra2[17];
    int rta = 1, renglon, i, encontro, p_art = 0, continua = 1, h, _signo, j;
    double recargo_neto,precio = 0,signo;
    int /*area_ant*/ tabla_ant, tipo_ant;
    INT16 pos_ticket;
    int k;
	double dif_aceptada = 0.0;

    /*struct    _ind_ev
    {
        long id_evento;
        int posicion;
        char tipo_motivo;
        int id_motivo;
    } ind_ev;*/

    //area_ant = SELECTED();
    tabla_ant = SELECTED2();
    tipo_ant = SELECTED_TIPO();
    
    *art_fact2 = calloc( *cantidad_de_items, sizeof( struct _art_fact ) );
    *art_fact_iva2 = calloc( MAX_COD_IVA, sizeof( struct _art_fact_iva ) );
    *art_fact_iva_backup2 = MALLOC( sizeof( struct _art_fact_iva ) * MAX_COD_IVA );
    
    
    if( DESCUENTOS_POR_CUERPO ) {
       *art_desc2 = calloc( *cantidad_de_items, sizeof( struct _art_fact ) );
    }
	
    
    if( !art_fact2 || ( DESCUENTOS_POR_CUERPO && !art_desc2 ) ) {
        rta = 0;
    }
    if( rta ) {
        memset( *art_fact2, 0, sizeof( struct _art_fact ) * *cantidad_de_items );
        if( DESCUENTOS_POR_CUERPO ) {
            memset( *art_desc2, 0, sizeof( struct _art_fact ) * *cantidad_de_items );
        }
        pos_ticket = 0;
        //SELECT( AREA_E_TICKET );
        //GET_EQUAL( ( char* )&event_ticket.id_evento );
		SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
		SET_WHERE("ID_EVENTO = '%ld'",event_ticket.id_evento);
		SET_ORDER2("ID_EVENTO");
		RUN_QUERY(NO);
        for( renglon = 0;renglon < *cantidad_de_items;renglon++ ) {
            if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
                BUSCA_COD_INTERNO_COD_BARRA( event_e_ticket.cod_articulo, event_e_ticket.cod_barra );
            }
            else {
                BUSCA_CODIGO_ESPECIAL( event_e_ticket.cod_articulo );
            }
            encontro = 0;
            continua = 1;
            /*-----Se descartan los archivos originados por promocion-----*/
            if( DESCUENTOS_POR_CUERPO ) {
                /* --- en las notas de credito si se imprimen las promociones --- */
                if( ( event_e_ticket.origen == _ORIGEN_PROMOCION && !RAM_NOTA_CR )
                      || event_e_ticket.origen == _ORIGEN_BOLETIN_D_ADIC ) {
                    continua = 0;
                    if( event_e_ticket.origen == _ORIGEN_PROMOCION && !RAM_NOTA_CR ) {
                        pos_ticket--;
                    }
                }
            }

            if( continua ) {
                i = 0;
                h = pos_ticket;
                strncpy( barra1, _TICKET_COD_BARRA( h ), sizeof( barra1 ) );
                strncpy( barra2, _TICKET_COD_BARRA_ORIGINAL( h ), sizeof( barra2 ) );
                _signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
                if( ( ( _TICKET_TIPO2( h ) & _PEDIR_PRECIO )
                   ||/* ( fabs( fmod( _TICKET_CANTIDAD( h ), 1.00 ) ) > 0.0005 ) */
					!DEBE_AGRUPAR_ARTICULOS_CON_DECIMALES( h )
                   || strcmp( barra1, barra2 ) ) && _signo > 0 ) {
                    i = p_art;
                }
                while( i < p_art && !encontro ) {
                    /*--------------------- Calculo precio ------------------------*/
                    if( ES_FACTURA_A( comp.cond_cli ) && !FACTURA_CON_PRECIO_FINAL ) {
                        precio = event_e_ticket.importe_sin_iva + event_e_ticket.exento
                               + event_e_ticket.recargo_tarjeta;
                    } else {
                        precio = event_e_ticket.importe_sin_iva + event_e_ticket.exento
                               + event_e_ticket.iva1 + event_e_ticket.recargo_tarjeta;
                        for( k = 0;k <= 3;k++ ) {
                            precio += event_e_ticket.monto_impuesto[k];
                        }
                    }

                    /*------------------- Busco el artculo -----------------------*/
					dif_aceptada = ( !recalcular_precio_neto_grupo )? 0.01: 0.001;

					if( event_e_ticket.cod_articulo == (*art_fact2)[i].cod_articulo//ddd
                            && ( fabs( fabs( (*art_fact2)[i].precio ) - fabs( precio ) ) < dif_aceptada )
                     && ( strcmp( event_e_ticket.cod_barra, (*art_fact2)[i].cod_barra ) == 0
                       || BUSQUEDA_DE_ARTICULOS != BUSQUEDA_COD_INT_Y_COD_BARRA ) ) {
                        encontro = 1;
                        if( ( ( _TICKET_TIPO( h ) & _DEV_ENVASE )
                           && !( _TICKET_TIPO( (*art_fact2)[i].pos_ticket ) & _DEV_ENVASE ) ) ) {
                            encontro = 0;

                            i++;
                        }
                        if( encontro && RAM_CAMBIO && (*art_fact2)[i].cod_movimiento != _TICKET_COD_MOVIMIENTO( h ) ){
                            encontro = 0;
                             i++;
                        }
					   if( encontro && (RAM_MODO_DEVOLUCION || RAM_NOTA_CR ) && event_e_ticket.origen == 4
						   && (( fabs( fabs( (*art_fact2)[i].precio ) - fabs( precio ) ) < 0.001 )
						        && (*art_fact2)[i].origen != 4 )){
							   //caso que el %descuento de igual que el porcentaje de cantidad
							   //ej 10% de descuento vs 10 articulos iguales...en ese caso el importe del
							   //descuento coincide con el importe del articulo y por eso lo agrupaba.
                            encontro = 0;
                             i++;
                        }
                        
                    }
                    else {
                        i++;
                    }
                }
                /*--------------------- Actualizo importes -----------------------*/
                recargo_neto = event_e_ticket.recargo_tarjeta;
                cantidad = fabs( event_e_ticket.cantidad );
                (*art_fact2)[i].cantidad += event_e_ticket.cantidad;
                (*art_fact2)[i].neto_gravado += ( event_e_ticket.importe_sin_iva
                                            - event_e_ticket.imp_int ) * cantidad;
                (*art_fact2)[i].imp_int += event_e_ticket.imp_int * cantidad;
                (*art_fact2)[i].exento += event_e_ticket.exento * cantidad;
                (*art_fact2)[i].iva1 += event_e_ticket.iva1 * cantidad;
                (*art_fact2)[i].iva2 += event_e_ticket.iva2 * cantidad;
                (*art_fact2)[i].origen = event_e_ticket.origen;
                for( j = 0;j < 4;j++ ) {
                   (*art_fact2)[i].monto_impuesto[j] += event_e_ticket.monto_impuesto[j] * cantidad;
                }

                if( articulo.gravado == 'S' || EXENTO_COMO_GRAVADO( OBTENER_COD_GRAVADO( ))
                        || fabs( event_e_ticket.exento ) < 0.001 ) {
                    (*art_fact2)[i].neto_gravado += recargo_neto * cantidad;
                }
                else {
                    (*art_fact2)[i].exento += recargo_neto * cantidad;
                }
                if( !encontro ) {
                    /*----------------- Copio datos fijos -------------------------*/
                    //SELECT( AREA_E_TICKET );
					SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
                    (*art_fact2)[i].cod_articulo = event_e_ticket.cod_articulo;
                    (*art_fact2)[i].costo = event_e_ticket.costo;
                    strcpy( (*art_fact2)[i].cod_barra, event_e_ticket.cod_barra );
                    (*art_fact2)[i].pos = GET_POSITION_REG();
                    (*art_fact2)[i].pos_ticket = pos_ticket;
                    (*art_fact2)[i].cod_movimiento = event_e_ticket.cod_movimiento;
                    if( !EXENTO_PARA_DIPLOMATICOS() ) {
                        if( articulo.gravado == 'E'
   				                &&	EXENTO_COMO_GRAVADO( OBTENER_COD_GRAVADO(  ) )
   					            || fabs( event_e_ticket.exento ) < 0.001 ) {
                            (*art_fact2)[i].cod_iva = 0;
   				} else {
   					(*art_fact2)[i].cod_iva = event_e_ticket.cod_iva;
   				}
   			} else {
                        (*art_fact2)[i].cod_iva = 1;
                    }
            (*art_fact2)[i].gravado = articulo.gravado;
                    if( ES_FACTURA_A( comp.cond_cli ) && !FACTURA_CON_PRECIO_FINAL ) {
					(*art_fact2)[i].precio =
						event_e_ticket.importe_sin_iva + event_e_ticket.exento +
						event_e_ticket.recargo_tarjeta;
                    }
                    else {
                        (*art_fact2)[i].precio = event_e_ticket.importe_sin_iva + event_e_ticket.exento
                                           + event_e_ticket.iva1 + event_e_ticket.recargo_tarjeta;
                        for( k = 0;k <= 3;k++ ) {
                            precio += event_e_ticket.monto_impuesto[k];
                        }
                    }
                    p_art++;
                }
                if( RAM_FACTURA && DESCUENTOS_POR_CUERPO ) {
                    int incrementar,procesar;
                    long codigo = event_e_ticket.cod_articulo;
                    int posicion = event_e_ticket.posicion;
                    /*ind_ev.id_evento = event_e_ticket.id_evento;
                    ind_ev.posicion = 0;
                    ind_ev.tipo_motivo = 0;
                    ind_ev.id_motivo = 0;*/
                    //SELECT( AREA_E_DES_PRO );
                    SELECT_TABLE( T_EV_DESPRO, tipo_t_ev_despro );
                    //SET_ORDER( 1 );
                    //GO( TOP );
                    //GET_GREATER_E( ( char* )&ind_ev );
                    SET_WHERE( "ID_EVENTO >= '%ld'", event_e_ticket.id_evento );
                    SET_ORDER2( "ID_EVENTO, POSICION, TIPO_MOTIVO, ID_MOTIVO" );
                    RUN_QUERY( NO );
                    while( event_despro->id_evento == event_e_ticket.id_evento && !dbEOF()/*!BtrvEOF()*/
                        && event_despro->posicion == posicion ) {
                        if( ( event_despro->cod_articulo == codigo )
                         && event_despro->no_es_dto != 'S' ) {
                            art_desc2[i]->cod_articulo = (*art_fact2)[i].cod_articulo;
                            strcpy( art_desc2[i]->cod_barra, (*art_fact2)[i].cod_barra );
                            art_desc2[i]->cantidad += (*art_fact2)[i].cantidad;
                            //signo = (RAM_NOTA_CR)? -1.00 : 1.00;
                            signo = 1.00;
                            art_desc2[i]->precio += -( event_despro->importe ) * signo;
                            _DISCRIMINA_IMPUESTOS_EN_ARTICULO( ( event_despro->importe * 100 )
                                                             / (*art_fact2)[i].precio * signo,
                                                               event_despro->posicion,
                                                               ( int* )&incrementar,
                                                               ( char* )&procesar );
                        }
                        //GET_NEXT();
                        SKIP2( 1 );
                    }
                }
            }
            //SELECT( AREA_E_TICKET );
			SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
            SKIP2( 1 );
            pos_ticket++;
        }
        cant_renglones = p_art;
		*cantidad_de_items = cant_renglones;
    }
    //SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );
    return ( rta );
}
/*****************************************************************************/
char * FORMATEA_CUIT( char *nro_cuit )
/*****************************************************************************/
{
    static char cuit[21];
    if( config.pais != ELSALVADOR ) {
        memcpy( cuit, nro_cuit, 2 );
        cuit[2] = '-';
        memcpy( &cuit[3], &nro_cuit[2], 8 );
        cuit[11] = '-';
        cuit[12] = nro_cuit[10];
        cuit[13] = 0;
   } else {
        memcpy( cuit, nro_cuit, 4 );
        cuit[4] = '-';
        memcpy( &cuit[5], &nro_cuit[4], 6 );
        cuit[11] = '-';
        memcpy( &cuit[12], &nro_cuit[10], 3 );
        cuit[15] = '-';
        cuit[16] = nro_cuit[13];
        cuit[17] = 0;
    }
    return ( cuit );
}

/***********************************************************************/
int OBTENER_DATOS_COMPROBANTE( int clase, int tipo, int cant, short int *nro_suc, long *nro_comp,
                               short int *copias, int actualizar_nro, int pedir_nro )
/***********************************************************************/
{
    /* actualizar_nro:
           -1 = No Actualiza el numero comprobante y setea nro_comp = 0
            0 = No actualiza el numero comprobante seteando en nro_comp el recuperado
            1 = Actualiza el numero de comprobante
    */
	struct _factu_mem factu_mem;
    char buffer[161], _nro_suc[25], _nro_comp[25], letra[] = {"_x"};
    char _copias[15] = {"copias"};
    int rta = 1, k, seguir = 1, cant_dig = 0, tipo_tabla = 0;
    long numero,num;
	char dato[100];

    /*------------------ Abre el archivo ------------------*/
	memset( &factu_mem, 0, sizeof( factu_mem ) );
	memset( _nro_comp, 0, sizeof( _nro_comp ) );
	memset( _nro_suc, 0, sizeof( _nro_suc ) );
    memset( buffer, 0, sizeof( buffer ) );

	if( clase == _REMITO ) {
        if( !ABRIR_ARCHIVOS_FACTURAS( clase, buffer ) ) {
            return 0;
        }
		strncpy( _nro_comp, "nro_remito", sizeof( _nro_comp ) - 1);
		strncpy( _nro_suc, "nro_sucursal", sizeof( _nro_suc ) - 1);
    }
	else {
		strncpy( _nro_suc, "nro_sucursal", sizeof( _nro_suc ) - 1);
		tipo_tabla = TT_ORIG;

		#ifdef COMPILAR_MODO_ENTRENAMIENTO
			if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
				tipo_tabla = TT_ENTR;
			} 
		#endif
        
		if( OPENED( T_FACTU_MEM, tipo_tabla ) 
			|| OPEN_TABLE( T_FACTU_MEM, tipo_tabla, ( char* )&factu_mem, sizeof( struct _factu_mem ) ) == 0 ) {
            
			SELECT_TABLE( T_FACTU_MEM, tipo_tabla );
			SET_WHERE("");
			RUN_QUERY(NO);
			if( clase == _FACTURA || clase == _NOTA_CREDITO ) {
				if( MODO_NEGOCIO == SERVICIOS ) {
					if( RAM_PERFIL == 3 ) {// si es Docthos
						letra[1] = 'a';
					} else {
						letra[1] = 'b';
					}
				} else {
					if( FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
						letra[1] = ( RAM_FACTURA == 1 ) ? 'a' : 'b';
					}
					else {
						letra[1] = ( tipo == 1 ) ? 'a' : 'b';
					}
				}
				strncpy( _nro_suc, "nro_sucursal", sizeof( _nro_suc ) - 1 );
				strncat( _nro_suc, letra, sizeof( _nro_suc ) - strlen( _nro_suc ) - 1 );
				strncpy( _nro_comp, "nro_factura", sizeof( _nro_comp ) - 1 );
				strncat( _nro_comp, letra, sizeof( _nro_comp ) - strlen( _nro_comp ) - 1 );
				strncat( _copias, letra, sizeof( _copias ) - strlen( _copias ) - 1 );
			} else {
				if( MODO_NEGOCIO != SERVICIOS ) {
					if( clase == _TICKET_FACTURA ) {
						strncpy( _nro_comp, "nro_ticket_factura", sizeof( _nro_comp ) - 1 );
					} else {
						/****** TICKET ******/
						strncpy( _nro_comp, "nro_ticket", sizeof( _nro_comp ) - 1 );
					}
				}
			}
		} else {
			return 0;	
		}
	}
    /*------------------------ Obtiene copias ----------------------*/
	if( copias ) {
        #ifdef INVEL_L
        GET_GREATER_E( _copias );
        #else
		SET_WHERE( "DESCRIPCION = '%s'", _copias );
		RUN_QUERY(NO);
        #endif
    	if( FOUND2() ) {
			cant_dig = OBTENER_DIGITOS( factu_mem.valor, factu_mem.tipo_dato );
			memset( dato, 0, sizeof( dato ) );
			_snprintf( dato, sizeof( dato ), "%s", factu_mem.valor );
			dato[cant_dig] = '\0';
			*copias = atoi( dato );
        } else {
            rta = 0;
        }
    }
    /*----------------------- Obtiene Nro. sucursal ---------------*/
    if( rta && nro_suc && ( !PEDIR_NUMERO_FACTURA || PEDIR_NUMERO_FACTURA == SUGERIR ) ) {
        #ifdef INVEL_L
        GET_GREATER_E( _nro_suc );
        #else
		SET_WHERE( "DESCRIPCION = '%s'", _nro_suc );
		RUN_QUERY(NO);
        #endif
		if( FOUND2() ) {
			cant_dig = OBTENER_DIGITOS( factu_mem.valor,  factu_mem.tipo_dato );
			memset( dato, 0, sizeof( dato ) );
			_snprintf( dato, sizeof( dato ), "%s", factu_mem.valor );
			dato[cant_dig] = '\0';
			*nro_suc = atoi( dato );
		} else {
            rta = 0;
        }
    }
    /*--------------------- Obtiene Nombre sucursal ---------------*/
    #ifdef INVEL_W
	SET_WHERE( "DESCRIPCION = '%s'", "sucursal" );
	RUN_QUERY(NO);
    #else
    GET_GREATER_E( "sucursal" );
    #endif
	if( FOUND2() ) {
		strncpy( comp.sucursal, factu_mem.valor, sizeof( comp.sucursal ) - 1 );
    }
    else {
        rta = 0;
    }
    /*---------------------- Obtiene Nro. comprobante ---------------------*/
    if(IMPRESORA_FISCAL != FISCAL)
	{
    if( rta && ( nro_comp || actualizar_nro )
     && ( !PEDIR_NUMERO_FACTURA || PEDIR_NUMERO_FACTURA == SUGERIR ) ) {
        SET_LOCKS( 1, 1, 0 );
        #ifdef INVEL_W
        SET_WHERE( "DESCRIPCION = '%s'",  _nro_comp );
		RUN_QUERY(NO);
        #else
        GET_GREATER_E( _nro_comp );
        #endif
		if( FOUND2() ) {
			cant_dig = OBTENER_DIGITOS( factu_mem.valor,  factu_mem.tipo_dato );
			memset( dato, 0, sizeof( dato ) );
			_snprintf( dato, sizeof( dato ), "%s", factu_mem.valor );
			dato[cant_dig + 1] = '\0';
			numero = atol( dato );
            if( !REIMPRESION || INCREMENTAR_NRO_EN_REIMPRESION ) {
                if( !( actualizar_nro && PEDIR_NUMERO_FACTURA == SUGERIR ) ) {
                    if( nro_comp ) {
                        *nro_comp = numero + 1;
                    }
                    if( actualizar_nro == 1 ) {
                        numero += cant;
						memset( dato, 0, sizeof( dato ) );
						_snprintf( dato, sizeof( dato ), "%ld", numero );
						dato[cant_dig + 1] = '\0';
						memset( factu_mem.valor, 0, sizeof( factu_mem.valor ) );
						strncpy( factu_mem.valor, dato, cant_dig + 1);
                        #ifdef INVEL_L
                        BEGIN_TRANSACTION();
                        #endif
						UPDATE2();
                        #ifdef INVEL_L
                        END_TRANSACTION();
                        #endif
                    }
                }
            }
            else if( nro_comp ) {
                *nro_comp = numero;
            }
        }
        else {
            rta = 0;
        }
    }
    /*--------------------- Pide nmero de factura ----------------------*/
    if( PEDIR_NUMERO_FACTURA ) {
        if( pedir_nro ) {
            LIMPIAR_RECUADRO_PETICION( S_NRO_COMPROBANTE );
            MOSTRAR_GRAN_CARACTER( 33 - ancho_panel / 2, 19, '-' );
            if( PEDIR_NUMERO_FACTURA == SI ) {
                *nro_suc = ( short )PEDIR_GRAN_ENTERO( 21 - ancho_panel / 2, 19, 4 );
                *nro_comp = PEDIR_GRAN_ENTERO( 35 - ancho_panel / 2, 19, 8 );
            }
            else if( PEDIR_NUMERO_FACTURA == SUGERIR ) {
                while( seguir ) {
                    MOSTRAR_GRAN_ENTERO( 21 - ancho_panel / 2, 19, "0000", *nro_suc );
                    MOSTRAR_GRAN_ENTERO( 35 - ancho_panel / 2, 19, "00000000", *nro_comp );
                    MENSAJE_STRING( S_ENTER_CONFIRMAR_ESC_INGRESAR_NRO_DE_FACTURA );
                    k = GETCH();
                    while( k != 13 && k != 27 ) {
                        k = GETCH();
                    }
                    BORRAR_MENSAJE();
                    if( k == 27 ) {
                        num = PEDIR_GRAN_ENTERO( 35 - ancho_panel / 2, 19, 8 );
                        if( num > 0 ) {
                            *nro_comp = num;
                            seguir = 0;
                        }
                    }
                    else {
                        seguir = 0;
                    }
                }
            }
        }
        else {
            if( !REIMPRESION || PEDIR_NUMERO_FACTURA == SUGERIR ) {
                *nro_suc = comp.nro_suc;
                *nro_comp = comp.nro_comp;
                if( rta && PEDIR_NUMERO_FACTURA == SUGERIR && actualizar_nro ) {
                    SET_LOCKS( 1, 1, 0 );
                    #ifdef  INVEL_W
					SET_WHERE( "DESCRIPCION = '%s'",  _nro_comp );
                    #else
                    GET_GREATER_E( _nro_comp );
                    #endif
                    numero = *nro_comp + cant;
					if( FOUND2() ) {
						cant_dig = OBTENER_DIGITOS( factu_mem.valor,  factu_mem.tipo_dato );
						memset( dato, 0, sizeof( dato ) );
						_snprintf( dato, sizeof( dato ), "%ld", numero );
						dato[cant_dig] = '\0';
						memset( factu_mem.valor, 0, sizeof( factu_mem.valor ) );
						strncpy( factu_mem.valor, dato, cant_dig );
                        #ifdef INVEL_L
                        BEGIN_TRANSACTION();
                        #endif
						UPDATE2();
                        #ifdef INVEL_L
                        END_TRANSACTION();
                        #endif
                    }
                }
            }
        }
    }
	}
    /*----------------- Termina ----------------*/
    #if defined(INVEL_W) || defined(INVEL_L)
    UNLOCK_();
    #else
    UNLOCK();
    #endif
    SET_LOCKS( 0, 0, 0 );
    if( nro_suc ) {
        SET_MEMORY_INT( __ram_nro_suc, *nro_suc );
    }
	if(IMPRESORA_FISCAL != FISCAL)
	{
		if( nro_comp ) {
			SET_MEMORY_LONG( __ram_nro_comp, *nro_comp );
		}
    }
	if( clase == _TICKET || clase == _TICKET_FACTURA || clase == _FACTURA || clase == _NOTA_CREDITO ) {
		CLOSE_TABLE( T_FACTU_MEM, tipo_tabla );
	} else {
		CLOSE_DATABASE( AREA_AUX );
	}
    if( actualizar_nro == -1 ) {
        *nro_comp = 0;
    }

	buffer[sizeof( buffer ) - 1]='\0';    
	_nro_suc[sizeof( _nro_suc ) - 1]='\0';
	_nro_comp[sizeof( _nro_comp ) - 1]='\0';

    return ( rta );
}
/***************************************************************************/
int DATOS_COMPROBANTE( int tipo )
/***************************************************************************/
{
    int cant = 0, rta;
    long cod;
    if( clientes.cod_cliente != RAM_NRO_CLIENTE ) {
        //SELECT( AREA_CLIENTES );
		SELECT_TABLE( T_CLIENTES, TT_ORIG );
        cod = RAM_NRO_CLIENTE;
		SET_WHERE("COD_CLIENTE = %ld",cod); 
		RUN_QUERY(SI);
        //GET_EQUAL( ( char* )&cod );
    }
    comp.tipo_comp = tipo;
	comp.cond_cli = clientes.cond_iva;
    comp.subtipo = ( ES_FACTURA_A( comp.cond_cli ) ) ? 1 : 2;
    cant = ( COMP_INDEP ) ? CANTIDAD_FORMULARIOS( cant_renglones ) : 1;
    rta = OBTENER_DATOS_COMPROBANTE( tipo, comp.subtipo, cant, &comp.nro_suc, &comp.nro_comp, NULL,
                                     NO, SI );

    return ( rta );
}
/***************************************************************************/
void APUNTA_CFG( int tipo )
/***************************************************************************/
{
    _tipo = tipo;
    switch( tipo ) {
        case _NOTA_CREDITO:
        case _FAC_REM:
        case _FACTURA:
            CONFIG_COMPROBANTE1 = CONFIG_FACTURA;
            //_tipo = _FACTURA;
            break;
        case _REMITO:
            CONFIG_COMPROBANTE1 = CONFIG_REMITO;
            break;
        case _TKT:
            CONFIG_COMPROBANTE1 = CONFIG_TKT1;
            break;
    }
}
/***************************************************************************/
void INIT_COMPROBANTE( int tipo )
/***************************************************************************/
{
    /*--------- Valores por defecto ------------*/
    CANT_RENGLONES[tipo] = 1;
    LARGO_COMPROBANTE[tipo] = 72;
    /*----------- Tomamos los parametros iniciales ---------*/
    APUNTA_CFG( tipo );
    ind_config = 0;
    while( CONFIG_COMPROBANTE1[ind_config].codigo != _TKT_FIN_INICIALIZACION
        && CONFIG_COMPROBANTE1[ind_config].codigo != _FIN ) {
        COMP_ejecutar_comando( &ind_config );
    }
    ind_comienzo[tipo] = ind_config + 1;
    //ImpFiscal-verificar
    //if( IMPRESORA_FISCAL ) {
    //    strcpy( TIPO_COMP[2], "GUIA" );
    //}
}
/***************************************************************************/
int CANTIDAD_FORMULARIOS( int renglones ){
/***************************************************************************/
	return _CANTIDAD_FORMULARIOS( renglones, _tipo );
}

/***************************************************************************/
int _CANTIDAD_FORMULARIOS( int renglones , int _tipo_loc)
/***************************************************************************/
{
    int cantidad = 0;
    int renglones_configuracion = 0;

    //renglones_configuracion = ( DESCUENTOS_POR_CUERPO ) ? CANT_RENGLONES[_tipo] - 1 : CANT_RENGLONES[_tipo];
    renglones_configuracion = CANT_RENGLONES[_tipo_loc];
    cantidad = renglones / ( ( renglones_configuracion <= 0 ) ? 1 : renglones_configuracion );
    if( renglones > 0 ) {
        if( renglones_configuracion 
			&& ( renglones % renglones_configuracion/*CANT_RENGLONES[_tipo]*/ ) ) {
            cantidad++;
        }
    }

    return ( cantidad );
}
/***************************************************************************/
void COMP_ejecutar_comando( int *ind_config )
/***************************************************************************/
{
    char buffer[210],aux[40],cadaux[512],cadaux2[512],xx[512];
    INT16 codigo = 0, _fila, i, quedan, cod_sub, cod_sol = 0,reconocida = 0;
    float cod_iva;
	char cadena[30];
    double importe_total = 0.0;
    char decimales = DECIMALES;
    int modalidad, nro_orden = 0, posicion = 0, ind_configTmp = 0, cod_var;
//	char print[30];
    static int imprimir = 0;
    ( *ind_config )++;
    codigo = ( INT16 )CONFIG_COMPROBANTE1[( *ind_config )].codigo;
	if( codigo > 0 ) //si llega a aveluar las condiciones y la rebota, figuracon desconocida y no es asi, que genio quien puso el log en el default!!!
		reconocida = 1;
    codigo = ( INT16 )COMP_evaluar_condiciones( ( int )codigo );
    //fabiana 15-02-05 ** nuevo manejo de redondeos--para factura o tickets donde se discrimina iva
    if( ( DECIMALES < 4 )
     && ( ES_FACTURA_A( clientes.cond_iva ) || MOSTRAR_SIN_IVA_ELSALVADOR()
       || MOSTRAR_PRECIO_SIN_IVA_EN_VENTAS ) ) {
        decimales = 4;
    }       
    switch( codigo ) {
        case _COMIENZO_SI:
            // Chequeo que la formula sea correcta
            _SI_P_FORMULA = CONFIG_COMPROBANTE1[( *ind_config )].parametros;
            codigo = ( INT16 )CONFIG_COMPROBANTE1[( *ind_config )].codigo;
            codigo = ( INT16 )COMP_evaluar_condiciones( ( int )codigo);
            while( CONFIG_COMPROBANTE1[( *ind_config )].codigo != _FIN_SI ) {
                if( codigo )
                                // Si la formula es correcta ejecuto hasta [FIN SI]
                {
                    COMP_ejecutar_comando( ind_config );
                }
                else
                                // si la formula no es correcta, saltea el comando
                {
                    ( *ind_config )++;
                }
            }
            break;
        case _LETRA_FACTURA:
            IMPRIMIR_CADENA( columna, ( ES_FACTURA_A( comp.cond_cli ) ) ? "A" : "B" );
            break;
        case _TIPO_COMPROBANTE:
            IMPRIMIR_CADENA( columna, TIPO_COMP[comp.tipo_comp] );
            break;
        case _NUMERO_COMPROBANTE:
            MOSTRAR_ENTERO( columna, PRN, "0000", comp.nro_suc );
            IMPRIMIR_CADENA( columna + 4, "-" );
            MOSTRAR_ENTERO( columna + 5, PRN, "00000000", nro_comp );
            break;
        case _NRO_ORDEN_COMPROBANTE:
            IMPRIMIR_CADENA( columna, ORDEN[orden] );
            break;
        case _FECHA:
            MOSTRAR_FECHA( columna, PRN, comp.fecha );
            break;
        case _HORATKT:
            MOSTRAR_HORA( columna, PRN, comp.hora );
            break;
        case _NOMBRE_SUCURSAL:
            //IMPRIMIR_CADENA( columna, comp.sucursal );
			MOSTRAR_ENTERO( columna, PRN, "000", config.sucursal );
            break;
        case _SECUENCIA:
            MOSTRAR_ENTERO( columna, PRN, ( config.pais != ECUADOR ) ? "000000000" : "0000000",
                            ( config.pais == ELSALVADOR && RAM_FACTURA ) ? NRO_EVENTO - 1
                          : NRO_EVENTO - 1 );
            break;
        case _NOMBRE_CLIENTE:
            IMPRIMIR_CADENA( columna, clientes.nombre/*CLIENTE_AFINIDAD_NOMBRE*/ );
            break;
        case _DOMICILIO_CLIENTE:
            IMPRIMIR_CADENA( columna, clientes.domicilio );
            break;
        case _LOCALIDAD_CLIENTE:
            IMPRIMIR_CADENA( columna, clientes.localidad );
            break;
        case _PROVINCIA_CLIENTE:
            IMPRIMIR_CADENA( columna, clientes.provincia );
            break;
        case _TIPO_IVA_CLIENTE:
            {
                char cond_iva[31];

                memset( cond_iva, 0, sizeof( cond_iva ) );

		        if( OBTENER_DESCRIPCION_COND_IVA( cond_iva, clientes.cond_iva, sizeof( cond_iva ) ) ) {
                    TRIM( cond_iva );
                    IMPRIMIR_CADENA( columna, cond_iva );
                }
            }
            //IMPRIMIR_CADENA( columna, TIPOS_IVA[comp.cond_cli - 1] );
            break;
        case _CUIT_CLIENTE:
            IMPRIMIR_CADENA( columna, FORMATEA_CUIT( clientes.cuit ) );
            break;
        case _CONDICION_VENTA:
            if( config.pais == ELSALVADOR ) {
                //SELECT( AREA_PAGO );
                //GET_EQUAL( ( char* )&event_ticket.id_evento );
				SELECT_TABLE( T_PAGOS, tipo_t_pagos );
				SET_WHERE("ID_EVENTO = '%ld'",event_ticket.id_evento);
				SET_ORDER2("ID_EVENTO");
				RUN_QUERY(NO);
                if( FOUND2() ) {
                    int credito = 0;
                    for( i = 0;i < cant_pagos;i++ ) {
                        if( medios[event_pago.modo_pago - 1].credito ) {
                            credito = 1;
                        }
                        SKIP2( 1 );
                    }
                    if( credito ) {
                        IMPRIMIR_CADENA( columna, "CREDITO" );
                    }
                    else {
                        IMPRIMIR_CADENA( columna, "CONTADO" );
                    }
                    // rox -- esto se hizo exclusivamente para El Salvador porque lo necesitaban asi..
                    // lo que sigue es la funcionalidad como estaba anteriormente.
                }
            }
            else {
                if( !RAM_NOTA_CR || !MEDIO_PAGO_NC ) {
                    buffer[0] = 0;
                    quedan = sizeof( buffer ) - 1;
                    //SELECT( AREA_PAGO );
                    //GET_EQUAL( ( char* )&event_ticket.id_evento );
					SELECT_TABLE( T_PAGOS, tipo_t_pagos );
					SET_WHERE("ID_EVENTO = '%ld'",event_ticket.id_evento);
					SET_ORDER2("ID_EVENTO");
					RUN_QUERY(NO);
                    for( i = 0;i < cant_pagos;i++ ) {
                        if( medios[event_pago.modo_pago - 1].tarjeta ) {
                            if( quedan > 0 ) {
                                strncat( buffer, NOMBRE_TARJETA( event_pago.tarjeta ), quedan );
                            }
                            quedan -= strlen( NOMBRE_TARJETA( event_pago.tarjeta ) );
                            TRIM( buffer );
                            if( quedan > 0 ) {
                                strncat( buffer, " CUPON: ", quedan );
                            }
                            quedan -= 8;
                            if( quedan > 9 ) {
                                sprintf( &buffer[strlen( buffer )], "%09li", event_pago.numero_id );
                            }
                            quedan -= 9;
                        }
                        else {
                            if( quedan > 0 ) {
                                strncat( buffer, medios[event_pago.modo_pago - 1].nombre, quedan );
                            }
                            quedan -= strlen( medios[event_pago.modo_pago - 1].nombre );
                            TRIM( buffer );
                        }
                        FTOC( signo_nc * event_pago.importe, aux, ENTEROS, DECIMALES, 0, 0 );
                        if( quedan > ENTEROS + DECIMALES + 3 ) {
                            IMPRIMIR_CADENA( columna, buffer );
							columna = 35;
                            IMPRIMIR_CADENA( columna, aux );
                            /*strcat( buffer, " " );
                            strcat( buffer, aux );
                            strcat( buffer, " " );*/
                            quedan -= ENTEROS + DECIMALES + 3;
                            columna = 1;
                            COMP_SET_FILA( fila + 1 );
                            memset( buffer, 0, sizeof( buffer ) );
                        }
                        SKIP2( 1 );
                    }
                    //IMPRIMIR_CADENA( columna, buffer );
                    //SELECT( AREA_E_TICKET );
					SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
                }
            }
            break;
        case _CODIGO_BARRA_12:
            memcpy( buffer, event_e_ticket.cod_barra, 12 );
            buffer[12] = 0;
            IMPRIMIR_CADENA( columna, buffer );
            break;
        case _CODIGO_BARRA:
            IMPRIMIR_CADENA( columna, event_e_ticket.cod_barra );
            //fabiana
            //IMPRIMIR_CADENA( columna, _TICKET_COD_BARRA_ORIGINAL( art_fact[renglon].pos_ticket ) );
            break;
        case _CODIGO_ARTICULO:
            MOSTRAR_ENTERO( columna, PRN, "000000000", event_e_ticket.cod_articulo );
            break;
        case _DESC_ARTICULO:
            IMPRIMIR_CADENA( columna, articulo.nombre );
            break;
        case _IVA_ARTICULO:
            if( art_fact[renglon].exento || articulo.gravado == 'N'
             || ( articulo.gravado == 'E' && !EXENTO_COMO_GRAVADO( OBTENER_COD_GRAVADO() ) ) ) {
                cod_iva = 0;
            }
            else {
                cod_iva = iva[art_fact[renglon].cod_iva].porc_iva1;
            }
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0, cod_iva );
            break;
        case _PRECIO_ARTICULO:
            //fabiana 15-02-05 ** nuevo manejo de redondeos-Se redondea a 4 decimales y no es configurable
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, 4, 1, 0, precio );
            break;
        case _IMPORTE_ARTICULO:
            //fabiana 15-02-05 ** nuevo manejo de redondeos- el importe del 
            //articulo cuando es factura a, se redondea por cdigo a los decimales configurables.
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0, importe );
            break;
        case _PRECIO_FINAL:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0, signo_nc * precio_final );
            break;
        case _SUBTOTAL:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                ( FACT_INDEP_DEL_TIPO_DE_CLIENTE ) ? neto + iva1 : signo_nc * subtotal );
            break;
        case _NETO_GRAVADO:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0, ( signo_nc * neto ) );
            break;
        case _EXENTO:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( exento ) ? ( signo_nc * exento ) : exento ) );
            break;
        case _IVA1:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( iva1 ) ? ( signo_nc * iva1 ) : iva1 ) );
            break;
        case _IVA2:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( iva2 ) ? ( signo_nc * iva2 ) : iva2 ) );
            break;
        case _ILA:
            //MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0, ( fabs( imp[0] ) ? ( signo_nc * imp[0] ) : imp[0] ) );
            break;
        case _IHA:
            //MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0, ( fabs( iha ) ? ( signo_nc * iha ) : iha ) );
            break;
        case _GIRO_CLIENTE:
            IMPRIMIR_CADENA( columna, clientes.giro );
            break;
        case _NRI_CLIENTE:
            IMPRIMIR_CADENA( columna, FORMATEA_NRI( clientes.nri ) );   
            break;  
        case _OC_GD_CLIENTE:
            IMPRIMIR_CADENA( columna, RAM_OC_GD );
            break;
        case _TOTAL:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0, signo_nc * total );
            break;
        case _PERCEP_212_IVA1:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( percep_212_iva1 ) ? ( signo_nc * percep_212_iva1 )
                              : percep_212_iva1 ) );
            break;
        case _PERCEP_212_IVA2:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( percep_212_iva2 ) ? ( signo_nc * percep_212_iva2 )
                              : percep_212_iva2 ) );
            break;
        case _LUGAR_DE_EMISION:
            IMPRIMIR_CADENA( columna, comp.emision );
            break;
        case _CANTIDAD_ARTICULO:
			MOSTRAR_FLOTANTE( columna, PRN, CANTIDAD_DIGITOS( cantidad, 3 ), 3, 1, 0 , cantidad );
            break;
        case _CANTIDAD_SIN_DECIMALES:
            if( cantidad < 1 ) {
                cantidad = 1;
            }
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, 0, 1, 0, cantidad );
            break;
        case _CONTENIDO_ARTICULO:
            IMPRIMIR_CADENA( columna, articulo.contenido );
            break;
        case _UNIDAD_ARTICULO:
            IMPRIMIR_CADENA( columna, articulo.unidad );
            break;
        case _IMPUESTO_INTERNO:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( imp_int ) ? ( signo_nc * imp_int ) : imp_int ) );
            break;
        case _NUMERO_CLIENTE:
            MOSTRAR_CLIENTE( columna, PRN, event_ticket.nro_cliente );
            break;
        case _TASA_IVA1:
            // MOSTRAR_FLOTANTE( columna, PRN, 2, 2, 0, 0, config.porc_iva1 );
            MOSTRAR_FLOTANTE( columna, PRN, 2, 2, 0, 0, iva[INDICE_IVA_ARTICULO()].porc_iva1 );
            break;
        case _TASA_IVA2:
            // MOSTRAR_FLOTANTE( columna, PRN, 2, 2, 0, 0, config.porc_iva2 );
            MOSTRAR_FLOTANTE( columna, PRN, 2, 2, 0, 0, iva[INDICE_IVA_ARTICULO()].porc_iva2 );
            break;
        case _LARGO_FACTURA:
        case _LARGO_FACTURA_A:
        case _LARGO_FACTURA_B:
        case _LARGO_REMITO:
            ind_configTmp = 0;
            LARGO_COMPROBANTE[_tipo] = COMP_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _RENGLONES_CUERPO:
        case _RENGLONES_CUERPO_A:
            ind_configTmp = 0;
            CANT_RENGLONES[_FACTURA] = COMP_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _RENGLONES_CUERPO_B:
            ind_configTmp = 0;
            CANT_RENGLONES[_NOTA_CREDITO] = COMP_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _NRO_DOC_FISCAL_COMP:
            ind_configTmp = 0;
            NRO_DOC_FISCAL_COMP = COMP_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _POSICION:
            {
                int col_aux,fila_aux;
                ind_configTmp = 0;
                col_aux = COMP_tomar_entero( ind_config, &ind_configTmp );
                fila_aux = COMP_tomar_entero( ind_config, &ind_configTmp );
                if( !imprimiendo_factura_journal ) {
                    columna = col_aux;
                    _fila = fila_aux;
                    COMP_SET_FILA( _fila );
                }
            }
            break;
        case _POS_X:
            {
                int col_aux;
                ind_configTmp = 0;
                col_aux = COMP_tomar_entero( ind_config, &ind_configTmp );
                if( !imprimiendo_factura_journal ) {
                    columna = col_aux;
                }
            }
            break;
        case _POS_Y:
            {
                int fila_aux;
                ind_configTmp = 0;
                fila_aux = COMP_tomar_entero( ind_config, &ind_configTmp );
                if( !imprimiendo_factura_journal ) {
                    _fila = fila_aux;
                    COMP_SET_FILA( _fila );
                }
            }
            break;
        case _CADENA:
            IMPRIMIR_CADENA( columna, CONFIG_COMPROBANTE1[( *ind_config )].parametros );
            //( *ind_config ) += strlen( &CONFIG_COMPROBANTE[( *ind_config )] ) + 1;
            break;
        case _LETRA_NORMAL:
			PRN_LETRA_CHICA();
            break;
        case _LETRA_DOBLE_ANCHO:
			PRN_LETRA_GRANDE();
            break;
		case _LETRA_RESALTADA:
			
			TIPO_DE_LETRA = LETRA_RESALTADA ;
			break;
		case _LETRA_DOBLE_ALTO:
			TIPO_DE_LETRA = LETRA_DOBLE_ALTO ;
			break;
		case _LETRA_DOBLE_ALTO_NEGRITA:
			TIPO_DE_LETRA = LETRA_DOBLE_ALTO_NEGRITA ;
			break;
		case _LETRA_DOBLE_ANCHO_NEGRITA:
			TIPO_DE_LETRA = LETRA_DOBLE_ANCHO_NEGRITA ;
			break;
		case _LETRA_DOBLE_ANCHO_ALTO:
			TIPO_DE_LETRA = LETRA_DOBLE_ANCHO_ALTO ;
			break;
		case _LETRA_DOBLE_ANCHO_ALTO_NEGRITA:
			TIPO_DE_LETRA = LETRA_DOBLE_ANCHO_ALTO_NEGRITA ;
			break;
		case _LETRA_SUBRAYADO:
			TIPO_DE_LETRA = LETRA_SUBRAYADO ;
            break;
        case _TKT_FIN_INICIALIZACION:
            break;
        case _INDEPENDIENTES:
            COMP_INDEP = 1;
            break;
        case _CADENA_CONTINUA_EN_PROXIMA:
            ind_configTmp = 0;
            COLUMNA_CONT = COMP_tomar_entero( ind_config, &ind_configTmp );
            FILA_CONT = COMP_tomar_entero( ind_config, &ind_configTmp );
            P_CAD_CONT = &CONFIG_COMPROBANTE1[( *ind_config )].parametros[ind_configTmp];
            
            break;
        case _AGREGAR_IVA_NO_INSCRIPTO:
            AGREGAR_IVA_NO_INSC = 1;
            break;
        case _SI_FACTURA_A:
            SI_FACTURA_A = 1;
            break;
        case _SI_FACTURA_B:
            SI_FACTURA_B = 1;
            break;
        case _LARGO_CADENA:
            ind_configTmp = 0;
            LARGO_CADENA = (char)COMP_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _TOTAL_EN_LETRAS:
            COMP_total_en_letras( ind_config );
            break;
        case _REIMPRESION_COMPROBANTE:
            if( REIMPRESION ) {
                IMPRIMIR_CADENA( columna, ST( S_REIMPRESION ) );
            }
            break;
        case _TOTAL_IMPUESTO:
            ind_configTmp = 0;
            codigo = COMP_tomar_entero( ind_config, &ind_configTmp );
            if( codigo <= 20 ) {
                MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                                  ( fabs( total_impuestos[codigo - 1] )
                                  ? ( signo_nc * total_impuestos[codigo - 1] )
                                  : total_impuestos[codigo - 1] ) );
            }
            break;
        case _TOTAL_IMPUESTO_FACTURA:
            total_impuestos_factura = 0.0;
            for( i = 0;i <= 19;i++ )
                                                                     //total_impuestos_factura += total_impuestos[i];
            {
                total_impuestos_factura += ROUND( total_impuestos[i], ENTEROS, DECIMALES );
            }
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( total_impuestos_factura )
                              ? ( signo_nc * total_impuestos_factura )
                              : total_impuestos_factura ) );
            break;
        case _EJECT:
            EJECT();
            break;
        case _CANTIDAD_PRESENTACION:
            MOSTRAR_ENTERO( columna, PRN, "999", event_e_ticket.cant_presentacion );
            break;
        case _CODIGO_PRESENTACION:
            MOSTRAR_ENTERO( columna, PRN, "9", event_e_ticket.cod_presentacion );
            break;
        case _SUC_REMITO_A_FACTURAR:
            MOSTRAR_ENTERO( columna, PRN, "0000", event_e_ticket.cant_presentacion );
            break;
        case _NRO_REMITO_A_FACTURAR:
            MOSTRAR_ENTERO( columna, PRN, "000000000", event_e_ticket.cod_articulo );
            break;
        case _NRO_VENDEDOR:
            MOSTRAR_ENTERO( columna, PRN, "00000", event_ticket.nro_vendedorl );
            break;
        case _NUMERO_CAJA:
            MOSTRAR_ENTERO( columna, PRN, "000", NRO_CAJA );
            break;
        case _NUMERO_CAJERO:
			memset(cadena, 0, sizeof(cadena));
            if( IMPRIMIR_LEGAJO_CAJERO ) {
				_snprintf( cadena, sizeof(cadena)-1,"%ld", NRO_LEGAJO_CAJERO );
				MOSTRAR_ENTERO( columna, PRN, MASCARA( '0', 8, cadena ), NRO_LEGAJO_CAJERO );
            }
            else {
				_snprintf( cadena, sizeof(cadena)-1,"%d", NRO_CAJERO );
				MOSTRAR_ENTERO( columna, PRN, MASCARA( '0', 4, cadena ), NRO_CAJERO );
            }
            break;
        case _RETENCION_DGR:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( retencion_dgr ) ? ( signo_nc * retencion_dgr )
                              : retencion_dgr ) );
            break;
        case _PERCEPCION_DGI:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( percepcion_dgi ) ? ( signo_nc * percepcion_dgi )
                              : percepcion_dgi ) );
            break;
        case _NRO_BOLETA_ANTERIOR:
            MOSTRAR_ENTERO( columna, PRN, "00000000", RAM_BOLETA_ANTERIOR );
            break;
        case _NRO_EVENTO:
            MOSTRAR_ENTERO( columna, PRN, "00000000", NRO_EVENTO - 1 );
            break;
        case _NUMERO_TARJETA:
            if( VAR_TARJETAS_STATUS == 2 ) {
                MOSTRAR_CADENA( columna, PRN, GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/ );
            }
            break;
        case _MENSAJE_TARJETA:
            if( IMPRIME_MENSAJE_TARJETA( VAR_TARJETAS_TARJETA ) && _VAR_TARJETAS_MENSAJE( 0 ) ) {
                MOSTRAR_CADENA( columna, PRN, VAR_TARJETAS_MENSAJE );
            }
            break;
        case _VIGENCIA_TARJETA:
            if( VAR_TARJETAS_COD_VIGENCIA ) {
                if( BUSCAR_VIGENCIA( VAR_TARJETAS_TARJETA, VAR_TARJETAS_COD_VIGENCIA, buffer ) ) {
                    MOSTRAR_CADENA( columna, PRN, buffer );
                }
            }
            break;
        case _PRIMER_VTO_TARJETA:
            if( VAR_TARJETAS_PRIMER_VTO ) {
                MOSTRAR_FECHA( columna, PRN, VAR_TARJETAS_PRIMER_VTO );
            }
            break;
        case _CANT_CUOTAS_TARJETA:
            if( VAR_TARJETAS_CUOTAS ) {
                MOSTRAR_ENTERO( columna, PRN, "00", VAR_TARJETAS_CUOTAS );
            }
            break;
        case _VALIDACION_TARJETA:
            if( VAR_TARJETAS_STATUS == 2 ) {
                VALIDAR_VOUCHER( COD_VALIDACION_VOUCHER( VAR_TARJETAS_TARJETA ) );
            }
            break;
        case _HOJA_ACTUAL:
            MOSTRAR_ENTERO( columna, PRN, "99", _hoja_actual );
            break;
        case _TOTAL_HOJAS_COMPROBANTE:
            MOSTRAR_ENTERO( columna, PRN, "99", comp.cant_formularios );
            break;
        case _PEDIR_FORMULARIO_CON_TECLA:
            //PEDIR_FORMULARIO_CON_TECLA = 1;
			MENSAJE_CON_TECLA( ST( S_COLOQUE_FORMULARIO_EN_IMPRESORA_PRES_ENTER ), 13 );
            break;
        case _NETO_GRAVADO_COD:
            ind_configTmp = 0;
            codigo = COMP_tomar_entero( ind_config, &ind_configTmp );
            if( art_fact_iva != NULL && codigo < MAX_COD_IVA ) {
                MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                                  signo_nc * art_fact_iva[codigo].neto_gravado );
            }
            else {
                MOSTRAR_CADENA( columna, PRN, ST( S_____PARAM_INVALIDO____ ) );
            }
            break;
        case _IVA1_COD:
            ind_configTmp = 0;
            codigo = COMP_tomar_entero( ind_config, &ind_configTmp );
            if( art_fact_iva != NULL && codigo < MAX_COD_IVA ) {
                MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                                  art_fact_iva[codigo].iva1 );
            }
            else {
                MOSTRAR_CADENA( columna, PRN, ST( S_____PARAM_INVALIDO____ ) );
            }
            break;
        case _IVA2_COD:
            ind_configTmp = 0;
            codigo = COMP_tomar_entero( ind_config, &ind_configTmp );
            if( art_fact_iva != NULL && codigo < MAX_COD_IVA ) {
                MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                                  art_fact_iva[codigo].iva2 );
            }
            else {
                MOSTRAR_CADENA( columna, PRN, ST( S_____PARAM_INVALIDO____ ) );
            }
            break;
        case _NC_DATOS_CAJA:
            MOSTRAR_ENTERO( columna, PRN, "999", RAM_CAJA_ANTERIOR );
			break;
        case _NC_DATOS_SUCURSAL:
            MOSTRAR_ENTERO( columna, PRN, "9999", RAM_SUCURSAL_ANTERIOR );
			break;
        case _NC_DATOS_COMPROBANTE:
            MOSTRAR_ENTERO( columna, PRN, "000000000", RAM_BOLETA_ANTERIOR );
			/* NO CORRESPONDE AGREGAR TEXTO PARA ESO CONFIGURA EN [CADENA][Comprobante :]
			if(IMPRESORA_FISCAL == FISCAL)
			{
				_snprintf(print,sizeof(print),"Comprobante: %d",RAM_BOLETA_ANTERIOR);	
				rta = IMPRIME_MENSAJE_FISCAL(print);
			}*/
            break;
        case _NC_DATOS_FECHA:
            MOSTRAR_FECHA( columna, PRN, RAM_FECHA_ANTERIOR );
			/*/* NO CORRESPONDE AGREGAR TEXTO PARA ESO CONFIGURA EN [CADENA][Comprobante :]
			if(IMPRESORA_FISCAL == FISCAL)
			{
				char fecha[20];
                DTOC( RAM_FECHA_ANTERIOR, print );
				_snprintf(fecha, sizeof(fecha),"Fecha Anterior: %s",print); 
				rta = IMPRIME_MENSAJE_FISCAL(fecha);
			}*/
            break;
            #define NC_ORIGEN_TICKET 1
            #define NC_ORIGEN_FACT_A 2
            #define NC_ORIGEN_FACT_B 3
        case _NC_DATOS_TIPO_COMP:
            switch( RAM_TIPO_COMP_ANTERIOR ) {
                case NC_ORIGEN_TICKET:
                    MOSTRAR_CADENA( columna, PRN, ST( S_TICKET ) );
					break;
                case NC_ORIGEN_FACT_A:
                    MOSTRAR_CADENA( columna, PRN, ST( S_FACTURA_A ) );
					break;
                case NC_ORIGEN_FACT_B:
                    MOSTRAR_CADENA( columna, PRN, ST( S_FACTURA_B ) );
					break;
            }
            break;
        case _NC_DATOS_AUTORIZACION:
            {
                struct _ev_medios ev_pago;
                //if( USE_DATABASE( AREA_BOTON, EVM_TMP, ( char* )&ev_pago, 
				//sizeof( struct _ev_medios ), NULL, 0, 0 ) == 0 ) {
				if( OPEN_TABLE( T_EVM_TMP, TT_ORIG, ( char* )&ev_pago, sizeof( struct _ev_medios )) == 0 ) { 
                    //SELECT( AREA_BOTON );
                    //SET_ORDER( 1 );
                    //GO( TOP );
					SELECT_TABLE( T_EVM_TMP, TT_ORIG );
					SET_WHERE("");
					RUN_QUERY(NO);
					GO2( TOP );
                    while( /*!BtrvEOF( )*/ !dbEOF( ) ) {
                        if( ev_pago.tarjeta ) {
                            MOSTRAR_CADENA( columna, PRN, NOMBRE_TARJETA( ev_pago.tarjeta ) );
                            MOSTRAR_CADENA( columna, PRN, " " );
                            MOSTRAR_CADENA( columna, PRN, ev_pago.auto_tarjeta );
                            LF( 1 );
                        }
                        SKIP2( 1 );
                    }
					CLOSE_TABLE( T_EVM_TMP, TT_ORIG );
                    //CLOSE_DATABASE( AREA_BOTON );
                }
            }
            break;
        case _SI_NOTA_CREDITO:
            SI_NOTA_CREDITO = 1;
            break;
        case _SI_BIEN_DE_USO:
            SI_BIEN_DE_USO = 1;
            break;
        case _SI_EXENTO:
            SI_EXENTO = 1;
            HAY_EXENTO = 0;
			articulo_exento = NO;
            if( !FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
                if( art_fact[renglon].exento && !EXENTO_COMO_GRAVADO( OBTENER_COD_GRAVADO() )
                 || EXENTO_PARA_DIPLOMATICOS() ) {
                    HAY_EXENTO = 1;
					articulo_exento = SI;
                }
            }
            else {
                if( articulo.gravado == 'N'
                 || ( articulo.gravado == 'E' && !EXENTO_COMO_GRAVADO( 2 ) ) ) {
                    HAY_EXENTO = 1;
					articulo_exento = SI;
                }
            }
            break;
        case _SI_GRAVADO:
            SI_GRAVADO = 1;
            HAY_GRAVADO = 0;
			articulo_gravado = NO;
            if( !FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
                if( !( art_fact[renglon].exento && !EXENTO_COMO_GRAVADO( OBTENER_COD_GRAVADO() )
                    || EXENTO_PARA_DIPLOMATICOS() ) ) {
                    HAY_GRAVADO = 1;
					articulo_gravado = SI;
                }
            }
            else {
                if( ( articulo.gravado == 'E' && EXENTO_COMO_GRAVADO( 2 ) )
                 || articulo.gravado == 'S' ) {
                    HAY_GRAVADO = 1;
					articulo_gravado = SI;
                }
            }
            break;
        case _TKT_LF:
            {
                int fila_aux;
                ind_configTmp = 0;
                fila_aux = COMP_tomar_entero( ind_config, &ind_configTmp );
                if( !imprimiendo_factura_journal ) {
                    COMP_SET_FILA( fila + fila_aux );
                }
				//imprimir = 10;
            }
            break;
        case _TKT_FOR:
            ind_configTmp = 0;
            memset( cadaux, 0, sizeof( cadaux ) );
            strncpy( cadaux, CONFIG_COMPROBANTE1[( *ind_config )].parametros,
                     sizeof( cadaux ) - 1 );
            ( ind_configTmp ) += strlen( CONFIG_COMPROBANTE1[( *ind_config )].parametros ) + 1;
            _COMP_FOR_I = ( int )RESOLVER( cadaux, NULL );
            memset( cadaux, 0, sizeof( cadaux ) );
            strncpy( cadaux, &CONFIG_COMPROBANTE1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux ) - 1 );
            (
            ind_configTmp ) += strlen(
            &CONFIG_COMPROBANTE1[( *ind_config )].parametros[ind_configTmp] ) + 1;
            _COMP_FOR_LIMITE = ( int )RESOLVER( cadaux, NULL );
            _COMP_FOR_IND_CONFIG = *ind_config;
            break;
        case _TKT_END_FOR:
            ind_configTmp = 0;
            _COMP_FOR_I += COMP_tomar_entero( ind_config, &ind_configTmp ); // Incremento de I
            if( _COMP_FOR_I <= _COMP_FOR_LIMITE ) {
                *ind_config = _COMP_FOR_IND_CONFIG;
            }
            break;
        case _TKT_SI_FORMULA:
            _SI_P_FORMULA = CONFIG_COMPROBANTE1[( *ind_config )].parametros;
            //( *ind_config ) += strlen( &CONFIG_COMPROBANTE[( *ind_config )] ) + 1;
            break;
        case _TKT_FORMULA:
            ind_configTmp = 0;
            strncpy( xx, CONFIG_COMPROBANTE1[( *ind_config )].parametros, sizeof( xx ) - 1 );
            ( ind_configTmp ) += strlen( CONFIG_COMPROBANTE1[( *ind_config )].parametros ) + 1;
            strncpy( cadaux, &CONFIG_COMPROBANTE1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux ) - 1 );
            (
            ind_configTmp ) += strlen(
            &CONFIG_COMPROBANTE1[( *ind_config )].parametros[ind_configTmp] ) + 1;
            strncpy( cadaux2, &CONFIG_COMPROBANTE1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux2 ) - 1 );
            //( *ind_config ) += strlen( &CONFIG_COMPROBANTE[( *ind_config )] ) + 1;

            importe_total = RESOLVER( xx, NULL );

            //if( cadaux2[0] ) {
                if( cadaux[0] ) {
                    FORMAT_FLOTANTE( cadaux, importe_total, xx );         
                    MOSTRAR_CADENA( columna, PRN, xx );
                }
                else {
					sprintf( xx, "%f", importe_total );
					RESOLVER_FUNCIONES( cadaux2, xx, &CONFIG_COMPROBANTE1[( *ind_config )].linea );
					//MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, SI, NO, importe_total );
                }
            //}
            break;
        case _TKT_VARIABLE_RNV:
            ind_configTmp = 0;
            cod_var = ( INT16 )COMP_tomar_entero( ind_config, &ind_configTmp );
            cod_sub = ( INT16 )COMP_tomar_entero( ind_config, &ind_configTmp );
            // esto todavia no estaria implementado, serviria solo para imprimir una variable comun.
            //if( cod_sub == 254 ) cod_sub = _TKT_FOR_I;
            if( DD_TIPO( cod_var ) == 4/*_DOUBLE*/ ) {
                char aux[20];
                memset( aux, 0, 20 );
                CARGAR_VARIABLE_RNV( cod_var, aux, 20, cod_sub, NULL );
                FTOC( strtod( aux, NULL ), aux, ENTEROS + 1, DECIMALES, SI, SI );
                //MOSTRAR_CADENA_ALD( tkt_columna, PRN, aux, 14 );
                MOSTRAR_CADENA( columna, PRN, aux );
            }
            else {
                MOSTRAR_VARIABLE_RNV( cod_var, cod_sub, columna, NULL );
            }
            break;
        case _COD_CLIENTE_GENERICO:
            IMPRIMIR_CADENA( columna, clientes.valor_grupo_cliente );
            break;
        case _DNI_CLIENTE:
            {
                char aux[50];
                sprintf( aux, "%ld", clientes.nro_documento );
                IMPRIMIR_CADENA( columna, aux );
            }
            break;  
        case _CONFIRMAR_IMPRESION:
            CONFIRMAR_IMPRESION = 1;
            break;
        case _DATOS_ADICIONALES_COMPROB:
			{
				int rta = 1;
				ind_configTmp = 0;
				cod_sol = COMP_tomar_entero( ind_config, &ind_configTmp );
				do{
					if( !imprimiendo_factura_journal ) {
						//PAGOS_PEDIR_DATOS_ADICIONALES( _hoja_actual, _nro_dato++, ( int ) cod_sol , NULL, 1,  NULL, 0, D_VALIDACIONES );
						rta = PAGOS_PEDIR_DATOS_ADICIONALES( _hoja_actual, _nro_dato++, ( int )cod_sol, NULL, D_VALIDACIONES,
													   NULL, 0, NULL, NULL , '\0', '\0');
					}
					//si aca ponen en !rta queda en bucle cerrado ya que datos adicionales no devuelve cuando no tiene..ojo
				}while( rta );
			}
            break;
        case _PROCESAR_DATOS_ADICIONALES:
            ind_configTmp = 0;
            cod_sol = COMP_tomar_entero( ind_config, &ind_configTmp );
            nro_orden = COMP_tomar_entero( ind_config, &ind_configTmp );
            modalidad = COMP_tomar_entero( ind_config, &ind_configTmp );   //ver si es antes de confirmar la impresion o despuï¿½      
            if( !imprimiendo_factura_journal ) {
                CALCULAR_MAXIMO_DATOS_ADICIONALES_MEDIO( cod_sol, nro_orden, RAM_ID_ULTIMO_EVENTO,
                                                         modalidad, D_VALIDACIONES, 0,
                                                         _hoja_actual );
            }
            else {
                CALCULAR_MAXIMO_DATOS_ADICIONALES_MEDIO( cod_sol, nro_orden, RAM_ID_ULTIMO_EVENTO,
                                                         0, D_VALIDACIONES, 0, _hoja_actual + 1 );
            }
            break;
        case _DATO_ADICIONAL_MEDIO_PAGO:
            ind_configTmp = 0;
            memset( xx, 0, sizeof( xx ) );
            posicion = COMP_tomar_entero( ind_config, &ind_configTmp );
            if( posicion == 254 ) {
                posicion = _COMP_FOR_I;
            }
            if( !imprimiendo_factura_journal ) {
                if( BUSCAR_DATO_ADICIONAL_MEDIO_PAGO( posicion, xx ) ) {
                    MOSTRAR_CADENA( columna, PRN, xx );
                }
            }
            else {
                if( BUSCAR_DATO_ADICIONAL_MEDIO_PAGO( _hoja_actual - 1, xx ) ) {
                    MOSTRAR_CADENA( columna, PRN, xx );
                }
            }
            break;
		case _TKT_SLIP_ON:
            PRN_HABILITA_SLIP( 1, NO );
			LF( 1 );	
			setData( ACTION_PRINT, NULL, 0 );
            break;
        case _TKT_SLIP_OFF:
            LF( 1 );
            setData( ACTION_PRINT, NULL, 0 );
            EJECT();
            LF( 1 );
            setData( ACTION_PRINT, NULL, 0 );
            PRN_INHABILITA_SLIP();
            break;        
		case _TKT_VALIDACION:
			{
			int validacion = 0;
			ind_configTmp = 0;
			validacion = COMP_tomar_entero( ind_config, &ind_configTmp );
			VALIDAR( validacion, 0, 0, 0, 0 );
			}
		break;
		case _TKT_VALIDACION_AL_FINAL:
			{
			ind_configTmp = 0;
			COD_VALIDACION_AL_FINAL = COMP_tomar_entero( ind_config, &ind_configTmp );
			}
			break;
        default:
            if( reconocida == 0 || (codigo > _FIN || codigo <  0) ){
                char log_tmp[65];

                memset(log_tmp,0,sizeof(log_tmp));
                _snprintf( log_tmp, sizeof( log_tmp ), "COMP_ejecutar_comando: Codigo, no reconocido: %x", codigo );
                GRABAR_LOG_SISTEMA( log_tmp , LOG_ERRORES,2);
            }
            break;
    }

    /*#if defined( INVEL_W ) || defined( INVEL_L )
    imprimir++;
    if( imprimir >= 10 ) {
        LF( 0 );
        setData( ACTION_PRINT, NULL, 0 );
        imprimir = 0;
    }
    #endif
    //#endif */
}

/***************************************************************************/
INT16  COMP_tomar_entero( int *ind_config, int *indice )
/***************************************************************************/
{
    INT16  valor;
    //valor = *( int* )&CONFIG_COMPROBANTE1[*ind_config].parametros[*indice];
    valor = *( INT16* )&CONFIG_COMPROBANTE1[*ind_config].parametros[*indice];
    ( *indice ) += 2;
    return ( valor );
}

/***************************************************************************/
void EJECT()
/***************************************************************************/
{
    #if defined( INVEL_W) || defined( INVEL_L)
    PRN_SELECT( SLIP );
    MOSTRAR_CADENA( 0, PRN, "!|e" );
    #else
    LF( 1 );
    MOSTRAR_CADENA( columna, PRN, "\x0c" );
    #endif
}
/***************************************************************************/
void COMP_SET_FILA( int _fila )
/***************************************************************************/
{
    if( _fila > fila ) {
        LF( _fila - fila );
        fila = _fila;
    }
    fila = _fila;
}
/***************************************************************************/
int EMITIR_FACTURA()
/***************************************************************************/
{
    int rta = 0;

    rta = ( ( RAM_FACTURA || RAM_NOTA_CR ) && !EMITIR_FACTURA_REMOTA() ) ? 1 : 0;
    
	if( IMPRESORA_FISCAL == FISCAL && ( RAM_FACTURA || RAM_NOTA_CR ) ) {
        rta = 1;
    }

    return ( rta );
}
/***************************************************************************/
int EMITIR_FACTURA_REMOTA()
/***************************************************************************/
{
    int rta;
    rta = RAM_FACTURA && !RAM_NOTA_CR
       && ( ( TIPO_FACTURA_A == REMOTO && ES_FACTURA_A( clientes.cond_iva ) )
         || ( TIPO_FACTURA_B == REMOTO && ES_FACTURA_B() ) );
    if( !rta ) {
        rta = RAM_FACTURA && RAM_NOTA_CR
           && ( ( TIPO_NC_A == REMOTO && ES_FACTURA_A( clientes.cond_iva ) )
             || ( TIPO_NC_B == REMOTO && ES_FACTURA_B() ) );
    }
    return ( rta );
}
/***************************************************************************/
int ES_FACTURA_B()
/***************************************************************************/
{
    int rta = 0;
    if( FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
        rta = ( RAM_FACTURA == FACTURA_TIPO_B );
    }
    else {
        rta = ( clientes.cond_iva > 2 ? 1 : 0 );
    }
    return rta;
}

/**************************************************************************/
int TIPO_COMPROBANTE( int *subtipo )
/**************************************************************************/
{
    int tipo = 0;

    switch ( RAM_NOTA_CR ) {
      case 1:
         tipo = _NOTA_CREDITO;
         *subtipo = 1;
         break; //NOTA DEL MERGE: ESTE BREAK FALTABA, PORQUE?
      case 2:
         tipo = _NOTA_CREDITO;
         *subtipo = 2;
         break;
      default:
         switch ( RAM_FACTURA  ) {
            case 1:
               if( FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
                tipo = _CREDITO_FISCAL;
               } else {
                tipo = _FACTURA;
               }
					*subtipo = 1;
               break;
            case 2:
               if( FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
					tipo = _FACTURA_CONS_F;
               } else {
					tipo = _FACTURA;
               }
					*subtipo = 2;
               break;
            default:
               switch ( RAM_TICKET_FACTURA  ) {
                  case 1:
                     tipo = _TICKET_FACTURA;
                     *subtipo = 1;
                     break;
                  case 2:
                     tipo = _TICKET_FACTURA;
                     *subtipo = 2;
                     break;
                  default:
                    *subtipo = 0;
                    if( NRO_REMITO ) {
                        tipo = _REMITO;
                    } else {
                        tipo = _TICKET;
                    }
               }
         }
   }

    return tipo;
}

/**************************************************************************/
int SETEAR_TIPO_COMPROBANTE( int tipo_comprobante, int subtipo )
/**************************************************************************/
{
    int tipo = 0;

	switch( tipo_comprobante ) {
		case _FACTURA:
			SET_MEMORY_CHAR( __ram_factura, subtipo );
			break;
		case _NOTA_CREDITO:
			SET_MEMORY_CHAR( __ram_factura, subtipo );
			break;
		case _TICKET_FACTURA:
			SET_MEMORY_CHAR (__ram_ticket_factura, subtipo );
			break;
	}

    return tipo;
}

/**************************************************************************/
int VALIDAR_CLIENTE_PARA_TIPO_COMPROBANTE(  )
/**************************************************************************/
{
	int cond_iva = 0, tipo_comp_hab = 0, tipo_comprob_actual = 0, subtipo = 0;

	if( !FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
		cond_iva = OBTENER_COND_IVA_CLIENTE( 0 );
		if( cond_iva ) {
			if( cond_iva < 2 ) {
				tipo_comp_hab = FACTURA_TIPO_A;
			} else {
				tipo_comp_hab = FACTURA_TIPO_B;
			}
			tipo_comprob_actual =  TIPO_COMPROBANTE( &subtipo );
			if( ( tipo_comp_hab != subtipo ) && ( tipo_comprob_actual != _TICKET ) ) {
				UNGETCH( -999 );
				MENSAJE_TECLA("EL CLIENTE CARGADO MODIFICA EL TIPO DE COMPROBANTE, PRESIONE TECLA");
				SETEAR_TIPO_COMPROBANTE( tipo_comprob_actual, tipo_comp_hab );
			}
		}
	}

	MUESTRA_PERFIL();
	MUESTRA_COMPROBANTE( );
}

/**************************************************************************/
int VALIDAR_CAMBIO_DE_CLIENTE_PARA_COMPROBANTE(  long nro_cliente, int operacion )
/**************************************************************************/
{
	int cond_iva_cli_actual = 0, tipo_comprob_actual = 0, tipo_comprob_ant = 0, ok = 1,
		cond_iva_cli_ant = 0, tabla_ant = 0, tipo_ant = 0, validar = 1;
	long cliente = 0;

	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

	if( !FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
		if( operacion == _NC_LIBRE || operacion == _DEVOL_LIBRE ) {
			if( RAM_TIPO_EVENTO_COMP_ORIGINAL == 35 // TICKET_FACTURA A y FACTURA A
					|| RAM_TIPO_EVENTO_COMP_ORIGINAL == 3 ) {
				cond_iva_cli_ant = RESP_INSC; 
			} else {
				cond_iva_cli_ant = RESP_NO_INSC; 
			}
			//Si NO se ingreso el tipo de comprobante referenciado y solo se ingreso
			//los datos solicitados en PEDIR_CAJA_Y_SECUENCIA
			//no se validara el cambio de cliente.
			if( RAM_TIPO_EVENTO_COMP_ORIGINAL < 0 && PEDIR_CAJA_Y_SECUENCIA ) {
				validar = 0;
			}
		} else {
			cond_iva_cli_ant = OBTENER_COND_IVA_CLIENTE( nro_cliente );//Condicion de iva del cliente anterior
		}

		if( validar ) {
			cond_iva_cli_actual = OBTENER_COND_IVA_CLIENTE( 0 );//Condicion de iva del cliente actual
			tipo_comprob_ant = TIPO_COMPROBANTE_DEPENDIENTE_DEL_CLIENTE(  cond_iva_cli_ant );
			tipo_comprob_actual = TIPO_COMPROBANTE_DEPENDIENTE_DEL_CLIENTE(  cond_iva_cli_actual );
			
			if( tipo_comprob_actual != tipo_comprob_ant ) {
				MENSAJE_TECLA( ST( S_NO_SE_PUEDE_CAMBIAR_CONDICION_FISCAL_CLIENTE_Pres_Tecla ) );
				ok = 0;
			} else {
				//Me posiciono en el cliente actual.
				cliente = RAM_NRO_CLIENTE;
				SELECT_TABLE( T_CLIENTES, TT_ORIG );
				SET_WHERE("COD_CLIENTE='%d'", cliente );
				RUN_QUERY(NO);
			}
		}
	}
	SELECT_TABLE( tabla_ant, tipo_ant );

	return( ok );
}

/**************************************************************************/
int TIPO_COMPROBANTE_DEPENDIENTE_DEL_CLIENTE(  int cond_iva_cli )
/**************************************************************************/
{
	int tipo_comp_habilitado = 0;

	if( !FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
		if( cond_iva_cli == 1 ) {
			tipo_comp_habilitado = TIPO_A;
		} else {
			tipo_comp_habilitado = TIPO_B;
		}
	}

	return( tipo_comp_habilitado );
}

/***************************************************************************/
void REIMPRIMIR_COMPROBANTE_NC_O_FACTURA( void )
/***************************************************************************/
{
    POSICIONA_EN_COMPROBANTE();
    if( ES_COMPROBANTE( _FACTURA ) ) {
        REIMPRIMIR_COMPROBANTE( _FACTURA );
    }
    else if( ES_COMPROBANTE( _NOTA_CREDITO ) ) {
        REIMPRIMIR_COMPROBANTE( _NOTA_CREDITO );
    }
    else {
        MENSAJE_STRING( S_COMPROBANTE_INVALIDO );
    }
}
/***************************************************************************/
void REIMPRIMIR_COMPROBANTE( int tipo )
/***************************************************************************/
{
    long act_ram_id_evento = 0;
	double nro_anterior;
    unsigned evento_anterior;
    char mensaje[80];
    int rta;
    REIMPRESION = 1;
    /*--------------- Decide si es factura comun o de remitos --------------*/
    if( tipo == _FACTURA ) {
        POSICIONA_EN_COMPROBANTE();
        if( ES_COMPROBANTE( _FAC_REM ) ) {
            tipo = _FAC_REM;
        }
    }
    SET_MEMORY_CHAR( __ram_factura, 1 );
    if( ES_COMPROBANTE( _NOTA_CREDITO ) ) {
        SET_MEMORY_CHAR( __ram_nota_cr, 1 );
    }
    else {
        SET_MEMORY_CHAR( __ram_nota_cr, 0 );
    }
    sprintf( mensaje, ST( S_REIMPRESION_DE__S ), TIPO_COMP[tipo] );
    DIBUJAR_RECUADRO_PETICION( mensaje, L_CODIGO );
    if( SOLICITAR_CLAVES( 30, 21, NO, SI, PEDIR_CLAVE_EN_REIMPRESION, "CLAVE", NULL,
                          RAM_COD_FUNCION, SI ) ) {
        if( VERIFICAR_EVENTOS_DEL_COMPROBANTE( tipo ) ) {
            /*------------------------- Busca el cliente ---------------------------*/
            POSICIONA_EN_COMPROBANTE();
            SET_MEMORY_LONG( __ram_nro_cliente, event_ticket.nro_cliente );
            EXISTE_CLIENTE( RAM_NRO_CLIENTE );
            SET_MEMORY_CHAR( __ram_bien_de_uso, ( event_ticket.bien_de_uso == 'S' ) ? 1 : 0 );
            SET_MEMORY_LONG( __ram_boleta_anterior, event_ticket.nro_boleta_anterior );
            SET_MEMORY_INT( __ram_sucursal_anterior, event_ticket.nro_sucursal_anterior );
            SET_MEMORY_INT( __ram_caja_anterior, event_ticket.nro_caja_anterior );
            SET_MEMORY_INT( __ram_fecha_anterior, event_ticket.fecha_anterior );
            SET_MEMORY_CHAR( __ram_tipo_comp_anterior, event_ticket.tipo_comprobante_anterior );
            /*--------------------------- Toma los datos ---------------------------*/
            DATOS_COMPROBANTE( tipo );
            comp.tipo_comp = tipo;
            comp.cond_cli = clientes.cond_iva;
            comp.subtipo = ( ES_FACTURA_A( comp.cond_cli ) ) ? 1 : 2;
            rta = OBTENER_DATOS_COMPROBANTE( tipo, comp.subtipo, 0, &comp.nro_suc, &comp.nro_comp,
                                             &comp.copias, SI, NO );
            if( rta ) {
                /*-------------------- Modifica el evento anterior ---------------------*/
                nro_anterior = event_ticket.nro_comprobante_new;
                evento_anterior = event_ticket.tipo_evento;
                act_ram_id_evento = event_ticket.id_evento;
                if( config.pais != CHILE && config.pais != BRASIL ) {
                    //SELECT( AREA_TICKET );
					SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
                    event_ticket.nro_comprobante_new = comp.nro_comp;
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
                    UPDATE2();
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                   /* if( UTILIZA_DBROUTER ) {
                        char where[40];
                        char patron_nulls[50];
                        memset( patron_nulls, 0, 50 );
                        patron_nulls[18] = 1;   // ticket->nro_comprobante
                        sprintf( where, "caja_z = %ld AND id_evento = %ld", event_ticket.caja_z,
                                 event_ticket.id_evento );
                        // AgregarCodCaja
                        DBROUTER_REQ_CON_NULL( ACTUALIZAR_REGISTRO, 60, patron_nulls, "eventos",
                                               ( char* )&event_ticket,
                                               sizeof( event_ticket )
                                             - sizeof( event_ticket.reserva ), where, 0 );
                    }*/
                    #ifdef COMPILAR_EVENTOS_ALL
                    if( event_ticket.id_all ) {
                        SELECT( AREA_TICKET_ALL );
                        GET_EQUAL( ( char* )&event_ticket.id_all );
                        event_ticket_all.nro_comprobante = comp.nro_comp;
                        #ifdef INVEL_L
                        BEGIN_TRANSACTION();
                        #endif
                        UPDATE();
                        #ifdef INVEL_L
                        END_TRANSACTION();
                        #endif
                    }
                    #endif
                }
                /*-------------------------- Imprime y graba ---------------------------*/
                if( IMPRIMIR_COMPROBANTE( tipo, SI, NO ) ) {
                    GRABA_EVENTO_ANULACION( nro_anterior, evento_anterior, comp.nro_comp,
                                            act_ram_id_evento );
                    sprintf( mensaje, "Reimpresin de %s", TIPO_COMP[tipo] );
                    GRABAR_LOG_SISTEMA( mensaje , LOG_VENTAS,2);
                    VALIDAR_REIMPRESION_FACTURA( event_ticket.nro_comprobante_new,
                                                 clientes.cod_cliente );
                }
                if( !NRO_CLIENTE_POR_DEFECTO ) {
                    SET_MEMORY_LONG( __ram_nro_cliente, 0L );
                }
            }
            else {
                MENSAJE_STRING( S_ERROR_LEER_DATOS_PARA_REIMPRIMIR_COMPROBANTE );
            }
        }
    }
    if( !NC_PERMANENTE && !MODO_FACTURACION_POR_DEFECTO ) {
        SET_MEMORY_CHAR( __ram_factura, 0 );
        SET_MEMORY_CHAR( __ram_nota_cr, 0 );
    }
    SET_MEMORY_CHAR( __ram_bien_de_uso, 0 );
    SET_MEMORY_LONG( __ram_boleta_anterior, 0 );
    SET_MEMORY_INT( __ram_sucursal_anterior, 0 );
    SET_MEMORY_INT( __ram_caja_anterior, 0 );
    SET_MEMORY_INT( __ram_fecha_anterior, 0 );
    SET_MEMORY_CHAR( __ram_tipo_comp_anterior, 0 );
    MOSTRAR_MODO_BIEN_DE_USO_EN_PANTALLA();
    MUESTRA_COMPROBANTE();
}
/*****************************************************************************/
void GRABA_EVENTO_ANULACION( double nro, unsigned evento, double nuevo_nro, long act_ram_id_evento )
/*****************************************************************************/
{
    long id_anterior,pos;
    int _p,_h,_estado;
    struct _eventos copia_eventos;
    OPEN_RECUPERO( &_p, &_h );
    //  _estado = STACK_INS( sizeof( int ) );
    _estado = STACK_INS( sizeof( short ) );
    if( _ESTADO == 0 ) {
        id_anterior = act_ram_id_evento;
        _SET_ESTADO( 1 );
    }
    if( _ESTADO == 1 ) {
        if( config.pais == CHILE || config.pais == BRASIL ) {
            GRABA_EVENTO_COMPROBANTE_ANULADO( nro, id_anterior );
        }
        _SET_ESTADO( 11 );
    }
    if( _ESTADO == 11 ) {
        if( config.pais != CHILE && config.pais != BRASIL ) {
            event_ticket.nro_comprobante_new = nro;
            event_ticket.bultos = evento;
            event_ticket.tipo_evento = _EVENT_ANULA_COMPROBANTE;
        }
        else {
            //SELECT( AREA_TICKET );
            //SET_ORDER( 1 );
            //GO( TOP );
            //GET_EQUAL( ( char* )&id_anterior );
			SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
			SET_WHERE("ID_EVENTO = '%ld'",id_anterior);
			SET_ORDER2("ID_EVENTO");
			RUN_QUERY(NO);
            if( FOUND2() ) {
                memcpy( &copia_eventos, &event_ticket, sizeof( copia_eventos ) );
            }
            memcpy( &event_ticket, &copia_eventos, sizeof( event_ticket ) );
            event_ticket.nro_comprobante_new = nuevo_nro;
            event_ticket.tipo_evento = _EVENT_REIMPRESION_COMPROBANTE;
        }
        GRABAR_EVENT_TICKET( SI );
        _SET_ESTADO( 2 );
    }
    if( _ESTADO == 2 ) {
        //SELECT( AREA_E_TICKET );
        //GET_EQUAL( ( char* )&id_anterior );
		SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
		SET_WHERE("ID_EVENTO = '%ld'",id_anterior);
		SET_ORDER2("ID_EVENTO");
		RUN_QUERY(NO);
        while( !dbEOF() && event_e_ticket.id_evento == id_anterior ) {
            pos = GET_POSITION_REG();
            event_e_ticket.id_evento = RAM_ID_EVENTO;
            #ifdef INVEL_L
            //BEGIN_TRANSACTION();
            #endif
            INSERT2(NO);
            #ifdef INVEL_L
            //END_TRANSACTION();
            #endif
            GET_DIRECT_REG(pos, 0);
            //GO2( pos );
            SKIP2( 1 );
        }
        if( config.pais == CHILE || config.pais == BRASIL ) {
            _SET_ESTADO( 31 );
        }
        else {
            _SET_ESTADO( 3 );
        }
    }
    if( _ESTADO == 31 ) {
        //SELECT( AREA_PAGO );
        //GET_EQUAL( ( char* )&id_anterior );
		SELECT_TABLE( T_PAGOS, tipo_t_pagos );
		SET_WHERE("ID_EVENTO = '%ld'",id_anterior);
		SET_ORDER2("ID_EVENTO");
		RUN_QUERY(NO);
        while( !dbEOF() && event_pago.id_evento == id_anterior ) {
            pos = GET_POSITION_REG();
            event_pago.id_evento = RAM_ID_EVENTO;
            #ifdef INVEL_L
            //BEGIN_TRANSACTION();
            #endif
            INSERT2(NO);
            #ifdef INVEL_L
            //END_TRANSACTION();
            #endif
            //GO2( pos );
            GET_DIRECT_REG( pos, 0 );
            SKIP2( 1 );
        }
        _SET_ESTADO( 32 );
    }
    if( _ESTADO == 32 ) {
        //SELECT( AREA_FACTURA );
        SELECT_TABLE( T_EV_FACTU, tipo_t_ev_factu );
        //GET_EQUAL( ( char* )&id_anterior );
        SET_WHERE( "ID_EVENTO = '%ld'", id_anterior );
        SET_ORDER2( "ID_EVENTO" );
        RUN_QUERY( NO );    
        while( /*!BtrvEOF()*/!dbEOF() && event_factura->id_evento == id_anterior ) {
            //pos = GET_POSITION();
            pos = GET_POSITION_REG();
            event_factura->id_evento = RAM_ID_EVENTO;
            event_factura->nro_comprobante_new = nuevo_nro;
			{//Controlar el tamaño del campo nro_comprobante_new
				char nro[12];//No debe superar los 10 digitos porque es lo maximo que el campo del servidor acepta
				memset(nro,0,sizeof(nro));
				_snprintf( nro, sizeof(nro)-2,"%lf",event_ticket.nro_comprobante_new );
				event_ticket.nro_comprobante_new = atof(nro);
			}
            //event_factura->nro_orden++;
            nuevo_nro++;
            #ifdef INVEL_L
            //BEGIN_TRANSACTION();
            #endif
            INSERT2( NO );
            #ifdef INVEL_L
            //END_TRANSACTION();
            #endif
            //GET_DIRECT( pos );
            //SKIP( 1 );
            GET_DIRECT_REG( pos, 0 );
            SKIP2( 1 );
        }
        _SET_ESTADO( 3 );
    }
    if( _ESTADO == 3 ) {
        ACTUALIZA_ID_EVENTO_NEW();
        _SET_ESTADO( 4 );
    }
    CLOSE_RECUPERO( _p, _h );
}
/***************************************************************************/
void COMP_total_en_letras( int *ind_config )
/***************************************************************************/
{
    char p_cad[200],*p_cad2;
    int largo,col_siguiente,l_cad,i;
    double total_factura = 0.00;
    int ind_configTmp = 0;
    /*---------------------- Inicializaciones ---------------------*/
    largo = COMP_tomar_entero( ind_config, &ind_configTmp );
    col_siguiente = COMP_tomar_entero( ind_config, &ind_configTmp );
    /*------------------ Transformacion a letras ------------------*/
    total_factura = SUMAR_TOTAL_FAC_CAJA( NO) + ROUND( RAM_RETENCION_DGR, ENTEROS, DECIMALES );
    NUM_A_LETRAS( fabs( total_factura/*total*/ ), p_cad );
    l_cad = strlen( p_cad );

    if( l_cad <= largo ) {
        MOSTRAR_CADENA( columna, PRN, p_cad );
    }
    else {
        for( i = largo - 1;i > 0 && p_cad[i] != ' ';i-- ) {
            ;
        }
        p_cad2 = &p_cad[i + 1];
        p_cad[i] = 0;
        MOSTRAR_CADENA( columna, PRN, p_cad );
        COMP_SET_FILA( fila + 1 );
        MOSTRAR_CADENA( col_siguiente, PRN, p_cad2 );
    }
}/***************************************************************************/
int VERIFICAR_EVENTOS_DEL_COMPROBANTE( int tipo)
/***************************************************************************/
{
	return _VERIFICAR_EVENTOS_DEL_COMPROBANTE( tipo, &cant_pagos , &cant_renglones);
}
/***************************************************************************/
int _VERIFICAR_EVENTOS_DEL_COMPROBANTE( int tipo, int *cantidad_pagos , int *cant_items)
/***************************************************************************/
{
    int rta = 0;
    /*------------------ Ultimo registro de ticket --------------------*/
    POSICIONA_EN_COMPROBANTE();
    if( ES_COMPROBANTE( tipo ) ) {
        /*---------------- Obtenemos cantidad de renglones ---------------*/
        //SELECT( AREA_E_TICKET );
        //GET_EQUAL( ( char* )&event_ticket.id_evento );
		SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
		SET_WHERE("ID_EVENTO = '%ld'",event_ticket.id_evento);
		SET_ORDER2("ID_EVENTO");
		RUN_QUERY(NO);
        *cant_items = 0;
        while( event_e_ticket.id_evento == event_ticket.id_evento && /*!BtrvEOF()*/ !dbEOF() ) {
            *cant_items+=1;
            SKIP2( 1 );
        }
        /*---------------- Obtenemos cantidad de pagos ---------------*/
        //SELECT( AREA_PAGO );
        //GET_EQUAL( ( char* )&event_ticket.id_evento );
		SELECT_TABLE( T_PAGOS, tipo_t_pagos );
		SET_WHERE("ID_EVENTO = '%ld'",event_ticket.id_evento);
		SET_ORDER2("ID_EVENTO");
		RUN_QUERY(NO);
        *cantidad_pagos = 0;
        while( event_pago.id_evento == event_ticket.id_evento && !dbEOF() ) {
            cantidad_pagos+=1;
            SKIP2( 1 );
        }
        /*------------ Controla que se hayan encontrado los datos ---------*/
        if( *cant_items != 0 ) {
            rta = 1;
        }
        if( tipo != _REMITO && !RAM_NOTA_CR && cantidad_pagos == 0 ) {
            rta = 0;
        }
        if( !IMPRESORA_FISCAL ) {
            if( RAM_CAMBIO && cantidad_pagos == 0 ) {
                 rta = 1;
            }
        }
    }
    if( !rta ) {
        MENSAJE_STRING( S_NO_SE_ENCUENTRAN_DATOS_PARA_EMITIR_COMPROBANTE );
    }
    return ( rta );
}
/*****************************************************************************/
int ES_COMPROBANTE( int tipo )
/*****************************************************************************/
{
    int rta = 0;
    switch( tipo ) {
        case _NOTA_CREDITO:
            if( event_ticket.tipo_evento >= 5 && event_ticket.tipo_evento <= 6 ) {
                rta = 1;
            }
            break;
        case _FACTURA:
            if( event_ticket.tipo_evento >= 3 && event_ticket.tipo_evento <= 4 
				|| ( event_ticket.tipo_evento == 21 && ( ( event_ticket.tipo_evento_importado >= 3 
				  && event_ticket.tipo_evento_importado <= 4 )  
				  || ( RAM_MODO_DEVOLUCION == _DEVOL_LIBRE  && RAM_FACTURA ) ) ) ) {
                    //Devuelvo el tipo de factura, A o B.
                    switch( event_ticket.tipo_evento ) {
                        case 3 :
                            rta = FACTURA_TIPO_A;
                            break;
                        case 4 :
                            rta = FACTURA_TIPO_B;
                            break;
					}
					if(event_ticket.tipo_evento ==21)
					{
						//Para Devoluciones Referenciadas
						switch( event_ticket.tipo_evento_importado ) {
							case 3 :
								rta = FACTURA_TIPO_A;
								break;
							case 4 :
								rta = FACTURA_TIPO_B;
								break;
							default:
								//En las Devoluciones Libre, el comprobante seleccionado indica 
								//el tipo de comprobante que se quiere procesar
								if( RAM_MODO_DEVOLUCION == _DEVOL_LIBRE ){
									switch( RAM_FACTURA ) {
										case 1 :
											rta = FACTURA_TIPO_A;
											break;
										case 2 :
											rta = FACTURA_TIPO_B;
											break;
									}
								}
								break;
						}
					}
                /*}
                else {
                    rta = 1;
                }*/
            }
            break;
        case _REMITO:
            if( event_ticket.tipo_evento == 11 ) {
                rta = 1;
            }
            break;
        case _FAC_REM:
            if( event_ticket.tipo_evento == 14 || event_ticket.tipo_evento == 15 ) {
                rta = 1;
            }
            break;
    }

    return ( rta );
}
/*****************************************************************************/
void MOSTRAR_NRO_FACTURA()
/*****************************************************************************/
{
    char letra;
    short int subtipo,suc = 0;
    long nro = 0;
    int rta = 0;

    #if defined(INVEL_W) || defined(INVEL_L)
    if( EMITIR_FACTURA() && NRO_FACTURA == LOCAL_FAC ) {
        if( ES_FACTURA_A( clientes.cond_iva ) ) {
            letra = 'A';
            subtipo = 1;
        }
        else {
            letra = 'B';
            subtipo = 2;
        }
        if( IMPRESORA_FISCAL ) {
            letra = ' ';
        }
        rta = OBTENER_DATOS_COMPROBANTE( _FACTURA, subtipo, 0, &suc, &nro, NULL, NO, NO );
        if( rta ) {
            char cad[30];  
            if( fac_on /*&& nro != nro_ant*/ ) {
                sprintf( cad, "FAC %c %04i-%08li", letra, suc, nro );
                ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_NRO_FACTURA_BORRAR, " ", " ", NULL, " " );
                ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_NRO_FACTURA_AGREGAR, cad, " ", NULL, " " );
                nro_ant = nro;
            } else if( !fac_on ) {
                sprintf( cad, "FAC %c %04i-%08li", letra, suc, nro );
                ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_NRO_FACTURA_AGREGAR, cad, " ", NULL, " " );
                fac_on = 1;
                nro_ant = nro;
            }
        } else {
            if( fac_on ) {
                //se puede dar cuando volvemos a la pantalla de ventas
                ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_NRO_FACTURA_BORRAR, " ", " ", NULL, " " );
                fac_on = 0;
            }
            MENSAJE_STRING( S_ERROR_AL_LEER_DATOS_PARA_MOSTRAR_NRO_FACTURA );
        }
    } else{
        if( fac_on ) {
            // si estamos en un ticket y el documento anterior fue una factura
            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_NRO_FACTURA_BORRAR, " ", " ", NULL, " " );
            fac_on = 0;
        }
    }
    #else
    char buffer[30];
    if( EMITIR_FACTURA() && NRO_FACTURA == LOCAL_FAC ) {
        if( ES_FACTURA_A( clientes.cond_iva ) ) {
            letra = 'A';
            subtipo = 1;
        }
        else {
            letra = 'B';
            subtipo = 2;
        }
        if( IMPRESORA_FISCAL ) {
            letra = ' ';
        }
        rta = OBTENER_DATOS_COMPROBANTE( _FACTURA, subtipo, 0, &suc, &nro, NULL, NO, NO );
        if( rta ) {
            sprintf( buffer, "FAC %c %04i-%08li", letra, suc, nro );
            _ATRIBUTO = 15;
            MOSTRAR_CADENA( 2, 21, buffer );
        }
        else {
            MENSAJE_STRING( S_ERROR_AL_LEER_DATOS_PARA_MOSTRAR_NRO_FACTURA );
        }
    }
    #endif
}
/*****************************************************************************/
int MODO_FACTURACION_ESPECIAL()
/*****************************************************************************/
{
    int rta = 0;
    if( MODO_FACTURACION == 1 && EMITIR_FACTURA() && ES_FACTURA_A( clientes.cond_iva ) ) {
        rta = 1;
    }
    return ( rta );
}
/***************************************************************************/
int COMP_evaluar_condiciones( int codigo )
/***************************************************************************/
{
    int rta;
    rta = codigo;
    if( SI_FACTURA_A ) {
        SI_FACTURA_A = 0;
        if( FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
            rta = ( RAM_FACTURA == 1 ) ? 1 : 0;
        }
        else {
            if( comp.cond_cli >= 3 ) {
                rta = 0;
            }
        }
    }
    if( SI_FACTURA_B ) {
        SI_FACTURA_B = 0;
        if( ES_FACTURA_A( comp.cond_cli ) ) {
            rta = 0;
        }
    }
    if( SI_NOTA_CREDITO ) {
        SI_NOTA_CREDITO = 0;
        if( comp.tipo_comp != _NOTA_CREDITO ) {
            rta = 0;
        }
    }
    if( SI_BIEN_DE_USO ) {
        SI_BIEN_DE_USO = 0;
        if( !RAM_BIEN_DE_USO ) {
            rta = 0;
        }
    }
    if( SI_EXENTO ) {
        SI_EXENTO = 0;
        if( !HAY_EXENTO ) {
            rta = 0;
        }
    }
    if( SI_GRAVADO ) {
        SI_GRAVADO = 0;
        if( !HAY_GRAVADO ) {
            rta = 0;
        }
    }
    if( _SI_P_FORMULA ) {
        if( fabs( RESOLVER( _SI_P_FORMULA, NULL ) ) < 0.005 ) {
            rta = 0;
        }
        _SI_P_FORMULA = NULL;
    }
    /*-------- Si no se realiza la accion incrementamos el indice -------*/
    if( !rta ) {
        /*switch ( codigo ) {
            case _LARGO_FACTURA:
            case _LARGO_FACTURA_A:
            case _LARGO_FACTURA_B:
            case _RENGLONES_CUERPO:
            case _POS_X:
            case _POS_Y:
            case _TOTAL_IMPUESTO:
            case _NETO_GRAVADO_COD:
            case _TKT_TICKS_B:
            case _NRO_DOC_FISCAL_COMP:
            case _IVA1_COD:
            case _IVA2_COD:
            case _TKT_LF:
                ( *ind_config ) += 2;
                break;
            case _POSICION:
                ( *ind_config ) += 4;
                break;
            case _TKT_FORMULA:
                ( *ind_config ) += strlen( &CONFIG_COMPROBANTE[( *ind_config )] ) + 1;
                ( *ind_config ) += strlen( &CONFIG_COMPROBANTE[( *ind_config )] ) + 1;
               ( *ind_config ) += strlen( &CONFIG_COMPROBANTE[( *ind_config )] ) + 1;
                break;
            case _TKT_SI_FORMULA:
            case _CADENA:
            case _CADENA_REEMPLAZO_TICKET:
                ( *ind_config ) += strlen( &CONFIG_COMPROBANTE[( *ind_config )] ) + 1;
                break;
            case _CADENA_CONTINUA_EN_PROXIMA:
                ( *ind_config ) += 4;
                ( *ind_config ) += strlen( &CONFIG_COMPROBANTE[( *ind_config )] ) + 1;
                break;
            }*/
        //(*ind_config) ++;
    }
    return ( rta );
}
/***************************************************************************/
void IMPRIMIR_CADENA( int x, char *cadena )
/***************************************************************************/
{
    char _cadena[200];
    strncpy( _cadena, cadena, 199 );
    _cadena[199] = 0;
    COMP_ajustar_largo( _cadena );
    MOSTRAR_CADENA( x, PRN, _cadena );
}
/***************************************************************************/
void COMP_ajustar_largo( char *cadena )
/***************************************************************************/
{
    if( OFFSET_CADENA ) {
        memmove( cadena, &cadena[OFFSET_CADENA], strlen( cadena ) - OFFSET_CADENA );
        cadena[strlen( cadena ) - OFFSET_CADENA] = 0;
        OFFSET_CADENA = 0;
    }
    if( LARGO_CADENA ) {
        cadena[LARGO_CADENA] = 0;
        LARGO_CADENA = 0;
    }
}
/**************************************************************************/
void IMPRIME_RETIRAR_FACTURA( double total )
/**************************************************************************/
{
    if( IMPRIMIR_RETIRAR_FACTURA ) {
        if( IMPRIMIR_RETIRAR_FACTURA != 3 ) {
            PRN_SELECT( RECEIPT );
            IMPRIME_CABECERA_GENERAL( NRO_EVENTO - 1 );
            LF( 1 );
            if( RAM_NOTA_CR ) {
                MOSTRAR_CADENA( 1, PRN, ST( S_RETIRE_SU_NOTA_DE_CREDITO_POR__ ) );
            }
            else {
                MOSTRAR_CADENA( 1, PRN, ST( S_RETIRE_SU_FACTURA_POR__ ) );
            }
            MOSTRAR_FLOTANTE( 32, PRN, ENTEROS, DECIMALES, SI, 0, total );
            if( IMPRIMIR_RETIRAR_FACTURA == 2 ) {
                LF( 2 );
                {
                    char aux[15];
                    sprintf( aux, "%s:", ST( S_CLIENTE ) );
                    MOSTRAR_CADENA( 1, PRN, aux );
                }
                MOSTRAR_CADENA( 10, PRN, CLIENTE_AFINIDAD_NOMBRE );
                LF( 1 );
                MOSTRAR_ENTERO( 10, PRN, "00000000", RAM_NRO_CLIENTE );
            }
            PRN_FIN_TICKET();
        }
        else {
            VALIDAR_FACTURA_REMOTA( total, CLIENTE_AFINIDAD_NOMBRE, RAM_NRO_CLIENTE );
        }
    }
}
/**************************************************************************/
void IMPRIMIR_TICKET_FACTURA( int cant )
/**************************************************************************/
{
    int i;
	char caracter_sig[2];
	caracter_sig[0]='-';
	caracter_sig[1]='\0';

    PRN_INHABILITA_AVANCE_RECEIPT();
    if( IMPRIMIR_EN_JOURNAL ) {
        LF( 1 );
        IMPRIME_CABECERA_GENERAL( NRO_EVENTO - 1 );
        for( i = 0;i < cant;i++ ) {
            if( RAM_NOTA_CR ) {
                MOSTRAR_CADENA( 1, PRN, ST( S_NOTA_CR_ ) );
            }
            else {
                LF( 1 );
                MOSTRAR_CADENA( 1, PRN, ST( S_FACTURA ) );
            }
            if( comp.nro_comp ) {
                MOSTRAR_ENTERO( 9, PRN, "0000", comp.nro_suc );
                MOSTRAR_CADENA( 13, PRN, caracter_sig );
                MOSTRAR_ENTERO( 14, PRN, "00000000", comp.nro_comp + i );
            }
            if( REIMPRESION ) {
                MOSTRAR_CADENA( 23, PRN, ST( S_REIMPRESION ) );
            }
            LF( 1 );
        }
        IMPRIMIR_DETALLE_FACTURA_EN_JOURNAL();
        if( IMPRIMIR_TOTAL_EN_TICKET_FACTURA && !REIMPRESION ) {
            {
                char aux[15];
                sprintf( aux, "%s :", ST( S_TOTAL ) );
                MOSTRAR_CADENA( 1, PRN, aux );
            }
            MOSTRAR_FLOTANTE( 9, PRN, ENTEROS, DECIMALES, SI, SI, PAGOS_TOTAL_A_PAGAR() );
            LF( 1 );
        }
        #if defined( INVEL_W) || defined( INVEL_L)
        setData( ACTION_PRINT, NULL, 0 );
        #endif
    }
}
/**************************************************************************/
void IMPRIMIR_DETALLE_FACTURA_EN_JOURNAL( void )
/**************************************************************************/
{
    int j;
    if( IMPRIMIR_DETALLE_EN_JOURNAL ) {
        PRN_INHABILITA_AVANCE_RECEIPT();
        for( j = 0;j < RAM_P_TICKET;j++ ) {
            BUSCA_CODIGO_ESPECIAL( _TICKET_COD_ESPECIAL( j ) );
            SET_MEMORY( __item_cod_barra, articulo.cod_barra );
            SET_MEMORY( __item_nombre_largo, articulo.nombre );
            SET_MEMORY_LONG( __item_cod_especial, articulo.cod_interno );
            SET_MEMORY_DOUBLE( __ram_cantidad, _TICKET_CANTIDAD( j ) );
            SET_MEMORY_INT( __ram_anulacion, ( ( _TICKET_TIPO( j ) & _RESTA ) ? 1 : 0 ) );
            SET_MEMORY_CHAR( __item_pedir_precio,
                             ( ( _TICKET_TIPO2( j ) & _PEDIR_PRECIO ) ? 1 : 0 ) );
            ITEM_FORMATO_JOURNAL( _TICKET_PRECIO( j ) + _TICKET_RECARGO( j ) );
            // MICRO_LF(3);
            PAUSA( 1 );
        }
        LF( 1 );
    }
}
/**************************************************************************/
int OBTENER_DATOS_RETENCIONES()
/**************************************************************************/
{
    int rta = 1;
    long cod;
//    struct _factu_mem factu_mem;

    if( clientes.cod_cliente != RAM_NRO_CLIENTE ) {
		SELECT_TABLE( T_CLIENTES, TT_ORIG );
        cod = RAM_NRO_CLIENTE;
		SET_WHERE("COD_CLIENTE = %ld",cod); 
		RUN_QUERY(SI);
    }
    if( ES_FACTURA_A( clientes.cond_iva ) 
            || clientes.exento_ing_brutos == 'N'
            || clientes.exento_ing_brutos == 'n' ) { 
        /*------------------ Abre el archivo ------------------*/
		/*CLOSE_TABLE( T_FACTU_MEM, TT_ORIG  );
		if( OPEN_TABLE( T_FACTU_MEM, TT_ORIG, ( char* )&factu_mem, sizeof( struct _factu_mem ) ) ) {

            return 0;
        }*/
        /*------------------ Obtiene datos Retencin DGR ----------------------*/
		if( CALCULAR_RETENCION_DGR ) {
				porc_retenc_dgr = OBTENER_ALICUOTAS_DGR( clientes.tasa_ingr_bruto,( int )clientes.cond_iva ,
				&min_retenc_dgr );
			//Campo retencion_descuento indica si la RETENCION que se aplica es positiva o negativa
			if( clientes.retencion_descuento == 'S' 
					|| clientes.retencion_descuento == 's' ) {
				porc_retenc_dgr *= -1 ;
			}
		} else {
			porc_retenc_dgr = 0;
			min_retenc_dgr = 0;
		}
        /*------------------ Obtiene datos Percepcin DGI --------------------*/
		if( rta && CALCULAR_PERCEPCION_DGI ) {
			min_percep_dgi = config.monto_min_dgi;
		}
		if( CALCULAR_PERCEPCION_TACIS && config.municipio_percep 
				&& clientes.realizar_percepcion_tacis != 'N') {//es decir que el clientes deba hacerla!!! sino directamente no se cargue
			//Falta implementar las percepciones TACIS, ver implementacion Libertad DOS 
			porc_percep_tacis = OBTENER_ALICUOTAS( config.municipio_percep,
					( int )clientes.cond_iva, RAM_TOTAL,( unsigned )clientes.fec_fin_acredita,
					( unsigned )clientes.fec_ini_tacis, ( unsigned )clientes.fec_fin_tacis );//DEVUELVE SI ESTA EN CONDICIONES!!
		} else {
			porc_percep_tacis = 0;
		}
        /*----------------- Termina ----------------*/
		//CLOSE_TABLE( T_FACTU_MEM, TT_ORIG  );
    }

    return ( rta );
}
/**************************************************************************/
void CALCULAR_RETENCIONES()
/**************************************************************************/
{
	double   suma212 = 0.00;
	
	SET_MEMORY_DOUBLE( __ram_retencion_dgr,		RETENCION( 0.0, SI ) );
	/* si en las devoluciones no debe calcular la cereamos, podria ir adentro pero seria llamado desde otros lados como para intereses ej.. */
	if( !AUTORIZAR_RETENCION_NC(RAM_RETENCION_DGR ) ){
		SET_MEMORY_DOUBLE( __ram_retencion_dgr,0.0);
	}
	
    SET_MEMORY_DOUBLE( __ram_percepcion_dgi, PERCEPCION( 0.0, SI ) );
	/* si en las devoluciones no debe calcular la cereamos, podria ir adentro pero seria llamado desde otros lados como para intereses ej.. */
	if( !AUTORIZAR_PERCEPCION_NC( RAM_PERCEPCION_DGI )){
		SET_MEMORY_DOUBLE( __ram_percepcion_dgi,0.0 );
	}

	SET_MEMORY_DOUBLE( __ram_percepcion_tacis, PERCEPCION_TACIS( 0.0, SI ) );
    SET_MEMORY_DOUBLE( __ram_percepcion_212_iva1, PERCEPCION_212_IVA_1() );
    SET_MEMORY_DOUBLE( __ram_percepcion_212_iva2, PERCEPCION_212_IVA_2() );

   suma212 = RAM_PERCEPCION_212_IVA1 + RAM_PERCEPCION_212_IVA2;

   if( fabs( suma212 ) < config.monto_min_212 ) {
	   SET_MEMORY_DOUBLE( __ram_percepcion_212_iva1, 0.00 );
	   SET_MEMORY_DOUBLE( __ram_percepcion_212_iva2, 0.00 );
	}

}
/**************************************************************************/
double RETENCION( double importe_neto, char verificar )
/**************************************************************************/
{
	double retencion = 0.0 , base = -1;
    #ifdef COMPILAR_COMPROBANTES
    int     calcularNC = 0 ;
	char referenciada = ( RAM_MODO_DEVOLUCION == _DEVOL_TOTAL || RAM_MODO_DEVOLUCION == _DEVOL_PARCIAL ||
        RAM_MODO_DEVOLUCION == _DEVOL_INTERME || RAM_MODO_DEVOLUCION == _NC_TOTAL ||
        RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL || RAM_MODO_DEVOLUCION == _NC_RF_INTERME );

    if( REALIZAR_RETENCION_DGR(  ) ) {
        //Salvador aplica "unicamente" RETENCIONES a cliente Gubernamentales. Esta RETENCIONES son negativas
		if( clientes.cond_iva == INST_GUBERNAMENTAL ) {
			if( fabs( importe_neto ) >= min_retenc_dgr ) {
				//porc_retenc_dgr bien en positivo o negativo,
				//segun  clientes.retencion_descuento es 'S' o 'N' respectivamente.
				retencion = ROUND( importe_neto * (double)(porc_retenc_dgr / 100), 10, DECIMALES );
				if( RAM_NOTA_CR || MODO_DEVOLUCION ) {
   					retencion = -retencion;
				}
			}
		} else {
			char      formula_retencion[500];
			struct _valores_calculo valores_calculo;
			memset(&valores_calculo,0,sizeof(struct _valores_calculo));

			//config.tipo_calculo_dgr= '1'; //truchada
			if( RETORNAR_FORMULA( formula_retencion, config.tipo_calculo_dgr, clientes.cond_iva ) ) {
				valores_calculo.importe_neto = importe_neto;
				valores_calculo.iva = 0.0; //aca puede variar en las variaciones de las retenciones
				valores_calculo.exento = 0.0;
				base = RESOLVER( formula_retencion, &valores_calculo );
			} else {
				MENSAJE("ERROR CALCULO BASEIMPONIBLE");
				base = 0;
				BORRAR_MENSAJE();
			}
			if( RAM_NOTA_CR && referenciada ) {
				calcularNC = 1;
				if( fabs( RAM_RETENCION_DGR ) < 0.001 ) {
					calcularNC = 0; //no hubo retenciones originalmente, no se tienen que devolver
					base = 0;
				}
				//esto anula cualquier Calculo para las NC
				/*if ( config.permite_devolucion_percepcion == 'N' ) {
					calcularNC = 0; //no hubo retenciones originalmente, no se tienen que devolver
					base = 0;
				}*/
			} /*else {
				if( RAM_MODO_DEVOLUCION == _NC_LIBRE ) 
					//falta la funcion de libertad que setea estas variables RAM_CALCULAR_RETENC_NC.
					if( ( RAM_CALCULAR_RETENC_NC == 1 || RAM_CALCULAR_RETENC_NC == 3 ) 
						|| !verificar ) {
						//0- no calcula nada
      					//1- calcula solo retencion
      					//2 - calcula solo percepcion
      					//3- calcula percepcion y retencion
						calcularNC = 1;
					} else {
						base = 0;
						retencion = 0.00;
					}
					//esto anula cualquier Calculo pra las NC
					//if ( config.permite_devolucion_percepcion == 'N' ) {
					//		calcularNC = 0; //no hubo retenciones originalmente, no se tienen que devolver
					//			base = 0;
					//}
            }*/
            
			if( verificar && base ) {
				if(  clientes.fec_fin_acred_dgr > 0 && GET_FECHA_DOS( ) > clientes.fec_fin_acred_dgr ) {
					//no le tengo en cuenta el minimo
					verificar = NO;
				}
            }

			if( ( verificar && fabs( base ) >= min_retenc_dgr ) || ( !verificar )  || calcularNC ) {
					retencion = ROUND( base * (double)porc_retenc_dgr / 100, 10, DECIMALES );
				if( referenciada ) {
					if( clientes.porc_exento_ib > 0.001 &&
							( RAM_FECHA_ANTERIOR >= clientes.fec_ini_exento_ib ) &&
							( RAM_FECHA_ANTERIOR <= clientes.fec_fin_exento_ib ) ) {
				        retencion = ROUND( retencion * (double)( 100 - clientes.porc_exento_ib ) / 100, 10, DECIMALES );
				    }
			    } else {
					if( clientes.porc_exento_ib > 0.001 &&
							( GET_FECHA_DOS( ) >= clientes.fec_ini_exento_ib ) &&
							( GET_FECHA_DOS( ) <= clientes.fec_fin_exento_ib ) ) {
						retencion = ROUND( retencion * (double)( 100 - clientes.porc_exento_ib ) / 100, 10, DECIMALES );
					}
				}
            }

            if( RAM_NOTA_CR && fabs(retencion) >  0.001 ){
               retencion = - retencion;
            }
        }
    }
    #endif

    return ( retencion );
}
/**************************************************************************/
double PERCEPCION( double importe_neto, char verificar )
/**************************************************************************/
{
	double    percepcion = 0.0;
	char      nc_referenciada = 0;
	int       h = 0;
	char referenciada = ( RAM_MODO_DEVOLUCION == _DEVOL_TOTAL || RAM_MODO_DEVOLUCION == _DEVOL_PARCIAL ||
        RAM_MODO_DEVOLUCION == _DEVOL_INTERME || RAM_MODO_DEVOLUCION == _NC_TOTAL ||
        RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL || RAM_MODO_DEVOLUCION == _NC_RF_INTERME );

	if( ( RAM_FACTURA || RAM_TICKET_FACTURA ) && REALIZAR_PERCEPCION_DGI(  )
			&& CALCULAR_PERCEPCION_DGI ) {
		for( h = 0;h < 9;h++ ) {
			//Calculo la suma de los % de percepcion calculados de los totales de los distintos iva
			double dividendo=0.0, divisor =0.0;
				
			dividendo=(double)( _TOT_TICK_TOTAL_POR_COD_IVA( h )  - _TOT_TICK_IMP_INT_POR_COD_IVA( h ) );
				
			divisor =(double)( 1 + ((double)(iva[h].porc_iva1) )/ 100 )  ;
				
			percepcion +=  (double)ROUND(
				(	 (dividendo)//( _TOT_TICK_TOTAL_POR_COD_IVA( h )  - _TOT_TICK_IMP_INT_POR_COD_IVA( h ) ) 
				/ 
				divisor //( 1 + ((double)(iva[h].porc_iva1) )/ 100 )  
				), 10, DECIMALES ) * ((double)iva[h].tasa_dgi) / 100;
		}

		if ( RAM_NOTA_CR 
				&& referenciada
				&& fabs( RAM_PERCEPCION_DGI ) < 0.001 ) {
            percepcion = 0;//no hubo percepciones originalmente, no se tienen que devolver
		}

		if( !RAM_NOTA_CR && ( RAM_TICKET_FACTURA || RAM_FACTURA ) && ( verificar &&
			fabs( percepcion ) <= min_percep_dgi ) ) {
			percepcion = 0.0;
		}

		if( RAM_NOTA_CR 
				&& referenciada ) {
			if( percepcion!=0.0 && clientes.porc_exceptuar_percep > 0 &&
					( RAM_FECHA_ANTERIOR >= clientes.fecha_ini_excepcion ) &&
					( RAM_FECHA_ANTERIOR <= clientes.fecha_vto_excepcion ) ) {
				percepcion = ROUND( percepcion * (double)( 100 - clientes.porc_exceptuar_percep ) / 100,
					10, DECIMALES );
			}
			//percepcion = ( !RAM_FECHA_ANTERIOR || ( MON( RAM_FECHA_ANTERIOR )!= MON( GET_FECHA_DOS( ) ) ) )? 0 : -percepcion;
			//se saca la restriccion del mes
			percepcion = ( !RAM_FECHA_ANTERIOR )? 0 : -percepcion;
		} else {
			 if( percepcion != 0.0 && clientes.porc_exceptuar_percep > 0 &&
				   ( GET_FECHA_DOS( ) >= clientes.fecha_ini_excepcion )  &&
				   ( GET_FECHA_DOS( ) <= clientes.fecha_vto_excepcion ) ) {
				percepcion = ROUND( (percepcion * (double)( 100 - clientes.porc_exceptuar_percep ) / 100), 10, DECIMALES );
			 }
		}
		
		if( RAM_MODO_DEVOLUCION == _NC_LIBRE ) {
			if(  fabs( percepcion ) < 0.0 )
			//|| ( RAM_CALCULAR_RETENC_NC == 0 || RAM_CALCULAR_RETENC_NC == 1 ) 
			{
				//0- no calcula nada
      			//1- calcula solo retencion
      			//2 - calcula solo percepcion 
      			//3- calcula percepcion y retencion
				percepcion = 0.0;
			} else {
				percepcion = - percepcion;
			}
		}

		if( RAM_MODO_DEVOLUCION == _NC_LIBRE ) {
			char aux[90];
			sprintf( aux, "DGI: %lf", percepcion );
			GRABAR_LOG_SISTEMA( aux, LOG_DEBUG,2 );
		}
		}
		return ( percepcion );
	}
/**************************************************************************/
void CALCULAR_RETENCIONES_TOTALES()
/**************************************************************************/
{
    int h;
    double ret_var=0.0, ret_tot_var = 0.0, iva2 = 0.0;
    double ret_pago= 0.0, total= 0.0, importe= 0.0, ret_int = 0.0, porc_total = 1.0;
	 if( ( RAM_FACTURA || RAM_TICKET_FACTURA ) && (  ES_FACTURA_A( clientes.cond_iva ) || CLIENTE_GUBERNAMENTAL( ) ) ) {
        for( h = 0;h < RAM_P_PAGO;h++ ) {
            if( _PAGO_VARIACION( h ) ) {
                ret_var = 0.0;
                if( !medios[_PAGO_MODO( h )].tarjeta ) {
                    total = RAM_IMPORTE_TOTAL + RAM_RETENCION_DGR + RAM_PERCEPCION_DGI
                          + RAM_PERCEPCION_212_IVA1 + RAM_PERCEPCION_212_IVA2;
                    if( RAM_INTERESES ) {
                        ret_int = RAM_PORC_INTERES / 100
                                * ( RAM_RETENCION_DGR + RAM_PERCEPCION_DGI + RAM_PERCEPCION_212_IVA1
                                  + RAM_PERCEPCION_212_IVA2 );
                    }
                    ret_pago = ROUND( ( _PAGO_IMPORTE( h ) - _PAGO_VARIACION( h ) )
                                    / total * ( RAM_RETENCION_DGR + RAM_PERCEPCION_DGI
                                              + RAM_PERCEPCION_212_IVA1 + RAM_PERCEPCION_212_IVA2
                                              + ret_int ), 10, DECIMALES );
                    ret_var = ROUND( _PAGO_VARIACION( h )
                                   / ( _PAGO_IMPORTE( h ) - _PAGO_VARIACION( h ) ) * ret_pago, 10,
                                     DECIMALES );
                }
                else {
                    importe = _PAGO_IMPORTE( h ) - _PAGO_VARIACION( h );
                    _VARIACION_TARJETA( importe,
                                        VAR_TARJETAS_RECARGO + VAR_TARJETAS_RECARGO_DIFERIM
                                      + medios[_PAGO_MODO( h )].variacion, &ret_var, &iva2,
                                        medios[_PAGO_MODO( h )].art_variacion, NO );
                }
                ret_tot_var += ret_var;
            }
        }
        if( ret_tot_var ) {
			if( ES_FACTURA_A( clientes.cond_iva ) || CLIENTE_GUBERNAMENTAL( ) ) {
                porc_total = ( ( RAM_RETENCION_DGR ) ? porc_retenc_dgr : 0 )
                           + ( ( RAM_PERCEPCION_DGI ) ? porc_percep_dgi
                             : ( 0 + ( iva[0].porc_iva1 / 2 ) + ( iva[1].porc_iva1 / 2 ) ) );
                ADD_MEMORY_DOUBLE( __ram_retencion_dgr,
                                   ret_tot_var / ( porc_total )
                                  *( ( RAM_RETENCION_DGR ) ? porc_retenc_dgr : 0 ) );
                ADD_MEMORY_DOUBLE( __ram_percepcion_dgi,
                                   ret_tot_var / ( porc_total )
                                  *( ( RAM_PERCEPCION_DGI ) ? porc_percep_dgi : 0 ) );
            }
            if( RAM_PERCEPCION_212_IVA1 )
            ADD_MEMORY_DOUBLE( __ram_percepcion_212_iva1,
                               ret_tot_var / ( porc_total )*( iva[0].porc_iva1 / 2 ) );
            if( RAM_PERCEPCION_212_IVA2 )
            ADD_MEMORY_DOUBLE( __ram_percepcion_212_iva2,
                               ret_tot_var / ( porc_total )*( iva[1].porc_iva1 / 2 ) );
        }
    }
}
/**************************************************************************/
double PERCEPCION_TACIS( double importe_neto, double iva )
/**************************************************************************/
{
   double    percepcion = 0, total = 0;

   if( ( RAM_FACTURA || RAM_TICKET_FACTURA ) && REALIZAR_PERCEPCION_TACIS(  )
      && CALCULAR_PERCEPCION_TACIS ) {
      switch ( clientes.cond_iva ) {
         case 1:  //  RESP_INSC
         case 2:  //  RESP_NO_INSC
            total = TOT_TICK_IMPORTE_SIN_IVA + importe_neto - TOT_TICK_IMP_INT + TOT_TICK_EXENTO;
            break;
         default:
            total = TOT_TICK_IMPORTE_SIN_IVA + importe_neto + iva + TOT_TICK_EXENTO + TOT_TICK_IVA1;
            break;
      }
      percepcion = ROUND( ( total * porc_percep_tacis ) / 100, 10, DECIMALES );
   }
   if( RAM_EN_IMPORTACION_DE_TICKET ) {
      percepcion = RAM_PERCEPCION_TACIS;
   } else if( RAM_NOTA_CR ) {
      percepcion = 0.0; 
   }

   return ( percepcion );
}

/**************************************************************************/
void POSICIONA_EN_COMPROBANTE( void )
/**************************************************************************/
{
    //long id_evento = RAM_ID_EVENTO - 1;
    long id_evento = RAM_ID_ULTIMO_EVENTO_FACTURA;
    int tabla_ant = SELECTED2();
	int tipo_ant = SELECTED_TIPO();

    if( tipo_devolucion == _DEVOL_PARCIAL || tipo_devolucion == _DEVOL_LIBRE ) {
			id_evento = RAM_ID_EVENTO-1;
	}
	//int area_ant = SELECTED();
    //SELECT( AREA_TICKET );
    //GET_LAST();
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
	SET_WHERE("");
	SET_ORDER2("ID_EVENTO");
	RUN_QUERY(NO);
	GO2(BOTTOM);
    //  if( event_ticket.tipo_evento == _EVENT_INICIO_CAJERO )
    //      id_evento = id_evento - 1; //si hubo corte de luz y el ultimo evento es un ingreso de cajero
    while( !dbBOF() && event_ticket.id_evento != ( id_evento ) ) {
        SKIP2( -1 );
    }
    while( !dbBOF() && event_ticket.tipo_evento == _EVENT_ANULA_COMPROBANTE
        || event_ticket.cajero != NRO_CAJERO ) {
        SKIP2( -1 );
    }
	SELECT_TABLE( tabla_ant, tipo_ant );
    //SELECT( area_ant );
}
/**************************************************************************/
int ES_FACTURA_A( int cond_cli )
/**************************************************************************/
{
    int rta = 0;

    if( cond_cli > 48) //problemas de casteo
        cond_cli = cond_cli -'0';

    if( FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
        rta = ( RAM_FACTURA == FACTURA_TIPO_A );
    }
    else {
        rta = ( cond_cli < 3 || cond_cli == 8 ) ? 1 : 0;
    }
    return rta;
}

/**************************************************************************/
int ES_CONSUMIDOR_FINAL( int cond_cli )
/**************************************************************************/
{
   if( cond_cli == 5 )
      return 1;
   else
      return 0;
}
/***************************************************************************/
int ABRIR_ARCHIVOS_FACTURAS( int clase, char *buffer )
/***************************************************************************/
{
    char _nombre[100];
    memset( _nombre, 0, sizeof( _nombre ) );
    if( RAM_MODO_EJECUCION != ENTRENAMIENTO ) {
        if( clase == _REMITO ) {
            if( NRO_FACTURA == LOCAL_FAC ) {
                NOM_ARCHIVO( _REMITO_MEM, _nombre );
            }
            else {
                NOM_ARCHIVO( _REMITO_MEM_REMOTO, _nombre );
            }
        }
        else {
            if( NRO_FACTURA == LOCAL_FAC ) {
                NOM_ARCHIVO( _FACTU_MEM, _nombre );
            }
            else {
                NOM_ARCHIVO( _FACTU_MEM_REMOTO, _nombre );
            }
        }
    }
    else {
        #ifdef COMPILAR_MODO_ENTRENAMIENTO
        if( clase == _REMITO ) {
            NOM_ARCHIVO( _REMITO_MEM_ENTR, _nombre );
        }
        else {
            NOM_ARCHIVO( _FACTU_MEM_ENTR, _nombre );
        }
        #endif
    }
    return ( USE_DATABASE( AREA_AUX, _nombre, buffer, 0, NULL, 0, 0 ) ) ? 0 : 1;
}
/**************************************************************************/
double PERCEPCION_212_IVA_1()
/**************************************************************************/
{
   int       h, cod;
   double    porc_perc_212_iva0 = 0.0, neto_grav0 = 0.0, iva0 = 0.0, signo;

   if( ( RAM_FACTURA || RAM_TICKET_FACTURA ) && ( RAM_NRO_CLIENTE
         && ( RAM_NRO_CLIENTE == -1 || EXISTE_CLIENTE( RAM_NRO_CLIENTE ) )
         && clientes.cond_iva == RESP_NO_IDEN && !RAM_BIEN_DE_USO ) ) {
      cod = OBTENER_COD_IVA_INICIAL(  );
      for( h = 0; h < RAM_P_TICKET; h++ ) {
         if( _TICKET_COD_IVA( h ) == cod ) {
            signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
            neto_grav0 +=
               ( ( _TICKET_IMPORTE_SIN_IVA( h ) -
                  _TICKET_IMP_INT( h ) ) * _TICKET_CANTIDAD( h ) * signo );
            iva0 +=
               ( _TICKET_IVA1_PRECIO( h ) +
               _TICKET_IVA1_REC( h ) ) * signo * _TICKET_CANTIDAD( h );
         }
      }
      if(( iva[cod].porc_212 > 0.001 ) &&
         ( fabs( neto_grav0 + iva0 /*+ importe_variacion*/ ) > 0.0001 )) {
         porc_perc_212_iva0 = ROUND( ( neto_grav0 + iva0 /*+ importe_variacion*/ ) *
             ( ( iva[cod].porc_212 ) / 100 ), 10,DECIMALES );
      }
      if( RAM_NOTA_CR )
         porc_perc_212_iva0 = ( !RAM_FECHA_ANTERIOR ||
            ( MON( RAM_FECHA_ANTERIOR )!= MON( GET_FECHA_DOS( ) ) ) ) ? 0
            : -porc_perc_212_iva0;
   }
	if( RAM_EN_IMPORTACION_DE_TICKET && RAM_NOTA_CR ) {
      porc_perc_212_iva0 = ( MON( RAM_FECHA_ANTERIOR )!= MON( GET_FECHA_DOS( ) ) ) ? 0
            : RAM_PERCEPCION_212_IVA1;
   } else if( RAM_NOTA_CR && config.monto_min_212 > 0.001 ) {
      porc_perc_212_iva0 = 0.0;
   }

   return ( porc_perc_212_iva0 );
}
/**************************************************************************/
double PERCEPCION_212_IVA_2()
/**************************************************************************/
{
   int       h, cod;
   double    porc_perc_212_iva1 = 0.0, neto_grav1 = 0.0, iva1 = 0.0, signo;

   if( ( RAM_FACTURA || RAM_TICKET_FACTURA ) && ( RAM_NRO_CLIENTE
         && (RAM_NRO_CLIENTE==-1 || EXISTE_CLIENTE( RAM_NRO_CLIENTE )) && clientes.cond_iva == RESP_NO_IDEN
         && !RAM_BIEN_DE_USO ) ) {
      cod = OBTENER_COD_IVA_INICIAL(  ) + 1;
      for( h = 0; h < RAM_P_TICKET; h++ ) {
         if( _TICKET_COD_IVA( h ) == cod ) {
            signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
            neto_grav1 +=
               ( ( _TICKET_IMPORTE_SIN_IVA( h ) -
                  _TICKET_IMP_INT( h ) ) * _TICKET_CANTIDAD( h ) * signo );
            iva1 +=
               ( _TICKET_IVA1_PRECIO( h ) +
               _TICKET_IVA1_REC( h ) ) * signo * _TICKET_CANTIDAD( h );
         }
      }
      if( ( iva[cod].porc_212 > 0.001 ) &&
         (fabs( neto_grav1 + iva1 /*+ importe_variacion*/) > 0.0001 ) ) {
         porc_perc_212_iva1 = ROUND( ( neto_grav1 + iva1 /*+ importe_variacion*/ ) *
            ( ( iva[cod].porc_212 ) / 100 ), 10,DECIMALES );
      }

      if( RAM_NOTA_CR )
         porc_perc_212_iva1 = ( !RAM_FECHA_ANTERIOR ||
            ( MON( RAM_FECHA_ANTERIOR )!= MON( GET_FECHA_DOS( ) ) ) ) ? 0
            : -porc_perc_212_iva1;
   }
	if( RAM_EN_IMPORTACION_DE_TICKET && RAM_NOTA_CR ) {
      porc_perc_212_iva1 = ( MON( RAM_FECHA_ANTERIOR )!= MON( GET_FECHA_DOS( ) ) ) ? 0
         : RAM_PERCEPCION_212_IVA2;
   } else if( RAM_NOTA_CR && config.monto_min_212 > 0.001 ) {
      porc_perc_212_iva1 = 0.0;
   }

   return ( porc_perc_212_iva1 );
}

#if defined(INVEL_W) || defined(INVEL_L)
/**************************************************************************/
int OBTENER_FLAG_COMPROBANTE( int cond_cli, int comprob )
/**************************************************************************/
{
    int flag = -1;
    if( !FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
    if( ES_FACTURA_A( cond_cli ) ) {
        if( comprob == 0 ) {
            flag = FLAG_FACA;
        }
        else {
            flag = FLAG_TFA;
        }
    }
    else {
        if( ES_FACTURA_B(/* cond_cli*/ ) ) {
            if( comprob == 0 ) {
                flag = FLAG_FACB;
            }
            else {
                flag = FLAG_TFB;
            }
        }
        else {
            if( comprob == 0 ) {
                flag = FLAG_FACC;
            }
            else {
                flag = FLAG_TFC;
            }
        }
    }
    } else {
        if( RAM_FACTURA == FACTURA_TIPO_A ) {
            if( comprob == 0 ) {
                flag = FLAG_CF;
            }
            else {
                flag = FLAG_TFA;
            }
        } else {
            if( comprob == 0 ) {
                flag = FLAG_FCF;
            }
            else {
                flag = FLAG_TFB;
            }
        }
    }

    return flag;
}
#endif
/**************************************************************************/
void CALCULAR_VALOR_CADA_ARTICULO_RESTANDO_DESCUENTOS( int renglon )
/**************************************************************************/
{
    double neto_gravado = 0.0, iva1 = 0.0, iva2 = 0.0,
           imp = 0.0, exento = 0.0;
    double coef = 0, pre_sin_iva = 0.0, total_en_art_f = 0.0;
    double impuesto[4];
    int i;
    if( art_desc[renglon].precio ) {
        /*------------ Posiciono en e_ticket ------------*/
        //SELECT( AREA_E_TICKET );
        //GET_DIRECT( art_desc[renglon].pos );
		SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
		SET_WHERE("");
		SET_ORDER2("ID_EVENTO");
		RUN_QUERY(NO);
		//GO2(art_desc[renglon].pos);
        GET_DIRECT_REG( art_desc[renglon].pos, 0);
        total_en_art_f = art_fact[renglon].neto_gravado + art_fact[renglon].imp_int
                       + art_fact[renglon].monto_impuesto[0] + art_fact[renglon].iva1
                       + art_fact[renglon].iva2 + art_fact[renglon].monto_impuesto[1]
                       + art_fact[renglon].monto_impuesto[2] + art_fact[renglon].monto_impuesto[3];
        coef = ( art_desc[renglon].precio * 100 ) / total_en_art_f;
        pre_sin_iva = art_fact[renglon].precio * coef / 100;
        neto_gravado = art_fact[renglon].neto_gravado * coef / 100;
        exento = art_fact[renglon].exento * coef / 100;
        iva1 = art_fact[renglon].iva1 * coef / 100;
        iva2 = art_fact[renglon].iva2 * coef / 100;
        for( i = 0;i < 4;i++ ) {
            impuesto[i] = art_fact[renglon].monto_impuesto[i] * coef / 100;
        }
        imp = art_fact[renglon].imp_int * coef / 100;
        /*------------ Resto los descuentos -------------*/
        art_fact[renglon].precio += pre_sin_iva;
        art_fact[renglon].neto_gravado += neto_gravado;
        art_fact[renglon].exento += exento;
        art_fact[renglon].imp_int += imp;
        art_fact[renglon].iva1 += iva1;
        art_fact[renglon].iva2 += iva2;
        for( i = 0;i < 4;i++ ) {
            art_fact[renglon].monto_impuesto[i] += impuesto[i];
        }
    }
}
/*****************************************************************************/
void GRABA_EVENTO_COMPROBANTE_ANULADO( double nro, long id_evento )
/*****************************************************************************/
{
    memset( &event_ticket, 0, sizeof( event_ticket ) );
    event_ticket.nro_ticket = 0;
    event_ticket.tipo_evento = _EVENT_ANULA_COMPROBANTE;
    event_ticket.nro_comprobante_new = nro;
    event_ticket.suc_comprobante = config.sucursal;
    event_ticket.id_evento_comprob_ant = id_evento;
    GRABAR_EVENT_TICKET( SI );
    ACTUALIZA_ID_EVENTO_NEW();
}
/***************************************************************************/
int IMPRIMIR_COMPROBANTE_JOURNAL( int tipo, int reimpresion )
/***************************************************************************/
{
    int     j = 0, i = 0, subtipo = 0, rta, h, cant, _uno_1 = 1, _uno_2 = 1;
    double  _neto = 0.00, tot_imp = 0.00;
    char    init_acu;
    float   _neto_percep_iva1 = 0, _neto_percep_iva2 = 0;
    int reglon_backup = 0, cond_IVA = 0, tipo_comprobante =0;

    imprimiendo_factura_journal = 1;
    fila = 1;
    columna = 1;
    _nro_dato = 0;
    /*------------------ Tipo de comprobante -------------------*/
    APUNTA_CFG( tipo );
	tipo_comprobante = TIPO_COMPROBANTE( &subtipo );
	cond_IVA = OBTENER_COND_IVA_CLIENTE( 0 );


    /*------------------ Inicializaciones ---------------*/

    REIMPRESION = reimpresion;
    if( !reimpresion ) {
        if( !VERIFICAR_EVENTOS_DEL_COMPROBANTE( tipo ) || !EXISTE_CLIENTE( RAM_NRO_CLIENTE ) ) {
            return ( 0 );
        }
        comp.tipo_comp = tipo_comprobante;
        comp.cond_cli = cond_IVA;
        comp.subtipo = subtipo;
        comp.nro_suc = event_ticket.suc_comprobante;
        comp.nro_comp = event_ticket.nro_comprobante_new;
    }
    #ifdef DEBUG_R
        if( MODO_DEBUG == 55 || MODO_DEBUG == _DEBUG_REDONDEO ) {
            char c[80];
            sprintf( c, "cmp. cond:%d sub:%d ramf:%d", comp.cond_cli, comp.subtipo, RAM_FACTURA );
            GRABAR_LOG_SISTEMA( c );
        }
        #endif      
if( !AGRUPAR_ARTICULOS( &cant_renglones, &art_fact, &art_fact_iva , &art_fact_iva_backup, &art_desc)){
    //if( !AGRUPAR_ARTICULOS( &j ) ) {
        MENSAJE_STRING( S_MEMORIA_INSUFICIENTE_PARA_EMITIR_COMPROBANTE );
        return ( 0 );
    }
	j = 0;
    comp.fecha = event_ticket.fecha;
    comp.hora = event_ticket.hora;
    comp.cant_formularios = CANTIDAD_FORMULARIOS( cant_renglones );
    cant = ( COMP_INDEP ) ? comp.cant_formularios : 1;
    rta = OBTENER_DATOS_COMPROBANTE( tipo, comp.subtipo, cant, &comp.nro_suc, &comp.nro_comp,
                                     &comp.copias, SI, NO );
    if( rta ) {
        for( orden = 0;orden < comp.copias;orden++ ) {
            init_acu = SI;
            nro_comp = comp.nro_comp;
            renglon = 0;

            /*----------------------- imprime formulario ----------------------*/
            for( j = 0; j < comp.cant_formularios; j++ ) {
                PRN_SELECT( JOURNAL );
                _hoja_actual = j + 1;
                _uno_1 = 1;
                _uno_2 = 1;

                if( COMP_INDEP || init_acu ) {
                    subtotal = 0.0;
                    exento = 0.0;
                    imp_int = 0.0;
                    iva1 = 0.0;
                    iva2 = 0.0;
                    neto = 0.0;
                    total = 0.0;
                    for( i = 0;i < 4;i++ ) {
                        imp[i] = 0.0;
                    }
                    retencion_dgr = 0.0;
                    percepcion_dgi = 0.0;

                    percep_212_iva1 = 0.0;
                    percep_212_iva2 = 0.0;

                    cant_total = 0.0;
                    costo = 0.0;
                    memset( total_impuestos, 0, sizeof( total_impuestos ) );
                    memset( art_fact_iva, 0, sizeof( struct _art_fact_iva ) * MAX_COD_IVA );
                    init_acu = NO;
                }
                /*----------- Imprimimos el encabezamiento ---------*/
                IMPRIMIR_CABECERA_FACTURA_JOURNAL( j );
                ind_config = ind_comienzo[_tipo];
                while( CONFIG_COMPROBANTE1[ind_config].codigo != _COMIENZO_CUERPO ) {
                   if( CONFIG_COMPROBANTE1[ind_config].codigo == _PROCESAR_DATOS_ADICIONALES  && _uno_1 ) {
					   COMP_ejecutar_comando( &ind_config );
					   _uno_1 = 0;
				   } else if( CONFIG_COMPROBANTE1[ind_config].codigo == _DATO_ADICIONAL_MEDIO_PAGO && _uno_2 ) {
					   COMP_ejecutar_comando( &ind_config );
					   _uno_2 = 0;
					   LF( 1 );
					} else {
                        ind_config++;
                    }
                }

                /*---------------- Avanza hasta el inicio del cuerpo ------------*/
                if( tipo == _FAC_REM ) {
                    while( CONFIG_COMPROBANTE1[ind_config].codigo != _COMIENZO_CUERPO_FAC_REM ) {
                        ind_config++;
                    }
                }
                ind_config++;
                ind_comienzo_cuerpo = ind_config;
                /*----------------------- Cuerpo de la factura ---------------------*/
                reglon_backup = renglon;
                h = 0;
                while( renglon < cant_renglones && h < CANT_RENGLONES[_tipo] ) {
                    if( fabs( art_fact[renglon].cantidad ) > 0.001 ) {
                        if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
                            BUSCA_COD_INTERNO_COD_BARRA( art_fact[renglon].cod_articulo,
                                                         art_fact[renglon].cod_barra );
                        }
                        else {
                            BUSCA_CODIGO_ESPECIAL( art_fact[renglon].cod_articulo );
                        }
                        //SELECT( AREA_E_TICKET );
                       //GET_DIRECT( art_fact[renglon].pos );
						SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
							SET_WHERE("");
							SET_ORDER2("ID_EVENTO");
							RUN_QUERY(NO);
							GET_DIRECT_REG( art_fact[renglon].pos, 0 );

                        cantidad = art_fact[renglon].cantidad;
                        precio = art_fact[renglon].precio;
						SET_MEMORY_CHAR( __item_gravado, art_fact[renglon].gravado );
                        //precio_t =
                        //  art_fact[renglon].neto_gravado + art_fact[renglon].exento +
                        //  art_fact[renglon].imp_int + art_fact[renglon].iva1 +
                        //  art_fact[renglon].iva2 + art_fact[renglon].monto_impuesto[0] +
                        //  art_fact[renglon].monto_impuesto[1] + art_fact[renglon].monto_impuesto[2]
                        //  + art_fact[renglon].monto_impuesto[3];
                        //fabiana 15-02-05 ** nuevo manejo de redondeos. Facturacion especial queda deshabilitado
                        //if( !MODO_FACTURACION_ESPECIAL(  ) ) {
                        //  precio_t = ROUND( precio_t, ENTEROS, DECIMALES );
                        //}

                        //precio_final =
                        //  ROUND( art_fact[renglon].neto_gravado + art_fact[renglon].exento +
                        //  art_fact[renglon].imp_int + art_fact[renglon].iva1 +
                        //  art_fact[renglon].iva2 + art_fact[renglon].ila +
                        //  art_fact[renglon].iha, ENTEROS, DECIMALES );

                        if( ES_FACTURA_A( comp.cond_cli ) && !FACTURA_CON_PRECIO_FINAL ) {
                                for( i = 0;i < 4;i++ ) {
                                    tot_imp = art_fact[renglon].monto_impuesto[i];
                                }
                                precio_final =
                                ROUND( art_fact[renglon].neto_gravado
                                + art_fact[renglon].exento
                                + art_fact[renglon].imp_int, ENTEROS, DECIMALES )
                                + ROUND( art_fact[renglon].iva1
                                + art_fact[renglon].iva2 + tot_imp, ENTEROS, DECIMALES );
                        } else {
                                precio_final =
                                ROUND( art_fact[renglon].neto_gravado
                                + art_fact[renglon].exento
                                + art_fact[renglon].imp_int + art_fact[renglon].iva1
                                + art_fact[renglon].iva2, ENTEROS, DECIMALES );
                        }

                        if( ES_FACTURA_A( comp.cond_cli ) && !FACTURA_CON_PRECIO_FINAL ) {
                            importe =
                                    art_fact[renglon].neto_gravado
                                    + art_fact[renglon].exento
                                    + art_fact[renglon].imp_int;
                            
                            #ifdef DEBUG_R
                            if( MODO_DEBUG == _DEBUG_REDONDEO ) {
                                memset( cadena_debug, 0, strlen( cadena_debug ) );
                                _snprintf( cadena_debug, 80, "14.1.Comprob, art=%li,pr=%lf,cant=%f,imp=%lf",
                                         art_fact[renglon].cod_articulo, art_fact[renglon].precio,
                                         art_fact[renglon].cantidad, importe);
                                cadena_debug[79] = '\0';
                                //GRABAR_LOG_SISTEMA( cadena_debug );
                                memset( cadena_debug, 0, strlen( cadena_debug ) );
                                _snprintf( cadena_debug, 80, "14.2.Comprob, %lf, pr=%lf,iva=%lf,imp=%lf, dif=%lf",
                                         precio_final, art_fact[renglon].neto_gravado,
                                         art_fact[renglon].iva1,
                                         ROUND( importe, ENTEROS, DECIMALES ),
                                         ROUND( importe, ENTEROS, DECIMALES ) - importe );
                                cadena_debug[79] = '\0';

                                GRABAR_LOG_SISTEMA( cadena_debug );
                            }
                            #endif

                            importe = ROUND( importe, ENTEROS, DECIMALES );
                        } else {
                            importe = precio_final;
                        }
                        /*------------------------- Acumula ----------------------------*/
                        cant_total += cantidad;
                        subtotal += importe;
                        exento += art_fact[renglon].exento;
                        costo += art_fact[renglon].costo;
                        imp_int += art_fact[renglon].imp_int;
                        iva1 += art_fact[renglon].iva1;
                        iva2 += art_fact[renglon].iva2;
                        for( i = 0;i < 4;i++ ) {
                            imp[i] += art_fact[renglon].monto_impuesto[i];
                        }
                        neto += art_fact[renglon].neto_gravado;
                        //total += precio_t;
                        total += precio_final;

                        if( art_fact[renglon].cod_iva < MAX_COD_IVA ) {
                            art_fact_iva[art_fact[renglon].cod_iva].neto_gravado +=
                                    art_fact[renglon].neto_gravado;
                            art_fact_iva[art_fact[renglon].cod_iva].iva1 +=
                                    art_fact[renglon].iva1;
                            art_fact_iva[art_fact[renglon].cod_iva].iva2 +=
                                    art_fact[renglon].iva2;
                        } else {
                            MENSAJE_STRING( S_COD_IVA_INVALIDO );
                        }

                        /*------------ Acumula ILA e IHA ------------*/
                        if( event_e_ticket.cod_ext ) {
                            CARGA_TOTAL_IMPUESTOS( total_impuestos, event_e_ticket.cod_ext );
                        } else {
                            for( i = 0;i < 4;i++ ) {
                                if( articulo.cod_impuesto[i] ) {
                                    total_impuestos[articulo.cod_impuesto[i] - 1] +=
                                        art_fact[renglon].monto_impuesto[i];
                                }
                            }
                        }
                        h++;
                        /*SET_MEMORY( __item_cod_barra, articulo.cod_barra );
                        SET_MEMORY( __item_nombre_largo, articulo.nombre );
                        SET_MEMORY_LONG( __item_cod_especial, articulo.cod_interno );*/
                        ASIGNA_ITEM( NO );
                        SET_MEMORY_DOUBLE( __ram_cantidad, cantidad );
                        SET_MEMORY_DOUBLE( __ram_cantidad_original, cantidad );

                        //SET_MEMORY_INT( __ram_anulacion, ( ( _TICKET_TIPO( j ) & _RESTA ) ? 1 : 0 ) );
                        //SET_MEMORY_CHAR( __item_pedir_precio,
                        //  ( ( _TICKET_TIPO2( j ) & _PEDIR_PRECIO ) ? 1 : 0 ) );

                        IMPRIMIR_TKT_CUERPO( ( importe / cantidad ), renglon );
                    }
                    renglon++;
                }
                
                while( CONFIG_COMPROBANTE1[ind_config].codigo != _FIN_CUERPO
                    && CONFIG_COMPROBANTE1[ind_config].codigo != _FIN ) {
                    ind_config++;
                }

                /*------------------------- Imprime totales ------------------------*/
                if( j == comp.cant_formularios - 1 || COMP_INDEP ) {
                    /*----- Calculamos retenciones --------*/
                    /* martha */
                    if( COMP_INDEP ) {
                        _neto += neto;
                    }

                    if( j == comp.cant_formularios - 1 ) {
                        if( !COMP_INDEP ) {
                            _neto = neto;
                        }
                        if( CALCULAR_RETENCION_DGR 
                                && !RAM_BIEN_DE_USO
                                && ( comp.tipo_comp == _FACTURA || comp.tipo_comp == _NOTA_CREDITO )
                                //Salvador aplica la retencion a clientes Gubernamentales tanto
                                //al comprobante A como al B. 
                                && ( comp.subtipo == TIPO_A || clientes.cond_iva == INST_GUBERNAMENTAL ) 
                                && fabs( _neto ) >= min_retenc_dgr
                                && ( clientes.exento_ing_brutos == 'N'
                                    || clientes.exento_ing_brutos == 'n' ) ) {
                            retencion_dgr =
                                ROUND( _neto * porc_retenc_dgr / 100, 10, DECIMALES );
                            total += retencion_dgr;
                        }
                        if( CALCULAR_PERCEPCION_DGI && !RAM_BIEN_DE_USO
                         && ( comp.tipo_comp == _FACTURA || comp.tipo_comp == _NOTA_CREDITO )
                         && comp.subtipo == 1 && fabs( _neto ) >= min_percep_dgi
                         && clientes.realizar_percepcion_dgi != 'N' ) {
                            percepcion_dgi =
                              ROUND( _neto * porc_percep_dgi / 100, 10, DECIMALES );
                            total += percepcion_dgi;
                        }
                        _neto_percep_iva1 = ( float )( art_fact_iva[0].neto_gravado
                            + art_fact_iva[0].iva1 )
                            * ( ( iva[0].porc_iva1 / 2 ) / 100 );
                        _neto_percep_iva2 = ( float )( art_fact_iva[1].neto_gravado
                            + art_fact_iva[1].iva1 )
                            * ( ( iva[1].porc_iva1 / 2 ) / 100 );
                        if( comp.cond_cli == RESP_NO_IDEN && comp.tipo_comp == _FACTURA ) {
                            percep_212_iva1 = ROUND( _neto_percep_iva1, 10, DECIMALES );
                            percep_212_iva2 = ROUND( _neto_percep_iva2, 10, DECIMALES );
                            total += ( percep_212_iva1 + percep_212_iva2 );
                        }
                        total = ROUND( total, 10, DECIMALES );
                    }
                    IMPRIMIR_PIE_FACTURA_JOURNAL();
                }

                if( COMP_INDEP ) {
                    nro_comp++;
                }
                LF( 1 );
                SET_COLUMNA( 1 );
            }
        }
        /*--------------- Vuelve ---------------*/
        PRN_INHABILITA_AVANCE_JOURNAL();

        if( art_fact ) {
            FREEMEM( ( void* )art_fact );
        }
        if( art_fact_iva ) {
            FREEMEM( ( void* )art_fact_iva );
        }
    } else {
        MENSAJE_STRING( S_ERROR_LEER_DATOS_PARA_EMITIR_COMPROBANTE );
    }
    SET_COLUMNA( 1 );
    imprimiendo_factura_journal = 0;

    return ( rta );
}

/******************************************************************************/
void IMPRIMIR_CABECERA_FACTURA_JOURNAL( int bandera )
/******************************************************************************/
{
    if( !bandera ) {
        MOSTRAR_CADENA( 1, PRN, "Fecha:" );
        MOSTRAR_FECHA( 8, PRN, comp.fecha );
        MOSTRAR_CADENA( 18, PRN, "Hora:" );
        MOSTRAR_HORA( 24, PRN, comp.hora );
        MOSTRAR_CADENA( 31, PRN, "Caja:" );
		MOSTRAR_ENTERO( 37, PRN, ( USAR_NRO_CAJA_DGI )?
				"0000":"000", NUMERO_CAJA( ) );
        LF( 1 );
    }
    MOSTRAR_CADENA( 1, PRN, "DOCUMENTO No: " );
}
/******************************************************************************/
void IMPRIMIR_PIE_FACTURA_JOURNAL( void )
/******************************************************************************/
{
    double total_factura = 0.00 ,iva;
    int h, posicion = 0;
	unsigned aux_fecha;

    LF( 1 );
    if( ES_FACTURA_A( comp.cond_cli ) ) {
        MOSTRAR_CADENA( 9, PRN, "TOTAL IVA:" );
		iva = TOT_TICK_IVA1;

		if( RAM_FACTURA == 1 && TOT_TICK_IMPORTE_SIN_IVA > 0 && FACT_INDEP_DEL_TIPO_DE_CLIENTE == 1)
			MOSTRAR_FLOTANTE( 25, PRN, ENTEROS, DECIMALES, 1, 0, TOT_TICK_IVA1 + RAM_DIFERENCIA_REDONDEO_GRAV );
		if( RAM_FACTURA == 1 && TOT_TICK_IMPORTE_SIN_IVA < 0 && FACT_INDEP_DEL_TIPO_DE_CLIENTE == 1)
			MOSTRAR_FLOTANTE( 25, PRN, ENTEROS, DECIMALES, 1, 0, TOT_TICK_IVA1 - RAM_DIFERENCIA_REDONDEO_GRAV );

        LF( 1 );
        MOSTRAR_CADENA( 9, PRN, "TOTAL GRAVADO:" );
		if(TOT_TICK_IMPORTE_SIN_IVA > 0)  
			MOSTRAR_FLOTANTE( 25, PRN, ENTEROS, DECIMALES, 1, 0, (TOT_TICK_IMPORTE_SIN_IVA - RAM_DIFERENCIA_REDONDEO_GRAV));
		if(TOT_TICK_IMPORTE_SIN_IVA < 0)  
			MOSTRAR_FLOTANTE( 25, PRN, ENTEROS, DECIMALES, 1, 0, (TOT_TICK_IMPORTE_SIN_IVA + RAM_DIFERENCIA_REDONDEO_GRAV));
    }
    else {
        MOSTRAR_CADENA( 9, PRN, "TOTAL GRAVADO:" );
		/*if(EXENTO_PARA_DIPLOMATICOS())
			        MOSTRAR_FLOTANTE( 25, PRN, ENTEROS, DECIMALES, 1, 0, 0);
		else*/
		if(TOT_TICK_IMPORTE_SIN_IVA < 0)  
			MOSTRAR_FLOTANTE( 25, PRN, ENTEROS, DECIMALES, 1, 0,
                          TOT_TICK_IMPORTE_SIN_IVA + RAM_DIFERENCIA_REDONDEO_GRAV + TOT_TICK_IVA1 );
		if(TOT_TICK_IMPORTE_SIN_IVA > 0)  
			MOSTRAR_FLOTANTE( 25, PRN, ENTEROS, DECIMALES, 1, 0,
                          TOT_TICK_IMPORTE_SIN_IVA - RAM_DIFERENCIA_REDONDEO_GRAV + TOT_TICK_IVA1 );
    }
    LF( 1 );
    if( RAM_RETENCION_DGR ) {
      MOSTRAR_CADENA( 9, PRN, ST( S_RETENCION_DGR___ )  );
      MOSTRAR_FLOTANTE( 25, PRN, ENTEROS, DECIMALES, 1, 0, RAM_RETENCION_DGR  );
      LF( 1 );
    }
    MOSTRAR_CADENA( 9, PRN, "TOTAL EXENTO:" );
	/*if(EXENTO_PARA_DIPLOMATICOS())
		MOSTRAR_FLOTANTE( 25, PRN, ENTEROS, DECIMALES, 1, 0, TOT_TICK_EXENTO + TOT_TICK_IMPORTE_SIN_IVA + TOT_TICK_IVA1 );
	else*/
	if(TOT_TICK_EXENTO > 0)
		MOSTRAR_FLOTANTE( 25, PRN, ENTEROS, DECIMALES, 1, 0, TOT_TICK_EXENTO - RAM_DIFERENCIA_REDONDEO_EXENT );
	if(TOT_TICK_EXENTO < 0)
		MOSTRAR_FLOTANTE( 25, PRN, ENTEROS, DECIMALES, 1, 0, TOT_TICK_EXENTO + RAM_DIFERENCIA_REDONDEO_EXENT );
    LF( 1 );
    total_factura = SUMAR_TOTAL_FAC_CAJA(NO) + ROUND( RAM_RETENCION_DGR, ENTEROS, DECIMALES );
    MOSTRAR_CADENA( 9, PRN, "TOTAL:" );
    MOSTRAR_FLOTANTE( 25, PRN, ENTEROS, DECIMALES, 1, 0, total_factura );
    LF( 2 );
    for( h = 0 ;h < RAM_P_PAGO;h++ ) {
        if( medios[_PAGO_MODO( h )].tarjeta && VARIAS_TARJETAS_POR_PAGO ) {
            LEE_TARJETAS( posicion++, TARJETA_TRADICIONAL );
        }
		if( medios[_PAGO_MODO( h )].tarjeta ) {
            aux_fecha = GET_FECHA_VTO_PRIVADA(  ) ;
			SET_FECHA_VTO_PRIVADA(DECREMENTAR_MES( INCREMENTAR_MES( aux_fecha, -1 ) ) );
		}
        IMPRIMIR_TKT_MEDIOS( h, 0 );
		if( medios[_PAGO_MODO( h )].tarjeta ) {
			SET_FECHA_VTO_PRIVADA(DECREMENTAR_MES(aux_fecha ) );
		}
        LF( 1 );
    }
	if( RAM_AHORRO_OBTENIDO ) {
		MOSTRAR_CADENA( 1, PRN, "REBAJADO, UD. AHORRO $" );
		MOSTRAR_FLOTANTE( 14, PRN, ENTEROS, DECIMALES, 1, 0, RAM_AHORRO_OBTENIDO  );
	}
	LF( 1 );
    if( TKT_CONFIGURABLE ) {
        IMPRIMIR_TKT_VUELTO( h );
    }
}
/*****************************************************************************/
char * FORMATEA_NRI( char *nro_nri ) 
/*****************************************************************************/
{
    static char nri[16];    

    // rox Formateo el CUIT para EL SALVADOR --> NIT tiene 14 digitos y se van a 17 con los guiones.
    if( config.pais != ELSALVADOR ) {
        memcpy( nri, nro_nri, 15 );
    }
    else {
        int largo_nri;  

        memcpy( nri, nro_nri, 15 );     
        TRIM( nri );    
        largo_nri = strlen( nri );  
        nri[largo_nri - 1] = '-';   
        memcpy( &nri[largo_nri], &nro_nri[largo_nri - 1], 1 );  
        nri[15] = 0;
    }   

    return ( nri );
}   
/******************************************************************************/
void CALCULAR_MAXIMO_DATOS_ADICIONALES_MEDIO( int cod_sol, int nro_orden, long id_evento, int bin,
                                              int origen, int cant_max, int posicion )
/******************************************************************************/
{
    int area, pos = 0;
    struct _datos datos_aux;
    char archivo[30];
    memset( &datos_aux, 0, sizeof( datos_aux ) );
    SET_MEMORY_INT( __ram_cant_datos_adic_medio, 0 );
    BIN_DATOS_ADICIONALES( D_IMPRESION, 0, archivo );
    _unlink( archivo );
    if( !bin ) {
        area = SELECTED();
        //SELECT( AREA_DPAGO );
        SELECT_TABLE( T_EV_DMEDIOS, tipo_ev_dpago );
        //SET_ORDER( 1 );
        SET_ORDER2( "ID_EVENTO" );
        //GET_EQUAL( ( char* )&id_evento );
        SET_WHERE( "ID_EVENTO = '%ld'", id_evento );
        RUN_QUERY( NO );
        while( event_dpago->id_evento == id_evento && /*!BtrvEOF()*/ !dbEOF() ) {
            if( event_dpago->cod_solicitud == cod_sol && event_dpago->nro_orden == nro_orden
             && event_dpago->pos_pago == ( posicion + 1 ) ) {
                memcpy( &datos_aux.valor, event_dpago->valor, sizeof( datos_aux.valor ) );
                AGREGA_DATOS_MEDIO( 0, ( char )pos, nro_orden, 0, &datos_aux, cod_sol, D_IMPRESION,
                                    ( pos == 0 ), 0 );
                pos++;
                ADD_MEMORY_INT( __ram_cant_datos_adic_medio, 1 );
            }
            //SKIP( 1 );
            SKIP2( 1 );
        }
        SELECT( area );
    }
    else {
        struct _datos_en_bin datos_en_bin;
        int handle, salir = 0, codigos_en_archivo = 0, h;
        pos = 0;
        if( posicion > 0 ) {
            /*la posicion ya fue calculada en TKT_ejecutar_comando para el caso de cheques*/
            BIN_DATOS_ADICIONALES( origen, posicion, archivo );
            if( access( archivo, 0 ) == 0 ) {
                handle = _OPEN( archivo, O_RDONLY, __LINE__, __FILE__ );
                if( handle >= 0 ) {
                    codigos_en_archivo = filelength( handle ) / sizeof( datos_en_bin );
                    while( !salir && codigos_en_archivo > 0 ) {
                        if( !_READ( handle, ( char* )&datos_en_bin, sizeof( datos_en_bin ),
                                    __LINE__, __FILE__ ) ) {
                            if( datos_en_bin.cod_solicitud == cod_sol
                             && datos_en_bin.nro_orden == nro_orden ) {
                                memcpy( &datos_aux.valor, datos_en_bin.valor,
                                        sizeof( datos_en_bin.valor ) );
                                AGREGA_DATOS_MEDIO( 0, ( char )pos, nro_orden, 0, &datos_aux,
                                                    cod_sol, D_IMPRESION, ( pos == 0 ), 0.0 );
                                pos++;
                                ADD_MEMORY_INT( __ram_cant_datos_adic_medio, 1 );
                            }
                            codigos_en_archivo--;
                        }
                        else {
                            salir = 1;
                        }
                    }
                    _CLOSE( handle, __LINE__, __FILE__ );
                }
            }
        }
        else {
            for( h = 0;h < cant_max;h++ ) {
                BIN_DATOS_ADICIONALES( origen, h, archivo );
                if( access( archivo, 0 ) == 0 ) {
                    handle = _OPEN( archivo, O_RDONLY, __LINE__, __FILE__ );
                    if( handle >= 0 ) {
                        codigos_en_archivo = filelength( handle ) / sizeof( datos_en_bin );
                        while( !salir && codigos_en_archivo > 0 ) {
                            if( !_READ( handle, ( char* )&datos_en_bin, sizeof( datos_en_bin ),
                                        __LINE__, __FILE__ ) ) {
                                if( datos_en_bin.cod_solicitud == cod_sol
                                 && datos_en_bin.nro_orden == nro_orden ) {
                                    memcpy( &datos_aux.valor, datos_en_bin.valor,
                                            sizeof( datos_en_bin.valor ) );
                                    AGREGA_DATOS_MEDIO( 0, ( char )pos, nro_orden, 0, &datos_aux,
                                                        cod_sol, D_IMPRESION, ( pos == 0 ), 0.0 );
                                    pos++;
                                    ADD_MEMORY_INT( __ram_cant_datos_adic_medio, 1 );
                                }
                                codigos_en_archivo--;
                            }
                            else {
                                salir = 1;
                            }
                        }
                        _CLOSE( handle, __LINE__, __FILE__ );
                    }
                }
            }
        }
    }
}
/******************************************************************************/
int BUSCAR_DATO_ADICIONAL_MEDIO_PAGO( int posicion, char *cadena )
/******************************************************************************/
{
    struct _datos_en_bin datos_en_bin;
    char archivo[30];
    int handle;
    int salir = 0;
    int codigos_en_archivo = 0;
    int rta = 0;
    BIN_DATOS_ADICIONALES( D_IMPRESION, 0, archivo );
    //BIN_DATOS_ADICIONALES( D_MEDIOS_PRE_IMPORTE, posicion, archivo );
    if( access( archivo, 0 ) == 0 ) {
        handle = _OPEN( archivo, O_RDONLY, __LINE__, __FILE__ );
        if( handle >= 0 ) {
            codigos_en_archivo = filelength( handle ) / sizeof( datos_en_bin );
            while( !salir && codigos_en_archivo > 0 ) {
                if( !_READ( handle, ( char* )&datos_en_bin, sizeof( datos_en_bin ), __LINE__,
                            __FILE__ ) ) {
                    if( datos_en_bin.indice_posicion == posicion ) {
                        rta = 1;
                        strncpy( cadena, datos_en_bin.valor, sizeof( datos_en_bin.valor ) );
                        salir = 1;
                    }
                    codigos_en_archivo--;
                }
                else {
                    salir = 1;
                }
            }
            _CLOSE( handle, __LINE__, __FILE__ );
        }
    }
    return ( rta );
}
/**************************************************************************/
int ES_CREDITO_FISCAL( int ev )
/**************************************************************************/
{
    int rta = 0;
    if( FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
        switch( ev ) {
            case _EVENT_FAC_A:
            case _EVENT_NC_A:
            case _EVENT_FAC_A_REMOTA:
            case _EVENT_NC_A_REMOTA:
            case _EVENT_TICKET:
            case _EVENT_TICKET_ESPERA:
                rta = 1;
        }
    }
    else {
        rta = 1;
    }  

    return rta;
}
/******************************************************************************/
double SUMAR_TOTAL_FAC_CAJA( int recalcular )
/******************************************************************************/
{
	int i = 0, decimales_=0;
	double total = 0.00, tot_imp = 0.00, tot_aux = 0.00, super_total = 0.00 ;
	double diferencia=0.00;
	double diff2=0.00;
	int ivaa=0;
	
	decimales_ = DECIMALES;
	if( IMPRESORA_FISCAL == FISCAL && ITEM_PEDIR_PRECIO == 1 && (!RAM_FACTURA && !RAM_TICKET_FACTURA) || RAM_NOTA_CR)
		decimales_ = DECIMALES + 2;

	if( PRECALCULAR_IMPUESTOS ){
		for( i = 0; i < 4; i++ ) {
			tot_imp += _TOT_TICK_IMPUESTOS( i );
		}
	}
	//tot_iva1 = ROUND( TOT_TICK_IVA1, ENTEROS, DECIMALES );

    if( IMPRESORA_FISCAL == FISCAL && ( RAM_FACTURA == 1 || RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1  ) )
		total = TOT_TICK_IMPORTE_SIN_IVA
			  + TOT_TICK_IVA2
			  + TOT_TICK_EXENTO + tot_imp;
    else
		total = ROUND( TOT_TICK_IMPORTE_SIN_IVA, ENTEROS, decimales_ )
			  + ROUND( TOT_TICK_IVA2, ENTEROS, decimales_ )
			  + ROUND( TOT_TICK_EXENTO, ENTEROS, decimales_ ) + tot_imp;
			  // + ROUND( RAM_RETENCION_DGR, ENTEROS, DECIMALES );

	if(TOT_TICK_EXENTO > 0)
		total -= ROUND(RAM_DIFERENCIA_REDONDEO_EXENT, ENTEROS, decimales_);
	if(TOT_TICK_EXENTO < 0)
		total += ROUND(RAM_DIFERENCIA_REDONDEO_EXENT, ENTEROS, decimales_);
	
	if(TOT_TICK_IMPORTE_SIN_IVA > 0)
		total -= ROUND(RAM_DIFERENCIA_REDONDEO_GRAV, ENTEROS, decimales_);
	if(TOT_TICK_IMPORTE_SIN_IVA < 0)
		total += ROUND(RAM_DIFERENCIA_REDONDEO_GRAV, ENTEROS, decimales_);

		
	//Factura, Creditos Fiscales y Notas de Credito
	if( (RAM_FACTURA == 1 || RAM_FACTURA == 2) && RAM_NOTA_CR !=1 && TOT_TICK_IMPORTE_SIN_IVA < 0 /*&& FACT_INDEP_DEL_TIPO_DE_CLIENTE == 1*/)
		total += ROUND( TOT_TICK_IVA1 - RAM_DIFERENCIA_REDONDEO_GRAV, ENTEROS, decimales_ ); 
	if( (RAM_FACTURA == 1 || RAM_FACTURA == 2) && RAM_NOTA_CR !=1  && TOT_TICK_IMPORTE_SIN_IVA > 0 /*&& FACT_INDEP_DEL_TIPO_DE_CLIENTE == 1*/)
		total += ROUND( TOT_TICK_IVA1 + RAM_DIFERENCIA_REDONDEO_GRAV, ENTEROS, decimales_ ); 
	
	//Ticket Comun
	if( !RAM_FACTURA || RAM_NOTA_CR == 1 ) {
	    if( IMPRESORA_FISCAL == FISCAL && ( RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1))
			total += TOT_TICK_IVA1 + RAM_DIFERENCIA_REDONDEO_GRAV;
		else
			total += ROUND( TOT_TICK_IVA1 + RAM_DIFERENCIA_REDONDEO_GRAV, ENTEROS, decimales_ ); 
    }

	//total += ROUND( RAM_RETENCION_DGR, ENTEROS, DECIMALES );
	if( IMPRESORA_FISCAL == FISCAL && ( recalcular ==SI || RAM_PUNTOS_ESP_3 > 0.001) )
	{

		double total2= 0.00, dif2 = 0.00;
		double impresora_total = 0.0;
	//	double importe1, importe2, iva_1,iva_2, exento_s, imp_int1, imp_int2;
		double importe1_[10], iva_1_[10], exento_s, imp_int1_[10];
		int ii= 0;
		
		exento_s = TOT_TICK_EXENTO;
		
		for(ii = 0; ii < 10; ii++) {
			importe1_[ii] = _TOT_TICK_IMP_NETO_GRAVADO(ii);
			iva_1_[ii] = _TOT_TICK_IMP_TASA(ii);
			imp_int1_[ii] = _TOT_TICK_IMP_INT_POR_COD_IVA(ii);
		}
		

		if( RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR ==1 ) {
			//ojo con el redondeo de 0.826449 -> 0.8265 la funcion round2 hace 0.8264
			int corregir;
			corregir = CORRECCION_REDONDEO_SISTEMA( &exento_s );
			ROUND2(&exento_s, ENTEROS, 2);
			for( ii = 0; ii < 10; ii++) {
				corregir = CORRECCION_REDONDEO_SISTEMA( &importe1_[ii] );
				ROUND2(&importe1_[ii], ENTEROS, 2);
				corregir = CORRECCION_REDONDEO_SISTEMA( &iva_1_[ii] );
				ROUND2(&iva_1_[ii], ENTEROS, 2);
				corregir = CORRECCION_REDONDEO_SISTEMA( &imp_int1_[ii] );
				ROUND2(&imp_int1_[ii], ENTEROS, 2);
			}

		}
		super_total = exento_s;
		for( ii = 0; ii < 10; ii++) {
			super_total += importe1_[ii] + iva_1_[ii] + imp_int1_[ii];
		}
		
		total2 = total;
		ROUND2(&total2,ENTEROS,2);
		ROUND2(&super_total,ENTEROS,2);
	
		if( RAM_PUNTOS_ESP_3 > 0.001 )
			impresora_total = RAM_PUNTOS_ESP_3;
		else
			impresora_total = fabs(super_total);

		diferencia = fabs( fabs(total) - impresora_total) ; //saquemos la diferencia en valor absoluto
		dif2 = fabs( total - total2);
	
		if( RAM_PUNTOS_ESP_3 > 0.001 && ( diferencia > 0.03 || dif2 > 0.03 )) //el tot+esc pide el subtotal pero no se actualiza cuando tickeo uno nuevo despues
			impresora_total = fabs(super_total);
	
	
		if(( RAM_PUNTOS_ESP_3 > 0.001 || (fabs(diferencia) > 0.001 ))  &&( RAM_TICKET_FACTURA == 1 ||  RAM_MODO_DEVOLUCION == _NC_TOTAL || RAM_MODO_DEVOLUCION == _NC_RF_INTERME || RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL)
			&& impresora_total > 0.001 && (fabs( ( fabs( total2) - impresora_total)) > 0.001
 			&& fabs(( fabs( total2 ) - impresora_total))  < 0.051 || dif2 > 0.001)) {
				//simulemos 0.01
				int exento = NO, posicion;
				double porc = 0.00, tot_aux2 = 0.00, tot_aux1= 0.00  ;

			/*	if( RAM_PUNTOS_ESP_3 > 0.001 && _TICKET_ORIGEN( RAM_P_TICKET - 1) != _ORIGEN_PROMOCION )
					posicion = 0;
				else*/
					posicion = RAM_P_TICKET - 1;

				if( fabs( _TICKET_EXENTO( posicion)) > 0.01) {
					int signos = 1;

					if( RAM_NOTA_CR )
						signos *= -1;

					ADD_MEMORY_DOUBLE( __tot_tick_exento, diferencia * signos );
					_ADD_MEMORY_DOUBLE( __ticket_exento, posicion,  diferencia * signos );
					_ADD_MEMORY_DOUBLE( __ticket_precio, posicion, diferencia * signos );
					exento = SI;
				} else {
					int signos = 1;
					double dd;//guardemos los valores iniciales para revertir con tot+esc si hay promos
					ivaa = _TICKET_COD_IVA(posicion);
					porc = iva[ivaa].porc_iva1 / 100.0 ;  //0.21
					
					if(  fabs(total) - impresora_total > 0)
						signos = -1; //descontar los totales

					if( _TICKET_ORIGEN( posicion) == _ORIGEN_PROMOCION && !( _TICKET_TIPO(posicion ) & _RESTA)) {
						if( RAM_NOTA_CR == 0 ) {
							//SET_MEMORY_DOUBLE( __ram_puntos_esp_4, diferencia); //GUARDEMOS la diferencia original si hay promos reversadoas
							diferencia_parcial[i_contador++] = diferencia;
						}
					}
   						//ojo con la conversion de double a float que pierdo precision y para los articulos por importe es critico
						dd =/*(float)*/ _TICKET_IMPORTE_SIN_IVA(posicion) ;
						//_SET_MEMORY_FLOAT( __ticket_alicuota_precio, posicion, dd - _TICKET_IMP_INT(posicion)  ); //guardemos el valor original
						_SET_MEMORY_DOUBLE( __ticket_descuento, posicion, dd - _TICKET_IMP_INT(posicion)  ); //guardemos el valor original
						
						//_SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, posicion, dd - _TICKET_IMP_INT(posicion)  ); //guardemos el valor original
   						dd = _TICKET_PRECIO(posicion) ;
   						//Reusamos __ticket_ila_rec
						_SET_MEMORY_FLOAT( __ticket_ila_rec, posicion , dd );
   						dd = _TICKET_IVA1_PRECIO (posicion) ;
   						//Reusamos __ticket_iha_rec
						_SET_MEMORY_FLOAT( __ticket_iha_rec, posicion , dd );
				    //}
						if( _TICKET_ORIGEN( posicion) == _ORIGEN_PROMOCION && ( _TICKET_TIPO(posicion ) & _RESTA && RAM_NOTA_CR == 0)) {
						//recuperemos la diferencia
							//diferencia = RAM_PUNTOS_ESP_4; //GUARDEMOS la diferencia original si hay promos reversadoas
							diferencia = diferencia_parcial[i_contador++];
						}


					//if(RAM_TICKET_FACTURA || !(_TICKET_ORIGEN( posicion) == _ORIGEN_PROMOCION && ( _TICKET_TIPO(posicion ) & _RESTA))){ //las reversiones de promo no
					if(RAM_TICKET_FACTURA || ( RAM_NOTA_CR  )){ //las reversiones de promo no
					    
						if( RAM_NOTA_CR ) {
							signos *= -1;
						}
						
						ADD_MEMORY_DOUBLE( __tot_tick_importe_sin_iva, (diferencia - (porc * diferencia)) * signos );
						ADD_MEMORY_DOUBLE( __tot_tick_iva1, (porc* diferencia) * signos );
						//agregarlo a lot tot


				
						if(  (fabs(total) - impresora_total) > 0.001) //recuperemos el signo
							signos = -1; //descontar los totales */
						else 
							signos = 1; //aumentar los totales */
						
						
						if( RAM_PUNTOS_ESP_3 > 0.00  && fabs(RAM_PUNTOS_ESP_3 - fabs(super_total)) > 0.009){  //solo lo cambio cuando la impresora me de el total
								_ADD_MEMORY_DOUBLE( __tot_tick_imp_neto_gravado,ivaa, diferencia * signos);
						}
					    
						
						if( RAM_NOTA_CR == 0 ) {
							if( posicion > 0 &&_TICKET_IMPORTE_SIN_IVA( posicion) > 0.00 ) 
								signos *= -1;
							if( posicion > 0 &&_TICKET_IMPORTE_SIN_IVA( posicion) > 0.00 && RAM_PUNTOS_ESP_3 < 0.001) 
								signos *= -1;
						
							if( _TICKET_ORIGEN( posicion) == _ORIGEN_PROMOCION && ( _TICKET_TIPO(posicion ) & _RESTA ))
								signos *= -1;
						}else {
							if( _TICKET_ORIGEN( posicion) == _ORIGEN_PROMOCION) {
								if (!( _TICKET_TIPO(posicion ) & _RESTA ))
								signos *= -1;
							} else
								if( RAM_PUNTOS_ESP_3 < 0.001)
									signos *=-1;
						}
						
						if( _TICKET_CANTIDAD(posicion )) {
							tot_aux = diferencia /	 _TICKET_CANTIDAD(posicion );
							tot_aux = FABS( tot_aux);
							diferencia = tot_aux;
						}
						 
						if( RAM_PUNTOS_ESP_3 < 0.001) {
							tot_aux2= _TICKET_IMPORTE_SIN_IVA(posicion);
							tot_aux1 = _TICKET_IVA1_PRECIO( posicion);
							tot_aux=  _TICKET_PRECIO(posicion);
						}
						if( RAM_PUNTOS_ESP_3 < 0.001 && _TICKET_TIPO(posicion ) & _RESTA && _TICKET_ORIGEN( posicion) != _ORIGEN_PROMOCION) {
							signos *=-1;
						}
						//if( RAM_NOTA_CR == 0  ) { //mmm
							_ADD_MEMORY_DOUBLE( __ticket_importe_sin_iva, posicion, (diferencia - (porc * diferencia))* signos   );
							_ADD_MEMORY_DOUBLE( __ticket_iva1_precio, posicion, (porc* diferencia)* signos  );
							_ADD_MEMORY_DOUBLE( __ticket_precio, posicion, diferencia * signos );
						//}
						}
						}
				MODIFICAR_EN_E_TICKET( posicion, exento );
				if( RAM_PUNTOS_ESP_3 < 0.001 && RAM_NOTA_CR > 0 && fabs(tot_aux2) > 0.001) { //para nc anda? recuperamos el valor original asi no manda distintos valores a la impresora para un mismo articulo
					_SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, posicion, tot_aux2   );
					_SET_MEMORY_DOUBLE( __ticket_iva1_precio, posicion, tot_aux1  );
					_SET_MEMORY_DOUBLE( __ticket_precio, posicion, tot_aux);
		         }
				SET_MEMORY_DOUBLE( __ram_puntos_esp_3, 0.00);
		}
		
		diff2 = impresora_total - fabs(RAM_TOTAL);
		if( fabs(diff2) < 0.57900 && (fabs(diff2) > 0.005  )) {
			int sig = 1;
			if( fabs(RAM_TOTAL) > impresora_total )
				sig = -1;
			if( RAM_NOTA_CR )
				 sig *= -1;
			ADD_MEMORY_DOUBLE( __ram_total,	 fabs(diff2) * sig );
			if( RAM_PUNTOS_ESP_3 > 0.00  && fabs(RAM_PUNTOS_ESP_3 - fabs(super_total)) > 0.009){  //solo lo cambio cuando la impresora me de el total
				ivaa = _TICKET_COD_IVA(0);
				_ADD_MEMORY_DOUBLE( __tot_tick_imp_neto_gravado,ivaa, fabs(diff2) * sig);
				if( total < 0.00)
					super_total = -RAM_PUNTOS_ESP_3 ;
				else
					super_total = RAM_PUNTOS_ESP_3 ;
			}
		}
		total = super_total;
	
		if( RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1 ) {
			double importe3;
			int posicion = RAM_P_TICKET - 1;
			int ivaa2 = _TICKET_COD_IVA(posicion );

			importe3 = importe1_[ivaa2];
			_SET_MEMORY_DOUBLE( __tot_tick_imp_neto_gravado,ivaa2, importe3);
			SET_MEMORY_DOUBLE( __tot_tick_exento, exento_s);
		}

	}
    return( fabs( total ) );
	
}

/*****************************************************************************/
void AGREGA_BIN_A_EXTENDIDO( int h, int origen, char anulado )
/*****************************************************************************/
{
	struct _datos_en_bin datos_en_bin;
	char      archivo[19];
	int       handle;
	int       salir = 0;
	int       codigos_en_archivo = 0;

   BIN_DATOS_ADICIONALES( origen, h, archivo );

	if( access( archivo, 0 ) == 0 ) {
		handle = _OPEN( archivo, O_RDONLY, __LINE__, __FILE__ );

		if( handle >= 0 ) {
			codigos_en_archivo = filelength( handle ) / sizeof( datos_en_bin );
			while( !salir && codigos_en_archivo > 0 ) {
				if( !_READ( handle, ( char * ) &datos_en_bin, sizeof( datos_en_bin ),
						__LINE__, __FILE__ ) ) {
               INSERT_A_BTRV_EXT( &datos_en_bin, anulado, h + 1 );
					codigos_en_archivo--;
				} else
					salir = 1;
			}
			_CLOSE( handle, __LINE__, __FILE__ );
		}
		_unlink( archivo );
	}
}

/******************************************************************************/
int INSERT_A_BTRV_EXT( struct _datos_en_bin *datos_en_bin, char anulado, int posicion )
/******************************************************************************/
{
  	char      archivo[16];
	int       handle;
	int       error = 1;

   struct _datos_en_bin_ext {
      struct _datos_en_bin datos_bin;
      char   anulado;
      int    posicion;
   } datos_ext;


	sprintf( archivo, "D_IMP_2.TMP");

   memset( &datos_ext, 0, sizeof( datos_ext ) );
   //if( indice_pos == 0 && primer_dato )_unlink( archivo );
   memcpy( &datos_ext, datos_en_bin, sizeof( struct _datos_en_bin ) );
	datos_ext.posicion = posicion;
	datos_ext.anulado  = anulado;

	handle = _OPEN_O_CREAT( archivo, O_WRONLY, __LINE__, __FILE__ );

	if( handle >= 0 ) {
		LSEEK( handle, 0, SEEK_END, __LINE__, __FILE__ );
		error =
			_WRITE( handle, ( char * ) &datos_ext, sizeof( datos_ext ), __LINE__,
			__FILE__ );
		_CLOSE( handle, __LINE__, __FILE__ );
	}

	return !error;
}


/***************************************************************************/
int REALIZAR_RETENCION_DGR( void )
/***************************************************************************/
{
   return (( RAM_NRO_CLIENTE > 0 )
      && ( clientes.cond_iva != 5 )
      && ( RAM_FACTURA || RAM_TICKET_FACTURA ||( RAM_REPROCESANDO && event_ticket.tipo_evento == 3 ) )
      && ( clientes.exento_ing_brutos == 'N' || clientes.exento_ing_brutos == 'n' )
      && !RAM_BIEN_DE_USO );
}

/***************************************************************************/
int REALIZAR_PERCEPCION_DGI( void )
/***************************************************************************/
{
   return ( ES_FACTURA_A( clientes.cond_iva ) && clientes.realizar_percepcion_dgi != 'N'
      && !RAM_BIEN_DE_USO );

}

/********************************<*******************************************/
int REALIZAR_PERCEPCION_TACIS( void )
/***************************************************************************/
{
   return (( porc_percep_tacis )? 1 : 0 );
   //si existe porcentaje de percepcion entonces siginifca que el cliente esta habilitado y dentro del rango permitido
   /*( ( clientes.cond_iva < 5 || clientes.cond_iva == 6 || clientes.cond_iva == 7 )
      && clientes.realizar_percepcion_tacis != 'N' );*/
}

//**********************************************************************/
float OBTENER_ALICUOTAS_DGR( int tasa_iibb, int cod_iva, double * minimo_retenc_dgr )
/* Para obtener el porcentaje de retencion a ese cliente segun la tasa e iva del cliente
  por ahora el cod_iva no lo usamos
*/
//**********************************************************************/
{
   float alicuota = 0.0;
   int area_ant = 0, tipo_ant = 0;
   int calcularNC = 1;

   area_ant = SELECTED2();
   tipo_ant = SELECTED_TIPO();

   if( OPEN_TABLE( T_PORCPERC, TT_ORIG, ( char* )&porcperc, sizeof( struct _porcperc ) )  == 0 ) {
		//SET_ORDER2("TASA_IIBB");
		SET_WHERE("TASA_IIBB ='%i'", tasa_iibb );
		RUN_QUERY(NO);
		if( FOUND2() ){
 			alicuota = (float)porcperc.alicuota;
 			*minimo_retenc_dgr = porcperc.minimo;
			if( RAM_NOTA_CR && !CONTROL_DIAS_EN_DEVOLUCION( porcperc.tipo_control_dias_percep,
				porcperc.dias_devolucion_percep, &calcularNC ) ) {
         		alicuota = 0;//no calcula la alicuota
				minimo_retenc_dgr = 0;
			}
        } else {
            if( MODO_DEBUG == 50 || MODO_DEBUG == 44 ){
				char auxi[22];

				memset(auxi,0,sizeof(auxi));
				_snprintf( auxi, sizeof(auxi)-1, "s%", "NO SE ENCUENTRA PORCENT PERC" );
				GRABAR_LOG_SISTEMA( auxi, LOG_DEBUG,2 );
            }
        }
        CLOSE_TABLE( T_PORCPERC, TT_ORIG );
   }else
      if( ( MODO_DEBUG == 50 || MODO_DEBUG == 44 ) ){
		char auxi[22];
		_snprintf( auxi, sizeof(auxi)-1, "s%", "NO SE ABRE ALICUOTA" );
        GRABAR_LOG_SISTEMA( auxi, LOG_DEBUG,2 );
      }

   SELECT_TABLE( area_ant, tipo_ant );

   return ( alicuota );
}

//**********************************************************************/
float OBTENER_ALICUOTAS( int municipio, int cod_iva, double importe ,unsigned  fecha_fin_a,
       unsigned fechaini, unsigned fechafin )
//**********************************************************************/
{
	float alicuota_ = 0.0;
	int calcular_alic_nc  = 0;
	int area_ant, tipo_ant = 0;

	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	if( municipio && OPEN_TABLE( T_ALICUOTA, TT_ORIG, ( char* )&alicuota, sizeof( struct _alicuota ) )  == 0 ) {
		//SET_ORDER2("COD_MUNICIPIO, CODICION_IVA");
		SET_WHERE("COD_MUNICIPIO ='%i' CODICION_IVA ='%i'", municipio, cod_iva );
		RUN_QUERY(NO);
        if( FOUND2() 
				&& ( importe >= alicuota.minimo 
					//CONTROL DE LA FECHA DE ACREDITACIÓN
					|| ( fecha_fin_a > 0 && ((unsigned)GET_FECHA_DOS( )) > fecha_fin_a ) ) ) {
 			alicuota_ = (float)alicuota.alicuota;

			if( RAM_NOTA_CR && !CONTROL_DIAS_EN_DEVOLUCION( alicuota.tipo_control_dias_percepcion,
				alicuota.dias_devolucion_percep, &calcular_alic_nc ) ) {
         		alicuota_ = 0;//no calcula la alicuota
			}
         	//rango de cobro de la percepcion
         	if( alicuota_ > 0 && (((unsigned) GET_FECHA_DOS( )) > fechafin) || ( ((unsigned)GET_FECHA_DOS( )) < fechaini ) ) {
         	   alicuota_ = 0;//no calcula la alicuota
         	}
         } else {
			/*if( MODO_DEBUG == 50 || MODO_DEBUG == 44 ){
               char auxi[100];
               sprintf(auxi,"/NO ENC.ALIC/%i/%i ",indice_percep.cod_munic, indice_percep.cod_ivam );
               GRABAR_LOG_SISTEMA(auxi);
            }*/

         }

        CLOSE_TABLE( T_ALICUOTA, TT_ORIG );

   } else
      if( ( MODO_DEBUG == 50 || MODO_DEBUG == 44 ) && municipio ){
		char auxi[22];

		memset(auxi,0,sizeof(auxi));
		_snprintf( auxi, sizeof(auxi)-1, "s%", "NO SE ABRE ALICUOTA" );
        GRABAR_LOG_SISTEMA( auxi, LOG_DEBUG,2 );
      }

   SELECT_TABLE( area_ant, tipo_ant );

   return ( alicuota_ );
}

//**********************************************************************/
int CONTROL_DIAS_EN_DEVOLUCION(int tipo,int dias_devolucion, int *calcularNC )
//**********************************************************************/
{ int base =  1;         //VEAMOS LOS TIEMPOS

        *calcularNC = 0;

         switch( tipo ) {
            case 1:// SE CONTROLARÁ LA CANTIDAD DE DIAS CALENDARIO
               if( dias_devolucion > 0 ) {
                  if( DIF_DIAS( GET_FECHA_DOS(  ), RAM_FECHA_ANTERIOR ) <= dias_devolucion ) {
                     *calcularNC = 1;
                  } else {
                     base = 0;
                  }
               } else {
                  base = 0;
                  MENSAJE("NO SE CONFIGURO LOS DIAS DE PERCEPCION");
               }
               break;
            case 2:// NO HAY LIMITE PARA LA DEVOLUCION DE PERCEPCIONES
                *calcularNC = 1;
               break;
            case 3: // DENTRO DEL MISMO MES CALENDARIO
               if( MON( RAM_FECHA_ANTERIOR )!= MON( GET_FECHA_DOS( ))) {
                  *calcularNC = 0;
                  base = 0;
               } else
                  *calcularNC = 1;
               break;
            default:
               *calcularNC = 0;
               base = 0;
               break;
         }
         //if( ( *calcularNC > 0 ) && fabs( RAM_RETENCION_DGR ) < 0.001 ) {
            //*calcularNC = 0; //no hubo retenciones originalmente, no se tienen que devolver
            //base = 0;
         //}
   return base;
}

/***********************************************************************/
int OBTENER_DIGITOS( char *valor, int tipo_dato )
/***********************************************************************/
{
	int cant_dig = 0;
	
	switch( tipo_dato ) {
		case _SHORT_INT:
			cant_dig = 1;
			break;
		case _INT:
			cant_dig = 4;
			break;
		case _FLOAT:
			cant_dig = 6;
			break;
		case _DOUBLE:
			cant_dig = 15;
			break;
		case _STRING:
			cant_dig = sizeof( valor );
			break;
		/*case _DATE :
			cant_dig = 8;
			break;
		case _HORA:
			cant_dig = 5;
			break;*/
		case _LONG:
			cant_dig = 10; 
			break;
		/*case _UNSIGNED:
			cant_dig = 4;
			break;*/
	}

	return( cant_dig );
}

#ifdef  DEBUG_IMPRESION	
/*****************************************************************************/
void LF_AUX(int cantidad){
/*****************************************************************************/		
int i = 0;
	if( strlen( AUX_LINEA ) > 1 ){
		GRABAR_LOG_COMPROBANTES( AUX_LINEA );
		AUX_COLUMNA = 0;
		memset(AUX_LINEA,0,sizeof(AUX_LINEA));
	}
	for( i = 0; i < cantidad; i++ ) {
		GRABAR_LOG_COMPROBANTES("#");
		
	}
}
		
#endif
#ifdef DEBUG_IMPRESION	
/*****************************************************************************/
void IMPRIME_AUX(int x,char *cadena){
/*****************************************************************************/
	
	if( x != AUX_COLUMNA ){
		
		if( AUX_COLUMNA < x ) {
			for( ; AUX_COLUMNA < x; AUX_COLUMNA ++ ){
				strcat( AUX_LINEA,"." );
			}
		}
	}
	AUX_COLUMNA = x;
	strcat(AUX_LINEA,cadena);
	}	
#endif
/********************************************************************************************/
int DEBE_AGRUPAR_ARTICULOS_CON_DECIMALES( int h ){
/********************************************************************************************/
int rta = 0;

		if( _TICKET_TIPO3( h ) & _PESABLE && PERMITE_AGRUPAR_PESABLES_EN_COMP && 
			_TICKET_ORIGEN( h ) != _ORIGEN_PROMOCION ){
			rta = 1;
		}else{
			if( fabs( fmod( _TICKET_CANTIDAD( h ), 1.00 ) ) > 0.0005 ){
				rta = 0;
			}else{
				rta = 1;
			}
		}


return rta;
}
/******************************************************************************/
void AGREGAR_EXT_BTRV( int actualizo_ev )
/******************************************************************************/
{
	//struct _datos_en_bin datos_en_bin;
	char      archivo[16];
	int       handle;
	int       salir = 0;
	int       codigos_en_archivo = 0;

	struct _datos_en_bin_ext {
      struct _datos_en_bin datos_bin;
      char   anulado;
      int    posicion;
   } datos_ext;


   sprintf( archivo, "D_IMP_2.TMP");
   if( !NRO_FECHA_OPERACION )
      SET_MEMORY_INT( __nro_fecha_operacion, _GET_FECHA_DOS(  ) );
	if( access( archivo, 0 ) == 0 ) {
		handle = _OPEN( archivo, O_RDONLY, __LINE__, __FILE__ );

		//SELECT( AREA_DPAGO );
		SELECT_TABLE( T_EV_DMEDIOS, tipo_ev_dpago );
		if( handle >= 0 ) {
			codigos_en_archivo = filelength( handle ) / sizeof( datos_ext );
			while( !salir && codigos_en_archivo > 0 ) {
				if( !_READ( handle, ( char * ) &datos_ext, sizeof( datos_ext ),
						__LINE__, __FILE__ ) ) {
					memset( event_dpago, 0, sizeof( struct _ev_dmedios ) );

					event_dpago->caja  	        = event_ticket.caja;
					event_dpago->nro_z 	  		  = event_ticket.nro_z;
               event_dpago->id_evento 	     = ( actualizo_ev ) ? RAM_ID_ULTIMO_EVENTO : RAM_ID_EVENTO;
					event_dpago->caja_z          = event_ticket.caja_z;
					event_dpago->pos_pago = datos_ext.posicion;
					event_dpago->indice_posicion = datos_ext.datos_bin.indice_posicion;
					event_dpago->nro_orden = datos_ext.datos_bin.nro_orden;
					event_dpago->id_dato = datos_ext.datos_bin.id_dato;
					memcpy( event_dpago->valor, datos_ext.datos_bin.valor,
						sizeof( event_dpago->valor ) - 1 );
					event_dpago->valor[30] = 0;
					event_dpago->boletin_consultado = datos_ext.datos_bin.boletin_consultado;
					event_dpago->cod_supervisor = datos_ext.datos_bin.cod_supervisor;
					event_dpago->cod_sucursal = config.sucursal;
					if( !NRO_FECHA_OPERACION )
				      SET_MEMORY_INT( __nro_fecha_operacion, GET_FECHA_DOS() );

					event_dpago->fecha_ticket = NRO_FECHA_OPERACION;
					strncpy( event_dpago->valor_en_boletin, datos_ext.datos_bin.valor_en_boletin,
						sizeof( event_dpago->valor_en_boletin ) );
					event_dpago->cod_solicitud = datos_ext.datos_bin.cod_solicitud;
					event_dpago->anulado = datos_ext.anulado;
					event_dpago->posicion = datos_ext.posicion;
					INSERT2(NO);
					codigos_en_archivo--;
				} else
					salir = 1;
			}
			_CLOSE( handle, __LINE__, __FILE__ );
		}
		unlink( archivo );
	}
}

/*****************************************************************************/
int CANTIDAD_DIGITOS( double cantidad, int enteros  )//parte entera
/*****************************************************************************/
{
   int rta = 0;
   char auxiliar[10] ;

   memset( auxiliar,'9', enteros );

   if( atof( auxiliar ) < FABS( cantidad ) ) {
      //dnc reemplazo los if por un ciclo , maximo permitido 10 digitos. 1.000.000.000
      while( FABS( cantidad ) >= 1 && rta < 10 ) {
         cantidad = cantidad / 10;
         rta++;
      }
   } else
      rta = enteros;

   return ( rta );
}

/*****************************************************************************/
double FABS( double x )
/*****************************************************************************/
{
   if ( x < 0 )
      x = -x;

   return( x );
}

//**********************************************************************/
int AUTORIZAR_RETENCION_NC_ORIG( )
//**********************************************************************/
{
	int ok = 1;
	char aux[55];
	memset( aux, 0, sizeof(aux) );

	if( porc_retenc_dgr 
			&& CALCULAR_RETENCION_DGR 
			&& REALIZAR_RETENCION_DGR(  )
			&& ( RAM_MODO_DEVOLUCION == _NC_LIBRE 
			|| RAM_MODO_DEVOLUCION == _DEVOL_LIBRE ) ) {
		_snprintf(aux, sizeof(aux), ST( S_DESEA_CALCULAR_RETENCIONES_LF_ENTER_ESC )
			,porc_retenc_dgr );
		aux[sizeof( aux ) - 1] = '\0';
		if( MENSAJE_CON_TECLA_S_N( aux,
					13, 27 ) == 13 ) {
			//Se calcula retencion para verificar si en funcion del importe y el porcetaje configurado
			//hay retenciones. Esto es mas que nada para importes y porcentajes pequeños donde el calculo
			//es practiamente cero.
			if( RETENCION( 0.0, NO ) ) {
				SET_MEMORY_INT( __ram_calcular_retenc_nc , SI );
			} else {
				SET_MEMORY_INT( __ram_calcular_retenc_nc , NO );
				ok = 0;
				memset( aux, 0, sizeof(aux) );
				_snprintf(aux, sizeof(aux), "%s %s", ST( S_RETENCION_MENOR_A ), ST( S____PRESIONE_UNA_TECLA__ ) );
				aux[sizeof( aux ) - 1] = '\0';
				MENSAJE_TECLA( aux );
			}
		} else {
			SET_MEMORY_INT( __ram_calcular_retenc_nc , NO );
			ok = 0;
   			/* valores posibles
   			0- no calcula nada
   			1- calcula solo retencion
   			2- calcula solo percepcion
   			3- calcula percepcion y retencion
			*/
		}
	}
   
	return( ok );
}
//**********************************************************************/
int AUTORIZAR_RETENCION_NC(double retencion_calculada )
//**********************************************************************/
{
	int ok = 1;
	char aux[55];
	memset( aux, 0, sizeof(aux) );

	if( retencion_calculada != 0.0 && ( RAM_MODO_DEVOLUCION == _NC_LIBRE 
			|| RAM_MODO_DEVOLUCION == _DEVOL_LIBRE ) ) {
		_snprintf(aux, sizeof(aux), ST( S_DESEA_CALCULAR_RETENCIONES_LF_ENTER_ESC )
			,porc_retenc_dgr );
		aux[sizeof( aux ) - 1] = '\0';
		if( MENSAJE_CON_TECLA_S_N( aux,
					13, 27 ) == 13 ) {
			//Se calcula retencion para verificar si en funcion del importe y el porcetaje configurado
			//hay retenciones. Esto es mas que nada para importes y porcentajes pequeños donde el calculo
			//es practiamente cero.
			if( retencion_calculada ) {
				SET_MEMORY_INT( __ram_calcular_retenc_nc , SI );
			} else {
				SET_MEMORY_INT( __ram_calcular_retenc_nc , NO );
				ok = 0;
				memset( aux, 0, sizeof(aux) );
				_snprintf(aux, sizeof(aux), "%s %s", ST( S_RETENCION_MENOR_A ), ST( S____PRESIONE_UNA_TECLA__ ) );
				aux[sizeof( aux ) - 1] = '\0';
				MENSAJE_TECLA( aux );
			}
		} else {
			SET_MEMORY_INT( __ram_calcular_retenc_nc , NO );
			ok = 0;
   			/* valores posibles
   			0- no calcula nada
   			1- calcula solo retencion
   			2- calcula solo percepcion
   			3- calcula percepcion y retencion
			*/
		}
	}
   
	return( ok );
}

//**********************************************************************/
int AUTORIZAR_PERCEPCION_NC_ORIGINAL( )
//**********************************************************************/
{
	int ok = 1;
	char aux[55];
	memset( aux, 0, sizeof(aux) );

	if( CALCULAR_PERCEPCION_DGI 
			&& REALIZAR_PERCEPCION_DGI(  )
			&& ( RAM_MODO_DEVOLUCION == _NC_LIBRE 
				|| RAM_MODO_DEVOLUCION == _DEVOL_LIBRE ) ) {
		_snprintf(aux, sizeof(aux), ST( S_DESEA_CALCULAR_PERCEPCIONES_ENTER_ESC )
			,porc_retenc_dgr );
		aux[sizeof( aux ) - 1] = '\0';

		if( MENSAJE_CON_TECLA_S_N( aux,
					13, 27 ) == 13 ) {
			//Se calcula percepcion para verificar que el no sea por debajo de 0.01. 
			//Esto es mas que nada para valores pequeños donde el calculo
			//es practiamente cero.
			//Tarea:5216,seteamos provisoriamente el valor ram_calcular_retenc_nc para que calcule la percepcion,
			//luego volvemos a tu valor original
			ADD_MEMORY_INT( __ram_calcular_retenc_nc, 99 );
			if( PERCEPCION( 0.0, SI ) ) {
				//Retorno su valor original
				ADD_MEMORY_INT( __ram_calcular_retenc_nc, -99 );
				//Seteo valor normalmente
				ADD_MEMORY_INT( __ram_calcular_retenc_nc , 2 );
			} else {
				//Seteo valor normalmente, como es cero no hace falta revertir el seteo anterior
				ADD_MEMORY_INT( __ram_calcular_retenc_nc , NO );
				ok = 0;
				memset( aux, 0, sizeof(aux) );
				_snprintf(aux, sizeof(aux), "%s %s", ST( S_PERCEPCION_MENOR_A ), ST( S____PRESIONE_UNA_TECLA__ ) );
				aux[sizeof( aux ) - 1] = '\0';
				MENSAJE_TECLA( aux );
			}
		} else {
			ADD_MEMORY_INT( __ram_calcular_retenc_nc , NO );
			ok = 0;
   			/* valores posibles
   			0- no calcula nada
   			1- calcula solo retencion
   			2- calcula solo percepcion
   			3- calcula percepcion y retencion
			*/
		}
	}
   
	return( ok );

}
//**********************************************************************/
int AUTORIZAR_PERCEPCION_NC( double percepcion_calculada)
//**********************************************************************/
{
	int ok = 1;
	char aux[55];
	memset( aux, 0, sizeof(aux) );

	if( percepcion_calculada!=0.0 && ( RAM_MODO_DEVOLUCION == _NC_LIBRE 
				|| RAM_MODO_DEVOLUCION == _DEVOL_LIBRE ) ) {
		_snprintf(aux, sizeof(aux), ST( S_DESEA_CALCULAR_PERCEPCIONES_ENTER_ESC )
			,porc_retenc_dgr );
		aux[sizeof( aux ) - 1] = '\0';

		if( MENSAJE_CON_TECLA_S_N( aux,
					13, 27 ) == 13 ) {
			//Se calcula percepcion para verificar que el no sea por debajo de 0.01. 
			//Esto es mas que nada para valores pequeños donde el calculo
			//es practiamente cero.
			//Tarea:5216,seteamos provisoriamente el valor ram_calcular_retenc_nc para que calcule la percepcion,
			//luego volvemos a tu valor original
			ADD_MEMORY_INT( __ram_calcular_retenc_nc, 99 );
			if( percepcion_calculada ) {
				//Retorno su valor original
				ADD_MEMORY_INT( __ram_calcular_retenc_nc, -99 );
				//Seteo valor normalmente
				ADD_MEMORY_INT( __ram_calcular_retenc_nc , 2 );
			} else {
				//Seteo valor normalmente, como es cero no hace falta revertir el seteo anterior
				ADD_MEMORY_INT( __ram_calcular_retenc_nc , NO );
				ok = 0;
				memset( aux, 0, sizeof(aux) );
				_snprintf(aux, sizeof(aux), "%s %s", ST( S_PERCEPCION_MENOR_A ), ST( S____PRESIONE_UNA_TECLA__ ) );
				aux[sizeof( aux ) - 1] = '\0';
				MENSAJE_TECLA( aux );
			}
		} else {
			ADD_MEMORY_INT( __ram_calcular_retenc_nc , NO );
			ok = 0;
   			/* valores posibles
   			0- no calcula nada
   			1- calcula solo retencion
   			2- calcula solo percepcion
   			3- calcula percepcion y retencion
			*/
		}
	}
   
	return( ok );

}

//**********************************************************************/
double RETORNAR_TOTAL_PRINTER( double variacion )
/*Esta función retorna el total calculado de la impresora 
y ademas calcula cuanto va a dar la impresora, pensado para los recargos de tarjeta por ejemplo
*/
//**********************************************************************/
{
		double importe1, importe2,importe3, iva_1,iva_2,iva_3, exento_s, imp_int1, imp_int2,imp_int3, super_total;

		importe1 = _TOT_TICK_IMP_NETO_GRAVADO(0);
		importe2 = _TOT_TICK_IMP_NETO_GRAVADO(1);
		importe3 = _TOT_TICK_IMP_NETO_GRAVADO(2);
		iva_1 = _TOT_TICK_IMP_TASA(0);
		iva_2 = _TOT_TICK_IMP_TASA(1);
		iva_3 = _TOT_TICK_IMP_TASA(2);
		exento_s = TOT_TICK_EXENTO;
		imp_int1 = _TOT_TICK_IMP_INT_POR_COD_IVA(0);
		imp_int2 = _TOT_TICK_IMP_INT_POR_COD_IVA(1);
		imp_int3 = _TOT_TICK_IMP_INT_POR_COD_IVA(2);
		if( RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR ==1 ) {
			//ojo con el redondeo de 0.826449 -> 0.8265 la funcion round2 hace 0.8264
			int corregir;
			corregir = CORRECCION_REDONDEO_SISTEMA( &importe1 );
		}
		if( fabs( variacion) > 0.01) { //para las variaciones
			double ivas = ( 1 + iva[articulo.cod_iva].porc_iva1 / 100 );
			double psiva = 0.00 ,iva_a = 0.00;
			if( ivas > 0.01 ) {
				if( RAM_NOTA_CR )
					variacion = -variacion;
				psiva = variacion / ivas; //obtenemos el total sin iva /ver exentos e impuestos internos.
				iva_a =  variacion - psiva;
				ROUND2(&psiva,ENTEROS,2);
				//ROUND2(&iva_a,ENTEROS,2); //el iva no se redondea sino el resultado
				iva_1 += iva_a;
				ROUND2(&importe1, ENTEROS, 2);
				ROUND2(&importe2, ENTEROS, 2);
				ROUND2(&importe3, ENTEROS, 2);
				ROUND2(&iva_1,ENTEROS,2); 
				ROUND2(&iva_2,ENTEROS,2);
				ROUND2(&iva_3,ENTEROS,2);
				ROUND2(&imp_int1,ENTEROS,2); 
				ROUND2(&imp_int2,ENTEROS,2); 
				ROUND2(&imp_int3,ENTEROS,2); 
				super_total = psiva + importe1 + importe2+ importe3+ iva_1 +iva_2+ iva_3 +exento_s+ imp_int1 +imp_int2  +imp_int3;
			}
			    
		} else {
			ROUND2(&importe1, ENTEROS, 2);
			ROUND2(&importe2, ENTEROS, 2);
			ROUND2(&importe3, ENTEROS, 2);
			ROUND2(&iva_1, ENTEROS, 2);
			ROUND2(&iva_2, ENTEROS, 2);
			ROUND2(&iva_3, ENTEROS, 2);
			ROUND2(&exento_s, ENTEROS, 2);
			ROUND2(&imp_int1, ENTEROS, 2);
			ROUND2(&imp_int2, ENTEROS, 2);
			ROUND2(&imp_int3, ENTEROS, 2);
			super_total = importe1 + importe2 + importe3 + iva_1 + iva_2+ iva_3+ exento_s + imp_int1 + imp_int2+ imp_int3;
		}


		return( super_total);

}


//**********************************************************************/
int RETORNAR_TOTAL_PRINTER_POR_IVA( double variacion )
/*Esta función retorna el total calculado de la impresora 
por iva para controlar los valores parciales. Se necesita la estructura item cargada
*/
//**********************************************************************/
{
		double importe[3],iva_[3],exento_s, imp_int[3], super_total = 0.00;
		int supera = NO;

		importe[0] = _TOT_TICK_IMP_NETO_GRAVADO(0);
		importe[1] = _TOT_TICK_IMP_NETO_GRAVADO(1);
		iva_[0] = _TOT_TICK_IMP_TASA(0);
		iva_[1] = _TOT_TICK_IMP_TASA(1);
		exento_s = TOT_TICK_EXENTO;
		imp_int[0] = _TOT_TICK_IMP_INT_POR_COD_IVA(0);
		imp_int[0] = _TOT_TICK_IMP_INT_POR_COD_IVA(1);
		if( RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR ==1 ) {
			//ojo con el redondeo de 0.826449 -> 0.8265 la funcion round2 hace 0.8264
			int corregir;
			corregir = CORRECCION_REDONDEO_SISTEMA( &importe[articulo.cod_iva] );
		}
		if( fabs( variacion) > 0.01) { //para las variaciones
			double ivas = ( 1 + iva[articulo.cod_iva].porc_iva1 / 100 );
			double psiva = 0.00 ,iva_a = 0.00;
			if( ivas > 0.01 ) {
				if( RAM_NOTA_CR )
					variacion = -variacion;
				psiva = variacion / ivas; //obtenemos el total sin iva /ver exentos e impuestos internos.
				iva_a =  variacion - psiva;
				ROUND2(&psiva,ENTEROS,2);
				//iva_[ += iva_a;
				ROUND2(&iva_[articulo.cod_iva],ENTEROS,2); 
				super_total = fabs(importe[articulo.cod_iva]) - fabs(psiva);
				if( super_total < 0.0001)
					supera = SI;
				if( supera == NO && super_total < 0.0001)
					super_total = fabs(iva_[articulo.cod_iva]) - fabs(iva_a);
				if( supera == NO && super_total < 0.0001)
					supera = SI;
			}
			    
		} 
		return( supera);

}

//**********************************************************************/
double RETORNAR_PRECIO_IMPRESORA( double cantidad, double precio_siva )
/*Esta función retorna el total que daria la impresora en base al precio y la cantidad
*/
//**********************************************************************/
{
	double total= 0.00, ivaa= 0.00;
	int corregir;
	total = cantidad * precio_siva;  //199.995 * 0.905 = 180.995475
	corregir = CORRECCION_REDONDEO_SISTEMA( &total );
	ROUND2(&total, ENTEROS, 2); //181
	if( ITEM_GRAVADO ) {
		ivaa= total * iva[articulo.cod_iva].porc_iva1 / 100; // 181 * 0.105 = 19.005;
		corregir = CORRECCION_REDONDEO_SISTEMA( &ivaa );
		ROUND2(&ivaa, ENTEROS, 2); //19.005
	}
	total = total + ivaa;
	ROUND2(&total, ENTEROS, 2); //19.005
	return total;

}

//**********************************************************************/
double RETORNAR_IMPORTES_IMPRESORA( double precio_final, double *iva_1 )
/*Esta función retorna el precio sin iva del item de impresora e iva correspondiente
pasando el precio final como parametro
*/
//**********************************************************************/
{
	double total= precio_final, ivaa= 0.00;
	int corregir;
	if( ITEM_GRAVADO ) {
		total= precio_final / (1 + iva[articulo.cod_iva].porc_iva1 / 100); // 289.99 / 1.21
		corregir = CORRECCION_REDONDEO_SISTEMA( &total );
		ROUND2(&total, ENTEROS, 2); //239.66
		*iva_1 = precio_final - total; // 50.33
	}else
		ROUND2(&total, ENTEROS, 2); //239.66
	return total ;

}
