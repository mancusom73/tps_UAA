#ifndef CR_PANT_H
#define CR_PANT_H
long PEDIR_ENTERO_XY( char *titulo, int x, int y, int digitos );
void MUESTRA_ET_TOMRA();
//  void      LIMPIAR_RECUADRO_PETICION( char *titulo );
void LIMPIAR_RECUADRO_PETICION( int mensaje );
void MOSTRAR_MODO_DEVOLUCION();
void DIBUJAR_RECUADRO_PETICION( char *titulo, int digitos );
void BORRA_RECUADRO_PETICION( int x1, int x2, int xc1, int xc2 );
long PEDIR_ENTERO_EN_RECUADRO( char *titulo, int salvar_pantalla, int digitos );
double PEDIR_FLOAT_EN_RECUADRO_CS( char *titulo, int salvar_pantalla, int enteros, int decimales,
                                   int tipo );
void CR1_CONFECCIONA_PANTALLA();
void DIBUJA_RECUADRO_DEL_TICKET();
void LIMPIA_RECUADRO_DEL_TICKET();
void CR1_DISPLAY_DE_PANTALLA();
void BORRA_DISPLAY_DE_PANTALLA();
void CR1_MUESTRA_MULTIPLICACION();
void CR1_MUESTRA_ARTICULO_EN_GRANDE();
void CR1_MUESTRA_ARTICULO_EN_GRANDE();
void __CR1_MUESTRA_ARTICULO_EN_GRANDE( int item );
void MUESTRA_BULTOS();
void MUESTRA_SUBTOTAL( int recalcular);
void MUESTRA_CARTEL_INICIAL();
void MUESTRA_ITEM_EN_PANTALLA( int nro );
void CR1_MUESTRA_TARJETA();
void MUESTRA_MUTUAL();
void MUESTRA_CLIENTE( int cheque );
void MOSTRAR_CLIENTE_EN_PANTALLA();
void MOSTRAR_PETICION_DE_PRESENTACIONES( int presentaciones );
void MOSTRAR_PETICION_DE_CODIGO( int pedir_cod_especial );
void MUESTRA_TIPO_PRECIO();
void MOSTRAR_MODO_CAMBIO();
void MOSTRAR_MOZO();
void MOSTRAR_VENDEDOR();
void MOSTRAR_CLIENTE_AFINIDAD();
void MUESTRA_COMPROBANTE();
void REGENERAR_PANTALLA_TICKET();
void MUESTRA_HORA_EN_PANTALLA();
void MUESTRA_TIPO_DEVOLUCION();
void MUESTRA_ERROR_EN_RED( void );
int PEDIR_FECHA_EN_RECUADRO( char *titulo, int salvar_pantalla, char *fecha );

int PEDIR_CADENA_EN_RECUADRO( char *titulo, int salvar_pantalla, int digitos, char *cadena,
                              char *mascara_scanner );
double PEDIR_FLOAT_EN_RECUADRO_CC( char *titulo, int salvar_pantalla, int enteros, int decimales,
                                   int cajero, int sup, int cond, int cajon, int controlar_efectivo,
                                   int funcion );
double PEDIR_FLOAT_EN_RECUADRO( char *titulo, int salvar_pantalla, int enteros, int decimales );
int PEDIR_CADENA_XY( char *titulo, int x, int y, int digitos, char *cadena );
void SET_BORRAR_RECUADRO_DE_PETICION( int borrar );
void GUI_MUESTRA_TARJETA( int operacion );
//  void         MUESTRA_REFERENCIA_NC(  );
double PEDIR_FLOAT_EN_RECUADRO_CON_VALOR( char *titulo, int salvar_pantalla, int enteros,
                                          int decimales, double valor );
double PEDIR_IMPORTE_XY( char *titulo, int x, int y, int enteros, int decimales );
void VERSION_DIALOG( void );
void MOSTRAR_ANULACION( void );
void MUESTRA_DATOS_CONTADOR_PANTALLA( int nro, int MostrarDatos );

#endif
