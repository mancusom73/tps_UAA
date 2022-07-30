#ifndef EVENTOS_H
#define EVENTOS_H

#define  _EVENT_TICKET                          0
#define  _EVENT_COBRO                           1
#define  _EVENT_PAGO                            2//_ev_medios 
#define  _EVENT_FAC_A                           3
#define  _EVENT_FAC_B                           4
#define  _EVENT_NC_A                            5
#define  _EVENT_NC_B                            6
#define  _EVENT_RETIRO                          7
#define  _EVENT_INFORME                         8
#define  _EVENT_Y                               9
#define  _EVENT_Z                               10
#define  _EVENT_REMITO                          11
#define  _EVENT_ANULA_COMPROBANTE               12
#define  _EVENT_INVENTARIO                      13
#define  _EVENT_FACTURA_A_REMITO                14
#define  _EVENT_FACTURA_B_REMITO                15
#define  _EVENT_FAC_A_REMOTA                    16
#define  _EVENT_FAC_B_REMOTA                    17
#define  _EVENT_REMITO_REMOTO                   18
#define  _EVENT_NC_A_REMOTA                     19
#define  _EVENT_NC_B_REMOTA                     20
#define  _EVENT_DEVOLUCION                      21
#define  _EVENT_COBRO_TARJETA                   22
#define  _EVENT_ANULA_COBRO_TARJ                23
#define  _EVENT_NC_AUTOMATICA                   24
#define  _EVENT_FONDO_FIJO                      25
#define  _EVENT_INICIO_CAJERO                   26
#define  _EVENT_FIN_CAJERO                      27
#define  _EVENT_INICIO_STANDBY                  28
#define  _EVENT_FIN_STANDBY                     29
#define  _EVENT_TICKET_ESPERA                   30
#define  _EVENTO_COBRO_UNIPAGO                  31
#define  _EVENTO_ANULA_COBRO_UNIPAGO            32
#define  _EVENTO_DOSIFICACION                   33

#define  _EVENT_TICKET_FACTURA_A                35
#define  _EVENT_TICKET_FACTURA_B                36
#define  _EVENT_REIMPRESION_COMPROBANTE         37
#define  _EVENT_TKT_FAC_A_ESPERA                38
#define  _EVENT_TKT_FAC_B_ESPERA                39
#define  _EVENT_CAMBIO_MEDIO                    40
#define  _EVENT_COMEDOR                         41
#define  _EVENT_ARTICULO_NO_ENCONTRADO          42

#define  _EVENT_DEVOLUCION_COBRANZA_ESPECIAL    43//carrefour
#define  _EVENT_ANULA_RETIRO                    44//carrefour

#define  _EVENT_ND_A                            45 // Se crea para Tarquino
#define  _EVENT_ND_B                            46 // Se crea para Tarquino

#define  _EVENT_MODO_ETIQUETA                   48

#define  _EVENTO_TRANSFERENCIAS                 49//ecuador
#define  _EVENTO_ANULACION_DE_COBRO             50//ecuador y SMG

#define  _EVENT_CAMBIO_PADRON_ARTICULO          51
#define  _EVENT_GENERA_REMITO                   52
#define  _EVENT_CAMBIO_CLAVE                    53
#define  _EVENT_DEVOLUCION_COBRO_TARJETA        54
#define  _EVENT_ANULA_DEVOLUCION_COBRO_TARJETA  55
#define  _EVENT_TICKET_COBRANZA_ESPECIAL        56
#define  _EVENT_FONDO_FIJO_AUTOMATICO           57
#define  _EVENT_CAMBIO_DE_PREMIO                58
#define  _EVENT_DATA_ENTRY                      59
#define  _EVENTO_COBRO_PAGO_FACIL               60
#define  _EVENTO_ANULA_COBRO_PAGO_FACIL         61
#define  _EVENTO_TOTALES_FISCAL                 62
#define  _EVENTO_INGRESA_ENVASE                 63

#define  _EVENTO_RECEPCION_ENVASES              65
#define  _EVENTO_ENVIO_WU                       66
#define  _EVENTO_ANULACION_ENVIO_WU             67
#define  _EVENTO_RECEPCION_WU                   68
#define  _EVENTO_ANULACION_RECEPCION_WU         69

#define  _EVENTO_COBRO_ESPECIAL                 70
#define  _EVENTO_DEVOLUCION_COBRO_ESPECIAL      71
#define  _EVENT_PAGO_ESPECIAL                   72
#define  _EVENT_REPROCESO_COBRO_ESPECIAL        73
#define  _EVENT_VALORES_EN_SUCURSAL             74

#define  _EVENT_DEBIT_PURSE_PUNTOS              90
#define  _EVENT_DEBIT_NOTA_CRED_PUNTOS          91


#define  _EVENT_ANULADO                         99


/*CARREFOUR                           _EVENT_DEVOLUCION_COBRO_TARJETA        28
CARREFOUR                           _EVENT_ANULA_DEVOLUCION_COBRO_TARJETA  29
CARREFOUR                           _EVENTO_TICKET_COBRANZA_ESPECIAL       38
CARREFOUR                           _EVENT_FONDO_FIJO_AUTOMATICO           57
CARREFOUR                           _EVENT_ANULACION_FONDO_FIJO_AUTOMATICO 64*/

void ABRIR_ARCHIVOS_EVENTOS();
void ABRIR_ARCHIVOS_EVENTOS_ALL();
int ABRIR_ARCHIVOS_EVENTOS_LOCAL();
int ABRIR_ARCHIVOS_EVENTOS_BACKUP();
void CERRAR_ARCHIVOS_EVENTOS_LOCAL();
void CERRAR_ARCHIVOS_EVENTOS();
void CERRAR_ARCHIVOS_EVENTOS_ALL();

int ACTUALIZA_ID_ALL();
int GRABAR_EVENT_TICKET( int grabar_all );
int GRABAR_EVENT_TICKET_ALL();
void GRABAR_EVENT_E_TICKET( int h, int grabar_all, int cod_barra_art, char origen, int tipo, int marcar_item );
int GRABAR_EVENT_E_TICKET_ALL();
int DELETE_EVENT_E_TICKET( int h, int grabar_all );
int DELETE_EVENT_E_TICKET_ALL( int h );
void GRABAR_EVENT_PAGO( int grabar_all );
int GRABAR_EVENT_PAGO_ALL();
void GRABAR_EVENT_VARIOS( char *buffer, int l_buffer, int grabar_all );
int GRABAR_EVENT_VARIOS_ALL( int l_buffer );
void BACKUP_EVENTOS();
int COPIAR_EVENTOS_ANTERIORES();
int COPIAR_EVENTOS_ACTUAL();
int COPIA_EVENTOS_A_PENDIENTES_ANTERIORES( void );
int GRABAR_EVENTOS_EN_PENDIENTES_Z_ANTERIOR( long nro_z, long id_evento );
void REPROCESAR_EVENTOS( int cant );
void GRABA_EVENTO_ANULADO();
void RETIRAR_TODOS_LOS_COMPROBANTES();
int DEBE_GRABAR_ALL();
int BUSCAR_EVENT_TICKET( long id );
int EVENTO_CON_PAGOS();
void BORRAR_EVENT_TICKET( int borrar_all );
int SECCION_DE_CAJA();
int CARGAR_PAGO_A_RNV( long id );
int GRABAR_EVENTO_PROMO( struct _acu_art *acu_art, int p_art, int id );
int REVERTIR_EVENTO_PROMO();
void ACTUALIZA_ID_EVENTO_NEW();
int CARGAR_ARRAY_NRO_EVENTOS();
void GRABAR_EVENT_SUPERVISOR( int cod_sup, long legajo, int ok, int funcion, int grabar_en_event );
int INCREMENTAR_NRO_EVENTO( int tipo_evento );
void GRABAR_EVENTO_DESC( char tipo_motivo, int id_motivo );
void REVERTIR_EVENTO_DESC_PROMO( int motivo_descuento, int id_motivo );
void GRABAR_EVENT_FACTURA( int nro_cuerpo );
void GRABAR_SUPER_REC_A_EVENT( long idEvento );
int LIBERAR_ARRAY_NRO_EVENTOS( void );
void VACIAR_ARCHIVOS_EVENTOS( int pTipo );
void ACTUALIZAR_FECHA_EVENTOS( );
#endif
