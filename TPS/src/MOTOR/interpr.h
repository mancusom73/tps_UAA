#pragma pack (push,1) 
/*
 * Definiciones de tipos y funciones de la libreria interpr.lib
 */
#include<stdio.h>

#pragma pack (1)


#define SALIDA stdout

/*
 * Tipo de dato para representar una promocion
 */
typedef struct promo *promo;

/*
 * Tipo de dato para mantener los estados de las variables globales
 */
typedef struct states *states;

typedef struct recover_info * recover_info;

typedef struct cashdesk_state * cashdesk_state;

/*
 *  Versionado de Promociones
 */ 
typedef struct pc_version {
	int macro;
	int version;
	int subversion;
	int build;
} pc_version;


/*
 * Funcion para leer una promocion desde el disco
 */
promo from_disk( char *file );
int compatible_version( char *file, pc_version *ver );


/*
 * Funcion para evaluar una promocion.
 */
typedef enum { NORM, ABNORM, DIVBYZERO, NEGATIVEMSET } evalres;
typedef enum { ONLINE, PREPAGO, POSTPAGO, CANCELLATION, EVER, INIT_PARAMS } evalTime;

evalres eval( evalTime et, promo ps, states *ss );

evalres add_articles( long article, char* barcod, long codPresentacion, double cant, double precio, long depart, int promocionable, int por_importe, promo ps, states *ss );

evalres remove_articles( long article, char* barcod, long codPresentacion, double cant, double precio, long depart, int promocionable, int por_importe, promo ps, states *ss );

evalres update_articles( long article, char* barcod, long codPresentacion, double cant, double precio, long depart, int promocionable, promo ps, states *ss );

evalres add_external_discount( double monto, promo ps, states *ss );

/*
 * Funcion para liberar la memoria utilizada por una promocion
 */
void unref_pr( promo e );

/*
 * Funcion para liberar la memoria utilizada por un estado
 */
states free_states( states s );

/*
 * Devuelve una copia del estado ss
 */
states states_clone( states ss );


recover_info actual_info( states ss );

states recover( recover_info r, states ss );

recover_info free_recover_info( recover_info s );



/*
 * Devuelve el programa que resulta de concatenar p1 ++ p2
 * Se modifica p1.
 */
promo concat( promo p1, promo p2 );



/*
 * Funciones para imprimir una o una lista de promociones.
 */
void up_promo( FILE *out, promo p );

void up_promos( FILE *out, promo p );


/*
 * Variables Globales
 */
extern promo pr, gpr;
extern states ss, ss_bak;
extern recover_info state_venta,state_pago;
extern cashdesk_state globalCashdeskState;
extern pc_version promo_ver;


//#define PROMO_STR

extern evalTime evalMode;

/*
 * PRUEBA: traduccion
 */
int recorrer_promos( void );


void freeall( void );



/*
 * Lectura de archivo de configuraciones generales
 */
void read_promo_config( char* );


/*
 * Parser de identificadores de articulos
 */
typedef enum {COD_INTERNO, COD_BARRA, COD_INTERNO_BARRA, 
			  COD_INTERNO_PRECIO, COD_BARRA_PRECIO, COD_INTERNO_BARRA_PRECIO,
			  COD_INTERNO_BARRA_PRESENTACION, COD_INTERNO_BARRA_PRESENTACION_PRECIO,
			  COD_ERROR} TIPO_COD_ARTICULO;

TIPO_COD_ARTICULO getCodsArt( char* ident, long* cod_int, char * cod_bar, double * precio, long* cod_presentacion);

int consultaPromos( long cod_int, char* cod_bar, long cod_dep, long cod_class_nivel, long cod_class_concepto );
int consultaPromociones( void );

#pragma pack (pop) 
