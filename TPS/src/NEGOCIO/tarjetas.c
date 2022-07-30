/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#pragma pack(1)
#include <cr.h>
#include <tpv_dat.h>
#include <config.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <lista.h>
#include <recupero.h>
#include <ini.h>
#ifdef INVEL_L
#include <sys/types.h>	
#include <unistd.h>
#include <sys/io.h>
#define _lseek lseek
#else
#include <io.h>
#endif
#include <math.h>
#include <memory.h>
#include <string.h>
#include <btrv.h>
#include <aarch.h>
#include <_val.h>
#include <panel.h>
#include <pagos.h>
#include <deftables.h>
#include <db.h>
#include <cmemory.h>
#include <cadafech.h>
#include <inc_mes.h>
#include <_cr1.h>
#include <mensaje.h>
#include <trim.h>
#include <m_cadena.h>
#include <pant.h>
#include <pg_fecha.h>
#include <tarjetas.h>
#include <pago_tar.h>
#include <dtoa.h>
#include <b_mens.h>
#include <clave.h>
#include <netcom2.h>
#include <tar_mag.h>
#include <cambio_m.h>
#include <func.h>
#include <comtar.h>
#include <fecha.h>
//#include <INC_DIA.h>
#include <inc_year.h>
#include <malloc.h>
#include <ftoc.h>
#include <round.h>
#include <get_fech.h>
#include <alarma.h>
#include <cr_pant.h>
#include <day.h>
#include <mon.h>
#include <year.h>
#include <cio.h>
#include <log.h>
#include <path.h>
#include <motor.h>
#if defined(INVEL_W) || defined(INVEL_L)
#include <actions.h>
#include <typeacti.h>
#endif
//Almacena el cod_tarjeta seleccionado por el usuario (modo servicios)
int cod_tar_selected;
struct _tarjetas *tarjetas_sic;
/*struct _cuo_tar 
{
 BINARY status;							//0
 unsigned short cod_tarjeta;			//1
 char nro_cuota;						//3
 char hab_cuota_pesos;					//4
 float rec_cuota_pesos;					//5
 char hab_cuota_fin_tar_pesos;			//9
 float rec_cuota_fin_tar_pesos;			//10
 char nro_orden;						//14
 char descripcion[21];					//15
 float entrega;							//36
 char hab_cuota_dol;					//40
 float rec_cuota_dol;					//41
 char hab_cuota_fin_tar_dol;			//45
 float rec_cuota_fin_tar_dol;			//46
 char tabla[6];							//50
 float coeficiente;						//56
 char hab_afinidad;						//60
 char pide_entrega;						//61
 char vigencia;							//62
 char controlar_total_cuotas;			//63
 char credito;							//64
 char hab_diferimiento;					//65
 float tasa;							//66
 short cod_grupo;						//70
 char unicamente_online;				//72
 char exclusivo_promocion;				//73
 char reserva[6];						//74
};*/
struct _cajas 
{
	BINARY status;				//0
	short nro_caja;				//1
	char habilitada_cuotas;     //3
    INT16 seccion;
	char serial[12];
	BINARY fin;		
};
struct _ind_cuo_tar
{
    unsigned short cod_tarjeta;
    char nro_orden;
};
struct _ind_cuo_tar_gru
{
    unsigned short cod_tarjeta;
    short cod_grupo;
    char nro_orden;
};

struct _vigencias 
{
	BINARY status;					//0
	unsigned short cod_tarjeta;     //1
	BINARY nro_orden;				//3
	char nombre[21];				//4
	BINARY cod_vigencia;			//25
	BINARY fin;				//26
};
struct _ind_vigencia
{
    unsigned short cod_tarjeta;
    BINARY nro_orden;
};
struct _ind3_vigencia
{
    unsigned short cod_tarjeta;
    char cod_vigencia;
};
struct _ter_caja 
{
 BINARY status;				//0
 unsigned short caja;		//1
 BINARY nodo;				//3
 long terminal;				//4
 BINARY fin;			//8
};

struct _diferir_tarjeta {
	BINARY status;						//0
	unsigned short cod_tarjeta;		//1
	unsigned short nro_diferimiento;   //3
	char descripcion[21];				//5
	BINARY fin;
};

struct
{
    unsigned short cod_tarjeta;
    unsigned short nro_diferimiento;
} indice_diferimiento;

struct _habil_difer_tarj {
	BINARY status;      //0
	INT16 cod_tarjeta;      //1
	INT16 dia_difer_inicio;      //3
	INT16 dia_difer_fin;      //5
	INT16 dia_difer_presenta;      //7
	INT16 cant_difer_meses;      //9
	BINARY fin;     //11
};

struct
{
    unsigned short cod_tarjeta;
    unsigned short difer_presenta;
    unsigned short difer_meses;
} indice_habil_diferimiento;
struct _ind_cuo_gru
{
    unsigned short cod_tarjeta;
    short cod_grupo;
};

#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
static struct _datos_privados_tarjeta {
	char numero_tarjeta_privado[30];
	unsigned  fecha_vto_privada;
	char cod_servicio_privado[4];
	char pin_privado[21];
	char track_1_privado[80];       //348
	char track_2_privado[38];
}VAR_TARJ_PRIV;

	
#endif

void REEMPLAZO_BANDERA_BINARIA(int numero_entero,char* cadena,int posicion, char picture);
long NRO_TERMINAL( int marca );
double _PIDE_ENTREGA( char *titulo, int enteros, int decimales, int signo );
int CONFIRMAR_NUMERO_TARJETA( int marca );
int TARJ_PIDE_TELEFONO( char * );
int TARJ_PIDE_TELEFONO( char * );
static char cambio_de_tarjeta = 0;
static struct _cuo_tar cuo_tar;
static struct _cuo_gru *cuo_gru;
static struct _ind_cuo_tar ind_cuo_tar;
static struct _ind_cuo_tar_gru ind_cuo_tar_gru;
static struct _ind_cuo_gru ind_cuo_gru;
static struct _vigencias vigencia;
static struct _ind_vigencia ind_vigencia;
static struct _diferir_tarjeta dif_tar;
static struct _habil_difer_tarj habdifta;
//extern char      buf_track1[81], buf_track2[51];
// Definiciones transitorias
#define _ARGENCARD          0
#define _MASTERCARD         1
#define _CARTA_FRANCA       2
#define _CREDENCIAL_VIEJA   3
#define _NARANJA            4
#define _CABAL_VIEJA        5
#define _VISA               6
#define _DINERS             7
#define _BANELCO            8
#define _AMERICAN           9
#define _MEGANET           10
#define _PROVENCRED        11
#define _LIDER             12
#define _KADICARD          13
#define _SOCIAL            14
#define _CABAL_NUEVA       15
#define _CREDENCIAL_NUEVA  16
#define _PLUSCARD          17
#define _MASTERCARD_INT    18
#define _CREDENCIAL_LIM_0  19
#define _PROVENCRED_NUEVA  20
#define _BISEL             21
#define _VALE_03200        22
#define _VALE_7130         23
#define _VALE_279900       10
#define _MAESTRO           24
#define _CMR               25
int  _MASCARA_ = 0 ;
char nro_orden[50];


#define  ENMASCARAMIENTO_CONFIGURABLE 



int TARJ_PIDE_DOCUMENTO( char *documento );
int ELEGIR_DIFERIMIENTO( int marca );
int MENU_SELECCIONA_HABILITAR( int cod_marca );
void NUMERO_DE_COMERCIO_TARJETA();
void GO_CUO_TAR_GRU( int modo );
void GO_VIGENCIA( int modo );
//int GET_MEDIO_ACTIVADO_PROMO(struct activacion_medios *act_medio, unsigned short sub_medio, char plan, 
//                             int SumarMonto, double *SumaMonto);
extern double MONTO_MEDIO_ACTIVADO_PROMO(unsigned short sub_medio, char plan, unsigned short cod_medio );
extern int ACT_MONTO_MEDIO_ACTIVADO_PROMO( unsigned short pSub_medio, char pPlan, double pMonto_usado, unsigned short cod_medio );
extern int MEDIO_ACTIVADO_PROMO(unsigned short sub_medio, char plan, unsigned short cod_medio );
extern double RETORNAR_TOTAL_PRINTER( double variacion );
void PASAR_PRIVADOS_A_DATOS_TARJETA(void);
void ENMASCARA_FECHA_TARJETA( int posicion );
long OBTENER_NRO_CUPON( char _marca );
int ANULACION_TARJETA_TECLA( int cashback);
/*****************************************************************************/
void CADENA_NRO_TARJETA( char *buffer, char *nro, char marca )
/*****************************************************************************/
{
    int h = 0, i = 0;
	//corresponde al T.2782, faltaba contemplar el rango(0-9) permitido de valores del track 
	//para obtener el nro , en al versión de Libertad DOS deberia corregirse tambien.
    while(  buffer[h] >= '0'  && buffer[h] <=  '9' && i < 19 && h < 21 ) {
		
        nro[i] = buffer[h];
        h++;
        i++;
    }
    nro[i] = 0;
}
/*****************************************************************************/
int OBTENER_COD_SERVICIO( char *buffer )
/*****************************************************************************/
{
    int h = 0, error = 0;
    while( buffer[h] > '9' ) {
        h++;
    }
    while( buffer[h] <= '9' && h < 22 ) {
        h++;
    }
    h += 5;
    SET_COD_SERVICIO_PRIVADO( &buffer[h] );
	switch(GET_COD_SERVICIO_PRIVADO_POSICION( 0 )){
            /*------------------------- Valores erroneos -----------------------*/
        case '0':
        case '3':
        case '4':
        case '7':
        case '8':
        case '9':
            error = 1;
            break;
    }
    if( !error ) {
        switch(GET_COD_SERVICIO_PRIVADO_POSICION( 1 )){
                /*----------------- Puede ser on-line u off-line ----------------*/
            case '0':
                break;
                /*----------------------- Debe ser on-line ----------------------*/
            case '2':
                SET_MEMORY_CHAR( __var_tarjetas_unicamente_ol, 1 );
                break;
                /*----------------------- Valores erroneos ----------------------*/
            default:
                error = 1;
                break;
        }
    }
    if( !error ) {
        switch( GET_COD_SERVICIO_PRIVADO_POSICION( 2 )){
                /*------------------- Necesita ingresar PIN ---------------------*/
            case '0':
            case '5':
                SET_MEMORY_CHAR( __var_tarjetas_necesita_pin, 1 );
                error = 2;
                break;
                /*---------------------- No requiere PIN ------------------------*/
            case '1':
            case '2':
                break;
                /*----------------- Requiere PIN si hay PinPad ------------------*/
            case '6':
            case '7':
                break;
                /*---------------- Solo para cajeros automticos ----------------*/
            case '3':
                error = 2;
                break;
                /*----------------------- Valores erroneos ----------------------*/
            default:
                error = 1;
                break;
        }
    }
    return ( error );
}
/*****************************************************************************/
void NRO_TARJETA( char *buffer, double *n1, double *n2, char marca_especial )
/*****************************************************************************/
{
    char nro1[20],nro2[20];

    CADENA_NRO_TARJETA( buffer, nro1, marca_especial );
    strcpy( nro2, nro1 );
    nro1[10] = 0;
    nro2[16] = 0;
    *n1 = atof( nro1 );
    *n2 = atof( &nro2[8] );
}
/*****************************************************************************/
void FECHA_VTO_TARJETA( char *buffer, unsigned * fecha )
/*****************************************************************************/
{
    char cadena[9] = {"01/xx/xx"};
    int h = 0;
    while( buffer[h] > '9'  || buffer[h] < '0' ) {
        h++;
    }
    while( (buffer[h] <= '9' && buffer[h] >= '0' ) && h < 22 ) {
        h++;
    }
    h++;
    cadena[3] = buffer[h + 2];
    cadena[4] = buffer[h + 3];
    cadena[6] = buffer[h];
    cadena[7] = buffer[h + 1];
    *fecha = CADENA_A_FECHA( cadena );
    *fecha = INCREMENTAR_MES( *fecha, 1 );
}
/*****************************************************************************/
int PEDIR_DATOS_TARJETA( char *cadena, char marca_especial )
/*****************************************************************************/
{
    int rta = 0, h, tope, l, pedir_fecha_vto, permitir_ant, indice = 0, falta_fecha = 1;
    unsigned fecha;
    char fec[9],aux[11],marca, res, estado_scanner;
    float aux_limite;
	int buffer_aux[60];
    
	memset(buffer_aux, 0, sizeof(buffer_aux));
    ExecuteSSAction( A_PEDIR, 1, "INGRESE NUMERO TARJETA", ST( S__N__DE_TARJETA_ ), NULL, " " );
    ExecuteSSAction( A_SET_PROP_INPUT, _INPUT_TARJETA, "_SIMPLE", "", NULL, " " );
    
	/*------------- Habilito Scanner ------------------*/
	permitir_ant = _PERMITIR_SCANNER;
	setData( ACTION_HABILITA_SCANNER, "true", 4 );
	estado_scanner = VARIOS_SCANNER_HABILITADO;
	SET_MEMORY_CHAR( __varios_scanner_habilitado, 1 );
	
	_PERMITIR_SCANNER = SI;
		
	/*------------ pide el numero de la tarjeta ---------------*/
	//habilito ingreso por scanner
    if( PEDIR_GRAN_CADENA_NUMERICA( 13 - ancho_panel / 2, 22, 19, cadena ) ) {
		#if defined(INVEL_W) || defined(INVEL_L)
        ExecuteAction( A_FIN_PEDIR );
        #endif
		
		if( VAR_TARJETAS_LECTURA_BANDA == 2 ){ //si se ingreso por scanner
			//buscar en la tabla con el prefijo, cuya longitud esta en la config_Dat
			int len;
			char cadena_aux[30], bin[10], prefijo[5], mascara[20];
			BEEP( 2000, 800 );
			MENSAJE( "TARJETA INGRESADA POR SCANNER" );
			memset( cadena_aux, 0, sizeof( cadena_aux ) );
			memset( prefijo, 0, sizeof( prefijo ) );
			memset( mascara, 0, sizeof( mascara ) );
            len = config.long_pref_barra;
			memcpy( prefijo, cadena, len );
			if( len != 0 ) {
				int l_tarj = 0;
				if( EXISTE_PREFIJO_BARRA_TARJETA( bin, atol( prefijo ), &l_tarj ) ) {
					len = strlen( bin );			
					sprintf( mascara, "%\%%d.%ds%\%%d.%ds", len, len, l_tarj - len, l_tarj - len );
					sprintf( cadena_aux, mascara, bin, cadena );
					memcpy( cadena, cadena_aux, sizeof( cadena_aux ) );		
				} else { //sino encuentra prefijo cancelo tarjeta
					MENSAJE( "PREFIJO SCANNER NO ENCONTRADO" );
					rta = 0;
					return rta;
				}
			} else {
				MENSAJE( "LONGITUD PREFIJO BARRA NO CONFIGURADO" );
				rta = 0;
				return rta;
			}
		}
		strcpy( buffer_aux, cadena );
		TRIM(buffer_aux);
        /*------ Buscamos fin de cadena --------*/
        for( h = strlen( buffer_aux )-1; cadena[h] == ' '; h-- ) {
        /*------ Buscamos fin de cadena --------*/
            ;
		}
        if( UTILIZAR_TABLA_MARCAS_ESPECIALES ) {
            if( marca_especial && tar_esp.prefijo_tesp[0] != ' ' && tar_esp.prefijo_tesp[0]
             && tar_esp.digitos ) {
                TRIM( tar_esp.prefijo_tesp );
                l = strlen( tar_esp.prefijo_tesp );
                memmove( &cadena[l], cadena, tar_esp.digitos );
                memcpy( cadena, tar_esp.prefijo_tesp, l );
                tope = l + tar_esp.digitos;
            }
            else {
                tope = h + 1;
            }
        }
        else {
            /*------------- Si es tarjeta CREDENCIAL_VIEJA ---------*/
            if( marca_especial == _CREDENCIAL_VIEJA ) {
                cadena[h - 1] = cadena[h];
                h--;
            }
            /*------------- Si es tarjeta NARANJA ---------*/
            if( marca_especial == _NARANJA && h < 7 ) {
                memmove( &cadena[7 - h], cadena, 8 );
                memset( cadena, '0', 7 - h );
                h = 7;
            }
            tope = h + 1;
            /*------------- Si es tarjeta CREDENCIAL_VIEJA ---------*/
            if( marca_especial == _CREDENCIAL_VIEJA ) {
                memmove( &cadena[15 - h], cadena, 16 );
                memset( cadena, '0', 15 - h );
                memcpy( cadena, "507000", 5 );
                tope = 16;
            }
            /*------------- Si es tarjeta PROVENCRED ---------*/
            if( marca_especial == _PROVENCRED ) {
                if( h > 12 ) {
                    h = 12;
                }
                memmove( &cadena[15 - h], cadena, 16 );
                memset( cadena, '0', 15 - h );
                memcpy( cadena, "560", 3 );
                tope = 16;
            }
            /*------------- Si es tarjeta PROVENCRED_NUEVA ---------*/
            if( marca_especial == _PROVENCRED_NUEVA ) {
                memmove( &cadena[6], cadena, 13 );
                memcpy( cadena, "589407", 6 );
                tope = 19;
            }
            /*------------- Si es tarjeta CABAL_VIEJA ---------*/
            if( marca_especial == _CABAL_VIEJA ) {
                memmove( &cadena[4], cadena, 16 );
                tope += 4;
            }
            /*------------- Si es tarjeta KADICARD ---------*/
            if( marca_especial == _KADICARD ) {
                memmove( &cadena[8], &cadena[6], 8 );
                memcpy( cadena, "99980000", 8 );
                tope = 16;
            }
            if( marca_especial == _VALE_03200 ) {
                memmove( &cadena[6], cadena, 12 );
                memcpy( cadena, "000071", 6 );
                tope = 18;
            }
            else if( marca_especial == _VALE_7130 ) {
                memmove( &cadena[2], cadena, 16 );
                memcpy( cadena, "00", 2 );
                tope = 18;
            }
            else if( marca_especial == _VALE_279900 ) {
                memmove( &cadena[6], cadena, 13 );
                memcpy( cadena, "589407", 6 );
                tope = 19;
            }
            else if( marca_especial == _CMR ) {
                memmove( &cadena[6], cadena, 13 );
                memcpy( cadena, "600001", 6 );
                tope = 16;
            }
        }
        /*------------------- Coloca el ENTER -----------------*/
        cadena[tope] = 13 + '0';
        /*---------------- pide la fecha de vto ---------------*/
        if( marca_especial ) {
            pedir_fecha_vto = TARJ[marca_especial - 1].pedir_fecha_vto != 'N'
                           && TARJ[marca_especial - 1].pedir_fecha_vto != 0;
        }
        else {
            memcpy( aux, cadena, DIGITOS_BIN_INTERNACIONAL );
            aux[( int )DIGITOS_BIN_INTERNACIONAL] = 0;
            if ( MODO_NEGOCIO == SERVICIOS ) {
                marca = cod_tar_selected; //Le pasamos la tarjeta seleccionada a la busqueda
                res = BUSCA_MARCA_TARJETA_SERVICIOS( atof( aux ), &marca, &aux_limite );
            }
            else {
                res = BUSCA_MARCA_TARJETA( atof( aux ), &marca, &aux_limite );
            }
            if ( res && RAM_PEDIR_CLIENTE_AFINIDAD != 2 ) { //si es manual si o si tiene que pedir fecha de vencimiento

                pedir_fecha_vto = TARJ[( int )marca].pedir_fecha_vto != 'N'
                               && TARJ[( int )marca].pedir_fecha_vto != 0;
				if( pedir_fecha_vto ==0 &&  VAR_TARJETAS_LECTURA_BANDA == 0 ){ 
					pedir_fecha_vto = 1;
				}
            }
            else {
                pedir_fecha_vto = 0;
            }
        }
        if( pedir_fecha_vto ) {
			indice = ( marca_especial )? marca_especial - 1: ( int )marca;			
			if(  TARJ[indice].usar_fecha_venc_defecto == 'S'|| TARJ[indice].usar_fecha_venc_defecto == 's' ){ 
				
				if( TARJ[indice].fecha_vencimiento != 0 ){ 				 
                  fecha =  ( marca_especial )? TARJ[marca_especial - 1].fecha_vencimiento : TARJ[( int )marca].fecha_vencimiento;
				  falta_fecha = 0;
				} else {
                   char auxi[100];
                   sprintf( auxi, "Fecha de Vencimiento incorrecta" );
				   GRABAR_LOG_SISTEMA( auxi,LOG_VENTAS,2 );                
			    }

			}
			if( falta_fecha ) {
               ExecuteSSAction( A_PEDIR, 1, "Ingrese Fecha de Vencimiento", ST( S__FECHA_DE_VENCIMIENTO_ ), "  -  ", " " );
            fecha = PEDIR_GRAN_FECHA_MMAA( 35 - ancho_panel / 2, 19 );
			}
            if( fecha != ( unsigned ) - 1 ) {
                DTOA( fecha, fec, NO );
                cadena[tope + 1] = fec[4];
                cadena[tope + 2] = fec[5];
                cadena[tope + 3] = fec[2];
                cadena[tope + 4] = fec[3];
                rta = 1;
			} else {
					MENSAJE_CON_PAUSA_CAJA( 
						"DATOS REQUERIDOS PARA CONTINUAR CON LA TRANSACCION", 30 ); 
							BORRAR_MENSAJE( );
            }
            ExecuteAction( A_FIN_PEDIR );
		} else {
            rta = 1;
        }
    }
    else {
        ExecuteAction( A_FIN_PEDIR );
    }
	
	_PERMITIR_SCANNER = permitir_ant;
	 setData( ACTION_HABILITA_SCANNER, "false", 5 );
	SET_MEMORY_CHAR( __varios_scanner_habilitado, estado_scanner );
    return ( rta );
}
/*****************************************************************************/
int PEDIR_CUOTAS( int marca, int financia_tarjeta, unsigned short cod_medio )
/*****************************************************************************/
{
    int nro = 1;
    if( CUOTAS_HABILITADAS ) {
        if( CAJA_HABILITADA_PARA_CUOTAS() ) {
            if( cambio_de_tarjeta ) {
                /*------------- Si hay cambio de tarjeta -----------------*/
                nro = VAR_TARJETAS_ANT_CUOTAS;
                if( LEE_CUO_TAR( marca, nro ) ) {
                    if( !financia_tarjeta
                     && ( ( VAR_TARJETAS_ANT_MONEDA == PESOS && cuo_tar.hab_cuota_pesos == 'N' )
                       || ( VAR_TARJETAS_ANT_MONEDA == DOLARES && cuo_tar.hab_cuota_dol == 'N' ) ) ) {
                        nro = 0;
                    }
                    else if( financia_tarjeta
                          && ( ( VAR_TARJETAS_ANT_MONEDA == PESOS
                              && cuo_tar.hab_cuota_fin_tar_pesos == 'N' )
                            || ( VAR_TARJETAS_ANT_MONEDA == DOLARES
                              && cuo_tar.hab_cuota_fin_tar_dol == 'N' ) ) ) {
                        nro = 0;
                    }
                }
                else {
                    nro = 0;
                }
			}
            else {
                /*-------------- Procedimiento normal --------------------*/
                if( MENU_CUOTAS ) {
					nro = ELEGIR_CUOTAS( marca, cod_medio );
                }
                else {
                    nro = 99;
                    while( nro == 99 ) {
                        nro = ( int )PEDIR_GRAN_ENTERO( 38 - ancho_panel / 2, 22, 2 );
                        if( nro > 0 ) {
                            if( LEE_CUO_TAR( marca, nro ) ) {
                                if( !financia_tarjeta && cuo_tar.hab_cuota_pesos == 'N'
                                 && cuo_tar.hab_cuota_dol == 'N' ) {
                                    nro = 99;
                                }
                                else if( financia_tarjeta && cuo_tar.hab_cuota_fin_tar_pesos == 'N'
                                      && cuo_tar.hab_cuota_fin_tar_dol == 'N' ) {
                                    nro = 99;
                                }
                            }
                            else {
                                nro = 99;
                            }
                        }
                    }
                }
            }
        }
        else {
            if( !LEE_CUO_TAR( marca, nro ) ) {
                nro = 0;
            }
        }
    }
    else {
        if( !LEE_CUO_TAR( marca, nro ) ) {
            nro = 0;
        }
    }
    BORRAR_MENSAJE();

    return ( nro );
}
/*****************************************************************************/
int PEDIR_FINANCIA_TARJETA()
/*****************************************************************************/
{
    int rta = 0;
    #if defined(INVEL_W) || defined(INVEL_L)
    struct _cabmenu *cab;
    int columnas;
    columnas = 2;
    cab = ( struct _cabmenu * )malloc( sizeof( struct _cabmenu ) * columnas );
    if( cab != NULL ) {
        cab[0].nombre = "COD";
        cab[0].align = ALIGN_LEFT;
        cab[0].width = 8;
        cab[1].nombre = "DESCRIPCION";
        cab[1].align = ALIGN_LEFT;
        cab[1].width = 20;
        //rta = MENU( ST( S__FINANCIA_TARJETA_ ), items, 2, cab, columnas );
        free( cab );
    }
    #else
    rta = MENU( 0, 21, ST( S__FINANCIA_TARJETA_ ), items, 2, 120, 52 );
    #endif
    return ( rta );
}
/*****************************************************************************/
int PEDIR_MONEDA( int marca, int financia_tarjeta, int consulta_saldo )
/*****************************************************************************/
{
    int rta = 0;
    struct _menu items[] = {{"1) PESOS", '1', PESOS},{"2) DOLARES", '2', DOLARES}};
    #if defined(INVEL_W) || defined(INVEL_L)
    struct _cabmenu *cab;
    int columnas;
    #endif
    if( consulta_saldo == SI 
     || ( !financia_tarjeta && cuo_tar.hab_cuota_pesos == 'S' && cuo_tar.hab_cuota_dol == 'S' )
     || ( financia_tarjeta && cuo_tar.hab_cuota_fin_tar_pesos == 'S'
       && cuo_tar.hab_cuota_fin_tar_dol == 'S' ) ) {
        if( TARJETA_CON_CECOR( marca ) ) {
            items[1].nom = "2) CECOR";
        }
        #if defined(INVEL_W) || defined(INVEL_L)
        columnas = 2;
        cab = ( struct _cabmenu * )malloc( sizeof( struct _cabmenu ) * columnas );
        if( cab != NULL ) {
            cab[0].nombre = "COD";
            cab[0].align = ALIGN_CENTER;
            cab[0].width = 8;
            cab[1].nombre = "MONEDA";
            cab[1].align = ALIGN_LEFT;
            cab[1].width = 20;
            //rta = MENU( " MONEDAS ", items, 2, cab, columnas );
            free( cab );
        }
        #else
        rta = MENU( 0, 21, " MONEDA ", items, 2, 120, 52 );
        #endif
    }
    else {
        if( !financia_tarjeta ) {
            if( cuo_tar.hab_cuota_pesos == 'S' ) {
                rta = 0;
            }
            else if( cuo_tar.hab_cuota_dol == 'S' ) {
                rta = 1;
            }
        }
        else {
            if( cuo_tar.hab_cuota_fin_tar_pesos == 'S' ) {
                rta = 0;
            }
            else if( cuo_tar.hab_cuota_fin_tar_dol == 'S' ) {
                rta = 1;
            }
        }
    }
    return ( rta );
}
/*****************************************************************************/
int PEDIR_TIPO_CUENTA( int marca )
/*****************************************************************************/
{
    int rta = 0;
    #if defined(INVEL_W) || defined(INVEL_L)
    struct _cabmenu *cab;
    int columnas;
	struct _menu items[] = {
      {"1) CAJA DE AHORRO", '1', 1},
      {"2) CUENTA CORRIENTE", '2', 2}
	};

    if( TARJ[marca].celda_pinpad ) {
        columnas = 2;
        cab = ( struct _cabmenu * )malloc( sizeof( struct _cabmenu ) * columnas );
        if( cab != NULL ) {
            cab[0].nombre = "COD";
            cab[0].align = ALIGN_LEFT;
            cab[0].width = 8;
            cab[1].nombre = "DESCRIPCION";
            cab[1].align = ALIGN_LEFT;
            cab[1].width = 20;
            rta = MENU( ST( S__TIPO_DE_CUENTA_ ), items, 2, cab, columnas );
            free( cab );
        }
    }
    #else
    if( TARJ[marca].celda_pinpad ) {
        rta = MENU( 0, 21, ST( S__TIPO_DE_CUENTA_ ), items, 2, 120, 52 );
    }
    #endif

    return ( rta );
}
/*****************************************************************************/
int PEDIR_CVV()
/*****************************************************************************/
{
    int nro = -1, salir = 0;
    char cnro[5];
    ExecuteSSAction( A_PEDIR, 1, "      INGRESE CVV ", " CVV ", NULL, " " );
    while( !salir ) {
		_MASCARA_ = 1;
        if( PEDIR_GRAN_CADENA_NUMERICA( 37 - ancho_panel / 2, 22, 4, cnro ) ) {
			//Condiciones:
			//- cnro entre tres a cuatro digitos
			//- cnro mayor a cero--> se acepta '000', pero no '00' o '0'
            if( 2 < strlen(cnro) 
				&& strlen(cnro) < 5 //controla 4 lugares
				//&& atoi( cnro ) -- acepta 0 ahora, el jauton convertira de '0' a '000'
				&& cnro[0] != ' ' && cnro[1] != ' ' && cnro[2] != ' ' ) { // al menos debe ingresar 3
                nro = atoi( cnro );
                salir = 1;
			} else {
				MENSAJE("CVV INVALIDO");
				BORRAR_MENSAJE( );
            }
        }
        else {
            salir = 1;
			MENSAJE_CON_PAUSA_CAJA( 
				"DATOS REQUERIDOS PARA CONTINUAR CON LA TRANSACCION", 30 ); 
				BORRAR_MENSAJE( );
        }
    }
	_MASCARA_ = 0;
    ExecuteAction( A_FIN_PEDIR );
    return ( nro );
}
/*****************************************************************************/
int _elegir_tar_esp( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0, ind_tar_esp;
    switch( comando ) {
        case _BUSCAR:
            ind_tar_esp = atoi( cadena );
			SET_WHERE( "COD_TESP >= '%i'", ind_tar_esp );
			SET_ORDER2( "COD_TESP" );
			RUN_QUERY( NO );
            //if( tar_esp.cod_tesp != ind_tar_esp ) {
                //GO2( BOTTOM );
            //}
            // b->pos = 0;
            // cadena[0] = ' ';
            break;
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[10];
                _itoa( tar_esp.cod_tesp, xx, 10 );
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, tar_esp.desc_tesp );
            }
            #endif
            MOSTRAR_ENTERO( 15, 13 + dato, "99", tar_esp.cod_tesp );
            MOSTRAR_CADENA( 19, 13 + dato, tar_esp.desc_tesp );
            MOSTRAR_CADENA( 15, 21, cadena );
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
            break;
        case _VALIDAR:
            if( isdigit( dato ) ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
            rta = SOLICITAR_CLAVES( 32, 19, NO, SI, PEDIR_CLAVE_EN_TARJETA_ESPECIAL, ST( S_CLAVE ),
                                    ST( S_CLAVE ), _F_SUP_TAR_ESP, SI );
            break;
        case _COMPARAR:
            if( atoi( cadena ) == tar_esp.cod_tesp ) {
                rta = 1;
            }
            break;
        case _TECLA:
            if( dato == -73 || dato == -81 ) {
                b->tecla = 0;
            }
            break;
		case _LIMIT:
            if( dato > 0 ) {
                SET_LIMIT( dato, SI );
            } else {
                SET_LIMIT( NO, NO );
            }
            break;
    }
    return ( rta );
}
/*****************************************************************************/
int PIDE_MARCA_ESPECIAL()
/*****************************************************************************/
{
    int rta = 99, k, y, ok = 0;
    if( UTILIZAR_TABLA_MARCAS_ESPECIALES ) {
        /*---------------------------- Abre el archivo ---------------------------*/
        MOSTRAR_CADENA( 13, 20, "��������������������������Ĵ" );
        if( OPEN_TABLE( T_TAR_ESP, TT_ORIG, ( char* )&tar_esp, sizeof( tar_esp ) ) == 0 ) {
            ok = Consulta_pide_tarjeta( _elegir_tar_esp );
            if( ok ) {
                rta = tar_esp.cod_tarjeta;
            }
            else {
                rta = 0;
            }
        }
        else {
            rta = 0;
        }
        CLOSE_TABLE( T_TAR_ESP, TT_ORIG );
    }
    else {
        MOSTRAR_CADENA( 15, 13, "1- CABAL ( anteriores al 12/94 )" );
        MOSTRAR_CADENA( 15, 14, "2- CMR ( ingresadas a mano )" );
        MOSTRAR_CADENA( 15, 15, "3- PROVENCRED VIEJAS ( ant al 12/95 e ing a mano )" );
        MOSTRAR_CADENA( 15, 16, "4- PROVENCRED NUEVAS ( ingresadas a mano )" );
        MOSTRAR_CADENA( 15, 17, "5- KADICARD ( ingresadas a mano )" );
        MOSTRAR_CADENA( 15, 18, "6- VALE 03200  ( ingresadas a mano )" );
        MOSTRAR_CADENA( 15, 19, "7- VALE 7103   ( ingresadas a mano )" );
        MOSTRAR_CADENA( 15, 20, "8- VALE 279900 ( ingresadas a mano )" );
        MOSTRAR_CADENA( 15, 21, "9- MAESTRO" );
        while( rta == 99 ) {
            k = GETCH();
            switch( k ) {
                case '1':
                    rta = _CABAL_VIEJA + 1;
                    break;
                case '2':
                    rta = _CMR + 1;
                    break;
                case '3':
                    rta = _PROVENCRED + 1;
                    break;
                case '4':
                    rta = _PROVENCRED_NUEVA + 1;
                    break;
                case '5':
                    rta = _KADICARD + 1;
                    break;
                case '6':
                    rta = _VALE_03200 + 1;
                    break;
                case '7':
                    rta = _VALE_7130 + 1;
                    break;
                case '8':
                    rta = _VALE_279900 + 1;
                    break;
                case '9':
                    rta = _MAESTRO + 1;
                    break;
                case 27:
                    rta = 0;
                    break;
            }
        }
        y = 13 + k - '1';
        if( !SOLICITAR_CLAVES( 32, 19, NO, SI, PEDIR_CLAVE_EN_TARJETA_ESPECIAL, ST( S_CLAVE ),
                               ST( S_CLAVE ), _F_SUP_TAR_ESP, SI ) ) {
            rta = 0;
        }
    }
    return ( rta );
}
/*****************************************************************************/
long PEDIR_NRO_CUPON( int marca, int cuotas, int financia_tarjeta )
/*****************************************************************************/
{
    long nro = 0;
    if( DEBE_PEDIR_CUPON( marca, cuotas, financia_tarjeta ) ) {
        nro = PIDE_NRO_CUPON( 8 );
    }
    return ( nro );
}
/*****************************************************************************/
long CALCULAR_NRO_CUPON( char _marca )
/*****************************************************************************/
{
    long nro = 0;
    unsigned marca;

	if ( TARJ[( int )_marca].cod_numeracion == 0 ) {
      marca = TARJ[( int )_marca].cod_numeracion;
	} else {
      marca = TARJ[( int )_marca].cod_numeracion - 1;
	}

    if( marca >= CANT_TARJETAS ) {
        marca = _marca;
    }
	 
	if( marca >= 20 ) {
		{ char mensa[100];_snprintf( mensa, sizeof( mensa ),"Error cupon %i %i, COD_NUMERACION ERRONEO, NO PUEDE SER MAYOR E IGUAL A 20",_marca, marca);glog(mensa,LOG_VENTAS,1);
		mensa[ sizeof( mensa ) - 1] = '\0';
		GRABAR_LOG_SISTEMA( mensa ,LOG_VENTAS,2); }
	} else {
	    nro = _CALCULAR_NRO_CUPON( ( char )marca );
	}

    return ( nro );
}

/*****************************************************************************/
long _CALCULAR_NRO_CUPON( char marca )
/*****************************************************************************/
{
    long nro;
   /* nro = ( marca + 1 ) * 10000000L;
    nro += ( long )config.id_suc * 1000000L;
    nro += ( long )NRO_CAJA * 10000L;*/
    _ADD_MEMORY_LONG( __nro_cupones, marca, 1L );
    if( _NRO_CUPONES( marca ) > 9999 ) {
        _SET_MEMORY_LONG( __nro_cupones, marca, 1 );
    }
//    nro += ( long )_NRO_CUPONES( marca );*/
	nro = ( long )_NRO_CUPONES( marca );
    return ( nro );
}
/*****************************************************************************/
void DECREMENTAR_NRO_CUPON( char marca )
/*****************************************************************************/
{
    long aux = 9999;
    marca = TARJ[( int )marca].cod_numeracion - 1;
    if( marca >= 0 && marca <= 20 ) {
        _ADD_MEMORY_LONG( __nro_cupones, marca, -1L );
        if( _NRO_CUPONES( marca ) == 0 ) {
            _SET_MEMORY( __nro_cupones, marca, ( char* )&aux );
        }
    }
}


/*****************************************************************************/
long OBTENER_NRO_CUPON( char _marca )
/*****************************************************************************/
{
   long      nro=0, nro_cupones=0;
   unsigned  marca;

   if( _marca < CANT_TARJETAS){
      if ( TARJ[_marca].cod_numeracion == 0 ) {
         marca = TARJ[_marca].cod_numeracion;
      } else {
         marca = TARJ[_marca].cod_numeracion - 1;
      }
      if( marca >= CANT_TARJETAS )
         marca = _marca;
      nro_cupones = ( long ) ( _NRO_CUPONES( marca ) );
	  if( nro_cupones == 0) {//no puede ser 0 el primer nro de cupon
		  _SET_MEMORY_LONG( __nro_cupones, marca, 1 );
		  nro_cupones = 1;
	  }

      nro = nro_cupones;
   }
   return ( nro );
}
/********************************************************************************************/
int VALIDA_TARJETA( int consulta_saldo, int consulta_boletin, int operacion, int hab_promo,
	char *mensaje, int origen_ventas, int medio )
/********************************************************************************************/
{
    int salir = 0, k, rta = 0, ok = 0, _k;
    //error;
    char buffer[50], marca = 0, marca_especial = 0, buffer2[80], aux_numero[21];
	long divisor;
    double n_corto,n1,n2;
    float aux_limite;
    unsigned aux_fecha;
    #ifdef COMPILAR_OMRON
    int delay_getkey,veces_getkey;
    #endif

    if( !TEF_HABILITADO() ) {
        return( 0 );
    }

    LIMPIA_BUFFER_TARJETA( SI );
    #ifdef COMPILAR_OMRON
    OMRON_GET_SETUP_GETKEY( &delay_getkey, &veces_getkey );
    OMRON_SETUP_GETKEY( 0, 1 );
    #endif
	//Se coloca medio = -1 para no confundirlo con el medio efectivo que siempre se encuentra
	//en la posicion = 0 del vector medios.
	//Se entiende que cuando se para por parametro medios = -1 no hace falta validar si se encuentra configurado 
	//en la tabla medios-tar
	//Se actualiza el vector TARJ en CARGAR_CONFIGURACION_TAR_MED
	if( medio != -1 && !CARGAR_CONFIGURACION_TAR_MED( medio ) ){
	//if( !CARGAR_CONFIGURACION_TAR_MED( ( medio ) ? medio : 6 ) ) {
		MENSAJE( ST( S_NO_HAY_CONF_MEDIO_EN_TAR_MED ) );
		// return 0; no tiene sentido cortar si no hay tarjetas para ese medio.
	}
    //Fin de actualizacion del vector TARJ
	if( TIPO_LECTOR_BANDA ) {
		if( mensaje != NULL ) {
			MENSAJE( mensaje );
//			MOSTRAR_MENSAJE_UBUNTU( 1, mensaje, 15 );
		} else {
			MENSAJE_STRING( S_PASE_LA_TARJETA );
//			MOSTRAR_MENSAJE_UBUNTU( 2, ST(S_PASE_LA_TARJETA), 15 );
		}
        #if defined(INVEL_W) || defined(INVEL_L)
		if( TIPO_LECTOR_BANDA == LECTOR_BANDA_DRIVER ) //solo cuando es por driver
			setData( ACTION_HABILITA_MSR, "true", 4 );
        #endif
        SET_ESPERA_GETCH( 2 );
    }                                             //Sin lector
    else {
        if( PEDIR_MARCA_ESPECIAL_EN_T_MANUAL ) {
            if( !EN_CAMBIO_DE_MEDIOS()
             || ( EN_CAMBIO_DE_MEDIOS() && PEDIR_SUBMEDIO_EN_CAMBIO_DE_MEDI ) ) {
                marca_especial = PIDE_MARCA_ESPECIAL();
            }
        }
        salir = 2;
    }
    Orden = NUMERO_PANEL_VALIDA_TARJETA;
    Response = PreparePanel( &pPanel, Orden );
    if( Response ) {
        ShowPanel( pPanel );
    }
	_PERMITIR_BANDA = SI;
    while( !salir ) {
        if( LECTOR_TARJETA_LEIDA() ) {

			salir = 1;
            #if defined(INVEL_W) || defined(INVEL_L)
			if( TIPO_LECTOR_BANDA == LECTOR_BANDA_DRIVER ) 
            setData( ACTION_HABILITA_MSR, "false", 5 );
            #endif
        }
        if( !salir ) {
            k = GETCH();
            if( k == 27 ) {
                salir = 3;
            } else if( k == -65 ) { //F7
                salir = 2;
            } else if( k == 132 ) {
                _k = SELECCIONA_FUNCION( 1 );
                if( _k == -65 ) {
                    UNGETCH( -65 );
                }
			} else if( k == -18 || ( PEDIR_MARCA_ESPECIAL_EN_T_MANUAL && !marca_especial ) ) {
                if( !EN_CAMBIO_DE_MEDIOS()
                 || ( EN_CAMBIO_DE_MEDIOS() && PEDIR_SUBMEDIO_EN_CAMBIO_DE_MEDI ) ) {
                    marca_especial = PIDE_MARCA_ESPECIAL();
					BORRAR_MENSAJE();
					MENSAJE_STRING( S_PASE_LA_TARJETA );
                }
            }
        }
    }
	_PERMITIR_BANDA = NO;
    ClearPanel( pPanel );
    #ifdef COMPILAR_OMRON
    /*-------- Restaura los valores anteriores -----------*/
    OMRON_SETUP_GETKEY( delay_getkey, veces_getkey );
    #endif
    /*--------------------- Consigue Nro de tarjeta ------------------*/
    SET_MEMORY_CHAR( __var_tarjetas_lectura_banda, 0 );
    BORRAR_MENSAJE();
    //salir = 1; // Solo para pruebas !!
	if( salir == 1 ) {
        //ok = 1;
        //strcpy( buffer, "1122334455667788=040433000000000" );
        //strcpy( buffer2, "" );
        //strcpy( &buf_track2[1], buffer );
        //buf_track2[0] = strlen( buffer );
        //strcpy( &buf_track1[1], buffer2 );
        //buf_track1[0] = strlen( buffer2 );
        GRABAR_LOG_SISTEMA( "LEE BUFFER TARJETA" ,LOG_VENTAS,4);
        ok = LEER_TARJETA_BANDA( buffer, buffer2 );
		EJECUTAR_SCRIPT_AUTOIT("killscript.exe","notificaciones", NO );//cierra cualquier notificacion 
        GRABAR_LOG_SISTEMA( "FIN LEE BUFFER TARJETA" ,LOG_VENTAS,4);
        if( ok ) {
            SET_MEMORY_CHAR( __var_tarjetas_lectura_banda, 1 );
            GRABAR_LOG_SISTEMA( "CARGA TRACKS EN RNV" ,LOG_VENTAS,4);
            CARGA_TRACKS_EN_RNV();
            GRABAR_LOG_SISTEMA( "FIN CARGA TRACKS EN RNV" ,LOG_VENTAS,4);
        }
    }
	else if( salir == 2 ) {
        GRABAR_LOG_SISTEMA( "PIDE DATOS TARJETA",LOG_VENTAS,4 );
		EJECUTAR_SCRIPT_AUTOIT("killscript.exe","notificaciones", NO );//cierra cualquier notificacion 
		ok = PEDIR_DATOS_TARJETA( buffer, ( char )( ( marca_especial ) ? marca_especial - 1 : 0 ) );
		
    }if( salir == 3 ) {
		//seteemos para el xpromo el submedio
		SET_MEMORY_INT( __cantidad_eventos_enviar_offline,((int) marca) + 1 );
		EJECUTAR_SCRIPT_AUTOIT("killscript.exe","notificaciones", NO );//cierra cualquier notificacion 
		//return 1;
		return 0;
		}
	
	if( ok ) {
		double n_corto_temp = 0.0, n_corto_temp2 = 0.0;
        /*--------------- Determina la marca de la tarjeta --------------*/
        GRABAR_LOG_SISTEMA( "DETERMINA LA MARCA",LOG_VENTAS,4 );
        NRO_TARJETA( buffer, &n1, &n2, ( char )( ( marca_especial ) ? marca_especial - 1 : 0 ) );
        // TOLEDO NEW: DIGITOS_BIN_INTERNACIONAL
        for( k = ( 10 - DIGITOS_BIN_INTERNACIONAL ), divisor = 1;k > 0;k--, divisor *= 10 ) {
            ;
        }
        n_corto_temp = n1 / divisor;
		n_corto_temp2 = modf( n_corto_temp, &n_corto );
		
        // n_corto = n1 / 100;
        if( marca_especial ) {
            marca = marca_especial - 1;
            rta = 1;
            SET_MEMORY_FLOAT( __var_tarjetas_limite, TARJ[( int )marca].limite );
        }
        else {
			char mensa[60];
			memset(mensa,0,sizeof(mensa));
			_snprintf(mensa,sizeof(mensa)-1,"BUSCA MARCA DE LA TARJETA %lf", n_corto_temp);
            aux_limite = VAR_TARJETAS_LIMITE;
            GRABAR_LOG_SISTEMA( mensa ,LOG_VENTAS,4);
            rta = BUSCA_MARCA_TARJETA( n_corto, &marca, &aux_limite );
            GRABAR_LOG_SISTEMA( "FIN BUSCA MARCA DE LA TARJETA" ,LOG_VENTAS,2);
            SET_MEMORY_FLOAT( __var_tarjetas_limite, aux_limite );
            if( rta ) {
				char mensa[60];
				memset(mensa,0,sizeof(mensa));
				_snprintf(mensa,sizeof(mensa)-1,"CONTROLA SI EXISTE LA TARJETA %i", marca+1);
                GRABAR_LOG_SISTEMA( mensa ,LOG_VENTAS,2);
                rta = EXISTE_TARJETA( marca );
                GRABAR_LOG_SISTEMA( "FIN CONTROLA SI EXISTE LA TARJETA",LOG_VENTAS,4 );
            }
        }

        /*--------------- valida la tarjeta --------------*/
        if( rta ) {
            GRABAR_LOG_SISTEMA( "CADENA_NRO_TARJETA",LOG_VENTAS,5 );
            CADENA_NRO_TARJETA( buffer, aux_numero, marca );
            //SET_MEMORY( __var_tarjetas_numero, aux_numero );
			SET_NUMERO_TARJETA_PRIVADO( aux_numero );
            GRABAR_LOG_SISTEMA( "FIN CADENA_NRO_TARJETA",LOG_VENTAS,5 );
        }
        else {
            MENSAJE_STRING( S_MARCA_TARJ_NO_DETECTAD );
            rta = 0;
        }
		if( rta ) {
			/*if(medio==0)

				medio=6; *///tarjetas de credito
			if( TARJ[( int )marca].pide_supervisor == 'S' && origen_ventas ) {
				if( !( SOLICITAR_CLAVES( 30 - ancho_panel, 20, NO, SI, SI, "AUTORIZACION TARJETA",
					"Autoriz¢ tarjeta", _F_SUP_MEDIO_DE_PAGO, SI ) ) ) {
					rta = 0;
					MENSAJE( "NO SE AUTORIZA EL MEDIO" );
				}
			}
		}
        if( rta && !AUTORIZACION_ON_LINE_HABILITADA && VAR_TARJETAS_UNICAMENTE_OL ) {
            MENSAJE_STRING( S_TERM_NO_HAB_TRANS_ONLI );
            rta = 0;
        }
		
		

        if( rta && ( config.pais == CHILE || config.pais == BRASIL ) && VAR_TARJETAS_LECTURA_BANDA != 1
         && TARJ[( int )marca].celda_pinpad ) {
            rta = 0;
            MENSAJE_STRING( S_INGRESO_MANUAL_TARJETA_NO_PERM );
        }
        if( rta && ( config.pais == CHILE || config.pais == BRASIL ) && VARIAS_TARJETAS_POR_PAGO
         && TARJ[( int )marca].celda_pinpad ) {
            if( CANTIDAD_TARJETAS_DEBITOS_INGRESADAS() > 0 ) {
                rta = 0;
                MENSAJE_STRING( S_NO_SE_PERMITEN_DOS_TAR_DEB );
            }
        }
        if( rta && ( config.pais == CHILE || config.pais == BRASIL ) && VARIAS_TARJETAS_POR_PAGO
         && !TARJ[( int )marca].celda_pinpad ) {
            if( CANTIDAD_TARJETAS_DEBITOS_INGRESADAS() > 0 ) {
                rta = 0;
                MENSAJE_STRING( S_EXISTEN_TAR_DEB_INGRESADAS );
            }
        }
        if( rta && ( config.pais == CHILE || config.pais == BRASIL ) && RAM_NOTA_CR
         && TARJ[( int )marca].celda_pinpad ) {
            rta = 0;
            MENSAJE_STRING( S_TARJETA_NO_PERMITIDA_EN_NOTA_DE_CREDITO );
        }
		if( rta && TARJ[( int )marca].permite_consulta_saldo == 'N' && consulta_saldo == SI ) {
		    rta = 0;
			MENSAJE( "TARJETA NO HABILITADA PARA CONSULTA DE SALDO" );
		}
		if( rta && TARJ[( int )marca].hab_promo == 'N' && hab_promo ) {
		    rta = 0;
			MENSAJE( "TARJETA NO HABILITADA PARA PROMOCIONES" );
		}
		if( rta && TARJ[( int )marca].hab_nc == 'N' && RAM_MODO_DEVOLUCION ) {
			rta = 0;
			MENSAJE( "TARJETA NO HABILITADA PARA DEVOLUCIONES" );
		}
        //-------- Verificacion de los ultimos 4 digitos de la tarjeta ------/
        if( rta && VAR_TARJETAS_LECTURA_BANDA == 1 ) {
            rta = CONFIRMAR_NUMERO_TARJETA( marca );
        }
      
        if( rta && !VAR_TARJETAS_LECTURA_BANDA && TARJ[marca].permite_ingreso_manual == 'N' ) {
           rta = 0;
           MENSAJE_STRING( S_NO_SE_PERMITE_INGRESO_MANUAL_TARJ );
        } 
		
		if( rta && !VALIDAR_MODO_INGRESO_TARJETA( marca, operacion ) ) {
			rta = 0;
			MENSAJE( "METODO DE INGRESO NO PERMITIDO" );
		}
        if( rta ) {
            //---------- valida largo del numero ---------/
            if( ( config.pais == COLOMBIA && marca_especial )
             || VALIDA_LARGO_NUMERO( marca, strlen( GET_NUMERO_TARJETA_PRIVADO() ) ) ) {
                //--------- valida numero -------/
                if( ( config.pais == COLOMBIA && marca_especial )
                 || VALIDA_NUMERO_TARJETA( GET_NUMERO_TARJETA_PRIVADO(), marca ) ) {
                    aux_fecha = GET_FECHA_VTO_PRIVADA(  ) ;
                    FECHA_VTO_TARJETA( buffer, &aux_fecha );
                    SET_FECHA_VTO_PRIVADA( aux_fecha );
                    //---------- valida fecha vto ------------/
                    if( TARJ[( int )marca].pedir_fecha_vto != 'N'
                     && TARJ[( int )marca].pedir_fecha_vto != 0 ) {
                        rta = VALIDA_FECHA_VTO( GET_FECHA_VTO_PRIVADA(  )  );
                    }
                    else {
                        rta = 1;
                    }
					if( rta ) {
						int rta_online = 0;
						/*
                        if( TARJ[( int )marca].celda_pinpad ) {
                            SET_MEMORY_CHAR( __var_tarjetas_unicamente_ol, 1 );
                        }*/
						if( TARJ[( int )marca].celda_pinpad ) {
							rta_online = 1;
                        }
						switch( TARJ[marca].exclusivo_online ){ 
							//case 0: //Offline
							case 'N': //Offline
							case 'n': //Offline
							case 0: //Offline
							case '0': //Offline
								rta_online = 0;
							break;
							case 'S': //Online
							case 's': //Online
							case '1': //Online
							case 1  : //Online
								rta_online = 1;
							break;
						}
						SET_MEMORY_CHAR( __var_tarjetas_unicamente_ol, rta_online );
						
                        /*------------ busca tarjeta en boletin --------------*/
                        if( !GENERAR_ON_LINE_EN_CAMBIO_DE_MED && EN_CAMBIO_DE_MEDIOS() ) {
                            rta = 1;
                        }
                        else {
                            if( NUMERO_ARCHIVO( marca )> 0 && !VAR_TARJETAS_UNICAMENTE_OL
                             && ( BUSCAR_BOLETIN == _AL_INICIO || consulta_boletin ) ) {
                                rta = BUSCAR_EN_BOLETIN( marca );
                            }
                        }
                        /*------------ pide CVV --------------*/
                        /*if( rta && !VAR_TARJETAS_LECTURA_BANDA
                            && TARJ[(int)marca].manual_especial == 'S' )
                        {
                            SET_MEMORY_INT( __var_tarjetas_cvv, PEDIR_CVV(  ) );
                            if( VAR_TARJETAS_CVV < 0 )
                                rta = 0;
                        }
                        else
                        {
                            SET_MEMORY_INT( __var_tarjetas_cvv, -1 );
                        }*/
                        if( rta ) {
                            SET_MEMORY_INT( __var_tarjetas_cvv, -1 );
                            switch( TARJ[marca].manual_especial ) {
                                case 'S':
                                    if( VAR_TARJETAS_LECTURA_BANDA != 1 ) {
                                        SET_MEMORY_INT( __var_tarjetas_cvv, PEDIR_CVV() );
                                        if( VAR_TARJETAS_CVV < 0 ) {
                                            rta = 0;
                                        }
                                    }
                                    break;
                                case 'N':
                                    SET_MEMORY_INT( __var_tarjetas_cvv, -1 );
                                    break;
                                case '1':
                                    if( VAR_TARJETAS_LECTURA_BANDA == 1 ) {
                                        SET_MEMORY_INT( __var_tarjetas_cvv, PEDIR_CVV() );
                                        if( VAR_TARJETAS_CVV < 0 ) {
                                            rta = 0;
                                        }
                                    }
                                    break;
                                case '2':
                                    SET_MEMORY_INT( __var_tarjetas_cvv, PEDIR_CVV() );
                                    if( VAR_TARJETAS_CVV < 0 ) {
                                        rta = 0;
                                    }
                                    break;
                            }
                        }
                        else {
                            SET_MEMORY_INT( __var_tarjetas_cvv, -1 );
                        }
						/*
						aca debria tener la funcion de reenvia de reversos y demas.... sino se hace por repuestas
						if( rta && AUTORIZACION_ON_LINE_HABILITADA 
								&& TARJETA_ON_LINE( (int )marca )
								&& EXISTEN_REVERSOS_PENDIENTES(TARJ[( int )marca].nodo)) {

							MENSAJE( "NODO AUTORIZADOR INHABILITADO POR REVERSOS PENDIENTES" );
						   	rta = 0;
						}*/
                        /*------------- Verifica Tarjetas repetidas ---------*/
                        if( rta && VERIFICAR_TARJETAS_REPETIDAS ) {
                            rta = VERIFICAR_TARJETA_EN_COMTAR( marca );
                        }
                        /*----------------- Verifica los datos ---------------*/
                        if( rta && VERIFICAR_DATOS_TARJETA && VAR_TARJETAS_LECTURA_BANDA != 1 ) {
                            MENSAJE_STRING( S_VERIF_DATOS_TARJETA );
                            rta = SOLICITAR_CLAVES( 60 - ancho_panel, 19, NO, SI, PEDIR_SUPERVISOR_VERIFICAR_TARJE, ST( S_TARJETA_VERIFICADA )/*NULL*/,
                                                    ST( S_TARJETA_VERIFICADA ), _F_SUP_DATOS_TAR, SI );
                        }
                        if( rta && PEDIR_DATOS_CLIENTE_TARJETA ) {
                            char buffer[32];
                            if( TARJ_PIDE_TELEFONO( buffer ) ) {
                                SET_MEMORY( __ram_tarj_telefono, buffer );
                            }
                            else {
                                buffer[0] = 0;
                            }
                            if( TARJ_PIDE_DOCUMENTO( buffer ) ) {
                                SET_MEMORY( __ram_tarj_documento, buffer );
                            }
                            else {
                                buffer[0] = 0;
                            }
                        }
                    }
                }
                else {
                    MENSAJE_STRING( S_NRO_TARJ_INVALIDO );
                    rta = 0;
                }
            }
            else {
                MENSAJE_STRING( S_NRO_TARJ_INVALIDO );
                rta = 0;
            }
        }
    }
    SET_ESPERA_GETCH( 0 );
    BORRAR_MENSAJE();
    SET_MEMORY_CHAR( __var_tarjetas_tarjeta, marca );
    SET_MEMORY_CHAR( __var_tarjetas_captura_on_line,
                     TARJ[( int )marca].captura_on_line );
    SET_MEMORY_CHAR( __var_tarjetas_status, ( rta ) ? 1 : 0 );
    if( rta
     && ( config.momento_recargo == AL_RENGLON || PEDIR_DATOS_TARJETA_AL_VALIDAR
       || cambio_de_tarjeta ) ) {
        rta = PIDE_DATOS_TARJETA( consulta_saldo, medios[_medio_tarjeta].nro );
        if( !rta ) {
            SET_MEMORY_CHAR( __var_tarjetas_status, 0 );
        }
    }
	
    return ( rta );
}
/*****************************************************************************/
int PIDE_DATOS_TARJETA( int consulta_saldo, unsigned short cod_medio )
/*****************************************************************************/
{
    int marca, rta = 1, ok = 0, nro, fecha_dif, cuotas;
	int tarmed_abierta = NO;

	marca = VAR_TARJETAS_TARJETA;
    /*--------------------------- Pide quien financia -----------------------*/
    if( cambio_de_tarjeta ) {
        SET_MEMORY_CHAR( __var_tarjetas_financia_tarjeta, VAR_TARJETAS_ANT_FINANCIA_TARJETA );
        if( VAR_TARJETAS_FINANCIA_TARJETA && ( TARJ[( int )marca].pide_numero & 0x80 ) == 0 ) {
            rta = 0;
        }
    }
    else if( consulta_saldo == SI ) {
        SET_MEMORY_CHAR( __var_tarjetas_financia_tarjeta, 0 );
    }
    else if( TARJ[( int )marca].pide_numero & 0x80 ) {
        SET_MEMORY_CHAR( __var_tarjetas_financia_tarjeta, PEDIR_FINANCIA_TARJETA() );
    }
    else {
        SET_MEMORY_CHAR( __var_tarjetas_financia_tarjeta, 0 );
    }
    /*------------------------- Pide tipo de cuenta -------------------------*/
	if( config.pais != CHILE && config.pais != BRASIL ) {
        SET_MEMORY_CHAR( __var_tarjetas_tipo_cuenta, PEDIR_TIPO_CUENTA( marca ) );
		//TECLA ESC -> -1 
		if( VAR_TARJETAS_TIPO_CUENTA < 0 ) {
			SET_MEMORY_CHAR( __var_tarjetas_tipo_cuenta, 0 );
			return 0;
		}
    }
    else if( TARJ[( int )marca].celda_pinpad ) {
        SET_MEMORY_CHAR( __var_tarjetas_tipo_cuenta, 1 );
    }
    /*------------------------- Graba datos pinpad --------------------------*/
    SET_MEMORY_CHAR( __var_tarjetas_celda_pinpad, TARJ[( int )marca].celda_pinpad );
    SET_MEMORY_CHAR( __var_tarjetas_l_pin, TARJ[( int )marca].largo_pin );
    if( VAR_TARJETAS_CELDA_PINPAD ) {
        SET_MEMORY_CHAR( __var_tarjetas_unicamente_ol, 1 );
    }
    /*--------------------------- Pide vigencia -----------------------------*/
    if( consulta_saldo == NO && DEBE_PEDIR_COD_VIGENCIA( marca ) ) {
        SET_MEMORY_CHAR( __var_tarjetas_cod_vigencia, ELEGIR_VIGENCIA( marca ) );
    }
    /*----------------------- Pide cantidad de cuotas -----------------------*/
	{int cuotas =VAR_TARJETAS_CUOTAS;
	cuotas;}
    if( consulta_saldo == SI ) {
        SET_MEMORY_CHAR( __var_tarjetas_cuotas, 1 );
    }else if( consulta_saldo == ANULACION ) {
			SET_MEMORY_CHAR( __var_tarjetas_cuotas, VAR_TARJETAS_CUOTAS );//DEBE QUEDAR IGUAL PARA EL CASO DE LAS DEVOLUCIONES.. PARA QUE NO PIDA
		  }else {
		cuotas = PEDIR_CUOTAS( marca, VAR_TARJETAS_FINANCIA_TARJETA, cod_medio );
        SET_MEMORY_CHAR( __var_tarjetas_cuotas, cuotas );
		if( ( cuotas == 0 ) && ( ( CUOTAS_HABILITADAS ) || ( MENU_CUOTAS ) ) ) {
            return 0;
        }
    }
	{int cuotas =VAR_TARJETAS_CUOTAS;
	cuotas;}

	/**********************Para Banda Magnetica**************************/
	//{
		/*struct{
			char cod_medio;
			char cod_tarjeta;
		}tm_index;*/

		//tm_index.cod_medio = (char)( cod_medio );
		//tm_index.cod_tarjeta = (char)( marca + 1 );
		if( OPENED( T_TAR_MED, TT_ORIG )) {
			tarmed_abierta = SI;
			
		}
		if( tarmed_abierta || (!OPEN_TABLE( T_TAR_MED, TT_ORIG, ( char* )&tar_med, sizeof( tar_med ) ) )) {
			SELECT_TABLE( T_TAR_MED, TT_ORIG );
			SET_WHERE(" COD_MEDIO = '%i' AND COD_TARJETA = '%i'", cod_medio, marca + 1);
			RUN_QUERY(NO);
			if( FOUND2( ) && (tar_med.cuotas_online == 's'|| tar_med.cuotas_online == 'S') ){
				cuo_tar.unicamente_online = tar_med.cuotas_online;
			}
			//CLOSE_DB( _TAR_MED_BTR );
			CLOSE_TABLE( T_TAR_MED, TT_ORIG );
		}
	//}
	/*******************************************************************/
    /*-------- Si no pide vigencia la toma de los datos de la cuota ---------*/
    if( !DEBE_PEDIR_COD_VIGENCIA( marca ) ) {
        SET_MEMORY_CHAR( __var_tarjetas_cod_vigencia, cuo_tar.vigencia );
    }
    /*------------------------------ Pide moneda ----------------------------*/
    if( cambio_de_tarjeta ) {
        SET_MEMORY_CHAR( __var_tarjetas_moneda, VAR_TARJETAS_ANT_MONEDA );
    }
    else {
        SET_MEMORY_CHAR( __var_tarjetas_moneda,
                         PEDIR_MONEDA( marca, VAR_TARJETAS_FINANCIA_TARJETA, consulta_saldo ) );
    }
    SET_MEMORY_FLOAT( __var_tarjetas_tasa, cuo_tar.tasa );
    /*--------------------------- Calcula recargo ---------------------------*/
    if( VAR_TARJETAS_FINANCIA_TARJETA ) {
        SET_MEMORY_FLOAT( __var_tarjetas_recargo,
                          ( VAR_TARJETAS_MONEDA == PESOS ) ? cuo_tar.rec_cuota_fin_tar_pesos
                        : cuo_tar.rec_cuota_fin_tar_dol );
    }
    else {
        SET_MEMORY_FLOAT( __var_tarjetas_recargo,
                          ( VAR_TARJETAS_MONEDA == PESOS ) ? cuo_tar.rec_cuota_pesos
                        : cuo_tar.rec_cuota_dol );
    }
    if( cuo_tar.pide_entrega == 'S' ) {
        SET_MEMORY_CHAR( __var_tarjetas_pide_entrega, SI );
    }
    else {
        SET_MEMORY_FLOAT( __var_tarjetas_porc_entrega, cuo_tar.entrega );
    }
    if( ( MODO_DEVOLUCION || RAM_NOTA_CR ) && config.pais != COLOMBIA && config.pais != MEXICO ) {
        if( config.pais == CHILE || config.pais == BRASIL ) {
            if( rta && !PEDIR_FECHA_ORIGINAL() ) {
                rta = 0;
            }
            if( rta && !PEDIR_HORA_ORIGINAL() ) {
                rta = 0;
            }
            if( rta && !PEDIR_AUT_ORIGINAL() ) {
                rta = 0;
            }
            if( rta && !PEDIR_CUPON_ORIGINAL() ) {
                rta = 0;
            }
            if( rta && VAR_TARJETAS_TASA > 0.001 && !PEDIR_TASA_APLICADA() ) {
                rta = 0;
            }
            if( rta && VAR_TARJETAS_TASA > 0.001 && !PEDIR_IMPORTE_CUOTA() ) {
                rta = 0;
            }
        }
        else {
            if( rta && !PEDIR_CUPON_ORIGINAL() ) {
                rta = 0;
            }
            if( rta && !PEDIR_FECHA_ORIGINAL() ) {
                rta = 0;
            }
        }
    }
    if( cuo_tar.controlar_total_cuotas == 'N' ) {
        SET_MEMORY_CHAR( __var_tarjetas_control_cuotas, NO );
    }
    else {
        SET_MEMORY_CHAR( __var_tarjetas_control_cuotas, SI );
    }
    /*--------------------------- Nmero de comercio ------------------------*/
    NUMERO_DE_COMERCIO_TARJETA();
    SET_MEMORY_CHAR( __var_tarjetas_unicamente_ol,
                     ( ( cuo_tar.unicamente_online == 'S' || VAR_TARJETAS_UNICAMENTE_OL )
						&& !RAM_NOTA_CR ) ? 1 : 0 );
	if( VAR_TARJETAS_CELDA_PINPAD > 0 && RAM_NOTA_CR && !VAR_TARJETAS_UNICAMENTE_OL) //tarjetas de debito siempre online
		SET_MEMORY_CHAR( __var_tarjetas_unicamente_ol, 1); 
    
    if( VAR_TARJETAS_CUOTAS > 0 && VAR_TARJETAS_MONEDA >= 0 && VAR_TARJETAS_FINANCIA_TARJETA >= 0
     && VAR_TARJETAS_TIPO_CUENTA >= 0 ) {
        /*------------ busca tabla -----------*/
        SET_MEMORY( __var_tarjetas_tabla, cuo_tar.tabla );
        /*---------------- coeficiente --------------------*/
        SET_MEMORY_FLOAT( __var_tarjetas_coeficiente, cuo_tar.coeficiente );
        /*------------- Busca nro de terminal ---------------*/
        SET_MEMORY_LONG( __var_tarjetas_terminal, NRO_TERMINAL( marca ) );
        /*-------------- diferimiento de tarjetas ----------------*/
        fecha_dif = 0;
        if( RAM_PEDIR_DIFERIMIENTO ) {
            if( TARJ[marca].diferimiento && cuo_tar.hab_diferimiento != 'N' ) {
                // por dias
                if( TARJ[marca].diferimiento == 1 ) {
                    nro = -1;
                    while( nro == -1 ) {
                        nro = ( int )PEDIR_GRAN_ENTERO( 40, 22, 2 );
                        if( nro > TARJ[marca].diferimiento_maxima_cant_dias ) {
                            MENSAJE_STRING( S_DIFERIM_EXCEDE_CANTIDA );
                            nro = -1;
                        }
                    }
                    if( nro > 0 ) {
                        fecha_dif = INC_DIA( nro, 1 );
                    }
                }                                 // por menu
                else if( TARJ[marca].diferimiento == 2 ) {
                    //USE_DB( AREA_AUX2, _DIF_TARJ, ( char* )&dif_tar, sizeof( dif_tar ), NULL, 0, 0 );
					OPEN_TABLE(T_DIF_TARJ, TT_ORIG,( char* )&dif_tar, sizeof( struct _diferir_tarjeta ) );
                    nro = ELEGIR_DIFERIMIENTO( marca + 1 );
                    //CLOSE_DATABASE( AREA_AUX2 );
					CLOSE_TABLE( T_DIF_TARJ, TT_ORIG );
                    if( nro > 0 ) {
                        fecha_dif = INC_DIA( nro, 1 );
                    }
                }                                 // por dias, pero
                else if( TARJ[marca].diferimiento == 3 ) {
                    // pidiendo fecha
                    fecha_dif = 0;
                    while( fecha_dif == 0 ) {
                        fecha_dif = PEDIR_GRAN_FECHA( 35, 22, NULL );
                        if( DIF_DIAS( fecha_dif, _GET_FECHA_DOS() )
                          > TARJ[marca].diferimiento_maxima_cant_dias ) {
                            MENSAJE_STRING( S_DIFERIM_EXCEDE_CANTIDA );
                            fecha_dif = 0;
                        }
                    }
                    if( fecha_dif < 0 ) {
                        fecha_dif = 0;
                    }
                    else if( fecha_dif == ( int )_GET_FECHA_DOS() ) {
                        fecha_dif = 0;
                    }
                }
                else if( TARJ[marca].diferimiento == 4 ) {
                    fecha_dif = MENU_SELECCIONA_HABILITAR( TARJ[marca].cod_tarjeta );
                    if( !fecha_dif ) {
                        MENSAJE_STRING( S_NO_DIFERIM_FEC_ACTUAL );
                    }
                }
            }
            else {
                MENSAJE_STRING( S_TARJ_NO_HAB_DIFERIR );
            }
        }
        SET_MEMORY_INT( __var_tarjetas_primer_vto, fecha_dif );
        //---- Se Agrega el recargo por diferimiento si se opto por diferir.
        if( fecha_dif ) {
            SET_MEMORY_FLOAT( __var_tarjetas_recargo_diferim, TARJ[marca].recargo_diferimiento );
        }
        else {
            SET_MEMORY_FLOAT( __var_tarjetas_recargo_diferim, 0.0 );
        }
        /*----------------- Verifica los datos ---------------*/
        if( consulta_saldo == NO && rta && VERIFICAR_DATOS_CUPON
         && !( DEBE_PEDIR_CUPON( marca, VAR_TARJETAS_CUOTAS, VAR_TARJETAS_FINANCIA_TARJETA ) )
         && VAR_TARJETAS_LECTURA_BANDA == 0 ) {
            MENSAJE_STRING( S_VERIFIQUE_LOS_DATOS );
            rta = SOLICITAR_CLAVES( 60 - ancho_panel, 19, NO, SI, SI, NULL,
                                    ST( S_CUPON_VERIFICADO ), _F_SUP_DATOS_CUPON, SI );
        }
        /*-------------------- Pide cupon ----------------------*/
        if( rta && VAR_TARJETAS_FINANCIA_TARJETA != -1 ) {
            if( consulta_saldo == SI ) {
                ok = 1;
            }
            else {
                SET_MEMORY_LONG( __var_tarjetas_nro_cupon,
                                 PEDIR_NRO_CUPON( marca, VAR_TARJETAS_CUOTAS,
                                                  VAR_TARJETAS_FINANCIA_TARJETA ) );
                if( VAR_TARJETAS_NRO_CUPON != -1 ) {
                    ok = 1;
                }
            }
            SET_MEMORY_CHAR( __var_tarjetas_nro_orden_cuota, cuo_tar.nro_orden );
        }
    }
    BORRAR_MENSAJE();
    SET_MEMORY_CHAR( __var_tarjetas_status, ( ok ) ? 2 : 1 );
    if(  MODO_DEVOLUCION || RAM_NOTA_CR ) 
		return (rta);
	else
	return ( 1 );

}
/*****************************************************************************/
void NUMERO_DE_COMERCIO_TARJETA()
/*****************************************************************************/
{
    if( TOMAR_NRO_COMERCIO_CUOTAS && VAR_TARJETAS_CUOTAS > 1 ) {
        if( VAR_TARJETAS_MONEDA == PESOS ) {
            SET_MEMORY( __var_tarjetas_nro_comercio,
                        TARJ[( int )VAR_TARJETAS_TARJETA].nro_comercio_pesos_cuotas );
        }
        else {
            SET_MEMORY( __var_tarjetas_nro_comercio,
                        TARJ[( int )VAR_TARJETAS_TARJETA].nro_comercio_dol_cuotas );
        }
    }
    else {
        if( VAR_TARJETAS_MONEDA == PESOS ) {
            SET_MEMORY( __var_tarjetas_nro_comercio,
                        TARJ[( int )VAR_TARJETAS_TARJETA].nro_comercio_pesos );
        }
        else {
            SET_MEMORY( __var_tarjetas_nro_comercio,
                        TARJ[( int )VAR_TARJETAS_TARJETA].nro_comercio_dol );
        }
    }
}
/*****************************************************************************/
int AUTORIZA_TARJETA_EN_BOLETIN()
/*****************************************************************************/
{
    int rta;
    rta = SOLICITAR_CLAVES( 30 - ancho_panel / 2, 30, NO, SI, SI, NULL,
                            ST( S_TARJETA_EN_BOLETIN_AUTORIZADA ), _F_SUP_TARJETA_EN_BOL, SI );
    return ( rta );
}
/*****************************************************************************/
int VALIDA_TARJETA_NUEVA()
/*****************************************************************************/
{
    int resultado;
    cambio_de_tarjeta = 1;
    resultado = VALIDA_TARJETA( NO, NO, VENTAS, NO, NULL, NO, -1/*NO*/ );
    cambio_de_tarjeta = 0;
    return ( resultado );
}
/*****************************************************************************/
int VALIDA_FECHA_VTO( unsigned fecha_vto )
/*****************************************************************************/
{
    int rta = 1;
    if( fecha_vto <= ( unsigned )GET_FECHA_DOS() ) {
        char mensaje[206]; //Max tamanio mensaje * 2
        strcpy( mensaje, ST( S_TARJETA_VENCIDA ) );
        strcat( mensaje, ".    " );
        strcat( mensaje, ST ( S_PRESIONE_ENTER_PARA_CONTINUAR ) );
        MENSAJE_CON_TECLA( mensaje, 13 );
        rta = 0;
    }
    else if( config.pais != CHILE && config.pais != BRASIL && config.pais != MEXICO
          && fecha_vto > ( unsigned )INC_YEAR( GET_FECHA_DOS(), 20 ) ) {
        MENSAJE_STRING( S_FEC_VIGENCIA_INVALIDA );
        rta = 0;
    }
    return ( rta );
}
/*****************************************************************************/
int MONEDA_EXTENDIDA()
/*****************************************************************************/
{
    int moneda;
    moneda = VAR_TARJETAS_MONEDA;
    if( moneda == DOLARES && TARJETA_CON_CECOR( VAR_TARJETAS_TARJETA ) ) {
        moneda = CECOR;
    }
    /** ya que se utiliza para indexar arrays, no puede ser menor a 0 **/
    else if ( moneda < 0 ) {
        moneda = 0;
    }
    return ( moneda );
}
/*****************************************************************************/
int TARJETA_CON_CECOR( int marca )
/*****************************************************************************/
{
    return ( TARJ[marca].monedas == 1 );
}
/*****************************************************************************/
int _elegir_cuotas( int comando, char *cadena, int dato, struct _browse *b, double monto2, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0;
    long reg = 0;
    double monto = 0.0;
    //char msg[70];
    char tmp[20];
    char tmp2[40];
	char cad[10];

	memset(tmp,0,sizeof(tmp));
	memset(tmp2,0,sizeof(tmp2));
	memset(cad,0,sizeof(cad));
    switch( comando ) {
        case _BUSCAR:
            ind_cuo_tar.nro_orden = atoi( cadena );
			/*SET_WHERE("CONCAT(COD_TARJETA,NRO_ORDEN) >= CONCAT(%i,%i)",
				ind_cuo_tar.cod_tarjeta,
				ind_cuo_tar.nro_orden);*/
			SET_WHERE( "(COD_TARJETA = %i and NRO_ORDEN >= %i)", 
            //SET_WHERE( "(COD_TARJETA = %i and NRO_ORDEN >= %i) or (COD_TARJETA = %i )", 
                    ind_cuo_tar.cod_tarjeta, ind_cuo_tar.nro_orden, ind_cuo_tar.cod_tarjeta );
            //GET_GREATER_E( ( char* )&ind_cuo_tar );
			SET_ORDER2( "COD_TARJETA, NRO_ORDEN" );
			RUN_QUERY(NO);
            if( cuo_tar.cod_tarjeta != ind_cuo_tar.cod_tarjeta ) {
                //GO_CUO_TAR( BOTTOM, cod_medio );
				//GO2(BOTTOM);
            }
            break;
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[20];
				double provisorio = 0.0;
				double variacion = 0.0;
				
				memset( xx, 0, sizeof(xx) );
					memset(xx,0,sizeof(xx));
                _itoa( cuo_tar.nro_orden, xx, 10 );
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, cuo_tar.descripcion );
				provisorio = RAM_IMPORTE;

				if( fabs(cuo_tar.coeficiente) > 0.01 )
					provisorio  = RAM_IMPORTE * cuo_tar.coeficiente;

				if( cuo_tar.hab_cuota_pesos != 'N' && fabs(cuo_tar.rec_cuota_pesos) > 0.1 ) {
					#ifdef VARIACION_FINANCIERA_SIN_RETENCION
						//Tarea 4982: Planeta no le aplica retenciones a los recargos financieros
						variacion = provisorio * cuo_tar.rec_cuota_pesos / 100;
						if( IMPRESORA_FISCAL == FISCAL ) {
							double tatalim= 0.00, aux = 0.00, dif = 0.00;
							tatalim = RETORNAR_TOTAL_PRINTER( variacion );
							aux = RAM_TOTAL + variacion;
							dif= fabs(tatalim)  - fabs(aux);
							if( fabs( dif ) > 0.01 && fabs( dif) < 0.02 )
								variacion+= dif;
						}
						provisorio = provisorio + variacion;
					#else
						provisorio = provisorio + ( provisorio * cuo_tar.rec_cuota_pesos / 100);
					#endif

				}
				FTOC( provisorio, tmp, ENTEROS + 1, DECIMALES, 0, 0 );
                
				LTRIM(tmp);
                if( UTILIZA_PROMOCIONES && UTILIZA_MODULO_PROMOCIONES && !RAM_NOTA_CR ) {
                    if(cuo_tar.exclusivo_promocion == 'S' && MEDIO_ACTIVADO_PROMO(cuo_tar.cod_tarjeta,cuo_tar.nro_orden, cod_medio )){
                        if((monto = MONTO_MEDIO_ACTIVADO_PROMO(cuo_tar.cod_tarjeta,cuo_tar.nro_orden, cod_medio )) < provisorio){
                            FTOC( monto, tmp2, ENTEROS + 1, DECIMALES, 0, 0 );
                        }else{
                            FTOC( provisorio, tmp2, ENTEROS + 1, DECIMALES, 0, 0 );
                        }
                        LTRIM(tmp2);
                        sprintf(xx,"%s",tmp2);
                    }else{
                        sprintf(xx,"%s",tmp);
                    }
                }else{
                    sprintf(xx,"%s",tmp);
                }

                setFieldValuepChar( b->index, 2, xx );
              
            }
            #endif
            MOSTRAR_ENTERO( 25 - ancho_panel / 2, 8 + dato, "99", cuo_tar.nro_orden );
            MOSTRAR_CADENA( 28 - ancho_panel / 2, 8 + dato, cuo_tar.descripcion );
            MOSTRAR_CADENA( 25 - ancho_panel / 2, 15, cadena );
            if( fabs(cuo_tar.coeficiente) > 0.01 ) {
                MOSTRAR_FLOTANTE( 49 - ancho_panel / 2, 8 + dato, ENTEROS, DECIMALES, NO, NO,
                                RAM_IMPORTE * cuo_tar.coeficiente );
            }
            break;
        case _SKIP:
            reg = GET_POSITION_REG();
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
			
			while( !rta 
					&& cuo_tar.dias_semana
					&& !CUOTA_VIGENTE( ) ) {
				SKIP2( (!dato) ? 1 : dato);
				rta = dbEOF() || dbBOF();
				if( rta ) {
					GET_DIRECT_REG(reg,GET_PAGINA_REG());
				}
            }

			if( !rta 
					&& cuo_tar.dias_semana
					&& CUOTA_VIGENTE( ) ) {
				memset( cad, 0, sizeof( cad ) );
				_snprintf( cad, sizeof( cad ), "%i",cuo_tar.nro_orden );
				 
				strncat( nro_orden, cad, sizeof( nro_orden ) - strlen( nro_orden ) -1 );
				strncat( nro_orden, ",", sizeof( nro_orden ) - strlen( nro_orden ) -1 );
			}  
			
			/*Si el plan es explusivo de promocion, verifica si fue activado por alguna promocion*/
            while(!rta && cuo_tar.exclusivo_promocion == 'S' && !MEDIO_ACTIVADO_PROMO(cuo_tar.cod_tarjeta,cuo_tar.nro_orden, cod_medio )){
                
                SKIP2( (!dato) ? 1 : dato);
                rta = dbEOF() || dbBOF();
                if(rta){
                    GET_DIRECT_REG(reg,GET_PAGINA_REG());
                }
            }
            if( !rta && cuo_tar.cod_tarjeta != ind_cuo_tar.cod_tarjeta ) {
                if( dato > 0 ) {
                    //GO_CUO_TAR( BOTTOM );
                }
                else {
                    //GO_CUO_TAR( TOP );
                }
                rta = 1;
            }

			if( rta && strlen( nro_orden ) ) {
				if( nro_orden[strlen(nro_orden)-1]== ',' )
					nro_orden[strlen(nro_orden)-1]=0;

				SET_WHERE( "(COD_TARJETA = %i and NRO_ORDEN IN( %s ) )", 
                    cuo_tar.cod_tarjeta, nro_orden );
				SET_ORDER2( "COD_TARJETA, NRO_ORDEN" );
				RUN_QUERY(NO);	

				memset( nro_orden, 0, sizeof( nro_orden ) );
			}
				
            break;
        case _VALIDAR:
            if( dato >= '0' && dato <= '9' ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
        case _COMPARAR:
			rta = 1;
            /*Validar si el plan seleccionado es exclusivo de promocion*/
			if( CANTIDAD_REGISTROS_REC(ARCH_REC_ACT_MEDIO_MOTOR, sizeof(struct activacion_medios ) ) ) {
                //HAY Promoción Activada 
                if( !MEDIO_ACTIVADO_PROMO(cuo_tar.cod_tarjeta,cuo_tar.nro_orden, cod_medio ) ) {
                    //El Submedio seleccionado NO Activa la Promoción
                    int teclado_alfabetico;
                    char car;

                    MENSAJE_SIN_SONIDO( ST( S_NO_UTILIZA_PROMO_ACTIVADA_S_N ), NO );
                    teclado_alfabetico = TECLADO_ALFABETICO;
                    TECLADO_ALFABETICO = 1;
                    do {
                        car = GETCH();
                    }
                    while( car != 'S' && car != 'N' && car != 's' && car != 'n' );
                    if( car == 'N' || car == 'n' ) {
                        BORRAR_MENSAJE();
                        TECLADO_ALFABETICO = teclado_alfabetico;
                        break;
                    }
                    BORRAR_MENSAJE();
                    TECLADO_ALFABETICO = teclado_alfabetico;
                }
            }
            /*Si el plan es exclusivo de promocion, verifica si fue activado por alguna promocion*/
            if(cuo_tar.exclusivo_promocion == 'S' && 
                ((monto = ROUND(MONTO_MEDIO_ACTIVADO_PROMO(cuo_tar.cod_tarjeta,cuo_tar.nro_orden, cod_medio ),
                    ENTEROS + 1, DECIMALES)) < RAM_IMPORTE)){
                FTOC( monto, tmp, ENTEROS + 1, DECIMALES, 0, 0 );
                LTRIM(tmp);
                //sprintf(msg,"RECALCULAR MONTO INGRESADO? ENTER = SI", tmp);
                MENSAJE_CON_TECLA_S_N("RECALCULAR MONTO INGRESADO? ENTER = SI",13,27);
                if(LASTKEY() == 13){
                    SET_MEMORY_DOUBLE( __ram_importe, monto );
                    rta = 1;
                }else{
                    rta = 0;
                }
            }else{
                if( !VAR_TARJETAS_FINANCIA_TARJETA
                && ( cuo_tar.hab_cuota_pesos == 'S' || cuo_tar.hab_cuota_dol == 'S' ) ) {
                    rta = 1;
                }
                else if( VAR_TARJETAS_FINANCIA_TARJETA
                    && ( cuo_tar.hab_cuota_fin_tar_pesos == 'S'
                        || cuo_tar.hab_cuota_fin_tar_dol == 'S' ) ) {
                    rta = 1;
                }
            }
            if(rta && cuo_tar.exclusivo_promocion == 'S' ){ 
                // si rta = 1 es porque dejo ingresar un importe, si es exclusivo promo, actualiza el mismo
                ACT_MONTO_MEDIO_ACTIVADO_PROMO(cuo_tar.cod_tarjeta,cuo_tar.nro_orden, RAM_IMPORTE, cod_medio );
            }
			if( cuo_tar.dias_semana
					&& !CUOTA_VIGENTE( ) ) {
						MENSAJE_SIN_SONIDO( ST( S_NO_ESTA_HABILITADA_LA_FACTURACION ), NO );
						rta =0;
			}
            break;
        case _TECLA:
            if( dato == -73 || dato == -81 ) {
                b->tecla = 0;
            }
            break;
    }
    return ( rta );
}
/*****************************************************************************/
int _elegir_cuotas_grupo( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0;
    switch( comando ) {
        case _BUSCAR:
            ind_cuo_tar_gru.nro_orden = atoi( cadena );
            //GET_GREATER_E( ( char* )&ind_cuo_tar_gru );
			SET_WHERE(/*"CONCAT(COD_TARJETA,COD_GRUPO,NRO_ORDEN) >= CONCAT(%i,%i,%i)"*/
					"(COD_TARJETA = %i and COD_GRUPO = %i and NRO_ORDEN >= %i)or(COD_TARJETA = %i and COD_GRUPO > %i)or COD_TARJETA > %i",
					ind_cuo_tar_gru.cod_tarjeta,
					ind_cuo_tar_gru.cod_grupo,
					ind_cuo_tar_gru.nro_orden,
					ind_cuo_tar_gru.cod_tarjeta,
					ind_cuo_tar_gru.cod_grupo,
					ind_cuo_tar_gru.cod_tarjeta);
			SET_ORDER2("COD_TARJETA,COD_GRUPO,NRO_ORDEN");
			RUN_QUERY(NO);
            if( cuo_tar.cod_tarjeta != ind_cuo_tar_gru.cod_tarjeta
             || cuo_tar.cod_grupo != ind_cuo_tar_gru.cod_grupo ) {
                //GO2( BOTTOM );
            }
            // b->pos = 0;
            // cadena[0] = ' ';
            break;
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[20];
                #ifdef INVEL_L
                sprintf( xx, "%d", cuo_tar.nro_orden );
                #else
                _itoa( cuo_tar.nro_orden, xx, 10 );
                #endif
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, cuo_tar.descripcion );
                if( cuo_tar.coeficiente != 0.00 ) {
                    FTOC( ( RAM_IMPORTE * cuo_tar.coeficiente ), xx, ENTEROS, DECIMALES, NO, NO );
                    setFieldValuepChar( b->index, 2, xx );
                }
            }
            #endif
            MOSTRAR_ENTERO( 25 - ancho_panel / 2, 8 + dato, "99", cuo_tar.nro_orden );
            MOSTRAR_CADENA( 28 - ancho_panel / 2, 8 + dato, cuo_tar.descripcion );
            if( cuo_tar.coeficiente !=  0.00 ) {
                MOSTRAR_FLOTANTE( 49 - ancho_panel / 2, 8 + dato, ENTEROS, DECIMALES, NO, NO,
                                ( long )( RAM_IMPORTE * cuo_tar.coeficiente ) );
            }
            MOSTRAR_CADENA( 25 - ancho_panel / 2, 15, cadena );
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
            if( !rta
             && ( cuo_tar.cod_tarjeta != ind_cuo_tar_gru.cod_tarjeta
               || cuo_tar.cod_grupo != ind_cuo_tar_gru.cod_grupo ) ) {
                if( dato > 0 ) {
                    //GO_CUO_TAR_GRU( BOTTOM );
					//GO2(BOTTOM);
                }
                else {
                    //GO_CUO_TAR_GRU( TOP );
					GO2(TOP);
                }
                rta = 1;
            }
            break;
        case _VALIDAR:
            if( dato >= '0' && dato <= '9' ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
        case _COMPARAR:
            if( !VAR_TARJETAS_FINANCIA_TARJETA
             && ( cuo_tar.hab_cuota_pesos == 'S' || cuo_tar.hab_cuota_dol == 'S' ) ) {
                rta = 1;
            }
            else if( VAR_TARJETAS_FINANCIA_TARJETA
                  && ( cuo_tar.hab_cuota_fin_tar_pesos == 'S'
                    || cuo_tar.hab_cuota_fin_tar_dol == 'S' ) ) {
                rta = 1;
            }
            break;
        case _TECLA:
            if( dato == -73 || dato == -81 ) {
                b->tecla = 0;
            }
            break;
    }
    return ( rta );
}
/*****************************************************************************/
int _elegir_grupo( int comando, char *cadena, int dato, struct _browse *b )
/*****************************************************************************/
{
    int rta = 0;
    switch( comando ) {
        case _BUSCAR:
            ind_cuo_gru.cod_grupo = atoi( cadena );
            //GET_GREATER_E( ( char* )&ind_cuo_gru );
			SET_WHERE( "COD_TARJETA >= '%i' AND COD_GRUPO >= '%i'", ind_cuo_gru.cod_tarjeta,
				ind_cuo_gru.cod_grupo );
			RUN_QUERY( NO );
            if( cuo_gru->cod_tarjeta != ind_cuo_gru.cod_tarjeta ) {
                GO_CUO_GRU( BOTTOM );
            }
            // b->pos = 0;
            // cadena[0] = ' ';
            break;
        case _MOSTRAR:
            MOSTRAR_ENTERO( 30 - ancho_panel / 2, 8 + dato, "99", cuo_gru->cod_grupo );
            MOSTRAR_CADENA( 33 - ancho_panel / 2, 8 + dato, cuo_gru->descripcion );
            MOSTRAR_CADENA( 30 - ancho_panel / 2, 15, cadena );
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
            if( !rta && cuo_gru->cod_tarjeta != ind_cuo_gru.cod_tarjeta ) {
                if( dato > 0 ) {
                    GO_CUO_GRU( BOTTOM );
                }
                else {
                    GO_CUO_GRU( TOP );
                }
                rta = 1;
            }
            break;
        case _VALIDAR:
            if( isdigit( dato ) ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
        case _COMPARAR:
            rta = 1;
            break;
        case _TECLA:
            if( dato == -73 || dato == -81 ) {
                b->tecla = 0;
            }
            break;
    }
    return ( rta );
}
/*****************************************************************************/
int ELEGIR_CUOTAS( int marca, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0, grupo;
    if( MENU_CUOTAS_POR_GRUPO ) {
        if( cuo_gru == NULL ) {
            cuo_gru = ( struct _cuo_gru *)MALLOC( sizeof( struct _cuo_gru ) );
        }
        grupo = ELEGIR_GRUPO_CUOTAS( marca, cod_medio );
        if( grupo ) {
            rta = _ELEGIR_CUOTAS( marca, grupo, cod_medio );
            if( rta ) {
                SET_MEMORY_INT( __var_tarjetas_grupo_cuota, grupo );
            }
        }
    }
    else {
        rta = _ELEGIR_CUOTAS( marca, NO, cod_medio );
    }
    return ( rta );
}
/*****************************************************************************/
int _ELEGIR_CUOTAS( int marca, int grupo, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0, ok = 0, CantCuotas = 0;
    /*---------------------------- Abre el archivo ---------------------------*/
    //USE_DB( AREA_AUX, _CUO_TAR_SIC, ( char* )&cuo_tar, sizeof( cuo_tar ), NULL, 0, 0 );
	if((rta=OPEN_TABLE(T_CUO_TAR, TT_ORIG, (char* )&cuo_tar,sizeof( cuo_tar )))!= 0){
		return rta;
	}
	SELECT_TABLE( T_CUO_TAR, TT_ORIG );
    /*if( grupo ) {
        SET_ORDER2( "COD_TARJETA,COD_GRUPO,NRO_ORDEN" );
        ind_cuo_tar_gru.cod_tarjeta = (unsigned short)marca + 1;
        ind_cuo_tar_gru.cod_grupo = (short)grupo;
        //GO_CUO_TAR_GRU( TOP );
		//GO2(TOP);
    }
    else {
        //SET_ORDER( 2 );
		SET_ORDER2("COD_TARJETA,NRO_ORDEN");
        ind_cuo_tar.cod_tarjeta = marca + 1;
        //GO_CUO_TAR( TOP );
		//GO2(TOP);
    }*/
    /*-------------------------- Prepara la pantalla -------------------------*/
    _ATRIBUTO = 120;
    MOSTRAR_CADENA( 23 - ancho_panel / 2, 14,
                    "������������������" );
    if(!grupo){
        //SET_ORDER( 2 );
		SET_ORDER2("COD_TARJETA, NRO_ORDEN");
        ind_cuo_tar.cod_tarjeta = marca + 1;
		//GET_EQUAL((char*)&ind_cuo_tar);
		SET_WHERE("COD_TARJETA ='%d' AND NRO_ORDEN >= '%d'", ind_cuo_tar.cod_tarjeta, ind_cuo_tar.nro_orden );
		RUN_QUERY(NO);
        //CantCuotas = CONTAR_CUOTAS(marca, cod_medio);
		//la funcion anterior no contaba siempre bien los registros, sera indice? uso ahora consulta directa
		CantCuotas = CONTAR_CUOTAS2(marca, cod_medio);
		{
			char mensa[50];
			sprintf(mensa,"CantCuotas %i Asignar CUo %i",CantCuotas,ASIGNAR_CUOTA_UNICA );
			glog(mensa,1,1);
		}
        if(ASIGNAR_CUOTA_UNICA && CantCuotas == 1){ //si esta configurado para autoasignar unica cuota y solo hay una cuota
            ok = 1;
			glog("Cuota Unica",1,1);
        }else{
            //GO_CUO_TAR( TOP, cod_medio );
            ok = Consulta_cuotas_tarjeta( _elegir_cuotas, cod_medio ); 
			{
			char mensa[50];
			sprintf(mensa,"Debio Salir Broser Ok%i CUota %i",ok, cuo_tar.nro_cuota);
			glog(mensa,1,1);
		}
        }  
    }else{
        //SET_ORDER( 3 );
		glog("Tarjeta fuera de grupo",1,1);
		SET_ORDER2("COD_TARJETA, COD_GRUPO, NRO_ORDEN");
        ind_cuo_tar_gru.cod_tarjeta = (unsigned short)marca + 1;
        ind_cuo_tar_gru.cod_grupo = (short)grupo;
        GO_CUO_TAR_GRU( TOP );
        ok = Consulta_cuotas_tarjeta( _elegir_cuotas_grupo, cod_medio );  
    }
    if( ok ) {
        rta = cuo_tar.nro_cuota;
    }
    //CLOSE_DATABASE( AREA_AUX );
	CLOSE_TABLE( T_CUO_TAR, TT_ORIG );
    return ( rta );
}
/*****************************************************************************/
int CONTAR_CUOTAS(int marca, unsigned short cod_medio )
/*****************************************************************************/
{
    int cnt = 0, reg = 0;
    while( /*!BtrvEOF() && !BOF()*/ !dbEOF() && !dbBOF() ){
        if( cuo_tar.exclusivo_promocion == 'S' && ( cuo_tar.cod_tarjeta == marca + 1 ) 
				&& MEDIO_ACTIVADO_PROMO(cuo_tar.cod_tarjeta,cuo_tar.nro_orden, cod_medio ) ) {
            cnt++;
            reg = GET_POSITION_REG();
        }else if(cuo_tar.exclusivo_promocion != 'S' && cuo_tar.cod_tarjeta == marca + 1){
            cnt++;
            reg = GET_POSITION_REG();
        }
        SKIP2(1);
    }
    if(cnt == 1){ //si solo hay uno, se para en ese
        GET_DIRECT_REG( reg, 0);
    }
    return cnt;
}
/*****************************************************************************/
int CONTAR_CUOTAS2(int marca, unsigned short cod_medio )
/*****************************************************************************/
{
    int cnt = 0, reg = 0;
	int cod_tarjeta=0, nro_orden=0;
	
	reg =DIRECT_DB_QUERY("SELECT count(*) FROM cuo_tar c where exclusivo_promocion = \"N\" and cod_tarjeta = %i",marca+1);
	GET_DATO( 1,C_INT,(char *)&cnt,sizeof(cnt));

	reg =DIRECT_DB_QUERY("SELECT cod_tarjeta,nro_orden FROM cuo_tar c where exclusivo_promocion = \"S\" and cod_tarjeta = %i",marca+1);
	do{ 
		if ( reg ) {
			GET_DATO( 1, C_INT, (char *)&cod_tarjeta , sizeof( cod_tarjeta )); 
			GET_DATO( 2, C_INT, (char *)&nro_orden , sizeof( nro_orden)); 
			if( MEDIO_ACTIVADO_PROMO(cod_tarjeta,nro_orden, cod_medio )){
				cnt++;
			}
		}
	} while (reg = DIRECT_DB_SKIP(1) );

    return cnt;
}

/*****************************************************************************/
int ELEGIR_GRUPO_CUOTAS( int marca, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0, ok;
    /*---------------------------- Abre el archivo ---------------------------*/
	OPEN_TABLE( T_CUO_GRU, TT_ORIG, ( char* )cuo_gru, sizeof( struct _cuo_gru ) );
    ind_cuo_gru.cod_tarjeta = marca + 1;
    GO_CUO_GRU( TOP );
    /*-------------------------- Prepara la pantalla -------------------------*/
    ok = Consulta_cuotas_tarjeta( _elegir_cuotas, cod_medio );
    if( ok ) {
        rta = cuo_tar.nro_cuota;
    }
    CLOSE_TABLE( T_CUO_GRU, TT_ORIG );
    return ( rta );
}
/*****************************************************************************/
void GO_CUO_TAR( int modo, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0;
    if( modo == TOP ) {
        ind_cuo_tar.nro_orden = 0;
        //GET_GREATER_E( ( char* )&ind_cuo_tar );
		SET_WHERE("(COD_TARJETA ='%d' AND NRO_ORDEN >= '%d') OR COD_TARJETA >'%d' ", 
						ind_cuo_tar.cod_tarjeta, ind_cuo_tar.nro_orden, ind_cuo_tar.cod_tarjeta );
		RUN_QUERY(NO);

        while(!rta && cuo_tar.exclusivo_promocion == 'S' 
				&& !MEDIO_ACTIVADO_PROMO(cuo_tar.cod_tarjeta,cuo_tar.nro_orden,cod_medio)) {
            SKIP2(1);
            rta = /*BtrvEOF() || BOF()*/ dbEOF() || dbBOF();
        }
    }
    else if( modo == BOTTOM ) {
        ind_cuo_tar.nro_orden = 99;
        //GET_GREATER_E( ( char* )&ind_cuo_tar );
        //SKIP( -1 );
		SET_WHERE("(COD_TARJETA ='%d' AND NRO_ORDEN >= '%d') OR COD_TARJETA >'%d' ", 
						ind_cuo_tar.cod_tarjeta, ind_cuo_tar.nro_orden, ind_cuo_tar.cod_tarjeta );
		RUN_QUERY(NO);
		SKIP2( -1 );
    }
}
/*****************************************************************************/
void GO_CUO_TAR_GRU( int modo )
/*****************************************************************************/
{
    if( modo == TOP ) {
        ind_cuo_tar_gru.nro_orden = 0;
        //GET_GREATER_E( ( char* )&ind_cuo_tar_gru );
		SET_WHERE( "(COD_TARJETA = '%d' AND COD_GRUPO = '%d', AND NRO_ORDEN >= '%d') OR \
					(COD_TARJETA = '%d' AND COD_GRUPO > '%d') OR \
				    (COD_TARJETA > '%d')", 
					ind_cuo_tar_gru.cod_tarjeta, ind_cuo_tar_gru.cod_grupo, ind_cuo_tar_gru.nro_orden,
	     		    ind_cuo_tar_gru.cod_tarjeta, ind_cuo_tar_gru.cod_grupo, 
					ind_cuo_tar_gru.cod_tarjeta );
    }
    else if( modo == BOTTOM ) {
        ind_cuo_tar_gru.nro_orden = 99;
        //GET_GREATER_E( ( char* )&ind_cuo_tar_gru );
        //SKIP( -1 );
		SET_WHERE( "(COD_TARJETA = '%d' AND COD_GRUPO = '%d', AND NRO_ORDEN >= '%d') OR \
					(COD_TARJETA = '%d' AND COD_GRUPO > '%d') OR \
				    (COD_TARJETA > '%d')", 
					ind_cuo_tar_gru.cod_tarjeta, ind_cuo_tar_gru.cod_grupo, ind_cuo_tar_gru.nro_orden,
	     		    ind_cuo_tar_gru.cod_tarjeta, ind_cuo_tar_gru.cod_grupo, 
					ind_cuo_tar_gru.cod_tarjeta );
		SKIP2( -1 );
    }
}
/*****************************************************************************/
void GO_CUO_GRU( int modo )
/*****************************************************************************/
{
    if( modo == TOP ) {
        ind_cuo_gru.cod_grupo = 0;
        //GET_GREATER_E( ( char* )&ind_cuo_gru );
		SET_WHERE( "COD_TARJETA >= '%i' AND COD_GRUPO >= '%i'", ind_cuo_gru.cod_tarjeta,
			ind_cuo_gru.cod_grupo );
    }
    else if( modo == BOTTOM ) {
        ind_cuo_gru.cod_grupo = 99;
        //GET_GREATER_E( ( char* )&ind_cuo_gru );
		SET_WHERE( "COD_TARJETA >= '%i' AND COD_GRUPO >= '%i'", ind_cuo_gru.cod_tarjeta,
			ind_cuo_gru.cod_grupo );
        SKIP2( -1 );
    }
	RUN_QUERY( NO );
}
/*****************************************************************************/
int VALIDA_LARGO_NUMERO( int marca, int largo )
/*****************************************************************************/
{
    int rta = 1;
    if( TARJ[marca].largo1 ) {
        if( largo != TARJ[marca].largo1 && largo != TARJ[marca].largo2 ) {
            rta = 0;
        }
    }
    return ( rta );
}
/***********************************************************************/
long NRO_TERMINAL( int marca )
/***********************************************************************/
{
    struct
    {
        unsigned short caja;
        char nodo;
    } indice;
    long rta = 0;
    struct _ter_caja ter_caja;
    indice.nodo = TARJ[marca].nodo;
    indice.caja = (unsigned short)NRO_CAJA;
    if( /*USE_DB( AREA_AUX, _TER_CAJA_SIC, ( char* )&ter_caja, sizeof( ter_caja ), NULL, 0, 0 )*/
			OPEN_TABLE(T_TER_CAJA, TT_ORIG, ( char* )&ter_caja, sizeof( ter_caja ))== 0 ) {
		SELECT_TABLE( T_TER_CAJA, TT_ORIG );
		SET_WHERE("CAJA = '%i' and NODO = '%i'",indice.caja,indice.nodo);
		RUN_QUERY(NO);
        //GET_EQUAL( ( char* )&indice );
        if( FOUND2() ) {
            rta = ter_caja.terminal;
        }
        CLOSE_TABLE( T_TER_CAJA, TT_ORIG );
    }
    return ( rta );
}
/***********************************************************************/
int LEE_CUO_TAR( int marca, int cuotas )
/***********************************************************************/
{
    struct
    {
        unsigned short cod_tarjeta;
        char nro_cuota;
    } indice;
    int rta;
    indice.cod_tarjeta = (unsigned short)marca + 1;
    indice.nro_cuota = cuotas;
    //USE_DB( AREA_AUX, _CUO_TAR_SIC, ( char* )&cuo_tar, sizeof( cuo_tar ), NULL, 0, 0 );
	if(OPEN_TABLE(T_CUO_TAR, TT_ORIG, (char* )&cuo_tar,sizeof( cuo_tar ))!= 0){
		return 0;
	}
	//SELECT_TABLE( T_CUO_TAR, TT_ORIG );
	SET_WHERE("COD_TARJETA = '%u' AND NRO_CUOTA = '%u'",
				indice.cod_tarjeta,
				indice.nro_cuota);
	RUN_QUERY(NO);
    //GET_EQUAL( ( char* )&indice );
    rta = FOUND2();
    //CLOSE_DATABASE( AREA_AUX );
	CLOSE_TABLE( T_CUO_TAR, TT_ORIG );

    return ( rta );
}
/***********************************************************************/
int CAJA_HABILITADA_PARA_CUOTAS()
/***********************************************************************/
{
    struct _cajas cajas;
    int rta = 0, ind;
    if( /*USE_DB( 0, _CAJAS_SIC, ( char* )&cajas, sizeof( cajas ), NULL, 0, 0 )*/
		OPEN_TABLE(T_CAJAS, TT_ORIG,( char* )&cajas, sizeof( cajas )) == 0 ) {
        ind = NRO_CAJA;
        //GET_EQUAL( ( char* )&ind );
		SELECT_TABLE( T_CAJAS, TT_ORIG );
		SET_WHERE("CAJA = '%i'",ind);
		RUN_QUERY(NO);
        if( FOUND2() && cajas.habilitada_cuotas == 'S' ) {
            rta = 1;
        }
        CLOSE_TABLE( T_CAJAS, TT_ORIG );
    }
    return ( rta );
}
/***********************************************************************/
void CARGAR_SERIAL_IMPRESORA(void)
/***********************************************************************/
{
    struct _cajas cajas;
    int  ind;
    if((/*1||*/ CONTROLAR_SERIAL_PRINTER )&& OPEN_TABLE(T_CAJAS, TT_ORIG,( char* )&cajas, sizeof( cajas )) == 0 ) {
        ind = NRO_CAJA;
        SELECT_TABLE( T_CAJAS, TT_ORIG );
		SET_WHERE("CAJA = '%i'",ind);
		RUN_QUERY(NO);
        if( FOUND2() ) {
            SET_MEMORY( __ram_nro_serie_impresora_ant,cajas.serial); 
        }
        CLOSE_TABLE( T_CAJAS, TT_ORIG );
    }
}
/*****************************************************************************/
int _elegir_vigencia( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0;
    switch( comando ) {
        case _BUSCAR:
            ind_vigencia.nro_orden = atoi( cadena );
			SET_WHERE("(COD_TARJETA = %i and NRO_ORDEN >= %i) or COD_TARJETA > %i",
				ind_vigencia.cod_tarjeta,
				ind_vigencia.nro_orden,
				ind_vigencia.cod_tarjeta);
			SET_ORDER2("COD_TARJETA,NRO_ORDEN");
			RUN_QUERY(NO);
            break;
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[10];
                _itoa( vigencia.nro_orden, xx, 10 );
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, vigencia.nombre );
            }
            #endif
            MOSTRAR_ENTERO( 30 - ancho_panel / 2, 8 + dato, "9", vigencia.nro_orden );
            MOSTRAR_CADENA( 33 - ancho_panel / 2, 8 + dato, vigencia.nombre );
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
            if( !rta && vigencia.cod_tarjeta != ind_vigencia.cod_tarjeta ) {
                /*if( dato > 0 ) {
                    GO_VIGENCIA( BOTTOM );
                }
                else {
                    GO_VIGENCIA( TOP );
                }*/
                rta = 1;
            }
            break;
        case _VALIDAR:
            if( isdigit( dato ) ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
            rta = 1;
            break;
        case _TECLA:
            if( dato == -73 || dato == -81 ) {
                b->tecla = 0;
            }
            break;
		case _LIMIT:
            if( dato > 0 ) {
                SET_LIMIT( dato, SI );
            } else {
                SET_LIMIT( NO, NO );
            }
            break;
    }
    return ( rta );
}
/*****************************************************************************/
int ELEGIR_VIGENCIA( int marca )
/*****************************************************************************/
{
    int rta = 0,ok = 0;
    /*---------------------------- Abre el archivo ---------------------------*/
    //USE_DB( AREA_AUX, _VIGENCIA_SIC, ( char* )&vigencia, sizeof( vigencia ), NULL, 0, 0 );
	if( OPEN_TABLE(T_VIGENCIA, TT_ORIG,( char* )&vigencia, sizeof( vigencia ) ) == 0 ) {
		//SET_ORDER( 2 );
		ind_vigencia.cod_tarjeta = (unsigned short)marca + 1;
		//GO_VIGENCIA( TOP );
		/*-------------------------- Prepara la pantalla -------------------------*/
		ok = Consulta_vigencia( _elegir_vigencia );
		if( ok ) {
			rta = vigencia.cod_vigencia;
		}
		CLOSE_TABLE( T_VIGENCIA, TT_ORIG );
	}
    return ( rta );
}
/*****************************************************************************/
void GO_VIGENCIA( int modo )
/*****************************************************************************/
{
	char ax[20];
    if( modo == TOP ) {
        ind_vigencia.nro_orden = 0;		
		sprintf(ax,"NRO_ORDEN");
		SET_WHERE("NRO_ORDEN >= '%i'",ind_vigencia.nro_orden);
        //GET_GREATER_E( ( char* )&ind_vigencia );
    }
    else if( modo == BOTTOM ) {
        ind_vigencia.nro_orden = 99;
		sprintf(ax,"%i - NRO_ORDEN",ind_vigencia.nro_orden);
		SET_WHERE("NRO_ORDEN <= '%i'",ind_vigencia.nro_orden);
        //GET_GREATER_E( ( char* )&ind_vigencia );
        //SKIP( -1 );
    }
	SET_ORDER2(ax);
	//SET_WHERE("NRO_ORDEN >= '%i'",ind_vigencia.nro_orden);
	RUN_QUERY(NO);
}
/*****************************************************************************/
int BUSCAR_VIGENCIA( int cod_tarjeta, int cod_vigencia, char *nom )
/*****************************************************************************/
{
    int rta = 0;
    struct _ind3_vigencia ind;
    if( /*USE_DB( 0, _VIGENCIA_SIC, ( char* )&vigencia, sizeof( vigencia ), NULL, 0, 0 )*/
		OPEN_TABLE(T_VIGENCIA, TT_ORIG, ( char* )&vigencia, sizeof( vigencia ))== 0 ) {
        //SET_ORDER( 3 );
		SELECT_TABLE( T_VIGENCIA, TT_ORIG );
        ind.cod_tarjeta = (unsigned short)cod_tarjeta + 1;
        ind.cod_vigencia = (char)cod_vigencia;
		SET_WHERE("COD_TARJETA = '%i' and COD_VIGENCIA = '%i'",
					ind.cod_tarjeta,
					ind.cod_vigencia);
        //GET_EQUAL( ( char* )&ind );
		RUN_QUERY(NO);
        rta = FOUND2();
        if( rta && nom ) {
            strcpy( nom, vigencia.nombre );
        }
        CLOSE_TABLE( T_VIGENCIA, TT_ORIG );
    }
    return ( rta );
}
/*****************************************************************************/
double PIDE_ENTREGA( double importe )
/*****************************************************************************/
{
    double entrega;
    entrega = -1;
    while( entrega == -1 ) {
        entrega = _PIDE_ENTREGA( ST( S__IMPORTE_ENTREGA_ ), ENTEROS, DECIMALES, NO );
        if( entrega < 0 || entrega >= importe ) {
            entrega = -1;
        }
    }
    return ( entrega );
}
/*****************************************************************************/
double _PIDE_ENTREGA( char *titulo, int enteros, int decimales, int signo )
/*****************************************************************************/
{
    double importe, total = 0, s = 1;
    int salir = 0;
    while( !salir ) {
        /* if( signo ) { importe = PEDIR_GRAN_FLOTANTE_CON_SIGNO( 29, 22,
         * enteros, decimales ); } else { importe = PEDIR_GRAN_FLOTANTE( 29,
         * 22, enteros, decimales ); } */
        if( signo ) {
            importe = PEDIR_GRAN_FLOTANTE_CON_SIGNO( 32 - ancho_panel / 2, 20, enteros, decimales );
        }
        else {
            importe = PEDIR_GRAN_FLOTANTE( 32 - ancho_panel / 2, 20, enteros, decimales );
        }
        switch( LASTKEY() ) {
            case 13:
                salir = 1;
                total += importe * s;
                break;
            case 27:
                salir = 2;
                total = 0;
                break;
            case '+':
                total += importe * s;
                s = 1;
                break;
            case '-':
                total += importe * s;
                s = -1;
                break;
        }
    }
    return ( total );
}
/*****************************************************************************/
int CONFIRMAR_NUMERO_TARJETA( int marca )
/*****************************************************************************/
{
    char aux[80],*n;
    int rta = 1, intentos = 1, nro=0, nro_ok=0, seguir = 1;
    n = GET_NUMERO_TARJETA_PRIVADO();

    if( TARJ[marca].confirmar_numero_tarjeta == 'S' ) {
        memcpy( aux, &n[strlen( n ) - 4], 4 );
        aux[4] = 0;
        nro_ok = atoi( aux );
        MENSAJE_SIN_SONIDO_STRING( S_INGRESE_LOS_ULTIMOS_4_DIGITOS_DE_LA_TARJETA );
		VACIAR_KEY();
        while( intentos < 3 && seguir ) {
            if( nro != 0 && intentos > 1 ) {
                MENSAJE_STRING( S_INGRESE_NUEV_4_DIGITOS );
            }
            nro = ( int )PEDIR_ENTERO_EN_RECUADRO( "DIGITOS TARJETA", NO , 4 );
            if( nro < 0 ) {
                seguir = 0;
                rta = 0;
            }
            else if( nro == nro_ok ) {
                seguir = 0;
            }
            intentos++;
        }
        BORRAR_MENSAJE();
        if( seguir ) {
            int dd= strlen( n);
            _snprintf(&n[dd -4],4,"XXXX"); 
            rta = 0;
            sprintf( aux, ST( S_LLAMAR_CODIGO_10__TARJETA___S_ ), n );
            MENSAJE( aux );
            while( GETCH() != 27 );
        }
    }
    return ( rta );
}
/*****************************************************************************/
int PEDIR_CUPON_ORIGINAL()
/*****************************************************************************/
{
    int rta = 0, salir = 0;
    long nro;

	while( !salir ) {
		nro = PEDIR_ENTERO_XY( ( ( config.pais == CHILE || config.pais == BRASIL ) ) ? 
				(char *)"BOLETA ORIGINAL"
                         : ST( S_CUPON_ORIGINAL ), 40 - ancho_panel / 2, 22, 8 );
		if( nro > 0 ) {
        SET_MEMORY_LONG( __var_tarjetas_cupon_original, nro );
			salir = 1;
			rta = 1;
		} else {
			MENSAJE_CON_PAUSA_CAJA( 
				"DATOS REQUERIDOS PARA CONTINUAR CON LA TRANSACCION", 30 ); 
			SET_MEMORY_LONG( __var_tarjetas_cupon_original, 0 );
			if( nro == -1 ) {
				salir = 1;
    }
			BORRAR_MENSAJE( );
		}
	}

    return rta;
}
/*****************************************************************************/
int PEDIR_FECHA_ORIGINAL()
/*****************************************************************************/
{
    unsigned fecha;
    int pant = 0, rta = 0;
    ExecuteSSAction( A_PEDIR, 1, ST( S__FECHA_ORIGINAL_ ), "FECHA:", "  -  -  ", " " );
    fecha = PEDIR_GRAN_FECHA( 29 - ancho_panel / 2, 23, NULL );
    if( fecha == 0xffff || !fecha ) {
        rta = 0;
		SET_MEMORY_INT( __var_tarjetas_fecha_original, 0 );
    }
    else {
        SET_MEMORY_INT( __var_tarjetas_fecha_original, fecha );
        rta = 1;
    }
    ExecuteAction( A_FIN_PEDIR );
    return ( rta );
}
/*****************************************************************************/
int BUSCAR_EN_BOLETIN( int marca )
/*****************************************************************************/
{
    int error, rta = 1;
    error = BUSCA_TARJETA( GET_NUMERO_TARJETA_PRIVADO(), ( char )marca );
	
    if( error == 0 ) {
        /*------------- valida boletin vencido -----------*/
        if( VALIDA_BOLETIN( ( char )NUMERO_ARCHIVO( marca ) ) ) {
            SET_MEMORY_CHAR( __var_tarjetas_boletin_vencido, 0 );
            rta = 1;
        }
        else {
            SET_MEMORY_CHAR( __var_tarjetas_boletin_vencido, 1 );
            MENSAJE_STRING( S_BOLETIN_VENCIDO );
            rta = SOLICITAR_CLAVES( 60 - ancho_panel, 19, NO, SI, SI, ST( S_BOLETIN_VENCIDO_ ),
                                    ST( S_BOLETIN_VENCIDO_ ), _F_SUP_BOLETIN_VENCIDO, SI );
        }
        SET_MEMORY_INT( __var_tarjetas_nro_boletin,
                        NUMERO_BOLETIN( ( char )NUMERO_ARCHIVO( marca ) ) );
    }
    else if( error == 1 ) {
        MENSAJE_STRING( S_TARJ_EN_BOL_PROTECTIVO );
        ALARMA( 3 );
        rta = 0;
        if( VAR_TARJETAS_LIMITE == 0 ) {
            rta = 1;
            SET_MEMORY_CHAR( __var_tarjetas_aut_en_boletin, 1 );
        }
        else if( PUEDE_AUTORIZAR_TARJETA_EN_BOLET ) {
            rta = AUTORIZA_TARJETA_EN_BOLETIN();
            if( rta ) {
                SET_MEMORY_CHAR( __var_tarjetas_aut_en_boletin, 1 );
            }
        }
    }
    else if( error == 2 ) {
        MENSAJE_STRING( S_ERROR_EN_BOL_PROTECTIV );
        rta = 0;
    }
    return rta;
}
/*****************************************************************************/
int _elegir_diferimiento( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0;
    switch( comando ) {
        case _BUSCAR:
            indice_diferimiento.nro_diferimiento = atoi( cadena );
            //GET_GREATER_E( ( char* )&indice_diferimiento );
			SET_WHERE("COD_DIFER >= '%i' ", indice_diferimiento.nro_diferimiento );
			SET_ORDER2("COD_DIFER");
			RUN_QUERY(NO);
            if( dif_tar.cod_tarjeta != indice_diferimiento.cod_tarjeta ) {
                indice_diferimiento.nro_diferimiento = 99;
                //GET_GREATER_E( ( char* )&indice_diferimiento );
				SET_WHERE("COD_DIFER >= '%i' ", indice_diferimiento.nro_diferimiento );
				SET_ORDER2("COD_DIFER");
				RUN_QUERY(NO);
                //SKIP( -1 );
				SKIP2( -1 );
            }
            break;
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[10];
                #ifdef INVEL_L
                sprintf( xx, "%d", dif_tar.nro_diferimiento );
                #else
                _itoa( dif_tar.nro_diferimiento, xx, 10 );
                #endif
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, dif_tar.descripcion );
            }
            #endif
            MOSTRAR_ENTERO( 30 / 2, 8 + dato, "99", dif_tar.nro_diferimiento );
            MOSTRAR_CADENA( 38 / 2, 8 + dato, dif_tar.descripcion );
            MOSTRAR_CADENA( 30 / 2, 15, cadena );
            break;
        case _SKIP:
            /*SKIP( dato );
            rta = BtrvEOF() || BOF();*/
			SKIP2( dato );
            rta = dbEOF() || dbBOF();
            if( !rta && dif_tar.cod_tarjeta != indice_diferimiento.cod_tarjeta ) {
                if( dato > 0 ) {
                    indice_diferimiento.nro_diferimiento = 99;
                    /*GET_GREATER_E( ( char* )&indice_diferimiento );
                    SKIP( -1 );*/
					SET_WHERE("COD_DIFER >= '%i' ", indice_diferimiento.nro_diferimiento );
					SET_ORDER2("COD_DIFER");
					RUN_QUERY(NO);
					//SKIP( -1 );
					SKIP2( -1 );
                }
                else {
                    indice_diferimiento.nro_diferimiento = 0;
                    //GET_GREATER_E( ( char* )&indice_diferimiento );
					SET_WHERE("COD_DIFER >= '%i' ", indice_diferimiento.nro_diferimiento );
					SET_ORDER2("COD_DIFER");
					RUN_QUERY(NO);
                }
                rta = 1;
            }
            break;
        case _VALIDAR:
            if( isdigit( dato ) ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? BOF() : BtrvEOF();
            break;
        case _ENTER:
        case _COMPARAR:
            rta = 1;
            break;
        case _TECLA:
            if( dato == -73 || dato == -81 ) {
                b->tecla = 0;
            }
            break;
    }
    return ( rta );
}
/*****************************************************************************/
int ELEGIR_DIFERIMIENTO( int marca )
/*****************************************************************************/
{
    int rta = 0, ok = 0;
    /*---------------------------- Abre el archivo ---------------------------*/
	SET_ORDER2( "COD_TARJETA, COD_DIFER");
	SET_WHERE("COD_TARJETA = '%i' AND COD_DIFER = '%i' ", (unsigned short)marca, 0 );
	RUN_QUERY(NO);
    ok = Consulta_diferimiento( _elegir_diferimiento );
    if( ok ) {
        rta = dif_tar.nro_diferimiento;
    }
    return ( rta );
}
/*------------------------*/
/********************************************************************************/
int TARJ_PIDE_DOCUMENTO( char *documento )
/********************************************************************************/
{
    int rta = 1;
    if( config.pais == COLOMBIA ) {
        MOSTRAR_CADENA( 35 - ancho_panel / 2, 21, ST( S__COMPROBANTE_ ) );
    }
    else {
		ExecuteSSAction( A_PEDIR, _SIMPLE, ST( S__DOCUMENTO_ ), ST( S__DOCUMENTO_ ), NULL, " " );
        MOSTRAR_CADENA( 35 - ancho_panel / 2, 21, ST( S__DOCUMENTO_ ) );
    }
    rta = PEDIR_GRAN_CADENA( ( char )( 23 - ancho_panel / 2 ), ( char )22, ( char )10, documento,
                             NULL );
	ExecuteAction( A_FIN_PEDIR );
    return ( rta );
}
/********************************************************************************/
int TARJ_PIDE_TELEFONO( char *telefono )
/********************************************************************************/
{
    int rta = 1;
	
	ExecuteSSAction( A_PEDIR, _SIMPLE, ST( S__TELEFONO_ ), ST( S__TELEFONO_ ), NULL, " " );
    rta = PEDIR_GRAN_CADENA( ( char )( 23 - ancho_panel / 2 ), ( char )22, ( char )10, telefono, NULL );
	ExecuteAction( A_FIN_PEDIR );
                             
    return ( rta );
}
/*****************************************************************************/
int _elegir_tarjeta( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0, cod;
    if( tarjetas_sic == NULL ) {
        return 0;
    }
    switch( comando ) {
        case _BUSCAR:
            MOSTRAR_CADENA( 8, 20, cadena );
            cod = atoi( cadena );
            //GET_GREATER_E( ( char* )&cod );
			SET_WHERE("COD_TARJETA >= '%i'",cod);
			SET_ORDER2( "COD_TARJETA" );
			RUN_QUERY(NO);
            break;
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[20];
                _itoa( tarjetas_sic->cod_tarjeta, xx, 10 );
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, tarjetas_sic->nombre );
            }
            #endif
            MOSTRAR_ENTERO( 8, 10 + dato, "99", tarjetas_sic->cod_tarjeta );
            MOSTRAR_CADENA( 11, 10 + dato, tarjetas_sic->nombre );
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
            break;
        case _VALIDAR:
            if( dato >= '0' && dato <= '9' ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
            rta = 1;
            break;
		case _TECLA:
			if( dato == -73 || dato == -81 /*|| ( dato >= '0' && dato <= '9' ) */)
				b->tecla = 0;
			break;
		case _LIMIT:
            if( dato > 0 ) {
                SET_LIMIT( dato, SI );
            } else {
                SET_LIMIT( NO, NO );
            }
            break;
    }
    return ( rta );
}
/***************************************************************************/
int MENU_SELECCIONA_HABILITAR( int cod_marca )
/***************************************************************************/
{
    struct _menu *items = NULL;
    char *fechas_presenta = NULL;
    int *indice = NULL, eleccion = -1;
    int cant_fechas = 0, h, cant_reg;
    unsigned rta = 0, fec_dif[30];
	int  area_ant, tipo_ant;
 
    #if defined(INVEL_W) || defined(INVEL_L)
    struct _cabmenu *cab;
    int columnas;
    #endif
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    //if( USE_DB( AREA_AUX2, _HAB_DIFERIMIENTO_TAR, ( char* )&habdifta, sizeof( habdifta ), NULL, 0,
    //            0 ) == 0 ) {
	if( OPEN_TABLE( T_HABIL_DIFER_TARJ, TT_ORIG, ( char* )&habdifta, sizeof( struct _habil_difer_tarj ) ) == 0 ) {
        //     SET_ORDER(2);
		RUN_QUERY(NO);
        cant_reg = RECCOUNT2( NULL, NULL);
        //items  = (struct _menu *) MALLOC( sizeof (struct _menu) * sizeof(habdifta));
        items = ( struct _menu * )MALLOC( sizeof( struct _menu ) * cant_reg );
        //fechas_presenta = (char *) MALLOC( sizeof(char[50]) * sizeof(habdifta));
        fechas_presenta = ( char* )MALLOC( sizeof( char[50] ) * cant_reg );
        //indice = (int  *) MALLOC( sizeof(int) * sizeof(habdifta));
        indice = ( int* )MALLOC( sizeof( int ) * cant_reg );
        //for (h = 0; h < sizeof(habdifta); h++) {
        if( items == NULL || fechas_presenta == NULL || indice == NULL ) {
            return( 0 );
        }
        for( h = 0;h < cant_reg;h++ ) {
            //SKIP(1);
            if( !( dbEOF() || dbBOF() ) ) {
                if( ( DAY( _GET_FECHA_DOS() ) ) >= habdifta.dia_difer_inicio
                 && DAY( _GET_FECHA_DOS() ) <= habdifta.dia_difer_fin
                 && habdifta.cod_tarjeta == cod_marca ) {
                    sprintf( ( char* )&fechas_presenta[cant_fechas * 50], " %d) %u/%u/%u",
                             cant_fechas, habdifta.dia_difer_presenta,
                             MON( _GET_FECHA_DOS() ) + habdifta.cant_difer_meses,
                             YEAR( _GET_FECHA_DOS() ) - 1900 );
                    items[cant_fechas].nom = ( char* )&fechas_presenta[cant_fechas * 50];
                    // Para obtener el Ascii de la Tecla
                    items[cant_fechas].tecla = cant_fechas + 48;
                    items[cant_fechas].rta = cant_fechas;
                    fec_dif[cant_fechas] = FECHA( ( char )habdifta.dia_difer_presenta,
                                                  ( char )( MON( _GET_FECHA_DOS() )
                                                          + habdifta.cant_difer_meses ),
                                                  YEAR( _GET_FECHA_DOS() ) - 1900 );
                    cant_fechas++;
                }
            }
            else {
                //h = sizeof(habdifta);
                h = cant_reg;
            }
            SKIP2( 1 );
        }
        if( cant_fechas ) {
            if( cant_fechas > 9 ) {
                cant_fechas = 9;
            }
            #if defined(INVEL_W) || defined(INVEL_L)
            columnas = 2;
            cab = ( struct _cabmenu * )malloc( sizeof( struct _cabmenu ) * columnas );
            if( cab != NULL ) {
                cab[0].nombre = "COD";
                cab[0].align = ALIGN_LEFT;
                cab[0].width = 8;
                cab[1].nombre = "FECHA";
                cab[1].align = ALIGN_LEFT;
                cab[1].width = 20;
                //eleccion = MENU( ST( S_FECHAS_A_HABILITAR_DIFERIMIENTO ), items, cant_fechas, cab, columnas );
                free( cab );
            }
            #else
            eleccion = MENU( 0, 9, ST( S_FECHAS_A_HABILITAR_DIFERIMIENTO ), items, cant_fechas, 120,
                             52 );
            #endif
            if( eleccion != -1 ) {
                rta = fec_dif[eleccion];
            }
            else {
                MENSAJE_STRING( S_NO_ELIJIO_DIFERIMIENTO );
                rta = 0;
            }
        }
        free( items );
        free( fechas_presenta );
        free( indice );
    }
    else {
        MENSAJE_STRING( S_ERROR_AL_ABRIR_LA_TABLA );
    }
   // CLOSE_DATABASE( AREA_AUX2 );
	CLOSE_TABLE( T_HABIL_DIFER_TARJ, TT_ORIG );
	SELECT_TABLE( area_ant, tipo_ant );
    return rta;
}
/***************************************************************************/
void GRABA_MODIFICACION_TARJETAS( int posicion, int id )
/***************************************************************************/
{
    int    handle_tarjeta;
    char  *buffer_tarjeta = 0;
    
	#ifdef INVEL_L
    handle_tarjeta = open( ( id == TARJETA_TRADICIONAL ) ? "tarjetas.$$$" : 
		( id == TARJETA_COBRO ) ? "tarjetas.cob" : "tarjetas.mon", O_RDWR );
    #else
    handle_tarjeta = _open( ( id == TARJETA_TRADICIONAL ) ? "tarjetas.$$$" : 
		( id == TARJETA_COBRO ) ? "tarjetas.cob" : "tarjetas.mon", O_RDWR );
    #endif
    if( handle_tarjeta != -1 )                    //OK
    {
        if( !buffer_tarjeta ) {
            buffer_tarjeta = ( char* )MALLOC( SIZEOF_STRUCT( _var_tarjetas ) );
        }
        if( buffer_tarjeta ) {
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
	if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
		char auxiliar[100];
		memset(auxiliar,0,sizeof(auxiliar));
		strncpy( auxiliar,GET_NUMERO_TARJETA_PRIVADO(  ), sizeof(VAR_TARJ_PRIV.numero_tarjeta_privado) );
		SET_MEMORY( __var_tarjetas_numero,auxiliar );
		/**/
		SET_MEMORY_INT( __var_tarjetas_fecha_vto,GET_FECHA_VTO_PRIVADA( ) );
		/**/
		memset(auxiliar,0,sizeof(auxiliar));
		strncpy( auxiliar,GET_COD_SERVICIO_PRIVADO(), sizeof(VAR_TARJ_PRIV.cod_servicio_privado) );
		SET_MEMORY( __var_tarjetas_cod_servicio,auxiliar );
		/**/
		memset(auxiliar,0,sizeof(auxiliar));
		strncpy( auxiliar,GET_PIN_PRIVADO(), sizeof(VAR_TARJ_PRIV.pin_privado) );
		SET_MEMORY( __var_tarjetas_pin,auxiliar );
		/**/
		memset(auxiliar,0,sizeof(auxiliar));
		strncpy( auxiliar,GET_TRACK_1_PRIVADO(), sizeof(VAR_TARJ_PRIV.track_1_privado) );
		SET_MEMORY( __var_tarjetas_track1,auxiliar );
		/**/
		memset(auxiliar,0,sizeof(auxiliar));
		strncpy( auxiliar,GET_TRACK_2_PRIVADO(), sizeof(VAR_TARJ_PRIV.track_2_privado) );
		SET_MEMORY( __var_tarjetas_track2,auxiliar );
		memset(auxiliar,0,sizeof(auxiliar));
   }
#endif
            COPY_STRUCT_TO_BUFFER( buffer_tarjeta, _var_tarjetas );
            if( posicion >= 0 ) {
                _lseek( handle_tarjeta, posicion * ( SIZEOF_STRUCT( _var_tarjetas ) ), SEEK_SET );
            }
            else {
                _lseek( handle_tarjeta, 0L, SEEK_END );
            }
            #ifdef INVEL_L
            write( handle_tarjeta, buffer_tarjeta, SIZEOF_STRUCT( _var_tarjetas ) );
            #else
            _write( handle_tarjeta, buffer_tarjeta, SIZEOF_STRUCT( _var_tarjetas ) );
            #endif
            FREEMEM( buffer_tarjeta );
            buffer_tarjeta = 0;
        }  else {
            MENSAJE_STRING( S_NO_SE_PUEDEN_PROCESAR_LAS_TARJETAS );
        }
        #ifdef INVEL_L
        close( handle_tarjeta );
        #else
        _close( handle_tarjeta );
        #endif
	}
    else {
        MENSAJE_STRING( S_NO_SE_PUEDEN_PROCESAR_MAS_TARJETAS );
    }
}
/***************************************************************************/
int LEE_TARJETAS( int posicion, int id )
/***************************************************************************/
{
    int handle, ok = 0;
    char *buffer_tarjeta = 0;
    buffer_tarjeta = ( char* )MALLOC( SIZEOF_STRUCT( _var_tarjetas ) );
    if( buffer_tarjeta ) {
        NRO_ARCHIVO = 0;
        #ifdef INVEL_L
		handle = open( ( id == TARJETA_TRADICIONAL ) ? "tarjetas.$$$" : 
			( id == TARJETA_COBRO ) ? "tarjetas.cob" : "tarjetas.mon" , O_RDONLY );
        #else
        handle = _open( ( id == TARJETA_TRADICIONAL ) ? "tarjetas.$$$" : 
			( id == TARJETA_COBRO ) ? "tarjetas.cob" : "tarjetas.mon" , O_RDONLY );
		#endif
        if( handle == -1 ) {
            MENSAJE_STRING( S_NO_SE_PUEDEN_PROCESAR_LAS_TARJETAS );
        } else {
			//_lseek( handle_tarjeta, posicion * ( SIZEOF_STRUCT( _var_tarjetas ) ), SEEK_SET );
            LSEEK( handle, posicion * ( SIZEOF_STRUCT( _var_tarjetas ) ), 0, __LINE__, __FILE__ );
            _READ( handle, buffer_tarjeta, ( SIZEOF_STRUCT( _var_tarjetas ) ), __LINE__, __FILE__ );
            COPY_BUFFER_TO_STRUCT( buffer_tarjeta, _var_tarjetas );
            ok = 1;
            _CLOSE( handle, __LINE__, __FILE__ );
        }
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
	if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
		char auxiliar[100];
		memset(auxiliar,0,sizeof(auxiliar));

        strncpy( auxiliar , VAR_TARJETAS_NUMERO,sizeof(VAR_TARJ_PRIV.numero_tarjeta_privado ));
		SET_NUMERO_TARJETA_PRIVADO( auxiliar  );
		/**/
		SET_FECHA_VTO_PRIVADA( VAR_TARJETAS_FECHA_VTO );
		/**/
		memset(auxiliar,0,sizeof(auxiliar));
		strncpy( auxiliar,VAR_TARJETAS_COD_SERVICIO,sizeof(VAR_TARJ_PRIV.cod_servicio_privado));
		SET_COD_SERVICIO_PRIVADO(auxiliar);
		/**/
		memset(auxiliar,0,sizeof(auxiliar));
		strncpy( auxiliar,VAR_TARJETAS_PIN,sizeof(VAR_TARJ_PRIV.pin_privado));
		SET_PIN_PRIVADO(auxiliar);
		/**/
		memset(auxiliar,0,sizeof(auxiliar));
		strncpy( auxiliar,VAR_TARJETAS_TRACK1,sizeof(VAR_TARJ_PRIV.track_1_privado));
		SET_TRACK_1_PRIVADO(auxiliar);
		/**/
		memset(auxiliar,0,sizeof(auxiliar));
		strncpy( auxiliar,VAR_TARJETAS_TRACK2,sizeof(VAR_TARJ_PRIV.track_2_privado));
		SET_TRACK_2_PRIVADO(auxiliar);
		memset(auxiliar,0,sizeof(auxiliar));
   }
#endif
        FREEMEM( buffer_tarjeta );
    } else {
        MENSAJE_STRING( S_NO_SE_PUEDEN_PROCESAR_LAS_TARJETAS );
    }
    return ( ok );
}
/*****************************************************************************/
int PEDIR_HORA_ORIGINAL()
/*****************************************************************************/
{
    int rta = 0;
    long nro;
    nro = PEDIR_ENTERO_XY( ST( S_HORA_ORIGINAL ), 40 - ancho_panel / 2, 22, 6 );
    if( nro ) {
        rta = 1;
        SET_MEMORY_LONG( __var_tarjetas_hora_original, nro );
    }
    return rta;
}
/*****************************************************************************/
int PEDIR_AUT_ORIGINAL()
/*****************************************************************************/
{
    char cadena[10];
    int rta = 0;
    rta = PEDIR_CADENA_XY( ST( S_AUT__ORIGINAL ), 40 - ancho_panel / 2, 22, 8, cadena );
    if( rta ) {
        SET_MEMORY( __var_tarjetas_aut_original, cadena );
    }
    return rta;
}
/*****************************************************************************/
int CANTIDAD_TARJETAS_DEBITOS_INGRESADAS()
/*****************************************************************************/
{
    int rta = 0, i;
    for( i = 0 ;i < RAM_P_PAGO ;i++ ) {
        if( medios[( int )_PAGO_MODO( i )].tarjeta ) {
            if( TARJ[( int )_PAGO_TARJETA( i )].celda_pinpad ) {
                rta++;
            }
        }
    }
    return rta;
}
/*****************************************************************************/
int PEDIR_TASA_APLICADA()
/*****************************************************************************/
{
    int rta = 0;
    double nro;
    nro = PEDIR_IMPORTE_XY( ST( S_TASA_APLICADA ), 40 - ancho_panel / 2, 22, 2, 2 );
    if( nro > 0.0 ) {
        rta = 1;
        SET_MEMORY_FLOAT( __var_tarjetas_tasa_aplicada, nro );
    }
    return rta;
}
/*****************************************************************************/
int PEDIR_IMPORTE_CUOTA()
/*****************************************************************************/
{
    int rta = 0;
    double nro;
    nro = PEDIR_IMPORTE_XY( ST( S_MONTO_CUOTA ), 40 - ancho_panel / 2, 22, ENTEROS, DECIMALES );
    if( nro > 0.0 ) {
        rta = 1;
        SET_MEMORY_FLOAT( __var_tarjetas_importe_cuota, nro );
    }
    return rta;
}


#ifndef ENMASCARAMIENTO_CONFIGURABLE
/*****************************************************************************/
void ENMASCARA_NUMERO_TARJETA( int marca,char *nro_cadena,char *temp)//-->enmascara el numero completo
/*****************************************************************************/
{/*
 EL enmascaramiento
 N= No se enmascara
 S = enmascara los 12 primeros ( Modo POsnet).
 1=  Modo VISA  ( No se enmascararan los 6 primeros dígitos de la tarjeta ni los 4 últimos.-
				(Ejemplo:  4111-11XX-XXXX-1234 )
				(Ejemplo:  4111-11**-****-1234 )
 
 */
	if( TARJ[marca].enmasc_nro_tarjeta == 's'
			|| TARJ[marca].enmasc_nro_tarjeta == 'S' || TARJ[marca].enmasc_nro_tarjeta == '1' )
	{
		char   cadena[20], auxiliar [20]; //  cadena[17], auxiliar[17];
        int    digitos = 0, i = 0;
/*		memcpy( auxiliar, nro_cadena, sizeof( auxiliar ) );
		memset( cadena, '*', sizeof( cadena ) );
		memcpy( &cadena[0], &auxiliar[0], sizeof( auxiliar ) - 18 );//ultimos 4 digitos enmascarados, de esta liena depende cuanto se enmascara
		cadena[19] = 0;
		memcpy( temp,cadena,sizeof(cadena));*/
		memset( cadena, 0, sizeof( cadena ) );
 		memset( auxiliar, 0, sizeof( auxiliar ) );

		memcpy( auxiliar, nro_cadena, sizeof( auxiliar ) );
		digitos = strlen( auxiliar );

		if( digitos > 3 ) {
			//_snprintf( cadena, sizeof( cadena ), "%s", MASCARA( '*', digitos - 4, "" ) ) ;
			//POsnet siempre hay que enmascarar 12 no dejar 4 sin enmascarar
			if( TARJ[marca].enmasc_nro_tarjeta == '1') {
				_snprintf( cadena, sizeof( cadena ), "%s", MASCARA( '*', digitos, "" ) ) ;
				for( i = 0; i < 6 ; i++ ) {
				cadena[i] = auxiliar[i];
			}
				for( i = digitos -4 ; i < digitos ; i++ ) {
					cadena[i] = auxiliar[i];
				}
			
			} else {
				_snprintf( cadena, sizeof( cadena ), "%s", MASCARA( '*', 12, "" ) ) ;
				for( i = 12; i < digitos ; i++ ) {
					cadena[i] = auxiliar[i];
				}
			}
			//Para evitar pisado de memoria
			for( i = digitos; i <= 19 && cadena[i - 1] != '\0'; i++ ) {
				cadena[i] = '\0';
			}
		}
		memcpy( temp, cadena, sizeof(cadena));	
	} else {
		sprintf( temp, nro_cadena );
	}
}

#else
/**************************************************************************************************/
void ENMASCARA_NUMERO_TARJETA( int marca,char *nro_cadena,char *temp)//-->enmascara el numero completo
/**************************************************************************************************/
{//• Número de Tarjeta o 4111-11XX-XXXX-1234//411111XXXXXX1234
   static char cadena[30];
   int inicio = 0,fin = 0, tipo = 0, posicion=0;
   char auxiliar [30] ;
   char picture ='*';/*+#$& */
   int long_nro_tarjeta = strlen( nro_cadena );//,enmascara_desde=0,enmascara_hasta=0;

/*CAMPOS QUE DEBEN SER AGREGADOS A LA TARJETA
*	--	enmascara_desde
*	--	enmascara_hasta
*	reutilizar tipo enmasc_nro_tarjeta)
*/
	inicio = TARJ[ marca ].enmascara_desde;
	fin = TARJ[marca ].enmascara_hasta;
	tipo = TARJ[marca ].enmasc_nro_tarjeta;
	
	switch( tipo ){
			case'#':
				picture ='#';
				tipo =1;
				break;
			case'$':
				picture ='$';
				tipo =1;
				break;
			case'&':
				picture ='&';
				tipo =1;
				break;
			case'1':
				tipo =1;
				break;
			case'2':
				tipo =2;
				break;
			case'3':
				tipo =3;
				break;
			case'4':
				tipo =4;
				break;
		}
		
	
	
	if( tipo!= 4 && fin < inicio ){
		int cambio = fin;
		fin = inicio;
		inicio = cambio;
		tipo = 3 ;
	}
	

   memset(cadena,0,sizeof(cadena));//cereo todo
   memcpy( cadena, nro_cadena, long_nro_tarjeta );// copio el numero_tarjeta

   if(( TARJ[marca].enmasc_nro_tarjeta == 's' || TARJ[marca].enmasc_nro_tarjeta == 'S' )//forma anterior que puede seguir
			||//forma nueva 
			( tipo >= 1 && long_nro_tarjeta >= fin && 
				( inicio >= 0 && fin >= inicio && 
					( TARJ[ marca ].largo2 >= fin ||TARJ[ marca ].largo1 >= fin ) ) )||
					(tipo == 4)/*usa inicio o fin como banderas de bits */ ){

      memset( auxiliar, picture, long_nro_tarjeta + 1 );//Prepara la mascara
      auxiliar[long_nro_tarjeta + 1 ] = 0;//corto la cadena al largo correspondiente
	

      switch (tipo){
         case 1:
		case '1':
         {//paso los * a las posiciones definidas, el resto son valores de la tarjeta
            memcpy( &cadena[inicio - 1], &auxiliar[inicio - 1], abs( fin - inicio )+ 1 );
            //12347********34560000

         }
         break;
         case 2:
		 case '2':
         {//paso los valores a las posiciones definidas, el resto son *
            memcpy( &auxiliar[inicio - 1 ], &cadena[inicio - 1 ],abs( fin - inicio ) + 1 );//a* en los cinco lugares pediso por visa
            memcpy( cadena, auxiliar, sizeof( cadena ) );//paso la cadena el numero_tarjeta

         }
         break;
         case 3:
		 case '3':
         {//todos *
            memset( cadena, 0, sizeof( cadena) );
            memcpy( cadena, auxiliar, long_nro_tarjeta );//paso la cadena el numero_tarjeta
         }
         break;
		 /* podemos dejar el tradicional si viene cargado con s o S */
		 case 's':
		 case 'S':{
			 int digitos = 0, i=0;
			memset( cadena, 0, sizeof( cadena ) );
			 memset( auxiliar, 0, sizeof( auxiliar ) );

			memcpy( auxiliar, nro_cadena, sizeof( auxiliar ) );
			digitos = strlen( auxiliar );

			if( digitos > 3 ) {
				_snprintf( cadena, sizeof( cadena ), "%s", MASCARA( '*', digitos - 4, "" ) ) ;
				for( i = digitos - 4; i < digitos ; i++ ) {
					cadena[i] = auxiliar[i];
			}
			//Para evitar pisado de memoria
			for( i = digitos; i <= 19 && cadena[i - 1] != '\0'; i++ ) {
				cadena[i] = '\0';
			}
			}
				  }
		 break;
		 case 4:
		 case '4':
			 
			 /*Enmascaramiento reutilizando los campos inicio y fin como bandera para utilizar mascara...
			 campo tipo = 4
			 campo inicio  desde  0 hasta el valor 32767, son 15 lugares , 111111111111111
			 campo fin   desde 0 hasta el valor 32767, son 15 lugares , 111111111111111
			 Ejemplo, si se desea enmascarar 5323601111111112, quedando ***************2,
			 se debe indicar en inicio = 32767, y fin =0, por lo que se aplicaria la mascara 111111111111111*/

			 if(inicio > 0 && inicio < 32768 )//32767 --> 15 lugares  111111111111111->0->14
				REEMPLAZO_BANDERA_BINARIA(inicio, cadena,14, picture);
			 
			 if(fin > 0 && fin < 32768 )//32767 --> 15 lugares  111111111111111->15->29
				REEMPLAZO_BANDERA_BINARIA(fin, &cadena[15],14, picture);
			 
			 break;
			
		 

         default:
            //nada, queda como entro
         break;
      }
   }
   memcpy( temp, cadena, long_nro_tarjeta);	
     
}
/*****************************************************************************/
void REEMPLAZO_BANDERA_BINARIA(int numero_entero,char* cadena,int posicion, char picture)
/*****************************************************************************/
{
    if (numero_entero!=0)
    {
        REEMPLAZO_BANDERA_BINARIA(numero_entero/2, cadena, posicion -1 , picture);
		if( numero_entero%2 ){
			cadena[posicion] = picture;
		}
    }
}
#endif
/**
/* Permite seleccionar un submedio de un medio tarjeta.
/* Se asume que el medio es de tipo tarjeta
/* @param medio numero de medio de la tarjeta
/*****************************************************************************/
void PEDIR_MARCAS_TARJETA( int medio )
/*****************************************************************************/
{
    struct _tarjetas tarjeta_sic;
	//struct _medios_tar tar_med;
    int marcas[20];
    int i = 0, sel = 0, open = 0;

    //if( USED( AREA_AUX ) ) {
    //    CLOSE_DATABASE( AREA_AUX );
    //}
    open = OPEN_TABLE(T_TARJETAS, TT_ORIG,( char* )&tarjeta_sic,sizeof( tarjeta_sic )); 
    open += OPEN_TABLE( T_TAR_MED, TT_ORIG, ( char* )&tar_med, sizeof( tar_med ) );
    if (open > 0) {
        GRABAR_LOG_SISTEMA("PEDIR_MARCAS_TARJETA: ERROR ABRIENDO TABLAS",LOG_VENTAS,1);
        MENSAJE("ERROR ABRIENDO TABLAS");
        CLOSE_TABLE( T_TAR_MED, TT_ORIG );
        CLOSE_TABLE( T_TARJETAS, TT_ORIG );
        return;
    }
	SELECT_TABLE( T_TAR_MED, TT_ORIG );
	SET_WHERE("");
	RUN_QUERY(NO);
    //GET_FIRST();//Andres ver equivalente en mySql!
	GO2( TOP );
    while( !dbEOF() ) {
        if ( tar_med.cod_medio == medio ) {
			SELECT_TABLE( T_TARJETAS, TT_ORIG );
			SET_WHERE("COD_TARJETA = '%i'",tar_med.cod_medio);
			RUN_QUERY( NO );
            if ( FOUND2( ) ){
                ExecuteSAction( A_AGREGAR_FILA_GRILLA, tarjeta_sic.nombre );
                marcas[i++] = tar_med.cod_medio;
            }
        }
        SELECT_TABLE( T_TAR_MED, TT_ORIG );
        //GET_NEXT();//Andres Ver equivalente en MySql
		SKIP2( 1 );
    }
    sel = ELEGIR_OPCION_GRILLA( i+1, "Tarjetas" );
    cod_tar_selected = marcas[sel];
    //Si el submedio tiene configurada la opcion "codigo aux" (cambia_marca)
    //se toma este valor en cambio de cod_tarjeta ya que sera el utilizado 
    //para validar el rango del numero de la tarjeta.
    if( tarjeta_sic.cambia_marca ){
        cod_tar_selected = tarjeta_sic.cambia_marca;
    }
    CLOSE_TABLE( T_TAR_MED, TT_ORIG );
    CLOSE_TABLE( T_TARJETAS, TT_ORIG );
}
    
/*****************************************************************************/
char HABILITAR_VOUCHER_MEDIO( int submedio )
/*****************************************************************************/
{
    char       rta;
    struct _tarjetas tarjeta_sic;
   	//---------------------------- Abre el archivo ---------------------------//
   	//if( USE_DB( AREA_AUX, _tarjetas, ( char* )&tarjeta_sic, sizeof( tarjeta_sic ), NULL, 0, 0 ) == 0 ) {
	if( OPEN_TABLE( T_TARJETAS, TT_ORIG, ( char* )&tarjeta_sic, sizeof( tarjeta_sic ) ) == 0 ) {
    	//GET_EQUAL( ( char* )&submedio );
		SET_WHERE("COD_TARJETA='%d'", submedio);
		RUN_QUERY(NO);
		if( FOUND2(  ) ) {
			rta = tarjeta_sic.hab_voucher_dev;
		} else {
			rta = 'S';
		}
		//CLOSE_DATABASE( AREA_AUX );
		CLOSE_TABLE( T_TARJETAS, TT_ORIG );
	}
	return ( rta );
}
    
/********************************************************************************/
int VALIDAR_MODO_INGRESO_TARJETA( int marca, int operacion )
/********************************************************************************/
{
	int rta = 0;
	
	if( VAR_TARJETAS_LECTURA_BANDA ) {
		switch( VAR_TARJETAS_LECTURA_BANDA ) { 
			case BANDA_MAGENETICA: // 1
				if( TARJ[marca].permite_id_banda & operacion ) {
					rta = 1;
				}
				break;
			case CODIGO_BARRA: // 2
				if( TARJ[marca].permite_id_barra & operacion ) {
					rta = 1;
				}
				break;
		}
	} else { //ingreso manual
		rta = 1;
	}
	return rta;
}



/********************************************************************************/
void SET_NUMERO_TARJETA_PRIVADO(char* valor)
/********************************************************************************/

{
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
      if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
         memset(VAR_TARJ_PRIV.numero_tarjeta_privado, 0 ,sizeof(VAR_TARJ_PRIV.numero_tarjeta_privado) );
         strcpy(VAR_TARJ_PRIV.numero_tarjeta_privado, valor);
         CEREAR_MEMORY( __var_tarjetas_numero );
      }else
#endif	  
	  {
         SET_MEMORY( __var_tarjetas_numero,valor );
      }
}
/********************************************************************************/
char *GET_NUMERO_TARJETA_PRIVADO( void )
/********************************************************************************/

{
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
   if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
      return VAR_TARJ_PRIV.numero_tarjeta_privado ;
   }else
#endif
   {
      return ( VAR_TARJETAS_NUMERO );
   }
}
/********************************************************************************/
void SET_FECHA_VTO_PRIVADA( int valor)
/********************************************************************************/

{
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
      if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
         VAR_TARJ_PRIV.fecha_vto_privada = valor;
         SET_MEMORY_INT( __var_tarjetas_fecha_vto,0 );
      }else
#endif	  
	  {
		  SET_MEMORY_INT( __var_tarjetas_fecha_vto,valor );
      }
}
/********************************************************************************/
int GET_FECHA_VTO_PRIVADA( void )
/********************************************************************************/
{
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
   if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
      return VAR_TARJ_PRIV.fecha_vto_privada ;
   }else
#endif
   {
      return ( VAR_TARJETAS_FECHA_VTO );
   }
}
/********************************************************************************/
void SET_COD_SERVICIO_PRIVADO(char* valor)
/********************************************************************************/
{
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
      if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
         memset(VAR_TARJ_PRIV.cod_servicio_privado, 0 ,sizeof(VAR_TARJ_PRIV.cod_servicio_privado) );
         strcpy(VAR_TARJ_PRIV.cod_servicio_privado, valor);
         CEREAR_MEMORY( __var_tarjetas_cod_servicio );
      }else
#endif	  
	  {
         SET_MEMORY( __var_tarjetas_cod_servicio,valor );
      }

}
/********************************************************************************/
char *GET_COD_SERVICIO_PRIVADO( void )
/********************************************************************************/
{
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
   if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
      return VAR_TARJ_PRIV.cod_servicio_privado ;
   }else
#endif   
	{
      return ( VAR_TARJETAS_COD_SERVICIO );
   }
}
/********************************************************************************/
int  GET_COD_SERVICIO_PRIVADO_POSICION( int posicion )
/********************************************************************************/
{
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
   if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
	   
      return VAR_TARJ_PRIV.cod_servicio_privado[posicion];
   }else
#endif   
	{
      return ( _VAR_TARJETAS_COD_SERVICIO(posicion) );
   }
}

/********************************************************************************/
void SET_PIN_PRIVADO(char* valor)
/********************************************************************************/
{
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
      if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
         memset(VAR_TARJ_PRIV.pin_privado, 0 ,sizeof(VAR_TARJ_PRIV.pin_privado) );
         strcpy(VAR_TARJ_PRIV.pin_privado, valor);
         CEREAR_MEMORY( __var_tarjetas_pin );
      }else
#endif	  
	  {
         SET_MEMORY( __var_tarjetas_pin,valor );
      }

}
/********************************************************************************/
char * GET_PIN_PRIVADO( void )
/********************************************************************************/
{
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
   if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
      return VAR_TARJ_PRIV.pin_privado ;
   }else
#endif   
   {
      return ( VAR_TARJETAS_PIN );
   }
}

/********************************************************************************/
void SET_TRACK_1_PRIVADO(char* valor)
/********************************************************************************/
{
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
      if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
         memset(VAR_TARJ_PRIV.track_1_privado, 0 ,sizeof(VAR_TARJ_PRIV.track_1_privado) );
         strcpy(VAR_TARJ_PRIV.track_1_privado, valor);
         CEREAR_MEMORY( __var_tarjetas_track1 );
      }else
#endif	  
	  {
         SET_MEMORY( __var_tarjetas_track1,valor );
      }
	   glog(valor,1,5);

}
/********************************************************************************/
char * GET_TRACK_1_PRIVADO( void )
/********************************************************************************/
{
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
   if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
      return VAR_TARJ_PRIV.track_1_privado ;
   }else
#endif   
   {
      return ( VAR_TARJETAS_TRACK1 );
   }
}
/********************************************************************************/
void SET_TRACK_2_PRIVADO(char* valor)
/********************************************************************************/
{

	/*char track2[50];
	int size = 0;

	memset( track2, 0, sizeof( track2 ) );
	memcpy( track2, valor, sizeof( track2 ) );
	size = strlen(track2);
	memset( &track2[size + 1], 0, sizeof( track2 ) - ( size + 1 ) );*/

#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
      if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){

		  memset(VAR_TARJ_PRIV.track_2_privado, 0 ,sizeof(VAR_TARJ_PRIV.track_2_privado) );
         strcpy(VAR_TARJ_PRIV.track_2_privado, valor);
         CEREAR_MEMORY( __var_tarjetas_track2 );
      }else
#endif	  
	  {
         SET_MEMORY( __var_tarjetas_track2,valor );
      }
	   glog(valor,1,5);

}
/********************************************************************************/
char * GET_TRACK_2_PRIVADO( void )
/********************************************************************************/
{
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
   if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
      return VAR_TARJ_PRIV.track_2_privado ;
   }else
#endif
   {
      return ( VAR_TARJETAS_TRACK2 );
   }
}
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
/********************************************************************************/
void datos_privados_tarjeta_limpiar ( void )
/********************************************************************************/
{
   if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
      memset( &VAR_TARJ_PRIV , 0, sizeof( struct _datos_privados_tarjeta));
   }
}
#endif

/****************************************************************************************/
int CARGAR_CONFIGURACION_TAR_MED( int medio ) 
/****************************************************************************************/
{
   struct{
         char cod_medio;
         char cod_tarjeta;
   }tm_index;
   
   /*if( !medio ) {
      medio = 6;
   }*/
	
   tm_index.cod_medio = (char)( medio + 1 );
   tm_index.cod_tarjeta = (char)0;
   //if(!USE_DB( AREA_TAR_MED, _TAR_MED_BTR, ( char* )&tar_med, sizeof( tar_med ), NULL, 0, 0 ) ) {
   if( OPEN_TABLE( T_TAR_MED, TT_ORIG, ( char* )&tar_med, sizeof( struct _medios_tar) ) == 0 ) {
       //SET_ORDER( 1 );
	   SET_ORDER2("COD_MEDIO, COD_TARJETA");
       //GET_GREATER_E( (char*)&tm_index );
	   SET_WHERE( "COD_MEDIO = '%i' AND COD_TARJETA >= '%i'", tm_index.cod_medio,
			tm_index.cod_tarjeta );
	   RUN_QUERY(NO);
       if( !FOUND2( ) ){
           return 0;
       }
       while( /*!BtrvEOF()*/ !dbEOF() && tar_med.cod_medio == tm_index.cod_medio ){
		   TARJ[tar_med.cod_tarjeta-1].pide_numero = tar_med.pide_numero;
           TARJ[tar_med.cod_tarjeta-1].pide_supervisor = tar_med.pide_supervisor;
           TARJ[tar_med.cod_tarjeta-1].permite_ingreso_manual = tar_med.permite_ingreso_manual;
           TARJ[tar_med.cod_tarjeta-1].on_line = tar_med.on_line;
           TARJ[tar_med.cod_tarjeta-1].pedir_fecha_vto = tar_med.pedir_fecha_vto;
           TARJ[tar_med.cod_tarjeta-1].manual_especial = tar_med.manual_especial;
           TARJ[tar_med.cod_tarjeta-1].captura_on_line = tar_med.captura_on_line;
           TARJ[tar_med.cod_tarjeta-1].permite_consulta_saldo = tar_med.permite_consulta_saldo;
           //SKIP( 1 );
		   SKIP2( 1 );
       }
       //CLOSE_DB( _TAR_MED_BTR );
	   CLOSE_TABLE( T_TAR_MED, TT_ORIG );
   }
   return 1;
}

void CEREAR_DATOS_PRIVADOS(void){
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
	memset(&VAR_TARJ_PRIV, 0 ,sizeof( struct _datos_privados_tarjeta ) );
#endif

}

/******************************************************/
void PASAR_DATOS_TARJETA_A_PRIVADOS(void) {
/******************************************************/
    #ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
    if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
        char auxiliar[100];
        memset(auxiliar,0,sizeof(auxiliar));

        strncpy( auxiliar , VAR_TARJETAS_NUMERO,sizeof(VAR_TARJ_PRIV.numero_tarjeta_privado ));
        SET_NUMERO_TARJETA_PRIVADO( auxiliar  );
        /**/
        SET_FECHA_VTO_PRIVADA( VAR_TARJETAS_FECHA_VTO );
        /**/
        memset(auxiliar,0,sizeof(auxiliar));
        strncpy( auxiliar,VAR_TARJETAS_COD_SERVICIO,sizeof(VAR_TARJ_PRIV.cod_servicio_privado));
        SET_COD_SERVICIO_PRIVADO(auxiliar);
        /**/
        memset(auxiliar,0,sizeof(auxiliar));
        strncpy( auxiliar,VAR_TARJETAS_PIN,sizeof(VAR_TARJ_PRIV.pin_privado));
        SET_PIN_PRIVADO(auxiliar);
        /**/
        memset(auxiliar,0,sizeof(auxiliar));
        strncpy( auxiliar,VAR_TARJETAS_TRACK1,sizeof(VAR_TARJ_PRIV.track_1_privado));
        SET_TRACK_1_PRIVADO(auxiliar);
        /**/
        memset(auxiliar,0,sizeof(auxiliar));
        strncpy( auxiliar,VAR_TARJETAS_TRACK2,sizeof(VAR_TARJ_PRIV.track_2_privado));
        SET_TRACK_2_PRIVADO(auxiliar);
        memset(auxiliar,0,sizeof(auxiliar));
    }
    #endif
}
/******************************************************/
void PASAR_PRIVADOS_A_DATOS_TARJETA(void) {
/******************************************************/
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
        char auxiliar[100];
        memset(auxiliar,0,sizeof(auxiliar));
        strncpy( auxiliar,GET_NUMERO_TARJETA_PRIVADO(  ), sizeof(VAR_TARJ_PRIV.numero_tarjeta_privado) );
        SET_MEMORY( __var_tarjetas_numero,auxiliar );
        /**/
        SET_MEMORY_INT( __var_tarjetas_fecha_vto,GET_FECHA_VTO_PRIVADA( ) );
        /**/
        memset(auxiliar,0,sizeof(auxiliar));
        strncpy( auxiliar,GET_COD_SERVICIO_PRIVADO(), sizeof(VAR_TARJ_PRIV.cod_servicio_privado) );
        SET_MEMORY( __var_tarjetas_cod_servicio,auxiliar );
        /**/
        memset(auxiliar,0,sizeof(auxiliar));
        strncpy( auxiliar,GET_PIN_PRIVADO(), sizeof(VAR_TARJ_PRIV.pin_privado) );
        SET_MEMORY( __var_tarjetas_pin,auxiliar );
        /**/
        memset(auxiliar,0,sizeof(auxiliar));
        strncpy( auxiliar,GET_TRACK_1_PRIVADO(), sizeof(VAR_TARJ_PRIV.track_1_privado) );
        SET_MEMORY( __var_tarjetas_track1,auxiliar );
        /**/
        memset(auxiliar,0,sizeof(auxiliar));
        strncpy( auxiliar,GET_TRACK_2_PRIVADO(), sizeof(VAR_TARJ_PRIV.track_2_privado) );
        SET_MEMORY( __var_tarjetas_track2,auxiliar );
        memset(auxiliar,0,sizeof(auxiliar));
   }
    #endif
}

/*****************************************************************************/
void ENMASCARA_FECHA_TARJETA( int posicion )
/*****************************************************************************/
{
   char fecha_vto[30];
   int marca_tarjeta = 0;

   memset( fecha_vto, 0, sizeof( fecha_vto ) );

	marca_tarjeta = VAR_TARJETAS_TARJETA;
   if( TARJ[marca_tarjeta].enmasc_fecha == 's'
      || TARJ[marca_tarjeta].enmasc_fecha == 'S' ) {
      strcpy( fecha_vto, "**/**" );
      MOSTRAR_CADENA( posicion, PRN, fecha_vto );
   } else {
	   int fecha_c =DECREMENTAR_MES( GET_FECHA_VTO_PRIVADA(  ) );
       MOSTRAR_FECHAMMAA(  posicion , PRN, fecha_c );
	   //MOSTRAR_FECHA( posicion , PRN, GET_FECHA_VTO_PRIVADA(  )  );
	   //MOSTRAR_CADENA( posicion, PRN, fecha_vto );
   }
}

/*****************************************************************************/
int ANULACION_TARJETA_TECLA( int cashback)
/*****************************************************************************/
{
	long nro_ticket = 0L;
	double importe = 0.00;
	int nodo = 0, cuota = 0;
	char modo_ingreso_ant;
	struct _rta_terminal rta_terminal;
	int rta = 1;
	char num_tarj[21];
	long fecha;
		

	UINT16 modo_pago;
	
	
	SET_MEMORY_CHAR( __var_tarjetas_status, 0 ); 
	CR1_MUESTRA_TARJETA();
	if( cashback == SI){
		MENSAJE("INGRESE NUEVAMENTE CUPON TARJETA DE LA COMPRA PARA EL RETIRO");
		nro_ticket = PEDIR_ENTERO_EN_RECUADRO( "CUPON COMPRA", NO, 6 );
	} else {
		MENSAJE("INGRESE NRO DE CUPON TARJETA  A ANULAR");
		nro_ticket = PEDIR_ENTERO_EN_RECUADRO( "CUPON A ANULAR", NO, 6 );
	}
	BORRAR_MENSAJE();
	rta = T_ANULAR_TRANSACCION( nro_ticket, &importe, &nodo, &modo_pago, &cuota, &fecha, cashback );
	strncpy( num_tarj, GET_NUMERO_TARJETA_PRIVADO()/*VAR_TARJETAS_NUMERO*/, 21 );
	num_tarj[20] = 0;
	if( cashback == SI && (!TARJ[VAR_TARJETAS_TARJETA ].tipo_tarjeta & _TIPO_CASH_ADVANCE)) {
		MENSAJE_TECLA( "TARJETA ORIGINAL NO ADMITE RETIROS ( Pres tecla )"  );
		rta = 0;
		return rta;
	}
	if( rta == 0 || rta < 0){
		MENSAJE("ERROR EN BUSQUEDA DE CUPON A ANULAR");
		if( rta == -1){
			MENSAJE( "CUPON NO REGISTRADO EN LOS PAGOS" ); //no existe en la ev_medios este pago
			BORRAR_MENSAJE();
		}
		if( rta == -2){
			MENSAJE( "LA COMPRA YA FUE ANULADA PREVIAMENTE" ); //no existe en la ev_medios este pago
			BORRAR_MENSAJE();
		}
		rta = 0;
		return rta;
	}

	modo_ingreso_ant = VAR_TARJETAS_LECTURA_BANDA;
	if( MOSTRAR_CUPON_ANULAR( nro_ticket, importe, cuota, fecha, num_tarj, cashback ) == 0){
	  	rta = 0;
		return rta;
	}
	if(config_tps.HabilitarCashPlusSolo == 0 && cashback == SI)
		SET_MEMORY_CHAR( __var_tarjetas_status, 1 );//agregado
	else
		CR1_VALIDA_TARJETA( NO, --modo_pago );
	if( VAR_TARJETAS_STATUS == 0 ) {
		MENSAJE("ERROR EN LECTURA DE TARJETA");
		rta = 0;
		return rta;
	}
	if( modo_ingreso_ant != VAR_TARJETAS_LECTURA_BANDA) {
		MENSAJE("MODO INGRESO INCORRECTO, DISTINTO DE LA COMPRA ORIGINAL");
		rta = 0;
		return rta;
	}
	if (!(ES_LA_MISMA_TARJETA(num_tarj,GET_NUMERO_TARJETA_PRIVADO()))) {
		MENSAJE_TECLA( "TARJETA INCORRECTA ( Presione una tecla )"  );
		rta = 0;
		return rta;
	}
	



	SELECT_TABLE( T_TRANSAC2, TT_ORIG );
	if( cashback == NO) {
		int tipo_operacion_anular;
		 if( VAR_TARJETAS_OPERACION == _ON_LINE_DEVOLUCION)
			 tipo_operacion_anular = _ON_LINE_ANULACION_DEVOLUCION;
		  if( VAR_TARJETAS_OPERACION == _ON_LINE_COMPRA)
			 tipo_operacion_anular = _ON_LINE_ANULACION;
		  if( VAR_TARJETAS_OPERACION == _OFF_LINE_COMPRA )
			 tipo_operacion_anular = _ON_LINE_ANULACION;
		  if( VAR_TARJETAS_OPERACION == _OFF_LINE_DEVOLUCION )
			 tipo_operacion_anular = _ON_LINE_ANULACION_DEVOLUCION;
		  if( VAR_TARJETAS_OPERACION == _ON_LINE_CASHPLUS)
			 tipo_operacion_anular = _ON_LINE_ANULACION_CASHPLUS;

		  

		if( !AUTORIZACION_ON_LINE( tipo_operacion_anular, importe, ( char* )&rta_terminal, sizeof( rta_terminal ) ) ) {
			rta_terminal.cod_rta = 1;
			MENSAJE("HUBO UN ERROR EN LA OPERACION DE ANULACION");
			rta = 0;
			return 0;
		} 
		if( rta_terminal.cod_rta == 0 ) {
			SET_MEMORY( __var_tarjetas_autorizacion, rta_terminal.autorizacion );
			SET_MEMORY_STR_CHAR( __var_tarjetas_autorizacion, 10, 0 );
				rta_terminal.autorizacion[10] = 0;
				SET_MEMORY( __var_tarjetas_autorizacion, rta_terminal.autorizacion );
				SET_MEMORY_CHAR( __var_tarjetas_on_line, SI );
				{
					char auxil[13];
					 memset(auxil,0, sizeof(auxil));
					_snprintf(auxil, sizeof(auxil) -1 ,rta_terminal.nro_referencia);
					//rta_terminal.nro_referencia;
				//SET_MEMORY( __var_tarjetas_nro_ref, rta_terminal.nro_referencia ); //guarda aca que no pegue junto el mensaje y este campo
					SET_MEMORY( __var_tarjetas_nro_ref, auxil ); //guarda aca que no pegue junto el mensaje y este campo
				//---- Esto debiera ser configurable
				}
				SET_MEMORY_INT( __var_tarjetas_lote, rta_terminal.lote );
				SET_MEMORY_CHAR( __var_tarjetas_aut_on_line, SI );
	//			if( strcmp( rta_terminal.mensaje, "NO" ) != 0)
				SET_MEMORY( __var_tarjetas_mensaje, rta_terminal.mensaje );
				SET_MEMORY_FLOAT( __var_tarjetas_saldo_cliente, rta_terminal.saldo );
				if( rta_terminal.cuota
				 && !( ( config.pais == CHILE || config.pais == BRASIL ) && RAM_NOTA_CR ) ) {
					SET_MEMORY_FLOAT( __var_tarjetas_importe_cuota, rta_terminal.cuota );
				}
				SET_MEMORY_INT( __var_tarjetas_fecha_contable, rta_terminal.fecha_contable );
				SET_MEMORY_INT( __var_tarjetas_fecha_host, rta_terminal.fecha_host );
				SET_MEMORY_LONG( __var_tarjetas_hora_host, rta_terminal.hora_host );
				SET_MEMORY( __var_tarjetas_nro_cuenta, rta_terminal.nro_cuenta );
				if( !RAM_NOTA_CR ) {
					SET_MEMORY_FLOAT( __var_tarjetas_tasa_aplicada, rta_terminal.tasa_aplicada );
				}
		} else {
			//algo fallo
			if( rta_terminal.cod_rta == 1)
				strncpy( rta_terminal.mensaje, ST( S_ERROR_DE_COMUNICACION ),
							 sizeof( rta_terminal.mensaje ) );
			
			MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal.mensaje );
			
			rta = 0;
			return 0;

		}

		//AUTORIZACION_ON_LINE( _ON_LINE_ANULACION, importe ,( char* )&rta_terminal, sizeof( rta_terminal ) ) ; 
		
		//ENVIAR_TRANSACCION_AL_AUT_ON( _ON_LINE_ANULACION, importe, NULL,  nodo  ); //aca debo procesar respuesta y el nodo no hardcodeado
		
		if (VAR_TARJETAS_CUOTAS )
			SET_MEMORY_CHAR( __var_tarjetas_status, 2 );
		//   guardo los datos de la tarjeta para asi recuperarla en la NC 
		GRABA_MODIFICACION_TARJETAS( 0, TARJETA_TRADICIONAL );
		MENSAJE("IMPRIMIENDO VOUCHER DE ANULACION TARJETA" );

		if (tipo_operacion_anular == _ON_LINE_ANULACION_DEVOLUCION)
				SET_MEMORY_CHAR( __ram_nota_cr, 1 );
		IMPRESION_BAUCHER_ANULACION_TECLA( importe, nro_ticket );
		SET_MEMORY_CHAR( __ram_nota_cr, 0 );
		BORRAR_MENSAJE();

		if( CEREAR_TARJETA_DESPUES_DE_VALIDA ) {
			 //CEREAR_STRUCT( _var_tarjetas );
			 CR1_MUESTRA_TARJETA();
			  if( VAR_TARJETAS_STATUS ) {
				  GUI_MUESTRA_TARJETA( T_DATOS_TARJETA_BORRAR );
			  }
		}
		CR1_MUESTRA_TARJETA();
//comentado para probar	CEREAR_STRUCT( _var_tarjetas );
	}
	return 1;
}

/*****************************************************************************/
int CUOTA_VIGENTE( )
/*****************************************************************************/
{
    int rta = 0;
    time_t time1;
    struct tm time_check;
    int year = 0, month = 0, day = 0;
	int  dia_actual = 0;

    FECHA__( _GET_FECHA_DOS(  ), &day, &month, &year );
    time_check.tm_year = year - 1900;
    time_check.tm_mon = month - 1;
    time_check.tm_mday = day;
    time_check.tm_hour = 0;
    time_check.tm_min = 0;
    time_check.tm_sec = 1;
    time_check.tm_isdst = -1;

    time1 = mktime( &time_check );
    if( time1 == -1 ) {
        time_check.tm_wday = 7;
    }

	if( time_check.tm_wday )
		dia_actual = ( char ) pow( 2.0, ( double ) ( time_check.tm_wday - 1 ) );
	else
		dia_actual = ( char ) pow( 2.0, ( double ) ( time_check.tm_wday + 6 ) );
   
	if( cuo_tar.dias_semana & dia_actual ) {
		rta = 1;
	}

    return ( rta );


}

