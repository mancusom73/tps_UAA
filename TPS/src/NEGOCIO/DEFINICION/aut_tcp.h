#pragma pack (push,1) 
#ifndef AUT_TCP_H
#define AUT_TCP_H
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV 
 * Definicion de estructuras para comunicacion con el auorizador 
 * de cheques
 * ------------------------------------------------------------------------------
 */

struct _autorizador_envio
{
    char tipo_trx[3];         //3
    char sucursal[3];         //6
    char nro_caja[3];         //9
    char cajero[8];           //17
    char nro_boleta[8];   //25
    char nro_rut_comprador[10]; //35
    char nro_rut_girador[10];   //45
    char nro_banco[4];        //49
    char nro_cuenta[19];      //68
    char nro_cheque[9];   //77
    char nro_plaza[4];        //81
    char tipo_credito[2];     //83
    char monto[11];           //94
    char fecha_vencimiento[8];  //102
};

struct _autorizador_respuesta
{
    char autorizacion[6];     //6
    char codigo_respuesta[4];   //10
    char desc_respuesta[32];    //42
    char cupo_actual[11];     //53
};
#endif
#pragma pack (pop) 
