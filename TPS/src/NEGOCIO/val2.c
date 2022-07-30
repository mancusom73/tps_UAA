#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <dbrouter.h> 
#include <pausa.h> 
#include <val.h> 
#include <val1.h> 
#include <val2.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/*----------------- Variables y definiciones globales -----------------*/
#define MAX_CANT_TARJETA 60
extern unsigned char status,cant_bytes,rta1,rta2;
static unsigned char com_leer_track[] = {0x20,0};
static unsigned char com_status[] = {0x00,0x01};
static unsigned char com_escribir_tarjeta[70] = {0xda,0,0xd0,0,0};
static unsigned char com_borrar_tarjeta[] = {0xda,0,0xde,0,0,0};
static unsigned char com_leer_tarjeta[] = {0xdb,0,0xb0,0,0,0};
static unsigned char com_cod_secreto_tarjeta[] = {0xda,0,0x20,0x04,0,0x02,0,0};
static unsigned char com_cod_borrado_tarjeta[10] = {0xda,0,0x20,0,0,0};
static unsigned char com_version[2] = {0,0};
static unsigned char id_card[] = {2,4,4,3,7};
static unsigned char com_select_card[3] = {0,0x20};
static unsigned char com_power_on[] = {0,0x21};
static unsigned char com_power_off[] = {0,0x22};
static unsigned char com_solenoide[] = {0,0x30};
static unsigned char com_encriptado[10] = {0,0x60};
static unsigned char com_abrir_cajon[] = {0x80,0};
static unsigned char com_reset[] = {0,0x02};
static unsigned char com_anula_apagado[] = {0,0x30};
static unsigned char com_permite_apagado[] = {0,0x31};
static unsigned char tipo_tarjeta;
static unsigned char com_payflex_select[] = {0xda,0,0xa4,0,0,2,0,0};
static unsigned char com_payflex_read[] = {0xdb,0,0xb2,0,4,0xf};
static unsigned char com_mpcos_read[] = {0xdb,0,0xb0,0,0,0};
static unsigned char com_payflex_write[38] = {0xda,0,0xdc,0,4,0};
static unsigned char com_payflex_clave[] = {0xda,0,0x20,0,2,8,0,0};
static unsigned char com_inicializa_lector[] = {0,0x20,3};
/*****************************************************************************/
int VAL_LEER_BANDA_TRACK1( char *buffer )
/*****************************************************************************/
{
    int st, rta = 0, i;
    com_leer_track[1] = 1;
    st = VAL_ENVIAR_COMANDO( com_leer_track, 2, BUFFER );
    if( st ) {
        rta = !status;
    }
    if( rta && cant_bytes > 4 ) {
        for( i = 0;i < cant_bytes - 3;i++ ) {
            buffer[i] = BUFFER[i + 1] + 0x20;
        }
        rta = cant_bytes - 4;
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_LEER_BANDA_TRACK2( char *buffer )
/*****************************************************************************/
{
    int st, rta = 0, i;
    com_leer_track[1] = 0;
    st = VAL_ENVIAR_COMANDO( com_leer_track, 2, BUFFER );
    if( st ) {
        rta = !status;
    }
    if( rta && cant_bytes > 4 ) {
        for( i = 0;i < cant_bytes - 3;i++ ) {
            buffer[i] = BUFFER[i + 1] + '0';
        }
        rta = cant_bytes - 4;
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_STATUS()
/*****************************************************************************/
{
    int st, rta = 0;
    st = VAL_ENVIAR_COMANDO( com_status, 2, BUFFER );
    if( st ) {
        rta = !status;
    }
    if( rta ) {
        rta = ( int )BUFFER[0] * 0x100 + BUFFER[1];
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_RESET()
/*****************************************************************************/
{
    int st, rta = 0;
    st = VAL_ENVIAR_COMANDO( com_reset, 2, BUFFER );
    if( st ) {
        rta = !status;
    }
    PAUSA( 18 );
    return ( rta );
}
/*****************************************************************************/
int VAL_ANULA_APAGADO()
/*****************************************************************************/
{
    int st, rta = 0;
    st = VAL_ENVIAR_COMANDO( com_anula_apagado, 2, BUFFER );
    if( st ) {
        rta = !status;
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_PERMITE_APAGADO()
/*****************************************************************************/
{
    int st, rta = 0;
    st = VAL_ENVIAR_COMANDO( com_permite_apagado, 2, BUFFER );
    if( st ) {
        rta = !status;
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_ESCRIBIR_TARJETA( unsigned char dir, unsigned char cant, unsigned char *buffer,
                          unsigned char *_rta1, unsigned char *_rta2 )
/*****************************************************************************/
{
    int st, rta = 1, ticks, _ticks;
    *_rta1 = 0;
    *_rta2 = 0;
    if( cant <= MAX_CANT_TARJETA ) {
        _ticks = VAL_GET_ESPERA_TICKS();
        ticks = cant;
        if( ticks < 10 ) {
            ticks = 10;
        }
        VAL_SET_ESPERA_TICKS( ticks );
        com_escribir_tarjeta[4] = dir;
        com_escribir_tarjeta[5] = cant;
        memcpy( &com_escribir_tarjeta[6], buffer, cant );
        st = VAL_ENVIAR_COMANDO( com_escribir_tarjeta, ( char )( cant + 6 ), BUFFER );
        if( st ) {
            rta = status;
        }
        if( !rta ) {
            if( _rta1 ) {
                *_rta1 = BUFFER[0];
            }
            if( _rta2 ) {
                *_rta2 = BUFFER[1];
            }
        }
        VAL_SET_ESPERA_TICKS( _ticks );
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_BORRAR_TARJETA( unsigned char dir, unsigned char cant, unsigned char *_rta1,
                        unsigned char *_rta2 )
/*****************************************************************************/
{
    int st, rta = 1;
    *_rta1 = 0;
    *_rta2 = 0;
    if( cant <= MAX_CANT_TARJETA ) {
        com_borrar_tarjeta[4] = dir;
        com_borrar_tarjeta[3] = cant;
        st = VAL_ENVIAR_COMANDO( com_borrar_tarjeta, 6, BUFFER );
        if( st ) {
            rta = status;
        }
        if( !rta ) {
            if( _rta1 ) {
                *_rta1 = BUFFER[0];
            }
            if( _rta2 ) {
                *_rta2 = BUFFER[1];
            }
        }
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_LEER_TARJETA( unsigned char dir, unsigned char cant, unsigned char *buffer,
                      unsigned char *_rta1, unsigned char *_rta2 )
/*****************************************************************************/
{
    int st, rta = 1;
    if( _rta1 ) {
        *_rta1 = 0;
    }
    if( _rta2 ) {
        *_rta2 = 0;
    }
    if( cant <= MAX_CANT_TARJETA ) {
        com_leer_tarjeta[4] = dir;
        com_leer_tarjeta[5] = cant;
        st = VAL_ENVIAR_COMANDO( com_leer_tarjeta, 6, BUFFER );
        if( st ) {
            rta = status;
        }
        if( !rta ) {
            memcpy( buffer, &BUFFER[0], cant );
            if( _rta1 ) {
                *_rta1 = BUFFER[cant];
            }
            if( _rta2 ) {
                *_rta2 = BUFFER[cant + 1];
            }
        }
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_COD_SECRETO_TARJETA( unsigned char *clave, unsigned char *_rta1, unsigned char *_rta2 )
/*****************************************************************************/
{
    int st, rta = 1;
    *_rta1 = 0;
    *_rta2 = 0;
    com_cod_secreto_tarjeta[4] = ( tipo_tarjeta == _GMP416 ) ? 8 : 0xa;
    memcpy( &com_cod_secreto_tarjeta[6], clave, 2 );
    st = VAL_ENVIAR_COMANDO( com_cod_secreto_tarjeta, 8, BUFFER );
    if( st ) {
        rta = status;
    }
    if( !rta ) {
        if( _rta1 ) {
            *_rta1 = BUFFER[0];
        }
        if( _rta2 ) {
            *_rta2 = BUFFER[1];
        }
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_COD_BORRADO1_TARJETA( unsigned char *clave, unsigned char *_rta1, unsigned char *_rta2 )
/*****************************************************************************/
{
    int st, rta = 1, cant, seguir = 1;
    *_rta1 = 0;
    *_rta2 = 0;
    switch( tipo_tarjeta ) {
        case _GMP416:
            com_cod_borrado_tarjeta[3] = 0x40;
            com_cod_borrado_tarjeta[4] = 0x28;
            com_cod_borrado_tarjeta[5] = 0x04;
            cant = 4;
            break;
        case _GMP896:
            com_cod_borrado_tarjeta[3] = 0;
            com_cod_borrado_tarjeta[4] = 0x36;
            com_cod_borrado_tarjeta[5] = 0x06;
            cant = 6;
            break;
        default:
            seguir = 0;
            break;
    }
    if( seguir ) {
        memcpy( &com_cod_borrado_tarjeta[6], clave, cant );
        st = VAL_ENVIAR_COMANDO( com_cod_borrado_tarjeta, ( char )( cant + 6 ), BUFFER );
        if( st ) {
            rta = status;
        }
        if( !rta ) {
            if( _rta1 ) {
                *_rta1 = BUFFER[0];
            }
            if( _rta2 ) {
                *_rta2 = BUFFER[1];
            }
        }
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_COD_BORRADO2_TARJETA( unsigned char *clave, unsigned char *_rta1, unsigned char *_rta2 )
/*****************************************************************************/
{
    int st, rta = 1, cant, seguir = 1;
    *_rta1 = 0;
    *_rta2 = 0;
    switch( tipo_tarjeta ) {
        case _GMP896:
            com_cod_borrado_tarjeta[3] = 0x80;
            com_cod_borrado_tarjeta[4] = 0x5c;
            com_cod_borrado_tarjeta[5] = 0x04;
            cant = 4;
            break;
        default:
            seguir = 0;
            break;
    }
    if( seguir ) {
        memcpy( &com_cod_borrado_tarjeta[6], clave, cant );
        st = VAL_ENVIAR_COMANDO( com_cod_borrado_tarjeta, ( char )( cant + 6 ), BUFFER );
        if( st ) {
            rta = status;
        }
        if( !rta ) {
            *_rta1 = BUFFER[0];
            *_rta2 = BUFFER[1];
        }
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_VERSION( char *buffer )
/*****************************************************************************/
{
    int st, rta = 99;
    st = VAL_ENVIAR_COMANDO( com_version, 2, BUFFER );
    if( st ) {
        rta = status;
        memcpy( buffer, &BUFFER[0], cant_bytes );
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_SELECT_CARD( char tipo )
/*****************************************************************************/
{
    int st, rta = 99;
    com_select_card[2] = id_card[tipo - 1];
    tipo_tarjeta = tipo;
    st = VAL_ENVIAR_COMANDO( com_select_card, 3, BUFFER );
    if( st ) {
        rta = status;
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_POWER_ON()
/*****************************************************************************/
{
    int st, rta = 99;
    st = VAL_ENVIAR_COMANDO( com_power_on, 2, BUFFER );
    if( st ) {
        rta = status;
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_POWER_OFF()
/*****************************************************************************/
{
    int st, rta = 99;
    st = VAL_ENVIAR_COMANDO( com_power_off, 2, BUFFER );
    if( st ) {
        rta = status;
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_SOLENOIDE()
/*****************************************************************************/
{
    int st, rta = 99;
    st = VAL_ENVIAR_COMANDO( com_solenoide, 2, BUFFER );
    if( st ) {
        rta = status;
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_ENCRIPTADO( char *buf1, char *buf2 )
/*****************************************************************************/
{
    int st, rta = 99;
    memcpy( &com_encriptado[2], buf1, 8 );
    st = VAL_ENVIAR_COMANDO( com_encriptado, 10, BUFFER );
    if( st ) {
        rta = status;
        memcpy( buf2, &BUFFER[0], cant_bytes );
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_ABRIR_CAJON()
/*****************************************************************************/
{
    int st, rta = 99;
    st = VAL_ENVIAR_COMANDO( com_abrir_cajon, 2, BUFFER );
    if( st ) {
        rta = status;
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_PAYFLEX_SELECT( unsigned dir, int tipo )
/*****************************************************************************/
{
    int st, rta = 99;
    com_payflex_select[3] = tipo;
    com_payflex_select[6] = dir >> 8;
    com_payflex_select[7] = dir & 0xff;
    st = VAL_ENVIAR_COMANDO( com_payflex_select, 8, BUFFER );
    if( st ) {
        rta = status;
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_PAYFLEX_READ( char registro, int bytes, char *buffer )
/*****************************************************************************/
{
    int st, rta = 99;
    com_payflex_read[3] = registro;
    com_payflex_read[5] = bytes;
    st = VAL_ENVIAR_COMANDO( com_payflex_read, sizeof( com_payflex_read ), BUFFER );
    if( st ) {
        rta = status;
        memcpy( buffer, &BUFFER[0], cant_bytes );
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_PAYFLEX_WRITE( char registro, char *buffer, int bytes )
/*****************************************************************************/
{
    int st, rta = 99;
    com_payflex_write[3] = registro;
    com_payflex_write[5] = bytes;
    if( bytes < sizeof( com_payflex_write ) - 6 ) {
        memcpy( &com_payflex_write[6], buffer, bytes );
        st = VAL_ENVIAR_COMANDO( com_payflex_write, ( char )( bytes + 6 ), BUFFER );
        if( st ) {
            rta = status;
        }
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_PAYFLEX_CLAVE( unsigned clave )
/*****************************************************************************/
{
    int st, rta = 99;
    com_payflex_clave[6] = clave >> 8;
    com_payflex_clave[7] = clave & 0xff;
    st = VAL_ENVIAR_COMANDO( com_payflex_clave, 8, BUFFER );
    if( st ) {
        rta = status;
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_INICIALIZA_LECTOR()
/*****************************************************************************/
{
    int st, rta = 99;
    st = VAL_ENVIAR_COMANDO( com_inicializa_lector, 3, BUFFER );
    if( st ) {
        rta = status;
    }
    return ( rta );
}
/*****************************************************************************/
int VAL_MPCOS_READ( char registro, int bytes, char *buffer )
/*****************************************************************************/
{
    int st, rta = 99;
    //   com_mpcos_read[3] = registro;
    com_mpcos_read[5] = bytes;
    st = VAL_ENVIAR_COMANDO( com_mpcos_read, sizeof( com_mpcos_read ), BUFFER );
    if( st ) {
        rta = status;
        memcpy( buffer, &BUFFER[0], cant_bytes );
    }
    return ( rta );
}

