#ifndef CLIENTES_H
#define CLIENTES_H
int GRABA_SALDO_CHEQUE_CLIENTE( double tot_cheques );
int PEDIR_NRO_CLIENTE( int recuadro, int permitir_escape );
void MOSTRAR_CLIENTE( int x, int y, long nro );
int EXISTE_CLIENTE( long cliente );
int BUSCAR_CLIENTE_POR_CUIT( char *cuit );
void ACTUALIZA_SALDO_CLIENTE( double importe );
int VERIFICA_CUPO_CLIENTE( double importe, double importe_ant, char cobro, char modo );
int VERIFICA_SALDO_CHEQUE( double importe );
int VALIDA_CLIENTE();
int ACTUALIZA_CLIENTES( void );
void VERIFICAR_CLIENTE_HABILITADO_PARA_CHEQUES();
int VERIFICA_EMPADRONAMIENTO_CLIENTES( int recuadro );
void IDENTIFICAR_CLIENTE_POR_CONSULTA();
int PEDIR_DATOS_CLIENTE( void );
int ACEPTAR_CLIENTE();
void CEREAR_PERFIL_CLIENTE( void );
int ALTA_CLIENTE( void );
void CARGAR_PERFIL_CLIENTE( int cod_perfil );
int SALIR_ALTA_CLIENTE( void );
int ASIGNA_CONDICION_IVA( char *cadena );
int       CARGAR_CONF_CLIENTES();
void      CLIENTE_ejecutar_comando( int *ind_config );
void      INIT_CLIENTE_CONFIGURABLE();
int       _COMIENZO_CONFIG_CLI( int comienzo, int final );
void      LIBERA_UTI_CLI();
int       obtenerTipo( char *nombre, char *abrev, int *numerico );
INT16     _tomar_entero_config( int *ind_config, int *indice, struct _ConfTkt *CONFIG_ARCH );
int       INGRESAR_DATOS_CLIENTES_CONFIG( int *posicion );
int       VALIDAR_CONF_CLIENTE( );
int       OBTENER_COND_IVA_CLIENTE( long cod_cliente );
int       VALIDAR_DATOS_CLIENTE_FISCAL( void );
#endif
