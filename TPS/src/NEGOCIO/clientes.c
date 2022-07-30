#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
//#include <browse.h>
#include <btrv.h>
#include <b_mens.h>
#include <clave.h>
#include <clientes.h>
#include <cmemory.h>
#include <coop.h>
#include <cr.h>
#include <cr_disco.h>
#include <cr_pant.h>
#include <dbrouter.h>
#include <envios.h>
#include <getch.h>
#include <log.h>
#include <malloc.h>
#include <mensaje.h>
#include <menu.h>
#include <mstring.h>
#include <m_cadena.h>
#include <pago_tar.h>
#include <pant.h>
#include <perfiles.h>
#include <tarjetas.h>
#include <tar_mag.h>
#include <tkt.h>
#include <trim.h>
#include <_cr1.h>
#include <aarch.h>
#include <netcom2.h>
#include <path.h>
#include <comprob.h>
#include <db.h>
#include <deftables.h>
#include <cio.h>
#include <ini.h>

/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W) || defined(INVEL_L)
#include <stdio.h>
#include <actions.h>
#include <typeacti.h>
#include <uti-comp.h>
#endif
#include "etpv.h"
#define INSERTAR_CLIENTE   1
#define MODIFICAR_CLIENTE  2
#define NOVEDAD_CLIENTE    1
#define BORRAR_CLIENTE     3
#define CODIGO       0
#define CUIT         1
#define DNI          2
#define GENERICA     3
#define MULTIPLE     4
#define COD_BARRA    5
#define ALTA_CLI     9

long reg_pos;
long reg_pos_inf;
static long nro;
static char cuit[14];
static char codigo[15];
extern struct _datos_transaccion *tran_rta;
//extern char *TIPOS_IVA[11];
//extern char *TIPOS_IVA_MOSTRAR[5];
struct _grupos_clientes *grupocli;
struct _condiciones_iva *_cond_iva;
struct _condiciones_iva cond_iva;
extern struct _ConfTkt *CONFIG_CLIENTES;
int ind_comienzo_datos_solicitar;
static int ind_config = 0;
int CANT_DATOS_SOLICITAR = 0;
char _CAMPO_CLIENTE[50];
char _TIPO_DATO_CLIENTE[30];
char _OFFSET_CLIENTE[20];
int _DIGITOS_CLIENTE = 0;
char _VALOR_POR_DEFECTO[15];

#define _X1  32
#define _X2  53
#define _Y1  12   //5
#define _Y2  20

int CLIENTE_HABILITADO_PARA_CHEQUES( void  );
int _consulta_grupos_cli( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
int _elegir_cond_iva( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
void MOSTRAR_DATOS_BOX_CLIENTE( void );
int BUSCAR_CLIENTE_POR_COD_GENERICO( char *cadena, char cod_grupo );
int OBTENER_DESCRIPCION_GRUPO( char *grupo_aux, char cod_grupo, int largo );
int OBTENER_DESCRIPCION_COND_IVA( char *c_iva, char con_iva, int largo );
int EXISTE_CLIENTE_DNI( long cliente );
int EXISTE_CLIENTE_POR_CODIGO( char *codigo );
int EDITAR_DATOS_CLIENTE();
int SALIR_EDICION_CLIENTE();
long BUSCAR_CLIENTE_COD_BARRA();
extern int __cliente;
extern int PEDIR_FECHA( int x, int y, char *str );
int lectura_banda= 0;
long  LEER_CLIENTE_BANDA(int inicio, int fin , int * lectura_manual,unsigned char* buffer1, unsigned char* buffer2  ) ;
/*****************************************************************************/
int CARGAR_CONF_CLIENTES()
/*****************************************************************************/
{
    /*No se crea una nueva variable BUFFER_CLIENTE ya que se va ha mejorar
    Alta de Clientes Configurable y no va ha ser necesaria. Se reutiliza la
    variable BUFFER_TICKET provisoriamente */
    return ( CARGAR_CONFIG_TKT( CLIENTES_CFG, &CONFIG_CLIENTES, BUFFER_TICKET ) );
}

/***************************************************************************/
void INIT_CLIENTE_CONFIGURABLE()
/***************************************************************************/
{
    /*----------- Tomamos los parametros iniciales ---------*/
    ind_config = 0;

    while( CONFIG_CLIENTES[ind_config].codigo != _TKT_FIN_INICIALIZACION
        && CONFIG_CLIENTES[ind_config].codigo != _FIN ) {
        CLIENTE_ejecutar_comando( ( int* )&ind_config );
    }
    ind_comienzo_datos_solicitar =_COMIENZO_CONFIG_CLI( _COMIENZO_CONFIG_DATOS_CLIENTE, _FIN_CONFIG_DATOS_CLIENTE );
}

/***************************************************************************/
int _COMIENZO_CONFIG_CLI( int comienzo, int final )
/***************************************************************************/
{
    int indice = 0;

    while( CONFIG_CLIENTES[indice].codigo != comienzo && CONFIG_CLIENTES[indice].codigo != final
        && CONFIG_CLIENTES[indice].codigo != _FIN ) {
        indice++;
    }
    if( CONFIG_CLIENTES[indice].codigo == _FIN || CONFIG_CLIENTES[indice].codigo == final ) {
        indice = 0;
    }

    return( indice );
}

/***************************************************************************/
void CLIENTE_ejecutar_comando( int *ind_config )
/***************************************************************************/
{
    int ind_configTmp = 0, digitos = 0;
    INT16 codigo = 0;

    codigo = ( INT16 )CONFIG_CLIENTES[( *ind_config )].codigo;

    switch( codigo ) {
        case _CANT_DATOS_SOLICITAR:
            ind_configTmp = 0;

            CANT_DATOS_SOLICITAR = _tomar_entero_config( ind_config, &ind_configTmp, CONFIG_CLIENTES );
            break;

        case _TKT_FIN_INICIALIZACION:
            break;

        case _CAMPOS_CLIENTES:
            ind_configTmp = 0;

            memset( _CAMPO_CLIENTE, 0, sizeof( _CAMPO_CLIENTE ) );
            memset( _TIPO_DATO_CLIENTE, 0, sizeof( _TIPO_DATO_CLIENTE ) );
            memset( _OFFSET_CLIENTE, 0, sizeof( _OFFSET_CLIENTE ) );
			memset( _VALOR_POR_DEFECTO, 0, sizeof( _VALOR_POR_DEFECTO ) );
            _DIGITOS_CLIENTE = 0;

            //Nombre del campo
            strncpy( _CAMPO_CLIENTE, &CONFIG_CLIENTES[( *ind_config )].parametros[ind_configTmp],
                     sizeof( _CAMPO_CLIENTE ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_CLIENTES[( *ind_config )].parametros[ind_configTmp] )
            + 1;

            //Tipo de datos
            strncpy( _TIPO_DATO_CLIENTE, &CONFIG_CLIENTES[( *ind_config )].parametros[ind_configTmp],
                     sizeof( _TIPO_DATO_CLIENTE ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_CLIENTES[( *ind_config )].parametros[ind_configTmp] )
            + 1;

            //Offset
            strncpy( _OFFSET_CLIENTE, &CONFIG_CLIENTES[( *ind_config )].parametros[ind_configTmp],
                sizeof( _OFFSET_CLIENTE ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_CLIENTES[( *ind_config )].parametros[ind_configTmp] )
            + 1;

            //Cantidad de digitos a ingresar
            _DIGITOS_CLIENTE = _tomar_entero_config( ind_config, &ind_configTmp, CONFIG_CLIENTES );

			//Valores por defecto
             strncpy( _VALOR_POR_DEFECTO, &CONFIG_CLIENTES[( *ind_config )].parametros[ind_configTmp],
                     sizeof( _VALOR_POR_DEFECTO ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_CLIENTES[( *ind_config )].parametros[ind_configTmp] )
            + 1;


            break;
    }
    ( *ind_config )++;

}

/***************************************************************************/
INT16 _tomar_entero_config( int *ind_config, int *indice, struct _ConfTkt *CONFIG_ARCH )
/***************************************************************************/
{
    INT16 valor;

    valor = *( INT16* )&CONFIG_ARCH[( *ind_config )].parametros[*indice];
    ( *indice ) += 2;
    return ( valor );
}

/*****************************************************************************/
void LIBERA_UTI_CLI()
/*****************************************************************************/
{
    if( CONFIG_CLIENTES ) {
        free( CONFIG_CLIENTES );
    }

}

/******************************************************************************/
int VALIDAR_CONF_CLIENTE( )
/******************************************************************************/
{
    char tiporesCh;
    int cant_datos_config = 0;

    ind_config = ind_comienzo_datos_solicitar + 1;

    while( CONFIG_CLIENTES[ind_config].codigo != _FIN_CONFIG_DATOS_CLIENTE
            && CONFIG_CLIENTES[ind_config].codigo == _CAMPOS_CLIENTES ) {

        //Valido que los Tipos de Datos configurados esten definidos
        CLIENTE_ejecutar_comando( ( int* )&ind_config );
        if( ( obtenerTipo( _TIPO_DATO_CLIENTE, &tiporesCh, NULL ) ) == -1 ) {
            GRABAR_LOG_SISTEMA( ST( S_ERROR_DATO_TABLA_tipo_no_definido ) ,LOG_ERRORES,1);
            return 0;
        }
        cant_datos_config ++;

    }

    //Valido que la cantidad configurada coincidan con la cantidad de campos configurados.
    if( cant_datos_config && CANT_DATOS_SOLICITAR != cant_datos_config  ) {
        MENSAJE( ST( S_ERROR_CANT_DE_DATOS_CONFIGURADOS ) );
        BORRAR_MENSAJE();
        return 0;
    }

    return 1;

}

/***********************************************************************/
int INGRESAR_DATOS_CLIENTES_CONFIG( int *posicion )
/***********************************************************************/
{
    int rta = 1, x = 0, y = 0, offset = 0, reintentar = 0, decimal = 0, posInicial=0, pos = 0;
    char abrev, cadena[100];
    char *config_cli = NULL;
    INT16 inttemporal;
    UINT16 uinttemporal;
    long longtemporal;
    double doubletemporal;
    float floattemporal;
    unsigned long ulongtemporal;
    char bytetemporal;
    char nom_perfil[12];
	int vaciar_cadena = SI, tipo_carga =0;
    pos = *posicion;
    posInicial = *posicion;
    config_cli = ( char* )MALLOC( sizeof( struct _clientes ) );
    memset( config_cli, 0, sizeof( struct _clientes ) );
    memset( nom_perfil, 0, sizeof(nom_perfil) );
	if( config.pais == ELSALVADOR ){
        //Dado que en salvador se pregunta por retenciones s/n y el mismo se setea en exento s/n se da vuelta la logica
        //Ya que si es exento no aplica retenciones
		if( clientes.cond_iva == INST_GUBERNAMENTAL && clientes.exento_ing_brutos == 0 && clientes.retencion_descuento == 0){
			clientes.exento_ing_brutos = 'S';//DESPUES LO DA VUELTA EN (**1) PARA QUE APLIQUE LA RETENCION
			clientes.retencion_descuento = 'S';
		}
	}

    memcpy( config_cli, &clientes, sizeof( struct _clientes ) );

    if(ind_config < (ind_comienzo_datos_solicitar + 1)) {
    ind_config = ind_comienzo_datos_solicitar + 1;
    }

    while( ( rta == 1 || rta == -3 || -72 == rta || -80 == rta) &&
            CONFIG_CLIENTES[ind_config].codigo != _FIN_CONFIG_DATOS_CLIENTE
            && CONFIG_CLIENTES[ind_config].codigo == _CAMPOS_CLIENTES && (posInicial - pos) <= 0 ) {
		memset( cadena, 0, sizeof( cadena ) );
        CLIENTE_ejecutar_comando( ( int* )&ind_config );
        vaciar_cadena = SI;
        //Obtengo el Tipo de dato del cliente
        if( ( obtenerTipo( _TIPO_DATO_CLIENTE, &abrev, NULL ) ) == -1 ) {
            GRABAR_LOG_SISTEMA( ST( S_ERROR_DATO_TABLA_tipo_no_definido ) ,LOG_ERRORES,1);
            *posicion = pos;
            return 0;
        }

        offset = atoi( _OFFSET_CLIENTE );
        reintentar = 0;
        TECLADO_ALFABETICO = 1;



		/*carga el VALOR_POR_DEFECTO */
		strncpy( cadena, _VALOR_POR_DEFECTO,(sizeof(_VALOR_POR_DEFECTO)-1) );
		//if( config_cli[offset] != 0 ) esta hace falta cuando es por defecto para gubernamental

		if( cadena[0] != 0x00 ){
			vaciar_cadena = NO;
		}

		if( config_cli[offset] != 0x00 ){
			switch( abrev ) {
				case 'i':
					if( _DIGITOS_CLIENTE == 1 ) {
						sprintf(cadena,"%c",config_cli[offset]);
					} else {
						sprintf(cadena,"%i",config_cli[offset]);
					}
						inttemporal = 0;
				break;
				case 'l':
					memcpy( &longtemporal,&config_cli[offset], sizeof( unsigned long ) );
					sprintf(cadena,"%lu",longtemporal);
					longtemporal = 0;

				break;
				case 'n'://No se probo
					memcpy( &ulongtemporal,&config_cli[offset], sizeof( unsigned long ) );
					sprintf(cadena,"%lu",ulongtemporal);
					ulongtemporal = 0;
				break;
				case 'b'://No se probo
					memcpy(  &bytetemporal,&config_cli[offset], sizeof( char ) );
                    sprintf(cadena,"%i",bytetemporal);
					bytetemporal = 0x00;
				break;
				case 'd'://No se probo
					memcpy(  &doubletemporal, &config_cli[offset],sizeof( double ) );
					sprintf(cadena,"%lf",doubletemporal);
					doubletemporal = 0.00;
				break;
				case 'f':
					memcpy(  &floattemporal, &config_cli[offset], sizeof( float ) );
					sprintf(cadena, "%fl", floattemporal);
					floattemporal = 0.0;
				break;
				case 'u'://Se utiliza para fecha
					memcpy( &uinttemporal, &config_cli[offset], sizeof( UINT16 ) );
					sprintf(cadena,"%i",uinttemporal);
					uinttemporal = 0;
					break;
				case 's':
					memcpy(  &cadena, &config_cli[offset],strlen( &config_cli[offset]) );
					//sprintf(cadena,"%s",config_cli[offset]);
					break;
				case 'c':
					sprintf(cadena,"%c",config_cli[offset]);
					break;
			}

			vaciar_cadena = NO;
		}




        //CODIGO PERFIL
        if( offset == 337 ) {
            int repetir = 1;
            do{
                if(!strlen(nom_perfil) && !*((long *)&config_cli[offset])){
                 //   do{
                        longtemporal = ( long )ASIGNA_PERFILES( nom_perfil );
                        memcpy( &config_cli[offset], &longtemporal, sizeof( long ) );
                        abrev = 'A';
                        MOSTRAR_CADENA( ( char )( x + 23 ), ( char )( y + pos + 1  ), nom_perfil );
                        ExecuteSSAction( A_PEDIR_DATO, pos, " ", " ", NULL, " " );
                        MOSTRAR_INPUT_CADENA( nom_perfil, pos ); // pone el valor
                    //}while(LASTKEY() == 27 );
                }
                ExecuteSSAction( A_PEDIR_DATO, pos, " ", " ", NULL, " " );
                abrev = 'A';
                while((rta = GETCH()) == -999);
                //si no es ni flecha arriba ni flecha abajo ni enter ni esc
                if(rta != -72 && rta != -80 && rta != 27 && rta != 13){
                    memset( nom_perfil, 0, sizeof(nom_perfil) );
                    longtemporal = 0;
                    memcpy( &config_cli[offset], &longtemporal, sizeof( long ) );
                }else{
                    repetir = 0;
                }
            }while(repetir);
        } else {
            ExecuteSSAction( A_PEDIR_DATO, pos, " ", " ", NULL, " " );
        }

        switch( abrev ) {
            case 'i':
            case 'l':
            case 'n':
            case 'b':
            case 'd':
            case 'f':
                while( reintentar < 2 ) {
                    if( reintentar ) {
                        MENSAJE( ST( S_INGRESO_VALOR_INCORRECTO) );
                        BORRAR_MENSAJE();
                    }
                    if( reintentar != 2 && ( abrev == 'd' || abrev == 'f' ) ) {
                        decimal = SI;
                    }
                    if( reintentar != 2 && abrev == 'i' && _DIGITOS_CLIENTE == 1 ) {
                        rta = PEDIR_CADENA( ( char )( x + 23 ), ( char )( y + pos + 1  ),
                                    ( char )_DIGITOS_CLIENTE, cadena, NULL, vaciar_cadena, SI );
                    } else {
                        rta = PEDIR_CADENA_NUMERICA( ( char )( x + 23 ), ( char )( y + pos + 1 ),
                                ( char )_DIGITOS_CLIENTE, cadena, NULL, decimal, vaciar_cadena,SI,NULL );
                    }
                    switch( abrev ) {
                        case 'i':
                            if( _DIGITOS_CLIENTE == 1 ) {
                                memcpy( &config_cli[offset], &cadena, sizeof( char ) );
                                reintentar = 2;
                            } else {
                                inttemporal = ( INT16 )strtoul( cadena, NULL, 10 );
                                if( inttemporal < 0 ) {
                                    reintentar++;
                                } else {
                                    reintentar = 2;
                                    memcpy( &config_cli[offset], &inttemporal, sizeof( INT16 ) );
                                }
                            }
                            break;

                        case 'l':
                            longtemporal = atol( cadena );
                            memcpy( &config_cli[offset], &longtemporal, sizeof( long ) );
                            reintentar = 2;
                            break;

                        case 'n'://No se probo
                            ulongtemporal = strtoul( cadena, NULL, 10 );
                            memcpy( &config_cli[offset], &ulongtemporal, sizeof( unsigned long ) );
                            reintentar = 2;
                            break;

				        case 'b'://No se probo
                            bytetemporal = ( char )atoi( cadena );
                            memcpy( &config_cli[offset], &bytetemporal, sizeof( char ) );
                            reintentar = 2;
                            break;

                        case 'd'://No se probo
                            doubletemporal = strtod( cadena, NULL );
                            memcpy( &config_cli[offset], &doubletemporal, sizeof( double ) );
                            reintentar = 2;
                            break;

                        case 'f':
                            floattemporal = ( float )atof( cadena );
                            memcpy( &config_cli[offset], &floattemporal, sizeof( float ) );
                            reintentar = 2;
                            break;
                    }
                }
                break;
            case 'u'://Se utiliza para fecha
                uinttemporal = PEDIR_FECHA( ( char )( x + 23 ), ( char )( y + pos + 1 ), cadena );
                memcpy( &config_cli[offset],&uinttemporal, sizeof( UINT16 ) );
                reintentar = 2;
                break;

            case 's':
                rta = PEDIR_CADENA( ( char )( x + 23 ), ( char )( y + pos + 1  ),
                        ( char )_DIGITOS_CLIENTE, cadena, NULL, vaciar_cadena, SI );
                strcpy( &config_cli[offset], cadena );
                break;

            case 'c':
                do {
                    rta = PEDIR_CADENA( ( char )( x + 23 ), ( char )( y + pos + 1  ),
                            ( char )_DIGITOS_CLIENTE, cadena, NULL, vaciar_cadena, SI );

                    strupr( cadena );//convierte todo a mayuscula
                    if( strcmp( cadena, "" ) == 0 ) {
                        MOSTRAR_INPUT_CADENA( "N", pos );
                        strcpy( cadena, "N" );
                        memcpy( &config_cli[offset], &cadena, sizeof( char ) );
                    }
                } while( strcmp( cadena, "N" ) != 0 && strcmp( cadena, "S" ) != 0 );

                memcpy( &config_cli[offset], &cadena, sizeof( char ) );
                break;
        }
        if(-72 == rta){
            ind_config-=2;
            pos--;
        }else{
            if(0 != rta)
                pos++;
            else
                ind_config--;
            //rta = SALIR_ALTA_CLIENTE();
        }

    }//fin while cliente.cfg

    {
        int cod_pefilTmp = clientes.cod_perfil; //guarda el codigo de perfil para asignarlo despues
        memcpy( &clientes, config_cli, sizeof( struct _clientes ) );
        clientes.cod_perfil = cod_pefilTmp;
    }

    if( config.pais == ELSALVADOR && rta == 1){//(**1)
        //Dado que en salvador se pregunta por retenciones s/n y el mismo se setea en exento s/n se da vuelta la logica
        //Ya que si es exento no aplica retenciones
        if(clientes.exento_ing_brutos == 'n' || clientes.exento_ing_brutos == 'N'){
            clientes.exento_ing_brutos = 'S';
        }else{
            if(clientes.exento_ing_brutos == 's' || clientes.exento_ing_brutos == 'S'){
                clientes.exento_ing_brutos = 'N';
            }
        }
    }

    FREEMEM( config_cli );
    if(0 != rta)
        *posicion = pos + 1;
    else
        *posicion = pos;
    return( rta );

}
/***************************************************************************/
int PEDIR_NRO_CLIENTE( int recuadro, int permitir_escape )
/***************************************************************************/
{
    int rta = 0, mensaje = 0, pant2 = 0, lectura_manual = 1, digitos, l;
    int area_ant, tipo_ant, mensaje_1 = 0, mostrar_cliente = 0;
    char buffer2[50],buffer1[80],cadena[30],cod_grupo,car = 'X';
	int datos_invalidos = 0;
    char cad[50];
	long cod_numerico = 0;
	int inicio = 8, fin =16;
    struct   _grupos_clientes grupocli_sic;

    struct busqueda vec_bus[] = { { _BUSQUEDA_NOMBRE, ST( S_BUSQUEDA_NOMBRE ) }, { _BUSQUEDA_DNI, ST( S_BUSQUEDA_DNI ) },
                                { _BUSQUEDA_CUIT, ST( S_BUSQUEDA_CUIT ) }, { _BUSQUEDA_CODIGO, ST( S_BUSQUEDA_CODIGO ) },
                                { _BUSQUEDA_BARRA, ST( S_BUSQUEDA_BARRA ) }, { _BUSQUEDA_GENERICO, ST( S_BUSQUEDA_GENERICO ) } };
    int opciones_busqueda = 8;
    int i = 0, y = 0, j = 0, valida_cliente = 1, codigo_incorrecto = 0, alta_cliente = 0;

    struct _menu *items = NULL;

    #if defined(INVEL_W) || defined(INVEL_L)
    char stringData[50];
    int hay_cli_cargado = 0;
    memset( stringData, 0, 50 );
    #endif
	lectura_banda = 0;
	_PERMITIR_BANDA = NO;
    /********************** Cargamos Busquedas de Cliente **********************/

    if( ( items = ( struct _menu * )malloc( sizeof( struct _menu ) * opciones_busqueda ) ) != NULL ) {
        y = 0;
        for( i = 0; i < 6; i++ ) {
            memset( cad, 0, sizeof( cad ) );
            if( ( items[y].nom = ( char* )malloc( sizeof( string_table->mensaje ) ) ) != NULL ) {
                _snprintf( cad, sizeof(cad), "%i) %s", j + 1 , vec_bus[i].mensaje );
                cad[ sizeof( cad ) - 1] = '\0';
                if( vec_bus[i].codigo & config.cliente_busqueda ) {
                    strcpy( items[y].nom, cad );
                    items[y].tecla = j + 49;
                    items[y].rta = vec_bus[i].codigo;
                    y++;
                    j++;
                }
            }
        }
        if( BUSQUEDA_CLIENTE == MULTIPLE && ALTA_DE_CLIENTE_HABILITADA ) {
            memset( cad, 0, sizeof( cad ) );
            if( ( items[y].nom = ( char* )malloc( sizeof( string_table->mensaje ) ) ) != NULL ) {
                strcpy( items[y].nom, ST( S_ESPACIO ) );
                items[y].tecla = '99' ;
                items[y].rta = 99;
                y++;
            }
            if( ( items[y].nom = ( char* )malloc( sizeof( string_table->mensaje ) ) ) != NULL ) {
                _snprintf( cad, sizeof(cad), "%i) %s", j + 1, ST( S_ALTA_DE_CLIENTE ) );
                cad[ sizeof( cad ) - 1] = '\0';
                strcpy( items[y].nom, cad );
                items[y].tecla = j + 49;
                items[y].rta = 9;
                y++;
            }
        }
    }

    grupocli = &grupocli_sic;
	if(config_tps.activarTFpermanente == 1) {
		SET_MEMORY_LONG( __ram_nro_cliente, 0 ); //para que permita buscarlo
	}


	if( FACTURACION_HABILITADA || PEDIR_CLIENTE_EN_CHEQUES ) {
		if( RAM_NRO_CLIENTE /*&& !RAM_MOSTRAR_SALDO_CHEQUE*/ ) {
            rta = 2;//Cliente previamente cargado.
        }
		else {
            LIMPIA_BUFFER_TARJETA( NO );
            #if defined(INVEL_W) || defined(INVEL_L)
            hay_cli_cargado = RAM_NRO_CLIENTE;
            #endif
            /*------------------------ Pide cliente -------------------------------*/
            SET_MEMORY_LONG( __ram_nro_cliente, 0 );
            CEREAR_PERFIL_CLIENTE();
            /*if( USAR_CUIT_COMO_NRO_CLIENTE ) {
                    *   digitos = ( config.pais == ECUADOR ) ? 10 : 11;
                    *   if( recuadro )
                   *     rta = PEDIR_CADENA_EN_RECUADRO( ST( S__CLIENTE_ ), NO, digitos, cuit, NULL );
                    *   else
                    *       rta =
                   *         PEDIR_CADENA_XY( ST( S__CLIENTE_ ), 40 - ancho_panel, 20, digitos, cuit );
                    *   if( rta ) {
                    *       rta = BUSCAR_CLIENTE_POR_CUIT( cuit );
                    *       mensaje = !rta;
                    *   }
                    *} else {
                    *   _PERMITIR_BANDA = SI;*/
			switch( BUSQUEDA_CLIENTE ) {
                case CODIGO:
                    //CODIGO
					_PERMITIR_BANDA = SI;
                    if( recuadro ) {
                        // PANTALLA DE VENTA
                        nro = PEDIR_ENTERO_EN_RECUADRO( ST( S_COD_CLIENTE ), NO, 8 );
                    }
                    else {
                        // PANTALLA DE PAGO
                        nro = PEDIR_ENTERO_XY( ST( S_COD_CLIENTE ), 40 - ancho_panel, 20, 8 );
                    }

                    
                    
					if( nro == -2 ){
						nro = LEER_CLIENTE_BANDA( inicio, fin , &lectura_manual,buffer1, buffer2  );
						if(nro > 0 && config_tps.ClienteTarjetaEspecial && buffer2 ){
							int cod_seguridad = atol( buffer2 );
							if(config_tps.ClienteTarjetaEspecial && config_tps.Codigo_seguridad_cliente == cod_seguridad  ){
								lectura_banda =1;
							}
						}
					}

					_PERMITIR_BANDA=NO;
					/*
					if( nro == -2 ) {
                        if( LEER_TARJETA_BANDA( (unsigned char *)buffer2, (unsigned char *)buffer1 ) ) {
                            buffer2[16] = 0;
                            nro = atol( &buffer2[8] );
                            lectura_manual = NO;
                        }
                    }*/


                    if( nro > 0 ) {
                        rta = EXISTE_CLIENTE( nro );
                        mensaje = !rta;
                    }
                    break;

                case CUIT:
                    digitos = 14;
					
                    if( recuadro ) {
                        rta = PEDIR_CADENA_EN_RECUADRO( ST( S_CUIT ), NO, digitos, cuit, NULL );
                    }
                    else {
                        rta = PEDIR_CADENA_XY( ST( S_CUIT ), 40 - ancho_panel, 20, digitos,
                                               cuit );
                    }
                    TRIM( cuit );
                    if( rta && strlen( cuit ) > 0 ) {
                        rta = BUSCAR_CLIENTE_POR_CUIT( cuit );
                        mensaje = !rta;
                    }
                    else {
                        rta = 0;
                    }
                    break;

                case DNI:
                    _PERMITIR_SCANNER = SI;
					_PERMITIR_BANDA = SI;
                    digitos = 9;
					rta = PEDIR_CADENA_EN_RECUADRO( ST( S_DNI ), SI, digitos, cadena, NULL );
                    l = strlen( cadena );
                    //cadena[l - 1] = 0;
                    TRIM( cadena );
                    nro = atol( cadena );

					if( rta == -2 ){
						nro = LEER_CLIENTE_BANDA( inicio, fin , &lectura_manual,buffer1, buffer2  );
						if(nro > 0 && config_tps.ClienteTarjetaEspecial && buffer2 ){
							int cod_seguridad = atol( buffer2 );
							if(config_tps.ClienteTarjetaEspecial && config_tps.Codigo_seguridad_cliente == cod_seguridad  ){
								lectura_banda =1;
							}
						}
					}

					_PERMITIR_BANDA=NO;
					/*
                    _PERMITIR_BANDA = NO;
                    if( nro == -2 ) {
                        if( LEER_TARJETA_BANDA( (unsigned char *)buffer2, (unsigned char *)buffer1 ) ) {
                            buffer2[16] = 0;
                            nro = atol( &buffer2[8] );
                            lectura_manual = NO;
                        }
                    }*/
                    if( nro > 0 && strlen( cadena )||( lectura_manual == NO && nro ) ) {
                        rta = EXISTE_CLIENTE_DNI( nro );
                        mensaje = !rta;
                    }
                    else {
                        rta = 0;
                    }
                    _PERMITIR_SCANNER = NO;
                    break;

                case GENERICA:
                        /*-------------------- Prepara la pantalla -----------------------*/
                        cod_grupo = -1;
                        _ATRIBUTO = 120;
                        area_ant = SELECTED2();
						tipo_ant = SELECTED_TIPO();
                      
						if( OPEN_TABLE( T_GRUPO_CLI, TT_ORIG, ( char* )&grupocli_sic, sizeof( struct _grupos_clientes ) ) == 0 ) {
                            int cant_datos;
							SELECT_TABLE( T_GRUPO_CLI, TT_ORIG );
							RUN_QUERY(NO);
                            //cant_datos = RECCOUNT(  );
                            cant_datos = 6;
                            if( Consulta_grupos_cli( _consulta_grupos_cli ) ) {
                                cod_grupo = grupocli_sic.cod_grupo_cliente;
                                rta = 1;
                            }
                            else {
                                mensaje_1 = 1;
                            }
                            /*--------------------- finaliza ---------------------*/
							CLOSE_TABLE( T_GRUPO_CLI, TT_ORIG );
                        }
                        if( rta ) {
                            digitos = 20;
                        rta = PEDIR_CADENA_EN_RECUADRO( ST( S_CODIGO_GENERICO ), SI, digitos,
                                                            cadena, NULL );
                        }
                        if( rta && cod_grupo != -1 ) {
                            rta = BUSCAR_CLIENTE_POR_COD_GENERICO( cadena, cod_grupo );
                        }
                        mensaje = !rta;
                    break;

                case MULTIPLE:
                    /*-------------------- Prepara la pantalla -----------------------*/
                    rta = 0;
                    while( !rta ) {
                        #if defined(INVEL_L) || defined(INVEL_W)
                        rta = MENU( " BUSCAR CLIENTE POR ", items, y, NULL, 52 );
                        #else
                        rta = MENU( 0, 17, " BUSCAR CLIENTE POR ", items, y, 120, 52 );
                        #endif
                        switch( rta ) {
                            case _BUSQUEDA_NOMBRE:
                                mostrar_cliente = 1;
                                rta = ELEGIR_CLIENTE_ALFABETICO( 1, NO );
                                mensaje = !rta;
								if( rta ) {
                                    valida_cliente = 0;
								}
								break;

                            case _BUSQUEDA_DNI:
                                mostrar_cliente = 0;
                                _PERMITIR_SCANNER = SI;
                                digitos = 9;
								_PERMITIR_BANDA = SI;

								cod_numerico = PEDIR_ENTERO_EN_RECUADRO( ST( S_DNI ), NO, digitos );
								
								
								if( cod_numerico == -2 ) {
									cod_numerico =LEER_CLIENTE_BANDA( inicio, fin , &lectura_manual,buffer1, buffer2  );
                                    if(cod_numerico > 0 && config_tps.ClienteTarjetaEspecial && buffer2 ){
										int cod_seguridad = atol( buffer2 );
										if(config_tps.ClienteTarjetaEspecial && config_tps.Codigo_seguridad_cliente == cod_seguridad  ){
											lectura_banda =1;
										}
									}
                                }
								_PERMITIR_BANDA = NO;
								if( cod_numerico > 0 ) {
                                    rta = EXISTE_CLIENTE_DNI( cod_numerico );
									mensaje = !rta;
                                    if( !rta ) {
									    MENSAJE_STRING( S_EL_CLIENTE_NO_EXISTE );
                                        BORRAR_MENSAJE();
                                    }
                                } else {
									rta = 0;
                                }
								_PERMITIR_SCANNER = NO;

                                break;

                            case _BUSQUEDA_CUIT :
                                mostrar_cliente = 0;
                                digitos = 14;
                                if( recuadro ) {
									cod_numerico = PEDIR_ENTERO_EN_RECUADRO( ST( S_CUIT ), NO, digitos );
                                }
                                else {
                                    cod_numerico = PEDIR_CADENA_XY( ST( S_CUIT ), 40 - ancho_panel,
                                                               20, digitos, cuit );
                                }

                                if( cod_numerico > 0 ) {
									if( recuadro ) {
										ltoa( cod_numerico, cuit, 10 );
									}
									TRIM( cuit );
                                    rta = BUSCAR_CLIENTE_POR_CUIT( cuit );
                                    if( !rta  ) {
                                        MENSAJE_STRING( S_EL_CLIENTE_NO_EXISTE );
                                        BORRAR_MENSAJE();
                                    }
                                }
                                else {
                                   rta = 0;
								   mensaje = 1;
                                }
								break;

                           case _BUSQUEDA_CODIGO:
								mostrar_cliente = 0;
                                digitos = 10;
								
								_PERMITIR_BANDA=SI;
								cod_numerico = PEDIR_ENTERO_EN_RECUADRO( ST( S_COD_CLIENTE ), NO, digitos );
								lectura_banda =0;
								if( cod_numerico == -2 ){ 
									cod_numerico =LEER_CLIENTE_BANDA( inicio, fin , &lectura_manual,buffer1, buffer2  );
									if(cod_numerico > 0 && config_tps.ClienteTarjetaEspecial && buffer2 ){
										int cod_seguridad = atol( buffer2 );
										if(config_tps.ClienteTarjetaEspecial && config_tps.Codigo_seguridad_cliente == cod_seguridad  ){
											lectura_banda =1;
										}
									}
								}

								_PERMITIR_BANDA=NO;
								if( cod_numerico > 0 ) {
									_ltoa( cod_numerico, codigo, 10 );
									TRIM( codigo );
                                    rta = EXISTE_CLIENTE_POR_CODIGO( codigo );
                                    if( !rta ) {
									    MENSAJE_STRING( S_EL_CLIENTE_NO_EXISTE );
                                        BORRAR_MENSAJE();
                                    }
                                } else {
									rta = 0;
									mensaje = 1;
                                }
                                break;

                            case _BUSQUEDA_BARRA:
                                nro = BUSCAR_CLIENTE_COD_BARRA();
                                if( nro > 0 ) {
                                    rta = EXISTE_CLIENTE( nro );
                                    mensaje = !rta;
                                    if( !rta ) {
                                        MENSAJE_STRING( S_EL_CLIENTE_NO_EXISTE );
                                        BORRAR_MENSAJE();
                                    }
                                } else {
                                    //Ingreso un nro incorrecto o que no cumple con el tamaño de barra
                                    if( LASTKEY() != 27 ) {
                                        MENSAJE( "CODIGO INCORRECTO" );
                                        BORRAR_MENSAJE();
                                    }
                                    rta = 0;
                                }
                                break;

                            case _BUSQUEDA_GENERICO:
                    cod_grupo = -1;
                    _ATRIBUTO = 120;
                    //area_ant = SELECTED();
                   // area_auxi = AREA_DISPONIBLE();
					area_ant = SELECTED2();
					tipo_ant = SELECTED_TIPO();
                    //if( USE_DB( area_auxi, _GRUPOCLI, ( char* )&grupocli_sic,
                    //            sizeof( struct _grupos_clientes ), NULL, 0, 0 ) == 0 ) {
					if( OPEN_TABLE( T_GRUPO_CLI, TT_ORIG, ( char* )&grupocli_sic, sizeof( struct _grupos_clientes ) ) == 0 ){
                        int cant_datos;
						SELECT_TABLE( T_GRUPO_CLI, TT_ORIG );
						RUN_QUERY(NO);

                        //cant_datos = RECCOUNT(  );
                        cant_datos = 6;
                        if( Consulta_grupos_cli( _consulta_grupos_cli ) ) {
                            cod_grupo = grupocli_sic.cod_grupo_cliente;
                            rta = 1;
                        }
                        else {
                            mensaje_1 = 1;
                        }
                        /*--------------------- finaliza ---------------------*/
                        //CLOSE_DATABASE( area_auxi );
						CLOSE_TABLE( T_GRUPO_CLI, TT_ORIG );
                    }
                    //SELECT( area_ant );
					SELECT_TABLE( area_ant, tipo_ant );
                    if( rta ) {
                        digitos = 20;
                                    rta = PEDIR_CADENA_EN_RECUADRO( ST( S_CODIGO_GENERICO ), SI, digitos,
                                                        cadena, NULL );
                    }
                                if( rta && cod_grupo != -1 ) {
                                    rta = BUSCAR_CLIENTE_POR_COD_GENERICO( cadena, cod_grupo );
                                }
                                if( !rta && LASTKEY() != 27 ) {
                                    MENSAJE_STRING( S_EL_CLIENTE_NO_EXISTE );
                                    BORRAR_MENSAJE();
                                }
                                mensaje = !rta;
                                break;

                            case ALTA_CLI:
                                alta_cliente = 1;
                                break;
                        }
                    }
                    break;

                case COD_BARRA:
                    nro = BUSCAR_CLIENTE_COD_BARRA();
                    if( nro > 0 ) {
                        rta = EXISTE_CLIENTE( nro );
                        mensaje = !rta;
                        if( !rta ) {
                            MENSAJE_STRING( S_EL_CLIENTE_NO_EXISTE );
                            BORRAR_MENSAJE();
                        }
                    } else {
                        //Ingreso un nro incorrecto o que no cumple con el tamaño de barra
                        if( LASTKEY() != 27 ) {
                            codigo_incorrecto = 1;
                            MENSAJE( "CODIGO INCORRECTO" );
                            BORRAR_MENSAJE();
                        }
                        rta = 0;
                    }
                    break;
			}
			if( rta && !alta_cliente && LASTKEY() != 27 ) {
				if( !FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
					datos_invalidos = !VALIDAR_DATOS_CLIENTE_FISCAL(  );
					if( datos_invalidos )
						rta = 0;
				} 
				if( rta ) {
					SET_MEMORY_LONG( __ram_nro_cliente, clientes.cod_cliente );
					CARGAR_PERFIL_CLIENTE( ( int )clientes.cod_perfil );
					//rox ..para que lo muestre siempre que lo pida
					if( !mostrar_cliente ) {
						//MOSTRAR_DATOS_BOX_CLIENTE();
					}
				}
            }
            else {
                SET_MEMORY_CHAR( __ram_mostrar_saldo_cheque, 0 );
            }
            /*}*/
            /*---------------------- Muestra cliente inexistente ----------------*/
			if( mensaje && !mensaje_1 && LASTKEY() != 27 && !alta_cliente ) {
                MENSAJE_CON_PAUSA( ST( S_EL_CLIENTE_NO_EXISTE ), 3 );
                if( BUSQUEDA_CLIENTE != MULTIPLE && ALTA_DE_CLIENTE_HABILITADA ) {
			        MENSAJE( "DESEA DAR DE ALTA AL CLIENTE S/N?" );
			        TECLADO_ALFABETICO = 1;
                    do {
                        car = GETCH();
                    }
                    while( car != 'S' && car != 'N' && car != 's' && car != 'n' );
                    BORRAR_MENSAJE();
                    TECLADO_ALFABETICO = 0;
                } else {
                    if( LASTKEY() != 27 ) {
						MENSAJE_CON_PAUSA( ST( S_EL_CLIENTE_NO_EXISTE ), 3 );
					}
                }
            }
			else {
				if( lectura_manual ) {
                    if( LASTKEY() != 27 && !codigo_incorrecto && !alta_cliente && !datos_invalidos ) {
                        if( valida_cliente && !VALIDA_CLIENTE( ) ) {
                            SET_MEMORY_LONG( __ram_nro_cliente, 0 );
                            CEREAR_PERFIL_CLIENTE();
                        }
                    } else {
                        SET_MEMORY_LONG( __ram_nro_cliente, 0 );
                        CEREAR_PERFIL_CLIENTE();
						rta = 0;
                    }
                }
            }
        }
	}
    else {
        MENSAJE_STRING( S_NO_ESTA_HABILITADA_FACTURACION_CHEQUES_CLIE );
    }
    if( !datos_invalidos && ( ( !rta && LASTKEY() != 27 && ( car == 'S' || car == 's' )
            && BUSQUEDA_CLIENTE != MULTIPLE ) || alta_cliente ) ) {
        BORRAR_MENSAJE();
        if( ALTA_CLIENTE( ) ) {
            rta = 1;
        } else {
            rta = 0;
            if( permitir_escape ) {
                rta = 1;
            }
        }
    }
    /*---------------------------- fin ---------------------------------*/
    if( items != NULL ) {
        for( i = 0; i < y; i++ ) {
            free( items[i].nom );
        }
        free( items );
    }
    if( permitir_escape && !datos_invalidos ) {
        if( car == 'N' || car == 'n' || LASTKEY() == 27 ) {
            //Para salir del While de pago
            rta = 1;
        }
    }
	if( rta == -1 ){
		rta = NO;
	}

    return ( rta );
}

/*****************************************************************************/
void MOSTRAR_CLIENTE( int x, int y, long nro )
/*****************************************************************************/
{
    if( USAR_CUIT_COMO_NRO_CLIENTE ) {
        EXISTE_CLIENTE( nro );
        MOSTRAR_CADENA( x, y, clientes.cuit );
    }
    else {
        MOSTRAR_ENTERO( x, y, "00000000", nro );
    }
}
/***********************************************************************/
int EXISTE_CLIENTE( long cliente )
/***********************************************************************/
{
    int rta/*,indice*/;
    if( !GET_ERROR_EN_RED() ) {
		if( cliente != clientes.cod_cliente ) {
			SELECT_TABLE( T_CLIENTES, TT_ORIG );
			SET_ORDER2("COD_CLIENTE");
			SET_WHERE("COD_CLIENTE = %ld",cliente);
			RUN_QUERY(SI);
            rta = FOUND2();
        }
        else {
            rta = 1;
        }
        if( rta ) {
            SET_MEMORY( __cliente_afinidad_nombre, clientes.nombre );
        }
        else {
            CEREAR_MEMORY( __cliente_afinidad_nombre );
        }
    }
    else {
        rta = 0;
    }
    return ( rta );
}
/***********************************************************************/
int BUSCAR_CLIENTE_POR_CUIT( char *cuit )
/***********************************************************************/
{
    int rta = 0;
    if( config.pais == ECUADOR ) {
        cuit[10] = 0;
    }
    if( config.pais == ELSALVADOR ) {
        cuit[14] = 0;
    }
    TRIM( cuit );
    if( !GET_ERROR_EN_RED() ) {
        //SELECT( AREA_CLIENTES );
		SELECT_TABLE( T_CLIENTES, TT_ORIG );
        //GO( TOP );
        //SET_ORDER( 3 );
        //GET_EQUAL( ( char* )cuit );
		SET_ORDER2("CUIT");
		SET_WHERE("CUIT = '%s'", cuit);
		RUN_QUERY(SI);
        if( FOUND2() ) {
            //SET_ORDER( 1 );
            rta = 1;
            SET_MEMORY_LONG( __ram_nro_cliente, clientes.cod_cliente );
            SET_MEMORY( __cliente_afinidad_nombre, clientes.nombre );
            CARGAR_PERFIL_CLIENTE( ( int )clientes.cod_perfil );
        }
        else {
            SET_MEMORY_LONG( __ram_nro_cliente, 0 );
            CEREAR_MEMORY( __cliente_afinidad_nombre );
            CEREAR_PERFIL_CLIENTE();
        }
    }
    return ( rta );
}
/***********************************************************************/
void ACTUALIZA_SALDO_CLIENTE( double importe )
/***********************************************************************/
{
    if( !GET_ERROR_EN_RED() ) {
        if( !EMITIR_FACTURA_REMOTA() ) {
         char where[40];
            _snprintf( where, 40, "cod_cliente = %ld", clientes.cod_cliente );
            where[39] = '\0';
            //SELECT( AREA_CLIENTES );
			SELECT_TABLE( T_CLIENTES, TT_ORIG );
            clientes.saldo += importe;
            clientes.replicar = 'S';
			UPDATE2();
			// aca le decimos al jsincro que envie los datos de cuenta corriente
			//se quita porque el triguer en la base cuando se hace un pago actualiza el saldo
		/*	if( fabs(importe) > 0.00 && INSERT_SCRIPT_POS( SPOS_UPDATE_CLIENTE, where ) > -1 ) {
               glog(ST( S_CLIENTE_REGISTRADO_EXITOSAMENTE ),LOG_VENTAS,2);
			} else
			   glog("ERROR EN ACTUALIZAR SALDO CLIENTE",LOG_VENTAS,2);*/


         //
            /*#ifdef INVEL_L
            BEGIN_TRANSACTION();
            #endif
            UPDATE();
            #ifdef INVEL_L
            END_TRANSACTION();
            #endif*/
        }
    }
}
/***********************************************************************/
int VERIFICA_CUPO_CLIENTE( double importe, double importe_ant, char cobro, char modo )
/***********************************************************************/
{
    int rta = 2, h, rtta=0;
	char cadena[80];

	memset( cadena, 0, sizeof( cadena ) );

    if( !CONTROLAR_CUPO_EN_VENTAS && modo != _MODO_VENTA ) {
        for( h = 0;h < RAM_P_PAGO;h ++ ) {
            if( medios[_PAGO_MODO( h )].cta_cte ) {
                importe += _PAGO_IMPORTE( h );
            }
        }
    }

    //aca controlo si hay conexion con la base para saber si tengo actualizado el clientes
    if( ConfIni.odbcServer[0] != 0 && RAM_NRO_CLIENTE 
			&& RAM_P_TICKET == 1 && CONTROLAR_CUPO_EN_VENTAS 
			&& clientes.ctacte == 'S')
		rtta= EXISTE_CLIENTE_SERVIDOR(); //por aho solo dejo un mensaje de falta de conexion pero no impido seguir

    if( COD_FINANCIACION_POR_DEFECTO && !NRO_COD_CUOTA && CONTROLAR_CUPO_EN_VENTAS && RAM_NRO_CLIENTE ) {
       int nro;
       nro = COD_FINANCIACION_POR_DEFECTO;
       SET_MEMORY_INT( __nro_cod_cuota, nro ) ;
    }

	if( UTILIZA_SOCIOS && RAM_NRO_CLIENTE && NRO_COD_CUOTA && !cobro ) {
		if( !MODO_DEVOLUCION && !RAM_NOTA_CR ) { 
			if( clientes.cupo_sobre_giro == 0 && importe > ( clientes.cupo - clientes.saldo )
				&& ( importe_ant < ( clientes.cupo - clientes.saldo  || importe_ant == 0 ) )) {
				rta = 0;
				_snprintf( cadena, sizeof(cadena)-1, "%s %s", ST( S__CUPO_EXCEDIDO_  ), 
					ST( S____PRESIONE_UNA_TECLA__ ) );
				MENSAJE_TECLA( cadena );
				if( modo == _MODO_VENTA ) {
					SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
					SET_MEMORY_INT( __ram_cant_bultos, 1 );
					CR1_MUESTRA_MULTIPLICACION();
				}
			}
			else if( clientes.cupo_sobre_giro
				  && importe > ( clientes.cupo + clientes.cupo_sobre_giro - clientes.saldo ) ) {
				rta = 0;
				_snprintf( cadena, sizeof(cadena)-1, "%s %s", ST( S_SOBREGIRO_EXCEDIDO ), 
					ST( S____PRESIONE_UNA_TECLA__ ) );
				MENSAJE_TECLA( cadena );
				if( modo == _MODO_VENTA ) {
					SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
					SET_MEMORY_INT( __ram_cant_bultos, 1 );
					CR1_MUESTRA_MULTIPLICACION();
				}
			}
			else if( clientes.cupo_sobre_giro
				  && importe <= ( clientes.cupo + clientes.cupo_sobre_giro - clientes.saldo )
				  && importe > ( clientes.cupo - clientes.saldo ) ) {
				rta = SOLICITAR_CLAVES( 30, 20, NO, SI, SI, ST( S__CUPO_EXCEDIDO_ ),
										ST( S_AUTORIZO_CUPO_EXCEDIDO ), _F_SUP_CUPO_EXCEDIDO, SI );
			}
		} else {
			if( clientes.cupo && importe > clientes.saldo ) {
				rta = 0;
				_snprintf( cadena, sizeof(cadena)-1, "%s %s", ST( S_NO_DISPONE_CUPO_USADO ), 
					ST( S____PRESIONE_UNA_TECLA__ ) );
				MENSAJE_TECLA( cadena );
			}
		}
    }
    return ( rta );
}
/***********************************************************************/
int VERIFICA_SALDO_CHEQUE( double importe )
/***********************************************************************/
{
    int rta = 1;
    char men[100];
    if( PEDIR_CLIENTE_EN_CHEQUES && CONTROLAR_CUPO_EN_CHEQUES ) {
        if( !GET_ERROR_EN_RED() ) {
            if( importe > ( clientes.limite_cheques - clientes.cupo_usado_cheques ) ) {
                sprintf( men, ST( S__EXCEDIDO__EXCEDE_____2F_ ),
                         importe - ( clientes.limite_cheques - clientes.cupo_usado_cheques ) );
                rta = SOLICITAR_CLAVES( 30, 20, NO, SI, SI, men, ST( S_AUTORIZO_CUPO_EXCEDIDO ),
                                        _F_SUP_CUPO_EXCEDIDO, SI );
            }
        }
    }
    return ( rta );
}
/***********************************************************************/
int VALIDA_CLIENTE()
/***********************************************************************/
{
    int rta = 1;
    MOSTRAR_DATOS_BOX_CLIENTE();
    rta = SOLICITAR_CLAVES( 30, 21, NO, SI, SUPERVISOR_PARA_CLIENTE_MANUAL, "SUPERVISOR", NULL,
                            _F_SUP_CLIENTE_MANUAL, SI );
    return ( rta );
}
/***********************************************************************/
int GRABA_SALDO_CHEQUE_CLIENTE( double tot_cheques )
/***********************************************************************/
{
    int rta = 0, reintento = 0;
    long cliente;
    unsigned long pos;
    //SELECT( AREA_CLIENTES );
	SELECT_TABLE( T_CLIENTES, TT_ORIG );
    //SET_ORDER( 1 );
    cliente = RAM_NRO_CLIENTE;
	SET_ORDER2("COD_CLIENTE");
	SET_WHERE("COD_CLIENTE = %ld",cliente);
	RUN_QUERY(SI);
    //GET_EQUAL( ( char* )&cliente );
    if( FOUND2() ) {
        clientes.cupo_usado_cheques += tot_cheques;
        /*#ifdef INVEL_L
        BEGIN_TRANSACTION();
        #endif*/
        while( ( reintento < 3 ) && UPDATE2() ) {
            pos = GET_POSITION_REG();
            GET_DIRECT_REG( pos, GET_PAGINA_REG());
            clientes.cupo_usado_cheques += tot_cheques;
            clientes.replicar = 'S';
            reintento++;
        }
        #ifdef INVEL_L
        END_TRANSACTION();
        #endif
        rta = 1;
        SET_MEMORY( __cliente_afinidad_nombre, clientes.nombre );
    }
    return rta;
}
/***********************************************************************/
int ACTUALIZA_CLIENTES( void )
/***********************************************************************/
{
    char mens[80];
    int error = 0, rtabtr = 0, ok;
    int nov_tipo;
    //Ya esta definida en tpv_dat.h y es mas facil mantenerla similar a la tabla clientes
    /*struct _clientes_nov
    {
        BINARY status;
        unsigned long novedad;
        char novedadregistro;
        long cod_cliente;
        char buffer[440];
    }        *clientes_nov;*/
    struct _clientes_nov *clientes_nov;

    //(Nota Unificacion MySQL-Linux): la siguiente verificacion de opend no tiene sentido en mysql
	//if( !OPENED( T_ARTICULO, TT_ORIG )) {
        ok = ABRIR_ARCHIVO_CLIENTE();
    //}
    //else {
    //    ok = 1;
    //}
    if( !ok ) {
        return ( 1 );
    }
    clientes_nov = (struct _clientes_nov *)MALLOC( sizeof( struct _clientes_nov ) );
    if( clientes_nov == NULL ) {
        return ( 1 );
    }
	if( OPENED( T_CLIENTES_NOV, TT_ORIG ) ) {
		CLOSE_TABLE( T_CLIENTES_NOV, TT_ORIG );
	}
    GRABAR_LOG_SISTEMA( BUSCA_MENSAJE_STRING( S_ACTUALIZANDO_CLIENTES ),LOG_VENTAS,3 );
    //if( !OPEN_TABLE( T_CLI_AFI, TT_ORIG, ( char* )clientes_nov, sizeof( struct _clientes_nov ) ) ) {
    if( !OPEN_TABLE( T_CLIENTES_NOV, TT_ORIG, ( char* )clientes_nov, sizeof( struct _clientes_nov ) ) ) {
        GO2( TOP );
		SET_WHERE("");
		RUN_QUERY(NO);
        while( !dbEOF() && !rtabtr ) {
            nov_tipo = ( char )clientes_nov->novedadregistro;
            clientes_nov->novedadregistro = 0;;
			SELECT_TABLE( T_CLIENTES, TT_ORIG );
            switch( nov_tipo ) {
                case MODIFICAR_CLIENTE:
                case NOVEDAD_CLIENTE:
                    sprintf( mens, "Novedad cliente: %li", clientes_nov->cod_cliente );
                    GRABAR_LOG_DBROUTER( mens ,LOG_DEBUG,4);
                    if( !EXISTE_CLIENTE( clientes_nov->cod_cliente ) ) {
                        memcpy( &clientes, &clientes_nov->novedadregistro,
                                sizeof( struct _clientes ) );
                        if( config.pais == ECUADOR ) {
                            clientes.cuit[10] = 0;
                        }
                       /* #ifdef INVEL_L
                        BEGIN_TRANSACTION();
                        #endif*/
                        rtabtr = INSERT2(SI);
                        /*#ifdef INVEL_L
                        END_TRANSACTION();
                        #endif*/
                    }
                    else {
                        memcpy( &clientes, &clientes_nov->novedadregistro,
                                sizeof( struct _clientes ) );
                        if( config.pais == ECUADOR ) {
                            clientes.cuit[10] = 0;
                        }
                        /*#ifdef INVEL_L
                        BEGIN_TRANSACTION();
                        #endif*/
                        rtabtr = UPDATE2();
                        /*#ifdef INVEL_L
                        END_TRANSACTION();
                        #endif*/
                    }
                    break;
                case BORRAR_CLIENTE:
                    sprintf( mens, "Borrar cliente: %li", clientes_nov->cod_cliente );
                    GRABAR_LOG_DBROUTER( mens ,LOG_DEBUG,4);
                    if( EXISTE_CLIENTE( clientes_nov->cod_cliente ) ) {
                        /*#ifdef INVEL_L
                        BEGIN_TRANSACTION();
                        #endif*/
                        rtabtr = DELETE2();
                        /*#ifdef INVEL_L
                        END_TRANSACTION();
                        #endif*/
                        //GRABAR_LOG_DBROUTER("Delete");
                    }
                    break;
                default:
                    rtabtr = -1;
                    GRABAR_LOG_DBROUTER( "Novedad no reconocida" ,LOG_DEBUG,2);
            }
            //SELECT_TABLE( T_CLI_AFI, TT_ORIG );
            SELECT_TABLE( T_CLIENTES_NOV, TT_ORIG );
            if( !rtabtr ) {
                SET_MEMORY_LONG( __nro_ultima_novedad_cliente, clientes_nov->novedad );
            }
            SKIP2( 1 );
        }
        //CLOSE_TABLE( T_CLI_AFI, TT_ORIG );
        CLOSE_TABLE( T_CLIENTES_NOV, TT_ORIG );
        error = rtabtr;
    }
    else {
        error = 1;
    }
    GRABAR_LOG_DBROUTER( "Free" ,LOG_DEBUG,4);
    FREEMEM( ( char* )clientes_nov );
    return ( error );
}
/***********************************************************************/
int VERIFICA_EMPADRONAMIENTO_CLIENTES( int recuadro )
/***********************************************************************/
{
    int rta,tecla/*,area*/;
    int tabla_ant, tipo_ant;
    rta = PEDIR_NRO_CLIENTE( recuadro, NO );
    if( LASTKEY() == 27 ) {
        return ( 0 );
    }
    if( !rta ) {
        MENSAJE_SIN_SONIDO_STRING( S_DESEA_EMPADRONAR_EL_CLIENTE_ENTER_SI );
        tecla = 0;
        while( tecla != 13 && tecla != 27 ) {
            tecla = GETCH();
        }
        if( tecla != 13 ) {
            rta = 1;
        }
    }
    else {
        MENSAJE_STRING( S_CLIENTE_YA_REGISTRADO );
    }
    BORRAR_MENSAJE();
    if( !rta ) {
        if( PEDIR_DATOS_CLIENTE() ) {
			tabla_ant= SELECTED2();
			tipo_ant = SELECTED_TIPO();
			SELECT_TABLE( T_CLIENTES, TT_ORIG );
			INSERT2(SI);
			SELECT_TABLE( tabla_ant, tipo_ant );
            MENSAJE_STRING( S_CLIENTE_REGISTRADO_EXITOSAMENTE );
        }
    }
    return ( rta );
}
/***********************************************************************/
int PEDIR_DATOS_CLIENTE( void )
/***********************************************************************/
{
    //char aux[20];
    int ok = 0,  pant2 = 0, cant_datos = 0, rta = 1, x = 0, y = 0, tecla, cant = 0, cliente = 0;
    int cond_retencion = 0, reintento = 0;
    struct   _grupos_clientes grupocli_sic;
    int cant_datos_config = 0;
    int cant_campos_oblig = 8;
    int ind_configTmp = 0;
    int posicion = 0, digitos_ing = 0;
    char *cad;
    grupocli = &grupocli_sic;

    cant_datos = CANT_DATOS_SOLICITAR + cant_campos_oblig;
    cant =  strlen( ST( S_NOMBRE_CLIENTE ) )
            + strlen( ST( S_DOMICILIO_CLIENTE ) )
            + strlen( ST( S_LOCALIDAD ) )
            + strlen( ST( S_PROVINCIA ) )
            + strlen( ST( S_TELEFONO_CLIENTE ) )
            + strlen( ST( S_COND_IVA ) )
            + strlen( ST( S_CTA_CTE_S_N ) )
            +  cant_datos;

    //Sumo el tamaño de las cadenas configuradas.
    ind_config = ind_comienzo_datos_solicitar + 1;

    while( CONFIG_CLIENTES[ind_config].codigo != _FIN_CONFIG_DATOS_CLIENTE
            && CONFIG_CLIENTES[ind_config].codigo == _CAMPOS_CLIENTES ) {
        cant += strlen( &CONFIG_CLIENTES[( ind_config )].parametros[ind_configTmp] );
        ind_config++;
        cant_datos_config ++;
    }

    //Valido que la cantidad configuradas coincidan con la cantidad de campos configurados.
    if( cant_datos_config && CANT_DATOS_SOLICITAR != cant_datos_config  ) {
        MENSAJE( ST( S_ERROR_CANT_DE_DATOS_CONFIGURADOS ) );
        BORRAR_MENSAJE();
        return 0;
    }

	if( ( cad = ( char* )malloc( cant ) ) != NULL ) {
		memset( cad, 0, cant );
        strncat( cad, ST( S_NOMBRE_CLIENTE ), strlen( ST( S_NOMBRE_CLIENTE ) ) );
        strcat( cad, "~" );// "-" sirve de separador en Java
        strncat( cad, ST( S_DOMICILIO_CLIENTE ), strlen( ST( S_DOMICILIO_CLIENTE ) ) );
        strcat( cad, "~" );// "-" sirve de separador en Java
        strncat( cad, ST( S_LOCALIDAD ), strlen( ST( S_LOCALIDAD ) ) );
        strcat( cad, "~" );// "-" sirve de separador en Java
        strncat( cad, ST( S_PROVINCIA ), strlen( ST( S_PROVINCIA ) ) );
        strcat( cad, "~" );// "-" sirve de separador en Java
        strncat( cad, ST( S_TELEFONO_CLIENTE ), strlen( ST( S_TELEFONO_CLIENTE ) ) );
        strcat( cad, "~" );// "-" sirve de separador en Java
        strncat( cad, ST( S_COND_IVA ), strlen( ST( S_COND_IVA) ) );
        strcat( cad, "~" );// "-" sirve de separador en Java
        strncat( cad, ST( S_CTA_CTE_S_N ), strlen( ST( S_CTA_CTE_S_N ) ) );

        if( cant_datos_config ) {
            strcat( cad, "~" );// "-" sirve de separador en Java

            //Concatenamos las cadenas de los campos configurados
            ind_config = ind_comienzo_datos_solicitar + 1;

            while( CONFIG_CLIENTES[ind_config].codigo != _FIN_CONFIG_DATOS_CLIENTE
                    && CONFIG_CLIENTES[ind_config].codigo == _CAMPOS_CLIENTES ) {

                strncat( cad, &CONFIG_CLIENTES[( ind_config )].parametros[ind_configTmp]
                        , strlen( &CONFIG_CLIENTES[( ind_config )].parametros[ind_configTmp] ) );
                //Verificamos que el sig. (ind_config + 1) no sea el FIN, para no agregar "~"
                if( CONFIG_CLIENTES[ind_config + 1].codigo != _FIN_CONFIG_DATOS_CLIENTE
                        && CONFIG_CLIENTES[ind_config + 1].codigo == _CAMPOS_CLIENTES ) {
                    strcat( cad, "~" );// "-" sirve de separador en Java
                }

                ind_config++;
            }
        }
    }
    /*******************************   SE PIDEN DATOS   *******************************/

    TECLADO_ALFABETICO = 1;
    memset( &clientes, 0, sizeof( clientes ) );

    if( config.pais == ELSALVADOR ) {
        int area_ant, tipo_ant;
		area_ant = SELECTED2();
		tipo_ant = SELECTED_TIPO();
        rta = 0;
       	// if( USE_DB( AREA_AUX8, _GRUPOCLI, ( char* )&grupocli_sic, sizeof( struct _grupos_clientes ), NULL,
       	//             0, 0 ) == 0 ) {
		if( OPEN_TABLE( T_GRUPO_CLI, TT_ORIG, ( char* )&grupocli_sic, sizeof( struct _grupos_clientes ) ) == 0 ){
        	int cant_datos;
			SELECT_TABLE( T_GRUPO_CLI, TT_ORIG );
			RUN_QUERY(NO);

            cant_datos = 6;
            //rta = Consulta_tipos_de_clientes( _consulta_grupos_cli );
			rta = Consulta_grupos_cli( _consulta_grupos_cli );
            if( rta ) {
                clientes.cod_grupo_cliente = grupocli_sic.cod_grupo_cliente;
                rta = 1;
            }
            //CLOSE_DATABASE( AREA_AUX8 );
			CLOSE_TABLE( T_GRUPO_CLI, TT_ORIG );
        }
       // SELECT( area_ant );
		SELECT_TABLE( area_ant, tipo_ant );
    }

    ind_config = ind_comienzo_datos_solicitar + 1;
    ExecuteSSAction( A_PEDIR, _CONCATENADO, ST( S_ALTA_DE_CLIENTES ), cad, NULL, " " );
    ExecuteSSAction( A_SET_PROP_INPUT, _INPUT_ALTA_CLIENTES, "_CONCATENADO", "", NULL, " " );

        if( cad ) {
            free( cad );
        }
        if( rta ) {
        int fin=0;
        char con_iva[31];
        memset( con_iva, 0, 31 );
		while( !fin ) {
			digitos_ing = 1;
			TECLADO_ALFABETICO = 1;
			ExecuteSSAction( A_PEDIR_DATO, posicion, " ", " ", NULL, " " );
			switch ( posicion ) {
				case 0: //DATO OBLIGATORIO - NOMBRE
					rta = PEDIR_CADENA( ( char )( x + 23 ), ( char )( y + 1 + posicion ), ( char )30,
							clientes.nombre, NULL, NO, SI );
					TRIM( clientes.nombre );
					digitos_ing = strlen( clientes.nombre );
				break;
				case 1: //DATO OBLIGATORIO - DOMICILIO
					rta = PEDIR_CADENA( ( char )( x + 23 ), ( char )( y + 1 + posicion ), ( char )40,
							clientes.domicilio, NULL, NO, SI );
					TRIM( clientes.domicilio );
					digitos_ing = strlen( clientes.domicilio );
				break;
				case 2: //DATO OBLIGATORIO - LOCALIDAD
					rta = PEDIR_CADENA( ( char )( x + 23 ), ( char )( y + 1 + posicion ), ( char )20,
							clientes.localidad, NULL, NO, SI );
					TRIM( clientes.localidad );
					digitos_ing = strlen( clientes.localidad );
				break;
				case 3: //DATO OBLIGATORIO - PROVINCIA
					rta = PEDIR_CADENA( ( char )( x + 23 ), ( char )( y + 1 + posicion ), ( char )15,
							clientes.provincia, NULL, NO, SI );
					TRIM( clientes.provincia  );
					digitos_ing = strlen( clientes.provincia );
				break;
				case 4: //DATO OBLIGATORIO - TELEFONO
					rta = PEDIR_CADENA( ( char )( x + 23 ), ( char )( y + 1 + posicion ), ( char )20,
							clientes.telefono, NULL, NO, SI );
					TRIM( clientes.telefono );
					digitos_ing = strlen( clientes.telefono );
				break;
				case 5: //DATO OBLIGATORIO - CONDICION IVA
					if(!strlen(con_iva)){
						rta = ASIGNA_CONDICION_IVA( con_iva );
					}else{
						MOSTRAR_CADENA( x + 23, y + 1 + posicion, con_iva );
						MOSTRAR_INPUT_CADENA( con_iva, posicion );
						while((rta = GETCH()) == -999);
						//si no es ni flecha arriba ni flecha abajo ni enter ni esc
						if(rta != -72 && rta != -80 && rta != 27 && rta != 13)
							rta = ASIGNA_CONDICION_IVA( con_iva );
					}
					if(rta){
						MOSTRAR_CADENA( x + 23, y + 1 + posicion, con_iva );
						MOSTRAR_INPUT_CADENA( con_iva, posicion );
					}
					TRIM( con_iva );
					digitos_ing = strlen( con_iva );
				break;
				case 6: //DATO OBLIGATORIO - CUENTA CORRIENTE
					//clientes.ctacte = 0;
					{
					char ctacte[2];
					ctacte[0] = clientes.ctacte;
					ctacte[1] = 0;
					rta = PEDIR_CADENA( ( char )( x + 23 ),
							( char )( y + 1 + posicion ), ( char )1,ctacte, NULL, NO, SI );
					clientes.ctacte = ctacte[0];
					if( (clientes.ctacte == ' ' || clientes.ctacte == 0) && rta ) {
						MOSTRAR_INPUT_CADENA( "N", posicion );
						clientes.ctacte = 'N';
					}
					//En caso de ingresar incorrectamente retrocedemos la posicion
					if( clientes.ctacte != 'S' && clientes.ctacte != 'N'
						&& clientes.ctacte != 's' && clientes.ctacte != 'n' && rta ) {
						posicion--;
					}
					}
				break;
				default:
					if( cant_datos_config  ) {
						if( (rta = INGRESAR_DATOS_CLIENTES_CONFIG( &posicion )) ) {
							digitos_ing = 1;
							ok = 1 ;
						}
					}
					//fin = SI;
					//posicion--;
					if( rta > 0 )
						fin = SI;
			}
			if( -72 == rta ){ //si es flecha arriba
				posicion -= ( posicion ) ? 1 : 0;
			} else if( !digitos_ing  && rta ) {
				MENSAJE( ST( S_INGRESE_DATO_OBLIGATORIO ) );
				BORRAR_MENSAJE();
			} else {
				if( 0 != rta )
					posicion++;
				rta = SALIR_ALTA_CLIENTE();
				if( !rta ) {
					fin = SI;
					ok = 0;
				}
			}
        }
    }

    /********************** CONFIRMACION DE LOS DATO INGRESADOS **********************/
    if( ok ) {
        MENSAJE_STRING( S_CONFIRMA_LOS_DATOS_DEL_CLIENTE_ENTER_SI );
        tecla = 0;
        while( tecla != 13 && tecla != 27 ) {
            tecla = GETCH();
        }
        if( tecla != 13 ) {
            ok = 0;
        }
        BORRAR_MENSAJE();
    }
    //--- Fin
    ExecuteAction( A_FIN_PEDIR );
    return ( ok );
}
/***********************************************************************/
void VERIFICAR_CLIENTE_HABILITADO_PARA_CHEQUES()
/***********************************************************************/
{
    if( PEDIR_NRO_CLIENTE( SI, NO ) ) {
		if( CLIENTE_HABILITADO_PARA_CHEQUES() ){
        //if( clientes.cheques == 'N' ) {
           MENSAJE_STRING( S_CLIENTE_OK );
        }
        else {
			 MENSAJE_TECLA_STRING( S_CLIENTE_NO_HABILITADO_PARA_CHEQUES_ENTER );

        }
    }
    SET_MEMORY_LONG( __ram_nro_cliente, 0 );
    CEREAR_MEMORY( __cliente_afinidad_nombre );
}
/***********************************************************************/
void IDENTIFICAR_CLIENTE_POR_CONSULTA()
/***********************************************************************/
{
    char aux[20],*p;
    struct _rta_terminal rta_terminal;
    long cod_cliente;
    if( VAR_TARJETAS_STATUS == 0 ) {
        VALIDA_TARJETA( SI, NO, IDEN_CLIENTE, NO, NULL, NO, -1/*NO*/);
    }
    memset( &rta_terminal, 0, sizeof( rta_terminal ) );
    if( VAR_TARJETAS_STATUS > 0 ) {
        if( AUTORIZACION_ON_LINE( _ON_LINE_CONSULTA_CLIENTE, 0, ( char* )&rta_terminal,
                                  sizeof( rta_terminal ) ) && rta_terminal.cod_rta == 0 ) {
            p = strchr( rta_terminal.mensaje, '|' );
            if( p ) {
                p[0] = 0;
                strncpy( aux, &p[1], sizeof( aux ) );
                TRIM( aux );
                if( config.pais == ECUADOR ) {
                    aux[strlen( aux ) - 1] = 0;
                }
                cod_cliente = atol( aux );
                if( EXISTE_CLIENTE( cod_cliente ) ) {
                    if( rta_terminal.mensaje[0] != ' ' ) {
                        strncpy( clientes.nombre, rta_terminal.mensaje,
                                 sizeof( clientes.nombre ) - 1 );
                    }
                    if( tran_rta->track_2[0] && tran_rta->track_2[0] != ' ' ) {
                        strncpy( clientes.domicilio, tran_rta->track_2,
                                 sizeof( clientes.domicilio ) );
                    }
                    if( tran_rta->numero_de_comercio[0] && tran_rta->numero_de_comercio[0] != ' ' ) {
                        strncpy( clientes.telefono, tran_rta->numero_de_comercio,
                                 sizeof( clientes.telefono ) );
                    }
                    //SELECT( AREA_CLIENTES );
					SELECT_TABLE( T_CLIENTES, TT_ORIG );
                    /*#ifdef INVEL_L
                    BEGIN_TRANSACTION();
                    #endif*/
                    UPDATE2();
                    /*#ifdef INVEL_L
                    END_TRANSACTION();
                    #endif*/
                }
                else {
                    //SELECT( AREA_CLIENTES );
					SELECT_TABLE( T_CLIENTES, TT_ORIG );
                    memset( &clientes, 0, sizeof( clientes ) );
                    strncpy( clientes.nombre, rta_terminal.mensaje, sizeof( clientes.nombre ) - 1 );
                    clientes.cod_cliente = cod_cliente;
                    strncpy( clientes.cuit, p + 1, sizeof( clientes.cuit ) );
                    strncpy( clientes.domicilio, tran_rta->track_2, sizeof( clientes.domicilio ) );
                    strncpy( clientes.telefono, tran_rta->numero_de_comercio,
                             sizeof( clientes.telefono ) );
                    if( config.pais == ECUADOR ) {
                        clientes.cuit[10] = 0;
                    }
                    /*#ifdef INVEL_L
                    BEGIN_TRANSACTION();
                    #endif*/
                    INSERT2(SI);
                    /*#ifdef INVEL_L
                    END_TRANSACTION();
                    #endif*/
                }
                if( ACEPTAR_CLIENTE() ) {
                    SET_MEMORY_LONG( __ram_nro_cliente, clientes.cod_cliente );
                    CARGAR_PERFIL_CLIENTE( clientes.cod_perfil );
                }
            }
        }
        else {
            if( !rta_terminal.cod_rta ) {
                rta_terminal.cod_rta = 1;
                strcpy( rta_terminal.mensaje, ST( S_ERROR_DE_COMUNICACION ) );
            }
            MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal.mensaje );
        }
    }
}
/*Andaba?Modificar*/
/***********************************************************************/
int ACEPTAR_CLIENTE()
/***********************************************************************/
{
    int ok = 1, tecla;
    MENSAJE_SIN_SONIDO_STRING( S_CONFIRMA_LOS_DATOS_DEL_CLIENTE_ENTER_SI );
    tecla = 0;
    while( tecla != 13 && tecla != 27 ) {
        tecla = GETCH();
    }
    if( tecla != 13 ) {
        ok = 0;
    }
    BORRAR_MENSAJE();
    return ( ok );
}
/******************************************************************************/
void CARGAR_PERFIL_CLIENTE( int cod_perfil )
/******************************************************************************/
{
    //if( TOMAR_PERFIL_DESDE_CLIENTE ){
	if( (UTILIZA_PERFILES && cod_perfil ) 
		|| ( cod_perfil> 0 && config_tps.Perfil_cliente_especial == cod_perfil ))  {
			if(config_tps.ClienteTarjetaEspecial && config_tps.Perfil_cliente_especial == cod_perfil 
				&& lectura_banda == 1){
					SET_MEMORY_INT( __ram_perfil, cod_perfil );
					ACTUALIZA_NOM_PERFIL();
					MUESTRA_PERFIL();
			}
	} else {	
		SET_MEMORY_INT( __ram_perfil, cod_perfil );
		ACTUALIZA_NOM_PERFIL();
		MUESTRA_PERFIL();
	}
}

/******************************************************************************/
void CEREAR_PERFIL_CLIENTE( void )
/******************************************************************************/
{
    //if( TOMAR_PERFIL_DESDE_CLIENTE )
    if( UTILIZA_PERFILES ) {
        if( RAM_NRO_CLIENTE > 0 ) {
            //int area = SELECTED();
            int tabla_ant= SELECTED2();
            int tipo_ant = SELECTED_TIPO();
            EXISTE_CLIENTE( RAM_NRO_CLIENTE );
            if( clientes.cod_perfil ) {
                SET_MEMORY_INT( __ram_perfil, 0 );
            }
            //SELECT( area );
			SELECT_TABLE( tabla_ant, tipo_ant );
		} else {
			SET_MEMORY_INT( __ram_perfil, 0 );
		}
	} else{
		if(config_tps.ClienteTarjetaEspecial )
			SET_MEMORY_INT( __ram_perfil, 0 ); //aqui cereamos el perfil igual
	}
}
/******************************************************************************/
int ALTA_CLIENTE( void )
/******************************************************************************/
{
    struct _nro_cli_tmp nro_cli;
    int rta = 0, aux = 0, autorizado = 1;
    int tabla_ant= SELECTED2();
    int tipo_ant = SELECTED_TIPO();
	char where[100];
	aux = TECLADO_ALFABETICO;

	if( !SOLICITAR_CLAVES( 30, 20, NO, SI, 
		PEDIR_SUPERVISOR_ALTA_CLIENTE, "AUTORIZACION ALTA CLIENTE",NULL, 0, NO ) ) {
		autorizado = 0;
	}

	if( autorizado ) {
	if( !RAM_NRO_CLIENTE && PEDIR_DATOS_CLIENTE() ) {
		if( OBTENER_TABLA_SERVIDOR( T_NRO_CLI, TT_ORIG, "Ultimo_cliente", ""/*NULL*/, SI, NULL ) ) {
        	rta = 0;
        } else {
			if( ConfIni.odbcServer[0] == 0 ) { //no tiene conexion al servidor
				long cod_cliente = 0;
				long cant_reg = 0;
				DIRECT_DB_QUERY("SELECT MAX(cod_cliente) from clientes" ); 
				GET_DATO( 1,C_INT,(char *)&cant_reg,sizeof(clientes.cod_cliente));
				clientes.cod_cliente = cant_reg +1;
				nro_cli.cod_cliente = clientes.cod_cliente;
				rta = 1;
			} else {
				if( OPEN_TABLE(T_NRO_CLI, TT_ORIG,( char* )&nro_cli,
							sizeof( struct _nro_cli_tmp))== 0 ) {
					SET_WHERE("");
					RUN_QUERY(NO);
					clientes.cod_cliente = nro_cli.cod_cliente;
					//clientes.cheques = 'N';
					//clientes.ctacte = 'N';
					//clientes.cond_iva = 5;
					rta = 1;
				} else {
					rta = 0;
				}
				CLOSE_TABLE( T_NRO_CLI, TT_ORIG );
				SELECT_TABLE( tabla_ant, tipo_ant );
			}
        }
        /*if( rta && !EMPEZAR_VTRANSACCION() ) {
            ELIMINAR_VTRANSACCION();
            if( !EMPEZAR_VTRANSACCION() ) {
                rta = 0;
            }
        }*/
		if( nro_cli.cod_cliente < 0 ) {
			rta = 0;
		}
		if( rta ) {
			_snprintf( where, sizeof(where)-1, "COD_CLIENTE = %ld", nro_cli.cod_cliente );
			rta = INSERT_SCRIPT_POS( SPOS_INSERT_CLIENTE, where );
			if( rta < 0 ) {
				rta = 0;
			} else {
				rta = 1;
			}
		}
		if( rta ) {
			//sacar dbrouter: FORRAY AGREGAR NOTIFICACION DE NUEVO CLIENTE AL SERVIDOR........
			tabla_ant = SELECTED2();
			tipo_ant = SELECTED_TIPO();
			SELECT_TABLE( T_CLIENTES, TT_ORIG );
			//Se habilita al cliente por defecto para que cuando realize la
			//actualización de la tabla, tambien actualize las nuevas altas
			clientes.habilitado = 'S';
			INSERT2(SI);
			SELECT_TABLE( tabla_ant, tipo_ant );
        //}
		//if( rta ) {
            MENSAJE_STRING( S_CLIENTE_REGISTRADO_EXITOSAMENTE );
            SET_MEMORY_LONG( __ram_nro_cliente, clientes.cod_cliente );
            CARGAR_PERFIL_CLIENTE( ( int )clientes.cod_perfil );
            MUESTRA_CLIENTE( 0 );
        }
        else {
            MENSAJE_STRING( S_EROR_AL_REGISTRAR_CLIENTE );
        }
    }
    else if( RAM_NRO_CLIENTE ) {
        MENSAJE_STRING( S_YA_EXISTE_UN_CLIENTE_CARGADO );
    }
	}
	TECLADO_ALFABETICO = aux;

    return( rta );
}
/***********************************************************************/
int EXISTE_CLIENTE_DNI( long cliente )
/***********************************************************************/
{
    int rta/*,indice*/;
    //SELECT( AREA_CLIENTES );
    //indice = GET_ORDER();
	SELECT_TABLE( T_CLIENTES, TT_ORIG );
    //SET_ORDER( 6 );
	SET_ORDER2("NRO_DOCUMENTO");
	SET_WHERE("NRO_DOCUMENTO = %ld",cliente);
    //GET_EQUAL( ( char* )&cliente );
	RUN_QUERY(SI);
    rta = FOUND2();
    //SET_ORDER( indice );
    return ( rta );
}
/******************************************************************************/
void MOSTRAR_DATOS_BOX_CLIENTE( void )
/******************************************************************************/
{
    int  arriba = 14, izquierda = 8, derecha = 70, CANT_COLUMNAS_TABLA_CLIENTE = 52;
    char car ,where[40]/*,patron_con_null[52]*/; //cantidad de columnas de la tabla clientes
    int tecla = 0;
    #if defined(INVEL_W) || defined(INVEL_L)
    int cant = 0;
    char aux[50],aux2[50],aux3[50],aux4[50],aux5[50],aux6[50],aux7[50],aux8[50],aux9[50];
    char *cad = NULL;
    char auxi[50], grupo_aux[21], grupo_aux2[31];
    #else
    char auxi[50];
    #endif
    if( MOSTRAR_DATOS_CLIENTE_BOX ) {
        /*-------- caja cerrada -------------*/
        if( config.pais == ELSALVADOR ) {
            _snprintf( aux, 50, "NOMBRE : %s", clientes.nombre );
            aux[49] = '\0';
            _snprintf( aux2, 50, "DUI : %ld", clientes.nro_documento );
            aux2[49] = '\0';
            _snprintf( aux3, 50, "NIT : %s", clientes.cuit );
            aux3[49] = '\0';
            _snprintf( aux4, 50, "NRI: %s", clientes.nri );
            aux4[49] = '\0';
            memset( grupo_aux, 0, sizeof( grupo_aux ) );
		    if( OBTENER_DESCRIPCION_GRUPO( grupo_aux, clientes.cod_grupo_cliente, sizeof( grupo_aux ) ) ) {
		        _snprintf( auxi, 50, "TIPO DE CLIENTE : %s", grupo_aux );
                auxi[49] = '\0';
		    } else {
                _snprintf( auxi, 50, "TIPO DE CLIENTE : %d", clientes.cod_grupo_cliente );
                auxi[49] = '\0';
		    }
            _snprintf( aux5, 50, "%s", auxi );
            aux5[49] = '\0';
            memset( grupo_aux2, 0, sizeof( grupo_aux2 ) );
		    if( OBTENER_DESCRIPCION_COND_IVA( grupo_aux2, clientes.cond_iva, sizeof( grupo_aux2 ) ) ) {
                _snprintf( auxi, 50, "CONDICION DE IVA : %s", grupo_aux2 );
                auxi[49] = '\0';
            } else {
                _snprintf( auxi, 50, "CONDICION DE IVA : %d", clientes.cond_iva );
                auxi[49] = '\0';
            }
            _snprintf( aux6, 50, "%s", auxi );
            aux6[49] = '\0';
            _snprintf( aux7, 50, "GIRO : %s", clientes.giro );
            aux7[49] = '\0';

            cant = strlen( aux ) + strlen( aux2 ) + strlen( aux3 ) + strlen( aux4 )
                 + strlen( aux5 ) + strlen( aux6 ) + strlen( aux7 );

            if( ( cad = ( char* )malloc( cant + 8 ) ) != NULL ) {
                memset( cad, 0, cant + 8 );
                _snprintf( cad, cant + 8, "&%s&%s&%s&%s&%s&%s&%s",
                        aux, aux2, aux3, aux4, aux5, aux6, aux7 );
                cad[ cant + 7] = '\0';
                ExecuteSSAction( A_PEDIR, _HIBRIDO, " DATOS DE CLIENTE ", cad, NULL, " " );
            }
        }
        else {
            _snprintf( aux, 50, "NUMERO : %ld", clientes.cod_cliente );
            aux[49] = '\0';
            _snprintf( aux2, 50, "NOMBRE : %s", clientes.nombre );
            aux2[49] = '\0';
            //memset( grupo_aux, 0, sizeof( grupo_aux ) );
            //OBTENER_DESCRIPCION_GRUPO( grupo_aux, ( char )clientes.cod_grupo_cliente,
            //                           ( int )sizeof( grupo_aux ) );
            //sprintf( aux3, "GRUPO : %s", grupo_aux );
            //sprintf( aux4, "COD. GENERICO : %s", clientes.valor_grupo_cliente );
            _snprintf( aux5, 50, "TELEFONO : %s", clientes.telefono );
            aux5[49] = '\0';
            _snprintf( aux6, 50, "DOMICILIO : %s", clientes.domicilio );
            aux6[49] = '\0';
            _snprintf( aux7, 50, "IEPS : %s", clientes.nri );
            aux7[49] = '\0';
            _snprintf( aux8, 50, "RFC : %s", clientes.cuit );
            aux8[49] = '\0';
            _snprintf( aux9, 50, "NRO DOCUMENTO : %ld", clientes.nro_documento );
            aux9[49] = '\0';

            cant = strlen( aux ) + strlen( aux2 ) /*+ strlen( aux3 ) + strlen( aux4 )*/ + strlen( aux5 )
                 + strlen( aux6 ) + strlen( aux7 ) + strlen( aux8 ) + strlen( aux9 );

            if( ( cad = ( char* )malloc( cant + 8 ) ) != NULL ) {
                memset( cad, 0, cant + 8 );
                _snprintf( cad, cant + 8, "&%s&%s&%s&%s&%s&%s&%s",
                        aux, aux2, aux5, aux6, aux9, aux8, aux7 );
                cad[ cant + 7] = '\0';
                ExecuteSSAction( A_PEDIR, _HIBRIDO, " DATOS DEL CLIENTE ", cad, NULL, " " );
            }
        }
        if( cad ) {
            free( cad );
        }
        //Se modifica el msj porque el ESC esta condicionado en otras funciones q usan esta
        if( EDITAR_DATOS_CLIENTE_AFINIDAD ){
            MENSAJE_SIN_SONIDO( ST( S_DESEA_EDITAR_LOS_DATOS_S_N ) ,NO );
            TECLADO_ALFABETICO = 1;
            do {
                car = GETCH();
            }
            while( car != 'S' && car != 'N' && car != 's' && car != 'n' );
            #if defined(INVEL_W) || defined(INVEL_L)
            ExecuteAction( A_FIN_PEDIR );
            #endif
            BORRAR_MENSAJE();
            TECLADO_ALFABETICO = 0;
            if( car == 'S' || car == 's' && SOLICITAR_CLAVES( 30, 20, NO, SI, PEDIR_SUPERVISOR_ALTA_CLIENTE, "AUTORIZACION EDITAR CLIENTE",NULL, 0, NO )) {
                if( EDITAR_DATOS_CLIENTE() ) {
                    _snprintf( where, 40, "cod_cliente = %ld", clientes.cod_cliente );
                    where[39] = '\0';
                    //Se setean las columnas a las que se les quiere aplicar el UPDATE
                    /*memset( patron_con_null, 0, CANT_COLUMNAS_TABLA_CLIENTE );
                    patron_con_null[1] = 1;   //nombre
                    patron_con_null[2] = 1;   //domicilio
                    patron_con_null[13] = 1;  //giro
                    patron_con_null[33] = 1;  //nro_documento
                    patron_con_null[44] = 1;  //cuit
                    patron_con_null[45] = 1;  //nri
                    if( DBROUTER_REQ_CON_NULL( ACTUALIZAR_REGISTRO, CANT_COLUMNAS_TABLA_CLIENTE,
                                               patron_con_null, "clientes", ( char* )&clientes,
                                               sizeof( struct _clientes )
                                             , where, 0 ) ) {
						int tabla_ant= SELECTED2();
                        int tipo_ant = SELECTED_TIPO();
						SELECT_TABLE( T_CLIENTES, TT_ORIG );
                        UPDATE2();
						SELECT_TABLE( tabla_ant, tipo_ant );
                        MENSAJE_STRING( S_CLIENTE_REGISTRADO_EXITOSAMENTE );
                    }*/
					if( INSERT_SCRIPT_POS( SPOS_UPDATE_CLIENTE, where ) > -1 ) {
						int tabla_ant= SELECTED2();
                  int tipo_ant = SELECTED_TIPO();
						SELECT_TABLE( T_CLIENTES, TT_ORIG );
                  UPDATE2();
						SELECT_TABLE( tabla_ant, tipo_ant );
                        MENSAJE_STRING( S_CLIENTE_REGISTRADO_EXITOSAMENTE );
					}
                }
            }
        }
        else {
            MENSAJE_SIN_SONIDO_STRING( S_PRESIONE_ENTER_PARA_CONTINUAR );
            while( GETCH() != 13 );
			BORRAR_MENSAJE();
            #if defined(INVEL_W) || defined(INVEL_L)
            ExecuteAction( A_FIN_PEDIR );
            #endif
        }
    }
}
/*****************************************************************************/
int EDITAR_DATOS_CLIENTE()
/*****************************************************************************/
{
    int rta = 0, cant_datos, x = 0, y = 0, ok = 0, dato=0,digitos_ing=0;
    char car,a[41],nombre_cliente[31],auxi[40];
    char domicilio_cliente[41],nri_cliente[15],cuit_cliente[21];
    char nro_doc_cliente[15],giro_cliente[31];
    #if defined(INVEL_W) || defined(INVEL_L)
    int cant = 0;
    char aux[100],aux2[100],aux3[100],aux4[100],aux5[100],aux6[100];
    char *cad;
    #endif
    cant_datos = 7;
    sprintf( aux, "%s : %s", ST( S_NOMBRE_CLIENTE ), clientes.nombre );
    sprintf( aux2, "%s : %s", ST( S_DOMICILIO_CLIENTE ), clientes.domicilio );
    sprintf( aux3, "%s : %s", ST( S_NRI_CLIENTE ), clientes.nri );
    sprintf( aux4, "%s : %s", ST( S_NIT_CLIENTE ), clientes.cuit );
    sprintf( aux5, "%s : %ld", ST( S_DUI_CLIENTE ), clientes.nro_documento );
    sprintf( aux6, "%s : %s", ST( S_GIRO ), clientes.giro );
    cant = strlen( aux ) + strlen( aux2 ) + strlen( aux3 ) + strlen( aux4 ) + strlen( aux5 )
         + strlen( aux6 ) + strlen( ST( S_NOMBRE_CLIENTE ) ) + strlen( ST( S_DOMICILIO_CLIENTE ) )
         + strlen( ST( S_NRI_CLIENTE ) ) + strlen( ST( S_NIT_CLIENTE ) )
         + strlen( ST( S_DUI_CLIENTE ) ) + strlen( ST( S_GIRO ) )
         + strlen( "& &-------------------DATOS DEL CLIENTE-------------------" );
    if( ( cad = ( char* )malloc( cant + 13 ) ) != NULL ) {
        memset( cad, 0, cant + 13 );
        strcat( cad, "~" );
        strncat( cad, ST( S_NOMBRE_CLIENTE ), strlen( ST( S_NOMBRE_CLIENTE ) ) );
        strcat( cad, "~" );
        strncat( cad, ST( S_DOMICILIO_CLIENTE ), strlen( ST( S_DOMICILIO_CLIENTE ) ) );
        strcat( cad, "~" );
        strncat( cad, ST( S_NRI_CLIENTE ), strlen( ST( S_NRI_CLIENTE ) ) );
        strcat( cad, "~" );
        strncat( cad, ST( S_NIT_CLIENTE ), strlen( ST( S_NIT_CLIENTE ) ) );
        strcat( cad, "~" );
        strncat( cad, ST( S_DUI_CLIENTE ), strlen( ST( S_DUI_CLIENTE ) ) );
        strcat( cad, "~" );
        strncat( cad, ST( S_GIRO ), strlen( ST( S_GIRO ) ) );
        strcat( cad, "& &-------------------DATOS DEL CLIENTE-------------------" );
        strcat( cad, "&" );
        strncat( cad, aux, strlen( aux ) );
        strcat( cad, "&" );
        strncat( cad, aux2, strlen( aux2 ) );
        strcat( cad, "&" );
        strncat( cad, aux3, strlen( aux3 ) );
        strcat( cad, "&" );
        strncat( cad, aux4, strlen( aux4 ) );
        strcat( cad, "&" );
        strncat( cad, aux5, strlen( aux5 ) );
        strcat( cad, "&" );
        strncat( cad, aux6, strlen( aux6 ) );
        ExecuteSSAction( A_PEDIR, _HIBRIDO, " ACTUALIZACION DE CLIENTES ", cad, NULL, " " );
        ExecuteSSAction( A_SET_PROP_INPUT, _INPUT_ALTA_CLIENTES, "_HIBRIDO", "", NULL, " " );

        ExecuteSSAction( A_PEDIR_DATO, 0, " ", " ", NULL, " " );
        strncpy(nombre_cliente, clientes.nombre, sizeof(nombre_cliente));
		TRIM( nombre_cliente );
        MOSTRAR_INPUT_CADENA( nombre_cliente, 0 );
        ExecuteSSAction( A_PEDIR_DATO, 1, " ", " ", NULL, " " );
        strncpy(domicilio_cliente, clientes.domicilio, sizeof(domicilio_cliente));
		TRIM( domicilio_cliente );
        MOSTRAR_INPUT_CADENA( domicilio_cliente, 0 );
        ExecuteSSAction( A_PEDIR_DATO, 2, " ", " ", NULL, " " );
        strncpy(nri_cliente, clientes.nri, sizeof(nri_cliente));
		TRIM( nri_cliente );
        MOSTRAR_INPUT_CADENA( nri_cliente, 0 );
        ExecuteSSAction( A_PEDIR_DATO, 3, " ", " ", NULL, " " );
        strncpy(cuit_cliente, clientes.cuit, sizeof(cuit_cliente));
		TRIM( cuit_cliente );
        MOSTRAR_INPUT_CADENA( cuit_cliente, 0 );
        ExecuteSSAction( A_PEDIR_DATO, 4, " ", " ", NULL, " " );
        sprintf( nro_doc_cliente, "%ld",clientes.nro_documento );
		TRIM( nro_doc_cliente );
        MOSTRAR_INPUT_CADENA( nro_doc_cliente, 0 );
        ExecuteSSAction( A_PEDIR_DATO, 5, " ", " ", NULL, " " );
        strncpy(giro_cliente, clientes.giro, sizeof(giro_cliente));
		TRIM( giro_cliente );
        MOSTRAR_INPUT_CADENA( giro_cliente, 0 );
    }
    while(dato < 6){
    TECLADO_ALFABETICO = 1;
        ExecuteSSAction( A_PEDIR_DATO, dato, " ", " ", NULL, " " );
        switch(dato){
        case 0:
            rta = PEDIR_CADENA( ( char )( x + 23 ), ( char )( y + 1 ), ( char )30, nombre_cliente, NULL, NO, SI );
            rta = SALIR_EDICION_CLIENTE();
            if( !rta || nombre_cliente[0] == ' ' ) {
                sprintf( auxi, "                                " );
                MOSTRAR_CADENA( x + 23, y + 1, auxi );
                sprintf( a, "%s", clientes.nombre );
                MOSTRAR_CADENA( x + 23, y + 1, a );
                strcpy( nombre_cliente, clientes.nombre );
                MOSTRAR_INPUT_CADENA( nombre_cliente, 0 );
            }
            digitos_ing = strlen(nombre_cliente);
            break;
        case 1:
            rta = PEDIR_CADENA( ( char )( x + 23 ), ( char )( y + 2 ), ( char )40, domicilio_cliente,
                                NULL, NO, SI );
            rta = SALIR_EDICION_CLIENTE();
            if( !rta || domicilio_cliente[0] == ' ' ) {
                sprintf( auxi, "                                " );
                MOSTRAR_CADENA( x + 23, y + 2, auxi );
                sprintf( a, "%s", clientes.domicilio );
                MOSTRAR_CADENA( x + 23, y + 2, a );
                strcpy( domicilio_cliente, clientes.domicilio );
                MOSTRAR_INPUT_CADENA( domicilio_cliente, 0 );
            }
            digitos_ing = strlen(domicilio_cliente);
            break;
        case 2:
            TECLADO_ALFABETICO = 0;
            rta = PEDIR_CADENA_NUMERICA( ( char )( x + 23 ), ( char )( y + 3 ), ( char )10,
                                         nri_cliente, NULL, 0, NO, SI,NULL );
            rta = SALIR_EDICION_CLIENTE();
            if( !rta || nri_cliente[0] == ' ' ) {
                sprintf( auxi, "                                " );
                MOSTRAR_CADENA( x + 23, y + 3, auxi );
                sprintf( a, "%s", clientes.nri );
                MOSTRAR_CADENA( x + 23, y + 3, a );
                strcpy( nri_cliente, clientes.nri );
                MOSTRAR_INPUT_CADENA( nri_cliente, 0 );
            }
            digitos_ing = strlen(nri_cliente);
            break;
        case 3:
                TECLADO_ALFABETICO = 0;
                rta = PEDIR_CADENA_NUMERICA( ( char )( x + 23 ), ( char )( y + 4 ), ( char )14,
                                             cuit_cliente, NULL, 0, NO, SI,NULL );
                rta = SALIR_EDICION_CLIENTE();
                if( !rta || cuit_cliente[0] == ' ' ) {
                    sprintf( auxi, "                                " );
                    MOSTRAR_CADENA( x + 23, y + 4, auxi );
                    sprintf( a, "%s", clientes.cuit );
                    MOSTRAR_CADENA( x + 23, y + 4, a );
                    strcpy( cuit_cliente, clientes.cuit );
                    MOSTRAR_INPUT_CADENA( cuit_cliente, 0 );
                }
                digitos_ing = strlen(cuit_cliente);
            break;
        case 4:
                TECLADO_ALFABETICO = 0;
                rta = PEDIR_CADENA_NUMERICA( ( char )( x + 23 ), ( char )( y + 5 ), ( char )9,
                                                nro_doc_cliente, NULL, 0, NO, SI,NULL );
                rta = SALIR_EDICION_CLIENTE();
                if( !rta || nro_doc_cliente[0] == ' ' ) {
                    sprintf( auxi, "                                " );
                    MOSTRAR_CADENA( x + 23, y + 5, auxi );
                    sprintf( a, "%ld", clientes.nro_documento );
                    MOSTRAR_CADENA( x + 23, y + 5, a );
                    strcpy( nro_doc_cliente, a );
                    MOSTRAR_INPUT_CADENA( nro_doc_cliente, 0 );
                }
                digitos_ing = strlen(nro_doc_cliente);
            break;
        case 5:
                TECLADO_ALFABETICO = 1;
                rta = PEDIR_CADENA( ( char )( x + 23 ), ( char )( y + 6 ), ( char )30,
                                    giro_cliente, NULL, NO, SI );//giro
                if(-80 == rta)
                    dato--;
                rta = SALIR_EDICION_CLIENTE();
                if( !rta || giro_cliente[0] == ' ' ) {
                    sprintf( auxi, "                                " );
                    MOSTRAR_CADENA( x + 23, y + 6, auxi );
                    sprintf( a, "%s", clientes.giro );
                    MOSTRAR_CADENA( x + 23, y + 6, a );
                    strcpy( giro_cliente, clientes.giro );
                    MOSTRAR_INPUT_CADENA( giro_cliente, 0 );
                }
                digitos_ing = strlen(giro_cliente);
            break;

        }
        if(-72 == rta ){ //si es flecha arriba
            if(dato > 0)
                dato--;
        }else if( !digitos_ing  && rta ) {
            MENSAJE( ST( S_INGRESE_DATO_OBLIGATORIO ) );
            BORRAR_MENSAJE();
        }else if(!rta){
            dato = 7; //fin
        }else{
            if(27 != rta)
                dato++;
        }
    }
    if(dato == 6)
        ok = 1;

    if( ok ) {
        MENSAJE_SIN_SONIDO( "CONFIRMA LA EDICION DE LOS DATOS S/N",NO );
        TECLADO_ALFABETICO = 1;
        do {
            car = GETCH();
        }
        while( car != 'S' && car != 'N' && car != 's' && car != 'n' );
        TECLADO_ALFABETICO = 0;
        if( car == 'S' || car == 's' ) {
            strncpy( clientes.nombre, nombre_cliente, sizeof(clientes.nombre) );
            strncpy( clientes.giro, giro_cliente, sizeof(clientes.giro) );
            strncpy( clientes.cuit, cuit_cliente, sizeof(clientes.cuit) );
            strncpy( clientes.nri, nri_cliente, sizeof(clientes.nri) );
            strncpy( clientes.domicilio, domicilio_cliente, sizeof( clientes.domicilio ) );
            if( nro_doc_cliente[0] != ' ' ) {
                clientes.nro_documento = atol( nro_doc_cliente );
            }
            rta = 1;
        }
        else {
            rta = 0;
        }
    }
    #if defined(INVEL_W) || defined(INVEL_L)
    ExecuteSSAction( A_SET_PROP_INPUT, _INPUT_GENERICO, "_SIMPLE", "", NULL, " " );
    ExecuteAction( A_FIN_PEDIR );
    #endif
    BORRAR_MENSAJE();
    return( rta );
}
/*****************************************************************************/
int _consulta_grupos_cli( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0, y;
    y = _Y1 + dato;
    switch( comando ) {
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[28];
                strncpy( xx, grupocli->descripcion, 26 );
                xx[27] = 0;
                setFieldValuepChar( b->index, 0, xx );
            }
            #endif
            MOSTRAR_CADENA( _X1, y, grupocli->descripcion );
            break;
        case _SKIP:
            //SKIP( dato );
            //rta = BtrvEOF() || BOF();
			SKIP2( dato );
            rta = dbBOF() || dbEOF();
            break;
        case _EXTREMO:
            //rta = ( dato == -1 ) ? BOF() : BtrvEOF();
			 rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
            rta = 1;
            break;
        case _COMPARAR:
            rta = 2;
            break;
    }
    return ( rta );
}
/***********************************************************************/
int BUSCAR_CLIENTE_POR_COD_GENERICO( char *cadena, char cod_grupo )
/***********************************************************************/
{
    int rta = 0;
    char cad[21];
    struct
    {
        char cod_grupo;
        char valor_grupo[21];
    } ind6_cli;
    //  memset( cad, '0', sizeof( cad ) );
    //  cad[20]=0;
    //   TRIM( cadena );
    memset( cad, 0, sizeof( cad ) );
    //   memcpy( &cad[strlen( cad ) - strlen( cadena )], cadena, strlen( cadena ) );
    memcpy( &cad, cadena, strlen( cadena ) );
    TRIM( cad );
    ind6_cli.cod_grupo = cod_grupo;
    strcpy( ind6_cli.valor_grupo, cad );
    //SELECT( AREA_CLIENTES );
	SELECT_TABLE( T_CLIENTES, TT_ORIG );
    GO2( TOP );
    //SET_ORDER( 7 );
    //GET_EQUAL( ( char* )&ind6_cli );
	SET_ORDER2("COD_GRUPO_CLIENTE, VALOR_GRUPO_CLIENTE");
	SET_WHERE("COD_GRUPO_CLIENTE = %u AND VALOR_GRUPO_CLIENTE = '%s'",cod_grupo,cad);
	RUN_QUERY(NO);
    if( FOUND2() ) {
        rta = 1;
    }
    else {
        SET_MEMORY_LONG( __ram_nro_cliente, 0 );
        CEREAR_PERFIL_CLIENTE();
    }
    return ( rta );
}
/*****************************************************************************/
int OBTENER_DESCRIPCION_GRUPO( char *grupo_aux, char cod_grupo, int largo )
/*****************************************************************************/
{
    int ok = 0, area_ant, tipo_ant;
    struct _grupos_clientes grupocli;
    area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
  //  area_auxi = AREA_DISPONIBLE();
    //if( USE_DB( area_auxi, _GRUPOCLI, ( char* )&grupocli, sizeof( struct _grupos_clientes ), NULL, 0, 0 )
    // == 0 ) {
	if( OPEN_TABLE( T_GRUPO_CLI, TT_ORIG, ( char* )&grupocli, sizeof( struct _grupos_clientes ) ) == 0 ){
        //SET_ORDER( 1 );
		SELECT_TABLE( T_GRUPO_CLI, TT_ORIG );
		//GET_EQUAL( ( char* )&cod_grupo );
		SET_WHERE("COD_GRUPO_CLIENTE = '%d'", cod_grupo );
		RUN_QUERY(NO);
        //if( FOUND() ) {
		if( FOUND2() ) {
            memcpy( grupo_aux, grupocli.descripcion, largo );
            ok = 1;
        }
       // CLOSE_DATABASE( area_auxi );
		CLOSE_TABLE( T_GRUPO_CLI, TT_ORIG );
    }
   // SELECT( area_ant );
	 SELECT_TABLE( area_ant, tipo_ant );
    return ( ok );
}

/*****************************************************************************/
int OBTENER_DESCRIPCION_COND_IVA( char *c_iva, char con_iva, int largo )
/*****************************************************************************/
{
	int       ok = 0/*, area_ant*/, tabla_ant, tipo_ant;

    tabla_ant = SELECTED2();
    tipo_ant = SELECTED_TIPO();

	if( OPEN_TABLE( T_COND_IVA, TT_ORIG, ( char* )&cond_iva, sizeof( cond_iva ) ) == 0 ) {

		SET_WHERE("COD_COND_IVA='%d'", con_iva);
		RUN_QUERY(NO);
		if( FOUND2(  ) ) {
			memcpy( c_iva, cond_iva.desc_cond_iva, largo );
			ok = 1;
		}

		CLOSE_TABLE( T_COND_IVA, TT_ORIG );
	}

	SELECT_TABLE( tabla_ant, tipo_ant );
	return ( ok );
}

/*****************************************************************************/
int OBTENER_COND_IVA_CLIENTE( long cod_cliente )
/*****************************************************************************/
{
	int indice = 0, cond_IVA = 0;
	int tabla_ant = 0, tipo_ant = 0, OpenTable = 0;
	long cliente = 0;

	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	
	if( cod_cliente ) {
		cliente = cod_cliente;
	} else {
		cliente = RAM_NRO_CLIENTE;
	}

	if( !OPENED( T_CLIENTES, TT_ORIG ) ) {
		//Se agrega la bandera para verificar que si la tabla esta cerrada, luego se cierre o viceversa
		//de manera que si hay otra funcionalidad que necesita la tabla abierta y solo realiza las consultas
		//sobre la misma no se caiga la caja porque fue cerrada.
		OpenTable = 1;
		OPEN_TABLE( T_CLIENTES, TT_ORIG, ( char* )&clientes, sizeof( struct _clientes ) );
	}
	SELECT_TABLE( T_CLIENTES, TT_ORIG );
	GO2( TOP );
	SET_WHERE("COD_CLIENTE='%d'", cliente);
	RUN_QUERY(NO);

	if( FOUND2() ) {
		cond_IVA = clientes.cond_iva;
	}
	//Nota del mergeador: aca arriba deberia ir AREA_CLIENTES
	if( OpenTable ) {
		CLOSE_TABLE( T_CLIENTES, TT_ORIG );
	}
	SELECT_TABLE( tabla_ant, tipo_ant );

	return ( cond_IVA );
}

/***************************************************************************/
int ASIGNA_CONDICION_IVA( char *cadena )
/***************************************************************************/
{
    int rta = 0;
    //struct _condiciones_iva cond_iva;
	int  area_ant, tipo_ant;

	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

        /*---------------------------- Abre el archivo ---------------------------*/
        _cond_iva = &cond_iva;
       // if( USE_DB( AREA_CONDICIONES_IVA, _COND_IVA_SIC, ( char* )&cond_iva, sizeof( cond_iva ),
       //             NULL, 0, 0 ) == 0 ) {
		if( OPEN_TABLE( T_COND_IVA, TT_ORIG, ( char* )&cond_iva, sizeof( cond_iva ) ) == 0 ) {
            /*-------------------------- Prepara la pantalla -------------------------*/
			SELECT_TABLE( T_COND_IVA, TT_ORIG );
			RUN_QUERY(NO);
			GO2( BOTTOM );
            while( _cond_iva->habilitado == 'N' ) {
				SKIP2( -1 );
            }
			reg_pos_inf = GET_POSITION_REG();
			GO2( TOP );
            while( _cond_iva->habilitado == 'N' ) {
				SKIP2( 1 );
            }
			reg_pos = GET_POSITION_REG();

            rta = Consulta_condicion_iva( _elegir_cond_iva );

			if( rta ) {
                clientes.cond_iva = cond_iva.cod_cond_iva;
                strcpy( cadena, cond_iva.desc_cond_iva );
            }
            else {
                //strcpy( cadena, " " );
            }
			CLOSE_TABLE( T_COND_IVA, TT_ORIG );
    }

	SELECT_TABLE( area_ant, tipo_ant );

	return ( rta );
}
/*****************************************************************************/
int _elegir_cond_iva( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0;
    static int indice = 0;
    static int cerear = 1;
    //int tam = sizeof( TIPOS_IVA_MOSTRAR ) / sizeof( *TIPOS_IVA_MOSTRAR );
    switch( comando ) {
        case _BUSCAR:
            break;
        case _MOSTRAR:
            /*if( config.pais == ELSALVADOR ) {
                #if defined(INVEL_W) || defined(INVEL_L)
                setFieldValuepChar( b->index, 0, TIPOS_IVA_MOSTRAR[indice] );
                #endif
                MOSTRAR_CADENA( 8, 10 + dato, TIPOS_IVA_MOSTRAR[indice] );
            }
            else {*/
                if( _cond_iva->habilitado == 'S' ) {
                    #if defined(INVEL_W) || defined(INVEL_L)
                    setFieldValuepChar( b->index, 0, _cond_iva->desc_cond_iva );
                    #endif
                    MOSTRAR_CADENA( 8, 10 + dato, _cond_iva->desc_cond_iva );
                }
            //}
            break;
        case _SKIP:
            /*if( config.pais == ELSALVADOR ) {
                if( ( dato > 0 && ( indice < tam ) ) || ( dato < 0 && indice >= 0 ) ) {
                    if( ( indice + dato ) < 0 ) {
                        indice = 0;
                        rta = 1;
                    }
                    else if( ( indice + dato ) > tam - 1 ) {
                        indice = tam - 1;
                        rta = 1;
                    }
                    else {
                        indice += dato;
                        rta = 0;
                    }
                }
            }
            else {*/
                if( !( ( reg_pos >= /*GET_POSITION()*/GET_POSITION_REG() ) && dato < 0 )
                 && !( ( reg_pos_inf <= /*GET_POSITION()*/GET_POSITION_REG() ) && dato > 0 ) ) {
                    do {
                        //SKIP( dato );
						SKIP2( dato );
                        //rta = BtrvEOF() || BOF();
						rta = dbEOF() || dbBOF();
                    }
                    while( !rta && _cond_iva->habilitado == 'N' );
                }
                else {
                    rta = 1;
                }
            //}
            break;
        case _VALIDAR:
            if( config.pais == ELSALVADOR ) {
            }
            else {
                if( dato >= '0' && dato <= '9' ) {
                    rta = 1;
                }
            }
            break;
        case _EXTREMO:
            /*if( config.pais == ELSALVADOR ) {
                rta = ( dato == -1 ) ? indice == 0 : indice == tam - 1;
            }
            else {*/
                rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            //}
            break;
        case _ENTER:
            /*if( config.pais == ELSALVADOR ) {
                rta = indice + 1;
                indice = 0;
                cerear = 1;
            }
            else {*/
                rta = 1;
            //}
            break;
        case _TECLA:
            if( cerear ) {
                indice = 0;
                cerear = 0;
            }
            if( dato == 27 ) {
                indice = 0;
                cerear = 1;
            }
            else if( dato == -73 ) {
                cerear = 1;
            }
            break;
    }
    return ( rta );
}
/***********************************************************************/
int EXISTE_CLIENTE_POR_CODIGO( char *codigo )
/***********************************************************************/
{
    int rta = 0;
    long cod = 0;
    //SELECT( AREA_CLIENTES );
	SELECT_TABLE( T_CLIENTES, TT_ORIG );
    GO2( TOP );
    if( config.pais == ELSALVADOR ) {
        codigo[15] = 0;
        TRIM( codigo );
    //SET_ORDER( 8 );
		SET_ORDER2("NRI");
    //GET_EQUAL( codigo );
		SET_WHERE("NRI = '%s'", codigo);
		RUN_QUERY(SI);
    } else {
        cod = atol( codigo );
    //SET_ORDER( 1 );
		SET_ORDER2("COD_CLIENTE");
		SET_WHERE("COD_CLIENTE = '%ld'",cod);
		RUN_QUERY(SI);
    }
    if( FOUND2() ) {
        rta = 1;
        SET_MEMORY_LONG( __ram_nro_cliente, clientes.cod_cliente );
        CARGAR_PERFIL_CLIENTE( ( int )clientes.cod_perfil );
    }
    else {
        SET_MEMORY_LONG( __ram_nro_cliente, 0 );
        CEREAR_PERFIL_CLIENTE();
    }
    return ( rta );
}
/***********************************************************************/
int SALIR_ALTA_CLIENTE( void )
/***********************************************************************/
{
    int rta = 1;
    char car;
    if( LASTKEY() == 27 ) {
        MENSAJE( "DESEA SALIR DEL ALTA DE CLIENTE  S/N" );
        TECLADO_ALFABETICO = 1;
        do {
            car = GETCH();
        }
        while( car != 'S' && car != 'N' && car != 's' && car != 'n' );
        if( car == 'S' || car == 's' ) {
            rta = 0;
        }
        else {
            rta = 1;
        }
    }
    BORRAR_MENSAJE();
    TECLADO_ALFABETICO = 0;
    return ( rta );
}
/***********************************************************************/
int SALIR_EDICION_CLIENTE()
/***********************************************************************/
{
    int rta = 1;
    char car;
    if( (rta = LASTKEY()) == 27 ) {
        MENSAJE_SIN_SONIDO( "DESEA SALIR DE LA EDICION DE CLIENTE  S/N",NO );
        TECLADO_ALFABETICO = 1;
        do {
            car = GETCH();
        }
        while( car != 'S' && car != 'N' && car != 's' && car != 'n' );
        if( car == 'S' || car == 's' ) {
            rta = 0;
        }
        else {
            //rta = 1;
        }
    }
    BORRAR_MENSAJE();
    TECLADO_ALFABETICO = 0;
    return ( rta );
}
/********************************************************************************/
struct _obtenido_codigo_barra_cli
{
    // Datos comprobante.
    char P[3];                  // PREFIJO
    char S[4];                  // NRO SUCURSAL
    char C[8];                  //CUENTA
} obtenido_cod_barra_cli;
/*****************************************************************************/
int ANALIZAR_CODIGO_BARRA_CLIENTE( char *scanner_cod_barra,
	struct _datos_cli *datos_cli, char *mascara )
/*****************************************************************************/
{
    char *letra_mascara = mascara;   // NEW
    char *letra_scanner = scanner_cod_barra;
    int dif_dias = 0;
    int iError = 0;
    int i,seguir;
    memset( &obtenido_cod_barra_cli, 0, sizeof( obtenido_cod_barra_cli ) );
    for( i = 0, seguir = 1;scanner_cod_barra[i] && seguir;i++ ) {
        if( scanner_cod_barra[i] < '0' || scanner_cod_barra[i] > '9' ) {
            scanner_cod_barra[i] = 0;
            seguir = 0;
        }
    }
    TRIM( letra_mascara );
    if( strlen( scanner_cod_barra ) != strlen( letra_mascara ) ) {
        iError = 1;
    }
    while( *letra_mascara && !iError ) {
        if( *letra_mascara >= '0' && *letra_mascara <= '9' ) {
            if( *letra_mascara != *letra_scanner ) {
                iError = 1;
                break;
            }
        }
        else {
            switch( *letra_mascara ) {
                case 'P':
                    // PARTE ENTERA
                    if( strlen( obtenido_cod_barra_cli.P ) + 1 < sizeof( obtenido_cod_barra_cli.P) ) {
                        obtenido_cod_barra_cli.P[strlen( obtenido_cod_barra_cli.P )] = *letra_scanner;
                    }
                    break;
                case 'S':
                    // PARTE DECIMAL
                    if( strlen( obtenido_cod_barra_cli.S ) + 1 < sizeof( obtenido_cod_barra_cli.S ) ) {
                        obtenido_cod_barra_cli.S[strlen( obtenido_cod_barra_cli.S )] = *letra_scanner;
                    }
                    break;
                case 'C':
                    // NRO CUENTA
                    if( strlen( obtenido_cod_barra_cli.C ) + 1 < sizeof( obtenido_cod_barra_cli.C ) ) {
                        obtenido_cod_barra_cli.C[strlen( obtenido_cod_barra_cli.C )] = *letra_scanner;
                    }
                    break;
            }
        };
        letra_mascara++;
        letra_scanner++;
    }
    if( !iError ) {
        datos_cli->prefijo = atoi( obtenido_cod_barra_cli.P );
        datos_cli->sucursal = atoi( obtenido_cod_barra_cli.S );
        datos_cli->cuenta = atol( obtenido_cod_barra_cli.C );
    }
    return ( iError );
}
/*****************************************************************************/
long BUSCAR_CLIENTE_COD_BARRA()
/*****************************************************************************/
{
    char cadena[14];
    struct _datos_cli datos_cli;
	char perm_scanner_ant = _PERMITIR_SCANNER;
    memset(&datos_cli,0,sizeof(struct _datos_cli));

    ExecuteSSAction( A_PEDIR, 1, "INGRESE CODIGO CLIENTE", ST( S_CODIGO_BARRA_ ), NULL, " " );
    ExecuteSSAction( A_SET_PROP_INPUT, _INPUT_TARJETA, "_SIMPLE", "", NULL, " " );
    _PERMITIR_SCANNER = SI;
	if( PEDIR_GRAN_CADENA_NUMERICA( 13 - ancho_panel / 2, 22, 13, cadena ) ) {
        if(strlen(config.mascara_scanner_cli) == strlen(cadena)){
            ANALIZAR_CODIGO_BARRA_CLIENTE(cadena,&datos_cli,config.mascara_scanner_cli);
        }else{
            MENSAJE( "CODIGO DE BARRA INCOMPLETO" );
            BORRAR_MENSAJE();
        }
    }
	_PERMITIR_SCANNER = perm_scanner_ant;
    ExecuteAction( A_FIN_PEDIR );
    return( datos_cli.cuenta );
}
/*****************************************************************************/
int CLIENTE_HABILITADO_PARA_CHEQUES( void  )
/*****************************************************************************/
{

	return ((int)(( clientes.cheques == 'S'|| clientes.cheques == 's')? SI : NO ));
}

/*****************************************************************************/
int EXISTE_CLIENTE_SERVIDOR( )
/*****************************************************************************/
{
    int tabla_ant= SELECTED2();
    int tipo_ant = SELECTED_TIPO();
    int rta = 1;
//    char where[40];
    struct _nro_cli_tmp nro_cli;



	if( OBTENER_TABLA_SERVIDOR( T_NRO_CLI, TT_ORIG, "Ultimo_cliente", ""/*NULL*/, SI, NULL ) ) {
      	rta = 0;
   } else {
      	if( OPEN_TABLE(T_NRO_CLI, TT_ORIG,( char* )&nro_cli,
						sizeof( struct _nro_cli_tmp))== 0 ) {
   				SET_WHERE("");
   				RUN_QUERY(NO);
               if( nro_cli.cod_cliente <= 0) {
                  rta = 0;
               }
            }
            else {
                rta = 0;
            }
    }
	CLOSE_TABLE( T_NRO_CLI, TT_ORIG );
   SELECT_TABLE( tabla_ant, tipo_ant );
    if( rta == 0) {
      MENSAJE_CON_TECLA( ST( S_EL_SERVER_NO_SE_ENCUENTRA_ON_LINE_ESC ), 27 );
      //glog(ST( S_EL_SERVER_NO_SE_ENCUENTRA_ON_LINE_ESC ),LOG_VENTAS,2);
   }

   return rta;

}

/*************************************************************************/
int VALIDAR_DATOS_CLIENTE_FISCAL( void )
/*************************************************************************/
{
	int ok = 0;
	int Entrada[12];
	int Peso[10], Suma, Resto, DVerif, i, j;
	char cadena[80];

	//if( RAM_NRO_CLIENTE==-1 || EXISTE_CLIENTE( RAM_NRO_CLIENTE ) ) {
		Suma = 0;
		for( j = 0; clientes.cuit[j] != '\0'; j++ )
			Entrada[j] = clientes.cuit[j] - '0';
		Peso[0] = 5;
		Peso[1] = 4;
		Peso[2] = 3;
		Peso[3] = 2;
		Peso[4] = 7;
		Peso[5] = 6;
		Peso[6] = 5;
		Peso[7] = 4;
		Peso[8] = 3;
		Peso[9] = 2;
		for( i = 0, Suma = 0; i <= 9; i++ )
			Suma += Entrada[i] * Peso[i];
		Resto = Suma % 11;
		DVerif = 11 - Resto;

		if( DVerif == 10 )		  /* excepcion DVerif es 10 */
			DVerif = 9;
		else if( DVerif == 11 )	  /* excepcion DVerif es 11 */
			DVerif = 0;

		if( ( DVerif + '0' == clientes.cuit[10] ) || clientes.cond_iva == 5 ||
				clientes.cond_iva == 7 )
			ok = 1;
		else {
			_snprintf( cadena, sizeof(cadena)-1, "%s %s", ST( S_CUIT_INVALIDO ), 
				ST( S____PRESIONE_UNA_TECLA__ ) );
		}

	//}
	if( ok ) {//Debemos verificar que tenga un nombre y una condicion de iva
		TRIM( clientes.nombre );
		if( !strlen( clientes.nombre ) ) {
			_snprintf( cadena, sizeof(cadena)-1, "%s %s", ST( S_EL_CLIENTE_DEBE_TENER_RAZ_SOC ), 
				ST( S____PRESIONE_UNA_TECLA__ ) );
			ok = 0;
		}
		if( ok && !clientes.cond_iva ) {
			_snprintf( cadena, sizeof(cadena)-1, "%s %s", ST( S_EL_CLIENTE_DEBE_TENER_CONDICION_ANTE_EL_IVA ), 
				ST( S____PRESIONE_UNA_TECLA__ ) );
			ok = 0;
		}
	}
	/*if( ok ) {
   		if( clientes.realizar_percepcion_tacis == 'S' &&
				( 0 == strlen( clientes.nro_inscripcion_municipal ) || atol( clientes.nro_inscripcion_municipal ) <= 0 )
				&& ( !ES_CONSUMIDOR_FINAL( clientes.cond_iva ) ) ) {
			if( OBTENER_MUNICIPIO(  ) == 1 )
				_snprintf( cadena, sizeof(cadena)-1, "%s %s", ST( S_EL_CLIENTE_DEBE_REGISTRAR_PERCEPCION_TACIS ), 
					ST( S____PRESIONE_UNA_TECLA__ ) );
			else
				_snprintf( cadena, sizeof(cadena)-1, "%s %s", ST( S_EL_CLIENTE_DEBE_REG_PERCEP_MUNI ), 
					ST( S____PRESIONE_UNA_TECLA__ ) );
			ok = 0;
		}
	}*/

	//CONTROLA QUE SI EL CHECK ESTA EN S Y QUE LA EL NRO DE INSC SEA
	//IGUAL O MAYOR A CERO Y QUE NO SEA CONS FINAL.

	//EL TACIS NO TIENE FECHA DE INICIO Y FIN.
	//NO CONFUNDIR CONTROL DE COMPROBANTE DE AFIP CON TACIS.

	//--- Se hace el control de Fecha siempre y cuando tenga el realiza percepcin_dgi en S
	// y condicion de IVA sea Responsable Inscripto.
	if( ok && REALIZAR_PERCEPCION_DGI(  ) && !ES_CONSUMIDOR_FINAL( clientes.cond_iva ) &&
			( clientes.fec_ini_ins_municipal != 0 && clientes.fec_fin_ins_municipal != 0 ) ) {
		if( clientes.fec_fin_ins_municipal < GET_FECHA_DOS(  ) ){
			_snprintf( cadena, sizeof(cadena)-1, "%s %s", "CONSTANCIA VENCIDA", 
				ST( S____PRESIONE_UNA_TECLA__ ) );
			ok = 0;
		} else if( clientes.fec_ini_ins_municipal > GET_FECHA_DOS(  )  && ok ){
			_snprintf( cadena, sizeof(cadena)-1, "%s %s", "CONSTANCIA NO HABILITADA", 
				ST( S____PRESIONE_UNA_TECLA__ ) );
			ok = 0;
		}
	}
	if( !ok ) {
		MENSAJE_TECLA( cadena );
	} 

	return ok;
}

/*****************************************************************************************************************/
long  LEER_CLIENTE_BANDA(int inicio, int fin , int * lectura_manual,unsigned char* buffer1, unsigned char* buffer2  ) {
	/*****************************************************************************************************************/
	long cod_numerico = 0, cod_seguridad =0;
	int k=-999, seguro=20;
	//Leyó por tarjeta
	if( LECTOR_TARJETA_LEIDA() ) {
		if( LEER_TARJETA_BANDA( (unsigned char *)buffer2, (unsigned char *)buffer1 ) ) {
			if (inicio <=0)
				inicio =8;
			if (fin <=0)
				fin=16;


			//control de limites
			if( INDICE_FIN_NRO_CLIENTE_EN_BANDA > 0 && INDICE_FIN_NRO_CLIENTE_EN_BANDA < strlen( buffer2 ))
				fin = INDICE_FIN_NRO_CLIENTE_EN_BANDA;
			if(config_tps.ClienteTarjetaEspecial && buffer2)
				cod_seguridad = atol( &buffer2[inicio] );
			
			buffer2[ fin ] = 0;//buffer2[ 16] = 0;
			if( INDICE_COMIENZO_CLIENTE_EN_BANDA > 0 && INDICE_COMIENZO_CLIENTE_EN_BANDA < fin )
				inicio = INDICE_COMIENZO_CLIENTE_EN_BANDA;
			//extracción del numero
			cod_numerico = atol( &buffer2[inicio-1] );//cod_numerico = atol( &buffer2[8] );
			*lectura_manual = NO;
			//cargamos en buffer2 el codigo de seguridad para que habilitemos la promo cliente especal
			if(config_tps.ClienteTarjetaEspecial && cod_seguridad >0 ){
				_snprintf(buffer2,6,"%6i", cod_seguridad);
				buffer2[ 6] = 0;
			}

			
		}
	LIMPIA_BUFFER_TARJETA( NO )	;
	LIMPIA_INDICADOR_TARJETA_LEIDA(  );
	do{
		k = GETCH();
		
	}while( !(k ==-999 || k ==  0xf000 )&&(seguro--));
	}
	return cod_numerico;
}