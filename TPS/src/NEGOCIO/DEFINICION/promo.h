#ifndef PROMO_H
#define PROMO_H
void ACUMULA_IVA( int h, struct _acu_iva *acu, double cantidad );
int CANTIDAD_ARTICULOS_EN_PROMOCION( int cod_promo, struct _pro_art *pro_art_sic );
int AGREGAR_ARTICULOS_DE_PROMOCION( int cod_promo, struct _art_ben *art_ben,
                                    struct _acu_art *acu_art, int p_art, float cant_min_canasta,
                                    int id_regitro, int limite_entrega, double **base_descuento );
void AGREGAR_DESCUENTO_ARTICULO( long cod_art, char* cod_bar, double precio, float cantidad, int id_registro, int origen );
double MODIFICAR_PUNTOS( double puntos, char modificar_puntos );
int PROMOCION_VIGENTE( int modo, struct _promo_sic *promo_sic, struct _pro_med_sic *pro_med_sic,
                       double *coef_medios, double *total_medios, struct _pro_fid *pro_fid,
                       struct _pro_plu *pro_plu, struct _acu_art *acu_art, int p_art );
void ACUMULAR_BASE_DESCUENTO( double **base_descuento, struct _acu_art *acu_art, int p_art,
                              double porc, int dpto, int pos, double cant );
void PROMOCION_DESCUENTO_PUNTOS( struct _acu_art *acu_art, int p_art, char *procesar,
                                 double **base_descuento );
void MOSTRAR_PROMOCION_CONSULTA( char *nombre_promo, char *promo, char *beneficio,
                                 char *inf_adicional, int *pos );
void MOSTRAR_ARTICULOS_AGREGADOS( char *nombre_promo, int cod_promo, char *promo,
                                  struct _art_ben *art_ben, int *pos );
char *OBTENER_STRING_TIPO_PROMO( int tipo_promo );
char *OBTENER_STRING_BENEFICIO_PROMO( int benef_promo );
int VERIFICA_PROMO_FID( int cod_promo, struct _pro_fid *pro_fid );
void AGREGAR_ART_PROMO( long cod_art, float cant, double coef, int enganchar,
                        double **base_descuento, struct _acu_art *acu_art, int p_art, int h );    
void CEREAR_VARIABLES_DE_PROMOCION();
void MARCAR_ARTICULOS_A_PROCESAR( char *procesar, int depto );
void RECALCULAR_PRECIO_PROMOCION( int h, double importe );
int CALCULAR_PROMOCIONES( int modo );
int ARTICULO_EN_PROMOCION( int cod_promo, struct _acu_art *art, int controlar_cant,
                           short int *cantidad );
void REVERTIR_PROMOCIONES();
void MARCAR_ARTICULOS_A_PROCESAR_ART( char *procesar, long cod_art );
void MOSTRAR_BONOS( void );
void ACUMULAR_BONOS();
int AGREGAR_CUPONES_DE_PROMOCION( int formato, unsigned cantidad, char lugar_impresion,
                                  double valor, char *leyenda, struct _promo_sic *promo );
void IMPRIMIR_CUPONES_DE_PROMOCION( char lugar_impresion );
int CARGA_ACU_ART( struct _acu_art *acu_art, int l, int permite_exclusion_art, double *total_cobros,
                   int agrupar_des, int agrupar_precio );
void REDISTRIBUIR_DESCUENTOS( double **base_descuento, struct _acu_art *acu_art, int p_art );
void BUSCAR_PUNTOS( int cod_promo, double *puntos, double *descuento );
long CLASE_OF( long cod );
int LINEA_OF( long cod );
int SUBLINEA_OF( long cod );
void MARCAR_ARTICULOS_A_PROCESAR( char *procesar, int depto );
void CARGAR_PROMO_MEMORIA();
int PROMO_EN_MEMORIA( int cod_promo, int busca_id );
int ARTICULO_INCLUIBLE_EN_PROMOCION( int h );
int ABRE_ARCHIVOS_PROMOCIONES( char *promo_sic, char *pro_art, char *pro_dep, char *pro_med,
                               char *pro_clas, char *art_ben, char *pro_fid, char *pro_plu );
int DEPARTAMENTO_EN_PROMOCION( int cod_promo, int dep );
int CLASIFICACION_EN_PROMOCION( int cod_promo, struct _acu_art *art );
int ACEPTA_PROMOCION( struct _promo_sic *promo_sic, struct _pro_med_sic *pro_med_sic,
                      struct _ind_pro_med_sic *ind_pro_med );
int MEDIO_EN_PROMOCION( int cod_promo, int h, struct _pro_med_sic *pro_med_sic );
int DEBE_PEDIR_SI_ACEPTA_PROMOCION( char cod_medio, char marca, char nro_orden );
int BUSCA_ACU_PROMO( struct _acu_art *acu_art, int h );
void LIBERAR_PROMO_MEMORIA();
void UBICAR_DESCUENTO( double *acumulado );
void ACUMULAR_TOTALES_IVA( struct _acu_iva *acu, struct _acu_iva *iva_art );
double CALCULAR_VALOR( float cant, double importe, float porcentaje, double monto, int h,
                       double *porc_base_dto, struct _promo_sic *promo );
int CALCULAR_VIGENCIA_CUPON_PROMO( struct _promo_sic *promo );
int PROMOCION_PLU( struct _promo_sic *promo_sic, struct _pro_plu *pro_plu_sic,
                   struct _acu_art *acu_art, int p_art );
void MOSTRAR_CUPONES_DE_PROMOCION();
void RECALCULAR_BASE_DESCUENTO( double **base_descuento, double porc );
char SUBMEDIO_EN_PROMOCION( int cod_medio, int pos_pago, char *cod_submedio );
void SALVADOR_RESETEAR_TICKET_CANT_DESC( void );
void AGREGAR_PROMOCION_DESCUENTO( struct _acu_iva *acu, long cod_art, float porc_desc,
                                  char *procesar, int id_registro, double **base_descuento );
void _AGREGAR_PROMOCION_DESCUENTO( struct _acu_iva *acu, long cod_art, char* cod_bar, float porc_desc,
                                   char *procesar, int id_registro, double **base_descuento,
                                   char origen, int exento, int cod_iva );
//void ACUMULA_IVA_ART( long codInt, char* codBar, struct _acu_iva **acu_gravado, struct _acu_iva *acu_excento, double monto );                                   
void ACUMULA_IVA_ART( int h, long codInt, char* codBar, struct _acu_iva (*acu_gravado)[], struct _acu_iva *acu_excento, double monto );                                   
#endif
