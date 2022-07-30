#ifndef XYZ_H
#define XYZ_H
int GRABAR_XYZ( int modo );
#if defined(INVEL_W)
int _GRABAR_XYZ( char *nom, int est, struct _xyz *cab_xyz );
#endif
void SUMAR_X_USUARIOS( char *destino );
void RESETEA_ACUMULADORES_X();
void RESETEA_ACUMULADORES_Z();
void ACUMULAR_X_EN_Z();
int GRABAR_BACKUP_XYZ( int modo );
void GRABAR_EVENTO_YZ( int modo );
#endif
