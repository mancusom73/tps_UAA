#ifndef SCRIPT_H
#define SCRIPT_H


#define NOVEDADES_POR_VEZ 50

#define _RECARGAR_PROMO				1
#define _RECARGAR_MEDIOS			2
#define _RECARGAR_COBROS			3
#define _RECARGAR_TARJETAS			4
#define _RECARGAR_ARTICULO_IVA		5
#define _RECARGAR_DEPARTAMENTOS		6
#define _RECARGAR_IMPUESTOS 		7
#define _RECARGAR_TENVASES		    8
#define _RECARGAR_MEDIOS_TAR		9


/*Codigos de respuesta en script*/
#define _SCRIPT_RTA_MAL_CONFIGURADO				2 //usado en el cierre z remoto, indica que la caja esta mal configurada para el cierre remoto
#define _SCRIPT_RTA_PROCESANDO_Z				3 //usado en el cierre z remoto, indica que se esta procesando el cierre z
#define _SCRIPT_RTA_Z_DUPLICADA					4
#define _SCRIPT_RTA_CAJA_OPERANDO				5
#define _SCRIPT_RTA_OK							6
#define _SCRIPT_RTA_IMPRESORA_OFF_LINE			7


/*Codigos de error en script*/
#define _SCRIPT_ERROR_INTERNO					1
#define _SCRIPT_ERROR_EN_PARAMETROS 			2
#define _SCRIPT_ERROR_NO_EXISTE_TABLA_SVR		3
#define _SCRIPT_ERROR_SCRIPT_NO_RECONOCIDO		4
//El define _SCRIPT_ERROR_JSINCRO es unicamente usado por el jsincro.
//Jsincro setea en la tabla SCRIPT del servidor cuando detecta un error al intentar pasar la tabla SVR
#define _SCRIPT_ERROR_JSINCRO					5
#define _SCRIPT_ERROR_OBTENER_TABLA				6



int PROCESAR_SCRIPT( int inicial );
int TOMAR_TABLA_SVR(char *pTableName);
#endif
