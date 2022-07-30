#ifndef PERFILES_H
#define PERFILES_H
//int       RECORRER_CLASIF1( struct _clasif1 *clasif1, struct _datos_recorrer *datos ) ;
int MENU_PERFILES();
void INGRESA_PERFIL();
int ACTUALIZA_NOM_PERFIL();
long COD_DESCUENTO_PERFIL();
void MUESTRA_PERFIL();
int ABRIR_PERFILES();
void CERRAR_PERFILES();
float CALCULA_PORC_PERFIL( long cod, long cod_clasificacion );
float PORC_DESC( int cod );
int INGRESA_NRO_LEGAJO_EMPLEADO();
int ASIGNA_PERFILES( char *cadena );
int TIPO_PERFIL( void );
#endif
