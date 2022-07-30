#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <global.h>
#include <tpv_dat.h>
//#include <browse.h> 
#include <btrv.h> 
#include <b_mens.h> 
#include <cmemory.h> 
#include <comprob.h> 
#include <consulta.h> 
#include <cr.h>  
#include <cr_disco.h> 
#include <cr_displ.h> 
#include <cr_pant.h> 
#include <display.h> 
#include <ftoc.h> 
#include <getch.h> 
#include <imp_item.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <m_caract.h> 
#include <m_entero.h> 
#include <pagos.h> 
#include <panel.h> 
#include <pant.h> 
#include <round.h> 
#include <scanner.h> 
#include <stringt.h> 
#include <tkt.h> 
#include <trim.h> 
#include <_cr1.h> 
#include <actions.h>
#include <ctype.h>
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#define CONSULTA_CODIGO_BARRA 0
#define CONSULTA_DESCRIPCION 1
#define CONSULTA_CODIGO_INTERNO 2
#define CONSULTA_MARCA 3
static char precio_en_me;
extern  int ES_IMPRIMIBLE(int dato);
extern int BLOQUEAR_DATOS;
extern void CR1_MODIFICAR_MOSTRAR_ITEM_( int pos, char *signo, char *cantidad, char *cinterno, char *cbarra, char *nombre,
                            char *precio );
int extremo;
//int cntEnter = -1; // contador de enter
#if defined(INVEL_W) || defined(INVEL_L)
/*****************************************************************************/
void CR1_MOSTRAR_ITEM_WIN( int item )
/*****************************************************************************/
{
    char cod_barra[17];
    char cant[15],cinterno[15],pr[15], /*ss[5], */desc[31]/*, prueba[20]*/;
    double precio;
    if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
        memcpy( cod_barra, _TICKET_COD_BARRA( item ), 17 );
        BUSCA_COD_INTERNO_COD_BARRA( _TICKET_COD_ESPECIAL( item ), cod_barra );
    }
    else {
        BUSCA_CODIGO_ESPECIAL( _TICKET_COD_ESPECIAL( item ) );
        if( config.pais == ELSALVADOR ) {
            SELECT_TABLE( T_ARTICULO, TT_ORIG );
			SET_WHERE("CODIGO_BARRA = '%s'",_TICKET_COD_BARRA( item ));
			RUN_QUERY(NO);
            //GET_EQUAL( _TICKET_COD_BARRA( item ) );
        }
    }        
    memcpy( cod_barra, _TICKET_COD_BARRA( item ), 17 );
    precio = ( _TICKET_IMPORTE_SIN_IVA( item ) + _TICKET_RECARGO( item ) ) * _TICKET_CANTIDAD( item );
    FTOC( _TICKET_CANTIDAD( item ), cant, 4, 2, 0, 0 );
    SET_ENTERO( "000000000", _TICKET_COD_ESPECIAL( item ), cinterno );
    FTOC( precio, pr, ENTEROS, DECIMALES, 1, 0 );
    strcpy( desc, articulo.nombre );
    ExecuteSAction( A_MUESTRA_DESCRIPCION, desc );
    ExecuteSAction( A_MUESTRA_CANTIDAD, cant );
    ExecuteSAction( A_MUESTRA_SUBTOTAL, pr );
    ExecuteSAction( A_MUESTRA_IMG_ART, cod_barra );
    __CR1_MUESTRA_ARTICULO_EN_GRANDE( item );
}
#endif
/*****************************************************************************/
void CR1_MOSTRAR_ITEM( int item, int y, int modificar )
/*****************************************************************************/
{
    char signo,cod_barra[17];
	char des_art[50];
    int x1;
    double precio,cantidad;
    char decimales = DECIMALES;
    x1 = 17 - L_CODIGO;
	if( IMPRESORA_FISCAL == FISCAL && ITEM_PEDIR_PRECIO == 1 && fabs(_TICKET_EXENTO( item))<0.009 && (RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1 )) {
        double precio_pantalla = 0.00;
        precio_pantalla = _TICKET_IMPORTE_SIN_IVA( item ) - _TICKET_IMP_INT(item );
		ROUND2(&precio_pantalla, ENTEROS, 4 );
		precio = _TICKET_IMP_INT(item ) + (( precio_pantalla + _TICKET_RECARGO( item )) 
	             * _TICKET_CANTIDAD( item ))* (  1 + iva[ _TICKET_COD_IVA( item )].porc_iva1 / 100.0 )  ;
	}else	
		precio = ( _TICKET_PRECIO( item ) + _TICKET_RECARGO( item ) ) * _TICKET_CANTIDAD( item ); //normal
    
    if( /*MODO_FACTURACION_ESPECIAL(  ) || */MOSTRAR_SIN_IVA_ELSALVADOR()
     || MOSTRAR_PRECIO_SIN_IVA_EN_VENTAS ) {
        precio -= ( ( _TICKET_IVA1_PRECIO( item ) + _TICKET_IVA2_PRECIO( item )
                    + _TICKET_IVA1_REC( item ) + _TICKET_IVA2_REC( item ) )
                  * _TICKET_CANTIDAD( item ) );
    }
    if( buscar_articulo_en_mostrar_item ) {
        if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
            memcpy( cod_barra, _TICKET_COD_BARRA( item ), 17 );
            BUSCA_COD_INTERNO_COD_BARRA( _TICKET_COD_ESPECIAL( item ), cod_barra );
        }
        else {
            if( config.pais == ELSALVADOR ) {
				BUSCA_CODIGO_BARRA( _TICKET_COD_BARRA( item ) );
                /*SELECT_TABLE( T_ARTICULO, TT_ORIG );
				SET_WHERE("CODIGO_BARRA = '%s'",_TICKET_COD_BARRA( item ));
				RUN_QUERY(NO);*/
                /*while( !( strcmp( _TICKET_COD_BARRA( item ), articulo.cod_barra ) == 0 )
                    && !dbEOF() ) {
                    SKIP( 1 );
                }*/
			} else {
				BUSCA_CODIGO_ESPECIAL( _TICKET_COD_ESPECIAL( item ) );
			}
        }
    }
    // controlo el pais, aunque deberia chequearse en otros lados donde usen scanner balanza porque muestra una cantidad que no es real en los items pesados en el scanner balanza que esta configurado en libras.
    cantidad = ( _TICKET_TIPO3(item) & _PESABLE /*ITEM_PESABLE*/ && RAM_CANTIDAD_ORIGINAL && config.pais != ELSALVADOR )
             ? RAM_CANTIDAD_ORIGINAL : _TICKET_CANTIDAD( item );
    y += 8;
    signo = ( _TICKET_TIPO( item ) & _RESTA ) ? '-' : ' ';
    MOSTRAR_CARACTER( 2, y, signo );
    //fabiana 15-02-05 ** nuevo manejo de redondeos--para factura o tickets donde se discrimina iva
    if( ( DECIMALES < 4 )
     && ( ES_FACTURA_A( clientes.cond_iva ) || config.pais == ELSALVADOR
       || MOSTRAR_SIN_IVA_ELSALVADOR() || MOSTRAR_PRECIO_SIN_IVA_EN_VENTAS ) ) {
        decimales = DECIMALES;
    }
	if( !ancho_panel ) {
        #if defined(INVEL_W) || defined(INVEL_L)
        char cant[15],cinterno[15],pr[15],ss[5],desc[31]/*, prueba[20]*/;
        //ExecuteAction(A_ACTIVAR_PANEL_VENTAS);
        MOSTRAR_INPUT_CADENA( "      ", 0 );
		FTOC( ( _TICKET_COD_COBRO( item ) ) ? 1 : _TICKET_CANTIDAD( item ), cant, 4, 3, 0, 0 );
        SET_ENTERO( "000000000", _TICKET_COD_ESPECIAL( item ), cinterno );
        FTOC( precio, pr, ENTEROS, DECIMALES, 1, 1 );
        sprintf( ss, "%c", signo );
		if( modificar == SI )
			CR1_MODIFICAR_MOSTRAR_ITEM_( item ,ss, cant, cinterno, articulo.cod_barra, articulo.nombre, pr );
		else
		if( PRESENTACIONES_HABILITADAS && _TICKET_COD_PRESENT( item ) && _TICKET_PRESENTACION( item ) && TIPO_PRESENTACIONES != 1 )
		{
			_snprintf(des_art, sizeof(articulo),"%s Px %d", articulo.nombre, _TICKET_PRESENTACION( item )/*MASCARA( '9', 3 )*/);
			des_art[sizeof( des_art ) - 1] = '\0';
        } 
		else
		{
			_snprintf(des_art, sizeof(des_art),"%s", articulo.nombre );
			des_art[sizeof( des_art ) - 1] = '\0';
		}
		if(des_art == NULL) {
			BUSCA_CODIGO_BARRA( _TICKET_COD_BARRA( item ) );
			_snprintf(des_art, sizeof(des_art),"%s", articulo.nombre );
			des_art[sizeof( des_art ) - 1] = '\0';
		}
		
		CR1_MOSTRAR_ITEM_( ss, cant, cinterno, articulo.cod_barra, des_art, pr );
		strcpy( desc, articulo.nombre );
		if( desc == NULL) {
			BUSCA_CODIGO_BARRA( _TICKET_COD_BARRA( item ) );
			strncpy( desc, articulo.nombre, 31 );
			desc[sizeof( desc ) - 1] = '\0';
		} else 
			ExecuteSAction( A_MUESTRA_DESCRIPCION, desc );
        ExecuteSAction( A_MUESTRA_CANTIDAD, cant );
        if( RAM_MODO_EJECUCION != INVENTARIO )
        ExecuteSAction( A_MUESTRA_SUBTOTAL, pr );
        #endif
/*
        switch( FORMATO_PANTALLA ) {
            case 0:
                //-------- Normal para codigos de 9 dgitos ----------
                MOSTRAR_CADENA( x1, y, articulo.cod_barra );
                MOSTRAR_ENTERO( 18 - ancho_panel, y, "000000000", _TICKET_COD_ESPECIAL( item ) );
                MOSTRAR_CANTIDAD( 28 - ancho_panel, y, cantidad );
                MOSTRAR_CADENA( 36 - ancho_panel, y, articulo.nombre );
                MOSTRAR_ENTERO( 67 - ancho_panel, y, "999", _TICKET_PRESENTACION( item ) );
                MOSTRAR_FLOTANTE( 71 - ancho_panel, y, ENTEROS, decimales, 1, 0, precio );
                break;
            case 1:
                //-------- Normal para codigos de 6 dgitos ----------
                MOSTRAR_CADENA( x1 + 1, y, articulo.cod_barra );
                MOSTRAR_ENTERO( 19 - ancho_panel, y, "000000", _TICKET_COD_ESPECIAL( item ) );
                MOSTRAR_CANTIDAD( 26 - ancho_panel, y, cantidad );
                MOSTRAR_CADENA( 34 - ancho_panel, y, articulo.nombre );
                MOSTRAR_ENTERO( 65 - ancho_panel, y, "999", _TICKET_PRESENTACION( item ) );
                if( config.pais != CHILE ) {
                    MOSTRAR_FLOTANTE( 69 - ancho_panel, y, ENTEROS + 1, decimales + 1, 1, 1,
                                      precio );
                }
                else {
                    MOSTRAR_FLOTANTE( 69 - ancho_panel, y, ENTEROS + 1, decimales + 1, 1, 0,
                                      precio );
                }
                break;
            case 2:
                //-------- Para GRANDIET ----------
                MOSTRAR_CADENA( x1 + 1, y, articulo.cod_barra );
                MOSTRAR_ENTERO( 20 - ancho_panel, y, "0000", _TICKET_COD_ESPECIAL( item ) );
                MOSTRAR_CANTIDAD( 26 - ancho_panel, y, cantidad );
                MOSTRAR_CADENA( 34 - ancho_panel, y, articulo.nombre );
                if( config.pais == ELSALVADOR ) {
                    MOSTRAR_ENTERO( 64 - ancho_panel, y, "999", _TICKET_PRESENTACION( item ) );
                    MOSTRAR_FLOTANTE( 68 - ancho_panel, y, ENTEROS + 1, decimales, 1, 1, precio );
                }
                else {
                    MOSTRAR_ENTERO( 65 - ancho_panel, y, "999", _TICKET_PRESENTACION( item ) );
                    if( config.pais != CHILE ) {
                        MOSTRAR_FLOTANTE( 69 - ancho_panel, y, ENTEROS + 1, decimales, 1, 1,
                                          precio );
                    }
                    else {
                        MOSTRAR_FLOTANTE( 69 - ancho_panel, y, ENTEROS + 1, decimales, 1, 0,
                                          precio );
                    }
                }
                break;
            case 3:
               //-------- Para Falabella ----------
                MOSTRAR_CADENA( x1 + 1, y, articulo.cod_barra );
                MOSTRAR_CANTIDAD( 26 - ancho_panel, y, cantidad );
                MOSTRAR_CADENA( 34 - ancho_panel, y, articulo.nombre );
                if( fabs( precio ) > 0.004 ) {
                    if( config.pais != CHILE ) {
                        MOSTRAR_FLOTANTE( 69 - ancho_panel, y, ENTEROS + 1, decimales, 1, 1,
                                          precio );
                    }
                    else {
                        MOSTRAR_FLOTANTE( 69 - ancho_panel, y, ENTEROS + 1, decimales, 1, 0,
                                          precio );
                    }
                }
                break;
        }*/
    }
    else {

    /*    switch( FORMATO_PANTALLA ) {
            case 0:
                //-------- Normal para codigos de 9 dgitos ----------
                MOSTRAR_CADENA( 3, y, articulo.cod_barra );
                MOSTRAR_CANTIDAD( 28 - ancho_panel, y, cantidad );
                MOSTRAR_CADENA( 36 - ancho_panel, y, articulo.nombre );
                MOSTRAR_ENTERO( 67 - ancho_panel, y, "999", _TICKET_PRESENTACION( item ) );
                MOSTRAR_FLOTANTE( 71 - ancho_panel, y, ENTEROS, decimales, 1, 0, precio );
                break;
            case 1:
                //-------- Normal para codigos de 6 dgitos ----------
                MOSTRAR_CADENA( 3, y, articulo.cod_barra );
                MOSTRAR_CANTIDAD( 26 - ancho_panel, y, cantidad );
                MOSTRAR_CADENA( 34 - ancho_panel, y, articulo.nombre );
                MOSTRAR_ENTERO( 65 - ancho_panel, y, "999", _TICKET_PRESENTACION( item ) );
                if( config.pais != CHILE ) {
                    MOSTRAR_FLOTANTE( 69 - ancho_panel, y, ENTEROS + 1, decimales + 1, 1, 1,
                                      precio );
                }
                else {
                    MOSTRAR_FLOTANTE( 69 - ancho_panel, y, ENTEROS + 1, decimales + 1, 1, 0,
                                      precio );
                }
                break;
            case 2:
                //-------- Para GRANDIET ----------
                MOSTRAR_CADENA( 3, y, articulo.cod_barra );
                MOSTRAR_CANTIDAD( 26 - ancho_panel, y, cantidad );
                MOSTRAR_CADENA( 34 - ancho_panel, y, articulo.nombre );
                if( config.pais == ELSALVADOR ) {
                    MOSTRAR_ENTERO( 64 - ancho_panel, y, "999", _TICKET_PRESENTACION( item ) );
                    MOSTRAR_FLOTANTE( 68 - ancho_panel, y, ENTEROS + 1, DECIMALES, 1, 1, precio );
                }
                else {
                    MOSTRAR_ENTERO( 65 - ancho_panel, y, "999", _TICKET_PRESENTACION( item ) );
                    if( config.pais != CHILE ) {
                        MOSTRAR_FLOTANTE( 69 - ancho_panel, y, ENTEROS + 1, decimales, 1, 1,
                                          precio );
                    }
                    else {
                        MOSTRAR_FLOTANTE( 69 - ancho_panel, y, ENTEROS + 1, decimales, 1, 0,
                                          precio );
                    }
                }
                break;
            case 3:
                //-------- Para Falabella ----------
                MOSTRAR_CADENA( 3, y, articulo.cod_barra );
                MOSTRAR_CANTIDAD( 26 - ancho_panel, y, cantidad );
                MOSTRAR_CADENA( 34 - ancho_panel, y, articulo.nombre );
                if( fabs( precio ) > 0.004 ) {
                    if( config.pais != CHILE ) {
                        MOSTRAR_FLOTANTE( 69 - ancho_panel, y, ENTEROS + 1, decimales, 1, 1,
                                          precio );
                    }
                    else {
                        MOSTRAR_FLOTANTE( 69 - ancho_panel, y, ENTEROS + 1, decimales, 1, 0,
                                          precio );
                    }
                }
                break;
        }*/
    }
} 
/*****************************************************************************/
void CR1_MOSTRAR_DATOS( int y, int vacio, int que_consulta, struct _browse *b )
/*****************************************************************************/
{
    int pos = 0;
    char xx[20],*coma;
    if( !vacio ) {
        CALCULA_ARTICULO_EXT();
        if( !ancho_panel ) {
            if( que_consulta == CONSULTA_CODIGO_BARRA ) {
                char xx[20];
                setFieldValuepChar( b->index, 0, articulo.cod_barra );
                sprintf( xx, "%09ld", articulo.cod_interno );
                setFieldValuepChar( b->index, 1, xx );
                setFieldValuepChar( b->index, 2, articulo.nombre );
                setFieldValuepChar( b->index, 3, articulo.unidad );
                setFieldValuepChar( b->index, 4, articulo.marca );
                sprintf( xx, "%*.*f", ENTEROS + 1, DECIMALES, 
                            ( MOSTRAR_SIN_IVA_ELSALVADOR( ) 
                                || MOSTRAR_PRECIO_SIN_IVA_EN_VENTAS )? 
                                    articulo.precio_sin_iva : articulo_ext.precio );
                setFieldValuepChar( b->index, 5, xx );
            }
            else {
                switch( FORMATO_CONSULTA ) {
                    case 0:
                        setFieldValuepChar( b->index, 0, articulo.cod_barra );
                        sprintf( xx, "%09ld", articulo.cod_interno );
                        setFieldValuepChar( b->index, 1, xx );
                        setFieldValuepChar( b->index, 2, articulo.marca );
                        setFieldValuepChar( b->index, 3, articulo.nombre );
                        setFieldValuepChar( b->index, 4, articulo.unidad );
                        sprintf( xx, "%*.*f", ENTEROS + 1, DECIMALES, 
                            ( MOSTRAR_SIN_IVA_ELSALVADOR( ) 
                                || MOSTRAR_PRECIO_SIN_IVA_EN_VENTAS )? 
                                    articulo.precio_sin_iva : articulo_ext.precio );
                        setFieldValuepChar( b->index, 5, xx );
                        break;
                    case 1:
                        setFieldValuepChar( b->index, pos++, articulo.cod_barra );
                        sprintf( xx, "%09ld", articulo.cod_interno );
                        setFieldValuepChar( b->index, pos++, xx );
                        setFieldValuepChar( b->index, pos++, articulo.nombre );
                        if( config.pais != CHILE ) {
                            sprintf( xx, "%*.*f", ENTEROS + 1, DECIMALES, articulo_ext.precio );
                            coma = strchr( xx, '.' );
                            *coma = ',';
                            setFieldValuepChar( b->index, pos++, xx );
                            sprintf( xx, "%*.*f", ENTEROS + pos++, DECIMALES,
                                     articulo.precio_sin_iva );
                            coma = strchr( xx, '.' );
                            *coma = ',';
                            setFieldValuepChar( b->index, pos, xx );
                        }
                        else {
                            sprintf( xx, "%*.*f", ENTEROS + 1, DECIMALES, articulo_ext.precio );
                            setFieldValuepChar( b->index, pos++, xx );
                            sprintf( xx, "%*.*f", ENTEROS + 1, DECIMALES, articulo.precio_sin_iva );
                            setFieldValuepChar( b->index, pos, xx );
                        }
                        break;
                    case 2:
                        setFieldValuepChar( b->index, pos++, articulo.cod_barra );
                        sprintf( xx, "%09ld", articulo.cod_interno );
                        setFieldValuepChar( b->index, pos++, xx );
                        setFieldValuepChar( b->index, pos++, articulo.nombre );
                        setFieldValuepChar( b->index, pos++, articulo.contenido );
                        setFieldValuepChar( b->index, pos++, articulo.unidad );
                        sprintf( xx, "%*.*f", ENTEROS + 1, DECIMALES, 
                            ( MOSTRAR_SIN_IVA_ELSALVADOR( ) 
                                || MOSTRAR_PRECIO_SIN_IVA_EN_VENTAS )? 
                                    articulo.precio_sin_iva : articulo_ext.precio );
                        setFieldValuepChar( b->index, pos, xx );
                        break;
                    case 3:
                        setFieldValuepChar( b->index, pos++, articulo.cod_barra );
                        sprintf( xx, "%09ld", articulo.cod_interno );
                        setFieldValuepChar( b->index, pos++, xx );
                        setFieldValuepChar( b->index, pos++, articulo.marca );
                        setFieldValuepChar( b->index, pos++, articulo.nombre );
                        setFieldValuepChar( b->index, pos++, articulo.contenido );
                        setFieldValuepChar( b->index, pos++, articulo.unidad );
                        sprintf( xx, "%*.*f", ENTEROS + 1, DECIMALES, 
                            ( MOSTRAR_SIN_IVA_ELSALVADOR( ) 
                                || MOSTRAR_PRECIO_SIN_IVA_EN_VENTAS )? 
                                    articulo.precio_sin_iva : articulo_ext.precio );
                        setFieldValuepChar( b->index, pos, xx );
                        break;
                    case 4:
                        setFieldValuepChar( b->index, pos++, articulo.cod_barra );
                        sprintf( xx, "%09ld", articulo.cod_interno );
                        setFieldValuepChar( b->index, pos++, xx );
                        setFieldValuepChar( b->index, pos++, articulo.marca );
                        setFieldValuepChar( b->index, pos++, articulo.nombre );
                        setFieldValuepChar( b->index, pos++, articulo.unidad );
                        sprintf( xx, "%*.*f", ENTEROS + 1, DECIMALES, 
                            ( MOSTRAR_SIN_IVA_ELSALVADOR( ) 
                                || MOSTRAR_PRECIO_SIN_IVA_EN_VENTAS )? 
                                    articulo.precio_sin_iva : articulo_ext.precio );
                        setFieldValuepChar( b->index, pos, xx );
                        break;
                }
            }
        }
    } 
}
/*****************************************************************************/
int _consulta_cod_barra( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0, h, l;
    char *p;
    int pos[] = {4,5,4,2,3}, dif;
    if( ancho_panel ) {
        pos[0] = 4;
        pos[1] = 5;
        pos[2] = 4;
        pos[3] = 2;
        pos[4] = 3;
    }
    else {
        pos[0] = 4;
        pos[1] = 5;
        pos[2] = 4;
        pos[3] = 3;
        pos[4] = 3;
    }
    dif = L_CODIGO - 13;
    switch( comando ) {
        case _BUSCAR:
            //cntEnter = -1; //reinicia el contador de enter
            //MOSTRAR_CADENA( pos[FORMATO_CONSULTA] - dif, 21, cadena );
			MOSTRAR_INPUT_CADENA( cadena, b->pos );
			SET_WHERE("CODIGO_BARRA >= '%s'",cadena);
			SET_ORDER2("CODIGO_BARRA");
			RUN_QUERY(NO);
            //GET_GREATER_E( cadena );
            break;
        case _MOSTRAR:
            CR1_MOSTRAR_DATOS( dato, dbEOF(), CONSULTA_CODIGO_BARRA, b );
            break;
        case _VACIO:
            CR1_MOSTRAR_DATOS( dato, SI, CONSULTA_CODIGO_BARRA, b );
            break;
        case _SKIP:
            //cntEnter = -1; //reinicia el contador de enter
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
            break;
        case _VALIDAR:
            if( dato >= '0' && dato <= '9' ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _COMPARAR:
            if( strcmp( cadena, articulo.cod_barra ) == 0 ) {
                rta = 1;
            }
            if( USAR_DISP_EN_BUSQUEDA_SCANNER )
                                                #ifdef COMPILAR_DISPLAY
            {
                BORRAR_DISPLAY();
            }
            #endif
            if( b->pos == L_CODIGO ) {
                if( rta ) {
                    if( USAR_DISP_EN_BUSQUEDA_SCANNER ) {
                        CALCULA_ARTICULO_EXT();
                        #ifdef COMPILAR_DISPLAY
                        MOSTRAR_EN_DISPLAY( articulo.nombre, articulo_ext.precio );
                        #endif
                    }
                }
                else {
                    MENSAJE_STRING( S_CODIGO_INEXISTENTE );
                }
            }
            break;
        case _ESPECIAL:
            /*------------- Lee Scanner ----------------*/
            if( VARIOS_SCANNER_HABILITADO ) {
                if( IS_SCANNER() ) {
                    memset( cadena, ' ', L_CODIGO );
                    cadena[L_CODIGO] = 0;
                    if( LEER_SCANNER( cadena, L_CODIGO + 1 ) ) {
						BEEP( 2000, 1000 );
                        p = strchr( cadena, ' ' );
                        if( p ) {
                            *p = 0;
                        }
                        l = strlen( cadena );
						if( !( PRESENTACIONES_EN_DUN14 && L_CODIGO > 13 ) ) 
						{
							memmove( &cadena[L_CODIGO - l], cadena, l + 1 );
							//tarq movmem( cadena, &cadena[L_CODIGO - l], l + 1 );
							for( h = 0;h < L_CODIGO - l;h++ ) {
								cadena[h] = '0';
							}
							b->pos = L_CODIGO;
						}
                    }
                    else 
					{
                        MENSAJE_STRING( S_ERROR_LECTURA_SCANNER );
                        memset( cadena, ' ', L_CODIGO );
                        b->pos = 0;
                    }
                    b->posicionar = 1;
                    cadena[L_CODIGO] = 0;
                    SET_MEMORY( __ram_cod_barra, cadena );
                    if( PRESENTACIONES_EN_DUN14 ) {
					   EVALUAR_CODIGOS_ESPECIALES( L_CODIGO - 1 );
					} else {
					   EVALUAR_CODIGOS_ESPECIALES( L_CODIGO );
					}
                    if( usar_cadena_scanner ) {
                        usar_cadena_scanner = 0;
                        memcpy( cadena, RAM_COD_BARRA, L_CODIGO );
                    }
                }
            }
            break;
        case _ENTER:
            //cntEnter++;
            if(HABILITAR_ENTER_EN_BUSQUEDA /*&& cntEnter*/){ //si esta habilitado el enter en las busquedas y es el segundo enter
                rta = 1;
            }else{
                rta = 0;
            }
            break;
        case _TECLA:
           if( !HABILITAR_ENTER_EN_BUSQUEDA && dato == 43 ){
				//funciona como el ENTER para cuando no esta permitido.
				b->seguir = 0;//corte el ciclo del browser
				rta=1;//final
				break;
			}
            MUESTRA_PROMOCIONES_ARTICULO( dato );
            break;
		case _LIMIT:
            if( dato > 0 ) {
                SET_LIMIT( dato, SI );
            } else {
                SET_LIMIT( NO, NO );
            }
            break;
    }
    return ( rta );
}
/*****************************************************************************/
void MUESTRA_PROMOCIONES_ARTICULO( int dato )
/*****************************************************************************/
{
    #ifdef COMPILAR_CONSULTA_ARTICULO
    int pant;
    char *consulta_art_str = "C O N S U L T A   D E   A R T I C U L O S";
    if( dato == 162 ) {
        MOSTRAR_CADENA( ( 80 - ancho_panel - strlen( consulta_art_str ) ) / 2, 3,
                        consulta_art_str /* S_CONSULTA_ARTICULO */ );
        MOSTRAR_DATOS_ARTICULO_CON_PROMOCIONES();
        MENSAJE_TECLA_STRING( S_PRESIONE_TECLA_CONSULTA );
        //SELECT( AREA_ARTICULOS );
		SELECT_TABLE( T_ARTICULO, TT_ORIG );
    }
    #endif
}
/*****************************************************************************/
int _consulta_descripcion( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0;
    int pos[] = {44,26,26,43,38};
    int largo[] = {20,30,25,25,30};
    if( ancho_panel ) {
        pos[0] = 44 - ancho_panel - 1;
        pos[1] = 26 - ancho_panel - 1;
        pos[2] = 26 - ancho_panel - 1;
        pos[3] = 37 - ancho_panel - 1;
        pos[4] = 38 - ancho_panel - 1;
    }
    switch( comando ) {
        case _BUSCAR:
            if( ancho_panel ) {
                MOSTRAR_CADENA( pos[FORMATO_CONSULTA] + 8, 21, cadena );
            }
            else {
                MOSTRAR_CADENA( pos[FORMATO_CONSULTA], 21, cadena );
            }
			SET_WHERE("NOMBRE LIKE '%s%%' ",cadena);
			SET_ORDER2("NOMBRE");
			RUN_QUERY(NO);
            //rta = GET_GREATER_E( cadena );
            break;
        case _MOSTRAR:
            CR1_MOSTRAR_DATOS( dato, dbEOF(), CONSULTA_DESCRIPCION, b );
            break;
        case _VACIO:
            CR1_MOSTRAR_DATOS( dato, SI, CONSULTA_DESCRIPCION, b );
            break;
        case _SKIP:
            /*if ( abs( dato ) >= b->alto && b->alto != 0 ) {
                //salto de pagina ( una pagina cada vez que supere el limite de la pagina )
                int pages = (int) floor( dato / b->alto );
                SKIP_PAGE( pages );
            } else {*/
                SKIP2( dato );
            //}
            rta = dbEOF() || dbBOF();
            break;
        case _VALIDAR:
            if( ES_IMPRIMIBLE( dato ) ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
            rta = HABILITAR_ENTER_EN_BUSQUEDA;
            break;
        case _TECLA:
           if( !HABILITAR_ENTER_EN_BUSQUEDA && dato == 43 ){
				//funciona como el ENTER para cuando no esta permitido.
				b->seguir = 0;//corte el ciclo del browser
				rta=1;//final
				break;
			}
            MUESTRA_PROMOCIONES_ARTICULO( dato );
            break;
		case _LIMIT:
            if( dato > 0 ) {
                SET_LIMIT( dato, SI );
            } else {
                SET_LIMIT( NO, NO );
            }
            break;
    }
    return ( rta );
}
/*****************************************************************************/
int _consulta_cod_interno( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0;
    long cod;
    int pos[] = {18,19,19,17,17};
    switch( comando ) {
        case _BUSCAR:
            if( ancho_panel ) {
                MOSTRAR_CADENA( pos[FORMATO_CONSULTA] - 1, 21, cadena );
            }
            else {
                MOSTRAR_CADENA( pos[FORMATO_CONSULTA], 21, cadena );
            }
            cod = atol( cadena );
            //GET_GREATER_E( ( char* )&cod );
			SET_WHERE("COD_INTERNO >= '%ld'",cod);
			SET_ORDER2("COD_INTERNO");
			RUN_QUERY(NO);
            break;
        case _MOSTRAR:
            CR1_MOSTRAR_DATOS( dato, dbEOF(), CONSULTA_CODIGO_INTERNO, b );
            break;
        case _VACIO:
            CR1_MOSTRAR_DATOS( dato, SI, CONSULTA_CODIGO_INTERNO, b );
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
            break;
        case _VALIDAR:
            if( dato >= '0' && dato <= '9' ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _COMPARAR:
            if( ( unsigned )atol( cadena ) == articulo.cod_interno && cadena[0] != ' ' ) {
                rta = 1;
            }
            break;
        case _ENTER:
            rta = HABILITAR_ENTER_EN_BUSQUEDA;
            break;
        case _TECLA:
           if( !HABILITAR_ENTER_EN_BUSQUEDA && dato == 43 ){
				//funciona como el ENTER para cuando no esta permitido.
				b->seguir = 0;//corte el ciclo del browser
				rta=1;//final
				break;
			}
			 MUESTRA_PROMOCIONES_ARTICULO( dato );
            break;
		case _LIMIT:
            if( dato > 0 ) {
                SET_LIMIT( dato, SI );
            } else {
                SET_LIMIT( NO, NO );
            }
            break;
    }
    return ( rta );
}
/*****************************************************************************/
int _consulta_marca( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0;
    int pos[] = {28,28,28,27,22};
    if( ancho_panel ) {
        pos[0] = 28;
        pos[1] = 28;
        pos[2] = 28;
        pos[3] = 17;
        pos[4] = 22;
    }
    switch( comando ) {
        case _BUSCAR:
            if( ancho_panel ) {
                MOSTRAR_CADENA( pos[FORMATO_CONSULTA] - 1, 21, cadena );
            }
            else {
                MOSTRAR_CADENA( pos[FORMATO_CONSULTA], 21, cadena );
            }
            //GET_GREATER_E( cadena );
			SET_WHERE("MARCA LIKE '%s%%'",cadena);
			SET_ORDER2("MARCA");
			RUN_QUERY(NO);
            break;
        case _MOSTRAR:
            CR1_MOSTRAR_DATOS( dato, dbEOF(), CONSULTA_MARCA, b );
            break;
        case _VACIO:
            CR1_MOSTRAR_DATOS( dato, SI, CONSULTA_MARCA, b );
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
            break;
        case _VALIDAR:
            if( ES_IMPRIMIBLE( dato ) ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
            rta = HABILITAR_ENTER_EN_BUSQUEDA;
            break;
        case _TECLA:
           if( !HABILITAR_ENTER_EN_BUSQUEDA && dato == 43 ){
				//funciona como el ENTER para cuando no esta permitido.
				b->seguir = 0;//corte el ciclo del browser
				rta=1;//final
				break;
			}
            MUESTRA_PROMOCIONES_ARTICULO( dato );
            break;
		case _LIMIT:
            if( dato > 0 ) {
                SET_LIMIT( dato, SI );
            } else {
                SET_LIMIT( NO, NO );
            }
            break;
    }
    return ( rta );
}
/*****************************************************************************/
_consulta_cod_cobro( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
	int       rta = 0;
//	long      cod;
	int       pos[] = { 18, 19, 19, 17, 17 };

	switch ( comando ) {
	case _BUSCAR:
		if( ancho_panel )
			MOSTRAR_CADENA( pos[FORMATO_CONSULTA] - 1, 21, cadena );
		else
			MOSTRAR_CADENA( pos[FORMATO_CONSULTA], 21, cadena );
		//cod = atol( cadena );
		//GET_GREATER_E( ( char * ) &cod );
        /*SET_WHERE("CODIGO >= '%ld'",cod);
		SET_ORDER2("CODIGO");*/
		SET_WHERE("CODIGO_BARRA >= '%s' AND COD_COBRO > 0 ",cadena);
		SET_ORDER2("CODIGO_BARRA");
		RUN_QUERY(NO);
		break;
	case _MOSTRAR:
	   CR1_MOSTRAR_DATOS( dato, dbEOF()/*BtrvEOF(  )*/, CONSULTA_CODIGO_INTERNO, b );
		break;
	case _VACIO:
		CR1_MOSTRAR_DATOS( dato, SI, CONSULTA_CODIGO_INTERNO, b );
		break;
	//LLENAR_ATRIBUTO no existe mas?
    /*case _RESALTAR:
		if( ancho_panel )
			LLENAR_ATRIBUTO( pos[FORMATO_CONSULTA] - 1, 4 + dato,
				pos[FORMATO_CONSULTA] + L_COD_INTERNO - 2, 4 + dato, 7 );
		else
			LLENAR_ATRIBUTO( pos[FORMATO_CONSULTA] - 1, 4 + dato,
				pos[FORMATO_CONSULTA] + L_COD_INTERNO, 4 + dato, 7 );
		break;*/
	case _SKIP:
        {
            int cant;           
            int dir = ( dato > 0 ) ? 1 : -1;
            for( cant = abs(dato); cant > 0; cant-- ){
                SKIP2( dir );
                if ( rta = ( dir == -1 ) ? /*BOF(  ) : BtrvEOF(  )*/ dbBOF() : dbEOF() ) {
                    break;
                }
            }
        }
		break;
	case _VALIDAR:
		if( dato >= '0' && dato <= '9' )
			rta = 1;
		break;
	case _EXTREMO:
		rta = ( dato == -1 ) ? /*BOF(  ) : BtrvEOF(  )*/ dbBOF() : dbEOF();
		break;
	case _COMPARAR:
		if( atol( cadena ) == articulo.cod_interno && cadena[0] != ' ' )
			rta = 1;
		break;
	case _ENTER:
		rta = HABILITAR_ENTER_EN_BUSQUEDA;
		break;
	case _TECLA:
           if( !HABILITAR_ENTER_EN_BUSQUEDA && dato == 43 ){
				//funciona como el ENTER para cuando no esta permitido.
				b->seguir = 0;//corte el ciclo del browser
				rta=1;//final
				break;
			}
		MUESTRA_PROMOCIONES_ARTICULO( dato );
		break;
   case _SET_REGISTROS:
      b->registros = extremo;
      break;
   case _SET_MODO:
      b->modo = 1;
      break;
		case _LIMIT:
            if( dato > 0 ) {
                SET_LIMIT( dato, SI );
            } else {
                SET_LIMIT( NO, NO );
            }
            break;
	}
	return ( rta );
}

/*****************************************************************************/
int CR1_CONSULTA( int indice )
/*****************************************************************************/
{
    int rta = 0/*, pant*/ ,autorizo_consulta = 1;
	char perm_scanner_ant = VARIOS_SCANNER_HABILITADO;
    /*int       j;*/
    /*char      aux[100];*/
	BLOQUEAR_DATOS = SI;
	if( SUPERVISOR_EN_CONSULTA_DE_ARTICU ) {
		 /*-------------- Verifica si pedir supervisor ----------------*/
		if( !( SOLICITAR_CLAVES( 30, 20, NO, SI, SI, ST( S__AUTORIZACION_ ), "Autoriz¢ ",
					RAM_COD_FUNCION ) ) ) {
			autorizo_consulta = 0;
			MENSAJE_STRING( S_NO_SE_AUTORIZA_CONSULTA );
		}
	} else
		autorizo_consulta = 1;

	if( autorizo_consulta ) {
		if( VARIOS_SCANNER_HABILITADO && indice != 1 ) { //Si la consulta no es por cod. de barra deshabilito el scanner
			CR1_HABILITA_SCANNER( NO );
		}
		if( !RAM_P_TICKET ) {
			DEMORA_SET();
			INICIALIZAR_DEMORA_SET( NO );
			ENABLE_VERIFICAR_CAMBIO_ARHIVOS();
		}
		SELECT_TABLE( T_ARTICULO, TT_ORIG ); 
		switch( indice ) {
			case 1:
				/*--- codigo de barra ---*/
				SET_ORDER2("CODIGO_BARRA");
				rta = Consulta_articulo_codbarra( _consulta_cod_barra );
				break;
			case 2:
				/*--- descripcion ---*/
				TECLADO_ALFABETICO = 1;
				SET_ORDER2("NOMBRE");
				rta = Consulta_articulo_descrip( _consulta_descripcion, indice );
				TECLADO_ALFABETICO = 0;
				break;
			case 3:
				/*--- codigo interno ---*/
				// Se llama a la funcin Consulta_articulo_descrip porque el formato del browser es el mismo
				SET_ORDER2("COD_INTERNO");
				rta = Consulta_articulo_descrip( _consulta_cod_interno, indice );
				break;
			case 5:
				/*--- marca ---*/
				TECLADO_ALFABETICO = 1;
				// Se llama a la funcin Consulta_articulo_descrip porque el formato del browser es el mismo
				SET_ORDER2("MARCA");
				rta = Consulta_articulo_descrip( _consulta_marca, indice );
				TECLADO_ALFABETICO = 0;
				break;
			case 13:
				extremo = 0;
				SET_WHERE("COD_COBRO > 0");
				SET_ORDER2("CODIGO_BARRA");
				RUN_QUERY(NO);
				while( !dbEOF() /*!BtrvEOF( )*/ ) {
					if( articulo.cod_cobro > 0 )
						extremo ++;
					SKIP2( 1 );
				}
				GO2( TOP );
				rta = Consulta_articulo_descrip( _consulta_cod_cobro, indice );
				//BROWSE( 2, 4, 79 - ancho_panel, 19, 15, _consulta_cod_cobro );
				break;
		}
		if( perm_scanner_ant != VARIOS_SCANNER_HABILITADO && indice != 1 ) { //Vuelvo scanner a estado anterior
			CR1_HABILITA_SCANNER( NO );
		}
	}
    //ExecuteAction(A_ACTIVAR_PANEL_VENTAS);
	BLOQUEAR_DATOS = NO;
	ExecuteAction( A_VENTAS_FOCUS );
    return ( rta );
}
char letras[8][4] = {{'A', 'B', 'C', 0},{'D', 'E', 'F', 0},{'G', 'H', 'I', 0},{'J', 'K', 'L', 0},
                     {'M', 'N', 'O', 0},{'P', 'Q', 'R', 0},{'S', 'T', 'U', 'V'},{'W', 'X', 'Y', 'Z'}};
int EVALUA_TECLA( int tecla )
{
    int tt,tt_ant;
    if( TECLADO_ALFABETICO ) {
        // APRETO ALGUN BOTON
        if( tecla >= CODIGO_BOTON_LETRA_MAX && tecla <= CODIGO_BOTON_LETRA_MIN ) {
            tt = tecla;
            tt_ant = tt;
            while( ( tt >= CODIGO_BOTON_LETRA_MAX && tt <= CODIGO_BOTON_LETRA_MIN ) || tt == -999 ) {
                // si apreta otro boton
                if( tt >= CODIGO_BOTON_LETRA_MAX && tt <= CODIGO_BOTON_LETRA_MIN ) {
                    tt_ant = tt;
                }
                tt = GETCH();
            }
            // la vuelve a apretar
            if( tt == 49 || tt == 50 || tt == 51 || tt == 52 ) {
                tt = letras[-tt_ant - 501][tt - '1'];
                if( tt ) {
                    tecla = tt;
                }
            }
        }
    }
    return tecla;
}
#ifdef COMPILAR_CONSULTA_ARTICULO
/*****************************************************************************/
void CONSULTA_DE_ARTICULO( int muestra_promociones )
/*****************************************************************************/
{
    int pant;
    char cadena[25];
    int seguir = 1, x, desplazamiento, yart = 18;
    int x_cursor, y_cursor, rta, consultado = 0;
    char *consulta_art_str = "C O N S U L T A   D E   A R T I C U L O S";
    /*float*/ double peso;
    GET_POSICION_CURSOR( &x_cursor, &y_cursor );
    precio_en_me = NO;
    MOSTRAR_CADENA( ( 80 - ancho_panel - strlen( consulta_art_str ) ) / 2, 3,
                    consulta_art_str /* S_CONSULTA_ARTICULO */ );
    //--------------------------- limpia buffer ------------------------------
    _ATRIBUTO = 7;
    // MOSTRAR_CADENA(40-ancho_panel/2, 22, "X"/*S_CON_ART_MENSAJE*/);
    x = ( 80 - ancho_panel - L_CODIGO * 3 + 1 ) / 2;
    CAJA( x - 1, yart - 1, x + L_CODIGO * 3, yart + 2 );
    CURSOR_ON();
    SET_ESPERA_GETCH( 2 );
    while( seguir ) {
        //if (PIDE_CODIGO_ARTICULO( cadena )) {
        MOSTRAR_DATOS_PRECIO_ME( precio_en_me );
        rta = PIDE_CODIGO_ARTICULO( cadena, consultado );
        if( rta > 0 ) {
            switch( rta ) {
                case 1:
                    cadena[L_CODIGO] = 0;
                    TRIM( cadena );
                    desplazamiento = L_CODIGO - strlen( cadena );
                    //movmem( cadena, &cadena[desplazamiento], L_CODIGO - desplazamiento );
                    //memmove( cadena, &cadena[desplazamiento], L_CODIGO - desplazamiento );
                    memmove( &cadena[desplazamiento], cadena, L_CODIGO - desplazamiento );
                    memset( cadena, ' ', desplazamiento );
                    if( VARIOS_SCANNER_HABILITADO ) {
                        SET_MEMORY( __ram_cod_barra, cadena );
						if( PRESENTACIONES_EN_DUN14 ) {
						   EVALUAR_CODIGOS_ESPECIALES( L_CODIGO - 1 );
						} else {
						   EVALUAR_CODIGOS_ESPECIALES( L_CODIGO );
						}
                        if( usar_cadena_scanner ) {
                            usar_cadena_scanner = 0;
                            memcpy( cadena, RAM_COD_BARRA, L_CODIGO );
                        }
                        if( BUSCA_CODIGO_BARRA( cadena ) ) {
                            consultado = 1;
                            _MOSTRAR_DATOS_ARTICULO( NO, muestra_promociones, precio_en_me, 0.0 );
                        }
                        else {
                            SET_ESPERA_GETCH( 0 );
                            CODIGO_INEXISTENTE();
                            SET_ESPERA_GETCH( 2 );
                        }
                    }
                    else {
                        if( BUSCA_CODIGO_ESPECIAL( atol( cadena ) ) ) {
                            consultado = 1;
                            _MOSTRAR_DATOS_ARTICULO( NO, muestra_promociones, precio_en_me, 0.0 );
                        }
                        else {
                            SET_ESPERA_GETCH( 0 );
                            CODIGO_INEXISTENTE();
                            SET_ESPERA_GETCH( 2 );
                        }
                    }
                    break;
                case 2:
                    if( consultado && articulo.pesable == 'S' ) {
                        SET_BORRAR_RECUADRO_DE_PETICION( 0 );
                        peso = CR1_PEDIR_PESO();
                        SET_BORRAR_RECUADRO_DE_PETICION( 1 );
                        if( peso ) {
                            _MOSTRAR_DATOS_ARTICULO( NO, muestra_promociones, precio_en_me, peso );
                        }
                        else {
                            BORRAR_DATOS_PESO( ( muestra_promociones ) ? 8 : 13 );
                            _MOSTRAR_DATOS_ARTICULO( NO, muestra_promociones, precio_en_me, 0.0 );
                        }
                    }
                    break;
            }
        }
        else {
            seguir = 0;
        }
        BORRAR_MENSAJE();
    }
    SET_ESPERA_GETCH( 0 );
}
/*****************************************************************************/
void MOSTRAR_DATOS_ARTICULO( int precio_en_me )
/*****************************************************************************/
{
    int col = 22, at;
    at = _ATRIBUTO;
    _ATRIBUTO = 7;
    MOSTRAR_CADENA( 5, 7, ST( S_NOMBRE_ ) );    //S_CON_ART_NOMBRE);     // 7
    MOSTRAR_CADENA( 5, 9, ST( S_CODIGO_ESPECIAL_ ) );   //S_CON_ART_COD_ESP);    // 9
    MOSTRAR_CADENA( 5, 11, ST( S_CODIGO_BARRA_ ) ); //S_CON_ART_COD_BARRA);  // 11
    MOSTRAR_CADENA( 5, 13, ST( S_PRECIO_ ) );   //S_CON_ART_PRECIO);     // 13 ----> 16
    if( precio_en_me ) {
        MOSTRAR_CADENA( 14, 13, medios[_medio_moneda_extranjera].simbolo_moneda );
    }
    MOSTRAR_CADENA( col, 7, articulo.nombre );
    MOSTRAR_ENTERO( col, 9, "000000000", articulo.cod_interno );
    MOSTRAR_CADENA( col, 11, articulo.cod_barra );
    CALCULA_ARTICULO_EXT();
    MOSTRAR_FLOTANTE( col, 13, ENTEROS + 1, DECIMALES, 1, 0,
                      ( precio_en_me )
                    ? PAGOS_ML_A_ME( articulo_ext.precio, _medio_moneda_extranjera )
                    : articulo_ext.precio );
    _ATRIBUTO = at;
}
/*****************************************************************************/
void MOSTRAR_DATOS_PESO( int fila, float peso, int precio_en_me )
/*****************************************************************************/
{
    int at;
    at = _ATRIBUTO;
    _ATRIBUTO = 7;
    MOSTRAR_CADENA( 35, fila, "x" );
    MOSTRAR_FLOTANTE( 37, fila, 3, 3, 1, 1, ( double )peso );
    MOSTRAR_CADENA( 46, fila, "=" );
    MOSTRAR_FLOTANTE( 47, fila, ENTEROS + 1, DECIMALES, 1, 0,
                      ROUND( peso * ( ( precio_en_me )
                                    ? PAGOS_ML_A_ME( articulo_ext.precio, _medio_moneda_extranjera )
                                    : articulo_ext.precio ), ENTEROS, DECIMALES ) );
    _ATRIBUTO = at;
}
/*****************************************************************************/
void BORRAR_DATOS_PESO( int fila )
/*****************************************************************************/
{

}
/*****************************************************************************/
int PIDE_CODIGO_ARTICULO( char *cadena, int consultado )
/*****************************************************************************/
{
    int pant;
    int iOk = 0;
    char seguir = 1, mostrar = 1, resetear_variables = 1;
    int posicion = 0, k, x, yart = 18;
    int x_cursor,y_cursor;
    GET_POSICION_CURSOR( &x_cursor, &y_cursor );
    x = ( 80 - ancho_panel - L_CODIGO * 3 + 1 ) / 2;
    _ATRIBUTO = 52;
    CAJA( x - 1, yart - 1, x + L_CODIGO * 3, yart + 2 );
    CURSOR_ON();
    SET_ESPERA_GETCH( 2 );
    while( seguir ) {
        if( resetear_variables ) {
            _leyo_scanner = 0;
            memset( cadena, ' ', L_CODIGO );
            cadena[L_CODIGO] = 0;
            posicion = 0;
            mostrar = 1;
            resetear_variables = 0;
        }
        if( mostrar ) {
            MOSTRAR_GRAN_CADENA( x, yart, cadena );
            mostrar = 0;
        }
        k = GETCH();
        switch( k ) {
            case -83:
                /*-------- DEL ---------*/
                strset( cadena, ' ' );
                posicion = 0;
                break;
            case 13:
                /*-------- ENTER --------*/
                if( posicion > 0 ) {
                    seguir = 0;
                    iOk = 1;
                }
                break;
            case 8:
                /*--------- BACKSPACE ---------*/
                posicion--;
                if( posicion < 0 ) {
                    posicion = 0;
                }
                cadena[posicion] = ' ';
                mostrar = 1;
                break;
            case 27:
                /*-------- ESC ---------*/
                seguir = 0;
                break;
            case 42:
                /*-------- Mutiplicacion --------*/
                seguir = 0;
                iOk = 2;
                break;
            case 180:
                precio_en_me = !precio_en_me;
                MOSTRAR_DATOS_PRECIO_ME( precio_en_me );
                if( consultado ) {
                    _MOSTRAR_DATOS_ARTICULO( SI, NO, precio_en_me, 0.0 );
                }
                break;
            case -999:
                /*------------- Sale por espera -------------------*/
                if( VARIOS_SCANNER_HABILITADO ) {
                    if( IS_SCANNER() ) {
                        CURSOR_OFF();
                        if( LEER_SCANNER( cadena, L_CODIGO + 1 ) ) {
							BEEP( 2000, 1000 );
                            //sizeof(cadena)
                            _leyo_scanner = 1;
                            cadena[L_CODIGO] = 0;
                            posicion = L_CODIGO - 1;
                            UNGETCH( 13 );
                            mostrar = 1;
                        }
                        else {
                            MENSAJE_STRING( S_ERROR_LECTURA_SCANNER );
                            memset( cadena, ' ', L_CODIGO );
                            mostrar = 1;
                            posicion = 0;
                        }
                        CURSOR_ON();
                    }
                }
                break;
            default:
                /*---------- Nmeros ----------*/
                if( k >= '0' && k <= '9'
                 || ( k == '.' && RAM_CANTIDAD == 1 && CANTIDADES_CON_DECIMALES ) ) {
                    if( posicion < L_CODIGO ) {
                        cadena[posicion++] = k;
                        mostrar = 1;
                    }
                }
                break;
        }
    }
    SET_ESPERA_GETCH( 0 );
    return iOk;
}
/*****************************************************************************/
void MOSTRAR_DATOS_PRECIO_ME( int me )
/*****************************************************************************/
{
    int at;
    at = _ATRIBUTO;
    _ATRIBUTO = ( me ) ? 120 : 7;
    if( me ) {
        MOSTRAR_CADENA( 5, 24, medios[_medio_moneda_extranjera].simbolo_moneda );
    }
    else {
        MOSTRAR_CADENA( 5, 24, "   " );
    }
    _ATRIBUTO = at;
}
/*****************************************************************************/
void _MOSTRAR_DATOS_ARTICULO( int refrescar, int muestra_promociones, int precio_en_me,
                              double peso )
/*****************************************************************************/
{
    static int _muestra_promociones;
    static double _peso;
    if( refrescar ) {
        peso = _peso;
        muestra_promociones = _muestra_promociones;
    }
    else {
        _peso = peso;
        _muestra_promociones = muestra_promociones;
    }
    if( !peso || refrescar ) {
        if( muestra_promociones ) {
            MOSTRAR_DATOS_ARTICULO_CON_PROMOCIONES( precio_en_me );
        }
        else {
            MOSTRAR_DATOS_ARTICULO( precio_en_me );
        }
    }
    if( peso ) {
        MOSTRAR_DATOS_PESO( ( muestra_promociones ) ? 8 : 13, ( float )peso, precio_en_me );
    }
}
#endif

