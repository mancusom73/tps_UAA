#pragma pack(push,1)

// Modulo de Emulación de Funcionalidades del TPV
// Se utiliza por el interprete de promociones genérico.

#ifdef EMULAR_FUNCIONALIDADES_TPV
#include "_stringt.h"

//void glog( char* str, int origen );
void glog( char *men, int origen, int nivel );

void MENSAJE( char *men );

void BORRAR_ARCHIVO_REC( char *archivo );

#ifndef WIN32
#define _strdup strdup
#define _snprintf snprintf
#endif //WIN32


int DD_TIPO( int var );



// ------------------ DEFINICIONES DE _DD.H DEL TPV -----------------------------

/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

// clase
#define _VAR_RNV        1
#define _VAR_USUARIO    2
#define _VAR_INT_INI    3
#define _VAR_INT_NOINI  4

// tipo_variable
#define  _SHORT_INT      1
#define  _INT            2
#define  _FLOAT          3
#define  _DOUBLE         4
#define  _STRING         5
#define  _DATE           6
#define  _HORA           7
#define  _LONG           8
#define  _UNSIGNED       9

void DD_INS( int var, int clase, long offset, unsigned largo, int elementos, char tipo_variable );

// ------------------------------------------------------------------------------





// ---------------------- SECCIONES CR.H DEL TPV -------------------------------

/*------------------------------  MEMORY  ------------------------------------*/
	void      COPY_STRUCT_TO_BUFFER( char *buffer, int origen );
	void      COPY_BUFFER_TO_STRUCT( char *buffer, int destino );
	void      _COPY_BUFFER_TO_STRUCT( char *buffer, int destino, int subindice );
	int       SIZEOF_ARRAY( int var );
	int       SIZEOF_STRUCT( int cod );
	int       _SIZEOF_STRUCT( int cod );
//   void      _( char *buffer, int origen, int subindice );
	void      CEREAR_STRUCT( int cod );
	void      CEREAR_ARRAY( int cod );
	void      COPY_STRUCT( int destino, int origen );

	void      _LOAD_MEMORY( int var, int subindice, char *valor );
	char      _GET_MEMORY_STR_CHAR( int var, int subindice, int elemento );
	void      _SET_MEMORY( int var, int subindice, char *valor );
	void      _CEREAR_MEMORY( int var, int subindice );
	char      GET_MEMORY_STR_CHAR( int var, int elemento );
	char     *GET_MEMORY_STR( int var );
	char     *_GET_MEMORY_STR( int var, int subindice );
	void      _COPY_MEMORY( int destino, int sub_des, int origen, int sub_ori );
	int       SIZEOF_VAR( int var );
	int       SIZEOF_ARRAY( int var );
	void      SET_MEMORY( int var, char *valor );
	void      CEREAR_MEMORY( int var );
	void      COPY_MEMORY( int destino, int origen );
	void      CEREAR_ARRAY( int cod );
	void      SET_MEMORY_STR_CHAR( int var, int elemento, char valor );
	void      _SET_MEMORY_STR_CHAR( int var, int subindice, int elemento, char valor );
#if defined(INVEL_W) || defined(INVEL_L)
	void      SET_MEMORY_CHAR( int var, int valor );
	void      _SET_MEMORY_CHAR( int var, int subindice, int valor );
#else
	void      SET_MEMORY_CHAR( int var, char valor );
	void      _SET_MEMORY_CHAR( int var, int subindice, char valor );
#endif
	void      SET_MEMORY_INT( int var, int valor );
	void      _SET_MEMORY_INT( int var, int subindice, int valor );
	void      SET_MEMORY_LONG( int var, long valor );
	void      _SET_MEMORY_LONG( int var, int subindice, long valor );
#if defined(INVEL_W) || defined(INVEL_L)
	void      SET_MEMORY_FLOAT( int var, double valor );
	void      _SET_MEMORY_FLOAT( int var, int subindice, double valor );
#else
	void      SET_MEMORY_FLOAT( int var, float valor );
	void      _SET_MEMORY_FLOAT( int var, int subindice, float valor );
#endif
	void      SET_MEMORY_DOUBLE( int var, double valor );
	void      _SET_MEMORY_DOUBLE( int var, int subindice, double valor );
	char      GET_MEMORY_CHAR( int var );
	char      _GET_MEMORY_CHAR( int var, int subindice );
	unsigned char GET_MEMORY_UCHAR( int var );
	unsigned char _GET_MEMORY_UCHAR( int var, int subindice );
	int       GET_MEMORY_INT( int var );
	int       _GET_MEMORY_INT( int var, int subindice );
	unsigned  GET_MEMORY_UNSIGNED( int var );
	unsigned  _GET_MEMORY_UNSIGNED( int var, int subindice );
	long      GET_MEMORY_LONG( int var );
	long      _GET_MEMORY_LONG( int var, int subindice );
	unsigned long GET_MEMORY_ULONG( int var );
	unsigned long _GET_MEMORY_ULONG( int var, int subindice );
	float     GET_MEMORY_FLOAT( int var );
	float     _GET_MEMORY_FLOAT( int var, int subindice );
	double    GET_MEMORY_DOUBLE( int var );
	double    _GET_MEMORY_DOUBLE( int var, int subindice );
	void      ADD_MEMORY_CHAR( int var, char valor );
	void      _ADD_MEMORY_CHAR( int var, int subindice, char valor );
	void      ADD_MEMORY_UCHAR( int var, unsigned char valor );
	void      _ADD_MEMORY_UCHAR( int var, int subindice, unsigned char valor );
	void      ADD_MEMORY_INT( int var, int valor );
	void      _ADD_MEMORY_INT( int var, int subindice, int valor );
	void      ADD_MEMORY_UINT( int var, unsigned int valor );
	void      _ADD_MEMORY_UINT( int var, int subindice, unsigned int valor );
	void      ADD_MEMORY_LONG( int var, long valor );
	void      _ADD_MEMORY_LONG( int var, int subindice, long valor );
	void      ADD_MEMORY_ULONG( int var, unsigned long valor );
	void      _ADD_MEMORY_ULONG( int var, int subindice, unsigned long valor );
	void      ADD_MEMORY_FLOAT( int var, float valor );
	void      _ADD_MEMORY_FLOAT( int var, int subindice, float valor );
	void      ADD_MEMORY_DOUBLE( int var, double valor );
	void      _ADD_MEMORY_DOUBLE( int var, int subindice, double valor );
	int       _APUNTAR_STRUCT( int cod );
	int       SIZEOF_STRUCT( int cod );
	int       _SIZEOF_STRUCT( int cod );
	void      ADD_BUFFER_TO_BUFFER( char *origen, char *destino, int estructura );
	void      _ADD_BUFFER_TO_BUFFER( char *origen, char *destino, int tipo );
	int       _APUNTAR_STRUCT_VERIFICANDO( int cod );
	void      ADD_BUFFER_TO_STRUCT( char *origen, int destino );
	void      _ADD_BUFFER_TO_STRUCT( char *origen, int var, int sub, int tipo );
	void		 _COPY_STRUCT_TO_BUFFER( char *buffer, int origen, int subindice );



/*--------------------------------    DD    --------------------------------------*/
	long      DD_LEER_COD_ESPECIAL( unsigned short var );
	long      GET_LONGITUD_RNV( void );
	void      DD_MEMORY( int clase, int comando, long offset, char *valor,
		unsigned largo );
	int       DD_ABRIR_ARCHIVOS( void );
	void      DD_INIT(  );
	void      DD_INS( int var, int clase, long offset, unsigned largo, int elementos,
		char tipo_variable );
	void      DD_EXEC( int comando, int var, int subindice, char *valor );
	void      _DD_EXEC( int comando, int clase, long offset, char *valor, unsigned largo );
	int       DD_ULTIMO_HANDLE(  );
	int       DD_MAX_HANDLE(  );
	long      DD_OFFSET( int var );
	void      DD_CLOSE(  );
	int       DD_LARGO( int var );
	char      DD_CLASE( int var );
	int       DD_ELEMENTOS( int var );
	int       DD_TIPO( int var );
	int       EST_COD( int var );
	int       EST_VAR( int var );
	char     *DD_PTR( int var );
	void      ABORT( char *t );

// ------------------------------------------------------------------------------




#ifndef __PATH_H
#define __PATH_H

/*Definicion de nombres de archivo comunicacion motor - caja*/
#define ARCH_REC_ACT_MEDIO_MOTOR        "./act_medio.rec"    //Contiene la comunicacion Motor - Caja sobre medios de pago
#define ARCH_REC_PAGOS_PROMO            "./pagos_promo.rec"  //Contiene los pagos realizados con medios activados por promocion, solo caja
#define ARCH_REC_ASIG_BENEFICIO         "./asig_benef.rec"   //Contiene los beneficios otorgados en un ticket para imprimir o asignar en una cuenta
#define ARCH_REC_EVENT_PROMO            "./event_promo.rec"  //Contiene los beneficios eventos de promoción que se deben informar para calcular el Costo Financiero
#define ARCH_REC_REQUERIR_MEDIO         "./requerir_med.rec" //Contine los medios requeridos por promocion


#define PATH_PROMOS           "./"
#define PROMO_COD             "promo.cod"
#define PROMO_WRK             "promo.wrk"

#endif



#else //EMULAR_FUNCIONALIDADES_TPV
#include "stringt.h"

#ifdef INVEL_W

#include <cr.h>
#include <recupero.h>
#include <path.h>
#include "_dd.h"
#include "ddtpv.h"
#include <_cr1.h>

//extern void glog( char *men , int origen );
extern void glog( char *men, int origen, int nivel );

#endif //INVEL_W

#endif //EMULAR_FUNCIONALIDADES_TPV

#pragma pack(pop)
