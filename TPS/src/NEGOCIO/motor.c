#pragma pack (1) 
//#ifdef MODULO_PROMOCIONES


/*! \file extern.c
    \brief AquÃ­ se define la Interfaz entre el Motor de Promociones y el POS

   Este archivo implementa la interfÃ¡z mediante la cual el Motor de Promociones
   le indica al POS las acciones que este debe ejecutar, y tambien las consultas 
   que le hace a Ã©ste.
   
   Se recomienda comenzar una lectura introductiva por las funciones mÃ¡s importantes:
   credit(), issueBonus(), givePoints(), activateCard(), 
   inDepartment(), inClass(), inGroup(),
   amount(), means(), change(), returned(),
 */

#ifdef INVEL_W

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <cr.h>
#include <cr_disco.h>
#include <btrv.h>
#include <aarch.h>
#include <config.h>
#include "extern.h"
//#include "uprinter.h"
#include "types2.h"
#include "libcache.h"
#include "params.h"
extern struct _articulo_iva iva[10];
#include <deftables.h>

#include "dd.h"
#include "_dd.h"
#include "cmemory.h"
#include "malloc.h"
#include "promo.h"
#include "recup.h"
#include "descuent.h"
#include "actual.h"
#include "cr_pant.h"
#include "imp_tick.h"
#include "mensaje.h"
#include <motor.h>
#include <recupero.h>
#include <path.h>
#include <actions.h>
#include <round.h>
#include <config.h>

#define  LOG_MOTOR_PROMO	5
#define TT_ORIG          0

//#define MEDIO_TARJETAS		7

#ifdef PROMO_FAULT_TOLERANCE
#include "fault.h"
#pragma pack (1)

extern cashdesk_state globalCashdeskState;
extern void * readData( id v, char ind, void *r, size_t size );
#endif //PROMO_FAULT_TOLERANCE

#pragma pack (1)

long codMedioCash(identifier);
long codMedioCtaCte(identifier);
long codMedioMutual(identifier);
long codMedioTicket(identifier);
long codMedioCheque(identifier);
long codMedioCurrency(identifier);
long codMedioCard(identifier);
long codSubMedio(identifier);

char rnv_error = 0; 
// variable global para indicar si el acceso a variables de la RNV
// resulto satisfactoriamente, o si se produjo un error. -> por ejemplo
// al acceder a una variable de cierto tipo con una funcion de otro tipo.
// p.e. string_rnv(nro_caja); => bool_rnv( error ) = TRUE.


// Numero de validacion del bono a entregarse en caso de una falla en la asignacion a monedero electrónico
int on_error_val = 0;
int on_error_vigencia = 0;
char on_error_limite[256] = "";
char on_error_limite_label[256] = "Válido hasta: ";

extern double diferencia_parcial[100]; //las diferencias entre totales en las promos
extern int  i_contador;
extern double SUMAR_TOTAL_FAC_CAJA( int recalcular );
extern int SELECTED2( );
extern  int SELECTED_TIPO( );
extern int SELECT_TABLE( int pTabla, int pTipo );
extern int SET_WHERE( char *pCondicion, ... );
extern int RUN_QUERY(int pDistinct);
extern int FOUND2();
extern int SET_ORDER2( char *pCampos );
extern int OPEN_TABLE( int pTabla, int pTipo, char *pEstruct, long pSizeof );
extern void CR1_MOSTRAR_ITEM( int item, int y, int modificar );
extern void ACUMULA_IVA_ART( int h, long codInt, char* codBar, struct _acu_iva (*acu_gravados)[], struct _acu_iva *acu_exento, double monto );
extern double IMPORTE_MEDIO_PROMOCIONADO( int medio, int submedio, double *importe_ingresado );
int promoIdMotor=0;
/*****************************************************************************/
double PEDIR_FLOAT_EN_RECUADRO_CON_TITULO( char *mensaje )
/*****************************************************************************/
{
    int pant = 0, digitos;
    double valor;
	char *titulo = NULL;
	char *label = NULL;

	{
		char *str;
		char seps[]   = "&";
		char *token;

		str = (char*)malloc(strlen(mensaje)+1);
		strncpy(str,mensaje,strlen(mensaje)+1);

		token = strtok( str, seps );
		if ( token ) {
			titulo = (char*)malloc(strlen(token)+6);
			//memset(titulo,0,strlen(token)+1);
			strncpy(titulo,"     ",5); //el imput es muy chico y sale cortado el mensaje largo
			strncpy(&titulo[5],token,strlen(token)+1);
			token = strtok( NULL, seps );
			if ( token ) {
				label = (char*)malloc(strlen(token)+1);
				strncpy(label,token,strlen(token)+1);
			}
		} else {
			label = (char*)malloc(strlen(mensaje)+1);
			strncpy(label,mensaje,strlen(mensaje)+1);
		}
		if( str)
		free( str );
	}




	ExecuteSSAction( A_PEDIR, _SIMPLE, titulo, label, NULL, " " );
    digitos = ENTEROS + DECIMALES + 1;
    DIBUJAR_RECUADRO_PETICION( titulo, digitos );
    valor = PEDIR_GRAN_FLOTANTE( ( 80 - ancho_panel - digitos * 3 + 1 ) / 2, 19, ENTEROS,
                                 DECIMALES );
    ExecuteAction( A_FIN_PEDIR );

	if (titulo)	{ 
		free( titulo ); 
		titulo = NULL;
	}
	if (label) { 
		free( label ); 
		label = NULL ;
	}
    return ( valor );
}

/*****************************************************************************/
double PEDIR_FLOAT_EN_RECUADRO_CON_TITULO2( char *mensaje )
/*****************************************************************************/
{
    int pant = 0, digitos = 0, lineas = 0, input = 0, i = 0,pos_aux = 0;
	int cant = 0, tam = 0;
    double valor = 0.00;
	char *titulo = NULL;
	char *label = NULL;
	char izquierda = 15;
    char derecha = 0;
    char arriba = 7;
    char abajo = 17;
	char *str;
//	char seps[]   = "&";
	char seps[]   = "|";
	char *token;
	int salir = 0;

	str = (char*)malloc(strlen(mensaje)+1);
	strncpy(str,mensaje,strlen(mensaje)+1);
	tam = strlen(mensaje);

	for (i = 0; i < tam; i++) {
		if( strncmp( &mensaje[i], "|"/*"&"*/, 1 ) == 0 ) {
			lineas++;
		}
		if( strncmp( &mensaje[i], "~", 1 ) == 0 ) {
			input++;
		}
	}

	token = strtok( str, seps );
	cant = (strlen(mensaje)+1);//+ strlen( "& -------------------DATOS DE LA PROMOCION-------------------" );
	label = (char*)malloc(cant);
	titulo = (char*)malloc(strlen(token)+6);
	memset(label,0,strlen(mensaje)+1);
	memset(titulo,0,strlen(token)+6);

	strncpy(titulo,"     &",5); //el imput es muy chico y sale cortado el mensaje largo
	strncpy(&titulo[5],token,strlen(token)+1);
	
	pos_aux= strlen(token)+1 ;//1=&
	//strncpy(str,&mensaje[pos],strlen(mensaje)+1);
	//espero algo como esto Precio Final 65.52   |~CON CUANTO ABONA ?: |Aplico Desc/Recar 18.48
	for( i = 0; i < lineas; i++ ) {//No tenemos en cuenta el titulo
		strncpy(str,&mensaje[pos_aux],strlen(mensaje)+1);
		token = strtok( str, seps );
		if ( token ) {
			if( i != 0 )//En la posicion del input, no debe ir &
				strcat( label, "&" );
			//if( i == 1 )//1era descripcion de la promo 
				//strcat( label, "& -----------------DATOS DE LA PROMOCION----------------- &&" );

			strncat( label, token, strlen(token)+1 );
			pos_aux += strlen(label);
		} else {
			if( !pos_aux ) {
				if( !input && !lineas ) {
					strcat( label, "&" ); //ojo ver cual es el separador real!!!! postpago
				} 
				strcat(label,mensaje,strlen(mensaje)+1);
			}
		}
	}
	izquierda = izquierda - ancho_panel / 2;
    derecha = 80 - ancho_panel / 2 - izquierda;
while( !salir ) {
	MOSTRAR_INPUT_CADENA( "", 0 );
	ExecuteSSAction( A_PEDIR, _HIBRIDO, titulo,/*"~CON CUANTO ABONA ?:*/ /*str*//*"~CON CUANTO ABONA ?: &Aplico Desc/Recar 16.80" */label, NULL, " " );

    valor = PEDIR_GRAN_FLOTANTE( izquierda + 25, arriba + 8, ENTEROS, DECIMALES );
	switch( LASTKEY() ) {
		case 27: //ESC
			salir = 1; 
			

			break;
		case 13: //enter
			if( valor > 0)	
				salir = 1;
			else{
				salir = 0;
					MENSAJE("INGRESE UN IMPORTE POR FAVOR o ESC");
			}
			break;

	}
}


    ExecuteAction( A_FIN_PEDIR );

	if( str)
		free( str );

	if (titulo)	{ 
		free( titulo ); 
		titulo = NULL;
	}
	if (label) { 
		free( label ); 
		label = NULL ;
	}
    return ( valor );
}




//BUSCA_COD_INTERNO_COD_BARRA

/*
 * averigua si se trata de un arreglo, en cuyo caso devuelve 1 y asigna a n y m
 * los valores de variable e índice respectivamente.
 * caso contrario devuelve 0 y asigna a n el numero de variable, y 0 a m.
 */
int arreglo( char *ident, int *n, int *m )
{
    int arr = 0;
    int i = 0;
    *n = 0;
    *m = 0;
    while( ident[i] != '\0' ) {
        switch( ident[i] ) {
            case '_':
                arr = 1;
                {
                    int j = *n;
                    *n = *m;
                    *m = j;
                }
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                *n = *n * 10 + ident[i] - '0';
                break;
            default:
                break;
        }
        i++;
    }
    if( arr ) {
        i = *n;
        *n = *m;
        *m = i;
    }
    return arr;
}



/*****************************************************************************/
double GET_MEMORY_NUM( int var )
/*****************************************************************************/
//double _GET_MEMORY_NUM( int var, int subindice )
//double GET_MEMORY_NUM( int var, int subindice )
{
    switch( DD_TIPO( var ) ) {
        case _SHORT_INT:
            return GET_MEMORY_CHAR( var );
            break;
        case _INT:
            return GET_MEMORY_INT( var );
            break;
        case _FLOAT:
            return GET_MEMORY_FLOAT( var );
            break;
        case _DOUBLE:
            return GET_MEMORY_DOUBLE( var );
            break;
        case _LONG:
            return GET_MEMORY_LONG( var );
            break;
        case _UNSIGNED:
            return GET_MEMORY_UNSIGNED( var );
            break;
        default:
            return 0;
            break;
    }
}



/**
 Convierte una expresiÃ³n genÃ©rica a un string. 
 Por el momento se utiliza para convertir expresiones de medios de pago, al nombre del mismo.
 @param e ExpresiÃ³n a convertir a string.
 */

//int cod_promo = 1;
char* expr2str(expr e) {
  char *s;
  if (e) {
    s = (char*)malloc(256);
    memset( s, ( int )'\0', 256 );
    switch(e->tag) {
    case CASH:
      sprintf(s,"Efectivo");
      break;
    case CARD:
      sprintf(s,"Tarjeta de Credito [%s,%s]", e->e.card.name, e->e.card.plan);
      break;
    case TICKET:
      sprintf(s,"Ticket [%s]", e->e.name);
      break;
    case MUTUAL:
      sprintf(s,"Mutual [%s]", e->e.name);
      break;
    case CHEQUE:
      sprintf(s,"Cheque [%s]", e->e.name);
      break;
    case CURRENCY:
      sprintf(s,"Moneda Extranjera [%s]", e->e.name);
      break;
    case NUM:
      {
	long a = (long) e->e.num;
	if ( e->e.num - (double) a == 0.0 ) {
	  sprintf( s, "%li", a );
	} else {
	  sprintf( s, "%.2lf", e->e.num );
	}
      }
      break;
    default:
      sprintf(s,"Expresion_Desconocida");
      break;
    }
    return s;
  } else {
    return NULL;
  }
}


/**
 Esta funcion se encarga de indicarle al POS que se debe aplicar un descuento.
 @param cant Indica la cantidad de descuentos que se aplicaran.
 @param article Es el identificador del articulo de descarga sobre el cual se debera registrar el descuento.
 @param num El monto del cada descuento aplicado
 @param e En esta expresion se incluyen todos los articulos con sus respectivas cantidad sobre los que 
     se aplica el descuento.
 */
//#define PRORRATEO_AUTOMATICO
#undef PRORRATEO_AUTOMATICO
#ifdef PRORRATEO_AUTOMATICO

void credit( double cant, identifier article, double num, expr e )
{
	expr e1 = NULL;
	artIdent art;

    int p_art;
    int l;
    int i;
    struct _acu_art *acu_art;
    struct _acu_iva acu_prom;
    double total;
    double total_cobros;
    double porc_desc = 0;
    double *base_descuent = NULL;
    char *procesar;
    //long cod_art;
    //int interno;

	long codInt = 0;
	//_int64 codBar = 0;
	char codBar[17] ="";
	double precio = 0;
	//TIPO_COD_ARTICULO r;


	l = sizeof( struct _acu_art ) * _LARGO_TICKET;
	acu_art = MALLOC( l );
	procesar = MALLOC( _LARGO_TICKET );
	memset( procesar, 0, _LARGO_TICKET );

	if (e) {
		if (e->tag == AMSET) {
			double monto_total = 0;
			e1 = ref_e ( clon_amset(e) );
			while (get_amset(e1, &art)) {
				char ident[256];
				memset(ident,0,sizeof(ident));
				if (art.codBar) {
					_snprintf(ident,256,"x%li_%s",art.codInt,art.codBar);
				} else {
					_snprintf(ident,256,"i%li",art.codInt);
				}
				monto_total += amount(ident,art.cant);
			}
			unref_e(e1);
		
			e1 = ref_e ( clon_amset(e) );
			p_art = CARGA_ACU_ART( acu_art, l, NO, &total_cobros, SI, SI );
			memset( &acu_prom, 0, sizeof( acu_prom ) );

			//memset( &acu_prom_excento, 0, sizeof( acu_prom_excento ) );
			//memset( &acu_prom_gravado, 0, sizeof( acu_prom_gravado ) );
			//acu_prom_gravado.exento = 0;
			//acu_prom_excento.exento = 1;
			while (get_amset(e1, &art)) {
				double monto, ponderacion, cant;
				char ident[256];
				memset(ident,0,sizeof(ident));
				if (art.codBar) {
					_snprintf(ident,256,"x%li_%s",art.codInt,art.codBar);
				} else {
					_snprintf(ident,256,"i%li",art.codInt);
				}
				monto = amount(ident,art.cant);
				if (monto_total != 0) {
					ponderacion = monto / monto_total;
				} else {
					// en este caso, el el descuento no se podra prorratear,
					// ya que el conjunto de articulos afectados tienen todos
					// precio $0.
					char buf[256];
					memset(buf,0,sizeof(buf));
					_snprintf(buf, 255, "PROMO: credit: intento de div_by_zero. \
Se pretende prorratear $%.2lf entre un conjunto de \
articulos con precio $0. Se registrara como un \
descuento de $0.", num);
					buf[255] = '\0';
					glog(buf);
					ponderacion = 0;
				}
				//cant = ponderacion * num / (monto/art.cant);
				//monto = ponderacion * num;
				////fprintf( stdout, "el credit afecta a %.2lf articulo(s) x%li_%s\n", art.cant, art.codInt, art.codBar );    
				////ACUMULA_IVA_ART( art.codInt, art.codBar, &acu_prom_gravado, &acu_prom_excento, monto );
				////ACUMULA_IVA_ART2( art.codInt, art.codBar, &acu_prom, monto );


				for( i = 0;i < p_art;i++ ) {
					if ( (acu_art[i].cod == art.codInt) && (strcmp(acu_art[i].cod_barra,art.codBar)==0) ) {
						//ACUMULA_IVA( acu_art[i].pos, &acu_prom, acu_art[i].cant );
						//cant = ponderacion * num / (monto/art.cant);
						if ( acu_art[i].importe != 0 ) {
							cant = ponderacion * num / acu_art[i].importe;
						} else {
							char buf[256];
							memset(buf,0,sizeof(buf));
							_snprintf(buf, 255, "PROMO: credit: intento de div_by_zero. (acu_art[i].importe == 0 )", num);
							buf[255] = '\0';
							glog(buf);
							cant = 0;
						}
						ACUMULA_IVA( acu_art[i].pos, &acu_prom, cant );
						i = p_art; //break;
					}
				}

			}
  
			unref_e(e1);

			


			switch ( getCodsArt( article, &codInt, codBar, &precio) ) {
			case COD_INTERNO:
			case COD_INTERNO_PRECIO:
				#ifdef PROMO_FAULT_TOLERANCE
				if( logCredit( article, num ) ) {
					_AGREGAR_PROMOCION_DESCUENTO( &acu_prom, codInt, NULL, 100, procesar, 1,
												  NULL, _ORIGEN_PROMOCION_DESC );
/*					_AGREGAR_PROMOCION_DESCUENTO( &acu_prom_excento, codInt, NULL, 100, procesar, 1,
												  NULL, _ORIGEN_PROMOCION_DESC );
					_AGREGAR_PROMOCION_DESCUENTO( &acu_prom_gravado, codInt, NULL, 100, procesar, 1,
												  NULL, _ORIGEN_PROMOCION_DESC );
*/					/**/ glog( "PROMO: FAULT_MODULE: CREDIT Aplicado" ,LOG_MOTOR_PROMO,2);
				}
				else {
					/**/ glog( "PROMO: FAULT_MODULE: CREDIT No Aplicado, se ha Recuperado.", LOG_MOTOR_PROMO,2 );
				}
				#else //PROMO_FAULT_TOLERANCE
				_AGREGAR_PROMOCION_DESCUENTO( &acu_prom, codInt, NULL, 100, procesar, 1,
											  &base_descuent, _ORIGEN_PROMOCION_DESC );
/*				_AGREGAR_PROMOCION_DESCUENTO( &acu_prom_excento, codInt, NULL, 100, procesar, 1,
											  &base_descuent, _ORIGEN_PROMOCION_DESC );
				_AGREGAR_PROMOCION_DESCUENTO( &acu_prom_gravado, codInt, NULL, 100, procesar, 1,
											  &base_descuent, _ORIGEN_PROMOCION_DESC );
*/				/**/glog( "PROMO: CREDIT Aplicado" );
				#endif //PROMO_FAULT_TOLERANCE
				break;
			case COD_INTERNO_BARRA:
			case COD_INTERNO_BARRA_PRECIO:
				#ifdef PROMO_FAULT_TOLERANCE
				if( logCredit( article, num ) ) {
					_AGREGAR_PROMOCION_DESCUENTO( &acu_prom, codInt, codBar, 100, procesar, 1,
												  &base_descuent, _ORIGEN_PROMOCION_DESC );
/*					_AGREGAR_PROMOCION_DESCUENTO( &acu_prom_excento, codInt, codBar, 100, procesar, 1,
												  &base_descuent, _ORIGEN_PROMOCION_DESC );
					_AGREGAR_PROMOCION_DESCUENTO( &acu_prom_gravado, codInt, codBar, 100, procesar, 1,
												  &base_descuent, _ORIGEN_PROMOCION_DESC );
*/					/**/ glog( "PROMO: FAULT_MODULE: CREDIT Aplicado" ,LOG_MOTOR_PROMO,2);
				}
				else {
					/**/ glog( "PROMO: FAULT_MODULE: CREDIT No Aplicado, se ha Recuperado." ,LOG_MOTOR_PROMO,2);
				}
				#else //PROMO_FAULT_TOLERANCE
				_AGREGAR_PROMOCION_DESCUENTO( &acu_prom, codInt, codBar, 100, procesar, 1,
											  &base_descuent, _ORIGEN_PROMOCION_DESC );
/*				_AGREGAR_PROMOCION_DESCUENTO( &acu_prom_excento, codInt, codBar, 100, procesar, 1,
											  &base_descuent, _ORIGEN_PROMOCION_DESC );
				_AGREGAR_PROMOCION_DESCUENTO( &acu_prom_gravado, codInt, codBar, 100, procesar, 1,
											  &base_descuent, _ORIGEN_PROMOCION_DESC );
*/				/**/glog( "PROMO: CREDIT Aplicado" );
				#endif //PROMO_FAULT_TOLERANCE
				break;
			case COD_ERROR:
			case COD_BARRA:
			case COD_BARRA_PRECIO:
				/**/glog( "PROMO: CREDIT: El tipo de artículo no está soportado o presenta un error" ,LOG_MOTOR_PROMO,2);
				break;
			default:
				/**/glog( "PROMO: CREDIT: Tipo de artículo desconocido" ,LOG_MOTOR_PROMO,2);
				break;

			}




		} else {
			fprintf( stdout, "credit: ERROR en el 3er argumento\n" ,LOG_MOTOR_PROMO,2);
		}

	FREEMEM( ( char* )acu_art );
	FREEMEM( ( char* )procesar );
	} else {
		glog("PROMO: credit: ERROR, descuento NO aplicado por no reportarse el conjunto de articulos afectados.",LOG_MOTOR_PROMO,2);
	}
}

#else //PRORRATEO_AUTOMATICO
void credit( double cant, identifier article, double num, expr e )
{
	expr e1 = NULL;
	artIdent art;

    int p_art;
    int l;
    int i;
	int v=0;
    struct _acu_art *acu_art;
    //struct _acu_iva acu_prom;
	struct _acu_iva acu_excento;
	struct _acu_iva acu_gravados[MAX_COD_IVA];
//    double total;
    double total_cobros;
    double porc_desc = 0;
    double *base_descuent = NULL;
    char *procesar;
    //long cod_art;
    //int interno;

	long codInt = 0;
	//_int64 codBar = 0;
	char codBar[17] ="";
	long codPresentacion = 0;
	double precio = 0;
	//TIPO_COD_ARTICULO r;


	l = sizeof( struct _acu_art ) * _LARGO_TICKET;
	acu_art = MALLOC( l );
	procesar = MALLOC( _LARGO_TICKET );
	memset( procesar, 0, _LARGO_TICKET );

	if (e) {
		if (e->tag == AMSET) {
			double monto_total = 0;
			e1 = ref_e ( clon_amset(e) );
			while (get_amset(e1, &art)) {
				char ident[256];
				memset(ident,0,sizeof(ident));
				if (art.codBar) {
					if (!art.codPresentacion) {
						_snprintf(ident,256,"x%li_%s",art.codInt,art.codBar);
					} else {
						_snprintf(ident,256,"z%li_%s%li",art.codInt,art.codBar,art.codPresentacion);
					}
				} else {
					_snprintf(ident,256,"i%li",art.codInt);
				}
				monto_total += amount(ident,art.cant);
			}
			unref_e(e1);
		
			e1 = ref_e ( clon_amset(e) );
			p_art = CARGA_ACU_ART( acu_art, l, NO, &total_cobros, SI, SI );
			//memset( &acu_prom, 0, sizeof( acu_prom ) );
			memset( &acu_excento, 0, sizeof( acu_excento ) );
			//for(v=0; v < MAX_COD_IVA; v++) {
			//	memset( &acu_gravados[v], 0, sizeof( acu_gravados ) );
			//}
			memset( &acu_gravados[v], 0, sizeof( acu_gravados ) );

			//memset( &acu_prom_excento, 0, sizeof( acu_prom_excento ) );
			//memset( &acu_prom_gravado, 0, sizeof( acu_prom_gravado ) );
			//acu_prom_gravado.exento = 0;
			//acu_prom_excento.exento = 1;
			while (get_amset(e1, &art)) {
				double monto, ponderacion, cant;
				char ident[256];

				memset(ident,0,sizeof(ident));
				if (art.codBar) {
					_snprintf(ident,256,"x%li_%s",art.codInt,art.codBar);
				} else {
					_snprintf(ident,256,"i%li",art.codInt);
				}
				monto = amount(ident,art.cant);
				if (monto_total != 0) {
					ponderacion = monto / monto_total;
				} else {
					// en este caso, el el descuento no se podra prorratear,
					// ya que el conjunto de articulos afectados tienen todos
					// precio $0.
					char buf[256];

					memset(buf,0,sizeof(buf));
					_snprintf(buf, 255, "PROMO: credit: intento de div_by_zero. \
Se pretende prorratear $%.2lf entre un conjunto de \
articulos con precio $0. Se registrara como un \
descuento de $0.", num);
					buf[255] = '\0';
					glog(buf , LOG_MOTOR_PROMO,3);
					ponderacion = 0;
				}
				//cant = ponderacion * num / (monto/art.cant);
				//monto = ponderacion * num;
				////fprintf( stdout, "el credit afecta a %.2lf articulo(s) x%li_%s\n", art.cant, art.codInt, art.codBar );    
				////ACUMULA_IVA_ART( art.codInt, art.codBar, &acu_prom_gravado, &acu_prom_excento, monto );
				////ACUMULA_IVA_ART2( art.codInt, art.codBar, &acu_prom, monto );


				for( i = 0;i < p_art;i++ ) {
					if ( (acu_art[i].cod == art.codInt) && (strcmp(acu_art[i].cod_barra,art.codBar)==0) ) {
						//ACUMULA_IVA( acu_art[i].pos, &acu_prom, acu_art[i].cant );
						//cant = ponderacion * num / (monto/art.cant);
						if ( acu_art[i].importe != 0 ) {
							cant = ponderacion * num / acu_art[i].importe;
						} else {
							char buf[256];
							memset(buf,0,sizeof(buf));
							_snprintf(buf, 255, "PROMO: credit: intento de div_by_zero. (acu_art[i].importe == 0 )", num);
							buf[255] = '\0';
							glog(buf,LOG_MOTOR_PROMO,3);
							cant = 0;
						}
						//ACUMULA_IVA( acu_art[i].pos, &acu_prom, cant );
						ACUMULA_IVA_ART( acu_art[i].pos, acu_art[i].cod, acu_art[i].cod_barra, acu_gravados, &acu_excento, cant);
						i = p_art; //break;
					}
				}

			}
  
			unref_e(e1);

			


			switch ( getCodsArt( article, &codInt, codBar, &precio, &codPresentacion) ) {
			case COD_INTERNO:
			case COD_INTERNO_PRECIO:
				#ifdef PROMO_FAULT_TOLERANCE
				if( logCredit( article, num ) ) {
//					_AGREGAR_PROMOCION_DESCUENTO( &acu_prom, codInt, NULL, 100, procesar, 1,
//												  NULL, _ORIGEN_PROMOCION_DESC );
/*					_AGREGAR_PROMOCION_DESCUENTO( &acu_prom_excento, codInt, NULL, 100, procesar, 1,
												  NULL, _ORIGEN_PROMOCION_DESC );
					_AGREGAR_PROMOCION_DESCUENTO( &acu_prom_gravado, codInt, NULL, 100, procesar, 1,
												  NULL, _ORIGEN_PROMOCION_DESC );
*/					/**/ glog( "PROMO: FAULT_MODULE: CREDIT Aplicado" ,LOG_MOTOR_PROMO,4);
				}
				else {
					/**/ glog( "PROMO: FAULT_MODULE: CREDIT No Aplicado, se ha Recuperado." ,LOG_MOTOR_PROMO,4);
				}
				#else //PROMO_FAULT_TOLERANCE
				_AGREGAR_PROMOCION_DESCUENTO( &acu_prom, codInt, NULL, 100, procesar, 1,
											  &base_descuent, _ORIGEN_PROMOCION_DESC );
/*				_AGREGAR_PROMOCION_DESCUENTO( &acu_prom_excento, codInt, NULL, 100, procesar, 1,
											  &base_descuent, _ORIGEN_PROMOCION_DESC );
				_AGREGAR_PROMOCION_DESCUENTO( &acu_prom_gravado, codInt, NULL, 100, procesar, 1,
											  &base_descuent, _ORIGEN_PROMOCION_DESC );
*/				/**/glog( "PROMO: CREDIT Aplicado" ,LOG_MOTOR_PROMO,2);
				#endif //PROMO_FAULT_TOLERANCE
				break;
			case COD_INTERNO_BARRA:
			case COD_INTERNO_BARRA_PRESENTACION:
			case COD_INTERNO_BARRA_PRECIO:
			case COD_INTERNO_BARRA_PRESENTACION_PRECIO:
				#ifdef PROMO_FAULT_TOLERANCE
				if( logCredit( article, num ) ) {
//					_AGREGAR_PROMOCION_DESCUENTO( &acu_prom, codInt, codBar, 100, procesar, 1,
//												  &base_descuent, _ORIGEN_PROMOCION_DESC );
					if ( fabs(acu_excento.exento + 
							  acu_excento.importe_sin_iva + 
							  acu_excento.iva1 + 
							  acu_excento.iva2 ) >= 0.0001) {
							_AGREGAR_PROMOCION_DESCUENTO( &acu_excento, codInt, codBar, 100, procesar, 1,
												  &base_descuent, _ORIGEN_PROMOCION_DESC, 1, 0 );
						}
					
					for(v=0; v < MAX_COD_IVA; v++) {
						if ( fabs(acu_gravados[v].exento + 
							  acu_gravados[v].importe_sin_iva + 
							  acu_gravados[v].iva1 + 
							  acu_gravados[v].iva2 ) >= 0.0001) {
							_AGREGAR_PROMOCION_DESCUENTO( &acu_gravados[v], codInt, codBar, 100, procesar, 1,
														  &base_descuent, _ORIGEN_PROMOCION_DESC, 0, /*iva[v].cod_iva*/v );
						}
					}
/*					_AGREGAR_PROMOCION_DESCUENTO( &acu_prom_excento, codInt, codBar, 100, procesar, 1,
												  &base_descuent, _ORIGEN_PROMOCION_DESC );
					_AGREGAR_PROMOCION_DESCUENTO( &acu_prom_gravado, codInt, codBar, 100, procesar, 1,
												  &base_descuent, _ORIGEN_PROMOCION_DESC );
*/					/**/ glog( "PROMO: FAULT_MODULE: CREDIT Aplicado",LOG_MOTOR_PROMO,4);
				}
				else {
					/**/ glog( "PROMO: FAULT_MODULE: CREDIT No Aplicado, se ha Recuperado." ,LOG_MOTOR_PROMO,4);
				}
				#else //PROMO_FAULT_TOLERANCE
				_AGREGAR_PROMOCION_DESCUENTO( &acu_prom, codInt, codBar, 100, procesar, 1,
											  &base_descuent, _ORIGEN_PROMOCION_DESC );
/*				_AGREGAR_PROMOCION_DESCUENTO( &acu_prom_excento, codInt, codBar, 100, procesar, 1,
											  &base_descuent, _ORIGEN_PROMOCION_DESC );
				_AGREGAR_PROMOCION_DESCUENTO( &acu_prom_gravado, codInt, codBar, 100, procesar, 1,
											  &base_descuent, _ORIGEN_PROMOCION_DESC );
*/				/**/glog( "PROMO: CREDIT Aplicado" ,LOG_MOTOR_PROMO,2);
				#endif //PROMO_FAULT_TOLERANCE
				break;
			case COD_ERROR:
			case COD_BARRA:
			case COD_BARRA_PRECIO:
				/**/glog( "PROMO: CREDIT: El tipo de artículo no está soportado o presenta un error" ,LOG_MOTOR_PROMO,2);
				break;
			default:
				/**/glog( "PROMO: CREDIT: Tipo de artículo desconocido" ,LOG_MOTOR_PROMO,3);
				break;

			}




		} else {
			fprintf( stdout, "credit: ERROR en el 3er argumento\n" );
		}
	} else {
		// comportamiento anterior, es decir, se aplica el descuento sobre todos los articulos del ticket.
		//fprintf( stdout, "credit(article %s,%f)\n", article, num );
		//fprintf( stdout, "el credit afecta a TODOS los articulos\n" );    

/*
		p_art = CARGA_ACU_ART( acu_art, l, NO, &total_cobros, SI, SI );
		memset( &acu_prom, 0, sizeof( acu_prom ) );

		for( i = 0;i < p_art;i++ ) {
			ACUMULA_IVA( acu_art[i].pos, &acu_prom, acu_art[i].cant );
		}

		total = acu_prom.importe_sin_iva + acu_prom.exento + acu_prom.iva1 + acu_prom.iva2;
		for( i = 0;i < 4;i++ ) {
			total += acu_prom.monto_impuesto[i];
		}

		if( total != 0 ) {
			porc_desc = num / total;
		}


		switch ( getCodsArt( article, &codInt, codBar, &precio) ) {
		case COD_INTERNO:
		case COD_INTERNO_PRECIO:
			#ifdef PROMO_FAULT_TOLERANCE
			if( logCredit( article, num ) ) {
				_AGREGAR_PROMOCION_DESCUENTO( &acu_prom, codInt, NULL, ( ( float )porc_desc ) * 100, procesar, 1,
											  &base_descuent, _ORIGEN_PROMOCION_DESC );
				glog( "PROMO: FAULT_MODULE: CREDIT Aplicado" ,LOG_MOTOR_PROMO,2);
			}
			else {
				glog( "PROMO: FAULT_MODULE: CREDIT No Aplicado, se ha Recuperado." ,LOG_MOTOR_PROMO,2);
			}
			#else //PROMO_FAULT_TOLERANCE
			_AGREGAR_PROMOCION_DESCUENTO( &acu_prom, codInt, NULL, ( ( float )porc_desc ) * 100, procesar, 1,
										  &base_descuent, _ORIGEN_PROMOCION_DESC );
			glog( "PROMO: CREDIT Aplicado" );
			#endif //PROMO_FAULT_TOLERANCE
			break;
		case COD_INTERNO_BARRA:
		case COD_INTERNO_BARRA_PRECIO:
			#ifdef PROMO_FAULT_TOLERANCE
			if( logCredit( article, num ) ) {
				_AGREGAR_PROMOCION_DESCUENTO( &acu_prom, codInt, codBar, ( ( float )porc_desc ) * 100, procesar, 1,
											  &base_descuent, _ORIGEN_PROMOCION_DESC );
				glog( "PROMO: FAULT_MODULE: CREDIT Aplicado" ,LOG_MOTOR_PROMO,2);
			}
			else {
				glog( "PROMO: FAULT_MODULE: CREDIT No Aplicado, se ha Recuperado." ,LOG_MOTOR_PROMO,2);
			}
			#else //PROMO_FAULT_TOLERANCE
			_AGREGAR_PROMOCION_DESCUENTO( &acu_prom, codInt, codBar, ( ( float )porc_desc ) * 100, procesar, 1,
										  &base_descuent, _ORIGEN_PROMOCION_DESC );
			glog( "PROMO: CREDIT Aplicado" );
			#endif //PROMO_FAULT_TOLERANCE
			break;
		case COD_ERROR:
		case COD_BARRA:
		case COD_BARRA_PRECIO:
			glog( "PROMO: CREDIT: El tipo de artículo no está soportado o presenta un error" ,LOG_MOTOR_PROMO,2);
			break;
		default:
			glog( "PROMO: CREDIT: Tipo de artículo desconocido" ,LOG_MOTOR_PROMO,2);
			break;

		}
*/

	}

	FREEMEM( ( char* )acu_art );
	FREEMEM( ( char* )procesar );
    //MUESTRA_ITEM_EN_PANTALLA( RAM_P_TICKET - 1 );
}
#endif //PRORRATEO_AUTOMATICO

/**
 Esta funcioÃ³n originalmente se utilizaba para indicarle al POS que se debÃ­a regalar determinado artÃ­culo.
 Actualmente es obsoleta.
 */
//void distribute( identifier article, double cantidad, double precio )
void distribute( long promo_id, char* promoIdExtern, long grupo, identifier article, double cantidad, double precio )
{
	long codInt = 0;
	char codBar[17] = "";
	long codPresentacion = 0;
	double valor = 0;

	
	switch ( getCodsArt( article, &codInt, codBar, &valor, &codPresentacion) ) {
	case COD_INTERNO:
	case COD_INTERNO_PRECIO:
        if ( !ASIGNA_CODIGO_ESPECIAL( codInt ) ) {
			/**/glog( "PROMO: DISTRIBUTE: No se encontró el artículo. No se aplicó el beneficio." ,LOG_MOTOR_PROMO,3);
			return;
		}
		break;
	case COD_BARRA:
	case COD_BARRA_PRECIO:
        if ( !ASIGNA_CODIGO_BARRA( codBar ) ) {
			/**/glog( "PROMO: DISTRIBUTE: No se encontró el artículo. No se aplicó el beneficio." ,LOG_MOTOR_PROMO,3);
			return;
		}
		break;
	case COD_INTERNO_BARRA:
	case COD_INTERNO_BARRA_PRECIO:
	case COD_INTERNO_BARRA_PRESENTACION:
	case COD_INTERNO_BARRA_PRESENTACION_PRECIO:
		if ( !ASIGNA_COD_ESPECIAL_COD_BARRA( codInt, codBar ) ) {
			/**/glog( "PROMO: DISTRIBUTE: No se encontró el artículo. No se aplicó el beneficio." ,LOG_MOTOR_PROMO,3);
			return;
		}
		break;
	case COD_ERROR:
		/**/glog( "PROMO: DISTRIBUTE: El tipo de artículo no está soportado o presenta un error. No se aplicó el beneficio." ,LOG_MOTOR_PROMO,3);
		return;
		break;
	default:
		/**/glog( "PROMO: DISTRIBUTE: Tipo de artículo desconocido. No se aplicó el beneficio." ,LOG_MOTOR_PROMO,3);
		return;
		break;

	}

    #ifdef PROMO_FAULT_TOLERANCE
    if( logDistribute( article, cantidad, precio ) == 1 ) {
    #endif //PROMO_FAULT_TOLERANCE

        AGREGAR_DESCUENTO_ARTICULO( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, precio, (float)cantidad, 
			_CORRECCION, /*_ORIGEN_TICKET*/ _ORIGEN_PROMOCION_DESC );
        //AGREGAR_DESCUENTO_ARTICULO( atol(&article[1]), precio, cantidad, 1); 

		recArt( promo_id, promoIdExtern, article, cantidad, grupo );
		recDistribute( promo_id, promoIdExtern, precio, grupo );

    #ifdef PROMO_FAULT_TOLERANCE
    }
    #endif //PROMO_FAULT_TOLERANCE
}


/**
 Esta funciÃ³n se encarga de indicarle al POS que se deben otorgar Bonos o Cupones.
 @param target Identificador que indica el destino del bono a estregar, como por ejemplo 
    SHOW (para mostrar la cantidad de cupones que el cajero debe otorgar al cliente)
    INTICKET (para imprimir el bono en el ticket)
    OUTSIDETICKET (para imprimir el bono fuera del ticket)
 @param type En este identificador se indica el tipo de bono a otorgar. AquÃ­ se indica
    el nÃºmero de validaciÃ³n o de indentificaciÃ³n de dicho bono.
 @param valor Monto o Valor en pesos (o en la moneda local correspondiente) de cada bono a entregarse.
 @param cantidad Es la cantidad de bonos a entregarse.
 @para leyenda En caso que el tipo de bono a imprimirse acepte alguna cadena descriptiva, en este
    parÃ¡metro se pasa la misma.
 */
 void issueBonus( identifier target, identifier type, double valor, double cantidad, char *leyenda, idate vencimiento, double validez )
{
	struct asig_beneficio asig_benef;
	memset( &asig_benef, 0, sizeof(struct asig_beneficio) );

	{	/* LOG */
		char tmp[256];
		char buff[64];

		memset(tmp,0,sizeof(tmp));
		memset(buff,0,sizeof(buff));
		strdate( buff, 64, vencimiento, 1 );

		_snprintf( tmp, 255, "PROMO: issueBonus(target %s, type %s, valor %lf, cantidad %lf, leyenda %s, vencimiento %s, validez %lf)\n",
			target, type, valor, cantidad, leyenda, buff, validez);
		tmp[255] = '\0';
		glog(tmp,LOG_MOTOR_PROMO,4);
	}

		asig_benef.tipo = ASIG_BENEF_BONO;
		if (type) {
			if (strlen(type)>1) {
				asig_benef.nro_validacion = atoi(&type[1]);
			} else {
				asig_benef.nro_validacion = 0;
			}
		} else {
			asig_benef.nro_validacion = 0;
		}

		asig_benef.cantidad = (int) cantidad;
		asig_benef.monto = valor;
	{
		idate vence = 0;
		if ( vencimiento < get_date() ) {
			// el vencimiento es anterior a la fecha actual.
			// Puede significar si la fecha es del año 2000 o anterior,
			// indica que se debe ignorar ese valor, y el bono no tiene
			// fecha de vencimiento.

			{ // Obtener el año de vencimiento para decidir si se tiene en cuenta la fecha de vencimiento
				char buf[11];
				int y,m,d;
				if( strdate( buf, 11, vencimiento, 0 ) == 0 ) {
					sscanf( buf, "%d-%d-%d", &y, &m, &d );
					if ( y > 2000 ) {
						vence = vencimiento;
					}
				}
	}
			} else {
			vence = vencimiento;
			}
		if ( validez > 0 ) {
			// el bono es válido por 'validez' dias.
			if ( vence > 0 ) {
				vence = min(vence, get_date() + (long) validez );
		} else {
				vence = get_date() + (long) validez;
			}
		}

		if ( vence > 0 ) {
			char buf[64];
			memset(buf,0,sizeof(buf));

			strdate( buf, 64, vence, 1 );
			_snprintf( asig_benef.msg_string, sizeof(asig_benef.msg_string), "%s %s\n%s", 
				on_error_limite_label, buf, leyenda);
		} else {
			_snprintf( asig_benef.msg_string, sizeof(asig_benef.msg_string), "%s", leyenda);
		}
		glog(asig_benef.msg_string,LOG_MOTOR_PROMO,4);
		asig_benef.msg_string[sizeof(asig_benef.msg_string)-1] = '\0';
	}
		asig_benef.asignado = 0;
	asig_benef.vigencia = (int)validez;


	if ( strcmp(target, "outsideticket") == 0 ) {
        glog( "PROMO: Imprimir BONOS fuera del Ticket" ,LOG_MOTOR_PROMO,5);
		asig_benef.lugar_impresion = FUERA_DEL_TICKET;
		GUARDAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO, (char *)&asig_benef,
		sizeof(struct asig_beneficio));
	}
	else if ( strcmp(target, "inticket") == 0 ) {
        glog( "PROMO: Imprimir BONOS dentro del Ticket" ,LOG_MOTOR_PROMO,5);
		asig_benef.lugar_impresion = DENTRO_DEL_TICKET;
		GUARDAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO, (char *)&asig_benef,
		sizeof(struct asig_beneficio));

	}
    else if( strcmp( target, "show" ) == 0 ) {
        //----- Mostrar cupones -----//
        glog( "PROMO: Mostrar BONOS" ,LOG_MOTOR_PROMO,4);
        ADD_MEMORY_INT( __ram_bonos_a_entregar, (int)cantidad );
    }


}


void givePoints( identifier point, double num )
{
    #ifdef PROMO_FAULT_TOLERANCE
//    if( logGivePoints( point, num ) == 1 ) {
        #endif //PROMO_FAULT_TOLERANCE

        if( strcmp( point, "local" ) == 0 ) {
            /**/ glog( "PROMO: GIVEPOINTS(local)" ,LOG_MOTOR_PROMO,4);
            ADD_MEMORY_DOUBLE( __ram_acu_puntos, num );
        }
        else if( strcmp( point, "central" ) == 0 ) {
            // "central"
            /**/ glog( "PROMO: GIVEPOINTS(central)" ,LOG_MOTOR_PROMO,4);
            ////// ///////////// falta completar -> aun no se como es este caso (o como es el caso local)
        }
        else if( strcmp( point, "birthday" ) == 0 ) {
            /**/ glog( "PROMO: GIVEPOINTS(birthday)" ,LOG_MOTOR_PROMO,4);
            ADD_MEMORY_DOUBLE( __ram_acu_puntos_cumple, num );
        }
        else if( strcmp( point, "money" ) == 0 ) {
			struct asig_beneficio asig_benef;
			memset( &asig_benef, 0, sizeof(struct asig_beneficio) );

			glog( "PROMO: Otorgar Puntos al MONEDERO ELECTRONICO" ,LOG_MOTOR_PROMO,4);

			asig_benef.tipo = ASIG_BENEF_MONEDERO;
			//asig_benef.cantidad = (int) num;
			asig_benef.monto = num;
			asig_benef.nro_validacion = on_error_val;
			asig_benef.vigencia = on_error_vigencia;
			strcpy(asig_benef.msg_string, on_error_limite);

			GUARDAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO, (char *)&asig_benef,
			sizeof(struct asig_beneficio));
		
		}
        else {
            /**/ glog( "PROMO: GIVEPOINTS(puntos electronicos)" ,LOG_MOTOR_PROMO,4);
            /////////////// como se acumulan los puntos electronicos ??????
        }

        /*
        {
            char tmp[50];sprintf( tmp, "PROMO: Puntos Agregados: %f", RAM_ACU_PUNTOS );glog( tmp );
        }
		*/
        #ifdef PROMO_FAULT_TOLERANCE
    //}
    #endif //PROMO_FAULT_TOLERANCE
}


void activateCard( long promoId, identifier submedio, identifier plan, double num )
{
	struct activacion_medios act_medio;
	memset( &act_medio, 0, sizeof(struct activacion_medios) );
	promoIdMotor =promoId;

	{
		char tmp[256];
		sprintf( tmp, "PROMO: activateCard(promoId %li, submedio %s, plan %s, monto %f)\n", promoId, submedio, plan, num );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}
	
	act_medio.tipo = ACTIVAR_MEDIO;
	act_medio.cod_promo = promoId;
	//act_medio.medio = MEDIO_TARJETAS;		// <--- CUIDADO: Se debe implementar la busqueda que determina que el MEDIO TARJETA corresponde al código 3
	act_medio.medio = codMedioCard(submedio);

	{
		char tmp[256];
		sprintf(tmp,"PROMO: DEBUG: Codigo de Medio de TARJETA: %li",codMedioCard(submedio));
		glog(tmp,LOG_MOTOR_PROMO,4);
	}

	if (submedio) {
		if (strlen(submedio)>1) {
			act_medio.submedio = codSubMedio(submedio);
			//act_medio.submedio = atol(&submedio[1]);
		} else {
			act_medio.submedio = 0;
		}
	} else {
		act_medio.submedio = 0;
	}
	if (plan) {
		if (strlen(plan)>1) {
			act_medio.plan = atol(&plan[1]);
		} else {
			act_medio.plan = 0;
		}
	} else {
		act_medio.plan = 0;
	}

	act_medio.monto = num;
	act_medio.monto_usado = 0;

	GUARDAR_ARCHIVO_REC(ARCH_REC_ACT_MEDIO_MOTOR, (char *)&act_medio,
	sizeof(struct activacion_medios));
}

void activateCurrency( identifier ident, double num )
{
	char tmp[256];
    sprintf( tmp, "PROMO: activateCurrency(ident %s, %f)\n", ident, num );
	glog(tmp,LOG_MOTOR_PROMO,4);
}

//void activateMutual( identifier ident, double num )
void activateMutual( long promoId, identifier ident, double num )
{
	struct activacion_medios act_medio;
	memset( &act_medio, 0, sizeof(struct activacion_medios) );
	promoIdMotor =promoId;

	{
		char tmp[256];
		sprintf( tmp, "PROMO: activateMutual(promoId %li, ident %s, monto %lf)\n", promoId, ident, num );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}

	act_medio.tipo = ACTIVAR_MEDIO;
	act_medio.cod_promo = promoId;

	if ( ident[0] == 'm' ) {
		// name es de la forma mXXXsYYY con XXX código de medio e YYY código de submedio.
		char *str;
		char seps[]   = "ms";
		char *token;

		str = (char*)malloc(strlen(ident)+1);
		strncpy(str,ident,strlen(ident)+1);

		token = strtok( str, seps );
		if ( token ) {
			act_medio.medio = atol(token);
			if ( act_medio.medio < 0 || act_medio.medio > 9 ) {
				// en caso de que el medio este fuera del rango valido
				act_medio.medio = codMedioMutual(ident);
			}
			token = strtok( NULL, seps );
			if ( token ) {
				act_medio.submedio = atol(token);
			}
		}
		if(str )
		free( str );		
	} else {
		// en name se codifica sólo el código de submedio
		act_medio.medio = codMedioMutual(ident);
		//act_medio.medio = codSubMedioMutual(ident);
		act_medio.submedio = codSubMedio(ident);
		//act_medio.submedio = atol(&ident[1]);
	}

	act_medio.monto = num;

	GUARDAR_ARCHIVO_REC(ARCH_REC_ACT_MEDIO_MOTOR, (char *)&act_medio,
		sizeof(struct activacion_medios));
}

void activateCheque( long promoId, identifier submedio, double num )
{
	struct activacion_medios act_medio;
	memset( &act_medio, 0, sizeof(struct activacion_medios) );
	promoIdMotor =promoId;

	{
		char tmp[256];
		sprintf( tmp, "PROMO: activateCheque(promoId %li, submedio %s, monto %lf)\n", promoId, submedio, num );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}

	act_medio.tipo = ACTIVAR_MEDIO;
	act_medio.cod_promo = promoId;
	act_medio.medio = codMedioCheque(submedio);

	{
		char tmp[256];
		sprintf(tmp,"PROMO: DEBUG: Codigo de Medio de CHEQUE encontrado: %li",codMedioCheque(submedio));
		glog(tmp,LOG_MOTOR_PROMO,5);
	}

	if (submedio) {
		if (strlen(submedio)>1) {
			act_medio.submedio = codSubMedio(submedio);
			//act_medio.submedio = atol(&submedio[1]);
		} else {
			act_medio.submedio = 0;
		}
	} else {
		act_medio.submedio = 0;
	}
	act_medio.monto = num;

	GUARDAR_ARCHIVO_REC(ARCH_REC_ACT_MEDIO_MOTOR, (char *)&act_medio,
		sizeof(struct activacion_medios));
}

//void activateTicket( identifier ident, double num )
void activateTicket( long promoId, identifier ident, double num )
{
	struct activacion_medios act_medio;
	memset( &act_medio, 0, sizeof(struct activacion_medios) );
	promoIdMotor =promoId;

	{
		char tmp[256];
		sprintf( tmp, "PROMO: activateTicket(promoId %li, ident %s, monto %lf)\n", promoId, ident, num );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}

	act_medio.tipo = ACTIVAR_MEDIO;
	act_medio.cod_promo = promoId;

	if ( ident[0] == 'm' ) {
		// name es de la forma mXXXsYYY con XXX código de medio e YYY código de submedio.
		char *str;
		char seps[]   = "ms";
		char *token;

		str = (char*)malloc(strlen(ident)+1);
		strncpy(str,ident,strlen(ident)+1);

		token = strtok( str, seps );
		if ( token ) {
			act_medio.medio = atol(token);
			if ( act_medio.medio < 0 || act_medio.medio > 9 ) {
				// en caso de que el medio este fuera del rango valido
				act_medio.medio = codMedioTicket(ident);
			}
			token = strtok( NULL, seps );
			if ( token ) {
				act_medio.submedio = atol(token);
			}
		}
		if(str )
		free( str );		
	} else {
		// en name se codifica sólo el código de submedio
		act_medio.medio = codMedioTicket(ident);
		act_medio.submedio = codSubMedio(ident);
		//act_medio.submedio = atol(&ident[1]);
	}

	act_medio.monto = num;

	GUARDAR_ARCHIVO_REC(ARCH_REC_ACT_MEDIO_MOTOR, (char *)&act_medio,
		sizeof(struct activacion_medios));
}

void activateCash( double num )
{
	char tmp[256];
    sprintf( tmp, "PROMO: activateCash(%f)\n", num );
	glog(tmp,LOG_MOTOR_PROMO,4);
}
/**
 Esta función se utiliza para ejecutar procedimientos o funciones externas, definidas en el TPV,
a modo de extensión. (no se hace ningún tipo de chequeo de tipos)

 @param procName Es el identificador del procedimiento que se desea invocar.
 @param params Es la lista de nombres de parámetros del procedimiento que se desea invocar,
  junto a los valores de los mismos,
 */
void extProc( identifier procName, char* params ) 
{
  //fprintf( stdout, "PROMO: eproc(%s,%s)\n", procName, params );
    assert( procName && strlen(procName) > 1 );

	#ifdef PROMO_FAULT_TOLERANCE
	if( logEProc( procName, params ) ) {
	#endif // PROMO_FAULT_TOLERANCE

		if ( strcmp( &procName[1], "video" ) == 0 ) {
			char* file = NULL;
			double w = -1, h = -1, x = -1, y = -1;
			if ( !getStrParam(params, "file", &file) ) {
				getNumParam(params, "height", &h);
				getNumParam(params, "width", &w);
				getNumParam(params, "x", &x);
				getNumParam(params, "y", &y);
				//print(file);
				REP_VIDEO(file,(int)w,(int)h,(int)x,(int)y);
				if (file) free(file);
			} else {
				glog("VIDEO: Parametros incorrectos. No se reproduce video",LOG_MOTOR_PROMO,3);
			}
		}

	#ifdef PROMO_FAULT_TOLERANCE
	}
	#endif // PROMO_FAULT_TOLERANCE
	
}



// MODO 0 -> Se debe pagar todo el ticket con alguno de los medios del grupo.
// MODO 1 -> El monto pagado con los medios del mismo grupo debe cubrir el total del ticket.
// MODO 2 -> Se debe pagar al menos $'num' con alguno de los medios del grupo.
// MODO 3 -> El monto pagado con los medios del mismo grupo debe cubrir al menos $'num'.
void reqCard( int group, int modo, identifier submedio, identifier plan, double num ) {
	struct requerir_medios rec_medio;
	memset( &rec_medio, 0, sizeof(struct requerir_medios) );
	{
		char tmp[256];
		sprintf( tmp, "PROMO: requireCard(grupo %li, modo %i, submedio %s, plan %s, monto %f)\n", group, modo, submedio, plan, num );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}

	rec_medio.cod_grupo = group;
	rec_medio.medio = codMedioCard(submedio);
	rec_medio.origen = ORIGEN_PROMOCION;
	rec_medio.cod_promo = promoIdMotor;
	if ( submedio ) {
		rec_medio.submedio = codSubMedio(submedio);
		//rec_medio.submedio = atol(&submedio[1]);
	} else {
		rec_medio.submedio = -1;
	}
	if ( plan ) {
		rec_medio.plan = atol(&plan[1]);
	} else {
		rec_medio.plan = -1;
	}
	switch (modo) {
	case 0: // Se debe pagar todo el ticket con alguno de los medios del grupo.
		rec_medio.monto = -1;
		rec_medio.monto_compartido = 0;
		rec_medio.condicion_monto =  3;
		break;
	case 1: // El monto pagado con los medios del mismo grupo debe cubrir el total del ticket.
		rec_medio.monto = -1;
		rec_medio.monto_compartido = 1;
		rec_medio.condicion_monto =  3;
		break;
	case 2: // Se debe pagar al menos $'num' con alguno de los medios del grupo.
		rec_medio.monto = num;
		rec_medio.monto_compartido = 0;
		rec_medio.condicion_monto =  1;
		break;
	case 3: // El monto pagado con los medios del mismo grupo debe cubrir al menos $'num'.
		rec_medio.monto = num;
		rec_medio.monto_compartido = 1;
		rec_medio.condicion_monto =  1;
		break;
	default:
		glog("PROMO: requireCard: ERROR: comportamiento no definido.",LOG_MOTOR_PROMO,3);
		break;
	}
	GUARDAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO, (char *)&rec_medio,
		sizeof(struct requerir_medios));

}

void reqCurrency( int group, int modo, identifier ident, double num ) {
	struct requerir_medios rec_medio;
	memset( &rec_medio, 0, sizeof(struct requerir_medios) );
	{
		char tmp[256];
		sprintf( tmp, "PROMO: requireCurrency(grupo %li, modo %i, submedio %s, monto %f)\n", group, modo, ident, num );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}

	rec_medio.cod_grupo = group;
	rec_medio.medio = codMedioCurrency(ident);
	rec_medio.cod_promo = promoIdMotor;
	if ( ident ) {
		rec_medio.submedio = codSubMedio(ident);
		//rec_medio.submedio = atol(&ident[1]);
	} else {
		rec_medio.submedio = -1;
	}
	rec_medio.plan = -1;
	switch (modo) {
	case 0: // Se debe pagar todo el ticket con alguno de los medios del grupo.
		rec_medio.monto = -1;
		rec_medio.monto_compartido = 0;
		rec_medio.condicion_monto =  3;
		break;
	case 1: // El monto pagado con los medios del mismo grupo debe cubrir el total del ticket.
		rec_medio.monto = -1;
		rec_medio.monto_compartido = 1;
		rec_medio.condicion_monto =  3;
		break;
	case 2: // Se debe pagar al menos $'num' con alguno de los medios del grupo.
		rec_medio.monto = num;
		rec_medio.monto_compartido = 0;
		rec_medio.condicion_monto =  1;
		break;
	case 3: // El monto pagado con los medios del mismo grupo debe cubrir al menos $'num'.
		rec_medio.monto = num;
		rec_medio.monto_compartido = 1;
		rec_medio.condicion_monto =  1;
		break;
	default:
		glog("PROMO: requireCurrency: ERROR: comportamiento no definido.",LOG_MOTOR_PROMO,3);
		break;
	}
	GUARDAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO, (char *)&rec_medio,
		sizeof(struct requerir_medios));

}

void reqMutual( int group, int modo, identifier ident, double num ) {
	struct requerir_medios rec_medio;
	memset( &rec_medio, 0, sizeof(struct requerir_medios) );
	{
		char tmp[256];
		sprintf( tmp, "PROMO: requireMutual(grupo %li, modo %i, submedio %s, monto %f)\n", group, modo, ident, num );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}

	rec_medio.cod_grupo = group;
	rec_medio.medio = codMedioMutual(ident);
	rec_medio.origen = ORIGEN_PROMOCION;
	rec_medio.cod_promo = promoIdMotor;
	if ( ident ) {
		rec_medio.submedio = codSubMedio(ident);
		//rec_medio.submedio = atol(&ident[1]);
	} else {
		rec_medio.submedio = -1;
	}
	rec_medio.plan = -1;
	switch (modo) {
	case 0: // Se debe pagar todo el ticket con alguno de los medios del grupo.
		rec_medio.monto = -1;
		rec_medio.monto_compartido = 0;
		rec_medio.condicion_monto =  3;
		break;
	case 1: // El monto pagado con los medios del mismo grupo debe cubrir el total del ticket.
		rec_medio.monto = -1;
		rec_medio.monto_compartido = 1;
		rec_medio.condicion_monto =  3;
		break;
	case 2: // Se debe pagar al menos $'num' con alguno de los medios del grupo.
		rec_medio.monto = num;
		rec_medio.monto_compartido = 0;
		rec_medio.condicion_monto =  1;
		break;
	case 3: // El monto pagado con los medios del mismo grupo debe cubrir al menos $'num'.
		rec_medio.monto = num;
		rec_medio.monto_compartido = 1;
		rec_medio.condicion_monto =  1;
		break;
	default:
		glog("PROMO: requireMutual: ERROR: comportamiento no definido.",LOG_MOTOR_PROMO,3);
		break;
	}
	GUARDAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO, (char *)&rec_medio,
		sizeof(struct requerir_medios));

}

void reqCheque( int group, int modo, identifier ident, double num ) {
	struct requerir_medios rec_medio;
	memset( &rec_medio, 0, sizeof(struct requerir_medios) );
	{
		char tmp[256];
		sprintf( tmp, "PROMO: requireCheque(grupo %li, modo %i, submedio %s, monto %f)\n", group, modo, ident, num );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}

	rec_medio.cod_grupo = group;
	rec_medio.medio = codMedioCheque(ident);
	rec_medio.origen = ORIGEN_PROMOCION;
	rec_medio.cod_promo = promoIdMotor;
	if ( ident ) {
		rec_medio.submedio = codSubMedio(ident);
		//rec_medio.submedio = atol(&ident[1]);
	} else {
		rec_medio.submedio = -1;
	}
	rec_medio.plan = -1;
	switch (modo) {
	case 0: // Se debe pagar todo el ticket con alguno de los medios del grupo.
		rec_medio.monto = -1;
		rec_medio.monto_compartido = 0;
		rec_medio.condicion_monto =  3;
		break;
	case 1: // El monto pagado con los medios del mismo grupo debe cubrir el total del ticket.
		rec_medio.monto = -1;
		rec_medio.monto_compartido = 1;
		rec_medio.condicion_monto =  3;
		break;
	case 2: // Se debe pagar al menos $'num' con alguno de los medios del grupo.
		rec_medio.monto = num;
		rec_medio.monto_compartido = 0;
		rec_medio.condicion_monto =  1;
		break;
	case 3: // El monto pagado con los medios del mismo grupo debe cubrir al menos $'num'.
		rec_medio.monto = num;
		rec_medio.monto_compartido = 1;
		rec_medio.condicion_monto =  1;
		break;
	default:
		glog("PROMO: requireCheque: ERROR: comportamiento no definido.",LOG_MOTOR_PROMO,3);
		break;
	}
	GUARDAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO, (char *)&rec_medio,
		sizeof(struct requerir_medios));

}

void reqTicket( int group, int modo, identifier ident, double num ) {
	struct requerir_medios rec_medio;
	memset( &rec_medio, 0, sizeof(struct requerir_medios) );
	{
		char tmp[256];
		sprintf( tmp, "PROMO: requireTicket(grupo %li, modo %i, submedio %s, monto %f)\n", group, modo, ident, num );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}

	rec_medio.cod_grupo = group;
	rec_medio.medio = codMedioTicket(ident);
	rec_medio.origen = ORIGEN_PROMOCION;
	rec_medio.cod_promo = promoIdMotor;
	if ( ident ) {
		rec_medio.submedio = codSubMedio(ident);
		//rec_medio.submedio = atol(&ident[1]);
	} else {
		rec_medio.submedio = -1;
	}
	rec_medio.plan = -1;
	switch (modo) {
	case 0: // Se debe pagar todo el ticket con alguno de los medios del grupo.
		rec_medio.monto = -1;
		rec_medio.monto_compartido = 0;
		rec_medio.condicion_monto =  3;
		break;
	case 1: // El monto pagado con los medios del mismo grupo debe cubrir el total del ticket.
		rec_medio.monto = -1;
		rec_medio.monto_compartido = 1;
		rec_medio.condicion_monto =  3;
		break;
	case 2: // Se debe pagar al menos $'num' con alguno de los medios del grupo.
		rec_medio.monto = num;
		rec_medio.monto_compartido = 0;
		rec_medio.condicion_monto =  1;
		break;
	case 3: // El monto pagado con los medios del mismo grupo debe cubrir al menos $'num'.
		rec_medio.monto = num;
		rec_medio.monto_compartido = 1;
		rec_medio.condicion_monto =  1;
		break;
	default:
		glog("PROMO: requireTicket: ERROR: comportamiento no definido.",LOG_MOTOR_PROMO,3);
		break;
	}
	GUARDAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO, (char *)&rec_medio,
		sizeof(struct requerir_medios));

}

void reqCash( int group, int modo, double num ) {
	struct requerir_medios rec_medio;
	memset( &rec_medio, 0, sizeof(struct requerir_medios) );
	{
		char tmp[256];
		sprintf( tmp, "PROMO: requireCash(grupo %li, modo %i, monto %f)\n", group, modo, num );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}

	rec_medio.cod_grupo = group;
	rec_medio.medio = codMedioCash(NULL);
	rec_medio.submedio = -1;
	rec_medio.plan = -1;
	rec_medio.origen = ORIGEN_PROMOCION;
	rec_medio.cod_promo = promoIdMotor;
	switch (modo) {
	case 0: // Se debe pagar todo el ticket con alguno de los medios del grupo.
		rec_medio.monto = -1;
		rec_medio.monto_compartido = 0;
		rec_medio.condicion_monto =  3;
		break;
	case 1: // El monto pagado con los medios del mismo grupo debe cubrir el total del ticket.
		rec_medio.monto = -1;
		rec_medio.monto_compartido = 1;
		rec_medio.condicion_monto =  3;
		break;
	case 2: // Se debe pagar al menos $'num' con alguno de los medios del grupo.
		rec_medio.monto = num;
		rec_medio.monto_compartido = 0;
		rec_medio.condicion_monto =  1;
		break;
	case 3: // El monto pagado con los medios del mismo grupo debe cubrir al menos $'num'.
		rec_medio.monto = num;
		rec_medio.monto_compartido = 1;
		rec_medio.condicion_monto =  1;
		break;
	default:
		glog("PROMO: requireCash: ERROR: comportamiento no definido.",LOG_MOTOR_PROMO,3);
		break;
	}
	GUARDAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO, (char *)&rec_medio,
		sizeof(struct requerir_medios));

}


void reqCtacte( int group, int modo, double num ) {
	struct requerir_medios rec_medio;
	memset( &rec_medio, 0, sizeof(struct requerir_medios) );
	{
		char tmp[256];
		sprintf( tmp, "PROMO: requireCtacte(grupo %li, modo %i, monto %f)\n", group, modo, num );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}

	rec_medio.cod_grupo = group;
	rec_medio.medio = codMedioCtaCte(NULL);
	rec_medio.submedio = -1;
	rec_medio.plan = -1;
	rec_medio.origen = ORIGEN_PROMOCION;
	rec_medio.cod_promo = promoIdMotor;
	switch (modo) {
	case 0: // Se debe pagar todo el ticket con alguno de los medios del grupo.
		rec_medio.monto = -1;
		rec_medio.monto_compartido = 0;
		rec_medio.condicion_monto =  3;
		break;
	case 1: // El monto pagado con los medios del mismo grupo debe cubrir el total del ticket.
		rec_medio.monto = -1;
		rec_medio.monto_compartido = 1;
		rec_medio.condicion_monto =  3;
		break;
	case 2: // Se debe pagar al menos $'num' con alguno de los medios del grupo.
		rec_medio.monto = num;
		rec_medio.monto_compartido = 0;
		rec_medio.condicion_monto =  1;
		break;
	case 3: // El monto pagado con los medios del mismo grupo debe cubrir al menos $'num'.
		rec_medio.monto = num;
		rec_medio.monto_compartido = 1;
		rec_medio.condicion_monto =  1;
		break;
	default:
		glog("PROMO: requireCctacte: ERROR: comportamiento no definido.",LOG_MOTOR_PROMO,3);
		break;
	}
	GUARDAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO, (char *)&rec_medio,
		sizeof(struct requerir_medios));

}



//struct eventos_promociones
//{
//    int b_promo;            //tipos de evento de promociones (constantes)
//    int cod_promo;			//identificador de la promoción
//	int id_grupo;			//numero del grupo de eventos al que pertenece este registro
//    double cantidad;        //Indica la cantidad en caso que el beneficio lo requiera Ej: 5 artículos o 25 puntos asignados al monedero
//    double precio_fijo;     //Indica el valor o precio en caso que el beneficio lo requiera Ej: descuento del 5%
//    float porc_desc;	    //Indica el porcentaje en caso que el beneficio lo requiera Ej: descuento de $5
//    long cod_arti;			//código interno del artículo
//    long cod_medio;			//código del medio de pago
//    long cod_submedio;		//código del submedio
//    long modo_plan_pago;	//plan de pago
//    int nro_validacion;     //Numero de validacion correspondiente al val-caja
//};


void recArt( long promoId, char* promoIdExt, identifier art, double cant, long grupo )
{
	struct eventos_promociones ev_promo;
	double precio = 0;

	memset( &ev_promo, 0, sizeof(struct eventos_promociones) );

	/*{
		char tmp[256];
		sprintf( tmp, "PROMO: rec_Articles(promo=%li, article=%li, cantidad=%lf, grupo=%li)\n", promoId,
				 atol( &art[1] ), cant, grupo );
		glog(tmp);
	}*/
	
	ev_promo.b_promo = BENEF_ARTICULOS;
	ev_promo.cod_promo = promoId;
	strncpy(ev_promo.cod_promo_ext, promoIdExt, sizeof(ev_promo.cod_promo_ext)-1);

	ev_promo.id_grupo = grupo;
	ev_promo.cantidad = cant;

	
	switch ( getCodsArt( art, &(ev_promo.cod_arti), &(ev_promo.cod_barr[0]), &precio, &(ev_promo.cod_presentacion)) ) {
	case COD_INTERNO:
	case COD_INTERNO_PRECIO:
	case COD_BARRA:
	case COD_BARRA_PRECIO:
	case COD_INTERNO_BARRA:
	case COD_INTERNO_BARRA_PRECIO:
	case COD_INTERNO_BARRA_PRESENTACION:
	case COD_INTERNO_BARRA_PRESENTACION_PRECIO:
		GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo, sizeof(struct eventos_promociones));
		break;
	case COD_ERROR:
		/**/glog( "PROMO: recArt: El tipo de artículo no está soportado o presenta un error. No se aplicó el beneficio." ,LOG_MOTOR_PROMO,3);
		break;
	default:
		/**/glog( "PROMO: recArt: Tipo de artículo desconocido. No se aplicó el beneficio." ,LOG_MOTOR_PROMO,3);
		break;

	}
}

void recCash( long promoId, char* promoIdExt, long grupo )
{
	struct eventos_promociones ev_promo;
	memset( &ev_promo, 0, sizeof(struct eventos_promociones) );
	promoIdMotor =promoId;

	{
		char tmp[256];
	    sprintf( tmp, "PROMO: rec_Cash(promo=%li, grupo=%li)\n", promoId, grupo );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}
	
	ev_promo.b_promo = BENEF_MEDIO;
	ev_promo.cod_promo = promoId;
	strncpy(ev_promo.cod_promo_ext, promoIdExt, sizeof(ev_promo.cod_promo_ext)-1);
	ev_promo.id_grupo = grupo;
	ev_promo.cod_medio = codMedioCash(NULL);

	GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo,
	sizeof(struct eventos_promociones));

}

void recCtacte( long promoId, char* promoIdExt, long grupo )
{
	struct eventos_promociones ev_promo;
	memset( &ev_promo, 0, sizeof(struct eventos_promociones) );
	promoIdMotor =promoId;

	{
		char tmp[256];
	    sprintf( tmp, "PROMO: rec_CtaCte(promo=%li, grupo=%li)\n", promoId, grupo );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}
	
	ev_promo.b_promo = BENEF_MEDIO;
	ev_promo.cod_promo = promoId;
	strncpy(ev_promo.cod_promo_ext, promoIdExt, sizeof(ev_promo.cod_promo_ext)-1);
	ev_promo.id_grupo = grupo;
	ev_promo.cod_medio = codMedioCtaCte(NULL);

	GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo,
	sizeof(struct eventos_promociones));

}

void recMutual( long promoId, char* promoIdExt, identifier name, long grupo )
{
	struct eventos_promociones ev_promo;
	memset( &ev_promo, 0, sizeof(struct eventos_promociones) );
	promoIdMotor =promoId;

	{
		char tmp[256];
	    sprintf( tmp, "PROMO: rec_Mutual(promo=%li, sub_medio=%s, grupo=%li)\n", promoId, name, grupo );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}
	
	ev_promo.b_promo = BENEF_MEDIO;
	ev_promo.cod_promo = promoId;
	strncpy(ev_promo.cod_promo_ext, promoIdExt, sizeof(ev_promo.cod_promo_ext)-1);
	ev_promo.id_grupo = grupo;
	ev_promo.cod_medio = codMedioMutual(name);
	ev_promo.cod_submedio = codSubMedio(name);
	//ev_promo.cod_submedio = atol(&name[1]);

	GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo,
	sizeof(struct eventos_promociones));

}

void recTicket( long promoId, char* promoIdExt, identifier name, long grupo )
{
	struct eventos_promociones ev_promo;
	memset( &ev_promo, 0, sizeof(struct eventos_promociones) );
	promoIdMotor =promoId;

	{
		char tmp[256];
	    sprintf( tmp, "PROMO: rec_Ticket(promo=%li, sub_medio=%s, grupo=%li)\n", promoId, name, grupo );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}
	
	ev_promo.b_promo = BENEF_MEDIO;
	ev_promo.cod_promo = promoId;
	strncpy(ev_promo.cod_promo_ext, promoIdExt, sizeof(ev_promo.cod_promo_ext)-1);
	ev_promo.id_grupo = grupo;
	ev_promo.cod_medio = codMedioTicket(name);
	ev_promo.cod_submedio = codSubMedio(name);
	//ev_promo.cod_submedio = atol(&name[1]);

	GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo,
	sizeof(struct eventos_promociones));

}

void recCheque( long promoId, char* promoIdExt, identifier name, long grupo )
{
	struct eventos_promociones ev_promo;
	memset( &ev_promo, 0, sizeof(struct eventos_promociones) );
	promoIdMotor =promoId;

	{
		char tmp[256];
	    sprintf( tmp, "PROMO: rec_Cheque(promo=%li, sub_medio=%s, grupo=%li)\n", promoId, name, grupo );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}
	
	ev_promo.b_promo = BENEF_MEDIO;
	ev_promo.cod_promo = promoId;
	strncpy(ev_promo.cod_promo_ext, promoIdExt, sizeof(ev_promo.cod_promo_ext)-1);
	ev_promo.id_grupo = grupo;
	ev_promo.cod_medio = codMedioCheque(name);
	ev_promo.cod_submedio = codSubMedio(name);
	//ev_promo.cod_submedio = atol(&name[1]);

	GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo,
	sizeof(struct eventos_promociones));

}

void recCurrency( long promoId, char* promoIdExt, identifier name, long grupo )
{
	struct eventos_promociones ev_promo;
	memset( &ev_promo, 0, sizeof(struct eventos_promociones) );
	promoIdMotor =promoId;

	{
		char tmp[256];
	    sprintf( tmp, "PROMO: rec_Currency(promo=%li, sub_medio=%s, grupo=%li)\n", promoId, name, grupo );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}
	
	ev_promo.b_promo = BENEF_MEDIO;
	ev_promo.cod_promo = promoId;
	strncpy(ev_promo.cod_promo_ext, promoIdExt, sizeof(ev_promo.cod_promo_ext)-1);
	ev_promo.id_grupo = grupo;
	ev_promo.cod_medio = codMedioCurrency(name);
	ev_promo.cod_submedio = codSubMedio(name);
	//ev_promo.cod_submedio = atol(&name[1]);

	GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo,
	sizeof(struct eventos_promociones));

}

void recCard( long promoId, char* promoIdExt, identifier name, identifier plan, long grupo )
{
	struct eventos_promociones ev_promo;
	memset( &ev_promo, 0, sizeof(struct eventos_promociones) );
	promoIdMotor =promoId;

	{
		char tmp[256];
	    sprintf( tmp, "PROMO: rec_Card(promo=%li, sub_medio=%s, plan=%s, grupo=%li)\n", promoId, name, plan, grupo );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}
	
	ev_promo.b_promo = BENEF_MEDIO;
	ev_promo.cod_promo = promoId;
	strncpy(ev_promo.cod_promo_ext, promoIdExt, sizeof(ev_promo.cod_promo_ext)-1);
	ev_promo.id_grupo = grupo;
	ev_promo.cod_medio = codMedioCard(name);
	ev_promo.cod_submedio = codSubMedio(name);
	//ev_promo.cod_submedio = atol(&name[1]);
	ev_promo.modo_plan_pago = atol(&plan[1]);

	GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo,
	sizeof(struct eventos_promociones));

}

void recAll( long promoId, char* promoIdExt, long grupo )
{
	struct eventos_promociones ev_promo;
	memset( &ev_promo, 0, sizeof(struct eventos_promociones) );

	{
		char tmp[256];
	    sprintf( tmp, "PROMO: rec_All(promo=%li, grupo=%li)\n", promoId, grupo );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}
	
	ev_promo.b_promo = BENEF_INCONDICIONAL;
	ev_promo.cod_promo = promoId;
	strncpy(ev_promo.cod_promo_ext, promoIdExt, sizeof(ev_promo.cod_promo_ext)-1);
	ev_promo.id_grupo = grupo;

	GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo,
	sizeof(struct eventos_promociones));
}

void recPerfil( long promoId, char* promoIdExt, long perfil, long grupo )
{
	struct eventos_promociones ev_promo;
	memset( &ev_promo, 0, sizeof(struct eventos_promociones) );

	{
		char tmp[256];
	    sprintf( tmp, "PROMO: rec_Perfil(promo=%li, grupo=%li)\n", promoId, grupo );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}
	
	ev_promo.b_promo = BENEF_PERFIL;
	ev_promo.cod_promo = promoId;
	strncpy(ev_promo.cod_promo_ext, promoIdExt, sizeof(ev_promo.cod_promo_ext)-1);
	ev_promo.id_grupo = grupo;

	GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo,
	sizeof(struct eventos_promociones));
}

void recCredit( long promoId, char* promoIdExt, double cant, long grupo )
{
	struct eventos_promociones ev_promo;
	memset( &ev_promo, 0, sizeof(struct eventos_promociones) );
	promoIdMotor =promoId;

	{
		char tmp[256];
	    sprintf( tmp, "PROMO: rec_Credit(promo=%li, cantidad=%lf, grupo=%li)\n", promoId, cant, grupo );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}
	
	ev_promo.b_promo = BENEF_DESCUENTO;
	ev_promo.cod_promo = promoId;
	strncpy(ev_promo.cod_promo_ext, promoIdExt, sizeof(ev_promo.cod_promo_ext)-1);
	ev_promo.id_grupo = grupo;
	ev_promo.precio_fijo = cant;

	GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo,
	sizeof(struct eventos_promociones));
}

void recDistribute( long promoId, char* promoIdExt, double cant, long grupo )
{
	struct eventos_promociones ev_promo;
	memset( &ev_promo, 0, sizeof(struct eventos_promociones) );

	{
		char tmp[256];
	    sprintf( tmp, "PROMO: rec_Credit(promo=%li, cantidad=%lf, grupo=%li)\n", promoId, cant, grupo );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}
	
	ev_promo.b_promo = BENEF_REGALO;
	ev_promo.cod_promo = promoId;
	strncpy(ev_promo.cod_promo_ext, promoIdExt, sizeof(ev_promo.cod_promo_ext)-1);
	ev_promo.id_grupo = grupo;
	ev_promo.precio_fijo = cant;

	GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo,
	sizeof(struct eventos_promociones));
}

void recBonus( long promoId, char* promoIdExt, identifier type, double value, double cant, long grupo )
{
	struct eventos_promociones ev_promo;
	memset( &ev_promo, 0, sizeof(struct eventos_promociones) );

	{
		char tmp[256];
	    sprintf( tmp, "PROMO: rec_Bonus(promo=%li, tipo=%s, valor=%lf, cantidad=%lf, grupo=%li)\n", promoId,
             type, value, cant, grupo );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}
	
	ev_promo.b_promo = BENEF_BONOS;
	ev_promo.cod_promo = promoId;
	strncpy(ev_promo.cod_promo_ext, promoIdExt, sizeof(ev_promo.cod_promo_ext)-1);
	ev_promo.id_grupo = grupo;
	ev_promo.nro_validacion = atoi(&type[1]);
	ev_promo.precio_fijo = value;
	ev_promo.cantidad = cant;
	switch (type[0]) {
	case 'c':
		// Es un bono que NO puede utilizarse como medio de pago.
		// Puede ser canjeado por algún artículo.
		ev_promo.tipo_bono = BONO_CANJEABLE;
		break;
	case 't':
	default:
		// Es un bono que puede utilizarse como medio de pago.
		ev_promo.tipo_bono = BONO_HAB_PAGO;
		break;
	}

	GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo,
	sizeof(struct eventos_promociones));
}

void recPoints( long promoId, char* promoIdExt, identifier point, double cant, long grupo )
{
	struct eventos_promociones ev_promo;
	memset( &ev_promo, 0, sizeof(struct eventos_promociones) );

	{
		char tmp[256];
		sprintf( tmp, "PROMO: rec_Points(promo=%li, punto=%s, cantidad=%lf, grupo=%li)\n", promoId, point,
             cant, grupo );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}

	if (strcmp(point, "money") == 0)
	{
		ev_promo.b_promo = BENEF_MONEDERO;
		ev_promo.cod_promo = promoId;
		strncpy(ev_promo.cod_promo_ext, promoIdExt, sizeof(ev_promo.cod_promo_ext)-1);
		ev_promo.id_grupo = grupo;
		ev_promo.precio_fijo= cant;

		GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo,
		sizeof(struct eventos_promociones));
	}
}

void recActivateCard( long promoId, char* promoIdExt, identifier name, identifier plan, double monto, long grupo )
{
	struct eventos_promociones ev_promo;
	memset( &ev_promo, 0, sizeof(struct eventos_promociones) );

	{
		char tmp[256];
	    sprintf( tmp, "PROMO: rec_ActivateCard(promo=%li, card_Name=%s, plan=%s, monto=%lf, grupo=%li)\n",
             promoId, name, plan, monto, grupo );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}
	
	ev_promo.b_promo = BENEF_ACTIVACION_MEDIO;
	ev_promo.cod_promo = promoId;
	strncpy(ev_promo.cod_promo_ext, promoIdExt, sizeof(ev_promo.cod_promo_ext)-1);
	ev_promo.id_grupo = grupo;
	//ev_promo.cod_medio = MEDIO_TARJETAS;  // CUIDADO: Se debe implementar la busqueda del código de medio correspondiente a tarjetas de forma dinámica
	ev_promo.cod_medio = codMedioCard(name);

	ev_promo.cod_submedio = codSubMedio(name);
	//ev_promo.cod_submedio = atol(&name[1]);
	ev_promo.modo_plan_pago = atol(&plan[1]);
	ev_promo.precio_fijo = monto;

	GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo,
	sizeof(struct eventos_promociones));
}

void recActivateTicket( long promoId, char* promoIdExt, identifier name, double monto, long grupo )
{
	struct eventos_promociones ev_promo;
	assert( name );

	memset( &ev_promo, 0, sizeof(struct eventos_promociones) );

	{
		char tmp[256];
	    sprintf( tmp, "PROMO: rec_ActivateTicket(promo=%li, card_Name=%s, monto=%lf, grupo=%li)\n",
             promoId, name, monto, grupo );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}
	
	ev_promo.b_promo = BENEF_ACTIVACION_MEDIO;
	ev_promo.cod_promo = promoId;
	strncpy(ev_promo.cod_promo_ext, promoIdExt, sizeof(ev_promo.cod_promo_ext)-1);
	ev_promo.id_grupo = grupo;
	if ( name[0] == 'm' ) {
		// name es de la forma mXXXsYYY con XXX código de medio e YYY código de submedio.
		char *str;
		char seps[]   = "ms";
		char *token;

		str = (char*)malloc(strlen(name)+1);
		strncpy(str,name,strlen(name)+1);

		token = strtok( str, seps );
		if ( token ) {
			ev_promo.cod_medio = atol(token);
			if ( ev_promo.cod_medio < 0 || ev_promo.cod_medio > 9 ) {
				// en caso de que el medio este fuera del rango valido
				ev_promo.cod_medio = codMedioTicket(name);
			}
			token = strtok( NULL, seps );
			if ( token ) {
				ev_promo.cod_submedio = atol(token);
			}
		}
		if(str )
		free( str );		
	} else {
		// en name se codifica sólo el código de submedio
		ev_promo.cod_medio = codMedioTicket(name);
		ev_promo.cod_submedio = codSubMedio(name);
		//ev_promo.cod_submedio = atol(&name[1]);
	}

	ev_promo.precio_fijo = monto;

	GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo,
	sizeof(struct eventos_promociones));
}

void recActivateMutual( long promoId, char* promoIdExt, identifier name, double monto, long grupo )
{
	struct eventos_promociones ev_promo;
	assert( name );

	memset( &ev_promo, 0, sizeof(struct eventos_promociones) );

	{
		char tmp[256];
	    sprintf( tmp, "PROMO: rec_ActivateMutual(promo=%li, card_Name=%s, monto=%lf, grupo=%li)\n",
             promoId, name, monto, grupo );
		glog(tmp,LOG_MOTOR_PROMO,4);
	}
	
	ev_promo.b_promo = BENEF_ACTIVACION_MEDIO;
	ev_promo.cod_promo = promoId;
	strncpy(ev_promo.cod_promo_ext, promoIdExt, sizeof(ev_promo.cod_promo_ext)-1);
	ev_promo.id_grupo = grupo;
	if ( name[0] == 'm' ) {
		// name es de la forma mXXXsYYY con XXX código de medio e YYY código de submedio.
		char *str;
		char seps[]   = "ms";
		char *token;

		str = (char*)malloc(strlen(name)+1);
		strncpy(str,name,strlen(name)+1);

		token = strtok( str, seps );
		if ( token ) {
			ev_promo.cod_medio = atol(token);
			if ( ev_promo.cod_medio < 0 || ev_promo.cod_medio > 9 ) {
				// en caso de que el medio este fuera del rango valido
				ev_promo.cod_medio = codMedioMutual(name);
			}
			token = strtok( NULL, seps );
			if ( token ) {
				ev_promo.cod_submedio = atol(token);
			}
		}
		if(str )
		free( str );		
	} else {
		// en name se codifica sólo el código de submedio
		ev_promo.cod_medio = codMedioMutual(name);
		ev_promo.cod_submedio = codSubMedio(name);
		//ev_promo.cod_submedio = atol(&name[1]);
	}

	ev_promo.precio_fijo = monto;

	GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo,
	sizeof(struct eventos_promociones));
}


void recLog( identifier target, char *leyenda) {
	if ( strcmp(target, "inticket") == 0) {
		struct asig_beneficio asig_benef;
		memset( &asig_benef, 0, sizeof(struct asig_beneficio) );

		glog( "PROMO: Imprimir LOG dentro del Ticket" ,LOG_MOTOR_PROMO,4);

		asig_benef.tipo = ASIG_BENEF_STR_TICKET;
		asig_benef.lugar_impresion = DENTRO_DEL_TICKET;
		strncpy( asig_benef.msg_string, leyenda, 256);

		GUARDAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO, (char *)&asig_benef,
		sizeof(struct asig_beneficio));
	
	} 
	else if ( strcmp(target, "outsideticket") == 0) {
		struct asig_beneficio asig_benef;
		memset( &asig_benef, 0, sizeof(struct asig_beneficio) );

		glog( "PROMO: Imprimir LOG fuera del Ticket" ,LOG_MOTOR_PROMO,4);

		asig_benef.tipo = ASIG_BENEF_STR_TICKET;
		asig_benef.lugar_impresion = FUERA_DEL_TICKET;
		strncpy( asig_benef.msg_string, leyenda, 256);

		GUARDAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO, (char *)&asig_benef,
		sizeof(struct asig_beneficio));
	
	} 
	else if ( strcmp(target, "header") == 0) {
		struct asig_beneficio asig_benef;
		memset( &asig_benef, 0, sizeof(struct asig_beneficio) );

		glog( "PROMO: Imprimir LOG fuera del Ticket" ,LOG_MOTOR_PROMO,4);

		asig_benef.tipo = ASIG_BENEF_STR_TICKET;
		asig_benef.lugar_impresion = ENCABEZADO_DEL_TICKET;
		strncpy( asig_benef.msg_string, leyenda, 256);

		GUARDAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO, (char *)&asig_benef,
		sizeof(struct asig_beneficio));
	
	} 
	else if ( strcmp(target, "body") == 0) {
		struct asig_beneficio asig_benef;
		memset( &asig_benef, 0, sizeof(struct asig_beneficio) );

		glog( "PROMO: Imprimir LOG fuera del Ticket" ,LOG_MOTOR_PROMO,4);

		asig_benef.tipo = ASIG_BENEF_STR_TICKET;
		asig_benef.lugar_impresion = CUERPO_DEL_TICKET;
		strncpy( asig_benef.msg_string, leyenda, 256);

		GUARDAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO, (char *)&asig_benef,
		sizeof(struct asig_beneficio));
	
	} 
	else if ( strcmp(target, "total") == 0) {
		struct asig_beneficio asig_benef;
		memset( &asig_benef, 0, sizeof(struct asig_beneficio) );

		glog( "PROMO: Imprimir LOG fuera del Ticket" ,LOG_MOTOR_PROMO,4);

		asig_benef.tipo = ASIG_BENEF_STR_TICKET;
		asig_benef.lugar_impresion = TOTAL_DEL_TICKET;
		strncpy( asig_benef.msg_string, leyenda, 256);

		GUARDAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO, (char *)&asig_benef,
		sizeof(struct asig_beneficio));
	
	} 
	else if ( strcmp(target, "footer") == 0) {
		struct asig_beneficio asig_benef;
		memset( &asig_benef, 0, sizeof(struct asig_beneficio) );

		glog( "PROMO: Imprimir LOG fuera del Ticket" ,LOG_MOTOR_PROMO,4);

		asig_benef.tipo = ASIG_BENEF_STR_TICKET;
		asig_benef.lugar_impresion = PIE_DEL_TICKET;
		strncpy( asig_benef.msg_string, leyenda, 256);

		GUARDAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO, (char *)&asig_benef,
		sizeof(struct asig_beneficio));
	
	} 
	else if ( strcmp(target, "show") == 0) {
        MENSAJE( leyenda );
	} 
	else if ( strcmp(target, "file") == 0) {
		char tmp[512];
		memset(tmp,0,sizeof(tmp));
		_snprintf(tmp, 512,"PROMO: LOG: %s", leyenda);
		glog(tmp,LOG_MOTOR_PROMO,4);
	} 
	else {
		glog("PROMO: LOG: ERROR, destino del LOG inválido",LOG_MOTOR_PROMO,3);
	}

}

int inDepartment( identifier art, identifier dep )
{
    long depart;
	long codInt = 0;
	char codBar[17] = "";
	double valor = 0;
	long codPresentacion = 0;
	double r;

    //glog( "PROMO: idDepartment" );


	
	switch ( getCodsArt( art, &codInt, codBar, &valor, &codPresentacion) ) {
	case COD_INTERNO:
	case COD_INTERNO_PRECIO:
		if ( dep[0] == 'd') {
			depart = atol( &dep[1] );
			
			r = get_dpto_cache ( codInt, 0 );
			if (r >= 0) {
				// Valor almacenado en Cache
		        //{ char buff[256];sprintf( buff, "PROMO: INDEPARTMENT_OBTENIDO_DE_CACHE(%s,%s) -> %li", art, dep, r );glog( buff ); }
				return r == depart;
			} else {
				// Valor obtenido de BBDD
				if ( !ASIGNA_CODIGO_ESPECIAL( codInt ) ) {
					/**/glog( "PROMO: inDepartment: No se encontró el artículo.",LOG_MOTOR_PROMO,3 );
				} else {
					return ITEM_DEPTO == depart;
				}
			}

		} else if ( dep[0] == 'p') {
			depart = atol( &dep[1] );
			
			//r = get_dpto_cache ( codInt, 0 );
			// se deberá buscar el codigo de proveedor en el cache.
			r = -1;

			if (r >= 0) {
				// Valor almacenado en Cache
		        //{ char buff[256];sprintf( buff, "PROMO: INDEPARTMENT_OBTENIDO_DE_CACHE(%s,%s) -> %li", art, dep, r );glog( buff ); }
				return r == depart;
			} else {
				// Valor obtenido de BBDD
				if ( !BUSCA_CODIGO_ESPECIAL( codInt ) ) {
					/**/glog( "PROMO: inDepartment: No se encontró el artículo." ,LOG_MOTOR_PROMO,3);
				} else {
					return articulo.cod_prov == depart;
				}
			}
		} else {
			if( strcmp( dep, "nopromo" ) == 0 ) {
				r = get_promocionable_cache ( codInt, 0 );
				if (r >= 0) {
					// Valor almacenado en Cache
					//{ char buff[256];sprintf( buff, "PROMO: INDEPARTMENT_OBTENIDO_DE_CACHE(%s,%s) -> %li", art, dep, r );glog( buff ); }
					return !r;
				} else {
					// Valor obtenido de BBDD
					if ( !ASIGNA_CODIGO_ESPECIAL( codInt ) ) {
						/**/glog( "PROMO: inDepartment: No se encontró el artículo." ,LOG_MOTOR_PROMO,3);
					} else {
						return !ITEM_HABILITADO_EN_PROMOCION;
					}
				}
			} else if( strcmp( dep, "specialCreditPrice" ) == 0 ) {
				if ( !ASIGNA_CODIGO_ESPECIAL( codInt ) ) {
					/**/glog( "PROMO: inDepartment: No se encontró el artículo." ,LOG_MOTOR_PROMO,3);
				} else {
					return ( articulo.cr_habilitado == 'S' && ( RAM_USAR_PRECIO_CR || USAR_PRECIO_CREDITO ) ) || articulo.habilitado_oferta == 'S';
				}
			} else {
				/**/glog( "PROMO: inDepartment: No se encontró el departamento." ,LOG_MOTOR_PROMO,3);
				return 0;
			}
		}
		break;
	case COD_BARRA:
	case COD_BARRA_PRECIO:
		if ( dep[0] == 'd') {
			depart = atol( &dep[1] );
			
			r = get_dpto_cache ( 0, _atoi64( codBar ) );
			if (r >= 0) {
				// Valor almacenado en Cache
		        //{ char buff[256];sprintf( buff, "PROMO: INDEPARTMENT_OBTENIDO_DE_CACHE(%s,%s) -> %li", art, dep, r );glog( buff ); }
				return r == depart;
			} else {
				// Valor obtenido de BBDD
				if ( !ASIGNA_CODIGO_BARRA( codBar ) ) {
					/**/glog( "PROMO: inDepartment: No se encontró el artículo." ,LOG_MOTOR_PROMO,3);
				} else {
					return ITEM_DEPTO == depart;
				}
			}
		} else if ( dep[0] == 'p') {
			depart = atol( &dep[1] );
			
			//r = get_dpto_cache ( 0, _atoi64( codBar ) );
			// se deberá buscar el codigo de proveedor en el cache.
			r = -1;

			if (r >= 0) {
				// Valor almacenado en Cache
		        //{ char buff[256];sprintf( buff, "PROMO: INDEPARTMENT_OBTENIDO_DE_CACHE(%s,%s) -> %li", art, dep, r );glog( buff ); }
				return r == depart;
			} else {
				// Valor obtenido de BBDD
				if ( !BUSCA_CODIGO_BARRA( codBar ) ) {
					/**/glog( "PROMO: inDepartment: No se encontró el artículo." ,LOG_MOTOR_PROMO,3);
				} else {
					return articulo.cod_prov == depart;
				}
			}

		} else {
			if( strcmp( dep, "nopromo" ) == 0 ) {
				r = get_promocionable_cache ( 0, _atoi64( codBar ) );
				if (r >= 0) {
					// Valor almacenado en Cache
					//{ char buff[256];sprintf( buff, "PROMO: INDEPARTMENT_OBTENIDO_DE_CACHE(%s,%s) -> %li", art, dep, r );glog( buff ); }
					return !r;
				} else {
					// Valor obtenido de BBDD
					if ( !ASIGNA_CODIGO_BARRA( codBar ) ) {
						/**/glog( "PROMO: inDepartment: No se encontró el artículo." ,LOG_MOTOR_PROMO,3);
					} else {
						return !ITEM_HABILITADO_EN_PROMOCION;
					}
				}
			} else if( strcmp( dep, "specialCreditPrice" ) == 0 ) {
				if ( !ASIGNA_CODIGO_BARRA( codBar ) ) {
					/**/glog( "PROMO: inDepartment: No se encontró el artículo." ,LOG_MOTOR_PROMO,3);
				} else {
					return ( articulo.cr_habilitado == 'S' && ( RAM_USAR_PRECIO_CR || USAR_PRECIO_CREDITO ) ) || articulo.habilitado_oferta == 'S';
				}
			} else {
				/**/glog( "PROMO: inDepartment: No se encontró el departamento." ,LOG_MOTOR_PROMO,3);
				return 0;
			}
		}		
		break;
	case COD_INTERNO_BARRA:
	case COD_INTERNO_BARRA_PRECIO:
	case COD_INTERNO_BARRA_PRESENTACION:
	case COD_INTERNO_BARRA_PRESENTACION_PRECIO:

		if ( dep[0] == 'd') {
			depart = atol( &dep[1] );
			
			r = get_dpto_cache ( codInt, _atoi64( codBar ) );
			if (r >= 0) {
				// Valor almacenado en Cache
		        //{ char buff[256];sprintf( buff, "PROMO: INDEPARTMENT_OBTENIDO_DE_CACHE(%s,%s) -> %li", art, dep, r );glog( buff ); }
				return r == depart;
			} else {
				// Valor obtenido de BBDD
				if ( !ASIGNA_COD_ESPECIAL_COD_BARRA( codInt, codBar ) ) {
					/**/glog( "PROMO: inDepartment: No se encontró el artículo." ,LOG_MOTOR_PROMO,3);
				} else {
					return ITEM_DEPTO == depart;
				}
			}

		} else if ( dep[0] == 'p') {
			depart = atol( &dep[1] );
			
			//r = get_dpto_cache ( codInt, _atoi64( codBar ) );
			// se deberá buscar el codigo de proveedor en el cache.
			r = -1;

			if (r >= 0) {
				// Valor almacenado en Cache
		        //{ char buff[256];sprintf( buff, "PROMO: INDEPARTMENT_OBTENIDO_DE_CACHE(%s,%s) -> %li", art, dep, r );glog( buff ); }
				return r == depart;
			} else {
				// Valor obtenido de BBDD
				if ( !BUSCA_COD_INTERNO_COD_BARRA( codInt, codBar ) ) {
					/**/glog( "PROMO: inDepartment: No se encontró el artículo." ,LOG_MOTOR_PROMO,3);
				} else {
					return articulo.cod_prov == depart;
					//return articulo.cod_dpto_old == depart;
				}
			}

		} else {
			if( strcmp( dep, "nopromo" ) == 0 ) {
				r = get_promocionable_cache ( codInt, _atoi64( codBar ) );
				if (r >= 0) {
					// Valor almacenado en Cache
					//{ char buff[256];sprintf( buff, "PROMO: INDEPARTMENT_OBTENIDO_DE_CACHE(%s,%s) -> %li", art, dep, r );glog( buff ); }
					return !r;
				} else {
					// Valor obtenido de BBDD
					if ( !ASIGNA_COD_ESPECIAL_COD_BARRA( codInt, codBar ) ) {
						/**/glog( "PROMO: inDepartment: No se encontró el artículo." ,LOG_MOTOR_PROMO,3);
					} else {
						return !ITEM_HABILITADO_EN_PROMOCION;
					}
				}
			} else if( strcmp( dep, "specialCreditPrice" ) == 0 ) {
				if ( !BUSCA_COD_INTERNO_COD_BARRA( codInt, codBar ) ) {
					/**/glog( "PROMO: inDepartment: No se encontró el artículo." ,LOG_MOTOR_PROMO,3);
				} else {
					return ( articulo.cr_habilitado == 'S' && ( RAM_USAR_PRECIO_CR || USAR_PRECIO_CREDITO ) ) || articulo.habilitado_oferta == 'S';
				}
			} else {
				/**/glog( "PROMO: inDepartment: No se encontró el departamento." ,LOG_MOTOR_PROMO,3);
				return 0;
			}
		}

		break;
	case COD_ERROR:
		/**/glog( "PROMO: inDepartment: El tipo de artículo no está soportado o presenta un error." ,LOG_MOTOR_PROMO,3);
		return 0;
		break;
	default:
		/**/glog( "PROMO: inDepartment: Tipo de artículo desconocido." ,LOG_MOTOR_PROMO,3);
		return 0;
		break;

	}

	return 0;



/* ANTIGUA IMPLEMENTACION PARA SOLO ARTICULOS POR COD_INTERNO
	if ( dep[0] == 'd') {
		depart = atoi( &dep[1] );


		if( art[0] == 'i' ) {
			double r;
			
			r = get_dpto_cache ( atol(&art[1]), 0 );
			if (r >= 0) {
				// Valor almacenado en Cache
		        //{ char buff[256];sprintf( buff, "PROMO: INDEPARTMENT_OBTENIDO_DE_CACHE(%s,%s) -> %li", art, dep, r );glog( buff ); }
				return r == depart;
			} else {
				// Valor obtenido de BBDD
				ASIGNA_CODIGO_ESPECIAL( atol( &art[1] ) );
				return ITEM_DEPTO == depart;
			}
		}
		else if( art[0] == 'x' ) {
			double r;
			
			// Parsear el article para recuperar el codint despues de la 'x' y el codbarra despues del '_'
			r = get_dpto_cache ( atol(&art[1]), _atoi64(&art[1]) );
			if (r >= 0) {
				// Valor almacenado en Cache
				return r == depart;
			} else {
				// Valor obtenido de BBDD
				ASIGNA_COD_ESPECIAL_COD_BARRA( atol( &art[1] ), &art[1] );
				return ITEM_DEPTO == depart;
			}
		}
		else if( art[0] == 'b' ) {
			double r;
			
			r = get_dpto_cache ( 0, _atoi64(&art[1]) );
			if (r >= 0) {
				// Valor almacenado en Cache
				return r == depart;
			} else {
				// Valor obtenido de BBDD
				ASIGNA_CODIGO_BARRA( &art[1] );
				return ITEM_DEPTO == depart;
			}
		}
		else if( art[0] == 'j' ) {
				char id[256];
				int n=1;
				while ((art[n] != '\0') && (art[n] != '$') && (n<256)) {
					id[n-1] = art[n];
					n++;
				}
				id[n-1]='\0'; 

				{
					double r;
					
					r = get_dpto_cache ( atol( id ), 0 );
					if (r >= 0) {
						// Valor almacenado en Cache
						return r == depart;
					} else {
						// Valor obtenido de BBDD
						ASIGNA_CODIGO_ESPECIAL( atol( id ) );
						return ITEM_DEPTO == depart;
					}
				}
				//ASIGNA_CODIGO_ESPECIAL( atol( id ) );
		} else {
			return 0;
		}
			

	} else {
		if( strcmp( dep, "nopromo" ) == 0 ) {
			//return !ITEM_HABILITADO_EN_PROMOCION;

			if( art[0] == 'i' ) {
				int r;
				
				r = get_promocionable_cache ( atol(&art[1]), 0 );
				if (r >= 0) {
					// Valor almacenado en Cache
					//{ char buff[256];sprintf( buff, "PROMO: INDEPARTMENT_OBTENIDO_DE_CACHE(%s,%s) -> %li", art, dep, r );glog( buff ); }
					return !r;
				} else {
					// Valor obtenido de BBDD
					ASIGNA_CODIGO_ESPECIAL( atol( &art[1] ) );
					return !ITEM_HABILITADO_EN_PROMOCION;
				}
			}
			else if( art[0] == 'x' ) {
				int r;
				
				// Parsear el article para recuperar el codint despues de la 'x' y el codbarra despues del '_'
				r = get_promocionable_cache ( atol(&art[1]), _atoi64(&art[1]) );
				if (r >= 0) {
					// Valor almacenado en Cache
					return r;
				} else {
					// Valor obtenido de BBDD
					ASIGNA_COD_ESPECIAL_COD_BARRA( atol( &art[1] ), &art[1] );
					return !ITEM_HABILITADO_EN_PROMOCION;
				}
			}
			else if( art[0] == 'b' ) {
				int r;
				
				r = get_promocionable_cache ( 0, _atoi64(&art[1]) );
				if (r >= 0) {
					// Valor almacenado en Cache
					return r;
				} else {
					// Valor obtenido de BBDD
					ASIGNA_CODIGO_BARRA( &art[1] );
					return !ITEM_HABILITADO_EN_PROMOCION;
				}
			}
			else if( art[0] == 'j' ) {
					char id[256];
					int n=1;
					while ((art[n] != '\0') && (art[n] != '$') && (n<256)) {
						id[n-1] = art[n];
						n++;
					}
					id[n-1]='\0'; 

					{
						int r;
						
						r = get_promocionable_cache ( atol( id ), 0 );
						if (r >= 0) {
							// Valor almacenado en Cache
							return r;
						} else {
							// Valor obtenido de BBDD
							ASIGNA_CODIGO_ESPECIAL( atol( id ) );
							return !ITEM_HABILITADO_EN_PROMOCION;
						}
					}
					//ASIGNA_CODIGO_ESPECIAL( atol( id ) );
			} else {
				return 0;
			}
				
			//return 0;
		} else {
			return 0;
		}

	}
*/
}


int inClass( identifier article, identifier nivel, identifier concept )
{
	int rta = 0;

    if( article[0] == 'b' ) {
        ASIGNA_CODIGO_BARRA( &article[1] );
    }
    else {
        ASIGNA_CODIGO_ESPECIAL( atol( &article[1] ) );
    }

	{ 
		int area_ant = 0;
		char cod_clas;
		struct _clasif1 clasif1;
		int LARGO_COD_CLASIF_EXT = 0;

        {
			/*
			 * Este bloque es sólo para calcular el tamaño del miembro 'cod_clasificacion' de la 
			 * estructura 'struct _articulo'.
			 * Falta seguir estudiando la existencia de alguna sintaxis para hacer esto en
			 * tiempo de compilacion.
			 */
			struct _articulo art;
            LARGO_COD_CLASIF_EXT = sizeof art.cod_clasificacion;
		}

		if ( nivel && strlen( nivel ) > 1 ) {
			cod_clas = (char)atoi(&nivel[1]);
		} else {
			return 0;
		}

		area_ant = SELECTED(  );

		
        //if( USE_DB( AREA_CLASIF1, _CLASIF1_SIC, ( char* )&clasif1, sizeof( clasif1 ), NULL, 0, 0 ) == 0 ) {
			//SELECT( AREA_CLASIF1 );
         if( OPEN_TABLE(T_CLASIF1, TT_ORIG,( char* )&clasif1, sizeof( struct _clasif1 )) == 0 ) {
        	 int cod_clas_i = (int) cod_clas  ;

        	 SELECT_TABLE( T_CLASIF1, TT_ORIG );

			//SET_ORDER( 1 );
        	SET_WHERE("CLA1_COD_CLASIF1 = '%d'", cod_clas_i );
        	SET_ORDER2( "CLA1_COD_CLASIF1");
        	RUN_QUERY(NO);


            //GET_EQUAL( ( char* )&cod_clas );

            if( FOUND2() ) {
				int largo_mascara = max( min( clasif1.digito_fin, LARGO_COD_CLASIF_EXT ) - clasif1.digito_ini + 1, 0 );
				
				if ( concept && strlen( concept ) > 1 ) {
					rta = ! strncmp( &concept[1], &ITEM_COD_CLASIFICACION_STR[min( clasif1.digito_ini, LARGO_COD_CLASIF_EXT) - 1], largo_mascara );
				}
			}
			CLOSE_DATABASE( AREA_CLASIF1 );
        }
		SELECT( area_ant );
	}

	return rta;
}

/*
 * Cuidado, no se debe averiguar el precio del articulo mirando el
 * ticket, porque se puede preguntar el precio de un articulo que
 * no haya sido tickeado.
 */
double amount( identifier article, double n )
{
	long codInt = 0;
	char codBar[17] = "";
	long codPresentacion = 0;
	double precio = 0;
	double r;
	
	switch ( getCodsArt( article, &codInt, codBar, &precio, &codPresentacion) ) {
	case COD_INTERNO:
		r = get_cost_cache ( codInt, 0 );

		if (r >= 0) {
			// Valor almacenado en Cache
		    //{ char buff[256];sprintf( buff, "PROMO: AMOUNT_OBTENIDO_DE_CACHE(%s,%lf) = %lf", article, n, r*n );glog( buff ); }

			return r * n;
		} else {
			// Valor obtenido de BBDD
			//if ( !ASIGNA_COD_ESPECIAL_COD_BARRA( codInt, codBar ) ) {
			if ( !BUSCA_COD_INTERNO_COD_BARRA( codInt, codBar ) ) {
				/**/glog( "PROMO: Amount: No se encontró el artículo. Se devuelve precio 0." ,LOG_MOTOR_PROMO,3);
				return 0;
			} else {
				return articulo.precio * n;
				//return ITEM_PRECIO * n;
			}
		}
		break;

	case COD_INTERNO_PRECIO:
		return precio * n;
		break;

	case COD_BARRA:
		r = get_cost_cache ( 0, _atoi64( codBar ) );

		if (r >= 0) {
			// Valor almacenado en Cache
		    //{ char buff[256];sprintf( buff, "PROMO: AMOUNT_OBTENIDO_DE_CACHE(%s,%lf) = %lf", article, n, r*n );glog( buff ); }

			return r * n;
		} else {
			// Valor obtenido de BBDD
			if ( !ASIGNA_CODIGO_BARRA( codBar ) ) {
				/**/glog( "PROMO: Amount: No se encontró el artículo. Se devuelve precio 0." ,LOG_MOTOR_PROMO,3);
				return 0;
			} else {
				return ITEM_PRECIO * n;
			}
		}
		break;

	case COD_BARRA_PRECIO:
		return precio * n;
		break;

	case COD_INTERNO_BARRA:
		r = get_cost_cache ( codInt, _atoi64( codBar) );

		if (r >= 0) {
			// Valor almacenado en Cache
		    //{ char buff[256];sprintf( buff, "PROMO: AMOUNT_OBTENIDO_DE_CACHE(%s,%lf) = %lf", article, n, r*n );glog( buff ); }

			return r * n;
		} else {
			// Valor obtenido de BBDD
			double rta = 0;
			if ( !ASIGNA_COD_ESPECIAL_COD_BARRA( codInt, codBar ) ) {
			//if ( !BUSCA_COD_INTERNO_COD_BARRA( codInt, codBar ) ) {
				/**/glog( "PROMO: Amount: No se encontró el artículo. Se devuelve precio 0." ,LOG_MOTOR_PROMO,3);
				rta = 0;
			} else {
				//rta = articulo.precio * n;
				rta = ITEM_PRECIO * n;
			}
			CEREAR_STRUCT( _item );
			return rta;
		}
		break;

	case COD_INTERNO_BARRA_PRECIO:
		return precio * n;
		break;

	case COD_INTERNO_BARRA_PRESENTACION:
		// Buscar en ticket
		{
			int h;
			for( h = 0;h < RAM_P_TICKET;h++ ) {
				if ( (_TICKET_COD_ESPECIAL( h ) == codInt) && 
					 (_TICKET_COD_PRESENT( h ) == codPresentacion) &&
					 //(_TICKET_PRESENTACION( h ) == codPresentacion) &&
					 (strcmp(_TICKET_COD_BARRA( h ), codBar) == 0) ) {
						 //return _TICKET_PRECIO( h ) / _TICKET_CANTIDAD( h ) * n;
						 return _TICKET_PRECIO( h ) * n;
				}
			}
			return 0;
		}
		break;

	case COD_INTERNO_BARRA_PRESENTACION_PRECIO:
		return precio * n;
		break;

	case COD_ERROR:
		/**/glog( "PROMO: Amount: No se encontró el artículo. Se devuelve precio 0.",LOG_MOTOR_PROMO,3 );
		return 0;
		break;
	default:
		/**/glog( "PROMO: Amount: No se encontró el artículo. Se devuelve precio 0." ,LOG_MOTOR_PROMO,3);
		return 0;
		break;

	}



	/* ANTIGUA IMPLEMENTACIO PARA SOLO ARTICULOS POR COD_INTERNO
	
	
	char id[256];
    char tok[] = "$";
    char *val;


    //{ char buff[256];sprintf( buff, "PROMO: AMOUNT(%s,%lf)", article, n );glog( buff ); }

    strcpy( id, article );
    val = strtok( id, tok );
    val = strtok( NULL, tok );
    if( val != NULL ) {

        //{ char buff[256];sprintf( buff, "PROMO: AMOUNT_CON_PRECIO(%s,%lf) = %lf", article, n, atof( val ) * n );glog( buff ); }

        return atof( val ) * n;
    }
    else {

		if( article[0] == 'x' ) {
			double r;
			
			// Parsear el article para recuperar el codint despues de la 'x' y el codbarra despues del '_'
			r = get_cost_cache ( atol(&article[1]), _atoi64(&article[1]) );

			if (r >= 0) {
				// Valor almacenado en Cache
				return r * n;
			} else {
				// Valor obtenido de BBDD
				ASIGNA_COD_ESPECIAL_COD_BARRA( atol( &article[1] ), &article[1] );
				return ITEM_PRECIO * n;
			}
		}
		else if( article[0] == 'b' ) {
			double r;
			
			r = get_cost_cache ( 0, _atoi64(&article[1]) );

			if (r >= 0) {
				// Valor almacenado en Cache
				return r * n;
			} else {
				// Valor obtenido de BBDD
				ASIGNA_CODIGO_BARRA( &article[1] );
				return ITEM_PRECIO * n;
			}
		}
		else { //if( article[0] == 'i' )
			double r;
			
			r = get_cost_cache ( atol( &article[1] ), 0 );

			if (r >= 0) {
				// Valor almacenado en Cache
		        //{ char buff[256];sprintf( buff, "PROMO: AMOUNT_OBTENIDO_DE_CACHE(%s,%lf) = %lf", article, n, r*n );glog( buff ); }

				return r * n;
			} else {
				// Valor obtenido de BBDD
				ASIGNA_CODIGO_ESPECIAL( atol( &article[1] ) );
				return ITEM_PRECIO * n;
			}
		}


    }
	*/
}

int affiliated()
{
    long r = 0;
    void *s;

    #ifdef PROMO_FAULT_TOLERANCE
    s = readData( __ram_cliente_afinidad, -1, ( void* )&r, sizeof( long ) );
    #else  //PROMO_FAULT_TOLERANCE
    r = RAM_CLIENTE_AFINIDAD;
    s = 1;
    #endif //PROMO_FAULT_TOLERANCE

    if( s ) {
        glog( "PROMO: affiliated: valor leido correctamente" ,LOG_MOTOR_PROMO,3);
        return ( int )r;
    }
    else {
        glog( "PROMO: affiliated: error leyendo el valor de rnv desde el log",LOG_MOTOR_PROMO,3 );
        return -1;
    }
}



idate dateofbirth()
{
    long r = 0;
    void *s;

    if( affiliated() ) {
        #ifdef PROMO_FAULT_TOLERANCE
        s = readData( __ram_afinidad_fecha_nac, -1, ( void* )&r, sizeof( long ) );
        #else  //PROMO_FAULT_TOLERANCE
        r = RAM_AFINIDAD_FECHA_NAC;
        s = 1;
        #endif //PROMO_FAULT_TOLERANCE

        if( s ) {
            glog( "PROMO: affiliated: valor leido correctamente" ,LOG_MOTOR_PROMO,3);
            return mk_date_from_invel( r );
        }
        else {
            glog( "PROMO: affiliated: error leyendo el valor de rnv desde el log" ,LOG_MOTOR_PROMO,3);
            return get_date();
        }
    }
    else {
        return get_date();
    }
}


int holiday()
{
    return 0;
}

expr means()
{
    expr e;
    int h;
    e = new_means();

    for( h = 0;h < RAM_P_PAGO;h++ ) {
        char ident[256];
        //char* p_nombre;

        memset(ident,0,sizeof(ident));
		#ifdef LOG_MOTOR_PROMO
		{
			char tmp[256];
			memset(tmp,0,sizeof(tmp));

			sprintf(tmp,"PROMO: DEBUG: means(%i) PAGO_MODO = %i IMPORTE = %lf TARJETA = %i", h,
				_PAGO_MODO(h), _PAGO_IMPORTE(h), _PAGO_TARJETA(h)
				
				);
			glog(tmp,LOG_MOTOR_PROMO,4);
		}
		#endif //LOG_DEBUG

        _snprintf( ident, 256, "i%i", ( int )medios[_PAGO_MODO( h )].nro );

        if( medios[_PAGO_MODO( h )].efectivo ) {
            //ins_cash(e, _PAGO_IMPORTE(h) - _PAGO_VUELTO(h));
            ins_cash( e, _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h ) );

			#ifdef LOG_MOTOR_PROMO
			{
				char tmp[256];
				memset(tmp,0,sizeof(tmp));
				sprintf(tmp,"PROMO: DEBUG: means(%i).cash = %lf",h , _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h ) );
				glog(tmp,LOG_DEBUG,4);
			}
			#endif //LOG_MOTOR_PROMO


        }
        else if( medios[_PAGO_MODO( h )].cta_cte ) {
            //ins_ctacte(e, _PAGO_CUENTA(h));
            //ins_ctacte( e, _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h ) );
            ins_ctacte( e, _PAGO_IMPORTE( h ) - _PAGO_VUELTO_MEDIO( h ) );
			#ifdef LOG_MOTOR_PROMO
			{
				char tmp[256];
				sprintf(tmp,"PROMO: DEBUG: means(%i).ctacte = %lf",h , _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h ) );
				glog(tmp,LOG_DEBUG,4);
			}
			#endif //LOG_MOTOR_PROMO
        }
        else if( medios[_PAGO_MODO( h )].cheque ) {
            //_snprintf(ident,256,"i%i",(int)medios[_PAGO_MODO(h)].nro);
            _snprintf(ident,256,"i%i\0",_PAGO_CUENTA(h));
            //_snprintf( ident, 256, "i%i\0", _PAGO_BANCO( h ) );
            //ins_cheque(e, ident, _PAGO_IMPORTE(h));
            //ins_cheque( e, ident, _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h ) );
            ins_cheque( e, ident, _PAGO_IMPORTE( h ) - _PAGO_VUELTO_MEDIO( h ) );
			#ifdef LOG_MOTOR_PROMO
			{
				char tmp[256];
				memset(tmp,0,sizeof(tmp));
				sprintf(tmp,"PROMO: DEBUG: means(%i).cheque = %lf",h , _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h ) );
				glog(tmp,LOG_DEBUG,4);
			}
			#endif //LOG_MOTOR_PROMO
        }
        else if( medios[_PAGO_MODO( h )].tarjeta ) {
            char plan[256];
            memset(plan,0,sizeof(plan));
            //_snprintf( plan, 256, "p%i\0", ( int )_PAGO_CUOTAS( h ) );
            //_snprintf( ident, 256, "i%i\0", _PAGO_CUENTA( h ) );
            _snprintf( plan, 256, "p%i\0", _PAGO_CUENTA( h ) /*+1*/ );
            _snprintf( ident, 256, "i%i\0", _PAGO_COD_SUBMEDIO( h ) );
            //ins_card(e, ident, plan, _PAGO_IMPORTE(h)); 
            //ins_card( e, ident, plan, _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h ) );
            //ins_card( e, ident, plan, _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h ) );
            ins_card( e, ident, plan, _PAGO_IMPORTE( h ) - _PAGO_VUELTO_MEDIO( h ) );
			#ifdef LOG_MOTOR_PROMO
			{
				char tmp[256];
				memset(tmp,0,sizeof(tmp));
				//sprintf(tmp,"PROMO: DEBUG: means(%i).card = %lf",h , _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h ) );
				sprintf(tmp,"PROMO: DEBUG: means(%i).card(c%i, p%i) = %lf",h, _PAGO_COD_SUBMEDIO( h ), _PAGO_CUENTA( h ), _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h ) );
				glog(tmp,LOG_DEBUG,4);
			}
			#endif //LOG_MOTOR_PROMO
        }
        else if( medios[_PAGO_MODO( h )].ticket_compra ) {
            //_snprintf( ident, 256, "i%i\0", _PAGO_CUENTA( h ) );
            //_snprintf( ident, 256, "i%i\0", _PAGO_COD_SUBMEDIO( h ) );
            _snprintf( ident, 256, "m%is%i\0", _PAGO_MODO( h ) + 1, _PAGO_COD_SUBMEDIO( h ) );
            //ins_ticket(e, ident, _PAGO_IMPORTE(h) - _PAGO_VUELTO(h));
            ins_ticket( e, ident, _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h ) );
			#ifdef LOG_MOTOR_PROMO
			{
				char tmp[256];
				memset(tmp,0,sizeof(tmp));
				sprintf(tmp,"PROMO: DEBUG: means(%i).ticket = %lf",h , _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h ) );
				glog(tmp,LOG_DEBUG,4);
			}
			#endif //LOG_MOTOR_PROMO
        }
        else if( medios[_PAGO_MODO( h )].mutual ) {
            //_snprintf( ident, 256, "i%i\0", _PAGO_CUENTA( h ) );
            _snprintf( ident, 256, "m%is%i\0", _PAGO_MODO( h ) + 1, _PAGO_CUENTA( h ) );
            //ins_mutual(e, ident, _PAGO_IMPORTE(h) - _PAGO_VUELTO(h));
            //ins_mutual( e, ident, _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h ) );
            ins_mutual( e, ident, _PAGO_IMPORTE( h ) - _PAGO_VUELTO_MEDIO( h ) );
			#ifdef LOG_MOTOR_PROMO
			{
				char tmp[256];
				sprintf(tmp,"PROMO: DEBUG: means(%i).mutual = %lf",h , _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h ) );
				glog(tmp,LOG_DEBUG,4);
			}
			#endif //LOG_MOTOR_PROMO
        }
        else if( medios[_PAGO_MODO( h )].moneda_extranjera ) {
            _snprintf(ident,256,"i%i\0",_PAGO_CUENTA(h));
            //_snprintf( ident, 256, "i%s\0", ( int )medios[_PAGO_MODO( h )].simbolo_moneda );
            //ins_currency(e, ident, _PAGO_IMPORTE_ME(h) - /*RAM_RESTO_ME*/_PAGO_VUELTO(h));
            //ins_currency( e, ident, _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h ) );
            ins_currency( e, ident, _PAGO_IMPORTE( h ) - _PAGO_VUELTO_MEDIO( h ) );
			#ifdef LOG_MOTOR_PROMO
			{
				char tmp[256];
				memset(tmp,0,sizeof(tmp));
 				sprintf(tmp,"PROMO: DEBUG: means(%i).currency = %lf",h , _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h ) );
				glog(tmp,LOG_MOTOR_PROMO,4);
			}
			#endif //LOG_MOTOR_PROMO
        }
        else {
            glog( "PROMO: extern.c: means: ERROR: medio de pago no valido" ,LOG_MOTOR_PROMO,3);
        }
    }

    //_PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h )
    //PAGOS_AGREGAR_PAGO
    return e;
}

expr change()
{
	int h;
    expr e;
    e = new_means();    

    for( h = 0;h < RAM_P_PAGO;h++ ) {
        char ident[256];

        //_snprintf( ident, 256, "i%i", ( int )medios[_PAGO_MODO( h )].nro );
        memset(ident,0,sizeof(ident));
        if( medios[_PAGO_MODO( h )].efectivo ) {
            ins_cash( e, _PAGO_VUELTO_MEDIO( h )  );
        }
        else if( medios[_PAGO_MODO( h )].cta_cte ) {
            ins_ctacte( e, _PAGO_VUELTO_MEDIO( h ) );
        }
        else if( medios[_PAGO_MODO( h )].cheque ) {
            //_snprintf( ident, 256, "i%i\0", _PAGO_BANCO( h ) );
            _snprintf( ident, 256, "i%i\0", _PAGO_COD_SUBMEDIO( h ) );
            ins_cheque( e, ident, _PAGO_VUELTO_MEDIO( h ) );
        }
        else if( medios[_PAGO_MODO( h )].tarjeta ) {
            char plan[256];

            memset(plan,0,sizeof(plan));
            _snprintf( plan, 256, "p%i\0", ( int )_PAGO_CUENTA( h ) /*+1*/ );
            _snprintf( ident, 256, "i%i\0", _PAGO_COD_SUBMEDIO( h ) );
            ins_card( e, ident, plan, _PAGO_VUELTO_MEDIO( h ) );
        }
        else if( medios[_PAGO_MODO( h )].ticket_compra ) {
            //_snprintf( ident, 256, "i%i\0", _PAGO_COD_SUBMEDIO( h ) );
            _snprintf( ident, 256, "m%is%i\0", _PAGO_MODO( h ) + 1, _PAGO_COD_SUBMEDIO( h ) );
            //_snprintf( ident, 256, "i%i\0", _PAGO_CUENTA( h ) );
            ins_ticket( e, ident, _PAGO_VUELTO_MEDIO( h ) );
        }
        else if( medios[_PAGO_MODO( h )].mutual ) {
            //_snprintf( ident, 256, "i%i\0", _PAGO_CUENTA( h ) );
            _snprintf( ident, 256, "m%is%i\0", _PAGO_MODO( h ) + 1, _PAGO_COD_SUBMEDIO( h ) );
            //_snprintf( ident, 256, "i%i\0", _PAGO_COD_SUBMEDIO( h ) );
            ins_mutual( e, ident, _PAGO_VUELTO_MEDIO( h ) );
        }
        else if( medios[_PAGO_MODO( h )].moneda_extranjera ) {
            //_snprintf( ident, 256, "i%s\0", ( int )medios[_PAGO_MODO( h )].simbolo_moneda );
            _snprintf( ident, 256, "i%i\0", _PAGO_COD_SUBMEDIO( h ) );
            ins_currency( e, ident, _PAGO_VUELTO_MEDIO( h ) );
        }
        else {
            glog( "PROMO: extern.c: change: ERROR: medio de pago no valido" ,LOG_MOTOR_PROMO,3);
        }
    }

    return e;
}

expr returned()
{
	int h;
    expr e;
    e = new_means();    

    for( h = 0;h < RAM_P_PAGO;h++ ) {
        char ident[256];

        memset(ident,0,sizeof(ident));
		if ( _PAGO_VUELTO_MEDIO( h ) > 0 ) {


			if( medios[_PAGO_COD_MEDIO_VUELTO( h ) - 1].efectivo ) {
				ins_cash( e, _PAGO_VUELTO_MEDIO( h )  );
			}
			else if( medios[_PAGO_COD_MEDIO_VUELTO( h ) - 1].cta_cte ) {
				ins_ctacte( e, _PAGO_VUELTO_MEDIO( h ) );
			}
			else if( medios[_PAGO_COD_MEDIO_VUELTO( h ) - 1].cheque ) {
				//_snprintf( ident, 256, "i%i\0", _PAGO_BANCO( h ) );
				_snprintf( ident, 256, "i%i\0", _PAGO_COD_SUBMEDIO_VUELTO( h ) );
				ins_cheque( e, ident, _PAGO_VUELTO_MEDIO( h ) );
			}
			else if( medios[_PAGO_COD_MEDIO_VUELTO( h ) - 1].tarjeta ) {
				char plan[256];
				//_snprintf( plan, 256, "p%i\0", ( int )_PAGO_CUOTAS( h ) );
				//_snprintf( ident, 256, "i%i\0", _PAGO_CUENTA( h ) );
				_snprintf( plan, 256, "null" );
				_snprintf( ident, 256, "i%i\0", _PAGO_COD_SUBMEDIO_VUELTO( h ) );
				ins_card( e, ident, plan, _PAGO_VUELTO_MEDIO( h ) );
			}
			else if( medios[_PAGO_COD_MEDIO_VUELTO( h ) - 1].ticket_compra ) {
				//_snprintf( ident, 256, "i%i\0", _PAGO_CUENTA( h ) );
				_snprintf( ident, 256, "m%is%i\0", _PAGO_MODO( h ) + 1, _PAGO_COD_SUBMEDIO_VUELTO( h ) );
				//_snprintf( ident, 256, "i%i\0", _PAGO_COD_SUBMEDIO_VUELTO( h ) );
				ins_ticket( e, ident, _PAGO_VUELTO_MEDIO( h ) );
			}
			else if( medios[_PAGO_COD_MEDIO_VUELTO( h ) - 1].mutual ) {
				//_snprintf( ident, 256, "i%i\0", _PAGO_COD_SUBMEDIO_VUELTO( h ) );
				_snprintf( ident, 256, "m%is%i\0", _PAGO_MODO( h ) + 1, _PAGO_COD_SUBMEDIO_VUELTO( h ) );
				//_snprintf( ident, 256, "i%i\0", _PAGO_CUENTA( h ) );
				ins_mutual( e, ident, _PAGO_VUELTO_MEDIO( h ) );
			}
			else if( medios[_PAGO_COD_MEDIO_VUELTO( h ) - 1].moneda_extranjera ) {
				//_snprintf( ident, 256, "i%s\0", ( int )medios[_PAGO_MODO( h )].simbolo_moneda );
				_snprintf( ident, 256, "i%i\0", _PAGO_COD_SUBMEDIO_VUELTO( h ) );
				ins_currency( e, ident, _PAGO_VUELTO_MEDIO( h ) );
			}
			else {
				glog( "PROMO: extern.c: change: ERROR: medio de pago no valido" ,LOG_MOTOR_PROMO,3);
			}


		}

    }

    return e;
}


expr purchase()
{
    expr e;
    int i;
    int p_art;
    int l;
    struct _acu_art *acu_art;
    double total_cobros;
    char buff[100];

    glog( "PROMO: PURCHASE" ,LOG_MOTOR_PROMO,4);

    l = sizeof( struct _acu_art ) * _LARGO_TICKET;
    acu_art = MALLOC( l );
    p_art = CARGA_ACU_ART( acu_art, l, SI, &total_cobros, SI, SI );

    e = new_amset();
    /*
    #ifdef PROMO_FAULT_TOLERANCE
      { int p = 0;
        p = readPurchase();
        if (p >= 0) {
            p_art = 0;
        }
        logPurchase(p_art);
      }
    #endif //PROMO_FAULT_TOLERANCE
    */

    for( i = 0;i < p_art;i++ ) {
        /*
           buff[0] = 'b';
           strcpy(&buff[1],acu_art[i].cod_barra);
        */
        buff[0] = 'i';
        sprintf( &buff[1], "%li", _TICKET_COD_ESPECIAL( acu_art[i].pos ) );
        //glog( buff );
        ins_amset( e, buff, acu_art[i].cant );
    }
    FREEMEM( ( char* )acu_art );
    return e;
}



expr buyed_articles( void )
{
    expr e;
    int i;
    int p_art;
    int l;
    struct _acu_art *acu_art;
    double total_cobros;
    char buff[100];

    glog( "PROMO: BUYED ARTICLES" ,LOG_MOTOR_PROMO,4);

    l = sizeof( struct _acu_art ) * _LARGO_TICKET;
    acu_art = MALLOC( l );
    p_art = CARGA_ACU_ART( acu_art, l, SI, &total_cobros, SI, SI );

    e = new_amset();
    for( i = 0;i < p_art;i++ ) {
        if( _TICKET_ORIGEN( i ) == _ORIGEN_TICKET ) {
            buff[0] = 'i';
            sprintf( &buff[1], "%li", _TICKET_COD_ESPECIAL( acu_art[i].pos ) );
            //glog( buff );
            ins_amset( e, buff, acu_art[i].cant );
        }
    }
    FREEMEM( ( char* )acu_art );
    return e;
}

expr delivered_articles( void )
{
    expr e;
    int i;
    int p_art;
    int l;
    struct _acu_art *acu_art;
    double total_cobros;
    char buff[100];

    glog( "PROMO: DELIVERED ARTICLES" ,LOG_MOTOR_PROMO,4);

    l = sizeof( struct _acu_art ) * _LARGO_TICKET;
    acu_art = MALLOC( l );
    p_art = CARGA_ACU_ART( acu_art, l, SI, &total_cobros, SI, SI );

    e = new_amset();
    for( i = 0;i < p_art;i++ ) {
        if( _TICKET_ORIGEN( i ) == _ORIGEN_PROMOCION ) {
            buff[0] = 'i';
            sprintf( &buff[1], "%li", _TICKET_COD_ESPECIAL( acu_art[i].pos ) );
            //glog( buff );
            ins_amset( e, buff, acu_art[i].cant );
        }
    }
    FREEMEM( ( char* )acu_art );
    return e;
}


expr discounts_credits( void )
{
    expr e;
    int i;
    int p_art;
    int l;
    struct _acu_art *acu_art;
    double total_cobros;
    char buff[100];

    glog( "PROMO: DISCOUNTS - CREDITS" ,LOG_MOTOR_PROMO,4);

    l = sizeof( struct _acu_art ) * _LARGO_TICKET;
    acu_art = MALLOC( l );
    p_art = CARGA_ACU_ART( acu_art, l, SI, &total_cobros, SI, SI );

    e = new_amset();
    for( i = 0;i < p_art;i++ ) {
        if( _TICKET_ORIGEN( i ) == _ORIGEN_PROMOCION_DESC ) {
            buff[0] = 'i';
            sprintf( &buff[1], "%li", _TICKET_COD_ESPECIAL( acu_art[i].pos ) );
            //glog( buff );
            ins_amset( e, buff, acu_art[i].cant );
        }
    }
    FREEMEM( ( char* )acu_art );
    return e;
}


double num_rnv( identifier ident )
{
    /*char c;*/
    int var = 0;
    int ind = 0;
    double res = 0;

	//truchada
	if (strcmp(ident, "cantidad_eventos_enviar_offline") == 0) {
		strncpy(ident,"i1845",5);
		ident[5]=0;

	}
	//

    rnv_error = 0;

    if( arreglo( ident, &var, &ind ) ) {
        // doble indice. i.e. un arreglo

        switch( DD_TIPO( var ) ) {
            case _SHORT_INT:
                res = _GET_MEMORY_CHAR( var, ind );
                break;
            case _INT:
                res = _GET_MEMORY_INT( var, ind );
                break;
            case _FLOAT:
                res = _GET_MEMORY_FLOAT( var, ind );
                break;
            case _DOUBLE:
                res = _GET_MEMORY_DOUBLE( var, ind );
                break;
            case _LONG:
                res = _GET_MEMORY_LONG( var, ind );
                break;
            case _UNSIGNED:
                res = _GET_MEMORY_UNSIGNED( var, ind );
                break;
            default:
                rnv_error = 1;
                res = 0;
                break;
        }
    }
    else {
        switch( DD_TIPO( var ) ) {
            case _SHORT_INT:
                res = GET_MEMORY_CHAR( var );
                break;
            case _INT:
                res = GET_MEMORY_INT( var );
                break;
            case _FLOAT:
                res = GET_MEMORY_FLOAT( var );
                break;
            case _DOUBLE:
                res = GET_MEMORY_DOUBLE( var );
                break;
            case _LONG:
                res = GET_MEMORY_LONG( var );
                break;
            case _UNSIGNED:
                res = GET_MEMORY_UNSIGNED( var );
                break;
            default:
                rnv_error = 1;
                res = 0;
                break;
        }
    }
    /*  {
            char buff[256];
            sprintf(buff,"NUM_RNV:%lf\n",res);
            glog(buff);
        }
    */
    return res;
}

int bool_rnv( identifier ident )
{
    char c;
    int var = 0;
    int ind = 0;
    /*int res;*/

    if( strcmp( ident, "error" ) == 0 ) {
        return rnv_error;
    }
    else {
        rnv_error = 0;

        if( arreglo( ident, &var, &ind ) ) {
            // doble indice. i.e. un arreglo

            switch( DD_TIPO( var ) ) {
                case _SHORT_INT:
                    c = _GET_MEMORY_CHAR( var, ind );
                    if( ( c ) && ( c != 'n' ) && ( c != 'N' ) ) {
                        //glog("BOOL_RNV: TRUE");
                        return 1;
                    }
                    else {
                        //glog("BOOL_RNV: FALSE");
                        return 0;
                    }
                    break;
                default:
                    rnv_error = 1;
                    return 0;
                    break;
            }
        }
        else {
            switch( DD_TIPO( var ) ) {
                case _SHORT_INT:
                    c = GET_MEMORY_CHAR( var );
                    if( ( c ) && ( c != 'n' ) && ( c != 'N' ) ) {
                        //glog("BOOL_RNV: TRUE");
                        return 1;
                    }
                    else {
                        //glog("BOOL_RNV: FALSE");
                        return 0;
                    }
                    break;
                default:
                    rnv_error = 1;
                    return 0;
                    break;
            }
        }
    }
}



/*
 * Esta funcion debe devolver una copia del string en un nuevo espacio de memoria.
 * El evaluador se encarga de hacer free cuando corresponda.
 */
char * string_rnv( identifier ident )
{
    /*char c;*/
    int var = 0;
    int ind = 0;

    rnv_error = 0;

    if( arreglo( ident, &var, &ind ) ) {
        // doble indice. i.e. un arreglo
        switch( DD_TIPO( var ) ) {
            case _STRING:
                return strcpy( ( char* )malloc( strlen( _GET_MEMORY_STR( var, ind ) ) + 1 ),
                               GET_MEMORY_STR( var ) );
                break;
            default:
                rnv_error = 1;
                return NULL;
                break;
        }
    }
    else {
        switch( DD_TIPO( var ) ) {
            case _STRING:
                return strcpy( ( char* )malloc( strlen( GET_MEMORY_STR( var ) ) + 1 ),
                               GET_MEMORY_STR( var ) );
                break;
            default:
                rnv_error = 1;
                return NULL;
                break;
        }
    }
}

long date_rnv( identifier ident )
{
    /*char c;*/
    int var = 0;
    int ind = 0;

    rnv_error = 0;
    if( arreglo( ident, &var, &ind ) ) {
        // doble indice. i.e. un arreglo
        switch( DD_TIPO( var ) ) {
            case _DATE:
            case _UNSIGNED:
                {
                    //int f = _GET_MEMORY_INT( var, ind );
                    int f = _GET_MEMORY_UNSIGNED( var, ind );
                    if( f > 0 ) {
                        return mk_date_from_invel( f );
                    }
                    else {
                        rnv_error = 1;
                        return 0;
                    }
                }
                break;
            default:
                rnv_error = 1;
                return 0;
                break;
        }
    }
    else {
        switch( DD_TIPO( var ) ) {
            case _DATE:
            case _UNSIGNED:
                {
                    //int f = GET_MEMORY_INT( var );
                    int f = GET_MEMORY_UNSIGNED( var );
                    if( f > 0 ) {
                        return mk_date_from_invel( f );
                    }
                    else {
                        rnv_error = 1;
                        return 0;
                    }
                }
                break;
            default:
                rnv_error = 1;
                return 0;
                break;
        }
    }
}

long time_rnv( identifier ident )
{
    //#define  _DATE           6
    //#define  _HORA           7
    /*char c;*/
    int var = 0;
    int ind = 0;

    rnv_error = 0;
    if( arreglo( ident, &var, &ind ) ) {
        // doble indice. i.e. un arreglo
        switch( DD_TIPO( var ) ) {
            case _HORA:
            case _LONG:
            case _UNSIGNED:
                {
                    int f = _GET_MEMORY_INT( var, ind );
                    if( f > 0 ) {
                        return mk_time_from_invel( f );
                    }
                    else {
                        rnv_error = 1;
                        return 0;
                    }
                }
                break;
            default:
                rnv_error = 1;
                return 0;
                break;
        }
    }
    else {
        switch( DD_TIPO( var ) ) {
            case _HORA:
            case _LONG:
            case _UNSIGNED:
                {
                    int f = GET_MEMORY_INT( var );
                    if( f > 0 ) {
                        return mk_time_from_invel( f );
                    }
                    else {
                        rnv_error = 1;
                        return 0;
                    }
                }
                break;
            default:
                rnv_error = 1;
                return 0;
                break;
        }
    }
}

void num_rnv_update( identifier ident, double f )
{
    int var = 0;
    int ind = 0;

    rnv_error = 0;

	if (!ident) return;

	if ( strcmp(ident,"cupon_error") == 0 ) {
		on_error_val = (int) f;
		return;
	}

	if ( strcmp(ident,"cupon_vigencia") == 0 ) {
		on_error_vigencia = (int) f;
		{char buff[256]; sprintf(buff,"PROMO: num_rnv_update( cupon_vigencia ) = %i", on_error_vigencia );glog(buff,LOG_MOTOR_PROMO,4);}
		return;
	}

    if( arreglo( ident, &var, &ind ) ) {
        // doble indice. i.e. un arreglo

        switch( DD_TIPO( var ) ) {
            case _SHORT_INT:
                _SET_MEMORY_CHAR( var, ind, (int)f );
                break;
            case _INT:
                _SET_MEMORY_INT( var, ind, (int)f );
                break;
            case _FLOAT:
                _SET_MEMORY_FLOAT( var, ind, f );
                break;
            case _DOUBLE:
                _SET_MEMORY_DOUBLE( var, ind, f );
                break;
            case _LONG:
                _SET_MEMORY_LONG( var, ind, (long)f );
                break;
            case _UNSIGNED:
                _SET_MEMORY_INT( var, ind, (int)f );
                break;
            default:
                _SET_MEMORY_INT( var, ind, (int)f );
                rnv_error = 1;
                break;
        }
    }
    else {
        switch( DD_TIPO( var ) ) {
            case _SHORT_INT:
                SET_MEMORY_CHAR( var, (int)f );
                break;
            case _INT:
                SET_MEMORY_INT( var, (int)f );
                break;
            case _FLOAT:
                SET_MEMORY_FLOAT( var, f );
                break;
            case _DOUBLE:
                SET_MEMORY_DOUBLE( var, f );
                break;
            case _LONG:
                SET_MEMORY_LONG( var, (long)f );
                break;
            case _UNSIGNED:
                SET_MEMORY_INT( var, (int)f );
                break;
            default:
                SET_MEMORY_INT( var, (int)f );
                rnv_error = 1;
                break;
        }
    }
    return;
}

void bool_rnv_update( identifier ident, int b )
{
    //fprintf(stdout,"BOOL_RNV_UPDATE: %s = %s\n",ident, (b==FALSE ? "FALSE" : "TRUE"));
}

/*
 * Esta funcion debe hacer free del string que toma por parametro cuando deje de
 * utilizar dicho string.
 * ( assert: strlen( b ) < 256 )
 */
void string_rnv_update( identifier ident, char *b )
{

	if (!ident) return;

	if ( strcmp(ident,"cupon_limite") == 0 ) {
		strcpy(on_error_limite, b);
		{char buff[256];
		memset(buff,0,sizeof(buff));
		sprintf(buff,"PROMO: string_rnv_update( on_error_limite ) = %s", on_error_limite );glog(buff,LOG_MOTOR_PROMO,4);}
		return;
	}
	if ( strcmp(ident,"cupon_limite_label") == 0 ) {
		strncpy(on_error_limite_label, b, sizeof(on_error_limite_label));
		on_error_limite_label[ sizeof(on_error_limite_label) - 1 ] = '\0'; // Garantiza que exista el fin de cadena
		{char buff[256];
		memset(buff,0,sizeof(buff));
		_snprintf(buff,sizeof(buff),"PROMO: string_rnv_update( on_error_limite_label ) = %s", on_error_limite_label );
		buff[sizeof(buff)-1]='\0';
		glog(buff,LOG_MOTOR_PROMO,4); }

		return;
	}
    //fprintf(stdout,"STRING_RNV_UPDATE: %s = %s\n",ident, b);
    if(b )
    free( b );
}

void date_rnv_update( identifier ident, long b )
{
    char buf[20];
    strdate( buf, 20, b, 0 );
    fprintf( stdout, "DATE_RNV_UPDATE: %s = %s\n", ident, buf );
}
void time_rnv_update( identifier ident, long b )
{
    char buf[20];
    strtime( buf, 20, b );
    //fprintf(stdout,"TIME_RNV_UPDATE: %s = %s\n",ident, buf);
}

int countable( identifier article )
{
    //glog( "PROMO: countable" );
    if( article[0] == 'b' ) {
        ASIGNA_CODIGO_BARRA( &article[1] );
    }
    else {
        ASIGNA_CODIGO_ESPECIAL( atol( &article[1] ) );
    }
    return ( int ) ! ITEM_PESABLE;
}


void print( char *string )
{
    char buff[256];
    memset(buff,0,sizeof(buff));
    _snprintf( buff,sizeof(buff), "PROMO: print: [%s]", string );
    buff[sizeof(buff)-1]='\0';

    #ifdef PROMO_FAULT_TOLERANCE
    if ( recState == recoveringState ) {
        /* No debe imprimirse nada en este caso */
        /* Lo que sigue es solo para debug */
        /**/_snprintf( buff, 255, "PROMO: FaultPR: %s", string );
        //MENSAJE(buff);
        //glog( buff );
        /* ------------------------------- */
    }
    else {
		//MENSAJE_TECLA( string );
        //MENSAJE( string );
/*		
 			ExecuteSSAction( A_PEDIR, _HIBRIDO, " MENSAJE DE PROMOCIONES ", string, NULL );
			MENSAJE_TECLA("Presiona alguna tecla para continuar...");
			ExecuteAction( A_FIN_PEDIR );
*/
		char *msg[20];
		char *str;
		char seps[]   = "&";
		char *token;
		int i = 0, j;

		str = (char*)malloc(strlen(string)+1);
		strncpy(str,string,strlen(string)+1);

		token = strtok( str, seps );
		while( ( token != NULL ) && ( i < 20 ) )
		{
			/* While there are tokens in "string" */
			//printf( " %s\n", token );
			msg[i] = (char*)malloc(strlen(token)+1);
			strncpy(msg[i],token,strlen(token)+1);

			/* Get next token: */
			token = strtok( NULL, seps );

			i++;
		}

		MOSTRAR_MENSAJE_POP_UP( "MENSAJE DE PROMOCIONES", msg, i );

		for (j=0 ; j<i; j++) {
		   if( msg[j] )
			free (msg[j]);
		}
		if(str)
		free (str);
    }

    #else //PROMO_FAULT_TOLERANCE

	//MENSAJE_TECLA( string );
    //MENSAJE( string );
/*
	ExecuteSSAction( A_PEDIR, _HIBRIDO, " MENSAJE DE PROMOCIONES ", string, NULL );
	MENSAJE_TECLA("Presiona alguna tecla para continuar...");
	ExecuteAction( A_FIN_PEDIR );
*/
	{
		char *msg[20];
		char *str;
		char seps[]   = "&";
		char *token;
		int i = 0;

		str = (char*)malloc(strlen(string)+1);
		strncpy(str,string,strlen(string)+1);

		token = strtok( str, seps );
		while( ( token != NULL ) && ( i < 20 ) )
		{
			/* While there are tokens in "string" */
			//printf( " %s\n", token );
			msg[i] = (char*)malloc(strlen(token)+1);
			strncpy(msg[i],token,strlen(token)+1);

			/* Get next token: */
			token = strtok( NULL, seps );

			i++;
		}

		MOSTRAR_MENSAJE_POP_UP( "MENSAJE DE PROMOCIONES", msg, i );

		if( str) free (str);

	}
    #endif //PROMO_FAULT_TOLERANCE

    /**/ if (buff) ;//glog( buff );
    /**/ if (buff) glog("PROMO: BUFFER",LOG_MOTOR_PROMO,4);
}



double cashdesk( void )
{
    #ifdef PROMO_FAULT_TOLERANCE
    int r = 0;
    //#define  NRO_CAJA                                 GET_MEMORY_INT( __nro_caja )
    if( readNumRnv( &r, __nro_caja, -1 ) ) {
        return ( double )r;
    }
    else {
        glog( "PROMO: cashdesk: ERROR. error leyendo el la rnv desde el log" ,LOG_MOTOR_PROMO,3);
        return -1;
    }
    #else //PROMO_FAULT_TOLERANCE
    return NRO_CAJA;
    #endif //PROMO_FAULT_TOLERANCE
}


expr points( void )
{
    double puntos_local;
    double puntos_birthday;
    expr e;
    e = new_points();

    #ifdef PROMO_FAULT_TOLERANCE

    if( readNumRnv( ( void* )&puntos_local, __ram_acu_puntos, -1 ) ) {
    }
    else {
        glog( "PROMO: expr points(): ERROR. error leyendo el la rnv desde el log" ,LOG_MOTOR_PROMO,3);
        puntos_local = 0;
    }

    if( readNumRnv( &puntos_birthday, __ram_acu_puntos_cumple, -1 ) ) {
    }
    else {
        glog( "PROMO: expr points(): ERROR. error leyendo el la rnv desde el log" ,LOG_MOTOR_PROMO,3);
        puntos_birthday = 0;
    }
    #else //PROMO_FAULT_TOLERANCE
    puntos_local = RAM_ACU_PUNTOS;
    puntos_birthday = RAM_ACU_PUNTOS_CUMPLE;
    #endif //PROMO_FAULT_TOLERANCE


    ins_point( e, "local", puntos_local );
    ins_point( e, "birthday", puntos_birthday );
    return e;

}

expr ebonus( void )
{
    expr e;
    e = new_ebonus();
    ins_point( e, "c01", 100 );
    ins_point( e, "c02", 25 );
    ins_point( e, "c01", 10 );
    ins_point( e, "c03", 50 );
    return e;
}


int _input( char *str )
{
    //  int AFINIDAD_DESCUENTO_PUNTOS( double puntos, double descuento )
    int rta = 0;
    int salir = 0, k = 0;
    int espera = 0;
    #ifdef PROMO_FAULT_TOLERANCE
    int logVal = 0;


    logVal = readBoolInput();
    if( logVal == -1 ) {
        #endif //PROMO_FAULT_TOLERANCE

        MENSAJE_SIN_SONIDO( str,NO );
        espera = GET_ESPERA_GETCH();

        SET_ESPERA_GETCH( 1 );
		ExecuteAction( A_VENTAS_FOCUS );
        while( !salir ) {
            k = GETCH();
            if( k == 27 ) {
                salir = 1;
            }
            if( k == 13 ) {
                salir = 1;
                rta = 1;
            }
        }
        SET_ESPERA_GETCH( espera );

        #ifdef PROMO_FAULT_TOLERANCE
    }
    else {
        rta = logVal;
    }

    logBoolInput( rta );
    #endif //PROMO_FAULT_TOLERANCE
    
	MENSAJE_SIN_SONIDO( "",NO );
    return rta;
}


double numinput( char *str )
{
    double rta = 0;
    int r = 0;
    int salir = 0/*, k*/;
    /*int       espera;*/
    #ifdef PROMO_FAULT_TOLERANCE
    double logVal;


    r = readNumInput( &logVal );
    if( r == -1 ) {
        char * postpago;
        #endif //PROMO_FAULT_TOLERANCE
       	//aca las promos postpago se leen distinto el input que las prepago como el descuento interactivo
		postpago=strrchr(str,'|');
		if( postpago == NULL)
			postpago=strrchr(str,'~');

		if( postpago == NULL) 
			rta = ( double )PEDIR_FLOAT_EN_RECUADRO_CON_TITULO( str );
		else {
		//rta = ( double )PEDIR_FLOAT_EN_RECUADRO_CON_TITULO2( "Precio Final 70.00 |~CON CUANTO ABONA ?:|Aplico Desc/Recar 30.00." );
			rta = ( double )PEDIR_FLOAT_EN_RECUADRO_CON_TITULO2( str );
			SET_MEMORY_LONG( __nro_pagare, 1 );//Variable reutilizada para identificar cuando se promociona el importe total ingresado
		}


        #ifdef PROMO_FAULT_TOLERANCE
    }
    else {
        rta = logVal;
    }

    logNumInput( rta );
    #endif //PROMO_FAULT_TOLERANCE
	MENSAJE_SIN_SONIDO( "",NO );
    return rta;
}


idate get_date()
{
    idate rta;
    int r = 0;
    #ifdef PROMO_FAULT_TOLERANCE
    long logVal;


    r = readDate( &logVal );
    if( r == -1 ) {
        #endif //PROMO_FAULT_TOLERANCE

        rta = getdate_();

        #ifdef PROMO_FAULT_TOLERANCE
    }
    else {
        rta = ( idate )logVal;
    }

    logDate( rta );
    #endif //PROMO_FAULT_TOLERANCE

    return rta;
}

itime get_time()
{
    idate rta;
    int r = 0;
    #ifdef PROMO_FAULT_TOLERANCE
    long logVal;


    r = readTime( &logVal );
    if( r == -1 ) {
	#endif //PROMO_FAULT_TOLERANCE

        rta = gettime_();

    #ifdef PROMO_FAULT_TOLERANCE
    }
    else {
        rta = ( itime )logVal;
    }

    logTime( rta );
    #endif //PROMO_FAULT_TOLERANCE
/*
    {
        char buff[256];sprintf( buff, "PROMO: TIME: %li", rta );glog( buff );
    }
*/
    return rta;
}



/*
 * Para la supuesta funcion distribute, estudiar la funcion
 * void AGREGAR_DESCUENTO_ARTICULO( long cod_art, double precio, float cantidad,
 *  int id_registro )
 *
 * o sino
 *
 * void AGREGAR_ART_PROMO( long cod_art, float cant, double coef, int enganchar,
 *  double **base_descuento, struct _acu_art *acu_art,  int p_art, int h )

  probar con:
AGREGAR_ART_PROMO( cod_art, cant, coef, enganchar=NO,
                    base_descuento=NULL, acu_art=NULL,  p_art=0, h=0 )

 *
 */




__AGREGAR_ART_PROMO( long cod_art, double cant,
                     double coef /* es en realidad un porcentage del precio*/ )
{
    /*double    //precio, precio_sin_iva, costo, iva1_precio, iva2_precio, iva1_rec, iva2_rec;*/
    /*double     imp[4];*/

    ASIGNA_CODIGO_ESPECIAL( cod_art );

    SET_MEMORY_DOUBLE( __ram_cantidad, cant );
    SET_MEMORY_INT( __ram_anulacion, 0 );

    AGREGA_ITEM_AL_TICKET( RAM_P_TICKET, 0, NO, NO, _ORIGEN_PROMOCION );
    /**/ADD_MEMORY_INT( __ram_cant_promociones, 1 );

    SET_MEMORY_DOUBLE( __item_precio, ITEM_PRECIO * ( -1 ) );
    _SET_MEMORY_INT( __ticket_presentacion, RAM_P_TICKET, 1 );
    _SET_MEMORY_DOUBLE( __ticket_precio, RAM_P_TICKET, ITEM_PRECIO * coef );
    _SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, RAM_P_TICKET,
                        -1 * coef * _TICKET_IMPORTE_SIN_IVA( RAM_P_TICKET ) );
    _SET_MEMORY_DOUBLE( __ticket_exento, RAM_P_TICKET, -1 * coef * _TICKET_EXENTO( RAM_P_TICKET ) );
    _SET_MEMORY_DOUBLE( __ticket_iva1_precio, RAM_P_TICKET,
                        -1 * coef * _TICKET_IVA1_PRECIO( RAM_P_TICKET ) );
    _SET_MEMORY_DOUBLE( __ticket_iva2_precio, RAM_P_TICKET,
                        -1 * coef * _TICKET_IVA2_PRECIO( RAM_P_TICKET ) );
	
    /*_SET_MEMORY_FLOAT( __ticket_ila_precio, RAM_P_TICKET,
        -1 * coef * _TICKET_ILA_PRECIO( RAM_P_TICKET ) );
    _SET_MEMORY_FLOAT( __ticket_iha_precio, RAM_P_TICKET,
        -1 * coef * _TICKET_IHA_PRECIO( RAM_P_TICKET ) );
     */
    _SET_MEMORY_FLOAT( __ticket_impuesto1_precio, RAM_P_TICKET,
                       -1 * coef * _TICKET_IMPUESTO1_PRECIO( RAM_P_TICKET ) );
    _SET_MEMORY_FLOAT( __ticket_impuesto2_precio, RAM_P_TICKET,
                       -1 * coef * _TICKET_IMPUESTO2_PRECIO( RAM_P_TICKET ) );
    _SET_MEMORY_FLOAT( __ticket_impuesto3_precio, RAM_P_TICKET,
                       -1 * coef * _TICKET_IMPUESTO3_PRECIO( RAM_P_TICKET ) );
    _SET_MEMORY_FLOAT( __ticket_impuesto4_precio, RAM_P_TICKET,
                       -1 * coef * _TICKET_IMPUESTO4_PRECIO( RAM_P_TICKET ) );

    _SET_MEMORY_DOUBLE( __ticket_recargo, RAM_P_TICKET,
                        -1 * coef * _TICKET_RECARGO( RAM_P_TICKET ) );
    _SET_MEMORY_DOUBLE( __ticket_imp_int, RAM_P_TICKET,
                        -1 * coef * _TICKET_IMP_INT( RAM_P_TICKET ) );
    _SET_MEMORY_DOUBLE( __ticket_iva1_rec, RAM_P_TICKET,
                        -1 * coef * _TICKET_IVA1_REC( RAM_P_TICKET ) );
    _SET_MEMORY_DOUBLE( __ticket_iva2_rec, RAM_P_TICKET,
                        -1 * coef * _TICKET_IVA2_REC( RAM_P_TICKET ) );
    /*_SET_MEMORY_FLOAT( __ticket_ila_rec, RAM_P_TICKET,
        -1 * coef * _TICKET_ILA_REC( RAM_P_TICKET ) );
    _SET_MEMORY_FLOAT( __ticket_iha_rec, RAM_P_TICKET,
        -1 * coef * _TICKET_IHA_REC( RAM_P_TICKET ) );*/
    ADD_MEMORY_INT( __ram_p_ticket, 1 );
    SET_MEMORY_DOUBLE( __ram_total, ROUND( RAM_TOTAL + cant * ITEM_PRECIO, 10, DECIMALES ) );
    if( DEBE_IMPRIMIR_ITEM() ) {
        IMPRIME_ITEM( RAM_P_TICKET - 1 );
    }
    ACTUALIZA_ACUMULADORES_RAM( RAM_P_TICKET - 1 );
    GRABAR_EVENT_E_TICKET( RAM_P_TICKET - 1, NO, SI, _ORIGEN_PROMOCION, 0, NO );
    GRABAR_TICKET( RAM_P_TICKET - 1 );

    //if( enganchar )ACUMULAR_BASE_DESCUENTO( base_descuento, acu_art, p_art, 100.0, NO, h, cant );

    //if( enganche && enganchar ) {
    //  AGREGAR_ART_PROMO( enganche, cant, coef, NO, base_descuento,
    //      acu_art,    p_art, h );
    //}
    SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
}

/*****************************************************************************/
long codMedioCash(identifier ident)
{
	int h = 0;
	if (ident && ident[0]=='m') {
		return atol(&ident[1]);
	} else {
		for( h = 0;h < 9;h++ ) {
			if( ( medios[h].nro ) && ( medios[h].efectivo ) ) {
				return h+1;
			}
		}
		glog( "PROMO: extern.c: codMedioCard: ERROR: No se encontró el Código de Medio correspondiente a Cash" ,LOG_MOTOR_PROMO,3);
		return -1;
	}
}

long codMedioCtaCte(identifier ident)
{
	int h = 0;
	if (ident && ident[0]=='m') {
		return atol(&ident[1]);
	} else {
		for( h = 0;h < 9;h++ ) {
			if( ( medios[h].nro ) && ( medios[h].cta_cte ) ) {
				return h+1;
			}
		}
		glog( "PROMO: extern.c: codMedioCtaCte: ERROR: No se encontró el Código de Medio correspondiente a CtaCte" ,LOG_MOTOR_PROMO,3);
		return -1;
	}
}

long codMedioMutual(identifier ident)
{
	int h = 0;
	if (ident && ident[0]=='m') {
		return atol(&ident[1]);
	} else {
		for( h = 0;h < 9;h++ ) {
			if( ( medios[h].nro ) && ( medios[h].mutual ) ) {
				return h+1;
			}
		}
		glog( "PROMO: extern.c: codMedioMutual: ERROR: No se encontró el Código de Medio correspondiente a Mutual" ,LOG_MOTOR_PROMO,3);
		return -1;
	}
}

long codMedioTicket(identifier ident)
{
	int h = 0;
	if (ident && ident[0]=='m') {
		return atol(&ident[1]);
	} else {
		for( h = 0;h < 9;h++ ) {
			if( ( medios[h].nro ) && ( medios[h].ticket_compra ) ) {
				return h+1;
			}
		}
	glog( "PROMO: extern.c: codMedioCard: ERROR: No se encontró el Código de Medio correspondiente a Ticket" ,LOG_MOTOR_PROMO,3);
		return -1;
	}
}


long codMedioCheque(identifier ident)
{
	int h = 0;
	if (ident && ident[0]=='m') {
		return atol(&ident[1]);
	} else {
		for( h = 0;h < 9;h++ ) {
			if( ( medios[h].nro ) && ( medios[h].cheque ) ) {
				return h+1;
			}
		}
		glog( "PROMO: extern.c: codMedioCheque: ERROR: No se encontró el Código de Medio correspondiente a Cheques" ,LOG_MOTOR_PROMO,3);
		return -1;
	}
}


long codMedioCurrency(identifier ident)
{
	int h = 0;
	if (ident && ident[0]=='m') {
		return atol(&ident[1]);
	} else {
		for( h = 0;h < 9;h++ ) {
			if( ( medios[h].nro ) && ( medios[h].moneda_extranjera ) ) {
				return h+1;
			}
		}
		glog( "PROMO: extern.c: codMedioCurrency: ERROR: No se encontró el Código de Medio correspondiente a Moneda Extranjera",LOG_MOTOR_PROMO,3 );
		return -1;
	}
}


long codMedioCard(identifier ident)
{
	int h = 0;
	if (ident && ident[0]=='m') {
		return atol(&ident[1]);
	} else {
		for( h = 0;h < 9;h++ ) {
			if( ( medios[h].nro ) && ( medios[h].tarjeta ) ) {
				return h+1;
			}
		}
		glog( "PROMO: extern.c: codMedioCard: ERROR: No se encontró el Código de Medio correspondiente a Tarjetas" ,LOG_MOTOR_PROMO,3);
		return -1;
	}
}



long codSubMedio(identifier ident)
{
	long rta = -1;
	if ( ident ) {
		if ( ident[0] == 'm' ) {
			// name es de la forma mXXXsYYY con XXX código de medio e YYY código de submedio.
			char *str;
			char seps[]   = "s";
			char *token;

			str = (char*)malloc(strlen(ident)+1);
			strncpy(str,ident,strlen(ident)+1);

			token = strtok( str, seps );
			while ( token && ( (token[0] < '0') || (token[0] > '9') )) {
				token = strtok( NULL, seps );
			}
			if ( token ) {
				rta = atol(token);
			}
			if(str )
			free( str );
		} else {
			return atol(&ident[1]);
		}
	} else {
		glog("promo: codSubMedio: Error: Codigo de submedio no encontrado.",LOG_MOTOR_PROMO,3);
	}
	return rta;
}


void read_promo_config( char* strArch ) {
   FILE *stream;
   char line[256];

   if( (stream = fopen( strArch, "r" )) != NULL )
   {
	   while ( fgets( line, 256, stream ) != NULL) {
         //glog( "extern.c: read_promo_config: fgets error" );
		  char*pwnd;
		  //{char buff[256];sprintf(buff, "extern.c: read_promo_config: -> %s\n", line);glog(buff);}

		  pwnd=strtok(line,"=");
		  //glog(pwnd);

		  if ( strncmp("on_error_val",pwnd,12) == 0 ) {
			pwnd=strtok(NULL,";");
		    //glog(pwnd);
			on_error_val = atoi(pwnd);
		  {char buff[256];sprintf(buff, "extern.c: read_promo_config: on_error_val = %i\n", on_error_val);glog(buff,LOG_MOTOR_PROMO,3);}

		  }

		  if ( strncmp("limite_bono_label",pwnd,17) == 0 ) {
			pwnd=strtok(NULL,"\n\r");
		    //glog(pwnd);
			strncpy( on_error_limite_label, pwnd, 255 );
			on_error_limite_label[255] = '\0';
			{char buff[256];
			memset(buff,0,sizeof(buff));
			_snprintf(buff, sizeof(buff), "extern.c: read_promo_config: limite_bono_label = %s", on_error_limite_label);
			buff[sizeof(buff)-1]='\0';
			glog(buff,LOG_MOTOR_PROMO,3);}

		  }

	  }
      fclose( stream );
   }
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/


struct cashdesk_state
{
    int p_ticket; // indice del ultimo elemento del ticket.
    double acu_puntos;
    double acu_puntos_cumple;

    //long* _ext_impuestos;
    //long* __ram_medios_obligatorios;

    int cant_promociones;
    int bonos_a_entregar;
    int ptr_prom_cupon;


    struct
    {
        int prom_cupon_formato;
        int prom_cupon_cantidad;
        char prom_cupon_lugar_impresion;
        double prom_cupon_importe;
        char* cupon_leyenda;
    } cupones[5]; //porque se limita a 5 ???
};

cashdesk_state recoverCashdeskState( cashdesk_state s )
{
    cashdesk_state r;
    int j;
    r = ( cashdesk_state )malloc( sizeof( struct cashdesk_state ) );
    memcpy( r, s, sizeof( struct cashdesk_state ) );

    j = sizeof( struct cashdesk_state );

    /*for( i = 0;i < 5;i++ ) {
        char *str = ( char * )( ( int )s + j );
        char *cp;
        l = strlen( str ) + 1;

        cp = ( char* )malloc( l );
        strcpy( cp, str );

        r->cupones[i].cupon_leyenda = cp;

        j += l;
    }*/ //ver fabian

    return r;
}

// devuelve una estructura que representa al estado actual de la caja
cashdesk_state actual_state( void )
{
    cashdesk_state s; 


    #ifdef PROMO_FAULT_TOLERANCE
    //typedef enum {normalState, recoveredState, recoveringState} rState;
    //extern rState recState;
    if( ( recState == recoveringState ) && ( globalCashdeskState != NULL ) ) {
        ///////////////EN VEZ DE HACER ESTO, crear un clon y hacer un unico free del
        // globalCashdeskState.
        // el clon debe hacer mallocs para es struct por un lado y para los strings
        // aparte, uno para cada uno.
        s = recoverCashdeskState( globalCashdeskState );
        if( globalCashdeskState)
        free( globalCashdeskState );
        globalCashdeskState = NULL;
    }
    else {
        #endif //PROMO_FAULT_TOLERANCE

        s = ( cashdesk_state )malloc( sizeof( struct cashdesk_state ) );
		memset(s,0,sizeof(struct cashdesk_state));
        s->p_ticket = RAM_P_TICKET;
        s->acu_puntos = RAM_ACU_PUNTOS;
        s->acu_puntos_cumple = RAM_ACU_PUNTOS_CUMPLE;

        s->cant_promociones = RAM_CANT_PROMOCIONES;
        s->bonos_a_entregar = RAM_BONOS_A_ENTREGAR;
        s->ptr_prom_cupon = RAM_PTR_PROM_CUPON;
/*
        {
            int i;
            for( i = 0;i < 5;i++ ) {
                s->cupones[i].prom_cupon_formato = _RAM_PROM_CUPON_FORMATO( i );
                s->cupones[i].prom_cupon_cantidad = _RAM_PROM_CUPON_CANTIDAD( i );
                s->cupones[i].prom_cupon_lugar_impresion = _RAM_PROM_CUPON_LUGAR_IMPRESION( i );
                s->cupones[i].prom_cupon_importe = _RAM_PROM_CUPON_IMPORTE( i );
                //s->cupones[i].cupon_leyenda = malloc( strlen( _RAM_CUPON_LEYENDA( i ) ) + 1 );
                //strcpy( s->cupones[i].cupon_leyenda, _RAM_CUPON_LEYENDA( i ) );/ //ver fabian
            }
        }
*/
        #ifdef PROMO_FAULT_TOLERANCE
    }

    /*logCashdeskState( ( void* )s, sizeof( struct cashdesk_state ), s->cupones[0].cupon_leyenda,
                      s->cupones[1].cupon_leyenda, s->cupones[2].cupon_leyenda,
                      s->cupones[3].cupon_leyenda, s->cupones[4].cupon_leyenda );*/ //ver favian

    logCashdeskState( ( void* )s, sizeof( struct cashdesk_state ), "","", "","", "" );//ver fabian
    #endif //PROMO_FAULT_TOLERANCE


    return s;
}

// modifica el estado actual para dejarlo en un estado
// igual a s.
void recover_state( cashdesk_state s )
{
    char tipo;
    int h, pos, cerear_impuestos = 0;
    int eventos_a_deshacer = 0;
	int pos_base = 0;
	double ram_total_antes = 0.0, dd= 0.0;
    //char buffer[200]; //log sacar

    #ifdef PROMO_FAULT_TOLERANCE
    if( recState == recoveringState ) {
    }
    else {
        #endif //PROMO_FAULT_TOLERANCE



		eventos_a_deshacer = RAM_P_TICKET - s->p_ticket - _RAM_PROMOCIONES_AFINIDAD_USADAS(0);
		pos_base = s->p_ticket;
		//pos_base = RAM_P_TICKET - 1;
        //sprintf(buffer,"Motor eventos a deshacer: %i",eventos_a_deshacer);//log sacar
        ///**/glog( buffer ); //log sacar
        SET_MEMORY_DOUBLE( __ram_importe, 0.0 );
		ram_total_antes = RAM_TOTAL;
		//reiniciemos el vector de diferenicas
		i_contador = 0;
        for( h = 0;h < eventos_a_deshacer;h++ ) {
            //pos = RAM_P_TICKET - 1;
			dd = 0.0;
			pos = pos_base + h;
            //sprintf(buffer,"Motor evento a deshacer numero: %i Pos: %i",eventos_a_deshacer,pos);//log sacar
            ///**/glog( buffer ); //log sacar

			if ( BUSCAR_PROMOCION_POR_CODIGO_INTE ) {
				ASIGNA_CODIGO_ESPECIAL( _TICKET_COD_ESPECIAL( pos ) );
			} else {
				ASIGNA_COD_ESPECIAL_COD_BARRA( _TICKET_COD_ESPECIAL( pos ) , _TICKET_COD_BARRA( pos ) );
			}

			//dd = (double) _TICKET_ALICUOTA_PRECIO(pos);
			if( _TICKET_DESCUENTO(pos)  )
				dd = _TICKET_DESCUENTO(pos) + _TICKET_IMP_INT(pos);
			if( dd  )  {
				_SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, pos, dd ); 
				dd = (double) _TICKET_ILA_REC(pos);
				_SET_MEMORY_FLOAT( __ticket_ila_rec, pos , _TICKET_PRECIO( pos ) ); //guardemos el valor original
				_SET_MEMORY_DOUBLE( __ticket_precio, pos, dd ); 
				dd = (double) _TICKET_IHA_REC(pos);
			    _SET_MEMORY_DOUBLE( __ticket_iva1_precio, pos, dd ); 
			} 
			
			SET_MEMORY_DOUBLE( __item_precio, _TICKET_PRECIO( pos ) );
			SET_MEMORY_DOUBLE( __item_precio_sin_iva, _TICKET_IMPORTE_SIN_IVA( pos ) );
			SET_MEMORY_DOUBLE( __item_iva1_precio, _TICKET_IVA1_PRECIO(pos) );
	
			
            SET_MEMORY_DOUBLE( __ram_cantidad, _TICKET_CANTIDAD( pos ) );
			
			SET_MEMORY_DOUBLE( __item_costo, _TICKET_COSTO( pos ) );
            SET_MEMORY_INT( __ram_anulacion, 1 );
            //_SET_MEMORY_CHAR( __ticket_tipo, pos, _TICKET_TIPO( pos ) | _RESTA );
			if( _TICKET_EXENTO( pos ) ) {
				SET_MEMORY_CHAR( __item_gravado, 0 );
				SET_MEMORY_DOUBLE( __item_precio_sin_iva, _TICKET_EXENTO( pos ) );
			} else
				SET_MEMORY_CHAR( __item_gravado, 1 );

			SET_MEMORY_CHAR( __ram_diferencia_precio, 0 );
            ADD_MEMORY_DOUBLE( __ram_importe,
                               ROUND( _TICKET_CANTIDAD( pos )
                                    * ( ITEM_PRECIO + _TICKET_RECARGO( pos ) ), ENTEROS,
                                      DECIMALES ) );
			SET_MEMORY_DOUBLE( __item_iva2_precio, _TICKET_IVA2_PRECIO(pos) );
			SET_MEMORY_DOUBLE( __item_iha_precio, _TICKET_IHA_PRECIO(pos) );
			SET_MEMORY_DOUBLE( __item_ila_precio, _TICKET_ILA_PRECIO(pos) );
			SET_MEMORY_DOUBLE( __item_imp_int, _TICKET_IMP_INT( pos ) );
			
			_SET_MEMORY_FLOAT( __item_monto_impuesto, 0,
                               _TICKET_IMPUESTO1_PRECIO( pos ) );
            _SET_MEMORY_FLOAT( __item_monto_impuesto, 1,
                               _TICKET_IMPUESTO2_PRECIO( pos ) );
            _SET_MEMORY_FLOAT( __item_monto_impuesto, 2,
                               _TICKET_IMPUESTO3_PRECIO( pos ) );
            _SET_MEMORY_FLOAT( __item_monto_impuesto, 3,
                               _TICKET_IMPUESTO4_PRECIO( pos ) );


			SET_MEMORY_CHAR( __item_cod_iva, _TICKET_COD_IVA( pos ) );
			SET_MEMORY_DOUBLE( __ram_cantidad, _TICKET_CANTIDAD( pos ) );
            tipo = _TICKET_TIPO2( pos );
			//ADD_MEMORY_INT( __ram_p_ticket, -1 ); 
            if( _TICKET_COD_EXT( pos ) ) {
                ADD_MEMORY_INT( __ram_p_ext_impuestos, -1 );
                cerear_impuestos = 1;
            }


			//AGREGA_ITEM_AL_TICKET( RAM_P_TICKET, SI, NO, SI, _ORIGEN_PROMOCION );
			AGREGA_ITEM_AL_TICKET( RAM_P_TICKET, SI, NO, SI, _TICKET_ORIGEN( pos ) );

			_SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, RAM_P_TICKET, _TICKET_IMPORTE_SIN_IVA( pos ) );
			_SET_MEMORY_DOUBLE( __ticket_exento, RAM_P_TICKET, _TICKET_EXENTO( pos ) );

			ADD_MEMORY_INT( __ram_p_ticket, 1 );
			SET_MEMORY_DOUBLE( __ram_total, ROUND( RAM_TOTAL - RAM_IMPORTE, 10, DECIMALES ) ); //hmm necesito el ram_total antes de imprimir

            if( DEBE_IMPRIMIR_ITEM() ) {
                IMPRIME_ITEM( RAM_P_TICKET - 1 );
            }
            ADD_MEMORY_DOUBLE( __ram_resto, -1*( ITEM_PRECIO + ITEM_RECARGO) );
			ACTUALIZA_ACUMULADORES_RAM( RAM_P_TICKET - 1 );
            //GRABAR_EVENT_E_TICKET( RAM_P_TICKET - 1 , NO, SI, _ORIGEN_PROMOCION, ( int )tipo );
            GRABAR_EVENT_E_TICKET( RAM_P_TICKET - 1 , NO, SI, _TICKET_ORIGEN( pos ), _TICKET_TIPO( pos ) | _RESTA, NO );
			GRABAR_TICKET( RAM_P_TICKET - 1 );
			//MUESTRA_ITEM_EN_PANTALLA( RAM_P_TICKET - 1 );
			if( IMPRESORA_FISCAL == FISCAL) 
				SUMAR_TOTAL_FAC_CAJA( SI); 

			CR1_MOSTRAR_ITEM( RAM_P_TICKET - 1, 9, NO );
        }
        ///**/glog( "Motor fin for**********" ); //log sacar
	 // lo comente porque lo hago dentro del for	

		SET_MEMORY_DOUBLE( __ram_total,ram_total_antes );
		SET_MEMORY_DOUBLE( __ram_total, ROUND( RAM_TOTAL - RAM_IMPORTE, 10, DECIMALES ) );
		SET_MEMORY_DOUBLE( __ram_importe_total, ROUND( RAM_TOTAL , 10, DECIMALES ) );
		if( NRO_PAGARE == 99){ //quiere decir que hubo promo postpago se corto en pantalla y lo dejamos en ventas
			SET_MEMORY_LONG( __nro_pagare, 0 );
			SET_MEMORY_DOUBLE( __ram_resto, RAM_TOTAL  );
		}
		


        SET_MEMORY_CHAR( __ram_prohibir_mas, 1 );
        SET_MEMORY_INT( __ram_anulacion, 0 );
        ///**/glog( "Motor fin for**********2" ); //log sacar
        SET_MEMORY_DOUBLE( __ram_cantidad, 1 ); //ahv

        //CEREAR_VARIABLES_DE_PROMOCION(  );
        SET_MEMORY_INT( __ram_cant_promociones, s->cant_promociones );
        SET_MEMORY_INT( __ram_bonos_a_entregar, s->bonos_a_entregar );
        SET_MEMORY_INT( __ram_ptr_prom_cupon, s->ptr_prom_cupon );
        //  s->p_ticket = RAM_P_TICKET;
        //  s->acu_puntos = RAM_ACU_PUNTOS;
        //  s->acu_puntos_cumple = RAM_ACU_PUNTOS_CUMPLE;
        SET_MEMORY_DOUBLE( __ram_acu_puntos, s->acu_puntos );
        SET_MEMORY_DOUBLE( __ram_acu_puntos_cumple, s->acu_puntos_cumple );


        //CEREAR_ARRAY( __ram_medios_obligatorios );
		/*
        {
            int i;
           // //glog( "Motor inicio for**********" ); //log sacar
            for( i = 0;i < 5;i++ ) {
            //sprintf(buffer,"Motor seteando ram numero: %i",i);//log sacar
            ////glog( buffer ); //log sacar
                _SET_MEMORY_INT( __ram_prom_cupon_formato, i, s->cupones[i].prom_cupon_formato );
                ////glog( "Motor seteando1**********" ); //log sacar
                _SET_MEMORY_INT( __ram_prom_cupon_cantidad, i, s->cupones[i].prom_cupon_cantidad );
                ////glog( "Motor seteando2**********" ); //log sacar
                _SET_MEMORY_CHAR( __ram_prom_cupon_lugar_impresion, i,
                                  s->cupones[i].prom_cupon_lugar_impresion );
                ////glog( "Motor seteando3**********" ); //log sacar
                _SET_MEMORY_DOUBLE( __ram_prom_cupon_importe, i, s->cupones[i].prom_cupon_importe );
                ////glog( "Motor seteando4**********" ); //log sacar
                //_SET_MEMORY( __ram_cupon_leyenda, i, s->cupones[i].cupon_leyenda ); //ver fabian
            }
        }
	*/
        ///**/glog( "Motor cereando**********" ); //log sacar
        CEREAR_MEMORY( __ram_cupon_leyenda );


        //if( cerear_impuestos )
        //  CEREAR_STRUCT( _ext_impuestos );


        ///**/glog( "Motor elimina base**********" ); //log sacar
        ELIMINA_BASE_DTO( 99 );

        //REVERTIR_EVENTO_DESC_PROMO( MOTIVO_DESC_PROMOCION, 0 );
        //REVERTIR_EVENTO_DESC_PROMO( MOTIVO_DESC_PROMO_GENERAL, 0 );
        //REVERTIR_EVENTO_DESC_PROMO( MOTIVO_CONCEPTOS_SIN_DTOS, 0 );


        //bonos_ant = RAM_BONOS_A_ENTREGAR;
        //bonos_on = 0;         


        //  if(mostrar_item_pantalla) {
        //  MUESTRA_BULTOS(  );
        ///**/glog( "Motor muestra subtotal**********" ); //log sacar
        MUESTRA_SUBTOTAL(NO);
        //  MUESTRA_ITEM_EN_PANTALLA( RAM_P_TICKET - 1 );
        //  }

        #ifdef PROMO_FAULT_TOLERANCE
    }
    #endif //PROMO_FAULT_TOLERANCE

	{
		///// se debe implementar correctamente: es decir, falta hacer una copia
		// del archivo para el caso en que esten funcionando las promos online
		// luego, recuperar el estado significará volver el archivo al estado en 
		// el que se encontraba antes de ejecutar las prepago, osea borrar el archivo
		// y renombrar la copia temporal que se habia hecho.
		if( !RAM_FLAG_COBRO_VENTA ) {
			BORRAR_ARCHIVO_REC(ARCH_REC_ACT_MEDIO_MOTOR); //borra todos los registros
			BORRAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO); //borra todos los registros
			BORRAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO); //borra todos los registros
			BORRAR_ARCHIVO_REC( ARCH_REC_REQUERIR_MEDIO ); //borra todos los registros
			glog( "PROMO: extern.c: Se ELIMINO el archivo de ACTIVACION DE MEDIOS\n",LOG_MOTOR_PROMO,5 );
			glog( "PROMO: extern.c: Se ELIMINO el archivo de BONOS/MONEDERO/ETC.\n" ,LOG_MOTOR_PROMO,5);
			glog( "PROMO: extern.c: Se ELIMINO el archivo de INFORME DE EVENTOS.\n" ,LOG_MOTOR_PROMO,5);
			glog( "PROMO: extern.c: Se ELIMINO el archivo de REQUERIMIENTO DE MEDIOS DE PAGO.\n" ,LOG_MOTOR_PROMO,5);
		}
	}

    glog( "PROMO: extern.c: Estado recuperado\n" ,LOG_MOTOR_PROMO,5);
    return;
}

// imprime todos los elementos del ticket producidos 
// desde el estado s hasta el estado actual.
void redo_actions( cashdesk_state s )
{
    //RAM_P_TICKET = s->ind_ticket;
    fprintf( stdout, "Todas las cosas impresas\n" );
    return;
}


cashdesk_state free_cashdesk_state( cashdesk_state s )
{

    /*for( i = 0;i < 5;i++ ) {
        free( s->cupones[i].cupon_leyenda );
    }*/ //Ver Fabian
    if( s )
    free( s );
    return NULL;
}


cashdesk_state cashdesk_state_set_str( cashdesk_state s, int n, char *str )
{
    /*if( ( n >= 0 ) && ( n < 5 ) ) {
        s->cupones[n].cupon_leyenda = str;
    }*/ //ver fabian
    return s;
}



/**
 * Consulta de Promociones
 */

#define PROMO_CONSULTAS_SQL
#ifdef PROMO_CONSULTAS_SQL
#include "DB.h"
#include "DefTables.h"



/***********************************************************************/
int ManipularPromos(int cant, int *nroPromo )
/***********************************************************************/
{
    int seguir = 1, k, rta = 0, cont = 0, salir = 0;
    int grilla = A_SCROLL_PROMOS;

    ExecuteSAction( grilla, "0" );            // 0 habilita

    while( seguir ) {
        k = GETCH();

        switch( k ) {
            case 27:
                /*-------- ESC -----------*/
                seguir = 0;
                ExecuteSAction( grilla, "-99" ); //-99 deshabilita 
                rta = 0;    //sin accion
                break;
            case 32:
                /*-------- TOTAL ---------*/
            case 13:
                /*-------- ENTER ---------*/ 
                seguir = 0;
                ExecuteSAction( grilla, "-99" ); //-99 deshabilita                
                rta = 1;  //editar
                break;
            case -72:
                /*----- FLECHA ARRIBA -----*/
                ExecuteSAction( grilla, "-1" );
                salir = 0;
                if( cont > 0 ) {
                    rta = 1;
                    cont--;
                }

                break;
            case -80:
                /*------ FLECHA ABAJO -----*/
                ExecuteSAction( grilla, "1" );
                salir = 0;
                if( cont < cant ) {
                    rta = 1;
                    cont++;
                }
                break;
            case 45:
                /*------ '-' -----*/
                seguir = 0;
                rta = 2;  //borrar
        }
    }
    *nroPromo = cont;
    return ( rta );
}

/**
 * Divide el string de entrada en tokens por '\n' y los
 * agrega de a uno como líneas independientes en el campo 
 * 'descripción' de la grilla de consulta de promociones.
 */
 
int agregarDescripcionPromo ( const char* string ) {
	char *str;
	char seps[]   = "\n";
	char *token;

	str = (char*)malloc(strlen(string)+1);
	strncpy(str,string,strlen(string)+1);

	token = strtok( str, seps );
	while( token != NULL )
	{
		agregarPromo( "", token );
		token = strtok( NULL, seps );
	}

	if( str )
	free (str);
	return 0;
}

int consultaPromoSQL( long cod_int, char* cod_bar, long cod_dep, char* cod_class, int incluible_en_promocion ) {
	int rta = 0;
	char promoName[64];
	char promoDesc[256];
	int promoPerfil = 0;
	char dia[20];
	memset(dia,0,sizeof(dia));

	{
		switch ( ( ( (int)get_date() + 6 ) % 7 ) ) {

			case 0:
				_snprintf(dia,20,"lunes");
				break;
			case 1:
				_snprintf(dia,20,"martes");
				break;
			case 2:
				_snprintf(dia,20,"miercoles");
				break;
			case 3:
				_snprintf(dia,20,"jueves");
				break;
			case 4:
				_snprintf(dia,20,"viernes");
				break;
			case 5:
				_snprintf(dia,20,"sabado");
				break;
			case 6:
				_snprintf(dia,20,"domingo");
				break;
		}
		dia[sizeof(dia)-1] = '\0';
	}




	DIRECT_DB_QUERY("SET @caja = %i;",NRO_CAJA);
	DIRECT_DB_QUERY("SET @card = \"%s\";",RAM_CUENTA_CLIENTE);
	DIRECT_DB_QUERY("SET @perfil = %i;",RAM_PERFIL);
	DIRECT_DB_QUERY("SET @hora = CURTIME();");
	DIRECT_DB_QUERY("SET @fecha = CURDATE();");
	DIRECT_DB_QUERY("SET @dia = LOWER(DAYNAME(CURDATE()));");
	DIRECT_DB_QUERY("CREATE TABLE activas_tmp SELECT DISTINCT promos_vigencia.*, cod_perfil FROM promos_vigencia LEFT JOIN promos_participantes USING (cod_promo) WHERE \
		( ( rango_hora ='0' OR ( hora_desde <= @hora AND hora_hasta >= @hora ) ) AND \
			( rango_fecha='0' OR ( fecha_desde <= @fecha AND fecha_hasta >= @fecha ) ) AND \
			( rango_caja='0'  OR ( caja_desde <= @caja AND caja_hasta >= @caja ) ) AND \
			( rango_tarjeta='0' OR ( tarjeta_desde <= @card AND tarjeta_hasta >= @card ) ) AND \
			( perfil='0' OR ( cod_perfil > 0 AND cod_perfil = @perfil ) ) AND \
			( dias='0' OR %s = '1' ) AND \
			( afinidad='0' OR %s = '1' ) \
		);", dia, ( affiliated() ? "1" : "0" ) );
//			( perfil='0' OR cod_perfil = @perfil ) AND \
//			( perfil='0' OR cod_perfil > 0 ) AND \


	/**
	 * Buscar las promociones por exclusión de articulos, departamentos y clasificaciones.
	 */
	DIRECT_DB_QUERY("SET @cod_art_int = %li;", cod_int);
	DIRECT_DB_QUERY("SET @cod_art_bar = %s;", cod_bar);
	DIRECT_DB_QUERY("SET @cod_depart  = %li;", cod_dep);
	DIRECT_DB_QUERY("SET @cod_class_nivel = %s;", cod_class /*cod_class_nivel*/);
	DIRECT_DB_QUERY("SET @cod_class_concept = %s;", cod_class /*cod_class_concepto*/);
	rta = DIRECT_DB_QUERY("SELECT DISTINCT activas_tmp.cod_promo FROM activas_tmp LEFT JOIN promos_participantes USING (cod_promo) WHERE \
	( ( ( tipo='2' OR tipo='8' ) AND ( cod_art_int = @cod_art_int AND cod_art_bar = @cod_art_bar ) ) OR \
		( tipo='4' AND ( cod_dept = @cod_depart ) ) OR \
		( tipo='6' AND ( cod_class_nivel = @cod_class_nivel AND cod_class_concepto = @cod_class_concept ) )  \
		);");

	/**
	 * Eliminar los registros de promociones por exclusión de articulos, departamentos y clasificaciones.
	 */
	// Armar el listado de cod_promo's a eliminar
	if ( rta ) {
		int cod_promo = 0;
		char promos[2*1024];
		memset(promos,0,sizeof(promos));
		do {
			char tmp[10];
			GET_DATO(1,C_INT,(void*)&cod_promo,sizeof(cod_promo));
			if ( strlen(promos) > 0 ) {
				strncat(promos, ", ", sizeof(promos) - strlen(promos) - 1);
			}
			_snprintf(tmp,sizeof(tmp),"%i",cod_promo);
			tmp[sizeof(tmp)-1]='\0';
			strncat(promos, tmp, sizeof(promos) - strlen(promos) - 1);
		} while ( rta = DIRECT_DB_SKIP(1) );

		// Eliminar los registros de activas_tmp
		DIRECT_DB_QUERY("DELETE FROM activas_tmp WHERE cod_promo IN (%s);", promos);
	}

	/**
	 * Consultar promociones asociadas al artículo
	 */
	if ( incluible_en_promocion ) {
	rta = DIRECT_DB_QUERY("SELECT DISTINCT nombre, descripcion, detalle, info FROM activas_tmp p LEFT JOIN promos_participantes p2 USING (cod_promo) WHERE \
		(	( tipo='1' AND ( cod_art_int = @cod_art_int AND cod_art_bar = @cod_art_bar ) ) OR \
			( tipo='2' AND ( cod_art_int <> @cod_art_int OR cod_art_bar <> @cod_art_bar ) ) OR \
			( ( tipo='3' OR tipo='8' ) AND ( cod_dept = @cod_depart ) ) OR \
			( tipo='4' AND ( cod_dept <> @cod_depart ) ) OR \
			( tipo='5' AND ( cod_class_nivel = @cod_class_nivel AND cod_class_concepto = @cod_class_concept ) ) OR \
			( tipo='6' AND ( cod_class_nivel <> @cod_class_nivel OR cod_class_concepto <> @cod_class_concept ) ) OR \
			( tipo='7' ) \
		);");
	} else {
		rta = DIRECT_DB_QUERY("SELECT DISTINCT nombre, descripcion, detalle, info FROM activas_tmp p LEFT JOIN promos_participantes p2 USING (cod_promo) WHERE \
			(	( tipo='7' ) );");
	}

	if ( rta ) {
		char detalle[4 * 1024];
		char info[4 * 1024];
		do {
			GET_DATO(1,C_CHAR,promoName,64);
			GET_DATO(2,C_CHAR,promoDesc,256);
			agregarPromo( promoName, promoDesc );

			GET_DATO(3,C_CHAR,detalle,sizeof(detalle));
			if ( strlen(detalle) > 0 ) {
				agregarDescripcionPromo( detalle );
			}

			GET_DATO(4,C_CHAR,info,sizeof(info));
			if ( strlen(info) > 0 ) {
				agregarDescripcionPromo( info );
			}

		} while ( rta = DIRECT_DB_SKIP(1) );
	} else {
		agregarPromo( "", "No hay promociones activas asociadas al artículo" );
	}

	DIRECT_DB_QUERY("drop table if exists activas_tmp;");
	return 0;
}

int consultaPromoPostPago( int cod_promo ) {

	int rta =0 ;

	DIRECT_DB_QUERY("SET @fecha = CURDATE();");
	DIRECT_DB_QUERY("SET @caja = %i;",NRO_CAJA);
	rta = DIRECT_DB_QUERY("SELECT * FROM DBTPVIV.promos_vigencia  where  cod_promo = %i and detalle like \"%%POSTPAGO%%\" and  ( rango_fecha='0' OR ( fecha_desde <= @fecha AND fecha_hasta >= @fecha ) ) AND  ( rango_caja='0'  OR ( caja_desde <= @caja AND caja_hasta >= @caja ) )",cod_promo );
    return rta;
}


int consultaPromociones( void ) {
	int rta = 0, i = 0;

	int nroPromo = 0;
	long codigo_barra = 0;
	char cod_barra[81];

	ExecuteAction( A_ACTIVAR_PANEL_CONSULTA_PROMOS );

	do {
		_PERMITIR_SCANNER = SI;
		ExecuteAction( A_MUESTRA_DATO_COD_BARRA_PROMO );
		memset( cod_barra, 0, sizeof( cod_barra ) );
		codigo_barra = PEDIR_CADENA_NUMERICA( ( char )( 51 - ancho_panel / 2 ),
				( char )0, ( char )80, cod_barra, NULL, 0, SI, NO , NULL);
		_PERMITIR_SCANNER = NO;

		if ( !codigo_barra ) {
			rta = 0;
		} else if ( codigo_barra && BUSCA_CODIGO_BARRA( cod_barra ) ) {
			consultaPromoSQL( articulo.cod_interno, articulo.cod_barra, articulo.cod_dpto, articulo.cod_clasificacion, ((articulo.incluible_promocion=='S') || (articulo.incluible_promocion=='s')) );
			rta = ManipularPromos(i,&nroPromo);
			clearPromos( );
		} else {
			MENSAJE_STRING( S_CODIGO_INEXISTENTE );
			MENSAJE( "" );
			rta = 1;
		}
	} while ( rta );


    ExecuteAction( A_DESACTIVAR_PANEL_CONSULTA_PROMOS );       

	return 0;
}
/* Esta promocion se usa para poder mezclar articulos en la promo mxn ya que el xpromo solo lo hace
con articulos iguales es decir una 3x2 tienen que ser 3 de mismo codigo de barra en cambio esta la hace la caja
mezclando 3 de una lista
*/
int PromoSQL_MxN( long id_eventos ) {

	int rta =0 ;
	int cod_promo =0 ;
	char sql[300];
	int rta2 =0, cant,cnt=0;
	int cod_int = 0; //tiene que ser un vector
	int cantidad =0, auxcant = 0, primercant= 0, cant_registros = 0  ;
	char detallepromo[100];
	char *p1;
	int cant_lineas =0;
	char dato[50];
	int por_cada, beneficiamos;
	float altantoporciento;
	char mensajepromo[100];
	double precio, psiva, exento, iva,iint;
	char cod_barra[15];
	struct eventos_promociones motor_event_promo;
	int promos_activadas[50];
	int promos_candidatas[50];
	int indice =0, ciclo=0;
	char dia[20];

	memset(sql,0,sizeof(sql));
	memset(dia,0,sizeof(dia));

	{
		switch ( ( ( (int)get_date() + 6 ) % 7 ) ) {

			case 0:
				_snprintf(dia,20,"lunes");
				break;
			case 1:
				_snprintf(dia,20,"martes");
				break;
			case 2:
				_snprintf(dia,20,"miercoles");
				break;
			case 3:
				_snprintf(dia,20,"jueves");
				break;
			case 4:
				_snprintf(dia,20,"viernes");
				break;
			case 5:
				_snprintf(dia,20,"sabado");
				break;
			case 6:
				_snprintf(dia,20,"domingo");
				break;
		}
		dia[sizeof(dia)-1] = '\0';
	}

	//primero saber si existe la promo
	//para ello ejecutemos el procedimiento almacenado en la mysql
	/*CREATE PROCEDURE getPromoActivaSQL(
    IN _dia VARCHAR(20),
	IN _afinidad INT
	)*/
	//de esta manera CALL dbtpviv.getPromoActivaSQL('jueves', 0, 'depto_descuento_tkt');
	//seteamos variables de session en el mysql asi hace la consulta con esas variable

	
	DIRECT_DB_QUERY("DROP TABLE IF EXISTS `activas_tmp`");
	
	DIRECT_DB_QUERY("SET @caja = %i;",NRO_CAJA);
	DIRECT_DB_QUERY("SET @card = \"%s\";",RAM_CUENTA_CLIENTE);
	DIRECT_DB_QUERY("SET @perfil = %i;",RAM_PERFIL);

	_snprintf(sql,sizeof(sql)-1,"CALL getPromoActivaSQL('%s', %i, 'm_x_n_articulos_distintos_multiple_tkt')",dia,RAM_CLIENTE_AFINIDAD?  1 : 0 );
	rta = DIRECT_DB_QUERY(sql);
/*	rta = DIRECT_DB_QUERY( "CREATE TABLE activas_tmp SELECT DISTINCT promos_vigencia.*, cod_perfil FROM dbtpviv.promos_vigencia  LEFT JOIN dbtpviv.promos_participantes USING (cod_promo)  \
		WHERE (( nombre = '%s') \
        and (( rango_hora ='0') OR ( rango_hora <= CURTIME() AND rango_hora >= CURTIME()))\
       and ((rango_fecha='0')  OR ( fecha_desde <= CURDATE() AND fecha_hasta >= CURDATE()))\
       and ((rango_tarjeta='0' OR ( tarjeta_desde <= @card AND tarjeta_hasta >= @card)))\
      and ( (perfil='0' OR ( perfil > 0 AND cod_perfil > 0 AND cod_perfil = @perfil)))\
      and ((rango_caja='0'  OR ( caja_desde <= @caja AND caja_hasta >= @caja )))\
	  and ((dias='0' OR  ( %s  = '1'))))","depto_descuento_tkt",dia);
*/
	DIRECT_DB_QUERY("show databases;");
 

	rta = DIRECT_DB_QUERY("SELECT DISTINCT activas_tmp.cod_promo FROM activas_tmp");

	do{
    		if ( rta && indice < 50 ) {
				GET_DATO( 1, C_INT, (char *)&cod_promo , sizeof( cod_promo  ) );
				promos_candidatas[indice] = cod_promo;
				indice++;
			} 
     } while ( rta = DIRECT_DB_SKIP(1) ); //dow while
	 if( indice == 0) 
		 return 0;
	 //ya tenemos la lista de promos candidatas, ahora a recorrerlas
	 while(ciclo < indice) {
			// busquemnos si ya se activo para no activarla de nuevo
				if(promos_candidatas[ciclo] &&((cant = CANTIDAD_REGISTROS_REC(ARCH_REC_EVENT_PROMO,sizeof(struct eventos_promociones))) > 0)){
					while(cnt < cant && ciclo < 50){
						memset(&motor_event_promo,0,sizeof( struct eventos_promociones ));
						RECUPERAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO,(char *)&motor_event_promo,sizeof(struct eventos_promociones),cnt);
						promos_activadas[cnt] = motor_event_promo.cod_promo;
						if( promos_candidatas[ciclo] == promos_activadas[cnt]) { //ya se activo
							cod_promo =0;  
							promos_candidatas[ciclo]= 0; 
							break; //salimos del while
						}
						cnt++;
					}
				}

			   //debemos saber las cantidades y si existen esos articulos
			   //select sum(e.cantidad) from dbtps.ev_cont e, dbtps.promos_participantes p where id_evento = 545 and e.cod_articulo = p.cod_art_int and e.cantidad > 0 and origen <> 4
				if( promos_candidatas[ciclo] > 0 ) {
					   int conta = 0 ;
						memset(sql,0,sizeof(sql));
				//		_snprintf(sql, sizeof(sql)-1,"select sum(e.cantidad) from dbtps.ev_cont e, dbtps.promos_participantes p where id_evento = %li and e.cod_articulo = p.cod_art_int and e.cantidad > 0 and origen <> 4 and p.cod_promo = %i", id_eventos, cod_promo);
    					_snprintf(sql, sizeof(sql)-1,"select sum(e.cantidad) from ev_cont e, promos_participantes p where id_evento = %li and e.cod_articulo = p.cod_art_int and  origen <> 4 and p.cod_promo = %i group by p.cod_art_int order by e.posicion",id_eventos, promos_candidatas[ciclo]);
						rta = DIRECT_DB_QUERY( sql); //aqui pueden venir muchas lineas
					do{
						if ( rta ) {
							GET_DATO( 1, C_INT, (char *)&auxcant , sizeof( auxcant  ) );
							cantidad += auxcant;
							if( conta == 0) {
								conta = 1;
								primercant = auxcant;
							}
							cant_registros++;
						}
					} while ( rta = DIRECT_DB_SKIP(1) );
				
				
				} else
					return 0;
				// ahora veamos el articulo disparante, tomamos el primero porque se supone igual precio
				if( cantidad>0 &&  promos_candidatas[ciclo] > 0 ) {
					memset(sql,0,sizeof(sql));
					_snprintf(sql, sizeof(sql)-1,"SELECT cod_art_int, cod_art_bar FROM promos_participantes p where cod_promo = %i and cod_art_int > 0 ;", promos_candidatas[ciclo]);
  					rta = DIRECT_DB_QUERY( sql);
					if ( rta ) {
						GET_DATO( 1, C_INT, (char *)&cod_int, sizeof( cod_int  ) );
						GET_DATO( 2, C_CHAR, cod_barra, sizeof( cod_barra ) );
					}
				} else
					cantidad =0;

				// debemos saber cuantos es son los articulos disparantes y cuantos los beneficios y de paso al cuanto % y el articulo de descarga, y el mensaje
				// promo_vigencia.detalle frmato  6;1;100;100;PROMOCION 6X5 CERVEZAS;  esto es por cada 6, beneficiamos 1 al 100% en el articulo 100.
				 // update dbtps.promos_vigencia set detalle = "6;1;100;100;PROMOCION 6X5 CERVEZAS;" where nombre = "m_x_n_articulos_iguales_multiple_tkt"
			   //SELECT detalle FROM dbtps.promos_vigencia p where cod_promo =468;
				if( cantidad > 0 && promos_candidatas[ciclo] > 0 ) {
					memset(sql,0,sizeof(sql));
    				 _snprintf(sql, sizeof(sql)-1,"SELECT detalle FROM promos_vigencia p where cod_promo =%i", promos_candidatas[ciclo]);
  					rta = DIRECT_DB_QUERY( sql);
					if ( rta ) {
						GET_DATO( 1, C_CHAR, detallepromo, sizeof( detallepromo ) );
					}
					p1 = strtok( detallepromo , ";" );
						while( p1 != NULL && cant_lineas < 5) {   
							strncpy( dato, p1, 40 );
							//
							switch(cant_lineas) {
								
								case 0: //por cada
									por_cada = atoi(dato);
									break;
								case 1: //beneficio
									beneficiamos = atoi(dato);
									break;
								case 2: //al tanto porciento //en uaa la nueva plantilla viene de 0.0 a 1.0( 100%(
									altantoporciento = atof(dato) *100;
										break;
								case 3:  //Articulo descarga
									 if(atol(dato) > 1)
										 cod_int =atol(dato);
									 break;
								case 4: //mensaje
									strncpy(mensajepromo,dato,sizeof(mensajepromo)-1);

							}
							p1 = strtok( NULL, ";"  );
							cant_lineas++;
						}

				}

				if( cant_lineas == 5 && cantidad && por_cada && (cantidad >= por_cada)){ //sino tengo todas las lineas no aplico la promo
					//la promocion veamos el precio
				/*	memset(sql,0,sizeof(sql));
    				_snprintf(sql, sizeof(sql)-1,"SELECT total FROM dbtps.ev_cont where id_evento = %li && cod_articulo = %i", id_eventos,cod_int ); 
  					rta = DIRECT_DB_QUERY( sql);
					if ( rta ) {
						GET_DATO(1,C_DOUBLE,(char *)&precio,8);
					}*/
					precio =1 ;
				} else
					precio = 0;
				cant_lineas = 0;
				if( precio > 0){ //39.00 suponemos al 100%, generemos el descuento al 100%
					//saquemos los importes para calcular el descuento
					struct _acu_iva acu_gravados[MAX_COD_IVA];
					int cant_combosposibles = 0;
					int cantiss = 0;
					memset(sql,0,sizeof(sql));
//original					_snprintf(sql, sizeof(sql)-1,"select sum(cantidad) from ev_cont e, promos_participantes p where id_evento = %li and e.cod_articulo = p.cod_art_int and e.cantidad > 0 and origen <> 4 and p.cod_promo = %i limit %i", id_eventos,promos_candidatas[ciclo],100/*por_cada*/  ); 
					_snprintf(sql, sizeof(sql)-1,"select sum(cantidad) from ev_cont e, promos_participantes p where id_evento = %li and e.cod_articulo = p.cod_art_int and origen <> 4 and p.cod_promo = %i limit %i", id_eventos,promos_candidatas[ciclo],100/*por_cada*/  ); 
					 rta = DIRECT_DB_QUERY( sql);
					 GET_DATO(1, C_INT, (char *)&cantiss, sizeof( cantiss  ) );
					 cant_combosposibles = (int) cantiss / por_cada; // aca da si es 3x1 y tickeo 4 aca me da 1.


					_snprintf(sql, sizeof(sql)-1,"select  importe_sin_iva as PSIVA, exento  as EXENTO, iva1 as IVA, imp_int AS IINT from ev_cont e, promos_participantes p where id_evento = %li and e.cod_articulo = p.cod_art_int and e.cantidad > 0 and origen <> 4 and p.cod_promo = %i", id_eventos ,promos_candidatas[ciclo]); 
					 rta = DIRECT_DB_QUERY( sql);
					 if ( rta && cant_combosposibles >0 && beneficiamos > 0) {
						 char *msg[1];
						GET_DATO(1,C_DOUBLE,(char *)&psiva,8);
						GET_DATO(2,C_DOUBLE,(char *)&exento,8);
						GET_DATO(3,C_DOUBLE,(char *)&iva,8);
						GET_DATO(4,C_DOUBLE,(char *)&iint,8);
			// veamos sobre que articulo descargamoz

						memset( &acu_gravados[0], 0, sizeof( acu_gravados ) );
						acu_gravados[0].exento = (exento  * cant_combosposibles * (altantoporciento /100)) * beneficiamos;
						acu_gravados[0].iva1 = (iva  * cant_combosposibles * (altantoporciento /100)) * beneficiamos;
						acu_gravados[0].imp_int =  (iint  * cant_combosposibles * (altantoporciento /100)) * beneficiamos;
						acu_gravados[0].importe_sin_iva = (((psiva *cant_combosposibles ) + acu_gravados[0].imp_int )* (altantoporciento /100)) * beneficiamos;

						BUSCA_CODIGO_ESPECIAL( cod_int  );
						
						_AGREGAR_PROMOCION_DESCUENTO( &acu_gravados[0], cod_int /*305816*/, articulo.cod_barra /* "7793147118822"*/, 100, 0, 1,
					  				  0, _ORIGEN_PROMOCION_DESC, 0, 0 );
			        
						msg[0] = (char*)malloc(51);
						sprintf(msg[0],mensajepromo);
						MOSTRAR_MENSAJE_POP_UP("MENSAJE DE PROMOCIONES",msg,1);
						free( msg[0] );
						msg[0] = NULL;
						
						{ //guardemos para la evbpromo
							struct eventos_promociones ev_promo;
							double descuento = acu_gravados[0].exento + acu_gravados[0].iva1 + acu_gravados[0].imp_int + acu_gravados[0].importe_sin_iva;
							memset(&ev_promo,0, sizeof( struct eventos_promociones));
							
							ev_promo.b_promo = BENEF_ARTICULOS;
							ev_promo.cod_promo = promos_candidatas[ciclo];
							strncpy(ev_promo.cod_promo_ext, "200", sizeof(ev_promo.cod_promo_ext)-1);
							ev_promo.id_grupo = 0;
							ev_promo.cantidad = cant_combosposibles; //ojo aca que  es otra cosa creo que es la cantidad disparante
							ev_promo.cod_arti = cod_int;
							strncpy(ev_promo.cod_barr, cod_barra, sizeof(ev_promo.cod_barr)-1);
						    GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo, sizeof(struct eventos_promociones));
						//beneficio
						memset(&ev_promo,0, sizeof( struct eventos_promociones));
					 	ev_promo.b_promo = BENEF_DESCUENTO;
						ev_promo.cod_promo = promos_candidatas[ciclo];
						strncpy(ev_promo.cod_promo_ext, "200", sizeof(ev_promo.cod_promo_ext)-1);
						ev_promo.precio_fijo = descuento;
						GUARDAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&ev_promo, sizeof(struct eventos_promociones));
						//

						}




					}

					//aca va el descuento
				}
	ciclo++;	
	}//end while
	
 return rta;
}

#endif //PROMO_CONSULTAS_SQL



#else // INVEL_W
// Lo que sigue es solo para DEBUG en linux.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extern.h"
#include "uprinter.h"
#include "types2.h"
#include "libcache.h"

struct cashdesk_state
{
    long ind_ticket; // indice del ultimo elemento del ticket.
    long *points;
    long *bonus;
};

// devuelve una estructura que representa al estado actual de la caja
cashdesk_state actual_state( void )
{
    cashdesk_state s;
    s = ( cashdesk_state )malloc( sizeof( struct cashdesk_state ) );
    s->ind_ticket = 7; //RAM_P_TICKET; ///sanata
    return s;
}

// modifica el estado actual para dejarlo en un estado
// igual a s.
void recover_state( cashdesk_state s )
{
    //RAM_P_TICKET = s->ind_ticket;
    fprintf( stdout, "Estado recuperado\n" );
    return;
}

// imprime todos los elementos del ticket producidos 
// desde el estado s hasta el estado actual.
void redo_actions( cashdesk_state s )
{
    //RAM_P_TICKET = s->ind_ticket;
    fprintf( stdout, "Todas las cosas impresas\n" );
    return;
}

cashdesk_state free_cashdesk_state( cashdesk_state s )
{
    if( s )
    free( s );
    return NULL;
}

/*
void credit(expr article, expr num) {
    fprintf(stdout,"credit(");
    up_expr(article,0);
    fprintf(stdout,",");
    up_expr(num,0);
    fprintf(stdout,")\n");
}
*/
void credit( identifier article, double num, expr e)
{
  expr e1 = NULL;
  artIdent art;

  if (e) {
    if (e->tag == AMSET) {
      e1 = ref_e ( clon_amset(e) );

      fprintf( stdout, "new_credit(article %s,%f)\n", article, num );
      while (get_amset(e1, &art)) {
	fprintf( stdout, "el credit afecta a %.2lf articulo(s) x%li_%s\n", art.cant, art.codInt, art.codBar );    
      }
      
      unref_e(e1);
    } else {
      fprintf( stdout, "credit: ERROR en el 3er argumento\n" );
    }
  } else {
    // comportamiento anterior, es decir, se aplica el descuento sobre todos los articulos del ticket.
    fprintf( stdout, "credit(article %s,%f)\n", article, num );
    fprintf( stdout, "el credit afecta a TODOS los articulos\n" );    
  }
}

void distribute( identifier article, double cant, double val )
{
    fprintf( stdout, "distribute(article %s,%lf,%lf)\n", article, cant, val );
}

//void issueBonus(identifier bonus, double num) {
void issueBonus( identifier target, identifier type, double valor, double cantidad, char *leyenda, idate vencimiento, double validez )
{
	char buff[64];
	strdate( buff, 64, vencimiento, 0 );

    fprintf( stdout, "issueBonus(bonus (%s,%s,%f,%s,%s,%lf),%f)\n", target, type, valor, leyenda, buff, validez,
             cantidad );
}


void givePoints( identifier point, double num )
{
    fprintf( stdout, "givePoints(point %s,%f)\n", point, num );
}


void activateCard( long promoId, identifier submedio, identifier plan, double num )
//void activateCard( identifier submedio, identifier plan, double num )
{
    fprintf( stdout, "activateCard(submedio %s, plan %s, %lf)\n", submedio, plan, num );
}
void activateCurrency( identifier ident, double num )
{
    fprintf( stdout, "activateCurrency(ident %s, %lf)\n", ident, num );
}
void activateMutual( identifier ident, double num )
{
    fprintf( stdout, "activateMutual(ident %s, %lf)\n", ident, num );
}
void activateCheque( identifier ident, double num )
{
    fprintf( stdout, "activateCheque(ident %s, %lf)\n", ident, num );
}
void activateTicket( identifier ident, double num )
{
    fprintf( stdout, "activateTicket(ident %s, %lf)\n", ident, num );
}
void activateCash( double num )
{
    fprintf( stdout, "activateCash(%lf)\n", num );
}


// MODO 0 -> Se debe pagar todo el ticket con alguno de los medios del grupo.
// MODO 1 -> El monto pagado con los medios del mismo grupo debe cubrir el total del ticket.
// MODO 2 -> Se debe pagar al menos $'num' con alguno de los medios del grupo.
// MODO 3 -> El monto pagado con los medios del mismo grupo debe cubrir al menos $'num'.
void reqCard( int group, int modo, identifier submedio, identifier plan, double num ) {
  fprintf( stdout, "requireCard(%s,%s,%lf)[%i]\n", submedio, plan, num, group );
}

void reqCurrency( int group, int modo, identifier ident, double num ) {
  fprintf( stdout, "requireCurrency(%s,%lf)[%i]\n", ident, num, group );
}

void reqMutual( int group, int modo, identifier ident, double num ) {
  fprintf( stdout, "requireMutual(%s,%lf)[%i]\n", ident, num, group );
}

void reqCheque( int group, int modo, identifier ident, double num ) {
  fprintf( stdout, "requireCheque(%s,%lf)[%i]\n", ident, num, group );
}

void reqTicket( int group, int modo, identifier ident, double num ) {
  fprintf( stdout, "requireTicket(%s,%lf)[%i]\n", ident, num, group );
}

void reqCash( int group, int modo, double num ) {
  fprintf( stdout, "requireCash(%lf)[%i]\n", num, group );
}


void recArt( long promoId, char* promoIdExt, identifier art, double cant, long grupo )
{
    fprintf( stdout, "rec_Articles(promo=%li [%s], article=%li, cantidad=%lf, grupo=%li)\n", promoId, promoIdExt, atol( &art[1] ), cant, grupo );
}

void recCash( long promoId, char* promoIdExt, long grupo )
{
    fprintf( stdout, "rec_Cash(promo=%li [%s], grupo=%li)\n", promoId, promoIdExt, grupo );
}

void recCtacte( long promoId, char* promoIdExt, long grupo )
{
    fprintf( stdout, "rec_CuentaCorriente(promo=%li [%s], grupo=%li)\n", promoId, promoIdExt, grupo );
}

void recMutual( long promoId, char* promoIdExt, identifier name, long grupo )
{
    fprintf( stdout, "rec_Mutual(promo=%li [%s], Mutual=%s, grupo=%li)\n", promoId, promoIdExt, name, grupo );
}

void recTicket( long promoId, char* promoIdExt, identifier name, long grupo )
{
    fprintf( stdout, "rec_Ticket(promo=%li [%s], Ticket=%s, grupo=%li)\n", promoId, promoIdExt, name, grupo );
}

void recCheque( long promoId, char* promoIdExt, identifier name, long grupo )
{
    fprintf( stdout, "rec_Cheque(promo=%li [%s], Cheque=%s, grupo=%li)\n", promoId, promoIdExt, name, grupo );
}

void recCurrency( long promoId, char* promoIdExt, identifier name, long grupo )
{
    fprintf( stdout, "rec_Currency(promo=%li [%s], Moneda=%s, grupo=%li)\n", promoId, promoIdExt, name, grupo );
}

void recCard( long promoId, char* promoIdExt, identifier name, identifier plan, long grupo )
{
    fprintf( stdout, "rec_Card(promo=%li [%s], card_Name=%s, plan=%s, grupo=%li)\n", promoId, promoIdExt, name, plan,
             grupo );
}

void recAll( long promoId, char* promoIdExt, long grupo )
{
    fprintf( stdout, "rec_All(promo=%li [%s], grupo=%li)\n", promoId, promoIdExt, grupo );
}


void recCredit( long promoId, char* promoIdExt, double cant, long grupo )
{
    fprintf( stdout, "rec_Credit(promo=%li [%s], cantidad=%lf, grupo=%li)\n", promoId, promoIdExt, cant, grupo );
}

void recBonus( long promoId, char* promoIdExt, identifier type, double value, double cant, long grupo )
{
    fprintf( stdout, "rec_Bonus(promo=%li [%s], tipo=%s, valor=%lf, cantidad=%lf, grupo=%li)\n", promoId, promoIdExt,
             type, value, cant, grupo );
}

void recPoints( long promoId, char* promoIdExt, identifier point, double cant, long grupo )
{
    fprintf( stdout, "rec_Points(promo=%li [%s], punto=%s, cantidad=%lf, grupo=%li)\n", promoId, promoIdExt, point,
             cant, grupo );
}

void recActivateCard( long promoId, char* promoIdExt, identifier name, identifier plan, double monto, long grupo )
{
    fprintf( stdout, "rec_ActivateCard(promo=%li [%s], card_Name=%s, plan=%s, monto=%lf, grupo=%li)\n",
             promoId, promoIdExt, name, plan, monto, grupo );
}


void recLog( identifier target, char *leyenda) 
{
	fprintf(stdout,"recLog: %s - %s\n", target, leyenda);
}

char* expr2str(expr e) {
  char *s;
  if (e) {
    s = (char*)malloc(256);
    memset( s, ( int )'\0', 256 );
    switch(e->tag) {
    case CASH:
      sprintf(s,"Efectivo");
      break;
    case CARD:
      sprintf(s,"Tarjeta de Credito [%s,%s]", e->e.card.name, e->e.card.plan);
      break;
    case TICKET:
      sprintf(s,"Ticket [%s]", e->e.name);
      break;
    case MUTUAL:
      sprintf(s,"Mutual [%s]", e->e.name);
      break;
    case CHEQUE:
      sprintf(s,"Cheque [%s]", e->e.name);
      break;
    case CURRENCY:
      sprintf(s,"Moneda Extranjera [%s]", e->e.name);
      break;
    case NUM:
      {
	long a = (long) e->e.num;
	if ( e->e.num - (double) a == 0.0 ) {
	  sprintf( s, "%li", a );
	} else {
	  sprintf( s, "%.2lf", e->e.num );
	}
      }
      break;
    default:
      sprintf(s,"Expresion_Desconocida");
      break;
    }
    return s;
  } else {
    return NULL;
  }
}

int inDepartment( identifier art, identifier dep )
{
  long r = get_dpto_cache ( atol(&art[1]), 1 );
  if (r >= 0) {
    // Valor almacenado en Cache
    return (r == atol(&dep[1]));
  } else {
    // Valor obtenido de BBDD
  return (art[1]==dep[1]?1:0); 
  }

}

int inClass( identifier art, identifier nivel, identifier concept )
{
    return ( art[1] % 2 == ( nivel[1] + concept[1] ) % 2 ? 1 : 0 );
}


double amount( identifier article, double n )
{
  /*
    // e1 = articulo, n = cantidad de articulos
  double r = get_cost_cache ( atol(&article[1]), 1 );
  if (r >= 0) {
    // Valor almacenado en Cache
  fprintf(stdout,"AMOUNT->CACHE\n");
    return r*n;
  } else {
    // Valor obtenido de BBDD
  fprintf(stdout,"AMOUNT->BBDD\n");
    return n;
  }

  */


  long codInt = 0;
  char codBar[17] = "";
  double precio = 0;
  double r;
  
  switch ( getCodsArt( article, &codInt, codBar, &precio) ) {
  case COD_INTERNO:
    r = get_cost_cache ( codInt, 0 );

    if (r >= 0) {
      // Valor almacenado en Cache
      //{ char buff[256];sprintf( buff, "PROMO: AMOUNT_OBTENIDO_DE_CACHE(%s,%lf) = %lf", article, n, r*n );glog( buff ); }
      
      return r * n;
    } else {
      // Valor obtenido de BBDD
      return n;
    }
    break;
    
  case COD_INTERNO_PRECIO:
    return precio * n;
    break;
    
  case COD_BARRA:
	#ifdef WIN32
	    r = get_cost_cache ( 0, _atoi64( codBar ) );
	#else
    r = get_cost_cache ( 0, atoll( codBar ) );
	#endif
    
    if (r >= 0) {
      // Valor almacenado en Cache
      //{ char buff[256];sprintf( buff, "PROMO: AMOUNT_OBTENIDO_DE_CACHE(%s,%lf) = %lf", article, n, r*n );glog( buff ); }
      
      return r * n;
    } else {
      // Valor obtenido de BBDD
      return n;
    }
    break;
    
  case COD_BARRA_PRECIO:
    return precio * n;
    break;
    
  case COD_INTERNO_BARRA:
	#ifdef WIN32
	    r = get_cost_cache ( codInt, _atoi64( codBar) );
	#else
    r = get_cost_cache ( codInt, atoll( codBar) );
	#endif
    
    if (r >= 0) {
      // Valor almacenado en Cache
      //{ char buff[256];sprintf( buff, "PROMO: AMOUNT_OBTENIDO_DE_CACHE(%s,%lf) = %lf", article, n, r*n );glog( buff ); }
      
      return r * n;
    } else {
      // Valor obtenido de BBDD
      return n;
    }
    break;
    
  case COD_INTERNO_BARRA_PRECIO:
    return precio * n;
    break;
    
  case COD_ERROR:
    //glog( "PROMO: Amount: No se encontró el artículo. Se devuelve precio 0." );
  return 0;
  break;
  default:
    //glog( "PROMO: Amount: No se encontró el artículo. Se devuelve precio 0." );
  return 0;
  break;
  
  }
  
  


}

int affiliated()
{
    return 1;
}

idate dateofbirth()
{
    idate d;
    if( affiliated() ) {
        if( mk_date( &d, "1950-3-1" ) == 0 ) {
            return d;
        }
        else {
            return getdate_();
        }
    }
    else {
        return getdate_();
    }
}

int holiday()
{
    return 0;
}

expr means()
{
    expr e;
    e = new_means();
    ins_card( e, "visa", "p0", 100 );
    ins_card( e, "maestro", "p6", 50 );
    ins_card( e, "provencred", "p0", 50 );
    ins_cash( e, 75 );
    return e;
}

expr change()
{
    expr e;
    e = new_means();
    ins_card( e, "visa", "p0", 10 );
    ins_card( e, "maestro", "p6", 5 );
    ins_cash( e, 5 );
    return e;
}

expr returned()
{
    expr e;
    e = new_means();
    ins_cash( e, 20 );
    return e;
}




/*
expr purchase() { 
  expr e; 
  e = new_amset(); 
  ins_amset(e,"azucar",2);  
  ins_amset(e,"yerba",1); 
  ins_amset(e,"jabon",3);  
  ins_amset(e,"arroz",1); 
  ins_amset(e,"tomate",0.25);  
  ins_amset(e,"manzana",1.5); 
  ins_amset(e,"tomate",0.5);  
  ins_amset(e,"mayonesa",1); 
  ins_amset(e,"fideos",2); 
  ins_amset(e,"spaghetti",0.8);
  ins_amset(e,"marucha",2.5);
  ins_amset(e,"costeleta",1.2);
  ins_amset(e,"papa",3.3);
  ins_amset(e,"lampara",1);
  ins_amset(e,"dentrifico",1);
  ins_amset(e,"bondiola",0.4);
  ins_amset(e,"zanahoria",0.25);
  ins_amset(e,"huevos",12);
  ins_amset(e,"mermelada",0.3);
  ins_amset(e,"jabon",3);
  ins_amset(e,"galletitas",6);
  ins_amset(e,"cebolla",1.5);
  ins_amset(e,"manteca",0.4);
  ins_amset(e,"pimienta",0.025);
  ins_amset(e,"sal",0.2);
  ins_amset(e,"leche",2);
  ins_amset(e,"dulce",0.5); 
  ins_amset(e,"azucar",5);
  ins_amset(e,"harina",2); 
  ins_amset(e,"mentoplus",2);
  ins_amset(e,"picadillo",2);
  return e;
}
*/

expr buyed_articles()
{
    expr e; 
    e = new_amset(); 
    ins_amset( e, "azucar", 2 );  
    ins_amset( e, "yerba", 1 ); 
    ins_amset( e, "jabon", 3 );  
    return e;
}

expr delivered_articles()
{
    expr e; 
    e = new_amset(); 
    ins_amset( e, "art_entregado_1", 2 );  
    ins_amset( e, "art_entregado_2", 1 ); 
    ins_amset( e, "art_entregado_3", 3 );  
    return e;
}

expr discounts_credits()
{
    expr e; 
    e = new_amset(); 
    ins_amset( e, "art_descarga_1", 2 );  
    ins_amset( e, "art_descarga_2", 1 ); 
    ins_amset( e, "art_descarga_3", 3 );  
    return e;
}



double num_rnv( identifier ident )
{
    return 1.3;
}

int bool_rnv( identifier ident )
{
    return 1;
}

/*
 * Esta funcion debe devolver una copia del string en un nuevo espacio de memoria.
 * El evaluador se encarga de hacer free cuando corresponda.
 */
char * string_rnv( identifier ident )
{
    return strcpy( ( char* )memset( malloc( 20 ), '\0', 20 ), "Feliz Navidad !!!" );
}

long date_rnv( identifier ident )
{
    return 964;
}

long time_rnv( identifier ident )
{
    return 765;
}

void num_rnv_update( identifier ident, double f )
{
    fprintf( stdout, "NUM_RNV_UPDATE: %s = %.2f\n", ident, f );
}

void bool_rnv_update( identifier ident, int b )
{
    fprintf( stdout, "BOOL_RNV_UPDATE: %s = %s\n", ident, ( b == FALSE ? "FALSE" : "TRUE" ) );
}

/*
 * Esta funcion debe hacer free del string que toma por parametro cuando deje de
 * utilizar dicho string.
 */
void string_rnv_update( identifier ident, char *b )
{
    fprintf( stdout, "STRING_RNV_UPDATE: %s = %s\n", ident, b );
    if( b )
    free( b );
}

void date_rnv_update( identifier ident, long b )
{
    char buf[20];
    strdate( buf, 20, b, 0 );
    fprintf( stdout, "DATE_RNV_UPDATE: %s = %s\n", ident, buf );
}
void time_rnv_update( identifier ident, long b )
{
    char buf[20];
    strtime( buf, 20, b );
    fprintf( stdout, "TIME_RNV_UPDATE: %s = %s\n", ident, buf );
}

int countable( identifier ident )
{
    return 0;
}

void print( char *string )
{
    fprintf( stdout, "PRINT: %s\n", string );
}

double cashdesk( void )
{
    return 6.0;
}

expr points( void )
{
    expr e;
    e = new_points();
    ins_point( e, "central", 100 );
    ins_point( e, "local", 25 );
    ins_point( e, "central", 10 );
    return e;
}

expr ebonus( void )
{
    expr e;
    e = new_ebonus();
    ins_point( e, "c01", 100 );
    ins_point( e, "c02", 25 );
    ins_point( e, "c01", 10 );
    ins_point( e, "c03", 50 );
    return e;
}


int __input( char *str )
{
    char r;
    fprintf( stdout, "INPUT: %s (y = YES, !y = NO) ", str );
    scanf( "%c", &r );
    return ( int )( r == 'y' );
}

double numinput( char *str )
{
    double r;
    fprintf( stdout, "NUM_INPUT: %s ", str );
    scanf( "%lf", &r );
    return r;
}



idate get_date()
{
    idate rta;
    int r = 0;
    #ifdef PROMO_FAULT_TOLERANCE
    long logVal;


    r = readDate( &logVal );
    if( r == -1 ) {
        #endif //PROMO_FAULT_TOLERANCE

        rta = getdate_();

        #ifdef PROMO_FAULT_TOLERANCE
    }
    else {
        rta = ( idate )logVal;
    }

    logDate( rta );
    #endif //PROMO_FAULT_TOLERANCE

    return rta;
}

itime get_time()
{
    idate rta;
    #ifdef PROMO_FAULT_TOLERANCE
    int r = 0;
    long logVal;


    r = readTime( &logVal );
    if( r == -1 ) {
        #endif //PROMO_FAULT_TOLERANCE

        rta = gettime_();

        #ifdef PROMO_FAULT_TOLERANCE
    }
    else {
        rta = ( itime )logVal;
    }

    logTime( rta );
    #endif //PROMO_FAULT_TOLERANCE
/*    {
        char buff[256];sprintf( buff, "TIME: %li", rta );fprintf( stdout, buff );
    }
*/
    return rta;
}



#endif // INVEL_W


//#endif
