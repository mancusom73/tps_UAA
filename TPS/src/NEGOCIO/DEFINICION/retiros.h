#ifndef RETIROS_H
#define RETIROS_H
void 	CR4_CONFECCIONA_PANTALLA();
void 	CR4_PIDE_RETIRO();
void 	CR4_INGRESA_RETIRO( int nro, int posicion );
void 	CR4_AGREGAR_RETIRO(int nro, int posicion);
void 	CR4_RETIROS();
int 	ELEGIR_MOTIVO_RETIRO( int *cod_solicitud );
int     SUMAR_IMPORTE_SUBMEDIO( int medio, int submedio, double importe );
void	CR4_ACUMULAR_RETIRO_EN_X( int modo, double importe, int anulacion, double importe_me, int motivo, int cantidad );
void      CR4_ACTIVA_ANULACION(  );
double    CR4_PIDE_IMPORTE(  );
void      CR4_IMPRIME_RETIRO(  );
double    CR4_BUSCA_COMPROBANTE( int modo, long id, int todos, int *id_registro,
	char *sub_medio, short *banco, short tipo_busqueda, double importe, char *id_cadena,
    short anulacion, short marca_usado );
void      GRABA_EVENTO_RETIRO( int modo );
void      _GRABA_EVENTO_RETIRO( int medio, double importe, int id_registro,
	char cod_sub_medio, double importe_me, int motivo_cheque, int posicion, int diferencia );
void      CR4_SET_ACU_RETIRADO( int medio, double importe );
int       RETIRO_REMOTO( char *param );
void      CR4_RESET_ACU_RETIRADO(  );
int       OBTIENE_CANT_SUB_RETIRO( );
void      CR4_MOSTRAR_RETIRO(  );
long      CR4_PIDE_NRO_CHEQUE(  );
long      CR4_PIDE_NRO_CUPON(  );
long      CR4_PIDE_CUENTA(  );
double    CR4_BUSCA_CHEQUE(  );
double    CR4_BUSCA_TARJETA(  );
double    CR4_BUSCA_CUENTA(  );
int		  _elegir_tarjeta( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
int       PIDE_TARJETA( int *id_registro, int solicitar_submedio );
void      GRABA_EVENTO_CABECERA_RETIRO( double importe_retiro );
int       CONTROL_IMPORTES_ESP( int indice, int actualiza, long *codnivel1, long *codigo, int nro, double *importe, long nro_cheque, int banco, int motivo, short anulacion, char *nro_cheque_str );
int       CONTROL_CONSISTENCA_RNV_REC();
void      ACUMULA_RETIRADO( int medio, int submedio, double importe );
double    OBTIENE_RETIRADO( int medio, int submedio );
int       CARGAR_MEDIOS_RETIRO( int medio, int submedio, double importe, double importe_me, int expandir );
int       POSICIONAR_EN_REGISTRO_RETIRO( int medio, int submedio );
double	  ELEGIR_DENOMINACION( int grilla, int cant, char *simbolo);
double    MOSTRAR_DENOMINACIONES( int cod_medio, int cod_submedio);
int		  PIDE_CANTIDAD_DENOMINACION(int k);
#endif
