#ifndef IMPORT_T_H
#define IMPORT_T_H

 struct _comprob_importados 
 {
    long id_evento;
    long caja_z;
    INT16 caja;
    long secuencia;
    UINT16 fecha;
    int tipo_dev;
 };

int OBTENER_EVENTOS_TEMPORALES( int ev_requerido, int caja, long nro_ticket, long secuencia,
	int fecha, long id_evento, char origen, int online, int *res_online, int comprob_original);
int OBTENER_EVENTOS_TEMPORALES_LOCAL( int ev_requerido, int caja, long nro_ticket, long secuencia,
	int fecha, long evento, char origen, int comprob_original );

int OBTENER_EVENTOS_TEMPORALES_ONLINE( int ev_requerido, int caja, long nro_ticket, long secuencia,
	int fecha, long evento, char origen );
int ABRIR_EVENTOS_TEMPORALES( struct _eventos *eventos, struct _ev_cont *ev_cont,
                              struct _ev_medios *ev_pago, struct _ev_despro *ev_despro,
                              struct _ev_dmedios *ev_dpago, struct _ev_cod_barra *ev_cod_barra, 
                              struct _ev_bpromo *ev_b_promo );

int VERIFICA_DATOS_IMPORTACION( int remoto, int ev_requerido, int fecha, long nro_ticket, int caja,
                                long secuencia );

int BUSCA_TICK_ALL( int fecha, long nro_ticket );
int CARGA_ITEM( int h, double *base_dto );
int PIDE_DATOS_IMPORTAR_T( int remoto, char *titulo, int modo );
int IMPORTAR_TICKET( int remoto, int ev_requerido, int reimpresion, char *titulo,
                     int pedir_supervisor, char origen, int modo ,int comprob_original, int dias_limites);
int AGREGAR_ITEMS_AL_TICKET( int remoto );
void PANTALLA_IMPORTAR_T( char *titulo );
void CERRAR_EVENTOS_TEMPORALES();
void REIMPRIMIR_TICKET_REMOTO();
void IMPORTAR_DESCUENTOS( int remoto, long id, struct _ev_despro *ev_despro );
int CONTAR_TICKET_ESPERA(int ev_requerido, int caja, long nro_z, long secuencia, long nro_ticket, long fecha );
int REIMPRIMIR_TICKET_LOCAL(char *archivo);
void ASIGNAR_PRECIO_PADRON();
void      ASIGNAR_PRECIO_PADRON( );
int LOCAL_OBTENER_TABLA( int nombre_tabla_local, long caja, long secuencia, long evento,
						int ev_requerido, int origen, long nro_ticket, int fecha, long *caja_z );

int       VERIFICAR_TICKET_ESPERA_PROCESADO( long caja_z, long id_evento );
int       CONTAR_TICK_ESP_SIN_PROCESAR( long secuencia, int estado );
int       VERIFICAR_ESTADO_TICK_ESP( long nro_caja, long nro_secuencia, int estado );
void      AGRUPAR_ART_IMPORT( int permite_exclusion_art, int agrupar_des,
		                  int agrupar_precio, int _operacion );
void      GUARDAR_INFORMACION_IMPORTACION( int tipo_dev );
int       CARGAR_INFORMACION_IMPORTACION( long id_evento, long caja_z,
                          struct _comprob_importados *pComp_imp );
int CAMBIO_CLIENTE( char tipo_evento_original, long nro_cliente_evento, int cod_financiacion, char origen );
int COMPARAR_DATOS_IMPORTACION( void );
int CARGAR_ARTICULOS_IMPORT_EN_PANEL(  );
int ManipularImportacion(  int cant );
void agregarImportacion( char *s, char *s1, char *s2, char *s3);
void AGREGAR_ITEMS_IMPORT_EN_PANEL_AL_TICKET(  );
int MARCAR_ARTICULOS_IMPORT_EN_PANEL( int registro );
void SETEAR_COMPROBANTE( int tipo_comprobante );
int VERIFICAR_TIPO_COMPROBANTE( int tipo_evento_original );
int VALIDAR_CLIENTE_PARA_TIPO_COMPROBANTE(  );
int VALIDAR_CAMBIO_DE_CLIENTE( int nro_cliente_ant, int pedir_cliente, int origen, int operacion );
int VALIDAR_CAMBIO_DE_CLIENTE_PARA_COMPROBANTE(  long nro_cliente, int operacion );
int TIPO_COMPROBANTE_DEPENDIENTE_DEL_CLIENTE(  int cond_iva_cli );
int VERIFICAR_EVENTO_EN_PROMOCION( void );

#endif
