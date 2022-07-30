#ifndef VAL2_H
#define VAL2_H
int VAL_LEER_BANDA_TRACK1( char *buffer );
int VAL_LEER_BANDA_TRACK2( char *buffer );
int VAL_STATUS();
int VAL_RESET();
int VAL_ANULA_APAGADO();
int VAL_PERMITE_APAGADO();
int VAL_ESCRIBIR_TARJETA( unsigned char dir, unsigned char cant, unsigned char *buffer,
                          unsigned char *_rta1, unsigned char *_rta2 );
int VAL_BORRAR_TARJETA( unsigned char dir, unsigned char cant, unsigned char *_rta1,
                        unsigned char *_rta2 );
int VAL_LEER_TARJETA( unsigned char dir, unsigned char cant, unsigned char *buffer,
                      unsigned char *_rta1, unsigned char *_rta2 );
int VAL_COD_SECRETO_TARJETA( unsigned char *clave, unsigned char *_rta1, unsigned char *_rta2 );
int VAL_COD_BORRADO1_TARJETA( unsigned char *clave, unsigned char *_rta1, unsigned char *_rta2 );
int VAL_COD_BORRADO2_TARJETA( unsigned char *clave, unsigned char *_rta1, unsigned char *_rta2 );
int VAL_VERSION( char *buffer );
int VAL_SELECT_CARD( char tipo );
int VAL_POWER_ON();
int VAL_POWER_OFF();
int VAL_SOLENOIDE();
int VAL_ENCRIPTADO( char *buf1, char *buf2 );
int VAL_ABRIR_CAJON();
int VAL_PAYFLEX_SELECT( unsigned dir, int tipo );
int VAL_PAYFLEX_READ( char registro, int bytes, char *buffer );
int VAL_PAYFLEX_WRITE( char registro, char *buffer, int bytes );
int VAL_PAYFLEX_CLAVE( unsigned clave );
int VAL_INICIALIZA_LECTOR();
int VAL_MPCOS_READ( char registro, int bytes, char *buffer );
#endif
