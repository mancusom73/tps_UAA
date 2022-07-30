#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <abort.h> 
#include <btrv.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <deftables.h>
#include <dd.h> 
#include <rnv.h> 
#include <_dd.h> 
#include <path.h>
#include <db.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef EOF
#if !defined(INVEL_L) && !defined(INVEL_W)
#endif
#define _DD_ENTRADAS_RESERVADAS 50
static long l_clase[11],l_dd,l_est;
static char *ptr_clase[11];
struct _dd
{
    char clase;
    long offset;
    unsigned largo;
    unsigned elementos;
    char tipo_variable;
};
struct _est
{
    int cod;
    int var;
};
struct _dd *_DD;
struct _est *_EST;
void ABORT( char *t );
char * _DD_EXEC_NEW( int comando, int clase, long offset, unsigned largo );
/*****************************************************************************/
int GET_TYPE( int cod )
/*****************************************************************************/
{
	return _DD[cod].tipo_variable;
}
/*****************************************************************************/
void DD_INIT()
/*****************************************************************************/
{
    unsigned h;
    char cadena[60];
    long a = 0;
    char auxi[100];
    long cant_var,recc;

    if( !DD_ABRIR_ARCHIVOS() ) {
        ABORT( "Error al inicializar DD" );
    }
    else {
        /*------------------- Control de Variables -------------------------*/
        cant_var = CANTIDAD_VARIABLES;
        recc = RECCOUNT2(NULL,NULL);
        if( CANTIDAD_VARIABLES > recc ) {
			ABORT( "DIFERENCIAS EN DICCIONARIO" );
        }
        /*--------- Determina los tamaos de cada clase ----------*/
        for( h = 1;h < 10;h++ ) {
            l_clase[h] = DD_LEER_COD_ESPECIAL( ( unsigned short ) ( 10000 + h ) );
        }
        /*--------- Controla el tamao de la RNV ----------*/
        //printf( "RNV TOTAL: %ld RNV USADA %ld. Presione ESC.\n", GET_LONGITUD_RNV(), l_clase[_VAR_RNV] );
        //getch();
        if( l_clase[_VAR_RNV] > GET_LONGITUD_RNV() ) {
            _snprintf(cadena, sizeof(cadena)-1,
                     "DD Error: No hay memoria RNV suficiente (%li pedidos - %li disponibles)",
                     l_clase[_VAR_RNV], GET_LONGITUD_RNV() );
            ABORT( cadena );
        }
        /*--------- Pide memoria RAM para las clases ----------*/
        for( h = 1;h < 10;h++ ) {
            if( l_clase[h] ) {
                ptr_clase[h] = ( char* )malloc( l_clase[h] );
                a += l_clase[h];
                _snprintf(auxi, sizeof(auxi)-1, "clase %d :%ld  total %ld", h, l_clase[h], a );
                //MENSAJE(auxi);
                if( !ptr_clase[h] ) {
                    _snprintf(cadena, sizeof(cadena)-1, "DD Error: No hay memoria para clase %i", h );
                    ABORT( cadena );
                }
                else {
                    memset( ptr_clase[h], 0, l_clase[h] );
                }
            }
        }
        /*------------ Determina tamao del DD y pide memoria ---------------*/
        l_dd = DD_LEER_COD_ESPECIAL( 10000 );
        l_est = DD_LEER_COD_ESPECIAL( 10010 );
        _DD = ( struct _dd * )calloc( ( l_dd + _DD_ENTRADAS_RESERVADAS ), sizeof( struct _dd ) );
        /*( struct _dd * ) malloc( ( l_dd +
        _DD_ENTRADAS_RESERVADAS ) * sizeof( struct _dd ) );*/
        /*_EST = ( struct _est * ) malloc( ( l_est + 1 ) * sizeof( struct _est ) );*/
        _EST = ( struct _est * )calloc( l_est + 1, sizeof( struct _est ) );
        if( !_DD || !_EST ) {
            _snprintf(cadena, sizeof(cadena)-1, "DD Error: No hay memoria para DD (entradas: %li+%li)", l_dd, l_est );
            ABORT( cadena );
        }
        /*------------------- Carga DD de variables ------------------------*/
        for( h = 1;h <= ( unsigned )l_dd;h++ ) {
            SET_WHERE("COD_VARIABLE = %d", h);
			//GET_EQUAL( ( char* )&h );
			RUN_QUERY(NO);
            if( FOUND2() ) {
                DD_INS( dd_var.cod_variable, (int)dd_var.clase, dd_var.offset, dd_var.longitud,
                        dd_var.elementos, dd_var.tipo_variable );
                if( dd_var.clase == _VAR_USUARIO || dd_var.clase == _VAR_INT_INI ) {
                    SET_MEMORY( dd_var.cod_variable, dd_var.valor_inicial );
                }
            }
            else {
                memset( &_DD[dd_var.cod_variable], 0, sizeof( struct _dd ) );
            }
        }
        /*------------------- Carga DD de estructuras ----------------------*/
		SELECT_TABLE(DD_EST2, TT_ORIG);
        #ifdef INVEL_L
        //SET_ORDER( 3 );
		SET_ORDER2("COD_VARIABLE");
        #endif
		GO2(TOP);
		RUN_QUERY(NO);
        for( h = 0; !dbEOF() && h < ( unsigned )l_est;h++ ) {
            _EST[h].cod = dd_est2.cod_estructura;
            _EST[h].var = dd_est2.cod_variable;
			SKIP2(1);
        }
        if( h != ( unsigned )l_est || !dbEOF() ) {
			DIRECT_DB_QUERY("DELETE FROM DD_EST2");
			USE_DATABASE( AREA_AUX7, DD_EST2_SIC, ( char* )&dd_est2_btrieve, sizeof( struct _dd_est2/*struct _dd_est2_btr*/ ), NULL, 0, 0 );
			GO(TOP);
			SELECT_TABLE( DD_EST2, TT_ORIG );
			while( !BtrvEOF( ) ) {
				memcpy(&dd_est2,&dd_est2_btrieve, sizeof( struct _dd_est2 ) );
				INSERT2(NO);
				SKIP(1);
			}
			GO2(TOP);
			RUN_QUERY(NO);
			for( h = 0; !dbEOF() && h < ( unsigned )l_est;h++ ) {
				_EST[h].cod = dd_est2.cod_estructura;
				_EST[h].var = dd_est2.cod_variable;
				SKIP2(1);
			}
			if( h != ( unsigned )l_est || !dbEOF() ) {
				ABORT( "DD Error: Inconsistencia en DD_EST2" );
			}
			CLOSE_DATABASE( AREA_AUX7 );
        }
        _EST[l_est].cod = 999;
        // Para compatibilidad durante pruebas //
        //ptr_clase[10] = MK_FP( SEG_MNV, 0 );
        /*--------------- Carga valores de RNV en el cache ----------------*/
        if( !_RNV_VIRTUAL ) {
            DRIVER_MEMORY( _DM_GET, 0, ptr_clase[_VAR_RNV], l_clase[_VAR_RNV] );
        }
        else {
            DRIVER_MEMORY_VIRTUAL( _DM_GET, 0, ptr_clase[_VAR_RNV], l_clase[_VAR_RNV] );
        }
    }
	CLOSE_TABLE(DD_VAR, TT_ORIG);
	CLOSE_TABLE(DD_EST2, TT_ORIG);
}
/*****************************************************************************/
int DD_ABRIR_ARCHIVOS( void )
/*****************************************************************************/
{
    int rta = 0;
    int h = 0;
    //int error;
    /*struct _dd_var __DD_VAR;
    
	
	error = USE_DATABASE( AREA_AUX, DD_VAR_SIC, ( char* )&__DD_VAR, sizeof( struct _dd_var ), NULL, 0, 0 );

    SELECT( AREA_AUX );

	error = OPEN_TABLE(DD_VAR, TT_ORIG ,(char*)&dd_var, sizeof(struct _dd_var ));
    
	while(!BtrvEOF()){
    memcpy((void *)&dd_var, (void *)&__DD_VAR,(int)sizeof( struct _dd_var ));
    INSERT2(NO);
    SKIP(1);
    }*/
	
	
	if( /*USE_DATABASE( AREA_AUX, DD_VAR_SIC, ( char* )&dd_var, sizeof( dd_var ), NULL, 0, 0 )*/ 
			OPEN_TABLE(DD_VAR, TT_ORIG ,(char*)&dd_var, sizeof(struct _dd_var ))== 0 ) {
			//SET_ORDER( 3 ); 
		    SET_ORDER2("COD_VARIABLE");			
			if( /*USE_DATABASE( AREA_AUX2, DD_EST2_SIC, ( char* )&dd_est2, sizeof( dd_est2 ), NULL, 0, 0 )*/ 
				OPEN_TABLE(DD_EST2, TT_ORIG ,(char*)&dd_est2, sizeof(struct _dd_est2 ))== 0 ) {
				//SET_ORDER( 4 );
				SET_ORDER2("COD_ESTRUCTURA, ORDEN");
				//SELECT( AREA_AUX );
				SELECT_TABLE(DD_VAR, TT_ORIG);
				rta = 1;
			}
	}
    return ( rta );
}
/*****************************************************************************/
long DD_LEER_COD_ESPECIAL( unsigned short var )
/*****************************************************************************/
{
    long rta = 0;
    //GET_EQUAL( ( char* )&var );
	SET_WHERE("COD_VARIABLE = %d",var);
	RUN_QUERY(NO);
    if( FOUND2() ) {
        rta = *( long* )dd_var.valor_inicial;
    }
    return ( rta );
}
/*****************************************************************************/
void DD_INS( int var, int clase, long offset, unsigned largo, int elementos, char tipo_variable )
/*****************************************************************************/
{
    _DD[var].clase = clase;
    _DD[var].offset = offset;
    _DD[var].largo = largo;
    _DD[var].elementos = elementos;
	_DD[var].tipo_variable = tipo_variable;
}
/*****************************************************************************/
long GET_LONGITUD_RNV( void )
/*****************************************************************************/
{
    long rta;
    if( _RNV_VIRTUAL ) {
        rta = 0xfff0;
    }
    else {
        DRIVER_MEMORY( _DM_SIZE, 0, ( char* )&rta, 0 );
    }
    return ( rta );
}
/*****************************************************************************/
void DD_EXEC( int comando, int var, int subindice, char *valor )
/*****************************************************************************/
{
    long offset;
    char cadena[90];
    if( subindice > ( signed )_DD[var].elementos ) {
        _snprintf(cadena, sizeof(cadena)-1, "DD Error: Subindice overflow en variable %i (max:%i, req:%i)", var,
                 _DD[var].elementos, subindice );
        ABORT( cadena );
	} else {
		if( subindice < 0) {
			_snprintf(cadena, sizeof(cadena)-1, "DD Error: Subindice NEGATIVO VARIABLE %i (max:%i, req:%i)", var,
                 _DD[var].elementos, subindice );
			glog(cadena, LOG_INCONDICIONAL,1);
		}
    }

    offset = _DD[var].offset + _DD[var].largo * subindice;
    _DD_EXEC( comando, _DD[var].clase, offset, valor, _DD[var].largo );
}
/*****************************************************************************/
char * DD_EXEC_NEW( int comando, int var, int subindice )
/*****************************************************************************/
{
    long offset;
    char cadena[90];
    if( subindice > ( signed )_DD[var].elementos ) {
        _snprintf(cadena, sizeof(cadena)-1, "DD Error: Subindice overflow en variable %i (max:%i, req:%i)", var,
                 _DD[var].elementos, subindice );
        ABORT( cadena );
	} else {
		if( subindice < 0) {
			_snprintf(cadena, sizeof(cadena)-1, "DD Error: Subindice NEGATIVO VARIABLE %i (max:%i, req:%i)", var,
                 _DD[var].elementos, subindice );
			glog(cadena, LOG_INCONDICIONAL,1);
		}
    }

    offset = _DD[var].offset + _DD[var].largo * subindice;
    return _DD_EXEC_NEW( comando, _DD[var].clase, offset, _DD[var].largo );
}
/*****************************************************************************/
void _DD_EXEC( int comando, int clase, long offset, char *valor, unsigned largo )
/*****************************************************************************/
{
    DD_MEMORY( clase, comando, offset, valor, largo );
    if( clase == _VAR_RNV && comando != _DM_GET ) {
        if( !_RNV_VIRTUAL ) {
            DRIVER_MEMORY( comando, offset, valor, largo );
        }
        else {
            DRIVER_MEMORY_VIRTUAL( comando, offset, valor, largo );
        }
    }
}
/*****************************************************************************/
char * _DD_EXEC_NEW( int comando, int clase, long offset, unsigned largo )
/*****************************************************************************/
{
    // o DD_MEMORY_NEW( clase, comando, offset, largo );
	return ptr_clase[clase] + offset;
	
	/*DD_MEMORY( clase, comando, offset, valor, largo );
    if( clase == _VAR_RNV && comando != _DM_GET ) {
        if( !_RNV_VIRTUAL ) {
            DRIVER_MEMORY( comando, offset, valor, largo );
        }
        else {
            DRIVER_MEMORY_VIRTUAL( comando, offset, valor, largo );
        }
    }*/
}
/*****************************************************************************/
void DD_MEMORY( int clase, int comando, long offset, char *valor, unsigned largo )
/*****************************************************************************/
{
    char *p;
    p = ptr_clase[clase] + offset;
    switch( comando ) {
        case _DM_GET:
            /*------------- Trae valor desde la memoria --------------*/
            if( valor )
                                                                //#ifdef INVEL_L
            {
                memmove( valor, p, largo );
            }
            //#else
            //memcpy( valor, p, largo );
            //#endif
            break;
        case _DM_SET:
            /*-------------- Coloca valor en la memoria ---------------*/
            if( valor )
                                                                    //#ifdef INVEL_L
            {
                memmove( p, valor, largo );
            }
            //#else
            //memcpy( p, valor, largo );
            //#endif
            break;
        case _DM_CER:
            /*--------------- Crerea un area de memoria ---------------*/
            memset( p, 0, largo );
            break;
    }
}
/*****************************************************************************/
char * DD_MEMORY_NEW( int clase, int comando, long offset,unsigned largo )
/*****************************************************************************/
{
    return ptr_clase[clase] + offset;
}
/*****************************************************************************/
int DD_ULTIMO_HANDLE()
/*****************************************************************************/
{
    return ( l_dd );
}
/*****************************************************************************/
int DD_MAX_HANDLE()
/*****************************************************************************/
{
    return ( l_dd + _DD_ENTRADAS_RESERVADAS );
}
/*****************************************************************************/
long DD_OFFSET( int var )
/*****************************************************************************/
{
    return ( _DD[var].offset );
}
/*****************************************************************************/
int DD_LARGO( int var )
/*****************************************************************************/
{
    return ( _DD[var].largo );
}
/*****************************************************************************/
char DD_CLASE( int var )
/*****************************************************************************/
{
    return ( _DD[var].clase );
}
/*****************************************************************************/
int DD_ELEMENTOS( int var )
/*****************************************************************************/
{
    return ( _DD[var].elementos );
}
/*****************************************************************************/
int DD_TIPO( int var )
/*****************************************************************************/
{
	return ( ( int )_DD[var].tipo_variable );
}
/*****************************************************************************/
int EST_COD( int var )
/*****************************************************************************/
{
    return ( ( int )_EST[var].cod );
}
/*****************************************************************************/
int EST_VAR( int var )
/*****************************************************************************/
{
    return ( _EST[var].var );
}
/*****************************************************************************/
char * DD_PTR( int var )
/*****************************************************************************/
{
    char *p;
    p = ptr_clase[( int )_DD[var].clase] + _DD[var].offset;
    return ( p );
}
/*****************************************************************************/
void DD_CLOSE()
/*****************************************************************************/
{
    int h;
    for( h = 1;h < 10;h++ ) {
        if( ptr_clase[h] ) {
            free( ptr_clase[h] );
        }
    }
    free( _DD );
    free( _EST );
}

