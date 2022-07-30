/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <btrv.h>
#include <cmemory.h>
#include <cr.h>
#include <redondeo.h>
#include <round.h>
#include <_cr1.h>
#include <math.h>
#include <db.h>
#include <deftables.h>
extern int ACTUALIZA_PANTALLA_CON_REDONDEO( void );
extern void MODIFICAR_EN_E_TICKET( int h , int exento );

/***************************************************************************/
int PRECIO_SEGUN_CANTIDAD()
/***************************************************************************/
{
    int seguir = 1, rta = 0;
    double dif;
    /*----------------- Busca la primera presentacion ---------------------*/
    /* sino se habilitan las presentaciones no se abre la tabla presentaciones por lo que 
	 cualquier acceso se debe bloquear*/
	if( !PRESENTACIONES_HABILITADAS )
		return NO;

	if(OPENED( T_PRESENT, TT_ORIG )){
		SELECT_TABLE( T_PRESENT, TT_ORIG );
		SET_WHERE(" " );
		ind_present.autoinc = (unsigned long)ITEM_AUTOINC;
		ind_present.cod_present = 0;
		/*SET_WHERE("CONCAT(AUTOINC,COD_PRESENT) >= CONCAT(%ld,%i)",
					ind_present.autoinc,
					ind_present.cod_present);
		SET_ORDER2("AUTOINC,COD_PRESENT");*/
		//SET_WHERE( "AUTOINC >= %ld",ind_present.autoinc );
		SET_WHERE( "AUTOINC = '%lu' ",ind_present.autoinc );
		//SET_ORDER2( "AUTOINC" );
		SET_ORDER2("AUTOINC,COD_PRESENT");
		//GET_GREATER_E( ( char* )&ind_present );
		RUN_QUERY(NO);
		/*------------------- Busca la presentacion seg£n la cantidad ---------*/
		while( /*present.autoinc == ( unsigned long ) ITEM_AUTOINC &&*/ !dbEOF() && seguir ) {
			if( present.habilitada == 'S' ) {
				dif = ROUND( present.cantidad - RAM_CANTIDAD, 3, 3 );
				if( dif > 0 ) {
					seguir = 0;
				}
				else {
					rta = present.cod_present;
				}
			}
			SKIP2( 1 );
		}
	}
    return ( rta );
}
/***************************************************************************/
void REDONDEAR_PRECIO2(void)
/***************************************************************************/
{
    double precio_rnd = 0.0, total_rnd = 0.0, precio_recalculado = 0.0, importe_neto_recalculado = 0.0, iva_recalculado = 0.0;
    if( TIPO_REDONDEO != 0 ) {
        //redondea precio + iva
        precio_rnd = ROUND( ITEM_PRECIO, ENTEROS, DECIMALES );
        //redondea total: (precio + iva + recargos) x cantidad
        total_rnd = ROUND( ( precio_rnd + ITEM_RECARGO ) * RAM_CANTIDAD, ENTEROS, DECIMALES );
        //recalcula el precio en base al total redondeado
        precio_recalculado = ( total_rnd / RAM_CANTIDAD ) - ITEM_RECARGO;
        //saca el importe neto recalculado
        if( CR1_ITEM_GRAVADO() ) {
            importe_neto_recalculado = precio_recalculado / ((iva[ITEM_COD_IVA].porc_iva1 / 100.0) + 1);
        }else{
            importe_neto_recalculado = precio_recalculado;
        }
        //obtiene el iva recalculado
        iva_recalculado = precio_recalculado - importe_neto_recalculado;

        //setea la rnv con los nuevos valores calculados
        SET_MEMORY_DOUBLE( __item_precio, precio_recalculado );
        SET_MEMORY_DOUBLE( __item_precio_sin_iva, importe_neto_recalculado );
        SET_MEMORY_DOUBLE( __item_iva1_precio, iva_recalculado );
    }
}


/***************************************************************************/
void REDONDEAR_PRECIO()
/***************************************************************************/
{
    int coef,dif,i;
    char ult_digito,digito;
    double precio_unit,total,resto,total_entero,division,diferencia;
    float item_imp = 0.0;
    if( TIPO_REDONDEO != 0 ) {
        total = ROUND( ( ITEM_PRECIO + ITEM_RECARGO ) * RAM_CANTIDAD, ENTEROS, DECIMALES );
        coef = ( int )pow( 10.0, ( double )DECIMALES );
        resto = modf( total, &total_entero );
        ult_digito = ( ( int )( ROUND( resto * coef, DECIMALES, 0 ) ) ) % 10;
        resto -= ( double )ult_digito / coef;
        if( ult_digito != 0 && ult_digito != ULTIMO_DECIMAL ) {
            switch( TIPO_REDONDEO ) {
                case 1:
                    /*----- REDONDEO EQUILIBRADO -----*/
                    division = ( 10 - ( double )ULTIMO_DECIMAL ) / 2;
                    dif = ult_digito - ULTIMO_DECIMAL;
                    if( dif > 0 ) {
                        digito = ( dif < division ) ? ULTIMO_DECIMAL : 10;
                    }
                    else {
                        digito = ( -dif < division ) ? ULTIMO_DECIMAL : 0;
                    }
                    break;
                case 2:
                    /*----- REDONDEO ARRIBA -----*/
                    digito = ( ult_digito > ULTIMO_DECIMAL ) ? 10 : ULTIMO_DECIMAL;
                    break;
                case 3:
                    /*----- REDONDEO ABAJO -----*/
                    digito = ( ult_digito < ULTIMO_DECIMAL ) ? 0 : ULTIMO_DECIMAL;
                    break;
            }
            /*------------ Realiza el redondeo del precio ---------------*/
            resto += ( double )digito / coef;
            //precio_unit = ( total_entero + resto - ITEM_RECARGO * RAM_CANTIDAD ) / RAM_CANTIDAD;
			precio_unit = ROUND( total / RAM_CANTIDAD - ITEM_RECARGO, ENTEROS, DECIMALES );
            SET_MEMORY_DOUBLE( __item_precio, precio_unit );
            if( CR1_ITEM_GRAVADO() ) {
                diferencia = precio_unit - ITEM_PRECIO;
                // ADD_MEMORY_DOUBLE( __item_iva1_precio, diferencia *
                // config.porc_iva1 / 100.0 );
                ADD_MEMORY_DOUBLE( __item_iva1_precio,
                                   diferencia * iva[ITEM_COD_IVA].porc_iva1 / 100.0 );
            }
            for( i = 0;i < 4;i++ ) {
                item_imp += _ITEM_MONTO_IMPUESTO( i );
            }
            SET_MEMORY_DOUBLE( __item_precio_sin_iva,
                               ITEM_PRECIO - ITEM_IVA1_PRECIO - ITEM_IVA2_PRECIO - item_imp );
        }
    }
    //fabiana 15-02-05 ** nuevo manejo de redondeos--Guardo la diferencia por item, pero del precio sin iva
    #ifdef DEBUG_R
    if( MODO_DEBUG == _DEBUG_REDONDEO ) {
        memset( cadena_debug, 0, strlen( cadena_debug ) );
        sprintf( cadena_debug, "7.Red precio, IPSI=%lf", ITEM_PRECIO_SIN_IVA, ITEM_PRECIO );
    }
    #endif
}

/***************************************************************************/
void REDONDEAR_IMPORTE( double* total, int decimal) //no anda bien
/***************************************************************************/
{
    int coef,dif;
    char ult_digito,digito;
    double precio_unit,resto,total_entero,division;
    float item_imp = 0.0;
	int tipo_redon = 1;

        *total = ROUND( *total, ENTEROS, decimal );
        coef = ( int )pow( 10.0, ( double )decimal /*DECIMALES*/ );
        resto = modf( *total, &total_entero );
        ult_digito = ( ( int )( ROUND( resto * coef, decimal /*DECIMALES*/, 0 ) ) ) % 10;
        resto -= ( double )ult_digito / coef;
	
		switch( tipo_redon ) {
                case 1:
                    /*----- REDONDEO EQUILIBRADO -----*/
                    division = ( 10 - ( double )ULTIMO_DECIMAL ) / 2;
                    dif = ult_digito - ULTIMO_DECIMAL;
                    if( dif > 0 ) {
                        digito = ( dif < division ) ? ULTIMO_DECIMAL : 10;
                    }
                    else {
                        digito = ( -dif < division ) ? ULTIMO_DECIMAL : 0;
                    }
                    break;
                case 2:
                    /*----- REDONDEO ARRIBA -----*/
                    digito = ( ult_digito > ULTIMO_DECIMAL ) ? 10 : ULTIMO_DECIMAL;
                    break;
                case 3:
                    /*----- REDONDEO ABAJO -----*/
                    digito = ( ult_digito < ULTIMO_DECIMAL ) ? 0 : ULTIMO_DECIMAL;
                    break;
            }

           /*------------ Realiza el redondeo del precio ---------------*/
            resto += ( double )digito / coef;
         //   precio_unit = ( total_entero + resto - ITEM_RECARGO * RAM_CANTIDAD ) / RAM_CANTIDAD;
			precio_unit = ROUND((*total ) , ENTEROS, decimal );
			*total = precio_unit;
			//return ( precio_unit);
		
}


/***************************************************************************/
void RECALCULAR_PRECIO_NETO()
//recalcula el precio neto en base al total redondeado
//usado para facturas que discriminan iva
/***************************************************************************/
{
    double precio_neto_s_iva, iva_recalculado, precio_neto_c_iva;
    if( CR1_ITEM_GRAVADO() ) {
        precio_neto_s_iva = (ROUND( ITEM_PRECIO_SIN_IVA * RAM_CANTIDAD, ENTEROS, DECIMALES )) / RAM_CANTIDAD;
        iva_recalculado = precio_neto_s_iva * iva[ITEM_COD_IVA].porc_iva1 / 100.0;
        precio_neto_c_iva = precio_neto_s_iva + iva_recalculado + ITEM_RECARGO;

        SET_MEMORY_DOUBLE( __item_precio, precio_neto_c_iva );
        SET_MEMORY_DOUBLE( __item_precio_sin_iva, precio_neto_s_iva );
        SET_MEMORY_DOUBLE( __item_iva1_precio, iva_recalculado );
    }

}


/***************************************************************************/
void RECALCULAR_PRECIO_NETO_GRUPO( void )
//recalcula el precio neto en base al total redondeado para todos los articulos iguales tickeados
//usado para facturas que discriminan iva
/***************************************************************************/
{
    double precio_neto_s_iva, iva_recalculado, precio_neto_c_iva;
    double importe_sin_iva_acu=0.0, iva1_acu=0.0, total_acu=0.0;
    double cant=0.0, cant_total=0.0, cant_aux = 0.0;
    int h=0, signo = 1;
    char cod_barra_tmp[20];
	double  total_XX=0.0;

	//if( !( RAM_MODO_DEVOLUCION == _DEVOL_LIBRE
            //|| RAM_MODO_DEVOLUCION == _DEVOL_PARCIAL
            //|| RAM_MODO_DEVOLUCION == _NC_LIBRE
            //|| RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL
			//|| RAM_MODO_DEVOLUCION == _NC_TOTAL
			//|| RAM_MODO_DEVOLUCION == _DEVOL_TOTAL
			//) ) {

            // Se utilizará el mismo motivo porque son operaciones similares
			if( CR1_ITEM_GRAVADO() ) {
				strncpy(cod_barra_tmp,ITEM_COD_BARRA, sizeof(cod_barra_tmp)- 1);
			for( h = 0; h < RAM_P_TICKET; h++ ) {
				if(strcmp(_TICKET_COD_BARRA(h),cod_barra_tmp)== 0 &&
						_TICKET_COD_MOVIMIENTO( h )!= _CAMBIO_DE_MERCADERIA_MENOS &&
						!(_TICKET_TIPO2( h ) & _PEDIR_PRECIO ) ) {// este item tiene la diferencia para cobrar o devolver al cliente no debe cambiarse
					signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
					cant+=_TICKET_CANTIDAD(h) * signo;
					total_acu += ((_TICKET_PRECIO(h) * _TICKET_CANTIDAD(h)) + _TICKET_RECARGO(h)) * signo;
					importe_sin_iva_acu += (_TICKET_IMPORTE_SIN_IVA(h) * _TICKET_CANTIDAD(h)) * signo;
					iva1_acu += (_TICKET_IVA1_PRECIO(h) * _TICKET_CANTIDAD(h)) * signo;
				}
			}
			//resta los valores anteriores a los totales
			ADD_MEMORY_DOUBLE( __tot_tick_importe_sin_iva, -importe_sin_iva_acu );
			ADD_MEMORY_DOUBLE( __ram_total, -total_acu );
			ADD_MEMORY_DOUBLE( __tot_tick_iva1, -iva1_acu );

			//para recalcular los importes controla si es una cancelacion para no tener en cuenta las cantidades
			//si no es una cancelacion de articulo entonces le suma la cantidad actual a las cantidad acumulada,
			//sino la resta
			//Se hace la validacion de ( RAM_CANTIDAD - cant ) de esta forna porque da error en modo release
			//de las anteriores formas. Hay error cuando la diferencia de la variable double es cero
			if( fabs(  cant - RAM_CANTIDAD ) < 0.0001 ) {
				//Si la diferencia es 0 entonces vuelvo a asignar el valor de cant para evitar problemas
				//en el redondeo.
				//Asignarle 1 a cant_total cuando ( cant - RAM_CANTIDAD ) = 0 produce error de redondeo en
				//la division.
				cant_aux = cant;
			} else {
				//Si la diferencia es distinto de 0 entonces asigno el valor de la diferencia.
				cant_aux = cant - RAM_CANTIDAD;
			}

			cant_total = ( !RAM_ANULACION ) ? RAM_CANTIDAD + cant : cant_aux;

			if (fabs(cant_total) < 0.0001 ) {
			   //cant_total = 1.00; //error
			   cant_total = cant;
			}

			if( RAM_TICKET_FACTURA == 1 && IMPRESORA_FISCAL == FISCAL )
				precio_neto_s_iva =
					(ROUND( ( ITEM_PRECIO_SIN_IVA - ITEM_IMP_INT )* cant_total, 
						ENTEROS, DECIMALES /*+ 2*/ )) / cant_total;
			else
				precio_neto_s_iva =
					(ROUND( ( ITEM_PRECIO_SIN_IVA - ITEM_IMP_INT )* cant_total, 
						ENTEROS, DECIMALES )) / cant_total;
			iva_recalculado = precio_neto_s_iva * iva[ITEM_COD_IVA].porc_iva1 / 100.0;
			precio_neto_c_iva = precio_neto_s_iva + iva_recalculado + ITEM_IMP_INT + ITEM_RECARGO ;

			//setea la total x sin tener en cuenta el importe actual ya que el mismo es acumulado normalmente
			ADD_MEMORY_DOUBLE( __tot_tick_importe_sin_iva, ( ( precio_neto_s_iva + ITEM_IMP_INT )* cant ) );
			//setea la ram total sin tener en cuenta el importe actual ya que el mismo es acumulado normalmente
			total_XX = RAM_TOTAL;
			ADD_MEMORY_DOUBLE( __ram_total, (precio_neto_c_iva * cant) );
			//setea el iva total sin tener en cuenta el importe actual ya que el mismo es acumulado normalmente
			ADD_MEMORY_DOUBLE( __tot_tick_iva1, ( iva_recalculado * cant ) );

			//setea los valores actuales
			SET_MEMORY_DOUBLE( __item_precio, precio_neto_c_iva );
			SET_MEMORY_DOUBLE( __item_precio_sin_iva, precio_neto_s_iva + ITEM_IMP_INT );
			SET_MEMORY_DOUBLE( __item_iva1_precio, iva_recalculado );

			//setea los importes nuevos en todos los articulos iguales, hasta en los cancelados...
			for( h = 0; h < RAM_P_TICKET /*&& RAM_TICKET_FACTURA != 1*/; h++ ) {
					//MISMO ITEM
				if( strcmp( _TICKET_COD_BARRA( h ),cod_barra_tmp ) == 0 &&
						// ESTE TIENE DISTINO IMPORTE NO SE DEBE TOCAR
					_TICKET_COD_MOVIMIENTO( h ) != _CAMBIO_DE_MERCADERIA_MENOS &&
							//SI ES IGUAL PARA QUE TOCARLO..
						precio_neto_c_iva != _TICKET_PRECIO( h )  &&
					!(_TICKET_TIPO2( h ) & _PEDIR_PRECIO )){
					_SET_MEMORY_DOUBLE( __ticket_precio, h, precio_neto_c_iva );
					_SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, h, precio_neto_s_iva + ITEM_IMP_INT );
					_SET_MEMORY_DOUBLE( __ticket_iva1_precio, h, iva_recalculado );
					//CAMBIOS EN ESTRUC _TICKET
					GRABAR_TICKET( h );
					if( ACTUALIZA_PANTALLA_CON_REDONDEO( ) ){
					//CAMBIOS EN PANTALLA
						CR1_MOSTRAR_ITEM( h, 9, SI );
					}
					//CAMBIOS EN EL GRABADO
					MODIFICAR_EN_E_TICKET( h, NO );
				}
			}
		}
	//}
}

/***************************************************************************/
void RECALCULAR_PRECIO_NETO_GRUPO2()
//recalcula el precio neto en base al total redondeado
//usado para facturas que discriminan iva
/***************************************************************************/
{
    double precio_neto_s_iva, iva_recalculado, precio_neto_c_iva;



    if( CR1_ITEM_GRAVADO() ) {
        precio_neto_s_iva = (ROUND( ITEM_PRECIO_SIN_IVA * RAM_CANTIDAD, ENTEROS, DECIMALES )) / RAM_CANTIDAD;
        iva_recalculado = precio_neto_s_iva * iva[ITEM_COD_IVA].porc_iva1 / 100.0;
        precio_neto_c_iva = precio_neto_s_iva + iva_recalculado + ITEM_RECARGO;

        SET_MEMORY_DOUBLE( __item_precio, precio_neto_c_iva );
        SET_MEMORY_DOUBLE( __item_precio_sin_iva, precio_neto_s_iva );
        SET_MEMORY_DOUBLE( __item_iva1_precio, iva_recalculado );
    }

}

/***************************************************************************/
void RECALCULAR_PRECIO_NETO_GRUPO3( void )
//pone todos los importes sin iva iguales para el motor de promociones
/***************************************************************************/
{
    double precio_neto_s_iva, iva_recalculado, precio_neto_c_iva;
    double importe_sin_iva_acu=0.0, iva1_acu=0.0, total_acu=0.0, ii_acumu = 0.0;
    double cant=0.0, cant_total=0.0, cant_aux = 0.0;
    int h=0, signo = 1;
    char cod_barra_tmp[20];
	double  total_XX=0.0;

            // Se utilizará el mismo motivo porque son operaciones similares
			if( CR1_ITEM_GRAVADO() ) {
				strncpy(cod_barra_tmp,ITEM_COD_BARRA, sizeof(cod_barra_tmp)- 1);
			for( h = 0; h < RAM_P_TICKET; h++ ) {
				if(strcmp(_TICKET_COD_BARRA(h),cod_barra_tmp)== 0 &&
						_TICKET_COD_MOVIMIENTO( h )!= _CAMBIO_DE_MERCADERIA_MENOS &&
						!(_TICKET_TIPO2( h ) & _PEDIR_PRECIO ) ) {// este item tiene la diferencia para cobrar o devolver al cliente no debe cambiarse
					signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
					cant+=_TICKET_CANTIDAD(h) * signo;
					total_acu += ((_TICKET_PRECIO(h) * _TICKET_CANTIDAD(h)) + _TICKET_RECARGO(h)) * signo;
					importe_sin_iva_acu += ROUND((_TICKET_IMPORTE_SIN_IVA(h) * _TICKET_CANTIDAD(h)) * signo, ENTEROS, 2);
					iva1_acu += (_TICKET_IVA1_PRECIO(h) * _TICKET_CANTIDAD(h)) * signo;
					ii_acumu +=  _TICKET_IMP_INT(h) * signo ;
				}
			}

			//para recalcular los importes controla si es una cancelacion para no tener en cuenta las cantidades
			//si no es una cancelacion de articulo entonces le suma la cantidad actual a las cantidad acumulada,
			//sino la resta
			//Se hace la validacion de ( RAM_CANTIDAD - cant ) de esta forna porque da error en modo release
			//de las anteriores formas. Hay error cuando la diferencia de la variable double es cero
			if( fabs(  cant - RAM_CANTIDAD ) < 0.0001 ) {
				//Si la diferencia es 0 entonces vuelvo a asignar el valor de cant para evitar problemas
				//en el redondeo.
				//Asignarle 1 a cant_total cuando ( cant - RAM_CANTIDAD ) = 0 produce error de redondeo en
				//la division.
				cant_aux = cant;
			} else {
				//Si la diferencia es distinto de 0 entonces asigno el valor de la diferencia.
				cant_aux = cant - RAM_CANTIDAD;
			}

			cant_total = ( !RAM_ANULACION ) ? RAM_CANTIDAD + cant : cant_aux;

			if (fabs(cant_total) < 0.0001 ) {
			   //cant_total = 1.00; //error
			   cant_total = cant;
			}

			if(( RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1 )&& IMPRESORA_FISCAL == FISCAL )
				precio_neto_s_iva =
					(ROUND( ( importe_sin_iva_acu - ii_acumu ), 
						ENTEROS, DECIMALES /*+ 2*/ )) / (cant_total - 1);
			else
				precio_neto_s_iva =
					(ROUND( ( ITEM_PRECIO_SIN_IVA - ITEM_IMP_INT )* cant_total, 
						ENTEROS, DECIMALES )) / cant_total;
			iva_recalculado = precio_neto_s_iva * iva[ITEM_COD_IVA].porc_iva1 / 100.0;
			precio_neto_c_iva = precio_neto_s_iva + iva_recalculado + ITEM_IMP_INT + ITEM_RECARGO ;


			if( fabs(cant_total) > 2 ) {
					//setea los valores actuales
					SET_MEMORY_DOUBLE( __item_precio, precio_neto_c_iva );
					SET_MEMORY_DOUBLE( __item_precio_sin_iva, precio_neto_s_iva + ITEM_IMP_INT );
					SET_MEMORY_DOUBLE( __item_iva1_precio, iva_recalculado );

				//setea los importes nuevos en todos los articulos iguales, hasta en los cancelados...
				for( h = 0; h < RAM_P_TICKET /*&& RAM_TICKET_FACTURA != 1*/; h++ ) {
						//MISMO ITEM
					if( strcmp( _TICKET_COD_BARRA( h ),cod_barra_tmp ) == 0 &&
							// ESTE TIENE DISTINO IMPORTE NO SE DEBE TOCAR
						_TICKET_COD_MOVIMIENTO( h ) != _CAMBIO_DE_MERCADERIA_MENOS &&
								//SI ES IGUAL PARA QUE TOCARLO..
							precio_neto_c_iva != _TICKET_PRECIO( h )  &&
						!(_TICKET_TIPO2( h ) & _PEDIR_PRECIO )){
						_SET_MEMORY_DOUBLE( __ticket_precio, h, precio_neto_c_iva );
						_SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, h, precio_neto_s_iva + ITEM_IMP_INT );
						_SET_MEMORY_DOUBLE( __ticket_iva1_precio, h, iva_recalculado );
						//CAMBIOS EN ESTRUC _TICKET
						GRABAR_TICKET( h );
						MODIFICAR_EN_E_TICKET( h, NO );
					}
				}
			}
			}
	//}
}



/***************************************************************************/
double REDONDEAR( double importe, int tipo, double minima_denominacion )
/***************************************************************************/
{
    int tipo_r, cant_dec, bandera = 0;
    double modulo=0.00;
    long md,l_importe,l_md;
	if( ( minima_denominacion > 0 ) ) {
        tipo_r = tipo;
        cant_dec = DECIMALES;
        md = ( long )minima_denominacion;
        if( importe < 0 ) {
            importe *= ( -1 );
            bandera = 1;
        }
        importe = ( importe )*pow( 10, cant_dec );
        l_importe = ( long )ceil( importe );
        if( ( minima_denominacion * pow( 10, cant_dec ) ) >= 1 ) {
            md = ( long )( minima_denominacion * pow( 10, cant_dec ) );
        }
        else {
            md = 1;
        };
        l_md = md;
        switch( tipo_r ) {
            case 1:
                /*-------- PARA ARRIBA ---------*/
                modulo = ( l_importe % l_md );
                if( modulo != 0 ) {
                    importe = ( importe - modulo + md );
                }
                else {
                    importe = l_importe;
                };
                break;
            case 2:
                /*-------- PARA ABAJO ----------*/
                modulo = ( l_importe % l_md );
                if( modulo != 0 ) {
                    importe = ( importe - modulo );
                }
                else {
                    importe = l_importe;    //- l_md;
                };
                break;
            case 3:
                /*-------- SIMETRICO -----------*/
                modulo = ( l_importe % l_md );
                if( modulo != 0 ) {
                    if( modulo < ( md / 2 ) ) {
                        md = 0;
                    }
                    importe = ( l_importe - modulo + md );
                }
                else {
                    importe = ( double )l_importe;
                }
                break;
        }
        if( importe < md ) {
            importe = 0;
        }
        importe = importe / pow( 10, cant_dec );
        if( bandera ) {
            return ( importe *= ( -1 ) );
        }
        else {
            return importe;
        }
    }
    else {
        return ( 0.00 );
    }
}

/***************************************************************************/
void DIFERENCIA_REDONDEO_CAJA_IMPRESORA( )
/*
Esta funcion tendria que desaparecer con todo lo que se hizo de redondeo desde la version 17 en adelante
*/
/***************************************************************************/
{
	double dif_impresora_precio_sin_iva = 0.00;
	double comp_decimal = 0.00;
	int  haydife=0, signo = 1;
    double xFactor = 5; //para tratar de ajustar la diferencia de redondeo.
    double iconiva = 0.00, iconiva_redondeado=0.00, dife =0.00;

	/*if( RAM_FACTURA == 1 || RAM_TICKET_FACTURA == 1 )
	{*/
		//Se realiza un analisis previo de como se pasará el importe redondeado a la impresora fiscal
		//para evitar diferencias de centavos entre la pantalla y la impresion. La caja trabaja con
		//todos los decimales mientras que la impresora TM220 trabaja solo con dos decimales.
		//La diferencia radica que en comprobantes donde se discrimina el iva se pasa a la impresora
		//el precio sin iva y esta (la impresora) calculo el iva en funcion del porcentaje que tambien le
		//pasamos. En comprobantes donde no discrimina el iva le pasamos el precio con iva.
	if( CR1_ITEM_GRAVADO() ) {
		dif_impresora_precio_sin_iva =  ROUND( ITEM_PRECIO_SIN_IVA, ENTEROS, DECIMALES )
				- CORTE_HASTA_DECIMALES( ITEM_PRECIO_SIN_IVA, DECIMALES + 2 );


		comp_decimal = DECIMAL_COMPARATIVO( DECIMALES + 2 );
		// control extra veremos si anda

		if(( RAM_FACTURA == 1 || RAM_TICKET_FACTURA == 1 ) && ITEM_PEDIR_PRECIO == 0 ) { //los articulos por precio no lo tocamos
		   //veamos el importe con iva cuanto difiere entre lo de la caja y lo que haria la impresora
		   iconiva =ITEM_PRECIO_SIN_IVA * RAM_CANTIDAD ; //11.33
		   iconiva_redondeado =  ROUND( ROUND(ITEM_PRECIO_SIN_IVA,ENTEROS,/*2*/ 4) * RAM_CANTIDAD , ENTEROS, 2 )  ; //impresora 11.34

		   dife = fabs(iconiva_redondeado) - fabs(iconiva) ; // 11.34 - 11.33 = 0.01
		   if( fabs( dife ) > 0.009 ) { //aca corregimos
			   haydife = 1;
			   if( dife < 0.00 )
				   signo = -1;

			   if( fabs(RAM_CANTIDAD) > 0.00 ) { //14.22/5.85=2.430769.... - 2.43
					xFactor = iconiva_redondeado / RAM_CANTIDAD  ;
			   }
		   }
		   if( haydife == 0 && fabs(RAM_CANTIDAD) > 1) { // PARA LOS QUE VIENEN MULTIPLICADOS Y PUEDEN PRODUCIR DIRERENCIAS
				iconiva =ITEM_PRECIO_SIN_IVA * RAM_CANTIDAD * ((iva[ITEM_COD_IVA].porc_iva1 / 100.0  ) + 1) ; //62.073
   				iconiva_redondeado =  ROUND( iconiva , ENTEROS, 2 )  ; //impresora 62.07

   				dife = fabs(iconiva) - fabs(iconiva_redondeado) ; // 62.073 - 62,07 = 0.003
				if( fabs( dife ) > 0.002 ) { //aca corregimos
					haydife = 1;
					if( dife > 0.00 )
						signo = -1;

   			   if( fabs(RAM_CANTIDAD) > 0.00 ) {
   					xFactor = iconiva_redondeado / RAM_CANTIDAD / ((iva[ITEM_COD_IVA].porc_iva1 / 100.0  ) + 1)  ; //2,699869508481
			      }
		      }
         }
      }

		if(( dif_impresora_precio_sin_iva && fabs( dif_impresora_precio_sin_iva ) >= comp_decimal && ITEM_PEDIR_PRECIO == 0)
			 || haydife == 1)
		{
		  if((IMPRESORA_FISCAL == FISCAL && iva[ITEM_COD_IVA].porc_iva1 > 0.00 )
			   && (/*RAM_FACTURA == 1 ||*/ RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1 )){

			   double factor_correccion   =0.00;

			 /*  if( haydife == 1)
				   factor_correccion= fabs(( ITEM_PRECIO_SIN_IVA - xFactor)) * signo;
			   else*/
				   factor_correccion= 0;//comp_decimal * xFactor * signo;

				SET_MEMORY_DOUBLE( __item_precio_sin_iva, ITEM_PRECIO_SIN_IVA + factor_correccion ); //truchada
				SET_MEMORY_DOUBLE( __item_precio,
						( ( ( ITEM_PRECIO_SIN_IVA - ITEM_IMP_INT ) * iva[ITEM_COD_IVA].porc_iva1 ) / 100.0 ) 
						+ ITEM_PRECIO_SIN_IVA );
				SET_MEMORY_DOUBLE( __item_iva1_precio,
						( ( ITEM_PRECIO_SIN_IVA - ITEM_IMP_INT ) * iva[ITEM_COD_IVA].porc_iva1 ) / 100.0 );
			} else {
				/*Para la impresora tm220 cuando el importe sin iva del articulo en el padron no estaba bien la diferencia
				entre el valor del articulo en el browse y en el que ponia en el subtotal en pantalla difiere y luego el total
				del ticket de la impresora con el de la pantalla es distinto
				*/
				SET_MEMORY_DOUBLE( __item_precio,ROUND( ITEM_PRECIO, ENTEROS, DECIMALES ));
				SET_MEMORY_DOUBLE( __item_precio_sin_iva, 
					( ( ITEM_PRECIO - ITEM_IMP_INT ) / ((iva[ITEM_COD_IVA].porc_iva1 / 100.0  ) + 1)) + ITEM_IMP_INT );
				SET_MEMORY_DOUBLE( __item_iva1_precio,
					( ( ITEM_PRECIO_SIN_IVA - ITEM_IMP_INT ) * iva[ITEM_COD_IVA].porc_iva1) / 100.0 );
			}
		}
	}
}
/***************************************************************************/
double CORTE_HASTA_DECIMALES( double precio, int decimal )
/***************************************************************************/
{
	//Tomamos el valor por parametro y copiamos hasta que complete la cantidad
	//de decimales configurados
	int h, fin = 0, decimales = 0;
	double doublevalor = 0.00;
	char redondeo[50], cadena[50];

	_snprintf( cadena, sizeof(cadena)-1, "%*.*f", ENTEROS, decimal /*DECIMALES + 1*/, precio );

	for( h = 0; h <= strlen( cadena ) - 1 && !fin; h++ )
	{
		redondeo[h] = cadena[h];
		if( cadena[h] == '.' || decimales )
		{
			if( decimales++ == decimal )
			{
				redondeo[h + 1]= '\0';
				doublevalor = strtod( redondeo, NULL );
				fin = 1;
			}
		}
	}
	return( doublevalor );
}

/***************************************************************************/
double DECIMAL_COMPARATIVO( int decimal )
/***************************************************************************/
{
	char comp_decimal[50];
	int h, fin = 0, decimales = 0;
	double doubledecimal = 0;
	//Tomamos el decimal comparativo segun cantidad de decimales pasados por parametro
	comp_decimal[0] = '0';
	comp_decimal[1] = '.';
	for( h = 2; h < decimal + 2 && !fin; h++ )
	{
		comp_decimal[h] = '0';
		if( ++decimales == decimal )
		{
			comp_decimal[h] = '1';
			comp_decimal[h+1] = '\0';
			doubledecimal = strtod( comp_decimal, NULL );
			fin = 1;
		}
	}

	return( doubledecimal );
}


/***************************************************************************/
double TRUNCAR(double f, int n) 
/***************************************************************************/
{ 
	float d = pow(10,n); 
 	return ((long)(d * f)) / d; 
}
