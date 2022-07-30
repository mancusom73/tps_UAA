#pragma pack (push,1) 
#ifndef CONFIG_H
#define CONFIG_H
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Configuracion para compilacion
 * ------------------------------------------------------------------------------
 */
#define ICONO_MEDIO
//#define BLINKER
//#define COMPILAR_CONSULTA_ARTICULO
#define COMPILAR_FISCAL
//#define COMPILAR_EMULADOR_FISCAL //para poder ejecutar el emulador con el javaserver
//#define COMPILAR_NET_COM
#define COMPILAR_DESCUENTOS
#define COMPILAR_PROMOCIONES
#define COMPILAR_TIPOS_DEVOLUCIONES
#define COMPILAR_PERFILES    // Para implementacin de Identificacin del Personal
#define COMPILAR_COMPROBANTES
//#define COMPILAR_GRABAR_CIERRE
#define COMPILAR_INFORME5
#define COMPILAR_INFORME15
#define COMPILAR_ON_LINE
#define COMPILAR_PRESENTACIONES
//#define COMPILAR_FACTURAS
#define COMPILAR_FORMATO_TICKET
#define COMPILAR_TECLA_REPROCESAR
#define COMPILAR_TECLA_CAMBIO
#define COMPILAR_MUTUALES
#define COMPILAR_ANULACION_TARJETA
//#define COMPILAR_CARGAUTOMATICAITEMS

//#define COMPILAR_VENTA_PENDIENTE
//#define COMPILAR_DYNAKEY          //Fuentes Panel y Vgagraf
//#define COMPILAR_GP416
//#define ENCRIPTADO
#define COMPILAR_MODO_ENTRENAMIENTO
//#define COMPILAR_SIN_USAR_IMPRESORA

//#define COMPILAR_OMRON
//#define COMPILAR_FACTURAR_REMITOS
//#define COMPILAR_EVENTOS_ALL
//#define COMPILAR_SCANNER_SPAR
#define COMPILAR_TECLA_MODO_CREDITO
//#define COMPILAR_SEGURO
//#define FECHA_DEMO
#ifdef FECHA_DEMO
	#define  FECHA_FINALIZACION_DIA  31
	#define  FECHA_FINALIZACION_MES  12
	#define  FECHA_FINALIZACION_ANIO 2011 
#endif
//#define COMPILAR_TEC
//#define COMPILAR_CMR
//#define COMPILAR_ASC
//#define COMPILAR_FLAGS
//#define COMPILAR_CUS
#define COMPILAR_PINPAD
//#define COMPILAR_PINPAD1
//#define COMPILAR_PINPAD3 //cosas de pinpad que son obsoletas no decomentar esta linea
//#define COMPILAR_HERCULES
//#define COMPILAR_MOZO
//#define COMPILAR_GEMPLUS
//#define COMPILAR_NOVIOS
//#define COMPILAR_PENDIENTES
#define COMPILAR_REMITOS
#define COMPILAR_BALANZA
#define VARIACION_FINANCIERA_SIN_RETENCION
//#define COMPILAR_RECUPERO
//#define DEBUG_IMPRESION
//#define COMPILAR_REVALIDAR_COBROS
#define COMPILAR_CLIENTE_AFINIDAD_ON_LINE
#define COMPILAR_COMPROMISO_200
#define COMPILAR_DISPLAY
#define COMPILAR_UML
#define COMPILAR_ENCRIPTACION//Funciones de encriptacion debe habilitarse para cualquiera de la encriptaciones
#define COMPILAR_ENCRIPTACION_TARJETAS//encripta los tracks 
#define COMPILA_DATOS_TARJETAS_PRIVADOS // pasa los datos privados de la var tarjeta a otra estructura
//#define COMPILA_TRACKS_TECLADOS // PARA EL	CASO QUE NO SE LEA BIEN LOS SEPARADORES DE CAMPOS DE LAS TARJETAS como me pasa

#ifdef COMPILAR_ENCRIPTACION_TARJETAS 
	#define ENCRIPTAR_DATOS_TARJETA 1
#else
	#define ENCRIPTAR_DATOS_TARJETA 0 
#endif


#define DEFINE_ENCRIPTA_TRANSACCIONES // encripta las transacciones que quedan en la transac2.sic
#define ALTA_RAPIDA_REINTEGROS
/******************************************/
#define  CANTIDAD_PANELES_BROWSE    33
#define  CANTIDAD_CARACTERISTICAS   30
#define  CANTIDAD_INPUTS            30
#define  DEBUG_IMPRESION
#if INVEL_L
#define INVEL_PACK __attribute__ ((packed))
#define S_FILEDELIMITER "/"
#define C_FILEDELIMITER '/'
#else
#define S_FILEDELIMITER "\\"
#define C_FILEDELIMITER '\\'
#define INVEL_PACK
int       CARGAR_ARCHIVO_DE_COMANDOS( char *nom_archivo, int *buffer, int ancho );
#endif

#endif
#pragma pack (pop) 
