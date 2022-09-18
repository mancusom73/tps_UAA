#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <abort.h> 
#include <aarch.h>
#ifdef INVEL_W
#include <io.h>
#else
#include <sys/io.h>
#endif
#include <btrv.h> 
#include <b_mens.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <cr_disco.h> 
#include <dbrouter.h> 
#include <dd.h> 
#include <dec_mes.h> 
#include <getch.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <lista.h> 
#include <malloc.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <netcom.h> 
#include <pagos.h> 
#include <pago_tar.h> 
#include <tarjetas.h> 
#include <tcp_ip.h> 
#include <tkt.h> 
#include <t_file.h> 
//#include <vgagraf.h> 
#include <_cr1.h> 
#include <db.h>
#include <deftables.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifdef INVEL_W
#include <dos.h>
#else
#endif
#include <sys/stat.h>
#define COMPILAR_NET_COM
#define ES_TARJETA (nodo < 9)

struct _nodos 
{
	BINARY status;				//0
	BINARY cod_nodo;			//1
	char nombre[21];			//2
	INT16 time_out_disponible;  //23
	BINARY fin;			//25
};
struct _transac_sic *transac;
struct _ind2_transac ind2_transac;
struct _ind3_transac ind3_transac;
static char protocolo;
static void MARCAR_COMO_ENVIADO();
static int _ENVIAR_TRANSACCIONES_PENDIENTES( int nodo );
/****************************************************************************/
int ENVIAR_TRANSACCION( int nodo, int operacion, double importe, long comprobante, long *nro,
                        long *autorizacion, long anulacion )
/****************************************************************************/
{
    struct _transaccion tran;
    long nro_tran;
    int ok = 0, cant, k = 0, seguir = 1;
    char aux_track2[50],nomip[20];
    #ifdef COMPILAR_NET_COM
    SET_MEMORY_INT( __ram_resultado_transac, -1 );
    /*----------------- Define el protocolo a utilizar -----------------*/
    protocolo = ( nodo == 9 ) ? TCP_IP : T_FILE;
    strcpy( nomip, NOM_CANAL_TCP_IP );
    if( !ABRIR_CANAL( protocolo, _CANAL_TARJETAS, nomip, _FORMATO_MENSAJE_INVEL ) ) {
        MENSAJE_STRING( S_ERROR_ABRIENDO_CANAL );
        return ( 0 );
    }
    if( _ENVIAR_TRANSACCIONES_PENDIENTES( nodo ) ) {
        nro_tran = CALCULA_NRO_TRANSACCION();
        /*----------------- Graba la transaccion -----------------*/
        memset( &tran, 0, sizeof( tran ) );
        tran.clase = Normal;
        tran.operacion = operacion;
        tran.modo = OnLine;
        tran.fecha_operacion = GET_FECHA_DOS();
        tran.hora_operacion = GET_HORA_DOS();
        tran.caja = NRO_CAJA;
        tran.cajero = NRO_CAJERO;
        tran.importe_total = importe;
        tran.comprobante = comprobante;
        if( !ES_TARJETA ) {
            tran.cuotas = NRO_COD_CUOTA;
            sprintf( tran.cuenta, "%li", RAM_NRO_CLIENTE );
        }
        else {
            /*-------------------- Tarjetas de credito -----------------------*/
            tran.nro_transaccion = NRO_EVENTO;
            tran.terminal = VAR_TARJETAS_TERMINAL;
            //  tran.lote             = ;
            tran.nodo = nodo;
            tran.importe_entrega = VAR_TARJETAS_IMPORTE_ENTREGA;
            tran.moneda = MONEDA_EXTENDIDA();
            tran.cuotas = VAR_TARJETAS_CUOTAS;
            tran.primer_vto = VAR_TARJETAS_PRIMER_VTO;
            tran.modo_ingreso = VAR_TARJETAS_LECTURA_BANDA;
            tran.fecha_expiracion = DECREMENTAR_MES( VAR_TARJETAS_FECHA_VTO );
            tran.marca = VAR_TARJETAS_TARJETA;
            tran.boletin = VAR_TARJETAS_NRO_BOLETIN;
            tran.cvv = VAR_TARJETAS_CVV;
            tran.tipo_cuenta = VAR_TARJETAS_TIPO_CUENTA;
            tran.cod_autorizacion = atol( VAR_TARJETAS_AUTORIZACION );
            tran.cod_vigencia = VAR_TARJETAS_COD_VIGENCIA;
            tran.fecha_original = GET_FECHA_DOS();  // OJO guardar la original
            tran.comprobante_original = anulacion;
            strcpy( tran.numero_comercio, VAR_TARJETAS_NRO_COMERCIO );
            strcpy( tran.plan, VAR_TARJETAS_TABLA );
            memcpy( tran.pin, VAR_TARJETAS_PIN, sizeof( tran.pin ) );
            memcpy( tran.nro_referencia, VAR_TARJETAS_NRO_REF, sizeof( tran.nro_referencia ) );
            if( VAR_TARJETAS_LECTURA_BANDA == 1 ) {
                memset( tran.cuenta, 0, 40 );
                cant = _VAR_TARJETAS_TRACK2( 0 ) - 1;
                memcpy( aux_track2, VAR_TARJETAS_TRACK2, sizeof( aux_track2 ) );
                memcpy( tran.cuenta, &aux_track2[1], cant );
            }
            else {
                strcpy( tran.cuenta, GET_NUMERO_TARJETA_PRIVADO( ));
            }
            // Esto debiera ser configurable
            /* else if( VAR_TARJETAS_TARJETA == _CABAL_VIEJA ) { strcpy(
                     * tran.cuenta, "3121" ); strcat( tran.cuenta,
                     * VAR_TARJETAS_NUMERO ); } else { strcpy( tran.cuenta,
                     * VAR_TARJETAS_NUMERO ); } */
        }
        if( AGREGAR_TRANSACCION( &tran, nro_tran, nodo ) ) {
            *nro = transac->nro;
            ok = _ENVIAR_TRANSACCIONES_PENDIENTES( nodo );
            if( OPERACION_NO_REVERSABLE( transac->transaccion.operacion ) ) {
                if( !transac->enviado ) {
                    transac->enviado = 1;
                    #ifdef INVEL_L
                    BEGIN_TRANSACTION();
                    #endif
                    UPDATE();
                    #ifdef INVEL_L
                    END_TRANSACTION();
                    #endif
                }
            }
            else if( transac->transaccion.clase != Reverso ) {
                /*---------------- Genera el reverso si es necesario ---------------*/
                if( transac->error && transac->enviado ) {
                    REVERSAR_TRANSACCION( nodo, *nro );
                    ok = 0;
                }
                /*----------------- En caso de error local --------------------*/
                else if( transac->error == ERROR_LOCAL ) {
                    strncpy( transac->rta.mensaje, ST( S_ERROR_DE_COMUNICACION ),
                             sizeof( transac->rta.mensaje ) );
                    MOSTRAR_RESULTADO( transac->rta.mensaje, NO );
                    #ifdef INVEL_L
                    BEGIN_TRANSACTION();
                    #endif
                    UPDATE();
                    #ifdef INVEL_L
                    END_TRANSACTION();
                    #endif
                    seguir = 0;
                    if( operacion == Pago ) {
                        MARCAR_COMO_ENVIADO();
                        ok = SI;
                    }
                    else {
                        ok = AUTORIZAR_TRANSACCION( autorizacion, ok );
                    }
                }
            }
        }
        if( ok && seguir ) {
            GET_EQUAL( ( char* )nro );
            /*-------------- Procesa la respuesta ---------------*/
            SET_MEMORY_INT( __ram_resultado_transac, transac->rta.cod_rta );
            SET_MEMORY_INT( __var_tarjetas_primer_vto, transac->rta.primer_vto );
            if( transac->rta.cod_rta == Aceptada ) {
                if( ES_TARJETA ) {
                    SET_MEMORY_CHAR( __var_tarjetas_on_line, SI );
                    SET_MEMORY( __var_tarjetas_nro_ref, transac->rta.nro_referencia );
                    SET_MEMORY_CHAR( __var_tarjetas_aut_on_line, SI );
                    SET_MEMORY( __var_tarjetas_mensaje, transac->rta.mensaje );
                }
                if( transac->rta.cod_perfil ) {
                    SET_MEMORY_INT( __ram_perfil, transac->rta.cod_perfil );
                }
                else {
                    SET_MEMORY_INT( __ram_perfil, 0 );
                }
            }
            else {
                ok = NO;
            }
            if( transac->rta.mensaje[0] ) {
                k = MOSTRAR_RESULTADO( transac->rta.mensaje, ( transac->rta.cod_accion == 3 ) );
            }
            if( transac->rta.cod_accion == 3 && k == 13 ) {
                transac->rta.cod_accion = 1;
            }
            *autorizacion = transac->rta.cod_autorizacion;
            switch( transac->rta.cod_accion ) {
                case 1:
                    /*------- Pedir autorizacion --------*/
                    if( transac->transaccion.operacion != Consulta_de_Estado ) {
                        if( ES_TARJETA ) {
                            ok = AUTORIZAR_TRANSACCION( autorizacion, 0 );
                        }
                        else {
                            *autorizacion = PAGOS_PIDE_AUTORIZACION( ST( S_AUTORIZACION ) );
                        }
                    }
                    break;
                case 2:
                    /*------- Mostrar importe aceptado --------*/
                    MOSTRAR_IMPORTE_ACEPTADO();
                    break;
                case 4:
                    /*----------- Pedir PIN nuevamente --------*/
                    #ifdef COMPILAR_PINPAD
                    //PIDE_CLAVE_EN_PINPAD( SI );
                    #endif
                    ok = 0;
                    break;
                case 5:
                    /*----------- Off Line si se puede ----------*/
                    if( !EN_CUOTAS() && LIMITE2_TARJETA( VAR_TARJETAS_TARJETA ) != 0
                     && !VAR_TARJETAS_UNICAMENTE_OL
                     && importe <= LIMITE2_TARJETA( VAR_TARJETAS_TARJETA ) ) {
                        ok = ENVIAR_COMO_OFF_LINE();
                    }
                    break;
            }
        }
    }
    if( !CERRAR_CANAL( protocolo, _CANAL_TARJETAS ) ) {
        MENSAJE_STRING( S_ERROR_AL_CERRAR_CANAL );
    }
    return ( ok );
    #else
    return 0;
    #endif
}
#ifdef COMPILAR_NET_COM
/****************************************************************************/
void MOSTRAR_IMPORTE_ACEPTADO()
/****************************************************************************/
{
    char mensaje[81];
    sprintf( mensaje, ST( S_CUENTA_AUTORIZADA_HASTA__7_2F__ ), transac->rta.importe_autorizado );
    MOSTRAR_RESULTADO( mensaje, NO );
}
/*****************************************************************************/
int MOSTRAR_RESULTADO( char *mensaje, int autoriza )
/*****************************************************************************/
{
    int k = 0;
    _ATRIBUTO = 15;
    MOSTRAR_CADENA( 1, 25, mensaje );
    if( autoriza ) {
        MOSTRAR_CADENA( 46, 25, ST( S__ESC__RECHAZA_O_ENTER__AUTORIZA_ ) );
        while( k != 27 && k != 13 ) {
            k = GETCH();
        }
    }
    else {
        MOSTRAR_CADENA( 64, 25, ST( S__PRESIONE_ESC_ ) );
        while( k != 27 ) {
            k = GETCH();
        }
    }
    return ( k );
}
#endif
/****************************************************************************/
int REVERSAR_TRANSACCION( int nodo, long nro )
/****************************************************************************/
{
    int ok = 0;
    #ifdef COMPILAR_NET_COM
    long nro_new;
    struct _transaccion tran;
    nro_new = CALCULA_NRO_TRANSACCION();
    SELECT( AREA_TRANSAC );
    SET_ORDER( 1 );
    GET_EQUAL( ( char* )&nro );
    if( FOUND() ) {
        transac->transaccion.clase = Reverso;
        memcpy( &tran, &transac->transaccion, sizeof( tran ) );
        if( AGREGAR_TRANSACCION( &tran, nro_new, nodo ) ) {
            _ENVIAR_TRANSACCIONES_PENDIENTES( nodo );
            ok = 1;
        }
    }
    #endif
    return ( ok );
}
/****************************************************************************/
int ABRIR_TRANSAC()
/****************************************************************************/
{
    if( transac == NULL ) {
        transac = ( struct _transac_sic * )malloc( sizeof( struct _transac_sic ) );
    }
    return ( USE_DB( AREA_TRANSAC, _TRANSAC_SIC, ( char* )transac, sizeof( struct _transac_sic ),
                     NULL, 0, 0 ) );
}
#ifdef COMPILAR_NET_COM
/****************************************************************************/
long CALCULA_NRO_TRANSACCION()
/****************************************************************************/
{
    long nro,actual;
    SELECT( AREA_TRANSAC );
    SET_ORDER( 1 );
    if( EMPTY() ) {
        nro = 1;
    }
    else {
        actual = transac->nro;
        GO( BOTTOM );
        nro = transac->nro + 1;
        GET_EQUAL( ( char* )&actual );
    }
    return ( nro );
}
/****************************************************************************/
int AGREGAR_TRANSACCION( struct _transaccion *tran, long nro, int nodo )
/****************************************************************************/
{
    int rta = 0;
    memset( transac, 0, sizeof( struct _transac_sic ) );
    transac->id.dispositivo = 0;
    transac->id.version = VERSION.version;
    transac->id.sub_version = VERSION.sub_version;
    transac->id.revision = VERSION.release;
    transac->id.terminal = NRO_CAJA;
    transac->nodo = nodo;
    transac->nro_z = NRO_Z;
    transac->nro = nro;
    memcpy( &transac->transaccion, tran, sizeof( transac->transaccion ) );
    #ifdef INVEL_L
    BEGIN_TRANSACTION();
    #endif
    if( INSERT() == 0 ) {
        rta = 1;
    }
    #ifdef INVEL_L
    END_TRANSACTION();
    #endif
    return ( rta );
}
/****************************************************************************/
void ENVIAR_TRANSACCIONES_PENDIENTES()
/****************************************************************************/
{
    int h;
    for( h = 0;h < 10;h++ ) {
        _ENVIAR_TRANSACCIONES_PENDIENTES( h );
    }
}
/****************************************************************************/
static int _ENVIAR_TRANSACCIONES_PENDIENTES( int nodo )
/****************************************************************************/
{
    int error = 0, ok = 1;
    /*----------------- Define el protocolo a utilizar -----------------*/
    /*---------------- Recorre los registros pendientes ----------------*/
    SELECT( AREA_TRANSAC );
    SET_ORDER( 2 );
    ind2_transac.nodo = (char)nodo;
    ind2_transac.enviado = NO;
    ind2_transac.nro = 0;
    GET_GREATER_E( ( char* )&ind2_transac );
    while( transac->nodo == ind2_transac.nodo && !transac->enviado && !BtrvEOF() && !error ) {
        if( transac->error != ERROR_LOCAL || transac->transaccion.modo == OffLine ) {
            ok = 0;
            error = ENVIAR_PAQUETE( protocolo, nodo );
            if( !error || ( error != ERROR_LOCAL && transac->transaccion.clase != Reverso ) ) {
                transac->enviado = SI;
            }
            if( !error ) {
                ok = 1;
            }
            transac->error = error;
            #ifdef INVEL_L
            BEGIN_TRANSACTION();
            #endif
            UPDATE();
            #ifdef INVEL_L
            END_TRANSACTION();
            #endif
        }
        if( !error ) {
            SKIP( 1 );
        }
    }
    return ( ok );
}
/****************************************************************************/
int ENVIAR_PAQUETE( int protocolo, int nodo )
/****************************************************************************/
{
    struct _paquete paquete;
    struct _paquete_rta rta;
    int ok;
    memset( &rta, 0, sizeof( rta ) );
    memcpy( &paquete.id, &transac->id, sizeof( paquete.id ) );
    memcpy( &paquete.transaccion, &transac->transaccion, sizeof( paquete.transaccion ) );
    ok = _ENVIAR_PAQUETE( protocolo, ( char* )&paquete, sizeof( paquete ), ( char* )&rta,
                          sizeof( rta ), TIMEOUT_NODO( ( char )nodo ), SI, _CANAL_TARJETAS );
    memcpy( &transac->id_rta, &rta.id, sizeof( transac->id_rta ) );
    memcpy( &transac->rta, &rta.rta, sizeof( transac->rta ) );
    return ( ok );
}
/****************************************************************************/
int ENVIAR_COMO_OFF_LINE()
/****************************************************************************/
{
    int ok = 1;
    /*----------------- Marca la transaccion como enviada -----------------*/
    MARCAR_COMO_ENVIADO();
    /*--------------- Genera un nuevo registro pero OffLine ---------------*/
    // CMR no utiliza OFF-LINE //
    /* transac.nro     = CALCULA_NRO_TRANSACCION(); // hacer esto primero
     * siempre memset( &transac.rta, 0, sizeof(transac.rta) );
     * transac.transaccion.modo            = OffLine;
     * transac.transaccion.fecha_operacion = GET_FECHA_DOS();
     * transac.transaccion.hora_operacion  = GET_HORA_DOS(); transac.enviado
     * = 0; transac.error   = 0; INSERT(); */
    /*----------------------- Intenta enviarla ----------------------------*/
    /* ok = _ENVIAR_TRANSACCIONES_PENDIENTES( transac.nodo ); */
    return ( ok );
}
/****************************************************************************/
int OPERACION_NO_REVERSABLE( int op )
/****************************************************************************/
{
    int rta = 0;
    if( op == Consulta_de_Saldo || op == Consulta_de_Estado ) {
        rta = 1;
    }
    return ( rta );
}
/****************************************************************************/
static void MARCAR_COMO_ENVIADO()
/****************************************************************************/
{
    SELECT( AREA_TRANSAC );
    transac->enviado = 1;
    #ifdef INVEL_L
    BEGIN_TRANSACTION();
    #endif
    UPDATE();
    #ifdef INVEL_L
    END_TRANSACTION();
    #endif
}
/****************************************************************************/
int AUTORIZAR_TRANSACCION( long *autorizacion, int ok )
/****************************************************************************/
{
    if( !VAR_TARJETAS_UNICAMENTE_OL ) {
        if( PIDE_AUTORIZACION_TARJETA( 0.0 ) ) {
            *autorizacion = atol( VAR_TARJETAS_AUTORIZACION );
            ok = ENVIAR_COMO_OFF_LINE();
        }
        else if( ANULAR_TICKET_SI_NO_SE_AUTORIZA ) {
            SET_MEMORY_CHAR( __ram_anular_ticket, _ANUL_TICKET_NO_SE_AUTORIZA );
        }
    }
    return ( ok );
}
#endif
/****************************************************************************/
void COMIENZO_NET_COM()
/****************************************************************************/
{
    #ifdef COMPILAR_NET_COM
    transac = (struct _transac_sic *)MALLOC( sizeof( struct _transac_sic ) );
    if( !transac ) {
        ABORT( "ERROR AL CARGAR NET COM" );
    }
    #endif
}
/****************************************************************************/
void FIN_NET_COM()
/****************************************************************************/
{
    #ifdef COMPILAR_NET_COM
    if( transac ) {
        free( transac );
        transac = NULL;
    }
    #endif
}
/***********************************************************************/
int TIMEOUT_NODO( char nodo )
/***********************************************************************/
{
    struct _nodos nodos;
    int rta = 20, area_ant = 0/*area*/, tipo_ant = 0;
    //area = SELECTED();
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

	if( OPENED( T_NODOS, TT_ORIG ) ) {
		CLOSE_TABLE( T_NODOS, TT_ORIG );
	}
	if(	OPEN_TABLE(T_NODOS, TT_ORIG,( char* )&nodos, sizeof( nodos ))== 0 ) {
		//SELECT_TABLE( T_NODOS, TT_ORIG );
		SET_WHERE("COD_NODO = '%i'",nodo);
        //GET_EQUAL( &nodo );
		RUN_QUERY(NO);
        if( FOUND2() ) {
            rta = nodos.time_out_disponible;
        }
        CLOSE_TABLE( T_NODOS, TT_ORIG );
    }
    //SELECT( area );
	SELECT_TABLE( area_ant, tipo_ant );

    return ( rta );
}
/****************************************************************************/
int ABRIR_CANAL( int protocolo, int canal, char *ip, int formato_mensaje )
/****************************************************************************/
{
    int rta = 1;
    switch( protocolo ) {
        case TCP_IP:
            //FIN_BUSCA_TARJETA(  );        
            rta = ABRIR_CANAL_TCP_IP( canal, ip, formato_mensaje );
            break;
    }
    return ( rta );
}
/****************************************************************************/
int CERRAR_CANAL( int protocolo, int canal )
/****************************************************************************/
{
    int rta = 1;
    switch( protocolo ) {
        case TCP_IP:
            rta = CERRAR_CANAL_TCP_IP( canal );
            //INICIALIZAR_BUSCA_TARJETA(  );
            break;
    }
    return ( rta );
}
/****************************************************************************/
int _ENVIAR_PAQUETE( int protocolo, char *paquete, int l_paquete, char *rta, int l_rta,
                     int time_out_disponible, char espera_rta, int canal )
/****************************************************************************/
{
    int error = 1;

#ifdef COMPILAR_LOGS_VIA_TCP
   if(CAMBIAR_PUERTO != 0 )
#endif
    MENSAJE_SIN_SONIDO_STRING( S_ENVIANDO_TRANSACCION );
    //MENSAJE_STRING( S_ENVIANDO_TRANSACCION );
    DEBUG_PAQUETE( "ENVIO.BIN", paquete, l_paquete );
    switch( protocolo ) {
        case TCP_IP:
            error = _ENVIAR_PAQUETE_TCP_IP( paquete, l_paquete, rta, l_rta, time_out_disponible,
                                            espera_rta, canal );
            break;
        case T_FILE:
            error = _ENVIAR_PAQUETE_T_FILE( paquete, l_paquete, rta, l_rta, time_out_disponible );
            break;
		case NAPSE:
            error = _ENVIAR_PAQUETE_NAPSE( paquete, l_paquete, rta, l_rta, time_out_disponible );
            break;
    }
    if( !error ) {
        DEBUG_PAQUETE( "RTA.BIN", rta, l_rta );
    }
    BORRAR_MENSAJE();
    return ( error );
}
/****************************************************************************/
void DEBUG_PAQUETE( char *archivo, char *paquete, int l_paquete )
/****************************************************************************/
{
    int handle;
    #ifdef INVEL_L
    if( MODO_DEBUG > 1 ) {
        handle = creat( archivo, S_IFREG );
        if( handle > 0 ) {
            write( handle, paquete, l_paquete );
            close( handle );
        }
    }
    #else
    if( MODO_DEBUG > 1 ) {
        //handle = _creat( archivo, S_IFREG );
		handle = _creat( archivo,  _S_IREAD | _S_IWRITE );
        if( handle > 0 ) {
            _write( handle, paquete, l_paquete );
            _close( handle );
        }
    }
    #endif
}
#ifdef COMPILAR_LOGS_VIA_TCP
//ESTA EN LA VERSION dos ECU.374
/****************************************************************************/
void LOGS_VIA_TCP(char *cadena  )
/****************************************************************************/
{
   if(HABILITA_LOGS_TCP)
   {  int  error = 0,pant = 0;
      //#define _CANAL_LOGS      3
   //int CAMBIAR_PUERTO
      //char IP_DEMO_LOGS[16];
      //   int PUERTO_IP_DEMO_LOGS = 0;CAMBIAR_PUERTO
      //CERRAR_CANAL_TCP_IP( _CANAL_TARJETAS );

      char      aux[100+1]; //(#define SIZE_BUFFER_JOURNAL 100
      int      canal = _CANAL_LOGS;//_CANAL_TARJETAS;//_CANAL_LOGS
   /*********************************************************************************/
   /*********************************************************************************/
   /*********************************************************************************/
//  sprintf(aux,"1 Enviar: CAn%i, Pto:%i, IP %s",CANAL_ABIERTO_DEMO_LOGS ,PUERTO_IP_DEMO_LOGS ,IP_DEMO_LOGS);
//      mmmm(aux);
//      glog(aux);

  if( PUERTO_IP_DEMO_LOGS == 0 && CANAL_ABIERTO_DEMO_LOGS ){
            CAMBIAR_PUERTO = 1;
         	CERRAR_CANAL( _TCP_IP, canal );
            CAMBIAR_PUERTO = 0;
      }

   // sprintf(IP_DEMO_LOGS,"172.30.1.144");
   // PUERTO_IP_DEMO_LOGS = 6544;

      memset(aux, 0 , strlen(aux) );


   /*********************************************************************************/
   /*********************************************************************************/
   /*********************************************************************************/



      if(!PUERTO_IP_DEMO_LOGS ){
         {	int       arriba = 9/*8*/, izquierda = 13,margen = 4, atributo = 0 ;
      //         char mensajes[40];
            pant = SAVE_SCREEN( 1, 5, 80 - ancho_panel, 25 );
            BOX_TITULADA( izquierda, arriba - 1, izquierda + 56, arriba +
                  9,"Servidor Remoto"
         /* "Reintegro Percepcion IIBB Bs As"*/, 127 );
                  memset(aux,0,40);
                  sprintf( aux, "DIRECCION IP SERVIDOR :");
            arriba ++ ;
                  MOSTRAR_CADENA( izquierda + margen, ++arriba  ,aux /*"CUIT del cliente:"*/ );
                  PEDIR_CADENA( izquierda + margen + strlen(aux) + 1, arriba , 16, IP_DEMO_LOGS,NULL, NO);
            if(LASTKEY() == 27 ){
               PUERTO_IP_DEMO_LOGS = 0;
               sprintf(IP_DEMO_LOGS,"               ");
            }else{
               TRIM(IP_DEMO_LOGS);
                     memset(aux,0,40);
                     sprintf( aux, "PUERTO DEL SERVIDOR :");
               arriba += 3;
                     MOSTRAR_CADENA( izquierda + margen, arriba  ,aux /*"CUIT del cliente:"*/ );
               PUERTO_IP_DEMO_LOGS = ( int ) PEDIR_ENTERO( izquierda + margen + strlen(mensajes) + 1, arriba , 6);

               if(LASTKEY() == 27 ){
                  PUERTO_IP_DEMO_LOGS = 0;
                  sprintf(IP_DEMO_LOGS,"               ");
               }else{
                     memset(aux,0,strlen(aux));
        //             sprintf( mensajes, "       ---  Version Beta ---         ");
          //           arriba += 2;
            //         MOSTRAR_CADENA( izquierda + margen, arriba  ,mensajes /*"CUIT del cliente:"*/ );
      //               sprintf( mensajes, "La configuracion se realizara en CONFTPV");
      //               MOSTRAR_CADENA( izquierda + margen, ++arriba  ,mensajes /*"CUIT del cliente:"*/ );

               }
            }
         if( pant )
            RESTORE_SCREEN( pant );
         }
      }
  //    sprintf(aux,"2 Enviar: CAn%i, Pto:%i, IP %s",CANAL_ABIERTO_DEMO_LOGS ,PUERTO_IP_DEMO_LOGS ,IP_DEMO_LOGS);
  //    mmmm(aux);
  //    glog(aux);

   /*********************************************************************************/
   /*********************************************************************************/
   /*********************************************************************************/
      if( !CANAL_ABIERTO_DEMO_LOGS && PUERTO_IP_DEMO_LOGS > 0 && strlen( IP_DEMO_LOGS ) > 0 ){
            CAMBIAR_PUERTO = 1;

         //  CANAL_ABIERTO_DEMO_LOGS = ABRIR_CANAL_NETCOM2( TCP_IP, 2 );//
            {
               	int       cantidad = 0,reintentos = 2;
               	int       abrio_canal = 0;
               	int       rta = 1;

               	do {
   //            	   rta = ABRIR_CANAL( TCP_IP, _CANAL_TARJETAS, NOM_CANAL_TCP_IP );
               		rta = ABRIR_CANAL( _TCP_IP, canal, IP_DEMO_LOGS );
               		if( !rta ) {
               			if( reintentos ) {
                     				sprintf( aux, "Error abrir canal,int %d,C.%i", cantidad + 1,canal );
               				GRABAR_LOG_SISTEMA( aux );
//                     				mmms(aux);
               			}
               		} else{
               			CANAL_ABIERTO_DEMO_LOGS = rta;
               			abrio_canal = 1;
               		}
               		cantidad++;
               	} while( cantidad < reintentos && !abrio_canal );
               		//canal_tarj_abierto = CANAL_ABIERTO_DEMO_LOGS;

               }


   //         CANAL_ABIERTO_DEMO_LOGS = ABRIR_CANAL( _TCP_IP, canal, IP_DEMO_LOGS );

            CAMBIAR_PUERTO = 0;
    //       sprintf(aux,"aux : %i,%i", CANAL_ABIERTO_DEMO_LOGS,strlen( cadena ));
    //       mmmm(aux);
    //       glog(aux);
         }
//      sprintf(aux,"3 Enviar: CAn%i, Pto:%i, IP %s",CANAL_ABIERTO_DEMO_LOGS ,PUERTO_IP_DEMO_LOGS ,IP_DEMO_LOGS);
//      mmmm(aux);
//      glog(aux);


            memset(aux,0,strlen(aux));
      if( CANAL_ABIERTO_DEMO_LOGS ){
   //      sprintf(aux,"long %i ",strlen(cadena));
   //      glog(aux);

         memset(aux, 0 , strlen(aux) );
         error = _ENVIAR_PAQUETE( ( int ) TCP_IP, ( char * )  cadena, strlen( cadena ),	//envio
   				( char * ) &aux ,strlen( aux ),30, NO, ( int ) canal );
               sprintf(aux,"POS SEND: `%s´,",cadena);
   //      mmmm(aux);
               glog(aux);

   		if( error ){
//   		   CANAL_ABIERTO_DEMO_LOGS = 0;
//            //CAMBIAR_PUERTO = 1;
//         	//CERRAR_CANAL( _TCP_IP, canal );
//            //CAMBIAR_PUERTO = 0;
                  //glog("ERROR");
         }

      }
    //  sprintf(aux,"4 Enviar: CAn%i, Pto:%i, IP %s",CANAL_ABIERTO_DEMO_LOGS ,PUERTO_IP_DEMO_LOGS ,IP_DEMO_LOGS);
    //  mmmm(aux);
    //  glog(aux);
      if( CANAL_ABIERTO_DEMO_LOGS ){
   	   CANAL_ABIERTO_DEMO_LOGS = 0;
         CAMBIAR_PUERTO = 1;
         CERRAR_CANAL( _TCP_IP, canal );
         CAMBIAR_PUERTO = 0;
      }
            sprintf(aux,"canal:%i: %i ",canal,CANAL_ABIERTO_DEMO_LOGS);
            glog(aux);
   }
}
#endif
