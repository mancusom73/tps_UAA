#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cmemory.h> 
#include <consulta.h> 
#include <cr.h>  
#include <cr_pant.h> 
#include <c_ticket.h> 
#include <getch.h> 
#include <m_caract.h> 
#include <pant.h> 
#include <perfiles.h> 
#include <round.h> 
#include <scanner.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W) || defined(INVEL_L)
#include <actions.h>
#endif
/*****************************************************************************/
void CONSULTA_TICKET()
/*****************************************************************************/
{
    int seguir = 1, mostrar = 1, linea, item, k, h, i = 0;
    clock_t timex = clock();

    buscar_articulo_en_mostrar_item = 1;
    /*----------- Edicion -------------*/
    linea = 9;
    item = RAM_P_TICKET - 1;
    #if defined(INVEL_W) || defined(INVEL_L)
    ExecuteSAction( A_SCROLL_TICKET, "0" );// 0 habilita
    #endif
    
        //if( mostrar ) {
        //    mostrar = 0;
        //    /*----------- flechitas -----------*/
        //    _ATRIBUTO = ( item > 0 ) ? 143 : 15;
        //    MOSTRAR_CARACTER( 80 - ancho_panel, 8, 30 );
        //    _ATRIBUTO = ( item < RAM_P_TICKET - 1 ) ? 143 : 15;
        //    MOSTRAR_CARACTER( 80 - ancho_panel, 17, 31 );
        //    /*----------- item --------------*/
        //    _ATRIBUTO = 7;

            #if defined(INVEL_W) || defined(INVEL_L)
            CR1_MOSTRAR_ITEM_WIN( item );
            #else
            CR1_MOSTRAR_ITEM( item, linea, NO  );
            #endif
        //}
    while( seguir ) {
        k = FAST_GETCH();
        //k = GETCH();

        switch( k ) {
            case -72:
                /*-------- FLECHA ARRIBA ---------*/
                if( item > 0 ) {
                    item--;
                    mostrar = 1;
                    if( linea > 0 ) {
                        linea--;
                    }
                    else {
                        //SCROLL( 2, 8, 78 - ancho_panel, 16, 1 );
                    }
                } else {
                    break;
                }
                //#if defined(INVEL_W) || defined(INVEL_L)
                /*ExecuteSAction( A_BATCH_TICKET, "begin" );
                ExecuteSAction( A_SCROLL_TICKET, "-1" );
                ExecuteSAction( A_BATCH_TICKET, "end" );*/
                //#endif
				ExecuteSAction( A_SCROLL_TICKET, "-1" );
                timex = clock();
                break;
            case -73:
                /*-------- PAGINA ARRIBA ---------*/
                if( item > 0 ) {
                    ExecuteSAction( A_BATCH_TICKET, "begin" );
                    item -= 10;
                    if( item < 0 ) {
                        item = 0;
                        linea = ( RAM_P_TICKET > 10 ) ? 0 : 10 - RAM_P_TICKET;
                    }
                    mostrar = 1;
                    /*---------- llena la ventana ---------------*/
                    _ATRIBUTO = 103;
                    if( item < 9 && RAM_P_TICKET < 10 ) {
                        h = RAM_P_TICKET - 1;
                    }
                    else if( item < 9 ) {
                        h = 9;
                        linea = item;
                    }
                    else {
                        h = item;
                    }
                    if( h >= 0 ) {
                        CR1_MOSTRAR_ITEM_WIN( item - 10 );
                    }
                    ExecuteSAction( A_SCROLL_TICKET, "-10" );
                    timex = clock();
                }
                break;
            case -80:
                /*-------- FLECHA ABAJO ---------*/
                if( item < RAM_P_TICKET - 1 ) {
                    item++;
                    mostrar = 1;
                    if( linea < 9 ) {
                        linea++;
                    }
                    else {
                        //SCROLL( 2, 9, 78 - ancho_panel, 17, -1 );
                    }
                }else{
                    break;
                }

                //#if defined(INVEL_W) || defined(INVEL_L)
                /*ExecuteSAction( A_BATCH_TICKET, "begin" );
                ExecuteSAction( A_SCROLL_TICKET, "1" );
                ExecuteSAction( A_BATCH_TICKET, "end" );*/
                //#endif
				ExecuteSAction( A_SCROLL_TICKET, "1" );
                timex = clock();
                break;
            case -81:
                /*-------- PAGINA ABAJO ---------*/
                if( item < RAM_P_TICKET - 1 ) {
                    ExecuteSAction( A_BATCH_TICKET, "begin" );
                    item += 10;
                    if( item >= RAM_P_TICKET ) {
                        item = RAM_P_TICKET - 1;
                        linea = 9;
                    }
                    mostrar = 1;
                    /*---------- llena la ventana ---------------*/
                    _ATRIBUTO = 103;
                    h = item + 9 - linea;
                    if( RAM_P_TICKET < 10 ) {
                        h = item;
                    }
                    else if( h >= RAM_P_TICKET ) {
                        h = RAM_P_TICKET - 2;
                        linea = 9;
                    }
                    if( h >= 0 ) {
                        CR1_MOSTRAR_ITEM_WIN( item + 10 );
                    }
                    ExecuteSAction( A_SCROLL_TICKET, "10" );
                    timex = clock();
                }
                break;
            case 27:
                /*-------- ESC ---------*/
                RECUPERA_VENTANA_TICKET();
                seguir = 0;
                ExecuteSAction( A_SCROLL_TICKET, "-99" );//-99 deshabilita  
                break;
            case -999:
                ExecuteSAction( A_BATCH_TICKET, "end" );
                if( mostrar && ( clock() - timex ) > 200 ) {
                    CR1_MOSTRAR_ITEM_WIN( item );
                    mostrar = 0;
                }
                break;
        }
    }
    ExecuteSAction( A_BATCH_TICKET, "end" );
    buscar_articulo_en_mostrar_item = 0;
}

/*****************************************************************************/
void RECUPERA_VENTANA_TICKET()
/*****************************************************************************/
{
    int h;
    //#if defined(INVEL_W) || defined(INVEL_L)
    //#else
    /* int h;
     _ATRIBUTO = 103;
     for( h = 0;h < 10 && h < RAM_P_TICKET;h++ ) {
         SET_MEMORY_DOUBLE( __ram_cantidad_original,
                            ROUND( _TICKET_CANTIDAD( RAM_P_TICKET - 1 - h ), 5, 3 ) );
         CR1_MOSTRAR_ITEM( RAM_P_TICKET - 1 - h, 9 - h );
     }
     _ATRIBUTO = 7;
     if( RAM_P_TICKET ) {
         SET_MEMORY_DOUBLE( __ram_cantidad_original,
                            ROUND( _TICKET_CANTIDAD( RAM_P_TICKET - 1 ), 5, 3 ) );
         CR1_MOSTRAR_ITEM( RAM_P_TICKET - 1, 9 );
     }*/
    //#endif
   if( RAM_PERFIL && UTILIZA_PROMOCIONES && !RAM_NOTA_CR 
		&& !UTILIZA_MODULO_PROMOCIONES ) {
        if( BORRAR_PERFIL_AL_SALIR_DE_PAGOS && !TOMAR_PERFIL_DESDE_CLIENTE ) {
            SET_MEMORY_INT( __ram_perfil, 0 );
            MUESTRA_PERFIL();
        }
        ExecuteAction( A_INIT_VENTAS );
        _ATRIBUTO = 7;
        for( h = RAM_P_TICKET ;h > 0;h-- ) {
			SET_MEMORY_DOUBLE( __ram_cantidad_original,
                           ROUND( _TICKET_CANTIDAD( h - 1 ), 5, 3 ) );
            MUESTRA_ITEM_EN_PANTALLA( h - 1 );
        }
    }
}

