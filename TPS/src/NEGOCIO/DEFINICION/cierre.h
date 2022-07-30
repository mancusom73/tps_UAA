#pragma pack (push,1) 
#ifndef CIERRE_H
#define CIERRE_H
//#include <tpv_dat.h>
/*------------*
 * Public     *
 *------------*/

int cant_mem_rendicion_medio[10];

struct rendicion_en_memoria
{
    INT16 codigo;
    char desc[21];
    double monto;
    double monto_retiro;
    INT16 cantidad;
};

struct _datosCierre
{
    double tot[10],dif[10],cierre[10],tot_me[10],dif_me[10],cierre_me[10];
};

int CIERRE();
int GRABAR_ENTRADA_CAJERO();
int GRABAR_SALIDA_CAJERO();
int ARQUEO();
struct rendicion_en_memoria *RENDICION_MEDIO( int medio, int posicion );
void VACIAR_CIERRE_DE_LOTE( void );

/*------------*
 * Private    *
 *------------*/

//#include <inf_dif.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <aarch.h>
#include <panel.h>
#include <memory.h>
#include <btrv.h>
#include <string.h>
#include <stdio.h>
#include <actions.h>
#include <typeacti.h>
#include <path.h>

struct rendicion_en_memoria *mem_rendicion_medio[10];
//int c = 0;

#define SALIR      1
#define ATRAS      2
#define ADELANTE   3
#define OK         4
#define P_ATRAS    5
#define P_ADELANTE 6

struct difSubmedio
{
    char strDif[40];
    double importe;
}        diferencias[10][100];  //[medio][submedio]

struct _ev_dmedios dpago_boleta_plata;
//extern struct sub_medios sub_medios;

typedef struct
{
    short int medio;
    short int sub_medio;
    short int terminal;
    short int lote;
} ind1_cie_lote;

void GRABAR_EVENTO_DIFERENCIAS( int modo, double cierre, double tot, double cierre_me,
                                double tot_me, int submedio );
int CONTINUA_BUSCA_EVENTO( void );
double DISCRIMINAR_CIERRE( struct _medios *, struct rendicion_en_memoria *, int cantidad );
int LLENAR_ARRAY_DIFERENCIAS( void );
void CREAR_ARRAY_DIFERENCIAS( void );
void FREE_ARRAY_DIFERENCIAS( void );
void IMPRIME_DISC_RETIROS( char *desc, double monto );
void CIERRE_RETIRO_DISCRIMINA_SUBMEDIOS( double *cierre );
double PEDIR_IMPORTE_CIERRE( int *reproceso, double monto, double monto_retiro, int i );
struct rendicion_en_memoria *BUSCAR_ESTRUCTURA_EN_MEMORIA( char medio, char id_registro );
void MOSTRAR_ITEM_RENDICION( struct rendicion_en_memoria *mem, int pant_pos );
void SELECCIONA_ITEM_RENDICION( char *nombre, struct rendicion_en_memoria *mem, int pant_pos );
int REINTENTA_RENDICION();
void CIERRE_SUBMEDIOS_CONFECCIONA_PANTALLA( int cierre );
int ABORTA_CIERRE();
void INGRESA_IMPORTE_DISC_CIERRE( int h, double tot, double *cierre, double *dif, double tot_me,
                                  double *cierre_me, double *dif_me );
void CIERRE_CONFECCIONA_PANTALLA( int cierre );
void INGRESA_IMPORTE_CIERRE( int h, double tot, double *cierre, double *dif, double tot_me,
                             double *cierre_me, double *dif_me );
void CIERRE_RETIRO( double *cierre, double *cierre_me );
void GRABAR_CABECERA_EVENTO_INFORMES( int reintento, int incrementa_evento );
void GRABAR_CABECERA_EVENTO_REIMPRESION( int valor );
void ACTUALIZA_CABECERA_EVENTO();
void MOSTRAR_ITEM_RENDICION_EN_JAVA( int pant_pos, char *nombre, double importe,
                                     double diferencia );
void INIT_VEC_DIFERENCIAS( int medio );
double ACUMULAR_CIERRE_LOTE( int medio, int sub_medio );
int GRABAR_CIERRE_DE_LOTE( double importe, int terminal, int lote, int medio, int sub_medio,
                           long *registro );
void IMPRIME_VALE_DIFERENCIA( int medio, int submedio, double importe, int duplicado );
void IMPRIME_VALE_DIFERENCIA_CONTINUO( struct _datosCierre *datosCierre, int duplicado );
void ELIM_MEDIO_CIERRE_DE_LOTE( int medio );
void VACIAR_DEC_SUBMEDIOS();
#ifdef _DEBUG
void DEBUG_VERIFICAR_CONSISTENCIA_DIFERENCIAS( double tot[] );
#endif
int MOSTRAR_SUBMEDIO( int medio, int submedio );
#endif
#pragma pack (pop) 
