/*
 *   Sistema: TPV 1000 - IV
 *
 *   Implementacion de un contenedor de ceTipos
 *
 *   @author: Nicolas Zandarin (2007)
 */

#include <stdio.h>
#include <memory.h>
#include "confContainer.h"
#include <assert.h>

confContainer::confContainer(void)
{
    memset( base, 0, sizeof( base ) );
}

confContainer::~confContainer(void)
{
    int iter;
    for( iter = 0; iter < sizeof( base ); iter++ ) {
        if( base[iter] ) {
            delete( base[iter] );
        }
    }
}

/**
 * 
 */
void confContainer::typeError( int posicion, ctype_enum solicitado, ctype_enum encontrado ) {
    char mesg[80];

    _snprintf( mesg, sizeof( mesg ), "ERROR EN VAR CONFTPV %d: SOLICITADO %d ENCONTRADO %d", posicion, solicitado, encontrado );
    /*MENSAJE( mesg );
    GRABAR_LOG_SISTEMA( mesg );*/
}

/*--------------------------------------------------------------------------*/
/* sets y gets de todos los tipos soportados. Al crear un elemento, se      */
/* almacena el tipo junto al dato, este tipo se verifica al modificar el    */
/* dato y al obtenerlo.                                                     */
/*--------------------------------------------------------------------------*/

void confContainer::setInt( int posicion , CTYPE_INT dato ) {
    
    assert( posicion > 0 && posicion < sizeof( base ) );
    if( base[posicion] && base[posicion]->getType() == ctype_int ) {
        static_cast<confFixedType<CTYPE_INT>*>( base[posicion] )->set( dato );
    } else {
        base[posicion] = new confFixedType<CTYPE_INT>( dato, ctype_int );
    }                                  
}

CTYPE_INT confContainer::getInt( int posicion ) {

    assert( posicion > 0 && posicion < sizeof( base ) );
    if( base[posicion]->getType() == ctype_int ) {
        return static_cast<confFixedType<CTYPE_INT>*>(base[posicion])->get();
    } else {
        typeError( posicion, ctype_int, base[posicion]->getType() );
    }
    return 0;
}

void confContainer::setFloat( int posicion , CTYPE_FLOAT dato ) {
    
    assert( posicion > 0 && posicion < sizeof( base ) );
    if( base[posicion]&& base[posicion]->getType() == ctype_float ) {
        static_cast<confFixedType<CTYPE_FLOAT>*>(base[posicion])->set( dato );
    } else {
        base[posicion] = new confFixedType<CTYPE_FLOAT>( dato, ctype_float );
    }
}

CTYPE_FLOAT confContainer::getFloat( int posicion ) {

    assert( posicion > 0 && posicion < sizeof( base ) );
    if( base[posicion]->getType() == ctype_float ) {
        return static_cast<confFixedType<CTYPE_FLOAT>*>(base[posicion])->get();
    } else {
        typeError( posicion, ctype_float, base[posicion]->getType() );
    }
    return 0;
}

void confContainer::setDouble( int posicion , CTYPE_DOUBLE dato ) {
    
    assert( posicion > 0 && posicion < sizeof( base ) );
    if( base[posicion] && base[posicion]->getType() == ctype_double ) {
        static_cast<confFixedType<CTYPE_DOUBLE>*>(base[posicion])->set( dato );
    } else {
        base[posicion] = new confFixedType<CTYPE_DOUBLE>( dato, ctype_double );
    }                                  
}

CTYPE_DOUBLE confContainer::getDouble( int posicion ) {

    assert( posicion > 0 && posicion < sizeof( base ) );
    if( base[posicion]->getType() == ctype_double ) {
        return static_cast<confFixedType<CTYPE_DOUBLE>*>(base[posicion])->get();
    } else {
        typeError( posicion, ctype_double, base[posicion]->getType() );
    }
    return 0;
}

void confContainer::setUnsigned( int posicion , CTYPE_UNSIGNED dato ) {
    
    assert( posicion > 0 && posicion < sizeof( base ) );
    if( base[posicion] && base[posicion]->getType() == ctype_unsigned ) {
        static_cast<confFixedType<CTYPE_UNSIGNED>*>(base[posicion])->set( dato );
    } else {
        base[posicion] = new confFixedType<CTYPE_UNSIGNED>( dato, ctype_unsigned );
    }                                  
}

CTYPE_UNSIGNED confContainer::getUnsigned( int posicion ) {

    assert( posicion > 0 && posicion < sizeof( base ) );
    if( base[posicion]->getType() == ctype_unsigned ) {
        return static_cast<confFixedType<CTYPE_UNSIGNED>*>(base[posicion])->get();
    } else {
        typeError( posicion, ctype_unsigned, base[posicion]->getType() );
    }
    return 0;
}

void confContainer::setLong( int posicion , CTYPE_LONG dato ) {
    
    assert( posicion > 0 && posicion < sizeof( base ) );
    if( base[posicion] && base[posicion]->getType() == ctype_long ) {
        static_cast<confFixedType<CTYPE_LONG>*>(base[posicion])->set( dato );
    } else {
        base[posicion] = new confFixedType<CTYPE_LONG>( dato, ctype_long );
    }
}

CTYPE_LONG confContainer::getLong( int posicion ) {

    assert( posicion > 0 && posicion < sizeof( base ) );
    if( base[posicion]->getType() == ctype_long ) {
        return static_cast<confFixedType<CTYPE_LONG>*>(base[posicion])->get();
    } else {
        typeError( posicion, ctype_long, base[posicion]->getType() );
    }
    return 0;
}

void confContainer::setUnsignedLong( int posicion, CTYPE_ULONG dato ) {

    assert( posicion > 0 && posicion < sizeof( base ) );
    if( base[posicion] && base[posicion]->getType() == ctype_ulong ) {
        static_cast<confFixedType<CTYPE_ULONG>*>(base[posicion])->set( dato );
    } else {
        base[posicion] = new confFixedType<CTYPE_ULONG>( dato, ctype_ulong );
    }
}

CTYPE_ULONG confContainer::getUnsignedLong( int posicion ) {

    assert( posicion > 0 && posicion < sizeof( base ) );
    if( base[posicion]->getType() == ctype_ulong ) {
        return static_cast<confFixedType<CTYPE_ULONG>*>(base[posicion])->get();
    } else {
        typeError( posicion, ctype_ulong, base[posicion]->getType() );
    }
    return 0;
}

void confContainer::setString( int posicion , CTYPE_STRING dato, int largo ) {
    
    /* El dato es copiado dentro del objeto, por lo que se puede destruir
     * despues de llamar a esta funcion */
    assert( posicion > 0 && posicion < sizeof( base ) );
    if( base[posicion] && base[posicion]->getType() == ctype_string ) {
        static_cast<confString*>(base[posicion])->set( dato, largo );
    } else {
        base[posicion] = new confString( dato, largo );
    }                                  
}

CTYPE_STRING confContainer::getString( int posicion ) {

    assert( posicion > 0 && posicion < sizeof( base ) );
    if( base[posicion]->getType() == ctype_string ) {
        return static_cast<confString*>(base[posicion])->get();
    } else {
        typeError( posicion, ctype_string, base[posicion]->getType() );
    }
    return NULL;
}

int confContainer::getStringLen( int posicion ) {
    
    assert( posicion > 0 && posicion < sizeof( base ) );
    if( base[posicion]->getType() == ctype_string ) {
        return static_cast<confString*>(base[posicion])->getLargo();
    } else {
        typeError( posicion, ctype_string, base[posicion]->getType() );
    }
    return 0;
}
