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
#include <b_mens.h> 
#include <cio.h> 
#ifdef INVEL_W
#include <io.h>
#else
#include <sys/io.h>
#endif
#include <cmemory.h> 
#include <cr.h>  
#include <cr_disc2.h> 
#include <cr_disco.h> 
#include <cupones.h> 
#include <dbrouter.h> 
#include <export.h> 
#include <import.h> 
#include <lista.h> 
#include <log.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <script.h> 
#include <socios.h> 
#include <tkt.h> 
#include <ventas.h> 
#include <zip.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#define _CR_DISC2_C
#undef _CR_DISC2_C
#include <config.h>
#undef MK_FP
#ifdef INVEL_W
#include <dos.h>
#endif
#ifdef INVEL_L
#include <sys/io.h>
#endif
#include <memory.h>
#include <string.h>
#include <aarch.h>
#include <errno.h>
#include <bridge.h>
#include <ini.h>
#include <path.h>
#include <db.h>
#include <deftables.h>
#include <assert.h>
/***********************************************************************/
int VERIFICA_CAMBIO_ARCHIVOS_INICIAL()
/***********************************************************************/
{
    if( RAM_MODO_EJECUCION != ENTRENAMIENTO ) {
        PROCESAR_SCRIPT( SI );
    }
    return ( errno );
}
/***********************************************************************/
int VERIFICA_CAMBIO_ARCHIVOS_ACTIVOS(int inicial)
/***********************************************************************/
{
    int area_ant = 0, tipo_ant = 0;

	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	
	if( RAM_MODO_EJECUCION != ENTRENAMIENTO ) {
		PROCESAR_SCRIPT( inicial );
		if( inicial == NO && MODALIDAD_TICKET_EN_ESPERA != 0) //agregado para Jsincro ) //agregado para modo remoto
			MOSTRAR_T_ESPERA_POR_PANTALLA();
    }

	SELECT_TABLE( area_ant, tipo_ant );

    return ( errno );
}
/***********************************************************************/
int INIT_COPIAR_BOLETIN()
/***********************************************************************/
{
    MENSAJE_SIN_SONIDO_STRING( S_CARGANDO_BOLETINES_NUEVOS_ESPERE_UN_MOMENTO );
    FIN_BUSCA_TARJETA();
    return ( 1 );
}
/**
/* Verifica los permisos de los archivos utilizando ARCH_GZ
/***********************************************************************/
int COMPROBAR_ARCH_GZ( const char *path_orig, char *pnomb_orig )
/***********************************************************************/
{
    return ARCH_GZ( AGZ_A_SOLO_COMP_ACCESO, path_orig, pnomb_orig, "sin destino" );
}
/**
/* Efectua la transferencia utilizando ARCH_GZ
/***********************************************************************/
int COPIAR_ARCH_GZ( const char *path_orig, char *pnomb_orig, const char *path_dest )
/***********************************************************************/
{
    return ARCH_GZ( AGZ_A_TRANSFERIR, path_orig, pnomb_orig, path_dest );
}
/** 
 * Copia un archivo comprimido y luego lo descomprime en el lugar de destino, 
 * todo esto, si existe un archivo de bandera (en el dir del archivo original)
 * con el mismo nombre, pero con extension igual al numero de caja actual.
 *
 * Se verifican permisos y accesos, tener en cuenta que el archivo puede estar
 * en una unidad de red.
 * Tiene 2 funcionalidades (acciones):
 * accion = AGZ_A_TRANSFERIR: verifica si es posible, y transfiere el archivo.
 * accion = AGZ_A_SOLO_COMP_ACCESO : solo verifica si es posible la transferencia.
 * Se asume que el archivo de origen tiene extension .gz, lo extrae y sobreescribe 
 * los contenidos creando un archivo backup (anexandole .bak).
 *
 / @param accion    tipo de accion (ver mas arriba)
 * @param path_orig path del archivo comprimido (posiblemente de red)
 * @param nomb_orig nombre del archivo comprimido
 * @param path_dest path del destino
 * @return          estado de salida
 ***********************************************************************/
int ARCH_GZ( int accion, const char *path_orig, char *pnomb_orig, const char *path_dest )
/***********************************************************************/
{
    #ifdef INVEL_W
    int estado = AGZ_E_OK_NO_COMENZAR;
    char mensaje[300];
    //nombres de archivo usados
    char nomb_orig[255]; //donde se copia pnomb_orig para editarlo
    char arch_origen[255]; //path_orig+nomb_orig
    char arch_destino_tmp_gz[255]; //path_dest+"ob_tmp.tmp.gz"
    char arch_destino_tmp[255]; //path_dest+"ob_tmp.tmp"
    char arch_destino[255]; //path_dest+nomb_orig(sin .gz)
    char arch_bak[255]; //path_dest+nomb_orig(sin .gz)+.bak
    char arch_bandera[255]; //path_orig+nomb_orig(sin .gz)+,.#caja
    char ext_caja[5];
    char *punto;
    strcpy( nomb_orig, pnomb_orig );
    strcpy( arch_origen, path_orig );
    strcat( arch_origen, nomb_orig );
    strcpy( arch_destino_tmp_gz, path_dest );
    strcat( arch_destino_tmp_gz, "OB_TMP.TMP.GZ" );
    strcpy( arch_destino_tmp, path_dest );
    strcat( arch_destino_tmp, "OB_TMP.TMP" );
    strcpy( arch_destino, path_dest );
    punto = strrchr( nomb_orig, '.' );
    #ifdef _DEBUG
    assert( stricmp( _strupr( punto ), ".GZ" ) == 0 ); //tiene que ser .gz
    #endif
    punto[0] = '\0';
    strcat( arch_destino, nomb_orig );
    strcpy( arch_bak, path_dest );
    strcat( arch_bak, nomb_orig );
    strcat( arch_bak, ".BAK" );
    sprintf( ext_caja, ".%03d", NRO_CAJA );
    strcpy( arch_bandera, path_orig );
    strcat( arch_bandera, nomb_orig );
    strcat( arch_bandera, ext_caja );
    /*  Ejemplos
        path_orig           // "j:\\"
        path_dest           // "c:\\"
        nomb_orig           // "tabla.btr.gz"
        arch_origen         // "j:\\tabla.btr.gz"
        arch_destino_tmp_gz // "c:\\ob_tmp.tmp.gz"
        arch_destino_tmp    // "c:\\ob_tmp.tmp"
        arch_destino        // "c:\\tabla.btr"
        arch_bak            // "c:\\tabla.btr.bak"
        arch_bandera        // "j:\\tabla.btr.004"
    */
    if( accion == AGZ_A_TRANSFERIR ) {
        sprintf( mensaje, "Transferencia  Origen %s  Destino %s", arch_origen, arch_destino );
    }
    else if( accion == AGZ_A_SOLO_COMP_ACCESO ) {
        sprintf( mensaje, "Verifica Origen %s", arch_origen );
    }
    GRABAR_LOG_DBROUTER( mensaje ,LOG_DEBUG,4);
    //Se verifica que exista el mapeo a la unidad
    if( _access( path_orig, 0 ) == 0 ) {
        //Se verifica lecto-escritura en bandera
        if( _access( arch_bandera, 06 ) == 0 ) {
            //Se verifica lecto-escritura en gzip
            if( _access( arch_origen, 06 ) == 0 ) {
                estado = AGZ_E_OK_COMENZAR;
            }
            else {
                estado = AGZ_E_ERR_GZIP;
            }
        }
        //Se verifica si la bandera no existe ( CORRECTO ) 
        //o si no tiene permiso ( INCORRECTO )
        else {
            estado = AGZ_E_ERR_BANDERA;
            if( errno == ENOENT ) {
                estado = AGZ_E_OK_NO_COMENZAR;
            }
        }
    }
    else {
        estado = AGZ_E_ERR_MAPEO;
    }
    //si solo se verifica el acceso a los archivos, no se procesa la transferencia
    //TRANSFERENCIA
    if( estado == AGZ_E_OK_COMENZAR && accion == AGZ_A_TRANSFERIR ) {
        sprintf( mensaje, "TRANSFIRIENDO %s", pnomb_orig );
        MENSAJE( mensaje );
        if( COPIAR_ARCHIVO( arch_origen, arch_destino_tmp_gz, __LINE__, __FILE__ ) == 0 ) {
            if( DESCOMPRIMIR( arch_destino_tmp_gz ) ) {
                remove( arch_destino_tmp_gz );
                remove( arch_bak );
                rename( arch_destino, arch_bak );
                rename( arch_destino_tmp, arch_destino );
                remove( arch_bandera );
            }
            else {
                estado = AGZ_E_ERR_DESC;
            }
        }
        else {
            estado = AGZ_E_ERR_COPIA;
        }
    }
    switch( estado ) {
        case AGZ_E_OK_COMENZAR :
            if( accion == AGZ_A_SOLO_COMP_ACCESO ) {
                sprintf( mensaje, "PERMISOS OK - SE PUEDE COMENZAR" );
            }
            else if( accion == AGZ_A_TRANSFERIR ) {
                sprintf( mensaje, "RESULTADO OK" );
            }
            break;
        case AGZ_E_OK_NO_COMENZAR:
            if( accion == AGZ_A_SOLO_COMP_ACCESO ) {
                sprintf( mensaje, "PERMISOS OK (NO ES NECESARIO ACTUALIZAR)" );
            }
            else if( accion == AGZ_A_TRANSFERIR ) {
                sprintf( mensaje, "RESULTADO OK (NO FUE NECESARIO ACTUALIZAR)" );
            }
            break;
        case AGZ_E_ERR_GZIP:
            sprintf( mensaje, "ERR %s NO EXISTE O NO TIENE PERMISOS", arch_origen );
            break;
        case AGZ_E_ERR_BANDERA:
            sprintf( mensaje, "ERR %s NO TIENE PERMISOS", arch_bandera );
            break;
        case AGZ_E_ERR_MAPEO:
            sprintf( mensaje, "ERR NO SE PUEDE ACCEDER UNIDAD %s", path_orig );
            break;
        case AGZ_E_ERR_DESC:
            sprintf( mensaje, "ERR NO SE PUEDE DESCOMPRIMIR %s", arch_origen );
            break;
        case AGZ_E_ERR_COPIA:
            sprintf( mensaje, "ERR NO SE PUEDE COPIAR %s A %s", arch_origen, arch_destino_tmp_gz );
            break;
        default:
            sprintf( mensaje, "ERR DESCONOCIDO" );
            break;
    }
    if( estado != AGZ_E_OK_COMENZAR && estado != AGZ_E_OK_NO_COMENZAR ) {
        MENSAJE_CON_PAUSA( mensaje, 50 );
    }
    GRABAR_LOG_DBROUTER( mensaje ,LOG_ERRORES,2);
    return estado;
    #endif
}

