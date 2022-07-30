#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cargacon.h> 
#include <config.h> 
#include <cr.h>  
#include <dbrouter.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
int _CARGA_procesa_cadena( char *cadena, int *buffer, int ancho );
/***************************************************************************/
int CARGAR_ARCHIVO_DE_COMANDOS( char *nom_archivo, int *buffer, int ancho )
/***************************************************************************/
{
    char cadena[100],*p;
    int rta = 0, i = 0;
    FILE *fp;
    fp = fopen( nom_archivo, "r+t" );
    if( fp ) {
        rta = 1;
        while( fgets( cadena, sizeof( cadena ), fp ) ) {
            p = strchr( cadena, '\n' );
            if( p ) {
                p[0] = 0;
            }
            if( _CARGA_procesa_cadena( cadena, &buffer[i * ancho], ancho ) ) {
                i++;
            }
        }
        fclose( fp );
    }
    return ( rta );
}
/***************************************************************************/
int _CARGA_procesa_cadena( char *cadena, int *buffer, int ancho )
/***************************************************************************/
{
    char _codigo[10];
    int i, j, k, largo, rta = 0;
    largo = strlen( cadena );
    for( i = 0, j = 0;j < ancho && i < largo; ) {
        if( cadena[i] == '<' ) {
            k = 0;
            while( cadena[++i] != '>' ) {
                _codigo[k++] = cadena[i];
            }
            _codigo[k] = 0;
            buffer[j++] = atoi( _codigo );
            i++;
            rta = 1;
        }
        else if( cadena[i] == ';' ) {
            i = largo;
        }
        else if( cadena[i] == ' ' ) {
            i++;
        }
            /* Para considerar los tabs */
        else if( cadena[i] == 9 ) {
            i++;
        }
        else {
            buffer[j++] = cadena[i++];
            rta = 1;
        }
    }
    return ( rta );
}

