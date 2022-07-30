#pragma pack (push,1) 
#ifndef _CONT_SUB_H_
#define _CONT_SUB_H_

//#define _DEBUG_DIF
//#define _DEBUG_RNV_REC

int CONTROL_IMPORTE_DEVOLUCION( );
int ACUMULAR_IMPORTE_DEVOLUCION( int medio, double importe );

int    CONTROL_IMPORTE_SUBMEDIO( int medio, int submedio, int acumulado );
double OBTENER_IMPORTE_SUBMEDIO( int medio, int submedio, int acumulado );
int    OBTENER_IMPORTE_Y_DECLARACION_SUBMEDIO( int medio, int submedio, 
        double *importe, double *importeDec );

int    SUMAR_IMPORTE_SUBMEDIO( int medio, int submedio, double importe );
int    GUARDAR_DECLARACION_SUBMEDIO( int medio, int submedio, double importeDec );

void   VACIAR_DECLARACION_SUBMEDIO();
void   VACIAR_SUBMEDIO();
void   VACIAR_IMPORTE_MEDIO( int medio);

#endif
#pragma pack (pop) 