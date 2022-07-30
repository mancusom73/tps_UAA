#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <actions.h>
#include <b_mens.h> 
#include <cajon.h> 
#include <cmemory.h> 
#include <cr.h>  
#include <dbrouter.h> 
#include <driver.h> 
#include <fiscal.h> 
#include <getch.h> 
#include <get_fech.h> 
#include <mstring.h> 
#include <omron.h> 
#include <pausa.h> 
#include <pinpad1.h> 
#include <print.h> 
#include <status.h> 
#include <tkt.h> 
#include <val.h> 
#include <val2.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/*extern char estado_cajon_tm300;*/
/*****************************************************************************/
int ABRIR_CAJON()
/*****************************************************************************/
{
    int rta = 0;
    if( TIPO_CAJON == CAJON_DRIVER ) {
        if( CAJON_CERRADO() ) {	
            rta = setData( ACTION_OPEN_DRAWER, NULL, 0 );
        }
    }else
		if( IMPRESORA_FISCAL ){
			int seguro=10;
			/*El cajon conectado a la impresora fiscal no se configura, 
			por lo que se realiza el pedido de apertura aunque no haya cajon con figurado!!!
			Se controla que el comando no de error o que si se realiza efectivamente la apertura.. 
			se devuelva el estado de cajon abierto
				*/
			do{
				rta = CONTROLA_LOS_ENVIOS_Y_REINTENTA(ACTION_FISCAL_OPEN_CASHDRAWER, NULL,0, 4 ,SI);
				if( GET_ESTADO_FISCAL(_FISCAL_cajon_abierto) )//aunque de error si verifica que esta abierto lo toma
					rta = 1;
			//el seguro es para no generar ciclo infinito por alguna rta no contemplada
			}while( rta != 1  && seguro-- );
		}//fin fiscal

    rta = 1;
    SET_MEMORY_CHAR( __nro_hubo_operaciones, 1 );
    if( NRO_Z_EMITIDA ) {
        SET_MEMORY_CHAR( __nro_z_emitida, 0 );
        SET_MEMORY_INT( __nro_fecha_operacion, GET_FECHA_DOS() );
    }
    return ( rta );
}
/*****************************************************************************/
void CERRAR_CAJON()
/*****************************************************************************/
{
    int seguir = 1, espera = 0, borrarMsg = 0;

    while( !IS_PRINTER() );

    if( !CAJON_CERRADO() ) {
        MENSAJE_STRING( S_CIERRE_EL_CAJON );
        borrarMsg = 1;
    }
    else {
        seguir = 0;
    }

    while( seguir ) {
        if( CAJON_CERRADO() ) {
            seguir = 0;
        }
        else {
            //PAUSA( 4 );
            SLEEP( 50 );
        }
    }

    if(borrarMsg == 1)
        BORRAR_MENSAJE();
}
/*****************************************************************************/
int CAJON_CERRADO()
/*****************************************************************************/
{
    int rta = 1;
    if( TIPO_CAJON == CAJON_DRIVER ) {
        getData( ACTION_DRAWER_OPENED, ( char* )&rta, 0 );
        rta = !rta;
	}
	if( IMPRESORA_FISCAL && _CAJON_POR_IMPRESORA_ABIERTO ){
		COMANDO_LEER_STATUS_PRINTER_FISCAL(NO, (char)&rta);
		 rta =!GET_ESTADO_FISCAL( _FISCAL_cajon_abierto );
	}
    return ( rta );
}
/*****************************************************************************/
int INIT_CAJON()
/******************************************************************************/
{
    int error = 0;
    if( TIPO_CAJON == CAJON_DRIVER ) {
        DRIVER( _DRV_cajon, _CAJ_inicializacion, 0, &error, sizeof( error ) );
    }
    return ( !error );
}

