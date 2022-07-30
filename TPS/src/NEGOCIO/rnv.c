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
#include <cio.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <dd.h> 
#include <driver.h> 
#include <log.h> 
#include <malloc.h> 
#include <mensaje.h> 
#include <modo_eje.h> 
#include <rnv.h> 
#include <tkt.h> 
//#include <io.h>
#include <path.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef INVEL_L
#include <sys/types.h>
#include <unistd.h>
#include <sys/io.h>
#else
#include <dos.h>
#include <io.h>
#endif
static int handle_rnv;
extern    char drivevirtual;
/*****************************************************************************/
void DRIVER_MEMORY( int comando, long offset, char *valor, int largo )
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    char buf_mens[80];
    struct _rnv_acceso
    {
        int rta;
        long offset;
        unsigned bytes;
        char *buffer;
    };
    struct _rnv_acceso rnv_acceso;
    char tmp[100];
    if( largo > LONGITUD_MEMORIA_DRIVER_INVEL ) {
        CLOSE_ALL();
        GRABAR_LOG_SISTEMA( "error longitud de memoria driver excedida en RNV" );
        sprintf( tmp, "comando %d", comando );
        GRABAR_LOG_SISTEMA( tmp );
        sprintf( tmp, "offset %d", offset );
        GRABAR_LOG_SISTEMA( tmp );
        sprintf( tmp, "largo %u", largo );
        GRABAR_LOG_SISTEMA( tmp );
        printf( "\n\n error longitud de memoria driver excedida en RNV\n\n" );
        exit( 1 );
    }
    if( valor && largo >= 0 ) {
        memcpy( puntero_memoria_driver_invel, valor, largo );
    }
    rnv_acceso.offset = offset;
    rnv_acceso.bytes = largo;
    rnv_acceso.buffer = puntero_memoria_real_driver_invel;
    //   printf("Real segment: %04X  selector: %04X %p  %p\n",paragraph,selector,rnv_acceso.buffer,pun);
    switch( comando ) {
        case _DM_GET:
            /*------------- Trae valor desde la memoria --------------*/
            if( valor ) {
                DRIVER( _DRV_rnv, _RNV_leer, 0, &rnv_acceso, sizeof( struct _rnv_acceso ) );
            }
            break;
        case _DM_SET:
            /*-------------- Coloca valor en la memoria ---------------*/
            if( valor ) {
                DRIVER( _DRV_rnv, _RNV_escribir, 0, &rnv_acceso, sizeof( struct _rnv_acceso ) );
            }
            break;
        case _DM_CER:
            /*--------------- Cerea un area de memoria ---------------*/
            DRIVER( _DRV_rnv, _RNV_cerear, 0, &rnv_acceso, sizeof( struct _rnv_acceso ) );
            break;
        case _DM_SIZE:
            /*--------------- Pide tamao de la memoria -------------*/
            DRIVER( _DRV_rnv, _RNV_configuracion, 0, &rnv_acceso, sizeof( struct _rnv_acceso ) );
            *( long* )puntero_memoria_driver_invel = rnv_acceso.offset;
            break;
    }
    if( valor && largo >= 0 ) {
        memcpy( valor, puntero_memoria_driver_invel, largo );
    }
    if( rnv_acceso.rta && ( MODO_DEBUG == 4 || MODO_DEBUG == 50 ) ) {
        sprintf( buf_mens, "RNV: OP[%i] ERR[%i] Off[%li] Buf[%p] Bytes[%i]", comando,
                 rnv_acceso.rta, rnv_acceso.offset, rnv_acceso.buffer, rnv_acceso.bytes );
        MENSAJE( buf_mens );
    }
    #endif
}
/*****************************************************************************/
int INIT_RNV()
/*****************************************************************************/
{
    int error = 0;
    DRIVER( _DRV_rnv, _RNV_inicializacion, 0, &error, sizeof( error ) );
    return ( !error );
}
/*****************************************************************************/
int SET_RNV( int estado )
/*****************************************************************************/
{
    int error = 0;
    #if !defined(INVEL_W) && !defined(INVEL_L)
    DRIVER( _DRV_rnv, _RNV_set, estado, &error, sizeof( error ) );
    #endif
    return ( !error );
}
/*****************************************************************************/
void DRIVER_MEMORY_VIRTUAL( int comando, long offset, char *valor, int largo )
/*****************************************************************************/
{
    char buffer[100];
    //char buf_mens[80];
    int nro,cant;
    nro = NRO_ARCHIVO;
    NRO_ARCHIVO = 0;
    switch( comando ) {
        case _DM_GET:
            /*------------- Trae valor desde la memoria --------------*/
            LSEEK( handle_rnv, offset, SEEK_SET, __LINE__, __FILE__ );
            if( valor ) {
                _READ( handle_rnv, valor, largo, __LINE__, __FILE__ );
            }
            break;
        case _DM_SET:
            /*-------------- Coloca valor en la memoria ---------------*/
            LSEEK( handle_rnv, offset, SEEK_SET, __LINE__, __FILE__ );
            if( valor ) {
                _WRITE( handle_rnv, valor, largo, __LINE__, __FILE__ );
            }
            break;
        case _DM_CER:
            /*--------------- Cerea un area de memoria ---------------*/
            memset( buffer, 0, 100 );
            LSEEK( handle_rnv, offset, SEEK_SET, __LINE__, __FILE__ );
            while( largo ) {
                cant = ( largo > 100 ) ? 100 : largo;
                _WRITE( handle_rnv, buffer, cant, __LINE__, __FILE__ );
                largo -= cant;
            }
            break;
        case _DM_SIZE:
            /*--------------- Pide tamao de la memoria -------------*/
            // DRIVER( _DRV_rnv, _RNV_configuracion, 0, &rnv_acceso );
            //*(long *)valor = rnv_acceso.offset;
            break;
    }
    /* if( MODO_DEBUG == 4 && offset < 10L ) { sprintf( buf_mens, "RNV:
     * OP[%i] Off[%li] Buf[%p] Bytes[%i] %x %x %x",comando, offset, valor,
     * largo, valor[0], valor[1], valor[2] ); MENSAJE( buf_mens ); getch(); } */
    NRO_ARCHIVO = nro;
}
/*****************************************************************************/
void OPEN_RNV_VIRTUAL()
/*****************************************************************************/
{
    OPEN_ARCHIVO_RNV_VIRTUAL( RNV_DAT );
}

/*****************************************************************************/
void OPEN_ARCHIVO_RNV_VIRTUAL( char *nom_arch_rnv )
/*****************************************************************************/
{
	int       a, nro;
	char      nombre[25];
	
	memset( nombre, 0, sizeof( nombre ) );

	#ifdef INVEL_L
    handle_rnv = open( RNV_DAT, O_RDWR | O_SYNC );
    #else 
	if( _RNV_VIRTUAL ) {
		nro = NRO_ARCHIVO;
		NRO_ARCHIVO = 0;
		#ifdef INVEL_L
		sprintf( nombre, RNV_DAT );
		a = access( nombre, R_OK | W_OK );
		#else
		_snprintf(nombre, sizeof(nombre)-1, "%c:%s", drivevirtual, nom_arch_rnv );
		a = access( nombre, 0 );
		#endif
		if( a != 0 ) {
			//handle_rnv = _CREAT( nombre, FA_ARCH, __LINE__, __FILE__ );
			handle_rnv = _CREAT( nombre, S_IFREG | S_IWRITE | S_IREAD, __LINE__, __FILE__ );
			if( handle_rnv ) {
				char     *aux;
				long      cant = 0x8000;
				LSEEK( handle_rnv, 0, SEEK_SET, __LINE__, __FILE__ );
				aux = MALLOC( cant );
				if( aux ) {
					memset( aux, 0, cant );
					_WRITE( handle_rnv, aux, cant, __LINE__, __FILE__ );
					free( aux );
					//aux = NULL;
				}
			}
			_CLOSE( handle_rnv, __LINE__, __FILE__ );
			//handle_rnv = 0;
		}
		#ifdef INVEL_L
		handle_rnv = _OPEN( nombre, O_RDWR, __LINE__, __FILE__ );
		#else
		handle_rnv = _OPEN( nombre, O_RDWR | O_BINARY, __LINE__, __FILE__ );
		#endif
		if( handle_rnv <= 0 ) {
			ABORT( "FALLO EN ARCHIVO DE MEMORIA VIRTUAL" );
		}
   		NRO_ARCHIVO = nro;
	}
	#endif
}

/*****************************************************************************/
void CLOSE_RNV_VIRTUAL()
/*****************************************************************************/
{
    int nro;
    #ifdef INVEL_L
    close( handle_rnv );
    #else
    if( _RNV_VIRTUAL ) {
        nro = NRO_ARCHIVO;
        NRO_ARCHIVO = 0;
        if( handle_rnv ) {
			//char debug[100];
			//sprintf(debug, "Close Handle RNV %i\n", handle_rnv );
            _CLOSE( handle_rnv, __LINE__, __FILE__ );
        }
        NRO_ARCHIVO = nro;
    }
    #endif
}
/*****************************************************************************/
int CLOSE_RNV()
/*****************************************************************************/
{
    int error = 0;
    if( !_RNV_VIRTUAL ) {
        #if !defined(INVEL_W) && !defined(INVEL_L)
        DRIVER( _DRV_rnv, _RNV_fin, 0, &error, sizeof( error ) );
        #endif
    }
    return ( !error );
}
/*****************************************************************************/
int GRABAR_RNV_DESDE_ARCHIVO( char *filename )
/*****************************************************************************/
{
    char *p,*comp;
    int handle, iOk = 0;
    handle = _OPEN( filename, O_RDONLY, __LINE__, __FILE__ );
    if( !handle ) {
        return 0;
    }
    p = (char *)MALLOC( LONGITUD_MEMORIA_DRIVER_INVEL );
    if( !p ) {
        return 0;
    }
    comp = (char *)MALLOC( LONGITUD_MEMORIA_DRIVER_INVEL );
    if( !comp ) {
        FREEMEM( p );
        return 0;
    }
    if( _READ( handle, p, LONGITUD_MEMORIA_DRIVER_INVEL, __LINE__, __FILE__ ) != 0 ) {
        return 0;
    }
    _CLOSE( handle, __LINE__, __FILE__ );
    DRIVER_MEMORY( _DM_SET, 0, p, LONGITUD_MEMORIA_DRIVER_INVEL );
    DRIVER_MEMORY( _DM_GET, 0, comp, LONGITUD_MEMORIA_DRIVER_INVEL );
    if( memcmp( p, comp, LONGITUD_MEMORIA_DRIVER_INVEL ) == 0 ) {
        iOk = 1;
    }
    FREEMEM( p );
    FREEMEM( comp );
    return iOk;
}
/*****************************************************************************/
int GRABAR_RNV_EN_ARCHIVO( char *filename )
/*****************************************************************************/
{
    char *comp;
    int iOk = 0;
    int handle;
    handle = _OPEN( filename, O_CREAT | O_TRUNC | O_RDWR, __LINE__, __FILE__ );
    if( !handle ) {
        return 0;
    }
    comp = MALLOC( LONGITUD_MEMORIA_DRIVER_INVEL );
    if( !comp ) {
        return 0;
    }
    DRIVER_MEMORY( _DM_GET, 0, comp, LONGITUD_MEMORIA_DRIVER_INVEL );
    if( _WRITE( handle, comp, LONGITUD_MEMORIA_DRIVER_INVEL, __LINE__, __FILE__ ) == 0 ) {
        iOk = 1;
    }
    FREEMEM( comp );
    return iOk;
}

