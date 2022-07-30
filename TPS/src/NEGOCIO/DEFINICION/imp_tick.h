#ifndef IMP_TICK_H
#define IMP_TICK_H
void LEER_DATOS_TICKET();
int IMPRIME_CABECERA_TICKET( int doc_no_fiscal );
void _IMPRIME_CABECERA_TICKET( int lineas_corte, int lineas, int corte_activado );
void IMPRIME_TITULO_TICKET();
void IMPRIME_INTERESES( double importe, double porc );
void IMPRIME_SUBTOTAL( double importe, int incondicional );
int IMPRIME_TOTAL( double importe, int bultos, int modo );
void IMPRIME_IVA();
void IMPRIME_MEDIO_DE_PAGO( char modo, double importe );
void IMPRIME_VUELTO( double importe );
void _IMPRIME_VUELTO( char *nombre, double importe );
void IMPRIME_TICKET_ANULADO(int suspendido);
void IMPRIME_PIE();
int IMPRIME_BONOS();
void IMPRIME_NRO_TICKET();
void IMPRIME_REPROCESAMIENTO();
void IMPRIME_CABECERA_GENERAL( long nro_evento );
int DEBE_IMPRIMIR_ITEM();
int IMPRIMIR_TICKET_COMPLETO(int cabecera);
void IMPRIME_CAMBIO();
void IMPRIMIR_CAJERO_EN_SLIP( int ingreso );
void IMPRIME_CAJERO();
int IMPRIME_EMPLEADO();
void IMPRIME_TOTAL_TICKET_ENVIO_DOMICILIO();
void IMPRIMIR_COPIAS_TICKET_VOUCHERS();
int  IMPRIME_FIN_TICKET( void );
void	  IMPRIME_CABECERA_TIQUE_FACTURA();//Seba
void	  IMPRIME_CABECERA_NOTA_DE_CREDITO();//Seba
#endif
