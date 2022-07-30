#ifndef CONSULTA_H
#define CONSULTA_H
void MOSTRAR_DATOS_ARTICULO_CON_PROMOCIONES();
//  void      CR1_MOSTRAR_DATOS( int y, int vacio, int que_consulta );
int EVALUA_TECLA( int tecla );
void MUESTRA_PROMOCIONES_ARTICULO( int dato );
void MOSTRAR_DATOS_PESO( int fila, float peso, int precio_en_me );
void _MOSTRAR_DATOS_ARTICULO( int refrescar, int muestra_promociones, int precio_en_me,
                              double peso );
void CR1_MOSTRAR_ITEM_WIN( int item );
void CR1_MOSTRAR_ITEM( int item, int y, int modificar );
//void      CR1_MOSTRAR_DATOS( int y, int vacio, int que_consulta );
void CR1_MOSTRAR_DATOS( int y, int vacio, int que_consulta, struct _browse *b );
int _consulta_cod_barra( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
int _consulta_descripcion( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
int _consulta_cod_interno( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
int _consulta_marca( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
int _consulta_cod_cobro( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
int CR1_CONSULTA( int indice );
float BUSCAR_STOCK( long cod_interno );
void REPORTE_STOCK();
int PIDE_CODIGO_ARTICULO( char *cadena, int consultado );
void MOSTRAR_DATOS_PRECIO_ME( int me );
void BORRAR_DATOS_PESO( int fila );
#endif
