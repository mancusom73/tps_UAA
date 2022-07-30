#ifndef BTRV_H
#define BTRV_H
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

#include <config.h>
#if defined(INVEL_W) || defined(INVEL_L)
#ifdef __cplusplus
extern    "C" {
#endif
#endif
	extern char error_btrv;

/*--------------------------- Definiciones --------------------------------*/
#define MAX_LONG_KEY 255
#define TOP          -1
#define BOTTOM       -2
#define S_FOUND       1
#define S_BOF         2
#define S_EOF         3
#define BLOQUEADO     7

#define CADENA_EN_BROWSE     31

/*------------------- Estructuras y variables globales --------------------*/

	void      GET_LOCKS( char *lock, char *wait, char *multiple );
	void      SET_LOCKS( char lock, char wait, char multiple );
	int       GET_STATUS( char orden );
	void      SET_STATUS( char orden, char valor );
	//int USE_DATABASE( int area, char *nombre, char *estructura, char *clave_acceso, int largo, int modo, int tamanno);
	int       USE_DATABASE( int area, char *nombre, char *estructura, int tam_estructura,
		char *clave_acceso, int largo, int modo );
	int       CLOSE_ALL( void );
	int       SELECT( int num_area );
	int       SELECTED( void );
	int       SET_ORDER( int number_key );
	int       GET_ORDER( void );
	int       BOF( void );
	int       BtrvEOF( void );
	int       FOUND( void );
	int       EMPTY( void );
	unsigned long RECCOUNT( void );
	//DELETE(void);
	int       RECALL( void );
	int       DELETED( void );
	int       DELETE_ALL( void );
	int       RECALL_ALL( void );
	int       TESTEAR( char orden );
	void      SETEAR( char orden, char bit );
	int       PACK( void );
	int       ZAP(  );
	int       CREATE( char *nombre, char *especif, int largo_esp, int modo );
	int       _DELETE( void );
	int       END_TRANSACTION( void );
	int       GET_DIRECTORY( char *buffer, int drive );
	int       INSERT( void );
	int       _INSERT( int );
	int       SET_DIRECTORY( char *path );
	int       SET_OWNER( char *nombre, int longitud, int modo );
	int       CLEAR_OWNER( void );
	int       ABORT_TRANSACTION( void );
	int       BEGIN_TRANSACTION( void );
	int       STAT( char *buffer, int longitud, char *extension );
	int       UNLOCK_( void );
	int       UNLOCK_SINGLE( long registro );
	int       UNLOCK_ALL( void );
	int       UPDATE( void );
	long      GET_POSITION( void );
	int       GET_DIRECT( unsigned long posicion );
	int       STEP_FIRST( void );
	int       STEP_LAST( void );
	int       STEP_NEXT( void );
	int       STEP_PREVIOUS( void );
	int       GET_EQUAL( char *clave );
	int       GET_GREATER( char *clave );
	int       GET_GREATER_E( char *clave );
	int       GET_LESS( char *clave );
	int       GET_LESS_E( char *clave );
	int       GET_NEXT( void );
	int       GET_PREVIOUS( void );
	int       GET_FIRST( void );
	int       GET_LAST( void );
	int       GET_KEY_EQUAL( char *clave );
	int       GET_KEY_GREATER( char *clave );
	int       GET_KEY_GREATER_E( char *clave );
	int       GET_KEY_LESS( char *clave );
	int       GET_KEY_LESS_E( char *clave );
	int       GET_KEY_NEXT( void );
	int       GET_KEY_PREVIOUS( void );
	int       GET_KEY_FIRST( void );
	int       GET_KEY_LAST( void );
	int       LAST_KEY( char *buffer, int l_buffer );
	int       FIRST_KEY( char *buffer, int l_buffer );
	int       SKIP( int salto );
	int       GO( int registro );
	char     *BUFFER_REC_DB( int area );
	int       CARGAR_DATABASE( char *nombre, char *buffer, int *l_reg );
	int       AREA_DISPONIBLE( void );
	int       CLOSE_DATABASE( int );
	//USE_DB( int area, int cod, char *estructura, char *clave, int largo, int modo, int tamanno);
	int       USE_DB( int area, int cod, char *estructura, int tam_estructura, char *clave,
																		int largo, int modo );
	int       USED( int area );
	int       CLOSE_DB( int cod );
	int       INSERT_VARIABLE( int long_reg );
	int       CREAT( int cod, int modo );
	int       SIZEOF_REC_DB( int area );
	char     *BUFFER_REC_DB( int area );
	int       CARGAR_DB( int cod, char *buffer, int *l_reg );
	int       BTRIEVE_RESET(  );
	int       BEGIN_TRANSACTION_CONCURRENTE(  );
	int       BTRIEVE_PRESENTE(  );
	int       RECALL(  );
	int       BtrvDELETE(  );
	int       INSERT_NO_MODIF_STATUS(  );


#ifdef __cplusplus
}
#endif
#endif
