#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <archivos.h> 
#include <cio.h> 
#include <cr.h> 
#include <export.h> 
#include <tkt.h> 
#include <trim.h> 
#include <aarch.h>
#include <DefTables.h>
#include <path.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/***********************************************************************/
int EXPORTAR_ARCHIVO()
/***********************************************************************/
{
    char origen[51],destino[51],aux[51],*p;
    int handle, rta = 0;
    /*---- Obtengo el nombre del archivo a copiar ----*/
    handle = OPEN( _IMPORT, O_RDONLY );
    if( handle > 0 ) {
        if( _READ( handle, origen, 31, __LINE__, __FILE__ ) == 0 ) {
            TRIM( origen );
            /*----------------- construye el nombre del destino ----------------*/
            p = strrchr( origen, '\\' );
            if( p ) {
                strcpy( aux, &p[1] );
            }
            else {
                strcpy( aux, origen );
            }
            strcpy( destino, "N:" );
            strcat( destino, aux );
            /*------------------------ realiza la copia -------------------------*/
            if( !COPIAR_ARCHIVO( origen, destino, __LINE__, __FILE__ ) ) {
                rta = 1;
            }
        }
        /*------------ cierra import -------------*/
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    return ( rta );
}

/*****************************************************************************/
int EXPORTA_ARCHIVO_DE_LA_SEMANA( unsigned fecha, int origen, int destino )
/*****************************************************************************/
{
    _FECHA_ARCHIVO = fecha;
    return ( !COPY( origen, destino ) );
}
/*****************************************************************************/
int BACKUP_ARCHIVO_DE_LA_SEMANA( unsigned fecha, int origen, int destino )
/*****************************************************************************/
{
    int a;
	char nom_destino[80];

    _FECHA_ARCHIVO = fecha;
    a = !COPY( origen, destino );
    NOM_ARCHIVO( destino, nom_destino );
	LOG_EXISTE_ARCHIVO( nom_destino, LOG_VARIOS,2);

    return ( a );
}


/*****************************************************************************/
int BACKUP_ARCHIVO_DE_LA_SEMANA_SQL( unsigned fecha, int origen, int destino )
/*****************************************************************************/
{
    int a;
//	char aux[80], aux1[80], *p, *p1;
	char nombre_tabla_tmp[35];
	char nombre_tabla_dest_tmp[80];
	//Nombre de la tabla origen + path
	char nombre_tabla_orig_frm[80]; 
	char nombre_tabla_orig_myd[80]; 
	char nombre_tabla_orig_myi[80]; 
	//Nombre de la tabla destino + path
	char nombre_tabla_dest_frm[80]; 
	char nombre_tabla_dest_myd[80]; 
	char nombre_tabla_dest_myi[80];


    _FECHA_ARCHIVO = fecha;

	memset( nombre_tabla_orig_frm, 0, sizeof( nombre_tabla_orig_frm ) );
	memset( nombre_tabla_orig_myd, 0, sizeof( nombre_tabla_orig_myd ) );
	memset( nombre_tabla_orig_myi, 0, sizeof( nombre_tabla_orig_myi ) );
	memset( nombre_tabla_dest_frm, 0, sizeof( nombre_tabla_dest_frm ) );
	memset( nombre_tabla_dest_myd, 0, sizeof( nombre_tabla_dest_myd ) );
	memset( nombre_tabla_dest_myi, 0, sizeof( nombre_tabla_dest_myi ) );

	//obtiene el nombre de la tabla origen
	a = GET_TABLE_NAME( origen, TT_ORIG, nombre_tabla_tmp, sizeof( nombre_tabla_tmp ) );
	_snprintf(nombre_tabla_orig_frm,sizeof(nombre_tabla_orig_frm)-1,"%s%s.frm",PATH_MYSQL,nombre_tabla_tmp);
	_snprintf(nombre_tabla_orig_myd,sizeof(nombre_tabla_orig_myd)-1,"%s%s.MYD",PATH_MYSQL,nombre_tabla_tmp);
	_snprintf(nombre_tabla_orig_myi,sizeof(nombre_tabla_orig_myi)-1,"%s%s.MYI",PATH_MYSQL,nombre_tabla_tmp);

	//obtiene el nombre del destino sacado de la tabla archivos
	NOM_ARCHIVO( destino, nombre_tabla_dest_tmp );
	_snprintf(nombre_tabla_dest_frm,sizeof(nombre_tabla_dest_frm)-1,"%s.frm",nombre_tabla_dest_tmp);
	_snprintf(nombre_tabla_dest_myd,sizeof(nombre_tabla_dest_myd)-1,"%s.MYD",nombre_tabla_dest_tmp);
	_snprintf(nombre_tabla_dest_myi,sizeof(nombre_tabla_dest_myi)-1,"%s.MYI",nombre_tabla_dest_tmp);

	a = COPIAR_ARCHIVO( nombre_tabla_orig_myd, nombre_tabla_dest_myd, __LINE__, __FILE__ );
	a = COPIAR_ARCHIVO( nombre_tabla_orig_frm, nombre_tabla_dest_frm, __LINE__, __FILE__ );
	a = COPIAR_ARCHIVO( nombre_tabla_orig_myi, nombre_tabla_dest_myi, __LINE__, __FILE__ );

    return ( a );
}
/*****************************************************************************/
int LEE_FECHA( int cod_arch )
/*****************************************************************************/
{
    int handle, fecha = 0;
    handle = OPEN( cod_arch, O_RDONLY );
    if( handle > 0 ) {
        if( _READ( handle, ( char* )&fecha, 2, __LINE__, __FILE__ ) == 0 ) {
            _CLOSE( handle, __LINE__, __FILE__ );
        }
        else {
            fecha = 0;
        }
    }
    return ( fecha );
}

