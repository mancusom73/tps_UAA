#ifndef PAGOS_H
#define PAGOS_H

double PAGOS_TOTAL_A_PAGAR();
double PAGOS_TOTAL( int redondeo );
double _VARIACION_TARJETA( double importe, float porc, double *ret_var, double *iva2,
                           long art_variacion, int modificar );
double PAGOS_PIDE_IMPORTE( char *titulo, int enteros, int decimales, int signo );
char *PAGOS_OBTENER_NOMBRE( char medio, char submedio );
double PAGOS_ME_A_ML( double importe_me, int h );
double PAGOS_ML_A_ME( double importe_me, int h );
int PAGOS_OBTENER_VALOR( int h, int nro_orden, char *cadena, int origen );
int PAGOS_CARGAR_DATOS_MEDIO( struct _dmedios **datos_medio, int datos_adicionales, int origen );
//int         PAGOS_CONSTRUYE_MASCARA( struct _dmedios *datos_medio, char *mascara_in, int tam_mascara, char *mascara, int cant_datos, double importe, struct _datos *datos ) ;
//int         PAGOS_OBTENER_DATOS( int h, int posicion, struct _datos *datos ) ;
int _PAGOS_OBTENER_DATOS( int h, int posicion, struct _datos *datos, int origen );
double PAGOS_NETO_PROPORCIONAL( int h, double importe );
double PAGOS_IVA_PROPORCIONAL( int h, double importe );
double PAGOS_EXENTO_PROPORCIONAL( double importe );
void PAGOS_REPROCESAR( char tipo_pago, int reproceso );
int PAGOS_CARGAR_MEDIOS_IMPORTACION();
int DONACION_INGRESA_MULTIPLO();
void MOSTRAR_PORC_INTERES();
void ELIMINA_BASES_DTOS_POR_TECLA();
int LEER_CHEQUE_IMPRESORA();
int DAR_VUELTA_CHEQUE_IBM4610();
void INSERTAR_EN_DPAGO( struct _ev_dmedios dp_boletaplata );
int PAGOS_CANTIDAD_DATOS_ADICIONALES( int h );
int PAGOS_OBTENER_VALOR_POS( int h, int nro_orden, int ind_posicion, char *cadena, int origen );
int PAGOS_CANTIDAD_DATOS( int h );
int CALCULAR_FECHA_VTO_CUO_MED( int cuota );
int SET_DATO_ADICIONAL( int h, int posicion, int origen, int id_dato, char *valor );
int PAGOS_F_DATOS_ADICIONALES( int pos, int ind_pos, int nro, int pos_actual, double importe,
                               struct _dmedios *datos_medio, char *buffer );
void BIN_DATOS_ADICIONALES( int origen, int h, char *archivo );
void BORRAR_BIN_DATOS_ADICIONALES( int origen );
int AGREGA_DATOS_MEDIO( int pos, char indice_pos, int nro_orden, int id_dato, struct _datos *datos,
                        int cod_solicitud, int origen, int primer_dato, double importe );
int CARGAR_CHEQUE_MANUAL( int al_dia, long *numero, int *banco, unsigned * fecha_vto,
                          int *cod_post_cheq, int *cod_hora_acre, char *cuit, char *telefono,
                          char *cuenta_banco, int *plaza, char *nrobanco, int habil, char *serie,
                          int *cod_valida_cheque, int pedir_fecha_vto );

void CARGAR_EV_DMEDIOS( int cod_sol, int nro_orden, long id_evento, int origen, int area_ac,
                        struct _ev_dmedios *ev_dpago );
//************************
int PAGOS( int modo, int ( funcion_val ) ( double importe ), int dev_cobro );
int PAGOS_PIDE_PAGO( int modo, int ( funcion_val ) ( double importe ), int dev_cobro );
int PAGOS_INGRESA_PAGO( int nro, int modo, int dev_cobro  );
void PAGOS_AGREGAR_PAGO( int pos, char modo, double importe, long autorizacion, unsigned cuenta,
                         char tarjeta, long numero, char *numero_tar, unsigned banco,
                         unsigned fecha_vto, unsigned cantidad_cupones, double valor_cupones,
                         int c_postal_c, int c_hora_c, unsigned comprobante, char *cuit,
                         char *telefono, char *cuenta_banco, int id_registro, char cod_medio_vuelto,
                         char cod_submedio_vuelto, char tipo_vuelto, double importe_me,
                         int cod_valida_cheque, int datos_adicionales, char *telefono_comprador,
						 int cod_restriccion_art );
void ACTUALIZA_REDONDEO_DE_VUELTO( int modo );
void PAGOS_ANULAR_PAGO();
void PAGOS_INTERESES();
void PAGOS_PEDIR_INTERESES();
void PAGOS_DISCRIMINA_IVA();
void PAGOS_MARCA_AUTORIZACION();
double PAGOS_PIDE_IMPORTE( char *titulo, int enteros, int decimales, int signo );
void PAGOS_MUESTRA_IMPORTE( double importe );
long PAGOS_PIDE_AUTORIZACION( char * );
int PAGOS_IMPRIME_PAGO( int modo );
void PAGOS_IMPRIME_PAGOS_ESPECIALES(int devolucion, int preticket );
int PAGOS_GRABA_PAGO( int modo, int dev_cobro);
double CALCULA_VARIACION( double importe, int modo, int submedio);
double CALCULA_VARIACION_TARJETA( double importe, int modo, int modificar );
void PAGOS_AGREGAR_VARIACIONES();
void PAGOS_PREPARA_ITEM( int h, int anulacion );
void PAGOS_AGREGA_ITEM_INTERESES();
void PAGOS_ELIMINA_ITEM_INTERESES();
void _PAGOS_PREPARA_ITEM( double importe, double porc, int anulacion, int gravado, int origen,
                          int tipo );
int VALIDAR_PAGO( int modo );
int MEDIO_DE_PAGO_ESPECIAL( int h );
int MEDIO_DE_PAGO_HABILITADO( int modo, int n, int dev_cobro );
int MEDIO_DE_PAGO_HABILITADO_MARGINAL( int n, int dev_cobro );
//double PAGOS_TOTAL_A_PAGAR();
//  double    PAGOS_TOTAL(  );
//int VERIFICA_MARCA_EN_MEDIO( char cod_medio, char marca );
//int DEBE_ABRIR_CAJON();
//void PAGOS_MUESTRA_IMPORTE_TEMPORAL( double importe );
//void PAGOS_ENVIAR_OFFLINE();
//void PAGOS_VALIDA_CHEQUE_CLIENTE();
//void PAGOS_VALIDA_200();
//void LIMPIAR_VARIAS_TARJETAS();
//double VARIACION_CUOTAS_MEDIO( int indice, double importe );
//int BUSCA_CUO_MED( char medio, char submedio );
//int _elegir_cuotas_medio( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
//int _consulta_instituciones( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );    
//int CANT_CUOTAS_MEDIO( int indice, unsigned * d_adicionales, int *cant_veces, int *nro_cuota );
//int ELEGIR_CUOTAS_MEDIO( int medio, unsigned * d_adicionales, int *cant_veces, int *nro_cuota );
//int ELEGIR_INSTITUCION();
//int PROCESAR_DONACIONES();
//int DONACION_INGRESA_IMPORTE( double imp_sugerido );
//int GRABAR_MONEDERO_ELECTRONICO();
//void ROLL_BACK_TARJETAS();
//int VERIFICAR_IMPRESORA_FISCAL();
//void PAGOS_MOSTRAR_INTERESES();
//int BUSCA_MEDIO_EN_ARRAY( int k );
//int PAGOS_IMPRIMIR_TICKET_COMPLETO( int modo );
//void BORRAR_DATOS_ADICIONALES( int pos, int origen );
//int ELEGIR_OPCION_GRILLA( int cant, char *titulo );

int PAGOS_PEDIR_DATOS_ADICIONALES( int pos, char indice_pos, int datos_adicionales, double *importe,
                                   int origen, char *titulo, int nro, long *comprobante,
                                   int ( f_datos )( int, int, int, int, double, struct _dmedios *,
                                                    char * ) , char cod_medio, char submedio );
int _PAGOS_PEDIR_DATOS_ADICIONALES( int pos, char indice_pos, double *importe,
                                    struct _dmedios *datos_medio, int cant_datos, int origen,
                                    char *titulo, int nro, long *comprobante,
                                    int ( f_datos )( int, int, int, int, double, struct _dmedios *,
                                                     char * ) , char cod_medio, char submedio );
int AGREGA_BIN_EV_DMEDIO_A_BTRV( int h, int origen );
int GUARDAR_POSICION_SUBMEDIO( int medio, double submedio, double importe, double importe_dec );
double MUTUALES_ML_A_ME_COT( double importe_me, double cotizacion );
double MUTUALES_ME_A_ML_COT( double importe_me, double cotizacion );
int MUTUAL_MONEDA_EXTRANJERA( int h );
double MUTUALES_ML_A_ME( double importe_me, int h );
double MUTUALES_ME_A_ML( double importe_me, int h );
int MEDIO_ACTIVADO_PROMO(unsigned short sub_medio, char plan, unsigned short cod_medio );
//int GET_MEDIO_ACTIVADO_PROMO( struct activacion_medios *act_medio, unsigned short sub_medio,
//                              char plan, int SumarMonto, double *SumaMonto );
double MONTO_MEDIO_ACTIVADO_PROMO(unsigned short sub_medio, char plan, unsigned short cod_medio );
int ACT_MONTO_MEDIO_ACTIVADO_PROMO( unsigned short pSub_medio, char pPlan, double pMonto_usado, unsigned short cod_medio );
void ACT_MONTO_PROMO(long pCod_promo, double pMonto_promo, int pNuevo, unsigned short medio, unsigned short submedio);
// void ELIMINA_PAGO_PROMO();
#if !defined (INVEL_L)
void ELIMINA_PAGO_PROMO( unsigned short medio, unsigned short submedio );
#else
void ELIMINA_PAGO_PROMO( unsigned int medio, unsigned int submedio );
#endif
int HAY_EFECTIVO( );

//**********************************************************
//char     *PAGOS_OBTENER_NOMBRE( char medio, char submedio );
	//int		  PAGOS( int modo, int ( funcion_val ) ( double importe ), int dev_cobro );
	//int       PAGOS_INGRESA_PAGO( int nro, int modo, int dev_cobro );
	//void      PAGOS_AGREGAR_PAGO( int pos, char modo, double importe, long autorizacion,
	//	unsigned cuenta, char tarjeta, long numero, char *numero_tar, unsigned banco,
	//	unsigned fecha_vto, unsigned cantidad_cupones, double valor_cupones, int c_postal_c,
	//	int c_hora_c, unsigned comprobante, char *cuit, char *telefono, char *cuenta_banco,
	//	int id_registro, char cod_medio_vuelto, char cod_submedio_vuelto, char tipo_vuelto,
    //  double importe_me, int cod_valida_cheque, int datos_adicionales, char *telefono_comprador );
	//void      ACTUALIZA_REDONDEO_DE_VUELTO( int modo );
	//void      PAGOS_ANULAR_PAGO(  );
	//void      PAGOS_INTERESES(  );
	//void      PAGOS_PEDIR_INTERESES(  );
	//void      PAGOS_DISCRIMINA_IVA(  );
	//void      PAGOS_MARCA_AUTORIZACION(  );
	//double    PAGOS_PIDE_IMPORTE( char *titulo, int enteros, int decimales, int signo );
	//void      PAGOS_MUESTRA_IMPORTE( double importe );
	//long      PAGOS_PIDE_AUTORIZACION( char * );
	//int       PAGOS_IMPRIME_PAGO( int modo );
	//void      PAGOS_IMPRIME_PAGOS_ESPECIALES(  );
	//int       PAGOS_GRABA_PAGO( int modo, int dev_cobro );
	//double    CALCULA_VARIACION( double importe, int modo );
	//double    CALCULA_VARIACION_TARJETA( double importe, int modo );
	//double    _VARIACION_TARJETA( double importe, float porc, double *ret_var,
	//	double *iva2, long art_variacion );
	//void      PAGOS_AGREGAR_VARIACIONES(  );
	//void      PAGOS_PREPARA_ITEM( int h, int anulacion );
	//void      PAGOS_AGREGA_ITEM_INTERESES(  );
	//void      PAGOS_ELIMINA_ITEM_INTERESES(  );
	//void      _PAGOS_PREPARA_ITEM( double importe, double porc, int anulacion, int gravado,
	//	int origen, int tipo );
	//int       VALIDAR_PAGO( int modo );
	//int       MEDIO_DE_PAGO_ESPECIAL( int h );
	//int       MEDIO_DE_PAGO_HABILITADO( int modo, int n, int dev_cobro );
	//double    PAGOS_TOTAL_A_PAGAR(  );
//	double    PAGOS_TOTAL(  );
	int       VERIFICA_MARCA_EN_MEDIO( char cod_medio, char marca );
	int       DEBE_ABRIR_CAJON(  );
void PAGOS_MUESTRA_IMPORTE_TEMPORAL( double importe, int mostrar_importe_temporal );
	void      PAGOS_ENVIAR_OFFLINE(  );
	void      PAGOS_VALIDA_CHEQUE_CLIENTE(  );
	void      PAGOS_VALIDA_200(  );
	void      LIMPIAR_VARIAS_TARJETAS(  );
	double    VARIACION_CUOTAS_MEDIO( int indice, double importe );
	int       BUSCA_CUO_MED( char medio, char submedio );
	int _elegir_cuotas_medio( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
	int _consulta_instituciones( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );	
   int       CANT_CUOTAS_MEDIO( int indice, unsigned* d_adicionales, int* cant_veces, int* nro_cuota );
   int       ELEGIR_CUOTAS_MEDIO( int medio, unsigned* d_adicionales, int* cant_veces, int* nro_cuota );
	int       ELEGIR_INSTITUCION(  );
	int       PROCESAR_DONACIONES(  );
	int       DONACION_INGRESA_IMPORTE( double imp_sugerido );
	int       GRABAR_MONEDERO_ELECTRONICO(  );
	void      ROLL_BACK_TARJETAS(  );
	int       VERIFICAR_IMPRESORA_FISCAL(  );
	void      PAGOS_MOSTRAR_INTERESES(  );
	int       BUSCA_MEDIO_EN_ARRAY( int k );
	int 		 PAGOS_IMPRIMIR_TICKET_COMPLETO( int modo );
   void      BORRAR_DATOS_ADICIONALES( int pos, int origen );
   int       ELEGIR_OPCION_GRILLA( int cant, char *titulo );
   int MEDIOS_HABILITADOS_COBROS( int n, int dev_cobro, struct    _datos_cobro *datosCobros );
   int		 PAGOS_MEDIO_HABILITADO_COBRO( int cod_medio, int cod_submedio, int dev_cobro );
   int      TOMAR_DATO_ADICIONAL( char *valor, char *descripcion, int orden, int tipo_dato, char cod_medio, char submedio );
   int      TIPO_DATO_ADICIONAL( int solicitud, int orden );
   void     GRABAR_TIPO_DATO_DEL_DATO_ADICIONAL( int tipo_dato, struct _ev_dmedios *dpago, char *valor );
   void		 GRABAR_TARJETAS_REC( int h, int posicion );//Seba
   void		 LEER_TARJETAS_REC(); //Seba
   int      CONFIRMAR_DATO_ADICIONAL(  struct _dmedios *datos_medio, char *valor, int posicion );
   int RECUPERAR_POSICION_TARJETA_FISCAL( int h );
   void CANCELAR_MEDIOS_PAGO( int modo );
   void ELIMINA_PAGO_PROMO_POSPAGO( unsigned short medio, unsigned short submedio, int tecla );
#endif
