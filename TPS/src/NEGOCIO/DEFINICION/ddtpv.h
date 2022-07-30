/****************************************************************************/
/* DICCIONARIO DE DATOS DE LA TPV 1000                                      */
/* FECHA : 22/01/13                                                         */
/****************************************************************************/
#define CANTIDAD_VARIABLES 1964
#define VERSION_DICCIONARIO "DD_RE_22_01_20013_1964"


/****************************************************************************/
/*   Definiciones de Estructuras                                            */ 
/****************************************************************************/

/*---------------------- Estructuras RAM no vol til ----------------------*/
#define _varios                1
#define _var_tarjetas          2
#define _var_tarjetas_ant      3
#define _pago                  4
#define _nro                   5
#define _x                     6
#define _z                     7
#define _z_ant                 8
#define _ram                   9
#define _tot_tick              10
#define _tot_tick_ant          11
#define _tot_tick_sv           12
#define _item                  13
#define _stack                 15
#define _mutual                17
#define _conceptos_a_pagar     51
#define _x_cajero              55
#define _sitef                 64

/*--------------- Estructuras Configurables por el Usuario ---------------*/
#define _especiales            16
#define _hardware              19
#define _memoria               20
#define _impresora             21
#define _teclado               22
#define _torreta               23
#define _scanner               24
#define _balanza               25
#define _cajon                 26
#define _lector_banda          27
#define _display               28
#define _llave                 29
#define _pinpad                30
#define _salva_pantalla        31
#define _maq_recepc_envases    32
#define _formatos              33
#define _usuarios              34
#define _control_y_seguridad   35
#define _facturacion           36
#define _remitos_guias         37
#define _para_cooperativas     38
#define _tarjetas_de_credito   39
#define _cheques               40
#define _cuenta_corriente      41
#define _mutuales              42
#define _informes              43
#define _comprobantes          44
#define _redondeo_de_precios   45
#define _presentaciones        46
#define _promociones           47
#define _credito               48
#define _generacion_archivos   49
#define _miscelaneas           50
#define _articulos             52
#define _pedidos               53
#define _comunicaciones        54
#define _fondo_fijo            56
#define _retiros               57
#define _cliente_afinidad      58
#define _donaciones            59
#define _autorizador           60
#define _medios_de_pago        61
#define _devolucion            62
#define _ticket_en_espera      63
#define _vendedores            65
#define _descuentos            66
#define _datafono              67
#define _negocio               68
#define _jgateway              69

/*------------------ Estructuras Internas Inicializadas ------------------*/

/*---------------- Estructuras Internas no Inicializadas ----------------*/
#define _ext_impuestos         14
#define _ticket                18


/****************************************************************************/
/*   Definiciones de Variables                                              */ 
/****************************************************************************/

/*--------------------- Variables en RAM no vol til ---------------------*/
#define __nro_caja                             1
#define __nro_nom_caja                         2
#define __nro_intereses                        3
#define __nro_x                                4
#define __nro_z                                5
#define __nro_ticket                           6
#define __nro_cajero                           7
#define __nro_cajero_ant                       8
#define __nro_variacion_precio                 9
#define __nro_modo                             10
#define __nro_y                                11
#define __nro_vendedor                         12
#define __nro_fecha_operacion                  13
#define __nro_dia_operativo                    14
#define __nro_hubo_operaciones                 15
#define __nro_cupones                          16
#define __nro_nro_retiro                       17
#define __nro_fecha_z                          18
#define __nro_ultimo_ticket_ant                19
#define __nro_gran_total                       20
#define __nro_recargo_financiacion             21
#define __nro_cod_cuota                        22
#define __nro_z_emitida                        23
#define __nro_cant_tickets_ant                 24
#define __nro_total_bonos                      25
#define __nro_ultima_alta_procesada            26
#define __nro_ultima_modif_procesada           27
#define __nro_comp_cta_cte                     28
#define __nro_remito                           29
#define __nro_rollo                            30
#define __nro_mozo                             31
#define __var_tarjetas_nro_comercio            32
#define __var_tarjetas_cod_servicio            33
#define __var_tarjetas_unicamente_ol           34
#define __var_tarjetas_necesita_pin            35
#define __var_tarjetas_pedir_auto              36
#define __var_tarjetas_lote                    37
#define __var_tarjetas_captura_on_line         38
#define __var_tarjetas_coeficiente             39
#define __var_tarjetas_aut_on_line             40
#define __var_tarjetas_ant_tarjeta             41
#define __var_tarjetas_ant_numero              42
#define __var_tarjetas_ant_fecha_vto           43
#define __var_tarjetas_ant_limite              44
#define __var_tarjetas_ant_cuotas              45
#define __var_tarjetas_ant_financia_tarjeta    46
#define __var_tarjetas_ant_autorizacion        47
#define __var_tarjetas_ant_nro_cupon           48
#define __var_tarjetas_ant_nro_cupon_int       49
#define __var_tarjetas_ant_lectura_banda       50
#define __var_tarjetas_ant_boletin_vencido     51
#define __var_tarjetas_ant_recargo             52
#define __var_tarjetas_ant_nro_boletin         53
#define __var_tarjetas_ant_tabla               54
#define __var_tarjetas_ant_on_line             55
#define __var_tarjetas_ant_adherente           56
#define __var_tarjetas_ant_anticipo            57
#define __var_tarjetas_ant_saldo_cliente       58
#define __var_tarjetas_ant_importe_cuota       59
#define __var_tarjetas_ant_primer_vto          60
#define __var_tarjetas_ant_moneda              61
#define __var_tarjetas_ant_importe_entrega     62
#define __var_tarjetas_ant_porc_entrega        63
#define __var_tarjetas_ant_nro_ref             64
#define __var_tarjetas_ant_aut_en_boletin      65
#define __var_tarjetas_ant_cvv                 66
#define __var_tarjetas_ant_nro_comercio        67
#define __var_tarjetas_ant_cod_servicio        68
#define __var_tarjetas_ant_unicamente_ol       69
#define __var_tarjetas_ant_necesita_pin        70
#define __var_tarjetas_ant_pedir_auto          71
#define __var_tarjetas_ant_lote                72
#define __var_tarjetas_ant_captura_on_line     73
#define __var_tarjetas_ant_coeficiente         74
#define __var_tarjetas_ant_aut_on_line         75
#define __pago_modo                            76
#define __pago_importe                         77
#define __pago_tarjeta                         78
#define __pago_cuenta                          79
#define __pago_buffer1_long                    80
#define __pago_numero                          81
#define __pago_status_tarjeta                  82
#define __pago_banco                           83
#define __pago_fecha_vto                       84
#define __pago_cantidad_cupones                85
#define __pago_valor_cupones                   86
#define __pago_cod_post_cheq                   87
#define __pago_cod_hora_acre                   88
#define __pago_nro_tarjeta                     89
#define __pago_auto_tarjeta                    90
#define __pago_nro_cupon1                      91
#define __pago_nro_cupon_int1                  92
#define __pago_cuotas                          93
#define __pago_buffer                          94
#define __pago_nro_boletin                     95
#define __pago_tabla                           96
#define __pago_variacion                       97
#define __pago_viejo_cuit                      98
#define __pago_telefono                        99
#define __varios_marca_error_red               100
#define __varios_scanner_habilitado            101
#define __varios_hora_z                        102
#define __varios_cajero_z                      103
#define __var_tarjetas_tarjeta                 104
#define __var_tarjetas_numero                  105
#define __var_tarjetas_fecha_vto               106
#define __var_tarjetas_limite                  107
#define __var_tarjetas_cuotas                  108
#define __var_tarjetas_financia_tarjeta        109
#define __var_tarjetas_autorizacion            110
#define __var_tarjetas_nro_cupon               111
#define __var_tarjetas_nro_cupon_int           112
#define __var_tarjetas_lectura_banda           113
#define __var_tarjetas_boletin_vencido         114
#define __var_tarjetas_recargo                 115
#define __var_tarjetas_nro_boletin             116
#define __var_tarjetas_tabla                   117
#define __var_tarjetas_on_line                 118
#define __var_tarjetas_adherente               119
#define __var_tarjetas_anticipo                120
#define __var_tarjetas_saldo_cliente           121
#define __var_tarjetas_importe_cuota           122
#define __var_tarjetas_primer_vto              123
#define __var_tarjetas_moneda                  124
#define __var_tarjetas_importe_entrega         125
#define __var_tarjetas_porc_entrega            126
#define __var_tarjetas_nro_ref                 127
#define __var_tarjetas_aut_en_boletin          128
#define __var_tarjetas_cvv                     129
#define __x_ventas_importe                     130
#define __x_ventas_cantidad                    131
#define __x_retiros_importe                    132
#define __x_retiros_cantidad                   133
#define __x_adelantos_importe                  134
#define __x_adelantos_cantidad                 135
#define __x_intereses_importe                  136
#define __x_intereses_cantidad                 137
#define __x_fondo_fijo_importe                 138
#define __x_fondo_fijo_cantidad                139
#define __x_anulaciones_importe                140
#define __x_anulaciones_cantidad               141
#define __x_dev_envases_importe                142
#define __x_dev_envases_cantidad               143
#define __x_devoluciones_importe               144
#define __x_devoluciones_cantidad              145
#define __x_ventas_gravadas                    146
#define __x_ventas_exentas                     147
#define __x_imp_int                            148
#define __x_dep_imp                            149
#define __x_dep_can                            150
#define __x_demora_segundos                    151
#define __x_demora_ventas                      152
#define __x_tot_dinero                         153
#define __x_iva1                               154
#define __x_iva2                               155
#define __x_retiro_fondo_importe               156
#define __x_retiro_fondo_cantidad              157
#define __x_mozos_importe                      158
#define __x_mozos_cantidad                     159
#define __x_cobros_importe                     160
#define __x_cobros_cantidad                    161
#define __x_pagos_por_caja_importe             162
#define __x_pagos_por_caja_cantidad            163
#define __x_facturas_a_importe                 164
#define __x_facturas_a_cantidad                165
#define __x_facturas_b_importe                 166
#define __x_facturas_b_cantidad                167
#define __x_vueltos_importe                    168
#define __x_vueltos_cantidad                   169
#define __x_bonos_entregados                   170
#define __x_ila                                171
#define __x_iha                                172
#define __x_diferencia                         173
#define __z_ventas_importe                     174
#define __z_ventas_cantidad                    175
#define __z_retiros_importe                    176
#define __z_retiros_cantidad                   177
#define __z_adelantos_importe                  178
#define __z_adelantos_cantidad                 179
#define __z_intereses_importe                  180
#define __z_intereses_cantidad                 181
#define __z_fondo_fijo_importe                 182
#define __z_fondo_fijo_cantidad                183
#define __z_anulaciones_importe                184
#define __z_anulaciones_cantidad               185
#define __z_dev_envases_importe                186
#define __z_dev_envases_cantidad               187
#define __z_devoluciones_importe               188
#define __z_devoluciones_cantidad              189
#define __z_ventas_gravadas                    190
#define __z_ventas_exentas                     191
#define __z_imp_int                            192
#define __z_dep_imp                            193
#define __z_dep_can                            194
#define __z_demora_segundos                    195
#define __z_demora_ventas                      196
#define __z_tot_dinero                         197
#define __z_iva1                               198
#define __z_iva2                               199
#define __z_retiro_fondo_importe               200
#define __z_retiro_fondo_cantidad              201
#define __z_mozos_importe                      202
#define __z_mozos_cantidad                     203
#define __z_cobros_importe                     204
#define __z_cobros_cantidad                    205
#define __z_pagos_por_caja_importe             206
#define __z_pagos_por_caja_cantidad            207
#define __z_facturas_a_importe                 208
#define __z_facturas_a_cantidad                209
#define __z_facturas_b_importe                 210
#define __z_facturas_b_cantidad                211
#define __z_vueltos_importe                    212
#define __z_vueltos_cantidad                   213
#define __z_bonos_entregados                   214
#define __z_ila                                215
#define __z_iha                                216
#define __z_diferencia                         217
#define __z_ant_ventas_importe                 218
#define __z_ant_ventas_cantidad                219
#define __z_ant_retiros_importe                220
#define __z_ant_retiros_cantidad               221
#define __z_ant_adelantos_importe              222
#define __z_ant_adelantos_cantidad             223
#define __z_ant_intereses_importe              224
#define __z_ant_intereses_cantidad             225
#define __z_ant_fondo_fijo_importe             226
#define __z_ant_fondo_fijo_cantidad            227
#define __z_ant_anulaciones_importe            228
#define __z_ant_anulaciones_cantidad           229
#define __z_ant_dev_envases_importe            230
#define __z_ant_dev_envases_cantidad           231
#define __z_ant_devoluciones_importe           232
#define __z_ant_devoluciones_cantidad          233
#define __z_ant_ventas_gravadas                234
#define __z_ant_ventas_exentas                 235
#define __z_ant_imp_int                        236
#define __z_ant_dep_imp                        237
#define __z_ant_dep_can                        238
#define __z_ant_demora_segundos                239
#define __z_ant_demora_ventas                  240
#define __z_ant_tot_dinero                     241
#define __z_ant_iva1                           242
#define __z_ant_iva2                           243
#define __z_ant_retiro_fondo_importe           244
#define __z_ant_retiro_fondo_cantidad          245
#define __z_ant_mozos_importe                  246
#define __z_ant_mozos_cantidad                 247
#define __z_ant_cobros_importe                 248
#define __z_ant_cobros_cantidad                249
#define __z_ant_pagos_por_caja_importe         250
#define __z_ant_pagos_por_caja_cantidad        251
#define __z_ant_facturas_a_importe             252
#define __z_ant_facturas_a_cantidad            253
#define __z_ant_facturas_b_importe             254
#define __z_ant_facturas_b_cantidad            255
#define __z_ant_vueltos_importe                256
#define __z_ant_vueltos_cantidad               257
#define __z_ant_bonos_entregados               258
#define __z_ant_ila                            259
#define __z_ant_iha                            260
#define __z_ant_diferencia                     261
#define __ram_sign                             262
#define __ram_p_ticket                         263
#define __ram_ptr                              264
#define __ram_en_importacion_de_ticket         265
#define ____no_se_usa_002                      266
#define __ram_cantidad                         267
#define __ram_total                            268
#define __ram_total_ant                        269
#define __ram_importe                          270
#define __ram_importe_total                    271
#define ____ram_total_gravado                  272
#define ____ram_tot_gravado                    273
#define ____m_gravado                          274
#define ____m_total_imp_int                    275
#define __ram_intereses                        276
#define __ram_vuelto                           277
#define __ram_cod_alto                         278
#define __ram_cod_bajo                         279
#define __ram_fin_ticket                       280
#define __ram_anulacion                        281
#define __ram_item_supervisado                 282
#define __ram_bultos                           283
#define __ram_bultos_ant                       284
#define __ram_cant_bultos                      285
#define __ram_present_ant                      286
#define __ram_empieza_ticket                   287
#define __ram_discriminar                      288
#define __ram_p_pago                           289
#define __ram_hora_ingreso_cajero              290
#define __ram_hora_inicio_ticket               291
#define __ram_factura                          292
#define __ram_nro_cliente                      293
#define __ram_nom_cajero                       294
#define __ram_doc_cajero                       295
#define __ram_reprocesando                     296
#define __ram_no_reprocesar                    297
#define __ram_solicitar_autorizacion           298
#define __ram_id_evento                        299
#define __ram_variar_precio                    300
#define __ram_variacion                        301
#define __ram_porc_interes                     302
#define __ram_cant_promociones                 303
#define __ram_prohibir_mas                     304
#define __ram_bonos_a_entregar                 305
#define __ram_track1                           306
#define __ram_track2                           307
#define __ram_val_status_st                    308
#define __ram_val_status_status                309
#define __ram_buffer                           310
#define __ram_ptr_prom_cupon                   311
#define __ram_prom_cupon_formato               312
#define __ram_prom_cupon_cantidad              313
#define __ram_pedir_cliente_afinidad           314
#define __ram_cliente_afinidad                 315
#define __ram_nota_cr                          316
#define __ram_oc_gd                            317
#define __ram_tipo_inventario                  318
#define __tot_tick_importe_sin_iva             319
#define __tot_tick_imp_int                     320
#define __tot_tick_costo                       321
#define __tot_tick_recargo_tarjeta             322
#define __tot_tick_exento                      323
#define __tot_tick_iva1                        324
#define __tot_tick_iva2                        325
#define __tot_tick_ila                         326
#define __tot_tick_iha                         327
#define __tot_tick_diferencia                  328
#define __tot_tick_ant_importe_sin_iva         329
#define __tot_tick_ant_imp_int                 330
#define __tot_tick_ant_costo                   331
#define __tot_tick_ant_recargo_tarjeta         332
#define __tot_tick_ant_exento                  333
#define __tot_tick_ant_iva1                    334
#define __tot_tick_ant_iva2                    335
#define __tot_tick_ant_ila                     336
#define __tot_tick_ant_iha                     337
#define __tot_tick_ant_diferencia              338
#define __tot_tick_sv_importe_sin_iva          339
#define __tot_tick_sv_imp_int                  340
#define __tot_tick_sv_costo                    341
#define __tot_tick_sv_recargo_tarjeta          342
#define __tot_tick_sv_exento                   343
#define __tot_tick_sv_iva1                     344
#define __tot_tick_sv_iva2                     345
#define __tot_tick_sv_ila                      346
#define __tot_tick_sv_iha                      347
#define __tot_tick_sv_diferencia               348
#define __item_cod_alto                        349
#define __item_cod_bajo                        350
#define __item_cod_barra                       351
#define __item_nombre_largo                    352
#define __item_imp_int_rec                     353
#define __item_depto                           354
#define __item_precio                          355
#define __item_eng_cod_alto                    356
#define __item_eng_cod_bajo                    357
#define __item_envase                          358
#define __item_dev_envase                      359
#define __item_pesable                         360
#define __item_gravado                         361
#define __item_pedir_precio                    362
#define __item_inhabilitado                    363
#define __item_precio_sin_iva                  364
#define __item_imp_int                         365
#define __item_costo                           366
#define __item_cant_present                    367
#define __item_presentacion                    368
#define __item_cod_presentacion                369
#define __item_cod_especial                    370
#define __item_cod_enganche                    371
#define __item_recargo                         372
#define __item_iva1_precio                     373
#define __item_iva2_precio                     374
#define __item_marca                           375
#define __item_contenido                       376
#define __item_unidad                          377
#define __item_autoinc                         378
#define __item_ila_precio                      379
#define __item_ila_rec                         380
#define __item_iha_precio                      381
#define __item_iha_rec                         382
#define __item_porc_ila                        383
#define __item_porc_iha                        384
#define __item_cod_impuesto                    385
#define __item_iva1_rec                        386
#define __item_iva2_rec                        387
#define __ticket_cod_alto                      389
#define __ticket_cod_bajo                      390
#define __stack_puntero                        414
#define __stack_handle                         415
#define __stack_datos                          416
#define __var_tarjetas_terminal                417
#define __var_tarjetas_status                  418
#define __var_tarjetas_ant_terminal            419
#define __var_tarjetas_ant_status              420
#define __pago_vuelto                          421
#define __varios_offset_altas                  422
#define __ram_cupon_envases                    423
#define __ram_usar_precio_cr                   424
#define __ram_total_vta_contado                425
#define __ram_total_anterior                   426
#define __ram_debe_pagar_contado               427
#define __ram_hora_inicio_evento               428
#define __ram_cod_barra                        429
#define __ram_cod_funcion                      430
#define __item_cupon_dev_env                   431
#define __item_credito                         432
#define __item_scanner                         433
#define __ram_id_evento_all                    434
#define __ram_resto                            435
#define __var_tarjetas_tipo_cuenta             437
#define __var_tarjetas_pin                     438
#define __var_tarjetas_ant_tipo_cuenta         439
#define __var_tarjetas_ant_pin                 440
#define __nro_supervisor_ant                   441
#define __var_tarjetas_celda_pinpad            442
#define __var_tarjetas_ant_celda_pinpad        443
#define __var_tarjetas_l_pin                   444
#define __var_tarjetas_ant_l_pin               445
#define __ram_cantidad_total                   446
#define __ram_renglones                        447
#define __mutual_codigo                        448
#define __mutual_recargo                       449
#define __mutual_orden                         450
#define __mutual_cuotas                        451
#define __ram_p_ext_impuestos                  646
#define __x_nc_a_importe                       649
#define __x_nc_a_cantidad                      650
#define __x_nc_b_importe                       651
#define __x_nc_b_cantidad                      652
#define __z_nc_a_importe                       653
#define __z_nc_a_cantidad                      654
#define __z_nc_b_importe                       655
#define __z_nc_b_cantidad                      656
#define __z_ant_nc_a_importe                   657
#define __z_ant_nc_a_cantidad                  658
#define __z_ant_nc_b_importe                   659
#define __z_ant_nc_b_cantidad                  660
#define __ram_ticket_en_espera                 662
#define __nro_evento                           664
#define __item_cod_clasificacion               665
#define __ram_cambio                           668
#define __ram_pedir_precio                     669
#define __ram_negativo                         670
#define __ram_boleta_anterior                  671
#define __ram_perfil                           674
#define __var_tarjetas_mensaje                 678
#define __var_tarjetas_ant_mensaje             679
#define __ram_descuento_porcentual             683
#define __var_tarjetas_cod_vigencia            685
#define __var_tarjetas_ant_cod_vigencia        686
#define __ram_nom_perfil                       687
#define __ram_retencion_dgr                    692
#define __ram_percepcion_dgi                   693
#define __ram_anular_ticket                    705
#define __ram_pedir_novios                     706
#define __ram_resultado_transac                707
#define __ram_sucursal_anterior                712
#define __ram_caja_anterior                    713
#define __ram_importacion_activa               721
#define __conc_a_pagar_cod_fletero             725
#define __conc_a_pagar_cod_comisionista        726
#define __conc_a_pagar_conceptos               727
#define __conc_a_pagar_status                  728
#define __ram_importe_aut_ctacte               729
#define __ram_importe_aut_cheque               730
#define __pago_imp_moneda_ext                  736
#define __ram_corte_carro                      738
#define __ram_nro_carro                        740
#define ____m_plazo_financiacion_cliente       742
#define __pago_moneda                          744
#define __pago_cod_vigencia                    745
#define __pago_primer_vto                      746
#define __cliente_afinidad_nombre              747
#define __cliente_afinidad_puntaje             748
#define __cliente_afinidad_cod_estado          749
#define __pago_cuenta_bco                      751
#define __ram_novios                           754
#define __ram_medios_obligatorios              755
#define __ram_importacion_pedidos              759
#define __ram_transac_en_cobro                 766
#define __var_tarjetas_pide_entrega            768
#define __var_tarjetas_ant_pide_entrega        769
#define __ram_imprimir_voucher_cmr             770
#define __ram_subindice_pago_cmr               771
#define __pago_importe_entrega                 772
#define __item_stockeable                      775
#define __ram_acu_puntos                       777
#define __ram_perfil_excluir_articulos         779
#define __ram_nombre_cliente_afinidad          780
#define __ram_puntos_afinidad                  781
#define __ram_fecha_puntos_afinidad            782
#define __varios_usuarios_habilitados          788
#define __varios_cajon_activo                  789
#define __var_tarjetas_nro_orden_cuota         794
#define __var_tarjetas_ant_nro_orden_cuota     795
#define __item_habilitado_en_cuotas            796
#define __tot_tick_total_en_cuotas             797
#define __tot_tick_ant_total_en_cuotas         798
#define __tot_tick_sv_total_en_cuotas          799
#define __var_tarjetas_control_cuotas          801
#define __var_tarjetas_ant_control_cuotas      802
#define __nro_caja_dgi                         807
#define __x_fondo_fijo_multiple_importe        816
#define __x_fondo_fijo_multiple_cantidad       817
#define __x_retencion_dgr                      818
#define __x_percepcion_dgi                     819
#define __x_importe_iva                        820
#define __x_importe_sobretasa_iva              821
#define __z_fondo_fijo_multiple_importe        822
#define __z_fondo_fijo_multiple_cantidad       823
#define __z_retencion_dgr                      824
#define __z_percepcion_dgi                     825
#define __z_importe_iva                        826
#define __z_importe_sobretasa_iva              827
#define __z_ant_fondo_fijo_multiple_importe    828
#define __z_ant_fondo_fijo_multiple_cantidad   829
#define __z_ant_retencion_dgr                  830
#define __z_ant_percepcion_dgi                 831
#define __z_ant_importe_iva                    832
#define __z_ant_importe_sobretasa_iva          833
#define __item_cod_iva                         835
#define __nro_legajo_cajero                    837
#define __varios_nro_legajo_cajero_z           838
#define __x_correccion_importe                 840
#define __x_correccion_cantidad                841
#define __z_correccion_importe                 842
#define __z_correccion_cantidad                843
#define __z_ant_correccion_importe             844
#define __z_ant_correccion_cantidad            845
#define __ram_correccion                       847
#define __ram_vuelto_efectivo                  852
#define __ram_diferencia_precio                853
#define __ram_compromiso_200                   854
#define __ram_precio_mas_bajo                  855
#define __item_cod_movimiento                  859
#define __ram_present_ant_bak                  861
#define __nro_evento_a_procesar                868
#define __ram_enviar_offline                   869
#define __x_nro_primer_ticket                  874
#define __x_cantidad_tickets                   875
#define __z_nro_primer_ticket                  876
#define __z_cantidad_tickets                   877
#define __z_ant_nro_primer_ticket              878
#define __z_ant_cantidad_tickets               879
#define __ram_p_pago_ant                       880
#define __varios_modo_y_en_z                   882
#define __ram_mostrar_saldo_cheque             883
#define __var_tarjetas_cupon_original          885
#define __var_tarjetas_fecha_original          886
#define __var_tarjetas_ant_cupon_original      887
#define __var_tarjetas_ant_fecha_original      888
#define __ram_imprimir_cabecera                901
#define __nro_dia_operativo_ultima_z           904
#define __nro_proximo_dia_operativo_servidor   912
#define __varios_copia_ticket_prn              918
#define __xc_vtas_importe                      921
#define __xc_vtas_cantidad                     922
#define __xc_retiros_importe                   923
#define __xc_retiros_cantidad                  924
#define __xc_adelantos_importe                 925
#define __xc_adelantos_cantidad                926
#define __xc_intereses_importe                 927
#define __xc_intereses_cantidad                928
#define __xc_fondo_fijo_importe                929
#define __xc_fondo_fijo_cantidad               930
#define __xc_anulaciones_importe               931
#define __xc_anulaciones_cantidad              932
#define __xc_dev_envases_importe               933
#define __xc_dev_envases_cantidad              934
#define __xc_devoluciones_importe              935
#define __xc_devoluciones_cantidad             936
#define __xc_ventas_gravadas                   937
#define __xc_ventas_exentas                    938
#define __xc_imp_int                           939
#define __xc_dep_imp                           940
#define __xc_demora_segundos                   941
#define __xc_demora_ventas                     942
#define __xc_tot_dinero                        943
#define __xc_iva1                              944
#define __xc_iva2                              945
#define __xc_retiro_fondo_importe              946
#define __xc_retiro_fondo_cantidad             947
#define __xc_mozos_importe                     948
#define __xc_mozos_cantidad                    949
#define __xc_cobros_importe                    950
#define __xc_cobros_cantidad                   951
#define __xc_pagos_por_caja_importe            952
#define __xc_pagos_por_caja_cantidad           953
#define __xc_facturas_a_importe                954
#define __xc_facturas_a_cantidad               955
#define __xc_facturas_b_importe                956
#define __xc_facturas_b_cantidad               957
#define __xc_dep_can                           958
#define __xc_vueltos_importe                   959
#define __xc_vueltos_cantidad                  960
#define __xc_bonos_entregados                  961
#define __xc_ila                               962
#define __xc_iha                               963
#define __xc_diferencia                        964
#define __xc_nc_a_importe                      965
#define __xc_nc_a_cantidad                     966
#define __xc_nc_b_importe                      967
#define __xc_nc_b_cantidad                     968
#define __xc_fondo_fijo_multiple_importe       969
#define __xc_fondo_fijo_multiple_cantidad      970
#define __xc_retencion_dgr                     971
#define __xc_percepcion_dgi                    972
#define __xc_importe_iva                       973
#define __xc_importe_sobretasa_iva             974
#define __xc_correcion_importe                 975
#define __xc_correccion_cantidad               976
#define __xc_nro_primer_ticket                 977
#define __xc_cantidad_tickets                  978
#define __pago_id_registro                     981
#define __nro_ultima_novedad_cliente           982
#define __var_tarjetas_recargo_diferim         992
#define __var_tarjetas_ant_recargo_diferim     993
#define __item_atrib_descuentos                997
#define __ram_pedir_diferimiento               1004
#define __ram_transaccion_enviada              1005
#define __ram_nro_transaccion_pedida           1006
#define __item_habilitado_en_promocion         1008
#define __ram_mult_ticket_en_espera            1011
#define __ram_hora_mult_ticket_en_espera       1012
#define __permite_repactar_tarjeta_cmr         1013
#define __x_cobros_cantidad_servicios          1017
#define __z_cobros_cantidad_servicios          1018
#define __z_ant_cobros_cantidad_servicios      1019
#define __ram_tarj_documento                   1021
#define __ram_tarj_telefono                    1022
#define __ram_desc_porc_pendientes             1030
#define __ram_modo_ejecucion                   1031
#define __ram_modo_memoria_anterior            1032
#define __ram_cantidad_controlada              1033
#define __ram_bien_de_uso                      1034
#define __nro_ultimo_evento_analizado          1035
#define __nro_ultimo_evento_controlado         1036
#define __ram_fecha_anterior                   1039
#define __ram_tipo_comp_anterior               1040
#define __xc_cobros_cantidad_servicios         1041
#define __ram_percepcion_212_iva1              1046
#define __ram_percepcion_212_iva2              1047
#define __x_percepcion_212_iva1                1048
#define __x_percepcion_212_iva2                1049
#define __z_percepcion_212_iva1                1050
#define __z_percepcion_212_iva2                1051
#define __z_ant_percepcion_212_iva1            1052
#define __z_ant_percepcion_212_iva2            1053
#define __xc_percepcion_212_iva1               1054
#define __xc_percepcion_212_iva2               1055
#define __ram_tarj_numero                      1057
#define __ram_tarj_importe                     1058
#define __ram_tarj_cuotas                      1059
#define __item_cod_promocion                   1064
#define __x_diferencia_redondeo                1068
#define __z_diferencia_redondeo                1069
#define __z_ant_diferencia_redondeo            1070
#define __xc_diferencia_redondeo               1071
#define __ram_diferencia_redondeo              1072
#define __ram_cantidad_original                1073
#define __nro_eventos_pendientes_z_anterior    1075
#define __ram_permite_anular_cobro             1080
#define __varios_copia_cobro_prn               1083
#define __varios_tarjetas_por_pago             1086
#define __ram_string_cliente_afinidad          1088
#define __ram_usa_template                     1094
#define __ram_imp_descuento                    1095
#define __ram_tmpo_reg_articulo                1096
#define __ram_tmpo_pago                        1097
#define __ram_tmpo_standby                     1098
#define __ram_modo_devolucion                  1099
#define __ram_tmpo_ocio                        1100
#define __x_imp_devoluciones                   1101
#define __z_imp_devoluciones                   1102
#define __z_ant_imp_devoluciones               1103
#define __xc_imp_devoluciones                  1104
#define __x_cant_devoluciones                  1105
#define __z_cant_devoluciones                  1106
#define __z_ant_cant_devoluciones              1107
#define __xc_cant_devoluciones                 1108
#define __x_cant_articulos                     1109
#define __z_cant_articulos                     1110
#define __z_ant_cant_articulos                 1111
#define __xc_cant_articulos                    1112
#define __x_cant_art_escaneados                1113
#define __z_cant_art_escaneados                1114
#define __z_ant_cant_art_escaneados            1115
#define __xc_cant_art_escaneados               1116
#define __x_tmpo_reg_articulo                  1117
#define __z_tmpo_reg_articulo                  1118
#define __z_ant_tmpo_reg_articulo              1119
#define __xc_tmpo_reg_articulo                 1120
#define __x_tmpo_pago                          1121
#define __z_tmpo_pago                          1122
#define __z_ant_tmpo_pago                      1123
#define __xc_tmpo_pago                         1124
#define __x_tmpo_standby                       1125
#define __z_tmpo_standby                       1126
#define __z_ant_tmpo_standby                   1127
#define __xc_tmpo_standby                      1128
#define __nro_estado_actual                    1129
#define __nro_entrada_estado_actual            1130
#define __x_tmpo_ocio                          1131
#define __z_tmpo_ocio                          1132
#define __z_ant_tmpo_ocio                      1133
#define __xc_tmpo_ocio                         1134
#define __x_imp_cancelados                     1135
#define __z_imp_cancelados                     1136
#define __z_ant_imp_cancelados                 1137
#define __xc_imp_cancelados                    1138
#define __x_cant_cancelados                    1139
#define __z_cant_cancelados                    1140
#define __z_ant_cant_cancelados                1141
#define __xc_cant_cancelados                   1142
#define __ram_tomado_por_template              1143
#define __nro_ultima_novedad_boletin           1144
#define __varios_restringir_volver_de_pagos    1146
#define __ram_ticket_factura                   1152
#define __ram_impresion_fiscal                 1153
#define __varios_imprimiendo_texto_fiscal      1155
#define __item_cod_cobro                       1160
#define __item_comprobante                     1161
#define __pago_vuelto_medio                    1163
#define __pago_cod_medio_vuelto                1164
#define __pago_cod_submedio_vuelto             1165
#define __pago_tipo_vuelto                     1166
#define __ram_descuento_importe                1168
#define __x_cambios_importe                    1169
#define __x_cambios_cantidad                   1170
#define __z_cambios_importe                    1171
#define __z_cambios_cantidad                   1172
#define __z_ant_cambios_importe                1173
#define __z_ant_cambios_cantidad               1174
#define __xc_cambios_importe                   1175
#define __xc_cambios_cantidad                  1176
#define __ram_afinidad_fecha_nac               1180
#define __ram_codigo_tarjeta_afinidad          1181
#define __ram_diferencia_afinidad              1183
#define __ram_cant_articulos_afinidad          1184
#define __ram_utiliza_precio_afinidad          1185
#define __item_descuento_cantidad              1187
#define __ram_puntos_usados                    1188
#define __ram_motivo_descuento                 1189
#define __ram_motivo_descuento_pendiente       1192
#define __pago_importe_me                      1193
#define __x_ventas_importe_me                  1194
#define __x_fondo_fijo_multiple_imp_me         1195
#define __x_retiros_importe_me                 1196
#define __z_ventas_importe_me                  1197
#define __z_fondo_fijo_multiple_imp_me         1198
#define __z_retiros_importe_me                 1199
#define __z_ant_ventas_importe_me              1200
#define __z_ant_fondo_fijo_multiple_imp_m      1201
#define __z_ant_retiros_importe_me             1202
#define __xc_ventas_importe_me                 1203
#define __xc_fondo_fijo_multiple_imp_me        1204
#define __xc_retiros_importe_me                1205
#define __ram_resto_me                         1206
#define __ram_mostrar_faltante_en_me           1208
#define __var_tarjetas_track1                  1209
#define __var_tarjetas_track2                  1210
#define __var_tarjetas_ant_track1              1211
#define __var_tarjetas_ant_track2              1212
#define __tot_tick_imp_tasa                    1215
#define __tot_tick_imp_sobretasa               1216
#define __tot_tick_imp_neto_gravado            1217
#define __tot_tick_ant_imp_tasa                1218
#define __tot_tick_ant_imp_sobretasa           1219
#define __tot_tick_ant_imp_neto_gravado        1220
#define __tot_tick_sv_imp_tasa                 1221
#define __tot_tick_sv_imp_sobretasa            1222
#define __tot_tick_sv_imp_neto_gravado         1223
#define __ram_control_cambio                   1224
#define __ram_total_donacion                   1237
#define __x_donaciones_cantidad                1238
#define __x_donaciones_importe                 1239
#define __z_donaciones_cantidad                1240
#define __z_donaciones_importe                 1241
#define __z_ant_donaciones_cantidad            1242
#define __z_ant_donaciones_importe             1243
#define __xc_donaciones_cantidad               1244
#define __xc_donaciones_importe                1245
#define __ram_motivo_devolucion                1250
#define __pago_cod_solicitud                   1252
#define __ram_string_legajo_empleado           1253
#define __nro_vendedor_long                    1258
#define __item_cod_solicitud                   1261
#define __item_pedir_supervisor                1263
#define __pago_iva_proporcional                1264
#define __ram_institucion_beneficiaria         1265
#define __ram_emitiendo_z                      1268
#define __nro_correlativo_fondo_fijo           1269
#define __nro_correlativo_retiro               1270
#define __ram_id_ultimo_evento                 1288
#define __item_alicuota_precio                 1295
#define __x_alicuota                           1296
#define __z_alicuota                           1297
#define __z_ant_alicuota                       1298
#define __xc_alicuota                          1299
#define __tot_tick_alicuota                    1300
#define __tot_tick_ant_alicuota                1301
#define __tot_tick_sv_alicuota                 1302
#define __nro_correlativo_inventario           1303
#define __ram_cliente_afinidad_adicional       1304
#define __ram_fecha_cumple_afi_adicional       1305
#define __ram_nom_cli_afinidad_adicional       1306
#define __ram_descuento_afinidad_adicional     1307
#define __ram_cod_mutual_afinidad_adicional    1308
#define __ram_numero_ticket_tomra              1309
#define __ram_acu_puntos_cumple                1310
#define __ram_hoy_cumple_anios                 1311
#define __ram_promociones_afinidad             1317
#define __ram_promociones_afinidad_usadas      1318
#define __pago_telefono_otro                   1320
#define __ram_puntos_especiales_afinidad       1322
#define __ram_prom_cupon_lugar_impresion       1323
#define __ram_cupon_leyenda                    1324
#define __ram_prom_cupon_importe               1325
#define __item_hab_desc_cliente                1326
#define __ram_codigo_supervisor                1328
#define __nro_recupero_z                       1330
#define __ram_venta_fraccion                   1333
#define __item_cod_banda                       1335
#define __ram_cadena_cheque                    1342
#define __ram_acumulador                       1345
#define __ram_cod_boleta_plata                 1346
#define __ram_cant_envases                     1348
#define __item_tipo_envase                     1349
#define __ram_referencia_nc                    1351
#define __var_tarjetas_fecha_contable          1353
#define __var_tarjetas_ant_fecha_contable      1354
#define __ram_acumulador_fecha                 1363
#define __ram_prom_cupon_tipo                  1364
#define __ram_prom_cupon_formato_monitor       1365
#define __ram_prom_cupon_grupo                 1366
#define __ram_prom_cupon_nro_acumulador        1367
#define __x_desc_porc_importe                  1368
#define __x_desc_porc_cantidad                 1369
#define __x_desc_porc_ult_art_importe          1370
#define __x_desc_porc_ult_art_cantidad         1371
#define __x_desc_pesos_importe                 1372
#define __x_desc_pesos_cantidad                1373
#define __x_desc_pesos_ult_art_importe         1374
#define __x_desc_pesos_ult_art_cantidad        1375
#define __z_desc_porc_importe                  1376
#define __z_desc_porc_cantidad                 1377
#define __z_desc_porc_ult_art_importe          1378
#define __z_desc_porc_ult_art_cantidad         1379
#define __z_desc_pesos_importe                 1380
#define __z_desc_pesos_cantidad                1381
#define __z_desc_pesos_ult_art_importe         1382
#define __z_desc_pesos_ult_art_cantidad        1383
#define __z_ant_desc_porc_importe              1384
#define __z_ant_desc_porc_cantidad             1385
#define __z_ant_desc_porc_ult_art_importe      1386
#define __z_ant_desc_porc_ult_art_cantidad     1387
#define __z_ant_desc_pesos_importe             1388
#define __z_ant_desc_pesos_cantidad            1389
#define __z_ant_desc_pesos_ult_art_importe     1390
#define __z_ant_desc_pesos_ult_art_cantidad    1391
#define __xc_desc_porc_importe                 1392
#define __xc_desc_porc_cantidad                1393
#define __xc_desc_porc_ult_art_importe         1394
#define __xc_desc_porc_ult_art_cantidad        1395
#define __xc_desc_pesos_importe                1396
#define __xc_desc_pesos_cantidad               1397
#define __xc_desc_pesos_ult_art_importe        1398
#define __xc_desc_pesos_ult_art_cantidad       1399
#define __var_tarjetas_fecha_host              1400
#define __var_tarjetas_hora_host               1401
#define __var_tarjetas_nro_cuenta              1402
#define __var_tarjetas_ant_fecha_host          1403
#define __var_tarjetas_ant_hora_host           1404
#define __var_tarjetas_ant_nro_cuenta          1405
#define __var_tarjetas_hora_original           1412
#define __var_tarjetas_aut_original            1413
#define __var_tarjetas_ant_hora_original       1414
#define __var_tarjetas_ant_aut_original        1415
#define __ram_tipo_evento_comp_original        1416
#define __nro_evento_anterior                  1417
#define __item_porc_ica                        1419
#define __ram_subtotal                         1421
#define __x_desc_porc_importe_sin_iva          1422
#define __z_desc_porc_importe_sin_iva          1423
#define __z_ant_desc_porc_importe_sin_iva      1424
#define __xc_desc_porc_importe_sin_iva         1425
#define __x_desc_porc_ult_art_imp_sin_iva      1426
#define __z_desc_porc_utl_art_imp_sin_iva      1427
#define __z_ant_desc_porc_ult_art_imp_sin_iva  1428
#define __xc_desc_porc_utl_art_imp_sin_iva     1429
#define __x_desc_pesos_importe_sin_iva         1433
#define __x_desc_pesos_ult_art_imp_sin_iva     1434
#define __z_desc_pesos_importe_sin_iva         1435
#define __z_desc_pesos_ult_art_imp_sin_iva     1436
#define __z_ant_desc_pesos_importe_sin_iva     1437
#define __z_ant_desc_pesos_ult_art_imp_sin_iv  1438
#define __xc_desc_pesos_importe_sin_iva        1439
#define __xc_desc_pesos_ult_art_imp_sin_iva    1440
#define __ram_cod_barra_original               1441
#define __var_tarjetas_tasa                    1442
#define __var_tarjetas_vuelto                  1443
#define __var_tarjetas_ant_tasa                1444
#define __var_tarjetas_ant_vuelto              1445
#define __var_tarjetas_grupo_cuota             1447
#define __var_tarjetas_ant_grupo_cuota         1448
#define __ram_nro_suc                          1449
#define __ram_nro_comp                         1450
#define __ram_cant_comp                        1451
#define __var_tarjetas_tasa_aplicada           1452
#define __var_tarjetas_ant_tasa_aplicada       1453
#define __ram_variable_validacion              1454
#define __ram_id_ultimo_evento_factura         1455
#define __ram_nro_ultimo_ticket                1457
#define __x_cant_ticket_dev_envase             1458
#define __z_cant_ticket_dev_envase             1459
#define __z_ant_cant_ticket_dev_envase         1460
#define __xc_cant_ticket_dev_envase            1461
#define __pago_imp_cuota                       1462
#define __item_descuento                       1463
#define __item_monto_impuesto                  1465
#define __item_porc_impuesto                   1466
#define __x_impuestos                          1471
#define __z_impuestos                          1472
#define __z_ant_impuestos                      1473
#define __xc_impuestos                         1474
#define __tot_tick_impuestos                   1475
#define __tot_tick_ant_impuestos               1476
#define __tot_tick_sv_impuestos                1477
#define __item_suma_impuesto_al_total          1478
#define __pago_cod_restriccion_art             1482
#define __sitef_sesion_iniciada                1483
#define __sitef_nid                            1484
#define __sitef_op_habilitadas                 1485
#define __sitef_fecha_debito_predatada         1486
#define __sitef_limite_inf_consulta_cheques    1487
#define __sitef_limite_debito_forzada          1488
#define __sitef_solicitar_cuotas_credito       1489
#define __sitef_timeout                        1490
#define __sitef_cuotas_debito_parcelada        1491
#define __sitef_intervalo_debito_parcela       1492
#define __sitef_1_cuota_debito_vista           1493
#define __sitef_debito_mes_cerrado             1494
#define __sitef_emite_nota_promisoria          1495
#define __var_tarjetas_red                     1496
#define __var_tarjetas_confirmar_numero        1497
#define __var_tarjetas_ultimos_digitos         1498
#define __var_tarjetas_min_cuotas              1499
#define __var_tarjetas_max_cuotas              1500
#define __var_tarjetas_fecha_tope_predatada    1501
#define __var_tarjetas_fecha_tope_1_cuota      1502
#define __var_tarjetas_pedir_cvv               1503
#define __var_tarjetas_pedir_garantia          1504
#define __var_tarjetas_op_habilitadas          1505
#define __var_tarjetas_ant_red                 1506
#define __var_tarjetas_ant_confirmar_numero    1507
#define __var_tarjetas_ant_ultimos_digitos     1508
#define __var_tarjetas_ant_min_cuotas          1509
#define __var_tarjetas_ant_max_cuotas          1510
#define __var_tarjetas_ant_fecha_tope_predata  1511
#define __var_tarjetas_ant_fecha_tope_1_cuota  1512
#define __var_tarjetas_ant_pedir_cvv           1513
#define __var_tarjetas_ant_pedir_garantia      1514
#define __var_tarjetas_ant_op_habilitadas      1515
#define __nro_pagare                           1516
#define __ram_desc_porc_importe                1517
#define __ram_cond_iva_cliente                 1522
#define __ram_fecha_ult_compra_afin            1523
#define __ram_suc_ult_compra_afin              1524
#define __ram_desc_categoria_afin              1525
#define __ram_promos_limitan_medios            1526
#define __ram_nro_pedido_pin                   1527
#define __var_tarjetas_caja_original           1529
#define __var_tarjetas_suc_original            1530
#define __var_tarjetas_ant_caja_original       1531
#define __var_tarjetas_ant_suc_original        1532
#define __var_tarjetas_pin_ascii               1533
#define __var_tarjetas_ant_pin_ascii           1534
#define __var_tarjetas_nom_arch_voucher        1535
#define __var_tarjetas_ant_nom_arch_voucher    1536
#define __ram_no_a_todo                        1539
#define __nro_correlativo_informes             1540
#define __nro_vendedor_string                  1551
#define __item_vta_asistida                    1552
#define __ram_total_vta_asistida               1554
#define __ram_total_donacion_venta             1555
#define __x_donaciones_ventas_importe          1556
#define __x_donaciones_ventas_cantidad         1557
#define __z_donaciones_ventas_importe          1558
#define __z_donaciones_ventas_cantidad         1559
#define __z_ant_donaciones_ventas_importe      1560
#define __z_ant_donaciones_ventas_cantidad     1561
#define __item_participa_donacion              1563
#define __ram_cant_descuento                   1567
#define __ram_cod_promo                        1571
#define __ram_iniciar_periodo_venta            1580
#define __ram_consulta_afin_remota             1583
#define __nro_estado_journal                   1588
#define __nro_estacion_activa                  1589
#define __var_tarjetas_datafono                1598
#define __var_tarjetas_ant_datafono            1599
#define __item_origen                          1601
#define __pago_datafono                        1604
#define __ram_voucher_tarj_comercio            1608
#define __var_tarjetas_nro_trace               1609
#define __var_tarjetas_ant_nro_trace           1610
#define __pago_cuit                            1612
#define __ram_cod_premio                       1613
#define __ram_premio_ejecutado                 1614
#define __x_boletas_con_derecho_a_nc           1616
#define __x_cant_boletas_con_derecho_a_nc      1617
#define __z_boletas_con_derecho_a_nc           1618
#define __z_cant_boletas_con_derecho_a_nc      1619
#define __z_ant_boletas_con_derecho_a_nc       1620
#define __z_ant_cant_boletas_con_derecho_a_nc  1621
#define __xc_donaciones_ventas_importe         1622
#define __xc_donaciones_ventas_cantidad        1623
#define __xc_boletas_con_derecho_a_nc          1624
#define __xc_cant_boletas_con_derecho_a_nc     1625
#define __ram_ahorro_obtenido                  1630
#define __item_precio_original                 1631
#define __ram_puntos_esp_1                     1634
#define __ram_puntos_esp_2                     1635
#define __ram_puntos_esp_3                     1636
#define __ram_puntos_esp_4                     1637
#define __ram_puntos_esp_5                     1638
#define __x_devoluciones_medio                 1639
#define __x_factura_a_importe_exento           1640
#define __x_factura_a_importe_gravado          1641
#define __x_nc_a_importe_exento                1642
#define __x_nc_a_importe_gravado               1643
#define __x_cons_final_importe_exent_fact      1644
#define __x_cons_final_importe_grav_fact       1645
#define __x_cons_final_cantidad_fact           1646
#define __x_dev_cons_f_importe_exent_fact      1647
#define __x_dev_cons_f_importe_grav_fact       1648
#define __x_dev_cons_f_cantidad_fact           1649
#define __x_ventas_diplom_importe              1650
#define __x_ventas_diplom_cantidad             1651
#define __x_devoluciones_diplom_importe        1652
#define __x_devoluciones_diplom_cantidad       1653
#define __x_ventas_dipl_importe                1654
#define __x_ventas_dipl_cantidad               1655
#define __x_devoluciones_export_importe        1656
#define __x_devoluciones_export_cantidad       1657
#define __x_cons_final_importe_exent_tkt       1658
#define __x_cons_final_importe_grav_tkt        1659
#define __x_cons_final_cantidad_tkt            1660
#define __x_dev_cons_final_importe_exent_tkt   1661
#define __x_dev_cons_f_importe_grav_tkt        1662
#define __x_dev_cons_f_cantidad_tkt            1663
#define __x_iva_factura_a                      1664
#define __x_iva_cons_final                     1665
#define __x_nro_transaccion_inicial            1666
#define __x_nro_transaccion_final              1667
#define __z_devoluciones_medio                 1668
#define __z_factura_a_importe_exento           1669
#define __z_factura_a_importe_gravado          1670
#define __z_nc_a_importe_exento                1671
#define __z_nc_a_importe_gravado               1672
#define __z_cons_final_importe_exent_fact      1673
#define __z_cons_final_importe_grav_fact       1674
#define __z_cons_final_cantidad_fact           1675
#define __z_dev_cons_f_importe_exent_fact      1676
#define __z_dev_cons_f_importe_grav_fact       1677
#define __z_dev_cons_f_cantidad_fact           1678
#define __z_ventas_diplom_importe              1679
#define __z_ventas_diplom_cantidad             1680
#define __z_devoluciones_diplom_importe        1681
#define __z_devoluciones_diplom_cantidad       1682
#define __z_ventas_dipl_importe                1683
#define __z_ventas_dipl_cantidad               1684
#define __z_devoluciones_export_importe        1685
#define __z_devoluciones_export_cantidad       1686
#define __z_cons_final_importe_exent_tkt       1687
#define __z_cons_final_importe_grav_tkt        1688
#define __z_cons_final_cantidad_tkt            1689
#define __z_dev_cons_f_importe_exent_tkt       1690
#define __z_dev_cons_f_importe_grav_tkt        1691
#define __z_dev_cons_f_cantidad_tkt            1692
#define __z_iva_factura_a                      1693
#define __z_iva_cons_final                     1694
#define __z_nro_transaccion_inicial            1695
#define __z_nro_transaccion_final              1696
#define __z_ant_devoluciones_medio             1697
#define __z_ant_factura_a_importe_exento       1698
#define __z_ant_factura_a_importe_gravado      1699
#define __z_ant_nc_a_importe_exento            1700
#define __z_ant_nc_a_importe_gravado           1701
#define __z_ant_cons_final_imp_exent_fact      1702
#define __z_ant_cons_final_imp_grav_fact       1703
#define __z_ant_cons_final_cantidad_fact       1704
#define __z_ant_dev_cons_f_imp_exent_fact      1705
#define __z_ant_dev_cons_f_imp_grav_fact       1706
#define __z_ant_dev_cons_f_cantidad_fact       1707
#define __z_ant_ventas_diplom_importe          1708
#define __z_ant_ventas_diplom_cantidad         1709
#define __z_ant_devoluciones_diplom_importe    1710
#define __z_ant_devoluciones_diplom_cantidad   1711
#define __z_ant_ventas_dipl_importe            1712
#define __z_ant_ventas_dipl_cantidad           1713
#define __z_ant_devoluciones_export_importe    1714
#define __z_ant_devoluciones_export_cantidad   1715
#define __z_ant_cons_fin_importe_exent_tkt     1716
#define __z_ant_cons_fin_importe_grav_tkt      1717
#define __z_ant_cons_final_cantidad_tkt        1718
#define __z_ant_dev_con_f_imp_exent_tkt        1719
#define __z_ant_dev_con_f_imp_grav_tkt         1720
#define __z_ant_dev_cons_f_cantidad_tkt        1721
#define __z_ant_iva_factura_a                  1722
#define __z_ant_iva_cons_final                 1723
#define __z_ant_nro_transaccion_inicial        1724
#define __z_ant_nro_transaccion_final          1725
#define __xc_devoluciones_medio                1726
#define __xc_factura_a_importe_exento          1727
#define __xc_factura_a_importe_gravado         1728
#define __xc_nc_a_importe_exento               1729
#define __xc_nc_a_importe_gravado              1730
#define __xc_cons_final_importe_exent_fact     1731
#define __xc_cons_final_importe_grav_fact      1732
#define __xc_cons_final_cantidad_fact          1733
#define __xc_dev_cons_f_importe_exent_fact     1734
#define __xc_dev_cons_f_importe_grav_fact      1735
#define __xc_dev_cons_f_cantidad_fact          1736
#define __xc_ventas_diplom_importe             1737
#define __xc_ventas_diplom_cantidad            1738
#define __xc_devoluciones_diplom_importe       1739
#define __xc_devoluciones_diplom_cantidad      1740
#define __xc_ventas_dipl_importe               1741
#define __xc_ventas_dipl_cantidad              1742
#define __xc_devoluciones_export_importe       1743
#define __xc_devoluciones_export_cantidad      1744
#define __xc_cons_final_importe_exent_tkt      1745
#define __xc_cons_final_importe_grav_tkt       1746
#define __xc_cons_final_cantidad_tkt           1747
#define __xc_dev_cons_f_importe_exent_tkt      1748
#define __xc_dev_cons_f_importe_grav_tkt       1749
#define __xc_dev_cons_f_cantidad_tkt           1750
#define __xc_iva_factura_a                     1751
#define __xc_iva_cons_final                    1752
#define __xc_nro_transaccion_inicial           1753
#define __xc_nro_transaccion_final             1754
#define __nro_ultimo_ticket_rango              1757
#define __nro_orden_dosificacion               1758
#define __x_ventas_export_importe              1760
#define __x_ventas_export_cantidad             1761
#define __z_ventas_export_importe              1762
#define __z_ventas_export_cantidad             1763
#define __z_ant_ventas_export_importe          1764
#define __z_ant_ventas_export_cantidad         1765
#define __xc_ventas_export_importe             1766
#define __xc_ventas_export_cantidad            1767
#define __x_ofertas_importe                    1768
#define __z_oferta_importe                     1769
#define __z_ant_oferta_importe                 1770
#define __xc_oferta_importe                    1771
#define __nro_alfanumerico                     1772
#define __ram_cant_datos_adic_medio            1773
#define __nro_alfa                             1774
#define __nro_sucursal_renta                   1776
#define __nro_registro                         1777
#define __x_descuentos_medio                   1778
#define __z_descuentos_medio                   1779
#define __z_ant_descuentos_medio               1780
#define __xc_descuentos_medio                  1781
#define __x_iva_nc_a                           1783
#define __x_iva_devol_consumidor_final         1784
#define __z_iva_nc_a                           1785
#define __z_iva_devol_consumidor_final         1786
#define __z_ant_iva_nc_a                       1787
#define __z_ant_iva_devol_consumidor_final     1788
#define __xc_iva_nc_a                          1789
#define __xc_iva_devol_consumidor_final        1790
#define __ram_medios_limitados                 1793
#define __ram_sub_medios_limitados             1794
#define __cantidad_medios_limitados            1795
#define __ram_devolucion_parcial_ref           1800
#define __x_ventas_importe_sin_redondeo        1805
#define __z_ventas_importe_sin_redondeo        1806
#define __z_ant_ventas_importe_sin_redondeo    1807
#define __xc_ventas_importe_sin_redondeo       1808
#define __ram_nro_serie_impresora              1809
#define __ram_nro_serie_impresora_ant          1811
#define __ram_hora_anterior                    1812
#define __pago_cod_submedio                    1814
#define __x_cobros_importe_me                  1815
#define __z_cobros_importe_me                  1816
#define __z_ant_cobros_importe_me              1817
#define __xc_cobros_importe_me                 1818
#define __x_retiro_fondo_importe_pagos         1820
#define __z_retiro_fondo_importe_pagos         1821
#define __z_ant_retiro_fondo_importe_pagos     1822
#define __xc_retiro_fondo_importe_pagos        1823
#define __ram_cuenta_cliente                   1827
#define __ram_pos_mask_eliminar                1829
#define __ram_cierre_y                         1831
#define __ram_copia                            1833
#define __ram_pos_rev_promo                    1834
#define __item_cod_especial_alfa               1837
#define __item_cod_enganche_alfa               1838
#define __ram_cobro_servicios                  1840
#define __varios_vaciar_eventos_al_inicio      1844
#define __nro_evento_prioridad_procesar        1846
#define __ram_pos_pago                         1847
#define __ram_agrega_item_tick_import          1848
#define __ram_tmpo_no_venta                    1849
#define __x_tmpo_no_venta                      1850
#define __z_tmpo_no_venta                      1851
#define __z_ant_tmpo_no_venta                  1852
#define __xc_tmpo_no_venta                     1853
#define __item_imprimir_barra                  1854
#define __ram_folio_cobro                      1855
#define __item_cod_clasificacion_str           1857
#define __ram_flag_cobro_venta                 1862
#define __ram_hay_cobro_venta                  1863
#define __ram_tmpo_ocio_aux                    1866
#define __ram_cliente_afinidad_nombre          1873
#define __pago_importe_descuento               1879
#define __item_motivo_devolucion               1881
#define __ram_diferencia_redondeo_grav         1883
#define __ram_diferencia_redondeo_exent        1884
#define __ram_diferencia_redondeo_iva          1885
#define __x_dif_redondeo_vtas_contrib          1886
#define __z_dif_redondeo_vtas_contrib          1887
#define __z_ant_dif_redondeo_vtas_contrib      1888
#define __xc_dif_redondeo_vtas_contrib         1889
#define __x_dif_redondeo_iva_contrib           1890
#define __z_dif_redondeo_iva_contrib           1891
#define __z_ant_dif_redondeo_iva_contrib       1892
#define __xc_dif_redondeo_iva_contrib          1893
#define __x_dif_redondeo_nc                    1894
#define __z_dif_redondeo_nc                    1895
#define __z_ant_dif_redondeo_nc                1896
#define __xc_dif_redondeo_nc                   1897
#define __x_dif_redondeo_iva_nc                1898
#define __z_dif_redondeo_iva_nc                1899
#define __z_ant_dif_redondeo_iva_nc            1900
#define __xc_dif_redondeo_iva_nc               1901
#define __x_dif_redondeo_grav_ticket           1902
#define __x_dif_redondeo_exent_ticket          1903
#define __x_dif_redondeo_iva_ticket            1904
#define __z_dif_redondeo_grav_ticket           1905
#define __z_dif_redondeo_exent_ticket          1906
#define __z_dif_redondeo_iva_ticket            1907
#define __z_ant_dif_redondeo_grav_ticket       1908
#define __z_ant_dif_redondeo_exent_ticket      1909
#define __z_ant_dif_redondeo_iva_ticket        1910
#define __xc_dif_redondeo_grav_ticket          1911
#define __xc_dif_redondeo_exent_ticket         1912
#define __xc_dif_redondeo_iva_ticket           1913
#define __ram_precio_credito                   1917
#define __x_cobros_importes_servicios          1921
#define __z_cobros_importes_servicios          1922
#define __z_ant_cobros_importes_servicios      1923
#define __xc_cobros_importes_servivios         1924
#define __tot_tick_total_por_cod_iva           1932
#define __tot_tick_iva1_por_cod_iva            1933
#define __tot_tick_iva2_por_cod_iva            1934
#define __tot_tick_imp_int_por_cod_iva         1935
#define __ram_percepcion_tacis                 1936
#define __tot_tick_ant_total_por_cod_iva       1938
#define __tot_tick_ant_iva1_por_cod_iva        1939
#define __tot_tick_ant_iva2_por_cod_iva        1940
#define __tot_tick_ant_imp_int_por_cod_iva     1941
#define __tot_tick_sv_total_por_cod_iva        1942
#define __tot_tick_sv_iva1_por_cod_iva         1943
#define __tot_tick_sv_iva2_por_cod_iva         1944
#define __tot_tick_sv_imp_int_por_cod_iva      1945
#define __ram_calcular_retenc_nc               1946
#define __tot_tick_base_por_cod_iva            1947
#define __tot_tick_ant_base_por_cod_iva        1948
#define __tot_tick_sv_base_por_cod_iva         1949
#define __varios_cambio_fecha_y_hora_cf        1950
#define __var_tarjetas_importe                 1953
#define __var_tarjetas_operacion               1954
#define __var_tarjetas_ant_importe             1955
#define __var_tarjetas_ant_operacion           1956
#define __ram_nro_cliente_ant                  1957
#define __ram_reimpresion                      1959
#define __ram_instancia_evento                 1960
#define __ram_modo_ejecucion_ant               1961
#define __ram_impresion_comprobante            1962

/*   Tama¤o de la clase --> 26436 bytes                                     */


/*---------------- Variables Configurables por el Usuario ----------------*/
#define __modo_debug                        436
#define __permite_modo_inventario           452
#define __hard_omron                        453
#define __hard_tec                          454
#define __control_de_apagado                455
#define __inicializar_timer                 456
#define __buffer_lista                      460
#define __buffer_copiar_archivo             461
#define __buffer_validaciones               462
#define __cantidad_validaciones             463
#define __buffer_factura                    464
#define __buffer_remito                     465
#define __tipo_impresora                    466
#define __puerto_impresora                  467
#define __puerto_paralelo                   468
#define __impresora_fiscal                  469
#define __tipo_teclado                      470
#define __estado_num_lock                   471
#define __estado_caps_lock                  472
#define __hard_invel                        473
#define ____rsion_perifericos_invel         474
#define __tipo_de_scanner                   475
#define __scanner_puerto                    476
#define __scanner_data_bits                 477
#define __scanner_stop_bits                 478
#define __scanner_parity                    479
#define __scanner_baudios                   480
#define __trabar_codigo_inexistente         481
#define __tipo_balanza                      482
#define __balanza_puerto                    483
#define __balanza_data_bit                  484
#define __balanza_stop_bit                  485
#define __balanza_parity                    486
#define __balanza_baudios                   487
#define __balanza_cantidad_lecturas         488
#define __tipo_cajon                        489
#define __nro_cajon                         490
#define __tipo_lector_banda                 491
#define __tipo_display                      492
#define __mostrar_hora_display              493
#define __tipo_llave                        494
#define __utiliza_pinpad                    495
#define __salva_pantalla                    496
#define __permanencia_pantalla              497
#define __prefijo_tomra                     498
#define __l_ticket_tomra                    499
#define __enteros                           500
#define __decimales                         501
#define __punto_decimal_obligatorio         502
#define __cantidades_con_decimales          503
#define __l_codigo                          504
#define __l_cod_interno                     505
#define __formato_consulta                  506
#define __formato_ticket                    507
#define __formato_pantalla                  508
#define __datos_cajero_en_ticket            509
#define __datos_vendedor_en_ticket          510
#define __imprimir_pie_y                    511
#define __imprimir_pie_z                    512
#define __id_balanza                        513
#define __largo_codigo_de_confeccion        514
#define __id_confeccion_inicial             515
#define __id_confeccion_final               516
#define __sufijo_codigos_de_preempaque      517
#define __mostrar_hora_pantalla             518
#define __tipo_clave_supervisor             519
#define __tipo_tarjeta                      520
#define __tipo_clave_cajero                 521
#define __identificacion_usuario            522
#define __usar_legajo                       523
#define __break                             524
#define __verificar_ram                     525
#define __verificar_impresora_on_line       526
#define __verificar_paper_out               527
#define __pedir_clave_en_intereses          528
#define __pedir_clave_en_cobros             529
#define __pedir_clave_en_pagos              530
#define __pedir_clave_en_reimpresion        531
#define __pide_supervisor_para_cierre       532
#define __devolucion_habilitada             533
#define __verificar_importe_envases         534
#define __verificar_envases_por_renglon     535
#define __controlar_limite_venta            536
#define __verificar_limite_dgi              537
#define __anulaciones_permitidas            538
#define __autorizar_anulaciones             539
#define __permite_anular_ticket             540
#define __verificar_cajeros_habilitados     541
#define __controlar_vuelto                  542
#define __controlar_cajon                   543
#define __pedir_supervisor_para_codigo_0    544
#define __habilitar_codigos_preempaque      545
#define __facturacion_habilitada            546
#define __tipo_factura_a                    547
#define __nro_factura                       548
#define __limite_lineas_factura             549
#define __pedir_oc_gd                       550
#define __imprimir_retirar_factura          551
#define __puerto_impresora_opcional_factur  552
#define __modo_facturacion                  553
#define __modo_facturacion_por_defecto      554
#define __nro_cliente_por_defecto           555
#define __medio_pago_nc                     556
#define __pedir_nro_remito                  557
#define __pedir_fechas_al_facturar_remitos  558
#define __utiliza_socios                    559
#define __generar_archivo_de_socios         560
#define __cliente_obligatorio               561
#define __cuotas_habilitadas                562
#define __menu_cuotas                       563
#define __puede_autorizar_tarjeta_en_bolet  564
#define __verificar_datos_cupon             565
#define __autorizacion_on_line_habilitada   566
#define __aut_on_line_si_pasa_limite        567
#define __generar_cupones_si_on_line        568
#define __validar_que_pague_con_tarjeta     569
#define __verificar_tarjetas_repetidas      570
#define __tarjetas_permitidas_en_comtar     571
#define __autorizar_tarjetas_repetidas      572
#define __cabal_y_carta_franca_a_posnet     573
#define __pedir_clave_en_tarjeta_especial   575
#define __pedir_datos_tarjeta_al_validar    576
#define __verificar_datos_tarjeta           577
#define __pedir_codigo_postal               578
#define __pedir_codigo_hora                 579
#define __pedir_cuit                        580
#define __pedir_telefono                    581
#define __usar_menu_bancos                  582
#define __pedir_cliente_en_cheques          583
#define __pedir_comprobante_cta_cte         584
#define __emitir_voucher_cta_cte            585
#define __t_compra_entra_como_ctacte        586
#define __usar_cuit_como_nro_cliente        587
#define __cta_cte_on_line                   588
#define __nom_canal_tcp_ip                  589
#define __tratamiento_mutuales              590
#define __discriminar_retiros               591
#define __discriminar_cobros                592
#define __inf10_habilitado                  593
#define __inf15_habilitado                  594
#define __emitir_inf00_en_y                 595
#define __recalcular_iva_en_inf_00          596
#define __emitir_informe_diferencias        597
#define __imprime_fondo_fijo                598
#define __imprime_adelantos                 599
#define __imprime_retiros                   600
#define __imprime_devoluciones              601
#define __tipo_redondeo                     602
#define __ultimo_decimal                    603
#define __presentaciones_habilitadas        604
#define __tipo_presentaciones               605
#define __utiliza_promociones               606
#define __reset_bonos                       607
#define __credito_default                   608
#define __permitir_tecla_credito            609
#define __usar_precio_credito               610
#define __controlar_pago_contado            611
#define __usar_acu_vta_all                  612
#define __usar_acu_vta_caja                 613
#define __usar_acu_vta_local                614
#define __ventas_a_ventas_z                 615
#define __actualizar_stock                  616
#define __usar_eventos_all                  617
#define __multi_supervisor                  618
#define __hay_cod_dev_envase                619
#define __mensajes_habilitados              620
#define __utiliza_mozos                     621
#define __funcion_pide_precio               622
#define __resetear_nro_ticket               623
#define __mantener_fecha_z                  624
#define __utilizar_dia_operativo            625
#define __utiliza_puestos                   626
#define __usar_variacion_precio_siempre     627
#define __mostrar_fecha_operacion           628
#define __salir_al_dos                      629
#define __mostrar_vuelto                    630
#define __usar_disp_en_busqueda_scanner     631
#define __cajeros_flotantes                 632
#define __vendedor_obligatorio              633
#define __imprimir_ticket_al_final          634
#define __reinicializar_al_terminar         635
#define __usar_escape_para_cierre           636
#define __tratamiento_codigo_0              637
#define __mostrar_diferencias               638
#define __controlar_cierre_fallido          639
#define __imprimir_total_en_ticket_factura  640
#define __incrementar_nro_en_reimpresion    641
#define __id_afinidad                       642
#define __inicializar_valdaciones           643
#define __habilitar_enter_en_busqueda       644
#define __factura_con_precio_final          645
#define __permitidos_durante_espera         661
#define __tipo_remito                       663
#define __tipo_factura_b                    667
#define __pedir_precio_en_nc                672
#define __debe_pedir_boleta_anterior        673
#define __inf_tar_habilitado                675
#define __imprimir_nro_evento               676
#define __utiliza_perfiles                  677
#define __utiliza_driver_tcp_ip             680
#define __pedir_supervisor_desc_imp         681
#define __pedir_supervisor_desc_porc        682
#define __pedir_supervisor_en_cambio        684
#define __validacion_del_nro_de_vendedor    688
#define __tipo_nc_a                         689
#define __tipo_nc_b                         690
#define __acepta_precio_cero                691
#define __calcular_retencion_dgr            694
#define __calcular_percepcion_dgi           695
#define __supervisor_para_anular_ticket     696
#define __pedir_codigo_de_financiacion      697
#define __pedir_clave_en_perfiles           698
#define __inf05_habilitado                  699
#define __emitir_voucher_tarjeta            700
#define __pedir_clave_autorizacion_tarjeta  701
#define __borrar_tarjeta_al_salir_de_pagos  702
#define __borrar_perfil_al_salir_de_pagos   703
#define __anular_ticket_si_no_se_autoriza   704
#define __time_out_corte_manual             708
#define __imprimir_cobros                   709
#define __imprimir_ticket_si_es_remota      710
#define __pasar_tarjeta_solo_al_inicio      711
#define __exento_como_gravado_en_fac_a      714
#define __supervisor_para_cliente_manual    715
#define __vaciar_log_al_terminar            716
#define __modo_export_vtas                  717
#define __modo_devol_env                    718
#define __imprimir_ticket                   719
#define __importar_ventas                   720
#define __nc_permanente                     722
#define __presentacion_a_tomar              723
#define __consultar_estado_ctacte           724
#define __pedir_precio_en_cambio            731
#define __permite_cambio_negativo           732
#define __reimprimir_comprobantes           733
#define __grabar_all_en_facturas_locales    734
#define __grabar_evento_en_reimpresion      735
#define __buffer_spooler                    737
#define __controlar_plazo_financiacion      741
#define __pedir_cuenta_bancaria             743
#define __primer_medio_de_pago_tarjeta      750
#define __posicion_nro_ticket               753
#define __permite_cambiar_vendedor          756
#define __pedir_diferencias                 757
#define __supervisor_para_abrir_cajon       758
#define __actualizar_precio_en_pedidos      760
#define __verificar_tarjetas_permitidas     761
#define __posicion_vuelto                   762
#define __imprimir_marca_tarjeta_en_ticket  763
#define __pedir_numero_factura              764
#define __imprimir_bultos_en_ticket         765
#define __modo_identificacion_usuarios      767
#define __prefijo_tarjeta_usuario           773
#define __copiar_eventos_en_cierre          774
#define __tomar_dia_operativo_al_inicio     778
#define __consulta_afinidad_on_line         783
#define __compra_afinidad_on_line           784
#define __permite_superar_limite_ticket     785
#define __utilizar_articulos_remoto         786
#define __multiples_usuarios_para_venta     787
#define __salir_despues_de_cada_ticket      790
#define __usar_numero_de_cajero_como_vende  791
#define __modo_restringido_en_medios        792
#define __pedir_supervisor_para_habil_cajo  793
#define __controlar_total_en_cuotas         800
#define __retiro_remoto_habilitado          803
#define __bmp_en_inicio_del_pos             804
#define __mostrar_subtotal_en_display       805
#define __usar_dynakey                      806
#define __usar_nro_caja_dgi                 808
#define __segundos_verificar_cambio_archiv  809
#define __digitos_bin_internacional         810
#define __utilizar_tabla_marcas_especiales  811
#define __busqueda_cod_interno_alternativa  812
#define __importe_contado_aditivo_en_pagos  813
#define __pausa_en_mensajes                 814
#define __descartar_verificador_busq_alter  815
#define __imprimir_legajo_cajero            836
#define __imprimir_detalle_en_journal       839
#define __codigo_de_venta_por_clasificacio  846
#define __codigo_validacion_limite_dgi      848
#define __modalidad_preempaque              849
#define __pedir_supervisor_ticket_en_esper  850
#define __modo_devolucion                   851
#define __pedir_supervisor_diferencia_prec  856
#define __pedir_supervisor_compromiso_200   857
#define __pedir_supervisor_precio_mas_bajo  858
#define __usar_venta_por_clasificacion      862
#define __usar_venta_por_medios             863
#define __usar_venta_por_xyz                864
#define __id_devolucion_envases             865
#define __script_respuesta                  866
#define __pedir_supervisor_reimp_validacio  867
#define __modo_identificacion_supervisor    870
#define __pedir_cajero_para_cierre          871
#define __tomar_nro_comercio_cuotas         872
#define __validacion_cliente_para_cheque    873
#define __imprimir_y_en_z                   881
#define __cerear_tarjeta_despues_de_valida  884
#define __permite_z_remota                  889
#define __permite_z_local                   890
#define __permite_cierre_sin_red            891
#define __cabecera_ingreso_egreso_cajero    892
#define __lineas_en_blanco_en_voucher_tarj  893
#define __segundos_espera_en_informe        894
#define __permite_multiplicar_item_pesable  895
#define __permite_multiplicar_item_precio   896
#define __validacion_compromiso_200         897
#define __imprimir_cabecera_al_final        898
#define __lineas_corte_receipt              899
#define __grabar_archivo_de_cierre_y        900
#define __usar_digito_para_reimprimir_z     902
#define __deshabilita_corte_papel_en_z      903
#define __time_out_server_no_disponible     905
#define __time_out_llamado_no_atendido      906
#define __time_out_server_no_respondio      907
#define __buscar_boletin                    908
#define __time_out_ping                     909
#define __habilita_ping                     910
#define __direccion_ip_servidor             911
#define __cantidad_minima_modificaciones    913
#define __cantidad_minima_altas             914
#define __habilita_copia_ticket_prn         915
#define __habilita_copia_voucher_prn        916
#define __ticks_power_down                  917
#define __verificar_sincronizacion_impreso  919
#define __inf_cajero_consolidado_habilitad  920
#define __segundos_para_modo_stand_by       979
#define __utiliza_dbrouter                  980
#define __fondo_fijo_multiples_medios       983
#define __retiro_de_fondo_fijo              984
#define __comp_supervisor_fondo_fijo        985
#define __buscar_promocion_por_codigo_inte  986
#define __puerto_servidor                   987
#define __control_de_importes_en_retiros    988
#define __pedir_importe_en_retiro_de_ticke  989
#define __pedir_importe_en_retiro_de_mutua  990
#define __pedir_supervisor_para_retiros     991
#define __sistema_manual_de_tarjeta_diferi  994
#define __descuentos_fijos_selectivos       995
#define __pedir_cajon_en_multiples_usuario  998
#define __pedir_importe_en_retiro_de_chequ  1000
#define __pedir_importe_en_retiro_de_tarje  1001
#define __pedir_importe_en_retiro_de_ctact  1002
#define __recalcular_peso_en_pesables       1003
#define __protocolo_autorizacion_tarjeta    1007
#define __server_login                      1009
#define __server_password                   1010
#define __porcentaje_minimo_de_pago_cmr     1014
#define __cantidad_de_servicios_por_cobro   1015
#define __imprimir_pasillo_en_ticket        1016
#define __max_cantidad_ticket_en_espera     1020
#define __pedir_datos_cliente_tarjeta       1023
#define __informe_10_reducido               1024
#define __discriminar_submedios_cierre_caj  1025
#define __permitir_reintento_de_arqueo      1026
#define __emitir_informe_diferencias_subme  1027
#define __selecciona_cobros_en_revalidacio  1028
#define __calcular_desc_al_final_ticket     1029
#define __controlar_ventas_en_server        1037
#define __emitir_informe_10_en_y            1038
#define __pedir_supervisor_bien_de_uso      1042
#define __selec_submedios_cierre_por_numer  1043
#define __pedir_supervisor_reimp_valid_med  1044
#define __permitir_uso_de_monotributo       1045
#define __permitir_abortar_cierre           1056
#define __buffer_ticket                     1065
#define __tkt_configurable                  1066
#define __tipo_redondeo_vuelto              1067
#define __controlar_ventas_en_historicos    1074
#define __permite_z_sin_red                 1076
#define __permitir_unipago_offline          1077
#define __supervisor_anular_cobro_unipago   1078
#define __numero_comercio_unipago           1079
#define __usar_cobro_tarjeta                1081
#define __habilita_copia_cobro_prn          1082
#define __varias_tarjetas_por_pago          1084
#define __solicitar_nrosucursal_y_ticket    1085
#define __prefijo_cod_dev_envases_evento    1087
#define __habilita_travelpass               1089
#define __eliminar_digito_verificador       1090
#define __prefijo_lectura_por_scanner       1091
#define __longitud_tarjeta_afinidad         1092
#define __cierre_de_lote_por_nodo           1093
#define __controlar_tasas_de_iva            1145
#define __pedir_marca_especial_en_t_manual  1147
#define __controlar_entrada_de_cajeros      1150
#define __imprime_subtotal_al_pasar_a_tota  1151
#define __formato_ticket_fiscal             1154
#define __time_out_reintentos_en_paralelo   1156
#define __controlar_vuelto_maximo_por_medi  1157
#define __modalidad_ticket_en_espera        1158
#define __pedir_supervisor_trae_ticket_esp  1159
#define __imprime_precio_en_modo_inventari  1167
#define __pedir_supervisor_para_cambio_med  1177
#define __cinta_testigo_electronica         1178
#define __mostrar_datos_cliente_afinidad    1179
#define __utilizar_precio_afinidad          1182
#define __pedir_motivo_en_descuentos        1190
#define __pedir_motivo_en_anulaciones       1191
#define __pedir_supervisor_vuelto_moneda_e  1207
#define __medios_de_pago_online             1213
#define __impresion_opcional_del_cheque     1214
#define __referenciar_comp_original_en_cam  1225
#define __imprimir_copia_ticket_devolucion  1226
#define __imprimir_copia_ticket_cambio      1227
#define __pedir_datos_en_devoluciones       1228
#define __pedir_importe_sin_iva_en_cambio   1229
#define __validacion_telefonica_habilitada  1230
#define __ip_host_validacion_telefonica     1231
#define __identificacion_clientes_habilita  1232
#define __pedir_numero_de_cheque            1233
#define __registrar_cheques_en_archivo      1234
#define __donacion_de_vuelto                1235
#define __confirmar_donacion                1236
#define __puerto_host_validacion_telefonic  1246
#define __modalidad_canal_tarjetas          1247
#define __pedir_supervisor_transferencias   1248
#define __pedir_motivo_en_devoluciones      1249
#define __limite_maximo_numero_de_evento    1251
#define __pedir_clave_en_legajo_empleado    1254
#define __codigo_de_perfil_empleados        1255
#define __mostrar_precio_sin_iva_en_ventas  1256
#define __mostrar_mensaje_en_cobros_online  1257
#define __incrementar_nro_evento_configura  1259
#define __imprimir_en_journal               1260
#define __codigo_validacion_para_donacion   1266
#define __incluir_donaciones_en_detalle_00  1267
#define __pasar_cheque_por_lector           1271
#define __verificar_cheque_manual_en_bolet  1272
#define __dias_permitidos_para_cheque_a_fe  1273
#define __longitud_nro_cliente_afinidad     1274
#define __controlar_cliente_afinidad        1275
#define __desplazam_desde_id_cliente_afini  1276
#define __usar_cli_afi_para_clientes_afini  1277
#define __controlar_cupo_en_cheques         1278
#define __utiliza_autorizador_cheques       1279
#define __numero_ip_autorizador_cheques     1280
#define __time_out_server_autorizador_cheq  1281
#define __pide_supervisor_en_cheque_rechaz  1282
#define __pedir_supervisor_autorizador_che  1283
#define __habilita_identificacion_billetes  1284
#define __prefijo_2_de_balanza              1285
#define __prefijo_articulos_por_unidad      1286
#define __id_transaccion                    1287
#define __pedir_precio_sin_iva              1289
#define __pedir_supervisor_articulo_import  1290
#define __pedir_datos_comprador_en_cheques  1291
#define __control_digito_verificador_rut    1292
#define __supervisor_cheque_cliente_inexis  1293
#define __modalidad_vales_institucionales   1312
#define __modalidad_tomra                   1313
#define __codigo_validacion_ticket_tomra    1314
#define __offset_numero_ticket_tomra        1315
#define __pedir_numero_de_vale              1316
#define __mostrar_mensaje_de_envases        1319
#define __pedir_supervisor_devolucion_enva  1321
#define __codigo_validacion_factura_remota  1329
#define __busqueda_de_articulos             1331
#define __controla_importe_medios           1336
#define __puerto_canal_tcp_ip               1337
#define __modelo_tef                        1338
#define __generar_reversos                  1339
#define __generar_cierre_de_lote            1340
#define __codigo_solicitud_datos_adic_clte  1341
#define __t_espera                          1343
#define __descuentos_por_cuerpo             1344
#define __mostrar_menu_de_cuotas_por_medio  1347
#define __reversar_acumuladores_en_devoluc  1352
#define __scanner_habilitado_por_teclado    1355
#define __prefijo_scanner                   1356
#define __sufijo_scanner                    1357
#define __control_nota_de_credito           1358
#define __multiplo_precio_mas_bajo          1359
#define __pedir_caja_y_secuencia            1360
#define __presentaciones_en_dun14           1361
#define __menu_presentaciones_incondiciona  1362
#define __pedir_submedio_en_retiros         1406
#define __pedir_submedio_en_fondo_fijo      1407
#define __pedir_submedio_en_cambio_de_medi  1408
#define __generar_on_line_en_cambio_de_med  1409
#define __validar_medios_en_cambios_de_med  1410
#define __validacion_ticket_en_espera       1411
#define __restringir_cambio_de_valores      1418
#define __sincronizar_numero_ticket_fiscal  1420
#define __verificar_minimo_y_maximo_en_caj  1431
#define __recordar_articulos_en_promocion   1432
#define __menu_cuotas_por_grupo             1446
#define __anulacion_de_cobro                1456
#define __verificar_pinpad_online           1479
#define __verificar_dia_operativo           1480
#define __direccion_ip_safekey              1481
#define __imprimir_cabecera_en_fondo_fijo   1518
#define __indice_comienzo_cliente_en_banda  1519
#define __indice_fin_nro_cliente_en_banda   1520
#define __usar_punto_en_pantalla            1521
#define __mostrar_cupo_usado_cliente        1528
#define __impresora_ssi                     1537
#define __utilizar_ssi                      1538
#define __cantidad_de_digitos_en_nro_legaj  1541
#define __validar_cliente_cta_cte           1542
#define __elegir_cliente_alfabetico_en_pag  1543
#define __nro_vendedor_por_defecto_devoluc  1544
#define __nro_vendedor_por_defecto_venta    1545
#define __digitos_maximo_nro_vendedor       1546
#define __consulta_padron_de_vendedores     1547
#define __usar_venta_asistida               1548
#define __validacion_venta_asistida         1549
#define __copias_validacion_venta_asistida  1550
#define __aplicar_donacion_en_ventas        1562
#define __borrar_mutual_al_salir_de_pagos   1564
#define __pedir_numero_de_orden             1565
#define __mostrar_mutual_activa_en_ventas   1566
#define __tiempo_espera_en_inicializacion   1568
#define __factor_de_sincronizacion_printer  1569
#define __utiliza_modulo_promociones        1570
#define __utilizar_promocion_por_item       1572
#define __mejora_performance_tipo_a         1573
#define __grabar_descuento_en_eventos       1574
#define __handshake                         1575
#define __impresora_data_bits               1576
#define __impresora_stop_bits               1577
#define __impresora_parity                  1578
#define __impresora_baudios                 1579
#define __imprime_totales_en_modo_inventar  1581
#define __imprimir_informes_en_el_cierre    1582
#define __intentos_consulta_servidor        1584
#define __tiempo_entre_reintentos           1585
#define __imprimir_item_recargo_en_fiscal   1586
#define __sugerir_importe_no_nulo_siempre   1587
#define __utilizar_datafono                 1590
#define __puerto_datafono                   1591
#define __datafono_baudios                  1592
#define __datafono_handshake                1593
#define __datafono_data_bits                1594
#define __datafono_stop_bit                 1595
#define __datafono_parity                   1596
#define __teclado_posiflex                  1597
#define __id_afinidad_opcional              1600
#define __editar_datos_cliente_afinidad     1602
#define __mostrar_ticket_en_pantalla        1603
#define __puerto_servidor_dbrouter          1605
#define __clave_supervisor                  1606
#define __tarjeta_supervisor_en_cierre_lot  1607
#define __dias_en_importacion_en_devolucio  1611
#define __controlar_cliente_en_cta_cte      1615
#define __tomar_perfil_desde_cliente        1626
#define __fact_indep_del_tipo_de_cliente    1627
#define __excluir_art_afectados_por_desc    1628
#define __busqueda_cliente                  1633
#define __referenciar_por_secuencia         1755
#define __utilizar_dosificaciones           1756
#define __mostrar_datos_cliente_box         1759
#define __informe_x_y_no_configurable       1775
#define __comandos_modificados              1782
#define __cod_financiacion_por_defecto      1791
#define __controlar_cupo_en_ventas          1792
#define __tomar_script_al_cierre            1796
#define __time_out_espera_script            1797
#define __precio_unitario_en_devoluciones   1798
#define __reset_contador_retiro_en_x        1799
#define __espacio_hd_libre_advertencia      1801
#define __espacio_hd_libre_minimo           1802
#define __anular_item_estricto              1803
#define __pedir_supervisor_alta_cliente     1804
#define __controlar_serial_printer          1810
#define __modo_negocio                      1813
#define __modo_cheque_como_un_medio         1819
#define __timeout_jgateway                  1824
#define __port_jgateway                     1825
#define __direccion_ip_jgateway             1826
#define __generar_cod_barra_invertido       1828
#define __precalcular_impuestos             1830
#define __imprimir_copia_tf                 1832
#define __imprimir_cabecera_principio       1835
#define __habilitar_scanner_al_inicio       1836
#define __asignar_cuota_unica               1841
#define __emitir_voucher_copia_cash_advanc  1842
#define __validar_cierre_cajero             1843
#define __cantidad_eventos_enviar_offline   1845
#define __fondo_fijo_automatico             1856
#define __modo_identificacion_gerente       1859
#define __tipo_clave_gerente                1860
#define __repetir_medio                     1861
#define __alta_de_cliente_habilitada        1864
#define __pide_supervisor_cajero_flotante   1865
#define __imprimir_validacion_1_tecla       1867
#define __validacion_a_imprimir_tecla       1868
#define __alta_de_clientes_permitida        1869
#define __restringir_volver_de_pagos        1870
#define __autorizacion_tarj_en_notas_credi  1871
#define __limitar_notas_de_credito_con_tar  1872
#define __pedir_motivo_anulacion_ticket     1874
#define __pedir_motivo_en_nota_de_credito   1875
#define __permite_medio_deshab_en_dev_tot   1876
#define __autorizar_cambio_medio_en_devol   1877
#define __autorizar_retenciones             1878
#define __orden_voucher_de_tarjeta          1880
#define __importacion_con_seleccion_art     1914
#define __encriptar_datos_de_tarjetas_priv  1915
#define __encripta_datos_de_transac_tarjet  1916
#define __pedir_sup_cierre_con_ticket_espe  1918
#define __permitir_cierre_con_ticket_esper  1919
#define __permite_agrupar_pesables_en_comp  1920
#define __pedir_supervisor_verificar_tarje  1925
#define __voucher_tarjeta_fiscal            1926
#define __dias_en_importacion_a_tf_o_factu  1927
#define __permite_modificar_ticket_importa  1928
#define __sincronizar_hora_controlador_fis  1929
#define __proponer_dia_operativo_al_vencid  1930
#define __supervisor_en_consulta_de_articu  1931
#define __calcular_percepcion_tacis         1937
#define __pedir_nro_cuenta_cheque           1951
#define __pedir_plaza                       1952
#define __limite_de_lineas_de_tickets       1958

/*   Tama¤o de la clase --> 981   bytes                                     */


/*------------------- Variables Internas Inicializadas -------------------*/

/*   Tama¤o de la clase --> 0     bytes                                     */


/*----------------- Variables Internas no Inicializadas -----------------*/
#define __ticket_tipo                          388
#define __ticket_cantidad                      391
#define __ticket_presentacion                  392
#define ____ticket_depto                       393
#define __ticket_precio                        394
#define __ticket_cod_especial                  395
#define __ticket_imp_int                       396
#define __ticket_costo                         397
#define __ticket_recargo                       398
#define __ticket_cod_present                   399
#define __ticket_iva1_precio                   400
#define __ticket_iva2_precio                   401
#define __ticket_iva1_rec                      402
#define __ticket_iva2_rec                      403
#define __ticket_exento                        404
#define __ticket_importe_sin_iva               405
#define __ticket_ila_precio                    406
#define __ticket_iha_precio                    407
#define __ticket_ila_rec                       408
#define __ticket_iha_rec                       409
#define __ticket_cod_impuesto1                 410
#define __ticket_cod_impuesto2                 411
#define __ticket_cod_impuesto3                 412
#define __ticket_cod_impuesto4                 413
#define __ext_impuestos                        647
#define __ticket_cod_ext                       648
#define __ticket_cod_clasificacion             666
#define __ticket_nro_carro                     739
#define __ticket_plazo_financiacion            752
#define __ticket_tipo2                         776
#define __ticket_cod_iva                       834
#define __ticket_cod_movimiento                860
#define __ticket_atrib_descuentos              996
#define __ticket_origen                        999
#define __ticket_cod_promocion                 1060
#define __ticket_cod_promocion1                1061
#define __ticket_cod_promocion2                1062
#define __ticket_cod_promocion3                1063
#define __ticket_gravado                       1148
#define __ticket_depto                         1149
#define __ticket_cod_cobro                     1162
#define __ticket_descuento_cantidad            1186
#define __ticket_cod_solicitud                 1262
#define __ticket_alicuota_precio               1294
#define __ticket_hab_desc_cliente              1327
#define __ticket_cod_barra                     1332
#define __ticket_cod_banda                     1334
#define __ticket_tipo_envase                   1350
#define __ticket_cod_barra_original            1430
#define __ticket_descuento                     1464
#define __ticket_impuesto1_precio              1467
#define __ticket_impuesto2_precio              1468
#define __ticket_impuesto3_precio              1469
#define __ticket_impuesto4_precio              1470
#define __ticket_tipo3                         1553
#define __ticket_cant_excl_desc                1629
#define __ticket_precio_original               1632
#define __ticket_cod_especial_alfa             1839
#define __ticket_cod_clasificacion_str         1858
#define __ticket_motivo_devolucion             1882

/*   Tama¤o de la clase --> 230400bytes                                     */



/****************************************************************************/
/*   Definiciones de Funciones GET_MEMORY                                   */ 
/****************************************************************************/

/*--------------------- Variables en RAM no vol til ---------------------*/
#define  NRO_CAJA                                 GET_MEMORY_INT( __nro_caja )
#define  NRO_NOM_CAJA                             GET_MEMORY_STR( __nro_nom_caja )
#define _NRO_NOM_CAJA(x)                          GET_MEMORY_STR_CHAR( __nro_nom_caja, x )
#define  NRO_INTERESES                            GET_MEMORY_FLOAT( __nro_intereses )
#define  NRO_X                                    GET_MEMORY_LONG( __nro_x )
#define  NRO_Z                                    GET_MEMORY_LONG( __nro_z )
#define  NRO_TICKET                               GET_MEMORY_LONG( __nro_ticket )
#define  NRO_CAJERO                               GET_MEMORY_INT( __nro_cajero )
#define  NRO_CAJERO_ANT                           GET_MEMORY_INT( __nro_cajero_ant )
#define  NRO_VARIACION_PRECIO                     GET_MEMORY_FLOAT( __nro_variacion_precio )
#define  NRO_MODO                                 GET_MEMORY_CHAR( __nro_modo )
#define  NRO_Y                                    GET_MEMORY_LONG( __nro_y )
#define  NRO_VENDEDOR                             GET_MEMORY_INT( __nro_vendedor )
#define  NRO_FECHA_OPERACION                      GET_MEMORY_UNSIGNED( __nro_fecha_operacion )
#define  NRO_DIA_OPERATIVO                        GET_MEMORY_UNSIGNED( __nro_dia_operativo )
#define  NRO_HUBO_OPERACIONES                     GET_MEMORY_CHAR( __nro_hubo_operaciones )
#define _NRO_CUPONES(x)                          _GET_MEMORY_LONG( __nro_cupones, x )
#define  NRO_NRO_RETIRO                           GET_MEMORY_UNSIGNED( __nro_nro_retiro )
#define  NRO_FECHA_Z                              GET_MEMORY_UNSIGNED( __nro_fecha_z )
#define  NRO_ULTIMO_TICKET_ANT                    GET_MEMORY_LONG( __nro_ultimo_ticket_ant )
#define  NRO_GRAN_TOTAL                           GET_MEMORY_DOUBLE( __nro_gran_total )
#define  NRO_RECARGO_FINANCIACION                 GET_MEMORY_FLOAT( __nro_recargo_financiacion )
#define  NRO_COD_CUOTA                            GET_MEMORY_INT( __nro_cod_cuota )
#define  NRO_Z_EMITIDA                            GET_MEMORY_CHAR( __nro_z_emitida )
#define  NRO_CANT_TICKETS_ANT                     GET_MEMORY_INT( __nro_cant_tickets_ant )
#define  NRO_TOTAL_BONOS                          GET_MEMORY_LONG( __nro_total_bonos )
#define  NRO_ULTIMA_ALTA_PROCESADA                GET_MEMORY_LONG( __nro_ultima_alta_procesada )
#define  NRO_ULTIMA_MODIF_PROCESADA               GET_MEMORY_LONG( __nro_ultima_modif_procesada )
#define  NRO_COMP_CTA_CTE                         GET_MEMORY_LONG( __nro_comp_cta_cte )
#define  NRO_REMITO                               GET_MEMORY_LONG( __nro_remito )
#define  NRO_ROLLO                                GET_MEMORY_LONG( __nro_rollo )
#define  NRO_MOZO                                 GET_MEMORY_INT( __nro_mozo )
#define  VAR_TARJETAS_NRO_COMERCIO                GET_MEMORY_STR( __var_tarjetas_nro_comercio )
#define _VAR_TARJETAS_NRO_COMERCIO(x)             GET_MEMORY_STR_CHAR( __var_tarjetas_nro_comercio, x )
#define  VAR_TARJETAS_COD_SERVICIO                GET_MEMORY_STR( __var_tarjetas_cod_servicio )
#define _VAR_TARJETAS_COD_SERVICIO(x)             GET_MEMORY_STR_CHAR( __var_tarjetas_cod_servicio, x )
#define  VAR_TARJETAS_UNICAMENTE_OL               GET_MEMORY_CHAR( __var_tarjetas_unicamente_ol )
#define  VAR_TARJETAS_NECESITA_PIN                GET_MEMORY_CHAR( __var_tarjetas_necesita_pin )
#define  VAR_TARJETAS_PEDIR_AUTO                  GET_MEMORY_CHAR( __var_tarjetas_pedir_auto )
#define  VAR_TARJETAS_LOTE                        GET_MEMORY_UNSIGNED( __var_tarjetas_lote )
#define  VAR_TARJETAS_CAPTURA_ON_LINE             GET_MEMORY_CHAR( __var_tarjetas_captura_on_line )
#define  VAR_TARJETAS_COEFICIENTE                 GET_MEMORY_FLOAT( __var_tarjetas_coeficiente )
#define  VAR_TARJETAS_AUT_ON_LINE                 GET_MEMORY_CHAR( __var_tarjetas_aut_on_line )
#define  VAR_TARJETAS_ANT_TARJETA                 GET_MEMORY_CHAR( __var_tarjetas_ant_tarjeta )
#define  VAR_TARJETAS_ANT_NUMERO                  GET_MEMORY_STR( __var_tarjetas_ant_numero )
#define _VAR_TARJETAS_ANT_NUMERO(x)               GET_MEMORY_STR_CHAR( __var_tarjetas_ant_numero, x )
#define  VAR_TARJETAS_ANT_FECHA_VTO               GET_MEMORY_UNSIGNED( __var_tarjetas_ant_fecha_vto )
#define  VAR_TARJETAS_ANT_LIMITE                  GET_MEMORY_FLOAT( __var_tarjetas_ant_limite )
#define  VAR_TARJETAS_ANT_CUOTAS                  GET_MEMORY_CHAR( __var_tarjetas_ant_cuotas )
#define  VAR_TARJETAS_ANT_FINANCIA_TARJETA        GET_MEMORY_CHAR( __var_tarjetas_ant_financia_tarjeta )
#define  VAR_TARJETAS_ANT_AUTORIZACION            GET_MEMORY_STR( __var_tarjetas_ant_autorizacion )
#define _VAR_TARJETAS_ANT_AUTORIZACION(x)         GET_MEMORY_STR_CHAR( __var_tarjetas_ant_autorizacion, x )
#define  VAR_TARJETAS_ANT_NRO_CUPON               GET_MEMORY_LONG( __var_tarjetas_ant_nro_cupon )
#define  VAR_TARJETAS_ANT_NRO_CUPON_INT           GET_MEMORY_LONG( __var_tarjetas_ant_nro_cupon_int )
#define  VAR_TARJETAS_ANT_LECTURA_BANDA           GET_MEMORY_CHAR( __var_tarjetas_ant_lectura_banda )
#define  VAR_TARJETAS_ANT_BOLETIN_VENCIDO         GET_MEMORY_CHAR( __var_tarjetas_ant_boletin_vencido )
#define  VAR_TARJETAS_ANT_RECARGO                 GET_MEMORY_FLOAT( __var_tarjetas_ant_recargo )
#define  VAR_TARJETAS_ANT_NRO_BOLETIN             GET_MEMORY_UNSIGNED( __var_tarjetas_ant_nro_boletin )
#define  VAR_TARJETAS_ANT_TABLA                   GET_MEMORY_STR( __var_tarjetas_ant_tabla )
#define _VAR_TARJETAS_ANT_TABLA(x)                GET_MEMORY_STR_CHAR( __var_tarjetas_ant_tabla, x )
#define  VAR_TARJETAS_ANT_ON_LINE                 GET_MEMORY_CHAR( __var_tarjetas_ant_on_line )
#define  VAR_TARJETAS_ANT_ADHERENTE               GET_MEMORY_STR( __var_tarjetas_ant_adherente )
#define _VAR_TARJETAS_ANT_ADHERENTE(x)            GET_MEMORY_STR_CHAR( __var_tarjetas_ant_adherente, x )
#define  VAR_TARJETAS_ANT_ANTICIPO                GET_MEMORY_FLOAT( __var_tarjetas_ant_anticipo )
#define  VAR_TARJETAS_ANT_SALDO_CLIENTE           GET_MEMORY_FLOAT( __var_tarjetas_ant_saldo_cliente )
#define  VAR_TARJETAS_ANT_IMPORTE_CUOTA           GET_MEMORY_FLOAT( __var_tarjetas_ant_importe_cuota )
#define  VAR_TARJETAS_ANT_PRIMER_VTO              GET_MEMORY_UNSIGNED( __var_tarjetas_ant_primer_vto )
#define  VAR_TARJETAS_ANT_MONEDA                  GET_MEMORY_CHAR( __var_tarjetas_ant_moneda )
#define  VAR_TARJETAS_ANT_IMPORTE_ENTREGA         GET_MEMORY_DOUBLE( __var_tarjetas_ant_importe_entrega )
#define  VAR_TARJETAS_ANT_PORC_ENTREGA            GET_MEMORY_FLOAT( __var_tarjetas_ant_porc_entrega )
#define  VAR_TARJETAS_ANT_NRO_REF                 GET_MEMORY_STR( __var_tarjetas_ant_nro_ref )
#define _VAR_TARJETAS_ANT_NRO_REF(x)              GET_MEMORY_STR_CHAR( __var_tarjetas_ant_nro_ref, x )
#define  VAR_TARJETAS_ANT_AUT_EN_BOLETIN          GET_MEMORY_CHAR( __var_tarjetas_ant_aut_en_boletin )
#define  VAR_TARJETAS_ANT_CVV                     GET_MEMORY_INT( __var_tarjetas_ant_cvv )
#define  VAR_TARJETAS_ANT_NRO_COMERCIO            GET_MEMORY_STR( __var_tarjetas_ant_nro_comercio )
#define _VAR_TARJETAS_ANT_NRO_COMERCIO(x)         GET_MEMORY_STR_CHAR( __var_tarjetas_ant_nro_comercio, x )
#define  VAR_TARJETAS_ANT_COD_SERVICIO            GET_MEMORY_STR( __var_tarjetas_ant_cod_servicio )
#define _VAR_TARJETAS_ANT_COD_SERVICIO(x)         GET_MEMORY_STR_CHAR( __var_tarjetas_ant_cod_servicio, x )
#define  VAR_TARJETAS_ANT_UNICAMENTE_OL           GET_MEMORY_CHAR( __var_tarjetas_ant_unicamente_ol )
#define  VAR_TARJETAS_ANT_NECESITA_PIN            GET_MEMORY_CHAR( __var_tarjetas_ant_necesita_pin )
#define  VAR_TARJETAS_ANT_PEDIR_AUTO              GET_MEMORY_CHAR( __var_tarjetas_ant_pedir_auto )
#define  VAR_TARJETAS_ANT_LOTE                    GET_MEMORY_UNSIGNED( __var_tarjetas_ant_lote )
#define  VAR_TARJETAS_ANT_CAPTURA_ON_LINE         GET_MEMORY_CHAR( __var_tarjetas_ant_captura_on_line )
#define  VAR_TARJETAS_ANT_COEFICIENTE             GET_MEMORY_FLOAT( __var_tarjetas_ant_coeficiente )
#define  VAR_TARJETAS_ANT_AUT_ON_LINE             GET_MEMORY_CHAR( __var_tarjetas_ant_aut_on_line )
#define _PAGO_MODO(x)                            _GET_MEMORY_CHAR( __pago_modo, x )
#define _PAGO_IMPORTE(x)                         _GET_MEMORY_DOUBLE( __pago_importe, x )
#define _PAGO_TARJETA(x)                         _GET_MEMORY_CHAR( __pago_tarjeta, x )
#define _PAGO_CUENTA(x)                          _GET_MEMORY_INT( __pago_cuenta, x )
#define _PAGO_BUFFER1_LONG(x)                    _GET_MEMORY_LONG( __pago_buffer1_long, x )
#define _PAGO_NUMERO(x)                          _GET_MEMORY_LONG( __pago_numero, x )
#define _PAGO_STATUS_TARJETA(x)                  _GET_MEMORY_CHAR( __pago_status_tarjeta, x )
#define _PAGO_BANCO(x)                           _GET_MEMORY_INT( __pago_banco, x )
#define _PAGO_FECHA_VTO(x)                       _GET_MEMORY_UNSIGNED( __pago_fecha_vto, x )
#define _PAGO_CANTIDAD_CUPONES(x)                _GET_MEMORY_INT( __pago_cantidad_cupones, x )
#define _PAGO_VALOR_CUPONES(x)                   _GET_MEMORY_DOUBLE( __pago_valor_cupones, x )
#define _PAGO_COD_POST_CHEQ(x)                   _GET_MEMORY_INT( __pago_cod_post_cheq, x )
#define _PAGO_COD_HORA_ACRE(x)                   _GET_MEMORY_INT( __pago_cod_hora_acre, x )
#define _PAGO_NRO_TARJETA(x)                     _GET_MEMORY_STR( __pago_nro_tarjeta, x )
#define _PAGO_AUTO_TARJETA(x)                    _GET_MEMORY_STR( __pago_auto_tarjeta, x )
#define _PAGO_NRO_CUPON1(x)                      _GET_MEMORY_LONG( __pago_nro_cupon1, x )
#define _PAGO_NRO_CUPON_INT1(x)                  _GET_MEMORY_LONG( __pago_nro_cupon_int1, x )
#define _PAGO_CUOTAS(x)                          _GET_MEMORY_CHAR( __pago_cuotas, x )
#define _PAGO_BUFFER(x)                          _GET_MEMORY_STR( __pago_buffer, x )
#define _PAGO_NRO_BOLETIN(x)                     _GET_MEMORY_UNSIGNED( __pago_nro_boletin, x )
#define _PAGO_TABLA(x)                           _GET_MEMORY_STR( __pago_tabla, x )
#define _PAGO_VARIACION(x)                       _GET_MEMORY_DOUBLE( __pago_variacion, x )
#define _PAGO_VIEJO_CUIT(x)                      _GET_MEMORY_STR( __pago_viejo_cuit, x )
#define _PAGO_TELEFONO(x)                        _GET_MEMORY_STR( __pago_telefono, x )
#define  VARIOS_MARCA_ERROR_RED                   GET_MEMORY_CHAR( __varios_marca_error_red )
#define  VARIOS_SCANNER_HABILITADO                GET_MEMORY_CHAR( __varios_scanner_habilitado )
#define  VARIOS_HORA_Z                            GET_MEMORY_UNSIGNED( __varios_hora_z )
#define  VARIOS_CAJERO_Z                          GET_MEMORY_INT( __varios_cajero_z )
#define  VAR_TARJETAS_TARJETA                     GET_MEMORY_CHAR( __var_tarjetas_tarjeta )
#define  VAR_TARJETAS_NUMERO                      GET_MEMORY_STR( __var_tarjetas_numero )
#define _VAR_TARJETAS_NUMERO(x)                   GET_MEMORY_STR_CHAR( __var_tarjetas_numero, x )
#define  VAR_TARJETAS_FECHA_VTO                   GET_MEMORY_UNSIGNED( __var_tarjetas_fecha_vto )
#define  VAR_TARJETAS_LIMITE                      GET_MEMORY_FLOAT( __var_tarjetas_limite )
#define  VAR_TARJETAS_CUOTAS                      GET_MEMORY_CHAR( __var_tarjetas_cuotas )
#define  VAR_TARJETAS_FINANCIA_TARJETA            GET_MEMORY_CHAR( __var_tarjetas_financia_tarjeta )
#define  VAR_TARJETAS_AUTORIZACION                GET_MEMORY_STR( __var_tarjetas_autorizacion )
#define _VAR_TARJETAS_AUTORIZACION(x)             GET_MEMORY_STR_CHAR( __var_tarjetas_autorizacion, x )
#define  VAR_TARJETAS_NRO_CUPON                   GET_MEMORY_LONG( __var_tarjetas_nro_cupon )
#define  VAR_TARJETAS_NRO_CUPON_INT               GET_MEMORY_LONG( __var_tarjetas_nro_cupon_int )
#define  VAR_TARJETAS_LECTURA_BANDA               GET_MEMORY_CHAR( __var_tarjetas_lectura_banda )
#define  VAR_TARJETAS_BOLETIN_VENCIDO             GET_MEMORY_CHAR( __var_tarjetas_boletin_vencido )
#define  VAR_TARJETAS_RECARGO                     GET_MEMORY_FLOAT( __var_tarjetas_recargo )
#define  VAR_TARJETAS_NRO_BOLETIN                 GET_MEMORY_UNSIGNED( __var_tarjetas_nro_boletin )
#define  VAR_TARJETAS_TABLA                       GET_MEMORY_STR( __var_tarjetas_tabla )
#define _VAR_TARJETAS_TABLA(x)                    GET_MEMORY_STR_CHAR( __var_tarjetas_tabla, x )
#define  VAR_TARJETAS_ON_LINE                     GET_MEMORY_CHAR( __var_tarjetas_on_line )
#define  VAR_TARJETAS_ADHERENTE                   GET_MEMORY_STR( __var_tarjetas_adherente )
#define _VAR_TARJETAS_ADHERENTE(x)                GET_MEMORY_STR_CHAR( __var_tarjetas_adherente, x )
#define  VAR_TARJETAS_ANTICIPO                    GET_MEMORY_FLOAT( __var_tarjetas_anticipo )
#define  VAR_TARJETAS_SALDO_CLIENTE               GET_MEMORY_FLOAT( __var_tarjetas_saldo_cliente )
#define  VAR_TARJETAS_IMPORTE_CUOTA               GET_MEMORY_FLOAT( __var_tarjetas_importe_cuota )
#define  VAR_TARJETAS_PRIMER_VTO                  GET_MEMORY_UNSIGNED( __var_tarjetas_primer_vto )
#define  VAR_TARJETAS_MONEDA                      GET_MEMORY_CHAR( __var_tarjetas_moneda )
#define  VAR_TARJETAS_IMPORTE_ENTREGA             GET_MEMORY_DOUBLE( __var_tarjetas_importe_entrega )
#define  VAR_TARJETAS_PORC_ENTREGA                GET_MEMORY_FLOAT( __var_tarjetas_porc_entrega )
#define  VAR_TARJETAS_NRO_REF                     GET_MEMORY_STR( __var_tarjetas_nro_ref )
#define _VAR_TARJETAS_NRO_REF(x)                  GET_MEMORY_STR_CHAR( __var_tarjetas_nro_ref, x )
#define  VAR_TARJETAS_AUT_EN_BOLETIN              GET_MEMORY_CHAR( __var_tarjetas_aut_en_boletin )
#define  VAR_TARJETAS_CVV                         GET_MEMORY_INT( __var_tarjetas_cvv )
#define _X_VENTAS_IMPORTE(x)                     _GET_MEMORY_DOUBLE( __x_ventas_importe, x )
#define _X_VENTAS_CANTIDAD(x)                    _GET_MEMORY_INT( __x_ventas_cantidad, x )
#define _X_RETIROS_IMPORTE(x)                    _GET_MEMORY_DOUBLE( __x_retiros_importe, x )
#define _X_RETIROS_CANTIDAD(x)                   _GET_MEMORY_INT( __x_retiros_cantidad, x )
#define  X_ADELANTOS_IMPORTE                      GET_MEMORY_DOUBLE( __x_adelantos_importe )
#define  X_ADELANTOS_CANTIDAD                     GET_MEMORY_INT( __x_adelantos_cantidad )
#define  X_INTERESES_IMPORTE                      GET_MEMORY_DOUBLE( __x_intereses_importe )
#define  X_INTERESES_CANTIDAD                     GET_MEMORY_INT( __x_intereses_cantidad )
#define  X_FONDO_FIJO_IMPORTE                     GET_MEMORY_DOUBLE( __x_fondo_fijo_importe )
#define  X_FONDO_FIJO_CANTIDAD                    GET_MEMORY_INT( __x_fondo_fijo_cantidad )
#define  X_ANULACIONES_IMPORTE                    GET_MEMORY_DOUBLE( __x_anulaciones_importe )
#define  X_ANULACIONES_CANTIDAD                   GET_MEMORY_INT( __x_anulaciones_cantidad )
#define  X_DEV_ENVASES_IMPORTE                    GET_MEMORY_DOUBLE( __x_dev_envases_importe )
#define  X_DEV_ENVASES_CANTIDAD                   GET_MEMORY_INT( __x_dev_envases_cantidad )
#define  X_DEVOLUCIONES_IMPORTE                   GET_MEMORY_DOUBLE( __x_devoluciones_importe )
#define  X_DEVOLUCIONES_CANTIDAD                  GET_MEMORY_INT( __x_devoluciones_cantidad )
#define  X_VENTAS_GRAVADAS                        GET_MEMORY_DOUBLE( __x_ventas_gravadas )
#define  X_VENTAS_EXENTAS                         GET_MEMORY_DOUBLE( __x_ventas_exentas )
#define  X_IMP_INT                                GET_MEMORY_DOUBLE( __x_imp_int )
#define _X_DEP_IMP(x)                            _GET_MEMORY_DOUBLE( __x_dep_imp, x )
#define _X_DEP_CAN(x)                            _GET_MEMORY_DOUBLE( __x_dep_can, x )
#define _X_DEMORA_SEGUNDOS(x)                    _GET_MEMORY_UNSIGNED( __x_demora_segundos, x )
#define _X_DEMORA_VENTAS(x)                      _GET_MEMORY_UNSIGNED( __x_demora_ventas, x )
#define  X_TOT_DINERO                             GET_MEMORY_DOUBLE( __x_tot_dinero )
#define  X_IVA1                                   GET_MEMORY_DOUBLE( __x_iva1 )
#define  X_IVA2                                   GET_MEMORY_DOUBLE( __x_iva2 )
#define  X_RETIRO_FONDO_IMPORTE                   GET_MEMORY_DOUBLE( __x_retiro_fondo_importe )
#define  X_RETIRO_FONDO_CANTIDAD                  GET_MEMORY_INT( __x_retiro_fondo_cantidad )
#define _X_MOZOS_IMPORTE(x)                      _GET_MEMORY_DOUBLE( __x_mozos_importe, x )
#define _X_MOZOS_CANTIDAD(x)                     _GET_MEMORY_DOUBLE( __x_mozos_cantidad, x )
#define _X_COBROS_IMPORTE(x)                     _GET_MEMORY_DOUBLE( __x_cobros_importe, x )
#define _X_COBROS_CANTIDAD(x)                    _GET_MEMORY_INT( __x_cobros_cantidad, x )
#define  X_PAGOS_POR_CAJA_IMPORTE                 GET_MEMORY_DOUBLE( __x_pagos_por_caja_importe )
#define  X_PAGOS_POR_CAJA_CANTIDAD                GET_MEMORY_INT( __x_pagos_por_caja_cantidad )
#define  X_FACTURAS_A_IMPORTE                     GET_MEMORY_DOUBLE( __x_facturas_a_importe )
#define  X_FACTURAS_A_CANTIDAD                    GET_MEMORY_INT( __x_facturas_a_cantidad )
#define  X_FACTURAS_B_IMPORTE                     GET_MEMORY_DOUBLE( __x_facturas_b_importe )
#define  X_FACTURAS_B_CANTIDAD                    GET_MEMORY_INT( __x_facturas_b_cantidad )
#define  X_VUELTOS_IMPORTE                        GET_MEMORY_DOUBLE( __x_vueltos_importe )
#define  X_VUELTOS_CANTIDAD                       GET_MEMORY_INT( __x_vueltos_cantidad )
#define  X_BONOS_ENTREGADOS                       GET_MEMORY_UNSIGNED( __x_bonos_entregados )
#define  X_ILA                                    GET_MEMORY_DOUBLE( __x_ila )
#define  X_IHA                                    GET_MEMORY_DOUBLE( __x_iha )
#define  X_DIFERENCIA                             GET_MEMORY_DOUBLE( __x_diferencia )
#define _Z_VENTAS_IMPORTE(x)                     _GET_MEMORY_DOUBLE( __z_ventas_importe, x )
#define _Z_VENTAS_CANTIDAD(x)                    _GET_MEMORY_INT( __z_ventas_cantidad, x )
#define _Z_RETIROS_IMPORTE(x)                    _GET_MEMORY_DOUBLE( __z_retiros_importe, x )
#define _Z_RETIROS_CANTIDAD(x)                   _GET_MEMORY_INT( __z_retiros_cantidad, x )
#define  Z_ADELANTOS_IMPORTE                      GET_MEMORY_DOUBLE( __z_adelantos_importe )
#define  Z_ADELANTOS_CANTIDAD                     GET_MEMORY_INT( __z_adelantos_cantidad )
#define  Z_INTERESES_IMPORTE                      GET_MEMORY_DOUBLE( __z_intereses_importe )
#define  Z_INTERESES_CANTIDAD                     GET_MEMORY_INT( __z_intereses_cantidad )
#define  Z_FONDO_FIJO_IMPORTE                     GET_MEMORY_DOUBLE( __z_fondo_fijo_importe )
#define  Z_FONDO_FIJO_CANTIDAD                    GET_MEMORY_INT( __z_fondo_fijo_cantidad )
#define  Z_ANULACIONES_IMPORTE                    GET_MEMORY_DOUBLE( __z_anulaciones_importe )
#define  Z_ANULACIONES_CANTIDAD                   GET_MEMORY_INT( __z_anulaciones_cantidad )
#define  Z_DEV_ENVASES_IMPORTE                    GET_MEMORY_DOUBLE( __z_dev_envases_importe )
#define  Z_DEV_ENVASES_CANTIDAD                   GET_MEMORY_INT( __z_dev_envases_cantidad )
#define  Z_DEVOLUCIONES_IMPORTE                   GET_MEMORY_DOUBLE( __z_devoluciones_importe )
#define  Z_DEVOLUCIONES_CANTIDAD                  GET_MEMORY_INT( __z_devoluciones_cantidad )
#define  Z_VENTAS_GRAVADAS                        GET_MEMORY_DOUBLE( __z_ventas_gravadas )
#define  Z_VENTAS_EXENTAS                         GET_MEMORY_DOUBLE( __z_ventas_exentas )
#define  Z_IMP_INT                                GET_MEMORY_DOUBLE( __z_imp_int )
#define _Z_DEP_IMP(x)                            _GET_MEMORY_DOUBLE( __z_dep_imp, x )
#define _Z_DEP_CAN(x)                            _GET_MEMORY_DOUBLE( __z_dep_can, x )
#define _Z_DEMORA_SEGUNDOS(x)                    _GET_MEMORY_UNSIGNED( __z_demora_segundos, x )
#define _Z_DEMORA_VENTAS(x)                      _GET_MEMORY_UNSIGNED( __z_demora_ventas, x )
#define  Z_TOT_DINERO                             GET_MEMORY_DOUBLE( __z_tot_dinero )
#define  Z_IVA1                                   GET_MEMORY_DOUBLE( __z_iva1 )
#define  Z_IVA2                                   GET_MEMORY_DOUBLE( __z_iva2 )
#define  Z_RETIRO_FONDO_IMPORTE                   GET_MEMORY_DOUBLE( __z_retiro_fondo_importe )
#define  Z_RETIRO_FONDO_CANTIDAD                  GET_MEMORY_INT( __z_retiro_fondo_cantidad )
#define _Z_MOZOS_IMPORTE(x)                      _GET_MEMORY_DOUBLE( __z_mozos_importe, x )
#define _Z_MOZOS_CANTIDAD(x)                     _GET_MEMORY_DOUBLE( __z_mozos_cantidad, x )
#define _Z_COBROS_IMPORTE(x)                     _GET_MEMORY_DOUBLE( __z_cobros_importe, x )
#define _Z_COBROS_CANTIDAD(x)                    _GET_MEMORY_INT( __z_cobros_cantidad, x )
#define  Z_PAGOS_POR_CAJA_IMPORTE                 GET_MEMORY_DOUBLE( __z_pagos_por_caja_importe )
#define  Z_PAGOS_POR_CAJA_CANTIDAD                GET_MEMORY_INT( __z_pagos_por_caja_cantidad )
#define  Z_FACTURAS_A_IMPORTE                     GET_MEMORY_DOUBLE( __z_facturas_a_importe )
#define  Z_FACTURAS_A_CANTIDAD                    GET_MEMORY_INT( __z_facturas_a_cantidad )
#define  Z_FACTURAS_B_IMPORTE                     GET_MEMORY_DOUBLE( __z_facturas_b_importe )
#define  Z_FACTURAS_B_CANTIDAD                    GET_MEMORY_INT( __z_facturas_b_cantidad )
#define  Z_VUELTOS_IMPORTE                        GET_MEMORY_DOUBLE( __z_vueltos_importe )
#define  Z_VUELTOS_CANTIDAD                       GET_MEMORY_INT( __z_vueltos_cantidad )
#define  Z_BONOS_ENTREGADOS                       GET_MEMORY_UNSIGNED( __z_bonos_entregados )
#define  Z_ILA                                    GET_MEMORY_DOUBLE( __z_ila )
#define  Z_IHA                                    GET_MEMORY_DOUBLE( __z_iha )
#define  Z_DIFERENCIA                             GET_MEMORY_DOUBLE( __z_diferencia )
#define _Z_ANT_VENTAS_IMPORTE(x)                 _GET_MEMORY_DOUBLE( __z_ant_ventas_importe, x )
#define _Z_ANT_VENTAS_CANTIDAD(x)                _GET_MEMORY_INT( __z_ant_ventas_cantidad, x )
#define _Z_ANT_RETIROS_IMPORTE(x)                _GET_MEMORY_DOUBLE( __z_ant_retiros_importe, x )
#define _Z_ANT_RETIROS_CANTIDAD(x)               _GET_MEMORY_INT( __z_ant_retiros_cantidad, x )
#define  Z_ANT_ADELANTOS_IMPORTE                  GET_MEMORY_DOUBLE( __z_ant_adelantos_importe )
#define  Z_ANT_ADELANTOS_CANTIDAD                 GET_MEMORY_INT( __z_ant_adelantos_cantidad )
#define  Z_ANT_INTERESES_IMPORTE                  GET_MEMORY_DOUBLE( __z_ant_intereses_importe )
#define  Z_ANT_INTERESES_CANTIDAD                 GET_MEMORY_INT( __z_ant_intereses_cantidad )
#define  Z_ANT_FONDO_FIJO_IMPORTE                 GET_MEMORY_DOUBLE( __z_ant_fondo_fijo_importe )
#define  Z_ANT_FONDO_FIJO_CANTIDAD                GET_MEMORY_INT( __z_ant_fondo_fijo_cantidad )
#define  Z_ANT_ANULACIONES_IMPORTE                GET_MEMORY_DOUBLE( __z_ant_anulaciones_importe )
#define  Z_ANT_ANULACIONES_CANTIDAD               GET_MEMORY_INT( __z_ant_anulaciones_cantidad )
#define  Z_ANT_DEV_ENVASES_IMPORTE                GET_MEMORY_DOUBLE( __z_ant_dev_envases_importe )
#define  Z_ANT_DEV_ENVASES_CANTIDAD               GET_MEMORY_INT( __z_ant_dev_envases_cantidad )
#define  Z_ANT_DEVOLUCIONES_IMPORTE               GET_MEMORY_DOUBLE( __z_ant_devoluciones_importe )
#define  Z_ANT_DEVOLUCIONES_CANTIDAD              GET_MEMORY_INT( __z_ant_devoluciones_cantidad )
#define  Z_ANT_VENTAS_GRAVADAS                    GET_MEMORY_DOUBLE( __z_ant_ventas_gravadas )
#define  Z_ANT_VENTAS_EXENTAS                     GET_MEMORY_DOUBLE( __z_ant_ventas_exentas )
#define  Z_ANT_IMP_INT                            GET_MEMORY_DOUBLE( __z_ant_imp_int )
#define _Z_ANT_DEP_IMP(x)                        _GET_MEMORY_DOUBLE( __z_ant_dep_imp, x )
#define _Z_ANT_DEP_CAN(x)                        _GET_MEMORY_DOUBLE( __z_ant_dep_can, x )
#define _Z_ANT_DEMORA_SEGUNDOS(x)                _GET_MEMORY_UNSIGNED( __z_ant_demora_segundos, x )
#define _Z_ANT_DEMORA_VENTAS(x)                  _GET_MEMORY_UNSIGNED( __z_ant_demora_ventas, x )
#define  Z_ANT_TOT_DINERO                         GET_MEMORY_DOUBLE( __z_ant_tot_dinero )
#define  Z_ANT_IVA1                               GET_MEMORY_DOUBLE( __z_ant_iva1 )
#define  Z_ANT_IVA2                               GET_MEMORY_DOUBLE( __z_ant_iva2 )
#define  Z_ANT_RETIRO_FONDO_IMPORTE               GET_MEMORY_DOUBLE( __z_ant_retiro_fondo_importe )
#define  Z_ANT_RETIRO_FONDO_CANTIDAD              GET_MEMORY_INT( __z_ant_retiro_fondo_cantidad )
#define _Z_ANT_MOZOS_IMPORTE(x)                  _GET_MEMORY_DOUBLE( __z_ant_mozos_importe, x )
#define _Z_ANT_MOZOS_CANTIDAD(x)                 _GET_MEMORY_DOUBLE( __z_ant_mozos_cantidad, x )
#define _Z_ANT_COBROS_IMPORTE(x)                 _GET_MEMORY_DOUBLE( __z_ant_cobros_importe, x )
#define _Z_ANT_COBROS_CANTIDAD(x)                _GET_MEMORY_INT( __z_ant_cobros_cantidad, x )
#define  Z_ANT_PAGOS_POR_CAJA_IMPORTE             GET_MEMORY_DOUBLE( __z_ant_pagos_por_caja_importe )
#define  Z_ANT_PAGOS_POR_CAJA_CANTIDAD            GET_MEMORY_INT( __z_ant_pagos_por_caja_cantidad )
#define  Z_ANT_FACTURAS_A_IMPORTE                 GET_MEMORY_DOUBLE( __z_ant_facturas_a_importe )
#define  Z_ANT_FACTURAS_A_CANTIDAD                GET_MEMORY_INT( __z_ant_facturas_a_cantidad )
#define  Z_ANT_FACTURAS_B_IMPORTE                 GET_MEMORY_DOUBLE( __z_ant_facturas_b_importe )
#define  Z_ANT_FACTURAS_B_CANTIDAD                GET_MEMORY_INT( __z_ant_facturas_b_cantidad )
#define  Z_ANT_VUELTOS_IMPORTE                    GET_MEMORY_DOUBLE( __z_ant_vueltos_importe )
#define  Z_ANT_VUELTOS_CANTIDAD                   GET_MEMORY_INT( __z_ant_vueltos_cantidad )
#define  Z_ANT_BONOS_ENTREGADOS                   GET_MEMORY_UNSIGNED( __z_ant_bonos_entregados )
#define  Z_ANT_ILA                                GET_MEMORY_DOUBLE( __z_ant_ila )
#define  Z_ANT_IHA                                GET_MEMORY_DOUBLE( __z_ant_iha )
#define  Z_ANT_DIFERENCIA                         GET_MEMORY_DOUBLE( __z_ant_diferencia )
#define  RAM_SIGN                                 GET_MEMORY_STR( __ram_sign )
#define _RAM_SIGN(x)                              GET_MEMORY_STR_CHAR( __ram_sign, x )
#define  RAM_P_TICKET                             GET_MEMORY_INT( __ram_p_ticket )
#define  RAM_PTR                                  GET_MEMORY_INT( __ram_ptr )
#define  RAM_EN_IMPORTACION_DE_TICKET             GET_MEMORY_CHAR( __ram_en_importacion_de_ticket )
#define  __NO_SE_USA_002                          GET_MEMORY_INT( ____no_se_usa_002 )
#define  RAM_CANTIDAD                             GET_MEMORY_DOUBLE( __ram_cantidad )
#define  RAM_TOTAL                                GET_MEMORY_DOUBLE( __ram_total )
#define  RAM_TOTAL_ANT                            GET_MEMORY_DOUBLE( __ram_total_ant )
#define  RAM_IMPORTE                              GET_MEMORY_DOUBLE( __ram_importe )
#define  RAM_IMPORTE_TOTAL                        GET_MEMORY_DOUBLE( __ram_importe_total )
#define  __RAM_TOTAL_GRAVADO                      GET_MEMORY_DOUBLE( ____ram_total_gravado )
#define  __RAM_TOT_GRAVADO                        GET_MEMORY_DOUBLE( ____ram_tot_gravado )
#define  __M_GRAVADO                              GET_MEMORY_DOUBLE( ____m_gravado )
#define  __M_TOTAL_IMP_INT                        GET_MEMORY_DOUBLE( ____m_total_imp_int )
#define  RAM_INTERESES                            GET_MEMORY_DOUBLE( __ram_intereses )
#define  RAM_VUELTO                               GET_MEMORY_DOUBLE( __ram_vuelto )
#define  RAM_COD_ALTO                             GET_MEMORY_LONG( __ram_cod_alto )
#define  RAM_COD_BAJO                             GET_MEMORY_INT( __ram_cod_bajo )
#define  RAM_FIN_TICKET                           GET_MEMORY_INT( __ram_fin_ticket )
#define  RAM_ANULACION                            GET_MEMORY_INT( __ram_anulacion )
#define  RAM_ITEM_SUPERVISADO                     GET_MEMORY_INT( __ram_item_supervisado )
#define  RAM_BULTOS                               GET_MEMORY_INT( __ram_bultos )
#define  RAM_BULTOS_ANT                           GET_MEMORY_INT( __ram_bultos_ant )
#define  RAM_CANT_BULTOS                          GET_MEMORY_INT( __ram_cant_bultos )
#define  RAM_PRESENT_ANT                          GET_MEMORY_INT( __ram_present_ant )
#define  RAM_EMPIEZA_TICKET                       GET_MEMORY_CHAR( __ram_empieza_ticket )
#define  RAM_DISCRIMINAR                          GET_MEMORY_CHAR( __ram_discriminar )
#define  RAM_P_PAGO                               GET_MEMORY_CHAR( __ram_p_pago )
#define  RAM_HORA_INGRESO_CAJERO                  GET_MEMORY_LONG( __ram_hora_ingreso_cajero )
#define  RAM_HORA_INICIO_TICKET                   GET_MEMORY_LONG( __ram_hora_inicio_ticket )
#define  RAM_FACTURA                              GET_MEMORY_CHAR( __ram_factura )
#define  RAM_NRO_CLIENTE                          GET_MEMORY_LONG( __ram_nro_cliente )
#define  RAM_NOM_CAJERO                           GET_MEMORY_STR( __ram_nom_cajero )
#define _RAM_NOM_CAJERO(x)                        GET_MEMORY_STR_CHAR( __ram_nom_cajero, x )
#define  RAM_DOC_CAJERO                           GET_MEMORY_STR( __ram_doc_cajero )
#define _RAM_DOC_CAJERO(x)                        GET_MEMORY_STR_CHAR( __ram_doc_cajero, x )
#define  RAM_REPROCESANDO                         GET_MEMORY_CHAR( __ram_reprocesando )
#define  RAM_NO_REPROCESAR                        GET_MEMORY_CHAR( __ram_no_reprocesar )
#define  RAM_SOLICITAR_AUTORIZACION               GET_MEMORY_CHAR( __ram_solicitar_autorizacion )
#define  RAM_ID_EVENTO                            GET_MEMORY_LONG( __ram_id_evento )
#define  RAM_VARIAR_PRECIO                        GET_MEMORY_CHAR( __ram_variar_precio )
#define  RAM_VARIACION                            GET_MEMORY_DOUBLE( __ram_variacion )
#define  RAM_PORC_INTERES                         GET_MEMORY_FLOAT( __ram_porc_interes )
#define  RAM_CANT_PROMOCIONES                     GET_MEMORY_INT( __ram_cant_promociones )
#define  RAM_PROHIBIR_MAS                         GET_MEMORY_CHAR( __ram_prohibir_mas )
#define  RAM_BONOS_A_ENTREGAR                     GET_MEMORY_INT( __ram_bonos_a_entregar )
#define  RAM_TRACK1                               GET_MEMORY_STR( __ram_track1 )
#define _RAM_TRACK1(x)                            GET_MEMORY_STR_CHAR( __ram_track1, x )
#define  RAM_TRACK2                               GET_MEMORY_STR( __ram_track2 )
#define _RAM_TRACK2(x)                            GET_MEMORY_STR_CHAR( __ram_track2, x )
#define  RAM_VAL_STATUS_ST                        GET_MEMORY_INT( __ram_val_status_st )
#define  RAM_VAL_STATUS_STATUS                    GET_MEMORY_INT( __ram_val_status_status )
#define  RAM_BUFFER                               GET_MEMORY_STR( __ram_buffer )
#define _RAM_BUFFER(x)                            GET_MEMORY_STR_CHAR( __ram_buffer, x )
#define  RAM_PTR_PROM_CUPON                       GET_MEMORY_INT( __ram_ptr_prom_cupon )
#define _RAM_PROM_CUPON_FORMATO(x)               _GET_MEMORY_UNSIGNED( __ram_prom_cupon_formato, x )
#define _RAM_PROM_CUPON_CANTIDAD(x)              _GET_MEMORY_UNSIGNED( __ram_prom_cupon_cantidad, x )
#define  RAM_PEDIR_CLIENTE_AFINIDAD               GET_MEMORY_CHAR( __ram_pedir_cliente_afinidad )
#define  RAM_CLIENTE_AFINIDAD                     GET_MEMORY_LONG( __ram_cliente_afinidad )
#define  RAM_NOTA_CR                              GET_MEMORY_CHAR( __ram_nota_cr )
#define  RAM_OC_GD                                GET_MEMORY_STR( __ram_oc_gd )
#define _RAM_OC_GD(x)                             GET_MEMORY_STR_CHAR( __ram_oc_gd, x )
#define  RAM_TIPO_INVENTARIO                      GET_MEMORY_INT( __ram_tipo_inventario )
#define  TOT_TICK_IMPORTE_SIN_IVA                 GET_MEMORY_DOUBLE( __tot_tick_importe_sin_iva )
#define  TOT_TICK_IMP_INT                         GET_MEMORY_DOUBLE( __tot_tick_imp_int )
#define  TOT_TICK_COSTO                           GET_MEMORY_DOUBLE( __tot_tick_costo )
#define  TOT_TICK_RECARGO_TARJETA                 GET_MEMORY_DOUBLE( __tot_tick_recargo_tarjeta )
#define  TOT_TICK_EXENTO                          GET_MEMORY_DOUBLE( __tot_tick_exento )
#define  TOT_TICK_IVA1                            GET_MEMORY_DOUBLE( __tot_tick_iva1 )
#define  TOT_TICK_IVA2                            GET_MEMORY_DOUBLE( __tot_tick_iva2 )
#define  TOT_TICK_ILA                             GET_MEMORY_DOUBLE( __tot_tick_ila )
#define  TOT_TICK_IHA                             GET_MEMORY_DOUBLE( __tot_tick_iha )
#define  TOT_TICK_DIFERENCIA                      GET_MEMORY_DOUBLE( __tot_tick_diferencia )
#define  TOT_TICK_ANT_IMPORTE_SIN_IVA             GET_MEMORY_DOUBLE( __tot_tick_ant_importe_sin_iva )
#define  TOT_TICK_ANT_IMP_INT                     GET_MEMORY_DOUBLE( __tot_tick_ant_imp_int )
#define  TOT_TICK_ANT_COSTO                       GET_MEMORY_DOUBLE( __tot_tick_ant_costo )
#define  TOT_TICK_ANT_RECARGO_TARJETA             GET_MEMORY_DOUBLE( __tot_tick_ant_recargo_tarjeta )
#define  TOT_TICK_ANT_EXENTO                      GET_MEMORY_DOUBLE( __tot_tick_ant_exento )
#define  TOT_TICK_ANT_IVA1                        GET_MEMORY_DOUBLE( __tot_tick_ant_iva1 )
#define  TOT_TICK_ANT_IVA2                        GET_MEMORY_DOUBLE( __tot_tick_ant_iva2 )
#define  TOT_TICK_ANT_ILA                         GET_MEMORY_DOUBLE( __tot_tick_ant_ila )
#define  TOT_TICK_ANT_IHA                         GET_MEMORY_DOUBLE( __tot_tick_ant_iha )
#define  TOT_TICK_ANT_DIFERENCIA                  GET_MEMORY_DOUBLE( __tot_tick_ant_diferencia )
#define  TOT_TICK_SV_IMPORTE_SIN_IVA              GET_MEMORY_DOUBLE( __tot_tick_sv_importe_sin_iva )
#define  TOT_TICK_SV_IMP_INT                      GET_MEMORY_DOUBLE( __tot_tick_sv_imp_int )
#define  TOT_TICK_SV_COSTO                        GET_MEMORY_DOUBLE( __tot_tick_sv_costo )
#define  TOT_TICK_SV_RECARGO_TARJETA              GET_MEMORY_DOUBLE( __tot_tick_sv_recargo_tarjeta )
#define  TOT_TICK_SV_EXENTO                       GET_MEMORY_DOUBLE( __tot_tick_sv_exento )
#define  TOT_TICK_SV_IVA1                         GET_MEMORY_DOUBLE( __tot_tick_sv_iva1 )
#define  TOT_TICK_SV_IVA2                         GET_MEMORY_DOUBLE( __tot_tick_sv_iva2 )
#define  TOT_TICK_SV_ILA                          GET_MEMORY_DOUBLE( __tot_tick_sv_ila )
#define  TOT_TICK_SV_IHA                          GET_MEMORY_DOUBLE( __tot_tick_sv_iha )
#define  TOT_TICK_SV_DIFERENCIA                   GET_MEMORY_DOUBLE( __tot_tick_sv_diferencia )
#define  ITEM_COD_ALTO                            GET_MEMORY_LONG( __item_cod_alto )
#define  ITEM_COD_BAJO                            GET_MEMORY_INT( __item_cod_bajo )
#define  ITEM_COD_BARRA                           GET_MEMORY_STR( __item_cod_barra )
#define _ITEM_COD_BARRA(x)                        GET_MEMORY_STR_CHAR( __item_cod_barra, x )
#define  ITEM_NOMBRE_LARGO                        GET_MEMORY_STR( __item_nombre_largo )
#define _ITEM_NOMBRE_LARGO(x)                     GET_MEMORY_STR_CHAR( __item_nombre_largo, x )
#define  ITEM_IMP_INT_REC                         GET_MEMORY_DOUBLE( __item_imp_int_rec )
#define  ITEM_DEPTO                               GET_MEMORY_INT( __item_depto )
#define  ITEM_PRECIO                              GET_MEMORY_DOUBLE( __item_precio )
#define  ITEM_ENG_COD_ALTO                        GET_MEMORY_LONG( __item_eng_cod_alto )
#define  ITEM_ENG_COD_BAJO                        GET_MEMORY_INT( __item_eng_cod_bajo )
#define  ITEM_ENVASE                              GET_MEMORY_CHAR( __item_envase )
#define  ITEM_DEV_ENVASE                          GET_MEMORY_CHAR( __item_dev_envase )
#define  ITEM_PESABLE                             GET_MEMORY_CHAR( __item_pesable )
#define  ITEM_GRAVADO                             GET_MEMORY_CHAR( __item_gravado )
#define  ITEM_PEDIR_PRECIO                        GET_MEMORY_CHAR( __item_pedir_precio )
#define  ITEM_INHABILITADO                        GET_MEMORY_CHAR( __item_inhabilitado )
#define  ITEM_PRECIO_SIN_IVA                      GET_MEMORY_DOUBLE( __item_precio_sin_iva )
#define  ITEM_IMP_INT                             GET_MEMORY_DOUBLE( __item_imp_int )
#define  ITEM_COSTO                               GET_MEMORY_DOUBLE( __item_costo )
#define  ITEM_CANT_PRESENT                        GET_MEMORY_CHAR( __item_cant_present )
#define  ITEM_PRESENTACION                        GET_MEMORY_INT( __item_presentacion )
#define  ITEM_COD_PRESENTACION                    GET_MEMORY_CHAR( __item_cod_presentacion )
#define  ITEM_COD_ESPECIAL                        GET_MEMORY_LONG( __item_cod_especial )
#define  ITEM_COD_ENGANCHE                        GET_MEMORY_LONG( __item_cod_enganche )
#define  ITEM_RECARGO                             GET_MEMORY_DOUBLE( __item_recargo )
#define  ITEM_IVA1_PRECIO                         GET_MEMORY_DOUBLE( __item_iva1_precio )
#define  ITEM_IVA2_PRECIO                         GET_MEMORY_DOUBLE( __item_iva2_precio )
#define  ITEM_MARCA                               GET_MEMORY_STR( __item_marca )
#define _ITEM_MARCA(x)                            GET_MEMORY_STR_CHAR( __item_marca, x )
#define  ITEM_CONTENIDO                           GET_MEMORY_STR( __item_contenido )
#define _ITEM_CONTENIDO(x)                        GET_MEMORY_STR_CHAR( __item_contenido, x )
#define  ITEM_UNIDAD                              GET_MEMORY_STR( __item_unidad )
#define _ITEM_UNIDAD(x)                           GET_MEMORY_STR_CHAR( __item_unidad, x )
#define  ITEM_AUTOINC                             GET_MEMORY_LONG( __item_autoinc )
#define  ITEM_ILA_PRECIO                          GET_MEMORY_FLOAT( __item_ila_precio )
#define  ITEM_ILA_REC                             GET_MEMORY_FLOAT( __item_ila_rec )
#define  ITEM_IHA_PRECIO                          GET_MEMORY_FLOAT( __item_iha_precio )
#define  ITEM_IHA_REC                             GET_MEMORY_FLOAT( __item_iha_rec )
#define  ITEM_PORC_ILA                            GET_MEMORY_FLOAT( __item_porc_ila )
#define  ITEM_PORC_IHA                            GET_MEMORY_FLOAT( __item_porc_iha )
#define _ITEM_COD_IMPUESTO(x)                    _GET_MEMORY_CHAR( __item_cod_impuesto, x )
#define  ITEM_IVA1_REC                            GET_MEMORY_DOUBLE( __item_iva1_rec )
#define  ITEM_IVA2_REC                            GET_MEMORY_DOUBLE( __item_iva2_rec )
#define _TICKET_COD_ALTO(x)                      _GET_MEMORY_LONG( __ticket_cod_alto, x )
#define _TICKET_COD_BAJO(x)                      _GET_MEMORY_INT( __ticket_cod_bajo, x )
#define  STACK_PUNTERO                            GET_MEMORY_INT( __stack_puntero )
#define  STACK_HANDLE                             GET_MEMORY_INT( __stack_handle )
#define  STACK_DATOS                              GET_MEMORY_STR( __stack_datos )
#define _STACK_DATOS(x)                           GET_MEMORY_STR_CHAR( __stack_datos, x )
#define  VAR_TARJETAS_TERMINAL                    GET_MEMORY_LONG( __var_tarjetas_terminal )
#define  VAR_TARJETAS_STATUS                      GET_MEMORY_CHAR( __var_tarjetas_status )
#define  VAR_TARJETAS_ANT_TERMINAL                GET_MEMORY_LONG( __var_tarjetas_ant_terminal )
#define  VAR_TARJETAS_ANT_STATUS                  GET_MEMORY_CHAR( __var_tarjetas_ant_status )
#define _PAGO_VUELTO(x)                          _GET_MEMORY_DOUBLE( __pago_vuelto, x )
#define  VARIOS_OFFSET_ALTAS                      GET_MEMORY_LONG( __varios_offset_altas )
#define  RAM_CUPON_ENVASES                        GET_MEMORY_DOUBLE( __ram_cupon_envases )
#define  RAM_USAR_PRECIO_CR                       GET_MEMORY_CHAR( __ram_usar_precio_cr )
#define  RAM_TOTAL_VTA_CONTADO                    GET_MEMORY_DOUBLE( __ram_total_vta_contado )
#define  RAM_TOTAL_ANTERIOR                       GET_MEMORY_DOUBLE( __ram_total_anterior )
#define  RAM_DEBE_PAGAR_CONTADO                   GET_MEMORY_CHAR( __ram_debe_pagar_contado )
#define  RAM_HORA_INICIO_EVENTO                   GET_MEMORY_UNSIGNED( __ram_hora_inicio_evento )
#define  RAM_COD_BARRA                            GET_MEMORY_STR( __ram_cod_barra )
#define _RAM_COD_BARRA(x)                         GET_MEMORY_STR_CHAR( __ram_cod_barra, x )
#define  RAM_COD_FUNCION                          GET_MEMORY_INT( __ram_cod_funcion )
#define  ITEM_CUPON_DEV_ENV                       GET_MEMORY_CHAR( __item_cupon_dev_env )
#define  ITEM_CREDITO                             GET_MEMORY_CHAR( __item_credito )
#define  ITEM_SCANNER                             GET_MEMORY_CHAR( __item_scanner )
#define  RAM_ID_EVENTO_ALL                        GET_MEMORY_LONG( __ram_id_evento_all )
#define  RAM_RESTO                                GET_MEMORY_DOUBLE( __ram_resto )
#define  VAR_TARJETAS_TIPO_CUENTA                 GET_MEMORY_CHAR( __var_tarjetas_tipo_cuenta )
#define  VAR_TARJETAS_PIN                         GET_MEMORY_STR( __var_tarjetas_pin )
#define _VAR_TARJETAS_PIN(x)                      GET_MEMORY_STR_CHAR( __var_tarjetas_pin, x )
#define  VAR_TARJETAS_ANT_TIPO_CUENTA             GET_MEMORY_CHAR( __var_tarjetas_ant_tipo_cuenta )
#define  VAR_TARJETAS_ANT_PIN                     GET_MEMORY_STR( __var_tarjetas_ant_pin )
#define _VAR_TARJETAS_ANT_PIN(x)                  GET_MEMORY_STR_CHAR( __var_tarjetas_ant_pin, x )
#define  NRO_SUPERVISOR_ANT                       GET_MEMORY_INT( __nro_supervisor_ant )
#define  VAR_TARJETAS_CELDA_PINPAD                GET_MEMORY_CHAR( __var_tarjetas_celda_pinpad )
#define  VAR_TARJETAS_ANT_CELDA_PINPAD            GET_MEMORY_CHAR( __var_tarjetas_ant_celda_pinpad )
#define  VAR_TARJETAS_L_PIN                       GET_MEMORY_CHAR( __var_tarjetas_l_pin )
#define  VAR_TARJETAS_ANT_L_PIN                   GET_MEMORY_CHAR( __var_tarjetas_ant_l_pin )
#define  RAM_CANTIDAD_TOTAL                       GET_MEMORY_DOUBLE( __ram_cantidad_total )
#define  RAM_RENGLONES                            GET_MEMORY_INT( __ram_renglones )
#define  MUTUAL_CODIGO                            GET_MEMORY_INT( __mutual_codigo )
#define  MUTUAL_RECARGO                           GET_MEMORY_DOUBLE( __mutual_recargo )
#define  MUTUAL_ORDEN                             GET_MEMORY_LONG( __mutual_orden )
#define  MUTUAL_CUOTAS                            GET_MEMORY_CHAR( __mutual_cuotas )
#define  RAM_P_EXT_IMPUESTOS                      GET_MEMORY_INT( __ram_p_ext_impuestos )
#define  X_NC_A_IMPORTE                           GET_MEMORY_DOUBLE( __x_nc_a_importe )
#define  X_NC_A_CANTIDAD                          GET_MEMORY_INT( __x_nc_a_cantidad )
#define  X_NC_B_IMPORTE                           GET_MEMORY_DOUBLE( __x_nc_b_importe )
#define  X_NC_B_CANTIDAD                          GET_MEMORY_INT( __x_nc_b_cantidad )
#define  Z_NC_A_IMPORTE                           GET_MEMORY_DOUBLE( __z_nc_a_importe )
#define  Z_NC_A_CANTIDAD                          GET_MEMORY_INT( __z_nc_a_cantidad )
#define  Z_NC_B_IMPORTE                           GET_MEMORY_DOUBLE( __z_nc_b_importe )
#define  Z_NC_B_CANTIDAD                          GET_MEMORY_INT( __z_nc_b_cantidad )
#define  Z_ANT_NC_A_IMPORTE                       GET_MEMORY_DOUBLE( __z_ant_nc_a_importe )
#define  Z_ANT_NC_A_CANTIDAD                      GET_MEMORY_INT( __z_ant_nc_a_cantidad )
#define  Z_ANT_NC_B_IMPORTE                       GET_MEMORY_DOUBLE( __z_ant_nc_b_importe )
#define  Z_ANT_NC_B_CANTIDAD                      GET_MEMORY_INT( __z_ant_nc_b_cantidad )
#define  RAM_TICKET_EN_ESPERA                     GET_MEMORY_CHAR( __ram_ticket_en_espera )
#define  NRO_EVENTO                               GET_MEMORY_LONG( __nro_evento )
#define  ITEM_COD_CLASIFICACION                   GET_MEMORY_LONG( __item_cod_clasificacion )
#define  RAM_CAMBIO                               GET_MEMORY_CHAR( __ram_cambio )
#define  RAM_PEDIR_PRECIO                         GET_MEMORY_CHAR( __ram_pedir_precio )
#define  RAM_NEGATIVO                             GET_MEMORY_CHAR( __ram_negativo )
#define  RAM_BOLETA_ANTERIOR                      GET_MEMORY_LONG( __ram_boleta_anterior )
#define  RAM_PERFIL                               GET_MEMORY_INT( __ram_perfil )
#define  VAR_TARJETAS_MENSAJE                     GET_MEMORY_STR( __var_tarjetas_mensaje )
#define _VAR_TARJETAS_MENSAJE(x)                  GET_MEMORY_STR_CHAR( __var_tarjetas_mensaje, x )
#define  VAR_TARJETAS_ANT_MENSAJE                 GET_MEMORY_STR( __var_tarjetas_ant_mensaje )
#define _VAR_TARJETAS_ANT_MENSAJE(x)              GET_MEMORY_STR_CHAR( __var_tarjetas_ant_mensaje, x )
#define  RAM_DESCUENTO_PORCENTUAL                 GET_MEMORY_CHAR( __ram_descuento_porcentual )
#define  VAR_TARJETAS_COD_VIGENCIA                GET_MEMORY_CHAR( __var_tarjetas_cod_vigencia )
#define  VAR_TARJETAS_ANT_COD_VIGENCIA            GET_MEMORY_CHAR( __var_tarjetas_ant_cod_vigencia )
#define  RAM_NOM_PERFIL                           GET_MEMORY_STR( __ram_nom_perfil )
#define _RAM_NOM_PERFIL(x)                        GET_MEMORY_STR_CHAR( __ram_nom_perfil, x )
#define  RAM_RETENCION_DGR                        GET_MEMORY_DOUBLE( __ram_retencion_dgr )
#define  RAM_PERCEPCION_DGI                       GET_MEMORY_DOUBLE( __ram_percepcion_dgi )
#define  RAM_ANULAR_TICKET                        GET_MEMORY_CHAR( __ram_anular_ticket )
#define  RAM_PEDIR_NOVIOS                         GET_MEMORY_CHAR( __ram_pedir_novios )
#define  RAM_RESULTADO_TRANSAC                    GET_MEMORY_INT( __ram_resultado_transac )
#define  RAM_SUCURSAL_ANTERIOR                    GET_MEMORY_INT( __ram_sucursal_anterior )
#define  RAM_CAJA_ANTERIOR                        GET_MEMORY_INT( __ram_caja_anterior )
#define  RAM_IMPORTACION_ACTIVA                   GET_MEMORY_CHAR( __ram_importacion_activa )
#define  CONC_A_PAGAR_COD_FLETERO                 GET_MEMORY_INT( __conc_a_pagar_cod_fletero )
#define  CONC_A_PAGAR_COD_COMISIONISTA            GET_MEMORY_INT( __conc_a_pagar_cod_comisionista )
#define _CONC_A_PAGAR_CONCEPTOS(x)               _GET_MEMORY_FLOAT( __conc_a_pagar_conceptos, x )
#define  CONC_A_PAGAR_STATUS                      GET_MEMORY_CHAR( __conc_a_pagar_status )
#define  RAM_IMPORTE_AUT_CTACTE                   GET_MEMORY_DOUBLE( __ram_importe_aut_ctacte )
#define  RAM_IMPORTE_AUT_CHEQUE                   GET_MEMORY_DOUBLE( __ram_importe_aut_cheque )
#define _PAGO_IMP_MONEDA_EXT(x)                  _GET_MEMORY_DOUBLE( __pago_imp_moneda_ext, x )
#define _RAM_CORTE_CARRO(x)                      _GET_MEMORY_INT( __ram_corte_carro, x )
#define  RAM_NRO_CARRO                            GET_MEMORY_CHAR( __ram_nro_carro )
#define  __M_PLAZO_FINANCIACION_CLIENTE           GET_MEMORY_UNSIGNED( ____m_plazo_financiacion_cliente )
#define _PAGO_MONEDA(x)                          _GET_MEMORY_CHAR( __pago_moneda, x )
#define _PAGO_COD_VIGENCIA(x)                    _GET_MEMORY_CHAR( __pago_cod_vigencia, x )
#define _PAGO_PRIMER_VTO(x)                      _GET_MEMORY_UNSIGNED( __pago_primer_vto, x )
#define  CLIENTE_AFINIDAD_NOMBRE                  GET_MEMORY_STR( __cliente_afinidad_nombre )
#define _CLIENTE_AFINIDAD_NOMBRE(x)               GET_MEMORY_STR_CHAR( __cliente_afinidad_nombre, x )
#define  CLIENTE_AFINIDAD_PUNTAJE                 GET_MEMORY_DOUBLE( __cliente_afinidad_puntaje )
#define  CLIENTE_AFINIDAD_COD_ESTADO              GET_MEMORY_INT( __cliente_afinidad_cod_estado )
#define _PAGO_CUENTA_BCO(x)                      _GET_MEMORY_STR( __pago_cuenta_bco, x )
#define  RAM_NOVIOS                               GET_MEMORY_CHAR( __ram_novios )
#define _RAM_MEDIOS_OBLIGATORIOS(x)              _GET_MEMORY_CHAR( __ram_medios_obligatorios, x )
#define  RAM_IMPORTACION_PEDIDOS                  GET_MEMORY_CHAR( __ram_importacion_pedidos )
#define  RAM_TRANSAC_EN_COBRO                     GET_MEMORY_LONG( __ram_transac_en_cobro )
#define  VAR_TARJETAS_PIDE_ENTREGA                GET_MEMORY_CHAR( __var_tarjetas_pide_entrega )
#define  VAR_TARJETAS_ANT_PIDE_ENTREGA            GET_MEMORY_CHAR( __var_tarjetas_ant_pide_entrega )
#define  RAM_IMPRIMIR_VOUCHER_CMR                 GET_MEMORY_CHAR( __ram_imprimir_voucher_cmr )
#define  RAM_SUBINDICE_PAGO_CMR                   GET_MEMORY_INT( __ram_subindice_pago_cmr )
#define _PAGO_IMPORTE_ENTREGA(x)                 _GET_MEMORY_DOUBLE( __pago_importe_entrega, x )
#define  ITEM_STOCKEABLE                          GET_MEMORY_CHAR( __item_stockeable )
#define  RAM_ACU_PUNTOS                           GET_MEMORY_DOUBLE( __ram_acu_puntos )
#define  RAM_PERFIL_EXCLUIR_ARTICULOS             GET_MEMORY_CHAR( __ram_perfil_excluir_articulos )
#define  RAM_NOMBRE_CLIENTE_AFINIDAD              GET_MEMORY_STR( __ram_nombre_cliente_afinidad )
#define _RAM_NOMBRE_CLIENTE_AFINIDAD(x)           GET_MEMORY_STR_CHAR( __ram_nombre_cliente_afinidad, x )
#define  RAM_PUNTOS_AFINIDAD                      GET_MEMORY_DOUBLE( __ram_puntos_afinidad )
#define  RAM_FECHA_PUNTOS_AFINIDAD                GET_MEMORY_UNSIGNED( __ram_fecha_puntos_afinidad )
#define _VARIOS_USUARIOS_HABILITADOS(x)          _GET_MEMORY_INT( __varios_usuarios_habilitados, x )
#define  VARIOS_CAJON_ACTIVO                      GET_MEMORY_CHAR( __varios_cajon_activo )
#define  VAR_TARJETAS_NRO_ORDEN_CUOTA             GET_MEMORY_CHAR( __var_tarjetas_nro_orden_cuota )
#define  VAR_TARJETAS_ANT_NRO_ORDEN_CUOTA         GET_MEMORY_CHAR( __var_tarjetas_ant_nro_orden_cuota )
#define  ITEM_HABILITADO_EN_CUOTAS                GET_MEMORY_CHAR( __item_habilitado_en_cuotas )
#define  TOT_TICK_TOTAL_EN_CUOTAS                 GET_MEMORY_DOUBLE( __tot_tick_total_en_cuotas )
#define  TOT_TICK_ANT_TOTAL_EN_CUOTAS             GET_MEMORY_DOUBLE( __tot_tick_ant_total_en_cuotas )
#define  TOT_TICK_SV_TOTAL_EN_CUOTAS              GET_MEMORY_DOUBLE( __tot_tick_sv_total_en_cuotas )
#define  VAR_TARJETAS_CONTROL_CUOTAS              GET_MEMORY_CHAR( __var_tarjetas_control_cuotas )
#define  VAR_TARJETAS_ANT_CONTROL_CUOTAS          GET_MEMORY_CHAR( __var_tarjetas_ant_control_cuotas )
#define  NRO_CAJA_DGI                             GET_MEMORY_INT( __nro_caja_dgi )
#define _X_FONDO_FIJO_MULTIPLE_IMPORTE(x)        _GET_MEMORY_DOUBLE( __x_fondo_fijo_multiple_importe, x )
#define _X_FONDO_FIJO_MULTIPLE_CANTIDAD(x)       _GET_MEMORY_INT( __x_fondo_fijo_multiple_cantidad, x )
#define  X_RETENCION_DGR                          GET_MEMORY_DOUBLE( __x_retencion_dgr )
#define  X_PERCEPCION_DGI                         GET_MEMORY_DOUBLE( __x_percepcion_dgi )
#define _X_IMPORTE_IVA(x)                        _GET_MEMORY_DOUBLE( __x_importe_iva, x )
#define _X_IMPORTE_SOBRETASA_IVA(x)              _GET_MEMORY_DOUBLE( __x_importe_sobretasa_iva, x )
#define _Z_FONDO_FIJO_MULTIPLE_IMPORTE(x)        _GET_MEMORY_DOUBLE( __z_fondo_fijo_multiple_importe, x )
#define _Z_FONDO_FIJO_MULTIPLE_CANTIDAD(x)       _GET_MEMORY_INT( __z_fondo_fijo_multiple_cantidad, x )
#define  Z_RETENCION_DGR                          GET_MEMORY_DOUBLE( __z_retencion_dgr )
#define  Z_PERCEPCION_DGI                         GET_MEMORY_DOUBLE( __z_percepcion_dgi )
#define _Z_IMPORTE_IVA(x)                        _GET_MEMORY_DOUBLE( __z_importe_iva, x )
#define _Z_IMPORTE_SOBRETASA_IVA(x)              _GET_MEMORY_DOUBLE( __z_importe_sobretasa_iva, x )
#define _Z_ANT_FONDO_FIJO_MULTIPLE_IMPORTE(x)    _GET_MEMORY_DOUBLE( __z_ant_fondo_fijo_multiple_importe, x )
#define _Z_ANT_FONDO_FIJO_MULTIPLE_CANTIDAD(x)   _GET_MEMORY_INT( __z_ant_fondo_fijo_multiple_cantidad, x )
#define  Z_ANT_RETENCION_DGR                      GET_MEMORY_DOUBLE( __z_ant_retencion_dgr )
#define  Z_ANT_PERCEPCION_DGI                     GET_MEMORY_DOUBLE( __z_ant_percepcion_dgi )
#define _Z_ANT_IMPORTE_IVA(x)                    _GET_MEMORY_DOUBLE( __z_ant_importe_iva, x )
#define _Z_ANT_IMPORTE_SOBRETASA_IVA(x)          _GET_MEMORY_DOUBLE( __z_ant_importe_sobretasa_iva, x )
#define  ITEM_COD_IVA                             GET_MEMORY_CHAR( __item_cod_iva )
#define  NRO_LEGAJO_CAJERO                        GET_MEMORY_LONG( __nro_legajo_cajero )
#define  VARIOS_NRO_LEGAJO_CAJERO_Z               GET_MEMORY_LONG( __varios_nro_legajo_cajero_z )
#define  X_CORRECCION_IMPORTE                     GET_MEMORY_DOUBLE( __x_correccion_importe )
#define  X_CORRECCION_CANTIDAD                    GET_MEMORY_INT( __x_correccion_cantidad )
#define  Z_CORRECCION_IMPORTE                     GET_MEMORY_DOUBLE( __z_correccion_importe )
#define  Z_CORRECCION_CANTIDAD                    GET_MEMORY_INT( __z_correccion_cantidad )
#define  Z_ANT_CORRECCION_IMPORTE                 GET_MEMORY_DOUBLE( __z_ant_correccion_importe )
#define  Z_ANT_CORRECCION_CANTIDAD                GET_MEMORY_INT( __z_ant_correccion_cantidad )
#define  RAM_CORRECCION                           GET_MEMORY_CHAR( __ram_correccion )
#define  RAM_VUELTO_EFECTIVO                      GET_MEMORY_DOUBLE( __ram_vuelto_efectivo )
#define  RAM_DIFERENCIA_PRECIO                    GET_MEMORY_CHAR( __ram_diferencia_precio )
#define  RAM_COMPROMISO_200                       GET_MEMORY_CHAR( __ram_compromiso_200 )
#define  RAM_PRECIO_MAS_BAJO                      GET_MEMORY_CHAR( __ram_precio_mas_bajo )
#define  ITEM_COD_MOVIMIENTO                      GET_MEMORY_CHAR( __item_cod_movimiento )
#define  RAM_PRESENT_ANT_BAK                      GET_MEMORY_INT( __ram_present_ant_bak )
#define  NRO_EVENTO_A_PROCESAR                    GET_MEMORY_LONG( __nro_evento_a_procesar )
#define  RAM_ENVIAR_OFFLINE                       GET_MEMORY_CHAR( __ram_enviar_offline )
#define  X_NRO_PRIMER_TICKET                      GET_MEMORY_LONG( __x_nro_primer_ticket )
#define  X_CANTIDAD_TICKETS                       GET_MEMORY_LONG( __x_cantidad_tickets )
#define  Z_NRO_PRIMER_TICKET                      GET_MEMORY_LONG( __z_nro_primer_ticket )
#define  Z_CANTIDAD_TICKETS                       GET_MEMORY_LONG( __z_cantidad_tickets )
#define  Z_ANT_NRO_PRIMER_TICKET                  GET_MEMORY_LONG( __z_ant_nro_primer_ticket )
#define  Z_ANT_CANTIDAD_TICKETS                   GET_MEMORY_LONG( __z_ant_cantidad_tickets )
#define  RAM_P_PAGO_ANT                           GET_MEMORY_CHAR( __ram_p_pago_ant )
#define  VARIOS_MODO_Y_EN_Z                       GET_MEMORY_CHAR( __varios_modo_y_en_z )
#define  RAM_MOSTRAR_SALDO_CHEQUE                 GET_MEMORY_CHAR( __ram_mostrar_saldo_cheque )
#define  VAR_TARJETAS_CUPON_ORIGINAL              GET_MEMORY_LONG( __var_tarjetas_cupon_original )
#define  VAR_TARJETAS_FECHA_ORIGINAL              GET_MEMORY_UNSIGNED( __var_tarjetas_fecha_original )
#define  VAR_TARJETAS_ANT_CUPON_ORIGINAL          GET_MEMORY_LONG( __var_tarjetas_ant_cupon_original )
#define  VAR_TARJETAS_ANT_FECHA_ORIGINAL          GET_MEMORY_UNSIGNED( __var_tarjetas_ant_fecha_original )
#define  RAM_IMPRIMIR_CABECERA                    GET_MEMORY_CHAR( __ram_imprimir_cabecera )
#define  NRO_DIA_OPERATIVO_ULTIMA_Z               GET_MEMORY_UNSIGNED( __nro_dia_operativo_ultima_z )
#define  NRO_PROXIMO_DIA_OPERATIVO_SERVIDOR       GET_MEMORY_UNSIGNED( __nro_proximo_dia_operativo_servidor )
#define  VARIOS_COPIA_TICKET_PRN                  GET_MEMORY_CHAR( __varios_copia_ticket_prn )
#define _XC_VTAS_IMPORTE(x)                      _GET_MEMORY_DOUBLE( __xc_vtas_importe, x )
#define _XC_VTAS_CANTIDAD(x)                     _GET_MEMORY_INT( __xc_vtas_cantidad, x )
#define _XC_RETIROS_IMPORTE(x)                   _GET_MEMORY_DOUBLE( __xc_retiros_importe, x )
#define _XC_RETIROS_CANTIDAD(x)                  _GET_MEMORY_INT( __xc_retiros_cantidad, x )
#define  XC_ADELANTOS_IMPORTE                     GET_MEMORY_DOUBLE( __xc_adelantos_importe )
#define  XC_ADELANTOS_CANTIDAD                    GET_MEMORY_INT( __xc_adelantos_cantidad )
#define  XC_INTERESES_IMPORTE                     GET_MEMORY_DOUBLE( __xc_intereses_importe )
#define  XC_INTERESES_CANTIDAD                    GET_MEMORY_INT( __xc_intereses_cantidad )
#define  XC_FONDO_FIJO_IMPORTE                    GET_MEMORY_DOUBLE( __xc_fondo_fijo_importe )
#define  XC_FONDO_FIJO_CANTIDAD                   GET_MEMORY_INT( __xc_fondo_fijo_cantidad )
#define  XC_ANULACIONES_IMPORTE                   GET_MEMORY_DOUBLE( __xc_anulaciones_importe )
#define  XC_ANULACIONES_CANTIDAD                  GET_MEMORY_INT( __xc_anulaciones_cantidad )
#define  XC_DEV_ENVASES_IMPORTE                   GET_MEMORY_DOUBLE( __xc_dev_envases_importe )
#define  XC_DEV_ENVASES_CANTIDAD                  GET_MEMORY_INT( __xc_dev_envases_cantidad )
#define  XC_DEVOLUCIONES_IMPORTE                  GET_MEMORY_DOUBLE( __xc_devoluciones_importe )
#define  XC_DEVOLUCIONES_CANTIDAD                 GET_MEMORY_INT( __xc_devoluciones_cantidad )
#define  XC_VENTAS_GRAVADAS                       GET_MEMORY_DOUBLE( __xc_ventas_gravadas )
#define  XC_VENTAS_EXENTAS                        GET_MEMORY_DOUBLE( __xc_ventas_exentas )
#define  XC_IMP_INT                               GET_MEMORY_DOUBLE( __xc_imp_int )
#define _XC_DEP_IMP(x)                           _GET_MEMORY_DOUBLE( __xc_dep_imp, x )
#define _XC_DEMORA_SEGUNDOS(x)                   _GET_MEMORY_UNSIGNED( __xc_demora_segundos, x )
#define _XC_DEMORA_VENTAS(x)                     _GET_MEMORY_UNSIGNED( __xc_demora_ventas, x )
#define  XC_TOT_DINERO                            GET_MEMORY_DOUBLE( __xc_tot_dinero )
#define  XC_IVA1                                  GET_MEMORY_DOUBLE( __xc_iva1 )
#define  XC_IVA2                                  GET_MEMORY_DOUBLE( __xc_iva2 )
#define  XC_RETIRO_FONDO_IMPORTE                  GET_MEMORY_DOUBLE( __xc_retiro_fondo_importe )
#define  XC_RETIRO_FONDO_CANTIDAD                 GET_MEMORY_INT( __xc_retiro_fondo_cantidad )
#define _XC_MOZOS_IMPORTE(x)                     _GET_MEMORY_DOUBLE( __xc_mozos_importe, x )
#define _XC_MOZOS_CANTIDAD(x)                    _GET_MEMORY_DOUBLE( __xc_mozos_cantidad, x )
#define _XC_COBROS_IMPORTE(x)                    _GET_MEMORY_DOUBLE( __xc_cobros_importe, x )
#define _XC_COBROS_CANTIDAD(x)                   _GET_MEMORY_INT( __xc_cobros_cantidad, x )
#define  XC_PAGOS_POR_CAJA_IMPORTE                GET_MEMORY_DOUBLE( __xc_pagos_por_caja_importe )
#define  XC_PAGOS_POR_CAJA_CANTIDAD               GET_MEMORY_INT( __xc_pagos_por_caja_cantidad )
#define  XC_FACTURAS_A_IMPORTE                    GET_MEMORY_DOUBLE( __xc_facturas_a_importe )
#define  XC_FACTURAS_A_CANTIDAD                   GET_MEMORY_INT( __xc_facturas_a_cantidad )
#define  XC_FACTURAS_B_IMPORTE                    GET_MEMORY_DOUBLE( __xc_facturas_b_importe )
#define  XC_FACTURAS_B_CANTIDAD                   GET_MEMORY_INT( __xc_facturas_b_cantidad )
#define _XC_DEP_CAN(x)                           _GET_MEMORY_DOUBLE( __xc_dep_can, x )
#define  XC_VUELTOS_IMPORTE                       GET_MEMORY_DOUBLE( __xc_vueltos_importe )
#define  XC_VUELTOS_CANTIDAD                      GET_MEMORY_INT( __xc_vueltos_cantidad )
#define  XC_BONOS_ENTREGADOS                      GET_MEMORY_UNSIGNED( __xc_bonos_entregados )
#define  XC_ILA                                   GET_MEMORY_DOUBLE( __xc_ila )
#define  XC_IHA                                   GET_MEMORY_DOUBLE( __xc_iha )
#define  XC_DIFERENCIA                            GET_MEMORY_DOUBLE( __xc_diferencia )
#define  XC_NC_A_IMPORTE                          GET_MEMORY_DOUBLE( __xc_nc_a_importe )
#define  XC_NC_A_CANTIDAD                         GET_MEMORY_INT( __xc_nc_a_cantidad )
#define  XC_NC_B_IMPORTE                          GET_MEMORY_DOUBLE( __xc_nc_b_importe )
#define  XC_NC_B_CANTIDAD                         GET_MEMORY_INT( __xc_nc_b_cantidad )
#define _XC_FONDO_FIJO_MULTIPLE_IMPORTE(x)       _GET_MEMORY_DOUBLE( __xc_fondo_fijo_multiple_importe, x )
#define _XC_FONDO_FIJO_MULTIPLE_CANTIDAD(x)      _GET_MEMORY_INT( __xc_fondo_fijo_multiple_cantidad, x )
#define  XC_RETENCION_DGR                         GET_MEMORY_DOUBLE( __xc_retencion_dgr )
#define  XC_PERCEPCION_DGI                        GET_MEMORY_DOUBLE( __xc_percepcion_dgi )
#define _XC_IMPORTE_IVA(x)                       _GET_MEMORY_DOUBLE( __xc_importe_iva, x )
#define _XC_IMPORTE_SOBRETASA_IVA(x)             _GET_MEMORY_DOUBLE( __xc_importe_sobretasa_iva, x )
#define  XC_CORRECION_IMPORTE                     GET_MEMORY_DOUBLE( __xc_correcion_importe )
#define  XC_CORRECCION_CANTIDAD                   GET_MEMORY_INT( __xc_correccion_cantidad )
#define  XC_NRO_PRIMER_TICKET                     GET_MEMORY_LONG( __xc_nro_primer_ticket )
#define  XC_CANTIDAD_TICKETS                      GET_MEMORY_LONG( __xc_cantidad_tickets )
#define _PAGO_ID_REGISTRO(x)                     _GET_MEMORY_CHAR( __pago_id_registro, x )
#define  NRO_ULTIMA_NOVEDAD_CLIENTE               GET_MEMORY_LONG( __nro_ultima_novedad_cliente )
#define  VAR_TARJETAS_RECARGO_DIFERIM             GET_MEMORY_FLOAT( __var_tarjetas_recargo_diferim )
#define  VAR_TARJETAS_ANT_RECARGO_DIFERIM         GET_MEMORY_FLOAT( __var_tarjetas_ant_recargo_diferim )
#define  ITEM_ATRIB_DESCUENTOS                    GET_MEMORY_CHAR( __item_atrib_descuentos )
#define  RAM_PEDIR_DIFERIMIENTO                   GET_MEMORY_CHAR( __ram_pedir_diferimiento )
#define  RAM_TRANSACCION_ENVIADA                  GET_MEMORY_CHAR( __ram_transaccion_enviada )
#define  RAM_NRO_TRANSACCION_PEDIDA               GET_MEMORY_LONG( __ram_nro_transaccion_pedida )
#define  ITEM_HABILITADO_EN_PROMOCION             GET_MEMORY_CHAR( __item_habilitado_en_promocion )
#define _RAM_MULT_TICKET_EN_ESPERA(x)            _GET_MEMORY_CHAR( __ram_mult_ticket_en_espera, x )
#define _RAM_HORA_MULT_TICKET_EN_ESPERA(x)       _GET_MEMORY_LONG( __ram_hora_mult_ticket_en_espera, x )
#define  PERMITE_REPACTAR_TARJETA_CMR             GET_MEMORY_INT( __permite_repactar_tarjeta_cmr )
#define  X_COBROS_CANTIDAD_SERVICIOS              GET_MEMORY_INT( __x_cobros_cantidad_servicios )
#define  Z_COBROS_CANTIDAD_SERVICIOS              GET_MEMORY_INT( __z_cobros_cantidad_servicios )
#define  Z_ANT_COBROS_CANTIDAD_SERVICIOS          GET_MEMORY_INT( __z_ant_cobros_cantidad_servicios )
#define  RAM_TARJ_DOCUMENTO                       GET_MEMORY_STR( __ram_tarj_documento )
#define _RAM_TARJ_DOCUMENTO(x)                    GET_MEMORY_STR_CHAR( __ram_tarj_documento, x )
#define  RAM_TARJ_TELEFONO                        GET_MEMORY_STR( __ram_tarj_telefono )
#define _RAM_TARJ_TELEFONO(x)                     GET_MEMORY_STR_CHAR( __ram_tarj_telefono, x )
#define _RAM_DESC_PORC_PENDIENTES(x)             _GET_MEMORY_CHAR( __ram_desc_porc_pendientes, x )
#define  RAM_MODO_EJECUCION                       GET_MEMORY_CHAR( __ram_modo_ejecucion )
#define  RAM_MODO_MEMORIA_ANTERIOR                GET_MEMORY_CHAR( __ram_modo_memoria_anterior )
#define  RAM_CANTIDAD_CONTROLADA                  GET_MEMORY_CHAR( __ram_cantidad_controlada )
#define  RAM_BIEN_DE_USO                          GET_MEMORY_CHAR( __ram_bien_de_uso )
#define  NRO_ULTIMO_EVENTO_ANALIZADO              GET_MEMORY_LONG( __nro_ultimo_evento_analizado )
#define  NRO_ULTIMO_EVENTO_CONTROLADO             GET_MEMORY_LONG( __nro_ultimo_evento_controlado )
#define  RAM_FECHA_ANTERIOR                       GET_MEMORY_INT( __ram_fecha_anterior )
#define  RAM_TIPO_COMP_ANTERIOR                   GET_MEMORY_CHAR( __ram_tipo_comp_anterior )
#define  XC_COBROS_CANTIDAD_SERVICIOS             GET_MEMORY_INT( __xc_cobros_cantidad_servicios )
#define  RAM_PERCEPCION_212_IVA1                  GET_MEMORY_DOUBLE( __ram_percepcion_212_iva1 )
#define  RAM_PERCEPCION_212_IVA2                  GET_MEMORY_DOUBLE( __ram_percepcion_212_iva2 )
#define  X_PERCEPCION_212_IVA1                    GET_MEMORY_DOUBLE( __x_percepcion_212_iva1 )
#define  X_PERCEPCION_212_IVA2                    GET_MEMORY_DOUBLE( __x_percepcion_212_iva2 )
#define  Z_PERCEPCION_212_IVA1                    GET_MEMORY_DOUBLE( __z_percepcion_212_iva1 )
#define  Z_PERCEPCION_212_IVA2                    GET_MEMORY_DOUBLE( __z_percepcion_212_iva2 )
#define  Z_ANT_PERCEPCION_212_IVA1                GET_MEMORY_DOUBLE( __z_ant_percepcion_212_iva1 )
#define  Z_ANT_PERCEPCION_212_IVA2                GET_MEMORY_DOUBLE( __z_ant_percepcion_212_iva2 )
#define  XC_PERCEPCION_212_IVA1                   GET_MEMORY_DOUBLE( __xc_percepcion_212_iva1 )
#define  XC_PERCEPCION_212_IVA2                   GET_MEMORY_DOUBLE( __xc_percepcion_212_iva2 )
#define  RAM_TARJ_NUMERO                          GET_MEMORY_STR( __ram_tarj_numero )
#define _RAM_TARJ_NUMERO(x)                       GET_MEMORY_STR_CHAR( __ram_tarj_numero, x )
#define  RAM_TARJ_IMPORTE                         GET_MEMORY_DOUBLE( __ram_tarj_importe )
#define  RAM_TARJ_CUOTAS                          GET_MEMORY_CHAR( __ram_tarj_cuotas )
#define _ITEM_COD_PROMOCION(x)                   _GET_MEMORY_INT( __item_cod_promocion, x )
#define  X_DIFERENCIA_REDONDEO                    GET_MEMORY_DOUBLE( __x_diferencia_redondeo )
#define  Z_DIFERENCIA_REDONDEO                    GET_MEMORY_DOUBLE( __z_diferencia_redondeo )
#define  Z_ANT_DIFERENCIA_REDONDEO                GET_MEMORY_DOUBLE( __z_ant_diferencia_redondeo )
#define  XC_DIFERENCIA_REDONDEO                   GET_MEMORY_DOUBLE( __xc_diferencia_redondeo )
#define  RAM_DIFERENCIA_REDONDEO                  GET_MEMORY_DOUBLE( __ram_diferencia_redondeo )
#define  RAM_CANTIDAD_ORIGINAL                    GET_MEMORY_DOUBLE( __ram_cantidad_original )
#define  NRO_EVENTOS_PENDIENTES_Z_ANTERIOR        GET_MEMORY_CHAR( __nro_eventos_pendientes_z_anterior )
#define  RAM_PERMITE_ANULAR_COBRO                 GET_MEMORY_CHAR( __ram_permite_anular_cobro )
#define  VARIOS_COPIA_COBRO_PRN                   GET_MEMORY_CHAR( __varios_copia_cobro_prn )
#define  VARIOS_TARJETAS_POR_PAGO                 GET_MEMORY_INT( __varios_tarjetas_por_pago )
#define  RAM_STRING_CLIENTE_AFINIDAD              GET_MEMORY_STR( __ram_string_cliente_afinidad )
#define _RAM_STRING_CLIENTE_AFINIDAD(x)           GET_MEMORY_STR_CHAR( __ram_string_cliente_afinidad, x )
#define  RAM_USA_TEMPLATE                         GET_MEMORY_CHAR( __ram_usa_template )
#define  RAM_IMP_DESCUENTO                        GET_MEMORY_DOUBLE( __ram_imp_descuento )
#define  RAM_TMPO_REG_ARTICULO                    GET_MEMORY_INT( __ram_tmpo_reg_articulo )
#define  RAM_TMPO_PAGO                            GET_MEMORY_INT( __ram_tmpo_pago )
#define  RAM_TMPO_STANDBY                         GET_MEMORY_INT( __ram_tmpo_standby )
#define  RAM_MODO_DEVOLUCION                      GET_MEMORY_INT( __ram_modo_devolucion )
#define  RAM_TMPO_OCIO                            GET_MEMORY_INT( __ram_tmpo_ocio )
#define  X_IMP_DEVOLUCIONES                       GET_MEMORY_DOUBLE( __x_imp_devoluciones )
#define  Z_IMP_DEVOLUCIONES                       GET_MEMORY_DOUBLE( __z_imp_devoluciones )
#define  Z_ANT_IMP_DEVOLUCIONES                   GET_MEMORY_DOUBLE( __z_ant_imp_devoluciones )
#define  XC_IMP_DEVOLUCIONES                      GET_MEMORY_DOUBLE( __xc_imp_devoluciones )
#define  X_CANT_DEVOLUCIONES                      GET_MEMORY_INT( __x_cant_devoluciones )
#define  Z_CANT_DEVOLUCIONES                      GET_MEMORY_INT( __z_cant_devoluciones )
#define  Z_ANT_CANT_DEVOLUCIONES                  GET_MEMORY_INT( __z_ant_cant_devoluciones )
#define  XC_CANT_DEVOLUCIONES                     GET_MEMORY_INT( __xc_cant_devoluciones )
#define  X_CANT_ARTICULOS                         GET_MEMORY_INT( __x_cant_articulos )
#define  Z_CANT_ARTICULOS                         GET_MEMORY_INT( __z_cant_articulos )
#define  Z_ANT_CANT_ARTICULOS                     GET_MEMORY_INT( __z_ant_cant_articulos )
#define  XC_CANT_ARTICULOS                        GET_MEMORY_INT( __xc_cant_articulos )
#define  X_CANT_ART_ESCANEADOS                    GET_MEMORY_INT( __x_cant_art_escaneados )
#define  Z_CANT_ART_ESCANEADOS                    GET_MEMORY_INT( __z_cant_art_escaneados )
#define  Z_ANT_CANT_ART_ESCANEADOS                GET_MEMORY_INT( __z_ant_cant_art_escaneados )
#define  XC_CANT_ART_ESCANEADOS                   GET_MEMORY_INT( __xc_cant_art_escaneados )
#define  X_TMPO_REG_ARTICULO                      GET_MEMORY_UNSIGNED( __x_tmpo_reg_articulo )
#define  Z_TMPO_REG_ARTICULO                      GET_MEMORY_UNSIGNED( __z_tmpo_reg_articulo )
#define  Z_ANT_TMPO_REG_ARTICULO                  GET_MEMORY_UNSIGNED( __z_ant_tmpo_reg_articulo )
#define  XC_TMPO_REG_ARTICULO                     GET_MEMORY_UNSIGNED( __xc_tmpo_reg_articulo )
#define  X_TMPO_PAGO                              GET_MEMORY_UNSIGNED( __x_tmpo_pago )
#define  Z_TMPO_PAGO                              GET_MEMORY_UNSIGNED( __z_tmpo_pago )
#define  Z_ANT_TMPO_PAGO                          GET_MEMORY_UNSIGNED( __z_ant_tmpo_pago )
#define  XC_TMPO_PAGO                             GET_MEMORY_UNSIGNED( __xc_tmpo_pago )
#define  X_TMPO_STANDBY                           GET_MEMORY_UNSIGNED( __x_tmpo_standby )
#define  Z_TMPO_STANDBY                           GET_MEMORY_UNSIGNED( __z_tmpo_standby )
#define  Z_ANT_TMPO_STANDBY                       GET_MEMORY_UNSIGNED( __z_ant_tmpo_standby )
#define  XC_TMPO_STANDBY                          GET_MEMORY_UNSIGNED( __xc_tmpo_standby )
#define  NRO_ESTADO_ACTUAL                        GET_MEMORY_CHAR( __nro_estado_actual )
#define  NRO_ENTRADA_ESTADO_ACTUAL                GET_MEMORY_LONG( __nro_entrada_estado_actual )
#define  X_TMPO_OCIO                              GET_MEMORY_UNSIGNED( __x_tmpo_ocio )
#define  Z_TMPO_OCIO                              GET_MEMORY_UNSIGNED( __z_tmpo_ocio )
#define  Z_ANT_TMPO_OCIO                          GET_MEMORY_UNSIGNED( __z_ant_tmpo_ocio )
#define  XC_TMPO_OCIO                             GET_MEMORY_UNSIGNED( __xc_tmpo_ocio )
#define  X_IMP_CANCELADOS                         GET_MEMORY_DOUBLE( __x_imp_cancelados )
#define  Z_IMP_CANCELADOS                         GET_MEMORY_DOUBLE( __z_imp_cancelados )
#define  Z_ANT_IMP_CANCELADOS                     GET_MEMORY_DOUBLE( __z_ant_imp_cancelados )
#define  XC_IMP_CANCELADOS                        GET_MEMORY_DOUBLE( __xc_imp_cancelados )
#define  X_CANT_CANCELADOS                        GET_MEMORY_INT( __x_cant_cancelados )
#define  Z_CANT_CANCELADOS                        GET_MEMORY_INT( __z_cant_cancelados )
#define  Z_ANT_CANT_CANCELADOS                    GET_MEMORY_INT( __z_ant_cant_cancelados )
#define  XC_CANT_CANCELADOS                       GET_MEMORY_INT( __xc_cant_cancelados )
#define  RAM_TOMADO_POR_TEMPLATE                  GET_MEMORY_DOUBLE( __ram_tomado_por_template )
#define  NRO_ULTIMA_NOVEDAD_BOLETIN               GET_MEMORY_LONG( __nro_ultima_novedad_boletin )
#define  VARIOS_RESTRINGIR_VOLVER_DE_PAGOS        GET_MEMORY_CHAR( __varios_restringir_volver_de_pagos )
#define  RAM_TICKET_FACTURA                       GET_MEMORY_CHAR( __ram_ticket_factura )
#define  RAM_IMPRESION_FISCAL                     GET_MEMORY_CHAR( __ram_impresion_fiscal )
#define  VARIOS_IMPRIMIENDO_TEXTO_FISCAL          GET_MEMORY_CHAR( __varios_imprimiendo_texto_fiscal )
#define  ITEM_COD_COBRO                           GET_MEMORY_INT( __item_cod_cobro )
#define  ITEM_COMPROBANTE                         GET_MEMORY_LONG( __item_comprobante )
#define _PAGO_VUELTO_MEDIO(x)                    _GET_MEMORY_DOUBLE( __pago_vuelto_medio, x )
#define _PAGO_COD_MEDIO_VUELTO(x)                _GET_MEMORY_CHAR( __pago_cod_medio_vuelto, x )
#define _PAGO_COD_SUBMEDIO_VUELTO(x)             _GET_MEMORY_CHAR( __pago_cod_submedio_vuelto, x )
#define _PAGO_TIPO_VUELTO(x)                     _GET_MEMORY_CHAR( __pago_tipo_vuelto, x )
#define  RAM_DESCUENTO_IMPORTE                    GET_MEMORY_CHAR( __ram_descuento_importe )
#define _X_CAMBIOS_IMPORTE(x)                    _GET_MEMORY_DOUBLE( __x_cambios_importe, x )
#define _X_CAMBIOS_CANTIDAD(x)                   _GET_MEMORY_INT( __x_cambios_cantidad, x )
#define _Z_CAMBIOS_IMPORTE(x)                    _GET_MEMORY_DOUBLE( __z_cambios_importe, x )
#define _Z_CAMBIOS_CANTIDAD(x)                   _GET_MEMORY_INT( __z_cambios_cantidad, x )
#define _Z_ANT_CAMBIOS_IMPORTE(x)                _GET_MEMORY_DOUBLE( __z_ant_cambios_importe, x )
#define _Z_ANT_CAMBIOS_CANTIDAD(x)               _GET_MEMORY_INT( __z_ant_cambios_cantidad, x )
#define _XC_CAMBIOS_IMPORTE(x)                   _GET_MEMORY_DOUBLE( __xc_cambios_importe, x )
#define _XC_CAMBIOS_CANTIDAD(x)                  _GET_MEMORY_INT( __xc_cambios_cantidad, x )
#define  RAM_AFINIDAD_FECHA_NAC                   GET_MEMORY_INT( __ram_afinidad_fecha_nac )
#define  RAM_CODIGO_TARJETA_AFINIDAD              GET_MEMORY_INT( __ram_codigo_tarjeta_afinidad )
#define  RAM_DIFERENCIA_AFINIDAD                  GET_MEMORY_DOUBLE( __ram_diferencia_afinidad )
#define  RAM_CANT_ARTICULOS_AFINIDAD              GET_MEMORY_INT( __ram_cant_articulos_afinidad )
#define  RAM_UTILIZA_PRECIO_AFINIDAD              GET_MEMORY_CHAR( __ram_utiliza_precio_afinidad )
#define  ITEM_DESCUENTO_CANTIDAD                  GET_MEMORY_DOUBLE( __item_descuento_cantidad )
#define  RAM_PUNTOS_USADOS                        GET_MEMORY_DOUBLE( __ram_puntos_usados )
#define  RAM_MOTIVO_DESCUENTO                     GET_MEMORY_INT( __ram_motivo_descuento )
#define _RAM_MOTIVO_DESCUENTO_PENDIENTE(x)       _GET_MEMORY_INT( __ram_motivo_descuento_pendiente, x )
#define _PAGO_IMPORTE_ME(x)                      _GET_MEMORY_DOUBLE( __pago_importe_me, x )
#define _X_VENTAS_IMPORTE_ME(x)                  _GET_MEMORY_DOUBLE( __x_ventas_importe_me, x )
#define _X_FONDO_FIJO_MULTIPLE_IMP_ME(x)         _GET_MEMORY_DOUBLE( __x_fondo_fijo_multiple_imp_me, x )
#define _X_RETIROS_IMPORTE_ME(x)                 _GET_MEMORY_DOUBLE( __x_retiros_importe_me, x )
#define _Z_VENTAS_IMPORTE_ME(x)                  _GET_MEMORY_DOUBLE( __z_ventas_importe_me, x )
#define _Z_FONDO_FIJO_MULTIPLE_IMP_ME(x)         _GET_MEMORY_DOUBLE( __z_fondo_fijo_multiple_imp_me, x )
#define _Z_RETIROS_IMPORTE_ME(x)                 _GET_MEMORY_DOUBLE( __z_retiros_importe_me, x )
#define _Z_ANT_VENTAS_IMPORTE_ME(x)              _GET_MEMORY_DOUBLE( __z_ant_ventas_importe_me, x )
#define _Z_ANT_FONDO_FIJO_MULTIPLE_IMP_M(x)      _GET_MEMORY_DOUBLE( __z_ant_fondo_fijo_multiple_imp_m, x )
#define _Z_ANT_RETIROS_IMPORTE_ME(x)             _GET_MEMORY_DOUBLE( __z_ant_retiros_importe_me, x )
#define _XC_VENTAS_IMPORTE_ME(x)                 _GET_MEMORY_DOUBLE( __xc_ventas_importe_me, x )
#define _XC_FONDO_FIJO_MULTIPLE_IMP_ME(x)        _GET_MEMORY_DOUBLE( __xc_fondo_fijo_multiple_imp_me, x )
#define _XC_RETIROS_IMPORTE_ME(x)                _GET_MEMORY_DOUBLE( __xc_retiros_importe_me, x )
#define  RAM_RESTO_ME                             GET_MEMORY_DOUBLE( __ram_resto_me )
#define  RAM_MOSTRAR_FALTANTE_EN_ME               GET_MEMORY_CHAR( __ram_mostrar_faltante_en_me )
#define  VAR_TARJETAS_TRACK1                      GET_MEMORY_STR( __var_tarjetas_track1 )
#define _VAR_TARJETAS_TRACK1(x)                   GET_MEMORY_STR_CHAR( __var_tarjetas_track1, x )
#define  VAR_TARJETAS_TRACK2                      GET_MEMORY_STR( __var_tarjetas_track2 )
#define _VAR_TARJETAS_TRACK2(x)                   GET_MEMORY_STR_CHAR( __var_tarjetas_track2, x )
#define  VAR_TARJETAS_ANT_TRACK1                  GET_MEMORY_STR( __var_tarjetas_ant_track1 )
#define _VAR_TARJETAS_ANT_TRACK1(x)               GET_MEMORY_STR_CHAR( __var_tarjetas_ant_track1, x )
#define  VAR_TARJETAS_ANT_TRACK2                  GET_MEMORY_STR( __var_tarjetas_ant_track2 )
#define _VAR_TARJETAS_ANT_TRACK2(x)               GET_MEMORY_STR_CHAR( __var_tarjetas_ant_track2, x )
#define _TOT_TICK_IMP_TASA(x)                    _GET_MEMORY_DOUBLE( __tot_tick_imp_tasa, x )
#define _TOT_TICK_IMP_SOBRETASA(x)               _GET_MEMORY_DOUBLE( __tot_tick_imp_sobretasa, x )
#define _TOT_TICK_IMP_NETO_GRAVADO(x)            _GET_MEMORY_DOUBLE( __tot_tick_imp_neto_gravado, x )
#define _TOT_TICK_ANT_IMP_TASA(x)                _GET_MEMORY_DOUBLE( __tot_tick_ant_imp_tasa, x )
#define _TOT_TICK_ANT_IMP_SOBRETASA(x)           _GET_MEMORY_DOUBLE( __tot_tick_ant_imp_sobretasa, x )
#define _TOT_TICK_ANT_IMP_NETO_GRAVADO(x)        _GET_MEMORY_DOUBLE( __tot_tick_ant_imp_neto_gravado, x )
#define _TOT_TICK_SV_IMP_TASA(x)                 _GET_MEMORY_DOUBLE( __tot_tick_sv_imp_tasa, x )
#define _TOT_TICK_SV_IMP_SOBRETASA(x)            _GET_MEMORY_DOUBLE( __tot_tick_sv_imp_sobretasa, x )
#define _TOT_TICK_SV_IMP_NETO_GRAVADO(x)         _GET_MEMORY_DOUBLE( __tot_tick_sv_imp_neto_gravado, x )
#define  RAM_CONTROL_CAMBIO                       GET_MEMORY_CHAR( __ram_control_cambio )
#define  RAM_TOTAL_DONACION                       GET_MEMORY_DOUBLE( __ram_total_donacion )
#define  X_DONACIONES_CANTIDAD                    GET_MEMORY_INT( __x_donaciones_cantidad )
#define  X_DONACIONES_IMPORTE                     GET_MEMORY_DOUBLE( __x_donaciones_importe )
#define  Z_DONACIONES_CANTIDAD                    GET_MEMORY_INT( __z_donaciones_cantidad )
#define  Z_DONACIONES_IMPORTE                     GET_MEMORY_DOUBLE( __z_donaciones_importe )
#define  Z_ANT_DONACIONES_CANTIDAD                GET_MEMORY_INT( __z_ant_donaciones_cantidad )
#define  Z_ANT_DONACIONES_IMPORTE                 GET_MEMORY_DOUBLE( __z_ant_donaciones_importe )
#define  XC_DONACIONES_CANTIDAD                   GET_MEMORY_INT( __xc_donaciones_cantidad )
#define  XC_DONACIONES_IMPORTE                    GET_MEMORY_DOUBLE( __xc_donaciones_importe )
#define  RAM_MOTIVO_DEVOLUCION                    GET_MEMORY_INT( __ram_motivo_devolucion )
#define _PAGO_COD_SOLICITUD(x)                   _GET_MEMORY_INT( __pago_cod_solicitud, x )
#define  RAM_STRING_LEGAJO_EMPLEADO               GET_MEMORY_STR( __ram_string_legajo_empleado )
#define _RAM_STRING_LEGAJO_EMPLEADO(x)            GET_MEMORY_STR_CHAR( __ram_string_legajo_empleado, x )
#define  NRO_VENDEDOR_LONG                        GET_MEMORY_LONG( __nro_vendedor_long )
#define  ITEM_COD_SOLICITUD                       GET_MEMORY_INT( __item_cod_solicitud )
#define  ITEM_PEDIR_SUPERVISOR                    GET_MEMORY_CHAR( __item_pedir_supervisor )
#define _PAGO_IVA_PROPORCIONAL(x)                _GET_MEMORY_DOUBLE( __pago_iva_proporcional, x )
#define  RAM_INSTITUCION_BENEFICIARIA             GET_MEMORY_CHAR( __ram_institucion_beneficiaria )
#define  RAM_EMITIENDO_Z                          GET_MEMORY_CHAR( __ram_emitiendo_z )
#define  NRO_CORRELATIVO_FONDO_FIJO               GET_MEMORY_LONG( __nro_correlativo_fondo_fijo )
#define  NRO_CORRELATIVO_RETIRO                   GET_MEMORY_LONG( __nro_correlativo_retiro )
#define  RAM_ID_ULTIMO_EVENTO                     GET_MEMORY_LONG( __ram_id_ultimo_evento )
#define  ITEM_ALICUOTA_PRECIO                     GET_MEMORY_FLOAT( __item_alicuota_precio )
#define  X_ALICUOTA                               GET_MEMORY_DOUBLE( __x_alicuota )
#define  Z_ALICUOTA                               GET_MEMORY_DOUBLE( __z_alicuota )
#define  Z_ANT_ALICUOTA                           GET_MEMORY_DOUBLE( __z_ant_alicuota )
#define  XC_ALICUOTA                              GET_MEMORY_DOUBLE( __xc_alicuota )
#define  TOT_TICK_ALICUOTA                        GET_MEMORY_DOUBLE( __tot_tick_alicuota )
#define  TOT_TICK_ANT_ALICUOTA                    GET_MEMORY_DOUBLE( __tot_tick_ant_alicuota )
#define  TOT_TICK_SV_ALICUOTA                     GET_MEMORY_DOUBLE( __tot_tick_sv_alicuota )
#define  NRO_CORRELATIVO_INVENTARIO               GET_MEMORY_LONG( __nro_correlativo_inventario )
#define  RAM_CLIENTE_AFINIDAD_ADICIONAL           GET_MEMORY_LONG( __ram_cliente_afinidad_adicional )
#define  RAM_FECHA_CUMPLE_AFI_ADICIONAL           GET_MEMORY_UNSIGNED( __ram_fecha_cumple_afi_adicional )
#define  RAM_NOM_CLI_AFINIDAD_ADICIONAL           GET_MEMORY_STR( __ram_nom_cli_afinidad_adicional )
#define _RAM_NOM_CLI_AFINIDAD_ADICIONAL(x)        GET_MEMORY_STR_CHAR( __ram_nom_cli_afinidad_adicional, x )
#define  RAM_DESCUENTO_AFINIDAD_ADICIONAL         GET_MEMORY_FLOAT( __ram_descuento_afinidad_adicional )
#define  RAM_COD_MUTUAL_AFINIDAD_ADICIONAL        GET_MEMORY_INT( __ram_cod_mutual_afinidad_adicional )
#define _RAM_NUMERO_TICKET_TOMRA(x)              _GET_MEMORY_LONG( __ram_numero_ticket_tomra, x )
#define  RAM_ACU_PUNTOS_CUMPLE                    GET_MEMORY_DOUBLE( __ram_acu_puntos_cumple )
#define  RAM_HOY_CUMPLE_ANIOS                     GET_MEMORY_CHAR( __ram_hoy_cumple_anios )
#define _RAM_PROMOCIONES_AFINIDAD(x)             _GET_MEMORY_CHAR( __ram_promociones_afinidad, x )
#define _RAM_PROMOCIONES_AFINIDAD_USADAS(x)      _GET_MEMORY_CHAR( __ram_promociones_afinidad_usadas, x )
#define _PAGO_TELEFONO_OTRO(x)                   _GET_MEMORY_STR( __pago_telefono_otro, x )
#define  RAM_PUNTOS_ESPECIALES_AFINIDAD           GET_MEMORY_DOUBLE( __ram_puntos_especiales_afinidad )
#define _RAM_PROM_CUPON_LUGAR_IMPRESION(x)       _GET_MEMORY_CHAR( __ram_prom_cupon_lugar_impresion, x )
#define _RAM_CUPON_LEYENDA(x)                    _GET_MEMORY_STR( __ram_cupon_leyenda, x )
#define _RAM_PROM_CUPON_IMPORTE(x)               _GET_MEMORY_DOUBLE( __ram_prom_cupon_importe, x )
#define  ITEM_HAB_DESC_CLIENTE                    GET_MEMORY_CHAR( __item_hab_desc_cliente )
#define  RAM_CODIGO_SUPERVISOR                    GET_MEMORY_LONG( __ram_codigo_supervisor )
#define  NRO_RECUPERO_Z                           GET_MEMORY_INT( __nro_recupero_z )
#define  RAM_VENTA_FRACCION                       GET_MEMORY_CHAR( __ram_venta_fraccion )
#define  ITEM_COD_BANDA                           GET_MEMORY_INT( __item_cod_banda )
#define  RAM_CADENA_CHEQUE                        GET_MEMORY_STR( __ram_cadena_cheque )
#define _RAM_CADENA_CHEQUE(x)                     GET_MEMORY_STR_CHAR( __ram_cadena_cheque, x )
#define _RAM_ACUMULADOR(x)                       _GET_MEMORY_DOUBLE( __ram_acumulador, x )
#define  RAM_COD_BOLETA_PLATA                     GET_MEMORY_STR( __ram_cod_boleta_plata )
#define _RAM_COD_BOLETA_PLATA(x)                  GET_MEMORY_STR_CHAR( __ram_cod_boleta_plata, x )
#define _RAM_CANT_ENVASES(x)                     _GET_MEMORY_INT( __ram_cant_envases, x )
#define  ITEM_TIPO_ENVASE                         GET_MEMORY_INT( __item_tipo_envase )
#define  RAM_REFERENCIA_NC                        GET_MEMORY_CHAR( __ram_referencia_nc )
#define  VAR_TARJETAS_FECHA_CONTABLE              GET_MEMORY_UNSIGNED( __var_tarjetas_fecha_contable )
#define  VAR_TARJETAS_ANT_FECHA_CONTABLE          GET_MEMORY_UNSIGNED( __var_tarjetas_ant_fecha_contable )
#define _RAM_ACUMULADOR_FECHA(x)                 _GET_MEMORY_UNSIGNED( __ram_acumulador_fecha, x )
#define _RAM_PROM_CUPON_TIPO(x)                  _GET_MEMORY_CHAR( __ram_prom_cupon_tipo, x )
#define _RAM_PROM_CUPON_FORMATO_MONITOR(x)       _GET_MEMORY_INT( __ram_prom_cupon_formato_monitor, x )
#define _RAM_PROM_CUPON_GRUPO(x)                 _GET_MEMORY_INT( __ram_prom_cupon_grupo, x )
#define _RAM_PROM_CUPON_NRO_ACUMULADOR(x)        _GET_MEMORY_INT( __ram_prom_cupon_nro_acumulador, x )
#define _X_DESC_PORC_IMPORTE(x)                  _GET_MEMORY_DOUBLE( __x_desc_porc_importe, x )
#define _X_DESC_PORC_CANTIDAD(x)                 _GET_MEMORY_INT( __x_desc_porc_cantidad, x )
#define  X_DESC_PORC_ULT_ART_IMPORTE              GET_MEMORY_DOUBLE( __x_desc_porc_ult_art_importe )
#define  X_DESC_PORC_ULT_ART_CANTIDAD             GET_MEMORY_INT( __x_desc_porc_ult_art_cantidad )
#define _X_DESC_PESOS_IMPORTE(x)                 _GET_MEMORY_DOUBLE( __x_desc_pesos_importe, x )
#define _X_DESC_PESOS_CANTIDAD(x)                _GET_MEMORY_INT( __x_desc_pesos_cantidad, x )
#define  X_DESC_PESOS_ULT_ART_IMPORTE             GET_MEMORY_DOUBLE( __x_desc_pesos_ult_art_importe )
#define  X_DESC_PESOS_ULT_ART_CANTIDAD            GET_MEMORY_INT( __x_desc_pesos_ult_art_cantidad )
#define _Z_DESC_PORC_IMPORTE(x)                  _GET_MEMORY_DOUBLE( __z_desc_porc_importe, x )
#define _Z_DESC_PORC_CANTIDAD(x)                 _GET_MEMORY_INT( __z_desc_porc_cantidad, x )
#define  Z_DESC_PORC_ULT_ART_IMPORTE              GET_MEMORY_DOUBLE( __z_desc_porc_ult_art_importe )
#define  Z_DESC_PORC_ULT_ART_CANTIDAD             GET_MEMORY_INT( __z_desc_porc_ult_art_cantidad )
#define _Z_DESC_PESOS_IMPORTE(x)                 _GET_MEMORY_DOUBLE( __z_desc_pesos_importe, x )
#define _Z_DESC_PESOS_CANTIDAD(x)                _GET_MEMORY_INT( __z_desc_pesos_cantidad, x )
#define  Z_DESC_PESOS_ULT_ART_IMPORTE             GET_MEMORY_DOUBLE( __z_desc_pesos_ult_art_importe )
#define  Z_DESC_PESOS_ULT_ART_CANTIDAD            GET_MEMORY_INT( __z_desc_pesos_ult_art_cantidad )
#define _Z_ANT_DESC_PORC_IMPORTE(x)              _GET_MEMORY_DOUBLE( __z_ant_desc_porc_importe, x )
#define _Z_ANT_DESC_PORC_CANTIDAD(x)             _GET_MEMORY_INT( __z_ant_desc_porc_cantidad, x )
#define  Z_ANT_DESC_PORC_ULT_ART_IMPORTE          GET_MEMORY_DOUBLE( __z_ant_desc_porc_ult_art_importe )
#define  Z_ANT_DESC_PORC_ULT_ART_CANTIDAD         GET_MEMORY_INT( __z_ant_desc_porc_ult_art_cantidad )
#define _Z_ANT_DESC_PESOS_IMPORTE(x)             _GET_MEMORY_DOUBLE( __z_ant_desc_pesos_importe, x )
#define _Z_ANT_DESC_PESOS_CANTIDAD(x)            _GET_MEMORY_INT( __z_ant_desc_pesos_cantidad, x )
#define  Z_ANT_DESC_PESOS_ULT_ART_IMPORTE         GET_MEMORY_DOUBLE( __z_ant_desc_pesos_ult_art_importe )
#define  Z_ANT_DESC_PESOS_ULT_ART_CANTIDAD        GET_MEMORY_INT( __z_ant_desc_pesos_ult_art_cantidad )
#define _XC_DESC_PORC_IMPORTE(x)                 _GET_MEMORY_DOUBLE( __xc_desc_porc_importe, x )
#define _XC_DESC_PORC_CANTIDAD(x)                _GET_MEMORY_INT( __xc_desc_porc_cantidad, x )
#define  XC_DESC_PORC_ULT_ART_IMPORTE             GET_MEMORY_DOUBLE( __xc_desc_porc_ult_art_importe )
#define  XC_DESC_PORC_ULT_ART_CANTIDAD            GET_MEMORY_INT( __xc_desc_porc_ult_art_cantidad )
#define _XC_DESC_PESOS_IMPORTE(x)                _GET_MEMORY_DOUBLE( __xc_desc_pesos_importe, x )
#define _XC_DESC_PESOS_CANTIDAD(x)               _GET_MEMORY_INT( __xc_desc_pesos_cantidad, x )
#define  XC_DESC_PESOS_ULT_ART_IMPORTE            GET_MEMORY_DOUBLE( __xc_desc_pesos_ult_art_importe )
#define  XC_DESC_PESOS_ULT_ART_CANTIDAD           GET_MEMORY_INT( __xc_desc_pesos_ult_art_cantidad )
#define  VAR_TARJETAS_FECHA_HOST                  GET_MEMORY_UNSIGNED( __var_tarjetas_fecha_host )
#define  VAR_TARJETAS_HORA_HOST                   GET_MEMORY_LONG( __var_tarjetas_hora_host )
#define  VAR_TARJETAS_NRO_CUENTA                  GET_MEMORY_STR( __var_tarjetas_nro_cuenta )
#define _VAR_TARJETAS_NRO_CUENTA(x)               GET_MEMORY_STR_CHAR( __var_tarjetas_nro_cuenta, x )
#define  VAR_TARJETAS_ANT_FECHA_HOST              GET_MEMORY_UNSIGNED( __var_tarjetas_ant_fecha_host )
#define  VAR_TARJETAS_ANT_HORA_HOST               GET_MEMORY_LONG( __var_tarjetas_ant_hora_host )
#define  VAR_TARJETAS_ANT_NRO_CUENTA              GET_MEMORY_STR( __var_tarjetas_ant_nro_cuenta )
#define _VAR_TARJETAS_ANT_NRO_CUENTA(x)           GET_MEMORY_STR_CHAR( __var_tarjetas_ant_nro_cuenta, x )
#define  VAR_TARJETAS_HORA_ORIGINAL               GET_MEMORY_LONG( __var_tarjetas_hora_original )
#define  VAR_TARJETAS_AUT_ORIGINAL                GET_MEMORY_STR( __var_tarjetas_aut_original )
#define _VAR_TARJETAS_AUT_ORIGINAL(x)             GET_MEMORY_STR_CHAR( __var_tarjetas_aut_original, x )
#define  VAR_TARJETAS_ANT_HORA_ORIGINAL           GET_MEMORY_LONG( __var_tarjetas_ant_hora_original )
#define  VAR_TARJETAS_ANT_AUT_ORIGINAL            GET_MEMORY_STR( __var_tarjetas_ant_aut_original )
#define _VAR_TARJETAS_ANT_AUT_ORIGINAL(x)         GET_MEMORY_STR_CHAR( __var_tarjetas_ant_aut_original, x )
#define  RAM_TIPO_EVENTO_COMP_ORIGINAL            GET_MEMORY_CHAR( __ram_tipo_evento_comp_original )
#define  NRO_EVENTO_ANTERIOR                      GET_MEMORY_LONG( __nro_evento_anterior )
#define  ITEM_PORC_ICA                            GET_MEMORY_FLOAT( __item_porc_ica )
#define  RAM_SUBTOTAL                             GET_MEMORY_DOUBLE( __ram_subtotal )
#define _X_DESC_PORC_IMPORTE_SIN_IVA(x)          _GET_MEMORY_DOUBLE( __x_desc_porc_importe_sin_iva, x )
#define _Z_DESC_PORC_IMPORTE_SIN_IVA(x)          _GET_MEMORY_DOUBLE( __z_desc_porc_importe_sin_iva, x )
#define _Z_ANT_DESC_PORC_IMPORTE_SIN_IVA(x)      _GET_MEMORY_DOUBLE( __z_ant_desc_porc_importe_sin_iva, x )
#define _XC_DESC_PORC_IMPORTE_SIN_IVA(x)         _GET_MEMORY_DOUBLE( __xc_desc_porc_importe_sin_iva, x )
#define  X_DESC_PORC_ULT_ART_IMP_SIN_IVA          GET_MEMORY_DOUBLE( __x_desc_porc_ult_art_imp_sin_iva )
#define  Z_DESC_PORC_UTL_ART_IMP_SIN_IVA          GET_MEMORY_DOUBLE( __z_desc_porc_utl_art_imp_sin_iva )
#define  Z_ANT_DESC_PORC_ULT_ART_IMP_SIN_IVA      GET_MEMORY_DOUBLE( __z_ant_desc_porc_ult_art_imp_sin_iva )
#define  XC_DESC_PORC_UTL_ART_IMP_SIN_IVA         GET_MEMORY_DOUBLE( __xc_desc_porc_utl_art_imp_sin_iva )
#define _X_DESC_PESOS_IMPORTE_SIN_IVA(x)         _GET_MEMORY_DOUBLE( __x_desc_pesos_importe_sin_iva, x )
#define  X_DESC_PESOS_ULT_ART_IMP_SIN_IVA         GET_MEMORY_DOUBLE( __x_desc_pesos_ult_art_imp_sin_iva )
#define _Z_DESC_PESOS_IMPORTE_SIN_IVA(x)         _GET_MEMORY_DOUBLE( __z_desc_pesos_importe_sin_iva, x )
#define  Z_DESC_PESOS_ULT_ART_IMP_SIN_IVA         GET_MEMORY_DOUBLE( __z_desc_pesos_ult_art_imp_sin_iva )
#define _Z_ANT_DESC_PESOS_IMPORTE_SIN_IVA(x)     _GET_MEMORY_DOUBLE( __z_ant_desc_pesos_importe_sin_iva, x )
#define  Z_ANT_DESC_PESOS_ULT_ART_IMP_SIN_IV      GET_MEMORY_DOUBLE( __z_ant_desc_pesos_ult_art_imp_sin_iv )
#define _XC_DESC_PESOS_IMPORTE_SIN_IVA(x)        _GET_MEMORY_DOUBLE( __xc_desc_pesos_importe_sin_iva, x )
#define  XC_DESC_PESOS_ULT_ART_IMP_SIN_IVA        GET_MEMORY_DOUBLE( __xc_desc_pesos_ult_art_imp_sin_iva )
#define  RAM_COD_BARRA_ORIGINAL                   GET_MEMORY_STR( __ram_cod_barra_original )
#define _RAM_COD_BARRA_ORIGINAL(x)                GET_MEMORY_STR_CHAR( __ram_cod_barra_original, x )
#define  VAR_TARJETAS_TASA                        GET_MEMORY_FLOAT( __var_tarjetas_tasa )
#define  VAR_TARJETAS_VUELTO                      GET_MEMORY_DOUBLE( __var_tarjetas_vuelto )
#define  VAR_TARJETAS_ANT_TASA                    GET_MEMORY_FLOAT( __var_tarjetas_ant_tasa )
#define  VAR_TARJETAS_ANT_VUELTO                  GET_MEMORY_DOUBLE( __var_tarjetas_ant_vuelto )
#define  VAR_TARJETAS_GRUPO_CUOTA                 GET_MEMORY_INT( __var_tarjetas_grupo_cuota )
#define  VAR_TARJETAS_ANT_GRUPO_CUOTA             GET_MEMORY_INT( __var_tarjetas_ant_grupo_cuota )
#define  RAM_NRO_SUC                              GET_MEMORY_INT( __ram_nro_suc )
#define  RAM_NRO_COMP                             GET_MEMORY_LONG( __ram_nro_comp )
#define  RAM_CANT_COMP                            GET_MEMORY_INT( __ram_cant_comp )
#define  VAR_TARJETAS_TASA_APLICADA               GET_MEMORY_FLOAT( __var_tarjetas_tasa_aplicada )
#define  VAR_TARJETAS_ANT_TASA_APLICADA           GET_MEMORY_FLOAT( __var_tarjetas_ant_tasa_aplicada )
#define  RAM_VARIABLE_VALIDACION                  GET_MEMORY_INT( __ram_variable_validacion )
#define  RAM_ID_ULTIMO_EVENTO_FACTURA             GET_MEMORY_LONG( __ram_id_ultimo_evento_factura )
#define  RAM_NRO_ULTIMO_TICKET                    GET_MEMORY_LONG( __ram_nro_ultimo_ticket )
#define  X_CANT_TICKET_DEV_ENVASE                 GET_MEMORY_INT( __x_cant_ticket_dev_envase )
#define  Z_CANT_TICKET_DEV_ENVASE                 GET_MEMORY_INT( __z_cant_ticket_dev_envase )
#define  Z_ANT_CANT_TICKET_DEV_ENVASE             GET_MEMORY_INT( __z_ant_cant_ticket_dev_envase )
#define  XC_CANT_TICKET_DEV_ENVASE                GET_MEMORY_INT( __xc_cant_ticket_dev_envase )
#define _PAGO_IMP_CUOTA(x)                       _GET_MEMORY_DOUBLE( __pago_imp_cuota, x )
#define  ITEM_DESCUENTO                           GET_MEMORY_DOUBLE( __item_descuento )
#define _ITEM_MONTO_IMPUESTO(x)                  _GET_MEMORY_FLOAT( __item_monto_impuesto, x )
#define _ITEM_PORC_IMPUESTO(x)                   _GET_MEMORY_FLOAT( __item_porc_impuesto, x )
#define _X_IMPUESTOS(x)                          _GET_MEMORY_DOUBLE( __x_impuestos, x )
#define _Z_IMPUESTOS(x)                          _GET_MEMORY_DOUBLE( __z_impuestos, x )
#define _Z_ANT_IMPUESTOS(x)                      _GET_MEMORY_DOUBLE( __z_ant_impuestos, x )
#define _XC_IMPUESTOS(x)                         _GET_MEMORY_DOUBLE( __xc_impuestos, x )
#define _TOT_TICK_IMPUESTOS(x)                   _GET_MEMORY_DOUBLE( __tot_tick_impuestos, x )
#define _TOT_TICK_ANT_IMPUESTOS(x)               _GET_MEMORY_DOUBLE( __tot_tick_ant_impuestos, x )
#define _TOT_TICK_SV_IMPUESTOS(x)                _GET_MEMORY_DOUBLE( __tot_tick_sv_impuestos, x )
#define _ITEM_SUMA_IMPUESTO_AL_TOTAL(x)          _GET_MEMORY_CHAR( __item_suma_impuesto_al_total, x )
#define _PAGO_COD_RESTRICCION_ART(x)             _GET_MEMORY_INT( __pago_cod_restriccion_art, x )
#define  SITEF_SESION_INICIADA                    GET_MEMORY_CHAR( __sitef_sesion_iniciada )
#define  SITEF_NID                                GET_MEMORY_CHAR( __sitef_nid )
#define  SITEF_OP_HABILITADAS                     GET_MEMORY_LONG( __sitef_op_habilitadas )
#define  SITEF_FECHA_DEBITO_PREDATADA             GET_MEMORY_UNSIGNED( __sitef_fecha_debito_predatada )
#define  SITEF_LIMITE_INF_CONSULTA_CHEQUES        GET_MEMORY_DOUBLE( __sitef_limite_inf_consulta_cheques )
#define  SITEF_LIMITE_DEBITO_FORZADA              GET_MEMORY_DOUBLE( __sitef_limite_debito_forzada )
#define  SITEF_SOLICITAR_CUOTAS_CREDITO           GET_MEMORY_CHAR( __sitef_solicitar_cuotas_credito )
#define  SITEF_TIMEOUT                            GET_MEMORY_INT( __sitef_timeout )
#define  SITEF_CUOTAS_DEBITO_PARCELADA            GET_MEMORY_CHAR( __sitef_cuotas_debito_parcelada )
#define  SITEF_INTERVALO_DEBITO_PARCELA           GET_MEMORY_INT( __sitef_intervalo_debito_parcela )
#define  SITEF_1_CUOTA_DEBITO_VISTA               GET_MEMORY_CHAR( __sitef_1_cuota_debito_vista )
#define  SITEF_DEBITO_MES_CERRADO                 GET_MEMORY_CHAR( __sitef_debito_mes_cerrado )
#define  SITEF_EMITE_NOTA_PROMISORIA              GET_MEMORY_CHAR( __sitef_emite_nota_promisoria )
#define  VAR_TARJETAS_RED                         GET_MEMORY_INT( __var_tarjetas_red )
#define  VAR_TARJETAS_CONFIRMAR_NUMERO            GET_MEMORY_CHAR( __var_tarjetas_confirmar_numero )
#define  VAR_TARJETAS_ULTIMOS_DIGITOS             GET_MEMORY_INT( __var_tarjetas_ultimos_digitos )
#define  VAR_TARJETAS_MIN_CUOTAS                  GET_MEMORY_CHAR( __var_tarjetas_min_cuotas )
#define  VAR_TARJETAS_MAX_CUOTAS                  GET_MEMORY_CHAR( __var_tarjetas_max_cuotas )
#define  VAR_TARJETAS_FECHA_TOPE_PREDATADA        GET_MEMORY_UNSIGNED( __var_tarjetas_fecha_tope_predatada )
#define  VAR_TARJETAS_FECHA_TOPE_1_CUOTA          GET_MEMORY_UNSIGNED( __var_tarjetas_fecha_tope_1_cuota )
#define  VAR_TARJETAS_PEDIR_CVV                   GET_MEMORY_CHAR( __var_tarjetas_pedir_cvv )
#define  VAR_TARJETAS_PEDIR_GARANTIA              GET_MEMORY_CHAR( __var_tarjetas_pedir_garantia )
#define  VAR_TARJETAS_OP_HABILITADAS              GET_MEMORY_LONG( __var_tarjetas_op_habilitadas )
#define  VAR_TARJETAS_ANT_RED                     GET_MEMORY_INT( __var_tarjetas_ant_red )
#define  VAR_TARJETAS_ANT_CONFIRMAR_NUMERO        GET_MEMORY_CHAR( __var_tarjetas_ant_confirmar_numero )
#define  VAR_TARJETAS_ANT_ULTIMOS_DIGITOS         GET_MEMORY_INT( __var_tarjetas_ant_ultimos_digitos )
#define  VAR_TARJETAS_ANT_MIN_CUOTAS              GET_MEMORY_CHAR( __var_tarjetas_ant_min_cuotas )
#define  VAR_TARJETAS_ANT_MAX_CUOTAS              GET_MEMORY_CHAR( __var_tarjetas_ant_max_cuotas )
#define  VAR_TARJETAS_ANT_FECHA_TOPE_PREDATA      GET_MEMORY_UNSIGNED( __var_tarjetas_ant_fecha_tope_predata )
#define  VAR_TARJETAS_ANT_FECHA_TOPE_1_CUOTA      GET_MEMORY_UNSIGNED( __var_tarjetas_ant_fecha_tope_1_cuota )
#define  VAR_TARJETAS_ANT_PEDIR_CVV               GET_MEMORY_CHAR( __var_tarjetas_ant_pedir_cvv )
#define  VAR_TARJETAS_ANT_PEDIR_GARANTIA          GET_MEMORY_CHAR( __var_tarjetas_ant_pedir_garantia )
#define  VAR_TARJETAS_ANT_OP_HABILITADAS          GET_MEMORY_LONG( __var_tarjetas_ant_op_habilitadas )
#define  NRO_PAGARE                               GET_MEMORY_LONG( __nro_pagare )
#define _RAM_DESC_PORC_IMPORTE(x)                _GET_MEMORY_DOUBLE( __ram_desc_porc_importe, x )
#define  RAM_COND_IVA_CLIENTE                     GET_MEMORY_CHAR( __ram_cond_iva_cliente )
#define  RAM_FECHA_ULT_COMPRA_AFIN                GET_MEMORY_INT( __ram_fecha_ult_compra_afin )
#define  RAM_SUC_ULT_COMPRA_AFIN                  GET_MEMORY_INT( __ram_suc_ult_compra_afin )
#define  RAM_DESC_CATEGORIA_AFIN                  GET_MEMORY_STR( __ram_desc_categoria_afin )
#define _RAM_DESC_CATEGORIA_AFIN(x)               GET_MEMORY_STR_CHAR( __ram_desc_categoria_afin, x )
#define _RAM_PROMOS_LIMITAN_MEDIOS(x)            _GET_MEMORY_INT( __ram_promos_limitan_medios, x )
#define  RAM_NRO_PEDIDO_PIN                       GET_MEMORY_LONG( __ram_nro_pedido_pin )
#define  VAR_TARJETAS_CAJA_ORIGINAL               GET_MEMORY_INT( __var_tarjetas_caja_original )
#define  VAR_TARJETAS_SUC_ORIGINAL                GET_MEMORY_INT( __var_tarjetas_suc_original )
#define  VAR_TARJETAS_ANT_CAJA_ORIGINAL           GET_MEMORY_INT( __var_tarjetas_ant_caja_original )
#define  VAR_TARJETAS_ANT_SUC_ORIGINAL            GET_MEMORY_INT( __var_tarjetas_ant_suc_original )
#define  VAR_TARJETAS_PIN_ASCII                   GET_MEMORY_STR( __var_tarjetas_pin_ascii )
#define _VAR_TARJETAS_PIN_ASCII(x)                GET_MEMORY_STR_CHAR( __var_tarjetas_pin_ascii, x )
#define  VAR_TARJETAS_ANT_PIN_ASCII               GET_MEMORY_STR( __var_tarjetas_ant_pin_ascii )
#define _VAR_TARJETAS_ANT_PIN_ASCII(x)            GET_MEMORY_STR_CHAR( __var_tarjetas_ant_pin_ascii, x )
#define  VAR_TARJETAS_NOM_ARCH_VOUCHER            GET_MEMORY_STR( __var_tarjetas_nom_arch_voucher )
#define _VAR_TARJETAS_NOM_ARCH_VOUCHER(x)         GET_MEMORY_STR_CHAR( __var_tarjetas_nom_arch_voucher, x )
#define  VAR_TARJETAS_ANT_NOM_ARCH_VOUCHER        GET_MEMORY_STR( __var_tarjetas_ant_nom_arch_voucher )
#define _VAR_TARJETAS_ANT_NOM_ARCH_VOUCHER(x)     GET_MEMORY_STR_CHAR( __var_tarjetas_ant_nom_arch_voucher, x )
#define  RAM_NO_A_TODO                            GET_MEMORY_CHAR( __ram_no_a_todo )
#define  NRO_CORRELATIVO_INFORMES                 GET_MEMORY_LONG( __nro_correlativo_informes )
#define  NRO_VENDEDOR_STRING                      GET_MEMORY_STR( __nro_vendedor_string )
#define _NRO_VENDEDOR_STRING(x)                   GET_MEMORY_STR_CHAR( __nro_vendedor_string, x )
#define  ITEM_VTA_ASISTIDA                        GET_MEMORY_INT( __item_vta_asistida )
#define  RAM_TOTAL_VTA_ASISTIDA                   GET_MEMORY_DOUBLE( __ram_total_vta_asistida )
#define  RAM_TOTAL_DONACION_VENTA                 GET_MEMORY_DOUBLE( __ram_total_donacion_venta )
#define  X_DONACIONES_VENTAS_IMPORTE              GET_MEMORY_DOUBLE( __x_donaciones_ventas_importe )
#define  X_DONACIONES_VENTAS_CANTIDAD             GET_MEMORY_INT( __x_donaciones_ventas_cantidad )
#define  Z_DONACIONES_VENTAS_IMPORTE              GET_MEMORY_DOUBLE( __z_donaciones_ventas_importe )
#define  Z_DONACIONES_VENTAS_CANTIDAD             GET_MEMORY_INT( __z_donaciones_ventas_cantidad )
#define  Z_ANT_DONACIONES_VENTAS_IMPORTE          GET_MEMORY_DOUBLE( __z_ant_donaciones_ventas_importe )
#define  Z_ANT_DONACIONES_VENTAS_CANTIDAD         GET_MEMORY_INT( __z_ant_donaciones_ventas_cantidad )
#define  ITEM_PARTICIPA_DONACION                  GET_MEMORY_CHAR( __item_participa_donacion )
#define  RAM_CANT_DESCUENTO                       GET_MEMORY_INT( __ram_cant_descuento )
#define  RAM_COD_PROMO                            GET_MEMORY_INT( __ram_cod_promo )
#define  RAM_INICIAR_PERIODO_VENTA                GET_MEMORY_CHAR( __ram_iniciar_periodo_venta )
#define  RAM_CONSULTA_AFIN_REMOTA                 GET_MEMORY_CHAR( __ram_consulta_afin_remota )
#define  NRO_ESTADO_JOURNAL                       GET_MEMORY_LONG( __nro_estado_journal )
#define  NRO_ESTACION_ACTIVA                      GET_MEMORY_CHAR( __nro_estacion_activa )
#define  VAR_TARJETAS_DATAFONO                    GET_MEMORY_CHAR( __var_tarjetas_datafono )
#define  VAR_TARJETAS_ANT_DATAFONO                GET_MEMORY_CHAR( __var_tarjetas_ant_datafono )
#define  ITEM_ORIGEN                              GET_MEMORY_CHAR( __item_origen )
#define _PAGO_DATAFONO(x)                        _GET_MEMORY_LONG( __pago_datafono, x )
#define  RAM_VOUCHER_TARJ_COMERCIO                GET_MEMORY_CHAR( __ram_voucher_tarj_comercio )
#define  VAR_TARJETAS_NRO_TRACE                   GET_MEMORY_LONG( __var_tarjetas_nro_trace )
#define  VAR_TARJETAS_ANT_NRO_TRACE               GET_MEMORY_LONG( __var_tarjetas_ant_nro_trace )
#define _PAGO_CUIT(x)                            _GET_MEMORY_STR( __pago_cuit, x )
#define  RAM_COD_PREMIO                           GET_MEMORY_LONG( __ram_cod_premio )
#define  RAM_PREMIO_EJECUTADO                     GET_MEMORY_LONG( __ram_premio_ejecutado )
#define  X_BOLETAS_CON_DERECHO_A_NC               GET_MEMORY_DOUBLE( __x_boletas_con_derecho_a_nc )
#define  X_CANT_BOLETAS_CON_DERECHO_A_NC          GET_MEMORY_INT( __x_cant_boletas_con_derecho_a_nc )
#define  Z_BOLETAS_CON_DERECHO_A_NC               GET_MEMORY_DOUBLE( __z_boletas_con_derecho_a_nc )
#define  Z_CANT_BOLETAS_CON_DERECHO_A_NC          GET_MEMORY_INT( __z_cant_boletas_con_derecho_a_nc )
#define  Z_ANT_BOLETAS_CON_DERECHO_A_NC           GET_MEMORY_DOUBLE( __z_ant_boletas_con_derecho_a_nc )
#define  Z_ANT_CANT_BOLETAS_CON_DERECHO_A_NC      GET_MEMORY_INT( __z_ant_cant_boletas_con_derecho_a_nc )
#define  XC_DONACIONES_VENTAS_IMPORTE             GET_MEMORY_DOUBLE( __xc_donaciones_ventas_importe )
#define  XC_DONACIONES_VENTAS_CANTIDAD            GET_MEMORY_INT( __xc_donaciones_ventas_cantidad )
#define  XC_BOLETAS_CON_DERECHO_A_NC              GET_MEMORY_DOUBLE( __xc_boletas_con_derecho_a_nc )
#define  XC_CANT_BOLETAS_CON_DERECHO_A_NC         GET_MEMORY_INT( __xc_cant_boletas_con_derecho_a_nc )
#define  RAM_AHORRO_OBTENIDO                      GET_MEMORY_DOUBLE( __ram_ahorro_obtenido )
#define  ITEM_PRECIO_ORIGINAL                     GET_MEMORY_DOUBLE( __item_precio_original )
#define  RAM_PUNTOS_ESP_1                         GET_MEMORY_DOUBLE( __ram_puntos_esp_1 )
#define  RAM_PUNTOS_ESP_2                         GET_MEMORY_DOUBLE( __ram_puntos_esp_2 )
#define  RAM_PUNTOS_ESP_3                         GET_MEMORY_DOUBLE( __ram_puntos_esp_3 )
#define  RAM_PUNTOS_ESP_4                         GET_MEMORY_DOUBLE( __ram_puntos_esp_4 )
#define  RAM_PUNTOS_ESP_5                         GET_MEMORY_DOUBLE( __ram_puntos_esp_5 )
#define _X_DEVOLUCIONES_MEDIO(x)                 _GET_MEMORY_DOUBLE( __x_devoluciones_medio, x )
#define  X_FACTURA_A_IMPORTE_EXENTO               GET_MEMORY_DOUBLE( __x_factura_a_importe_exento )
#define  X_FACTURA_A_IMPORTE_GRAVADO              GET_MEMORY_DOUBLE( __x_factura_a_importe_gravado )
#define  X_NC_A_IMPORTE_EXENTO                    GET_MEMORY_DOUBLE( __x_nc_a_importe_exento )
#define  X_NC_A_IMPORTE_GRAVADO                   GET_MEMORY_DOUBLE( __x_nc_a_importe_gravado )
#define  X_CONS_FINAL_IMPORTE_EXENT_FACT          GET_MEMORY_DOUBLE( __x_cons_final_importe_exent_fact )
#define  X_CONS_FINAL_IMPORTE_GRAV_FACT           GET_MEMORY_DOUBLE( __x_cons_final_importe_grav_fact )
#define  X_CONS_FINAL_CANTIDAD_FACT               GET_MEMORY_INT( __x_cons_final_cantidad_fact )
#define  X_DEV_CONS_F_IMPORTE_EXENT_FACT          GET_MEMORY_DOUBLE( __x_dev_cons_f_importe_exent_fact )
#define  X_DEV_CONS_F_IMPORTE_GRAV_FACT           GET_MEMORY_DOUBLE( __x_dev_cons_f_importe_grav_fact )
#define  X_DEV_CONS_F_CANTIDAD_FACT               GET_MEMORY_INT( __x_dev_cons_f_cantidad_fact )
#define  X_VENTAS_DIPLOM_IMPORTE                  GET_MEMORY_DOUBLE( __x_ventas_diplom_importe )
#define  X_VENTAS_DIPLOM_CANTIDAD                 GET_MEMORY_INT( __x_ventas_diplom_cantidad )
#define  X_DEVOLUCIONES_DIPLOM_IMPORTE            GET_MEMORY_DOUBLE( __x_devoluciones_diplom_importe )
#define  X_DEVOLUCIONES_DIPLOM_CANTIDAD           GET_MEMORY_INT( __x_devoluciones_diplom_cantidad )
#define  X_VENTAS_DIPL_IMPORTE                    GET_MEMORY_DOUBLE( __x_ventas_dipl_importe )
#define  X_VENTAS_DIPL_CANTIDAD                   GET_MEMORY_INT( __x_ventas_dipl_cantidad )
#define  X_DEVOLUCIONES_EXPORT_IMPORTE            GET_MEMORY_DOUBLE( __x_devoluciones_export_importe )
#define  X_DEVOLUCIONES_EXPORT_CANTIDAD           GET_MEMORY_INT( __x_devoluciones_export_cantidad )
#define  X_CONS_FINAL_IMPORTE_EXENT_TKT           GET_MEMORY_DOUBLE( __x_cons_final_importe_exent_tkt )
#define  X_CONS_FINAL_IMPORTE_GRAV_TKT            GET_MEMORY_DOUBLE( __x_cons_final_importe_grav_tkt )
#define  X_CONS_FINAL_CANTIDAD_TKT                GET_MEMORY_INT( __x_cons_final_cantidad_tkt )
#define  X_DEV_CONS_FINAL_IMPORTE_EXENT_TKT       GET_MEMORY_DOUBLE( __x_dev_cons_final_importe_exent_tkt )
#define  X_DEV_CONS_F_IMPORTE_GRAV_TKT            GET_MEMORY_DOUBLE( __x_dev_cons_f_importe_grav_tkt )
#define  X_DEV_CONS_F_CANTIDAD_TKT                GET_MEMORY_INT( __x_dev_cons_f_cantidad_tkt )
#define  X_IVA_FACTURA_A                          GET_MEMORY_DOUBLE( __x_iva_factura_a )
#define  X_IVA_CONS_FINAL                         GET_MEMORY_DOUBLE( __x_iva_cons_final )
#define  X_NRO_TRANSACCION_INICIAL                GET_MEMORY_LONG( __x_nro_transaccion_inicial )
#define  X_NRO_TRANSACCION_FINAL                  GET_MEMORY_LONG( __x_nro_transaccion_final )
#define _Z_DEVOLUCIONES_MEDIO(x)                 _GET_MEMORY_DOUBLE( __z_devoluciones_medio, x )
#define  Z_FACTURA_A_IMPORTE_EXENTO               GET_MEMORY_DOUBLE( __z_factura_a_importe_exento )
#define  Z_FACTURA_A_IMPORTE_GRAVADO              GET_MEMORY_DOUBLE( __z_factura_a_importe_gravado )
#define  Z_NC_A_IMPORTE_EXENTO                    GET_MEMORY_DOUBLE( __z_nc_a_importe_exento )
#define  Z_NC_A_IMPORTE_GRAVADO                   GET_MEMORY_DOUBLE( __z_nc_a_importe_gravado )
#define  Z_CONS_FINAL_IMPORTE_EXENT_FACT          GET_MEMORY_DOUBLE( __z_cons_final_importe_exent_fact )
#define  Z_CONS_FINAL_IMPORTE_GRAV_FACT           GET_MEMORY_DOUBLE( __z_cons_final_importe_grav_fact )
#define  Z_CONS_FINAL_CANTIDAD_FACT               GET_MEMORY_INT( __z_cons_final_cantidad_fact )
#define  Z_DEV_CONS_F_IMPORTE_EXENT_FACT          GET_MEMORY_DOUBLE( __z_dev_cons_f_importe_exent_fact )
#define  Z_DEV_CONS_F_IMPORTE_GRAV_FACT           GET_MEMORY_DOUBLE( __z_dev_cons_f_importe_grav_fact )
#define  Z_DEV_CONS_F_CANTIDAD_FACT               GET_MEMORY_INT( __z_dev_cons_f_cantidad_fact )
#define  Z_VENTAS_DIPLOM_IMPORTE                  GET_MEMORY_DOUBLE( __z_ventas_diplom_importe )
#define  Z_VENTAS_DIPLOM_CANTIDAD                 GET_MEMORY_INT( __z_ventas_diplom_cantidad )
#define  Z_DEVOLUCIONES_DIPLOM_IMPORTE            GET_MEMORY_DOUBLE( __z_devoluciones_diplom_importe )
#define  Z_DEVOLUCIONES_DIPLOM_CANTIDAD           GET_MEMORY_INT( __z_devoluciones_diplom_cantidad )
#define  Z_VENTAS_DIPL_IMPORTE                    GET_MEMORY_DOUBLE( __z_ventas_dipl_importe )
#define  Z_VENTAS_DIPL_CANTIDAD                   GET_MEMORY_INT( __z_ventas_dipl_cantidad )
#define  Z_DEVOLUCIONES_EXPORT_IMPORTE            GET_MEMORY_DOUBLE( __z_devoluciones_export_importe )
#define  Z_DEVOLUCIONES_EXPORT_CANTIDAD           GET_MEMORY_INT( __z_devoluciones_export_cantidad )
#define  Z_CONS_FINAL_IMPORTE_EXENT_TKT           GET_MEMORY_DOUBLE( __z_cons_final_importe_exent_tkt )
#define  Z_CONS_FINAL_IMPORTE_GRAV_TKT            GET_MEMORY_DOUBLE( __z_cons_final_importe_grav_tkt )
#define  Z_CONS_FINAL_CANTIDAD_TKT                GET_MEMORY_INT( __z_cons_final_cantidad_tkt )
#define  Z_DEV_CONS_F_IMPORTE_EXENT_TKT           GET_MEMORY_DOUBLE( __z_dev_cons_f_importe_exent_tkt )
#define  Z_DEV_CONS_F_IMPORTE_GRAV_TKT            GET_MEMORY_DOUBLE( __z_dev_cons_f_importe_grav_tkt )
#define  Z_DEV_CONS_F_CANTIDAD_TKT                GET_MEMORY_INT( __z_dev_cons_f_cantidad_tkt )
#define  Z_IVA_FACTURA_A                          GET_MEMORY_DOUBLE( __z_iva_factura_a )
#define  Z_IVA_CONS_FINAL                         GET_MEMORY_DOUBLE( __z_iva_cons_final )
#define  Z_NRO_TRANSACCION_INICIAL                GET_MEMORY_LONG( __z_nro_transaccion_inicial )
#define  Z_NRO_TRANSACCION_FINAL                  GET_MEMORY_LONG( __z_nro_transaccion_final )
#define _Z_ANT_DEVOLUCIONES_MEDIO(x)             _GET_MEMORY_DOUBLE( __z_ant_devoluciones_medio, x )
#define  Z_ANT_FACTURA_A_IMPORTE_EXENTO           GET_MEMORY_DOUBLE( __z_ant_factura_a_importe_exento )
#define  Z_ANT_FACTURA_A_IMPORTE_GRAVADO          GET_MEMORY_DOUBLE( __z_ant_factura_a_importe_gravado )
#define  Z_ANT_NC_A_IMPORTE_EXENTO                GET_MEMORY_DOUBLE( __z_ant_nc_a_importe_exento )
#define  Z_ANT_NC_A_IMPORTE_GRAVADO               GET_MEMORY_DOUBLE( __z_ant_nc_a_importe_gravado )
#define  Z_ANT_CONS_FINAL_IMP_EXENT_FACT          GET_MEMORY_DOUBLE( __z_ant_cons_final_imp_exent_fact )
#define  Z_ANT_CONS_FINAL_IMP_GRAV_FACT           GET_MEMORY_DOUBLE( __z_ant_cons_final_imp_grav_fact )
#define  Z_ANT_CONS_FINAL_CANTIDAD_FACT           GET_MEMORY_INT( __z_ant_cons_final_cantidad_fact )
#define  Z_ANT_DEV_CONS_F_IMP_EXENT_FACT          GET_MEMORY_DOUBLE( __z_ant_dev_cons_f_imp_exent_fact )
#define  Z_ANT_DEV_CONS_F_IMP_GRAV_FACT           GET_MEMORY_DOUBLE( __z_ant_dev_cons_f_imp_grav_fact )
#define  Z_ANT_DEV_CONS_F_CANTIDAD_FACT           GET_MEMORY_INT( __z_ant_dev_cons_f_cantidad_fact )
#define  Z_ANT_VENTAS_DIPLOM_IMPORTE              GET_MEMORY_DOUBLE( __z_ant_ventas_diplom_importe )
#define  Z_ANT_VENTAS_DIPLOM_CANTIDAD             GET_MEMORY_INT( __z_ant_ventas_diplom_cantidad )
#define  Z_ANT_DEVOLUCIONES_DIPLOM_IMPORTE        GET_MEMORY_DOUBLE( __z_ant_devoluciones_diplom_importe )
#define  Z_ANT_DEVOLUCIONES_DIPLOM_CANTIDAD       GET_MEMORY_INT( __z_ant_devoluciones_diplom_cantidad )
#define  Z_ANT_VENTAS_DIPL_IMPORTE                GET_MEMORY_DOUBLE( __z_ant_ventas_dipl_importe )
#define  Z_ANT_VENTAS_DIPL_CANTIDAD               GET_MEMORY_INT( __z_ant_ventas_dipl_cantidad )
#define  Z_ANT_DEVOLUCIONES_EXPORT_IMPORTE        GET_MEMORY_DOUBLE( __z_ant_devoluciones_export_importe )
#define  Z_ANT_DEVOLUCIONES_EXPORT_CANTIDAD       GET_MEMORY_INT( __z_ant_devoluciones_export_cantidad )
#define  Z_ANT_CONS_FIN_IMPORTE_EXENT_TKT         GET_MEMORY_DOUBLE( __z_ant_cons_fin_importe_exent_tkt )
#define  Z_ANT_CONS_FIN_IMPORTE_GRAV_TKT          GET_MEMORY_DOUBLE( __z_ant_cons_fin_importe_grav_tkt )
#define  Z_ANT_CONS_FINAL_CANTIDAD_TKT            GET_MEMORY_INT( __z_ant_cons_final_cantidad_tkt )
#define  Z_ANT_DEV_CON_F_IMP_EXENT_TKT            GET_MEMORY_DOUBLE( __z_ant_dev_con_f_imp_exent_tkt )
#define  Z_ANT_DEV_CON_F_IMP_GRAV_TKT             GET_MEMORY_DOUBLE( __z_ant_dev_con_f_imp_grav_tkt )
#define  Z_ANT_DEV_CONS_F_CANTIDAD_TKT            GET_MEMORY_INT( __z_ant_dev_cons_f_cantidad_tkt )
#define  Z_ANT_IVA_FACTURA_A                      GET_MEMORY_DOUBLE( __z_ant_iva_factura_a )
#define  Z_ANT_IVA_CONS_FINAL                     GET_MEMORY_DOUBLE( __z_ant_iva_cons_final )
#define  Z_ANT_NRO_TRANSACCION_INICIAL            GET_MEMORY_LONG( __z_ant_nro_transaccion_inicial )
#define  Z_ANT_NRO_TRANSACCION_FINAL              GET_MEMORY_LONG( __z_ant_nro_transaccion_final )
#define _XC_DEVOLUCIONES_MEDIO(x)                _GET_MEMORY_DOUBLE( __xc_devoluciones_medio, x )
#define  XC_FACTURA_A_IMPORTE_EXENTO              GET_MEMORY_DOUBLE( __xc_factura_a_importe_exento )
#define  XC_FACTURA_A_IMPORTE_GRAVADO             GET_MEMORY_DOUBLE( __xc_factura_a_importe_gravado )
#define  XC_NC_A_IMPORTE_EXENTO                   GET_MEMORY_DOUBLE( __xc_nc_a_importe_exento )
#define  XC_NC_A_IMPORTE_GRAVADO                  GET_MEMORY_DOUBLE( __xc_nc_a_importe_gravado )
#define  XC_CONS_FINAL_IMPORTE_EXENT_FACT         GET_MEMORY_DOUBLE( __xc_cons_final_importe_exent_fact )
#define  XC_CONS_FINAL_IMPORTE_GRAV_FACT          GET_MEMORY_DOUBLE( __xc_cons_final_importe_grav_fact )
#define  XC_CONS_FINAL_CANTIDAD_FACT              GET_MEMORY_INT( __xc_cons_final_cantidad_fact )
#define  XC_DEV_CONS_F_IMPORTE_EXENT_FACT         GET_MEMORY_DOUBLE( __xc_dev_cons_f_importe_exent_fact )
#define  XC_DEV_CONS_F_IMPORTE_GRAV_FACT          GET_MEMORY_DOUBLE( __xc_dev_cons_f_importe_grav_fact )
#define  XC_DEV_CONS_F_CANTIDAD_FACT              GET_MEMORY_INT( __xc_dev_cons_f_cantidad_fact )
#define  XC_VENTAS_DIPLOM_IMPORTE                 GET_MEMORY_DOUBLE( __xc_ventas_diplom_importe )
#define  XC_VENTAS_DIPLOM_CANTIDAD                GET_MEMORY_INT( __xc_ventas_diplom_cantidad )
#define  XC_DEVOLUCIONES_DIPLOM_IMPORTE           GET_MEMORY_DOUBLE( __xc_devoluciones_diplom_importe )
#define  XC_DEVOLUCIONES_DIPLOM_CANTIDAD          GET_MEMORY_INT( __xc_devoluciones_diplom_cantidad )
#define  XC_VENTAS_DIPL_IMPORTE                   GET_MEMORY_DOUBLE( __xc_ventas_dipl_importe )
#define  XC_VENTAS_DIPL_CANTIDAD                  GET_MEMORY_INT( __xc_ventas_dipl_cantidad )
#define  XC_DEVOLUCIONES_EXPORT_IMPORTE           GET_MEMORY_DOUBLE( __xc_devoluciones_export_importe )
#define  XC_DEVOLUCIONES_EXPORT_CANTIDAD          GET_MEMORY_INT( __xc_devoluciones_export_cantidad )
#define  XC_CONS_FINAL_IMPORTE_EXENT_TKT          GET_MEMORY_DOUBLE( __xc_cons_final_importe_exent_tkt )
#define  XC_CONS_FINAL_IMPORTE_GRAV_TKT           GET_MEMORY_DOUBLE( __xc_cons_final_importe_grav_tkt )
#define  XC_CONS_FINAL_CANTIDAD_TKT               GET_MEMORY_INT( __xc_cons_final_cantidad_tkt )
#define  XC_DEV_CONS_F_IMPORTE_EXENT_TKT          GET_MEMORY_DOUBLE( __xc_dev_cons_f_importe_exent_tkt )
#define  XC_DEV_CONS_F_IMPORTE_GRAV_TKT           GET_MEMORY_DOUBLE( __xc_dev_cons_f_importe_grav_tkt )
#define  XC_DEV_CONS_F_CANTIDAD_TKT               GET_MEMORY_INT( __xc_dev_cons_f_cantidad_tkt )
#define  XC_IVA_FACTURA_A                         GET_MEMORY_DOUBLE( __xc_iva_factura_a )
#define  XC_IVA_CONS_FINAL                        GET_MEMORY_DOUBLE( __xc_iva_cons_final )
#define  XC_NRO_TRANSACCION_INICIAL               GET_MEMORY_LONG( __xc_nro_transaccion_inicial )
#define  XC_NRO_TRANSACCION_FINAL                 GET_MEMORY_LONG( __xc_nro_transaccion_final )
#define  NRO_ULTIMO_TICKET_RANGO                  GET_MEMORY_LONG( __nro_ultimo_ticket_rango )
#define  NRO_ORDEN_DOSIFICACION                   GET_MEMORY_STR( __nro_orden_dosificacion )
#define _NRO_ORDEN_DOSIFICACION(x)                GET_MEMORY_STR_CHAR( __nro_orden_dosificacion, x )
#define  X_VENTAS_EXPORT_IMPORTE                  GET_MEMORY_DOUBLE( __x_ventas_export_importe )
#define  X_VENTAS_EXPORT_CANTIDAD                 GET_MEMORY_INT( __x_ventas_export_cantidad )
#define  Z_VENTAS_EXPORT_IMPORTE                  GET_MEMORY_DOUBLE( __z_ventas_export_importe )
#define  Z_VENTAS_EXPORT_CANTIDAD                 GET_MEMORY_INT( __z_ventas_export_cantidad )
#define  Z_ANT_VENTAS_EXPORT_IMPORTE              GET_MEMORY_DOUBLE( __z_ant_ventas_export_importe )
#define  Z_ANT_VENTAS_EXPORT_CANTIDAD             GET_MEMORY_INT( __z_ant_ventas_export_cantidad )
#define  XC_VENTAS_EXPORT_IMPORTE                 GET_MEMORY_DOUBLE( __xc_ventas_export_importe )
#define  XC_VENTAS_EXPORT_CANTIDAD                GET_MEMORY_INT( __xc_ventas_export_cantidad )
#define  X_OFERTAS_IMPORTE                        GET_MEMORY_DOUBLE( __x_ofertas_importe )
#define  Z_OFERTA_IMPORTE                         GET_MEMORY_DOUBLE( __z_oferta_importe )
#define  Z_ANT_OFERTA_IMPORTE                     GET_MEMORY_DOUBLE( __z_ant_oferta_importe )
#define  XC_OFERTA_IMPORTE                        GET_MEMORY_DOUBLE( __xc_oferta_importe )
#define  NRO_ALFANUMERICO                         GET_MEMORY_STR( __nro_alfanumerico )
#define _NRO_ALFANUMERICO(x)                      GET_MEMORY_STR_CHAR( __nro_alfanumerico, x )
#define  RAM_CANT_DATOS_ADIC_MEDIO                GET_MEMORY_INT( __ram_cant_datos_adic_medio )
#define  NRO_ALFA                                 GET_MEMORY_STR( __nro_alfa )
#define _NRO_ALFA(x)                              GET_MEMORY_STR_CHAR( __nro_alfa, x )
#define  NRO_SUCURSAL_RENTA                       GET_MEMORY_INT( __nro_sucursal_renta )
#define  NRO_REGISTRO                             GET_MEMORY_INT( __nro_registro )
#define _X_DESCUENTOS_MEDIO(x)                   _GET_MEMORY_DOUBLE( __x_descuentos_medio, x )
#define _Z_DESCUENTOS_MEDIO(x)                   _GET_MEMORY_DOUBLE( __z_descuentos_medio, x )
#define _Z_ANT_DESCUENTOS_MEDIO(x)               _GET_MEMORY_DOUBLE( __z_ant_descuentos_medio, x )
#define _XC_DESCUENTOS_MEDIO(x)                  _GET_MEMORY_DOUBLE( __xc_descuentos_medio, x )
#define  X_IVA_NC_A                               GET_MEMORY_DOUBLE( __x_iva_nc_a )
#define  X_IVA_DEVOL_CONSUMIDOR_FINAL             GET_MEMORY_DOUBLE( __x_iva_devol_consumidor_final )
#define  Z_IVA_NC_A                               GET_MEMORY_DOUBLE( __z_iva_nc_a )
#define  Z_IVA_DEVOL_CONSUMIDOR_FINAL             GET_MEMORY_DOUBLE( __z_iva_devol_consumidor_final )
#define  Z_ANT_IVA_NC_A                           GET_MEMORY_DOUBLE( __z_ant_iva_nc_a )
#define  Z_ANT_IVA_DEVOL_CONSUMIDOR_FINAL         GET_MEMORY_DOUBLE( __z_ant_iva_devol_consumidor_final )
#define  XC_IVA_NC_A                              GET_MEMORY_DOUBLE( __xc_iva_nc_a )
#define  XC_IVA_DEVOL_CONSUMIDOR_FINAL            GET_MEMORY_DOUBLE( __xc_iva_devol_consumidor_final )
#define _RAM_MEDIOS_LIMITADOS(x)                 _GET_MEMORY_CHAR( __ram_medios_limitados, x )
#define _RAM_SUB_MEDIOS_LIMITADOS(x)             _GET_MEMORY_CHAR( __ram_sub_medios_limitados, x )
#define  CANTIDAD_MEDIOS_LIMITADOS                GET_MEMORY_CHAR( __cantidad_medios_limitados )
#define  RAM_DEVOLUCION_PARCIAL_REF               GET_MEMORY_CHAR( __ram_devolucion_parcial_ref )
#define _X_VENTAS_IMPORTE_SIN_REDONDEO(x)        _GET_MEMORY_DOUBLE( __x_ventas_importe_sin_redondeo, x )
#define _Z_VENTAS_IMPORTE_SIN_REDONDEO(x)        _GET_MEMORY_DOUBLE( __z_ventas_importe_sin_redondeo, x )
#define _Z_ANT_VENTAS_IMPORTE_SIN_REDONDEO(x)    _GET_MEMORY_DOUBLE( __z_ant_ventas_importe_sin_redondeo, x )
#define _XC_VENTAS_IMPORTE_SIN_REDONDEO(x)       _GET_MEMORY_DOUBLE( __xc_ventas_importe_sin_redondeo, x )
#define  RAM_NRO_SERIE_IMPRESORA                  GET_MEMORY_STR( __ram_nro_serie_impresora )
#define _RAM_NRO_SERIE_IMPRESORA(x)               GET_MEMORY_STR_CHAR( __ram_nro_serie_impresora, x )
#define  RAM_NRO_SERIE_IMPRESORA_ANT              GET_MEMORY_STR( __ram_nro_serie_impresora_ant )
#define _RAM_NRO_SERIE_IMPRESORA_ANT(x)           GET_MEMORY_STR_CHAR( __ram_nro_serie_impresora_ant, x )
#define  RAM_HORA_ANTERIOR                        GET_MEMORY_INT( __ram_hora_anterior )
#define _PAGO_COD_SUBMEDIO(x)                    _GET_MEMORY_INT( __pago_cod_submedio, x )
#define _X_COBROS_IMPORTE_ME(x)                  _GET_MEMORY_DOUBLE( __x_cobros_importe_me, x )
#define _Z_COBROS_IMPORTE_ME(x)                  _GET_MEMORY_DOUBLE( __z_cobros_importe_me, x )
#define _Z_ANT_COBROS_IMPORTE_ME(x)              _GET_MEMORY_DOUBLE( __z_ant_cobros_importe_me, x )
#define _XC_COBROS_IMPORTE_ME(x)                 _GET_MEMORY_DOUBLE( __xc_cobros_importe_me, x )
#define _X_RETIRO_FONDO_IMPORTE_PAGOS(x)         _GET_MEMORY_DOUBLE( __x_retiro_fondo_importe_pagos, x )
#define _Z_RETIRO_FONDO_IMPORTE_PAGOS(x)         _GET_MEMORY_DOUBLE( __z_retiro_fondo_importe_pagos, x )
#define _Z_ANT_RETIRO_FONDO_IMPORTE_PAGOS(x)     _GET_MEMORY_DOUBLE( __z_ant_retiro_fondo_importe_pagos, x )
#define _XC_RETIRO_FONDO_IMPORTE_PAGOS(x)        _GET_MEMORY_DOUBLE( __xc_retiro_fondo_importe_pagos, x )
#define  RAM_CUENTA_CLIENTE                       GET_MEMORY_STR( __ram_cuenta_cliente )
#define _RAM_CUENTA_CLIENTE(x)                    GET_MEMORY_STR_CHAR( __ram_cuenta_cliente, x )
#define  RAM_POS_MASK_ELIMINAR                    GET_MEMORY_CHAR( __ram_pos_mask_eliminar )
#define  RAM_CIERRE_Y                             GET_MEMORY_INT( __ram_cierre_y )
#define  RAM_COPIA                                GET_MEMORY_INT( __ram_copia )
#define  RAM_POS_REV_PROMO                        GET_MEMORY_INT( __ram_pos_rev_promo )
#define  ITEM_COD_ESPECIAL_ALFA                   GET_MEMORY_STR( __item_cod_especial_alfa )
#define _ITEM_COD_ESPECIAL_ALFA(x)                GET_MEMORY_STR_CHAR( __item_cod_especial_alfa, x )
#define  ITEM_COD_ENGANCHE_ALFA                   GET_MEMORY_STR( __item_cod_enganche_alfa )
#define _ITEM_COD_ENGANCHE_ALFA(x)                GET_MEMORY_STR_CHAR( __item_cod_enganche_alfa, x )
#define  RAM_COBRO_SERVICIOS                      GET_MEMORY_INT( __ram_cobro_servicios )
#define  VARIOS_VACIAR_EVENTOS_AL_INICIO          GET_MEMORY_CHAR( __varios_vaciar_eventos_al_inicio )
#define  NRO_EVENTO_PRIORIDAD_PROCESAR            GET_MEMORY_LONG( __nro_evento_prioridad_procesar )
#define  RAM_POS_PAGO                             GET_MEMORY_INT( __ram_pos_pago )
#define  RAM_AGREGA_ITEM_TICK_IMPORT              GET_MEMORY_INT( __ram_agrega_item_tick_import )
#define  RAM_TMPO_NO_VENTA                        GET_MEMORY_INT( __ram_tmpo_no_venta )
#define  X_TMPO_NO_VENTA                          GET_MEMORY_UNSIGNED( __x_tmpo_no_venta )
#define  Z_TMPO_NO_VENTA                          GET_MEMORY_UNSIGNED( __z_tmpo_no_venta )
#define  Z_ANT_TMPO_NO_VENTA                      GET_MEMORY_UNSIGNED( __z_ant_tmpo_no_venta )
#define  XC_TMPO_NO_VENTA                         GET_MEMORY_UNSIGNED( __xc_tmpo_no_venta )
#define  ITEM_IMPRIMIR_BARRA                      GET_MEMORY_CHAR( __item_imprimir_barra )
#define  RAM_FOLIO_COBRO                          GET_MEMORY_STR( __ram_folio_cobro )
#define _RAM_FOLIO_COBRO(x)                       GET_MEMORY_STR_CHAR( __ram_folio_cobro, x )
#define  ITEM_COD_CLASIFICACION_STR               GET_MEMORY_STR( __item_cod_clasificacion_str )
#define _ITEM_COD_CLASIFICACION_STR(x)            GET_MEMORY_STR_CHAR( __item_cod_clasificacion_str, x )
#define  RAM_FLAG_COBRO_VENTA                     GET_MEMORY_INT( __ram_flag_cobro_venta )
#define  RAM_HAY_COBRO_VENTA                      GET_MEMORY_CHAR( __ram_hay_cobro_venta )
#define  RAM_TMPO_OCIO_AUX                        GET_MEMORY_INT( __ram_tmpo_ocio_aux )
#define  RAM_CLIENTE_AFINIDAD_NOMBRE              GET_MEMORY_STR( __ram_cliente_afinidad_nombre )
#define _RAM_CLIENTE_AFINIDAD_NOMBRE(x)           GET_MEMORY_STR_CHAR( __ram_cliente_afinidad_nombre, x )
#define _PAGO_IMPORTE_DESCUENTO(x)               _GET_MEMORY_DOUBLE( __pago_importe_descuento, x )
#define  ITEM_MOTIVO_DEVOLUCION                   GET_MEMORY_INT( __item_motivo_devolucion )
#define  RAM_DIFERENCIA_REDONDEO_GRAV             GET_MEMORY_DOUBLE( __ram_diferencia_redondeo_grav )
#define  RAM_DIFERENCIA_REDONDEO_EXENT            GET_MEMORY_DOUBLE( __ram_diferencia_redondeo_exent )
#define  RAM_DIFERENCIA_REDONDEO_IVA              GET_MEMORY_DOUBLE( __ram_diferencia_redondeo_iva )
#define  X_DIF_REDONDEO_VTAS_CONTRIB              GET_MEMORY_DOUBLE( __x_dif_redondeo_vtas_contrib )
#define  Z_DIF_REDONDEO_VTAS_CONTRIB              GET_MEMORY_DOUBLE( __z_dif_redondeo_vtas_contrib )
#define  Z_ANT_DIF_REDONDEO_VTAS_CONTRIB          GET_MEMORY_DOUBLE( __z_ant_dif_redondeo_vtas_contrib )
#define  XC_DIF_REDONDEO_VTAS_CONTRIB             GET_MEMORY_DOUBLE( __xc_dif_redondeo_vtas_contrib )
#define  X_DIF_REDONDEO_IVA_CONTRIB               GET_MEMORY_DOUBLE( __x_dif_redondeo_iva_contrib )
#define  Z_DIF_REDONDEO_IVA_CONTRIB               GET_MEMORY_DOUBLE( __z_dif_redondeo_iva_contrib )
#define  Z_ANT_DIF_REDONDEO_IVA_CONTRIB           GET_MEMORY_DOUBLE( __z_ant_dif_redondeo_iva_contrib )
#define  XC_DIF_REDONDEO_IVA_CONTRIB              GET_MEMORY_DOUBLE( __xc_dif_redondeo_iva_contrib )
#define  X_DIF_REDONDEO_NC                        GET_MEMORY_DOUBLE( __x_dif_redondeo_nc )
#define  Z_DIF_REDONDEO_NC                        GET_MEMORY_DOUBLE( __z_dif_redondeo_nc )
#define  Z_ANT_DIF_REDONDEO_NC                    GET_MEMORY_DOUBLE( __z_ant_dif_redondeo_nc )
#define  XC_DIF_REDONDEO_NC                       GET_MEMORY_DOUBLE( __xc_dif_redondeo_nc )
#define  X_DIF_REDONDEO_IVA_NC                    GET_MEMORY_DOUBLE( __x_dif_redondeo_iva_nc )
#define  Z_DIF_REDONDEO_IVA_NC                    GET_MEMORY_DOUBLE( __z_dif_redondeo_iva_nc )
#define  Z_ANT_DIF_REDONDEO_IVA_NC                GET_MEMORY_DOUBLE( __z_ant_dif_redondeo_iva_nc )
#define  XC_DIF_REDONDEO_IVA_NC                   GET_MEMORY_DOUBLE( __xc_dif_redondeo_iva_nc )
#define  X_DIF_REDONDEO_GRAV_TICKET               GET_MEMORY_DOUBLE( __x_dif_redondeo_grav_ticket )
#define  X_DIF_REDONDEO_EXENT_TICKET              GET_MEMORY_DOUBLE( __x_dif_redondeo_exent_ticket )
#define  X_DIF_REDONDEO_IVA_TICKET                GET_MEMORY_DOUBLE( __x_dif_redondeo_iva_ticket )
#define  Z_DIF_REDONDEO_GRAV_TICKET               GET_MEMORY_DOUBLE( __z_dif_redondeo_grav_ticket )
#define  Z_DIF_REDONDEO_EXENT_TICKET              GET_MEMORY_DOUBLE( __z_dif_redondeo_exent_ticket )
#define  Z_DIF_REDONDEO_IVA_TICKET                GET_MEMORY_DOUBLE( __z_dif_redondeo_iva_ticket )
#define  Z_ANT_DIF_REDONDEO_GRAV_TICKET           GET_MEMORY_DOUBLE( __z_ant_dif_redondeo_grav_ticket )
#define  Z_ANT_DIF_REDONDEO_EXENT_TICKET          GET_MEMORY_DOUBLE( __z_ant_dif_redondeo_exent_ticket )
#define  Z_ANT_DIF_REDONDEO_IVA_TICKET            GET_MEMORY_DOUBLE( __z_ant_dif_redondeo_iva_ticket )
#define  XC_DIF_REDONDEO_GRAV_TICKET              GET_MEMORY_DOUBLE( __xc_dif_redondeo_grav_ticket )
#define  XC_DIF_REDONDEO_EXENT_TICKET             GET_MEMORY_DOUBLE( __xc_dif_redondeo_exent_ticket )
#define  XC_DIF_REDONDEO_IVA_TICKET               GET_MEMORY_DOUBLE( __xc_dif_redondeo_iva_ticket )
#define  RAM_PRECIO_CREDITO                       GET_MEMORY_INT( __ram_precio_credito )
#define  X_COBROS_IMPORTES_SERVICIOS              GET_MEMORY_DOUBLE( __x_cobros_importes_servicios )
#define  Z_COBROS_IMPORTES_SERVICIOS              GET_MEMORY_DOUBLE( __z_cobros_importes_servicios )
#define  Z_ANT_COBROS_IMPORTES_SERVICIOS          GET_MEMORY_DOUBLE( __z_ant_cobros_importes_servicios )
#define  XC_COBROS_IMPORTES_SERVIVIOS             GET_MEMORY_DOUBLE( __xc_cobros_importes_servivios )
#define _TOT_TICK_TOTAL_POR_COD_IVA(x)           _GET_MEMORY_DOUBLE( __tot_tick_total_por_cod_iva, x )
#define _TOT_TICK_IVA1_POR_COD_IVA(x)            _GET_MEMORY_DOUBLE( __tot_tick_iva1_por_cod_iva, x )
#define _TOT_TICK_IVA2_POR_COD_IVA(x)            _GET_MEMORY_DOUBLE( __tot_tick_iva2_por_cod_iva, x )
#define _TOT_TICK_IMP_INT_POR_COD_IVA(x)         _GET_MEMORY_DOUBLE( __tot_tick_imp_int_por_cod_iva, x )
#define  RAM_PERCEPCION_TACIS                     GET_MEMORY_DOUBLE( __ram_percepcion_tacis )
#define _TOT_TICK_ANT_TOTAL_POR_COD_IVA(x)       _GET_MEMORY_DOUBLE( __tot_tick_ant_total_por_cod_iva, x )
#define _TOT_TICK_ANT_IVA1_POR_COD_IVA(x)        _GET_MEMORY_DOUBLE( __tot_tick_ant_iva1_por_cod_iva, x )
#define _TOT_TICK_ANT_IVA2_POR_COD_IVA(x)        _GET_MEMORY_DOUBLE( __tot_tick_ant_iva2_por_cod_iva, x )
#define _TOT_TICK_ANT_IMP_INT_POR_COD_IVA(x)     _GET_MEMORY_DOUBLE( __tot_tick_ant_imp_int_por_cod_iva, x )
#define _TOT_TICK_SV_TOTAL_POR_COD_IVA(x)        _GET_MEMORY_DOUBLE( __tot_tick_sv_total_por_cod_iva, x )
#define _TOT_TICK_SV_IVA1_POR_COD_IVA(x)         _GET_MEMORY_DOUBLE( __tot_tick_sv_iva1_por_cod_iva, x )
#define _TOT_TICK_SV_IVA2_POR_COD_IVA(x)         _GET_MEMORY_DOUBLE( __tot_tick_sv_iva2_por_cod_iva, x )
#define _TOT_TICK_SV_IMP_INT_POR_COD_IVA(x)      _GET_MEMORY_DOUBLE( __tot_tick_sv_imp_int_por_cod_iva, x )
#define  RAM_CALCULAR_RETENC_NC                   GET_MEMORY_INT( __ram_calcular_retenc_nc )
#define _TOT_TICK_BASE_POR_COD_IVA(x)            _GET_MEMORY_DOUBLE( __tot_tick_base_por_cod_iva, x )
#define _TOT_TICK_ANT_BASE_POR_COD_IVA(x)        _GET_MEMORY_DOUBLE( __tot_tick_ant_base_por_cod_iva, x )
#define _TOT_TICK_SV_BASE_POR_COD_IVA(x)         _GET_MEMORY_DOUBLE( __tot_tick_sv_base_por_cod_iva, x )
#define  VARIOS_CAMBIO_FECHA_Y_HORA_CF            GET_MEMORY_CHAR( __varios_cambio_fecha_y_hora_cf )
#define  VAR_TARJETAS_IMPORTE                     GET_MEMORY_DOUBLE( __var_tarjetas_importe )
#define  VAR_TARJETAS_OPERACION                   GET_MEMORY_INT( __var_tarjetas_operacion )
#define  VAR_TARJETAS_ANT_IMPORTE                 GET_MEMORY_DOUBLE( __var_tarjetas_ant_importe )
#define  VAR_TARJETAS_ANT_OPERACION               GET_MEMORY_INT( __var_tarjetas_ant_operacion )
#define  RAM_NRO_CLIENTE_ANT                      GET_MEMORY_LONG( __ram_nro_cliente_ant )
#define  RAM_REIMPRESION                          GET_MEMORY_INT( __ram_reimpresion )
#define  RAM_INSTANCIA_EVENTO                     GET_MEMORY_INT( __ram_instancia_evento )
#define  RAM_MODO_EJECUCION_ANT                   GET_MEMORY_CHAR( __ram_modo_ejecucion_ant )
#define  RAM_IMPRESION_COMPROBANTE                GET_MEMORY_INT( __ram_impresion_comprobante )

/*---------------- Variables Configurables por el Usuario ----------------*/
#define  MODO_DEBUG                               GET_MEMORY_CHAR( __modo_debug )
#define  PERMITE_MODO_INVENTARIO                  GET_MEMORY_CHAR( __permite_modo_inventario )
#define  HARD_OMRON                               GET_MEMORY_CHAR( __hard_omron )
#define  HARD_TEC                                 GET_MEMORY_CHAR( __hard_tec )
#define  CONTROL_DE_APAGADO                       GET_MEMORY_CHAR( __control_de_apagado )
#define  INICIALIZAR_TIMER                        GET_MEMORY_CHAR( __inicializar_timer )
#define  BUFFER_LISTA                             GET_MEMORY_UNSIGNED( __buffer_lista )
#define  BUFFER_COPIAR_ARCHIVO                    GET_MEMORY_UNSIGNED( __buffer_copiar_archivo )
#define  BUFFER_VALIDACIONES                      GET_MEMORY_UNSIGNED( __buffer_validaciones )
#define  CANTIDAD_VALIDACIONES                    GET_MEMORY_UNSIGNED( __cantidad_validaciones )
#define  BUFFER_FACTURA                           GET_MEMORY_UNSIGNED( __buffer_factura )
#define  BUFFER_REMITO                            GET_MEMORY_UNSIGNED( __buffer_remito )
#define  TIPO_IMPRESORA                           GET_MEMORY_CHAR( __tipo_impresora )
#define  PUERTO_IMPRESORA                         GET_MEMORY_CHAR( __puerto_impresora )
#define  PUERTO_PARALELO                          GET_MEMORY_CHAR( __puerto_paralelo )
#define  IMPRESORA_FISCAL                         GET_MEMORY_CHAR( __impresora_fiscal )
#define  TIPO_TECLADO                             GET_MEMORY_CHAR( __tipo_teclado )
#define  ESTADO_NUM_LOCK                          GET_MEMORY_CHAR( __estado_num_lock )
#define  ESTADO_CAPS_LOCK                         GET_MEMORY_CHAR( __estado_caps_lock )
#define  HARD_INVEL                               GET_MEMORY_CHAR( __hard_invel )
#define  __RSION_PERIFERICOS_INVEL                GET_MEMORY_CHAR( ____rsion_perifericos_invel )
#define  TIPO_DE_SCANNER                          GET_MEMORY_CHAR( __tipo_de_scanner )
#define  SCANNER_PUERTO                           GET_MEMORY_INT( __scanner_puerto )
#define  SCANNER_DATA_BITS                        GET_MEMORY_INT( __scanner_data_bits )
#define  SCANNER_STOP_BITS                        GET_MEMORY_INT( __scanner_stop_bits )
#define  SCANNER_PARITY                           GET_MEMORY_INT( __scanner_parity )
#define  SCANNER_BAUDIOS                          GET_MEMORY_INT( __scanner_baudios )
#define  TRABAR_CODIGO_INEXISTENTE                GET_MEMORY_CHAR( __trabar_codigo_inexistente )
#define  TIPO_BALANZA                             GET_MEMORY_CHAR( __tipo_balanza )
#define  BALANZA_PUERTO                           GET_MEMORY_INT( __balanza_puerto )
#define  BALANZA_DATA_BIT                         GET_MEMORY_INT( __balanza_data_bit )
#define  BALANZA_STOP_BIT                         GET_MEMORY_INT( __balanza_stop_bit )
#define  BALANZA_PARITY                           GET_MEMORY_INT( __balanza_parity )
#define  BALANZA_BAUDIOS                          GET_MEMORY_INT( __balanza_baudios )
#define  BALANZA_CANTIDAD_LECTURAS                GET_MEMORY_INT( __balanza_cantidad_lecturas )
#define  TIPO_CAJON                               GET_MEMORY_CHAR( __tipo_cajon )
#define  NRO_CAJON                                GET_MEMORY_INT( __nro_cajon )
#define  TIPO_LECTOR_BANDA                        GET_MEMORY_CHAR( __tipo_lector_banda )
#define  TIPO_DISPLAY                             GET_MEMORY_CHAR( __tipo_display )
#define  MOSTRAR_HORA_DISPLAY                     GET_MEMORY_CHAR( __mostrar_hora_display )
#define  TIPO_LLAVE                               GET_MEMORY_CHAR( __tipo_llave )
#define  UTILIZA_PINPAD                           GET_MEMORY_CHAR( __utiliza_pinpad )
#define  SALVA_PANTALLA                           GET_MEMORY_UNSIGNED( __salva_pantalla )
#define  PERMANENCIA_PANTALLA                     GET_MEMORY_UNSIGNED( __permanencia_pantalla )
#define  PREFIJO_TOMRA                            GET_MEMORY_STR( __prefijo_tomra )
#define _PREFIJO_TOMRA(x)                         GET_MEMORY_STR_CHAR( __prefijo_tomra, x )
#define  L_TICKET_TOMRA                           GET_MEMORY_INT( __l_ticket_tomra )
#define  ENTEROS                                  GET_MEMORY_INT( __enteros )
#define  DECIMALES                                GET_MEMORY_INT( __decimales )
#define  PUNTO_DECIMAL_OBLIGATORIO                GET_MEMORY_CHAR( __punto_decimal_obligatorio )
#define  CANTIDADES_CON_DECIMALES                 GET_MEMORY_CHAR( __cantidades_con_decimales )
#define  L_CODIGO                                 GET_MEMORY_INT( __l_codigo )
#define  L_COD_INTERNO                            GET_MEMORY_INT( __l_cod_interno )
#define  FORMATO_CONSULTA                         GET_MEMORY_CHAR( __formato_consulta )
#define  FORMATO_TICKET                           GET_MEMORY_CHAR( __formato_ticket )
#define  FORMATO_PANTALLA                         GET_MEMORY_CHAR( __formato_pantalla )
#define  DATOS_CAJERO_EN_TICKET                   GET_MEMORY_CHAR( __datos_cajero_en_ticket )
#define  DATOS_VENDEDOR_EN_TICKET                 GET_MEMORY_CHAR( __datos_vendedor_en_ticket )
#define  IMPRIMIR_PIE_Y                           GET_MEMORY_CHAR( __imprimir_pie_y )
#define  IMPRIMIR_PIE_Z                           GET_MEMORY_CHAR( __imprimir_pie_z )
#define  ID_BALANZA                               GET_MEMORY_STR( __id_balanza )
#define _ID_BALANZA(x)                            GET_MEMORY_STR_CHAR( __id_balanza, x )
#define  LARGO_CODIGO_DE_CONFECCION               GET_MEMORY_CHAR( __largo_codigo_de_confeccion )
#define  ID_CONFECCION_INICIAL                    GET_MEMORY_STR( __id_confeccion_inicial )
#define _ID_CONFECCION_INICIAL(x)                 GET_MEMORY_STR_CHAR( __id_confeccion_inicial, x )
#define  ID_CONFECCION_FINAL                      GET_MEMORY_STR( __id_confeccion_final )
#define _ID_CONFECCION_FINAL(x)                   GET_MEMORY_STR_CHAR( __id_confeccion_final, x )
#define  SUFIJO_CODIGOS_DE_PREEMPAQUE             GET_MEMORY_STR( __sufijo_codigos_de_preempaque )
#define _SUFIJO_CODIGOS_DE_PREEMPAQUE(x)          GET_MEMORY_STR_CHAR( __sufijo_codigos_de_preempaque, x )
#define  MOSTRAR_HORA_PANTALLA                    GET_MEMORY_CHAR( __mostrar_hora_pantalla )
#define  TIPO_CLAVE_SUPERVISOR                    GET_MEMORY_CHAR( __tipo_clave_supervisor )
#define  TIPO_TARJETA                             GET_MEMORY_CHAR( __tipo_tarjeta )
#define  TIPO_CLAVE_CAJERO                        GET_MEMORY_CHAR( __tipo_clave_cajero )
#define  IDENTIFICACION_USUARIO                   GET_MEMORY_CHAR( __identificacion_usuario )
#define  USAR_LEGAJO                              GET_MEMORY_CHAR( __usar_legajo )
#define  BREAK                                    GET_MEMORY_CHAR( __break )
#define  VERIFICAR_RAM                            GET_MEMORY_CHAR( __verificar_ram )
#define  VERIFICAR_IMPRESORA_ON_LINE              GET_MEMORY_CHAR( __verificar_impresora_on_line )
#define  VERIFICAR_PAPER_OUT                      GET_MEMORY_CHAR( __verificar_paper_out )
#define  PEDIR_CLAVE_EN_INTERESES                 GET_MEMORY_CHAR( __pedir_clave_en_intereses )
#define  PEDIR_CLAVE_EN_COBROS                    GET_MEMORY_CHAR( __pedir_clave_en_cobros )
#define  PEDIR_CLAVE_EN_PAGOS                     GET_MEMORY_CHAR( __pedir_clave_en_pagos )
#define  PEDIR_CLAVE_EN_REIMPRESION               GET_MEMORY_CHAR( __pedir_clave_en_reimpresion )
#define  PIDE_SUPERVISOR_PARA_CIERRE              GET_MEMORY_CHAR( __pide_supervisor_para_cierre )
#define  DEVOLUCION_HABILITADA                    GET_MEMORY_CHAR( __devolucion_habilitada )
#define  VERIFICAR_IMPORTE_ENVASES                GET_MEMORY_CHAR( __verificar_importe_envases )
#define  VERIFICAR_ENVASES_POR_RENGLON            GET_MEMORY_CHAR( __verificar_envases_por_renglon )
#define  CONTROLAR_LIMITE_VENTA                   GET_MEMORY_CHAR( __controlar_limite_venta )
#define  VERIFICAR_LIMITE_DGI                     GET_MEMORY_CHAR( __verificar_limite_dgi )
#define  ANULACIONES_PERMITIDAS                   GET_MEMORY_CHAR( __anulaciones_permitidas )
#define  AUTORIZAR_ANULACIONES                    GET_MEMORY_CHAR( __autorizar_anulaciones )
#define  PERMITE_ANULAR_TICKET                    GET_MEMORY_CHAR( __permite_anular_ticket )
#define  VERIFICAR_CAJEROS_HABILITADOS            GET_MEMORY_CHAR( __verificar_cajeros_habilitados )
#define  CONTROLAR_VUELTO                         GET_MEMORY_CHAR( __controlar_vuelto )
#define  CONTROLAR_CAJON                          GET_MEMORY_CHAR( __controlar_cajon )
#define  PEDIR_SUPERVISOR_PARA_CODIGO_0           GET_MEMORY_CHAR( __pedir_supervisor_para_codigo_0 )
#define  HABILITAR_CODIGOS_PREEMPAQUE             GET_MEMORY_CHAR( __habilitar_codigos_preempaque )
#define  FACTURACION_HABILITADA                   GET_MEMORY_CHAR( __facturacion_habilitada )
#define  TIPO_FACTURA_A                           GET_MEMORY_CHAR( __tipo_factura_a )
#define  NRO_FACTURA                              GET_MEMORY_CHAR( __nro_factura )
#define  LIMITE_LINEAS_FACTURA                    GET_MEMORY_UNSIGNED( __limite_lineas_factura )
#define  PEDIR_OC_GD                              GET_MEMORY_CHAR( __pedir_oc_gd )
#define  IMPRIMIR_RETIRAR_FACTURA                 GET_MEMORY_CHAR( __imprimir_retirar_factura )
#define  PUERTO_IMPRESORA_OPCIONAL_FACTUR         GET_MEMORY_CHAR( __puerto_impresora_opcional_factur )
#define  MODO_FACTURACION                         GET_MEMORY_CHAR( __modo_facturacion )
#define  MODO_FACTURACION_POR_DEFECTO             GET_MEMORY_CHAR( __modo_facturacion_por_defecto )
#define  NRO_CLIENTE_POR_DEFECTO                  GET_MEMORY_CHAR( __nro_cliente_por_defecto )
#define  MEDIO_PAGO_NC                            GET_MEMORY_CHAR( __medio_pago_nc )
#define  PEDIR_NRO_REMITO                         GET_MEMORY_CHAR( __pedir_nro_remito )
#define  PEDIR_FECHAS_AL_FACTURAR_REMITOS         GET_MEMORY_CHAR( __pedir_fechas_al_facturar_remitos )
#define  UTILIZA_SOCIOS                           GET_MEMORY_CHAR( __utiliza_socios )
#define  GENERAR_ARCHIVO_DE_SOCIOS                GET_MEMORY_CHAR( __generar_archivo_de_socios )
#define  CLIENTE_OBLIGATORIO                      GET_MEMORY_CHAR( __cliente_obligatorio )
#define  CUOTAS_HABILITADAS                       GET_MEMORY_CHAR( __cuotas_habilitadas )
#define  MENU_CUOTAS                              GET_MEMORY_CHAR( __menu_cuotas )
#define  PUEDE_AUTORIZAR_TARJETA_EN_BOLET         GET_MEMORY_CHAR( __puede_autorizar_tarjeta_en_bolet )
#define  VERIFICAR_DATOS_CUPON                    GET_MEMORY_CHAR( __verificar_datos_cupon )
#define  AUTORIZACION_ON_LINE_HABILITADA          GET_MEMORY_CHAR( __autorizacion_on_line_habilitada )
#define  AUT_ON_LINE_SI_PASA_LIMITE               GET_MEMORY_CHAR( __aut_on_line_si_pasa_limite )
#define  GENERAR_CUPONES_SI_ON_LINE               GET_MEMORY_CHAR( __generar_cupones_si_on_line )
#define  VALIDAR_QUE_PAGUE_CON_TARJETA            GET_MEMORY_CHAR( __validar_que_pague_con_tarjeta )
#define  VERIFICAR_TARJETAS_REPETIDAS             GET_MEMORY_CHAR( __verificar_tarjetas_repetidas )
#define  TARJETAS_PERMITIDAS_EN_COMTAR            GET_MEMORY_INT( __tarjetas_permitidas_en_comtar )
#define  AUTORIZAR_TARJETAS_REPETIDAS             GET_MEMORY_CHAR( __autorizar_tarjetas_repetidas )
#define  CABAL_Y_CARTA_FRANCA_A_POSNET            GET_MEMORY_CHAR( __cabal_y_carta_franca_a_posnet )
#define  PEDIR_CLAVE_EN_TARJETA_ESPECIAL          GET_MEMORY_CHAR( __pedir_clave_en_tarjeta_especial )
#define  PEDIR_DATOS_TARJETA_AL_VALIDAR           GET_MEMORY_CHAR( __pedir_datos_tarjeta_al_validar )
#define  VERIFICAR_DATOS_TARJETA                  GET_MEMORY_CHAR( __verificar_datos_tarjeta )
#define  PEDIR_CODIGO_POSTAL                      GET_MEMORY_CHAR( __pedir_codigo_postal )
#define  PEDIR_CODIGO_HORA                        GET_MEMORY_CHAR( __pedir_codigo_hora )
#define  PEDIR_CUIT                               GET_MEMORY_CHAR( __pedir_cuit )
#define  PEDIR_TELEFONO                           GET_MEMORY_CHAR( __pedir_telefono )
#define  USAR_MENU_BANCOS                         GET_MEMORY_CHAR( __usar_menu_bancos )
#define  PEDIR_CLIENTE_EN_CHEQUES                 GET_MEMORY_CHAR( __pedir_cliente_en_cheques )
#define  PEDIR_COMPROBANTE_CTA_CTE                GET_MEMORY_CHAR( __pedir_comprobante_cta_cte )
#define  EMITIR_VOUCHER_CTA_CTE                   GET_MEMORY_CHAR( __emitir_voucher_cta_cte )
#define  T_COMPRA_ENTRA_COMO_CTACTE               GET_MEMORY_CHAR( __t_compra_entra_como_ctacte )
#define  USAR_CUIT_COMO_NRO_CLIENTE               GET_MEMORY_CHAR( __usar_cuit_como_nro_cliente )
#define  CTA_CTE_ON_LINE                          GET_MEMORY_CHAR( __cta_cte_on_line )
#define  NOM_CANAL_TCP_IP                         GET_MEMORY_STR( __nom_canal_tcp_ip )
#define _NOM_CANAL_TCP_IP(x)                      GET_MEMORY_STR_CHAR( __nom_canal_tcp_ip, x )
#define  TRATAMIENTO_MUTUALES                     GET_MEMORY_CHAR( __tratamiento_mutuales )
#define  DISCRIMINAR_RETIROS                      GET_MEMORY_CHAR( __discriminar_retiros )
#define  DISCRIMINAR_COBROS                       GET_MEMORY_CHAR( __discriminar_cobros )
#define  INF10_HABILITADO                         GET_MEMORY_CHAR( __inf10_habilitado )
#define  INF15_HABILITADO                         GET_MEMORY_CHAR( __inf15_habilitado )
#define  EMITIR_INF00_EN_Y                        GET_MEMORY_CHAR( __emitir_inf00_en_y )
#define  RECALCULAR_IVA_EN_INF_00                 GET_MEMORY_CHAR( __recalcular_iva_en_inf_00 )
#define  EMITIR_INFORME_DIFERENCIAS               GET_MEMORY_CHAR( __emitir_informe_diferencias )
#define  IMPRIME_FONDO_FIJO                       GET_MEMORY_CHAR( __imprime_fondo_fijo )
#define  IMPRIME_ADELANTOS                        GET_MEMORY_CHAR( __imprime_adelantos )
#define  IMPRIME_RETIROS                          GET_MEMORY_CHAR( __imprime_retiros )
#define  IMPRIME_DEVOLUCIONES                     GET_MEMORY_CHAR( __imprime_devoluciones )
#define  TIPO_REDONDEO                            GET_MEMORY_CHAR( __tipo_redondeo )
#define  ULTIMO_DECIMAL                           GET_MEMORY_CHAR( __ultimo_decimal )
#define  PRESENTACIONES_HABILITADAS               GET_MEMORY_CHAR( __presentaciones_habilitadas )
#define  TIPO_PRESENTACIONES                      GET_MEMORY_CHAR( __tipo_presentaciones )
#define  UTILIZA_PROMOCIONES                      GET_MEMORY_CHAR( __utiliza_promociones )
#define  RESET_BONOS                              GET_MEMORY_CHAR( __reset_bonos )
#define  CREDITO_DEFAULT                          GET_MEMORY_CHAR( __credito_default )
#define  PERMITIR_TECLA_CREDITO                   GET_MEMORY_CHAR( __permitir_tecla_credito )
#define  USAR_PRECIO_CREDITO                      GET_MEMORY_CHAR( __usar_precio_credito )
#define  CONTROLAR_PAGO_CONTADO                   GET_MEMORY_CHAR( __controlar_pago_contado )
#define  USAR_ACU_VTA_ALL                         GET_MEMORY_CHAR( __usar_acu_vta_all )
#define  USAR_ACU_VTA_CAJA                        GET_MEMORY_CHAR( __usar_acu_vta_caja )
#define  USAR_ACU_VTA_LOCAL                       GET_MEMORY_CHAR( __usar_acu_vta_local )
#define  VENTAS_A_VENTAS_Z                        GET_MEMORY_CHAR( __ventas_a_ventas_z )
#define  ACTUALIZAR_STOCK                         GET_MEMORY_CHAR( __actualizar_stock )
#define  USAR_EVENTOS_ALL                         GET_MEMORY_CHAR( __usar_eventos_all )
#define  MULTI_SUPERVISOR                         GET_MEMORY_CHAR( __multi_supervisor )
#define  HAY_COD_DEV_ENVASE                       GET_MEMORY_CHAR( __hay_cod_dev_envase )
#define  MENSAJES_HABILITADOS                     GET_MEMORY_CHAR( __mensajes_habilitados )
#define  UTILIZA_MOZOS                            GET_MEMORY_CHAR( __utiliza_mozos )
#define  FUNCION_PIDE_PRECIO                      GET_MEMORY_CHAR( __funcion_pide_precio )
#define  RESETEAR_NRO_TICKET                      GET_MEMORY_CHAR( __resetear_nro_ticket )
#define  MANTENER_FECHA_Z                         GET_MEMORY_CHAR( __mantener_fecha_z )
#define  UTILIZAR_DIA_OPERATIVO                   GET_MEMORY_CHAR( __utilizar_dia_operativo )
#define  UTILIZA_PUESTOS                          GET_MEMORY_CHAR( __utiliza_puestos )
#define  USAR_VARIACION_PRECIO_SIEMPRE            GET_MEMORY_CHAR( __usar_variacion_precio_siempre )
#define  MOSTRAR_FECHA_OPERACION                  GET_MEMORY_CHAR( __mostrar_fecha_operacion )
#define  SALIR_AL_DOS                             GET_MEMORY_CHAR( __salir_al_dos )
#define  MOSTRAR_VUELTO                           GET_MEMORY_CHAR( __mostrar_vuelto )
#define  USAR_DISP_EN_BUSQUEDA_SCANNER            GET_MEMORY_CHAR( __usar_disp_en_busqueda_scanner )
#define  CAJEROS_FLOTANTES                        GET_MEMORY_CHAR( __cajeros_flotantes )
#define  VENDEDOR_OBLIGATORIO                     GET_MEMORY_CHAR( __vendedor_obligatorio )
#define  IMPRIMIR_TICKET_AL_FINAL                 GET_MEMORY_CHAR( __imprimir_ticket_al_final )
#define  REINICIALIZAR_AL_TERMINAR                GET_MEMORY_CHAR( __reinicializar_al_terminar )
#define  USAR_ESCAPE_PARA_CIERRE                  GET_MEMORY_CHAR( __usar_escape_para_cierre )
#define  TRATAMIENTO_CODIGO_0                     GET_MEMORY_CHAR( __tratamiento_codigo_0 )
#define  MOSTRAR_DIFERENCIAS                      GET_MEMORY_CHAR( __mostrar_diferencias )
#define  CONTROLAR_CIERRE_FALLIDO                 GET_MEMORY_CHAR( __controlar_cierre_fallido )
#define  IMPRIMIR_TOTAL_EN_TICKET_FACTURA         GET_MEMORY_CHAR( __imprimir_total_en_ticket_factura )
#define  INCREMENTAR_NRO_EN_REIMPRESION           GET_MEMORY_CHAR( __incrementar_nro_en_reimpresion )
#define  ID_AFINIDAD                              GET_MEMORY_STR( __id_afinidad )
#define _ID_AFINIDAD(x)                           GET_MEMORY_STR_CHAR( __id_afinidad, x )
#define  INICIALIZAR_VALDACIONES                  GET_MEMORY_CHAR( __inicializar_valdaciones )
#define  HABILITAR_ENTER_EN_BUSQUEDA              GET_MEMORY_CHAR( __habilitar_enter_en_busqueda )
#define  FACTURA_CON_PRECIO_FINAL                 GET_MEMORY_CHAR( __factura_con_precio_final )
#define  PERMITIDOS_DURANTE_ESPERA                GET_MEMORY_CHAR( __permitidos_durante_espera )
#define  TIPO_REMITO                              GET_MEMORY_CHAR( __tipo_remito )
#define  TIPO_FACTURA_B                           GET_MEMORY_CHAR( __tipo_factura_b )
#define  PEDIR_PRECIO_EN_NC                       GET_MEMORY_CHAR( __pedir_precio_en_nc )
#define  DEBE_PEDIR_BOLETA_ANTERIOR               GET_MEMORY_CHAR( __debe_pedir_boleta_anterior )
#define  INF_TAR_HABILITADO                       GET_MEMORY_CHAR( __inf_tar_habilitado )
#define  IMPRIMIR_NRO_EVENTO                      GET_MEMORY_CHAR( __imprimir_nro_evento )
#define  UTILIZA_PERFILES                         GET_MEMORY_CHAR( __utiliza_perfiles )
#define  UTILIZA_DRIVER_TCP_IP                    GET_MEMORY_CHAR( __utiliza_driver_tcp_ip )
#define  PEDIR_SUPERVISOR_DESC_IMP                GET_MEMORY_CHAR( __pedir_supervisor_desc_imp )
#define  PEDIR_SUPERVISOR_DESC_PORC               GET_MEMORY_CHAR( __pedir_supervisor_desc_porc )
#define  PEDIR_SUPERVISOR_EN_CAMBIO               GET_MEMORY_CHAR( __pedir_supervisor_en_cambio )
#define  VALIDACION_DEL_NRO_DE_VENDEDOR           GET_MEMORY_CHAR( __validacion_del_nro_de_vendedor )
#define  TIPO_NC_A                                GET_MEMORY_CHAR( __tipo_nc_a )
#define  TIPO_NC_B                                GET_MEMORY_CHAR( __tipo_nc_b )
#define  ACEPTA_PRECIO_CERO                       GET_MEMORY_CHAR( __acepta_precio_cero )
#define  CALCULAR_RETENCION_DGR                   GET_MEMORY_CHAR( __calcular_retencion_dgr )
#define  CALCULAR_PERCEPCION_DGI                  GET_MEMORY_CHAR( __calcular_percepcion_dgi )
#define  SUPERVISOR_PARA_ANULAR_TICKET            GET_MEMORY_CHAR( __supervisor_para_anular_ticket )
#define  PEDIR_CODIGO_DE_FINANCIACION             GET_MEMORY_CHAR( __pedir_codigo_de_financiacion )
#define  PEDIR_CLAVE_EN_PERFILES                  GET_MEMORY_CHAR( __pedir_clave_en_perfiles )
#define  INF05_HABILITADO                         GET_MEMORY_CHAR( __inf05_habilitado )
#define  EMITIR_VOUCHER_TARJETA                   GET_MEMORY_CHAR( __emitir_voucher_tarjeta )
#define  PEDIR_CLAVE_AUTORIZACION_TARJETA         GET_MEMORY_CHAR( __pedir_clave_autorizacion_tarjeta )
#define  BORRAR_TARJETA_AL_SALIR_DE_PAGOS         GET_MEMORY_CHAR( __borrar_tarjeta_al_salir_de_pagos )
#define  BORRAR_PERFIL_AL_SALIR_DE_PAGOS          GET_MEMORY_CHAR( __borrar_perfil_al_salir_de_pagos )
#define  ANULAR_TICKET_SI_NO_SE_AUTORIZA          GET_MEMORY_CHAR( __anular_ticket_si_no_se_autoriza )
#define  TIME_OUT_CORTE_MANUAL                    GET_MEMORY_CHAR( __time_out_corte_manual )
#define  IMPRIMIR_COBROS                          GET_MEMORY_CHAR( __imprimir_cobros )
#define  IMPRIMIR_TICKET_SI_ES_REMOTA             GET_MEMORY_CHAR( __imprimir_ticket_si_es_remota )
#define  PASAR_TARJETA_SOLO_AL_INICIO             GET_MEMORY_CHAR( __pasar_tarjeta_solo_al_inicio )
#define  EXENTO_COMO_GRAVADO_EN_FAC_A             GET_MEMORY_CHAR( __exento_como_gravado_en_fac_a )
#define  SUPERVISOR_PARA_CLIENTE_MANUAL           GET_MEMORY_CHAR( __supervisor_para_cliente_manual )
#define  VACIAR_LOG_AL_TERMINAR                   GET_MEMORY_CHAR( __vaciar_log_al_terminar )
#define  MODO_EXPORT_VTAS                         GET_MEMORY_CHAR( __modo_export_vtas )
#define  MODO_DEVOL_ENV                           GET_MEMORY_CHAR( __modo_devol_env )
#define  IMPRIMIR_TICKET                          GET_MEMORY_CHAR( __imprimir_ticket )
#define  IMPORTAR_VENTAS                          GET_MEMORY_CHAR( __importar_ventas )
#define  NC_PERMANENTE                            GET_MEMORY_CHAR( __nc_permanente )
#define  PRESENTACION_A_TOMAR                     GET_MEMORY_CHAR( __presentacion_a_tomar )
#define  CONSULTAR_ESTADO_CTACTE                  GET_MEMORY_CHAR( __consultar_estado_ctacte )
#define  PEDIR_PRECIO_EN_CAMBIO                   GET_MEMORY_CHAR( __pedir_precio_en_cambio )
#define  PERMITE_CAMBIO_NEGATIVO                  GET_MEMORY_CHAR( __permite_cambio_negativo )
#define  REIMPRIMIR_COMPROBANTES                  GET_MEMORY_CHAR( __reimprimir_comprobantes )
#define  GRABAR_ALL_EN_FACTURAS_LOCALES           GET_MEMORY_CHAR( __grabar_all_en_facturas_locales )
#define  GRABAR_EVENTO_EN_REIMPRESION             GET_MEMORY_CHAR( __grabar_evento_en_reimpresion )
#define  BUFFER_SPOOLER                           GET_MEMORY_UNSIGNED( __buffer_spooler )
#define  CONTROLAR_PLAZO_FINANCIACION             GET_MEMORY_CHAR( __controlar_plazo_financiacion )
#define  PEDIR_CUENTA_BANCARIA                    GET_MEMORY_CHAR( __pedir_cuenta_bancaria )
#define  PRIMER_MEDIO_DE_PAGO_TARJETA             GET_MEMORY_CHAR( __primer_medio_de_pago_tarjeta )
#define  POSICION_NRO_TICKET                      GET_MEMORY_CHAR( __posicion_nro_ticket )
#define  PERMITE_CAMBIAR_VENDEDOR                 GET_MEMORY_CHAR( __permite_cambiar_vendedor )
#define  PEDIR_DIFERENCIAS                        GET_MEMORY_CHAR( __pedir_diferencias )
#define  SUPERVISOR_PARA_ABRIR_CAJON              GET_MEMORY_CHAR( __supervisor_para_abrir_cajon )
#define  ACTUALIZAR_PRECIO_EN_PEDIDOS             GET_MEMORY_CHAR( __actualizar_precio_en_pedidos )
#define  VERIFICAR_TARJETAS_PERMITIDAS            GET_MEMORY_CHAR( __verificar_tarjetas_permitidas )
#define  POSICION_VUELTO                          GET_MEMORY_CHAR( __posicion_vuelto )
#define  IMPRIMIR_MARCA_TARJETA_EN_TICKET         GET_MEMORY_CHAR( __imprimir_marca_tarjeta_en_ticket )
#define  PEDIR_NUMERO_FACTURA                     GET_MEMORY_CHAR( __pedir_numero_factura )
#define  IMPRIMIR_BULTOS_EN_TICKET                GET_MEMORY_CHAR( __imprimir_bultos_en_ticket )
#define  MODO_IDENTIFICACION_USUARIOS             GET_MEMORY_CHAR( __modo_identificacion_usuarios )
#define  PREFIJO_TARJETA_USUARIO                  GET_MEMORY_STR( __prefijo_tarjeta_usuario )
#define _PREFIJO_TARJETA_USUARIO(x)               GET_MEMORY_STR_CHAR( __prefijo_tarjeta_usuario, x )
#define  COPIAR_EVENTOS_EN_CIERRE                 GET_MEMORY_CHAR( __copiar_eventos_en_cierre )
#define  TOMAR_DIA_OPERATIVO_AL_INICIO            GET_MEMORY_CHAR( __tomar_dia_operativo_al_inicio )
#define  CONSULTA_AFINIDAD_ON_LINE                GET_MEMORY_CHAR( __consulta_afinidad_on_line )
#define  COMPRA_AFINIDAD_ON_LINE                  GET_MEMORY_CHAR( __compra_afinidad_on_line )
#define  PERMITE_SUPERAR_LIMITE_TICKET            GET_MEMORY_CHAR( __permite_superar_limite_ticket )
#define  UTILIZAR_ARTICULOS_REMOTO                GET_MEMORY_CHAR( __utilizar_articulos_remoto )
#define  MULTIPLES_USUARIOS_PARA_VENTA            GET_MEMORY_CHAR( __multiples_usuarios_para_venta )
#define  SALIR_DESPUES_DE_CADA_TICKET             GET_MEMORY_CHAR( __salir_despues_de_cada_ticket )
#define  USAR_NUMERO_DE_CAJERO_COMO_VENDE         GET_MEMORY_CHAR( __usar_numero_de_cajero_como_vende )
#define  MODO_RESTRINGIDO_EN_MEDIOS               GET_MEMORY_CHAR( __modo_restringido_en_medios )
#define  PEDIR_SUPERVISOR_PARA_HABIL_CAJO         GET_MEMORY_CHAR( __pedir_supervisor_para_habil_cajo )
#define  CONTROLAR_TOTAL_EN_CUOTAS                GET_MEMORY_CHAR( __controlar_total_en_cuotas )
#define  RETIRO_REMOTO_HABILITADO                 GET_MEMORY_CHAR( __retiro_remoto_habilitado )
#define _BMP_EN_INICIO_DEL_POS(x)                _GET_MEMORY_STR( __bmp_en_inicio_del_pos, x )
#define  MOSTRAR_SUBTOTAL_EN_DISPLAY              GET_MEMORY_CHAR( __mostrar_subtotal_en_display )
#define  USAR_DYNAKEY                             GET_MEMORY_CHAR( __usar_dynakey )
#define  USAR_NRO_CAJA_DGI                        GET_MEMORY_CHAR( __usar_nro_caja_dgi )
#define  SEGUNDOS_VERIFICAR_CAMBIO_ARCHIV         GET_MEMORY_INT( __segundos_verificar_cambio_archiv )
#define  DIGITOS_BIN_INTERNACIONAL                GET_MEMORY_CHAR( __digitos_bin_internacional )
#define  UTILIZAR_TABLA_MARCAS_ESPECIALES         GET_MEMORY_CHAR( __utilizar_tabla_marcas_especiales )
#define  BUSQUEDA_COD_INTERNO_ALTERNATIVA         GET_MEMORY_CHAR( __busqueda_cod_interno_alternativa )
#define  IMPORTE_CONTADO_ADITIVO_EN_PAGOS         GET_MEMORY_CHAR( __importe_contado_aditivo_en_pagos )
#define  PAUSA_EN_MENSAJES                        GET_MEMORY_CHAR( __pausa_en_mensajes )
#define  DESCARTAR_VERIFICADOR_BUSQ_ALTER         GET_MEMORY_CHAR( __descartar_verificador_busq_alter )
#define  IMPRIMIR_LEGAJO_CAJERO                   GET_MEMORY_CHAR( __imprimir_legajo_cajero )
#define  IMPRIMIR_DETALLE_EN_JOURNAL              GET_MEMORY_CHAR( __imprimir_detalle_en_journal )
#define  CODIGO_DE_VENTA_POR_CLASIFICACIO         GET_MEMORY_CHAR( __codigo_de_venta_por_clasificacio )
#define  CODIGO_VALIDACION_LIMITE_DGI             GET_MEMORY_CHAR( __codigo_validacion_limite_dgi )
#define  MODALIDAD_PREEMPAQUE                     GET_MEMORY_CHAR( __modalidad_preempaque )
#define  PEDIR_SUPERVISOR_TICKET_EN_ESPER         GET_MEMORY_CHAR( __pedir_supervisor_ticket_en_esper )
#define  MODO_DEVOLUCION                          GET_MEMORY_CHAR( __modo_devolucion )
#define  PEDIR_SUPERVISOR_DIFERENCIA_PREC         GET_MEMORY_CHAR( __pedir_supervisor_diferencia_prec )
#define  PEDIR_SUPERVISOR_COMPROMISO_200          GET_MEMORY_CHAR( __pedir_supervisor_compromiso_200 )
#define  PEDIR_SUPERVISOR_PRECIO_MAS_BAJO         GET_MEMORY_CHAR( __pedir_supervisor_precio_mas_bajo )
#define  USAR_VENTA_POR_CLASIFICACION             GET_MEMORY_CHAR( __usar_venta_por_clasificacion )
#define  USAR_VENTA_POR_MEDIOS                    GET_MEMORY_CHAR( __usar_venta_por_medios )
#define  USAR_VENTA_POR_XYZ                       GET_MEMORY_CHAR( __usar_venta_por_xyz )
#define  ID_DEVOLUCION_ENVASES                    GET_MEMORY_STR( __id_devolucion_envases )
#define _ID_DEVOLUCION_ENVASES(x)                 GET_MEMORY_STR_CHAR( __id_devolucion_envases, x )
#define  SCRIPT_RESPUESTA                         GET_MEMORY_CHAR( __script_respuesta )
#define  PEDIR_SUPERVISOR_REIMP_VALIDACIO         GET_MEMORY_CHAR( __pedir_supervisor_reimp_validacio )
#define  MODO_IDENTIFICACION_SUPERVISOR           GET_MEMORY_CHAR( __modo_identificacion_supervisor )
#define  PEDIR_CAJERO_PARA_CIERRE                 GET_MEMORY_CHAR( __pedir_cajero_para_cierre )
#define  TOMAR_NRO_COMERCIO_CUOTAS                GET_MEMORY_CHAR( __tomar_nro_comercio_cuotas )
#define  VALIDACION_CLIENTE_PARA_CHEQUE           GET_MEMORY_CHAR( __validacion_cliente_para_cheque )
#define  IMPRIMIR_Y_EN_Z                          GET_MEMORY_CHAR( __imprimir_y_en_z )
#define  CEREAR_TARJETA_DESPUES_DE_VALIDA         GET_MEMORY_CHAR( __cerear_tarjeta_despues_de_valida )
#define  PERMITE_Z_REMOTA                         GET_MEMORY_CHAR( __permite_z_remota )
#define  PERMITE_Z_LOCAL                          GET_MEMORY_CHAR( __permite_z_local )
#define  PERMITE_CIERRE_SIN_RED                   GET_MEMORY_CHAR( __permite_cierre_sin_red )
#define  CABECERA_INGRESO_EGRESO_CAJERO           GET_MEMORY_CHAR( __cabecera_ingreso_egreso_cajero )
#define  LINEAS_EN_BLANCO_EN_VOUCHER_TARJ         GET_MEMORY_CHAR( __lineas_en_blanco_en_voucher_tarj )
#define  SEGUNDOS_ESPERA_EN_INFORME               GET_MEMORY_CHAR( __segundos_espera_en_informe )
#define  PERMITE_MULTIPLICAR_ITEM_PESABLE         GET_MEMORY_CHAR( __permite_multiplicar_item_pesable )
#define  PERMITE_MULTIPLICAR_ITEM_PRECIO          GET_MEMORY_CHAR( __permite_multiplicar_item_precio )
#define  VALIDACION_COMPROMISO_200                GET_MEMORY_CHAR( __validacion_compromiso_200 )
#define  IMPRIMIR_CABECERA_AL_FINAL               GET_MEMORY_CHAR( __imprimir_cabecera_al_final )
#define  LINEAS_CORTE_RECEIPT                     GET_MEMORY_CHAR( __lineas_corte_receipt )
#define  GRABAR_ARCHIVO_DE_CIERRE_Y               GET_MEMORY_CHAR( __grabar_archivo_de_cierre_y )
#define  USAR_DIGITO_PARA_REIMPRIMIR_Z            GET_MEMORY_CHAR( __usar_digito_para_reimprimir_z )
#define  DESHABILITA_CORTE_PAPEL_EN_Z             GET_MEMORY_CHAR( __deshabilita_corte_papel_en_z )
#define  TIME_OUT_SERVER_NO_DISPONIBLE            GET_MEMORY_INT( __time_out_server_no_disponible )
#define  TIME_OUT_LLAMADO_NO_ATENDIDO             GET_MEMORY_INT( __time_out_llamado_no_atendido )
#define  TIME_OUT_SERVER_NO_RESPONDIO             GET_MEMORY_INT( __time_out_server_no_respondio )
#define  BUSCAR_BOLETIN                           GET_MEMORY_CHAR( __buscar_boletin )
#define  TIME_OUT_PING                            GET_MEMORY_INT( __time_out_ping )
#define  HABILITA_PING                            GET_MEMORY_CHAR( __habilita_ping )
#define  DIRECCION_IP_SERVIDOR                    GET_MEMORY_STR( __direccion_ip_servidor )
#define _DIRECCION_IP_SERVIDOR(x)                 GET_MEMORY_STR_CHAR( __direccion_ip_servidor, x )
#define  CANTIDAD_MINIMA_MODIFICACIONES           GET_MEMORY_INT( __cantidad_minima_modificaciones )
#define  CANTIDAD_MINIMA_ALTAS                    GET_MEMORY_INT( __cantidad_minima_altas )
#define  HABILITA_COPIA_TICKET_PRN                GET_MEMORY_CHAR( __habilita_copia_ticket_prn )
#define  HABILITA_COPIA_VOUCHER_PRN               GET_MEMORY_CHAR( __habilita_copia_voucher_prn )
#define  TICKS_POWER_DOWN                         GET_MEMORY_INT( __ticks_power_down )
#define  VERIFICAR_SINCRONIZACION_IMPRESO         GET_MEMORY_CHAR( __verificar_sincronizacion_impreso )
#define  INF_CAJERO_CONSOLIDADO_HABILITAD         GET_MEMORY_CHAR( __inf_cajero_consolidado_habilitad )
#define  SEGUNDOS_PARA_MODO_STAND_BY              GET_MEMORY_INT( __segundos_para_modo_stand_by )
#define  UTILIZA_DBROUTER                         GET_MEMORY_CHAR( __utiliza_dbrouter )
#define  FONDO_FIJO_MULTIPLES_MEDIOS              GET_MEMORY_CHAR( __fondo_fijo_multiples_medios )
#define  RETIRO_DE_FONDO_FIJO                     GET_MEMORY_CHAR( __retiro_de_fondo_fijo )
#define  COMP_SUPERVISOR_FONDO_FIJO               GET_MEMORY_CHAR( __comp_supervisor_fondo_fijo )
#define  BUSCAR_PROMOCION_POR_CODIGO_INTE         GET_MEMORY_CHAR( __buscar_promocion_por_codigo_inte )
#define  PUERTO_SERVIDOR                          GET_MEMORY_INT( __puerto_servidor )
#define  CONTROL_DE_IMPORTES_EN_RETIROS           GET_MEMORY_CHAR( __control_de_importes_en_retiros )
#define  PEDIR_IMPORTE_EN_RETIRO_DE_TICKE         GET_MEMORY_CHAR( __pedir_importe_en_retiro_de_ticke )
#define  PEDIR_IMPORTE_EN_RETIRO_DE_MUTUA         GET_MEMORY_CHAR( __pedir_importe_en_retiro_de_mutua )
#define  PEDIR_SUPERVISOR_PARA_RETIROS            GET_MEMORY_CHAR( __pedir_supervisor_para_retiros )
#define  SISTEMA_MANUAL_DE_TARJETA_DIFERI         GET_MEMORY_CHAR( __sistema_manual_de_tarjeta_diferi )
#define  DESCUENTOS_FIJOS_SELECTIVOS              GET_MEMORY_CHAR( __descuentos_fijos_selectivos )
#define  PEDIR_CAJON_EN_MULTIPLES_USUARIO         GET_MEMORY_CHAR( __pedir_cajon_en_multiples_usuario )
#define  PEDIR_IMPORTE_EN_RETIRO_DE_CHEQU         GET_MEMORY_CHAR( __pedir_importe_en_retiro_de_chequ )
#define  PEDIR_IMPORTE_EN_RETIRO_DE_TARJE         GET_MEMORY_CHAR( __pedir_importe_en_retiro_de_tarje )
#define  PEDIR_IMPORTE_EN_RETIRO_DE_CTACT         GET_MEMORY_CHAR( __pedir_importe_en_retiro_de_ctact )
#define  RECALCULAR_PESO_EN_PESABLES              GET_MEMORY_CHAR( __recalcular_peso_en_pesables )
#define  PROTOCOLO_AUTORIZACION_TARJETA           GET_MEMORY_CHAR( __protocolo_autorizacion_tarjeta )
#define  SERVER_LOGIN                             GET_MEMORY_STR( __server_login )
#define _SERVER_LOGIN(x)                          GET_MEMORY_STR_CHAR( __server_login, x )
#define  SERVER_PASSWORD                          GET_MEMORY_STR( __server_password )
#define _SERVER_PASSWORD(x)                       GET_MEMORY_STR_CHAR( __server_password, x )
#define  PORCENTAJE_MINIMO_DE_PAGO_CMR            GET_MEMORY_CHAR( __porcentaje_minimo_de_pago_cmr )
#define  CANTIDAD_DE_SERVICIOS_POR_COBRO          GET_MEMORY_CHAR( __cantidad_de_servicios_por_cobro )
#define  IMPRIMIR_PASILLO_EN_TICKET               GET_MEMORY_CHAR( __imprimir_pasillo_en_ticket )
#define  MAX_CANTIDAD_TICKET_EN_ESPERA            GET_MEMORY_CHAR( __max_cantidad_ticket_en_espera )
#define  PEDIR_DATOS_CLIENTE_TARJETA              GET_MEMORY_CHAR( __pedir_datos_cliente_tarjeta )
#define  INFORME_10_REDUCIDO                      GET_MEMORY_CHAR( __informe_10_reducido )
#define  DISCRIMINAR_SUBMEDIOS_CIERRE_CAJ         GET_MEMORY_CHAR( __discriminar_submedios_cierre_caj )
#define  PERMITIR_REINTENTO_DE_ARQUEO             GET_MEMORY_CHAR( __permitir_reintento_de_arqueo )
#define  EMITIR_INFORME_DIFERENCIAS_SUBME         GET_MEMORY_CHAR( __emitir_informe_diferencias_subme )
#define  SELECCIONA_COBROS_EN_REVALIDACIO         GET_MEMORY_CHAR( __selecciona_cobros_en_revalidacio )
#define  CALCULAR_DESC_AL_FINAL_TICKET            GET_MEMORY_CHAR( __calcular_desc_al_final_ticket )
#define  CONTROLAR_VENTAS_EN_SERVER               GET_MEMORY_CHAR( __controlar_ventas_en_server )
#define  EMITIR_INFORME_10_EN_Y                   GET_MEMORY_CHAR( __emitir_informe_10_en_y )
#define  PEDIR_SUPERVISOR_BIEN_DE_USO             GET_MEMORY_CHAR( __pedir_supervisor_bien_de_uso )
#define  SELEC_SUBMEDIOS_CIERRE_POR_NUMER         GET_MEMORY_CHAR( __selec_submedios_cierre_por_numer )
#define  PEDIR_SUPERVISOR_REIMP_VALID_MED         GET_MEMORY_CHAR( __pedir_supervisor_reimp_valid_med )
#define  PERMITIR_USO_DE_MONOTRIBUTO              GET_MEMORY_CHAR( __permitir_uso_de_monotributo )
#define  PERMITIR_ABORTAR_CIERRE                  GET_MEMORY_CHAR( __permitir_abortar_cierre )
#define  BUFFER_TICKET                            GET_MEMORY_UNSIGNED( __buffer_ticket )
#define  TKT_CONFIGURABLE                         GET_MEMORY_CHAR( __tkt_configurable )
#define  TIPO_REDONDEO_VUELTO                     GET_MEMORY_CHAR( __tipo_redondeo_vuelto )
#define  CONTROLAR_VENTAS_EN_HISTORICOS           GET_MEMORY_CHAR( __controlar_ventas_en_historicos )
#define  PERMITE_Z_SIN_RED                        GET_MEMORY_CHAR( __permite_z_sin_red )
#define  PERMITIR_UNIPAGO_OFFLINE                 GET_MEMORY_CHAR( __permitir_unipago_offline )
#define  SUPERVISOR_ANULAR_COBRO_UNIPAGO          GET_MEMORY_CHAR( __supervisor_anular_cobro_unipago )
#define  NUMERO_COMERCIO_UNIPAGO                  GET_MEMORY_STR( __numero_comercio_unipago )
#define _NUMERO_COMERCIO_UNIPAGO(x)               GET_MEMORY_STR_CHAR( __numero_comercio_unipago, x )
#define  USAR_COBRO_TARJETA                       GET_MEMORY_CHAR( __usar_cobro_tarjeta )
#define  HABILITA_COPIA_COBRO_PRN                 GET_MEMORY_CHAR( __habilita_copia_cobro_prn )
#define  VARIAS_TARJETAS_POR_PAGO                 GET_MEMORY_CHAR( __varias_tarjetas_por_pago )
#define  SOLICITAR_NROSUCURSAL_Y_TICKET           GET_MEMORY_CHAR( __solicitar_nrosucursal_y_ticket )
#define  PREFIJO_COD_DEV_ENVASES_EVENTO           GET_MEMORY_STR( __prefijo_cod_dev_envases_evento )
#define _PREFIJO_COD_DEV_ENVASES_EVENTO(x)        GET_MEMORY_STR_CHAR( __prefijo_cod_dev_envases_evento, x )
#define  HABILITA_TRAVELPASS                      GET_MEMORY_CHAR( __habilita_travelpass )
#define  ELIMINAR_DIGITO_VERIFICADOR              GET_MEMORY_CHAR( __eliminar_digito_verificador )
#define  PREFIJO_LECTURA_POR_SCANNER              GET_MEMORY_STR( __prefijo_lectura_por_scanner )
#define _PREFIJO_LECTURA_POR_SCANNER(x)           GET_MEMORY_STR_CHAR( __prefijo_lectura_por_scanner, x )
#define  LONGITUD_TARJETA_AFINIDAD                GET_MEMORY_CHAR( __longitud_tarjeta_afinidad )
#define  CIERRE_DE_LOTE_POR_NODO                  GET_MEMORY_CHAR( __cierre_de_lote_por_nodo )
#define  CONTROLAR_TASAS_DE_IVA                   GET_MEMORY_CHAR( __controlar_tasas_de_iva )
#define  PEDIR_MARCA_ESPECIAL_EN_T_MANUAL         GET_MEMORY_CHAR( __pedir_marca_especial_en_t_manual )
#define  CONTROLAR_ENTRADA_DE_CAJEROS             GET_MEMORY_CHAR( __controlar_entrada_de_cajeros )
#define  IMPRIME_SUBTOTAL_AL_PASAR_A_TOTA         GET_MEMORY_CHAR( __imprime_subtotal_al_pasar_a_tota )
#define  FORMATO_TICKET_FISCAL                    GET_MEMORY_CHAR( __formato_ticket_fiscal )
#define  TIME_OUT_REINTENTOS_EN_PARALELO          GET_MEMORY_CHAR( __time_out_reintentos_en_paralelo )
#define  CONTROLAR_VUELTO_MAXIMO_POR_MEDI         GET_MEMORY_CHAR( __controlar_vuelto_maximo_por_medi )
#define  MODALIDAD_TICKET_EN_ESPERA               GET_MEMORY_CHAR( __modalidad_ticket_en_espera )
#define  PEDIR_SUPERVISOR_TRAE_TICKET_ESP         GET_MEMORY_CHAR( __pedir_supervisor_trae_ticket_esp )
#define  IMPRIME_PRECIO_EN_MODO_INVENTARI         GET_MEMORY_CHAR( __imprime_precio_en_modo_inventari )
#define  PEDIR_SUPERVISOR_PARA_CAMBIO_MED         GET_MEMORY_CHAR( __pedir_supervisor_para_cambio_med )
#define  CINTA_TESTIGO_ELECTRONICA                GET_MEMORY_CHAR( __cinta_testigo_electronica )
#define  MOSTRAR_DATOS_CLIENTE_AFINIDAD           GET_MEMORY_CHAR( __mostrar_datos_cliente_afinidad )
#define  UTILIZAR_PRECIO_AFINIDAD                 GET_MEMORY_CHAR( __utilizar_precio_afinidad )
#define  PEDIR_MOTIVO_EN_DESCUENTOS               GET_MEMORY_CHAR( __pedir_motivo_en_descuentos )
#define  PEDIR_MOTIVO_EN_ANULACIONES              GET_MEMORY_CHAR( __pedir_motivo_en_anulaciones )
#define  PEDIR_SUPERVISOR_VUELTO_MONEDA_E         GET_MEMORY_CHAR( __pedir_supervisor_vuelto_moneda_e )
#define  MEDIOS_DE_PAGO_ONLINE                    GET_MEMORY_CHAR( __medios_de_pago_online )
#define  IMPRESION_OPCIONAL_DEL_CHEQUE            GET_MEMORY_CHAR( __impresion_opcional_del_cheque )
#define  REFERENCIAR_COMP_ORIGINAL_EN_CAM         GET_MEMORY_CHAR( __referenciar_comp_original_en_cam )
#define  IMPRIMIR_COPIA_TICKET_DEVOLUCION         GET_MEMORY_CHAR( __imprimir_copia_ticket_devolucion )
#define  IMPRIMIR_COPIA_TICKET_CAMBIO             GET_MEMORY_CHAR( __imprimir_copia_ticket_cambio )
#define  PEDIR_DATOS_EN_DEVOLUCIONES              GET_MEMORY_CHAR( __pedir_datos_en_devoluciones )
#define  PEDIR_IMPORTE_SIN_IVA_EN_CAMBIO          GET_MEMORY_CHAR( __pedir_importe_sin_iva_en_cambio )
#define  VALIDACION_TELEFONICA_HABILITADA         GET_MEMORY_CHAR( __validacion_telefonica_habilitada )
#define  IP_HOST_VALIDACION_TELEFONICA            GET_MEMORY_STR( __ip_host_validacion_telefonica )
#define _IP_HOST_VALIDACION_TELEFONICA(x)         GET_MEMORY_STR_CHAR( __ip_host_validacion_telefonica, x )
#define  IDENTIFICACION_CLIENTES_HABILITA         GET_MEMORY_CHAR( __identificacion_clientes_habilita )
#define  PEDIR_NUMERO_DE_CHEQUE                   GET_MEMORY_CHAR( __pedir_numero_de_cheque )
#define  REGISTRAR_CHEQUES_EN_ARCHIVO             GET_MEMORY_CHAR( __registrar_cheques_en_archivo )
#define  DONACION_DE_VUELTO                       GET_MEMORY_CHAR( __donacion_de_vuelto )
#define  CONFIRMAR_DONACION                       GET_MEMORY_CHAR( __confirmar_donacion )
#define  PUERTO_HOST_VALIDACION_TELEFONIC         GET_MEMORY_INT( __puerto_host_validacion_telefonic )
#define  MODALIDAD_CANAL_TARJETAS                 GET_MEMORY_CHAR( __modalidad_canal_tarjetas )
#define  PEDIR_SUPERVISOR_TRANSFERENCIAS          GET_MEMORY_CHAR( __pedir_supervisor_transferencias )
#define  PEDIR_MOTIVO_EN_DEVOLUCIONES             GET_MEMORY_CHAR( __pedir_motivo_en_devoluciones )
#define  LIMITE_MAXIMO_NUMERO_DE_EVENTO           GET_MEMORY_LONG( __limite_maximo_numero_de_evento )
#define  PEDIR_CLAVE_EN_LEGAJO_EMPLEADO           GET_MEMORY_CHAR( __pedir_clave_en_legajo_empleado )
#define  CODIGO_DE_PERFIL_EMPLEADOS               GET_MEMORY_INT( __codigo_de_perfil_empleados )
#define  MOSTRAR_PRECIO_SIN_IVA_EN_VENTAS         GET_MEMORY_CHAR( __mostrar_precio_sin_iva_en_ventas )
#define  MOSTRAR_MENSAJE_EN_COBROS_ONLINE         GET_MEMORY_CHAR( __mostrar_mensaje_en_cobros_online )
#define  INCREMENTAR_NRO_EVENTO_CONFIGURA         GET_MEMORY_CHAR( __incrementar_nro_evento_configura )
#define  IMPRIMIR_EN_JOURNAL                      GET_MEMORY_CHAR( __imprimir_en_journal )
#define  CODIGO_VALIDACION_PARA_DONACION          GET_MEMORY_CHAR( __codigo_validacion_para_donacion )
#define  INCLUIR_DONACIONES_EN_DETALLE_00         GET_MEMORY_CHAR( __incluir_donaciones_en_detalle_00 )
#define  PASAR_CHEQUE_POR_LECTOR                  GET_MEMORY_CHAR( __pasar_cheque_por_lector )
#define  VERIFICAR_CHEQUE_MANUAL_EN_BOLET         GET_MEMORY_CHAR( __verificar_cheque_manual_en_bolet )
#define  DIAS_PERMITIDOS_PARA_CHEQUE_A_FE         GET_MEMORY_CHAR( __dias_permitidos_para_cheque_a_fe )
#define  LONGITUD_NRO_CLIENTE_AFINIDAD            GET_MEMORY_CHAR( __longitud_nro_cliente_afinidad )
#define  CONTROLAR_CLIENTE_AFINIDAD               GET_MEMORY_CHAR( __controlar_cliente_afinidad )
#define  DESPLAZAM_DESDE_ID_CLIENTE_AFINI         GET_MEMORY_INT( __desplazam_desde_id_cliente_afini )
#define  USAR_CLI_AFI_PARA_CLIENTES_AFINI         GET_MEMORY_CHAR( __usar_cli_afi_para_clientes_afini )
#define  CONTROLAR_CUPO_EN_CHEQUES                GET_MEMORY_CHAR( __controlar_cupo_en_cheques )
#define  UTILIZA_AUTORIZADOR_CHEQUES              GET_MEMORY_CHAR( __utiliza_autorizador_cheques )
#define  NUMERO_IP_AUTORIZADOR_CHEQUES            GET_MEMORY_STR( __numero_ip_autorizador_cheques )
#define _NUMERO_IP_AUTORIZADOR_CHEQUES(x)         GET_MEMORY_STR_CHAR( __numero_ip_autorizador_cheques, x )
#define  TIME_OUT_SERVER_AUTORIZADOR_CHEQ         GET_MEMORY_INT( __time_out_server_autorizador_cheq )
#define  PIDE_SUPERVISOR_EN_CHEQUE_RECHAZ         GET_MEMORY_CHAR( __pide_supervisor_en_cheque_rechaz )
#define  PEDIR_SUPERVISOR_AUTORIZADOR_CHE         GET_MEMORY_CHAR( __pedir_supervisor_autorizador_che )
#define  HABILITA_IDENTIFICACION_BILLETES         GET_MEMORY_CHAR( __habilita_identificacion_billetes )
#define  PREFIJO_2_DE_BALANZA                     GET_MEMORY_STR( __prefijo_2_de_balanza )
#define _PREFIJO_2_DE_BALANZA(x)                  GET_MEMORY_STR_CHAR( __prefijo_2_de_balanza, x )
#define  PREFIJO_ARTICULOS_POR_UNIDAD             GET_MEMORY_STR( __prefijo_articulos_por_unidad )
#define _PREFIJO_ARTICULOS_POR_UNIDAD(x)          GET_MEMORY_STR_CHAR( __prefijo_articulos_por_unidad, x )
#define  ID_TRANSACCION                           GET_MEMORY_STR( __id_transaccion )
#define _ID_TRANSACCION(x)                        GET_MEMORY_STR_CHAR( __id_transaccion, x )
#define  PEDIR_PRECIO_SIN_IVA                     GET_MEMORY_CHAR( __pedir_precio_sin_iva )
#define  PEDIR_SUPERVISOR_ARTICULO_IMPORT         GET_MEMORY_CHAR( __pedir_supervisor_articulo_import )
#define  PEDIR_DATOS_COMPRADOR_EN_CHEQUES         GET_MEMORY_CHAR( __pedir_datos_comprador_en_cheques )
#define  CONTROL_DIGITO_VERIFICADOR_RUT           GET_MEMORY_CHAR( __control_digito_verificador_rut )
#define  SUPERVISOR_CHEQUE_CLIENTE_INEXIS         GET_MEMORY_CHAR( __supervisor_cheque_cliente_inexis )
#define  MODALIDAD_VALES_INSTITUCIONALES          GET_MEMORY_CHAR( __modalidad_vales_institucionales )
#define  MODALIDAD_TOMRA                          GET_MEMORY_CHAR( __modalidad_tomra )
#define  CODIGO_VALIDACION_TICKET_TOMRA           GET_MEMORY_INT( __codigo_validacion_ticket_tomra )
#define  OFFSET_NUMERO_TICKET_TOMRA               GET_MEMORY_INT( __offset_numero_ticket_tomra )
#define  PEDIR_NUMERO_DE_VALE                     GET_MEMORY_CHAR( __pedir_numero_de_vale )
#define  MOSTRAR_MENSAJE_DE_ENVASES               GET_MEMORY_CHAR( __mostrar_mensaje_de_envases )
#define  PEDIR_SUPERVISOR_DEVOLUCION_ENVA         GET_MEMORY_CHAR( __pedir_supervisor_devolucion_enva )
#define  CODIGO_VALIDACION_FACTURA_REMOTA         GET_MEMORY_CHAR( __codigo_validacion_factura_remota )
#define  BUSQUEDA_DE_ARTICULOS                    GET_MEMORY_CHAR( __busqueda_de_articulos )
#define  CONTROLA_IMPORTE_MEDIOS                  GET_MEMORY_CHAR( __controla_importe_medios )
#define  PUERTO_CANAL_TCP_IP                      GET_MEMORY_INT( __puerto_canal_tcp_ip )
#define  MODELO_TEF                               GET_MEMORY_CHAR( __modelo_tef )
#define  GENERAR_REVERSOS                         GET_MEMORY_CHAR( __generar_reversos )
#define  GENERAR_CIERRE_DE_LOTE                   GET_MEMORY_CHAR( __generar_cierre_de_lote )
#define  CODIGO_SOLICITUD_DATOS_ADIC_CLTE         GET_MEMORY_INT( __codigo_solicitud_datos_adic_clte )
#define  T_ESPERA                                 GET_MEMORY_INT( __t_espera )
#define  DESCUENTOS_POR_CUERPO                    GET_MEMORY_CHAR( __descuentos_por_cuerpo )
#define  MOSTRAR_MENU_DE_CUOTAS_POR_MEDIO         GET_MEMORY_CHAR( __mostrar_menu_de_cuotas_por_medio )
#define  REVERSAR_ACUMULADORES_EN_DEVOLUC         GET_MEMORY_CHAR( __reversar_acumuladores_en_devoluc )
#define  SCANNER_HABILITADO_POR_TECLADO           GET_MEMORY_CHAR( __scanner_habilitado_por_teclado )
#define  PREFIJO_SCANNER                          GET_MEMORY_CHAR( __prefijo_scanner )
#define  SUFIJO_SCANNER                           GET_MEMORY_CHAR( __sufijo_scanner )
#define  CONTROL_NOTA_DE_CREDITO                  GET_MEMORY_CHAR( __control_nota_de_credito )
#define  MULTIPLO_PRECIO_MAS_BAJO                 GET_MEMORY_INT( __multiplo_precio_mas_bajo )
#define  PEDIR_CAJA_Y_SECUENCIA                   GET_MEMORY_CHAR( __pedir_caja_y_secuencia )
#define  PRESENTACIONES_EN_DUN14                  GET_MEMORY_CHAR( __presentaciones_en_dun14 )
#define  MENU_PRESENTACIONES_INCONDICIONA         GET_MEMORY_CHAR( __menu_presentaciones_incondiciona )
#define  PEDIR_SUBMEDIO_EN_RETIROS                GET_MEMORY_CHAR( __pedir_submedio_en_retiros )
#define  PEDIR_SUBMEDIO_EN_FONDO_FIJO             GET_MEMORY_CHAR( __pedir_submedio_en_fondo_fijo )
#define  PEDIR_SUBMEDIO_EN_CAMBIO_DE_MEDI         GET_MEMORY_CHAR( __pedir_submedio_en_cambio_de_medi )
#define  GENERAR_ON_LINE_EN_CAMBIO_DE_MED         GET_MEMORY_CHAR( __generar_on_line_en_cambio_de_med )
#define  VALIDAR_MEDIOS_EN_CAMBIOS_DE_MED         GET_MEMORY_CHAR( __validar_medios_en_cambios_de_med )
#define  VALIDACION_TICKET_EN_ESPERA              GET_MEMORY_INT( __validacion_ticket_en_espera )
#define  RESTRINGIR_CAMBIO_DE_VALORES             GET_MEMORY_CHAR( __restringir_cambio_de_valores )
#define  SINCRONIZAR_NUMERO_TICKET_FISCAL         GET_MEMORY_CHAR( __sincronizar_numero_ticket_fiscal )
#define  VERIFICAR_MINIMO_Y_MAXIMO_EN_CAJ         GET_MEMORY_CHAR( __verificar_minimo_y_maximo_en_caj )
#define  RECORDAR_ARTICULOS_EN_PROMOCION          GET_MEMORY_CHAR( __recordar_articulos_en_promocion )
#define  MENU_CUOTAS_POR_GRUPO                    GET_MEMORY_CHAR( __menu_cuotas_por_grupo )
#define  ANULACION_DE_COBRO                       GET_MEMORY_CHAR( __anulacion_de_cobro )
#define  VERIFICAR_PINPAD_ONLINE                  GET_MEMORY_CHAR( __verificar_pinpad_online )
#define  VERIFICAR_DIA_OPERATIVO                  GET_MEMORY_CHAR( __verificar_dia_operativo )
#define  DIRECCION_IP_SAFEKEY                     GET_MEMORY_STR( __direccion_ip_safekey )
#define _DIRECCION_IP_SAFEKEY(x)                  GET_MEMORY_STR_CHAR( __direccion_ip_safekey, x )
#define  IMPRIMIR_CABECERA_EN_FONDO_FIJO          GET_MEMORY_CHAR( __imprimir_cabecera_en_fondo_fijo )
#define  INDICE_COMIENZO_CLIENTE_EN_BANDA         GET_MEMORY_INT( __indice_comienzo_cliente_en_banda )
#define  INDICE_FIN_NRO_CLIENTE_EN_BANDA          GET_MEMORY_INT( __indice_fin_nro_cliente_en_banda )
#define  USAR_PUNTO_EN_PANTALLA                   GET_MEMORY_CHAR( __usar_punto_en_pantalla )
#define  MOSTRAR_CUPO_USADO_CLIENTE               GET_MEMORY_CHAR( __mostrar_cupo_usado_cliente )
#define  IMPRESORA_SSI                            GET_MEMORY_CHAR( __impresora_ssi )
#define  UTILIZAR_SSI                             GET_MEMORY_CHAR( __utilizar_ssi )
#define  CANTIDAD_DE_DIGITOS_EN_NRO_LEGAJ         GET_MEMORY_INT( __cantidad_de_digitos_en_nro_legaj )
#define  VALIDAR_CLIENTE_CTA_CTE                  GET_MEMORY_CHAR( __validar_cliente_cta_cte )
#define  ELEGIR_CLIENTE_ALFABETICO_EN_PAG         GET_MEMORY_CHAR( __elegir_cliente_alfabetico_en_pag )
#define  NRO_VENDEDOR_POR_DEFECTO_DEVOLUC         GET_MEMORY_STR( __nro_vendedor_por_defecto_devoluc )
#define _NRO_VENDEDOR_POR_DEFECTO_DEVOLUC(x)      GET_MEMORY_STR_CHAR( __nro_vendedor_por_defecto_devoluc, x )
#define  NRO_VENDEDOR_POR_DEFECTO_VENTA           GET_MEMORY_STR( __nro_vendedor_por_defecto_venta )
#define _NRO_VENDEDOR_POR_DEFECTO_VENTA(x)        GET_MEMORY_STR_CHAR( __nro_vendedor_por_defecto_venta, x )
#define  DIGITOS_MAXIMO_NRO_VENDEDOR              GET_MEMORY_CHAR( __digitos_maximo_nro_vendedor )
#define  CONSULTA_PADRON_DE_VENDEDORES            GET_MEMORY_CHAR( __consulta_padron_de_vendedores )
#define  USAR_VENTA_ASISTIDA                      GET_MEMORY_CHAR( __usar_venta_asistida )
#define  VALIDACION_VENTA_ASISTIDA                GET_MEMORY_CHAR( __validacion_venta_asistida )
#define  COPIAS_VALIDACION_VENTA_ASISTIDA         GET_MEMORY_CHAR( __copias_validacion_venta_asistida )
#define  APLICAR_DONACION_EN_VENTAS               GET_MEMORY_CHAR( __aplicar_donacion_en_ventas )
#define  BORRAR_MUTUAL_AL_SALIR_DE_PAGOS          GET_MEMORY_CHAR( __borrar_mutual_al_salir_de_pagos )
#define  PEDIR_NUMERO_DE_ORDEN                    GET_MEMORY_CHAR( __pedir_numero_de_orden )
#define  MOSTRAR_MUTUAL_ACTIVA_EN_VENTAS          GET_MEMORY_CHAR( __mostrar_mutual_activa_en_ventas )
#define  TIEMPO_ESPERA_EN_INICIALIZACION          GET_MEMORY_INT( __tiempo_espera_en_inicializacion )
#define  FACTOR_DE_SINCRONIZACION_PRINTER         GET_MEMORY_INT( __factor_de_sincronizacion_printer )
#define  UTILIZA_MODULO_PROMOCIONES               GET_MEMORY_CHAR( __utiliza_modulo_promociones )
#define  UTILIZAR_PROMOCION_POR_ITEM              GET_MEMORY_CHAR( __utilizar_promocion_por_item )
#define  MEJORA_PERFORMANCE_TIPO_A                GET_MEMORY_CHAR( __mejora_performance_tipo_a )
#define  GRABAR_DESCUENTO_EN_EVENTOS              GET_MEMORY_CHAR( __grabar_descuento_en_eventos )
#define  HANDSHAKE                                GET_MEMORY_CHAR( __handshake )
#define  IMPRESORA_DATA_BITS                      GET_MEMORY_CHAR( __impresora_data_bits )
#define  IMPRESORA_STOP_BITS                      GET_MEMORY_CHAR( __impresora_stop_bits )
#define  IMPRESORA_PARITY                         GET_MEMORY_CHAR( __impresora_parity )
#define  IMPRESORA_BAUDIOS                        GET_MEMORY_CHAR( __impresora_baudios )
#define  IMPRIME_TOTALES_EN_MODO_INVENTAR         GET_MEMORY_CHAR( __imprime_totales_en_modo_inventar )
#define  IMPRIMIR_INFORMES_EN_EL_CIERRE           GET_MEMORY_CHAR( __imprimir_informes_en_el_cierre )
#define  INTENTOS_CONSULTA_SERVIDOR               GET_MEMORY_INT( __intentos_consulta_servidor )
#define  TIEMPO_ENTRE_REINTENTOS                  GET_MEMORY_INT( __tiempo_entre_reintentos )
#define  IMPRIMIR_ITEM_RECARGO_EN_FISCAL          GET_MEMORY_CHAR( __imprimir_item_recargo_en_fiscal )
#define  SUGERIR_IMPORTE_NO_NULO_SIEMPRE          GET_MEMORY_CHAR( __sugerir_importe_no_nulo_siempre )
#define  UTILIZAR_DATAFONO                        GET_MEMORY_CHAR( __utilizar_datafono )
#define  PUERTO_DATAFONO                          GET_MEMORY_CHAR( __puerto_datafono )
#define  DATAFONO_BAUDIOS                         GET_MEMORY_CHAR( __datafono_baudios )
#define  DATAFONO_HANDSHAKE                       GET_MEMORY_CHAR( __datafono_handshake )
#define  DATAFONO_DATA_BITS                       GET_MEMORY_CHAR( __datafono_data_bits )
#define  DATAFONO_STOP_BIT                        GET_MEMORY_CHAR( __datafono_stop_bit )
#define  DATAFONO_PARITY                          GET_MEMORY_CHAR( __datafono_parity )
#define  TECLADO_POSIFLEX                         GET_MEMORY_CHAR( __teclado_posiflex )
#define  ID_AFINIDAD_OPCIONAL                     GET_MEMORY_STR( __id_afinidad_opcional )
#define _ID_AFINIDAD_OPCIONAL(x)                  GET_MEMORY_STR_CHAR( __id_afinidad_opcional, x )
#define  EDITAR_DATOS_CLIENTE_AFINIDAD            GET_MEMORY_CHAR( __editar_datos_cliente_afinidad )
#define  MOSTRAR_TICKET_EN_PANTALLA               GET_MEMORY_CHAR( __mostrar_ticket_en_pantalla )
#define  PUERTO_SERVIDOR_DBROUTER                 GET_MEMORY_INT( __puerto_servidor_dbrouter )
#define  CLAVE_SUPERVISOR                         GET_MEMORY_INT( __clave_supervisor )
#define  TARJETA_SUPERVISOR_EN_CIERRE_LOT         GET_MEMORY_CHAR( __tarjeta_supervisor_en_cierre_lot )
#define  DIAS_EN_IMPORTACION_EN_DEVOLUCIO         GET_MEMORY_CHAR( __dias_en_importacion_en_devolucio )
#define  CONTROLAR_CLIENTE_EN_CTA_CTE             GET_MEMORY_CHAR( __controlar_cliente_en_cta_cte )
#define  TOMAR_PERFIL_DESDE_CLIENTE               GET_MEMORY_CHAR( __tomar_perfil_desde_cliente )
#define  FACT_INDEP_DEL_TIPO_DE_CLIENTE           GET_MEMORY_CHAR( __fact_indep_del_tipo_de_cliente )
#define  EXCLUIR_ART_AFECTADOS_POR_DESC           GET_MEMORY_CHAR( __excluir_art_afectados_por_desc )
#define  BUSQUEDA_CLIENTE                         GET_MEMORY_CHAR( __busqueda_cliente )
#define  REFERENCIAR_POR_SECUENCIA                GET_MEMORY_CHAR( __referenciar_por_secuencia )
#define  UTILIZAR_DOSIFICACIONES                  GET_MEMORY_CHAR( __utilizar_dosificaciones )
#define  MOSTRAR_DATOS_CLIENTE_BOX                GET_MEMORY_CHAR( __mostrar_datos_cliente_box )
#define  INFORME_X_Y_NO_CONFIGURABLE              GET_MEMORY_CHAR( __informe_x_y_no_configurable )
#define  COMANDOS_MODIFICADOS                     GET_MEMORY_CHAR( __comandos_modificados )
#define  COD_FINANCIACION_POR_DEFECTO             GET_MEMORY_INT( __cod_financiacion_por_defecto )
#define  CONTROLAR_CUPO_EN_VENTAS                 GET_MEMORY_CHAR( __controlar_cupo_en_ventas )
#define  TOMAR_SCRIPT_AL_CIERRE                   GET_MEMORY_CHAR( __tomar_script_al_cierre )
#define  TIME_OUT_ESPERA_SCRIPT                   GET_MEMORY_INT( __time_out_espera_script )
#define  PRECIO_UNITARIO_EN_DEVOLUCIONES          GET_MEMORY_CHAR( __precio_unitario_en_devoluciones )
#define  RESET_CONTADOR_RETIRO_EN_X               GET_MEMORY_CHAR( __reset_contador_retiro_en_x )
#define  ESPACIO_HD_LIBRE_ADVERTENCIA             GET_MEMORY_UNSIGNED( __espacio_hd_libre_advertencia )
#define  ESPACIO_HD_LIBRE_MINIMO                  GET_MEMORY_UNSIGNED( __espacio_hd_libre_minimo )
#define  ANULAR_ITEM_ESTRICTO                     GET_MEMORY_CHAR( __anular_item_estricto )
#define  PEDIR_SUPERVISOR_ALTA_CLIENTE            GET_MEMORY_CHAR( __pedir_supervisor_alta_cliente )
#define  CONTROLAR_SERIAL_PRINTER                 GET_MEMORY_CHAR( __controlar_serial_printer )
#define  MODO_NEGOCIO                             GET_MEMORY_INT( __modo_negocio )
#define  MODO_CHEQUE_COMO_UN_MEDIO                GET_MEMORY_CHAR( __modo_cheque_como_un_medio )
#define  TIMEOUT_JGATEWAY                         GET_MEMORY_CHAR( __timeout_jgateway )
#define  PORT_JGATEWAY                            GET_MEMORY_INT( __port_jgateway )
#define  DIRECCION_IP_JGATEWAY                    GET_MEMORY_STR( __direccion_ip_jgateway )
#define _DIRECCION_IP_JGATEWAY(x)                 GET_MEMORY_STR_CHAR( __direccion_ip_jgateway, x )
#define  GENERAR_COD_BARRA_INVERTIDO              GET_MEMORY_CHAR( __generar_cod_barra_invertido )
#define  PRECALCULAR_IMPUESTOS                    GET_MEMORY_CHAR( __precalcular_impuestos )
#define  IMPRIMIR_COPIA_TF                        GET_MEMORY_CHAR( __imprimir_copia_tf )
#define  IMPRIMIR_CABECERA_PRINCIPIO              GET_MEMORY_CHAR( __imprimir_cabecera_principio )
#define  HABILITAR_SCANNER_AL_INICIO              GET_MEMORY_CHAR( __habilitar_scanner_al_inicio )
#define  ASIGNAR_CUOTA_UNICA                      GET_MEMORY_CHAR( __asignar_cuota_unica )
#define  EMITIR_VOUCHER_COPIA_CASH_ADVANC         GET_MEMORY_CHAR( __emitir_voucher_copia_cash_advanc )
#define  VALIDAR_CIERRE_CAJERO                    GET_MEMORY_INT( __validar_cierre_cajero )
#define  CANTIDAD_EVENTOS_ENVIAR_OFFLINE          GET_MEMORY_INT( __cantidad_eventos_enviar_offline )
#define  FONDO_FIJO_AUTOMATICO                    GET_MEMORY_CHAR( __fondo_fijo_automatico )
#define  MODO_IDENTIFICACION_GERENTE              GET_MEMORY_CHAR( __modo_identificacion_gerente )
#define  TIPO_CLAVE_GERENTE                       GET_MEMORY_CHAR( __tipo_clave_gerente )
#define  REPETIR_MEDIO                            GET_MEMORY_CHAR( __repetir_medio )
#define  ALTA_DE_CLIENTE_HABILITADA               GET_MEMORY_CHAR( __alta_de_cliente_habilitada )
#define  PIDE_SUPERVISOR_CAJERO_FLOTANTE          GET_MEMORY_CHAR( __pide_supervisor_cajero_flotante )
#define  IMPRIMIR_VALIDACION_1_TECLA              GET_MEMORY_CHAR( __imprimir_validacion_1_tecla )
#define  VALIDACION_A_IMPRIMIR_TECLA              GET_MEMORY_INT( __validacion_a_imprimir_tecla )
#define  ALTA_DE_CLIENTES_PERMITIDA               GET_MEMORY_CHAR( __alta_de_clientes_permitida )
#define  RESTRINGIR_VOLVER_DE_PAGOS               GET_MEMORY_CHAR( __restringir_volver_de_pagos )
#define  AUTORIZACION_TARJ_EN_NOTAS_CREDI         GET_MEMORY_CHAR( __autorizacion_tarj_en_notas_credi )
#define  LIMITAR_NOTAS_DE_CREDITO_CON_TAR         GET_MEMORY_CHAR( __limitar_notas_de_credito_con_tar )
#define  PEDIR_MOTIVO_ANULACION_TICKET            GET_MEMORY_CHAR( __pedir_motivo_anulacion_ticket )
#define  PEDIR_MOTIVO_EN_NOTA_DE_CREDITO          GET_MEMORY_CHAR( __pedir_motivo_en_nota_de_credito )
#define  PERMITE_MEDIO_DESHAB_EN_DEV_TOT          GET_MEMORY_CHAR( __permite_medio_deshab_en_dev_tot )
#define  AUTORIZAR_CAMBIO_MEDIO_EN_DEVOL          GET_MEMORY_CHAR( __autorizar_cambio_medio_en_devol )
#define  AUTORIZAR_RETENCIONES                    GET_MEMORY_CHAR( __autorizar_retenciones )
#define  ORDEN_VOUCHER_DE_TARJETA                 GET_MEMORY_CHAR( __orden_voucher_de_tarjeta )
#define  IMPORTACION_CON_SELECCION_ART            GET_MEMORY_CHAR( __importacion_con_seleccion_art )
#define  ENCRIPTAR_DATOS_DE_TARJETAS_PRIV         GET_MEMORY_CHAR( __encriptar_datos_de_tarjetas_priv )
#define  ENCRIPTA_DATOS_DE_TRANSAC_TARJET         GET_MEMORY_CHAR( __encripta_datos_de_transac_tarjet )
#define  PEDIR_SUP_CIERRE_CON_TICKET_ESPE         GET_MEMORY_CHAR( __pedir_sup_cierre_con_ticket_espe )
#define  PERMITIR_CIERRE_CON_TICKET_ESPER         GET_MEMORY_CHAR( __permitir_cierre_con_ticket_esper )
#define  PERMITE_AGRUPAR_PESABLES_EN_COMP         GET_MEMORY_CHAR( __permite_agrupar_pesables_en_comp )
#define  PEDIR_SUPERVISOR_VERIFICAR_TARJE         GET_MEMORY_CHAR( __pedir_supervisor_verificar_tarje )
#define  VOUCHER_TARJETA_FISCAL                   GET_MEMORY_CHAR( __voucher_tarjeta_fiscal )
#define  DIAS_EN_IMPORTACION_A_TF_O_FACTU         GET_MEMORY_CHAR( __dias_en_importacion_a_tf_o_factu )
#define  PERMITE_MODIFICAR_TICKET_IMPORTA         GET_MEMORY_CHAR( __permite_modificar_ticket_importa )
#define  SINCRONIZAR_HORA_CONTROLADOR_FIS         GET_MEMORY_CHAR( __sincronizar_hora_controlador_fis )
#define  PROPONER_DIA_OPERATIVO_AL_VENCID         GET_MEMORY_CHAR( __proponer_dia_operativo_al_vencid )
#define  SUPERVISOR_EN_CONSULTA_DE_ARTICU         GET_MEMORY_CHAR( __supervisor_en_consulta_de_articu )
#define  CALCULAR_PERCEPCION_TACIS                GET_MEMORY_CHAR( __calcular_percepcion_tacis )
#define  PEDIR_NRO_CUENTA_CHEQUE                  GET_MEMORY_CHAR( __pedir_nro_cuenta_cheque )
#define  PEDIR_PLAZA                              GET_MEMORY_CHAR( __pedir_plaza )
#define  LIMITE_DE_LINEAS_DE_TICKETS              GET_MEMORY_UNSIGNED( __limite_de_lineas_de_tickets )

/*------------------- Variables Internas Inicializadas -------------------*/

/*----------------- Variables Internas no Inicializadas -----------------*/
#define _TICKET_TIPO(x)                          _GET_MEMORY_CHAR( __ticket_tipo, x )
#define _TICKET_CANTIDAD(x)                      _GET_MEMORY_FLOAT( __ticket_cantidad, x )
#define _TICKET_PRESENTACION(x)                  _GET_MEMORY_INT( __ticket_presentacion, x )
#define ___TICKET_DEPTO(x)                       _GET_MEMORY_CHAR( ____ticket_depto, x )
#define _TICKET_PRECIO(x)                        _GET_MEMORY_DOUBLE( __ticket_precio, x )
#define _TICKET_COD_ESPECIAL(x)                  _GET_MEMORY_LONG( __ticket_cod_especial, x )
#define _TICKET_IMP_INT(x)                       _GET_MEMORY_DOUBLE( __ticket_imp_int, x )
#define _TICKET_COSTO(x)                         _GET_MEMORY_DOUBLE( __ticket_costo, x )
#define _TICKET_RECARGO(x)                       _GET_MEMORY_DOUBLE( __ticket_recargo, x )
#define _TICKET_COD_PRESENT(x)                   _GET_MEMORY_CHAR( __ticket_cod_present, x )
#define _TICKET_IVA1_PRECIO(x)                   _GET_MEMORY_DOUBLE( __ticket_iva1_precio, x )
#define _TICKET_IVA2_PRECIO(x)                   _GET_MEMORY_DOUBLE( __ticket_iva2_precio, x )
#define _TICKET_IVA1_REC(x)                      _GET_MEMORY_DOUBLE( __ticket_iva1_rec, x )
#define _TICKET_IVA2_REC(x)                      _GET_MEMORY_DOUBLE( __ticket_iva2_rec, x )
#define _TICKET_EXENTO(x)                        _GET_MEMORY_DOUBLE( __ticket_exento, x )
#define _TICKET_IMPORTE_SIN_IVA(x)               _GET_MEMORY_DOUBLE( __ticket_importe_sin_iva, x )
#define _TICKET_ILA_PRECIO(x)                    _GET_MEMORY_FLOAT( __ticket_ila_precio, x )
#define _TICKET_IHA_PRECIO(x)                    _GET_MEMORY_FLOAT( __ticket_iha_precio, x )
#define _TICKET_ILA_REC(x)                       _GET_MEMORY_FLOAT( __ticket_ila_rec, x )
#define _TICKET_IHA_REC(x)                       _GET_MEMORY_FLOAT( __ticket_iha_rec, x )
#define _TICKET_COD_IMPUESTO1(x)                 _GET_MEMORY_CHAR( __ticket_cod_impuesto1, x )
#define _TICKET_COD_IMPUESTO2(x)                 _GET_MEMORY_CHAR( __ticket_cod_impuesto2, x )
#define _TICKET_COD_IMPUESTO3(x)                 _GET_MEMORY_CHAR( __ticket_cod_impuesto3, x )
#define _TICKET_COD_IMPUESTO4(x)                 _GET_MEMORY_CHAR( __ticket_cod_impuesto4, x )
#define _EXT_IMPUESTOS(x)                        _GET_MEMORY_DOUBLE( __ext_impuestos, x )
#define _TICKET_COD_EXT(x)                       _GET_MEMORY_CHAR( __ticket_cod_ext, x )
#define _TICKET_COD_CLASIFICACION(x)             _GET_MEMORY_LONG( __ticket_cod_clasificacion, x )
#define _TICKET_NRO_CARRO(x)                     _GET_MEMORY_CHAR( __ticket_nro_carro, x )
#define _TICKET_PLAZO_FINANCIACION(x)            _GET_MEMORY_UNSIGNED( __ticket_plazo_financiacion, x )
#define _TICKET_TIPO2(x)                         _GET_MEMORY_CHAR( __ticket_tipo2, x )
#define _TICKET_COD_IVA(x)                       _GET_MEMORY_CHAR( __ticket_cod_iva, x )
#define _TICKET_COD_MOVIMIENTO(x)                _GET_MEMORY_CHAR( __ticket_cod_movimiento, x )
#define _TICKET_ATRIB_DESCUENTOS(x)              _GET_MEMORY_CHAR( __ticket_atrib_descuentos, x )
#define _TICKET_ORIGEN(x)                        _GET_MEMORY_CHAR( __ticket_origen, x )
#define _TICKET_COD_PROMOCION(x)                 _GET_MEMORY_INT( __ticket_cod_promocion, x )
#define _TICKET_COD_PROMOCION1(x)                _GET_MEMORY_INT( __ticket_cod_promocion1, x )
#define _TICKET_COD_PROMOCION2(x)                _GET_MEMORY_INT( __ticket_cod_promocion2, x )
#define _TICKET_COD_PROMOCION3(x)                _GET_MEMORY_INT( __ticket_cod_promocion3, x )
#define _TICKET_GRAVADO(x)                       _GET_MEMORY_CHAR( __ticket_gravado, x )
#define _TICKET_DEPTO(x)                         _GET_MEMORY_INT( __ticket_depto, x )
#define _TICKET_COD_COBRO(x)                     _GET_MEMORY_INT( __ticket_cod_cobro, x )
#define _TICKET_DESCUENTO_CANTIDAD(x)            _GET_MEMORY_DOUBLE( __ticket_descuento_cantidad, x )
#define _TICKET_COD_SOLICITUD(x)                 _GET_MEMORY_INT( __ticket_cod_solicitud, x )
#define _TICKET_ALICUOTA_PRECIO(x)               _GET_MEMORY_FLOAT( __ticket_alicuota_precio, x )
#define _TICKET_HAB_DESC_CLIENTE(x)              _GET_MEMORY_CHAR( __ticket_hab_desc_cliente, x )
#define _TICKET_COD_BARRA(x)                     _GET_MEMORY_STR( __ticket_cod_barra, x )
#define _TICKET_COD_BANDA(x)                     _GET_MEMORY_INT( __ticket_cod_banda, x )
#define _TICKET_TIPO_ENVASE(x)                   _GET_MEMORY_INT( __ticket_tipo_envase, x )
#define _TICKET_COD_BARRA_ORIGINAL(x)            _GET_MEMORY_STR( __ticket_cod_barra_original, x )
#define _TICKET_DESCUENTO(x)                     _GET_MEMORY_DOUBLE( __ticket_descuento, x )
#define _TICKET_IMPUESTO1_PRECIO(x)              _GET_MEMORY_FLOAT( __ticket_impuesto1_precio, x )
#define _TICKET_IMPUESTO2_PRECIO(x)              _GET_MEMORY_FLOAT( __ticket_impuesto2_precio, x )
#define _TICKET_IMPUESTO3_PRECIO(x)              _GET_MEMORY_FLOAT( __ticket_impuesto3_precio, x )
#define _TICKET_IMPUESTO4_PRECIO(x)              _GET_MEMORY_FLOAT( __ticket_impuesto4_precio, x )
#define _TICKET_TIPO3(x)                         _GET_MEMORY_CHAR( __ticket_tipo3, x )
#define _TICKET_CANT_EXCL_DESC(x)                _GET_MEMORY_FLOAT( __ticket_cant_excl_desc, x )
#define _TICKET_PRECIO_ORIGINAL(x)               _GET_MEMORY_DOUBLE( __ticket_precio_original, x )
#define _TICKET_COD_ESPECIAL_ALFA(x)             _GET_MEMORY_STR( __ticket_cod_especial_alfa, x )
#define _TICKET_COD_CLASIFICACION_STR(x)         _GET_MEMORY_STR( __ticket_cod_clasificacion_str, x )
#define _TICKET_MOTIVO_DEVOLUCION(x)             _GET_MEMORY_INT( __ticket_motivo_devolucion, x )


/****************************************************************************/
/*   Definiciones de Orden de Variables                                     */ 
/****************************************************************************/

/*--------------------- Variables en RAM no vol til ---------------------*/
#define o_varios_marca_error_red               0
#define o_varios_scanner_habilitado            0
#define o_varios_hora_z                        0
#define o_varios_cajero_z                      0
#define o_varios_offset_altas                  0
#define o_varios_usuarios_habilitados          0
#define o_varios_cajon_activo                  0
#define o_varios_nro_legajo_cajero_z           0
#define o_varios_modo_y_en_z                   0
#define o_varios_copia_ticket_prn              0
#define o_varios_copia_cobro_prn               0
#define o_varios_tarjetas_por_pago             0
#define o_varios_restringir_volver_de_pagos    0
#define o_varios_imprimiendo_texto_fiscal      0
#define o_varios_vaciar_eventos_al_inicio      0
#define o_varios_cambio_fecha_y_hora_cf        0
#define o_var_tarjetas_tarjeta                 1
#define o_var_tarjetas_numero                  2
#define o_var_tarjetas_fecha_vto               3
#define o_var_tarjetas_limite                  4
#define o_var_tarjetas_cuotas                  5
#define o_var_tarjetas_financia_tarjeta        6
#define o_var_tarjetas_autorizacion            7
#define o_var_tarjetas_nro_cupon               8
#define o_var_tarjetas_nro_cupon_int           9
#define o_var_tarjetas_lectura_banda           10
#define o_var_tarjetas_boletin_vencido         11
#define o_var_tarjetas_recargo                 12
#define o_var_tarjetas_nro_boletin             13
#define o_var_tarjetas_tabla                   14
#define o_var_tarjetas_on_line                 15
#define o_var_tarjetas_adherente               16
#define o_var_tarjetas_anticipo                17
#define o_var_tarjetas_saldo_cliente           18
#define o_var_tarjetas_importe_cuota           19
#define o_var_tarjetas_primer_vto              20
#define o_var_tarjetas_moneda                  21
#define o_var_tarjetas_importe_entrega         22
#define o_var_tarjetas_porc_entrega            23
#define o_var_tarjetas_nro_ref                 24
#define o_var_tarjetas_aut_en_boletin          25
#define o_var_tarjetas_cvv                     26
#define o_var_tarjetas_nro_comercio            27
#define o_var_tarjetas_cod_servicio            28
#define o_var_tarjetas_unicamente_ol           29
#define o_var_tarjetas_necesita_pin            30
#define o_var_tarjetas_pedir_auto              31
#define o_var_tarjetas_lote                    32
#define o_var_tarjetas_captura_on_line         33
#define o_var_tarjetas_coeficiente             34
#define o_var_tarjetas_aut_on_line             35
#define o_var_tarjetas_terminal                36
#define o_var_tarjetas_status                  37
#define o_var_tarjetas_tipo_cuenta             38
#define o_var_tarjetas_pin                     39
#define o_var_tarjetas_celda_pinpad            40
#define o_var_tarjetas_l_pin                   41
#define o_var_tarjetas_mensaje                 42
#define o_var_tarjetas_cod_vigencia            43
#define o_var_tarjetas_pide_entrega            44
#define o_var_tarjetas_nro_orden_cuota         45
#define o_var_tarjetas_control_cuotas          46
#define o_var_tarjetas_cupon_original          47
#define o_var_tarjetas_fecha_original          48
#define o_var_tarjetas_recargo_diferim         49
#define o_var_tarjetas_track1                  50
#define o_var_tarjetas_track2                  51
#define o_var_tarjetas_fecha_contable          52
#define o_var_tarjetas_fecha_host              53
#define o_var_tarjetas_hora_host               54
#define o_var_tarjetas_nro_cuenta              55
#define o_var_tarjetas_hora_original           56
#define o_var_tarjetas_aut_original            57
#define o_var_tarjetas_tasa                    58
#define o_var_tarjetas_vuelto                  59
#define o_var_tarjetas_grupo_cuota             60
#define o_var_tarjetas_tasa_aplicada           61
#define o_var_tarjetas_red                     62
#define o_var_tarjetas_confirmar_numero        63
#define o_var_tarjetas_ultimos_digitos         64
#define o_var_tarjetas_min_cuotas              65
#define o_var_tarjetas_max_cuotas              66
#define o_var_tarjetas_fecha_tope_predatada    67
#define o_var_tarjetas_fecha_tope_1_cuota      68
#define o_var_tarjetas_pedir_cvv               69
#define o_var_tarjetas_pedir_garantia          70
#define o_var_tarjetas_op_habilitadas          71
#define o_var_tarjetas_caja_original           72
#define o_var_tarjetas_suc_original            73
#define o_var_tarjetas_pin_ascii               74
#define o_var_tarjetas_nom_arch_voucher        75
#define o_var_tarjetas_datafono                76
#define o_var_tarjetas_nro_trace               77
#define o_var_tarjetas_importe                 78
#define o_var_tarjetas_operacion               79
#define o_var_tarjetas_ant_tarjeta             1
#define o_var_tarjetas_ant_numero              2
#define o_var_tarjetas_ant_fecha_vto           3
#define o_var_tarjetas_ant_limite              4
#define o_var_tarjetas_ant_cuotas              5
#define o_var_tarjetas_ant_financia_tarjeta    6
#define o_var_tarjetas_ant_autorizacion        7
#define o_var_tarjetas_ant_nro_cupon           8
#define o_var_tarjetas_ant_nro_cupon_int       9
#define o_var_tarjetas_ant_lectura_banda       10
#define o_var_tarjetas_ant_boletin_vencido     11
#define o_var_tarjetas_ant_recargo             12
#define o_var_tarjetas_ant_nro_boletin         13
#define o_var_tarjetas_ant_tabla               14
#define o_var_tarjetas_ant_on_line             15
#define o_var_tarjetas_ant_adherente           16
#define o_var_tarjetas_ant_anticipo            17
#define o_var_tarjetas_ant_saldo_cliente       18
#define o_var_tarjetas_ant_importe_cuota       19
#define o_var_tarjetas_ant_primer_vto          20
#define o_var_tarjetas_ant_moneda              21
#define o_var_tarjetas_ant_importe_entrega     22
#define o_var_tarjetas_ant_porc_entrega        23
#define o_var_tarjetas_ant_nro_ref             24
#define o_var_tarjetas_ant_aut_en_boletin      25
#define o_var_tarjetas_ant_cvv                 26
#define o_var_tarjetas_ant_nro_comercio        27
#define o_var_tarjetas_ant_cod_servicio        28
#define o_var_tarjetas_ant_unicamente_ol       29
#define o_var_tarjetas_ant_necesita_pin        30
#define o_var_tarjetas_ant_pedir_auto          31
#define o_var_tarjetas_ant_lote                32
#define o_var_tarjetas_ant_captura_on_line     33
#define o_var_tarjetas_ant_coeficiente         34
#define o_var_tarjetas_ant_aut_on_line         35
#define o_var_tarjetas_ant_terminal            36
#define o_var_tarjetas_ant_status              37
#define o_var_tarjetas_ant_tipo_cuenta         38
#define o_var_tarjetas_ant_pin                 39
#define o_var_tarjetas_ant_celda_pinpad        40
#define o_var_tarjetas_ant_l_pin               41
#define o_var_tarjetas_ant_mensaje             42
#define o_var_tarjetas_ant_cod_vigencia        43
#define o_var_tarjetas_ant_pide_entrega        44
#define o_var_tarjetas_ant_nro_orden_cuota     45
#define o_var_tarjetas_ant_control_cuotas      46
#define o_var_tarjetas_ant_cupon_original      47
#define o_var_tarjetas_ant_fecha_original      48
#define o_var_tarjetas_ant_recargo_diferim     49
#define o_var_tarjetas_ant_track1              50
#define o_var_tarjetas_ant_track2              51
#define o_var_tarjetas_ant_fecha_contable      52
#define o_var_tarjetas_ant_fecha_host          53
#define o_var_tarjetas_ant_hora_host           54
#define o_var_tarjetas_ant_nro_cuenta          55
#define o_var_tarjetas_ant_hora_original       56
#define o_var_tarjetas_ant_aut_original        57
#define o_var_tarjetas_ant_tasa                58
#define o_var_tarjetas_ant_vuelto              59
#define o_var_tarjetas_ant_grupo_cuota         60
#define o_var_tarjetas_ant_tasa_aplicada       61
#define o_var_tarjetas_ant_red                 62
#define o_var_tarjetas_ant_confirmar_numero    63
#define o_var_tarjetas_ant_ultimos_digitos     64
#define o_var_tarjetas_ant_min_cuotas          65
#define o_var_tarjetas_ant_max_cuotas          66
#define o_var_tarjetas_ant_fecha_tope_predata  67
#define o_var_tarjetas_ant_fecha_tope_1_cuota  68
#define o_var_tarjetas_ant_pedir_cvv           69
#define o_var_tarjetas_ant_pedir_garantia      70
#define o_var_tarjetas_ant_op_habilitadas      71
#define o_var_tarjetas_ant_caja_original       72
#define o_var_tarjetas_ant_suc_original        73
#define o_var_tarjetas_ant_pin_ascii           74
#define o_var_tarjetas_ant_nom_arch_voucher    75
#define o_var_tarjetas_ant_datafono            76
#define o_var_tarjetas_ant_nro_trace           77
#define o_var_tarjetas_ant_importe             78
#define o_var_tarjetas_ant_operacion           79
#define o_pago_modo                            0
#define o_pago_importe                         0
#define o_pago_tarjeta                         0
#define o_pago_cuenta                          0
#define o_pago_buffer1_long                    0
#define o_pago_numero                          0
#define o_pago_status_tarjeta                  0
#define o_pago_banco                           0
#define o_pago_fecha_vto                       0
#define o_pago_cantidad_cupones                0
#define o_pago_valor_cupones                   0
#define o_pago_cod_post_cheq                   0
#define o_pago_cod_hora_acre                   0
#define o_pago_nro_tarjeta                     0
#define o_pago_auto_tarjeta                    0
#define o_pago_nro_cupon1                      0
#define o_pago_nro_cupon_int1                  0
#define o_pago_cuotas                          0
#define o_pago_buffer                          0
#define o_pago_nro_boletin                     0
#define o_pago_tabla                           0
#define o_pago_variacion                       0
#define o_pago_viejo_cuit                      0
#define o_pago_telefono                        0
#define o_pago_vuelto                          0
#define o_pago_imp_moneda_ext                  0
#define o_pago_moneda                          0
#define o_pago_cod_vigencia                    0
#define o_pago_primer_vto                      0
#define o_pago_cuenta_bco                      0
#define o_pago_importe_entrega                 0
#define o_pago_id_registro                     0
#define o_pago_vuelto_medio                    0
#define o_pago_cod_medio_vuelto                0
#define o_pago_cod_submedio_vuelto             0
#define o_pago_tipo_vuelto                     0
#define o_pago_importe_me                      0
#define o_pago_cod_solicitud                   0
#define o_pago_iva_proporcional                0
#define o_pago_telefono_otro                   0
#define o_pago_imp_cuota                       0
#define o_pago_cod_restriccion_art             0
#define o_pago_datafono                        0
#define o_pago_cuit                            0
#define o_pago_cod_submedio                    0
#define o_pago_importe_descuento               0
#define o_nro_caja                             0
#define o_nro_nom_caja                         0
#define o_nro_intereses                        0
#define o_nro_x                                0
#define o_nro_z                                0
#define o_nro_ticket                           0
#define o_nro_cajero                           0
#define o_nro_cajero_ant                       0
#define o_nro_variacion_precio                 0
#define o_nro_modo                             0
#define o_nro_y                                0
#define o_nro_vendedor                         0
#define o_nro_fecha_operacion                  0
#define o_nro_dia_operativo                    0
#define o_nro_hubo_operaciones                 0
#define o_nro_cupones                          0
#define o_nro_nro_retiro                       0
#define o_nro_fecha_z                          0
#define o_nro_ultimo_ticket_ant                0
#define o_nro_gran_total                       0
#define o_nro_recargo_financiacion             0
#define o_nro_cod_cuota                        0
#define o_nro_z_emitida                        0
#define o_nro_cant_tickets_ant                 0
#define o_nro_total_bonos                      0
#define o_nro_ultima_alta_procesada            0
#define o_nro_ultima_modif_procesada           0
#define o_nro_comp_cta_cte                     0
#define o_nro_remito                           0
#define o_nro_rollo                            0
#define o_nro_mozo                             0
#define o_nro_supervisor_ant                   0
#define o_nro_evento                           0
#define o_nro_caja_dgi                         0
#define o_nro_legajo_cajero                    0
#define o_nro_evento_a_procesar                0
#define o_nro_dia_operativo_ultima_z           0
#define o_nro_proximo_dia_operativo_servidor   0
#define o_nro_ultima_novedad_cliente           0
#define o_nro_ultimo_evento_analizado          0
#define o_nro_ultimo_evento_controlado         0
#define o_nro_eventos_pendientes_z_anterior    0
#define o_nro_estado_actual                    0
#define o_nro_entrada_estado_actual            0
#define o_nro_ultima_novedad_boletin           0
#define o_nro_vendedor_long                    0
#define o_nro_correlativo_fondo_fijo           0
#define o_nro_correlativo_retiro               0
#define o_nro_correlativo_inventario           0
#define o_nro_recupero_z                       0
#define o_nro_evento_anterior                  0
#define o_nro_pagare                           0
#define o_nro_correlativo_informes             0
#define o_nro_vendedor_string                  0
#define o_nro_estado_journal                   0
#define o_nro_estacion_activa                  0
#define o_nro_ultimo_ticket_rango              0
#define o_nro_orden_dosificacion               0
#define o_nro_alfanumerico                     0
#define o_nro_alfa                             0
#define o_nro_sucursal_renta                   0
#define o_nro_registro                         0
#define o_nro_evento_prioridad_procesar        0
#define o_x_ventas_importe                     0
#define o_x_ventas_cantidad                    1
#define o_x_retiros_importe                    2
#define o_x_retiros_cantidad                   3
#define o_x_adelantos_importe                  4
#define o_x_adelantos_cantidad                 5
#define o_x_intereses_importe                  6
#define o_x_intereses_cantidad                 7
#define o_x_fondo_fijo_importe                 8
#define o_x_fondo_fijo_cantidad                9
#define o_x_anulaciones_importe                10
#define o_x_anulaciones_cantidad               11
#define o_x_dev_envases_importe                12
#define o_x_dev_envases_cantidad               13
#define o_x_devoluciones_importe               14
#define o_x_devoluciones_cantidad              15
#define o_x_ventas_gravadas                    16
#define o_x_ventas_exentas                     17
#define o_x_imp_int                            18
#define o_x_dep_imp                            19
#define o_x_dep_can                            20
#define o_x_demora_segundos                    21
#define o_x_demora_ventas                      22
#define o_x_tot_dinero                         23
#define o_x_iva1                               24
#define o_x_iva2                               25
#define o_x_retiro_fondo_importe               26
#define o_x_retiro_fondo_cantidad              27
#define o_x_mozos_importe                      28
#define o_x_mozos_cantidad                     29
#define o_x_cobros_importe                     30
#define o_x_cobros_cantidad                    31
#define o_x_pagos_por_caja_importe             32
#define o_x_pagos_por_caja_cantidad            33
#define o_x_facturas_a_importe                 34
#define o_x_facturas_a_cantidad                35
#define o_x_facturas_b_importe                 36
#define o_x_facturas_b_cantidad                37
#define o_x_vueltos_importe                    38
#define o_x_vueltos_cantidad                   39
#define o_x_bonos_entregados                   40
#define o_x_ila                                41
#define o_x_iha                                42
#define o_x_diferencia                         43
#define o_x_nc_a_importe                       44
#define o_x_nc_a_cantidad                      45
#define o_x_nc_b_importe                       46
#define o_x_nc_b_cantidad                      47
#define o_x_fondo_fijo_multiple_importe        48
#define o_x_fondo_fijo_multiple_cantidad       49
#define o_x_retencion_dgr                      50
#define o_x_percepcion_dgi                     51
#define o_x_importe_iva                        52
#define o_x_importe_sobretasa_iva              53
#define o_x_correccion_importe                 54
#define o_x_correccion_cantidad                55
#define o_x_nro_primer_ticket                  56
#define o_x_cantidad_tickets                   57
#define o_x_cobros_cantidad_servicios          58
#define o_x_percepcion_212_iva1                59
#define o_x_percepcion_212_iva2                60
#define o_x_diferencia_redondeo                61
#define o_x_imp_devoluciones                   62
#define o_x_cant_devoluciones                  63
#define o_x_cant_articulos                     64
#define o_x_cant_art_escaneados                65
#define o_x_tmpo_reg_articulo                  66
#define o_x_tmpo_pago                          67
#define o_x_tmpo_standby                       68
#define o_x_tmpo_ocio                          69
#define o_x_imp_cancelados                     70
#define o_x_cant_cancelados                    71
#define o_x_cambios_importe                    72
#define o_x_cambios_cantidad                   73
#define o_x_ventas_importe_me                  74
#define o_x_fondo_fijo_multiple_imp_me         75
#define o_x_retiros_importe_me                 76
#define o_x_donaciones_cantidad                77
#define o_x_donaciones_importe                 78
#define o_x_alicuota                           79
#define o_x_desc_porc_importe                  80
#define o_x_desc_porc_cantidad                 81
#define o_x_desc_porc_ult_art_importe          82
#define o_x_desc_porc_ult_art_cantidad         83
#define o_x_desc_pesos_importe                 84
#define o_x_desc_pesos_cantidad                85
#define o_x_desc_pesos_ult_art_importe         86
#define o_x_desc_pesos_ult_art_cantidad        87
#define o_x_desc_porc_importe_sin_iva          88
#define o_x_desc_porc_ult_art_imp_sin_iva      89
#define o_x_desc_pesos_importe_sin_iva         90
#define o_x_desc_pesos_ult_art_imp_sin_iva     91
#define o_x_cant_ticket_dev_envase             92
#define o_x_impuestos                          93
#define o_x_donaciones_ventas_importe          94
#define o_x_donaciones_ventas_cantidad         95
#define o_x_boletas_con_derecho_a_nc           96
#define o_x_cant_boletas_con_derecho_a_nc      97
#define o_x_devoluciones_medio                 98
#define o_x_factura_a_importe_exento           99
#define o_x_factura_a_importe_gravado          100
#define o_x_nc_a_importe_exento                101
#define o_x_nc_a_importe_gravado               102
#define o_x_cons_final_importe_exent_fact      103
#define o_x_cons_final_importe_grav_fact       104
#define o_x_cons_final_cantidad_fact           105
#define o_x_dev_cons_f_importe_exent_fact      106
#define o_x_dev_cons_f_importe_grav_fact       107
#define o_x_dev_cons_f_cantidad_fact           108
#define o_x_ventas_diplom_importe              109
#define o_x_ventas_diplom_cantidad             110
#define o_x_devoluciones_diplom_importe        111
#define o_x_devoluciones_diplom_cantidad       112
#define o_x_nro_transaccion_inicial            113
#define o_x_nro_transaccion_final              114
#define o_x_devoluciones_export_importe        115
#define o_x_devoluciones_export_cantidad       116
#define o_x_cons_final_importe_exent_tkt       117
#define o_x_cons_final_importe_grav_tkt        118
#define o_x_cons_final_cantidad_tkt            119
#define o_x_dev_cons_final_importe_exent_tkt   120
#define o_x_dev_cons_f_importe_grav_tkt        121
#define o_x_dev_cons_f_cantidad_tkt            122
#define o_x_iva_factura_a                      123
#define o_x_iva_cons_final                     124
#define o_x_ventas_export_importe              125
#define o_x_ventas_export_cantidad             126
#define o_x_ofertas_importe                    127
#define o_x_descuentos_medio                   128
#define o_x_iva_nc_a                           129
#define o_x_iva_devol_consumidor_final         130
#define o_x_ventas_importe_sin_redondeo        131
#define o_x_cobros_importe_me                  132
#define o_x_retiro_fondo_importe_pagos         133
#define o_x_tmpo_no_venta                      134
#define o_x_dif_redondeo_vtas_contrib          135
#define o_x_dif_redondeo_iva_contrib           136
#define o_x_dif_redondeo_nc                    137
#define o_x_dif_redondeo_iva_nc                138
#define o_x_dif_redondeo_grav_ticket           139
#define o_x_dif_redondeo_exent_ticket          140
#define o_x_dif_redondeo_iva_ticket            141
#define o_x_cobros_importes_servicios          142
#define o_z_ventas_importe                     0
#define o_z_ventas_cantidad                    1
#define o_z_retiros_importe                    2
#define o_z_retiros_cantidad                   3
#define o_z_adelantos_importe                  4
#define o_z_adelantos_cantidad                 5
#define o_z_intereses_importe                  6
#define o_z_intereses_cantidad                 7
#define o_z_fondo_fijo_importe                 8
#define o_z_fondo_fijo_cantidad                9
#define o_z_anulaciones_importe                10
#define o_z_anulaciones_cantidad               11
#define o_z_dev_envases_importe                12
#define o_z_dev_envases_cantidad               13
#define o_z_devoluciones_importe               14
#define o_z_devoluciones_cantidad              15
#define o_z_ventas_gravadas                    16
#define o_z_ventas_exentas                     17
#define o_z_imp_int                            18
#define o_z_dep_imp                            19
#define o_z_dep_can                            20
#define o_z_demora_segundos                    21
#define o_z_demora_ventas                      22
#define o_z_tot_dinero                         23
#define o_z_iva1                               24
#define o_z_iva2                               25
#define o_z_retiro_fondo_importe               26
#define o_z_retiro_fondo_cantidad              27
#define o_z_mozos_importe                      28
#define o_z_mozos_cantidad                     29
#define o_z_cobros_importe                     30
#define o_z_cobros_cantidad                    31
#define o_z_pagos_por_caja_importe             32
#define o_z_pagos_por_caja_cantidad            33
#define o_z_facturas_a_importe                 34
#define o_z_facturas_a_cantidad                35
#define o_z_facturas_b_importe                 36
#define o_z_facturas_b_cantidad                37
#define o_z_vueltos_importe                    38
#define o_z_vueltos_cantidad                   39
#define o_z_bonos_entregados                   40
#define o_z_ila                                41
#define o_z_iha                                42
#define o_z_diferencia                         43
#define o_z_nc_a_importe                       44
#define o_z_nc_a_cantidad                      45
#define o_z_nc_b_importe                       46
#define o_z_nc_b_cantidad                      47
#define o_z_fondo_fijo_multiple_importe        48
#define o_z_fondo_fijo_multiple_cantidad       49
#define o_z_retencion_dgr                      50
#define o_z_percepcion_dgi                     51
#define o_z_importe_iva                        52
#define o_z_importe_sobretasa_iva              53
#define o_z_correccion_importe                 54
#define o_z_correccion_cantidad                55
#define o_z_nro_primer_ticket                  56
#define o_z_cantidad_tickets                   57
#define o_z_cobros_cantidad_servicios          58
#define o_z_percepcion_212_iva1                59
#define o_z_percepcion_212_iva2                60
#define o_z_diferencia_redondeo                61
#define o_z_imp_devoluciones                   62
#define o_z_cant_devoluciones                  63
#define o_z_cant_articulos                     64
#define o_z_cant_art_escaneados                65
#define o_z_tmpo_reg_articulo                  66
#define o_z_tmpo_pago                          67
#define o_z_tmpo_standby                       68
#define o_z_tmpo_ocio                          69
#define o_z_imp_cancelados                     70
#define o_z_cant_cancelados                    71
#define o_z_cambios_importe                    72
#define o_z_cambios_cantidad                   73
#define o_z_ventas_importe_me                  74
#define o_z_fondo_fijo_multiple_imp_me         75
#define o_z_retiros_importe_me                 76
#define o_z_donaciones_cantidad                77
#define o_z_donaciones_importe                 78
#define o_z_alicuota                           79
#define o_z_desc_porc_importe                  80
#define o_z_desc_porc_cantidad                 81
#define o_z_desc_porc_ult_art_importe          82
#define o_z_desc_porc_ult_art_cantidad         83
#define o_z_desc_pesos_importe                 84
#define o_z_desc_pesos_cantidad                85
#define o_z_desc_pesos_ult_art_importe         86
#define o_z_desc_pesos_ult_art_cantidad        87
#define o_z_desc_porc_importe_sin_iva          88
#define o_z_desc_porc_utl_art_imp_sin_iva      89
#define o_z_desc_pesos_importe_sin_iva         90
#define o_z_desc_pesos_ult_art_imp_sin_iva     91
#define o_z_cant_ticket_dev_envase             92
#define o_z_impuestos                          93
#define o_z_donaciones_ventas_importe          94
#define o_z_donaciones_ventas_cantidad         95
#define o_z_boletas_con_derecho_a_nc           96
#define o_z_cant_boletas_con_derecho_a_nc      97
#define o_z_devoluciones_medio                 98
#define o_z_factura_a_importe_exento           99
#define o_z_factura_a_importe_gravado          100
#define o_z_nc_a_importe_exento                101
#define o_z_nc_a_importe_gravado               102
#define o_z_cons_final_importe_exent_fact      103
#define o_z_cons_final_importe_grav_fact       104
#define o_z_cons_final_cantidad_fact           105
#define o_z_dev_cons_f_importe_exent_fact      106
#define o_z_dev_cons_f_importe_grav_fact       107
#define o_z_dev_cons_f_cantidad_fact           108
#define o_z_ventas_diplom_importe              109
#define o_z_ventas_diplom_cantidad             110
#define o_z_devoluciones_diplom_importe        111
#define o_z_devoluciones_diplom_cantidad       112
#define o_z_nro_transaccion_inicial            113
#define o_z_nro_transaccion_final              114
#define o_z_devoluciones_export_importe        115
#define o_z_devoluciones_export_cantidad       116
#define o_z_cons_final_importe_exent_tkt       117
#define o_z_cons_final_importe_grav_tkt        118
#define o_z_cons_final_cantidad_tkt            119
#define o_z_dev_cons_f_importe_exent_tkt       120
#define o_z_dev_cons_f_importe_grav_tkt        121
#define o_z_dev_cons_f_cantidad_tkt            122
#define o_z_iva_factura_a                      123
#define o_z_iva_cons_final                     124
#define o_z_ventas_export_importe              125
#define o_z_ventas_export_cantidad             126
#define o_z_oferta_importe                     127
#define o_z_descuentos_medio                   128
#define o_z_iva_nc_a                           129
#define o_z_iva_devol_consumidor_final         130
#define o_z_ventas_importe_sin_redondeo        131
#define o_z_cobros_importe_me                  132
#define o_z_retiro_fondo_importe_pagos         133
#define o_z_tmpo_no_venta                      134
#define o_z_dif_redondeo_vtas_contrib          135
#define o_z_dif_redondeo_iva_contrib           136
#define o_z_dif_redondeo_nc                    137
#define o_z_dif_redondeo_iva_nc                138
#define o_z_dif_redondeo_grav_ticket           139
#define o_z_dif_redondeo_exent_ticket          140
#define o_z_dif_redondeo_iva_ticket            141
#define o_z_cobros_importes_servicios          142
#define o_z_ant_ventas_importe                 0
#define o_z_ant_ventas_cantidad                1
#define o_z_ant_retiros_importe                2
#define o_z_ant_retiros_cantidad               3
#define o_z_ant_adelantos_importe              4
#define o_z_ant_adelantos_cantidad             5
#define o_z_ant_intereses_importe              6
#define o_z_ant_intereses_cantidad             7
#define o_z_ant_fondo_fijo_importe             8
#define o_z_ant_fondo_fijo_cantidad            9
#define o_z_ant_anulaciones_importe            10
#define o_z_ant_anulaciones_cantidad           11
#define o_z_ant_dev_envases_importe            12
#define o_z_ant_dev_envases_cantidad           13
#define o_z_ant_devoluciones_importe           14
#define o_z_ant_devoluciones_cantidad          15
#define o_z_ant_ventas_gravadas                16
#define o_z_ant_ventas_exentas                 17
#define o_z_ant_imp_int                        18
#define o_z_ant_dep_imp                        19
#define o_z_ant_dep_can                        20
#define o_z_ant_demora_segundos                21
#define o_z_ant_demora_ventas                  22
#define o_z_ant_tot_dinero                     23
#define o_z_ant_iva1                           24
#define o_z_ant_iva2                           25
#define o_z_ant_retiro_fondo_importe           26
#define o_z_ant_retiro_fondo_cantidad          27
#define o_z_ant_mozos_importe                  28
#define o_z_ant_mozos_cantidad                 29
#define o_z_ant_cobros_importe                 30
#define o_z_ant_cobros_cantidad                31
#define o_z_ant_pagos_por_caja_importe         32
#define o_z_ant_pagos_por_caja_cantidad        33
#define o_z_ant_facturas_a_importe             34
#define o_z_ant_facturas_a_cantidad            35
#define o_z_ant_facturas_b_importe             36
#define o_z_ant_facturas_b_cantidad            37
#define o_z_ant_vueltos_importe                38
#define o_z_ant_vueltos_cantidad               39
#define o_z_ant_bonos_entregados               40
#define o_z_ant_ila                            41
#define o_z_ant_iha                            42
#define o_z_ant_diferencia                     43
#define o_z_ant_nc_a_importe                   44
#define o_z_ant_nc_a_cantidad                  45
#define o_z_ant_nc_b_importe                   46
#define o_z_ant_nc_b_cantidad                  47
#define o_z_ant_fondo_fijo_multiple_importe    48
#define o_z_ant_fondo_fijo_multiple_cantidad   49
#define o_z_ant_retencion_dgr                  50
#define o_z_ant_percepcion_dgi                 51
#define o_z_ant_importe_iva                    52
#define o_z_ant_importe_sobretasa_iva          53
#define o_z_ant_correccion_importe             54
#define o_z_ant_correccion_cantidad            55
#define o_z_ant_nro_primer_ticket              56
#define o_z_ant_cantidad_tickets               57
#define o_z_ant_cobros_cantidad_servicios      58
#define o_z_ant_percepcion_212_iva1            59
#define o_z_ant_percepcion_212_iva2            60
#define o_z_ant_diferencia_redondeo            61
#define o_z_ant_imp_devoluciones               62
#define o_z_ant_cant_devoluciones              63
#define o_z_ant_cant_articulos                 64
#define o_z_ant_cant_art_escaneados            65
#define o_z_ant_tmpo_reg_articulo              66
#define o_z_ant_tmpo_pago                      67
#define o_z_ant_tmpo_standby                   68
#define o_z_ant_tmpo_ocio                      69
#define o_z_ant_imp_cancelados                 70
#define o_z_ant_cant_cancelados                71
#define o_z_ant_cambios_importe                72
#define o_z_ant_cambios_cantidad               73
#define o_z_ant_ventas_importe_me              74
#define o_z_ant_fondo_fijo_multiple_imp_m      75
#define o_z_ant_retiros_importe_me             76
#define o_z_ant_donaciones_cantidad            77
#define o_z_ant_donaciones_importe             78
#define o_z_ant_alicuota                       79
#define o_z_ant_desc_porc_importe              80
#define o_z_ant_desc_porc_cantidad             81
#define o_z_ant_desc_porc_ult_art_importe      82
#define o_z_ant_desc_porc_ult_art_cantidad     83
#define o_z_ant_desc_pesos_importe             84
#define o_z_ant_desc_pesos_cantidad            85
#define o_z_ant_desc_pesos_ult_art_importe     86
#define o_z_ant_desc_pesos_ult_art_cantidad    87
#define o_z_ant_desc_porc_importe_sin_iva      88
#define o_z_ant_desc_porc_ult_art_imp_sin_iva  89
#define o_z_ant_desc_pesos_importe_sin_iva     90
#define o_z_ant_desc_pesos_ult_art_imp_sin_iv  91
#define o_z_ant_cant_ticket_dev_envase         92
#define o_z_ant_impuestos                      93
#define o_z_ant_donaciones_ventas_importe      94
#define o_z_ant_donaciones_ventas_cantidad     95
#define o_z_ant_boletas_con_derecho_a_nc       96
#define o_z_ant_cant_boletas_con_derecho_a_nc  97
#define o_z_ant_devoluciones_medio             98
#define o_z_ant_factura_a_importe_exento       99
#define o_z_ant_factura_a_importe_gravado      100
#define o_z_ant_nc_a_importe_exento            101
#define o_z_ant_nc_a_importe_gravado           102
#define o_z_ant_cons_final_imp_exent_fact      103
#define o_z_ant_cons_final_imp_grav_fact       104
#define o_z_ant_cons_final_cantidad_fact       105
#define o_z_ant_dev_cons_f_imp_exent_fact      106
#define o_z_ant_dev_cons_f_imp_grav_fact       107
#define o_z_ant_dev_cons_f_cantidad_fact       108
#define o_z_ant_ventas_diplom_importe          109
#define o_z_ant_ventas_diplom_cantidad         110
#define o_z_ant_devoluciones_diplom_importe    111
#define o_z_ant_devoluciones_diplom_cantidad   112
#define o_z_ant_nro_transaccion_inicial        113
#define o_z_ant_nro_transaccion_final          114
#define o_z_ant_devoluciones_export_importe    115
#define o_z_ant_devoluciones_export_cantidad   116
#define o_z_ant_cons_fin_importe_exent_tkt     117
#define o_z_ant_cons_fin_importe_grav_tkt      118
#define o_z_ant_cons_final_cantidad_tkt        119
#define o_z_ant_dev_con_f_imp_exent_tkt        120
#define o_z_ant_dev_con_f_imp_grav_tkt         121
#define o_z_ant_dev_cons_f_cantidad_tkt        122
#define o_z_ant_iva_factura_a                  123
#define o_z_ant_iva_cons_final                 124
#define o_z_ant_ventas_export_importe          125
#define o_z_ant_ventas_export_cantidad         126
#define o_z_ant_oferta_importe                 127
#define o_z_ant_descuentos_medio               128
#define o_z_ant_iva_nc_a                       129
#define o_z_ant_iva_devol_consumidor_final     130
#define o_z_ant_ventas_importe_sin_redondeo    131
#define o_z_ant_cobros_importe_me              132
#define o_z_ant_retiro_fondo_importe_pagos     133
#define o_z_ant_tmpo_no_venta                  134
#define o_z_ant_dif_redondeo_vtas_contrib      135
#define o_z_ant_dif_redondeo_iva_contrib       136
#define o_z_ant_dif_redondeo_nc                137
#define o_z_ant_dif_redondeo_iva_nc            138
#define o_z_ant_dif_redondeo_grav_ticket       139
#define o_z_ant_dif_redondeo_exent_ticket      140
#define o_z_ant_dif_redondeo_iva_ticket        141
#define o_z_ant_cobros_importes_servicios      142
#define o_ram_sign                             0
#define o_ram_p_ticket                         0
#define o_ram_ptr                              0
#define o_ram_en_importacion_de_ticket         0
#define o___no_se_usa_002                      0
#define o_ram_cantidad                         0
#define o_ram_total                            0
#define o_ram_total_ant                        0
#define o_ram_importe                          0
#define o_ram_importe_total                    0
#define o___ram_total_gravado                  0
#define o___ram_tot_gravado                    0
#define o___m_gravado                          0
#define o___m_total_imp_int                    0
#define o_ram_intereses                        0
#define o_ram_vuelto                           0
#define o_ram_fin_ticket                       0
#define o_ram_anulacion                        0
#define o_ram_item_supervisado                 0
#define o_ram_bultos                           0
#define o_ram_bultos_ant                       0
#define o_ram_cant_bultos                      0
#define o_ram_present_ant                      0
#define o_ram_empieza_ticket                   0
#define o_ram_discriminar                      0
#define o_ram_p_pago                           0
#define o_ram_hora_ingreso_cajero              0
#define o_ram_hora_inicio_ticket               0
#define o_ram_factura                          0
#define o_ram_nro_cliente                      0
#define o_ram_nom_cajero                       0
#define o_ram_doc_cajero                       0
#define o_ram_reprocesando                     0
#define o_ram_no_reprocesar                    0
#define o_ram_solicitar_autorizacion           0
#define o_ram_id_evento                        0
#define o_ram_variar_precio                    0
#define o_ram_variacion                        0
#define o_ram_porc_interes                     0
#define o_ram_cant_promociones                 0
#define o_ram_prohibir_mas                     0
#define o_ram_bonos_a_entregar                 0
#define o_ram_track1                           0
#define o_ram_track2                           0
#define o_ram_ptr_prom_cupon                   0
#define o_ram_prom_cupon_formato               0
#define o_ram_prom_cupon_cantidad              0
#define o_ram_pedir_cliente_afinidad           0
#define o_ram_cliente_afinidad                 0
#define o_ram_nota_cr                          0
#define o_ram_oc_gd                            0
#define o_ram_tipo_inventario                  0
#define o_ram_cupon_envases                    0
#define o_ram_usar_precio_cr                   0
#define o_ram_total_vta_contado                0
#define o_ram_total_anterior                   0
#define o_ram_debe_pagar_contado               0
#define o_ram_hora_inicio_evento               0
#define o_ram_cod_barra                        0
#define o_ram_cod_funcion                      0
#define o_ram_id_evento_all                    0
#define o_ram_resto                            0
#define o_ram_cantidad_total                   0
#define o_ram_renglones                        0
#define o_ram_p_ext_impuestos                  0
#define o_ram_ticket_en_espera                 0
#define o_ram_cambio                           0
#define o_ram_pedir_precio                     0
#define o_ram_negativo                         0
#define o_ram_boleta_anterior                  0
#define o_ram_perfil                           0
#define o_ram_descuento_porcentual             0
#define o_ram_nom_perfil                       0
#define o_ram_retencion_dgr                    0
#define o_ram_percepcion_dgi                   0
#define o_ram_anular_ticket                    0
#define o_ram_pedir_novios                     0
#define o_ram_resultado_transac                0
#define o_ram_sucursal_anterior                0
#define o_ram_caja_anterior                    0
#define o_ram_importacion_activa               0
#define o_ram_importe_aut_ctacte               0
#define o_ram_importe_aut_cheque               0
#define o_ram_nro_carro                        0
#define o___m_plazo_financiacion_cliente       0
#define o_ram_novios                           0
#define o_ram_medios_obligatorios              0
#define o_ram_importacion_pedidos              0
#define o_ram_transac_en_cobro                 0
#define o_ram_imprimir_voucher_cmr             0
#define o_ram_subindice_pago_cmr               0
#define o_ram_acu_puntos                       0
#define o_ram_perfil_excluir_articulos         0
#define o_ram_nombre_cliente_afinidad          0
#define o_ram_puntos_afinidad                  0
#define o_ram_fecha_puntos_afinidad            0
#define o_ram_correccion                       0
#define o_ram_vuelto_efectivo                  0
#define o_ram_diferencia_precio                0
#define o_ram_compromiso_200                   0
#define o_ram_precio_mas_bajo                  0
#define o_ram_present_ant_bak                  0
#define o_ram_enviar_offline                   0
#define o_ram_p_pago_ant                       0
#define o_ram_mostrar_saldo_cheque             0
#define o_ram_imprimir_cabecera                0
#define o_ram_pedir_diferimiento               0
#define o_ram_transaccion_enviada              0
#define o_ram_nro_transaccion_pedida           0
#define o_ram_mult_ticket_en_espera            0
#define o_ram_hora_mult_ticket_en_espera       0
#define o_ram_tarj_documento                   0
#define o_ram_tarj_telefono                    0
#define o_ram_desc_porc_pendientes             0
#define o_ram_modo_ejecucion                   0
#define o_ram_modo_memoria_anterior            0
#define o_ram_cantidad_controlada              0
#define o_ram_bien_de_uso                      0
#define o_ram_fecha_anterior                   0
#define o_ram_tipo_comp_anterior               0
#define o_ram_percepcion_212_iva1              0
#define o_ram_percepcion_212_iva2              0
#define o_ram_tarj_numero                      0
#define o_ram_tarj_importe                     0
#define o_ram_tarj_cuotas                      0
#define o_ram_permite_anular_cobro             0
#define o_ram_string_cliente_afinidad          0
#define o_ram_imp_descuento                    0
#define o_ram_tmpo_reg_articulo                0
#define o_ram_tmpo_pago                        0
#define o_ram_tmpo_standby                     0
#define o_ram_modo_devolucion                  0
#define o_ram_tmpo_ocio                        0
#define o_ram_usa_template                     0
#define o_ram_tomado_por_template              0
#define o_ram_ticket_factura                   0
#define o_ram_impresion_fiscal                 0
#define o_ram_descuento_importe                0
#define o_ram_cantidad_original                0
#define o_ram_afinidad_fecha_nac               0
#define o_ram_codigo_tarjeta_afinidad          0
#define o_ram_diferencia_afinidad              0
#define o_ram_cant_articulos_afinidad          0
#define o_ram_utiliza_precio_afinidad          0
#define o_ram_puntos_usados                    0
#define o_ram_motivo_descuento                 0
#define o_ram_motivo_descuento_pendiente       0
#define o_ram_resto_me                         0
#define o_ram_mostrar_faltante_en_me           0
#define o_ram_control_cambio                   0
#define o_ram_total_donacion                   0
#define o_ram_motivo_devolucion                0
#define o_ram_string_legajo_empleado           0
#define o_ram_institucion_beneficiaria         0
#define o_ram_emitiendo_z                      0
#define o_ram_id_ultimo_evento                 0
#define o_ram_cliente_afinidad_adicional       0
#define o_ram_fecha_cumple_afi_adicional       0
#define o_ram_nom_cli_afinidad_adicional       0
#define o_ram_descuento_afinidad_adicional     0
#define o_ram_cod_mutual_afinidad_adicional    0
#define o_ram_numero_ticket_tomra              0
#define o_ram_acu_puntos_cumple                0
#define o_ram_hoy_cumple_anios                 0
#define o_ram_promociones_afinidad             0
#define o_ram_promociones_afinidad_usadas      0
#define o_ram_puntos_especiales_afinidad       0
#define o_ram_prom_cupon_lugar_impresion       0
#define o_ram_cupon_leyenda                    0
#define o_ram_prom_cupon_importe               0
#define o_ram_codigo_supervisor                0
#define o_ram_venta_fraccion                   0
#define o_ram_cadena_cheque                    0
#define o_ram_acumulador                       0
#define o_ram_cod_boleta_plata                 0
#define o_ram_cant_envases                     0
#define o_ram_referencia_nc                    0
#define o_ram_acumulador_fecha                 0
#define o_ram_prom_cupon_tipo                  0
#define o_ram_prom_cupon_formato_monitor       0
#define o_ram_prom_cupon_grupo                 0
#define o_ram_prom_cupon_nro_acumulador        0
#define o_ram_tipo_evento_comp_original        0
#define o_ram_subtotal                         0
#define o_ram_cod_barra_original               0
#define o_ram_nro_suc                          0
#define o_ram_nro_comp                         0
#define o_ram_cant_comp                        0
#define o_ram_variable_validacion              0
#define o_ram_id_ultimo_evento_factura         0
#define o_ram_nro_ultimo_ticket                0
#define o_ram_desc_porc_importe                0
#define o_ram_cond_iva_cliente                 0
#define o_ram_fecha_ult_compra_afin            0
#define o_ram_suc_ult_compra_afin              0
#define o_ram_desc_categoria_afin              0
#define o_ram_promos_limitan_medios            0
#define o_ram_nro_pedido_pin                   0
#define o_ram_no_a_todo                        0
#define o_ram_total_vta_asistida               0
#define o_ram_total_donacion_venta             0
#define o_ram_cant_descuento                   0
#define o_ram_cod_promo                        0
#define o_ram_iniciar_periodo_venta            0
#define o_ram_consulta_afin_remota             0
#define o_ram_voucher_tarj_comercio            0
#define o_ram_cod_premio                       0
#define o_ram_premio_ejecutado                 0
#define o_ram_ahorro_obtenido                  0
#define o_ram_puntos_esp_1                     0
#define o_ram_puntos_esp_2                     0
#define o_ram_puntos_esp_3                     0
#define o_ram_puntos_esp_4                     0
#define o_ram_puntos_esp_5                     0
#define o_ram_cant_datos_adic_medio            0
#define o_ram_medios_limitados                 0
#define o_ram_sub_medios_limitados             0
#define o_cantidad_medios_limitados            0
#define o_ram_devolucion_parcial_ref           0
#define o_ram_nro_serie_impresora              0
#define o_ram_nro_serie_impresora_ant          0
#define o_ram_hora_anterior                    0
#define o_ram_cuenta_cliente                   0
#define o_ram_pos_mask_eliminar                0
#define o_ram_cierre_y                         0
#define o_ram_copia                            0
#define o_ram_pos_rev_promo                    0
#define o_ram_cobro_servicios                  0
#define o_ram_pos_pago                         0
#define o_ram_agrega_item_tick_import          0
#define o_ram_tmpo_no_venta                    0
#define o_ram_folio_cobro                      0
#define o_ram_flag_cobro_venta                 0
#define o_ram_hay_cobro_venta                  0
#define o_ram_tmpo_ocio_aux                    0
#define o_ram_cliente_afinidad_nombre          0
#define o_ram_diferencia_redondeo_grav         0
#define o_ram_diferencia_redondeo_exent        0
#define o_ram_diferencia_redondeo_iva          0
#define o_ram_precio_credito                   0
#define o_ram_percepcion_tacis                 0
#define o_ram_calcular_retenc_nc               0
#define o_ram_nro_cliente_ant                  0
#define o_ram_reimpresion                      0
#define o_ram_instancia_evento                 0
#define o_ram_modo_ejecucion_ant               0
#define o_ram_impresion_comprobante            0
#define o_tot_tick_importe_sin_iva             1
#define o_tot_tick_imp_int                     2
#define o_tot_tick_costo                       3
#define o_tot_tick_recargo_tarjeta             4
#define o_tot_tick_exento                      5
#define o_tot_tick_iva1                        6
#define o_tot_tick_iva2                        7
#define o_tot_tick_ila                         8
#define o_tot_tick_iha                         9
#define o_tot_tick_diferencia                  10
#define o_tot_tick_total_en_cuotas             11
#define o_tot_tick_imp_tasa                    12
#define o_tot_tick_imp_sobretasa               13
#define o_tot_tick_imp_neto_gravado            14
#define o_tot_tick_alicuota                    15
#define o_tot_tick_impuestos                   16
#define o_tot_tick_total_por_cod_iva           17
#define o_tot_tick_iva1_por_cod_iva            18
#define o_tot_tick_iva2_por_cod_iva            19
#define o_tot_tick_imp_int_por_cod_iva         20
#define o_tot_tick_base_por_cod_iva            21
#define o_tot_tick_ant_importe_sin_iva         1
#define o_tot_tick_ant_imp_int                 2
#define o_tot_tick_ant_costo                   3
#define o_tot_tick_ant_recargo_tarjeta         4
#define o_tot_tick_ant_exento                  5
#define o_tot_tick_ant_iva1                    6
#define o_tot_tick_ant_iva2                    7
#define o_tot_tick_ant_ila                     8
#define o_tot_tick_ant_iha                     9
#define o_tot_tick_ant_diferencia              10
#define o_tot_tick_ant_total_en_cuotas         11
#define o_tot_tick_ant_imp_tasa                12
#define o_tot_tick_ant_imp_sobretasa           13
#define o_tot_tick_ant_imp_neto_gravado        14
#define o_tot_tick_ant_alicuota                15
#define o_tot_tick_ant_impuestos               16
#define o_tot_tick_ant_total_por_cod_iva       17
#define o_tot_tick_ant_iva1_por_cod_iva        18
#define o_tot_tick_ant_iva2_por_cod_iva        19
#define o_tot_tick_ant_imp_int_por_cod_iva     20
#define o_tot_tick_ant_base_por_cod_iva        21
#define o_tot_tick_sv_importe_sin_iva          1
#define o_tot_tick_sv_imp_int                  2
#define o_tot_tick_sv_costo                    3
#define o_tot_tick_sv_recargo_tarjeta          4
#define o_tot_tick_sv_exento                   5
#define o_tot_tick_sv_iva1                     6
#define o_tot_tick_sv_iva2                     7
#define o_tot_tick_sv_ila                      8
#define o_tot_tick_sv_iha                      9
#define o_tot_tick_sv_diferencia               10
#define o_tot_tick_sv_total_en_cuotas          11
#define o_tot_tick_sv_imp_tasa                 12
#define o_tot_tick_sv_imp_sobretasa            13
#define o_tot_tick_sv_imp_neto_gravado         14
#define o_tot_tick_sv_alicuota                 15
#define o_tot_tick_sv_impuestos                16
#define o_tot_tick_sv_total_por_cod_iva        17
#define o_tot_tick_sv_iva1_por_cod_iva         18
#define o_tot_tick_sv_iva2_por_cod_iva         19
#define o_tot_tick_sv_imp_int_por_cod_iva      20
#define o_tot_tick_sv_base_por_cod_iva         21
#define o_item_cod_barra                       0
#define o_item_nombre_largo                    0
#define o_item_imp_int_rec                     0
#define o_item_depto                           0
#define o_item_precio                          0
#define o_item_envase                          0
#define o_item_dev_envase                      0
#define o_item_pesable                         0
#define o_item_gravado                         0
#define o_item_pedir_precio                    0
#define o_item_inhabilitado                    0
#define o_item_precio_sin_iva                  0
#define o_item_imp_int                         0
#define o_item_costo                           0
#define o_item_cant_present                    0
#define o_item_presentacion                    0
#define o_item_cod_presentacion                0
#define o_item_cod_especial                    0
#define o_item_cod_enganche                    0
#define o_item_recargo                         0
#define o_item_iva1_precio                     0
#define o_item_iva2_precio                     0
#define o_item_marca                           0
#define o_item_contenido                       0
#define o_item_unidad                          0
#define o_item_autoinc                         0
#define o_item_ila_precio                      0
#define o_item_ila_rec                         0
#define o_item_iha_precio                      0
#define o_item_iha_rec                         0
#define o_item_porc_ila                        0
#define o_item_porc_iha                        0
#define o_item_cod_impuesto                    0
#define o_item_iva1_rec                        0
#define o_item_iva2_rec                        0
#define o_item_cupon_dev_env                   0
#define o_item_credito                         0
#define o_item_scanner                         0
#define o_item_cod_clasificacion               0
#define o_item_stockeable                      0
#define o_item_habilitado_en_cuotas            0
#define o_item_cod_iva                         0
#define o_item_cod_movimiento                  0
#define o_item_atrib_descuentos                0
#define o_item_habilitado_en_promocion         0
#define o_item_cod_promocion                   0
#define o_item_cod_cobro                       0
#define o_item_comprobante                     0
#define o_item_descuento_cantidad              0
#define o_item_cod_solicitud                   0
#define o_item_pedir_supervisor                0
#define o_item_alicuota_precio                 0
#define o_item_hab_desc_cliente                0
#define o_item_cod_banda                       0
#define o_item_tipo_envase                     0
#define o_item_porc_ica                        0
#define o_item_descuento                       0
#define o_item_monto_impuesto                  0
#define o_item_porc_impuesto                   0
#define o_item_suma_impuesto_al_total          0
#define o_item_vta_asistida                    0
#define o_item_participa_donacion              0
#define o_item_origen                          0
#define o_item_precio_original                 0
#define o_item_cod_especial_alfa               0
#define o_item_cod_enganche_alfa               0
#define o_item_imprimir_barra                  0
#define o_item_cod_clasificacion_str           0
#define o_item_motivo_devolucion               0
#define o_stack_puntero                        0
#define o_stack_handle                         0
#define o_stack_datos                          0
#define o_mutual_cuotas                        0
#define o_mutual_codigo                        1
#define o_mutual_orden                         2
#define o_mutual_recargo                       3
#define o_conc_a_pagar_cod_fletero             0
#define o_conc_a_pagar_cod_comisionista        0
#define o_conc_a_pagar_conceptos               0
#define o_conc_a_pagar_status                  0
#define o_xc_vtas_importe                      0
#define o_xc_vtas_cantidad                     1
#define o_xc_retiros_importe                   2
#define o_xc_retiros_cantidad                  3
#define o_xc_adelantos_importe                 4
#define o_xc_adelantos_cantidad                5
#define o_xc_intereses_importe                 6
#define o_xc_intereses_cantidad                7
#define o_xc_fondo_fijo_importe                8
#define o_xc_fondo_fijo_cantidad               9
#define o_xc_anulaciones_importe               10
#define o_xc_anulaciones_cantidad              11
#define o_xc_dev_envases_importe               12
#define o_xc_dev_envases_cantidad              13
#define o_xc_devoluciones_importe              14
#define o_xc_devoluciones_cantidad             15
#define o_xc_ventas_gravadas                   16
#define o_xc_ventas_exentas                    17
#define o_xc_imp_int                           18
#define o_xc_dep_imp                           19
#define o_xc_dep_can                           20
#define o_xc_demora_segundos                   21
#define o_xc_demora_ventas                     22
#define o_xc_tot_dinero                        23
#define o_xc_iva1                              24
#define o_xc_iva2                              25
#define o_xc_retiro_fondo_importe              26
#define o_xc_retiro_fondo_cantidad             27
#define o_xc_mozos_importe                     28
#define o_xc_mozos_cantidad                    29
#define o_xc_cobros_importe                    30
#define o_xc_cobros_cantidad                   31
#define o_xc_pagos_por_caja_importe            32
#define o_xc_pagos_por_caja_cantidad           33
#define o_xc_facturas_a_importe                34
#define o_xc_facturas_a_cantidad               35
#define o_xc_facturas_b_importe                36
#define o_xc_facturas_b_cantidad               37
#define o_xc_vueltos_importe                   38
#define o_xc_vueltos_cantidad                  39
#define o_xc_bonos_entregados                  40
#define o_xc_ila                               41
#define o_xc_iha                               42
#define o_xc_diferencia                        43
#define o_xc_nc_a_importe                      44
#define o_xc_nc_a_cantidad                     45
#define o_xc_nc_b_importe                      46
#define o_xc_nc_b_cantidad                     47
#define o_xc_fondo_fijo_multiple_importe       48
#define o_xc_fondo_fijo_multiple_cantidad      49
#define o_xc_retencion_dgr                     50
#define o_xc_percepcion_dgi                    51
#define o_xc_importe_iva                       52
#define o_xc_importe_sobretasa_iva             53
#define o_xc_correcion_importe                 54
#define o_xc_correccion_cantidad               55
#define o_xc_nro_primer_ticket                 56
#define o_xc_cantidad_tickets                  57
#define o_xc_cobros_cantidad_servicios         58
#define o_xc_percepcion_212_iva1               59
#define o_xc_percepcion_212_iva2               60
#define o_xc_diferencia_redondeo               61
#define o_xc_imp_devoluciones                  62
#define o_xc_cant_devoluciones                 63
#define o_xc_cant_articulos                    64
#define o_xc_cant_art_escaneados               65
#define o_xc_tmpo_reg_articulo                 66
#define o_xc_tmpo_pago                         67
#define o_xc_tmpo_standby                      68
#define o_xc_tmpo_ocio                         69
#define o_xc_imp_cancelados                    70
#define o_xc_cant_cancelados                   71
#define o_xc_cambios_importe                   72
#define o_xc_cambios_cantidad                  73
#define o_xc_ventas_importe_me                 74
#define o_xc_fondo_fijo_multiple_imp_me        75
#define o_xc_retiros_importe_me                76
#define o_xc_donaciones_cantidad               77
#define o_xc_donaciones_importe                78
#define o_xc_alicuota                          79
#define o_xc_desc_porc_importe                 80
#define o_xc_desc_porc_cantidad                81
#define o_xc_desc_porc_ult_art_importe         82
#define o_xc_desc_porc_ult_art_cantidad        83
#define o_xc_desc_pesos_importe                84
#define o_xc_desc_pesos_cantidad               85
#define o_xc_desc_pesos_ult_art_importe        86
#define o_xc_desc_pesos_ult_art_cantidad       87
#define o_xc_desc_porc_importe_sin_iva         88
#define o_xc_desc_porc_utl_art_imp_sin_iva     89
#define o_xc_desc_pesos_importe_sin_iva        90
#define o_xc_desc_pesos_ult_art_imp_sin_iva    91
#define o_xc_cant_ticket_dev_envase            92
#define o_xc_impuestos                         93
#define o_xc_donaciones_ventas_importe         94
#define o_xc_donaciones_ventas_cantidad        95
#define o_xc_boletas_con_derecho_a_nc          96
#define o_xc_cant_boletas_con_derecho_a_nc     97
#define o_xc_devoluciones_medio                98
#define o_xc_factura_a_importe_exento          99
#define o_xc_factura_a_importe_gravado         100
#define o_xc_nc_a_importe_exento               101
#define o_xc_nc_a_importe_gravado              102
#define o_xc_cons_final_importe_exent_fact     103
#define o_xc_cons_final_importe_grav_fact      104
#define o_xc_cons_final_cantidad_fact          105
#define o_xc_dev_cons_f_importe_exent_fact     106
#define o_xc_dev_cons_f_importe_grav_fact      107
#define o_xc_dev_cons_f_cantidad_fact          108
#define o_xc_ventas_diplom_importe             109
#define o_xc_ventas_diplom_cantidad            110
#define o_xc_devoluciones_diplom_importe       111
#define o_xc_devoluciones_diplom_cantidad      112
#define o_xc_nro_transaccion_inicial           113
#define o_xc_nro_transaccion_final             114
#define o_xc_devoluciones_export_importe       115
#define o_xc_devoluciones_export_cantidad      116
#define o_xc_cons_final_importe_exent_tkt      117
#define o_xc_cons_final_importe_grav_tkt       118
#define o_xc_cons_final_cantidad_tkt           119
#define o_xc_dev_cons_f_importe_exent_tkt      120
#define o_xc_dev_cons_f_importe_grav_tkt       121
#define o_xc_dev_cons_f_cantidad_tkt           122
#define o_xc_iva_factura_a                     123
#define o_xc_iva_cons_final                    124
#define o_xc_ventas_export_importe             125
#define o_xc_ventas_export_cantidad            126
#define o_xc_oferta_importe                    127
#define o_xc_descuentos_medio                  128
#define o_xc_iva_nc_a                          129
#define o_xc_iva_devol_consumidor_final        130
#define o_xc_ventas_importe_sin_redondeo       131
#define o_xc_cobros_importe_me                 132
#define o_xc_retiro_fondo_importe_pagos        133
#define o_xc_tmpo_no_venta                     134
#define o_xc_dif_redondeo_vtas_contrib         135
#define o_xc_dif_redondeo_iva_contrib          136
#define o_xc_dif_redondeo_nc                   137
#define o_xc_dif_redondeo_iva_nc               138
#define o_xc_dif_redondeo_grav_ticket          139
#define o_xc_dif_redondeo_exent_ticket         140
#define o_xc_dif_redondeo_iva_ticket           141
#define o_xc_cobros_importes_servivios         142
#define o_sitef_sesion_iniciada                0
#define o_sitef_nid                            0
#define o_sitef_op_habilitadas                 0
#define o_sitef_fecha_debito_predatada         0
#define o_sitef_limite_inf_consulta_cheques    0
#define o_sitef_limite_debito_forzada          0
#define o_sitef_solicitar_cuotas_credito       0
#define o_sitef_timeout                        0
#define o_sitef_cuotas_debito_parcelada        0
#define o_sitef_intervalo_debito_parcela       0
#define o_sitef_1_cuota_debito_vista           0
#define o_sitef_debito_mes_cerrado             0
#define o_sitef_emite_nota_promisoria          0

/*---------------- Variables Configurables por el Usuario ----------------*/

/*------------------- Variables Internas Inicializadas -------------------*/

/*----------------- Variables Internas no Inicializadas -----------------*/
#define o_ext_impuestos                        0
#define o_ticket_tipo                          0
#define o_ticket_cantidad                      0
#define o_ticket_presentacion                  0
#define o___ticket_depto                       0
#define o_ticket_precio                        0
#define o_ticket_cod_especial                  0
#define o_ticket_imp_int                       0
#define o_ticket_costo                         0
#define o_ticket_recargo                       0
#define o_ticket_cod_present                   0
#define o_ticket_iva1_precio                   0
#define o_ticket_iva2_precio                   0
#define o_ticket_iva1_rec                      0
#define o_ticket_iva2_rec                      0
#define o_ticket_exento                        0
#define o_ticket_importe_sin_iva               0
#define o_ticket_ila_precio                    0
#define o_ticket_iha_precio                    0
#define o_ticket_ila_rec                       0
#define o_ticket_iha_rec                       0
#define o_ticket_cod_impuesto1                 0
#define o_ticket_cod_impuesto2                 0
#define o_ticket_cod_impuesto3                 0
#define o_ticket_cod_impuesto4                 0
#define o_ticket_cod_ext                       0
#define o_ticket_cod_clasificacion             0
#define o_ticket_nro_carro                     0
#define o_ticket_plazo_financiacion            0
#define o_ticket_tipo2                         0
#define o_ticket_cod_iva                       0
#define o_ticket_cod_movimiento                0
#define o_ticket_atrib_descuentos              0
#define o_ticket_origen                        0
#define o_ticket_cod_promocion                 0
#define o_ticket_cod_promocion1                0
#define o_ticket_cod_promocion2                0
#define o_ticket_cod_promocion3                0
#define o_ticket_gravado                       0
#define o_ticket_depto                         0
#define o_ticket_cod_cobro                     0
#define o_ticket_descuento_cantidad            0
#define o_ticket_cod_solicitud                 0
#define o_ticket_alicuota_precio               0
#define o_ticket_hab_desc_cliente              0
#define o_ticket_cod_barra                     0
#define o_ticket_cod_banda                     0
#define o_ticket_tipo_envase                   0
#define o_ticket_cod_barra_original            0
#define o_ticket_descuento                     0
#define o_ticket_impuesto1_precio              0
#define o_ticket_impuesto2_precio              0
#define o_ticket_impuesto3_precio              0
#define o_ticket_impuesto4_precio              0
#define o_ticket_tipo3                         0
#define o_ticket_cant_excl_desc                0
#define o_ticket_precio_original               0
#define o_ticket_cod_especial_alfa             0
#define o_ticket_cod_clasificacion_str         0
#define o_ticket_motivo_devolucion             0
