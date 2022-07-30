#ifndef FISCAL_H
#define FISCAL_H
void CONTROLAR_COMANDO_FISCAL();
int CAD_HEXA_2_DEC( char byte );
void RESET_FLAG_ERROR_IBM4610( int ticks_b );
void LEER_PRINTER_IBM4610_FLASH( int tipo );
int _COMANDO_LEER_CONTADORES_PRINTER_FISCAL( int asignar, long *nro_boleta );
int COMANDO_LEER_CONTADORES_PRINTER_FISCAL( int reintentos, int asignar, long *nro_boleta );
int STATUS_PRINTER_FISCAL();
void IMPRIME_BMP_LOGO( char *nomarch );
//int COMANDO_LEER_STATUS_PRINTER_FISCAL( int reintentos );
int COMANDO_LEER_STATUS_PRINTER_FISCAL(int reintentos, char *cajon_abierto);
int _COMANDO_LEER_STATUS_PRINTER_FISCAL();              
//void RESET_FLAG_ERROR_IBM4610( int ticks_b );
void CONSULTAR_DATOS_IMPRESORA_FISCAL( int *nro_caja, long *nro_z );
void FIN_DOCUMENTO( int anula, double total );
void ABRIR_DOCUMENTO( int strclave, long nro_comprobante );
void CERRAR_DOCUMENTO( double total, int corte_papel, int lf );
int FISCAL_X();
int FISCAL_Z();
int FISCAL_B();
void FISCAL_V();
void FISCAL_Y();
void FISCAL_VERIFICA_ESTADO();
int FISCAL_T( double importe, int modo );
void FISCAL_FIN_INFORME();
void FISCAL_FIN_BOLETA();
void ENVIAR_COMANDO_FISCAL( unsigned char *datos );
void TOMAR_DATOS_TOTAL_FISCAL( int comando, char *respuesta );
int FISCAL_TOTAL_COMPROBANTE( int final );
void FISCAL_ABRIR_CAJON();
int FISCAL_CAJON_CERRADO();
void FISCAL_VERIFICACION();
void FISCAL_INICIALIZAR();
int PROCESAR_ESTADO_IMPRESORA( char *estado );
int PROCESAR_ESTADO_FISCAL( char *estado );
void ABRIR_DOCUMENTO_NO_FISCAL( char slip );
int IMPRIME_ITEM_FISCAL( char *desc, double importe, double cantidad, int indice );
void IMPRIME_PERCEPCION_FISCAL( char *desc, double importe, int tipo_percepcion );
void IMPRIME_MEDIO_DE_PAGO_FISCAL( char tipo, char *desc, double importe );
void CERRAR_DOCUMENTO_NO_FISCAL();
int ABRIR_DOCUMENTO_FISCAL();
int IMPRESORA_FISCAL_EN_LINEA( char *estado );
int CERRAR_DOCUMENTO_FISCAL();
int       HABILITAR_SLIP_FISCAL( int tipo_val, int mens, struct _valida *valida );
int ENVIAR_PERCEPCION_COMO_ARTICULO( char *nombre, double importe );
void TOMAR_DATOS_RESPUESTA_FISCAL( int comando, char *respuesta, char tipo_status );
int ERROR_IMPRESORA_FISCAL( char *estado );
int CHEQUEAR_CRC_FISCAL_EPSON( unsigned char *respuesta, int cantidad );
int ESPERAR_RESPUESTA_COMANDO_FISCAL_EPSON( char *respuesta, char *datos, int control_anterior );
int COMANDO_FISCAL_EPSON_A_CONTROLAR( char comando );
int RECIBIR_RESPUESTA_IMPRESORA_FISCAL( char *respuesta, int len, char delimitador, int extra );
int FISCAL_SUBTOTAL_COMPROBANTE( int imprimir );
void ANULA_TICKET_FISCAL( char *desc );
void		 AVANZA_PAPEL(int lineas );//Seba
void	     IMPRIMIR_IMPORTE_FF(int pos, int columna);//Seba
void		 IMPRIMIR_IMPORTE_RETIRO(int medio);//Seba
void		 IMPRIME_TKT_DUPLICADO_TARJETA(int indice, int indice2);//Seba
void		 IMPRIME_TKT_RETIRO(); //Seba
void		 IMPRIMIR_TEXTO_NO_FISCAL(int station, int tipo, char *texto, int salto_fila );//Seba
void      ABRIR_NOTA_DE_CREDITO(char formtype, int IvaEmi, int IvaComp, char *name, char *lastname,int docType, char *DocNum, char *address1, char *address2, char *address3, char *remito, char *remito1);//Seba
void      ABRIR_TIQUE_FACTURA(char formtype, int IvaEmi, char IvaComp, char *name, char *lastname,int docType, char *DocNum, char *address1, char *address2, char *address3, char *remito, char *remito1);//Seba
int		 IMPRIME_ITEM_FAC(char *desc, double importe, double impuestos_internos, double cantidad );//Seba
int		 CERRAR_TIQUE_FACTURA(int IvaEmi, int IvaComp);//Seba
int		 CERRAR_NOTA_DE_CREDITO(int IvaEmi, int IvaComp);//Seba
int		 IMPRIME_MENSAJE_FISCAL( char *mensaje );
void    CEREAR_PIE_FISCAL( );
int OBTENER_FECHA_HORA_FISCAL( );
int ESTABLECER_FECHA_HORA_FISCAL( char *buffer );
int DOCUMENTO_ABIERTO( );
int GET_ESTADO_FISCAL(int pregunta);
int MEDIOS_AGRUPADOS( int sumar, int modo, int cod_submedio );

#endif
