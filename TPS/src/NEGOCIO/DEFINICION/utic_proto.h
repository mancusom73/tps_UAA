#ifndef UTIC_PROTO_H
#define UTIC_PROTO_H

/*--------------------------- Funciones --------------------------*/
void COMP_ajustar_largo( char *cadena );
extern int COMP_evaluar_condiciones( int codigo );
int       CARGAR_CONF_FACTURA(  );
int       CARGAR_CONF_REMITO(  );
long      _tomar_long( char **cadena );
int       _tomar_fecha( char **cadena );
float     _tomar_float( char **cadena );
int       _tomar_entero( char **cadena );
int       CARGAR_CONFIG_TKT( char *nom_arch, /*unsigned char*/struct _ConfTkt *config[], int largo );
void      TRANSFORMAR_CADENA_HEXA( char *cadena, int largo );
void      _tomar_cadena( char **cadena );
int       CARGAR_CONF_TKT(  );
void		 LIBERA_UTI_COMP(  );
void		 _tomar_cadena_dos( char **cadena, char **cobtenida );

#endif
