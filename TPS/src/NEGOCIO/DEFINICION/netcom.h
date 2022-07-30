#ifndef NETCOM_H
#define NETCOM_H
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Definicion de datos para comunicaci�n mediante protocolos de red
 * ------------------------------------------------------------------------------
 */

#define TCP_IP  1
#define IPX     2
#define T_FILE  3

#define ERROR_LOCAL   1
#define NODO_CTA_CTE  9

struct _id_origen
{
    char dispositivo;         // 0-TPV, 1-Validador, 2-Terminal de Devoluciones,
    // 3-Server CtaCte, 4-Aut-on, 5-Server de tarjetas
    char version;
    char sub_version;
    char revision;
    long terminal;
};                                        // 8 bytes

struct _rta_transaccion
{
    char cod_rta;
    char cod_accion;
    long cod_autorizacion;
    double importe_autorizado;
    double importe_cuota;
    double saldo;
    unsigned primer_vto;
    char clave_de_trabajo[8];
    char nro_referencia[12];
    char mensaje[41];         // Se mostrara cada vez que venga
    int cod_perfil;
    char working_key[8];
    char permite_pago_menor;
    char reserva[16];
};                                        // 120 bytes

// Clase
#define Normal   0
#define Reverso  1

// Operacion
#define Compra                       0
#define Anulacion                    1
#define Devolucion                   2
#define Pago                         3
#define Anulacion_de_Pago            4
#define Consulta_de_Saldo            5
#define Consulta_de_Estado           6
#define Cambio                       7
#define Consulta_de_Estado_Cobro     8
#define Consulta_de_Estado_Adelanto  9
#define Adelanto                    10

// Modo
#define OnLine   0
#define OffLine  1

// Moneda
#define Pesos  0
#define Dolar  1
#define Cecor  2

// Cod_rta
#define Aceptada 0
#define Denegada 1

// Cod_accion
#define Ninguna                      0
#define Pedir_autorizacion           1
#define Mostrar_importe_autorizado   2
#define Pedir_autorizacion_si_desea  3
#define PIN_invalido                 4
#define Sin_saldo                    5

struct _paquete
{
    struct _id_origen id;
    struct _transaccion transaccion;
};

struct _paquete_rta
{
    struct _id_origen id;
    struct _rta_transaccion rta;
};

struct _transac_sic
{
    BINARY status;
    long nro;
    char enviado;
    char error;
    char nodo;
    unsigned nro_z;
    struct _id_origen id;
    struct _transaccion transaccion;
    struct _id_origen id_rta;
    struct _rta_transaccion rta;
    char reserva[54];
};                                        // 400

// El ind1 es por nro unicamente

struct _ind2_transac
{
    char nodo;
    char enviado;
    long nro;
};

struct _ind3_transac
{
    unsigned nro_z;
    char nodo;
};
int ABRIR_CANAL( int protocolo, int canal, char *ip, int formato_mensaje );
int CERRAR_CANAL( int protocolo, int canal );
int _ENVIAR_PAQUETE( int protocolo, char *paquete, int l_paquete, char *rta, int l_rta,
                     int time_out_disponible, char espera_rta, int canal );
int AGREGAR_TRANSACCION( struct _transaccion *tran, long nro, int nodo );
void DEBUG_PAQUETE( char *archivo, char *paquete, int l_paquete );
int ENVIAR_TRANSACCION( int nodo, int operacion, double importe, long comprobante, long *nro,
                        long *autorizacion, long anulacion );
void MOSTRAR_IMPORTE_ACEPTADO();
int MOSTRAR_RESULTADO( char *mensaje, int autoriza );
long CALCULA_NRO_TRANSACCION();
void ENVIAR_TRANSACCIONES_PENDIENTES();
int ENVIAR_PAQUETE( int protocolo, int nodo );
int ENVIAR_COMO_OFF_LINE();
int OPERACION_NO_REVERSABLE( int op );
int TIMEOUT_NODO( char nodo );
int AUTORIZAR_TRANSACCION( long *autorizacion, int ok );
void COMIENZO_NET_COM();
int ABRIR_TRANSAC();
int REVERSAR_TRANSACCION( int nodo, long nro );
void FIN_NET_COM();
#endif
