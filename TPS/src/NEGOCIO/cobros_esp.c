#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <actual.h> 
#include <bancos.h>
#include <btrv.h> 
#include <b_mens.h> 
#include <cajon.h> 
#include <cambio.h> 
////#include <cio.h> 
#include <clave.h> 
#include <cmemory.h> 
#include <cobros.h> 
#include <cobros_esp.h> 
#include <comprob.h> 
#include <cr.h>  
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <descuent.h> 
#include <eventos.h> 
#include <fecha.h> 
#include <ftoc.h> 
#include <func.h> 
#include <f_fijo.h> 
#include <getch.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <import_t.h> 
#include <imp_comp.h> 
#include <log.h> 
#include <malloc.h> 
#include <med_rein.h> 
#include <mensaje.h> 
#include <menu.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <pagos.h> 
#include <pagos_esp.h> 
#include <pausa.h> 
#include <print.h> 
#include <recupero.h> 
#include <retiros.h> 
#include <round.h> 
#include <socios.h> 
#include <stack.h> 
#include <stand_by.h> 
#include <tkt.h> 
#include <trim.h> 
#include <xyz.h> 
#include <_cr1.h> 
#include <aarch.h>
#include <path.h>
#ifdef INVEL_W
#include <io.h>
#else
#include <sys/io.h>
#endif
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 * configurar: 
 * config.pais                               ARGENTINA
 * NRO_CLIENTE_POR_DEFECTO: cliente por defecto para COBRO A CUENTA DE FUTUROS SOCIOS.
 * PRESENTACION_A_TOMAR:    NO SE USA MAS
 * CUOTAS HABILITADAS                        SI
 * MENU CUOTAS                               SI
 * AUTORIZACION ON LINE HABILITADA           NO
 * DONACION DE VUELTO                        NO HABILITADO
 * UTILIZA PERFILES                          NO
 * PEDIR CODIGO DE FINANCIACION              NO
 * PROTOCOLO AUTORIZACION TARJETA            ARCHIVO    
 * NEGOCIO                                   SERVICIOS
 * PAUSA EN MENSAJE                          0 A 127
 * ******************************************************************************
 */
#if defined(INVEL_L)
#include <string.h>
#include <actions.h>
#include <sys/io.h>
#endif
#if defined(INVEL_W)
//
#include <dos.h>
#include <string.h>
#include <actions.h>
#endif
#include <myodbc.h>
struct varios_esp
{
    char nroCompTemp[10];
    char importeTemp[20];
    int Compania;
    long Comprobante;
    double Importe;
    double ImporteMin;
    long Id_deuda;
    long nroCli;
    long id_cli_cobro;
    long prepaga;
    long Compania2;
    int TipoCli;
};
struct event_varios_esp
{
    int codigo;
    long id_cobro;
    long comprobante;
    long cuenta;
    double importe;
};
struct _ind_deuda
{
    INT16 grupo_cobro; //unidad de negocio
    INT16 cod_cobro;
    char letra_comp;
    INT16 punto_venta;
    long nro_comp;
    INT16 tipo_mov;
};
struct _ind_deuda2
{
    INT16 cod_cobro;
    long nro_socio;
};
struct _ind_deuda3
{
    INT16 cod_cobro;
    long nro_socio;
    INT16 fecha;
};
struct _ind_deuda4
{
    INT16 cod_cobro;
    long nro_socio;
    INT16 tipo_cli;
    INT16 fecha;
};
struct _ind_tipo_mov
{
    INT16 grupo_cobro; //unidad de negocio
    INT16 cod_cobro;   
    INT16 cod_interno;
};
struct _CliImpreso
{
    long idCli;         // Id del cliente
    int TipoCobro;       // Tipo de cobro (Pago con comprobante, a cuenta, manual)
};
struct _conf
{
    int CantCopias;                        // Cantidad de copias a imprimir
    int CantTimbrado;                      // Cantidad de Timbrados
    int TimbradoOpcional;                  // Timbrado Opcional
};
struct _config_impresion
{
    struct  _conf SmgPComp;                // Swiss Medical Pago con Comprobante
    struct  _conf SmgPcuenta;              // Swiss Medical Pago a Cuenta
    struct  _conf SmgRmanual;              // Swiss Medical Recibo Manual
    struct  _conf SmgFuturoSocio;          // Swiss Medical Futuro Socio
    struct  _conf OtrosPComp;              // Otros Pago con Comprobante
    struct  _conf OtrosPCuenta;            // Otros Pago a Cuenta
    struct  _conf OtrosRManual;            // Otros Recibo Manual
    struct  _conf SocioBaja;               // Socio Dado de Baja
    struct  _conf OtrosIngresos ;          // Otros Ingresos
}ConfImpresion;
struct _cobro_inc
{
    long evento_cobro_completo;
    int id_ev_cobro_actual;
};
int PROCESAR_TECLAS_COBROS_ESPECIALES( int k, struct DatosCobro *datosCobro );
void GRABAR_EVENTO_REPROCESO_PAGO_ESP( void );
void GRABAR_EVENTO_COBROS_VARIOS_ESP( int codigo, int id, long comprobante, long cuenta,
                                      double importe, char *cta_cmr, long idDeuda, int scanner,
                                      long cobro2, long unidadnegocio, int pos, long codSocio,
                                      long idSocio, char tipoSocio, int motivo );
int GRABAR_COD_BARRA_EN_BINARIO_ESP( char *cod_barra, int pos );
void TECLA_REPROCESAR_PAGO_COBRO_ESP( struct DatosCobro *datosCobro );
char *LEER_COD_BARRA_EN_BINARIO_ESP( int posicion );
void TICKET_COBRANZA_VARIOS_ESP( char *concepto, int comprobante, long cuenta, double importe,
                                 char *cta_cmr, char cabecera );
int MENU_SELECCIONA_COD_COBRO( void );
void GUARDAR_EVENTO_COMPLETO( int valor );
int OBTENER_CLI_COMPROBANTE( struct _DatosCodBarra *DatosCodBarra, struct DatosCobro *datosCobro );
int REIMPRIMIR_COBRANZAS( void );
int GUARDAR_POSICION_RECIBO( long cliente, int motivo, int pos, long recibo, int indice );
int AGREGAR_DETALLES_COBRO( long IdDeuda, char *nombreCobro, char *nroComp, char *importeAPagar );
long PEDIR_DATOS_COBROS_ESP( void );
int CALCULAR_IMPORTE_DEUDA( double *importe, double *importeMin, int pregunta );
void CARGAR_CONF_IMPRESION();
int EVENTO_COBRO_COMPLETO();
void GUARDAR_COBRO_EN_REC( struct DatosCobro *datosCobro );
void EVENTO_COBROS_TOTAL( struct DatosCobro *datosCobro );
void PANTALLA_COBROS_ESP( void );
int COBRO_POR_COD_BARRA( struct DatosCobro *datosCobro, long *Comprobante, int *Compania,
                         double *Importe, long *IdDeuda );
void IMPRIMIR_COBROS_ESP( struct DatosCobro *datosCobro, int timbrar );
void MOSTRAR_COBRO_GRID( void );
void GRABAR_SUB_DETALLE( struct DatosCobro *datosCobro );
void CEREAR_VAR_TICKET( struct DatosCobro *datosCobro );
int CARGAR_LETRA_TIPO( int grupo, int compania, int cod_interno );
void TIMBRADO_OPCIONAL( char *msg, int funSup, int indice );
int OBTENER_TIPO_CLI( void );
void GUARDAR_ID_COBRO( long valor );
void ACTUALIZA_COPIAS();
void IMPRIMIR_CLIENTE( struct DatosCobro *datosCobro, int j, int todos );
void OBTENER_CONF_TKT( struct DatosCobro *datosCobro, struct _conf **Conf );
void RECUPERAR_COBRO( struct DatosCobro *datosCobro );
int IMPORTAR_COBROS( int tipo_importacion, int tipo_evento );
double IMPORTE_CLIENTE( long id_cliente );
void GRABAR_EVENTOS();
int REVERSAR_COBRO( struct DatosCobro *datosCobro );
int ELEGIR_UNI_NEG( int grilla, struct DatosCobro *datosCobro );
void TERMINAR_COBRO( struct DatosCobro *datosCobro );
int MOSTRAR_DEUDA( long   nroSocio, int Compania, struct DatosCobro *datosCobro );
void LIMPIAR_PANEL( int    grillas, int cliente );
void MOSTRAR_SOCIO( struct DatosCobro *datosCobro, int posCobro );
int CLI_PROCESADO( long cliente, int tipoCobro, struct _CliImpreso *clieImpresos );
void EVENTO_COBRO( struct DatosCobro *datosCobro );
int ELEGIR_DEUDA( int grilla, int cant, struct _ind_deuda4 *indice, struct DatosCobro *datosCobro );
int LLENAR_PANEL( struct _DatosCodBarra *datos, long *Comprobante, int *Compania, double *Importe,
                  long cliente );
int SET_VALORES( struct DatosCobro *datosCobro, struct varios_esp *datos_temp, int );
int PEDIR_DATOS( struct DatosCobro *structura );
void SETEAR_CONF( char *grupo, char *variable, char *valor );
void PAGO_COBRO( struct DatosCobro *datosCobro );
void INIT_COBRO( struct DatosCobro *datosCobro );
int ID_COBRADO( long idDeuda, char pagado );
void IniStructs( struct DatosCobro *structura );
void SETEAR_VAR( struct _conf *grupo, char *variable, char *valor );
int REIMPRIMIR( int timbrado, int funSup );
void REC_MEM( void );
void TIMBRAR( struct DatosCobro *datosCobro, struct _conf *Conf, int j, int funSup );
void MOVTOC( int codigo, char *tipoMov );
void CARGAR_REC_DESDE_EVENT( long idEvent, struct DatosCobro *datosCobroTmp );
int CONTINUAR_TIMBRADO();
void GUARDAR_EVENTOS_ESP( struct DatosCobro *datosCobro );
void DUPLICAR_DPAGO( long idEvento );
int DEUDA_MAS_ANTIGUA( long nroSocio, int Compania, int tipoCli, long idDeuda );
void SETEAR_ACUMULADORES( void );
int CONTROL_IMPORTES_EN_REVERSOS( int nro, double importe );
void ACTUALIZAR_ESTADO_EVENTOS( void );
void CEREAR_TEMPORALES( void );
#define _CANT          GET_MEMORY_INT( _cantidad )
#define _TIPO_CLI      datosCobro->_TipoCli
#define _ID_CLIENTE    datosCobro->_id_cliente
#define _CODIGO        datosCobro->_codigo
#define _DATO          datosCobro->_dato
#define _UNI_NEG       datosCobro->_Compania
#define _COMPROBANTE   datosCobro->_Comprobante
#define _NRO_CLIENTE   datosCobro->_nroCliente
#define _IMPORTE       datosCobro->_Importe
#define _ID_DEUDA      datosCobro->_IdDeuda
#define _MOTIVO        datosCobro->_motivo
#define _POSICION      datosCobro->_posicion
#define _RECIBO        datosCobro->_recibo
#define _OP_MULTIPLE         GET_MEMORY_INT( operMultiples )
#define CANTIDAD_DE_SERVICIOS_POR_COBRO_ESP  500
extern int calcHojas;
extern int cantCopias;
extern int cantTotalCopias;
int operMultiples;
struct   _tipo_movimiento tipo_mov; //tabla letra de comprobante / tipo de movimiento
double SubTotal;
long idCobrados[CANTIDAD_DE_SERVICIOS_POR_COBRO_ESP];
int _ptr,_handle,cantidad;
int _cantidad,_estado;
struct   DatosCobro *__datosCobro;
static int exigir_ingreso_de_valor = 0;
void FTOC_2( double numero, char *cadena, int enteros, int decimales, int signo, int comas );
void FTOA_2( double importe, char *cadena, int enteros, int decimales, int signo, int comas,
             int *signo_ );
extern int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos;
/***********************************************************************/
int COBROS_ESP( void )                        //inicio del cobro
/***********************************************************************/
{
    int go = 0, pos = 0;
    struct   DatosCobro datosCobro;
    __datosCobro = &datosCobro;
    IniStructs( &datosCobro );
    _estado = STACK_INS( sizeof( short ) );
    REC_MEM();
    CARGAR_CONF_IMPRESION();
    if( /*USE_DB( AREA_COBROS, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 )*/
		OPEN_TABLE(T_COBROS, TT_ORIG ,( char* )&cobros, sizeof( cobros ))== 0 ) {
		//SELECT_TABLE( T_COBROS, TT_ORIG );
        PANTALLA_COBROS_ESP();
        while( !go ) {
            if( _ESTADO == 0 ) {
                /*if( !USED( AREA_COBROS ) ) {
                    USE_DB( AREA_COBROS, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0,
                            0 );
                }*/  
                SubTotal = 0;
                LIMPIAR_PANEL( SI, SI );
                SET_MEMORY_INT( _cantidad, 0 );
                datosCobro._nroCliente = 0;
                RECUPERAR_COBRO( &datosCobro ); //por si esta activado el flag de operaciones multiples
                INIT_COBRO( &datosCobro );
            }
            if( _ESTADO == 1 ) {
                /*if( !USED( AREA_COBROS ) ) {
                    USE_DB( AREA_COBROS, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0,
                            0 );
                }*/
                SubTotal = 0;
                LIMPIAR_PANEL( SI, SI );
                RECUPERAR_COBRO( &datosCobro );
                datosCobro._nroCliente = 0;
                INIT_COBRO( &datosCobro );
            }
            if( _ESTADO == 11 ) {
                SubTotal = 0;
                LIMPIAR_PANEL( SI, SI );
                RECUPERAR_COBRO( &datosCobro );
                _SET_ESTADO( 2 );
            }
            if( _ESTADO == 2 ) {
                /*se almacena id evento para eventual recupero*/
                GUARDAR_ID_COBRO( RAM_ID_EVENTO );
                /*----------------- Busca los datos de la factura ---------------------*/
                DATOS_COMPROBANTE( _FACTURA );
                PAGO_COBRO( &datosCobro ); 
                cantidad = _CANT;
            }
            if( _ESTADO == 3 ) {
                SETEAR_ACUMULADORES();
                _SET_ESTADO( 31 );
            }
            if( _ESTADO == 31 ) {
                if( EVENTO_COBRO_COMPLETO() == 0 ) {
                    GRABAR_LOG_SISTEMA( "recuperamos cobro" , LOG_ERRORES,3);
                    RECUPERAR_COBRO( &datosCobro );
                    cantidad = _CANT;
                }
                /*se almacena id evento para eventual recupero*/
                GUARDAR_ID_COBRO( RAM_ID_EVENTO );
                //Se comienzan a guardar los eventos 
                GUARDAR_EVENTO_COMPLETO( NO );
                GRABAR_LOG_SISTEMA( "EVENTOS NO COMPLETOS", LOG_ERRORES,3);
                GUARDAR_EVENTOS_ESP( &datosCobro );
                /*cierra y abre los eventos, para asegurar que se guardan*/
                /*CERRAR_ARCHIVOS_EVENTOS_LOCAL();
                ABRIR_ARCHIVOS_EVENTOS_LOCAL();*/
                //Fin del guardado de los eventos (evento completo)
                GUARDAR_EVENTO_COMPLETO( SI );
                GRABAR_LOG_SISTEMA( "EVENTOS SI COMPLETOS" , LOG_ERRORES,3);
                _SET_ESTADO( 32 );
            }
            if( _ESTADO == 32 ) {
                GRABAR_EVENTOS();
                _SET_ESTADO( 41 );
            }
            if( _ESTADO == 41 ) {
                cantidad = _CANT;
                if( MODO_NEGOCIO == SERVICIOS && MODO_DEVOLUCION ) {
                    MENSAJE( "FIN DE OPERACION REVERSO" );
                }
                else {
                    IMPRIMIR_COBROS_ESP( &datosCobro, SI );
                }                
                _SET_ESTADO( 42 );
            }
            if( _ESTADO == 42 ) {
                _SET_ESTADO( 4 );
            }
            if( _ESTADO == 4 ) {
                TERMINAR_COBRO( &datosCobro );
                GUARDAR_ID_COBRO( RAM_ID_EVENTO );
                _SET_ESTADO( 0 );
                CLOSE_TABLE( T_COBROS, TT_ORIG );
            }
            if( _ESTADO == 5 ) {
                FIN_DE_TICKET( SI );
                _SET_ESTADO( 0 );
                CLOSE_TABLE( T_COBROS, TT_ORIG );
            }
            if( _ESTADO == 51 ) {
                FIN_DE_TICKET( SI );
                _SET_ESTADO( 0 );
            }
            if( _ESTADO == 6 ) {
                go = 1;
                _SET_ESTADO( 0 );            
                CLOSE_TABLE( T_COBROS, TT_ORIG );
                return 0;
            }
            if( _ESTADO == 7 ) {
                int cnt;
                // anulacion de ultimo item 
                cnt = _CANT;
                SET_MEMORY_INT( _cantidad, ( cnt - 1 ) );
                _SET_ESTADO( 1 );
                CLOSE_TABLE( T_COBROS, TT_ORIG );
            }
            /*Si estaba en un pago de reintegro*/
            if( _ESTADO == 133 ) {
                SETEAR_FUNCION( 17, SI );
                PAGOS_ESPECIALES();
                _SET_ESTADO( 0 );
            }
        }
    }
    return( 0 );
}
/***********************************************************************/
void INIT_COBRO( struct DatosCobro *datosCobro )
/***********************************************************************/
{
    int seguir = 1, pago = 0;
	int  area_ant, tipo_ant;
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	 
	if( OPENED( T_OPE_CLIENTE, TT_ORIG ) ||
		OPEN_TABLE( T_OPE_CLIENTE, TT_ORIG, ( char* )&oper_cli, sizeof( oper_cli ) )  == 0 ) {
  //  if( USED( AREA_OPER_CLI )
   //  || USE_DB( AREA_OPER_CLI, _OP_CLI_SIC, ( char* )&oper_cli, sizeof( oper_cli ), NULL, 0, 0 )
   //  == 0 ) {
		SELECT_TABLE( T_OPE_CLIENTE, TT_ORIG );
		RUN_QUERY(NO);

        if( !RECCOUNT2( NULL, NULL) ) {
            MENSAJE_STRING( S_NO_HAY_DEUDAS_CARGADAS );
            BORRAR_MENSAJE();
        }
        if( _ESTADO == 0 ) {
            CEREAR_VAR_TICKET( datosCobro );
        }
        cantidad = _CANT;
        if( cantidad == 0 ) {
            char cad_aux[30];
            sprintf( cad_aux, "IdEvento: %li", RAM_ID_EVENTO );
            GRABAR_LOG_SISTEMA( cad_aux , LOG_DEBUG,4);
        }
        else {
            char cad_aux[30];
            sprintf( cad_aux, "Rec IdEv: %li", RAM_ID_EVENTO );
            GRABAR_LOG_SISTEMA( cad_aux , LOG_DEBUG,4);
        }
        while( seguir > 0 ) {
            if( cantidad < CANTIDAD_DE_SERVICIOS_POR_COBRO_ESP ) {
                pago = 0;
                seguir = PEDIR_DATOS( datosCobro );
                if( seguir == 1 ) {
                    GUARDAR_COBRO_EN_REC( datosCobro );
                    ADD_MEMORY_INT( _cantidad, 1 );
                    cantidad = _CANT;
                    datosCobro->_nroCliente = 0;
                    _SET_ESTADO( 1 );
                }
                else {
                    if( seguir == 2 ) {
                        // se cargaron los cobros y se va a pagos
                        seguir = 0;
                        pago = 1;
                    }
                    else {
                        if( seguir < 0 ) {
                            //termino el cobro por anulacion
                            if( seguir == -660 ) {
                                _SET_ESTADO( 6 );
                            }
                            else if( seguir == -650 ) {
                                //anulacion ultimo items
                                _SET_ESTADO( 7 );
                            }
                            else {
                                _SET_ESTADO( 5 );
                                SET_MEMORY_DOUBLE( __ram_total, 0.0 );
                            }
                        }
                    }
                }
                if( cantidad == CANTIDAD_DE_SERVICIOS_POR_COBRO_ESP && _ESTADO != 2 ) {
                    MENSAJE_STRING( S_NO_MAS_SERVICIOS );
                    MENSAJE_CON_TECLA_STRING( S_PRESIONE_ENTER_PARA_CONTINUAR, 13 );
                    BORRAR_MENSAJE();
                    seguir = 0;
                    pago = 1;
                }
            }
            else {
                MENSAJE_STRING( S_NO_MAS_SERVICIOS );
                MENSAJE_CON_TECLA_STRING( S_PRESIONE_ENTER_PARA_CONTINUAR, 13 );
                BORRAR_MENSAJE();
                seguir = 0;
                pago = 1;
            }
        }
        if( ( pago == 1 && seguir == 0 ) && _ESTADO == 1 ) {
            _SET_ESTADO( 2 ); //termino el cobro y paso a pagos
            SET_MEMORY_DOUBLE( __ram_total, SubTotal );
        }
        //CLOSE_DATABASE( AREA_OPER_CLI );
		 CLOSE_TABLE( T_OPE_CLIENTE, TT_ORIG );
    }
	SELECT_TABLE( area_ant, tipo_ant );
}
/***********************************************************************/
int PEDIR_DATOS( struct DatosCobro *datosCobro ) //secuencia de pedido de datos
/***********************************************************************/
{
    char buffer[50];
    int rta = 0,importeEnt = 0,importeDec = 0, cod_solicitud = 0;
    //   char      importeStr[10];
    struct varios_esp datos_temp;
    int _k = 0, seguir = 1;
    short pedir_supervisor = 0;
    int estado_tmp = 0, cod_aux = 0, estado_ant;
    int mensaje = 0, tipo_busqueda = 0;
    int reintentar = 3, _r = 0;
    /*PIDE EL CODIGO DE BARRA*/
    datos_temp.nroCli = 0;
    datos_temp.id_cli_cobro = 0;
    datos_temp.Comprobante = 0;
    datosCobro->_motivo = 0;
    datosCobro->CodigoBarra.codigo[0] = '\0';
    ExecuteAction( A_MUESTRA_DATO_CODIGO_BARRA );
    rta = PEDIR_CADENA_NUMERICA( ( char )3, ( char )16, ( char )100,
                                 datosCobro->CodigoBarra.codigo, NULL, 0, SI, NO, NULL );
    _k = rta;
    while( seguir ) {
        rta = _k;
        rta = PROCESAR_TECLAS_COBROS_ESPECIALES( rta, datosCobro );
        seguir = 0;
        estado_tmp = 0;   
        switch( rta ) {
            case -108:
                /*---- ALT L ----*//*---- Consulta Socio Alfabetico ---*/
                SETEAR_FUNCION( 18, SI );
                rta = 0;
                estado_tmp = 1;
                tipo_busqueda = _BUSQ_ALFABETI;
                break;
            case -109:
                /*---- ALT K ----*//*---- Consulta Socio Numerico ---*/
                SETEAR_FUNCION( 19, SI );
                rta = 0;
                estado_tmp = 1;
                tipo_busqueda = _BUSQ_CODSOCIO_UNICO;        
                rta = 0;
                break;
            case -659:
                /* stand by */
                SETEAR_FUNCION( 14, SI );
                STAND_BY( 0, _MODO_COBRANZA );
                rta = 0;
                break;
            case -660:
                //cierre x
                BORRAR_ARCHIVO_REC( ARCH_REC_CIERRE );
                BORRAR_ARCHIVO_REC( ARCH_REC_DIFERENCIAS );
                SETEAR_FUNCION( 158, SI );
                rta = -660;
                break;
            case -662:
                //Fondo fijo
                if( FONDO_FIJO_MULTIPLES_MEDIOS ) {
                    SETEAR_FUNCION( 58, SI );
                    CR1_FONDO_FIJO_MULT_MEDIOS();
                }
                else {
                    SETEAR_FUNCION( 5, SI );
                    CR1_FONDO_FIJO();
                }
                rta = 0;
                break;
            case -663:
                //Retiros                
                SETEAR_FUNCION( 8, SI );
                SET_MEMORY_CHAR( __ram_p_pago, 0 );
                CR4_RETIROS();
                _SET_ESTADO( 51 );
                rta = 0;
                break;
            case -133:
                //pagos de reintegros
                SETEAR_FUNCION( 17, SI );
                estado_ant = _ESTADO;       //Guarda estado actual
                _SET_ESTADO( 133 );         //Setea el estado en 133 por si hay un corte dentro de reintegro
                PAGOS_ESPECIALES();
                //_SET_ESTADO( estado_ant );  //Regresa al estado original
                _SET_ESTADO( 51 );
                rta = 0;
                break;
            case -658:
                //reproceso de cobro
                SETEAR_FUNCION( 29, SI );
                #ifdef COMPILAR_TECLA_REPROCESAR
                TECLA_REPROCESAR_PAGO_COBRO_ESP( datosCobro );
                _SET_ESTADO( 51 );
                #endif      
                rta = 0;
                break;
            case -667:
                //reverso de cobro
                if( !_CANT ) // si no hay cobros cargados
                {
                    SETEAR_FUNCION( 91, SI );
                    if( REVERSAR_COBRO( datosCobro ) ) {
                        _SET_ESTADO( 3 );
                    }
                    else {
                        _SET_ESTADO( 51 );
                    }                
                    rta = 0;
                }
                break;
            case 132:
                /*---- Menu de funciones ---*/
                SETEAR_FUNCION( 1, SI );
                _k = SELECCIONA_FUNCION( 5 );
                seguir = 1;
                break;
                #if defined (ALTA_RAPIDA_REINTEGROS)
            case -64:
                /*---- F6 ----*//*---- Carga r�pida de reintegros ---*/         
                SETEAR_FUNCION( 46, SI );
                MEDIO_REINT();
                _SET_ESTADO( 51 );
                rta = 0;
                break;
                #endif
            case 150:
                /*---- Shift p ----*//*---- Reimprimir ultimo cobro ---*/
                if( !_CANT ) // si no hay cobros cargados
                {
                    SETEAR_FUNCION( 54, SI );
                    if( REIMPRIMIR_COBRANZAS() ) {
                        _SET_ESTADO( 32 );
                        rta = 0;
                    }
                }                
                break;
            case 124:
                /*---- m ----*//*---- Entrada Modo operaciones multiples ---*/
                SETEAR_FUNCION( 124, SI );
                ExecuteAction( A_MOSTRAR_OP_MULT );
                SET_MEMORY_INT( operMultiples, SI );
                break; 
            case 131:
                /*---- Shift m ----*//*---- Salida Modo operaciones multiples ---*/
                SETEAR_FUNCION( 131, SI );
                ExecuteAction( A_OCULTAR_OP_MULT );
                SET_MEMORY_INT( operMultiples, NO );
                break;
            case 195:
                /*----- v ------*//*--- Mostrar version de caja ---*/
                SETEAR_FUNCION( 195, SI );
                VERSION_DIALOG();
                break;
            default:
                break;
        }
    }
    if( rta == 0 && estado_tmp == 1 ) {
        //elijo el socio por el browse alfa o numeric
        if( !_CANT || _OP_MULTIPLE ) {
            if( datosCobro->_nroCliente == 0 ) {
                if( tipo_busqueda == _BUSQ_ALFABETI ) {
                    datos_temp.nroCli = PEDIR_CLIENTE_ALFABETICO();
                }
                else if( tipo_busqueda == _BUSQ_CODSOCIO_UNICO ) {
                    datos_temp.nroCli = PEDIR_CLIENTE_CODSOCIO();
                } 
                if( datos_temp.nroCli != 0 ) {
                    datos_temp.id_cli_cobro = cli_cobro.id_cli_cobro;
                    datos_temp.TipoCli = cli_cobro.empresa;
                    datos_temp.Compania2 = cli_cobro.cod_cobro;
                    datosCobro->_nroCliente = datos_temp.nroCli;
                    datosCobro->_id_cliente = datos_temp.id_cli_cobro;
                    datosCobro->_TipoCli = datos_temp.TipoCli; // retorna el tipo de cliente 0-individual, 1-empresa, 2-obra social
                    datosCobro->_codigo = cli_cobro.codigo;
                    /* modalidad de emision
                          0: Resumen individual
                          1: Grupal con resumen individual
                          2: Grupal sin resumen individual
                          3: No factura cuota
                    */
                    if( cli_cobro.modo_emision > 1 ) {
                        mensaje = 1;
                    }
                }
                MOSTRAR_SOCIO( datosCobro, _CANT );
            }
        }
        if( datosCobro->_nroCliente != 0 ) {
            datos_temp.Compania = cli_cobro.cod_cobro;
            datos_temp.prepaga = cli_cobro.codigo;
            //datos_temp.Compania2 = 0;
            datosCobro->_TipoCli = cli_cobro.empresa;
            //posiciono en el codigo de cobro correspondiente
            //SELECT( AREA_COBROS );
			SELECT_TABLE(T_COBROS, TT_ORIG);
            //GO( TOP );
			GO2( TOP );
            //SET_ORDER( 1 );
			SET_ORDER2("ID");
            //GET_EQUAL( ( char* )&cli_cobro.cod_cobro );
			SET_WHERE("ID = %d", &cli_cobro.cod_cobro);
			RUN_QUERY(NO);
            if( /*FOUND()*/ FOUND2() ) {
                sprintf( buffer, "%s", cobros.nombre );
                ExecuteAction( A_MUESTRA_DATO_UNIDAD_NEGOCIO );
                MOSTRAR_INPUT_CADENA( buffer, 0 );
                cod_solicitud = cobros.cod_solicitud;
                estado_tmp = 2;
            }
        }
    }
    if( rta == 1 ) {
        TRIM( datosCobro->CodigoBarra.codigo );
        if( !strlen( datosCobro->CodigoBarra.codigo ) ) {
            //SELECT( AREA_COBROS );
			SELECT_TABLE(T_COBROS, TT_ORIG);
            rta = ELEGIR_UNI_NEG( A_SCROLL_UNIDAD_NEGOCIO, datosCobro );
            if( rta == 1 ) //si se eligio una compa�a
            {
                datos_temp.Compania = cobros.codigo;
                //datos_temp.Compania2 = 0;
                sprintf( buffer, "%s", cobros.nombre );
                ExecuteAction( A_MUESTRA_DATO_UNIDAD_NEGOCIO );
                MOSTRAR_INPUT_CADENA( buffer, 0 );
                cod_aux = datos_temp.Compania;
                if( !_CANT || _OP_MULTIPLE ) {
                    //COBROS SIN COMPROBANTE
                    //No se pide el cod_cobro ni el tipo cliente, se obtienen de las tablas.
                    if( cobros.cobro_a_cuenta == 1 ) {
                        long aux;
                        char CliAux[20];
                        if( datosCobro->_nroCliente == 0 ) {
                            aux = PEDIR_DATOS_COBROS_ESP();
                            if( ( aux != 0 )
                             && ( CLIENTE_AUTORIZADO( /*aux*/cli_cobro.id_cli_cobro,
                                                      cli_cobro.cod_cobro,
                                                      ( short int ) cli_cobro.empresa ) ) ) {
                                if( cli_cobro.cod_cobro == 5 && datos_temp.Compania == 4 ) {
                                    datos_temp.nroCli = 0;
                                    MENSAJE_STRING( S_NO_SE_PUEDE_COBRAR_SIN_CLIENTE );
                                    rta = 0;
                                }
                                else {
                                    datosCobro->_nroCliente = aux;
                                    datosCobro->_dato = cli_cobro.cod_cobro;
                                    datosCobro->_TipoCli = cli_cobro.empresa;
                                    SET_MEMORY_LONG( __ram_nro_cliente, cli_cobro.id_cli_cobro );
                                    SET_MEMORY_LONG( __ram_cliente_afinidad, cli_cobro.nro_socio );
                                    CEREAR_MEMORY( __ram_string_cliente_afinidad );
                                    SET_MEMORY_CHAR( __ram_pedir_cliente_afinidad, 0 );
                                    SET_MEMORY_INT( __ram_perfil, cli_cobro.cod_cobro );
                                    datos_temp.nroCli = aux;
                                    datos_temp.id_cli_cobro = cli_cobro.id_cli_cobro;
                                    datos_temp.prepaga = cli_cobro.codigo;
                                    datosCobro->_id_cliente = cli_cobro.id_cli_cobro;               
                                    datosCobro->_codigo = datos_temp.prepaga;
                                    //Se muestra el codigo del cliente
                                    sprintf( CliAux, "%ld", aux );
                                    ExecuteAction( A_MUESTRA_DATO_CLIENTE );
                                    MOSTRAR_INPUT_CADENA( CliAux, 0 );
                                    rta = 1;
                                }
                            }
                            else {
                                //Cliente no existe o no autorizado
                                rta = 0;
                            }
                        }
                    }
                    else {
                        datos_temp.Compania2 = 0;
                        /*   if( cobros.cobro_a_cuenta == 1 ) {
                              //tengo que identificar la unidad de negocio, compa�a 
                              //y tipo de cliente para poder buscar el cliente
                              //datos_temp.Compania = 
                              cod_aux = MENU_SELECCIONA_COD_COBRO( );
                              datos_temp.Compania2 = cod_aux;   
                              SET_MEMORY_LONG( datosCobro->_dato, datos_temp.Compania2 );
                           }*/
                        if( cobros.cobro_a_cuenta == 0 || cobros.cobro_a_cuenta == 1 ) {
                            datosCobro->_TipoCli = OBTENER_TIPO_CLI(); // retorna el tipo de cliente 0-individual, 1-empresa, 2-obra social
                            if( datosCobro->_TipoCli < 0 ) {
                                estado_tmp = 0;
                                rta = 0;
                            }
                        }
                    }
                }
                if( rta ) {
                    if( _CANT > 0 && cobros.cobro_a_cuenta != 1 && !_OP_MULTIPLE ) {
                        RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP, ( char* )datosCobro,
                                               sizeof( struct DatosCobro ), _CANT - 1 );
                    }
                    datos_temp.nroCli = _NRO_CLIENTE;      
                    datos_temp.id_cli_cobro = _ID_CLIENTE;
                    datos_temp.TipoCli = _TIPO_CLI;
                    datos_temp.prepaga = _CODIGO;
                    if( cobros.cobro_a_cuenta == 1 ) {
                        datos_temp.Compania2 = _DATO;
                    }
                    else {
                        datos_temp.Compania2 = cod_aux;
                        _NRO_CLIENTE = 0;
                    }
                    //OTROS INGRESOS
                    if( cobros.cobro_a_cuenta > 1 ) {
                        //no se graba compania para OTROS INGRESOS
                        //no se pide socio para ra OTROS INGRESOS
                        cod_aux = 0;
                        datos_temp.nroCli = -97;
                        datos_temp.id_cli_cobro = 3;
                        datos_temp.prepaga = -1;
                        datosCobro->_TipoCli = 3;//el 3 no existe
                        datos_temp.TipoCli = 3;
                        datosCobro->_Compania = cobros.codigo;
                        if( !EXISTE_SOCIO( _BUSQ_CODSOCIO, datos_temp.nroCli, COD_UNIDAD_NEGOCIO,
                                           cobros.codigo, ( char )_TIPO_CLI, NULL ) ) {
                            datosCobro->_nroCliente = 0;
                            GRABAR_LOG_SISTEMA_STRING( S_CLIENTE_SIN_CODIGO_NO_ENCONTRADO , LOG_ERRORES,2);
                            MENSAJE_STRING( S_CLIENTE_SIN_CODIGO_NO_ENCONTRADO );
                            return( 0 );
                        }
                        else {
                            datosCobro->_nroCliente = datos_temp.nroCli;
                            datosCobro->_id_cliente = cli_cobro.id_cli_cobro;               
                            datosCobro->_codigo = datos_temp.prepaga;
                            datosCobro->_dato = datos_temp.Compania2;
                        }
                    }         
                    if( ( !_CANT || _OP_MULTIPLE ) && _NRO_CLIENTE == 0 ) {
                        datos_temp.nroCli = PEDIR_CLIENTE( ( short )cod_aux, ( char )_TIPO_CLI );
                        //verifica que no sea un socio para otros cobros cuando esta en cobro sin comprobante
                        if( cli_cobro.cod_cobro == 5 && datos_temp.Compania == 4 ) {
                            datos_temp.nroCli = 0;
                            MENSAJE_STRING( S_NO_SE_PUEDE_COBRAR_SIN_CLIENTE );
                        }
                        else {
                            if( datos_temp.nroCli != 0 ) {
                                datos_temp.id_cli_cobro = cli_cobro.id_cli_cobro;
                                datos_temp.prepaga = cli_cobro.codigo;
                                datosCobro->_nroCliente = datos_temp.nroCli;
                                datosCobro->_id_cliente = datos_temp.id_cli_cobro;               
                                datosCobro->_codigo = datos_temp.prepaga;
                                datosCobro->_dato = datos_temp.Compania2;
                                /* modalidad de emision
                                0: Resumen individual
                                1: Grupal con resumen individual
                                2: Grupal sin resumen individual
                                3: No factura cuota
                                */
                                if( cli_cobro.modo_emision > 1 ) {
                                    mensaje = 1;
                                }
                            }
                        }
                    }
                    rta = 0;
                    estado_tmp = 2;
                    cod_solicitud = cobros.cod_solicitud;
                }
            }
        }
        else {
            estado_tmp = 3;
        }
    }
    if( estado_tmp == 2 ) {
        rta = 0;
        if( mensaje ) {
            MENSAJE_CON_TECLA_STRING( S_SOCIO_PERTENECE_A_EMPRESA_PRES_ENTER, 13 );
            mensaje = 0;
        }
        if( datos_temp.nroCli != 0 )       //si hay un socio cargado
        {
            if( cobros.cobro_a_cuenta ) {
                //cobro a cuenta de deuda
                char aux[30];
                int aux2;
                SET_MEMORY_INT( __ram_motivo_descuento, 0 );
                //pido motivo
                if( NRO_CLIENTE_POR_DEFECTO && ( datos_temp.nroCli == -NRO_CLIENTE_POR_DEFECTO ) ) {
                    SET_MEMORY_INT( __ram_motivo_descuento, NRO_CLIENTE_POR_DEFECTO );
                    VERIFICAR_MOTIVO( _MOTIVO_OBTENER_CODSOLYSUPER, ( int* )&cod_solicitud,
                                      ( short* )&pedir_supervisor, ( int )RAM_MOTIVO_DESCUENTO );
                    sprintf( aux, "%s", "Futuros Clientes" );
                    aux2 = _F_SUP_FUTUROS_SOCIOS;
                }
                else {
                    if( datos_temp.nroCli < 0 && cobros.cobro_a_cuenta == 1 ) {
                        SET_MEMORY_INT( __ram_motivo_descuento, ( -1 )*datos_temp.nroCli );
                        VERIFICAR_MOTIVO( _MOTIVO_OBTENER_CODSOLYSUPER, ( int* )&cod_solicitud,
                                          ( short* )&pedir_supervisor,
                                          ( int )RAM_MOTIVO_DESCUENTO );
                        sprintf( aux, "%s", "Cliente dado de baja" );
                        aux2 = _F_SUP_CLIENTE_INHAB;
                    }
                    else {
                        PEDIR_MOTIVO_OPERACION_NEGATIVA( ( int* )&cod_solicitud, _MOTIVO_DESCUENTO );
                        VERIFICAR_MOTIVO( _MOTIVO_OBTENER_SUPERVISOR, ( int* )&cod_solicitud,
                                          ( short* )&pedir_supervisor,
                                          ( int )RAM_MOTIVO_DESCUENTO );
                        if( cobros.cobro_a_cuenta == 2 ) {
                            sprintf( aux, "%s", "Otros Ingresos" );
                        }
                        else {
                            sprintf( aux, "%s", "Cobro a Cuenta de Deuda" );
                        }
                        aux2 = _F_SUP_CARGA_MANUAL;
                    }
                }
                if( pedir_supervisor ) {
                    rta = SOLICITAR_CLAVES( 60, 19, NO, SI, SI, aux, aux, aux2, NO );
                    PRN_SELECT( SLIP );
                }
                else {
                    rta = 1;
                }
                if( !rta ) {
                    return( 0 );
                }
                //_SET_MEMORY_INT( __ram_motivo_descuento_pendiente, _CANT,RAM_MOTIVO_DESCUENTO );
                datosCobro->_motivo = RAM_MOTIVO_DESCUENTO;
            }
            if( cod_solicitud > 0 ) //si tiene datos adicionales
            {
                SET_MEMORY_INT( __item_cod_solicitud, cod_solicitud );
                datos_temp.Comprobante = 0;
                rta = PAGOS_PEDIR_DATOS_ADICIONALES( 0, ( char )cantidad, cod_solicitud,
                                                     ( double* )&datos_temp.Importe,
                                                     D_COBRO_SERVICIOS, "         DATOS           ",
                                                     1, &datos_temp.Comprobante, NULL , '\0', '\0');
                if( rta )  //si se cargaron los datos adicionales
                {
                    _r = reintentar;
                    while( rta && _r ) {
                        exigir_ingreso_de_valor = 1;
                        datos_temp.Importe = PEDIR_FLOAT_EN_RECUADRO( " IMPORTE", NO, ENTEROS,
                                                                      DECIMALES ); 
                        exigir_ingreso_de_valor = 0;
                        if( datos_temp.Importe < 0.0 ) {
                            MENSAJE( "DATO INVALIDO - INTENTE NUEVAMENTE" );
                            _r--;
                        }
                        else {
                            _r = 0;
                        }
                    }
                    if( datos_temp.Importe < 0.0 ) {
                        rta = 0;
                    }
                }
                else {
                    // si no se cargaron los datos adicionales
                    datosCobro->_nroCliente = 0;
                }
                if( rta )  //si se ingres� correctamente el importe
                {
                    rta = SET_VALORES( datosCobro, &datos_temp, SI );
                }
            }
            else //si no tiene datos adiconales
            {
                if( !cobros.cobro_a_cuenta ) {
                    rta = MOSTRAR_DEUDA( datos_temp.nroCli, datos_temp.Compania, datosCobro );
                    if( rta > 0 ) // si se selecciono una deuda 
                    {
                        if( CALCULAR_IMPORTE_DEUDA( &datos_temp.Importe, &datos_temp.ImporteMin,
                                                    SI ) ) {
                            rta = SET_VALORES( datosCobro, &datos_temp, NO );
                        }
                        else {
                            rta = 0;
                        }
                    }
                    ExecuteAction( A_DESACTIVAR_PANEL_DEUDA_SOCIO );
                    if( rta == -1 ) {
                        MENSAJE_STRING( S_NO_SE_REGISTRAN_DEUDAS );
                        rta = 0;
                    }
                }
            }
        }
    }
    if( rta > 0 && estado_tmp == 3 ) {
        rta = COBRO_POR_COD_BARRA( datosCobro, &datos_temp.Comprobante, &datos_temp.Compania,
                                   &datos_temp.Importe, &datos_temp.Id_deuda );
    }
    if( rta == 1 ) {
        datosCobro->_Comprobante = datos_temp.Comprobante;
        SET_MEMORY_LONG( __item_comprobante, datos_temp.Comprobante );
        datosCobro->_Compania = datos_temp.Compania;
        datosCobro->_Importe = datos_temp.Importe;
        datosCobro->_IdDeuda = datos_temp.Id_deuda;
        idCobrados[_CANT] = datos_temp.Id_deuda;
        GRABAR_COD_BARRA_EN_BINARIO_ESP( datosCobro->CodigoBarra.codigo, cantidad );
    }
    else {
        if( cantidad > 0 ) {
            LIMPIAR_PANEL( NO, NO );
        }
        else {
            LIMPIAR_PANEL( NO, SI );
        }
    }
    BORRAR_MENSAJE();
    return( rta );
}
/***********************************************************************/
int OBTENER_TIPO_CLI( void )
/***********************************************************************/
{
    int y,rta = 0,columnas, h;
    struct  _cabmenu *cab;
    struct _menu items[3];
    columnas = 2;
    y = 0;
    if( ( items[y].nom = ( char* )malloc( sizeof( string_table->mensaje ) ) ) != NULL ) {
        strcpy( items[y].nom, "0 - Individual" );
        items[y].tecla = '0';
        items[y].rta = 0;
        y++;
    }
    if( ( items[y].nom = ( char* )malloc( sizeof( string_table->mensaje ) ) ) != NULL ) {
        strcpy( items[y].nom, "1 - Empresa" );
        items[y].tecla = '1';
        items[y].rta = 1;
        y++;
    }
    if( ( items[y].nom = ( char* )malloc( sizeof( string_table->mensaje ) ) ) != NULL ) {
        strcpy( items[y].nom, "2 - Obra Social" );
        items[y].tecla = '2';
        items[y].rta = 2;
        y++;
    }
    cab = ( struct _cabmenu * )calloc( columnas, sizeof( struct _cabmenu ) );
    if( cab != NULL ) {
        cab[0].nombre = "DESCRIPCION";
        cab[0].align = ALIGN_LEFT;
        cab[0].width = 20;
        rta = MENU( "SELECCIONE TIPO DE CLIENTE", items, y, cab, columnas );
        free( cab );
    }
    //libero memoria asignada
    for( h = 0;h < y;h++ ) {
        FREEMEM( items[h].nom );
    }
    return rta;
}
/*****************************************************************************/
long PEDIR_DATOS_COBROS_ESP( void )
/*****************************************************************************/
{
    long nroCli = 0;   
    int seguir = 1, k, rta = 0;
    int aux; 
    char cuit[21];
    char whereTmp[50];
    MENSAJE_STRING( S_SELECCIONE_EL_CLIENTE_ALT_L_A );
    aux = TECLADO_ALFABETICO;
    TECLADO_ALFABETICO = 0;
    while( seguir ) {
        k = GETCH();
        switch( k ) {
            case 27:
                rta = PEDIR_CADENA_EN_RECUADRO( " CUIT CLIENTE ", NO, 13, cuit, NULL );
                SELECT_TABLE( T_CLI_COBRO, TT_ORIG );
                //SELECT( AREA_SOCIOS );
                //indice = GET_ORDER();            
                //SET_ORDER( 3 );
                TRIM( cuit );
                //GET_EQUAL( ( char* )&cuit );
                sprintf( whereTmp, "CUIT = '%s%%'", cuit );
				SET_WHERE("%s",whereTmp);
                RUN_QUERY( NO );
                if( FOUND2() ) {
                    rta = 1;
                }
                else {
                    rta = 0;
                }
                seguir = 0;
                //SET_ORDER( indice );
                break;
            case 179:
            case 108:
                /*---- ALT L ----*//*---- Consulta Socio Alfabetico ---*/
                rta = EXISTE_SOCIO( _BUSQ_ALFABETI, 0, COD_UNIDAD_NEGOCIO, 0, 0, NULL );
                if( !rta ) {
                    MENSAJE_STRING( S_SELECCIONE_EL_CLIENTE_ALT_L_A );
                }
                seguir = !rta;
                break;
            case 171:
            case 109:
                /*---- ALT K ----*//*---- Consulta Socio Numerico ---*/
                rta = EXISTE_SOCIO( _BUSQ_CODSOCIO_UNICO, 0, COD_UNIDAD_NEGOCIO, 0, 0, NULL );
                if( !rta ) {
                    MENSAJE_STRING( S_SELECCIONE_EL_CLIENTE_ALT_L_A );
                }
                seguir = !rta;
                break;
            default:
                seguir = 0;
                rta = 0;
                break;
        }
    }
    TECLADO_ALFABETICO = aux;
    if( !rta ) {
        MENSAJE_STRING( S_CLIENTE_NO_ENCONTRADO );
        BORRAR_MENSAJE();
        return( 0 );
    }
    else {
        nroCli = cli_cobro.nro_socio;
    }
    //SET_MEMORY_LONG( _cod_cliente, nroCli );    
    //OBTIENE_IDENTIFICACION_UNICA( );
    return( nroCli );
}
/***********************************************************************/
int SET_VALORES( struct DatosCobro *datosCobro, struct varios_esp *datos_temp, int a_cuenta )
/***********************************************************************/
{
    int rta;
    //if(datos_temp->Importe > 0)
    //{
    rta = 1;
    datosCobro->_nroCliente = datos_temp->nroCli;      
    datosCobro->_id_cliente = datos_temp->id_cli_cobro;
    datosCobro->_TipoCli = datos_temp->TipoCli;
    datosCobro->_codigo = datos_temp->prepaga;
    datosCobro->_dato = datos_temp->Compania2;
    SET_MEMORY_LONG( __ram_nro_cliente, datosCobro->_id_cliente );
    MOSTRAR_SOCIO( datosCobro, _CANT );  ////mostrarCliCobros(clientes.nombre,datos_temp->TipoCli,clientes.domicilio);
    //ExecuteSAction( A_MUESTRA_DATO_SOCIO_COBRO,clientes.nombre );
    if( a_cuenta == SI ) {
        datos_temp->Id_deuda = 0;
        sprintf( datos_temp->nroCompTemp, "%ld", datos_temp->Comprobante );
        //(datos_temp->nroCli == -99) ? datos_temp->Comprobante : 0 );
        //datos_temp->Comprobante = 0;
    }
    else {
        datos_temp->Id_deuda = oper_cli.id_deuda;
        sprintf( datos_temp->nroCompTemp, "%ld", oper_cli.nro_comprobante );
        datos_temp->Comprobante = oper_cli.nro_comprobante;
    }
    FTOC( datos_temp->Importe, datos_temp->importeTemp, ENTEROS + 1, DECIMALES, SI, SI );
    SubTotal += datos_temp->Importe;
    //agregarCobroEsp(cobros.nombre,datos_temp->nroCompTemp,datos_temp->importeTemp);
    datosCobro->_tipo_mov = AGREGAR_DETALLES_COBRO( datos_temp->Id_deuda, cobros.nombre,
                                                    datos_temp->nroCompTemp,
                                                    datos_temp->importeTemp );
    datosCobro->_letra_comp = oper_cli.letra_comp;
    datosCobro->_punto_venta = oper_cli.punto_venta;
    FTOC( SubTotal, datos_temp->importeTemp, ENTEROS + 1, DECIMALES, SI, 0 );
    ExecuteSAction( A_MUESTRA_SUBTOTAL_COBRO_ESP, datos_temp->importeTemp );
    /*}
    else
    {
       MENSAJE( "NO SE PERMITEN IMPORTES EN CERO" );
       BORRAR_MENSAJE(  );
       rta = 0;
    }*/
    return rta;
}
/***********************************************************************/
int COBRO_POR_COD_BARRA( struct DatosCobro *datosCobro, long *Comprobante, int *Compania,
                         double *Importe, long *IdDeuda )
/***********************************************************************/
{
    struct   _DatosCodBarra DatosCodBarra;
    int rta = 0,i,limite = 0;
    char UniNegTemp[10];
    char importeTemp[20];
    //   long     cliente;
    int vencido = 0,fechaActual;
    for( i = 0;i < 3;i++ ) {
        UniNegTemp[i] = datosCobro->CodigoBarra.codigo[i];
    }
    UniNegTemp[3] = 0;
    *Compania = atoi( UniNegTemp );
    //SELECT( AREA_COBROS );
	SELECT_TABLE(T_COBROS, TT_ORIG);
    //SET_ORDER( 2 );
	SET_ORDER2("ID");
    //GET_EQUAL( ( char* )Compania );
	SET_WHERE("NOMBRE = '%s'",Compania);
	RUN_QUERY(NO);
    if( /*FOUND()*/ FOUND2() ) {
        *Compania = cobros.codigo;
        rta = ANALIZAR_CODIGO_BARRA_PARA_SERVICIOS_ESP( *Compania, datosCobro->CodigoBarra.codigo,
                                                        &DatosCodBarra );
        if( rta == 2 ) // si el comprobante esta vencido
        {
            vencido = 1;
            rta = 0;
        }
        else if( rta != 0 ) {
            MENSAJE_STRING( S_CODIGO_INVALIDO );
        }
        if( rta == 0 ) {
            rta = OBTENER_CLI_COMPROBANTE( &DatosCodBarra, datosCobro );
            //si se encontro la deuda y ya fue cargada o pagada
            if( /*rta==1 &&*/ID_COBRADO( oper_cli.id_deuda, oper_cli.pagado ) ) {
                rta = -2;
            }
            if( rta == 1 ) {
                fechaActual = ( int )_GET_FECHA_DOS();
                //Se chequea fecha de vencimiento solo a los 
                //individuales con fecha limite de pago excedida.
                //Si es empresa, se cobra igual.
                //Si es individual vencido solo se muestra el mensaje y no le deja cobrar
                if( oper_cli.fecha_limite_pago < fechaActual && oper_cli.fecha_limite_pago != 0 ) {
                    if( cli_cobro.empresa == 0 ) {
                        // fecha limite excedida
                        MENSAJE_STRING( S_LIMITE_DE_COBRO_EXCEDIDO );
                        limite = 1;
                        rta = 0;
                    }
                }
                if( !limite ) {
                    //Si tiene que controlar deuda mas antigua, pide supervisor
                    if( oper_cli.cobro_antig != 0 ) {
                        if( !DEUDA_MAS_ANTIGUA( oper_cli.nro_socio, oper_cli.cod_cobro,
                                                oper_cli.tipo_cli, oper_cli.id_deuda ) ) {
                            rta = SOLICITAR_CLAVES( 30 - ancho_panel, 21, NO, SI,
                                                    SI/*PEDIR_CLAVE_EN_COBROS*/,
                                                    "DEUDA MAS ANTIGUA", NULL,
                                                    _F_SUP_DEUDA_MAS_ANTIGUA, NO );
                        }
                    }
                    if( vencido && rta ) {
                        MENSAJE_STRING( S_COMPROBANTE_VENCIDO );
                        rta = SOLICITAR_CLAVES( 60, 19, NO, SI, SI, "AUTORIZAR VENCIMIENTO",
                                                "AUTORIZAR VENCIMIENTO",
                                                _F_SUP_AUTORIZA_VENCIMIENTO, NO );
                    }
                } //limite
                if( rta ) {
                    rta = LLENAR_PANEL( &DatosCodBarra, Comprobante, Compania, Importe, 0 );
                    *IdDeuda = oper_cli.id_deuda;
                    datosCobro->_tipo_mov = oper_cli.tipo_mov;
                }
                else {
                    rta = 0;
                }
            }
            else {
                if( rta == 0 ) {
                    MENSAJE_STRING( S_DEUDA_NO_ENCONTRADA );
                    rta = 0;
                }
                else {
                    if( rta == -1 ) {
                        MENSAJE_STRING( S_NO_ES_EL_CLIENTE_ACTUAL );
                        rta = 0;
                    }
                    else {
                        if( rta == -2 ) {
                            MENSAJE( "EL COBRO YA FUE CARGADO O COBRADO" );
                            rta = 0;
                        }
                        else {
                            MENSAJE_STRING( S_CODIGO_INVALIDO );
                            rta = 0;
                        }
                    }
                }
            }
        }
        else {
            rta = 0;
        }
        if( rta == 1 ) {
            SubTotal += *Importe;
            FTOC( SubTotal, importeTemp, ENTEROS, DECIMALES, 1, 0 );
            ExecuteSAction( A_MUESTRA_SUBTOTAL_COBRO_ESP, importeTemp );
        }
    }
    else {
        MENSAJE_STRING( S_CODIGO_DE_COBRO_NO_ENCONTRADO );
        rta = 0;
    }
    BORRAR_MENSAJE();
    return rta;
}
/***********************************************************************/
int OBTENER_CLI_COMPROBANTE( struct _DatosCodBarra *DatosCodBarra, struct DatosCobro *datosCobro )
/***********************************************************************/
{
    char strTemp[50];
    int rta;
    long nroCliTemp;
    struct _ind_deuda ind_deuda;
    if( DatosCodBarra->cod_cobro <= 0 || DatosCodBarra->importe <= 0
     || DatosCodBarra->nro_comprobante <= 0 || DatosCodBarra->letra_comp < 0
     || DatosCodBarra->punto_venta <= 0 ) {
        return -2; //codigo invalido
    }
    if( CARGAR_LETRA_TIPO( COD_UNIDAD_NEGOCIO, DatosCodBarra->cod_cobro, DatosCodBarra->letra_comp )
     == 1 ) {
        ind_deuda.cod_cobro = (short)DatosCodBarra->cod_cobro;
        ind_deuda.grupo_cobro = COD_UNIDAD_NEGOCIO;
        ind_deuda.letra_comp = tipo_mov.letra_comp;
        ind_deuda.nro_comp = DatosCodBarra->nro_comprobante;
        ind_deuda.punto_venta = ( short )DatosCodBarra->punto_venta;
        ind_deuda.tipo_mov = tipo_mov.tipo_mov;
      //  if( USED( AREA_OPER_CLI )
      //   || USE_DB( AREA_OPER_CLI, _OP_CLI_SIC, ( char* )&oper_cli, sizeof( oper_cli ), NULL, 0, 0 )
      //   == 0 ) {
		if( OPENED( T_OPE_CLIENTE, TT_ORIG ) ||
			OPEN_TABLE( T_OPE_CLIENTE, TT_ORIG, ( char* )&oper_cli, sizeof( oper_cli ) ) == 0 ) {
				
			SELECT_TABLE( T_OPE_CLIENTE, TT_ORIG );
			SET_ORDER2( "GRUPO_COBRO, COD_COBRO, LETRA_COMP, PUNTO_VENTA, NRO_COMPROBANTE, TIPO_MOV" );
 
			SET_WHERE("GRUPO_COBRO = '%d' and COD_COBRO = '%d' and LETRA_COMP = '%d' and PUNTO_VENTA = '%d' and NRO_COMPROBANTE = '%Ld' and TIPO_MOV  = '%d'",
				ind_deuda.grupo_cobro, ind_deuda.cod_cobro, ind_deuda.letra_comp, ind_deuda.punto_venta,
				ind_deuda.nro_comp,	ind_deuda.tipo_mov  );
			RUN_QUERY(NO);

            //SELECT( AREA_OPER_CLI );
            //SET_ORDER( 1 );
            //GET_EQUAL( ( char* )&ind_deuda );
        }
        if( FOUND2()
         && EXISTE_SOCIO( _BUSQ_CODSOCIO, oper_cli.nro_socio, COD_UNIDAD_NEGOCIO,
                          oper_cli.cod_cobro, ( char )oper_cli.tipo_cli, NULL )
         && CLIENTE_AUTORIZADO( cli_cobro.id_cli_cobro, ind_deuda.cod_cobro, oper_cli.tipo_cli ) ) {
            nroCliTemp = ( !_CANT ) ? 0 : RAM_NRO_CLIENTE;
            if( nroCliTemp == 0 || nroCliTemp == oper_cli.nro_socio || _OP_MULTIPLE ) {
                sprintf( strTemp, "%ld", oper_cli.nro_socio );            
                ExecuteAction( A_MUESTRA_DATO_CLIENTE );
                MOSTRAR_INPUT_CADENA( strTemp, 0 );
                datosCobro->_TipoCli = oper_cli.tipo_cli;
                //ExecuteSAction( A_MUESTRA_DATO_SOCIO_COBRO,clientes.nombre );
                SET_MEMORY_LONG( __ram_nro_cliente, cli_cobro.id_cli_cobro );
                MOSTRAR_SOCIO( datosCobro, _CANT );
                datosCobro->_nroCliente = oper_cli.nro_socio;
                datosCobro->_id_cliente = cli_cobro.id_cli_cobro;
                datosCobro->_dato = oper_cli.cod_cobro;
                datosCobro->_letra_comp = oper_cli.letra_comp;
                datosCobro->_punto_venta = oper_cli.punto_venta;
                datosCobro->_codigo = cli_cobro.codigo;
                rta = 1;
            }
            else {
                rta = -1;
            }
        }
        else {
            rta = 0;
        }
    }
    else {
        MENSAJE_STRING( S_TIPO_DE_MOVIMIENTO_Y_LETRA_DE_COMPROB );
        BORRAR_MENSAJE();
        rta = 0;
    }
   // CLOSE_DATABASE( AREA_OPER_CLI );
	CLOSE_TABLE( T_OPE_CLIENTE, TT_ORIG );
    return rta;
}
/***********************************************************************/
int LLENAR_PANEL( struct _DatosCodBarra *datos, long *Comprobante, int *Compania, double *Importe,
                  long cliente )
/***********************************************************************/
{
    //   char      strTemp[10];
    char nroCompTemp[10];
    char importeTemp[20];
    //SELECT( AREA_COBROS );
	SELECT_TABLE(T_COBROS, TT_ORIG);
    //GO( TOP );
	GO2( TOP );
    //SET_ORDER( 1 );
	SET_ORDER2("CODIGO");
    //GET_EQUAL( ( char* )Compania );
	SET_WHERE("NOMBRE = '%s'", Compania);
	RUN_QUERY(NO);
    *Comprobante = datos->nro_comprobante;
    sprintf( nroCompTemp, "%ld", *Comprobante );
    *Importe = datos->importe;
    FTOC( datos->importe, importeTemp, ENTEROS, DECIMALES, SI, SI );
    //agregarCobroEsp(cobros.nombre,nroCompTemp,importeTemp);
    AGREGAR_DETALLES_COBRO( oper_cli.id_deuda, cobros.nombre, nroCompTemp, importeTemp );
    return 1;
}
/***********************************************************************/
int MOSTRAR_DEUDA( long nroSocio, int Compania, struct DatosCobro *datosCobro )
/***********************************************************************/
{
    char fechaTemp[15];
    char nroCompTemp[15];
    char importeTemp[20];
    char importeMinTemp[20];
    char tipoMov[10];
    double Importe,ImporteMin;
    int year, month, day, rta = 0,cnt = 0,activado = 0;
    struct _ind_deuda4 ind_deuda3;
  //  if( !USED( AREA_OPER_CLI ) ) {
  //      USE_DB( AREA_OPER_CLI, _OP_CLI_SIC, ( char* )&oper_cli, sizeof( oper_cli ), NULL, 0, 0 );
  //  }
	if( !OPENED( T_OPE_CLIENTE, TT_ORIG ) ) {
		OPEN_TABLE( T_OPE_CLIENTE, TT_ORIG, ( char* )&oper_cli, sizeof( oper_cli ) ); 
	}
    ind_deuda3.cod_cobro = (short)Compania;
    ind_deuda3.nro_socio = nroSocio;   
    ind_deuda3.tipo_cli = _TIPO_CLI;
    ind_deuda3.fecha = 0;
	
	SELECT_TABLE( T_OPE_CLIENTE, TT_ORIG );
	SET_ORDER2( "COD_COBRO, NRO_SOCIO, TIPO_CLI, FECHA_COMP" );

	SET_WHERE("COD_COBRO >= '%d' and NRO_SOCIO >= '%ld' and TIPO_CLI >= '%d' and FECHA_COMP >= '%d'",
		ind_deuda3.cod_cobro, ind_deuda3.nro_socio, ind_deuda3.tipo_cli, ind_deuda3.fecha );
		RUN_QUERY(NO);

  //  SELECT( AREA_OPER_CLI );
  //  SET_ORDER( 4 );
   // GET_GREATER_E( ( char* )&ind_deuda3 );
    MENSAJE_SIN_SONIDO_STRING( S_CARGANDO_DEUDAS );
    while( !dbEOF() && !dbBOF() && oper_cli.cod_cobro == Compania && oper_cli.nro_socio == nroSocio
        && oper_cli.tipo_cli == ind_deuda3.tipo_cli )     // llena la grilla del panel deuda
    {
        if( !ID_COBRADO( oper_cli.id_deuda, oper_cli.pagado ) ) {
            char fecha[9],fechaVenc[10];
            CALCULAR_IMPORTE_DEUDA( &Importe, &ImporteMin, NO );
            FECHA__( oper_cli.fecha_comp, &day, &month, &year );
            sprintf( fechaTemp, "   %02d/%02d/%d", day, month, year );
            sprintf( nroCompTemp, "%ld", oper_cli.nro_comprobante );
            FTOC_2( Importe, importeTemp, ENTEROS, DECIMALES, 1, 1 );
            FTOC_2( ImporteMin, importeMinTemp, ENTEROS, DECIMALES, 1, 1 ); 
            DTOC( oper_cli.periodo_fac, fecha );     
            MOVTOC( oper_cli.tipo_mov, tipoMov );
            sprintf( fechaVenc, "%s", strcmp( fecha, "  /  /  " ) ? fecha : " " );
            //sprintf(tipoMov,      "%d",  (int)oper_cli.tipo_mov);
            agregarDeuda( fechaTemp, tipoMov, nroCompTemp, fechaVenc, importeTemp, importeMinTemp );
            cnt++;
        }
        SKIP2( 1 );
    }
    if( cnt ) {
        ExecuteAction( A_ACTIVAR_PANEL_DEUDA_SOCIO );
        ExecuteSAction( A_MUESTRA_DATO_SOCIO_DEUDA, cli_cobro.apellynom );
    }
    BORRAR_MENSAJE();
    if( cnt > 0 ) {
        rta = ELEGIR_DEUDA( A_SCROLL_DEUDA_SOCIO, cnt, &ind_deuda3, datosCobro );
    }
    else {
        rta = -1;
    }
    //CLOSE_DATABASE( AREA_OPER_CLI );
	CLOSE_TABLE( T_OPE_CLIENTE, TT_ORIG );
    return rta;
}
/** Traduce un codigo de tipo de movimiento a un string
  */
/***************************************************************************/
void MOVTOC( int codigo, char tipoMov[6] )
/***************************************************************************/
{
    struct _tipo_movimiento_caja tip_mov_caja;
	int  area_ant, tipo_ant;
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
  //  int selected;
  //  selected = SELECTED();
   // if( USE_DB( AREA_TIPO_MOV_CAJA, _tipo_movimiento_caja, ( char* )&tip_mov_caja, sizeof( tip_mov_caja ),
   //             NULL, 0, 0 ) == 0 ) {
   //    GET_EQUAL( ( char* )&codigo );
	if( OPEN_TABLE( T_TIP_MOV_CAJA, TT_ORIG, ( char* )&tip_mov_caja, sizeof( struct _tipo_movimiento_caja ) ) == 0 ) {
		SELECT_TABLE( T_TIP_MOV_CAJA, TT_ORIG );
		SET_WHERE("TIPO_MOV = '%d'", codigo );
		RUN_QUERY(NO);
        if( FOUND2() ) {
            strcpy( tipoMov, tip_mov_caja.abrev );
        }
        else {
            sprintf( tipoMov, "%d", codigo );
        }
     //   CLOSE_DATABASE( AREA_TIPO_MOV_CAJA );
		 CLOSE_TABLE( T_TIP_MOV_CAJA, TT_ORIG );
    }
    //SELECT( selected );
	SELECT_TABLE( area_ant, tipo_ant );
}
/*
   Funcion que verifica si un id deuda ya esta cargado para ser cobrado
   retorna 1 en caso de ya estar cargado y 0 en caso de no estarlo
   Modificacion: Tambien verifica el campo "pagado" que indica si ya fue
   cobrado en otra transaccion en esta u otra caja.
*/
/***********************************************************************/
int ID_COBRADO( long idDeuda, char pagado )
/***********************************************************************/
{
    int rta = 0, cnt = 0;
    if( 'S' == pagado ) {
        rta = 1;
    }
    else {
        while( rta == 0 && cnt < _CANT ) {
            if( idDeuda == idCobrados[cnt] ) {
                rta = 1;
                break;
            }
            cnt++;
        }
    }
    return rta;
}
/***********************************************************************/
int CALCULAR_IMPORTE_DEUDA( double *importe, double *importeMin, int pregunta )
/***********************************************************************/
{
    int fechaActual, rta = 1, salir = 0, minimo = 0, credito = 0;
    char msg[100];
    double importeTemp;
    char impMinChar[20];
    char impMaxChar[20];
    //Si es un comprobante de credito se toma el importe original (NEGATIVO)
    //Comprobantes de credito: Nota de Credito (Tipo de Mov 2)
    //                         Cargo Credito   (Tipo de Mov 6)
    credito = oper_cli.tipo_mov == TMOV_NOTA_CREDITO || oper_cli.tipo_mov == TMOV_CARGO_CREDITO;
    if( credito ) {
        *importe = ( -1 )*oper_cli.importe_venc1;
        *importeMin = *importe;
    }
    else {
        //no credito
        fechaActual = ( int )_GET_FECHA_DOS();
        if( oper_cli.vencimiento1 >= fechaActual ) {
            *importe = oper_cli.importe_venc1;
            *importeMin = oper_cli.importe_min1;
        }
        else if( oper_cli.vencimiento2 >= fechaActual ) {
            *importe = oper_cli.importe_venc2;
            *importeMin = oper_cli.importe_min2;
        }
        else if( oper_cli.vencimiento3 >= fechaActual ) {
            *importe = oper_cli.importe_venc3;
            *importeMin = oper_cli.importe_min3;
        }
        else // en caso que este vencido. toma el importe de 3er vencimiento y retorna 0
        {
            *importe = oper_cli.importe_venc3;
            *importeMin = oper_cli.importe_min3;      
            //rta = 0;
        }
    }
    if( pregunta ) {
        if( credito ) {
            //si es Credito, no debe propasar el monto del pago acumulado del cliente
            if( oper_cli.importe_venc1 > IMPORTE_CLIENTE( cli_cobro.id_cli_cobro ) ) {
                MENSAJE_STRING( S_EL_CREDITO_NO_PUEDE_EXCEDER_LA_DEUDA );
                BORRAR_MENSAJE();
                rta = 0;
            }
        }
        else {
            //no credito
            if( cli_cobro.empresa != 0
             || ( cli_cobro.empresa == 0
               && ( oper_cli.fecha_limite_pago == 0
                 || oper_cli.fecha_limite_pago >= fechaActual ) ) ) {
                //Se chequea fecha de vencimiento solo a los 
                //individuales con fecha limite de pago excedida.
                //Si es empresa, se cobra igual.
                //Si es individual vencido solo se muestra el mensaje y no le deja cobrar
                if( rta ) {
                    if( *importe != *importeMin ) {
                        //si el importe minimo es distinto al importe
                        //if(*importeMin <= 0){ //si el minimo es 0, se puede ingresar
                        importeTemp = -10;
                        FTOC( *importeMin, impMinChar, ENTEROS, DECIMALES, SI, SI );
                        FTOC( *importe, impMaxChar, ENTEROS, DECIMALES, SI, SI );
                        while( ( importeTemp > *importe || importeTemp < *importeMin )
                            && salir == 0 ) {
                            sprintf( msg, "INGRESE UN IMPORTE ENTRE %s Y %s", impMinChar,
                                     impMaxChar );
                            MENSAJE_SIN_SONIDO( msg, NO );
                            importeTemp = PEDIR_FLOAT_EN_RECUADRO( "  IMPORTE", NO, ENTEROS,
                                                                   DECIMALES );
                            BORRAR_MENSAJE();
                        }
                        *importe = importeTemp;
                    }
                }
            }
            else {
                MENSAJE_STRING( S_LIMITE_DE_COBRO_EXCEDIDO );
                BORRAR_MENSAJE();
                rta = 0;
            }
        }
    }
    return rta;
}
/***********************************************************************/
/***********************************************************************/
void PANTALLA_COBROS_ESP( void )
/***********************************************************************/
{
    char cod[10];   
    ExecuteAction( A_ACTIVAR_PANEL_COBROS_ESPECIALES );
    SELECT( AREA_COBROS );
    GO( TOP );
    while( !BtrvEOF() && !BOF() )     // llena la grilla del panel
    {
        sprintf( cod, "%i", cobros.codigo );
        ExecuteSSAction( AGREGAR_UNIDAD_DE_NEGOCIO, 0, cod, cobros.nombre, NULL, " " );
        SKIP( 1 );
    }
}
/***********************************************************************/
int ELEGIR_UNI_NEG( int grilla, struct DatosCobro *datosCobro )
/***********************************************************************/
{
    int seguir = 1, k, rta = 0;
    long CompaniaTmp; 
    //SET_ORDER( 1 );   
	SET_ORDER2("CODIGO");
    //GO( TOP );
	GO2( TOP );
    ExecuteSAction( grilla, "0" );            // 0 habilita
    if( !_CANT || _OP_MULTIPLE ) {
        // si es el primer cobros
        while( seguir ) {
            k = GETCH();
            switch( k ) {
                case 27:
                    /*-------- ESC -----------*/
                    seguir = 0;
                    ExecuteSAction( grilla, "-99" ); //-99 deshabilita 
                    rta = 0;
                    break;
                case 32:
                    /*-------- TOTAL ---------*/
                    /*if(_ESTADO == 1)
                    {
                       seguir = 0;
                       ExecuteSAction(grilla,"-99"); //-99 deshabilita 
                       rta=0;
                    }
                    break;*/
                case 13:
                    /*-------- ENTER ---------*/
                    seguir = 0;
                    ExecuteSAction( grilla, "-99" ); //-99 deshabilita 
                    rta = 1;
                    break;
                case -72:
                    /*----- FLECHA ARRIBA -----*/
                    ExecuteSAction( grilla, "-1" );
                    //SKIP( -1 );
					SKIP2( -1 );
                    rta = 1;
                    break;
                case -80:
                    /*------ FLECHA ABAJO -----*/
                    ExecuteSAction( grilla, "1" );
                    //SKIP( 1 );
					SKIP2 ( -1 );
                    rta = 1;
                    break;
            }
        }
    }
    else {
        // si ya hay una compania cargada
        CompaniaTmp = datosCobro->_Compania;
        while( CompaniaTmp != cobros.codigo ) {
            ExecuteSAction( grilla, "1" );
            //SKIP( 1 );
			SKIP2( 1 );
        }
        ExecuteSAction( grilla, "-99" ); //-99 deshabilita 
        rta = 1;
    }
    return ( rta );
}
/***********************************************************************/
int ELEGIR_DEUDA( int grilla, int cant, struct _ind_deuda4 *indice, struct DatosCobro *datosCobro )
/***********************************************************************/
{
    int seguir = 1, k, rta = 0, cont = 1, salir = 0;
    int fechaActual = ( int )_GET_FECHA_DOS();
    SELECT( AREA_OPER_CLI );
	SET_ORDER( 4 );
    GO( TOP );
    GET_GREATER_E( ( char* )indice );
    while( !BtrvEOF() && ID_COBRADO( oper_cli.id_deuda, oper_cli.pagado ) ) {
        SKIP( 1 );
    };
    ExecuteSAction( grilla, "0" );            // 0 habilita
    while( seguir ) {
        k = GETCH();
        switch( k ) {
            case 27:
                /*-------- ESC -----------*/
                seguir = 0;
                ExecuteSAction( grilla, "-99" ); //-99 deshabilita 
                rta = 0;
                break;
            case 32:
                /*-------- TOTAL ---------*/
            case 13:
                /*-------- ENTER ---------*/ 
                if( cont == 1 || oper_cli.cobro_antig == 0 ) {
                    // si selecciono la deuda mas antigua o es un cliente empresa
                    seguir = 0;
                    ExecuteSAction( grilla, "-99" ); //-99 deshabilita 
                    rta = 1;
                }
                else {
                    if( oper_cli.fecha_limite_pago >= fechaActual ) {
                        if( SOLICITAR_CLAVES( 30 - ancho_panel, 21, NO, SI,
                                              SI/*PEDIR_CLAVE_EN_COBROS*/, "DEUDA MAS ANTIGUA",
                                              NULL, _F_SUP_DEUDA_MAS_ANTIGUA, NO ) ) {
                            //MENSAJE("DEBE SELECCIONAR LA DEUDA MAS ANTIGUA!");
                            //BORRAR_MENSAJE(  );
                            seguir = 0;
                            ExecuteSAction( grilla, "-99" );
                            rta = 1;
                        }
                    }
                    else {
                        // sale por limite de cobro
                        seguir = 0;
                        ExecuteSAction( grilla, "-99" );
                        rta = 1;
                    }
                }
                break;
            case -72:
                /*----- FLECHA ARRIBA -----*/
                ExecuteSAction( grilla, "-1" );
                salir = 0;
                if( cont > 1 ) {
                    SKIP( -1 );
                    while( !BtrvEOF() && !BOF() && !salir ) {
                        if( ID_COBRADO( oper_cli.id_deuda, oper_cli.pagado ) ) {
                            SKIP( -1 );
                        }
                        else {
                            salir = 1;
                        }
                    }
                    rta = 1;
                    cont--;
                }
                break;
            case -80:
                /*------ FLECHA ABAJO -----*/
                ExecuteSAction( grilla, "1" );                
                salir = 0;                
                if( cont < cant ) {
                    SKIP( 1 );
                    while( !BtrvEOF() && !BOF() && !salir ) {
                        if( ID_COBRADO( oper_cli.id_deuda, oper_cli.pagado ) ) {
                            SKIP( 1 );
                        }
                        else {
                            salir = 1;
                        }
                    }
                    rta = 1;
                    cont++;
                }
                break;
        }
    }
    return ( rta );
}
/*PREPARA EL RECUPERO DE MEMORIA*/
/***********************************************************************/
void REC_MEM( void )
/***********************************************************************/
{
    SET_MEMORY_INT( __ram_hora_inicio_evento, GET_HORA_DOS() ); 
    //operMultiples = STACK_INS( sizeof( int ) );
    operMultiples = STACK_INS( sizeof( short ) );
    //_cantidad = STACK_INS( sizeof( int ) );
    _cantidad = STACK_INS( sizeof( short ) );
}
/***********************************************************************/
void RECUPERAR_COBRO( struct DatosCobro *datosCobro )
/***********************************************************************/
{
    int j = 0;
    char strTemp[10];
    char nroCompTemp[10];
    char importeTemp[20];
    long Compania;
    long nroCli;
    char tipoCli;
    //_cantidad = 0;
    SubTotal = 0;
    while( j < _CANT ) {
        if( RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP, ( char* )datosCobro,
                                   sizeof( struct DatosCobro ), j ) == 0 ) {
            SELECT( AREA_COBROS );
            GO( TOP );
            SET_ORDER( 1 );
            Compania = datosCobro->_Compania;
            GET_EQUAL( ( char* )&Compania );
            sprintf( nroCompTemp, "%ld", datosCobro->_Comprobante );
            FTOC( datosCobro->_Importe, importeTemp, ENTEROS, DECIMALES, SI, SI );
            //agregarCobroEsp(cobros.nombre,nroCompTemp,importeTemp);
            datosCobro->_tipo_mov = AGREGAR_DETALLES_COBRO( datosCobro->_IdDeuda, cobros.nombre,
                                                            nroCompTemp, importeTemp );
            SubTotal += datosCobro->_Importe;
            idCobrados[j] = datosCobro->_IdDeuda;
            j++;
            //_cantidad++;
        }
    }
    if( j > 0 ) {
        //si hay algun cobro
        FTOC( SubTotal, importeTemp, ENTEROS + 1, DECIMALES, 1, 0 );
        ExecuteSAction( A_MUESTRA_SUBTOTAL_COBRO_ESP, importeTemp );
        nroCli = _NRO_CLIENTE;
        tipoCli = ( char )_TIPO_CLI;
        sprintf( strTemp, "%ld", nroCli );
        Compania = datosCobro->_Compania;
        SET_MEMORY_LONG( __ram_nro_cliente, _ID_CLIENTE );
        EXISTE_SOCIO( _BUSQ_CODSOCIO, nroCli, COD_UNIDAD_NEGOCIO, ( short )Compania, tipoCli,
                      NULL );   
        ExecuteAction( A_MUESTRA_DATO_CLIENTE );
        MOSTRAR_INPUT_CADENA( strTemp, 0 );
        //ExecuteSAction( A_MUESTRA_DATO_SOCIO_COBRO,clientes.nombre );
        MOSTRAR_SOCIO( datosCobro, _CANT - 1 );
    }
    if( _OP_MULTIPLE ) {
        ExecuteAction( A_MOSTRAR_OP_MULT );
    }
    else {
        ExecuteAction( A_OCULTAR_OP_MULT );
    }
}
/***********************************************************************/
int IMPORTAR_COBROS( int tipo_importacion, int tipo_evento )
/***********************************************************************/
{
    int ok = 1;
    char modo = -1;
    struct _eventos eventos;
    struct _ev_cont ev_cont;
    struct _ev_medios ev_pago;
    struct _ev_despro ev_despro;
    struct _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
    struct _ev_bpromo ev_b_promo;
    struct DatosCobro datosCobroTmp;     
    if( ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago, &ev_despro, &ev_dpago,
                                  &ev_cod_barra, &ev_b_promo ) ) {
        // el campo eventos.importado se graba unicamente cuando se hace un reverso.
        // En caso de reimpresiones no se graba, porque cualquier comprobante se puede
        // reimprimir.
        // Un evento de cobro, del tipo 70, marcado como reversado con valor 3, no se
        // puede reimprimir
        //if( eventos.importado != tipo_importacion ) { // esto tiene q ser manejado por interfaz
        SELECT_TABLE( T_EV_TMP, TT_ORIG );
		SET_WHERE("");
		RUN_QUERY(NO);
							  
		SET_MEMORY_INT( __ram_caja_anterior, eventos.caja );
        SET_MEMORY_INT( __ram_sucursal_anterior, eventos.suc_comprobante );
        SET_MEMORY_INT( __ram_fecha_anterior, eventos.fecha );
        SET_MEMORY_CHAR( __ram_tipo_comp_anterior, eventos.tipo_evento );
        SET_MEMORY_DOUBLE( __ram_total, eventos.importe_sin_iva );      
        SET_MEMORY_LONG( __nro_evento_anterior, eventos.id_evento );
        SET_MEMORY_CHAR( __ram_tipo_evento_comp_original, ( char )tipo_evento );
        //if( tipo_importacion == _IMPORTAR_REIMPRESION ) {            
		SELECT_TABLE( T_EVM_TMP, TT_ORIG );
        GO2( TOP );
        SET_ORDER2( "ID_EVENTO" ); //por id evento
        /*busca el ultimo id evento de un cobro realizado*/
		SET_WHERE( "ID_EVENTO = '%ld'", eventos.id_evento );
		RUN_QUERY( NO );
        if( FOUND2() ) {
            SET_MEMORY_CHAR( __ram_p_pago, 0 );
            CEREAR_STRUCT( _pago );
            while( ok && !dbEOF() && ev_pago.id_evento == eventos.id_evento ) {
                char cad[50];
                SET_MEMORY_CHAR( __var_tarjetas_tarjeta, ev_pago.tarjeta );
                SET_MEMORY_DOUBLE( __ram_resto, eventos.importe_sin_iva );
                sprintf( cad, "%lf", RAM_P_PAGO );
                _SET_MEMORY_DOUBLE( __pago_importe, RAM_P_PAGO, ev_pago.importe );
                _SET_MEMORY_DOUBLE( __pago_importe_me, RAM_P_PAGO, ev_pago.importe_me );
                modo = ( char )( ev_pago.modo_pago - 1 );
                //Para REVERSOS: controlo importes por medio de pago
                if( tipo_importacion == _IMPORTAR_REVERSO
                 && CONTROL_IMPORTES_EN_REVERSOS( modo,
                                                  ( medios[modo].moneda_extranjera )
                                                ? ev_pago.importe_me : ev_pago.importe ) ) {
                    ok = 0;
                }
                //Para REVERSOS:me fijo si el pago no ha sido retirado
                if( ok && tipo_importacion == _IMPORTAR_REVERSO && ev_pago.retirado ) {
                    MENSAJE( "OPERACION INVALIDA: NO SE PUEDE REVERSAR" );
      //              GRABAR_LOG_SISTEMA( "OPERACION INVALIDA: NO SE PUEDE REVERSAR" ,LOG_ERRORES,3);
                    ok = 0;
                }
                //Para REVERSOS: tengo que ver si todavia est� el cheque, que no se haya retirado.
                if( ok && tipo_importacion == _IMPORTAR_REVERSO && medios[modo].cheque ) {
                    SELECT( AREA_DPAGO ); // busco en la local porq ah� solo est� marcado
                    GO( TOP );
                    SET_ORDER( 1 );
                    GET_EQUAL( ( char* )&eventos.id_evento );
                    while( ok && !BtrvEOF() && event_dpago->id_evento == eventos.id_evento ) {
                        if( event_dpago->anulado ) {
                            MENSAJE( "OPERACION INVALIDA: NO SE PUEDE REVERSAR" );
//                            GRABAR_LOG_SISTEMA( "OPERACION INVALIDA: NO SE PUEDE REVERSAR", LOG_ERRORES,23 );
                            BORRAR_MENSAJE();
                            ok = 0;
                        }
                        SKIP( 1 );
                    }
                }
                if( ok ) {
                    int _pos = 0;
                    long cuenta;
                    // hay que guardar los datos importados del cheque en el bin
                    /*if( MODO_CHEQUE_COMO_UN_MEDIO && medios[modo].cheque ) {                            
                        _pos = ( modo * 100 ) + 0 ;
                    }
                    CARGAR_EV_DMEDIOS( _pos, eventos.id_evento, D_MEDIOS_PRE_IMPORTE, AREA_AUX8, &ev_pago );
                    */
                    SELECT_TABLE( T_EVM_TMP, TT_ORIG );
                    /* vueltos */
                    _SET_MEMORY_DOUBLE( __pago_vuelto_medio, RAM_P_PAGO, ev_pago.vuelto_medio );
                    _SET_MEMORY_DOUBLE( __pago_vuelto, RAM_P_PAGO, ev_pago.vuelto_propio );                                             
                    /***********/
                    cuenta = ( ( medios[modo].ticket_compra || medios[modo].mutual
                              || medios[modo].tarjeta ) ? ev_pago.numero_id : 0 );
                    PAGOS_AGREGAR_PAGO( -1/*RAM_P_PAGO*/, modo, ev_pago.importe,
                                        ev_pago.buffer1_long, cuenta, ev_pago.tarjeta,
                                        ev_pago.numero_id, ev_pago.nro_tarjeta, ev_pago.banco,
                                        ev_pago.fecha_vto, ev_pago.cantidad_cupones,
                                        ev_pago.valor_cupones, ev_pago.cod_post_cheq,
                                        ev_pago.cod_hora_acre, ev_pago.banco, ev_pago.cuit,
                                        ev_pago.telefono, ev_pago.nro_comercio, (int)ev_pago.id_registro, ev_pago.cod_medio_vuelto,
                                        ev_pago.cod_submedio_vuelto, medios[modo].vuelto, ev_pago.importe_me, (int)0, (int)0, "", 0 );

//                        ADD_MEMORY_CHAR( __ram_p_pago, 1 );
                        SELECT_TABLE( T_EVM_TMP, TT_ORIG );
                        SKIP2( 1 );
						_ADD_MEMORY_DOUBLE( __x_dep_imp, modo, ev_pago.importe );
					}
                }
            }
        else {
            ok = 0;
        }
        //}
        if( ok ) {
            SET_MEMORY_INT( _cantidad, 0 );
            /*carga todos los eventos en el archivo rec, y asi poder reutilizar las funciones de impresion*/
            CARGAR_REC_DESDE_EVENT( eventos.id_evento, &datosCobroTmp );
        }
	}
    CERRAR_EVENTOS_TEMPORALES();
    return( ok );
}
/***********************************************************************/
void IniStructs( struct DatosCobro *structura )
/***********************************************************************/
{
    memset( structura, 0, sizeof( structura ) );
    ExecuteSAction( A_MUESTRA_SUBTOTAL_COBRO_ESP, " " );
}
/***********************************************************************/
void PAGO_COBRO( struct DatosCobro *datosCobro )
/***********************************************************************/
{
    //   int      j;
    //   long     Compania;
    /*SELECT( AREA_COBROS );
    for( j = 0; j < datosCobro->cantidad; j++ ) 
    {
       SELECT( AREA_COBROS );
       GO( TOP );
       SET_ORDER( 1 );
       Compania=GET_MEMORY_LONG( datosCobro->_Compania[j]);
       GET_EQUAL(( char * ) &Compania);
       //if( cobros.tarjeta )
          //strcpy( nro_cuenta, VAR_TARJETAS_NUMERO );
       //else
          //nro_cuenta[0] = 0;
       //TICKET_COBRANZA_VARIOS( cobros.nombre, GET_MEMORY_LONG( _comprobante[j] ),
       //GET_MEMORY_LONG( _cuenta[j] ), GET_MEMORY_DOUBLE( _importe[j] ), NULL, NO );
       TICKET_COBRANZA_VARIOS_ESP( cobros.nombre, datosCobro->_Comprobante[j],
       GET_MEMORY_LONG( datosCobro->_Compania[j]), GET_MEMORY_DOUBLE( datosCobro->_Importe[j] ), NULL,
                                                       ( char ) ( j == 0 ? SI : NO ) );
    }*/
    CEREAR_ARRAY( __ram_medios_obligatorios );
    SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
    if( _ESTADO == 2 ) {
        _SET_ESTADO( 11 );
        SETEAR_FUNCION( 30, SI );
        if( PAGOS( _MODO_COBRANZA, NULL, NO ) ) {
            ADD_MEMORY_INT( __x_cobros_cantidad_servicios, cantidad );
			ADD_MEMORY_DOUBLE( __x_cobros_importes_servicios,datosCobro->_Importe );
            _SET_ESTADO( 3 );         
            //IMPRIME_PIE_COMPROBANTE( IMPRIMIR_COBROS, NO );
        }
        else {
            _SET_ESTADO( 1 );
        }
    }
    else {
        if( _ESTADO == 11 ) {
            // se cort�la corriente en la pantalla de pagos.
            //debo recuperar el cobro
        }
    }
}
/***********************************************************************/
void GRABAR_EVENTOS()
/***********************************************************************/
{
    ACTUALIZAR_X();
    GRABAR_LOG_SISTEMA( "fin enviar todos al dbrouter" , LOG_VENTAS,4);
}
/***********************************************************************/
void EVENTO_COBRO( struct DatosCobro *datosCobro )
/***********************************************************************/
{
    int area = SELECTED();
    int j, j2, pos = 0, sumarNro = 0;
    long Compania;
    struct _CliImpreso clienteGrabado[CANTIDAD_DE_SERVICIOS_POR_COBRO_ESP];
    double importeTmp = 0;
    struct _conf *Config;
    memset( clienteGrabado, -1, sizeof( clienteGrabado ) );
    for( j = 0;j < cantidad;j++ ) {
        RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP, ( char* )datosCobro,
                               sizeof( struct DatosCobro ), j );
        if( !CLI_PROCESADO( _ID_CLIENTE, _MOTIVO, clienteGrabado )
         || ( datosCobro->_nroCliente == -99 || datosCobro->_nroCliente == -98
           || datosCobro->_nroCliente == -97 ) ) {
            char *cod_barra;
            struct   DatosCobro datosCobro2;
            /*Calcula el importe por cliente*/
            importeTmp = 0;
            if( datosCobro->_nroCliente != -99 && datosCobro->_nroCliente != -98
             && datosCobro->_nroCliente != -97 ) {
                for( j2 = j;j2 < cantidad;j2++ ) {
                    RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP, ( char* )&datosCobro2,
                                           sizeof( datosCobro2 ), j2 );
                    if( _ID_CLIENTE == datosCobro2._id_cliente && _MOTIVO == datosCobro2._motivo ) {
                        importeTmp += datosCobro2._Importe;
                    }
                }
            }
            else {
                importeTmp = datosCobro->_Importe;
            }
            //SET_MEMORY_INT( __ram_perfil, datosCobro->_Compania );
            SET_MEMORY_INT( __ram_perfil, datosCobro->_dato );
            OBTENER_CONF_TKT( datosCobro, &Config );    // Obtiene la configuracion de impresion para el cobro actual
            /*Si es una reimpresion, se fija si tiene que sumar el nro comp*/
            sumarNro = ( RAM_TIPO_EVENTO_COMP_ORIGINAL == _EVENT_REIMPRESION_COMPROBANTE
                      && !INCREMENTAR_NRO_EN_REIMPRESION ) ? NO : SI;
            /*En reversos no se imprime nada. No debe incrementar numero*/
            sumarNro = ( RAM_TIPO_EVENTO_COMP_ORIGINAL == _EVENTO_DEVOLUCION_COBRO_ESPECIAL ) ? NO
                     : sumarNro;
            if( sumarNro ) {
                /*Si el cobro actual no imprime copias, entonces no actualiza el nro de comprobante*/
                OBTENER_DATOS_COMPROBANTE( _FACTURA, comp.subtipo, 1, &comp.nro_suc, &comp.nro_comp,
                                           NULL, ( Config->CantCopias > 0 ) ? SI : -1, SI );
                if( datosCobro->_nroCliente != -99 && datosCobro->_nroCliente != -98
                 && datosCobro->_nroCliente != -97 ) {
                    GUARDAR_POSICION_RECIBO( _ID_CLIENTE, _MOTIVO, pos + 1, comp.nro_comp, -1 );
                }
                else {
                    GUARDAR_POSICION_RECIBO( _ID_CLIENTE, _MOTIVO, pos + 1, comp.nro_comp, j );
                }
                datosCobro->_recibo = comp.nro_comp;
            }
            cod_barra = LEER_COD_BARRA_EN_BINARIO_ESP( j );
            SELECT( AREA_COBROS );
            Compania = datosCobro->_Compania;
            GET_EQUAL( ( char* )&Compania );
            /*guarda el detalle por cliente*/
            GRABAR_EVENTO_COBROS_VARIOS_ESP( ( int )datosCobro->_codigo, cobros.id, _RECIBO, 0,
                                             importeTmp, NULL, datosCobro->_IdDeuda,
                                             strlen( cod_barra ), datosCobro->_dato,
                                             cobros.grupo_cobro, pos, _NRO_CLIENTE, _ID_CLIENTE,
                                             ( char )_TIPO_CLI, _MOTIVO );
            pos++;
        }
        clienteGrabado[j].idCli = _ID_CLIENTE;
        clienteGrabado[j].TipoCobro = _MOTIVO;
    }
    GRABAR_SUB_DETALLE( datosCobro );
    SET_MEMORY_CHAR( __ram_no_reprocesar, 0 );   // New
    //CLOSE_DATABASE( AREA_COBROS );
    SELECT( area );
}
/***********************************************************************/
void GRABAR_SUB_DETALLE( struct DatosCobro *datosCobro )
/***********************************************************************/
{
    /*SUB DETALLE (EV_CONT) SE GUARDAN TODOS LOS COBROS RELACIONADOS CON LA E_TICKET*/
    int area,j;
    area = SELECTED();
    //SELECT( AREA_COD_BARRA );
	SELECT_TABLE( T_EV_COD_BARRA, TT_ORIG );
    for( j = 0;j < cantidad;j++ ) {
        char *cod_barra;
        RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP, ( char* )datosCobro,
                               sizeof( struct DatosCobro ), j );
        cod_barra = LEER_COD_BARRA_EN_BINARIO_ESP( j );
        memset( &event_cod_barra, 0, sizeof( event_cod_barra ) );
        event_cod_barra.caja = NRO_CAJA;
        event_cod_barra.nro_z = NRO_Z;
        event_cod_barra.id_evento = RAM_ID_EVENTO;
        event_cod_barra.posicion_cod_barra = _POSICION; //posicion en la e_ticket
        event_cod_barra.importe = datosCobro->_Importe;
        event_cod_barra.nro_comprobante = _ID_DEUDA;
        event_cod_barra.posicion_medio = j + 1;
        if( cod_barra != NULL ) {
            memcpy( event_cod_barra.codigo_barra, cod_barra,
                    sizeof( event_cod_barra.codigo_barra ) );
        }
        else {
            memset( event_cod_barra.codigo_barra, 0, sizeof( event_cod_barra.codigo_barra ) );
        }
        event_cod_barra.cod_sucursal = config.sucursal;
		if( !NRO_FECHA_OPERACION )
			SET_MEMORY_INT( __nro_fecha_operacion, GET_FECHA_DOS() );

		event_cod_barra.fecha_ticket = NRO_FECHA_OPERACION;
        event_cod_barra.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
        event_cod_barra.nro_cuenta_comprob = _COMPROBANTE;
        event_cod_barra.cuenta = ( short )datosCobro->_Compania;
        event_cod_barra.motivo = _MOTIVO;
        event_cod_barra.origen = ( INT16 )datosCobro->_tipo_mov;
        event_cod_barra.texto = datosCobro->_letra_comp;
        event_cod_barra.puntos = ( INT16 )datosCobro->_punto_venta;
        #ifdef INVEL_L
        //BEGIN_TRANSACTION();
        #endif
        INSERT2(NO);
		//INSERT();
        #ifdef INVEL_L
        //END_TRANSACTION();
        #endif
    }
    GRABAR_LOG_SISTEMA( "fin grab ev_tcomp.btr" ,LOG_VENTAS,4);
    AGREGA_BIN_EV_DMEDIO_A_BTRV( 0, D_COBRO_SERVICIOS );
    ACTUALIZA_ID_EVENTO_NEW();
    SELECT( area );
}
/***********************************************************************/
void GRABAR_EVENTO_COBROS_VARIOS_ESP( int codigo, int id, long comprobante, long cuenta,
                                      double importe, char *cta_cmr, long idDeuda, int scanner,
                                      long cobro2, long unidadnegocio, int pos, long codSocio,
                                      long idSocio, char tipoSocio, int motivo )
/***********************************************************************/
{
    //    char _cta_cmr[11];
    // struct event_varios_esp *pvarios = NULL;
    int area;
    area = SELECTED();
    //SELECT( AREA_E_TICKET );
	SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
    memset( &event_e_ticket, 0, sizeof( event_e_ticket ) );
    event_e_ticket.cod_articulo = codSocio;
    event_e_ticket.id_evento = RAM_ID_EVENTO;
    event_e_ticket.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
    event_e_ticket.importe_sin_iva = importe;
    //event_e_ticket.cod_clasificacion = idSocio;
    event_e_ticket.nro_z = NRO_Z;
    event_e_ticket.caja = NRO_CAJA;
    event_e_ticket.cod_depto = codigo;
    event_e_ticket.fecha_ticket = NRO_FECHA_OPERACION/* ( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS()*/;
    event_e_ticket.cod_sucursal = config.sucursal;
    event_e_ticket.motivo = motivo;//_RAM_MOTIVO_DESCUENTO_PENDIENTE(pos);
    event_e_ticket.cant_presentacion = ( short )cobro2; //compania
    event_e_ticket.tipo2 = ( short )unidadnegocio;
    event_e_ticket.posicion = pos + 1;
    event_e_ticket.comprobante = comprobante; // nro recibo
    event_e_ticket.tipo3 = tipoSocio;
    #ifdef INVEL_L
    //BEGIN_TRANSACTION();
    #endif
    INSERT2(NO);
    #ifdef INVEL_L
    //END_TRANSACTION();
    #endif
    GRABAR_LOG_SISTEMA( "fin grab e_ticket.btr" ,LOG_VENTAS,4);
    /*SELECT( area );
    if( pvarios == NULL ) {
        pvarios = ( struct event_varios_esp * )MALLOC( sizeof( struct event_varios_esp ) );
    }
    if( cta_cmr != NULL) {
        strcpy( _cta_cmr, cta_cmr );
    }
    if( pvarios ) {
        pvarios->codigo = codigo;
        pvarios->id_cobro = ( long )id;
        pvarios->comprobante = comprobante;
        pvarios->cuenta = cuenta;
        pvarios->importe = importe;
        GRABAR_EVENT_VARIOS( ( char* )pvarios, sizeof( struct varios_esp ), NO );
        if( cta_cmr != NULL ) {
            GRABAR_EVENT_VARIOS( _cta_cmr, strlen( _cta_cmr ), NO );
        }
        GRABAR_LOG_SISTEMA( "fin grab varios.btr" );
        FREEMEM( (char *)pvarios );
    }*/
}
/***********************************************************************/
void EVENTO_COBROS_TOTAL( struct DatosCobro *datosCobro )
/***********************************************************************/
{
    //SET_MEMORY_INT( __ram_perfil, datosCobro->_Compania );
    SET_MEMORY_INT( __ram_perfil, datosCobro->_dato );
    OBTENER_DATOS_COMPROBANTE( _FACTURA, comp.subtipo, 1, &comp.nro_suc, &comp.nro_comp, NULL, NO,
                               SI );
    memset( &event_ticket, 0, sizeof( event_ticket ) );
    event_ticket.bultos = cantidad;
    if( ( event_ticket.importe_sin_iva = SubTotal ) >= 0 ) {
        event_ticket.tipo_evento = _EVENTO_COBRO_ESPECIAL;
    }   //cobros especiales
    event_ticket.suc_comprobante = comp.nro_suc;
    if( MODO_DEVOLUCION || ( strlen( RAM_OC_GD ) > 0 ) ) {
        event_ticket.tipo_evento = RAM_TIPO_EVENTO_COMP_ORIGINAL;   //reverso y reimpresi�n
        event_ticket.id_evento_comprob_ant = NRO_EVENTO_ANTERIOR;
        event_ticket.importe_sin_iva = RAM_TOTAL;
        event_ticket.importado = 0;
        SET_MEMORY_DOUBLE( __ram_total, 0 );
    }
    event_ticket.hora_inicio = RAM_HORA_INICIO_EVENTO;    
    GRABAR_EVENT_TICKET( NO );
    GRABAR_LOG_SISTEMA( "fin grab ticket.btr" ,LOG_VENTAS,4);
    GRABA_EVENTOS_PAGO( 0, NO );
    GRABAR_LOG_SISTEMA( "fin grab pagos.btr",LOG_VENTAS,4 );
    GRABAR_SUPER_REC_A_EVENT( RAM_ID_EVENTO );
    GRABAR_LOG_SISTEMA( "fin grab super.btr" ,LOG_VENTAS,4);
}
/***********************************************************************/
void TICKET_COBRANZA_VARIOS_ESP( char *concepto, int comprobante, long cuenta, double importe,
                                 char *cta_cmr, char cabecera )
/***********************************************************************/
{
    int comas = 0;
    PRN_SELECT( SLIP );
    //PRN_LETRA_NORMAL();
    if( IMPRIMIR_COBROS ) {
        if( config.pais == COLOMBIA ) {
            comas = 1;
        }
        if( cabecera ) {
            SET_MEMORY_CHAR( __ram_empieza_ticket, 1 );
            IMPRIMIR_TKT_CABECERA( comprobante );
        }
        IMPRIMIR_TKT_CUERPO( importe, comprobante );
    }
}
/***********************************************************************/
void TERMINAR_COBRO( struct DatosCobro *datosCobro )
/***********************************************************************/
{
    int nro = 0;
    GRABAR_LOG_SUPERVISOR( ST( S_COBROS ), USUARIO_CLAVE_ANTERIOR() ,LOG_VENTAS,4);
    GRABAR_XYZ( 'X' );
    //SET_MEMORY_CHAR( _reprocesar, RAM_NO_REPROCESAR );
    /*--------------- fin ----------------*/
    BORRAR_BIN_DATOS_ADICIONALES( D_MEDIOS_PRE_IMPORTE );
    BORRAR_BIN_DATOS_ADICIONALES( D_COBRO_SERVICIOS );
    /*guarda todos las intervenciones de supervisor que no fueron guardadas 
      en los eventos*/
    GRABAR_SUPER_REC_A_EVENT( NRO_ULTIMO_EVENTO_ANALIZADO );
    FIN_DE_TICKET( SI );
    //SET_MEMORY_CHAR( __ram_no_reprocesar, GET_MEMORY_CHAR( _reprocesar ) );
    CERRAR_CAJON();
    if( ( _X_RETIRO_FONDO_IMPORTE_PAGOS( nro ) > 0 ) && ( _X_COBROS_IMPORTE( nro ) > 0 ) ) {
        if( _X_COBROS_IMPORTE( nro ) > _X_RETIRO_FONDO_IMPORTE_PAGOS( nro ) ) {
            //_SET_MEMORY_DOUBLE(__x_cobros_importe,nro,(_X_COBROS_IMPORTE(nro) - _X_RETIRO_FONDO_IMPORTE_PAGOS(nro)));
            _SET_MEMORY_DOUBLE( __x_retiro_fondo_importe_pagos, nro, 0 );
        }
        else {
            _SET_MEMORY_DOUBLE( __x_retiro_fondo_importe_pagos, nro,
                                ( _X_RETIRO_FONDO_IMPORTE_PAGOS( nro )
                                - _X_COBROS_IMPORTE( nro ) ) );
            //_SET_MEMORY_DOUBLE(__x_cobros_importe,nro,0); 
        }
    }
    /*--------------------- Fin de recupero de memoria ----------------------*/
    //CLOSE_RECUPERO( _ptr, _handle );
    /*if( ( char * ) _comprobante != NULL )
       FREEMEM( ( char * ) datosCobro->_comprobante );
    if( ( char * ) _cuenta != NULL )
       FREEMEM( ( char * ) _cuenta );
    if( ( char * ) _importe != NULL )
       FREEMEM( ( char * ) _importe );
    if( ( char * ) _codigo != NULL )
       FREEMEM( ( char * ) _codigo );*/
    ENABLE_STANDBY_AUTOMATICO(); 
    ///setData(ACTION_DOPRINT, NULL, 0);
}
/***********************************************************************/
int REVERSAR_COBRO( struct DatosCobro *datosCobro )
/***********************************************************************/
{
    int ok = 0;
    SET_MEMORY_INT( __ram_modo_devolucion, NO );
    SET_MEMORY_CHAR( __modo_devolucion, NO );                
    SET_MEMORY_INT( __ram_caja_anterior, 0 );
    SET_MEMORY_INT( __ram_sucursal_anterior, 0 );
    SET_MEMORY_INT( __ram_fecha_anterior, 0 );
    SET_MEMORY_CHAR( __ram_tipo_comp_anterior, ( char )0 );
    SET_MEMORY_DOUBLE( __ram_total, 0.0 );
    SET_MEMORY_LONG( __nro_evento_anterior, 0 );
    SET_MEMORY_CHAR( __ram_tipo_evento_comp_original, ( char )0 );
    if( SOLICITAR_CLAVES( 30, 20, NO, SI, SI, " AUTORIZACION REVERSO", " AUTORIZO REVERSO",
                          _F_SUP_REVERSO_COBRO, SI ) ) {
        ok = 1;
        if( PEDIR_MOTIVO_EN_DEVOLUCIONES ) {
            SET_MEMORY_CHAR( __modo_devolucion, SI );
            PEDIR_MOTIVO_OPERACION_NEGATIVA( NULL, _MOTIVO_DEVOL_COBRO );
            SET_MEMORY_CHAR( __modo_devolucion, NO );
            if( RAM_MOTIVO_DEVOLUCION <= 0 ) {
                ok = 0;
            }
        }
        if( ok && PEDIR_DATOS_EN_DEVOLUCIONES && !PEDIR_CAJA_Y_SECUENCIA ) {
            ok = PEDIR_BOLETA_ANTERIOR();
        }
        if( ok ) {
            SET_MEMORY_INT( __ram_modo_devolucion, _DEVOL_TOTAL );
            SET_MEMORY_CHAR( __modo_devolucion, SI );
            MOSTRAR_MODO_DEVOLUCION();
            CEREAR_TEMPORALES();
            ok = IMPORTAR_TICKET( SI, _EVENTO_COBRO_ESPECIAL, NO, " REVERSAR COBRO", NO, 0, _NO_APLICA,SI ,NO );            
            if( ok ) {
                ok = IMPORTAR_COBROS( _IMPORTAR_REVERSO, _EVENTO_DEVOLUCION_COBRO_ESPECIAL );                                               
                SET_MEMORY_CHAR( __varios_restringir_volver_de_pagos, SI );
                //importando_ticket_devolucion = 1;
                UNGETCH( 32 );
            }
            if( !ok ) {
                SET_MEMORY_INT( __ram_modo_devolucion, NO );
                SET_MEMORY_CHAR( __modo_devolucion, NO );
            }
            MOSTRAR_MODO_DEVOLUCION();
        }
    }
    if( !ok ) {
        //guardo el evento de cancelacion de reverso
        memset( &event_ticket, 0, sizeof( event_ticket ) );   
        event_ticket.suc_comprobante = comp.nro_suc;
        event_ticket.tipo_evento = _EVENTO_ANULACION_DE_COBRO;   //ANULACION DE REVERSO
        SET_MEMORY_DOUBLE( __ram_total, 0 );        
        event_ticket.hora_inicio = RAM_HORA_INICIO_EVENTO; 
        GRABAR_EVENT_TICKET( NO );
        ACTUALIZA_ID_EVENTO_NEW();
		ACTUALIZAR_X();
    }
    return ok;
}
/***********************************************************************/
void LIMPIAR_PANEL( int grillas, int cliente )
/***********************************************************************/
{
    if( grillas ) {
        ExecuteAction( A_LIMPIA_PANEL_COBROS_ESP );
    }
    if( cliente ) {
        ExecuteAction( A_MUESTRA_DATO_CLIENTE );
        MOSTRAR_INPUT_CADENA( "", 0 );
    }
    ExecuteAction( A_MUESTRA_DATO_UNIDAD_NEGOCIO );
    MOSTRAR_INPUT_CADENA( "", 0 );
    ExecuteAction( A_MUESTRA_DATO_CODIGO_BARRA );
    MOSTRAR_INPUT_CADENA( "", 0 );
}
/***********************************************************************/
int GRABAR_COD_BARRA_EN_BINARIO_ESP( char *cod_barra, int pos )
/***********************************************************************/
{
    char archivo[80];
    int handle;
    int error;
    char codigo[80];
    char nom_aux[30];
    memset( codigo, 0, sizeof( codigo ) );
    strncpy( codigo, cod_barra, sizeof( codigo ) - 1 );
    //sprintf( archivo, "ev_codba.%d", pos );
    sprintf( nom_aux, "ev_codba.%d", pos );
    sprintf( archivo, "%s%s", PATH_TMP_DADIC, nom_aux );
    if( cod_barra != NULL && pos <= CANTIDAD_DE_SERVICIOS_POR_COBRO_ESP ) {
        handle = _CREAT( archivo, S_IFREG | S_IWRITE | S_IREAD, __LINE__, __FILE__ );
        if( handle > 0 ) {
            error = _WRITE( handle, codigo, sizeof( codigo ), __LINE__, __FILE__ );
        }
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    return !error;
}
/***********************************************************************/
char * LEER_COD_BARRA_EN_BINARIO_ESP( int posicion )
/***********************************************************************/
{
    int handle;
    static char buffer[80];
    char archivo[30];
    char nom_aux[16];
    //sprintf( archivo, "ev_codba.%d", posicion );
    sprintf( nom_aux, "ev_codba.%d", posicion );
    sprintf( archivo, "%s%s", PATH_TMP_DADIC, nom_aux );
    if( access( archivo, 0 ) == 0 ) {
        handle = _OPEN( archivo, O_RDONLY, __LINE__, __FILE__ );
        if( handle > 0 ) {
            if( !_READ( handle, buffer, sizeof( buffer ), __LINE__, __FILE__ ) ) {
                return buffer;
            }
            _CLOSE( handle, __LINE__, __FILE__ );
        }
    }
    return NULL;
}
/***********************************************************************/
int CARGAR_LETRA_TIPO( int grupo, int compania, int cod_interno ) //tabla letra de comprobante / tipo de movimiento
/***********************************************************************/
{
    struct _ind_tipo_mov ind_tipo_mov;
    int rta = 0;
	int  area_ant, tipo_ant;
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();


   // if( USE_DB( AREA_TIPO_MOV, _TIP_MOV_BTR, ( char* )&tipo_mov, sizeof( tipo_mov ), NULL, 0, 0 )
   //  == 0 ) {
	if( OPEN_TABLE( T_TIP_MOV, TT_ORIG, ( char* )&tipo_mov, sizeof( tipo_mov ) ) == 0 ) {
 
		SELECT_TABLE( T_TIP_MOV, TT_ORIG );
     //   SELECT( AREA_TIPO_MOV );
        ind_tipo_mov.cod_cobro = (short)compania;
        ind_tipo_mov.grupo_cobro = (short)grupo;
        ind_tipo_mov.cod_interno = (short)cod_interno;
       // SET_ORDER( 1 );
        //GET_EQUAL( ( char* )&ind_tipo_mov );
		SET_WHERE("GRUPO_COBRO = '%d' and COD_COBRO = '%d' and COD_INTERNO = '%d'", 
			ind_tipo_mov.cod_cobro, ind_tipo_mov.grupo_cobro, ind_tipo_mov.cod_interno );
		RUN_QUERY(NO);
        if( FOUND2() ) {
            rta = 1;
        }
        else {
            rta = 0;
        }
        //CLOSE_DATABASE( AREA_TIPO_MOV );
		CLOSE_TABLE( T_TIP_MOV, TT_ORIG );
    }

	SELECT_TABLE( area_ant, tipo_ant );
    return rta;
}
/***********************************************************************/
int TECLAS_COBROS_ESPECIALES( int k, int *rta, int seguir, int *asignar )
/***********************************************************************/
{
    int ok = seguir;
    int rta1 = *rta;
    //   char aux[100];
    switch( k ) {
        case 9:
            /*---- TAB ----*//*---- REPROCESAR -----*/
            rta1 = -658;
            ok = 0;
            *asignar = 0;
            break;
        case 38:
            /*---- & (STAND BY)  ----*/
            rta1 = -659;
            ok = 0;
            *asignar = 0;
            break;
        case 45:
            /*---- - ----*//*---- Anulacion ultimo item ---*/   
            rta1 = -650;
            ok = 0;
            *asignar = 0;
            break;
            //case 179:
        case 108:
            /*---- ALT L ----*//*---- Consulta Cliente Alfabetico ---*/
            if( !_CANT || _OP_MULTIPLE ) {
                rta1 = -108;
                ok = 0;
                *asignar = 0;
            }
            break;
            //case 171:
        case 109:
            /*---- ALT K ----*//*---- Consulta Cliente Numerico ---*/
            if( !_CANT || _OP_MULTIPLE ) {
                rta1 = -109;
                ok = 0;
                *asignar = 0;
            }
            break;      
        case 119:
            /*---- ALT Y ----*//*--- Cierre ---*/
            rta1 = -660;
            ok = 0;
            *asignar = 0;
            break;
        case  -60:
            /*---- F2 ----*//*--- Retiros ---*/
            rta1 = -663;
            ok = 0;
            *asignar = 0;
            break;
        case  -62:
            /*----  F4 ----*//*---- Cancelar Cobro de Servicios Especiales ---*/     
            rta1 = -661;
            ok = 0;
            *asignar = 0;
            break;
        case -63:
            /*---- F5 ----*//*--- Fondo fijo ---*/
            rta1 = -662;
            ok = 0;
            *asignar = 0;
            break;
        case  -68:
            /*---- F10 ----*//*---- Reversar Cobro de Servicios Especiales ---*/
            rta1 = -667;
            ok = 0;
            *asignar = 0;
            break;
        case -133:
            /*---- F11 ----*//*---- Pagos ---*/
            rta1 = -133;
            ok = 0;
            *asignar = 0;         
            break;
        case -64:
            /*---- F6 ----*//*---- Carga r�pida de reintegros ---*/         
            rta1 = -64;
            ok = 0;
            *asignar = 0;         
            break;
        case 132:
            /*---- Menu de funciones ---*/
            rta1 = 132;
            ok = 0;
            *asignar = 0;
            break;
        case 150:
            /*---- Shift p ----*//*---- Reimprimir ultimo cobro ---*/
            rta1 = k;
            ok = 0;
            *asignar = 0;
            break;
        case 124:
            /*---- m ----*//*---- Entrada Modo operaciones multiples ---*/
            rta1 = k;
            ok = 0;
            *asignar = 0;
            break; 
        case 131:
            /*---- Shift m ----*//*---- Salida Modo operaciones multiples ---*/
            rta1 = k;
            ok = 0;
            *asignar = 0;
            break; 
        case 195:
            /*----- v ------*//*--- Mostrar version de caja ---*/
            rta1 = k;
            ok = 0;
            *asignar = 0;
        default:
            break;
    }
    *rta = rta1;
    return( ok );
}
/***********************************************************************/
int PROCESAR_TECLAS_COBROS_ESPECIALES( int k, struct DatosCobro *dCobro ) 
/***********************************************************************/
{
    int rta = k;
    switch( k ) {
        case -108:
            /*---- ALT L ----*//*---- Consulta Cliente Alfabetico ---*/
        case -109:
            /*---- ALT K ----*//*---- Consulta Cliente Numerico ---*/
            rta = ( int )( ( cantidad && !_OP_MULTIPLE ) ? 0 : k );
            break;
        case -658:
            /*---- TAB (REPROCESO)  ----*/                  
            rta = ( int )( cantidad ? 0 : k );
            break;
        case -659:
            /*---- & (STAND BY)  ----*/          
            rta = ( int )( cantidad ? 0 : k );
            break;
            //case :               /*---- '-' ----*//*---- Anular Cobro de Servicios Especiales ---*/         
            //         break;
        case -660:
            /*---- ALT Y ----*//*--- Cierre ---*//*--- Salida del cajero ---*/
            rta = ( cantidad ? 0 : k );              
            SETEAR_FUNCION( 158, SI );
            break;
        case -661:
            /*---- F4 ----*//*---- Cancelar Cobro de Servicios Especiales ---*/              
            rta = -661;
            SETEAR_FUNCION( 4, SI );
            break;         
        case -662:
            /*---- F5 ----*//*--- Fondo fijo ---*/
            rta = ( cantidad ? 0 : k );
            break;         
        case -663:
            /*---- F2 ----*//*--- Retiros ---*/
            rta = ( cantidad ? 0 : k );
            break;         
        case -667  :
            /*---- F10 ----*//*---- Reversar Cobro de Servicios Especiales ---*/         
            rta = ( cantidad ? 0 : k );         
            break;
        case -133:
            /*---- F11 ----*//*---- Pagos ---*/         
            rta = ( cantidad ? 0 : k );         
            break;
        case -64:
            /*---- F6 ----*//*---- Carga r�pida de reintegros ---*/   
            rta = ( cantidad ? 0 : k );
            break;
        case -650:
            /*---- - ----*//*---- Anulacion ultimo item ---*/               
            rta = ( cantidad ? k : 0 );
            break;
        case 132:
            /*---- Menu de funciones ---*/        
            break;
        case 1:
            /*---- enter ----*/            
        case 2:
            /*  Tecla total  */
            SETEAR_FUNCION( 30, SI );
            break;
        case 150:
            /*---- Shift p ----*//*---- Reimprimir ultimo cobro ---*/
            rta = ( cantidad ? 0 : k );
        default:
            //rta = 0;
            break;
    }
    return( rta );
}
/***********************************************************************/
void CEREAR_VAR_TICKET( struct DatosCobro *datosCobro ) 
/***********************************************************************/
{
    //   int i;
    SET_MEMORY_DOUBLE( __ram_cantidad, 1 );   
    CEREAR_STRUCT( _ticket );
    CEREAR_STRUCT( _tot_tick );   
    SET_MEMORY_LONG( __ram_nro_cliente, 0L );
	SET_MEMORY_LONG ( __ram_nro_cliente_ant, 0 );
    SET_MEMORY_LONG( __ram_cliente_afinidad, 0L );
    CEREAR_MEMORY( __ram_string_cliente_afinidad );
    SET_MEMORY_CHAR( __ram_pedir_cliente_afinidad, 0 );   
    SET_MEMORY_INT( __ram_perfil, 0 );
    BORRAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP );
    // SET_MEMORY_INT( __ram_motivo_descuento, 0 );
    /*for( i=0; i<CANTIDAD_DE_SERVICIOS_POR_COBRO_ESP; i++ ) {
       SET_MEMORY_INT( datosCobro->_motivo[i], 0 );
    }*/
}
/***********************************************************************/
void MOSTRAR_SOCIO( struct DatosCobro *datosCobro, int posCobro )
/***********************************************************************/
{
    char strTemp1[61];
    char strTemp2[61];
    char strTemp3[61];
    char Tipos[3][50];
    char buffer[50];
    if( EXISTE_SOCIO( _BUSQ_ID_CLICO, 0, COD_UNIDAD_NEGOCIO, 0, 0, NULL ) ) {
        sprintf( Tipos[0], "%s", "Individual" );
        sprintf( Tipos[1], "%s", "Empresa" );
        sprintf( Tipos[2], "%s", "Obra social" );
        sprintf( strTemp1, "Cliente: %s", cli_cobro.apellynom );
        sprintf( strTemp2, "%s", ( _TIPO_CLI < 3 ) ? Tipos[_TIPO_CLI] : " " );
        sprintf( strTemp3, "Domicilio: %s", cli_cobro.domicilio );
        mostrarCliCobros( strTemp1, strTemp2, strTemp3 );
    }
    ExecuteAction( A_MUESTRA_DATO_CLIENTE );
    sprintf( buffer, "%ld", cli_cobro.nro_socio );
    MOSTRAR_INPUT_CADENA( buffer, 0 );
    ExecuteAction( A_MUESTRA_DATO_UNIDAD_NEGOCIO );
    MOSTRAR_INPUT_CADENA( cobros.nombre, 0 );
}
/***************************************************************************/
void TECLA_REPROCESAR_PAGO_COBRO_ESP( struct DatosCobro *datosCobro )
/***************************************************************************/
{
/*  struct _eventos eventos;
    struct _ev_cont ev_cont;
    struct _ev_medios ev_pago;
    struct _ev_despro ev_despro;
    struct _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
    if( _CANT == 0 ) {
        // si no hay cobros cargados
        if( RAM_REPROCESANDO == 0 ) {
            if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ST( S__REPROCESAR_PAGO_ ),
                                  ST( S_REPROCESO_DEL_PAGO ), RAM_COD_FUNCION, SI ) ) {
                if( IMPORTAR_TICKET( SI, _EVENTO_COBRO_ESPECIAL, NO, " REPROCESAR COBRO", NO, 0 ) ) {
                    ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago, &ev_despro, &ev_dpago,
                                              &ev_cod_barra );
                    SELECT( AREA_AUX3 );        //EVENTOS
                    if( eventos.reproceso == 0 ) {
                        memcpy( &event_ticket, &eventos, sizeof( struct _eventos ) );
                        memcpy( &event_e_ticket, &ev_cont, sizeof( struct _ev_cont ) );
                        memcpy( &event_pago, &ev_pago, sizeof( struct _ev_medios ) );
                        //memcpy( event_despro,    &ev_despro,    sizeof( struct _ev_despro   ) );                 
                        memcpy( &event_cod_barra, &ev_cod_barra,
                                sizeof( struct _ev_cod_barra ) );
                        SET_MEMORY_DOUBLE( __ram_importe_total, event_ticket.importe_sin_iva );                  
                        SET_MEMORY_DOUBLE( __ram_total, event_ticket.importe_sin_iva );
                        //se setea __ram_total_anterior por compatibilidad con el reprocesar de DOS
                        SET_MEMORY_DOUBLE( __ram_total_anterior, event_ticket.importe_sin_iva );
                        SET_MEMORY_CHAR( __ram_reprocesando, 1 );
                        BORRAR_MENSAJE();
                        if( PAGOS( _MODO_REPROCESAMIENTO, NULL, NO ) ) {
                            GRABAR_EVENTO_REPROCESO_PAGO_ESP();                     
                            //FIN_DE_TICKET( SI );
                        }
                    }
                    else {
                        MENSAJE_STRING( S_EVENTO_YA_REPROCESADO );
                        GRABAR_LOG_SISTEMA_STRING( S_EVENTO_YA_REPROCESADO );
                        BORRAR_MENSAJE();
                    }
                    CERRAR_EVENTOS_TEMPORALES();
                }  FIN_DE_TICKET( SI );
            }
        }
    }
    SET_MEMORY_CHAR( __ram_reprocesando, 0 );*/
}
/***************************************************************************/
void GRABAR_EVENTO_REPROCESO_PAGO_ESP( void )
/***************************************************************************/
{
    event_ticket.tipo_evento = _EVENT_REPROCESO_COBRO_ESPECIAL;   //reproceso de pago
    event_ticket.reproceso = EVENTO_REPROCESADO;
    SET_MEMORY_INT( __ram_caja_anterior, event_ticket.caja );         // numero de caja del evento importado
    SET_MEMORY_LONG( __nro_evento_anterior, event_ticket.nro_evento ); // numero de secuencia del evento importado
    GRABAR_EVENT_TICKET( NO );
    GRABA_EVENTOS_PAGO( 0, NO );
    /*#ifdef INVEL_L
    BEGIN_TRANSACTION( );
    #endif
    INSERT(  );
    #ifdef INVEL_L
    END_TRANSACTION( );
    #endif*/
    AGREGA_BIN_EV_DMEDIO_A_BTRV( 0, D_COBRO_SERVICIOS );
    ACTUALIZA_ID_EVENTO_NEW();
	ACTUALIZAR_X();
}
/***************************************************************************/
int MENU_SELECCIONA_COD_COBRO( void )
/***************************************************************************/
{
    struct _menu *items = NULL;
    int cant_reg, *indice = NULL, h;
    int cant = 0, eleccion = -1, rta = 0;
    struct _cabmenu *cab;
    int columnas, area_ant = 0;
    unsigned long pos;
    area_ant = SELECTED();
    SELECT( AREA_COBROS );
    pos = GET_POSITION();    
    GO( TOP );
    cant_reg = RECCOUNT();
    items = ( struct _menu * )MALLOC( sizeof( struct _menu ) * cant_reg );        
    indice = ( int* )MALLOC( sizeof( int ) * cant_reg );
    for( h = 0;h < cant_reg;h++ ) {
        if( !( BtrvEOF() || BOF() ) ) {
            if( !cobros.cobro_a_cuenta ) {
                if( ( items[cant].nom
                    = ( char* )malloc( strlen( cobros.nombre ) + sizeof( cobros.codigo ) + 3 ) )
                 != NULL ) {
                    sprintf( items[cant].nom, "%d- %s", cobros.codigo, cobros.nombre );
                    items[cant].tecla = cobros.codigo + 48;
                    //items[cant].rta = cant;
                    items[cant].rta = cobros.codigo;
                    cant++;
                }
            }
            SKIP( 1 );
        }
    }
    if( cant ) {
        if( cant > 9 ) {
            cant = 9;
        }
        columnas = 1;
        cab = ( struct _cabmenu * )malloc( sizeof( struct _cabmenu ) * columnas );
        if( cab != NULL ) {
            cab[0].nombre = "DESCRIPCION";
            cab[0].align = ALIGN_LEFT;
            cab[0].width = 30;
            eleccion = MENU( "SELECCIONE CODIGO DE COBRO", items, cant, cab, columnas );
            free( cab );
        }
        if( eleccion != -1 ) {
            rta = eleccion;
        }
        else {
            MENSAJE_STRING( S_NO_SELECCIONO_COBRO );
            rta = 0;
        }
    }
    //libero memoria asignada
    for( h = 0;h < cant;h++ ) {
        FREEMEM( items[h].nom );
    }
    FREEMEM( ( char* )items );
    FREEMEM( ( char* )indice );
    SELECT( area_ant );
    GET_DIRECT( pos );
    return rta;
}
/***************************************************************************/
void MOSTRAR_DATO_COBRO( int dato, short tkt_columna )
/***************************************************************************/
{
    int j = _CANT - 1;
    char nroCompTemp[10];
    switch( dato ) {
        case 1:
            sprintf( nroCompTemp, "%ld", __datosCobro->_Comprobante );
            MOSTRAR_CADENA( tkt_columna, PRN, nroCompTemp );            
            break;
    }
}
/***************************************************************************/
int AGREGAR_DETALLES_COBRO( long IdDeuda, char *nombreCobro, char *nroComp, char *importeAPagar )
/***************************************************************************/
{
    char numeroComp[15],letra[2],tipoMov[10],reglaAntigua[10],fechaVenc[10],importeMin[15],
         moneda[2],fechaEmision[10],fechaHasta[10],periodoFact[10];
    //int area_ant;
    int fechaActual;
    int tipo;
	int  area_ant, tipo_ant;
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	//  area_ant = SELECTED();

   // if( USED( AREA_OPER_CLI )
   //  || USE_DB( AREA_OPER_CLI, _OP_CLI_SIC, ( char* )&oper_cli, sizeof( oper_cli ), NULL, 0, 0 )
   //  == 0 ) { ID_DEUDA
	if( OPENED( T_OPE_CLIENTE, TT_ORIG ) ||
		OPEN_TABLE( T_OPE_CLIENTE, TT_ORIG, ( char* )&oper_cli, sizeof( oper_cli ) )  == 0 ) {
	
		SELECT_TABLE( T_OPE_CLIENTE, TT_ORIG );
		SET_ORDER2( "ID_DEUDA" );
		SET_WHERE("ID_DEUDA = '%ld'", IdDeuda );
		RUN_QUERY(NO);

        //SELECT( AREA_OPER_CLI );
        //SET_ORDER( 6 );
        //GET_EQUAL( ( char* )&IdDeuda );
        if( FOUND2() ) {
            char fecha[9];
            sprintf( numeroComp, "%ld", oper_cli.nro_comprobante );
            letra[0] = oper_cli.letra_comp;
            letra[1] = 0;
            MOVTOC( oper_cli.tipo_mov, tipoMov );
            tipo = oper_cli.tipo_mov;
            //sprintf(tipoMov,      "%d",  (int)oper_cli.tipo_mov);
            sprintf( reglaAntigua, "%d", ( int )oper_cli.cobro_antig );
            fechaActual = ( int )_GET_FECHA_DOS();
            if( oper_cli.vencimiento1 >= fechaActual ) {
                DTOC( oper_cli.vencimiento1, fecha );
                sprintf( importeMin, "%5.2lf", oper_cli.importe_min1 );
            }
            else if( oper_cli.vencimiento2 >= fechaActual ) {
                DTOC( oper_cli.vencimiento2, fecha );
                sprintf( importeMin, "%5.2lf", oper_cli.importe_min2 );
            }
            else {
                DTOC( oper_cli.vencimiento3, fecha );
                sprintf( importeMin, "%5.2lf", oper_cli.importe_min3 );
            }
            sprintf( fechaVenc, "%s", strcmp( fecha, "  /  /  " ) ? fecha : " " );
            //mapear los codigos de
            if( oper_cli.cod_moneda == 0 ) {
                sprintf( moneda, "$" );
            }
            else {
                sprintf( moneda, "U$S" );
            }      
            DTOC( oper_cli.fecha_comp, fecha );
            sprintf( fechaEmision, "%s", strcmp( fecha, "  /  /  " ) ? fecha : " " );
            DTOC( oper_cli.fecha_limite_pago, fecha );
            sprintf( fechaHasta, "%s", strcmp( fecha, "  /  /  " ) ? fecha : " " );
            DTOC( oper_cli.periodo_fac, fecha );
            sprintf( periodoFact, "%s", strcmp( fecha, "  /  /  " ) ? fecha : " " );
            //agregarCobroEsp(nombreCobro, numeroComp, importeAPagar);
            agregarCobroEsp( numeroComp, letra, tipoMov, reglaAntigua, fechaVenc, importeAPagar,
                             importeMin, moneda, fechaEmision, fechaHasta, periodoFact );
        }
        else {
            tipo = 0;
            agregarCobroEsp( nroComp, " ", " ", " ", " ", importeAPagar, " ", " ", " ", " ", " " );
        }
        //CLOSE_DATABASE( AREA_OPER_CLI );
		CLOSE_TABLE( T_OPE_CLIENTE, TT_ORIG );
    }
    else {
        GRABAR_LOG_SISTEMA( "DETALLES COBRO: ERROR ABRIENDO OPER_CLI" ,LOG_ERRORES,2);
    }
    //SELECT( area_ant );
	SELECT_TABLE( area_ant, tipo_ant );
    return tipo;
}
/***************************************************************************/
int REIMPRIMIR_COBRANZAS( void )
/***************************************************************************/
{
    int cnt = 0, imprimir = 0, salir = 0, reimprimir = 0, signo = 1;
    char tipoEvento = 70,k;
    struct DatosCobro datosCobroTmp;
    //struct DatosCobro *datosCobro;
    int error = 0;
    /*struct _eventos eventos;
    struct _ev_medios ev_pago;*/
    /*Borra el archivo rec de supervisores*/
    BORRAR_ARCHIVO_REC( ARCH_REC_SUPERVISOR );
    MENSAJE( "REIMPRIMIR COBRANZAS? <ENTER> PARA SEGUIR - <ESC> PARA CANCELAR" );
    while( !salir ) {
        k = GETCH();
        switch( k ) {
            case 13:
                reimprimir = 1;
                salir = 1;
                break;
            case 27:
                salir = 1;
        }
    }
    if( reimprimir ) {
        CEREAR_TEMPORALES();
        reimprimir = IMPORTAR_TICKET( SI, _EVENTO_COBRO_ESPECIAL, NO, " REIMPRIMIR COBRANZAS", NO, 0, _NO_APLICA,SI,NO );            
        if( reimprimir ) {
            reimprimir = IMPORTAR_COBROS( _IMPORTAR_REIMPRESION, _EVENT_REIMPRESION_COMPROBANTE );            
            SET_MEMORY_CHAR( __varios_restringir_volver_de_pagos, SI );
            UNGETCH( 32 );
        }
    }
    if( reimprimir ) {
        MENSAJE_SIN_SONIDO_STRING( S_REIMPRIMIENDO );
        SET_MEMORY( __ram_oc_gd, "REIMPRESION" );
        /*Primero guarda los eventos luego imprime*/
        GUARDAR_EVENTOS_ESP( &datosCobroTmp );
        DUPLICAR_DPAGO( NRO_EVENTO_ANTERIOR );
        IMPRIMIR_COBROS_ESP( &datosCobroTmp, NO );
    }
    if( !reimprimir ) {
        SET_MEMORY_CHAR( __varios_restringir_volver_de_pagos, NO );
        SET_MEMORY_INT( __ram_caja_anterior, 0 );
        SET_MEMORY_INT( __ram_sucursal_anterior, 0 );
        SET_MEMORY_INT( __ram_fecha_anterior, 0 );
        SET_MEMORY_CHAR( __ram_tipo_comp_anterior, ( char )0 );
        SET_MEMORY_DOUBLE( __ram_total, 0.0 );
        SET_MEMORY_LONG( __nro_evento_anterior, 0 );
        SET_MEMORY_CHAR( __ram_tipo_evento_comp_original, ( char )0 );                     
        CEREAR_MEMORY( __ram_oc_gd );
    }
    SET_MEMORY_INT( _cantidad, 0 );
    /*guarda todos las intervenciones de supervisor que no fueron guardadas 
      en los eventos*/
    GRABAR_SUPER_REC_A_EVENT( RAM_ID_EVENTO - 1 );
    BORRAR_MENSAJE();
    return( reimprimir );
}
/***************************************************************************/
void GUARDAR_EVENTOS_ESP( struct DatosCobro *datosCobro )
/***************************************************************************/
{
    /*primero guarda la cabecera (ticket)*/
    EVENTO_COBROS_TOTAL( datosCobro );
    /*guarda detalle y sub detalle (e-ticket, ev_cont)*/
    EVENTO_COBRO( datosCobro );
    /*guarda el numero de evento actual por si se corta la enegia en la impresion*/
    SET_MEMORY_LONG( __nro_ultimo_evento_analizado, RAM_ID_EVENTO - 1/*_ID_EV_COBRO_ACTUAL*/ );
    //exit(0);
}
/***************************************************************************/
void DUPLICAR_DPAGO( long idEvento )
/***************************************************************************/
{
    int cnt = 0;
    SELECT( AREA_DPAGO );
    GO( TOP );
    SET_ORDER( 1 ); //por id evento
    GET_EQUAL( ( char* )&idEvento );
    while( !BtrvEOF() && event_dpago->id_evento == idEvento ) {
        /*solo le cambia el id evento*/
        event_dpago->id_evento = RAM_ID_EVENTO - 1;
        #ifdef INVEL_L
        BEGIN_TRANSACTION();
        #endif
        INSERT();
        #ifdef INVEL_L
        END_TRANSACTION();
        #endif
        cnt++;
        GO( TOP );
        SET_ORDER( 1 ); //por id evento
        GET_EQUAL( ( char* )&idEvento );
        SKIP( cnt );
    }
}
/***************************************************************************/
void CARGAR_REC_DESDE_EVENT( long idEvent, struct DatosCobro *datosCobroTmp )
/***************************************************************************/
{
    int area_ant, signo = 1;
    area_ant = SELECTED();
    signo = ( MODO_DEVOLUCION ) ? -1 : 1;
    BORRAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP );
    /*Crea el espacio en el archivo REC para luego actualizar cada registro*/
	SELECT_TABLE( T_EV_COD_BARRA, TT_ORIG );
	//SELECT( AREA_COD_BARRA );
    GO2( TOP);
	//GO( TOP );
	SET_ORDER2("ID_EVENTO");
	SET_WHERE("ID_EVENTO = '%ld'",idEvent);
	RUN_QUERY(NO);
	//SET_ORDER( 1 ); //por id evento
    //GET_EQUAL( ( char* )&idEvent );
    while( /*!BtrvEOF()*/ !dbEOF() && event_cod_barra.id_evento == idEvent ) {
        /*Guarda la estructura vacia para mantener la posicion*/
        GUARDAR_COBRO_EN_REC( datosCobroTmp );
        //SKIP( 1 );
		SKIP2( 1 );
    }
    /*selecciona la eticket donde estan todos los eventos de cobros agrupados*/
    //SELECT( AREA_E_TICKET );
    //GO( TOP );
    //SET_ORDER( 1 ); //por id evento
    //GET_EQUAL( ( char* )&idEvent );
	SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
	SET_WHERE("ID_EVENTO = '%ld'",idEvent);
	SET_ORDER2("ID_EVENTO");
	RUN_QUERY(NO);
    while( !dbEOF() && event_e_ticket.id_evento == idEvent ) {
        //SELECT( AREA_COD_BARRA );
        //GO( TOP );
        //SET_ORDER( 1 ); //por id evento
        //GET_EQUAL( ( char* )&idEvent );
		SELECT_TABLE( T_EV_COD_BARRA, TT_ORIG );
	    GO2( TOP);
		SET_ORDER2("ID_EVENTO");
		SET_WHERE("ID_EVENTO = '%ld'",idEvent);
		RUN_QUERY(NO);
        /*recorre todos los cobros realizados para el idEvento grabando en el archivo rec todos  
          los que corresponden al evento de la eticket*/
        while( /*!BtrvEOF()*/ !dbEOF() && event_cod_barra.id_evento == idEvent ) {
            if( event_cod_barra.posicion_cod_barra == event_e_ticket.posicion ) {
                //datosCobroTmp->_Compania = event_e_ticket.cant_presentacion;
                datosCobroTmp->_Compania = event_cod_barra.cuenta;
                datosCobroTmp->_motivo = event_e_ticket.motivo;
                datosCobroTmp->_nroCliente = event_e_ticket.cod_articulo;
                datosCobroTmp->_id_cliente = event_e_ticket.cod_clasificacion;
                datosCobroTmp->_recibo = event_e_ticket.comprobante;
                datosCobroTmp->_TipoCli = event_e_ticket.tipo3;
                datosCobroTmp->_dato = event_e_ticket.cant_presentacion;
                datosCobroTmp->_posicion = event_cod_barra.posicion_cod_barra;
                datosCobroTmp->_Importe = event_cod_barra.importe * signo;
                datosCobroTmp->_Comprobante = event_cod_barra.nro_cuenta_comprob;
                datosCobroTmp->_IdDeuda = event_cod_barra.nro_comprobante;
                datosCobroTmp->_codigo = event_e_ticket.cod_depto;
                datosCobroTmp->_tipo_mov = event_cod_barra.origen;
                datosCobroTmp->_letra_comp = event_cod_barra.texto;
                datosCobroTmp->_punto_venta = event_cod_barra.puntos;                
                ACTUALIZAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP, ( char* )datosCobroTmp,
                                        sizeof( struct DatosCobro ),
                                        event_cod_barra.posicion_medio - 1 );
                ADD_MEMORY_INT( _cantidad, 1 );
            }
            //SKIP( 1 );
			SKIP2(1);
        }
        //SELECT( AREA_E_TICKET );
		SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
        SKIP2( 1 );
    }
    cantidad = _CANT;
    SELECT( area_ant );
}
/***************************************************************************/
void IMPRIMIR_COBROS_ESP( struct DatosCobro *datosCobro, int timbrar )
/***************************************************************************/
{
    int j,cnt = 1,rta,imprimir = 1;
    struct _CliImpreso clienteImpreso[CANTIDAD_DE_SERVICIOS_POR_COBRO_ESP];
    int ok;
    struct _conf *Config;       
    SET_MEMORY_CHAR( __ram_imprimir_cabecera, ( char )1 );
    memset( clienteImpreso, -1, sizeof( clienteImpreso ) );
    getData( ACTION_READ_PRINTER_STATUS, ( char* )&ok, 0 );
    OBTENER_DATOS_COMPROBANTE( _FACTURA, comp.subtipo, 1, &comp.nro_suc, &comp.nro_comp, NULL, NO,
                               SI );
    if( ok == 1 ) {
        #ifdef _DEBUG
        int k;
        MENSAJE_SIN_SONIDO( "Imprimir? S/N", NO );
        imprimir = ( ( k = GETCH() ) != 'S' && k != 's' && k != 127 && k != 130 ) ? NO : SI;
        BORRAR_MENSAJE();
        #endif
        //Si hay conexion con el Java Server
        if( imprimir ) {
            PRN_SELECT( SLIP );
            for( j = 0;j < cantidad;j++ ) {
                RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP, ( char* )datosCobro,
                                       sizeof( struct DatosCobro ), j );
                if( !CLI_PROCESADO( _ID_CLIENTE, _MOTIVO, &clienteImpreso[0] )
                /*|| datosCobro->_nroCliente == -99*/ || datosCobro->_nroCliente == -98
                 || datosCobro->_nroCliente == -97 ) {
                    SET_MEMORY_INT( __ram_perfil,
                                    ( datosCobro->_Compania > 3 ) ? datosCobro->_dato
                                  : datosCobro->_Compania );      //compania actual
                    /*busca el socio para poder imprimir sus datos*/
                    SET_MEMORY_LONG( __ram_nro_cliente, _ID_CLIENTE );
                    EXISTE_SOCIO( _BUSQ_ID_CLICO, 0, 0, 0, ' ', NULL );     // busca el cliente
                    OBTENER_CONF_TKT( datosCobro, &Config );                // Obtiene la configuracion para el cobro actual
                    for( cnt = 1;cnt<= Config->CantCopias;cnt++ )           // Imprime n cantidad de copias para un cliente
                    {
                        if( datosCobro->_nroCliente == -99 || datosCobro->_nroCliente == -98
                         || datosCobro->_nroCliente == -97 ) {
                            IMPRIMIR_CLIENTE( datosCobro, j, NO );
                        }
                        else {
                            IMPRIMIR_CLIENTE( datosCobro, j, SI );
                        }
                    }
                    /* AHORA EL TIMBRADO */
                    if( timbrar ) {
                        TIMBRAR( datosCobro, Config, j, _F_SUP_RETIMBRADO_IMPRESION );
                    }
                    else {
                        if( CONTINUAR_TIMBRADO() ) {
                            rta = SOLICITAR_CLAVES( 60, 19, NO, SI, SI, "AUTORIZAR TIMBRADO",
                                                    "AUTORIZAR TIMBRADO",
                                                    _F_SUP_TIMBRADO_REIMPRESION, NO );
                            if( rta ) {
                                TIMBRAR( datosCobro, Config, j, _F_SUP_RETIMBRADO_REIMPRESION );
                            }
                        }
                    }
                }
                clienteImpreso[j].idCli = _ID_CLIENTE;
                clienteImpreso[j].TipoCobro = _MOTIVO; //indica si es cobro con comp, a cuenta o manual
            }
        }
    }
    else {
        //Si no hay conexion con el Java Server
        MENSAJE_STRING( S_NO_HAY_CONEXION_CON_LA_IMPRESORA );
    }
}
/***************************************************************************/
void TIMBRAR( struct DatosCobro *datosCobro, struct _conf *Conf, int j, int funSup )
/***************************************************************************/
{
    int cnt1 = 1,cnt = 1,ok = 0,j2;
    char msg[100];
    struct    DatosCobro datosCobro2;
    if( Conf->CantCopias > 0 ) {
        SET_MEMORY_INT( __ram_variable_validacion, 1 );       //Indica que tiene que timbrar "Nro Recibo"
        SET_MEMORY_LONG( __nro_ticket, comp.nro_comp );       //numero de comprobante a timbrar
        for( cnt1 = 1;cnt1<= Conf->CantCopias;cnt1++ ) {
            //contador de copias
            sprintf( msg, "INGRESE EL RECIBO: %i  PARA TIMBRAR, PRESIONE UNA TECLA AL FINALIZAR",
                     comp.nro_comp );
            for( cnt = 1;cnt<= Conf->CantTimbrado;cnt++ ) {
                //contador de timbrados por copia
                ok = 0;
                while( !ok ) {
                    TIMBRADO_TKT( j );
                    MENSAJE_TECLA( msg );
                    if( !REIMPRIMIR( SI, funSup ) ) {
                        ok = 1;
                    }
                }
            }
            if( Conf->TimbradoOpcional ) {
                TIMBRADO_OPCIONAL( msg, funSup, j );
            }
        }
    }
    else {
        /* si no tiene que imprimir, entonces timbra
            todos los comprobantes por separado */
        for( j2 = j;j2 < cantidad;j2++ ) {
            RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP, ( char* )&datosCobro2,
                                   sizeof( datosCobro2 ), j2 );
            if( _ID_CLIENTE == datosCobro2._id_cliente && _MOTIVO == datosCobro2._motivo ) {
                SET_MEMORY_INT( __ram_variable_validacion, 0 ); //Indica que tiene que timbrar "Nro cpte"
                SET_MEMORY_LONG( __nro_ticket, datosCobro2._Comprobante ); //numero de comprobante a timbrar
                SET_MEMORY_DOUBLE( __ram_total, datosCobro2._Importe ); //setea el importe a timbrar
                sprintf( msg, "INGRESE EL CPTE: %i  PARA TIMBRAR, PRESIONE UNA TECLA AL FINALIZAR",
                         datosCobro2._Comprobante );
                for( cnt = 1;cnt<= Conf->CantTimbrado;cnt++ ) {
                    //contador de timbrados por copia
                    ok = 0;
                    while( !ok ) {
                        TIMBRADO_TKT( j );
                        MENSAJE_TECLA( msg );
                        if( !REIMPRIMIR( SI, funSup ) ) {
                            ok = 1;
                        }
                    }
                }
                if( Conf->TimbradoOpcional ) {
                    TIMBRADO_OPCIONAL( msg, funSup, j );
                }
            }
        }
    }
}
/***************************************************************************/
int REIMPRIMIR( int timbrado, int funSup )
/***************************************************************************/
{
    int k,salir = 0,reimprimir = 0,ok = 1;
    if( timbrado ) {
        MENSAJE_STRING( S_EL_TIMBRADO_SE_REALIZO_CORRECTAMENTE );
    }
    else {
        MENSAJE_STRING( S_LA_IMPRESION_SE_REALIZO_CORRECTAMENTE );
    }
    while( !salir ) {
        k = GETCH();
        switch( k ) {
            case 127:
                //s
            case 130:
                //S
                if( !ok )           //si no se realizo correctamente
                {
                    reimprimir = SOLICITAR_CLAVES( 60, 19, NO, SI, SI, "AUTORIZAR TIMBRADO",
                                                   "AUTORIZAR TIMBRADO", funSup, NO );
                    if( !reimprimir ) {
                        MENSAJE_STRING( S_REINTENTAR_S_N );
                    }
                    else {
                        salir = 1;
                    }
                }
                else {
                    reimprimir = 0;
                    salir = 1;
                }
                break;
            case 135:
                //n
            case 191:
                //N
                if( ok ) {
                    MENSAJE_STRING( S_REINTENTAR_S_N );
                    ok = 0;
                }
                else {
                    salir = 1;
                }
        }
    }
    return reimprimir;
}
/***************************************************************************/
void TIMBRADO_OPCIONAL( char *msg, int funSup, int indice )
/***************************************************************************/
{
    int k,salir = 0,ok = 0;
    MENSAJE_STRING( S_UTILIZAR_TIMBRADO_OPCIONAL_S_N );
    while( !salir ) {
        k = GETCH();
        switch( k ) {
            case 127:
                //s
            case 130:
                //S
                while( !ok ) {
                    TIMBRADO_TKT( indice );
                    MENSAJE_TECLA( msg );
                    if( !REIMPRIMIR( SI, funSup ) ) {
                        ok = 1;
                    }
                }
                salir = 1;
                break;
            case 135:
                //n
            case 191:
                //N
                salir = 1;
        }
    }
}
/***************************************************************************/
int CONTINUAR_TIMBRADO()
/***************************************************************************/
{
    int rta,salir = 0,k;
    MENSAJE_SIN_SONIDO( "CONTINUA CON TIMBRADO? <1> ACEPTAR, <2> SOLO REIMPRESION", NO );
    while( !salir ) {
        k = GETCH();
        switch( k ) {
                //1
            case 49:
                rta = 1;
                salir = 1;
                break;
                //2
            case 50:
                rta = 0;
                salir = 1;
                break;
        }
    }
    BORRAR_MENSAJE();
    return rta;
}
/***************************************************************************/
void IMPRIMIR_CLIENTE( struct DatosCobro *datosCobro, int j, int todos )
/***************************************************************************/
{
    int j2,i;
    double importeTmp = 0;
    struct    DatosCobro datosCobro2;
    long Compania;
    importeTmp = 0;
    /*primero calcula el importe*/
    if( todos ) {
        for( j2 = j;j2 < cantidad;j2++ ) {
            RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP, ( char* )&datosCobro2,
                                   sizeof( datosCobro2 ), j2 );
            if( _ID_CLIENTE == datosCobro2._id_cliente && _MOTIVO == datosCobro2._motivo ) {
                importeTmp += datosCobro2._Importe;
            }
        }
    }
    else {
        importeTmp += datosCobro->_Importe;
    }
    SET_MEMORY_DOUBLE( __ram_total, importeTmp );
    comp.nro_comp = _RECIBO;
    /*primero calcula la cantidad total de hojas*/
    for( i = 1 ;i >= 0 ;i-- ) {
        calcHojas = i;  //cuando i es 1 indica que tiene que calcular las hojas
        if( todos ) {
            for( j2 = j;j2 < cantidad;j2++ ) {
                RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP, ( char* )&datosCobro2,
                                       sizeof( datosCobro2 ), j2 );
                if( _ID_CLIENTE == datosCobro2._id_cliente && _MOTIVO == datosCobro2._motivo ) {
                    SELECT( AREA_COBROS );
                    GO( TOP );
                    SET_ORDER( 1 );
                    Compania = datosCobro2._Compania;
                    GET_EQUAL( ( char* )&Compania );
                    TICKET_COBRANZA_VARIOS_ESP( cobros.nombre, j2, datosCobro2._Compania,
                                                datosCobro2._Importe, NULL,
                                                ( char )( j2 - j == 0 ? SI : NO ) );
                }
            }
        }
        else {
            SELECT( AREA_COBROS );
            GO( TOP );
            SET_ORDER( 1 );
            Compania = datosCobro->_Compania;
            GET_EQUAL( ( char* )&Compania );
            TICKET_COBRANZA_VARIOS_ESP( cobros.nombre, j, datosCobro->_Compania,
                                        datosCobro->_Importe, NULL, SI );
        }
        IMPRIMIR_TKT_PIE();
		if( TKT_CONFIGURABLE ) {
			IMPRIME_TKT_IMPORTE_TOTAL( 1 );
		}
        PAGOS_IMPRIME_PAGO( _MODO_COBRANZA );
		if( RAM_ANULAR_TICKET > 100 )
			FIN_DE_TICKET( NO );

        IMPRIME_PIE_COMPROBANTE( IMPRIMIR_COBROS, NO );
        cantTotalCopias = cantCopias;
        cantCopias = 0;
    }
    setData( ACTION_DOPRINT, NULL, 0 );
    cantCopias = 0;
}
/***************************************************************************/
void OBTENER_CONF_TKT( struct DatosCobro *datosCobro, struct _conf **Conf )
/***************************************************************************/
{
    /*si es socio directo Swiss (individual) - Cupos Recibo*/
    if( ( 1 == datosCobro->_Compania || 1 == datosCobro->_dato ) && 0 == datosCobro->_TipoCli ) {
        //Si es Recibo Manual
        if( 1 == datosCobro->_motivo ) {
            *Conf = &ConfImpresion.SmgRmanual;
            //Si es Pago a Cuenta
        }
        else if( 2 == datosCobro->_motivo ) {
            *Conf = &ConfImpresion.SmgPcuenta;
            //Si es Pago con comprobante
        }
        else if( 0 == datosCobro->_motivo ) {
            *Conf = &ConfImpresion.SmgPComp;
        }
    }
    /*si es Empresa - Obra social - Socios directos Docthos (individual)*/
    if( 1 == datosCobro->_TipoCli || 2 == datosCobro->_TipoCli
     || ( ( 3 == datosCobro->_Compania || 3 == datosCobro->_dato ) && 0 == datosCobro->_TipoCli ) ) {
        //Si es Recibo Manual
        if( 1 == datosCobro->_motivo ) {
            *Conf = &ConfImpresion.OtrosRManual;
            //Si es Pago a Cuenta
        }
        else if( 2 == datosCobro->_motivo ) {
            *Conf = &ConfImpresion.OtrosPCuenta;
            //Si es Pago con comprobante
        }
        else if( 0 == datosCobro->_motivo ) {
            *Conf = &ConfImpresion.OtrosPComp;
        }
    }
    if( datosCobro->_motivo == 99 ) {
        //Si es Futuro Socio
        *Conf = &ConfImpresion.SmgFuturoSocio;
    }
    else if( datosCobro->_motivo == 98 ) {
        //Si es Socio dado de baja
        *Conf = &ConfImpresion.SocioBaja;
    }
    /*Otros Ingresos*/
    if( 5 == datosCobro->_Compania ) {
        *Conf = &ConfImpresion.OtrosIngresos;
    }
    /*
    switch( datosCobro->_Compania ) {
        case 1:
            //Swiss Medical
            *Conf = &ConfImpresion.SmgPComp;
            break;
        case 3:
            //docthos
            *Conf = &ConfImpresion.DocthosPComp;
            break;
        case 4:
            //Cobros sin Comprobantes
            if( datosCobro->_dato == 1 ) {
                //Si es Swiss Medical
                if( datosCobro->_motivo == 1 ) {
                    //Si es Recibo Manual
                    *Conf = &ConfImpresion.SmgRmanual;
                }
                else if( datosCobro->_motivo == 2 ) {
                    //Si es Pago a Cuenta
                    *Conf = &ConfImpresion.SmgPcuenta;
                }
            }
            else if( datosCobro->_dato == 3 ) {
                //Si es Docthos
                if( datosCobro->_motivo == 1 ) {
                    //Si es Recibo Manual
                    *Conf = &ConfImpresion.DocthosRManual;
                }
                else if( datosCobro->_motivo == 2 ) {
                    //Si es Pago a Cuenta
                    *Conf = &ConfImpresion.DocthosPCuenta;
                }
            }
            else if( datosCobro->_dato == 0 ) {
                //Futuro socio / socio dado de baja
                if( datosCobro->_motivo == 99 ) {
                    //Si es Futuro Socio
                    *Conf = &ConfImpresion.SmgFuturoSocio;
                }
                else if( datosCobro->_motivo == 98 ) {
                    //Si es Socio dado de baja
                    *Conf = &ConfImpresion.SocioBaja;
                }
            }
            break;
        case 5:
            //Otros ingresos
            *Conf = &ConfImpresion.OtrosIngresos;
            break;
    } */
}
/***************************************************************************/
int CLI_PROCESADO( long cliente, int tipoCobro, struct _CliImpreso *clieImpresos )
/***************************************************************************/
{
    int rta = 0,cnt = 0;
    while( cnt < CANTIDAD_DE_SERVICIOS_POR_COBRO_ESP && clieImpresos[cnt].idCli != -1 ) {
        if( cliente == clieImpresos[cnt].idCli && tipoCobro == clieImpresos[cnt].TipoCobro ) {
            rta = 1;
            break;
        }
        cnt ++;
    }
    return( rta );
}
/***************************************************************************/
int GUARDAR_POSICION_RECIBO( long cliente, int motivo, int pos, long recibo, int indice )
/***************************************************************************/
{
    int rta = 0,cnt = 0;
    struct   DatosCobro datosCobroTmp;   
    if( indice == -1 ) {
        // cuando no es socio de baja ni futuro socio
        // graba el mismo numero de comprobante para
        // todos los clientes iguales.
        while( cnt < _CANT ) {
            RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP, ( char* )&datosCobroTmp,
                                   sizeof( struct DatosCobro ), cnt );
            if( cliente == datosCobroTmp._id_cliente && motivo == datosCobroTmp._motivo ) {
                datosCobroTmp._posicion = pos ;
                datosCobroTmp._recibo = recibo;
                ACTUALIZAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP, ( char* )&datosCobroTmp,
                                        sizeof( struct DatosCobro ), cnt );
            }
            cnt ++;
        }
    }
    else {
        // cuando es socio de baja o futuro socio graba un solo numero de comprobante
        // por cada socio.
        RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP, ( char* )&datosCobroTmp,
                               sizeof( struct DatosCobro ), indice );
        datosCobroTmp._posicion = pos ;
        datosCobroTmp._recibo = recibo;
        ACTUALIZAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP, ( char* )&datosCobroTmp,
                                sizeof( struct DatosCobro ), indice );
    }
    return( rta );
}
/***************************************************************************/
void GUARDAR_COBRO_EN_REC( struct DatosCobro *datosCobro )
/***************************************************************************/
{
    if( CANTIDAD_REGISTROS_REC( ARCH_REC_COBROS_ESP, sizeof( struct DatosCobro ) ) > _CANT ) {
        ACTUALIZAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP, ( char* )datosCobro,
                                sizeof( struct DatosCobro ), _CANT );
    }
    else {
        GUARDAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP, ( char* )datosCobro,
                             sizeof( struct DatosCobro ) );
    }
}
/***************************************************************************/
void CARGAR_CONF_IMPRESION()
/***************************************************************************/
{
    FILE *archivo;
    char buffer[161];
    char grupo[50];
    char variable[50];
    char valor[50];
    char *pos1,*pos2;
    int rta = 1;
    /*--------------- Abrimos el archivo de configuracion ---------------*/
    archivo = fopen( TKT_CAJA_INI, "rt" );
    if( archivo ) {
        /*-------------- Leemos un renglon y los procesamos -----------*/
        while( fgets( buffer, 160, archivo ) && rta ) {
            LTRIM( buffer );                      //saca los espacios que pueda tener al principio de la cadena
            pos1 = strchr( buffer, '\n' );
            _strupr( buffer );                   //convierte todo a mayuscula
            if( pos1 ) {
                *pos1 = 0;
            } 
            if( buffer[0] == '[' ) {
                pos1 = &buffer[1];
                pos2 = strchr( pos1, ']' );
                if( pos1 && pos2 ) {
                    *pos2 = 0;
                    strcpy( grupo, pos1 );           //guarda el nombre del grupo 
                }
            }
            else if( buffer[0] != ';' ) {
                pos1 = buffer;
                pos2 = strchr( pos1, '=' );
                if( pos1 && pos2 ) {
                    strcpy( valor, &pos2[1] );       //guarda el valor de la variable
                    *pos2 = 0;
                    strcpy( variable, pos1 );        //guarda el nombre de la variable
                    SETEAR_CONF( grupo, variable, valor );
                }
            }
        }
        fclose( archivo );
    }
    else {
        MENSAJE_STRING( S_NO_SE_ENCUENTRA_EL_ARCHIVO_tkt_caja );
    }
}
/***************************************************************************/
void SETEAR_CONF( char *grupo, char *variable, char *valor )
/***************************************************************************/
{
    if( !strcmp( grupo, "PAGO CON COMPROBANTE SWISS" ) ) {
        SETEAR_VAR( &ConfImpresion.SmgPComp, variable, valor );
    }
    else if( !strcmp( grupo, "PAGO A CUENTA SWISS" ) ) {
        SETEAR_VAR( &ConfImpresion.SmgPcuenta, variable, valor );
    }
    else if( !strcmp( grupo, "RECIBO MANUAL SWISS" ) ) {
        SETEAR_VAR( &ConfImpresion.SmgRmanual, variable, valor );
    }
    else if( !strcmp( grupo, "FUTURO SOCIO" ) ) {
        SETEAR_VAR( &ConfImpresion.SmgFuturoSocio, variable, valor );
    }
    else if( !strcmp( grupo, "PAGO CON COMPROBANTE OTROS" ) ) {
        SETEAR_VAR( &ConfImpresion.OtrosPComp, variable, valor );
    }
    else if( !strcmp( grupo, "PAGO A CUENTA OTROS" ) ) {
        SETEAR_VAR( &ConfImpresion.OtrosPCuenta, variable, valor );
    }
    else if( !strcmp( grupo, "RECIBO MANUAL OTROS" ) ) {
        SETEAR_VAR( &ConfImpresion.OtrosRManual, variable, valor );
    }
    else if( !strcmp( grupo, "OTROS INGRESOS" ) ) {
        SETEAR_VAR( &ConfImpresion.OtrosIngresos, variable, valor );
    }
    else if( !strcmp( grupo, "SOCIO DADO DE BAJA" ) ) {
        SETEAR_VAR( &ConfImpresion.SocioBaja, variable, valor );
    }
}
/***************************************************************************/
void SETEAR_VAR( struct _conf *grupo, char *variable, char *valor )
/***************************************************************************/
{
    if( !strcmp( variable, "CANTIDAD DE COPIAS" ) ) {
        grupo->CantCopias = atoi( valor );
    }
    else if( !strcmp( variable, "CANTIDAD DE TIMBRADO" ) ) {
        grupo->CantTimbrado = atoi( valor );
    }
    else if( !strcmp( variable, "TIMBRADO OPCIONAL" ) ) {
        grupo->TimbradoOpcional = ( !strcmp( valor, "SI" ) ) ? 1 : 0;
    }
}
/************************************************/
/* Funciones para manejo de eventos incompletos */
/************************************************/
/***************************************************************************/
void GUARDAR_ID_COBRO( long valor )
/***************************************************************************/
{
    struct _cobro_inc cobroInc;
    RECUPERAR_ARCHIVO_REC( ARCH_REC_COBRO_INC, ( char* )&cobroInc, sizeof( struct _cobro_inc ), 0 );
    cobroInc.id_ev_cobro_actual = valor;
    ACTUALIZAR_ARCHIVO_REC( ARCH_REC_COBRO_INC, ( char* )&cobroInc, sizeof( struct _cobro_inc ),
                            0 );
}
/***************************************************************************/
int EVENTO_COBRO_COMPLETO()
/***************************************************************************/
{
    struct _cobro_inc cobroInc;
    RECUPERAR_ARCHIVO_REC( ARCH_REC_COBRO_INC, ( char* )&cobroInc, sizeof( struct _cobro_inc ), 0 );
    return cobroInc.evento_cobro_completo;
}
/***************************************************************************/
void GUARDAR_EVENTO_COMPLETO( int valor )
/***************************************************************************/
{
    struct _cobro_inc cobroInc;
    cobroInc.evento_cobro_completo = valor;
    ACTUALIZAR_ARCHIVO_REC( ARCH_REC_COBRO_INC, ( char* )&cobroInc, sizeof( struct _cobro_inc ),
                            0 );
}
/****************************************************/
/* FIN Funciones para manejo de eventos incompletos */
/****************************************************/
/***************************************************************************/
double IMPORTE_CLIENTE( long id_cliente )
/***************************************************************************/
{
    struct DatosCobro dC;
    double importeTmp = 0;
    int i;
    /*Calcula el importe por cliente*/
    for( i = 0;i < cantidad;i++ ) {
        RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_ESP, ( char* )&dC, sizeof( dC ), i );
        if( id_cliente == dC._id_cliente ) {
            importeTmp += dC._Importe;
        }
    }
    return importeTmp;
}
/***************************************************************************/
int VALOR_OBLIGATORIO( void )
/***************************************************************************/
{
    return( exigir_ingreso_de_valor );
}
/***************************************************************************/
int DEUDA_MAS_ANTIGUA( long nroSocio, int Compania, int tipoCli, long idDeuda )
/***************************************************************************/
{
    //retorna 1 si es la deuda mas antigua
    //retorna 0 si no es la deuda mas antigua
    struct _ind_deuda4 ind_deuda3;
    int /*areaAnt,*/rta; 
	int  area_ant, tipo_ant;

	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    
	ind_deuda3.cod_cobro = (short)Compania;
    ind_deuda3.nro_socio = nroSocio;   
    ind_deuda3.tipo_cli = (short)tipoCli;
    ind_deuda3.fecha = 0;
   // if( !USED( AREA_OPER_CLI ) ) {
   //     USE_DB( AREA_OPER_CLI, _OP_CLI_SIC, ( char* )&oper_cli, sizeof( oper_cli ), NULL, 0, 0 );
   // }
	if( !OPENED( T_OPE_CLIENTE, TT_ORIG ) ) {
		OPEN_TABLE( T_OPE_CLIENTE, TT_ORIG, ( char* )&oper_cli, sizeof( oper_cli ) ); 
	}
    //SELECT( AREA_OPER_CLI );
    //SET_ORDER( 4 );
    //GO( TOP );
    //GET_GREATER_E( ( char* )&ind_deuda3 );
	SELECT_TABLE( T_OPE_CLIENTE, TT_ORIG );
	SET_ORDER2( "COD_COBRO, NRO_SOCIO, TIPO_CLI, FECHA_COMP" );

	SET_WHERE("COD_COBRO >= '%d' and NRO_SOCIO >= '%ld' and TIPO_CLI >= '%d' and FECHA_COMP >= '%d'",
		ind_deuda3.cod_cobro, ind_deuda3.nro_socio, ind_deuda3.tipo_cli, ind_deuda3.fecha );
		RUN_QUERY(NO);
    while( ID_COBRADO( oper_cli.id_deuda, oper_cli.pagado ) ) {
        //pasa los registros hasta encontrar el primero no cobrado
        //y asi el primero seria el mas antiguo
        SKIP2( 1 );
    }
    if( idDeuda == oper_cli.id_deuda ) {
        rta = 1;   //retorna 1 si es la mas antigua
    }
    else {
        rta = 0;   //retorna 0 si no es la mas antigua
    }
   // SET_ORDER( 6 );
	SELECT_TABLE( T_OPE_CLIENTE, TT_ORIG );
	SET_ORDER2( "ID_DEUDA" );
	SET_WHERE("ID_DEUDA = '%ld'", idDeuda );
	RUN_QUERY(NO);
   // GET_EQUAL( ( char* )&idDeuda ); // selecciona la deuda 
   // SELECT( areaAnt );
	SELECT_TABLE( area_ant, tipo_ant );
    return rta;
}
/***************************************************************************/
void SETEAR_ACUMULADORES( void )
/***************************************************************************/
{
    int area_ant = SELECTED();
    if( RAM_MODO_DEVOLUCION ) {            
        if( PAGOS_GRABA_PAGO( _MODO_COBRANZA, NO ) ) {
                //ok
                //MENSAJE( "OKKKKKKKKKKKKK" );
        }        
        ACTUALIZAR_ESTADO_EVENTOS();
        ADD_MEMORY_DOUBLE( __x_devoluciones_importe, RAM_TOTAL );
        ADD_MEMORY_INT( __x_devoluciones_cantidad, 1 );
    }
    SELECT( area_ant );
}
/*****************************************************************************/
int CONTROL_IMPORTES_EN_REVERSOS( int nro, double importe )
/*****************************************************************************/
{
    if( CONTROL_DE_IMPORTES_EN_RETIROS ) {
        if( medios[nro].moneda_extranjera ) {
            if( importe
              > ( _X_VENTAS_IMPORTE_ME( nro ) + _X_COBROS_IMPORTE_ME( nro )
                + _X_FONDO_FIJO_MULTIPLE_IMP_ME( nro ) - _X_RETIROS_IMPORTE_ME( nro ) ) + 0.02 ) {
                MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
                return( 1 );
            }
        }
        else {
            if( importe
              > ( _X_VENTAS_IMPORTE( nro ) + _X_COBROS_IMPORTE( nro )
                + _X_FONDO_FIJO_MULTIPLE_IMPORTE( nro ) - X_PAGOS_POR_CAJA_IMPORTE
                - _X_RETIROS_IMPORTE( nro ) ) + 0.02 ) {
                MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
                BORRAR_MENSAJE();        
                return( 1 );
            }
        }
    }
    return( 0 );
}
/*****************************************************************************/
void ACTUALIZAR_ESTADO_EVENTOS( void )
/*****************************************************************************/
{
    struct _eventos eventos;
    struct _ev_cont ev_cont;
    struct _ev_medios ev_pago;
    struct _ev_despro ev_despro;
    struct _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
    struct _ind_evdpago_6 ind;
    struct _ev_bpromo ev_b_promo;
    unsigned long registro;
    int area_ant = SELECTED();
    if( ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago, &ev_despro, &ev_dpago,
                                  &ev_cod_barra, &ev_b_promo ) ) {
        //marco todos los datos adicionales como reversados
        SELECT( AREA_DPAGO );
        GO( TOP );
        SET_ORDER( 1 ); //id_evento
        GET_GREATER_E( ( char* )&eventos.id_evento );
        while( !BtrvEOF() && event_dpago->id_evento == eventos.id_evento ) {
            event_dpago->anulado = 3;
            #ifdef INVEL_L
            BEGIN_TRANSACTION();
            #endif
            UPDATE();
            #ifdef INVEL_L
            END_TRANSACTION();
            #endif
            SKIP( 1 );
        }
        //busco los cheques en la cliecheq y los elimino
        SELECT( AREA_AUX5 ); //ev_pago
        GO( TOP );
        SET_ORDER( 2 ); //id_evento+posicion
        while( !BtrvEOF() ) {
            char modo = ( char )( ev_pago.modo_pago - 1 );
            if( medios[modo].cheque ) {
                //si se pag� con cheques, busco en la dpago para eliminarlos a todos
                UINT16 cod_banco;
                char nro_cheque_str[15];
                int ok = 0;
                memset( nro_cheque_str, 0, 15 );
                SELECT( AREA_AUX8 ); //ev_dpago
                GO( TOP );
                SET_ORDER( 1 );//id_evento
                while( !BtrvEOF() && ev_dpago.id_evento == ev_pago.id_evento ) {
                    if( ev_dpago.indice_posicion == ev_pago.posicion ) {
                        ok = 0;
                        cod_banco = 0;
                        memset( nro_cheque_str, 0, 15 );
                        if( ev_dpago.nro_orden == 2 ) {
                            //nro_cheque
                            strncpy( nro_cheque_str, ev_dpago.valor, strlen( ev_dpago.valor ) );                         
                            //BUSCO EL BANCO
                            registro = GET_POSITION();
                            ind.id_evento = ev_dpago.id_evento;
                            ind.cod_solicitud = medios[modo].cod_solicitud;
                            ind.nro_orden = 3; //banco
                            ind.pos_pago = ev_dpago.pos_pago;
                            SELECT( AREA_AUX8 ); //ev_dpago
                            SET_ORDER( 6 );
                            GET_EQUAL( ( char* )&ind );
                            if( FOUND() ) {
                                cod_banco = atoi( ev_dpago.valor );
                                ok = 1;
                            }
                            GO( TOP );
                            SET_ORDER( 2 );//id_evento+nro_orden
                            GET_DIRECT( registro );
                        }                    
                        if( ok ) {
                            char cad[50];
                            sprintf( cad, "VA A ELIMINAR: idev:%li, bco:%d, chq:%s",
                                     ev_dpago.id_evento, cod_banco, nro_cheque_str );
                            GRABAR_LOG_SISTEMA( cad ,LOG_DEBUG,4);
                            if( ELIMINA_CHEQUE( 0, cod_banco, nro_cheque_str ) ) {
                                GRABAR_LOG_SISTEMA( "ELIMINAR: OK",LOG_DEBUG,4 );
                            }
                            else {
                                GRABAR_LOG_SISTEMA( "ELIMINAR: ERROR",LOG_DEBUG,4 );
                            }
                        }
                    }
                    SELECT( AREA_AUX8 ); //ev_dpago
                    SKIP( 1 );
                }
            }
            SELECT( AREA_AUX5 ); //ev_pago
            SKIP( 1 );
        }
    }
    CERRAR_EVENTOS_TEMPORALES();
    SELECT( area_ant );
}
/*****************************************************************************/
void CEREAR_TEMPORALES( void )
/*****************************************************************************/
{
    struct _eventos eventos;
    struct _ev_cont ev_cont;
    struct _ev_medios ev_pago;
    //struct _ev_despro ev_despro;
    struct _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
	struct _ev_bpromo ev_b_promo;

    int error = 0;
    int area_ant = SELECTED2();
	int tipo_ant = SELECTED_TIPO();
    /*error = USE_DATABASE( AREA_AUX3, EV_TMP, ( char* )&eventos, sizeof( struct _eventos ),
                          NULL, 0, 0 );*/
    error = OPEN_TABLE( T_EV_TMP, TT_ORIG, ( char* )&eventos, sizeof( struct _eventos ) );
    if( !error ) {
        #ifdef INVEL_L
        #else
        ZAP2();
        #endif
    }
    error = OPEN_TABLE( T_EVC_TMP, TT_ORIG, ( char* )&ev_cont, sizeof( struct _ev_cont ) );
    if( !error ) {
        #ifdef INVEL_L
        #else
        ZAP2();
        #endif
    }
    error = OPEN_TABLE( T_EVM_TMP, TT_ORIG, ( char* )&ev_pago, sizeof( struct _ev_medios ) );
    if( !error ) {
        #ifdef INVEL_L
        #else
        ZAP2();
        #endif
    }
    #ifdef COMPILAR_EV_DESPRO
    error = OPEN_TABLE( T_EVD_TMP, TT_ORIG, ( char* )&ev_despro, sizeof( struct _ev_despro ) );
    if( !error ) {
        #ifdef INVEL_L
        #else
        ZAP2();
        #endif
    }
    #endif
    error = OPEN_TABLE( T_EVP_TMP, TT_ORIG, ( char* )&ev_dpago, sizeof( struct _ev_dmedios ) );
    if( !error ) {
        #ifdef INVEL_L
        #else
        ZAP2();
        #endif
    }
    error = OPEN_TABLE( T_EVCB_TMP, TT_ORIG, ( char* )&ev_cod_barra, sizeof( struct _ev_cod_barra ) );
    if( !error ) {
        #ifdef INVEL_L
        #else
        ZAP2();
        #endif
    }
	error = OPEN_TABLE( T_EVBP_TMP, TT_ORIG, ( char* )&ev_b_promo, sizeof( struct _ev_bpromo ) );
    if( !error ) {
        #ifdef INVEL_L
        #else
        ZAP2();
        #endif
    }

    CERRAR_EVENTOS_TEMPORALES();
    SELECT_TABLE( area_ant, tipo_ant );
}
/******************************************************************************/
void FTOC_2( double numero, char *cadena, int enteros, int decimales, int signo, int comas )
/******************************************************************************/
{
    int fin = 0, signo2 = 0;
    FTOA_2( numero, cadena, enteros, decimales, signo, comas, &signo2 );
    fin = enteros + decimales + signo2;
    if( decimales ) {
        fin++;
    }
    if( signo2 ) {
        fin++;
    }
    if( comas ) {
        fin += ( enteros - 1 ) / 3;
    }
    cadena[fin] = 0;
}
/******************************************************************************/
void FTOA_2( double importe, char *cadena, int enteros, int decimales, int signo, int comas,
             int *signo_ )
/******************************************************************************/
{
    char *cadena2;
    int digitos = 0, signo2 = 0, l_cadena, l_cadena2, h, punto = 0;
    /*--------------------- convierte a cadena --------------------*/
    importe = ROUND( importe, enteros, decimales );
    cadena2 = _fcvt( importe, decimales, &digitos, &signo2 );
    l_cadena2 = strlen( cadena2 );
    /*------------- pasa a cadena final y rellena con ceros -----------------*/
    if( decimales ) {
        punto = 1;
    }
    l_cadena = enteros + decimales + punto + signo2;
    if( comas ) {
        l_cadena += ( enteros - 1 ) / 3;
    }
    if( l_cadena2 <= enteros + decimales ) {
        memcpy( &cadena[l_cadena - l_cadena2], cadena2, l_cadena2 );
        cadena[l_cadena] = 0;
        memset( cadena, '0', l_cadena - l_cadena2 );
        /*------------------------ coloca punto decimal -------------------------*/
        if( decimales ) {
            memmove( cadena, &cadena[1], l_cadena - decimales );
            cadena[l_cadena - decimales - 1] = ',';
        }
        /*-------------------------- rellena con blancos ------------------------*/
        if( digitos < 1 ) {
            digitos = 1;
        }
        memset( cadena, ' ', l_cadena - decimales - punto - digitos );
        /*--------------------------- coloca comas ------------------------------*/
        if( comas ) {
            for( h = 1;h < 4;h++ ) {
                if( digitos > h * 3 ) {
                    //movmem( &cadena[1], cadena,
                    // l_cadena - decimales - 1 - punto - ( h * 3 + ( h - 1 ) ) );
                    //memmove( &cadena[1], cadena,
                    //  l_cadena - decimales - 1 - punto - ( h * 3 + ( h - 1 ) ) );
                    memmove( cadena, &cadena[1],
                             l_cadena - decimales - 1 - punto - ( h * 3 + ( h - 1 ) ) );
                    cadena[l_cadena - decimales - 1 - punto - ( h * 3 + ( h - 1 ) )] = '.';
                }
            }
        }
        /*------------------------- coloca signo --------------------------------*/
        if( signo && signo2 ) {
            for( h = 0;cadena[h] == ' '; ) {
                h++;
            }
            if( h == 0 ) {
                memset( cadena, '-', l_cadena );
            }
            else {
                cadena[h - 1] = '-';
            }
        }
    }
    else {
        memset( cadena, '-', l_cadena );
    }
    *signo_ = signo2;
}
