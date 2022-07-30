/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <stdlib.h>
#pragma pack(1)
#include <cr.h>
#include <time.h>
/******************************************************************************/
unsigned long GET_TIME2()
/******************************************************************************/
{
	unsigned long hora;
	time( &hora );
	return( hora );
}
/******************************************************************************/
unsigned long GET_TIME()
/******************************************************************************/
{
    //static unsigned long t_ant = 0xffffffff;
    //  static unsigned f_ant, cambio_fecha;
    unsigned long hora;
    #if defined(INVEL_W) || defined(INVEL_L)
    hora = ( long )( clock() / CLOCKS_PER_SEC * 18.2 );
    #else
    unsigned fecha;
    unsigned long *t;
    t = ( void* )MK_FP_BIOS_PROTEGIDO( 0x6c );
    hora = *t;
    fecha = _GET_FECHA_DOS();
    if( t_ant > hora && hora < 2000 && t_ant != 0xffffffff ) {
        if( f_ant == fecha && !cambio_fecha ) {
            INCREMENTAR_DIA_DOS();
        }
    }
    t_ant = hora;
    cambio_fecha = 0;
    if( f_ant != fecha ) {
        cambio_fecha = 1;
    }
    f_ant = fecha;
    #endif
    return ( hora );
}
/******************************************************************************/
void SET_TIME( unsigned long valor )
/******************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    unsigned long *t;
    t = ( void* )MK_FP_BIOS_PROTEGIDO( 0x6c );
    *t = valor;
    #endif
}

