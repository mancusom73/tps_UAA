#pragma pack (push,1) 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Btrieve.h: interface for the Btrieve class.
 * ------------------------------------------------------------------------------
 */

#if !defined(AFX_BTRIEVE_H__49965941_D212_11D2_88AB_006008AE92D1__INCLUDED_)
#define AFX_BTRIEVE_H__49965941_D212_11D2_88AB_006008AE92D1__INCLUDED_
#ifdef INVEL_W
#include "stdafx.h"
#endif
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef INVEL_L
#include "btrapi.h"
#include "btrdef.h"
#else
#define MAX_AREA_BTRV 100
#define WORD unsigned short
#define DWORD unsigned long
#define BOOL int
#define BYTE unsigned char
#define TRUE 1
#define FALSE 0
#include "btrdef.h"
#endif

//--------------------------- Definiciones --------------------------------
#define S_FOUND       1
#define S_BOF         2
#define S_EOF         3
#define BLOQUEADO     7



struct _db
{
    char pos_block[128];
    char *puntero_registro;
    short long_buffer;
    char key_buffer[MAX_LONG_KEY];
    short key_number;
    BINARY status;
    char indices;
    unsigned long registros;
    char nombre[30];
    char buffer_interno;
    char long_indice[10];
    short id;
    short last_l_buffer;
    long formato;
    short lFixedRecord;
};


struct _key_specification
{
    short posicion;
    short longitud;
    short flags;
    long numbers_of_key;
    char key_type;
    char null_value;
    char reservado[4];
};

struct e_stat
{
    char nombre[128];
    unsigned short long_registro;
    unsigned short pag_tamano;
    unsigned short indices;
    unsigned short registros;
    char flag[2];
    char resto1[4];
    struct _key_specification key_specification[20];
    char resto2[105];
};

class     BtrieveManager
{
  private:
    struct _db *_db[MAX_AREA_BTRV];
    void *BufferDefault;
    DWORD lBufferDefault;

    WORD _btrv( int area, WORD operacion, void *pos_block, void *buffer, unsigned short *l_buffer,
                void *key, short nro_key );
    WORD btrv( int area, WORD operacion, void *buffer, unsigned short *l_buffer, void *key,
               short *nro_key );
    void SetFlags( int area, short status, char controlar );
    void _ERROR( int st, int id, int area );
    BOOL GetStatus( int area, BYTE orden );
    void SetStatus( int area, char orden, BOOL valor );

  public:
    int GetEqual( int area, char *clave );
    BtrieveManager();
    virtual ~BtrieveManager();
    int Open( WORD id, char *nombre, void *pRecord, WORD lRecord, char *clave_acceso, short modo,
              DWORD formato );
    WORD Close( int area );
    void CloseAll( void );
    void SetIndex( int area, WORD number_key );
    WORD GetIndex( int area );
    BOOL Bof( int area );
    BOOL Eof( int area );
    BOOL Found( int area );
    BOOL Empty( int area );
    DWORD RecCount( int area );
    void GetLocks( int area, BOOL *lock, BOOL *wait, BOOL *multiple );
    void SetLocks( int area, BOOL lock, BOOL wait, BOOL multiple );
    int Zap( int area );
    int Create( char *nombre, char *especif, unsigned short largo_esp, short modo );
    int Insert( int area, WORD lRecord, int ModifStatus );
    int Update( int area, WORD lRecord );
    int Delete( int area );
    int BeginTransaction( void );
    int EndTransaction( void );
    int AbortTransaction( void );
    int GetDirectory( char *buffer, short drive );
    int SetDirectory( char *path );
    int SetOwner( int area, char *nombre, short longitud, short modo );
    int ClearOwner( int area );
    int Stat( int area, char *buffer, WORD longitud, char *extension );
    int Unlock( int area );
    int UnlockSingle( int area, long registro );
    int UnlockAll( int area );
    long GetPosition( int area );
    int GetDirect( int area, unsigned long posicion );
    int Seek( int area, short operacion, void *clave );
    int Go( int area, long registro );
    int Skip( int area, int salto );
    int SizeOfRecord( int area );
    int FixedSizeOfRecord( int area );
    WORD SizeOfRecordReaded( int area );
    void SetPtrRecord( int area, void *buffer );
    void *GetPtrRecord( int area );
    void SetRecordDefault( void *buffer, DWORD lBuffer );
    BOOL Actived( int area );
    int BtrieveManager::GetGreaterEqual( int area, char *clave );

    int BtrieveManager::GetGreater( int area, char *clave );
    int BtrieveManager::Reset();
    int AreaDisponible( void );
    int BtrieveManager::BeginConcurrent( void );
    int BtrieveManager::Testear(int area, int orden);
};
extern BtrieveManager Btrieve;
#endif // !defined(AFX_BTRIEVE_H__49965941_D212_11D2_88AB_006008AE92D1__INCLUDED_)

#pragma pack (pop) 
