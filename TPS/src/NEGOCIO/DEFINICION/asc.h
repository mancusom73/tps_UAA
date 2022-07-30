#ifndef ASC_H
#define ASC_H
int GRABAR_ARCHIVO( int cod, int registro, char *buffer, int l_reg );
int ANULAR_REGISTRO( int cod, int registro, char *buffer, int l_reg, char *buf_cer, int l_buf_cer );
void GENERA_MUTUAL_ASC( int h );
void GENERA_TICKET_ASC( int h );
void GENERA_TOTALES_ASC();
void GENERA_DEPARTA_ASC();
void GENERA_CHEQUES_ASC( int h );
void GENERA_TARJETAS_ASC( int h );
void GENERA_COBROS_ASC( int id, struct _datos_cobro *datos_cobro );
void GENERA_PAGOS_ASC( int id, long comprobante, long cuenta, double importe, double gravado,
                       double exento, double iva );
void ANULA_CHEQUES_ASC( int reg );
void ANULA_TARJETAS_ASC( int reg );
#endif
