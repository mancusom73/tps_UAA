#ifndef CAMBIO_H
#define CAMBIO_H
void TECLA_CAMBIO();
int PEDIR_BOLETA_ANTERIOR();
int TIPOS_CAMBIO();
int IMPORTAR_ITEMS();
int EXISTE_ARTICULO_TICKET_ORIGINAL();
int INGRESAR_CAJA_Y_SECUENCIA( int *caja, long *secuencia, int *fecha );
int ASIGNAR_PRECIO_TICKET_ORIGINAL( struct _ev_cont ctrl_nc );
double CANTIDAD_TOTAL_ITEM();
int TIPO_EVENTO( INT16 codigo, INT16 tipo_comprobante );
#endif
