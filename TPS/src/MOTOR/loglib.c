/*! \file loglib.c
    \brief Librería de creación y acceso a log's de Tolerancia a Fallas.
    
    Esta librería implementa funciones para la creación y lectura de
    archivos binarios de log's para mantener información que luego
    podría ser utilizada para resturar el Motor de Promociones en caso
    de producirse una falla que interrumpa la normal ejecución del
    mismo.
 */


#pragma pack (1)
#if defined(INVEL_W) || defined(INVEL_L)
#include "cr.h"
#include "path.h"
#endif
#include "extern.h"
#include "loglib.h"
#include <stdlib.h>
#include <stdio.h>
//#include <io.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include "etpv.h"
#include "ini.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#pragma pack (1)

static char logname[256] = "log.xxx";

static char new_log[256] = "log.xxx";

/**
 * Escribe data (de tama#o tam) caracter a caracter en flujo.
 * Devuelve la cantidad de caracteres escritos correctamente.
 */ 
int fputn( void *data, size_t tam, FILE *flujo )
{
    unsigned int i;
    for( i = 0;i < tam;i++ ) {
        if( fputc( ( ( char* )data )[i], flujo ) == EOF ) {
            return i;
        }
    }
    return i;
}


/**
 * Lee tam caracteres a caracter de flujo y los almacena en data.
 * Devuelve un puntero a data en caso de exito o NULL c.c.
 */ 
void * fgetn( void *data, size_t tam, FILE *flujo )
{
    unsigned int i;
    for( i = 0;i < tam;i++ ) {
        //    if ( (((char*)data)[i] = (char)fgetc(flujo)) == EOF ) {
        ( ( ( char* )data )[i] = ( char )fgetc( flujo ) );
        if( feof( flujo ) ) {
            return NULL;
        }
    }
    return data;
}

int putInitValue( id i, void *data, size_t tam, FILE *flujo )
{
    const char v = ( char )InitValue;
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    fputn( ( void* )&v, 1, flujo ); // se escribe el tipo de registro
    fputn( &i, sizeof( id ), flujo ); // se escribe el identificador
    fputn( &tam, 1, flujo ); // se escribe el tama#o del dato ( <256 )
    fputn( data, tam, flujo );// se escribe el dato
    return 1;
}

int putCountValue( id i, number count, FILE *flujo )
{
    const char v = ( char )CountValue;
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    fputn( ( void* )&v, 1, flujo ); // se escribe el tipo de registro
    fputn( &i, sizeof( id ), flujo ); // se escribe el identificador
    fputn( &count, sizeof( number ), flujo );// se escribe la cuenta
    return 1;
}

int putInitSubValue( id i, char ind, void *data, size_t tam, FILE *flujo )
{
    const char v = ( char )InitSubValue;
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    fputn( ( void* )&v, 1, flujo ); // se escribe el tipo de registro
    fputn( &i, sizeof( id ), flujo ); // se escribe el identificador
    fputn( ( void* )&ind, 1, flujo ); // se escribe el subindice
    fputn( &tam, 1, flujo ); // se escribe el tama#o del dato ( <256 )
    fputn( data, tam, flujo );// se escribe el dato
    return 0;
}

int putCountSubValue( id i, char ind, number count, FILE *flujo )
{
    const char v = ( char )CountSubValue;
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    fputn( ( void* )&v, 1, flujo ); // se escribe el tipo de registro
    fputn( &i, sizeof( id ), flujo ); // se escribe el identificador
    fputn( ( void* )&ind, 1, flujo ); // se escribe el subindice
    fputn( &count, sizeof( number ), flujo );// se escribe la cuenta
    return 0;
}

int putAddArticle( identifier i, number count, long department, double precio, char promocionable, char porImporte, FILE *flujo )
{
    const char v = ( char )AddArticle;
    const char size = ( char )( strlen( i ) + 1 );
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    fputn( ( void* )&v, 1, flujo ); // se escribe el tipo de registro
    fputn( ( void* )&size, 1, flujo ); // se escribe el tama#o del identificador ( <256 )
    fputn( ( void* )i, size, flujo ); // se escribe el identificador
    fputn( &count, sizeof( number ), flujo );// se escribe la cantidad
    fputn( &department, sizeof( long ), flujo );// se escribe el departamento
    fputn( &precio, sizeof( double ), flujo );// se escribe el precio
    fputn( &promocionable, 1, flujo );// se escribe si es promocionable
    fputn( &porImporte, 1, flujo );// se escribe si es articulo por importe
    return 0;
}

int putRemoveArticle( identifier i, number count, long department, double precio, char promocionable, char porImporte, FILE *flujo )
{
    const char v = ( char )RemoveArticle;
    const char size = ( char )( strlen( i ) + 1 );
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    fputn( ( void* )&v, 1, flujo ); // se escribe el tipo de registro
    fputn( ( void* )&size, 1, flujo ); // se escribe el tama#o del identificador ( <256 )
    fputn( ( void* )i, size, flujo ); // se escribe el identificador
    fputn( &count, sizeof( number ), flujo );// se escribe la cantidad
    fputn( &department, sizeof( long ), flujo );// se escribe la cantidad
    fputn( &precio, sizeof( double ), flujo );// se escribe la cantidad
    fputn( &promocionable, 1, flujo );// se escribe si es promocionable
    fputn( &porImporte, 1, flujo );// se escribe si es articulo por importe
    return 0;
}

int putUpdateArticle( identifier i, number count, long department, double precio, char promocionable, char porImporte, FILE *flujo )
{
    const char v = ( char )UpdateArticle;
    const char size = ( char )( strlen( i ) + 1 );
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    fputn( ( void* )&v, 1, flujo ); // se escribe el tipo de registro
    fputn( ( void* )&size, 1, flujo ); // se escribe el tama#o del identificador ( <256 )
    fputn( ( void* )i, size, flujo ); // se escribe el identificador
    fputn( &count, sizeof( number ), flujo );// se escribe la cantidad
    fputn( &department, sizeof( long ), flujo );// se escribe la cantidad
    fputn( &precio, sizeof( double ), flujo );// se escribe la cantidad
    fputn( &promocionable, 1, flujo );// se escribe si es promocionable
    fputn( &porImporte, 1, flujo );// se escribe si es articulo por importe
    return 0;
}


int putAddExternalDiscount( identifier i, number count, long department, double precio, char promocionable, char porImporte, FILE *flujo )
{
    const char v = ( char )AddExternalDiscount;
    const char size = ( char )( strlen( i ) + 1 );
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    fputn( ( void* )&v, 1, flujo ); // se escribe el tipo de registro
    fputn( ( void* )&size, 1, flujo ); // se escribe el tama#o del identificador ( <256 )
    fputn( ( void* )i, size, flujo ); // se escribe el identificador
    fputn( &count, sizeof( number ), flujo );// se escribe la cantidad
    fputn( &department, sizeof( long ), flujo );// se escribe la cantidad
    fputn( &precio, sizeof( double ), flujo );// se escribe la cantidad
    fputn( &promocionable, 1, flujo );// se escribe si es promocionable
    fputn( &porImporte, 1, flujo );// se escribe si es articulo por importe
    return 0;
}

int putPanelVentas( FILE *flujo )
{
    const char v = ( char )PanelVentas;
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    fputn( ( void* )&v, 1, flujo ); // se escribe el tipo de registro
    return 0;
}

int putPanelPagos( FILE *flujo )
{
    const char v = ( char )PanelPagos;
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    fputn( ( void* )&v, 1, flujo ); // se escribe el tipo de registro
    return 0;
}

int putPanelPosPago( FILE *flujo )
{
    const char v = ( char )PanelPosPago;
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    fputn( ( void* )&v, 1, flujo ); // se escribe el tipo de registro
    return 0;
}

int putCredit( identifier i, number count, FILE *flujo )
{
    const char v = ( char )Credit;
    const char size = ( char )( strlen( i ) + 1 );
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    fputn( ( void* )&v, 1, flujo ); // se escribe el tipo de registro
    fputn( ( void* )&size, 1, flujo ); // se escribe el tama#o del identificador ( <256 )
    fputn( ( void* )i, size, flujo ); // se escribe el identificador
    fputn( &count, sizeof( number ), flujo );// se escribe la cantidad
    return 0;
}


int putDistribute( identifier i, number count, number val, FILE *flujo )
{
    const char v = ( char )Distribute;
    const char size = ( char )( strlen( i ) + 1 );
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    fputn( ( void* )&v, 1, flujo ); // se escribe el tipo de registro
    fputn( ( void* )&size, 1, flujo ); // se escribe el tama#o del identificador ( <256 )
    fputn( ( void* )i, size, flujo ); // se escribe el identificador
    fputn( &count, sizeof( number ), flujo );// se escribe la cantidad
    fputn( &val, sizeof( number ), flujo );// se escribe el valor
    return 0;
}

int putIssueBonus( identifier target, identifier type, number count, number value, char *label,
                   FILE *flujo )
{
    const char v = ( char )IssueBonus;
    char size = ( char )( strlen( target ) + 1 );
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    fputn( ( void* )&v, 1, flujo ); // se escribe el tipo de registro
    fputn( ( void* )&size, 1, flujo ); // se escribe el tama#o del target ( <256 )
    fputn( ( void* )target, size, flujo ); // se escribe el target

    size = ( char )( strlen( type ) + 1 );
    fputn( ( void* )&size, 1, flujo ); // se escribe el tama#o del type ( <256 )
    fputn( ( void* )type, size, flujo ); // se escribe el type

    fputn( &value, sizeof( number ), flujo );// se escribe el valor

    fputn( &count, sizeof( number ), flujo );// se escribe la cantidad

    size = ( char )( strlen( label ) + 1 );
    fputn( ( void* )&size, 1, flujo ); // se escribe el tama#o del type ( <256 )
    fputn( ( void* )label, size, flujo ); // se escribe el label

    return 0;
}


int putGivePoints( identifier i, number count, FILE *flujo )
{
    const char v = ( char )GivePoints;
    const char size = ( char )( strlen( i ) + 1 );
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    fputn( ( void* )&v, 1, flujo ); // se escribe el tipo de registro
    fputn( ( void* )&size, 1, flujo ); // se escribe el tama#o del identificador ( <256 )
    fputn( ( void* )i, size, flujo ); // se escribe el identificador
    fputn( &count, sizeof( number ), flujo );// se escribe la cantidad
    return 0;
}



int putBoolInput( int value, FILE *flujo )
{
    int r = 0;
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    r = putInitValue( ( id )boolInput, ( void* )&value, sizeof( int ), flujo );
    r = r | putCountValue( ( id )boolInput, ( number )1, flujo );
    return r;
}

int putNumInput( double value, FILE *flujo )
{
    int r = 0;
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    r = putInitValue( ( id )numInput, ( void* )&value, sizeof( double ), flujo );
    r = r | putCountValue( ( id )numInput, ( number )1, flujo );
    return r;
}

int putTime( long value, FILE *flujo )
{
    int r = 0;
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    // >>>> Reimplementar esto de forma optimizada. i.e. que la cuenta no sea 1

    r = putInitValue( ( id )idTime, ( void* )&value, sizeof( long ), flujo );
    r = r | putCountValue( ( id )idTime, ( number )1, flujo );
    return r;
}

int putDate( long value, FILE *flujo )
{
    int r = 0;
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    // >>>> Reimplementar esto de forma optimizada. i.e. que la cuenta no sea 1

    r = putInitValue( ( id )idDate, ( void* )&value, sizeof( long ), flujo );
    r = r | putCountValue( ( id )idDate, ( number )1, flujo );
    return r;
}

int putDay( long value, FILE *flujo )
{
    int r = 0;
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    // >>>> Reimplementar esto de forma optimizada. i.e. que la cuenta no sea 1

    r = putInitValue( ( id )idDay, ( void* )&value, sizeof( long ), flujo );
    r = r | putCountValue( ( id )idDay, ( number )1, flujo );
    return r;
}


int putPurchase( int value, FILE *flujo )
{
    int r = 0;
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    r = putInitValue( ( id )idPurchase, ( void* )&value, sizeof( int ), flujo );
    r = r | putCountValue( ( id )boolInput, ( number )1, flujo ); // corregir esto. deberia hacerse bien para optimizar.

    //oValue (&outbuffer, v, ind, r, size, normalLog);   

    return r;
}


/*
 * Se escribe la estructura s (de tamaño size) y los 5 strings str* en flujo.
 */
int putCashdeskState( void *s, int size, char *str0, char *str1, char *str2, char *str3, char *str4,
                      FILE *flujo )
{
    const char v = ( char )CashdeskState;
    char len = 0;
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    //totalsize = size + strlen(str0) + strlen(str1) + strlen(str2) + strlen(str3) + strlen(str4) + 5

    fputn( ( void* )&v, 1, flujo ); // se escribe el tipo de registro
    fputn( ( void* )&size, sizeof( int ), flujo ); // se escribe el tama#o del struct ( <256 )
    fputn( ( void* )s, size, flujo ); // se escribe el struct

    len = ( char )( strlen( str0 ) + 1 );
    fputn( ( void* )&len, 1, flujo ); // se escribe el tama#o del str0 ( <256 )
    fputn( ( void* )str0, len, flujo ); // se escribe el str0 

    len = ( char )( strlen( str1 ) + 1 );
    fputn( ( void* )&len, 1, flujo ); // se escribe el tama#o del str1 ( <256 )
    fputn( ( void* )str1, len, flujo ); // se escribe el str1

    len = ( char )( strlen( str2 ) + 1 );
    fputn( ( void* )&len, 1, flujo ); // se escribe el tama#o del str2 ( <256 )
    fputn( ( void* )str2, len, flujo ); // se escribe el str2

    len = ( char )( strlen( str3 ) + 1 );
    fputn( ( void* )&len, 1, flujo ); // se escribe el tama#o del str3 ( <256 )
    fputn( ( void* )str3, len, flujo ); // se escribe el str3

    len = ( char )( strlen( str4 ) + 1 );
    fputn( ( void* )&len, 1, flujo ); // se escribe el tama#o del str4 ( <256 )
    fputn( ( void* )str4, len, flujo ); // se escribe el str4

    return 0;
}





/* 
 * Devuelve NULL en caso de EOF
 */
logReg getLogEntry( /*logtype *l, id *i, void *data, size_t max, number *count, */FILE *flujo )
{
    char tam;
    int tamm;
    logReg r;
	if (!flujo) {
		glog("loglib: read error: flujo = NULL",LOG_VARIOS,2);
		return NULL;
	}
    r = ( logReg )malloc( sizeof( struct logReg ) );
    if( r == NULL ) {
        fprintf( stderr, "getLogEntry: error de memoria" );
        return NULL;
    }

    memset( r, 0, sizeof( struct logReg ) );
    if( fgetn( &r->type, 1, flujo ) == NULL ) {
        // lee tipo de registro
        free( r );
        return NULL;
    }

    switch( r->type ) {
        case InitValue:
            if( fgetn( &( r->id ), sizeof( id ), flujo ) == NULL ) {
                // lee el identificador
                free( r );
                return NULL;
            }
            if( fgetn( ( void* )&tam, 1, flujo ) == NULL ) {
                // lee el tama#o del dato
                free( r );
                return NULL;
            }
            if( ( r->data = malloc( tam ) ) == NULL ) {
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                return NULL;
            }
            if( fgetn( r->data, tam, flujo ) == NULL ) {
                // lee el dato
                free( r->data );
                free( r );
                return NULL;
            }
            break;
        case CountValue:
            if( fgetn( &( r->id ), sizeof( id ), flujo ) == NULL ) {
                // lee el identificador
                free( r );
                return NULL;
            }
            if( fgetn( &( r->count ), sizeof( number ), flujo ) == NULL ) {
                // lee la cuenta.
                free( r );
                return NULL;
            }
            break;
        case InitSubValue:
            if( fgetn( &( r->id ), sizeof( id ), flujo ) == NULL ) {
                // lee el identificador
                free( r );
                return NULL;
            }
            if( fgetn( &( r->subind ), 1, flujo ) == NULL ) {
                // lee el subindice
                free( r );
                return NULL;
            }
            if( fgetn( ( void* )&tam, 1, flujo ) == NULL ) {
                // lee el tama#o del dato
                free( r );
                return NULL;
            }
            if( ( r->data = malloc( tam ) ) == NULL ) {
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                return NULL;
            }
            if( fgetn( r->data, tam, flujo ) == NULL ) {
                // lee el dato
                free( r->data );
                free( r );
                return NULL;
            }
            break;
        case CountSubValue:
            if( fgetn( &( r->id ), sizeof( id ), flujo ) == NULL ) {
                // lee el identificador
                free( r );
                return NULL;
            }
            if( fgetn( &( r->subind ), 1, flujo ) == NULL ) {
                // lee el subindice
                free( r );
                return NULL;
            }
            if( fgetn( &( r->count ), sizeof( number ), flujo ) == NULL ) {
                // lee la cuenta.
                free( r );
                return NULL;
            }
            break;
        case AddArticle:
            if( fgetn( ( void* )&tam, 1, flujo ) == NULL ) {
                // lee el tama#o del identificador
                free( r );
                return NULL;
            }
            if( ( r->data = (void*) malloc( tam ) ) == NULL ) {
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                return NULL;
            }
            if( fgetn( r->data, tam, flujo ) == NULL ) {
                // lee el identificador
                free( r->data );
                free( r );
                return NULL;
            }
            if( fgetn( &( r->count ), sizeof( number ), flujo ) == NULL ) {
                // lee la cuenta.
                free( r->data );
                free( r );
                return NULL;
            }
            if( fgetn( &( r->depart ), sizeof( long ), flujo ) == NULL ) {
                // lee la cuenta.
                free( r->data );
                free( r );
                return NULL;
            }
            if( fgetn( &( r->precio ), sizeof( double ), flujo ) == NULL ) {
                // lee la cuenta.
                free( r->data );
                free( r );
                return NULL;
            }
            if( fgetn( &( r->promoc ), 1, flujo ) == NULL ) {
                // lee promocionabilidad.
                free( r->data );
                free( r );
                return NULL;
            }
            if( fgetn( &( r->porImporte ), 1, flujo ) == NULL ) {
                // lee articulo por importe.
                free( r->data );
                free( r );
                return NULL;
            }
            break;
        case RemoveArticle:
            if( fgetn( ( void* )&tam, 1, flujo ) == NULL ) {
                // lee el tama#o del identificador
                free( r );
                return NULL;
            }
            if( ( r->data = malloc( tam ) ) == NULL ) {
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                return NULL;
            }
            if( fgetn( r->data, tam, flujo ) == NULL ) {
                // lee el identificador
                free( r->data );
                free( r );
                return NULL;
            }
            if( fgetn( &( r->count ), sizeof( number ), flujo ) == NULL ) {
                // lee la cuenta.
                free( r );
                return NULL;
            }
            if( fgetn( &( r->depart ), sizeof( long ), flujo ) == NULL ) {
                // lee la cuenta.
                free( r->data );
                free( r );
                return NULL;
            }
            if( fgetn( &( r->precio ), sizeof( double ), flujo ) == NULL ) {
                // lee la cuenta.
                free( r->data );
                free( r );
                return NULL;
			}
            if( fgetn( &( r->promoc ), 1, flujo ) == NULL ) {
                // lee promocionabilidad.
                free( r->data );
                free( r );
                return NULL;
            }
            if( fgetn( &( r->porImporte ), 1, flujo ) == NULL ) {
                // lee articulo por importe.
                free( r->data );
                free( r );
                return NULL;
            }
            break;
        case UpdateArticle:
        case AddExternalDiscount:
            if( fgetn( ( void* )&tam, 1, flujo ) == NULL ) {
                // lee el tama#o del identificador
                free( r );
                return NULL;
            }
            if( ( r->data = malloc( tam ) ) == NULL ) {
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                return NULL;
            }
            if( fgetn( r->data, tam, flujo ) == NULL ) {
                // lee el identificador
                free( r->data );
                free( r );
                return NULL;
            }
            if( fgetn( &( r->count ), sizeof( number ), flujo ) == NULL ) {
                // lee la cuenta.
                free( r );
                return NULL;
            }
            if( fgetn( &( r->depart ), sizeof( long ), flujo ) == NULL ) {
                // lee la cuenta.
                free( r->data );
                free( r );
                return NULL;
            }
            if( fgetn( &( r->precio ), sizeof( double ), flujo ) == NULL ) {
                // lee la cuenta.
                free( r->data );
                free( r );
                return NULL;
			}
            if( fgetn( &( r->promoc ), 1, flujo ) == NULL ) {
                // lee promocionabilidad.
                free( r->data );
                free( r );
                return NULL;
            }
            if( fgetn( &( r->porImporte ), 1, flujo ) == NULL ) {
                // lee articulo por importe.
                free( r->data );
                free( r );
                return NULL;
            }
            break;
        case PanelVentas:
        case PanelPagos:
        case PanelPosPago:
            break;
        case Credit:
            if( fgetn( ( void* )&tam, 1, flujo ) == NULL ) {
                // lee el tama#o del identificador
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                free( r );
                return NULL;
            }
            if( ( r->data = malloc( tam ) ) == NULL ) {
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                return NULL;
            }
            if( fgetn( r->data, tam, flujo ) == NULL ) {
                // lee el identificador
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                free( r->data );
                free( r );
                return NULL;
            }
            if( fgetn( &( r->count ), sizeof( number ), flujo ) == NULL ) {
                // lee la cuenta.
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                free( r->data );
                free( r );
                return NULL;
            }
            break;

        case Distribute:
            if( fgetn( ( void* )&tam, 1, flujo ) == NULL ) {
                // lee el tama#o del identificador
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                free( r );
                return NULL;
            }
            if( ( r->data = malloc( tam ) ) == NULL ) {
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                return NULL;
            }
            if( fgetn( r->data, tam, flujo ) == NULL ) {
                // lee el identificador
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                free( r->data );
                free( r );
                return NULL;
            }
            if( fgetn( &( r->count ), sizeof( number ), flujo ) == NULL ) {
                // lee la cuenta.
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                free( r->data );
                free( r );
                return NULL;
            }
            /* Ahora, el valor del bono se almacena en el mismo espacio
               de memoria que el identificador, a continuacion del mismo
            */
            if( ( r->data = realloc( r->data, tam + sizeof( number ) ) ) == NULL ) {
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                free( r->data );
                free( r );
                return NULL;
            }
            if( fgetn( ( void* )( ( int )r->data + tam ), sizeof( number ), flujo ) == NULL ) {
                // lee el valor.
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                free( r->data );
                free( r );
                return NULL;
            }

            break;


        case IssueBonus:
            if( fgetn( ( void* )&tam, 1, flujo ) == NULL ) {
                // lee el tama#o del target
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                free( r );
                return NULL;
            }
            if( ( r->data = malloc( tam ) ) == NULL ) {
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                return NULL;
            }
            if( fgetn( r->data, tam, flujo ) == NULL ) {
                // lee el target
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                free( r->data );
                free( r );
                return NULL;
            }

            /* Ahora, el type del bono se almacena en el mismo espacio
               de memoria que el target, a continuacion del mismo
            */
            {
                int i = tam;
                if( fgetn( ( void* )&tam, 1, flujo ) == NULL ) {
                    // lee el tama#o del type
                    fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                    free( r );
                    return NULL;
                }
                if( ( r->data = realloc( r->data, i + tam ) ) == NULL ) {
                    fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                    return NULL;
                }
                if( fgetn( ( void* )( ( int )r->data + i ), tam, flujo ) == NULL ) {
                    // lee el type
                    fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                    free( r->data );
                    free( r );
                    return NULL;
                }
                tam = tam + i;
            }

            /* Ahora, el valor del bono se almacena en el mismo espacio
               de memoria que el identificador, a continuacion del mismo
            */
            if( ( r->data = realloc( r->data, tam + sizeof( number ) ) ) == NULL ) {
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                free( r->data );
                free( r );
                return NULL;
            }
            if( fgetn( ( void* )( ( int )( r->data ) + tam ), sizeof( number ), flujo ) == NULL ) {
                // lee el valor.
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                free( r->data );
                free( r );
                return NULL;
            }
            tam += sizeof( number );

            if( fgetn( &( r->count ), sizeof( number ), flujo ) == NULL ) {
                // lee la cuenta.
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                free( r->data );
                free( r );
                return NULL;
            }

            /* Ahora, el label del bono se almacena en el mismo espacio
               de memoria que el target,type y value, a continuacion del ellos
            */
            {
                int i = tam;
                if( fgetn( ( void* )&tam, 1, flujo ) == NULL ) {
                    // lee el tama#o del label
                    fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                    free( r );
                    return NULL;
                }
                if( ( r->data = realloc( r->data, i + tam ) ) == NULL ) {
                    fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                    return NULL;
                }
                if( fgetn( ( void* )( ( int )( r->data ) + i ), tam, flujo ) == NULL ) {
                    // lee el label
                    fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                    free( r->data );
                    free( r );
                    return NULL;
                }
                tam = tam + i;
            }

            break;

        case GivePoints:
            if( fgetn( ( void* )&tam, 1, flujo ) == NULL ) {
                // lee el tama#o del identificador
                free( r );
                return NULL;
            }
            if( ( r->data = malloc( tam ) ) == NULL ) {
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                return NULL;
            }
            if( fgetn( r->data, tam, flujo ) == NULL ) {
                // lee el identificador
                free( r->data );
                free( r );
                return NULL;
            }
            if( fgetn( &( r->count ), sizeof( number ), flujo ) == NULL ) {
                // lee la cuenta.
                free( r->data );
                free( r );
                return NULL;
            }
            break;

        case CashdeskState:
            if( fgetn( ( void* )&tamm, sizeof( int ), flujo ) == NULL ) {
                // lee el tama#o del struct
                fprintf( stderr, "getLogEntry: Error: error de lectura\n" );
                free( r );
                return NULL;
            }
            if( ( r->data = malloc( tamm ) ) == NULL ) {
                fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                return NULL;
            }
            if( fgetn( r->data, tamm, flujo ) == NULL ) {
                // lee el struct
                fprintf( stderr, "getLogEntry: Error: error de lectura\n" );
                free( r->data );
                free( r );
                return NULL;
            }

            /* Ahora, los strings se almacenan en el mismo espacio de memoria
              que el struct, a continuacion del mismo
            */
            {
                int y;
                int i = tamm;

                for( y = 0;y < 5;y++ ) {
                    if( fgetn( ( void* )&tam, 1, flujo ) == NULL ) {
                        // lee el tama#o del str0
                        fprintf( stderr, "getLogEntry: Error: error de lectura\n" );
                        free( r->data );
                        free( r );
                        return NULL;
                    }
                    if( ( r->data = realloc( r->data, i + tam ) ) == NULL ) {
                        fprintf( stderr, "getLogEntry: Error: falta de memoria\n" );
                        free( r );
                        return NULL;
                    }
                    if( fgetn( ( void* )( ( int )r->data + i ), tam, flujo ) == NULL ) {
                        // lee el str_y
                        fprintf( stderr, "getLogEntry: Error: error de lectura\n" );
                        free( r->data );
                        free( r );
                        return NULL;
                    }
                    /*
                     * Aca necesito ir recostruyendo los punteros del struct para que apunten
                     * a los strings correspondientes.
                     */
                    //        r->data = (void*) cashdesk_state_set_str ((cashdesk_state) r->data, y,
                    //                                                  (char*)((int)r->data+i) );

                    i += tam;
                }
                // Ahora almaceno en count el tamaño del bloque de memoria (struct+strings)
                r->count = ( double )i;
            }


            break;

        default:
            fprintf( stderr, "getLogEntry: Error: Valor inesperado\n" );
            free( r );
            return NULL;
            break;
		
    }
    return r;
}


void freeLogReg ( logReg l )
{
    if( l ) {
        if( l->data ) {
            free( l->data );
        }
        free( l );
    }
}


FILE * openLog( void )
{
    struct stat attr;
    int i = 0;
    int logsize = 0;
    char fname[256];
    FILE *file = NULL;

    memset( &attr, 0, sizeof(struct stat));
    sprintf( fname, "%slog.000", PATH_PROMOS );

    #ifdef __MSDOS__
    _fmode = O_BINARY;
    #endif
    #ifdef WIN32
    _fmode = O_BINARY;
    #endif

    for( i = 0;i < 999;i++ ) {
        if( stat( fname, &attr ) == 0 ) {
            if( attr.st_size > logsize ) {
                remove( logname );
                strcpy( logname, fname );
                logsize = attr.st_size;
            }
            else {
                remove( fname );
            }
	}
        sprintf( fname, "%slog.%.3i", PATH_PROMOS, i + 1 );
    }

    file = fopen( logname, "r" );
    if( !file ) {
        fprintf( stderr, "openLog: error, no se pudo abrir el log\n" );
        return NULL;
    }

    if (setvbuf(file, NULL, _IONBF, 0)) {
      fprintf(stderr,"openLog: error, no se pudo setear el buffer del log como vacio.\n");
      return NULL;
    }

    return file;
}

int closeLog( FILE *flujo )
{
    int r;
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    r = fclose( flujo );
	if (r) {
		//char desc[256];
		char *desc;
		glog("PROMO: FaultToleranceEngine Error: endLog: ",LOG_VARIOS,2);
		//strerror_r(errno, desc, 255);
		desc = strerror(errno);
		glog(desc,LOG_VARIOS,2);
	}
    if( !r ) {
        r = remove( logname );
    }
    //strcpy(logname,"log.xxx");
    sprintf( logname, "%slog.xxx", PATH_PROMOS );
    return r;
}

FILE * newLog( void )
{
    struct stat attr;
    int i = 0;
    char fname[256];
    FILE *file = NULL;

    sprintf( fname, "%slog.xxx", PATH_PROMOS );

    #ifdef __MSDOS__
    _fmode = O_BINARY;
    #endif
    #ifdef WIN32
    _fmode = O_BINARY;
    #endif

    //sprintf(fname,"log.%.3i",i++);
    sprintf( fname, "%slog.%.3i", PATH_PROMOS, i++ );
    while( stat( fname, &attr ) == 0 ) {
        //sprintf(fname,"log.%.3i",i++);
        sprintf( fname, "%slog.%.3i", PATH_PROMOS, i++ );
    }
    //  fprintf(stderr,fname);
    strcpy( new_log, fname );
    //strcpy(logname,fname); // 14-02-06

    //  return fopen(fname, "w");

    file = fopen( fname, "w" );
    if( !file ) {
        //fprintf( stderr, "newLog: error, no se pudo abrir el log\n" );
        glog( "newLog: error, no se pudo abrir el log" ,LOG_VARIOS,2);
        return NULL;
    }

    if( setvbuf( file, NULL, _IONBF, 0 ) ) {
        glog( "newLog: error, no se pudo setear el buffer del log como vacio." ,LOG_VARIOS,2);
        //fprintf(stderr,"newLog: error, no se pudo setear el buffer del log como vacio.\n");
        return NULL;
    }
    return file;
}

int endLog( FILE *flujo )
{
    int r;
	if (!flujo) {
		glog("loglib: write error: flujo = NULL",LOG_VARIOS,2);
		return 0;
	}
    r = fclose( flujo );
	if (r) {
		//char desc[256];
		char *desc;
		glog("PROMO: FaultToleranceEngine Error: endLog: ",LOG_VARIOS,2);
		//strerror_r(errno, desc, 255);
		desc = strerror(errno);
		glog(desc,LOG_VARIOS,2);
	}
    if( !r ) {
        r = remove( new_log );
    }
    //strcpy(new_log,"log.xxx");
    sprintf( new_log, "%slog.xxx", PATH_PROMOS );
    return r;
}


