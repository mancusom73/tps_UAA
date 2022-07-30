#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <global.h>
#include <tpv_dat.h>
#include <band_hor.h> 
#include <btrv.h> 
#include <cr.h>  
#include <dbmem.h> 
#include <dbrouter.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <malloc.h> 
#include <mensaje.h> 
#include <_cr1.h> 
#include <cmemory.h>
#include <path.h>
#include <db.h>
#include <deftables.h>
#include <aarch.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifndef INVEL_L
#include <dos.h>
#endif
/*****************************************************************************/
int VERIFICAR_BANDA_HORARIA()
/*****************************************************************************/
{
    int b_art, rta = 1, l, h;
    int  area_ant, tipo_ant;
    char mensaje[90],articulo[14],banda[30];
    struct _band_acu_art *acu_art = NULL;
    struct _bandas_ventas bandas_sic;
    struct _band_art band_art_sic;
    struct _banda_clasificacion band_clas_sic;
    /*------------- Inicializaciones ---------------*/
 
	memset( articulo, 0, sizeof( articulo ) );
    memset( banda, 0, sizeof( banda ) );
    /*----- Contruye la tabla de acumuladores por artculo ----*/
    l = sizeof( struct _band_acu_art ) * _LARGO_TICKET;
    acu_art = ( struct _band_acu_art *)MALLOC( l );
    b_art = BAND_ACU_ART( acu_art, l );
    if( !acu_art ) {
        return ( 0 );
    }
    //area_ant = SELECTED();
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    
	/*-------------------- Recorre todas las bandas -------------------------*/
    if( b_art
     && ABRE_ARCHIVOS_BANDAS( ( char* )&bandas_sic, ( char* )&band_art_sic,
                              ( char* )&band_clas_sic ) && !RAM_MODO_DEVOLUCION ) {
       // SELECT( AREA_AUX ); // BANDAS_SIC
		SELECT_TABLE( T_BANDAS, TT_ORIG );
		RUN_QUERY( NO );
        //GO( TOP );
		GO2( TOP );
        //while( !BtrvEOF() && rta )
		while( !dbEOF() && rta ) { 
            if( BANDA_VIGENTE( &bandas_sic ) ) {
                /*------------ Artculos en Banda de Restriccion -------------*/
                /*
                     *for( h = 0; h < b_art && rta; h++ ) {
                     *  if( acu_art[h].cant > 0.02 &&
                     *        acu_art[h].cod_banda == bandas_sic.cod_banda ) {
                     *     strcpy( articulo, acu_art[h].cod_barra);
                     *     strcpy( banda, bandas_sic.nombre );
                     *     rta = 0;
                     *     SELECT( AREA_AUX ); //BANDAS_SIC
                     *     GO( BOTTOM );
                     *  }
                     *}
                    */
                /*-------------- Artculos en Archivo Band_Art ---------------*/
                for( h = 0;h < b_art && rta;h++ ) {
                    if( ARTICULO_EN_BAND_ART( bandas_sic.cod_banda, &( acu_art[h] ) ) ) {
                        strcpy( articulo, acu_art[h].cod_barra );
                        strcpy( banda, bandas_sic.nombre );
                        rta = 0;
                        //SELECT( AREA_AUX ); //BANDAS_SIC
						SELECT_TABLE( T_BANDAS, TT_ORIG );
						RUN_QUERY( NO );
                        //GO( BOTTOM );
						GO2( BOTTOM );
                    }
                }
                /*------- Clasificacion -------*/
                for( h = 0;h < b_art && rta;h++ ) {
                    if( CLASIFICACION_EN_BANDA( bandas_sic.cod_banda, &( acu_art[h] ) ) ) {
                        strcpy( articulo, acu_art[h].cod_barra );
                        strcpy( banda, bandas_sic.nombre );
                        rta = 0;
                         //SELECT( AREA_AUX ); //BANDAS_SIC
						SELECT_TABLE( T_BANDAS, TT_ORIG );
						RUN_QUERY( NO );
                        //GO( BOTTOM );
						GO2( BOTTOM );
                    }
                }
            }
            //SELECT( AREA_AUX ); //BANDAS_SIC
			SELECT_TABLE( T_BANDAS, TT_ORIG );
            //SKIP( 1 );
			SKIP2( 1 );
        }
    }
    if( rta == 0 ) {
        MENSAJE( banda ); // Nombre de Banda Horaria.. Ley..
        sprintf( mensaje, ST( S_EL_ITEM__S_NO_PUEDE_SER_VENDIDO_EN_ESTE_HORARIO ), articulo );
        MENSAJE_TECLA( mensaje );
    }
    CIERRA_ARCHIVOS_BANDAS();
    FREEMEM( ( char* )acu_art );
    //SELECT( area_ant );
	SELECT_TABLE( area_ant, tipo_ant );
    return( rta );
}
/*****************************************************************************/
int BANDA_VIGENTE( struct _bandas_ventas *bandas_sic )
/*****************************************************************************/
{
    int ok = 0;
    /*--------------------- Verifica fecha de vigencia ----------------------*/
    if( GET_FECHA_DOS() >= bandas_sic->fecha_desde && GET_FECHA_DOS() <= bandas_sic->fecha_hasta ) {
        if( bandas_sic->hora_desde > bandas_sic->hora_hasta ) {
            //--- Banda que pasa de medianoche.
            if( GET_HORA_DOS() >= bandas_sic->hora_desde
             || GET_HORA_DOS() <= bandas_sic->hora_hasta ) {
                ok = 1;
            }
        }
        else {
            if( GET_HORA_DOS() >= bandas_sic->hora_desde
             && GET_HORA_DOS() <= bandas_sic->hora_hasta ) {
                ok = 1;
            }
        }
    }
    return ( ok );
}
/*****************************************************************************/
int ABRE_ARCHIVOS_BANDAS( char *bandas_sic, char *band_art, char *band_clas )
/*****************************************************************************/
{
    int ok = 0;
    //if( USE_DB( AREA_AUX, _BANDAS_SIC, bandas_sic, sizeof( struct _bandas_ventas ), NULL, 0, 0 ) == 0 ) {
    if( OPEN_TABLE( T_BANDAS, TT_ORIG, ( char* )bandas_sic, sizeof( struct _bandas_ventas ) ) == 0 ) {
	
	//    if( USE_DB( AREA_AUX2, _BANDAS_ART_SIC, band_art, sizeof( struct _band_art ), NULL, 0,
    //                0 ) == 0 ) {
		if( OPEN_TABLE( T_BAND_ART, TT_ORIG, ( char* )band_art, sizeof( struct _band_art ) ) == 0 ) {
    //        if( USE_DB( AREA_AUX3, _BANDAS_CLAS_SIC, band_clas, sizeof( struct _banda_clasificacion ),
    //                    NULL, 0, 0 ) == 0 ) {
			if( OPEN_TABLE( T_BANDAS_CLAS, TT_ORIG, ( char* )band_clas, sizeof( struct _banda_clasificacion ) ) == 0 ) {
                ok = 1;
            }
        }
    }
    return ( ok );
}
/*****************************************************************************/
void CIERRA_ARCHIVOS_BANDAS()
/*****************************************************************************/
{
   // CLOSE_DATABASE( AREA_AUX );  //BANDAS_SIC
	CLOSE_TABLE( T_BANDAS, TT_ORIG );
   // CLOSE_DATABASE( AREA_AUX2 ); //BAND_ART_SIC
    CLOSE_TABLE( T_BAND_ART, TT_ORIG );
  // CLOSE_DATABASE( AREA_AUX3 ); //BAND_CLAS_SIC
	CLOSE_TABLE( T_BANDAS_CLAS, TT_ORIG );

}
/*****************************************************************************/
int ARTICULO_EN_BAND_ART( int cod_banda, struct _band_acu_art *art )
/*****************************************************************************/
{
    struct ind1_band_art_sic indice1;
    struct ind2_band_art_sic indice2;
    int ok = 0;
    if( art->cant > 0.02 ) {
        //SELECT( AREA_AUX2 ); // BAND_ART_SIC
		SELECT_TABLE( T_BAND_ART, TT_ORIG );
		
        //GO( TOP );
		GO2( TOP );
        if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
            //SET_ORDER( 1 );
			SET_ORDER2("COD_BANDA, COD_ART, COD_BARRA");
            indice2.cod_banda = (short)cod_banda;
            indice2.cod_art = (unsigned long)art->cod;
            memset( indice2.cod_barra, 0, 17 );
            strncpy( indice2.cod_barra, art->cod_barra, 17 );
            //GET_EQUAL( ( char* )&indice2 );
			SET_WHERE("COD_BANDA = '%d' and  COD_ART = '%ld' and COD_BARRA = '%s'", 
					indice2.cod_banda, indice2.cod_art, indice2.cod_barra );
			RUN_QUERY( NO );
        }
        else {
            //SET_ORDER( 0 );
			SET_ORDER2("COD_BANDA, COD_ART");
            indice1.cod_banda = (short)cod_banda;
            indice1.cod_art = (unsigned long)art->cod;
            //GET_EQUAL( ( char* )&indice1 );
			SET_WHERE("COD_BANDA = '%d' and  COD_ART = '%ld'", indice1.cod_banda, indice1.cod_art );
			RUN_QUERY( NO );
        }
        //if( FOUND() ) {
		if( FOUND2() ) {
            ok = 1;
        }
    }
    return ( ok );
}
/*****************************************************************************/
int BAND_ACU_ART( struct _band_acu_art *acu_art, int l )
/*****************************************************************************/
{
    int h, i, signo, encontro, b_art = 0;
    float cantidad;
    if( acu_art ) {
        memset( acu_art, 0, l );
        for( h = 0;h < RAM_P_TICKET;h++ ) {
            signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
            cantidad = _TICKET_CANTIDAD( h ) * signo;
            encontro = 0;
            for( i = 0;i < b_art && !encontro;i++ ) {
                if( _TICKET_COD_ESPECIAL( h ) == acu_art[i].cod
                 && _TICKET_PRECIO( h ) == _TICKET_PRECIO( acu_art[i].pos ) ) {
                    encontro = 1;
                    acu_art[i].cant += cantidad;
                }
                if( _TICKET_COD_ESPECIAL( h ) == acu_art[i].cod
                 && fabs( _TICKET_PRECIO( h ) ) == fabs( _TICKET_PRECIO( acu_art[i].pos ) )
                 && ( ( _TICKET_PRECIO( h ) < _TICKET_PRECIO( acu_art[i].pos ) )
                   || ( _TICKET_PRECIO( h ) > _TICKET_PRECIO( acu_art[i].pos ) ) ) ) {
                    cantidad -= 1;
                    acu_art[i].cant = acu_art[i].cant - 1;
                }
            }
            if( !encontro ) {
                acu_art[b_art].cod = _TICKET_COD_ESPECIAL( h );
                acu_art[b_art].cant = cantidad;
                acu_art[b_art].pos = h;
                acu_art[b_art].importe = _TICKET_PRECIO( h );
                acu_art[b_art].cod_banda = _TICKET_COD_BANDA( h );
                memcpy( acu_art[b_art].cod_barra, _TICKET_COD_BARRA( h ), 17 );
                b_art++;
            }
        }
    }
    return ( b_art );
}
/*******************************************************************************/
int BANDAS_RECORRER_CLASIF( struct _clasif1 *clasif, struct _datos_clasif *datos )
/*******************************************************************************/
{
    struct ind_band_clas_sic ind;
    char aux_old[10], aux[35];
    /*------------------- Calcula el valor a buscar ---------------------*/
    _snprintf( aux_old, sizeof(aux_old)-1, "%9li", datos->cod_clas_old );
	aux_old[clasif->digito_fin] = 0;
	sprintf( aux, datos->cod_clas );    
    /*--------------------- Busca si esta en band_clas ----------------------*/
    //SELECT( AREA_AUX3 ); //BANDA_CLAS
	SELECT_TABLE( T_BANDAS_CLAS, TT_ORIG );
    //GO( TOP );
    ind.cod_banda = datos->cod_banda;
    ind.cod_clasificacion = clasif->cod_clasif1;
    ind.valor = atol( &aux[clasif->digito_ini - 1] );
    SET_WHERE("COD_BANDA = '%d' and COD_CLASIFICACION = '%d' and VALOR = '%ld'", ind.cod_banda, ind.cod_clasificacion, ind.valor );
	RUN_QUERY( NO );
	//GET_EQUAL( ( char* )&ind );
   // if( FOUND() ) {
	 if( FOUND2() ) {
        if( datos->cantidad > 0.01 ) {
            datos->encontro = 1;
            //GO( BOTTOM );
        }
    }
    return 0;
}
/***************************************************************************/
int CLASIFICACION_EN_BANDA( int cod_banda, struct _band_acu_art *art )
/***************************************************************************/
{
    struct _datos_clasif datos;
    datos.cod_banda = cod_banda;
	datos.cod_clas_old = _TICKET_COD_CLASIFICACION( art->pos );
    datos.encontro = 0;
    datos.cantidad = ( int )( art->cant );
	sprintf( datos.cod_clas, _GET_MEMORY_STR( __ticket_cod_clasificacion_str, art->pos ) );
    DBMEM_RECORRER( _CLASIF1_SIC, BANDAS_RECORRER_CLASIF, ( char* )&datos );
    return ( datos.encontro );
}

