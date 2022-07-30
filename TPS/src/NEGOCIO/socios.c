#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <btrv.h> 
#include <b_mens.h> 
#include <cio.h> 
#include <clave.h> 
#include <clientes.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <cr_disco.h> 
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <get_fech.h> 
#include <malloc.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <m_fecha.h> 
#include <pant.h> 
#include <socios.h> 
#include <stringt.h> 
#include <tkt.h> 
#include <trim.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <config.h>
#include <ctype.h>
#include <math.h>
#include <aarch.h>
#include <sys/stat.h>
#include <myodbc.h>
#if defined(INVEL_L)
#include <string.h>
#include <actions.h>
#include <sys/io.h>
#endif
#if defined(INVEL_W)
#include <dos.h>
#include <string.h>
#include <actions.h>
#endif
extern struct _pagos2 pagos2;
int ES_IMPRIMIBLE(int dato);
/***********************************************************************/
long PEDIR_CLIENTE( short int cod_cobro, char tipo_cli )
/***********************************************************************/
{
    static char cuit[14];
    long nroCli = 0, id_cli_cobro = 0;
    int rta = 0;
    MENSAJE_SIN_SONIDO( "INGRESE EL CODIGO DEL CLIENTE", NO );
    ExecuteAction( A_MUESTRA_DATO_CLIENTE );
    nroCli = PEDIR_ENTERO( '0', '0', 10, NO);
    rta = EXISTE_SOCIO( _BUSQ_CODSOCIO, nroCli, COD_UNIDAD_NEGOCIO, cod_cobro, tipo_cli, NULL );
    if( !rta || !nroCli ) {
        nroCli = 0;
        MENSAJE( "CLIENTE NO ENCONTRADO" );
        rta = PEDIR_CADENA_EN_RECUADRO( " CUIT CLIENTE ", NO, 13, cuit, NULL );
        if( rta ) {
            nroCli = 0;
            if( !EXISTE_SOCIO( _BUSQ_CUIT, 0, COD_UNIDAD_NEGOCIO, cod_cobro, tipo_cli, cuit ) ) {
                if( !EXISTE_SOCIO( _BUSQ_ALFABETI, 0, COD_UNIDAD_NEGOCIO, 0, 0, NULL ) ) {
                    rta = 0;
                }
                else {
                    rta = 1;
                }
            }
            else {
                rta = 1;
            }         
            if( rta ) {
                nroCli = cli_cobro.nro_socio;
                id_cli_cobro = cli_cobro.id_cli_cobro;
            }
        }
    }
    if( !rta
     || ( rta
       && !CLIENTE_AUTORIZADO( cli_cobro.id_cli_cobro, cod_cobro, ( short int ) tipo_cli ) ) ) {
        nroCli = 0;
        id_cli_cobro = 0;
        rta = 0;
    }
    BORRAR_MENSAJE();
    if( nroCli ) {
        SET_MEMORY_LONG( __ram_nro_cliente, cli_cobro.id_cli_cobro );
        SET_MEMORY_LONG( __ram_cliente_afinidad, cli_cobro.nro_socio );
        CEREAR_MEMORY( __ram_string_cliente_afinidad );
        SET_MEMORY_CHAR( __ram_pedir_cliente_afinidad, 0 );
        SET_MEMORY_INT( __ram_perfil, cod_cobro );
    }
    return( nroCli );
}
/***********************************************************************/
int EXISTE_SOCIO( int tipo_busqueda, long cliente, short int cod_un_negocio, short int cod_cobro,
                  char tipo_cli, char *cuit )
/***********************************************************************/
{
    int rta = 0, indice = 0;
    long nro_cli = RAM_NRO_CLIENTE;
    int area_ant = SELECTED();
    memset( &cli_cobro, 0, sizeof( struct _cli_cobros ) );
    if( !GET_ERROR_EN_RED() ) {
        switch( tipo_busqueda ) {
            case _BUSQ_CODSOCIO:
                if( cliente != 0 ) {
                    //SELECT( AREA_SOCIOS );
                    SELECT_TABLE( T_CLI_COBRO, TT_ORIG );
                    SET_WHERE("UNIDAD_NEGOCIO = %i and COD_COBRO = %i and NRO_SOCIO = %ld and EMPRESA = %d",
                    cod_un_negocio, cod_cobro, cliente, tipo_cli );
                    SET_ORDER2( "INTEGRANTE asc" );
                    RUN_QUERY( NO );
                    if( FOUND2() ) {
                        rta = cliente;
                    }
                    /*GO( TOP );
                    indice = GET_ORDER();
                    ind5_cli_cobro.unidad_negocio = cod_un_negocio;
                    ind5_cli_cobro.cod_cobro = cod_cobro;
                    ind5_cli_cobro.nro_socio = cliente;
                    ind5_cli_cobro.empresa = tipo_cli;
                    ind5_cli_cobro.integrante = 0;
                    SET_ORDER( 5 );
                    //GET_EQUAL( ( char * ) &ind5_cli_cobro );
                    GET_GREATER_E( ( char* )&ind5_cli_cobro );
                    //if( FOUND( ) )
                    //   rta = cliente;
                    if( ind5_cli_cobro.unidad_negocio == cli_cobro.unidad_negocio
                     && ind5_cli_cobro.cod_cobro == cli_cobro.cod_cobro
                     && ind5_cli_cobro.nro_socio == cli_cobro.nro_socio
                     && ind5_cli_cobro.empresa == cli_cobro.empresa ) {
                        rta = cliente;
                        SET_MEMORY_LONG( __ram_cliente_afinidad, cli_cobro.nro_socio );
                    }
                    SET_ORDER( indice );*/
                }
                else {
                    rta = 0;
                }
                break;
            case _BUSQ_CUIT:
                TRIM( cuit );
                if( strncmp( cuit, "", 20 ) != 0 ) {
                    /*ind6_cli_cobro.unidad_negocio = COD_UNIDAD_NEGOCIO;
                    ind6_cli_cobro.cod_cobro = cod_cobro;
                    ind6_cli_cobro.cuit[0] = 0;
                    strcat( ind6_cli_cobro.cuit, cuit );               
                    ind6_cli_cobro.empresa = tipo_cli;
                    SELECT( AREA_SOCIOS );
                    GO( TOP );
                    indice = GET_ORDER();
                    SET_ORDER( 6 );
                    GET_EQUAL( ( char* )&ind6_cli_cobro );*/
                    SELECT_TABLE( T_CLI_COBRO, TT_ORIG );
                    SET_WHERE("UNIDAD_NEGOCIO = %i and COD_COBRO = %i and CUIT = %s and EMPRESA = %d",
                    cod_un_negocio, cod_cobro, cuit, tipo_cli );
                    RUN_QUERY( NO );
                    if( FOUND2( NO ) ) {
                        SET_MEMORY_LONG( __ram_cliente_afinidad, cli_cobro.nro_socio );
                        rta = 1;
                    }
                    /*GET_GREATER_E( ( char * ) &ind6_cli_cobro );
                    if( ind6_cli_cobro.unidad_negocio == cli_cobro.unidad_negocio 
                       && ind6_cli_cobro.cod_cobro == cli_cobro.cod_cobro
                       && ind6_cli_cobro.cuit == cli_cobro.cuit
                       && ind6_cli_cobro.empresa == cli_cobro.empresa ) {
                          rta = cliente;
                    }*/
                    //SET_ORDER( indice );
                }
                else {
                    rta = 0;
                }
                break;    
            case _BUSQ_ALFABETI:
                //OPEN_TABLE(T_CLI_COBRO, TT_ORIG,(char *)&cli_cobro, sizeof(cli_cobro));
                SELECT_TABLE( T_CLI_COBRO, TT_ORIG );
                rta = ELEGIR_SOCIO( _consulta_socio, SI );
                break;
            case _BUSQ_ID_CLICO:
                //SELECT( AREA_SOCIOS );
                SELECT_TABLE( T_CLI_COBRO, TT_ORIG );
                //indice = GET_ORDER();            
                //SET_ORDER( 1 );
                //GET_EQUAL( ( char* )&nro_cli );
                SET_WHERE( "ID_CLI_COBRO = %ld", nro_cli );
                RUN_QUERY( NO );
                if( FOUND2() ) {
                    SET_MEMORY_LONG( __ram_cliente_afinidad, cli_cobro.nro_socio );
                    rta = 1;
                }
                ///SET_ORDER( indice );
                break;
            case _BUSQ_CODSOCIO_UNICO:
                //SELECT( AREA_SOCIOS );
                SELECT_TABLE( T_CLI_COBRO, TT_ORIG );
                //indice = GET_ORDER();
                //SET_ORDER( 7 );
                rta = ELEGIR_SOCIO( _consulta_socio2, NO );
                //SET_ORDER( indice );
                break;
            case _BUSQ_ALFABETI_REINTEGROS:
                //if( USE_DB( AREA_PAGO2, _PAGOS2_SIC, ( char * ) &pagos2, sizeof( pagos2 ), NULL, 0, 0 ) == 0 ) {
                SELECT_TABLE( T_CLI_COBRO, TT_ORIG );
                ////indice = GET_ORDER();
                //SET_ORDER( 5 );
                rta = ELEGIR_SOCIO( _consulta_socio3, SI );
                //SET_ORDER( indice );
                //CLOSE_DATABASE(AREA_PAGO2);
                //}
                break;
            case _BUSQ_CODSOCIO_UNICO_REINTEGROS:
                //if( USE_DB( AREA_PAGO2, _PAGOS2_SIC, ( char * ) &pagos2, sizeof( pagos2 ), NULL, 0, 0 ) == 0 ) {
                SELECT_TABLE( T_CLI_COBRO, TT_ORIG );
                ////indice = GET_ORDER();
                ////SET_ORDER( 2 );
                rta = ELEGIR_SOCIO( _consulta_socio4, NO );
                ////SET_ORDER( indice );
                //CLOSE_DATABASE(AREA_PAGO2);
                //}
                break;
        }//del switch
    }
    else {
        rta = 0;
    }
    if( rta && cli_cobro.fecha_baja ) {
        if( cli_cobro.fecha_baja <= GET_FECHA_DOS() ) {
            MENSAJE( "SOCIO DADO DE BAJA"/*FEHA DE BAJA SUPERIOR A 6 MESES DE ANTIGUEDAD"*/ );
        }
    }
    SELECT( area_ant );
    return ( rta );
}
/***********************************************************************/
int CLIENTE_AUTORIZADO( long nroCli, short int cod_cobro, short int tipo_cli )
/***********************************************************************/
{
    SET_MEMORY_LONG( __ram_nro_cliente, nroCli );
    if( nroCli >= 0
     && EXISTE_SOCIO( _BUSQ_ID_CLICO, nroCli, COD_UNIDAD_NEGOCIO, cod_cobro, ( char )tipo_cli,
                      NULL ) ) {
        if( cli_cobro.habilitado == 'N' ) {
            MENSAJE( "AUTORIZAR CLIENTE" );
            if( SOLICITAR_CLAVES( 30, 20, NO, SI, SI, "AUTORIZAR CLIENTE",
                                  "AUTORIZO CLIENTE INHABILITADO", _F_SUP_CLIENTE_INHAB, SI ) ) {
                //PEDRI MOTIVO
                return 1; // No habilitado, autorizado
            }
            else {
                MENSAJE_CON_PAUSA( "CLIENTE NO AUTORIZADO", 25 );
                return 0; // No habilitado, no autorizado
            }
        }
        else {
            return 1; //Habilitado
        }
    }
    else {
        if( nroCli < 0 ) {
            //es un futuro socio o un socio por defecto
            return 1;
        }
    }   
    return 0; //No existente
}
/*****************************************************************************/
int _consulta_socio( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0;
    char xx[20], fechaTemp[12] = {""};
    switch( comando ) {
        case _BUSCAR:
            //MOSTRAR_CADENA( 21 - ancho_panel / 2, 21, cadena );
            //GET_GREATER_E( cadena );
            SET_WHERE( "APELLIDOYNOMBRE LIKE '%s%%'", cadena );
            SET_ORDER2( "APELLIDOYNOMBRE" );
            RUN_QUERY( NO );
            break;
        case _MOSTRAR:
            //Apellido y Nombre / Razon Social
            //nro_socio
            //cuit
            //compañia
            //Prepaga        
            //Tipo
            //direccion
            //fecha baja
            setFieldValuepChar( b->index, 0, cli_cobro.apellynom );
            _ltoa( cli_cobro.nro_socio, xx, 10 );
            setFieldValuepChar( b->index, 1, xx );
            setFieldValuepChar( b->index, 2, cli_cobro.cuit );
            //setFieldValuepChar( b->index, 3, ( cli_cobro.unidad_negocio >= 0 ? _dUnidadNegocio[cli_cobro.unidad_negocio] : "   " ) );
            setFieldValuepChar( b->index, 3,
                                ( cli_cobro.cod_cobro > 0 ? _dCompania[cli_cobro.cod_cobro - 1]
                                : "   " ) );
            setFieldValuepChar( b->index, 4,
                                ( cli_cobro.codigo >= 0 ? _dPrepaga[cli_cobro.codigo] : "   " ) );
            setFieldValuepChar( b->index, 5,
                                ( cli_cobro.empresa >= 0 ? _dTipoSocio[cli_cobro.empresa]
                                : "   " ) );
            setFieldValuepChar( b->index, 6, cli_cobro.domicilio );
            setFieldValuepChar( b->index, 7, cli_cobro.localidad );
            if( cli_cobro.fecha_baja ) {
                DTOC_2000( cli_cobro.fecha_baja, fechaTemp );
                //MENSAJE( "SOCIO DADO DE BAJA"/*FEHA DE BAJA SUPERIOR A 6 MESES DE ANTIGUEDAD"*/ );
            } // formato dd/mm/aaaa*/
            setFieldValuepChar( b->index, 8, fechaTemp );
            break;
        case _VACIO:
            break;
        case _SKIP:
            SKIP2( dato );
            ////rta = BtrvEOF(  ) || BOF(  );
            rta = dbEOF() || dbBOF();
            break;
        case _VALIDAR:
            if( ES_IMPRIMIBLE( dato ) ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            ///rta = ( dato == -1 ) ? BOF(  ) : BtrvEOF(  );
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
/*****************************************************************************/
int _consulta_socio2( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0;
    long indice;
    char xx[20], fechaTemp[15] = {""};
    switch( comando ) {
        case _BUSCAR:
            MOSTRAR_CADENA( 21 - ancho_panel / 2, 21, cadena );
            indice = ( strlen( cadena ) > 0 ) ? strtol( cadena, NULL, 10 ) : -1000;
            SET_WHERE( "NRO_SOCIO >= %ld", indice );
            SET_ORDER2( "NRO_SOCIO" );
            RUN_QUERY( NO );
            //GET_GREATER_E( ( char* )&indice );
            break;
        case _MOSTRAR:
            setFieldValuepChar( b->index, 0, cli_cobro.apellynom );
            _ltoa( cli_cobro.nro_socio, xx, 10 );
            setFieldValuepChar( b->index, 1, xx );
            setFieldValuepChar( b->index, 2, cli_cobro.cuit );
            //setFieldValuepChar( b->index, 3, ( cli_cobro.unidad_negocio >= 0 ? _dUnidadNegocio[cli_cobro.unidad_negocio] : "   " ) );
            setFieldValuepChar( b->index, 3,
                                ( cli_cobro.cod_cobro > 0 ? _dCompania[cli_cobro.cod_cobro - 1]
                                : "   " ) );
            setFieldValuepChar( b->index, 4,
                                ( cli_cobro.codigo >= 0 ? _dPrepaga[cli_cobro.codigo] : "   " ) );
            setFieldValuepChar( b->index, 5,
                                ( cli_cobro.empresa >= 0 ? _dTipoSocio[cli_cobro.empresa]
                                : "   " ) );
            setFieldValuepChar( b->index, 6, cli_cobro.domicilio );
            setFieldValuepChar( b->index, 7, cli_cobro.localidad );
            if( cli_cobro.fecha_baja ) {
                DTOC_2000( cli_cobro.fecha_baja, fechaTemp );
                //MENSAJE( "SOCIO DADO DE BAJA"/*FEHA DE BAJA SUPERIOR A 6 MESES DE ANTIGUEDAD"*/ );
            } // formato dd/mm/aaaa
            setFieldValuepChar( b->index, 8, fechaTemp );
            break;
        case _VACIO:
            break;
        case _SKIP:
            SKIP2( dato );
            //rta = BtrvEOF() || BOF();
            rta = dbEOF() || dbBOF();
            break;
        case _VALIDAR:
            if( isdigit( dato ) ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            //rta = ( dato == -1 ) ? BOF() : BtrvEOF();
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
/*CONSULTA 3 Y 4 EN DESARROLLO: consultas para buscar clientes en la pagos2
  en cambio de la clicobro para usarlas en pagos de reintegros*/
/*****************************************************************************/
int _consulta_socio3( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0/*, fecha, anio, dia, mes*/;
    static int result = 0;
    char xx[20];
    static char claveidTok[21] = {""};
    char *punt = NULL;
    char fechaTemp[12] = {""};
    switch( comando ) {
        case _BUSCAR:
            SET_WHERE("cli_cobro.ID_CLI_COBRO = pagos2.ID_CLI_COBRO and APELLIDOYNOMBRE LIKE '%s%%'",
            cadena );
            SET_FROM( "pagos2" );
            SET_ORDER2( "APELLIDOYNOMBRE" );
            RUN_QUERY( SI );
            /*GET_GREATER_E( cadena );
            strcpy( claveidTok, pagos2.clave_id );
            punt = strtok( claveidTok, "-" );
            ind5.unidad_negocio = atoi( punt ); punt = strtok( NULL, "-" );
            ind5.cod_cobro = atoi( punt ); punt = strtok( NULL, "-" );
            ind5.nro_socio = atol( punt ); punt = strtok( NULL, "-" );
            ind5.empresa = atoi( punt ); punt = strtok( NULL, "-" );
            ind5.integrante = atoi( punt ); punt = strtok( NULL, "-" );
            SELECT( AREA_SOCIOS );
            SET_ORDER( 5 );
            result = GET_EQUAL( ( char* )&ind5 );
            SELECT( AREA_PAGO2 );
            SET_ORDER( 5 );*/
            break;
        case _MOSTRAR:
            /*if( result != 0 ) {
                //si no se encontro el cliente
                memset( &cli_cobro, 0, sizeof( struct _cli_cobros ) );
                setFieldValuepChar( b->index, 0, pagos2.nombre );
                for( rta = 1;rta < 9;rta++ ) {
                    setFieldValuepChar( b->index, rta, "..." );
                }
            }
            else {*/
            setFieldValuepChar( b->index, 0, cli_cobro.apellynom );
            _ltoa( cli_cobro.nro_socio, xx, 10 );
            setFieldValuepChar( b->index, 1, xx );
            setFieldValuepChar( b->index, 2, cli_cobro.cuit );
            //setFieldValuepChar( b->index, 3, ( cli_cobro.unidad_negocio >= 0 ? _dUnidadNegocio[cli_cobro.unidad_negocio] : "   " ) );
            setFieldValuepChar( b->index, 3,
                                ( cli_cobro.cod_cobro > 0 ? _dCompania[cli_cobro.cod_cobro - 1]
                                : "   " ) );
            setFieldValuepChar( b->index, 4,
                                ( cli_cobro.codigo >= 0 ? _dPrepaga[cli_cobro.codigo] : "   " ) );
            setFieldValuepChar( b->index, 5,
                                ( cli_cobro.empresa >= 0 ? _dTipoSocio[cli_cobro.empresa]
                                : "   " ) );
            setFieldValuepChar( b->index, 6, cli_cobro.domicilio );
            setFieldValuepChar( b->index, 7, cli_cobro.localidad );
            if( cli_cobro.fecha_baja ) {
                DTOC_2000( cli_cobro.fecha_baja, fechaTemp );
            } // formato dd/mm/aaaa*/
            setFieldValuepChar( b->index, 8, fechaTemp );
            /*}
            result = 0;*/
            break;
        case _VACIO:
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
            /*SKIP( dato );
            rta = BtrvEOF() || BOF();
            strcpy( claveidTok, pagos2.clave_id );
            punt = strtok( claveidTok, "-" );
            ind5.unidad_negocio = atoi( punt ); punt = strtok( NULL, "-" );
            ind5.cod_cobro = atoi( punt ); punt = strtok( NULL, "-" );
            ind5.nro_socio = atol( punt ); punt = strtok( NULL, "-" );
            ind5.empresa = atoi( punt ); punt = strtok( NULL, "-" );
            ind5.integrante = atoi( punt ); punt = strtok( NULL, "-" );
            SELECT( AREA_SOCIOS );
            SET_ORDER( 5 );
            result = GET_EQUAL( ( char* )&ind5 );
            SELECT( AREA_PAGO2 );
            SET_ORDER( 5 );*/
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
            rta = 1;
            /*strcpy( claveidTok, pagos2.clave_id );
            punt = strtok( claveidTok, "-" );
            ind5.unidad_negocio = atoi( punt ); punt = strtok( NULL, "-" );
            ind5.cod_cobro = atoi( punt ); punt = strtok( NULL, "-" );
            ind5.nro_socio = atol( punt ); punt = strtok( NULL, "-" );
            ind5.empresa = atoi( punt ); punt = strtok( NULL, "-" );
            ind5.integrante = atoi( punt ); punt = strtok( NULL, "-" );
            SELECT( AREA_SOCIOS );
            SET_ORDER( 5 );
            if( GET_EQUAL( ( char* )&ind5 ) ) {
                rta = 0;
            }
            SELECT( AREA_PAGO2 );
            SET_ORDER( 5 );*/
            //Se verifica que no este dado de baja hace mas de 6 meses 
            if( cli_cobro.fecha_baja ) {
                //MENSAJE( "SOCIO DADO DE BAJA"/*FEHA DE BAJA SUPERIOR A 6 MESES DE ANTIGUEDAD"*/ );
            }            
            if( !rta ) {
                MENSAJE( " SOCIO NO ENCONTRADO" );
            }
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
int _consulta_socio4( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0/*, fecha, anio, dia, mes*/;
    static int result = 0;
    static char claveidTok[21] = {""};
    char *punt = NULL;
    char fechaTemp[12] = {""};
    char xx[20];
    switch( comando ) {
        case _BUSCAR:
            SET_WHERE( "cli_cobro.ID_CLI_COBRO = pagos2.ID_CLI_COBRO and NRO_SOCIO >= %ld",
                       strtol( cadena, NULL, 10 ) );
            SET_FROM( "pagos2" );
            SET_ORDER2( "NRO_SOCIO" );
            RUN_QUERY( SI );
            /*
                        MOSTRAR_CADENA( 21 - ancho_panel / 2, 21, cadena );
                        indice.codnivel1 = 1;
                        indice.codcliente = strtol( cadena, NULL, 10 );
                        indice.codigo = 0;
                        GET_GREATER_E( ( char* )&indice );
                        strcpy( claveidTok, pagos2.clave_id );
                        punt = strtok( claveidTok, "-" );
                        ind5.unidad_negocio = atoi( punt ); punt = strtok( NULL, "-" );
                        ind5.cod_cobro = atoi( punt ); punt = strtok( NULL, "-" );
                        ind5.nro_socio = atol( punt ); punt = strtok( NULL, "-" );
                        ind5.empresa = atoi( punt ); punt = strtok( NULL, "-" );
                        ind5.integrante = atoi( punt ); punt = strtok( NULL, "-" );
                        SELECT( AREA_SOCIOS );
                        SET_ORDER( 5 );
                        result = GET_EQUAL( ( char* )&ind5 );
                        SELECT( AREA_PAGO2 );
                        SET_ORDER( 2 );*/
            break;
        case _MOSTRAR:
            /*if( result != 0 ) {
                //si no se encontro el cliente
                memset( &cli_cobro, 0, sizeof( struct _cli_cobros ) );
                setFieldValuepChar( b->index, 0, pagos2.nombre );
                for( rta = 1;rta < 9;rta++ ) {
                    setFieldValuepChar( b->index, rta, "..." );
                }
            }
            else {*/
            setFieldValuepChar( b->index, 0, cli_cobro.apellynom );
            _ltoa( cli_cobro.nro_socio, xx, 10 );
            setFieldValuepChar( b->index, 1, xx );
            setFieldValuepChar( b->index, 2, cli_cobro.cuit );
            //setFieldValuepChar( b->index, 3, ( cli_cobro.unidad_negocio >= 0 ? _dUnidadNegocio[cli_cobro.unidad_negocio] : "   " ) );
            setFieldValuepChar( b->index, 3,
                                ( cli_cobro.cod_cobro > 0 ? _dCompania[cli_cobro.cod_cobro - 1]
                                : "   " ) );
            setFieldValuepChar( b->index, 4,
                                ( cli_cobro.codigo >= 0 ? _dPrepaga[cli_cobro.codigo] : "   " ) );
            setFieldValuepChar( b->index, 5,
                                ( cli_cobro.empresa >= 0 ? _dTipoSocio[cli_cobro.empresa]
                                : "   " ) );
            setFieldValuepChar( b->index, 6, cli_cobro.domicilio );
            setFieldValuepChar( b->index, 7, cli_cobro.localidad );
            if( cli_cobro.fecha_baja ) {
                DTOC_2000( cli_cobro.fecha_baja, fechaTemp );
            } // formato dd/mm/aaaa*/
            setFieldValuepChar( b->index, 8, fechaTemp );
            /*}
            result = 0;*/
            break;
        case _VACIO:
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
            /*SKIP( dato );
            rta = BtrvEOF() || BOF();
            strcpy( claveidTok, pagos2.clave_id );
            punt = strtok( claveidTok, "-" );
            ind5.unidad_negocio = atoi( punt ); punt = strtok( NULL, "-" );
            ind5.cod_cobro = atoi( punt ); punt = strtok( NULL, "-" );
            ind5.nro_socio = atol( punt ); punt = strtok( NULL, "-" );
            ind5.empresa = atoi( punt ); punt = strtok( NULL, "-" );
            ind5.integrante = atoi( punt ); punt = strtok( NULL, "-" );
            SELECT( AREA_SOCIOS );
            SET_ORDER( 5 );
            result = GET_EQUAL( ( char* )&ind5 );
            SELECT( AREA_PAGO2 );
            SET_ORDER( 2 );*/
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
            /*strcpy( claveidTok, pagos2.clave_id );
            punt = strtok( claveidTok, "-" );
            ind5.unidad_negocio = atoi( punt ); punt = strtok( NULL, "-" );
            ind5.cod_cobro = atoi( punt ); punt = strtok( NULL, "-" );
            ind5.nro_socio = atol( punt ); punt = strtok( NULL, "-" );
            ind5.empresa = atoi( punt ); punt = strtok( NULL, "-" );
            ind5.integrante = atoi( punt ); punt = strtok( NULL, "-" );
            SELECT( AREA_SOCIOS );
            SET_ORDER( 5 );
            if( GET_EQUAL( ( char* )&ind5 ) ) {
                rta = 0;
            }
            SELECT( AREA_PAGO2 );
            SET_ORDER( 2 );*/
            //Se verifica que no este dado de baja hace mas de 6 meses
            if( rta && cli_cobro.fecha_baja ) {
                //MENSAJE( "SOCIO DADO DE BAJA"/*FEHA DE BAJA SUPERIOR A 6 MESES DE ANTIGUEDAD"*/ );
            }
            if( !rta ) {
                MENSAJE( " SOCIO NO ENCONTRADO" );
            }
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
/**************************************************************************
int ELEGIR_SOCIO_ALFABETICO( void )
/**************************************************************************
{
    int       rta = 1;
    if( !UTILIZA_DBROUTER && GET_ERROR_EN_RED(  ) ) {
        MENSAJE_STRING( S_ERROR_EN_LA_RED );
        return ( 0 );
    }
   if( !archivo_clicobro_abierto ) {
      if( !ABRIR_ARCHIVO_SOCIO(  ) )
           rta = 0;
   }
    
   if( rta ) {
        SELECT( AREA_SOCIOS );
        SET_ORDER( 2 );
        GO( TOP );
        TECLADO_ALFABETICO = 1;
        rta =   Consulta_socio( _consulta_socio );
        TECLADO_ALFABETICO = 0;
        /*--------------------- finaliza ---------------------
        SET_ORDER( 1 );
        if( rta ) {
            SET_MEMORY_LONG( __ram_nro_cliente, cli_cobro.nro_socio );
           SET_MEMORY( __cliente_afinidad_nombre, cli_cobro.apellynom );            
        }       
   }
   if( !rta ) {
      SET_MEMORY_LONG( __ram_nro_cliente, 0 );
        CEREAR_PERFIL_CLIENTE( );            
   }
   return( rta );
}
*/
/***************************************************************************/
int ELEGIR_SOCIO( int ( *f_usuario )( int, char *, int, struct _browse *, double, unsigned short ), int alfabetico )
/***************************************************************************/
{
    long rta = 1;

    if( !archivo_clicobro_abierto ) {
        if( !ABRIR_ARCHIVO_SOCIO() ) {
            rta = 0;
        }
    }
    if( rta ) {
        /*SELECT( AREA_SOCIOS );
            SET_ORDER( 7 );*/
        ////GO( TOP );
        if( alfabetico ) {
            TECLADO_ALFABETICO = 1;
        }
        rta = Consulta_socio( f_usuario );
        if( alfabetico ) {
            TECLADO_ALFABETICO = 0;
        }
        /*--------------------- finaliza ---------------------*/
        //SELECT( AREA_SOCIOS ); forray
        //SET_ORDER( 1 );        forray
        if( rta ) {
            SET_MEMORY_LONG( __ram_nro_cliente, cli_cobro.nro_socio );
            SET_MEMORY( __cliente_afinidad_nombre, cli_cobro.apellynom );
            SET_MEMORY_LONG( __ram_cliente_afinidad, cli_cobro.nro_socio );
        }
    }
    if( !rta ) {
        SET_MEMORY_LONG( __ram_nro_cliente, 0 );
        CEREAR_PERFIL_CLIENTE();
    }
    return( rta );
}
/***********************************************************************/
long PEDIR_CLIENTE_ALFABETICO( void )
/***********************************************************************/
{
    return PEDIR_CLIENTE_( _BUSQ_ALFABETI );
}
/***********************************************************************/
long PEDIR_CLIENTE_CODSOCIO( void )
/***********************************************************************/
{
    return PEDIR_CLIENTE_( _BUSQ_CODSOCIO_UNICO );
}
/***********************************************************************/
long PEDIR_CLIENTE_( int busqueda )
/***********************************************************************/
{
    long nroCli = -1;
    int rta;
    if( !EXISTE_SOCIO( busqueda, 0, COD_UNIDAD_NEGOCIO, 0, 0, NULL ) ) {
        rta = 0;
    }
    else {
        rta = 1;
    }
    if( rta ) {
        nroCli = cli_cobro.nro_socio;
    }
    if( !rta
     || ( rta
       && !CLIENTE_AUTORIZADO( cli_cobro.id_cli_cobro, cli_cobro.cod_cobro,
                               ( short int ) cli_cobro.empresa ) ) ) {
        nroCli = 0;
        rta = 0;
    }
    BORRAR_MENSAJE();
    SET_MEMORY_LONG( __ram_nro_cliente, cli_cobro.id_cli_cobro );
    SET_MEMORY_LONG( __ram_cliente_afinidad, cli_cobro.nro_socio );
    CEREAR_MEMORY( __ram_string_cliente_afinidad );
    SET_MEMORY_CHAR( __ram_pedir_cliente_afinidad, 0 );
    return( nroCli );
}/***********************************************************************************************************/
int ES_IMPRIMIBLE(int dato){
/***********************************************************************************************************/
int rta = 0;
	if( dato >= 0 && dato <= 256 && isprint( dato ) ) {
		rta = 1;
	}
	return rta;
}
