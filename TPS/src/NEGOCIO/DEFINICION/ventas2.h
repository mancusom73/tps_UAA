#ifndef VENTAS2_H
#define VENTAS2_H
int EVENTO_A_PROCESAR( struct _eventos *ticket );
void ACTUALIZA_VENTAS2( int reproceso );
int ACTUALLIZA_EVENTO_ANTERIOR();
int EVENTOIZA_EVENTO_ANTERIOR( int cant );
int _ACTUA_A_PROCESAR();
int _ACTUALIZA_VENTAS2( long id_evento, int reprocesar );
void ABRIR_VTA_CLASIF();
int ACTUALIZA_VENTAS_POR_CLASIFICACION( long id_evento );
int _ACTUALIZA_VENTAS_POR_CLASIFICACION();
int ACUMULAR_VENTAS_POR_CLASIFICACION( float cantidad, double importe, long cod_clasificacion );


void CALCULA_FACTOR_DESPLAZAMIENTO( void );
int ACTUALIZA_VENTAS_POR_MEDIOS( long id_evento );
int _ACTUALIZA_VENTAS_POR_MEDIOS();
int ACUMULAR_VENTAS_POR_MEDIOS( float cantidad, double importe, char cod_medio, int cod_submedio );
void ACTUALIZA_VENTAS_XYZ( char modo );
void ACUMULAR_VENTAS_XYZ( char modo );
void ACTUALIZA_VENTAS2_ANTERIORES( int actualizar_si_o_si );
int ACTUALIZA_COBROS_TARJETAS( long id_evento );
int _ACTUALIZA_COBROS_TARJETAS();
int ACUMULAR_COBROS_TARJETAS( float cantidad, double importe, char cod_medio, int cod_submedio );
int _GRABA_TICKET_REMOTO();
int BORRAR_PAGO_ALL();
int _BORRAR_PAGO_ALL( long id_evento );
int _BORRAR_E_TICKET_ALL( long id_evento );
int BORRA_ALL();
int BORRA_TICKET_REMOTO();
int BUSCA_TICKET_ALL();
int CANCELAR_ACTUALIZACION( int actualizar_si_o_si, int cancelacion );
int ABRE_VENTAS2();
int CIERRA_VENTAS2();
int ACTUALIZA_STOCK_TRANSACCIONAL( float cant );
int ACTUALIZA_TICKET( long id_evento );
int EXISTE_TRANSACCION();
int VERIFICA_TRANSACCION();
int UPDATE_CTRLTRAN();
int INSERTA_CTRLTRAN();
int TIPO_EVENTO_A_PROCESAR( char tipo_evento );
#endif
