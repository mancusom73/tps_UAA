#ifndef CAMBIO_M_H
#define CAMBIO_M_H
double CAMBIO_PIDE_IMPORTE( int datos_adicionales, int pos );
void CAMBIO_MOSTRAR_MEDIO( int modo, double importe, long numero, int anulacion, double importe_me );
void _GRABA_EVENTO_CAMBIO( int medio, double importe, int id_registro, char cod_sub_medio, int tipo,
                           int h );
int VALIDA_ANULACION_MEDIO( int medio, int submedio, double importe, int tipo );
void CAMBIO_AGREGAR_MEDIO( int modalidad, int tipo, long numero, int banco, int fecha_vto, int c_postal_c,
                           int c_hora_c, char *cuit, char *telefono, char *cuenta_banco,
                           int tarjeta, char *numero_tar, long cod_sub_medio );
void CAMBIO_GUARDAR_SUBMEDIOS_REC( int medio, int submedio, double importe, int anulacion, int tipo );
void CAMBIO_MEDIOS( int cambio_medio_automatico );
void CAMBIO_CONFECCIONA_PANTALLA( int tipo );
void CAMBIO_INGRESA_MEDIO( int nro, int tipo );
void CAMBIO_ACUMULAR_CAMBIO_EN_X( int modo, double importe, int anulacion, int tipo );
void CAMBIO_ACTIVA_ANULACION();
void CAMBIO_IMPRIME_MEDIO( int tipo );
void CAMBIO_SET_ACU_RETIRADO();
void GRABA_EVENTO_CABECERA_CAMBIO();
void CAMBIO_MUESTRA_TOTAL( int tipo, int nro );
int CAMBIO_PIDE_MEDIOS( int tipo );
int EN_CAMBIO_DE_MEDIOS();
int PERMITIR_MEDIO( int medio, int tipo );
int VALIDAR_MEDIOS_CAMBIO_M( char cod_medio_ing, int cod_submedio_ing );
int PERMITE_SUBMEDIO( char medio, int sub_medio, char tipo );
int SOLICITAR_SUPERVISOR_CAMBIO_M( char medio, int sub_medio, char tipo );
int CONSULTA_SALDO_TARJETAS( double monto );
void CAMBIO_MOSTRAR_ANULACION();
int PROCESAR_CAMBIO_MEDIOS_AUTOMATICO( void );
int CARGAR_CAMBIO_MEDIO_AUTOMATICO( void );
void INGRESAR_CAMBIO_MEDIO_AUTOMATICO( int nro, int submedio, int tipo );
long PROXIMO_TRANSACCION_CAMBIO_MEDIO_A_PROCESAR( int medio, int submedio, int *CantIdSinProcesar );
int CARGAR_DATOS_CAMBIO_MEDIO( int medio, int submedio );
int _CARGAR_CAMBIO_MEDIO_AUTOMATICO( int CantIdAProcesar, int j  );

#endif
