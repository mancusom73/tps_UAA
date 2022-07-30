#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include "ini.h"
#include <tpv_dat.h>
#include <aarch.h>
#include <abort.h>
#include <archivos.h>
#include <btrv.h>
#include <carga.h>
#include <cio.h>
#include <mensaje.h>
#ifdef INVEL_W
#include <io.h>
#else
#include <sys/io.h>
#endif
#include <cr.h>
#include <dbrouter.h>
#include <dd.h>
#include <tkt.h>
#include <_cr1.h>
#include <deftables.h>
#include <archivos.h>
#include <cio.h>
#include <db.h>
#include <log.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Rutinas para cargar datos fijos
 * ------------------------------------------------------------------------------
 */
#ifdef INVEL_L
#include <sys/io.h>
#include <sys/stat.h>
#include <unistd.h>
#else
#include <sys/stat.h>
#endif
void SN_TO_BIN( char *buffer, int cantidad );
struct _medios medios_sic;
static int cod_iva_inicial;
/*****************************************************************************/
void CARGAR_CONFIG()
/*****************************************************************************/
{	
	char mensaje[128];
	//int _borrar_msj = 0;

	if( OPEN_TABLE( T_TPVCONFIG, TT_ORIG, ( char* )&config, sizeof( struct _tpvconfig ) )== 0 ) {
		SET_WHERE("");
		RUN_QUERY(NO);
	}

	while(config.sucursal != ConfIni.sucursal){ //forray sacar, es para revisar si cambia la sucursal en mexico
        char strTmp[512];
		memset(strTmp, 0, sizeof(strTmp));

		if( ConfIni.odbcServer[0] != 0 )
			MENSAJE("NUMERO DE SUCURSAL INCORRECTO, PIDIENDO CONFIG DAT....");
        
		_snprintf(strTmp,512,"NUMERO DE SUCURSAL EN CONFIG: %i",config.sucursal);
        strTmp[511] = '\0';
        GRABAR_LOG_SISTEMA(strTmp,LOG_VENTAS,2);
        _snprintf(strTmp,512,"NUMERO DE SUCURSAL EN INI: %i",ConfIni.sucursal);
        strTmp[511] = '\0';
        GRABAR_LOG_SISTEMA(strTmp,LOG_VENTAS,2);
		if( ConfIni.odbcServer[0] == 0 ) {
			MENSAJE_CON_PAUSA_CAJA("NUMERO DE SUCURSAL INCORRECTO", 40);
			MENSAJE_CON_PAUSA_CAJA(strTmp, 40);
			BORRAR_MENSAJE( );
		}
		//_borrar_msj = 1;
		if( OBTENER_TABLA_SERVIDOR( T_TPVCONFIG, TT_ORIG, "TPVCONFIG", "", SI, NULL ) ) {
			memset( mensaje, 0, sizeof( mensaje ) );
			_snprintf(mensaje, sizeof(mensaje)-1,"TPVCONFIG - %s", ST( S_ERROR_TABLA_NO_OBTENIDA ) );
			MENSAJE_SIN_SONIDO( mensaje,NO );
			GRABAR_LOG_BD(mensaje,SI ,LOG_VENTAS,1);
		} else { 
			if( OPENED( T_TPVCONFIG, TT_ORIG ) ) {
				CLOSE_TABLE( T_TPVCONFIG, TT_ORIG );
			}
			if( OPEN_TABLE( T_TPVCONFIG, TT_ORIG, ( char* )&config, sizeof( struct _tpvconfig ) )== 0 ) {
				SET_WHERE("");
				RUN_QUERY(NO);
			}
		}
    }
	//if( _borrar_msj )
		BORRAR_MENSAJE( );
	if( OPENED( T_TPVCONFIG, TT_ORIG ) ) {
		CLOSE_TABLE( T_TPVCONFIG, TT_ORIG );
	}

}
/*****************************************************************************/
int CARGAR_MEDIOS_DE_PAGO()
/*****************************************************************************/
{
    int h, rta = 0;
    //struct _medios medios_sic;
	/*int  area_ant, tipo_ant;

	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();*/

    if( medios == NULL ) {
        medios = (struct _medios *)calloc( 10, sizeof( struct _medios ) );
    }
    memset( medios, 0, sizeof( struct _medios ) * 10 );
    if( /*USE_DB( AREA_AUX, _MEDIOS_SIC, ( char* )&medios_sic, sizeof( medios_sic ), NULL, 0, 0 )*/
		OPEN_TABLE(T_MEDIOS, TT_ORIG ,( char* )&medios_sic, sizeof( medios_sic )) == 0 ) {
			SELECT_TABLE( T_MEDIOS, TT_ORIG );
			SET_WHERE("");
			SET_ORDER2("COD_MEDIO");
			RUN_QUERY(NO);
        for( h = 0;!dbEOF() && h < 10;h++ ) {
            SN_TO_BIN( &medios_sic.vuelto, 8 );
            SN_TO_BIN( &medios_sic.credito, 3 );
            SN_TO_BIN( &medios_sic.hab_fac_rem, 1 );
            SN_TO_BIN( &medios_sic.hab_nc, 1 );
            SN_TO_BIN( &medios_sic.moneda_extranjera, 1 );
            memcpy( &medios[h], &medios_sic, sizeof( medios_sic ) );
            if( medios[h].efectivo ) {
                _medio_efectivo = h;
            }
            if( medios[h].cta_cte ) {
                _medio_ctacte = h;
            }
            if( medios[h].cheque ) {
                _medio_cheque = h;
            }
            if( medios[h].tarjeta ) {
                if( _medio_tarjeta == -1 ) {
                    _medio_tarjeta = h;
                }
                else {
                    _medio_tarjeta2 = h;
                }
            }
            if( medios[h].ticket_compra ) {
                _medio_ticket_compra = h;
            }
            if( medios[h].mutual ) {
                _medio_mutual = h;
            }
            if( medios[h].moneda_extranjera ) {
                _medio_moneda_extranjera = h;
            }
            SKIP2( 1 );
        }
        CLOSE_TABLE( T_MEDIOS, TT_ORIG );
		//SELECT_TABLE( area_ant, tipo_ant );
        rta = 1;
    }

    return ( rta );
}
/*****************************************************************************/
void SN_TO_BIN( char *buffer, int cantidad )
/*****************************************************************************/
{
    int h;
    for( h = 0;h < cantidad;h++ ) {
        if( buffer[h] == 'S' ) {
            buffer[h] = 1;
        }
        if( buffer[h] == 'N' ) {
            buffer[h] = 0;
        }
    }
}
/*****************************************************************************/
int CARGAR_IMPUESTOS()
/*****************************************************************************/
{
    int h, rta = 0;
    struct _impuestos impuestos_sic;
	/*int  area_ant, tipo_ant;

	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();*/

    if( impuestos == NULL ) {
        impuestos = ( struct _impuestos *)calloc( 40, sizeof( struct _impuestos ) );
    }
    memset( impuestos, 0, sizeof( struct _impuestos ) * 40 );
    if( /*USE_DB( AREA_IMPUESTO, _IMPUESTO_SIC, ( char* )&impuestos_sic, sizeof( impuestos_sic ),
                NULL, 0, 0 )*/
		OPEN_TABLE(T_IMPUESTO, TT_ORIG , ( char* )&impuestos_sic, sizeof( impuestos_sic ))== 0 ) {
			SELECT_TABLE( T_IMPUESTO, TT_ORIG );
			SET_WHERE("");
			RUN_QUERY(NO);
        for( h = 1;!dbEOF() && h < 40;h++ ) {
            memcpy( &impuestos[impuestos_sic.cod_impuesto], &impuestos_sic,
                    sizeof( impuestos_sic ) );
            SKIP2( 1 );
        }
        CLOSE_TABLE( T_IMPUESTO, TT_ORIG );
        rta = 1;
    }
	//SELECT_TABLE( area_ant, tipo_ant );
    return ( rta );
}
/*****************************************************************************/
int CARGAR_IVA()
/*****************************************************************************/
{
    int h, rta = 0;
    struct _articulo_iva iva_sic;
    int cantidad_iva = 10;
    //char nombre[200];
	/*int  area_ant, tipo_ant;

	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();*/

	memset( iva, 0, sizeof( iva ) );
    //NOM_ARCHIVO( _IVA_SIC, nombre );
    //if( access( nombre, 0 ) == 0 ) {
       // if( USE_DB( AREA_IVA, _IVA_SIC, ( char* )&iva_sic, sizeof( iva_sic ), NULL, 0, 0 ) == 0 ) {
		if( OPEN_TABLE( T_ARTICULO_IVA, TT_ORIG, ( char* )&iva_sic, sizeof( struct _articulo_iva ) ) == 0 ) {
			SELECT_TABLE(T_ARTICULO_IVA, TT_ORIG);
			SET_WHERE("");
			RUN_QUERY(NO);
			cod_iva_inicial = iva_sic.cod_iva;
            for( h = 0;!dbEOF() && h < cantidad_iva;h++ ) {
                if( iva_sic.cod_iva >= 0 && iva_sic.cod_iva < cantidad_iva ) {
                    memcpy( &iva[( int )iva_sic.cod_iva], &iva_sic, sizeof( iva_sic ) );
                    //SKIP( 1 );
					SKIP2( 1 );
                }
                else {
                    ABORT( ST( S_CODIGO_DE_IVA_NO_PERMITIDO ) );
                }
            }
            //CLOSE_DATABASE( AREA_IVA );
			CLOSE_TABLE( T_ARTICULO_IVA, TT_ORIG );
            rta = 1;
        }
   // }
	//SELECT_TABLE( area_ant, tipo_ant );
    return ( rta );
}
/*****************************************************************************/
int CARGAR_DPTOS()
/*****************************************************************************/
{
    int rta = 0;
    struct _departamentos depto;
	/*int  area_ant, tipo_ant;

	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();*/

    if( /*USE_DB( AREA_AUX, _DPTO_SIC, ( char* )&depto, sizeof( depto ), NULL, 0, 0 )*/
		OPEN_TABLE(T_DPTO, TT_ORIG ,( char* )&depto,sizeof( depto ))== 0 ) {
		SELECT_TABLE( T_DPTO, TT_ORIG );
		RUN_QUERY(NO);
        if( RECCOUNT2(NULL,NULL) > 1 ) {
			SET_WHERE("");
			RUN_QUERY(NO);
            while( !dbEOF() ) {
                if( depto.cod_depto >= 0 && depto.cod_depto < CANT_DEPTOS ) {
                    ATRIB_DESC_DPTO[depto.cod_depto] = depto.atrib_descuentos;
                    ATRIB_DISCR_DPTO[depto.cod_depto] = depto.reportar;
                }
                SKIP2( 1 );
            }
            CLOSE_TABLE( T_DPTO, TT_ORIG );
            rta = 1;
        }
    }
	//SELECT_TABLE( area_ant, tipo_ant );
    return ( rta );
}
/*****************************************************************************/
int CARGAR_TIPOS_DE_ENVASES()
/*****************************************************************************/
{
    int h, rta = 0;
    struct _tipo_envase tipo_envase_sic;
	/*int  area_ant, tipo_ant;

	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();*/

	memset( tenvase, 0, 10 * ( sizeof( struct _tipo_envase ) ));
    //if( USE_DB( AREA_ENVASES, _TENVASE_SIC, ( char* )&tipo_envase_sic, sizeof( tipo_envase_sic ),
    //            NULL, 0, 0 ) == 0 ) {
	if( !OPEN_TABLE( T_TENVASE, TT_ORIG, ( char* )&tipo_envase_sic, sizeof( struct _tipo_envase ) ) == 0 ) {
		SELECT_TABLE( T_TENVASE, TT_ORIG );
		GRABAR_LOG_SISTEMA("ENTRE EN CARGA ENVASES",LOG_VENTAS,4);
		SET_WHERE("");
		RUN_QUERY(NO);
        for( h = 0;!dbEOF() && h < 10;h++ ) {
            memcpy( &tenvase[h], &tipo_envase_sic, sizeof( struct _tipo_envase ) );
            SKIP2( 1 );
        }
        //CLOSE_DATABASE( AREA_ENVASES );
		CLOSE_TABLE( T_TENVASE, TT_ORIG );
        rta = 1;
    }
	//GRABAR_LOG_SISTEMA("SALI CARGA ENVASES",LOG_VENTAS,2);
	//SELECT_TABLE( area_ant, tipo_ant );
    return ( rta );
}
/*****************************************************************************/
int CARGAR_COBROS_ESPECIALES( void )
/*****************************************************************************/
{
    int reabrir = 0, rta = 0;
    /*if( USED( AREA_COBROS ) ) {
        reabrir = 1;
    }
    CLOSE_DATABASE( AREA_COBROS );*/
    if( /*USE_DB( AREA_COBROS, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 )*/
		OPEN_TABLE(T_COBROS, TT_ORIG ,( char* )&cobros, sizeof( cobros ))== 0 ) {
			//SELECT_TABLE( T_COBROS, TT_ORIG );
        /*if( !reabrir ) {
            CLOSE_DATABASE( AREA_COBROS );
        }*/
        rta = 1;
    }
    return ( rta );
}

/*****************************************************************************/
int OBTENER_COD_IVA_INICIAL( void )
/*****************************************************************************/
{
	return ( cod_iva_inicial );
}