#pragma pack (push,1) 
#ifndef MSTRING_H
#define MSTRING_H
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

void glog( char *men, int origen, int nivel );
void MENSAJE_STRING( int id );
void MENSAJE_TECLA_STRING( int id );
void MENSAJE_SIN_SONIDO_STRING( int id );
void MOSTRAR_CADENA_STRING( int x, int y, int id );
void ABORT_STRING( int id );
void MENSAJE_NO_ENCONTRADO( int id );
void LOG_MENSAJE_NO_ENCONTRADO( int id );
void _GRABAR_LOG_SISTEMA_STRING( int id ,int origen, int nivel);
void GRABAR_LOG_SISTEMA_STRING( int id ,int origen, int nivel);
void MENSAJE_CON_TECLA_STRING( int id, int tecla );
void MOSTRAR_CADENA_CENTRADA_STRING( int x, int y, int id );
void LOG__( char *men );
void MENSAJE_STRING_SIN_PAUSA( int id );
#endif
#pragma pack (pop)
