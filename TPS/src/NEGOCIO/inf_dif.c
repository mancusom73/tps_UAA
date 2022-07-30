#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <btrv.h> 
#include <cierre.h> 
#include <cmemory.h> 
#include <comprob.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <imp_tick.h> 
#include <inf.h> 
#include <inf_dif.h> 
#include <log.h> 
#include <m_cadena.h> 
#include <print.h> 
#include <tkt.h> 
#include <trim.h> 
#include <_cr1.h>
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/*-------------------------------------------------------------------*
 * Utilizado por CIERRE.C para imprimir los informes de diferencias  *
 *-------------------------------------------------------------------*/
#define  _INF_DIF_C
#undef   _INF_DIF_C
extern int calcHojas;
extern int cantCopias;
extern int cantTotalCopias;
/*****************************************************************************/
void INFORME_DIFERENCIAS( double *dif, int cierre, double *dif_me )
/*****************************************************************************/
{
    double total = 0;
    int h;
    /*if( TKT_CONFIGURABLE ) {
		
        IMPRIMIR_TKT_INF_DIFERENCIAS( );

    }
    else {*/
        /*---------------------------- cabecera ---------------------------------*/
        IMPRIME_CABECERA_TICKET( SI );

        IMPRIMIR_USUARIO_Y_SUPERVISOR();


        MOSTRAR_CADENA( 15, PRN, ST( S_DIFERENCIAS ) );
        LF( 1 );
        if( cierre ) {
            MOSTRAR_CADENA( 13, PRN, ST( S_CIERRE_DE_CAJA ) );
        }
        else {
            MOSTRAR_CADENA( 13, PRN, ST( S_ARQUEO_DE_CAJA ) );
        }
        IMPRIME_NRO_INFORME( 0 );
        LF_RECEIPT( 1 );
        /*--------------------------- cuerpo -----------------------------------*/
        for( h = 0;h < 9;h++ ) {
            if( medios[h].nro ) {
                INFORME_DIFERENCIAS_CUERPO( medios[h].nombre, dif[h], dif_me[h] );
                total += dif[h];
            }
        }
        MOSTRAR_CADENA( 25, PRN, "----------" );
        LF( 1 );
        INFORME_DIFERENCIAS_CUERPO( ST( S_TOTAL ), total, 0.0 );
        /*----------------------------- pie -------------------------------------*/
        PRN_FIN_TICKET();
    //}
    setData( ACTION_DOPRINT, NULL, 0 );
}
/*****************************************************************************/
void INFORME_DIFERENCIAS_SUBMEDIOS_CONTINUO()
/*****************************************************************************/
{    
    int i = 0;
    SET_MEMORY_CHAR( __ram_imprimir_cabecera, ( char )2 );
    for( i = 1 ;i >= 0 ;i-- ) {
        calcHojas = i;  //cuando i es 1 indica que tiene que calcular las hojas
        IMPRIMIR_TKT_CABECERA_DIF_SUB();
        IMPRIMIR_TKT_INF_DIFERENCIAS_SUBMEDIOS_CONTINUO();
        cantTotalCopias = cantCopias;
        cantCopias = 0;
    }
    if( MODO_NEGOCIO == SERVICIOS ) {
        char pagina[31];
        //Imprime numero de hoja
        cantCopias++;
        sprintf( pagina, "pagina %d de %d", cantCopias + 1, cantTotalCopias + 1 );         
        setData( ACTION_SET_POSITION, "150#235", 0 );
        IMPRIMIR_CADENA( 0, pagina );
    }
    cantCopias = 0;
    setData( ACTION_DOPRINT, NULL, 0 );
}
/*****************************************************************************
void INFORME_DIFERENCIAS_DISC( double *dif, int cierre )
/*****************************************************************************
{
    double    total = 0;
    int       h, j;
    if( mem_mutuales == NULL || mem_t_compra == NULL || mem_tarjetas == NULL ) {
        GRABAR_LOG_SISTEMA( "ERROR PARAM NULL" );
        //      exit(0);
    }
     /*---------------------------- cabecera ---------------------------------*
    IMPRIME_CABECERA_TICKET(  );
    IMPRIMIR_USUARIO_Y_SUPERVISOR(  );
    MOSTRAR_CADENA( 15, PRN, ST( S_DIFERENCIAS ) );
    LF( 1 );
    if( cierre )
        MOSTRAR_CADENA( 13, PRN, ST( S_CIERRE_DE_CAJA ) );
    else
        MOSTRAR_CADENA( 13, PRN, ST( S_ARQUEO_DE_CAJA ) );
    IMPRIME_NRO_INFORME( 0 );
    LF_RECEIPT( 1 );
     /*--------------------------- cuerpo -----------------------------------*
    for( h = 0; h < 9; h++ ) {
        if( medios[h].nro ) {
            INFORME_DIFERENCIAS_CUERPO( medios[h].nombre, dif[h], 0.0 );
            if( medios[h].mutual ) {
                for( j = 0; j < cant_mem_mutuales && mem_mutuales != NULL; j++ ) {
                    INFORME_DIFERENCIAS_DISC_CUERPO( mem_mutuales[j].desc,
                        mem_mutuales[j].monto_retiro - mem_mutuales[j].monto );
                }
            } else if( medios[h].ticket_compra ) {
                for( j = 0; j < cant_mem_t_compra && mem_t_compra != NULL; j++ ) {
                    INFORME_DIFERENCIAS_DISC_CUERPO( mem_t_compra[j].desc,
                        mem_t_compra[j].monto_retiro - mem_t_compra[j].monto );
                }
            } else if( medios[h].tarjeta ) {
                for( j = 0; j < cant_mem_tarjetas && mem_tarjetas != NULL; j++ ) {
                    INFORME_DIFERENCIAS_DISC_CUERPO( mem_tarjetas[j].desc,
                        mem_tarjetas[j].monto_retiro - mem_tarjetas[j].monto );
                }
            }
            total += dif[h];
        }
    }
    MOSTRAR_CADENA( 3, PRN, "---------------------------" );
    LF( 1 );
    INFORME_DIFERENCIAS_CUERPO( ST( S_TOTAL ), total, 0.0 );
     /*----------------------------- pie -------------------------------------*
    PRN_FIN_TICKET(  );
}*/
/*****************************************************************************/
void INFORME_DIFERENCIAS_CUERPO( char *nombre, double importe, double importe_me )
/*****************************************************************************/
{
    MOSTRAR_CADENA( 3, PRN, nombre );
    if( DISCRIMINAR_SUBMEDIOS_CIERRE_CAJ ) {
        MOSTRAR_FLOTANTE( 22, PRN, ENTEROS + 1, DECIMALES, 1, 0, importe );
    }
    else {
        MOSTRAR_FLOTANTE( 16, PRN, ENTEROS + 1, DECIMALES, 1, 0, importe );
    }
    LF( 1 );
    if( importe_me ) {
        MOSTRAR_CADENA( 5, PRN, "[" );
        MOSTRAR_CADENA( 7, PRN, medios[_medio_moneda_extranjera].simbolo_moneda );
        if( DISCRIMINAR_SUBMEDIOS_CIERRE_CAJ ) {
            MOSTRAR_FLOTANTE( 22, PRN, ENTEROS + 1, DECIMALES, 1, 0, importe_me );
            MOSTRAR_CADENA( 32, PRN, "]" );
        }
        else {
            MOSTRAR_FLOTANTE( 16, PRN, ENTEROS + 1, DECIMALES, 1, 0, importe_me );
            MOSTRAR_CADENA( 26, PRN, "]" );
        }
        LF( 1 );
    }
}
/*****************************************************************************/
void INFORME_DIFERENCIAS_DISC_CUERPO( char *nombre, double importe )
/*****************************************************************************/
{
    MOSTRAR_CADENA( 5, PRN, nombre );
    MOSTRAR_FLOTANTE( 16, PRN, ENTEROS + 1, DECIMALES, 1, 0, importe );
    LF( 1 );
}
/*****************************************************************************/
void INFORME_DIFERENCIAS_SUBMEDIOS( double *cierre, double *tot, int en_cierre, double *cierre_me,
                                    double *tot_me )
/*****************************************************************************/
{
    int h,j;
    double monto_caja,monto_retiro;
    struct rendicion_en_memoria *rend_tmp;
    /*if( mem_mutuales == NULL || mem_t_compra == NULL || mem_tarjetas == NULL ) {
        GRABAR_LOG_SISTEMA( "ERROR PARAM NULL" );
        //      exit(0);
    } */
    /*---------------------------- cabecera ---------------------------------*/
    IMPRIME_CABECERA_TICKET( SI );
    IMPRIMIR_USUARIO_Y_SUPERVISOR();
    PRN_LETRA_GRANDE();
    MOSTRAR_CADENA( 2, PRN, ST( S_DIFERENCIAS_POR_SUBMEDIOS ) );
    LF( 1 );
    if( en_cierre ) {
        MOSTRAR_CADENA( 4, PRN, ST( S_CIERRE_DE_CAJA ) );
    }
    else {
        MOSTRAR_CADENA( 4, PRN, ST( S_ARQUEO_DE_CAJA ) );
    }
    PRN_LETRA_NORMAL();
    IMPRIME_NRO_INFORME( 0 );
    LF_RECEIPT( 1 );
    LF( 1 );
    MOSTRAR_CADENA( 18, PRN, ST( S_INFORMADO ) );
    MOSTRAR_CADENA( 29, PRN, ST( S_DIFERENCIAS ) );
    LF( 1 );
    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
    /*--------------------------- cuerpo -----------------------------------*/
    for( h = 0;h < 9;h++ ) {
        if( medios[h].nro ) {
            if( medios[h].mutual || medios[h].ticket_compra || medios[h].tarjeta ) {
                INFORME_DIFERENCIAS_SUBMEDIO_CUERPO( medios[h].nombre, NULL, tot[h], cierre[h],
                                                     tot_me[h], cierre_me[h] );
                for( j = 0;j < cant_mem_rendicion_medio[h] /*&& mem_mutuales != NULL*/;j++ ) {
                    rend_tmp = RENDICION_MEDIO( h, j );
                    if((rend_tmp->monto != 0) || (rend_tmp->monto_retiro != 0)){
                        INFORME_DIFERENCIAS_SUBMEDIO_CUERPO( medios[h].nombre, rend_tmp->desc,
                                                             rend_tmp->monto, rend_tmp->monto_retiro,
                                                             0.0, 0.0 );
                    }
                }
            }
            else {
                INFORME_DIFERENCIAS_SUBMEDIO_CUERPO( medios[h].nombre, NULL, tot[h], cierre[h],
                                                     tot_me[h], cierre_me[h] );
            }
        }
    }
    monto_caja = 0;
    monto_retiro = 0;
    for( h = 0;h < 9;h++ ) {
        monto_caja += tot[h];
        monto_retiro += cierre[h];
    }
    MOSTRAR_CADENA( 3, PRN, "-------------------------------------" );
    LF( 1 );
    INFORME_DIFERENCIAS_SUBMEDIO_CUERPO( ST( S_TOTAL ), NULL, monto_caja, monto_retiro, 0.0, 0.0 );
    /*----------------------------- pie -------------------------------------*/
    PRN_FIN_TICKET();
}
/*****************************************************************************/
void INFORME_DIFERENCIAS_SUBMEDIO_CUERPO( char *medio, char *submedio, double monto_caja,
                                          double monto_retiro, double monto_caja_me,
                                          double monto_retiro_me )
/*****************************************************************************/
{
    char desc[100];
    TRIM( medio );
    if( submedio != NULL ) {
        TRIM( submedio );
        sprintf( desc, " *%s", submedio );
    }
    else {
        strcpy( desc, medio );
    }
    MOSTRAR_CADENA( 1, PRN, desc );
    MOSTRAR_FLOTANTE( 16, PRN, ENTEROS + 1, DECIMALES, 1, 0, monto_retiro );
    MOSTRAR_FLOTANTE( 29, PRN, ENTEROS + 1, DECIMALES, 1, 0, ( monto_retiro - monto_caja ) );
    LF( 1 );
    /*if( monto_retiro - monto_caja ) {
        if( monto_retiro - monto_caja > 0 ) {
            MOSTRAR_CADENA( 3, PRN, "******" );
            MOSTRAR_CADENA( 10, PRN, ST( S_SOBRANTE ) );
            MOSTRAR_FLOTANTE( 19, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              fabs( monto_retiro - monto_caja ) );
        }
        else {
            MOSTRAR_CADENA( 3, PRN, "******" );
            MOSTRAR_CADENA( 10, PRN, ST( S_FALTANTE ) );
            MOSTRAR_FLOTANTE( 30, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              fabs( monto_retiro - monto_caja ) );
        }
        LF( 1 );
    }*/
    if( monto_caja_me || monto_retiro_me ) {
        MOSTRAR_CADENA( 3, PRN, "[" );
        MOSTRAR_CADENA( 5, PRN, medios[_medio_moneda_extranjera].simbolo_moneda );
        MOSTRAR_FLOTANTE( 16, PRN, ENTEROS + 1, DECIMALES, 1, 0, monto_retiro_me );
        MOSTRAR_FLOTANTE( 29, PRN, ENTEROS + 1, DECIMALES, 1, 0, monto_retiro_me - monto_caja_me );
        MOSTRAR_CADENA( 40, PRN, "]" );
        LF( 1 );
        /*if( monto_retiro_me - monto_caja_me ) {
            MOSTRAR_CADENA( 3, PRN, "[" );
            if( monto_retiro_me - monto_caja_me > 0 ) {
                MOSTRAR_CADENA( 5, PRN, ST( S_SOBRANTE ) );
                MOSTRAR_FLOTANTE( 11, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                                  fabs( monto_retiro_me - monto_caja_me ) );
            }
            else {
                MOSTRAR_CADENA( 5, PRN, ST( S_FALTANTE ) );
                MOSTRAR_FLOTANTE( 11, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                                  fabs( monto_retiro_me - monto_caja_me ) );
            }
            MOSTRAR_CADENA( 22, PRN, "]" );
            LF( 1 );
        }*/
    }
    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}
/*****************************************************************************/
void INFORME_DIFERENCIAS_SUBMEDIOS_RETAIL( char tipo_cierre )
/*****************************************************************************/
{
    int i = 1, j = 0 , k = 0, tipo_evento = _EVENT_INFORME, nro_evento = 1;
    long cant_eventos,id_evento_nro,ultimo_id;
    double imp_declarado_temp = 0, imp_real_temp = 0;
    int tipo_pagos_local;
    /*  struct _dif_submedios
        {   
            char nombre_medio[30];
            char cod_medio;
            int  cod_submedio[30];
            char nombre_submedio[30][30];
            double  imp_declarado[30];
            double  imp_real[30];
        } dif_submedios[10];
    */
    //struct _ev_medios event_pago; 
    struct _tarjetas tarjeta_sic;
    struct _t_mutuales mutuales_sic;
    struct _t_compra  t_compra_sic;
    for( i = 0 ;i < 10 ;i++ ) {
        for( j = 0 ;j < 30 ;j++ ) {
            dif_submedios[i].imp_declarado[j] = 0;
            dif_submedios[i].imp_real[j] = 0;
        }
    }

    /*------ Se copia el nombre de cada medio a dif_submedios -----*/
    for( i = 0 ;i < 10 ;i++ ) {
        dif_submedios[i].cod_medio = medios[i].nro;
        sprintf( dif_submedios[i].nombre_medio, "%s", medios[i].nombre );
    }

    /*---- Se copia el nombre de cada submedio ---*/
	for( i = 0 ;i < 10 ;i++ ) {
        j = 0;
        if( medios[i].tarjeta ) {
            if( !/*USED( AREA_AUX2 )*/OPENED( T_TARJETAS, TT_ORIG ) ) {
                /*USE_DB( AREA_AUX2, _tarjetas, ( char* )&tarjeta_sic,
                         sizeof( struct _tarjetas ), NULL, 0, 0 );
                SELECT( AREA_AUX2 );*/
				OPEN_TABLE(T_TARJETAS, TT_ORIG,( char* )&tarjeta_sic,sizeof( struct _tarjetas ));
            }
			SELECT_TABLE( T_TARJETAS, TT_ORIG );
            //GO( TOP );
            //SET_ORDER( 1 );
			SET_WHERE("");
			SET_ORDER2("COD_TARJETA");
			RUN_QUERY(NO);
            while( !dbEOF() ) {
                dif_submedios[i].cod_submedio[j] = tarjeta_sic.cod_tarjeta;
                sprintf( dif_submedios[i].nombre_submedio[j], "%s", tarjeta_sic.nombre );
                j++;
                SKIP2( 1 );
            }
            if( /*USED( AREA_AUX2 )*/OPENED( T_TARJETAS, TT_ORIG ) ) {
                CLOSE_TABLE( T_TARJETAS, TT_ORIG );
            }
        }
        else if( medios[i].ticket_compra ) {
			if( !/*USED( AREA_AUX2 )*/OPENED( T_COMPRA, TT_ORIG ) ) {
				/*USE_DB( AREA_AUX2, _t_compra , ( char* )&t_compra_sic,
                sizeof( struct _t_compra  ), NULL, 0, 0 );
            	SELECT( AREA_AUX2 );*/
				OPEN_TABLE(T_COMPRA, TT_ORIG,( char* )&t_compra_sic,sizeof( struct _t_compra  ));
            }
			SELECT_TABLE( T_COMPRA, TT_ORIG );
			SET_WHERE("");
            SET_ORDER2( "COD_T_COMPRA" );
			RUN_QUERY(NO);
            while( !dbEOF() ) {
                if( t_compra_sic.cod_medio_padre == medios[i].nro ) {
                    dif_submedios[i].cod_submedio[j] = t_compra_sic.cod_t_compra;
                    sprintf( dif_submedios[i].nombre_submedio[j], "%s", t_compra_sic.des_t_compra );
                    j++;
                }
                SKIP2( 1 );
            }
			if( OPENED( T_COMPRA, TT_ORIG ) ) {
				CLOSE_TABLE( T_COMPRA, TT_ORIG );
            }
        }
		else if( medios[i].mutual ) {
			if( !OPENED( T_MUTUALES, TT_ORIG ) ) {
				OPEN_TABLE( T_MUTUALES, TT_ORIG,( char* )&mutuales_sic,sizeof( struct _t_mutuales ));
            }
			SELECT_TABLE( T_MUTUALES, TT_ORIG );
			SET_WHERE("");
			RUN_QUERY(NO);
            while( !dbEOF() ) {
				if( mutuales_sic.cod_medio_padre == medios[i].nro ) {
					dif_submedios[i].cod_submedio[j] = mutuales_sic.cod_mutual;
                    //_snprintf( dif_submedios[i].nombre_submedio[j],sizeof( dif_submedios[i].nombre_submedio[j] ) - 1, "%s", mutuales_sic.des_mutual );
					sprintf( dif_submedios[i].nombre_submedio[j], "%s", mutuales_sic.des_mutual );
                    j++;
                }
                SKIP2( 1 );
			}
			if( OPENED( T_MUTUALES, TT_ORIG ) ) {
				CLOSE_TABLE( T_MUTUALES, TT_ORIG );
            }
        }
    }
    switch( tipo_cierre ) {
        case 'Z':
            tipo_pagos_local = TT_ORIG;
            nro_evento = 1;
            cant_eventos = CANTIDAD_EVENTOS( tipo_evento, NO );
            id_evento_nro = ID_EVENTO_NRO( nro_evento, tipo_evento );
            if( _IMPRIMIENDO_Z_ANT == 1 ) {
                //if( !USED( AREA_AUX ) ) {
                //USE_DB( AREA_AUX, _PAGO_BAK2, ( char* )&event_ticket, sizeof( struct _eventos ), NULL, 0, 0 );
                //SELECT( AREA_AUX );
                //(Nota Unific MySQL-Linux): notar que se estaba usando event_ticket para almacenar pago.
                OPEN_TABLE( T_PAGOS, TT_BAK, ( char* )&event_pago, sizeof( struct _ev_medios ) );
                tipo_pagos_local = TT_BAK;
                //}
            }
            else {
                /*USE_DB( AREA_AUX, _PAGO_BTR, ( char* )&event_pago, sizeof( struct _ev_medios ),
                        NULL, 0, 0 );
                SELECT( AREA_AUX );*/
				OPEN_TABLE(T_PAGOS, TT_ORIG,( char* )&event_pago, sizeof( event_pago ));
                tipo_pagos_local = TT_ORIG;
            }
			SELECT_TABLE( T_PAGOS, tipo_pagos_local );
            //GO( TOP );
            //SET_ORDER( 1 );
			SET_WHERE("ID_EVENTO = '%ld'",id_evento_nro);
			SET_ORDER2("ID_EVENTO");
			RUN_QUERY(NO);
            for( k = 1 ;k <= cant_eventos ;k++ ) {
                while( !dbEOF() ) {
                    if( event_pago.id_evento == id_evento_nro ) {
						for( j = 0 ;j < 30 ;j ++ ) {
                            i = event_pago.modo_pago;
                            if( event_pago.cod_sub_medio
                             == ( dif_submedios[i - 1].cod_submedio[j] ) ) {
                                imp_declarado_temp = dif_submedios[i - 1].imp_declarado[j];
                                imp_real_temp = dif_submedios[i - 1].imp_real[j];
                                dif_submedios[i - 1].imp_declarado[j] = imp_declarado_temp
                                                                      + event_pago.importe;
                                dif_submedios[i - 1].imp_real[j] = imp_real_temp
                                                                 + event_pago.imp_pago;
                            }
                        }
                    }
                    SKIP2( 1 );
                }
                nro_evento++;
                id_evento_nro = ID_EVENTO_NRO( nro_evento, tipo_evento );
                //if( USED( AREA_AUX ) ) {
                    //CLOSE_DATABASE( AREA_AUX );
                //}
                //if( !/*USED( AREA_AUX )*/OPENED( T_PAGOS, TT_ORIG ) ) {
                    /*USE_DB( AREA_AUX, _PAGO_BTR, ( char* )&event_pago, sizeof( struct _ev_medios ),
                            NULL, 0, 0 );
                    SELECT( AREA_AUX );*/
					OPEN_TABLE(T_PAGOS, TT_ORIG,( char* )&event_pago, sizeof( event_pago ));
					SELECT_TABLE( T_PAGOS, TT_ORIG );
                //}
                /*GO( TOP );
                SET_ORDER( 1 );*/
				SET_WHERE("");
				RUN_QUERY(NO);
            }

            //(Nota Unific MySQL-Linux): no se debe cerrar pagos.
            //if( USED( AREA_AUX ) ) {
                //CLOSE_DATABASE( AREA_AUX );
            //}
            break;
        case 'Y':
            ultimo_id = ULTIMO_ID_EVENTO( tipo_evento );
            if( !/*USED( AREA_AUX )*/OPENED( T_PAGOS, TT_ORIG ) ) {
                /*USE_DB( AREA_AUX, _PAGO_BTR, ( char* )&event_pago, sizeof( struct _ev_medios ),
                        NULL, 0, 0 );
                SELECT( AREA_AUX );*/
				OPEN_TABLE(T_PAGOS, TT_ORIG,( char* )&event_pago, sizeof( event_pago ));
            }
            SELECT_TABLE( T_PAGOS, TT_ORIG );            
            /*GO( TOP );
            SET_ORDER( 1 );*/
			SET_WHERE("ID_EVENTO = '%ld'",ultimo_id);
			SET_ORDER2("ID_EVENTO");
			RUN_QUERY(NO);
            while( !dbEOF() ) {
                if( event_pago.id_evento == ultimo_id ) {
                    j = event_pago.modo_pago;
                    for( i = 0 ;i < 30 ;i ++ ) {
                        if( event_pago.cod_sub_medio == ( dif_submedios[j - 1].cod_submedio[i] ) ) {
                            dif_submedios[j - 1].imp_declarado[i] = event_pago.importe;
                            dif_submedios[j - 1].imp_real[i] = event_pago.imp_pago;
                        }
                    }
                }
                SKIP2( 1 );
            }
            //if( USED( AREA_AUX ) ) {
            //    CLOSE_DATABASE( AREA_AUX );
            //}
            break;
        default:
            break;
    }
    if( !IMPRIMIR_CABECERA_AL_FINAL ) {
        IMPRIMIR_TKT_CABECERA( 1 );
    }
    //IMPRIMIR_USUARIO_Y_SUPERVISOR();
    IMPRIMIR_TKT_CABECERA_DIF_SUB();
    IMPRIMIR_TKT_INF_DIFERENCIAS_SUBMEDIOS_CONTINUO();
    setData( ACTION_PRINT, NULL, 0 );
    TKT_CORTE_PAPEL();
}           

