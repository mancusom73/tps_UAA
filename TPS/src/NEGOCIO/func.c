#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
//#include <browse.h>
#include <btrv.h>
#include <cr.h>
#include <dbrouter.h>
#include <func.h>
#include <m_cadena.h>
#include <pant.h>
#include <stringt.h>
#include <_cr1.h>
#include <aarch.h>
#include <db.h>
#include <deftables.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
struct _ind1_funcion
{
    char habilitada;
    short int lugar_uso;
    short int orden;
};
/*
 * INDICES
 * -------
 * 0 -> habilitada + uso + orden
 * 1 -> nom_funcion
 */
//static struct _funcion_sic *_funcion;
static struct _ind1_funcion ind;
static void GO_FUNCION( int modo );
/*****************************************************************************/
static int _elegir_funcion( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0;

    switch( comando ) {
        case _BUSCAR:
			memset(&_funcion,0,sizeof(struct _funcion_sic));
            MOSTRAR_CADENA( 8, 16, cadena );
            ind.orden = atoi( cadena );
			SET_WHERE(/*"CONCAT(HABILITADA,LUGAR_USO,ORDEN) >= CONCAT(%c,%i,%i)"*/
				"(HABILITADA = '%c' and LUGAR_USO = %i and ORDEN >= %i) or (HABILITADA = '%c' and LUGAR_USO > %i) or (HABILITADA > '%c')",
								ind.habilitada,
								ind.lugar_uso,
								ind.orden,
								ind.habilitada,
								ind.lugar_uso,
								ind.habilitada);
            //GET_GREATER_E( ( char* )&ind );
			SET_ORDER2("ORDEN");
			RUN_QUERY(NO);
            /*if( _funcion->habilitada != ind.habilitada || _funcion->lugar_uso != ind.lugar_uso ) {
                GO_FUNCION( BOTTOM );
            }*/
            break;
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[20];
				memset( xx, 0, sizeof( xx ) );
                #ifdef INVEL_L
                sprintf( xx, "%d", _funcion->orden );
                #else
                _itoa( _funcion.orden, xx, 10 );
                #endif
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, _funcion.nom_funcion );
            }
            #endif
            MOSTRAR_ENTERO( 8, 10 + dato, "999", _funcion.orden );
            MOSTRAR_CADENA( 11, 10 + dato, _funcion.nom_funcion );
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbEOF() || dbBOF();

			if( !rta
				&& ( _funcion.habilitada != ind.habilitada || _funcion.lugar_uso != ind.lugar_uso ) ) {
                /*if( dato > 0 ) {
                    GO_FUNCION( BOTTOM );
                }
                else {
                    GO_FUNCION( TOP );
                }*/
                rta = 1;
            }
            break;
        case _VALIDAR:
            if( dato >= '0' && dato <= '9' ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
            rta = 1;
            break;
		case _LIMIT:
            if( dato > 0 ) {
                SET_LIMIT( dato, SI );
            } else {
                SET_LIMIT( NO, NO );
            }
            break;
    }
    return ( rta );
}
/*****************************************************************************/
static void GO_FUNCION( int modo )
/*****************************************************************************/
{
	char ax[20];
    if( modo == TOP ) {
        ind.orden = 0;
		sprintf(ax,"ORDEN");
		SET_WHERE("ORDEN >= '%i'", ind.orden);
        //GET_GREATER_E( ( char* )&ind );
    }
    else if( modo == BOTTOM ) {
        ind.orden = 99;
		sprintf(ax,"%i - ORDEN",ind.orden);
		SET_WHERE("ORDEN <= '%i'", ind.orden);
        //GET_GREATER_E( ( char* )&ind );
        //SKIP( -1 );
    }
	SET_ORDER2(ax);
	//SET_WHERE("ORDEN >= '%i'", ind.orden);
	RUN_QUERY(NO);
}
/***************************************************************************/
int SELECCIONA_FUNCION( int lugar )
/***************************************************************************/
{
    int rta = 0, ok = 0, largo = 0;
    //struct _funcion_sic funcion;
    ind.habilitada = 'S';
    ind.lugar_uso = lugar;
    /*---------------------------- Abre el archivo ---------------------------*/
    //_funcion = &funcion;
    if(OPEN_TABLE(T_FUNCION, TT_ORIG, ( char* )&_funcion, sizeof( struct _funcion_sic ))== 0 ) {
        /*-------------------------- Prepara la pantalla -------------------------*/
		SELECT_TABLE( T_FUNCION, TT_ORIG );
		SET_ORDER2("HABILITADA, LUGAR_USO, ORDEN");
		RUN_QUERY(NO);
		GO2( BOTTOM );
		if( /*!BtrvEOF( )*/ !dbEOF() ) {
			char cad[20];
			memset( cad, 0, sizeof( cad ) );
			_itoa( _funcion.orden, cad, 10 );
			largo = strlen( cad );
		}
        //GO_FUNCION( TOP );
        ok = Consulta_elegir_funcion( _elegir_funcion, largo );
        if( ok ) {
            rta = _funcion.tecla_devuelta;
        }
        //CLOSE_DATABASE( T_FUNCION );
        CLOSE_TABLE( T_FUNCION, TT_ORIG );
    }
    return ( rta );
}

