#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cajon.h> 
#include <cio.h> 
#include <clave.h> 
#include <cmemory.h> 
#include <comprob.h> 
#include <consulta.h> 
#include <cr.h>  
#include <cr_disco.h> 
#include <cr_displ.h> 
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <descuent.h> 
#include <ftoc.h> 
#include <getch.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <modo_eje.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <m_hora.h> 
#include <pagos.h> 
#include <pant.h> 
#include <pausa.h> 
#include <perfiles.h> 
#include <pg_fecha.h> 
#include <round.h> 
#include <stringt.h> 
#include <trim.h> 
#include <t_espera.h> 
#include <_cr0.h> 
#include <_cr1.h> 
#include <tarjetas.h>
#include <clientes.h>
#include <cobros.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W) || defined(INVEL_L)
#include <actions.h>
#include <typeacti.h>
#endif
extern int cobro_unipago;
static char borrar_recuadro_de_peticion = 1;
#if defined(INVEL_W) || defined(INVEL_L)
static int muestra_vendedor = 0;
static long nro_vendedor_aux = 0L;
static long afinidad_ant = 0L;
static int afinidad_on = 0;
static int type_ant = 0;
static int signal_on = 0;
static int present_on = 0;
static int mutual_on = 0;
static int devolucion_on = 0;
static int cambio_on = 0;
static int muestra_cliente = 0;
static long nro_cliente_aux = 0L;
static int tarjetas_on = 0;
static int comprob_on = 0;
static int flag_ant = -1;
static int cod_on = 0;
static int cr_on = 0;
static int dev_ant = 0;
static int tipo_dev_on = 0;
static int cant_bultos_ant = -1;
static char tit_bult_ant[15];
static int det_list_on = 1;
static char det_list_ant[15];
static int mozo_ant = 0;
static int muestra_mozo = 0;
static int multiplica_on = 0;
static double mult_ant = 0.0;
static int muestra_saldo = 0;
static double saldo_aux = 0.0;
static char tarj_ant[21],cad_ant[21];
long codigo_consecutivo = 0L;
char barrar_consecutivo[17];
double cantidad_consecutivo = 0.0;
int pant_activa_cliente = 0;
int pant_activa_saldo = 0;
struct _cont_art cont_art;
int cobro_on_tick = -1;
int cobro_marginal = 0;
int cobro_unico = -1;
extern int disable_standby_automatico;
char prevOffline = 0;
void SET_TARJERTAS_ON( int valor);
#endif
/*****************************************************************************/
void LIMPIAR_RECUADRO_PETICION( int mensaje )
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    int type = 0;
    if( !mensaje ) {
        if( signal_on ) {
            ExecuteSSAction( A_LIMPIA_CUADRO_PETICION, T_DESACTIVA_CUADRO_PETICION, "", "", NULL, " " );       
            signal_on = 0;    
            type_ant = 0;
        }
    }
    else {
        //levantar una bandera que indique que se ingresa un valor en el mismo lugar que el cdigo interno
        switch( mensaje ) {
            case S_NOVIOS:
                type = T_NOVIOS;
                break;                                              
            case S__CLIENTE_AFINIDAD_:
                type = T_CLIENTE_AFINIDAD;         
                break;                                                       
            case S__NRO_DOCUMENTO_CLIENTE__:
                type = T_NRO_DOCUMENTO_CLIENTE;         
                break;                                                                   
            case S_NRO_COMPROBANTE:
                type = T_NRO_COMPROBANTE;         
                break;                                                                   
            case S_FECHA_INICIAL:
                type = T_FECHA_INICIAL;         
                break;                                                                   
            case S_FECHA_FINAL:
                type = T_FECHA_FINAL;                                             
                break;
        }     
        if( type != type_ant ) {
            if( signal_on ) {
                ExecuteSSAction( A_LIMPIA_CUADRO_PETICION, T_DESACTIVA_CUADRO_PETICION, "", "",
                                 NULL, " " );
            }
            else {
                signal_on = 1;
            }                         
            ExecuteSSAction( A_LIMPIA_CUADRO_PETICION, type, ST( mensaje ), "", NULL, " " );         
            type_ant = type;
        }
    }
    #else         
    if( mensaje == 0 ) {
        DIBUJAR_RECUADRO_PETICION( NULL, L_CODIGO );
    }
    else {
        DIBUJAR_RECUADRO_PETICION( ST( mensaje ), L_CODIGO );
    }
    #endif
}
/*****************************************************************************/
void DIBUJAR_RECUADRO_PETICION( char *titulo, int digitos )
/*****************************************************************************/
{

}
/*****************************************************************************/
void BORRA_RECUADRO_PETICION( int x1, int x2, int xc1, int xc2 )
/*****************************************************************************/
{
    x1--;
    x2++;
    xc1--;
    xc2++;
}
/*****************************************************************************/
int PEDIR_FECHA_EN_RECUADRO( char *titulo, int salvar_pantalla, char *fecha )
/*****************************************************************************/
{

    long numero;
    ExecuteSSAction( A_PEDIR, 1, titulo, titulo, NULL, " " );
    DIBUJAR_RECUADRO_PETICION( titulo, 9 );
    numero = PEDIR_GRAN_FECHA( ( 80 - ancho_panel - 9 * 3 + 1 ) / 2, 19, fecha );

    ExecuteAction( A_FIN_PEDIR );

    return ( numero );
}
/*****************************************************************************/
long PEDIR_ENTERO_EN_RECUADRO( char *titulo, int salvar_pantalla, int digitos )
/*****************************************************************************/
{
    long numero;
    ExecuteSSAction( A_PEDIR, 1, titulo, titulo, NULL, " " );
    DIBUJAR_RECUADRO_PETICION( titulo, digitos );
    numero = PEDIR_GRAN_ENTERO( ( 80 - ancho_panel - digitos * 3 + 1 ) / 2, 19, digitos );

    ExecuteAction( A_FIN_PEDIR );
    return ( numero );
}
/*****************************************************************************/
int PEDIR_CADENA_EN_RECUADRO( char *titulo, int salvar_pantalla, int digitos, char *cadena,
                              char *mascara_scanner )
/*****************************************************************************/
{
    int pant = 0,rta;
    ExecuteSSAction( A_PEDIR, 1, titulo, titulo, NULL, " " );
    DIBUJAR_RECUADRO_PETICION( titulo, digitos );
    TECLADO_ALFABETICO = SI;
    rta = PEDIR_GRAN_CADENA( ( char )( ( 80 - ancho_panel - digitos * 3 + 1 ) / 2 ), ( char )19,
                             ( char )digitos, cadena, mascara_scanner );
    TECLADO_ALFABETICO = NO;
    ExecuteAction( A_FIN_PEDIR );
    return ( rta );
}
/*****************************************************************************/
double PEDIR_FLOAT_EN_RECUADRO( char *titulo, int salvar_pantalla, int enteros, int decimales )
/*****************************************************************************/
{
    int pant = 0, digitos;
    double valor;
    char label[80];
    strcpy( label, titulo );
    strcat( label, " :" );
    ExecuteSSAction( A_PEDIR, _SIMPLE, " ",label, NULL, " " );
    digitos = enteros + decimales + 1;
    DIBUJAR_RECUADRO_PETICION( titulo, digitos );
    valor = PEDIR_GRAN_FLOTANTE( ( 80 - ancho_panel - digitos * 3 + 1 ) / 2, 19, enteros,
                                 decimales );

    ExecuteAction( A_FIN_PEDIR );
    return ( valor );
}
/*****************************************************************************/
double PEDIR_FLOAT_EN_RECUADRO_CS( char *titulo, int salvar_pantalla, int enteros, int decimales,
                                   int tipo )
/*****************************************************************************/
{
    int pant = 0, digitos;
    double valor;
    ExecuteSSAction( A_PEDIR, tipo, titulo, " ", NULL, " " );
    if( tipo == _MOVIL ) {
        ExecuteSSAction( A_SET_PROP_INPUT, _INPUT_COBROS_ESP, "_MOVIL", "", NULL, " " );
    }
    digitos = enteros + decimales + 1;
    //DIBUJAR_RECUADRO_PETICION( titulo, digitos );
    valor = PEDIR_GRAN_FLOTANTE_CON_SIGNO( ( 80 - digitos * 3 + 1 ) / 2, 19, enteros, decimales );
    ExecuteSSAction( A_SET_PROP_INPUT, _INPUT_GENERICO, "_SIMPLE", "", NULL, " " );
    ExecuteAction( A_FIN_PEDIR );
    return ( valor );
}
/*****************************************************************************/
double PEDIR_FLOAT_EN_RECUADRO_CC( char *titulo, int salvar_pantalla, int enteros, int decimales,
                                   int cajero, int sup, int cond, int cajon, int controlar_efectivo,
                                   int funcion )
/*****************************************************************************/
{
    int pant = 0;
    double valor, rta = 0;
	char titulo_vacio[20];
	memset(titulo_vacio,' ', sizeof(titulo_vacio));
	sprintf(titulo_vacio,"Importe");
	titulo_vacio[19]=0;

	valor = PEDIR_FLOAT_EN_RECUADRO( ((titulo!=NULL)?titulo:titulo_vacio), NO, enteros, decimales );

    if( valor > 0.0 ) {
        if( controlar_efectivo && valor > X_TOT_DINERO ) {
            MENSAJE_STRING( S_NO_HAY_EFECTIVO_SUFICIENTE );
            PAUSA( 36 );
        }
		else if( SOLICITAR_CLAVES( 30, 21, cajero, sup, cond, ((titulo!=NULL)?titulo:"CLAVE"), "CLAVE", funcion, SI ) ) {
            if( !cajon || ABRIR_CAJON() ) {
                rta = valor;
            }
        }
    }
    return ( rta );
}
/*****************************************************************************/
void CR1_CONFECCIONA_PANTALLA()
/*****************************************************************************/
{
    CR1_DISPLAY_DE_PANTALLA();
    DIBUJA_RECUADRO_DEL_TICKET();
    DIBUJAR_RECUADRO_PETICION( NULL, L_CODIGO );
    MUESTRA_COMPROBANTE();
    MOSTRAR_CLIENTE_AFINIDAD();
    MUESTRA_ET_TOMRA();
    MOSTRAR_MODO_DEVOLUCION();
    MUESTRA_TIPO_DEVOLUCION();
    MUESTRA_BULTOS();
	CR1_CONTADOR_INIT();
    MUESTRA_SUBTOTAL(NO);
    #ifdef COMPILAR_TECLA_MODO_CREDITO
    MUESTRA_TIPO_PRECIO();
    #endif
    #ifdef COMPILAR_MOZO
    MOSTRAR_MOZO();
    #endif
    MOSTRAR_MODO_CAMBIO();
    MUESTRA_PERFIL();
    MOSTRAR_VENDEDOR();
    MOSTRAR_CAJERO_EN_PANTALLA();
    MOSTRAR_T_ESPERA_POR_PANTALLA();
	MOSTRAR_MONEDERO_POR_PANTALLA();
    MOSTRAR_DESCUENTOS_EN_PANTALLA();
    MOSTRAR_MODO_BIEN_DE_USO_EN_PANTALLA();
    MUESTRA_MUTUAL();
    MOSTRAR_PORC_INTERES();
    CR1_MUESTRA_TARJETA();
	MUESTRA_DIA_OPERATIV0();
    #ifdef COMPILAR_MODO_ENTRENAMIENTO
    MOSTRAR_MODO_ENTRENAMIENTO_EN_PANTALLA( 1 );
    #endif
}
/*****************************************************************************/
void DIBUJA_RECUADRO_DEL_TICKET()
/*****************************************************************************/
{
    char _cadena[20];
    if( MULTIPLES_USUARIOS_PARA_VENTA ) {
        if( IMPRIMIR_LEGAJO_CAJERO ) {
            sprintf( _cadena, " %s: %.8li ", ST( S_CAJERO__ ), NRO_LEGAJO_CAJERO );
        }
        else {
            sprintf( _cadena, " %s: %.4i ", ST( S_CAJERO__ ), NRO_CAJERO );
        }
        MOSTRAR_CADENA( 16, 7, _cadena );
        if( PEDIR_CAJON_EN_MULTIPLES_USUARIO ) {
            sprintf( _cadena, " %s: %.1i ", ST( S_CAJON ), VARIOS_CAJON_ACTIVO );
            MOSTRAR_CADENA( 32, 7, _cadena );
        }
    }
}
/*****************************************************************************/
void LIMPIA_RECUADRO_DEL_TICKET()
/*****************************************************************************/
{
}
/*****************************************************************************/
void CR1_DISPLAY_DE_PANTALLA()
/*****************************************************************************/
{
}
/*****************************************************************************/
void BORRA_DISPLAY_DE_PANTALLA()
/*****************************************************************************/
{
    int j;
    char auxi[] = "                 ";
    _ATRIBUTO = 59;
    j = strlen( auxi );
    auxi[j - ancho_panel / 2] = '\0';
    MOSTRAR_GRAN_CADENA( 3, 3, auxi );
}
/*****************************************************************************/
void CR1_MUESTRA_MULTIPLICACION()
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    char cant[15];
    if( RAM_CANTIDAD == 1 ) {
        if( multiplica_on ) {
            ExecuteSAction( A_MUESTRA_MULTIPLICACION, " " );
            multiplica_on = 0;
        }
    }
    else {
        if( !CANTIDADES_CON_DECIMALES ) {
            MOSTRAR_CADENA( 22 - ancho_panel / 2, 18, "      X " );
            MOSTRAR_ENTERO( 23 - ancho_panel / 2, 18, "9999", ( int )RAM_CANTIDAD );
            _itoa( ( int )RAM_CANTIDAD, cant, 10 );
        }
        else {
            MOSTRAR_CADENA( 22 - ancho_panel / 2, 18, "          X " );
            if( config.pais == COLOMBIA || config.pais == ELSALVADOR ) {
                FTOC( RAM_CANTIDAD, cant, ENTEROS, DECIMALES, 1, 0 );
            }
            else {
                FTOC( RAM_CANTIDAD, cant, 4, 3, 1, 0 );
            }
        }
        strcat( cant, "  X" );
        if( !multiplica_on || mult_ant != RAM_CANTIDAD ) {
            ExecuteSAction( A_MUESTRA_MULTIPLICACION, cant );
            multiplica_on = 1;
            mult_ant = RAM_CANTIDAD;
        }
        else {
            ExecuteSAction( A_MUESTRA_MULTIPLICACION, " " );
            multiplica_on = 0;
            mult_ant = RAM_CANTIDAD;
        }
    }
    #else
    if( RAM_CANTIDAD == 1 ) {
        _ATRIBUTO = 52;
        MOSTRAR_CADENA( 22 - ancho_panel / 2, 18, "������" );
    }
    else {
        _ATRIBUTO = 120;
        if( !CANTIDADES_CON_DECIMALES ) {
            MOSTRAR_CADENA( 22 - ancho_panel / 2, 18, "      X " );
            MOSTRAR_ENTERO( 23 - ancho_panel / 2, 18, "9999", ( int )RAM_CANTIDAD );
        }
        else {
            MOSTRAR_CADENA( 22 - ancho_panel / 2, 18, "          X " );
            if( config.pais == COLOMBIA || config.pais == ELSALVADOR ) {
                MOSTRAR_FLOTANTE( 23 - ancho_panel / 2, 18, ENTEROS, DECIMALES, 0, 0,
                                  RAM_CANTIDAD );
            }
            else {
                MOSTRAR_FLOTANTE( 23 - ancho_panel / 2, 18, 4, 3, 0, 0, RAM_CANTIDAD );
            }
        }
    }
    #endif
}
/********************************************************************************/
void CR1_MUESTRA_ARTICULO_EN_GRANDE()
/********************************************************************************/
{
    char aux[50];
    int posx = 40;
    double precio = 0.0;
    _ATRIBUTO = 59;
    strcpy( aux, ITEM_NOMBRE_LARGO );
    aux[18 - ancho_panel / 2] = ' ';
    aux[19 - ancho_panel / 2] = 0;
    _LETRA_GRANDE_PEGADA = 1;
    MOSTRAR_GRAN_CADENA( 3, 3, aux );
    if( config.pais == COLOMBIA || config.pais == ELSALVADOR ) {
        posx = 30;
    }
    precio = ( ( MOSTRAR_SIN_IVA_ELSALVADOR() || MOSTRAR_PRECIO_SIN_IVA_EN_VENTAS ) )
           ? ITEM_PRECIO_SIN_IVA : ITEM_PRECIO;
    //precio = ( MOSTRAR_PRECIO_SIN_IVA_EN_VENTAS ) ? ITEM_PRECIO_SIN_IVA : ITEM_PRECIO;
    MOSTRAR_GRAN_FLOTANTE( posx - ancho_panel, 3, ENTEROS, DECIMALES, 1, 0, precio );
    _LETRA_GRANDE_PEGADA = 0;
}
/********************************************************************************/
void __CR1_MUESTRA_ARTICULO_EN_GRANDE( int item )
/********************************************************************************/
{
    char aux[50];
    int posx = 40;
    double precio;
    char aux1[50];
    char cod_barra[17];
    precio = _TICKET_PRECIO( item );
    if( MOSTRAR_SIN_IVA_ELSALVADOR() || MOSTRAR_PRECIO_SIN_IVA_EN_VENTAS ) {
        precio -= ( _TICKET_IVA1_PRECIO( item ) + _TICKET_IVA2_PRECIO( item )
                  + _TICKET_IVA1_REC( item ) + _TICKET_IVA2_REC( item ) );
    }
    if( buscar_articulo_en_mostrar_item ) {
        if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
            memcpy( cod_barra, _TICKET_COD_BARRA( item ), 17 );
            BUSCA_COD_INTERNO_COD_BARRA( _TICKET_COD_ESPECIAL( item ), cod_barra );
        }
        else {
            BUSCA_CODIGO_ESPECIAL( _TICKET_COD_ESPECIAL( item ) );
        }
    }
    _ATRIBUTO = 59;
    strcpy( aux, articulo.nombre );
    aux[18 - ancho_panel / 2] = ' ';
    aux[19 - ancho_panel / 2] = 0;
    _LETRA_GRANDE_PEGADA = 1;
    MOSTRAR_GRAN_CADENA( 3, 3, aux );
    if( config.pais == COLOMBIA || config.pais == ELSALVADOR ) {
        posx = 30;
    }
    FTOC( precio, aux1, ENTEROS, 4, 1, 0 );
    strcat( aux1, "   " );
    strcat( aux, aux1 );
    ExecuteSAction( A_MUESTRA_ARTICULO, aux1 );
    _LETRA_GRANDE_PEGADA = 0;
}
/********************************************************************************/
void MUESTRA_BULTOS()
/********************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    char pr[20],tit[15],det_list[25];
    int cant_aux = -1;
    if( RAM_MODO_EJECUCION == INVENTARIO ) {
        cant_aux = RAM_RENGLONES;
        strncpy( tit, ST( S_RENGLONES ), sizeof( tit ) );
        det_list_on = 1;
        sprintf( det_list, "%s %d", ST( S_TIPO_INV__ ), RAM_TIPO_INVENTARIO );
    }
    else if( RAM_MODO_EJECUCION == TRANSFERENCIAS ) {
        strcpy( tit, " " );
        cant_aux = -1;
        det_list_on = 1;
        sprintf( det_list, "%s %d", ST( S_TRANSF__ ), RAM_SUCURSAL_ANTERIOR );
    }
    else {
        cant_aux = RAM_BULTOS;
        strncpy( tit, ST( S_BULTOS ), 14 );
        det_list_on = 0;
    }
    FTOC( cant_aux, pr, ENTEROS, 0, 1, 0 );
    LTRIM( pr );
	if( !cant_aux && cant_aux == cant_bultos_ant ) {
		//Para que muestre el bulto en 0 en caso de cant_aux = cant_bultos_ant = 0
		cant_bultos_ant = -1;
	}
    if( cant_aux == -1 ) {
        ExecuteSAction( A_MUESTRA_BULTOS, " " );
        cant_bultos_ant = cant_aux;
    }
    else if( cant_aux != cant_bultos_ant ) {
        ExecuteSAction( A_MUESTRA_BULTOS, pr );
        cant_bultos_ant = cant_aux;
    }
    if( strcmp( tit_bult_ant, tit ) != 0 && RAM_MODO_EJECUCION != INVENTARIO) {
        ExecuteSAction( A_MUESTRA_TIT_BULTOS, tit );
        strncpy( tit_bult_ant, tit, 14 );
    }
    if( det_list_on ) {
        if( strcmp( det_list, det_list_ant ) != 0 ) {
            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_DET_LIST_BORRAR, " ", " ", NULL, " " );
            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_DET_LIST_AGREGAR, det_list, " ", NULL, " " );
            strcpy( det_list_ant, det_list );
        }
    }
    else {
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_DET_LIST_BORRAR, " ", " ", NULL, " " );
        ExecuteSAction( A_MUESTRA_TIT_BULTOS, tit );
        memset( det_list, 0, 15 );
        memset( det_list_ant, 0, 15 );
    }
    #else
    _ATRIBUTO = 15;
    if( RAM_MODO_EJECUCION == INVENTARIO ) {
        MOSTRAR_CADENA( 66 - ancho_panel, 21, ST( S_TIPO_INV______ ) );
        MOSTRAR_ENTERO( 76 - ancho_panel, 21, "9999", RAM_TIPO_INVENTARIO );
        MOSTRAR_CADENA( 66 - ancho_panel, 22, ST( S_CANT_RENG_____ ) );
        MOSTRAR_ENTERO( 76 - ancho_panel, 22, "9999", RAM_RENGLONES );
    }
    else if( RAM_MODO_EJECUCION == TRANSFERENCIAS ) {
        MOSTRAR_CADENA( 69 - ancho_panel, 21, ST( S_TRANSF______ ) );
        MOSTRAR_ENTERO( 76 - ancho_panel, 21, "9999", RAM_SUCURSAL_ANTERIOR );
    }
    else {
        MOSTRAR_CADENA( 66 - ancho_panel, 21, ST( S____ARTIC______ ) );
        MOSTRAR_ENTERO( 76 - ancho_panel, 21, "9999", RAM_BULTOS );
    }
    #endif
}
/********************************************************************************/
void MUESTRA_SUBTOTAL( int recalcular)
/********************************************************************************/
{
    char pr[20];
	double total = 0.00;
    #if defined(INVEL_W) || defined(INVEL_L)
    if( RAM_TOTAL ) {
        //FTOC( RAM_TOTAL, pr, ENTEROS, DECIMALES, 1, 1 );
        //FTOC( TOT_TICK_IMPORTE_SIN_IVA + TOT_TICK_IVA1 + TOT_TICK_IVA2 + TOT_TICK_EXENTO + TOT_TICK_IMP_INT, pr, ENTEROS, DECIMALES, 1, 1 );
		total = SUMAR_TOTAL_FAC_CAJA( recalcular);
		//Al invertir la funcionalidad de Cambio de Mercaderia no hace falta la condicion RAM_CAMBIO == 1
		if( total && ( MODO_DEVOLUCION || RAM_MODO_DEVOLUCION 
				|| RAM_NOTA_CR || RAM_CAMBIO == 1 ) ) {//RAM_CAMBIO = 1 = _CAMBIO_DE_MERCADERIA_MENOS 
   			total *= -1;
   		}   
		_snprintf( pr, sizeof( pr )-1, "%*.*f", ENTEROS, DECIMALES, total );
    }
    else {
        sprintf( pr, " " );
		if(RAM_MODO_EJECUCION == INVENTARIO && RAM_P_TICKET ) {
            sprintf( pr, " " );
            ExecuteSAction( A_MUESTRA_SUBTOTAL, pr );
            strcpy(pr, ST( S_INVENTARIO_ ));

		} else {
		    ExecuteSAction( A_MUESTRA_SUBTOTAL, pr );
			ExecuteSAction( A_MUESTRA_ARTICULO, pr );
			ExecuteSAction( A_MUESTRA_DESCRIPCION, pr );
			ExecuteSAction( A_MUESTRA_CANTIDAD, pr );
        }


    }
    ExecuteSAction( A_VENTA_MUESTRA_TOTAL, pr );
    #else 
    int posx = 59;
    if( config.pais == CHILE || config.pais == BRASIL ) {
        posx = 58;
    }
    if( config.pais == COLOMBIA || config.pais == ELSALVADOR ) {
        posx = 53;
    }
    if( RAM_MODO_EJECUCION != INVENTARIO && RAM_MODO_EJECUCION != TRANSFERENCIAS ) {
        _ATRIBUTO = 52;
        if( ( config.pais == CHILE || config.pais == BRASIL ) && RAM_TOTAL > 9999999.99 ) {
            _LETRA_GRANDE_PEGADA = 1;
            MOSTRAR_GRAN_FLOTANTE( posx - ancho_panel, 3, ENTEROS + 3, DECIMALES, 1, 0, RAM_TOTAL );
            _LETRA_GRANDE_PEGADA = 0;
        }
        else {
            MOSTRAR_GRAN_FLOTANTE( posx - ancho_panel, 3, ENTEROS, DECIMALES, 1, 0, RAM_TOTAL );
        }
    }
    else {
        _ATRIBUTO = 63;
        _LETRA_GRANDE_PEGADA = 1;
        MOSTRAR_GRAN_CADENA( posx - ancho_panel, 3,
                             ( RAM_MODO_EJECUCION == INVENTARIO ) ? ST( S_INVENTARIO_ )
                           : ST( S_TRANSFERENC_ ) );
        _LETRA_GRANDE_PEGADA = 0;
    }
    #endif
}
/********************************************************************************/
void MUESTRA_CARTEL_INICIAL()
/********************************************************************************/
{
    char auxi[50];
    _ATRIBUTO = 59;
    if( ancho_panel == 0 ) {
       // sprintf( auxi, ST( S_CREDITO_O_CONTADO ) );
		 _snprintf( auxi, sizeof(auxi)-1, ST( S_CREDITO_O_CONTADO ));
    }
    else {
        _snprintf( auxi, sizeof(auxi)-1,ST( S_CRED_O_CONT_ ) );
    }
    _ATRIBUTO = 59;
    MOSTRAR_GRAN_CADENA( 3, 3, auxi );
	memset(auxi,0,sizeof(auxi));
}
/********************************************************************************/
void MUESTRA_ITEM_EN_PANTALLA( int nro )
/********************************************************************************/
{
    /*--------- muestra datos -------------------*/
    CR1_MOSTRAR_ITEM( nro, 9, NO );
    /*---------- muestra el articulo --------------*/
    #if defined(INVEL_W) || defined(INVEL_L)
    __CR1_MUESTRA_ARTICULO_EN_GRANDE( nro );
    #else
    CR1_MUESTRA_ARTICULO_EN_GRANDE( nro );
    #endif
    #ifdef COMPILAR_DISPLAY
    //MOSTRAR_ITEM_EN_DISPLAY( ITEM_NOMBRE_LARGO,
    //  ( ITEM_PRECIO + ITEM_RECARGO ) * RAM_CANTIDAD );
    MOSTRAR_ITEM_EN_DISPLAY( articulo.nombre,
                             ( _TICKET_PRECIO( nro ) + _TICKET_RECARGO( nro ) )
                           * _TICKET_CANTIDAD( nro ) );       
    #endif
}
/********************************************************************************/
void MUESTRA_DATOS_CONTADOR_PANTALLA( int nro, int MostrarDatos )
/********************************************************************************/
{	
	char desc[31], cant[15], cant1[15];
	
	memset( desc, 0, 31 );
	memset( cant, 0, 15 );
	memset( cant1, 0, 15 );

    if( MostrarDatos ) {
		
		CR1_CONTADOR_ARTICULO( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, nro );

		if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
            BUSCA_COD_INTERNO_COD_BARRA( cont_art.cod, cont_art.cod_barra );
        }
        else {
			BUSCA_CODIGO_ESPECIAL( cont_art.cod );
        }
        strncpy( desc, articulo.nombre, 31 );
		FTOC( cantidad_consecutivo, cant, 4, 0, 0, 0 );
		FTOC( cont_art.cant, cant1, 4, 0, 0, 0 );
	}

	ExecuteSAction( A_MUESTRA_DESCRIPCION_CONTADOR, desc );
	ExecuteSAction( A_MUESTRA_CONTADOR_DIFERENCIAL, cant );
	ExecuteSAction( A_MUESTRA_CONTADOR_TOTAL_IDEN, cant1 );

}
/*****************************************************************************/
void CR1_MUESTRA_TARJETA()
/*****************************************************************************/
{
    char *cmoneda[] = {"$","D","C"}, cadena[50], aux[30];
    if( VAR_TARJETAS_STATUS ) {
        _ATRIBUTO = 120;
        strncpy( cadena, TARJ[VAR_TARJETAS_TARJETA].nombre, sizeof( cadena ) );      
        if( VAR_TARJETAS_STATUS == 2 ) {
            strncpy( cadena, " ( ", sizeof( cadena ) );
            sprintf( aux, "%s%02i", cmoneda[MONEDA_EXTENDIDA()], VAR_TARJETAS_CUOTAS );
            strcat( cadena, aux );
            if( VAR_TARJETAS_IMPORTE_CUOTA ) {
                FTOC( VAR_TARJETAS_IMPORTE_CUOTA, aux, ENTEROS, DECIMALES, 0, 0 );
                strcat( cadena, " " );
                strcat( cadena, aux );
            }
            if( VAR_TARJETAS_NRO_CUPON_INT ) {
                sprintf( aux, " %02li", VAR_TARJETAS_NRO_CUPON_INT );
                strcat( cadena, aux );
            }
            strcat( cadena, ") " );
        }
        if( !tarjetas_on ) {
            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_TARJETA_AGREGAR, cadena, " ", NULL, " " );
            strcpy( tarj_ant, GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/ );
            strcpy( cad_ant, cadena );
            tarjetas_on = 1;
        } else if( tarjetas_on
              && ( strcmp( tarj_ant, GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/ ) != 0
                || strcmp( cad_ant, cadena ) != 0 ) ) {
            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_TARJETA_BORRAR, " ", " ", NULL, " " );
            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_TARJETA_AGREGAR, cadena, " ", NULL, " " );
            strcpy( tarj_ant, GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/ );
            strcpy( cad_ant, cadena );
        }
    } else {
        if( tarjetas_on ) {
            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_TARJETA_BORRAR, " ", " ", NULL, " " );
            tarjetas_on = 0;
            memset( tarj_ant, 0, sizeof( tarj_ant ) );
        }
    }
}
/*****************************************************************************/
void MUESTRA_MUTUAL()
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    char cad1[20];
    if( MUTUAL_CODIGO /*&& !mutual_on*/ ) {
        memset( cad1, 0, sizeof( cad1 ) );
        sprintf( cad1, "MUTUAL (%02d)", MUTUAL_CUOTAS );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_MUTUAL_AGREGAR, cad1, " ", NULL, " " );
        mutual_on = 1;
    }
    else if( !MUTUAL_CODIGO && mutual_on ) {
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_MUTUAL_BORRAR, " ", " ", NULL, " " );
        mutual_on = 0;
    } 
    #else   
    if( MUTUAL_CODIGO ) {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 66 - ancho_panel, 23, ST( S__MUTUAL______ ) );
        MOSTRAR_ENTERO( 75 - ancho_panel, 23, "00", MUTUAL_CUOTAS );
    }
    else {
        _ATRIBUTO = 7;
        MOSTRAR_CADENA( 66 - ancho_panel, 23, "             " );
    }
    #endif
}
/*****************************************************************************/
void MUESTRA_CLIENTE( int cheque )
/*****************************************************************************/
{

	if( MODO_NEGOCIO != SERVICIOS /*&& RAM_NRO_CLIENTE*/ ) {
		if( RAM_NRO_CLIENTE ) {
            _ATRIBUTO = 15;
            if( !cheque ) {
                MOSTRAR_CADENA( 2, 20, ST( S_COD_FIN_ ) );
                MOSTRAR_ENTERO( 10, 20, "99", NRO_COD_CUOTA );
                MOSTRAR_CADENA( 2, 22, ST( S_SALDO________________ ) );
                MOSTRAR_FLOTANTE( 8, 22, ENTEROS, DECIMALES, 1, 0, clientes.cupo - clientes.saldo );
            }
			if( clientes.exento_ing_brutos == 'N' ) {
   				MOSTRAR_CADENA( 2, 23, ST( S_RETENCION_DGR ) );
   			} else {
   			   MOSTRAR_CADENA( 2, 23, "                     " );
			}
        }
	} else {
            _ATRIBUTO = 7;
            if( !cheque ) {
                MOSTRAR_CADENA( 2, 20, "          " );
                MOSTRAR_CADENA( 2, 22, "                " );
			    MOSTRAR_CADENA( 2, 23, "                     " );
            }
    }

    if( cheque ) {
		SET_MEMORY_CHAR( __ram_mostrar_saldo_cheque, 1 );
    } else {
		SET_MEMORY_CHAR( __ram_mostrar_saldo_cheque, 0 );
    }
	//MOSTRAR_CLIENTE_EN_PANTALLA();
    //MUESTRA_REFERENCIA_NC(  );
	MUESTRA_COMPROBANTE();

}

/*****************************************************************************/
void MOSTRAR_CLIENTE_EN_PANTALLA()
/*****************************************************************************/
{
    int ok = 1;
    char s_saldo[25];
    char stringAux[80],stringAux2[50];
    double saldo = 0.0;
    if( MODO_NEGOCIO == SERVICIOS ) {
        return;
    }
    memset( stringAux, 0, 80 );
    memset( stringAux2, 0, 50 );
    if( RAM_NRO_CLIENTE ) {
        if( !EXISTE_CLIENTE( RAM_NRO_CLIENTE ) ) {
            ok = 0;
            SET_MEMORY_LONG( __ram_nro_cliente, 0 );
        }
    }else {
		muestra_cliente = 1;
		muestra_saldo = 1;
        ok = 0;
    }
	if( ok ) {
		sprintf( stringAux, "%09li - ", RAM_NRO_CLIENTE );
		strcat( stringAux, clientes.nombre );
		if( muestra_cliente && nro_cliente_aux != RAM_NRO_CLIENTE ) {
            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_CLIENTE_BORRAR, " ", " ", NULL, " " );  
            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_CLIENTE_AGREGAR, stringAux, " ", NULL, " " );  
            nro_cliente_aux = RAM_NRO_CLIENTE;
            if( clientes.exento_ing_brutos == 'N'
                 || clientes.exento_ing_brutos == 'n' ) {
                ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_RETENCION_DGR_BORRAR, " ", " ", NULL, " " );
                ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_RETENCION_DGR_AGREGAR, ST( S_RETENCION_X_PORCENTAJE ), " ", NULL, " " );
            }else {
                ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_RETENCION_DGR_BORRAR, " ", " ", NULL, " " );
            }
			if( clientes.ctacte == 'S' ) {
				saldo = clientes.cupo - clientes.saldo;
				strcpy( s_saldo, ST( S_SALDO________________ ) );
				TRIM( s_saldo );
				sprintf( stringAux2, "%s %.02f", s_saldo, ROUND( saldo, ENTEROS, DECIMALES ) );
				if( muestra_saldo && saldo_aux != saldo ) {
					ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_SALDO_BORRAR, " ", " ", NULL, " " );         
					ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_SALDO_AGREGAR, stringAux2, " ", NULL, " " );      
					saldo_aux = saldo;
				}
				else if( !muestra_saldo ) {
					ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_SALDO_AGREGAR, stringAux2, " ", NULL, " " );
					muestra_saldo = 1;
					saldo_aux = saldo;
				}
			} else {
				if( muestra_saldo ) {
					ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_CUPO_CHEQUE_BORRAR, " ", " ", NULL, " " );         
					saldo_aux = saldo;
					muestra_saldo = 0;
				}
			}
        }
		else if( !muestra_cliente ) {
			ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_CLIENTE_AGREGAR, stringAux, " ", NULL, " " );
            ExecuteSAction( A_GET_PANEL_ACTIVO, ( char* )&pant_activa_cliente );
            muestra_cliente = 1;
            nro_cliente_aux = RAM_NRO_CLIENTE;
            if( clientes.exento_ing_brutos == 'N'
                    || clientes.exento_ing_brutos == 'n' ) {
                ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_RETENCION_DGR_BORRAR, " ", " ", NULL, " " );
                ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_RETENCION_DGR_AGREGAR, ST( S_RETENCION_X_PORCENTAJE ), " ", NULL, " " );
            }
			if( clientes.ctacte == 'S' ) {
				saldo = clientes.cupo - clientes.saldo;
				strcpy( s_saldo, ST( S_SALDO________________ ) );
				TRIM( s_saldo );
				_snprintf(stringAux2, sizeof(stringAux2),"%s %.02f", s_saldo, ROUND( saldo, ENTEROS, DECIMALES ) );
				stringAux2[ sizeof(stringAux2)-1 ] = '/0';
				if( muestra_saldo && saldo_aux != saldo ) {
					ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_SALDO_BORRAR, " ", " ", NULL, " " );         
					ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_SALDO_AGREGAR, stringAux2, " ", NULL, " " );      
					saldo_aux = saldo;
				}
				else if( !muestra_saldo ) {
					ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_SALDO_AGREGAR, stringAux2, " ", NULL, " " );
					muestra_saldo = 1;
					saldo_aux = saldo;
				}
			} else {
				if( muestra_saldo ) {
					ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_CUPO_CHEQUE_BORRAR, " ", " ", NULL, " " );         
					saldo_aux = saldo;
					muestra_saldo = 0;
				}
			}
        }
		else if( clientes.ctacte == 'S' ) {
			ExecuteSAction( A_GET_PANEL_ACTIVO, ( char* )&pant_activa_saldo );
			if( muestra_saldo && saldo_aux && pant_activa_saldo == PANTALLA_PAGO ) {
				saldo = saldo_aux;
				strcpy( s_saldo, ST( S_SALDO________________ ) );
				TRIM( s_saldo );
				_snprintf(stringAux2, sizeof(stringAux2),"%s %.02f", s_saldo, ROUND( saldo, ENTEROS, DECIMALES ) );
				stringAux2[ sizeof(stringAux2)-1 ] = '/0';
				ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_SALDO_BORRAR, " ", " ", NULL, " " ); 
				ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_SALDO_AGREGAR, stringAux2, " ", NULL, " " );
			}
        }
		else if( RAM_MOSTRAR_SALDO_CHEQUE ) {//Se comenta provisoriamente
            /*saldo = clientes.limite_cheques - clientes.cupo_usado_cheques;
            sprintf( stringAux2, "CUPO CHEQUE DISP.: %.02f", ROUND( saldo, ENTEROS, DECIMALES ) );
            if( muestra_saldo && saldo_aux != saldo ) {
                ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_CUPO_CHEQUE_BORRAR, " ", " ", NULL );         
                ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_CUPO_CHEQUE_AGREGAR, stringAux2, " ",
                                 NULL );    
                saldo_aux = saldo;
            }
            else if( !muestra_saldo ) {
                ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_CUPO_CHEQUE_AGREGAR, stringAux2, " ",
                                 NULL );
                muestra_saldo = 1;
                saldo_aux = saldo;
            }*/
		}
        else {
            //ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_SALDO_BORRAR, " ", " ", NULL, " " );
			;
        }
    }
    else if( muestra_cliente && !RAM_NRO_CLIENTE ) {
		ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_CLIENTE_BORRAR, " ", " ", NULL, " " );         
		ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_COMP_VTAS_BORRAR, " ", " ", NULL, " " );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_RETENCION_DGR_BORRAR, " ", " ", NULL, " " );
		SET_MEMORY_CHAR( __ram_ticket_factura, 0 );
        muestra_cliente = 0;
        nro_cliente_aux = RAM_NRO_CLIENTE;
        if( muestra_saldo ) {
            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_CUPO_CHEQUE_BORRAR, " ", " ", NULL, " " );         
            saldo_aux = saldo;
            muestra_saldo = 0;
        }
    }
}
/*****************************************************************************/
void BORRAR_CLIENTE_EN_PANTALLA()
/*****************************************************************************/
{   
	ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_CLIENTE_BORRAR, " ", " ", NULL, " " );         
	ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_COMP_VTAS_BORRAR, " ", " ", NULL, " " );
    ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_CUPO_CHEQUE_BORRAR, " ", " ", NULL, " " );         
	ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_RETENCION_DGR_BORRAR, " ", " ", NULL, " " );
}
/*****************************************************************************/
void MOSTRAR_PETICION_DE_PRESENTACIONES( int presentaciones )
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    if( presentaciones && !present_on ) {
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_PRESENT_AGREGAR, "<PRE>", " ", NULL, " " );
        present_on = 1;
    }
    else if( !presentaciones && present_on ) {
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_PRESENT_BORRAR, " ", " ", NULL, " " );
        present_on = 0;
    }
    #else   
    if( presentaciones ) {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 1, 24, "<PRE>" );
    }
    else {
        _ATRIBUTO = 7;
        MOSTRAR_CADENA( 1, 24, "     " );
    }
    #endif
}
/*****************************************************************************/
void MOSTRAR_PETICION_DE_CODIGO( int pedir_cod_especial )
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    if( pedir_cod_especial ) {
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_CODE_VTAS_BORRAR, " ", " ", NULL, " " );
        setData( ACTION_HABILITA_SCANNER, "false", 5 );
        SET_MEMORY_CHAR( __varios_scanner_habilitado, 0 );
    }
    else {
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_CODE_VTAS_AGREGAR, " ", " ", NULL, " " ); 
        setData( ACTION_HABILITA_SCANNER, "true", 4 );
        SET_MEMORY_CHAR( __varios_scanner_habilitado, 1 );
    }
    #else
    if( pedir_cod_especial ) {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 8 - ancho_panel / 2, 24, "<COD>" );
    }
    else {
        _ATRIBUTO = 7;
        MOSTRAR_CADENA( 8 - ancho_panel / 2, 24, "     " );
    }
    #endif
}
#ifdef COMPILAR_TECLA_MODO_CREDITO
/********************************************************************************/
void MUESTRA_TIPO_PRECIO()
/********************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    if( RAM_USAR_PRECIO_CR ) {
        if( !cr_on ) {
            //bam ??
            ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_CRED_VTAS_AGREGAR, " ", " ", NULL, " " );
            cr_on = 1;
        }
    }
    else {
        if( cr_on ) {
            ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_CRED_VTAS_BORRAR, " ", " ", NULL, " " );
            cr_on = 0;
        }
    }
    #else
    if( RAM_USAR_PRECIO_CR ) {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 26 - ancho_panel / 2, 24, "<CR>" );
    }
    else {
        _ATRIBUTO = 7;
        MOSTRAR_CADENA( 26 - ancho_panel / 2, 24, "    " );
    }
    #endif
}
#endif
/********************************************************************************/
void MOSTRAR_MODO_CAMBIO()
/********************************************************************************/
{
    char text[15];
    #if defined(INVEL_W) || defined(INVEL_L)
    int flag;

    switch( RAM_CAMBIO ) {
        case 0:
            flag = 0;
            break;
        case 1:/***** CAMBIO - *****/
            flag = FLAG_CAMBIO_MENOS;
            break;
        case 2:/***** CAMBIO + *****/
            flag = FLAG_CAMBIO_MAS;
            break;
    }
    if( flag > 0 ) {
        if( cambio_on ) {
            if( flag != flag_ant ) {
                _itoa( flag, text, 10 );
                ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_CAMBIO_BORRAR, " ", " ", NULL, " " );
                ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_CAMBIO_AGREGAR, text, " ", NULL, " " );
                flag_ant = flag;
                cambio_on = 1;
            }
        }
        else {
            //se muestra por primera vez
            _itoa( flag, text, 10 );
            ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_CAMBIO_AGREGAR, text, " ", NULL, " " );
            flag_ant = flag;
            cambio_on = 1;
        }
    } else {
        if( cambio_on ) {
            ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_CAMBIO_BORRAR, " ", " ", NULL, " " );
            flag_ant = -1;
            comprob_on = 0;
        }
    }
    #else
    _ATRIBUTO = 120;
    switch( RAM_CAMBIO ) {
        case 0:
            _ATRIBUTO = 7;
            MOSTRAR_CADENA( 31 - ancho_panel / 2, 24, "         " );
            break;
        case 1:
            sprintf( text, "<%s->", ST( S_CAMBIO ) );
            MOSTRAR_CADENA( 31 - ancho_panel / 2, 24, text );
            break;
        case 2:
            sprintf( text, "<%s+>", ST( S_CAMBIO ) );
            MOSTRAR_CADENA( 31 - ancho_panel / 2, 24, text );
            break;
    }
    #endif
}
#ifdef COMPILAR_MOZO
/*****************************************************************************/
void MOSTRAR_MOZO()
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    char cad[10];
    if( muestra_mozo && NRO_MOZO && NRO_MOZO != mozo_ant ) {
        sprintf( cad, "MOZO: %d", NRO_MOZO );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_MOZO_BORRAR, " ", " " );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_MOZO_AGREGAR, cad, " " );
        mozo_ant = NRO_MOZO;
    }
    else if( muestra_mozo && !NRO_MOZO ) {
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_MOZO_BORRAR, " ", " " );
        mozo_ant = NRO_MOZO;
        muestra_mozo = 0;
    }
    else if( !muestra_mozo && NRO_MOZO ) {
        sprintf( cad, "MOZO: %d", NRO_MOZO );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_MOZO_AGREGAR, cad, " " );
        mozo_ant = NRO_MOZO;
        muestra_mozo = 1;
    }
    #else
    if( NRO_MOZO ) {
        _ATRIBUTO = 15;
        MOSTRAR_CADENA( 2, 23, ST( S_MOZO_ ) );
        MOSTRAR_ENTERO( 7, 23, "999", NRO_MOZO );
    }
    else {
        _ATRIBUTO = 7;
        MOSTRAR_CADENA( 2, 23, "        " );
    }
    #endif
}
#endif
/*****************************************************************************/
void MOSTRAR_VENDEDOR()
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    char cad[20] ;
    if( muestra_vendedor && ( nro_vendedor_aux != NRO_VENDEDOR_LONG )
     && ( NRO_VENDEDOR_LONG != 0L ) ) {
        sprintf( cad, "VENDEDOR: %05ld", NRO_VENDEDOR_LONG );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_VENDEDOR_BORRAR, " ", " ", NULL, " " );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_VENDEDOR_AGREGAR, cad, " ", NULL, " " );
        nro_vendedor_aux = NRO_VENDEDOR_LONG;
    }
    else if( muestra_vendedor && NRO_VENDEDOR_LONG == 0L ) {
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_VENDEDOR_BORRAR, " ", " ", NULL, " " );
        nro_vendedor_aux = NRO_VENDEDOR_LONG;
        muestra_vendedor = 0;
    }
    else if( NRO_VENDEDOR_LONG && !muestra_vendedor ) {
        sprintf( cad, "VENDEDOR: %05ld", NRO_VENDEDOR_LONG );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_VENDEDOR_AGREGAR, cad, " ", NULL, " " );
        nro_vendedor_aux = NRO_VENDEDOR_LONG;
        muestra_vendedor = 1;
    }
    #else
    if( NRO_VENDEDOR_LONG ) {
        _ATRIBUTO = 15;
        MOSTRAR_CADENA( 2, 23, ST( S_VEND_ ) );
        MOSTRAR_ENTERO( 7, 23, "99999", NRO_VENDEDOR_LONG );
    }
    else {
        _ATRIBUTO = 7;
        MOSTRAR_CADENA( 2, 23, "          " );
    }
    #endif
}
/*****************************************************************************/
void MOSTRAR_CLIENTE_AFINIDAD()
/*****************************************************************************/
{
    if( MODO_NEGOCIO != SERVICIOS ) {
        if( RAM_CLIENTE_AFINIDAD ) {
            _ATRIBUTO = 15;
            switch( config.pais ) {
                case CHILE:
                case ARGENTINA:
                case BRASIL:
                case ELSALVADOR:
                    if( afinidad_on ) {
                        //el icono ya se esta mostrando
                        if( RAM_CLIENTE_AFINIDAD != afinidad_ant ) {
                            //cambiar datos cliente
                            char cad[50];
							memset(cad,0, sizeof(cad));
                            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_AFINIDAD_BORRAR, " ", " ",
                                             NULL, " " );
                            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_AFINIDAD_AGREGAR, cad, " ",
                                             NULL, " " );
                            afinidad_ant = RAM_CLIENTE_AFINIDAD;
                        }
                    }
                    else {
                        char cad[50];
						memset(cad,0,sizeof(cad));
						if( LONGITUD_NRO_CLIENTE_AFINIDAD >0 && LONGITUD_NRO_CLIENTE_AFINIDAD < 45) {
							_snprintf(cad, LONGITUD_NRO_CLIENTE_AFINIDAD+5,"AF: %ld ", RAM_CLIENTE_AFINIDAD);
							if( clientes.nombre != NULL && strlen(clientes.nombre) > 0) {
								int canti = 0;
								canti= 29 - strlen(cad);
								strncat( cad, clientes.nombre, canti );
							}

						}else
							sprintf( cad, "AF: %09ld", RAM_CLIENTE_AFINIDAD );
                        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_AFINIDAD_AGREGAR, cad, " ",
                                         NULL, " " );               
                        afinidad_ant = RAM_CLIENTE_AFINIDAD;
                        afinidad_on = 1;
                    }
                    break;
                case COLOMBIA:
                    if( afinidad_on ) {
                        //el icono ya se esta mostrando
                        if( RAM_CLIENTE_AFINIDAD != afinidad_ant ) {
                            char cad[50];
                            sprintf( cad, "AF: %s", RAM_STRING_CLIENTE_AFINIDAD );
                            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_AFINIDAD_BORRAR, " ", " ",
                                             NULL, " " );
                            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_AFINIDAD_AGREGAR, cad, " ",
                                             NULL, " " );
                            afinidad_ant = RAM_CLIENTE_AFINIDAD;
                        }
                    }
                    else {
                        char cad[50];
                        sprintf( cad, "AF: %s", RAM_STRING_CLIENTE_AFINIDAD );
                        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_AFINIDAD_AGREGAR, cad, " ",
                                         NULL, " " );
                        afinidad_ant = RAM_CLIENTE_AFINIDAD;
                        afinidad_on = 1;
                    }
                    break;
            }
        }
        else {
            if( afinidad_on ) {
                ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_AFINIDAD_BORRAR, " ", " ", NULL, " " );
                afinidad_on = 0;
            }
        }
    }
}
/*****************************************************************************/
void MUESTRA_COMPROBANTE()
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    int flag = -1;
    char aux[10];
    memset( aux, 0, 10 );
    //1: <FAC>
    //2: <T F>
    //3: <NC>
    //4: GUIA   :
    //5: REMITO :
    //6: <GUIA>
    //7: <REM>
    // : <REF>
    //: borrar
	if( RAM_FACTURA ) {
        _ATRIBUTO = 120;
		if( RAM_NOTA_CR == 3 ){
			flag = ( ( RAM_REFERENCIA_NC)?FLAG_NC_REF:FLAG_NC );
		}else{
			if( RAM_NOTA_CR && RAM_NOTA_CR < 3 && !RAM_REFERENCIA_NC ) {
				MOSTRAR_CADENA( 14 - ancho_panel / 2, 24, ST( S__NC_ ) );
				flag = ((RAM_NOTA_CR == 1)?FLAG_NC_A:FLAG_NC_B);
				//flag = FLAG_NC;
			}
			else {
				if( RAM_NOTA_CR && RAM_REFERENCIA_NC ) {
					flag = ((RAM_NOTA_CR == 1)?FLAG_NC_A_REF:FLAG_NC_B_REF);
					//flag = FLAG_NC_REF;
				}
				else {
					MOSTRAR_CADENA( 14 - ancho_panel / 2, 24, ST( S__FAC_ ) );
					flag = OBTENER_FLAG_COMPROBANTE( clientes.cond_iva, 0 );
				}
			}
		}
    }
    else if( RAM_TICKET_FACTURA ) {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 14 - ancho_panel / 2, 24, ST( S__T_F_ ) );
        flag = OBTENER_FLAG_COMPROBANTE( clientes.cond_iva, 1 );
    }
	else if( NRO_REMITO ) {
        if( PEDIR_NRO_REMITO ) {
            _ATRIBUTO = 15;
            if( IMPRESORA_FISCAL ) {
                MOSTRAR_CADENA( 2, 22, ST( S_GUIA____ ) );
            }
            else {
                MOSTRAR_CADENA( 2, 22, ST( S_REMITO__ ) );
            }
            MOSTRAR_ENTERO( 11, 22, "99999999", NRO_REMITO );
        }
        _ATRIBUTO = 120;
        if( IMPRESORA_FISCAL ) {
            MOSTRAR_CADENA( 14, 24, ST( S__GUIA_ ) );
            flag = FLAG_GUIA;
        }
        else {
            MOSTRAR_CADENA( 14, 24, ST( S__REM_ ) );
            flag = FLAG_REM;
        }
    }
    else {
        _ATRIBUTO = 7;
        MOSTRAR_CADENA( 2, 22, "                 " );
        MOSTRAR_CADENA( 14, 24, "      " );
    }
    if( flag >= 0 ) {
        if( comprob_on ) {
            //si ya se est�mostrando el �ono en la pantalla
            if( flag != flag_ant ) {
                _itoa( flag, aux, 10 );
                ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_COMP_VTAS_BORRAR, " ", " ", NULL, " " );
                ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_COMP_VTAS_AGREGAR, aux, " ", NULL, " " );
                flag_ant = flag;
                comprob_on = 1;
            }
        }
        else {
            //se muestra por primera vez
            _itoa( flag, aux, 10 );
            ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_COMP_VTAS_AGREGAR, aux, " ", NULL, " " );
            flag_ant = flag;
            comprob_on = 1;
        }
    }
    else {
        if( comprob_on ) {
            ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_COMP_VTAS_BORRAR, " ", " ", NULL, " " );
            flag_ant = -1;
            comprob_on = 0;
        }
    }
    #else
    //1: <FAC>
    //2: <T F>
    //3: <NC>
    //4: GUIA   :
    //5: REMITO :
    //6: <GUIA>
    //7: <REM>
    //: borrar
    if( RAM_FACTURA ) {
        _ATRIBUTO = 120;
        if( RAM_NOTA_CR ) {
            MOSTRAR_CADENA( 14 - ancho_panel / 2, 24, "<NC>" );
        }
        else {
            MOSTRAR_CADENA( 14 - ancho_panel / 2, 24, "<FAC>" );
        }
    }
    else if( RAM_TICKET_FACTURA ) {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 14 - ancho_panel / 2, 24, "<T F>" );
    }
    else if( NRO_REMITO ) {
        if( PEDIR_NRO_REMITO ) {
            _ATRIBUTO = 15;
            if( IMPRESORA_FISCAL ) {
                MOSTRAR_CADENA( 2, 22, "GUIA   :" );
            }
            else {
                MOSTRAR_CADENA( 2, 22, "REMITO :" );
            }
            MOSTRAR_ENTERO( 11, 22, "99999999", NRO_REMITO );
        }
        _ATRIBUTO = 120;
        if( IMPRESORA_FISCAL ) {
            MOSTRAR_CADENA( 14, 24, "<GUIA>" );
        }
        else {
            MOSTRAR_CADENA( 14, 24, "<REM>" );
        }
    }
    else {
        _ATRIBUTO = 7;
        MOSTRAR_CADENA( 2, 22, "                 " );
        MOSTRAR_CADENA( 14, 24, "      " );
    }
    #endif
    MOSTRAR_CLIENTE_EN_PANTALLA();
}
/*****************************************************************************/
void REGENERAR_PANTALLA_TICKET()
/*****************************************************************************/
{
    int h;
    buscar_articulo_en_mostrar_item = 1;
    for( h = 0;h < RAM_P_TICKET;h++ ) {
        SET_MEMORY_DOUBLE( __ram_cantidad_original, ROUND( _TICKET_CANTIDAD( h ), 5, 3 ) );
        MUESTRA_ITEM_EN_PANTALLA( h );
    }
    buscar_articulo_en_mostrar_item = 0;
    cobro_marginal = COBROS_VENTA_EN_TICKET( &cobro_on_tick, &cobro_unico );
    MUESTRA_SUBTOTAL(NO);
    CR1_MUESTRA_TARJETA();
    MUESTRA_MUTUAL();
    MOSTRAR_MODO_DEVOLUCION();
}
/*****************************************************************************/
//BOX_AUTORIZAR( char *titulo, int salvar_pantalla, int cajero, int sup, int cond )
/*****************************************************************************/
/*
 * {
 * int pant, rta, x=29, l=20, lt, xt, attr;
 *
 * attr = _ATRIBUTO;
 * if( salvar_pantalla ) pant = SAVE_SCREEN(1,19,80,24);
 * if( titulo ) {
 * lt = strlen(titulo);
 * if( lt >= l ) {
 * x = x - (lt-l) / 2;
 * l = lt+1;
 * }
 * xt = x + (l-lt) / 2 + 1;
 * }
 * _ATRIBUTO = 120;
 * BOX( x, 19, x+l, 22 );
 * if( titulo ) {
 * _ATRIBUTO = 127;
 * MOSTRAR_CADENA( xt, 19, titulo );
 * }
 * rta = SOLICITAR_CLAVES( 30, 21, cajero, sup, cond, NULL, NULL, SI );
 * if( salvar_pantalla ) RESTORE_SCREEN(pant);
 * _ATRIBUTO = attr;
 *
 * return( rta );
 * }
 */
/*****************************************************************************/
void MUESTRA_HORA_EN_PANTALLA()
/*****************************************************************************/
{
    int atr,grande;
    if( MOSTRAR_HORA_PANTALLA ) {
        atr = _ATRIBUTO;
        grande = _GRANDE;
        _ATRIBUTO = 7;
        _GRANDE = 0;
        MOSTRAR_FECHA( 1, 1, GET_FECHA_DOS() );
        MOSTRAR_HORA_COMPLETA( 73 - ancho_panel, 1, GET_HORA_COMPLETA_DOS() );
        if( cajero_operando ) {
            MOSTRAR_CADENA( pos_x_cajero, 1, dato__cajero );
        }
        _GRANDE = grande;
        _ATRIBUTO = atr;
    }
}
/*****************************************************************************/
void MUESTRA_TIPO_DEVOLUCION()
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    int tipo_dev = 0;
    char aux[10];
    memset( aux, 0, 10 );
    if( RAM_DIFERENCIA_PRECIO ) {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 30 - ancho_panel / 2, 24, "<DIF >" );
        tipo_dev = FLAG_DIF;
    }
    else if( RAM_COMPROMISO_200 ) {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 30 - ancho_panel / 2, 24, "<200%>" );
        tipo_dev = FLAG_COMPROMISO;
    }
    else if( RAM_PRECIO_MAS_BAJO ) {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 30 - ancho_panel / 2, 24, "<BAJO>" );
        tipo_dev = FLAG_BAJO;
    }
    else {
        _ATRIBUTO = 7;
        MOSTRAR_CADENA( 30 - ancho_panel / 2, 24, "      " );
    }
    if( tipo_dev ) {
        if( tipo_dev_on ) {
            //si ya se est�mostrando el �ono en la pantalla
            if( tipo_dev != dev_ant ) {
                _itoa( tipo_dev, aux, 10 );
                ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_TIPO_DEV_BORRAR, " ", " ", NULL, " " );
                ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_TIPO_DEV_AGREGAR, aux, "  ", NULL, " " );
                dev_ant = tipo_dev;
            }
        }
        else {
            //se muestra por primera vez
            _itoa( tipo_dev, aux, 10 );
            ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_TIPO_DEV_AGREGAR, aux, aux, NULL, " " );
            dev_ant = tipo_dev;
            tipo_dev_on = 1;
        }
    }
    else {
        if( tipo_dev_on ) {
            ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_TIPO_DEV_BORRAR, " ", " ", NULL, " " );
            dev_ant = -1;
            tipo_dev_on = 0;
        }
    }
    #else
    int at;
    at = _ATRIBUTO;
    if( RAM_DIFERENCIA_PRECIO ) {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 30 - ancho_panel / 2, 24, ST( S__DIF__ ) );
        #ifdef COMPILAR_COMPROMISO_200
    }
    else if( RAM_COMPROMISO_200 ) {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 30 - ancho_panel / 2, 24, ST( S__200__ ) );
        #endif
    }
    else if( RAM_PRECIO_MAS_BAJO ) {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 30 - ancho_panel / 2, 24, ST( S__BAJO_ ) );
    }
    else if( RAM_REFERENCIA_NC ) {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 30 - ancho_panel / 2, 24, ST( S__REF_ ) );
    }
    else {
        _ATRIBUTO = 7;
        MOSTRAR_CADENA( 30 - ancho_panel / 2, 24, "      " );
    }
    _ATRIBUTO = at;
    #endif
}
/*****************************************************************************/
long PEDIR_ENTERO_XY( char *titulo, int x, int y, int digitos )
/*****************************************************************************/
{
    long numero;
    int l = 0, temp = 0, xpedir=0;
    ExecuteSSAction( A_PEDIR, 1, titulo, " ", NULL, " " );
    numero = PEDIR_GRAN_ENTERO( xpedir, y, digitos );
    ExecuteAction( A_FIN_PEDIR );
    return ( numero );
}
/*****************************************************************************/
void MUESTRA_ERROR_EN_RED( void )
/*****************************************************************************/
{

	if( prevOffline != bandera_offline ) {
		ExecuteSSAction( A_MUESTRA_FLAGS,
						 ( bandera_offline ? T_FLAGS_ERROR_RED_AGREGAR : T_FLAGS_ERROR_RED_BORRAR ), " ",
						 "offline", NULL, " " );
		ExecuteAction( bandera_offline? A_MUESTRA_PANTALLA_OFFLINE : A_OCULTA_PANTALLA_OFFLINE );
	}
	prevOffline = bandera_offline;

}

/*****************************************************************************/
void MOSTRAR_MODO_DEVOLUCION()
{
    /*****************************************************************************/
    #if defined(INVEL_W) || defined(INVEL_L)
    if( !RAM_NOTA_CR && RAM_MODO_DEVOLUCION && !devolucion_on ) {
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_DEVOLUCION_AGREGAR, " ", "DEVOLUCION", NULL, " " );
        devolucion_on = 1;
    }
    else if( !RAM_NOTA_CR && !RAM_MODO_DEVOLUCION && devolucion_on ) {
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_DEVOLUCION_BORRAR, " ", " ", NULL, " " );
        devolucion_on = 0;
    }
    #else
    int attr;
    attr = _ATRIBUTO;
    if( RAM_MODO_DEVOLUCION || MODO_DEVOLUCION ) {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 40 - ancho_panel / 2, 24, ST( S__MODO_DEVOLUCION_ ) );
    }
    else {
        _ATRIBUTO = 7;
        MOSTRAR_CADENA( 40 - ancho_panel / 2, 24, "                 " );
    }
    _ATRIBUTO = attr;
    #endif  
}
/*****************************************************************************/
void SET_BORRAR_RECUADRO_DE_PETICION( int borrar )
/*****************************************************************************/
{
    borrar_recuadro_de_peticion = borrar;
}
/*****************************************************************************/
int PEDIR_CADENA_XY( char *titulo, int x, int y, int digitos, char *cadena )
/*****************************************************************************/
{
    int rta;
    int xder;
    int xizq;
    int yarr;
    int yaba;
    int l = 0, temp = 0, xpedir;
    xder = ( x ) + ( digitos * 3 + 1 ) / 2 + 1;
    xizq = ( x ) - ( digitos * 3 + 1 ) / 2 - 1;
    yarr = y - 1;
    yaba = y + 2;
    xpedir = xizq + 2;
    if( titulo ) {
        l = strlen( titulo );
    }
    if( l > xder - xizq ) {
        temp = ( l - ( xder - xizq ) ) / 2;
        xder += temp;
        xizq += temp;
    }
    ExecuteSSAction( A_PEDIR, 1, titulo, " ", NULL, " " );
    TECLADO_ALFABETICO = SI;
    rta = PEDIR_GRAN_CADENA( ( char )xpedir, ( char )y, ( char )digitos, cadena, NULL );
    TECLADO_ALFABETICO = NO;
    ExecuteAction( A_FIN_PEDIR );
    return ( rta );
}
/*****************************************************************************/
void MUESTRA_ET_TOMRA()
/*****************************************************************************/
{
    int i = 0;
    _ATRIBUTO = 120;
    if( MODALIDAD_TOMRA == _POR_TIPO ) {
        if( _RAM_NUMERO_TICKET_TOMRA( 0 ) > 0 ) {
            MOSTRAR_CADENA( 69 - ancho_panel, 22, ST( S_ET_TOMRA__ ) );
            MOSTRAR_ENTERO( 79 - ancho_panel, 22, "99", _RAM_NUMERO_TICKET_TOMRA( 0 ) );
        }
    }
    else {
        while( _RAM_NUMERO_TICKET_TOMRA( i ) >0 && i < 5 ) {
            i++;
        }
        MOSTRAR_CADENA( 69 - ancho_panel, 22, ST( S_ET_TOMRA__ ) );
        MOSTRAR_ENTERO( 79 - ancho_panel, 22, "9", i );
    }
}
#if defined(INVEL_W) || defined(INVEL_L)
/*****************************************************************************/
void GUI_MUESTRA_TARJETA( int operacion )
/*****************************************************************************/
{
    char *cmoneda[] = {"$","D","C"}, cadena[50], aux[30];
    memset( cadena, 0, strlen( cadena ) );
    if( !cobro_unipago && VAR_TARJETAS_STATUS ) {
        strncpy( cadena, ST( S__TARJETA_PARENTESIS ), sizeof( cadena ) );
        if( VAR_TARJETAS_STATUS == 2 ) {
            sprintf( aux, "%s%02i", cmoneda[MONEDA_EXTENDIDA()], VAR_TARJETAS_CUOTAS );
            strcat( cadena, aux );
            if( VAR_TARJETAS_IMPORTE_CUOTA ) {
                FTOC( VAR_TARJETAS_IMPORTE_CUOTA, aux, ENTEROS, DECIMALES, 0, 0 );
                strcat( cadena, " " );
                strcat( cadena, aux );
            }
            if( VAR_TARJETAS_NRO_CUPON_INT ) {
                sprintf( aux, " %02li", VAR_TARJETAS_NRO_CUPON_INT );
                strcat( cadena, aux );
            }
        }
        strcat( cadena, ") " );
    }
    ExecuteSSAction( A_MUESTRA_DATOS, operacion, cadena, " ", NULL, " " );
}
#endif
/*****************************************************************************
void MUESTRA_REFERENCIA_NC(  )
*****************************************************************************
{
    int       at;
   char      aux[10];
    at = _ATRIBUTO;
    if( RAM_REFERENCIA_NC ) {
        _ATRIBUTO = 120;
        MOSTRAR_CADENA( 30 - ancho_panel / 2, 24, ST( S__REF_ ) );
      _itoa( FLAG_NC_REF, aux, 10 );
      ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_COMP_VTAS_BORRAR, " ", " " );
       ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_COMP_VTAS_AGREGAR, aux, " " );
    } else {
        _ATRIBUTO = 7;
        MOSTRAR_CADENA( 30 - ancho_panel / 2, 24, "     " );
      ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_COMP_VTAS_BORRAR, " ", " " );
    }
    _ATRIBUTO = at;
}*/
/*****************************************************************************/
double PEDIR_FLOAT_EN_RECUADRO_CON_VALOR( char *titulo, int salvar_pantalla, int enteros,
                                          int decimales, double valor )
/*****************************************************************************/
{
    int pant = 0, digitos;
    ExecuteSSAction( A_PEDIR, 0, titulo, " ", NULL, " " );

    digitos = enteros + decimales + 1;
    DIBUJAR_RECUADRO_PETICION( titulo, digitos );
    valor = PEDIR_GRAN_FLOTANTE_CON_VALOR( ( 80 - ancho_panel - digitos * 3 + 1 ) / 2, 19, enteros,
                                           decimales, valor );

    ExecuteAction( A_FIN_PEDIR );
    return ( valor );
}
/*****************************************************************************/
double PEDIR_IMPORTE_XY( char *titulo, int x, int y, int enteros, int decimales )
/*****************************************************************************/
{
    double numero;
    int xpedir=0;
    ExecuteSSAction( A_PEDIR, 1, titulo, " ", NULL, " " );
    numero = PEDIR_GRAN_FLOTANTE( xpedir, y, enteros, decimales );
    ExecuteAction( A_FIN_PEDIR );
    return ( numero );
}
/*****************************************************************************/
void VERSION_DIALOG( void ) 
/*****************************************************************************/
{
    char versaux[70];
    sprintf( versaux, "&  %s   ( presione ENTER )", ver_posvisual );
    ExecuteSSAction( A_PEDIR, _HIBRIDO, "     VERSION", versaux, NULL, " " );
    while( GETCH() != 13 && !disable_standby_automatico );
	disable_standby_automatico = NO;
    ExecuteAction( A_FIN_PEDIR );
}
/*****************************************************************************/
void MOSTRAR_ANULACION( void )
/*****************************************************************************/
{
    if( RAM_ANULACION ){
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_DEV_ITEM_AGREGAR, " ", " ", NULL, " " );
    }
    else{
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_DEV_ITEM_BORRAR, " ", " ", NULL, " " );
    }
}

void SET_TARJERTAS_ON( int valor)
{
	tarjetas_on = valor; 
}