#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <asc.h> 
#include <bancos.h>
#include <btrv.h> 
#include <b_mens.h> 
#include <clave.h> 
#include <clientes.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <fecha.h> 
#include <get_fech.h> 
#include <log.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <m_fecha.h> 
#include <pago_che.h> 
#include <pant.h> 
#include <pg_fecha.h> 
#include <print.h> 
#include <stack.h> 
#include <stringt.h> 
#include <tkt.h> 
#include <trim.h> 
#include <_cr1.h> 
#include <math.h>
#include <aarch.h>
#include <actions.h>
#include <coop.h>
#include <db.h>
#include <deftables.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
struct _indice_cheques
{
	INT16 cod_banco;
    INT16 cod_sucursal;
    char  cuenta[2][10];
	/*int cod_banco;
    int cod_sucursal;
    char cuenta[10];*/
};
struct _boletin_cheque
{
    int banco;
    int plaza;
    char cuenta[12];
    long cheque;
    char cod_rechazo;
    char reserva[14];
};
struct _indice_uno_boletin
{
    int banco;
    int plaza;
    char cuenta[12];
    long cheque;
};
struct _indice_dos_boletin
{
    char cuenta[12];
    long cheque;
};
/*------------variables globales--------*/
int recargo;
extern int CLIENTE_HABILITADO_PARA_CHEQUES( void  );
/*****************************************************************************/
long PIDE_NRO_CHEQUE(void)
/*****************************************************************************/
{
    long numero=0;
    int y=0;
    ExecuteSSAction( A_PEDIR, 1, ST( S__N__DE_CHEQUE_ ), ST( S__N__DE_CHEQUE_ ), NULL, " " );
    numero = ( long )PEDIR_GRAN_ENTERO( 29 - ancho_panel / 2, y + 1, 8 );    
    ExecuteAction( A_FIN_PEDIR );
    return ( numero );
}
/*****************************************************************************/
unsigned PIDE_FECHA_VTO(void)
/*****************************************************************************/
{
    unsigned fecha;
    //    int       pant;
    #if defined(INVEL_W) || defined(INVEL_L)
    ExecuteSSAction( A_PEDIR, 1, "Ingrese Fecha de Vencimiento", ST( S__FECHA_DE_VENCIMIENTO_ ),
                     "  -  -  ", " " );
    #endif
    fecha = PEDIR_GRAN_FECHA( 29 - ancho_panel / 2, 23, NULL );
    #if defined(INVEL_W) || defined(INVEL_L)
    ExecuteAction( A_FIN_PEDIR );
    #endif
    return ( fecha );
}
/********************************************************************************/
int PIDE_COD_POST(void)
/********************************************************************************/
{
    int numero = 0;
    if( PEDIR_CODIGO_POSTAL ) {
        ExecuteSSAction( A_PEDIR, 1, ST( S__COD__POST__CHEQUE_ ), ST( S__COD__POST__CHEQUE_ ),
                         NULL, " " );
        numero = ( int )PEDIR_GRAN_ENTERO( 35 - ancho_panel / 2, 22, 4 );
        ExecuteAction( A_FIN_PEDIR );
    }
    return ( numero );
}
/********************************************************************************/
int PIDE_COD_HORA(void)
/********************************************************************************/
{
    int numero = 0;
    if( PEDIR_CODIGO_HORA ) {
        ExecuteSSAction( A_PEDIR, 1, ST( S__COD__DE_HORA_ ), ST( S__COD__DE_HORA_ ), NULL, " " );
        numero = ( int )PEDIR_GRAN_ENTERO( 36 - ancho_panel / 2, 22, 2 );
        ExecuteAction( A_FIN_PEDIR );
        
	}
    return ( numero );
}
/********************************************************************************/
int PIDE_CUIT( char *cuit, int otro_cuit, char *titulo, unsigned pos )
/********************************************************************************/
{
    int rta = 1;
    if( RAM_NRO_CLIENTE && !otro_cuit ) {
        strcpy( cuit, clientes.cuit );
    }
    else if( PEDIR_CUIT || otro_cuit ) {
        ExecuteSSAction( A_PEDIR, 1, ST( S_CUIT ), ST( S_CUIT ), NULL, " " );
        rta = PEDIR_GRAN_CADENA( ( char )( 23 - ancho_panel / 2 ), ( char )22, ( char )11, cuit,
                                 NULL );
        ExecuteAction( A_FIN_PEDIR );
    }
    if( rta && CONTROL_DIGITO_VERIFICADOR_RUT && strlen( cuit ) > 0 ) {
        //if( !CHEQUEAR_DIGITO_VERIFICADOR_EN_RUT( cuit ) ) {
        if( !VALIDAR_DIGITO_VERIFICADOR_EN_RUT( cuit ) ) {
            MENSAJE_STRING( S_ERROR_EN_DIGITO_VERIFICADOR_DEL_RUT );
            rta = 0;
        }
    }
    return ( rta );
}
/********************************************************************************/
int PIDE_TELEFONO( char *telefono, int otro_tel, char *titulo, unsigned pos )
/********************************************************************************/
{
    int rta = 1;
    if( RAM_NRO_CLIENTE && !otro_tel ) {
        memcpy( telefono, clientes.telefono, 11 );
        telefono[11] = 0;
    }
    else if( PEDIR_TELEFONO || otro_tel ) {
        ExecuteSSAction( A_PEDIR, 1, ST( S__TELEFONO_ ), ST( S__TELEFONO_ ), NULL, " " );
        rta = PEDIR_GRAN_CADENA( ( char )( 23 - ancho_panel / 2 ), ( char )22, ( char )11, telefono,
                                 NULL );
        ExecuteAction( A_FIN_PEDIR );
    }
    return ( rta );
}
/*****************************************************************************/
void REPROCESAR_CHEQUES( char tipo_pago )
/*****************************************************************************/
{
    int h,_cant;
    int _ptr,_handle,_estado,_h;
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    /*  _estado = STACK_INS( sizeof( int ) );
        _h = STACK_INS( sizeof( int ) );
       _cant = STACK_INS( sizeof( int ) );*/
    _estado = STACK_INS( sizeof( short ) );
    _h = STACK_INS( sizeof( short ) );
    _cant = STACK_INS( sizeof( short ) );
    for( ;GET_MEMORY_INT( _h ) < RAM_P_PAGO;ADD_MEMORY_INT( _h, 1 ) ) {
        h = GET_MEMORY_INT( _h );
        if( medios[_PAGO_MODO( h )].cheque ) {
            if( _ESTADO == 0 ) {
                ADD_MEMORY_INT( _cant, 1 );
                _SET_ESTADO( 1 );
            }
            if( _ESTADO == 1 ) {
                #ifdef COMPILAR_ASC
                ANULA_CHEQUES_ASC( -GET_MEMORY_INT( _cant ) );
                #endif
                _SET_ESTADO( 2 );
            }
            if( _ESTADO == 2 ) {
                SET_MEMORY_LONG( __ram_nro_cliente, _PAGO_BUFFER1_LONG( h ) );
                GRABA_SALDO_CHEQUE_CLIENTE( -( _PAGO_IMPORTE( h ) ) );
                _SET_ESTADO( 3 );
            }
            if( _ESTADO == 3 ) {
                BORRA_CHEQUE_CLIENTE( h );
                _SET_ESTADO( 4 );
            }
            if( _ESTADO == 4 ) {
                if( tipo_pago == 1 ) {
                    _ADD_MEMORY_DOUBLE( __x_cobros_importe, _PAGO_MODO( h ),
                                        -( _PAGO_IMPORTE( h ) ) );
                }
                else {
                    _ADD_MEMORY_DOUBLE( __x_ventas_importe, _PAGO_MODO( h ),
                                        -( _PAGO_IMPORTE( h ) ) );
                }
                _SET_ESTADO( 5 );
            }
            if( _ESTADO == 5 ) {
                if( tipo_pago == 1 ) {
                    _ADD_MEMORY_INT( __x_cobros_cantidad, _PAGO_MODO( h ), -1 );
                }
                else {
                    _ADD_MEMORY_INT( __x_ventas_cantidad, _PAGO_MODO( h ), -1 );
                }
                _SET_ESTADO( 6 );
            }
            _SET_ESTADO( 0 );
        }
    }
	CLOSE_RECUPERO( _ptr, _handle );
}
/*****************************************************************************/
void IMPRIME_DATOS_CHEQUE( long nro, char *nom_banco, unsigned fecha, char *picture )
/*****************************************************************************/
{
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, "Ch" );
    MOSTRAR_ENTERO( 4, PRN, picture, nro );
    MOSTRAR_FECHA_2000( 13, PRN, fecha );
    MOSTRAR_CADENA( 22, PRN, nom_banco );
}
/****************************************************************************************/
int PIDE_DATOS_CHEQUE( int al_dia, long *numero, int *banco, unsigned * fecha_vto,
                       int *cod_post_cheq, int *cod_hora_acre, char *cuit, char *telefono,
                       char *cuenta_banco, int *cod_valida_cheque, int *plaza, int pedir_fecha_vto )
/****************************************************************************************/
{
    int ok = 1;
    char mensaje[60];
    /*--------------------------- Pide cliente ------------------------------*/
	if( PEDIR_CLIENTE_EN_CHEQUES ) {
        if( !RAM_NRO_CLIENTE ) {
            if( !ELEGIR_CLIENTE_NUMERICO( 1, 0 ) ) {
                ok = 0;
            }
        }
		if( RAM_NRO_CLIENTE ){
            if( CLIENTE_HABILITADO_PARA_CHEQUES() == NO ){
			//if( clientes.cheques == 'N' ) {
				MENSAJE_STRING( S_CLIENTE_NO_HABILITADO_PARA_TRABAJAR_CHEQUES );
                ok = 0;
            }
        }else
			 ok = 0;
    }
    /*----------------------- Pide numero de cheque -------------------------*/
    if( ok && PEDIR_NUMERO_DE_CHEQUE ) {
        *numero = PIDE_NRO_CHEQUE();
        if( *numero <= 0 ) {
            ok = 0;
        }
    }
    /*----------------------------- Pide banco ------------------------------*/
    if( ok && config.pais != ELSALVADOR ) // rox -- poner una variable de configuracion mas representativa.
	{
        if( USAR_MENU_BANCOS ) {
            *banco = PIDE_BANCO( cuenta_banco );
        }
        else {
            *banco = PIDE_NRO_BANCO();
        }
        if( *banco <= 0 ) {
            ok = 0;
        }
        else {
            *cod_valida_cheque = bancos.cod_valida_cheque;
        }
    }
    //#if !defined(INVEL_W) && !defined(INVEL_L)
    /*----------------------------- Pide cuenta banco ----------------------*/
    if( ok && *banco ) {
        if( strlen( cuenta_banco ) <= 0 ) {
            if( !PIDE_CUENTA_CHEQUE( cuenta_banco ) ) {
                ok = 0;
            }
        }
    }
    /*----------------------------- Pide plaza -----------------------------*/
    if( ok && *cuenta_banco ) {
        ok = PIDE_PLAZA( plaza );
        if( *plaza <= 0 ) {
            ok = 0;
        }
    }

    //#endif
    /*--------------------- Pide fecha de vencimiento -----------------------*/
    if( ok && pedir_fecha_vto ) {
        if( al_dia ) {
            *fecha_vto = _GET_FECHA_DOS();
        }
        else {
            *fecha_vto = PIDE_FECHA_VTO();
        }
        if( *fecha_vto == 0xffff || !*fecha_vto ) {
            ok = 0;
        }
        if( ok ) {
			int dif_dias = DIF_DIAS( *fecha_vto, GET_FECHA_DOS() );
            if( dif_dias > DIAS_PERMITIDOS_PARA_CHEQUE_A_FE ) {
                sprintf( mensaje, ST( S_CHEQUE_CON_FECHA_A_MAS_DE__02D_DIAS ),
                         DIAS_PERMITIDOS_PARA_CHEQUE_A_FE );
                MENSAJE( mensaje );
                ok = 0;
                if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ST( S_FECHA_EXCEDIDA ),
                                      ST( S_CHEQUE_CON_FECHA_A_MAS_DE_30_DIAS ),
                                      _F_SUP_CHEQUE_30_DIAS, SI ) ) {
                    ok = 1;
                }
            }
            if( dif_dias < -29 ) {
                MENSAJE_STRING( S_CHEQUE_VENCIDO_NO_ACEPTADO );
                BORRAR_MENSAJE();
                ok = 0;
            }
        }
    }
    /*----------------------- Pide codigo postal --------------------------*/
    if( ok ) {
        *cod_post_cheq = PIDE_COD_POST();
        if( *cod_post_cheq < 0 ) {
            ok = 0;
        }
    }
    /*----------------------- Pide codigo de hora -------------------------*/
    if( ok ) {
        *cod_hora_acre = PIDE_COD_HORA();
        if( *cod_hora_acre < 0 ) {
            ok = 0;
        }
    }
    /*---------------------------- Pide cuit ------------------------------*/
    if( ok ) {
        if( !PIDE_CUIT( cuit, NO, ST( S__RUT_ ), 38 ) ) {
            ok = 0;
        }
    }
    /*-------------------------- Pide telefono -----------------------------*/
    if( ok ) {
        if( !PIDE_TELEFONO( telefono, NO, ST( S__FONO_ ), 38 ) ) {
            ok = 0;
        }
    }
    return ( ok );
}
/*****************************************************************************/
int PIDE_OTROS_DATOS_CHEQUE( int *suc, char *cta, char *serie )
/*****************************************************************************/
{
    int rta = 1;
    /*------------------ Pide cuenta cheque -------------------------*/
        ExecuteSSAction( A_MUESTRA_TITULO_BOX, 1, ST( S__CUENTA_ ), ST( S__CUENTA_ ), NULL, " " );
        PEDIR_GRAN_CADENA( ( char )( 23 - ancho_panel / 2 ), ( char )22, ( char )11, cta, NULL );
        if( cta < 0 ) {
            rta = 0;
        }
    return ( rta );
}
/* Para buscar al cliente cuando paga con cheque */
/********************************************************************************/
int BUSCAR_CLIENTE_EN_BOLETIN( char *banco, char *cuenta, char *cuit, char *telefono,
                               int *habilitado, long nro_cheque, int lector, char *serie,
                               int plaza )
/********************************************************************************/
{
    int rta = 1, en_boletin;
    en_boletin = ( VERIFICAR_CHEQUE_MANUAL_EN_BOLET ? CHEQUE_EN_BOLETIN( cuenta, nro_cheque )
                 : _NO_USAR_BOLETIN );
    *habilitado = 1;
    if( en_boletin == _EN_BOLETIN ) {
        MENSAJE_TECLA_STRING( S_CHEQUE_NO_AUTORIZADO_LLAME_SUPERVISOR_ENTER );
        //GRABAR_LOG_SISTEMA( ST( S_CHEQUE_NO_AUTORIZADO_LLAME_SUPERVISOR_ENTER ) ,LOG_VENTAS,2);
        *habilitado = 0;
    }
    else if( en_boletin == _ERROR_BOLETIN ) {
        if( !SOLICITAR_CLAVES( 30, 20, NO, SI, SI, ST( S_BOLETIN ), ST( S_AUTORIZO_SIN_BOLETIN ),
                               RAM_COD_FUNCION, SI ) ) {
            *habilitado = 0;
        }
    }
    else if( en_boletin == _NO_USAR_BOLETIN ) {
        GRABAR_LOG_SISTEMA( "No se usa Boletin" ,LOG_VENTAS,4);
    }                                             //_NO_EN_BOLETIN == 0
    else {
        GRABAR_LOG_SISTEMA( BUSCA_MENSAJE_STRING( S_NO_SE_ENCUENTRA_EN_BOLETIN ) ,LOG_PAGOS,3);
    }
    /*if( MODO_NEGOCIO != SERVICIOS ) { 
	//Se comento luego de revisar con JP Forray, 06/06/09, se debe verificar de que form se quiere controlar el tema de los cheques,. No existe esto en la Version DOS.
        if( habilitado ) {
            *habilitado = BUSCAR_DATOS_CHEQUE_EN_ARCHIVO_CLIENTES( banco, cuenta, plaza, cuit,
                                                                   telefono );
        }
    }*/
    return ( rta );
}
/**************************************************************************************/
int PIDE_DATOS_CHEQUE_FALTANTES( int solicitar_supervisor, int al_dia, long *numero, char *banco,
                                 unsigned * fecha_vto, int *cod_post_cheq, int *cod_hora_acre,
                                 char *cuit, char *telefono, int pedir_fecha_vto )
/**************************************************************************************/
{
    int ok = 1;
    char mensaje[60];
    if( solicitar_supervisor ) {
        if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ST( S__CHEQUE__ ),
                              ST( S__CHEQUE_ ), RAM_COD_FUNCION, SI ) ) {
            ok = 1;
        }
        else {
            ok = 0;
        }
    }
    /*-----------------------Valida numero de cheque -------------------------*/
    if( ok ) {
        if( *numero <= 0 ) {
            ok = 0;
        }
    }
    /*--------------------- Pide fecha de vencimiento -----------------------*/
    if( ok && pedir_fecha_vto ) {
        if( al_dia ) {
            *fecha_vto = _GET_FECHA_DOS();
        }
        else {
            *fecha_vto = PIDE_FECHA_VTO();
        }
        if( *fecha_vto == 0xffff || !*fecha_vto ) {
            ok = 0;
        }
    }
    if( ok && pedir_fecha_vto ) {
        if( DIF_DIAS( *fecha_vto, GET_FECHA_DOS() ) > DIAS_PERMITIDOS_PARA_CHEQUE_A_FE ) {
            sprintf( mensaje, ST( S_CHEQUE_CON_FECHA_A_MAS_DE__02D_DIAS ),
                     DIAS_PERMITIDOS_PARA_CHEQUE_A_FE );
            MENSAJE( mensaje );
            ok = 0;
            if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ST( S_FECHA_EXCEDIDA ),
                                  ST( S_CHEQUE_CON_FECHA_A_MAS_DE_30_DIAS ),
                                  _F_SUP_CHEQUE_30_DIAS, SI ) ) {
                ok = 1;
            }
        }
        if( DIF_DIAS( *fecha_vto, GET_FECHA_DOS() ) < -29 ) {
            MENSAJE_STRING( S_CHEQUE_VENCIDO_NO_ACEPTADO );
            ok = 0;
        }
    }
    /*----------------------- Pide codigo postal --------------------------*/
    if( ok ) {
        *cod_post_cheq = PIDE_COD_POST();
        if( *cod_post_cheq < 0 ) {
            ok = 0;
        }
    }
    /*----------------------- Pide codigo de hora -------------------------*/
    if( ok ) {
        *cod_hora_acre = PIDE_COD_HORA();
        if( *cod_hora_acre < 0 ) {
            ok = 0;
        }
    }
    /*---------------------------- Pide cuit ------------------------------*/
    if( ok && solicitar_supervisor ) {
        if( !PIDE_CUIT( cuit, NO, ST( S__RUT_ ), 38 ) ) {
            ok = 0;
        }
    }
    /*-------------------------- Pide telefono -----------------------------*/
    if( ok && solicitar_supervisor ) {
        if( !PIDE_TELEFONO( telefono, NO, ST( S__FONO_ ), 38 ) ) {
            ok = 0;
        }
    }
    return ( ok );
}
/***************************************************************/
void CEREAR_DATOS_CHEQUE( long *numero, char *banco, unsigned * fecha_vto, int *cod_post_cheq,
                          int *cod_hora_acre, char *cuit, char *telefono )
/***************************************************************/
{
    *numero = 0;
    banco = "                 ";
    *fecha_vto = 0;
    *cod_post_cheq = 0;
    *cod_hora_acre = 0;
    cuit = "            ";
    telefono = "            ";
    //SET_MEMORY_LONG( __ram_nro_cliente, 0L );
	//si borra por esta razon  queda sin cliente, y queda por ejemplos sin TFA, por mas que este abierto
    CEREAR_MEMORY( __cliente_afinidad_nombre );
}
/************************************************************************/
int CHEQUE_EN_BOLETIN( char *cuenta, long nro_cheque )
/************************************************************************/
{
    int rta = 0;
    //struct _indice_dos_boletin indice_boletin;
    //struct _boletin_cheque boletin_cheque;
    //int area_ant;
	int tabla_ant, tipo_ant;
    //area_ant = SELECTED();
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

//(Nota Unific MySQL-Linux): Se comenta por desuso (Este es la unica aparicion de la tabla bol_chq_btr en el codigo
	/*if( USE_DB( AREA_BOLCH, _BOL_CHQ_BTR, ( char* )&boletin_cheque, sizeof( boletin_cheque ), NULL,
                0, 0 ) == 0 ) {
        SET_ORDER( 2 );
        GO( TOP );
        TRIM( cuenta );
        strcpy( indice_boletin.cuenta, cuenta );
        indice_boletin.cheque = nro_cheque;
        GET_EQUAL( ( char* )&indice_boletin );
        if( FOUND() ) {
            rta = 1;
        }
        CLOSE_DATABASE( AREA_BOLCH );
    }
    else {
        rta = 2;
    }*/
    //SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );
    return rta;
}
/************************************************************************/
void VERIFICAR_CHEQUE_EN_BOLETIN()
/************************************************************************/
{
    char cuenta[15];
    long nro_cheque = PIDE_NRO_CHEQUE();
    PEDIR_GRAN_CADENA_NUMERICA( ( char )( 13 - ancho_panel / 2 ), ( char )22, ( char )11, cuenta );
    switch( CHEQUE_EN_BOLETIN( cuenta, nro_cheque ) ) {
        case 1:
            MENSAJE_TECLA_STRING( S_CHEQUE_NO_AUTORIZADO_LLAME_SUPERVISOR_ENTER );
            break;
        case 2:
            MENSAJE_TECLA_STRING( S_ERROR_EN_BOL_PROTECTIV );
            break;
        default:
            MENSAJE_STRING( S_CHEQUE_OK );
    }
    _ATRIBUTO = 52;
}
/********************************************************************************/
int PIDE_PLAZA( int *numero )
/********************************************************************************/
{
    int rta = 1;
	if( PEDIR_PLAZA ){
		ExecuteSSAction( A_MUESTRA_TITULO_BOX, 1, ST( S__PLAZA_ ), ST( S__PLAZA_ ), NULL, " " );
		*numero = ( int )PEDIR_GRAN_ENTERO( 35 - ancho_panel / 2, 22, 4 );
		ExecuteSSAction( A_MUESTRA_TITULO_BOX, 0, ST( S__PLAZA_ ), ST( S__PLAZA_ ), NULL, " " );
		if(*numero <= 0)
			rta = 0;

	}
    return ( rta);
}
/*****************************************************************************/
int PIDE_DATOS_COMPRADOR_EN_CHEQUE( char *cuit, char *telefono, int al_dia, unsigned * fecha_vto,
                                    int lector )
/*****************************************************************************/
{
    int ok = 1;
    char mensaje[60];
    /*---------------------------- Pide cuit ------------------------------*/
    if( ok ) {
        if( !PIDE_CUIT( cuit, SI, ST( S_RUT_COMPRADOR ), 33 ) ) {
            ok = 0;
        }
    }
    /*-------------------------- Pide telefono -----------------------------*/
    if( ok ) {
        if( !PIDE_TELEFONO( telefono, SI, ST( S_FONO_COMPRADOR ), 34 ) ) {
            ok = 0;
        }
    }
    /*--------------------- Pide fecha de vencimiento -----------------------*/
    if( ok && lector ) {
        if( al_dia ) {
            *fecha_vto = _GET_FECHA_DOS();
        }
        else {
            *fecha_vto = PIDE_FECHA_VTO();
        }
        if( *fecha_vto == 0xffff || !*fecha_vto ) {
            ok = 0;
        }
        if( ok ) {
            if( DIF_DIAS( *fecha_vto, GET_FECHA_DOS() ) > DIAS_PERMITIDOS_PARA_CHEQUE_A_FE ) {
                sprintf( mensaje, ST( S_CHEQUE_CON_FECHA_A_MAS_DE__02D_DIAS ),
                         DIAS_PERMITIDOS_PARA_CHEQUE_A_FE );
                MENSAJE( mensaje );
                ok = 0;
                if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ST( S_FECHA_EXCEDIDA ),
                                      ST( S_CHEQUE_CON_FECHA_A_MAS_DE_30_DIAS ),
                                      _F_SUP_CHEQUE_30_DIAS, SI ) ) {
                    ok = 1;
                }
            }
            if( DIF_DIAS( *fecha_vto, GET_FECHA_DOS() ) < -29 ) {
                MENSAJE_STRING( S_CHEQUE_VENCIDO_NO_ACEPTADO );
                ok = 0;
            }
        }
    }
    return ( ok );
}
/*****************************************************************************/
int BUSCAR_DATOS_CHEQUE_EN_ARCHIVO_CLIENTES( char *banco, char *cuenta, int plaza, char *cuit,
                                             char *telefono )
/*****************************************************************************/
{
    int habilitado = 1;
    struct _indice_cheques indice_cheques;
    indice_cheques.cod_sucursal = (INT16)plaza;
    indice_cheques.cod_banco = (INT16)atoi( banco );
    //En el cheque se leen 11 dgitos, pero el configur est preparado slo
    //para 9 dgitos. Asi que en el clientes.sic se deben cargar 9 dgitos
    //de la cuenta leda. 08/03/02.
    strncpy( indice_cheques.cuenta[0], &cuenta[1], 9 );
    indice_cheques.cuenta[0][9] = 0;
    //SELECT( AREA_CLIENTES );
	SELECT_TABLE( T_CLIENTES, TT_ORIG );
    //SET_ORDER( 5 );
    //GET_EQUAL( ( char* )&indice_cheques );
	SET_WHERE("COD_BANCO1 = '%s' AND NRO_SUC_BANCO1 = %i AND COD_CUENTA1_BANCO1 = '%s'",
						banco,plaza,cuenta);
	RUN_QUERY(SI);
    if( FOUND2() ) {
		if( CLIENTE_HABILITADO_PARA_CHEQUES() == NO ){
        //if( clientes.cheques == 'N' ) {
            MENSAJE_STRING( S_CLIENTE_NO_HABILITADO_PARA_TRABAJAR_CHEQUES );
            habilitado = 0;
        }
        else {
            habilitado = 1;
            SET_MEMORY( __cliente_afinidad_nombre, clientes.nombre );
            //SET_MEMORY_LONG( __ram_nro_cliente, clientes.cod_cliente ); comentado para salvador
            PIDE_CUIT( cuit, NO, ST( S__RUT_ ), 38 );
            PIDE_TELEFONO( telefono, NO, ST( S__FONO_ ), 38 );
        }
    }
    else {
        if( SUPERVISOR_CHEQUE_CLIENTE_INEXIS ) {
            if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 21, NO, SI, SI,
                                  ST( S_CHEQUE_EN_CLIENTES_INEXISTENTE ),
                                  ST( S_CHEQUE_DE_CLIENTE_INEXISTENTE ), RAM_COD_FUNCION, SI ) ) {
                if( !RAM_NRO_CLIENTE ) {
                    SET_MEMORY_LONG( __ram_nro_cliente, 1 );
                }
                habilitado = 1;
            }
            else {
                CEREAR_MEMORY( __cliente_afinidad_nombre );
                SET_MEMORY_LONG( __ram_nro_cliente, 0 );
                habilitado = 0;
            }
        }
        else {
            if( !RAM_NRO_CLIENTE ) {
                SET_MEMORY_LONG( __ram_nro_cliente, 1 );
                CEREAR_MEMORY( __cliente_afinidad_nombre );
            }
            habilitado = 1;
        }
    }
    return ( habilitado );
}
/*****************************************************************************/
int PIDE_CUENTA_CHEQUE( char *cta )
/*****************************************************************************/
{
    int rta = 1;
    /*------------------ Pide cuenta cheque -------------------------*/
	if( rta && PEDIR_NRO_CUENTA_CHEQUE ) {
        ExecuteSSAction( A_MUESTRA_TITULO_BOX, 1, ST( S__CUENTA_ ), ST( S__CUENTA_ ), NULL, " " );
        PEDIR_GRAN_CADENA( ( char )( 23 - ancho_panel / 2 ), ( char )22, ( char )11, cta, NULL );
        if( *cta <= 0 ) {
            rta = 0;
        }
    }
    return ( rta );
}
/*****************************************************************************/
int CHEQUEAR_DIGITO_VERIFICADOR_EN_RUT( char *cuit )
/*****************************************************************************/
{
    int rta = 0, i, j = 2, k = 2, suma = 0, mod = 0, len = 0;
    char cadena[9],verif;
    sprintf( cadena, "%-s", cuit );
    TRIM( cadena );
    len = strlen( cadena );
    TRIM( cadena );
    cadena[len - 1] = 0;
    _strrev( cadena );
    verif = cuit[len - 1];
    for( i = 1;cadena[i] && i <= 6;i++ ) {
        cadena[i] = ( cadena[i] - '0' ) * j;
        j++;
        suma += cadena[i];
    }
    for( i = 7;cadena[i] && i <= ( len - 1 );i++ ) {
        cadena[i] = ( cadena[i] - '0' ) * k;
        k++;
        suma += cadena[i];
    }
    mod = MOD11( suma );
    if( mod == 10 ) {
        if( verif == 'K' ) {
            rta = 1;
        }
    }
    else {
        if( mod == 11 ) {
            if( verif == '0' ) {
                rta = 1;
            }
        }
        else {
            if( mod < 10 ) {
                if( verif == mod ) {
                    rta = 1;
                }
            }
        }
    }
    cuit[len - 1] = 0;
    return ( rta );
}
/*****************************************************************************/
int MOD11( int suma )
/*****************************************************************************/
{
    int mod = 0;
    mod = 11 - ( int )( suma - ( floor( suma / 11 ) * 11 ) );
    return ( mod );
}
/*****************************************************************************/
int VALIDAR_DIGITO_VERIFICADOR_EN_RUT( char *rut )
/*****************************************************************************/
{
    char srut[15];
    int i, j, mrut, res = 0;
    strcpy( srut, "32765432" );
    if( strlen( rut ) < 3 ) {
        return 0;
    }
    for( i = strlen( rut ) - 2, j = 0;i >= 0;i--,j++ ) {
        res += ( ( rut[i] - '0' ) * ( srut[7 - j] - '0' ) );
    }
    mrut = 11 - ( res % 11 );
    if( mrut == 11 ) {
        mrut = 0;
    }
    if( mrut == 10 ) {
        mrut = 11;
    }
    if( atoi( &rut[strlen( rut ) - 1] ) != mrut ) {
        return 0;
    }
    return 1;
}
/*****************************************************************************/
int PIDE_CADENA_NRO_CHEQUE( char *numero )
/*****************************************************************************/
{
    int rta = 1;
    //ExecuteSSAction( A_MUESTRA_TITULO_BOX, 1, ST( S__N__DE_CHEQUE_ ), ST( S__N__DE_CHEQUE_ ), NULL );
    ExecuteSSAction( A_PEDIR, 1, ST( S__N__DE_CHEQUE_ ), ST( S__N__DE_CHEQUE_ ), NULL, " " );
    PEDIR_GRAN_CADENA( ( char )( 23 - ancho_panel / 2 ), ( char )22, ( char )11, numero, NULL );
    ExecuteAction( A_FIN_PEDIR );
    if( numero < 0 ) {
        rta = 0;
    }    
    return ( rta );
}

