#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <b_mens.h> 
#include <cadafech.h> 
#include <cr.h> 
#include <ini.h>
#include <dbrouter.h> 
#include <fiscal.h> 
#include <log.h> 
#include <malloc.h> 
#include <mensaje.h> 
#include <pant.h> 
#include <tkt.h> 
#include <uti-comp.h> 
#include <_cr0.h> 
#include <_cr1.h> 
#include <cmemory.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <string.h>
#include <bridge.h>
#ifdef INVEL_W
#include <conio.h>
#endif
#include "path.h"
#include <trim.h>
#ifdef COMPILAR_COMPROBANTES
extern struct _ConfTkt *CONFIG_FACTURA;
extern struct _ConfTkt *CONFIG_REMITO;
extern struct _ConfTkt *CONFIG_TKT1;

/*--------------- codigos de advertencias y errores -------------*/
typedef enum warning_codes { no_warning, fun_sin_param } warning_codes;
char *warning_msgs[] = {"", "Funcion sin parametro"};
static int ind_config;
static char *campos[] = {"LETRA FACTURA","TIPO COMPROBANTE","NUMERO FACTURA","NRO ORDEN FACTURA",
                         "FECHA","HORA","NOMBRE SUCURSAL","NOMBRE CLIENTE","DOMICILIO CLIENTE",
                         "LOCALIDAD CLIENTE","PROVINCIA CLIENTE","TIPO IVA CLIENTE","CUIT CLIENTE",
                         "CONDICION VENTA","CODIGO BARRA 12","CODIGO BARRA","CODIGO ARTICULO",
                         "DESC. ARTICULO","IVA ARTICULO","PRECIO ARTICULO","IMPORTE ARTICULO",
                         "SUBTOTAL","NETO GRAVADO","EXENTO","IVA1","IVA2","TOTAL",
                         "LUGAR DE EMISION","CANTIDAD ARTICULO","CONTENIDO ARTICULO",
                         "UNIDAD ARTICULO","IMPUESTO INTERNO","NUMERO CLIENTE",
                         "CANTIDAD SIN DECIMALES","TASA IVA1","TASA IVA2","ILA","IHA",
                         "GIRO CLIENTE","OC/GD CLIENTE","CANTIDAD PRESENTACION",
                         "CODIGO PRESENTACION","SUCURSAL REMITO A FACTURAR",
                         "NUMERO REMITO A FACTURAR","PRECIO FINAL","NUMERO VENDEDOR","NUMERO CAJA",
                         "NUMERO CAJERO","RETENCION DGR","PERCEPCION DGI","NUMERO BOLETA ANTERIOR",
                         "NUMERO EVENTO","NUMERO TARJETA","MENSAJE TARJETA","VIGENCIA TARJETA",
                         "FECHA PRIMER VTO TARJETA","CANTIDAD CUOTAS TARJETA","VALIDACION TARJETA",
                         "NC DATOS CAJA","NC DATOS SUCURSAL","NC DATOS COMPROBANTE",
                         "NC DATOS FECHA","NC DATOS TIPO COMP","PERCEP 212 IVA1","PERCEP 212 IVA2",
                         "NRO TICKET","MARCA","NOMBRE CAJERO","SECUENCIA","NUMERO Z","NOMBRE MEDIO",
                         "BASE IMPONIBLE","COD. IVA","BULTOS","EAN CLIENTE AFINIDAD","TRANSACCION",
                         "CAJA ANTERIOR","FECHA ANTERIOR","PUNTAJE VENTA","NUMERO CLIENTE AFINIDAD",
                         "PUNTAJE VENTA TRUNCADO","NOMBRE CLIENTE AFINIDAD","FECHA ACTUALIZACION",
                         "PUNTAJE ACUMULADO","DIFERENCIA AFINIDAD","CANT. ARTICULOS AFINIDAD",
                         "NUMERO DOCUMENTO","COTIZACION","IMPORTE DONACION","NRO SERIE",
                         "PUNTOS CLIENTE","BUSCA CLIENTE","DISCRIMINACION IVA",
                         "ANIO CUATRO DIGITOS","FECHA CUATRO DIGITOS","TOTAL IMPUESTO FACTURA",
                         "TOTAL PROMOCION","SI TOTAL PROMOCION","BEGIN","END",
                         "COMIENZO CUERPO SUBTOTAL","FIN CUERPO SUBTOTAL","CODIGO CLIENTE GENERICO",
                         "DNI CLIENTE","CAN TOTAL TMP","IMPORTE TMP","CANTIDAD TMP",
                         "DESC TMP","WHILE TMP","END WHILE TMP","NRI CLIENTE","DATO SUBMEDIO TAR",
                         /*"DATO LOTE TAR",*/"TOTAL IMPUESTOS", "LETRA IMPUESTO", "NRO AUTO TARJETA", 
						 "IMPORTE INGRESADO", "SALDO TARJETA", "MES ANIO VENCIMIENTO", "DETALLE SUBMEDIOS",
						 "CODIGO BARRA COBROS", "CODIGO BARRA ARTICULO COBROS", "LETRA NEGRITA", 
                         "DESHABILITAR FORMATOS LETRAS","NC DATOS AUTORIZACION",
						 "COD AUTORIZACION","TICKET FINAL","TRANSACCION FINAL",
						 "SECUENCIA ANTERIOR","NUMERO DE X","TIPO TARJETA","CLIENTE","VENCIMIENTO","NUMERO ESTABLECIMIENTO",
						 "NUMERO CUPON","COMPROBANTE NUMERO","CODIGO AUTORIZACION","OPERACION","IMPORTE TARJETA",
						 "NUMERO CUOTAS","MONEDA","NUMERO TERMINAL","NUMERO LOTE","NUMERO TPV","NUMERO SUCURSAL",
                         "OPERADOR","TIQUE REFERENCIA","FIRMA","ACLARACION","PHONE","IMPRIME VOUCHER FISCAL",
						 "ABRIR DOC NO FISCAL","CERRAR DOC NO FISCAL","SUPERVISOR","NOMBRE SUPERVISOR",NULL};

static char *funciones[] = {"LARGO FACTURA","RENGLONES CUERPO","POSICION","POS X","POS Y","EJECT",
                            "CADENA","COMIENZO CUERPO","FIN CUERPO","LETRA NORMAL",
                                //  "LETRA CALIDAD",
                                //  "LETRA DOBLE PASADA",
                                "LETRA DOBLE ANCHO",
                                //  "LETRA 12 CPI",
                                //  "LETRA 10 CPI",
                                "FIN INICIALIZACION","FACTURAS INDEPENDIENTES",
                            "CADENA CONTINUA EN PROXIMA","AGREGAR IVA NO INSCRIPTO","SI FACTURA A",
                            "CADENA REEMPLAZO TICKET",
                                //  "LETRA CONDENSADA ON",
                                //  "LETRA CONDENSADA OFF",
                                "SI FACTURA B","LARGO FACTURA A","LARGO FACTURA B",
                            "RENGLONES CUERPO A","RENGLONES CUERPO B","LARGO CADENA",
                            "TOTAL EN LETRAS","REIMPRESION","TOTAL IMPUESTO","LARGO REMITO",
                            "COMIENZO CUERPO FACTURA DE REMITO",
                                //  "REMITOS INDEPENDIENTES",
                                "TOTAL HOJAS COMPROBANTE","HOJA ACTUAL",
                            "PEDIR FORMULARIO CON TECLA","NETO GRAVADO COD","IVA1 COD","IVA2 COD",
                            "SI NOTA CREDITO","SI BIEN DE USO","COMIENZO CUERPO CANTIDAD",
                            "FIN CUERPO CANTIDAD","COMIENZO CUERPO UNITARIO","FIN CUERPO UNITARIO",
                            "PRECIO UNITARIO","PRECIO TOTAL","TOTAL TICKET","VUELTO","CORTE TICKET",
                            "COMIENZO CUERPO TOTAL","FIN CUERPO TOTAL","COMIENZO CUERPO MEDIOS",
                            "FIN CUERPO MEDIOS","COMIENZO CUERPO IVA","FIN CUERPO IVA",
                            "IMPORTE MEDIO","SELECT","COMIENZO PIE","FIN PIE",
                            "COMIENZO CUERPO VUELTO","FIN CUERPO VUELTO","FIN PARTE FIJA",
                            "COMIENZO CUERPO AFINIDAD","FIN CUERPO AFINIDAD",
                            "SI MODO ENTRENAMIENTO","SI REIMPRESION","SI ANULACION",
                            "SI MODO INVENTARIO","SI DEVOLUCION","COMIENZO CUERPO NO AFINIDAD",
                            "FIN CUERPO NO AFINIDAD","SI MEDIO","IMPORTE MEDIO ME",
                            "COMIENZO CUERPO DONACION","FIN CUERPO DONACION","DATO MEDIO",
                            "PRECIO SIN IVA EN TICKET","SI VARIABLE RNV","SI NO VARIABLE RNV",
                            "COMIENZO CUERPO ID CLIENTE","FIN CUERPO ID CLIENTE",
                            "COMIENZO CUERPO ID SIN CLIENTE","FIN CUERPO ID SIN CLIENTE",
                            "OFFSET CADENA","VARIABLE RNV","SLIP ON","SLIP OFF","CABECERA FISCAL",
                            "CABECERA FISCAL 2","PIE FISCAL","PIE FISCAL 2","CADENA FISCAL",
                            "ITEM FISCAL","CADENA NO FISCAL","COMIENZO CUERPO FONDO FIJO",
                            "FISCAL Z","FIN CUERPO FONDO FIJO","INICIO INFORME DIFERENCIA",
                            "FIN INFORME DIFERENCIA","COMANDO FISCAL","LF RECEIPT",
                            "MAX LIN COMENTARIOS","CARACTERES X LIN","INICIO CORTE PAPEL",
                            "FIN CORTE PAPEL","RESET","TICKS B","NRO DOC FISCAL COMP",
                            "DATO ARTICULO","SI DATO ARTICULO","MAXIMO RENGLONES TRANSFERENCIA",
                            "OFFSET COLUMNA","NRO TICKET CONFIG","TOTAL CONFIG","NRO CAJA CONFIG",
                            "PAUSA","FORMULA","SI FORMULA","FOR","END FOR","LF","SI EXENTO",
                            "SI GRAVADO","COMIENZO INF00","FIN INF00","VARIABLE RNV OFF",
                            "SI VARIABLE RNV OFF","SI NO VARIABLE RNV OFF","DESC MEDIO",
                            "TOTALIZAR TMP","SI VARIABLE RNV Y SI MEDIO",
                            "SI NO VARIABLE RNV Y SI MEDIO","PROCESAR DATOS ADICIONALES",
                            "DATO ADICIONAL MEDIO PAGO","DATOS ADICIONALES","CONFIRMAR IMPRESION",
                            "CONSULTA TABLA","COMIENZO INF TAR","FIN INF TAR","ENVIAR BMP",
                            "COMIENZO TIMBRADORA","FIN TIMBRADORA","SET FONT",
                            "INICIO INFORME SUPERVISOR","FIN INFORME SUPERVISOR",
                            "INICIO CABECERA INFORME SUPERVISOR","FIN CABECERA INFORME SUPERVISOR",
                            "COMIENZO SI","FIN SI","VERSION","FECHA ALINEADA",
                            "COMIENZO PAGO REINTEGRO","FIN PAGO REINTEGRO","COMIENZO TIMBRADO REINTEGRO",
                            "FIN TIMBRADO REINTEGRO","ULTIMO ID","CONSULTA TABLA SQL","INICIO INFORME DIFERENCIA SUBMEDIOS",
							"FIN INFORME DIFERENCIA SUBMEDIOS","INICIO CABECERA INFORME DIFERENCIA SUBMEDIOS",
							"FIN CABECERA INFORME DIFERENCIA SUBMEDIOS","CANTIDAD EVENTOS","ID EVENTO NRO","SI VALOR MED",
							"DATO MEDIO SUBMEDIO","CODIGO BARRA FOLIO","IMAGEN", "COMIENZO DESCRIPCION CUERPO", "FIN DESCRIPCION CUERPO", 
							"COMIENZO DESCRIPCION TARJETA", "FIN DESCRIPCION TARJETA", "CODIGO FOLIO", 
							"COMIENZO IMPORTE TOTAL", "FIN IMPORTE TOTAL", "COMIENZO FRANQUEO", "FIN FRANQUEO",
							"MENSAJE ESPECIAL","ESPERAR ENTER","COMIENZO TICKET EN ESPERA", "FIN TICKET EN ESPERA",
                            "IMP TOTAL TMP", "COMIENZO CUERPO INFCOMPARATIVA", "FIN CUERPO INFCOMPARATIVA", "COMIENZO FIN TICKET",
                            "FIN TICKET", "COMIENZO FRANQUEO COBROS", "FIN FRANQUEO COBROS", "INTERLINEADO", "CARACTERES POR LINEA",
                            "REPETIR CADENA","NO DEVOLUCION", 
							"SI CIERRE Z","SI CIERRE X","FIN CIERRE Z","FIN CIERRE X",
							"COMIENZO DUPLICADO TARJETA","FIN DUPLICADO TARJETA", 
							"IMPORTE MEDIO FF",
							"COMIENZO CUERPO RETIRO","FIN CUERPO RETIRO",
							"IMPORTE DE RETIROS","TOTAL DE RETIROS","TOTAL FF",
							"SI MEDIO SALIENTE",
							"COMIENZO CUERPO ING CAJERO","FIN CUERPO ING CAJERO",
							"LETRA RESALTADA","LETRA DOBLE ALTO","LETRA DOBLE ALTO NEGRITA",
							"LETRA DOBLE ANCHO NEGRITA","LETRA DOBLE ANCHO ALTO","LETRA DOBLE ANCHO ALTO NEGRITA",
							"LETRA SUBRAYADO","LETRA SUBRAYADO NEGRITA", "CAMPOS CLIENTES", "CANTIDAD DE DATOS",
                            "COMIENZO CONFIGURACION DATOS CLIENTE", "FIN CONFIGURACION DATOS CLIENTE", "FRANQUE0 AL FINAL",
							"COMIENZO INFO_Y","FIN INFO_Y","CAMPO ARCH REC", "VALIDACION","FOR CONTADOR",
							"VALIDACION AL FINAL","COMIENZO INF05","FIN INF05","COMIENZO CABECERA INF10","FIN CABECERA INF10", 
							"PRODUCTIVIDAD CAJERO","VENTA POR DEPARTAMENTO","COMIENZO INFORME 10","FIN INFORME 10",
							"COMIENZO CABECERA INFORME 15","FIN CABECERA INFORME 15","COMIENZO INFORME 15","FIN INFORME 15",
							"DIA OPERATIVO","COMIENZO CUERPO RETIRO CIERRE","FIN CUERPO RETIRO CIERRE","LINEA DE DESCRIPCION",
							"IMPRIME LINEA DE DESCRIPCION","PREFERENCIA IMPRESION",
							 NULL };

void      CONTROL_TKT_VERSION( void * );
void      _agregar_parametros_funcion( int funcion, int *ind_config, char *cadena,
    unsigned char *CONFIG_COMP );
/*****************************************************************************/
int CARGAR_CONF_FACTURA()
/*****************************************************************************/
{
    return ( CARGAR_CONFIG_TKT( FAC_CAJA_CFG, &CONFIG_FACTURA, BUFFER_FACTURA ) );
}
/*****************************************************************************/
int CARGAR_CONF_REMITO()
/*****************************************************************************/
{
    return ( CARGAR_CONFIG_TKT( REM_CAJA_CFG, &CONFIG_REMITO, BUFFER_REMITO ) );
}
/*****************************************************************************/
int CARGAR_CONF_TKT()
/*****************************************************************************/
{
    return ( CARGAR_CONFIG_TKT( TKT_CAJA_CFG, &CONFIG_TKT1, BUFFER_TICKET ) );
}
/*****************************************************************************/
int CARGAR_CONFIG_TKT( char *nom_arch, struct _ConfTkt *config[], int largo )
/*****************************************************************************/
{
    char buffer[500],*pos,*pos2,*inicio_func , tipo_error[30],*pos3;
    FILE *archivo;
    int rta = 1, var, h, linea = 0, ind_configTmp = 0, existe_tkt_version = 0;
    int comentario,i;
    warning_codes warning = no_warning;
    //unsigned char *CONFIG_COMP;
    struct _ConfTkt *CONFIG_COMP;
    #ifdef INVEL_L
    char *nom_arch_l;
    #endif
    /*---------------- Pedimos memoria para validaciones ----------------*/
    //CONFIG_COMP = ( unsigned char * ) MALLOC( largo );
    CONFIG_COMP = ( struct _ConfTkt * )MALLOC( largo * sizeof( struct _ConfTkt ) );
    *config = CONFIG_COMP;
    /*--------------- Abrimos el archivos de configuracion ---------------*/
    archivo = fopen( nom_arch, "rt" );
    if( archivo && CONFIG_COMP ) {
        /*-------------- Leemos un rengln y los procesamos -----------*/
        ind_config = 0;
        while( fgets( buffer, sizeof(buffer)-1, archivo ) && rta ) {
            linea++;
			memset(tipo_error,0,sizeof(tipo_error));
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
                    //if( buffer[0] != ';' ) {
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
                                }
                            }
                            if( var >= 0 ) {
                                CONFIG_COMP[ind_config].linea = linea;
                                CONFIG_COMP[ind_config++].codigo = var + 1;
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
                                    
                                    CONFIG_COMP[ind_config/*++*/].codigo = var + 1 + _FIN_CAMPOS;
                                    ind_configTmp = 0;
                                    CONFIG_COMP[ind_config].parametros = ( char* )malloc( cantTmp );
                                    _agregar_parametros_funcion(  var + 1, 
                                                                  &ind_configTmp, 
                                                                  &pos2[1],
                                                                  CONFIG_COMP[ind_config].parametros );
									//sino tiene parametros no debe dar warning;
									if( cantTmp < 1 && ind_configTmp != 0  ) {  //Funcion sin parametros
										// si antes del final de cadena existe un ";" ()comentario
										pos3 = strchr( &pos2[1], ';' );
										if( pos3 )
											*pos3=0;
										else{// final de cadena
											pos3 = strchr( &pos2[1], '\n' );
											if( pos3 )
												*pos3=0;
										}
										//SE REQUIERE PARA VERIFICAR QUE NO SON ESPACIOS EN BLANCO AL FINAL
										TRIM(&pos2[1]);
										if( strlen( &pos2[1] ) < 1 )   //Funcion sin parametros
											warning = fun_sin_param;
                                    }
                                    if( _TKT_VERSION == CONFIG_COMP[ind_config].codigo ) {
                                        CONTROL_TKT_VERSION( CONFIG_COMP[ind_config].parametros );
                                        existe_tkt_version = 1;
                                    }
                                    CONFIG_COMP[ind_config++].linea = linea;
                                }
                                else {
                                    rta = 0;
									sprintf( tipo_error, "No es Funcion o Variable.");
                                }
                            }
                        }
                        else {
                            rta = 0;
							sprintf( tipo_error, "en Coordinacion de []");
                        }
                    }
                    else {
                        rta = 0;
						sprintf( tipo_error, "en Final de Cadena");
                    }
                }
            }
            else {
                //printf( "\nOverflow en buffer de comprobantes.\n" );
                GRABAR_LOG_SISTEMA( "\nOverflow en buffer de comprobantes.\n" ,LOG_VENTAS,1);
                CR0_EXIT( 1 );
            }
            if( warning != no_warning ) {
				_snprintf( buffer, sizeof( buffer ), "(%s) ADVERTENCIA EN LINEA: %i %s", 
                        nom_arch, linea, warning_msgs[ warning ] );
                GRABAR_LOG_SISTEMA( buffer ,LOG_VENTAS,1);
                warning = no_warning;
            }
        }
        fclose( archivo );
        CONFIG_COMP[ind_config].codigo = _FIN;
        if( !rta ) {
			sprintf( buffer, "Error: %s", tipo_error);
            GRABAR_LOG_SISTEMA( buffer ,LOG_VENTAS,1);
            sprintf( buffer, "(%s) %s: %i ", nom_arch, ST( S_ERROR_EN_LINEA ), linea );
            GRABAR_LOG_SISTEMA( buffer ,LOG_VENTAS,1);
            MENSAJE_CON_PAUSA( buffer, 20 );
			
        }
        if( !existe_tkt_version && ( strcmp( TKT_CAJA_CFG, nom_arch ) == 0 ) ) {
            MENSAJE( "ERROR EN TKT ( NO EXISTE NUMERO DE VERSION TKT )" );
            CR0_EXIT( 1 );
        }
    }
    else {
        rta = 0;
    }
    return ( rta );
}
/*****************************************************************************
int CARGAR_CONF( char *nom_arch, unsigned char *config[], int largo )
*****************************************************************************
{
    char buffer[161],*pos,*pos2;
    FILE *archivo;
    int rta = 1, var, h, linea = 0;
    unsigned char *CONFIG_COMP;
    #ifdef INVEL_L
    char *nom_arch_l;
    #endif
    //---------------- Pedimos memoria para validaciones ----------------
    CONFIG_COMP = ( unsigned char* )MALLOC( largo );
    *config = CONFIG_COMP;
    //--------------- Abrimos el archivo de configuracion ---------------
    archivo = fopen( nom_arch, "rt" );
    if( archivo && CONFIG_COMP ) {
        //-------------- Leemos un rengln y los procesamos -----------
        ind_config = 0;
        while( fgets( buffer, 160, archivo ) && rta ) {
            linea++;
            if( ind_config < largo ) {
                if( buffer[0] != ';' ) {
                    pos = strchr( buffer, '\n' );
                    if( pos ) {
                        *pos = 0;
                        pos = strchr( buffer, '[' );
                        pos2 = strchr( pos, ']' );
                        if( pos && pos2 ) {
                            *pos2 = 0;
                            _strupr( buffer );
                            //---------- Verificamos si es un campo  ------
                            for( h = 0, var = -1;campos[h] && var < 0;h++ ) {
                                if( strcmp( campos[h], &pos[1] ) == 0 ) {
                                    var = h;
                                }
                            }
                            if( var >= 0 ) {
                                CONFIG_COMP[ind_config++] = var + 1;
                            }
                            else {
                                //--------- Verificamos si es una funcion ---------
                                for( h = 0, var = -1;funciones[h] && var < 0;h++ ) {
                                    if( strcmp( funciones[h], &pos[1] ) == 0 ) {
                                        var = h;
                                    }
                                }
                                if( var >= 0 ) {
                                    CONFIG_COMP[ind_config++] = var + 1 + _FIN_CAMPOS;
                                    _agregar_parametros_funcion( var + 1, &ind_config, &pos2[1],
                                                                 CONFIG_COMP );
                                }
                                else {
                                    rta = 0;
                                }
                            }
                        }
                        else {
                            rta = 0;
                        }
                    }
                    else {
                        rta = 0;
                    }
                }
            }
            else {
                printf( "\nOverflow en buffer de comprobantes.\n" );
                GRABAR_LOG_SISTEMA( "\nOverflow en buffer de comprobantes.\n" );
                CR0_EXIT( 1 );
            }
        }
        fclose( archivo );
        CONFIG_COMP[ind_config] = 0xff;
        if( !rta ) {
            sprintf( buffer, "%s: %i\n", ST( S_ERROR_EN_LINEA ), linea );
            GRABAR_LOG_SISTEMA( buffer );
            MENSAJE( buffer );
        }
    }
    else {
        rta = 0;
    }
    return ( rta );
}*/
/*****************************************************************************/
void _agregar_parametros_funcion( int funcion, int *ind_config, char *cadena,
                                  unsigned char *CONFIG_COMP )
/*****************************************************************************/
{
    char *p_cadena;
    int valor,largo,largo_ant;
    char *cadfinal;
    p_cadena = cadena;
    switch( funcion + _FIN_CAMPOS ) {
        case _LARGO_FACTURA:
        case _LARGO_REMITO:
        case _LARGO_FACTURA_A:
        case _LARGO_FACTURA_B:
        case _RENGLONES_CUERPO:
        case _RENGLONES_CUERPO_A:
        case _RENGLONES_CUERPO_B:
        case _LARGO_CADENA:
        case _POS_X:
        case _POS_Y:
        case _TOTAL_IMPUESTO:
        case _NETO_GRAVADO_COD:
        case _IVA1_COD:
        case _IVA2_COD:
        case _SELECT:
        case _SI_MEDIO:
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
		case _DATO_ADICIONAL_MEDIO_PAGO:
        case _CANT_DATOS_SOLICITAR:
        case _IMPORTE_MEDIO_FF:
		case _TKT_VALIDACION:
		case _TKT_VALIDACION_AL_FINAL:
	    case _TKT_DIA_OPERATIVO:
		case _IMPRIME_LINEA_DE_DESCRIPCION:
		    valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            break;
        /*case _DATO_ADICIONAL_MEDIO_PAGO:
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            //--- Primera cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;*/
        case _POSICION:
        case _PRECIO_UNITARIO:
        case _PRECIO_TOTAL:
        case _TOTAL_EN_LETRAS:
        case _IMPORTE_MEDIO:
		case _TKT_SALDO_TARJETA:
        case _TKT_IMPORTE_MEDIO_ME:
        case _TKT_OFF_VARIABLE_RNV:
        case _TKT_VARIABLE_RNV:
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            break;
        case _TKT_VERSION:
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
        case _TKT_SI_VALOR_VARIABLE_RNV:
        case _TKT_SI_NO_VALOR_VARIABLE_RNV:
        case _TKT_SI_VALOR_VARIABLE_RNV_OFF:
        case _TKT_SI_NO_VALOR_VARIABLE_RNV_OFF:
        case _TOTAL_TICKET:
        case _PROCESAR_DATOS_ADICIONALES:
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            break;
        case _TKT_TOTALIZAR_TMP:
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            break;
        case _TKT_SI_VALOR_VARIABLE_RNV_Y_SI_MEDIO:
        case _TKT_SI_NO_VALOR_VARIABLE_RNV_Y_SI_MEDIO:
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            break;
        case _TKT_ID_EVENTO_NRO:
            //--- Primera cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            // Primer entero
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            //Segundo entero
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
        case _TKT_FOR:
            //--- Primera cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Segunda cadena
            _tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_COMP[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
        case _TKT_CONSULTA_TABLA:
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            //--- Primera cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Segunda cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Tercer cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Cuarta cadena
            _tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_COMP[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
        case _TKT_CONSULTA_TABLA_SQL:
        case _TKT_FORMULA:
            //--- Primera cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Segunda cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Tercer cadena
            _tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_COMP[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
        case _TKT_SI_FORMULA:
        case _CADENA:
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
        case _IMAGEN:
		case _TKT_FOR_CONTADOR:				 
			_tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_COMP[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
		case _CABECERA_FISCAL:
		case _PIE_FISCAL:
			valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            //--- Primera cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Segunda cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
			break;
		case _LINEA_DE_DESCRIPCION:
        case _CADENA_CONTINUA_EN_PROXIMA:
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            _tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_COMP[( *ind_config )], p_cadena, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
        case _TKT_COMANDO_FISCAL:
            _tomar_cadena( &p_cadena );
            largo = strlen( p_cadena );
            largo_ant = largo;
            TRANSFORMAR_CADENA_HEXA( p_cadena, largo );
            largo = strlen( p_cadena );
            memcpy( &CONFIG_COMP[( *ind_config )], p_cadena, largo + 1 );
            //( *ind_config ) += largo_ant + 1;
            ( *ind_config ) += largo + 1;
            break;
        case _TKT_FECHA_ALINEADA:
        case _TKT_ULTIMO_ID:
        case _TKT_CANT_EVENTOS:
		case _PREFERENCIA_IMPRESION :
            // ancho del string
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
        case _SET_FONT:
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            break;
        case _ENVIAR_BMP:
            //cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //entero 1
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            //entero 2
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            break;
        case _TKT_SI_VALOR_MED:
            //--- Primera cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Segunda cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //---Entero
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            break;
        case _TKT_DATO_MEDIO_SUBMEDIO:
            //--- Primera cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Segunda cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
			//---Entero
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
			//--- Tercer cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
			break;
        case _TKT_REPETIR_CADENA:
            //cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //---Entero
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
            //cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
        case _CAMPOS_CLIENTES:
            //--- Primera cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Segunda cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Tercera cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            //--- Primer entero 
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
			//--- cuarta cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
            break;
		case _TKT_CAMPO_ARCH_REC:
			//--- Primera cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
			//--- Primer entero 
            valor = _tomar_entero( &p_cadena );
            *( short int* )&CONFIG_COMP[( *ind_config )] = valor;
            ( *ind_config ) += 2;
			//--- segunda cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
			//--- tercer cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
			//--- cuarta cadena
            _tomar_cadena_dos( &p_cadena, &cadfinal );
            largo = strlen( cadfinal );
            memcpy( &CONFIG_COMP[( *ind_config )], cadfinal, largo + 1 );
            ( *ind_config ) += largo + 1;
			break;
    }
}
#endif
/*****************************************************************************/
void LIBERA_UTI_COMP()
/*****************************************************************************/
{
    #ifdef COMPILAR_COMPROBANTES
    if( CONFIG_FACTURA ) {
        free( CONFIG_FACTURA );
    }
    if( CONFIG_REMITO ) {
        free( CONFIG_REMITO );
    }
    //if( CONFIG_TKT )
    //free( CONFIG_TKT );
    if( CONFIG_TKT1 ) {
        free( CONFIG_TKT1 );
    }
    #endif
}
/*****************************************************************************/
int _tomar_entero( char **cadena )
/*****************************************************************************/
{
    char *pos1,*pos2;
    int valor = 0;
    pos1 = strchr( *cadena, '[' );
    pos2 = strchr( *cadena, ']' );
    if( pos1 && pos2 ) {
        *pos2 = 0;
        //      valor = atoi( &pos1[1] );
        if( pos1[1] == 'H' ) {
            valor = -3;
        }
        if( pos1[1] == 'I' ) {
            valor = 254;
		} else {
			if( pos1[1] == 'P' ) {
				valor = 255;
			} 
			else {
				valor = atoi( &pos1[1] );
			}
		}
        *cadena = &pos2[1];
    }
    return ( valor );
}
/*****************************************************************************/
float _tomar_float( char **cadena )
/*****************************************************************************/
{
    char *pos1,*pos2;
    float valor = 0;
    pos1 = strchr( *cadena, '[' );
    pos2 = strchr( *cadena, ']' );
    if( pos1 && pos2 ) {
        *pos2 = 0;
        valor = ( float )atof( &pos1[1] );
        *cadena = &pos2[1];
    }
    return ( valor );
}
/*****************************************************************************/
void _tomar_cadena( char **cadena )
/*****************************************************************************/
{
    char *pos1,*pos2;
    pos1 = strchr( *cadena, '[' );
    pos2 = strchr( *cadena, ']' );
    if( pos1 && pos2 ) {
        *cadena = &pos1[1];
        pos2[0] = 0;
    }
    else {
        ( *cadena )[0] = 0;
        ( *cadena )[1] = 0;
    }
}
/*****************************************************************************/
long _tomar_long( char **cadena )
/*****************************************************************************/
{
    char *pos1,*pos2;
    long int valor = 0;
    pos1 = strchr( *cadena, '[' );
    pos2 = strchr( *cadena, ']' );
    if( pos1 && pos2 ) {
        *pos2 = 0;
        valor = atol( &pos1[1] );
        *cadena = &pos2[1];
    }
    return ( valor );
}
/*****************************************************************************/
int _tomar_fecha( char **cadena )
/*****************************************************************************/
{
    char *pos1,*pos2;
    int valor = 0;
    pos1 = strchr( *cadena, '[' );
    pos2 = strchr( *cadena, ']' );
    if( pos1 && pos2 ) {
        *pos2 = 0;
        valor = CADENA_A_FECHA( &pos1[1] );
        *cadena = &pos2[1];
    }
    return ( valor );
}
/*****************************************************************************/
void TRANSFORMAR_CADENA_HEXA( char *cadena, int largo )
/*****************************************************************************/
{
    unsigned char *comando;
    int i = 0, j = 0;
    comando = cadena;
    while( cadena[i] && i <= largo ) {
        if( cadena[i] == 0x5c ) {
            // Caracter "\" para hexadecimales
            comando[j] = ( CAD_HEXA_2_DEC( cadena[i + 1] ) * 16 ) + CAD_HEXA_2_DEC( cadena[i + 2] );
            i += 3;
        }
        else {
            comando[j] = cadena[i];
            i++;
        }
        j++;
    }
    comando[j] = '\0';            //le pongo el caracter de fin de cadena a comando
    strcpy( cadena, comando );
}
/*****************************************************************************/
void _tomar_cadena_dos( char **cadena, char **cobtenida )
/*****************************************************************************/
{
    char *pos1,*pos2;
    pos1 = strchr( *cadena, '[' );
    pos2 = strchr( *cadena, ']' );
    if( pos1 && pos2 ) {
        *cobtenida = &pos1[1];
        *pos2 = 0;
        *cadena = &pos2[1];
    }
}
/*****************************************************************************/
void CONTROL_TKT_VERSION( void *vers )
/*****************************************************************************/
{
    struct _ver
    {
        INT16 ver;
        INT16 sub;
        char neg[10];
    } v;
    char *modos[] = {"RETAIL","SERVICIOS"};
    char salida[70];
    int modo = -1, ok = 0, tkt_ver;
    memcpy( &v, vers, sizeof( struct _ver ) );
    if( MODO_NEGOCIO == SERVICIOS && ( strncmp( v.neg, modos[1], 9 ) == 0 ) ) {
        modo = 1;
        tkt_ver = TKT_VERSION_SERVICIOS;
    }
    else if( MODO_NEGOCIO == RETAIL && ( strncmp( v.neg, modos[0], 6 ) == 0 ) ) {
        modo = 0;
        tkt_ver = TKT_VERSION_RETAIL;
    }
    if( modo == 0 || modo == 1 ) {
        if( v.ver == tkt_ver ) {
            sprintf( salida, " Tkt v%d.%d", v.ver, v.sub );
            ok = 1;
        }
        else /*(v.ver != TKT_VERSION )*/ {
            sprintf( salida, "TKT v%d.%d  ( SE REQUIERE  v%d.x )", v.ver, v.sub, tkt_ver );
        }
    }
    else {
        sprintf( salida, "VERSION TKT ( %s ) INCOMPATIBLE", v.neg );
    }
	
	MENSAJE_CON_PAUSA_CAJA( salida, 30 );
    BORRAR_MENSAJE();

	if( !ok ) {
        CR0_EXIT( 1 );
    }
}
/***********************************************************************/
char * DEVOLVER_TKT_FUNCION( int posicion ){
/***********************************************************************/
	return funciones[posicion];
}