#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <float.h>
#include <cr.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Redefinicion de la rutina de manejo de errores matematicos
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
/*****************************************************************************/
void cdecl _fperr()
/*****************************************************************************/
#else
     void _fperr ( )
#endif
{
    #ifndef INVEL_L
    _fpreset();
    #endif
    error_matematico = 1;
}

