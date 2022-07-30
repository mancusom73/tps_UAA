/*
 *   Sistema: TPV 1000 - IV
 *
 *   Implementacion de metodos para crear y cargar la estructura de datos que almacenara
 *   los datos de configuracion de conftpv
 *
 *   @author: Nicolas Zandarin (2007)
 */
#include "confManager.h"
#include "DB.h"
#include "defTables.h"
#include "memory.h"
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

/**
 * Para realizar operaciones sobre un contenerdor existente
 */
confManager::confManager( confContainer pConf ) {

    conf = &pConf;
}

confManager::confManager(void) {
    
    conf = new confContainer();
}

confManager::~confManager(void) {
    //no destruir conf, ya que lo exportamos.
}

void confManager::castingError( int pIdVariable ) {
    char lMesg[80];

    _snprintf( lMesg, sizeof( lMesg ), "ERROR CARGANDO VARIABLE CONFTPV %d", pIdVariable );
    /*MENSAJE( mesg );
    GRABAR_LOG_SISTEMA( mesg );*/
}

bool confManager::load() {

    long idvariable, dimension, localTypeSize, type;
    int iter = 0;
    char* valor; 
    
    if( DIRECT_DB_QUERY("SELECT var.idVariable, var.dimension, var.idTipoDato, val.valor FROM variable var, valoresXcaja valx, valor val WHERE var.idVariable = valx.idVariable AND valx.idValor = val.idValor ORDER BY 1") > 0 ) {
        //LOG: tabla abierta
        do {
            //---- El numero de variable ----
            GET_DATO( 1, C_INT, (char*)&idvariable, sizeof( idvariable ) );
            //---- El tipo de dato ----
            GET_DATO( 3, C_INT, (char*)&type, sizeof( type ) );
            //---- El largo del tipo ---- (el largo solo es util cuando el tipo es char)
            GET_DATO( 2, C_INT, (char*)&dimension, sizeof( dimension ) );
            //---- Se obtiene el tamaño de memoria apropiado para el tipo ----
            localTypeSize = toLocalType( type, dimension );
            valor = (char*)malloc( MAX_LARGO_VALOR + 1 );
            //---- El valor ----
            GET_DATO( 4, C_CHAR, (char*)valor, MAX_LARGO_VALOR );
            //---- Se almacena el valor obtenido en la variable ----
            setVarConfig( type, idvariable, (char*)valor, localTypeSize );
            //---- El valor ya fue almacenado ----
            free( valor );

        } while( 1 == DIRECT_DB_SKIP( 1 ) );
    } else {
        //LOG: Error tomando datos de las tablas.
    }
    return true;
}

confContainer* confManager::getData() {
    
    return conf;
}


int confManager::toLocalType( int codTipo, int tamString ) {

    switch( codTipo ) {
        case 1: //(1,'shortint','Entero largo','1')
        case 2: //(2,'int','Entero','2')
        case 3: //(3,'float','Decimal','4')
        case 4: //(4,'double','Decimal doble','8')
        case 6: //(6,'date','Fecha','2')
        case 7: //(7,'hora','Hora','2')
        case 8: //(8,'long','Decimal largo','4')
        case 9: //(9,'unsigned','Sin signo','2')
            return MAX_LARGO_VALOR;
        case 5: //(5,'string ','Caracter','1')
            return ( MAX_LARGO_VALOR > tamString )? tamString : MAX_LARGO_VALOR;
         default:
            return MAX_LARGO_VALOR;
    }
}

bool confManager::setVarConfig( int codTipo, int variable, char* valor, int largo ) {
    
    bool outOfRange = false;
    long longTemp = 0;
    double doubleTemp = 0;
    unsigned long uLongTemp = 0;
    signed __int64 heavyLongTemp = 0;
    
    switch( codTipo ) {
        case 2: //Tipo 2 en base 'int' desc: 'Entero' tam: '2'
        case 6: //Tipo 6 en base 'date' desc: 'Fecha' tam: '2'
        case 7: //Tipo 7 en base 'hora' desc: 'Hora' tam: '2'
            longTemp = strtol( valor, NULL, 10 ); 
            if( longTemp >= SHRT_MIN && longTemp <= SHRT_MAX ) {
                conf->setInt( variable, (CTYPE_INT)longTemp );
            } else {
                outOfRange = true;    
            }
            break;
        case 3: //Tipo 3 en base: 'float' 'Decimal','4'
        case 4: //Tipo 4 en base: 'double','Decimal doble','8'
            doubleTemp = strtod( valor, NULL );
            if( doubleTemp < HUGE_VAL && doubleTemp > -1 * HUGE_VAL ) {
                conf->setDouble( variable, (CTYPE_DOUBLE)doubleTemp );
            } else {
                outOfRange = true;       
            }
            break;
        case 5: //(5,'string ','Caracter','1')
            conf->setString( variable, (CTYPE_STRING)valor, largo );
            break;
        case 1: //Tipo 1 en base 'shortint' desc: 'Entero largo' tam: '1'
            heavyLongTemp = strtol( valor, NULL, 10 ); 
            if( heavyLongTemp >= LONG_MIN && heavyLongTemp <= LONG_MAX ) {
                conf->setLong( variable, (CTYPE_LONG)heavyLongTemp );
            }
            break;
        case 9: //Tipo 9 en la base: 'unsigned' desc: 'Sin signo' tam: '2'
            longTemp = strtol( valor, NULL, 10 ); 
            if( longTemp >= 0 && longTemp <= USHRT_MAX ) {
                conf->setUnsigned( variable, (CTYPE_UNSIGNED)longTemp );
            }
        case 8: //(8,'long','Decimal largo','4')
            heavyLongTemp = strtoul( valor, NULL, 10 ); 
            if( heavyLongTemp >= 0 && heavyLongTemp <= ULONG_MAX ) {
                conf->setUnsignedLong( variable, (CTYPE_ULONG)heavyLongTemp );
            }
            break;
        default:
            return false;
    }

    if( outOfRange || errno == ERANGE ) {
        castingError( variable );    
    }
    return true;
}