#ifndef BAUCHER_H
#define BAUCHER_H
void IMPRIME_BAUCHER( int p, int para_comercio, long anulacion );
int IMPRIME_VOUCHER( int p, int para_comercio, long anulacion );
void IMPRIME_VOUCHER_CTACTE( int p, int para_comercio, long anulacion, int modo );
void OBTIENE_NOMBRE_CLIENTE( char *nombre, int p_pago );
void _VOUCHER_BLOQUE1( char *titulo, long anulacion );
void _VOUCHER_BLOQUE2();
void MINI_VOUCHER_CMR();
int IMPRIME_VOUCHER_IDEM_POSNET( int p, int para_comercio, long anulacion );
#endif
