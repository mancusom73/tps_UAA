#ifndef MENSAJE_H
#define MENSAJE_H
void MENSAJE_SIN_SONIDO( char *cadena, unsigned long pPausa );
void MENSAJE( char *cadena );
void MENSAJE_CON_PAUSA( char *cadena, int pausa );
void MENSAJE_CON_PAUSA_CAJA( char *cadena, int pausa );
void MENSAJE_CON_TECLA( char *cadena, int tecla );
void MENSAJE_TECLA( char *cadena );
int MENSAJE_CON_TECLA_S_N( char *cadena, int tecla_si, int tecla_no );
void MENSAJE_SIN_PAUSA( char *cadena );
#endif
