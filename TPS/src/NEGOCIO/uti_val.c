#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <abort.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <dd.h> 
#include <get_dow.h> 
#include <log.h> 
#include <mensaje.h> 
#include <print.h> 
#include <ini.h>
//#include <uti-comp.h> 
#include <uti-val.h> 
#include <_cr1.h> 
#include <cmemory.h>
#include <malloc.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/*Todo lo referido a val caja tiende a desaparecer, no agregar nada mas.. usar tkt*/
#include <string.h>
#include <path.h>
extern float _tomar_float( char **cadena );
extern int _tomar_entero( char **cadena );
extern char SI_DESCUENTO_TARJETA;
extern struct _ConfTkt *CONFIG_VALIDA;
extern void _tomar_cadena( char **cadena );
extern float _tomar_float( char **cadena );
extern int _tomar_entero( char **cadena );
extern void _tomar_cadena_dos( char **cadena, char **cobtenida );
extern void TRANSFORMAR_CADENA_HEXA( char *cadena, int largo );
extern unsigned *TABLA_VALIDA;
extern UINT16 _COD_VARIABLE_RNV,_SKIP_BLOCK;
extern INT16 _SUB_VARIABLE_RNV;
extern UINT16 _VALOR_RNV;
extern char _COMPARACION_IGUAL;
static int ind_config;
static char *campos[] = {"FECHA","HORA","IMPORTE","NUMERO CLIENTE","NOMBRE MEDIO","CAJA","CAJERO","SUCURSAL",
"CUENTA","COMPROBANTE","NOMBRE COMERCIO","FECHA CHEQUE","ANIO","TELEFONO","CUIT",
"NOMBRE CLIENTE","NUMERO TICKET","IMPORTE CUPON ENVASES","DIA","MES","NOMBRE MES",
"NUMERO EVENTO","NUMERO TARJETA","CUENTA CMR","NUMERO SECUENCIA","PUNTAJE VENTA",
"NUMERO CLIENTE AFINIDAD","PUNTAJE VENTA TRUNCADO","NOMBRE CLIENTE AFINIDAD",
"FECHA ACTUALIZACION","PUNTAJE ACUMULADO","SALDO CHEQUE CLIENTE","NOMBRE SERVICIO",
"NUMERO","AUTORIZACION","NRO BANCO","NOMBRE BANCO","MARCA TARJETA","NOMBRE CLIENTE TRACK I",
"INSTITUCION","PUNTOS CLIENTE","ANIO CUATRO DIGITOS","FECHA CUATRO DIGITOS","COD AUTORIZACION",
"NOMBRE CLIENTE AFI ADICIONAL","NUMERO CLIENTE AFI ADICIONAL","CODIGO TOMRA","DESCRIPCION TOMRA",
"CANTIDAD TOMRA","TELEFONO COMPRADOR","CUIT CLIENTE VERIFICADOR","TIPO TOMRA","COD BOLETA PLATA",
"CALCULAR COD BOLPLATA","FIN PRINT BUFFER","BEGIN","END","GRUPO CUOTA","IMPORTE SIN VUELTO","VTO TARJETA",
"NOMBRE CAJERO","CODIGO BARRA FOLIO","CODIGO FOLIO","CODIGO BARRA","DATO ADICIONAL COBRO","CUERPO MEDIO",
"NOMBRE COMPROBANTE","COSTO SERVICIO","CUENTA LARGA","CONFIRMACION","HORA LARGA","FOLIO PAGATODO",
"NIP PAGATODO","CANTIDAD","STRING PROMO","LLAVERO","VIGENCIA","VIGENCIA EN FECHA","IMP EXTRAER CASH BACK",
"VENTA","SALDO TARJETA","DATO ADICCIONAL NRO TARJETA","DATO ADICCIONAL AUTORIZACION",
"DATO ADICCIONAL FECHA VENCIMIENTO","DATO ADICCIONAL NRO CUENTA","MES ANIO VENCIMIENTO",
"CUOTA TARJETA","MENSAJE2","ID TRANSAC","IMPORTE MEDIO","IMPORTE DESCUENTO TARJETA",
"IMPORTE ORIGINAL MEDIO","SUPERVISOR","NOMBRE SUPERVISOR","ABRIR DOC NO FISCAL","CERRAR DOC NO FISCAL",
"LETRA FACTURA","TIPO COMPROBANTE","NUMERO FACTURA","NRO ORDEN FACTURA","X_FECHA","X_HORA","NOMBRE SUCURSAL",
"X_NOMBRE CLIENTE","DOMICILIO CLIENTE","LOCALIDAD CLIENTE","PROVINCIA CLIENTE","TIPO IVA CLIENTE","CUIT CLIENTE",
"CONDICION VENTA","CODIGO BARRA 12","X_CODIGO BARRA","CODIGO ARTICULO","DESC. ARTICULO","IVA ARTICULO","PRECIO ARTICULO",
"IMPORTE ARTICULO","SUBTOTAL","NETO GRAVADO","EXENTO","IVA1","IVA2","TOTAL","LUGAR DE EMISION","CANTIDAD ARTICULO",
"CONTENIDO ARTICULO","UNIDAD ARTICULO","IMPUESTO INTERNO","X_NUMERO CLIENTE","CANTIDAD SIN DECIMALES","TASA IVA1",
"TASA IVA2","ILA","IHA","GIRO CLIENTE","OC/GD CLIENTE","CANTIDAD PRESENTACION","CODIGO PRESENTACION",
"SUCURSAL REMITO A FACTURAR","NUMERO REMITO A FACTURAR","PRECIO FINAL","NUMERO VENDEDOR","NUMERO CAJA",
"NUMERO CAJERO","RETENCION DGR","PERCEPCION DGI","NUMERO BOLETA ANTERIOR","X_NUMERO EVENTO","X_NUMERO TARJETA",
"MENSAJE TARJETA","VIGENCIA TARJETA","FECHA PRIMER VTO TARJETA","CANTIDAD CUOTAS TARJETA","VALIDACION TARJETA",
"NC DATOS CAJA","NC DATOS SUCURSAL","NC DATOS COMPROBANTE","NC DATOS FECHA","NC DATOS TIPO COMP",
"PERCEP 212 IVA1","PERCEP 212 IVA2","NRO TICKET","MARCA","X_NOMBRE CAJERO","SECUENCIA","NUMERO Z",
"X_NOMBRE MEDIO","BASE IMPONIBLE","COD. IVA","BULTOS","EAN CLIENTE AFINIDAD","TRANSACCION","CAJA ANTERIOR",
"FECHA ANTERIOR","X_PUNTAJE VENTA","X_NUMERO CLIENTE AFINIDAD","X_PUNTAJE VENTA TRUNCADO","X_NOMBRE CLIENTE AFINIDAD",
"X_FECHA ACTUALIZACION","X_PUNTAJE ACUMULADO","DIFERENCIA AFINIDAD","CANT. ARTICULOS AFINIDAD","NUMERO DOCUMENTO",
"COTIZACION","IMPORTE DONACION","NRO SERIE","X_PUNTOS CLIENTE","BUSCA CLIENTE","DISCRIMINACION IVA",
"X_ANIO CUATRO DIGITOS","X_FECHA CUATRO DIGITOS","TOTAL IMPUESTO FACTURA","TOTAL PROMOCION","SI TOTAL PROMOCION",
"X_BEGIN","X_END","COMIENZO CUERPO SUBTOTAL","FIN CUERPO SUBTOTAL","CODIGO CLIENTE GENERICO","DNI CLIENTE",
"CAN TOTAL TMP","IMPORTE TMP","CANTIDAD TMP","DESC TMP","WHILE TMP","END WHILE TMP","NRI CLIENTE",
"DATO SUBMEDIO TAR","TOTAL IMPUESTOS","LETRA IMPUESTO","NRO AUTO TARJETA","IMPORTE INGRESADO",
"X_SALDO TARJETA","X_MES ANIO VENCIMIENTO","DETALLE SUBMEDIOS","CODIGO BARRA COBROS","CODIGO BARRA ARTICULO COBROS",
"LETRA NEGRITA","DESHABILITAR FORMATOS LETRAS","NC DATOS AUTORIZACION","X_COD AUTORIZACION","TICKET FINAL",
"TRANSACCION FINAL","SECUENCIA ANTERIOR","NUMERO DE X","TIPO TARJETA","CLIENTE","VENCIMIENTO",
"NUMERO ESTABLECIMIENTO","NUMERO CUPON","COMPROBANTE NUMERO","CODIGO AUTORIZACION","OPERACION",
"IMPORTE TARJETA","NUMERO CUOTAS","MONEDA","NUMERO TERMINAL","NUMERO LOTE","NUMERO TPV",
"NUMERO SUCURSAL","OPERADOR","TIQUE REFERENCIA","FIRMA","ACLARACION","PHONE",
"IMPRIME VOUCHER FISCAL","X_ABRIR DOC NO FISCAL","X_CERRAR DOC NO FISCAL","X_SUPERVISOR",
"X_NOMBRE SUPERVISOR","SI REIMPRESION","CUIT ESTABLECIMIENTO","NRO ESTABLECIMIENTO","NRO CUPON",
"NRO COMPROBANTE","TARJETA TABLA","CUOTA COEFICIENTE","FECHA ORIGINAL","CUPON ORIGINAL","NRO CUENTA TARJETA",NULL} ;


static char *funciones[] = {"POSICION","POS X","POS Y","CADENA","VALIDACION",
	"FIN VALIDACION","LETRA NORMAL","FIN INICIALIZACION","SLIP ON","SLIP OFF",
	"MENSAJE SIN ESPERA","MENSAJE CON ESPERA","NRO IMPRESION CHEQUE","MENSAJE ESPECIAL","ESPERAR ENTER",
	"IMPORTE EN LETRAS","CORTE RECEIPT","PORCENTAJE DEL TOTAL","PORCENTAJE TRUNCADO DEL TOTAL","LETRA GRANDE",
	"CABECERA TICKET","CABECERA CAJERO","PIE TICKET","NRO FACTURA","VARIABLE RNV","NETO GRAVADO PROP",
	"IVA PROP",	"EXENTO PROP","SI VARIABLE RNV","SI NO VARIABLE RNV","NRO DOC FISCAL","NRO COM RETIRO",
	"NRO COM FONDO FIJO","NRO COM INVENTARIO","MENSAJE CUMPLEANIO","LF RECEIPT","COMIENZO ENV TOMRA",
	"FIN ENV TOMRA","SELECT","INSERTAR BOLETIN","DATO MEDIO","DIA DATO MEDIO","MES DATO MEDIO",
	"ANIO DATO MEDIO","PRINT A BUFFER","PRINT COL BUFFER","ENVIAR BMP","DATO ADICIONAL DATA ENTRY",
	"PEDIR DATO ADICIONAL","PROCESAR DA MEDIO PAGO","IMPRIMIR DATO ADICIONAL","FOR","END FOR","LF",
	"SI FORMULA","FORMULA","SI MEDIO","CONSULTA TABLA","LINEA MENSAJE","SETEA TOTAL","IMAGEN","COMIENZO SI",
	"FIN SI","SI DEVOLUCION","INTERLINEADO","CARACTERES POR LINEA","SI HAY MEDIO","SI DESCUENTO TARJETA",
	"SI NO DESCUENTO TARJETA","CADENA NO FISCAL","CONSULTA TABLA SQL","LARGO FACTURA","RENGLONES CUERPO",
	"x_POSICION","x_POS X","x_POS Y","EJECT","x_CADENA","COMIENZO CUERPO","FIN CUERPO","x_LETRA NORMAL",
	"LETRA DOBLE ANCHO","x_FIN INICIALIZACION","FACTURAS INDEPENDIENTES","CADENA CONTINUA EN PROXIMA",
	"AGREGAR IVA NO INSCRIPTO","SI FACTURA A","CADENA REEMPLAZO TICKET","SI FACTURA B","LARGO FACTURA A",
	"LARGO FACTURA B","RENGLONES CUERPO A","RENGLONES CUERPO B","LARGO CADENA","TOTAL EN LETRAS","REIMPRESION",
	"TOTAL IMPUESTO","LARGO REMITO","COMIENZO CUERPO FACTURA DE REMITO","TOTAL HOJAS COMPROBANTE","HOJA ACTUAL",
	"PEDIR FORMULARIO CON TECLA","NETO GRAVADO COD","IVA1 COD","IVA2 COD","SI NOTA CREDITO","SI BIEN DE USO",
	"COMIENZO CUERPO CANTIDAD","FIN CUERPO CANTIDAD","COMIENZO CUERPO UNITARIO","FIN CUERPO UNITARIO",
	"PRECIO UNITARIO","PRECIO TOTAL","TOTAL TICKET","VUELTO","CORTE TICKET","COMIENZO CUERPO TOTAL",
	"FIN CUERPO TOTAL","COMIENZO CUERPO MEDIOS","FIN CUERPO MEDIOS","COMIENZO CUERPO IVA","FIN CUERPO IVA",
	"IMPORTE MEDIO","x_SELECT","COMIENZO PIE","FIN PIE","COMIENZO CUERPO VUELTO","FIN CUERPO VUELTO",
	"FIN PARTE FIJA","COMIENZO CUERPO AFINIDAD","FIN CUERPO AFINIDAD","SI MODO ENTRENAMIENTO",
	"SI REIMPRESION","SI ANULACION","SI MODO INVENTARIO","x_SI DEVOLUCION","COMIENZO CUERPO NO AFINIDAD",
	"FIN CUERPO NO AFINIDAD","x_SI MEDIO","IMPORTE MEDIO ME","COMIENZO CUERPO DONACION","FIN CUERPO DONACION",
	"x_DATO MEDIO","PRECIO SIN IVA EN TICKET","x_SI VARIABLE RNV","x_SI NO VARIABLE RNV","COMIENZO CUERPO ID CIENTE",
	"FIN CUERPO ID CLIENTE","COMIENZO CUERPO ID SIN CLIENTE","FIN CUERPO ID SIN CLIENTE","OFFSET CADENA","x_VARIABLE RNV",
	"x_SLIP ON","x_SLIP OFF","CABECERA FISCAL","CABECERA FISCAL 2","PIE FISCAL","PIE FISCAL 2","CADENA FISCAL",
	"ITEM FISCAL","x_CADENA NO FISCAL","COMIENZO CUERPO FONDO FIJO","FISCAL Z","FIN CUERPO FONDO FIJO",
	"INICIO INFORME DIFERENCIA","FIN INFORME DIFERENCIA","COMANDO FISCAL","x_LF RECEIPT","MAX LIN COMENTARIOS",
	"CARACTERES X LIN","INICIO CORTE PAPEL","FIN CORTE PAPEL","RESET","TICKS B","NRO DOC FISCAL COMP","DATO ARTICULO",
	"SI DATO ARTICULO","MAXIMO RENGLONES TRANSFERENCIA","OFFSET COLUMNA","NRO TICKET CONFIG","TOTAL CONFIG",
	"NRO CAJA CONFIG","PAUSA","x_FORMULA","x_SI FORMULA","x_FOR","x_END FOR","x_LF","SI EXENTO","SI GRAVADO",
	"COMIENZO INF00","FIN INF00","VARIABLE RNV OFF","SI VARIABLE RNV OFF","SI NO VARIABLE RNV OFF","DESC MEDIO",
	"TOTALIZAR TMP","SI VARIABLE RNV Y SI MEDIO","SI NO VARIABLE RNV Y SI MEDIO","PROCESAR DATOS ADICIONALES",
	"DATO ADICIONAL MEDIO PAGO","DATOS ADICIONALES","CONFIRMAR IMPRESION","x_CONSULTA TABLA","COMIENZO INF TAR",
	"FIN INF TAR","x_ENVIAR BMP","COMIENZO TIMBRADORA","FIN TIMBRADORA","SET FONT","INICIO INFORME SUPERVISOR",
	"FIN INFORME SUPERVISOR","INICIO CABECERA INFORME SUPERVISOR","FIN CABCERA INFORME SUPERVISOR","x_COMIENZO SI",
	"x_FIN SI","VERSION","FECHA ALINEADA","COMIENZO PAGO REINTEGRO","FIN PAGO REINTEGRO","COMIENZO TIMBRADO REINTEGRO",
	"FIN TIMBRADO REINTEGRO","ULTIMO ID","x_CONSULTA TABLA SQL","INICIO INFORME DIFERENCIA SUBMEDIOS",
	"FIN INFORME DIFERENCIA SUBMEDIOS","INICIO CABECERA INFORME DIFERENCIA SUBMEDIOS",
	"FIN CABECERA INFORME DIFERENCIA SUBMEDIOS","CANTIDAD EVENTOS","ID EVENTO NRO","SI VALOR MED","DATO MEDIO SUBMEDIO",
	"CODIGO BARRA FOLIO","x_IMAGEN","COMIENZO DESCRIPCION CUERPO","FIN DESCRIPCION CUERPO","COMIENZO DESCRIPCION TARJETA",
	"FIN DESCRIPCION TARJETA","CODIGO FOLIO","COMIENZO IMPORTE TOTAL",
	"FIN IMPORTE TOTAL","COMIENZO FRANQUEO","FIN FRANQUEO","x_MENSAJE ESPECIAL","x_ESPERAR ENTER",
	"COMIENZO TICKET EN ESPERA","FIN TICKET EN ESPERA","IMP TOTAL TMP",
	"COMIENZO CUERPO INFCOMPARATIVA","FIN CUERPO INFCOMPARATIVA",
	"COMIENZO FIN TICKET","FIN TICKET","COMIENZO FRANQUEO COBROS","FIN FRANQUEO COBROS",
	"x_INTERLINEADO","x_CARACTERES POR LINEA","REPETIR CADENA","NO DEVOLUCION",
	"SI CIERRE Z","SI CIERRE X","FIN CIERRE Z","FIN CIERRE X","COMIENZO DUPLICADO TARJETA",
	"FIN DUPLICADO TARJETA","IMPORTE MEDIO FF","COMIENZO CUERPO RETIRO","FIN CUERPO RETIRO",
	"IMPORTE DE RETIROS","TOTAL DE RETIROS","TOTAL FF","SI MEDIO SALIENTE","COMIENZO CUERPO ING CAJERO",
	"FIN CUERPO ING CAJERO","LETRA RESALTADA","LETRA DOBLE ALTO","LETRA DOBLE ALTO NEGRITA",
	"LETRA DOBLE ANCHO NEGRITA","LETRA DOBLE ANCHO ALTO","LETRA DOBLE ANCHO ALTO NEGRITA","LETRA SUBRAYADO",
	"LETRA SUBRAYADO NEGRITA","CAMPOS CLIENTES","CANTIDAD DE DATOS","COMIENZO CONFIGURACION DATOS CLIENTE",
	"FIN CONFIGURACION DATOS CLIENTE","CANTIDAD MASCARA","IMPORTE EXTRACCION","IMPORTE TOTALCASHPLUS",NULL};

void VAL_agregar_parametros_funcion( int funcion, int *ind_config, char *cadena,
                                  unsigned char *CONFIG_VALIDA, int ind_config_pos );
void VAL_agregar_enteros( int *ind_config, char *cadena, int cant, unsigned char *CONFIG_VALIDA );
void VAL_agregar_float( int *ind_config, char *cadena, int cant, unsigned char *CONFIG_VALIDA );

/*****************************************************************************/
int CARGAR_CONF_VALIDA()
/*****************************************************************************/
{
    return ( CARGAR_CONFIG_VALIDA( VAL_CAJA_CFG, &CONFIG_VALIDA, BUFFER_VALIDACIONES/*BUFFER_TICKET*/ ) );
}

/*****************************************************************************/
int CARGAR_CONFIG_VALIDA( char *nom_arch, struct _ConfTkt *config[], int largo )
/*****************************************************************************/
{
    char buffer[300],*pos,*pos2, *inicio_func, tipo_error[30];
    FILE *archivo;
    int rta = 1, var, h, ind_configTmp = 0;
    UINT16 linea = 0;
    int comentario,i;

    /*---------------- Pedimos memoria para validaciones ----------------*/

    CONFIG_VALIDA = ( struct _ConfTkt * )MALLOC( largo * sizeof( struct _ConfTkt ) );
    TABLA_VALIDA = ( unsigned * )calloc( CANTIDAD_VALIDACIONES, sizeof( int ) );
    /*--------------- Abrimos el archivo de configuracion ---------------*/
    #ifdef INVEL_L
    archivo = fopen( VAL_CAJA_CFG, "rt" );
    #else
    archivo = fopen( VAL_CAJA_CFG, "rt" );
    #endif

    if( archivo && CONFIG_VALIDA && TABLA_VALIDA ) {
        /*--------- Inicializaciones -------*/
        memset( TABLA_VALIDA, 0, CANTIDAD_VALIDACIONES );
        /*-------------- Leemos un rengln y los procesamos -----------*/
        ind_config = 0;
        while( fgets( buffer, sizeof( buffer ) - 1, archivo ) && rta ) {
            linea++;
			memset(tipo_error,0,sizeof(tipo_error));
			if( buffer[0] && buffer[0] != 10 && buffer[0] != 13 ) {
                if( ind_config < largo ) {
                    // controla que la linea no sea un comentario
                    comentario = 0;
                    inicio_func = strchr( buffer, '[' );
                    if( inicio_func != NULL ) {
                        for( i = 0;i < inicio_func - buffer + 1;i++ ) {
                            if( buffer[i] == ';' ) {
                                comentario = 1;
                                break;
                            }
                        }
                    }
                    else {
                        comentario = 1;
                    }
                    if( !comentario ) {
                        pos = strchr( buffer, '\n' );
                        if( pos ) {
                            *pos = 0;
                            pos = strchr( buffer, '[' );
                            pos2 = strchr( pos, ']' );
                            if( pos && pos2 ) {
                                *pos2 = 0;
                                _strupr( buffer );
                                /*---------- Verificamos si es un campo  ------*/
                                for( h = 0, var = -1;campos[h] && var < 0;h++ ) {
                                    if( strcmp( campos[h], &pos[1] ) == 0 ) {
                                        var = h;
                                        break;
                                    }
                                }
                                if( var >= 0 ) {
                                    CONFIG_VALIDA[ind_config++].codigo = var + 1;
                                    CONFIG_VALIDA[ind_config].linea = linea;
                                }
                                else {
                                    /*--------- Verificamos si es una funcion ---------*/
                                    for( h = 0, var = -1;funciones[h] && var < 0;h++ ) {
                                        if( strcmp( funciones[h], &pos[1] ) == 0 ) {
                                            var = h;
                                            break;
                                        }
                                    }
                                    if( var >= 0 ) {
                                        long cantTmp = strlen( &pos2[1] );
                                        
                                        CONFIG_VALIDA[ind_config].codigo = var + 1 + _VAL_FIN_CAMPOS ;
                                        ind_configTmp = 0;
                                        CONFIG_VALIDA[ind_config].parametros = ( char* )malloc( cantTmp );
                                        VAL_agregar_parametros_funcion( var + 1, 
                                                                        &ind_configTmp, 
                                                                        &pos2[1], 
                                                                        CONFIG_VALIDA[ind_config].parametros, ind_config );
                                        CONFIG_VALIDA[ind_config++].linea = linea;
                                    }
                                    else {
										sprintf( tipo_error, "No es Funcion o Variable.");
                                        rta = 0;
                                    }
                                }
                            }
                            else {
								sprintf( tipo_error, "en Coordinacion de []");
                                rta = 0;
                            }
                        }
                        else {
                            rta = 0;
							sprintf( tipo_error, "en Final de Cadena");
                        }
                    }
                }
                else {
                    ABORT( "Overflow en buffer de validaciones." );
                }
            }
        }
        fclose( archivo );
        CONFIG_VALIDA[ind_config].codigo = _VAL_FIN;
        if( !rta ) {
			GRABAR_LOG_SISTEMA( buffer ,LOG_VENTAS,2);
			sprintf( buffer, "Error:%s", tipo_error );
            GRABAR_LOG_SISTEMA( buffer ,LOG_VENTAS,1);
			sprintf( buffer, "%s: %i Error:%s", ST( S_ERROR_EN_LINEA ), linea ,tipo_error);
            GRABAR_LOG_SISTEMA( buffer ,LOG_VENTAS,1);
            MENSAJE( buffer );
			
        }
    }
    else {
        rta = 0;
    }
    return ( rta );
}
/*****************************************************************************/
void VAL_agregar_parametros_funcion( int funcion, int *ind_config, char *cadena,
                                            unsigned char *CONFIG_VALIDA, int ind_config_pos )
/*****************************************************************************/
{
    char *p_cadena,*cadfinal;
    int valor,largo,largo_ant;

    p_cadena = cadena;
    switch( funcion + _VAL_FIN_CAMPOS ) {
        case _POS_X:
        case _POS_Y:
        case _IMPRESION_CHEQUE:
        case _REIMPRESION_FACTURA:
        case _NETO_GRAVADO_PROPORCIONAL:
        case _IVA_PROPORCIONAL:
        case _NRO_DOC_FISCAL:
        case _NRO_COM_RETIRO:
        case _NRO_COM_FONDO_FIJO:
        case _NRO_COM_INVENTARIO:
        case _VAL_SELECT:
        case _LF_RECEIPT:
        case _VALIDA_DATO_MEDIO:
        case _VALIDA_DIA_DATO_MEDIO:
        case _VALIDA_MES_DATO_MEDIO:
        case _VALIDA_ANIO_DATO_MEDIO:
        case _DATO_ADICIONAL_D_ENTRY:
        case _PEDIR_DATO_ADICIONAL:
        case _IMPRIMIR_DATO_ADICIONAL:
        case _VALIDA_LF:
        case _VALIDA_END_FOR:
        case _LARGO_FACTURA:
        case _LARGO_REMITO:
        case _LARGO_FACTURA_A:
        case _LARGO_FACTURA_B:
        case _RENGLONES_CUERPO:
        case _RENGLONES_CUERPO_A:
        case _RENGLONES_CUERPO_B:
        case _LARGO_CADENA:
        case _POS_X_2:
        case _POS_Y_2:
        case _TOTAL_IMPUESTO:
        case _NETO_GRAVADO_COD:
        case _IVA1_COD:
        case _IVA2_COD:
        case _SELECT:
        //case _SI_MEDIO://DEL FAC-CAJA
        case _TKT_DATO_MEDIO:
        case _OFFSET_CADENA:
        case _MAXIMO_RENGLONES_TRANSFERENCIA:
        case _TKT_DATO_ARTICULO:
        case _SI_DATO_ARTICULO:
        case _TKT_OFFSET_COLUMNA:
        case _TKT_LF_RECEIPT:
        case _TKT_MAX_LIN_COMENTARIOS:
        case _TKT_CARACTERES_X_LIN:
        case _TKT_TICKS_B:
        case _NRO_DOC_FISCAL_COMP:
        case _TKT_PAUSA:
        case _TKT_END_FOR:
        case _TKT_LF:
        case _TKT_DESC_MEDIO:
        case _DATOS_ADICIONALES_COMPROB:
        case _TKT_IMPORTE_TOTAL_TMP:
        case _TKT_INTERLINEADO:
        case _TKT_CARACTERES_LINEA:
        case _CANT_DATOS_SOLICITAR:
        case _IMPORTE_MEDIO_FF:
		  case _LINEA_MENSAJE:
        case _VAL_INTERLINEADO:
        case _VAL_CARACTERES_LINEA:
            VAL_agregar_enteros( ind_config, cadena, 1, CONFIG_VALIDA);
            break;
        case _VALIDA_FOR:
            //--- Primera cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Segunda cadena
            _tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_VALIDA[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
            
        case _TKT_SI_FORMULA:
        //case _CADENA://DEL COMTPROB POR FAC CAJA
		  case _TKT_MENSAJE_ESPECIAL:
        case _CADENA_REEMPLAZO_TICKET:
        case _TKT_NRO_CAJA:
        case _TKT_NRO_TICKET:
        case _TKT_TOTAL:
        case _CABECERA_FISCAL_2:
        case _PIE_FISCAL_2:
        case _CADENA_FISCAL:
        case _CADENA_NO_FISCAL:
        case _COMIENZO_SI:
        //case _IMAGEN:    //DEL COMTPROB POR FAC CAJA
        case _VALIDA_SI_FORMULA:
            _tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_VALIDA[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
        case _VALIDA_CONSULTA_TABLA:
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            //el break falta intencionalmente
		case _VAL_CONSULTA_TABLA_SQL:
        case _VALIDA_FORMULA:
            //--- Primera cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Segunda cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Tercer cadena
            _tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_VALIDA[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
        case _POSICION://esta duplicado pero buscar el nuevo donde esta
            //case _IMPORTE_EN_LETRAS:
        case _VALIDA_REDIRECCIONAR_A_BUFFER:
            //case _VARIABLE_RNV:
            //case _SI_VALOR_VARIABLE_RNV:
            //case _SI_NO_VALOR_VARIABLE_RNV:
        case _VALIDA_IMPRIMIR_COLUMNAS_BUFFER:
            VAL_agregar_enteros( ind_config, cadena, 2, CONFIG_VALIDA);
            break;
            
        case _TKT_VERSION:    
        
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;

        case _SI_VALOR_VARIABLE_RNV:
        case _SI_NO_VALOR_VARIABLE_RNV:
            VAL_agregar_enteros( ind_config, cadena, 3, CONFIG_VALIDA);
            break;
        case _IMPORTE_EN_LETRAS:
            VAL_agregar_enteros( ind_config, cadena, 4, CONFIG_VALIDA);
            break;
        case _CADENA://esta en original
        case _MENSAJE_ESPECIAL:
        case _MENSAJE_CUMPLEANIO:
		case _VALIDA_COMIENZO_SI:
    	case _IMAGEN://esta en original
		case _SETEA_TOTAL:
           _tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            largo_ant = largo;
            TRANSFORMAR_CADENA_HEXA( p_cadena, largo );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_VALIDA[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
	    case _VAL_CADENA_NO_FISCAL:			
			valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            
			_tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_VALIDA[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
			break;
        case _VALIDACION:
            valor = _tomar_entero( &p_cadena );
            if( valor && ( unsigned )valor < CANTIDAD_VALIDACIONES ) {
                TABLA_VALIDA[valor - 1] = ind_config_pos;
            }
            else {
                ABORT( "Overflow en cantidad de validaciones" );
            }
            break;
        case _SI_MEDIO://esta en original
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            break;
        case _PORCENTAJE_DEL_TOTAL:
        case _PORCENTAJE_TRUNCADO_DEL_TOTAL:
            VAL_agregar_float( ind_config, cadena, 1, CONFIG_VALIDA);
            break;
        case _INSERTAR_BOLETIN:
            // [codigo_solicitud]
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            // [posicion]
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            // [indice_posicion]
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            // [id_dato]
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            // [boletin_consultado]
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            // [valor] o [@RAM_COD_BOLETA_PLATA]
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            // [importe] o [@RAM_ACUMULADOR[indice]]
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            // [fecha_viencia] o [@RAM_FECHA_VIGENCIA[indice]]
            _tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_VALIDA[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
        case _ENVIAR_BMP:
            _tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_VALIDA[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
		case _VAL_SI_HAY_MEDIO:
			valor = 1;
			while( valor ){
				valor = _tomar_entero( &p_cadena );
				*( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
				( *ind_config ) += 2;
			}
			break;
		case _TKT_SI_VALOR_VARIABLE_RNV:
      case _TKT_SI_NO_VALOR_VARIABLE_RNV:
      case _TKT_SI_VALOR_VARIABLE_RNV_OFF:
      case _TKT_SI_NO_VALOR_VARIABLE_RNV_OFF:
      case _TOTAL_TICKET:
      case _PROCESAR_DATOS_ADICIONALES:	
	  case _PROC_DATOS_ADIC_MEDIO_P:
			 valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
			valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
			break;
			
		//case _POSICION:DEL FAC -CAJA VER DE RESOLVER
      case _PRECIO_UNITARIO:
      case _PRECIO_TOTAL:
      case _TOTAL_EN_LETRAS:
      case _IMPORTE_MEDIO:
	  case _TKT_SALDO_TARJETA:
      case _TKT_IMPORTE_MEDIO_ME:
      case _TKT_OFF_VARIABLE_RNV:
		
      case _TKT_VARIABLE_RNV:
      		valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
      		break;	
      		
      case _TKT_TOTALIZAR_TMP:
      		valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            break;
       case _TKT_SI_VALOR_VARIABLE_RNV_Y_SI_MEDIO:
        case _TKT_SI_NO_VALOR_VARIABLE_RNV_Y_SI_MEDIO:     	
        		valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            break;
        case _TKT_ID_EVENTO_NRO:    
        		_tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
        		valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            break;
       case _TKT_FOR:
       		_tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            _tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_VALIDA[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
       case _TKT_CONSULTA_TABLA:
       		valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
       		_tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            _tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_VALIDA[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
        case _TKT_CONSULTA_TABLA_SQL:
        case _TKT_FORMULA:	
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            _tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_VALIDA[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
      case _CABECERA_FISCAL:
		case _PIE_FISCAL:     
				valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
				_tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;

		
      case _CADENA_CONTINUA_EN_PROXIMA:      
		case _VARIABLE_RNV:
      		valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            
            
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            
            
              _tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_VALIDA[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
	 break;
      case _TKT_COMANDO_FISCAL:
            _tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            largo_ant = largo;
            TRANSFORMAR_CADENA_HEXA( p_cadena, largo );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_VALIDA[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
      case _TKT_FECHA_ALINEADA:
      case _TKT_ULTIMO_ID:
        case _TKT_CANT_EVENTOS:
            // ancho del string
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;      
       

case _SET_FONT:
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
             memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            break;
        case _ENVIAR_BMP_2:
            //cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //entero 1
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            //entero 2
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            break;
        
        
        
        
        case _TKT_SI_VALOR_MED:
                       
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Segunda cadena
            
            
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //---Entero
            
            
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            break;
            
            
            
            
        case _TKT_DATO_MEDIO_SUBMEDIO:
            //--- Primera cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Segunda cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
			//---Entero
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
			//--- Tercer cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
			break;
        case _TKT_REPETIR_CADENA:
            //cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //---Entero
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            //cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
        case _CAMPOS_CLIENTES:
            //--- Primera cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Segunda cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Tercera cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_VALIDA[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Primer entero 
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            break;     
		case  _CANTIDAD2:
			_tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            largo_ant = largo;
            TRANSFORMAR_CADENA_HEXA( p_cadena, largo );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_VALIDA[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
			break;     
       		     
    }
}

/*****************************************************************************/
void VAL_agregar_enteros( int *ind_config, char *cadena, int cant, unsigned char *CONFIG_VALIDA )
/*****************************************************************************/
{
    char *p_cadena;
    int valor,i;
    p_cadena = cadena;
    for( i = 0;i < cant;i++ ) {
        valor = _tomar_entero( &p_cadena );
        *( short int* )&CONFIG_VALIDA[( *ind_config )] = valor;
        ( *ind_config ) += 2;
    }
}
/*****************************************************************************/
void VAL_agregar_float( int *ind_config, char *cadena, int cant, unsigned char *CONFIG_VALIDA )
/*****************************************************************************/
{
    char *p_cadena;
    float valor;
    int i;
    p_cadena = cadena;
    for( i = 0;i < cant;i++ ) {
        valor = _tomar_float( &p_cadena );
        *( float* )&CONFIG_VALIDA[( *ind_config )] = valor;
        ( *ind_config ) += 4;
    }
}
/***************************************************************************/
void LIBERA_UTI_VAL()
/***************************************************************************/
{
    if( CONFIG_VALIDA ) {
        free( CONFIG_VALIDA );
    }
    if( TABLA_VALIDA ) {
        free( TABLA_VALIDA );
    }
}

