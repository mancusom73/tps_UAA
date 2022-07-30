#pragma pack (push,1)
/* STRT_RE_23_06_2011_1513*/
//1395+22+35+62=1513

#define S_CANTIDAD_GRANDE									1 /*CANTIDAD DEMASIADO GRANDE*/
#define S_ANULACION											2 /*ANULACION*/
#define S_MODO_ENTR_ABORTADO								3 /*MODO ENTRENAMIENTO ABORTADO !  YA PUEDE APAGAR LA T.P.V.*/
#define S_TERMINAL_PUNTO_VENTA								4 /*- TERMINAL DE PUNTO DE VENTA -*/
#define S_TPV1000_II										5 /****** T.P.V. - 1000 - III ******/
#define S_VERSION											6 /*Version:*/
#define S_COPYRIGHT											7 /*Copyright 1993-2005 - INVEL LATINOAMERICANA*/
#define S_DIRECCION											8 /*JJ Paso 1666 +54 351 4525757 - Cba. Arg.*/
#define S_ERROR_CONF_ARCHIVOS								9 /*ERROR AL CARGAR LA CONFIGURACION DE ARCHIVOS*/
#define S_DIA_OPERATIVO_VENCIDO								10 /*EL DIA OPERATIVO DISPONIBLE ESTA VENCIDO*/
#define S_CAMBIO_CAJERO										11 /*NO PUEDE CAMBIAR DE CAJERO SIN CERRAR LA CAJA*/
#define S_TOMANDO_ALTAS										12 /*TOMANDO ALTA DE PADRON PRINCIPAL... - ESC CANCELA*/
#define S_TOMANDO_MODIFICACIONES							13 /*TOMANDO MODIFICACIONES DE PADRON PRINCIPAL... - ESC CANCELA*/
#define S_ERROR_DRIVER_POWER								14 /*ERROR AL INICIALIZAR EL DRIVER DE POWER DOWN*/
#define S_ERROR_DRIVER_LLAVE								15 /*ERROR AL INICIALIZAR EL DRIVER DE LA LLAVE*/
#define S_ERROR_DRIVER_CAJON								16 /*ERROR AL INICIALIZAR EL DRIVER DEL CAJON*/
#define S_ERROR_SCANNER										17 /*ERROR AL INICIALIZAR EL SCANNER*/
#define S_ERROR_LECTOR_TARJ_MAG								18 /*ERROR AL INICIALIZAR EL DRIVER DEL LECTOR DE TARJETA MAGNETICA*/
#define S_ERROR_IMPRESORA									19 /*ERROR AL INICIALIZAR LA IMPRESORA*/
#define S_ERROR_TECLADO										20 /*ERROR AL INICIALIZAR EL TECLADO*/
#define S_ERROR_TCP_IP										21 /*ERROR AL INICIALIZAR EL DRIVER TCP/IP*/
#define S_ERROR_PINPAD										22 /*ERROR AL INICIALIZAR EL DRIVER DEL PINPAD*/
#define S_ERROR_DISPLAY										23 /*ERROR AL INICIALIZAR EL DRIVER DEL DISPLAY*/
#define S_NO_ENCUENTRA_BANCO								24 /*NO SE ENCONTRO EL BANCO DEL CLIENTE EN BANCOS*/
#define S_NO_HAY_BANCO_INGRESADO							25 /*NO SE HAN INGRESADO BANCOS PARA ESTE CLIENTE*/
#define S_ERROR_ABRIR_TABLA_BANCOS							26 /*ERROR AL ABRIR LA TABLA DE BANCOS*/
#define S_NUMERO_BANCO_INEXISTENTE							27 /*NUMERO DE BANCO INEXISTENTE*/
#define S_NO_HAY_CUENTAS_INGRESADAS							28 /*NO SE HAN INGRESADO CUENTAS PARA ESTE CLIENTE*/
#define S_YA_EXISTE_EL_CHEQUE								29 /*YA EXISTE ESTE CHEQUE*/
#define S_NO_PUEDE_ABRIR_CHEQUES							30 /*NO SE PUEDE ABRIR EL ARCHIVO DE CHEQUES*/
#define S_ERROR_GRABAR_COMTAR								31 /*ERROR AL GRABAR COMTAR*/
#define S_TARJ_CON_COMPRA_NO_USAR							32 /*TARJETA CON COMPRAS ANTERIORES - NO SE PUEDE UTILIZAR*/
#define S_TARJ_CON_COMPRA_AUTORIZ							33 /*TARJETA CON COMPRAS ANTERIORES - PIDA AUTORIZACION*/
#define S_NO_NUMERO_SUPERVISOR								34 /*NO ES NUMERO DE SUPERVISOR*/
#define S_NO_TARJETA_USUARIO								35 /*NO ES TARJETA DE USUARIO*/
#define S_ERROR_LEER_TARJETA								36 /*ERROR AL LEER TARJETA*/
#define S_INGRESE_CLAVE										37 /*INGRESE LA CLAVE*/
#define S_NO_DETECTO_TARJETA								38
#define S_CLAVE_INCORRECTA_RETIRE							39
#define S_OK_RETIRE											40
#define S_RETIRE_TARJETA									41
#define S_NO_DETECTO_TARJETA_RETIRE							42
#define S_NO_TARJETA_SUP_RETIRE								43
#define S_TARJETA_INCORRECTA								44
#define S_ERROR_TARJETA_BLOQUEADA							45
#define S_ERROR_ARCHIVO_CAJEROS								46
#define S_ERROR_LEER_CLAVE									47
#define S_CLAVE_INCORRECTA									48
#define S_CAJERO_NO_HABILITADO								49
#define S_INGRESE_TARJ_IDENTIFICACION						50
#define S_USUARIO_INCORRECTO_RETIRE							51
#define S_TARJETA_INCORRECTA_RETIRE							52
#define S_GRACIAS_POR_SU_COMPRA								53
#define S_ANULE_CHEQUES_CAMBIAR_CLIENTE						54
#define S_CLIENTE_EN_CHEQUES_NO_CONF						55
#define S_SOLO_UN_PAGO_TARJETA								56
#define	S_TARJETA_NO_PASADA									57
#define S_TARJETA_NO_ACEPTADA_POR_MEDIO						58
#define S_IMPORTE_EXCEDE_LIMITE_COMPRA						59
#define S_NO_INGRESO_CODIGO_FINANCIACION					60
#define S_PORCENTAJE_INTERES_EXCEDIDO						61
#define S_DEBE_HABER_PAGO_TARJETA							62
#define S_PAGO_NO_ALCANZA_ENTREGA_TARJETA					63
#define S_IMPORTE_EXCEDIDO									64
#define S_NO_EFECTIVO_PARA_VUELTO							65
#define S_TOTAL_REPROCESAR_DEBE_SER_IGUAL_PAGO_ORIGINAL		66
#define S_TOTAL_CAJA_EXCEDIDO								67
#define S_CIERRE_NO_AUTORIZADO 68
#define S_ARTICULO_NO_TICKEADO 69
#define S_NO_QUEDAN_ENVASES_DEVOLVER 70
#define S_ITEM_PRECIO_PREFIJO_DESCONOCIDO 71
#define S_LIMITE_VENTA_EXCEDIDO 72
#define S_FAVOR_SELECCIONE_CLIENTE 73
#define S_FAVOR_INGRESE_VENDEDOR 74
#define S_NO_PRESENTACIONES_HABILITADAS 75
#define S_NO_PUEDO_UTILIZAR_CANTIDAD_TIPO_PRODUCTO 76
#define S_NO_MULTIPLICAR_PESABLES 77
#define S_ARTICULO_SIN_PRECIO 78
#define S_NO_MULTIPLICAR_CON_PRECIO 79
#define S_DEBE_SER_ARTICULO_SIN_PRECIO 80
#define S_NO_DISPONIBLE_PRESENTACION1 81
#define S_QUEDAN_ARTICULOS_EN_SCANNER 82
#define S_LONGITUD_TICKET_EXCEDIDA 83
#define S_LONGITUD_FACTURA_EXCEDIDA 84
#define S_IMPORTE_TICKET_EXCEDIDO 85
#define S_ERROR_MATEMATICO 86
#define S_EXCESO_ENVASES_ENTREGADOS 87
#define S_ERROR_LECTURA_SCANNER 88
#define S_CODIGO_INEXISTENTE 89
#define S_CODIGO_INCORRECTO  90
#define S_IMPRESORA_FUERA_LINEA 91
#define S_TIENE_TICKET_ESPERA                  92
#define S_NO_REMITOS_GUIAS_FACTURAR_PERIODO    93
#define S_ERROR_ESCRIBIR_IMPRESORA_FISCAL      94
#define S_CAMBIE_ROLLO_PAPEL                   95
#define S_VERIFIQUE_IMPRESORA                  96
#define S_ERROR_ABRIR_ARCHIVO_FISC_IBM         97
#define S_IMPRESORA_OFF_LINE                   98
#define S_ERROR_IMPRESION_ENTER                99
#define S_IMPRESORA_ABIERTA                   100
#define S_OBTENIENDO_PINES_VIRTUALES          101  /*OBTENIENDO PINES VIRTUALES*/
#define S_IMPRESORA_ERROR 							 102
#define S_JOURNAL_SIN_PAPEL 						 103
#define S_JOURNAL_CERCA_FINAL 					 104
#define S_RECEIPT_SIN_PAPEL 						 105
#define S_RECEIPT_CERCA_FINAL 					 106
#define S_ERROR_ABRIENDO_CANAL 					 107
#define S_CANAL_ABIERTO  							 108
#define S_ERROR_OBTENER_PINES_VIRTUALES       109  /*ERROR AL OBTENER PINES VIRTUALES - DEBE ANULAR LOS ARTICULOS*/
#define S_FALTA_ALGUN_MEDIO_DE_PAGO_ACEPTADO_X_PROMOCION 110  /*FALTA ALGUN MEDIO DE PAGO ACEPTADO POR PROMOCION*/
#define S_ERROR_AL_VALIDAR                    111  /*ERROR AL VALIDAR*/
#define S_ERROR_ABRIR_SCRIPT_RTA 				 112
#define S_NO_SE_TOMARON_ALTAS 					 113
#define S_NO_SE_TOMARON_MODIF 					 114
#define S_AREA_TRANSAC2_CERRADA 					 115
#define S_ERROR_OPERACIONES     					 116
#define S_INGRESE_TARJETA_PRIMERO             117
#define S_COMPROBANTE_INVALIDO		          118
#define S_NRO_CTA_INVALIDO                    119
#define S_NO_MAS_SERVICIOS                    120
#define S_PAGO_NO_ENCONTRADO						 121
#define S_COMPROBANTE_VENCIDO						 122
#define S_CODIGO_INVALIDO							 123
#define S_CUENTA_DENEGADA							 124
#define S_ULTIMA_OPERACION_NO_FUE_COBRO		 125
#define S_IDENTIF_COBRO_NO_ENCONTRADO			 126
#define S_IDENTIF_COBROS_VARIOS_NO_ENC			 127
#define S_FECHA_INVALIDA							 128
#define S_CRC_EAN13_INVALIDO						 129
#define S_NO_PERMITIDO_ANULAR_COBRO_UNIPAGO   130
#define S_MAL_CONFIGURADO_COBROS					 131
#define S_ERROR_ABRIR_COBROS	   				 132
#define S_NO_SE_PUEDEN_PROCESAR_LAS_TARJETAS  133
#define S_NO_SE_PUEDEN_IMPRIMIR_LOS_VOUCHERS_DE_LAS_TARJETAS 134
#define S_ERROR_GRABANDO_PAGOS				    135
#define S_PRESIONE_ENTER_PARA_RECARGO			 136
#define S_DEBE_PAGAR_TODO_DE_CONTADO		    137
#define S_ERROR_AL_CERRAR_CANAL					 138
#define S_ESPERE_UN_MOMENTO_PARA_APAGAR_LA_TERMINAL	139
#define S_Grabando_Cierre_de_Caja_en_Servidor			140
#define S_EL_CAJON_YA_ESTA_UTILIZADO_POR_OTRO_CAJERO	141
#define S_PROBLEMA_ENVIAR_DATOS								142
#define S_SERVER_FUERA_LINEA                          143
#define S_TPV_FUERA_LINEA                             144
#define S_SERVER_NO_ATIENDE_DISP                      145
#define S_SERVER_NO_ATIENDE_ATEN                      146
#define S_SERVER_NO_RESPONDIO                         147
#define S_PROBLEMA_LEER_RESPUEST                      148
#define S_NO_PUDO_ABRIR_TARJ_OFF                      149
#define S_ABRIENDO_ARCH_ENTRENAM								150
#define S_ARCHIVOS_ABIERTOS                           151
#define S_ERROR_EN_LA_RED										152
#define S_IMPRIMIENDO_Y                               153
#define S_IMPRIMIENDO_TARJETAS								154
#define S_PASE_LA_TARJETA                             155
#define S_MARCA_TARJ_NO_DETECTAD                      156
#define S_TERM_NO_HAB_TRANS_ONLI                      157
#define S_VERIF_DATOS_TARJETA                         158
#define S_NRO_TARJ_INVALIDO                           159
#define S_DIFERIM_EXCEDE_CANTIDA                      160
#define S_NO_DIFERIM_FEC_ACTUAL                       161
#define S_TARJ_NO_HAB_DIFERIR                         162
#define S_VERIFIQUE_LOS_DATOS                         163
#define S_TARJETA_VENCIDA                             164
#define S_FEC_VIGENCIA_INVALIDA                       165
#define S_INGRESE_NUEV_4_DIGITOS                      166
#define S_BOLETIN_VENCIDO                             167
#define S_TARJ_EN_BOL_PROTECTIVO                      168
#define S_ERROR_EN_BOL_PROTECTIV                      169
#define S_NO_ELIJIO_DIFERIMIENTO                      170
#define S_ERROR_CARGAR_ARCHIVOS                       171
#define S_ERROR_CANT_TKT_ESPERA                       172
#define S_ABORTAR_CARGA_FACCFG                        173
#define S_ABORTAR_CARGA_REMCFG                        174
#define S_ABORTAR_CARGA_VALIDA                        175
#define S_ABORTAR_CARGA_TKTCFG                        176
#define S_PROCESANDO_SCRIPT                           177
#define S_TARJETA_OK                                  178
#define S_Articulo_no_Hablilidato_para_Venta_de_Bienes_de_Uso	179
#define S_NO_PUEDE_FINALIZAR_LA_OPERACION             180
#define S_CIERRE_DE_LOTE                              181
#define S_CODIGO_INEXISTENTE_Presione_ESC             182
#define S_DIGITO_VERIFICADOR_INVALIDO                 183
#define S_CAJERO_CON_ENTRADAS_ANTERIORES					184
#define S_ERROR_DE_APERTURA_NO_SE_PUEDE_CONTROLAR		185
#define S_ERROR_OBTENIENDO_TABLA_NO_SE_PUEDE_CONTROLAR 186
#define S_NO_ESTA_AUTORIZADO_PARA_HACER_RETIROS       187
#define S_NO_HA_RETIRADO_TANTO_FONDO_FIJO					188
#define S_NO_HA_RETIRADO_TANTO_EFECTIVO					189
#define S_FFIJO													190
#define S_IMPORTE													191
#define S_COMPROBANTE_NO_ENCONTRADO							192
#define S_RETIROS													193
#define S_PRESIONE_ENTER_PARA_ACEPTAR_EL_FONDO_FIJO	194
#define S_TOTAL													195
#define S_AUTORIZACION                                196
#define S_TICKET_PASADO_A_ESPERA_presione_enter     	197
#define S_TICKET_ANULADO_presione_enter             	198
#define S_COBROS													199
#define S_COBRO_ANULADO											200
#define S_Comprobante											201
#define S_N_Cuenta												202
#define S_Importe													203
#define S_COBROS_A_REVALIDAR									204
#define S_FALTANTE												205
#define S_SOBRANTE												206
#define S_VUELTO													207
#define S_VUELTO_EN												208
#define S_CUIT														209
#define S_NRO_SUCURSAL											210
#define S_NRO_TICKET												211
#define S_COPIA_TICKET											212
#define S_IMPORTAR_TICKET										213
#define S_DEBE_FINALIZAR_EL_TICKET							214
#define S_PUEDE_REALIZAR_SOLO_UN_DESCUENTO_POR_TICKET	215
#define S_DESCUENTO_NO_REALIZADO_EN_EL_TICKET			216
#define S_EL_DESCUENTO_SERA_APLICADO_AL_FINAL_DEL_TIC 217
#define S_MAXIMO_DE_TICKET_EN_ESPERA						218
#define S_TICKET_ESPERA											219
#define S_No_hay_ticket_en_espera_para_recuperar		220
#define S_RECUPERA_TICKET_EN_ESPERA							221
#define S_AUTORIZACION_DE_TICKET_EN_ESPERA				222
#define S_TICKET_NO_ENCONTRADO								223
#define S_EL_EVENTO_NO_ES_DEL_TIPO_REQUERIDO				224
#define S_EL_NUMERO_DE_TICKET_ES_INVALIDO					225
#define S_LA_FECHA_DEL_TICKET_ES_INVALIDA					226
#define S_TICKET													227
#define S_CAMBIO_MEDIOS											228
#define S_CAMBIO_ANULADO  									   229
#define S_TICKET_YA_IMPORTADO                         230
#define S_DEBE_SELECCIONAR_EL_CLIENTE_AL_COMIENZO     231
#define S_EL_PRECIO_DE_LA_COMPETENCIA_DEBE_SER_MENOR  232
#define S_DRIVER_OMRON_NO_PRESENTE                    233
#define S_PUNTO_DE_VENTA_OMRON_RS_7000                234
#define S_PUNTO_DE_VENTA_TEC_RE_3500                  235
#define S_PUNTO_DE_VENTA_TEC_ST_5000                  236
#define S_PUNTO_DE_VENTA_TPV_1000_II                  237
#define S_CANTIDAD_DE_TICKET_EN_ESPERA_ERROR          238
#define S_NO_PUDO_GRABAR_ENTRADA_CAJERO               239
#define S_MEDIO_DE_INGRESO_INVALIDO                   240
#define S_SALGA_PRIMERO_DE_MODO_ENTRENAMIENTO         241
#define S_EL_CAJERO_YA_ESTA_HABILITADO_EN_OTRA_CAJA   242
#define S_Presione_9_para_CONTINUAR_o_ESC_para_SALIR 243
#define S_CONFIRMA_LA_EMISION_DE_LA_Z                  244
#define S_ERROR_AL_CARGAR_CONF_DE_FACTURAS             245
#define S_ERROR_AL_CARGAR_CONF_DE_REMITOS              246
#define S_ERROR_AL_CARGAR_CONF_DE_VALIDACIONES         247
#define S_CONFIRMA_LA_IMPRESION_DEL_CHEQUE_DE          248
#define S_EL_COMP_ORIGINAL_NO_PERTENECE_A_LA_SUCURSAL  249
#define S_ERROR_AL_ABRIR_CONTROL_NC                    250
#define S_ERROR_EN_EVENTOS_TEMPORALES                  251
#define S_LA_CANTIDAD_SUPERA_EL_TIKET_ORIGINAL         252
#define S_ARTICULO_NO_VENDIDO_EN_EL_TIKET_ORIGINAL     253
#define S_TIPO_DE_COMP_ANTERIOR                        254
#define S_NRO_COMPROBANTE_ANTERIOR                     255
#define S_SUCURSAL_ANTERIOR                            256
#define S_CAJA_ANTERIOR                                257
#define S_FECHA_ANTERIOR                               258
#define S_NO_SE_ENCUENTRA_EN_BOLETIN                   259
#define S_EL_MONTO_INDICADO_NO_CORRESPONDE_CON_EL_BONO 260
#define S_ESE_BONO_YA_HA_SIDO_UTILIZADO                261
#define S_ERROR_AL_ABRIR_BOLETIN                       262
#define S_NOVEDAD_DE_BOLETIN_NO_RECONOCIDA             263
#define S_1_AUTORIZACION_OFF_LINE_2_ENVIAR_TRANSACCION_ON_LINE 264
#define S_ERROR_AL_ABRIR_CONFIGURACION_ON_LINE         265
#define S_BACKUP_EVENTOS                               266
#define S_VA_A_VACIAR_EVENTOS                          267
#define S_VACIO_EVENTOS                                268
#define S_INGRESE_EL_COMPROBANTE_DE                    269
#define S_VALIDACION_TELEFONICA_ENVIADA_PRESIONE_ENTER 270
#define S_VALIDACION_TELEFONICA_ERROR_PRESIONE_ENTER   271
#define S_ERROR_DE_COMUNICACION                        272
#define S_DEBE_IDENTIFICAR_EL_CLIENTE_ESC_RECHAZA      273
#define S_DEBE_SALIR_DE_MODO_DEVOLUCION                274
#define S_ARTICULO_DE_DESCARGA_INEXISTE                275
#define S_ERROR_FATAL_LLAME_AL_SERVICIO_TECNICO        276
#define S_DEBE_IDENTIFICAR_AL_CLIENTE                  277
#define S_DESEA_EMPADRONAR_EL_CLIENTE_ENTER_SI         278
#define S_NUMERO_CLIENTE                               279
#define S_NOMBRE_CLIENTE                               280
#define S_TELEFONO_CLIENTE                             281
#define S_CLIENTE_YA_REGISTRADO                        282
#define S_CLIENTE_REGISTRADO_EXITOSAMENTE              283
#define S_EROR_AL_REGISTRAR_CLIENTE                    284
#define S_CONFIRMA_LOS_DATOS_DEL_CLIENTE_ENTER_SI      285
#define S_DESEA_HACER_UNA_DONACION_ENTER_ESC           286
#define S_MONTO_DONACION_SUGERIDO                      287
#define S_MONTO_DONACION                               288
#define S_LA_DONACION_NO_PUEDE_SER_MAYOR_QUE_EL_VUELTO 289
#define S_DOMICILIO_CLIENTE                            290
#define S_COMPROBANTE_SOLICITADO_NO_ENCONTRADO         291
#define S_COBRANZA                                     292
#define S_MEDIO_EN_BOLETIN                             293
#define S_MODO_TRANSFERENCIAS                          294
#define S_LINEA                                        295
#define S_ERROR_EN_NRO_LEGAJO                          296
#define S_DEBE_IDENTIFICAR_EL_EMPLEADO                 297
#define S_DESCUENTO_YA_APLICADO                        298
#define S_NO_SE_ACEPTAN_DOS_DESCUENTOS_DEL_MISMO_GRUPO 299
#define S_NO_HAY_ARTICULOS_HABILITADOS_PARA_DICHO_DESCUENTO 300
#define S_DESCUENTO_A_ANULAR_NO_ENCONTRADO             301
#define S_EL_IMPORTE_SUPERA_EL_MONTO_ORIGINAL          302
#define S_NO_SE_PUEDE_APLICAR_EL_DESCUENTO             303
#define S_DESBORDE_EN_ARRAY_NRO_EVENTO                 304
#define S_ERROR_AL_CARGAR_ARRAY_NRO_EVENTOS            305
#define S_LA_ULTIMA_TRANSACCION_NO_FUE_COBRO           306
#define S_ANUL_COBROS                                  307
#define S_ANUL_COBRANZA                                308
#define S_AUTORIZACION_ARTICULO                        309
#define S_PERCEPCION_DE_IVA								    310
#define S_CONFIRMA_LA_ANULACION_DEL_COBRO_ENTER_ESC     311
#define S_ERROR_ARCHIVO_DET_TRAN_BTR                   312
#define S_ERROR_AREA_1                                 313
#define S_HOY_CUMPLE_ANIOS 									 314
#define S_PORCENTAJE_DE_DESCUENTO_EXCEDIDO 				 315
#define S_ERROR_EN_DIGITO_VERIFICADOR_DEL_RUT 			 316
#define S_EL_CLIENTE_NO_PERTENECE_A_ESA_INSTITUCION    317
#define S_RECUERDE_SOLICITAR_DEVOLUCION_DE_ENVASES     318
#define S_INICIALIZANDO_IMPRESORA_FISCAL               319
#define S_ERROR_AL_ACTUALIZAR_TABLA_ENVASES				 320
#define S_ERROR_EN_AREA_AUXILIAR_ENVASES					 321
#define S_ERROR_EN_AREA_ENVASES								 322
#define S_TICKET_DE_ENVASES_CERRADO							 323
#define S_TICKET_DE_ENVASES_NO_ENCONTRADO					 324
#define S_TICKET_DE_ENVASES_INVALIDO 						 325
#define S_EL_LIMITE_DE_T_DE_ENVASES_POR_TICKET_ES_5	 326
#define S_NO_PUEDE_PASAR_T_ENVASE_SIN_TICKEAR_ITEM		 327
#define S_GRABANDO_EN_RED										 328
#define S_COLOQUE_FORMULARIO_EN_IMPRESORA_PRES_ENTER	 329
#define S_COLOQUE_EL_FORMULARIO_EN_LA_IMPRESORA			 330
#define S_BUSCANDO_EN_BOLETIN									 331
#define S_CARGANDO_BOLETINES_NUEVOS_ESPERE_UN_MOMENTO	 332
#define S_ACTUALIZANDO_BOLETINES_PRES_ESC_PARA_ANULAR  333
#define S_OBTENIENDO_COMPLETO_DE_BOLETINES				 334
#define S_GRABANDO_CHEQUES_EN_RED							 335
#define S_CLIENTE_NO_HABILITADO_PARA_TRABAJAR_CHEQUES	 336
#define S_CLIENTE_NO_HABILITADO_PARA_CHEQUES_ENTER		 337
#define S_CHEQUE_VENCIDO_NO_ACEPTADO					    338
#define S_CHEQUE_NO_AUTORIZADO_LLAME_SUPERVISOR_ENTER	 339
#define S_CHEQUE_OK												 340
#define S_INGRESE_CHEQUE_A_LEER_Y_PRESS_ENTER			 341
#define S_PRESIONE_ENTER_PARA_IMPRIMIR_EL_CHEQUE		 342
#define S_ERROR_EN_EL_LECTOR_DE_CHEQUES					 343
#define S_NO_ESTA_HABILITADA_FACTURACION_CHEQUES_CLIE	 344
#define S_ERROR_AL_ABRIR_LA_TABLA							 345
#define S_NO_SE_PUEDEN_PROCESAR_MAS_TARJETAS				 346
#define S_NO_HAY_MEMORIA_PARA_TARJETAS_PRESIONE_ESC 	 347
#define S_CAMBIO_DE_TARJETA_NO_ACEPTADO					 348
#define S_ESPERANDO_ATENCION_DEL_SERVER					 349
#define S_ATENDIDO_ESPERANDO_RESPUESTA							350
#define S_PRESIONE_UNA_TECLA										351
#define S_PRESIONE_ENTER_PARA_CONTINUAR						352
#define S_PRESIONE_TECLA_CONSULTA								353
#define S_FUNCION_NO_HABILITADA_EN_MODO_ENTRENAMIENTO		354
#define S_PREPARANDO_MODO_ENTRENAMIENTO						355
#define S_DEBE_INGRESAR_EL_NUMERO_DE_SERIE_DEL_BILLETE 	356
#define S_NO_SE_PUDO_REGISTRAR_EL_BILLETE						357
#define S_DEBE_INGRESAR_COMPROBANTE_EN_ESTACION_DOCUMEN	358
#define S_NO_ES_CLIENTE_AFINIDAD									359
#define S_EL_CLIENTE_NO_EXISTE									360
#define S_CLIENTE_BLOQUEADO										361
#define S_CLIENTE_NO_HABILITADO									362
#define S_CLIENTE_ADICIONAL_BLOQUEADO							363
#define S_CLIENTE_NO_HABILITADO_PARA_CUENTA_CORRIENTE		364
#define S_ACTUALIZANDO_CLIENTES									365
#define S_OBTENIENDO_COMPLETO_DE_CLIENTES						366
#define S_CLIENTE_OK													367
#define S_EL_CLIENTE_DEBE_INGRESAR_SU_PIN						368
#define S_CLIENTE_DEBE_PRES_ENTER_PINPAD_PARA_CONFIRMAR	369
#define S_NO_ESTA_HABILITADA_LA_FACTURACION					370
#define S_ERROR_AL_LEER_DATOS_PARA_MOSTRAR_NRO_FACTURA	371
#define S_IMPRIMIENDO_EL_INFORME_DE_VENTAS_Y_RETIROS		372
#define S_INFORME_Y_NO_ENCONTRADO								373
#define S_IMPRIMIENDO_EL_INFORME_15								374
#define S_IMPRIMIENDO_INFORME_DE_VENTAS_POR_DEPARTAMENTO	375
#define S_IMPRIMIENDO_INFORME_PRODUCTIVIDAD_DE_CAJEROS	376
#define S_NO_SE_PERMITE_EMITIR_Z_LOCAL							377
#define S_Z_NO_ENCONTRADA											378
#define S_Z_YA_PROCESADA											379
#define S_ENVIANDO_EVENTOS_Z_ANTERIORES						380
#define S_Y_NO_ENCONTRADA											381
#define S_Y_YA_PROCESADA											382
#define S_NUMERO_DE_AUTORIZACION_INVALIDO						383
#define S_SOLICITANDO_AUTORIZACION								384
#define S_ERROR_AL_ABRIR_AUTORIZADOR							385
#define S_DEBE_AUTORIZAR_LAS_ANULACIONES						386
#define S_NO_SE_PUEDEN_HACER_LAS_ANULACIONES					387
#define S_ANULA_MODALIDAD_BIEN_DE_USO							388
#define S_MODALIDAD_BIEN_DE_USO									389
#define S_PRES_ENTER_PARA_CERRAR_CAJEROS_ESC_PARA_ANULAR	390
#define S_CERRANDO_CAJA												391
#define S_NO_PUDO_INSERTAR_EN_RENDICION_DE_CAJA				392
#define S_DEBE_HACER_EL_CIERRE_DE_CAJA_PRES_ESC				393
#define S_ARQUEO_DE_CAJERO											394
#define S_ARQUEO_ABORTADO											395
#define S_ENTER_PARA_ABORTAR_ARQUEO_ESC_RETORNAR_ARQUEO	396
#define S_PRES_ENTER_ACEPTAR_EL_ARQUEO							397
#define S_NO_HAY_CAJONES_LIBRES_PARA_UN_NUEVO_CAJERO		398
#define S_CIERRE_EL_CAJON											399
#define S_ELIJA_NUMERO_CAJON_A_CERRAR_O_PRES_ESC			400
#define S_ERROR_AL_RECUPERAR_CAJERO                      401
#define S_ERROR_GRABANDO_INGRESO_CAJERO						402
#define S_NRO_DE_CUENTA_INVALIDO									403
#define S_CODIGO_NO_ASIGNADO										404
#define S_COD_IVA_INVALIDO											405
#define S_INGRESE_COMPROBANTE_FONDO_FIJO_ENTER				406
#define S_INGRESE_COMPROBANTE_RETIRO_ENTER					407
#define S_MEMORIA_INSUFICIENTE_PARA_EMITIR_COMPROBANTE	408
#define S_ERROR_LEER_DATOS_PARA_EMITIR_COMPROBANTE			409
#define S_ERROR_LEER_DATOS_PARA_REIMPRIMIR_COMPROBANTE	410
#define S_NO_SE_ENCUENTRAN_DATOS_PARA_EMITIR_COMPROBANTE	411
#define S_OBTENIENDO_ARCHIVO_DE_SERVER_FTP					412
#define S_ENVIANDO_ARCHVIO_AL_SERVER_FTP						413
#define S_ERROR_LLENAR_ARRAY										414
#define S_PORC_DE_DESCUENTO_INVALIDO							415
#define S_ERROR_DE_LECTURA_ENTER_REINTENTAR_ESC_CANCELAR	416
#define S_DEBE_ESPECIFICAR_LA_MINIMA_DENOMINACION			417
#define S_NUMERO_DE_VENDEDOR_INVALIDO							418
#define S_NO_HAY_VENTAS_PENDIENTES								419
#define S_ACTUALIZANDO_VENTAS										420
#define S_TERMINO_ACTUALIZAR_VENTAS								421
#define S_NUMERO_VENTA_INEXISTENTE								422
#define S_VENTA_NO_CERRADA											423
#define S_CIERRE_OFF_LINE											424
#define S_OFFSET_INVALIDO											425
#define S_ENVIANDO_EVENTOS_OFF_LINE								426
#define S_CIERRE_ABORTADO											427
#define S_ERROR_AL_OBTENER_TABLA_EN_DBROUTER					428
#define S_LEYENDO_DATOS												429
#define S_ENVIANDO_TRANSACCION									430
#define S_LA_TRANSAC_ESPECIF_NO_EXISTE       				431
#define S_NO_SE_PUDO_ACTUALIZAR									432
#define S_SELECCIONE_SUB_MEDIO									433
#define S_RECONECTANDO												434
#define S_RECONECCION_OK											435
#define S_ERROR_AL_INICIALIZAR_RECUPERO_DE_TICKET			436
#define S_LOS_EVENTOS_NO_SON_CONSISTENTES						437
#define S_TOMANDO_PADRON_DE_ARTICULOS							438
#define S_ERROR_TABLA_NO_OBTENIDA								439
#define S_TABLA_INSERTADA_ERROR									440
#define S_PULSE_ENTER_PARA_CORTE_DE_PAPEL						441
#define S_CORTE_EL_PAPEL_Y_PRESIONE_ENTER						442
#define S_YA_SE_PUEDE_APAGAR_LA_TERMINAL_GRACIAS			443
#define S_SOFTWARE_SIN_LICENCIA									444
#define S_NO_SE_PUDO_RESETEAR_APAGUE_EL_POST					445
#define S_LEYENDO_BALANZA											446
#define S_AREA5_USED													447
#define S_AREA_INVALIDA												448
#define S_NO_HAY_MEMORIA_PARA_COPIAR_ARCHIVO					449
#define S_ERROR_AL_COPIAR_ARCHIVO								450
#define S_ERROR_AL_CREAR_DESTINO									451
#define S_ERROR_AL_ABRIR_ORIGEN									452
#define S_UTILIZE_EL_PINPAD										453
#define S_POWER_DOWN													454
#define S_EL_NOMBRE_NO_FUE_LEIDO									455
#define S_NO_ES_POSIBLE_REIMPRIMIR_EL_VOUCHER				456
#define S_NO_HAY_EFECTIVO_SUFICIENTE							457
#define S_INGRESE_LOS_ULTIMOS_4_DIGITOS_DE_LA_TARJETA		458
#define S_UNICAMENTE_ON_LINE                             459
#define S_ESTE_ARTICULO_NO_ADMITE_SER_MULTIPLICADO       460
#define S_ERROR_GRABANDO_ITEM                            461
#define S_SIN_RED_NO_SE_PROCESARON_TICKETS_DEV_ENVASES   462
#define S_EL_IMPORTE_EXCEDE_MAXIMO_PERMITIDO             463
#define S_EL_IMPORTE_NO_LLEGA_MINIMO_EXIGIDO             464
#define S_SOLICITANDO_WORKING_KEY                        465
#define S_ERROR_AL_SOLICITAR_WORKING_KEY                 466
#define S_DEBE_ANULAR_EL_ITEM_INVALIDO_PARA_CONTINUAR    467
#define S_NUMERO_DE_RUT_ERRONEO                          468
#define S_ERROR_EN_SUBINDICE                             469
#define S_ERROR_SUBINDICE_PROMO                          470
#define S_ERROR_GRABANDO_EV_DPAGO_BTR                    471
#define S_INGRESE_EL_CHEQUE_POR_LA_IMPRESORA             472
#define S_ERROR_EN_ARCHIVO_E_DESPRO_BTR                  473
#define S_EL_CLIENTE_YA_LLEVO_SU_CUPON                   474
#define S_EL_CLIENTE_YA_LLEVO_SUS_PUNTOS                 475
#define S_ERROR_AL_CARGAR_DATOS_MEDIOS                   476
#define S_NO_HAY_MEMORIA_SUFICIENTE_PARA_ON_LINE_MEDIOS  477
#define S_ERROR_AL_OBTENER_DATOS_MEDIOS                  478
#define S_EL_NUMERO_DE_CAJA_NO_COINCIDE                  479
#define S_EL_NUMERO_DE_SECUENCIA_ES_INVALIDO             480
#define S_NRO_SECUENCIA_ANTERIOR                         481
#define S_NOVIOS                                         482
#define S_CLIENTE_AFINIDAD                               483
#define S_NRO_DOCUMENTO_CLIENTE                          484
#define S_NRO_COMPROBANTE                                485
#define S_FECHA_INICIAL                                  486
#define S_FECHA_FINAL                                    487
#define S_QUEDA_POCO_PAPEL_EN_IMPRESORA                  488
#define S_LA_TAPA_DE_LA_IMPRESORA_ESTA_ABIERTA           489
#define S_ENTER_CONFIRMAR_ESC_INGRESAR_NRO_DE_FACTURA    490
#define S_CAJERO_INVALIDO                                491
#define S_DEBE_IDENTIFICAR_COMPROBANTE_ORIGINAL          492
#define S_ANTES_DE_ENVIAR_PAQUETE                        493
#define S_DESPUES_DE_ENVIAR_PAQUETE                      494
#define S_ANTES_DE_CERRAR_CANAL                          495
#define S_DESPUES_DE_CERRAR_CANAL                        496
#define S_BACKUP_TRANSAC2                                497
#define S_ERROR_EN_TRANSACCION                           498
#define S_NO_ES_TITULAR                                  499
#define S_NO_ES_ADICIONAL                                500
#define S_ERROR_COMANDO_VALIDACION                       501
#define S_ERROR_EN_NRO_DOC_FISCAL                        502
#define S_SE_PERDIO_CONECC_AL_ACT_VTAS_RECONECC_FALLO    503
#define S_FUNCION_144_DIFERENCIA_DE_PRECIO               504
#define S_FUNCION_145_COMPROMISO_200                     505
#define S_FUNCION_146_COMPROMISO_PRECIO_MAS_BAJO         506
#define S_AUTORIZACION_ON_LINE_ERROR                     507
#define S_AUTORIZACION_ON_LINE_OK                        508
#define S_AUTORIZACION_OFF_LINE_OK                       509
#define S_EL_BONO_HA_VENCIDO                             510
#define S_DEBE_AUTORIZAR_LOS_PRECIOS                     511
#define S_LA_OPERACION_SOLO_PUEDE_SER_ON_LINE  512
#define S_TARJETA_NO_PERMITIDA_EN_NOTA_DE_CREDITO  513
#define S_TARJETA_NO_PERMITIDA_EN_NC      514
#define S_CADENA_REIMPRESION              515
#define S_INGRESO_MANUAL_TARJETA_NO_PERM  516
#define S_NO_SE_PERMITEN_DOS_TAR_DEB      517
#define S_EXISTEN_TAR_DEB_INGRESADAS      518
#define S_BANCOS                                519  /*Bancos*/
#define S__NUMERO_DE_BANCO_                     520  /* NUMERO DE BANCO */
#define S_CUENTAS                               521  /*Cuentas*/
#define S__YA_EXISTE_EL_CHEQUE__LD              522  /* YA EXISTE EL CHEQUE %ld*/
#define S__YA_FUE_INGRESADO_EL_CHEQUE__LD       523  /* YA FUE INGRESADO EL CHEQUE %ld*/
#define S_ESTABLEC_______                       524  /*Establec.     :*/
#define S_TIPO___________                       525  /*Tipo          :*/
#define S_TARJETA_DE_DEBITO                     526  /*TARJETA DE DEBITO*/
#define S_TARJETA_DE_CREDITO                    527  /*TARJETA DE CREDITO*/
#define S_TARJETA________                       528  /*Tarjeta       :*/
#define S_VENCIMIENTO____                       529  /*Vencimiento   :*/
#define S_CUP_N_N________                       530  /*Cup¢n N§      :*/
#define S_COMPROBANTE_N__                       531  /*Comprobante N§:*/
#define S_FECHA__________                       532  /*Fecha         :*/
#define S_C_DIGO_DE_AUT__                       533  /*C¢digo de Aut.:*/
#define S_OPERACI_N______                       534  /*Operaci¢n     :*/
#define S_IMPORTE________                       535  /*Importe       :*/
#define S_ENTREGA________                       536  /*Entrega       :*/
#define S_SALDO__________                       537  /*Saldo         :*/
#define S_CANT_DE_CUOTAS_                       538  /*Cant de cuotas:*/
#define S_IMPORTE_CUOTAS_                       539  /*Importe cuotas:*/
#define S_PRIMER_VTO_____                       540  /*Primer Vto.   :*/
#define S_TIPO_DE_CUENTA_                       541  /*Tipo de cuenta:*/
#define S_MONEDA_________                       542  /*Moneda        :*/
#define S_N__DE_TABLA____                       543  /*N§ de Tabla   :*/
#define S_VIGENCIA                              544  /*Vigencia*/
#define S_TERMINAL_N_____                       545  /*Terminal N§   :*/
#define S_LOTE_____N_____                       546  /*Lote     N§   :*/
#define S_FIRMA_____                            547  /*Firma    :*/
#define S_NOMBRE____                            548  /*Nombre   :*/
#define S_DOCUMENTO_                            549  /*Documento:*/
#define S_DIRECCI_N_                            550  /*Direcci¢n:*/
#define S_TEL_FONO__                            551  /*Tel‚fono :*/
#define S_EL_COMERCIANTE_RECONOCE_LA_ANULACI_N  552  /*El comerciante reconoce la anulaci¢n*/
#define S_EL_COMERCIANTE_RECONOCE_LA_DEVOLUCI_N 553  /*El comerciante reconoce la devoluci¢n*/
#define S_DE_LA_OPERACI_N_POR_EL_IMPORTE_TOTAL  554  /*de la operaci¢n por el importe total*/
#define S_PRECEDENTEMENTE_CONSIGNADO_           555  /*precedentemente consignado.*/
#define S_SE_AUTORIZA_EL_PAGO_DEL_TICKET_       556  /*Se autoriza el pago del ticket.*/
#define S_PARA_EL_COMERCIO_                     557  /*Para el comercio.*/
#define S_PARA_EL_CLIENTE_                      558  /*Para el cliente.*/
#define S_CUENTA_N_______                       559  /*Cuenta N§     :*/
#define S_FIRMA___                              560  /*Firma  :*/
#define S_NOMBRE__                              561  /*Nombre :*/
#define S_A_N_U_L_A_C_I_O_N                     562  /*A N U L A C I O N*/
#define S_DEL_COMPROBANTE_N__                   563  /*Del Comprobante N§:*/
#define S_C_U_I_T________                       564  /*C.U.I.T.      :*/
#define S_T_P_V____N_____                       565  /*T.P.V.   N§   :*/
#define S_SUCURSAL_N_____                       566  /*Sucursal N§   :*/
#define S_OPERADOR_N_____                       567  /*Operador N§   :*/
#define S_HORA___________                       568  /*Hora          :*/
#define S_FACTURA__N_____                       569  /*Factura  N§   :*/
#define S_TICKET___N_____                       570  /*Ticket   N§   :*/
#define S_ABONO____                             571  /*Abono   :*/
#define S_NUMERO_REMITO__                       572  /*Numero Remito :*/
#define S_CODIGO_DE_IVA_NO_PERMITIDO            573  /*codigo de iva no permitido*/
#define S_TARJETA_REPETIDA___I____S_            574  /*Tarjeta repetida (%i) (%s)*/
#define S_ADELANTOS                             575  /*ADELANTOS*/
#define S_NOTA_DE_CREDITO                       576  /*NOTA DE CREDITO*/
#define S_DEVOLUCIONES                          577  /*DEVOLUCIONES*/
#define S_CAJERO_N___                           578  /*Cajero N§ :*/
#define S_NOMBRE_____                           579  /*Nombre    :*/
#define S_DOCUMENTO__                           580  /*Documento :*/
#define S_FIRMA_CAJERO_________FIRMA_SUPERVISOR 581  /*Firma Cajero         Firma Supervisor*/
#define S_FECHA_____________                    582  /*Fecha ............*/
#define S_HORA______________                    583  /*Hora .............*/
#define S_SUCURSAL__________                    584  /*Sucursal .........*/
#define S_CAJA______________                    585  /*Caja .............*/
#define S_CAJERO____________                    586  /*Cajero ...........*/
#define S_ULTIMA_BOLETA_____                    587  /*Ultima boleta ....*/
#define S_ULTIMO_TICKET_____                    588  /*Ultimo ticket ....*/
#define S_D_A_OPERATIVO_____                    589  /*D¡a Operativo ....*/
#define S__1__INFORME_00_                       590  /* 1- INFORME 00 */
#define S__2__INFORME_05_                       591  /* 2- INFORME 05 */
#define S__3__INFORME_10_                       592  /* 3- INFORME 10 */
#define S__4__INFORME_15_                       593  /* 4- INFORME 15 */
#define S__5__CON__CAJERO                       594  /* 5- CON. CAJERO*/
#define S__1__INFORMES_Z_                       595  /* 1- INFORMES Z */
#define S__9__REIMPRIMIR_                       596  /* 9- REIMPRIMIR */
#define S__R__REIMPRIMIR_                       597  /* R- REIMPRIMIR */
#define S_ERROR_CONTROLA_ANTERIORES_SERVER      598  /*Error Controla Anteriores Server*/
#define S_ERROR_ENVIANDO_EVENTOS_OFF_LINE_AL_SERVER 599  /*Error Enviando Eventos OFF-Line al Server*/
#define S_INFORME_05                            600  /*INFORME 05*/
#define S_HORA_____PROMEDIO                     601  /*Hora     Promedio*/
#define S_SUBTOTAL_NO_DISCRIM_                  602  /*SUBTOTAL NO DISCRIM.*/
#define S_DESCONOCIDO                           603  /*Desconocido*/
#define S_INFORME_10                            604  /*INFORME 10*/
#define S_CANT__TICKETS_                        605  /*CANT. TICKETS:*/
#define S_TOTALES                               606  /*TOTALES*/
#define S_ERROR_AL_CARGAR_RANGOS                607  /*Error al cargar RANGOS*/
#define S_ERROR_AL_CARGAR_TARJETAS              608  /*Error al cargar TARJETAS*/
#define S_NO_HAY_MEMORIA_PARA_CARGAR_BOLETINES_PROTECTIVOS 609  /*No hay memoria para cargar boletines protectivos*/
#define S_ERROR_AL_CARGAR_LISTA1                610  /*Error al cargar LISTA1*/
#define S_ERROR_AL_CARGAR_LISTA2                611  /*Error al cargar LISTA2*/
#define S_______GRACIAS_______                  612  /*      GRACIAS       */
#define S____POR_SU_COMPRA____                  613  /*   POR SU COMPRA    */
#define S__AUTORIZACION_VUELTO_ME_              614  /* AUTORIZACION VUELTO ME */
#define S_AUTORIZ__VUELTO_MONEDA_EXT_           615  /*Autoriz¢ vuelto moneda ext.*/
#define S__RUT_                                 616  /* RUT */
#define S__IMPORTE_                             617  /* IMPORTE */
#define S_CH_LEIDO___S__S__LI__D                618  /*CH.LEIDO: %s %s %li %d*/
#define S_CH__S__S__LI__D__S                    619  /*CH:%s %s %li %d %s*/
#define S_AUTORIZAR_PAGO                        620  /*AUTORIZAR PAGO*/
#define S__INTERESES_                           621  /* INTERESES */
#define S_DATOS__D_DE__D                        622  /*Datos %d de %d*/
#define S_INTENTO_REPROCESO_CONFIRMADO          623  /*INTENTO REPROCESO CONFIRMADO*/
#define S_ARTICULO_VARIACION_NO_ENCONTRADO____LD 624  /*ARTICULO VARIACION NO ENCONTRADO : %ld*/
#define S___BASE___2LF_                         625  /* (base %.2lf)*/
#define S_NO_HAY__S_PARA_VUELTO                 626  /*NO HAY %s PARA VUELTO*/
#define S_EL_VUELTO_EN__S_NO_PUEDE_EXCEDER__10_2F 627  /*EL VUELTO EN %s NO PUEDE EXCEDER %10.2f*/
#define S_NO_PUEDE_PAGAR_MAS_DEL__2_2F____CON__S 628  /*NO PUEDE PAGAR MAS DEL %2.2f %% CON %s*/
#define S_DEBE_PAGAR_AL_MENOS__5_2LF_DE_CONTADO 629  /*DEBE PAGAR AL MENOS %5.2lf DE CONTADO*/
#define S_DEBE_HABER_UN_PAGO_EN__S              630  /*DEBE HABER UN PAGO EN %s*/
#define S_IMPORTE_                              631  /*Importe:*/
#define S_ENVIAR_OFFLINE                        632  /*ENVIAR OFFLINE*/
#define S_DEBE_INGRESAR_AL_MENOS__D_DIGITOS__PRESIONE__ENTER_ 633  /*DEBE INGRESAR AL MENOS %d DIGITOS. PRESIONE <ENTER>*/
#define S__AUTORIZACION_BOLETIN_                634  /* AUTORIZACION BOLETIN */
#define S_AUTORIZ__MEDIO_EN_BOLETIN             635  /*Autoriz¢ medio en boletin*/
#define S_INST__BENEFICAS                       636  /*INST. BENEFICAS*/
#define S__DENOMINACION_                        637  /* DENOMINACION */
#define S__NRO__IDENTIFICACION_CLIENTE_         638  /* NRO. IDENTIFICACION CLIENTE */
#define S__NRO__SERIE_                          639  /* NRO. SERIE */
#define S_IDENTIFICACION_BILLETE                640  /*IDENTIFICACION BILLETE*/
#define S_DENOMINACI_N___D                      641  /*Denominaci¢n: %d*/
#define S_CLIENTE___S                           642  /*Cliente: %s*/
#define S_NRO__SERIE___S                        643  /*Nro. Serie: %s*/
#define S__N__CUENTA_                           644  /* NRO CUENTA */
#define S__COMPROBANTE_                         645  /* COMPROBANTE */
#define S_CUENTA_N_                             646  /*Cuenta N§*/
#define S_CLIENTE__                             647  /*Cliente :*/
#define S__CLIENTE_AFINIDAD_                    648  /* CLIENTE AFINIDAD */
#define S__NRO_DOCUMENTO_CLIENTE__              649  /* NRO DOCUMENTO CLIENTE  */
#define S_NUMERO____S                           650  /*Numero : %s*/
#define S_NOMBRE____S                           651  /*Nombre : %s*/
#define S_CATEG_____S                           652  /*Categ. : %s*/
#define S_ACEPTA_DESCONTAR__5_2F_PUNTOS__DESCUENTO___5_2F__ENTER_ESC_ 653  /*ACEPTA DESCONTAR %5.2f PUNTOS. DESCUENTO  %5.2f (Enter/Esc)*/
#define S__ANULACION_DEL_TICKET_                654  /* ANULACION DEL TICKET */
#define S_ANULACI_N_DEL_TICKET                  655  /*Anulaci¢n del ticket*/
#define S_EL_ITEM__S_NO_PUEDE_SER_VENDIDO_EN_ESTE_HORARIO 656  /*EL ITEM %s NO PUEDE SER VENDIDO EN ESTE HORARIO*/
#define S_REINTENTO_ARQUEO___LI_                657  /*REINTENTO ARQUEO (%li)*/
#define S___INFORMADO_DIFERENCIA                658  /*  Informado Diferencia*/
#define S_C_I_E_R_R_E___D_E___C_A_J_A           659  /*C I E R R E   D E   C A J A*/
#define S_A_R_Q_U_E_O___D_E___C_A_J_A           660  /*A R Q U E O   D E   C A J A*/
#define S_INFORMADO__DIFERENCIA                 661  /*Informado  Diferencia*/
#define S_SUBMEDIO____INFORMADO____DIFERENCIA   662  /*SubMedio    Informado    Diferencia*/
#define S_____PAG__ANTERIOR                     663  /*<-- PAG. ANTERIOR*/
#define S_PAG__SIGUIENTE____                    664  /*PAG. SIGUIENTE -->*/
#define S_EL_CAJERO_YA_ESTA_HAB_EN_LA_CAJA___I 665  /*EL CAJERO YA ESTA HABILITADO EN LA CAJA: %i*/
#define S_MUTUAL                                666  /*Mutual*/
#define S_TARJETA                               667  /*Tarjeta*/
#define S_TICKET_COMPRA                         668  /*Ticket Compra*/
#define S_NO_ENCONTRADO_MEDIO__S___D__D__DE___2F__ENTER_ 669  /*No Encontrado Medio %s (%d,%d) de %.2f <Enter>*/
#define S_ABORTAR_CIERRE                        670  /*ABORTAR CIERRE*/
#define S_ABORTAR_CIERRE_DE_CAJERO              671  /*Abortar Cierre de Cajero*/
#define S_ARQUEO                                672  /*ARQUEO*/
#define S_REINTENTO_ARQUEO_CAJERO_EN_CIERRE     673  /*Reintento Arqueo Cajero en Cierre*/
#define S_CONCEPTO_____                         674  /*Concepto    :*/
#define S_COMPROBANTE__                         675  /*Comprobante :*/
#define S_CUENTA_______                         676  /*Cuenta      :*/
#define S_IMPORTE______                         677  /*Importe     :*/
#define S__COBRO_CMR_                           678  /* COBRO CMR */
#define S_COBRO_REPACTADO                       679  /*COBRO REPACTADO*/
#define S_COBRO_CMR                             680  /*Cobro CMR*/
#define S__IMPORTE_A_PAGAR_                     681  /* IMPORTE A PAGAR */
#define S__NUMERO_DE_CUENTA_                    682  /* NUMERO DE CUENTA */
#define S_REIMPRESION_VALIDACION_COBRO          683  /*REIMPRESION VALIDACION COBRO*/
#define S_CANTIDAD__                            684  /*CANTIDAD: */
#define S_TOTAL__                               685  /*TOTAL: */
#define S__CLIENTE_INEXISTENTE_                 686  /* CLIENTE INEXISTENTE */
#define S_CHEQUE_DE_CLIENTE_INEXISTENTE         687  /*Cheque de Cliente Inexistente*/
#define S__COD_FIN_                             688  /* COD FIN */
#define S__OC___GD_                             689  /* OC - GD */
#define S_SUBTOTAL                              690  /*SUBTOTAL*/
#define S____CAJA__ABIERTA____                  691  /*   CAJA  ABIERTA    */
#define S____CAJA_EN_ESPERA___                  692  /*   CAJA EN ESPERA   */
#define S_____CAJA_CERRADA____                  693  /*    CAJA CERRADA    */
#define S__IMPORTE_A_DESCONTAR_                 694  /* IMPORTE A DESCONTAR */
#define S__DESCUENTO___                         695  /* DESCUENTO % */
#define S_DESCUENTO__                           696  /*Descuento %*/
#define S__PORCENTAJE_A_DESCONTAR_              697  /* PORCENTAJE A DESCONTAR */
#define S__DESCUENTO___ULT_ART__                698  /* DESCUENTO % ULT.ART. */
#define S_DESCUENTO___UTL_ART_                  699  /*Descuento % utl.art.*/
#define S__IMPORTE_A_ANULAR_                    700  /* IMPORTE A ANULAR */
#define S_DIFERENCIA_PRECIO                     701  /*DIFERENCIA PRECIO*/
#define S_CODIGO_BARRA______S                   702  /*Codigo Barra   : %s*/
#define S_CODIGO_INTERNO____LD                  703  /*Codigo Interno : %ld*/
#define S_NOMBRE_ARTICULO___S                   704  /*Nombre Articulo: %s*/
#define S_PRECIO_ACTUAL___X_U___                705  /*Precio actual   x u :-*/
#define S_PRECIO_DE_VENTA_X_U___                706  /*Precio de venta x u :-*/
#define S_PRECIO_ACTUAL___X_U__                 707  /*Precio actual   x u :*/
#define S_PRECIO_DE_VENTA_X_U__                 708  /*Precio de venta x u :*/
#define S__DIFERENCIA_DE_PRECIO                 709  /* DIFERENCIA DE PRECIO*/
#define S_COMPROMISO_PRECIO_MAS_BAJO            710  /*COMPROMISO PRECIO MAS BAJO*/
#define S_PRECIO_ACTUAL____________             711  /*Precio actual           :*/
#define S_PRECIO_DE_VENTA__________             712  /*Precio de venta         :*/
#define S_PRECIO_DE_LA_COMPETENCIA_             713  /*Precio de la competencia:*/
#define S__TARJETA___AUTO___CTA___NRO           714  /* Tarjeta Ä Auto Ä Cta Ä Nro*/
#define S__TIPO_INVENTARIO_                     715  /* TIPO INVENTARIO */
#define S__MODO_INVENTARIO_                     716  /* MODO INVENTARIO */
#define S_AUTORIZO_EL_MODO_INVENTARIO           717  /*Autorizo el modo inventario*/
#define S__SALIR_INVENTARIO_                    718  /* SALIR INVENTARIO */
#define S_SALIR_MODO_INVENTARIO                 719  /*Salir modo inventario*/
#define S_TIPO_INVENTARIO__                     720  /*TIPO INVENTARIO :*/
#define S_TOTAL_RENGLONES__                     721  /*TOTAL RENGLONES :*/
#define S_TOTAL_ARTICULOS__                     722  /*TOTAL ARTICULOS :*/
#define S__N__DE_VENTA_                         723  /* NRO DE VENTA */
#define S__PERFILES_                            724  /* PERFILES */
#define S__SELECCIONAR_PERFIL_                  725  /* SELECCIONAR PERFIL */
#define S__NRO__RUT_                            726  /* NRO. RUT */
#define S__NRO__LEGAJO_                         727  /* NRO. LEGAJO */
#define S_ACEPTA_LA_PROMOCION__S                728  /*ACEPTA LA PROMOCION %s*/
#define S_BONOS_A_ENTREGAR__                    729  /*BONOS A ENTREGAR :*/
#define S_ACEPTA_EL_PAGO_DE_LA_PROMOCION_EN_    730  /*ACEPTA EL PAGO DE LA PROMOCION EN */
#define S__ENTER_ACEPTA___ESC_RECHAZA_          731  /*(ENTER Acepta - ESC Rechaza)*/
#define S__PROMOCIONES_RELACIONADAS_AL_ARTICULO_732  /* PROMOCIONES RELACIONADAS AL ARTICULO */
#define S_NOMBRE_                               733  /*Nombre:*/
#define S_CODIGO_ESPECIAL_                      734  /*Codigo Especial:*/
#define S_CODIGO_BARRA_                         735  /*CODIGO BARRA:*/
#define S_PRECIO_                               736  /*Precio:*/
#define S_LLEVA__D_PAGA__D                      737  /*lleva %d paga %d*/
#define S_NO_TIENE_PROMOCIONES_RELACIONADAS___  738  /*NO TIENE PROMOCIONES RELACIONADAS!!!*/
#define S_DESCUENTO                             739  /*DESCUENTO*/
#define S_CUPONES                               740  /*CUPONES*/
#define S_MOSTRAR_CANTIDAD_CUPONES              741  /*MOSTRAR CANTIDAD CUPONES*/
#define S_CUPON                                 742  /*CUPON*/
#define S_MOSTRAR_UN_CUPON                      743  /*MOSTRAR UN CUPON*/
#define S_AGREGAR_ARTICULOS                     744  /*AGREGAR ARTICULOS*/
#define S_PUNTOS                                745  /*PUNTOS*/
#define S_ENTREGA_DE_ARTICULO                   746  /*ENTREGA DE ARTICULO*/
#define S_PRECIO_FIJO                           747  /*PRECIO FIJO*/
#define S_SOBRE_ARTICULO                        748  /*SOBRE ARTICULO*/
#define S_DEPARTAMENTO                          749  /*DEPARTAMENTO*/
#define S_CANASTA                               750  /*CANASTA*/
#define S_GRUPO_DE_ARTICULOS                    751  /*GRUPO DE ARTICULOS*/
#define S_INCONDICIONAL                         752  /*INCONDICIONAL*/
#define S_ARTICULOS_DIFERENTES                  753  /*ARTICULOS DIFERENTES*/
#define S_ARTICULOS_EXCEPTUADOS                 754  /*ARTICULOS EXCEPTUADOS*/
#define S_BOLSON                                755  /*BOLSON*/
#define S_N_X_M                                 756  /*N x M*/
#define S__N__DE_GUIA_                          757  /* NRO DE GUIA */
#define S__N__DE_REMITO_                        758  /* NRO DE REMITO */
#define S_FONDO_FIJO                            759  /*FONDO FIJO*/
#define S__NRO_ROLLO_                           760  /* NRO ROLLO */
#define S__STAND_BY_                            761  /* STAND BY */
#define S__CAJA_EN_ESPERA_                      762  /* CAJA EN ESPERA */
#define S_CAJA_______CAJERO_____                763  /*CAJA     - CAJERO     */
#define S__HABILITA_CAJA_                       764  /* HABILITA CAJA */
#define S___TOTAL__________________012_0F__     765  /*  TOTAL                 %012.0f  */
#define S__REPROCESAR_PAGO_                     766  /* REPROCESAR PAGO */
#define S_REPROCESO_DEL_PAGO                    767  /*Reproceso del pago*/
#define S_REIMPRESION_DE_FACTURA                768  /*REIMPRESION DE FACTURA*/
#define S_DEVOLUCION                            769  /*DEVOLUCION*/
#define S__S__10_2LF___ENTER__SI                770  /*%s %10.2lf. <ENTER> SI*/
#define S_CHEQUE                                771  /*CHEQUE*/
#define S__S__S_A_VALIDAR_Y_PRESIONE__ENTER_    772  /*%s %s A VALIDAR Y PRESIONE <ENTER>*/
#define S__S__S__LI_Y_PRESIONE__ENTER_          773  /*%s %s %li Y PRESIONE <ENTER>*/
#define S__S__S_A_VALIDAR                       774  /*%s %s A VALIDAR*/
#define S__S__S__LI_A_VALIDAR                   775  /*%s %s %li A VALIDAR*/
#define S_CHEQUES_CLIENTE                       776  /*CHEQUES CLIENTE*/
#define S_REIMPRESION_VALIDACION_MEDIO_DE_PAGO  777  /*REIMPRESION VALIDACION MEDIO DE PAGO*/
#define S_MEDIOS_DE_PAGO_A_REIMPRIMIR           778  /*MEDIOS DE PAGO A REIMPRIMIR*/
#define S_ENTIDAD_DESCONOCIDA                   779  /*Entidad Desconocida*/
#define S_VALIDACION__I_NO_DEFINIDA             780  /*VALIDACION %i NO DEFINIDA*/
#define S_DEVOLUCION_DE_ENVASES                 781  /*DEVOLUCION DE ENVASES*/
#define S__NRO_VENDEDOR_                        782  /* NRO VENDEDOR */
#define S__MOZO_                                783  /* MOZO */
#define S_FALLO_AL_INSETAR__D_                  784  /*FALLO AL INSETAR %d */
#define S__FUNCIONES_                           785  /* FUNCIONES */
#define S_AUTORIZ__TRANSFERENCIAS               786  /*Autoriz¢ transferencias*/
#define S_LA_ET__TOMRA_N___LI_YA_FUE_PROCESADA  787  /*LA ET. TOMRA Nº %li YA FUE PROCESADA*/
#define S_NO_SE_ENCONTRO_LA_ET__TOMRA_N___LI    788  /*NO SE ENCONTRO LA ET. TOMRA Nº %li*/
#define S_MENSAJES                              789  /*MENSAJES*/
#define S_MENSAJE_ENVIADO__                     790  /*MENSAJE ENVIADO !*/
#define S_CONFIRME_MONTO                        791  /*CONFIRME MONTO*/
#define S_INGRESE_SU_CLAVE                      792  /*INGRESE SU CLAVE*/
#define S_EL_PIN_DEBE_TENER_ENTRE__I_Y__I_DIGITOS 793  /*EL PIN DEBE TENER ENTRE %i y %i DIGITOS*/
#define S_EL_PIN_DEBE_TENER__I_DIGITOS          794  /*EL PIN DEBE TENER %i DIGITOS*/
#define S________REDCOMPRA                      795  /*       REDCOMPRA*/
#define S_N__SUPERVISOR_                        796  /*Nro Supervisor:*/
#define S_CAJERO___                             797  /*Cajero : */
#define S_CAJERO___D                            798  /*CAJERO: %d*/
#define S_SUPERVISOR___                         799  /*Supervisor : */
#define S_SUPERVISOR___D__S__S                  800  /*SUPERVISOR: %d %s %s*/
#define S_SUPERVISOR___D__S                     801  /*SUPERVISOR: %d %s*/
#define S_SUPERVISOR___D                        802  /*SUPERVISOR: %d*/
#define S_SUPERVISOR                            803  /*supervisor*/
#define S_SUPERVISOR___LD__NOMBRE___S           804  /*Supervisor: %ld  Nombre: %s*/
#define S_CAJERO___LD__NOMBRE___S               805  /*Cajero: %ld  Nombre: %s*/
#define S__LDNOMBRE_S                           806  /*%ldNOMBRE%s*/
#define S_PREPARANDO_MODO_ENTRENAMIENTO_        807  /*Preparando Modo Entrenamiento.*/
#define S_NO_SE_PUDO_RESETEAR___APAGUE_EL_POST  808  /*NO SE PUDO RESETEAR - APAGUE EL POST*/
#define S_COLOQUE_LA_LLAVE_EN__S                809  /*COLOQUE LA LLAVE EN %s*/
#define S__NRO_BONO_                            810  /* NRO BONO */
#define S_AUTORIZO_CAJERO_DUPLICADO             811  /*Autoriz¢ cajero duplicado*/
#define S_AUTORIZACION_DE_CAJERO                812  /* AUTORIZACION DE CAJERO */
#define S_YA_EMITIO_UNA_Z_HOY                   813  /*YA EMITIO UNA Z HOY*/
#define S_DEBE_HACER_EL_CIERRE_DE_CAJA_ESC      814  /*DEBE HACER EL CIERRE DE CAJA (presione ESC)*/
#define S_EL_SERVER_NO_SE_ENCUENTRA_ON_LINE_ESC 815  /*EL SERVER NO SE ENCUENTRA ON-LINE (presione ESC)*/
#define S_HABILITAR_CAJERO                      816  /*HABILITAR CAJERO*/
#define S_CAJONES_HABILITADOS                   817  /*CAJONES HABILITADOS*/
#define S__CAJON__CAJERO_                       818  /* Caj¢n  Cajero */
#define S_EL_MEDIO_DE_PAGO__S_ESTA_EXCEDIDO     819  /*EL MEDIO DE PAGO %s ESTA EXCEDIDO - LLAME AL SUPERVISOR*/
#define S_NO_TIENE_SUFICIENTE__S___             820  /*NO TIENE SUFICIENTE %s - LLAME AL SUPERVISOR*/
#define S__AUTORIZACION_DE_ANULACIONES_         821  /* AUTORIZACION DE ANULACIONES */
#define S_AUTORIZO_ANULACIONES                  822  /*Autoriz¢ anulaciones*/
#define S__EMISION_DE_INFORMES_                 823  /* EMISION DE INFORMES */
#define S_INFORMES                              824  /*Informes*/
#define S_CIERRE_DE_LA_CAJA_                    825  /* CIERRE DE LA CAJA */
#define S_NOTA_DE_CREDITO_                      826  /* NOTA DE CREDITO */
#define S_AUTORIZO_NC                           827  /*Autoriz¢ NC*/
#define S_HABILITACION_DEL_SCANNER              828  /*Habilitaci¢n del scanner*/
#define S_COPIA_VOUCHER                         829  /*COPIA VOUCHER*/
#define S_ENTRENAMIENTO                         830  /*ENTRENAMIENTO*/
#define S_BIEN_DE_USO                           831  /*BIEN_DE_USO*/
#define S_DEBE_INGRESAR_CLIENTE                 832  /*DEBE INGRESAR CLIENTE*/
#define S__AUTORIZACION_DEVOLUCION_             833  /* AUTORIZACION DEVOLUCION */
#define S_AUTORIZO_DEVOLUCIONES                 834  /*Autoriz¢ devoluciones*/
#define S_AUTORIZO_IMPORTE_EXCEDIDO             835  /*Autoriz¢ importe excedido*/
#define S_COMPROMISO_200_                       836  /*COMPROMISO 200%*/
#define S_NO_ES_POSIBLE_REG_COMPR_200           837  /*NO ES POSIBLE REGISTAR UN COMPROMISO 200%% */
#define S_TIENE__D_TICKETS_EN_ESPERA            838  /*TIENE %d TICKET(S) EN ESPERA*/
#define S_CODIGO                                839  /*CODIGO*/
#define S__ABRIR_CAJON_                         840  /* ABRIR CAJON */
#define S_AUTORIZO_TECLA_ABRIR_CAJON            841  /*Autoriz¢ tecla abrir caj¢n*/
#define S_FECHA                                 842  /*fecha*/
#define S_HORA                                  843  /*hora*/
#define S_SUC_                                  844  /*suc.*/
#define S_CAJA                                  845  /*caja*/
#define S_CAJERO                                846  /*cajero*/
#define S_RECIBE                                847  /*RECIBE*/
#define S_FALTA                                 848  /*FALTA*/
#define S_COD_CLIENTE                           849  /* CODIGO DE CLIENTE */
#define S_CLIENTE                               850  /*CLIENTE*/
#define S__CUPO_EXCEDIDO_                       851  /* CUPO EXCEDIDO */
#define S_AUTORIZO_CUPO_EXCEDIDO                852  /*Autoriz¢ cupo excedido*/
#define S_REIMPRESION                           853  /*REIMPRESION*/
#define S_FACTURA_A                             854  /*FACTURA \"A\"*/
#define S_FACTURA_B                             855  /*FACTURA \"B\"*/
#define S_REIMPRESION_DE__S                     856  /*REIMPRESION DE %s*/
#define S_RETIRE_SU_NOTA_DE_CREDITO_POR__       857  /*RETIRE SU NOTA DE CREDITO POR $*/
#define S_RETIRE_SU_FACTURA_POR__               858  /*RETIRE SU FACTURA POR $*/
#define S_NOTA_CR_                              859  /*NOTA CR.*/
#define S_FACTURA                               860  /*FACTURA*/
#define S_CUENTA_CORRIENTE                      861  /*CUENTA CORRIENTE*/
#define S__CAMBIO_                              862  /* CAMBIO */
#define S_CAMBIO                                863  /*CAMBIO*/
#define S_INVENTARIO_                           864  /*INVENTARIO */
#define S_TRANSFERENC_                          865  /*TRANSFERENC */
#define S_CREDITO_O_CONTADO                     866  /*CREDITO O CONTADO*/
#define S_CRED_O_CONT_                          867  /*CRED.O CONT.*/
#define S_COD_FIN_                              868  /*COD FIN:*/
#define S_SALDO________________                 869  /*SALDO:               */
#define S_DISPONIBLE_                           870  /*DISPONIBLE:*/
#define S_MOZO_                                 871  /*MOZO:*/
#define S_VEND_                                 872  /*VEND:*/
#define S_GUIA____                              873  /*GUIA   :*/
#define S_REMITO__                              874  /*REMITO :*/
#define S_BASE_DEL_DESCUENTO                    875  /*BASE DEL DESCUENTO*/
#define S_TOTAL___________                      876  /*Total    :      */
#define S_ERROR_EN_LINEA                        877  /*ERROR EN LINEA*/
#define S_MONTO_CUOTA                           878  /*MONTO CUOTA*/
#define S_TASA_APLICADA                         879  /*TASA APLICADA*/
#define S_AUT__ORIGINAL                         880  /*AUT. ORIGINAL*/
#define S_HORA_ORIGINAL                         881  /*HORA ORIGINAL*/
#define S_FECHAS_A_HABILITAR_DIFERIMIENTO       882  /*FECHAS A HABILITAR DIFERIMIENTO*/
#define S_BOLETIN_VENCIDO_                      883  /*Boletin Vencido*/
#define S_CUPON_ORIGINAL                        884  /*CUPON ORIGINAL*/
#define S_LLAMAR_CODIGO_10__TARJETA___S_        885  /*LLAMAR CODIGO 10 (TARJETA: %s)*/
#define S_VERIFICACION                          886  /*VERIFICACION*/
#define S__AUTORIZA_TARJETA_EN_BOLETIN_         887  /* AUTORIZA TARJETA EN BOLETIN */
#define S_TARJETA_EN_BOLETIN_AUTORIZADA         888  /*Tarjeta en bolet¡n autorizada*/
#define S_CUPON_VERIFICADO                      889  /*Cup¢n verificado*/
#define S_TARJETA_VERIFICADA                    890  /*Tarjeta verificada*/
#define S_PROCESANDO_EVENTO_OFF_LINE___LI_      891  /*Procesando Evento OFF-Line (%li)*/
#define S_PROCESANDO_REPROCESO_DE_PAGO_EVENTO   892  /*Procesando Reproceso de Pago Evento (%li)*/
#define S_AUTORIZO_PRECIOS                      893  /*Autoriz¢ precios*/
#define S__AUTORIZACION_DE_PRECIOS_             894  /* AUTORIZACION DE PRECIOS */
#define S_TICKETS_EN_ESPERA                     895  /*TICKETS EN ESPERA*/
#define S_OBTENIENDO_TABLA__S										896  /*Obteniendo Tabla %s*/
#define S_RECUERDE_LA_PROMOCION_DE__S_A_____LF						897  /*RECUERDE LA PROMOCION DE %s A %*.*lf*/
#define S_PENDIENTES_EN_SLIP__COLOQUE_EL_PAPEL						898  /*PENDIENTES EN SLIP, COLOQUE EL PAPEL*/
#define S_TOTAL_A_PAGAR												899  /*TOTAL A PAGAR*/
#define S_CUOTAS_______                                             900  /*Cuotas      :*/
#define S_ADELANTO_EN_EFECTIVO                                      901  /*ADELANTO EN EFECTIVO*/
#define S_PAGOS                                                     902  /*PAGOS*/
#define S_INVALIDO                                                  903  /*INVALIDO*/
#define S_SOLO_PUEDE_PAGAR____8_2F_EN_CUOTAS                        904  /*SOLO PUEDE PAGAR $ %8.2f EN CUOTAS*/
#define S_DOCUMENTO                                                 905  /*DOCUMENTO*/
#define S_CONECTANDO___                                             906  /*CONECTANDO...*/
#define S_CUOTAS_                                                   907  /*Cuotas:*/
#define S_TARJETA_N_                                                908  /*Tarjeta N§*/
#define S_CUPON_N_                                                  909  /*Cupon N§*/
#define S_INGRESO_N__AUTORIZACION                                   910  /*Ingres¢ N§ autorizaci¢n*/
#define S_TARJETA_AUTORIZADA_HASTA__7_2F__                          911  /*TARJETA AUTORIZADA HASTA %7.2f $*/
#define S_CHEQUE_EN_CLIENTES_INEXISTENTE                            912  /*CHEQUE EN CLIENTES INEXISTENTE*/
#define S_FECHA_EXCEDIDA                                            913  /*FECHA EXCEDIDA*/
#define S_CHEQUE_CON_FECHA_A_MAS_DE_30_DIAS                         914  /*CHEQUE CON FECHA A MAS DE 30 DIAS*/
#define S_CHEQUE_CON_FECHA_A_MAS_DE__02D_DIAS                       915  /*CHEQUE CON FECHA A MAS DE %02d DIAS*/
#define S_FONO_COMPRADOR                                            916  /*FONO COMPRADOR*/
#define S_RUT_COMPRADOR                                             917  /*RUT COMPRADOR*/
#define S_BOLETIN                                                   918  /*BOLETIN*/
#define S_AUTORIZO_SIN_BOLETIN                                      919  /*Autoriz¢ sin boletin*/
#define S_CUENTA_AUTORIZADA_HASTA__7_2F__                           920  /*CUENTA AUTORIZADA HASTA %7.2f $*/
#define S_PRECIOS_NC                                                921  /*PRECIOS NC*/
#define S_TICKET_______                                             922  /*TICKET      :*/
#define S_CAJA_DGI_____                                             923  /*Caja DGI    :*/
#define S_FECHA________                                             924  /*FECHA       :*/
#define S_TRANSACCION__                                             925  /*TRANSACCION :*/
#define S_CAJA_________                                             926  /*CAJA        :*/
#define S_IMPORTAR_EN_NOTA_DE_CREDITO                               927  /*IMPORTAR EN NOTA DE CREDITO*/
#define S_CIERRE                                                    928  /*CIERRE*/
#define S_VENTA                                                     929  /*VENTA*/
#define S_DIA_OPERATIVO___S                                         930  /*DIA OPERATIVO: %s*/
#define S_CIERRE_DE_CAJA                                            931  /*CIERRE DE CAJA*/
#define S_INFORME_00                                                932  /*INFORME 00*/
#define S_FONDO_FIJO_                                               933  /*FONDO_FIJO*/
#define S_VENTAS                                                    934  /*VENTAS*/
#define S_CAMBIOS                                                   935  /*CAMBIOS*/
#define S_DONACIONES                                                936  /*DONACIONES*/
#define S_DESCONOCIDA                                               937  /*DESCONOCIDA*/
#define S_INTERESES                                                 938  /*INTERESES*/
#define S_RET__F__FIJO                                              939  /*RET. F. FIJO*/
#define S_ANULACIONES                                               940  /*ANULACIONES*/
#define S_CORRECCIONES                                              941  /*CORRECCIONES*/
#define S_DEV_ENVASES                                               942  /*DEV.ENVASES*/
#define S_PAGOS_X_CAJA                                              943  /*PAGOS X CAJA*/
#define S_VUELTOS                                                   944  /*VUELTOS*/
#define S_REDONDEO                                                  945  /*REDONDEO*/
#define S_TOTAL_DE_CAJA                                             946  /*TOTAL DE CAJA*/
#define S_VENTAS_GRAVADAS                                           947  /*VENTAS GRAVADAS*/
#define S_VENTAS_EXENTAS                                            948  /*VENTAS EXENTAS*/
#define S_IMP__INTERNOS                                             949  /*IMP. INTERNOS*/
#define S_SERVICIOS_COBRADOS                                        950  /*SERVICIOS COBRADOS*/
#define S_NRO_DE_TICKETS                                            951  /*NRO DE TICKETS*/
#define S_DIFERENCIAS                                               952  /*DIFERENCIAS*/
#define S_FACTURAS_A                                                953  /*FACTURAS A*/
#define S_FACTURAS_B                                                954  /*FACTURAS B*/
#define S_NC_A                                                      955  /*NC A*/
#define S_NC_B                                                      956  /*NC B*/
#define S_PERCEPCION_DGI                                            957  /*PERCEPCION DGI*/
#define S_RETENCION_DGR                                             958  /*RETENCION DGR*/
#define S_PERCEP_212_IVA1                                           959  /*PERCEP.212 IVA1*/
#define S_PERCEP_212_IVA2                                           960  /*PERCEP.212 IVA2*/
#define S_TICKETS                                                   961  /*TICKETS*/
#define S_GRAN_TOTAL_ANT                                            962  /*GRAN TOTAL ANT*/
#define S_GRAN_TOTAL____                                            963  /*GRAN TOTAL    */
#define S_BONOS_ENTREGADOS                                          964  /*BONOS ENTREGADOS*/
#define S_TOTAL_DE_BONOS                                            965  /*TOTAL DE BONOS*/
#define S_VENTAS_X_BOLETAS                                          966  /*VENTAS x BOLETAS*/
#define S_VENTAS_X_FACTURAS                                         967  /*VENTAS x FACTURAS*/
#define S_NOTAS_DE_CREDITO                                          968  /*NOTAS DE CREDITO*/
#define S_BOLETAS_EMITIDAS                                          969  /*BOLETAS EMITIDAS*/
#define S_FACTURAS_EMITIDAS                                         970  /*FACTURAS EMITIDAS*/
#define S_N_C______EMITIDAS                                         971  /*N.C.     EMITIDAS*/
#define S_RETIRO_F__FIJO                                            972  /*RETIRO F. FIJO*/
#define S_RETIROS_DE_CAJA                                           973  /*RETIROS DE CAJA*/
#define S_ADELANTO_SUELDOS                                          974  /*ADELANTO SUELDOS*/
#define S_OTROS_INGRESOS                                            975  /*OTROS INGRESOS*/
#define S_INGRESOS_DE_3_                                            976  /*INGRESOS DE 3§*/
#define S_TOTAL_IVA_                                                977  /*TOTAL IVA */
#define S_INFORME_DE_TRANSACCIONES_AL                               979  /*INFORME DE TRANSACCIONES AL*/
#define S_HORA___TRANS_______BOL__TRA____MONTO                      980  /*HORA   TRANS.     #BOL.#TRA.   MONTO*/
#define S_INFORME_DE_TARJETAS                                       981  /*INFORME DE TARJETAS*/
#define S_ARQUEO_DE_CAJA                                            982  /*ARQUEO DE CAJA*/
#define S_DIFERENCIAS_POR_SUBMEDIOS                                 983  /*DIFERENCIAS POR SUBMEDIOS*/
#define S_INFORMADO                                                 984  /*INFORMADO*/
#define S_REGISTRADO                                                985  /*REGISTRADO*/
#define S_Fecha________                                             986  /*Fecha       :*/
#define S_CAJERO_ACTUAL________S_                                   987  /*Cajero actual     : %s */
#define S_CAJERO_DEL_TICKET____09LI_                                988  /*Cajero del ticket : %09li */
#define S_CAJERO_ACTUAL________06LI_                                989  /*Cajero actual     : %06li */
#define S_CAJERO_DEL_TICKET____06LI_                                990  /*Cajero del ticket : %06li */
#define S_CAJERO_ACTUAL________D_                                   991  /*Cajero actual     : %d */
#define S_CAJERO_DEL_TICKET____D_                                   992  /*Cajero del ticket : %d */
#define S_CAJEROS_DISTINTOS                                         993  /*Cajeros distintos*/
#define S_AUTORIZA_CAJEROS_DISTINTOS                                994  /*AUTORIZA CAJEROS DISTINTOS*/
#define S_INVENTARIO                                                995  /*INVENTARIO*/
#define S_NRO_SECTOR____                                            996  /*Nro Sector   :*/
#define S_NRO_VENDEDOR__                                            997  /*Nro vendedor :*/
#define S_______________DESCRIPCION____________                     998  /*--------------DESCRIPCION---------------*/
#define S_RECARGO                                                   999  /*RECARGO*/
#define S_TOTAL_____                                                1000  /*TOTAL    :*/
#define S_RETENCION_DGR___                                          1001  /*RETENCION DGR  :*/
#define S_PERCEPCION_DGI__                                          1002  /*PERCEPCION DGI :*/
#define S_ARTICULOS_                                                1003  /*ARTICULOS:*/
#define S_GRAVADO____                                               1004  /*Gravado   :*/
#define S_I_V_A__1___                                               1005  /*I.V.A. 1  :*/
#define S_I_V_A__2___                                               1006  /*I.V.A. 2  :*/
#define S_EXENTO_____                                               1007  /*Exento    :*/
#define S_IMP_INT____                                               1008  /*Imp.Int   :*/
#define S_IMPORTE__                                                 1009  /*IMPORTE: */
#define S_BOLETA_ANTERIOR                                           1010  /*Boleta anterior*/
#define S_BOLETA                                                    1011  /*Boleta*/
#define S_SECUENCIA                                                 1012  /*Secuencia*/
#define S_VENDEDOR_                                                 1013  /*Vendedor:*/
#define S______DEVOLUCION_____                                      1014  /*     DEVOLUCION     */
#define S_TOTAL_RETIROS                                             1015  /*TOTAL RETIROS*/
#define S_SECTOR                                                    1016  /*SECTOR*/
#define S_CORRECCION                                                1017  /*CORRECCION*/
#define S_SUPERVISOR_                                               1018  /*Supervisor:*/
#define S_CAJERO_                                                   1019  /*Cajero:*/
#define S_TOTAL_FONDO_FIJO_                                         1020  /*TOTAL FONDO FIJO:*/
#define S_ERROR_FISCAL                                              1021  /*ERROR FISCAL*/
#define S_M_O_D_O___Z                                               1022  /*M O D O   Z*/
#define S__PRECIO_                                                  1023  /* PRECIO */
#define S_______CODIGO________                                      1024  /*      CODIGO        */
#define S_____INEXISTENTE_____                                      1025  /*    INEXISTENTE     */
#define S__CODIGO_DE_BARRAS_                                        1026  /* CODIGO DE BARRAS */
#define S__NOVIOS_                                                  1027  /* NOVIOS */
#define S_______APERTURA_DEL_CAJON______                            1028  /*********** APERTURA DEL CAJON ***********/
#define S__ADELANTOS_                                               1029  /* ADELANTOS */
#define S____PRESIONE_UNA_TECLA__                                   1030  /* ( Presione una tecla )*/
#define S__EXCEDIDO__EXCEDE_____2F_                                 1031  /* EXCEDIDO (Excede:$ %.2f)*/
#define S__D_CUOTAS_DE__7_2F__                                      1032  /*%d cuotas de %7.2f $*/
#define S_____PARAM_INVALIDO____                                    1033  /*****PARAM INVALIDO*****/
#define S__CAJA____4I_                                              1034  /* CAJA: %.4i */
#define S__SISTEMA_INVEL_                                           1035  /* SISTEMA INVEL */
#define S_CAJERO__                                                  1036  /*CAJERO*/
#define S_CAJON                                                     1037  /*CAJON*/
#define S_ARTICULO                                                  1038  /*ARTICULO*/
#define S_SUBTOTAL___                                               1039  /*SUBTOTAL: $*/
#define S___________TICKET_SUSPENDIDO__________                     1040  /***********TICKET SUSPENDIDO***********/
#define S______________TICKET_ANULADO_____________                  1041 /**************TICKET ANULADO**************/
#define S_______BONOS_A_ENTREGAR__                                  1042  /****** BONOS A ENTREGAR :*/
#define S_________REPROCESAMIENTO_DEL_PAGO________                  1043 /******** REPROCESAMIENTO DEL PAGO ********/
#define S____CAMBIO___                                              1044  /*** CAMBIO ***/
#define S_NRO__TICKET__                                             1045  /*Nro. Ticket :*/
#define S_NRO__CAJA____                                             1046  /*Nro. Caja   :*/
#define S_TIPO_INV______                                            1047  /*TIPO INV.:    */
#define S_CANT_RENG_____                                            1048  /*CANT.RENG:    */
#define S_TRANSF______                                              1049  /*TRANSF.:    */
#define S____ARTIC______                                            1050  /*   ARTIC.:    */
#define S__TARJETA__                                                1051  /* TARJETA (*/
#define S__MUTUAL______                                             1052  /* MUTUAL (  ) */
#define S__NC_                                                      1053  /*<NC>*/
#define S__FAC_                                                     1054  /*<FAC>*/
#define S__T_F_                                                     1055  /*<T F>*/
#define S__GUIA_                                                    1056  /*<GUIA>*/
#define S__REM_                                                     1057  /*<REM>*/
#define S__DIF__                                                    1058  /*<DIF >*/
#define S__200__                                                    1059  /*<200%>*/
#define S__BAJO_                                                    1060  /*<BAJO>*/
#define S__REF_                                                     1061  /*<REF>*/
#define S__MODO_DEVOLUCION_                                         1062  /*<MODO DEVOLUCION>*/
#define S_ET_TOMRA__                                                1063  /*ET.TOMRA: */
#define S__DESCUENTO_I_ULT_ART__                                    1064  /* DESCUENTO $ ULT.ART. */
#define S__DEVOLUCIONES_                                            1065  /* DEVOLUCIONES */
#define S_INFORME_15                                                1066  /*INFORME 15*/
#define S__TICKET_ORIGINAL_                                         1067  /* TICKET ORIGINAL */
#define S__ESC__RECHAZA_O_ENTER__AUTORIZA_                          1068  /*(ESC->Rechaza o ENTER->Autoriza)*/
#define S__PRESIONE_ESC_                                            1069  /*(Presione ESC)*/
#define S__N__DE_CHEQUE_                                            1070  /* NRO DE CHEQUE */
#define S__FECHA_DE_VENCIMIENTO_                                    1071  /* FECHA DE VENCIMIENTO */
#define S__COD__POST__CHEQUE_                                       1072  /* COD. POST. CHEQUE */
#define S__COD__DE_HORA_                                            1073  /* COD. DE HORA */
#define S__TELEFONO_                                                1074  /* TELEFONO */
#define S__FONO_                                                    1075  /* FONO */
#define S__SUCURSAL_                                                1076  /* SUCURSAL */
#define S__CUENTA_                                                  1077  /* CUENTA */
#define S__CHEQUE__                                                 1078  /* CHEQUE  */
#define S__CHEQUE_                                                  1079  /* CHEQUE */
#define S__N__DE_CUENTA_                                            1080  /* NRO DE CUENTA */
#define S__PLAZA_                                                   1081  /* PLAZA */
#define S__AUTORIZACION_                                            1082  /* AUTORIZACION */
#define S__NRO_CUPON_                                               1083  /* NRO CUPON */
#define S__CANT__CUPONES_                                           1084  /* CANT. CUPONES */
#define S__VALOR_CUPON_                                             1085  /* VALOR CUPON */
#define S__PAGOS_                                                   1086  /* PAGOS */
#define S__P_A_G_O_S_                                               1087  /* P A G O S */
#define S__IMPORTE_A_RETIRAR_                                       1088  /* IMPORTE A RETIRAR */
#define S_TOTAL_                                                    1089  /*TOTAL */
#define S_T__ESPERA                                                 1090  /*T. ESPERA*/
#define S__N__DE_TARJETA_                                           1091  /* NRO DE TARJETA */
#define S__CUOTAS_                                                  1092  /* CUOTAS */
#define S__FINANCIA_TARJETA_                                        1093  /* FINANCIA TARJETA */
#define S__TIPO_DE_CUENTA_                                          1094  /* TIPO DE CUENTA */
#define S__MARCAS_ESPECIALES_                                       1095  /* MARCAS ESPECIALES */
#define S__DIFERIMIENTO_                                            1096  /* DIFERIMIENTO */
#define S__IMPORTE_ENTREGA_                                         1097  /* IMPORTE ENTREGA */
#define S__FECHA_ORIGINAL_                                          1098  /* FECHA ORIGINAL */
#define S__DOCUMENTO_                                               1099  /* DOCUMENTO */
#define S_NO_SE_PERMITE_INGRESO_MANUAL_TARJ                         1100  /* NO SE PERMITE EL INGRESO MANUAL DE ESTA TARJETA */
#define S_POS_FUERA_DE_LINEA                                        1101  /* POS FUERA DE LINEA */
#define S__TARJETA_PARENTESIS                                       1102  /*  TARJETA ( */
#define S_TIPO_INV__                                                1103  /* TIPO INV.:     */
#define S_ARTIC_____                                                1104  /* ARTIC.:     */
#define S_RENGLONES                                                 1105  /* RENGLONES */
#define S_TRANSF__                                                  1106  /* TRANSF.: */
#define S_BULTOS                                                    1107  /* BULTOS */
#define S_FECHA_DE_OPERACION__                                      1108  /*  Fecha de Operaci¢n :           */
#define S_CLAVE_                                                    1109  /* Clave: */
#define S_libre5___________                                         1110  /* Supervisor:  */
#define S_NRO_USUARIO                                               1111  /*  Nº Usuario  */
#define S_CLAVE                                                     1112  /* Clave*/
#define S_ETIQUETAS                                                 1113  /* ETIQUETAS */
#define S_DIA_OPERATIVO_SERVIDOR                                    1114  /*dia operativo servidor  : */
#define S_DIA_OPERATIVO                                             1115  /*dia operativo           : */
#define S_FECHA_CALENDARIA                                          1116  /*fecha                   : */
#define S_CLAVE_SEGUIR_OPERANDO                                     1117  /*INGRESE CLAVE PARA SEGUIR OPERANDO*/
#define S_ERROR_GRABANDO_EVENTO                                     1118
#define S_ERROR_GRABANDO_FACTURA                                    1119
#define S_CAJA_ORIGINAL                                             1120  /*Caja Original*/
#define S_SUCURSAL_ORIGINAL                                         1121  /*Sucursal Original*/
#define S_NRO_COMPROBANTE_DE_COBRO                                  1122  /*NRO COMPROBANTE DE COBRO*/
#define S_SUCURSAL_DE_COBRO                                         1123  /*SUCURSAL DE COBRO*/
#define S_CAJA_DE_COBRO                                             1124  /*CAJA DE COBRO*/
#define S_FECHA_DE_COBRO                                            1125  /*FECHA DE COBRO*/
#define S_DESCUENTOS                                                1126  /*DESCUENTOS*/
#define S_INGRESE_POR_SLIP_EL_FORMULARIO                            1127  /*Ingrese por SLIP el formulario para el informe*/
#define S_EL_FORMULARIO_NO_HA_SIDO_INGRESADO                        1128  /*El formulario no ha sido ingresado*/
#define S_EL_IMP_EXCEDE_TOTAL_ART_REST_X_MEDIO                      1129  /*EL IMPORTE EXCEDE AL TOTAL DE ARTICULOS RESTRINGIDOS POR EL MEDIO*/
#define S_MEDIO_EXCLUIDO                                            1130  /*MEDIO EXCLUIDO*/
#define S_PRESIONE_UNA_TECLA_PARA_CONTINUAR                         1131  /*PRESIONE UNA TECLA PARA CONTINUAR*/
#define S_INICIO_CONF_REMOTA                                        1132  /*INICIO DE CONFIGURACION REMOTA*/
#define S_HUBO_ERRORES_DURANTE_LA_CONFIG                            1133  /*HUBO ERRORES DURANTE LA CONFIGURACION*/
#define S_CONFIGURACION_REMOTA_OK                                   1134  /*CONFIGURACION REMOTA OK*/
#define S_FIN_CONF_REMOTA                                           1135  /*FIN DE CONFIGURACION REMOTA*/
#define S_PREFIJO_INVALIDO                                          1136  /*PREFIJO INVALIDO*/
#define S_VERIFIQUE_LOS_DATOS_DEL_CODIGO_INGRESADO                  1137  /*VERIFIQUE LOS DATOS DEL CODIGO INGRESADO*/
#define S_POR_FAVOR_INGRESE_EL_VENDEDOR                             1138  /*POR FAVOR INGRESE EL VENDEDOR*/
#define S_NO_SE_ENCONTRO_LA_MUTUAL                                  1139  /*NO SE ENCONTRO LA MUTUAL*/
#define S_CAMBIA_VENDEDOR_ENTER_ESC                                 1140  /*CAMBIA VENDEDOR?. <ENTER SI - ESC NO>*/
#define S_VENDEDOR_INEX_EN_PADRON_DE_VENDEDORES                     1141  /*VENDEDOR INEXISTENTE EN PADRON DE VENDEDORES*/
#define S_RUT_INVALIDO                                              1142  /*RUT INVALIDO*/
#define S_TICKET_TOMRA_REPETIDO                                     1143  /*TICKET TOMRA REPETIDO*/
#define S_NO_SE_ENCUENTRA_TOMRA_TMP_O_AREA_USADA                    1144  /*NO SE ENCUENTRA TOMRA.TMP O AREA USADA*/
#define S_ERROR_EN_MALLOC                                           1145  /*ERROR EN MALLOC*/
#define S_HAY_PROMO_O_DESC_NO_REGRESA_VENTAS                        1146  /*HAY PROMOCIONES O DESCUENTOS, NO SE PUEDE REGRESAR A LA PANTALLA DE VENTAS*/
#define S_CONSULTA_CENTRAL                                          1147  /*CONSULTA CENTRAL*/
#define S_CERRANDO_TICKET_POR_CORTE_DE_ENERGIA                      1148  /*CERRANDO TICKET POR CORTE DE ENERGIA*/
#define S_IMPORTE_DE_DESCUENTO_EXCEDIDO                             1149  /*IMPORTE DE DESCUENTO EXCEDIDO*/
#define S_ANULE_PRIMERO_EL_DESCUENTO                                1150  /*ANULE PRIMERO EL DESCUENTO*/
#define S_REIMPRESION_NO_PERMITIDA                                  1151  /*REIMPRESION NO PERMITIDA*/
#define S_NO_ES_CLIENTE_DE_LA_FACTURA_ING_OTRO                      1152  /*NO ES CLIENTE DE LA FACTURA. INGRESA OTRO? <ENTER SI - ESC NO>*/
#define S_RULETA_PRESENTACION_1                                     1153
#define S_RULETA_PRESENTACION_2                                     1154
#define S_RULETA_PRESENTACION_3                                     1155
#define S_RULETA_PRESENTACION_4                                     1156
#define S_RULETA_GANADOR_1                                          1157
#define S_RULETA_GANADOR_2                                          1158
#define S_VENTAS_BRUTAS                                             1159
#define S_RULETA_PREDEDOR_1                                         1160
#define S_INGRESOS_NETOS                                            1161
#define S_VTAS_CONTRIBUYENTES                                       1162
#define S_RULETA_INGRESE_NUMERO_1                                   1163
#define S_RULETA_INGRESE_NUMERO_2                                   1164
#define S_NO_HAY_MONTO_DE_F_FIJO							        1165  /* NO HAY MONTO DE FONDO FIJO */
#define S_NIT_CLIENTE                                               1166
#define S_NRI_CLIENTE                                               1167
#define S_DUI_CLIENTE                                               1168
#define S_GIRO                                                      1169
#define S_ERROR_RTA_HARDLOCK                                        1170
#define S_VERSION_DEMO                                              1171
#define S_VERSION_FINAL                                             1172
#define S_VERSION_ERRONEA                                           1173
#define S_AUTORIZADO_HARDLOCK                                       1174
#define S_ERROR_EN_COMANDO                                          1175
#define S_LIMITE_EXCEDIDO                                           1176
#define S_ERROR_PROCESAR_COMANDO                                    1177
#define S_ERROR_BUSCAR_LICENCIA                                     1178
#define S_ERROR_ASIGNAR_LICENCIA                                    1179
#define S_ERROR_DESASIGNAR_LICENCIA                                 1180
#define S_AUTORIZACION_DENEGADA                                     1181
#define S_VENTAS_BRUTAS2                                            1182
#define S___EMPLEADOS                                               1183
#define S___CLTES_DOLAR                                             1184
#define S___CLTES_COLON                                             1185
#define S___CLTES_TARJETAS                                          1186
#define S___CLTES_CHEQUES                                           1187
#define S___CLTES_WESTERN                                           1188
#define S___CLTES_CREDITO                                           1189
#define S___CLTES_C_REGALO                                          1190
#define S___CLTES_O_COMPRA                                          1191
#define S_TOTAL_TRANS_CONTRIB_                                      1192
#define S___EXENTOS                                                 1193
#define S___GRAVADOS                                                1194
#define S_VTAS_CONSUM_FINAL                                         1195
#define S_DEVOL__CONTRIB____                                        1196
#define S_TOTAL_TRANS_C_FACT__                                      1197
#define S_TOTAL_TRANS_C_TIQU__                                      1198
#define S___VTAS_A_DIPLOMATICOS                                     1199
#define S___VTAS_A_EXPORTACION                                      1200
#define S___GRAVADOS_FACTURAS                                       1201
#define S___GRAVADOS_TIQUETE                                        1202
#define S_DEVOL__CONSUM_FINAL___                                    1203
#define S___TKT_GRAVADOS                                            1204
#define S___FACT_GRAVADAS                                           1205
#define S_DEBITOS_FISCALES                                          1206
#define S___IVA_VTAS_CONTRIB_                                       1207
#define S___IVA_VTAS_CONSUM_FINAL                                   1208
#define S_TOTAL_DE_VENTAS                                           1209
#define S___TRANSACCION_INICIAL                                     1210
#define S___TRANSACCION_FINAL                                       1211
#define S___40PUNTOS                                                1212   /*........................................*/
#define S_CAJERO_______                                             1213   /*CAJERO      :*/
#define S_SECUENCIA____                                             1214   /*SECUENCIA   :*/
#define S_FECHA_                                                    1215   /*FECHA:*/
#define S_HORA_                                                     1216   /*HORA:*/
#define S_TOTAL__________                                           1217   /*TOTAL           :*/
#define S_EL_NUMER_CONFIGURADO___s__NO_COINCIDE                     1218   /*EL NUMERO DE SERIE CONFIGURADO (%s) NO COINCIDE*/
#define S_CON_EL_SERIAL_DE_LA_IMPRESORA___s_                        1219   /*CON EL SERIAL DE LA IMPRESORA (%s)*/
#define S_ACTUALIZANDO_PORCENTAJES_DE_IVAS                          1220   /*ACTUALIZANDO PORCENTAJES DE IVAS*/
#define S_IVAS_ACTUALIZADOS                                         1221   /*IVAS_ACTUALIZADOS*/
#define S_COMANDO_FISCAL_DESCONOCIDO___d__x_                        1222   /*COMANDO FISCAL DESCONOCIDO: %d(%x)*/
#define S_Error_IVA___d___Porcentaje___f_                           1223   /*Error IVA(%%d), Porcentaje(%%f)*/
#define S_RULETA_GANADOR_3                                          1224
#define S_RULETA_PREDEDOR_2                                         1225
#define S_RULETA_PREDEDOR_3                                         1226
#define TRAMA_CPU_MUY_LARGA                                         1227   /* Trama CPU muy larga                                          */
#define ERROR_AL_IMPRIMIR_DETALLES_DEL_IVA                          1228   /* Error al imprimir detalles del iva                           */
#define PRECIO_UNITARIO_NO_TIENE_FIN_DE_TRAMA                       1229   /* Precio unitario no tiene fin de trama                        */
#define ERROR_POR_ALTERACION_DE_HORA_O_FECHA                        1230   /* Error por alteracion de hora o fecha                         */
#define ERROR_DE_OVERFLOW_EN_LA_MULTIPLICACION                      1231   /* Error de overflow en la multiplicacion                       */
#define ERROR_DE_OVERFLOW_EN_LA_RESTA                               1232   /* Error de overflow en la resta                                */
#define E_EN_LA_IDENTIFICACION_DE_LA_IMPRESORA                      1233   /* Error en la identificacion de la impresora                   */
#define E_POR_MEMORIA_FISCAL_AGOTADA                                1234   /* Error por memoria fiscal agotada                             */
#define ERROR_DE_IMPRESORA___OFF_LINE                               1235   /* Error de impresora - Off Line                                */
#define ERROR_DE_IMPRESORA___PAPER_OUT                              1236   /* Error de impresora - Paper Out                               */
#define ERROR_DE_IMPRESORA___ERROR_GENERAL                          1237   /* Error de impresora - Error General                           */
#define ERROR_DE_IMPRESORA___MECANICA                               1238   /* Error de impresora - Mecanica                                */
#define ERROR_DE_IMPRESORA___UNRECOVERABLE                          1239   /* Error de impresora - unrecoverable                           */
#define ERROR_DE_IMPRESORA_PAPEL_CASI_AL_FINAL                      1240   /* Error de impresora - Papel casi al final                     */
#define SE_HA_ENVIADO_UN_COMANDO_DE_FACTURA_S                       1241   /* Se ha enviado un comando de factura sin haber comenzado una  */
#define S_PRES_ENTER_INTRODUZCA_CHEQUE_IMPRESORA                    1242  /* Presione enter e introduzca el cheque en la impresora*/
#define S_DEBE_REINICIAR_EL_SISTEMA                                 1243  /* DEBE REINICIAR EL SISTEMA  */
#define S_NO_SE_ENCONTRO_CONFIGURACION                              1244  /* NO SE ENCONTRO CONFIGURACION*/
#define S_REINICIE_LA_IMPRESORA                                     1245  /* REINICIE LA IMPRESORA*/
#define S_REINICIE_IMPRESORA_PRESIONE_UNA_TECLA                     1246  /* REINICIE IMPRESORA Y PRESIONE UNA TECLA*/
#define S_AUTORIZA_SERIAL                                           1247  /* Autoriza Serial*/
#define S_ERROR_EN_CODIGO_IVA___D___ARTICULO                        1248  /* ERROR EN CODIGO IVA( %d ) ARTICULO*/
#define S_ERROR_ITEM_GRAVADO___D_                                   1249  /* ERROR ITEM GRAVADO( %d )*/
#define S_COMANDO_NO_VALIDO                                         1250  /* Comando no valido */
#define S_______REIMPRESION_______                                  1251  /* ***************REIMPRESION***************/
#define S_COMIENZA_REIMPRESION                                      1252  /* comienza reimpresion*/
#define S_ERROR_IMPRESION_REVISE_IMPRESORA_P_E                      1253  /* ERROR DE IMPRESION ! REVISE LA IMPRESORA. (Presione ENTER)*/
#define S_PRES_ENTER_CHEQUE__ESC_REPE                               1254  /* Presione <Enter> si se imprimio bien el cheque, <Esc> para repetirlo */
#define S_EL_CREDITO_NO_PUEDE_EXCEDER_LA_DEUDA                      1255  /* EL CREDITO NO PUEDE EXCEDER LA DEUDA DEL CLIENTE */
#define S_SE_EXCEDIO_EL_MONTO_REGISTRADO		                    1256  /* SE EXCEDIO EL MONTO REGISTRADO */
#define S_LIMITE_DE_COBRO_EXCEDIDO				                    1257  /* LIMITE DE COBRO EXCEDIDO */
#define S_DEUDA_NO_ENCONTRADA					                    1258  /* DEUDA NO ENCONTRADA */
#define S_NO_ES_EL_CLIENTE_ACTUAL				                    1259  /* NO ES EL CLIENTE ACTUAL */
#define S_CODIGO_DE_COBRO_NO_ENCONTRADO			                    1260  /* CODIGO DE COBRO NO ENCONTRADO */
#define S_TIPO_DE_MOVIMIENTO_Y_LETRA_DE_COMPROB	                    1261  /* TIPO DE MOVIMIENTO Y LETRA DE COMPROBANTE NO ENCONTRADOS! */
#define S_NO_HAY_DEUDAS_CARGADAS				                    1262  /* NO HAY DEUDAS CARGADAS */
#define S_NO_SE_PUEDE_COBRAR_SIN_CLIENTE                            1263  /* NO SE PUEDE COBRAR SIN CLIENTE */
#define S_CLIENTE_SIN_CODIGO_NO_ENCONTRADO                          1264  /* CLIENTE SIN CODIGO NO ENCONTRADO */
#define S_SOCIO_PERTENECE_A_EMPRESA_PRES_ENTER                      1265  /* SOCIO PERTENECE A EMPRESA, PRESIONE <ENTER> PARA CONTINUAR */
#define S_NO_SE_REGISTRAN_DEUDAS 				                    1266  /* NO SE REGISTRAN DEUDAS */
#define S_SELECCIONE_EL_CLIENTE_ALT_L_A                             1267  /* SELECCIONE EL CLIENTE: ALT-L Alfabetico, ALT-K Nro. de Cliente */
#define S_CLIENTE_NO_ENCONTRADO 				                    1268  /* CLIENTE NO ENCONTRADO */
#define S_CARGANDO_DEUDAS						                    1269  /* CARGANDO DEUDAS... */
#define S_EVENTO_YA_REPROCESADO 				                    1270  /* EVENTO YA REPROCESADO */
#define S_NO_SELECCIONO_COBRO 					                    1271  /* NO SELECCIONO COBRO */
#define S_REIMPRIMIR_ULTIMO_COBRO				                    1272	 /* REIMPRIMIR ULTIMO COBRO?, ENTER (SI), ESC (NO)*/
#define S_REIMPRIMIENDO							                    1273	 /* REIMPRIMIENDO... */
#define S_NO_HAY_CONEXION_CON_LA_IMPRESORA 		                    1274  /* NO HAY CONEXION CON LA IMPRESORA */
#define S_EL_TIMBRADO_SE_REALIZO_CORRECTAMENTE                      1275  /* EL TIMBRADO SE REALIZO CORRECTAMENTE? S/N */
#define S_LA_IMPRESION_SE_REALIZO_CORRECTAMENTE                     1276  /* LA IMPRESION SE REALIZO CORRECTAMENTE? S/N */
#define S_REINTENTAR_S_N						                    1277  /* REINTENTAR? S/N */
#define S_UTILIZAR_TIMBRADO_OPCIONAL_S_N		                    1278  /* UTILIZAR TIMBRADO OPCIONAL?, S/N */
#define S_NO_SE_ENCUENTRA_EL_ARCHIVO_tkt_caja                       1279  /* NO SE ENCUENTRA EL ARCHIVO: tkt-caja.ini */
#define S_AUTORIZO_IMPORTE_MINIMO_NO_ALCANZADO                      1280  /* Autorizo importe minimo no alcanzado*/
#define S_IMPORTE_MINIMO_NO_ALCANZADO                               1281  /* IMPORTE MINIMO NO ALCANZADO */
#define S_TARJETA___S__ULTIMOS_4_DIG__S__RECHAZADA_ENTER			1282 /* Tarjeta ( %s ) ultimos digitos ( ---%s ) fue rechazada - (Presione ENTER) */
#define S_NO_ALCANZADO_IMPORTE_MINIMO_EN__S							1283 /* NO SE HA ALCANZADO IMPORTE MINIMO EN %s */
#define S_SE_HA_SUPERADO_IMPORTE_MAXIMO_EN__S						1284 /* SE HA SUPERADO IMPORTE MAXIMO EN %s */
#define S_NO_ALCANZADO_IMPORTE_MINIMO_TOTAL_EN_RETIROS				1285 /* NO SE HA ALCANZADO IMPORTE MINIMO TOTAL EN RETIROS */
#define S_SE_HA_SUPERADO_IMPORTE_MAXIMO_TOTAL_EN_RETIROS		    1286 /* SE HA SUPERADO IMPORTE MAXIMO TOTAL EN RETIROS */
#define S_OBTIENE_EVENTOS_TEMPORALES							    1287 /* OBTENIENDO EVENTOS TEMPORALES... */
#define S_TF_CLIENTE_DESEA_CAMBIARLO_S_N							1288 /* TICKET FACTURA YA POSEE UN CLIENTE. DESEA CAMBIARLO? S/N */
#define S__INGRESAR_COMO__							                1289 /* INGRESAR COMO: */
#define S_AUTORIZAR_TARJETA___S__ULTIMOS_4_DIG__S__ENTER		    1290 /* Autorizar tarjeta ( %s ) últimos dígitos ( ---%s ) - (Presione ENTER) */
#define S_BUSQUEDA_NOMBRE		                                    1291 /* NOMBRE DEL CLIENTE*/
#define S_BUSQUEDA_DNI		                                        1292 /* DNI DEL CLIENTE*/
#define S_BUSQUEDA_CUIT		                                        1293 /* CUIT DEL CLIENTE*/
#define S_BUSQUEDA_CODIGO		                                    1294 /* CODIGO DEL CLIENTE*/
#define S_BUSQUEDA_BARRA		                                    1295 /* CODIGO BARRA DEL CLIENTE*/
#define S_BUSQUEDA_GENERICO		                                    1296 /* GENERICO */
#define S_ALTA_DE_CLIENTE		                                    1297 /* ALTA DE CLIENTE */
#define S_ESPACIO		                                            1298 /*                          */
#define S_DNI		                                                1299 /* DNI DEL CLIENTE */
#define S_CODIGO_GENERICO                                           1300 /* CODIGO GENERICO */
#define S_CODIGO_DE_IMPUESTO_NO_PERMITIDO			                1301 /* Codigo de Impuesto no permitido */
#define S_NO_UTILIZA_PROMO_ACTIVADA_S_N 			                1302 /* NO ESTA UTILIZANDO LA PROMOCION ACTIVADA, DESEA CONTINUAR? S/N */
#define S_DESEA_EDITAR_LOS_DATOS_S_N 			                    1303 /* DESEA EDITAR LOS DATOS S/N */
#define S_RETENCION_S_N 			                                1304 /* RETENCION ( S / N ) */
#define S_TOT_ANT__LF_ 			                                    1305 /* TOT ANT: %*.*lf */
#define S_RET__F_ 			                                        1306 /* RET: %*.*f */
#define S_COND_IVA 			                                        1307 /* COND IVA */
#define S_CTA_CTE_S_N 			                                    1308 /* CTA CTE ( S / N ) */
#define S_CHEQUES_S_N 			                                    1309 /* CHEQUES ( S / N ) */
#define S_CODIGO_PERFIL			                                    1310 /* CODIGO PERFIL */
#define S_NO_HAY_DIN_P_COMP_OP                                      1311 /* NO HAY DIENRO PARA COMPLETAR LA OPERACION */
#define S_TIPO_COMPROB_INVALIDO_PARA_OPERACION                      1312 /* TIPO DE COMPROBANTE INVALIDO PARA LA OPERACION */
#define S_RETENCION_X_PORCENTAJE                                    1313 /* RETENCION % */
#define S_RETENCION_DESCUENTO_S_N                                   1314 /* RETENCION DESCUENTO ( S / N )*/
#define S_VALOR_POR_DEFECTO       			                        1315 /* NO HAY MOTIVO CONFIGURADO */
#define S_Codigo_Barra											    1316 /* Codigo de Barra : */
#define S_Nro_Folio											        1317 /* Nro Folio : */
#define S_Articulo											        1318 /* Articulo : */
#define S_REINGRESE__S											    1319 /* REINGRESE %s : */
#define S_ERROR_REINGRESO_REINTENTAR_ENTER_SALIR_ESC				1320 /* Error en el reingreso. Reintentar < ENTER > - Salir < ESC > */
#define S_AUTORIZO_CAMBIO_MEDIOS_NC                                 1321 /* AUTORIZO CAMBIO DE MEDIOS EN NC */
#define S_CAMBIO_MEDIO_EN_NC                                        1322 /* CAMBIO DE MEDIOS EN NC */
#define S_PAGADO_CON_MEDIOS_NO_HAB_PARA_LA_OPERACION                1323 /* PAGADO CON MEDIOS NO HABILITADOS PARA LA OPERACION */
#define S_MEDIO___S__NO_SE_UTILIZO_O_IMPORTE_MAYOR                  1324 /* EL MEDIO %S NO SE UTILIZO O SU IMPORTE ES MAYOR */
#define S_COBROS_ONLINE_TICKET_DESEA_CANCELARLOS_ENTER_ESC          1325 /* COBROS ONLINE EN TICKET, DESEA CANCELARLOS? <ENTER>=SI <ESC>=NO */
#define S_ERROR_AL_ENVIAR_TICKET_EN_ESPERA                          1326 /* ERROR AL ENVIAR TICKET EN ESPERA */
#define S_ERROR_AL_TRAER_COBROS_VENTA_EN_ESPERA                     1327 /* ERROR AL TRAER COBROS VENTA EN ESPERA */
#define S_PRES_UNA_TECLA                                            1328 /* PRES. UNA TECLA */
#define S_NO_HAY_CONF_MEDIO_EN_TAR_MED                              1329 /* NO HAY CONFIGURACION PARA EL MEDIO EN TAR_MED.BTR */
#define S_AUTORIZACION_RETENCIONES                                  1330 /* AUTORIZACION RETENCIONES */
#define S_ERROR_AL_CARGAR_CONF_DE_CLIENTES                          1331 /* ERROR AL CARGAR CONFIGURACION DE CLIENTES*/
#define S_INGRESO_VALOR_INCORRECTO                                  1332 /* INGRESO UN VALOR INCORRECTO */
#define S_LOCALIDAD                                                 1333 /* LOCALIDAD */
#define S_PROVINCIA                                                 1334 /* PROVINCIA */
#define S_ERROR_CANT_DE_DATOS_CONFIGURADOS                          1335 /* ERROR EN LA CANT. DE DATOS CONFIGURADOS */
#define S_ALTA_DE_CLIENTES                                          1336 /* ALTA RAPIDA DE CLIENTES */
#define S_ERROR_DATO_TABLA_tipo_no_definido                         1337 /* ERROR DATO TABLA: tipo no definido */
#define S_INGRESE_DATO_OBLIGATORIO                                  1338 /* INGRESE EL DATO, ES OBLIGATORIO */
#define S_CARGANDO_ARTICULOS                                        1339 /* CARGANDO_ARTICULOS... */
#define S_EL_ARTICULO_NO_ESTA_EN_PADRON                             1340 /* EL ARTICULO NO ESTA EN EL PADRON */
#define S_ACTIVA_PANEL_IMPORTACION                                  1341 /* ACTIVA PANEL IMPORTACION */
#define S_ARTICULO_ENGANCHE                                         1342 /* ARTICULO DE ENGANCHE */
#define S_NO_SE_ENCUENTRA_EL_ARTICULO								1343 /*EL ARTICULO NO SE ENCUENTRA EN EL PADRON LOCAL"*/
#define S_TICKET_ESPERA_CANCELADO									1344 /*SE CANCELA EL RECUPERO DEL TICKET EN ESPERA*/
#define S_NO_SE_ENCUENTRA_EL_CLIENTE								1345 /*EL CLIENTE NO SE ENCUENTRA EN EL PADRON LOCAL"*/
#define S_POR_FAVOR_RETIRE_COMPROB_DEL_SLIP                         1346 /* POR FAVOR RETIRE EL COMPROBANTE DEL SLIP */
#define S_COLOQUE_PAPEL_EN_SLIP                                     1347 /* COLOQUE PAPEL EN SLIP */
#define S_IMPRIMIENDO                                               1348 /* IMPRIMIENDO... */
#define S_NO_SE_PUDO_COMPLETAR_OPERACION_DE_IMPRESION               1349 /* NO SE PUDO COMPLETAR OPERACION DE IMPRESION */
#define S_OPERACION_DE_IMPRESION_INVALIDA                           1350 /* OPERACION DE IMPRESION INVALIDA */
#define S_NO_SE_PERMITE_CAMBIO_PRECIO_EN_ART_POR_IMPORTE            1351 /* NO SE PERMITE EL CAMBIO DE PRECIO EN ART. POR IMPORTE */
#define S_SE_IMPRIMIO_CORRECTAMENTE_COMPROBANTE_ENTER_ESC           1352 /* Se imprimio correctamente el Comprobante? (Enter/Esc) */
#define S_SE_IMPRIMIO_CORRECTAMENTE_VALIDACION_ENTER_ESC            1353 /* Se imprimio correctamente la Validacion? (Enter/Esc) */
#define S_FALTA_CONFIG_CANCELA_CAMBIO_MEDIOS_Pres_Tecla             1354 /* Falta configurar Validación. CAMBIO DE MEDIO CANCELADO ( Presione una tecla )*/
#define S_DEBE_BAJAR_LA_APLICACION                                  1355 /* DEBE BAJAR LA APLICACION */
#define S_NO_HAY_CONEXION_CON_MYSQL_SERVER                          1356 /* No hay Conexion con MySQL Server*/
#define S_NO_SE_PUEDE_REALIZAR_CAMBIO								1357 /*NO SE PUEDE REALIZAR EL CAMBIO*/
#define S_NO_SE_PUEDE_MANDAR_ESPERA 								1358 /*NO SE PUEDE MANDAR TICKET A ESPERA*/
#define S_SELECCIONE_MODO_IMPRESION_1_JOURNAL_2_RECEIPT_3_AMBOS		1359 /*SELECCIONE MODO DE IMPRESION. [1]-JOURNAL, [2]-RECEIPT, [3]-AMBOS*/
#define S_SUPERVISOR__                                              1360 /*SUPERVISOR*/
#define S_ATENCION_EN_LONGITUD_CODIGO_SCANNER                       1361 /*ATENCION EN LONGITUD CODIGO SCANNER*/
#define S_NO_NUMERO_GERENTE										    1362 /*NO ES NUMERO DE GERENTE*/
#define S_DESCUENTO_IMPORTE											1363 /*Descuento $*/
#define S_NO_SE_PUEDE_DEVOLVER_ENVASE_ULTIMO_ARTICULO			    1364 /*NO SE PUEDE DEVOLVER EL ENVASE A ESTE ULTIMO ARTICULO*/
#define S_CAMBIO_DE_MEDIO_CANCELADO_Pres_Tecla					    1365 /*CAMBIO DE MEDIO CANCELADO ( Presione una tecla )*/
#define S_CAMBIO_DE_MEDIO_NO_HABILITADO_CANCELA_CAMBIO_MEDIOS_Pres_Tecla			1366 /*Cambio de Medio no Habilitado. CAMBIO DE MEDIO CANCELADO ( Presione una tecla )*/
#define S_PER__LF_ 																	1367 /*PER: %.0lf*/
#define S_NO_SE_PERMITE_MODIFICAR_TICKET											1368 /*NO SE PERMITE MODIFICAR TICKET*/
#define S_NO_PUEDE_SINCRONIZAR_FECHA_Y_HORA_DEL_CONTROLADOR_FISCAL_YA_HAY_TICKET	1369  /*NO PUEDE SINCRONIZAR FECHA Y HORA DEL CONTROLADOR FISCAL YA HAY TICKET*/
#define S_FECHA_Y_HORA_DEL_SISTEMA_E_IMPRESORA_ESTAN_SINCRONIZADOS			        1370  /*FECHA Y HORA DEL SISTEMA E IMPRESORA ESTAN SINCRONIZADOS*/
#define S_YA_FUERON_SINCRONIZADO_EL_SISTEMA_CON_EL_CONTROLADOR_FISCAL               1371  /*YA FUERON SINCRONIZADO EL SISTEMA CON EL CONTROLADOR FISCAL*/
#define S_PIDIENDO_FECHA_Y_HORA_AL_CONTROLADOR_FISCAL								1372  /*PIDIENDO FECHA Y HORA AL CONTROLADOR FISCAL*///#define S_ARTICULO_DESCUENTO_NO_DEFINIDO									1368 /*ARTICULO DESCUENTO NO DEFINIDO*/
#define S_NO_SE_AUTORIZA_CONSULTA							                        1373  /*NO SE AUTORIZA CONSULTA*/
#define S_CUIT_INVALIDO																1374  /*CUIT INVALIDO*/
#define S_EL_CLIENTE_DEBE_TENER_RAZ_SOC												1375  /*EL CLIENTE DEBE TENER RAZON SOCIAL*/
#define S_EL_CLIENTE_DEBE_TENER_CONDICION_ANTE_EL_IVA								1376  /*EL CLIENTE DEBE TENER CONDICION ANTE EL IVA*/
#define S_ERROR_CLIENTE_FISCAL								                        1377  /*ERROR EN LOS DATOS DEL CLIENTE FISCAL*/
#define S_FECHA_NO_MENOR_AL_DIA_OPERATIVO								            1378  /*LA FECHA NO PUEDE SER MENOR AL DIA OPERATIVO*/
#define S_SOBREGIRO_EXCEDIDO							                            1379  /*SOBREGIRO EXCEDIDO - SUPERO EL LIMITE PERMITIDO*/
#define S_NO_DISPONE_CUPO_USADO							                            1380  /*NO DISPONE DE CUPO USADO*/
#define S_DESEA_CALCULAR_RETENCIONES_LF_ENTER_ESC							        1381  /*DESEA CALCULAR RETENCIONES %02.2f%% <ENTER SI - ESC NO>*/
#define S_RETENCION_MENOR_A							                                1382  /*RETENCION MENOR A */
#define S_DESEA_CALCULAR_PERCEPCIONES_ENTER_ESC                                     1383  /*DESEA CALCULAR PERCEPCIONES <ENTER SI - ESC NO>*/
#define S_PERCEPCION_MENOR_A							                            1384  /*PERCEPCION MENOR A */
#define S_NO_HAY_PAGOS_X_CAJA_CONFIGURADOS							                1385  /*NO HAY PAGOS X CAJA CONFIGURADOS*/
#define S_NO_SELECCIONO_INSTITUCION_NO_SE_REALIZARA_DONACION	                    1386  /*NO SE SELECCIONO NINGUNA INSTITUCION, NO SE REALIZARA LA DONACION*/
#define S_DESEA_SELECCIONAR_INSTITUCION_ENTER_ESC							        1387  /*DESEA SELECCIONAR UNA INSTITUCION? <ENTER>=SI <ESC>=NO*/
#define S_SI_NO_SELECCIONA_INSTITUCION_NO_SE_PUEDE_REALIZAR_DONACION		        1388  /*SI NO SELECCIONA UNA INSTITUCION, NO SE PUEDE REALIZAR LA DONACION*/
#define S_NO_SE_PUEDE_REALIZAR_LA_ANULACION											1389  /*NO SE PUEDE REALIZAR LA ANULACION*/
#define S_CTRL_ANULACION_POR_INC                                                    1390  /*CONTROL ANULACION POR INCONSISTENCIA*/
#define S_NO_SE_PUEDE_CAMBIAR_CONDICION_FISCAL_CLIENTE_Pres_Tecla                   1391  /*NO SE PUEDE CAMBIAR LA CONDICION FISCAL DEL CLIENTE ( Presione una tecla )*/
#define S_NO_SELECCIONO_CLIENTE_CONDICION_FISCAL_CORRECTA_Pres_Tecla                1392  /*NO SELECCIONO EL CLIENTE CON LA CONDICION FISCAL CORRECTA ( Presione una tecla )*/
#define S_EVENTO_CON_PROMOCION_REALIZAR_NC_LIBRE_TOTAL_Pres_Tecla                   1393  /*Evento con Promocion, realizar una Nota de Credito Libre o Total ( Presione una tecla )*/
#define S_ATENCION_CONFIGURADA_IMPRESORA_FISCAL_Pres_Tecla                          1394  /*ATENCION!!!, tiene configurado una IMPRESORA FISCALIZADA ( Presione una tecla )*/
#define S_DESEA_CONTINUAR_MODALIDAD_ENTRENAMIENTO_ENTER_ESC                         1395  /*Desea continuar con la Modalidad Entrenamiento? <ENTER>=SI <ESC>=NO*/


//--- Strings Errores DBRouter // 22 STRING
#define SDBR_PET_NO_ENTRA_TCP_IP 				1700
#define SDBR_ERROR_AL_ENVIAR     				1701
#define SDBR_EMPEZAR_VTRAN_SIN_TERM 			1702
#define SDBR_INCONSISTENCIA_CANT_TRANS 		    1703
#define SDBR_RETORNO_ERROR_FLAG_ON 				1704
#define SDBR_RETORNO_ERROR_TCP_IP_ACK 			1705
#define SDBR_TIME_OUT_RETORNO_TCP_IP_ACK 		1706
#define SDBR_MAXIMO_PETICIONES_EXCEDIDO 		1707
#define SDBR_TABLA_GRABADA_OK 					1708
#define SDBR_TABLA_GRABADA_ERROR 				1709
#define SDBR_TABLA_OBTENIDA_OK 					1710
#define SDBR_TABLA_OBTENIDA_ERROR 				1711
#define SDBR_RETORNO_ERROR_TCP_IP 				1712
#define SDBR_TIME_OUT_RETORNO_TCP_IP 			1713
#define SDBR_VTRAN_INIT 						1714
#define SDBR_GRABANDO_CIERRE 					1715
#define SDBR_CIERRE_GRABADO_OK 					1716
#define SDBR_CIERRE_GRABADO_ERR 				1717
#define SDBR_GRABAR_EV_STATUS1 					1718
#define SDBR_ERROR_ALTAS  						1719
#define SDBR_ERROR_MODIF  						1720
#define SDBR_BUFFER_CONTROLA_OVERFLOW			1721

//--- Strings Drivers // 35 STRING
#define S_INIT_DRIVERS_SECUNDARIOS              2000
#define S_INICIANDO_POWER_DOWN					2001
#define S_POWER_DOWN_INICIADO					2002
#define S_INICIANDO_LLAVE  						2003
#define S_LLAVE_INICIADA     					2004
#define S_INICIANDO_CAJON  						2005
#define S_CAJON_INICIADO     					2006
#define S_INICIANDO_SCANNER						2007
#define S_SCANNER_INICIADO   					2008
#define S_INICIANDO_TARJETA_MAGNETICA			2009
#define S_TARJETA_MAGNETICA_INICIADA			2010
#define S_INICIANDO_PRINTER						2011
#define S_PRINTER_INICIADA						2012
#define S_INICIANDO_TECLADO						2013
#define S_TECLADO_INICIADO						2014
#define S_INICIANDO_TCP_IP 						2015
#define S_TCP_IP_INICIADO						2016
#define S_INICIANDO_PIN_PAD						2017
#define S_PIN_PAD_INICIADO						2018
#define S_INICIANDO_DISPLAY						2019
#define S_DISPLAY_INICIADO						2020
#define S_DRIVERS_SECUNDARIOS_INICIADOS	 	    2021
#define S_INICIANDO_TIMER                       2022
#define S_TIMER_INICIADO                        2023
#define S_CHEQUEANDO_BTRIEVE                    2024
#define S_BTRIEVE_CHEQUEADO                     2025
#define S_INICIANDO_ARCHIVOS                    2026
#define S_ARCHIVOS_INICIADOS                    2027
#define S_INICIANDO_BALANZA                     2028
#define S_BALANZA_INICIADA                      2029
#define S_INICIANDO_ARCHIVOS_CR0                2030
#define S_ARCHIVOS_CR0_INICIADOS                2031
#define S_INICIANDO_TARJETAS                    2032
#define S_TARJETAS_INICIADAS                    2033
#define S_ENTRANDO_LAZO_PRINCIPAL               2034

//--- No poner stringt acá // 57 STRING
#define S_COMPILE_EXPR__CASO__I_NO_CONTEMPLADON                         3001  /*Compile_expr: Caso %i no contempladon*/
#define S_CASO_DE_BENEF_NO_CONTEMPLADO                                  3002  /*Caso de benef no contemplado*/
#define S_CASO_NO_CONTEMPLADO_                                          3003  /*Caso no contemplado!*/
#define S_W                                                             3004  /*w*/
#define S_TO_DISK__CANNOT_OPEN_FILE__S_                                 3005  /*to_disk: Cannot open file %s.*/
#define S_TO_DISK__CANNOT_CLOSE_FILE__S_                                3006  /*to_disk: Cannot close file %s.*/
#define S_R                                                             3007  /*r*/
#define S_FROM_DISK__CANNOT_OPEN_FILE__S_                               3008  /*from_disk: Cannot open file %s.*/
#define S_FROM_DISK__CANNOT_CLOSE_FILE__S_                              3009  /*from_disk: Cannot close file %s.*/
#define S_BENEFICIO_NO_CONTEMPLADO_                                     3010  /*Beneficio no contemplado.*/
#define S_EXPRESION_NO_CONTEMPLADA_PARA_LA_                             3011  /*Expresion no contemplada para la evaluacion.*/
#define S_SUMA_MAL_TIPADA                                               3012  /*SUMA MAL TIPADA*/
#define S_RESTA_MAL_TIPADA                                              3013  /*RESTA MAL TIPADA*/
#define S_NEGATIVE_ARITHMETIC_MULTI_SET                                 3014  /*Negative Arithmetic Multi Set*/
#define S_DIVISION_BY_ZERO                                              3015  /*Division by zero*/
#define S_EVIN__DAY__                                                   3016  /*EvIN: Day: */
#define S_EVIN__ALGO_NO_CONTEMPLADO_                                    3017  /*EvIN: Algo no contemplado.*/
#define S_EVCOMPLEMENT__CASO_NO_CONTEMPLADO                             3018  /*EvCOMPLEMENT: caso no contemplado*/
#define S_EVSIZE__CASO_NO_CONTEMPLADO                                   3019  /*EvSIZE: caso no contemplado*/
#define S_EVLENGTH__CASO_NO_CONTEMPLADO                                 3020  /*EvLENGTH: caso no contemplado*/
#define S_EVBYMEANS___ESTO_NO_DEBERIA_PASAR_                            3021  /*EvBYMEANS: (esto no deberia pasar)*/
#define S_BELONGS__CASO_NO_CONTEMPLADO__                                3022  /*BELONGS: caso no contemplado: */
#define S_SWITCH_COMMANDO___CASO_NO_CONTEM                              3023  /*switch(commando)->Caso no contemplado.*/
#define S_ERROR___S                                                     3024  /*ERROR: %s*/
#define S_INTENTO_DE_POPEAR_EXPR_STACK_VACIO                            3025  /*Intento de popear expr_stack vacio*/
#define S_INTENTO_DE_POPEAR_BENEF_STACK_VACIO                           3026  /*Intento de popear benef_stack vacio*/
#define S_INTENTO_DE_POPEAR_PARAM_STACK_VACIO                           3027  /*Intento de popear param_stack vacio*/
#define S_INTENTO_DE_POPEAR_PROMO_STACK_VACIO                           3028  /*Intento de popear promo_stack vacio*/
#define S_INTENTO_DE_SWAPEAR_UNA_PILA_VACIA_                            3029  /*Intento de swapear una pila vacia.*/
#define S_INTENTO_DE_SWAPEAR_UNA_PILA_CON_                              3030  /*Intento de swapear una pila con un solo elemento.*/
#define S_INTENTO_DE_SWAPEAR_2_UNA_PILA_VACIA_                          3031  /*Intento de swapear_2 una pila vacia.*/
#define S_INTENTO_DE_SWAPEAR_2_UNA_PILA1                                3032  /*Intento de swapear_2 una pila con un solo elemento.*/
#define S_INTENTO_DE_SWAPEAR_2_UNA_PILA2                                3033  /*Intento de swapear_2 una pila con solo dos elementos.*/
#define S_VALSTACK_C__ESTO_NO_DEBERIA_OCURRIR_                          3034  /*valstack.c: Esto no deberia ocurrir nunca!*/
#define S_UNREF_E__CASO_NO_CONTEMPLADO                                  3035  /*unref_e: Caso no contemplado*/
#define S_SOLO_IMPORTACIONES_HASTA                                      3036  /*SOLO IMPORTACIONES HASTA*/
#define S_SOLO_SE_ACEPTAN_IMPORTACIONES_CON_FECHA                       3037  /*SOLO SE ACEPTAN IMPORTACIONES CON FECHA:*/
#define S_ARTICULO_DE_DESCARGA_NO_ENCONTRADO                            3038  /*ARTICULO DE DESCARGA NO ENCONTRADO*/
#define S_CLIENTE_NO_ESTA_INGRESADO                                     3039 /*CLIENTE NO ESTA INGRESADO*/
#define S_YA_EXISTE_UN_CLIENTE_CARGADO                                  3040 /*YA EXISTE UN CLIENTE CARGADO*/
#define S_DESEA_REALIZAR_EL_ABONO_DE_ESTA_TARJETA_ENTER_ESC             3041 /*DESEA REALIZAR EL ABONO DE ESTA TARJETA? <ENTER / ESC>*/
#define S_COBRO_YA_ANULADO                                              3042 /*COBRO YA ANULADO*/
#define S_SE_IMPRIMIO_CORRECTAMENTE_ENTER_ESC                           3043 /*Se imprimio correctamente? (Enter/Esc)*/
#define S_NUMERO_DE_SECUENCIA_INVALIDO                                  3044 /*NUMERO DE SECUENCIA INVALIDO*/
#define S_ERROR_GRABANDO_DOSIFICACIONES                                 3045 /*ERROR GRABANDO DOSIFICACIONES*/
#define S_SE_PERDIO_CONECCION_AL_ACTUALIZAR_VENTAS_RECONECCION_FALLO    3046 /*SE PERDIO LA CONECCION AL ACTUALIZAR VENTAS Y LA RECONECCION FALLO*/
#define S_BTRIEVE_NO_CARGADO                                            3047 /*BTRIEVE NO CARGADO*/
#define S_MENSAJE_ENVIADO                                               3048 /*MENSAJE ENVIADO !*/
#define S_RECONECCION_fallo                                             3049 /*RECONECCION fallo*/
#define S_FALLOS_DETECTADOS_EN_LA_RAM_NO_VOLATIL                        3050 /*FALLOS DETECTADOS EN LA RAM NO VOLATIL*/
#define S_HAY_VALORES_ERRONEOS_EN_LA_RAM_NO_VOLATIL                     3051 /*HAY VALORES ERRONEOS EN LA RAM NO VOLATIL !*/
#define S_ERROR_NO_SE_ENCONTRO_FORMULA                                  3052 /*ERROR: NO SE ENCONTRO FORMULA*/
#define S_ERROR_AREA_AUX7_USADA                                         3053 /*ERROR: AREA AUX7 USADA*/
#define S_MENSAJE_SIN_CMR                                               3054 /*MENSAJE SIN CMR*/
#define S_PUNTO_DE_VENTA_TPV_1000_IV                                    3055 /*PUNTO DE VENTA INVEL TPV - IV*/
#define S_LA_TRANSACCION_ESPECIFICADA_NO_EXISTE                         3056 /*LA TRANSACCION ESPECIFICADA NO EXISTE*/
#define S_USUARIO 									                    3057 /*Usuario*/
#define S_HA_ALCANZADO_LIMITE_MEDIOS_PAGO                               3058 /* HA ALCANZADO EL LIMITE DE MEDIOS DE PAGO (%d) */
#define S_DEBE_REALIZAR_UN_ARQUEO		                                3059 /*Debe realizar un ARQUEO - presione un tecla*/
#define S_CAJERO_FLOTANTE_OFFLINE		                                3060 /*INGRESO DE CAJERO FLOTANTE SIN CONEXION DE RED*/
#define S_LOGIN_NO_AUTORIZADO			                                3061 /*NO SE AUTORIZA EL INGRESO DE CAJERO*/
#define S_ERROR_CUPON__I__I                                             3062 /*Error cupon %i %i, COD_NUMERACION ERRONEO, NO PUEDE SER MAYOR O IGUAL A 20*/

#pragma pack (pop)
