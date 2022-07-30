#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <archivos.h>
#include <b_mens.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <dd.h> 
#include <eventos.h> 
#include <log.h> 
#include <mensaje.h> 
#include <modo_eje.h> 
#include <m_cadena.h> 
#include <print.h> 
#include <rnv.h> 
#include <stack.h> 
#include <stringt.h> 
#include <tkt.h> 
#include <_cr1.h>
#include <DefTables.h> 
#include <path.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <aarch.h>
#if defined(INVEL_W) || defined(INVEL_L)
#include <actions.h>
#include <typeacti.h>
#endif
#ifdef COMPILAR_MODO_ENTRENAMIENTO
void MOSTRAR_MODO_ENTRENAMIENTO_EN_PANTALLA( int );
#endif
void SETEAR_MODO_EJECUCION( int modo );
char modo_memoria_anterior;
//static int modo_ent_on = 0;
static int modo_ant = -1;
extern    char drivevirtual;
extern int DICCIO_CARGADO;
#define MODO_MEMORIA_ANTERIOR_FLASH   1
#define MODO_MEMORIA_ANTERIOR_ARCHIVO 2
#ifdef COMPILAR_MODO_ENTRENAMIENTO
/*****************************************************************************/
int SET_ENTRENAMIENTO_ON()
/*****************************************************************************/
{
    int iOk = 1;
	char nom_rnv[20];
	char nom_entrenam[25];

	memset( nom_rnv, 0, sizeof( nom_rnv ) );
	memset( nom_entrenam, 0, sizeof( nom_entrenam ) );
    if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
        return 1;
    }
    MENSAJE( ST( S_PREPARANDO_MODO_ENTRENAMIENTO_ ) );
    STACK_INIT_DATOS();
	DICCIO_CARGADO = NO;
    if( _RNV_VIRTUAL ) {
        modo_memoria_anterior = MODO_MEMORIA_ANTERIOR_ARCHIVO;
        CLOSE_RNV_VIRTUAL();
		_snprintf(nom_rnv, sizeof(nom_rnv)-1, "%c:%s", drivevirtual, RNV_DAT );
		_snprintf(nom_entrenam, sizeof(nom_entrenam)-1, "%c:%s", drivevirtual, ENTRENAM_DAT );
		COPIAR_ARCHIVO( nom_rnv, nom_entrenam, __LINE__, __FILE__ );
		DD_CLOSE();    
    }
    else {
        // Vaciar la Memoria de la rnv en el archivo entrenam.dat
        modo_memoria_anterior = MODO_MEMORIA_ANTERIOR_FLASH;
        GRABAR_RNV_EN_ARCHIVO( "entrenam.dat" );
        DD_CLOSE();
    }
    _RNV_VIRTUAL = SI;
	OPEN_ARCHIVO_RNV_VIRTUAL( ENTRENAM_DAT );
    DD_INIT();
	DICCIO_CARGADO = SI;
    STACK_INIT();
    STACK_INIT_DATOS();
    CERRAR_ARCHIVOS_EVENTOS();

    //if( iOk )
    //  iOk = !COPY( _TICKET_BTR, _TICKET_BTR_ENTR );
    /*if( iOk ) {
        iOk = !COPY( _E_TICKET_BTR, _E_TICKET_BTR_ENTR );
    }*/
    /*if( iOk )
        iOk = !COPY( _PAGO_BTR, _PAGO_BTR_ENTR );
    if( iOk )
        iOk = !COPY( _VARIOS_BTR, _VARIOS_BTR_ENTR );
    if( iOk )
        iOk = !COPY( _CLI_CHEQ_SIC, _CLI_CHEQ_SIC_ENTR );
    // COPIA FACTURAS
    if( iOk )
        iOk = !COPY( _REMITO_MEM, _REMITO_MEM_ENTR );
    if( iOk )
    iOk = !COPY( _FACTU_MEM, _FACTU_MEM_ENTR );*/
    VACIAR_ARCHIVOS_EVENTOS( TT_ENTR ); //vacia los eventos de entrenamiento
    iOk = 1;
    if( iOk ) {
        SETEAR_MODO_EJECUCION( ENTRENAMIENTO );
    }
    ABRIR_ARCHIVOS_EVENTOS();
    MOSTRAR_MODO_ENTRENAMIENTO_EN_PANTALLA( 1 );
    BORRAR_MENSAJE();
    return iOk;
}
/*****************************************************************************/
int SET_ENTRENAMIENTO_OFF()
/*****************************************************************************/
{
    int ok = 1;
    if( RAM_MODO_EJECUCION != ENTRENAMIENTO ) {
        return 1;
    }
    DICCIO_CARGADO = NO;
    DD_CLOSE();
    CLOSE_RNV_VIRTUAL();
    switch( modo_memoria_anterior ) {
        case MODO_MEMORIA_ANTERIOR_ARCHIVO:
            OPEN_RNV_VIRTUAL();
            break;
        case MODO_MEMORIA_ANTERIOR_FLASH:
            _RNV_VIRTUAL = NO;
            break;
    };
    DD_INIT();
	DICCIO_CARGADO = SI;
    STACK_INIT();
    STACK_INIT_DATOS();
    CERRAR_ARCHIVOS_EVENTOS();
    SETEAR_MODO_EJECUCION( OPERACION );
    ABRIR_ARCHIVOS_EVENTOS();
    modo_memoria_anterior = 0;
    MOSTRAR_MODO_ENTRENAMIENTO_EN_PANTALLA( 1 );
    return ok;
}
/*****************************************************************************/
void MOSTRAR_MODO_ENTRENAMIENTO_EN_PANTALLA( int cajero_operando )
/*****************************************************************************/
{
    #ifdef COMPILAR_MODO_ENTRENAMIENTO
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    char *mensaje_modo_ent = "************ M O D O    E N T R E N A M I E N T O ************";
    char *borra_mensaje = "                                                              ";
    int atr_ant = _ATRIBUTO;
    if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
        _ATRIBUTO = 15 + 128;
        if( cajero_operando ) {
            MOSTRAR_CADENA( 11 - ancho_panel / 2, 6, mensaje_modo_ent );
        }
        else {
            MOSTRAR_CADENA( 11 - ancho_panel / 2, 23, mensaje_modo_ent );
        }
    }
    else {
        _ATRIBUTO = 7;
        if( cajero_operando ) {
            MOSTRAR_CADENA( 11 - ancho_panel / 2, 6, borra_mensaje );
        }
        else {
            MOSTRAR_CADENA( 11 - ancho_panel / 2, 23, borra_mensaje );
        }
    }
    _ATRIBUTO = atr_ant;
}
#endif
#endif
}
#endif
/*****************************************************************************/
void SETEAR_MODO_EJECUCION( int modo )
/*****************************************************************************/
{
    char *cmodo[] = {"OPERACION","ENTRENAMIENTO","INVENTARIO","ETIQUETAS","TRANSFERENCIAS","CAMBIO DE MEDIO","COBROS"," vacio "};
    char aux[25];
    int modo_aux;
    char cad_modo[15];
    SET_MEMORY_CHAR( __ram_modo_ejecucion, modo );
    if( RAM_MODO_EJECUCION != modo_ant ) {
        switch( RAM_MODO_EJECUCION ) {
            case OPERACION:
                modo_aux = FLAG_MODO_OPERACION;
				_snprintf(cad_modo, sizeof( cad_modo )-1, "%s", ST( S_VENTA ) );
                //strncpy( cad_modo, ST( S_VENTA ), sizeof( cad_modo ) );
                break;
            case ENTRENAMIENTO:
                modo_aux = FLAG_MODO_ENTRENAMIENTO;
				_snprintf(cad_modo, sizeof( cad_modo) -1, "%s", ST( S_ENTRENAMIENTO ) );//
                //strncpy( cad_modo, ST( S_ENTRENAMIENTO ), sizeof( cad_modo ) );
                break;
            case INVENTARIO:
                modo_aux = FLAG_MODO_INVENTARIO;
                _snprintf(cad_modo, sizeof( cad_modo )-1, "%s", ST( S_INVENTARIO ) );//strncpy( cad_modo, ST( S_INVENTARIO ), sizeof( cad_modo ) );
                break;
            case ETIQUETAS:
                modo_aux = FLAG_MODO_ETIQUETAS;
                _snprintf(cad_modo, sizeof( cad_modo )-1, "%s", ST( S_ETIQUETAS ) );//strncpy( cad_modo, ST( S_ETIQUETAS ), sizeof( cad_modo ) );
                break;
            case TRANSFERENCIAS:
                modo_aux = FLAG_MODO_TRANSFERENCIA;
                _snprintf(cad_modo, sizeof( cad_modo )-1, "%s", ST( S_TRANSFERENC_ ) );//strncpy( cad_modo, ST( S_TRANSFERENC_ ), sizeof( cad_modo ) );
                break;
		    case CAMBIO_MEDIO:
				modo_aux = FLAG_CAMBIO_MEDIO;
                _snprintf(cad_modo, sizeof( cad_modo )-1, "%s", ST( S_CAMBIO_MEDIOS ) );//strncpy( cad_modo, ST( S_CAMBIO_MEDIOS ), sizeof( cad_modo ) );
                break;
			case COBRO:
				modo_aux = FLAG_COBRO;
                _snprintf(cad_modo, sizeof( cad_modo )-1, "%s", ST( S_COBROS ) );//strncpy( cad_modo, "COBROS", sizeof( cad_modo ) );
                break;
        }
        memset( aux, 0, 25 );
        _itoa( modo_aux, aux, 10 );
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_MODO_BORRAR, " ", " ", NULL, " " );
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_MODO_AGREGAR, aux, aux, NULL, " " );
		//ExecuteSAction( A_MUESTRA_MODO, cad_modo );
        modo_ant = RAM_MODO_EJECUCION;
    }
    if( IMPRIMIR_EN_JOURNAL ) {
        memset( aux, 0, 25 );
        //sprintf( aux, "MODO %s\n", cmodo[modo] );
        _snprintf(aux, sizeof(aux)-1, "MODO %s", cmodo[modo] );
        PRN_SELECT( JOURNAL );
        if( IMPRESORA_FISCAL ) {
        }
        else {
            MOSTRAR_CADENA( 1, PRN, aux );
            LF( 1 );
            setData( ACTION_PRINT, NULL, 0 );
        }
    }
    PRN_HABILITA_AVANCES();
    GRABAR_LOG_SISTEMA( aux,LOG_DEBUG,1 );
}

