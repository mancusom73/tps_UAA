#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <actual.h> 
#include <archivos.h> 
#include <bancos.h>
#include <btrv.h> 
#include <b_mens.h> 
#include <cajon.h> 
#include <cio.h> 
#include <clave.h> 
#include <cmemory.h> 
#include <comprob.h> 
#include <cr.h> 
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <eventos.h> 
#include <ftoa.h> 
#include <getch.h> 
#include <imp_fond.h> 
#include <imp_tick.h> 
#include <lista.h> 
#include <log.h> 
#include <med_rein.h> 
#include <mensaje.h> 
#include <m_cadena.h> 
#include <pagos_esp.h> 
#include <pagxcaja.h> 
#include <pant.h> 
#include <print.h> 
#include <recupero.h> 
#include <socios.h> 
#include <stack.h> 
#include <stringt.h> 
#include <tkt.h> 
#include <trim.h> 
#include <xyz.h> 
#include <_cr0.h> 
#include <_cr1.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <ctype.h>
#include <math.h>
#ifndef INVEL_L
#include <dos.h>
#endif
#include <aarch.h>
#include <config.h>
#include <actions.h>
#include <myodbc.h>
#include <path.h>
#include <recupero.h>
#include <retiros.h>
struct _datos_comp_rec
{
    long nro_comp_reintegro;
};
static struct _pagos1 pagos1;
extern struct _pagos2 pagos2;
static struct _ind_pagos2 ind_pagos2;
static struct _ind2_pagos2 ind2_pagos2;
static struct _ind3_pagos2 ind3_pagos2;
//static struct _medios_para_reintegros medios_para_reintegros;
static struct _ind_mreintegro_1 ind_mreintegro_1;
extern struct _cajas_suc caja_suc;
#define _COD_NIV1      GET_MEMORY_LONG(   _cod_niv1 )
#define _CODIGO_P2     GET_MEMORY_LONG(   _codigo_p2 )
#define _ID            GET_MEMORY_INT(    _id )
#define _IDLONG        GET_MEMORY_LONG(    _idlong )
#define _COD_VALIDA    GET_MEMORY_INT(    _cod_valida )
#define _COD_MEDIO     GET_MEMORY_CHAR(   _cod_medio )
#define _COD_SUB_MEDIO GET_MEMORY_CHAR(   _cod_sub_medio )
#define _IMPORTE_P2    GET_MEMORY_DOUBLE( _importe_p2 )
#define _COD_CLIENTE   GET_MEMORY_LONG(   _cod_cliente )
#define _CLAVE_ID      GET_MEMORY_STR(    _clave_id )
#define _ID_EVENT      GET_MEMORY_LONG(    _id_evento )
#define _ID_CLI_COBRO  GET_MEMORY_LONG(    _id_cli_cobro )
static int _estado,_cod_niv1,_codigo_p2,_id,_idlong,_cod_valida,_cod_medio,_cod_sub_medio;
static int _importe_p2,_cod_cliente;
static int _clave_id,_id_evento,_id_cli_cobro;
long PEDIR_DATOS_PAGOS_ESP();
void PANTALLA_PAGOS_ESPECIALES();
void GRABAR_EVENTO_PAGO_ESP( void );
void TICKET_PAGOS_ESPECIALES();
int PAGOS_ESP_AGREGAR_PAGO( int nro, double *importe );
void IMPRIMIR_PAGOS_ESPECIALES();
//void ACTUALIZA_ACUMULADORES_MEDIOS_EN_PAGOS( void );
void CEREAR_VARIABLES_PAGOS_ESP( void );
void OBTIENE_IDENTIFICACION_UNICA( void );
extern int REIMPRIMIR( int timbrado, int funSup );
void OBTENER_DATOS_COMPROBANTE_REC( short int *nro_suc, long *nro_comp, int actualizar_nro );
void CREAR_STACK()
{
    _estado = STACK_INS( sizeof( short ) );
    _cod_niv1 = STACK_INS( sizeof( long ) );
    _codigo_p2 = STACK_INS( sizeof( long ) );
    _id = STACK_INS( sizeof( short ) );
    _idlong = STACK_INS( sizeof( long ) );
    _cod_valida = STACK_INS( sizeof( short ) );
    _cod_medio = STACK_INS( sizeof( char ) );
    _cod_sub_medio = STACK_INS( sizeof( char ) );
    _importe_p2 = STACK_INS( sizeof( double ) );
    _cod_cliente = STACK_INS( sizeof( long ) );
    _id_evento = STACK_INS( sizeof( long ) );
    _clave_id = STACK_INS( 21 );
    _id_cli_cobro = STACK_INS( sizeof( long ) );
}
/*****************************************************************************/
void PAGOS_ESPECIALES()
/*****************************************************************************/
{
    int _ptr,_handle;
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    CREAR_STACK();  

    if( _ESTADO == 0 ) {
        SET_MEMORY_LONG( __ram_nro_comp, 0 );
        if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 21, SI, SI, PEDIR_CLAVE_EN_PAGOS,
                              ST( S__PAGOS_ ), NULL, RAM_COD_FUNCION, SI ) ) {
            SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
            if( OPEN_TABLE( T_PAGOS1, TT_ORIG, ( char* )&pagos1, sizeof( struct _pagos1 ) ) == 0 ) {
                if( OPEN_TABLE( T_PAGOS2, TT_ORIG, ( char* )&pagos2, sizeof( pagos2 ) ) == 0 ) {
                    PANTALLA_PAGOS_ESPECIALES();
                    _ATRIBUTO = 112;
                    /*----------- ingresa pagos ------------*/
                    if( PIDE_PAGOS_ESPECIALES() ) {
                        _SET_ESTADO( 1 );
                    }
                    else {
                        MENSAJE( "No se pudo efectuar el pago. Cerrando pagos..." );
                        BORRAR_MENSAJE();
                    }
                }
            }
        }
    }
    if( _ESTADO == 1 ) {
        ABRIR_CAJON();
        MENSAJE( "Cerrando pagos..." );
        //TICKET_PAGOS_ESPECIALES(  );
        _SET_ESTADO( 2 );
        BORRAR_MENSAJE();
    }
    if( _ESTADO == 2 ) {
        if( RAM_NRO_COMP == 0 ) {
            //SET_MEMORY_INT( __ram_perfil, 1 );  //Swiss medical se usa dentro de OBTENER_DATOS_COMPROBANTE
            /*OBTENER_DATOS_COMPROBANTE( _FACTURA, comp.subtipo, 1, &comp.nro_suc, &comp.nro_comp, NULL, SI,
                                       SI );*/
            OBTENER_DATOS_COMPROBANTE_REC( &comp.nro_suc, &comp.nro_comp, SI );
        }
        _SET_ESTADO( 3 );
    }
    if( _ESTADO == 3 ) {
        //guarda el ultimo id evento de cobros
        SET_MEMORY_LONG( _id_evento, NRO_ULTIMO_EVENTO_ANALIZADO );
        GRABAR_EVENTO_PAGO_ESP();
        SET_MEMORY_LONG( __nro_ultimo_evento_analizado,
                        RAM_ID_EVENTO - 1);
        _SET_ESTADO( 4 );
    }   
    if( _ESTADO == 4 ) {
        GRABAR_LOG_SUPERVISOR( "Pagos Reintegros", USUARIO_CLAVE_ANTERIOR() ,LOG_PAGOS,4);
        GRABAR_XYZ( 'X' );
        _SET_ESTADO( 5 );
    }   
    if( _ESTADO == 5 ) {
        int med_tmp = ( int )_COD_MEDIO - 1;
        double imp_tmp = _IMPORTE_P2;        
        /*------------ actualiza acumuladores -------------*/             
        if( medios[med_tmp].efectivo ) {
            ADD_MEMORY_DOUBLE( __x_tot_dinero, -( imp_tmp ) );
            ADD_MEMORY_DOUBLE( __x_pagos_por_caja_importe, imp_tmp ); //pagos hechos en efectivo            
            ADD_MEMORY_INT( __x_pagos_por_caja_cantidad, 1 );//cantidad de pagos EF
        }
        else {
            //pagos hechos con cheques            
            ADD_MEMORY_DOUBLE( __x_cons_final_importe_exent_tkt, -imp_tmp );//importe total de cheques en caja
            ADD_MEMORY_INT( __x_cons_final_cantidad_tkt, -1 );//cantidad total de cheques en caja
            ADD_MEMORY_DOUBLE( __x_dev_cons_final_importe_exent_tkt, imp_tmp );//importe total de cheques pagados
            ADD_MEMORY_INT( __x_dev_cons_f_cantidad_tkt, 1 );//cantidad total de cheques pagados
        }        
        CEREAR_VARIABLES_PAGOS_ESP();
        _SET_ESTADO( 6 );
    }
    if( _ESTADO == 6 ) {
        IMPRIMIR_PAGOS_ESPECIALES();
        GRABAR_SUPER_REC_A_EVENT( RAM_ID_EVENTO );
    }
    BORRAR_MENSAJE();
    /*--------------- fin ----------------*/
    ExecuteAction( A_DES_PAGOS_ESP_ACT_COBROS_ESP );
    CERRAR_CAJON();
    //CLOSE_DATABASE( AREA_PAGO2 );
    CLOSE_TABLE( T_PAGOS2, TT_ORIG );
    CLOSE_DATABASE( AREA_PAGO1 );
    SET_MEMORY_LONG( __nro_ultimo_evento_analizado, _ID_EVENT );
    /*--------------------- Fin de recupero de memoria ----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
}
/*****************************************************************************/
/*int GET_ESTADO_PAGOS_ESP()
/*****************************************************************************/
/*{
    int _ptr;
    int _handle;
    int rta;
    // Prepara recupero de memoria
    OPEN_RECUPERO( &_ptr, &_handle );
    // inicializo el stack
    CREAR_STACK();
    // devuelve el estado actual
    rta = _ESTADO;
    // Fin de recupero de memoria SIN cerear el stack
    STACK_SET_POINTER( _ptr );
    STACK_SET_HANDLE( _handle );
    return rta;
} */
/*****************************************************************************/
void PANTALLA_PAGOS_ESPECIALES()
/*****************************************************************************/
{
    //char cad_modo[15];
    ExecuteAction( A_DES_COBROS_ESP_ACT_PAGOS_ESP );
    //strncpy( cad_modo, ST( S__PAGOS_ ), sizeof(cad_modo) );
    ExecuteAction( A_MUESTRA_MODO_PAGOS_ESP );
    ExecuteAction( A_MUESTRA_TIT_PAGOS_ESP );
    ExecuteSAction( A_MUESTRA_SOCIO_PAGOS_ESP, " " );
    ExecuteSAction( A_MUESTRA_REINT_PAGOS_ESP, " " );
    ExecuteSAction( A_MUESTRA_IMPORTE_PAGOS_ESP, " " );
    ExecuteSAction( A_MUESTRA_MEDIO_PAGOS_ESP, " " );
    ExecuteSAction( A_MUESTRA_SUBMEDIO_PAGOS_ESP, " " );
    ExecuteSAction( A_MUESTRA_CHEQUE_PAGOS_ESP, " " );   
    ExecuteSAction( A_MUESTRA_BANCO_PAGOS_ESP, " " );
}
/*****************************************************************************/
int ELEGIR_PAGOS_ESP_BROWSE()
/*****************************************************************************/
{
    int rta;
    SELECT( AREA_PAGO1 );
    GO( TOP );
    _ATRIBUTO = 120;    
    rta = Consulta_Pagos_Esp( _consulta_pagos_esp );
    if( !rta ) {
        rta = -1;
    }
    //SELECT( AREA_PAGO1 );
    return ( rta );
}
/*****************************************************************************/
int _consulta_pagos_esp( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0, y, cod;
    int _X1,_X2;
    _X1 = 32;
    _X2 = 53;
    y = 5 + dato;
    switch( comando ) {
        case _BUSCAR:
            //MOSTRAR_CADENA( _X1 + 1, _Y2 + 2, cadena );
            cod = atoi( cadena );
            GET_GREATER_E( ( char* )&cod );
            break;
        case _MOSTRAR:
            {
                char xx[10];      
                _itoa( pagos1.codigo, xx, 10 );
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, pagos1.nombre );
            }
            MOSTRAR_ENTERO( _X1, y, "999", pagos1.codigo );
            MOSTRAR_CADENA( _X1 + 4, y, pagos1.nombre );
            break;
        case _SKIP:
            SKIP( dato );
            rta = BtrvEOF() || BOF();
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? BOF() : BtrvEOF();
            break;
        case _ENTER:
            rta = 1;
            break;
        case _COMPARAR:
            rta = 2;
            break;
        case _TECLA:
            /*if( dato == 32 )
                    b->seguir = 0;*/
            break;
        case _VALIDAR:
            if( dato >= '0' && dato <= '9' ) {
                rta = 1;
            }
            break;
    }
    return ( rta );
}
/*****************************************************************************/
int _consulta_pagos_esp2( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0, y;
    int _X1,_X2;
    _X1 = 32;
    _X2 = 53;
    y = 5 + dato;
    switch( comando ) {
        case _BUSCAR:
            //cod = atoi( cadena );
            //GET_GREATER_E( ( char* )&cod );
            break;
        case _MOSTRAR:
            {
                char xx[10];
                _itoa( pagos2.idlong, xx, 10 );
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, pagos2.nombre );
            }
            MOSTRAR_ENTERO( _X1, y, "999", pagos2.idlong );
            MOSTRAR_CADENA( _X1 + 4, y, pagos2.nombre );
            break;
        case _SKIP:
            /* SKIP( dato );
             rta = BtrvEOF() || BOF();
             //if( !rta && (( pagos2.cod_nivel1 != pagos1.codigo ) || ( pagos2.cod_cliente != _COD_CLIENTE ) )) {
             if( !rta
              && ( ( pagos2.cod_nivel1 != pagos1.codigo )
                || strcmp( pagos2.clave_id, ind3_pagos2.clave_id ) != 0 ) ) {
                 if( !rta ) {
                     SKIP( dato * -1 );
                 }
                 rta = 1;
             }*/
            SKIP2( dato );
            ////rta = BtrvEOF(  ) || BOF(  );
            rta = dbEOF() || dbBOF();
            break;
        case _EXTREMO:
            //rta = ( dato == -1 ) ? BOF() : BtrvEOF();
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
            rta = 1;
            break;
        case _COMPARAR:
            rta = 2;
            break;
        case _TECLA:
            /*if( dato == 32 )
                    b->seguir = 0;*/
            break;
        case _VALIDAR:
            if( dato >= '0' && dato <= '9' ) {
                rta = 1;
            }
            break;
    }
    return ( rta );
}
/*****************************************************************************/
int MOSTRAR_SUBNIVEL_ESP()
/*****************************************************************************/
{
    int rta = 1; 
    int area_ant = 0, tipo_ant = 0;
    area_ant = SELECTED2();
    tipo_ant = SELECTED_TIPO();
    /*------------ muestra los nombres ------------*/
    if( !OPENED( T_PAGOS2, TT_ORIG )
     && ( !OPEN_TABLE( T_PAGOS2, TT_ORIG, ( char* )&pagos2, sizeof( pagos2 ) ) == 0 ) ) {
        GRABAR_LOG_DBROUTER( "ERROR ABRIENDO PAGOS2" ,LOG_ERRORES,2);
        MENSAJE( "ERROR ABRIENDO TABLA PAGOS2" );
        return( 0 );
    }
    SELECT_TABLE( T_PAGOS2, TT_ORIG );     
    SET_WHERE( "COD_NIVEL1 = %ld and ID_CLI_COBRO = %ld", pagos1.codigo, _ID_CLI_COBRO );
    RUN_QUERY( NO );
    if( FOUND2() ) {
        /*SELECT( AREA_PAGO2 );     
        ////SET_ORDER( 2 );   
        ////ind2_pagos2.cod_nivel1 = pagos1.codigo;
        ////ind2_pagos2.cod_cliente = ( long ) _COD_CLIENTE;
        ////ind2_pagos2.codigo = 0;
        ////GET_GREATER_E( ( char * ) &ind2_pagos2 );
        ////if( FOUND(  ) && pagos2.cod_cliente == ind2_pagos2.cod_cliente){
        ////
        GO( TOP );
        SET_ORDER( 3 );   
        ind3_pagos2.cod_nivel1 = pagos1.codigo;
        strcpy( ind3_pagos2.clave_id, _CLAVE_ID );    
        GET_GREATER_E( ( char* )&ind3_pagos2 );
        if( FOUND() && strcmp( pagos2.clave_id, ind3_pagos2.clave_id ) == 0 ) {
        .merge-right.r2360*/
        //if( strcmp( pagos2.clave_id, ind3_pagos2.clave_id ) == 0 ) {
        rta = Consulta_Pagos_Esp2( _consulta_pagos_esp2 );
    }
    else {
        MENSAJE( "NO SE REGISTRAN REINTEGROS" );
        BORRAR_MENSAJE();
        rta = 0;
    }
    SELECT_TABLE( area_ant, tipo_ant );
    return ( rta );
}
/*****************************************************************************/
int PIDE_PAGOS_ESPECIALES( void )
/*****************************************************************************/
{
    int seguir = 1, rta = 0;
    long _codniv1 = 0, _cod = 0;
    seguir = ELEGIR_PAGOS_ESP_BROWSE();
    SET_MEMORY_LONG( __ram_nro_cliente, 0L );
    if( seguir < 0 ) {
        return( 0 );
    }   
    if( !_COD_CLIENTE ) {
        seguir = PEDIR_DATOS_PAGOS_ESP();
    }
    if( seguir < 0 ) {
        return( 0 );
    }
    if( pagos1.subnivel == 'S' || pagos1.subnivel == 1 || pagos1.subnivel == '1' ) {
        seguir = MOSTRAR_SUBNIVEL_ESP();
    }
    else {
        pagos2.codigo = pagos1.codigo;
        pagos2.id = pagos1.id;
        pagos2.idlong = ( long )pagos1.id;
        pagos2.discrimina = pagos1.discrimina;
        strcpy( pagos2.nombre, pagos1.nombre );
        seguir = 1;
    }
    if( seguir ) {
        SET_MEMORY_LONG( _cod_niv1, pagos2.cod_nivel1 );
        SET_MEMORY_LONG( _codigo_p2, pagos2.codigo );
        SET_MEMORY_INT( _id, pagos2.id );
        SET_MEMORY_LONG( _idlong, pagos2.idlong );
        SET_MEMORY_INT( _cod_valida, pagos1.cod_valida );
        SET_MEMORY_CHAR( _cod_medio, pagos2.cod_medio );
        SET_MEMORY_CHAR( _cod_sub_medio, pagos2.cod_sub_medio );
        SET_MEMORY_DOUBLE( _importe_p2, pagos2.importe );
        SET_MEMORY_LONG( _cod_cliente, pagos2.cod_cliente );
        //SET_MEMORY( _clave_id, pagos2.clave_id );
        SET_MEMORY_LONG( _id_cli_cobro, pagos2.id_cli_cobro );
        rta = 1;
        if( rta ) {
            double imp_tmp = _IMPORTE_P2;
            _codniv1 = _COD_NIV1;
            _cod = _CODIGO_P2;
            if( !CONTROL_IMPORTES_ESP( 1, NO, &_codniv1, &_cod, _COD_MEDIO - 1, ( double* )&imp_tmp,
                                       0, 0, 100, 0, NULL ) ) {
                rta = 0;
                CEREAR_VARIABLES_PAGOS_ESP();
            }
        }
        if( rta ) {
            rta = CONFIRMAR_DATOS_PAGO_ESP( pagos2.cod_nivel1, pagos2.codigo, pagos2.cod_cliente );
        }
    }
    BORRAR_MENSAJE();
    if( rta ) {
        double imp_tmp = _IMPORTE_P2;
        rta = PAGOS_ESP_AGREGAR_PAGO( _COD_MEDIO - 1, ( double* )&imp_tmp );
    }   
    return ( rta );
}
/*****************************************************************************/
int ELEGIR_SUBNIVEL_ESP()
/*****************************************************************************/
{
    int seguir = 1, k, rta = 0;
    while( seguir ) {
        k = GETCH();
        switch( k ) {
            case 27:
                /*-------- ESC ---------*/
                seguir = 0;
                break;
            default:
                if( k >= '0' && k <= '9' ) {
                    SELECT_TABLE( T_PAGOS2, TT_ORIG );
                    SET_WHERE( "COD_NIVEL1 = %ld and CODIGO = %i", pagos1.codigo, ( k - '0' ) );
                    RUN_QUERY( NO );
                    if( FOUND2() ) {
                        seguir = 0;
                        rta = 1;
                    }
                }
                break;
        }
    }
    BORRAR_MENSAJE();
    return ( rta );
}
/*****************************************************************************/
int CONFIRMAR_DATOS_PAGO_ESP( long codnivel1, long codigo, long cod_cliente )
/*****************************************************************************/
{
    int rta = 0, k = 0, salir = 0, ind = _COD_MEDIO, rta2 = 0, es_cheque = 0;
    char importeS[20] = {""}, reintegroS[10] = {""}, idS[10] = {""}, submedioS[20] = {""},
         nrochequeS[20] = {""}, bancoS[21] = {""};
    extern struct _medios_para_reintegros medios_para_reintegros; 
    SELECT_TABLE( T_PAGOS2, TT_ORIG );     
    SET_WHERE( "COD_NIVEL1 = %ld and CODIGO = %ld", codnivel1, codigo );
    RUN_QUERY( NO );
    if( !FOUND2() ) {
        MENSAJE_CON_PAUSA( "EL MEDIO NO FUE CARGADO EN EL POS", 25 );
        return( 0 );
    }
    else {
        // si el pago es un cheque
        if( pagos2.cod_medio && medios[pagos2.cod_medio - 1].cheque ) {
            //if( pagos2.nro_cheque != 0 || strlen( pagos2.nro_chq_str ) > 0 ) {
            es_cheque = 1;
        }
        if( es_cheque ) {
            ind_mreintegro_1.cod_nivel1 = codnivel1;
            ind_mreintegro_1.codigo = codigo;      
          
			if(  !OPENED( T_MEDIOS_REINT, TT_ORIG ) ) {	
				CLOSE_TABLE( T_MEDIOS_REINT, TT_ORIG );
			}  
          
			if( OPEN_TABLE( T_MEDIOS_REINT, TT_ORIG, ( char* )&medios_para_reintegros, sizeof( medios_para_reintegros ) ) == 0 ) {
				SELECT_TABLE( T_MEDIOS_REINT, TT_ORIG );
				SET_ORDER2( "COD_NIVEL1, CODIGO" );
				SET_WHERE("COD_NIVEL1 = '%ld' and CODIGO = '%ld'", ind_mreintegro_1.cod_nivel1, ind_mreintegro_1.codigo );
				RUN_QUERY(NO);

				if( !FOUND2() ) {
					MENSAJE_CON_PAUSA( "EL MEDIO NO FUE CARGADO EN EL POS", 25 );
					return( 0 );
				}
			} 
			CLOSE_TABLE( T_MEDIOS_REINT, TT_ORIG );
        }
    }
    FTOA( pagos2.importe, importeS, ENTEROS, DECIMALES, 1, 0 );    
    sprintf( idS, "%ld", pagos2.cod_cliente );    
    sprintf( reintegroS, "%ld", pagos2.idlong );
    if( strlen( pagos2.nro_chq_str ) > 0 ) {
        strncpy( nrochequeS, pagos2.nro_chq_str, strlen( pagos2.nro_chq_str ) );
    }
    else {
        sprintf( nrochequeS, "%ld", pagos2.nro_cheque );
    }
    BUSCAR_BANCO( pagos2.banco, bancoS );
    SELECT_TABLE( T_PAGOS2, TT_ORIG );
    if( pagos2.cod_medio && medios[pagos2.cod_medio - 1].tarjeta ) {
        sprintf( submedioS, "%s", NOMBRE_TARJETA( _COD_SUB_MEDIO ) );
    }
    LTRIM( importeS );
    ExecuteSAction( A_MUESTRA_SOCIO_PAGOS_ESP, idS );
    ExecuteSAction( A_MUESTRA_REINT_PAGOS_ESP, reintegroS );
    ExecuteSAction( A_MUESTRA_IMPORTE_PAGOS_ESP, importeS );
    ExecuteSAction( A_MUESTRA_MEDIO_PAGOS_ESP, medios[ind - 1].nombre );
    ExecuteSAction( A_MUESTRA_SUBMEDIO_PAGOS_ESP, submedioS );
    ExecuteSAction( A_MUESTRA_CHEQUE_PAGOS_ESP, nrochequeS );   
    ExecuteSAction( A_MUESTRA_BANCO_PAGOS_ESP, bancoS );     
    //Confirma que el medio fue cargado en la tabla medReint
    /* ind_mreintegro_1.cod_nivel1 = codnivel1;
     ind_mreintegro_1.codigo = codigo;
     
    // if( USED( AREA_MEDREINT ) || (USE_DB( AREA_MEDREINT, _MEDIOS_P_REINTEGROS, ( char * ) &medios_para_reintegros, sizeof( medios_para_reintegros ), NULL, 0, 0 ) == 0 )) {
  //      SELECT( AREA_MEDREINT );
        GO(TOP);
        GET_EQUAL( ( char * ) &ind_mreintegro_1 );
        if(!FOUND( )){
              MENSAJE_CON_PAUSA("EL MEDIO NO FUE CARGADO EN EL POS",25 );
              return( 0 );
        }
       // CLOSE_DATABASE( AREA_MEDREINT );  
      }       */
    MENSAJE( "CONFIRMA EL PAGO, ENTER (ACEPTA), ESC (NO ACEPTA)" );
    while( !salir ) {
        k = GETCH();
        switch( k ) {
            case 13:
                {
                    if( !OPENED( T_CAJA_SUC, TT_ORIG )
					&& ( !OPEN_TABLE( T_CAJA_SUC, TT_ORIG, ( char* )&caja_suc,
                                   sizeof( caja_suc ) ) == 0 ) ) {
                        salir = 0;
                        rta = 0;
                    }
                    else {
                        int sucursal = SUCURSAL_DE_CAJA();
                        if( pagos2.suc_pago != sucursal ) {
                            if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI,
                                                  "DE OTRA SUCURSAL",
                                                  "Autoriz REINTEGRO EN EFECTIVO",
                                                  _F_SUP_REINT_EFEC_OTRA_SUC, SI ) ) {
                                salir = 1;
                                rta = 1;
                            }
                            else {
                                salir = 0;
                                rta = 0;
                            }
                        }
                        else {
                            salir = 1;
                            rta = 1;
                        }
                    }
                }
                break;
            case 27:
                salir = 1;
                MENSAJE( "ESTA SEGURO QUE DESEA CANCELAR, ENTER (ACEPTA), ESC (NO ACEPTA)" );
                k = GETCH();
                if( k == 13 ) {
                    rta = 0;
                }
                else {
                    BORRAR_MENSAJE();
                    MENSAJE( "CONFIRMA EL PAGO, ENTER (ACEPTA), ESC (NO ACEPTA)" );
                    rta = 0;
                    salir = 0;
                }
                break;
            case 195:
                /*----- v ------*//*--- Mostrar version de caja ---*/
                VERSION_DIALOG();
                break;
        }
    }
    BORRAR_MENSAJE();
    return ( rta );
}
#ifdef COMPILAR_ASC
/***********************************************************************/
void CREAR_PAGOS_ASC()
/***********************************************************************/
{
    int handle;
    handle = CREAT( _PAGOS_ASC, FA_ARCH );
    _CLOSE( handle, __LINE__, __FILE__ );
}
/***********************************************************************/
void COPIAR_PAGOS_ASC()
/***********************************************************************/
{
    COPY( _PAGOS_ASC, _PAGOS_ASC_BAK );
    COPY( _PAGOS_ASC, _PAGOS_ASC_REMOTO );
    LOG_EXISTE_ARCHIVO_COD( _PAGOS_ASC_REMOTO );
}
#endif
/**************************************************************************/
void TICKET_PAGOS_ESPECIALES()
/**************************************************************************/
{
    if( IMPRESORA_FISCAL ) {
        PRN_SELECT( JOURNAL );
        IMPRIME_CABECERA_GENERAL( NRO_EVENTO + 1 );
    }
    else {
        PRN_CONTINUA();
        IMPRIME_CABECERA_TICKET( SI );
        if( PEDIR_CLAVE_EN_PAGOS ) {
            IMPRIMIR_NOMBRE_USUARIO( USUARIO_CLAVE_ANTERIOR() );
        }
        IMPRIMIR_NOMBRE_USUARIO( NRO_CAJERO );
        LF( 1 );
    }
    PRN_LETRA_GRANDE();
    MOSTRAR_CADENA( 15, PRN, ST( S_PAGOS ) );
    PRN_LETRA_NORMAL();
    LF( 2 );
    MOSTRAR_CADENA( 1, PRN, ST( S_CONCEPTO_____ ) );
    MOSTRAR_CADENA( 15, PRN, pagos2.nombre );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_COMPROBANTE__ ) );
    //  MOSTRAR_ENTERO( 19, PRN, "9999", _COMPROBANTE );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_CUENTA_______ ) );
    //  MOSTRAR_ENTERO( 15, PRN, "99999999", _CUENTA );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_IMPORTE______ ) );
    MOSTRAR_FLOTANTE( 16, PRN, ENTEROS, DECIMALES, 0, 0, _IMPORTE_P2 );
    LF( 1 );
    //if( !IMPRESORA_FISCAL ) {
        PRN_FIN_TICKET();
    //}
}
/*****************************************************************************/
void GRABAR_EVENTO_PAGO_ESP( void )
/*****************************************************************************/
{
    /*SET_MEMORY_INT( __ram_perfil, 1 );  //Swiss medical se usa dentro de OBTENER_DATOS_COMPROBANTE
    OBTENER_DATOS_COMPROBANTE( _FACTURA, comp.subtipo, 1, &comp.nro_suc, &comp.nro_comp, NULL, NO,
                               SI );*/
    ACTUALIZA_MREINTEG( _PAGO_NRO_CUPON_INT1( 0 ), _PAGO_NRO_CUPON1( 0 ),
                        ( short )( _COD_MEDIO - 1 ) );
    memset( &event_ticket, 0, sizeof( event_ticket ) );
    event_ticket.nro_cliente = _ID_CLI_COBRO;//_COD_CLIENTE;
    event_ticket.suc_comprobante = RAM_NRO_SUC;
    event_ticket.nro_comprobante_new = RAM_NRO_COMP;
    event_ticket.importe_sin_iva = _IMPORTE_P2;
    event_ticket.bultos =0;//se comenta porque ya no se conoce para que se uso esta codificacion ( short )( _COD_NIV1 * 256 + _CODIGO_P2 );
    event_ticket.tipo_evento = _EVENT_PAGO_ESPECIAL;
    event_ticket.cod_cobro = 0; //no es necesario porq se toma al nro_reintegro como unico identificador   
    GRABAR_EVENT_TICKET( NO ); 
    GRABA_EVENTOS_PAGO( 0, NO );
    ACTUALIZA_ID_EVENTO_NEW();
	ACTUALIZAR_X();
}
/*****************************************************************************/
int PAGOS_ESP_AGREGAR_PAGO( int nro, double *importe )
/*****************************************************************************/
{
    int pos = 0;
    double total_en_caja = 0.0;
    if( medios[nro].efectivo ) {
        //si es efectivo 
        total_en_caja = _X_COBROS_IMPORTE( nro ) - X_PAGOS_POR_CAJA_IMPORTE
                      - _X_RETIROS_IMPORTE( nro );                    
        if( *importe > total_en_caja ) {
            double saldo = 0.0;
            if( total_en_caja <= 0 ) {
                saldo = *importe;
            }
            else {
                saldo = *importe - total_en_caja;
            }
            if( saldo <= ( _X_FONDO_FIJO_MULTIPLE_IMPORTE( nro ) - X_RETIRO_FONDO_IMPORTE ) ) {
                MENSAJE( "SE UTILIZARa FONDO FIJO" );
                BORRAR_MENSAJE();
                if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 21, NO, SI, SI,
                                      "  AUTORIZAR USO DE F. FIJO", "  AUTORIZAR USO DE F. FIJO",
                                      _F_SUP_USO_F_FIJO_REINTEGRO, SI ) ) {
                    //decrementar la variable
                    _ADD_MEMORY_DOUBLE( __x_retiro_fondo_importe_pagos, nro, saldo );
                }
                else {
                    *importe = 0;
                    return 0;
                }
            }
            else {
                //bug en caja
                *importe = 0;
                return 0;
            }
        }
    }
    CEREAR_STRUCT( _pago );
    _CEREAR_MEMORY( __pago_auto_tarjeta, pos );
    _CEREAR_MEMORY( __pago_buffer, pos );
    _CEREAR_MEMORY( __pago_tabla, pos );
    _CEREAR_MEMORY( __pago_nro_tarjeta, pos );
    _SET_MEMORY_LONG( __pago_numero, pos, ( long )pagos2.nro_cheque );
    _SET_MEMORY( __pago_nro_tarjeta, pos, pagos2.nro_chq_str );
    _SET_MEMORY_LONG( __pago_nro_cupon1, pos, _CODIGO_P2 );
    _SET_MEMORY_LONG( __pago_nro_cupon_int1, pos, _COD_NIV1 );
    _SET_MEMORY_DOUBLE( __pago_valor_cupones, pos, pagos2.idlong );
    _SET_MEMORY_INT( __pago_cuenta, pos, _ID );   
    _SET_MEMORY_INT( __pago_cod_solicitud, pos, pagos2.cod_solicitud );
    _SET_MEMORY_CHAR( __pago_modo, pos, _COD_MEDIO - 1 );
    _SET_MEMORY_CHAR( __pago_id_registro, pos, medios[_COD_MEDIO - 1].id_registro );
    _SET_MEMORY_INT( __pago_cod_submedio, pos, _COD_SUB_MEDIO );
    _SET_MEMORY_INT( __pago_banco, pos, pagos2.banco );
    _SET_MEMORY_LONG( __pago_buffer1_long, pos, 0 );
    _SET_MEMORY_DOUBLE( __pago_importe, pos, _IMPORTE_P2 );
    /*------------------------- Muestra datos --------------------------*/
    //MOSTRAR_PAGO( pos );
    //ADD_MEMORY_CHAR( __ram_p_pago, 1 );
    SET_MEMORY_CHAR( __ram_p_pago, 1 );
    return 1;
}
/*****************************************************************************/
void IMPRIMIR_PAGOS_ESPECIALES()
/*****************************************************************************/
{
    char msg[100];
    int ok = 0;
    int k = 0, seguir = 1, copias = 2, i;
    MENSAJE( "Imprimir? SI (S) o NO (N)" );
    while( seguir ) {
        k = GETCH();
        if( k == 127 || k == 130 || k == -126 || k == 's' || k == 'S' ) {
            // s o S
            seguir = 0;
            ok = 1;
        }
        else {
            if( k == 135 || k == 191 || k == 27 ) {
                // n o N o esc
                seguir = 0;
            }
        }
    }
    BORRAR_MENSAJE();
    if( ok ) {
        for( i = 0;i < copias;i++ ) {
            IMPRIMIR_TKT_PAGO_REINTEGRO();
        }
    }
    for( i = 0;i < copias;i++ ) {
        ok = 0;
        sprintf( msg, "INGRESE COMPROB. A TIMBRAR, PRESIONE UNA TECLA AL FINALIZAR",
                 comp.nro_comp );
        while( !ok ) {
            TIMBRADO_TKT_REINTEGRO();
            MENSAJE_TECLA( msg );
            if( !REIMPRIMIR( SI, _F_SUP_RETIMBRADO_REINTEGRO ) ) {
                ok = 1;
            }
        }
    }
}
/*****************************************************************************/
long PEDIR_DATOS_PAGOS_ESP( void )
/*****************************************************************************/
{
    long nroCli = -1;   
    int seguir = 1, k, rta;
    int aux;
    MENSAJE( "SELECCIONE EL CLIENTE: ALT-L Alfabetico, ALT-K Nro. de Cliente" );
    aux = TECLADO_ALFABETICO;
    TECLADO_ALFABETICO = 0;
    while( seguir ) {
        k = GETCH();
        switch( k ) {
            case 179:
            case 108:
                /*---- ALT L ----*//*---- Consulta Socio Alfabetico ---*/
                rta = EXISTE_SOCIO( _BUSQ_ALFABETI_REINTEGROS, 0, COD_UNIDAD_NEGOCIO, 0, 0, NULL );
                if( !rta ) {
                    MENSAJE( "SELECCIONE EL CLIENTE: ALT-L Alfabetico, ALT-K Nro. de Cliente" );
                }
                seguir = !rta;
                break;
            case 171:
            case 109:
                /*---- ALT K ----*//*---- Consulta Socio Numerico ---*/
                rta = EXISTE_SOCIO( _BUSQ_CODSOCIO_UNICO_REINTEGROS, 0, COD_UNIDAD_NEGOCIO, 0, 0,
                                    NULL );
                if( !rta ) {
                    MENSAJE( "SELECCIONE EL CLIENTE: ALT-L Alfabetico, ALT-K Nro. de Cliente" );
                }
                seguir = !rta;
                break;
            case 195:
                /*----- v ------*//*--- Mostrar version de caja ---*/
                VERSION_DIALOG();
                break;
            default:
                seguir = 0;
                rta = 0;
                break;
        }
    }
    TECLADO_ALFABETICO = aux;
    if( !rta ) {
        MENSAJE( "CLIENTE NO ENCONTRADO" );
        BORRAR_MENSAJE();
        return( 0 );
    }
    nroCli = cli_cobro.nro_socio;
    SET_MEMORY_LONG( _cod_cliente, nroCli );
    SET_MEMORY_LONG( _id_cli_cobro, cli_cobro.id_cli_cobro );    
    //OBTIENE_IDENTIFICACION_UNICA();
    return( nroCli );
}
/*****************************************************************************/
void CEREAR_VARIABLES_PAGOS_ESP( void )
/*****************************************************************************/
{
    SET_MEMORY_LONG( _cod_niv1, 0 );
    SET_MEMORY_LONG( _codigo_p2, 0 );
    SET_MEMORY_INT( _id, 0 );
    SET_MEMORY_LONG( _idlong, 0 );
    SET_MEMORY_INT( _cod_valida, 0 );
    SET_MEMORY_CHAR( _cod_medio, 0 );
    SET_MEMORY_CHAR( _cod_sub_medio, 0 );
    SET_MEMORY_DOUBLE( _importe_p2, 0.0 );
    SET_MEMORY_LONG( _cod_cliente, 0 );
    CEREAR_MEMORY( _clave_id );
    SET_MEMORY_CHAR( __ram_p_pago, 0 );
    SET_MEMORY_LONG( _id_cli_cobro, 0 );
}
/*****************************************************************************/
//void OBTIENE_IDENTIFICACION_UNICA( void )
/*****************************************************************************/
/*{
    char aux[5],buff[21];
    memset( buff, 0, sizeof( buff ) );
    memset( aux, 0, sizeof( aux ) );
    _itoa( cli_cobro.unidad_negocio, aux, 10 );
    TRIM( aux );
    strcpy( buff, aux );
    strcat( buff, "-" );
    memset( aux, 0, sizeof( aux ) );
    _itoa( cli_cobro.cod_cobro, aux, 10 );
    TRIM( aux );
    strcat( buff, aux );
    strcat( buff, "-" );
    memset( aux, 0, sizeof( aux ) );
    _itoa( cli_cobro.nro_socio, aux, 10 );
    TRIM( aux );
    strcat( buff, aux );
    strcat( buff, "-" );
    memset( aux, 0, sizeof( aux ) );
    _itoa( cli_cobro.empresa, aux, 10 );
    TRIM( aux );
    strcat( buff, aux );
    strcat( buff, "-" );
    memset( aux, 0, sizeof( aux ) );
    _itoa( cli_cobro.integrante, aux, 10 );
    TRIM( aux );
    strcat( buff, aux );
    buff[20] = 0;
    SET_MEMORY( _clave_id, buff );
}*/
/*****************************************************************************/
void OBTENER_DATOS_COMPROBANTE_REC( short int *nro_suc, long *nro_comp, int actualizar_nro )
/*****************************************************************************/
{
    struct _datos_comp_rec datos_comp_rec;
    //obtiene el numero de sucursal
    OBTENER_DATOS_COMPROBANTE( _FACTURA, comp.subtipo, 1, &comp.nro_suc, NULL, NULL, NO, NO );
    *nro_suc = comp.nro_suc;
    if( CANTIDAD_REGISTROS_REC( ARCH_REC_DATOS_COMP, sizeof( struct _datos_comp_rec ) ) == 0 ) {
        datos_comp_rec.nro_comp_reintegro = 0;
        GUARDAR_ARCHIVO_REC( ARCH_REC_DATOS_COMP, ( char* )&datos_comp_rec,
                             sizeof( struct _datos_comp_rec ) );
    }
    else {
        RECUPERAR_ARCHIVO_REC( ARCH_REC_DATOS_COMP, ( char* )&datos_comp_rec,
                               sizeof( struct _datos_comp_rec ), 0 );
    }
    if( actualizar_nro ) {
        datos_comp_rec.nro_comp_reintegro++;
        if( datos_comp_rec.nro_comp_reintegro == 99999999L ) {
            //reinicia el contador
            datos_comp_rec.nro_comp_reintegro = 1;
        }
        ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_COMP, ( char* )&datos_comp_rec,
                                sizeof( struct _datos_comp_rec ), 0 );
    }
    *nro_comp = datos_comp_rec.nro_comp_reintegro;
    SET_MEMORY_INT( __ram_nro_suc, *nro_suc );
    SET_MEMORY_LONG( __ram_nro_comp, *nro_comp );
}

