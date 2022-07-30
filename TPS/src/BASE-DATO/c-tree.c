/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <archivos.h>
#include"config.h"
#if defined(INVEL_W) || defined(INVEL_L)
#include <ctype.h>
#ifndef INVEL_L
#include <windows.h>                              // SDK
#endif
#if defined(USE_PRAGMA)
#pragma pack(1)
#endif

#include "ctreep.h"

#include "c-tree.h"

//#include "wtixmg.h"      // specific to this program


#ifndef INVEL_L
ctCONV BOOL ctDECL ViewCtreeError( HWND hWnd );
#endif

#ifdef ctPortBC3                                  // Borlandc C++ 3.x
typedef FARPROC DLGPROC;

// typedef LPSTR LPCSTR;   // May be needed for v3.0 or earlier
typedef HANDLE HINSTANCE;
#endif
extern pCTGVAR ctWNGV;                            // far pointer to c-tree global variables;

#if (!defined(ctNOGLOBALS) || !defined(VARLDATA) || !defined(VARLKEYS))
#error aa;
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//#undef EOF
//#include <driver.h>
#include <btrv.h>
#include <cr.h>

#if !defined(INVEL_L)
pCTGVAR ctWNGV;                                 // far pointer to c-tree global variables
#endif

extern void *MALLOC();

//------------------- Estructuras y variables globales --------------------
//static char area;
extern int area__;
void Ctree_sets_busqueda( int status );
int Ctree_sets_mov_arr( int status );
int Ctree_sets_mov_aba( int status );
int Ctree_CARGAR_DATABASE( char *nombre, char *buffer, int *l_reg );
int Ctree__INSERT( int long_reg );
unsigned long Ctree_RECCOUNT( void );

#define MAX_AREA     100
#define MAX_LONG_KEY 255
#define TOP          -1
#define BOTTOM       -2
#define S_FOUND       1
#define S_BOF         2
#define S_EOF         3
#define BLOQUEADO     7

#define CADENA_EN_BROWSE     31

//------------------- Estructuras y variables globales --------------------
#define FILE_TYPE_VARIABLE_LENGTH 2

struct _db_ctree
{
    //char pos_block[128];
    char *puntero_registro;
    int long_buffer;
    char key_buffer[MAX_LONG_KEY];
    int key_number;
    BINARY status;
    short indices;
    unsigned long registros;
    char nombre[30];
    char buffer_interno;
    int file_type;
    //char long_indice[10];
    pIFIL work_pIFIL;
    COUNT file_no;
};
static struct _db_ctree *db_ctree[MAX_AREA];
int Ctree_ReReadVRecord( int datno, char *recptr, int len );
extern int UsedByCtree[MAX_AREA];
void setUsedByCtree( int index, int value );

/*****************************************************************************/
void Ctree_GET_LOCKS( char *lock, char *wait, char *multiple )
/*****************************************************************************/
{
    *lock = Ctree_GET_STATUS( 4 );
    *wait = Ctree_GET_STATUS( 5 );
    *multiple = Ctree_GET_STATUS( 6 );
}


/*****************************************************************************/
void Ctree_SET_LOCKS( char lock, char wait, char multiple )
/*****************************************************************************/
{
    Ctree_SET_STATUS( 4, lock );
    Ctree_SET_STATUS( 5, wait );
    Ctree_SET_STATUS( 6, multiple );
}


/*****************************************************************************/
int Ctree_GET_STATUS( char orden )
/*****************************************************************************/
{
    char bit;
    int rta = 0;

    bit = 1 << --orden;
    if( db_ctree[area__] ) {
        rta = db_ctree[area__]->status & bit;
    }
    return ( rta );
}


/*****************************************************************************/
int Ctree_SET_STATUS( char orden, char valor )
/*****************************************************************************/
{
    char bit;

    bit = 1 << --orden;
    if( db_ctree[area__] ) {
        db_ctree[area__]->status = ( valor ) ? db_ctree[area__]->status | bit
                                 : db_ctree[area__]->status & ( ~bit );
    }
    return 0;
}


/*****************************************************************************/
int Ctree_USE_DB( int area, int cod, char *estructura, int tam_estructura, char *clave, int largo,
                  int modo )
/*****************************************************************************/
{
    char nombre[80];

    NOM_ARCHIVO( cod, nombre );
    if( !area ) {
        area = AREA_DISPONIBLE();
    }
    SET_AREA( cod, area );

    return ( Ctree_USE_DATABASE( area, nombre, estructura, tam_estructura, clave, largo, modo ) );
}


/*****************************************************************************/
int Ctree_USE_DATABASE( int area, char *nombre, char *estructura, int tam_estructura,
                        char *clave_acceso, int largo, int modo )
/*****************************************************************************/
{
    //  struct e_stat stat;
    int error = 1;

    // , l_buffer;
    //,  nro_key;

    VRLEN buf_size;
    VRLEN res_len;
    NINT tran_files = 0;
    COUNT file_no;
    #if defined(INVEL_L)
    //  Cambiar mayusculas por minisculas
    char *nombre_tmp;
    int str_len;
    int i;
    int delimiter_flag = 0;
    str_len = strlen( nombre );
    nombre_tmp = ( char* )malloc( str_len + 1 );
    for( i = 0;i < str_len;i++ ) {
        if( !delimiter_flag ) {
            nombre_tmp[str_len - i - 1] = tolower( nombre[str_len - i - 1] );
        }
        else {
            nombre_tmp[str_len - i - 1] = nombre[str_len - i - 1];
        }
        if( nombre[str_len - i - 1] == C_FILEDELIMITER ) {
            delimiter_flag = 1;
        }
    }
    nombre_tmp[i] = '\0';
    #endif

    Ctree_SELECT( area );

    //--------- Verifica que el rea est disponible y pide memoria ---------
    if( db_ctree[area] != NULL ) {
        #if defined(INVEL_L)
        free( nombre_tmp );
        return ( -1 );
        #endif
    }
    db_ctree[area] = ( struct _db_ctree * )MALLOC( sizeof( struct _db_ctree ) );
    if( db_ctree[area] ) {
        memset( db_ctree[area], 0, sizeof( struct _db_ctree ) );
        memcpy( db_ctree[area]->nombre, nombre, 30 );
        //--------- abre el archivo ( OPEN ) -----------
        //l_buffer = (clave_acceso) ? strlen(clave_acceso) : 0;
        //ok = btrv(0, clave_acceso, &l_buffer, nombre, &modo);
        // printf("\n btrv abrio archivo %d area %d",ok,area);
        #if defined(INVEL_L)
        if( ( file_no = OPNRFILX( -1,             // file number, -1 means auto assign it
        nombre_tmp,                           // data file name with extension
        SHARED,                               // data file mode
        NULL ) ) < 0 )                        // data file password
        #else
            if( ( file_no = OPNRFILX( -1,         // file number, -1 means auto assign it
            nombre,                           // data file name with extension
            SHARED,                           // data file mode
            NULL ) ) < 0 )                    // data file password
            #endif
            {
            }
        #if INVEL_L
        fprintf( stderr, "OPNRFILX %s (%d,%d) fileno= %i\n", nombre_tmp, isam_err, sysiocod,
                 file_no );
        #endif
        error = isam_err;
    };

    //------------ Si no hubo error pide STATUS -----------
    if( !error ) {
        db_ctree[area]->file_no = file_no;
        res_len = GETIFIL( file_no, 0, NULL );
        //   info = GETFIL(FILE_NO,FILMOD);
        db_ctree[area]->file_type = GETFIL( file_no, FILTYP );

        if( !res_len )        /* IFIL not found */ {
            //printf("Error getting IFIL, error = %d.\n",isam_err);
            //if (isam_err == RNOT_ERR)
            // printf("IFIL information not stored for this file.\n");
            //printf("The FairCom(R) ODBC Driver can not be used without the IFIL structure.\n\n");
        }        /* IFIL found */
        else {
            //printf("\nPRESS <Return> KEY TO CONTINUE\n");
            //getchar();

            db_ctree[area]->work_pIFIL = ( pIFIL )malloc( ( size_t )res_len );

            if( !db_ctree[area]->work_pIFIL ) {
                printf( "Error allocating space for IFIL.\n" );
                error = -1;
            }
            if( !error ) {
                buf_size = res_len;
                res_len = GETIFIL( file_no, buf_size, db_ctree[area]->work_pIFIL );
                if( !res_len )                /* IFIL not found */ {
                    printf( "Error getting IFIL, error = %d.\n", isam_err );
                    if( isam_err == RNOT_ERR ) {
                        printf( "IFIL information not stored for this file.\n" );
                    }
                    error = -2;
                }
            }

            // Display IFIL, IIDX, ISEG
            if( !error ) {
                if( tam_estructura > 0 && tam_estructura != db_ctree[area]->work_pIFIL->dreclen ) {
                    char mensaje[100];

                    _snprintf( mensaje, sizeof(mensaje)-1, "Dif tam: nombre %s tam %d btrv %d", nombre,
                             db_ctree[area]->work_pIFIL->dreclen, tam_estructura );
                    printf( "Dif tam: nombre %s DB File = %d Header = %d\n", nombre,
                            db_ctree[area]->work_pIFIL->dreclen, tam_estructura );
                    // Lo saque para que deje pasar el VARIOS.BTR
                    //error = -3;
                    //ABORT(mensaje);
                }
            }
            // printf("\n\n stat.long_registro %u largo %u
            // \n",stat.long_registro,largo);
            // getch();
            if( largo == 0 ) {
                //largo = stat.long_registro;
                largo = db_ctree[area]->work_pIFIL->dreclen;
            }
            if( estructura ) {
                db_ctree[area]->puntero_registro = estructura;
                db_ctree[area]->buffer_interno = NO;
            }
            else {
                db_ctree[area]->puntero_registro = ( char* )MALLOC_CLEAR( largo );
                db_ctree[area]->buffer_interno = SI;
            }
            db_ctree[area]->status = 0;
            //db_ctree[area]->indices = stat.indices;
            db_ctree[area]->indices = db_ctree[area]->work_pIFIL->dnumidx;

            //db_ctree[area]->registros = stat.registros;
            db_ctree[area]->registros = NbrOfRecords( db_ctree[area]->file_no );
            db_ctree[area]->key_number = 0;
            db_ctree[area]->long_buffer = largo;

            //-------------- inicializa el archivo y status ---------------
            if( !db_ctree[area]->puntero_registro ) {
                error = 99;
            }
            else if( db_ctree[area]->registros ) {
                // printf("\n\n llendo al principio \n");
                Ctree_SET_ORDER( 1 );
                Ctree_GO( TOP );
            }
            else {
                Ctree_SET_STATUS( S_BOF, 1 );
                Ctree_SET_STATUS( S_EOF, 1 );
            }
        }
    }
    //------------ Si hubo error devuelve la memoria y lo trata ----------
    if( error && db_ctree[area] ) {
        if( db_ctree[area]->work_pIFIL ) {
            FREEMEM( ( char* )db_ctree[area]->work_pIFIL );
        }
        if( db_ctree[area]->buffer_interno ) {
            FREEMEM( db_ctree[area]->puntero_registro );
        }

        FREEMEM( ( char* )db_ctree[area] );
        db_ctree[area] = NULL;
    }
    printf( "Done reading file error = %i\n", error );
    #if INVEL_L
    free( nombre_tmp );
    #endif
    return ( error );
}

/*****************************************************************************/
void Ctree_CLOSE_DB( int cod )
/*****************************************************************************/
{
    Ctree_CLOSE_DATABASE( AREA_OF( cod ) );
}


/*****************************************************************************/
int Ctree_CLOSE_DATABASE( int area )
/*****************************************************************************/
{
    int ok = 0, area_ant;

    area_ant = Ctree_SELECTED();
    if( db_ctree[area] ) {
        Ctree_SELECT( area );
        CLRFIL( db_ctree[area]->file_no );

        //ok = btrv(1, NULL, NULL, NULL, NULL);
        //-------------- libera el area y la memoria asignada ------------
        if( db_ctree[area]->buffer_interno ) {
            FREEMEM( db_ctree[area]->puntero_registro );
        }
        if( db_ctree[area]->work_pIFIL ) {
            FREEMEM( ( char* )db_ctree[area]->work_pIFIL );
        }
        FREEMEM( ( char* )db_ctree[area] );
        db_ctree[area] = NULL;
        UsedByCtree[area] = 0;
    }
    Ctree_SELECT( area_ant );

    return ( ok );
}


/*****************************************************************************/
void Ctree_CLOSE_ALL()
/*****************************************************************************/
{
    //_HUBO_CLOSE_ALL = SI;
    int area;

    for( area = 0;area < MAX_AREA;area++ ) {
        Ctree_CLOSE_DATABASE( area );
    }
    area = 0;
}


/*****************************************************************************/
void Ctree_SELECT( int num_area )
/*****************************************************************************/
{
    if( area__ >= 0 && area__ < MAX_AREA ) {
        area__ = num_area;
    }
    else {
        //ERROR_(0, "Area invalida");
    }
}


/*****************************************************************************/
int Ctree_SELECTED()
/*****************************************************************************/
{
    return ( area__ );
}


/*****************************************************************************/
void Ctree_SET_ORDER( int number_key )
/*****************************************************************************/
{
    if( db_ctree[area__] ) {
        db_ctree[area__]->key_number = number_key;
    }
}


/*****************************************************************************/
int Ctree_GET_ORDER( void )
/*****************************************************************************/
{
    int order = 1;

    if( db_ctree[area__] ) {
        order = db_ctree[area__]->key_number;
    }

    return ( order );
}


/*****************************************************************************/
int Ctree_BOF()
/*****************************************************************************/
{
    return ( Ctree_GET_STATUS( S_BOF ) );
}


/*****************************************************************************/
int Ctree_EOF()
/*****************************************************************************/
{
    return ( Ctree_GET_STATUS( S_EOF ) );
}


/*****************************************************************************/
int Ctree_FOUND()
/*****************************************************************************/
{
    return ( Ctree_GET_STATUS( S_FOUND ) );
}


/*****************************************************************************/
int Ctree_EMPTY()
/*****************************************************************************/
{
    int rta;

    rta = ( Ctree_RECCOUNT() ) ? 0 : 1;
    return ( rta );
}


/*****************************************************************************/
unsigned long Ctree_RECCOUNT( void )
/*****************************************************************************/
{
    return ( ( db_ctree[area__] ) ? db_ctree[area__]->registros : 0 );
}


/*****************************************************************************/
void Ctree_DELETE()
/*****************************************************************************/
{
    if( !Ctree_TESTEAR( 8 ) ) {
        if( db_ctree[area__] ) {
            db_ctree[area__]->puntero_registro[0] |= 0x80;
        }
        Ctree_UPDATE();
    }
}


/*****************************************************************************/
void Ctree_RECALL()
/*****************************************************************************/
{
    if( Ctree_TESTEAR( 8 ) ) {
        if( db_ctree[area__] ) {
            db_ctree[area__]->puntero_registro[0] &= 0x7f;
        }
        Ctree_UPDATE();
    }
}


/*****************************************************************************/
int Ctree_DELETED()
/*****************************************************************************/
{
    int rta = 0;

    if( db_ctree[area__] ) {
        rta = db_ctree[area__]->puntero_registro[0] & 0x80;
    }
    return ( rta );
}


/*****************************************************************************/
void Ctree_DELETE_ALL()
/*****************************************************************************/
{
    Ctree_GO( TOP );
    while( !Ctree_EOF() ) {
        Ctree_DELETE();
        Ctree_SKIP( 1 );
    }
}


/*****************************************************************************/
void Ctree_RECALL_ALL()
/*****************************************************************************/
{
    Ctree_GO( TOP );
    while( !Ctree_EOF() ) {
        Ctree_RECALL();
        Ctree_SKIP( 1 );
    }
}


/*****************************************************************************/
int Ctree_TESTEAR( char orden )
/*****************************************************************************/
{
    char rotacion;
    int rta = 0;

    rotacion = 1 << ( orden - 1 );
    if( db_ctree[area__] ) {
        rta = db_ctree[area__]->puntero_registro[0] & rotacion;
    }
    return ( rta );
}


/*****************************************************************************/
void Ctree_SETEAR( char orden, char bit )
/*****************************************************************************/
{
    char rotacion;

    rotacion = 1 << ( orden - 1 );
    if( !bit ) {
        rotacion = 255 - rotacion;
        if( db_ctree[area__] ) {
            db_ctree[area__]->puntero_registro[0] &= rotacion;
        }
    }
    else {
        if( db_ctree[area__] ) {
            db_ctree[area__]->puntero_registro[0] |= rotacion;
        }
    }
    Ctree_UPDATE();
}


/*****************************************************************************/
void Ctree_PACK()
/*****************************************************************************/
{
    Ctree_GO( TOP );
    while( !Ctree_EOF() ) {
        if( Ctree_DELETED() ) {
            Ctree__DELETE();
        }
        Ctree_SKIP( 1 );
    }
    Ctree_GO( TOP );
}


/*****************************************************************************/
void Ctree_ZAP()
/*****************************************************************************/
{
    Ctree_GO( TOP );
    while( Ctree_FOUND() ) {
        Ctree__DELETE();
        Ctree_GO( TOP );
    }
    Ctree_SET_STATUS( S_BOF, 1 );
    Ctree_SET_STATUS( S_EOF, 1 );
}


/*****************************************************************************/
int Ctree__DELETE()
/*****************************************************************************/
{
    int ok;

    //ok = btrv(4, NULL, NULL, NULL, NULL);
    ok = DELREC( db_ctree[area__]->file_no );
    if( ok == 0 ) {
        if( db_ctree[area__] ) {
            db_ctree[area__]->registros--;
        }
    }
    if( Ctree_GET_NEXT() == INOT_ERR ) {
        Ctree_GET_PREVIOUS();
    }
    return ( ok );
}


/*****************************************************************************/
int Ctree_END_TRANSACTION()
/*****************************************************************************/
{
    int error = 0;

    //ok = btrv(20, NULL, NULL, NULL, NULL);
    error = TRANEND( FREE );
    return ( !error );
}


/*****************************************************************************/
int Ctree_GET_DIRECTORY( char *buffer, int drive )
/*****************************************************************************/
{
    int ok = 0;

    //ok = btrv(-18, NULL, NULL, buffer, &drive);
    return ( ok );
}


/*****************************************************************************/
int Ctree_INSERT()
/*****************************************************************************/
{
    int ok = 100;

    if( db_ctree[area__] == NULL ) {
        return ( 100 );
    }
    db_ctree[area__]->puntero_registro[0] = 0;
    ok = Ctree__INSERT( db_ctree[area__]->long_buffer );
    return ( ok );
}


/*****************************************************************************/
int Ctree_INSERT_NO_MODIF_STATUS()
/*****************************************************************************/
{
    int ok = 100;

    if( db_ctree[area__] == NULL ) {
        return ( 100 );
    }
    ok = Ctree__INSERT( db_ctree[area__]->long_buffer );
    return ( ok );
}


/*****************************************************************************/
int Ctree_INSERT_VARIABLE( int long_reg )
/*****************************************************************************/
{
    int ok = 100;

    if( db_ctree[area__] == NULL ) {
        return ( 100 );
    }
    db_ctree[area__]->puntero_registro[0] = 0;
    ok = ADDVREC( db_ctree[area__]->file_no, db_ctree[area__]->puntero_registro, long_reg );
    if( ok == 0 ) {
        db_ctree[area__]->registros++;
        Ctree_SET_STATUS( S_BOF, 0 );
        Ctree_SET_STATUS( S_EOF, 0 );
    }
    return ( ok );
}


/*****************************************************************************/
int Ctree__INSERT( int long_reg )
/*****************************************************************************/
{
    int ok;
    int l_buffer;

    if( db_ctree[area__] == NULL ) {
        return ( 100 );
    }
    l_buffer = long_reg;
    //ok = btrv(2, NULL, &l_buffer, NULL, NULL);


    ok = LKISAM( ENABLE );

    ok = ADDREC( db_ctree[area__]->file_no, db_ctree[area__]->puntero_registro );

    ok = LKISAM( FREE );
    if( ok == 0 ) {
        db_ctree[area__]->registros++;
        Ctree_SET_STATUS( S_BOF, 0 );
        Ctree_SET_STATUS( S_EOF, 0 );
    }
    return ( ok );
}


/*****************************************************************************/
int Ctree_SET_DIRECTORY( char *path )
/*****************************************************************************/
{
    int ok = 0;

    //ok = btrv(-17, NULL, NULL, path, NULL);
    return ( ok );
}


/*****************************************************************************/
int Ctree_ABORT_TRANSACTION()
/*****************************************************************************/
{
    int ok;

    ok = TRANEND( FREE );
    //ok = btrv(21, NULL, NULL, NULL, NULL);
    return ( ok );
}


/*****************************************************************************/
int Ctree_BEGIN_TRANSACTION()
/*****************************************************************************/
{
    int ok;

    //ok = btrv(19, NULL, NULL, NULL, NULL);
    ok = TRANBEG( TRNLOG | ENABLE | SAVENV );
    return ( ok );
}


/*****************************************************************************/
int Ctree_BEGIN_TRANSACTION_CONCURRENTE()
/*****************************************************************************/
{
    int ok = 0;

    //ok = btrv(1519, NULL, NULL, NULL, NULL);
    ok = TRANBEG( TRNLOG | ENABLE | SAVENV );
    return ( ok );
}


/*****************************************************************************/
int Ctree_STAT( char *buffer, int longitud, char *extension )
/*****************************************************************************/
{
    int ok = -1;

    if( db_ctree[area__] ) {
        //ok = btrv(15, buffer, &longitud, extension, NULL);
    }
    return ( ok );
}


/*****************************************************************************/
int Ctree_UNLOCK()
/*****************************************************************************/
{
    int ok = -1;

    if( db_ctree[area__] ) {
        //ok = btrv(27, NULL, NULL, NULL, NULL);
    }
    return ( ok );
}


/*****************************************************************************/
int Ctree_UNLOCK_SINGLE( long registro )
/*****************************************************************************/
{
    int ok = -1, l_buffer = 4, nro_key = -1;

    if( db_ctree[area__] ) {
        if( !registro ) {
            registro = GET_POSITION();
        }
        //ok = btrv(27, (char *) (&registro), &l_buffer, NULL, &nro_key);
    }
    return ( ok );
}


/*****************************************************************************/
int Ctree_UNLOCK_ALL()
/*****************************************************************************/
{
    int ok = -1, key_number = -2;

    if( db_ctree[area__] ) {
        //ok = btrv(27, NULL, NULL, NULL, &key_number);
    }
    return ( ok );
}


/*****************************************************************************/
int Ctree_UPDATE()
/*****************************************************************************/
{
    int ok;

    //ok = btrv(3, NULL, NULL, NULL, NULL);
    if( db_ctree[area__]->file_type & FILE_TYPE_VARIABLE_LENGTH ) {
        ok = RWTVREC( db_ctree[area__]->file_no, db_ctree[area__]->puntero_registro,
                      db_ctree[area__]->long_buffer );
    }
    else {
        ok = RWTREC( db_ctree[area__]->file_no, db_ctree[area__]->puntero_registro );
    }
    if( ok == NO_ERROR ) {
        // no se pierde la posicion en el archivo
        //ok = UPDCURI(db_ctree[area__]->file_no, SWTCURI);
    }

    return ( ok );
}


/*---------------------------------------------------------------------------*/
/*                   FUNCIONES DE MOVIMIENTO SIN INDICES                     */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
long Ctree_GET_POSITION()
/*****************************************************************************/
{
    int largo = 4, ok = 0;
    long posicion = 0;

    if( db_ctree[area__] == NULL ) {
        return ( 0L );
    }
    //if (db_ctree[area__]->registros)
    if( NbrOfRecords( db_ctree[area__]->file_no ) ) {
        //ok = btrv(22, (char *) (&posicion), &largo, NULL, NULL);
        posicion = CurrentFileOffset( db_ctree[area__]->file_no );
        if( ok != 0 ) {
            posicion = 0;
        }
    }
    return ( posicion );
}


/*****************************************************************************/
int Ctree_GET_DIRECT( unsigned long posicion )
/*****************************************************************************/
{
    int ok = 100;

    //long buffer;
    VRLEN numberOfBytes;

    if( db_ctree[area__] == NULL ) {
        return ( 100 );
    }
    if( posicion ) {
        //buffer = *(long *) (db_ctree[area__]->puntero_registro);
        ///*(long *) (db_ctree[area__]->puntero_registro) = posicion;
        //ok = btrv(23, NULL, NULL, NULL, NULL);
        if( db_ctree[area__]->file_type & FILE_TYPE_VARIABLE_LENGTH ) {
            numberOfBytes = db_ctree[area__]->long_buffer;
            ok = REDIVREC( db_ctree[area__]->file_no, posicion, db_ctree[area__]->puntero_registro,
                           &numberOfBytes );
        }
        else {
            ok = REDIREC( db_ctree[area__]->file_no, posicion, db_ctree[area__]->puntero_registro );
        }

        Ctree_sets_busqueda( ok );
        if( ok != 0 ) {
            ///*(long *) (db_ctree[area__]->puntero_registro) = buffer;
        }
    }
    return ( ok );
}


/*****************************************************************************/
int Ctree_STEP_FIRST()
/*****************************************************************************/
{
    int ok = 0;

    //ok = btrv(33, NULL, NULL, NULL, NULL);
    Ctree_sets_busqueda( ok );
    if( ok == INOT_ERR ) {
        Ctree_SET_STATUS( S_BOF, 1 );
        Ctree_SET_STATUS( S_EOF, 1 );
    }
    return ( ok );
}


/*****************************************************************************/
int Ctree_STEP_LAST()
/*****************************************************************************/
{
    int ok = 0;

    // ok = btrv(34, NULL, NULL, NULL, NULL);
    Ctree_sets_busqueda( ok );
    if( ok == INOT_ERR ) {
        Ctree_SET_STATUS( S_BOF, 1 );
        Ctree_SET_STATUS( S_EOF, 1 );
    }
    return ( ok );
}


/*****************************************************************************/
int Ctree_STEP_NEXT()
/*****************************************************************************/
{
    int ok = 0;

    //ok = btrv(24, NULL, NULL, NULL, NULL);
    Ctree_sets_mov_aba( ok );
    return ( ok );
}


/*****************************************************************************/
int Ctree_STEP_PREVIOUS()
/*****************************************************************************/
{
    int ok = 0;

    //ok = btrv(35, NULL, NULL, NULL, NULL);
    Ctree_sets_mov_arr( ok );
    return ( ok );
}


/*---------------------------------------------------------------------------*/
/*                   FUNCIONES DE MOVIMIENTO CON INDICE                      */
/*---------------------------------------------------------------------------*/
char buffTmp[128];
void Ctree__CopyClave( char *clave )
{
    int bytes = 0;
    int aux;
    bytes = db_ctree[area__]->work_pIFIL->ix[db_ctree[area__]->key_number - 1].ikeylen;
    if( db_ctree[area__]->work_pIFIL->ix[db_ctree[area__]->key_number - 1].ikeydup ) {
        aux = bytes - 4;
        memset( buffTmp, 0, sizeof( buffTmp ) );
        memcpy( buffTmp, clave, ( ( aux > sizeof( clave ) ) ? sizeof( clave ) : aux ) );
    }
    else {
        memset( buffTmp, 0, sizeof( buffTmp ) );
        memcpy( buffTmp, clave, ( ( bytes > sizeof( clave ) ) ? sizeof( clave ) : bytes ) );
    }
}


/*****************************************************************************/
int Ctree_GET_EQUAL( char *clave )
/*****************************************************************************/
{
    int ok,ok1;
    //    int       res;
    int i;
    // char*     aux;
    int numSegment;
    long segOffset;
    long segSize;
    int keyOffset;
    int keyType;

    Ctree__CopyClave( clave );


    if( db_ctree[area__]->work_pIFIL->ix[db_ctree[area__]->key_number - 1].ikeydup ) {
        // Como tengo duplicados no puedo utilizar EQLREC
        ok = GTEREC( ( COUNT )( db_ctree[area__]->file_no + db_ctree[area__]->key_number ), clave,
                     db_ctree[area__]->puntero_registro );
        numSegment = db_ctree[area__]->work_pIFIL->ix[db_ctree[area__]->key_number - 1].inumseg;
        keyType = db_ctree[area__]->work_pIFIL->ix[db_ctree[area__]->key_number - 1].ikeytyp;  

        keyOffset = 0; 
        if( !ok ) {
            for( i = 0;i < numSegment;i++ ) {
                segOffset = db_ctree[area__]->work_pIFIL->
                ix[db_ctree[area__]->key_number - 1].seg[i].soffset;
                segSize = db_ctree[area__]->work_pIFIL->
                ix[db_ctree[area__]->key_number - 1].seg[i].slength;
                ok1 = memcmp( &clave[keyOffset], db_ctree[area__]->puntero_registro + segOffset,
                              segSize );

                if( ok1 ) //Si fallo memcmp uso strncmp
                {
                    ok |= strncmp( &clave[keyOffset],
                                   db_ctree[area__]->puntero_registro + segOffset, segSize );
                }

                keyOffset += segSize;
            }
        }
    }
    else {
        ok = EQLREC( ( COUNT )( db_ctree[area__]->file_no + db_ctree[area__]->key_number ), clave,
                     db_ctree[area__]->puntero_registro );
    }
    if( ok == 101 ) {
        printf( "Error elemnt not found %i\n", *( ( int* )buffTmp ) );
    }
    Ctree_sets_busqueda( ok );
    return ( ok );
}


/*****************************************************************************/
int Ctree_GET_GREATER( char *clave )
/*****************************************************************************/
{
    int ok;

    //ok = btrv(8, NULL, NULL, clave, NULL);
    Ctree__CopyClave( clave );
    ok = GTREC( ( COUNT )( db_ctree[area__]->file_no + db_ctree[area__]->key_number ), buffTmp,
                db_ctree[area__]->puntero_registro );
    Ctree_sets_busqueda( ok );
    return ( ok );
}


/*****************************************************************************/
int Ctree_GET_GREATER_E( char *clave )
/*****************************************************************************/
{
    int ok;

    Ctree__CopyClave( clave );
    //ok = btrv(9, NULL, NULL, clave, NULL);
    ok = GTEREC( ( COUNT )( db_ctree[area__]->file_no + db_ctree[area__]->key_number ), buffTmp,
                 db_ctree[area__]->puntero_registro );
    Ctree_sets_busqueda( ok );
    return ( ok );
}


/*****************************************************************************/
int Ctree_GET_LESS( char *clave )
/*****************************************************************************/
{
    int ok;

    //ok = btrv(10, NULL, NULL, clave, NULL);
    // extern ctCONV  COUNT ctDECL LTREC(COUNT keyno,pVOID target,pVOID recptr);
    Ctree__CopyClave( clave );
    ok = LTREC( ( COUNT )( db_ctree[area__]->file_no + db_ctree[area__]->key_number ),
                ( void* )buffTmp, ( void* )db_ctree[area__]->puntero_registro );
    Ctree_sets_busqueda( ok );
    return ( ok );
}


/*****************************************************************************/
int Ctree_GET_LESS_E( char *clave )
/*****************************************************************************/
{
    int ok;

    Ctree__CopyClave( clave );
    //ok = btrv(11, NULL, NULL, clave, NULL);
    ok = LTEREC( ( COUNT )( db_ctree[area__]->file_no + db_ctree[area__]->key_number ), buffTmp,
                 db_ctree[area__]->puntero_registro );
    Ctree_sets_busqueda( ok );
    return ( ok );
}


/*****************************************************************************/
int Ctree_GET_NEXT()
/*****************************************************************************/
{
    int ok;

    //ok = btrv(6, NULL, NULL, NULL, NULL);
    ok = NXTREC( ( COUNT )( db_ctree[area__]->file_no + db_ctree[area__]->key_number ),
                 db_ctree[area__]->puntero_registro );
    Ctree_sets_mov_aba( ok );
    if( db_ctree[area__]->file_type & FILE_TYPE_VARIABLE_LENGTH ) {
        ok = Ctree_ReReadVRecord( db_ctree[area__]->file_no, db_ctree[area__]->puntero_registro,
                                  db_ctree[area__]->long_buffer );
    }
    return ( ok );
}


/*****************************************************************************/
int Ctree_GET_PREVIOUS()
/*****************************************************************************/
{
    int ok;

    //ok = btrv(7, NULL, NULL, NULL, NULL);
    ok = PRVREC( ( COUNT )( db_ctree[area__]->file_no + db_ctree[area__]->key_number ),
                 db_ctree[area__]->puntero_registro );
    Ctree_sets_mov_arr( ok );

    if( db_ctree[area__]->file_type & FILE_TYPE_VARIABLE_LENGTH ) {
        ok = Ctree_ReReadVRecord( db_ctree[area__]->file_no, db_ctree[area__]->puntero_registro,
                                  db_ctree[area__]->long_buffer );
    }

    return ( ok );
}


/*****************************************************************************/
int Ctree_ReReadVRecord( int datno, char *recptr, int len )
/*****************************************************************************/
{
    int l;
    int ok = 0;

    l = VRecordLength( ( COUNT )datno );
    if( l > len ) {
        l = len;
    }
    ok = ( int )ReReadVRecord( ( COUNT )datno, ( void* )recptr, ( VRLEN )l );

    return ok;
}


/*****************************************************************************/
int Ctree_GET_FIRST()
/*****************************************************************************/
{
    int ok;

    //ok = btrv(12, NULL, NULL, NULL, NULL);
    ok = FRSREC( ( COUNT )( db_ctree[area__]->file_no + db_ctree[area__]->key_number ),
                 db_ctree[area__]->puntero_registro );
    Ctree_sets_busqueda( ok );
    if( ok == INOT_ERR ) {
        Ctree_SET_STATUS( S_BOF, 1 );
        Ctree_SET_STATUS( S_EOF, 1 );
    }
    return ( ok );
}


/*****************************************************************************/
int Ctree_GET_LAST()
/*****************************************************************************/
{
    int ok;

    ok = LSTREC( ( COUNT )( db_ctree[area__]->file_no + db_ctree[area__]->key_number ),
                 db_ctree[area__]->puntero_registro );
    Ctree_sets_busqueda( ok );
    if( ok == INOT_ERR ) {
        Ctree_SET_STATUS( S_BOF, 1 );
        Ctree_SET_STATUS( S_EOF, 1 );
    }
    return ( ok );
}


/*---------------------------------------------------------------------------*/
/*           ACTUALIZACION DE STATUS DESPUES DE ALGUN MOVIMIENTO             */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
void Ctree_sets_busqueda( int status )
/*****************************************************************************/
{
    if( status != 0 ) {
        Ctree_SET_STATUS( S_FOUND, 0 );
    }
    else {
        Ctree_SET_STATUS( S_FOUND, 1 );
        Ctree_SET_STATUS( S_BOF, 0 );
        Ctree_SET_STATUS( S_EOF, 0 );
    }
    if( db_ctree[area__]->file_type & FILE_TYPE_VARIABLE_LENGTH ) {
        Ctree_ReReadVRecord( db_ctree[area__]->file_no, db_ctree[area__]->puntero_registro,
                             db_ctree[area__]->long_buffer );
    }
}


/*****************************************************************************/
int Ctree_sets_mov_arr( int status )
/*****************************************************************************/
{
    if( db_ctree[area__] == NULL ) {
        return ( 100 );
    }
    if( status != 0 ) {
        if( status == INOT_ERR ) {
            //if (db_ctree[area__]->registros)
            if( NbrOfRecords( db_ctree[area__]->file_no ) ) {
                Ctree_SET_STATUS( S_EOF, 0 );
            }
            Ctree_SET_STATUS( S_FOUND, 0 );
            Ctree_SET_STATUS( S_BOF, 1 );
        }
    }
    else {
        Ctree_SET_STATUS( S_FOUND, 1 );
        Ctree_SET_STATUS( S_BOF, 0 );
        Ctree_SET_STATUS( S_EOF, 0 );
    }
    return 0;
}


/*****************************************************************************/
int Ctree_sets_mov_aba( int status )
/*****************************************************************************/
{
    if( db_ctree[area__] == NULL ) {
        return ( 100 );
    }
    if( status != 0 ) {
        if( status == INOT_ERR ) {
            //if (db_ctree[area__]->registros)
            if( NbrOfRecords( db_ctree[area__]->file_no ) ) {
                Ctree_SET_STATUS( S_BOF, 0 );
            }
            Ctree_SET_STATUS( S_FOUND, 0 );
            Ctree_SET_STATUS( S_EOF, 1 );
        }
    }
    else {
        Ctree_SET_STATUS( S_FOUND, 1 );
        Ctree_SET_STATUS( S_BOF, 0 );
        Ctree_SET_STATUS( S_EOF, 0 );
    }
    return 0;
}


/*---------------------------------------------------------------------------*/
/*          FUNCIONES GENERALES DE MOVIMIENTO (CON O SIN INDICES)            */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
int Ctree_SKIP( int salto )
/*****************************************************************************/
{
    int respuesta = 0, i = 0;

    if( db_ctree[area__] == NULL ) {
        return ( 0 );
    }
    if( salto > 0 ) {
        for( i = 0;i < salto && !respuesta;i++ ) {
            respuesta = ( db_ctree[area__]->indices ) ? Ctree_GET_NEXT() : Ctree_STEP_NEXT();
        }
        if( respuesta && i ) {
            i--;
        }
    }
    else {
        for( i = 0;i > salto && !respuesta;i-- ) {
            respuesta = ( db_ctree[area__]->
            indices ) ? Ctree_GET_PREVIOUS() : Ctree_STEP_PREVIOUS();
        }
        if( respuesta && i ) {
            i++;
        }
    }

    return ( i );
}


/*****************************************************************************/
int Ctree_GO( int registro )
/*****************************************************************************/
{
    if( db_ctree[area__] == NULL ) {
        return ( 100 );
    }
    switch( registro ) {
        case TOP:
            if( db_ctree[area__]->indices ) {
                Ctree_GET_FIRST();
            }
            else {
                Ctree_STEP_FIRST();
            }
            break;

        case BOTTOM:
            if( db_ctree[area__]->indices ) {
                Ctree_GET_LAST();
            }
            else {
                Ctree_STEP_LAST();
            }
            break;
    }
    return 0;
}


/*****************************************************************************/
//BTRIEVE_PRESENTE()
/*****************************************************************************
 * {
 *      char buffer[255];
 *      int rta = 1;
 *      if (GET_DIRECTORY(buffer, 0) == 20)
 *           rta = 0;
 *      return (rta);
 * }
 */
/*****************************************************************************/
int Ctree_USED( int area )
/*****************************************************************************/
{
    int rta = 0;

    if( db_ctree[area__] != NULL ) {
        rta = 1;
    }
    return ( rta );
}


/*****************************************************************************/
int Ctree_SIZEOF_REC_DB( int area )
/*****************************************************************************/
{
    return ( ( db_ctree[area__] ) ? db_ctree[area__]->long_buffer : 0 );
}


/*****************************************************************************/
char * Ctree_BUFFER_REC_DB( int area )
/*****************************************************************************/
{
    return ( ( db_ctree[area__] ) ? db_ctree[area__]->puntero_registro : 0 );
}


/*****************************************************************************/
int Ctree_CARGAR_DB( int cod, char *buffer, int *l_reg )
/*****************************************************************************/
{
    char nombre[80];

    NOM_ARCHIVO( cod, nombre );

    return ( Ctree_CARGAR_DATABASE( nombre, buffer, l_reg ) );
}


/*****************************************************************************/
int Ctree_CARGAR_DATABASE( char *nombre, char *buffer, int *l_reg )
/*****************************************************************************/
{
    int c_reg, h, ok = 0;
    char *p_reg;

    //int area, char *nombre, char *estructura, int tam_estructura, char * clave_acceso, int largo,int modo
    if( !Ctree_USE_DATABASE( 0, nombre, NULL, 0, NULL, 0, 0 ) ) {
        *l_reg = Ctree_SIZEOF_REC_DB( 0 );
        c_reg = ( int )Ctree_RECCOUNT();
        p_reg = Ctree_BUFFER_REC_DB( 0 );
        buffer = ( char* )calloc( c_reg, ( *l_reg ) );
        if( buffer ) {
            ok = 1;
            for( h = 0;h < c_reg;h++ ) {
                memcpy( &buffer[h * ( *l_reg )], p_reg, *l_reg );
                Ctree_SKIP( 1 );
            }
        }
    }
    Ctree_CLOSE_DATABASE( 0 );

    return ( ok );
}


/*****************************************************************************/
int Ctree_AREA_DISPONIBLE( void )
/*****************************************************************************/
{
    int area = 0, h;

    for( h = 1;h < MAX_AREA && !area;h++ ) {
        if( !db_ctree[h] ) {
            area = h;
        }
    }

    return ( area );
}


TEXT uid[MAX_NAME] = "CAJA";                 // user id
TEXT upw[MAX_NAME] = "caja";                 // user password
TEXT svn[MAX_NAME] = "FAIRCOMS@127.0.0.1";   // server name
TEXT last_ctree[MAX_NAME];

/*****************************************************************************/
int Ctree_Init()
/*****************************************************************************/
{
    COUNT result;

    if( result = INTISAMX( 6,                        // 6 index buffers
   100,                                         // 10 files (five extra for optional rebuild
   100,                                           // page sectors => 512 bytes cache page size
   6,                                       // 6 data file buffers
   0,                                        // UserProfile: perform automatic TFRMKEYs
   uid,                                      // pointer to user id
   upw,                                      // pointer to user password
   svn ) )                                   // pointer to server name
    {
        //char      msg[256];
        printf( "Unable to initialize c-tree (%d)\n", result );
        return result;
    }

    return ( NO_ERROR );
}

#endif
