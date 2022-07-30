#pragma pack (push,1) 
#ifndef LOGLIB
#define LOGLIB

#include <stdlib.h>
#include <stdio.h>

//#include "extern.h"

#pragma pack (1)

typedef int id;
typedef double number;
#ifndef _TYPES_
typedef char *identifier;
#endif //_TYPES_

/**
 * Tipo del registro en el log.
 * Este enumerado se utiliza en cada registro del log, para indicar
 * el tipo de registro de que se trata.
 */

typedef enum logtype { InitValue, CountValue, InitSubValue, CountSubValue, 
	AddArticle, /**< Registra el agregado de un articulo */
	RemoveArticle, /**< Registra la devolucion de un articulo */
	UpdateArticle, /**< Registra la actualizacion de los datos de un articulo */
	AddExternalDiscount, /**< Registra el agregado de un descuento externo al motor de promociones */
	PanelVentas, /**< Registra la vuelta desde el Panel Pagos al Panel Ventas */
	PanelPagos, /**< Registra el paso al Panel Pagos */
	PanelPosPago, /**< Registra el momento de evaluacion de promos PosPago */
Credit, Distribute, IssueBonus, GivePoints, CashdeskState, Pip } logtype; 

/**
 * Identificadores del tipo de eventos dentro de log.
 * Utilizado para identificar el tipo de evento ocurrido que se registra en el log.
 * Notar que deben ser todos valores negativos, ya que este enumerado luego
 * se graba en un tipo int, en el cual el signo se utiliza para definir
 * el tipo de datos.
 */	
typedef enum idEventosLog { boolInput = -2, numInput = -3, idPurchase = -4, idBuyed_articles = -5,
                            idDelivered_articles = -6, idTime = -7, idDate = -8, idDay = -9 } idEventosLog; 
                                                                                             

/**
 * Estructura de cada registro del log.
 */
typedef struct logReg
{
    logtype type;
    id id;
    char subind;
    void *data;  /**< se utiliza para el dato o para el identificador del article */
    number count;
    long depart;
    double precio;
	char promoc;  /**< indica si el articulo es promocionable */
	char porImporte;
    //  identifier article;
} *logReg;


/**
 * Escribe data (de tama#o tam) caracter a caracter en flujo.
 * Devuelve la cantidad de caracteres escritos correctamente.
 */ 
int fputn( void *data, size_t tam, FILE *flujo );

/**
 * Lee tam caracteres a caracter de flujo y los almacena en data.
 * Devuelve un puntero a data en caso de exito o NULL c.c.
 */ 
void *fgetn( void *data, size_t tam, FILE *flujo );

int putInitValue( id i, void *data, size_t tam, FILE *flujo );

int putCountValue( id i, number count, FILE *flujo );

int putInitSubValue( id i, char ind, void *data, size_t tam, FILE *flujo );

int putCountSubValue( id i, char ind, number count, FILE *flujo );

int putAddArticle( identifier i, number count, long department, double precio, char promocionable, char porImporte, FILE *flujo );

int putRemoveArticle( identifier i, number count, long department, double precio, char promocionable, char porImporte, FILE *flujo );

int putUpdateArticle( identifier i, number count, long department, double precio, char promocionable, char porImporte, FILE *flujo );

int putAddExternalDiscount( identifier i, number count, long department, double precio, char promocionable, char porImporte, FILE *flujo );

int putPanelVentas( FILE *flujo );
int putPanelPagos( FILE *flujo );
int putPanelPosPago( FILE *flujo );

int putCredit( identifier i, number c, FILE *flujo );
int putDistribute( identifier i, number cant, number val, FILE *flujo );
int putIssueBonus( identifier target, identifier type, number count, number value, char *label,
                   FILE *flujo );
int putGivePoints( identifier i, number c, FILE *flujo );

int putBoolInput( int value, FILE *flujo );
int putNumInput( double value, FILE *flujo );
int putTime( long value, FILE *flujo );
int putDate( long value, FILE *flujo );
int putDay( long value, FILE *flujo );
int putPurchase( int value, FILE *flujo );

int putCashdeskState( void *s, int size, char *str0, char *str1, char *str2, char *str3, char *str4,
                      FILE *flujo );


logReg getLogEntry( FILE *flujo );

void freeLogReg( logReg l );

FILE *openLog( void );

int closeLog( FILE *flujo );

FILE *newLog( void );

int endLog( FILE *flujo );

#endif //LOGLIB

#pragma pack (pop) 
