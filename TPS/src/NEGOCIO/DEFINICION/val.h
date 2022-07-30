#ifndef VAL_H
#define VAL_H
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

#include "_val.h"

extern    int VAL_LEER_BANDA_TRACK1( char *buffer );
extern    int VAL_LEER_BANDA_TRACK2( char *buffer );
extern    int VAL_STATUS( void );
extern    int VAL_ESCRIBIR_TARJETA( unsigned char dir, unsigned char cant, unsigned char *buffer,
                                unsigned char *_rta1, unsigned char *_rta2 );
extern    int VAL_BORRAR_TARJETA( unsigned char dir, unsigned char cant, unsigned char *_rta1,
                              unsigned char *_rta2 );
extern    int VAL_LEER_TARJETA( unsigned char dir, unsigned char cant, unsigned char *buffer,
                            unsigned char *_rta1, unsigned char *_rta2 );
extern    int VAL_COD_SECRETO_TARJETA( unsigned char *clave, unsigned char *_rta1,
                                   unsigned char *_rta2 );
extern    int VAL_COD_BORRADO1_TARJETA( unsigned char *clave, unsigned char *_rta1,
                                    unsigned char *_rta2 );
extern    int VAL_COD_BORRADO2_TARJETA( unsigned char *clave, unsigned char *_rta1,
                                    unsigned char *_rta2 );
extern    int VAL_VERSION( char *buffer );
extern    int VAL_SELECT_CARD( char tipo );
extern    int VAL_POWER_ON( void );
extern    int VAL_POWER_OFF( void );
extern    int VAL_SOLENOIDE( void );
extern    int VAL_ENCRIPTADO( char *buf1, char *buf2 );
extern    void VAL_SET_PUERTO( int puerto );
extern    int VAL_INICIALIZAR_PUERTO( void );
extern    void VAL_DESINSTALAR_PUERTO( void );
extern    int VAL_ENCRIPTADO( char *buf1, char *buf2 );
extern    int VAL_ABRIR_CAJON();
#endif
