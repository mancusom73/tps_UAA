#ifndef CLAVE_H
#define CLAVE_H
char *MK_FP_BIOS_PROTEGIDO( unsigned off );
int PIDE_SUPERVISOR( int x, int y, int tipo, char *titulo );
long LEER_NUMERO_USUARIO( int longitud_a_tomar );
int INGRESAR_CLAVE_MANUAL( int x, int y, int tipo, int cajero, int sup );
int INGRESAR_CLAVE_PCOS( int x, int y, int tipo );
int INGRESAR_CLAVE_GP416( int x, int y, int tipo, int cajero, int sup );
void CODIFICA_CLAVE( char *buffer, char *codigo );
int LEE_CLAVE_CAJERO( int nro, char *buffer );
int BUSCA_CAJERO_X_LEGAJO( long legajo );
int CLAVE_ANTERIOR();
int TIPO_USUARIO();
void CLAVE_ANTERIOR_GP( char *buffer );
int USUARIO_CLAVE_ANTERIOR();
int SOLICITAR_CLAVES( int x, int y, int cajero, int supervisor, int condicion, char *titulo,
                      char *log, int funcion, int grabar_en_event );
int VERIFICAR_CLAVE_EN_ARCHIVO( int clave, int tipo, int cajero, int sup );
int LEER_TARJETA_CONNAC( int x, int y, int tipo, int usuario_esperado );
int BCD_A_INT( unsigned char *buffer );
long BCD_A_NRO_USUARIO( unsigned char *buffer );
int BCD_A_CHAR( unsigned char buffer );
int PREPARA_GP416();
//int INGRESA_GP416();
void SACA_GP416();
long OBTENER_USUARIO( long usuario_esperado );
int OBTENER_TIPO( int tipo_esperado );
int OBTENER_CLAVE( int clave );
int USUARIO_PUEDE_RETIRAR( int nro );
int INGRESAR_CLAVE( int x, int y, char *mensaje, int tipo, int cajero, char *titulo, int funcion,
                    int pant_inicial, int grabar_en_event );
int LEE_CAJERO( int nro, struct _cajeros *datos_cajero );
void MOSTRAR_CAJERO_EN_PANTALLA();
long __BCD_A_NRO_USUARIO( unsigned char *buffer );
long PIDE_SUPERVISOR_LONG( int x, int y, int tipo );
int BUSCA_CAJERO_X_DNI( long dni );
int BUSCA_CAJERO_X_CODIGO( int nro );
int CLAVE_EXTENDIDA( int tipo );
int INGRESA_GP416();
#endif
