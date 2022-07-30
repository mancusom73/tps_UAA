/*
 *   Sistema: TPV 1000 - IV
 *
 *   Definicion de metodos para crear y cargar la estructura de datos que almacenara
 *   los datos de configuracion de conftpv
 *
 *   @author: Nicolas Zandarin (2007)
 */

#pragma once
#include "confContainer.h"

#define MAX_LARGO_VALOR 20 //basado en el tamaño definido en la tabla Valor

class confManager
{
private: 
    confContainer* conf;

    void confManager::castingError( int pIdVariable );
    int toLocalType( int codTipo, int tamString );
    bool setVarConfig( int codTipo, int variable, char* valor, int largo );
    
public:
    confManager( confContainer );
    confManager( void );
    ~confManager( void );
    bool load( void );
    confContainer* getData( void );
};
