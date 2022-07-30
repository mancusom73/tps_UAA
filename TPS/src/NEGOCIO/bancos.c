#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <global.h>
#include <aarch.h>
#include <actions.h>
#include <bancos.h>
//#include <browse.h> 
#include <btrv.h> 
#include <b_mens.h> 
#include <cr.h>  
#include <cmemory.h>
#include <dbrouter.h> 
#include <getch.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <pagos.h> 
#include <pant.h> 
#include <stringt.h> 
#include <tkt.h> 
#include <trim.h> 
#include <_cr1.h> 
#include <deftables.h>
#include <db.h>
#include <str.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_W
//#include <btrv.h>
//#include <actions.h>
#endif
#define _X1  32
#define _X2  53
#define _Y1   5
#define _Y2  20
#define CANTIDAD_BANCOS_POR_CLIENTE 2
#define CANTIDAD_CUENTA_BANCOS_POR_CLIENTE 2
#define LONGITUD_CUENTA_BANCARIA 10
struct _cliecheq *cheques_clientes;
/*****************************************************************************/
int _consulta_bancos( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0, y, cod_banco = 0;
    y = _Y1 + dato;

    switch( comando ) {
		case _BUSCAR:
			cod_banco = atoi( cadena );
			SET_WHERE("COD_BANCO >= '%d'", cod_banco );
			SET_ORDER2( "COD_BANCO");
			RUN_QUERY(NO);
			break;
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[10];
                _itoa( bancos.nro_banco, xx, 10 );
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, bancos.nom_banco );
            }
            #endif
            MOSTRAR_CADENA( _X1, y, bancos.nom_banco );
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbBOF() || dbEOF();
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
            rta = 1;
            break;
		case _TECLA:
			if( dato == -73 || dato == -81 /*|| ( dato >= '0' && dato <= '9' ) */)
				b->tecla = 0;
			break;
        case _COMPARAR:
            rta = 2;
            break;
		case _LIMIT:
            if( dato > 0 ) {
                SET_LIMIT( dato, SI );
            } else {
                SET_LIMIT( NO, NO );
            }
            break;
		case _VALIDAR:
			if( isdigit( dato ) ) {
                rta = 1;
            }
    }
    return ( rta );
}
/*****************************************************************************/
int _consulta_bancos2( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0;
    long indice;
    char xx[20];
    switch( comando ) {
        case _BUSCAR:
            MOSTRAR_CADENA( 21 - ancho_panel / 2, 21, cadena );
            indice = strtol( cadena, NULL, 10 );
            //GET_GREATER_E( ( char* )&indice );
			if(cadena){
				SET_WHERE("COD_BANCO >= '%li'", indice);
				SET_ORDER2( "COD_BANCO" );
				RUN_QUERY(NO);
			}            
			break;
        case _MOSTRAR:
            _itoa( bancos.nro_banco, xx, 10 );
            setFieldValuepChar( b->index, 0, xx );
            setFieldValuepChar( b->index, 1, bancos.nom_banco );
            break;
        case _VACIO:
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbBOF() || dbEOF();
            break;
        case _VALIDAR:
            if( isdigit( dato ) ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
            rta = 1;
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
/***************************************************************************/
int PIDE_BANCO( char *cuenta_banco )
/***************************************************************************/
{
    int rta = 0, banco = 0;
    if( PEDIR_CLIENTE_EN_CHEQUES && CONTROLAR_CUPO_EN_CHEQUES ) {
        banco = PIDE_BANCO_CLIENTE( cuenta_banco );
    }
    else {
        /*---------------------- Prepara archivo ----------------------------*/
        //SELECT( AREA_BANCO );
        if( /*USE_DB( AREA_BANCO, _BANCOS_SIC, ( char* )&bancos, sizeof( bancos ), NULL, 0, 0 )*/
			OPEN_TABLE(T_BANCOS, TT_ORIG ,( char* )&bancos,sizeof( bancos ))== 0 ) {
			SET_WHERE("");
			SET_ORDER2( "COD_BANCO" );
			RUN_QUERY(NO);
            if( MODO_NEGOCIO == SERVICIOS ) {
                rta = Consulta_bancos( _consulta_bancos2 );
            }
            else {
                rta = Consulta_bancos( _consulta_bancos );
            }
            /*--------------------- finaliza ---------------------*/
            //CLOSE_DATABASE( AREA_AUX );
            CLOSE_TABLE( T_BANCOS, TT_ORIG );
        }
        if( rta ) {
            banco = bancos.nro_banco;
        }
    }
    return ( banco );
}
/***************************************************************************/
int PIDE_BANCO_CLIENTE( char *cuenta_banco )
/***************************************************************************/
{
    int rta = 0, banco = 0;
    char auxi[80];
    char hab_banco[CANTIDAD_BANCOS_POR_CLIENTE];
    int cant_banco = 0, j;
    int seguir = 1, k;
    int arriba = 15, izquierda = 30;
    /*-------------------- Prepara la pantalla -----------------------*/
    memset( hab_banco, 0, CANTIDAD_BANCOS_POR_CLIENTE );
    if( /*USE_DB( AREA_AUX, _BANCOS_SIC, ( char* )&bancos, sizeof( bancos ), NULL, 0, 0 )*/
		OPEN_TABLE(T_BANCOS, TT_ORIG ,( char* )&bancos, sizeof( bancos ))== 0 ) {
			SELECT_TABLE( T_BANCOS, TT_ORIG );
        for( j = 0;j < CANTIDAD_BANCOS_POR_CLIENTE;j++ ) {
            if( clientes.cuentas[j].cod_banco ) {
                cant_banco++;
                if( !EXISTE_BANCO__( clientes.cuentas[j].cod_banco ) ) {
                    hab_banco[j] = 0;
                    cant_banco--;
                    MENSAJE_STRING( S_NO_ENCUENTRA_BANCO );
                }
                else {
                    hab_banco[j] = 1;
                }
            }
        }
        if( cant_banco > 1 ) {
            for( j = 0;j < CANTIDAD_CUENTA_BANCOS_POR_CLIENTE;j++ ) {
                if( hab_banco[j] ) {
                    EXISTE_BANCO__( clientes.cuentas[j].cod_banco );
                    _snprintf( auxi, sizeof(auxi)-1, "%d %s", j + 1, bancos.nom_banco );
                    MOSTRAR_CADENA( izquierda + 2, arriba + 1 + j, auxi );
                }
            }
            while( seguir ) {
                k = GETCH();
                switch( k ) {
                    case 27:
                        /*-------- ESC ---------*/
                        seguir = 0;
                        banco = 0;
                        break;
                    default:
                        j = k - '1';
                        if( j >= 0 && j < CANTIDAD_BANCOS_POR_CLIENTE && hab_banco[j] ) {
                            banco = clientes.cuentas[j].cod_banco;
                            if( !PIDE_CUENTA_BANCO_CLIENTE( banco, cuenta_banco ) ) {
                                banco = 0;
                            }
                            else {
                                seguir = 0;
                            }
                        }
                        break;
                }
            }
        }
        else if( cant_banco == 1 ) {
            for( j = 0;j < CANTIDAD_BANCOS_POR_CLIENTE;j++ ) {
                if( hab_banco[j] ) {
                    banco = clientes.cuentas[j].cod_banco;
                    if( !PIDE_CUENTA_BANCO_CLIENTE( banco, cuenta_banco ) ) {
                        banco = 0;
                    }
                }
            }
        }
        else {
            MENSAJE_STRING( S_NO_HAY_BANCO_INGRESADO );
        }
    }
    else {
        MENSAJE_STRING( S_ERROR_ABRIR_TABLA_BANCOS );
    }
    //SELECT( AREA_AUX );
    CLOSE_TABLE( T_BANCOS, TT_ORIG );
    /*---------------------- Prepara archivo ----------------------------*/
    if( rta ) {
        banco = bancos.nro_banco;
    }
    return ( banco );
}
/********************************************************************************/
int PIDE_NRO_BANCO()
/********************************************************************************/
{
    int numero;
    ExecuteSSAction( A_PEDIR, 1, "BANCO", ST( S__NUMERO_DE_BANCO_ ), NULL, " " );
    MOSTRAR_CADENA( 32, 21, ST( S__NUMERO_DE_BANCO_ ) );
    _ATRIBUTO = 120;
    numero = ( int )PEDIR_GRAN_ENTERO( 36, 22, 3 );
    if( numero > 0 ) {
        if( !EXISTE_BANCO( numero ) ) {
            MENSAJE_STRING( S_NUMERO_BANCO_INEXISTENTE );
            numero = 0;
            BORRAR_MENSAJE();
        }
    }
    ExecuteAction( A_FIN_PEDIR );
    return ( numero );
}
/*****************************************************************************/
int EXISTE_BANCO( int nro )
/*****************************************************************************/
{
    int ok = 0;
    if( OPEN_TABLE(T_BANCOS, TT_ORIG ,( char* )&bancos, sizeof( bancos ))== 0 ) {
		SELECT_TABLE( T_BANCOS, TT_ORIG );
		SET_WHERE("COD_BANCO = '%i'",nro);
        ok = FOUND2();
        CLOSE_TABLE( T_BANCOS, TT_ORIG );
    }
    return ( ok );
}
/*****************************************************************************/
int EXISTE_BANCO__( int nro )
/*****************************************************************************/
{
    int ok = 0;
    /*GET_EQUAL( ( char* )&nro );*/
	SET_WHERE("COD_BANCO = '%i'",nro);
	SET_ORDER2("COD_BANCO");
	RUN_QUERY(NO);
    ok = FOUND2();
    return ( ok );
}
/***************************************************************************/
int PIDE_CUENTA_BANCO_CLIENTE( int banco, char *cuenta_banco )
/***************************************************************************/
{
    int rta = 1;
    char auxi[50],temp[60];
    char hab_cuenta[CANTIDAD_CUENTA_BANCOS_POR_CLIENTE];
    int cant_cuenta = 0, j;
    int seguir = 1, k;
    int arriba = 17, izquierda = 30;
    /*-------------------- Prepara la pantalla -----------------------*/
    if( PEDIR_CUENTA_BANCARIA ) {
        rta = 0;
        for( j = 0;j < CANTIDAD_CUENTA_BANCOS_POR_CLIENTE;j++ ) {
            memcpy( temp, clientes.cuentas[banco].cod_cuenta[j], LONGITUD_CUENTA_BANCARIA );
            TRIM( temp );
            if( temp[0] ) {
                cant_cuenta++;
                hab_cuenta[j] = 1;
            }
        }
        if( cant_cuenta > 1 ) {
            for( j = 0;j < CANTIDAD_CUENTA_BANCOS_POR_CLIENTE;j++ ) {
                if( hab_cuenta[j] ) {
                    memcpy( temp, clientes.cuentas[banco].cod_cuenta[j], LONGITUD_CUENTA_BANCARIA );
                    TRIM( temp );
                    _snprintf( auxi, sizeof(auxi)-1, "%d %s", j + 1, temp );
                    MOSTRAR_CADENA( izquierda + 2, arriba + 1 + j, auxi );
                }
            }
            while( seguir ) {
                k = GETCH();
                switch( k ) {
                    case 27:
                        /*-------- ESC ---------*/
                        cuenta_banco[0] = 0;
                        seguir = 0;
                        rta = 0;
                        break;
                    default:
                        j = k - '1';
                        if( j >= 0 && j < CANTIDAD_CUENTA_BANCOS_POR_CLIENTE && hab_cuenta[j] ) {
                            memcpy( cuenta_banco, clientes.cuentas[banco].cod_cuenta[j],
                                    LONGITUD_CUENTA_BANCARIA );
                            seguir = 0;
                            rta = 1;
                        }
                        break;
                }
            }
        }
        else if( cant_cuenta == 1 ) {
            cuenta_banco[0] = 0;
            for( j = 0;j < CANTIDAD_BANCOS_POR_CLIENTE;j++ ) {
                if( hab_cuenta[j] ) {
                    memcpy( cuenta_banco, clientes.cuentas[banco].cod_cuenta[j],
                            LONGITUD_CUENTA_BANCARIA );
                    rta = 1;
                }
            }
        }
        else {
            MENSAJE_STRING( S_NO_HAY_CUENTAS_INGRESADAS );
        }
    }
    /*---------------------- Prepara archivo ----------------------------*/
    return ( rta );
}
/*****************************************************************************/
int ELIMINA_CHEQUE( int cheque, int banco, char *cheque_str )
/*****************************************************************************/
{
    // Busca el cheque numero "cheque" en la tabla cliecheq.
//    int old_area;
    int ok = 1;
	
    
	if( !REGISTRAR_CHEQUES_EN_ARCHIVO ) {
        return( 1 );
    }
    if( MODO_NEGOCIO == SERVICIOS ) {
		int  area_ant, tipo_ant;
		area_ant = SELECTED2();
		tipo_ant = SELECTED_TIPO();
        ok = 0;
        // solo para SERVICIOS
      //  old_area = SELECTED();
      //  CLOSE_DATABASE( AREA_CHEQUE_CLIENTE );
      //  SELECT( AREA_CHEQUE_CLIENTE );
      //  USE_DB( AREA_CHEQUE_CLIENTE, _CLI_CHEQ_SIC, ( char* )cheques_clientes,
      //          sizeof( struct _cliecheq ), NULL, 0, 0 );
      if( OPEN_TABLE( T_CLI_CHEQ, TT_ORIG, ( char* )&cheques_clientes, sizeof( struct _cliecheq ) ) == 0 ) {
		SELECT_TABLE( T_CLI_CHEQ, TT_ORIG );
		RUN_QUERY(NO);
        GO2( TOP );
		while( !dbBOF() ) {
            if( strcmp( cheques_clientes->nro_cheque_str, cheque_str ) == 0
             && ( cheques_clientes->cod_banco == banco ) ) {
                #ifdef INVEL_L
                //BEGIN_TRANSACTION();
                #endif
                //_DELETE(); //
				 DELETE2();
                #ifdef INVEL_L
                //END_TRANSACTION();
                #endif                
                ok = 1;
            }
            SKIP2( 1 );
        }
        //CLOSE_DATABASE( AREA_CHEQUE_CLIENTE );
		 CLOSE_TABLE( T_CLI_CHEQ, TT_ORIG );
        //SELECT( old_area );
		 SELECT_TABLE( area_ant, tipo_ant );
	  }
		
	}
    return( ok );
}
/***************************************************************************/
int GRABA_CHEQUE_CLIENTE( int h, int pos, int borrar )
/***************************************************************************/
{
    /*
        Guarda un cheque en cliecheq.sic, 
            si el cheque ya existe y borrar en 0 retorna 0,
            si el cheque ya existe y borrar es 1 lo borra
        si fue guardado correctamente retorna 1
    */
    int rta = 0;
    struct _datos datos[5];
    struct _indice
    {
        long nro_cliente;
        UINT16 cod_banco;
        long nro_cheque;
    };
    struct _indice indice;
    int cod_banco = 0, seguir = 1;
    char nro_cheque[15];
	int  area_ant, tipo_ant;
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    if( !REGISTRAR_CHEQUES_EN_ARCHIVO ) {
        return ( 1 );
    }
    if( MODO_NEGOCIO == SERVICIOS ) {
        // Modo Servicios
        /* Busco que el cheque no exista */
        //SELECT( AREA_CHEQUE_CLIENTE );
        /* Si el cheque no existe lo inserto en ClieCheq.sic */
        if( ABRIR_CHEQUES_CLIENTES() == 0 ) {
            //SET_ORDER( 1 );
			SET_ORDER2("COD_CLIENTE, ESTADO, FECHA_VTO, COD_BANCO, NRO_CHEQUE");
            // el cheque esta guardado en d_tmp_pr.xxx
            _PAGOS_OBTENER_DATOS( pos, h, &datos[0], 3 );            
            cod_banco = atoi( ( &datos[2] )->valor );
            strcpy( nro_cheque, ( &datos[1] )->valor );
            //SELECT( AREA_CHEQUE_CLIENTE );            
            //SET_ORDER( 1 );
			SET_ORDER2("COD_CLIENTE, ESTADO, FECHA_VTO, COD_BANCO, NRO_CHEQUE");
			RUN_QUERY(NO);
            GO2( TOP );
            while( seguir && !dbEOF() ) {
                if( strcmp( cheques_clientes->nro_cheque_str, nro_cheque ) == 0 && 
                    ( cheques_clientes->cod_banco == cod_banco ) ) {
                    //el cheque está en la cliecheq
                    seguir = 0;
                    if( borrar ) {
                        //_DELETE();
						DELETE2();
                    }
                }
                //SKIP( 1 );
				 SKIP2( 1 );
            }
            if( !borrar ) {
                if( seguir ) {
                    // si no lo encuentra lo inserta
                    /*
                        &datos[0] = Cuit
                        &datos[1] = Numero cheque
                        &datos[2] = Banco
                        &datos[3] = Fecha
                        &datos[4] = Monto
                    */
                    memset( cheques_clientes, 0, sizeof( struct _cliecheq ) );
                    cheques_clientes->importe = _PAGO_IMPORTE( h );
                    cheques_clientes->nro_cliente = 0; // RAM_NRO_CLIENTE;
                    cheques_clientes->cod_banco = atoi( ( &datos[2] )->valor );
                    memcpy( cheques_clientes->nro_cuenta, _PAGO_BUFFER( h ), 10 );
                    cheques_clientes->nro_cheque = atoi( ( &datos[1] )->valor );
                    cheques_clientes->importe = atof( ( &datos[4] )->valor );
                    //cheques_clientes->fecha_vencimiento = ( unsigned short ) ( &datos[3] )->valor;
                    cheques_clientes->fecha_vencimiento = 0;
                    cheques_clientes->dia_operativo = GET_FECHA_DOS();
                    cheques_clientes->hora = GET_HORA_DOS();
                    cheques_clientes->caja = NRO_CAJA;
                    cheques_clientes->cajero = NRO_CAJERO;
                    cheques_clientes->estado = 0;         // no acreditado
                    memcpy( cheques_clientes->cod_banco_str, 0, strlen( cheques_clientes->cod_banco_str ) );
                    memcpy( cheques_clientes->nro_cheque_str, 0, strlen( cheques_clientes->nro_cheque_str ) );
					_snprintf(cheques_clientes->cod_banco_str, sizeof(cheques_clientes->cod_banco_str)-1, ( &datos[2] )->valor );
					_snprintf(cheques_clientes->nro_cheque_str, sizeof(cheques_clientes->nro_cheque_str)-1, ( &datos[1] )->valor );
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
                    //INSERT();
					INSERT2(NO);
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                    rta = 1;
                }
                else {
                    MENSAJE_STRING( S_YA_EXISTE_EL_CHEQUE );
                }
            }//!borrar
            //CLOSE_DATABASE( AREA_CHEQUE_CLIENTE );
			CLOSE_CHEQUE_CLIENTES();
        }
    }
    else // Modo Servicio
    {
        // Modo Retail
       // SELECT( AREA_CHEQUE_CLIENTE );
        if( ABRIR_CHEQUES_CLIENTES() == 0 ) {
         //   SET_ORDER( 1 );
			SET_ORDER2("COD_CLIENTE, ESTADO, FECHA_VTO, COD_BANCO, NRO_CHEQUE");
            
            indice.nro_cliente = _PAGO_BUFFER1_LONG( h );
            indice.cod_banco = (UINT16)_PAGO_BANCO( h );
            indice.nro_cheque = _PAGO_NUMERO( h );
            //GET_EQUAL( ( char* )&indice );
			SET_WHERE("NRO_CLIENTE = '%ld' and COD_BANCO = '%u' and NRO_CHEQUE = '%ld'", indice.nro_cliente, 
				indice.cod_banco, indice.nro_cheque );
			RUN_QUERY(NO);
            if( !FOUND2() ) {
                memset( cheques_clientes, 0, sizeof( struct _cliecheq ) );
                cheques_clientes->nro_cliente = RAM_NRO_CLIENTE;
                cheques_clientes->cod_banco = _PAGO_BANCO( h );
                memcpy( cheques_clientes->nro_cuenta, _PAGO_BUFFER( h ), 10 );
                cheques_clientes->nro_cheque = _PAGO_NUMERO( h );
                cheques_clientes->importe = _PAGO_IMPORTE( h );
                cheques_clientes->fecha_vencimiento = _PAGO_FECHA_VTO( h );
                cheques_clientes->dia_operativo = GET_FECHA_DOS();
                cheques_clientes->hora = GET_HORA_DOS();
                cheques_clientes->caja = NRO_CAJA;
                cheques_clientes->cajero = NRO_CAJERO;
                cheques_clientes->estado = 0;         // no acreditado
                #ifdef INVEL_L
                //BEGIN_TRANSACTION();
                #endif
                INSERT2(NO);
                #ifdef INVEL_L
                //END_TRANSACTION();
                #endif
                rta = 1;
            }
            else {
                MENSAJE_STRING( S_YA_EXISTE_EL_CHEQUE );
            }
            //CLOSE_DATABASE( AREA_CHEQUE_CLIENTE );
			CLOSE_CHEQUE_CLIENTES();
        }
    }
	SELECT_TABLE( area_ant, tipo_ant );
    return rta;
}
/***************************************************************************/
int EXISTEN_CHEQUES_CLIENTE()
/***************************************************************************/
{
    int rta = 1;
    struct _indice
    {
        long nro_cliente;
        unsigned short cod_banco;
        long nro_cheque;
    };
    struct _indice indice;
    char men[100];
    int i,h;
	int  area_ant, tipo_ant;
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    
	if( !REGISTRAR_CHEQUES_EN_ARCHIVO ) {
        return ( 1 );
    }
   // SELECT( AREA_CHEQUE_CLIENTE );
    if( ABRIR_CHEQUES_CLIENTES() == 0 ) {
        for( h = 0;h < RAM_P_PAGO;h++ ) {
            //SET_ORDER( 1 );
			SET_ORDER2("COD_CLIENTE, ESTADO, FECHA_VTO, COD_BANCO, NRO_CHEQUE");
            indice.nro_cliente = _PAGO_BUFFER1_LONG( h );
            indice.cod_banco = (unsigned short)_PAGO_BANCO( h );
            indice.nro_cheque = _PAGO_NUMERO( h );
            //GET_EQUAL( ( char* )&indice );
			SET_WHERE("NRO_CLIENTE = '%ld' and COD_BANCO = '%u' and NRO_CHEQUE = '%ld'", indice.nro_cliente, 
				indice.cod_banco, indice.nro_cheque );
			RUN_QUERY(NO);
            if( FOUND2() ) {
                sprintf( men, ST( S__YA_EXISTE_EL_CHEQUE__LD ), _PAGO_NUMERO( h ) );
                MENSAJE( men );
                rta = 0;
            }
            if( medios[( int )_PAGO_MODO( h )].cheque ) {
                for( i = h + 1;i < RAM_P_PAGO;i++ ) {
                    if( _PAGO_BUFFER1_LONG( h ) == _PAGO_BUFFER1_LONG( i )
                     && _PAGO_BANCO( h ) == _PAGO_BANCO( i )
                     && _PAGO_NUMERO( h ) == _PAGO_NUMERO( i )
                     && medios[( int )_PAGO_MODO( i )].cheque && MODO_NEGOCIO != SERVICIOS ) {
                        sprintf( men, ST( S__YA_FUE_INGRESADO_EL_CHEQUE__LD ), _PAGO_NUMERO( h ) );
                        MENSAJE( men );
                        rta = 0;
                    }
                }
            }
        }
        //CLOSE_DATABASE( AREA_CHEQUE_CLIENTE );
		CLOSE_CHEQUE_CLIENTES();
    }
    else {
        MENSAJE_STRING( S_NO_PUEDE_ABRIR_CHEQUES );
    }
	SELECT_TABLE( area_ant, tipo_ant );
    return rta;
}
/***************************************************************************/
int BORRA_CHEQUE_CLIENTE( int h )
/***************************************************************************/
{
    //int area;
    int  area_ant, tipo_ant;
	struct _indice
    {
        long nro_cliente;
        int cod_banco;
        long nro_cheque;
    };

    struct _indice indice;
    if( !REGISTRAR_CHEQUES_EN_ARCHIVO ) {
        return ( 0 );
    }
	 
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    //area = SELECTED();
    //SELECT( AREA_CHEQUE_CLIENTE );
    if( ABRIR_CHEQUES_CLIENTES() == 0 ) {
        //SET_ORDER( 1 );
		SET_ORDER2("COD_CLIENTE, ESTADO, FECHA_VTO, COD_BANCO, NRO_CHEQUE");
        indice.nro_cliente = _PAGO_BUFFER1_LONG( h );
        indice.cod_banco = _PAGO_BANCO( h );
        indice.nro_cheque = _PAGO_NUMERO( h );
        //GET_EQUAL( ( char* )&indice );
		SET_WHERE("NRO_CLIENTE = '%ld' and COD_BANCO = '%u' and NRO_CHEQUE = '%ld'", indice.nro_cliente, 
				indice.cod_banco, indice.nro_cheque );
			RUN_QUERY(NO);
        if( FOUND2() ) {
            DELETE2();
        }
        //CLOSE_DATABASE( AREA_CHEQUE_CLIENTE );
		CLOSE_CHEQUE_CLIENTES();
    }
    //SELECT( area );
	SELECT_TABLE( area_ant, tipo_ant );
    return ( 1 );
}
/***************************************************************************/
int ABRIR_CHEQUES_CLIENTES( void )
/***************************************************************************/
{
    #ifdef COMPILAR_MODO_ENTRENAMIENTO
    if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
       // return USE_DB( AREA_CHEQUE_CLIENTE, _CLI_CHEQ_SIC_ENTR, ( char* )cheques_clientes,
       //                sizeof( struct _cliecheq ), NULL, 0, 0 );
		OPEN_TABLE( T_CLI_CHEQ, TT_ENTR, ( char* )&cheques_clientes, sizeof( struct _cliecheq ) );
		return SELECT_TABLE( T_CLI_CHEQ, TT_ENTR );
    }
    else
    #endif
    { // 
       // return USE_DB( AREA_CHEQUE_CLIENTE, _CLI_CHEQ_SIC, ( char* )cheques_clientes,
        //               sizeof( struct _cliecheq ), NULL, 0, 0 );
		OPEN_TABLE( T_CLI_CHEQ, TT_ORIG, ( char* )&cheques_clientes, sizeof( struct _cliecheq ) );
		return SELECT_TABLE( T_CLI_CHEQ, TT_ORIG );
    }
}

/***************************************************************************/
void CLOSE_CHEQUE_CLIENTES( void )
/***************************************************************************/
{
	if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
		CLOSE_TABLE( T_CLI_CHEQ, TT_ENTR );
	} else {
		CLOSE_TABLE( T_CLI_CHEQ, TT_ORIG );
	}
}

/***************************************************************************/
void BUSCA_NOMBRE_BANCO( int codigo_banco )
/***************************************************************************/
{
    //int area_anterior;
    int tabla_ant, tipo_ant;
    //area_anterior = SELECTED();
    tabla_ant = SELECTED2();
    tipo_ant = SELECTED_TIPO();
    if( /*USE_DB( AREA_BANCO, _BANCOS_SIC, ( char* )&bancos, sizeof( bancos ), NULL, 0, 0 )*/
		OPEN_TABLE(T_BANCOS, TT_ORIG ,( char* )&bancos, sizeof( bancos ))== 0 ) {
        EXISTE_BANCO__( codigo_banco );
    }
    CLOSE_TABLE( T_BANCOS, TT_ORIG );
	//SELECT( area_anterior );
	SELECT_TABLE( tabla_ant, tipo_ant );
}
/***************************************************************************/
void BUSCAR_BANCO( int codigo_banco, char *aux )
/***************************************************************************/
{
	int area_ant, tipo_ant;
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

    if( USAR_MENU_BANCOS ) {
       // int area_ant = SELECTED();
       // SELECT( AREA_BANCO );      
        if( /*USE_DB( AREA_BANCO, _BANCOS_SIC, ( char* )&bancos, sizeof( bancos ), NULL, 0, 0 )*/
			OPEN_TABLE(T_BANCOS, TT_ORIG,( char* )&bancos, sizeof( bancos )) == 0 ) {
            SET_ORDER2( "COD_BANCO" );
            //GET_EQUAL( ( char* )&codigo_banco );         
			SET_WHERE("COD_BANCO = '%i'",codigo_banco);
			RUN_QUERY(NO);
            if( FOUND2() ) {
                strcpy( aux, bancos.nom_banco );
            }
            CLOSE_TABLE( T_BANCOS, TT_ORIG );
        }
	SELECT_TABLE( area_ant, tipo_ant );
//        SELECT( area_ant );
    }
}
/*****************************************************************************/
int CANCELA_CHEQUES( void )
/*****************************************************************************/
{
    int ok = 1;
    if( !REGISTRAR_CHEQUES_EN_ARCHIVO ) {
        return( ok );
    }
    if( MODO_NEGOCIO == SERVICIOS ) {
        int y = 0, i = 0, a = 0, pos = 0, h = 0, pp = 0;
        struct _datos datos[5];        
        if( RAM_P_PAGO > 0 ) {
            while( pp < RAM_P_PAGO ) {
                //if( medios[_PAGO_MODO( RAM_P_PAGO - 1 )].cheque ) {                
                if( medios[_PAGO_MODO( pp )].cheque ) {
                    // recorro todos los dd_tmp_pr.xxx para encontrar los cheques
                    // y los elimino de cliecheq.btr
                    pos = pp;
                    h = _PAGO_MODO( pp );
                    for( a = 0;a <= h;a++ ) {
                        // con esto obtengo el xxx de dd_tmp_pr.xxx
                        i = ( h * 100 ) + a;
                        for( y = 0;y <= pos;y++ ) {
                            // obtengo los datos de los cheques
                            if( _PAGOS_OBTENER_DATOS( i, y, &datos[0], 3 ) ) {
                                /* datos[1] es el numero de cheque, 
                                   datos[2] es el codigo de banco,
                                   uso esos valores para eliminar el cheque 
                                   de la tabla cliecheq */
                                ELIMINA_CHEQUE( atoi( ( &datos[1] )->valor ),
                                                atoi( ( &datos[2] )->valor ),
                                                ( &datos[1] )->valor );
                            }
                        }
                    }
                }
                pp++;
            }
        }
    }
    return( ok );
}

