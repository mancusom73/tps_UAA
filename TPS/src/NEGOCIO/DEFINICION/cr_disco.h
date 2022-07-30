#pragma pack (push,1) 
#ifndef _CR_DISCO_H
#define _CR_DISCO_H
/*------------*
 * Public     *
 *------------*/

enum {_alta_ = 1, _baja_ = 5, _modificacion_ = 1 } operacion;


int ABRIR_STRING_TABLE();
int INICIALIZA_ARCHIVOS( int inicial );
void CIERRA_ARCHIVOS();
void CIERRA_TODOS_LOS_ARCHIVOS();
void CREAR_ARCHIVOS_SPAR();

int BUSCA_COD_INTERNO_COD_BARRA( long cod, char *cod_barra );
int ASIGNA_COD_ESPECIAL_COD_BARRA( long cod, char *cod_barra );
int BUSCA_CODIGO_ESPECIAL( long cod );
int ASIGNA_CODIGO_ESPECIAL( long cod );
int BUSCA_CODIGO_BARRA( char *cod_barra );

int ASIGNA_CODIGO_BARRA( char *cod_barra );
int EXENTO_PARA_DIPLOMATICOS( void );

int ABRIR_ARCHIVO_BOLETIN_TCOMPRA_MUTUALES();
int ABRIR_ARCHIVO_CLIENTE();

int OBTENER_COD_GRAVADO();
int INDICE_IVA_ARTICULO();
int GET_AREA_ARTICULOS();
void CALCULA_ARTICULO_EXT();
int ASIGNA_ITEM( int segunda_pasada );

int AGREGAR_ALTAS();
int AGREGAR_MODIFICACIONES();
int GUARDAR_MODIFICACIONES_DEUDAS( int max_procesada );

int ACTUALIZA_DEUDAS( void );

int ABRE_ARTICULOS();
void CIERRA_ARTICULOS();
int ABRE_DEUDAS();
int CIERRA_DEUDAS();
int ABRIR_ARCHIVO_SOCIO( void );
int CLIENTE_GUBERNAMENTAL( void );

//#ifdef  _CR_DISCO_C
/*------------*
 * Private    *
 *------------*/

void ALTA_ARTICULO( struct _articulo *altas, struct _present *altas_present, int *_c1, int *_c2 );
void INICIALIZA_ARCHIVOS_RECONECCION();
int BUSCA_DESCRIPCION( char *cadena );
//void      CALCULA_IMPUESTOS(  );
void PREPARA_REGISTRO_ARTICULOS( int precio_original);
//void CEREAR_ARCHIVO_BINARIO( int cod_archivo );
int ACTUALIZA_MONITOR( int estado );
int ACTUALIZA_HISTORICO( long estado, int subestado, double importe );
void CALCULAR_DIFERENCIA_AFINIDAD( void );
int EXISTE_DEUDA( long id_deuda );
void MODIFICAR_ALICUOTA_DE_ARTICULO( );
int ALICUOTA_MODIFICADO( int cod_exento, char *cod_barra, long cod_interno );
//int _ENVIAR_PAQUETE_TCP_IP( char *paquete, int l_paquete, char *rta, int l_rta, int timeout_nodo,
  //                          char espera_rta, int canal );
//int INIT_DRV_TCP_IP();
//int CERRAR_CANAL_TCP_IP();
//int ABRIR_CANAL_TCP_IP( int canal, char *ip, int formato_mensaje );
//int PING_ABRIR_SOCKET( char *ip_addres );
//int PING_CLOSE_SOCKET();
//int TCP_ftp( char *argv[] );
//int FTP_ARCHIVO( char *origen, char *destino, int get_put, int modo );
//unsigned TCP_canal_puerto( int canal, int puerto );
//#endif
#endif

#pragma pack (pop) 
