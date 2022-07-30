#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h>  
#include <crypt.h> 
#include <dbrouter.h> 
#include <_cr1.h> 
#include <cmemory.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/**************************************************************************/
void CRYPT( int episodio, char *final )
/**************************************************************************/
{
    #ifdef ENCRIPTADO
    unsigned char __cadena_fiscal_x[] = {36,4,190,219,15,240,177,248,96,147,234,155,52,161,99,122,
                                         96,147,234,155,52,161,99,122,18,89,80,116,16,90,89,126};
    unsigned char __cadena_fiscal_b[] = {105,59,94,165,175,157,122,239,167,224,70,7,201,106,57,177};
    unsigned char __cadena_fiscal_z[] = {73,28,230,215,199,109,121,208,83,107,216,184,140,113,254,
                                         249};
    /*
     * unsigned char __cadena_fiscal_t1[] = {
     * 127, 151,   2, 215, 189,  12,  81,  76,
     * 204, 204, 115,  52,  20, 240, 243, 235,
     * 253, 223, 207,  80, 207, 116,   8, 189,
     * 205, 243,  83, 183,  34, 254, 111,  61
     * };
     * unsigned char __cadena_fiscal_t2[] = {
     * 107, 182,  76, 210, 174, 105,  73, 178,
     * 136,  33, 176,  84, 165, 212, 188, 157,
     * 61, 113,   9,  25,  65,  92,   7, 181,
     * 219, 156, 104, 149, 113,  12, 227, 170
     * };
     */
    unsigned char __conversor_hora[] = {88,202,143,38,220,196,223,227};
    unsigned char __conversor_fecha1[] = {191,35,192,141,116,121,229,17};
    unsigned char __conversor_fecha2[] = {140,246,84,158,14,239,54,110};
    unsigned char __segmento_vga[] = {125,16,187,87,212,14,198,242};
    int rta;
    switch( episodio ) {
        case __CADENA_FISCAL_X:
            rta = HL_CRYPT( &__cadena_fiscal_x, sizeof( __cadena_fiscal_x ) / 8 );
            memcpy( final, __cadena_fiscal_x, 30 );
            final[30] = 0;
            break;
        case __CADENA_FISCAL_B:
            rta = HL_CRYPT( &__cadena_fiscal_b, sizeof( __cadena_fiscal_b ) / 8 );
            memcpy( final, __cadena_fiscal_b, 14 );
            final[14] = 0;
            break;
        case __CADENA_FISCAL_Z:
            rta = HL_CRYPT( &__cadena_fiscal_z, sizeof( __cadena_fiscal_z ) / 8 );
            memcpy( final, __cadena_fiscal_z, 14 );
            final[14] = 0;
            break;
        case __CONVERSOR_HORA:
            rta = HL_CRYPT( &__conversor_hora, sizeof( __conversor_hora ) / 8 );
            memcpy( final, __conversor_hora, 2 );
            break;
        case __CONVERSOR_FECHA1:
            rta = HL_CRYPT( &__conversor_fecha1, sizeof( __conversor_fecha1 ) / 8 );
            memcpy( final, __conversor_fecha1, 2 );
            break;
        case __CONVERSOR_FECHA2:
            rta = HL_CRYPT( &__conversor_fecha2, sizeof( __conversor_fecha2 ) / 8 );
            memcpy( final, __conversor_fecha2, 2 );
            break;
        case __SEGMENTO_VGA:
            rta = HL_CRYPT( &__segmento_vga, sizeof( __segmento_vga ) / 8 );
            memcpy( final, __segmento_vga, 2 );
            break;
    }
    #else
    switch( episodio ) {
        case __CADENA_FISCAL_X:
            switch( IMPRESORA_FISCAL ) {
                case FISCAL_DITEMPO:
                    // Comando X DITEMPO
                    strcpy( final, ",X                            " );
                    break;
                case FISCAL_IBM_4694:
                    // 123456789012345678901234567890
                case FISCAL_IBM_4614:
                    // 123456789012345678901234567890
                    strcpy( final, ",X                            " );
                    break;
            }
            break;
        case __CADENA_FISCAL_B:
            switch( IMPRESORA_FISCAL ) {
                case FISCAL_DITEMPO:
                    strcpy( final, ",B------%012li" );
                    break;
                case FISCAL_IBM_4694:
                    strcpy( final, ",B %012li %s %04i" );
                    break;
                case FISCAL_IBM_4614:
                    strcpy( final, ",B %012li %s %04i" );
                    break;
            }
            break;
        case __CADENA_FISCAL_Z:
            switch( IMPRESORA_FISCAL ) {
                case FISCAL_DITEMPO:
                    strcpy( final, ",Z123456%012li" );
                    break;
                case FISCAL_IBM_4694:
                case FISCAL_IBM_4614:
                    strcpy( final, ",Z      %012li" );
                    break;
            }
            break;
        case __CADENA_FISCAL_T1:
            switch( IMPRESORA_FISCAL ) {
                case FISCAL_DITEMPO:
                    sprintf( final, "  %s          ", ST( S_TOTAL ) );
                    break;
                case FISCAL_IBM_4694:
                case FISCAL_IBM_4614:
                    break;
            }
            break;
        case __CADENA_FISCAL_T2:
            switch( IMPRESORA_FISCAL ) {
                case FISCAL_DITEMPO:
                    strcpy( final, ",TTOTAL        $%12.0lf  " );
                    break;
                case FISCAL_IBM_4694:
                case FISCAL_IBM_4614:
                    break;
            }
            break;
        case __CADENA_FISCAL_V:
            switch( IMPRESORA_FISCAL ) {
                case FISCAL_IBM_4694:
                case FISCAL_IBM_4614:
                    strcpy( final, ",V" );
                    break;
            }
            break;
        case __CONVERSOR_HORA:
            *( unsigned * )( final ) = 100;
            break;
        case __CONVERSOR_FECHA1:
            *( unsigned * )( final ) = 1980;
            break;
        case __CONVERSOR_FECHA2:
            *( unsigned * )( final ) = 5;
            break;
        case __SEGMENTO_VGA:
            *( unsigned * )( final ) = 0xB800;
            break;
    }
    #endif
}

