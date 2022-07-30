#ifndef ARCHIVOS_H
#define ARCHIVOS_H
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

//#include <aarch.h>

#define ARCH_LISTA1_001         30
#define ARCH_LISTA2_001         31
#define ARCH_SISTEMA_LOG        93
#define ARCH_BTRIEVE_LOG       134
#define ARCH_X_BAK             135
#define ARCH_Y_BAK             136
#define ARCH_Z_BAK             137

#define ARCH_M_Z              1176
#define ARCH_M_X              1193
#define ARCH_M_Y              1194
void NOM_ARCHIVO( int cod, char *destino );
int AREA_OF( int cod );
int SET_AREA( int cod, int area );
int INIT_ARCHIVOS();
int COPY( int origen, int destino );
int APPEND( int origen, int destino );
int OPEN( int cod, int modo );
int OPEN_O_CREAT( int cod, int modo );
int CREAT_( int cod, int modo );
int SETFTIME( int cod, unsigned fecha );
int ARCHIVO_LOCAL( int cod );
int COD_CARPETA( int cod );
int CARPETA_LOCAL( int cod );
void LOG_EXISTE_ARCHIVO_COD( int cod );
void NOM_CARPETA_ARCHIVO( int cod, char *destino );
#endif
