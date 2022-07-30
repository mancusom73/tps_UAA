#ifndef _CR_DISC2_H
#define _CR_DISC2_H
/*------------*
 * Public     *
 *------------*/

int VERIFICA_CAMBIO_ARCHIVOS_INICIAL();
int VERIFICA_CAMBIO_ARCHIVOS_ACTIVOS(int inicial);     

//#ifdef  _CR_DISC2_C
/*------------*
 * Private    *
 *------------*/

//Definiciones para la transferencia de archivos GZ
//Acciones
#define AGZ_A_TRANSFERIR        0x1
#define AGZ_A_SOLO_COMP_ACCESO  0x2
//Estados
#define AGZ_E_OK_COMENZAR       0x0
#define AGZ_E_OK_NO_COMENZAR    0x1
#define AGZ_E_ERR_GZIP          0x2
#define AGZ_E_ERR_BANDERA       0x3
#define AGZ_E_ERR_MAPEO         0x4
#define AGZ_E_ERR_DESC          0x5
#define AGZ_E_ERR_COPIA         0x6

int VERIFICA_FLAGS2( int inicial );
int PROCESA_FLAGS_COMUNES( unsigned bandera, unsigned bandera2 );
int INIT_COPIAR_BOLETIN();
int COMPROBAR_ARCH_GZ( const char *path_orig, char *pnomb_orig );
int COPIAR_ARCH_GZ( const char *path_orig, char *pnomb_orig, const char *path_dest );
int ARCH_GZ( int accion, const char *path_orig, char *nomb_orig, const char *path_dest );

//#endif
#endif
