#ifndef _CR0_H
#define _CR0_H
void CR0_EXCEPCION_PEDIR_CAJERO( int *tecla );
long CR0_PEDIR_CAJERO( int x, int y, int l, int *tipo_ingreso );
int CR0_INGRESA_CAJERO();
int ES_SUPERVISOR( int cajero );
int _ES_SUPERVISOR( long cajero, struct _cajeros *datos_cajero );
void CR0_EXIT( int status );
int CR0_MODO_Z( int con_menu );
int Z_DUPLICADA();
void CR0_GRABAR_ARCHIVOS_SPAR();
int CR0_c_b( void );
int CR0_CIERRE(int recupero);
int CR0_GRABAR_ENTRADA_CAJERO();
int CR0_VERIFICAR_CAJERO_HABILITADO( int cajero );
int CR0_HAY_USUARIOS_HABILITADOS();
int CR0_PEMITE_CERRAR( int con_menu );
int CR0_GRABAR_X( int cajon );
int CR0_RECUPERAR_X( int cajon );
int CR0_MOSTRAR_CAJONES_HABILITADOS( int elegir_cajero );
int NUMERO_CAJA();
void LIBERA_BUFFERS_TEMPORALES();
void GRABA_EVENTO_SALIDA_CAJERO( void );
void GRABA_EVENTO_INGRESO_CAJERO( void );
int GRABAR_PRUEBA( int modalidad_espera_respuesta );
void CR0_TOMAR_DIA_OPERATIVO( int onfly);
unsigned PIDE_NUEVO_DIA_OPERATIVO( void );
void CR0_FINAL( char *version );
void INICIALIZAR_PINPAD();
//void      CR0_GENERAR_Z( int bandera );
void CR0_GENERAR_Z();
void CR0_INIT_ARCHIVOS();
void CR0_LIMPIAR_RAMPAC();
void SEGURO();
void MOSTRAR_DIA_OPERATIVO();
void MOSTRAR_MEMORIA_DISPONIBLE();
void TESTEAR_CIERRE_FALLIDO();
void ALOJA_BUFFERS_TEMPORALES();
void ALOJA_STRING_TABLE();
void SETEAR_TIEMPO( int estado );
void ACUMULAR_TIEMPOS( void );
void ALOJA_RESPUESTAS_FISCAL();
void SUMAR_VAR_UINT( int var, long valor );
void CEREAR_TIEMPOS( void );
void CONTROL_CANT_MEDIOS_SUBMEDIOS_FONDO_F_AUTO( int cajero, int *fondo_fijo_aut );
void CEREAR_TIEMPOS( void );
int       MENU_CAJERO_SUPERVISOR( void );
void       CONTROL_CANT_MEDIOS_SUBMEDIOS_FONDO_F_AUTO( int cajero, int *fondo_fijo_aut );
int       ES_GERENTE( int cajero );
char GRABAR_LOG_ACTIONS( void );
int VERICAR_CONF_SERVICIOS( void );
void CONTROLAR_TABLAS_OLD();
int EVENTO_PROCESADO( long id_evento );
void ACTUALIZA_FECHA_Y_HORA_CONTROLADOR_FISCAL_EPSON( void );
void PIDE_FECHA_Y_HORA_AL_CONTROLADOR_FISCAL_EPSON( void );

#endif
