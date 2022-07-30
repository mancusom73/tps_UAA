#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
//#include <browse.h> 
#include <btrv.h> 
#include <b_mens.h> 
#include <clave.h> 
#include <clientes.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <cr_pant.h> 
#include <dbmem.h> 
#include <dbrouter.h> 
#include <m_cadena.h> 
#include <pant.h> 
#include <perfiles.h> 
#include <stringt.h> 
#include <tkt.h> 
#include <_cr1.h>
#include <db.h>
#include <deftables.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_PERFILES
#include <string.h>
#include <aarch.h>
#include <actions.h>
#include <typeacti.h>
float PORC_DESC( int cod );
struct _ind1_clas_per_sic
{
    char cod_clasif1;
    long valor;
    INT16 cod_perfil;
};
struct _datos_recorrer
{
    long cod_clas;
    INT16 cod_desc;
};
struct _ind_perf_des
{
    INT16 cod_perfil;
    INT16 cod_desc;
};
struct _perfiles *_perfiles;
struct _perfiles perfiles;
static struct _art_perf art_perf;
static struct _perf_des perf_des;
static struct _clas_per clas_per;
//static int _elegir_perfil( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
static int perfil_on = 0;
static int perfil_ant = 0;
/*****************************************************************************/
static int _elegir_perfil( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0, cod;
    switch( comando ) {
        case _BUSCAR:
            MOSTRAR_CADENA( 8, 16, cadena );
            cod = atoi( cadena );
			SET_WHERE("COD_PERF >= '%i'",cod);
			SET_ORDER2("COD_PERF");
			RUN_QUERY(NO);
            //GET_GREATER_E( ( char* )&cod );
            break;
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[10];
                _itoa( _perfiles->cod_perfil, xx, 10 );
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, _perfiles->nombre );
            }
            #endif
            MOSTRAR_ENTERO( 8, 10 + dato, "99", _perfiles->cod_perfil );
            MOSTRAR_CADENA( 11, 10 + dato, _perfiles->nombre );
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
int MENU_PERFILES()
/***************************************************************************/
{
    int rta = 0, ok;
    /*---------------------------- Abre el archivo ---------------------------*/
    _perfiles = &perfiles;
    if( OPEN_TABLE(T_PERFILES, TT_ORIG,( char* )&perfiles, sizeof( perfiles ))== 0 ) {
        /*-------------------------- Prepara la pantalla -------------------------*/
		SELECT_TABLE( T_PERFILES, TT_ORIG );
        MOSTRAR_CADENA( 6, 15, "��������Ĵ" ); 
        ok = Consulta_pide_perfil( _elegir_perfil );
        if( ok ) {
            SET_MEMORY_INT( __ram_perfil, perfiles.cod_perfil );
            SET_MEMORY( __ram_nom_perfil, perfiles.nombre );
            SET_MEMORY_CHAR( __ram_perfil_excluir_articulos, perfiles.excluir_articulos );
            rta = 1;
        }
        else {
            SET_MEMORY_INT( __ram_perfil, 0 );
        }
        CLOSE_TABLE( T_PERFILES, TT_ORIG );
    }
    BORRAR_MENSAJE();
    return ( rta );
}
/***************************************************************************/
void INGRESA_PERFIL()
/***************************************************************************/
{
    if( SOLICITAR_CLAVES( 30, 21, NO, SI, PEDIR_CLAVE_EN_PERFILES, ST( S__SELECCIONAR_PERFIL_ ),
                          NULL, RAM_COD_FUNCION, SI ) ) {
        /* los perfiles se manejan por promocion. En el caso de selectos 
         * RAM_PERFIL_EXCLUIR_ARTICULOS es D. Los descuentos se toman siempre de la tabla
         * perf_des.
         */
        MENU_PERFILES();
        MUESTRA_PERFIL();
    }
}
/***************************************************************************/
int ACTUALIZA_NOM_PERFIL()
/***************************************************************************/
{
    struct _perfiles *perfiles;
    int cod, rta = 1;
    perfiles = ( struct _perfiles * )malloc( sizeof( struct _perfiles ) );
    if( /*USE_DB( 0, _perfiles, ( char* )perfiles, sizeof( perfiles ), NULL, 0, 0 )*/
		OPEN_TABLE(T_PERFILES, TT_ORIG,( char* )perfiles, sizeof( struct _perfiles ))== 0 ) {
		SELECT_TABLE( T_PERFILES, TT_ORIG );
        cod = RAM_PERFIL;
        //GET_EQUAL( ( char* )&cod );
		SET_WHERE("COD_PERF = '%i'",cod);
		RUN_QUERY(NO);
        if( FOUND2() ) {
            SET_MEMORY( __ram_nom_perfil, perfiles->nombre );
            SET_MEMORY_CHAR( __ram_perfil_excluir_articulos, perfiles->excluir_articulos );
        }
        else {
            rta = 0;
        }
        CLOSE_TABLE( T_PERFILES, TT_ORIG );
    }
    else {
        rta = 0;
    }
    if( perfiles)
    free( ( void* )perfiles );
    return rta;
}
/***************************************************************************/
long COD_DESCUENTO_PERFIL()
/***************************************************************************/
{
    struct _perfiles perfiles;
    int cod;
    long rta = 0;
    if( /*USE_DB( AREA_PERFILES, _perfiles, ( char* )&perfiles, sizeof( perfiles ), NULL, 0, 0 )*/
     OPEN_TABLE(T_PERFILES, TT_ORIG,( char* )&perfiles, sizeof( perfiles ))== 0 ) {
		SELECT_TABLE( T_PERFILES, TT_ORIG );
        cod = RAM_PERFIL;
		SET_WHERE("COD_PERF = '%i'",cod),
        //GET_EQUAL( ( char* )&cod );
		RUN_QUERY(NO);
        if( FOUND2() ) {
            rta = perfiles.cod_interno;
        }
        CLOSE_TABLE( T_PERFILES, TT_ORIG );
    }
    return ( rta );
}
#endif
/***************************************************************************/
void MUESTRA_PERFIL()
/***************************************************************************/
{
    #ifdef COMPILAR_PERFILES
    char cad[15] ;
	memset(cad,0, sizeof(cad));
    if( MODO_NEGOCIO != SERVICIOS ) {
        if( perfil_on && RAM_PERFIL && RAM_PERFIL != perfil_ant ) {
            strcpy( cad, RAM_NOM_PERFIL );
            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_PERFIL_BORRAR, " ", " ", NULL, " " );                 
            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_PERFIL_AGREGAR, cad, " ", NULL, " " );    
            perfil_ant = RAM_PERFIL;
        }
        else if( perfil_on && !RAM_PERFIL ) {
            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_PERFIL_BORRAR, " ", " ", NULL, " " );           
            perfil_ant = RAM_PERFIL;
            perfil_on = 0;
        }
        else if( RAM_PERFIL && !perfil_on ) {
            strcpy( cad, RAM_NOM_PERFIL );
            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_PERFIL_AGREGAR, cad, " ", NULL, " " );    
            perfil_ant = RAM_PERFIL;
            perfil_on = 1;
        }
    }
    #endif 
}
#ifdef COMPILAR_PERFILES
/***********************************************************************/
int ABRIR_PERFILES()
/***********************************************************************/
{
    int error = 0;
    if( UTILIZA_PERFILES ) {
        error = /*USE_DB( AREA_PERF_DES, _perf_des, ( char* )&perf_des, sizeof( perf_des ), NULL,
                        0, 0 );*/
					OPEN_TABLE(T_PERF_DES, TT_ORIG,( char* )&perf_des, sizeof( perf_des ));
        error += /*USE_DB( AREA_ART_PERF, _art_perf, ( char* )&art_perf, sizeof( art_perf ), NULL,
                         0, 0 );*/
				 OPEN_TABLE(T_ART_PERF, TT_ORIG,( char* )&art_perf, sizeof( art_perf ));
        error += /*USE_DB( AREA_CLAS_PER, _CLAS_PER_SIC, ( char* )&clas_per, sizeof( clas_per ), NULL,
                         0, 0 );*/
				 OPEN_TABLE(T_CLAS_PER, TT_ORIG,( char* )&clas_per, sizeof( clas_per ));
    }
    return ( !error );
}
/***********************************************************************/
void CERRAR_PERFILES()
/***********************************************************************/
{
    //CLOSE_DATABASE( AREA_PERF_DES );
	CLOSE_TABLE( T_PERF_DES, TT_ORIG );
    //CLOSE_DATABASE( AREA_ART_PERF );
	CLOSE_TABLE( T_ART_PERF, TT_ORIG );
    CLOSE_TABLE( T_CLAS_PER, TT_ORIG );
}
/***********************************************************************/
int RECORRER_CLASIF1( struct _clasif1 *clasif1, struct _datos_recorrer *datos )
/***********************************************************************/
{
    struct _ind1_clas_per_sic ind;
    char aux[10];
    /*------------------- Calcula el valor a buscar ---------------------*/
    _snprintf(aux, sizeof(aux)-1, "%9li", datos->cod_clas );
    aux[clasif1->digito_fin] = 0;
    /*--------------------- Busca si hay descuento ----------------------*/
    ind.cod_clasif1 = clasif1->cod_clasif1;
    // CLASIFICACION2
    ind.valor = atol( &aux[clasif1->digito_ini - 1] );
    ind.cod_perfil = RAM_PERFIL;
    //SELECT( AREA_CLAS_PER );
	SELECT_TABLE( T_CLAS_PER, TT_ORIG );
    //GET_EQUAL( ( char* )&ind );
	SET_WHERE("COD_CLASIF1 = '%i' and COD_CLASIF2 = '%ld' and COD_PERFIL = '%i'",
			ind.cod_clasif1,
			ind.valor,
			ind.cod_perfil);
	RUN_QUERY(NO);
    if( FOUND2() ) {
        datos->cod_desc = clas_per.cod_desc;
    }
    return 0;
}
/***********************************************************************
float CALCULA_PORC_PERFIL( long cod, long cod_clasificacion )
/***********************************************************************
{
    struct
    {
        long cod_interno;
        int cod_perfil;
    } ind_perf;
    struct _datos_recorrer datos;
    float rta = 0;
    long cliente;
    if( UTILIZA_PERFILES && RAM_PERFIL ) {
        //--------------- Busca a nivel de articulo ----------------
        //SELECT( AREA_ART_PERF );
		SELECT_TABLE( T_ART_PERF, TT_ORIG);
        ind_perf.cod_interno = cod;
        ind_perf.cod_perfil = RAM_PERFIL;
		SET_WHERE("COD_INTERNO = '%ld' and COD_PERFIL = '%i'",
				ind_perf.cod_interno,
				ind_perf.cod_perfil);
		RUN_QUERY(NO);
        //GET_EQUAL( ( char* )&ind_perf );
        if( FOUND2() ) {
            rta = PORC_DESC( art_perf.cod_desc );
        }
        //--------------- Busca a nivel de clasificaciones --------------
        else {
            datos.cod_desc = 0;
            datos.cod_clas = cod_clasificacion;
            DBMEM_RECORRER( _CLASIF1_SIC, RECORRER_CLASIF1, ( char* )&datos );
            if( datos.cod_desc ) {
                rta = PORC_DESC( datos.cod_desc );
            }
        }
        //--------------- Busca a nivel de cliente --------------
        //Para Selectos asumimos que existe un nico descuento por perfil de cliente
        cliente = EXISTE_CLIENTE( RAM_NRO_CLIENTE );
        if( clientes.cod_perfil && cliente ) {
            rta = PORC_DESC( 1 );
        }
    }
    return ( rta );
}*/
/***********************************************************************/
float PORC_DESC( int cod )
/***********************************************************************/
{
    struct _ind_perf_des ind_perf_des;
    float rta = 0;
    //SELECT( AREA_PERF_DES );
	SELECT_TABLE( T_PERF_DES, TT_ORIG );
    ind_perf_des.cod_desc = (short)cod;
    ind_perf_des.cod_perfil = RAM_PERFIL;
	SET_WHERE("COD_DESC = '%i' and COD_PERFIL = '%i'",
			ind_perf_des.cod_desc,
			ind_perf_des.cod_perfil);
    //GET_EQUAL( ( char* )&ind_perf_des );
	RUN_QUERY(NO);
    if( FOUND2() ) {
        rta = perf_des.descuento;
    }
    return ( rta );
}
/*****************************************************************************/
int INGRESA_NRO_LEGAJO_EMPLEADO()
/*****************************************************************************/
{
    int rta = 0;
    char legajo[16];
    memset( legajo, 0, sizeof( legajo ) );
    _PERMITIR_SCANNER = SI;
    if( USAR_LEGAJO == RUT ) {
        rta = PEDIR_CADENA_EN_RECUADRO( ST( S__NRO__RUT_ ), SI, 15, legajo, NULL );
    }
    else {
        rta = PEDIR_CADENA_EN_RECUADRO( ST( S__NRO__LEGAJO_ ), SI, 15, legajo,
                                        config.mascara_legajo );
    }
    _PERMITIR_SCANNER = NO;
    SET_MEMORY( __ram_string_legajo_empleado, legajo );
    if( rta && CODIGO_DE_PERFIL_EMPLEADOS ) {
        SET_MEMORY_INT( __ram_perfil, CODIGO_DE_PERFIL_EMPLEADOS );
        if( ACTUALIZA_NOM_PERFIL() == 0 ) {
            rta = 0;
        }
    }
    else {
        if( CODIGO_DE_PERFIL_EMPLEADOS ) {
            SET_MEMORY_INT( __ram_perfil, 0 );
            ACTUALIZA_NOM_PERFIL();
        }
    }
    if( !rta ) {
        CEREAR_MEMORY( __ram_string_legajo_empleado );
    }
    MUESTRA_PERFIL();
    return rta;
}
/***************************************************************************/
int ASIGNA_PERFILES( char *cadena )
/***************************************************************************/
{
    int rta = 0;
    /*---------------------------- Abre el archivo ---------------------------*/
    _perfiles = &perfiles;
    if( /*USE_DB( AREA_PERFILES, _perfiles, ( char* )&perfiles, sizeof( perfiles ), NULL, 0, 0 )*/
     OPEN_TABLE(T_PERFILES, TT_ORIG,( char* )&perfiles, sizeof( perfiles ))== 0 ) {
        /*-------------------------- Prepara la pantalla -------------------------*/
		SELECT_TABLE( T_PERFILES, TT_ORIG );
        MOSTRAR_CADENA( 6, 15, "������������" );
        rta = Consulta_pide_perfil( _elegir_perfil );
        if( rta ) {
            clientes.cod_perfil = perfiles.cod_perfil;
            strcpy( cadena, perfiles.nombre );
        }
        else {
            strcpy( cadena, " " );
        }
        rta = 1;
        CLOSE_TABLE( T_PERFILES, TT_ORIG );
    }
    return ( rta );
}
#endif
/*****************************************************************************/
int TIPO_PERFIL( void )
/*****************************************************************************/
{
    int /*area_ant,*/ rta = 0;
    struct _ind_perf_des ind_perf_des;
    //struct _perfiles perfiles;   
    //area_ant = SELECTED();
	int tabla_ant = SELECTED2();
	int tipo_ant = SELECTED_TIPO();
	
    /*if( USE_DB( AREA_PERFILES, _perfiles, ( char * ) &perfiles, sizeof( perfiles ),
       NULL, 0, 0 ) == 0 ) {
       cod = RAM_PERFIL;
       GET_EQUAL( ( char * ) &cod );
       if( FOUND(  ) ) {
          rta = perfiles.tipo;
       }
       CLOSE_DATABASE( AREA_PERFILES );
    }*/
    if( /*USE_DB( AREA_PERF_DESC, _perf_des, ( char* )&perf_des, sizeof( perf_des ), NULL, 0, 0 )*/
     OPEN_TABLE(T_PERF_DES, TT_ORIG,( char* )&perf_des, sizeof( perf_des ))== 0 ) {
		SELECT_TABLE( T_PERF_DES, TT_ORIG );
        ind_perf_des.cod_desc = 1; 
        ind_perf_des.cod_perfil = (short)RAM_PERFIL;
		SET_WHERE("COD_DESC = '%i' and COD_PERFIL = '%i'",
			ind_perf_des.cod_desc,
			ind_perf_des.cod_perfil);
        //GET_EQUAL( ( char* )&ind_perf_des );
		RUN_QUERY(NO);
        if( FOUND2() ) {
            rta = perf_des.tipo;
        }
        CLOSE_TABLE( T_PERF_DES, TT_ORIG );
    }
    //SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );
    return( rta );
}

