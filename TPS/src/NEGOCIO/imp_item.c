#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <comprob.h> 
#include <cr.h> 
#include <fiscal.h> 
#include <ftoa.h> 
#include <imp_item.h> 
#include <m_cadena.h> 
#include <print.h> 
#include <tkt.h> 
#include <_cr1.h> 
#include <cmemory.h>
#include <math.h>
#include <actions.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W) || defined(INVEL_L)
#endif
extern double difer , total_acumulado; //Seba
/*****************************************************************************/
int IMPRIME_ITEM( int indice )
/*****************************************************************************/
{
    double precio = 0;
    int error = 0;
	int _ptr = 0, _handle = 0, _estado = 0;
	/*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    _estado = STACK_INS( sizeof( short ) );

	if( IMPRIMIR_TICKET ) {
		precio = ITEM_PRECIO + ITEM_RECARGO;
		if( ( RAM_MODO_EJECUCION == INVENTARIO && !IMPRIME_PRECIO_EN_MODO_INVENTARI )
			|| fabs( precio ) < 0.005 ) {
			precio = 0;
		}

		if( IMPRESORA_FISCAL == FISCAL ) {
			if( _ESTADO == 0 ) {
				if( RAM_TICKET_FACTURA || RAM_NOTA_CR )
				{
					if( clientes.cond_iva != 1 )
						if( RAM_ANULACION ) {
							if( difer != 0 )
								error = IMPRIME_ITEM_FAC(ITEM_NOMBRE_LARGO, difer,0.0, -RAM_CANTIDAD );//Cancela la Bonificacion
							
							error = IMPRIME_ITEM_FAC(ITEM_NOMBRE_LARGO, precio, ITEM_IMP_INT, -RAM_CANTIDAD   );
						} else
							error = IMPRIME_ITEM_FAC(ITEM_NOMBRE_LARGO, precio, ITEM_IMP_INT, RAM_CANTIDAD   );
					else if( clientes.cond_iva == 1 ) { // RESPONSABLE INCRIPTO --> FACTURA A
						if ( RAM_ANULACION ) {
								
							if( difer != 0 )
									error = IMPRIME_ITEM_FAC(ITEM_NOMBRE_LARGO, difer,0.0, -RAM_CANTIDAD );//Cancela la Bonificacion

							error = IMPRIME_ITEM_FAC(ITEM_NOMBRE_LARGO, ( ITEM_PRECIO_SIN_IVA - ITEM_IMP_INT ), ITEM_IMP_INT, -RAM_CANTIDAD );
						} else
							error = IMPRIME_ITEM_FAC(ITEM_NOMBRE_LARGO, ( ITEM_PRECIO_SIN_IVA - ITEM_IMP_INT ), ITEM_IMP_INT, RAM_CANTIDAD );
					}
				}
				else
				{
					error = ITEM_FORMATO_FISCAL( precio, indice );
				}
				_SET_ESTADO( 99 );
			}
		} else {
			if( TKT_CONFIGURABLE ) {
				if( _ESTADO == 0 ) 
					IMPRIMIR_TKT_CUERPO( precio, indice );
				_SET_ESTADO( 99 );
                #ifdef COMPILAR_FORMATO_TICKET
            }
			else {
				if( _ESTADO == 0 ) {
					switch( FORMATO_TICKET ) {
                    case 0:
                        if( HARD_OMRON ) {
                            #ifdef COMPILAR_OMRON
								ITEM_FORMATO_OMROM_AMERICANOS( precio );
                            #endif
                        }
                        else {
                            ITEM_FORMATO_AMERICANOS( precio );
                        }
                        break;
					case 1:
						ITEM_FORMATO_LIBERTAD( precio );
						break;
					case 2:
						ITEM_FORMATO_OMROM_LAR( precio );
						break;
					case 3:
						ITEM_FORMATO_SPAR( precio );
						break;
					case 4:
						ITEM_FORMATO_TP( precio );
						break;
					case 5:
						#ifdef COMPILAR_OMRON
							ITEM_FORMATO_OMRON_FERNI( precio );
						#endif
						break;
					case 6:
						ITEM_FORMATO_VALLEJO( precio );
						break;
					case 7:
						ITEM_FORMATO_FALABELLA( precio );
						break;
					case 8:
						ITEM_FORMATO_BIENESTAR( precio );
						break;
					case 9:
						// FORMATO AMERICANOS IBM
						break;
					case 10:
						ITEM_FORMATO_TOLEDO( precio );
						break;
					case 11:
						ITEM_FORMATO_CARREFOUR( precio );
						break;
					case 12:
						ITEM_FORMATO_CARREFOUR_IBM_4694( precio );
						break;
					case 13:
						ITEM_FORMATO_CONFIGURABLE();
						break;
					}
					_SET_ESTADO( 99 );
					#endif
				}
			}
		}
	}

	CLOSE_RECUPERO( _ptr, _handle );

    return error;
}
#ifdef COMPILAR_FORMATO_TICKET
/*****************************************************************************/
void ITEM_FORMATO_AMERICANOS( double precio )
/*****************************************************************************/
{
    char caracter_sig[2];
	char caracter_por[2];
	caracter_por[0]='x';
	caracter_por[1]='\0';
	caracter_sig[0]='-';
	caracter_sig[1]='\0';

	PRN_CONTINUA();
    PRN_INHABILITA_AVANCE_JOURNAL();
    if( RAM_ANULACION ) {
        precio = -precio;
    }
    /*----------------------------- Receipt -------------------------------*/
    if( RAM_ANULACION ) {
        MOSTRAR_CADENA( 1, PRN, caracter_sig );
    }
    MOSTRAR_CADENA( 2, PRN, ITEM_NOMBRE_LARGO );
    MICRO_LF( 22 );
    MOSTRAR_CADENA( 2, PRN, ITEM_MARCA );
    MOSTRAR_CADENA( 18, PRN, ITEM_CONTENIDO );
    MOSTRAR_CADENA( 23, PRN, ITEM_UNIDAD );
    if( RAM_CANTIDAD == 1 ) {
        MOSTRAR_FLOTANTE( 34, PRN, ENTEROS, DECIMALES, 1, 0, precio );
    }
    else if( ITEM_PEDIR_PRECIO && RAM_MODO_EJECUCION != INVENTARIO ) {
        MOSTRAR_FLOTANTE( 34, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    }
    else {
        MICRO_LF( 22 );
        MOSTRAR_CANTIDAD( 2, PRN, RAM_CANTIDAD );
        MOSTRAR_CADENA( 11, PRN, caracter_por );
        MOSTRAR_FLOTANTE( 13, PRN, ENTEROS, DECIMALES, 1, 0, precio );
        MOSTRAR_FLOTANTE( 34, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    }
    /*----------------------------- Journal ------------------------------*/
    PRN_HABILITA_AVANCES();
    MICRO_LF( 10 );
    if( RAM_ANULACION ) {
        MOSTRAR_CADENA( 1 + 40, PRN, caracter_sig );
    }
    MOSTRAR_CADENA( 2 + 40, PRN, ITEM_COD_BARRA );
    if( RAM_CANTIDAD != 1 ) {
        MOSTRAR_CANTIDAD( 16 + 40, PRN, RAM_CANTIDAD );
        MOSTRAR_CADENA( 24 + 40, PRN, caracter_por );
        MOSTRAR_FLOTANTE( 26 + 40, PRN, ENTEROS, DECIMALES, 1, 0, precio );
    }
    MOSTRAR_FLOTANTE( 34 + 40, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    /*------------------------------ Com£n ------------------------------*/
    PRN_DISCONTINUA();
    MICRO_LF( 20 );
}
/*****************************************************************************/
void ITEM_FORMATO_LIBERTAD( double precio )
/*****************************************************************************/
{
    char caracter_sig[2];
	char caracter_por[2];
	caracter_por[0]='x';
	caracter_por[1]='\0';
	caracter_sig[0]='-';
	caracter_sig[1]='\0';

	if( RAM_ANULACION ) {
        MOSTRAR_CADENA( 1, PRN, caracter_sig );
    }
    MOSTRAR_ENTERO( 2, PRN, "000000", ITEM_COD_ESPECIAL );
    MOSTRAR_CADENA( 9, PRN, ITEM_NOMBRE_LARGO );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, caracter_por );
    MOSTRAR_CANTIDAD( 3, PRN, RAM_CANTIDAD );
    MOSTRAR_CADENA( 12, PRN, caracter_por );
    MOSTRAR_ENTERO( 14, PRN, "999", ITEM_PRESENTACION );
    MOSTRAR_CADENA( 18, PRN, "(" );
    MOSTRAR_ENTERO( 19, PRN, "9", ITEM_COD_PRESENTACION );
    MOSTRAR_CADENA( 20, PRN, ")" );
    #if defined(INVEL_W) || defined(INVEL_L)
    MOSTRAR_FLOTANTE( 40 - ( ENTEROS + DECIMALES + 1 ), PRN, ENTEROS, DECIMALES, 1, 0,
                      precio * RAM_CANTIDAD );
    #else
    MOSTRAR_FLOTANTE( 32, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    #endif
    LF( 1 );
    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}
/*****************************************************************************/
void ITEM_FORMATO_OMROM_AMERICANOS( double precio )
/*****************************************************************************/
{
    char caracter_sig[2];
	char caracter_por[2];
	caracter_sig[0]='x';
	caracter_sig[1]='\0';
	caracter_sig[0]='-';
	caracter_sig[1]='\0';

	PRN_CONTINUA();
    PRN_INHABILITA_AVANCE_JOURNAL();
    if( RAM_ANULACION ) {
        precio = -precio;
    }
    /*----------------------------- Receipt -------------------------------*/
    if( RAM_ANULACION ) {
        MOSTRAR_CADENA( 1, PRN, caracter_sig );
    }
    MOSTRAR_CADENA( 2, PRN, ITEM_NOMBRE_LARGO );
    LF( 1 );
    MOSTRAR_CADENA( 2, PRN, ITEM_MARCA );
    MOSTRAR_CADENA( 18, PRN, ITEM_CONTENIDO );
    MOSTRAR_CADENA( 23, PRN, ITEM_UNIDAD );
    if( RAM_CANTIDAD == 1 ) {
        MOSTRAR_FLOTANTE( 34, PRN, ENTEROS, DECIMALES, 1, 0, precio );
    }
    else if( ITEM_PEDIR_PRECIO && RAM_MODO_EJECUCION != INVENTARIO ) {
        MOSTRAR_FLOTANTE( 34, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    }
    else {
        LF( 1 );
        MOSTRAR_CANTIDAD( 2, PRN, RAM_CANTIDAD );
        MOSTRAR_CADENA( 11, PRN, caracter_por );
        MOSTRAR_FLOTANTE( 13, PRN, ENTEROS, DECIMALES, 1, 0, precio );
        MOSTRAR_FLOTANTE( 34, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    }
    /*----------------------------- Journal ------------------------------*/
    PRN_INHABILITA_AVANCE_RECEIPT();
    if( RAM_ANULACION ) {
        MOSTRAR_CADENA( 1 + 40, PRN, caracter_sig );
    }
    MOSTRAR_CADENA( 2 + 40, PRN, ITEM_COD_BARRA );
    if( RAM_CANTIDAD != 1 ) {
        MOSTRAR_CANTIDAD( 16 + 40, PRN, RAM_CANTIDAD );
        MOSTRAR_CADENA( 24 + 40, PRN, caracter_por );
        MOSTRAR_FLOTANTE( 26 + 40, PRN, ENTEROS, DECIMALES, 1, 0, precio );
    }
    MOSTRAR_FLOTANTE( 34 + 40, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    /*------------------------------ Com£n ------------------------------*/
    PRN_HABILITA_AVANCES();
    PRN_DISCONTINUA();
    LF( 1 );
}
#endif
/*****************************************************************************/
int ITEM_FORMATO_FISCAL( double precio, int indice )
/*****************************************************************************/
{
    char decimales = DECIMALES;
    //char      buffer[25];
    int ok = 0;

    //fabiana 15-02-05 ** nuevo manejo de redondeos--para factura o tickets donde se discrimina iva
    if( ( DECIMALES < 4 )
     && ( ES_FACTURA_A( clientes.cond_iva ) || MOSTRAR_SIN_IVA_ELSALVADOR()
       || MOSTRAR_PRECIO_SIN_IVA_EN_VENTAS ) ) {
        decimales = 4;
    }
	
	if( IMPRESORA_FISCAL == FISCAL ) {
		//una opcion para tomar la descripcion via tkt ver otra para desarrrollar tambien para TF y F, NCr

		IMPRIME_TKT_DESCRIPCION_CUERPO( 1 );

        if( RAM_ANULACION )
		{
			if(difer!=0)
			{
				ok = IMPRIME_ITEM_FISCAL( ITEM_NOMBRE_LARGO, difer, RAM_CANTIDAD, indice );//Cancela la Bonificacion
				ok = IMPRIME_ITEM_FISCAL( ITEM_NOMBRE_LARGO, ( total_acumulado - ( difer * RAM_CANTIDAD ) )/RAM_CANTIDAD, -RAM_CANTIDAD, indice );//Cancelo el item
				difer=0.0;
				total_acumulado=0.0;
			}
			else
				ok = IMPRIME_ITEM_FISCAL( ITEM_NOMBRE_LARGO, precio, -RAM_CANTIDAD, indice );
		}
        else 
            ok = IMPRIME_ITEM_FISCAL( ITEM_NOMBRE_LARGO, precio, RAM_CANTIDAD, indice );
        
    }

    return ok;
}
#ifdef COMPILAR_FORMATO_TICKET
/*****************************************************************************/
void ITEM_FORMATO_OMROM_LAR( double precio )
/*****************************************************************************/
{
    char caracter_sig[2];
	char caracter_por[2];
	caracter_por[0]='x';
	caracter_por[1]='\0';
	caracter_sig[0]='-';
	caracter_sig[1]='\0';
	/*----------------------------- Receipt -------------------------------*/
    if( RAM_ANULACION ) {
        precio = -precio;
        MOSTRAR_CADENA( 1, PRN, caracter_sig );
    }
    MOSTRAR_CADENA( 2, PRN, ITEM_NOMBRE_LARGO );
    if( RAM_CANTIDAD == 1 ) {
        MOSTRAR_FLOTANTE( 34, PRN, ENTEROS, DECIMALES, 1, 0, precio );
    }
    else if( ITEM_PEDIR_PRECIO && RAM_MODO_EJECUCION != INVENTARIO ) {
        MOSTRAR_FLOTANTE( 34, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    }
    else {
        LF( 1 );
        MOSTRAR_FLOTANTE( 2, PRN, 3, 3, 1, 0, RAM_CANTIDAD );
        MOSTRAR_CADENA( 11, PRN, caracter_por );
        MOSTRAR_FLOTANTE( 13, PRN, ENTEROS, DECIMALES, 1, 0, precio );
        MOSTRAR_FLOTANTE( 34, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    }
    LF( 1 );
}
/*****************************************************************************/
void ITEM_FORMATO_SPAR( double precio )
/*****************************************************************************/
{
    char nombre[31];
	char caracter_sig[2];
	char caracter_por[2];
	caracter_por[0]='x';
	caracter_por[1]='\0';
	caracter_sig[0]='-';
	caracter_sig[1]='\0';

    PRN_CONTINUA();
    /*----------------------- receipt -----------------------*/
    memcpy( nombre, ITEM_NOMBRE_LARGO, 25 );
    nombre[25] = 0;
    if( RAM_ANULACION ) {
        MOSTRAR_CADENA( 1, PRN, caracter_sig );
    }
    MOSTRAR_CADENA( 2, PRN, nombre );
    MOSTRAR_CADENA( 28, PRN, ITEM_CONTENIDO );
    MOSTRAR_CADENA( 32, PRN, ITEM_UNIDAD );
    if( RAM_CANTIDAD == 1 ) {
        MOSTRAR_FLOTANTE( 35, PRN, 3, 2, 1, 0, precio );
    }
    else {
        LF( 1 );
        MOSTRAR_CADENA( 2, PRN, "$" );
        MOSTRAR_FLOTANTE( 3, PRN, 3, 2, 1, 0, precio );
        MOSTRAR_CADENA( 11, PRN, caracter_por );
        MOSTRAR_FLOTANTE( 12, PRN, 3, 3, 0, 0, RAM_CANTIDAD );
        MOSTRAR_FLOTANTE( 35, PRN, 3, 2, 1, 0, precio * RAM_CANTIDAD );
    }
    /*----------------------- journal -----------------------*/
    MOSTRAR_ENTERO( 1 + 40, PRN, "000000000", ITEM_COD_ESPECIAL );
    if( RAM_ANULACION ) {
        precio = -precio;
    }
    if( RAM_CANTIDAD == 1 ) {
        MOSTRAR_FLOTANTE( 34 + 40, PRN, 4, 2, 1, 0, precio );
    }
    else {
        MOSTRAR_FLOTANTE( 15 + 40, PRN, 4, 2, 1, 0, precio );
        MOSTRAR_CADENA( 24 + 40, PRN, caracter_por );
        MOSTRAR_FLOTANTE( 26 + 40, PRN, 3, 3, 0, 0, RAM_CANTIDAD );
        MOSTRAR_FLOTANTE( 34 + 40, PRN, 4, 2, 1, 0, precio * RAM_CANTIDAD );
    }
    PRN_DISCONTINUA();
    LF( 1 );
}
#ifdef COMPILAR_OMRON
/*****************************************************************************/
void ITEM_FORMATO_OMRON_FERNI( double precio )
/*****************************************************************************/
{
    PRN_CONTINUA();
    PRN_INHABILITA_AVANCE_JOURNAL();
    /*----------------------- receipt -----------------------*/
    if( RAM_ANULACION ) {
        MOSTRAR_CADENA( 1, PRN, "-" );
        precio = -precio;
    }
    MOSTRAR_CADENA( 2, PRN, ITEM_NOMBRE_LARGO );
    MOSTRAR_CADENA( 33, PRN, ITEM_CONTENIDO );
    MOSTRAR_CADENA( 38, PRN, ITEM_UNIDAD );
    LF( 1 );
    /*------------ 2§ linea receipt == journal --------------*/
    PRN_HABILITA_AVANCES();
    MOSTRAR_CADENA( 2, PRN, ITEM_COD_BARRA );
    if( RAM_CANTIDAD != 1 ) {
        if( precio ) {
            MOSTRAR_FLOTANTE( 16, PRN, ENTEROS, DECIMALES, 1, 0, precio );
        }
        MOSTRAR_CADENA( 24, PRN, "x" );
        MOSTRAR_FLOTANTE( 25, PRN, 3, 3, 0, 0, RAM_CANTIDAD );
    }
    if( precio ) {
        MOSTRAR_CADENA( 33, PRN, "$" );
        MOSTRAR_FLOTANTE( 34, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    }
    LF( 1 );
    LF_RECEIPT( 1 );
}
#endif
/*****************************************************************************/
void ITEM_FORMATO_TP( double precio )
/*****************************************************************************/
{
    char caracter_sig[2];
	char caracter_por[2];
	caracter_por[0]='x';
	caracter_por[1]='\0';
	caracter_sig[0]='-';
	caracter_sig[1]='\0';

	if( RAM_ANULACION ) {
        MOSTRAR_CADENA( 1, PRN, caracter_sig );
    }
    MOSTRAR_ENTERO( 2, PRN, "000000", ITEM_COD_ESPECIAL );
    MOSTRAR_CADENA( 9, PRN, ITEM_NOMBRE_LARGO );
    MICRO_LF( 22 );
    MOSTRAR_FLOTANTE( 1, PRN, 3, 3, 0, 0, RAM_CANTIDAD );
    MOSTRAR_CADENA( 10, PRN, caracter_por );
    MOSTRAR_FLOTANTE( 12, PRN, ENTEROS, DECIMALES, 1, 0, precio );
    MOSTRAR_FLOTANTE( 32, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    MICRO_LF( 30 );
}
/*****************************************************************************/
void ITEM_FORMATO_VALLEJO( double precio )
/*****************************************************************************/
{
    char caracter_sig[2];
	char caracter_por[2];
	caracter_por[0]='x';
	caracter_por[1]='\0';
	caracter_sig[0]='-';
	caracter_sig[1]='\0';

	PRN_CONTINUA();
    PRN_INHABILITA_AVANCE_JOURNAL();
    /*----------------------- receipt -----------------------*/
    if( RAM_ANULACION ) {
        MOSTRAR_CADENA( 1, PRN, caracter_sig );
        precio = -precio;
    }
    MOSTRAR_CADENA( 2, PRN, ITEM_NOMBRE_LARGO );
    MOSTRAR_CADENA( 33, PRN, ITEM_CONTENIDO );
    MOSTRAR_CADENA( 38, PRN, ITEM_UNIDAD );
    LF( 1 );
    /*------------ 2§ linea receipt == journal --------------*/
    PRN_HABILITA_AVANCES();
    MOSTRAR_CADENA( 2, PRN, ITEM_COD_BARRA );
    if( RAM_CANTIDAD != 1 ) {
        MOSTRAR_FLOTANTE( 16, PRN, ENTEROS, DECIMALES, 1, 0, precio );
        MOSTRAR_CADENA( 24, PRN, caracter_por );
        MOSTRAR_FLOTANTE( 25, PRN, 3, 3, 0, 0, RAM_CANTIDAD );
    }
    MOSTRAR_CADENA( 33, PRN, "$" );
    MOSTRAR_FLOTANTE( 34, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    LF( 1 );
    MOSTRAR_CADENA( 2, PRN, ST( S_SECTOR ) );
    MOSTRAR_ENTERO( 9, PRN, "0000", NRO_VENDEDOR );
    LF( 1 );
    LF_RECEIPT( 1 );
}
#endif
/*****************************************************************************/
void MOSTRAR_CANTIDAD( int x, int y, double cantidad )
/*****************************************************************************/
{
    if( cantidad > 999.999 ) {
        MOSTRAR_FLOTANTE( x, y, 4, 2, 0, 0, cantidad );
    }
    else {
        MOSTRAR_FLOTANTE( x, y, 3, 3, 0, 0, cantidad );
    }
}
#ifdef COMPILAR_FORMATO_TICKET
/*****************************************************************************/
void ITEM_FORMATO_FALABELLA( double precio )
/*****************************************************************************/
{
    char caracter_sig[2];
	char caracter_por[2];
	caracter_por[0]='x';
	caracter_por[1]='\0';
	caracter_sig[0]='-';
	caracter_sig[1]='\0';

	if( RAM_ANULACION ) {
        MOSTRAR_CADENA( 1, PRN, caracter_sig );
    }
    MOSTRAR_CADENA( 2, PRN, ITEM_COD_BARRA );
    MOSTRAR_CADENA( 16, PRN, ITEM_MARCA );
    if( RAM_CANTIDAD == 1 ) {
        // Se agrega un digito en enteros por los importes negativos
        MOSTRAR_FLOTANTE( 31, PRN, ENTEROS + 1, DECIMALES, 1, 0, precio );
    }
    else {
        LF( 1 );
        MOSTRAR_FLOTANTE( 1, PRN, 3, 3, 0, 0, RAM_CANTIDAD );
        MOSTRAR_CADENA( 10, PRN, caracter_por );
        MOSTRAR_FLOTANTE( 12, PRN, ENTEROS, DECIMALES, 1, 0, precio );
        MOSTRAR_FLOTANTE( 31, PRN, ENTEROS + 1, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    }
    LF( 1 );
}
#endif
#ifdef COMPILAR_FORMATO_TICKET
/*****************************************************************************/
void ITEM_FORMATO_BIENESTAR( double precio )
/*****************************************************************************/
{
    char nombre[40];
	char caracter_sig[2];
	char caracter_por[2];
	caracter_por[0]='x';
	caracter_por[1]='\0';
	caracter_sig[0]='-';
	caracter_sig[1]='\0';

    /*------------------- receipt == journal -----------------------*/
    if( RAM_ANULACION ) {
        precio = -precio;
    }
    PRN_HABILITA_AVANCES();
    if( RAM_CANTIDAD != 1 && precio ) {
        MOSTRAR_FLOTANTE( 2, PRN, 3, 3, 0, 0, RAM_CANTIDAD );
        MOSTRAR_CADENA( 10, PRN, caracter_por );
        MOSTRAR_FLOTANTE( 12, PRN, ENTEROS, DECIMALES, 1, 0, precio );
        LF( 1 );
    }
    if( RAM_ANULACION ) {
        MOSTRAR_CADENA( 1, PRN, caracter_sig );
    }
    strcpy( nombre, ITEM_NOMBRE_LARGO );
    nombre[29] = 0;
    MOSTRAR_CADENA( 2, PRN, nombre );
    if( precio ) {
        MOSTRAR_CADENA( 31, PRN, "$" );
        MOSTRAR_FLOTANTE( 32, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    }
    LF( 1 );
}
#endif
#ifdef COMPILAR_FORMATO_TICKET
/*****************************************************************************/
void ITEM_FORMATO_TOLEDO( double precio )
/*****************************************************************************/
{
    char caracter_sig[2];
	char caracter_por[2];
	caracter_por[0]='x';
	caracter_por[1]='\0';
	caracter_sig[0]='-';
	caracter_sig[1]='\0';

	PRN_CONTINUA();
    PRN_INHABILITA_AVANCE_JOURNAL();
    if( RAM_ANULACION ) {
        precio = -precio;
    }
    /*----------------------------- Receipt -------------------------------*/
    if( RAM_ANULACION ) {
        MOSTRAR_CADENA( 1, PRN, caracter_sig );
    }
    MOSTRAR_CADENA( 2, PRN, ITEM_NOMBRE_LARGO );
    MICRO_LF( 22 );
    MOSTRAR_CADENA( 2, PRN, ITEM_MARCA );
    if( RAM_CANTIDAD == 1 ) {
        MOSTRAR_FLOTANTE( 34, PRN, ENTEROS, DECIMALES, 1, 0, precio );
    }
    else if( ITEM_PEDIR_PRECIO && RAM_MODO_EJECUCION != INVENTARIO ) {
        MOSTRAR_FLOTANTE( 34, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    }
    else {
        MICRO_LF( 22 );
        MOSTRAR_CANTIDAD( 2, PRN, RAM_CANTIDAD );
        MOSTRAR_CADENA( 11, PRN, caracter_por );
        MOSTRAR_FLOTANTE( 13, PRN, ENTEROS, DECIMALES, 1, 0, precio );
        MOSTRAR_FLOTANTE( 34, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    }
    PRN_HABILITA_AVANCES();
    MICRO_LF( 10 );
    ITEM_FORMATO_JOURNAL( precio );
    /*----------------------------- Journal ------------------------------*/
    PRN_DISCONTINUA();
    MICRO_LF( 20 );
}
#endif
/*****************************************************************************/
void ITEM_FORMATO_JOURNAL( double precio )
/*****************************************************************************/
{
    char caracter_sig[2];
	char caracter_por[2];
	caracter_por[0]='x';
	caracter_por[1]='\0';
	caracter_sig[0]='-';
	caracter_sig[1]='\0';
	/*----------------------------- Journal ------------------------------*/
    switch( FORMATO_TICKET ) {
        case 2:
            if( RAM_ANULACION ) {
                precio = -precio;
                MOSTRAR_CADENA( 1, PRN, caracter_sig );
            }
            MOSTRAR_CADENA( 2, PRN, ITEM_NOMBRE_LARGO );
            if( RAM_CANTIDAD == 1 ) {
                MOSTRAR_FLOTANTE( 32, PRN, ENTEROS, DECIMALES, 1, 0, precio );
            }
            else if( ITEM_PEDIR_PRECIO && RAM_MODO_EJECUCION != INVENTARIO ) {
                MOSTRAR_FLOTANTE( 32, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
            }
            else {
                LF( 1 );
                MOSTRAR_FLOTANTE( 2, PRN, 3, 3, 1, 0, RAM_CANTIDAD );
                MOSTRAR_CADENA( 11, PRN, caracter_por );
                MOSTRAR_FLOTANTE( 13, PRN, ENTEROS, DECIMALES, 1, 0, precio );
                MOSTRAR_FLOTANTE( 32, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
            }
            LF( 1 );
            break;
        case 10:
            if( RAM_ANULACION ) {
                MOSTRAR_CADENA( 1 + 40, PRN, caracter_sig );
            }
            if( strcmp( ITEM_COD_BARRA, "0000000000000" ) ) {
                MOSTRAR_CADENA( 2 + 40, PRN, ITEM_COD_BARRA );
            }
            else {
                MOSTRAR_ENTERO( 2 + 40, PRN, "000000000", ITEM_COD_ESPECIAL );
            }
            if( RAM_CANTIDAD != 1 ) {
                MOSTRAR_CANTIDAD( 16 + 40, PRN, RAM_CANTIDAD );
                MOSTRAR_CADENA( 24 + 40, PRN, caracter_por );
                MOSTRAR_FLOTANTE( 26 + 40, PRN, ENTEROS, DECIMALES, 1, 0, precio );
            }
            MOSTRAR_FLOTANTE( 32 + 40, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
            MICRO_LF( 3 );
            break;
        default:
            if( RAM_ANULACION ) {
                MOSTRAR_CADENA( 1 + 40, PRN, caracter_sig );
            }
            if( strcmp( ITEM_COD_BARRA, "0000000000000" ) ) {
                MOSTRAR_CADENA( 2 + 40, PRN, ITEM_COD_BARRA );
            }
            else {
                MOSTRAR_ENTERO( 2 + 40, PRN, "000000000", ITEM_COD_ESPECIAL );
            }
            if( RAM_CANTIDAD != 1 ) {
                MOSTRAR_CANTIDAD( 16 + 40, PRN, RAM_CANTIDAD );
                MOSTRAR_CADENA( 24 + 40, PRN, caracter_por );
                MOSTRAR_FLOTANTE( 26 + 40, PRN, ENTEROS, DECIMALES, 1, 0, precio );
            }
            MOSTRAR_FLOTANTE( 32 + 40, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
            LF( 1 );
            /*------------------------------ Com£n ------------------------------*/
            break;
    }
    #if defined( INVEL_W) || defined( INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}
#ifdef COMPILAR_FORMATO_TICKET
/*****************************************************************************/
void ITEM_FORMATO_CARREFOUR( double precio )
/*****************************************************************************/
{
    char nombre[40];
    char cod_barra[17];
	char caracter_sig[2];
	char caracter_por[2];
	caracter_por[0]='x';
	caracter_por[1]='\0';
	caracter_sig[0]='-';
	caracter_sig[1]='\0';
    /*------------------- receipt == journal -----------------------*/
    PRN_HABILITA_AVANCES();
    if( RAM_CORRECCION ) {
        MOSTRAR_CADENA( 1, PRN, ST( S_CORRECCION ) );
        LF( 1 );
    }
    else if( RAM_ANULACION ) {
        MOSTRAR_CADENA( 1, PRN, ST( S_ANULACION ) );
        LF( 1 );
    }
    if( RAM_ANULACION ) {
        precio = -precio;
    }
    if( RAM_NEGATIVO || RAM_MODO_DEVOLUCION ) {
        if( ITEM_COD_MOVIMIENTO == _DIFERENCIA_PRECIO ) {
            MOSTRAR_CADENA( 1, PRN, ST( S_DIFERENCIA_PRECIO ) );
            LF( 1 );
        }
        #ifdef COMPILAR_COMPROMISO_200
        if( ITEM_COD_MOVIMIENTO == _COMPROMISO_200 ) {
            MOSTRAR_CADENA( 1, PRN, ST( S_COMPROMISO_200_ ) );
            LF( 1 );
        }
        #endif
        if( ITEM_COD_MOVIMIENTO == _COMPROMISO_PRECIO_MAS_BAJO ) {
            MOSTRAR_CADENA( 1, PRN, ST( S_COMPROMISO_PRECIO_MAS_BAJO ) );
            LF( 1 );
        }
    }
    if( ( RAM_CANTIDAD != 1 ) && precio && !ITEM_PESABLE && !ITEM_PEDIR_PRECIO ) {
        MOSTRAR_FLOTANTE( 1, PRN, 3, 3, 1, 0, RAM_CANTIDAD );
        MOSTRAR_CADENA( 10, PRN, caracter_por );
        MOSTRAR_FLOTANTE( 12, PRN, ENTEROS, DECIMALES, 1, 0, precio );
        LF( 1 );
    }
    if( RAM_ANULACION ) {
        MOSTRAR_CADENA( 1, PRN, caracter_sig );
    }
    else {
        MOSTRAR_CADENA( 1, PRN, "#" );
    }
    strcpy( nombre, ITEM_NOMBRE_LARGO );
    nombre[17] = 0;
    if( ITEM_PEDIR_PRECIO ) {
        strcpy( cod_barra, ITEM_COD_BARRA );
        ARMA_COD_BARRA( cod_barra, fabs( precio * RAM_CANTIDAD ) );
        MOSTRAR_CADENA( 3, PRN, cod_barra );
    }
    else {
        MOSTRAR_CADENA( 3, PRN, ITEM_COD_BARRA );
    }
    MOSTRAR_CADENA( 17, PRN, nombre );
    if( precio ) {
        MOSTRAR_FLOTANTE( 34, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    }
    LF( 1 );
}
/*****************************************************************************/
void ITEM_FORMATO_CARREFOUR_IBM_4694( double precio )
/*****************************************************************************/
{
    char nombre[40];
    char cod_barra[17];
	char caracter_sig[2];
	char caracter_por[2];
	caracter_por[0]='x';
	caracter_por[1]='\0';
	caracter_sig[0]='-';
	caracter_sig[1]='\0';
    /*------------------- receipt == journal -----------------------*/
    PRN_HABILITA_AVANCES();
    if( RAM_CORRECCION ) {
        MOSTRAR_CADENA( 1, PRN, ST( S_CORRECCION ) );
        LF( 1 );
    }
    else if( RAM_ANULACION ) {
        MOSTRAR_CADENA( 1, PRN, ST( S_ANULACION ) );
        LF( 1 );
    }
    if( RAM_ANULACION ) {
        precio = -precio;
    }
    if( RAM_NEGATIVO || RAM_MODO_DEVOLUCION ) {
        if( ITEM_COD_MOVIMIENTO == _DIFERENCIA_PRECIO ) {
            MOSTRAR_CADENA( 1, PRN, ST( S_DIFERENCIA_PRECIO ) );
            LF( 1 );
        }
        #ifdef COMPILAR_COMPROMISO_200
        if( ITEM_COD_MOVIMIENTO == _COMPROMISO_200 ) {
            MOSTRAR_CADENA( 1, PRN, ST( S_COMPROMISO_200_ ) );
            LF( 1 );
        }
        #endif
        if( ITEM_COD_MOVIMIENTO == _COMPROMISO_PRECIO_MAS_BAJO ) {
            MOSTRAR_CADENA( 1, PRN, ST( S_COMPROMISO_PRECIO_MAS_BAJO ) );
            LF( 1 );
        }
    }
    if( ( RAM_CANTIDAD != 1 ) && precio && !ITEM_PESABLE && !ITEM_PEDIR_PRECIO ) {
        MOSTRAR_FLOTANTE( 1, PRN, 3, 3, 1, 0, RAM_CANTIDAD );
        MOSTRAR_CADENA( 10, PRN, caracter_por );
        MOSTRAR_FLOTANTE( 12, PRN, ENTEROS, DECIMALES, 1, 0, precio );
        LF( 1 );
    }
    if( RAM_ANULACION ) {
        MOSTRAR_CADENA( 1, PRN, caracter_sig );
    }
    else {
        MOSTRAR_CADENA( 1, PRN, "#" );
    }
    strcpy( nombre, ITEM_NOMBRE_LARGO );
    nombre[15] = 0;
    if( ITEM_PEDIR_PRECIO ) {
        strcpy( cod_barra, ITEM_COD_BARRA );
        ARMA_COD_BARRA( cod_barra, fabs( precio * RAM_CANTIDAD ) );
        MOSTRAR_CADENA( 3, PRN, cod_barra );
    }
    else {
        MOSTRAR_CADENA( 3, PRN, ITEM_COD_BARRA );
    }
    MOSTRAR_CADENA( 17, PRN, nombre );
    if( precio ) {
        MOSTRAR_FLOTANTE( 32, PRN, ENTEROS, DECIMALES, 1, 0, precio * RAM_CANTIDAD );
    }
    LF( 1 );
}
#endif
/*************************************************************************/
void ITEM_FORMATO_CONFIGURABLE()
/*************************************************************************/
{
    //IMPRIMIR_TKT_PIE();
    LF( 1 );
}
/*****************************************************************************/
void CAMBIA_COMA_POR_PUNTO( char *m, int len )
/*****************************************************************************/
{
    int j;
    for( j = 0;j <= len;j++ ) {
        if( m[j] == ',' ) {
            m[j] = '.';
        }
    }
}
/*****************************************************************************/
void MOSTRAR_CANTIDAD2( int x, int y, double cantidad )
/*****************************************************************************/
{
    char cadena[10];
    int fin,enteros,decimales,l_cadena;
    if( cantidad > 999.999 ) {
        enteros = 4;
        decimales = 2;
    }
    else {
        enteros = 3;
        decimales = 3;
    }
    FTOA( cantidad, cadena, enteros, decimales, 0, 0 );
    fin = enteros + decimales;
    if( decimales ) {
        fin++;
        l_cadena = enteros + decimales + 1;
        cadena[l_cadena - decimales - 1] = '.';
    }
    cadena[fin] = 0;
    MOSTRAR_CADENA( x, y, cadena );
}

