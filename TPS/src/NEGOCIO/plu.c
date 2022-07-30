#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <archivos.h> 
#include <btrv.h> 
#include <cio.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <plu.h> 
#include <tkt.h> 
#include <aarch.h>
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#pragma pack(1)
#include <cr.h>
#include <aarch.h>
#include <actions.h>
#include <math.h>

/*****************************************************************************/
long BUSCAR_PLU( int nro, char *cadena )
/*****************************************************************************/
{
    //int handle;
    long plu_ = 0;
    //struct _plu plu_sic;
    /*----------- Lee el archivo -------------*/
    /*
     * handle = OPEN( _PLU_DAT, O_RDONLY );
     * if( handle ) {
     * LSEEK( handle, (long)(4*nro), SEEK_SET, __LINE__, __FILE__  );
     * _READ( handle, (char *)&plu, sizeof(plu), __LINE__, __FILE__ );
     * }
     * _CLOSE( handle, __LINE__, __FILE__ );
     */
    nro++;
    if( /*USE_DB( AREA_AUX, _plu, ( char* )&plu_sic, sizeof( plu_sic ), NULL, 0, 0 )*/
		OPEN_TABLE(T_PLU, TT_ORIG,( char* )&plu, sizeof( plu ))== 0 ) {
		//SELECT_TABLE( T_PLU, TT_ORIG );
        //GET_EQUAL( ( char* )&nro );
		SET_WHERE("PLU_TECLA = '%i'",nro);
		RUN_QUERY(NO);
        if( FOUND2() ) {
			if( plu.cant_dig_ingresar == 0 ) {
				plu_ = plu.cod_especial;
        	} else {
				char plu_aux[10];
				int  rta_pedir = 0;

				memset( plu_aux, 0x00, sizeof( plu_aux ) );
				ExecuteSSAction( A_PEDIR, _SIMPLE, "PLU", "CODIGO", NULL, " " );
				rta_pedir = PEDIR_CADENA_NUMERICA( 1, 1, ( char ) plu.cant_dig_ingresar, plu_aux, NULL, 0, SI, NO, NULL );
				ExecuteAction( A_FIN_PEDIR );
				if( rta_pedir ) {
					rta_pedir = atoi( plu_aux );  //reutilizo para obtener codigo ingresado.
					plu_ = ( long )( ( plu.cod_especial * pow( 10, plu.cant_dig_ingresar ) ) + rta_pedir );
				}
			}
		}
        CLOSE_TABLE( T_PLU, TT_ORIG );
    }
    return ( plu_ );
}

