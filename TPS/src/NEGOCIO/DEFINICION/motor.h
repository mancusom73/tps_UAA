#pragma pack (push,1) 
#ifndef __MOTOR_H__
#define __MOTOR_H__



/*-------------Constantes en comun motor - caja--------------------------*/

/*---TIPOS EN ARCHIVO ACTIVACION DE MEDIOS---*/

#define     ACTIVAR_MEDIO               1

/*---TIPOS EN ARCHIVO ASIGNACION DE BENEFICIO---*/

#define     ASIG_BENEF_BONO             1
#define     ASIG_BENEF_VALE             2
#define     ASIG_BENEF_STR_TICKET       3
#define     ASIG_BENEF_MONEDERO         4

/*---LUGAR DE IMPRESION---*/

#define     DENTRO_DEL_TICKET           1
#define     FUERA_DEL_TICKET            2
#define     ENCABEZADO_DEL_TICKET       3
#define     CUERPO_DEL_TICKET           4
#define     TOTAL_DEL_TICKET            5
#define     PIE_DEL_TICKET              6



/*---TIPOS EN ARCHIVO ASIGNACION DE BENEFICIO---*/

/*--ELEMENTOS BENEFICIADOS--*/
#define     BENEF_ARTICULOS             1
#define     BENEF_DEPARTAMENTO          2
//#define     BENEF_CLASIFICACION         3
#define     BENEF_PERFIL                4
#define     BENEF_MEDIO                 5
#define     BENEF_INCONDICIONAL         6
/*--BENEFICIOS OTORGADOS--*/
#define     BENEF_DESCUENTO           101
#define     BENEF_BONOS               102
#define     BENEF_PUNTOS              103
#define     BENEF_MONEDERO            104
#define     BENEF_ACTIVACION_MEDIO    105
#define     BENEF_MONEDERO_NO_ASIG    106 //USO SOLO POS, INDICA QUE NO SE QUISO ASIGNAR A MONEDERO POR EL CAJERO
#define     BENEF_REGALO              107




/*---CONDICION MONTO REQUERIR MEDIO - SUBMEDIO - PLAN---*/
/*Condiciones del monto requerido por promocion*/
#define     CONDICION_MAYOR_IGUAL       1
#define     CONDICION_MENOR_IGUAL       2
#define     CONDICION_TOTAL_TIKET       3

/*Origen del requerimiento de medio*/
#define     ORIGEN_PROMOCION			1
#define     ORIGEN_COBRO				2

/*------------------ TIPOS DE BONOS ------------------*/
/* 
 * Para indicar si el bono puede utilizarse como medio de pago,
 * o si se trata de un bono canjeable
 */
#define     BONO_HAB_PAGO               0
#define     BONO_CANJEABLE              1


/*-------Estructuras de comunicacion motor - caja------------------------*/

struct activacion_medios
{
    int tipo;               //Constantes
    long cod_promo;         //Codigo de la promocion grabada por el motor
    double monto;           //Monto maximo
    double monto_usado;     //Usado solo por Pos
    long medio;             //Medios a activar
    long submedio;          //Submedio a activar
    long plan;              //Plan a activar
};




struct asig_beneficio
{
    int tipo;               //tipos de asignacion de beneficios (constantes)
    int lugar_impresion;	//lugar de impresion
    int nro_validacion;     //Numero de validacion correspondiente al val-caja
    int cantidad;           //Idica la cantidad en caso que el beneficio lo requiera Ej: 5 bonos
    int vigencia;           //Idica la vigencia en caso que el beneficio lo requiera Ej: El bono estara vigente por 5 dias
    double monto;           //Indica el monto a asignar en caso que el beneficio lo requiera Ej: vale $50
    char msg_string[256];   //Mensaje a imprimir en el ticket, en caso que no entre en los 256 se grabaran 2 o 3 etc.
    int asignado;           //asignado, usado solo por el pos, indica si ya se asigno(ejecuto) el beneficio
};


struct eventos_promociones
{
    int b_promo;            //tipos de evento de promociones (constantes)
    int cod_promo;			//identificador de la promocion
    char cod_promo_ext[25];	//identificador externo de la promoción
	int id_grupo;			//numero del grupo de eventos al que pertenece este registro
    double cantidad;        //Indica la cantidad en caso que el beneficio lo requiera Ej: 5 articulos o 25 puntos asignados al monedero
    double precio_fijo;     //Indica el valor o precio en caso que el beneficio lo requiera Ej: descuento del 5%
    float porc_desc;	    //Indica el porcentaje en caso que el beneficio lo requiera Ej: descuento de $5
    long cod_arti;			//codigo interno del articulo
    char cod_barr[17];		//codigo de barra del articulo
	long cod_presentacion;	//codigo de presentacion del articulo
    long cod_medio;			//codigo del medio de pago
    long cod_submedio;		//codigo del submedio
    long modo_plan_pago;	//plan de pago
    int nro_validacion;     //Numero de validacion correspondiente al val-caja
	int procesado;			//Indica si fue grabado como evento. Solo para uso de la caja.
	int tipo_bono;			//Indica el tipo de bono otorgado. { 0: Bono común | 1: Bono canjeable. }
};



struct requerir_medios
{
    int cod_grupo;          //Agrupa medios requeridos en una misma promocion
    int condicion_monto;    //Constantes condiciones del monto
    double monto;           //Monto 
    long medio;             //Medio a requerir
    long submedio;          //Submedio a requerir
    long plan;              //Plan a requerir
    int monto_compartido;   //Indica si el monto se comparte entre todos los medios requeridos
	int origen;				//Indica si es un medio requerido por promocion o por cobros
	int eliminado;
	int cod_promo;
};

#endif
#pragma pack (pop) 
