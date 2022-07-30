/*
 *   Sistema: TPV 1000 - IV
 *
 *   Definicion de un contenedor de ceTipos
 *
 *   @author: Nicolas Zandarin (2007)
 */

#pragma once
#include "confTypes.h"
#include "ddtpv.h"

class confContainer
{

private:
    /* No vale la pena hacerlo dinamco ya que hay que volver a compilar
     * en cada cambio de ddtpv.h */
    confTypes* base[CANTIDAD_VARIABLES];
    void typeError( int posicion, ctype_enum solicitado, ctype_enum encontrado );

public:
    confContainer(void);
    ~confContainer(void);
    
    void setInt( int posicion , CTYPE_INT dato );
    void setFloat( int posicion , CTYPE_FLOAT dato );
    void setDouble( int posicion , CTYPE_DOUBLE dato );
    void setUnsigned( int posicion , CTYPE_UNSIGNED dato );
    void setLong( int posicion , CTYPE_LONG dato );
    void setUnsignedLong( int posicion, CTYPE_ULONG dato );
    void setString( int posicion , CTYPE_STRING dato, int largo );

    CTYPE_INT getInt( int posicion );
    CTYPE_FLOAT getFloat( int posicion );
    CTYPE_DOUBLE getDouble( int posicion );
    CTYPE_UNSIGNED getUnsigned( int posicion );
    CTYPE_LONG getLong( int posicion );
    CTYPE_ULONG getUnsignedLong( int posicion );
    CTYPE_STRING getString( int posicion );

    int getStringLen( int posicion );
};
