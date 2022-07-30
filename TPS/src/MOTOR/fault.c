#pragma pack (1)
#if defined(INVEL_W) || defined(INVEL_L)
#include "cr.h"
#include "path.h"
#endif
#include "fault.h"
//#include "ftypes.h"
#include "logrecov.h"
#include "obuf.h"
#include "ini.h"
//#include "fbenef.h"
#include <string.h>

#include "etpv.h"

#include <assert.h>
#include "cmemory.h"
#pragma pack (1)


/*
 * Modo NORMAL: solo se registran en un log todos los eventos de la ejecucion actual.
 * Modo RECOVER: se recupera la ejecucion anterior que termino con falla.
 * en caso de haber terminado varias veces con falla, se continua desde la ejecucion
 * menos incompleta.
 * Ademas se registra en otro log la ejecucion que se realiza comenzando con la recuperacion
 * y continua luego en modo normal.
 */ 
typedef enum faultMode { undef, normal, recover, totalRecover, fatalError } faultMode;


static faultMode MODO = undef;

static FILE *normalLog = NULL; // = newLog(); 
// Log actual (uno nuevo) sobre el que se registran todos los eventos
// de la ejecucion actual (ya sea que se este ejecutando en modo normal,
// o en modo recuperacion de falla.

static FILE *recoverLog = NULL; // = openLog(); 
// Log desde el cual se recupera la ejecucion anterior finalizada
// con una falla.

eventQueue qs = NULL;

dataList ls = NULL;

obuf outbuffer = NULL;

benefList bs = NULL;

/*
 * Las siguientes variables se utilizan para indicar que cada una
 * de las estructuras de datos, ya se han vaciado (desde el modo
 * recover), para ayudar a detectar cuando se debe volver al modo
 * normal. 
 * (eventFinish && dataFinish && benefFinish ) => MODO = normal
 */
int eventFinish = 0; 
int dataFinish = 0; 
int benefFinish = 0; 

void updateMode( void )
{
    if( ( MODO == recover ) && eventFinish && benefFinish ) {
        if( dataListOpen( ls ) ) {
            dataFinish = 1;

            MODO = normal;
			recState = recoveredState;

            if( recoverLog != NULL ) {
                closeLog( recoverLog );
				recoverLog = NULL;
            }
        }
    }
}

int startFaultToleranceEngine()
{
    if( MODO != undef ) {
        return 1;
    } // se debe detener el motor antes de volver a iniciarlo.
    recoverLog = openLog();
    normalLog = newLog();
    if( recoverLog == NULL ) {
        MODO = normal;
        recState = normalState;
    }
    else {
        int r;
        MODO = recover;
        recState = recoveringState;
        //fprintf( stderr, "startFaultToleranceEngine: recobrando ESTADO perdido...\n" );
        /**/glog( "PROMO: startFaultToleranceEngine: recobrando ESTADO perdido...\n",LOG_MOTOR_PROMO,4);
        // Aqui se deben ejecutar todas las acciones indicadas en el log (i.e. agregar
        // articulos, eliminar articulos, pedir total, etc. etc. para reconstruir el
        // mismo estado al momento en que se produjo la falla.

        r = loadLog( recoverLog, &qs, &ls, &bs );
        if( ls != NULL ) {
            dataFinish = 0;
        }
        else {
            dataFinish = 1;
        }
        if( bs != NULL ) {
            benefFinish = 0;
        }
        else {
            benefFinish = 1;
        }
        if( !isEmptyQueue( qs ) ) {
            eventFinish = 0;
        }
        else {
            eventFinish = 1;
        }


        return r;
    }
    return 0;
}

int recoverEngine()
{
    int r;
    if( MODO == recover ) {
        recState = recoveringState;

		{
			BORRAR_ARCHIVO_REC(ARCH_REC_ACT_MEDIO_MOTOR); //borra todos los registros
			BORRAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO); //borra todos los registros
			BORRAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO); //borra todos los registros
			BORRAR_ARCHIVO_REC( ARCH_REC_REQUERIR_MEDIO ); //borra todos los registros
			glog( "PROMO: fault.c: Se ELIMINO el archivo de ACTIVACION DE MEDIOS\n" ,LOG_MOTOR_PROMO,4);
			glog( "PROMO: fault.c: Se ELIMINO el archivo de BONOS/MONEDERO/ETC.\n" ,LOG_MOTOR_PROMO,4);
			glog( "PROMO: fault.c: Se ELIMINO el archivo de INFORME DE EVENTOS.\n" ,LOG_MOTOR_PROMO,4);
			glog( "PROMO: extern.c: Se ELIMINO el archivo de REQUERIMIENTO DE MEDIOS DE PAGO.\n" ,LOG_MOTOR_PROMO,4);
		}

        r = recoverState( &qs );


        if( recoverLog != NULL ) {
            // Se cierra el log ya recuperado
            r = r | closeLog( recoverLog );
			recoverLog = NULL;
        }


        MODO = normal;
        recState = recoveredState;

        return r;
    }
    else {
        return 0;
    }
}

int stopFaultToleranceEngine()
{
    int r = 0;

    if( MODO == recover ) {
        // este caso es muy raro que ocurra.
        if( recoverLog != NULL ) {
            r = closeLog( recoverLog );
			recoverLog = NULL;
        }

        MODO = normal;
        recState = normalState;
    }

    if( recoverLog != NULL ) {
      r = closeLog( recoverLog );
	  recoverLog = NULL;
    }

    if( outbuffer ) {
        freeOBuf( outbuffer, normalLog );
        outbuffer = NULL;
    }

    if( MODO == normal ) {
        if( normalLog != NULL ) {
            r = endLog( normalLog ) || r ;
			normalLog = NULL;
        }
        MODO = undef;
        recState = normalState;
    }

    if( qs ) {
        /**/fprintf( stderr, "stopFaultToleranceEngine: liberando cola de eventos\n" );
        freeEventQueue( qs );
        qs = NULL;
    }

    if( ls ) {
        /**/fprintf( stderr, "stopFaultToleranceEngine: liberando lista de datos\n" );
        freeDataList( ls );
        ls = NULL;
    }

    if( bs ) {
        /**/fprintf( stderr, "stopFaultToleranceEngine: liberando lista de beneficios\n" );
        freeBenefList( bs );
        bs = NULL;
    }

    eventFinish = 0; 
    dataFinish = 0; 
    benefFinish = 0; 

    return r;
}

int restartFaultToleranceEngine()
{
    int r;
    r = stopFaultToleranceEngine();
    if( r ) {
        return r;
    }
    r = startFaultToleranceEngine();
    return r;
}

/*
 * Las funciones que llamen a readData NO deben hacer FREE,
 * ya que devuelve un puntero a un area de memoria interna,
 * que se libera automaticamente.
 */
void * readData( id v, char ind, void *r, size_t size )
{
    //  void * r = NULL;
    //  void * d;
    /*
    d = malloc (size);
    if (!d) {
      fprintf(stderr,"readData: error. falta de memoria\n");
      return  NULL;
    }
    */
    //fprintf(stderr,"readData: ###\n");

    if( MODO == recover ) {
        if( ind < 0 ) {
            r = dataListGetValue( &ls, v, r, size );
        }
        else {
            r = dataListGetSubValue( &ls, v, ind, r, size );
        }
        //fprintf(stderr,"readData: !!!\n");
        if( !r ) {
            fprintf( stderr, "readData: MODO RECOVER. error. no se pudo leer el identificador\n" );
            return NULL;
        }
        //fprintf(stderr,"readData: ---\n");
        //memcpy(d,r,size);
        //fprintf(stderr,"readData: @@@\n");
        oValue( &outbuffer, v, ind, r, size, normalLog );
        return r;
    }
    else if( MODO == normal ) {
        /* DEBUG */
        // esto debe reemplazarse por la rutina que devuelve en d el 
        // valor de la RNV(v)
        //r = malloc(size);
        if( ind < 0 ) {
            switch( DD_TIPO( v ) ) {
                    //#define  _SHORT_INT        1
                    //#define  _INT              2
                    //#define  _FLOAT            3
                    //#define  _DOUBLE         4
                    //#define  _STRING         5
                    //#define  _DATE           6
                    //#define  _HORA           7
                    //#define  _LONG           8
                    //#define  _UNSIGNED       9
                case _SHORT_INT:
                    //memcpy(r, GET_MEMORY_CHAR(v), size);
                    *( ( char* )r ) = GET_MEMORY_CHAR( v );
                    break;
                case _INT:
                    //memcpy(r, GET_MEMORY_INT(v), size);
                    *( ( int* )r ) = GET_MEMORY_INT( v );
                    break;
                case _FLOAT:
                    //memcpy(r, GET_MEMORY_FLOAT(v), size);
                    *( ( float* )r ) = GET_MEMORY_FLOAT( v );
                    break;
                case _DOUBLE:
                    //memcpy(r, GET_MEMORY_DOUBLE(v), size);
                    /*          { 
                                    double p;
                                    p = GET_MEMORY_DOUBLE(v);
                                    memcpy(r, &p, size);
                                }*/
                    *( ( double* )r ) = GET_MEMORY_DOUBLE( v );
                    break;
                case _STRING:
                    //memcpy(r, GET_MEMORY_STRING(v), size);
                    //memcpy(r, GET_MEMORY_CHAR(v), size);
                    *( ( char* )r ) = GET_MEMORY_CHAR( v );
                    break;
                case _DATE:
                    //memcpy(r, GET_MEMORY_DATE(v), size);
                    //memcpy(r, GET_MEMORY_CHAR(v), size);
                    *( ( char* )r ) = GET_MEMORY_CHAR( v );
                    break;
                case _HORA:
                    //memcpy(r, GET_MEMORY_HORA(v), size);
                    //memcpy(r, GET_MEMORY_CHAR(v), size);
                    *( ( char* )r ) = GET_MEMORY_CHAR( v );
                    break;
                case _LONG:
                    /*          {  // En realidad esta version es mas segura ya que solo copia size bytes
                                    long p = GET_MEMORY_LONG(v);
                                    memcpy(r, &p, size);
                                }
                                */
                    *( ( long* )r ) = GET_MEMORY_LONG( v );
                    break;
                case _UNSIGNED:
                    //memcpy(r, GET_MEMORY_UNSIGNED(v), size);
                    *( ( unsigned * )r ) = GET_MEMORY_UNSIGNED( v );
                    break;
                    //#define  _SHORT_INT        1
                    //#define  _INT              2
                    //#define  _FLOAT            3
                    //#define  _DOUBLE         4
                    //#define  _STRING         5
                    //#define  _DATE           6
                    //#define  _HORA           7
                    //#define  _LONG           8
                    //#define  _UNSIGNED       9
                default:
                    r = NULL;
                    break;
            }
        }
        else {
            /* Caso: Se trata de un arreglo */
        }

        //    fprintf(stderr,"readData: Ingrese un valor para la RNV(%i): ",v);
        //    scanf("%lf",(number*)r);
        /* ----- */

        //memcpy(d,r,size);
        oValue( &outbuffer, v, ind, r, size, normalLog );    
        return r;
    }
    else {
        fprintf( stderr,
                 "readData: error, lectura sin inicializar el motor de tolerancia a fallas.\n" );
        return NULL;
    }
}

void writeData( id v, char ind, void *data, size_t size )
{
    /**/fprintf( stderr, "writeData: escribiendo valor para el identificador %i.\n", v );
}


/*
number readNumRnv(id v, char ind) {
  //  number r;
  number d;
  
  updateMode();

  //fprintf(stderr,"readNumRnv: %i[%i]\n",v,ind);
  if (!readData(v,ind,&d,sizeof(number))) {
    //fprintf(stderr,"readNumRnv: error leyendo variable\n");
    return 0;
  } else {
    //fprintf(stderr,"readNumRnv: TODO BIEN\n");
    return d;
  }
}
*/


/*
 * Devuelve 1 en caso de exito. 0 c.c.
 */

int readNumRnv( void *r, id v, char ind )
{
    //int DD_LARGO( int var )
    //int DD_ELEMENTOS( int var )

    updateMode();

    //fprintf(stderr,"readNumRnv: %i[%i]\n",v,ind);

    if( ind < DD_ELEMENTOS( v ) ) {
        if( !readData( v, ind, r, DD_LARGO( v ) ) ) {
            //fprintf(stderr,"readNumRnv: error leyendo variable\n");
			glog( "readNumRnv: readData: ERROR. Indice de arreglo incorrecto" ,LOG_VARIOS,2);
            return 0;
        }
        else {
            //fprintf(stderr,"readNumRnv: TODO BIEN\n");
            return 1;
        }
    }
    else {
        glog( "readNumRnv: ERROR. Indice de arreglo incorrecto",LOG_VARIOS,2);
        return 0;
    }
}


void writeNumRnv( id v, char ind, number e )
{
    writeData( v, ind, &e, sizeof( number ) );
}


/*
 * 1 => TRUE, 0 => FALSE, -1 => ERROR (input no registrado en log) 
 */
int readBoolInput( void )
{
    //////////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    void *r = NULL;
    int b = 0;

    updateMode();

    if( MODO == recover ) {
        r = dataListGetValue( &ls, boolInput, &b, sizeof( int ) );
        if( !r ) {
            fprintf( stderr, "readData: MODO RECOVER. error. no se pudo leer el identificador\n" );
            return -1;
        }
        //putBoolInput(r,normalLog);
        return ( b ? 1 : 0 );
    }
    else if( MODO == normal ) {
        return -1;
    }
    else {
        fprintf( stderr,
                 "readData: error, lectura sin inicializar el motor de tolerancia a fallas.\n" );
        return -1;
    }
}


/*
 * 0 => OK, -1 => ERROR (input no registrado en log) 
 */
int readNumInput( double *rta )
{
    //////////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    void *r = NULL;
    //int b = 0;

    updateMode();

    if( MODO == recover ) {
        r = dataListGetValue( &ls, numInput, rta, sizeof( double ) );
        if( !r ) {
            fprintf( stderr,
                     "readNumInput: MODO RECOVER. error. no se pudo leer el identificador\n" );
            return -1;
        }
        //putBoolInput(r,normalLog);

        return 0;
    }
    else if( MODO == normal ) {
        return -1;
    }
    else {
        fprintf(
        stderr, "readNumInput: error, lectura sin inicializar el motor de tolerancia a fallas.\n" );
        return -1;
    }
}


/*
 * 0 => OK, -1 => ERROR (Time no registrado en log) 
 */
int readTime( long *rta )
{
    //////////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    void *r = NULL;
    //int b = 0;

    updateMode();

    if( MODO == recover ) {
        r = dataListGetValue( &ls, idTime, rta, sizeof( long ) );
        if( !r ) {
            fprintf( stderr, "readTime: MODO RECOVER. error. no se pudo leer el identificador\n" );
            return -1;
        }
        //putBoolInput(r,normalLog);

        return 0;
    }
    else if( MODO == normal ) {
        return -1;
    }
    else {
        fprintf( stderr,
                 "readTime: error, lectura sin inicializar el motor de tolerancia a fallas.\n" );
        return -1;
    }
}

/*
 * 0 => OK, -1 => ERROR (Time no registrado en log) 
 */
int readDate( long *rta )
{
    //////////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    void *r = NULL;
    //int b = 0;

    updateMode();

    if( MODO == recover ) {
        r = dataListGetValue( &ls, idDate, rta, sizeof( long ) );
        if( !r ) {
            fprintf( stderr, "readDate: MODO RECOVER. error. no se pudo leer el identificador\n" );
            return -1;
        }
        //putBoolInput(r,normalLog);

        return 0;
    }
    else if( MODO == normal ) {
        return -1;
    }
    else {
        fprintf( stderr,
                 "readDate: error, lectura sin inicializar el motor de tolerancia a fallas.\n" );
        return -1;
    }
}

/*
 * 0 => OK, -1 => ERROR (Time no registrado en log) 
 */
int readDay( long *rta )
{
    //////////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    void *r = NULL;
    //int b = 0;

    updateMode();

    if( MODO == recover ) {
        r = dataListGetValue( &ls, idDay, rta, sizeof( long ) );
        if( !r ) {
            fprintf( stderr, "readDay: MODO RECOVER. error. no se pudo leer el identificador\n" );
            return -1;
        }
        //putBoolInput(r,normalLog);

        return 0;
    }
    else if( MODO == normal ) {
        return -1;
    }
    else {
        fprintf( stderr,
                 "readDay: error, lectura sin inicializar el motor de tolerancia a fallas.\n" );
        return -1;
    }
}



/*
 * Se registra un indice para la evaluacion de expr purchase()
 *
int readPurchase(void) {
//////////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void * r = NULL;
int b = 0;

  updateMode();

  if (MODO == recover) {
      r = dataListGetValue(&ls, idPurchase, &b, sizeof(int));
    if (!r) {
      fprintf(stderr,"readData: MODO RECOVER. error. no se pudo leer el identificador\n");
      return -1;
    }
    //putBoolInput(r,normalLog);
    return b;

  } else if (MODO == normal) {

    return -1;

  } else {
    fprintf(stderr,"readPurchase: error, lectura sin inicializar el motor de tolerancia a fallas.\n");
    return -1;
  }
}

*/

int logAddArticle( identifier id, number count, long department, double precio, char promocionable, char porImporte )
{
    return putAddArticle( id, count, department, precio, promocionable, porImporte, normalLog );
}

int logRemoveArticle( identifier id, number count, long department, double precio, char promocionable, char porImporte )
{
    return putRemoveArticle( id, count, department, precio, promocionable, porImporte, normalLog );
}

int logUpdateArticle( identifier id, number count, long department, double precio, char promocionable, char porImporte )
{
    return putUpdateArticle( id, count, department, precio, promocionable, porImporte, normalLog );
}

int logAddExternalDiscount( identifier id, number count, long department, double precio, char promocionable, char porImporte )
{
    return putAddExternalDiscount( id, count, department, precio, promocionable, porImporte, normalLog );
}

int logCredit( identifier id, number count )
{
    updateMode();

    if( MODO == recover ) {
        if( benefListCompareFirst( bs, Credit, id, NULL, count, 0, NULL ) ) {
            bs = tailBenefList( bs );

            benefFinish = ( bs == NULL ); 
            // Inidica si se ha vaciado no o no la lista de beneficios,
            // para ayudar a detectar cuando se debe finalizar el modo
            // recupero de falla.

            putCredit( id, count, normalLog );
            return 0;
        }
        else {
            MODO = fatalError;
            glog( "logCredit: FATAL ERROR. no coinciden los beneficios", LOG_MOTOR_PROMO,3);
            //fprintf(stderr,"logCredit: FATAL ERROR. no coinciden los beneficios\n");

            /* >>> TODO: INICIAR RECUPERO TOTAL <<< */

            return -1;
        }
    }
    else if( MODO == normal ) {
        /* DEBUG */
        // esto debe reemplazarse por la rutina que hace efectivo el descuento
        //fprintf( stderr, "logCredit: CREDIT(%s,%lf)\n", id, count );
        /* ----- */

        putCredit( id, count, normalLog );
        return 1;
    }
    else {
        fprintf(
        stderr,
        "logCredir: error, falta inicializar el motor de tolerancia a fallas, o estado no contemplado.\n" );
        return -1;
    }
}


int logDistribute( identifier id, number count, number value )
{
    updateMode();

    if( MODO == recover ) {
        if( benefListCompareFirst( bs, Distribute, id, NULL, count, value, NULL ) ) {
            bs = tailBenefList( bs );

            benefFinish = ( bs == NULL ); 
            // Inidica si se ha vaciado no o no la lista de beneficios,
            // para ayudar a detectar cuando se debe finalizar el modo
            // recupero de falla.

            putDistribute( id, count, value, normalLog );
            return 0;
        }
        else {
            MODO = fatalError;
            glog( "logDistribute: FATAL ERROR. no coinciden los beneficios",LOG_MOTOR_PROMO,3);
            //fprintf(stderr,"logDistribute: FATAL ERROR. no coinciden los beneficios\n");

            /* >>> TODO: INICIAR RECUPERO TOTAL <<< */

            return -1;
        }
    }
    else if( MODO == normal ) {
        /* DEBUG */
        // esto debe reemplazarse por la rutina que hace efectivo el descuento
        fprintf( stderr, "logDistribute: DISTRIBUTE(%s,%lf,%lf)\n", id, count, value );
        /* ----- */

        putDistribute( id, count, value, normalLog );
        return 1;
    }
    else {
        fprintf(
        stderr,
        "logDistribute: error, falta inicializar el motor de tolerancia a fallas, o estado no contemplado.\n" );
        return -1;
    }
}


int logIssueBonus( identifier target, identifier type, number count, number value,
                   identifier label )
{
    updateMode();

    if( MODO == recover ) {
        if( benefListCompareFirst( bs, IssueBonus, target, type, count, value, label ) ) {
            bs = tailBenefList( bs );

            benefFinish = ( bs == NULL ); 
            // Inidica si se ha vaciado no o no la lista de beneficios,
            // para ayudar a detectar cuando se debe finalizar el modo
            // recupero de falla.

            putIssueBonus( target, type, count, value, label, normalLog );
            return 0;
        }
        else {
            MODO = fatalError;
            glog( "logIssueBonus: FATAL ERROR. no coinciden los beneficios" ,LOG_MOTOR_PROMO,3);
            //fprintf(stderr,"logIssueBonus: FATAL ERROR. no coinciden los beneficios\n");

            /* >>> TODO: INICIAR RECUPERO TOTAL <<< */

            return -1;
        }
    }
    else if( MODO == normal ) {
        /* DEBUG */
        // esto debe reemplazarse por la rutina que hace efectivo el descuento
        fprintf( stderr, "logIssueBonus: ISSUE_BONUS(%s,%s,%lf,%lf,%s)\n", target, type, count,
                 value, label );
        /* ----- */

        putIssueBonus( target, type, count, value, label, normalLog );
        return 1;
    }
    else {
        fprintf(
        stderr,
        "logIssueBonus: error, falta inicializar el motor de tolerancia a fallas, o estado no contemplado.\n" );
        return -1;
    }
}



int logGivePoints( identifier id, number count )
{
    updateMode();

    if( MODO == recover ) {
        if( benefListCompareFirst( bs, GivePoints, id, NULL, count, 0, NULL ) ) {
            bs = tailBenefList( bs );

            benefFinish = ( bs == NULL ); 
            // Inidica si se ha vaciado no o no la lista de beneficios,
            // para ayudar a detectar cuando se debe finalizar el modo
            // recupero de falla.

            putGivePoints( id, count, normalLog );
            return 0;
        }
        else {
            MODO = fatalError;
            glog( "logGivePoints: FATAL ERROR. no coinciden los beneficios" ,LOG_MOTOR_PROMO,3);
            //fprintf(stderr,"logGivePoints: FATAL ERROR. no coinciden los beneficios\n");

            /* >>> TODO: INICIAR RECUPERO TOTAL <<< */

            return -1;
        }
    }
    else if( MODO == normal ) {
        /* DEBUG */
        // esto debe reemplazarse por la rutina que hace efectivo el descuento
        fprintf( stderr, "logGivePoints: GIVE_POINTS(%s,%lf)\n", id, count );
        /* ----- */

        putGivePoints( id, count, normalLog );
        return 1;
    }
    else {
        fprintf(
        stderr,
        "logGivePoints: error, falta inicializar el motor de tolerancia a fallas, o estado no contemplado.\n" );
        return -1;
    }
}

/**
 * Se deber� implementar el grabado al log de recupero de fallas.
 * Por falta de tiempo, s�lo para la demo, se implementa la siguiente funcionalidad:
 * Si el motor de tolerancia a fallas se encuentra en modo 'recupero', entonces
 * NO se ejecutan los procedimientos externos, caso contrario, los ejecutar� normalmente.
 * Notar que no se est� haciendo ning�n tipo de chequeo de que coincidan las ejecuciones,
 * ni se estan registrando ni devolviendo los resultados de las ejecuciones de dichos 
 * procedimientos.
 */
int logEProc( identifier id, char* params )
{
    updateMode();

    if( MODO == recover ) {
        return 0;
    } else if( MODO == normal ) {
        return 1;
    } else {
		char buf[256];
		_snprintf(buf,255,"logEProc: error, falta inicializar el motor de tolerancia a fallas, o estado no contemplado [estado: %i]",(int)MODO);
		buf[255] = '\0';
        glog ( buf ,LOG_MOTOR_PROMO,3);
        return -1;
    }
}



int logPanelVentas( void )
{
    return putPanelVentas( normalLog );
}

int logPanelPagos( void )
{
    return putPanelPagos( normalLog );
}

int logPanelPosPago( void )
{
    return putPanelPosPago( normalLog );
}


int logBoolInput( int value )
{
    return putBoolInput( value, normalLog );
}

int logNumInput( double value )
{
    return putNumInput( value, normalLog );
}

int logTime( long value )
{
    return putTime( value, normalLog );
}

int logDate( long value )
{
    return putDate( value, normalLog );
}

int logDay( long value )
{
    return putDay( value, normalLog );
}


int logPurchase( int value )
{
    return putPurchase( value, normalLog );
}

int logCashdeskState( void *s, int size, char *str0, char *str1, char *str2, char *str3,
                      char *str4 )
{
    return putCashdeskState( s, size, str0, str1, str2, str3, str4, normalLog );
}
