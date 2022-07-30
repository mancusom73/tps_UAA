#ifndef DRIVER_H
#define DRIVER_H
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

//#include "drv_func.h"

/*---------------- Codigos de los drivers ----------------*/
#define _CANT_DRIVERS        20
#define _DRV_inicializacion   0
#define _DRV_fin              1
#define _DRV_printer          2
#define _DRV_scanner          3
#define _DRV_lector_banda     4
#define _DRV_cajon            5
#define _DRV_llave            6
#define _DRV_teclado          7
#define _DRV_display          8
#define _DRV_rnv              9
#define _DRV_pinpad          10
#define _DRV_tcp             11
#define _DRV_balanza         12
#define _DRV_btrieve         13
#define _DRV_power           14
#define _DRV_icc             15
#define _DRV_lector_cheque   16

/*---------------- Subfunciones fijas ---------------*/
#define _VERSION  2

/*---------------- Subfunciones para la IMPRESORA -----------------*/
#define _PRN_inicializacion              0
#define _PRN_fin                         1
#define _PRN_version                     2
#define _PRN_status                      3
#define _PRN_enviar_byte                 4
#define _PRN_cr                          5
#define _PRN_lf                          6
#define _PRN_enviar_cadena               7
#define _PRN_reintentar                  8
#define _PRN_sincronizar                 9
#define _PRN_leer                       80

#define _PRN_base_funciones             10
#define _PRN_inicializar                10
#define _PRN_corte_total                11
#define _PRN_continua                   12
#define _PRN_discontinua                13
#define _PRN_letra_normal               14
#define _PRN_letra_chica                15
#define _PRN_letra_grande               16
#define _PRN_habilita_deteccion_papel   17
#define _PRN_select_receipt_y_journal   18
#define _PRN_select_receipt             19
#define _PRN_select_journal             20
#define _PRN_habilita_slip              21
#define _PRN_inhabilita_slip            22
#define _PRN_set_espaciado              23
#define _PRN_espaciado_normal           24
#define _PRN_feed                       25
#define _PRN_select                     26
#define _PRN_select_para_espaciado      27
#define _PRN_inicializar_prn_opcional   28
#define _PRN_inicializar_ticket         29
#define _PRN_2pasadas                   30
#define _PRN_micro_lf                   31
#define _PRN_habilitar_cheque           32
#define _PRN_hay_datos                  33
#define _PRN_leer_datos                 34
#define _PRN_leer_cheque                35

/*----------------------------- Status --------------------------*/
#define _PRN_online         1
#define _PRN_paper_out      2
#define _PRN_slip_present   4
#define _PRN_buffers_empty  8
/*----------------------------- Status Fiscal--------------------*/
#define _FISCAL_estado_inicial          9
#define _FISCAL_ok                      1
#define _FISCAL_offline                 0
#define _FISCAL_paper_out              -1
#define _FISCAL_printer_error          -2
#define _FISCAL_doc_fiscal_abierto     -3
#define _FISCAL_doc_no_fiscal_abierto  -4
#define _FISCAL_nec_cierre             -5
#define _FISCAL_mem_llena              -6
#define _FISCAL_desborde_totales       -7
#define _FISCAL_comando_invalido       -8
#define _FISCAL_paper_near_end         -9
#define _FISCAL_mem_casi_llena         -10
#define _FISCAL_cajon_abierto          -11
/*agregar al medio si hace falta subirlo maximo 127 */
#define _FISCAL_estado_final           -20


/*---------------- Subfunciones para SCANNER -----------------*/
#define _SCA_inicializacion              0
#define _SCA_fin                         1
#define _SCA_version                     2
#define _SCA_status                      3
#define _SCA_leer_codigo                 4


/*--------- Subfunciones para LECTOR BANDA MAGNETICA ----------*/
#define _LBM_inicializacion              0
#define _LBM_fin                         1
#define _LBM_version                     2
#define _LBM_status                      3
#define _LBM_leer_banda                  4
#define _LBM_habilitar_lectura           5
#define _LBM_inhabilitar_lectura         6


/*---------------- Subfunciones para CAJON --------------------*/
#define _CAJ_inicializacion              0
#define _CAJ_fin                         1
#define _CAJ_version                     2
#define _CAJ_status                      3
#define _CAJ_abrir_cajon                 4
//---------- Comandos configurrables del cajon
#define _CAJON_inicializar               0
#define _CAJON_status                    1
#define _CAJON_abrir                     2


/*---------------- Subfunciones para LLAVE --------------------*/
#define _LLA_inicializacion              0
#define _LLA_fin                         1
#define _LLA_version                     2
#define _LLA_status                      3
#define _LLA_id_posiciones               4
//--------- Posiciones de la llave
#define _LLAVE_pos_programacion  1
#define _LLAVE_pos_off           2
#define _LLAVE_pos_normal        4
#define _LLAVE_pos_supervisor    8
#define _LLAVE_pos_x            16
#define _LLAVE_pos_z            32


/*---------------- Subfunciones para TECLADO --------------------*/
#define _TEC_inicializacion              0
#define _TEC_fin                         1
#define _TEC_version                     2
#define _TEC_status                      3
#define _TEC_pedir_tecla                 4
//--------- Modos del teclado
#define _TECLADO_REGISTRADORA  0
#define _TECLADO_ALFABETICO    1
#define _TECLADO_PROG_EXTERNOS 2


/*-------------- Subfunciones para DISPLAY ---------------*/
#define _DSP_inicializacion              0
#define _DSP_fin                         1
#define _DSP_version                     2
#define _DSP_setxy                       4
#define _DSP_display                     5
#define _DSP_borrar_linea                6
#define _DSP_borrar_display              7
//---------- Comandos configurrables del display
#define _DISPLAY_inicializar    0
#define _DISPLAY_setxy          1
#define _DISPLAY_borrar_linea   2
#define _DISPLAY_borrar_display 3
#define _DISPLAY_on             4
#define _DISPLAY_off            5
//---------- Tipos de DISPLAY
#define _DISPLAY_cliente    0x80
#define _DISPLAY_operador   0x40
#define _DISPLAY_cli_y_ope  0xC0


/*-------------- Subfunciones para RNV ---------------*/
#define _RNV_inicializacion              0
#define _RNV_fin                         1
#define _RNV_version                     2
#define _RNV_configuracion               3
#define _RNV_leer                        4
#define _RNV_escribir                    5
#define _RNV_cerear                      6
#define _RNV_set                         7

/*-------------- Subfunciones para PINPAD ---------------*/
#define _PIN_inicializacion              0
#define _PIN_fin                         1
#define _PIN_version                     2
#define _PIN_mostrar                     3
#define _PIN_pedir_pin                   4
#define _PIN_select_master_key           5
#define _PIN_cancel_session              6
#define _PIN_getkey                      7
#define _PIN_set_timeout                 8
#define _PIN_mensajes_rotativos          9
#define _PIN_getstring                  10
#define _PIN_transfer_master_key        11
#define _PIN_verificar_online           12


/*-------------- Subfunciones para TCP ---------------*/
#define _TCP_inicializacion              0
#define _TCP_fin                         1
#define _TCP_version                     2
#define _TCP_abrir_canal                 3
#define _TCP_cerrar_canal                4
#define _TCP_enviar                      5
#define _TCP_recibir                     6
#define _TCP_status                      7
#define _TCP_abrir_socket_ping           8
#define _TCP_ping                        9
#define _TCP_close_socket_ping          10
#define _FTP_put                        11
#define _FTP_get                        12


/*---------------- Subfunciones para la BALANZA -----------------*/
#define _BAL_inicializacion              0
#define _BAL_fin                         1
#define _BAL_version                     2
#define _BAL_leer_balanza                3

/*---------------- Subfunciones para BTRIEVE -----------------*/
#define _BTR_inicializacion              0
#define _BTR_fin                         1
#define _BTR_version                     2
#define _BTR_funcion                     3

/*---------------- Subfunciones para POWER -----------------*/
#define _PWR_inicializacion              0
#define _PWR_fin                         1
#define _PWR_version                     2
#define _PWR_status                      3
#define _PWR_down                        4

/*---------------- Subfunciones para TARJETA CHIP ----------------*/
#define _ICC_inicializacion              0
#define _ICC_fin                         1
#define _ICC_version                     2
#define _ICC_open_channel                3
#define _ICC_open_session                4
#define _ICC_close_session               5
#define _ICC_select_df                   6
#define _ICC_select_ef                   7
#define _ICC_status_tarjeta              8
#define _ICC_leer_archivo                9
#define _ICC_transaccion                10
#define _ICC_verificar_transaccion      11
#define _ICC_debito_monedero            12
#define _ICC_credito_monedero           13
#define _ICC_card_serial_number         14
void DRIVER( int funcion, int subfuncion, int bx, void *valor, int longitud );
void INIT_DRIVERS_PRIMARIOS();
void INIT_DRIVERS_SECUNDARIOS();
#endif
