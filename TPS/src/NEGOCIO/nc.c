#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <btrv.h> 
#include <cambio.h> 
#include <clave.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <cr_disco.h> 
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <getch.h> 
#include <import_t.h> 
#include <mensaje.h> 
#include <menu.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <nc.h> 
#include <pant.h> 
#include <p_fecha.h> 
#include <stack.h> 
#include <_cr1.h> 
#include <actions.h>
#include <aarch.h>
#include <deftables.h>
#include <db.h>
/***************************************************************************/
/* Sistema: TPV 1000                                                       */
/* Mdulo: Rutinas de Devoluciones para Nota de Credito                    */
/* Programador:                                                            */
/* Fecha: 18/03/00                                                         */
/***************************************************************************/
#ifdef INVEL_W
#endif
extern int _ticket_caja;
extern int _ticket_fecha;
extern int _ticket_n_ticket;
extern int _ticket_tipo_comp;
extern long _ticket_secuencia;
#define _TICKET_CAJA           GET_MEMORY_INT( _ticket_caja )
#define _TICKET_FECHA          GET_MEMORY_INT( _ticket_fecha )
#define _TICKET_N_TICKET       GET_MEMORY_INT( _ticket_n_ticket )
#define _TICKET_TIPO_COMP      GET_MEMORY_INT( _ticket_tipo_comp )
#define _TICKET_SECUENCIA      GET_MEMORY_LONG( _ticket_secuencia )
extern int en_importacion_de_ticket;
/*****************************************************************************/
int TIPOS_NC()
/*****************************************************************************/
{
    int rta = 1;
    /* codigo obsoleto
    *if( ( config.pais == CHILE || config.pais == BRASIL ) && NC_PERMANENTE && RAM_NOTA_CR
    * && RAM_TIPO_EVENTO_COMP_ORIGINAL == -1 ) {
    *    if( REFERENCIA_NC() != 0 ) {
    *        MENSAJE_STRING( S_DEBE_IDENTIFICAR_COMPROBANTE_ORIGINAL );
    *        rta = 0;
    *    }
    *    else {
    *        //--- Para que recalcule los impuestos IHA , ICA si corresponde.
    *        ASIGNA_ITEM( SI );
    *    }
    *}
    */

    if( ( RAM_NOTA_CR || MODO_DEVOLUCION ) && RAM_REFERENCIA_NC && rta && !RAM_COMPROMISO_200 ) {
        switch( CONTROL_NOTA_DE_CREDITO ) {
            case 1:
                if( !RAM_ANULACION && !en_importacion_de_ticket ) {
                    rta = EXISTE_ARTICULO_TICKET_ORIGINAL();
                }
                else if( !PEDIR_PRECIO_EN_NC ) {
                    PRECIO_TICKET_ORIGINAL();
                }
                break;
        }
    }
    
    /* codigo obsoleto
    *if( rta && RAM_NOTA_CR && RAM_PEDIR_PRECIO && NC_PERMANENTE ) {
    *    rta = PEDIR_PRECIO_NOTA_CR_LIBRE();
    *}
    */

    return ( rta );
}

/*****************************************************************************/
int REFERENCIA_NC()
/*****************************************************************************/
{
    int _ptr, _handle, ok = 0, _estado;
    int error = 0;
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    /*  _estado = STACK_INS( sizeof( int ) );
        _ticket_caja = STACK_INS( sizeof( int ) );
        _ticket_fecha = STACK_INS( sizeof( int ) );
        _ticket_n_ticket = STACK_INS( sizeof( int ) );*/
    _estado = STACK_INS( sizeof( short ) );
    _ticket_caja = STACK_INS( sizeof( short ) );
    _ticket_fecha = STACK_INS( sizeof( short ) );
    _ticket_n_ticket = STACK_INS( sizeof( short ) );
    _ticket_tipo_comp = STACK_INS( sizeof( short ) );
    _ticket_secuencia = STACK_INS( sizeof( long ) );
    if( _ESTADO == 0 ) {
        PANTALLA_IMPORTAR_NC();
        if( PIDE_DATOS_IMPORTAR_NC() ) {
            _SET_ESTADO( 1 );
        }
        else {
            error = 1;
        }
    }
    if( _ESTADO == 1 ) {
        error = OBTENER_EVENTOS_TEMPORALES( _TICKET_TIPO_COMP, _TICKET_CAJA, _TICKET_N_TICKET,
                                            _TICKET_SECUENCIA, 0, 0, 0, SI, NULL, NO );
        if( !error ) {
            if( VERIFICA_DATOS_IMPORTACION( 1, _TICKET_TIPO_COMP, _TICKET_FECHA, _TICKET_N_TICKET,
                                            _TICKET_CAJA, _TICKET_SECUENCIA ) ) {
                if( IMPORTAR_ITEMS() == 0 ) {
                    ok = 1;
                }
            }
        }
        _SET_ESTADO( 2 );
    }
    if( _ESTADO == 2 ) {
        if( !ok ) {
            if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ST( S_IMPORTAR_EN_NOTA_DE_CREDITO ),
                                  ST( S_IMPORTAR_EN_NOTA_DE_CREDITO ), _F_SUP_AUT_IMPORTAR_NC,
                                  SI ) ) {
                error = PIDE_TIPO_COMPROBANTE_EN_NC();
                if( PEDIR_PRECIO_EN_NC ) {//es referenciada 
                    SET_MEMORY_CHAR( __ram_pedir_precio, 1 );
                }
            }
            else {
                error = 1;
                SET_MEMORY_CHAR( __ram_tipo_evento_comp_original, ( char ) - 1 );
            }
        }
        _SET_ESTADO( 3 );
    }
    /*--------------------- Fin de recupero de memoria ----------------------*/
    SET_MEMORY_CHAR( __ram_referencia_nc, ok );
    MUESTRA_COMPROBANTE();
    CLOSE_RECUPERO( _ptr, _handle );
    return( error );
}
/*****************************************************************************/
void PANTALLA_IMPORTAR_NC()
/*****************************************************************************/
{
}
/*****************************************************************************/
int PIDE_DATOS_IMPORTAR_NC()
/*****************************************************************************/
{
    int rta = 0;
    char *cad;
    int cant = 0;
    if( PEDIR_CAJA_Y_SECUENCIA ) {
        cant = strlen( ST( S_CAJA_________ ) ) + strlen( ST( S_TRANSACCION__ ) );
        if( ( cad = ( char* )malloc( cant + 2 ) ) != NULL ) {
            memset( cad, 0, cant + 2 );
            strncat( cad, ST( S_CAJA_________ ), strlen( ST( S_CAJA_________ ) ) );
            strcat( cad, "~" );// "-" sirve de separador en Java
            strncat( cad, ST( S_TRANSACCION__ ), strlen( ST( S_TRANSACCION__ ) ) );
            ExecuteSSAction( A_PEDIR, 21, " ", cad, NULL, " " );         

            ExecuteSSAction( A_PEDIR_DATO, 0, " ", " ", NULL, " " );
            SET_MEMORY_INT( _ticket_caja, PEDIR_ENTERO( 44 - ancho_panel / 2, 9, 4, NO ) );
            if( _TICKET_CAJA > 0 ) {
                ExecuteSSAction( A_PEDIR_DATO, 1, " ", " ", NULL, " " );
                SET_MEMORY_LONG( _ticket_secuencia, PEDIR_ENTERO( 44 - ancho_panel / 2, 10, 8, NO ) );
                if( _TICKET_SECUENCIA > 0 ) {
                    rta = 1;
                }
            }
        }
    }
    else {
        cant = strlen( ST( S_FECHA________ ) ) + strlen( ST( S_CAJA_DGI_____ ) )
             + strlen( ST( S_TICKET_______ ) ) + strlen( ST( S_COMPROBANTE__ ) );
        if( ( cad = ( char* )malloc( cant + 3 ) ) != NULL ) {
            memset( cad, 0, cant + 4 );
            strncat( cad, ST( S_FECHA________ ), strlen( ST( S_FECHA________ ) ) );
            strcat( cad, "~" );// "-" sirve de separador en Java
            strncat( cad, ST( S_CAJA_DGI_____ ), strlen( ST( S_CAJA_DGI_____ ) ) );
            strcat( cad, "~" );
            strncat( cad, ST( S_TICKET_______ ), strlen( ST( S_TICKET_______ ) ) );
            strcat( cad, "~" );
            strncat( cad, ST( S_COMPROBANTE__ ), strlen( ST( S_COMPROBANTE__ ) ) );
            ExecuteSSAction( A_PEDIR, 21, " ", cad, NULL, " " );

            ExecuteSSAction( A_PEDIR_DATO, 0, " ", " ", NULL, " " );
            SET_MEMORY_INT( _ticket_fecha, PEDIR_FECHA( 44 - ancho_panel / 2, 9, NULL ) );
            if( _TICKET_FECHA > 0 ) {
                ExecuteSSAction( A_PEDIR_DATO, 1, " ", " ", NULL, " " );
                SET_MEMORY_INT( _ticket_caja, PEDIR_ENTERO( 44 - ancho_panel / 2, 10, 4, NO ) );
                if( _TICKET_CAJA > 0 ) {
                    ExecuteSSAction( A_PEDIR_DATO, 2, " ", " ", NULL, " " );
                    SET_MEMORY_INT( _ticket_n_ticket, PEDIR_ENTERO( 44 - ancho_panel / 2, 11, 7, NO ) );
                    if( _TICKET_N_TICKET >= 0 ) {
                        ExecuteSSAction( A_PEDIR_DATO, 3, " ", " ", NULL, " " );
                        MENSAJE( "1)Ticket  2)Tick Fac A  3)Tick Fac B  4)Factura A  5)Factura B" );
                        SET_MEMORY_INT( _ticket_tipo_comp,
                                        PEDIR_ENTERO( 44 - ancho_panel / 2, 12, 1, NO ) );
                        if( _TICKET_TIPO_COMP > 0 ) {
                            switch( _TICKET_TIPO_COMP ) {
                                case 1:
                                    SET_MEMORY_INT( _ticket_tipo_comp, 0 );
                                    break;
                                case 2:
                                    SET_MEMORY_INT( _ticket_tipo_comp, 35 );
                                    break;
                                case 3:
                                    SET_MEMORY_INT( _ticket_tipo_comp, 36 );
                                    break;
                                case 4:
                                    SET_MEMORY_INT( _ticket_tipo_comp, 3 );
                                    break;
                                case 5:
                                    SET_MEMORY_INT( _ticket_tipo_comp, 4 );
                                    break;
                            }
                            rta = 1;
                        }
                    }
                }
            }
        }
    }
    ExecuteAction( A_FIN_PEDIR );
    return ( rta );
}
/*****************************************************************************/
int PRECIO_TICKET_ORIGINAL()
/*****************************************************************************/
{
    int rta = 0;
    struct _ev_cont ctrl_nc;
    if( RAM_REFERENCIA_NC ) {
        OPEN_TABLE( T_CTRL_NC, TT_ORIG, ( char* )&ctrl_nc, sizeof( event_e_ticket ) );
		while( !dbEOF() && !rta ) {
            if( ctrl_nc.cod_articulo == ITEM_COD_ESPECIAL ) {
                rta = 1;
            }
            if( !rta ) {
                SKIP2( 1 );
            }
        }
        if( rta ) {
            ASIGNAR_PRECIO_TICKET_ORIGINAL( ctrl_nc );
        }
        CLOSE_TABLE( T_CTRL_NC, TT_ORIG );
    }
    else {
        rta = 1;
    }
    return ( rta );
}
/*****************************************************************************/
int PIDE_TIPO_COMPROBANTE_EN_NC()
/*****************************************************************************/
{
    int error = 0;
    struct _menu items[] = {{"1) BOLETA ", '1', 0},{"2) FACTURA", '2', 3}};
    /*---------------------- Dibuja la pantalla ----------------------------*/
    #if defined(INVEL_W) || defined(INVEL_L)
    struct _cabmenu *cab;
    int columnas;
    columnas = 2;
    cab = ( struct _cabmenu * )calloc( columnas, sizeof( struct _cabmenu ) );
    if( cab != NULL ) {
        cab[0].nombre = "COD";
        cab[0].align = ALIGN_LEFT;
        cab[0].width = 8;
        cab[1].nombre = "DESCRIPCION";
        cab[1].align = ALIGN_LEFT;
        cab[1].width = 20;
        SET_MEMORY_CHAR( __ram_tipo_evento_comp_original,
                         ( char )MENU( " TIPO DE COMP. ANTERIOR ", items, 2, cab, columnas ) );
        free( cab );
    }
    #else
    SET_MEMORY_CHAR( __ram_tipo_evento_comp_original,
                     ( char )MENU( 0, 17, " TIPO DE COMP. ANTERIOR ", items, 2, 120, 52 ) );
    #endif
    if( LASTKEY() == 27 ) {
        error = 1;
    }
    return ( error );
}
/*****************************************************************************/
int PEDIR_PRECIO_NOTA_CR_LIBRE()
/*****************************************************************************/
{
    int rta = 0, i;
    double precio, tasas, iva1 = 0.0, imp[4];
    SET_MEMORY_DOUBLE( __item_precio,
                       PEDIR_FLOAT_EN_RECUADRO_CON_VALOR( " PRECIO ", SI, ENTEROS, DECIMALES,
                                                          ITEM_PRECIO ) );
    if( ITEM_PRECIO > 0 ) {
        SET_MEMORY_DOUBLE( __item_precio, -ITEM_PRECIO );
        rta = 1;
        precio = ITEM_PRECIO;
        if( ITEM_GRAVADO ) {
            if( config.pais == BRASIL ) {
                tasas = iva[ITEM_COD_IVA].porc_iva1 / 100.0
                      + ( ( _ITEM_SUMA_IMPUESTO_AL_TOTAL( 0 ) ? _ITEM_PORC_IMPUESTO( 0 ) : 0 )
                        / 100.0 )
                      + ( ( _ITEM_SUMA_IMPUESTO_AL_TOTAL( 1 ) ? _ITEM_PORC_IMPUESTO( 1 ) : 0 )
                        / 100.0 )
                      + ( ( _ITEM_SUMA_IMPUESTO_AL_TOTAL( 2 ) ? _ITEM_PORC_IMPUESTO( 2 ) : 0 )
                        / 100.0 )
                      + ( ( _ITEM_SUMA_IMPUESTO_AL_TOTAL( 3 ) ? _ITEM_PORC_IMPUESTO( 3 ) : 0 )
                        / 100.0 );
            }
            else {
                tasas = iva[ITEM_COD_IVA].porc_iva1 / 100.0 + _ITEM_PORC_IMPUESTO( 0 ) / 100.0
                      + _ITEM_PORC_IMPUESTO( 1 ) / 100.0 + _ITEM_PORC_IMPUESTO( 2 ) / 100.0
                      + _ITEM_PORC_IMPUESTO( 3 ) / 100.0;
            }
            SET_MEMORY_DOUBLE( __item_precio_sin_iva, precio / ( 1 + tasas ) );
            iva1 = ITEM_PRECIO_SIN_IVA * iva[ITEM_COD_IVA].porc_iva1 / 100.0;
            for( i = 0;i < 4;i++ ) {
                imp[i] = ITEM_PRECIO_SIN_IVA * _ITEM_PORC_IMPUESTO( i ) / 100.0;
            }
        }
        else {
            SET_MEMORY_DOUBLE( __item_precio_sin_iva, precio );
        }
        SET_MEMORY_DOUBLE( __item_iva1_precio, iva1 );
        SET_MEMORY_DOUBLE( __item_iva1_rec, 0.0 );
        SET_MEMORY_DOUBLE( __item_iva2_precio, 0.0 );
        SET_MEMORY_DOUBLE( __item_iva2_rec, 0.0 );
        for( i = 0;i < 4;i++ ) {
            _SET_MEMORY_FLOAT( __item_monto_impuesto, i, imp[i] );
        }
    }
    return( rta );
}
/*****************************************************************************/
int VERIFICA_NC_AUTORIZADAS()
/*****************************************************************************/
{
    int rta = 1;
    if( RAM_NOTA_CR && RAM_PEDIR_PRECIO && NC_PERMANENTE ) {
        rta = SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, NULL, ST( S_PRECIOS_NC ),
                                _F_SUP_PRECIOS_NC, SI );
    }
    return( rta );
}

