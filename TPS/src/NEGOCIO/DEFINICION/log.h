#ifndef LOG_H
#define LOG_H
#include <stdio.h>
void GRABAR_LOG_SUPERVISOR_STRING( int id_cadena, int supervisor, int origen, int nivel );
void _GRABAR_LOG_SUPERVISOR_STRING( int id_cadena, int supervisor );
void GRABAR_LOG_SUPERVISOR( char *operacion, int supervisor , int origen, int nivel );
void _GRABAR_LOG_SUPERVISOR( char *operacion, int supervisor );
/*int*/ void GRABAR_LOG_SISTEMA( char *operacion , int origen, int nivel );
int _GRABAR_LOG_SISTEMA( char *operacion ,int origen, int nivel );
void _LOG_EXISTE_ARCHIVO( char *nom_arch );
void GRABAR_LOG_MEMORIA();
void _GRABAR_LOG_MEMORIA();
void GRABAR_LOG_NRO_TICKET();
int _GRABAR_LOG_BTRIEVE( char *cadena );
int GRABAR_LOG_BTRIEVE( char *cadena, int origen, int nivel );
int GRABAR_LOG_BD( char *cadena, int error, int origen, int nivel );
int _GRABAR_LOG_BD( char *cadena, int error, int origen, int nivel );
int GetConfIniclienteDbServer(int q_cliente);
void VACIAR_LOG();
//void glog( char *men, int origen );
int logPanelVentas( void );
int logPanelPagos( void );
int logPanelPosPago( void );
int putPurchase( int value, FILE *flujo );
void GRABAR_LOG_SISTEMA_CONFIG( char *cadena, int origen, int nivel );
int DEBE_GRABAR_LOG_SISTEMA1( int origen, int nivel );
void ERROR_SISTEMA_ERRNO(void);
double CORTAR_ARCHIVOS_LOG_GRANDES( void );
#endif
