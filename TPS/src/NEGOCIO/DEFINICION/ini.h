#pragma pack (push,8) 
#ifndef __INI_H__
#define __INI_H__

#define BTRIEVE   1
#define MYSQL     2
#define SQLSERVER 3
#define ORACLE    4







struct _ConfIni
{
    char javaserverIp[20];
	char FtpIp[20];
    int  clienteDb;
    char odbc[15];
    char user[12];
    char password[12];
    char dirNovedades[50];
    int  touch_screen;
    char logPantallas;
    char logMotorPromo;
    char logVentas;
    char logCobros;
    char logPagos;
    char logFondoFijo;
    char logRetiros;
    char logCierre;
    char logJavaServer;
    char logVarios;
    char logCambioMedio;
	char logOdbc;
	char logConServidor;
	char logComunicaciones;
	char logErrores;
	char logRobot;
	char logDebug;
    int  tiempo_pop_up;
    int  sucursal;
    char dirVideosPromos[50];
	char dirVideosVentas[50];
	char dirVideosSsaver[50];
    char dirRepro[50];
    int vAncho;
    int vAlto;
    int vPosx;
    int vPosy;
    int vAnchoS;
    int vAltoS;
    int vPosxS;
    int vPosyS;
    int vAnchoV;
    int vAltoV;
    int vPosxV;
    int vPosyV;
	char logComprobantes;
	char ActualizPantRedondeo;
    int reintentos_envio_cierre;
    int clienteDbServer;
    char odbcServer[15];
    char userServer[12];
    char passwordServer[12];
    char pathDatabase[255]; 
	double CorteArchGrandes;
	int timerScreenSaver;
	char dirSkin[50];
	int RnvDirectoMemoria;
	int panelesPosX;
    int panelesPosY;
	int AceleracionPantallas;
}ConfIni;

struct _CambioMedio
{
	int MedioEntrante;
	int SubmedioEntrante;
	int MedioSaliente;
	int SubmedioSaliente;
};

struct _config_tps 
{
	int versionxml;
	int activarTFpermanente;
	int encabezadoTF;
	int activarLecturaClienteBarra; 
	char NombreCliente[17];
    char DniCliente[15];
	char Prefijo_scanner_cliente[5];
	int ControlarMediosDePago;
	int AgruparMediosEnImpresion;
	int CantMediosPermitidos;
	int InformarMediosExcedidos;
	int ClienteTarjetaEspecial;
	int Codigo_seguridad_cliente;
	int Perfil_cliente_especial;
	int ActivarCashPlus;
	int ValidacionCambioMediosCash;
	int MinimoaHabilitar;
	int HabilitarCashPlusSolo;
	float MontosValidos[10];
	int CambioMedioAutomatico;
	int PedirAutorizacionCambioMedioAutomatico;
	int ValidacionCambioMedioAutomatico;
	int PedirAutorizacionEnCierreConCambioMedio;
	struct _CambioMedio det_CambioMedio[10]; 
	int PadronesPorFTP;
	char RutaFtp[50];
	int AlicuotasEnArticulo;
	int ModificarAlicuotaDeArticuloConsFinal;
	int NapseModalidad;
}config_tps; //actualizar la estuctura en la clase CConfigTPS

int GET_POSICION_PANTALLAS_X(void);
int GET_POSICION_PANTALLAS_Y(void);
int SET_POSICION_PANTALLAS_X(int x );
int SET_POSICION_PANTALLAS_Y(int y);
void CARGAR_CONF_INI();
void SET_VALORES_DEF();
void SETEAR_CONF_INI( char *grupo, char *variable, char *valor );


#endif
#pragma pack (pop) 
