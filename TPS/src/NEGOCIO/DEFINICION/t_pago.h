#ifndef T_PAGO_H
#define T_PAGO_H
int BORRA_EVENTO( long nro_z, long id_evento );
int CERRAR_ARCHIVOS_EVENTOS_HISTORICOS();
int ABRIR_ARCHIVOS_EVENTOS_HISTORICOS();
int TECLA_PAGO();
int VERIFICA_ANULACIONES_AUTORIZADAS();
void VERIFICA_EL_TOTAL();
int AUTORIZAR_PRECIOS();
int VERIFICA_SUPERA_LIMITE_AL_FINAL();
void OBTIENE_PUNTEROS_A_EVENTOS( struct _eventos **ticket, 
								 struct _ev_cont **e_ticket,
                                 struct _ev_medios **pago, 
								 struct _ev_cod_barra **cod_barra,
                                 struct _ev_cierre **cierre, 
								 struct _ev_super **super, 
								 struct _ev_dmedios **dpago,
                                 struct _ev_despro **e_despro, 
                                 struct _ev_bpromo **e_b_promo,
								 struct _ev_factu **factura,
							     long nro_z );
void PROCESA_DESCUENTOS_ESPECIALES();
void ACTUALIZA_PUNTOS_CLIENTES();
int BUSCAR_ARTICULO_DESCUENTO( long id_evento );
void REVERSAR_PAGO_BOLETA_PLATA_EN_DEVOLUCIONES();
void INSERTAR_REVERSA_BOLETIN( struct _ev_dmedios dpago, double importe );
void REVERTIR_ARTICULO_DESCUENTO( void );
void CALCULA_AHORRO_OBTENIDO( void );
int CONSOLIDAR_E_TICKET( long id_eventos, int /*area*/ tabla, int tipo_t, struct _ev_cont *ev,
                         struct _art *art, int *p );
int       AGRUPAR_ARTICULOS_TRANSACCION_ACTIVA( void );
#endif
