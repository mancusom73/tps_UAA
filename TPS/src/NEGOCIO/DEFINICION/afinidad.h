#ifndef AFINIDAD_H
#define AFINIDAD_H
void PEDIR_CLIENTE_AFINIDAD( int documento );
void TOMAR_CLIENTE_AFINIDAD( int recupero);
void LEER_PUNTOS();
int OBTENER_FECHA_PUNTOS( char *buffer );
int SELECCIONAR_AFINIDAD_CHIP();
int AFINIDAD_DESCUENTO_PUNTOS( double puntos, double descuento );
void TOMAR_CLIENTE_AFINIDAD_NORTE();
int OBTENER_DATOS_CLIENTE_AFINIDAD( int documento );
int VERIFICAR_CLIENTE_HABILITADO( char adicional );
#endif
