#ifndef IMP_ITEM_H
#define IMP_ITEM_H
int IMPRIME_ITEM( int indice );
void CAMBIA_COMA_POR_PUNTO( char *m, int len );
void ITEM_FORMATO_AMERICANOS( double precio );
void ITEM_FORMATO_LIBERTAD( double precio );
void ITEM_FORMATO_OMROM_AMERICANOS( double precio );
int ITEM_FORMATO_FISCAL( double precio, int indice );
void ITEM_FORMATO_OMROM_LAR( double precio );
void ITEM_FORMATO_SPAR( double precio );
void ITEM_FORMATO_OMRON_FERNI( double precio );
void ITEM_FORMATO_TP( double precio );
void ITEM_FORMATO_VALLEJO( double precio );
void MOSTRAR_CANTIDAD( int x, int y, double cantidad );
void ITEM_FORMATO_FALABELLA( double precio );
void ITEM_FORMATO_BIENESTAR( double precio );
void ITEM_FORMATO_TOLEDO( double precio );
void ITEM_FORMATO_JOURNAL( double precio );
void ITEM_FORMATO_CARREFOUR( double precio );
void ITEM_FORMATO_CARREFOUR_IBM_4694( double precio );
void ITEM_FORMATO_CONFIGURABLE();
void MOSTRAR_CANTIDAD2( int x, int y, double cantidad );
void IMPRIMIR_LINEA_EN_TEXTO_FISCAL();
#endif
