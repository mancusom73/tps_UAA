#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <abort.h> 
#include <cmemory.h> 
#include <cr.h>  
#include <dbrouter.h> 
#include <dd.h> 
#include <limits.h>
#include <math.h>


/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#include <dos.h>
#endif
extern long DD_OFFSET( int var );
extern int DATOS_RNV_DIRECTO_MEMORIA;
extern void GRABAR_LOG_SISTEMA( char *operacion ,int origen, int nivel );
struct _dd
{
    char clase;
    long offset;
    unsigned largo;
    unsigned elementos;
    char tipo_variable;
};


extern char * _DD_EXEC_NEW( int comando, int clase, long offset, unsigned largo );
extern struct _dd *_DD;
void _ADD_BUFFER_TO_BUFFER( char *origen, char *destino, int tipo );
char aux_temp[256];
void _ADD_BUFFER_TO_STRUCT( char *origen, int var, int sub, int tipo );
char * _GET_MEMORY_STR_NEW( int var, int subindice );
char * GET_MEMORY_STR_NEW( int var);
float GET_MEMORY_FLOAT_NEW( int var );
float _GET_MEMORY_FLOAT_NEW( int var, int subindice );

unsigned long GET_MEMORY_ULONG_NEW( int var );
unsigned long _GET_MEMORY_ULONG_NEW( int var, int subindice );
char GET_MEMORY_CHAR_NEW( int var );
char _GET_MEMORY_CHAR_NEW( int var, int subindice );
unsigned char GET_MEMORY_UCHAR_NEW( int var );
unsigned char _GET_MEMORY_UCHAR_NEW( int var, int subindice );
extern void CEREAR_DATOS_PRIVADOS(void);
int GET_MEMORY_INT_NEW( int var );
int _GET_MEMORY_INT_NEW( int var , int subindice);
unsigned GET_MEMORY_UNSIGNED_NEW( int var );
char * _LOAD_MEMORY_NEW( int var, int subindice );
unsigned _GET_MEMORY_UNSIGNED_NEW( int var, int subindice );
long GET_MEMORY_LONG_NEW( int var );
long _GET_MEMORY_LONG_NEW( int var, int subindice );
char _GET_MEMORY_STR_CHAR_NEW( int var, int subindice, int elemento );
char * GET_MEMORY_STR_NEW( int var);
char * _GET_MEMORY_STR_NEW( int var, int subindice );
double GET_MEMORY_DOUBLE_NEW( int var );
double _GET_MEMORY_DOUBLE_NEW( int var , int subindice );
/*****************************************************************************/
void _LOAD_MEMORY( int var, int subindice, char *valor )
/*****************************************************************************/
{
    long offset = 0;    
	char vacio[10];
	 char cadena[90];
	memset(vacio,0,sizeof(vacio));
	memset(cadena,0,sizeof(cadena));

    if( subindice > ( signed )_DD[var].elementos ) {
       
        _snprintf(cadena, sizeof(cadena)-1, "DD Error: Subindice overflow en variable %i (max:%i, req:%i)", var,
                 _DD[var].elementos, subindice );
        ABORT( cadena );
    }	
    offset = _DD[var].offset + _DD[var].largo * subindice;
	if( var > 0 )///errores derivados de variables no inicializadas
		_DD_EXEC( _DM_GET, _DD[var].clase, offset, valor, _DD[var].largo );
	else{
		memcpy(valor,vacio,_DD[var].largo);
		_snprintf(cadena, sizeof(cadena)-1, "Error: Variable no Inicializada %i (max:%i, req:%i)", var,
                 _DD[var].elementos, subindice );
		GRABAR_LOG_SISTEMA( cadena ,LOG_INCONDICIONAL,1);
	}
	
}


/*****************************************************************************/
char * _LOAD_MEMORY_NEW( int var, int subindice )
/*****************************************************************************/
{
    long offset = 0;    
	char vacio[10];
	char cadena[90];
	memset(vacio,0,sizeof(vacio));
    if( subindice > ( signed )_DD[var].elementos ) {
        
        _snprintf(cadena, sizeof(cadena)-1, "DD Error: Subindice overflow en variable %i (max:%i, req:%i)", var,
                 _DD[var].elementos, subindice );
        ABORT( cadena );
    }	
    offset = _DD[var].offset + _DD[var].largo * subindice;
//    _DD_EXEC( _DM_GET, _DD[var].clase, offset, valor, _DD[var].largo );
	if(var>0)
		return (char *)_DD_EXEC_NEW( _DM_GET, _DD[var].clase, offset, _DD[var].largo );
	else{
		_snprintf(cadena, sizeof(cadena)-1, "Error: Variable no Inicializada %i (max:%i, req:%i)", var,
                 _DD[var].elementos, subindice );
		GRABAR_LOG_SISTEMA( cadena ,LOG_INCONDICIONAL,1);
		return vacio;
	}
}


/*****************************************************************************/
char _GET_MEMORY_STR_CHAR( int var, int subindice, int elemento )
/*****************************************************************************/
{
	char aux=0;
	if(DATOS_RNV_DIRECTO_MEMORIA){
		return _GET_MEMORY_STR_CHAR_NEW( var, subindice, elemento );
	}else{
	
    long offset;
    offset = DD_OFFSET( var ) + DD_LARGO( var ) * subindice + elemento;
	_DD_EXEC( _DM_GET, DD_CLASE( var ), offset, &aux, 1 );
	}
    return ( aux );

}
/*****************************************************************************/
char _GET_MEMORY_STR_CHAR_NEW( int var, int subindice, int elemento )
/*****************************************************************************/
{
    long offset;
    offset = DD_OFFSET( var ) + DD_LARGO( var ) * subindice + elemento;
    return *(char *)_DD_EXEC_NEW( _DM_GET, DD_CLASE( var ), offset, 1 );
}

/*****************************************************************************/
void _SET_MEMORY( int var, int subindice, char *valor )
/*****************************************************************************/
{
    DD_EXEC( _DM_SET, var, subindice, valor );
}
/*****************************************************************************/
void _CEREAR_MEMORY( int var, int subindice )
/*****************************************************************************/
{
    DD_EXEC( _DM_CER, var, subindice, NULL );
}
/*****************************************************************************/
char GET_MEMORY_STR_CHAR( int var, int elemento )
/*****************************************************************************/
{
char a=0;
if( DATOS_RNV_DIRECTO_MEMORIA ){
	return  _GET_MEMORY_STR_CHAR_NEW( var, 0, elemento );
}else{
	a = _GET_MEMORY_STR_CHAR( var, 0, elemento );
}
    return ( a );

}
/*****************************************************************************/
char GET_MEMORY_STR_CHAR_NEW( int var, int elemento )
/*****************************************************************************/
{
    return *(char*) _GET_MEMORY_STR_CHAR_NEW( var, 0, elemento );
    
}
/*****************************************************************************/
char * GET_MEMORY_STR( int var )
/*****************************************************************************/
{
if( DATOS_RNV_DIRECTO_MEMORIA ){
	return GET_MEMORY_STR_NEW( var);
}else{
	_LOAD_MEMORY( var, 0, aux_temp );
}
    return ( aux_temp );


}
/*****************************************************************************/
char * GET_MEMORY_STR_NEW( int var)
/*****************************************************************************/
{
    return _LOAD_MEMORY_NEW( var, 0 );
}
/*****************************************************************************/
char * _GET_MEMORY_STR( int var, int subindice )
/*****************************************************************************/
{
if( DATOS_RNV_DIRECTO_MEMORIA ){
	return _GET_MEMORY_STR_NEW( var, subindice);
}else
    _LOAD_MEMORY( var, subindice, aux_temp );
    
	return ( aux_temp );
}

/*****************************************************************************/
char * _GET_MEMORY_STR_NEW( int var, int subindice )
/*****************************************************************************/
{
    
    return _LOAD_MEMORY_NEW( var, subindice );
}

/*****************************************************************************/
void _COPY_MEMORY( int destino, int sub_des, int origen, int sub_ori )
/*****************************************************************************/
{
    char aux[256];
    _LOAD_MEMORY( origen, sub_ori, aux );
    _SET_MEMORY( destino, sub_des, aux );
}
/*****************************************************************************/
int SIZEOF_VAR( int var )
/*****************************************************************************/
{
    return ( DD_LARGO( var ) );
}
/*****************************************************************************/
int SIZEOF_ARRAY( int var )
/*****************************************************************************/
{
    return ( DD_LARGO( var ) * DD_ELEMENTOS( var ) );
}
/*****************************************************************************/
void SET_MEMORY( int var, char *valor )
/*****************************************************************************/
{
    _SET_MEMORY( var, 0, valor );
}
/*****************************************************************************/
void CEREAR_MEMORY( int var )
/*****************************************************************************/
{
    _CEREAR_MEMORY( var, 0 );
}
/*****************************************************************************/
void COPY_MEMORY( int destino, int origen )
/*****************************************************************************/
{
    _COPY_MEMORY( destino, 0, origen, 0 );
}
/*****************************************************************************/
void CEREAR_ARRAY( int cod )
/*****************************************************************************/
{
    int i;
    for( i = 0;i < DD_ELEMENTOS( cod );i++ ) {
        _CEREAR_MEMORY( cod, i );
    }
}
/*
 * -----------------------------------------------------------------------------
 * COLOCAR VALORES EN LA MEMORIA
 * -----------------------------------------------------------------------------
 */
/*****************************************************************************/
void SET_MEMORY_STR_CHAR( int var, int elemento, char valor )
/*****************************************************************************/
{
    _SET_MEMORY_STR_CHAR( var, 0, elemento, valor );
}
/*****************************************************************************/
void _SET_MEMORY_STR_CHAR( int var, int subindice, int elemento, char valor )
/*****************************************************************************/
{
    long offset;
    offset = DD_OFFSET( var ) + DD_LARGO( var ) * subindice + elemento;
    _DD_EXEC( _DM_SET, DD_CLASE( var ), offset, &valor, 1 );
}
#if defined(INVEL_W) || defined(INVEL_L)
/*****************************************************************************/
void SET_MEMORY_CHAR( int var, int valor )
/*****************************************************************************/
{
    _SET_MEMORY_CHAR( var, 0, valor );
}
/*****************************************************************************/
void _SET_MEMORY_CHAR( int var, int subindice, int valor )
/*****************************************************************************/
{
    char v;
    v = ( char )valor;
    _SET_MEMORY( var, subindice, &v );
}
#else
/*****************************************************************************/
void SET_MEMORY_CHAR( int var, char valor )
/*****************************************************************************/
{
    _SET_MEMORY_CHAR( var, 0, valor );
}
/*****************************************************************************/
void _SET_MEMORY_CHAR( int var, int subindice, char valor )
/*****************************************************************************/
{
    _SET_MEMORY( var, subindice, &valor );
}
#endif
/*****************************************************************************/
void SET_MEMORY_INT( int var, int valor )
/*****************************************************************************/
{
    _SET_MEMORY_INT( var, 0, valor );
}
/*****************************************************************************/
void _SET_MEMORY_INT( int var, int subindice, int valor )
/*****************************************************************************/
{
    short int v;
    v = ( short int ) valor;
    _SET_MEMORY( var, subindice, ( char* )&v );
}
/*****************************************************************************/
void SET_MEMORY_LONG( int var, long valor )
/*****************************************************************************/
{
    _SET_MEMORY_LONG( var, 0, valor );
}
/*****************************************************************************/
void _SET_MEMORY_LONG( int var, int subindice, long valor )
/*****************************************************************************/
{
    _SET_MEMORY( var, subindice, ( char* )&valor );
}
#if defined(INVEL_W) || defined(INVEL_L)
/*****************************************************************************/
void SET_MEMORY_FLOAT( int var, double valor )
/*****************************************************************************/
{
    _SET_MEMORY_FLOAT( var, 0, valor );
}
/*****************************************************************************/
void _SET_MEMORY_FLOAT( int var, int subindice, double valor )
/*****************************************************************************/
{
    float v;
    v = ( float )valor;
    _SET_MEMORY( var, subindice, ( char* )&v );
}
#else
/*****************************************************************************/
void SET_MEMORY_FLOAT( int var, float valor )
/*****************************************************************************/
{
    _SET_MEMORY_FLOAT( var, 0, valor );
}
/*****************************************************************************/
void _SET_MEMORY_FLOAT( int var, int subindice, float valor )
/*****************************************************************************/
{
    float v;
    v = ( float )valor;
    _SET_MEMORY( var, subindice, ( char* )&v );
}
#endif
/*****************************************************************************/
void SET_MEMORY_DOUBLE( int var, double valor )
/*****************************************************************************/
{
    double v;
    v = ( double )valor;
    _SET_MEMORY_DOUBLE( var, 0, v );
}
/*****************************************************************************/
void _SET_MEMORY_DOUBLE( int var, int subindice, double valor )
/*****************************************************************************/
{
    double v;
    v = ( double )valor;
    _SET_MEMORY( var, subindice, ( char* )&v );
}
/*
 * -----------------------------------------------------------------------------
 * OBTENER VALORES DESDE LA MEMORIA
 * -----------------------------------------------------------------------------
 */
/*****************************************************************************/
char GET_MEMORY_CHAR( int var )
/*****************************************************************************/
{
	char aux = 0;
if( DATOS_RNV_DIRECTO_MEMORIA ){
	return GET_MEMORY_CHAR_NEW( var );
}else{

	_LOAD_MEMORY( var, 0, &aux );
}
    return ( aux );

}

/*****************************************************************************/
char GET_MEMORY_CHAR_NEW( int var )
/*****************************************************************************/
{
    
    return*(char* ) _LOAD_MEMORY_NEW( var, 0);
}
/*****************************************************************************/
char _GET_MEMORY_CHAR( int var, int subindice )
/*****************************************************************************/
{		char aux;
	if( DATOS_RNV_DIRECTO_MEMORIA ){
		return _GET_MEMORY_CHAR_NEW( var, subindice );
	}else{

		_LOAD_MEMORY( var, subindice, &aux );
	}
    return ( aux );
}

/*****************************************************************************/
char _GET_MEMORY_CHAR_NEW( int var, int subindice )
/*****************************************************************************/
{
    return *( char * )( _LOAD_MEMORY_NEW( var, subindice ) );
}

/*****************************************************************************/
unsigned char GET_MEMORY_UCHAR( int var )
/*****************************************************************************/
{unsigned char aux;
	if( DATOS_RNV_DIRECTO_MEMORIA ){
		return GET_MEMORY_UCHAR_NEW( var );
	}else{
	    _LOAD_MEMORY( var, 0, ( char* )&aux );
	}
    return ( aux );

}
/*****************************************************************************/
unsigned char GET_MEMORY_UCHAR_NEW( int var )
/*****************************************************************************/
{
    
    return *(unsigned char*)_LOAD_MEMORY_NEW( var, 0 );
    
}
/*****************************************************************************/
unsigned char _GET_MEMORY_UCHAR( int var, int subindice )
/*****************************************************************************/
{
unsigned char aux;
if( DATOS_RNV_DIRECTO_MEMORIA ){
	return _GET_MEMORY_UCHAR_NEW( var , subindice);
}else{

		_LOAD_MEMORY( var, subindice, ( char* )&aux );
}
    return ( aux );

}
/*****************************************************************************/
unsigned char _GET_MEMORY_UCHAR_NEW( int var , int subindice)
/*****************************************************************************/
{
    return *(unsigned char*)_LOAD_MEMORY_NEW( var, subindice );
}
/*****************************************************************************/
int GET_MEMORY_INT( int var )
/*****************************************************************************/
{
	short int aux = 0;

	if( DATOS_RNV_DIRECTO_MEMORIA ){
		return GET_MEMORY_INT_NEW( var );
	}else{
		_LOAD_MEMORY( var, 0, ( char* )&aux );
}
    return ( aux );

}

/*****************************************************************************/
int GET_MEMORY_INT_NEW( int var )
/*****************************************************************************/
{
    return *(short int*)_LOAD_MEMORY_NEW( var, 0 );
}
/*****************************************************************************/
int _GET_MEMORY_INT( int var, int subindice )
/*****************************************************************************/
{
	short int aux;
	if( DATOS_RNV_DIRECTO_MEMORIA ){
		return _GET_MEMORY_INT_NEW( var , subindice);
	}else{
		_LOAD_MEMORY( var, subindice, ( char* )&aux );
	}
    return ( aux );

}
/*****************************************************************************/
int _GET_MEMORY_INT_NEW( int var , int subindice)
/*****************************************************************************/
{
    return *(short int*)_LOAD_MEMORY_NEW( var, subindice );
}
/*****************************************************************************/
unsigned GET_MEMORY_UNSIGNED( int var )
/*****************************************************************************/
{
	unsigned short aux;    
if( DATOS_RNV_DIRECTO_MEMORIA ){
	return GET_MEMORY_UNSIGNED_NEW( var);
}else{

    _LOAD_MEMORY( var, 0, ( char* )&aux );
}
    return ( aux );

}
/*****************************************************************************/
unsigned GET_MEMORY_UNSIGNED_NEW( int var )
/*****************************************************************************/
{
    
    return *(unsigned short *)_LOAD_MEMORY_NEW( var, 0);
    
}
/*****************************************************************************/
unsigned _GET_MEMORY_UNSIGNED( int var, int subindice )
/*****************************************************************************/
{
	    unsigned short aux;
if( DATOS_RNV_DIRECTO_MEMORIA ){
	return _GET_MEMORY_UNSIGNED_NEW( var, subindice);
}else{

    _LOAD_MEMORY( var, subindice, ( char* )&aux );
}
    return ( aux );

}

/*****************************************************************************/
unsigned _GET_MEMORY_UNSIGNED_NEW( int var, int subindice )
/*****************************************************************************/
{
    
    return *(unsigned short *)_LOAD_MEMORY_NEW( var, subindice );
    
}

/*****************************************************************************/
long GET_MEMORY_LONG( int var )
/*****************************************************************************/
{
    long aux;
	if( DATOS_RNV_DIRECTO_MEMORIA ){
		return GET_MEMORY_LONG_NEW( var );
	}else
		_LOAD_MEMORY( var, 0, ( char* )&aux );
    return ( aux );

}/*****************************************************************************/
long GET_MEMORY_LONG_NEW( int var )
/*****************************************************************************/
{
    return *( long *)_LOAD_MEMORY_NEW( var, 0);
}
/*****************************************************************************/
long _GET_MEMORY_LONG( int var, int subindice )
/*****************************************************************************/
{
	long aux;
	if( DATOS_RNV_DIRECTO_MEMORIA ){
		return _GET_MEMORY_LONG_NEW( var, subindice );
	}else
		_LOAD_MEMORY( var, subindice, ( char* )&aux );
    return ( aux );
}
/*****************************************************************************/
long _GET_MEMORY_LONG_NEW( int var, int subindice )
/*****************************************************************************/
{
    return *( long *)_LOAD_MEMORY_NEW( var, subindice);
}
/*****************************************************************************/
unsigned long GET_MEMORY_ULONG( int var )
/*****************************************************************************/
{
	unsigned long aux;    
if( DATOS_RNV_DIRECTO_MEMORIA ){
	return GET_MEMORY_ULONG_NEW( var);
}else
    _LOAD_MEMORY( var, 0, ( char* )&aux );
return ( aux );

}
/*****************************************************************************/
unsigned long GET_MEMORY_ULONG_NEW( int var )
/*****************************************************************************/
{
    return *(unsigned long *)_LOAD_MEMORY_NEW( var, 0 );
}
/*****************************************************************************/
unsigned long _GET_MEMORY_ULONG( int var, int subindice )
/*****************************************************************************/
{
	unsigned long aux;    
if( DATOS_RNV_DIRECTO_MEMORIA ){
	return _GET_MEMORY_ULONG_NEW( var, subindice);
}else

    _LOAD_MEMORY( var, subindice, ( char* )&aux );
    return ( aux );

}

/*****************************************************************************/
unsigned long _GET_MEMORY_ULONG_NEW( int var, int subindice )
/*****************************************************************************/
{
    return *(unsigned long *)_LOAD_MEMORY_NEW( var, subindice);
}
/*****************************************************************************/
float GET_MEMORY_FLOAT( int var )
/*****************************************************************************/
{
	float aux;    
if( DATOS_RNV_DIRECTO_MEMORIA ){
	return GET_MEMORY_FLOAT_NEW( var );
}else

    _LOAD_MEMORY( var, 0, ( char* )&aux );
    return ( aux );

}

/*****************************************************************************/
float GET_MEMORY_FLOAT_NEW( int var )
/*****************************************************************************/
{
    return  * ( float * )_LOAD_MEMORY_NEW( var, 0 );
    
}
/*****************************************************************************/
float _GET_MEMORY_FLOAT( int var, int subindice )
/*****************************************************************************/
{
float aux;    
if( DATOS_RNV_DIRECTO_MEMORIA ){
	return _GET_MEMORY_FLOAT_NEW( var, subindice );
}else

    _LOAD_MEMORY( var, subindice, ( char* )&aux );
return ( aux );

}

/*****************************************************************************/
float _GET_MEMORY_FLOAT_NEW( int var, int subindice )
/*****************************************************************************/
{
    return  * ( float * )_LOAD_MEMORY_NEW( var, subindice);
    
}
/*****************************************************************************/
double GET_MEMORY_DOUBLE( int var )
/*****************************************************************************/
{
	double aux;    
if( DATOS_RNV_DIRECTO_MEMORIA ){
	return  GET_MEMORY_DOUBLE_NEW( var );
}else
    _LOAD_MEMORY( var, 0, ( char* )&aux );
    return ( aux );

}
/*****************************************************************************/
double GET_MEMORY_DOUBLE_NEW( int var )
/*****************************************************************************/
{
    return *(double*) _LOAD_MEMORY_NEW( var, 0 );
}


/*****************************************************************************/
double _GET_MEMORY_DOUBLE( int var, int subindice )
/*****************************************************************************/
{
	double *aux,aux1;    
if( DATOS_RNV_DIRECTO_MEMORIA ){
	return  _GET_MEMORY_DOUBLE_NEW( var, subindice );
}else{

    aux = ( double* )malloc( sizeof( double ) );
    if( aux != NULL ) {
        _LOAD_MEMORY( var, subindice, ( char* )aux );
        aux1 = *aux;
    }
    if( aux ) {
        free( aux );
	}
}
    return ( aux1 );

}

/*****************************************************************************/
double _GET_MEMORY_DOUBLE_NEW( int var, int subindice )
/*****************************************************************************/
{
    return *(double*) _LOAD_MEMORY_NEW( var, subindice);
}
/*
 * -----------------------------------------------------------------------------
 * SUMAR VALORES EN LA MEMORIA
 * -----------------------------------------------------------------------------
 */
/*****************************************************************************/
void ADD_MEMORY_CHAR( int var, char valor )
/*****************************************************************************/
{
    char aux;
	if( DATOS_RNV_DIRECTO_MEMORIA ){
		aux = GET_MEMORY_CHAR_NEW( var);
	}else
		_LOAD_MEMORY( var, 0, &aux );
    aux += valor;
    _SET_MEMORY( var, 0, &aux );
    //_ADD_MEMORY_CHAR( var, 0, valor );
}
/*****************************************************************************/
void _ADD_MEMORY_CHAR( int var, int subindice, char valor )
/*****************************************************************************/
{
    char aux;
	if( DATOS_RNV_DIRECTO_MEMORIA ){
		aux = _GET_MEMORY_CHAR_NEW( var, subindice);
	}else
		_LOAD_MEMORY( var, subindice, &aux );
    aux += valor;
    _SET_MEMORY( var, subindice, &aux );
}
/*****************************************************************************/
void ADD_MEMORY_UCHAR( int var, unsigned char valor )
/*****************************************************************************/
{
    unsigned char aux;
	if( DATOS_RNV_DIRECTO_MEMORIA ){
		aux = GET_MEMORY_UCHAR_NEW( var);
	}else
		_LOAD_MEMORY( var, 0, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, 0, ( char* )&aux );
}
/*****************************************************************************/
void _ADD_MEMORY_UCHAR( int var, int subindice, unsigned char valor )
/*****************************************************************************/
{
    unsigned char aux;
	if( DATOS_RNV_DIRECTO_MEMORIA ){
		aux = _GET_MEMORY_UCHAR_NEW( var, subindice);
	}else
		_LOAD_MEMORY( var, subindice, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, subindice, ( char* )&aux );
}
/*****************************************************************************/
void ADD_MEMORY_INT( int var, int valor )
/*****************************************************************************/
{
    int aux = 0;
if( DATOS_RNV_DIRECTO_MEMORIA ){
	aux = GET_MEMORY_INT_NEW( var);
}else
    _LOAD_MEMORY( var, 0, ( char* )&aux );

    aux += valor;
    _SET_MEMORY( var, 0, ( char* )&aux );
}
/*****************************************************************************/
void _ADD_MEMORY_INT( int var, int subindice, int valor )
/*****************************************************************************/
{
    int aux = 0;
if( DATOS_RNV_DIRECTO_MEMORIA ){
	aux = _GET_MEMORY_INT_NEW( var, subindice);
}else
	_LOAD_MEMORY( var, subindice, ( char* )&aux );

    aux += valor;
    _SET_MEMORY( var, subindice, ( char* )&aux );
}
/*****************************************************************************/
void ADD_MEMORY_UINT( int var, unsigned int valor )
/*****************************************************************************/
{
    unsigned int aux = 0;
if( DATOS_RNV_DIRECTO_MEMORIA ){
	aux = GET_MEMORY_UNSIGNED_NEW( var);
}else
	_LOAD_MEMORY( var, 0, ( char* )&aux );

	aux += valor;
    _SET_MEMORY( var, 0, ( char* )&aux );
}
/*****************************************************************************/
void _ADD_MEMORY_UINT( int var, int subindice, unsigned int valor )
/*****************************************************************************/
{
    unsigned int aux;
if( DATOS_RNV_DIRECTO_MEMORIA ){
	aux = _GET_MEMORY_UNSIGNED_NEW( var,subindice);
}else
    _LOAD_MEMORY( var, subindice, ( char* )&aux );

    aux += valor;
    _SET_MEMORY( var, subindice, ( char* )&aux );
}
/*****************************************************************************/
void ADD_MEMORY_LONG( int var, long valor )
/*****************************************************************************/
{
    long aux;
if( DATOS_RNV_DIRECTO_MEMORIA ){
	aux = GET_MEMORY_LONG_NEW( var);
}else
    _LOAD_MEMORY( var, 0, ( char* )&aux );

    aux += valor;
    _SET_MEMORY( var, 0, ( char* )&aux );
}
/*****************************************************************************/
void _ADD_MEMORY_LONG( int var, int subindice, long valor )
/*****************************************************************************/
{
    long aux;
if( DATOS_RNV_DIRECTO_MEMORIA ){
	aux = _GET_MEMORY_LONG_NEW( var, subindice);
}else
    _LOAD_MEMORY( var, subindice, ( char* )&aux );

    aux += valor;
    _SET_MEMORY( var, subindice, ( char* )&aux );
}
/*****************************************************************************/
void ADD_MEMORY_ULONG( int var, unsigned long valor )
/*****************************************************************************/
{
    unsigned long aux;
if( DATOS_RNV_DIRECTO_MEMORIA ){
	aux = GET_MEMORY_ULONG_NEW( var);
}else
	_LOAD_MEMORY( var, 0, ( char* )&aux );

    aux += valor;
    _SET_MEMORY( var, 0, ( char* )&aux );
}
/*****************************************************************************/
void _ADD_MEMORY_ULONG( int var, int subindice, unsigned long valor )
/*****************************************************************************/
{
    unsigned long aux;
if( DATOS_RNV_DIRECTO_MEMORIA ){
	aux = _GET_MEMORY_ULONG_NEW( var, subindice);
}else
    _LOAD_MEMORY( var, subindice, ( char* )&aux );

    aux += valor;
    _SET_MEMORY( var, subindice, ( char* )&aux );
}
/*****************************************************************************/
void ADD_MEMORY_FLOAT( int var, float valor )
/*****************************************************************************/
{
    float aux;
if( DATOS_RNV_DIRECTO_MEMORIA ){
	aux = GET_MEMORY_FLOAT_NEW( var);
}else
    _LOAD_MEMORY( var, 0, ( char* )&aux );

    aux += valor;
    _SET_MEMORY( var, 0, ( char* )&aux );
}
/*****************************************************************************/
void _ADD_MEMORY_FLOAT( int var, int subindice, float valor )
/*****************************************************************************/
{
    float aux;
if( DATOS_RNV_DIRECTO_MEMORIA ){
	aux = _GET_MEMORY_FLOAT_NEW( var, subindice);
}else
    _LOAD_MEMORY( var, subindice, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, subindice, ( char* )&aux );
}
/*****************************************************************************/
void ADD_MEMORY_DOUBLE( int var, double valor )
/*****************************************************************************/
{
    double aux = 0.0;
if( DATOS_RNV_DIRECTO_MEMORIA ){
	aux = GET_MEMORY_DOUBLE_NEW( var);
}else
	_LOAD_MEMORY( var, 0, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, 0, ( char* )&aux );
}
/*****************************************************************************/
void _ADD_MEMORY_DOUBLE( int var, int subindice, double valor )
/*****************************************************************************/
{
    double aux;
if( DATOS_RNV_DIRECTO_MEMORIA ){
	aux = _GET_MEMORY_DOUBLE_NEW( var, subindice );
}else
	_LOAD_MEMORY( var, subindice, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, subindice, ( char* )&aux );
}
/*
 * --------------------------------------
 * E S T R U C T U R A S
 * --------------------------------------
 */
/*****************************************************************************/
int _APUNTAR_STRUCT( int cod )
/*****************************************************************************/
{
    int h;
    for( h = 0;EST_COD( h ) != 999 && EST_COD( h ) != cod;h++ ) {
        ;
    }
    return ( h );
}
/*****************************************************************************/
void CEREAR_STRUCT( int cod )
/*****************************************************************************/
{
    int h,i;
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
	if( cod == _var_tarjetas ){
		CEREAR_DATOS_PRIVADOS();
	}
#endif
	
    for( h = _APUNTAR_STRUCT( cod );EST_COD( h ) == cod;h++ ) {
        for( i = 0;i < DD_ELEMENTOS( EST_VAR( h ) );i++ ) {
            _CEREAR_MEMORY( EST_VAR( h ), i );
        }
    }
}
/*****************************************************************************/
void COPY_STRUCT( int destino, int origen )
/*****************************************************************************/
{
    int h,i,po,pd;
    po = _APUNTAR_STRUCT( origen );
    pd = _APUNTAR_STRUCT( destino );
    for( h = 0;EST_COD( po + h ) == origen;h++ ) {
        for( i = 0;i < DD_ELEMENTOS( EST_VAR( pd + h ) );i++ ) {
            _COPY_MEMORY( EST_VAR( pd + h ), i, EST_VAR( po + h ), i );
        }
    }
}
/*****************************************************************************/
int SIZEOF_STRUCT( int cod )
/*****************************************************************************/
{
    int h, l = 0;
    for( h = _APUNTAR_STRUCT( cod );EST_COD( h ) == cod;h++ ) {
        l += DD_LARGO( EST_VAR( h ) ) * DD_ELEMENTOS( EST_VAR( h ) );
    }
    return ( l );
}
/*****************************************************************************/
int _SIZEOF_STRUCT( int cod )
/*****************************************************************************/
{
    int h, l = 0;
    for( h = _APUNTAR_STRUCT( cod );EST_COD( h ) == cod;h++ ) {
        l += DD_LARGO( EST_VAR( h ) );
    }
    return ( l );
}
/*****************************************************************************/
void COPY_STRUCT_TO_BUFFER( char *buffer, int origen )
/*****************************************************************************/
{
    int h, pos = 0, v, l;
    for( h = _APUNTAR_STRUCT( origen );EST_COD( h ) == origen;h++ ) {
        v = EST_VAR( h );
        l = DD_LARGO( v ) * DD_ELEMENTOS( v );
        _DD_EXEC( _DM_GET, DD_CLASE( v ), DD_OFFSET( v ), &buffer[pos], l );
        pos += l;
    }
}
/*****************************************************************************/
void COPY_BUFFER_TO_STRUCT( char *buffer, int destino )
/*****************************************************************************/
{
    int h, pos = 0, v, l;
    for( h = _APUNTAR_STRUCT( destino );EST_COD( h ) == destino;h++ ) {
        v = EST_VAR( h );
        l = DD_LARGO( v ) * DD_ELEMENTOS( v );
        _DD_EXEC( _DM_SET, DD_CLASE( v ), DD_OFFSET( v ), &buffer[pos], l );
        pos += l;
    }
}
/*****************************************************************************/
void _COPY_STRUCT_TO_BUFFER( char *buffer, int origen, int subindice )
/*****************************************************************************/
{
    int h, pos = 0, v, l;
    long offset;
    for( h = _APUNTAR_STRUCT( origen );EST_COD( h ) == origen;h++ ) {
        v = EST_VAR( h );
        l = DD_LARGO( v );
        offset = DD_OFFSET( v ) + l * subindice;
        _DD_EXEC( _DM_GET, DD_CLASE( v ), offset, &buffer[pos], l );
        pos += l;
    }
}
/*****************************************************************************/
void _COPY_BUFFER_TO_STRUCT( char *buffer, int destino, int subindice )
/*****************************************************************************/
{
    int h, pos = 0, v, l;
    long offset;
    for( h = _APUNTAR_STRUCT( destino );EST_COD( h ) == destino;h++ ) {
        v = EST_VAR( h );
        l = DD_LARGO( v );
        offset = DD_OFFSET( v ) + l * subindice;
        _DD_EXEC( _DM_SET, DD_CLASE( v ), offset, &buffer[pos], l );
        pos += l;
    }
}
/*****************************************************************************/
void ADD_BUFFER_TO_BUFFER( char *origen, char *destino, int estructura )
/*****************************************************************************/
{
    int h, pos = 0, v, s;
    for( h = _APUNTAR_STRUCT( estructura );EST_COD( h ) == estructura;h++ ) {
        v = EST_VAR( h );
        for( s = 0;s < DD_ELEMENTOS( v );s++ ) {
            _ADD_BUFFER_TO_BUFFER( &origen[pos], &destino[pos], DD_TIPO( v ) );
            pos += DD_LARGO( v );
        }
    }
}
/*****************************************************************************/
void _ADD_BUFFER_TO_BUFFER( char *origen, char *destino, int tipo )
/*****************************************************************************/
{
    switch( tipo ) {
        case _SHORT_INT:
            *destino += *origen;
            break;
        case _INT:
            *( int* )destino += *( int* )origen;
            break;
        case _UNSIGNED:
            *( unsigned * )destino += *( unsigned * )origen;
            break;
        case _LONG:
            *( long* )destino += *( long* )origen;
            break;
        case _FLOAT:
            *( float* )destino += *( float* )origen;
            break;
        case _DOUBLE:
            *( double* )destino += *( double* )origen;
            break;
    }
}
/*****************************************************************************/
int _APUNTAR_STRUCT_VERIFICANDO( int cod )
/*****************************************************************************/
{
    int h;
    for( h = 0;EST_COD( h ) != 999 && EST_COD( h ) != cod;h++ ) {
        ;
    }
    if( EST_COD( h ) == 999 ) {
        h = -1;
    }
    return ( h );
}
/*****************************************************************************/
void ADD_BUFFER_TO_STRUCT( char *origen, int destino )
/*****************************************************************************/
{
    int h, pos = 0, v, s;
    for( h = _APUNTAR_STRUCT( destino );EST_COD( h ) == destino;h++ ) {
        v = EST_VAR( h );
        for( s = 0;s < DD_ELEMENTOS( v );s++ ) {
            _ADD_BUFFER_TO_STRUCT( &origen[pos], v, s, DD_TIPO( v ) );
            pos += DD_LARGO( v );
        }
    }
}
/*****************************************************************************/
void _ADD_BUFFER_TO_STRUCT( char *origen, int var, int sub, int tipo )
/*****************************************************************************/
{
    char charvar;
    int intvar;
    unsigned unsvar;
    long longvar;
    float floatvar;
    double doublevar;
    switch( tipo ) {
        case _SHORT_INT:
            charvar = _GET_MEMORY_CHAR( var, sub ) + *origen;
            _SET_MEMORY_CHAR( var, sub, charvar );
            break;
        case _INT:
            intvar = _GET_MEMORY_INT( var, sub ) + *( int* )origen;
            _SET_MEMORY_INT( var, sub, intvar );
            break;
        case _UNSIGNED:
            unsvar = _GET_MEMORY_UNSIGNED( var, sub ) + *( unsigned * )origen;
            _SET_MEMORY_INT( var, sub, unsvar );
            break;
        case _LONG:
            longvar = _GET_MEMORY_LONG( var, sub ) + *( long* )origen;
            _SET_MEMORY_LONG( var, sub, longvar );
            break;
        case _FLOAT:
            floatvar = _GET_MEMORY_FLOAT( var, sub ) + *( float* )origen;
            _SET_MEMORY_FLOAT( var, sub, floatvar );
            break;
        case _DOUBLE:
            doublevar = _GET_MEMORY_DOUBLE( var, sub ) + *( double* )origen;
            _SET_MEMORY_DOUBLE( var, sub, doublevar );
            break;
    }
}



/*****************************************************************************/
int _CONTROLAR_RANGO_VARIABLES( char *origen, int tipo )
/*Esta funcion controlara que el rango de la variable este dentro de lo aceptable para la caja
Ojo no son solo los limites del tipo de dato.( ej SHRT_MIN <= X <= SHRT_MAX o LONG_MIN <= X <= LONG_MAX
In: cadena con el importe, y tipo con el tipo de variable
*/
/*****************************************************************************/
{
	
	double rangomin;  
	double rangomaz;
	char * pEnd;
	int ok = 1;
	double MINDOUBLE =  -2.22507e+300;
	double MAXDOUBLE =  1.79769e+300;
	rangomin = -1 * pow(10,10);  
	rangomaz = pow(10,10);

	switch( tipo ) {
        case _SHORT_INT:
            break;
        case _INT:
            break;
        case _LONG:
            break;
        case _FLOAT:
            break;
		case _DOUBLE: {
			double d1;
			d1 = strtod (origen,&pEnd);
			if(d1 != 0.00 && !(( d1 >MINDOUBLE ) &&(d1 < MAXDOUBLE))) //control de rangos
				ok = 0;
			//control de caja
			if( d1 < rangomin || d1 > rangomaz )
				ok = 0;
            break;
					  }
    }
	return ok;
}