#ifndef TKT_H
#define TKT_H
void TKT_ejecutar_comando( int *ind_config, double precio, int h, int h2 );
void INIT_COMPROBANTE_TKT();
void TKT_FISCAL_B();
void TKT_CORTE_PAPEL();
void IMPRIMIR_TKT_CUERPO( double precio, int indice );
void IMPRIMIR_TKT_CABECERA( int indice );
void IMPRIMIR_TKT_TITULO_TICKET();
void IMPRIMIR_TKT_IVAS();
void IMPRIMIR_TKT_AFINIDAD();
void IMPRIMIR_TKT_NO_AFINIDAD();
void IMPRIMIR_TKT_IDENTIFICACION();
void IMPRIMIR_TKT_PIE();
void TIMBRADO_TKT( int indice );
void IMPRIMIR_TKT_PAGO_REINTEGRO();
void TIMBRADO_TKT_REINTEGRO();
int IMPRIMIR_TKT_TOTAL( double importe, int modo );
void IMPRIMIR_TKT_VUELTO( int h );
void IMPRIMIR_TKT_DONACION();
void IMPRIMIR_TKT_MEDIOS( int h, int h2 );
int _COMPROBANTE_TKT( int comienzo, int final );
int TKT_evaluar_condiciones( int codigo, /*int *ind_config,*/ int h );
INT16 TKT_tomar_entero( int *ind_config, int *indice );
void TKT_SET_FILA( int _fila );
int TKT_desplazar_config( int codigo, int *indice );
void IMPRIMIR_TKT_SUBTOTAL();
void TKT_incrementar_puntero( int codigo, int *ind_config );
void TKT_TOTALIZAR_TEMPORAL( int tipo_evento, int sub_evento, int disc_medios, int filtra_medios,
                             int cod_medio_sub_medio );
void TKT_AGREGAR_EVENTO_A_TMP( int disc_medios, int filtra_medios, int cod_medio_sub_medio );
int TKT_EVENTO_SOLICITADO( int tipo_evento, int sub_evento );
int IMPRIMIR_TKT_INF00( int ini_est, double *dif, double *dif_me );
void INFORME_TARJETAS( int est );
void OBTENER_DATO_TABLA( int codtabla, int orden, char *indice, char *tipores,
                         char offset[200]/*, int linea*/ );
int IMPRIMIR_TKT_FONDO_FIJO( );
int IMPRIMIR_TKT_INF05( int ini_est );
int IMPRIMIR_TKT_INF10( int ini_est );
int IMPRIMIR_TKT_INF15( int ini_est );
int IMPRIMIR_TKT_INF10_CABECERA( );
int IMPRIMIR_TKT_INF_DIFERENCIAS();
int IMPRIMIR_TKT_INF_DIFERENCIAS_SUBMEDIOS_CONTINUO();
long ULTIMO_ID_EVENTO( int tipo_evento );
long CANTIDAD_EVENTOS_ORIGINAL( int tipo_evento ); 
long CANTIDAD_EVENTOS( int tipo_evento , int evento_bak);
long ID_EVENTO_NRO( int nro_evento, int tipo_evento );
void	  IMPRIME_TKT_DESCRIPCION_CUERPO( int indice );
void      IMPRIME_TKT_DESCRIPCION_TARJETA( int indice, int indice2 );
void      IMPRIME_TKT_IMPORTE_TOTAL( int indice );
void      IMPRIMIR_TKT_TICKET_ESPERA( void );
void      EXISTE_PRINTER( void );
void	  TKT_total_en_letras( int *ind_config );
void      LETRA_IMPUESTO( char cod_impuesto, int iva, double exento, char*letra );
void      IMPRIME_COTIZACION( int x_loc , int y_loc , int h );
void      IMPRIME_IMPORTE_INGRESADO( int x_loc, int y_loc, int h );
void      RESOLVER_FUNCIONES( char *funciones, char *RtaConsulta, int linea );
void      IMPRIMIR_TKT_FRANQUEO( int modo );

int IMPRIMIR_TKT_INFORME_SUPERVISOR();
void IMPRIMIR_TKT_CABECERA_SUP();
void IMPRIMIR_TKT_CABECERA_DIF_SUB();
void ARMAR_STRING_FOLIO( char *cad );
void IMPRIMIR_TKT_INFORMACION_COMPARATIVA( void );
void IMPRIMIR_TKT_FIN_TICKET( );
void IMPRIME_TKT_RETIRO(); //Seba
#endif
