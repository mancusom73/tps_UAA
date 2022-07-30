#pragma pack (push,1) 
#ifndef ACTIONS_H
#define ACTIONS_H

#include "bridge.h"

#define A_ACTIVAR_PANEL_VENTAS             1
#define A_VENTA_MUESTRA_TOTAL              2
#define A_MUESTRA_ARTICULO                 3
#define A_PEDIR                            4
#define A_FIN_PEDIR                        5
#define A_PAGOS_INIT                       6
#define A_AGREGAR_MEDIO                    7
#define A_BORRAR_MEDIOS                    8
#define A_BORRAR_PAGO                      9
#define A_BORRAR_PAGOS                     10
#define A_PAGO_MUESTRA_TOTAL               12
#define A_DES_VENTAS_ACT_PAGOS             13
#define A_DES_PAGOS_ACT_VENTAS             14
#define A_INIT_VENTAS                      15
#define A_MUESTRA_RESTO                    16
#define A_MUESTRA_FLAGS                    17
#define A_MUESTRA_DATOS                    18
#define A_DES_VTAS_ACT_CIERRE              19
#define A_AGREGAR_MEDIO_CIERRE             20
#define A_MOSTRAR_IMPO_EN_CIERRE           21
#define A_REINTENT_ARQUEO_CIERRE           22
#define A_DES_CIERRE_ACT_PRESENT           23
#define A_DES_PRESEN_ACT_CIERRE            24
#define A_DES_CIERRE_ACT_VENTAS            25
#define A_MOSTRAR_ITEM_RE_CIERRE           26
#define A_RESALTAR_MEDIO_CIERRE            27
#define A_RESALTAR_SUBMEDIO_CIER           28
#define A_MOSTRAR_SUMA_EN_CIERRE           29
#define A_CEREAR_ITEM_RE_CIERRE            30
#define A_MOSTRAR_IMPO_SUB_CIERR           31
#define A_MOSTRAR_SUMA_SUB_CIERR           32
#define A_MENSAJE_PAGINA_CIERRE            33
#define A_MOSTRAR_HORA                     34
#define A_MOSTRAR_CAJA_CAJERO              35
#define A_ACTIVA_MODO_INVENTARIO           36
#define A_DESACT_MODO_INVENTARIO           37
#define A_PEDIR_CL_PRES                    38
#define A_FIN_PEDIR_CL_PRES                39
#define A_LIMPIA_CUADRO_PETICION           40
#define A_MUESTRA_BULTOS                   41
#define A_MUESTRA_TIT_BULTOS               42
#define A_MUESTRA_MODO                     43
#define A_MUESTRA_DESCRIPCION              44
#define A_MUESTRA_CANTIDAD                 45
#define A_MUESTRA_SUBTOTAL                 46
#define A_MUESTRA_MULTIPLICACION           47
#define A_DES_VENTAS_ACT_COBROS            48
#define A_DES_COBROS_ACT_VENTAS            49
#define A_DES_COBROS_ACT_PAGOS             50
#define A_MUESTRA_TIT_COMPROB_COBRO        51
#define A_MUESTRA_TIT_CUENTA_COBRO         52
#define A_MUESTRA_TIT_IMPORTE_COBRO        53
#define A_MUESTRA_DATO_COMPROB_COBRO       54
#define A_MUESTRA_DATO_CUENTA_COBRO        55
#define A_MUESTRA_DATO_IMPORTE_COBRO       56
#define A_MUESTRA_TIT_CANTIDAD_COBRO       57
#define A_MUESTRA_TIT_TOTAL_COBRO          58
#define A_MUESTRA_CANTIDAD_COBRO           59
#define A_MUESTRA_TOTAL_COBRO              60
#define A_MUESTRA_TIT_USUARIO_PRES         61
#define A_MUESTRA_TIT_CLAVE_PRES           62
#define A_MUESTRA_TIT_FECHA_PRES           63
#define A_DES_VENTAS_ACT_FFIJO             64
#define A_DES_FFIJO_ACT_VENTAS             65
#define A_DES_VENTAS_ACT_RETIROS           66
#define A_DES_RETIROS_ACT_VENTAS           67
#define A_MUESTRA_TITULO_FFIJO             68
#define A_MUESTRA_TITULO_RETIROS           69
#define A_MUESTRA_TIT_TOTAL_FFIJO          70
#define A_MUESTRA_TIT_TOTAL_PAGOS          71
#define A_PEDIR_DATO                       72
#define A_DES_VENTAS_ACT_STANDBY           73
#define A_DES_STANDBY_ACT_VENTAS           74
#define A_SET_TIT_STANDBY                  75
#define A_LIBRE                            76
#define A_MUESTRA_TITULO_BOX               77
#define A_RESALTAR_MEDIO_PP                78
#define A_DESELECCIONA_MEDIO               79
#define A_MUESTRA_MODO_COBROS              80
#define A_SET_PROP_INPUT                   81
#define A_ELIMINAR_ARTICULO                82
#define A_DES_VENTAS_ACT_CAMBIOMEDIO       83
#define A_DES_CAMBIOMEDIO_ACT_VENTAS       84
#define A_RESALTAR_MEDIO_PR                85
#define A_RESALTAR_MEDIO_PFF               86
#define A_RESALTAR_MEDIO_PCM               87
#define A_SCROLL_TICKET                    88
#define A_DES_VENTAS_ACT_PAGOS_CAJA        89
#define A_MUESTRA_DATO_COMPROB_PAGOS_CAJA  90
#define A_MUESTRA_DATO_CUENTA_PAGOS_CAJA   91
#define A_MUESTRA_DATO_IMPORTE_PAGOS_CAJA  92
#define A_MUESTRA_DATO_GRAVADO_PAGOS_CAJA  93
#define A_MUESTRA_DATO_EXENTO_PAGOS_CAJA   94
#define A_MUESTRA_DATO_IVA_PAGOS_CAJA      95
#define A_MUESTRA_MODO_PAGOS_CAJA          96
#define A_MUESTRA_TIT_COMPROB_PAGOS_CAJA   97
#define A_MUESTRA_TIT_CUENTA_PAGOS_CAJA    98
#define A_MUESTRA_TIT_IMPORTE_PAGOS_CAJA   99
#define A_MUESTRA_TIT_GRAVADO_PAGOS_CAJA   100
#define A_MUESTRA_TIT_EXENTO_PAGOS_CAJA    101
#define A_MUESTRA_TIT_IVA_PAGOS_CAJA       102
#define A_DES_PAGOS_CAJA_ACT_VENTAS        103
#define A_OCULTA_DISCRIMINA_PAGOS_CAJA     104
#define A_MUESTRA_DISCRIMINA_PAGOS_CAJA    105
#define A_MOSTRAR_GRILLA                   106
#define A_AGREGAR_FILA_GRILLA              107
#define A_SELECCION_FILA_GRILLA            108
#define A_FIN_GRILLA                       109

#define A_ACTIVAR_PANEL_COBROS_ESPECIALES  110
#define AGREGAR_UNIDAD_DE_NEGOCIO          111
#define A_SCROLL_UNIDAD_NEGOCIO            112
#define A_MUESTRA_DATO_CODIGO_BARRA        113
#define A_MUESTRA_DATO_UNIDAD_NEGOCIO      114
#define A_MUESTRA_DATO_CLIENTE             115
#define A_SCROLL_DEUDA_SOCIO               116
#define A_ACTIVAR_PANEL_DEUDA_SOCIO        117
#define A_DESACTIVAR_PANEL_DEUDA_SOCIO     118
#define A_MUESTRA_DATO_SOCIO_DEUDA         119
#define A_MUESTRA_DATO_SOCIO_COBRO         120
#define A_MUESTRA_SUBTOTAL_COBRO_ESP       121
#define A_LIMPIA_PANEL_COBROS_ESP          122
#define A_DES_PAGOS_ACT_COBROS_ESP         123
#define A_MOSTRAR_FOTO_PRES                124
#define A_DES_COBROS_ESP_ACT_PAGOS_ESP     125
#define A_DES_PAGOS_ESP_ACT_COBROS_ESP     126
#define A_MUESTRA_MODO_PAGOS_ESP           127
#define A_MUESTRA_TIT_PAGOS_ESP            128
#define A_MUESTRA_SOCIO_PAGOS_ESP          129
#define A_MUESTRA_REINT_PAGOS_ESP          130
#define A_MUESTRA_IMPORTE_PAGOS_ESP        131
#define A_MUESTRA_MEDIO_PAGOS_ESP          132
#define A_MUESTRA_SUBMEDIO_PAGOS_ESP       133
#define A_MUESTRA_CHEQUE_PAGOS_ESP         134
#define A_MUESTRA_BANCO_PAGOS_ESP          135

#define A_DES_COBROSESP_ACT_STANDBY        136
#define A_DES_STANDBY_ACT_COBROSESP        137

#define A_DES_COBROSESP_ACT_MEDIOREINT     138
#define A_DES_MEDIOREINT_ACT_COBROSESP     139
#define A_SELEC_FILA_MED_REINT             140
#define A_AGREGAR_MEDIO_REINTEGRO          141
#define A_AGREGAR_MEDIO_REINTEGRO_ING      142
#define A_QUITAR_MEDIO_REINTEGRO           143
#define A_SELEC_FILA_MED_ING               144
#define A_QUITAR_FILA_MED_DISP             145
#define A_GET_DATA_MEDREIN_ING             146
#define A_GET_DATA_MEDREIN_DISP            147
#define A_OCULTAR_PAN_PEDIR                148
#define A_SET_INPUT_TEXT                   149
#define A_MOSTRAR_OP_MULT                  150
#define A_OCULTAR_OP_MULT                  151
#define A_QUITAR_FILAS_REINTEGROS          152
#define A_ACTIVAR_PANEL_CHEQUES            153
#define A_DESACTIVAR_PANEL_CHEQUES         154
#define A_SCROLL_CHEQUES                   155
#define A_REFRESH_PAGOS                    156
#define A_BORRA_DIF_SUBMEDIOS              157
#define A_OCULTA_DIF_SUBMEDIO              158

#define A_CAMBIO_PANTALLA_TOUCH            159
#define A_MUESTRA_TIT_TOTAL_RETIROS        160
#define A_MUESTRA_IMG_ART                  161
#define A_MUESTRA_TITULO_BROWSE            162

#define A_MUESTRA_ICON_COBRO               163
#define A_ACTIVAR_PANEL_VUELTO             164
#define A_SCROLL_VUELTO                    165
#define A_DESACTIVAR_PANEL_VUELTO          166
#define A_ACTIVAR_PANEL_PAGATODO		   167
#define A_DESACTIVAR_PANEL_PAGATODO        168
#define A_MUESTRA_DATO_CODIGO_BARRA_PT     169
#define A_MUESTRA_DATO_IMPORTE_PT          170
#define A_ACTIVAR_PANEL_GARANTIA		   171
#define A_DESACTIVAR_PANEL_GARANTIA        172
#define A_MUESTRA_DATO_FOLIO_G			   173
#define A_MUESTRA_COD_ARTICULO_G           174
#define A_MUESTRA_DATO_IMPORTE_G           175
#define A_MUESTRA_TIT_COD_FOLIO_G		   176
#define A_MUESTRA_TIT_COD_ARTICULO_G       177
#define A_MUESTRA_TIT_IMPORTE_G            178
#define A_MUESTRA_TIT_IMPORTE_PAGATODO     179
#define A_MUESTRA_FECHA_PRES               180
#define A_MUESTRA_COBRO_IMPORTE_ME		   181
#define A_REBOOT_SYSTEM					   182
#define A_MOSTRAR_CAJA_CAJERO_PRESENT      183
#define A_MUESTRA_DESCRIPCION_CONTADOR	   184	
#define A_MUESTRA_CONTADOR_DIFERENCIAL	   185	
#define A_MUESTRA_CONTADOR_TOTAL_IDEN	   186	
#define A_ACTIVAR_PANEL_DENOMINACIONES     187
#define A_SCROLL_DENOMINACIONES            188
#define A_DESACTIVAR_PANEL_DENOMINACIONES  189
#define A_SUBTOTAL_DENOMINACIONES		   190
#define A_PAGO_MUESTRA_IMPORTE_TEMPORAL    191
#define A_GET_PANEL_ACTIVO	               192
#define A_MUESTRA_TIT_COD_BARRA_COBRO      193
#define A_ACTIVAR_PANEL_CONSULTA_PROMOS	   194
#define A_DESACTIVAR_PANEL_CONSULTA_PROMOS 195
#define A_SCROLL_PROMOS	                   196
#define A_MUESTRA_DATO_COD_BARRA_PROMO     197
#define A_SCROLL_IMPORTACION               198
#define A_ACTIVAR_PANEL_IMPORTACION        199
#define A_DESACTIVAR_PANEL_IMPORTACION     200 
#define A_BATCH_TICKET                     201
#define A_MUESTRA_FLAG_CAMBIO_MEDIO        202
#define A_MUESTRA_FECHA_OP_VENTA           203
#define A_SHUTDOWN_SYSTEM                  204
#define A_DES_TODOS_ACT_VENTAS             205
#define A_MUESTRA_FECHA_SISTEMA            206
#define A_MUESTRA_PANTALLA_OFFLINE         207
#define A_OCULTA_PANTALLA_OFFLINE          208
#define A_MUESTRA_MEMORIA		           209
#define A_PAGOS_FOCUS			           213
#define A_VENTAS_FOCUS			           214
#define A_DES_BROWSE					   215
#define A_DES_TECLADO_ALFABETICO		   216
#define A_DES_WIN_ON_TOP			       217
#define A_FOCO_A_PANEL_ACTIVO			   218

#define ACTION_IS_SCANNER               0
#define ACTION_READ_DATA_SCANNER        1
#define ACTION_READ_KEYBOARD            2
#define ACTION_IS_MSR                   3
#define ACTION_READ_DATA_MSR_TRACK1     4
#define ACTION_READ                     5
#define ACTION_SET_DATA_PRINTER         6
#define ACTION_HABILITA_SCANNER         9
#define ACTION_DRAWER_OPENED            10
#define ACTION_OPEN_DRAWER              11
#define ACTION_WAIT_FOR_DRAWER_CLOSE    12
#define ACTION_READ_DATA_MSR_TRACK2     13
#define ACTION_PRINT                    14
#define ACTION_SET_PRINT_POS            15
#define ACTION_PRINT_LF                 16
#define ACTION_PRINT_CUT_PAPER          17
#define ACTION_PRINT_MODE               18
#define ACTION_SET_PRINT_STATION        19
#define ACTION_SET_DISPLAY_TEXT_LINE_1  20
#define ACTION_SET_DISPLAY_TEXT_LINE_2  22
#define ACTION_SET_DISPLAY_CLEAR_TEXT   21

#define ACTION_PRINT_CENTER             100
#define ACTION_PRINT_NORMAL             101
#define ACTION_HABILITA_MSR             23
#define ACTION_READ_PRINTER_STATUS      24

#define ACTION_SLP_EMPTY                25
#define ACTION_PRINT_FINISH             26
#define ACTION_READ_WEIGHT              27
#define ACTION_HABILITA_SCALE           28

#define ACTION_BEGIN_FISCAL_REC         29
#define ACTION_END_FISCAL_REC           30
#define ACTION_PRINT_REC_ITEM           31
#define ACTION_PRINT_REC_SUBTOTAL       32
#define ACTION_PRINT_REC_TOTAL          33
#define ACTION_PRINT_X_REPORT           34
#define ACTION_PRINT_Z_REPORT           35
#define ACTION_BEGIN_NON_FISCAL         36
#define ACTION_END_NON_FISCAL           37
#define ACTION_PRINT_REC_VOID           38
#define ACTION_GET_FISCAL_STATE         39
#define ACTION_GET_TERMINAL_NUM         40
#define ACTION_GET_Z_NUM                41
#define ACTION_PRINT_REC_VOID_ITEM      42
#define ACTION_PRINT_REC_ADJUST_ITEM    43
#define ACTION_SET_TRAILER_LINE         44
#define ACTION_SET_HEADER_LINE          45
#define ACTION_SET_POSITION             46
#define ACTION_DOPRINT                  47
#define ACTION_NORMAL_LETTER            48 //BORRAR(ignorada por JavaServer) 
#define ACTION_BIG_LETTER               49
#define ACTION_SMALL_LETTER             50
#define ACTION_PRINT_REC_MESSAGE        51
#define ACTION_IMAGE                    52
#define ACTION_TIMBRADORA               53
#define ACTION_SET_FONT                 54
#define ACTION_GETCH_KEY                55
#define ACTION_SET_TAM_IMAGE            56
#define ACTION_KEYLOCK_WAIT_POSITION    57
#define ACTION_KEYLOCK_GET_POSITION     58
#define ACTION_KEYLOCK_POSITION_COUNT   59
#define ACTION_PRINT_BAR_CODE           60
#define ACTION_SET_PRINT_BAR_CODE       61
#define ACTION_PRINT_BITMAP             62
#define ACTION_SET_PRINT_BITMAP         63
#define ACTION_GET_CHARSET              64
#define ACTION_PRINT_EXISTE_POS_PRINTER 65
#define ACTION_CLEAN_BUFFER_SCANNER     66
#define ACTION_SET_MODO_IMPRESION_FINAL  67
#define ACTION_SET_MODO_IMPRESION_NORMAL 68
#define ACTION_FORCE_PRINT               69
#define ACTION_DETECT_ERROR_PRINTER      70
#define ACTION_SET_PRINT_LINE_CHARS      71
#define ACTION_SET_PRINT_LINE_SPACE      72
#define ACTION_GET_PRINT_STATION         73
#define ACTION_GET_PRINT_LETTER          74
#define ACTION_SET_HEADER				 75
#define ACTION_PRINT_VOUCHER			 76 //Seba
#define ACTION_FEED_PAPER				 77 //Seba
#define ACTION_PRINT_NORMAL_FISCAL       78 //Seba
#define ACTION_PRINT_REC_REFUND			 79 //Seba
#define ACTION_PRINT_REC_ITEM_FAC		 80 //Seba
#define ACTION_PRINT_REC_ITEM_VOID_FAC	 81 //Seba
#define ACTION_BEGIN_FAC				 82 //Seba
#define ACTION_BEGIN_REPAYMENT_NOTE		 83 //Seba
#define ACTION_END_FAC					 84 //Seba
#define ACTION_END_REPAYMENT_NOTE		 85 //Seba
#define ACTION_PRINT_REC_TOTAL_FAC		 86 //Seba
#define ACTION_PRINT_REC_VOID_FAC		 87 //Seba
#define ACTION_PRINT_REC_SUBTOTAL_FAC	 88 //Seba
#define ACTION_PRINT_REC_VOID_ADJUST_ITEM		89 //Seba
#define ACTION_PRINT_REC_VOID_ITEM_FAC			90 //Seba
#define ACTION_GET_FISCAL_ACUM					91 
#define ACTION_CLEAR_PRINTER					92
#define ACTION_PRINT_REC_BON_REC_FAC            93 //no utilizado
#define ACTION_PRINT_BONIF_ITEM_FAC				94 //Seba
#define ACTION_PRINT_ANUL_BONIF_ITEM_FAC		95 //Seba
#define ACTION_PRINT_PERCEP_RETEN				96 //Seba
#define ACTION_FISCAL_OPEN_CASHDRAWER		    97
#define ACTION_FISCAL_ESTABLECER_FECHA_HORA     98
#define ACTION_FISCAL_OBTENER_FECHA_HORA        99
#define ACTION_UPDATE_ESTADO_DOC_NO_FISCAL      209
#define ACTION_SET_PRINTER_PREF					210//dnc nuevos comandos
#define ACTION_SET_PAPER_PREF					211
#define ACTION_SET_COMPR_PREF			        212
//****PINDPAD
#define ACTION_PINPAD_MENSAJES_ROT				213
#define ACTION_PINPAD_VERSION					214
#define ACTION_PINPAD_PEDIR_PIN					215
#define ACTION_PINPAD_GET_KEY					216	
#define ACTION_PINPAD_GET_STRING				217
#define ACTION_PINPAD_ONLINE					218
#define ACTION_PINPAD_MOSTRAR					219
#define ACTION_PINPAD_SELECT_MK					220
#define ACTION_PINPAD_CANCEL_SESSION			221
#define ACTION_PINPAD_TIMEOUT					222
#define ACTION_PINPAD_TRANSFER_MK				223

#endif
#pragma pack (pop) 
