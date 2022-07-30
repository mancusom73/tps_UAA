#ifndef MED_REIN_H
#define MED_REIN_H
void MEDIO_REINT( void );
void ACTUALIZA_ACUMULADORES_CARGA_RAPIDA_CHEQUES( double total, int h, int signo );
void RECUPERO_DE_CHEQUES_CARGADOS();
int ACTUALIZA_MREINTEG( long codnivel1, long codigo, short nro );
int SUCURSAL_DE_CAJA( void );
void MREINTEG_EN_CIERRE( void );
long OBTENER_CODIGO_REINTEG( long codnivel1, long codigo, short nro );
#endif
