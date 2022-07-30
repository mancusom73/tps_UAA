#ifndef EXPORT_H
#define EXPORT_H
int EXPORTAR_ARCHIVO();
int EXPORTA_ARCHIVO_DE_LA_SEMANA( unsigned fecha, int origen, int destino );
int BACKUP_ARCHIVO_DE_LA_SEMANA( unsigned fecha, int origen, int destino );
int BACKUP_ARCHIVO_DE_LA_SEMANA_SQL( unsigned fecha, int origen, int destino );
int LEE_FECHA( int cod_arch );
void CEREAR_ARCHIVO_BINARIO( int cod_archivo );
#endif
