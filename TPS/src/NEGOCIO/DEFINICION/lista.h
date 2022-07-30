#ifndef LISTA_H
#define LISTA_H
void INICIALIZAR_TARJETAS();
void INICIALIZAR_BUSCA_TARJETA();
void FIN_BUSCA_TARJETA();
void LISTA_CARGA_INDICE( char nro );
int VALIDA_NUMERO_TARJETA( char *num_tarjeta, char marca );
int VALIDA_BOLETIN( char nro );
int BUSCA_TARJETA( char *nro_tarjeta, char cod_marca );
int f_cmp_busca_tarjeta_cab1( const void *_p1, const void *_p2 );
int f_cmp_busca_tarjeta_cab2( const void *_p1, const void *_p2 );
int f_cmp_busca_tarjeta_lista( const void *_p1, const void *_p2 );
int DEBE_PEDIR_CUPON( int marca, int cuotas, int financia_tarjeta );
int CARGAR_TARJETAS();
int NUMERO_BOLETIN( char nro );
int NUMERO_ARCHIVO( int marca );
char *NOMBRE_TARJETA( int marca );
float LIMITE2_TARJETA( int marca );
int TARJETA_ON_LINE( int marca );
int IMPRIME_MENSAJE_TARJETA( int marca );
int DEBE_PEDIR_COD_VIGENCIA( int marca );
int NODO_TARJETA( int marca );
int COD_VALIDACION_VOUCHER( int marca );
int BUSCA_MARCA_TARJETA( double numero, char *cod_tarjeta, float *limite );
int BUSCA_MARCA_TARJETA_SERVICIOS( double numero, char *cod_tarjeta, float *limite );
int EXISTE_TARJETA( int marca );
int BOLETINES_INICIALIZADOS();
void BUSCA_MARCA_TARJETA_ESPECIAL( char *cod_tarjeta );
//int EXISTE_PREFIJO_BARRA_TARJETA( char *bin, long *prefijo, int *l_tarj );
int EXISTE_PREFIJO_BARRA_TARJETA( char *bin, long prefijo, int *l_tarj );
#endif
