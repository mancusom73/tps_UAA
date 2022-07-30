#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <clave.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <devoluci.h> 
#include <ftoc.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <pant.h> 
#include <p_float.h> 
#include <round.h> 
#include <stack.h> 
#include <_cr1.h> 
#include <actions.h>
#include <math.h>
#include <mensaje.h>
#include <DefTables.h>
#include <db.h>
#include <import_t.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef INVEL_W
#endif
#ifdef COMPILAR_COMPROMISO_200
void PANTALLA_COMPROMISO_200( void );
#endif
void PANTALLA_COMPROMISO_PRECIO_MAS_BAJO( void );
#define _IMPORTE      GET_MEMORY_DOUBLE( _importe )
#define _IMPORTE1      GET_MEMORY_DOUBLE( _importe1 )
#define _X1   7
#define _X2  30
#define _Y1   9
#define _Y2  18

int recalcular_precio_neto_grupo = SI;

/*****************************************************************************/
int TIPOS_DEVOLUCIONES()
/*****************************************************************************/
{
    int rta = 1, ok = 0, ok1 = 0;
    #ifdef COMPILAR_TIPOS_DEVOLUCIONES
    if( RAM_DIFERENCIA_PRECIO ) {
        ok1 = CR1_EXISTE_ITEM( ITEM_COD_ESPECIAL, ITEM_PRESENTACION, ITEM_COD_BARRA );
        if( ok1 && !MODO_DEVOLUCION ) {
            ok = CR1_EXISTE_ITEM_DIF_PRECIO( ITEM_COD_ESPECIAL, ITEM_PRESENTACION, NO,
                                             ITEM_COD_BARRA );
        }
        if( ok == 0 ) {
            MENSAJE_STRING( S_ARTICULO_NO_TICKEADO );
            rta = 0;
        }
        else if( ok == -1 ) {
            MENSAJE( "EL ARTICULO YA TIENE DIF. PRECIO" );
            rta = 0;
        }
        else {
            rta = DIFERENCIA_PRECIO();
        }
    }
    #ifdef COMPILAR_COMPROMISO_200
    if( RAM_COMPROMISO_200 ) {
        rta = COMPROMISO_200();
    }
    #endif
    if( RAM_PRECIO_MAS_BAJO ) {
        rta = PRECIO_MAS_BAJO();
    }
    #endif
    return rta;
}
#ifdef COMPILAR_TIPOS_DEVOLUCIONES
/*****************************************************************************/
int DIFERENCIA_PRECIO()
/*****************************************************************************/
{
    int rta = 0;
    //SET_MEMORY_CHAR( __ram_diferencia_precio, 0 );
    if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 21, NO, SI, PEDIR_SUPERVISOR_DIFERENCIA_PREC,
                          ST( S_DIFERENCIA_PRECIO ), NULL, _F_SUP_DIF_PRECIO, SI ) ) {
        if( PEDIR_DATOS_DIFERENCIA_PRECIO( NO ) ) {
            rta = 1;
        }
    }
    return rta;
}
/*****************************************************************************/
int PEDIR_DATOS_DIFERENCIA_PRECIO( int cambio )
/*****************************************************************************/
{
    int rta = 0, signo, i;
    int _ptr,_handle,_importe;
    char aux[100];
    char izquierda = 15;
    char derecha;
    char arriba = 7;
    char abajo = 17;
    char *cad;
    int cant = 0;
    char cadena[20];
    char aux2[100];
    char aux3[100];
    char aux4[100];
    signo = ( MODO_DEVOLUCION || RAM_NOTA_CR ) ? -1 : 1;
    izquierda = izquierda - ancho_panel / 2;
    derecha = 80 - ancho_panel / 2 - izquierda;
    OPEN_RECUPERO( &_ptr, &_handle );
    _importe = STACK_INS( sizeof( double ) );

    PANTALLA_DIFERENCIA_PRECIO();
    /*---------------------- Dibuja la pantalla ----------------------------*/
    _ATRIBUTO = 103;
    sprintf( aux, ST( S_CODIGO_BARRA______S ), ITEM_COD_BARRA );
    sprintf( aux2, ST( S_CODIGO_INTERNO____LD ), ITEM_COD_ESPECIAL );
    sprintf( aux3, ST( S_NOMBRE_ARTICULO___S ), ITEM_NOMBRE_LARGO );
    if( cambio ) {
        FTOC( ( ITEM_PRECIO - _ITEM_MONTO_IMPUESTO( 0 ) - _ITEM_MONTO_IMPUESTO( 1 )
              - _ITEM_MONTO_IMPUESTO( 2 ) - _ITEM_MONTO_IMPUESTO( 3 ) ), cadena, ENTEROS, DECIMALES,
              1, 0 );
    } else {
		if( PRECALCULAR_IMPUESTOS ) {
			FTOC( ( ITEM_PRECIO ), cadena, ENTEROS,	DECIMALES, 1, 0 );
		} else {
			FTOC( ( ITEM_PRECIO - _ITEM_MONTO_IMPUESTO( 0 ) - _ITEM_MONTO_IMPUESTO( 1 )
				- _ITEM_MONTO_IMPUESTO( 2 ) - _ITEM_MONTO_IMPUESTO( 3 ) ), cadena, ENTEROS,
				DECIMALES, 1, 0 );
		}
    }
    if( NC_PERMANENTE ) {
        sprintf( aux4, "%s %s", ST( S_PRECIO_ACTUAL___X_U___ ), cadena );
    }
    else {
        sprintf( aux4, "%s %s", ST( S_PRECIO_ACTUAL___X_U__ ), cadena );
    }
    cant = strlen( ST( S_PRECIO_DE_VENTA_X_U__ ) ) + strlen( aux ) + strlen( aux2 ) + strlen( aux3 )
         + strlen( aux4 ) + strlen( "& &-------------------DATOS DEL ARTICULO-------------------" );
    if( ( cad = ( char* )malloc( cant + 6 ) ) != NULL ) {
        memset( cad, 0, cant + 6 );
        strcat( cad, "~" );
        strncat( cad, ST( S_PRECIO_DE_VENTA_X_U__ ), strlen( ST( S_PRECIO_DE_VENTA_X_U__ ) ) );
        strcat( cad, "& &-------------------DATOS DEL ARTICULO-------------------" );
        strcat( cad, "&" );
        strncat( cad, aux, strlen( aux ) );
        strcat( cad, "&" );
        strncat( cad, aux2, strlen( aux2 ) );
        strcat( cad, "&" );
        strncat( cad, aux3, strlen( aux3 ) );
        strcat( cad, "&" );
        strncat( cad, aux4, strlen( aux4 ) );
    }
	MOSTRAR_INPUT_CADENA( "", 0 );
    ExecuteSSAction( A_PEDIR, _HIBRIDO, " DIFERENCIA ", cad, NULL, " " );	
    /*------------------------ Pide los datos -----------------------------*/
    SET_MEMORY_DOUBLE( _importe, 
		PEDIR_FLOTANTE( izquierda + 25, arriba + 8, ENTEROS, DECIMALES ) );

    if( _IMPORTE > 0 && ( fabs( ITEM_PRECIO  - _IMPORTE ) >= 0.005 ) ) {
        for( i = 0;i < RAM_P_TICKET;i++ ) {
            if( _TICKET_COD_MOVIMIENTO( i ) == _DEVOLUCION
             && _TICKET_COD_ESPECIAL( i ) == ITEM_COD_ESPECIAL ) {
                signo = -1;
            }
        }
        if( NC_PERMANENTE && signo < 0 ) {
            signo *= -1;
        }
		//La diferencia de precio no tiene incluida el impuesto interno por eso se setea 
		//ITEM_IMP_INT = 0 para evitar el recalculo de este impuesto
		SET_MEMORY_DOUBLE( __item_imp_int, 0.00 );
		SET_MEMORY_CHAR( __item_cod_movimiento, _DIFERENCIA_PRECIO );
		if( cambio ) {
			SET_MEMORY_DOUBLE( __item_precio, signo * ( _IMPORTE + ( ITEM_PRECIO - 
				_ITEM_MONTO_IMPUESTO( 0 ) - _ITEM_MONTO_IMPUESTO( 1 ) - _ITEM_MONTO_IMPUESTO( 2 )
                - _ITEM_MONTO_IMPUESTO( 3 ) ) ) );
		} else {
			if( PRECALCULAR_IMPUESTOS ) {
				SET_MEMORY_DOUBLE( __item_precio, signo * ( _IMPORTE - ( ITEM_PRECIO -  
					_ITEM_MONTO_IMPUESTO( 0 ) - _ITEM_MONTO_IMPUESTO( 1 ) - _ITEM_MONTO_IMPUESTO( 2 )
					- _ITEM_MONTO_IMPUESTO( 3 ) ) ) );	
			} else {
				SET_MEMORY_DOUBLE( __item_precio, ROUND( signo * ( _IMPORTE - ( ITEM_PRECIO ) ),
					ENTEROS, DECIMALES ) );
			}
		}
        RECALCULA_ITEM_POR_PRECIO( ITEM_PRECIO );
		//Seteamos variable que identifica una diferencia de precio para un determinado comprobante.
		//Esta variable no permitirá que se realice el recalculo de los importes para comprobante especificado.
		if( RAM_FACTURA && !ITEM_PEDIR_PRECIO ) {
            if( ES_FACTURA_A( clientes.cond_iva ) ) {
				recalcular_precio_neto_grupo = NO;
			}
		}
		rta = 1;

	}
	if( RAM_CAMBIO ) {
		int seguir = 1;
		//aca si es cambio negativo tenemos que evitar que reste mas de lo que hay
        /*if( RAM_CAMBIO == 2 ) // Positivo
        {
            signo = 1;
        }
        else                   // Negativo
        {*/
            signo = -1;
        //}
			/*if ( RAM_CAMBIO == 1) { //CONTROLEMOS LOS IMPORTES
				double dif =0.0000;
				int desborde = 0;
				//funcion para ver la resta de totales.
				if( fabs(TOT_TICK_IMP_INT) < fabs(ITEM_IMP_INT)  )
				   desborde =1;
				dif = fabs( _TOT_TICK_IMP_TASA(ITEM_COD_IVA)) -  fabs(ITEM_IVA1_PRECIO);
				if( dif < -0.00 )
					desborde =2;
				if(fabs( RAM_TOTAL) <  fabs(_IMPORTE)   )
					desborde =3;

				if( desborde > 0 ){
					{char mensa[48];sprintf(mensa,"DESBORDE %i dif %lf", desborde, dif);
				     glog(mensa);}
					seguir = 0;
                    glog(BUSCA_MENSAJE_STRING( S_NO_SE_PUEDE_REALIZAR_CAMBIO ));
				if( fabs( RAM_TOTAL) <  fabs(_IMPORTE) ){
					seguir = 0;

					MENSAJE_CON_PAUSA( BUSCA_MENSAJE_STRING( S_NO_SE_PUEDE_REALIZAR_CAMBIO ), 18 );
				}
			}*/
		if( _IMPORTE > 0 && seguir == 1) {
            SET_MEMORY_DOUBLE( __item_precio, signo * ( _IMPORTE ) );
            SET_MEMORY_CHAR( __item_cod_movimiento, _CAMBIO_DE_MERCADERIA_MENOS );
            RECALCULA_ITEM_POR_PRECIO( ITEM_PRECIO );
            rta = 1;
        }else {
            //SET_MEMORY_DOUBLE( __item_precio, signo * ( ITEM_PRECIO ) );
             rta = 0;
        }
        //RECALCULA_ITEM_POR_PRECIO( ITEM_PRECIO );
        //rta = 1;
    }
    ExecuteAction( A_FIN_PEDIR );
    free( cad );
	CLOSE_RECUPERO( _ptr, _handle );
    return ( rta );
}
/*****************************************************************************/
void DIFERENCIA_PRECIO_AUTOMATICA( long cod_especial, int presentacion, int anulacion,
   char *cod_barra )
/*****************************************************************************/
{	
	long id_evento;
	int rta = 0, /*area = 0,*/ i = 0, posicion = 0, dif_precio = 0;
	int tabla_ant, tipo_ant;
	double cantidad_ant = 0, tot = 0;
	struct
	{
		long nro_caja_z;
		long nro_evento;
		int nro_posicion;
	} indice3;

	id_evento = RAM_ID_EVENTO;
    tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	//area = SELECTED();
	
	
	SELECT_TABLE( T_EV_CONT, TT_ORIG );
	SET_ORDER2("ID_EVENTO, POSICION");
	SET_WHERE("ID_EVENTO='%ld'", id_evento );
	RUN_QUERY(NO);
	GO2( BOTTOM );
	/*Me posiciono en el ultimo item tickeado para conocer la cantidad a la cual se le va 
	ha aplicar la diferencia de precio*/
	
	/*Cantidad del articulos a los cuales se le aplica la diferencia de precio automantica*/
	cantidad_ant = event_e_ticket.cantidad;
	/*Me posiciono al comienzo de la tabla para buscar el evento y para que quede posicionado
	correctamente en la posicion 1 el evento buscado*/
	GO2( TOP );
	//SET_ORDER( 1 ); //por id evento
	//GET_EQUAL( ( char* )&id_evento );
	SET_ORDER2("ID_EVENTO");
	SET_WHERE("ID_EVENTO='%ld'", id_evento );
	RUN_QUERY(NO);
	if( FOUND2( ) ) {
		while( /*!BtrvEOF( )*/ !dbEOF() ) {
			if( strcmp( event_e_ticket.cod_barra, cod_barra ) == 0 
				&& event_e_ticket.cod_articulo == cod_especial 
				&& event_e_ticket.origen == _ORIGEN_DIF_PRECIO )
				if( event_e_ticket.cod_movimiento == _DIFERENCIA_PRECIO ) {
					dif_precio ++;
					posicion = event_e_ticket.posicion;
				} else {
					dif_precio --;
				}
			SKIP2( 1 );
		}
	}
	if( dif_precio != 0 ) {
		indice3.nro_caja_z = event_e_ticket.caja_z;
		indice3.nro_evento = id_evento;
		indice3.nro_posicion = posicion;
		GO2( TOP );
		//SET_ORDER( 3 ); 
		//GET_EQUAL( ( char* )&indice3 );
		SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO='%ld' AND POSICION='%d'", indice3.nro_caja_z, 
						indice3.nro_evento, indice3.nro_posicion);
		RUN_QUERY(NO);
		if( FOUND2( ) ) {
			MENSAJE("ARTICULO CON DIFERENCIA DE PRECIO");
			CARGA_ITEM( RAM_P_TICKET, NULL );
			SET_MEMORY_CHAR( __ram_diferencia_precio, 1 );
			SET_MEMORY_DOUBLE( __ram_cantidad, cantidad_ant );
		}
	}
	//SELECT( area );
	SELECT_TABLE(tabla_ant, tipo_ant);

}
/*****************************************************************************/
int DIFERENCIA_PRECIO_ANULADA( long cod_especial, int presentacion, char *cod_barra,
							   long caja_z, long id_evento, int posicion )
/*****************************************************************************/
{	
	int fin = 0, rta = 0, /*area = 0,*/ i = 0, tabla_ant, tipo_ant;
	double cantidad_ant = 0, tot = 0;
	struct
	{
		long nro_caja_z;
		long nro_evento;
		int nro_posicion;
	} indice3;

    //area = SELECTED();
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

	indice3.nro_caja_z = caja_z;
	indice3.nro_evento = id_evento;
	indice3.nro_posicion = posicion;

    //GO( TOP );
	//SET_ORDER( 3 );
    //GET_EQUAL( ( char* )&indice3 );
	SELECT_TABLE( T_EV_CONT, TT_ORIG );
    SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO='%ld' AND POSICION='%d'", indice3.nro_caja_z, 
						indice3.nro_evento, indice3.nro_posicion);
	RUN_QUERY(NO);
	if( FOUND2( ) ) {
		while( /*!BtrvEOF( )*/ !dbEOF() && !fin ) {
			if( strcmp( event_e_ticket.cod_barra, cod_barra ) == 0 && 
				event_e_ticket.cod_articulo == cod_especial &&
				event_e_ticket.cod_movimiento == _DIFERENCIA_PRECIO_ANULADA ) {
				fin = 1;
			}
			SKIP2( 1 );
		}
	}
	//Se posiciona correctamente para continuar la busqueda
	if( fin ) {
		SKIP2( -1 );
	} else {
		//SET_ORDER( 3 );
		//GET_EQUAL( ( char* )&indice3 );
		SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO='%ld' AND POSICION='%d'", indice3.nro_caja_z, 
						indice3.nro_evento, indice3.nro_posicion);
		RUN_QUERY(NO);
	}
	//SELECT( area );
	SELECT_TABLE( tabla_ant, tipo_ant );
	return( fin );

}
void PANTALLA_DIFERENCIA_PRECIO( void )
/*****************************************************************************/
{
}
#ifdef COMPILAR_COMPROMISO_200
/*****************************************************************************/
int COMPROMISO_200()
/*****************************************************************************/
{
    int rta = 0;
    rta = 1;
    SET_MEMORY_CHAR( __ram_compromiso_200, 0 );
    SET_MEMORY_CHAR( __item_cod_movimiento, _COMPROMISO_200 );
    return rta;
}
#endif
/*****************************************************************************/
int PRECIO_MAS_BAJO()
/*****************************************************************************/
{
    int rta = 0;
    SET_MEMORY_CHAR( __ram_precio_mas_bajo, 0 );
    if( SOLICITAR_CLAVES( 27 - ancho_panel, 21, NO, SI, PEDIR_SUPERVISOR_COMPROMISO_200,
                          ST( S_COMPROMISO_PRECIO_MAS_BAJO ), NULL, _F_SUP_PRECIO_MAS_BAJO, SI ) ) {
        if( __COMPROMISO_PRECIO_MAS_BAJO() ) {
            rta = 1;
        }
    }
    return rta;
}
/*****************************************************************************/
int __COMPROMISO_PRECIO_MAS_BAJO()
/*****************************************************************************/
{
    int  rta = 0;
    int _ptr,_handle,_importe,_importe1;
    char aux[100];
    char izquierda = 15;
    char arriba = 7;
    char abajo = 19;
    double diferencia_calculada,multiplo;
    char *cad;
    int cant = 0;
    char aux2[100];
    char aux3[100];
    char aux4[100];
    char cadena[20];

    OPEN_RECUPERO( &_ptr, &_handle );
    _importe = STACK_INS( sizeof( double ) );
    _importe1 = STACK_INS( sizeof( double ) );

    PANTALLA_COMPROMISO_PRECIO_MAS_BAJO();
    sprintf( aux, ST( S_CODIGO_BARRA______S ), ITEM_COD_BARRA );
    sprintf( aux2, ST( S_CODIGO_INTERNO____LD ), ITEM_COD_ESPECIAL );
    sprintf( aux3, ST( S_NOMBRE_ARTICULO___S ), ITEM_NOMBRE_LARGO );
    FTOC( -( ITEM_PRECIO - _ITEM_MONTO_IMPUESTO( 0 ) - _ITEM_MONTO_IMPUESTO( 1 )
           - _ITEM_MONTO_IMPUESTO( 2 ) - _ITEM_MONTO_IMPUESTO( 3 ) ), cadena, ENTEROS, DECIMALES, 1,
          0 );
    sprintf( aux4, "%s %s", ST( S_PRECIO_ACTUAL____________ ), cadena );
    cant = strlen( aux ) + strlen( aux2 ) + strlen( aux3 ) + strlen( aux4 )
         + strlen( ST( S_PRECIO_DE_VENTA__________ ) ) + strlen( ST( S_PRECIO_DE_LA_COMPETENCIA_ ) )
         + strlen( "& &-------------------DATOS DEL ARTICULO-------------------" );
    if( ( cad = ( char* )malloc( cant + 6 ) ) != NULL ) {
        memset( cad, 0, cant + 6 );
        strcat( cad, "~" );// "~" previo a Labels con Inputs
        strncat( cad, ST( S_PRECIO_DE_VENTA__________ ),
                 strlen( ST( S_PRECIO_DE_VENTA__________ ) ) );      
        strcat( cad, "~" );
        strncat( cad, ST( S_PRECIO_DE_LA_COMPETENCIA_ ),
                 strlen( ST( S_PRECIO_DE_LA_COMPETENCIA_ ) ) );
        strcat( cad, "& &-------------------DATOS DEL ARTICULO-------------------" );
        strcat( cad, "&" );// "&" previo a labels sin input
        strncat( cad, aux, strlen( aux ) );
        strcat( cad, "&" );
        strncat( cad, aux2, strlen( aux2 ) );
        strcat( cad, "&" );
        strncat( cad, aux3, strlen( aux3 ) );
        strcat( cad, "&" );
        strncat( cad, aux4, strlen( aux4 ) );
        ExecuteSSAction( A_PEDIR, _HIBRIDO, " COMPROMISO ", cad, NULL, " " );         
        ExecuteSSAction( A_PEDIR_DATO, 0, " ", " ", NULL, " " );
    }
    /*------------------------ Pide los datos -----------------------------*/
    if( ( RAM_NOTA_CR || MODO_DEVOLUCION ) && RAM_REFERENCIA_NC && CONTROL_NOTA_DE_CREDITO ) {
        SET_MEMORY_DOUBLE( _importe,
                           fabs( ROUND( ITEM_PRECIO - _ITEM_MONTO_IMPUESTO( 0 )
                                      - _ITEM_MONTO_IMPUESTO( 1 ) - _ITEM_MONTO_IMPUESTO( 2 )
                                      - _ITEM_MONTO_IMPUESTO( 3 ), ENTEROS, DECIMALES ) ) );
    }
    else {
        SET_MEMORY_DOUBLE( _importe,
                           PEDIR_FLOTANTE( izquierda + 30, arriba + 8, ENTEROS, DECIMALES ) );
    }
    if( _IMPORTE > 0 ) {
        ExecuteSSAction( A_PEDIR_DATO, 1, " ", " ", NULL, " " );
        SET_MEMORY_DOUBLE( _importe1,
                           PEDIR_FLOTANTE( izquierda + 30, arriba + 10, ENTEROS, DECIMALES ) );
        if( _IMPORTE1 > 0 ) {
            if( _IMPORTE1 > _IMPORTE ) {
                MENSAJE_STRING( S_EL_PRECIO_DE_LA_COMPETENCIA_DEBE_SER_MENOR );
            }
            else {
                if( ITEM_PEDIR_PRECIO ) {
                    SET_MEMORY_DOUBLE( __ram_cantidad, -RAM_CANTIDAD * ITEM_PRECIO / _IMPORTE );
                }
                multiplo = ( double )MULTIPLO_PRECIO_MAS_BAJO > 0 ? MULTIPLO_PRECIO_MAS_BAJO : 1;
                diferencia_calculada = ( -_IMPORTE + _IMPORTE1 ) * multiplo;
                SET_MEMORY_DOUBLE( __item_precio, diferencia_calculada );
                SET_MEMORY_CHAR( __item_cod_movimiento, _COMPROMISO_PRECIO_MAS_BAJO );
                //SET_MEMORY_FLOAT( __item_iha_precio, 0.0 );
                //SET_MEMORY_FLOAT( __item_alicuota_precio, 0.0 );
                //SET_MEMORY_FLOAT( __item_porc_iha, 0.0 );
                //SET_MEMORY_FLOAT( __item_porc_ica, 0.0 );
                RECALCULA_ITEM_POR_PRECIO( ITEM_PRECIO );
                rta = 1;
            }
        }
    }
    ExecuteAction( A_FIN_PEDIR );
	CLOSE_RECUPERO( _ptr, _handle );
    return ( rta );
}
/*****************************************************************************/
void PANTALLA_COMPROMISO_PRECIO_MAS_BAJO( void )
/****************************************************************************/
{
}
#endif

