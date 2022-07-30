#ifndef SOCIOS_H
#define SOCIOS_H
long PEDIR_CLIENTE( short int cod_cobro, char tipo_cli );
int EXISTE_SOCIO( int tipo_busqueda, long cliente, short int cod_un_negocio, short int cod_cobro,
                  char tipo_cli, char *cuit );
//int       ELEGIR_SOCIO_ALFABETICO( void );
//int       ELEGIR_SOCIO_CODSOCIO( void );
int ELEGIR_SOCIO( int ( *f_usuario )( int, char *, int, struct _browse *, double ),
                  int alfabetico );
int _consulta_socio( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
int _consulta_socio2( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
int _consulta_socio3( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
int _consulta_socio4( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
int CLIENTE_AUTORIZADO( long nroCli, short int cod_cobro, short int tipo_cli );
long PEDIR_CLIENTE_ALFABETICO( void );
long PEDIR_CLIENTE_CODSOCIO( void );
long PEDIR_CLIENTE_( int busqueda );
int ACTUALIZA_NOVEDADES_SOCIOS( void );
#endif
