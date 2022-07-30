/*******************************************************************************
*   Sistema: TPV 1000 - IV
*
*   Descripcion :   Manejador de funcionalidades de Cajero Flotante
*
*--------------------------- HISTORIAL DE CAMBIOS ------------------------------
*
*         Autor	       		Fecha			Versión			Descripción
*   -----------------		------			--------		------------
*   Davalle Sebastián		28/12/07		  1.0			Versión Inicial
********************************************************************************/

//FR
#pragma pack (push, 1)

#include <cr.h>



struct __sub_medios
{
    int medios;
    int submedios;
    double importe;
    double importe_dec;
};

int cant_cierres_y;
int flotando;

int IMPORTAR_DATOS_CAJA_ANTERIOR(int cajero);
void GUARDAR_DATOS_IMPORTADOS();
void CALCULA_TOTAL_DE_CAJERO();
void ENVIAR_SUBMEDIOS_REC();
void UPDATE_SUBMEDIOS_REC();
void UPDATE_VARIABLES__X_();
void CHECK_SUBMEDIOS_REC();
void OBTENER_TABLAS_TEMPORALES(long secuencia, int caja, int fecha, long id_evento);
int ABRIR_EV_CIERRE( struct _ev_cierre *ev_cierre);
int OBTENER_TEMPORALES( int ev_requerido, int caja, long nro_ticket, long secuencia, int fecha, char origen, int online, long id_evento);
void UPDATE_VARIABLES__Z_();
int POSICIONA_PAGO( long id_evento );
#pragma pack (pop)
