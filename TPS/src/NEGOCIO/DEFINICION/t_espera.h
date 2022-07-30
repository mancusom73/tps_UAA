#ifndef T_ESPERA_H
#define T_ESPERA_H
int TICKET_EN_ESPERA( int modo );
int CONTROLAR_TICKET_EN_ESPERA();
//void      TRAER_TICKET_EN_ESPERA(  );
static int GRABA_TICKET_EN_ESPERA( char nro_ticket );
static int RECUPERA_TICKET_EN_ESPERA( char nro_ticket );
int GRABAR_STRUCT( int handle, int est );
int _GRABAR_STRUCT( int handle, int est, int subindice );
int LEER_STRUCT( int handle, int est );
int _LEER_STRUCT( int handle, int est, int subindice );
void IMPRIME_MENSAJE_TICKET_EN_ESPERA( void );
void IMPRIME_MENSAJE_TRAE_TICKET_EN_ESPERA( void );
int AUTORIZA_TICKET_EN_ESPERA();
void MOSTRAR_T_ESPERA_POR_PANTALLA();
int BUSCAR_TICKET_LIBRE( void );
void MOSTRAR_MONEDERO_POR_PANTALLA();
int REVERTIR_DIFERENCIA_PRECIO( long caja_z, long id_evento, int posicion );
void PROCESAR_TICKET_EN_ESPERA( int modo );
#endif
