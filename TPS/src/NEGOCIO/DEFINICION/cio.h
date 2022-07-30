#ifndef IO_H
#define IO_H
#ifdef INVEL_L
int filelength( int handle );
#endif
#ifndef INVEL_L
int _READ( int handle, char *buffer, int cant, int linea, char *file );
int _WRITE( int handle, char *buffer, int cant, int linea, char *file );
int LSEEK( int handle, long pos, int tipo, int linea, char *file );
int _CLOSE( int handle, int linea, char *file );
int _OPEN( char *nombre, int tipo, int linea, char *file );
int _CREAT( char *nombre, int tipo, int linea, char *file );
int _OPEN_O_CREAT( char *nombre, int tipo, int linea, char *file );
int COPIAR_ARCHIVO( char *nom_origen, char *nom_destino, int linea, char *file );
int APPEND_ARCHIVO( char *nom_origen, char *nom_destino, int linea, char *file );
void _SET_ERROR_EN_RED( void );
int error_disco();
int GET_ERROR_EN_RED();
void _SET_FECHA_ARCHIVO( int handle, unsigned fecha );
void _VERIFICAR_REINTENTO();
void GRABAR_ERROR( int operacion, int error, int intentos, char *nombre, int handle );
int ARCH_RED( int nro );
void VERIFICAR_REINTENTO();
int SET_FECHA_ARCHIVO( char *nom_archivo, unsigned fecha );
int _COPIAR_ARCHIVO( int modo, char *nom_origen, char *nom_destino, int linea, char *file );
void SET_ERROR_EN_RED( void );
int _GET_ERROR_EN_RED();
#endif
#endif
