#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifdef INVEL_L
#else
#endif
#if !defined(INVEL_W) && !defined(INVEL_L)
/*****************************************************************************/
GET_PTR_LISTA_DE_LISTAS( char **ptr )
/*****************************************************************************/
{
    union REGS regs;
    struct SREGS sregs;
    regs.h.ah = 0x52;
    intdosx( &regs, &regs, &sregs );
    *ptr = MK_FP( sregs.es, regs.x.bx );
}
/*****************************************************************************/
_NOMBRE_ARCHIVO( int handle, char *nombre, char *ext )
/*****************************************************************************/
{
    int sfn,pos;
    char *ptr,**handles;
    struct _sft
    {
        char reserva1[32];
        char nombre[11];
        char reserva2[16];
    }        *sft;
    struct _sft_header
    {
        char *ptr;
        int cant;
    }        *sft_header;
    /*-------- Calcula el subindice dentro del SFT segn el handle --------*/
    handles = MK_FP( _psp, 0x34 );
    ptr = *handles;
    sfn = ptr[handle];
    /*------------------------- Apunta al 1 SFT ---------------------------*/
    GET_PTR_LISTA_DE_LISTAS( &ptr );
    sft_header = ( struct _sft_header * )( ptr + 4 );
    sft_header = ( struct _sft_header * )( sft_header->ptr );
    sft = ( struct _sft * )( sft_header + 1 );
    /*------------------- Busca el bloque del SFT correcto -----------------*/
    pos = 0;
    while( sft_header->cant + pos <= sfn ) {
        pos += sft_header->cant;
        sft_header = ( struct _sft_header * )( sft_header->ptr );
        sft = ( struct _sft * )( sft_header + 1 );
    }
    /*------------------- Apunta a la entrada SFT buscada ------------------*/
    sft += sfn - pos;
    ptr = memccpy( ext, &sft->nombre[8], ' ', 3 );
    if( !ptr ) {
        ptr = &ext[4];
    }
    *( ptr - 1 ) = 0;
    ptr = memccpy( nombre, sft->nombre, ' ', 8 );
    if( !ptr ) {
        ptr = &nombre[9];
    }
    *( ptr - 1 ) = 0;
}
/*****************************************************************************/
NOMBRE_ARCHIVO( int handle, char *nombre )
/*****************************************************************************/
{
    char nom[20],ext[4];
    _NOMBRE_ARCHIVO( handle, nom, ext );
    strcpy( nombre, nom );
    if( strlen( ext ) ) {
        strcat( nombre, "." );
        strcat( nombre, ext );
    }
}
#endif

