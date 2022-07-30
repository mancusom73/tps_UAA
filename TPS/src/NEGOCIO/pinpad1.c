#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <pinpad1.h> 
#include <status.h> 
#include <timer.h> 
#include <cmemory.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if !defined(INVEL_W) && !defined(INVEL_L)
#endif
/*----------------- Variables y definiciones globales -----------------*/
static int _PUERTO = 0;
#define ACK (buffer[0] == comando[1])
#define APP comando[0]
#define LEN comando[4]
void PINPAD1_DELAY( int tiempo );
/***************************************************************************/
void SET_PUERTO( int puerto )
/***************************************************************************/
{
    _PUERTO = puerto - 1;
}
/***************************************************************************/
int INICIALIZAR_PUERTO()
/***************************************************************************/
{
    unsigned status = 1;
    #if !defined(INVEL_W) && !defined(INVEL_L)
    status = bioscom( 0, 0xE3, _PUERTO );         /* 111 00 0 11  = 0xE3 */
    #endif
    PINPAD1_DELAY( 1500 );
    return ( status );
}
/***************************************************************************/
int ENVIAR_COMANDO( comando, buf_datos, error1, error2, error3 )
unsigned char *comando,*buf_datos,*error1,*error2,*error3;
/***************************************************************************/
{
    unsigned resultado = 0;
    #ifdef COMPILAR_PINPAD1
    unsigned i;
    unsigned char lrc, buffer[256], buf_envio[256], cant, cant_envio = 0, leer;
    /*---- Pepara buffer para envio de datos si es comando de escritura ----*/
    if( ( APP == 0xf1 || APP == 0xf4 || APP == 0xe1 ) && LEN > 0 ) {
        cant_envio = LEN + 1;
        for( i = 0;i < LEN;i++ ) {
            buf_envio[i] = buf_datos[i];
            if( i == 0 ) {
                lrc = buf_envio[0];
            }
            else {
                lrc ^= buf_envio[i];
            }
        }
        buf_envio[LEN] = lrc;
    }
    /*------------------------ Envia comando -------------------------------*/
    for( i = 0;i < 5;i++ ) {
        #if !defined(INVEL_W) && !defined(INVEL_L)
        bioscom( 1, comando[i], _PUERTO );
        #endif
        if( i == 0 ) {
            lrc = APP;
        }
        else {
            lrc ^= comando[i];
        }
        PINPAD1_DELAY( 5 );
    }
    #if !defined(INVEL_W) && !defined(INVEL_L)
    bioscom( 1, lrc, _PUERTO );
    #endif
    /*--------- Espera Respuesta ----------*/
    PINPAD1_DELAY( 3 );
    RECIBIR_RESPUESTA( buffer, 3 );
    /*---- controla LRC ------*/
    if( ( buffer[0] ^ buffer[1] ) != buffer[2] ) {
        resultado = 1;
    }
    leer = 1;
    switch( buffer[0] ) {
        case 153:
            /*--------------- error inicial ------------------*/
            *error1 = buffer[0];
            *error2 = buffer[1];
            *error3 = 0xff;
            leer = 0;
            break;
        default:
            /*------------------ otra respuesta --------------*/
            if( !ACK ) {
                resultado = 1;
            }
            break;
    }
    /*-------------------- Espera mas datos si es necesario ------------------*/
    if( !resultado && leer ) {
        switch( APP ) {
            case 0xe1:
                /*------------- ESCRITURA EN TARJETA GP416 --------------*/
            case 0xf1:
                /*---------- ESCRITURA EN TARJETA ELECTRONICA -----------*/
                if( LEN > 0 ) {
                    ENVIAR_DATOS( buf_envio, cant_envio );
                }
                PINPAD1_DELAY( 3 );
                /*----- Coloco respuesta en variables de error  y controla lrc -----*/
                RECIBIR_RESPUESTA( buffer, 4 );
                *error1 = buffer[0];
                *error2 = buffer[1];
                *error3 = buffer[2];
                if( ( *error1 ^ *error2 ^ *error3 ) != buffer[3] ) {
                    resultado = 1;
                }
                break;
            case 0xe2:
                /*-------------- LECTURA EN TARJETA GP416 ---------------*/
                if( LEN == 0 ) {
                    cant = 4;
                }
                else {
                    cant = LEN + 5;
                }
                RECIBIR_RESPUESTA( buffer, cant );
                /*----- Pasa datos recibidos al buffer de datos y controla lrc ----*/
                for( i = 0;i < LEN;i++ ) {
                    buf_datos[i] = buffer[i];
                    if( i == 0 ) {
                        lrc = buffer[0];
                    }
                    else {
                        lrc ^= buffer[i];
                    }
                }
                if( lrc != buffer[LEN] ) {
                    resultado = 1;
                }
                /*----- Coloca respuesta en variables de error  y controla lrc -----*/
                *error1 = buffer[LEN + 1];
                *error2 = buffer[LEN + 2];
                *error3 = buffer[LEN + 3];
                if( ( *error1 ^ *error2 ^ *error3 ) != buffer[LEN + 4] ) {
                    resultado = 1;
                }
                break;
            case 0xf2:
                /*----------- LECTURA EN TARJETA ELECTRONICA ------------*/
                if( LEN == 0 ) {
                    cant = 4;
                }
                else {
                    cant = LEN + 5;
                }
                RECIBIR_RESPUESTA( buffer, cant );
                /*----- Pasa datos recibidos al buffer de datos y controla lrc ----*/
                for( i = 0;i < LEN;i++ ) {
                    buf_datos[i] = buffer[i];
                    if( i == 0 ) {
                        lrc = buffer[0];
                    }
                    else {
                        lrc ^= buffer[i];
                    }
                }
                if( lrc != buffer[LEN] ) {
                    resultado = 1;
                }
                /*----- Coloca respuesta en variables de error  y controla lrc -----*/
                *error1 = buffer[LEN + 1];
                *error2 = buffer[LEN + 2];
                *error3 = buffer[LEN + 3];
                if( ( *error1 ^ *error2 ^ *error3 ) != buffer[LEN + 4] ) {
                    resultado = 1;
                }
                break;
            case 0xf0:
                /*------------------------ STATUS -----------------------*/
            case 0xfa:
                /*----------------------- SCANNER -----------------------*/
                RECIBIR_RESPUESTA( buffer, LEN );
                /*----- Pasa datos recibidos al buffer de datos y controla lrc ----*/
                for( i = 0;i < LEN;i++ ) {
                    buf_datos[i] = buffer[i];
                    if( i == 0 ) {
                        lrc = buffer[0];
                    }
                    else {
                        lrc ^= buffer[i];
                    }
                }
                /*----------- Pone 0 en variables de error --------------*/
                *error1 = 0;
                *error2 = 0;
                *error3 = 0;
                break;
            case 0xf8:
                /*------------------ TARJETA MAGNETICA ------------------*/
                PINPAD1_DELAY( 2 );
                RECIBIR_RESPUESTA( &buffer[0], 1 );
                RECIBIR_RESPUESTA( &buffer[1], buffer[0] );
                /*----- Pasa datos recibidos al buffer de datos y controla lrc ----*/
                for( i = 0;i < buffer[0];i++ ) {
                    buf_datos[i] = buffer[i];
                    if( i == 0 ) {
                        lrc = buffer[0];
                    }
                    else {
                        lrc ^= buffer[i];
                    }
                }
                if( buffer[buffer[0]] != lrc ) {
                    resultado = 1;
                }
                /*----------- Pone 0 en variables de error --------------*/
                *error1 = 0;
                *error2 = 0;
                *error3 = 0;
                break;
        }
    }
    #endif
    return ( resultado );
}
/***************************************************************************/
void RECIBIR_RESPUESTA( unsigned char *rta, unsigned char cant )
/***************************************************************************/
{
    unsigned status = 0;
    int i, c = 0;
    for( i = 0;i < cant && c < 2;i++ ) {
        #if !defined(INVEL_W) && !defined(INVEL_L)
        status = bioscom( 2, 0, _PUERTO );
        #endif
        if( ( status & 0xff00 ) == 0xe000 ) {
            i--;
            c++;
        }
        else {
            rta[i] = status & 0x00ff;
        }
    }
}
/***************************************************************************/
void ENVIAR_DATOS( unsigned char *datos, unsigned char cant )
/***************************************************************************/
{
    int i;
    for( i = 0;i < cant;i++ ) {
        #if !defined(INVEL_W) && !defined(INVEL_L)
        bioscom( 1, datos[i], _PUERTO );
        #endif
        PINPAD1_DELAY( 5 );
    }
}
/*****************************************************************************/
int EJECUTAR_COMANDO( app, com, ref1, ref2, len )
unsigned char app, com, ref1, ref2, len;
/*****************************************************************************/
/*---------------------------------------------------------------------------*/
/* Respuesta : 0 -> Comunicaci¢n OK          (Errores en res1 y res2)        */
/*        1 -> Error en la comunicaci¢n (res1 y res2 = 0xFF    )        */
/*---------------------------------------------------------------------------*/
{
    int st = 0;
    #ifdef COMPILAR_PINPAD1
    unsigned char cmd[5];
    unsigned char rta1,rta2,rta3;
    cmd[0] = app;
    cmd[1] = com;
    cmd[2] = ref1;
    cmd[3] = ref2;
    cmd[4] = len;
    st = ENVIAR_COMANDO( cmd, BUFFER, &rta1, &rta2, &rta3 );
    /*-------------------- TRATAMIENTO DE LAS RESPUESTAS ---------------------*/
    if( !st ) {
        if( app == 0xf1 || app == 0xf2 ) {
            /************************* GEMPLUS ***********************/
            if( rta1 == com ) {
                /*------------------------------- ACK ------------------------------*/
                switch( rta2 ) {
                    case 0x90:
                        /* Comando ejecutado                  */
                    case 0x92:
                        /* Problema de escritura en la EEPROM */
                    case 0x94:
                        /* Datos incoherentes                 */
                    case 0x98:
                        /* Problema de seguridad              */
                        res1 = rta2;
                        res2 = rta3;
                        break;
                    default:
                        /* Respuesta no contemplada           */
                        res1 = 0xff;
                        res2 = 0xff;
                        st = 1;
                        break;
                }
            }
            else {
                /*---------------------- RESPUESTA SIN ACK -------------------------*/
                switch( rta1 ) {
                    case 153:
                        /*-------- No se puede comunicar ----------*/
                        switch( rta2 ) {
                            case 0:
                                /* CRC incorrecto            */
                            case 2:
                                /* Time Out                  */
                            case 4:
                                /* Tarjeta no presente       */
                                res1 = rta1;
                                res2 = rta2;
                                break;
                            default:
                                /* Respuesta no contemplada */
                                res1 = 0xff;
                                res2 = 0xff;
                                st = 1;
                                break;
                        }
                        break;
                    case 0x67:
                        /* Longitud incorrecta            */
                    case 0x6b:
                        /* Referencia incorrecta          */
                    case 0x6d:
                        /* Comando desconocido o invalido */
                    case 0x6e:
                        /* Byte de aplicacin erroneo     */
                    case 0x6f:
                        /* Error no especificado          */
                        res1 = rta1;
                        res2 = 0;
                        break;
                    default:
                        /* Respuesta no contemplada       */
                        res1 = 0xff;
                        res2 = 0xff;
                        st = 1;
                        break;
                }
            }
        }
        else {
            /********************* SERIAL ***********************/
            res1 = rta1;
            res2 = rta2;
        }
    }
    else {
        res1 = 0xff;
        res2 = 0xff;
    }
    #endif
    return ( st );
}
/*****************************************************************************/
int GET_RESPUESTA( unsigned char nro )
/*****************************************************************************/
{
    int rta;
    if( nro == 1 ) {
        rta = res1;
    }
    else {
        rta = res2;
    }
    return ( rta );
}
/*****************************************************************************/
void PINPAD1_DELAY( int tiempo )
/*****************************************************************************/
{
    if( INICIALIZAR_TIMER ) {
        INIT_TIMER();
    }
    DELAY( tiempo );
}

