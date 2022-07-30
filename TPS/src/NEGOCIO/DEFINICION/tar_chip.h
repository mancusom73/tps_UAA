#ifndef TAR_CHIP_H
#define TAR_CHIP_H
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

struct _ptos_venc
{
    long puntos;
    int fecha_venc;
};

struct _status
{
    int rta;
    long balance_monedero;
    long balance_puntos;
    struct _ptos_venc ptos_venc;
    long puntos_acumulados;
    long cupones_electronicos[5];
    long id_novedad;
};

struct _open_channel
{
    int rta;
    int puerto;
    int baudios;
    int paridad;
    int data;
    int stop;
};

struct _rta_driver
{
    int rta;
    char *buf1;
};

struct _datos_fijos
{
    long id_cliente;
    int id_sucursal;
    char sufijo;
    char apellido[21];
    char nombre[21];
    char tipo_documento;
    char numero_doc[12];
    int fecha_nac;
    char sexo;
    char tipo_cliente;
    int fecha_vencimiento;
    char track2[40];
};

struct _parametro
{
    int rta;
    char *buffer;
    int tamano;
};

struct _datos_variables
{
    int rta;
    char calle[31];
    int numero;
    char departamento[6];
    char barrio[21];
    char ciudad[21];
    char provincia[21];
    char telefono[16];
    char trabajo;
    char funciones_habilitadas[3];
};

struct _actualizacion_puntos
{
    int rta;
    int bitmap;
    long mov_puntos;
    struct _ptos_venc ptos_venc;
    long puntos_acum;
    long cupones_electronicos[5];
    long serial[2];
    long id_novedad;
};

struct _ctrl_compras
{
    int fecha;
    int cantidad;
    long puntos;
};

struct _p_monedero
{
    int rta;
    long importe;
    char pin[4];
    char firma[2];
};

struct _serial_number
{
    int rta;
    unsigned long serial1;
    unsigned long serial2;
};

struct _novoffli
{
    BINARY status;
    long id_novedad;
    unsigned fecha_novedad;   //fecha en que graban ellos la novedad
    int id_sucursal;
    long id_cliente;
    char sufijo;
    float mov_cupones[5];
    float mov_puntos;
    float puntos_con_vto;
    unsigned vto_puntos;
    unsigned fecha_incorporado;
    int hora_incorporado;
    int sucursal_incorporado;
    int caja_incorporado;
    long nro_ticket;
    char reserva[19];
};
#endif