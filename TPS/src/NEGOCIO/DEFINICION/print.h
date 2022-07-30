#pragma pack (push,1) 
#ifndef PRINT_H
#define PTINT_H
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

int INICIALIZAR_PRINTER();
void CLOSE_PRINTER();
int STATUS_IBM_4614();
void _INICIALIZAR_PRINTER();
int PRINT( char columna, char modo, char *cadena );
void CR();
void LF( int cantidad );
void SET_COLUMNA( int col );
void LF_RECEIPT( int cantidad );
void LF_JOURNAL( int cantidad );
void MICRO_LF( int cantidad );
void PRN_LETRA_NORMAL();
void PRN_LETRA_GRANDE();
void PRN_LETRA_CHICA();
void PRN_LETRA_DOBLE_PASADA();
void PRN_CONTINUA();
void PRN_DISCONTINUA();
void PRN_HABILITA_AVANCES();
void PRN_INHABILITA_AVANCE_JOURNAL();
void PRN_INHABILITA_AVANCE_RECEIPT();
void PRN_CORTE_TOTAL();
void PRN_FIN_TICKET();
void PRN_FIN_TICKET_CONTINUO( int pagina, int tot_paginas );
int VERIFICA_CORTE_FIN_TICKET( int lineas, int lineas_corte );
void ENVIAR_CODIGOS( int op );
void PRINTER_ON();
void PRINTER_OFF();
void SPOOLER_OFF();
void SPOOLER_ON();
int SEND_BYTE( unsigned char byte );
int _SEND_BYTE();

void PRN_INHABILITA_SLIP();
void PRN_ESPACIADO_NORMAL();
void PRN_ESPACIADO( int x );
void PRN_FEED( int x );
void PRN_SELECT( int x );
void PRN_SELECT_PARA_ESPACIADO( int x );
void PRN_HABILITAR_IMPRESORA_OPCIONAL();
void PRN_DESHABILITAR_IMPRESORA_OP();
int IS_PRINTER();
int PAPER_OUT();
int ERROR_IMPRESORA(void );
int TM930_IS_PRINTER();
void TM930_ABRIR_CAJON();
int TM930_CAJON_CERRADO();
int TM930_PAPER_OUT();
//int RECIBIR_RESPUESTA_IMPRESORA( unsigned char *rta, unsigned char cant );
int __SEND_BYTE( unsigned char byte, char testear );
int __SEND_BYTE_CODIGO( unsigned char byte );
void SEND_BYTE_SERIAL( unsigned char byte );
void SET_IMPRESORA( int puerto, int paralelo );
void OPEN_COPIA_PRN();
void CLOSE_COPIA_PRN();
void IMPRIME_COPIA_PRN();
void IMPRIME_GRANDE( int x, char *cadena );
int LEER_STATUS_PRINTER();
int COMANDO_PRN_DRIVER( int comando, int bx );
int REINTENTAR_impresion();
void CORTE_DE_HOJA_MANUAL();
void OPEN_COPIA_TICKET_PRN();
void OPEN_COPIA_VOUCHER_PRN();
void CLOSE_COPIA_TICKET_PRN();
void CLOSE_COPIA_VOUCHER_PRN();
void CLOSE_COPIA_Z_PRN();
int IMPRIME_COPIA_VOUCHER(int reimpresion);
void IMPRIME_COPIA_TICKET(int reimpresion);
int _IMPRIME_COPIA_PRN( char *archivo, int copiando_a_archivo, int imprimir_cabecera );
void GRABA_COPIA( char comando, int param );
int SINCRONIZAR_PRINTER();
void VERIFICAR_IMPRESORA();
int ARMAR_PAQUETE_FISCAL( char comando, char cantidad, unsigned char *texto, double monto1,
                          double monto2 );
int ARMAR_PAQUETE_FISCAL_EPSON( char comando, char cantidad, unsigned char *texto, double monto1,
                                double monto2 );
int TM930_COMANDO( char *buffer_envio, int cantidad_envio, char *buffer_recibir,
                   int cantidad_recibir );
void PRN_HABILITAR_IMPRESORA_OP_VOUCHER();
void PRN_DESHABILITAR_IMPRESORA_OP_VOUCHER();
void _CARGAR_CONFIGURACION();
void ASIGNAR_SECUENIA_FISCAL( unsigned char *datos, int offset );
void QUITAR_CARACTERES_NO_FISCALES( unsigned char *texto, int len );
int RECIBIR_RESPUESTA_IMPRESORA_XON_XOFF( unsigned char *rta, unsigned char cant, char delimitador,
                                          int extra );
int TM930_OPCIONAL_COMANDO( char *buffer_envio, int cantidad_envio, char *buffer_recibir,
                            int cantidad_recibir );
int TM930_OPCIONAL_PAPER_OUT();
void IMPRIME_COPIA_COBRO();
void CLOSE_COPIA_COBRO_PRN();
int SLIP_HABILITADO();

void PRN_HABILITA_SLIP( int orientacion, int enviar_blanco );

void ABRIR_JOURNAL_ELECTRONICO( void );
void CERRAR_JOURNAL_ELECTRONICO( void );
void GRABAR_JOURNAL_ELECTRONICO( unsigned char byte );
void CALCULAR_CHECK_SUM( unsigned char *cadena, unsigned char *cadena2 );
void BACKUP_JOURNAL_ELECTRONICO( void );
void INIT_JOURNAL_ELECTRONICO( void );
int GET_COLUMNA( void );
int TM930_IS_SLIP_PRESENT();

void PRN_HABILITAR_LECTURA_CHEQUE();
void PRN_LEER_CHEQUE();
int PRN_HAY_DATOS();
int LEER_DATOS_CHEQUE_IMPRESORA();

int REDIRECCION_PRINTER_A_BUFFER( int cant_columnas, int cant_filas );
void FIN_REDIRECCION_PRINTER_A_BUFFER();
void DISK_FLUSH( void );

void PRINT_BITMAP(char *buff);
int SET_BITMAP( char *buff );
void SET_BAR_CODE( char *buff );
void BAR_CODE();
void CONTROLAR_ESTADO_IMPRESORA();
int REPETIR_MIENTRAS_IGUALES( char* mensaje, int* valA, int valB );
void GRABAR_JOURNAL_ELECTRONICO2( char *cadena );
int RECIBIR_RESPUESTA_IMPRESORA( unsigned char *rta, unsigned char cant );
void PRN_LETRA_NEGRITA(  );
void PRN_DESHABILITA_FORMATOS_LETRAS(  );
void OPEN_Z_PRN();
void CARACTERES_ESPECIALES( char *texto, int origen );

/***************** Tipos de Letras *****************/
#define LETRA_CHICA          0
#define LETRA_GRANDE         1
#define LETRA_NEGRITA        2
#define DESHABILITA_FORMATO  10
#endif
#pragma pack (pop) 
