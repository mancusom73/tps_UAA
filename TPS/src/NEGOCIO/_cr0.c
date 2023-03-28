/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <ini.h>
#pragma pack(1)
#include <cr.h>
#include <cierre.h>
#include <cr_disco.h>
#include <cr_disc2.h>
#include <config.h>
#include <stdlib.h>
#undef MK_FP
#ifndef INVEL_L
#include <dos.h>
#endif
#include <math.h>
#include <fcntl.h>
#include <memory.h>
#include <aarch.h>
#include <dbrouter.h>
#include <mstring.h>
#include <path.h>
#include <limits.h>

#if defined(INVEL_W)
#include <stdio.h>
#include <btrv.h>
#include <panel.h>
#include <string.h>
#include <_cr_var.h>
#include <io.h>
//#include <direct.h>
#include <actions.h>
#include <direct.h>
#include <c-tree.h>
#endif

#if defined(INVEL_L)
#include <stdio.h>
#include <btrv.h>
#include <panel.h>
#include <string.h>
#include <_cr_var.h>
#include <actions.h>
#include <c-tree.h>
#define _MAX_PATH 255
#endif
#include <myodbc.h>
#include <motor.h>
#include <deftables.h>
#include <db.h>
#include <pant.h>
#include <ssaver.h>
#include <handles.h>
#include <rnv.h>
#include <driver.h>
#include <dd.h>
#include <stack.h>
#include <cmemory.h>
#include <m_cadena.h>
#include <_cr0.h>
#include <ram.h>
#include <val1.h>
#include <display.h>
#include <archivos.h>
#include <present.h>
#include <balanza.h>
#include <log.h>
#include <lista.h>
#include <promo.h>
#include <tcp_ip.h>
#include <fiscal.h>
#include <clave.h>
#include <cobros_esp.h>
#include <t_pago.h>
#include <netcom2.h>
#include <b_mens.h>
#include <modo_eje.h>
#include <ventas.h>
#include <llave.h>
#include <xyz.h>
#include <pausa.h>
#include <_cr1.h>
#include <tar_mag.h>
#include <med_rein.h>
//#include <cr_displ.h>
#include <apagado.h>
#include <clientes.h>
#include <consulta.h>
#include <ventas2.h>
#include <imp_piez.h>
#include <mensaje.h>
#include <cajon.h>
#include <get_fech.h>
#include <power.h>
#include <g_cierre.h>
#include <inf00.h>
#include <get_hora.h>
#include <cupones.h>
#include <imp_tick.h>
#include <carga.h>
#include <comprob.h>
#include <uti-val.h>
#include <tkt.h>
#include <script.h>
#include <cio.h>
#include <netcom.h>
#include <round.h>
#include <malloc.h>
#include <envios.h>
#include <valida.h>
#include <pedir.h>
#include <cr_var.h>
#include <utic_proto.h>
#include <cr_displ.h>
#include <mstring.h>
#include <stringt.h>
#include <caj_flotante.h>
#include <f_fijo.h>
#include <menu.h>
#include <eventos.h>
extern char _MODO_GRAFICO,_TIPO_PANTALLA;
extern int DATOS_RNV_DIRECTO_MEMORIA;
extern int BLOQUEAR_DATOS;
struct _dd_var_btr {
    BINARY status;      //0
    char clase;      //1
    short int cod_variable;      //2
    char nom_variable[36];
    char tipo_variable;
    short int elementos;      //43
	long valor;
    char valor_inicial[75];      //45
    char nivel_seg_consulta;      //124
    char nivel_seg_modif;      //125
    unsigned short tabla_valores;
    unsigned short longitud;
    long offset;      //130
    char define[33];
    char reserva[25];      //166
}dd_var_btrieve;
static int primera_entrada_cajero,cajero_elegido;
char _RNV_VIRTUAL = NO, _MOSTRAR_BMP_INICIAL = NO;
char _CONTROLAR_RNV = NO;
char *_PTR_MNV;
char ancho_pantalla = 80;
char ancho_panel = 0;
char con_request = SI;
char cajero_operando = 0;
char corte_habilitado = 1;
char numero_serie[40];
char reenviar_eventos = NO;
char drivevirtual = 'D';



/*
   core_iniciado
   Variable Utilizada como semaforo para sincronoizar los procesos 
   Pasos de sincronizacion:
         1) Arranca el Core hasta cargar los diccionarios y btrieve
         2) El core se queda en espera y arrancan los procesos de pantallas
         3) El core continua sincronizado con los demas procesos
*/
int core_iniciado = 0;
int init_core = 0;
int init_archivos = 0;
#if defined(INVEL_W) || defined(INVEL_L)
char buffer[_MAX_PATH];
int PantallaCorbaInit();
void InitTable();
void startEventData();
void resetJAVAAplication();
int CorbaInitMisc();
char glog_actions;
char mensaje [50];
char ip[30];
#endif
//char ver[30];
int ACELERACION_PANTALLAS = 0; //para pc lentas y preferentemente  micro de 1 core ej celeron
extern int GUARDAR_ESTADO_RAM();
extern int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos, tipo_t_ev_cierre;
extern struct _fecha_controlador fecha_cf;
extern struct _hora_controlador hora_cf;
extern void CARGAR_SERIAL_IMPRESORA(void);
extern unsigned PIDE_FECHA_VTO(void);
extern void MENSAJE_ID_CON_PAUSA_CAJA( int id );
extern int DIF_DIAS( unsigned fecha1, unsigned fecha2 );
int  VERIFICA_DIFERENCIA_DIA_OPERATIVO_FECHA_ACTUAL( void );
int DICCIO_CARGADO;
#if defined(INVEL_W) || defined(INVEL_L)
void Presentacion( char *version, char *fecha, char *branch );
#endif
/***************************************************************************/
#if defined(INVEL_W) || defined(INVEL_L)
int CR0( char *version, char *branch, int _argc, char **_argv )
#else
int CR0( char *version )
#endif
/***************************************************************************/
{
	int offline = 0;
    int h;
    int salir = 0, cajero;
    int demo = 0, set_caja = 0, mem = 0, rta_printer = 0;
    char aux[81];
    int n_caja = 1;
    int initScanner = 0;
	int       es_supervisor;
	int fondo_fijo_aut = 0;
    //  char      nom_caja[10];
    #if defined(INVEL_W) || defined(INVEL_L)
    int error = 0; 
    #endif
    char pathCaja[200];
	int rta = 0, /*pant,*/ continua = 1, tit = 0;
	int copiar_dd_var=0;
    #if defined(INVEL_W) || defined(INVEL_L)
    memset( pathCaja, 0, sizeof( pathCaja ) );
	memset( aux, 0, sizeof( aux ) );
    glog_actions = 'S';
	tit=1;
    #else     
    INIT_MODO_PROTEGIDO();
    #endif  
    /*----------- Verifico parametros del programa ( Pasada 1 )-------------*/
    #ifdef INVEL_L
    #define STRCMP_CASE strcasecmp
    #define STRNCMP_CASE strncasecmp
    #else 
    #define STRCMP_CASE _stricmp
    #define STRNCMP_CASE _strnicmp
    #endif
    DICCIO_CARGADO = NO;
    CARGAR_CONF_INI();
	DATOS_RNV_DIRECTO_MEMORIA = ConfIni.RnvDirectoMemoria;
	ACELERACION_PANTALLAS = ConfIni.AceleracionPantallas;
	SetConfig( CONF_DIR_SKIN, 0, ConfIni.dirSkin );
	init_core = 1;
	while( init_core == 1 ) // espera para iniciar
    {
        SLEEP(10);
    }
    for( h = 1;h < _argc;h++ ) {
        if( STRCMP_CASE( _argv[h], "version" ) == 0 ) {
            #ifdef COMPILAR_SEGURO
            printf( "\n\n%s  [ V ]\n\n", version );
            #else
            printf( "\n\n%s\n\n", version );
            #endif
            return 0;
        }
        /*if( strcmp( _argv[h], "cga" ) == 0 ) {
            _TIPO_PANTALLA = 2;
        }
        else if( strcmp( _argv[h], "herc" ) == 0 ) {
            _TIPO_PANTALLA = 1;
        }
        else if( strcmp( _argv[h], "g" ) == 0 ) {
            _MODO_GRAFICO = 1;
        }*/
        else if( strcmp( _argv[h], "m" ) == 0 ) {
            mem = 1;
        }
        else if( strcmp( _argv[h], "demo" ) == 0 ) {
            demo = 1;
        }
        else if( strcmp( _argv[h], "rnv_virtual" ) == 0 ) {
            _RNV_VIRTUAL = SI;
        }
        else if( strcmp( _argv[h], "printer_off" ) == 0 ) {
            PRINTER_OFF();
        }
        else if( strncmp( _argv[h], "setcaja:", 8 ) == 0 ) {
            sscanf( &_argv[h][8], "%d", &n_caja );
            set_caja = 1;
        }
        else if( strncmp( _argv[h], "serie:", 6 ) == 0 ) {
            sprintf( numero_serie, "            " );
            memcpy( numero_serie, &_argv[h][6], strlen( &_argv[h][6] ) );
            printf( "%s ", numero_serie );
        }
        else if( strncmp( _argv[h], "seried:", 7 ) == 0 ) {
            sprintf( numero_serie, "            " );
            memcpy( numero_serie + 12 - strlen( &_argv[h][7] ), &_argv[h][7],
                    strlen( &_argv[h][7] ) );
            printf( "%s", numero_serie );
        }
        else if( strncmp( _argv[h], "bmp:", 4 ) == 0 ) {
            _MOSTRAR_BMP_INICIAL = SI;
            strcpy( aux, &_argv[h][4] );
        }
        else if( strncmp( _argv[h], "sin_request", 11 ) == 0 ) {
            con_request = NO;
        }
        else if( strncmp( _argv[h], "eventos", 7 ) == 0 ) {
            reenviar_eventos = SI;
        }
        else if( strncmp( _argv[h], "path=", 5 ) == 0 ) {
            sscanf( &_argv[h][5], "%s", pathCaja );
            set_caja = 1;
        }
        else if( STRCMP_CASE( _argv[h], "diccio" ) == 0 ) {
            printf( "\n\n%s\n\n", VERSION_DICCIONARIO );
            return 0;
        }
        else if( strncmp( _argv[h], "drive_virtual:", 14 ) == 0 ) {
            drivevirtual = _argv[h][14];
        }
        else if( STRNCMP_CASE( _argv[h], "no_glog_actions", 12 ) == 0 ) {
            glog_actions = 'N';
        }
		else if( strncmp( _argv[h], "copiar_dd_var", 13 ) == 0 ) {
            copiar_dd_var = 1;
        }
        /*else if( STRNCMP_CASE( _argv[h], "printer_server=", 15 ) == 0 ) {
            sscanf( &_argv[h][15], "%s", printerServer );
        }
        else if( STRNCMP_CASE( _argv[h], "touch_screen", 12 ) == 0 ) {
            _TOUCH_SCREEN = 1;            
        }*/
    }
    /*#ifdef INVEL_W
    if( strlen( printerServer ) == 0 ) //si no se configuro el ip de la impresora se toma por defecto la local
    {
        sprintf( printerServer, "%s", "localhost" );
    }
    #endif*/

    #if defined(INVEL_W) || defined(INVEL_L)
    _RNV_VIRTUAL = 1;
    InitTable();
    #endif
    #ifdef INVEL_W
    if( pathCaja[0] ) {
        _chdir( pathCaja );
    }
    _getcwd( buffer, _MAX_PATH );   
    #endif
    #ifdef INVEL_L
    error = Ctree_Init();
    if( error ) {
        return error;
    }
    #endif
    
    CLRSCR( 7 );
   
    /*---------------------------- Muestra memoria ---------------------------*/
    if( mem ) {
        MOSTRAR_MEMORIA_DISPONIBLE();
    }   
    /*-------------------------- Cargar configuracion ------------------------*/
    AJUSTA_CANTIDAD_HANDLES( 100 );
    if( demo || _RNV_VIRTUAL ) {
        _RNV_VIRTUAL = SI;
        OPEN_RNV_VIRTUAL();
    }
    MOSTRAR_CADENA( 1, 1, "Inicializando sistema ..." );
    INIT_DRIVERS_PRIMARIOS();
    MOSTRAR_CADENA( 1, 1, "Configurando memoria ... " );
    BTRIEVE_RESET();
	
	while(initOdbc(ConfIni.odbc,ConfIni.user,ConfIni.password,ODBC_LOCAL)!= 0 );
	verificar_tablas = NO;
	CONTROLAR_TABLAS_OLD(); //Controla si quedo alguna tabla old, significa que hubo un corte en medio de una actualizacion
	VERIFICAR_SCRIPT();
	/*Rutina para copiar dd_var de tabla btrieve a tabla mysql borrar cuando se implemente el nuevo conftpv*/
	if(copiar_dd_var)
	{
		DIRECT_DB_QUERY("DELETE FROM DD_VAR");
		USE_DATABASE( AREA_AUX7, DD_VAR_SIC, ( char* )&dd_var_btrieve, sizeof( dd_var_btrieve ), NULL, 0, 0 );
		OPEN_TABLE(DD_VAR, TT_ORIG ,(char*)&dd_var,sizeof( struct _dd_var ));
		GO(TOP);
		while(!BtrvEOF()){
			memcpy(&dd_var,&dd_var_btrieve,sizeof( dd_var));
			INSERT2(NO);
			SKIP(1);
		}
		CLOSE_DATABASE( AREA_AUX7 );
	}
    DD_INIT();
	DICCIO_CARGADO = SI;
	verificar_tablas = SI;
    STACK_INIT();
    SetConfig( CONF_TIEMPO_POP_UP, ConfIni.tiempo_pop_up, NULL );
    SetConfig( CONF_JAVA_SERVER, 0, ConfIni.javaserverIp );
    SetConfig( CONF_IP_FTP, 0,ConfIni.FtpIp );
	SetConfig( CONF_DIR_VIDEO_VENTAS, 0,ConfIni.dirVideosVentas );
    /*Como ya se inicializo la rnv podemos sacar valores de configuracion*/
    SetConfig( CONF_MODO_NEGOCIO, MODO_NEGOCIO, NULL ); // carga la configuracion de rnv al objeto conf de pantallas
	SetConfig_TPS(  );//cargo el conftps.xml
        /*Leo la variable en C (_TOUCH_SCREEN) y seteo la variable en C++ */
    _TOUCH_SCREEN = ConfIni.touch_screen;
    SetConfig( CONF_TOUCH_SCREEN, (int)_TOUCH_SCREEN, NULL );
    
    SET_VIDEO_DEFAULTS( ConfIni.dirRepro, ConfIni.dirVideosPromos, ConfIni.dirVideosVentas, ConfIni.dirVideosSsaver, ConfIni.vAncho, 
		ConfIni.vAlto, ConfIni.vPosx, ConfIni.vPosy, ConfIni.vAnchoV, ConfIni.vAltoV, ConfIni.vPosxV, ConfIni.vPosyV );

//(Nota Unific MySQL-Linux): Se comenta ya que funciona en Linux
    //#ifdef INVEL_W   
	core_iniciado = 1;         //Inicia los procesos de pantallas
    while( core_iniciado != 2 ) // espera la inicializacion de las pantallas
    {
        SLEEP(1);
    }
    //#endif
    /* Controla si hay script para ejecutar */
  
	if( GetConfig( CONF_TOUCH_SCREEN ) ){
        EJECUTAR_SCRIPT_TOUCH();
	}
    MOSTRAR_CADENA( 1, 1, "Inicializando hardware ..." );

	ALOJA_STRING_TABLE();
    ABRIR_STRING_TABLE();
    GRABAR_LOG_SISTEMA_STRING( S_INIT_DRIVERS_SECUNDARIOS ,LOG_VENTAS,1);
    INIT_DRIVERS_SECUNDARIOS();
    GRABAR_LOG_SISTEMA_STRING( S_DRIVERS_SECUNDARIOS_INICIADOS ,LOG_VENTAS,1);
    _snprintf( aux, sizeof(aux)-1, "%s", DIRECCION_IP_SERVIDOR );
    PING_ABRIR_SOCKET( aux );
    if( USAR_DYNAKEY ) {
        ancho_panel = 10;
    }
    else {
        ancho_panel = 0;
    }

    #if defined(INVEL_W) || defined(INVEL_L)
    {
        char aux[50];
        int l;

		memset( aux, 0, sizeof( aux ) );
		_snprintf( aux, sizeof(aux)-1,"%s", NRO_NOM_CAJA );

        l = NRO_CAJA;
        //printf("%d",l);
    }
    //  startEventData(  );
    //  CorbaInitBrowse(  );
    //  CorbaInitMenuGen(  );
    #endif
	_snprintf( aux, sizeof(aux)-1,"%s", NOM_CANAL_TCP_IP );

    /*----------------------- Inicializa Sistema -----------------------------*/
    /*---------------- inicializa punteros a RAM no volatil ---------------*/
	SET_RAM( ON );
	/* if( 1 ) {
		 int cajax = 8;
		 char nom_caja[5];
		 memset(nom_caja,0,sizeof(nom_caja));
        SET_MEMORY_INT( __nro_caja, cajax );
        sprintf( nom_caja, "%03d", cajax );
        SET_MEMORY( __nro_nom_caja, nom_caja );
        SET_MEMORY_LONG( __nro_x, 1L );
        SET_MEMORY_LONG( __nro_y, 1L );
        SET_MEMORY_LONG( __nro_z, 1L );
    }*/
    _CONTROLAR_RNV = SI;
    // si glog_actions == 'S' guardo el estado de la ram no volatil en el archivo RNV.log
    // esto es util para debugguear.
    if( glog_actions == 'S' ) {
        GUARDAR_ESTADO_RAM();
    }
    VALIDAR_RAM();
    /*----------------------- Inicializa Sistema -----------------------------*/
    if( HARD_OMRON ) {
        #ifdef COMPILAR_OMRON
        if( !OMRON_INIT() ) {
            ABORT_STRING( S_DRIVER_OMRON_NO_PRESENTE );
        }
        #endif
    }
    switch( HARD_INVEL ) {
        case VERSION_1:
            #ifdef COMPILAR_PINPAD3
            INICIALIZAR_PINPAD();
            #endif
            break;
        case VERSION_2:
            VAL_SET_PUERTO( 1 );
            VAL_INICIALIZAR_PUERTO();
            break;
    }
    #ifdef COMPILAR_DISPLAY
    if( HARD_OMRON ) {
        DISPLAY_STRING( S_PUNTO_DE_VENTA_OMRON_RS_7000 );
    }
    else if( HARD_TEC == 1 ) {
        DISPLAY_STRING( S_PUNTO_DE_VENTA_TEC_RE_3500 );
    }
    else if( HARD_TEC == 2 ) {
        DISPLAY_STRING( S_PUNTO_DE_VENTA_TEC_ST_5000 );
    }
    else {
        DISPLAY_STRING( S_PUNTO_DE_VENTA_TPV_1000_IV );
    }
    #endif
    if( MODO_DEBUG > 3 ) {
        GRABAR_LOG_SISTEMA_STRING( S_CHEQUEANDO_BTRIEVE ,LOG_DEBUG,2);
    }
  /*  if( !BTRIEVE_PRESENTE() ) {
        CR0_EXIT( 1 );
    }*/
    if( MODO_DEBUG > 3 ) {
        GRABAR_LOG_SISTEMA_STRING( S_BTRIEVE_CHEQUEADO ,LOG_DEBUG,2);
    }
    if( MODO_DEBUG > 3 ) {
        GRABAR_LOG_SISTEMA_STRING( S_INICIANDO_ARCHIVOS ,LOG_DEBUG,2);
    }
    if( !INIT_ARCHIVOS() ) {
        ABORT( "ERROR AL CARGAR LA CONFIGURACION DE ARCHIVOS" );
    }
    if( MODO_DEBUG > 3 ) {
        GRABAR_LOG_SISTEMA_STRING( S_ARCHIVOS_INICIADOS,LOG_DEBUG, 2 );
    }
    AbrirArchivosPanel();
	#ifdef FECHA_DEMO
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
   	{ // FECHA_FINALIZACION_DE_VERSION
		unsigned fecha = FECHA( FECHA_FINALIZACION_DIA, FECHA_FINALIZACION_MES, FECHA_FINALIZACION_ANIO );

		sprintf( aux, "VERSION DE PRUEBA VENCE %02d/%02d/%04d", FECHA_FINALIZACION_DIA, FECHA_FINALIZACION_MES, FECHA_FINALIZACION_ANIO );
		GRABAR_LOG_SISTEMA( aux, LOG_DEBUG,2 );

		if( fecha < _GET_FECHA_DOS(  ) ){
			GRABAR_LOG_SISTEMA( "LA VERSION HA CADUCADO" , LOG_DEBUG,2 );
			ABORT( "LA VERSION HA CADUCADO" );
		}else{
			memset(aux,0,sizeof(aux));
			sprintf(aux," __ V.Demo(%d/%d/%d)",FECHA_FINALIZACION_DIA, FECHA_FINALIZACION_MES,FECHA_FINALIZACION_ANIO );
			strcat(version,aux);
		}
		
	}

//ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
#endif

    #ifdef COMPILAR_DYNAKEY
    HidePanelArea();
    #endif
	Presentacion( version, " ", branch );
    if( MODO_DEBUG > 3 ) {
        GRABAR_LOG_SISTEMA_STRING( S_INICIANDO_BALANZA,LOG_DEBUG,2 );
    }
    #ifdef COMPILAR_BALANZA
    INIT_BALANZA();
    #endif
	EXISTE_PRINTER(  );
    if( MODO_DEBUG > 3 ) {
        GRABAR_LOG_SISTEMA_STRING( S_BALANZA_INICIADA ,LOG_DEBUG,2);
    }
    if( !IS_PRINTER() && IMPRESORA_FISCAL != FISCAL_IBM_4610 ) {
        MENSAJE_STRING( S_IMPRESORA_FUERA_LINEA );
    }
    SPOOLER_OFF();
    /*------------------ Graba LOG -------------------------*/
	_snprintf( aux, 81, "ENCENDIDO TPV (Versin %s) (Caja %i)", version, NRO_CAJA );
    aux[80] = '\0';
    GRABAR_LOG_SISTEMA( aux ,LOG_VENTAS,1);
	if(	strlen( branch ) > 0 ) {
		char aux1[80];

		memset( aux1, 0, sizeof( aux1 ) );
		_snprintf( aux1, 80, "BRANCH TPV (Versin %s)", branch );
        aux1[79] = '\0';
		GRABAR_LOG_SISTEMA( aux1 ,LOG_VENTAS,1);
	}
    //GRABAR_LOG_MEMORIA();
	ExecuteAction( A_MUESTRA_MEMORIA ); //agregado
    _LARGO_TICKET = DD_ELEMENTOS( __ticket_tipo );
    /*------------------------- Inicializa Archivos --------------------------*/
    if( MODO_DEBUG > 3 ) {
        GRABAR_LOG_SISTEMA_STRING( S_INICIANDO_ARCHIVOS_CR0,LOG_DEBUG,2 );
    }
	/* Si hubo error al borrar eventos al cierre */
	if( VARIOS_VACIAR_EVENTOS_AL_INICIO ) {
		VACIAR_ARCHIVOS_EVENTOS( TT_ORIG );
		SET_MEMORY_CHAR( __varios_vaciar_eventos_al_inicio, NO );
	}
	while( !init_archivos ) {
		//Sincronizamos el hilo con la caja
        SLEEP(1);
    }
    CR0_INIT_ARCHIVOS();


    if( MODO_DEBUG > 3 ) {
        GRABAR_LOG_SISTEMA_STRING( S_ARCHIVOS_CR0_INICIADOS ,LOG_DEBUG,2);
    }
    if( MODO_DEBUG > 3 ) {
        GRABAR_LOG_SISTEMA_STRING( S_INICIANDO_ARCHIVOS,LOG_DEBUG,2 );
    }
    if( INICIALIZA_ARCHIVOS( SI ) < 0 ) {
        ABORT_STRING( S_ERROR_CARGAR_ARCHIVOS );
    }
    if( MODO_DEBUG > 3 ) {
        GRABAR_LOG_SISTEMA_STRING( S_ARCHIVOS_INICIADOS ,LOG_DEBUG,2);
    }
    if( MODO_DEBUG > 3 ) {
        GRABAR_LOG_SISTEMA_STRING( S_INICIANDO_TARJETAS ,LOG_DEBUG,2);
    }
    INICIALIZAR_TARJETAS();
    INICIALIZAR_BUSCA_TARJETA();
    if( MODO_DEBUG > 3 ) {
        GRABAR_LOG_SISTEMA_STRING( S_TARJETAS_INICIADAS ,LOG_DEBUG,2);
    }    
	/*-------------------- Controla dia operativo ----------------------*/
    if( TOMAR_DIA_OPERATIVO_AL_INICIO ) {
        CR0_TOMAR_DIA_OPERATIVO( NO);
    }
	if( ACELERACION_PANTALLAS == 1)
		glog("Usando Aceleracion Pantallas", LOG_INCONDICIONAL,1 );
    /*------ Carga tabla de promociones en memoria ------*/
    //CARGAR_PROMO_MEMORIA();
    if( HABILITAR_SCANNER_AL_INICIO ) {
        SET_MEMORY_CHAR( __varios_scanner_habilitado, 1 );
    }
    /*------ Configuraciones para Selectos ------*/
    if( config.pais == ELSALVADOR ) {
        if( !UTILIZA_PERFILES ) {
            ABORT( "CHEQUEAR CONFIGURACION DE PERFILES" );
        }
        //el perfil para Selectos es un descuento para clientes. ï¿½te se carga junto con la
        //elecciï¿½ del cliente y no de un men como era tradicionalmente. Hay que asegurarse
        //de que el campo clientes.cod_perfil tenga un dato.
    }
    /*Controla que la cantidad de ticket en espera local no sea mayor a 5*/
    if( MAX_CANTIDAD_TICKET_EN_ESPERA > 5 && MODALIDAD_TICKET_EN_ESPERA == 0 ) {
        SET_MEMORY_CHAR( __max_cantidad_ticket_en_espera, 0 );
        MENSAJE_STRING( S_CANTIDAD_DE_TICKET_EN_ESPERA_ERROR );
    }
    /*Controla que la cantidad de ticket en espera remota no sea mayor a 99*/
    if( MAX_CANTIDAD_TICKET_EN_ESPERA > 99 && MODALIDAD_TICKET_EN_ESPERA == 1 ) {
        SET_MEMORY_CHAR( __max_cantidad_ticket_en_espera, 0 );
        MENSAJE_STRING( S_CANTIDAD_DE_TICKET_EN_ESPERA_ERROR );
    }
    MOSTRAR_DIA_OPERATIVO();
    TESTEAR_CIERRE_FALLIDO();
    /*---------------------------- Muestra memoria ---------------------------*/
    if( mem ) {
        MOSTRAR_MEMORIA_DISPONIBLE();
    }
    CR0_CIERRE( 1 );

	if( IMPRESORA_FISCAL == FISCAL ) {
		char cadena[3];

		memset( cadena, 0, sizeof( cadena ) );
		_snprintf( cadena, sizeof( cadena ), "%s", ConfIni.javaserverIp );
		cadena[ sizeof( cadena ) - 1] = '\0';

		_strupr( cadena );//conviertir a mayuscula
		if( strcmp( cadena, "NO" ) == 0 ) {
			int msj = 0;
			while( !msj ) {
				MENSAJE( "ERROR DE CONFIG. EN JAVASERVER, VERIFICAR TPVIV.INI O CONFTPV..." );
			}
		}
		 CARGAR_SERIAL_IMPRESORA();
		//FISCAL_VERIFICA_ESTADO();
		rta_printer = COMANDO_LEER_STATUS_PRINTER_FISCAL( 1 , NULL );
		//En caso de corte de luz, se consulta si el doc esta abierto al recuperar su estado
		if( RAM_P_TICKET && DOCUMENTO_ABIERTO( ) ) {
			NO_INC_NRO = 1;
		} else {
			NO_INC_NRO = 0;
		}
		COMANDO_LEER_CONTADORES_PRINTER_FISCAL( 1, SI, NULL );
    }
    if( VARIOS_MODO_Y_EN_Z ) {
        CEREAR_STRUCT( _x );
    }
//PINPAD_MOSTRAR( ST( S_CONECTANDO___ ) );    
#ifdef COMPILAR_PINPAD
    if( UTILIZA_PINPAD ) {
		if( PINPAD_VERIFICAR_ONLINE( ) == 0)
			//pinpad offline
		     MENSAJE("PINPAD OFFLINE");
		PINPAD_VERSION();
		PINPAD_CANCEL_SESSION();
        PINPAD_CARTEL();
    }                  
    #endif  
    //if( !RAM_P_TICKET ) {
    //PINPAD_PEDIR_WORKING_KEY(  );
    //}
    /*----------------------Verificacion de configuracion --------------------*/
    if( MODO_NEGOCIO == SERVICIOS ) {
        while( VERICAR_CONF_SERVICIOS() ) {
            PAUSA( 25 );
        }
    }
    /*--------------------------- Lazo principal -----------------------------*/
    if( MODO_DEBUG > 3 ) {
        GRABAR_LOG_SISTEMA_STRING( S_ENTRANDO_LAZO_PRINCIPAL,LOG_DEBUG,2 );
    }
    if( !RAM_ID_EVENTO ) {
        ACTUALIZA_ID_EVENTO_NEW();
    }
    if( !NRO_NRO_RETIRO ) {
        SET_MEMORY_INT( __nro_nro_retiro, 1 );
    }
	GRABAR_LOG_SISTEMA_CONFIG( "Abre Tarjetas",LOG_VARIOS,5);
    ABRIR_CANAL_TARJETAS();
    while( !salir ) {
        BORRAR_MENSAJE();
        #ifdef COMPILAR_MODO_ENTRENAMIENTO
        MOSTRAR_MODO_ENTRENAMIENTO_EN_PANTALLA( 0 );
        #endif
		GRABAR_LOG_SISTEMA_CONFIG( "CERRAR_ARCHIVOS_EVENTOS_ALL",LOG_VARIOS,5);
        CERRAR_ARCHIVOS_EVENTOS_ALL();
        CERRAR_VTA();
		GRABAR_LOG_SISTEMA_CONFIG( "DISABLE_STANDBY_AUTOMATICO",LOG_VARIOS,5);
		DISABLE_STANDBY_AUTOMATICO(  );
        forzar_online = 1;
		GRABAR_LOG_SISTEMA_CONFIG( "CR0_INGRESA_CAJERO",LOG_VARIOS,5);
        //Controla si el scanner esta habilitado para deshabilitar
        /*if( initScanner = VARIOS_SCANNER_HABILITADO ) {
            CR1_HABILITA_SCANNER( NO );
        }*/
		INIT_SCREEN_SAVER();
		MOSTRAR_CAJERO_EN_PANTALLA();
		CARTEL( version, branch );
		cajero = CR0_INGRESA_CAJERO( &es_supervisor, &fondo_fijo_aut );
        //regresa al estado anterior del scanner, en caso de haber estado habilitado
        /*if(initScanner){
            CR1_HABILITA_SCANNER( NO );
        }*/
		forzar_online = 0;
		GRABAR_LOG_SISTEMA_CONFIG( "CR0_INGRESA_CAJERO ingresado",LOG_VARIOS,4);
        if(IMPRIMIR_TICKET_AL_FINAL){
            setData(ACTION_SET_MODO_IMPRESION_FINAL, NULL, 0);
        }else{
            setData(ACTION_SET_MODO_IMPRESION_NORMAL, NULL, 0);
        }
		ENABLE_STANDBY_AUTOMATICO(  );
        memcpy( &datos_cajero_actual, &datos_cajero, sizeof( datos_cajero_actual ) );
        ABRIR_ARCHIVOS_EVENTOS_ALL();
        ABRIR_VTA();
		/*Control de la cantidad de medios y submedios en fondo fijo automatico*/
		fondo_fijo_aut = ( FONDO_FIJO_AUTOMATICO ) ? fondo_fijo_aut: 1;

		if( cajero > 0 && fondo_fijo_aut ) {
            if( !TOMAR_DIA_OPERATIVO_AL_INICIO ) {
                CR0_TOMAR_DIA_OPERATIVO(NO);
            }            
            /*----------------------------- Cajero -----------------------------*/
            //if( !ES_SUPERVISOR( cajero ) && ESPERAR_LLAVE( 'R' ) ) { 
			if( es_supervisor == -1 ) 
				es_supervisor = ES_SUPERVISOR( cajero );
                if( !es_supervisor )
                    es_supervisor = ES_GERENTE( cajero );
				if( !es_supervisor ) {
					if( ESPERAR_LLAVE( 'R' ) ) {
                        if( CR0_GRABAR_ENTRADA_CAJERO() ) {                    
                            int vuelve_a_cr1 = 1;

                            RECUPERO_DE_CHEQUES_CARGADOS();

                            if( USAR_NUMERO_DE_CAJERO_COMO_VENDE ) {
                                SET_MEMORY_LONG( __nro_vendedor_long, NRO_CAJERO );
                            }

                            GRABAR_XYZ( 'X' );
                            SET_MEMORY_CHAR( __nro_modo, 'X' );
                            GRABAR_XYZ( 'Y' );
                            #ifdef COMPILAR_DISPLAY
                            CR1_DISPLAY_CAJA_ABIERTA();
                            #endif
							
							
							_snprintf(mensaje, sizeof(mensaje)-1, "Debe realizar un ARQUEO - presione un tecla");

							if(CAJEROS_FLOTANTES && !ABRIR_CANAL_TCP_IP( _CANAL_DBROUTER, ip, _FORMATO_MENSAJE_INVEL )) 
							{
								MENSAJE_STRING( S_POS_FUERA_DE_LINEA );
								SLEEP(5);
								MENSAJE_TECLA( mensaje );
								bandera_offline = 1;
							}

							
							if( PIDE_SUPERVISOR_CAJERO_FLOTANTE && offline) 
							{
								do
								{
									if (INGRESAR_CLAVE( 30 - ancho_panel / 2, 21, ST( S_SUPERVISOR___ ), 2, 0,( tit ? ST( S_CAJERO_FLOTANTE_OFFLINE ): NULL ), _F_SUP_CAJERO_FLOTANTE_OFFLINE, NO, SI ) ) 
									{
										GRABAR_LOG_SISTEMA("SE AUTORIZA LOGIN SIN RED",LOG_VENTAS,1);
										rta=1;
									}
									else
									{
										MENSAJE_STRING( S_LOGIN_NO_AUTORIZADO );
										rta=0;
									}
								}
								while(rta!=1);
							}
							
							if(CAJEROS_FLOTANTES && !bandera_offline )
								{
								if(IMPORTAR_DATOS_CAJA_ANTERIOR(NRO_CAJERO))
									GRABAR_LOG_SISTEMA("SE IMPORTARON LOS DATOS DEL CAJERO",LOG_VENTAS,3);
								}
							else
								{
								//SELECT( AREA_CIERRE );//Me fijo si es el primer cierre del cajero para poner la cantidad de cierres Y en cero
								SELECT_TABLE( T_EV_CIERRE, tipo_t_ev_cierre );
								SET_WHERE("");
								RUN_QUERY(NO);
								
								/*if( !RECCOUNT() ) */
								if( !RECCOUNT2( NO, NULL ) ) {
									GRABAR_LOG_SISTEMA("PRIMER INGRESO DEL CAJERO",LOG_VENTAS,3);		
									cant_cierres_y = 0;
									}
								}

								cajero_operando = 1;
                            ENABLE_STANDBY_AUTOMATICO();

                            while( vuelve_a_cr1 ) {
                                vuelve_a_cr1 = 0;
                                if( CR1() ) {
                                    SET_APAGADO( OFF );
                                    //#ifdef COMPILAR_DISPLAY
                                    //CR1_DISPLAY_CAJA_CERRADA();
                                    //#endif
                                    if( !CR0_CIERRE( 0 ) ) {
                                        SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
                                        SET_MEMORY_LONG( __ram_nro_cliente, 0 );
                                        GRABAR_LOG_SISTEMA_CONFIG( "CEREAR_PERFIL_CLIENTE()",LOG_VARIOS,5);
                                        CEREAR_PERFIL_CLIENTE();
                                        SET_MEMORY_INT( __ram_cod_funcion, 0 );
                                        vuelve_a_cr1 = 1;
                                        SET_APAGADO( ON );
                                        //                              continue;
                                    }
                                }
                                else {
							        //CR1_DISPLAY_CAJA_CERRADA();
                                    CR0_GRABAR_X( VARIOS_CAJON_ACTIVO );
                                    SET_MEMORY_INT( __ram_fin_ticket, 0 );
                                    SET_MEMORY_INT( __nro_cajero, 0 );
                                }

                                if( !vuelve_a_cr1 ) {
                                    ExecuteAction( A_DES_CIERRE_ACT_PRESENT );
                                    //VER: No es necesario crear el panel presentacion nuevamente,
                                    //solo desactivar ventas u otro panel visible.
                                    MOSTRAR_CAJERO_EN_PANTALLA();
									CARTEL( version, branch );
                                    //ACTUAL_FECHA_PRESS();
                                    cajero_operando = 0;
							        #ifdef COMPILAR_DISPLAY
								        CR1_DISPLAY_CAJA_CERRADA();
                                    #endif
                                    DISABLE_STANDBY_AUTOMATICO();
                                    //esto está mal porq se envia el evento en el medio de un 8 y un 10
                                    //if( MODO_NEGOCIO == SERVICIOS ) {                                
                                    //    MREINTEG_EN_CIERRE( );
                                    //}                                    
                                }
                                else {
                                    //if( MODO_NEGOCIO == SERVICIOS ) {                                
                                        SET_MEMORY_INT( __ram_fin_ticket, 0 );
                                        ExecuteAction( A_DES_CIERRE_ACT_VENTAS );
                                        BORRAR_MENSAJE();
                                    //}
                                }
                            }
						} else {
							MENSAJE_STRING( S_NO_PUDO_GRABAR_ENTRADA_CAJERO );
						}
					}
				} else {
                /*---------------------------- Supervisor --------------------------*/
                if( ESPERAR_LLAVE( 'Z' ) ) {
                    if( CR0_MODO_Z( 1 ) ) {
                        salir = 1;
                    }
                }
            }
        }
    }
    /*-------------------------------- Final ---------------------------------*/
    CR0_FINAL( version );
	GRABAR_LOG_SISTEMA_CONFIG( "Finalizando CR0",LOG_VARIOS,4);
    return 0;
}
/*****************************************************************************/
void CR0_TOMAR_DIA_OPERATIVO( int onfly )
/*****************************************************************************/
{
    char nuevo_dia_operativo,aux2[30],aux[80];
	int       seguir = 1, seguir2 = 1;
    unsigned dia;
    //CARGAR_CONFIG(  );
    if( NRO_Z_EMITIDA ) {
        SET_MEMORY_CHAR( __ram_imprimir_cabecera, ( char )1 );
    }
     nuevo_dia_operativo = 0;
	
	 if( UTILIZAR_DIA_OPERATIVO && (NRO_Z_EMITIDA || onfly == SI)) {
		//config.dia_operativo = 15687;//
		{
			char mensa[85];
			char diaoperativo[20];
			char diaoperativo2[20];
			glog("*******DIA OPERATIVO ACTUAL*****",LOG_VENTAS,2);
			DTOC( NRO_DIA_OPERATIVO, diaoperativo );
			DTOC( config.dia_operativo, diaoperativo2 );
			sprintf(mensa,"DIA OPERATIVO RNV %s config.dia_operativo  %s",diaoperativo,diaoperativo2 );
			glog(mensa,LOG_VENTAS,2);
			memset( mensa, 0, sizeof( mensa ) );
			memset( diaoperativo, 0, sizeof( diaoperativo ) );
			DTOC( NRO_PROXIMO_DIA_OPERATIVO_SERVIDOR, diaoperativo );
			sprintf(mensa,"Nro Prox OPERATIVOServidor %s",diaoperativo);
			glog(mensa,LOG_VENTAS,2);

		}
		if( config.dia_operativo > NRO_DIA_OPERATIVO ) {//NRO_DIA_OPERATIVO = 08/10/10
            if( config.dia_operativo < NRO_PROXIMO_DIA_OPERATIVO_SERVIDOR ) {
                dia = NRO_PROXIMO_DIA_OPERATIVO_SERVIDOR;
				memset( aux, 0, sizeof( aux ) );
                memset( aux2, 0, sizeof( aux2 ) );
                DTOC( dia, aux2 );
                _snprintf( aux, sizeof(aux)-1, "tomando el dia operativo del servidor: %s ", aux2 );
                GRABAR_LOG_SISTEMA( aux ,LOG_VENTAS,2);
            }
            else {
				char diaoperativo4[20];
				DTOC( config.dia_operativo, diaoperativo4 );
				_snprintf( aux, sizeof(aux)-1, "tomo config.dia_operativo: %s ", diaoperativo4 );
                dia = config.dia_operativo;
				GRABAR_LOG_SISTEMA( aux ,LOG_VENTAS,2);
            }
            if( dia > NRO_DIA_OPERATIVO ) {
                SET_MEMORY( __nro_dia_operativo, ( char* )&( dia ) );
                nuevo_dia_operativo = 1;
            }
		}
        else {
            if( config.proximo_dia_operativo < NRO_PROXIMO_DIA_OPERATIVO_SERVIDOR ) {
                dia = NRO_PROXIMO_DIA_OPERATIVO_SERVIDOR;
                memset( aux, 0, sizeof( aux ) );
                memset( aux2, 0, sizeof( aux2 ) );
                DTOC( dia, aux2 );
                _snprintf( aux, sizeof(aux)-1, "tomando el dia operativo del servidor: %s (prox)", aux2 );
                GRABAR_LOG_SISTEMA( aux ,LOG_VENTAS,2);
            }
            else {
					char diaoperativo3[20];
					DTOC( config.proximo_dia_operativo, diaoperativo3 );
					_snprintf( aux, sizeof(aux)-1, "tomo config.proximo_dia_operativor: %s ", diaoperativo3 );
					dia = config.proximo_dia_operativo;
					GRABAR_LOG_SISTEMA( aux ,LOG_VENTAS,2);
            }
            if( dia > NRO_DIA_OPERATIVO ) {
                SET_MEMORY( __nro_dia_operativo, ( char* )&( dia ) );
                nuevo_dia_operativo = 2;
            } else {
				if( PROPONER_DIA_OPERATIVO_AL_VENCID && onfly == NO) {
					while( seguir ) {
						if( SOLICITAR_CLAVES( 26 - ancho_panel, 22, NO, SI, SI,
								"CAMBIAR DIA OPERATIVO", NULL, RAM_COD_FUNCION, SI ) ) {
							while( seguir2 ) {
								dia = PIDE_NUEVO_DIA_OPERATIVO(  );
								if( dia ) {
									SET_MEMORY( __nro_dia_operativo, ( char * ) &( dia ) );
									nuevo_dia_operativo = 1;
									memset( aux, 0, sizeof( aux ) );
									memset( aux2, 0, sizeof( aux2 ) );
									DTOC( dia, aux2 );
									_snprintf( aux, sizeof( aux )-1, "tomando el dia operativo ingresado: %s", aux2 );
									GRABAR_LOG_SISTEMA( aux, LOG_DEBUG,2 );
									seguir = 0;
									seguir2 = 0;
								}
							}
						} else
							BORRAR_MENSAJE(  );
					}
				} else
					ABORT_STRING( S_DIA_OPERATIVO_VENCIDO );
			}
        }
		if( onfly == NO)
			SET_MEMORY_CHAR( __nro_z_emitida, ( char )0 );
	 } else {
		 glog("*******NO SE ACTUALIZA DIA OPERATIVO*****",LOG_VENTAS,2);
	 }
    /*----------- Graba en SISTEMA.LOG el nuevo dia operativo ----------*/
    if( nuevo_dia_operativo ) {
		memset( aux, 0, sizeof( aux ) );
		memset( aux2, 0, sizeof( aux2 ) );
        _snprintf( aux, sizeof(aux)-1, "***ACTUALIZADO Nuevo dia operativo (%1i) = ", nuevo_dia_operativo );
        DTOC( NRO_DIA_OPERATIVO, aux2 );
        strcat( aux, aux2 );
        GRABAR_LOG_SISTEMA( aux ,LOG_VENTAS,2);
	} else {
		memset( aux, 0, sizeof( aux ) );
		memset( aux2, 0, sizeof( aux2 ) );
		DTOC( NRO_DIA_OPERATIVO, aux2 );
        _snprintf( aux, sizeof(aux)-1, "***NO MODIFICADO DIA operativo (%s)",aux2);
        GRABAR_LOG_SISTEMA( aux ,LOG_VENTAS,2);
    }
}
/*****************************************************************************/
void CR0_EXCEPCION_PEDIR_CAJERO( int *tecla )
/*****************************************************************************/
{
    int attr;
    DISABLE_VERIFICAR_CAMBIO_ARHIVOS();
    switch( *tecla ) {
        case '%':
            //---- Consulta por Codigo de Scanner
            attr = _ATRIBUTO;
            CR1_CONSULTA( 1 );
            _ATRIBUTO = attr;
            break;
        case 140:
            //---- Mostrar Cajones habilitados
            cajero_elegido = CR0_MOSTRAR_CAJONES_HABILITADOS( SI );
            if( cajero_elegido ) {
                *tecla = 27;
            }
            break;
    }
    ENABLE_VERIFICAR_CAMBIO_ARHIVOS();
    _PERMITIR_BANDA = SI;
}
/*****************************************************************************/
long CR0_PEDIR_CAJERO( int x, int y, int l, int *tipo_ingreso )
/*****************************************************************************/
{
    long usuario = -3;
    int teclas[] = {'%',140,0};
	char *barra_usuario=NULL;
	GRABAR_LOG_SISTEMA_CONFIG( "ENABLE_VERIFICAR_CAMBIO_ARHIVOS",LOG_VARIOS,5);
    ENABLE_VERIFICAR_CAMBIO_ARHIVOS();
	GRABAR_LOG_SISTEMA_CONFIG( "CR0_EXCEPCION_PEDIR_CAJERO",LOG_VARIOS,5);
    SET_TECLA_EXCEPCION( teclas, CR0_EXCEPCION_PEDIR_CAJERO );
    _PERMITIR_BANDA = SI;
	GRABAR_LOG_SISTEMA_CONFIG( "LIMPIA_BUFFER_TARJETA",LOG_VARIOS,5);
    LIMPIA_BUFFER_TARJETA( NO );
	GRABAR_LOG_SISTEMA_CONFIG( "sale LIMPIA_BUFFER_TARJETA",LOG_VARIOS,5);
	
	//original usuario = PEDIR_ENTERO(x, y, l, SI);
	usuario = PEDIR_ENTERO_COD_BARRA( x, y, l, 0, &barra_usuario, SI, NO, "" );
	_PERMITIR_BANDA = NO;
	_PERMITIR_SCANNER = NO;
    /*--------- El supervisor elige el cajero que desea ingresar ------*/
    if( MULTIPLES_USUARIOS_PARA_VENTA && cajero_elegido ) {
        usuario = cajero_elegido;
        cajero_elegido = 0;
    }
    else {
        if( usuario == -2 ) {
            usuario = LEER_NUMERO_USUARIO(l);
            *tipo_ingreso = IDENTIFICACION_BANDA;
        }
        else {
            if( usuario == -3 ||(usuario == 0 && barra_usuario!=NULL && strlen(barra_usuario)>0)){
				usuario= LEER_NUMERO_USUARIO_GENERICO( l,barra_usuario );
				*tipo_ingreso = IDENTIFICACION_BARRA;
			} else {
				*tipo_ingreso = IDENTIFICACION_MANUAL;
			}
        }
    }
    SET_TECLA_EXCEPCION( NULL, NULL);
    //DISABLE_VERIFICAR_CAMBIO_ARHIVOS();
    return ( usuario );
}
/*****************************************************************************/
int CR0_INGRESA_CAJERO( int *es_supervisor, int *fondo_fijo_aut )
/*****************************************************************************/
{
    char aux[30], select[60];
    int nro = 1, l_nro, cajero = -1, tit = 0, necesita_fondo_fijo = NO;
	long usuario=0;
    char strUser[255];
    int tipo_ingreso = IDENTIFICACION_MANUAL, fondo_fijo_auto_online = 1;
    struct _estado_cajeros ctrl_cajero;
    char     *mensajes[] = { "Clave: ", "Clave: ", "Supervisor: ", "", "", "Clave: ", "Supercajero: " };
	char modo_identificacion_supervisor = MODO_IDENTIFICACION_SUPERVISOR;
	char modo_identificacion_gerente = MODO_IDENTIFICACION_GERENTE;
	int ingreso_incorrecto = NO;
	char titulo[30];


    #ifdef INVEL_L
    #define CTRL_CAJ "ctrl_caj.ctr"
    #else
    #define CTRL_CAJ "ctrl_caj.sic"
    #endif

    #if defined(INVEL_W) || defined(INVEL_L)
    //ExecuteAction( A_DES_CIERRE_ACT_PRESENT );
    tit = 1;
    #endif
	memset( titulo, 0, sizeof( titulo ) );
    _ATRIBUTO = 7;
    if( USAR_LEGAJO == RUT ) {
        //l_nro = 9;
		l_nro = 8;
    }
    else {
        l_nro = ( USAR_LEGAJO == LEGAJO ) ? CANTIDAD_DE_DIGITOS_EN_NRO_LEGAJ : 4;
    }
    ExecuteAction( A_DES_CIERRE_ACT_PRESENT );
    GRABAR_LOG_SISTEMA_CONFIG( "CR0_PEDIR_CAJERO",LOG_VARIOS,4);
    usuario = CR0_PEDIR_CAJERO( 41 - ancho_panel / 2 - l_nro / 2, 20, l_nro, &tipo_ingreso );
    _snprintf( strUser, sizeof(strUser)-1, "%ld.gif", usuario );
    ExecuteSAction( A_MOSTRAR_FOTO_PRES, strUser );
		
	if( usuario > 0 && VERIFICA_DIFERENCIA_DIA_OPERATIVO_FECHA_ACTUAL( )== 0)
		usuario=0;

    if( usuario > 0 ) {
        if( USAR_LEGAJO == RUT ) {
            cajero = BUSCA_CAJERO_X_DNI( usuario );
        }
        else if( USAR_LEGAJO == LEGAJO ) {
            cajero = BUSCA_CAJERO_X_LEGAJO( usuario );
        }
        else {
            cajero = BUSCA_CAJERO_X_CODIGO( usuario );
        }
    }
    if( cajero > 0 ) {
        if( cajero >= config.primer_supervisor ) {
            nro = 2;
        }
		if( IDENTIFICACION_USUARIO == ID_X_TIPO ) {
            if( LEE_CLAVE_CAJERO( cajero, NULL ) == 1 ) {
                nro = datos_cajero.tipo;
            }
            else {
                cajero = 0;
            }
			if (cajero && datos_cajero.tipo == CAJERO) {
				if (MODO_IDENTIFICACION_USUARIOS == IDENTIFICACION_MANUAL
					&& ( tipo_ingreso == IDENTIFICACION_BANDA 
						|| tipo_ingreso == IDENTIFICACION_BARRA ) ) {
					cajero = -1;
				}
				if (MODO_IDENTIFICACION_USUARIOS == IDENTIFICACION_BANDA
					&& ( tipo_ingreso == IDENTIFICACION_MANUAL 
						|| tipo_ingreso == IDENTIFICACION_BARRA ) ) {
					cajero = -2;
				}
				if( MODO_IDENTIFICACION_USUARIOS == IDENTIFICACION_BARRA						 
					&& ( tipo_ingreso == IDENTIFICACION_BANDA  
						|| tipo_ingreso == IDENTIFICACION_MANUAL ) ) {
					cajero = -3;
				}
			}

			if( cajero && datos_cajero.tipo == SUPERVISOR ) {
				if( MODO_IDENTIFICACION_SUPERVISOR == IDENTIFICACION_MANUAL
					&& ( tipo_ingreso == IDENTIFICACION_BANDA 
				     || tipo_ingreso == IDENTIFICACION_BARRA ) ) {
                    cajero = -1;
                }
                if( MODO_IDENTIFICACION_SUPERVISOR == IDENTIFICACION_BANDA
					&& ( tipo_ingreso == IDENTIFICACION_MANUAL 
						|| tipo_ingreso == IDENTIFICACION_BARRA ) ) {
                    cajero = -2;
                }
				if( MODO_IDENTIFICACION_SUPERVISOR == IDENTIFICACION_BARRA						 
					&& ( tipo_ingreso == IDENTIFICACION_BANDA 
						|| tipo_ingreso == IDENTIFICACION_MANUAL ) ) {
					cajero = -3;
				}
            }

			if( cajero && datos_cajero.tipo == GERENTE ) {
				if( MODO_IDENTIFICACION_GERENTE == IDENTIFICACION_MANUAL
					&& ( tipo_ingreso == IDENTIFICACION_BANDA  
						|| tipo_ingreso == IDENTIFICACION_BARRA ) ) {
					cajero = -1;
				}
				if( MODO_IDENTIFICACION_GERENTE == IDENTIFICACION_BANDA
					&& ( tipo_ingreso == IDENTIFICACION_MANUAL 
						|| tipo_ingreso == IDENTIFICACION_BARRA ) ) {
					cajero = -2;		
				}
				if( MODO_IDENTIFICACION_GERENTE == IDENTIFICACION_BARRA 
					&& ( tipo_ingreso == IDENTIFICACION_BANDA  
						|| tipo_ingreso == IDENTIFICACION_MANUAL ) ) {
					cajero = -3;
				}
			}
			if( cajero < 0 ) {
				switch( cajero ) {
				case -1: strcpy( titulo, "INGRESE CODIGO MANUAL" );
					break;
				case -2: strcpy( titulo, "INGRESE CODIGO POR BANDA" );
					break;
				case -3: strcpy( titulo, "INGRESE CODIGO DE BARRA" );
					break;
				}
				ingreso_incorrecto = SI;
				cajero = 0;			
			}

            if( !cajero ) {
                MENSAJE_STRING( S_MEDIO_DE_INGRESO_INVALIDO );
            }
            #ifdef COMPILAR_MODO_ENTRENAMIENTO
            if( cajero && ( datos_cajero.tipo == SUPERVISOR || datos_cajero.tipo == GERENTE ) 
                        && RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
                MENSAJE_STRING( S_SALGA_PRIMERO_DE_MODO_ENTRENAMIENTO );
                cajero = 0;
				nro = -1;
            }
            #endif
		}
		if( cajero > 0 ) {
			if( SINCRONIZAR_HORA_CONTROLADOR_FIS  ) {
   				if( IMPRESORA_FISCAL == FISCAL  ) {
   					ACTUALIZA_FECHA_Y_HORA_CONTROLADOR_FISCAL_EPSON(  );
   				}
   			}
		}
		if( datos_cajero.tipo == SUPERCAJERO ) {
            nro = MENU_CAJERO_SUPERVISOR();
            switch( nro ) {
               case 1:
                  *es_supervisor = NO;
                  break;
               case 2:
                  *es_supervisor = SI;
                  break;
            }
         } else {
            *es_supervisor = -1;
         }
		 if( nro != -1 ) {
			if( !INGRESAR_CLAVE( 34 - ancho_panel / 2, 21, mensajes[nro], nro, cajero,
				( tit ? ( ( strlen( titulo ) > 2 )? titulo: mensajes[nro] ) : NULL ), _F_SUP_MODO_Z, SI, SI ) ) {
                cajero = -1;
			} else {
				if( NRO_HUBO_OPERACIONES && cajero != NRO_CAJERO_ANT
						||  ( NRO_HUBO_OPERACIONES && datos_cajero.tipo == SUPERCAJERO
							&& cajero == NRO_CAJERO_ANT && *es_supervisor == SI )
					&&  !MULTIPLES_USUARIOS_PARA_VENTA ) {
					MENSAJE_STRING( S_CAMBIO_CAJERO );
					cajero = -1;
				}
			}

			if( cajero == 0 && ingreso_incorrecto ) {
				//Seteamos el cajero que ingreso
				cajero = USUARIO_CLAVE_ANTERIOR();
			}

         } else {
            cajero = 0;
        }
		if( cajero > 0 ) {
			if( cajero > 0 && MULTIPLES_USUARIOS_PARA_VENTA ) {
                if( !ES_SUPERVISOR( cajero ) ) {
                    if( NRO_CAJERO && cajero != NRO_CAJERO ) {
                        MENSAJE_STRING( S_CAMBIO_CAJERO );
                        cajero = -1;
                    }
                    else {
                        cajero = CR0_VERIFICAR_CAJERO_HABILITADO( cajero );
                    }
                }
                else {
                    if( NRO_CAJERO && NRO_CAJERO != cajero ) {
                        MENSAJE_STRING( S_CAMBIO_CAJERO );
                        cajero = -1;
                    }
                }
            }
			/*Se controla que la cantidad de medios incluyendo submendios en d_f_fijo.sic no supere los 10*/
			if( FONDO_FIJO_AUTOMATICO ) {
				CONTROL_CANT_MEDIOS_SUBMEDIOS_FONDO_F_AUTO( cajero, fondo_fijo_aut );
			} else {
				*fondo_fijo_aut = 1;
			}
			if( cajero > 0 && ( ( CONTROLAR_ENTRADA_DE_CAJEROS && !ES_SUPERVISOR( cajero ) )
               || FONDO_FIJO_AUTOMATICO ) && *fondo_fijo_aut ) {
				_snprintf( select, sizeof(select)-1, " cajero = %d", cajero );
				GRABAR_LOG_SISTEMA_CONFIG( "DBROUTER_OBTENER_TABLA CTRL_CAJ",LOG_VARIOS,4);
				if( !OBTENER_TABLA_SERVIDOR( T_ESTADO_CAJEROS, TT_ORIG, "estado_cajeros", select, SI, NULL ) ) {
                    if( !OPEN_TABLE( T_ESTADO_CAJEROS, TT_ORIG, ( char* )&ctrl_cajero,
                                 sizeof( struct _estado_cajeros ) ) ) {
						int salir = NO;
						SET_WHERE("");
						RUN_QUERY(NO);  
						necesita_fondo_fijo = SI;

						while( !salir && !dbEOF() ) {
							if( ctrl_cajero.nro_cajero == cajero ) {
								if( ctrl_cajero.fecha == GET_FECHA_DOS() ) {
									necesita_fondo_fijo = NO;
									if( ctrl_cajero.nro_caja != NRO_CAJA 
											&& (int)ctrl_cajero.estado_cajero == 1 ) {
										MENSAJE_STRING( S_EL_CAJERO_YA_ESTA_HABILITADO_EN_OTRA_CAJA );
										if( !SOLICITAR_CLAVES( 30, 20, NO, SI, SI,
															ST( S_AUTORIZACION_DE_CAJERO ), NULL,
															_F_SUP_CAJERO_DUPLICADO, SI ) ) {
											cajero = 0;
											salir = 1;
										}
										else {
											LEE_CLAVE_CAJERO( cajero, NULL );
											salir = 1;
										}
									}
								}
								else {
									MENSAJE_STRING( S_CAJERO_CON_ENTRADAS_ANTERIORES );
								}
							}
							SELECT_TABLE( T_ESTADO_CAJEROS, TT_ORIG); //la vuelvo a seleccionar porque cuando pide supervisor pierde la tabla
							SKIP2( 1 );
						}
                        CLOSE_TABLE( T_ESTADO_CAJEROS, TT_ORIG );
                    }
                    else {
                        MENSAJE_STRING( S_ERROR_DE_APERTURA_NO_SE_PUEDE_CONTROLAR );
						fondo_fijo_auto_online = 0;
                    }
                }
                else {
                    MENSAJE_STRING( S_ERROR_OBTENIENDO_TABLA_NO_SE_PUEDE_CONTROLAR );
					fondo_fijo_auto_online = 0;
                }
            }
			if( cajero > 0 && *fondo_fijo_aut ) {
                primera_entrada_cajero = ( NRO_CAJERO ) ? 0 : 1;
                SET_MEMORY( __nro_cajero, ( char* )&cajero );
                SET_MEMORY( __nro_legajo_cajero, ( char* )&datos_cajero.nro_legajo );
                if( USAR_LEGAJO == RUT ) {
                    SET_MEMORY( __ram_doc_cajero, datos_cajero.documento );
                }
                if( ES_SUPERVISOR( cajero ) ) {
                    SET_MEMORY( __nro_supervisor_ant, ( char* )&cajero );
                }
                else {
                    SET_MEMORY( __nro_cajero_ant, ( char* )&cajero );
                }
                /*---------------------- graba LOG ---------------------------*/
                _snprintf( aux, sizeof(aux)-1, "Entro usuario %04i", cajero );
                GRABAR_LOG_SISTEMA( aux ,LOG_VENTAS,1);
				/*Fondo Fijo Automatico*/
				if( FONDO_FIJO_AUTOMATICO && necesita_fondo_fijo &&
            			!ES_SUPERVISOR( NRO_CAJERO ) 
						&& !( datos_cajero.tipo == SUPERCAJERO && (*es_supervisor == SI ) ) 
						&& fondo_fijo_auto_online ) {
					if( LEE_CAJERO( NRO_CAJERO, &datos_cajero_actual ) == 1 ) {
						/* El msj string no acepta el acento en la "A" */
						MENSAJE_CON_PAUSA( " FONDO FIJO AUTOMÁTICO...", 50 );
						CR1_FONDO_FIJO_MULT_MEDIOS( SI );
					}
				} else {
					if( FONDO_FIJO_AUTOMATICO && !necesita_fondo_fijo && !fondo_fijo_auto_online
							&& !ES_SUPERVISOR( NRO_CAJERO )
                            && !ES_GERENTE( NRO_CAJERO )
							&& !(datos_cajero.tipo == SUPERCAJERO && (*es_supervisor == SI ) ) ) {
						 /* El msj string no acepta el acento en la "A" */
						MENSAJE_CON_PAUSA( " NO SE PUEDE REALIZAR EL FONDO FIJO AUTOMÁTICO, CAJA OFFLINE ", 100 );
					}
				}
            }
        }
    }
    if( cajero <= 0 && datos_cajero.tipo != SUPERVISOR) {
        MENSAJE_STRING( S_CAJERO_INVALIDO );
		MOSTRAR_MENSAJE_UBUNTU( 0,ST(S_CAJERO_INVALIDO), 2 );
		ExecuteSAction( A_MOSTRAR_FOTO_PRES, "fotodefault.gif" );
    }
    return ( cajero );
}
/*****************************************************************************/
int ES_SUPERVISOR( int cajero )
/*****************************************************************************/
{
    int rta = NO;

	rta = _ES_SUPERVISOR( cajero, &datos_cajero );

	return ( rta );

}

/*****************************************************************************/
int _ES_SUPERVISOR( long cajero, struct _cajeros *datos_cajero )
/*****************************************************************************/
{
	int       rta = NO;

	if( IDENTIFICACION_USUARIO == ID_X_TIPO ) {
		switch( datos_cajero->tipo ) {
			case SUPERVISOR:
				rta = SI;
				break;
			case SUPERCAJERO:
				if( cajero != NRO_CAJERO ) {
					rta = SI;
				}
				break;
		}
	} else {
		rta = ( cajero < config.primer_supervisor ) ? NO : SI;
	}

	return ( rta );
}
/*****************************************************************************/
void CR0_FINAL( char *version )
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    int finish = 0;
    #endif
    CLOSE_PRINTER();
    CIERRA_ARCHIVOS();
    FIN_BUSCA_TARJETA();
    //textattr(7);
    //  CARTEL( version );
    CLOSE_ALL();
    CR0_LIMPIAR_RAMPAC();
    GRABAR_LOG_SISTEMA( "Apagar TPV" ,LOG_VENTAS,1);
    if( NRO_Z_EMITIDA ) {
        VACIAR_LOG();
    }

    SLEEP( 3000 );
	SET_MEMORY_CHAR( __varios_cambio_fecha_y_hora_cf, 1 );
    //MENSAJE( "YA SE PUEDE APAGAR LA TERMINAL. GRACIAS...." );
	MENSAJE_STRING( S_YA_SE_PUEDE_APAGAR_LA_TERMINAL_GRACIAS);
    SLEEP(180000); // 3 minutos
    //PAUSA( 36 );
	if( REINICIALIZAR_AL_TERMINAR ) {
		ExecuteAction( A_REBOOT_SYSTEM );
    }else{// si no, apaga la maquina
        ExecuteAction( A_SHUTDOWN_SYSTEM );
    }
    CR0_EXIT( 99 );
}
/*****************************************************************************/
void CR0_EXIT( int status )
/*****************************************************************************/
{
    SET_APAGADO( ON );
    _MODO_GRAFICO = 0;
    if( SALIR_AL_DOS ) {
        CIERRA_TODOS_LOS_ARCHIVOS();
        CURSOR_ON();
        LIBERA_BUFFERS_TEMPORALES();
        //exit( status );
    }
    else if( REINICIALIZAR_AL_TERMINAR ) {
        CIERRA_TODOS_LOS_ARCHIVOS();
        CURSOR_ON();
        LIBERA_BUFFERS_TEMPORALES();
        //BOOT();
    }
    else {
        CIERRA_TODOS_LOS_ARCHIVOS();
        //exit(0);
        /*while( 1 );*/
    }
    if( status == 2 ) { //abort
		//Cuando no hay conexion con Mysql no se puede Consultar el stringt a la tabla 
        //MENSAJE( ST( S_DEBE_BAJAR_LA_APLICACION ) );		
        MENSAJE( "DEBE BAJAR LA APLICACION" );
		
#if defined(INVEL_W)
		ALERTAS_SIN_CARGAR_CONF("Debe Bajar la Aplicacion", "   ");
#endif
		while( 1 );
		
    }
}
/*****************************************************************************/
int CR0_MODO_Z( int con_menu )
/*****************************************************************************/
{
    int rta = 0, ok = 1;
    if( DESHABILITA_CORTE_PAPEL_EN_Z ) {
        corte_habilitado = 0;
    }
    SET_APAGADO( OFF );
    MOSTRAR_CADENA( 34 - ancho_panel / 2, 6, ST( S_M_O_D_O___Z ) );
    /*---------------- verifica la fecha de la ultima Z emitida --------------*/
    if( NRO_FECHA_Z == ( unsigned )GET_FECHA_DOS() && !UTILIZAR_DIA_OPERATIVO ) {
        if( con_menu ) {
            ok = Z_DUPLICADA();
        }
        else {
            ok = 0;
            rta = -1;
        }
    }
    if( UTILIZAR_DIA_OPERATIVO && NRO_DIA_OPERATIVO <= NRO_DIA_OPERATIVO_ULTIMA_Z ) {
        if( con_menu ) {
            ok = Z_DUPLICADA();
        }
        else {
            char texto[128];
            _snprintf( texto, sizeof(texto)-1, "Cartel de Z duplicada Remota Dia Op: %d Ult Z: %d", NRO_DIA_OPERATIVO,
                     NRO_DIA_OPERATIVO_ULTIMA_Z );
            GRABAR_LOG_SISTEMA( texto ,LOG_VENTAS,1);
            _ATRIBUTO = ( char )( 15 + 128 );
            MOSTRAR_GRAN_CADENA( 12 - ancho_panel, 18, ST( S_YA_EMITIO_UNA_Z_HOY ) );
            _ATRIBUTO = 7;
            ok = 0;
            rta = -1;
        }
    }
    /*-------------------------- Men de informes Z --------------------------*/
    if( ok ) {
        GRABAR_LOG_SUPERVISOR( "Modo Z", USUARIO_CLAVE_ANTERIOR(),LOG_VENTAS,1 );
        ok = MENU_INFORMES_Z( con_menu );
        SET_MEMORY_CHAR( __nro_modo, 'Z' );
        if( ok == 1 || ok == -2 ) {
            CR0_GENERAR_Z();
            rta = 1;
        }
        else if( ok == 2 ) {
            if( con_menu ) {
                MENSAJE_CON_TECLA( ST( S_DEBE_HACER_EL_CIERRE_DE_CAJA_ESC ), 27 );
            }
            rta = 0;
        }
        else if( ok == 3 || ok == 0 ) {
            rta = 0;
			GRABA_EVENTO_SALIDA_CAJERO(  );//salida del supervisor
        }
        else if( ok == 4 ) {
            if( con_menu ) {
                MENSAJE_CON_TECLA( ST( S_EL_SERVER_NO_SE_ENCUENTRA_ON_LINE_ESC ), 27 );
            }
            rta = 0;
        }
        else {
            rta = 1;
        }
    }
    SET_MEMORY_CHAR( __nro_modo, 'X' );
    SET_MEMORY_INT( __nro_cajero, 0 );
    SET_APAGADO( ON );
    corte_habilitado = 1;
    CLOSE_COPIA_Z_PRN();
    return ( rta );
}
/*****************************************************************************/
int Z_DUPLICADA()
/*****************************************************************************/
{
    int ok = 1, salir = 0, k;
    GRABAR_LOG_SISTEMA( "Cartel de Z duplicada",LOG_VENTAS,1 );
    MOSTRAR_GRAN_CADENA( 12 - ancho_panel, 18, ST( S_YA_EMITIO_UNA_Z_HOY ) );
    MENSAJE_STRING( S_Presione_9_para_CONTINUAR_o_ESC_para_SALIR );
    while( !salir ) {
        k = GETCH();
        if( k == '9' ) {
            salir = 1;
            ok = 1;
        }
        if( k == 27 ) {
            salir = 1;
            ok = 0;
        }
    }
    /*-------------------------- Confirmacin --------------------------*/
    if( ok ) {
        salir = 0;
        //MENSAJE_STRING( S_CONFIRMA_LA_EMISION_DE_LA_Z );
		MENSAJE("CONFIRMA LA EMISION DE LA Z? [1]-SIGUE [0]-CANCELA");
        TECLADO_ALFABETICO = 1;
        while( !salir ) {
            k = GETCH();
            if( ( k == '1' ) /*|| ( k == 's' ) */) {
                salir = 1;
                ok = 1;
                GRABAR_LOG_SISTEMA( "Confirma duplicar Z" ,LOG_VENTAS,1);
            }
            if( ( k == 27 ) || ( k == '0' ) /*|| ( k == 'n' )*/ ) {
                salir = 1;
                ok = 0;
            }
        }
        TECLADO_ALFABETICO = 0;
    }
    BORRAR_MENSAJE();
    return ( ok );
}
#ifdef COMPILAR_ASC
/*****************************************************************************/
CR0_GRABAR_ARCHIVOS_SPAR()
/*****************************************************************************/
{
    GENERA_TOTALES_ASC();
    LOG_EXISTE_ARCHIVO_COD( _TOTALES_ASC );
}
#endif
/*****************************************************************************/
int CR0_c_b( void )
/*****************************************************************************/
{
    if( BREAK ) {
        END_POWER_DOWN();
        CLOSE_ALL();
        ABRIR_STRING_TABLE();
        CERRAR_CANAL_DBR();
        CLOSE_RNV_VIRTUAL();
        PING_CLOSE_SOCKET();
        LIBERA_BUFFERS_TEMPORALES();
        return ( 0 );
    }
    else {
        return ( 1 );
    }
}
/*****************************************************************************/
int CR0_CIERRE( int recupero )
/*****************************************************************************/
{
    int rta = 0;
    if( !recupero ) {
        SET_MEMORY_INT( __nro_recupero_z, 1 );
    }
    if( NRO_RECUPERO_Z == 1 ) {
        if( MODO_NEGOCIO == SERVICIOS ) {   
            //envio todos los cheques pendientes antes de grabar un cierre x, tipo evento 8
            MREINTEG_EN_CIERRE( );
        }
        GRABAR_LOG_SISTEMA( "Modo Y" ,LOG_VENTAS,1);
        SET_MEMORY_CHAR( __nro_modo, 'Y' );
        ADD_MEMORY_LONG( __nro_y, 1L );
        if( CIERRE() ) {
            if( GRABAR_ARCHIVO_DE_CIERRE_Y ) {
                GRABAR_CIERRE();
            }
            if( MULTIPLES_USUARIOS_PARA_VENTA ) {
                _SET_MEMORY_INT( __varios_usuarios_habilitados, VARIOS_CAJON_ACTIVO - 1, 0 );
                SET_MEMORY_CHAR( __varios_cajon_activo, 1 );
            }
			VALIDA_CIERRE_CAJERO( VALIDAR_CIERRE_CAJERO );
            SET_MEMORY_CHAR( __nro_hubo_operaciones, 0 );
            SET_MEMORY_CHAR( __nro_modo, 'X' );
            SET_MEMORY_LONG( __nro_legajo_cajero, 0 );
            if( USAR_LEGAJO == RUT ) {
                SET_MEMORY( __ram_doc_cajero, "          " );
            }
            if( config.pais == ELSALVADOR ) {
                SET_MEMORY_INT( __nro_nro_retiro, 1 );
            }
            GRABA_EVENTO_SALIDA_CAJERO();
            SETEAR_TIEMPO( _INDEFINIDO );
            SET_MEMORY_INT( __nro_cajero, 0 );
            rta = 1;
            SET_MEMORY_INT( __nro_recupero_z, 2 );
            VACIAR_CIERRE_DE_LOTE( );
        }
        else {
            GRABAR_LOG_SISTEMA( "Modo Y ABORTADO - Retornando a X",LOG_VENTAS,1 );
            SET_MEMORY_CHAR( __nro_modo, 'X' );
            ADD_MEMORY_LONG( __nro_y, -1L );
            rta = 0;
            SET_MEMORY_INT( __nro_recupero_z, 0 );
        }
        //SET_MEMORY_INT( __nro_recupero_z, 2 );
    }
    if( NRO_RECUPERO_Z == 2 ) {
        SET_RAM( ON );
        SETEAR_TIEMPO( _INDEFINIDO );
        SET_MEMORY_INT( __ram_tmpo_ocio_aux, RAM_TMPO_OCIO );
        ACUMULAR_TIEMPOS( );
        ACUMULAR_X_EN_Z();
        GRABAR_XYZ( 'Y' );
        ACTUALIZA_VENTAS_XYZ( 'Y' );
        SET_MEMORY_INT( __nro_recupero_z, 3 );
    }
    if( NRO_RECUPERO_Z == 3 ) {
        RESETEA_ACUMULADORES_X();
        ACTUALIZAR_X();  // Andres
        GRABAR_XYZ( 'X' );
        CLOSE_RNV_VIRTUAL();
        OPEN_RNV_VIRTUAL();
        CERRAR_CAJON();
        SET_APAGADO( ON );
        SET_MEMORY_INT( __nro_recupero_z, 0 );
    }
    return rta;
}
#ifdef COMPILAR_PINPAD3
/*****************************************************************************/
void INICIALIZAR_PINPAD()
/*****************************************************************************/
{
    SET_PUERTO( 1 );
    INICIALIZAR_PUERTO();
    STATUS();
}
#endif
/*****************************************************************************/
void CR0_GENERAR_Z()
/*****************************************************************************/
{
    //char msg[198];
    if( MANTENER_FECHA_Z ) {
        _UTILIZAR_FECHA_OPERACION = 1;
    }
    if( IMPRIMIR_PIE_Z ) {
        IMPRIME_PIE_Z();
    }
    GRABAR_LOG_SISTEMA( "Emision de Z" ,LOG_VENTAS,1);
    _ATRIBUTO = ( char )( 15 + 128 );
    MOSTRAR_GRAN_CADENA( 15 - ancho_panel / 2, 15, "N O   A P A G U E" );
    MENSAJE_SIN_SONIDO_STRING( S_ESPERE_UN_MOMENTO_PARA_APAGAR_LA_TERMINAL );
    ACTUALIZA_VENTAS2_ANTERIORES( SI );
    MENSAJE_SIN_SONIDO_STRING( S_ESPERE_UN_MOMENTO_PARA_APAGAR_LA_TERMINAL );
    ACTUALIZA_VENTAS_XYZ( 'Z' );
    GRABAR_XYZ( 'Z' );
    GRABAR_EVENTO_YZ( 'Z' );
	ACTUALIZAR_FECHA_EVENTOS( );
	//Informo a JSincro que puede continuar con su actividad seteando en
    //cero el tiempo
	//DIRECT_DB_QUERY("UPDATE script_pos SET PARAM = %ld where CODCOMANDO = %i",
		//0, SPOS_PAUSA_JSINCRO);

	//Verificar que procese el evento antes de enviarlos a historicos
	{
		int reintento = 0;

		while( reintento < 100 && !EVENTO_PROCESADO( RAM_ID_EVENTO - 1 ) ) {
			PAUSA( 10 );
			reintento ++;
		}
	}

    if( ( config.pais == CHILE || config.pais == BRASIL ) && IMPRESORA_FISCAL == FISCAL ) {
        IMPRIMIR_REPORTE_TRANSACCIONES();
    }
    //long id_cierre;
    //id_cierre = GRABAR_CIERRE();
    GRABAR_BACKUP_XYZ( 'Z' );
	COPIA_EVENTOS_A_PENDIENTES_ANTERIORES();      
	/*Si se habilita la funcion tendria que ser aqui. Antes del vaciado de eventos, cierre de eventos e incremento de Z*/
	if( !SINCRONIZAR_HORA_CONTROLADOR_FIS ) {
		RESETEA_ACUMULADORES_Z();  // SUMA Z
		GRABAR_XYZ( 'Y' );
	} else 	{
		if(config_tps.ImprimirYdespuesDeZ == 0) {
			if( FISCAL_Z(  ) != 1 ) {
  				return;
  			}
		}
		RESETEA_ACUMULADORES_Z(); 
		GRABAR_XYZ( 'Y' );
	}	
    SET_MEMORY_CHAR( __nro_z_emitida, ( char )1 );
    SET_MEMORY_INT( __nro_fecha_z, GET_FECHA_DOS() );
    SET_MEMORY_INT( __varios_hora_z, GET_HORA_DOS() );
    SET_MEMORY_INT( __varios_cajero_z, NRO_CAJERO );
    SET_MEMORY_LONG( __varios_nro_legajo_cajero_z, NRO_LEGAJO_CAJERO );
    COPIAR_VENTAS( SI );              /* graba log */
    if( UTILIZAR_DIA_OPERATIVO || VENTAS_A_VENTAS_Z ) {
        COPIAR_VENTAS_A_VENTAS_Z();
    }
    SET_MEMORY_CHAR( __ram_emitiendo_z, 1 ); //para que alguna falla en el mysql temrine la Z haciendo el backup
	BACKUP_VENTAS();              /* graba log */
    COPIAR_CUPONES();             /* graba log */
    CREAR_ARCHIVOS_SPAR();    /* !!!!!!!!! */
    COPIAR_EVENTOS_ACTUAL();      
    BACKUP_EVENTOS();
    BACKUP_JOURNAL_ELECTRONICO();
	SET_MEMORY_CHAR( __ram_emitiendo_z, 0 );
    #ifdef COMPILAR_ASC
    COPIAR_COBROS_ASC();
    COPIAR_PAGOS_ASC();
    CREAR_COBROS_ASC();
    CREAR_PAGOS_ASC();
    #endif
    CREAR_CUPONES();
    CEREAR_VENTAS();
    #ifdef COMPILAR_CMR
    ENVIAR_TRANSACCIONES_PENDIENTES();
    #endif
    #ifdef COMPILAR_ON_LINE
    CIERRE_DE_LOTE( _CIERRE_DE_LOTE_EN_Z );
    #endif
    if( RESETEAR_NRO_TICKET ) {
        SET_MEMORY_LONG( __nro_ultimo_ticket_ant, 0L );
        SET_MEMORY_INT( __nro_cant_tickets_ant, ( int )NRO_TICKET );
        SET_MEMORY_LONG( __nro_ticket, 0L );
    }
    else {
        SET_MEMORY_INT( __nro_cant_tickets_ant, ( int )NRO_TICKET - ( int )NRO_ULTIMO_TICKET_ANT );
        SET_MEMORY_LONG( __nro_ultimo_ticket_ant, NRO_TICKET );
    }
    SET_MEMORY_INT( __nro_nro_retiro, 1 );
    _UTILIZAR_FECHA_OPERACION = 0;
    SET_MEMORY_CHAR( __nro_z_emitida, ( char )2 );
    SET_MEMORY_INT( __nro_cajero, 0 );
    SET_MEMORY_INT( __nro_dia_operativo_ultima_z, NRO_DIA_OPERATIVO );
    SET_MEMORY_LONG( __nro_evento_a_procesar, 0 );
	SET_MEMORY_LONG( __nro_evento_prioridad_procesar, 0L );
    SET_MEMORY_LONG( __nro_ultimo_evento_controlado, 0 );
    /*if ( MODO_NEGOCIO == SERVICIOS ){
        SET_MEMORY_LONG( __nro_ultima_novedad_cliente, 0 );
    }*/
    STACK_INIT_DATOS();
}
/*****************************************************************************/
void CR0_INIT_ARCHIVOS()
/***************************************************************************/
{
    ALOJA_BUFFERS_TEMPORALES();
    LEER_DATOS_TICKET();
	VERIFICA_CAMBIO_ARCHIVOS_ACTIVOS(SI);
    CARGAR_MEDIOS_DE_PAGO();
    CARGAR_DPTOS();
    CARGAR_CONFIG();
	//VERIFICA_CAMBIO_ARCHIVOS_INICIAL();
    ALOJA_RESPUESTAS_FISCAL(); 
    CARGAR_TIPOS_DE_ENVASES();
    #ifdef COMPILAR_COMPROBANTES
    if( TIPO_FACTURA_A == LOCAL_FAC || TIPO_FACTURA_B == LOCAL_FAC || TIPO_NC_A == LOCAL_FAC
     || TIPO_NC_B == LOCAL_FAC ) {
        if( !CARGAR_CONF_FACTURA() ) {
            ABORT_STRING( S_ERROR_AL_CARGAR_CONF_DE_FACTURAS );
        }
        INIT_COMPROBANTE( _FACTURA );
    }
    if( !CARGAR_CONF_REMITO() ) {
        ABORT_STRING( S_ERROR_AL_CARGAR_CONF_DE_REMITOS );
    }
    INIT_COMPROBANTE( _REMITO );
    #endif
    if( !CARGAR_CONF_VALIDA() ) {
        ABORT_STRING( S_ERROR_AL_CARGAR_CONF_DE_VALIDACIONES );
    }
    INIT_VALIDA();
    if( !CARGAR_CONF_CLIENTES( ) ) {
        ABORT_STRING( S_ERROR_AL_CARGAR_CONF_DE_CLIENTES );
    }
    INIT_CLIENTE_CONFIGURABLE( );
    VALIDAR_CONF_CLIENTE( ); 

    #ifdef COMPILAR_COMPROBANTES
    if( TKT_CONFIGURABLE ) {
        if( !CARGAR_CONF_TKT() ) {
            ABORT_STRING( S_ABORTAR_CARGA_TKTCFG );
        }
        INIT_COMPROBANTE_TKT();
    }
    #endif
	if( !RAM_NOTA_CR ) {
    SET_MEMORY_CHAR( __modo_devolucion, RAM_MODO_DEVOLUCION );
	}
    if( CINTA_TESTIGO_ELECTRONICA ) {
        INIT_JOURNAL_ELECTRONICO();
    }
    if( !CARGAR_ARRAY_NRO_EVENTOS() ) {
        ABORT_STRING( S_ERROR_AL_CARGAR_ARRAY_NRO_EVENTOS );
    }
	if( SINCRONIZAR_HORA_CONTROLADOR_FIS ) {
		if( IMPRESORA_FISCAL == FISCAL ) { 
			ACTUALIZA_FECHA_Y_HORA_CONTROLADOR_FISCAL_EPSON(  );
		}
	}
}
/*****************************************************************************/
void CR0_LIMPIAR_RAMPAC()
/*****************************************************************************/
{
    CEREAR_STRUCT( _ram );
}
/*****************************************************************************/
int CR0_GRABAR_ENTRADA_CAJERO()
/*****************************************************************************/
{
    int ok = 1;
    if( CAJEROS_FLOTANTES ) {
        ok = GRABAR_ENTRADA_CAJERO();
        if( ok ) {
            SET_MEMORY_CHAR( __nro_hubo_operaciones, 1 );
        }
    }
    if( ok /*&& MODO_NEGOCIO == SERVICIOS*/ ) {
        SET_MEMORY_CHAR( __nro_hubo_operaciones, 1 );
    }
    SETEAR_TIEMPO( _INDEFINIDO );
    SET_MEMORY_INT( __ram_tmpo_ocio_aux, RAM_TMPO_OCIO );  
    GRABA_EVENTO_INGRESO_CAJERO();
    ACUMULAR_TIEMPOS( );
    CEREAR_TIEMPOS( );
    SETEAR_TIEMPO( _ENTRADA_OCIO );
    return ( ok );
}
/*****************************************************************************/
void MOSTRAR_DIA_OPERATIVO()
/*****************************************************************************/
{
    if( MOSTRAR_FECHA_OPERACION ) {
        _ATRIBUTO = 31;
        CAJA( 24 - ancho_panel / 2, 15, 56 - ancho_panel / 2, 17 );
        MOSTRAR_CADENA( 25 - ancho_panel / 2, 16, ST( S_FECHA_DE_OPERACION__ ) );
        MOSTRAR_FECHA( 47 - ancho_panel / 2, 16, GET_FECHA_BACKUP() );
    }
}
/*****************************************************************************/
void MOSTRAR_MEMORIA_DISPONIBLE()
/*****************************************************************************/
{
    long mem = 0;
    char aux[80];
    _snprintf( aux, sizeof(aux)-1, "Memoria : %li bytes", mem );

    MENSAJE( aux );
}
/*****************************************************************************/
void TESTEAR_CIERRE_FALLIDO()
/*****************************************************************************/
{
    if( CONTROLAR_CIERRE_FALLIDO && NRO_MODO == 'Z' && NRO_Z_EMITIDA != 2 ) {
        MOSTRAR_GRAN_CADENA( 20, 19, "CIERRE FALLIDO" );
        MOSTRAR_CADENA( 33 - ancho_panel / 2, 21, "SUPERVISOR" );
        MOSTRAR_ENTERO( 44 - ancho_panel / 2, 21, "0000", NRO_SUPERVISOR_ANT );
        while( NRO_MODO == 'Z' ) {
            MENSAJE("CONTROLANDO CIERRE FALLIDO (OBTENIENDO SCRIPT)");
            GRABAR_LOG_SISTEMA("CONTROLANDO CIERRE FALLIDO (OBTENIENDO SCRIPT)",LOG_VENTAS,3);
            PROCESAR_SCRIPT( NO );
            //PAUSA( 90 );
            SLEEP( 5000 );
            BORRAR_MENSAJE();
        }
    }
}
/*****************************************************************************/
int CR0_VERIFICAR_CAJERO_HABILITADO( int cajero )
/*****************************************************************************/
{
    int i, _cajero = -1, libres = 0, cajon = 0, primer_cajon_libre;
    /*------ Verifica que no cambie de cajero si hay recupero de memoria -----*/
    /* if( STACK_PUNTERO && cajero != NRO_CAJERO ){ MENSAJE("NO PUEDE CAMBIAR
     * DE CAJERO SIN TERMINAR LA OPERACION ANTERIOR"); return( -1 ); } */
    /*------ Busca si el cajero esta habilitado en algun cajon ----*/
    for( i = 0;i < 10;i++ ) {
        if( cajero == _VARIOS_USUARIOS_HABILITADOS( i ) ) {
            _cajero = cajero;
            cajon = i + 1;
        }
        else if( !_VARIOS_USUARIOS_HABILITADOS( i ) ) {
            libres = 1;
            primer_cajon_libre = i + 1;
        }
    }
    if( _cajero < 0 ) {
        if( libres ) {
            if( PEDIR_CAJON_EN_MULTIPLES_USUARIO ) {
                while( cajon == 0 ) {
                    cajon = PEDIR_ENTERO( 40, 20, 1, NO );
                    if( cajon > 0 && _VARIOS_USUARIOS_HABILITADOS( cajon - 1 ) ) {
                        MENSAJE_STRING( S_EL_CAJON_YA_ESTA_UTILIZADO_POR_OTRO_CAJERO );
                        cajon = 0;
                    }
                }
                BORRAR_MENSAJE();
            }
            else {
                cajon = primer_cajon_libre;
            }
            if( cajon > 0 ) {
                if( SOLICITAR_CLAVES( 34, 21, NO, SI, PEDIR_SUPERVISOR_PARA_HABIL_CAJO,
                                      ST( S_HABILITAR_CAJERO ), NULL, _F_SUP_HABILITAR_CAJERO, SI ) ) {
                    LEE_CLAVE_CAJERO( cajero, NULL );
                    CEREAR_STRUCT( _x );
                    if( CR0_GRABAR_X( cajon ) ) {
                        SET_MEMORY_CHAR( __varios_cajon_activo, ( char )cajon );
                        _cajero = cajero;
                        _SET_MEMORY_INT( __varios_usuarios_habilitados, cajon - 1, cajero );
                    }
                }
            }
        }
        else {
            MENSAJE_STRING( S_NO_HAY_CAJONES_LIBRES_PARA_UN_NUEVO_CAJERO );
        }
    }
    else {
        /*----- Recupera la estructura X -----*/
        if( !NRO_CAJERO ) {
            if( CR0_RECUPERAR_X( cajon ) ) {
                SET_MEMORY_CHAR( __varios_cajon_activo, ( char )cajon );
            }
            else {
                _cajero = -1;
            }
        }
        else {
            SET_MEMORY_CHAR( __varios_cajon_activo, ( char )cajon );
        }
    }
    return ( _cajero );
}
/*****************************************************************************/
int CR0_HAY_USUARIOS_HABILITADOS()
/*****************************************************************************/
{
    int i, rta = 0;
    /*------ Busca si hay usuarios habilitados en algun cajon ----*/
    for( i = 0;i < 10;i++ ) {
        if( _VARIOS_USUARIOS_HABILITADOS( i ) ) {
            rta = 1;
        }
    }
    return ( rta );
}
/*****************************************************************************/
int CR0_PEMITE_CERRAR( int con_menu )
/*****************************************************************************/
{
    int rta = 1, tecla = 0, i, error;
    if( CR0_HAY_USUARIOS_HABILITADOS() ) {
        if( con_menu ) {
            MENSAJE_STRING( S_PRES_ENTER_PARA_CERRAR_CAJEROS_ESC_PARA_ANULAR );
            while( tecla != 13 && tecla != 27 ) {
                tecla = GETCH();
            }
            BORRAR_MENSAJE();
            if( tecla == 13 ) {
                for( i = 0, error = 0;i < 10 && !error;i++ ) {
                    if( _VARIOS_USUARIOS_HABILITADOS( i ) ) {
                        SET_RAM( ON );
                        if( CR0_RECUPERAR_X( i + 1 ) ) {
                            ACUMULAR_X_EN_Z();
                            _SET_MEMORY_INT( __varios_usuarios_habilitados, i, 0 );
                        }
                        else {
                            error = 1;
                            MENSAJE_STRING( S_ERROR_AL_RECUPERAR_CAJERO );
                            rta = 0;
                        }
                    }
                }
                RESETEA_ACUMULADORES_X();
                GRABAR_XYZ( 'Y' );
                GRABAR_XYZ( 'X' );
                SET_MEMORY_CHAR( __nro_hubo_operaciones, 0 );
                SET_MEMORY_CHAR( __nro_modo, 'X' );
                SET_MEMORY_LONG( __nro_legajo_cajero, 0 );
                SET_MEMORY_INT( __nro_cajero, 0 );
                if( USAR_LEGAJO == RUT ) {
                    SET_MEMORY( __ram_doc_cajero, "          " );
                }
            }
            else {
                rta = 0;
            }
        }
        else {
            rta = 0;
        }
    }
    return ( rta );
}
/*****************************************************************************/
int CR0_GRABAR_X( int cajon )
/*****************************************************************************/
{
    char nombre[13],*buffer;
    int handle, ok = 0, l_buffer;
    NRO_ARCHIVO = 0;
    _snprintf( nombre, sizeof(nombre)-1, "X.%.3i", cajon );
    handle = _OPEN_O_CREAT( nombre, O_RDWR, __LINE__, __FILE__ );
    if( handle > 0 ) {
        l_buffer = SIZEOF_STRUCT( _x );
//(Nota Unific MySQL-Linux): casting existente en la version linux. No en 01.002
        buffer = (char *)MALLOC( l_buffer );
        if( buffer ) {
            COPY_STRUCT_TO_BUFFER( buffer, _x );
            if( _WRITE( handle, buffer, l_buffer, __LINE__, __FILE__ ) == 0 ) {
                ok = 1;
            }
            free( buffer );
        }
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    return ( ok );
}
/*****************************************************************************/
int CR0_RECUPERAR_X( int cajon )
/*****************************************************************************/
{
    char nombre[13],*buffer;
    int handle, ok = 0, l_buffer;
    NRO_ARCHIVO = 0;
    _snprintf( nombre, sizeof(nombre)-1, "X.%.3i", cajon );
    handle = _OPEN( nombre, O_RDWR, __LINE__, __FILE__ );
    if( handle > 0 ) {
        l_buffer = SIZEOF_STRUCT( _x );
        buffer = MALLOC( l_buffer );
        if( buffer ) {
            if( _READ( handle, buffer, l_buffer, __LINE__, __FILE__ ) == 0 ) {
                ok = 1;
                COPY_BUFFER_TO_STRUCT( buffer, _x );
            }
            free( buffer );
        }
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    return ( ok );
}
/*****************************************************************************/
int CR0_MOSTRAR_CAJONES_HABILITADOS( int elegir_cajero )
/*****************************************************************************/
{
    int i, rta = 0, seguir, tecla;
    if( MULTIPLES_USUARIOS_PARA_VENTA
     && SOLICITAR_CLAVES( 30, 21, NO, SI, SI, ST( S_CAJONES_HABILITADOS ), NULL,
                          _F_SUP_CAJONES_HAB, SI ) ) {
        for( i = 0;i < 9;i++ ) {
            MOSTRAR_ENTERO( 36, 13 + i, "0", i + 1 );
            if( _VARIOS_USUARIOS_HABILITADOS( i ) ) {
                MOSTRAR_ENTERO( 42, 13 + i, "0000", _VARIOS_USUARIOS_HABILITADOS( i ) );
            }
            else {
                MOSTRAR_CADENA( 42, 13 + i, "----" );
            }
        }
        if( !elegir_cajero ) {
            MENSAJE_STRING( S_PRESIONE_ENTER_PARA_CONTINUAR );
            while( GETCH() != 13 );
            BORRAR_MENSAJE();
        }
        else {
            seguir = 1;
            MENSAJE_STRING( S_ELIJA_NUMERO_CAJON_A_CERRAR_O_PRES_ESC );
            while( seguir ) {
                tecla = GETCH();
                if( tecla == 27 ) {
                    seguir = 0;
                }
                else {
                    if( tecla >= '1' && tecla <= '9' ) {
                        if( _VARIOS_USUARIOS_HABILITADOS( tecla - '1' ) ) {
                            seguir = 0;
                            rta = _VARIOS_USUARIOS_HABILITADOS( tecla - '1' );
                        }
                    }
                }
            }
            BORRAR_MENSAJE();
        }
    }
    return ( rta );
}
/*****************************************************************************/
int NUMERO_CAJA()
/*****************************************************************************/
{
    int rta;
    rta = ( USAR_NRO_CAJA_DGI ) ? NRO_CAJA_DGI : NRO_CAJA;
    return ( rta );
}
/*****************************************************************************/
void ALOJA_BUFFERS_TEMPORALES()
/*****************************************************************************/
{
    //Se comenta provisoriamente hasta que quede claro para que se usa
	//COMIENZO_NET_COM();
    event_varios = ( struct _event_varios * )MALLOC( sizeof( struct _event_varios ) );
    if( !event_varios ) {
        ABORT( "ERROR - CARGAR EVENT VARIOS" );
    }
    event_varios_all = MALLOC( sizeof( struct _event_varios ) );
    if( !event_varios_all ) {
        ABORT( "ERROR - EVENT VARIOS ALL" );
    }
    cheques_clientes = MALLOC( sizeof( struct _cliecheq ) );
    if( !cheques_clientes ) {
        ABORT( "ERROR - CHEQUES CLIENTES" );
    }
    event_despro = MALLOC( sizeof( struct _ev_despro ) );
//(Nota Unific MySQL-Linux): lo siguiente queda comentado como en la version linux. En 01.002 estaba sin comentar.
    //indice = MALLOC( sizeof( struct _ind_e_des_pro_btr ) );
    if( !event_despro /*|| !indice*/ ) {
        ABORT( "ERROR - EVENTOS PRORRATEO" );
    }
    event_factura = MALLOC( sizeof( struct _ev_factu ) );
    if( !event_factura ) {
        ABORT( "ERROR - EVENTOS FACTURA" );
    }
    T_COMIENZO_NETCOM2();
}
/*****************************************************************************/
void LIBERA_BUFFERS_TEMPORALES()
/*****************************************************************************/
{
    free( event_varios );
    free( event_varios_all );
    free( cheques_clientes );
    free( event_despro );
    free( indice );
    free( event_factura );
    free( rta_fiscal );
    FIN_NET_COM();
    LIBERA_UTI_VAL();
    LIBERA_UTI_COMP();
    LIBERA_UTI_CLI();
    // ---PROTOCOLO_AUTORIZACION_TARJETA
    LIBERA_MEMORIA_NETCOM2();
    LIBERAR_ARRAY_NRO_EVENTOS( );
}
/*****************************************************************************/
void ALOJA_STRING_TABLE()
/*****************************************************************************/
{
    string_table = MALLOC( sizeof( struct _stringt ) );
    if( !string_table ) {
        ABORT( "ERROR - STRING" );
    }
}
/*****************************************************************************/
long GRABAR_CIERRE( void )
/*****************************************************************************/
{
    int i, rta = 0, area, h, tipo;
    double tot_ventas = 0;
	char cadena[100];

    memset( event_cierre,  0, sizeof( struct _ev_cierre ) );
	memset( cadena, 0, sizeof( cadena ) );

    for( h = 0;h < 9;h++ ) {
        tot_ventas += _GET_MEMORY_DOUBLE( __z_ventas_importe, h );
    }
    if( tot_ventas == 0 ) {
        tot_ventas = 0.00001;
    }
	area = SELECTED2();
	tipo = SELECTED_TIPO();
    // ------------------------------------------------
    // PROCESA TICKET
    // ------------------------------------------------
	/*SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
	SET_ORDER2("ID_EVENTO");
	SET_WHERE("");
	RUN_QUERY(NO);
	GO2(BOTTOM);*/
    if( event_ticket.tipo_evento != 10 ) {
        MENSAJE_STRING( S_Z_NO_ENCONTRADA );
    }
    else {
        if( event_ticket.status == 1/*2*/ ) {
            MENSAJE_STRING( S_Z_YA_PROCESADA );
            return -2;
        }
        else {
            rta = 0;
            if( !rta ) {
                struct
                {
                    long caja_z;
                    long id_evento;
                } index;
                index.caja_z = /*event_ticket.caja_z*/( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
                index.id_evento = /*event_ticket.id_evento*/RAM_ID_EVENTO;
				SELECT_TABLE( T_EV_CIERRE, TT_ORIG );
				SET_WHERE("CAJA_Z = '%ld' and ID_EVENTO = '%ld'",index.caja_z,
							index.id_evento);
				SET_ORDER2("CAJA_Z,ID_EVENTO");
				RUN_QUERY(NO);
                if( FOUND2() ) {
                    //SELECT( area );
                    return rta = event_ticket.id_evento;
                }
                // -------------------------------------------
                // INCLUIR Z
                // -------------------------------------------
#ifndef COMPILAR_MOZO
				{	//peido por UAA grabar el nro de lote en la ev_ciere
					char consulta[50];
					int lote =0 ;
					int C_INT_=   4;
					memset(consulta,0,50);
					for( h = 1;h < 4;h++ ) { //tenemos para 3 nodos
						_snprintf(consulta,sizeof(consulta)-1,"SELECT MAX(LOTE) FROM transac2 WHERE nodo = %i",h);
						if(DIRECT_DB_QUERY(consulta) > 0){ //OBTIENE EL MAXIMO ID EVENTO
							int h2 = h;
							--h2;
							GET_DATO(1,C_INT_,(char *)&lote,4);
							_SET_MEMORY_DOUBLE( __z_mozos_cantidad, h2, lote );
						}
					}
				}
#endif COMPILAR_MOZO
                event_cierre->caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L )/*event_ticket.caja_z*/;
                event_cierre->caja =  NRO_CAJA/*event_ticket.caja*/;
                event_cierre->nro_z = NRO_Z/*event_ticket.nro_z*/;
                event_cierre->id_evento = RAM_ID_EVENTO/*event_ticket.id_evento*/;
                event_cierre->gran_total = 0.0;
                event_cierre->nro = NRO_Z;
                event_cierre->adelantos.imp = Z_ADELANTOS_IMPORTE;
                event_cierre->adelantos.can = Z_ADELANTOS_CANTIDAD;
                event_cierre->intereses.imp = Z_INTERESES_IMPORTE;
                event_cierre->intereses.can = Z_INTERESES_CANTIDAD;
                event_cierre->fondo_fijo.imp = Z_FONDO_FIJO_IMPORTE;
                event_cierre->fondo_fijo.can = Z_FONDO_FIJO_CANTIDAD;
                event_cierre->anulaciones.imp = Z_ANULACIONES_IMPORTE;
                event_cierre->anulaciones.can = Z_ANULACIONES_CANTIDAD;
                event_cierre->dev_envases.imp = Z_DEV_ENVASES_IMPORTE;
                event_cierre->dev_envases.can = Z_DEV_ENVASES_CANTIDAD;
                event_cierre->ventas_gravadas = Z_VENTAS_GRAVADAS;
                event_cierre->ventas_exentas = Z_VENTAS_EXENTAS;
                event_cierre->imp_int = Z_IMP_INT;
                event_cierre->tot_dinero = Z_TOT_DINERO;
                event_cierre->iva1 = Z_IVA1;
                event_cierre->iva2 = Z_IVA2;
                event_cierre->retiro_fondo.imp = Z_RETIRO_FONDO_IMPORTE;
                event_cierre->retiro_fondo.can = Z_RETIRO_FONDO_CANTIDAD;
                event_cierre->pagos_por_caja.imp = Z_PAGOS_POR_CAJA_IMPORTE;
                event_cierre->pagos_por_caja.can = Z_PAGOS_POR_CAJA_CANTIDAD;
                event_cierre->facturas_a.imp = Z_FACTURAS_A_IMPORTE;
                event_cierre->facturas_a.can = Z_FACTURAS_A_CANTIDAD;
                event_cierre->facturas_b.imp = Z_FACTURAS_B_IMPORTE;
                event_cierre->facturas_b.can = Z_FACTURAS_B_CANTIDAD;
				event_cierre->factura_a_importe_gravado = Z_FACTURA_A_IMPORTE_GRAVADO;
				event_cierre->factura_a_importe_exento = Z_FACTURA_A_IMPORTE_EXENTO;
                event_cierre->vueltos.imp = Z_VUELTOS_IMPORTE;
                event_cierre->vueltos.can = Z_VUELTOS_CANTIDAD;
                event_cierre->bonos_entregados = Z_BONOS_ENTREGADOS;
                event_cierre->diferencia = ROUND( Z_DIFERENCIA, ENTEROS, DECIMALES );
                event_cierre->nc_a.imp = Z_NC_A_IMPORTE;
                event_cierre->nc_a.can = Z_NC_A_CANTIDAD;
                event_cierre->nc_b.imp = Z_NC_B_IMPORTE;
                event_cierre->nc_b.can = Z_NC_B_CANTIDAD;
                event_cierre->retencion_dgr = Z_RETENCION_DGR;
                event_cierre->percepdion_dgi = Z_PERCEPCION_DGI;
                event_cierre->imp_correccion = Z_CORRECCION_IMPORTE;
                event_cierre->can_correcion = Z_CORRECCION_CANTIDAD;
                event_cierre->primer_ticket = Z_NRO_PRIMER_TICKET;
                event_cierre->cantidad_tickets = Z_CANTIDAD_TICKETS;
                event_cierre->ret_rg_212_1 = Z_PERCEPCION_212_IVA1;
                event_cierre->ret_rg_212_2 = Z_PERCEPCION_212_IVA2;
                event_cierre->redondeo = Z_DIFERENCIA_REDONDEO;
                event_cierre->devoluciones.imp = Z_DEVOLUCIONES_IMPORTE;
                event_cierre->devoluciones.can = Z_DEVOLUCIONES_CANTIDAD;
                event_cierre->cant_articulos = Z_CANT_ARTICULOS;
                event_cierre->cant_art_escaneados = Z_CANT_ART_ESCANEADOS;
                event_cierre->tmpo_reg_articulo = Z_TMPO_REG_ARTICULO;
                event_cierre->tmpo_pago = Z_TMPO_PAGO;
                event_cierre->tmpo_standby = Z_TMPO_STANDBY;
                event_cierre->tmpo_ocio = Z_TMPO_OCIO;
                event_cierre->tmpo_no_venta = Z_TMPO_NO_VENTA;
                event_cierre->anulados.imp = Z_IMP_CANCELADOS;
                event_cierre->anulados.can = Z_CANT_CANCELADOS;
                event_cierre->cobros_cantidad_servicios = Z_COBROS_CANTIDAD_SERVICIOS;
				event_cierre->cobros_importe_servicios = Z_COBROS_IMPORTES_SERVICIOS;
                event_cierre->cod_sucursal = config.sucursal;
				if( !NRO_FECHA_OPERACION )
				      SET_MEMORY_INT( __nro_fecha_operacion, GET_FECHA_DOS() );

				event_cierre->fecha_ticket = NRO_FECHA_OPERACION;
                event_cierre->donaciones.imp = Z_DONACIONES_IMPORTE;
                event_cierre->donaciones.can = Z_DONACIONES_CANTIDAD; 

				
				event_cierre->nc_a_importe_gravado =    Z_NC_A_IMPORTE_GRAVADO                ;
				event_cierre->nc_a_importe_exento = Z_NC_A_IMPORTE_EXENTO                     ;
				event_cierre->cons_final_importe_exent_fact = Z_CONS_FINAL_IMPORTE_EXENT_FACT;
				event_cierre->cons_final_importe_grav_fact = Z_CONS_FINAL_IMPORTE_GRAV_FACT  ;
				event_cierre->cons_final_cantidad_fact = Z_CONS_FINAL_CANTIDAD_FACT          ;
				event_cierre->cons_final_importe_exent_tkt = Z_CONS_FINAL_IMPORTE_EXENT_TKT  ;
				event_cierre->cons_final_cantidad_tkt = Z_CONS_FINAL_CANTIDAD_TKT			 ;
				event_cierre->cons_final_importe_grav_tkt = Z_CONS_FINAL_IMPORTE_GRAV_TKT    ;
				event_cierre->dev_cons_f_importe_exent_tkt = Z_DEV_CONS_F_IMPORTE_EXENT_TKT  ;
				event_cierre->dev_cons_f_importe_grav_tkt = Z_DEV_CONS_F_IMPORTE_GRAV_TKT    ;
				event_cierre->dev_cons_f_cantidad_tkt = Z_DEV_CONS_F_CANTIDAD_TKT            ;
				event_cierre->iva_factura_a = Z_IVA_FACTURA_A                                ;
				event_cierre->ofertas_importe = Z_OFERTA_IMPORTE                             ;
				event_cierre->iva_nc_a = Z_IVA_NC_A                                          ;
				event_cierre->iva_devol_consumidor_final= Z_IVA_DEVOL_CONSUMIDOR_FINAL       ;
				event_cierre->iva_cons_final = Z_IVA_CONS_FINAL								 ;
				event_cierre->dev_cons_f_importe_grav_fact = Z_DEV_CONS_F_IMPORTE_GRAV_FACT	 ;
				event_cierre->dev_cons_f_importe_exent_fact = Z_DEV_CONS_F_IMPORTE_EXENT_FACT;
				//3ingreso
				event_cierre->ventas_diplom_importe = Z_VENTAS_DIPLOM_IMPORTE				 ;
                event_cierre->ventas_diplom_cantidad = Z_VENTAS_DIPLOM_CANTIDAD				 ;
				event_cierre->devoluciones_diplom_importe = Z_DEVOLUCIONES_DIPLOM_IMPORTE	 ;
                event_cierre->devoluciones_diplom_cantidad = Z_DEVOLUCIONES_DIPLOM_CANTIDAD	 ;
				event_cierre->devoluciones_export_importe = Z_DEVOLUCIONES_EXPORT_IMPORTE    ;
                event_cierre->devoluciones_export_cantidad = Z_DEVOLUCIONES_EXPORT_CANTIDAD	 ;
				
                for( i = 0;i < 10;i++ ) {
                    event_cierre->imp_ventas[i] = _Z_VENTAS_IMPORTE( i );
                    event_cierre->can_ventas[i] = _Z_VENTAS_CANTIDAD( i );
                    event_cierre->imp_cobros[i] = _Z_COBROS_IMPORTE( i );
                    event_cierre->can_cobros[i] = _Z_COBROS_CANTIDAD( i );
                    event_cierre->imp_fondo_fijo_multiple[i] = _Z_FONDO_FIJO_MULTIPLE_IMPORTE( i );
                    event_cierre->can_fondo_fijo_multiple[i] = _Z_FONDO_FIJO_MULTIPLE_CANTIDAD( i );
                    event_cierre->importes_iva[i] = _Z_IMPORTE_IVA( i );
                    event_cierre->sobretasas_iva[i] = _Z_IMPORTE_SOBRETASA_IVA( i );            
                    event_cierre->imp_retiros[i] = _Z_RETIROS_IMPORTE( i );             
                    event_cierre->can_retiros[i] = _Z_RETIROS_CANTIDAD( i );
                    event_cierre->imp_cambios[i] = _Z_CAMBIOS_IMPORTE( i );
                    event_cierre->can_cambios[i] = _Z_CAMBIOS_CANTIDAD( i );
                    event_cierre->devoluciones_medio[i] = _Z_DEVOLUCIONES_MEDIO( i );

					event_cierre->descuentos_medio[i]=_Z_DESCUENTOS_MEDIO(i) ;
					event_cierre->ventas_importe_sin_redondeo[i]=_Z_VENTAS_IMPORTE_SIN_REDONDEO(i);

					//aca grabo el nro de lote para UAA
#ifndef COMPILAR_MOZO
					event_cierre->can_pagosesp[i] = ( short )_Z_MOZOS_CANTIDAD( i );
#endif COMPILAR_MOZO
                }
				SELECT_TABLE( T_EV_CIERRE, TT_ORIG );
                rta = INSERT2(NO);
				_snprintf( cadena, sizeof(cadena)-1,"MSJ EV_CIERRE --> ID_EVENTO %ld RTA %i",
						event_cierre->id_evento, rta );
				GRABAR_LOG_SISTEMA( cadena ,LOG_ERRORES,2);
			}
        }
    }
    SELECT_TABLE( area, tipo );
    return rta;
}

/*****************************************************************************/
int EVENTO_PROCESADO( long id_evento )
/*****************************************************************************/
{
    int rta = 0;
	char cad[100];
	
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );

	SET_ORDER2("ID_EVENTO");
	SET_WHERE("ID_EVENTO='%ld'", id_evento );
	RUN_QUERY(NO);

	if( FOUND2() ) {
		if( event_ticket.status == 1 ) {
			rta = 1;
			_snprintf( cad, sizeof(cad)-1,
			"MSJ EVENTO PROCESADO --> ID_EVENTO %ld TIPO_EVENTO %d",
				event_ticket.id_evento, event_ticket.tipo_evento );
		} else {
			_snprintf( cad, sizeof(cad)-1,
			"MSJ EVENTO NO PROCESADO --> ID_EVENTO %ld TIPO_EVENTO %d",
			event_ticket.id_evento, event_ticket.tipo_evento );
		}
	} else {
		rta = 99;
		_snprintf( cad, sizeof(cad)-1,
			"MSJ ERROR NO SE ENCUENTRA --> ID_EVENTO %ld ",
			id_evento );
	}

	GRABAR_LOG_SISTEMA( cad ,LOG_VENTAS,1);

    return ( rta );
}

/*****************************************************************************/
int GRABAR_CIERRE_CAJERO( void )
/*****************************************************************************/
{
    int i = 0, rta = 0, tabla_ant = 0, tipo_ant = 0;
	char cadena[100];
	int nro_fecha_operacion = 0;

    MENSAJE_STRING( S_Grabando_Cierre_de_Caja_en_Servidor );
	memset( cadena, 0, sizeof( cadena ) );
    memset( event_cierre, 0, sizeof( struct _ev_cierre ) );
    tabla_ant = SELECTED2();
    tipo_ant = SELECTED_TIPO();
    #ifdef COMPILAR_MODO_ENTRENAMIENTO
    if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
        return 1;
    }
    #endif
    // ------------------------------------------------
    // PROCESA TICKET
    // ------------------------------------------------
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
	SET_ORDER2("ID_EVENTO");
	SET_WHERE("");
	RUN_QUERY(NO);
	GO2(BOTTOM);
    if( event_ticket.tipo_evento != 9 ) {
        MENSAJE_STRING( S_Y_NO_ENCONTRADA );
    }
    else {
        if( event_ticket.status == 1 ) {
            MENSAJE_STRING( S_Y_YA_PROCESADA );
        }
        else {
            rta = 0;
            if( !rta ) {
                // -------------------------------------------
                // INCLUIR Y
                // -------------------------------------------
                event_cierre->caja = event_ticket.caja;
                event_cierre->nro_z = event_ticket.nro_z;
                event_cierre->caja_z = event_ticket.caja_z;
                event_cierre->id_evento = event_ticket.id_evento;
                event_cierre->gran_total = 0.0;
                event_cierre->nro = NRO_Y;
                for( i = 0;i < 10;i++ ) {
                    event_cierre->imp_ventas[i] = _X_VENTAS_IMPORTE( i );
                    event_cierre->can_ventas[i] = _X_VENTAS_CANTIDAD( i );
                    event_cierre->imp_cobros[i] = _X_COBROS_IMPORTE( i );
                    event_cierre->can_cobros[i] = _X_COBROS_CANTIDAD( i );
                    event_cierre->imp_fondo_fijo_multiple[i] = _X_FONDO_FIJO_MULTIPLE_IMPORTE( i );
                    event_cierre->imp_fondo_fijo_multiple_me[i] = _X_FONDO_FIJO_MULTIPLE_IMP_ME(
                    i );
                    event_cierre->can_fondo_fijo_multiple[i] = _X_FONDO_FIJO_MULTIPLE_CANTIDAD( i );
                    event_cierre->importes_iva[i] = _X_IMPORTE_IVA( i );
                    event_cierre->sobretasas_iva[i] = _X_IMPORTE_SOBRETASA_IVA( i );
                    event_cierre->imp_retiros[i] = _X_RETIROS_IMPORTE( i );
                    event_cierre->imp_retiros_me[i] = _X_RETIROS_IMPORTE_ME( i );
                    event_cierre->can_retiros[i] = _X_RETIROS_CANTIDAD( i );
                    event_cierre->imp_cambios[i] = _X_CAMBIOS_IMPORTE( i );
                    event_cierre->can_cambios[i] = _X_CAMBIOS_CANTIDAD( i );
                    event_cierre->devoluciones_medio[i] = _X_DEVOLUCIONES_MEDIO( i );
                    event_cierre->imp_pagosesp[i] = _X_MOZOS_IMPORTE( i );
                    event_cierre->can_pagosesp[i] = ( short )_X_MOZOS_CANTIDAD( i );
					event_cierre->ventas_importe_sin_redondeo[i] = _X_VENTAS_IMPORTE_SIN_REDONDEO(i);
					event_cierre->cobros_importe_me[i] = _X_COBROS_IMPORTE_ME(i);
					event_cierre->retiro_fondo_importe_pago[i] = _X_RETIRO_FONDO_IMPORTE_PAGOS(i);
                }               
                event_cierre->adelantos.imp = X_ADELANTOS_IMPORTE;
                event_cierre->adelantos.can = X_ADELANTOS_CANTIDAD;
                event_cierre->intereses.imp = X_INTERESES_IMPORTE;
                event_cierre->intereses.can = X_INTERESES_CANTIDAD;
                event_cierre->fondo_fijo.imp = X_FONDO_FIJO_IMPORTE;
                event_cierre->fondo_fijo.can = X_FONDO_FIJO_CANTIDAD;
                event_cierre->anulaciones.imp = X_ANULACIONES_IMPORTE;
                event_cierre->anulaciones.can = X_ANULACIONES_CANTIDAD;
                event_cierre->dev_envases.imp = X_DEV_ENVASES_IMPORTE;
                event_cierre->dev_envases.can = X_DEV_ENVASES_CANTIDAD;
                event_cierre->ventas_gravadas = X_VENTAS_GRAVADAS;
                event_cierre->ventas_exentas = X_VENTAS_EXENTAS;
                event_cierre->imp_int = X_IMP_INT;
                event_cierre->tot_dinero = X_TOT_DINERO;
                event_cierre->iva1 = X_IVA1;
                event_cierre->iva2 = X_IVA2;
                event_cierre->retiro_fondo.imp = X_RETIRO_FONDO_IMPORTE;
                event_cierre->retiro_fondo.can = X_RETIRO_FONDO_CANTIDAD;
                event_cierre->pagos_por_caja.imp = X_PAGOS_POR_CAJA_IMPORTE;
                event_cierre->pagos_por_caja.can = X_PAGOS_POR_CAJA_CANTIDAD;
                event_cierre->facturas_a.imp = X_FACTURAS_A_IMPORTE;
                event_cierre->facturas_a.can = X_FACTURAS_A_CANTIDAD;
                event_cierre->facturas_b.imp = X_FACTURAS_B_IMPORTE;
                event_cierre->facturas_b.can = X_FACTURAS_B_CANTIDAD;
                event_cierre->vueltos.imp = X_VUELTOS_IMPORTE;
                event_cierre->vueltos.can = X_VUELTOS_CANTIDAD;
                event_cierre->bonos_entregados = X_BONOS_ENTREGADOS;
                event_cierre->diferencia = ROUND( X_DIFERENCIA, ENTEROS, DECIMALES );
                event_cierre->nc_a.imp = X_NC_A_IMPORTE;
                event_cierre->nc_a.can = X_NC_A_CANTIDAD;
                event_cierre->nc_b.imp = X_NC_B_IMPORTE;
                event_cierre->nc_b.can = X_NC_B_CANTIDAD;                                   
                event_cierre->retencion_dgr = X_RETENCION_DGR;
                event_cierre->percepdion_dgi = X_PERCEPCION_DGI;                    
                event_cierre->imp_correccion = X_CORRECCION_IMPORTE;
                event_cierre->can_correcion = X_CORRECCION_CANTIDAD;
                event_cierre->primer_ticket = X_NRO_PRIMER_TICKET;
                event_cierre->cantidad_tickets = X_CANTIDAD_TICKETS;                
                event_cierre->cobros_cantidad_servicios = X_COBROS_CANTIDAD_SERVICIOS;
				event_cierre->cobros_importe_servicios = X_COBROS_IMPORTES_SERVICIOS;
                event_cierre->ret_rg_212_1 = X_PERCEPCION_212_IVA1;
                event_cierre->ret_rg_212_2 = X_PERCEPCION_212_IVA2;
                event_cierre->redondeo = X_DIFERENCIA_REDONDEO;
                event_cierre->devoluciones.imp = X_DEVOLUCIONES_IMPORTE;
                event_cierre->devoluciones.can = X_DEVOLUCIONES_CANTIDAD;
                event_cierre->cant_articulos = X_CANT_ARTICULOS;
                event_cierre->cant_art_escaneados = X_CANT_ART_ESCANEADOS;
                event_cierre->tmpo_reg_articulo = X_TMPO_REG_ARTICULO;
                event_cierre->tmpo_pago = X_TMPO_PAGO;
                event_cierre->tmpo_standby = X_TMPO_STANDBY;
                event_cierre->tmpo_ocio = X_TMPO_OCIO;
                event_cierre->tmpo_no_venta = X_TMPO_NO_VENTA;
                event_cierre->anulados.imp = X_IMP_CANCELADOS;
                event_cierre->anulados.can = X_CANT_CANCELADOS;
                event_cierre->cod_sucursal = config.sucursal;
				nro_fecha_operacion = NRO_FECHA_OPERACION;
				if( !nro_fecha_operacion )
					SET_MEMORY_INT( __nro_fecha_operacion, GET_FECHA_DOS() );
				event_cierre->fecha_ticket = NRO_FECHA_OPERACION/*( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS()*/;
                event_cierre->donaciones.imp = X_DONACIONES_IMPORTE;
                event_cierre->donaciones.can = X_DONACIONES_CANTIDAD;
                event_cierre->factura_a_importe_exento = X_FACTURA_A_IMPORTE_EXENTO;
                event_cierre->factura_a_importe_gravado = X_FACTURA_A_IMPORTE_GRAVADO;
                event_cierre->nc_a_importe_exento = X_NC_A_IMPORTE_EXENTO;
                event_cierre->nc_a_importe_gravado = X_NC_A_IMPORTE_GRAVADO;
                event_cierre->cons_final_importe_exent_fact = X_CONS_FINAL_IMPORTE_EXENT_FACT;
                event_cierre->cons_final_importe_grav_fact = X_CONS_FINAL_IMPORTE_GRAV_FACT;
                event_cierre->cons_final_cantidad_fact = X_CONS_FINAL_CANTIDAD_FACT;
                event_cierre->dev_cons_f_importe_exent_fact = X_DEV_CONS_F_IMPORTE_EXENT_FACT;
                event_cierre->dev_cons_f_importe_grav_fact = X_DEV_CONS_F_IMPORTE_GRAV_FACT;
                event_cierre->dev_cons_f_cantidad_fact = X_DEV_CONS_F_CANTIDAD_FACT;
                event_cierre->ventas_diplom_importe = X_VENTAS_DIPLOM_IMPORTE;
                event_cierre->ventas_diplom_cantidad = X_VENTAS_DIPLOM_CANTIDAD;
                event_cierre->devoluciones_diplom_importe = X_DEVOLUCIONES_DIPLOM_IMPORTE;
                event_cierre->devoluciones_diplom_cantidad = X_DEVOLUCIONES_DIPLOM_CANTIDAD;
                event_cierre->nro_transaccion_inicial = X_NRO_TRANSACCION_INICIAL;
                event_cierre->nro_transaccion_final = X_NRO_TRANSACCION_FINAL;
                event_cierre->devoluciones_export_importe = X_DEVOLUCIONES_EXPORT_IMPORTE;
                event_cierre->devoluciones_export_cantidad = X_DEVOLUCIONES_EXPORT_CANTIDAD;
                event_cierre->cons_final_importe_exent_tkt = X_CONS_FINAL_IMPORTE_EXENT_TKT;
                event_cierre->cons_final_importe_grav_tkt = X_CONS_FINAL_IMPORTE_GRAV_TKT;
                event_cierre->cons_final_cantidad_tkt = X_CONS_FINAL_CANTIDAD_TKT;
                event_cierre->dev_cons_f_importe_exent_tkt = X_DEV_CONS_FINAL_IMPORTE_EXENT_TKT;
                event_cierre->dev_cons_f_importe_grav_tkt = X_DEV_CONS_F_IMPORTE_GRAV_TKT;
                event_cierre->dev_cons_f_cantidad_tkt = X_DEV_CONS_F_CANTIDAD_TKT;
                event_cierre->iva_factura_a = X_IVA_FACTURA_A;
                event_cierre->iva_cons_final = X_IVA_CONS_FINAL;
                event_cierre->ventas_export_importe = X_VENTAS_EXPORT_IMPORTE;
                event_cierre->ventas_export_cantidad = X_VENTAS_EXPORT_CANTIDAD;
                event_cierre->ofertas_importe = X_OFERTAS_IMPORTE;
                event_cierre->iva_nc_a = X_IVA_NC_A;
                event_cierre->iva_devol_consumidor_final = X_IVA_DEVOL_CONSUMIDOR_FINAL;
				event_cierre->cant_cierres_y = cant_cierres_y + 1;

				for(i=0;i<4;i++)
					event_cierre->impuestos[i] = _X_IMPUESTOS(i);

				for(i=0;i<11;i++)
					event_cierre->descuentos_medio[i] = _X_DESCUENTOS_MEDIO(i);
				SELECT_TABLE( T_EV_CIERRE, TT_ORIG );
                #ifdef INVEL_L
                //BEGIN_TRANSACTION();
                #endif
                rta = INSERT2(NO);
				_snprintf( cadena, sizeof(cadena)-1,"MSJ EV_CIERRE GCC--> ID_EVENTO %ld RTA %i",
						event_cierre->id_evento, rta );
				GRABAR_LOG_SISTEMA( cadena ,LOG_ERRORES,2);
                #ifdef INVEL_L
                //END_TRANSACTION();
                #endif
				ENVIAR_SUBMEDIOS_REC();
            }
        }
    }
    SELECT_TABLE( tabla_ant, tipo_ant );
    return rta;
}
/*****************************************************************************/
void GRABA_EVENTO_INGRESO_CAJERO( void )
/*****************************************************************************/
{
    memset( &event_ticket, 0, sizeof( event_ticket ) );
    if( !__NO_SE_USA_002 ) {  
		//__NO_SE_USA_002  sirve para saber si un cajero ya iniciï¿½ un perï¿½odo de venta
		//si ya estï¿½ abierto no debe grabar otro ingreso de cajero. 
        event_ticket.tipo_evento = 26;  //Ingreso Cajero
        event_ticket.nro_ticket = 0;
        event_ticket.nro_comprobante_new = 0;
		event_ticket.importe_sin_iva = 0;
        GRABAR_EVENT_TICKET( NO );
        ACTUALIZA_ID_EVENTO_NEW();
		SET_MEMORY_INT( ____no_se_usa_002, SI );
    }
}
/*****************************************************************************/
void GRABA_EVENTO_SALIDA_CAJERO( void )
/*****************************************************************************/
{
    
    event_ticket.tipo_evento = 27;  //Salida Cajero
    event_ticket.nro_ticket = 0;
    event_ticket.nro_comprobante_new = 0;
    GRABAR_EVENT_TICKET( NO );
    ACTUALIZA_ID_EVENTO_NEW();
    
}

/*****************************************************************************/
void SETEAR_TIEMPO( int estado )
/*****************************************************************************/
{
    long hora_segundos = 0, segundos_aux = 0;
    char ok = 0, log[50];
    int set_var = 0, iter;
#define CANT_EST 5
    short int estados[CANT_EST] = { _ENTRADA_OCIO, _ENTRADA_REG, _ENTRADA_SBY, _ENTRADA_PAGO, _ENTRADA_NO_VENTA };
    char *nombres[CANT_EST] = { "OCIO", "VENTAS", "STANDBY", "PAGO", "NO VENTA" };
    //int set_vars[CANT_EST] = { __x_tmpo_ocio, __ram_tmpo_reg_articulo, __x_tmpo_standby, __ram_tmpo_pago, __x_tmpo_no_venta };
    int set_vars[CANT_EST] = { __ram_tmpo_ocio, __ram_tmpo_reg_articulo, __x_tmpo_standby, __ram_tmpo_pago, __ram_tmpo_no_venta };
    
    if( estado != NRO_ESTADO_ACTUAL ) {                       
        /*---Acumulo el tiempo de Ocio antes de tickar algo---*/
        hora_segundos = GET_HORA_SEGUNDOS( );
        segundos_aux = hora_segundos - NRO_ENTRADA_ESTADO_ACTUAL;
        if ( segundos_aux < 0 ) {
            segundos_aux = 0; 
        }
          
        strncpy( log, "PRODUCTIVIDAD: Entra a estado ", sizeof( log ) );
        log[ sizeof( log ) - 1] = '\0';

        //Determina la variable RNV donde guardar el tiempo transcurrido del
        //estado actual y guarda el nombre del estado al que se pasa, para el log
        for( iter = 0; iter < CANT_EST; iter++ ) {
            if( NRO_ESTADO_ACTUAL == estados[iter] ) {
                set_var = set_vars[iter];
            }
            if( estado == estados[iter] ) {            
                strncat( log, nombres[iter], sizeof( log ) - strlen( log ) - 1 );
            }
        }
        //No se guarda tiempo de estado indefinido, pero si log
        if( estado == _INDEFINIDO ) {
            strncat( log, "INDEFINIDO", sizeof( log ) - strlen( log ) - 1 );
        }
        log[sizeof( log ) - 1] = '\0';
        
        if ( set_var != 0 ) {
            SUMAR_VAR_UINT( set_var, segundos_aux );
        }
        SET_MEMORY_LONG( __nro_entrada_estado_actual, hora_segundos );
        SET_MEMORY_CHAR( __nro_estado_actual, estado );
        GRABAR_LOG_SISTEMA( log ,LOG_VENTAS,1);
    }
#undef CANT_EST
}
/*****************************************************************************/
void SUMAR_VAR_UINT( int var, long valor )
/*****************************************************************************/
{
    unsigned int tmpo_acum = 0;

    tmpo_acum = GET_MEMORY_UNSIGNED( var );
    if ( tmpo_acum + valor >= USHRT_MAX ) { //esto es porque si se hacen dos tickets con más de 18hrs., 12min.,
        ADD_MEMORY_UINT( var, USHRT_MAX - tmpo_acum - 1 );  //y 16seg. de diferencia da overflow de variable de tipo uint. (porque se restan dos long)
        GRABAR_LOG_SISTEMA( "Overflow en variables de tiempo",LOG_VENTAS,1 );
    }
    else {
        ADD_MEMORY_UINT( var, valor );
    }
}

/*****************************************************************************/
void ACUMULAR_TIEMPOS( void )
/*****************************************************************************/
{
    ADD_MEMORY_UINT( __x_tmpo_reg_articulo, RAM_TMPO_REG_ARTICULO );
    ADD_MEMORY_UINT( __x_tmpo_pago, RAM_TMPO_PAGO );
    ADD_MEMORY_UINT( __x_tmpo_no_venta, RAM_TMPO_NO_VENTA );
    ADD_MEMORY_UINT( __x_tmpo_ocio, RAM_TMPO_OCIO_AUX );
    
}
/*****************************************************************************/
void CEREAR_TIEMPOS( void )
/*****************************************************************************/
{
    SET_MEMORY_INT( __ram_tmpo_reg_articulo, 0 );
    SET_MEMORY_INT( __ram_tmpo_pago, 0 );
    SET_MEMORY_INT( __ram_tmpo_no_venta, 0 );
    SET_MEMORY_INT( __ram_tmpo_ocio, 0 );
}

#define NO_ESPERA_RESPUESTA 0
#define ESPERA_RESPUESTA     1
#define ARCHIVOS_HISTORICOS 2

/*****************************************************************************/
void ALOJA_RESPUESTAS_FISCAL()
/*****************************************************************************/
{
    if( IMPRESORA_FISCAL == FISCAL ) {
        rta_fiscal = malloc( 100 );
    }
}
/*****************************************************************************/
char GRABAR_LOG_ACTIONS( void )
/*****************************************************************************/
{
	if( ConfIni.logPantallas == 0x05)
		return 'S';	
	return ( ConfIni.logPantallas );
}
/*****************************************************************************/
int VERICAR_CONF_SERVICIOS( void )
/*****************************************************************************/
{
    /*********************************************
     |      VALORES DE RETORNO (rta)             |
     |                                           |
     |  0 = valores configurados correctamente   |
     |  1 = valores mal configurados             |
     *********************************************/
    int rta = 0;
    if( config.pais != ARGENTINA ) {
        MENSAJE( "EL CONFIG PAIS DEBE SER ARGENTINA" );
        rta = 1;
    }
    /*   if( CUOTAS_HABILITADAS ){
          MENSAJE( "DEBE DESHABILITAR LAS CUOTAS EN TARJETAS" );
          rta = 1;
       }
       if( MENU_CUOTAS ){
          MENSAJE( "DEBE DESHABILITAR EL MENU CUOTAS EN TARJETAS" );
          rta = 1;
       }
    */
    if( AUTORIZACION_ON_LINE_HABILITADA ) {
        MENSAJE( "DEBE DESHABILITAR LA AUTORIZACION ON LINE EN TARJETAS" );
        rta = 1;
    }
    if( DONACION_DE_VUELTO ) {
        MENSAJE( "DEBE DESHABILITAR LA DONACION DE VUELTOS EN DONACIONES" );
        rta = 1;
    }
    if( UTILIZA_PERFILES ) {
        MENSAJE( "DEBE DESHABILITAR PEFILES EN MISCELANEAS" );
        rta = 1;
    }
    if( PEDIR_CODIGO_DE_FINANCIACION ) {
        MENSAJE( "DEBE DESHABILITAR PEDIR CODIGO DE FINANCIACION EN CUENTA CORRIENTE" );
        rta = 1;
    }
    if( PROTOCOLO_AUTORIZACION_TARJETA == _TCP_IP ) {
        MENSAJE( "EL PROTOCOLO DE AUTORIZACION DE TARJETAS DEBE SER POR ARCHIVO" );
        rta = 1;
    }
    if( !IMPRIME_FONDO_FIJO ) {
        MENSAJE( "DEBE HABLITAR LA IMPRESION DE FONDO FIJO" );
        rta = 1;
    }
    if( PEDIR_MOTIVO_EN_DEVOLUCIONES ) {
        MENSAJE( "DEBE DESHABLITAR PERDIR MOTIVO EN DEVOLUCIONES" );
        rta = 1;
    }
    if( PEDIR_CUENTA_BANCARIA ) {
        MENSAJE( "DEBE DESHABLITAR PEDIR CUENTA BANCARIA" );
        rta = 1;
    }
    if( USAR_MENU_BANCOS != 1 ) {
        MENSAJE( "DEBE SELECCIONAR TODOS EN MENU BANCOS" );
        rta = 1;
    }
    if( PEDIR_NUMERO_DE_CHEQUE ) {
        MENSAJE( "DEBE DESHABLITAR PEDIR NUMERO DE CHEQUE" );
        rta = 1;
    }
    if( PEDIR_DATOS_COMPRADOR_EN_CHEQUES ) {
        MENSAJE( "DEBE DESHABLITAR PEDIR DATOS COMPRADOR EN CHEQUES" );
        rta = 1;
    }
    if( rta == 1 ) {
        MENSAJE( "HAY ERRORES DE CONFIGURACION, CONFIGURE LA CAJA Y VUELVA A INICIAR" );
    }
    return ( rta );
}
/*****************************************************************************/
int MENU_CAJERO_SUPERVISOR( void )
/*****************************************************************************/
{
	int    rta = 0, columnas = 1;
	struct _cabmenu *cab;
	struct _menu items[] = {
		{"1) CAJERO", '1', 1},
		{"2) SUPERVISOR", '2', 2}
	};

	cab = ( struct _cabmenu * )malloc( sizeof( struct _cabmenu ) * columnas );
    if( cab != NULL ) {
		cab[0].nombre = "DESCRIPCION";
		cab[0].align = ALIGN_LEFT;
		cab[0].width = 30;
		rta = MENU( ST( S__INGRESAR_COMO__ ), items, 2, cab, columnas );
		free( cab );
    }

    return ( rta );
}
/*****************************************************************************/
void CONTROL_CANT_MEDIOS_SUBMEDIOS_FONDO_F_AUTO( int cajero, int *fondo_fijo_aut )
/*****************************************************************************/
{
	char    cod_ff_aut = 0;
	int   	ok1 = 0, i = 0, medio_f_fijo_auto[10];
			 
				
	memset( medio_f_fijo_auto, 0, sizeof( medio_f_fijo_auto ) );
	if(  /*USE_DB( AREA_CAJEROS, _CAJEROS_SIC, 
		( char* )&datos_cajero, sizeof( datos_cajero ), NULL, 0, 0 )*/
        OPEN_TABLE(T_CAJEROS, TT_ORIG ,( char* )&datos_cajero, sizeof( struct _cajeros ))== 0 ) {
        SELECT_TABLE( T_CAJEROS, TT_ORIG );
		SET_WHERE("COD_CAJERO = %i",cajero);
		RUN_QUERY(1);
		//GET_EQUAL( ( char * ) &cajero );
		if( FOUND2( ) ){
			cod_ff_aut = datos_cajero.cod_ff;
			ok1 = 1;
		}
		//CLOSE_DATABASE( AREA_CAJEROS );
        CLOSE_TABLE( T_CAJEROS, TT_ORIG );
	} else {
		MENSAJE_STRING( S_ERROR_ARCHIVO_CAJEROS );
	}
	/*if( ok1 && !USE_DB( AREA_AUX2, _D_F_FIJO_SIC, ( char * )&det_ff, 
			sizeof( struct d_f_fijo_sic ), NULL, 0, 0 ) ) {*/
	if( ok1 && OPEN_TABLE( T_DETALLE_FONDOFIJO, TT_ORIG ,( char* )&detalle_fondofijo,
			sizeof( struct _detalle_fondofijo ) ) == 0 ) {
	    
		/*ind_det_ff.cod_ff = cod_ff_aut;
		GET_GREATER_E( ( char* ) &ind_det_ff );*/
		SET_WHERE("COD_FF = '%i' AND COD_MEDIO >= %d AND COD_SUBMEDIO >= %i", cod_ff_aut, 0, 0 );
		RUN_QUERY(SI);

		if( FOUND2( )&& ( detalle_fondofijo.cod_ff == cod_ff_aut ) ) {
			while( !dbEOF() && *fondo_fijo_aut != -1 && ( detalle_fondofijo.cod_ff == cod_ff_aut ) ) {
				if( *fondo_fijo_aut ) {
					for( i = 0; i < *fondo_fijo_aut && *fondo_fijo_aut != -1; i++ ) {
						if( medio_f_fijo_auto[i] == detalle_fondofijo.cod_medio ) {
							*fondo_fijo_aut = -1;
						}
					}
				}
				if( *fondo_fijo_aut >= 0 && *fondo_fijo_aut < 10 ) {
					medio_f_fijo_auto[*fondo_fijo_aut] = detalle_fondofijo.cod_medio;
					*fondo_fijo_aut = *fondo_fijo_aut + 1 ;
				}
				//SKIP( 1 );
				 SKIP2( 1 );
			}
		}
	}
	//CLOSE_DATABASE( AREA_AUX2 );
	CLOSE_TABLE( T_DETALLE_FONDOFIJO, TT_ORIG );
	if( *fondo_fijo_aut > 10  ) {
		MENSAJE("NO PUEDE REALIZAR MAS DE 10 FONDO FIJO AUTOMATICO");
		*fondo_fijo_aut = 0;
	} else {
		if( *fondo_fijo_aut < 0 ) {
			MENSAJE("SOLO SE PERMITE 1 FONDO FIJO AUTOMATICO POR MEDIO");
			*fondo_fijo_aut = 0;
		} else {
			*fondo_fijo_aut = 1;
		}
	}
} 
/*****************************************************************************/
int ES_GERENTE( int cajero )
/*****************************************************************************/
{
	int       rta;

	if( IDENTIFICACION_USUARIO == ID_X_TIPO ) {
		rta = ( datos_cajero.tipo == GERENTE ) ? SI : NO;
	} else {
		rta = ( cajero < config.primer_supervisor ) ? NO : SI;
	}
	return ( rta );
}


/*****************************************************************************/
void CONTROLAR_TABLAS_OLD()
//controla si existe alguna tabla _old, en caso de existir, controla si existe la original
//si la original existe, elimina la old si la original no existe, renombra la old como original
/*****************************************************************************/
{
	int salir = 0;
	char table[50];
	table[0] = 0;
	while(DIRECT_DB_QUERY("show tables LIKE '%%_old'") && !salir){ //busca las tabla old
		GET_DATO(1,C_CHAR,(char *)table,sizeof(table));
		if(strlen(table)>0){
			table[strlen(table) - 4] = 0;	//saca el _old
			if(DIRECT_DB_QUERY("show tables LIKE '%s'",table) > 0){ //busca la original
				DIRECT_DB_QUERY("drop table if exists %s_old",table); //si la original existe, elimina la old
			}else{
				DIRECT_DB_QUERY( "ALTER TABLE `%s_old` RENAME `%s`", table, table ); //si la original no existe, renombra la old a original
			}
		}else{
			//no deberia salir nunca por aqui
			salir = 1;
		}
	}
}

/*****************************************************************************/
void ACTUALIZA_FECHA_Y_HORA_CONTROLADOR_FISCAL_EPSON( void )
/*****************************************************************************/
{
	char tmp[30];
	struct tm *fecha_hora_sistema;
	int tm_year = 0, tm_mon = 0, tm_mday = 0, tm_hour = 0, tm_min = 0, tm_sec = 0;

	time_t t;
	t = time(NULL);
	fecha_hora_sistema = localtime(&t);
	tm_year = fecha_hora_sistema->tm_year + 1900;
	tm_mon = fecha_hora_sistema->tm_mon + 1;
	tm_mday = fecha_hora_sistema->tm_mday;
	tm_hour = fecha_hora_sistema->tm_hour;
	tm_min = fecha_hora_sistema->tm_min;
	tm_sec = fecha_hora_sistema->tm_sec;
	memset( tmp, 0, sizeof(tmp) );

	if( VARIOS_CAMBIO_FECHA_Y_HORA_CF ) {
		PIDE_FECHA_Y_HORA_AL_CONTROLADOR_FISCAL_EPSON(  );
		if( fecha_cf.da_mon 
		   && ( ( tm_year >= ( fecha_cf.da_year + 2000 ) )
		   || ( tm_mon >= fecha_cf.da_mon )
		   || ( tm_mday >= fecha_cf.da_day )
		   || ( tm_hour != hora_cf.ti_hour )
		   || ( tm_min != hora_cf.ti_min ) ) ) {
			if( !Z_CANTIDAD_TICKETS && !X_CANTIDAD_TICKETS ) {
				SET_MEMORY_CHAR( __varios_cambio_fecha_y_hora_cf, 0 );
				if( tm_year > 2000 ) {
					tm_year -= 2000;
				} else {
					tm_year = 1;
				}
				_snprintf( tmp, sizeof( tmp )-1, "%02d%02d%02d%02d%02d%02d", tm_year, tm_mon, 
					tm_mday, tm_hour,tm_min, tm_sec );
				ESTABLECER_FECHA_HORA_FISCAL( tmp );
				//PIDE_FECHA_Y_HORA_AL_CONTROLADOR_FISCAL_EPSON(  );
			} else {
				GRABAR_LOG_SISTEMA_STRING
					( S_NO_PUEDE_SINCRONIZAR_FECHA_Y_HORA_DEL_CONTROLADOR_FISCAL_YA_HAY_TICKET
					,LOG_DEBUG,2);
			}
		} else {
			//Si fecha_cf.da_mon > 0 no hubo error en method getDate
			if( fecha_cf.da_mon ) {
				GRABAR_LOG_SISTEMA_STRING
					( S_FECHA_Y_HORA_DEL_SISTEMA_E_IMPRESORA_ESTAN_SINCRONIZADOS
					,LOG_DEBUG,2 );
			} 
		}
	} else {
		GRABAR_LOG_SISTEMA_STRING
			( S_YA_FUERON_SINCRONIZADO_EL_SISTEMA_CON_EL_CONTROLADOR_FISCAL
			,LOG_DEBUG,2 );
	}

}

/*****************************************************************************/
void PIDE_FECHA_Y_HORA_AL_CONTROLADOR_FISCAL_EPSON( void )
/*****************************************************************************/
{
	int       NO_USADO = 0;
	char      temporal[80];

	memset( temporal, 0,  sizeof( temporal ) );
	GRABAR_LOG_SISTEMA_STRING( S_PIDIENDO_FECHA_Y_HORA_AL_CONTROLADOR_FISCAL 
		,LOG_DEBUG,2 );
	OBTENER_FECHA_HORA_FISCAL( );
	_snprintf( temporal, sizeof( temporal )-1, 
		"Fecha CF:%02d/%02d/%02d Hora CF:%02d:%02d:%02d", fecha_cf.da_day, 
		fecha_cf.da_mon, fecha_cf.da_year, hora_cf.ti_hour, hora_cf.ti_min, 
		hora_cf.ti_sec ); 
	GRABAR_LOG_SISTEMA( temporal, LOG_DEBUG,2);
}

/****************************************************************************/
unsigned PIDE_NUEVO_DIA_OPERATIVO( void )
/****************************************************************************/
{
	unsigned  fecha;
	char      mensaje[30];

	_snprintf( mensaje, sizeof( mensaje )-1, "%s", " NUEVO DIA OPERATIVO " );
	//fecha = PIDE_FECHA_VTO( mensaje );//no tiene parametro
	fecha = PIDE_FECHA_VTO( );
	if( fecha == 0xffff || !fecha )
		fecha = 0;

	if( fecha ) {
		if( !( fecha > NRO_DIA_OPERATIVO ) ) {
			MENSAJE_ID_CON_PAUSA_CAJA( S_FECHA_NO_MENOR_AL_DIA_OPERATIVO );
			fecha = 0;
		} else {
			if( DIF_DIAS( fecha, NRO_DIA_OPERATIVO ) > 30 ) {
				MENSAJE_STRING( S_FECHA_EXCEDIDA );
				if( !( SOLICITAR_CLAVES( 26 - ancho_panel / 2, 22, NO, SI, SI,
                     ST( S_FECHA_EXCEDIDA ), NULL, RAM_COD_FUNCION, SI ) ) )
					fecha = 0;
			}
		}

	}
	BORRAR_MENSAJE(  );

   return ( fecha );
}
/****************************************************************************/
int  VERIFICA_DIFERENCIA_DIA_OPERATIVO_FECHA_ACTUAL( void )
/****************************************************************************/
{int rta = SI;
if( NRO_DIA_OPERATIVO != _GET_FECHA_DOS() ){
		 char nuevo_dia_operativo,aux2[40],aux[80];
		memset( aux, 0, sizeof( aux ) );
		memset( aux2, 0, sizeof( aux2 ) );
		
		sprintf(&aux2[strlen(aux2)]," Dia Oper.:");
		DTOC( NRO_DIA_OPERATIVO, &aux2[strlen(aux2)]);
		sprintf(&aux2[strlen(aux2)]," vs ");
		sprintf(&aux2[strlen(aux2)]," Fecha:");
		DTOC( _GET_FECHA_DOS(), &aux2[strlen(aux2)] );
			   
		if(  NRO_DIA_OPERATIVO > _GET_FECHA_DOS()  ){
			_snprintf( aux, sizeof(aux)-1, "<< Día Operativo mayor a Fecha [%s] >>",aux2);
			MENSAJE_SIN_SONIDO( aux,10 );
			rta = SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, rta,
                               "Clave Autoriza Dia Operativo.", aux, _F_SUP_AUT_DIA_OPERATIVO, SI ) ;
		}else{
						_snprintf( aux, sizeof(aux)-1,"INFORME hay Diferencia[%s](Pres.Tecla)",aux2);
			MENSAJE_TECLA( aux ); 
		}
		glog(aux,LOG_INCONDICIONAL,0);		
		memset( aux, 0, sizeof( aux ) );
		memset( aux2, 0, sizeof( aux2 ) );
		BORRAR_MENSAJE();
		if( ! rta ){
			MENSAJE_SIN_SONIDO_STRING( S_LOGIN_NO_AUTORIZADO );
			BORRAR_MENSAJE();
		}
			
	}
return rta;
}