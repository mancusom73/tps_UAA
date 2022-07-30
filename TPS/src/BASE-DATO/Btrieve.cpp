/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
extern "C" {
#include <malloc.h>
#include <archivos.h>
#include <error.h>
#include <string.h>
}
#pragma pack(1)
#include "cr.h"

#if defined(INVEL_W) || defined(INVEL_L)
#ifndef INVEL_L
#include "stdafx.h"
//#include "afxwin.h"
#endif
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"
#include "btrieve.h"
#include "btrv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#define new DEBUG_NEW
#endif
//#include "btrapi.c"
#include "btrconst.h"

#include "c-tree.h"
#define VACIO        -3

#define MAX_AREA MAX_AREA_BTRV
int CARGAR_DATABASE( char *nombre, char *buffer, int *l_reg );
extern void *MALLOC( unsigned memoria );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BtrieveManager Btrieve;
char tmp[256];
struct _Table
{
    char name[25];
    char newName[25];
};

_Table *FileTable;
extern    "C"
{
    int area__ = 0;
    int UsedByCtree[MAX_AREA];
}


/*****************************************************************************/
WORD BtrieveManager::btrv( int area, WORD operacion, void *buffer, unsigned short *l_buffer,
                           void *key, short *nro_key )
/*****************************************************************************/
{
    BOOL lock,wait,multiple;
    void *p_buffer,*key_buffer;
    unsigned short l_registro,*largo;
    short nro_clave;
    void *pPosBlock = 0;

    /*--------------------- Controla que este en un area activa ---------------*/
    if( operacion == 28 || operacion == 18 || operacion == 19 || operacion == 20
     || operacion == 1519 ) {
        p_buffer = buffer;
        if( nro_key ) {
            nro_clave = *nro_key;
        }
        else {
            nro_clave = 0;
        }
        largo = l_buffer;
        key_buffer = key;
        pPosBlock = tmp;
        memset( pPosBlock, 0, sizeof( pPosBlock ) );
    }
    else {
        if( /*( operacion >= 0 ) && */( _db[area] == NULL ) ) {
            _ERROR( 100, 0, area );
            return ( 100 );
        }

        /*-------------------------- Setea bloqueado a 0 --------------------------*/
        SetStatus( area, BLOQUEADO, 0 );

        /*--------------- Calcula codigo de operacion segun bloqueo ---------------*/
        GetLocks( area, &lock, &wait, &multiple );
        if( lock
         && ( ( operacion > 4 && operacion < 14 ) || operacion == 23 || operacion == 24
           || operacion == 19 ) ) {
            operacion += ( WORD )( ( wait ) ? ( WORD )100 : ( WORD )200 );
            if( multiple ) {
                operacion += ( WORD )200;
            }
        }

        /*-------------------------- prepara parametros ---------------------------*/
        l_registro = _db[area]->long_buffer;
        p_buffer = ( !buffer ) ? _db[area]->puntero_registro : buffer;
        nro_clave = ( !nro_key ) ? _db[area]->key_number : *nro_key;
        largo = ( !l_buffer ) ? &l_registro : l_buffer;
        if( key )//Juanf
        {
            memcpy( _db[area]->key_buffer, key, MAX_LONG_KEY );
        }

        key_buffer = ( char* )( &_db[area]->key_buffer );
        pPosBlock = &_db[area]->pos_block;
    }

    return ( _btrv( area, operacion, pPosBlock, p_buffer, largo, key_buffer, nro_clave ) );
}


/*****************************************************************************/
int BtrieveManager::AreaDisponible( void )
/*****************************************************************************/
{
    int area = 0, h;

    for( h = 1;h < MAX_AREA_BTRV && !area;h++ ) {
        if( !_db[h] ) {
            area = h;
        }
    }

    return ( area );
}


/*****************************************************************************/
void BtrieveManager::SetFlags( int area, short status, char controlar )
/*****************************************************************************/
{
    if( status != 0 ) {
        SetStatus( area, S_FOUND, 0 );
        if( status == 9 && controlar ) {
            if( controlar == TOP ) {
                if( _db[area]->registros ) {
                    SetStatus( area, S_EOF, 0 );
                }
                SetStatus( area, S_BOF, 1 );
            }
            else if( controlar == BOTTOM ) {
                if( _db[area]->registros ) {
                    SetStatus( area, S_BOF, 0 );
                }
                SetStatus( area, S_EOF, 1 );
            }
            else if( controlar == VACIO ) {
                SetStatus( area, S_BOF, 1 );
                SetStatus( area, S_EOF, 1 );
            }
        }
    }
    else {
        SetStatus( area, S_FOUND, 1 );
        SetStatus( area, S_BOF, 0 );
        SetStatus( area, S_EOF, 0 );
    }
}


/*****************************************************************************/
int BtrieveManager::Open( WORD id, char *nombre, void *pRecord, WORD lRecord, char *clave_acceso,
                          short modo, DWORD formato )
/*****************************************************************************/
{
    struct e_stat stat;
    short ok = 0, nro_key;
    int area;
    unsigned short l_buffer;

    /*   area = AreaDisponible();
     * if( !area ) return( ERR_AREA_INACTIVA );   // No hay areas disponibles
     */
    area = id;
    if( Actived( area ) ) {
        return ( 0 );
    }
    /*--------- Verifica que el �ea est�disponible y pide memoria ---------*/
    _db[area] = ( struct _db * )malloc( sizeof( struct _db ) );
    memset((void*)_db[area],0,sizeof(struct _db));
    if( !_db[area] ) {
        return ( ERR_MALLOC );
    }                    // No hay memoria disponible
    _db[area]->id = id;
    _db[area]->formato = formato;
    strcpy( _db[area]->nombre, nombre );
    _db[area]->buffer_interno = NO;

    /*--------- abre el archivo ( OPEN ) -----------*/
    l_buffer = ( clave_acceso ) ? ( short )strlen( clave_acceso ) : ( short )0;
    ok = btrv( area, 0, ( void* )clave_acceso, &l_buffer, nombre, &modo );

    /*------------ Si no hubo error pide STATUS -----------*/
	if( ok == 0 ) {
        nro_key = 0;
        l_buffer = sizeof( stat ) - 128;
        ok = btrv( area, 15, ( void* )( &stat.long_registro ), &l_buffer, stat.nombre, &nro_key );

        /*-------------- Si no hubo error llena la estructura -------------*/
        if( ok == 22 ) {
            ok = 0;
        }
        if( ok == 0 ) {
            if( lRecord == 0 ) {
                lRecord = stat.long_registro;
            }
            if( pRecord ) {
                _db[area]->status = 0;
            }
            _db[area]->indices = ( char )stat.indices;
            _db[area]->registros = stat.registros;
            _db[area]->key_number = 0;
            _db[area]->lFixedRecord = stat.long_registro;
            _db[area]->long_buffer = ( lRecord && ( formato & FORMATO_L_VARIABLE ) ) ? lRecord
                                   : stat.long_registro;
            _db[area]->puntero_registro = ( char* )( ( pRecord ) ? pRecord : BufferDefault );
            if( !pRecord ) {
                _db[area]->puntero_registro = ( char* )MALLOC_CLEAR( lRecord );
                _db[area]->buffer_interno = SI;
            }
            else {
                _db[area]->buffer_interno = NO;
            }

            //if( !pRecord && _db[area]->long_buffer > (signed short)lBufferDefault ) {
            //   ok = -ERR_BUFFER_ESCASO;
            //}
            if( stat.registros ) {
                Go( area, TOP );
            }
        }
    }

    /*------------ Si hubo error devuelve la memoria y lo trata ----------*/
    if( ok != 0 && _db[area] ) {
        if( _db[area]->buffer_interno ) {
            FREEMEM( ( char* )_db[area]->puntero_registro );
        }
        free( _db[area] );
        _db[area] = NULL;
    }

    return ( ( ok ) ? 0 : 1 );
}


/*****************************************************************************/
WORD BtrieveManager::Close( int area )
/*****************************************************************************/
{
    WORD ok;

    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }
    if( _db[area] ) {
        ok = btrv( area, 1, NULL, NULL, NULL, NULL );
        if( ok == 0 ) {
            if( _db[area]->buffer_interno ) {
                FREEMEM( ( char* )_db[area]->puntero_registro );
            }
            free( _db[area] );
            _db[area] = NULL;
        }
    }

    return ( ok );
}


/*****************************************************************************/
void BtrieveManager::CloseAll( void )
/*****************************************************************************/
{
    int area;

    for( area = 0;area < MAX_AREA_BTRV;area++ ) {
        Close( area );
    }
}


/*****************************************************************************/
void BtrieveManager::SetIndex( int area, WORD number_key )
/*****************************************************************************/
{
    unsigned long PosAnt;

    if( Actived( area ) ) {
        PosAnt = GetPosition( area );
        _db[area]->key_number = ( WORD )( number_key - 1 );
        GetDirect( area, PosAnt );
    }
}


/*****************************************************************************/
WORD BtrieveManager::GetIndex( int area )
/*****************************************************************************/
{
    if( !Actived( area ) ) {
        return ( 0 );
    }
    return ( ( WORD )( _db[area]->key_number + 1 ) );
}


/*****************************************************************************/
BOOL BtrieveManager::Bof( int area )
/*****************************************************************************/
{
    return ( GetStatus( area, S_BOF ) );
}


/*****************************************************************************/
BOOL BtrieveManager::Eof( int area )
/*****************************************************************************/
{
    return ( GetStatus( area, S_EOF ) );
}


/*****************************************************************************/
BOOL BtrieveManager::Found( int area )
/*****************************************************************************/
{
    return ( GetStatus( area, S_FOUND ) );
}


/*****************************************************************************/
BOOL BtrieveManager::Empty( int area )
/*****************************************************************************/
{
    BOOL rta;

    rta = ( RecCount( area ) ) ? 0 : 1;

    return ( rta );
}


/*****************************************************************************/
DWORD BtrieveManager::RecCount( int area )
/*****************************************************************************/
{
    if( !Actived( area ) ) {
        return ( 0 );
    }

    return ( _db[area]->registros );
}


/*****************************************************************************/
void BtrieveManager::GetLocks( int area, BOOL *lock, BOOL *wait, BOOL *multiple )
/*****************************************************************************/
{
    *lock = GetStatus( area, 4 );
    *wait = GetStatus( area, 5 );
    *multiple = GetStatus( area, 6 );
}


/*****************************************************************************/
void BtrieveManager::SetLocks( int area, BOOL lock, BOOL wait, BOOL multiple )
/*****************************************************************************/
{
    SetStatus( area, ( char )4, lock );
    SetStatus( area, ( char )5, wait );
    SetStatus( area, ( char )6, multiple );
}


/*****************************************************************************/
BOOL BtrieveManager::GetStatus( int area, BYTE orden )
/*****************************************************************************/
{
    char bit;
    BOOL rta;

    if( !Actived( area ) ) {
        return ( FALSE );
    }
    orden--;
    bit = ( char )( 1 << orden );
    rta = ( _db[area]->status & bit ) ? 1 : 0;

    return ( rta );
}


/*****************************************************************************/
void BtrieveManager::SetStatus( int area, char orden, BOOL valor )
/*****************************************************************************/
{
    char bit;

    if( !Actived( area ) ) {
        return;
    }
    orden--;
    bit = ( char )( 1 << orden );
    _db[area]->status = ( valor ) ? ( BYTE )( _db[area]->status | bit ) : ( BYTE )( _db[area]->
        status & ( ~bit ) );
}


/*****************************************************************************/
int BtrieveManager::Zap( int area )
/*****************************************************************************/
{
    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }
    Go( area, TOP );
    while( Found( area ) ) {
        Delete( area );
        Go( area, TOP );
    }
    SetStatus( area, S_BOF, 1 );
    SetStatus( area, S_EOF, 1 );

    return ( 0 );
}


/*****************************************************************************/
int BtrieveManager::Create( char *nombre, char *especif, unsigned short largo_esp, short modo )
/*****************************************************************************/
{
    return ( _btrv( 0, 14, NULL, especif, &largo_esp, nombre, modo ) );
}


/*****************************************************************************/
int BtrieveManager::Insert( int area, WORD lRecord, int ModifStatus )
/*****************************************************************************/
{
    short ok;
    unsigned short l_buffer;

    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }

    if( ModifStatus ) {
        _db[area]->puntero_registro[0] = 0;
    }
    l_buffer = ( lRecord ) ? lRecord : _db[area]->lFixedRecord;
    if( lRecord && _db[area]->formato & FORMATO_L_VARIABLE ) {
        l_buffer += _db[area]->lFixedRecord;
    }

    /*   if( _db[area]->formato & FORMATO_NRO_REGISTRO ) {
     * *(long *)(_db[area]->puntero_registro) = 0L;
     * } */
    ok = btrv( area, 2, NULL, &l_buffer, NULL, NULL );
    if( ok == 0 ) {
        _db[area]->registros++;
        SetStatus( area, S_BOF, 0 );
        SetStatus( area, S_EOF, 0 );
    }

    return ( ok );
}


/*****************************************************************************/
int BtrieveManager::Update( int area, WORD lRecord )
/*****************************************************************************/
{
    unsigned short l_buffer;

    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }
    l_buffer = ( lRecord ) ? lRecord : _db[area]->lFixedRecord;
    if( lRecord && _db[area]->formato & FORMATO_L_VARIABLE ) {
        l_buffer += _db[area]->lFixedRecord;
    }

    return ( btrv( area, 3, NULL, &l_buffer, NULL, NULL ) );
}


/*****************************************************************************/
int BtrieveManager::Delete( int area )
/*****************************************************************************/
{
    WORD ok;

    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }
    ok = btrv( area, 4, NULL, NULL, NULL, NULL );
    if( ok == 0 ) {
        _db[area]->registros--;
    }
    if( Seek( area, B_GET_NEXT, NULL ) == 9 ) {
        Seek( area, B_GET_PREVIOUS, NULL );
    }

    return ( ok );
}


/*****************************************************************************/
int BtrieveManager::BeginTransaction( void )
/*****************************************************************************/
{
    return ( btrv( 0, 19, NULL, NULL, NULL, NULL ) );
}


/*****************************************************************************/
int BtrieveManager::BeginConcurrent( void )
/*****************************************************************************/
{
    return ( btrv( 0, 1519, NULL, NULL, NULL, NULL ) );
}


/*****************************************************************************/
int BtrieveManager::EndTransaction( void )
/*****************************************************************************/
{
    return ( btrv( 0, 20, NULL, NULL, NULL, NULL ) );
}


/*****************************************************************************/
int BtrieveManager::AbortTransaction( void )
/*****************************************************************************/
{
    return ( btrv( 0, 21, NULL, NULL, NULL, NULL ) );
}


/*****************************************************************************/
int BtrieveManager::GetDirectory( char *buffer, short drive )
/*****************************************************************************/
{
    return ( _btrv( 0, 18, NULL, NULL, NULL, buffer, drive ) );
}


/*****************************************************************************/
int BtrieveManager::SetDirectory( char *path )
/*****************************************************************************/
{
    return ( _btrv( 0, 17, NULL, NULL, NULL, path, 0 ) );
}


/*****************************************************************************/
int BtrieveManager::SetOwner( int area, char *nombre, short longitud, short modo )
/*****************************************************************************/
{
    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }

    return ( btrv( area, 29, nombre, ( unsigned short* )&longitud, nombre, &modo ) );
}


/*****************************************************************************/
int BtrieveManager::ClearOwner( int area )
/*****************************************************************************/
{
    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }
    return ( btrv( area, 30, NULL, NULL, NULL, NULL ) );
}


/*****************************************************************************/
int BtrieveManager::Stat( int area, char *buffer, WORD longitud, char *extension )
/*****************************************************************************/
{
    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }

    return ( btrv( area, 15, buffer, ( unsigned short* )&longitud, extension, NULL ) );
}


/*****************************************************************************/
int BtrieveManager::Unlock( int area )
/*****************************************************************************/
{
    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }

    return ( btrv( area, 27, NULL, NULL, NULL, NULL ) );
}


/*****************************************************************************/
int BtrieveManager::UnlockSingle( int area, long registro )
/*****************************************************************************/
{
    unsigned short l_buffer = 4;
    short nro_key = -1;

    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }
    if( !registro ) {
        registro = GetPosition( area );
    }

    return ( btrv( area, 27, ( char* )( &registro ), &l_buffer, NULL, &nro_key ) );
}


/*****************************************************************************/
int BtrieveManager::UnlockAll( int area )
/*****************************************************************************/
{
    short key_number = -2;

    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }

    return ( btrv( area, 27, NULL, NULL, NULL, &key_number ) );
}


/*****************************************************************************/
long BtrieveManager::GetPosition( int area )
/*****************************************************************************/
{
    unsigned short largo = 4;
    WORD ok;
    long posicion = 0;

    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }
    if( _db[area]->registros ) {
        ok = btrv( area, 22, ( void* )( &posicion ), &largo, NULL, NULL );
        if( ok != 0 ) {
            posicion = 0;
        }
    }

    return ( posicion );
}


/*****************************************************************************/
int BtrieveManager::GetDirect( int area, unsigned long posicion )
/*****************************************************************************/
{
    WORD ok;
    long buffer;

    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }
    if( !posicion ) {
        return ( ERR_PARAMETROS );
    }
    buffer = *( long* )( _db[area]->puntero_registro );
    *( long* )( _db[area]->puntero_registro ) = posicion;
    ok = btrv( area, 23, NULL, NULL, NULL, NULL );
    SetFlags( area, ok, 0 );
    if( ok != 0 ) {
        *( long* )( _db[area]->puntero_registro ) = buffer;
    }

    return ( ok );
}


/*****************************************************************************/
int BtrieveManager::Seek( int area, short operacion, void *clave )
/*****************************************************************************/
{
    int ok;
    char controlar = 0;

    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }
    switch( operacion ) {
        case B_GET_FIRST:
        case B_GET_LAST:
        case B_STEP_FIRST:
        case B_STEP_LAST:
            controlar = VACIO;
            break;
        case B_GET_NEXT:
        case B_STEP_NEXT:
        case B_GET_NEXT + 50:
        case B_GET_GT:
            controlar = BOTTOM;
            break;
        case B_GET_PREVIOUS:
        case B_STEP_PREVIOUS:
        case B_GET_PREVIOUS + 50:
        case B_GET_LT:
            controlar = TOP;
            break;
    }

    ok = btrv( area, operacion, NULL, NULL, clave, NULL );
    SetFlags( area, ( short )ok, controlar );
    if( ok ) {
        ok = -ok;
    }
    else {
        ok = _db[area]->last_l_buffer;
    }

    if( ok > 0 && ok < _db[area]->long_buffer ) {
        _db[area]->puntero_registro[ok] = 0;
    }

    return ( ok );
}


/*****************************************************************************/
int BtrieveManager::Go( int area, long registro )
/*****************************************************************************/
{
    int ok;
    static long r /*PosAnt */ ;
    WORD IndAnt;

    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }
    switch( registro ) {
        case TOP:
            if( _db[area]->indices ) {
                ok = Seek( area, B_GET_FIRST, NULL );
            }
            else {
                ok = Seek( area, B_STEP_FIRST, NULL );
            }
            break;

        case BOTTOM:
            if( _db[area]->indices ) {
                ok = Seek( area, B_GET_LAST, NULL );
            }
            else {
                ok = Seek( area, B_STEP_LAST, NULL );
            }
            break;
        default:
            if( _db[area]->formato & FORMATO_NRO_REGISTRO ) {
                IndAnt = GetIndex( area );
                SetIndex( area, 1 );
                r = registro;
                ok = Seek( area, B_GET_EQUAL, &r /*egistro */ );
                SetIndex( area, IndAnt );
            }
            else {
                ok = ERR_PARAMETROS;
            }
    }

    return ( ok );
}


/*****************************************************************************/
int BtrieveManager::Skip( int area, int salto )
/*****************************************************************************/
{
    int respuesta = 0, i;
    WORD op;

    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }
    if( salto > 0 ) {
        op = ( WORD )( ( _db[area]->indices ) ? B_GET_NEXT : B_STEP_NEXT );
        for( i = 0;i < salto && respuesta >= 0;i++ ) {
            respuesta = Seek( area, op, NULL );
        }
        if( respuesta < 0 && i ) {
            i--;
        }
    }
    else {
        op = ( WORD )( ( _db[area]->indices ) ? B_GET_PREVIOUS : B_STEP_PREVIOUS );
        for( i = 0;i > salto && respuesta >= 0;i-- ) {
            respuesta = Seek( area, op, NULL );
        }
        if( respuesta < 0 && i ) {
            i++;
        }
    }

    return ( i );
}


/*****************************************************************************/
int BtrieveManager::SizeOfRecord( int area )
/*****************************************************************************/
{
    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }

    return ( _db[area]->long_buffer );
}


/*****************************************************************************/
int BtrieveManager::FixedSizeOfRecord( int area )
/*****************************************************************************/
{
    if( !Actived( area ) ) {
        return ( ERR_AREA_INACTIVA );
    }

    return ( _db[area]->lFixedRecord );
}


/*****************************************************************************/
WORD BtrieveManager::SizeOfRecordReaded( int area )
/*****************************************************************************/
{
    if( !Actived( area ) ) {
        return ( ( WORD )ERR_AREA_INACTIVA );
    }

    return ( _db[area]->last_l_buffer );
}


/*****************************************************************************/
void BtrieveManager::SetPtrRecord( int area, void *buffer )
/*****************************************************************************/
{
    if( !Actived( area ) ) {
        return;
    }
    if( _db[area] ) {
        _db[area]->puntero_registro = ( char* )buffer;
    }
}


/*****************************************************************************/
void * BtrieveManager::GetPtrRecord( int area )
/*****************************************************************************/
{
    if( !Actived( area ) ) {
        return ( NULL );
    }

    return ( _db[area]->puntero_registro );
}


/*****************************************************************************/
void BtrieveManager::SetRecordDefault( void *buffer, DWORD lBuffer )
/*****************************************************************************/
{
    BufferDefault = buffer;
    lBufferDefault = lBuffer;
}


/***************************************************************************/
void BtrieveManager::_ERROR( int st, int id, int area )
/***************************************************************************/
{
    char aux[80];
    char cadena[60];

    switch( st ) {
        case 100:
            strcpy( cadena, "AREA INACTIVA" );
			_snprintf(aux, sizeof(aux)-1,"Error %s [%i] area: %d  \n", cadena, st, area );
            break;
        default:
            strcpy( cadena, "GENERAL" );
			_snprintf( aux, sizeof(aux)-1, "Error %s [%i] en archivo %s \n", cadena, st, _db[area]->nombre );
            break;
    }
    printf( aux );
    //MessageBox( NULL, aux, "Bt", MB_ICONERROR | MB_OK );
}


/***************************************************************************/
BOOL BtrieveManager::Actived( int area )
/***************************************************************************/
{
    if( area < 0 || area > MAX_AREA_BTRV || !_db[area] ) {
        return ( FALSE );
    }

    return ( TRUE );
}


BtrieveManager::BtrieveManager()
{
    int i;

    for( i = 0;i < MAX_AREA_BTRV;i++ ) {
        _db[i] = NULL;
    }
}


BtrieveManager::~BtrieveManager()
{
}


/*****************************************************************************/
WORD BtrieveManager::_btrv( int area, WORD operacion, void *pos_block, void *buffer,
                            unsigned short *l_buffer, void *key, short nro_key )
/*****************************************************************************/
{
    char intentos = 3, seguir = 1;
    WORD st;

    /*---------------- Ejecuta operacion (intenta 3 veces) ----------------*/
    do {
        intentos--;
        #ifdef INVEL_L
        st = 0;
        #else
        st = BTRV( operacion, pos_block, buffer, l_buffer, key, nro_key );
        #endif

        switch( st ) {
            case 0:
            case 4:
                /* clave no encontrada */
            case 5:
                /* clave duplicada     */
            case 9:
                /* eof o bof           */
            case 22:
                /* data buffer corto   */
                seguir = 0;
                break;
            case 84:
            case 85:
                SetStatus( area, BLOQUEADO, 1 );
                break;
            default:
                if( operacion != 18 ) {
                    _ERROR( st, _db[area]->id, area );
                }
                else {
                    _ERROR( st, 0, area );
                }
                seguir = 0;
                break;
        }
    }
    while( intentos > 0 && seguir );
    // operaciones como getdirectory
    //no entran por aca
    if( operacion != 18 && operacion != 28 && operacion != 19 && operacion != 1519
     && operacion != 20 ) {
        _db[area]->last_l_buffer = ( WORD )*l_buffer;
    }

    return ( st );
}


int BtrieveManager::GetEqual( int area, char *clave )
{
    int ok;

    ok = btrv( area, 5, NULL, NULL, clave, NULL );
    SetFlags( area, ok, 0 );

    return ( ok );
}


int BtrieveManager::GetGreater( int area, char *clave )
{
    int ok;

    ok = btrv( area, B_GET_GT, NULL, NULL, clave, NULL );
    SetFlags( area, ok, 0 );

    return ( ok );
}


int BtrieveManager::GetGreaterEqual( int area, char *clave )
{
    int ok;

    ok = btrv( area, B_GET_GE, NULL, NULL, clave, NULL );
    SetFlags( area, ok, 0 );

    return ( ok );
}


int BtrieveManager::Reset()
{
    int ok;

    ok = btrv( 0, B_RESET, NULL, NULL, NULL, NULL );
    return ( ok );
}



int BtrieveManager::Testear(int area, int orden)
{
    int rta = 0;
    char rotacion;
    rotacion = 1 << (orden-1);
    if( _db[area] ) rta = _db[area]->puntero_registro[0] & rotacion;
    return ( rta );
}


//---------------------
// conversion de btrieve pos DOS a BtrieveManager windows
#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
int SELECT( int num_area )
/*****************************************************************************/
{
if( area__ >= 0 && area__ < MAX_AREA )
{
area__ = num_area;
}
else
{
ERROR__( 0, "Area invalida" );
}
return 0;
}

/*****************************************************************************/
int SELECTED(  )
/*****************************************************************************/
{
return ( area__ );
}

#define CANTFILES 100
void InitTable(  )
{
int c = 100;

FileTable = ( _Table * ) malloc( CANTFILES * sizeof( struct _Table ) );
memset( FileTable, 0, CANTFILES * sizeof( struct _Table ) );

// no usar la posicion 0
// la busqueda retorna la posicion o 0 si o la encuentra

#ifdef INVEL_L
sprintf(FileTable[1].name,"dd_var.sic");
sprintf(FileTable[1].newName,"dd_var.ctr");

sprintf(FileTable[2].name,"dd_est2.sic");
sprintf(FileTable[2].newName,"dd_est2.ctr");

sprintf(FileTable[3].name,"carpetas.sic");
sprintf(FileTable[3].newName,"carpetas.ctr");

sprintf(FileTable[4].name,"archivos.sic");
sprintf(FileTable[4].newName,"archivos.ctr");

sprintf(FileTable[5].name,"articulo.sic");
sprintf(FileTable[5].newName,"articulo.ctr");
#endif

}

void FreeTable(  )
{
free( FileTable );
}

int FindInTable( char *file )
{
int found = 0;
int j;
char     *t, *t1;

t = strchr( file, ':' );
if( t )
t++;
else
t = file;

do
{
t1 = t;
t = strchr( t, '\\' );
if( t )
t++;

} while( t );
t = t1;
t = strchr( file, ' ' );
if( t )
t[0] = 0;

for( j = 1; j < CANTFILES; j++ ){
// chequeamos desde la posicion 1 de la tabla
#ifdef INVEL_L
if( strcasecmp( FileTable[j].name, t1 ) == 0 )
{
#else
if( _stricmp( FileTable[j].name, t1 ) == 0 )
{
#endif
found = j;
break;
}
}
#ifdef INVEL_L
return 1;
#else
return found;
#endif
}

/*****************************************************************************/
//USE_DATABASE( int area, char *nombre, char *estructura, char *clave_acceso, int largo, int modo, int tamanno)
int USE_DATABASE( int area, char *nombre, char *estructura, int tam_estructura,
char *clave_acceso, int largo, int modo )
/*****************************************************************************/
//char *clave_acceso, *nombre, *estructura;
//int  modo, largo, area, tamanno;
{
int ok;
if(USED(area)){
    CLOSE_DATABASE(area);
}
if( USED(area) )
	CLOSE_DATABASE( area );
area__ = area;

//Open( WORD id, char *nombre, void *pRecord, WORD lRecord, char *clave_acceso, short modo, DWORD formato )
// invertimos...
// para la caja 0 es succes
if( FindInTable( nombre ) )
{
char     *p, nameTmp[40];
//int i;

strcpy( nameTmp, nombre );

//p = strchr( nameTmp, '.' );
p = strrchr( nameTmp, '.' );
if( p && strncmp( &p[1], "tmp", 3) &&  strncmp( &p[1], "his", 3) )
strcpy( &p[1], "ctr" );

ok =
Ctree_USE_DATABASE( area, nameTmp, estructura, tam_estructura, clave_acceso,
largo, modo );
if( !ok )
UsedByCtree[area__] = 1;
}
else
{
ok = !Btrieve.Open( area, nombre, estructura, largo, clave_acceso, modo, 0 );
UsedByCtree[area__] = 0;
}
return ok;
}

/*****************************************************************************/
int CLOSE_ALL(  )
/*****************************************************************************/
{

area__ = 0;
Btrieve.CloseAll(  );
Ctree_CLOSE_ALL(  );
return 0;
}

/*****************************************************************************/
int SET_ORDER( int number_key )
/*****************************************************************************/
{
if( UsedByCtree[area__] )
Ctree_SET_ORDER( number_key );
else
Btrieve.SetIndex( area__, number_key );
return 1;
}

/*****************************************************************************/
int GET_ORDER( void )
/*****************************************************************************/
{
if( UsedByCtree[area__] )
return Ctree_GET_ORDER(  );
else
return Btrieve.GetIndex( area__ );
}

/*****************************************************************************/
int BOF(  )
/*****************************************************************************/
{
    
    if( UsedByCtree[area__] ) {
        return Ctree_BOF(  );
    } else {
        return ( Btrieve.Bof( area__ ) );
    }
}

/*****************************************************************************/
int BtrvEOF(  )
/*****************************************************************************/
{
    if( RECCOUNT() ) {
        if( UsedByCtree[area__] ) {
            return Ctree_EOF(  );
        } else {
            return ( Btrieve.Eof( area__ ) );
        }
    } else {
        return( 1 );
    }
}

/*****************************************************************************/
int FOUND(  )
/*****************************************************************************/
{
    if( UsedByCtree[area__] ) {
        return Ctree_FOUND(  );
    } else {
        return ( Btrieve.Found( area__ ) );
    }
}

/*****************************************************************************/
int EMPTY(  )
/*****************************************************************************/
{
int rta;

if( UsedByCtree[area__] )
rta = Ctree_EMPTY(  );
else
rta = Btrieve.Empty( area__ );
return ( rta );
}

/*****************************************************************************/
unsigned long RECCOUNT( void )
/*****************************************************************************/
{
if( UsedByCtree[area__] )
return Ctree_RECCOUNT(  );
else
return Btrieve.RecCount( area__ );
}

/*****************************************************************************/
int BtrvDELETE(  )
/*****************************************************************************/
{
// cambiamos la modalidad
// los borramos directamente y no necesitamos hacer pack

// ver facturacion remitos como queda
if( UsedByCtree[area__] )
Ctree_DELETE(  );
else
Btrieve.Delete( area__ );

return 0;

}

/*****************************************************************************/
int RECALL(  )
/*****************************************************************************/
{
/// no existe mas
return 0;
}

/*****************************************************************************/
int _DELETE(  )
/*****************************************************************************/
{
if( UsedByCtree[area__] )
Ctree__DELETE(  );
else
Btrieve.Delete( area__ );
return 0;
}

/*****************************************************************************/
int END_TRANSACTION(  )
/*****************************************************************************/
{
int ok;

if( UsedByCtree[area__] )
ok = Ctree_END_TRANSACTION(  );
else
ok = Btrieve.EndTransaction(  );
return ( ok );
}

/*****************************************************************************/
int GET_DIRECTORY( char *buffer, int drive )
/*****************************************************************************/
{
int ok;

ok = Btrieve.GetDirectory( buffer, drive );
return ( ok );
}

/*****************************************************************************/
int INSERT(  )
/*****************************************************************************/
{
if( UsedByCtree[area__] ){
return Ctree_INSERT(  );  
}
else
return Btrieve.Insert( area__, Btrieve.SizeOfRecord( area__ ), SI );
}

/*****************************************************************************/
int INSERT_VARIABLE( int long_reg )
/*****************************************************************************/
{
int ok = 100;

if( UsedByCtree[area__] )
ok = Ctree_INSERT_VARIABLE( long_reg );
else
ok = Btrieve.Insert( area__, long_reg, SI );
return ( ok );
}

/*****************************************************************************/
int ABORT_TRANSACTION(  )
/*****************************************************************************/
{
int ok;

if( UsedByCtree[area__] )
ok = Ctree_ABORT_TRANSACTION(  );
else
ok = Btrieve.AbortTransaction(  );
//ok = btrv( 21, NULL, NULL, NULL, NULL );
return ( ok );
}

/*****************************************************************************/
int BEGIN_TRANSACTION(  )
/*****************************************************************************/
{
int ok;

if( UsedByCtree[area__] )
ok = Ctree_BEGIN_TRANSACTION(  );
else
ok = Btrieve.BeginTransaction(  );

return ( ok );
}

/*****************************************************************************/
int UNLOCK_(  )
/*****************************************************************************/
{
int ok = -1;

if( UsedByCtree[area__] )
ok = Ctree_UNLOCK(  );
else
ok = Btrieve.Unlock( area__ );
return ( ok );
}

/*****************************************************************************/
int UNLOCK_SINGLE( long registro )
/*****************************************************************************/
{
int ok = -1, l_buffer = 4, nro_key = -1;

if( UsedByCtree[area__] )
ok = Ctree_UNLOCK_SINGLE( registro );
else
ok = Btrieve.UnlockSingle( area__, registro );
return ( ok );
}

/*****************************************************************************/
int UNLOCK_ALL(  )
/*****************************************************************************/
{
int ok = -1, key_number = -2;

if( UsedByCtree[area__] )
ok = Ctree_UNLOCK_ALL(  );
else
ok = Btrieve.UnlockAll( area__ );
return ( ok );
}

/*****************************************************************************/
int UPDATE(  )
/*****************************************************************************/
{
int ok;

if( UsedByCtree[area__] )
ok = Ctree_UPDATE(  );
else
ok = Btrieve.Update( area__, Btrieve.SizeOfRecord( area__ ) );
return ( ok );
}

/*---------------------------------------------------------------------------*/
/*                   FUNCIONES DE MOVIMIENTO SIN INDICES                     */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
long GET_POSITION(  )
/*****************************************************************************/
{
long posicion = 0;

if( UsedByCtree[area__] )
posicion = Ctree_GET_POSITION(  );
else
posicion = Btrieve.GetPosition( area__ );
return ( posicion );
}

/*****************************************************************************/
int GET_DIRECT( unsigned long posicion )
/*****************************************************************************/
{
int ok = 100;

if( UsedByCtree[area__] )
ok = Ctree_GET_DIRECT( posicion );
else
ok = Btrieve.GetDirect( area__, posicion );
return ( ok );
}

/*****************************************************************************/
int GET_EQUAL( char *clave )
/*****************************************************************************/
{
int ok;

if( UsedByCtree[area__] )

ok = Ctree_GET_EQUAL( clave );
else
ok = Btrieve.GetEqual( area__, clave );
return ( ok );
}

/*****************************************************************************/
int GET_GREATER( char *clave )
/*****************************************************************************/
{
int ok;

if( UsedByCtree[area__] )
ok = Ctree_GET_GREATER( clave );
else
ok = Btrieve.GetGreater( area__, clave );
//ok = btrv( 8, NULL, NULL, clave, NULL );
//BTRV_sets_busqueda(ok);
return ( ok );
}

/*****************************************************************************/
int GET_GREATER_E( char *clave )
/*****************************************************************************/
{
int ok;

if( UsedByCtree[area__] )
ok = Ctree_GET_GREATER_E( clave );
else
ok = Btrieve.GetGreaterEqual( area__, clave );
//ok = btrv( 9, NULL, NULL, clave, NULL );
//BTRV_sets_busqueda(ok);
return ( ok );
}

/*****************************************************************************/
int SKIP( int salto )
/*****************************************************************************/
{
int respuesta = 0, i = 0;

if( UsedByCtree[area__] )
i = Ctree_SKIP( salto );
else
i = Btrieve.Skip( area__, salto );

return ( i );
}

/*****************************************************************************/
int GO( int registro )
/*****************************************************************************/
{
if( UsedByCtree[area__] )
Ctree_GO( registro );
else
Btrieve.Go( area__, registro );
return 0;
}

/*****************************************************************************/
int BTRIEVE_PRESENTE(  )
/*****************************************************************************/
{
char buffer[255];
int rta = 1;

if( GET_DIRECTORY( buffer, 0 ) == 20 )
rta = 0;
return ( rta );
}

/*****************************************************************************/
int USED( int area )
/*****************************************************************************/
{
int rta = 0;
if( UsedByCtree[area] )
return 1;
else
rta = Btrieve.Actived( area );
return ( rta );
}

/*****************************************************************************/
int SIZEOF_REC_DB( int area )
/*****************************************************************************/
{
if( UsedByCtree[area] )
return Ctree_SIZEOF_REC_DB( area );
else

return ( Btrieve.SizeOfRecord( area ) );
//( _db[area] )? _db[area]->long_buffer : 0 );
}

/*****************************************************************************/
char     *BUFFER_REC_DB( int area )
/*****************************************************************************/
{
if( UsedByCtree[area] )
return Ctree_BUFFER_REC_DB( area );
else
return ( char * ) Btrieve.GetPtrRecord( area );
//return( ( _db[area] )? _db[area]->puntero_registro : 0 );
}

/*****************************************************************************/
int CARGAR_DB( int cod, char *buffer, int *l_reg )
/*****************************************************************************/
{
char nombre[80];

NOM_ARCHIVO( cod, nombre );

return ( CARGAR_DATABASE( nombre, buffer, l_reg ) );
}

/*****************************************************************************/
int CARGAR_DATABASE( char *nombre, char *buffer, int *l_reg )
/*****************************************************************************/
{
int c_reg, h, ok = 0;
char     *p_reg;

if( !USE_DATABASE( 0, nombre, NULL, 0, NULL, 0, 0 ) )
{
*l_reg = SIZEOF_REC_DB( 0 );
c_reg = ( int ) RECCOUNT(  );
p_reg = BUFFER_REC_DB( 0 );
buffer = ( char * ) MALLOC( c_reg * ( *l_reg ) );
if( buffer )
{
ok = 1;
for( h = 0; h < c_reg; h++ )
{
memcpy( &buffer[h * ( *l_reg )], p_reg, *l_reg );
SKIP( 1 );
}
}
}
CLOSE_DATABASE( 0 );
return ( ok );
}

/*****************************************************************************/
int AREA_DISPONIBLE( void )
/*****************************************************************************/
{
int area = 0;

area = Btrieve.AreaDisponible(  );

return ( area );
}

/*****************************************************************************/
int BTRIEVE_RESET(  )
/*****************************************************************************/
{
int ok = 0;

ok = Btrieve.Reset(  );
return ( ok );
}

/*****************************************************************************/
int STAT( char *buffer, int longitud, char *extension )
/*****************************************************************************/
{
int ok = -1;

if( UsedByCtree[area__] );
else
ok = Btrieve.Stat( area__, buffer, longitud, extension );
return ( ok );
}

/*****************************************************************************/
int CLOSE_DB( int cod )
/*****************************************************************************/
{
CLOSE_DATABASE( AREA_OF( cod ) );
return 0;
}

/*****************************************************************************/
int USE_DB( int area, int cod, char *estructura, int tam_estructura, char *clave,
int largo, int modo )
/*****************************************************************************/
{
char nombre[80];

NOM_ARCHIVO( cod, nombre );
if( !area )
    area = AREA_DISPONIBLE(  );
SET_AREA( cod, area );

return ( USE_DATABASE( area, nombre, estructura, tam_estructura, clave, largo,
modo ) );
}

/*****************************************************************************/
int CLOSE_DATABASE( int area )
/*****************************************************************************/
{
int ok, area_ant = 0;

if( UsedByCtree[area] )
ok = Ctree_CLOSE_DATABASE( area );
else
{
area_ant = SELECTED(  );
ok = Btrieve.Close( area );
SELECT( area_ant );
}
return 0;
}

/*****************************************************************************/
int GET_LAST(  )
/*****************************************************************************/
{
int ok;

//   ok = btrv( 13, NULL, NULL, NULL, NULL );
//   BTRV_sets_busqueda(ok);
//   if( ok == 9 ) {
//      SET_STATUS(S_BOF,1);
//      SET_STATUS(S_EOF,1);
//   }
if( UsedByCtree[area__] )
ok = Ctree_GET_LAST(  );
else
ok = Btrieve.Seek( area__, B_GET_LAST, NULL );
return ( ok );
}

/*****************************************************************************/
int BEGIN_TRANSACTION_CONCURRENTE(  )
/*****************************************************************************/
{
int ok;

if( UsedByCtree[area__] )
ok = Ctree_BEGIN_TRANSACTION_CONCURRENTE(  );
else
ok = Btrieve.BeginConcurrent(  );

//ok = btrv( 1519, NULL, NULL, NULL, NULL );
return ( ok );
}

/*****************************************************************************/
int GET_STATUS( char orden )
/*****************************************************************************/
{
int rta = 0;

if( UsedByCtree[area__] )
rta = Ctree_GET_STATUS( orden );
else;
/*
* char bit,i;
* bit = 1 << --orden;
* if( _db[area] )rta = _db[area]->status & bit;
*/
return ( rta );
}

/*****************************************************************************/
void SET_STATUS( char orden, char valor )
/*****************************************************************************/
{
if( UsedByCtree[area__] )
Ctree_SET_STATUS( orden, valor );
else;

/*
* char bit;
* bit = 1 << --orden;
* if( _db[area] )_db[area]->status = (valor) ? _db[area]->status | bit : _db[area]->status & (~bit);
*/
}

/*****************************************************************************/
void GET_LOCKS( char *lock, char *wait, char *multiple )
/*****************************************************************************/
{
if( UsedByCtree[area__] )
Ctree_GET_LOCKS( lock, wait, multiple );
else
{
*lock = GET_STATUS( 4 );
*wait = GET_STATUS( 5 );
*multiple = GET_STATUS( 6 );
}
}

/*****************************************************************************/
void SET_LOCKS( char lock, char wait, char multiple )
/*****************************************************************************/
{
if( UsedByCtree[area__] )
Ctree_SET_LOCKS( lock, wait, multiple );
else
{

SET_STATUS( 4, lock );
SET_STATUS( 5, wait );
SET_STATUS( 6, multiple );
}
}

/*****************************************************************************/
int TESTEAR( char orden )
/*****************************************************************************/
{
int rta = 0;


if( UsedByCtree[area__] )
rta = Ctree_TESTEAR( orden );
else{
rta = Btrieve.Testear(area__,orden);
}

return ( rta );
}

/*****************************************************************************/
void SETEAR( char orden, char bit )
/*****************************************************************************/
{
if( UsedByCtree[area__] )
Ctree_SETEAR( orden, bit );
else;
/*
* char rotacion;
* rotacion = 1 << (orden-1);
* if( !bit ){
* rotacion = 255 - rotacion;
* if( _db[area] )_db[area]->puntero_registro[0] &= rotacion;
* }
* else {
* if( _db[area] )_db[area]->puntero_registro[0] |= rotacion;
* }
* UPDATE();
*/
}

/*****************************************************************************/
int DELETED(  )
/*****************************************************************************/
{
int rta = 0;

if( UsedByCtree[area__] )
rta = Ctree_DELETED(  );
else;
//if( _db[area__] ) rta = _db[area__]->puntero_registro[0] & 0x80;
return ( rta );
}

/*****************************************************************************/
int INSERT_NO_MODIF_STATUS(  )
/*****************************************************************************/
{
int ok;

if( UsedByCtree[area__] )
ok = Ctree_INSERT_NO_MODIF_STATUS(  );
else
ok = Btrieve.Insert( area__, Btrieve.SizeOfRecord( area__ ), NO );
return ok;
}

/*****************************************************************************/
int GET_NEXT(  )
/*****************************************************************************/
{
int ok;

//ok = btrv(6, NULL, NULL, NULL, NULL);
//BTRV_sets_mov_aba(ok);
if( UsedByCtree[area__] )
ok = Ctree_GET_NEXT(  );
else
ok = Btrieve.Seek( area__, B_GET_NEXT, NULL );
return ( ok );
}

/*****************************************************************************/
int GET_FIRST(  )
/*****************************************************************************/
{
int ok;

//ok = btrv(6, NULL, NULL, NULL, NULL);
//BTRV_sets_mov_aba(ok);
if( UsedByCtree[area__] )
ok = Ctree_GET_FIRST(  );
else
ok = Btrieve.Seek( area__, B_GET_FIRST, NULL );
return ( ok );
}

/*****************************************************************************/
int GET_PREVIOUS(  )
/*****************************************************************************/
{
int ok;

//ok = btrv(6, NULL, NULL, NULL, NULL);
//BTRV_sets_mov_aba(ok);
if( UsedByCtree[area__] )
ok = Ctree_GET_PREVIOUS(  );
else
ok = Btrieve.Seek( area__, B_GET_PREVIOUS, NULL );
return ( ok );
}

/*****************************************************************************/
int ZAP(  )
/*****************************************************************************/
{
GO( TOP );
while( FOUND(  ) )
{
_DELETE(  );
GO( TOP );
}
//SET_STATUS(S_BOF, 1);
//SET_STATUS(S_EOF, 1);
return 0;
}

///////////
///////////
//////////
/*****************************************************************************/
int STEP_FIRST(  )
/*****************************************************************************/
{
int ok;

ok = Btrieve.Seek( area__, B_STEP_FIRST, NULL );
//ok = btrv( 33, NULL, NULL, NULL, NULL );
return ( ok );
}

/*****************************************************************************/
int STEP_LAST(  )
/*****************************************************************************/
{
int ok;

ok = Btrieve.Seek( area__, B_STEP_LAST, NULL );
//ok = btrv( 34, NULL, NULL, NULL, NULL );
return ( ok );
}

/*****************************************************************************/
int STEP_NEXT(  )
/*****************************************************************************/
{
int ok;

ok = Btrieve.Seek( area__, B_STEP_NEXT, NULL );
//ok = btrv( 24, NULL, NULL, NULL, NULL );
return ( ok );
}

/*****************************************************************************/
int STEP_PREVIOUS(  )
/*****************************************************************************/
{
int ok;

ok = Btrieve.Seek( area__, B_STEP_PREVIOUS, NULL );
//ok = btrv( 35, NULL, NULL, NULL, NULL );
return ( ok );
}

#ifdef __cplusplus
}

void setUsedByCtree( int index, int value )
{
UsedByCtree[index] = value;

}
#endif
#endif
