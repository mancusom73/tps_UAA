#include "CObject.h"
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"
#include <time.h>
#ifdef INVEL_L
#include <sys/io.h>
#include <sys/types.h>
#include <unistd.h>
#else
#include <io.h>
#include <conio.h>
#include <dos.h>
#endif

#include "path.h"



int mutex_count = 0;

extern "C"
{
    extern int _GRABAR_LOG_SISTEMA( char *operacion ,int origen, int nivel );
//	extern int _GRABAR_LOG_SISTEMA( char *operacion );
	extern  /*int*/ void GRABAR_LOG_SISTEMA( char *operacion ,int origen, int nivel );
}


void sglog( const char *buffer )
{/// LOG_JAVA_SERVER = 9;
	int LOG_INCONDICIONAL = 24;
	
    GRABAR_LOG_SISTEMA((char *)buffer,LOG_INCONDICIONAL,1); //ojo de poner solo los errores graves
}

void __mutex_enter( int line, char *file )
{
    mutex_count++;
#ifdef _DEBUG
    assert( mutex_count == 1 );
#else
    if( mutex_count > 1 ) {
        char buff[1000];
        _snprintf(buff, sizeof(buff)-1, "FATAL ERROR ----------------- MUTEX COUNT %s(%i)\n", file, line );
        sglog( buff );
    }
#endif
    wxMutexGuiEnter();
}


void __mutex_leave( int line, char *file )
{
    mutex_count--;
    wxMutexGuiLeave();
}


void CObject::SetID( int i )
{
    if( id == 0 ) {
        id = i;
    }
    else {
        char aux[35];
		_snprintf(aux, sizeof(aux)-1, "ERROR Duplicate Object: %d size: %d\n", id, i );
        sglog( aux );
    }
}
