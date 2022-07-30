#include <assert.h>
#include <recupero.h>
#include "PATH.H"
#include "CONT_SUB.H"
#include <cr.h>
#include <b_mens.h>
#include <clave.h>
#include <cmemory.h>
#ifdef _DEBUG_RNV_REC
#include <ddtpv.h>
#include <math.h>
#endif
#pragma pack(1)

/** estructura que almacena el registro actual de trabajo */
struct _sub_medios
{
    int medios;
    int submedios;
    double importe;
    double importe_dec;
} submedioTrabajo;

/** posicion del registro actual de trabajo */
int posicionTrabajo;
int posicionTrabajoDev;

/** Almacena en el registro interno los datos del submedio solicitado, si es que existe.
 * @param medio el medio solicitado
 * @param submedio el submedio solicitado
 * @return 1 si se encontro el submedio 0 caso contrario
 *****************************************************************************/
int POSICIONAR_EN_REGISTRO( int medio, int submedio )
/*****************************************************************************/
{
    posicionTrabajo = 0;
    while( RECUPERAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS, ( char* )&submedioTrabajo, 
           sizeof( struct _sub_medios ), posicionTrabajo ) == 0 ) {
        if( medio == submedioTrabajo.medios && submedio == submedioTrabajo.submedios ) {
            return 1;
        }
        posicionTrabajo++;
    }
    return 0;
}

 /*****************************************************************************/
int POSICIONAR_EN_REGISTRO_DEVOLUCION( int medio )
/*****************************************************************************/
{
    posicionTrabajoDev = 0;
    while( RECUPERAR_ARCHIVO_REC( ARCH_REC_IMPORTE_DEVOLUCION, ( char* )&submedioTrabajo, 
           sizeof( struct _sub_medios ), posicionTrabajoDev ) == 0 ) {
        if( medio == submedioTrabajo.medios ) {
            return 1;
        }
        posicionTrabajoDev++;
    }
    return 0;
}

/** Acumula en el registro interno los datos del medio solicitado, si es que existe.
 * @param medio el medio solicitado
 * @return 1 si se encontro algun registro del medio. 0 caso contrario
 *****************************************************************************/
int ACUMULAR_EN_REGISTRO( int medio )
/*****************************************************************************/
{
    struct _sub_medios subTmp;
    int posTmp = 0;

    //Los valores se acumulan en la estructura global
    posicionTrabajo = 0;
    submedioTrabajo.medios = medio;
    submedioTrabajo.submedios = 0;
    submedioTrabajo.importe = 0.0;
    submedioTrabajo.importe_dec = 0.0;
    while( RECUPERAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS, ( char* )&subTmp, 
           sizeof( struct _sub_medios ), posTmp ) == 0 ) {
        if( medio == subTmp.medios ) {
            submedioTrabajo.importe += subTmp.importe;
            submedioTrabajo.importe_dec += subTmp.importe_dec;
            posicionTrabajo++;
        }
        posTmp++;
    }
    if( posicionTrabajo > 0 ) {     
        return 1;
    }
    return 0;
}

/** Verifica si hay dinero cargado en el POS para un determinado submedio.
/* @param medio medio de pago ( comienza en 1 )
/* @param submedio el codigo de submedio que se quiere verificar
/* @param acumulado determina si se utiliza el parametro 'submedio' o se controla todo el medio
/* @return 1 si se determina que hay dinero del sumedio en el POS, 0 si no
/*****************************************************************************/
int CONTROL_IMPORTE_SUBMEDIO( int medio, int submedio, int acumulado )
/*****************************************************************************/
{
    return( OBTENER_IMPORTE_SUBMEDIO( medio, submedio, acumulado ) > 0.001 );
}

/*****************************************************************************/
int CONTROL_IMPORTE_DEVOLUCION( )
/*****************************************************************************/
{
    int     rta = 0, i = 0, tipo_cajero = CAJERO;
	//char    aux[50],cadena[100];
	double  imp_med[9], total_Devolucion = 0;

    memset( imp_med, 0, sizeof( imp_med ) );
	posicionTrabajoDev = 0;
	total_Devolucion = 0;	

	if( CANTIDAD_REGISTROS_REC( ARCH_REC_IMPORTE_DEVOLUCION, sizeof(  struct _sub_medios ) ) ) { 
	    while( RECUPERAR_ARCHIVO_REC( ARCH_REC_IMPORTE_DEVOLUCION, ( char* )&submedioTrabajo, 
                sizeof( struct _sub_medios ), posicionTrabajoDev ) == 0 ) {
            if( medios[submedioTrabajo.medios - 1].efectivo )						   
			    imp_med[submedioTrabajo.medios - 1]+= submedioTrabajo.importe;	
			if( medios[submedioTrabajo.medios - 1].mutual ) 
				imp_med[submedioTrabajo.medios - 1]+= submedioTrabajo.importe;	
			if( medios[submedioTrabajo.medios - 1].ticket_compra )
				imp_med[submedioTrabajo.medios - 1]+= submedioTrabajo.importe;	
			if( medios[submedioTrabajo.medios - 1].tarjeta )
                imp_med[submedioTrabajo.medios - 1]+= submedioTrabajo.importe;	  
			/* if( medios[submedioTrabajo.medios - 1].cheque )
				imp_med[submedioTrabajo.medios - 1]+= submedioTrabajo.importe;	  
            if( medios[submedioTrabajo.medios - 1].cta_cte )
				imp_med[submedioTrabajo.medios - 1]+= submedioTrabajo.importe;	  
			*/
			posicionTrabajoDev++;
		}                				
    } 
	//Controla lo retirado por medio				
	for( i = 0; i < 9; i++ ) {
        if( imp_med[i] ) {
            if( medios[i].limite_dev_ger ) {
                if( medios[i].limite_dev_sup ) {// Valida gerente y supervisor
                    if( imp_med[i] >= medios[i].limite_dev_sup ) {
                        if( imp_med[i] <= medios[i].limite_dev_ger ) {
                            tipo_cajero = SUPERVISOR;
                        } else {
                            tipo_cajero = GERENTE;
                        }
                    } 
                } else {// Valida solo gerente
                    if( imp_med[i] >= medios[i].limite_dev_ger ) {
                        tipo_cajero = GERENTE;
                    }
                }
            } else {// Valida solo supervisor
                if( imp_med[i] >= medios[i].limite_dev_sup ) {
                    tipo_cajero = SUPERVISOR;
                }
            }
        }
         if( tipo_cajero == CAJERO ) {
             rta = 1;
         }
    }
	BORRAR_MENSAJE();
	if( tipo_cajero != CAJERO ) { 
	    if( SOLICITAR_CLAVES( 30, 20, NO, tipo_cajero, SI, "DEVOLUCION", "DEVOLUCION",
		        RAM_COD_FUNCION, SI ) ) {
			rta = 1;
		} else {
			rta = 0;
		}
	} else {    
        rta = 1;                                    
	}

    return( rta );
  
}

/** Obtiene el valor del importe del submedio solicitado.
/* @param medio medio de pago ( comienza en 1 )
/* @param submedio el codigo de submedio que se quiere obtener
/* @param acumulado determina si se utiliza el parametro 'submedio' o se controla todo el medio
/* @return el importe almacenado
/***********************************************************************************/
double OBTENER_IMPORTE_SUBMEDIO( int medio, int submedio, int acumulado )
/***********************************************************************************/
{
    int encontro = 0;

    if( M_ACUMULADO == acumulado ) {
        encontro = ACUMULAR_EN_REGISTRO( medio );
    } else {
        encontro = POSICIONAR_EN_REGISTRO( medio, submedio );
    }
    
    if( encontro ) {
        return( submedioTrabajo.importe );
    }
	return( 0.00 );
}

/** Obtiene el valor del importe y del importe declarado del submedio solicitado.
/* @param medio medio de pago ( comienza en 1 )
/* @param submedio el codigo de submedio que se quiere obtener
/* @param importe (salida) se almacena el importe del submedio ( 0 si no se encontro el registro )
/* @param importeDec (salida) se almacena el importe declarado del submedio ( 0 si no se encontro el registro )
/* @return 1 si se encontro el registro, 0 si no
/***********************************************************************************/
int OBTENER_IMPORTE_Y_DECLARACION_SUBMEDIO( int medio, int submedio, double *importe, double *importeDec )
/***********************************************************************************/
{
    *importe = 0.00;
    *importeDec = 0.00;
    if( POSICIONAR_EN_REGISTRO( medio, submedio ) ) {
        *importe =  submedioTrabajo.importe;
        *importeDec = submedioTrabajo.importe_dec;
        return 1;
    }
    return 0;
}

/** Incrementa el importe de un submedio.
/* @param medio medio de pago ( comienza en 1 )
/* @param submedio el codigo del submedio a modificar
/* @param importe el importe a sumar al submedio
/* @return 1 si se sumo a un submedio existente. 2 si se creo el registro del submedio
/***************************************************************************/
int SUMAR_IMPORTE_SUBMEDIO( int medio, int submedio, double importe )
/***************************************************************************/
{

    if( POSICIONAR_EN_REGISTRO( medio, submedio ) ) {
        submedioTrabajo.importe += importe;
        ACTUALIZAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS, (char*)&submedioTrabajo,
                sizeof( struct _sub_medios ), posicionTrabajo );
        return 1;                                                                   
    }  
    //No existe el registro, se crea uno nuevo
    submedioTrabajo.importe = importe;
    submedioTrabajo.importe_dec = 0;
    submedioTrabajo.medios = medio;
    submedioTrabajo.submedios = submedio;
    GUARDAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS, (char*)&submedioTrabajo,
            sizeof( struct _sub_medios ) );    
    return 2;
}
/***************************************************************************/
int ACUMULAR_IMPORTE_DEVOLUCION( int medio, double importe )
/***************************************************************************/
{

    if( POSICIONAR_EN_REGISTRO_DEVOLUCION( medio ) ) {
        submedioTrabajo.importe += importe;
        ACTUALIZAR_ARCHIVO_REC( ARCH_REC_IMPORTE_DEVOLUCION, (char*)&submedioTrabajo,
                sizeof( struct _sub_medios ), posicionTrabajoDev );
        return 1;                                                                   
    }  
    //No existe el registro, se crea uno nuevo
    submedioTrabajo.importe = importe;
    submedioTrabajo.importe_dec = 0;
    submedioTrabajo.medios = medio;
    submedioTrabajo.submedios = 0;
    GUARDAR_ARCHIVO_REC( ARCH_REC_IMPORTE_DEVOLUCION, (char*)&submedioTrabajo,
            sizeof( struct _sub_medios ) );    
    return 2;
}

/** Establece el importe declarado de un submedio.
/* @param medio medio de pago ( comienza en 1 )
/* @param submedio el codigo del submedio a modificar
/* @param importeDec el importe declarado del submedio
/* @return 1 si se establecio el valor de un submedio existente
/* 2 si el registro no existia, en este caso se crea solo si importeDec es > 0
/***************************************************************************/
int GUARDAR_DECLARACION_SUBMEDIO( int medio, int submedio, double importeDec )
/***************************************************************************/
{
    if( POSICIONAR_EN_REGISTRO( medio, submedio ) ) {
        submedioTrabajo.importe_dec = importeDec;
        ACTUALIZAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS, ( char* )&submedioTrabajo,
            sizeof( struct _sub_medios ), posicionTrabajo );
            return 1;
    }
    //No existe el registro, se crea uno nuevo ( si es necesario )
    if( importeDec > 0 ) {   
        submedioTrabajo.importe = 0;
        submedioTrabajo.importe_dec = importeDec;
        submedioTrabajo.medios = medio;
        submedioTrabajo.submedios = submedio;
        GUARDAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS, ( char* )&submedioTrabajo,
                sizeof( struct _sub_medios ) );    
    }
    return 2;
}

/** Elimina los importes declarados para TODOS los submedios.
 ***************************************************************************/
void VACIAR_DECLARACION_SUBMEDIO()
/***************************************************************************/
{
    while( RECUPERAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS, ( char* )&submedioTrabajo,
                                  sizeof( struct _sub_medios ), posicionTrabajo ) == 0 ) {
        if( submedioTrabajo.importe_dec != 0 ) {
            submedioTrabajo.importe_dec = 0;
            ACTUALIZAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS, ( char* )&submedioTrabajo,
                                    sizeof( struct _sub_medios ), posicionTrabajo );
        }
        posicionTrabajo++;
    }
}

/** Elimina los importes (medio-submedio) para un medio determinado
 ***************************************************************************/
void VACIAR_IMPORTE_MEDIO( int medio)
/***************************************************************************/
{
    int cnt = 0;
    struct _sub_medios subTmp;
    while( RECUPERAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS, ( char* )&subTmp,
                                  sizeof( struct _sub_medios ), cnt ) == 0 ) {
        if( subTmp.medios == medio ) {
            subTmp.importe = 0;
            ACTUALIZAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS, ( char* )&subTmp,
                                    sizeof( struct _sub_medios ), cnt );
        }
        cnt++;
    }
}

/** Elimina toda la informacion
 ***************************************************************************/
void VACIAR_SUBMEDIO()
/***************************************************************************/
{
    BORRAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS );  
}

/***************************************************************************/
#ifdef _DEBUG_RNV_REC
int CONTROL_CONSISTENCA_RNV_REC()
/***************************************************************************/
{
    double totales[10] = {0,0,0,0,0,0,0,0,0,0}, rnv[10] = {0,0,0,0,0,0,0,0,0,0};
    struct _sub_medios sub_mediosTmp;
    int i, cnt = 0;

    for( i = 0; i< 10; i++ ){
        rnv[i] = ( _X_VENTAS_IMPORTE( i ) + _X_COBROS_IMPORTE( i ) + _X_CAMBIOS_IMPORTE( i ) - _X_RETIROS_IMPORTE( i ) );
    }
    while( RECUPERAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS, ( char* )&sub_mediosTmp,
                                  sizeof( struct _sub_medios ), cnt ) == 0 ) {
        totales[sub_mediosTmp.medios - 1] += sub_mediosTmp.importe;
        cnt++;
    }
    for( i = 0;i < 10;i++ ) {
        assert( fabs( totales[i] - rnv[i] ) < 0.001 );
    }
    return 0;
}
#endif

/***************************************************************************/
#ifdef _DEBUG_DIF
void DEBUG_VERIFICAR_CONSISTENCIA_DIFERENCIAS( double tot_c[] )
/***************************************************************************/
{
    double totales[10] = {0,0,0,0,0,0,0,0,0,0};
    struct _sub_medios sub_mediosTmp;
    int i, cnt = 0;

    while( RECUPERAR_ARCHIVO_REC( ARCH_REC_SUBMEDIOS, ( char* )&sub_mediosTmp,
            sizeof( struct _sub_medios ), cnt ) == 0 ) {
        totales[sub_mediosTmp.medios - 1] += sub_mediosTmp.importe;
        cnt++;
    }
    for( i = 0;i < 10;i++ ) {
        assert( totales[i] == tot_c[i] );
    }
}
#endif
