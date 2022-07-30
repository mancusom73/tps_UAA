#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <archivos.h> 
#include <b_mens.h> 
#include <cio.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <driver.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <netcom2.h> 
#include <pinpad.h> 
#include <tkt.h> 
#include <_cr1.h> 
#include <path.h> 
#include <actions.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_PINPAD
#if !defined(INVEL_W) && !defined(INVEL_L)
#endif
#undef MK_FP
static int PINPAD_PEDIR_PIN( char *nro_tarjeta, char *working_key, char *pin, int l_pin );
static int _PINPAD_PEDIR_PIN( char *nro_tarjeta, char *working_key, char *pin, int l_pin );
int COMANDO_PINPAD_SET(int action, char *buffer, int len );
int COMANDO_PINPAD_GET(int action, char *buffer, int len );
int PINPAD_LEER_pin_WORKING_KEY( char *working_key );
int TARJETA_DEBITO = 0;
void SETEAR_DEBITO_POS( int nro) ;
/***************************************************************************/
int INIT_PINPAD()
/***************************************************************************/
{
    int error = 0;
    if( UTILIZA_PINPAD ) {
/*        DRIVER( _DRV_pinpad, _PIN_fin, 0, &error, sizeof( error ) );
        DRIVER( _DRV_pinpad, _PIN_inicializacion, 0, &error, sizeof( error ) );
		*/
//		PINPAD_VERIFICAR_ONLINE();
  //      PINPAD_CANCEL_SESSION();
        //if( !error )
        //PINPAD_CARTEL(  );
    }
    return ( !error );
}
/***************************************************************************/
int PINPAD_AUTORIZAR_COMPRA( char celda_pinpad, char *nro_tarjeta, char tipo_cuenta, char *pin,
                             int l_pin, int confirmar_importe, double importe )
/***************************************************************************/
{
    char _importe[40], *c_tipo_cuenta[] = {"","C.AHORRO","CTA.CTE."};
    char working_key[9];
    int rta = 0;

	memset(working_key,0, sizeof(working_key));
	memset(_importe,0, sizeof(_importe));


    if( PINPAD_PEDIR_WORKING_KEY() ) {
        //MENSAJE_STRING( S_UTILIZE_EL_PINPAD );
	//	MENSAJE_CON_PAUSA_CAJA( ST(S_UTILIZE_EL_PINPAD), 40 ); //seran 3 segundos
        rta = PINPAD_CANCEL_SESSION();
        if( rta && PINPAD_LEER_WORKING_KEY( celda_pinpad, working_key ) ) {
            rta = PINPAD_SELECCIONAR_MASTER_KEY( celda_pinpad - 1 );
            if( rta && config.pais != CHILE && config.pais != BRASIL ) {
				char tecla;
				tecla=PINPAD_CONFIRMAR( c_tipo_cuenta[tipo_cuenta], 30 );
/*				tecla = '#';
				if( tecla != '#' ) {
                if( PINPAD_CONFIRMAR( c_tipo_cuenta[tipo_cuenta], 30 ) != '#' ) {
					{
						char mensa[80];
						sprintf(mensa,"No presiono enter en pinpad %c", tecla);
						glog(mensa, LOG_VARIOS,1);
					}
                    rta = 0;
                }
				}*/
            }
            if( rta && confirmar_importe ) {
				char tecla2;
                //sprintf( _importe, "CONFIRME MONTO %*.*lf", ENTEROS+DECIMALES, DECIMALES, importe );
//                PINPAD_MOSTRAR( ST( S_CONFIRME_MONTO ) );
  //              PAUSA( 72 );
        //        sprintf( _importe, "$%*.*lf", ENTEROS + DECIMALES, DECIMALES, importe );
          //      tecla2=PINPAD_CONFIRMAR( _importe, 30 );
				tecla2 = '#';
				if( tecla2 != '#' ) { //si el caracter es ? deberia reintentar una vez mas
				//if( PINPAD_CONFIRMAR( _importe, 30 ) != '#' ) { //si el caracter es ? deberia reintentar una vez mas
						{
						char mensa[80];
						sprintf(mensa,"No presiono2 enter2 en pinpad %c", tecla2);
						glog(mensa, LOG_VARIOS,1);
					}
                    rta = 0;
                }
            }
	
            if( rta ) {
				//aca pedimimos el pin 
				//
				if( PINPAD_VERIFICAR_ONLINE( ) == 0) {
					//pinpad offline
					MENSAJE("PINPAD OFFLINE");
					MENSAJE("REVISE PINPAD Y REVISE JSERVER");
					rta = 0;
				} else
                rta = PINPAD_PEDIR_PIN( nro_tarjeta, working_key, pin, l_pin );
            }
            PINPAD_CANCEL_SESSION();
		} else  { //aca deberiamos ver como trabajmos si no tnemos workingkey
			MENSAJE("PINPAD OFFLINE");
			MENSAJE("REVISE PINPAD Y REVISE JSERVER");
        }
        BORRAR_MENSAJE();
    }
    return ( rta );
}
/***************************************************************************/
void MOSTRAR_SALDO_EN_PINPAD( double importe )
/***************************************************************************/
{
    char mensaje[20];
    sprintf( mensaje, "%*.*lf", ENTEROS, DECIMALES, importe );
    PINPAD_CONFIRMAR( mensaje, 30 );
    BORRAR_MENSAJE();
}
/***************************************************************************/
static int _WORKING_KEY( int ( *funcion )(), char celda_pinpad, char *working_key )
/***************************************************************************/
{
    // El working key puede venir en una respuesta de un mensaje de tipo 0110 , 0210 , 0510
    int handle, rta = 0;
    #ifdef INVEL_L
    handle = open( _PINPAD_DAT, O_RDWR, __LINE__, __FILE__ );
    #else
	handle = _OPEN_O_CREAT( _PINPAD_DAT, O_RDWR | O_BINARY, __LINE__, __FILE__ );
    //handle = _open( _PINPAD_DAT, O_RDWR | O_BINARY/*, __LINE__, __FILE__*/ );
    #endif


    if( handle > 0 && celda_pinpad > 0 ) {
        if( !LSEEK( handle, ( long )( celda_pinpad * 8 ), SEEK_SET, __LINE__, __FILE__ ) ) {
            if( funcion( handle, working_key, 8, __LINE__, __FILE__ ) == 0 ) {
                rta = 1;
            }
        }
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    if( !rta ) {
		//no tengo workingkey para trabajar
        MENSAJE_STRING( S_ERROR_PINPAD );
		strncpy(working_key,"11111111", 8); 
		working_key[8]='\0';
		rta = 1; //forcemos que pida el working
    } else //ajustemos 
		working_key[8]='\0'; // por las dudas traiga basura solo debe leer 8
    return ( rta );
}
/***************************************************************************/
int PINPAD_ACTUALIZAR_WORKING_KEY( char celda_pinpad, char *working_key )
/***************************************************************************/
{
    char ceros[8];
    int rta = 1;
    memset( ceros, 0, sizeof( ceros ) );
    if( memcmp( working_key, ceros, sizeof( ceros ) ) != 0 ) {
        rta = _WORKING_KEY( _WRITE, celda_pinpad, working_key );
    }
    return ( rta );
}
/***************************************************************************/
static int PINPAD_LEER_WORKING_KEY( char celda_pinpad, char *working_key )
/***************************************************************************/
{
    return ( _WORKING_KEY( _READ, celda_pinpad, working_key ) );
}
/***************************************************************************/
static PINPAD_MENSAJES_ROTATIVOS( int cant, char *mens0, char *mens1, char *mens2, char *mens3,
                                  char *mens4, char *mens5, char *mens6 )
/***************************************************************************/
{
#ifdef COMPILAR_PINPAD
	  int rta = 0;
	  char buff[500];
	  memset(buff,0,sizeof(buff));
	 _snprintf( buff, sizeof(buff)-1, "%s&%s&%s&%s&%s&%s&%s&",
					 mens0, mens1, mens2, mens3, mens4, mens5, mens6);
      rta = COMANDO_PINPAD_SET(ACTION_PINPAD_MENSAJES_ROT, buff, 0) ;
	  return ( rta );
#endif COMPILAR_PINPAD

#ifdef COMPILAR_PINPAD3
    struct
    {
        int rta;
        int cant_mensajes;
        char *mensajes[7];
    } p_mens_rotativos;
    #if defined(INVEL_W) || defined(INVEL_L)
    p_mens_rotativos.rta = 0;
    #else
    p_mens_rotativos.cant_mensajes = cant;
    p_mens_rotativos.mensajes[0] = mens0;
    memcpy( puntero_memoria_driver_invel, mens0, strlen( mens0 ) );
    p_mens_rotativos.mensajes[0] = puntero_memoria_real_driver_invel;
    memcpy( puntero_memoria_driver_invel + 100, mens1, strlen( mens1 ) );
    p_mens_rotativos.mensajes[1] = puntero_memoria_real_driver_invel + 100;
    memcpy( puntero_memoria_driver_invel + 200, mens2, strlen( mens2 ) );
    p_mens_rotativos.mensajes[2] = puntero_memoria_real_driver_invel + 200;
    memcpy( puntero_memoria_driver_invel + 300, mens3, strlen( mens3 ) );
    p_mens_rotativos.mensajes[3] = puntero_memoria_real_driver_invel + 300;
    memcpy( puntero_memoria_driver_invel + 400, mens4, strlen( mens4 ) );
    p_mens_rotativos.mensajes[4] = puntero_memoria_real_driver_invel + 400;
    memcpy( puntero_memoria_driver_invel + 500, mens5, strlen( mens5 ) );
    p_mens_rotativos.mensajes[5] = puntero_memoria_real_driver_invel + 500;
    memcpy( puntero_memoria_driver_invel + 600, mens6, strlen( mens6 ) );
    p_mens_rotativos.mensajes[6] = puntero_memoria_real_driver_invel + 600;
    DRIVER( _DRV_pinpad, _PIN_mensajes_rotativos, 0, &p_mens_rotativos,
            sizeof( p_mens_rotativos ) );
    #endif
    return ( !p_mens_rotativos.rta );
#endif COMPILAR_PINPAD3
}
/***************************************************************************/
int PINPAD_MOSTRAR( char *cadena )
/***************************************************************************/
{
	int rta   = 0;
	char buff[100];
	  memset(buff,0,sizeof(buff));
	 _snprintf( buff, sizeof(buff)-1, "%s",
					 cadena);
      rta = COMANDO_PINPAD_SET(ACTION_PINPAD_MOSTRAR, buff, 0) ;
	  return !rta;
	
#ifdef COMPILAR_PINPAD3	
    struct
    {
        int rta;
        char *buf1;
    } rta_drv;
    #if defined(INVEL_W) || defined(INVEL_L)
    rta_drv.rta = 0;
    #else
    memcpy( puntero_memoria_driver_invel, cadena, strlen( cadena ) + 1 );
    rta_drv.buf1 = puntero_memoria_real_driver_invel;
    DRIVER( _DRV_pinpad, _PIN_mostrar, 0, &rta_drv, sizeof( rta_drv ) );
    memcpy( cadena, puntero_memoria_driver_invel, strlen( cadena ) );
    #endif
    return ( !rta_drv.rta );
#endif COMPILAR_PINPAD3
}
/***************************************************************************/
static int PINPAD_PEDIR_PIN( char *nro_tarjeta, char *working_key, char *pin, int l_pin )
/***************************************************************************/
{
    char wk_pinpad[17],aux,_pin[17];
    int i, rta = 0;
	int listo = 0;
 //   MENSAJE_STRING( S_EL_CLIENTE_DEBE_INGRESAR_SU_PIN );
//	PAUSA(30);
	MENSAJE_SIN_SONIDO_STRING( S_CLIENTE_DEBE_PRES_ENTER_PINPAD_PARA_CONFIRMAR );
	PAUSA(30);
    memset( _pin, 0, 17 );
	memset( wk_pinpad, 0, 17 );
	
    /*----------------------- Codifica working key --------------------------*/
    for( i = 0;i < 8;i++ ) {
        aux = ( working_key[i] & 0xf0 ) >> 4;
        wk_pinpad[i * 2] = ( aux < 0xa ) ? '0' + aux : 'A' + aux - 0xa;
        aux = working_key[i] & 0x0f;
        wk_pinpad[i * 2 + 1] = ( aux < 0xa ) ? '0' + aux : 'A' + aux - 0xa;
    }
	wk_pinpad[16]='\0';
    /*---------------------- Pide el PIN al usuario -------------------------*/
//reintemos
	do {
//		if( PINPAD_MOSTRAR( ST( S_INGRESE_SU_CLAVE ) ) ) {
        /*if( ( config.pais == CHILE || config.pais == BRASIL ) ) {
            PINPAD_SET_TIMEOUT( 30 );
        }
        else {
            PINPAD_SET_TIMEOUT( 60 );
        }*/
        rta = _PINPAD_PEDIR_PIN( nro_tarjeta, wk_pinpad, _pin, l_pin );
		if( rta == 0)  {//fallo 
			int tecla  = 0;
			PINPAD_CANCEL_SESSION();
			PINPAD_CARTEL();
			tecla = MENSAJE_CON_TECLA_S_N("REINTENTAR PIN <ENTER>=SI <ESC>=NO ?",13,27);
			if( PINPAD_VERIFICAR_ONLINE( ) == 0) {
					//pinpad offline
					MENSAJE("PINPAD OFFLINE");
					MENSAJE("REVISE PINPAD Y REVISE JSERVER");
					rta = 0;
			} else  {
			     MENSAJE_STRING( S_EL_CLIENTE_DEBE_INGRESAR_SU_PIN );
			if( tecla == 13 ) {
				listo = 0;
			} else {
				listo = 1;
    }
			}
		} else
			listo = 1;

        //PINPAD_SET_TIMEOUT( 20 );
 //   }
} while ( listo != 1) ;
// reintento
    /*------------------------ Decodifica el PIN ----------------------------*/
    _pin[16]='\0';
    for( i = 0;i < 8;i++ ) {
        aux = ( _pin[i * 2] < 'A' ) ? _pin[i * 2] - '0' : _pin[i * 2] - 'A' + 0xa;
        pin[i] = aux << 4;
        aux = ( _pin[i * 2 + 1] < 'A' ) ? _pin[i * 2 + 1] - '0' : _pin[i * 2 + 1] - 'A' + 0xa;
        pin[i] += aux;
    }
	pin[8]='\0';
    return ( rta );
}
/***************************************************************************/
 int _PINPAD_PEDIR_PIN( char *nro_tarjeta, char *working_key, char *pin, int l_pin )
/***************************************************************************/
{
   //Método pinPedirPin(String nro_tarjeta, String working_key, String mensaje1, String mensaje2, String mensaje3) 
	char buff[500];
	char pin_hexa[17];
	int error = 1;
	  memset(buff,0,sizeof(buff));
	  memset(pin_hexa,0,sizeof(pin_hexa));
	 _snprintf( buff, sizeof(buff)-1, "%s&%s&%s&%s&%s&%d",
					 nro_tarjeta, working_key, "INGRESE SU PIN", "ENTER CONFIRMA", "PIN INGRESADO", 30);
     //pin= rta.pin l_pin
	 
	 error = COMANDO_PINPAD_GET(ACTION_PINPAD_PEDIR_PIN, buff, 0) ;
	 if( !error && l_pin ) {
		int digitos_pin =0;
		 sscanf( buff, "%16s&%i",&pin_hexa, &digitos_pin );

		 if( pin_hexa && strlen(pin_hexa)> 10) {
			 strncpy(pin,pin_hexa,16);
			 pin[16]='\0';
		 }
		if( digitos_pin != l_pin ) {
				char      mensaje[80];
				memset(mensaje,0, sizeof(mensaje));
                sprintf( mensaje, ST( S_EL_PIN_DEBE_TENER__I_DIGITOS ), l_pin );
                MENSAJE_CON_PAUSA( mensaje, 90 );
                error = 1;
		}
	 } else {
                MENSAJE_CON_PAUSA( "PIN INCORRECTO O NO INGRESADO", 90 );
	 }

      return (!error);
#ifdef COMPILAR_PINPAD3	
    int rta = 0;
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    char mensaje[80];
    struct
    {
        int rta;
        char *working_key;
        char *nro_tarjeta;
        char *pin;
        int digitos_pin;
    } p_pedir_pin;
    // dejo espacio de sobra 100 c/u
    memcpy( puntero_memoria_driver_invel, working_key, strlen( working_key ) );
    p_pedir_pin.working_key = puntero_memoria_real_driver_invel;
    memcpy( puntero_memoria_driver_invel + 100, nro_tarjeta, strlen( nro_tarjeta ) );
    p_pedir_pin.nro_tarjeta = puntero_memoria_real_driver_invel + 100;
    memcpy( puntero_memoria_driver_invel + 200, pin, l_pin );
    p_pedir_pin.pin = puntero_memoria_real_driver_invel + 200;
    DRIVER( _DRV_pinpad, _PIN_pedir_pin, 0, &p_pedir_pin, sizeof( p_pedir_pin ) );
    memcpy( working_key, puntero_memoria_driver_invel, strlen( working_key ) );
    memcpy( nro_tarjeta, puntero_memoria_driver_invel + 100, strlen( nro_tarjeta ) );
    memcpy( pin, puntero_memoria_driver_invel + 200, 16 );
    rta = !p_pedir_pin.rta;
    //if( rta && l_pin && p_pedir_pin.digitos_pin != l_pin ) {
    if( rta && l_pin ) {
        if( config.pais == CHILE || config.pais == BRASIL ) {
            if( p_pedir_pin.digitos_pin < 4 || p_pedir_pin.digitos_pin > l_pin ) {
                sprintf( mensaje, ST( S_EL_PIN_DEBE_TENER_ENTRE__I_Y__I_DIGITOS ), 4, l_pin );
                MENSAJE_CON_PAUSA( mensaje, 90 );
                rta = 0;
            }
            else {
                SET_MEMORY_CHAR( __var_tarjetas_l_pin, p_pedir_pin.digitos_pin );
            }
        }
        else {
            if( p_pedir_pin.digitos_pin != l_pin ) {
                sprintf( mensaje, ST( S_EL_PIN_DEBE_TENER__I_DIGITOS ), l_pin );
                MENSAJE_CON_PAUSA( mensaje, 90 );
                rta = 0;
            }
        }
    }
    #endif
    return ( rta );
#endif COMPILAR_PINPAD3
}
/***************************************************************************/
 int PINPAD_PEDIR_TECLA( int timeout, char *tecla )
/***************************************************************************/
{
    int rta = 0;
	 char buff[5];

	 memset(buff,0,sizeof(buff));
    _snprintf( buff, sizeof(buff)-1, "%d", timeout);
      rta = COMANDO_PINPAD_GET(ACTION_PINPAD_GET_KEY, buff, 0) ;
	  *tecla = buff[0];
//Key Pressed 1 2 3 4 5 6 7 8   9  *  0  # /a F1 F2 F3
//   Key Code 1 2 3 5 6 7 9 10 11 13 14 15 16 20 21 22
	  //* es la tecla roja X 
	  // # es la tecla verde ENTER
	  // /a  es la tecla amarilla de retroceso

#ifdef COMPILAR_PINPAD3
    DRIVER( _DRV_pinpad, _PIN_getkey, timeout, &rta, sizeof( rta ) );
#endif COMPILAR_PINPAD3
    return ( !rta );
}
/***************************************************************************/
static int PINPAD_SELECCIONAR_MASTER_KEY( int master_key )
/***************************************************************************/
{
    int rta = 0;
	 char buff[500];
	  memset(buff,0,sizeof(buff));
	 _snprintf( buff, sizeof(buff)-1, "%d",
					 master_key);
	   rta = COMANDO_PINPAD_SET(ACTION_PINPAD_SELECT_MK, buff, 0) ;
#ifdef COMPILAR_PINPAD3
    DRIVER( _DRV_pinpad, _PIN_select_master_key, master_key, &rta, sizeof( rta ) );
#endif
    return ( !rta );
}
/***************************************************************************/
int PINPAD_CANCEL_SESSION()
/***************************************************************************/
{
	int rta = 0;
	 char buff[10];
	  memset(buff,0,sizeof(buff));
	 _snprintf( buff, sizeof(buff)-1, "");
	 rta = COMANDO_PINPAD_SET(ACTION_PINPAD_CANCEL_SESSION, buff, 0) ;  
	 return ( !rta);
#ifdef COMPILAR_PINPAD3
    DRIVER( _DRV_pinpad, _PIN_cancel_session, 0, &rta, sizeof( rta ) );
    return ( !rta );
#endif
}
/***************************************************************************/
int PINPAD_SET_TIMEOUT( int timeout )
/***************************************************************************/
{
	 char buff[50];
	 int rta = 0;
	  memset(buff,0,sizeof(buff));
	 _snprintf( buff, sizeof(buff)-1, "%d", timeout);
//      rta = COMANDO_PINPAD_SET(ACTION_PINPAD_TIMEOUT, buff, 0) ;
#ifdef COMPILAR_PINPAD3
    DRIVER( _DRV_pinpad, _PIN_set_timeout, timeout, &rta, sizeof( rta ) );
#endif
    return ( !rta );
}
/***************************************************************************/
void PINPAD_CARTEL()
/***************************************************************************/
{
    if( config.pais == CHILE || config.pais == BRASIL ) {
        PINPAD_MENSAJES_ROTATIVOS( 1, ST( S________REDCOMPRA ), NULL, NULL, NULL, NULL, NULL,
                                   NULL );
    }
    else {
/*        PINPAD_MENSAJES_ROTATIVOS( 2, "        TPS V", "        INVEL SA", NULL, NULL, NULL,
                                   NULL, NULL );*/
		PINPAD_MENSAJES_ROTATIVOS( 3, "        TPS V", "        INVEL SA", config.nom_comercio, "", "","", "" );
    }
}
/***************************************************************************/
char PINPAD_CONFIRMAR( char *mensaje, int espera )
/***************************************************************************/
{
    char tecla[2];
	int rta = 1;
	memset(tecla,0,2);
	//MENSAJE_SIN_SONIDO_STRING( S_CLIENTE_DEBE_PRES_ENTER_PINPAD_PARA_CONFIRMAR );
    PINPAD_MOSTRAR( mensaje );
	//PINPAD_MOSTRAR("PRESIONE ENTER!");
	//rta = PINPAD_PEDIR_TECLA(espera , tecla );
	return  tecla[0] ; //reutilizamos mensaje para traer la tecla
}
/***************************************************************************/
int PINPAD_PEDIR_WORKING_KEY()
/***************************************************************************/
{
    int rta = 1;
    struct _rta_terminal rta_terminal;
    if( MODELO_TEF == _TEF_SWITCH_NCR ) {
        MENSAJE_STRING( S_SOLICITANDO_WORKING_KEY );
        rta = ENVIAR_TRANSACCION_AL_AUT_ON( _ON_LINE_ACTUALIZA_WK, 0.0, &rta_terminal, 1 );
        BORRAR_MENSAJE();
        if( rta ) {
            PINPAD_ACTUALIZAR_WORKING_KEY( 1, rta_terminal.working_key );
        }
        else {
            MENSAJE_STRING( S_ERROR_AL_SOLICITAR_WORKING_KEY );
        }
    }
    return( rta );
}
#endif


/***************************************************************************/
int PINPAD_VERIFICAR_ONLINE()
/***************************************************************************/
{
	 char buff[50];
	int rta = 0;
	  memset(buff,0,sizeof(buff));
	 _snprintf( buff, sizeof(buff)-1, "");
      rta = COMANDO_PINPAD_GET(ACTION_PINPAD_ONLINE, buff, 0) ;
      return !rta;
}

/***************************************************************************/
int PINPAD_TRANSFER_MK( int address, char *key)
/***************************************************************************/
{
	char buff[500];
	int rta = 0;
	  memset(buff,0,sizeof(buff));
	 _snprintf( buff, sizeof(buff)-1, "%d&%s&",
					 address, key);
	 //hay que pasar la nueva mk
      rta = COMANDO_PINPAD_SET(ACTION_PINPAD_TRANSFER_MK, buff, 0) ;;
      return rta;
}

/***************************************************************************/
int PINPAD_GET_STRING( int echo_flag, int timeout, int max_can)
/***************************************************************************/
{
	char buff[500];
	int rta = 0;
	  memset(buff,0,sizeof(buff));
	 _snprintf( buff, sizeof(buff)-1, "%d&%d&%s&",
					 echo_flag, timeout,max_can);

      rta = COMANDO_PINPAD_GET(ACTION_PINPAD_GET_STRING, buff, 0) ;;
      return rta;
}

/***************************************************************************/
void PINPAD_VERSION()
/***************************************************************************/
{
    char buff[50];
	int rta = 0;
	  memset(buff,0,sizeof(buff));
      rta = COMANDO_PINPAD_GET(ACTION_PINPAD_VERSION, buff, 0) ;
	  if( rta == 0 ) //no hubo error
		  GRABAR_LOG_SISTEMA( buff ,LOG_JAVA_SERVER,2);
	  else
		GRABAR_LOG_SISTEMA( "ERROR PINPAD" ,LOG_JAVA_SERVER,2);


}

/***************************************************************************/
int COMANDO_PINPAD_SET(int action, char *buffer, int len )
/***************************************************************************/
{
	int rta = 0;
	rta = setData(action, buffer, 0) ;
	return (rta);
}

/***************************************************************************/
int COMANDO_PINPAD_GET(int action, char *buffer, int len )
/***************************************************************************/
{
	int rta = 0;
	rta = getData(action, buffer, 0) ;
	return (rta);
}


/***************************************************************************/
int _PIN_WORKING_KEY( int ( *funcion )(),char *working_key )
/***************************************************************************/
{
    // El working key puede venir en una respuesta de un mensaje de tipo 0110 , 0210 , 0510
	int handle, rta = 0;
    #ifdef INVEL_L
    handle = open( _ARCH_PWK, O_RDWR, __LINE__, __FILE__ );
    #else
	handle = _OPEN_O_CREAT( _ARCH_PWK, O_RDWR | O_BINARY, __LINE__, __FILE__ );
    //handle = _open( _PINPAD_DAT, O_RDWR | O_BINARY/*, __LINE__, __FILE__*/ );
    #endif


	if( handle > 0 ) {
		int salida = 0;
		int jj;
		for( jj=0; jj<2 && rta == 0 && salida == 0; jj++) {
			if( !LSEEK( handle,( TARJETA_DEBITO) * 8 , SEEK_SET, __LINE__, __FILE__ )){
				if( funcion( handle, working_key, 8, __LINE__, __FILE__ ) == 0 ) {
					rta = 1;
				} else { //no lo encontro veamos si restamos 1 por error de lectura
					if( TARJETA_DEBITO > 0) {
						if( !LSEEK( handle,( TARJETA_DEBITO - 1) * 8 , SEEK_SET, __LINE__, __FILE__ )){
							if( funcion( handle, working_key, 8, __LINE__, __FILE__ ) == 0 ) 
								rta = 1;
							 else 
								salida = 1;
						}
					}
				}
			}
		}
		_CLOSE( handle, __LINE__, __FILE__ );
	}
    if( !rta ) {
		//no tengo workingkey para trabajar
        MENSAJE_STRING( S_ERROR_PINPAD );
		working_key[8]='\0';
		rta = 1; //forcemos que pida el working
    } else //ajustemos 
		working_key[8]='\0'; // por las dudas traiga basura solo debe leer 8
    return ( rta );
}


/***************************************************************************/
int PINPAD_ACTUALIZAR_pin_WORKING_KEY( char *working_key )
/***************************************************************************/
{
    char ceros[8];
    int rta = 1;
    memset( ceros, 0, sizeof( ceros ) );
    if( memcmp( working_key, ceros, sizeof( ceros ) ) != 0 ) {
        rta = _PIN_WORKING_KEY( _WRITE, working_key );
		if( rta == 1) 
			TARJETA_DEBITO++; //si es anulacion no tiene que incrementar
    }
    return ( rta );
}
/***************************************************************************/
int PINPAD_LEER_pin_WORKING_KEY( char *working_key )
/***************************************************************************/
{
    int rtaa= _PIN_WORKING_KEY( _READ, working_key ) ;
	return ( rtaa );
}

void SETEAR_DEBITO_POS( int nro) 
{
	if(nro == 0)
		TARJETA_DEBITO= nro ;
	else
		TARJETA_DEBITO+= nro ;
}
