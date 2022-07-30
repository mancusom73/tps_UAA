/*! \file init_pr.c
    \brief Módulo de inicialización de las estructuras necesarias para la utilización del Motor de Promociones.

  Este módulo provee los métods de inicialización y cierre de las estructuras
  necesarias para utilizar el motor de promociones.
  
  */

#include "etpv.h"
#if defined(INVEL_W) || defined(INVEL_L)
#include <cr.h>
#include <path.h>
#endif
#include "init_pr.h"
#include "interpr.h"
#include "fault.h"
#include "lib_req.h"
#include "lib_dreq.h"
#include "libcache.h"
#include <sys/stat.h>
#include <fcntl.h>
#include "mensaje.h"
#include "ini.h"

#define PROMO_CONSULTAS_SQL

#ifdef PROMO_CONSULTAS_SQL
#include "db.h"
#endif //PROMO_CONSULTAS_SQL

//#include <stdbool.h>

#pragma pack (1)

/** @defgroup groupLoadPromos Carga de Promociones
 *  @ingroup groupEval
 *  Este grupo de funciones aportan a la interfaz con el POS las funcionalidedes
 * de carga, reutilización y liberación de los programas de promociones que
 * el Motor de Promociones utilizará durante la evaluación. 
 *  @{
 */


/**
  Inidica si se deben recargar las promociones.
  Cambia el nombre de promo.cod a promo.wrk
  Ahora las promociones se deben leer del promo.wrk
*/

int existNewPromo ( void ) {
    struct stat attr;
    char ifname[256], ofname[256];

    sprintf( ifname, "%s%s", PATH_PROMOS, PROMO_COD );
    sprintf( ofname, "%s%s", PATH_PROMOS, PROMO_WRK );

    #ifdef __MSDOS__
    _fmode = O_BINARY;
    #endif
    #ifdef WIN32
    _fmode = O_BINARY;
    #endif

	if( recState == recoveringState ) {
		return 0;
	} // else  // recState == normalState

    if( stat( ifname, &attr ) == 0 ) {
      if( attr.st_size > 0 ) {
	if ( remove( ofname ) != 0 ) {
	  glog("PROMO: existNewPromo: error buscando nuevas promociones al borrar promo.wrk.",LOG_MOTOR_PROMO,3);
			//return 0;
	}
	if ( rename( ifname, ofname ) == 0 ) {
			return 1;
	} else {
	  glog("PROMO: existNewPromo: error buscando nuevas promociones al generar promo.wrk.",LOG_MOTOR_PROMO,3);
			return 0;	  
      }
	  }	else {
	glog("PROMO: existNewPromo: las nuevas promociones son vacias, se continua con las anteriores promociones.",LOG_MOTOR_PROMO,4);
		return 0;
      }
    } else {
      glog("PROMO: existNewPromo: error buscando nuevas promociones al leer promo.cod.",LOG_MOTOR_PROMO,4);
		return 0;
    }

}

/**
 Libera la memoria reservada para todas las estructuras necesarias
 para ejecutar el motor de promociones.
 */

int liberarEstructurasPromos ( void ) {

#ifdef PROMO_FAULT_TOLERANCE
  updateMode();
  stopFaultToleranceEngine();
  /**/glog( "PROMO: Motor de Tolerancia a Fallas Detenido." ,LOG_MOTOR_PROMO,4);
#endif //PROMO_FAULT_TOLERANCE

  
  if ( ss ) {
  /**/glog( "PROMO: Liberando estado global..." ,LOG_MOTOR_PROMO,4);
    ss = free_states( ss );
  }
  if ( ss_bak ) {
    /**/glog( "PROMO: Liberando backup del estado global..." ,LOG_MOTOR_PROMO,4);
    ss_bak = free_states( ss_bak );
  }
  if ( state_venta ) {
    /**/glog("PROMO: Liberando estados parciales utilizados para revertir promociones... [ventas]",LOG_MOTOR_PROMO,4);
    state_venta = free_recover_info( state_venta );
  }
  if ( state_pago ) {
    /**/glog("PROMO: Liberando estados parciales utilizados para revertir promociones... [pagos]" ,LOG_MOTOR_PROMO,4);
    state_pago = free_recover_info( state_pago );
  }

  //glog( "PROMO: Estado liberado." );
  
  /**/glog( "PROMO: Liberando AVLs" ,LOG_MOTOR_PROMO,5);
  destroy_articles_counts ( );
  /**/glog( "PROMO: AVL depart" ,LOG_MOTOR_PROMO,5);
  destroy_department_counts ( );
  /**/glog( "PROMO: AVL cache",LOG_MOTOR_PROMO,5);
  destroy_arts_cache ();
  /**/glog( "Liberando promociones..." ,LOG_MOTOR_PROMO,4);
  freeall();
  if ( gpr ) unref_pr( gpr );
  if ( pr ) unref_pr( pr );
  gpr = NULL;
  pr = NULL;
  /**/glog( "promos liberadas." ,LOG_MOTOR_PROMO,4);

  return 0;

}






/**
 Carga en memoria todas las estructuras para evaluar las promociones
 */
 
int load_promos( void ) {
  char strArch[256];
  
  /**/glog( "PROMO: Inicializando AVLs",LOG_MOTOR_PROMO,5);
  init_articles_counts ();
  /**/glog( "PROMO: AVL depart" ,LOG_MOTOR_PROMO,5);
  init_department_counts ();
  /**/glog( "PROMO: AVL cache" ,LOG_MOTOR_PROMO,5);
  init_arts_cache ();
  /**/glog( "PROMO: AVLs OK" ,LOG_MOTOR_PROMO,5);
  
  /**/glog( "PROMO: Leyendo configuraciones...",LOG_MOTOR_PROMO,5);
  sprintf( strArch, "%sglobals.conf", PATH_PROMOS );
  read_promo_config( strArch );
  
#ifdef PROMO_FAULT_TOLERANCE
  startFaultToleranceEngine();
  /**/glog( "PROMO: Motor de Tolerancia a Fallas Iniciado." ,LOG_MOTOR_PROMO,5);
#endif //PROMO_FAULT_TOLERANCE

  /**/glog( "PROMO: Cargando PROMOs ..." ,LOG_MOTOR_PROMO,4);
  sprintf( strArch, "%sglobals.cod", PATH_PROMOS );
  if ( compatible_version( strArch, &promo_ver ) ) {
    if ( existNewPromo( ) ) { 
      //Se renombran los archivos en caso de ser necesario.
    }
	pr = NULL;
    gpr = from_disk( strArch );
    sprintf( strArch, "%s%s", PATH_PROMOS, PROMO_WRK );
    if ( gpr && compatible_version( strArch, &promo_ver ) ) {
      pr = from_disk( strArch );
	  if (pr) {
		pr = concat( gpr, pr );
		gpr = NULL;
		/**/glog( "Promociones cargadas.",LOG_MOTOR_PROMO,4);
	  }
    }
	if ( !pr ) {
      char buff[256];
      sprintf(buff, "PROMO: Error cargando promociones: Archivo %s generado con una version no compatible del compilador de promociones, o archivo dañado.\nVersion requeridad: pc.exe [version %i.%i.%i.%i]",
	      strArch, promo_ver.macro, promo_ver.version, promo_ver.subversion, promo_ver.build);
      glog(buff,LOG_MOTOR_PROMO,3);
      MENSAJE( "Error cargando promociones. No se aplicaran promociones." );
      
      liberarEstructurasPromos ( );
      return -1;
    }
  } else {
    char buff[256];
    sprintf(buff, "PROMO: Error cargando promociones: Archivo %s generado con una version no compatible del compilador de promociones.\nVersion requeridad: pc.exe [version %i.%i.%i.%i]",
	    strArch, promo_ver.macro, promo_ver.version, promo_ver.subversion, promo_ver.build);
    glog(buff,LOG_MOTOR_PROMO,3);
    MENSAJE( "Error cargando promociones. No se aplicaran promociones." );
    
    liberarEstructurasPromos( );
    return -1;
  }
  
//#ifdef PROMO_FAULT_TOLERANCE
//  startFaultToleranceEngine();
//  /**/glog( "PROMO: Motor de Tolerancia a Fallas Iniciado." ,LOG_MOTOR_PROMO,2);
//#endif //PROMO_FAULT_TOLERANCE
  
  
  /**/glog( "PROMO: Inicializando Parametros globales ...",LOG_MOTOR_PROMO,4);
  if ( eval( INIT_PARAMS, pr, &ss ) != NORM ) {
    glog( "PROMO: ERROR Inicializando Parametros. No se evaluaran promociones." ,LOG_MOTOR_PROMO,3);
    MENSAJE( "Error inicializando promociones. No se aplicaran promociones.");

    liberarEstructurasPromos( );
    return -1;
  } else {
    /**/glog( "PROMO: Parametros inicializados. Clonando estado para reutilizarlo luego." ,LOG_MOTOR_PROMO,4);
    ss_bak = states_clone( ss );
  }

  /**/glog( "PROMO: Parametros inicializados. Evaluando PROMOs EVER ..." ,LOG_MOTOR_PROMO,4);
  if ( eval( EVER, pr, &ss ) != NORM ) {
    glog( "PROMO: ERROR Evaluando promos EVER. No se evaluaran promociones.",LOG_MOTOR_PROMO,3);
    MENSAJE( "Error inicializando promociones. No se aplicaran promociones." );
    
    liberarEstructurasPromos( );
    return -1;
  } else {
    /**/glog( "PROMO: PROMOs Evaluadas correctamente." ,LOG_MOTOR_PROMO,4);
    
#ifdef PROMO_FAULT_TOLERANCE
    recoverEngine();
#endif //PROMO_FAULT_TOLERANCE

#ifdef PROMO_CONSULTAS_SQL
	{
		char sc[256];
		struct stat attr;

		_snprintf( sc, 256, "%s%s", PATH_PROMOS, PROMO_SQL );
		sc[255] = '\0';

		if( !stat( sc, &attr ) && attr.st_size > 0 ) {
			glog("PROMO: load_promos: Ejecutando script SQL para consulta de promociones.",LOG_MOTOR_PROMO,4);
			EJECUTAR_SCRIPT( sc, PATH_PROMOS );
		} else {
			glog("PROMO: load_promos: Error inicializando promociones. No se encontr script sql.",LOG_MOTOR_PROMO,3);
		}
	}
#endif // PROMO_CONSULTAS_SQL
  }
  return 0;
}



// Funciones de inicializacin del motor de promocinoes.


/**
  Reinicia las promociones, estados de variables globales y est�ticas, motor de tolerancia a fallas, 
 y todas las estructuras para evaluar promociones. 
 NO SE LIBERAN LAS PROMOCIONES.
 */
int reinit_promos( void ) {


    /* REINICIAR PROMOCIONES */
    #ifdef PROMO_FAULT_TOLERANCE
    restartFaultToleranceEngine();
    /**/glog( "PROMO: Motor de Tolerancia a Fallas ReInicializado." ,LOG_MOTOR_PROMO,4);
    #endif //PROMO_FAULT_TOLERANCE

    /**/glog( "PROMO: Liberando estado global..." ,LOG_MOTOR_PROMO,4);
    ss = free_states( ss );
    /**/glog("PROMO: Liberando estados parciales utilizados para revertir promociones..." ,LOG_MOTOR_PROMO,4);
    state_venta = free_recover_info( state_venta );
    state_pago = free_recover_info( state_pago );
    /**/glog( "PROMO: Estado liberado." ,LOG_MOTOR_PROMO,4);


    if ( existNewPromo( ) ) {
      liberarEstructurasPromos( );
      return load_promos( );

    } else {
      /**/glog( "PROMO: ReInicializando AVLs" ,LOG_MOTOR_PROMO,4);
      restart_articles_counts ( );
      restart_department_counts ( );
      /**/glog( "PROMO: Liberando AVL cache" ,LOG_MOTOR_PROMO,4);
      destroy_arts_cache ();
      /**/glog( "PROMO: Inicializando AVL cache",LOG_MOTOR_PROMO,4);
      init_arts_cache ();

      if ( ss_bak ) {
	/**/glog( "PROMO: Recuperando Parametros globales previamente inicializados y clonados..." ,LOG_MOTOR_PROMO,4);
	ss = states_clone ( ss_bak );
      } else {
	/**/glog( "PROMO: Error, no se encontro backup del estado." ,LOG_MOTOR_PROMO,3);
	/**/glog( "PROMO: Se liberan estructuras de promociones." ,LOG_MOTOR_PROMO,4);
	liberarEstructurasPromos( );
	/**/glog( "PROMO: Se cargan nuevamente las promociones." ,LOG_MOTOR_PROMO,4);
	return load_promos( );
      }


      /**/glog( "PROMO: Evaluando PROMOs EVER ..." ,LOG_MOTOR_PROMO,4);
      if ( eval( EVER, pr, &ss ) != NORM ) {
	MENSAJE( "Error inicializando promociones EVER. No se aplicaran promociones." );
	liberarEstructurasPromos ( );
	return -1;
      } else {
	/**/glog( "PROMO: PROMOs Evaluadas correctamente." ,LOG_MOTOR_PROMO,4);

#ifdef PROMO_FAULT_TOLERANCE
	recoverEngine();
#endif //PROMO_FAULT_TOLERANCE
      }
      
      
    }

    return 0;
}



/** @} */

