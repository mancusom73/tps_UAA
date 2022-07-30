/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

#pragma pack(1)
#include <cr.h>
#include <config.h>
#include <btrv.h>
#include <stdio.h>
#include <mstring.h>
#include <string.h>
#include <log.h>
#include <mensaje.h>
#include <_cr1.h>
#include <m_cadena.h>
#include <b_mens.h>
#include <db.h>
#include <deftables.h>
#include <path.h>
#include <ini.h>
#include <pausa.h> 
int mtime()
{
    return clock();
}



/*****************************************************************************/
void glog( char *men, int origen, int nivel )
/*****************************************************************************/
{
	if( DEBE_GRABAR_LOG_SISTEMA1( origen, nivel ))
		_GRABAR_LOG_SISTEMA( men,origen, nivel );
}

/*****************************************************************************/
void MENSAJE_STRING( int id )
/*****************************************************************************/
{
    MENSAJE( ST( id ) );
}

/******************************************************************************/
void MENSAJE_STRING_SIN_PAUSA( int id )
/******************************************************************************/
{
	MENSAJE_SIN_PAUSA( ST( id ) );

}
/******************************************************************************/
void MENSAJE_ID_CON_PAUSA_CONF( int id )// se toma del stringt la configuracion de la pausa para mostrar el mensaje
/******************************************************************************/
{
	char buff[1024];
	int area_ant, tipo_ant;

    area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    SELECT_TABLE(T_STRINGT, TT_ORIG);
    SET_WHERE("id = %d", id);
    RUN_QUERY( NO );
    if( FOUND2() ) {
		MENSAJE_SIN_SONIDO( string_table->mensaje,string_table->pausa * 100  );
		_snprintf( buff, 1023, "MSG: Mensaje ID Con Pausa Conf %s", string_table->mensaje );
		buff[1023] = '\0';
		GRABAR_LOG_SISTEMA_CONFIG( buff, LOG_VARIOS,2);
    }
    else {
        char mensaje[80];
        sprintf( mensaje, "ID MENSAJE DESCONOCIDO %d", id );
        GRABAR_LOG_SISTEMA_CONFIG( mensaje, LOG_INCONDICIONAL,1);
    }
    SELECT_TABLE(area_ant, tipo_ant);
}

/******************************************************************************/
void MENSAJE_ID_CON_PAUSA_CAJA( int id )//se frena la ejecucion con la duracion de la pausa configurada en el stringt
/******************************************************************************/
{
	char buff[1024];
	int area_ant, tipo_ant;

    area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    SELECT_TABLE(T_STRINGT, TT_ORIG);
    SET_WHERE("id = %d", id);
    RUN_QUERY( NO );
    if( FOUND2() ) {
		MENSAJE_SIN_SONIDO( string_table->mensaje,NO );
		_snprintf( buff, 1023, "MSG: Mensaje ID Con Pausa Caja %s", string_table->mensaje );
		buff[1023] = '\0';
		GRABAR_LOG_SISTEMA_CONFIG( buff, LOG_VARIOS,2);
		PAUSA( string_table->pausa  );// NO MULTIPLICAR PORQUE LLEVA MUCHO TIEMPO EL CICLO.
    }
    else {
        char mensaje[80];
        sprintf( mensaje, "ID MENSAJE DESCONOCIDO %d", id );
        GRABAR_LOG_SISTEMA_CONFIG( mensaje, LOG_VARIOS,2);
    }
    SELECT_TABLE(area_ant, tipo_ant);
}

/*****************************************************************************/
void MENSAJE_TECLA_STRING( int id )
/*****************************************************************************/
{
    int area_ant, tipo_ant;
    area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    SELECT_TABLE(T_STRINGT, TT_ORIG);
    SET_WHERE("id = %d", id);
    RUN_QUERY( NO );

    if( FOUND2() ) {
        MENSAJE_TECLA( string_table->mensaje );
    }
    else {
        MENSAJE_NO_ENCONTRADO( id );
    }
    SELECT_TABLE(area_ant, tipo_ant);
}


/*****************************************************************************/
void MENSAJE_SIN_SONIDO_STRING( int id )
/*****************************************************************************/
{
        MENSAJE_STRING( id );
}

/*****************************************************************************/
void MOSTRAR_CADENA_CENTRADA_STRING( int x, int y, int id )
/*****************************************************************************/
{
    /*int area;

    area = SELECTED();
    SELECT( AREA_STRING_TABLE );
    GET_EQUAL( ( char* )&id );
    if( FOUND() ) {
        MOSTRAR_CADENA( x - strlen( string_table->mensaje ) / 2, y, string_table->mensaje );
    }
    else {
        MENSAJE_NO_ENCONTRADO( id );
    }
    SELECT( area );*/

    int area_ant, tipo_ant;
    area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    SELECT_TABLE(T_STRINGT, TT_ORIG);
    SET_WHERE("id = %d", id);
    RUN_QUERY( NO );
    if( FOUND2() ) {
        MOSTRAR_CADENA( x - strlen( string_table->mensaje ) / 2, y, string_table->mensaje );
    }
    else {
        MENSAJE_NO_ENCONTRADO( id );
    }
    SELECT_TABLE(area_ant, tipo_ant);
}


/*****************************************************************************/
void ABORT_STRING( int id )
/*****************************************************************************/
{
}


/*****************************************************************************/
void MENSAJE_NO_ENCONTRADO( int id )
/*****************************************************************************/
{
}


/*****************************************************************************/
void LOG_MENSAJE_NO_ENCONTRADO( int id )
/*****************************************************************************/
{
    char men[100];
		
    _snprintf( men, sizeof(men)-1, "MENSAJE NO ENCONTRADO %d", id );

    GRABAR_LOG_SISTEMA( men,LOG_INCONDICIONAL,2);
}
/*****************************************************************************/
void GRABAR_LOG_SISTEMA_STRING( int id ,int origen, int nivel)
/*****************************************************************************/
{
	if( DEBE_GRABAR_LOG_SISTEMA1( origen, nivel ))
		_GRABAR_LOG_SISTEMA_STRING( id,origen, nivel );
}
/*****************************************************************************/
void _GRABAR_LOG_SISTEMA_STRING( int id ,int origen, int nivel)
/*****************************************************************************/
{
    int area_ant, tipo_ant;
    area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    if(area_ant < 1){
        tipo_ant = tipo_ant;
    }
    SELECT_TABLE(T_STRINGT, TT_ORIG);
    SET_WHERE("id = %d", id);
    RUN_QUERY( NO );
    if( FOUND2() ) {
        GRABAR_LOG_SISTEMA( string_table->mensaje,origen, nivel);
    }
    else {
        LOG_MENSAJE_NO_ENCONTRADO( id );
    }
    SELECT_TABLE(area_ant, tipo_ant);
}


/******************************************************************************/
void MENSAJE_CON_TECLA_STRING( int id, int tecla )
/******************************************************************************/
{
    /*int area;

    area = SELECTED();
    SELECT( AREA_STRING_TABLE );
    GET_EQUAL( ( char* )&id );
    if( FOUND() ) {
        MENSAJE( string_table->mensaje );
    }
    else {
        MENSAJE_NO_ENCONTRADO( id );
    }
    while( GETCH() != tecla );
    BORRAR_MENSAJE();
    SELECT( area );*/

    int area_ant, tipo_ant;
    area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    SELECT_TABLE(T_STRINGT, TT_ORIG);
    SET_WHERE("id = %d", id);
    RUN_QUERY( NO );
    if( FOUND2() ) {
        MENSAJE( string_table->mensaje );
    }
    else {
        MENSAJE_NO_ENCONTRADO( id );
    }
    while( GETCH() != tecla );
    BORRAR_MENSAJE();

    SELECT_TABLE(area_ant, tipo_ant);
}


/*****************************************************************************/
void LOG__( char *men )
/*****************************************************************************/
{
}
