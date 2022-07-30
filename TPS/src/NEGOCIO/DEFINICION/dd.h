#pragma pack (push,1) 
#ifndef DD_H
#define DD_H
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * DD_VAR.SIC
 * ------------------------------------------------------------------------------
 */
#include <_dd.h>

/*********Enteros super chicos**********/
typedef char BINARY;

struct _dd_var {
    BINARY status;      //0
    char clase;      //1
    short int cod_variable;      //2
    char nom_variable[36];
    char tipo_variable;
    short int elementos;      //43
    char valor_inicial[79];      //45
    char nivel_seg_consulta;      //124
    char nivel_seg_modif;      //125
    unsigned short tabla_valores;
    unsigned short longitud;
    long offset;      //130
    char define[33];
    //char reserva[25];      //166
}dd_var;


struct _dd_est1
{
    BINARY status;
    char clase_variable;
    short int cod_estructura;
    char nom_estructura[21];
    //char reserva[15];
}dd_est1;

struct _dd_est2 {
 BINARY status;					//0
 short int cod_estructura;      //1
 short int cod_variable;		//3
 _BINARY orden;			        //5
 //char reserva[4];				//6
}dd_est2, dd_est2_btrieve;

/*
 * INDICES:
 * -------
 * 
 * DD_VAR
 * 
 * 1) Clase + codigo
 * 2) Clase + nombre
 * 3) Codigo
 * 4) Nombre
 * 
 * DD_EST1
 * 
 * 1) Clase + codigo
 * 2) Codigo
 * 3) Nombre
 * 4) Clase + nombre
 * 
 * DD_EST2
 * 
 * 1) Codigo de estructura + Codigo de variable
 * 2) Codigo de variable
 * 3) Codigo de estructura
 * 
 */

/*
 * Variables especiales
 * --------------------
 * Clase == 0; Cod_variable > 10000;
 * 
 * 10001 - 10009 -> tama�o en bytes para la clase 1 a 9.
 */
char *DD_PTR( int var );
void DD_EXEC( int comando, int var, int subindice, char *valor );
void _DD_EXEC( int comando, int clase, long offset, char *valor, unsigned largo );
int DD_LARGO( int var );
int EST_COD( int var );
int EST_VAR( int var );
long DD_OFFSET( int var );
int DD_ELEMENTOS( int var );
char DD_CLASE( int var );
int DD_TIPO( int var );
long DD_LEER_COD_ESPECIAL( unsigned short var );
long GET_LONGITUD_RNV( void );
void DD_MEMORY( int clase, int comando, long offset, char *valor, unsigned largo );
int DD_ABRIR_ARCHIVOS( void );
void DD_INIT();
void DD_INS( int var, int clase, long offset, unsigned largo, int elementos, char tipo_variable );
int DD_ULTIMO_HANDLE();
int DD_MAX_HANDLE();
void DD_CLOSE();
void ABORT( char *t );
#endif
#pragma pack (pop) 
