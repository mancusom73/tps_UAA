#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cierre.h>
#include <cmemory.h>
#include <cr.h>
#include <cio.h>
//#include <io.h>
//#include <Db.h>
#include <crypt.h>
#include <dbrouter.h>
#include <driver.h>
#include <error.h>
#include <fiscal.h>
#include <log.h>
#include <malloc.h>
#include <mensaje.h>
#include <mstring.h>
#include <m_cadena.h>
#include <pausa.h>
#include <print.h>
#include <tkt.h>
#if !defined(INVEL_L)
#include <DefTables.h>
#else
#include "deftables.h"
#endif
#include <_cr1.h>
#include <ini.h>

/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
//#define COMPILAR_EMULADOR_FISCAL

void CONTROLAR_COMANDO_FISCAL();
int ANALIZAR_RESPUESTA_FISCAL( int action, char *buffer, int rta_fiscal );
extern double efectivo_retirado,total_retiros,acu_retirado[10],acu_retirado_me[10];
//extern  int CR1_VERIFICAR_LIMITES_FACTURA( double total, int modo );
extern double t_compra_retirado[100], mutuales_retirado[100], tarjetas_retirado[50];
extern int tipo_t_ev_cont, tipo_t_pagos;
extern char BUFFER_LINEA_PRN[200];
extern int tkt_columna;
struct _fecha_controlador fecha_cf;
struct _hora_controlador hora_cf;
int  ANALISIS_RESPUESTA_IMPRESORA (char * buffer, int size_buf_prn);
void INICIA_ESTADOS_IMPRESORA( void );
int ESTADOS_IMPRESORA_FISCAL( int e_fiscal, char posicion );
//int GET_ESTADO_FISCAL(int pregunta);
void FORMATO_CAB_COMPROBANTE_FISCAL(  char *texto, int linea, char *dobleancho );
void FORMATO_FIN_COMPROBANTE_FISCAL( char *texto, int linea, char *dobleancho);
int COMANDO_PRINT_REC_FISCAL( int action, char *buffer, int len ) ;
int CONTINUO_IMPRESION ( int rta_fiscal );
int VERIFICAR_SERIAL_IMPRESORA( char* buffer,int size_buf_prn );
int CONTROLA_LOS_ENVIOS_Y_REINTENTA(int action, char *buffer, int len, int lim_reintentos , int preg_status);
extern void ROUND2( double* numero, int enteros, int decimales );
extern int GET_HANDLE_VOUCHER ();
extern void IMPRIMIR_CADENA( int x, char *cadena );
extern void CARGAR_SERIAL_IMPRESORA(void);
static struct{
	char e_impresora[17];
	char e_fiscal[17];
}ESTADO_IMPRESORA;
extern void TECLA_ANULA_TICKET( int incondicional );
extern int VERIFICA_CAMBIO_ARCHIVOS_ACTIVOS(int inicial);
extern void IMPRIME_CUERPO_RETIROS( int anulacion, char *nombre, double importe, long numero, int cantidad,
                             double importe_me, int medio );;
static char *printerErrors[] = {"PAPER_OUT","RECEIPT_NEAR_END","JOURNAL_NEAR_END","PRINTER_OFFLINE",
                             "PRINTER_ERROR","CASH_DRAWER_OPEN"};
static char *fiscalErrors[] = {"DOCUMENTO_FISCAL_ABIERTO","DOCUMENTO_NO_FISCAL_ABIERTO","CIERRE_FISCAL_TICKET",
                            "MEMORIA_FISCAL_LLENA","DESBORDAMIENTO_TOTALES","COMANDO_INVALIDO","MEMORIA_FISCAL_CASI_LLENA"};
int CONTROL_EN_NO_DOC_FISCALES(void);
int PROCESAR_RESPUESTA( int action );
extern int PIDE_SUBTOTAL( double *importe );
void CARGA_ESTADOS_IMPRESORA( int e_fiscal, char posicion );
void CARGAR_DATOS_TF(char *name,char *lastname,char *address1,char *address2,char *address3,char *remito,char *remito1);
struct Acu_Medios {
	int modoPago;
	int codSubmedio;			
	char nomMedio[10];
	char nomSubMedio[30];
	double AcuImporte;
}AcuMedios[10];
int mostrarmensaje = 1;
//extern int usar_tres_decimales;
/**************************************************************************/
int FISCAL_X()
/**************************************************************************/
{
    #ifdef COMPILAR_FISCAL
    if( IMPRESORA_FISCAL ) {
        #if defined(INVEL_W) || defined(INVEL_L)
		char buffer[100];
		int rta;
		memset(buffer,0,sizeof(buffer));
		rta = CONTROLA_LOS_ENVIOS_Y_REINTENTA(ACTION_PRINT_X_REPORT, buffer,100, 4 ,SI);
        //setData( ACTION_PRINT_X_REPORT, NULL, 0 );
        #else
        TKT_FISCAL_X();
        return ( 1 );
        #endif
    }
    #endif
    return ( 0 );
}
/**************************************************************************/
int FISCAL_Z()
/**************************************************************************/
{
    int ok = 1;
    #ifdef COMPILAR_FISCAL
    #if defined(INVEL_W) || defined(INVEL_L)
    if( IMPRESORA_FISCAL == FISCAL ) {
		char buffer[100];
		int rta;
		memset(buffer,0,sizeof(buffer));
//        setData( ACTION_PRINT_Z_REPORT, NULL, 0 );
		    rta = CONTROLA_LOS_ENVIOS_Y_REINTENTA(ACTION_PRINT_Z_REPORT, buffer,100, 4 ,SI);
			//rta = COMANDO_PRINT_REC_FISCAL( ACTION_PRINT_Z_REPORT, buffer, 0 ); //nueva modalidad codigo R

    }
    #else
    /*if( IMPRESORA_FISCAL ) {
        //if( SINCRONIZAR_NUMERO_TICKET_FISCAL ) {
        //  COMANDO_LEER_CONTADORES_PRINTER_FISCAL( 4, SI, NULL );
        //}
        TKT_FISCAL_Z();
    }*/
    #endif
    #endif
    return ok;
}
/**************************************************************************/
int FISCAL_B()
/**************************************************************************/
{
    int ok = 0;
    #ifdef COMPILAR_FISCAL
    #if defined(INVEL_W) || defined(INVEL_L)
    ok = ABRIR_DOCUMENTO_FISCAL();
    #else
    if( IMPRESORA_FISCAL ) {
        if( IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
            LEER_PRINTER_IBM4610_FLASH( 1 );
        }
        //if( IMPRESORA_FISCAL == FISCAL )
        //if( IMPRESORA_FISCAL == FISCAL && SINCRONIZAR_NUMERO_TICKET_FISCAL )
        //  COMANDO_LEER_CONTADORES_PRINTER_FISCAL( 2, SI, NULL );
        //TKT_FISCAL_B();
        //if( IMPRESORA_FISCAL == FISCAL && SINCRONIZAR_NUMERO_TICKET_FISCAL )
        //   ok = COMANDO_LEER_STATUS_PRINTER_FISCAL( 2 );
    }
    #endif
    #endif
    return( ok );
}
/**************************************************************************/
void FISCAL_Y()
/**************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    if( IMPRESORA_FISCAL ) {
        TKT_FISCAL_Y();
    }
    #endif
}
/**************************************************************************/
void FISCAL_VERIFICA_ESTADO()
/**************************************************************************/
{
    #ifdef COMPILAR_FISCAL
    #if defined(INVEL_W) || defined(INVEL_L)
    COMANDO_LEER_STATUS_PRINTER_FISCAL( 1 , NULL );
    #else
    char cadena[41];
    switch( IMPRESORA_FISCAL ) {
        case FISCAL_IBM_4694:
            sprintf( cadena, ",.                                    " );
            MOSTRAR_CADENA( 1, PRN, cadena );
            LF( 1 );
            break;
    }
    #endif
    #endif
}

/**************************************************************************/
int MEDIOS_AGRUPADOS( int sumar, int modo, int cod_submedio )
/**************************************************************************/
{
	int ram_p_pago = 0, i = 0, h = 0, salir = 0, CantMediosAcu = 0;
	int pago_modo = 0, pago_cod_submedio = 0, agregar = 1;
	char nombre[17];
	double pago_total = 0.0;

	memset (nombre,0,sizeof(nombre));
	memset (&AcuMedios,0,sizeof(struct Acu_Medios) *10);

	ram_p_pago = (int)RAM_P_PAGO;

	if( config_tps.AgruparMediosEnImpresion ) {
		ram_p_pago = 0;
		for( i = 0; i < RAM_P_PAGO + sumar; i++ ) {//i = medio; 9 medios
			memset (nombre,0,sizeof(nombre));
			salir = 0;
			pago_total = 0;
			
			if( i != RAM_P_PAGO ) {
				pago_total = _PAGO_IMPORTE( i );
				pago_modo = (int)_PAGO_MODO(i);
				pago_cod_submedio = _PAGO_COD_SUBMEDIO(i);
			} else {//Si son iguales entonces suma = 1
				pago_modo = modo;
				pago_cod_submedio = cod_submedio;
			}
			NOMBRE_MEDIO_SUBMEDIO( pago_modo, pago_cod_submedio, nombre, sizeof( nombre ) );

			for( h = 0; h < 10
					&& !salir 
					&& ( config_tps.CantMediosPermitidos
					&& CantMediosAcu + 1 <= config_tps.CantMediosPermitidos); h++ ) {//Acumula 10 medios/submedios distintos
				if( AcuMedios[h].modoPago == pago_modo 
						&& AcuMedios[h].codSubmedio == pago_cod_submedio 
						&& AcuMedios[h].AcuImporte > 0 ) {//Acumulo un medio existente
					AcuMedios[h].AcuImporte += pago_total;
					salir = 1;//Salimos porque encuentra el medio ha acumular
					CantMediosAcu ++;
					agregar = 0;
				}
			}

			if( agregar ) {//Cargo un nuevo medio
				AcuMedios[ram_p_pago].modoPago = pago_modo;
				AcuMedios[ram_p_pago].codSubmedio = pago_cod_submedio;
				_snprintf( AcuMedios[ram_p_pago].nomMedio, sizeof(AcuMedios[ram_p_pago].nomMedio)-1,"%s", medios[_PAGO_MODO( i )].nombre );
				_snprintf( AcuMedios[ram_p_pago].nomSubMedio, sizeof(AcuMedios[ram_p_pago].nomSubMedio)-1,"%s", nombre );
				AcuMedios[ram_p_pago].AcuImporte = pago_total;
				ram_p_pago++;
			}
			agregar = 1;
		}
	}

	return( ram_p_pago );
}

/**************************************************************************/
int FISCAL_T( double importe, int modo )
/**************************************************************************/
{
    int ok = 1, rta = 0;
    #if defined(INVEL_W) || defined(INVEL_L)
    char buff[150];
    int decimales = 0, signo = 0, tipo_medio =0;
    double pago_total = 0.0;
    int i = 0, ram_p_pago = 0;
    char total[20];
	char nombre[17];

	setHabPing(0);

	
	ram_p_pago = MEDIOS_AGRUPADOS( NO, 0, 0 );

	for( i = 0;i < ram_p_pago/*RAM_P_PAGO*/ && ok ;i++ ) {
		memset (buff,0,sizeof(buff));
		memset (total,0,sizeof(total));
		memset (nombre,0,sizeof(nombre));

        if( !config_tps.AgruparMediosEnImpresion ) {
			pago_total = _PAGO_IMPORTE( i );
			tipo_medio = TIPO_MEDIO_PAGO( _PAGO_MODO(i) );
			NOMBRE_MEDIO_SUBMEDIO( _PAGO_MODO(i), _PAGO_COD_SUBMEDIO(i), nombre, sizeof( nombre ) );
        	sprintf( total, "%s", fcvt( pago_total, DECIMALES, &decimales, &signo ) );
        	sprintf( buff, "%s&%s&%s&%i", total, fcvt( importe, DECIMALES, &decimales, &signo ), nombre, tipo_medio);
		} else {
			tipo_medio = TIPO_MEDIO_PAGO( _PAGO_MODO(i) );
			sprintf( total, "%s", fcvt( AcuMedios[i].AcuImporte, DECIMALES, &decimales, &signo ) );
			sprintf( buff, "%s&%s&%s&%i", total, fcvt( importe, DECIMALES, &decimales, &signo ), AcuMedios[i].nomSubMedio,tipo_medio );
		}
		if( modo == _MODO_VENTA ) {
		//Los siguientes comprobantes tiene distintos numeros de ticket
		//para la impresora TM220:
		//Ticket Factura A, Nota Credito A, Nota Credito B y Ticket Comun.
		//El Ticket Factura B posee la misma numeración que Ticket Comun.
		if( RAM_TICKET_FACTURA || RAM_NOTA_CR )
		{
			//setData( ACTION_PRINT_REC_TOTAL_FAC, buff, 0 );
			if( DOCUMENTO_ABIERTO( ) ) {
				char buffer3[150];
				memcpy(buffer3,buff, sizeof(buff));
				buffer3[149] = 0;
				rta = COMANDO_PRINT_REC_FISCAL( ACTION_PRINT_REC_TOTAL_FAC, buff, 0 ); //nueva modalidad
				glog("comparo con bufffer3" ,1,1);
				if( strncmp(buffer3, buff, 100) == 0)
				{    rta =1;
				} else {
					rta = ANALIZAR_RESPUESTA_FISCAL (ACTION_PRINT_REC_TOTAL_FAC, buff, rta );
					glog("OK ANALIZAR_RESPUESTA_FISCAL (ACTION_PRINT_REC_TOTAL_FAC",1,1);
				}
			}
			if( ( rta == 3 || rta== -1 ) || !( rta = CONTROL_EN_NO_DOC_FISCALES( ) ) )
				ok = 0;
			else {	
				SET_MEMORY_LONG( __ram_nro_ultimo_ticket, NRO_TICKET );
				ADD_MEMORY_LONG( __nro_ticket, 1L );
			}
		}
		else
		{
			//setData( ACTION_PRINT_REC_TOTAL, buff, 0 );
			if( DOCUMENTO_ABIERTO( ) ) {
				rta = COMANDO_PRINT_REC_FISCAL( ACTION_PRINT_REC_TOTAL, buff, 0 ); //nueva modalidad
				rta = ANALIZAR_RESPUESTA_FISCAL( ACTION_PRINT_REC_TOTAL, buff, rta );
			} 
			if( ( rta == 3 || rta == -1 ) || !( rta = CONTROL_EN_NO_DOC_FISCALES( ) ) )
				ok = 0;
			else {	// 3 y -1 anular ticket
				SET_MEMORY_LONG( __ram_nro_ultimo_ticket, NRO_TICKET );
				ADD_MEMORY_LONG( __nro_ticket, 1L );
			}
		}
	 } else {
			if( modo ==_MODO_COBRANZA ) {
				MOSTRAR_CADENA( 1, PRN, nombre );
				if( config_tps.AgruparMediosEnImpresion ) {
					pago_total = AcuMedios[i].AcuImporte;
				}
				MOSTRAR_FLOTANTE( 17, PRN, ENTEROS, DECIMALES, 0, SI, pago_total );
				LF( 1 );
			}
		}
        //PAUSA(18);
    }

		//IMPRIME_PIE();
		//IMPRIME_FIN_TICKET( );


    #else
    if( IMPRESORA_FISCAL == FISCAL_IBM_4694 ) {
        FISCAL_B();
        CONTROLAR_COMANDO_FISCAL();
        PRN_INHABILITA_AVANCE_RECEIPT();
    }
    if( IMPRESORA_FISCAL && modo == _MODO_VENTA ) {
        TKT_FISCAL_T( importe );
        SET_MEMORY_LONG( __ram_nro_ultimo_ticket, NRO_TICKET );
        ADD_MEMORY_LONG( __nro_ticket, 1L );
        /*
        if( IMPRESORA_FISCAL == FISCAL && SINCRONIZAR_NUMERO_TICKET_FISCAL ) {
           ok = COMANDO_LEER_CONTADORES_PRINTER_FISCAL( 3, NO, &nro_boleta );
           if( ok ) {
            if( nro_boleta != NRO_TICKET ) {
                MENSAJE_TECLA( "ERROR FISCAL EN TOTAL" );
                ok = 0;
            }
           }
        }*/
    }
    #endif
	setHabPing(1);
    return( ok );
}

/**************************************************************************/
int TIPO_MEDIO_PAGO( int cod_medio )
/* vamos a devolver el tipo de medio segun las nuevas fiscales 2G
1,Efectivo
2, Cta Cte
3,tarjeta Credito
4 ,Tarjeta Debito
5, Ticket
6, Cheque
7, Medio de pago comercio exterior
cualquier otro lo tomo como Otros medios de pago
*************************************************************************/
{
	if( medios[cod_medio].efectivo && !medios[cod_medio].moneda_extranjera)
		return 1;
	if( medios[cod_medio].cta_cte)
		return 2;
	if( medios[cod_medio].tarjeta)
		return 3;
	if( medios[cod_medio].ticket_compra)
		return 5;
	if( medios[cod_medio].cheque)
		return 6;
	if( medios[cod_medio].moneda_extranjera)
		return 7;
	return 8;
}


/**************************************************************************/
void IMPRIME_PERCEPCION_FISCAL( char *desc, double importe, int tipo_percepcion )
/**************************************************************************/
{
    char buffer[50];
	int dec = 0, signo = 0,h, salir=0;
	char tipo = 'O';
	int tasa = 0;
	double total2[10], total_ticket =0.00;

	switch(tipo_percepcion)
	{
	case PERCEP_DGI:// PERCPECION/RETENCION IVA
		
		tipo ='T';
		for( h = 0;h < 9 && salir < 1 ;h++ ) {
				total2[h] = _TOT_TICK_TOTAL_POR_COD_IVA( h )  - _TOT_TICK_IMP_INT_POR_COD_IVA( h );
				total_ticket += total2[h];
				if(fabs(total2[h]) >  fabs(RAM_PERCEPCION_DGI )) {
					salir = 1;
					tasa = iva[h].porc_iva1 * 100;
				}
		}

		break;
	case RETEN_DGR: //INGRESOS BRUTOS 
		tipo ='I';
		break;
	default ://OTROS TIPOS DE PERCEPCIONES Y RETENCIONES
		tipo ='O';
		break;
	}

	CARACTERES_ESPECIALES(desc, _IMPRIMIR_CADENA);
    sprintf( buffer, "%s&%s&%d&%c", desc, fcvt( importe, DECIMALES, &dec, &signo ), tasa, tipo);
    setData( ACTION_PRINT_PERCEP_RETEN, buffer, 0 );

}
/**************************************************************************/
int ENVIAR_PERCEPCION_COMO_ARTICULO( char *nombre, double importe )
/**************************************************************************/
{
	int rta = 0;

	SET_MEMORY_DOUBLE( __item_imp_int, 0.0 );
	SET_MEMORY( __item_cod_barra, MASCARA( ' ', 13, "" ) );
	SET_MEMORY_LONG( __item_cod_especial, 0L );
	SET_MEMORY_LONG( __item_gravado, 0);
	SET_MEMORY_LONG( __item_origen, _ORIGEN_TICKET );
	SET_MEMORY_DOUBLE( __ram_cantidad , 1 );
	CEREAR_MEMORY( __item_cod_especial_alfa );
	SET_MEMORY_DOUBLE( __item_precio_sin_iva, importe );
	rta = IMPRIME_ITEM_FAC(nombre, ITEM_PRECIO_SIN_IVA, ITEM_IMP_INT, RAM_CANTIDAD );

   return ( rta );
}

/**************************************************************************/
void FISCAL_FIN_INFORME()
/**************************************************************************/
{
    #ifdef COMPILAR_FISCAL
    #if defined(INVEL_W) || defined(INVEL_L)
    // HACER ALGO
    #else
    if( IMPRESORA_FISCAL == FISCAL_EPSON ) {
        ENVIAR_COMANDO_FISCAL( "\02" );
    }
    #endif
    #endif
}
/**************************************************************************/
void FISCAL_FIN_BOLETA()
/**************************************************************************/
{
    #ifdef COMPILAR_FISCAL
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    char fin_boleta[] = {2,'A',0x01,3,0};
    if( IMPRESORA_FISCAL == FISCAL_EPSON ) {
        ENVIAR_COMANDO_FISCAL( fin_boleta );
    }
    #endif
    #endif
}
/***************************************************************************/
void ENVIAR_COMANDO_FISCAL( unsigned char *datos )
/***************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    int i;
    for( i = 0;datos[i];i++ ) {
        SEND_BYTE( datos[i] );
    }
    #endif
}
/***************************************************************************/
void FISCAL_ABRIR_CAJON()
/***************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    // HACER ALGO
    #else
    char copia_ticket, comando[] = {27,'p','0',10,40,0};
    copia_ticket = VARIOS_COPIA_TICKET_PRN;
    SET_MEMORY_CHAR( __varios_copia_ticket_prn, 0 );
    ENVIAR_COMANDO_FISCAL( comando );
    SET_MEMORY_CHAR( __varios_copia_ticket_prn, copia_ticket );
    #endif
}
/***************************************************************************/
int FISCAL_CAJON_CERRADO()
/***************************************************************************/
{
    int rta = 1;
    #if defined(INVEL_W) || defined(INVEL_L)
    // HACER ALGO
    #else
    char comando[] = {29,5,0};
    ENVIAR_COMANDO_FISCAL( comando );
    rta = LEER_STATUS_PRINTER();
    #endif
    return ( rta );
}
/**************************************************************************/
void FISCAL_VERIFICACION()
/**************************************************************************/
{
    #ifdef COMPILAR_FISCAL
    char cadena[41];
    switch( IMPRESORA_FISCAL ) {
        case FISCAL_IBM_4614:
            sprintf( cadena, ",V" );
            MOSTRAR_CADENA( 1, PRN, cadena );
            LF( 1 );
            break;
        case FISCAL_IBM_4694:
            // 1234567890123456789012345678901234567890
            sprintf( cadena, ",V                                    " );
            MOSTRAR_CADENA( 1, PRN, cadena );
            LF( 1 );
            break;
    }
    #endif
}
/**************************************************************************/
int CAD_HEXA_2_DEC( char byte )
/**************************************************************************/
{
    int valor;
    switch( byte ) {
        case 'a':
        case 'A':
            valor = 10;
            break;
        case 'b':
        case 'B':
            valor = 11;
            break;
        case 'c':
        case 'C':
            valor = 12;
            break;
        case 'd':
        case 'D':
            valor = 13;
            break;
        case 'e':
        case 'E':
            valor = 14;
            break;
        case 'f':
        case 'F':
            valor = 15;
            break;
        default:
            valor = byte - '0';
            break;
    }
    return valor;
}
/**************************************************************************/
void FISCAL_INICIALIZAR()
/**************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    // HACER ALGO
    #else
    if( IMPRESORA_FISCAL ) {
        TKT_INICIALIZAR_FISCAL();
    }
    #endif
}
/**************************************************************************/
void CONTROLAR_COMANDO_FISCAL()
/**************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    int ok;
    if( IMPRESORA_FISCAL == FISCAL_IBM_4694 ) {
        DRIVER( _DRV_printer, _PRN_status, 0, &ok, sizeof( ok ) );
        while( !( ok & _PRN_buffers_empty ) ) {
            delay( 5 );
            DRIVER( _DRV_printer, _PRN_status, 0, &ok, sizeof( ok ) );
        }
    }
    #endif
}
/**************************************************************************/
void FIN_DOCUMENTO( int anula, double total )
/**************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    char cadena1[41];
    memset( cadena1, ' ', 41 );
    if( IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
        if( !anula ) {
            cadena1[0] = '.';
            cadena1[1] = '.';
            cadena1[2] = 0;
        }
        else if( anula == SI ) {
            cadena1[0] = '.';
            cadena1[1] = '.';
            cadena1[2] = 0x0c;
            cadena1[3] = 0;
        }
        else {
            memset( cadena1, ' ', 41 );
            cadena1[0] = '.';
            cadena1[1] = ':';
            sprintf( &cadena1[11], "%s %9.0lf ", ST( S_TOTAL___________ ), total );
            cadena1[38] = 0;
        }
        if( MODO_DEBUG == 50 ) {
            GRABAR_LOG_SISTEMA( cadena1 );
        }
        MOSTRAR_CADENA( 1, PRN, cadena1 );
        LF( 1 );
        if( anula == 2 ) {
            memset( cadena1, ' ', 41 );
            cadena1[0] = '.';
            cadena1[1] = '.';
            cadena1[2] = 0x0c;
            cadena1[3] = 0;
            if( MODO_DEBUG == 50 ) {
                GRABAR_LOG_SISTEMA( cadena1 );
            }
            MOSTRAR_CADENA( 1, PRN, cadena1 );
            LF( 1 );
        }
    }
    #endif
}

/**************************************************************************/
int ABRIR_DOCUMENTO_FISCAL()
/**************************************************************************/
{
    int rta = 0;
    #if defined(INVEL_W) || defined(INVEL_L)

	//setData( ACTION_BEGIN_FISCAL_REC, NULL, 0 );
    if( IMPRESORA_FISCAL ) {
		rta = COMANDO_LEER_STATUS_PRINTER_FISCAL( 1 , NULL );
		//Para Impresoras No Fiscalizadas la respuesta es _FISCAL_ok
		if( GET_ESTADO_FISCAL(_FISCAL_doc_no_fiscal_abierto ) ) {
			CERRAR_DOCUMENTO_NO_FISCAL();
			rta = COMANDO_LEER_STATUS_PRINTER_FISCAL( 1 , NULL );
		}
        if( !GET_ESTADO_FISCAL(_FISCAL_doc_fiscal_abierto) && CONTINUO_IMPRESION( rta) ) {
            setData( ACTION_BEGIN_FISCAL_REC, NULL, 0 );
			rta = GET_ESTADO_FISCAL(_FISCAL_doc_fiscal_abierto);
			SET_MEMORY_CHAR( __ram_impresion_fiscal, 1 );
			TIPO_DE_LETRA = LETRA_NORMAL;
        }
        else {
            rta = DEBE_ANULAR_TICKET;
        }
    }
    #else
    #endif
	SET_MEMORY_DOUBLE( __ram_puntos_esp_5, 0.00); //estas variables ayudan para el redondeo pero analizar sacarlas.
	SET_MEMORY_DOUBLE( __ram_puntos_esp_4, 0.00);
	SET_MEMORY_DOUBLE( __ram_puntos_esp_3, 0.00);
    return rta;
}


/**************************************************************************/
int CERRAR_DOCUMENTO_FISCAL()
/**************************************************************************/
{
    int rta = 0, intentos = 3;
	setHabPing(0);
    #if defined(INVEL_W) || defined(INVEL_L)
	//setData( ACTION_END_FISCAL_REC, NULL, 0 );
    rta = COMANDO_LEER_STATUS_PRINTER_FISCAL( 1 , NULL );
	if( GET_ESTADO_FISCAL(_FISCAL_doc_fiscal_abierto ) ){
		char buffer[100];
		memset(buffer,0,sizeof(buffer));
		do{
			if( GET_ESTADO_FISCAL(_FISCAL_doc_fiscal_abierto ) ) {
				SET_MEMORY_INT( __ram_impresion_comprobante, _COMPROBANTE_PRE_CERRADO );
				rta = COMANDO_PRINT_REC_FISCAL( ACTION_END_FISCAL_REC, buffer, 0 ); //envio el cierre
			}
			
			if( rta !=_FISCAL_ok ){//salio algo mal ?? 
				COMANDO_LEER_STATUS_PRINTER_FISCAL( 1 , NULL );
				rta = CONTROL_EN_NO_DOC_FISCALES();
			}
			if(GET_ESTADO_FISCAL(_FISCAL_doc_fiscal_abierto ))
				rta = 0;
		}while(  rta !=_FISCAL_ok && rta != SI && (--intentos) > 0  );
		
		if( rta )//Seteo un estado intermedio para identicar que el comprobante se cerro pero aun 
			//las variables ram no fueron cereadas. Todo esto es para cuando ocurre un corte de luz
			//Le resto un valor grande negativamente al valor actual para luego recuperar su valor 
			//sumando la diferencia restada.
			SET_MEMORY_INT( __ram_impresion_comprobante, _COMPROBANTE_POS_CERRADO );
		
		if( !GET_ESTADO_FISCAL(_FISCAL_doc_fiscal_abierto ))//sino esta abierto , lo logro cerrar
			SET_MEMORY_CHAR( __ram_impresion_fiscal, 0 );
	} 
	if( rta == 0 )// HUbo algun error!! sigo el codigo 16 para la inconsistencias
		rta = 16;
	
	//Se cerea el pie fiscal porque queda gravado en la memoria fiscal
	//CEREAR_PIE_FISCAL( );
    #else
    #endif
	setHabPing(1);
    return( rta );
}
/**************************************************************************/
int CONTROL_EN_NO_DOC_FISCALES(void){
/****************
Si existe algun error voy a esperar 10 teclas enter a que lo solucionen y sino sigue la operatoria normal

**********************************************************/
	int reintentos = 0, rta = NO;

	if( GET_ESTADO_FISCAL(_FISCAL_nec_cierre ) && GET_ESTADO_FISCAL(_FISCAL_comando_invalido   ) ){
		if( NRO_MODO == 'Z' ||NRO_MODO == 'Y' ||NRO_MODO == 'X' ){
			MENSAJE_SIN_SONIDO( "-  LA IMPRESORA REQUIERE CIERRE FISCAL - " ,10);
		}else{
			MENSAJE( "-  LA IMPRESORA REQUIERE CIERRE FISCAL - " );
			if( RAM_IMPRESION_FISCAL!= 0 ){
				TECLA_ANULA_TICKET( SI );
			}
		}
		rta = SI;
	}else{
		int borrar = NO;
		do { 
			//TOMO EL ESTADO ANTERIOR-- VER SI CONVIENE SIMPLIFICAR CON -->>CONTROLA_LOS_ENVIOS_Y_REINTENTA
			if( GET_ESTADO_FISCAL( _FISCAL_offline	         )||
				GET_ESTADO_FISCAL(_FISCAL_paper_out          )||   
				GET_ESTADO_FISCAL(_FISCAL_printer_error      )||   
				//GET_ESTADO_FISCAL(_FISCAL_nec_cierre         )||
				GET_ESTADO_FISCAL(_FISCAL_desborde_totales   )||
				( GET_ESTADO_FISCAL(_FISCAL_comando_invalido )&&//si se dan los dos entra debe seguir... no imprime
				(GET_ESTADO_FISCAL(_FISCAL_nec_cierre )== NO ))		){

				rta = NO;
				reintentos++;	
				borrar = SI;
				MENSAJE( ST(S_ERROR_IMPRESION_REVISE_IMPRESORA_P_E) ); 
				GRABAR_LOG_SISTEMA(  ST(S_ERROR_IMPRESION_REVISE_IMPRESORA_P_E) , LOG_ERRORES,2 );
				do{ 
					if( LASTKEY()!=-999 ){//que no muestre tantas veces..
						//BORRAR_MENSAJE();
						MENSAJE( ST(S_ERROR_IMPRESION_REVISE_IMPRESORA_P_E) );
					}
				}while(GETCH() != 13);
				//leo el nuevo estado a ver si cambia
				if( borrar = SI)
				BORRAR_MENSAJE();
				borrar = NO;

				COMANDO_LEER_STATUS_PRINTER_FISCAL(1, NULL);
				//un comprobante debe estar en curso abierto...	
				if( RAM_IMPRESION_FISCAL == -1 && ( !GET_ESTADO_FISCAL(_FISCAL_doc_no_fiscal_abierto ) ) ){
					char buffer_aux[20];
					SET_MEMORY_CHAR( __ram_impresion_fiscal, 0 );	
					memset(buffer_aux,0,sizeof(buffer_aux));
					sprintf(buffer_aux,"&0&");
					//actualizo el estado del comprobante para la bandera de impresion en CIMain
					setData( ACTION_UPDATE_ESTADO_DOC_NO_FISCAL,buffer_aux, 20 );
					ABRIR_DOCUMENTO_NO_FISCAL( NO );
				}

			} else {
				rta = SI;
				break;
			}
		}while(( rta != SI )&& reintentos < 10 ); // Jpos error wrong state
	}


	return rta ;
}
/**************************************************************************/
void ABRIR_DOCUMENTO_NO_FISCAL( char slip )
/**************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
	int rta;
	int ok = 0;
	
	rta = COMANDO_LEER_STATUS_PRINTER_FISCAL(1, NULL);
	//controlemos si quedo algun documento abierto de antes
	if( GET_ESTADO_FISCAL(_FISCAL_doc_no_fiscal_abierto ) ) { //hay un documento NO fiscal abierto
		char buffer_aux[20];
	//	SET_MEMORY_CHAR( __ram_impresion_fiscal, 0 );	
		memset(buffer_aux,0,sizeof(buffer_aux));
		sprintf(buffer_aux,"&1&");
		setData( ACTION_UPDATE_ESTADO_DOC_NO_FISCAL,buffer_aux, 20 );
		CERRAR_DOCUMENTO_NO_FISCAL();
		rta = COMANDO_LEER_STATUS_PRINTER_FISCAL( 1 , NULL );
	}

	if(GET_ESTADO_FISCAL(_FISCAL_doc_fiscal_abierto)) { //hay un documento  fiscal abierto
		CERRAR_DOCUMENTO_FISCAL();
//		rta = COMANDO_LEER_STATUS_PRINTER_FISCAL( 1 , NULL );
	}
	
	if( DOCUMENTO_ABIERTO( )!=1
			&& CONTROL_EN_NO_DOC_FISCALES()){
		setData( ACTION_BEGIN_NON_FISCAL, NULL, 0 );
		SET_MEMORY_CHAR( __ram_impresion_fiscal, -1 );
	}
	
	TIPO_DE_LETRA = LETRA_NORMAL;
	#else
    #endif
}

/**************************************************************************/
void CERRAR_DOCUMENTO_NO_FISCAL()
/**************************************************************************/
{
	#if defined(INVEL_W) || defined(INVEL_L)
   
	int rta_fiscal = 0;
	#ifdef COMPILAR_EMULADOR_FISCAL
		rta_fiscal = _FISCAL_doc_no_fiscal_abierto;
		if(rta_fiscal == _FISCAL_doc_no_fiscal_abierto){
	#else
		rta_fiscal =  COMANDO_LEER_STATUS_PRINTER_FISCAL(1, NULL ) ;
		if( CONTROL_EN_NO_DOC_FISCALES() &&  GET_ESTADO_FISCAL( _FISCAL_doc_no_fiscal_abierto ) ){
		//if( rta_fiscal == _FISCAL_doc_no_fiscal_abierto || CONTINUO_IMPRESION( rta_fiscal) ){
	#endif COMPILAR_EMULADOR_FISCAL


		//para la tm220 no hace falta esto
//		if( TEXTO_EN_IMPRESORA_FISCAL( ) ){

//			char buffer[10];
		/*aca se envia la orden de impresion si habia alguna linea en buffer*/
/*			memset( buffer, 0, 10 );
			buffer[0] = 0x0a;
			PRINT( 1, PRN, buffer );
		  }*/
		SET_MEMORY_INT( __ram_impresion_comprobante, _COMPROBANTE_PRE_CERRADO );
		setData( ACTION_END_NON_FISCAL, NULL, 0 );
		SET_MEMORY_CHAR( __ram_impresion_fiscal, 0 );
		//Seteo un estado intermedio para identicar que el comprobante se cerro pero aun 
		//las variables ram no fueron cereadas. Todo esto es para cuando ocurre un corte de luz
		//Le resto un valor grande negativamente al valor actual para luego recuperar su valor 
		//sumando la diferencia restada.
		SET_MEMORY_INT( __ram_impresion_comprobante, _COMPROBANTE_POS_CERRADO );
    }
    #else
    #endif
}
/**************************************************************************/
void ANULA_TICKET_FISCAL( char *desc )
/**************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
	CARACTERES_ESPECIALES( desc, _IMPRIMIR_CADENA );
	SET_MEMORY_INT( __ram_impresion_comprobante, _COMPROBANTE_PRE_CERRADO );
    setData( ACTION_PRINT_REC_VOID, desc, 0 );
	//Seteo un estado intermedio para identicar que el comprobante se cerro pero aun 
	//las variables ram no fueron cereadas. Todo esto es para cuando ocurre un corte de luz
	//Le resto un valor grande negativamente al valor actual para luego recuperar su valor 
	//sumando la diferencia restada.
	SET_MEMORY_INT( __ram_impresion_comprobante, _COMPROBANTE_POS_CERRADO );
    #else
    #endif
}
/**************************************************************************/
void ANULA_TICKET_FACTURA_FISCAL( char *desc )
/**************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
	CARACTERES_ESPECIALES( desc, _IMPRIMIR_CADENA );
	SET_MEMORY_INT( __ram_impresion_comprobante, _COMPROBANTE_PRE_CERRADO );
    setData( ACTION_PRINT_REC_VOID_FAC, desc, 0 );
	//Seteo un estado intermedio para identicar que el comprobante se cerro pero aun 
	//las variables ram no fueron cereadas. Todo esto es para cuando ocurre un corte de luz
	//Le resto un valor grande negativamente al valor actual para luego recuperar su valor 
	//sumando la diferencia restada.
	SET_MEMORY_INT( __ram_impresion_comprobante, _COMPROBANTE_POS_CERRADO );
    #else
    #endif
}

/**************************************************************************/
int IMPRIME_ITEM_FISCAL( char *desc, double importe, double cantidad, int indice )
/**************************************************************************/
{
    int rta = 0, rta_fiscal = 0;
    int reintentos = 0;
    /*
    * Si hay un error devuelve DEBE_ANULAR_TICKET
    */
    #if defined(INVEL_W) || defined(INVEL_L)
    char buff[500];
    char cprecio[20];
    char unidad[20];
    char civa[20];
    //   double cant;
    int signo, signo2 = 1;
    int dec;
	int /*area_ant*/ tabla_ant, tipo_ant, negativo = 0;
	int borrar = NO;
	double cantidad2 = 0.00;

	struct
	{
		long nro_caja_z;
		long nro_evento;
		int nro_posicion;
	} indice3;

	indice3.nro_posicion = indice-1;
	indice3.nro_evento = RAM_ID_EVENTO;
	indice3.nro_caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );;

	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
	SET_ORDER2("CAJA_Z, ID_EVENTO, POSICION");
	SET_WHERE("CAJA_Z = '%ld' AND ID_EVENTO = '%ld' AND POSICION = '%d' ", indice3.nro_caja_z, indice3.nro_evento, indice3.nro_posicion );
	RUN_QUERY(NO);

	if( FOUND2( ) )
	{
		if( IMPRESORA_FISCAL != FISCAL )
			SET_MEMORY_DOUBLE( __item_imp_int, event_e_ticket.imp_int ); ///y esto??
	}

	if( ( ITEM_ORIGEN == _ORIGEN_DESC_PESOS
				|| ITEM_ORIGEN == _ORIGEN_DESC_PORC
				|| ITEM_ORIGEN == _ORIGEN_DESC_PORC_FIJO )
	 	&& IMPRESORA_FISCAL == FISCAL ) 
	{ //cambiemos el signo para el descuento en el javaserver
		importe = fabs( importe);
		negativo = 1;

	}
	SELECT_TABLE( tabla_ant, tipo_ant );

   	strcpy( civa, fcvt( ( ITEM_GRAVADO )?(iva[ITEM_COD_IVA].porc_iva1 + iva[ITEM_COD_IVA].porc_iva2 ): 0, DECIMALES, &dec, &signo ) );
	//strcpy( civa, fcvt( iva[ITEM_COD_IVA].porc_iva1, DECIMALES, &dec, &signo ) );
    strcpy( cprecio, fcvt( importe , DECIMALES, &dec, &signo ) );
	if( (importe < 0.001 && ITEM_IMP_INT > 0.00) 
		|| (importe > 0.001 && ITEM_IMP_INT < -0.00) 
		) //tienen que ser del mismo signo
		signo2 = -1;
	strcpy( unidad, fcvt( (importe - ( ITEM_IMP_INT * signo2)), DECIMALES, &dec, &signo ) );
	CARACTERES_ESPECIALES( desc, _IMPRIMIR_CADENA );

//	usar_tres_decimales = NO;
    if(RAM_MODO_EJECUCION != INVENTARIO)
	{
		do {
			reintentos++;
			if( rta == 207 ) {
				if( rta = ABRIR_DOCUMENTO_FISCAL() == DEBE_ANULAR_TICKET ) {
					break;
				}
				SET_MEMORY_DOUBLE( __ram_puntos_esp_5, 0.00);
				SET_MEMORY_DOUBLE( __ram_puntos_esp_4, 0.00);
				SET_MEMORY_DOUBLE( __ram_puntos_esp_3, 0.00);
			}
			if( RAM_P_TICKET == 1 )	{ //solo al abrir el ticket controlamos el estado
				if( IMPRESORA_FISCAL == FISCAL ) {
					COMANDO_LEER_STATUS_PRINTER_FISCAL( 1 , NULL  ); //esto tambien puede demorar la impresion
					rta_fiscal = CONTINUO_IMPRESION ( rta_fiscal ) ;
				}
			} else {
				if( IMPRESORA_FISCAL == FISCAL )
					rta_fiscal = SI; //para acelerar la impresion

			}
			if( rta_fiscal == SI || IMPRESORA_FISCAL != FISCAL) {
      			if( ITEM_PEDIR_PRECIO == 1) {
					double iconiva_redondeado, itemmprecio_fiscal =0.00, itemmprecio2, canti_fiscal = 0;
					double acumi, diff, diff2, diff4 ;
					int arreglar = NO;
					double toti =  importe * cantidad; //este es el importe de la caja 12.2299

					itemmprecio_fiscal = ITEM_PRECIO;
					ROUND2(&itemmprecio_fiscal,ENTEROS,2);
					canti_fiscal = cantidad;  //0.879856
					canti_fiscal = (int)(canti_fiscal * 1000); //879.856
					canti_fiscal = canti_fiscal / 1000; //0.879 este iria a la printer
					iconiva_redondeado = itemmprecio_fiscal * canti_fiscal  ; //impresora 12.2181
					itemmprecio2 = fabs ( iconiva_redondeado - toti);

					{
						int i ;
						double tot_imp = 0.00;
						if ( RAM_P_TICKET == 1 )
							SET_MEMORY_DOUBLE( __ram_puntos_esp_5, 0.00  ); //acumulado de la printer

						for( i = 0;i < 4;i++ ) {
							tot_imp += _TOT_TICK_IMPUESTOS( i );
						}
						acumi = TOT_TICK_IMPORTE_SIN_IVA + TOT_TICK_IVA1 + TOT_TICK_IVA2 + tot_imp + TOT_TICK_EXENTO
							+ TOT_TICK_RECARGO_TARJETA + signo * RAM_RETENCION_DGR + signo * RAM_PERCEPCION_DGI
							+ signo * RAM_PERCEPCION_212_IVA1 + signo * RAM_PERCEPCION_212_IVA2 /*+ signo * RAM_PERCEPCION_TACIS*/;
					}
					diff = fabs(acumi - RAM_PUNTOS_ESP_5); //diferencia entre la fiscal y la caja

					if( diff > 0.005 && diff < 0.02 ) //si es mayor el redondeo de la printer hara de 0.01 la diferencia
						arreglar = SI;

					if( arreglar == SI || (fabs ( iconiva_redondeado - toti) > 0.005 && fabs ( iconiva_redondeado - toti) < 0.02  )) { //HAY DIFERENCIAS
						double  canti_fiscal4, canti_fiscal2,final =0.00, finalFiscal1 =0.00,finalFiscal2 =0.00, finalFiscal3 =0.00, finalFiscal4 =0.00 , finalTPS = 0.00;
						int index ;
						//modifiquemos la cantidad 1ero para saber si alcanza
						canti_fiscal2 = canti_fiscal ;
						canti_fiscal = cantidad ;

						//hay que ver cual esta mas cerca del valor final
						finalTPS  = acumi + toti ; //acumulado tpv

						//modificacion cantidad
						ROUND2(&canti_fiscal,ENTEROS,3);
						itemmprecio2 = importe  *canti_fiscal; //este valor dará en la printer

						//calculemos la cantidad
						canti_fiscal4 = finalTPS / itemmprecio_fiscal ;
						ROUND2(&canti_fiscal4,ENTEROS,3);

						finalFiscal1 =  RAM_PUNTOS_ESP_5 +  itemmprecio2 ; //acumulado fiscal cambia cantidad
						//finalFiscal2 =  RAM_PUNTOS_ESP_5 +  (importe - 0.01) * canti_fiscal ; //cambia cantidad y precio
						//finalFiscal3 =  RAM_PUNTOS_ESP_5 +  (importe - 0.01) * canti_fiscal2; //cambia precio pero no cantidad
						finalFiscal4 =  RAM_PUNTOS_ESP_5 +  ( importe * canti_fiscal4); //cambia precio pero no cantidad

						//ahora veremos que no difiera mas de 0.004
						diff = fabs(finalTPS - finalFiscal1 ); //diferencia entre la fiscal y la caja
						//	diff2 = fabs(finalTPS - finalFiscal2 );
						//	diff3 = fabs(finalTPS - finalFiscal3 );
						diff4 = fabs(finalTPS - finalFiscal4 );
						if( diff > 0.004 ) { //una diferencia hará que haya diferencia de 0.01 con la printer
							if( diff4 < 0.004 ) {//cambiemos el precio para arriba
								arreglar = SI;
								canti_fiscal = canti_fiscal4;
								index = 4;
							} else { //ninguno de los totales da..modifiquemos el importe. Ojo con las anulaciones
								arreglar = SI;
								diff2  =  toti  /canti_fiscal ;
								ROUND2(&diff2 ,ENTEROS,3);
								importe = diff2;
								index = 3;

							}
						} else {
							index = 1;
							arreglar = NO;
						}

						cantidad = canti_fiscal;

						if( arreglar == SI /*&& RAM_P_TICKET > 1*/) {
						/*  if( index > 1) {
								strcpy( cprecio, fcvt( importe + 0.01 , DECIMALES, &dec, &signo ) );
								ADD_MEMORY_DOUBLE( __ram_puntos_esp_5, importe * canti_fiscal  );
							} else {*/
								strcpy( cprecio, fcvt( importe , DECIMALES, &dec, &signo ) );
								strcpy( unidad, fcvt( (importe - ( ITEM_IMP_INT * signo2)), DECIMALES, &dec, &signo ) );
								ADD_MEMORY_DOUBLE( __ram_puntos_esp_5, importe * canti_fiscal  );
							//}
						} else
							ADD_MEMORY_DOUBLE( __ram_puntos_esp_5,itemmprecio2  );

					} else {
   						ADD_MEMORY_DOUBLE( __ram_puntos_esp_5,importe * canti_fiscal  );
   					}
				} else {
				  ADD_MEMORY_DOUBLE( __ram_puntos_esp_5,importe * cantidad  );
				}

				if( importe < 0 ||  negativo == 1 ) {
					memset( buff, 0, 500 );
					if(cantidad < 0) {
						cantidad = cantidad * (-1);
					}
					//Se hace el mismo calculo pero por pasos porque en forma directa da error
					//ej cantidad= 0.186, termina casteando en 0.185
					ROUND2(&cantidad,6,3);
					cantidad2 = (cantidad * 1000);
					ROUND2(&cantidad2,6,3);
					_snprintf( buff,sizeof(buff)-1, "%s&%s&%d&%s&%s&%s", desc, cprecio, ( int )cantidad2, civa, unidad, "$" );
					if( RAM_ANULACION )
						//rta = setData( ACTION_PRINT_REC_VOID_ADJUST_ITEM, buff, 0 );
						rta = COMANDO_PRINT_REC_FISCAL( ACTION_PRINT_REC_VOID_ADJUST_ITEM, buff, 0 ); //nueva modalidad
					else
						//rta = setData( ACTION_PRINT_REC_ADJUST_ITEM, buff, 0 );
						rta = COMANDO_PRINT_REC_FISCAL( ACTION_PRINT_REC_ADJUST_ITEM, buff, 0); //nueva modalidad
				} else {
					if( cantidad < 0 )
					{
						memset( buff, 0, 500 );
						//Se hace el mismo calculo pero por pasos porque en forma directa da error
						//ej cantidad= 0.186, termina casteando en 0.185
						ROUND2(&cantidad,6,3);
						cantidad2 = (cantidad * 1000);
						ROUND2(&cantidad2,6,3);
						_snprintf( buff,sizeof(buff)-1, "%s&%s&%d&%d&%s&%s", desc, cprecio, (int)cantidad2, 1, unidad, civa );
						//rta = setData( ACTION_PRINT_REC_VOID_ITEM, buff, 0 );
						rta = COMANDO_PRINT_REC_FISCAL( ACTION_PRINT_REC_VOID_ITEM, buff, 0); //nueva modalidad
					}
					else
					{
						memset( buff, 0, 500 );
						//Se hace el mismo calculo pero por pasos porque en forma directa da error
						//ej cantidad= 0.186, termina casteando en 0.185
						ROUND2(&cantidad,6,3);
						cantidad2 = (cantidad * 1000);
						ROUND2(&cantidad2,6,3);
						_snprintf( buff,sizeof(buff)-1, "%s&%s&%d&%s&%s&%s", desc, cprecio, (int)cantidad2, civa, unidad, "$" );
						// modo anterior rta = setData( ACTION_PRINT_REC_ITEM, buff, 0 );
						COMANDO_PRINT_REC_FISCAL( ACTION_PRINT_REC_ITEM, buff, 0 ); //nueva modalidad
					}
				
				}
				rta = CONTINUO_IMPRESION ( rta );
			} else {
				rta = DEBE_ANULAR_TICKET;
				break;
			}

			if( rta != 1 ) {
				if( GET_ESTADO_FISCAL(_FISCAL_desborde_totales)
						|| GET_ESTADO_FISCAL(_FISCAL_comando_invalido) 
						|| GET_ESTADO_FISCAL(_FISCAL_mem_llena)
						|| GET_ESTADO_FISCAL(_FISCAL_printer_error)
						|| GET_ESTADO_FISCAL(_FISCAL_nec_cierre) ) {

							rta = DEBE_ANULAR_TICKET;
						break;
				} else {
					  rta= -1 ; //reintento
					  borrar = SI;
					  if( reintentos > 1 ) {
					  MENSAJE( ST(S_ERROR_IMPRESION_REVISE_IMPRESORA_P_E) );
					  do{
						  if( LASTKEY()!=-999 ){//que no muestre tantas veces.. 
							  MENSAJE( ST(S_ERROR_IMPRESION_REVISE_IMPRESORA_P_E) );
						  }
					  }while(  GETCH() != 13);
					  }
				}
			}
		}
		while(( rta == 207 || rta == -1 )&& reintentos < 10 );   // Jpos error wrong state
		
		if( borrar == SI)
			BORRAR_MENSAJE();
		
		if(!CONTINUO_IMPRESION ( rta ) &&  reintentos )
			rta = DEBE_ANULAR_TICKET;
	} else { // si estan en modo inventario
		memset(buff,0, sizeof(buff));
		_snprintf( buff, sizeof(buff)-1,"%d      %s",(int)cantidad, desc);
		IMPRIMIR_TEXTO_NO_FISCAL(1,LETRA_NORMAL,buff, SI );
	}
	//SELECT(area_ant);
	SELECT_TABLE( tabla_ant, tipo_ant );
    /*
    * En rta esta la respuesta de Jpos.Verificar si el estado es correcto, si no abrir documento
    * si no se puede anular ticket.
    */
    #else
    #endif
    return rta;
}
/**************************************************************************/
int IMPRIME_MENSAJE_FISCAL( char *mensaje )
/**************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    int rta = 0;
    //if( COMANDO_LEER_STATUS_PRINTER_FISCAL(1) == _FISCAL_doc_fiscal_abierto )
	CARACTERES_ESPECIALES( mensaje, _IMPRIMIR_CADENA );
    if( setData( ACTION_PRINT_REC_MESSAGE, mensaje, 0 ) == 207 ) {
        rta = ABRIR_DOCUMENTO_FISCAL();
        if( rta != DEBE_ANULAR_TICKET ) {
            setData( ACTION_PRINT_REC_MESSAGE, mensaje, 0 );
        }
    }
    return rta;
    #endif
}
/**************************************************************************/
void ABRIR_DOCUMENTO( int strclave, long nro_comprobante )
/**************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    // HACER ALGO
    #else
    char comando[41];
    memset( comando, ' ', 41 );
    comando[0] = '.';
    comando[1] = 0xd4;
    switch( strclave ) {
        case 0:
            comando[2] = 0xd4;                    //selecciono RECEIP
            break;
        case 4:
            comando[2] = 0xd8;
            break;
        case 5:
            comando[2] = 0xd9;
            break;
        case 6:
            comando[2] = 0xda;
            break;
        case 7:
            comando[2] = 0xdb;
            break;
        case 8:
            comando[2] = 0xdc;
            break;
        case 9:
            comando[2] = 0xdd;
            break;
        case 10:
            comando[2] = 0xde;
            break;
        case 11:
            comando[2] = 0xdf;
            break;
        case 12:
            comando[2] = 0xe0;
            break;
        case 13:
            comando[2] = 0xe1;
            break;
        case 14:
            comando[2] = 0xe2;
            break;
        case 15:
            comando[2] = 0xe3;
            break;
        case 16:
            comando[2] = 0xe4;
            break;
        case 17:
            comando[2] = 0xe5;
            break;
        case 18:
            comando[2] = 0xe6;
            break;
        case 19:
            comando[2] = 0xe7;
            break;
        case 20:
            comando[2] = 0xe8;
            break;
        case 21:
            comando[2] = 0xe9;
            break;
        case 22:
            comando[2] = 0xea;
            break;
        case 23:
            comando[2] = 0xeb;
            break;
        case 24:
            comando[2] = 0xec;
            break;
        case 25:
            comando[2] = 0xed;
            break;
        case 26:
            comando[2] = 0xee;
            break;
        case 27:
            comando[2] = 0xef;
            break;
        case 28:
            comando[2] = 0xf0;
            break;
        case 29:
            comando[2] = 0xf1;
            break;
    }
    comando[3] = 0;
    if( MODO_DEBUG == 50 ) {
        GRABAR_LOG_SISTEMA( comando );
    }
    MOSTRAR_CADENA( 1, PRN, comando );
    LF( 1 );
    memset( comando, ' ', 41 );
    sprintf( comando, ".Correlativo:%6li Caja :%04d", nro_comprobante, NRO_CAJA );
    if( MODO_DEBUG == 50 ) {
        GRABAR_LOG_SISTEMA( comando );
    }
    MOSTRAR_CADENA( 1, PRN, comando );
    LF( 1 );
    #endif
}
/**************************************************************************/
void CERRAR_DOCUMENTO( double total, int corte_papel, int lf )
/**************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    // HACER ALGO
    #else
    char comando[41];
    memset( comando, ' ', 41 );
    comando[0] = '.';
    comando[1] = ':';
    sprintf( &comando[11], "                 %9.0lf ", total );
    if( MODO_DEBUG == 50 ) {
        GRABAR_LOG_SISTEMA( comando );
    }
    MOSTRAR_CADENA( 1, PRN, comando );
    LF( 1 );
    if( lf ) {
        LF( lf );
    }
    if( corte_papel ) {
        memset( comando, ' ', 41 );
        comando[0] = '.';
        comando[1] = '.';
        comando[2] = 0x1b;
        comando[3] = '^';
        comando[4] = 'X';
        comando[5] = 0;
        if( MODO_DEBUG == 50 ) {
            GRABAR_LOG_SISTEMA( comando );
        }
        MOSTRAR_CADENA( 1, PRN, comando );
        LF( 1 );
    }
    #endif
}
/**************************************************************************/
void CONSULTAR_DATOS_IMPRESORA_FISCAL( int *nro_caja, long *nro_z )
{
    /**************************************************************************/
    #if defined(INVEL_W) || defined(INVEL_L)
    char buffer[500];
    int pos;
    int caja = 0;
    long z = 0;
    //Asigna si los punteros son validos
    if( nro_caja ) {
        // Pide numero de caja
        getData( ACTION_GET_TERMINAL_NUM, buffer, 0 );
        if( strcmp( buffer, "Error" ) == 0 ) {
            MENSAJE_CON_PAUSA( "IMPRESORA FUERA DE LINEA", 26 );
        }
        else if( !( strcmp( buffer, "  " ) == 0 ) ) {
            strcpy( buffer, strstr( buffer, "Nro_Caja:" ) );
            pos = strcspn( buffer, ":" );
            if( pos > 0 ) {
                caja = atoi( &buffer[pos + 1] );
            }
            *nro_caja = caja;
        }
    }
    memset( buffer, 0, 500 );
    if( nro_z ) {
        // Pide numero Z
        getData( ACTION_GET_Z_NUM, buffer, 0 );
        if( strcmp( buffer, "Error" ) == 0 ) {
            MENSAJE_CON_PAUSA( "IMPRESORA FUERA DE LINEA", 26 );
        }
        else if( !( strcmp( buffer, "  " ) == 0 ) ) {
            strcpy( buffer, strstr( buffer, "Nro_Ult_Z:" ) );
            pos = strcspn( buffer, ":" );
            if( pos > 0 ) {
                z = atol( &buffer[pos + 1] );
            }
            *nro_z = z;
        }
    }
    #endif
}
/**************************************************************************/
int ESTABLECER_FECHA_HORA_FISCAL( char *buffer )
/**************************************************************************/
{
	CARACTERES_ESPECIALES( buffer, _IMPRIMIR_CADENA );
	setData( ACTION_FISCAL_ESTABLECER_FECHA_HORA, buffer, 0 );
	return 1;
}

/**************************************************************************/
int OBTENER_FECHA_HORA_FISCAL( )
/**************************************************************************/
{
	char fecha_hora[20];
	char day[3], mon[3], year[3], hour[3], min[3], sec[3];

	memset( fecha_hora, 0,  sizeof( fecha_hora ) );
	memset( day, 0,  sizeof( day ) );
	memset( mon, 0,  sizeof( mon ) );
	memset( year, 0,  sizeof( year ) );
	memset( hour, 0,  sizeof( hour ) );
	memset( min, 0,  sizeof( min ) );
	memset( sec, 0,  sizeof( sec ) );

	getData( ACTION_FISCAL_OBTENER_FECHA_HORA, fecha_hora, 0 );//DDMMAAHHMM 071010 1443
	_strlwr( fecha_hora );
	if( strcmp( fecha_hora,"error" ) ) {
		strncpy( day, &fecha_hora, 2 );
		day[2]= '\0';
		strncpy( mon, &fecha_hora[2], 2 );
		mon[2]= '\0';
		strncpy( year, &fecha_hora[4], 2 );
		year[2]= '\0';
		strncpy( hour, &fecha_hora[7], 2 );
		hour[2]= '\0';
		strncpy( min, &fecha_hora[9], 2 );
		min[2]= '\0';
		strncpy( sec,&fecha_hora[11], 2 );
		sec[2]= '\0';
		fecha_cf.da_day = atoi( day );
		fecha_cf.da_mon = atoi( mon );
		fecha_cf.da_year = atoi( year );
		hora_cf.ti_hour = atoi( hour );
		hora_cf.ti_min = atoi( min );
		hora_cf.ti_sec = atoi( sec );
	}
	
	return 1;
}

/***************************************************************************/
int COMANDO_LEER_CONTADORES_PRINTER_FISCAL( int reintentos, int asignar, long *nro_boleta )
/***************************************************************************/
{
    int ok = 0, _reintentos = 0, decrementar = 0, rta;
    char *buffer;
    char nro_ticket[200];
    char nro_factura[200];
	char nro_facturab[200];
	char nro_z[100];
	char nro_nca[100];
	char nro_ncb[100];
	long ticket = -1;
    long ticket_ant = -1;
	long factura = 0;
	long z = 0;
	long nca = 0;
	long ncb = 0;
	long facturaB = 0;
    //int index; //indice del error
	int pos = 0;
    int pos2 = 0;
	int pos3 = 0;
    int pos4 = 0;
	int pos5 = 0;
    int error_nro = 0;
    //char *token;

    //se piden los datos al menos 2 veces y se comparan, debido a que en muchos casos
    //la primera lectura devuelve datos erronos
    do {
        buffer = ( char* )malloc( 500 );
        memset( buffer, 0, 500 );
	    memset (nro_ticket,0,sizeof(nro_ticket));
	    memset (nro_factura,0,sizeof(nro_factura));
	    memset (nro_z, 0, sizeof(nro_z));
	    memset (nro_nca, 0, sizeof(nro_nca));
	    memset (nro_ncb, 0, sizeof(nro_ncb));

   	    //guardamos el valor de ticket anterior
        ticket_ant = ticket;

        //obtiene la respuesta de la impresora
        rta = getData( ACTION_GET_FISCAL_ACUM, buffer, 0 );

		if( rta != 0 ) {
        //parsea la respuesta
	    pos = strcspn( buffer, " " );
        strncpy( nro_ticket, &buffer[0], pos );
	    pos2 = strcspn( &buffer[pos + 1], " " );
        strncpy( nro_factura, &buffer[pos + 1], pos2 );
        pos3 = strcspn( &buffer[pos + pos2 + 2], " " );
	    strncpy( nro_z, &buffer[pos + pos2 + 2], pos3 );
	    pos4 = strcspn( &buffer[pos + pos2 + pos3 + 3], " " );
	    strncpy( nro_nca, &buffer[pos + pos2 + pos3 + 3], pos4 );
	    pos5 = strcspn( &buffer[pos + pos2 + pos3 + pos4 + 4], " " );
	    strncpy( nro_ncb, &buffer[pos + pos2 + pos3 + pos4 + 4], pos5 );
        //nro_factura[pos] = '\0';
		/* si hay mas datos en la cadena final los extraigo IMPRESORA HASAR 1000*/
		if( !( strcmp( buffer, " " ) == 0 ) ) {
			int posini=0,posfin=0;
			char bufferExtrae[200];
			char * aux;
			memset( bufferExtrae, 0, sizeof(bufferExtrae));
			aux = strstr( buffer, "Nro_FacturaB:" );
			if( aux != NULL){ 
				strcpy( bufferExtrae, strstr( buffer, "Nro_FacturaB:" ) );
				posini= strcspn( bufferExtrae, ":" );
				if( posini > 0 ) { 
					posfin = posini + 1 + strcspn( &bufferExtrae[posini + 1], " " ); 
					strncpy( nro_facturab, &bufferExtrae[posini + 1], posfin ); 
					nro_facturab[posfin - posini - 1]= 0;
					facturaB = atol(nro_facturab); 
				}
			}
		}
	#ifndef	COMPILAR_EMULADOR_FISCAL
        ticket = atol(nro_ticket);
	#else
		ticket = NRO_TICKET; //para incrementar el nro de ticket
	#endif COMPILAR_EMULADOR_FISCAL


	#ifndef	COMPILAR_EMULADOR_FISCAL
        factura = atol(nro_factura);
		z = atol(nro_z);
	    nca = atol(nro_nca);
	    ncb = atol(nro_ncb);
	#else
		factura = NRO_TICKET +1; //para incrementar el nro de ticket factura
		z = NRO_Z - 1;
	    nca = NRO_TICKET ;
	    ncb = NRO_TICKET ;
	#endif COMPILAR_EMULADOR_FISCAL

	    

        if( asignar ) {
 			char cad_prn[200];
			char fact, nc;
			
			//Los siguientes comprobantes tiene distintos numeros de ticket
			//para la impresora TM220:
			//Ticket Factura A, Nota Credito A, Nota Credito B y Ticket Comun.
			//El Ticket Factura B posee la misma numeración que Ticket Comun
			fact = RAM_FACTURA;
			nc = RAM_NOTA_CR;
			memset(cad_prn,0,sizeof(cad_prn));
			//La var. "incrementar" es para el caso en que se corte la caja y no la printer
			//La printer devuelve el ultimo nro de comprobante impreso luego la caja incrementa
			//ese nro ocacionandose que se grabe incorrectamente el nro de ticket.
			decrementar = ( NO_INC_NRO )? -1: 0;
			if(RAM_FACTURA == 1 && RAM_NOTA_CR == 1){ // Nota de Credito A
				SET_MEMORY_LONG( __ram_nro_comp, nca + decrementar );
			}else if(RAM_FACTURA == 2 && RAM_NOTA_CR == 1){ // Nota de Credito B
				SET_MEMORY_LONG( __ram_nro_comp, ncb + decrementar );
			}else if(RAM_TICKET_FACTURA == 1){				 // Ticket Factura A
				SET_MEMORY_LONG( __ram_nro_comp, factura + decrementar );
			}else if(RAM_TICKET_FACTURA == 2){				 // Ticket Factura B
				if( facturaB >0 )  //la hasar trae el nro de ticket factura aca
					ticket = facturaB;
			SET_MEMORY_LONG( __ram_nro_comp, ticket + decrementar );
				
			}else if(RAM_FACTURA == 1 && RAM_NOTA_CR == 2){ // Nota de Credito B
				SET_MEMORY_LONG( __ram_nro_comp, ncb + decrementar );
			}else {
				SET_MEMORY_LONG( __ram_nro_comp, ticket + decrementar );
			}
			if( NRO_MODO != 'Z') // en la fiscal no puedo pedir la Z de nuevo porque ya se incremento
				SET_MEMORY_LONG( __nro_z, z+1);
			_snprintf( cad_prn,sizeof(cad_prn)-1, "SISTEMA SINCRONIZADO: TICKET -> %ld, FACTURA -> %ld, NRO DE Z -> %ld, Nota de CREDITO A -> %ld, Nota de CREDITO B -> %ld", ticket, factura, z, nca, ncb );
            GRABAR_LOG_SISTEMA( cad_prn ,LOG_VENTAS,4);
            //Se agrega x q cuando se esta reconectando el JavaServer los contadores son igual a 0
            // y graba el evento en curso con el nro_z incorrecto y no se envia posteriormente el evento
            //a la base ya q no coincide con el nro_z actual.
            //error_nro = ( ticket == 0 && factura == 0 && z == 0 && nca == 0 && ncb == 0 )? 1 : 0;
			error_nro = ( ticket <= 0 && factura <= 0 && z <= 0 && nca <= 0 && ncb <= 0 )? 1 : 0;
        }
		if( buffer )
		free( buffer );
		}else { //fallo el comando
		   if( buffer )
			free( buffer );
			//error_nro = 1;
			//ok = 0;
			//_reintentos++;
			//if( _reintentos > 1 ) 
				break;
		}
		
	}while( error_nro || ticket < 0 || ticket != ticket_ant );

    return( ok );
}

/***************************************************************************/
int COMANDO_LEER_STATUS_PRINTER_FISCAL(int reintentos, char *cajon_abierto)
/***************************************************************************/
{
    int ok = 0;
      /*se usa ok para saber cual es el error
     */
    //char *buffer;
    char fstate[200];
    char pstate[200];
    char buffer[500];
	char * ptr;
//    int index; //indice del error

    //errores relevantes
/*    static char *printerErrors[] = {"OUT_OF_PAPER","RECEIPT_NEAR_END","JOURNAL_NEAR_END","PRINTER_OFFLINE",
                             "PRINTER_ERROR"};
    static char *fiscalErrors[] = {"DOCUMENTO_FISCAL_ABIERTO","DOCUMENTO_NO_FISCAL_ABIERTO","CIERRE_FISCAL_TICKET",
                            "MEMORIA_FISCAL_LLENA","DESBORDAMIENTO_TOTALES","COMANDO_INVALIDO","MEMORIA_FISCAL_CASI_LLENA"};
*/
    int pos = 0;
    int pos2 = 0, repeticiones=3;
//    char *pointer;
//    char *token;

	memset(fstate, 0, sizeof(fstate));
	memset(pstate, 0, sizeof(pstate));
	//EL CAJON SE INDICA COMO CERRADO PARA LAS FISCALES, SI DEVUELVE QUE ESTA ABIERTO CAMBIO EL ESTADO
	_CAJON_POR_IMPRESORA_ABIERTO = NO;
    memset( buffer, 0, 500 );
	INICIA_ESTADOS_IMPRESORA( );
    //obtiene la respuesta de la impresora
    getData( ACTION_GET_FISCAL_STATE, buffer, 0 );
	ptr=strstr( buffer, "PRINTER_OFFLINE" ); //busquemos la printer offline si esta en la respuesta

	while((strcmp( buffer, "Error" ) == 0) || ( ptr && repeticiones > 0) ||
		( ANALISIS_RESPUESTA_IMPRESORA (buffer, sizeof(buffer)) == 1&& GET_ESTADO_FISCAL( _FISCAL_offline ) &&  repeticiones-- )||//no tiene respuesta le damos tiempo
		( ! VERIFICAR_SERIAL_IMPRESORA( buffer, sizeof(buffer) ))){						  //y verificamos el serial de la printer con la respuesta
			if(ptr)
				repeticiones--;
			memset( buffer, 0, 500 );
			INICIA_ESTADOS_IMPRESORA( );
			getData( ACTION_GET_FISCAL_STATE, buffer, 0 ); //buffer envia los items, recibe los estados fiscal/printer;
			glog("Reitento getdata en status Fiscal",LOG_ERRORES,1);
			ptr=strstr( buffer, "PRINTER_OFFLINE" );
	}


    if( strcmp( buffer, "Error" ) == 0 ) {
        ok = -100;
		CARGA_ESTADOS_IMPRESORA( NO, 3 );
        	MENSAJE_CON_PAUSA( ST(S_IMPRESORA_FUERA_LINEA), 26 );
    }
    else if( strcmp( buffer, "  " ) == 0 || strlen( buffer ) == 0 ) {
        ok = -100;
		CARGA_ESTADOS_IMPRESORA( NO, 3 );
    }
	else {
		ok =  ANALISIS_RESPUESTA_IMPRESORA (buffer, sizeof(buffer));
	}
    return( ok );
}

/**************************************************************************/
void FORMATO_CAB_COMPROBANTE_FISCAL( char *texto, int linea, char *dobleancho )
/**************************************************************************/
{
    if( IMPRESORA_FISCAL  || TIPO_IMPRESORA ) {
    char buffer[50];
	CARACTERES_ESPECIALES(texto, _IMPRIMIR_CADENA);
    sprintf( buffer, "%d&%s&%s", linea, texto, dobleancho );
    setData( ACTION_SET_HEADER_LINE, buffer, 0 );
	}
}



/**************************************************************************/
void FORMATO_FIN_COMPROBANTE_FISCAL( char *texto, int linea, char *dobleancho  )
/**************************************************************************/
{
	if( IMPRESORA_FISCAL  || TIPO_IMPRESORA ) {
    char buffer[50];
	memset(buffer,0,50);
	setHabPing(0);
	CARACTERES_ESPECIALES(texto, _IMPRIMIR_CADENA);
    sprintf( buffer, "%d&%s&%s", linea, texto, dobleancho );
    setData( ACTION_SET_TRAILER_LINE, buffer, 0 );
	setHabPing(1);
	}
}
/*****************************************************************************/
void IMPRIME_VOUCHER_FISCAL(char *buffer)
/*****************************************************************************/
{int tam=strlen(buffer);
setData(ACTION_PRINT_VOUCHER, buffer, 0 );
ANALISIS_RESPUESTA_IMPRESORA (buffer, tam);
}
/**************************************************************************/
void AVANZA_PAPEL(int lineas )
/**************************************************************************/
{
	char buffer[5];
    sprintf( buffer, "%d", lineas);
	setData( ACTION_FEED_PAPER, buffer, 0 );
}
/**************************************************************************/
void IMPRIMIR_IMPORTE_FF(int pos, int columna)//Seba
/**************************************************************************/
{
	int sta=1,medio=0, salir=0,i=0;
	char cadena_aux[100], cadena_aux2[100];
	memset( cadena_aux, 0, sizeof(cadena_aux) );
	memset( cadena_aux2, 0, sizeof(cadena_aux2) );
	//Busco el nombre del medio en la posicion
	//SELECT( AREA_PAGO );
	//GO( BOTTOM );
	SELECT_TABLE( T_PAGOS, tipo_t_pagos );
	SET_WHERE("");
	RUN_QUERY(NO);
	GO2( BOTTOM );
	while( /*!BOF()*/ !dbBOF() && !salir)
	{
		if( event_pago.posicion == pos)
		{
			medio=event_pago.modo_pago;
			salir=1;
		}
		SKIP2( -1 );
	}
	if(medio!=0)
	{
		_snprintf( cadena_aux, sizeof(cadena_aux)-1,"%s",medios[medio-1].nombre);
		for(i=strlen(cadena_aux); i<columna; i++) {
			cadena_aux[i]= ' ';
		}
		_snprintf( cadena_aux2, sizeof(cadena_aux2),"%.2lf", _PAGO_IMPORTE( pos ));
		strcat(cadena_aux, cadena_aux2);
		IMPRIMIR_TEXTO_NO_FISCAL(1,LETRA_NORMAL,cadena_aux, SI );//Seba
	}
}
/**************************************************************************/
void IMPRIMIR_IMPORTE_RETIRO(int medio)//Seba
/**************************************************************************/
{
char buff[500];
int sta=1;
char aux[50];
double total=0.0;

memset( buff, 0, sizeof(buff) );
memset( aux, 0, sizeof(aux) );


if( medios[medio].nro && ( fabs( acu_retirado[medio] ) > 0.01 ) )
{
	IMPRIME_CUERPO_RETIROS( NO, medios[medio].nombre, acu_retirado[medio], 0, 0,( acu_retirado_me ) ? acu_retirado_me[medio] : 0.0, medio );
    total += acu_retirado[medio];
}
_snprintf( aux, sizeof(aux)-1,"Medio: %s %s $ %.2lf",medios[medio].nombre, ST( S_Importe ), _X_RETIROS_IMPORTE(medio));
_snprintf( buff, sizeof(buff)-1,"%d&%s",sta , aux);
setData( ACTION_PRINT_NORMAL_FISCAL, buff, 0 );//Seba
}
/**********************************************************************************************/
void IMPRIMIR_TEXTO_NO_FISCAL(int station,int tipo, char *texto, int salto_fila/*, int columna*/  )
/**********************************************************************************************/
{
	char buff[500], buff_aux[500];
	int caracteres = 0, i = 0;
	int largo = strlen( BUFFER_LINEA_PRN );

	memset( buff, 0, sizeof(buff) );
	memset( buff_aux, 0, sizeof(buff_aux) );
	
	//Posiciono X agregando espacios
	if( tkt_columna > largo && tkt_columna > 1 ) {
		memset(buff_aux,' ',(tkt_columna - largo - 1) );
	}
	strncat( &buff_aux, texto, sizeof( buff_aux ) - strlen( buff_aux ) - 1 );
	if( salto_fila )
	{
		//COMANDO_LEER_STATUS_PRINTER_FISCAL(1, NULL);
		


		strncat( &BUFFER_LINEA_PRN[largo], buff_aux, sizeof( BUFFER_LINEA_PRN ) - largo - 1 );
		if( strcmp( BUFFER_LINEA_PRN, "" ) == 0 ) {
			BUFFER_LINEA_PRN[0]=127;// 127 = 7F
		}
		_snprintf( buff, sizeof( buff ) - 1, "%d&%c%s", station, tipo, BUFFER_LINEA_PRN );

		if( RAM_IMPRESION_FISCAL!= 0 ){//se puede evitar el envio del comando si no hay comprobante abierto por lo tanto errores recursivos
			setData( ACTION_PRINT_NORMAL_FISCAL, buff, 0 );//Seba
			//memset( buff, 0, sizeof(buff) );
			if( RAM_IMPRESION_FISCAL == -1 ){
				ANALISIS_RESPUESTA_IMPRESORA (buff, sizeof(buff));
				CONTROL_EN_NO_DOC_FISCALES();	
			}
		}
		//agregamos lo de voucher esta incompleto hay qye seguir desarrollando para la reimpresion


		if( /*GET_COPIA_VOUCHER() == 1 && */RAM_IMPRESION_FISCAL == -1 && IMPRESORA_FISCAL == FISCAL  && VOUCHER_TARJETA_FISCAL == NO ) {
            int handle_voucher = GET_HANDLE_VOUCHER ();//<< ACA LO ESTA GRABANDO TAMBIEN!! GRABAR_VOUCHER_PRN( char *cadena ) {
			memset( buff, 0, sizeof(buff) );
			_snprintf( buff, sizeof( buff ) - 1, "%d&%c%s%c", station, tipo, BUFFER_LINEA_PRN,'\n' );
            _WRITE( handle_voucher, buff, 100, __LINE__, __FILE__ );
        }

		memset( BUFFER_LINEA_PRN ,0 ,sizeof( BUFFER_LINEA_PRN ) );
	}
	else
	{
		strncat( &BUFFER_LINEA_PRN[largo], buff_aux, sizeof( BUFFER_LINEA_PRN ) - largo - 1 );
	}

}



/**********************************************************************************************/
int RE_IMPRESION_VOUCHER_POR_VALIDACION( void )
/**********************************************************************************************/
{int rta = 0;
	if( HABILITA_COPIA_VOUCHER_PRN && access( "voucher.prn", 0 ) == 0 ) {
		int handle =0, salir = 0 ;
		handle = GET_HANDLE_VOUCHER();
		if(handle<=0){
			handle = _OPEN( "voucher.prn",  O_RDWR, __LINE__, __FILE__ );
		}
		if( handle > 0 ){
			char buff[200];
			IMPRIMIR_CADENA( 0, ST( S_______REIMPRESION_______  ) );
			LF(1);
			LSEEK( handle, 0L, SEEK_SET, __LINE__, __FILE__ );
            while( !salir ) {
				memset( buff, 0, sizeof(buff) );
				if( !_READ( handle, ( char* )buff, 1 , __LINE__, __FILE__ ) ) {
					switch(buff[0]){//viene de CONTINUA_COPIA_VOUCHER_PRN()
					case 0x04:
						IMPRIMIR_CADENA( 0, ST( S_______REIMPRESION_______  ) );
						LF(1);
						CERRAR_DOCUMENTO_NO_FISCAL();
						break;
					case 0x01:
						ABRIR_DOCUMENTO_NO_FISCAL( NO );
						IMPRIMIR_CADENA( 0, ST( S_______REIMPRESION_______  ) );
						LF(1);
						break;
					case '\n'://0x0a:
					case 0:
						{
						if(strlen(BUFFER_LINEA_PRN) > 0 ){
							setData( ACTION_PRINT_NORMAL_FISCAL, BUFFER_LINEA_PRN, 0 );
							rta = SI;
							memset( BUFFER_LINEA_PRN, 0, sizeof(BUFFER_LINEA_PRN) );
							}
						}
						break;
					default:
						{
							strcat( &BUFFER_LINEA_PRN, &buff[0] );
						}
						break;
					}

				}else{
					salir = 1;
					if(strlen(BUFFER_LINEA_PRN) > 0 ){
						setData( ACTION_PRINT_NORMAL_FISCAL, BUFFER_LINEA_PRN, 0 );
						memset( BUFFER_LINEA_PRN, 0, sizeof(BUFFER_LINEA_PRN) );
					}
					IMPRIMIR_CADENA( 0, ST( S_______REIMPRESION_______  ) );
					LF(1);
				}
            }
			_CLOSE( handle, __LINE__, __FILE__ );
		}
	}
	return rta;
}




/**************************************************************************/
void ABRIR_NOTA_DE_CREDITO(char formtype, int IvaEmi, int IvaComp, char *name, char *lastname,int docType, char *DocNum, char *address1, char *address2, char *address3, char *remito, char *remito1)//Seba
/**************************************************************************/
{
	char buff[500];
	char ivaemi;
	char ivacomp;
	char docfiscal;
	char doctype[10];


	if(IvaComp == RESP_INSC){
		ivacomp = 'I';
		docfiscal = 'A';
	}else if(IvaComp == NO_RESP){
		ivacomp = 'N';
		docfiscal = 'B';
	}else if(IvaComp == EXENTO){
        ivacomp = 'E';
		docfiscal = 'B';
	}else if(IvaComp == CONS_FINAL){
		ivacomp = 'F';
		docfiscal = 'B';
	}else if(IvaComp == MONOTRIBUTO){
		ivacomp = 'M';
		docfiscal = 'B';
	}else if(IvaComp == RESP_NO_IDEN){
		ivacomp = 'U';
		docfiscal = 'B';}

	if(IvaEmi == RESP_INSC){
		ivaemi = 'I';
		//docfiscal = 'A';
	}else if(IvaEmi == NO_RESP){
		ivaemi = 'N';
		docfiscal = 'B';
	}else if(IvaEmi == EXENTO){
        ivaemi = 'E';
		docfiscal = 'B';
	}else if(IvaEmi == CONS_FINAL){
		ivaemi = 'F';
		docfiscal = 'B';
	}else if(IvaEmi == MONOTRIBUTO){
		ivaemi = 'M';
		docfiscal = 'B';
	}else if(IvaEmi == RESP_NO_IDEN){
		ivaemi = 'U';
		docfiscal = 'B';}

	if(docType == _DNI)
		_snprintf(doctype,sizeof("DNI"),"DNI");
	else if(docType == CUIL)
		_snprintf(doctype,sizeof("CUIL"),"CUIL");
	else if(docType == __CUIT)
		_snprintf(doctype,sizeof("CUIT"),"CUIT");
	else if(docType == CEDULA)
		_snprintf(doctype,sizeof("CEDULA"),"CEDULA");
	else if(docType == PASAPORTE)
		_snprintf(doctype,sizeof("PASAPO"),"PASAPO");
	else if(docType == LIB_CIVICA)
		_snprintf(doctype,sizeof("LIB_CI"),"LIB_CI");
	else if(docType == LIB_ENROLA)
		_snprintf(doctype,sizeof("LIB_EN"),"LIB_EN");

	if(IvaEmi != RESP_INSC)
		docfiscal = 'C';

	// COMPLETO LOS CAMPOS OBLIGATORIOS si estan vacios
	if (strlen(name)==0)
		_snprintf(name, sizeof(name),".");
	if (strlen(lastname)==0)
		_snprintf(lastname, sizeof(lastname),".");
	if (strlen(address1)==0)
		_snprintf(address1, sizeof(address1),".");
	if (strlen(address2)==0)
		_snprintf(address2, sizeof(address2),".");
	if (strlen(address3)==0)
		_snprintf(address3, sizeof(address3),".");
	if (strlen(remito)==0)
		_snprintf(remito, sizeof(remito),".");
	if (strlen(remito1)==0)
		_snprintf(remito1, sizeof(remito1),".");


	memset( buff, 0, sizeof(buff) );
	CARACTERES_ESPECIALES( name, _IMPRIMIR_CADENA );
	CARACTERES_ESPECIALES( lastname, _IMPRIMIR_CADENA );
	CARACTERES_ESPECIALES( address1, _IMPRIMIR_CADENA );
	CARACTERES_ESPECIALES( address2, _IMPRIMIR_CADENA );
	CARACTERES_ESPECIALES( address3, _IMPRIMIR_CADENA );
	//"B&F&I&F&A.T.C. INTERNACIONA&L DE CENTRO&DNI&0&SAN SALVADOR&SAN SALVADOR&.&abc&.&"
	////beginRepaymentNote('A','A','P','I',"Sebastian", "Davalle", "CUIT","20292532238",".",".",".",".",".");
	_snprintf( buff,sizeof(buff)-1, "%c&%c&%c&%c&%s&%s&%s&%s&%s&%s&%s&%s&%s&", docfiscal, formtype, ivaemi, ivacomp, name, lastname, doctype, DocNum,  address1, address2, address3,  remito,  remito1);
	setData( ACTION_BEGIN_REPAYMENT_NOTE, buff, 0 );//Seba
	SET_MEMORY_DOUBLE( __ram_puntos_esp_3, 0.00);
}
/**************************************************************************/
void ABRIR_TIQUE_FACTURA( char formtype, int IvaEmi, char IvaComp, char *name, char *lastname,int docType, char *DocNum, char *address1, char *address2, char *address3, char *remito, char *remito1)//Seba
/**************************************************************************/
{
	char buff[500];
	char ivaemi;
	char ivacomp;
	char docfiscal;
	char doctype[10];
	int  rta;


	if(IvaComp == RESP_INSC){
		ivacomp = 'I';
		docfiscal = 'A';
	}else if(IvaComp == NO_RESP){
		ivacomp = 'N';
		docfiscal = 'B';
	}else if(IvaComp == EXENTO){
        ivacomp = 'E';
		docfiscal = 'B';
	}else if(IvaComp == CONS_FINAL){
		ivacomp = 'F';
		docfiscal = 'B';
	}else if(IvaComp == MONOTRIBUTO){
		ivacomp = 'M';
		docfiscal = 'B';
	}else if(IvaComp == RESP_NO_IDEN){
		/*para fiscal tmu220 debe venir con cuil para que no de error*/	
		//ivacomp = 'U';
		ivacomp = 'S';
		docfiscal = 'B';}
	if(IvaEmi == RESP_INSC){
		ivaemi = 'I';
		//docfiscal = 'A';
	}else if(IvaEmi == NO_RESP){
		ivaemi = 'N';
		docfiscal = 'B';
	}else if(IvaEmi == EXENTO){
        ivaemi = 'E';
		docfiscal = 'B';
	}else if(IvaEmi == CONS_FINAL){
		ivaemi = 'F';
		docfiscal = 'B';
	}else if(IvaEmi == MONOTRIBUTO){
		ivaemi = 'M';
		docfiscal = 'B';
	}else if(IvaEmi == RESP_NO_IDEN){
		ivaemi = 'U';
		docfiscal = 'B';}

	if(docType == _DNI)
		_snprintf(doctype,sizeof("DNI"),"DNI");
	else if(docType == CUIL)
		_snprintf(doctype,sizeof("CUIL"),"CUIL");
	else if(docType == __CUIT)
		_snprintf(doctype,sizeof("CUIT"),"CUIT");
	else if(docType == CEDULA)
		_snprintf(doctype,sizeof("CEDULA"),"CEDULA");
	else if(docType == PASAPORTE)
		_snprintf(doctype,sizeof("PASAPO"),"PASAPO");
	else if(docType == LIB_CIVICA)
		_snprintf(doctype,sizeof("LIB_CI"),"LIB_CI");
	else if(docType == LIB_ENROLA)
		_snprintf(doctype,sizeof("LIB_EN"),"LIB_EN");

	if(IvaEmi != RESP_INSC)
		docfiscal = 'C';

	// COMPLETO LOS CAMPOS OBLIGATORIOS si estan vacios
	//en el TF permanente aqui podemos definir que campo queremos aunque de un grupo si o si uno tiene que ir
	//Lo manejamos por campo de bits
	/*
		1=2do nombre  lastname
		2= direccion 1( direccion)  address1
		4= direccon 2(localidad) address2
		8 = direccion 3 (provincia) address3
		16= remito 1 remito
		32= remito 2 remito1  
		Ej Para el caso actual que solo esta la provicnia el nro seria 8
	*/
  if(config_tps.activarTFpermanente == 1) {
	  CARGAR_DATOS_TF(name,lastname,address1,address2,address3,remito,remito1);
  } else {
	if (strlen(name)==0)
		_snprintf(name, sizeof(name),".");
	if (strlen(lastname)==0)
		_snprintf(lastname, sizeof(lastname),".");
	if (strlen(address1)==0)
		_snprintf(address1, sizeof(address1),".");
	if (strlen(address2)==0)
		_snprintf(address2, sizeof(address2),".");
	if (strlen(address3)==0)
		_snprintf(address3, sizeof(address3),".");
	if (strlen(remito)==0)
		_snprintf(remito, sizeof(remito),".");
	if (strlen(remito1)==0)
		_snprintf(remito1, sizeof(remito1),".");
  }
	memset( buff, 0, sizeof(buff) );
	CARACTERES_ESPECIALES( name, _IMPRIMIR_CADENA );
	CARACTERES_ESPECIALES( lastname, _IMPRIMIR_CADENA );
	CARACTERES_ESPECIALES( address1, _IMPRIMIR_CADENA );
	CARACTERES_ESPECIALES( address2, _IMPRIMIR_CADENA );
	CARACTERES_ESPECIALES( address3, _IMPRIMIR_CADENA );
	_snprintf( buff,sizeof(buff)-1, "%c&%c&%c&%c&%s&%s&%s&%s&%s&%s&%s&%s&%s&", docfiscal, formtype, ivaemi, ivacomp , name, lastname, doctype, DocNum,  address1, address2, address3,  remito,  remito1);
	rta = COMANDO_LEER_STATUS_PRINTER_FISCAL( 1 , NULL );
	if( GET_ESTADO_FISCAL(_FISCAL_doc_no_fiscal_abierto ) ) { //hay un documento NO fiscal abierto
		char buffer_aux[20];
		memset(buffer_aux,0,sizeof(buffer_aux));
		sprintf(buffer_aux,"&1&");
		setData( ACTION_UPDATE_ESTADO_DOC_NO_FISCAL,buffer_aux, 20 );
		CERRAR_DOCUMENTO_NO_FISCAL();
		rta = COMANDO_LEER_STATUS_PRINTER_FISCAL( 1 , NULL );
	}

	if(GET_ESTADO_FISCAL(_FISCAL_doc_fiscal_abierto)) { //hay un documento  fiscal abierto
		CERRAR_DOCUMENTO_FISCAL();
	}
	
	setData( ACTION_BEGIN_FAC, buff, 0 );//Seba
	SET_MEMORY_DOUBLE( __ram_puntos_esp_5, 0.00);
	SET_MEMORY_DOUBLE( __ram_puntos_esp_4, 0.00);
	SET_MEMORY_DOUBLE( __ram_puntos_esp_3, 0.00);
}
/**************************************************************************/
int IMPRIME_ITEM_FAC(char *desc, double importe, double impuestos_internos, double cantidad )//Seba
/**************************************************************************/

{
    int rta = 0, rta_fiscal = 0;
    int reintentos = 0;
    /*
    * Si hay un error devuelve DEBE_ANULAR_TICKET
    */
    #if defined(INVEL_W) || defined(INVEL_L)
    char buff[500];
	char tipo;
    char cprecio[20];
	char cimp_int[20];

	char descripExtra1[30];
	char descripExtra2[30];
	char descripExtra3[30];

    char unidad[20];
    char civa[20];
    //   double cant;
    int signo = 0;
    int dec = 0;
	int borrar = NO;
	double aux_cantidad =0.0;
	double cantidad2 = 0.00;
	/*  es conveniente inicializar las variables.!!!!!!*/
	memset(buff,0,sizeof(buff));
	
	
	memset(cprecio,0,sizeof(cprecio));
	memset(cimp_int,0,sizeof(cimp_int));
	memset(unidad,0,sizeof(unidad));
	memset(civa,0,sizeof(civa));
	/****************************************************/

	//String description, long price, int quantity, int vatInfo, String extra, String extra1, String extra2, long impint
	strcpy( civa, fcvt( ( ITEM_GRAVADO )?(iva[ITEM_COD_IVA].porc_iva1 + iva[ITEM_COD_IVA].porc_iva2 ): 0, DECIMALES, &dec, &signo ) );
	
	memset(descripExtra1,0, sizeof(descripExtra1));
	memset(descripExtra2,0, sizeof(descripExtra2));
	memset(descripExtra3,0, sizeof(descripExtra3));

	if(( RAM_TICKET_FACTURA || RAM_NOTA_CR ) &&
			( ITEM_ORIGEN == _ORIGEN_PROMOCION || ITEM_ORIGEN == _ORIGEN_DESC_PORC
			|| ITEM_ORIGEN == _ORIGEN_DESC_PESOS|| ITEM_COD_MOVIMIENTO == _DIFERENCIA_PRECIO ))  {
				double difo;
				double auxiliar = importe;
				//para los importe tipo 8.9549999 el redondeo a 2 no funciona
				ROUND2(&auxiliar,ENTEROS,2);
				difo =fabs( auxiliar - importe );
				if( difo > 0.0049 )
					ROUND2(&importe,ENTEROS,4);
				else
		ROUND2(&importe,ENTEROS,2);
	}
	if( RAM_NOTA_CR == 2 && RAM_IMPORTACION_PEDIDOS ) { //solo hago este control para  la NC total B conanulacion de tarjetas
		//analicemos si tenemos que redondea
		char cprecio2[20];
		int uno;
		double parcial= importe;
		memset(cprecio2,0,sizeof(cprecio));
		strcpy( cprecio2, fcvt( importe, DECIMALES +2  , &dec, &signo ) );
		uno = strlen(cprecio2) - 1; 
		// para un importe de 0.8264 sin iva con iva da 0.999944 y se manda a la impresoa 0.9999 y no 1
		if(cprecio2[uno] != '0' && cprecio2[--uno] != '0')
			importe = importe + ( (importe > 0.00)? 0.00005:-0.00005) ; //correcion

	}	
   
	tipo = ( RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1 )?'A':'B';
	strcpy( cprecio, fcvt( importe, DECIMALES +2 , &dec, &signo ) );
	strcpy( cimp_int, fcvt( impuestos_internos, DECIMALES, &dec, &signo ) );
    strcpy( unidad, fcvt( importe, DECIMALES, &dec, &signo ) );
	CARACTERES_ESPECIALES(desc, _IMPRIMIR_CADENA );

	do {
        reintentos++;
		//Las notas de credito son documentos no fiscales homologados
		#ifdef COMPILAR_EMULADOR_FISCAL
		   rta_fiscal = SI;
		#else
		if( RAM_P_TICKET == 1 ) {
			if( IMPRESORA_FISCAL == FISCAL ) {
				COMANDO_LEER_STATUS_PRINTER_FISCAL( 1, NULL  );
				rta_fiscal = CONTINUO_IMPRESION ( rta_fiscal ) ;
			}
		} else {
			 if ( IMPRESORA_FISCAL == FISCAL )
				rta_fiscal = SI; //para acelerar la impresion
		}

		#endif COMPILAR_EMULADOR_FISCAL

		if ( RAM_TICKET_FACTURA || RAM_NOTA_CR ) {
			if( rta_fiscal == SI ) {
		if( IMPRESORA_FISCAL ){
			IMPRIME_TKT_DESCRIPCION_CUERPO( 1 );
			DEVUELVE_LINEA_DESCRIPCION(descripExtra1, 0, sizeof(descripExtra1)-1);
			DEVUELVE_LINEA_DESCRIPCION(descripExtra2, 1, sizeof(descripExtra2)-1);
			DEVUELVE_LINEA_DESCRIPCION(descripExtra3, 2, sizeof(descripExtra3)-1);

		}
		aux_cantidad = cantidad;
		if( cantidad < 0 ) {
				cantidad = cantidad * (-1);
				memset( buff, 0, sizeof(buff) );
				//EL ( ROUND(cantidad,6,3)*1000 ) produce error en el CMD que se envia a la printer FISCAL.
				//Se hace el mismo calculo pero por pasos porque en forma directa da error
				//ej cantidad= 0.186, termina casteando en 0.185
				ROUND2(&cantidad,6,3);
				cantidad2 = (cantidad * 1000);
				ROUND2(&cantidad2,6,3);
				_snprintf(buff,sizeof(buff)-1, "%s&%s&%d&%s&%s&%s&%s&%s&%c&",
					desc, cprecio, (int)cantidad2/*(ROUND2(&cantidad,6,3)*1000)*/,
					civa,
					((strlen(descripExtra1)>1)?descripExtra1:"."),
					((strlen(descripExtra2)>1)?descripExtra2:"."),
					((strlen(descripExtra3)>1)?descripExtra3:"."),
					 cimp_int/*(int)(impuestos_internos * 100)*/,
					 tipo);
				//rta = setData( ACTION_PRINT_REC_VOID_ITEM_FAC, buff, 0 );
				if( RAM_NOTA_CR 
					&& ITEM_ORIGEN == _ORIGEN_VARIACION_MEDIO ) {
					if( importe < 0 )
						importe *= -1;
				}

				if(( RAM_TICKET_FACTURA || RAM_NOTA_CR ) 
						&& ( (ITEM_ORIGEN == _ORIGEN_VARIACION_MEDIO && importe < 0 )
							|| ITEM_ORIGEN == _ORIGEN_PROMOCION 
							|| ITEM_ORIGEN == _ORIGEN_DESC_PORC
							|| ITEM_ORIGEN == _ORIGEN_DESC_PESOS 
							|| ITEM_COD_MOVIMIENTO == _DIFERENCIA_PRECIO ) )
					rta = COMANDO_PRINT_REC_FISCAL( ACTION_PRINT_ANUL_BONIF_ITEM_FAC, buff, 0 ); //nueva modalidad codigo R
				else
					rta = COMANDO_PRINT_REC_FISCAL( ACTION_PRINT_REC_VOID_ITEM_FAC, buff, 0 ); //nueva modalidad m
		} else {
				int devolucion_envase = 0; 
				/*
							
				ref #5249 Inconsistencia fiscal en ticket con envases. Se da cuando erroneamente
				se tickea un articulo devolucion de envase ( esta con signo negativo en el padron).
				Si bien no es correcto ya que se debe utilzar la tecla devolucion de envase (/) o a lo sumo
				anular el envase y no tickear este articulo.
				
				//creado por si quieren usar un item devolucion de envase que en el padron este negativo
*/

				if( ((importe < 0 && RAM_NOTA_CR == 0 && articulo.dev_envase =='S' ) || (importe > 0 && RAM_NOTA_CR > 0 && articulo.dev_envase =='S' )) 
					&& !(( ITEM_ORIGEN == _ORIGEN_PROMOCION || ITEM_ORIGEN == _ORIGEN_DESC_PORC
					|| ITEM_ORIGEN == _ORIGEN_DESC_PESOS|| ITEM_COD_MOVIMIENTO == _DIFERENCIA_PRECIO ))){
					devolucion_envase = 1;
				}
				
				memset( buff, 0, sizeof(buff) );
				//Se hace el mismo calculo pero por pasos porque en forma directa da error
				//ej cantidad= 0.186, termina casteando en 0.185
				ROUND2(&cantidad,6,3);
				cantidad2 = (cantidad * 1000);
				ROUND2(&cantidad2,6,3);

//int vatInfo, String extra, String extra1, String extra2

				_snprintf( buff, sizeof(buff)-1, "%s&%s&%d&%s&%s&%s&%s&%s&%c&",
					desc, cprecio, (int)cantidad2,
					civa,
					((strlen(descripExtra1)>1)?descripExtra1:"."),
					((strlen(descripExtra2)>1)?descripExtra2:"."),
					((strlen(descripExtra3)>1)?descripExtra3:"."),
					cimp_int/*(int)(impuestos_internos * 100)*/,
					tipo);
				
				if( RAM_NOTA_CR 
					&& ITEM_ORIGEN == _ORIGEN_VARIACION_MEDIO ) {
						importe *= -1;
				}
				
				if(( RAM_TICKET_FACTURA || RAM_NOTA_CR ) 
						&& ( ( ITEM_ORIGEN == _ORIGEN_VARIACION_MEDIO && importe < 0 )
							|| ITEM_ORIGEN == _ORIGEN_PROMOCION 
							|| ITEM_ORIGEN == _ORIGEN_DESC_PORC
							|| ITEM_ORIGEN == _ORIGEN_DESC_PESOS
							|| ITEM_COD_MOVIMIENTO == _DIFERENCIA_PRECIO ) 
							|| devolucion_envase == 1) //hmm
                    // rta = setData( ACTION_PRINT_BONIF_ITEM_FAC, buff, 0 ); //hmm
                    rta = COMANDO_PRINT_REC_FISCAL( ACTION_PRINT_BONIF_ITEM_FAC, buff, 0 ); //nueva modalidad codigo R
                else //hmm  desarrollo de bonificacion TFA*/
					//rta = setData( ACTION_PRINT_REC_ITEM_FAC, buff, 0 );
					rta = COMANDO_PRINT_REC_FISCAL( ACTION_PRINT_REC_ITEM_FAC, buff, 0 ); //nueva modalidad codigo  M

			}
			rta = CONTINUO_IMPRESION ( rta );
		} else {
			//if( rta_fiscal == _FISCAL_ok )//no tiene abierto el comprobante
				rta = DEBE_ANULAR_TICKET;
		}
	}
		if( rta != 1 ) {
			if( GET_ESTADO_FISCAL(_FISCAL_desborde_totales)
					|| GET_ESTADO_FISCAL(_FISCAL_comando_invalido) 
					|| GET_ESTADO_FISCAL(_FISCAL_mem_llena)
					|| GET_ESTADO_FISCAL(_FISCAL_printer_error)
					|| GET_ESTADO_FISCAL(_FISCAL_nec_cierre) ) {

				rta = DEBE_ANULAR_TICKET;
				break;
			} else {
				rta= -1 ; //reintento
				borrar = SI;
				cantidad = aux_cantidad ; 
				if( reintentos > 1 ) {
				MENSAJE( ST(S_ERROR_IMPRESION_REVISE_IMPRESORA_P_E) );
				do{
					if( LASTKEY()!=-999 ){//que no muestre tantas veces.. 
						MENSAJE( ST(S_ERROR_IMPRESION_REVISE_IMPRESORA_P_E) );
					}
				} while( GETCH() != 13 );
				}
			}
		}
	}
    while( ( rta == 207 || rta == -1 )&& reintentos < 10 );   // Jpos error wrong state
    
		if( borrar == SI)
			BORRAR_MENSAJE();

	if( !CONTINUO_IMPRESION( rta ) &&  reintentos )
		rta = DEBE_ANULAR_TICKET;
		
    
	

    /*
    * En rta esta la respuesta de Jpos.Verificar si el estado es correcto, si no abrir documento
    * si no se puede anular ticket.
    */
    #else
    #endif

    return rta;
}
/**************************************************************************/
int CERRAR_TIQUE_FACTURA(int IvaEmi, int IvaComp)//Seba
/**************************************************************************/
{
    int rta = 1, rta_fiscal = 0;
    #if defined(INVEL_W) || defined(INVEL_L)
	char ivaemi;
	char ivacomp;
	char docfiscal;
	char buff[100] ;


	if(IvaComp == RESP_INSC){
		ivacomp = 'I';
		docfiscal = 'A';
	}else if(IvaComp == NO_RESP){
		ivacomp = 'N';
		docfiscal = 'B';
	}else if(IvaComp == EXENTO){
        ivacomp = 'E';
		docfiscal = 'B';
	}else if(IvaComp == CONS_FINAL){
		ivacomp = 'F';
		docfiscal = 'B';
	}else if(IvaComp == MONOTRIBUTO){
		ivacomp = 'M';
		docfiscal = 'B';
	}else if(IvaComp == RESP_NO_IDEN){
		ivacomp = 'U';
		docfiscal = 'B';}

	if(IvaEmi == RESP_INSC){
		ivaemi = 'I';
		//docfiscal = 'A';
	}else if(IvaEmi == NO_RESP){
		ivaemi = 'N';
		docfiscal = 'B';
	}else if(IvaEmi == EXENTO){
        ivaemi = 'E';
		docfiscal = 'B';
	}else if(IvaEmi == CONS_FINAL){
		ivaemi = 'F';
		docfiscal = 'B';
	}else if(IvaEmi == MONOTRIBUTO){
		ivaemi = 'M';
		docfiscal = 'B';
	}else if(IvaEmi == RESP_NO_IDEN){
		ivaemi = 'U';
		docfiscal = 'B';}

	rta_fiscal = COMANDO_LEER_STATUS_PRINTER_FISCAL( 1, NULL );
	if( GET_ESTADO_FISCAL(_FISCAL_doc_fiscal_abierto )|| CONTINUO_IMPRESION( rta_fiscal) ) {
		int intentos = 3;
		/*Original
		rta = COMANDO_PRINT_REC_FISCAL( ACTION_END_FAC, buff, 0 ); //nueva modalidad
		rta =ANALIZAR_RESPUESTA_FISCAL (ACTION_END_FAC, buff, rta );*/
		do{
			if( GET_ESTADO_FISCAL(_FISCAL_doc_fiscal_abierto ) ){
				memset( buff, 0, sizeof(buff) );
				_snprintf(buff,sizeof(buff)-1,"%c",docfiscal);
				SET_MEMORY_INT( __ram_impresion_comprobante, _COMPROBANTE_PRE_CERRADO );
				rta = COMANDO_PRINT_REC_FISCAL( ACTION_END_FAC, buff, 0 ); //envio el cierre
			}
			
			if( rta !=_FISCAL_ok ){//salio algo mal ?? 
				COMANDO_LEER_STATUS_PRINTER_FISCAL( 1 , NULL );
				rta = CONTROL_EN_NO_DOC_FISCALES();
			}
			if( GET_ESTADO_FISCAL(_FISCAL_doc_fiscal_abierto ) )
				rta = 0;
		}while(  rta !=_FISCAL_ok && rta != SI && (--intentos) > 0  );
		if( rta )//Seteo un estado intermedio para identicar que el comprobante se cerro pero aun 
			//las variables ram no fueron cereadas. Todo esto es para cuando ocurre un corte de luz
			//Le resto un valor grande negativamente al valor actual para luego recuperar su valor 
			//sumando la diferencia restada.
			SET_MEMORY_INT( __ram_impresion_comprobante, _COMPROBANTE_POS_CERRADO );

	}
	if( rta == 0 )// HUbo algun error!! sigo el codigo 16 para la inconsistencias
		rta = 16;
	
    #else
    #endif
    return( rta );
}
/**************************************************************************/
int CERRAR_NOTA_DE_CREDITO(int IvaEmi, int IvaComp)//Seba
/**************************************************************************/
{
     int rta = 1, rta_fiscal = 0;
	char buff[2];
    #if defined(INVEL_W) || defined(INVEL_L)
	char ivaemi;
	char ivacomp;
	char docfiscal;

	if(IvaComp == RESP_INSC){
		ivacomp = 'I';
		docfiscal = 'A';
	}else if(IvaComp == NO_RESP){
		ivacomp = 'N';
		docfiscal = 'B';
	}else if(IvaComp == EXENTO){
        ivacomp = 'E';
		docfiscal = 'B';
	}else if(IvaComp == CONS_FINAL){
		ivacomp = 'F';
		docfiscal = 'B';
	}else if(IvaComp == MONOTRIBUTO){
		ivacomp = 'M';
		docfiscal = 'B';
	}else if(IvaComp == RESP_NO_IDEN){
		ivacomp = 'U';
		docfiscal = 'B';}

	if(IvaEmi == RESP_INSC){
		ivaemi = 'I';
		//docfiscal = 'A';
	}else if(IvaEmi == NO_RESP){
		ivaemi = 'N';
		docfiscal = 'B';
	}else if(IvaEmi == EXENTO){
        ivaemi = 'E';
		docfiscal = 'B';
	}else if(IvaEmi == CONS_FINAL){
		ivaemi = 'F';
		docfiscal = 'B';
	}else if(IvaEmi == MONOTRIBUTO){
		ivaemi = 'M';
		docfiscal = 'B';
	}else if(IvaEmi == RESP_NO_IDEN){
		ivaemi = 'U';
		docfiscal = 'B';}
	rta_fiscal = COMANDO_LEER_STATUS_PRINTER_FISCAL( 1, NULL );
	if(  DOCUMENTO_ABIERTO( ) || CONTINUO_IMPRESION( rta_fiscal) ) {
		int intentos = 3;
		do{
			if( DOCUMENTO_ABIERTO( ) ) {
				memset( buff, 0, sizeof(buff) );
				_snprintf(buff,sizeof(buff)-1,"%c",docfiscal);
				SET_MEMORY_INT( __ram_impresion_comprobante, _COMPROBANTE_PRE_CERRADO );
				rta = COMANDO_PRINT_REC_FISCAL( ACTION_END_REPAYMENT_NOTE, buff, 0 ); //envio el cierre
			}
			
			if( rta !=_FISCAL_ok ){//salio algo mal ?? 
				COMANDO_LEER_STATUS_PRINTER_FISCAL( 1 , NULL );
				rta = CONTROL_EN_NO_DOC_FISCALES();
			}
			if(  DOCUMENTO_ABIERTO( ) == 1 )
				rta = 0;
		}while(  rta !=_FISCAL_ok && rta != SI && (--intentos) > 0  );
		if( rta )//Seteo un estado intermedio para identicar que el comprobante se cerro pero aun 
			//las variables ram no fueron cereadas. Todo esto es para cuando ocurre un corte de luz
			//Le resto un valor grande negativamente al valor actual para luego recuperar su valor 
			//sumando la diferencia restada.
			SET_MEMORY_INT( __ram_impresion_comprobante, _COMPROBANTE_POS_CERRADO );
	}

	if( rta == 0 )// HUbo algun error!! sigo el codigo 16 para la inconsistencias
		rta = 16;


    #else
    #endif
    return( rta );
}
/**************************************************************************/
void CEREAR_PIE_FISCAL( )
/**************************************************************************/
{
	FORMATO_FIN_COMPROBANTE_FISCAL( "", 11, "false" );
	FORMATO_FIN_COMPROBANTE_FISCAL( "", 12, "false" );
	FORMATO_FIN_COMPROBANTE_FISCAL( "", 13, "false" );
	FORMATO_FIN_COMPROBANTE_FISCAL( "", 14, "false" );
	FORMATO_FIN_COMPROBANTE_FISCAL( "", 15, "false" );
	FORMATO_FIN_COMPROBANTE_FISCAL( "", 16, "false" );

}

/*
hmm
Función que envia item a impresora fiscal y toma la respuesta
para que otra función analice los estados fiscales y de la printer.
*/
/**************************************************************************/
int COMANDO_PRINT_REC_FISCAL( int action, char *buffer, int len )
/**************************************************************************/

{
    int rta =0, ok;
    char fstate[200];
    char pstate[200];
//    int index; //indice del error

    int pos = 0;
    int pos2 = 0;
//    char *pointer;
//    char *token;
//inicializar fstate y pstate:
	INICIA_ESTADOS_IMPRESORA( );
    memset(fstate, 0, sizeof(fstate));
	memset(pstate, 0, sizeof(pstate));
	rta = setData( action, buffer, 0 ); //buffer envia los items, recibe los estados fiscal/printer
	
	if( strcmp( buffer, "Error" ) == 0 ) {
        ok = -100;
        MENSAJE_CON_PAUSA( "IMPRESORA FUERA DE LINEA", 26 );
    }
	else if( strcmp( buffer, "  " ) == 0 || strlen( buffer ) == 0 ) {
        ok = -100;
		if( rta == 16 || rta == 15) //error grave
			ok = rta;
    }
	else {
		if( PROCESAR_RESPUESTA( action) ) {
			if( rta != -1 ) {
				if( action == ACTION_PRINT_ANUL_BONIF_ITEM_FAC && ( rta == 7 || rta == 1 || rta == 6) ) {
					//para este comando el jserver no devuelve el estado de error
					CARGA_ESTADOS_IMPRESORA( NO, 3 );
				    ok = _FISCAL_ok;
				} else
				ok =  ANALISIS_RESPUESTA_IMPRESORA (buffer, sizeof(buffer));
				if( GET_ESTADO_FISCAL( _FISCAL_offline ) && rta == 1 ) { //Tarea 4533 
					// rta = 1 da cuando la printer esta apagada (jserver -> Exception Number: 1)
					// El mensaje es buffer = PrinterState:PRINTER_OFFLINE  FiscalState:   
					// Esta condicion es para cuando la impresora esta apagada y el jserver
					// esta OK
					ok = -1;
				}
			}else { // -1 la respuesta, no hay respuesta del jserver
				 ok = _FISCAL_offline;
				MENSAJE_CON_PAUSA( "IMPRESORA FUERA DE LINEA", 26 );
			}
			if( ok == -100 || GET_ESTADO_FISCAL(_FISCAL_doc_fiscal_abierto ) ) {
				ok = _FISCAL_ok;
			}
	  } else
          if( rta != 16 && rta != 15 && rta != 7 )
				ok = _FISCAL_ok;
		  else
			  ok = rta;
	}
    //free( buffer );
    return( ok );
}

/**************************************************************************/
int ANALIZAR_RESPUESTA_FISCAL( int action, char *buffer, int rta_fiscal )
/**************************************************************************/

{
	int rta = 1;
	int reintentos = 0;

	if( rta_fiscal != 16 && rta_fiscal != 15 &&  rta_fiscal != _FISCAL_ok &&  rta_fiscal != -100 )  {
		do {
			if( CONTINUO_IMPRESION ( rta_fiscal ) )
				//aca continuo
				   rta= 1;
			 else
				if(		GET_ESTADO_FISCAL(_FISCAL_desborde_totales )
					||  GET_ESTADO_FISCAL(_FISCAL_comando_invalido )
					||  GET_ESTADO_FISCAL(_FISCAL_mem_llena )
					||  GET_ESTADO_FISCAL(_FISCAL_printer_error )
					||  GET_ESTADO_FISCAL(_FISCAL_nec_cierre )) {
						rta = DEBE_ANULAR_TICKET;
				break;
				   } else {
					  rta= -1 ; //reintento
					  MENSAJE_TECLA( ST(S_ERROR_IMPRESION_ENTER) );
					  if( rta_fiscal == 0 )
						  rta = 1;
			}

		reintentos++;
		if( rta == 1 ) //continuo
			rta_fiscal = COMANDO_PRINT_REC_FISCAL( action, buffer, 0 );
		}

		while(( rta == 207 || rta == -1 )&& reintentos < 3 );   // Jpos error wrong state

		if(( rta == 207 && reintentos == 3 )|| rta_fiscal == 16 ) {
			rta = DEBE_ANULAR_TICKET;
		}
	}
	if( rta_fiscal == 16 || rta_fiscal == 15 ) //comando no valido para estado fiscal
		rta = rta_fiscal;

	return rta;
}


/*
hmm
Función que indica si se tiene que seguie la impresion o no .
*/
/**************************************************************************/
int CONTINUO_IMPRESION ( int rta_fiscal )
/**************************************************************************/

{
   #define CONTINUO 1
   #define ANULO 0

   int rta = CONTINUO, i = 0;
   //GET_ESTADO_FISCAL(
   for( i = _FISCAL_estado_final; i < _FISCAL_estado_inicial && rta != ANULO ; i++ ){
	   if( GET_ESTADO_FISCAL( i ) ){
			switch( i ) {
		  case  _FISCAL_ok:                      //1
			 rta = CONTINUO ;
		  break;
		  case  _FISCAL_offline:                 //0
			 rta = ANULO ;
		  break;
		  case  _FISCAL_paper_out:              //-1
			 rta = ANULO ;
		  break;
		  case  _FISCAL_printer_error:          //-2
			 rta = ANULO ;
		  break;
		  case  _FISCAL_doc_fiscal_abierto:     //-3
			 rta = CONTINUO ;
		  break;
		  case  _FISCAL_doc_no_fiscal_abierto:  //-4
			 rta = CONTINUO ;
		  break;
		  case  _FISCAL_nec_cierre:             //-5
			 rta = ANULO ;
		  break;
		  case  _FISCAL_mem_llena:              //-6
			 rta = ANULO ;
		  break;
		  case  _FISCAL_desborde_totales:       //-7
			 rta = ANULO ;
		  break;
		  case  _FISCAL_comando_invalido:       //-8
			 rta = ANULO ;
		  break;
		  case  _FISCAL_paper_near_end:         //-9
			 rta = CONTINUO ;
		  break;
		  case  _FISCAL_mem_casi_llena:         //-10
			 rta = CONTINUO ;
		  break;
		  case  _PRN_paper_out:         //2
			 rta = CONTINUO ;
		  break;
		  default:
			   {
				char mensa[33];sprintf(mensa,"ERROR FISCAL DESCONOCIDO %i",rta_fiscal);
				glog(mensa, LOG_ERRORES,2);}
				rta = CONTINUO ;
		   break;
			}
	   }
	   
   }
   
   return rta;

}

/*
hmm
Función que indica si se tiene que seguie la impresion o no .
*/
/**************************************************************************/
int PROCESAR_RESPUESTA( int action )
/**************************************************************************/
{
	int procesar = SI;
	switch(action) {
		case ACTION_END_FISCAL_REC:
		case ACTION_END_FAC:
			procesar = NO;
			break;
	}

 return procesar;
}

/**************************************************************************/
FISCAL_TOTAL_COMPROBANTE( int final )
/**************************************************************************/
{int       rta = 0, ok = SI, intentos = 0, pos = 0;

	if( IMPRESORA_FISCAL ){
		char mens[100];
		double    dif=0.0, total=0.0,total_fiscal=0.00,total_a_controlar=0.0, diffe =0.00;
		int rta_f;
		memset(mens,0,sizeof(mens));
		ok = NO;
	
		rta_f =PIDE_SUBTOTAL( &total_fiscal );
#ifdef COMPILAR_EMULADOR_FISCAL
		total_fiscal = 1.00;
#endif COMPILAR_EMULADOR_FISCAL
		if( total_fiscal > 0 ){
			dif = ( final ) ? 0.16 : 0.11;
			total = ( total_a_controlar > 0.005 ) ? total_a_controlar : RAM_TOTAL;
#ifdef COMPILAR_EMULADOR_FISCAL
			total_fiscal = total;
#endif COMPILAR_EMULADOR_FISCAL
			if( fabs( fabs(total) - fabs(total_fiscal) ) < dif )
				ok = 1;
            // si la diferencia es de 0.01 a favor de la caja le mandamos el 0.01 para ajusta
			ROUND2(&total, ENTEROS,2);
			ROUND2(&total_fiscal, ENTEROS,2);
			diffe = fabs(total)  - total_fiscal;
			if( fabs(diffe) > 0.009 && fabs(diffe) < 0.011 && ( RAM_TICKET_FACTURA > 0 || RAM_NOTA_CR > 0 )) { //porque no se puede redondear total_fiscal
				char gravado = ITEM_GRAVADO, pedir_precio = ITEM_PEDIR_PRECIO, item_ori = ITEM_ORIGEN;
				int signo =1;
				SET_MEMORY_CHAR( __item_pedir_precio, NO );//Fuerza que no sea un pedir precio
				
				//si hay exentos puedo restar sino no
				if( total_fiscal > total && ( RAM_MODO_DEVOLUCION != _NC_TOTAL)) {
					SET_MEMORY_CHAR( __item_origen, _ORIGEN_DESC_PESOS );
					if( fabs(TOT_TICK_EXENTO) > 0.01 ) {
						SET_MEMORY_CHAR( __item_gravado, NO );//Fuerza el descarga a EXENTO
						//IMPRIME_ITEM_FAC("REDONDEO FISCAL", 0.01, 0.00, signo * 1.00  ); //lo sacamos
					}
				}
				
				//IMPRIME_ITEM_FAC("REDONDEO FISCAL", 0.01, 0.00, signo * 1.00  );
				SET_MEMORY_CHAR( __item_gravado, gravado );
				SET_MEMORY_CHAR( __item_pedir_precio, pedir_precio );
				SET_MEMORY_CHAR( __item_origen, item_ori );

			}



			if( !final || !ok ) {
				sprintf( mens, "Totales: %f %f", total, total_fiscal );				
			} else {
				sprintf( mens, "RamTot: %f TotFis: %f", total, total_fiscal );		
			}
			GRABAR_LOG_SISTEMA( mens, LOG_VENTAS,1);
		}
	}
	return ( ok );
}
/**************************************************************************/
void INICIA_ESTADOS_IMPRESORA( void ){
/**************************************************************************/

/*static struct{
	char e_impresora[17];
	char e_fiscal[17];
}ESTADO_INMPRESORA;*/
	memset(&ESTADO_IMPRESORA,0, sizeof(ESTADO_IMPRESORA));
}

/**************************************************************************/
void CARGA_ESTADOS_IMPRESORA( int e_fiscal, char posicion ){
/**************************************************************************/
	if( posicion >=0 && posicion < 17 ){
		switch(e_fiscal){
		case 0:
				ESTADO_IMPRESORA.e_impresora[posicion]= ((char)SI);
			break;
		case 1:
				ESTADO_IMPRESORA.e_fiscal[posicion]=((char)SI);
			break;
		}
	}
}
/**************************************************************************/
int ESTADOS_IMPRESORA_FISCAL( int e_fiscal, char posicion ){
/**************************************************************************/
/*static struct{
	char e_impresora[17];
	char e_fiscal[17];
}ESTADO_INMPRESORA;*/
	int rta=0;
	if( posicion >=0 && posicion < 17 ){
		switch(e_fiscal){
		case 0:
				rta=(int)ESTADO_IMPRESORA.e_impresora[posicion];
			break;
		case 1:
				rta=(int)ESTADO_IMPRESORA.e_fiscal[posicion];
			break;
		}
	}
	return rta;
}

/******************************************************************************************/
int  ANALISIS_RESPUESTA_IMPRESORA(char * buffer, int size_buf_prn)
/******************************************************************************************/
{ int ok = 0;
      /*se usa ok para saber cual es el error
	  buffer = 0x0208f714 "PrinterState:PRINTER_OFFLINE,RECEIPT_NEAR_END FiscalState:DOCUMENTO_FISCAL_ABIERTO, Info:"
     */
    
    char fstate[200];
    char pstate[200];
	
	  int index; //indice del error

 
    int pos = 0;
    int pos2 = 0;
    char *pointer;
    char *token;

	if( size_buf_prn > 0 && strlen( buffer )>4 ){

	/*	buffer = ( char* )malloc( size_buf_prn );
        memset( buffer, 0, size_buf_prn );
		memcpy(buffer,buf_prn,size_buf_prn);*/
        //Parsea el estado de la impresora
        pointer = &buffer[0];
        pos = strcspn( pointer, ":" );
        pos2 = strcspn( pointer, " " );
		
		if(( pos + 1 )>= strlen( buffer )||( pos2 + 1 )>= strlen( buffer )){
			//sino da error al usar posiciones pos2 en 0
			return 0;//error en la cadena de entrada
		}else{
			memcpy( pstate, &pointer[pos + 1], pos2 - pos - 1 );
		}

   
        pstate[pos2 - pos - 1] = '\0';
        //Parsea el estado fiscal
        pointer = &buffer[pos2 + 1];
        pos = strcspn( pointer, ":" );
        pos2 = strcspn( pointer, " " );
	
		if(( pos + 1 )>= strlen( buffer )||( pos2 + 1 )>= strlen( buffer )){
			//sino da error al usar posiciones pos2 en 0
			return 0;//error en la cadena de entrada
		}else{
			memcpy( fstate, &pointer[pos + 1], pos2 - pos - 1 );
		}
        
        fstate[pos2 - pos - 1] = '\0';

        //Analiza respuesta estado impresora
        token = strtok( pstate, "," );

        ok = -100;

        while( token != NULL ) {
            for( index = 0;index < 6;index++ ) {
                if( !strcmp( token, printerErrors[index] ) ) {
                    switch( index ) {
							case 0:
								//ok = _FISCAL_paper_out;
								CARGA_ESTADOS_IMPRESORA( NO, 14 );
								MENSAJE_CON_PAUSA( "CAMBIE EL ROLLO DE PAPEL", 26 );
								break;
							case 1:
								//ok = _FISCAL_paper_near_end;
								CARGA_ESTADOS_IMPRESORA( NO, 4 );
								if( mostrarmensaje ==1)
									MENSAJE_CON_PAUSA( ST(S_QUEDA_POCO_PAPEL_EN_IMPRESORA), 26 );//“POCO PAPEL EN RECEIPT
								break;
							case 2:
								//ok = _FISCAL_paper_near_end;
								CARGA_ESTADOS_IMPRESORA( NO, 4 );
								if( mostrarmensaje ==1)
									MENSAJE_CON_PAUSA( ST(S_QUEDA_POCO_PAPEL_EN_IMPRESORA), 26 );//“POCO PAPEL EN JOURNAL
								break;
							case 3:
								//ok = _FISCAL_offline;
								CARGA_ESTADOS_IMPRESORA( NO, 3 );
								MENSAJE_CON_PAUSA( ST(S_IMPRESORA_FUERA_LINEA), 26 );//"IMPRESORA FUERA DE LINEA"
								break;
							case 4:
								//ok = _FISCAL_printer_error;
								CARGA_ESTADOS_IMPRESORA( NO, 2 );
								MENSAJE_CON_PAUSA( "ERROR EN IMPRESORA", 26 );
								break;
							case 5://cajon abierto
								//ok = _FISCAL_cajon_abierto ;
								//ok = -100;//siga el curso normal
								CARGA_ESTADOS_IMPRESORA( NO, 12 );
								_CAJON_POR_IMPRESORA_ABIERTO = SI;
								//if( cajon_abierto!= NULL )					cajon_abierto= SI;
								MENSAJE_CON_PAUSA( " CAJON ABIERTO ", 15 );
								break;
						}
					//ok = index;
					break;
                    /*ok = 0;
                    MENSAJE_CON_PAUSA(printerErrors[index],26);*/
                }
            }
            token = strtok( NULL, "," );
        }
        ok = -100 ;
        if( ok == -100 ) {
            //Analiza respuesta estado fiscal
            token = strtok( fstate, "," );
            while( token != NULL ) {
                for( index = 0;index < 7;index++ ) {
                    if( !strcmp( token, fiscalErrors[index] ) ) {
                        ok = index;
                        switch( index ) {
							case 0:
								ok = _FISCAL_doc_fiscal_abierto;
								CARGA_ESTADOS_IMPRESORA( SI, 12 );
								//MENSAJE_CON_PAUSA( "DOCUMENTO FISCAL ABIERTO", 26 );
								break;
							case 1:
								//ok = _FISCAL_doc_no_fiscal_abierto;
								CARGA_ESTADOS_IMPRESORA( SI, 13 );
								//MENSAJE_CON_PAUSA( "DOCUMENTO NO FISCAL ABIERTO", 26 );
								break;
							case 2:
								//ok = _FISCAL_nec_cierre;
								CARGA_ESTADOS_IMPRESORA( SI, 11 );
								if( NRO_MODO == 'Z' ||NRO_MODO == 'Y' ||NRO_MODO == 'X' ){
									MENSAJE_SIN_SONIDO( "ES NECESARIO UN CIERRE DE JORNADA" ,10);
								}else{
									MENSAJE_CON_PAUSA( "ES NECESARIO UN CIERRE DE JORNADA", 26 );
								}
								break;
							case 3:
								///ok = _FISCAL_mem_llena;
								CARGA_ESTADOS_IMPRESORA( SI, 7 );
								MENSAJE_CON_PAUSA( "MEMORIA FISCAL LLENA", 15 );
								MENSAJE_CON_PAUSA( "TIENE QUE HACER UN CIERRE DE JORNADA", 26 );
								break;
							case 4:
								//ok = _FISCAL_desborde_totales;
								CARGA_ESTADOS_IMPRESORA( SI, 6 );
								break;
							case 5:
								//ok = _FISCAL_comando_invalido;
								CARGA_ESTADOS_IMPRESORA( SI, 5 );
								MENSAJE_CON_PAUSA( "ERROR COMANDO IMPRESORA",26 );
								break;
							case 6:
								//ok = _FISCAL_mem_casi_llena;
								CARGA_ESTADOS_IMPRESORA( SI, 8 );
								if( RAM_P_TICKET && RAM_P_TICKET  < 4 ){// PARA QUE NO SEA CANSADOR EL MENSAJE EN PANTALLA
									MENSAJE_CON_PAUSA( "MEMORIA FISCAL CASI LLENA", 15 );
									MENSAJE_CON_PAUSA( "DEBERA HACER UN CIERRE DE JORNADA", 26 );
								}
							break;
						}
					}	
                }
                token = strtok( NULL, "," );
            }
        }
        if( ok == -100 ) {
            ok = _FISCAL_ok;
			/*se captura el error por anulacion automatica del comprobante en la impresora es la misma funcio que 
			DOCUMENTO_ABIERTO() pero sin llamar nuevamente al pedido de status*/
			if( !(GET_ESTADO_FISCAL( _FISCAL_doc_no_fiscal_abierto )|| GET_ESTADO_FISCAL( _FISCAL_doc_fiscal_abierto )) //doc abierto 
				&& RAM_P_TICKET && RAM_NRO_COMP > 0){
					ok = _FISCAL_printer_error;
		} 
		}
				  
	
	}
	return ok;
	}
/*******************************************************************************************************/
int DOCUMENTO_ABIERTO( )
/*******************************************************************************************************/
{
	int doc_abierto = 1;
	
	//La Funcion DOCUMENTO_ABIERTO es para la impresora (no para caja) por eso esta la funcion
	//COMANDO_LEER_STATUS_PRINTER_FISCAL que consulta el estado y setea los mismos en funcion 
	//de la consulta. Luego consulta si el comprobante esta abierto en funcion de lo que respondio
	//la impresora.
	//
	//Esta funcion no es nueva sino que habia en varios lugares la pregunta si el comprante estaba 
	//abierto y en funcion de eso, se creo esta funcion para que contuviese esas preguntas y se lo 
	//reemplazo en el codigo para hacerlo un poco mas legible y para no olvidarnos que las Impresoras 
	//No Fiscalizadas consideran una Nota de Credito como un Doc No Fiscal
	if( IMPRESORA_FISCAL == FISCAL ) {
		COMANDO_LEER_STATUS_PRINTER_FISCAL( 1 , NULL );
		//Para Impresoras No Fiscalizadas una Nota de Credito es interpretada como Doc No Fiscal
		if( !( GET_ESTADO_FISCAL( _FISCAL_doc_no_fiscal_abierto ) 
				|| GET_ESTADO_FISCAL( _FISCAL_doc_fiscal_abierto ) ) ) 
			doc_abierto = 0;
	}
	/* hubo un 'corte' en la impresora el status es el correcto pero....HAY QUE REVISAR QUE DEBIA HABER UN COMPROB HABIERTO*/
	//La impresora cerro el comprobante pero la caja todavia tiene variables sin cerear o es un error fiscal
	if( RAM_P_TICKET && RAM_NRO_COMP > 0 && doc_abierto == 0 )
		doc_abierto = 2;
	
	//doc_abierto = 0 -> Documento cerrado para la impresora. 
	//doc_abierto = 1 -> Documento abierto para la impresora. La caja se encuentra en estado normal.
	//doc_abierto = 2 -> Documento cerrado para la impresora. La caja no se encuentra en estado normal
	//					 ya que tiene items aunque el comprobante este cerrado.

	return( doc_abierto );
}
/*******************************************************************************************************/
int GET_ESTADO_FISCAL(int pregunta){
/*******************************************************************************************************/
int rta = 0;
	switch( pregunta ){
				
			case _FISCAL_paper_out:
					rta=ESTADOS_IMPRESORA_FISCAL( NO, 14 );
                
				break;
                
			case _FISCAL_paper_near_end:
					rta=ESTADOS_IMPRESORA_FISCAL( NO, 4 );
                break;
                
            case _FISCAL_offline:
					rta=ESTADOS_IMPRESORA_FISCAL( NO, 3 );
				break;
                
            case _FISCAL_printer_error:
					rta=ESTADOS_IMPRESORA_FISCAL( NO, 2 );
				break;
            
			case _FISCAL_cajon_abierto:
					rta=ESTADOS_IMPRESORA_FISCAL( NO, 12 );
				break;
                
			case _FISCAL_doc_fiscal_abierto:
					rta=ESTADOS_IMPRESORA_FISCAL( SI, 12 );
				break;
            case _FISCAL_doc_no_fiscal_abierto:
					rta=ESTADOS_IMPRESORA_FISCAL( SI, 13 );
				break;
            case _FISCAL_nec_cierre:
					rta=ESTADOS_IMPRESORA_FISCAL( SI, 11 );
               break;
           case _FISCAL_mem_llena:
					rta=ESTADOS_IMPRESORA_FISCAL( SI, 7 );
				break;
           case _FISCAL_desborde_totales:
					rta=ESTADOS_IMPRESORA_FISCAL( SI, 6 );
               break;
           case _FISCAL_comando_invalido:
					rta=ESTADOS_IMPRESORA_FISCAL( SI, 5 );
				break;
			case _FISCAL_mem_casi_llena:
					rta=ESTADOS_IMPRESORA_FISCAL( SI, 8 );
				break;
			}
	return rta ;
}
void PREFERENCIAS_IMPRESORA(char * opciones){//ACTION_SET_PRINTER_PREF preferencias de la impresora
	char mensaje[200];//para cuando devuelve la rta de la impresora
	sprintf(mensaje,"%s",opciones);
	setData( ACTION_SET_PRINTER_PREF, mensaje, 0 );
}
					
void  PREFERENCIAS_PAPEL(char * opciones){//ACTION_SET_PAPER_PREF preferencias de papel
	char mensaje[200];//para cuando devuelve la rta de la impresora
	sprintf(mensaje,"%s",opciones);
	setData( ACTION_SET_PAPER_PREF, mensaje, 0 );
	
}
							
void PREFERENCIAS_COMPROBANTE(char * opciones){//ACTION_SET_COMPR_PREF preferencias de comprobante
	char mensaje[200];//para cuando devuelve la rta de la impresora
	sprintf(mensaje,"%s",opciones);
	setData( ACTION_SET_COMPR_PREF, mensaje, 0 );
}
/***************************************************************************************************/
int VERIFICAR_SERIAL_IMPRESORA( char* buffer,int size_buf_prn ){
/***************************************************************************************************/
	int rta = SI;
	
	{
		if(  CONTROLAR_SERIAL_PRINTER && !RAM_P_TICKET &&  size_buf_prn > 0 && strlen( buffer )>7  ){
			char * ptr,*ptr2;
			strstr( buffer, "Serial" );
			ptr = strstr( buffer, "Serial");
			if(ptr){
				char serial_aux[12];
				char serial_aux_ant[12];
				int longitud=12;
				rta = NO;
				memset(serial_aux,0,sizeof(serial_aux));
				memset(serial_aux_ant,0,sizeof(serial_aux_ant));
				
				
				ptr2 = strstr( ptr, ":");
				
				_snprintf( serial_aux_ant, sizeof(serial_aux)-1, "%s",RAM_NRO_SERIE_IMPRESORA_ANT);
				
				_snprintf( serial_aux, sizeof(serial_aux)-1, "%s",++ptr2);
				ptr2 =0;
				//controlar con el mas largo
				if(strlen(serial_aux_ant)>strlen(serial_aux))
					longitud=strlen(serial_aux_ant);
				else
					longitud=strlen(serial_aux);
				
				
				if( strlen( serial_aux )>3 && strlen( serial_aux_ant )> 3 && strnicmp(serial_aux_ant,serial_aux,longitud) == 0 ){
					rta = SI;
				}else{
					char mensaje[100];
					_snprintf( mensaje, sizeof(mensaje)-1,ST(S_EL_NUMER_CONFIGURADO___s__NO_COINCIDE),serial_aux_ant);
					MENSAJE(mensaje);
					GRABAR_LOG_SISTEMA(  mensaje , LOG_ERRORES,1 );
					_snprintf( mensaje, sizeof(mensaje)-1,ST(S_CON_EL_SERIAL_DE_LA_IMPRESORA___s_),serial_aux);
					MENSAJE(mensaje);
					GRABAR_LOG_SISTEMA(  mensaje , LOG_ERRORES,1 );
					CARGAR_SERIAL_IMPRESORA();
					VERIFICA_CAMBIO_ARCHIVOS_ACTIVOS(NO);
					_snprintf( serial_aux_ant, sizeof(serial_aux)-1, "%s",RAM_NRO_SERIE_IMPRESORA_ANT);

					//mensaje para funcion que autoriza el cambio de serial#define S_AUTORIZA_SERIAL 1247  /* Autoriza Serial*/
					/*while( !SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI,
						"MEDIO DE PAGO EXCEDIDO", NULL, _F_SUP_CODIGO_0, SI ) ){
					     MENSAJE( mensaje );
					}*/


				}
				SET_MEMORY( __ram_nro_serie_impresora,serial_aux );

			}
		}
	}
	return rta;
}
/***************************************************************************************************/
int CONTROLA_LOS_ENVIOS_Y_REINTENTA(int action, char *buffer, int len, int lim_reintentos , int preg_status){
/***************************************************************************************************/
	/*
	
	action: COMANDO A ENVIAR
	*buffer: SI LLEVA INFORMACIÓN PARA EL COMANDO O DEVUELVE DATOS
	len: LONGITUD DEL BUFFER
	lim_reintentos: HASTA CUANTOS  REINTENTOS SE PUEDEN HACER
	preg_status: HAY COMANDOS QUE NO TIENEN RESPUESTA DE ESTADO, POR LO QUE SE DEBE PREGUNTARLA
	
	*/
	int reintentos = 0, rta = NO ;
	int borrar = NO;

	if(lim_reintentos <= 0)
		lim_reintentos = 10;
	
	do {
			reintentos++;
			
			rta = setData( action, buffer, len );
			
			if( preg_status )//si el comando no tiene respuestas, pregunto el estado
				COMANDO_LEER_STATUS_PRINTER_FISCAL( 1 , NULL );

			rta = CONTINUO_IMPRESION ( rta );///  0  - 1
		
			if( rta == NO ) {
				if( GET_ESTADO_FISCAL(_FISCAL_desborde_totales)
						|| GET_ESTADO_FISCAL(_FISCAL_comando_invalido) 
						|| GET_ESTADO_FISCAL(_FISCAL_mem_llena)
						|| GET_ESTADO_FISCAL(_FISCAL_printer_error)
						|| GET_ESTADO_FISCAL(_FISCAL_nec_cierre) ) {
							rta = DEBE_ANULAR_TICKET;
				} else {
					  rta= -1 ; //reintento
					  borrar = SI;
					  if( reintentos > 1) { //esto es porque el jserver dice que esta offline la printer y con solo enviar de nuevo el comando lo toma
						MENSAJE( ST(S_ERROR_IMPRESION_REVISE_IMPRESORA_P_E) );
					  do{
						  if( LASTKEY()!=-999 ){//que no muestre tantas veces.. 
							  MENSAJE( ST(S_ERROR_IMPRESION_REVISE_IMPRESORA_P_E) );
						  }
					  }while(  GETCH() != 13);
					  }

				}
			}
		}
		while( ( rta == -1 ) && reintentos < lim_reintentos );   // Jpos error wrong state
		if( borrar == SI)
			BORRAR_MENSAJE();
	return rta;
}
/***************************************************************************************************/
void CARGAR_DATOS_TF(char *name,char *lastname,char *address1,char *address2,char *address3,char *remito,char *remito1)
/***************************************************************************************************/
{
  
  int mapadebits= config_tps.encabezadoTF;
  
  int listonombre =0, listodireccion=0, listoremito=0;
  //hay que revisar que el grupo de campos alguno tenga datos.
  //name y lastaname 
  //address1,address2 address3
  //remito, remito1

  if ((strlen(name)==0) && (strlen(lastname)==0)){
		listonombre =1;
		_snprintf(name, sizeof(name),".");
  }if ((strlen(address1)==0) && (strlen(address2)==0) && (strlen(address3)==0)){
		listodireccion =1;
		_snprintf(address1, sizeof(address1),".");
  }/*if ((strlen(remito)==0) && (strlen(remito1)==0)){
		listoremito =1;
		_snprintf(remito, sizeof(remito),".");
  } el remito no es siempre obligatorio*/
  if( listonombre == 0 ) {//hay un campo con valor
	if(mapadebits & 1)
		_snprintf(lastname,sizeof(lastname),".");
  }
  if( listodireccion == 0 ) {//hay un campo con valor
	  int okdireccion =0;
	  if(mapadebits & 2) {
		  okdireccion =1;
		  _snprintf(address1, sizeof(clientes.domicilio),((strlen(clientes.domicilio))>1)? clientes.domicilio : ".");
 	  } else
		  address1[0]= 0;
	  if(mapadebits & 4) {
		  okdireccion =1;
		  _snprintf(address2, sizeof(clientes.localidad),((strlen(clientes.localidad))>1)? clientes.localidad : ".");
	  } else
		  address2[0]= 0;
	  if(mapadebits & 8) {
		  okdireccion =1;
		  _snprintf(address3, sizeof(clientes.provincia),((strlen(clientes.provincia))>1)? clientes.provincia : ".");
	  } else
		  address3[0]= 0;
	  if (okdireccion ==0) 
		  _snprintf(address1, 1,".");

  }
  if( listoremito == 0 ) {//hay un campo con valor
	  int ii = strcmp( remito, "." );
	  if(mapadebits & 16 && ii == -1  && ii!= 0) //no hay campo remito
		_snprintf(remito, 1,".");
	   ii = strcmp( remito1, "." );
	  if(mapadebits & 32 && ii == -1 && ii!= 0)
		_snprintf(remito1, 5,".");
  }
}