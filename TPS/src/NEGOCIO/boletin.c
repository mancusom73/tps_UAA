#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <aarch.h>
#include <archivos.h>
#include <boletin.h>
#include <btrv.h>
#include <cmemory.h>
#include <cr.h>
#include <cr_disco.h>
#include <dbrouter.h>
#include <getch.h>
#include <get_fech.h>
#include <malloc.h>
#include <mstring.h>
#include <m_cadena.h>
#include <pant.h>
#include <round.h>
#include <_cr1.h>
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#define NOVEDAD_BOLETIN     0
#define BORRAR_BOLETIN     1
struct _boletin_protectivo boletin_prot,boletin_ticket;
int t_boletin_tipo = -1;
/***************************************************************************/
int BUSCAR_MEDIO_EN_BOLETIN( char tipo_boletin, int nro_archivo, char *nro_bono, double *importe,
                             int concatenar_importe, int verificar_importe, int local )
/***************************************************************************/
{
    int encontrado = 0, rta = 1, error = 0, cant;
    // struct _boletin_protectivo boletin_prot, boletin_ticket;
    if( MODO_DEVOLUCION || RAM_NOTA_CR ) {
        tipo_boletin = ( tipo_boletin == BOLETIN_POSITIVO ) ? BOLETIN_NEGATIVO : BOLETIN_POSITIVO;
    }
    if( tipo_boletin == BOLETIN_POS_CON_IMPORTE && !RAM_ANULACION ) {
        /*if( USE_DB( AREA_BOLETIN, _BOL_TIC_TMP, ( char* )&boletin_ticket, sizeof( boletin_ticket ),
                    NULL, 0, 0 ) == 0 ) {*/
        if( OPEN_TABLE( T_BOLETIN, TT_ORIG, ( char* )&boletin_ticket,
			sizeof( boletin_ticket ) ) == 0 ) {
            t_boletin_tipo = TT_ORIG;
            encontrado = EXISTE_BOLETIN_TCOMPRA_MUTUALES( nro_archivo, nro_bono, *importe,
                                                          concatenar_importe );
            if( encontrado ) {
                MENSAJE_STRING( S_ESE_BONO_YA_HA_SIDO_UTILIZADO );
                rta = 0;
            }
        }
		//CLOSE_DATABASE( AREA_BOLETIN );
        CLOSE_TABLE( T_BOLETIN, TT_ORIG ); //es TT_ORIG
    }
    /*---------------------------- Abre el archivo ---------------------------*/
    if( rta == 1 ) {
        if( local ) {
            //if( !USED( AREA_BOLETIN ) ) {
			if( !OPENED( T_BOLETIN, TT_ORIG ) ) {
                /*error = USE_DB( AREA_BOLETIN, _BOLETIN_SIC, ( char* )&boletin_prot,
                                sizeof( boletin_prot ), NULL, 0, 0 );*/
                error = OPEN_TABLE( T_BOLETIN, TT_ORIG, ( char* )&boletin_prot,
                                sizeof( boletin_prot ) );
                t_boletin_tipo = TT_ORIG;
                cant = 0;
            }
            if( error ) {
                MENSAJE_STRING( S_ERROR_AL_ABRIR_BOLETIN );
            }
        }
        else {
            if( OBTENER_BOLETIN_REMOTO( nro_archivo, nro_bono, *importe, concatenar_importe ) ) {
                /*error = USE_DB( AREA_BOLETIN, _BOLETIN_TMP, ( char* )&boletin_prot,
                                sizeof( boletin_prot ), NULL, 0, 0 );*/
				error = OPEN_TABLE( T_BOLETIN, TT_TEMP, ( char* )&boletin_prot,
                                sizeof( boletin_prot ) );
                t_boletin_tipo = TT_TEMP;
            }
            else {
                error = 1;
                rta = -1;
            }
        }
        if( !error ) {
            encontrado = EXISTE_BOLETIN_TCOMPRA_MUTUALES( nro_archivo, nro_bono, *importe,
                                                          concatenar_importe );
            //0:ignorar; 1:positivo; 2:negativo
            if( encontrado && tipo_boletin == BOLETIN_NEGATIVO ) {
                MENSAJE_STRING( S_MEDIO_EN_BOLETIN );
                rta = 0;
            }
            if( !encontrado
             && ( tipo_boletin == BOLETIN_POSITIVO || tipo_boletin == BOLETIN_POS_CON_IMPORTE
			   || tipo_boletin == BOLETIN_POS_CON_IMP_SIN_AC || tipo_boletin == BOLETIN_POS_SIN_ACTUALIZ) ) {
                rta = 0;
                MENSAJE_STRING( S_NO_SE_ENCUENTRA_EN_BOLETIN );
            }
            if( rta && ( tipo_boletin == BOLETIN_POSITIVO ) ) {
                if( verificar_importe && importe
                 && ( ROUND( *importe, ENTEROS, DECIMALES )
                   != ROUND( boletin_prot.importe, ENTEROS, DECIMALES ) ) ) {
                    rta = 0;
                    MENSAJE_STRING( S_EL_MONTO_INDICADO_NO_CORRESPONDE_CON_EL_BONO );
                }
            }
            if( rta
             && ( tipo_boletin == BOLETIN_POSITIVO || tipo_boletin == BOLETIN_POS_CON_IMPORTE )
             && boletin_prot.utilizado ) {
                rta = 0;
                MENSAJE_STRING( S_ESE_BONO_YA_HA_SIDO_UTILIZADO );
            }
            /*if( rta && ( tipo_boletin == BOLETIN_POSITIVO || tipo_boletin == BOLETIN_POS_CON_IMPORTE )
            //02/12/2004-- fabiana -- se comenta para poder igualar la base de datos
            //&& boletin_prot.fecha_vigencia < _GET_FECHA_DOS( )
            ) {
                rta = 0;
                MENSAJE_STRING( S_EL_BONO_HA_VENCIDO );
            }*/
            if( rta
             && ( tipo_boletin == BOLETIN_POS_CON_IMPORTE
               || tipo_boletin == BOLETIN_POS_CON_IMP_SIN_AC ) ) {
                *importe = boletin_prot.importe;
            }
        }
        else {
            MENSAJE_STRING( S_ERROR_AL_ABRIR_BOLETIN );
        }
        //CLOSE_DATABASE( AREA_BOLETIN );
		CLOSE_TABLE( T_BOLETIN, t_boletin_tipo );
    }
    if( rta == 1 && tipo_boletin == BOLETIN_POS_CON_IMPORTE ) {
        /*if( USE_DB( AREA_BOLETIN, _BOL_TIC_TMP, ( char* )&boletin_ticket, sizeof( boletin_ticket ),
                    NULL, 0, 0 ) == 0 ) {*/
		if( OPEN_TABLE( T_BOLETIN, TT_ORIG, ( char* )&boletin_ticket, sizeof( boletin_ticket ) ) == 0 ) {
            t_boletin_tipo = TT_ORIG;
            if( RAM_ANULACION ) {
                if( EXISTE_BOLETIN_TCOMPRA_MUTUALES( nro_archivo, nro_bono, *importe,
                                                     concatenar_importe ) ) {
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
                    //_DELETE();
					DELETE2();
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                }
            }
            else {
                memcpy( &boletin_ticket, &boletin_prot, sizeof( boletin_ticket ) );
                #ifdef INVEL_L
                //BEGIN_TRANSACTION();
                #endif
                //INSERT();
				INSERT2( NO );
                #ifdef INVEL_L
                //END_TRANSACTION();
                #endif
            }
            //CLOSE_DATABASE( AREA_BOLETIN );
			CLOSE_TABLE( T_BOLETIN, t_boletin_tipo );
        }
    }
    return rta;
}
/*****************************************************************************/
int PIDE_NRO_BONO( char *nro )
/*****************************************************************************/
{
    int rta = 1, scanner;
    memset( nro, ' ', 20 );
    MOSTRAR_CADENA( 37 - ancho_panel / 2, 21, ST( S__NRO_BONO_ ) );
    scanner = _PERMITIR_SCANNER;
    _PERMITIR_SCANNER = SI;
    rta = PEDIR_GRAN_CADENA( ( char )( 23 - ancho_panel / 2 ), ( char )22, ( char )11, nro, NULL );
    _PERMITIR_SCANNER = scanner;
    return ( rta );
}
/***********************************************************************/
int EXISTE_BOLETIN_TCOMPRA_MUTUALES( int nro_archivo, char *nro_bono, double importe,
                                     int concatenar )
/***********************************************************************/
{
    int encontrado;
    struct
    {
        INT16 nro_archivo;
        char nro_bono[20];
    } indice;
    //SELECT( AREA_BOLETIN );
	SELECT_TABLE( T_BOLETIN, t_boletin_tipo );
	//SET_ORDER( 1 );
    SET_ORDER2( "NRO_ARCHIVO, NRO_BONO" );
    indice.nro_archivo = (short)nro_archivo;
    CONTRUYE_MASCARA( nro_bono, importe, concatenar, indice.nro_bono, sizeof( indice.nro_bono ) );
    //GET_EQUAL( ( char* )&indice );
	SET_WHERE( "NRO_ARCHIVO = '%i' AND NRO_BONO = '%s'", indice.nro_archivo, indice.nro_bono );
	RUN_QUERY( NO );
    //encontrado = FOUND();
	encontrado = FOUND2();
    return ( encontrado );
}
/***********************************************************************/
int ACTUALIZA_BOLETINES_TCOMPRA_MUTUALES( int *registros )
/***********************************************************************/
{
    int error = 0, rtabtr = 0, ok, nov_tipo, existe = 0;
    unsigned long cant = 0L;

	struct _boletin_novedades
    {
        BINARY status;
        unsigned long novedad;                    //autoincremental
        int nro_archivo;
        char nro_bono[20];
        char novedadregistro;
        float importe;
        BINARY fin;
    }        *boletin_nov;
    //if( !USED( AREA_BOLETIN ) ) {
	if( !OPENED( T_BOLETIN, TT_ORIG ) ) {
        ok = ABRIR_ARCHIVO_BOLETIN_TCOMPRA_MUTUALES();
        t_boletin_tipo = TT_ORIG;
    }
    else {
        ok = 1;
    }
    if( !ok ) {
        return ( 1 );
    }
    boletin_nov = ( struct _boletin_novedades *)MALLOC( sizeof( struct _boletin_novedades ) );
    if( boletin_nov == NULL ) {
        return ( 1 );
    }
    if( !OPEN_TABLE( T_BOLETIN_NOV, TT_ORIG, ( char* )boletin_nov, sizeof( struct _boletin_novedades ) ) ) {
        if( registros ) {
			*registros = RECCOUNT2( NULL, NULL );
        }
		while( !dbEOF() && ( long )boletin_nov->novedad <= NRO_ULTIMA_NOVEDAD_BOLETIN ) {
			SKIP2( 1 );
        }
		while( !dbEOF() && !rtabtr && !error ) {
            cant++;
            if( KBHIT() && cant > ( unsigned long ) CANTIDAD_MINIMA_ALTAS ) {
                if( GETCH() == 27 ) {
                    error = 99;
                }
            }
            nov_tipo = boletin_nov->novedadregistro;
            boletin_nov->novedadregistro = 0;
			SELECT_TABLE( T_BOLETIN, TT_ORIG );
            switch( nov_tipo ) {
                case NOVEDAD_BOLETIN:
                    if( EXISTE_BOLETIN_TCOMPRA_MUTUALES( boletin_nov->nro_archivo,
                                                         boletin_nov->nro_bono,
                                                         boletin_nov->importe, NO ) ) {
                        existe = 1;
                    }
                    memset( &boletin_prot, 0, sizeof( struct _boletin_protectivo ) );
                    boletin_prot.nro_archivo = boletin_nov->nro_archivo;
                    memcpy( boletin_prot.nro_bono, boletin_nov->nro_bono, 20 );
                    if( boletin_nov->importe > 1.00 ) {
                        boletin_prot.importe = boletin_nov->importe;
                    }
                    boletin_prot.utilizado = 0;
                    if( !existe ) {
						rtabtr = INSERT2( NO );
                    }
                    else {
						rtabtr = UPDATE2();
                    }
                    break;
                case BORRAR_BOLETIN:
                    if( EXISTE_BOLETIN_TCOMPRA_MUTUALES( boletin_nov->nro_archivo,
                                                         boletin_nov->nro_bono,
                                                         boletin_nov->importe, NO ) ) {
						rtabtr = DELETE2();
                    }
                    break;
                default:
                    rtabtr = -1;
                    MENSAJE_STRING( S_NOVEDAD_DE_BOLETIN_NO_RECONOCIDA );
                    break;
            }
			SELECT_TABLE( T_BOLETIN_NOV, TT_ORIG );
            if( !rtabtr ) {
                SET_MEMORY_LONG( __nro_ultima_novedad_boletin, boletin_nov->novedad );
            }
			SKIP2( 1 );
        }
		CLOSE_TABLE( T_BOLETIN_NOV, TT_ORIG );
        if( !error ) {
            error = rtabtr;
        }
    }
    else {
        error = 99;
    }
	CLOSE_TABLE( T_BOLETIN, TT_ORIG );
    FREEMEM( ( char* )boletin_nov );
    return ( error );
}
/***********************************************************************/
int OBTENER_BOLETIN_REMOTO( int nro_archivo, char *nro_bono, double importe, int concatenar )
/***********************************************************************/
{
    char _where[80],mascara[20]/*,nom_bol[30]*/;
    int rta = 0;
    CONTRUYE_MASCARA( nro_bono, importe, concatenar, mascara, sizeof( mascara ) );
    _snprintf(_where, sizeof(_where)-1, "nro_archivo = %i AND nro_bono = '%s'", nro_archivo, mascara );
    if( !OBTENER_TABLA_SERVIDOR( T_BOLETIN, TT_TEMP, "BOLETINES_PROTECTIVO", _where, SI, NULL ) ){
        rta = 1;
    }
    return ( rta );
}
/***********************************************************************/
void CONTRUYE_MASCARA( char *nro_bono, double importe, int concatenar, char *dest, int size )
/***********************************************************************/
{
    int i;
    memset( dest, 0, size );
    if( concatenar ) {
        _snprintf(dest, sizeof(dest)-1, "%li%s", ( long )ROUND( importe, 8, 0 ), nro_bono );
    }
    else {
        strncpy( dest, nro_bono, size );
    }
    for( i = strlen( dest ) - 1;i && dest[i] == ' ';i-- ) {
        dest[i] = 0;
    }
}
/***********************************************************************/
void INICIALIZA_BOLETIN_TICKET()
/***********************************************************************/
{
	int area_ant, tipo_ant;

    area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

	if( !OPENED( T_BOLETIN, TT_ORIG ) ) {
        if( OPEN_TABLE( T_BOLETIN, TT_ORIG, ( char* )&boletin_ticket, sizeof( boletin_ticket ) ) == 0 ) {
            t_boletin_tipo = TT_ORIG;
			SET_WHERE("");
			RUN_QUERY(NO);
        	GO2( TOP );
			//SET_WHERE("");
			//RUN_QUERY(NO);
			while( !dbEOF() ) {
				DELETE2();
				//GO2( TOP );
         }
			CLOSE_TABLE( T_BOLETIN, TT_ORIG );
        }
    }
	SELECT_TABLE(area_ant, tipo_ant);

}

