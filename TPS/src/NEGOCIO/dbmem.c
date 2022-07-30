#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <archivos.h> 
#include <btrv.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <malloc.h> 
#include <mstring.h>
#include <deftables.h>
#include <db.h>
#include <dbmem.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#define MAX_AREA_DBMEM 20
struct _dbmem
{
    char *p_reg;
    char *cache;
    int c_reg;
    int l_reg;
    int n_reg;
    char bof;
    char eof;
};
/*------------------- Estructuras y variables globales --------------------*/
static struct _dbmem *_dbmem[MAX_AREA_DBMEM];
static char _area;
void _DBMEM_SELECT( int area );
int _DBMEM_LOAD( int area, char *nombre );
void _DBMEM_SET_RECORD( int area, char *p_reg );
void _DBMEM_UNLOAD( int area );
static void _DBMEM_ACTUALIZA_REC();
int DBMEM_AREA_DISPONIBLE();
static void _DBMEM_SET_STATUS();
void DBMEM_RECORRER( int cod, int ( *funcion )(), char *datos );
void _DBMEM_RECORRER( int area, int ( *funcion )(), char *datos );
/*****************************************************************************/
void DBMEM_SELECT( int cod )
/*****************************************************************************/
{
    _DBMEM_SELECT( AREA_OF( cod ) );
}

/*****************************************************************************/
void _DBMEM_SELECT( int area )
/*****************************************************************************/
{
    if( area >= 0 && area < MAX_AREA_DBMEM ) {
        _area = area;
    }
    else {
        MENSAJE_STRING( S_AREA_INVALIDA );
    }
}

/*****************************************************************************/
int DBMEM_LOAD( int cod )
/*****************************************************************************/
{
    char nombre[80];

    NOM_ARCHIVO( cod, nombre );
    SET_AREA( cod, DBMEM_AREA_DISPONIBLE() );

    return ( _DBMEM_LOAD( AREA_OF( cod ), nombre ) );
}

/*****************************************************************************/
int _DBMEM_LOAD( int area, char *nombre )
/*****************************************************************************/
{
    int h, ok = 0;

    _DBMEM_SELECT( area );
    _dbmem[area] = (struct _dbmem *)MALLOC( sizeof( struct _dbmem ) );
    if( _dbmem[area] && !USE_DATABASE( 0, nombre, NULL, 0, NULL, 0, 0 ) ) {
        _dbmem[area]->l_reg = SIZEOF_REC_DB( 0 );
        _dbmem[area]->c_reg = ( int )RECCOUNT();
        _dbmem[area]->p_reg = BUFFER_REC_DB( 0 );
        _dbmem[area]->cache = ( _dbmem[area]->c_reg )
                            ? MALLOC( _dbmem[area]->c_reg * _dbmem[area]->l_reg ) : NULL;
        if( _dbmem[area]->cache ) {
            ok = 1;
            for( h = 0;h < _dbmem[area]->c_reg;h++ ) {
                memcpy( &( _dbmem[area]->cache[h * _dbmem[area]->l_reg] ), _dbmem[area]->p_reg,
                        _dbmem[area]->l_reg );
                SKIP( 1 );
            }
        }
        _dbmem[area]->p_reg = NULL;
    }
    CLOSE_DATABASE( 0 );

    return ( ok );
}

/*****************************************************************************/
void DBMEM_UNLOAD( int cod )
/*****************************************************************************/
{
    _DBMEM_UNLOAD( AREA_OF( cod ) );
}

/*****************************************************************************/
void _DBMEM_UNLOAD( int area )
/*****************************************************************************/
{
    if( _dbmem[area] ) {
        if( _dbmem[area]->cache ) {
            free( _dbmem[area]->cache );
        }
        free( _dbmem[area] );
        _dbmem[area] = NULL;
    }
}

/*****************************************************************************/
void DBMEM_SET_RECORD( int cod, char *p_reg )
/*****************************************************************************/
{
    _DBMEM_SET_RECORD( AREA_OF( cod ), p_reg );
}

/*****************************************************************************/
void _DBMEM_SET_RECORD( int area, char *p_reg )
/*****************************************************************************/
{
    if( _dbmem[area] ) {
        _dbmem[area]->p_reg = p_reg;
    }
}

/*****************************************************************************/
void DBMEM_GO( int registro )
/*****************************************************************************/
{
    switch( registro ) {
        case TOP:
            _dbmem[( int )_area]->n_reg = 0;
            break;
        case BOTTOM:
            _dbmem[( int )_area]->n_reg = _dbmem[( int )_area]->c_reg - 1;
            break;
        default:
            _dbmem[( int )_area]->n_reg = registro;
            break;
    }
    _DBMEM_SET_STATUS();
    _DBMEM_ACTUALIZA_REC();
}

/*****************************************************************************/
void DBMEM_SKIP( int salto )
/*****************************************************************************/
{
    _dbmem[( int )_area]->n_reg += salto;
    _DBMEM_SET_STATUS();
    _DBMEM_ACTUALIZA_REC();
}

/*****************************************************************************/
int DBMEM_BOF()
/*****************************************************************************/
{
    return ( _dbmem[( int )_area]->bof );
}

/*****************************************************************************/
int DBMEM_EOF()
/*****************************************************************************/
{
    return ( _dbmem[( int )_area]->eof );
}

/*****************************************************************************/
static void _DBMEM_SET_STATUS()
/*****************************************************************************/
{
    _dbmem[( int )_area]->bof = 0;
    _dbmem[( int )_area]->eof = 0;
    if( _dbmem[( int )_area]->n_reg < 0 ) {
        _dbmem[( int )_area]->n_reg = 0;
        _dbmem[( int )_area]->bof = 1;
    }
    else if( _dbmem[( int )_area]->n_reg >= _dbmem[( int )_area]->c_reg ) {
        _dbmem[( int )_area]->n_reg = _dbmem[( int )_area]->c_reg - 1;
        _dbmem[( int )_area]->eof = 1;
    }
}

/*****************************************************************************/
static void _DBMEM_ACTUALIZA_REC()
/*****************************************************************************/
{
    int pos;

    if( _dbmem[( int )_area]->p_reg && _dbmem[( int )_area]->n_reg >= 0 ) {//puse igual a 0 para la primera posicion, porque en el cache la posicion es la o
        pos = _dbmem[( int )_area]->n_reg * _dbmem[( int )_area]->l_reg;
		memcpy( _dbmem[( int )_area]->p_reg, &( _dbmem[( int )_area]->cache[pos] ),
			_dbmem[( int )_area]->l_reg );
    }
}

/*****************************************************************************/
void DBMEM_RECORRER( int cod, int ( *funcion )(), char *datos )
/*****************************************************************************/
{
    _DBMEM_RECORRER( AREA_OF( cod ), funcion, datos );
}

/*****************************************************************************/
void _DBMEM_RECORRER( int area, int ( *funcion )(), char *datos )
/*****************************************************************************/
{
    int area_ant;

    if( _dbmem[( int )area] ) {
        area_ant = _area;
        _DBMEM_SELECT( area );
        _dbmem[( int )area]->p_reg = MALLOC( _dbmem[( int )area]->l_reg );
        if( _dbmem[( int )area]->p_reg ) {
            DBMEM_GO( TOP );
            while( !DBMEM_EOF() ) {
                funcion( _dbmem[( int )area]->p_reg, datos );
                DBMEM_SKIP( 1 );
            }
            free( _dbmem[( int )area]->p_reg );
            _dbmem[( int )area]->p_reg = NULL;
        }
        _area = area_ant;
    }
}

/*****************************************************************************/
int DBMEM_AREA_DISPONIBLE()
/*****************************************************************************/
{
    int area = 0, h;

    for( h = 1;h < MAX_AREA_DBMEM && !area;h++ ) {
        if( !_dbmem[h] ) {
            area = h;
        }
    }

    return ( area );
}

/**************************MySql**********************************************/
int DBMEM_LOAD_SQL( int cod , int pTabla, int pTipo, char * indice)
/*****************************************************************************/
{
    char nombre[80];
    NOM_ARCHIVO( cod, nombre );
    SET_AREA( cod, DBMEM_AREA_DISPONIBLE() );

    return ( _DBMEM_LOAD_SQL( AREA_OF( cod ), nombre , pTabla, pTipo , indice) );
}

/*****************************************************************************/
int _DBMEM_LOAD_SQL( int area, char *nombre, int pTabla, int pTipo ,char * indice)
/*****************************************************************************/
{
    int h, ok = 0;

    _DBMEM_SELECT( area );
    _dbmem[area] = MALLOC( sizeof( struct _dbmem ) );
    if( _dbmem[area] && !OPEN_TABLE( pTabla, pTipo, NULL, 0 ) ) {
		//SELECT_TABLE(pTabla, pTipo);
        _dbmem[area]->l_reg = SQL_ROW_SIZE(pTabla); //SIZEOF_REC_DB( 0 );
		_dbmem[area]->p_reg = (char*)malloc(_dbmem[area]->l_reg);//BUFFER_REC_DB( 0 );
		memset( _dbmem[area]->p_reg, 0, _dbmem[area]->l_reg );
		CLOSE_TABLE( pTabla, pTipo );
		//         int pTabla, int pTipo, char *pEstruct, long pSizeof
		if(!OPEN_TABLE( pTabla, pTipo, _dbmem[area]->p_reg, _dbmem[area]->l_reg )){
		//SELECT_TABLE( pTabla, pTipo );
        _dbmem[area]->c_reg = ( int )RECCOUNT2(NULL,NULL);
        _dbmem[area]->cache = ( _dbmem[area]->c_reg )
                            ? MALLOC( _dbmem[area]->c_reg * _dbmem[area]->l_reg ) : NULL;
		SET_WHERE("");
		if( indice !=NULL )
			SET_ORDER2(indice);
		RUN_QUERY(NO);
        if( _dbmem[area]->cache ) {
            ok = 1;
            for( h = 0;h < _dbmem[area]->c_reg;h++ ) {
                memcpy( &( _dbmem[area]->cache[h * _dbmem[area]->l_reg] ), _dbmem[area]->p_reg,
                        _dbmem[area]->l_reg );
                SKIP2( 1 );
            }
        }
        _dbmem[area]->p_reg = NULL;
		}
    }
    CLOSE_TABLE( pTabla, pTipo );

	return ( ok );
}

