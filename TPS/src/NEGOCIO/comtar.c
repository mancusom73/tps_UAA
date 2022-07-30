#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <btrv.h> 
#include <clave.h> 
#include <cmemory.h> 
#include <comtar.h> 
#include <cr.h>  
#include <dbrouter.h> 
#include <mstring.h> 
#include <pausa.h> 
#include <_cr1.h> 
#include <aarch.h>
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
struct _comtar
{
    BINARY status;
    BINARY marca;
    char nro_tarjeta[21];
    BINARY anulacion;
	BINARY fin;
};
/***********************************************************************/
void GRABAR_COMTAR( char marca, char *nro_tarjeta, char anulacion )
/***********************************************************************/
{
    struct _comtar comtar;
	
	memset( &comtar,  0, sizeof( struct _comtar ) );

    if( !VAR_TARJETAS_AUT_ON_LINE ) {
		if( OPEN_TABLE( T_COMTAR, TT_ORIG, ( char* )&comtar, sizeof( comtar ) ) == 0 ) {
            comtar.marca = marca + 1;
            strcpy( comtar.nro_tarjeta, nro_tarjeta );
            comtar.anulacion = anulacion;
            #ifdef INVEL_L
            //BEGIN_TRANSACTION();
            #endif
            INSERT2( NO );
            #ifdef INVEL_L
            //END_TRANSACTION();
            #endif
            CLOSE_TABLE( T_COMTAR, TT_ORIG );
        }
        else {
            MENSAJE_STRING( S_ERROR_GRABAR_COMTAR );
        }
    }
}
/***********************************************************************/
int BUSCAR_COMTAR( char marca, char *nro_tarjeta )
/***********************************************************************/
{
    struct
    {
        char marca;
        char nro_tarjeta[21];
    } indice;
    int rta = 0;
    struct _comtar comtar;
    indice.marca = marca;
    strncpy( indice.nro_tarjeta, nro_tarjeta, sizeof( indice.nro_tarjeta ) );
	if( OPEN_TABLE( T_COMTAR, TT_ORIG, ( char* )&comtar, sizeof( comtar ) ) == 0 ) {
		SET_WHERE( "MARCA = '%d' AND NRO_TARJETA = '%s'", indice.marca, indice.nro_tarjeta );
		RUN_QUERY( NO );
        while( marca == comtar.marca && strcmp( nro_tarjeta, comtar.nro_tarjeta ) == 0
            && !dbEOF() ) {
            rta = ( comtar.anulacion ) ? rta - 1 : rta + 1;
            SKIP2( 1 );
        }
    }
    CLOSE_TABLE( T_COMTAR, TT_ORIG );
    return ( rta );
}
/***********************************************************************/
int VERIFICAR_TARJETA_EN_COMTAR( char marca )
/***********************************************************************/
{
    int cant_comtar, rta = 1;
    char aux[60];
    cant_comtar = BUSCAR_COMTAR( marca, GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/ );
    if( cant_comtar >= 1 ) {
        if( cant_comtar >= TARJETAS_PERMITIDAS_EN_COMTAR ) {
            MENSAJE_STRING( S_TARJ_CON_COMPRA_NO_USAR );
            PAUSA( 72 );
            rta = 0;
        }
        else {
            MENSAJE_STRING( S_TARJ_CON_COMPRA_AUTORIZ );
            PAUSA( 72 );
            _snprintf(aux, sizeof(aux)-1, ST( S_TARJETA_REPETIDA___I____S_ ), marca, GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/ );
            rta = SOLICITAR_CLAVES( 60, 19, NO, SI, SI, NULL, aux, _F_SUP_TAR_REP, SI );
            if( rta && AUTORIZAR_TARJETAS_REPETIDAS ) {
                SET_MEMORY_CHAR( __var_tarjetas_pedir_auto, 1 );
            }
        }
    }
    return ( rta );
}

