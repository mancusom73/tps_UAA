#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <bancos.h>
#include <b_mens.h> 
#include <comprob.h> 
#include <cr.h> 
#include <cr_displ.h> 
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <ftoc.h> 
#include <getch.h> 
#include <lista.h> 
#include <log.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <m_entero.h> 
#include <pagos.h> 
#include <pago_mut.h> 
#include <pago_tc.h> 
#include <pant.h> 
#include <pan_pago.h> 
#include <promo.h> 
#include <tarjetas.h> 
#include <tkt.h> 
#include <trim.h> 
#include <_cr1.h>
#include <db.h>
#include <deftables.h>
#include <cmemory.h>
#include <tarjetas.h>
#include <pagos.h>
#include <round.h>
#include <motor.h>
#include <path.h>
#include <recupero.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#pragma pack(1)
//#include <CR_VAR.h>
#include <cr.h>
#include <config.h>
#include <aarch.h> 
#include <stdlib.h> 
#include <ctype.h>
#include <math.h>
#include <string.h>
#if defined(INVEL_W) || defined(INVEL_L)
#include <actions.h>
#include <typeacti.h>
#endif
void PAGOS_MOSTRAR_RESTO( double resto );
void MOSTRAR_PAGO( int h );
void MOSTRAR_ENVIAR_OFFLINE();
static int disc_iva_on = 0;
static int autoriza_on = 0;
static int env_offline_on = 0;
static int retencion_dgr_on = 0;
static int percepcion_212_on = 0;
static int percepcion_dgi_on = 0;
static int diferimiento_on = 0;
//extern void ENMASCARA_NUMERO_TARJETA( int marca, char *nro_cadena, char *temp );
extern double MUTUALES_ML_A_ME( double importe_me, int h );
extern void agregarVuelto( char *s, char *s1, char *s2 );
extern int MUTUAL_MONEDA_EXTRANJERA( int h );
struct _t_mutuales mutual;
struct _ind_vuelto
{
    char cod_medio;
    char cod_submedio;
};
void MENSAJE( char *cadena );
/*****************************************************************************/
void PAGOS_CONFECCIONA_PANTALLA( int modo, int dev_cobro )
/*****************************************************************************/
{
    int h;
    char tt[30];
    if( !ancho_panel ) {

        /*------------ muestra medios de pago ------------*/
        ExecuteSSAction( A_MUESTRA_DATOS, T_FLAGS_MEDIO_EXTR_BORRAR, " ", " ", NULL, " " );     
        ExecuteAction( A_BORRAR_MEDIOS );
        for( h = 0;h < 9;h++ ) {
            if( medios[h].nro && ( MEDIO_DE_PAGO_HABILITADO( modo, h, dev_cobro ) ||
						MEDIO_DE_PAGO_HABILITADO_MARGINAL( h, dev_cobro ) ) ) {
                #ifdef ICONO_MEDIO
                memset( tt, ' ', sizeof( tt ) );
                //bam
                strcpy( tt /*+ 10 - strlen( medios[h].nombre )*/, medios[h].nombre );
                switch( medios[h].nro ) {
                    case MEDIO_1:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_1_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_2:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_2_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_3:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_3_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_4:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_4_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_5:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_5_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_6:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_6_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_7:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_7_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_8:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_8_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_9:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_9_AGREGAR, " ", tt, NULL, " " );
                        break;
                    case MEDIO_10:
                        ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_10_AGREGAR, " ", tt, NULL, " " );
                        break;
                    default:
                        GRABAR_LOG_SISTEMA( "MEDIO NO DEFINIDO" ,LOG_PANTALLAS,2);
                }
                #endif
                MOSTRAR_ENTERO( 7, 9 + h, "9", medios[h].nro );
                MOSTRAR_CADENA( 9, 9 + h, medios[h].nombre );
            }
        }
    }
    /*------------ Muestra datos ------------*/
    for( h = 0;h < RAM_P_PAGO;h++ ) {
        MOSTRAR_PAGO( h );
    }
    //ExecuteAction( A_REFRESH_PAGOS );
    /*----------------------- muestra cuotas de tarjeta ----------------------*/
    CR1_MUESTRA_TARJETA();
    MUESTRA_MUTUAL();
    MOSTRAR_BONOS();
    #ifdef COMPILAR_COMPROBANTES
    MOSTRAR_NRO_FACTURA();
    #endif
    MOSTRAR_ENVIAR_OFFLINE();
}
/*****************************************************************************/
void PAGOS_MUESTRA_TOTAL( double resto )
/*****************************************************************************/
{
     double total=0.0,total_aux=0.0;
    char aux[15];
    #if defined(INVEL_W) || defined(INVEL_L)
    char ttt[20];
	memset( ttt, 0, sizeof( ttt ) );
    #endif
 	memset( aux, 0, sizeof( aux ) );
    total = PAGOS_TOTAL_A_PAGAR();
    _ATRIBUTO = 112;
    strncpy( aux, ( resto <= 0 ) ? ST( S_VUELTO ) : ST( S_TOTAL_ ), 15 );
    MOSTRAR_GRAN_CADENA( 23 - ancho_panel / 2, 3, aux );
    if( ( config.pais == CHILE || config.pais == BRASIL ) && total > 9999999.99 ) {
        total_aux = ( resto <= 0 ) ? ( ( RAM_RESTO_ME ) ? fabs( RAM_RESTO_ME ) : fabs( resto ) )
                  : total;
        MOSTRAR_GRAN_FLOTANTE( 40 - ancho_panel / 2, 3, ENTEROS + 3, DECIMALES, 0, 0, total_aux );
    }
    else {
        total_aux = ( resto <= 0 ) ? ( ( RAM_RESTO_ME ) ? fabs( RAM_RESTO_ME ) : fabs( resto ) )
                  : total;
        MOSTRAR_GRAN_FLOTANTE( 40 - ancho_panel / 2, 3, ENTEROS + 1, DECIMALES, 0, 0, total_aux );
    }
    #if defined(INVEL_W) || defined(INVEL_L)
    FTOC( total_aux, ttt, ENTEROS + 1, DECIMALES, 1, 1 );
    ExecuteSAction( A_PAGO_MUESTRA_TOTAL, ttt );
    ExecuteSAction( A_MUESTRA_TIT_TOTAL_PAGOS, aux );

    #endif
    PAGOS_MOSTRAR_RESTO( resto );
    /*---------------------- Muestra el total en el display ------------------*/
    #ifdef COMPILAR_DISPLAY
    MOSTRAR_EN_DISPLAY( ST( S_TOTAL_A_PAGAR ), total );
    #endif
}
/*****************************************************************************/
void MOSTRAR_DISCRIMINACION_IVA()
/*****************************************************************************/
{
    char cad[20];
    if( RAM_DISCRIMINAR && !disc_iva_on ) {
        strcpy( cad, "DISCR. IVA" );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_DISC_IVA_AGREGAR, cad, " ", NULL, " " );
        disc_iva_on = 1;
    }
    else if( !RAM_DISCRIMINAR && disc_iva_on ) {
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_DISC_IVA_BORRAR, " ", " ", NULL, " " );
        disc_iva_on = 0;
    }
}
/*****************************************************************************/
void MOSTRAR_MARCA_AUTORIZACION()
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    char cad[20];
    if( RAM_SOLICITAR_AUTORIZACION && !autoriza_on ) {
        strcpy( cad, "AUTORIZA" );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_AUTORIZACION_AGREGAR, cad, " ", NULL, " " );
        autoriza_on = 1;
    }
    else if( !RAM_SOLICITAR_AUTORIZACION && autoriza_on ) {
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_AUTORIZACION_BORRAR, " ", " ", NULL, " " );
        autoriza_on = 0;
    }
    #else
    if( !RAM_SOLICITAR_AUTORIZACION ) {
        _ATRIBUTO = 7;
        MOSTRAR_CADENA( 1, 24, "   " );
    }
    else {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 1, 24, "AUT" );
    }
    #endif
}
/***********************************************************************/
int ELEGIR_VUELTO( int grilla, int cant, struct _ind_vuelto *indice )
/***********************************************************************/
{
    int seguir = 1, k, rta = 0, cont = 1;
    //SELECT( AREA_VUELTOS );
	SELECT_TABLE( T_VUELTOS, TT_ORIG );
    //SET_ORDER( 1 );
    //GO( TOP );
    //GET_GREATER_E( ( char* )indice );
	SET_WHERE("COD_MEDIO='%i' AND COD_SUBMEDIO='%i'", indice->cod_medio, indice->cod_submedio );
	RUN_QUERY( NO );
    ExecuteSAction( grilla, "0" );            // 0 habilita
    while( seguir ) {
        k = GETCH();
        switch( k ) {
            case 13:
                /*-------- ENTER ---------*/ 
                seguir = 0;
                ExecuteSAction( grilla, "-99" ); //-99 deshabilita 
                rta = 1;
                break;
            case -72:
                /*----- FLECHA ARRIBA -----*/
                ExecuteSAction( grilla, "-1" );
                if( cont > 1 ) {
                    SKIP2( -1 );
                    rta = 1;
                    cont--;
                }
                break;
            case -80:
                /*------ FLECHA ABAJO -----*/
                ExecuteSAction( grilla, "1" );                
                if( cont < cant ) {
                    SKIP2( 1 );
                    rta = 1;
                    cont++;
                }
                break;
        }
    }
    return ( rta );
}
/***********************************************************************/
double	  CALCULAR_VUELTO(char cod_mutual)
/***********************************************************************/
{
	//struct _t_mutuales mutual;
	double vuelto;
    //int area;
	int tabla_ant = SELECTED2();
	int tipo_ant = SELECTED_TIPO();
	
    //area = SELECTED();
	//if( USE_DB( AREA_AUX3, _mutuales, ( char* )&mutual, sizeof( struct _mutuales ), NULL, 0, 0 ) == 0 ) {
	if( OPEN_TABLE( T_MUTUALES, TT_ORIG, ( char* )&mutual, sizeof( struct _t_mutuales ) ) == 0 ) {
        //GET_EQUAL( &cod_mutual );
		SET_WHERE("COD_MUTUAL='%d'", cod_mutual);
		RUN_QUERY(NO);

		if( FOUND2() ) {
			vuelto = _PAGO_VUELTO_MEDIO(RAM_P_PAGO)/mutual.cotizacion; 
		}
		else {
			vuelto = _PAGO_VUELTO_MEDIO(RAM_P_PAGO);
		}
	}
	else {
		vuelto = _PAGO_VUELTO_MEDIO(RAM_P_PAGO);
	}

   //CLOSE_DATABASE( AREA_AUX3 );
	CLOSE_TABLE( T_MUTUALES, TT_ORIG );
   //SELECT( area );
	SELECT_TABLE( tabla_ant, tipo_ant );

	return ROUND( vuelto, ENTEROS, DECIMALES );
}
/***********************************************************************/
int MOSTRAR_OPCIONES_VUELTO( char cod_medio, char cod_submedio, int *cod_medio_vuelto,
                             int *cod_submedio_vuelto )
/***********************************************************************/
{
    struct _ind_vuelto ind_vuelto;
    char vuelto_aux[20];
    char cod_aux[10];
	char vuelto[10];
	int cnt = 0, cant=0, rta = 0;
	int area_ant, tipo_ant;
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    
	//if( !USED( AREA_VUELTOS ) ) {
	if( !OPENED( T_VUELTOS, TT_ORIG ) ) {
       // USE_DB( AREA_VUELTOS, _VUELTOS, ( char* )&vueltos_sic, sizeof( struct _vueltos_x_medio ), NULL,
       //         0, 0 );
		OPEN_TABLE( T_VUELTOS, TT_ORIG, ( char* )&vueltos_sic, sizeof( struct _vueltos_x_medio ) ); 	
    }
    
	ind_vuelto.cod_medio = cod_medio + 1;
    ind_vuelto.cod_submedio = cod_submedio;
    //SELECT( AREA_VUELTOS );
    //SET_ORDER( 1 );
    //GET_EQUAL( ( char* )&ind_vuelto );
	SELECT_TABLE( T_VUELTOS, TT_ORIG );
	SET_ORDER2( "COD_MEDIO, COD_SUBMEDIO" );
	SET_WHERE("COD_MEDIO = '%d' and COD_SUBMEDIO = '%d'", ind_vuelto.cod_medio, ind_vuelto.cod_submedio );
	RUN_QUERY(NO);
    
	cant = RECCOUNT2(NO, NULL);
	//Llenamos la grilla del panel vueltos 
	while( !dbEOF() && !dbBOF() && vueltos_sic.cod_medio == ind_vuelto.cod_medio && vueltos_sic.cod_submedio == ind_vuelto.cod_submedio )  // llena la grilla del panel vueltos 
	{
	   _snprintf( cod_aux, sizeof(cod_aux), "%d - %d", vueltos_sic.cod_medio_vuelto, vueltos_sic.cod_submedio_vuelto  );	   
	   _snprintf( vuelto_aux, sizeof(vuelto_aux), "%s", PAGOS_OBTENER_NOMBRE( vueltos_sic.cod_medio_vuelto, vueltos_sic.cod_submedio_vuelto ) );
	   _snprintf( vuelto, sizeof(vuelto), "%.2lf", CALCULAR_VUELTO(vueltos_sic.cod_submedio_vuelto));
	   agregarVuelto( cod_aux, vuelto_aux, vuelto);
	   *cod_medio_vuelto = vueltos_sic.cod_medio_vuelto;//si hay un solo vuelto para ese medio, no se muestra el menu
	   *cod_submedio_vuelto = vueltos_sic.cod_submedio_vuelto;
       cnt++;
	   SKIP2( 1 );
    }
    if( cnt ) {
        ExecuteAction( A_ACTIVAR_PANEL_VUELTO );
    }
    if( cnt > 0 ) {
		if( cnt == 1 ) {
			//si hay un solo vuelto para ese medio, no se muestra el menu
			rta = 1;
		} else {
            rta = ELEGIR_VUELTO( A_SCROLL_VUELTO, cnt, &ind_vuelto );
            if( rta ) {
                *cod_medio_vuelto = vueltos_sic.cod_medio_vuelto;
                *cod_submedio_vuelto = vueltos_sic.cod_submedio_vuelto;
            }
        }
    } else {
        rta = 0;
    }
    ExecuteAction( A_DESACTIVAR_PANEL_VUELTO );
    //CLOSE_DATABASE( AREA_VUELTOS );
	CLOSE_TABLE( T_VUELTOS, TT_ORIG );

	SELECT_TABLE( area_ant, tipo_ant );

    return rta;
}

///*****************************************************************************
//void PAGOS_MOSTRAR_RESTO( double resto )
///*****************************************************************************
//{
//    char *p_nombre;
//    int h, cod_medio;
//    double vuelto,v,resto_ml;
//    int ytop = 18, xleft = 5;
//    char tmp[30];
//    char string_resto[40];
//    char medios_aux[10];
//    double vuelto_aux;
//  int cod_submedio_aux, cod_medio_aux;
//    _ATRIBUTO = 7;
//    _ATRIBUTO = 120;
//    
//  if( resto < -0.001 ) {
//      if( _PAGO_TIPO_VUELTO( RAM_P_PAGO - 1 ) == _VUELTO_OTRO_MEDIO ) {
//          MOSTRAR_OPCIONES_VUELTO( _PAGO_MODO( RAM_P_PAGO - 1  ), _PAGO_COD_SUBMEDIO( RAM_P_PAGO - 1 ), &cod_medio_aux, &cod_submedio_aux );
//          sprintf( string_resto, "%s : ", PAGOS_OBTENER_NOMBRE( vueltos_sic.cod_medio_vuelto, vueltos_sic.cod_submedio_vuelto ) );
//      } else {
//          sprintf( string_resto, "%s : ", PAGOS_OBTENER_NOMBRE( _PAGO_MODO( RAM_P_PAGO - 1  ), _PAGO_COD_SUBMEDIO( RAM_P_PAGO - 1 ) ) );
//      }
//      FTOC( vuelto_aux, tmp, ENTEROS + 1, DECIMALES, 0, 0 );
//        strcat( string_resto, tmp );
//        ExecuteSSAction( A_MUESTRA_RESTO, 0, ST( S_VUELTO ), string_resto, NULL );
//    } else {
//        if( RAM_MOSTRAR_FALTANTE_EN_ME ) {
//            resto_ml = resto;
//            resto = PAGOS_ML_A_ME( resto, _medio_moneda_extranjera );
//            if( PAGOS_ME_A_ML( resto, _medio_moneda_extranjera ) < resto_ml ) {
//                resto = resto + 1;
//            }
//            strcpy( medios_aux, medios[_medio_moneda_extranjera].simbolo_moneda );
//        }
//        FTOC( resto, string_resto, ENTEROS + 1, DECIMALES, 0, 1 );
//        ExecuteSSAction( A_MUESTRA_RESTO, 0, ST( S_FALTANTE ), string_resto, NULL );
//    }
//}

/*****************************************************************************/
void PAGOS_MOSTRAR_RESTO( double resto )
/*****************************************************************************/
{
    int h, y = 0;
    double vuelto,v,resto_ml;
    int ytop = 18, xleft = 5;
    char tmp[30];
    char string_resto[40]="";
	char string_resto_me[40]="";
    char medios_aux[20];
	int cod_sub=0, cod_med=0;
    double vuelto_aux = 0.0;
	double resto_me = 0.0;
    memset( medios_aux, 0, sizeof( medios_aux ) );
	memset( tmp, 0 ,sizeof( tmp ));
	

		for( h = 0;h < RAM_P_PAGO;h++ ) 
		{
		_snprintf( medios_aux, sizeof(medios_aux), "%s", PAGOS_OBTENER_NOMBRE( ( char )( _PAGO_MODO( h ) + 1 ), _PAGO_COD_SUBMEDIO( h ) ));
		cod_sub = _PAGO_COD_SUBMEDIO( h );
		cod_med = _PAGO_MODO( h );
		}

    if( resto < -0.001 ) {
        vuelto = -resto;
        for( h = 0;h < RAM_P_PAGO;h++ ) {
            v = _PAGO_VUELTO( h );
            if( v ) {
//                sprintf( medios_aux, "%s",medios[cod_medio].nombre );
				sprintf( medios_aux, "%s", PAGOS_OBTENER_NOMBRE( ( char )( _PAGO_MODO( h ) + 1 ),
                                                 _PAGO_COD_SUBMEDIO( h ) ));
            }
            else {
                v = _PAGO_VUELTO_MEDIO( h );
				if( v ) {
					sprintf( medios_aux, "%s", PAGOS_OBTENER_NOMBRE( ( char )( _PAGO_COD_MEDIO_VUELTO( h ) ),
                                                 _PAGO_COD_SUBMEDIO_VUELTO( h ) ));
				}
            }
            if( v ) {
                y++;
                vuelto -= v;
            }
        }
        if( vuelto > 0 ) {
            if( RAM_RESTO_ME ) {
                strcpy( medios_aux, medios[_medio_moneda_extranjera].nombre );
                vuelto_aux = -RAM_RESTO_ME;
            }
            else {
                strcpy( medios_aux, medios[_medio_efectivo].nombre );
                vuelto_aux = vuelto;
            }
        }
		vuelto_aux = -resto;
		
		if( medios[_PAGO_COD_MEDIO_VUELTO( RAM_P_PAGO - 1 )].mutual && 
			MUTUAL_MONEDA_EXTRANJERA( _PAGO_COD_SUBMEDIO_VUELTO( RAM_P_PAGO - 1 ) ) )
		vuelto_aux = MUTUALES_ML_A_ME( vuelto_aux, _PAGO_CUENTA( RAM_P_PAGO - 1 ) );

        FTOC( vuelto_aux, tmp, ENTEROS + 1, DECIMALES, 0, 0 );
        _snprintf( string_resto,sizeof(string_resto), "%s : %.2lf", medios_aux, vuelto_aux);
        resto = -resto;
		if(resto!= vuelto_aux)
			_snprintf( string_resto_me, sizeof(string_resto_me), "%s : %.2lf", PAGOS_OBTENER_NOMBRE( (char)1, (char)0 ), resto);
        ExecuteSSSAction( A_MUESTRA_RESTO, ST( S_VUELTO ), string_resto, string_resto_me);
    }
    else {
        if( RAM_MOSTRAR_FALTANTE_EN_ME ) {
            resto_ml = resto;
            resto_me  = PAGOS_ML_A_ME( resto, _medio_moneda_extranjera );
			/*if( PAGOS_ME_A_ML( resto_me, _medio_moneda_extranjera ) < resto_ml ) {
				resto = resto_me + 1;
			}//??? */
			_snprintf(string_resto_me,sizeof(string_resto_me) - 1, "%s : %.2lf", medios[_medio_moneda_extranjera].simbolo_moneda, resto_me );
		} else {
			//valores muy pequeños que vienen en negativo y visualizan el cero en negativo
			if( fabs( resto ) < 0.01 )
				resto = 0;
			FTOC( resto, string_resto, ENTEROS + 1, DECIMALES, 0, 1 );
			if( medios[cod_med].moneda_extranjera && !MUTUAL_MONEDA_EXTRANJERA( cod_sub ) ) {            
				resto_me = PAGOS_ML_A_ME( resto, cod_med );
			} else {

 				resto_me = CALCULAR_RESTO( medios[cod_med].moneda_extranjera, cod_sub, resto );
			}
			if(cod_med!=0 )//Si es distinto de Efectivo muestro el fantante en efectivo y en el otro medio seleccionado
				_snprintf(string_resto_me,sizeof(string_resto_me), "%s : %.2lf", medios_aux, resto_me);
		}
		_snprintf( string_resto, sizeof(string_resto), "%s : %.2lf", PAGOS_OBTENER_NOMBRE( (char)1, (char)0 ), resto);
        ExecuteSSSAction( A_MUESTRA_RESTO, ST( S_FALTANTE ), string_resto, string_resto_me );
    }
}
/*****************************************************************************/
void  MOSTRAR_IMPORTE_MUTUAL( double importe, int cod_submedio)
/*****************************************************************************/
{
	char string_resto[40]="";
	char string_resto_me[40]="";
    char medios_aux[20];
	double resto_me = 0.0;
    //struct _t_mutuales mutual;
	//int area;
	int tabla_ant = SELECTED2();
    int tipo_ant = SELECTED_TIPO();
    
	//area = SELECTED();
	//if( USE_DB( AREA_AUX3, _mutuales, ( char* )&mutual, sizeof( struct _mutuales ), NULL, 0, 0 ) == 0 ) {
	if( OPEN_TABLE( T_MUTUALES, TT_ORIG, ( char* )&mutual, sizeof( struct _t_mutuales ) ) == 0 ) {
        //GET_EQUAL((char*) &cod_submedio );
    	SET_WHERE("COD_MUTUAL='%d'", cod_submedio );
		RUN_QUERY(NO);
		
		if( FOUND2() ) 
			_snprintf( medios_aux, sizeof(medios_aux), "%s", mutual.des_mutual);
		}
	//CLOSE_DATABASE( AREA_AUX3 );
	//SELECT( area );
	CLOSE_TABLE( T_MUTUALES, TT_ORIG );
	SELECT_TABLE( tabla_ant, tipo_ant );
  
	FTOC( importe, string_resto, ENTEROS + 1, DECIMALES, 0, 1 );
	resto_me = CALCULAR_RESTO( 1, cod_submedio, importe );
	_snprintf( string_resto, sizeof(string_resto), "%s : %.2lf", PAGOS_OBTENER_NOMBRE( (char)1, (char)0 ), importe);
	
	if(resto_me != importe)
		_snprintf(string_resto_me, sizeof(string_resto_me), "%s : %.2lf", medios_aux, resto_me);
    ExecuteSSSAction( A_MUESTRA_RESTO, ST( S_FALTANTE ), string_resto, string_resto_me );



}
/*****************************************************************************/
double CALCULAR_RESTO(int moneda_extranjera , int cod_submedio, double resto)
/*****************************************************************************/
{
double faltante = resto;
    //struct _t_mutuales mutual;

	if( moneda_extranjera == SI) //Moneda Extranjera
	{
	
    //int area;
	int tabla_ant = SELECTED2();
	int tipo_ant = SELECTED_TIPO();
	
    //area = SELECTED();
	
	//if( USE_DB( AREA_AUX3, _mutuales, ( char* )&mutual, sizeof( struct _mutuales ), NULL, 0, 0 ) == 0 ) {
	if( OPEN_TABLE( T_MUTUALES, TT_ORIG, ( char* )&mutual, sizeof( struct _t_mutuales ) ) == 0 ) {

        //GET_EQUAL( (char*) &cod_submedio );
	    SET_WHERE("COD_MUTUAL='%d'", cod_submedio);
		RUN_QUERY(NO);
		
        if( FOUND2() && mutual.cotizacion > 0.0 ) 
			faltante = resto/mutual.cotizacion; 
		else
			faltante = resto;
		}
		CLOSE_TABLE( T_MUTUALES, TT_ORIG );

		SELECT_TABLE( tabla_ant, tipo_ant );
	}//sino tenia cargado el resto en faltante
	
		

//CLOSE_DATABASE( AREA_AUX3 );
//SELECT( area );
	

return faltante;
}

void MOSTRAR_VUELTO_EN_GRANDE( double resto )
/*****************************************************************************/
{
    double vuelto;
    #if defined(INVEL_W) || defined(INVEL_L)
    char aux[30],aux1[20];
    #endif
    // esto va al panel ventas
    if( MOSTRAR_VUELTO ) {
        vuelto = ( RAM_RESTO_ME ) ? fabs( RAM_RESTO_ME ) : fabs( resto );
        CR1_DISPLAY_DE_PANTALLA();
        _ATRIBUTO = 59;
        _LETRA_GRANDE_PEGADA = 1;
        MOSTRAR_GRAN_CADENA( 3, 3, ST( S_VUELTO ) );
        if( config.pais == COLOMBIA || config.pais == CHILE ) {
            MOSTRAR_GRAN_FLOTANTE( 32 - ancho_panel, 3, ENTEROS - 1, DECIMALES, 1, 0, vuelto );
        }
        else {
            MOSTRAR_GRAN_FLOTANTE( 42 - ancho_panel, 3, ENTEROS - 1, DECIMALES, 1, 0, vuelto );
        }
        _LETRA_GRANDE_PEGADA = 0;
        #ifdef COMPILAR_DISPLAY
        MOSTRAR_EN_DISPLAY( ST( S_VUELTO ), vuelto );
        #endif
        #if defined(INVEL_W) || defined(INVEL_L)
        #ifdef COMPILAR_DISPLAY
        MOSTRAR_EN_DISPLAY( ST( S_VUELTO ), vuelto );
        #endif
        FTOC( vuelto, aux1, ENTEROS - 1, DECIMALES, 1, 0 );
        sprintf( aux, " " );
        ExecuteSAction( A_MUESTRA_ARTICULO, aux );
        #endif
    }
}
/*****************************************************************************/
void MOSTRAR_PAGO( int h )
/*****************************************************************************/
{
    int y,modo;
    char cadena[43];
    char pr[30];
    char temp[21];
    char temp1[21];
    char pag_importe_me[15];
    char stringAux[40];
    char cadaux1[80];
    char cadaux2[80];
    int aditivo = 0;
    memset( temp, 0, sizeof( temp ) );
    memset( temp1, 0, sizeof( temp1 ) );
    memset( cadaux1, 0, sizeof( cadaux1 ) );
    memset( cadaux2, 0, sizeof( cadaux2 ) );
    memset( cadena, 0, sizeof( cadena ) );
    FTOC( _PAGO_IMPORTE( h ), pr, ENTEROS + 1, DECIMALES, 1, 1 );
    y = 8 + h;
    modo = _PAGO_MODO( h );
    _ATRIBUTO = 59;
    MOSTRAR_CADENA( 23, y, medios[modo].nombre );
    MOSTRAR_FLOTANTE( 32, y, ENTEROS + 1, DECIMALES, 1, 0, _PAGO_IMPORTE( h ) );
    if( medios[modo].tarjeta ) {
        sprintf( cadena, NOMBRE_TARJETA( _PAGO_TARJETA( h ) ) );
            ENMASCARA_NUMERO_TARJETA( _PAGO_TARJETA( h ), _PAGO_NRO_TARJETA( h ), temp );
        }
    else {
        if( medios[modo].cheque ) {
            if( EXISTE_BANCO( _PAGO_BANCO( h ) ) ) {
                strcpy( cadena, bancos.nom_banco );
                cadena[17] = 0;
                MOSTRAR_CADENA( 45, y, cadena );
            }
        }
        if( medios[modo].ticket_compra ) {
            NOMBRE_TICKET_COMPRA( _PAGO_CUENTA( h ), cadena );
        }
        if( medios[modo].mutual ) {
            char xx[15];
            char xx2[30];
            double cot = 0;
            NOMBRE_MUTUAL( ( char )_PAGO_CUENTA( h ), cadena );
		    memset( xx, 0, sizeof( xx ) );
            COTIZACION_MUTUAL( ( char )_PAGO_CUENTA( h ), &cot, NULL );
		    cot = ( cot > 0.001 ) ? cot : 1.0;
            FTOC( _PAGO_IMPORTE( h ) / cot, xx, ENTEROS + 1, DECIMALES, 1, 1 );
            TRIM(xx);
            TRIM(pr);
            LTRIM(xx);
            LTRIM(pr);
            sprintf(xx2,"%s {%s}",xx,pr);
            strcpy(pr,xx2);
        }
        if( _PAGO_BUFFER1_LONG( h ) ) {
            MOSTRAR_ENTERO( 56, y, "000000", _PAGO_BUFFER1_LONG( h ) );
            SET_ENTERO( "000000", _PAGO_BUFFER1_LONG( h ), temp1 );
            strcat( temp, temp1 );
        }
        if( _PAGO_CUENTA( h ) ) {
            MOSTRAR_ENTERO( 63, y, "0000", _PAGO_CUENTA( h ) );
            SET_ENTERO( "0000", _PAGO_CUENTA( h ), temp1 );
            strcat( temp, temp1 );
        }
        if( _PAGO_NUMERO( h ) ) {
            MOSTRAR_ENTERO( 68, y, "00000000", _PAGO_NUMERO( h ) );
            SET_ENTERO( "00000000", _PAGO_NUMERO( h ), temp1 );
            strcat( temp, temp1 );
        }            
        if( _PAGO_IMPORTE_ME( h ) && _medio_moneda_extranjera >= 0 ) {
            //#if defined(INVEL_W) || defined(INVEL_L)
            FTOC( _PAGO_IMPORTE_ME( h ), pag_importe_me, ENTEROS + 1, DECIMALES, 1,
                  config.pais != CHILE );
            sprintf( stringAux, "%s %s", medios[_medio_moneda_extranjera].simbolo_moneda,
                     pag_importe_me );
            //ExecuteSSAction( A_MUESTRA_DATOS, T_FLAGS_MEDIO_EXTR_AGREGAR, stringAux, " ", NULL );  
            /*
            #else
            MOSTRAR_CADENA( 58, y, medios[_medio_moneda_extranjera].simbolo_moneda );
            if( config.pais != CHILE ) {
                MOSTRAR_FLOTANTE( 63, y, ENTEROS + 1, DECIMALES, 1, 1, _PAGO_IMPORTE_ME( h ) );
            }
            else {
                MOSTRAR_FLOTANTE( 63, y, ENTEROS + 1, DECIMALES, 1, 0, _PAGO_IMPORTE_ME( h ) );
            }
            #endif
            */
        }
    }
    sprintf( temp1, "%s", medios[modo].nombre );
    TRIM( temp1 );
    TRIM( pr );
    LTRIM( pr );
    if( MODO_NEGOCIO != SERVICIOS ) {
        TRIM( cadena );
    }
    else {
        memset( cadena, 0, 43 );
    }
    TRIM( temp );
    if( medios[modo].efectivo ) {
        aditivo = 1;
    }
    if( medios[modo].cheque && MODO_CHEQUE_COMO_UN_MEDIO ) {
        aditivo = 1;
        SET_ENTERO( "00000000", _PAGO_NUMERO( h ), temp );
    }
    agregarPago( aditivo && IMPORTE_CONTADO_ADITIVO_EN_PAGOS, temp1, pr, cadena, temp );
}
/**************************************************************************/
void MOSTRAR_RETENCIONES()
/**************************************************************************/
{
    char cad[25];

    if( RAM_RETENCION_DGR || RAM_PERCEPCION_DGI || RAM_PERCEPCION_212_IVA1
			|| RAM_PERCEPCION_212_IVA2 || RAM_PERCEPCION_TACIS ) {
		_snprintf( cad, sizeof(cad)-1, ST( S_TOT_ANT__LF_ ), ENTEROS, DECIMALES, RAM_IMPORTE_TOTAL );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_TOTAL_ANT_BORRAR, " ", " ", NULL, " " );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_TOTAL_ANT_AGREGAR, cad," ", NULL, " " );
    }
    else {
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_TOTAL_ANT_BORRAR, " ", " ", NULL, " " );
    }
    if( !RAM_RETENCION_DGR && retencion_dgr_on ) {
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_RETENCION_DGR_BORRAR_PAGO, " ", " ", NULL, " " );
        retencion_dgr_on = 0;
    }
    else if( RAM_RETENCION_DGR /*&& !retencion_dgr_on*/ ) {
        _snprintf( cad, sizeof(cad)-1, ST( S_RET__F_ ), ENTEROS, DECIMALES, RAM_RETENCION_DGR );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_RETENCION_DGR_AGREGAR_PAGO, cad, " ", NULL, " " );
        retencion_dgr_on = 1;
    }
    if( !RAM_PERCEPCION_DGI && percepcion_dgi_on ) {
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_PERCEPCION_DGI_BORRAR, " ", " ", NULL, " " );
        percepcion_dgi_on = 0;
    }
    else if( RAM_PERCEPCION_DGI /*&& !percepcion_dgi_on*/ ) {
		_snprintf( cad, sizeof(cad)-1, ST( S_PER__LF_ ), ENTEROS, DECIMALES, RAM_PERCEPCION_DGI );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_PERCEPCION_DGI_AGREGAR, cad, " ", NULL, " " );
        percepcion_dgi_on = 1;
    }
    if( ( !RAM_PERCEPCION_212_IVA1 && !RAM_PERCEPCION_212_IVA2 ) && percepcion_212_on ) {
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_PERCEPCION_212_BORRAR, " ", " ", NULL, " " );
        percepcion_212_on = 0;
    }
    else if( ( RAM_PERCEPCION_212_IVA1 || RAM_PERCEPCION_212_IVA2 ) /*&& !percepcion_212_on*/ ) {
		_snprintf( cad, sizeof(cad)-1, ST( S_PER__LF_ ), ENTEROS, DECIMALES, 
			( RAM_PERCEPCION_212_IVA1 ) ? RAM_PERCEPCION_212_IVA1 : RAM_PERCEPCION_212_IVA2 ); 
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_PERCEPCION_212_AGREGAR, cad, " ", NULL, " " );
        percepcion_212_on = 1;
    }
}
/*****************************************************************************/
void MOSTRAR_ENVIAR_OFFLINE()
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    char cad[20];
    if( !RAM_ENVIAR_OFFLINE && env_offline_on ) {
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_ENV_OFFLINE_BORRAR, " ", " ", NULL, " " );
        env_offline_on = 0;
    }
    else if( RAM_ENVIAR_OFFLINE && !env_offline_on ) {
        strcpy( cad, "OFFLINE" );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_ENV_OFFLINE_AGREGAR, cad, " ", NULL, " " );
        env_offline_on = 1;
    }
    #else
    if( !RAM_ENVIAR_OFFLINE ) {
        _ATRIBUTO = 7;
        MOSTRAR_CADENA( 14, 24, "       " );
    }
    else {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 14, 24, "OFFLINE" );
    }
    #endif
}
/*****************************************************************************/
void MOSTRAR_PEDIR_DIFERIMIENTO()
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    char cad[20];
    if( !RAM_PEDIR_DIFERIMIENTO && diferimiento_on ) {
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_DIFERIMIENTO_BORRAR, " ", " ", NULL, " " );
        diferimiento_on = 0;
    }
    else if( RAM_PEDIR_DIFERIMIENTO && !diferimiento_on ) {
        strcpy( cad, "DIFERIMIENTO" );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_DIFERIMIENTO_AGREGAR, cad, " ", NULL, " " );
        diferimiento_on = 1;
    }
    #else
    if( !RAM_PEDIR_DIFERIMIENTO ) {
        _ATRIBUTO = 7;
        MOSTRAR_CADENA( 23, 24, "   " );
    }
    else {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 23, 24, "DIF" );
    }
    #endif
}
/*****************************************************************************/
int PROCESAR_PROMOCION( int medio, int submedio )
/*****************************************************************************/
{
	struct requerir_medios medios_promocionados;
	int procesar = 1, cant = 0, cnt = 0;
	int cantidad_activas =0, cantidad_anuladas = 0;

	cant = CANTIDAD_REGISTROS_REC(ARCH_REC_REQUERIR_MEDIO,
			sizeof(struct requerir_medios));
 // esta funcion debe determinar si se llama a la evaluacion de post_pago o no.
	//va a andar con 1 solo tipo de promo postpago que sea iguales
	//si hago 2 promos postpago distintas no va a andar, si andara si hago una promo psotpago con 2medios
	//aunque sea 2 promos distintas pero 1que activa una desactiva la otra
	while (cnt < cant ) {
		RECUPERAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO,
				(char *) &medios_promocionados, sizeof(struct requerir_medios), cnt);

		if(( medios_promocionados.medio == medio + 1 && medios_promocionados.submedio == -1 ) 
				|| ( medios_promocionados.medio == medio + 1 && submedio && medios_promocionados.submedio == submedio ))  {
					if( medios_promocionados.eliminado == 0){ 
						cantidad_activas++;
						//procesar = medios_promocionados.eliminado;
					}else
						cantidad_anuladas++;

			//medios_promocionados.eliminado = 45 --- Se anulo con tecla '-' el medio de pago, se procesa promocion
			//medios_promocionados.eliminado = 27 --- Se anulo con tecla ESC la promocion, no se procesa promocion, el medio ingresado esta aun vigente.
			//medios_promocionados.eliminado = 0 --- La promo no fue anulada. Esto es valido mientra le motor aplique la promo sobre el total del ticket.
			//Si la promo es sobre lo ingresado, eliminado = 0 tiene que procesarse
//			break; //si encontre el promocionado salgo 
		}else{
			if( medios_promocionados.eliminado == 0){ 
						cantidad_activas++;
						//procesar = medios_promocionados.eliminado;
					}else
						cantidad_anuladas++;

			procesar = 0; //aca para no disparar la promo cuando no es el medio de pago 
		}
		cnt++;
	}
	
// si es el primer pago si o si hay que procesar
		//y si todos estan eliminados entonces? y ram_p_pago > 0
	if( cant == cantidad_anuladas)
		procesar = 1;

	return procesar;
}

/*****************************************************************************/
double IMPORTE_MEDIO_PROMOCIONADO( int medio, int submedio, double *importe_ingresado )
/*****************************************************************************/
{
	struct requerir_medios medios_promocionados;
	int pant_activa = 0, tecla = 0, lineas = 0;
	int cant = 0, mp = 0, cnt = 0, h = 0;
	char linea1[70], linea2[70], linea3[170], linea4[70];
	char lineaM[70], lineaP[70];
	char nom_submedio[9][30];
	char *descripcion=NULL, plan[10];
	long auxpagare= 0;

	double ram_resto = 0.00, ram_resto_aux = 0.00;
	double ram_ram_resto = 0.00, ram_ram_importe_total = 0.00,	ram_ram_vuelto = 0.00;
	double precio_final_con_promo = 0.00;

	memset( plan, 0 ,sizeof(plan) );
	memset( linea1, 0, sizeof(linea1) );
	memset( linea2, 0 ,sizeof(linea2) );
	memset( linea3, 0 ,sizeof(linea2) );
	memset( linea4, 0 ,sizeof(linea2) );
	memset( lineaM, 0, sizeof(lineaM) );
	memset( lineaP, 0 ,sizeof(lineaP) );
	memset( nom_submedio, 0 ,sizeof(nom_submedio) );

	/*Cantidad de medios/sumedio/plan limitados por promocion*/
	cant = CANTIDAD_REGISTROS_REC(ARCH_REC_REQUERIR_MEDIO,
			sizeof(struct requerir_medios));

	auxpagare = NRO_PAGARE;

	while (cnt < cant && !mp ) {
		RECUPERAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO,
				(char *) &medios_promocionados, sizeof(struct requerir_medios), cnt);

		if( NRO_PAGARE == 1
			&& LASTKEY() == 27 
			&& medios_promocionados.monto == -1 ) {//Se presiono ESC al box donde indica el importe a abonar
			ELIMINA_PAGO_PROMO_POSPAGO( medio + 1, submedio, 45 );
			glog("ESC EN PEDIDO PROMO",LOG_MOTOR_PROMO,3);
			return -1;

		}

		if( medios_promocionados.monto //Si el monto esta en cero entonces fue anulado la promocion
			&& medios_promocionados.eliminado != 27//Si anula con escape no se activa la promocion
			&& medios[medios_promocionados.medio - 1].nro == medio + 1 ) {
		//Verifico el medio promocionado en el comprobante y lo cargo
			if( medios[medios_promocionados.medio - 1].efectivo ) {		
				mp++;
			}
			if( medios[medios_promocionados.medio - 1].tarjeta ) {
				if( medios_promocionados.submedio == submedio
						&& medios_promocionados.plan
						//Cuando el plan = -1, todas las cuotas estan en promocion
						&& ( medios_promocionados.plan == -1 || medios_promocionados.plan == VAR_TARJETAS_CUOTAS ) ) {
					_snprintf( nom_submedio[mp], sizeof(nom_submedio)-1,"%s", NOMBRE_TARJETA(medios_promocionados.submedio-1));
					if( medios_promocionados.plan > 0 )
						sprintf(plan, "Cuota %i", medios_promocionados.plan);
					mp++;
				}	
			}
			if( medios[medios_promocionados.medio - 1].ticket_compra ) {
				if( medios_promocionados.submedio == submedio ) {	
					NOMBRE_TICKET_COMPRA(medios_promocionados.submedio, nom_submedio[mp]);	
					mp++;
				}
			}
			if( medios[medios_promocionados.medio - 1].mutual ) {
				if( medios_promocionados.submedio == submedio ) {
					NOMBRE_MUTUAL((char)(medios_promocionados.submedio),nom_submedio[mp]);
					mp++;
				}
			} //neuvo cta cte
			if( medios[medios_promocionados.medio - 1].cta_cte) {
					mp++;
			}
		}
		cnt++;
	}

	if( mp > 0 ) {//Si mp > 0 entonces el medio esta promocionado
		//Cuando se aplica una promo RAM_TOTAL se actualiza con el importe +- la promo aplicada
		if( RAM_TOTAL != RAM_IMPORTE_TOTAL ) {		
		//	if( NRO_PAGARE == 1 || ) {//segun promo puede ser importe final con promo incluida osea el importe a pagar O el importe ingresado para abonar				
				*importe_ingresado = ROUND( medios_promocionados.monto, 10, DECIMALES);//importe ingresado para abonar 
		//	} 
			SET_MEMORY_LONG( __nro_pagare, 99 );//Variable reutilizada marco este estado 99 por si se corta aqui
				//precio_final_con_promo = ROUND(RAM_TOTAL, 10, DECIMALES);
			precio_final_con_promo = ROUND(RAM_RESTO, 10, DECIMALES); //mmancuso
//mmancuso			ram_resto = ROUND(RAM_TOTAL, 10, DECIMALES) - RAM_IMPORTE_TOTAL;
//mmancuso			ram_resto_aux = *importe_ingresado - precio_final_con_promo;//Ingrese mas $ que el total del ticket
			ram_resto = ram_resto_aux;
			ram_ram_resto = RAM_RESTO;
			ram_ram_importe_total = RAM_IMPORTE_TOTAL;
			ram_ram_vuelto = RAM_VUELTO;
//mmancuso			SET_MEMORY_DOUBLE(__ram_resto, ROUND(RAM_TOTAL, 10, DECIMALES));
			SET_MEMORY_DOUBLE(__ram_importe_total, ROUND(RAM_TOTAL, 10, DECIMALES));//Seteo para que actualice lo que muestra en PAGOS_MUESTRA_TOTAL
			SET_MEMORY_DOUBLE(__ram_vuelto, ram_resto);
			
		}

	}

	if( mp /*&& ram_resto*/ ) {
		ADD_MEMORY_CHAR( __ram_p_pago, 1 );//esta suma es a los fines de adelantar el pago y el corte de luz se de aca

		lineas = strlen("        Se aplico promocion en Medio de Pago")
			+ strlen("&")
			+ strlen("                        IMPORTE A PAGAR   $ %.2lf")
			+ strlen("&")
			+ strlen("&")
			+ ( strlen(lineaM) * mp ) + mp/*&*/			
			+ strlen("&")
			+ strlen("&")
			+ strlen("Total Ticket $%.2lf")
			+ ( strlen(lineaP) * mp ) + mp/*&*/
			+ strlen("&")
			+ strlen("&")
			//+ strlen("Confirmar Pago <ENTER> - Cancelar Pago <ESCAPE>");
			+ strlen("               Presione <ENTER> para continuar");

		//total de lineas a mostrar
		if( lineas > 0 )
			descripcion =(char*) malloc(lineas + 100);	

		if( descripcion && lineas ) {
			memset( descripcion, 0 ,lineas);
			//limpio las lineas para solo mostrar las posibles, con valores != 0
			lineas=0;

			//TITULO y centrado
			_snprintf( linea1, sizeof(linea1)-1,"%s", "        Se aplico promocion en Medio de Pago");
			linea1[strlen(linea1)]=0;

			//Descripcion
			linea2[0]='&';
			_snprintf( &linea2[1], sizeof(linea2)-1,"                        IMPORTE A PAGAR   $ %.2lf",RAM_RESTO /*RAM_TOTAL*/ ); //MANCUSO CAMBIO

			PAGOS_MOSTRAR_RESTO(ram_resto); 
			PAGOS_MUESTRA_TOTAL(RAM_TOTAL);
			linea2[strlen(linea2)]=0;

			lineaM[0]='&';
			lineaM[1]='&';
			if( medios[medios_promocionados.medio - 1].efectivo  || medios[medios_promocionados.medio - 1].cta_cte )
//				_snprintf( &lineaM[2], sizeof(lineaM)-1,"El medio en promocion %s   $%.2lf" ,medios[medios_promocionados.medio - 1].nombre,
				_snprintf( &lineaM[2], sizeof(lineaM)-1,"Usted decidio pagar con %s   $%.2lf" ,medios[medios_promocionados.medio - 1].nombre,
				*importe_ingresado/*precio_final_con_promo + ram_resto*/);
			if( medios[medios_promocionados.medio - 1].tarjeta)
				//_snprintf( &lineaM[2], sizeof(lineaM)-1,"El medio en promocion%s %s %s   $%.2lf" ,medios[medios_promocionados.medio - 1].nombre, 
				  _snprintf( &lineaM[2], sizeof(lineaM)-1,"Usted decidio pagar con %s   $%.2lf" ,medios[medios_promocionados.medio - 1].nombre,
				nom_submedio, plan, precio_final_con_promo + ram_resto);
			if( medios[medios_promocionados.medio - 1].ticket_compra || medios[medios_promocionados.medio - 1].mutual )
				//_snprintf( &lineaM[2], sizeof(lineaM)-1,"El medio en promocion %s %s   $%.2lf" ,medios[medios_promocionados.medio - 1].nombre, 
				  _snprintf( &lineaM[2], sizeof(lineaM)-1,"Usted decidio pagar con %s   $%.2lf" ,medios[medios_promocionados.medio - 1].nombre,
				nom_submedio, precio_final_con_promo + ram_resto);
			
			linea3[0]='&';
			_snprintf( &linea3[1], sizeof(linea2)-1,"-------------------------RESUMEN COMPRA---------------------");
			linea3[60]='&';
			linea3[61]='&';
			_snprintf( &linea3[62], sizeof(linea3)-1,"Importe al que Aplic Desc/Rec   $%.2lf",RAM_SUBTOTAL );
			linea3[strlen(linea3)+1]='&';
			linea3[strlen(linea3)]=0;
			
			lineaP[0]='&';
			_snprintf( &lineaP[1], sizeof(lineaP)-1,"Desc/Rec   -$%.2lf" , fabs(precio_final_con_promo - RAM_SUBTOTAL) );

			lineaM[strlen(lineaM)]=0;
			lineaP[strlen(lineaP)]=0;
			strcat(descripcion,linea2);
			strcat(descripcion,lineaM);
			strcat(descripcion,linea3);

			strcat(descripcion,lineaP);

			memset( linea4,' ',sizeof(linea4));
			linea4[0]='&';
			linea4[1]='&';
			_snprintf( &linea4[2],sizeof(linea4)-1," %s", "               Presione <ENTER> para continuar");
			linea4[strlen(linea4)]=0;
			strcat(descripcion,linea4);

			ExecuteSAction( A_GET_PANEL_ACTIVO, ( char* )&pant_activa );
			ExecuteSSAction( A_PEDIR, _HIBRIDO, linea1, descripcion, NULL, " " );
						
			while( tecla != 13 /*&& tecla != 27*/ 
					 ) {
				tecla = GETCH();
			}
			/*if( tecla == 27 ) {
				ram_resto = 0;
				ELIMINA_PAGO_PROMO_POSPAGO( medio + 1, submedio, 27 );
				//SET_MEMORY_LONG( __nro_pagare, medio + 1 );//Variable reutilizada para que no active la promocion si ingresa nuevamente $ en el medio en promo
			}*/

			ADD_MEMORY_CHAR( __ram_p_pago, -1 );//vuelvo el pago 
			ExecuteSAction( A_FOCO_A_PANEL_ACTIVO, ( char* )&pant_activa );
			ExecuteAction( A_FIN_PEDIR );
		}
	
		if( descripcion  ) {
			FREEMEM( ( char* )descripcion );
		}
	}
	SET_MEMORY_LONG( __nro_pagare, auxpagare );

	return( ram_resto );

}


/*****************************************************************************/
void MOSTRAR_SALDO_PROMOCIONES( )
/****************************************************************************/
{
	int pos = 0, b =0, a = 0; // posicion en el archivo rec
	int cant = 0;
	struct requerir_medios medios_requeridos;
	int lineas = 0, size_lineas = 0, s = 0, p = 0, i = 0;
	char *me_aux=NULL;
	char tmp[60], tmp1[100],submedio_promocionado[100], cadena_auxiliar[100], plan[20];
    //tmp[45]
	memset( tmp, 0,sizeof( tmp ) );
	memset( tmp1, 0,sizeof( tmp1 ) );
	memset( submedio_promocionado, 0,sizeof( submedio_promocionado ) );
	memset( cadena_auxiliar, 0,sizeof( cadena_auxiliar ) );
	memset( plan, 0,sizeof( plan ) );
	memset( &medios_requeridos, 0, sizeof( struct requerir_medios ) );

	//Cantidad de registros en el archivo req.
	cant = CANTIDAD_REGISTROS_REC( ARCH_REC_REQUERIR_MEDIO,
			sizeof(struct requerir_medios));
	//lineas = cant;
	while( pos < cant ) {
		RECUPERAR_ARCHIVO_REC( ARCH_REC_REQUERIR_MEDIO,
				(char *) &medios_requeridos, sizeof(struct requerir_medios),
				pos );
		if( medios_requeridos.origen == ORIGEN_PROMOCION
			/*&& medios_requeridos.cod_grupo == grupo*/ ) {
			lineas ++;
		}
		pos++;
	}

	//total de lineas a mostrar
	if( lineas > 0 ) {
		lineas = lineas + 1;
		size_lineas = ++lineas*sizeof( tmp1 );
		me_aux =(char*) malloc( size_lineas );
	}

	if( me_aux && lineas ) {
		memset( me_aux, 0, size_lineas );
		//limpio las lineas para solo mostrar las posibles, con valores != 0
		lineas = pos = 0;
		
		memset( tmp1, ' ',sizeof( tmp1)); 
		_snprintf( tmp1, sizeof(tmp1)-1, "Medio            Submedio            Plan            Importe" );
		b = strlen( tmp1 );
		a = ( sizeof( tmp1 ) - 20 - ( b ) ) / 2;
		memset( tmp1,' ',sizeof(tmp1)); 

		#if defined(INVEL_W)
				tmp1[0]='&';
				_snprintf( &tmp1[1+a], sizeof(tmp1)-1, "Medio            Submedio            Plan            Importe" );
		#else
				_snprintf( &tmp1[a], sizeof(tmp1)-1, "Medio            Submedio            Plan            Importe" );
		#endif
				tmp1[strlen(tmp1)]=0;
				strncat( me_aux, tmp1, strlen( tmp1 ) );

		//Recorremos las promociones a mostrarse
		while( pos < cant ) {
			RECUPERAR_ARCHIVO_REC( ARCH_REC_REQUERIR_MEDIO,
					(char *) &medios_requeridos, sizeof(struct requerir_medios),
					pos );
			if( medios_requeridos.origen == ORIGEN_PROMOCION 
					/*&& medios_requeridos.cod_grupo	== grupo*/ ) {				
				memset( submedio_promocionado, 0,sizeof( submedio_promocionado ) );
				s = 30, p = 40,	i = 74;
				if( medios_requeridos.submedio > -1 ) {
					s = 23, i = 60;
					if( medios[medios_requeridos.medio - 1].tarjeta ) {
						strncat( submedio_promocionado, 
							NOMBRE_TARJETA( medios_requeridos.submedio - 1 ), 
							10 );
					} else if( medios[medios_requeridos.medio - 1].ticket_compra ) {
						NOMBRE_TICKET_COMPRA( medios_requeridos.submedio,
								cadena_auxiliar );
						strncat( submedio_promocionado, 
							cadena_auxiliar, 
							11 );
					} else if( medios[medios_requeridos.medio - 1].mutual ) {
						NOMBRE_MUTUAL((char)( medios_requeridos.submedio ),
								cadena_auxiliar );
						strncat( submedio_promocionado, 
							cadena_auxiliar, 
							11 );
					}
				}
				if( medios_requeridos.plan > -1 ) {
					 p = 43, i = 58;
					_snprintf( plan, sizeof(plan)-1, "%i",medios_requeridos.plan);
				}
				memset( tmp1,' ',sizeof(tmp1));						
				#if defined(INVEL_W)
						tmp1[0]='&';
						strncpy( &tmp1[8], medios[medios_requeridos.medio - 1].nombre, 
							strlen( medios[medios_requeridos.medio - 1].nombre ) );
						strncpy( &tmp1[s], submedio_promocionado, 
							strlen( submedio_promocionado ) );
						strncpy( &tmp1[p], plan, strlen( plan ) );
						_snprintf( tmp, sizeof(tmp)-1, "$%.2lf", medios_requeridos.monto );
						strncpy( &tmp1[i], tmp, 8 );
				#else
						/*		
							snprintf( &tmp1[a], sizeof(tmp1)-1,  "%s         %s         %s         $%.2lf",
							medios[medios_requeridos.medio - 1].nombre, 
							medio_promocionado, 
							plan, medios_requeridos.monto );
						*/
				#endif
						//armo cadena final con el mensaje de la cotizacion para cada medio
						b=strlen(tmp1);
						tmp1[strlen(tmp1)]=0;
						strncat( me_aux, tmp1, strlen( tmp1 ) );
						lineas++;
			}
			pos++;
		}//fin de la carga de medios promocionados
	}
	
	if( lineas == 0 ) {//sino tengo datos aviso del error.. y dejo cargado los avisos
		int pant_activa = 0;
		lineas = 1;//mensaje

		/*por las dudas lo recargo al  vector de avisos*/
		FREEMEM( ( char* )me_aux );
		size_lineas = ++lineas*sizeof( tmp1 );
		me_aux =(char*) malloc( size_lineas );

		if( me_aux ) {
			//lineas*sizeof(tmp1)-5
			memset( me_aux, 0, size_lineas );

		//datos incompletos cotizacion o importe convertido
		//tamanio de la cadena para centrar..
		//mensaje de error
		memset( tmp1, " " ,sizeof( tmp1));	
		#if defined(INVEL_W)
				_snprintf( tmp1, sizeof(tmp1)-1, " %s", "NO HAY MEDIOS EN PROMOCION");
		#else
				_snprintf( tmp1, sizeof(tmp1)-1, " %s\n", "NO HAY MEDIOS EN PROMOCION");
		#endif
				//posicion que debe tomar la cadena, adelanta espacios en blanco
				b = strlen( tmp1 );
				a = ( sizeof( tmp1 ) - 33 - ( b ) ) / 2;				
				//a = 19;
				memset( tmp1,' ',sizeof(tmp1));
		#if defined(INVEL_W)
				tmp1[0]='&';
				_snprintf( &tmp1[1+a], sizeof(tmp1)-1, " %s", "NO HAY MEDIOS EN PROMOCION");
		#else
				_snprintf( &tmp1[a], sizeof(tmp1)-1, " %s \n", "NO HAY MEDIOS EN PROMOCION");
		#endif
				//armo cadena final
				tmp1[strlen(tmp1)]=0;
				strncat( me_aux, tmp1, strlen( tmp1 ) );
				MENSAJE_STRING(S_NO_SE_ENCONTRO_CONFIGURACION);
				BORRAR_MENSAJE();
		}
	}

	//completo el mensaje sea con datos o con mensaje de error de configuracion, 
	//y dibuja la pantalla con los mensajes
	if( lineas > 0 && me_aux ) {
		int pant_activa = 0;
					
		memset( tmp, " " ,sizeof( tmp));
		#if defined(INVEL_W)
				_snprintf( tmp1, sizeof(tmp1)-1, " %s", ST(S_PRESIONE_ENTER_PARA_CONTINUAR));
		#else
				_snprintf( tmp1, sizeof(tmp1)-1, " %s\n", ST(S_PRESIONE_ENTER_PARA_CONTINUAR));
		#endif
				//posicion que debe tomar la cadena, adelanta espacios en blanco
				b = strlen( tmp1 );
				a = ( sizeof( tmp1 ) - 33 - ( b ) ) / 2;
				
		//a=16;
		memset( tmp1,' ',sizeof(tmp1));
		#if defined(INVEL_W)
				tmp1[0]='&';
				tmp1[1]='&';
				_snprintf( &tmp1[1+a],sizeof(tmp1)-1," %s", ST(S_PRESIONE_ENTER_PARA_CONTINUAR));
		#else
				_snprintf( &tmp1[a], sizeof(tmp1)-1," %s", ST(S_PRESIONE_ENTER_PARA_CONTINUAR));
		#endif
				tmp1[strlen(tmp1)]=0;
				strncat( me_aux, tmp1, strlen( tmp1 ) );

		//TITULO y centrado
		memset( tmp, ' ',sizeof( tmp ) );
		_snprintf( tmp, sizeof(tmp)-1,"MEDIOS PROMOCIONADOS" );
		b = strlen( tmp );
		a = ( sizeof( tmp ) - 20 - ( b ) ) / 2;
		memset( tmp,' ',sizeof(tmp));
		#if defined(INVEL_W)
				_snprintf( &tmp[1+a], sizeof(tmp)-1,"MEDIOS PROMOCIONADOS" );
		#else
				_snprintf( &tmp[a], sizeof(tmp)-1,"MEDIOS PROMOCIONADOS" );
		#endif
		tmp[strlen(tmp)]=0;

		ExecuteSAction( A_GET_PANEL_ACTIVO, ( char* )&pant_activa );
		#if defined(INVEL_W)
				ExecuteSSAction( A_PEDIR, _HIBRIDO, tmp, me_aux, NULL, " " );
		#else
				ExecuteSSAction( A_PEDIR, _SIMPLE_INPUT_OCULTO , tmp, me_aux, NULL,"" );
		#endif

		while( GETCH() != 13 );

			ExecuteSAction( A_FOCO_A_PANEL_ACTIVO, ( char* )&pant_activa );
			ExecuteAction( A_FIN_PEDIR );
	}

	if( me_aux ) {
		FREEMEM( ( char* )me_aux );
	}
}

/*****************************************************************************/
int MOSTRAR_CUPON_ANULAR(long nro_ticket, double importe, int cuota, long fecha,char * num_tarj, int cashback )
/*****************************************************************************/
{
	char aux[50],aux2[50],aux3[50],aux4[50],aux5[50],aux6[50],aux7[50];
	int cant = 0, car;
	char *cad = NULL;
	double importeR;
	char fechas[9];
	int rta = 0;


	memset(&num_tarj[6],'*',5);
	memset(aux4,0, sizeof(aux4));
	memset(aux3,0, sizeof(aux3));
	
	DTOA_INVERSA2( fecha, fechas );
	fechas[8]=0;  //140113 

	importeR = importe;  
	importeR = (int)(importeR * 100);
	importeR = importeR / 100; //

	_snprintf( aux, 50, "Nro de Cupon  : %li", nro_ticket);
    aux[49] = '\0';
    _snprintf( aux2, 50, "Importe      : %lf", importeR );
    aux2[49] = '\0';
    _snprintf( aux5, 50, "Cuotas       : %d", cuota );
    aux5[49] = '\0';
    _snprintf( aux6, 50, "Fecha        : %s", fechas );
    aux6[49] = '\0';
    _snprintf( aux7, 50, "Nro Tarjeta  : %s", num_tarj);
    aux7[49] = '\0';
	
    cant = strlen( aux ) + strlen( aux2 ) + strlen( aux5 )
                 + strlen( aux6 ) + strlen( aux7 ) ;
	
      if( ( cad = ( char* )malloc( cant + 6 ) ) != NULL ) {
	            memset( cad, 0, cant + 6 );
				glog("1-19 ",1,1);
                _snprintf( cad, cant + 6, "&%s&%s&%s&%s&%s",
                        aux, aux2, aux5, aux6, aux7 );
				glog("1-20 ",1,1);
                cad[ cant + 5] = '\0';
				if( cashback == SI)
					ExecuteSSAction( A_PEDIR, _HIBRIDO, " COMPRA TARJETA ORIGINAL ", cad, NULL, " " );
				else
					ExecuteSSAction( A_PEDIR, _HIBRIDO, " COMPRA TARJETA A ANULAR ", cad, NULL, " " );

            }
     if( cad ) {
         free( cad );
     }
	 if( cashback == SI)
		MENSAJE( "ES LA COMPRA ORIGINAL? S/N" );
	 else
		MENSAJE( "ES LA COMPRA QUE DESEA ANULAR? S/N" );
     
	 TECLADO_ALFABETICO = 1;
            do {
                car = GETCH();
            }
            while( car != 'S' && car != 'N' && car != 's' && car != 'n' );
            #if defined(INVEL_W) || defined(INVEL_L)
            ExecuteAction( A_FIN_PEDIR );
            #endif
            BORRAR_MENSAJE();
            TECLADO_ALFABETICO = 0;
			 if( car == 'S' || car == 's')
				rta = 1;

 return rta;
}