/*
 *   Sistema: TPV 1000 - IV
 *
 *   Definicion de tipos utilizados para guardar parte de la configuracion.
 *
 *   @author: Nicolas Zandarin (2007)
 */

#pragma once
#include <stdlib.h>
#include <memory.h>

typedef short int      CTYPE_INT;       //2
typedef char           CTYPE_CHAR;      //1
typedef double         CTYPE_FLOAT;     //4
typedef char*          CTYPE_STRING;    //8
#define                CTYPE_STRING_NULL_TERMINATED /* definir si CTYPE_STRING es char* */
typedef long           CTYPE_LONG;      //4
typedef double         CTYPE_DOUBLE;    //8
typedef unsigned short CTYPE_UNSIGNED;  //2
typedef unsigned char  CTYPE_UCHAR;     //1
typedef unsigned long  CTYPE_ULONG;     //4

typedef enum { ctype_int, ctype_char, ctype_float, ctype_string ,ctype_long,
               ctype_double ,ctype_unsigned ,ctype_uchar, ctype_ulong } ctype_enum;

/* Para codigo C, solo son necesaras las definiciones anteriores. Lo que sigue
 * solo es util para el resto de las clases de configuracion en c++ */
#ifdef __cplusplus

class confTypes {
private:
    ctype_enum mType;
public:
    confTypes( void );
    
    confTypes( ctype_enum pType ) {
        mType = pType;
    }
    
    ~confTypes(void);

    ctype_enum getType( void ) {
        return mType;
    }

    void setType( ctype_enum pType ) {
        mType = pType;
    }
};

/* Template utilizada para generar objetos que almacenan tipos de tamaño fijo
 * como int, long, etc. 
 */
template<class T>
class confFixedType : public confTypes {
private:
    T mDato;
public:
    confFixedType( void ) {
    }
    
    confFixedType( T pDato, ctype_enum pType ) : confTypes( pType ) {
        mDato = pDato;
    }
    
    ~confFixedType( void ) {
    }
    
    void set( T pDato ) {
        mDato = pDato;
    }
    
    T get( void ) {
        return mDato;
    }
};

/* Clase que almacena CTYPE_STRING. 
 */
class confString : public confTypes {
private:
    int mLargo;
    CTYPE_STRING mDato;

    void safeStrCopy( CTYPE_STRING pDest, CTYPE_STRING pOrig, int pLargo ) {
        memcpy( pDest, pOrig, pLargo );
        /* hasta aca CTYPE_STRING puede ser cualquier cosa y todo anda bien.
         * Pero si insertamos un NULL en algo distinto a un char*, 
         * posiblemente se arruine el dato */
#ifdef CTYPE_STRING_NULL_TERMINATED
        pDest[ pLargo ] = NULL;
#endif 
    }

public:
    confString( CTYPE_STRING pDato, int pLargo ) : confTypes( ctype_string ) {
        
        mDato = (CTYPE_STRING) malloc( pLargo + 1 );
        mLargo = pLargo;
        safeStrCopy( mDato, pDato, pLargo );
        /* Almacenamos el puntero rencien creado, en vez de el parametro */
    }

    ~confString( void ) {

        free( mDato ); 
    }

    void set( CTYPE_STRING pDato, int pLargo ) {
        
        if( mDato ) {
            free( mDato );
        }
        mLargo = pLargo;
        mDato = (CTYPE_STRING) malloc( pLargo + 1 );
        safeStrCopy( mDato, pDato, pLargo );
    }

    CTYPE_STRING get( void ) {
        /* Se devuelve el puntero corriendo el riesgo de que se modifique */
        return mDato;
    }

    int getLargo( void ) {
        
        return mLargo;
    }
};

#endif  //__cplusplus