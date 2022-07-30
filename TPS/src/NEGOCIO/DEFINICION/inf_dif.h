#ifndef _INF_DIF_H
#define _INF_DIF_H
/*------------*
 * Public     *
 *------------*/

void INFORME_DIFERENCIAS( double *dif, int cierre, double *dif_me );
void INFORME_DIFERENCIAS_SUBMEDIOS_CONTINUO();
void INFORME_DIFERENCIAS_SUBMEDIOS_RETAIL( char tipo_cierre );
void INFORME_DIFERENCIAS_SUBMEDIOS( double *cierre, double *tot, int en_cierre, double *cierre_me,
                                    double *tot_me );
struct _dif_submedios
{
    char nombre_medio[30];
    char cod_medio;
    int cod_submedio[30];
    char nombre_submedio[30][30];
    double imp_declarado[30];
    double imp_real[30];
} dif_submedios[10];

//#ifdef  _INF_DIF_C
/*------------*
 * Private    *
 *------------*/

//#include <cierre.h>
#include <math.h>
#include <actions.h>

void INFORME_DIFERENCIAS_SUBMEDIO_CUERPO( char *medio, char *submedio, double monto_caja,
                                          double monto_retiro, double monto_caja_me,
                                          double monto_retiro_me );
void IMPRIMIR_USUARIO_Y_SUPERVISOR( void );
void INFORME_DIFERENCIAS_CUERPO( char *nombre, double importe, double importe_me );
void INFORME_DIFERENCIAS_DISC_CUERPO( char *nombre, double importe );


//#endif
#endif
