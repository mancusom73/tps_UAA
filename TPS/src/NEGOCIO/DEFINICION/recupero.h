#ifndef _RECUPERO_H
#define _RECUPERO_H
/*------------*
 * Public     *
 *------------*/

int GUARDAR_ARCHIVO_REC( char *archivo, char *estructura, int bytes );
int RECUPERAR_ARCHIVO_REC( char *archivo, char *estructura, int bytes, int indice );
void ACTUALIZAR_ARCHIVO_REC( char *archivo, char *estructura, int bytes, int indice );
void BORRAR_ARCHIVO_REC( char *archivo );
int CANTIDAD_REGISTROS_REC( char *archivo, int bytes );
int RESTAURAR_COPIA_ARCHIVO_REC( char *archivo, int borrar_copia );
int CREAR_COPIA_ARCHIVO_REC( char *archivo, int forzar );
void BORRAR_COPIA_ARCHIVO_REC( char *archivo );

#ifdef  _RECUPERO_C
/*------------*
 * Private    *
 *------------*/

#endif
#endif

