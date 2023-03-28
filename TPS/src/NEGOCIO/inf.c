#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cmemory.h> 
#include <cr.h> 
#include <fiscal.h>
#include <dbrouter.h> 
#include <eventos.h> 
#include <getch.h> 
#include <inf.h> 
#include <inf00.h> 
#include <inf05.h> 
#include <inf10.h> 
#include <inf15.h> 
#include <inf_dif.h> 
#include <med_rein.h> 
#include <mensaje.h> 
#include <menu.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <pant.h> 
#include <pausa.h> 
#include <print.h> 
#include <recupero.h> 
#include <ssaver.h> 
#include <tkt.h> 
#include <_cr0.h> 
#include <_cr1.h> 
#include <t_pago.h>
#include <path.h>
#include "import_t.h"
#include <ini.h>

/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W) || defined(INVEL_L)
#endif
extern int grabar_copia_cierre_z;
extern int grabar_copia_ticket;
/*****************************************************************************/
int MENU_INFORMES()
/*****************************************************************************/
{
    int k, seguir = 1, rta = 0, y = 0, i = 0;
    struct _menu *items = NULL;
    #if defined(INVEL_W) || defined(INVEL_L)
    struct _cabmenu *cab;
    int columnas;
    char *p = NULL;
    #endif
    if( INF05_HABILITADO ) {
        y++;
    }
    if( INF10_HABILITADO ) {
        y++;
    }
    if( INF15_HABILITADO ) {
        y++;
    }
    if( INF_CAJERO_CONSOLIDADO_HABILITAD ) {
        y++;
    }
    if( ( items = ( struct _menu * )malloc( sizeof( struct _menu ) * ( y + 1 ) ) ) != NULL ) {
        y = 0;
        if( ( items[y].nom = ( char* )malloc( sizeof( string_table->mensaje ) ) ) != NULL ) {
            strcpy( items[y].nom, ST( S__1__INFORME_00_ ) );
            items[y].tecla = '1';
            items[y].rta = 1;
            y++;
        }
        if( INF05_HABILITADO ) {
            if( ( items[y].nom = ( char* )malloc( sizeof( string_table->mensaje ) ) ) != NULL ) {
                strcpy( items[y].nom, ST( S__2__INFORME_05_ ) );
                items[y].tecla = '2';
                items[y].rta = 2;
                y++;
            }
        }
        if( INF10_HABILITADO ) {
            if( ( items[y].nom = ( char* )malloc( sizeof( string_table->mensaje ) ) ) != NULL ) {
                strcpy( items[y].nom, ST( S__3__INFORME_10_ ) );
                items[y].tecla = '3';
                items[y].rta = 3;
                y++;
            }
        }
        if( INF15_HABILITADO ) {
            if( ( items[y].nom = ( char* )malloc( sizeof( string_table->mensaje ) ) ) != NULL ) {
                strcpy( items[y].nom, ST( S__4__INFORME_15_ ) );
                items[y].tecla = '4';
                items[y].rta = 4;
                y++;
            }
        }
        if( INF_CAJERO_CONSOLIDADO_HABILITAD ) {
            if( ( items[y].nom = ( char* )malloc( sizeof( string_table->mensaje ) ) ) != NULL ) {
                strcpy( items[y].nom, ST( S__5__CON__CAJERO ) );
                items[y].tecla = '5';
                items[y].rta = 5;
                y++;
            }
        }
        #if defined(INVEL_W) || defined(INVEL_L)
        for( i = 0;i < y;i++ ) {
            p = strchr( items[i].nom, '-' );
            *p = ')';
        }
        #endif
    }
    if( IS_PRINTER() ) {
        /*----------------------- elige opcin -----------------------------*/
        while( seguir ) {
            #if defined(INVEL_W) || defined(INVEL_L)
            if( y ) {
                columnas = 2;
                cab = ( struct _cabmenu * )malloc( sizeof( struct _cabmenu ) * columnas );
                if( cab != NULL ) {
                    cab[0].nombre = "COD";
                    cab[0].align = ALIGN_LEFT;
                    cab[0].width = 8;
                    cab[1].nombre = "INFORME";
                    cab[1].align = ALIGN_LEFT;
                    cab[1].width = 20;
                    k = MENU( "INFORMES", items, y, cab, columnas );
                    free( cab );
                }
            }
            #else
            k = MENU( 0, 17, "INFORMES", items, y, 120, 52 );
            #endif
            k = ( k == -1 ) ? 27 : k;
            switch( k ) {
                case 27:
                    /*-------- ESC ---------*/
                    seguir = 0;
                    break;
                case 1:
                    INFORME_00( _x, 0, NO );
                    rta = 1;
                    break;
                case 2:
					
                    if( INF05_HABILITADO ) {
                        INFORME_05( _x );
						rta = 1;
                    }
                    break;
                case 3:
					
                    if( INF10_HABILITADO ) {
                        INFORME_10( _x );
						rta = 1;
                    }
                    break;
                case 4:
					
                    #ifdef COMPILAR_INFORME15
                    if( INF15_HABILITADO ) {
                        INFORME_15( _x );
						rta = 1;
                    }
                    #endif
                    break;
                case 5:
					
                    if( INF_CAJERO_CONSOLIDADO_HABILITAD ) {
                        INFORME_00_CAJERO( _x_cajero );
						rta = 1;
                    }
                    break;
            }
        }
    }
    else {
        MENSAJE_STRING( S_IMPRESORA_FUERA_LINEA );
        while( GETCH() != 27 );
    }
    /*---------------------------- fin ---------------------------------*/
    if( items != NULL ) {
        for( i = 0;i < y;i++ ) {
            free( items[i].nom );
        }
        free( items );
    }
    return ( rta );
}
/*****************************************************************************/
int MENU_INFORMES_Z( int con_menu )
/*****************************************************************************/
{
    int k, seguir = 1, rta = 0, emitir_z, y = 0, i;
    int emitir = 0, reimprimir = 1, salir = 0;
    struct _menu *items = NULL;
    #if defined(INVEL_W) || defined(INVEL_L)
    struct _cabmenu *cab;
    int columnas;
    char *p = NULL;
    #endif
    if( ( items = ( struct _menu * )malloc( sizeof( struct _menu ) * 2 ) ) != NULL ) {
        y = 0;
        if( ( items[y].nom = ( char* )malloc( sizeof( string_table->mensaje ) ) ) != NULL ) {
            strcpy( items[y].nom, ST( S__1__INFORMES_Z_ ) );
            items[y].tecla = '1';
            items[y].rta = 1;
            y++;
        }
        if( ( items[y].nom = ( char* )malloc( sizeof( string_table->mensaje ) ) ) != NULL ) {
            if( USAR_DIGITO_PARA_REIMPRIMIR_Z ) {
                strcpy( items[y].nom, ST( S__9__REIMPRIMIR_ ) );
                items[y].tecla = '9';
                items[y].rta = 9;
            }
            else {
                strcpy( items[y].nom, ST( S__R__REIMPRIMIR_ ) );
                items[y].tecla = 'R';
                items[y].rta = 3;
            }
            y++;
        }
        #if defined(INVEL_W) || defined(INVEL_L)
        for( i = 0;i < y;i++ ) {
            p = strchr( items[i].nom, '-' );
            *p = ')';
        }
        #endif
    }
    if( IS_PRINTER() || !con_menu ) {
        /*----------------------- elige opcin -----------------------------*/
        TECLADO_ALFABETICO = 1;
        while( seguir ) {
            if( con_menu ) {
                if( !ssaver_activo ) {
                    #if defined(INVEL_W) || defined(INVEL_L)
                    if( y ) {
                        columnas = 2;
                        cab = ( struct _cabmenu * )malloc( sizeof( struct _cabmenu ) * columnas );
                        if( cab != NULL ) {
                            cab[0].nombre = "COD";
                            cab[0].align = ALIGN_LEFT;
                            cab[0].width = 8;
                            cab[1].nombre = "INFORME";
                            cab[1].align = ALIGN_LEFT;
                            cab[1].width = 20;
                            ExecuteAction( A_DES_PRESEN_ACT_CIERRE );
                            k = MENU( "INFORMES", items, y, cab, columnas );
                            free( cab );
                        }
                    }
                    #else
                    k = MENU( 0, 17, "INFORMES", items, y, 120, 52 );
                    #endif
                    k = ( k == -1 ) ? 27 : k;
                }
            }
            else {
                k = 1;
            }
            switch( k ) {
                case 27:
                    /*-------- ESC ---------*/
                    seguir = 0;
                    break;
                case 1:
                    grabar_copia_ticket = 0; //indica que no guarde la impresion como ticket
                    OPEN_Z_PRN();
                    //si se hace un cierre se guarda el evento supervisor
                    GRABAR_SUPER_REC_A_EVENT( RAM_ID_EVENTO );
                    if( con_menu ) {
                        if( PERMITE_Z_LOCAL ) {
                            emitir = 1;
                        }
                        else {
                            MENSAJE_STRING( S_NO_SE_PERMITE_EMITIR_Z_LOCAL );
                        }
                    }
                    else {
                        if( TIPO_IMPRESORA != IMPRESORA_DRIVER && !INICIALIZAR_PRINTER() ) {
                            emitir = 0;
                            rta = -2;
                        }
                        else {
                            PAUSA( 5 );
                            emitir = 1;
                        }
                    }
                    //TIcket en espera
                    if( !PERMITIR_CIERRE_CON_TICKET_ESPER && emitir ) {
                        //if( CONTAR_TICKET_ESPERA( 30, NRO_CAJA, NRO_Z, -1) > 0 ){
						if( CONTAR_TICK_ESP_SIN_PROCESAR( 0, ON_OFF ) >= 1 ) {
                            emitir = 0;
                            MENSAJE_STRING( S_TIENE_TICKET_ESPERA );
                            MENSAJE_STRING( S_CIERRE_ABORTADO );
                            MENSAJE_TECLA_STRING( S_PRESIONE_UNA_TECLA );
                        }
                        else if( bandera_online = 0 && CONTAR_TICK_ESP_SIN_PROCESAR( 0, ON_OFF ) ) {
                            emitir = 0;
                            MENSAJE_STRING( S_TIENE_TICKET_ESPERA );
                            MENSAJE_STRING( S_CIERRE_ABORTADO );
                            MENSAJE_TECLA_STRING( S_PRESIONE_UNA_TECLA );
                        }
                    }
                    if( emitir ) {
                        emitir_z = ( MULTIPLES_USUARIOS_PARA_VENTA ) ? CR0_PEMITE_CERRAR( con_menu )
                                 : !__NO_SE_USA_002;
                        if( emitir_z ) {
                            rta = 1;
                            if( rta == 1 || PERMITE_Z_SIN_RED ) {
                                // HACER BACKUP ARCHIVOS
                                if( IMPRIMIR_Y_EN_Z ) {
                                    IMPRIME_INFORMES_Y_EN_Z( _z );
                                }
                                SET_MEMORY_CHAR( __nro_modo, 'Z' );
                                RECUPERO_DE_CHEQUES_CARGADOS();
								if(config_tps.ImprimirYdespuesDeZ == 1) {//esto e para que haga el abrir doc no fiscal que venia mal por no tirar la z 									
									char buffer_aux[20];
									SET_MEMORY_CHAR( __ram_impresion_fiscal, 0 );	
									memset(buffer_aux,0,sizeof(buffer_aux));
									sprintf(buffer_aux,"&0&");
									//actualizo el estado del comprobante para la bandera de impresion en CIMain
									setData( ACTION_UPDATE_ESTADO_DOC_NO_FISCAL,buffer_aux, 20 );
								}

                                INFORME_00( _z, 0, NO );
								if( MODO_NEGOCIO == SERVICIOS && EMITIR_INFORME_DIFERENCIAS_SUBME ) {
									INFORME_DIFERENCIAS_SUBMEDIOS_CONTINUO( );
								}
								if( MODO_NEGOCIO == RETAIL && config.pais != MEXICO  && EMITIR_INFORME_DIFERENCIAS_SUBME ) {
                                    //Mexico: no se puede evitar imprimir el inf dif solo desde el 
                                    //tkt-caja.cfg ya que parte del encabezado se imprime incondicionalmente
									INFORME_DIFERENCIAS_SUBMEDIOS_RETAIL( 'Z' );
								}
                                if( INF05_HABILITADO /*&& IMPRESORA_FISCAL!=FISCAL*/) {
                                    INFORME_05( _z );
                                }
                                if( INF10_HABILITADO /*&& IMPRESORA_FISCAL!=FISCAL*/) {
                                    INFORME_10( _z );
                                }
                                #ifdef COMPILAR_INFORME15
                                if( INF15_HABILITADO ) {
                                    INFORME_15( _z );
                                }
                                #endif
                                // SI ES LOCAL Y NO ESTADO  HABILITADO EL
                                // CORTE NO VA A CORTAR EL PAPEL;
                                if( con_menu ) {
                                    corte_habilitado = 1;
                                }
                                if( INF_TAR_HABILITADO ) {
                                    INFORME_TARJETAS( _z );
                                }
                                rta = 1;
                            }
                        }
                        else {
                            rta = 2;
                        }
                    }
                    seguir = 0;
                    break;
                case 9:
                case 3:
                    //Si es una reimpresion se borra el rec de supervisor ya que
                    //no se debe grabar evento supervisor
                    BORRAR_ARCHIVO_REC( ARCH_REC_SUPERVISOR );
                    grabar_copia_cierre_z = 0;
					SET_MEMORY_INT( __ram_reimpresion, 1 );
					if( IMPRESORA_FISCAL != FISCAL ) 
					{//Este mensaje para impresora tm220 no es valido porque solo tiene receipt. 
					 //	Ver mas adelante si hay impresora fiscal que cuente con todas las estaciones.
						MENSAJE( ST( S_SELECCIONE_MODO_IMPRESION_1_JOURNAL_2_RECEIPT_3_AMBOS ) );
						while( !salir ) {
							k = GETCH();
							switch( k ) 
							{
								case '1':
									PRN_SELECT( RECEIPT );
									salir = 1;
									break;
								case '2':
									PRN_SELECT( JOURNAL );
									salir = 1;
									break;
								case '3':
									PRN_SELECT( RECEIPT + JOURNAL );
									salir = 1;
									break;
								case 27:
									salir = 1;
									reimprimir = 0;
									break;
							}
						}
						BORRAR_MENSAJE( );
						if( reimprimir ) 
						{
							REIMPRIMIR_TICKET_LOCAL("CIERRE_Z.prn");
						}

					} 
					else 
					{
						_IMPRIMIENDO_Z_ANT = 1;
						if( IMPRIMIR_Y_EN_Z )
							IMPRIME_INFORMES_Y_EN_Z( _z_ant );
						SET_MEMORY_CHAR( __nro_modo, 'Z' );
						INFORME_00( _z_ant, 0, SI );
						if( INF05_HABILITADO )
							INFORME_05( _z_ant );
						if( INF10_HABILITADO )	
							INFORME_10( _z_ant );
						if( INF15_HABILITADO )
							INFORME_15( _z_ant );
						if( INF_TAR_HABILITADO )
							INFORME_TARJETAS( _z_ant );
						//if( EMITIR_INFORME_DE_MUTUALES )
							//INFORME_MUTUALES( _z_ant );
						/*if( !corte_habilitado ) {
							corte_habilitado = 1;
							PRN_CORTE_TOTAL(  );
						};*/

						_IMPRIMIENDO_Z_ANT = 0;
					}
                    rta = 3;
                    seguir = 0;
					SET_MEMORY_INT( __ram_reimpresion, 0 );
                    break;
            }
        }
        TECLADO_ALFABETICO = 0;
    }
    else {
        MENSAJE_STRING( S_IMPRESORA_FUERA_LINEA );
        while( GETCH() != 27 );
    }
    /*---------------------------- fin ---------------------------------*/
    if( items != NULL ) {
        for( i = 0;i < y;i++ ) {
            free( items[i].nom );
        }
        free( items );
    }
    return ( rta );
}
/*****************************************************************************/
void IMPRIME_NRO_INFORME( char restar )
/*****************************************************************************/
{
    long nro;
	char cad_aux[50];

    LF( 1 );
    PRN_LETRA_GRANDE();
	if( NRO_MODO == 'X' ) {
        if( IMPRESORA_FISCAL ) {
			if( IMPRESORA_FISCAL != FISCAL ) {
				MOSTRAR_CADENA( 20, PRN, "X" );
			} else {
				memset(cad_aux,0,sizeof(cad_aux));
				_snprintf(cad_aux, sizeof(cad_aux)-1, "             X00000000 %ld", NRO_X );
				IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_RESALTADA, cad_aux, SI );
			}
        }
        else {
            MOSTRAR_CADENA( 11, PRN, "X" );
            MOSTRAR_ENTERO( 12, PRN, "00000000", NRO_X );
        }
        ADD_MEMORY_LONG( __nro_x, 1L );
    }
    else if( NRO_MODO == 'Y' ) {
        if( IMPRESORA_FISCAL == FISCAL_DITEMPO ) {
            MOSTRAR_CADENA( 11, PRN, "X1" );
            MOSTRAR_ENTERO( 12, PRN, "00000000", NRO_Y );
        }
        else if( IMPRESORA_FISCAL == FISCAL_EPSON ) {
            MOSTRAR_CADENA( 18, PRN, "X1" );
        }
        else {
			if( IMPRESORA_FISCAL != FISCAL ) {
				MOSTRAR_CADENA( 7, PRN, "Y" );
				MOSTRAR_ENTERO( 8, PRN, "00000000", NRO_Y );
			} else {
				memset(cad_aux,0,sizeof(cad_aux));
				_snprintf(cad_aux, sizeof(cad_aux)-1, "     Y00000000 %ld", NRO_Y );
				IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_DOBLE_ANCHO_NEGRITA, cad_aux, SI );
			}
        }
    }
    else {
		nro = ( restar ) ? NRO_Z - 1 : NRO_Z;
		if( IMPRESORA_FISCAL != FISCAL ) {
			MOSTRAR_CADENA( 11, PRN, "Z" );
			MOSTRAR_ENTERO( 12, PRN, "00000000", nro );
		} else {
			memset(cad_aux,0,sizeof(cad_aux));
			_snprintf(cad_aux, sizeof(cad_aux)-1, "             Z00000000 %ld", nro );
			IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_DOBLE_ANCHO_NEGRITA, cad_aux, SI );
		}
    }
    PRN_LETRA_NORMAL();
    if( !IMPRESORA_FISCAL ) {
        LF( 1 );
    }
    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}
