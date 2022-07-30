#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <actual.h> 
#include <bancos.h>
#include <btrv.h> 
#include <b_mens.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <eventos.h> 
#include <ftoc.h> 
#include <getch.h> 
#include <log.h> 
#include <med_rein.h> 
#include <mensaje.h> 
#include <pausa.h> 
#include <tkt.h> 
#include <_cr1.h> 
#include <_cr0.h> 
#include <aarch.h>
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
#include <myodbc.h>
extern struct _pagos2 pagos2;
struct pagoslist
{
    struct _pagos2 lista;
    int estado;
};
typedef struct pagoslist listapagos;
listapagos *listapagostemp = NULL;
struct _cajas_suc caja_suc;
int cantfiles;
void PANTALLA_MEDIO_REINT();
void FIN_PANTALLA_MEDIO_REINT();
int BUSCAR_MEDIOS();
int OPCION_GRILLA_MR( int cant, int init );
void SELECCIONA( int elemento );
int BUSQ_REINTEG( long cod );
void REMOV_ULTIMO();
void GRABAR_EVENTO_CARGA_REINTEGRO( void );
long OBTENER_CODIGO( char *banco, char *nro_cheque );
/*
** __x_cons_final_importe_exent_tkt     == importe total de cheques en caja
** __x_cons_final_cantidad_tkt          == cantidad total de cheques en caja
** __x_dev_cons_final_importe_exent_tkt == importe total de cheques pagados
** __x_dev_cons_f_cantidad_tkt          == cantidad total de cheques pagados
** __x_cons_final_importe_grav_tkt      == importe total de cheques retirados
*/
/***************************************************************************/
void PACKLISTA( void )
/***************************************************************************/
{
    // esta funcion elimina todos los elementos marcados
    // con estado <> 0
    int i;
    int a;
    int cantfiles = 10;
    listapagos *temp = NULL;
    //temp = malloc( 500 * sizeof * temp );
    SELECT_TABLE( T_PAGOS2, TT_ORIG );
    cantfiles += RECCOUNT2( NULL, "" );
    if( temp == NULL ) {
        temp = malloc( cantfiles * sizeof( struct pagoslist ) * sizeof( struct _pagos2 ) );
    }
    a = 0;
    i = 0;
    //for( i = 0;i < 500;i++ ) {
    while( i <= cantfiles ) {
        if( listapagostemp[i].estado == 0 ) {
            temp[a] = listapagostemp[i];
            a++;
        }
        i++;
    }
    // vuelvo a elegir el area anterior
    //Con "=", significa una asignacion de memoria. Ahora listapagostemp no esta en 
    //la direccion original, ahora esta en la misma direccion de temp.
    //listapagostemp = temp;
    //encambio en la siguiente linea se tiene copia de array. 
    //La copia se realiza elemento por elemento.    
    i = 0;
    while( i <= a ) {
        listapagostemp[i] = temp[i];         
        i++;
    }
    if( temp ) {
        free( temp );
        temp = NULL;
    }
}
/***************************************************************************/
void MEDIO_REINT( void )
/***************************************************************************/
{
    int cantidad, elemento = 0;
    listapagostemp = NULL;
    PANTALLA_MEDIO_REINT();
    if( !OPENED( T_PAGOS2, TT_ORIG )
     && ( !OPEN_TABLE( T_PAGOS2, TT_ORIG, ( char* )&pagos2, sizeof( pagos2 ) ) == 0 ) ) {
        GRABAR_LOG_DBROUTER( "ERROR ABRIENDO PAGOS2",LOG_ERRORES,2 );
        MENSAJE( "ERROR ABRIENDO TABLA PAGOS2" );
        return;
    }
  //  if( !USED( AREA_MEDREINT )
   //  && ( !USE_DB( AREA_MEDREINT, _MEDIOS_P_REINTEGROS, ( char* )&medios_para_reintegros,
   //                sizeof( medios_para_reintegros ), NULL, 0, 0 ) == 0 ) ) {
    if(  !OPENED( T_MEDIOS_REINT, TT_ORIG )
		&& ( !OPEN_TABLE( T_MEDIOS_REINT, TT_ORIG, ( char* )&medios_para_reintegros, sizeof( medios_para_reintegros ) ) == 0 ) ) {

		GRABAR_LOG_DBROUTER( "ERROR ABRIENDO MEDREINT.BTR" ,LOG_ERRORES,2);
        MENSAJE( "ERROR ABRIENDO TABLA MEDREINT" );
        return;
    }

    if( !OPENED( T_CAJA_SUC, TT_ORIG )
     && ( !OPEN_TABLE( T_CAJA_SUC, TT_ORIG, ( char* )&caja_suc, sizeof( caja_suc ) ) == 0 ) ) {
        GRABAR_LOG_DBROUTER( "ERROR ABRIENDO CAJA_SUC" ,LOG_ERRORES,2);
        MENSAJE( "ERROR ABRIENDO TABLA CAJA_SUC" );
        return;
    }
    //Se obtienen los medios de la Pagos2 y se mustran
    //los medios cargados en la tabla medioRrint.
    //Cantidad es la cantidad total de medios en Pagos2
    cantidad = BUSCAR_MEDIOS();
    elemento = OPCION_GRILLA_MR( cantidad, SI );
    while( elemento != -1 ) {
        if( elemento >= 0 ) {
            SELECCIONA( elemento );
            PACKLISTA();
        }
        elemento = OPCION_GRILLA_MR( cantidad, NO );
    }
    GRABAR_EVENTO_CARGA_REINTEGRO();
	CLOSE_TABLE( T_CAJA_SUC, TT_ORIG );
    CLOSE_TABLE( T_MEDIOS_REINT, TT_ORIG );
	//CLOSE_DATABASE( AREA_MEDREINT );
	CLOSE_TABLE( T_PAGOS2, TT_ORIG );
    //CLOSE_DATABASE( AREA_PAGO2 );
    FIN_PANTALLA_MEDIO_REINT();
}
/***************************************************************************/
void PANTALLA_MEDIO_REINT()
/***************************************************************************/
{
    ExecuteAction( A_DES_COBROSESP_ACT_MEDIOREINT );
    ExecuteSSSAction( A_SELEC_FILA_MED_REINT, "0", NULL, NULL );    
    PAUSA( 1 );
    MENSAJE( "<ENTER>:AGREGAR   <->:QUITAR ULTIMO   <BARR ESP>:TERMINAR" );
}
/***************************************************************************/
void FIN_PANTALLA_MEDIO_REINT()
/***************************************************************************/
{
    ExecuteAction( A_DES_MEDIOREINT_ACT_COBROSESP );
    if( listapagostemp ) {
        // libero la lista temporal de pagos
        free( listapagostemp );
    }
}
/***************************************************************************/
int BUSCAR_MEDIOS( void )
/***************************************************************************/
{
    int /*i,*/ cantidad = 0, sucursal;
    char strBanco[10];
    char strNroCheq[20];
    char strNomBanco[21];
    char importe[20];
    char codigo_rein[20];
    char granStrNroCheq[21];
    cantfiles = 10;
    memset( strNroCheq, 0, sizeof( strNroCheq ) );
    memset( granStrNroCheq, 0, sizeof( granStrNroCheq ) );
    memset( strNomBanco, 0, sizeof( strNomBanco ) );
    if( listapagostemp == NULL ) {
        SELECT_TABLE( T_PAGOS2, TT_ORIG );
        RUN_QUERY( NO );
        cantfiles += RECCOUNT2( NULL, NULL );
        listapagostemp = malloc( cantfiles * sizeof( struct pagoslist )
                               * sizeof( struct _pagos2 ) );
    }
    //Se muestran los medios ya cargados anteriormente
    CLOSE_DATABASE( AREA_MEDREINT );
   // USE_DB( AREA_MEDREINT, _MEDIOS_P_REINTEGROS, ( char* )&medios_para_reintegros,
   //         sizeof( medios_para_reintegros ), NULL, 0, 0 );
   // SELECT( AREA_MEDREINT );
	OPEN_TABLE( T_MEDIOS_REINT, TT_ORIG, ( char* )&medios_para_reintegros, sizeof( medios_para_reintegros ) );
	SELECT_TABLE( T_MEDIOS_REINT, TT_ORIG );
	SET_ORDER2( "COD_NIVEL1, CODIGO" );
	RUN_QUERY(NO);
    //SET_ORDER( 1 );//codigo_nivel1+codigo
    //GET_FIRST();
    while( !dbEOF() && !dbBOF() ) {
        memset( strNroCheq, 0, sizeof( strNroCheq ) );
        memset( granStrNroCheq, 0, sizeof( granStrNroCheq ) );
        memset( strNomBanco, 0, sizeof( strNomBanco ) );
        //if( medios_para_reintegros.nro_cheque != 0 || strlen( medios_para_reintegros.nro_chq_str ) > 0 ) {        
        if( medios_para_reintegros.cod_nivel1 != 0 && medios_para_reintegros.codigo != 0 ) {
            sprintf( strNroCheq, "%ld", medios_para_reintegros.nro_cheque );
            strncpy( granStrNroCheq, medios_para_reintegros.nro_chq_str,
                     strlen( medios_para_reintegros.nro_chq_str ) );
            sprintf( strBanco, "%i", medios_para_reintegros.banco );
            BUSCAR_BANCO( medios_para_reintegros.banco, strNomBanco );
            FTOC( medios_para_reintegros.importe, importe, ENTEROS, DECIMALES, 1, 0 );                
            sprintf( codigo_rein, "%ld", medios_para_reintegros.idlong );
            ExecuteSSSSSAction( A_AGREGAR_MEDIO_REINTEGRO_ING, 0, strNomBanco, strBanco,
                                ( strlen( granStrNroCheq ) > 0 ) ? granStrNroCheq : strNroCheq,
                                importe, codigo_rein );
        }
        //GET_NEXT();
		SKIP2( 1 );
    }
    //Se muestran los medios disponibles
    sucursal = SUCURSAL_DE_CAJA();
    SELECT_TABLE( T_PAGOS2, TT_ORIG );
    SET_WHERE( "SUC_PAGO = %i", sucursal );
    SET_ORDER2( "COD_NIVEL1, CODIGO" );
    RUN_QUERY( NO );
    //SET_ORDER( 1 ); //codigo_nivel1+codigo
    //GET_FIRST();
    while( !dbEOF() && !dbBOF() ) {
        //Condiciones: medio=Cheque y medio no cargado en mreinteg y
        //caja perteneciente a la sucursal asignada al cheque
		if( pagos2.cod_medio && medios[pagos2.cod_medio - 1].cheque && !BUSQ_REINTEG( pagos2.codigo ) ) {
            if( strlen( strNroCheq ) > 0 || strlen( granStrNroCheq ) > 0 ) {
                if( pagos2.nro_cheque > 0 || strlen( pagos2.nro_chq_str ) > 0 ) {
                    strncpy( granStrNroCheq, pagos2.nro_chq_str, strlen( pagos2.nro_chq_str ) );
                    sprintf( strNroCheq, "%ld", pagos2.nro_cheque );
                }
                else {
                    memset( strNroCheq, 0, sizeof( strNroCheq ) );
                    memset( granStrNroCheq, 0, sizeof( granStrNroCheq ) );
                }
            }
            else {
                if( pagos2.nro_cheque != 0 || strlen( pagos2.nro_chq_str ) > 0 ) {
                    strncpy( granStrNroCheq, pagos2.nro_chq_str, strlen( pagos2.nro_chq_str ) );
                    sprintf( strNroCheq, "%ld", pagos2.nro_cheque );
                }
                else {
                    memset( strNroCheq, 0, sizeof( strNroCheq ) );
                    memset( granStrNroCheq, 0, sizeof( granStrNroCheq ) );
                }
            }
            memset( strNomBanco, 0, sizeof( strNomBanco ) );
            sprintf( strBanco, "%i", pagos2.banco );
            BUSCAR_BANCO( pagos2.banco, strNomBanco );
            FTOC( pagos2.importe, importe, ENTEROS, DECIMALES, 1, 0 );
            sprintf( codigo_rein, "%ld", pagos2.idlong );
            // Agrego el elemento actual pagos2 a la lista listapagostemp
            listapagostemp[cantidad].lista = pagos2;
            listapagostemp[cantidad].estado = 0;
            ExecuteSSSSSAction( A_AGREGAR_MEDIO_REINTEGRO, 0, strNomBanco, strBanco,
                                ( strlen( granStrNroCheq ) > 0 ) ? granStrNroCheq : strNroCheq,
                                importe, codigo_rein );
            cantidad++;
        }
        //GET_NEXT();
        SKIP2( 1 );
    }
    return( cantidad );
}
/***************************************************************************/
int OPCION_GRILLA_MR( int cant, int init )
/***************************************************************************/
{
    #define posGrilla_n atoi(posGrilla)
    #define cantGrilla_n atoi(cantGrilla)
    int seguir = 1, k;
    char cantGrilla[5],posGrilla[5];
    while( seguir ) {
        k = GETCH();
        switch( k ) {
            case 32:
                /*-------- TOTAL ---------*/
                ExecuteSSSAction( A_SELEC_FILA_MED_REINT, "-99", cantGrilla, posGrilla );
                seguir = 0;
                return -1;
                break;
            case 13:
                /*-------- ENTER ---------*/
                ExecuteSSSAction( A_SELEC_FILA_MED_REINT, "-99", cantGrilla, posGrilla );
                //Si se esta en la ultima linea pero que no sea a la vez la primera.
                if( ( cantGrilla_n - 1 == posGrilla_n ) && ( posGrilla_n > 0 ) ) {
                    ExecuteSSSAction( A_SELEC_FILA_MED_REINT, "-1", NULL, NULL );
                }
                seguir = 0;
                break;
            case -72:
                /*----- FLECHA ARRIBA -----*/
                ExecuteSSSAction( A_SELEC_FILA_MED_REINT, "-1", cantGrilla, posGrilla );
                break;
            case -80:
                /*------ FLECHA ABAJO -----*/
                ExecuteSSSAction( A_SELEC_FILA_MED_REINT, "1", cantGrilla, posGrilla );
                break;
            case 45:
                /*--- - ----*/
                REMOV_ULTIMO();
                /*Selecciona el primero*/
                ExecuteSSSAction( A_SELEC_FILA_MED_REINT, "0", cantGrilla, posGrilla );
                break;
            case 132:
                /*------ A -----*/
                ExecuteSSSAction( A_SELEC_FILA_MED_REINT, "-99", cantGrilla, posGrilla );
                ExecuteSAction( A_SELEC_FILA_MED_ING, "-1" );
                break;
            case -61:
                /*------ Z -----*/
                ExecuteSSSAction( A_SELEC_FILA_MED_REINT, "-99", cantGrilla, posGrilla );
                ExecuteSAction( A_SELEC_FILA_MED_ING, "1" );
                break;
        }
    }
    return ( posGrilla_n );
    #undef posGrilla_n
    #undef cantGrilla_n
}
/***************************************************************************/
void SELECCIONA( int elemento )
/***************************************************************************/
{
    //   int i;
    char strBanco[10];
    char strNroCheq[20];
    char strNomBanco[21];
    char importe[20];
    char codigo_rein[20];
    char aux[22];
    char elemento_char[11];
    char cantGrilla[5];
    char granStrNroCheq[21];
    memset( aux, 0, sizeof( aux ) );
    memset( elemento_char, 0, sizeof( elemento_char ) );
    memset( strNroCheq, 0, sizeof( strNroCheq ) );
    memset( granStrNroCheq, 0, sizeof( granStrNroCheq ) );
    //Se verifica cantidad de elementos Disponibles en la grilla
    ExecuteSSSAction( A_SELEC_FILA_MED_REINT, "-99", cantGrilla, NULL );
    if( atoi( cantGrilla ) > 0 ) {
        sprintf( aux, "%d", elemento );
        //Se le pasa la linea seleccionada y devuelve el codigo del reintegro
        ExecuteSAction( A_GET_DATA_MEDREIN_DISP, aux );
        //Se elimina la fila de la grilla "Disponibles"
        sprintf( elemento_char, "%d", elemento );
        ////GRABAR_LOG_SISTEMA(elemento_char);
        ExecuteSAction( A_QUITAR_FILA_MED_DISP, elemento_char );
        //SELECT( AREA_MEDREINT );
		SELECT_TABLE( T_MEDIOS_REINT, TT_ORIG );
        medios_para_reintegros.banco = listapagostemp[elemento].lista.banco;
        medios_para_reintegros.cod_medio = listapagostemp[elemento].lista.cod_medio;
        medios_para_reintegros.cod_nivel1 = listapagostemp[elemento].lista.cod_nivel1;
        medios_para_reintegros.cod_sub_medio = listapagostemp[elemento].lista.cod_sub_medio;
        medios_para_reintegros.codigo = listapagostemp[elemento].lista.codigo;        
        medios_para_reintegros.idlong = listapagostemp[elemento].lista.idlong;        
        medios_para_reintegros.importe = listapagostemp[elemento].lista.importe;
        medios_para_reintegros.nro_cheque = listapagostemp[elemento].lista.nro_cheque;
        medios_para_reintegros.estado = _CHQ_EN_CAJA;//en la caja y no enviado al dbrouter, 
        //strncpy( medios_para_reintegros.nro_chq_str, listapagostemp[elemento].lista.nro_chq_str, strlen( listapagostemp[elemento].lista.nro_chq_str ) );
        memset( medios_para_reintegros.nro_chq_str, 0,
                strlen( medios_para_reintegros.nro_chq_str ) );
        strcpy( medios_para_reintegros.nro_chq_str, listapagostemp[elemento].lista.nro_chq_str );        
        medios_para_reintegros.id_cli_cobro = listapagostemp[elemento].lista.id_cli_cobro;
        #ifdef INVEL_L
       // BEGIN_TRANSACTION();
        #endif
        INSERT2(NO);
        #ifdef INVEL_L
        //END_TRANSACTION();
        #endif
        //Se inserta el elemento en la grilla de "Ingresados"
        if( listapagostemp[elemento].lista.nro_cheque != 0
         || strlen( listapagostemp[elemento].lista.nro_chq_str ) > 0 ) {
            /*sprintf( elemento_char, ":%d", elemento );
            GRABAR_LOG_SISTEMA(elemento_char);*/
            sprintf( strNroCheq, "%ld", listapagostemp[elemento].lista.nro_cheque );
            strncpy( granStrNroCheq, listapagostemp[elemento].lista.nro_chq_str,
                     strlen( listapagostemp[elemento].lista.nro_chq_str ) );
        }
        else {
            memset( strNroCheq, 0, sizeof( strNroCheq ) );
            memset( granStrNroCheq, 0, sizeof( granStrNroCheq ) );
        }
        memset( strNomBanco, 0, 21 );
        sprintf( strBanco, "%d", listapagostemp[elemento].lista.banco );
        FTOC( medios_para_reintegros.importe, importe, ENTEROS, DECIMALES, 1, 0 );        
        BUSCAR_BANCO( listapagostemp[elemento].lista.banco, strNomBanco );
        sprintf( codigo_rein, "%ld", medios_para_reintegros.idlong );
        listapagostemp[elemento].estado = -1;
        ExecuteSSSSSAction( A_AGREGAR_MEDIO_REINTEGRO_ING, 0, strNomBanco, strBanco,
                            ( strlen( granStrNroCheq ) > 0 ) ? granStrNroCheq : strNroCheq, importe,
                            codigo_rein );
        //Se refresca la fila seleccionada
        ExecuteSSSAction( A_SELEC_FILA_MED_REINT, "-99", NULL, NULL );
    }
}
/***************************************************************************/
/* Existe un registro con el codigo @cod 
 * en la tabla MREINTEG ?
 */
int BUSQ_REINTEG( long cod )
/***************************************************************************/
{
    int found;
    struct _ind_mreintegro_1 ind;
	int  area_ant, tipo_ant;
	
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    
	ind.cod_nivel1 = 1;
    ind.codigo = cod;
    //selected = SELECTED();
    //SELECT( AREA_MEDREINT );
    //SET_ORDER( 1 );
    //GET_EQUAL( ( char* )&ind );
	SELECT_TABLE( T_MEDIOS_REINT, TT_ORIG );
	SET_WHERE("COD_NIVEL1 = '%ld' and CODIGO = '%ld'", ind.cod_nivel1, ind.codigo );
	SET_ORDER2( "COD_NIVEL1, CODIGO" );	
	RUN_QUERY(NO);

    found = FOUND2(); 
    //SELECT( selected );
	SELECT_TABLE( area_ant, tipo_ant );
    return found;
}
/***************************************************************************/
int SUCURSAL_DE_CAJA( void )
/***************************************************************************/
{
    int selected, selected_tipo, suc;
    INT16 nro_caja;

	selected = SELECTED2( );
	selected_tipo = SELECTED_TIPO();

    nro_caja = ( INT16 )NRO_CAJA;
	SELECT_TABLE( T_CAJA_SUC, TT_ORIG );
	SET_ORDER2( "NUM_CAJA" );
	SET_WHERE( "NUM_CAJA = '%i'", nro_caja );
	RUN_QUERY( NO );
    if( FOUND2() ) {
        suc = caja_suc.sucursal;
    }
    else {
        suc = 0;
    }
	SELECT_TABLE( selected, selected_tipo );

    return suc;
}
/***************************************************************************/
long OBTENER_CODIGO( char *banco, char *nro_cheque )
/***************************************************************************/
{
    /* Esta funcion recibe el parametro idlong
       y devuelve el codigo correspondiente 
       en la tabla PAGO2 */
    int oldarea = SELECTED2();
    int oldtipo = SELECTED_TIPO();
    SELECT_TABLE( T_PAGOS2, TT_ORIG );
    SET_WHERE( "BANCO = '%s' and NRO_CHQ_STR = '%s'", banco, nro_cheque/*SUCURSAL_DE_CAJA()*/ );
    RUN_QUERY( NO );
    /* vuelvo al area anterior */
    SELECT_TABLE( oldarea, oldtipo );
    /* retorno el resultado de la busqueda */
    return( pagos2.codigo );
}
/***************************************************************************/
void REMOV_ULTIMO()
/***************************************************************************/
{
    char strBanco[5];
    char strNroCheq[10];
    char strNomBanco[21];
    char importe[20];
    char codigo_rein[20];
    char aux[20];
    char aux2[20];
    struct _ind_mreintegro_1 ind;
    int i = 0; 
    long codigo = 0;
    char granStrNroCheq[21];
    memset( aux, 0, 20 );
    memset( strNroCheq, 0, sizeof( strNroCheq ) );
    memset( granStrNroCheq, 0, sizeof( granStrNroCheq ) );
    /* Se le pasa la linea seleccionada y devuelve el banco y nro_cheque */
    ExecuteSSAction( A_GET_DATA_MEDREIN_ING, 0,aux, aux2, NULL, " " );
    /* ahora usa el codigo encontrado para buscar en MReinteg */
    //SELECT( AREA_MEDREINT );
    //SET_ORDER( 1 );
    ind.cod_nivel1 = 1;
    ind.codigo = OBTENER_CODIGO( aux, aux2 );
    GET_EQUAL( ( char* )&ind );
    if( ( FOUND() ) && ( medios_para_reintegros.estado == _CHQ_EN_CAJA ) ) {
        //en la caja y no enviado al dbrouter, 
        //Se vuelve a poner la linea entre los disponibles
        if( pagos2.nro_cheque != 0 || strlen( pagos2.nro_chq_str ) > 0 ) {
            sprintf( strNroCheq, "%ld", medios_para_reintegros.nro_cheque );
            strncpy( granStrNroCheq, medios_para_reintegros.nro_chq_str,
                     strlen( medios_para_reintegros.nro_chq_str ) );
        }
        else {
            memset( strNroCheq, 0, sizeof( strNroCheq ) );
            memset( granStrNroCheq, 0, sizeof( granStrNroCheq ) );
        } 
        memset( strNomBanco, 0, 21 );
        sprintf( strBanco, "%d", medios_para_reintegros.banco );
        FTOC( medios_para_reintegros.importe, importe, ENTEROS, DECIMALES, 1, 0 );        
        BUSCAR_BANCO( medios_para_reintegros.banco, strNomBanco );
        sprintf( codigo_rein, "%ld", medios_para_reintegros.codigo );
        // Agrego el reintegro a la grilla izquierda
        ExecuteSSSSSAction( A_AGREGAR_MEDIO_REINTEGRO, 0, strNomBanco, strBanco,
                            ( strlen( granStrNroCheq ) > 0 ) ? granStrNroCheq : strNroCheq, importe,
                            codigo_rein );
        // elimino el reintegro de la grilla derecha
        ExecuteSAction( A_QUITAR_MEDIO_REINTEGRO, " "/*tmp*/ );//Siempre es el de abajo
        #ifdef INVEL_L
        BEGIN_TRANSACTION();
        #endif
        _DELETE();
        #ifdef INVEL_L
        END_TRANSACTION();
        #endif
        // limpia la grilla
        ExecuteAction( A_QUITAR_FILAS_REINTEGROS );
        if( listapagostemp ) {
            // libero la listapagostemp
            free( listapagostemp );
            listapagostemp = NULL;
        }
        // recarga los datos de la grilla
        BUSCAR_MEDIOS();
        // Se refresca la fila seleccionada
        ExecuteSSSAction( A_SELEC_FILA_MED_REINT, "-99", NULL, NULL );
    }
}
/***************************************************************************/
void GRABAR_EVENTO_CARGA_REINTEGRO( void )
/***************************************************************************/
{
    int cantreint = 0, area_ant = 0, seguir = 1, cant_reg = 0, h = 0;
    double total = 0.0;
    while( seguir ) {
        CEREAR_STRUCT( _pago );
        _CEREAR_MEMORY( __pago_auto_tarjeta, cantreint );
        _CEREAR_MEMORY( __pago_buffer, cantreint );
        _CEREAR_MEMORY( __pago_tabla, cantreint );
        _CEREAR_MEMORY( __pago_nro_tarjeta, cantreint );
        cant_reg = 0;
        cantreint = 0;
        total = 0.0;
        SELECT( AREA_MEDREINT );
        GO( TOP );
        //cuento los reintegros que hay para enviar al dbrouter
        while( !BtrvEOF() && !BOF() ) {
            if( medios_para_reintegros.estado == _CHQ_EN_CAJA ) {
                cant_reg++;
            }
            SKIP( 1 );
        }
        GO( TOP );
        while( cant_reg && !BtrvEOF() && !BOF() ) {
            //en la caja y no enviado al dbrouter,
            if( cantreint < 10 ) {
                if( medios_para_reintegros.estado == _CHQ_EN_CAJA ) {
                    medios_para_reintegros.estado = _CHQ_ENVIADO;
                    #ifdef INVEL_L
                    BEGIN_TRANSACTION();
                    #endif
                    UPDATE();
                    #ifdef INVEL_L
                    END_TRANSACTION();
                    #endif
                    //************************ agrego a la rnv*              
                    _SET_MEMORY_LONG( __pago_numero, cantreint, medios_para_reintegros.nro_cheque );
                    _SET_MEMORY_LONG( __pago_nro_cupon1, cantreint, medios_para_reintegros.codigo );
                    _SET_MEMORY_LONG( __pago_nro_cupon_int1, cantreint,
                                      medios_para_reintegros.cod_nivel1 );
                    _SET_MEMORY_INT( __pago_cuenta, cantreint, 0 );   
                    _SET_MEMORY_INT( __pago_cod_solicitud, cantreint, 0 );
                    _SET_MEMORY_CHAR( __pago_modo, cantreint,
                                      medios_para_reintegros.cod_medio - 1 );
                    _SET_MEMORY_CHAR( __pago_id_registro, cantreint,
                                      medios[medios_para_reintegros.cod_medio - 1].id_registro );
                    _SET_MEMORY_INT( __pago_cod_submedio, cantreint,
                                     medios_para_reintegros.cod_sub_medio );
                    _SET_MEMORY_DOUBLE( __pago_importe, cantreint, medios_para_reintegros.importe );       
                    _SET_MEMORY_INT( __pago_banco, cantreint, medios_para_reintegros.banco );
                    _SET_MEMORY_LONG( __pago_buffer1_long, cantreint,
                                      medios_para_reintegros.id_cli_cobro );
                    _SET_MEMORY_DOUBLE( __pago_valor_cupones, cantreint,
                                        ( double )medios_para_reintegros.idlong );
                    _SET_MEMORY( __pago_nro_tarjeta, cantreint,
                                 medios_para_reintegros.nro_chq_str );
                    total += medios_para_reintegros.importe;
                    //*****************************************
                    cantreint++;
                }
                SKIP( 1 );
            }
            else {
                break;
            }
        }
        //esto es una truchada: pero sabemos que son todos cheques
        h = medios_para_reintegros.cod_medio - 1;
        if( cantreint ) {
            ADD_MEMORY_DOUBLE( __x_cons_final_importe_exent_tkt, total );//total de cheques en caja
            ADD_MEMORY_INT( __x_cons_final_cantidad_tkt, cantreint ); //cantidad total de cheques en caja
        }
        if( cantreint ) {
            SET_MEMORY_CHAR( __ram_p_pago, cantreint );
            memset( &event_ticket, 0, sizeof( event_ticket ) );
            event_ticket.bultos = cantreint;
            event_ticket.tipo_evento = _EVENT_VALORES_EN_SUCURSAL;  //cobros especiales    
            event_ticket.hora_inicio = RAM_HORA_INICIO_EVENTO;
            event_ticket.nro_cliente = 0;
            event_ticket.cliente_afinidad = 0;
            event_ticket.nro_rollo = 0;
            event_ticket.bien_de_uso = 0;
            event_ticket.suc_comprobante = 0;
            event_ticket.nro_comprobante_new = 0;
            event_ticket.nro_cotizacion = 0;
            event_ticket.importe_sin_iva = total;
            area_ant = SELECTED();
            GRABAR_EVENT_TICKET( NO );
            GRABA_EVENTOS_PAGO( 0, NO );
            SELECT( area_ant );
            ACTUALIZA_ID_EVENTO_NEW();
            ACTUALIZAR_X();
        }
        //si todavia quedan pendientes de grabar, vuelvo a ejecutar la funcion.
        if( cant_reg > cantreint ) {
            //sigo enviando al dbrouter
        }
        else {
            seguir = 0;
        }
    }
}
/***************************************************************************/
void RECUPERO_DE_CHEQUES_CARGADOS( void )
/***************************************************************************/
{
    int ok = 1, cant_reg = 0;
    double total = 0.0;
   
	if( MODO_NEGOCIO == SERVICIOS ) {
        GRABAR_LOG_SISTEMA( "Recupero de cheques cargados" ,LOG_VARIOS,4);
       // if( USED( AREA_MEDREINT ) ) {
       //     CLOSE_DATABASE( AREA_MEDREINT );
       // }
		if( !OPENED( T_MEDIOS_REINT, TT_ORIG ) ){	
			if( OPEN_TABLE( T_MEDIOS_REINT, TT_ORIG, ( char* )&medios_para_reintegros, sizeof( medios_para_reintegros ) ) == 0 ) {		
				ok = 1;
			}
		} else { 
			ok = 1; 
		}
        if( ok ) {
			SELECT_TABLE( T_MEDIOS_REINT, TT_ORIG );
			SET_ORDER2( "COD_NIVEL1, CODIGO" );
			RUN_QUERY(NO);	

            if( RECCOUNT2(NULL,NULL) ) {
                if( !ES_SUPERVISOR( NRO_CAJERO ) ) {
                    //estamos en una X
                    GRABAR_EVENTO_CARGA_REINTEGRO();
                }
                else {
                    //estamos en una Z
                    //tengo que acumular lo de las X  
                    //SELECT( AREA_MEDREINT );
					SELECT_TABLE( T_MEDIOS_REINT, TT_ORIG );
					RUN_QUERY(NO);
                    GO2( TOP );                    
                    while( !dbEOF() && !dbBOF() ) {
                        total += medios_para_reintegros.importe;
                        cant_reg++;                        
                        SKIP2( 1 );
                    } 
                    if( cant_reg ) {
                        SET_MEMORY_DOUBLE( __z_cons_final_importe_exent_tkt, total );//total de cheques en caja
                        SET_MEMORY_INT( __z_cons_final_cantidad_tkt, cant_reg ); //cantidad total de cheques en caja
                    }
                }
            }
            else {
                ok = 0;
            }
            //CLOSE_DATABASE( AREA_MEDREINT );
			CLOSE_TABLE( T_MEDIOS_REINT, TT_ORIG );
        }        
        if( ok ) {
            MENSAJE( "FIN CARGA AUTOMATICA DE CHEQUES PARA REINTEGROS EN CAJA" );
            BORRAR_MENSAJE();
        }
    }
    GRABAR_LOG_SISTEMA( "FIN Recupero de cheques cargados" , LOG_VARIOS,4);
}

/***************************************************************************/
int ACTUALIZA_MREINTEG( long codnivel1, long codigo, short nro )
/***************************************************************************/
{
    int ok = 1;
    if( medios[nro].cheque ) {
        struct _medios_para_reintegros medios_para_reintegros; 
        struct _ind_mreintegro_1 ind_mreintegro_1;
        if( ok ) {
            ind_mreintegro_1.cod_nivel1 = codnivel1;
            ind_mreintegro_1.codigo = codigo;
           // if( USED( AREA_MEDREINT ) ) {
			if(  !OPENED( T_MEDIOS_REINT, TT_ORIG ) ) {	
            //    CLOSE_DATABASE( AREA_MEDREINT );
				 CLOSE_TABLE( T_MEDIOS_REINT, TT_ORIG );
            }   
           // if( USE_DB( AREA_MEDREINT, _MEDIOS_P_REINTEGROS, ( char* )&medios_para_reintegros,
           //             sizeof( medios_para_reintegros ), NULL, 0, 0 ) == 0 ) {
			if( OPEN_TABLE( T_MEDIOS_REINT, TT_ORIG, ( char* )&medios_para_reintegros, sizeof( medios_para_reintegros ) ) == 0 ) {
				SELECT_TABLE( T_MEDIOS_REINT, TT_ORIG );
				SET_WHERE("COD_NIVEL1 = '%ld' and CODIGO = '%ld'", ind_mreintegro_1.cod_nivel1, ind_mreintegro_1.codigo );
				SET_ORDER2( "COD_NIVEL1, CODIGO" );	
				RUN_QUERY(NO);
				// SELECT( AREA_MEDREINT );
               // GO( TOP );
               // SET_ORDER( 1 );         
                ok = 0;
                //GET_EQUAL( ( char* )&ind_mreintegro_1 );
                if( FOUND2() ) {
                    //if( medios_para_reintegros.cod_nivel1 == ind_mreintegro_1.cod_nivel1
                    //    && medios_para_reintegros.codigo == ind_mreintegro_1.codigo ) {
                    ok = 1;
                }
                if( !ok ) {
                    MENSAJE_CON_PAUSA( "EL MEDIO NO FUE CARGADO EN EL POS", 25 );
                    //GRABAR_LOG_SISTEMA( "EL MEDIO NO FUE CARGADO EN EL POS" , LOG_VENTAS,2);
                    //CLOSE_DATABASE( AREA_MEDREINT );
					 CLOSE_TABLE( T_MEDIOS_REINT, TT_ORIG );
                    return( 0 );
                }
                else {
                    //#ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    //#endif                     
                    DELETE2();    
                    //#ifdef INVEL_L
                    //END_TRANSACTION();
                    //#endif
                }
            }
        }
    }
    return( ok );
}
/***************************************************************************/
void MREINTEG_EN_CIERRE( void )
/***************************************************************************/
{
    int ok = 1;
	int  area_ant, tipo_ant;
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
   // int selected;
    //selected = SELECTED();
    if( MODO_NEGOCIO == SERVICIOS ) {

		if(  !OPENED( T_MEDIOS_REINT, TT_ORIG ) ) {	
			CLOSE_TABLE( T_MEDIOS_REINT, TT_ORIG );
		}   
           // if( USE_DB( AREA_MEDREINT, _MEDIOS_P_REINTEGROS, ( char* )&medios_para_reintegros,
           //             sizeof( medios_para_reintegros ), NULL, 0, 0 ) == 0 ) {
		if( OPEN_TABLE( T_MEDIOS_REINT, TT_ORIG, ( char* )&medios_para_reintegros, sizeof( medios_para_reintegros ) ) == 0 ) {	
       // SELECT( AREA_MEDREINT );
			ok = 0;
		}
			SELECT_TABLE( T_MEDIOS_REINT, TT_ORIG );
			RUN_QUERY(NO);
        if( ok ) {
            if( RECCOUNT2( NULL, NULL) ) {
                //si hay cheques que no pudieron ser enviados, se envian en el cierre.
                GRABAR_EVENTO_CARGA_REINTEGRO();  
                //Ahora cambio de estado todos los cheques enviados y que todavia estan 
                //en caja, para que si hago un cierre X, volverlos a cargar.
                //SELECT( AREA_MEDREINT ); 
				SELECT_TABLE( T_MEDIOS_REINT, TT_ORIG );
				RUN_QUERY(NO);
                GO2( TOP );
                while( !dbEOF() ) {
                    if( medios_para_reintegros.estado == _CHQ_ENVIADO ) {
                        medios_para_reintegros.estado = _CHQ_EN_CAJA;
                        #ifdef INVEL_L
                        //BEGIN_TRANSACTION();
                        #endif                     
                        UPDATE2(NO);    
                        #ifdef INVEL_L
                        //END_TRANSACTION();
                        #endif                    
                    }
                    SKIP2( 1 );
                }
            }
            else {
                ok = 0;
            }
        }
        //CLOSE_DATABASE( AREA_MEDREINT );        
		CLOSE_TABLE( T_MEDIOS_REINT, TT_ORIG );
        //SELECT( selected );
		SELECT_TABLE( area_ant, tipo_ant );
    }
}
/***************************************************************************/
long OBTENER_CODIGO_REINTEG( long codnivel1, long codigo, short nro )
/***************************************************************************/
{
    long ok = 0;
    if( medios[nro].cheque ) {
        struct _medios_para_reintegros medios_para_reintegros; 
        struct _ind_mreintegro_1 ind_mreintegro_1;
        ind_mreintegro_1.cod_nivel1 = codnivel1;
        ind_mreintegro_1.codigo = codigo;
       // if( USED( AREA_MEDREINT ) ) {
        //    CLOSE_DATABASE( AREA_MEDREINT );
       // }   
		
		if(  !OPENED( T_MEDIOS_REINT, TT_ORIG ) ) {	
			CLOSE_TABLE( T_MEDIOS_REINT, TT_ORIG );
		}   
      //  if( USE_DB( AREA_MEDREINT, _MEDIOS_P_REINTEGROS, ( char* )&medios_para_reintegros,
      //              sizeof( medios_para_reintegros ), NULL, 0, 0 ) == 0 ) {
          //  SELECT( AREA_MEDREINT );
          //  GO( TOP );
          //  SET_ORDER( 1 );         
          //  GET_EQUAL( ( char* )&ind_mreintegro_1 );

		if( OPEN_TABLE( T_MEDIOS_REINT, TT_ORIG, ( char* )&medios_para_reintegros, sizeof( medios_para_reintegros ) ) == 0 ) {
			SELECT_TABLE( T_MEDIOS_REINT, TT_ORIG );
			SET_WHERE("COD_NIVEL1 = '%ld' and CODIGO = '%ld'", ind_mreintegro_1.cod_nivel1, ind_mreintegro_1.codigo );
			SET_ORDER2( "COD_NIVEL1, CODIGO" );	
			RUN_QUERY(NO);
			if( FOUND2() ) {
				ok = medios_para_reintegros.idlong;
			}
        }
       // CLOSE_DATABASE( AREA_MEDREINT );
		CLOSE_TABLE( T_MEDIOS_REINT, TT_ORIG );
    }
    return( ok );
}
