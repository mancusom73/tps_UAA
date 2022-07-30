#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <baucher.h> 
#include <comprob.h> 
#include <cr.h>  
#include <dbrouter.h> 
#include <ftoc.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <imp_comp.h> 
#include <lista.h> 
#include <m_cadena.h> 
#include <m_caract.h> 
#include <print.h> 
#include <tarjetas.h> 
#include <tkt.h> 
#include <valida.h> 
#include <_cr0.h> 
#include <_cr1.h> 
#include <ini.h> 
#include <cmemory.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
void OBTIENE_NOMBRE_CLIENTE( char *nombre, int p_pago );
void _VOUCHER_BLOQUE1( char *titulo, long anulacion );
void _VOUCHER_BLOQUE2();
extern struct _tarjetas_fisc tarjetas_fisc_rec[10];
extern struct _comprobante comp;
void IMPRIMIR_MENSAJES_PROMOCIONALES(int para_comercio);
extern int RECUPERAR_POSICION_TARJETA_FISCAL( int h );
/*****************************************************************************/
int IMPRIME_VOUCHER( int p, int para_comercio, long anulacion )
/*****************************************************************************/
{
    unsigned marca,op, op1;
/*
cuando declaro este tipo de vectores y como subindice una variable
de la ram estilo las var_tarjetas, se debe controlar los posibles lugares..

*/

    char *cmoneda[] = {"3","Pesos","D¢lares","Cecor"};
	char *c_op[] = {"13","Compra","On-line Compra","Off-line Compra","Anulacion","On-line Anulacion",
		"Off-line Anulacion","Devolucion","On-line Devolucion","Off-line Devolucion","On-line Anulacion Devo.","Off-line Anulacion Dev.","On-Line Extraccion","On-Line Anul Extracc"};
    char *signo_moneda[] = {"3","$  ","u$s","   "},
          c_importe[20], aux[25];
    char *c_tipo_cuenta[] = {"4"," ","CAJA DE AHORRO","CUENTA CORRIENTE"};
    char nombre[26], devolucion = 0;
	char caracter_ast[2], autorizacion[15];
	int  marca_tarjeta = 0;
	char temp[21], numero_tarjeta[21];
	int      l = 0;
	double    importe_aux = 0.0;

	memset( c_importe, 0, sizeof( c_importe ) );
	memset( aux,0, sizeof(aux));
	memset( nombre, 0, sizeof( nombre ) );
	memset( caracter_ast, 0, sizeof( caracter_ast ) );
	memset( autorizacion, 0, sizeof( autorizacion ) );
	memset( temp, 0, sizeof( temp ) );
	memset( numero_tarjeta, 0, sizeof( numero_tarjeta ) );

	caracter_ast[0]='*';
	caracter_ast[1]='\0';

    if( !EMITIR_VOUCHER_TARJETA || ( EMITIR_VOUCHER_TARJETA == 1 && !para_comercio ) ) {
        return ( 0 );
    }
    if( RAM_NOTA_CR || MODO_DEVOLUCION ) {
        devolucion = 1;
		if(  VAR_TARJETAS_OPERACION == _ON_LINE_DEVOLUCION || VAR_TARJETAS_OPERACION == _OFF_LINE_DEVOLUCION)
			anulacion = 0;
    }
    marca = VAR_TARJETAS_TARJETA;

    OBTIENE_NOMBRE_CLIENTE( nombre, p/*RAM_P_PAGO*/ );
    _VOUCHER_BLOQUE1( NOMBRE_TARJETA( marca ), anulacion );
    MOSTRAR_CADENA( 1, PRN, ST( S_ESTABLEC_______ ) );
    MOSTRAR_CADENA( 17, PRN, VAR_TARJETAS_NRO_COMERCIO );
    LF( 2 );
    MOSTRAR_CADENA( 1, PRN, ST( S_TIPO___________ ) );
    if( VAR_TARJETAS_TIPO_CUENTA ) {
        MOSTRAR_CADENA( 17, PRN, ST( S_TARJETA_DE_DEBITO ) );
    }
    else {
        MOSTRAR_CADENA( 17, PRN, ST( S_TARJETA_DE_CREDITO ) );
    }
    LF( 1 );
//    MOSTRAR_CADENA( 1, PRN, ST( S_TARJETA________ ) );
	MOSTRAR_CADENA( 1, PRN, ST( S_TARJETA_N_ )  );

	marca_tarjeta = VAR_TARJETAS_TARJETA;
	_snprintf( numero_tarjeta, sizeof( numero_tarjeta ), "%s",GET_NUMERO_TARJETA_PRIVADO( ));
//posnet quiere los 12 primeros digitos enmascarados por eso lo de ENCRIPTAR_DATOS_DE_TARJETAS_PRIV no va
/*	if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ) {
		if( TARJ[marca_tarjeta].enmasc_nro_tarjeta == 's'
			|| TARJ[marca_tarjeta].enmasc_nro_tarjeta == 'S' )
		{
			strncpy( &numero_tarjeta[2], MASCARA( '*', 6 ), 6 );
		}
		strncpy( temp, numero_tarjeta, sizeof( temp ) -1 );
	} else {**/
	ENMASCARA_NUMERO_TARJETA( marca_tarjeta, numero_tarjeta, temp );
//	}
	MOSTRAR_CADENA( 10, PRN, temp );
//    LF( 1 );
    //MOSTRAR_CADENA( 30, PRN, ST( S_VENCIMIENTO____ ) );
	MOSTRAR_CADENA( 31, PRN, "Vto:" );
	ENMASCARA_FECHA_TARJETA( 35 );
    //MOSTRAR_FECHA( 17, PRN, GET_FECHA_VTO_PRIVADA(  )  );
	if( VAR_TARJETAS_NRO_CUENTA[0] != 0 ) {
		LF( 1 );
		MOSTRAR_CADENA( 1, PRN,"Nro. Cta.:" );
		MOSTRAR_CADENA( 17, PRN, VAR_TARJETAS_NRO_CUENTA );
	}

    LF( 1 );
	MOSTRAR_CADENA( 1, PRN, ST( S_CUP_N_N________ ) );
	if( VAR_TARJETAS_NRO_CUPON > 0)
    MOSTRAR_ENTERO( 17, PRN, "000000000", VAR_TARJETAS_NRO_CUPON );
	else
		MOSTRAR_ENTERO( 17, PRN, "000000000", VAR_TARJETAS_NRO_CUPON_INT );
    LF( 1 );
    /*MOSTRAR_CADENA( 1, PRN, ST( S_COMPROBANTE_N__ ) );
    MOSTRAR_ENTERO( 17, PRN, "000000000", VAR_TARJETAS_NRO_CUPON_INT );
    LF( 1 );*/
    MOSTRAR_CADENA( 1, PRN, ST( S_FECHA__________ ) );
    MOSTRAR_FECHA( 17, PRN, GET_FECHA_DOS() );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_C_DIGO_DE_AUT__ ) );
	
	strncpy( autorizacion, VAR_TARJETAS_AUTORIZACION, sizeof( autorizacion ) - 1 );
	TRIM(autorizacion);
	l = strlen( autorizacion );
	if( l > 0  )
		MOSTRAR_CADENA( 17, PRN, VAR_TARJETAS_AUTORIZACION );
    else
   		if( VAR_TARJETAS_ON_LINE != 1 ) {
			MOSTRAR_CADENA( 17, PRN, MASCARA('0', 6, "") ); 
	}
    if( VAR_TARJETAS_ON_LINE ) {
        MOSTRAR_CADENA( 28, PRN, caracter_ast );
    }
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_OPERACI_N______ ) );
	
	

    op = VAR_TARJETAS_ON_LINE;
	
    if( anulacion ) {
        op += 3;
		if( RAM_NOTA_CR > 0 &&  devolucion)
			op += 5;
    }
    else if( devolucion ) {
        op += 6;
    }
    op1=atoi(c_op[0]);
	if( op > op1)
		op=0;
    
	strncpy( aux, c_op[op+1] , sizeof( aux ) );// +1 porque la primera posicion es la cantidad de posiciones del vector
	MOSTRAR_CADENA( 17, PRN, aux );
//agregamos el tipo de lectura
	 LF( 1 );
	 MOSTRAR_CADENA( 1, PRN, "Captura        :" );
	if( VAR_TARJETAS_LECTURA_BANDA == 1 )
		 MOSTRAR_CADENA( 17, PRN, "LECTURA BANDA" );
	else
		 MOSTRAR_CADENA( 17, PRN, "LECTURA MANUAL"  );

	
	//MOSTRAR_CADENA( 17, PRN, (char*) c_op[op] );
    LF( 1 );
	if( p == -1 )
      importe_aux = VAR_TARJETAS_IMPORTE;
    else
      importe_aux = _PAGO_IMPORTE( p );

    MOSTRAR_CADENA( 1, PRN, ST( S_IMPORTE________ ) );
    if( VAR_TARJETAS_IMPORTE_ENTREGA ) {
        FTOC( importe_aux + VAR_TARJETAS_IMPORTE_ENTREGA, c_importe, ENTEROS, DECIMALES, 0,
              0 );
    }
    else {
        FTOC( importe_aux, c_importe, ENTEROS, DECIMALES, 0, 0 );
    }
	
	op = MONEDA_EXTENDIDA();
	if( op > atoi(signo_moneda[0]))
		op=0;
    strncpy( aux, signo_moneda[op+1], sizeof( aux ) );
    strncat( aux, c_importe, sizeof( aux ) - strlen( aux ) - 1 );
    //IMPRIME_GRANDE( 17, aux );
	//
	IMPRIME_GRANDE( 11, aux );
	if(config_tps.ActivarCashPlus == 1 && VAR_TARJETAS_ANTICIPO > 0.005){
		double cashplus = (double) VAR_TARJETAS_ANTICIPO;
		FTOC(cashplus, c_importe, ENTEROS, DECIMALES, 0, 0 );
		MOSTRAR_CADENA( 1, PRN, "Extraccion    :" );
		MOSTRAR_CADENA( 17, PRN,c_importe );
	}
	MOSTRAR_CADENA( 23, PRN, ST( S_CANT_DE_CUOTAS_ ) );
    MOSTRAR_ENTERO( 37, PRN, "00", VAR_TARJETAS_CUOTAS );
    if( VAR_TARJETAS_IMPORTE_ENTREGA ) {
        LF( 1 );
        MOSTRAR_CADENA( 1, PRN, ST( S_ENTREGA________ ) );
        MOSTRAR_FLOTANTE( 17, PRN, ENTEROS + 1, DECIMALES, 0, 1, VAR_TARJETAS_IMPORTE_ENTREGA );
        LF( 1 );
        MOSTRAR_CADENA( 1, PRN, ST( S_SALDO__________ ) );
        MOSTRAR_FLOTANTE( 17, PRN, ENTEROS + 1, DECIMALES, 0, 1, _PAGO_IMPORTE( p ) );
    }
    
    if( VAR_TARJETAS_COEFICIENTE ) {
		LF( 1 );
        MOSTRAR_CADENA( 1, PRN, ST( S_IMPORTE_CUOTAS_ ) );
        MOSTRAR_FLOTANTE( 17, PRN, ENTEROS + 1, DECIMALES, 0, 1,
                          _PAGO_IMPORTE( p ) * VAR_TARJETAS_COEFICIENTE );
        LF( 1 );
    }
    if( VAR_TARJETAS_PRIMER_VTO ) {
		LF( 1 );
        MOSTRAR_CADENA( 1, PRN, ST( S_PRIMER_VTO_____ ) );
        MOSTRAR_FECHA( 17, PRN, VAR_TARJETAS_PRIMER_VTO );
        LF( 1 );
    }
    if( VAR_TARJETAS_TIPO_CUENTA ) {
		LF( 1 );
        MOSTRAR_CADENA( 1, PRN, ST( S_TIPO_DE_CUENTA_ ) );
		op=sizeof(c_tipo_cuenta);
		
		 op = VAR_TARJETAS_TIPO_CUENTA;
		if( op > atoi(c_tipo_cuenta[0]))
			op=0;
        MOSTRAR_CADENA( 17, PRN, c_tipo_cuenta[( int )op+1] );
        //LF( 1 );
    }
    //MOSTRAR_CADENA( 1, PRN, ST( S_MONEDA_________ ) );
	if( _VAR_TARJETAS_TABLA( 0 ) != 0 && _VAR_TARJETAS_TABLA( 0 ) != ' ' ) {
        LF( 1 );
        //MOSTRAR_CADENA( 1, PRN, ST( S_N__DE_TABLA____ ) );
		MOSTRAR_CADENA( 1, PRN, "Nro de Plan  :"  );
        MOSTRAR_CADENA( 15, PRN, VAR_TARJETAS_TABLA );
    }
	MOSTRAR_CADENA( 21, PRN, "Moneda:" );
	op=sizeof(cmoneda);
	op=MONEDA_EXTENDIDA();
	if( op > atoi(cmoneda[0]))
		op=0;
	strncpy( aux, (char *) cmoneda[op+1] , sizeof( aux ) );
    //MOSTRAR_CADENA( 17, PRN, aux );
	MOSTRAR_CADENA( 31, PRN, aux );
    if( VAR_TARJETAS_COD_VIGENCIA != 0 ) {
        LF( 1 );
        {
            char aux[25];
            _snprintf(aux, sizeof(aux)-1, "%s      :", ST( S_VIGENCIA ) );
            MOSTRAR_CADENA( 1, PRN, aux );
        }
        MOSTRAR_ENTERO( 17, PRN, "00", VAR_TARJETAS_COD_VIGENCIA );
    }
    LF( 1 );
	if(  devolucion == 1) {
		MOSTRAR_CADENA( 1, PRN, "Cupon Original:");
        MOSTRAR_ENTERO( 17, PRN, "00000000", VAR_TARJETAS_CUPON_ORIGINAL );
		LF( 1 );
		if(  VAR_TARJETAS_OPERACION != _ON_LINE_ANULACION && VAR_TARJETAS_OPERACION != _OFF_LINE_ANULACION
			&& VAR_TARJETAS_OPERACION != _ON_LINE_ANULACION_DEVOLUCION  && VAR_TARJETAS_OPERACION !=_OFF_LINE_ANULACION_DEVOLUCION ) {
			MOSTRAR_CADENA( 1, PRN, "Fecha Original:");
			MOSTRAR_FECHA( 17 , PRN, VAR_TARJETAS_FECHA_ORIGINAL );
			LF( 1 );
		}

	}

    if( VAR_TARJETAS_ON_LINE ) {
        MOSTRAR_CADENA( 1, PRN, ST( S_TERMINAL_N_____ ) );
        MOSTRAR_ENTERO( 17, PRN, "00000000", VAR_TARJETAS_TERMINAL );
     //   LF( 1 );
        MOSTRAR_CADENA( 26, PRN, "Lote Nro.:"/*ST( S_LOTE_____N_____ ) */);
        MOSTRAR_ENTERO( 36, PRN, "000", VAR_TARJETAS_LOTE );
        LF( 1 );
    }
    _VOUCHER_BLOQUE2();
    if( IMPRIME_MENSAJE_TARJETA( marca ) && _VAR_TARJETAS_MENSAJE( 0 ) ) {
		//Segun el manual si el mensaje empieza con:
		/* "." Se debe mostrar en el display e imprimir en los voucher y no tiene 1 linea de longitud de no mas de 40 caracteres
		"," No se debe mostrar en display y debe imprimirse en los vouchers
		";" Solo imprimir en el voucher del comercio
		":" Solo voucher del cliente

		*/
		IMPRIMIR_MENSAJES_PROMOCIONALES( para_comercio);
/*		char mensaje1[201];
		char *respuesta[7];
		char s2[2] ="|";
		int   i, cant_lineas = 0;
		char *p1;

		memset(mensaje1,0,sizeof(mensaje1));
		strncpy(mensaje1, RAM_BUFFER, 200);
		s2[1]= '\0';

		if( mensaje1[0] == '.' || mensaje1[0] == ',' 
			|| ( para_comercio && mensaje1[0] == ';') || ( !para_comercio && mensaje1[0] == ':') ){ //se debe imprimir

			respuesta[0] = malloc( 40 );
			respuesta[1] = malloc( 40 );
			respuesta[2] = malloc( 40 );
			respuesta[3] = malloc( 40 );
			respuesta[4] = malloc( 40 );
			respuesta[5] = malloc( 40 );
			respuesta[6] = malloc( 40 );
			mensaje1[0]=' '; //asi no imprime el primer caracter

			p1 = strtok( mensaje1 , s2 );
			while( p1 != NULL && cant_lineas < 7) {   
				strncpy( respuesta[cant_lineas], p1, 40 );
				p1 = strtok( NULL, s2 );
				cant_lineas++;
			}
			 for( i = 0; i < cant_lineas && cant_lineas > 0 ; i++ ){
				 if( respuesta[i] != NULL && strlen( respuesta[i]) > 1) {
						LF( 1 );
						MOSTRAR_CADENA( 1, PRN, respuesta[i] );
				 }
			 }

			  for( i = 0; i< 7; i++) {
				  if( respuesta[i]) {
						free( respuesta[i] );
						respuesta[i] = NULL;
					}
			}
		}*/
    }
    LF( 1 );
    //if( IMPRESORA_FISCAL != FISCAL_IBM_4610 && !anulacion ) {
        //VALIDAR_VOUCHER( COD_VALIDACION_VOUCHER( marca ) );
    //}
    if( para_comercio ) {
        if( LINEAS_EN_BLANCO_EN_VOUCHER_TARJ && VAR_TARJETAS_LECTURA_BANDA != 1 ) {
            LF( LINEAS_EN_BLANCO_EN_VOUCHER_TARJ );
        }
        LF( 3 );
        MOSTRAR_CADENA( 1, PRN, ST( S_FIRMA_____ ) );
        MOSTRAR_CADENA( 12, PRN, BUSCA_MENSAJE_STRING( S_LINEA ) );
        LF( 2 );
        MOSTRAR_CADENA( 1, PRN, ST( S_NOMBRE____ ) );
		if( nombre[0] && strcmp( nombre, "NO" ) != 0 ) {
           MOSTRAR_CADENA( 12, PRN, nombre );
        }
        else {
            MOSTRAR_CADENA( 12, PRN, ST( S_LINEA ) );
        }
        LF( 2 );
        MOSTRAR_CADENA( 1, PRN, ST( S_DOCUMENTO_ ) );
        if( PEDIR_DATOS_CLIENTE_TARJETA && *( char* )RAM_TARJ_DOCUMENTO ) {
            MOSTRAR_CADENA( 12, PRN, ( char* )RAM_TARJ_DOCUMENTO );
        }
        else {
            MOSTRAR_CADENA( 12, PRN, ST( S_LINEA ) );
        }
        LF( 2 );
        MOSTRAR_CADENA( 1, PRN, ST( S_DIRECCI_N_ ) );
        MOSTRAR_CADENA( 12, PRN, BUSCA_MENSAJE_STRING( S_LINEA ) );
        LF( 2 );
        MOSTRAR_CADENA( 1, PRN, ST( S_TEL_FONO__ ) );
        if( PEDIR_DATOS_CLIENTE_TARJETA && *( char* )RAM_TARJ_TELEFONO ) {
            MOSTRAR_CADENA( 12, PRN, RAM_TARJ_TELEFONO );
        }
        else {
            MOSTRAR_CADENA( 12, PRN, ST( S_LINEA ) );
        }
    }
    LF( 2 );
    if( anulacion || devolucion ) {
        if( anulacion ) {
            MOSTRAR_CADENA( 1, PRN, ST( S_EL_COMERCIANTE_RECONOCE_LA_ANULACI_N ) );
        }
        else {
            MOSTRAR_CADENA( 1, PRN, ST( S_EL_COMERCIANTE_RECONOCE_LA_DEVOLUCI_N ) );
        }
        LF( 1 );
        MOSTRAR_CADENA( 1, PRN, ST( S_DE_LA_OPERACI_N_POR_EL_IMPORTE_TOTAL ) );
        LF( 1 );
        MOSTRAR_CADENA( 1, PRN, ST( S_PRECEDENTEMENTE_CONSIGNADO_ ) );
    }
    else {
        MOSTRAR_CADENA( 1, PRN, ST( S_SE_AUTORIZA_EL_PAGO_DEL_TICKET_ ) );
    }
    if( EMITIR_VOUCHER_TARJETA != 1 ) {
        LF( 1 );
        if( para_comercio ) {
            MOSTRAR_CADENA( 1, PRN, ST( S_PARA_EL_COMERCIO_ ) );
        }
        else {
            MOSTRAR_CADENA( 1, PRN, ST( S_PARA_EL_CLIENTE_ ) );
        }
    }
    PRN_FIN_TICKET();
    return 1;
}
/*****************************************************************************/
void OBTIENE_NOMBRE_CLIENTE( char *nombre, int p_pago )
/*****************************************************************************/
{
    char *p1 = NULL, *p2 = NULL, *p3 = NULL, track1[80];
    int l, jj=0, salir = 0, pos_fiscal = 0;
	char numero_tarjeta[21];
	char numero_tarjeta_aux[21];

    memset( nombre, 0, 26 );
	memset( track1, 0, 80 );
	memset( numero_tarjeta, 0, sizeof(numero_tarjeta) );
	memset( numero_tarjeta_aux, 0, sizeof(numero_tarjeta_aux) );

	_snprintf( numero_tarjeta, sizeof( numero_tarjeta ), "%s",GET_NUMERO_TARJETA_PRIVADO( ));
	numero_tarjeta[sizeof( numero_tarjeta ) - 1] = '\0';

	//busquemos 
	do {
		_snprintf( numero_tarjeta_aux, sizeof( numero_tarjeta_aux ),
			"%s",tarjetas_fisc_rec[jj].numero_tarjeta );
		numero_tarjeta_aux[sizeof( numero_tarjeta_aux ) - 1] = '\0';
		if( strcmp( numero_tarjeta, numero_tarjeta_aux ) == 0 ) {
			salir = 1;
		} else {
		   jj++;
		}
		memset( numero_tarjeta_aux, 0, sizeof(numero_tarjeta_aux) );
	} while ( jj < 10 && salir != 1);

	if( salir == 1 &&  jj < 10) {
		p_pago = jj ;
	}
	//%B5275728697807033^MANCUSO HECTRO M          ^1402101010266906370803972? track1
	pos_fiscal = RECUPERAR_POSICION_TARJETA_FISCAL( p_pago );
	memcpy( track1, tarjetas_fisc_rec[pos_fiscal].track1, sizeof( track1 ) );
    
	if( track1[0] == 0) {
		memcpy( track1, GET_TRACK_1_PRIVADO( ), sizeof( track1 ) );
	}
    p3 = track1;
    l = track1[0];
    p1 = (char *)memchr( p3, '^', 80 - 2 );
    if( p1 && p3[0]!= 0 ) {
        p2 = (char *)memchr( &p1[1], '^', 80 - ( p1 - p3 ) );
    }
    else {
		p1 = (char *)memchr( p3, '&', 80 - 2 );
		if( p1 && p3[0]!= 0 ) {
			p2 = (char *)memchr( &p1[1], '&', 80 - ( p1 - p3 ) );
		} else {
        if( config.pais == ECUADOR ) {
            if( l > 0 && p3[1] == 'B' ) {
                p1 = &p3[1];
                p2 = p1 + strlen( p1 );
            }
        }
		else
			sprintf(nombre, "NO", sizeof("NO"));//Seba
    }
	}
    if( p1 && p2 ) {
        l = ( int )( p2 - p1 - 1 );
        if( l > 25 ) {
            l = 25;
        }
        if( l > 0 ) {
            memcpy( nombre, &p1[1], l );
            nombre[l] = 0;
        }
    }
}
/*****************************************************************************/
void IMPRIME_VOUCHER_CTACTE( int p, int para_comercio, long anulacion, int modo )
/*****************************************************************************/
{
    char c_importe[20],aux[20];
    if( modo == _MODO_COBRANZA && para_comercio ) {
        IMPRIME_PIE_COMPROBANTE( IMPRIMIR_COBROS, NO );
    } else
      ABRIR_DOCUMENTO_NO_FISCAL( NO );

    _VOUCHER_BLOQUE1( ST( S_CUENTA_CORRIENTE ), anulacion );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_CUENTA_N_______ ) );
    if( UTILIZA_SOCIOS ) {
        MOSTRAR_ENTERO( 17, PRN, "00000000", _PAGO_NUMERO( p ) );
    }
    else {
        MOSTRAR_ENTERO( 17, PRN, "0000", _PAGO_CUENTA( p ) );
    }
    LF( 1 );
	
	
    MOSTRAR_CADENA( 1, PRN, ST( S_IMPORTE________ ) );
	

	FTOC( (_PAGO_IMPORTE( p )*((RAM_NOTA_CR)? -1:1 )), c_importe, ENTEROS, DECIMALES, SI, 0 );

    //FTOC( _PAGO_IMPORTE( p ), c_importe, ENTEROS, DECIMALES, 0, 0 );
    //strcpy( aux, "$" );
	/*if(RAM_NOTA_CR)
		strcpy( aux, "$-" );
	else*/
		strcpy( aux, "$" );
    strcat( aux, c_importe );
    IMPRIME_GRANDE( 17, aux );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_COMPROBANTE_N__ ) );
    MOSTRAR_ENTERO( 17, PRN, "00000000", _PAGO_NRO_CUPON_INT1( p ) );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_FECHA__________ ) );
    MOSTRAR_FECHA( 17, PRN, GET_FECHA_DOS() );
    LF( 1 );

    if( _PAGO_BUFFER1_LONG( p ) ) {
		MOSTRAR_CADENA( 1, PRN, ST( S_C_DIGO_DE_AUT__ ) );
        MOSTRAR_ENTERO( 17, PRN, "999999", _PAGO_BUFFER1_LONG( p ) );
		LF( 1 );
    }
    _VOUCHER_BLOQUE2();
    LF( 1 );
    if( para_comercio ) {
        LF( 1 );
        MOSTRAR_CADENA( 1, PRN, ST( S_FIRMA___ ) );
        MOSTRAR_CADENA( 10, PRN, BUSCA_MENSAJE_STRING( S_LINEA ) );
        LF( 1 );
    }
    if( UTILIZA_SOCIOS ) {
        MOSTRAR_CADENA( 1, PRN, ST( S_NOMBRE__ ) );
        MOSTRAR_CADENA( 10, PRN, CLIENTE_AFINIDAD_NOMBRE );
        LF( 1 );
    }
    LF( 1 );
    if( para_comercio ) {
        MOSTRAR_CADENA( 1, PRN, ST( S_PARA_EL_COMERCIO_ ) );
    }
    else {
        MOSTRAR_CADENA( 1, PRN, ST( S_PARA_EL_CLIENTE_ ) );
    }
	LF( 1 );
    PRN_FIN_TICKET();
}
/*****************************************************************************/
void _VOUCHER_BLOQUE1( char *titulo, long anulacion )
/*****************************************************************************/
{
    PRN_SELECT( RECEIPT );
    PRN_LETRA_GRANDE();
    MOSTRAR_CADENA( 1, PRN, titulo );
    PRN_LETRA_NORMAL();
    LF( 2 );
    if( anulacion ) {
        MOSTRAR_CADENA( 1, PRN, ST( S_A_N_U_L_A_C_I_O_N ) );
        LF( 1 );
        MOSTRAR_CADENA( 1, PRN, ST( S_DEL_COMPROBANTE_N__ ) );
        MOSTRAR_ENTERO( 21, PRN, "000000000", anulacion );
        LF( 2 );
    }
    MOSTRAR_CADENA( 1, PRN, config.nom_comercio );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_C_U_I_T________ ) );
    MOSTRAR_CADENA( 17, PRN, config.cuit );
    LF( 1 );
}
/*****************************************************************************/
void _VOUCHER_BLOQUE2()
/*****************************************************************************/
{
    char cadena[30];
	char caracter_sig[2];
	caracter_sig[0]='-';
	caracter_sig[1]='\0';
	
	MOSTRAR_CADENA( 1, PRN, ST( S_T_P_V____N_____ ) );
    MOSTRAR_ENTERO( 17, PRN, "0000", NUMERO_CAJA() );
    LF( 1 );
	MOSTRAR_CADENA( 1, PRN, "Sucursal:"/*ST(S_SUCURSAL_N_____ )*/ );
    MOSTRAR_ENTERO( 11, PRN, "000", config.sucursal );
    //LF( 1 );
    MOSTRAR_CADENA( 16, PRN, ST( S_OPERADOR_N_____ ) );
	memset(cadena, 0, sizeof(cadena));
    if( IMPRIMIR_LEGAJO_CAJERO ) {
		_snprintf( cadena, sizeof(cadena)-1,"%ld", NRO_LEGAJO_CAJERO );
		MOSTRAR_ENTERO( 31, PRN, MASCARA( '0', 8, cadena ),	NRO_LEGAJO_CAJERO );
    }
    else {
		_snprintf( cadena, sizeof(cadena)-1,"%d", NRO_CAJERO );
		MOSTRAR_ENTERO( 31, PRN, MASCARA( '0', 4, cadena ), NRO_CAJERO );
    }
    LF( 1 );
    //MOSTRAR_CADENA( 1, PRN, ST( S_HORA___________ ) );
	MOSTRAR_CADENA( 1, PRN, "Hora :" );
    MOSTRAR_HORA( 12, PRN, GET_HORA_DOS() );
    if( EMITIR_FACTURA() ) {
		LF( 1 );
        MOSTRAR_CADENA( 1, PRN, ST( S_FACTURA__N_____ ) );
        MOSTRAR_CARACTER( 17, PRN, ( char )( 'A' + comp.subtipo - 1 ) );
        MOSTRAR_ENTERO( 19, PRN, "0000", comp.nro_suc );
        MOSTRAR_CADENA( 23, PRN, caracter_sig );
        MOSTRAR_ENTERO( 24, PRN, "00000000", comp.nro_comp );
    }
    else {
		memset(cadena, 0, sizeof(cadena));
		_snprintf( cadena, sizeof(cadena)-1,"%ld", NRO_TICKET );
		MOSTRAR_CADENA( 19, PRN, ST( S_TICKET___N_____ ) );
        MOSTRAR_ENTERO( 32, PRN, MASCARA( '0', 5, cadena ), NRO_TICKET );
	}
}

/*****************************************************************************/
void IMPRIMIR_MENSAJES_PROMOCIONALES(int para_comercio)
/*****************************************************************************/
{
	    char mensaje1[201];
		char *respuesta[7];
		char s2[2] ="|";
		int   i, cant_lineas = 0;
		char *p1;

		memset(mensaje1,0,sizeof(mensaje1));
		strncpy(mensaje1, RAM_BUFFER, 200);
		s2[1]= '\0';

		if( mensaje1[0] == '.' || mensaje1[0] == ',' 
			|| ( para_comercio && mensaje1[0] == ';') || ( !para_comercio && mensaje1[0] == ':') ){ //se debe imprimir

			respuesta[0] = malloc( 40 );
			respuesta[1] = malloc( 40 );
			respuesta[2] = malloc( 40 );
			respuesta[3] = malloc( 40 );
			respuesta[4] = malloc( 40 );
			respuesta[5] = malloc( 40 );
			respuesta[6] = malloc( 40 );
			mensaje1[0]=' '; //asi no imprime el primer caracter

			p1 = strtok( mensaje1 , s2 );
			while( p1 != NULL && cant_lineas < 7) {   
				strncpy( respuesta[cant_lineas], p1, 40 );
				p1 = strtok( NULL, s2 );
				cant_lineas++;
			}
			 for( i = 0; i < cant_lineas && cant_lineas > 0 ; i++ ){
				 if( respuesta[i] != NULL && strlen( respuesta[i]) > 1) {
						LF( 1 );
						MOSTRAR_CADENA( 1, PRN, respuesta[i] );
				 }
			 }

			  for( i = 0; i< 7; i++) {
				  if( respuesta[i]) {
						free( respuesta[i] );
						respuesta[i] = NULL;
					}
			}
		}
}


#ifdef COMPILAR_CMR
/*****************************************************************************/
void MINI_VOUCHER_CMR()
/*****************************************************************************/
{
    int marca;
    char nom_vigencia[21];
    /*---------------------- Importe de entrega para CMR ---------------*/
    if( VAR_TARJETAS_IMPORTE_ENTREGA ) {
        MOSTRAR_CADENA( 1, PRN, ST( S_ABONO____ ) );
        MOSTRAR_FLOTANTE( 12, PRN, ENTEROS, DECIMALES, 1, 0, VAR_TARJETAS_IMPORTE_ENTREGA );
        LF( 1 );
    }
    marca = VAR_TARJETAS_TARJETA;
    if( IMPRIME_MENSAJE_TARJETA( marca ) && _VAR_TARJETAS_MENSAJE( 0 ) ) {
        MOSTRAR_CADENA( 1, PRN, VAR_TARJETAS_MENSAJE );
        LF( 1 );
    }
    if( BUSCAR_VIGENCIA( marca, VAR_TARJETAS_COD_VIGENCIA, nom_vigencia ) ) {
        {
            char aux[25];
            sprintf( aux, "%s      :", ST( S_VIGENCIA ) );
            MOSTRAR_CADENA( 1, PRN, aux );
        }
        MOSTRAR_CADENA( 17, PRN, nom_vigencia );
        LF( 1 );
    }
    if( VAR_TARJETAS_PRIMER_VTO ) {
        MOSTRAR_CADENA( 1, PRN, ST( S_PRIMER_VTO_____ ) );
        MOSTRAR_FECHA( 17, PRN, VAR_TARJETAS_PRIMER_VTO );
        LF( 1 );
    }
    MOSTRAR_CADENA( 1, PRN, ST( S_CANT_DE_CUOTAS_ ) );
    MOSTRAR_ENTERO( 17, PRN, "00", VAR_TARJETAS_CUOTAS );
    LF( 1 );
    VALIDAR_VOUCHER( COD_VALIDACION_VOUCHER( marca ) );
    LF( 2 );
    if( RAM_NRO_F12 ) {
        LF( 1 );
        MOSTRAR_CADENA( 1, PRN, ST( S_NUMERO_REMITO__ ) );
        MOSTRAR_ENTERO( 17, PRN, "00000000", RAM_NRO_F12 );
    }
    LF( 2 );
    MOSTRAR_CADENA( 1, PRN, ST( S_FIRMA_____ ) );
    MOSTRAR_CADENA( 12, PRN, BUSCA_MENSAJE_STRING( S_LINEA ) );
    LF( 2 );
    MOSTRAR_CADENA( 1, PRN, ST( S_DOCUMENTO_ ) );
    MOSTRAR_CADENA( 12, PRN, BUSCA_MENSAJE_STRING( S_LINEA ) );
    LF( 1 );
}
#endif



