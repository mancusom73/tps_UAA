#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <m_cadena.h> 
#include <m_fecha.h> 
#include <stringt.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void MOSTRAR_FECHA( int x, int y, unsigned fecha )
/******************************************************************************/
{
    unsigned char cadena[9];
    DTOC( fecha, cadena );
    MOSTRAR_CADENA( x, y, cadena );
}
/******************************************************************************/
void MOSTRAR_FECHAMMAA( int x, int y, unsigned fecha )
/******************************************************************************/
{
    unsigned char cadena[9];
    DTOC( fecha, cadena );
    MOSTRAR_CADENA( x, y, &cadena[3] );
}
/******************************************************************************/
void OBTENER_FECHA_LETRAS( unsigned fecha, unsigned char result[200] )
/******************************************************************************/
{
    char *meses[] = {"ENERO","FEBRERO","MARZO","ABRIL","MAYO","JUNIO","JULIO","AGOSTO","SEPTIEMBRE",
                     "OCTUBRE","NOVIEMBRE","DICIEMBRE"};
    int dia,mes,anio;
    DTOC( fecha, (char *)result );
    sscanf( (char *)result, "%d/%d/%d", &dia, &mes, &anio );
    sprintf( (char *)result, "%d DE %s DE %d", dia, meses[mes - 1], anio + 2000 );
}
/******************************************************************************/
void MOSTRAR_FECHA_LETRAS( int x, int y, unsigned fecha )
/******************************************************************************/
{
    unsigned char cadena[200];
    char *meses[] = {"ENERO","FEBRERO","MARZO","ABRIL","MAYO","JUNIO","JULIO","AGOSTO","SEPTIEMBRE",
                     "OCTUBRE","NOVIEMBRE","DICIEMBRE"};
    int dia,mes,anio;
    DTOC( fecha, (char *)cadena );
    sscanf( (char *)cadena, "%d/%d/%d", &dia, &mes, &anio );
    sprintf( (char *)cadena, "%d DE %s DE %d", dia, meses[mes - 1], anio + 2000 );
    MOSTRAR_CADENA( x, y, (char *)cadena );
}
/******************************************************************************/
void DTOC( unsigned fecha, char *cadena )
/******************************************************************************/
{
    unsigned char mes,dia,dd[3],mm[3],aa[5];
    unsigned anio;
    anio = ( ( fecha & 0xFE00 ) >> 9 ) + 1980;
    mes = ( ( fecha & 0x01E0 ) >> 5 );
    dia = ( fecha & 0x001F );
    _itoa( dia, dd, 10 );
    _itoa( mes, mm, 10 );
    _itoa( anio, aa, 10 );
    cadena[0] = ( dd[1] == 0 ) ? '0' : dd[0];
    cadena[1] = ( dd[1] == 0 ) ? dd[0] : dd[1];
    cadena[2] = '/';
    cadena[3] = ( mm[1] == 0 ) ? '0' : mm[0];
    cadena[4] = ( mm[1] == 0 ) ? mm[0] : mm[1];
    cadena[5] = '/';
    cadena[6] = ( aa[1] == 0 ) ? '0' : aa[2];
    cadena[7] = ( aa[1] == 0 ) ? aa[0] : aa[3];
    cadena[8] = 0;
    if( fecha == 0 ) {
        strcpy( cadena, "  /  /  " );
    }
}
/******************************************************************************/
void DTOC4( unsigned fecha, char *cadena )
/******************************************************************************/
{
    unsigned char mes,dia;
    unsigned anio;
    anio = ( ( fecha & 0xFE00 ) >> 9 ) + 1980;
    mes = ( ( fecha & 0x01E0 ) >> 5 );
    dia = ( fecha & 0x001F );
    if( fecha == 0 ) {
        strcpy( cadena, "  /  /  " );
    }
    else {
        sprintf( cadena, "%.02i/%.02i/%.04i", dia, mes, anio );
    }
}
/******************************************************************************/
void MOSTRAR_FECHA_2000( char x, char y, unsigned fecha )
/******************************************************************************/
{
    unsigned char cadena[11];
    DTOC_2000( fecha, cadena );
    MOSTRAR_CADENA( x, y, cadena );
}
/******************************************************************************/
void DTOC_2000( unsigned fecha, char *cadena )
/******************************************************************************/
{
    unsigned char mes,dia,dd[3],mm[3],aa[5];
    unsigned anio;
    anio = ( ( fecha & 0xFE00 ) >> 9 ) + 1980;
    mes = ( ( fecha & 0x01E0 ) >> 5 );
    dia = ( fecha & 0x001F );
    #ifdef INVEL_L
    sprintf( (char *)dd, "%ud", dia );
    sprintf( (char *)mm, "%ud", mes );
    sprintf( (char *)aa, "%ud", anio );
    #else
    _itoa( dia, dd, 10 );
    _itoa( mes, mm, 10 );
    _itoa( anio, aa, 10 );
    #endif
    cadena[0] = ( dd[1] == 0 ) ? '0' : dd[0];
    cadena[1] = ( dd[1] == 0 ) ? dd[0] : dd[1];
    cadena[2] = '/';
    cadena[3] = ( mm[1] == 0 ) ? '0' : mm[0];
    cadena[4] = ( mm[1] == 0 ) ? mm[0] : mm[1];
    cadena[5] = '/';
    cadena[6] = aa[0];
    cadena[7] = aa[1];
    cadena[8] = aa[2];
    cadena[9] = aa[3];
    cadena[10] = 0;
    if( fecha == 0 ) {
        strcpy( cadena, "  /  /    " );
    }
}
/******************************************************************************/
void FORMAT_FECHA( char *picture, unsigned fecha, char *cadena )
/******************************************************************************/
{
    unsigned char mes,dia;
    unsigned anio;
    int cant,i,j;
    anio = ( ( fecha & 0xFE00 ) >> 9 ) + 1980;
    mes = ( ( fecha & 0x01E0 ) >> 5 );
    dia = ( fecha & 0x001F );
    for( i = 0 ;i < ( int )strlen( picture ) ;i++ ) {
        switch( picture[i] ) {
            case 'A':
                j = i;
                cant = 0;
                while( picture[j++] == 'A' ) {
                    cant++;
                }
                sprintf( &cadena[i], "%.*i", cant, ( cant < 4 ) ? anio % 100 : anio );
                i += cant - 1;
                break;
            case 'M':
                j = i;
                cant = 0;
                while( picture[j++] == 'M' ) {
                    cant++;
                }
                sprintf( &cadena[i], "%.*i", cant, mes );
                i += cant - 1;
                break;
            case 'D':
                j = i;
                cant = 0;
                while( picture[j++] == 'D' ) {
                    cant++;
                }
                sprintf( &cadena[i], "%.*i", cant, dia );
                i += cant - 1;
                break;
            default:
                cadena[i] = picture[i];
                break;
        }
    }
}

/******************************************************************************/
void FECHA_DD_MMM_AAAA( unsigned fecha, unsigned char result[200] )
/******************************************************************************/
{
    char *meses[] = {"ENE","FEB","MAR","ABR","MAY","JUN","JUL","AGO","SEP","OCT","NOV","DIC"};
    int dia,mes,anio;

    DTOC( fecha, result );
    sscanf( result, "%d/%d/%d", &dia, &mes, &anio );

    sprintf( result, "%d-%s-%d", dia, meses[mes - 1], anio + 2000 );
}
