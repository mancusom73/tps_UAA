//#ifdef MODULO_PROMOCIONES

//#define PROMO_STR

#define STRLEN_DOUBLE 20
//define la cantidad de caracteres que se utilizan para imprir un double como un string.
#define STRLEN_LONG 20
#if defined(INVEL_W) || defined(INVEL_L)
#include "cr.h"
#endif
#include "state.h"
#include "staticSt.h"
#include "valstack.h"
#include "comstack.h"
#include "extern.h"
#include "eval.h"
#include "types2.h"
#include "ini.h"
#include "time2.h"
//#include "uprinter.h"

#ifdef PROMO_FAULT_TOLERANCE
#include "fault.h"
#endif //PROMO_FAULT_TOLERANCE

#include "etpv.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "lib_req.h"
#include "lib_dreq.h"
#include "libcache.h"

#pragma pack (1)

/*
 * TOLERANCE establece la cantidad de digitos decimales que
 * se utilizaran para hacer el redondeo con el operador %
 */
#define TOLERANCE 0

#ifdef __MSDOS__
#define atoll(A) atol(A)
#endif


struct states
{
    state s;
    staticState st;
};

struct recover_info
{
    states promo_state;
    cashdesk_state cashdesk_state;
};

states free_states( states ss );

void freeall( void );

void p_error( char * );

double round2( double, int );
state s = NULL;
vstack vs = NULL;
cstack cs = NULL;
expr purch = NULL;
expr wpurch = NULL;
expr added = NULL;
expr means_set;
long promo_id = 0;
char *promo_id_extern = NULL;
long rec_group = 0;
int reqMeansGroup = 0;

int evaluatingPreConditions = 0;

/** @defgroup groupEval Evaluación de Promociones
 *  Este grupo de funciones representan la interfaz mediante la cual el POS le indicará
 * al Motor de Promociones las acciones necesarias que el mismo debe hacer para
 * que se evalúen las promociones correspondientes a cada estado en el que se encuentra
 * el POS.
 *  @{
 */

evalres eval( evalTime et, promo ps, states *ss )
{
    // promo ps a evaluar en el estado de vars globales ss
    expr e1,e2,e3,e4;
    benef b1;
    param pa1;
    promo pr1,pr2;
    identifier ident, promo_name = NULL;
    list l1,l2;
    mlist ml1,ml2,ml3,ml4;
    part ps1,ps2;
    int index1,index2;

	if (!ss) return ABNORM;

    if( *ss == NULL ) {
        *ss = ( states )malloc( sizeof( struct states ) );
        ( *ss )->s = NULL;
        ( *ss )->st = NULL;
    }

    if( ps == NULL ) {
        return NORM;
    }

	if ( et == PREPAGO) {
		reqMeansGroup = 0;
	}

    //  purch = ref_e(purchase());
	if ( et == POSTPAGO) {
		means_set = ref_e( means() );
	} else {
		means_set = ref_e( new_means() );
	}

	if ( et == INIT_PARAMS) {
	  rec_group = 0;
	  reqMeansGroup = 0;
	} 

    if( added == NULL ) {
        added = ref_e( new_amset() );
    }

    vs = vs_push_pr( vs, ps );
    cs = cs_push( cs, EvPROG );

    while( cs != NULL ) {
        switch( cs_pop( &cs ) ) {
            case EvPROG:
                pr1 = vs_pop_pr( &vs );
                pr2 = pr1;
                while( pr2 != NULL ) {
                    if( ( pr2->evalTime == et )
                     || ( ( pr2->evalTime == ONLINE ) && ( et == CANCELLATION ) )
                     || ( ( pr2->evalTime == EVER )/* && (et != CANCELLATION)*/ )
                     || ( et == INIT_PARAMS ) ) {
                        // CUIDADO: Las promociones EVER deben tener CANCELACION !!!
                        if( pr2->nextPromo != NULL ) {
                            vs = vs_push_pr( vs, pr2->nextPromo );
                            cs = cs_push( cs, EvPROG );
                        }
                        vs = vs_push_pr( vs, pr2 );
                        cs = cs_push( cs, FREE );
                        cs = cs_push( cs, EvPROMO );
                        break;
                    }
                    else {
                        pr2 = pr2->nextPromo;
                    }
                }

                unref_pr( pr1 );
                break;

            case EvPROMO:
                pr1 = vs_pop_pr( &vs );

                /////////////// seguir trabajando aca
                //>>if (promo_name != NULL) {
                //>>free(promo_name);
                //>>}
                //>>promo_name = (identifier)malloc(strlen(pr1->name)+1);

                promo_name = pr1->name;

                if( et == CANCELLATION ) {
                    pr2 = pr1->cancellation;
                    vs = vs_push_l( vs, pr1->globals );
                    vs = vs_push_l( vs, pr1->externs );
                    vs = vs_push_l( vs, pr1->statics );
                    cs = cs_push( cs, PUSH_GLOBAL_VARS ); // actualiza vars globales externas y estaticas de la promo
                    vs = vs_push_l( vs, pr2->globals );
                    vs = vs_push_l( vs, pr2->externs );
                    vs = vs_push_l( vs, pr2->statics );
                    cs = cs_push( cs, PUSH_GLOBAL_VARS ); // actualiza vars globales externas y estaticas de la cancelacion de la promo

                    vs = vs_push_b( vs, pr2->benefits );
                    cs = cs_push( cs, IF_CONDS_BENEF );

                    vs = vs_push_l( vs, pr2->conditions );
                    cs = cs_push( cs, EvCONDS );

                    vs = vs_push_pa( vs, pr2->parameters );
                    cs = cs_push( cs, EvPARAMS );
                    vs = vs_push_pa( vs, pr1->parameters );
                    cs = cs_push( cs, EvPARAMS );

                    vs = vs_push_l( vs, pr2->externs );
                    cs = cs_push( cs, POP_EXTERN_VARS );
                    vs = vs_push_l( vs, pr1->externs );
                    cs = cs_push( cs, POP_EXTERN_VARS );
                }
                else if( et == INIT_PARAMS ) {
                    // Este modo de evaluacion se utiliza para inicializar todos
                    // los parametros globales y estaticos.

                    if( pr1->cancellation != NULL ) {
                        // Tambien los paramtros de las promos cancelacion
                        vs = vs_push_l( vs, pr1->cancellation->globals );
                        vs = vs_push_l( vs, pr1->cancellation->externs );
                        vs = vs_push_l( vs, pr1->cancellation->statics );
                        cs = cs_push( cs, PUSH_GLOBAL_VARS );

                        vs = vs_push_pa( vs, pr1->cancellation->parameters );
                        cs = cs_push( cs, EvPARAMS );

                        vs = vs_push_l( vs, pr1->cancellation->externs );
                        cs = cs_push( cs, POP_EXTERN_VARS );
                    }

                    vs = vs_push_l( vs, pr1->globals );
                    vs = vs_push_l( vs, pr1->externs );
                    vs = vs_push_l( vs, pr1->statics );
                    cs = cs_push( cs, PUSH_GLOBAL_VARS );

                    vs = vs_push_pa( vs, pr1->parameters );
                    cs = cs_push( cs, EvPARAMS );

                    vs = vs_push_l( vs, pr1->externs );
                    cs = cs_push( cs, POP_EXTERN_VARS );
                }
                else {
		  /* -- 13-02-07 - Se posterga hasta que se hayan evaluado las PreCondiciones 
                    vs = vs_push_l( vs, pr1->globals );
                    vs = vs_push_l( vs, pr1->externs );
                    vs = vs_push_l( vs, pr1->statics );
                    cs = cs_push( cs, PUSH_GLOBAL_VARS ); // actualiza vars globales externas y estaticas

                    vs = vs_push_b( vs, pr1->benefits );
                    cs = cs_push( cs, IF_CONDS_BENEF );

                    vs = vs_push_l( vs, pr1->conditions );
                    cs = cs_push( cs, EvCONDS );

                    vs = vs_push_pa( vs, pr1->parameters );
                    cs = cs_push( cs, EvPARAMS );

                    vs = vs_push_l( vs, pr1->externs );
                    cs = cs_push( cs, POP_EXTERN_VARS );
		  */

		    vs = vs_push_pr( vs, pr1 );
                    cs = cs_push( cs, IF_PRECONDS_PROMO );
                    vs = vs_push_l( vs, pr1->preconditions );
                    cs = cs_push( cs, EvPRECONDS );

                }

                unref_pr( pr1 );
                break;

            case IF_PRECONDS_PROMO:
	      e1 = vs_pop_e( &vs );
	      pr1 = vs_pop_pr( &vs );
	      if( e1->tag == TRUE ) {
		vs = vs_push_l( vs, pr1->globals );
		vs = vs_push_l( vs, pr1->externs );
		vs = vs_push_l( vs, pr1->statics );
		cs = cs_push( cs, PUSH_GLOBAL_VARS ); // actualiza vars globales externas y estaticas
		
		vs = vs_push_b( vs, pr1->benefits );
		cs = cs_push( cs, IF_CONDS_BENEF );
		
		vs = vs_push_l( vs, pr1->conditions );
		cs = cs_push( cs, EvCONDS );
		
		vs = vs_push_pa( vs, pr1->parameters );
		cs = cs_push( cs, EvPARAMS );
		
		vs = vs_push_l( vs, pr1->externs );
		cs = cs_push( cs, POP_EXTERN_VARS );		
	      }
	      else {
		
	      }
	      unref_pr( pr1 );
	      unref_e( e1 );
	      break;


            case IF_CONDS_BENEF:
                e1 = vs_pop_e( &vs );
                if( e1->tag == TRUE ) {
                    cs = cs_push( cs, EvBENEF );
                }
                else {
                    b1 = vs_pop_b( &vs );
                    unref_b( b1 );
                }
                unref_e( e1 );
                break;

            case EvPARAMS:
                pa1 = vs_pop_pa( &vs );
                if( pa1 != NULL ) {
                    vs = vs_push_pa( vs, pa1->nextParams );
                    cs = cs_push( cs, EvPARAMS );

                    vs = vs_push_pa( vs, pa1 );
                    cs = cs_push( cs, EvPARAM );
                    unref_pa( pa1 );
                }
                break;

            case EvPARAM:
                pa1 = vs_pop_pa( &vs );

                if( pa1->tag == GLOBAL ) {
                    e1 = value( ( *ss )->s, pa1->var );
                    if( e1 == NULL ) {
                        /* 16-02-06 Aqui busco la referencia a la expresion
                             que define a purchase y wholepurchase segun
                             se definen en globals.pro
                          */
                        if( strcmp( "purchase", pa1->var ) == 0 ) {
                            //up_expr(stderr,pa1->e,0);
                            if( purch ) {
                                unref_e( purch );
                            }
                            purch = ref_e( pa1->e );
                        }
                        if( strcmp( "wholepurchase", pa1->var ) == 0 ) {
                            //up_expr(stderr,pa1->e,0);
                            if( wpurch ) {
                                unref_e( wpurch );
                            }
                            wpurch = ref_e( pa1->e );
                        }
                        /* ---- */
                        vs = vs_push_id( vs, pa1->var );
                        vs = vs_push_e( vs, pa1->e );
                        cs = cs_push( cs, EvUPDATE );
                        cs = cs_push( cs, EvEXPR );
                    }
                    else {
                        // recuperar el valor global
                        s = update( s, pa1->var, e1 );
                    }
                }
                else if( pa1->tag == STATIC ) {
                    e1 = static_value( ( *ss )->st, promo_name, pa1->var );
                    if( e1 == NULL ) {
                        vs = vs_push_id( vs, pa1->var );
                        vs = vs_push_e( vs, pa1->e );
                        cs = cs_push( cs, EvUPDATE );
                        cs = cs_push( cs, EvEXPR );
                    }
                    else {
                        // recuperar el valor estatico
                        s = update( s, pa1->var, e1 );
                    }
                }
                else {
                    // LOCAL
                    vs = vs_push_id( vs, pa1->var );
                    vs = vs_push_e( vs, pa1->e );
                    cs = cs_push( cs, EvUPDATE );
                    cs = cs_push( cs, EvEXPR );
                }

                unref_pa( pa1 );
                break;

            case EvUPDATE:
                e1 = vs_pop_e( &vs );
                ident = vs_pop_id( &vs );
                s = update( s, ident, e1 );
                unref_e( e1 );
                free( ident );
                break;

            case EvBONUS_UPDATE:
                e4 = vs_pop_e( &vs );
                e3 = vs_pop_e( &vs );
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
				//28-09-07
                //unref_e( e1->e.bonus->e0 );
				//e1->e.bonus->e0 = ref_e( e2 ); // Se reemplaza esta línea por el siguiente bloque.
				//Ahora, es vez de modificar el árbol sintáctico, se crea un nuevo bono como resultado
				//de la evaluación y se lo deja en la pila. El bono del árbol queda intácto para
				//las próximas evaluaciones.
				{
					expr e5;
					e5 = ( expr )malloc( SIZE_OF_EXPR_BONUS );
					e5->refs = 0;
					e5->tag = BONUS;
					e5->e.bonus = ( struct bonus * )malloc( sizeof( struct bonus ) );
					/* target */
					e5->e.bonus->target = (char*)malloc(strlen(e1->e.bonus->target)+1);
					strcpy(e5->e.bonus->target,e1->e.bonus->target);
					/* type */
					e5->e.bonus->type = (char*)malloc(strlen(e1->e.bonus->type)+1);
					strcpy(e5->e.bonus->type,e1->e.bonus->type);
					/* leyenda */
					e5->e.bonus->leyenda = (char*)malloc(strlen(e1->e.bonus->leyenda)+1);
					strcpy(e5->e.bonus->leyenda,e1->e.bonus->leyenda);
					/* vigencia */
					e5->e.bonus->maturity = ref_e( e4 );
					/* validez */
					e5->e.bonus->validity = ref_e( e3 );
					/* valor */
					e5->e.bonus->e0 = ref_e( e2 );
	                vs = vs_push_e( vs, e5 );
				}
                unref_e( e1 );
                unref_e( e2 );
                unref_e( e3 );
                unref_e( e4 );
                break;

            case NUM_RNV_UPDATE:
                e1 = vs_pop_e( &vs );
                ident = vs_pop_id( &vs );
                num_rnv_update( ident, e1->e.num );
                unref_e( e1 );
                free( ident );
                break;
            case BOOL_RNV_UPDATE:
                e1 = vs_pop_e( &vs );
                ident = vs_pop_id( &vs );
                bool_rnv_update( ident, ( e1->tag == FALSE ? 0 : 1 ) );
                unref_e( e1 );
                free( ident );
                break;
            case STRING_RNV_UPDATE:
                e1 = vs_pop_e( &vs );
                ident = vs_pop_id( &vs );
                string_rnv_update( ident,
                                   strcpy( ( char* )malloc( strlen( e1->e.name ) + 1 ),
                                           e1->e.name ) );
                unref_e( e1 );
                free( ident );
                break;
            case DATE_RNV_UPDATE:
                e1 = vs_pop_e( &vs );
                ident = vs_pop_id( &vs );
                date_rnv_update( ident, e1->e.date );
                unref_e( e1 );
                free( ident );
                break;
            case TIME_RNV_UPDATE:
                e1 = vs_pop_e( &vs );
                ident = vs_pop_id( &vs );
                time_rnv_update( ident, e1->e.time );
                unref_e( e1 );
                free( ident );
                break;

            case EvCONDS:
                l1 = vs_pop_l( &vs );
                if( l1 == NULL ) {
                    e1 = malloc( sizeof( struct expr ) );
                    e1->refs = 0;
                    e1->tag = TRUE;
                    vs = vs_push_e( vs, e1 );
                }
                else {
                    vs = vs_push_l( vs, l1->tail );
                    cs = cs_push( cs, IF_COND_CONDS );
                    vs = vs_push_e( vs, l1->head );
                    cs = cs_push( cs, EvEXPR );
                }
                unref_l( l1 );
                break;

            case IF_COND_CONDS:
                e1 = vs_pop_e( &vs );

                if( e1->tag == TRUE ) {
                    cs = cs_push( cs, EvCONDS );
                }
                else {
                    l1 = vs_pop_l( &vs );
                    unref_l( l1 );
                    vs = vs_push_e( vs, e1 );
                }
                unref_e( e1 );
                break;

            case EvPRECONDS:
                l1 = vs_pop_l( &vs );
                if( l1 == NULL ) {
                    e1 = malloc( sizeof( struct expr ) );
                    e1->refs = 0;
                    e1->tag = TRUE;
                    vs = vs_push_e( vs, e1 );
		    evaluatingPreConditions = 0;
                }
                else {
                    vs = vs_push_l( vs, l1->tail );
                    cs = cs_push( cs, IF_COND_PRECONDS );
                    vs = vs_push_e( vs, l1->head );
                    cs = cs_push( cs, EvEXPR );
		    evaluatingPreConditions = 1;
                }
                unref_l( l1 );
                break;

            case IF_COND_PRECONDS:
                e1 = vs_pop_e( &vs );

                if( e1->tag == TRUE ) {
                    cs = cs_push( cs, EvPRECONDS );
                }
                else {
                    l1 = vs_pop_l( &vs );
                    unref_l( l1 );
                    vs = vs_push_e( vs, e1 );
                }
                unref_e( e1 );
                break;

            case EvBENEF:
                b1 = vs_pop_b( &vs );
                switch( b1->tag ) {
                    case ASSIGN:
                        vs = vs_push_id( vs, b1->b.assign.var );
                        vs = vs_push_e( vs, b1->b.assign.e );
                        cs = cs_push( cs, EvUPDATE );
                        cs = cs_push( cs, EvEXPR );
                        break;
                    case IF:
                        vs = vs_push_b( vs, b1->b.cond.c2 );
                        vs = vs_push_b( vs, b1->b.cond.c1 );
                        vs = vs_push_e( vs, b1->b.cond.b );
                        cs = cs_push( cs, IF_THEN_ELSE );
                        cs = cs_push( cs, EvEXPR );
                        break;
                    case FOR:
                        vs = vs_push_id( vs, b1->b.foreach.k );
                        vs = vs_push_e( vs, value( s, b1->b.foreach.k ) );
                        cs = cs_push( cs, EvUPDATE );
                        vs = vs_push_id( vs, b1->b.foreach.a );
                        vs = vs_push_e( vs, value( s, b1->b.foreach.a ) );
                        cs = cs_push( cs, EvUPDATE );
                        vs = vs_push_b( vs, b1 );
                        vs = vs_push_e( vs, b1->b.foreach.e );
                        cs = cs_push( cs, EvFOR );
                        cs = cs_push( cs, EvEXPR );
                        break;

                    case BPRINT:
                        //27/01/05
                        vs = vs_push_e( vs, b1->b.print );
                        cs = cs_push( cs, EvPRINT );
                        cs = cs_push( cs, EvEXPR );
                        break;

                    case CREDIT:
                        vs = vs_push_e( vs, b1->b.credit.e4 );
                        vs = vs_push_e( vs, b1->b.credit.e3 );
                        vs = vs_push_e( vs, b1->b.credit.e2 );
                        vs = vs_push_e( vs, b1->b.credit.e1 );
                        cs = cs_push( cs, EvCREDIT );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP_2 );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP_3 );
                        cs = cs_push( cs, EvEXPR );
                        break;

                    case DISTRIBUTE:
                        vs = vs_push_e( vs, b1->b.distribute.e3 );
                        vs = vs_push_e( vs, b1->b.distribute.e2 );
                        vs = vs_push_e( vs, b1->b.distribute.e1 );
                        cs = cs_push( cs, EvDISTRIBUTE );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP_2 );
                        cs = cs_push( cs, EvEXPR );
                        break;

                    case ISSUE_BONUS:
                        vs = vs_push_e( vs, b1->b.credit.e1 );
                        vs = vs_push_e( vs, b1->b.credit.e2 );
                        cs = cs_push( cs, EvISSUE_BONUS );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        break;

                    case GIVE_POINTS:
                        vs = vs_push_e( vs, b1->b.credit.e2 );
                        vs = vs_push_e( vs, b1->b.credit.e1 );
                        cs = cs_push( cs, EvGIVE_POINTS );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        break;

                    case ACTIVATE:
                        vs = vs_push_e( vs, b1->b.activate.e2 );
                        vs = vs_push_e( vs, b1->b.activate.e1 );
                        cs = cs_push( cs, EvACTIVATE );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        break;

                    case EPROC:
                        vs = vs_push_id( vs, b1->b.eproc.procName );
                        vs = vs_push_e( vs, b1->b.eproc.params );
                        cs = cs_push( cs, EvEPROC );
                        cs = cs_push( cs, EvEXPR );
                        break;

                    case REQ_MEANS:
		        if (b1->b.reqMeans.e2) {
			  vs = vs_push_e( vs, b1->b.reqMeans.e2 );
			  vs = vs_push_e( vs, b1->b.reqMeans.e1 );
			  cs = cs_push( cs, EvREQ_MEANS_CANT );
			  cs = cs_push( cs, EvEXPR );
			  cs = cs_push( cs, SWAP );
			  cs = cs_push( cs, EvEXPR );
			} else {
			  vs = vs_push_e( vs, b1->b.reqMeans.e1 );
			  cs = cs_push( cs, EvREQ_MEANS );
			  cs = cs_push( cs, EvEXPR );
			}
                        break;

                    case REC:
                        switch( b1->b.rec.type ) {
                            case REC_PROMOID_OLD:
                                vs = vs_push_e( vs, b1->b.rec.e1 );
                                cs = cs_push( cs, EvREC_PROMOID_OLD );
                                cs = cs_push( cs, EvEXPR );
                                break;
                            case REC_PROMOID:
                                vs = vs_push_e( vs, b1->b.rec.e2 );
                                vs = vs_push_e( vs, b1->b.rec.e1 );
                                cs = cs_push( cs, EvREC_PROMOID );
                                cs = cs_push( cs, EvEXPR );
			  	cs = cs_push( cs, SWAP );
				cs = cs_push( cs, EvEXPR );
                                break;
                            case REC_ARTS:
                                vs = vs_push_e( vs, b1->b.rec.e1 );
                                cs = cs_push( cs, EvREC_ARTS );
                                cs = cs_push( cs, EvEXPR );
                                break;
                            case REC_DEP:
                                vs = vs_push_e( vs, b1->b.rec.e1 );
                                cs = cs_push( cs, EvREC_DEP );
                                cs = cs_push( cs, EvEXPR );
                                break;
                            case REC_MEAN:
                                vs = vs_push_e( vs, b1->b.rec.e1 );
                                cs = cs_push( cs, EvREC_MEAN );
                                cs = cs_push( cs, EvEXPR );
                                break;
                            case REC_CLASS:
                                vs = vs_push_e( vs, b1->b.rec.e1 );
                                cs = cs_push( cs, EvREC_CLASS );
                                cs = cs_push( cs, EvEXPR );
                                break;
                            case REC_ALL:
                                cs = cs_push( cs, EvREC_ALL );
                                break;
                            case REC_PERFIL:
                                vs = vs_push_e( vs, b1->b.rec.e1 );
                                cs = cs_push( cs, EvREC_PERFIL );
                                cs = cs_push( cs, EvEXPR );
                                break;
                            case REC_CREDIT:
                                vs = vs_push_e( vs, b1->b.rec.e1 );
                                cs = cs_push( cs, EvREC_CREDIT );
                                cs = cs_push( cs, EvEXPR );
                                break;
                            case REC_BONUS:
                                vs = vs_push_e( vs, b1->b.rec.e2 );
                                vs = vs_push_e( vs, b1->b.rec.e1 );
                                cs = cs_push( cs, EvREC_BONUS );
                                cs = cs_push( cs, EvEXPR );
                                cs = cs_push( cs, SWAP );
                                cs = cs_push( cs, EvEXPR );
                                break;
                            case REC_POINTS:
                                vs = vs_push_e( vs, b1->b.rec.e2 );
                                vs = vs_push_e( vs, b1->b.rec.e1 );
                                cs = cs_push( cs, EvREC_POINTS );
                                cs = cs_push( cs, EvEXPR );
                                cs = cs_push( cs, SWAP );
                                cs = cs_push( cs, EvEXPR );
                                break;
                            case REC_ACTIVATE_MEAN:
                                vs = vs_push_e( vs, b1->b.rec.e2 );
                                vs = vs_push_e( vs, b1->b.rec.e1 );
                                cs = cs_push( cs, EvACTIVATE_MEAN );
                                cs = cs_push( cs, EvEXPR );
                                cs = cs_push( cs, SWAP );
                                cs = cs_push( cs, EvEXPR );
                                break;
                            case REC_STRING:
                                vs = vs_push_e( vs, b1->b.rec.e2 );
                                vs = vs_push_e( vs, b1->b.rec.e1 );
                                cs = cs_push( cs, EvREC_STRING );
                                cs = cs_push( cs, EvEXPR );
                                cs = cs_push( cs, SWAP );
                                cs = cs_push( cs, EvEXPR );
                                break;
                            default:
                                //fprintf( stderr, "eval.c: REC: registro de evento no soportado\n" );
                                glog( "PROMO: eval.c: REC: registro de evento no soportado\n" ,LOG_MOTOR_PROMO,4);
                                break;
                        }   
                        break;

                    case __SKIP:
                        break;

                    case COMP:
                        vs = vs_push_b( vs, b1->b.comp.c2 );
                        vs = vs_push_b( vs, b1->b.comp.c1 );
                        cs = cs_push( cs, EvBENEF );
                        cs = cs_push( cs, EvBENEF );
                        break;

                    case NUM_RNV_ASSIGN:
                        vs = vs_push_id( vs, b1->b.assign.var );
                        vs = vs_push_e( vs, b1->b.assign.e );
                        cs = cs_push( cs, NUM_RNV_UPDATE );
                        cs = cs_push( cs, EvEXPR );
                        break;
                    case BOOL_RNV_ASSIGN:
                        vs = vs_push_id( vs, b1->b.assign.var );
                        vs = vs_push_e( vs, b1->b.assign.e );
                        cs = cs_push( cs, BOOL_RNV_UPDATE );
                        cs = cs_push( cs, EvEXPR );
                        break;
                    case STRING_RNV_ASSIGN:
                        vs = vs_push_id( vs, b1->b.assign.var );
                        vs = vs_push_e( vs, b1->b.assign.e );
                        cs = cs_push( cs, STRING_RNV_UPDATE );
                        cs = cs_push( cs, EvEXPR );
                        break;
                    case DATE_RNV_ASSIGN:
                        vs = vs_push_id( vs, b1->b.assign.var );
                        vs = vs_push_e( vs, b1->b.assign.e );
                        cs = cs_push( cs, DATE_RNV_UPDATE );
                        cs = cs_push( cs, EvEXPR );
                        break;
                    case TIME_RNV_ASSIGN:
                        vs = vs_push_id( vs, b1->b.assign.var );
                        vs = vs_push_e( vs, b1->b.assign.e );
                        cs = cs_push( cs, TIME_RNV_UPDATE );
                        cs = cs_push( cs, EvEXPR );
                        break;

                    default:
                        unref_b( b1 );
                        p_error( ST( S_BENEFICIO_NO_CONTEMPLADO_ ) );
                        return ABNORM;
                }
                unref_b( b1 );
                break;

            case IF_THEN_ELSE:
                e1 = vs_pop_e( &vs );
                if( e1->tag == TRUE ) {
                    vs = vs_swap( vs );
                    b1 = vs_pop_b( &vs );
                    unref_b( b1 );
                    cs = cs_push( cs, EvBENEF );
                }
                else {
                    b1 = vs_pop_b( &vs );
                    unref_b( b1 );
                    cs = cs_push( cs, EvBENEF );
                }
                unref_e( e1 );
                break;

            case EvPRINT:
                e1 = vs_pop_e( &vs );
                print( e1->e.name );
                unref_e( e1 );
                break;

            case EvCREDIT:
                e4 = vs_pop_e( &vs );
                e3 = vs_pop_e( &vs );
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
		credit( e4->e.num, e1->e.name, e2->e.num, e3 );
		{
		  expr e5;
		  // actualizar la variable global discounts para agregar (e1,e2)
		  e5 = clon_amset( value( s, "discounts" ) );
		  // 09/02/06. Aca va una modificacioncita con un GRAN impacto
		  // Antes era solo esto
		  //ins_amset(e3 , e1->e.name, e2->e.num);
		  // Ahora es
		  {
                    char i[256];
                    char desc[256];
					int k;
					strncpy(desc, e1->e.name, 256);
					for (k=0;k<sizeof(desc);k++) {
						if (desc[k]=='$') desc[k] = '\0';
						if (desc[k]=='\0') break;
					}
                    //_snprintf( i,256, "%s$%lf", e1->e.name, -e2->e.num );
                    _snprintf( i,256, "%s$%lf", desc, -e2->e.num );
                    ins_amset( e5, i, e4->e.num );
		  }
		  // Fin
		  s = update( s, "discounts", e5 );
		  // el nombre de esta var (discounts) debe coincidir con el
		  // nombre utilizado en la promo global
		  //
		}
                unref_e( e1 );
                unref_e( e2 );
                unref_e( e3 );
                unref_e( e4 );
                break;

            case EvDISTRIBUTE:
                e3 = vs_pop_e( &vs );
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
                //distribute( e1->e.name, e2->e.num, e3->e.num );
                distribute( promo_id, promo_id_extern, rec_group++, e1->e.name, e2->e.num, e3->e.num );
                //recCredit( promo_id, promo_id_extern, e1->e.num, rec_group );
                // actualizar la variable global delivered para agregar (e1,e2)
                e4 = clon_amset( value( s, "delivered" ) );
                // 09/02/06. Aca va una modificacioncita con un GRAN impacto
                // Antes era solo esto
                //ins_amset(e4 , e1->e.name, e2->e.num);
                // Ahora es
                {
                    char i[256];
                    _snprintf( i, 256, "%s$%lf", e1->e.name, e3->e.num );
                    ins_amset( e4, i, e2->e.num );
                }
                // Fin
                s = update( s, "delivered", e4 ); 
                // el nombre de esta var (delivered) debe coincidir con el
                // nombre utilizado en la promo global
                //
                unref_e( e1 );
                unref_e( e2 );
                unref_e( e3 );
                break;

            case EvISSUE_BONUS:
                e1 = vs_pop_e( &vs );
                e2 = vs_pop_e( &vs ); //cantidad
                issueBonus( e1->e.bonus->target, e1->e.bonus->type, e1->e.bonus->e0->e.num,
                            e2->e.num, e1->e.bonus->leyenda, e1->e.bonus->maturity->e.date,
                            e1->e.bonus->validity->e.num );
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvGIVE_POINTS:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
                givePoints( e1->e.name, e2->e.num );
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvACTIVATE:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
                switch( e1->tag ) {
                    case CARD:
                        activateCard( promo_id, e1->e.card.name, e1->e.card.plan, e2->e.num );
                        break;
                    case TICKET:
                        activateTicket( promo_id, e1->e.name, e2->e.num );
                        break;
                    case MUTUAL:
                        activateMutual( promo_id, e1->e.name, e2->e.num );
                        break;
                    case CHEQUE:
                        activateCheque( promo_id, e1->e.name, e2->e.num );
                        break;
                    case CURRENCY:
                        activateCurrency( e1->e.name, e2->e.num );
                        break;
                    case CASH:
                        activateCash( e2->e.num );
                        break;
                    default:
                        //fprintf( stderr, "eval.c: EvACTIVATE: activacion de medio no soportado\n" );
                        glog( "PROMO: eval.c: EvACTIVATE: activacion de medio no soportado\n" ,LOG_MOTOR_PROMO,3);
                        break;
                }
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvEPROC:
                e1 = vs_pop_e( &vs );
                ident = vs_pop_id( &vs );
                extProc( ident, e1->e.name );
                unref_e( e1 );
                free( ident );
                break;

#define STR_OR_NULL( cadena ) ( strcmp( cadena, "null" ) == 0 ? NULL : cadena )
            case EvREQ_MEANS_CANT:
	      {
		struct node * s1 = NULL;
	        e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
		s1 = e1->e.set;
		
// MODO 0 -> Se debe pagar todo el ticket con alguno de los medios del grupo.
// MODO 1 -> El monto pagado con los medios del mismo grupo debe cubrir el total del ticket.
// MODO 2 -> Se debe pagar al menos $'num' con alguno de los medios del grupo.
// MODO 3 -> El monto pagado con los medios del mismo grupo debe cubrir al menos $'num'.
		while (s1) {
      		  switch( s1->head->tag ) {
		    case CARD:
		      reqCard( reqMeansGroup, 3, STR_OR_NULL(s1->head->e.card.name), STR_OR_NULL(s1->head->e.card.plan), e2->e.num );
                        break;
                    case TICKET:
		      reqTicket( reqMeansGroup, 3, STR_OR_NULL(s1->head->e.name), e2->e.num );
                        break;
                    case MUTUAL:
		      reqMutual( reqMeansGroup, 3, STR_OR_NULL(s1->head->e.name), e2->e.num );
                        break;
                    case CHEQUE:
		      reqCheque( reqMeansGroup, 3, STR_OR_NULL(s1->head->e.name), e2->e.num );
                        break;
                    case CURRENCY:
		      reqCurrency( reqMeansGroup, 3, STR_OR_NULL(s1->head->e.name), e2->e.num );
                        break;
                    case CASH:
                        reqCash( reqMeansGroup, 3, e2->e.num );
                        break;
                    case CTACTE:
                        reqCtacte( reqMeansGroup, 3, e2->e.num );
                        break;
						
                    default:
		        //fprintf( stderr, "eval.c: EvACTIVATE: activacion de medio no soportado\n" );
                        glog( "eval.c: EvREQ_MEANS: activacion de medio no soportado\n", LOG_MOTOR_PROMO,3);
                        break;
		  }
		  s1 = s1->tail;
		}
		reqMeansGroup++;
                unref_e( e1 );
                unref_e( e2 );
	      }
                break;

            case EvREQ_MEANS:
	      {
		struct node * s1 = NULL;
	        //e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
		s1 = e1->e.set;
		
		while (s1) {
      		  switch( s1->head->tag ) {
		    case CARD:
		      reqCard( reqMeansGroup, 1, STR_OR_NULL(s1->head->e.card.name), STR_OR_NULL(s1->head->e.card.plan), -1 );
                        break;
                    case TICKET:
		      reqTicket( reqMeansGroup, 1, STR_OR_NULL(s1->head->e.name), -1 );
                        break;
                    case MUTUAL:
		      reqMutual( reqMeansGroup, 1, STR_OR_NULL(s1->head->e.name), -1 );
                        break;
                    case CHEQUE:
		      reqCheque( reqMeansGroup, 1, STR_OR_NULL(s1->head->e.name), -1 );
                        break;
                    case CURRENCY:
		      reqCurrency( reqMeansGroup, 1, STR_OR_NULL(s1->head->e.name), -1 );
                        break;
                    case CASH:
                        reqCash( reqMeansGroup, 1, -1 );
                        break;
                    default:
		        fprintf( stderr, "eval.c: EvACTIVATE: activacion de medio no soportado\n" );
                        //glog( "eval.c: EvREQ_MEANS: activacion de medio no soportado\n" );
                        break;
		  }
		  s1 = s1->tail;
		}
		reqMeansGroup++;
                unref_e( e1 );
                //unref_e( e2 );
	      }
                break;


            case EvREC_PROMOID_OLD:
                e1 = vs_pop_e( &vs );
                promo_id = (long)e1->e.num;
		if (promo_id_extern) {
		    free(promo_id_extern);
		}
		promo_id_extern = NULL;
                //rec_group = 0;
                unref_e( e1 );
                break;

            case EvREC_PROMOID:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
                promo_id = (long)e1->e.num;
		if (promo_id_extern) {
		    free(promo_id_extern);
		}
		promo_id_extern = (char*) malloc(strlen(e2->e.name)+1);
		strcpy(promo_id_extern,e2->e.name);
//fprintf(stdout,"Y el nuevo promo_id_extern es ... %s\n",promo_id_extern);
                //rec_group = 0;
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvREC_ARTS:
                e1 = vs_pop_e( &vs );
                {
                    mlist ml1 = e1->e.amset;

                    while( ml1 ) {
                        recArt( promo_id, promo_id_extern, ml1->article->e.name, ml1->freq->e.num, rec_group );
                        ml1 = ml1->tail;
                    }
                }
                unref_e( e1 );
                break;

            case EvREC_MEAN:
                e1 = vs_pop_e( &vs );
                switch( e1->tag ) {
                    case CASH:
                        recCash( promo_id, promo_id_extern, rec_group );
                        break;
                    case CTACTE:
                        recCtacte( promo_id, promo_id_extern, rec_group );
                        break;
                    case MUTUAL:
                        recMutual( promo_id, promo_id_extern, e1->e.name, rec_group );
                        break;
                    case TICKET:
                        recTicket( promo_id, promo_id_extern, e1->e.name, rec_group );
                        break;
                    case CHEQUE:
                        recCheque( promo_id, promo_id_extern, e1->e.name, rec_group );
                        break;
                    case CURRENCY:
                        recCurrency( promo_id, promo_id_extern, e1->e.name, rec_group );
                        break;
                    case CARD:
                        recCard( promo_id, promo_id_extern, e1->e.card.name, e1->e.card.plan, rec_group );
                        break;
                    default:
                        break;
                }
                unref_e( e1 );
                break;

            case EvREC_ALL:
                recAll( promo_id, promo_id_extern, rec_group );
                break;

            case EvREC_PERFIL:
                e1 = vs_pop_e( &vs );
                recPerfil( promo_id, promo_id_extern, (long) e1->e.num, rec_group );
                unref_e( e1 );
                break;
                // FALTAN DE IMPLEMENTAR
                //               EvREC_DEP, EvREC_CLASS,
                //               EvREC_PERFIL,
                //               EvREC_STRING,

            case EvREC_CREDIT:
                e1 = vs_pop_e( &vs );
                recCredit( promo_id, promo_id_extern, e1->e.num, rec_group );
                rec_group++;
                unref_e( e1 );
                break;

            case EvREC_BONUS:
                e1 = vs_pop_e( &vs );
                e2 = vs_pop_e( &vs );
                recBonus( promo_id, promo_id_extern, e1->e.bonus->type, e1->e.bonus->e0->e.num, e2->e.num,
                          rec_group );
                rec_group++;
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvREC_POINTS:
                e1 = vs_pop_e( &vs );
                e2 = vs_pop_e( &vs );
                recPoints( promo_id, promo_id_extern, e1->e.name, e2->e.num, rec_group );
                rec_group++;
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvACTIVATE_MEAN:
                e1 = vs_pop_e( &vs );
                e2 = vs_pop_e( &vs );
                switch( e1->tag ) {
                    case CARD:
                        recActivateCard( promo_id, promo_id_extern, e1->e.card.name, e1->e.card.plan, e2->e.num,
                                         rec_group );
                        break;
                    case TICKET:
						recActivateTicket( promo_id, promo_id_extern, e1->e.name, e2->e.num, rec_group );
                        break;
                    case MUTUAL:
						recActivateMutual( promo_id, promo_id_extern, e1->e.name, e2->e.num, rec_group );
                        break;

                    default:
						glog("PROMO: EvACTIVATE_MEAN: recActivate: ERROR: Medio no implementado.",LOG_MOTOR_PROMO,3);
                        break;
                }

                //recPoints( promo_id, e1->e.name, e2->e.num, rec_group );
                rec_group++;
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvFOR:
                e1 = vs_pop_e( &vs );
                vs = vs_push_ml( vs, e1->e.amset );
                cs = cs_push( cs, EvFOREACH );
                unref_e( e1 );
                break;

            case EvFOREACH:
                ml1 = vs_pop_ml( &vs );
                b1 = vs_pop_b( &vs );
                if( ml1 ) {
                    vs = vs_push_b( vs, b1 );
                    vs = vs_push_ml( vs, ml1->tail );
                    vs = vs_push_b( vs, b1->b.foreach.c );
                    vs = vs_push_id( vs, b1->b.foreach.k );
                    vs = vs_push_e( vs, ml1->freq );
                    vs = vs_push_id( vs, b1->b.foreach.a );
                    vs = vs_push_e( vs, ml1->article );
                    cs = cs_push( cs, EvFOREACH );
                    cs = cs_push( cs, EvBENEF );
                    cs = cs_push( cs, EvUPDATE );
                    cs = cs_push( cs, EvUPDATE );
                }
                unref_b( b1 );
                unref_ml( ml1 );
                break;

            case EvEXPR:
                e1 = vs_pop_e( &vs );
                switch( e1->tag ) {
                    case NUM:
                        vs = vs_push_e( vs, e1 );
                        unref_e( e1 );
                        break;
                    case SUM:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvSUM );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;
                    case SUB:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvSUB );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;
                    case MULT:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvMULT );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case FDIV:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvFDIV );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case MOD:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvMOD );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case DIV:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvDIV );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case MAX:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvMAX );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case MIN:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvMIN );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case LEQ:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvLEQ );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case LESS:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvLESS );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case EQUAL:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvEQUAL );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case GREATER:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvGREATER );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case GEQ:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvGEQ );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case NEQ:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvNEQ );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case COND:
                        vs = vs_push_e( vs, e1->e.cond->e2 );
                        vs = vs_push_e( vs, e1->e.cond->e1 );
                        vs = vs_push_e( vs, e1->e.cond->e0 );
                        cs = cs_push( cs, EvECOND );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case AND:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvAND );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case OR:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvOR );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case LOG:
                        vs = vs_push_e( vs, e1 );
                        vs = vs_push_e( vs, e1->e.log.e0 );
                        cs = cs_push( cs, EvLOG );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case SET:
                        vs = vs_push_l( vs, NULL );
                        vs = vs_push_l( vs, e1->e.set );
                        cs = cs_push( cs, EvSET );
                        unref_e( e1 );
                        break;

                    case AMSET:
                        vs = vs_push_ml( vs, NULL );
                        vs = vs_push_ml( vs, e1->e.amset );
                        cs = cs_push( cs, EvAMSET );
                        unref_e( e1 );
                        break;

                    case PART:
                        vs = vs_push_p( vs, NULL );
                        vs = vs_push_p( vs, e1->e.part );
                        cs = cs_push( cs, EvPART );
                        unref_e( e1 );
                        break;

                    case DOT:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvDOT );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case INTERVAL:
                        vs = vs_push_e( vs, e1->e.interval.e1 );
                        vs = vs_push_e( vs, e1->e.interval.e0 );
                        cs = cs_push( cs, EvINTERVAL );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case NUM_RNV:
                        e2 = ( expr )malloc( sizeof( struct expr ) );
                        e2->refs = 0;
                        e2->tag = NUM;
                        e2->e.num = num_rnv( e1->e.name );
                        vs = vs_push_e( vs, e2 );
                        unref_e( e1 );
                        break;
                    case BOOL_RNV:
                        e2 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e2->refs = 0;
                        e2->tag = ( num_rnv( e1->e.name ) == 0 ? FALSE : TRUE );
                        vs = vs_push_e( vs, e2 );
                        unref_e( e1 );
                        break;
                    case STRING_RNV:
                        e2 = ( expr )malloc( SIZE_OF_EXPR_IDENT );
                        e2->refs = 0;
                        e2->tag = STRING;
                        e2->e.name = string_rnv( e1->e.name );
                        vs = vs_push_e( vs, e2 );
                        unref_e( e1 );
                        break;
                    case DATE_RNV:
                        e2 = ( expr )malloc( SIZE_OF_EXPR_DATEV );
                        e2->refs = 0;
                        e2->tag = DATEV;
                        e2->e.date = date_rnv( e1->e.name );
                        vs = vs_push_e( vs, e2 );
                        unref_e( e1 );
                        break;
                    case TIME_RNV:
                        e2 = ( expr )malloc( SIZE_OF_EXPR_TIMEV );
                        e2->refs = 0;
                        e2->tag = TIMEV;
                        e2->e.time = time_rnv( e1->e.name );
                        vs = vs_push_e( vs, e2 );
                        unref_e( e1 );
                        break;

                    case IDENT:
                        /* 16-02-06 Tratamiento especial para las variables purchase y whole purchase */
                        if( strcmp( "purchase", e1->e.name ) == 0 ) {
                            vs = vs_push_e( vs, purch );
                            cs = cs_push( cs, EvEXPR );
                        }
                        else if( strcmp( "wholepurchase", e1->e.name ) == 0 ) {
                            vs = vs_push_e( vs, wpurch );
                            cs = cs_push( cs, EvEXPR );
                        }
                        else {
                            /* ---- */
			  if (evaluatingPreConditions) {
                            vs = vs_push_e( vs, value( (*ss)->s, e1->e.name ) );
			    //e1 = value( ( *ss )->s, pa1->var );
			  } else {
                            vs = vs_push_e( vs, value( s, e1->e.name ) );
			  }
                        }
                        unref_e( e1 );
                        break;

                    case TIME:
                        e2 = ( expr )malloc( SIZE_OF_EXPR_TIMEV );
                        e2->refs = 0;
                        e2->tag = TIMEV;
                        e2->e.time = get_time();
                        vs = vs_push_e( vs, e2 );
                        unref_e( e1 );
                        break;

                    case DATE:
                        e2 = ( expr )malloc( SIZE_OF_EXPR_DATEV );
                        e2->refs = 0;
                        e2->tag = DATEV;
                        e2->e.date = get_date();
                        vs = vs_push_e( vs, e2 );
                        unref_e( e1 );
                        break;

                    case DATEOFBIRTH:
                        e2 = ( expr )malloc( SIZE_OF_EXPR_DATEV );
                        e2->refs = 0;
                        e2->tag = DATEV;
                        e2->e.date = dateofbirth();
                        vs = vs_push_e( vs, e2 );
                        unref_e( e1 );
                        break;

                    case _DAY:
                        e2 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e2->refs = 0;
						e2->tag = ( tExpr_ ) ( ( get_date() + 6 ) % 7 ) + MONDAY; //24-03-07
                        //e2->tag = ( tExpr_ )( get_date() + 5 ) % 7 + MONDAY; //10-11-06
                        //e2->tag = ( tExpr_ )( get_date() + 6 ) % 7 + MONDAY;
                        vs = vs_push_e( vs, e2 );
                        unref_e( e1 );
                        break;

                    case TRUE:
                    case FALSE:
                    case STRING:
                    case DEPARTMENT:
                    case CLASS:
                    case POINT:
                        //case BONUS:
                    case ARTICLE:
                    case CARD:
                    case MUTUAL:
                    case TICKET:
                    case CHEQUE:
                    case CURRENCY:
                    case ALL:
                    case CASH:
                    case CTACTE:
                    case MONDAY:
                    case TUESDAY:
                    case WEDNESDAY:
                    case THURSDAY:
                    case FRIDAY:
                    case SATURDAY:
                    case SUNDAY:
                    case TIMEV:
                    case DATEV:
                    case DAYSELECTOR:
                    case MONTHSELECTOR:
                    case YEARSELECTOR:
                        vs = vs_push_e( vs, e1 );
                        unref_e( e1 );
                        break;

                    case BONUS:
                        vs = vs_push_e( vs, e1 );
                        vs = vs_push_e( vs, e1->e.bonus->maturity );
                        vs = vs_push_e( vs, e1->e.bonus->validity );
                        vs = vs_push_e( vs, e1->e.bonus->e0 );
                        cs = cs_push( cs, EvBONUS_UPDATE );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP_2 );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case MEANS:
                        vs = vs_push_e( vs, means_set );
                        unref_e( e1 );
                        break;

                    case CHANGE:
                        vs = vs_push_e( vs, change() );
                        unref_e( e1 );
                        break;

                    case RETURNED:
                        vs = vs_push_e( vs, returned() );
                        unref_e( e1 );
                        break;

                    case POINTS:
                        vs = vs_push_e( vs, points() );
                        unref_e( e1 );
                        break;

                    case EBONUS:
                        vs = vs_push_e( vs, ebonus() );
                        unref_e( e1 );
                        break;

                    case CASHDESK:
                        e2 = ( expr )malloc( sizeof( struct expr ) );
                        e2->refs = 0;
                        e2->tag = NUM;
                        e2->e.num = cashdesk();
                        vs = vs_push_e( vs, e2 );
                        unref_e( e1 );
                        break;

                    case INPUT:
                        //vs=vs_push_e(vs, e1->e.unop);
                        vs = vs_push_e( vs, e1->e.unop );
                        cs = cs_push( cs, EvINPUT );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case NUMINPUT:
                        //vs=vs_push_e(vs, e1->e.unop);
                        vs = vs_push_e( vs, e1->e.unop );
                        cs = cs_push( cs, EvNUMINPUT );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case HOLIDAY:
                        e2 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e2->refs = 0;
                        if( holiday() ) {
                            e2->tag = TRUE;
                        }
                        else {
                            e2->tag = FALSE;
                        }
                        vs = vs_push_e( vs, e2 );
                        unref_e( e1 );
                        break;

                    case AFFILIATED:
                        e2 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e2->refs = 0;
                        if( affiliated() ) {
                            e2->tag = TRUE;
                        }
                        else {
                            e2->tag = FALSE;
                        }
                        vs = vs_push_e( vs, e2 );
                        unref_e( e1 );
                        break;
                        /*
                             case PURCHASE:
                        {
                          expr e_sub = (expr)malloc(sizeof(struct expr));
                          expr e_purch = (expr)malloc(SIZE_OF_EXPR_CTE);
                          expr e_union = (expr)malloc(sizeof(struct expr));
                          expr e_excluded = (expr)malloc(SIZE_OF_EXPR_IDENT);
                          expr e_nopromo = (expr)malloc(SIZE_OF_EXPR_IDENT);
                          e_sub->refs = 0;
                          e_purch->refs = 1;
                          e_union->refs = 1;
                          e_excluded->refs = 1;
                          e_nopromo->refs = 1;
                          e_sub->tag = SUB;
                          e_purch->tag = WPURCHASE;
                          e_union->tag = UNION;
                          e_excluded->tag = IDENT;
                          e_nopromo->tag = IDENT;
                          e_sub->e.binop.e0 = e_purch;
                          e_sub->e.binop.e1 = e_union;
                          e_union->e.binop.e0 = e_excluded;
                          e_union->e.binop.e1 = e_nopromo;
                          { char* vname = "excluded";
                            char* ident;
                            int index = strlen(vname);
                            ident = (char*)malloc(index+1);
                            strcpy(ident,vname);
                            ident[index] = '\0';
                            e_excluded->e.name = ident;
                          }
                          { char* vname = "nopromotion";
                            char* ident;
                            int index = strlen(vname);
                            ident = (char*)malloc(index+1);
                            strcpy(ident,vname);
                            ident[index] = '\0';
                            e_nopromo->e.name = ident;
                          }
                          vs=vs_push_e(vs, e_sub);
                          cs=cs_push(cs, EvEXPR);
                        }
                        unref_e(e1);
                        break;
                             case WPURCHASE:
                        vs = vs_push_e(vs, purch);
                        unref_e(e1);
                        break;
                        */

                    case BUYED:
                        vs = vs_push_e( vs, buyed_articles() );
                        unref_e( e1 );
                        break;

                    case DELIVERED:
                        vs = vs_push_e( vs, delivered_articles() );
                        unref_e( e1 );
                        break;

                    case DISCOUNTS:
                        vs = vs_push_e( vs, discounts_credits() );
                        unref_e( e1 );
                        break;

                    case ADDED:
                        vs = vs_push_e( vs, added );
                        unref_e( e1 );
                        break;

                    case UNION:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvUNION );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case INTERSECTION:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvINTERSECTION );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case IN:
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        cs = cs_push( cs, EvIN );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case CONCAT:
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        cs = cs_push( cs, EvCONCAT );
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case MEET:
                        vs = vs_push_e( vs, NULL );
                        vs = vs_push_e( vs, e1->e.binop.e0 );
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        cs = cs_push( cs, EvMEET );
                        cs = cs_push( cs, SACAR_ML ); // 21-04-06 nueva implementacion
                        cs = cs_push( cs, EvEXPR );
                        cs = cs_push( cs, SWAP );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case COMPLEMENT:
                        vs = vs_push_e( vs, e1->e.unop );
                        cs = cs_push( cs, EvCOMPLEMENT );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case _ROUND:
                        vs = vs_push_e( vs, e1->e.unop );
                        cs = cs_push( cs, EvROUND );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case TRUNC:
                        vs = vs_push_e( vs, e1->e.unop );
                        cs = cs_push( cs, EvTRUNC );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case AMOUNT:
                        //if (e1->e.unop == value(s, "wholepurchase")) {
                        //glog("Amount(WHOLEPURCHASE)");
                        //}
                        vs = vs_push_e( vs, e1->e.unop );
                        cs = cs_push( cs, EvAMOUNT );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case SIZE:
                        vs = vs_push_e( vs, e1->e.unop );
                        cs = cs_push( cs, EvSIZE );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case LENGTH:
                        vs = vs_push_e( vs, e1->e.unop );
                        cs = cs_push( cs, EvLENGTH );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case COUNTABLE:
                        vs = vs_push_e( vs, e1->e.unop );
                        cs = cs_push( cs, EvCOUNTABLE );
                        cs = cs_push( cs, EvEXPR );
                        unref_e( e1 );
                        break;

                    case REQ_ANY:
                        e2 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e2->refs = 0;
                        e2->tag = FALSE;
                        {
                            mlist ml1 = e1->e.amset;

                            while( ml1 ) {
                                if( *( double* )( ml1->article ) >= ml1->freq->e.num ) {
                                    e2->tag = TRUE;
                                    ml1 = NULL;
                                }
                                else {
                                    ml1 = ml1->tail;
                                }
                            }
                        }
                        vs = vs_push_e( vs, e2 );
                        unref_e( e1 );
                        break;

                    case REQ_ALL:
                        e2 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e2->refs = 0;
                        e2->tag = TRUE;
                        {
                            mlist ml1 = e1->e.amset;

                            while( ml1 ) {
                                if( *( double* )( ml1->article ) < ml1->freq->e.num ) {
                                    e2->tag = FALSE;
                                    ml1 = NULL;
                                }
                                else {
                                    ml1 = ml1->tail;
                                }
                            }
                        }
                        vs = vs_push_e( vs, e2 );
                        unref_e( e1 );
                        break;

                    case REQ_DEP:
                        e2 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e2->refs = 0;
                        e2->tag = FALSE;
                        {
			  mlist ml1 = (mlist)e1->e.binop.e0;
			    double req = e1->e.binop.e1->e.num;
			    double c = 0;

                            while( ml1 ) {
			      c += *( double* )( ml1->article );
			      if ( c >= req  ) {
				e2->tag = TRUE;
				ml1 = NULL;
			      } else {
				ml1 = ml1->tail;
			      }
                            }

                        }
                        vs = vs_push_e( vs, e2 );
                        unref_e( e1 );
                        break;


                    default:
                        unref_e( e1 );
                        p_error( ST( S_EXPRESION_NO_CONTEMPLADA_PARA_LA_ ) );  // 27/01/05
                        return ABNORM;
                }

                break;

            case EvSUM:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );

                switch( e1->tag ) {
                    case NUM:
                        e3 = ( expr )malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        if ( e2->tag == DATEV ) {
	                        e3->tag = DATEV;
	                        e3->e.date = (idate) e1->e.num + e2->e.date;
                        } else {
	                        e3->tag = NUM;
	                        e3->e.num = e1->e.num + e2->e.num;
                        }
                        vs = vs_push_e( vs, e3 );
                        break;
                    case DATEV:
                        e3 = ( expr )malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        e3->tag = DATEV;
                        e3->e.date = e1->e.date + (idate) e2->e.num;
                        //fprintf(stderr,"e1:%li, e2:%lf, e3:%li\n",e1->e.date,e2->e.num,e3->e.date);
                        vs = vs_push_e( vs, e3 );
                        break;
                    case AMSET:
                        ml1 = e1->e.amset;
                        ml2 = e2->e.amset;

                        ml3 = NULL;
                        while( ml1 ) {
                            ml4 = ( mlist )malloc( sizeof( struct mnode ) );
                            ml4->refs = 0;
                            ml4->article = ref_e( ml1->article );
                            ml4->freq = ref_e( ml1->freq );
                            ml4->tail = ref_ml( ml3 );
                            ml3 = ml4;
                            ml1 = ml1->tail;
                        }

                        ml1 = ml3;

                        while( ml2 ) {
                            int done = 0;
                            ml3 = ml1;
                            while( ml3 && !done ) {
                                if( idcmp( ml2->article->e.name, ml3->article->e.name ) == 0 ) {
                                    e4 = ( expr )malloc( sizeof( struct expr ) );
                                    e4->refs = 0;
                                    e4->tag = NUM;
                                    e4->e.num = ml2->freq->e.num + ml3->freq->e.num;
                                    unref_e( ml3->freq );
                                    ml3->freq = ref_e( e4 );
                                    done = 1;
                                }
                                ml3 = ml3->tail;
                            }
                            if( !done ) {
                                ml3 = ( mlist )malloc( sizeof( struct mnode ) );
                                ml3->refs = 0;
                                ml3->article = ref_e( ml2->article );
                                ml3->freq = ref_e( ml2->freq );
                                ml3->tail = ref_ml( ml1 );
                                ml1 = ml3;
                            }
                            ml2 = ml2->tail;
                        }

                        e3 = ( expr )malloc( SIZE_OF_EXPR_AMSET );
                        e3->refs = 0;
                        e3->tag = AMSET;
                        e3->e.amset = ref_ml( ml1 );

                        vs = vs_push_e( vs, e3 );
                        break;

                    default:
                        unref_e( e1 );
                        unref_e( e2 );
                        p_error( ST( S_SUMA_MAL_TIPADA ) );
                        return ABNORM;
                }

                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvSUB:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );

                switch( e1->tag ) {
                    case NUM:
                        e3 = ( expr )malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        e3->tag = NUM;
                        e3->e.num = e1->e.num - e2->e.num;
                        vs = vs_push_e( vs, e3 );
                        break;
                    case AMSET:
                        switch( e2->tag ) {
                            case ARTICLE:
                                ml1 = e1->e.amset;
                                ml2 = NULL;
                                while( ml1 ) {
                                    if( idcmp( ml1->article->e.name, e2->e.name ) != 0 ) {
                                        ml3 = ( mlist )malloc( sizeof( struct mnode ) );
                                        ml3->refs = 0;
                                        ml3->article = ref_e( ml1->article );
                                        ml3->freq = ref_e( ml1->freq );
                                        ml3->tail = ref_ml( ml2 );
                                        ml2 = ml3;
                                    }
                                    ml1 = ml1->tail;
                                }
                                e3 = ( expr )malloc( SIZE_OF_EXPR_AMSET );
                                e3->refs = 0;
                                e3->tag = AMSET;
                                e3->e.amset = ref_ml( ml2 );;
                                vs = vs_push_e( vs, e3 );
                                break;
                            case AMSET:
                            case ALL:
                            case UNION:
                            case INTERSECTION:
                            case COMPLEMENT:
                            case DEPARTMENT:
                            case CLASS:
                                vs = vs_push_ml( vs, NULL );
                                vs = vs_push_ml( vs, e1->e.amset );
                                vs = vs_push_e( vs, e2 );
                                cs = cs_push( cs, RESTE );    
                                break;
                            default:
                                unref_e( e1 );
                                unref_e( e2 );
                                p_error( ST( S_RESTA_MAL_TIPADA ) );
                                return ABNORM;
                        }
                        break;
                    default:
                        unref_e( e1 );
                        unref_e( e2 );
                        p_error( ST( S_RESTA_MAL_TIPADA ) );
                        return ABNORM;
                };
                unref_e( e1 );
                unref_e( e2 );
                break;


            case EvMULT:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );

                switch( e2->tag ) {
                    case NUM:
                        e3 = ( expr )malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        e3->tag = NUM;
                        e3->e.num = e1->e.num * e2->e.num;
                        vs = vs_push_e( vs, e3 );
                        break;

                    case AMSET:
                        if( e1->e.num == 0 ) {
                            ml2 = NULL;
                        }
                        else if( e1->e.num == 1 ) {
                            ml2 = e2->e.amset;
                        }
                        else if( e1->e.num < 0 ) {
                            unref_e( e1 );
                            unref_e( e2 );
                            p_error( ST( S_NEGATIVE_ARITHMETIC_MULTI_SET ) );
                            return ABNORM;
                        }
                        else {
                            ml1 = e2->e.amset;
                            ml2 = NULL;
                            while( ml1 ) {
                                ml3 = ( mlist )malloc( sizeof( struct mnode ) );
                                ml3->refs = 0;
                                ml3->article = ref_e( ml1->article );
                                e4 = ( expr )malloc( sizeof( struct expr ) );
                                e4->refs = 0;
                                e4->tag = NUM;
                                e4->e.num = ml1->freq->e.num * e1->e.num;
                                ml3->freq = ref_e( e4 );
                                ml3->tail = ref_ml( ml2 );
                                ml2 = ml3;
                                ml1 = ml1->tail;
                            }
                        }
                        e3 = ( expr )malloc( SIZE_OF_EXPR_AMSET );
                        e3->refs = 0;
                        e3->tag = AMSET;
                        e3->e.amset = ref_ml( ml2 );
                        vs = vs_push_e( vs, e3 );
                        break;

                    default:
                        e3 = ( expr )malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        e3->tag = MULT;
                        e3->e.binop.e0 = ref_e( e1 );
                        e3->e.binop.e1 = ref_e( e2 );
                        vs = vs_push_e( vs, e3 );
                };

                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvFDIV:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
                if( e2->e.num == 0 ) {
                    unref_e( e1 );
                    unref_e( e2 );
                    p_error( ST( S_DIVISION_BY_ZERO ) );
                    return DIVBYZERO;
                }
                e3 = malloc( sizeof( struct expr ) );
                e3->refs = 0;
                e3->tag = NUM;
                e3->e.num = e1->e.num / e2->e.num;
                vs = vs_push_e( vs, e3 );
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvMOD:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
                if( ( int )e2->e.num == 0 ) {
                    unref_e( e1 );
                    unref_e( e2 );
                    p_error( ST( S_DIVISION_BY_ZERO ) );
                    return DIVBYZERO;
                }
                e3 = malloc( sizeof( struct expr ) );
                e3->refs = 0;
                e3->tag = NUM;
                e3->e.num = ( double )( ( int )e1->e.num % ( int )e2->e.num );
                vs = vs_push_e( vs, e3 );
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvDIV:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
                if( ( int )e2->e.num == 0 ) {
                    unref_e( e1 );
                    unref_e( e2 );
                    p_error( ST( S_DIVISION_BY_ZERO ) );
                    return DIVBYZERO;
                }
                e3 = malloc( sizeof( struct expr ) );
                e3->refs = 0;
                e3->tag = NUM;
                e3->e.num = ( double )( ( int )e1->e.num / ( int )e2->e.num );
                vs = vs_push_e( vs, e3 );
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvMAX:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
                vs = vs_push_e( vs, ( e1->e.num >= e2->e.num ) ? e1 : e2 );
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvMIN:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
                vs = vs_push_e( vs, ( e1->e.num >= e2->e.num ) ? e2 : e1 );
                unref_e( e1 );
                unref_e( e2 );
                break;

                #define EV_COMP(relop) \
                      switch(e1->tag) {\
                      case NUM:\
                        e3 = malloc(sizeof(struct expr));\
                    e3->refs = 0;\
                    if (e1->e.num relop e2->e.num) {\
                      e3->tag = TRUE;\
                    } else {\
                      e3->tag = FALSE;\
                    }\
                    vs = vs_push_e(vs, e3);\
                    break;\
                      case TIMEV:\
                        e3 = malloc(sizeof(struct expr));\
                    e3->refs = 0;\
                    if (e1->e.time relop e2->e.time) {\
                      e3->tag = TRUE;\
                    } else {\
                      e3->tag = FALSE;\
                    }\
                    vs = vs_push_e(vs, e3);\
                    break;\
                      case DATEV:\
                        e3 = malloc(sizeof(struct expr));\
                    e3->refs = 0;\
                    if (e1->e.date relop e2->e.date) {\
                      e3->tag = TRUE;\
                    } else {\
                      e3->tag = FALSE;\
                    }\
                    vs = vs_push_e(vs, e3);\
                    break;\
                      default: /*si ambos son dia de semana u otra cosa..*/\
                    if ((MONDAY<=e1->tag && e1->tag<=SUNDAY) && (MONDAY<=e2->tag && e2->tag<=SUNDAY) ){ \
                      e3 = malloc(sizeof(struct expr));\
                      e3->refs = 0;\
                      if (e1->tag relop e2->tag) {\
                        e3->tag = TRUE;\
                      } else {\
                        e3->tag = FALSE;\
                      }\
                      vs = vs_push_e(vs, e3);\
                    }\
                      }

            case EvLEQ:
				e2 = vs_pop_e(&vs);
				e1 = vs_pop_e(&vs);
				if ( e1->tag == STRING ) {
                    e3 = malloc( sizeof( struct expr ) );
                    e3->refs = 0;
                    if( strcmp(e1->e.name, e2->e.name) <= 0 ) {
                        e3->tag = TRUE;
                    }
                    else {
                        e3->tag = FALSE;
                    }
                    vs = vs_push_e( vs, e3 );
				} else {
					EV_COMP( <= );
				}
				unref_e(e1);
				unref_e(e2);
				break;

            case EvLESS:
				e2 = vs_pop_e(&vs);
				e1 = vs_pop_e(&vs);
				if ( e1->tag == STRING ) {
                    e3 = malloc( sizeof( struct expr ) );
                    e3->refs = 0;
                    if( strcmp(e1->e.name, e2->e.name) < 0 ) {
                        e3->tag = TRUE;
                    }
                    else {
                        e3->tag = FALSE;
                    }
                    vs = vs_push_e( vs, e3 );
				} else {
					EV_COMP( < );
				}
				unref_e(e1);
				unref_e(e2);
				break;

            case EvEQUAL:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
                switch( e1->tag ) {
                    case NUM:
                        e3 = malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        if( e1->e.num == e2->e.num ) {
                            e3->tag = TRUE;
                        }
                        else {
                            e3->tag = FALSE;
                        }
                        vs = vs_push_e( vs, e3 );
                        break;
                    case TIMEV:
                        e3 = malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        if( e1->e.time == e2->e.time ) {
                            e3->tag = TRUE;
                        }
                        else {
                            e3->tag = FALSE;
                        }
                        vs = vs_push_e( vs, e3 );
                        break;
                    case DATEV:
                        e3 = malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        if( e1->e.date == e2->e.date ) {
                            e3->tag = TRUE;
                        }
                        else {
                            e3->tag = FALSE;
                        }
                        vs = vs_push_e( vs, e3 );
                        break;
                    case STRING:
                        e3 = malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        if( strcmp(e1->e.name, e2->e.name) == 0 ) {
                            e3->tag = TRUE;
                        }
                        else {
                            e3->tag = FALSE;
                        }
                        vs = vs_push_e( vs, e3 );
                        break;
                    case TRUE:
                    case FALSE:
                        e3 = malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        e3->tag = ( e1->tag == e2->tag ? TRUE : FALSE );
                        vs = vs_push_e( vs, e3 );
                        break;
                    default:
                        /*si ambos son dia de semana u otra cosa..*/
                        if( ( MONDAY<= e1->tag && e1->tag <= SUNDAY )
                         && ( MONDAY<= e2->tag && e2->tag <= SUNDAY ) ) {
                            e3 = malloc( sizeof( struct expr ) );
                            e3->refs = 0;
                            if( e1->tag == e2->tag ) {
                                e3->tag = TRUE;
                            }
                            else {
                                e3->tag = FALSE;
                            }
                            vs = vs_push_e( vs, e3 );
                        }
                }
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvGREATER:
				e2 = vs_pop_e(&vs);
				e1 = vs_pop_e(&vs);
				if ( e1->tag == STRING ) {
                    e3 = malloc( sizeof( struct expr ) );
                    e3->refs = 0;
                    if( strcmp(e1->e.name, e2->e.name) > 0 ) {
                        e3->tag = TRUE;
                    }
                    else {
                        e3->tag = FALSE;
                    }
                    vs = vs_push_e( vs, e3 );
				} else {
					EV_COMP( > );
				}
				unref_e(e1);
				unref_e(e2);
				break;

            case EvGEQ:
				e2 = vs_pop_e(&vs);
				e1 = vs_pop_e(&vs);
				if ( e1->tag == STRING ) {
                    e3 = malloc( sizeof( struct expr ) );
                    e3->refs = 0;
                    if( strcmp(e1->e.name, e2->e.name) >= 0 ) {
                        e3->tag = TRUE;
                    }
                    else {
                        e3->tag = FALSE;
                    }
                    vs = vs_push_e( vs, e3 );
				} else {
					EV_COMP( >= );
				}
				unref_e(e1);
				unref_e(e2);
				break;

            case EvNEQ:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
                switch( e1->tag ) {
                    case NUM:
                        e3 = malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        if( e1->e.num != e2->e.num ) {
                            e3->tag = TRUE;
                        }
                        else {
                            e3->tag = FALSE;
                        }
                        vs = vs_push_e( vs, e3 );
                        break;
                    case TIMEV:
                        e3 = malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        if( e1->e.time != e2->e.time ) {
                            e3->tag = TRUE;
                        }
                        else {
                            e3->tag = FALSE;
                        }
                        vs = vs_push_e( vs, e3 );
                        break;
                    case DATEV:
                        e3 = malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        if( e1->e.date != e2->e.date ) {
                            e3->tag = TRUE;
                        }
                        else {
                            e3->tag = FALSE;
                        }
                        vs = vs_push_e( vs, e3 );
                        break;
                    case STRING:
                        e3 = malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        if( strcmp(e1->e.name, e2->e.name) != 0 ) {
                            e3->tag = TRUE;
                        }
                        else {
                            e3->tag = FALSE;
                        }
                        vs = vs_push_e( vs, e3 );
                        break;
                    case TRUE:
                    case FALSE:
                        e3 = malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        e3->tag = ( e1->tag != e2->tag ? TRUE : FALSE );
                        vs = vs_push_e( vs, e3 );
                        break;
                    default:
                        /*si ambos son dia de semana u otra cosa..*/
                        if( ( MONDAY<= e1->tag && e1->tag <= SUNDAY )
                         && ( MONDAY<= e2->tag && e2->tag <= SUNDAY ) ) {
                            e3 = malloc( sizeof( struct expr ) );
                            e3->refs = 0;
                            if( e1->tag != e2->tag ) {
                                e3->tag = TRUE;
                            }
                            else {
                                e3->tag = FALSE;
                            }
                            vs = vs_push_e( vs, e3 );
                        }
                }
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvECOND:
                e1 = vs_pop_e( &vs );
                e2 = vs_pop_e( &vs );
                e3 = vs_pop_e( &vs );
                if( e1->tag == TRUE ) {
                    vs = vs_push_e( vs, e2 );
                    cs = cs_push( cs, EvEXPR );
                }
                else {
                    // FALSE
                    vs = vs_push_e( vs, e3 );
                    cs = cs_push( cs, EvEXPR );
                }

                unref_e( e1 );
                unref_e( e2 );
                unref_e( e3 );
                break;

            case EvAND:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
                e3 = malloc( sizeof( struct expr ) );
                e3->refs = 0;
                e3->tag = ( e1->tag == FALSE ? FALSE : e2->tag );
                vs = vs_push_e( vs, e3 );
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvOR:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
                e3 = malloc( sizeof( struct expr ) );
                e3->refs = 0;
                e3->tag = ( e1->tag == TRUE ? TRUE : e2->tag );
                vs = vs_push_e( vs, e3 );
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvLOG:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
				recLog( e1->e.log.target, e2->e.name);
                e3 = malloc( sizeof( struct expr ) );
                e3->refs = 0;
                e3->tag = TRUE;
                vs = vs_push_e( vs, e3 );
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvSET:
                l1 = vs_pop_l( &vs );
                l2 = vs_pop_l( &vs );
                if( l1 == NULL ) {
                    e1 = ( expr )malloc( SIZE_OF_EXPR_SET );
                    e1->refs = 0;
                    e1->tag = SET;
                    e1->e.set = ref_l( l2 );
                    vs = vs_push_e( vs, e1 );
                }
                else {
                    vs = vs_push_l( vs, l1->tail );
                    vs = vs_push_l( vs, l2 );
                    vs = vs_push_e( vs, l1->head );
                    cs = cs_push( cs, EvSET );
                    cs = cs_push( cs, SWAP );
                    cs = cs_push( cs, LINK_SET );
                    cs = cs_push( cs, EvEXPR );
                }
                unref_l( l1 );
                unref_l( l2 );
                break;

            case EvAMSET:
                ml1 = vs_pop_ml( &vs );
                ml2 = vs_pop_ml( &vs );
                if( ml1 == NULL ) {
                    e1 = ( expr )malloc( SIZE_OF_EXPR_AMSET );
                    e1->refs = 0;
                    e1->tag = AMSET;
                    e1->e.amset = ref_ml( ml2 );
                    vs = vs_push_e( vs, e1 );
                }
                else {
                    vs = vs_push_ml( vs, ml1->tail );
                    vs = vs_push_ml( vs, ml2 );
                    vs = vs_push_e( vs, ml1->article );
                    vs = vs_push_e( vs, ml1->freq );
                    cs = cs_push( cs, EvAMSET );
                    cs = cs_push( cs, SWAP );
                    cs = cs_push( cs, LINK_AMSET );
                    cs = cs_push( cs, EvEXPR );
                    cs = cs_push( cs, SWAP );
                    cs = cs_push( cs, EvEXPR );
                }
                unref_ml( ml1 );
                unref_ml( ml2 );
                break;

            case EvPART:
                ps1 = vs_pop_p( &vs );
                ps2 = vs_pop_p( &vs );
                if( ps1 == NULL ) {
                    e1 = ( expr )malloc( SIZE_OF_EXPR_PART );
                    e1->refs = 0;
                    e1->tag = PART;
                    e1->e.part = ref_pl( ps2 );
                    vs = vs_push_e( vs, e1 );
                }
                else {
                    vs = vs_push_p( vs, ps1->tail );
                    vs = vs_push_p( vs, ps2 );
                    vs = vs_push_e( vs, ps1->rep );
                    vs = vs_push_e( vs, ps1->clase );
                    cs = cs_push( cs, EvPART );
                    cs = cs_push( cs, SWAP );
                    cs = cs_push( cs, LINK_PART );
                    cs = cs_push( cs, EvEXPR );
                    cs = cs_push( cs, SWAP );
                    cs = cs_push( cs, EvEXPR );
                }
                unref_pl( ps1 );
                unref_pl( ps2 );
                break;

            case LINK_SET:
                e1 = vs_pop_e( &vs );
                l1 = vs_pop_l( &vs );
                l2 = ( list )malloc( sizeof( struct node ) );
                l2->refs = 0;
                l2->head = ref_e( e1 );
                l2->tail = ref_l( l1 );
                vs = vs_push_l( vs, l2 );
                unref_e( e1 );
                unref_l( l1 );
                break;

            case LINK_AMSET:
                e1 = vs_pop_e( &vs );
                e2 = vs_pop_e( &vs );
                ml1 = vs_pop_ml( &vs );
                ml2 = ml1;

                if( e1->tag == CASH ) {
                    while( ml2 ) {
                        if( ml2->article->tag == CASH ) {
                            e3 = ( expr )malloc( sizeof( struct expr ) );
                            e3->refs = 0;
                            e3->tag = NUM;
                            e3->e.num = ml2->freq->e.num + e2->e.num;
                            unref_e( ml2->freq );
                            ml2->freq = ref_e( e3 );
                            break;
                        }
                        ml2 = ml2->tail;
                    }
                }
                else if( e1->tag == CTACTE ) {
                    while( ml2 ) {
                        if( ml2->article->tag == CTACTE ) {
                            e3 = ( expr )malloc( sizeof( struct expr ) );
                            e3->refs = 0;
                            e3->tag = NUM;
                            e3->e.num = ml2->freq->e.num + e2->e.num;
                            unref_e( ml2->freq );
                            ml2->freq = ref_e( e3 );
                            break;
                        }
                        ml2 = ml2->tail;
                    }
                }
                else if( e1->tag == CARD ) {
                    while( ml2 ) {
                        if( ( ml2->article->tag == CARD )
                         && ( idcmp( e1->e.card.name, ml2->article->e.card.name ) == 0 )
                         && ( idcmp( e1->e.card.plan, ml2->article->e.card.plan ) == 0 ) ) {
                            e3 = ( expr )malloc( sizeof( struct expr ) );
                            e3->refs = 0;
                            e3->tag = NUM;
                            e3->e.num = ml2->freq->e.num + e2->e.num;
                            unref_e( ml2->freq );
                            ml2->freq = ref_e( e3 );
                            break;
                        }
                        ml2 = ml2->tail;
                    }
                    /*
                         } else if (e1->tag==CARD){
                    while (ml2) {
                      if (ml2->article->tag != CASH && idcmp(e1->e.name,ml2->article->e.name)==0) {
                        e3 = (expr)malloc(sizeof(struct expr));
                        e3->refs = 0;
                        e3->tag = NUM;
                        e3->e.num = ml2->freq->e.num + e2->e.num;
                        unref_e(ml2->freq);
                        ml2->freq = ref_e(e3);
                        break;
                      }
                      ml2 = ml2->tail;
                    }
                    */
                }
                else if( e1->tag == MUTUAL ) {
                    while( ml2 ) {
                        if( ml2->article->tag == MUTUAL
                         && idcmp( e1->e.name, ml2->article->e.name ) == 0 ) {
                            e3 = ( expr )malloc( sizeof( struct expr ) );
                            e3->refs = 0;
                            e3->tag = NUM;
                            e3->e.num = ml2->freq->e.num + e2->e.num;
                            unref_e( ml2->freq );
                            ml2->freq = ref_e( e3 );
                            break;
                        }
                        ml2 = ml2->tail;
                    }
                }
                else if( e1->tag == TICKET ) {
                    while( ml2 ) {
                        if( ml2->article->tag == TICKET
                         && idcmp( e1->e.name, ml2->article->e.name ) == 0 ) {
                            e3 = ( expr )malloc( sizeof( struct expr ) );
                            e3->refs = 0;
                            e3->tag = NUM;
                            e3->e.num = ml2->freq->e.num + e2->e.num;
                            unref_e( ml2->freq );
                            ml2->freq = ref_e( e3 );
                            break;
                        }
                        ml2 = ml2->tail;
                    }
                }
                else if( e1->tag == CHEQUE ) {
                    while( ml2 ) {
                        if( ml2->article->tag == CHEQUE
                         && idcmp( e1->e.name, ml2->article->e.name ) == 0 ) {
                            e3 = ( expr )malloc( sizeof( struct expr ) );
                            e3->refs = 0;
                            e3->tag = NUM;
                            e3->e.num = ml2->freq->e.num + e2->e.num;
                            unref_e( ml2->freq );
                            ml2->freq = ref_e( e3 );
                            break;
                        }
                        ml2 = ml2->tail;
                    }
                }
                else if( e1->tag == CURRENCY ) {
                    while( ml2 ) {
                        if( ml2->article->tag == CURRENCY
                         && idcmp( e1->e.name, ml2->article->e.name ) == 0 ) {
                            e3 = ( expr )malloc( sizeof( struct expr ) );
                            e3->refs = 0;
                            e3->tag = NUM;
                            e3->e.num = ml2->freq->e.num + e2->e.num;
                            unref_e( ml2->freq );
                            ml2->freq = ref_e( e3 );
                            break;
                        }
                        ml2 = ml2->tail;
                    }
                }
                else {
                    while( ml2 ) {
                        if( idcmp( e1->e.name, ml2->article->e.name ) == 0 ) {
                            e3 = ( expr )malloc( sizeof( struct expr ) );
                            e3->refs = 0;
                            e3->tag = NUM;
                            e3->e.num = ml2->freq->e.num + e2->e.num;
                            unref_e( ml2->freq );
                            ml2->freq = ref_e( e3 );
                            break;
                        }
                        ml2 = ml2->tail;
                    }
                }

                if( ml2 == NULL ) {
                    ml2 = ( mlist )malloc( sizeof( struct mnode ) );
                    ml2->refs = 0;
                    ml2->article = ref_e( e1 );
                    ml2->freq = ref_e( e2 );
                    ml2->tail = ref_ml( ml1 );
                    vs = vs_push_ml( vs, ml2 );
                }
                else {
                    vs = vs_push_ml( vs, ml1 );
                }

                unref_e( e1 );
                unref_e( e2 );
                unref_ml( ml1 );
                break;

            case LINK_PART:
                e1 = vs_pop_e( &vs );
                e2 = vs_pop_e( &vs );
                ps1 = vs_pop_p( &vs );

                ps2 = ( part )malloc( sizeof( struct pnode ) );
                ps2->refs = 0;
                ps2->rep = ref_e( e1 );
                ps2->clase = ref_e( e2 );
                ps2->tail = ref_pl( ps1 );
                vs = vs_push_p( vs, ps2 );

                unref_e( e1 );
                unref_e( e2 );
                unref_pl( ps1 );
                break;


            case EvINTERVAL:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
                e3 = malloc( sizeof( struct expr ) );
                e3->refs = 0;
                e3->tag = INTERVAL;
                e3->e.interval.e0 = ref_e( e1 );
                e3->e.interval.e1 = ref_e( e2 );
                vs = vs_push_e( vs, e3 );
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvUNION:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
                /* BUG semantica de ALL 1-10-2003
                if ((e1->tag == ALL) || (e2->tag == ALL)) {
                e3 = malloc(sizeof(struct expr));
                e3->refs = 0;
                e3->tag = ALL;
                vs = vs_push_e(vs, e3);
                } else */

                if( ( e1->tag == AMSET ) && ( e2->tag == AMSET ) ) {
                    ml1 = e1->e.amset; 
                    ml2 = e2->e.amset;

                    ml3 = NULL; 
                    while( ml1 ) {
                        ml4 = ( mlist )malloc( sizeof( struct mnode ) );
                        ml4->refs = 0;
                        ml4->article = ref_e( ml1->article );
                        ml4->freq = ref_e( ml1->freq );
                        ml4->tail = ref_ml( ml3 );
                        ml3 = ml4;
                        ml1 = ml1->tail;
                    }

                    ml1 = ml3; 

                    while( ml2 ) {
                        int done = 0; 
                        ml3 = ml1;
                        while( ml3 && !done ) {
                            if( idcmp( ml2->article->e.name, ml3->article->e.name ) == 0 ) {
                                e4 = ( expr )malloc( sizeof( struct expr ) ); 
                                e4->refs = 0;
                                e4->tag = NUM;
                                #ifndef INVEL_W
                                #ifndef __MSDOS__
                                #ifndef WIN32
                                #define max(a,b) ((a)<(b)?b:a)
                                #endif //WIN332
                                #endif
                                #endif
                                e4->e.num = max( ml2->freq->e.num, ml3->freq->e.num ); 
                                unref_e( ml3->freq );
                                ml3->freq = ref_e( e4 );
                                done = 1;
                            }
                            ml3 = ml3->tail;
                        }
                        if( !done ) {
                            ml3 = ( mlist )malloc( sizeof( struct mnode ) );
                            ml3->refs = 0;
                            ml3->article = ref_e( ml2->article );
                            ml3->freq = ref_e( ml2->freq );
                            ml3->tail = ref_ml( ml1 );
                            ml1 = ml3;
                        }
                        ml2 = ml2->tail;
                    }

                    e3 = ( expr )malloc( SIZE_OF_EXPR_AMSET );
                    e3->refs = 0;
                    e3->tag = AMSET;
                    e3->e.amset = ref_ml( ml1 );

                    vs = vs_push_e( vs, e3 );
                }
                else {
                    e3 = malloc( sizeof( struct expr ) );
                    e3->refs = 0;
                    e3->tag = UNION;
                    e3->e.binop.e0 = ref_e( e1 );
                    e3->e.binop.e1 = ref_e( e2 );
                    vs = vs_push_e( vs, e3 );
                }

                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvINTERSECTION:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );

                if( ( e1->tag == AMSET ) && ( e2->tag == AMSET ) ) {
                    ml1 = e1->e.amset;
                    ml2 = e2->e.amset;

                    ml3 = NULL;

                    while( ml1 ) {
                        while( ml2 ) {
                            if( idcmp( ml1->article->e.name, ml2->article->e.name ) == 0 ) {
                                ml4 = ( mlist )malloc( sizeof( struct mnode ) );
                                ml4->refs = 0;
                                ml4->article = ref_e( ml1->article );
                                ml4->tail = ref_ml( ml3 );

                                if( ml1->freq->e.num < ml2->freq->e.num ) {
                                    ml4->freq = ref_e( ml1->freq );
                                }
                                else {
                                    ml4->freq = ref_e( ml2->freq );
                                }
                                ml3 = ml4;
                                break;
                            }
                            ml2 = ml2->tail;
                        }
                        ml2 = e2->e.amset;
                        ml1 = ml1->tail;
                    }

                    e3 = ( expr )malloc( SIZE_OF_EXPR_AMSET );
                    e3->refs = 0;
                    e3->tag = AMSET;
                    e3->e.amset = ref_ml( ml3 );

                    vs = vs_push_e( vs, e3 );

                    /* BUG semantica de ALL 1-10-2003
                         } else if (e1->tag == ALL) {
                    vs = vs_push_e(vs, e2);
                         } else if (e2->tag == ALL) {
                    vs = vs_push_e(vs, e1);
                    */
                }
                else {
                    e3 = malloc( sizeof( struct expr ) );
                    e3->refs = 0;
                    e3->tag = INTERSECTION;
                    e3->e.binop.e0 = ref_e( e1 );
                    e3->e.binop.e1 = ref_e( e2 );
                    vs = vs_push_e( vs, e3 );
                }
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvIN:
                e2 = vs_pop_e( &vs );
                e1 = vs_pop_e( &vs );
                switch( e1->tag ) {
                    case MONDAY:
                    case TUESDAY:
                    case WEDNESDAY:
                    case THURSDAY:
                    case FRIDAY:
                    case SATURDAY:
                    case SUNDAY:
                        switch( e2->tag ) {
                            case SET:
                                l1 = e2->e.set;
                                e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                                e3->refs = 0;
                                e3->tag = FALSE;
                                while( l1 ) {
                                    if( e1->tag == l1->head->tag ) {
                                        e3->tag = TRUE;
                                        break;
                                    }
                                    l1 = l1->tail;
                                }
                                vs = vs_push_e( vs, e3 );
                                unref_e( e1 );
                                unref_e( e2 );
                                break;
                            case INTERVAL:
                                e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                                e3->refs = 0;
                                if( ( e1->tag >= e2->e.interval.e0->tag )
                                 && ( e1->tag <= e2->e.interval.e1->tag ) ) {
                                    e3->tag = TRUE;
                                }
                                else {
                                    e3->tag = FALSE;
                                }
                                vs = vs_push_e( vs, e3 );
                                unref_e( e1 );
                                unref_e( e2 );
                                break;

                            default:
                                unref_e( e1 );
                                unref_e( e2 );
                                p_error( ST( S_EVIN__DAY__ ) );
                                return ABNORM;
                        }
                        break;

                    case CASH:
                        l1 = e2->e.set;
                        e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e3->refs = 0;
                        e3->tag = FALSE;
                        while( l1 ) {
                            if( l1->head->tag == CASH ) {
                                e3->tag = TRUE;
                                break;
                            }
                            l1 = l1->tail;
                        }
                        vs = vs_push_e( vs, e3 );
                        unref_e( e1 );
                        unref_e( e2 );
                        break;

                    case CTACTE:
                        l1 = e2->e.set;
                        e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e3->refs = 0;
                        e3->tag = FALSE;
                        while( l1 ) {
                            if( l1->head->tag == CTACTE ) {
                                e3->tag = TRUE;
                                break;
                            }
                            l1 = l1->tail;
                        }
                        vs = vs_push_e( vs, e3 );
                        unref_e( e1 );
                        unref_e( e2 );
                        break;

                    case CARD:
                        l1 = e2->e.set;
                        e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e3->refs = 0;
                        e3->tag = FALSE;
                        while( l1 ) {
                            if( l1->head->tag == CARD ) {
                                if( ( idcmp( e1->e.card.name, l1->head->e.card.name ) == 0 )
                                 && ( idcmp( e1->e.card.plan, l1->head->e.card.plan ) == 0 ) ) {
                                    e3->tag = TRUE;
                                    break;
                                }
                            }
                            l1 = l1->tail;
                        }
                        vs = vs_push_e( vs, e3 );
                        unref_e( e1 );
                        unref_e( e2 );
                        break;

                    case MUTUAL:
                        l1 = e2->e.set;
                        e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e3->refs = 0;
                        e3->tag = FALSE;
                        while( l1 ) {
                            if( l1->head->tag == MUTUAL ) {
                                if( idcmp( e1->e.name, l1->head->e.name ) == 0 ) {
                                    e3->tag = TRUE;
                                    break;
                                }
                            }
                            l1 = l1->tail;
                        }
                        vs = vs_push_e( vs, e3 );
                        unref_e( e1 );
                        unref_e( e2 );
                        break;

                    case TICKET:
                        l1 = e2->e.set;
                        e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e3->refs = 0;
                        e3->tag = FALSE;
                        while( l1 ) {
                            if( l1->head->tag == TICKET ) {
                                if( idcmp( e1->e.name, l1->head->e.name ) == 0 ) {
                                    e3->tag = TRUE;
                                    break;
                                }
                            }
                            l1 = l1->tail;
                        }
                        vs = vs_push_e( vs, e3 );
                        unref_e( e1 );
                        unref_e( e2 );
                        break;

                    case CHEQUE:
                        l1 = e2->e.set;
                        e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e3->refs = 0;
                        e3->tag = FALSE;
                        while( l1 ) {
                            if( l1->head->tag == CHEQUE ) {
                                if( idcmp( e1->e.name, l1->head->e.name ) == 0 ) {
                                    e3->tag = TRUE;
                                    break;
                                }
                            }
                            l1 = l1->tail;
                        }
                        vs = vs_push_e( vs, e3 );
                        unref_e( e1 );
                        unref_e( e2 );
                        break;

                    case CURRENCY:
                        l1 = e2->e.set;
                        e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e3->refs = 0;
                        e3->tag = FALSE;
                        while( l1 ) {
                            if( l1->head->tag == CURRENCY ) {
                                if( idcmp( e1->e.name, l1->head->e.name ) == 0 ) {
                                    e3->tag = TRUE;
                                    break;
                                }
                            }
                            l1 = l1->tail;
                        }
                        vs = vs_push_e( vs, e3 );
                        unref_e( e1 );
                        unref_e( e2 );
                        break;

                    case DATEV:
                        e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e3->refs = 0;
                        if( ( e1->e.date >= e2->e.interval.e0->e.date )
                         && ( e1->e.date <= e2->e.interval.e1->e.date ) ) {
                            e3->tag = TRUE;
                        }
                        else {
                            e3->tag = FALSE;
                        }
                        vs = vs_push_e( vs, e3 );
                        unref_e( e1 );
                        unref_e( e2 );
                        break;

                    case TIMEV:
                        e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e3->refs = 0;
                        if( ( e1->e.time >= e2->e.interval.e0->e.time )
                         && ( e1->e.time <= e2->e.interval.e1->e.time ) ) {
                            e3->tag = TRUE;
                        }
                        else {
                            e3->tag = FALSE;
                        }
                        vs = vs_push_e( vs, e3 );
                        unref_e( e1 );
                        unref_e( e2 );
                        break;

                    case NUM:
                        e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e3->refs = 0;
                        if( ( e1->e.num >= e2->e.interval.e0->e.num )
                         && ( e1->e.num <= e2->e.interval.e1->e.num ) ) {
                            e3->tag = TRUE;
                        }
                        else {
                            e3->tag = FALSE;
                        }
                        vs = vs_push_e( vs, e3 );
                        unref_e( e1 );
                        unref_e( e2 );
                        break;

                    default:
                        unref_e( e1 );
                        unref_e( e2 );
                        p_error( ST( S_EVIN__ALGO_NO_CONTEMPLADO_ ) );
                        return ABNORM;
                }

                break;

            case EvMEET:
                ml1 = vs_pop_ml( &vs );
                e1 = vs_pop_e( &vs );
                ml2 = vs_pop_ml( &vs ); // el que se construye (zs)
                ml3 = vs_pop_ml( &vs );     // 21-04-06 nueva implementacion - el original del lado izquierdo del meet

                if( ml1 == NULL ) {
                    e2 = ( expr )malloc( SIZE_OF_EXPR_AMSET );
                    e2->refs = 0;
                    e2->tag = AMSET;
                    e2->e.amset = ref_ml( ml2 );
                    vs = vs_push_e( vs, e2 );
                }
                else {
                    vs = vs_push_ml( vs, ml3 );
                    vs = vs_push_ml( vs, ml2 );
                    vs = vs_push_e( vs, e1 );
                    vs = vs_push_ml( vs, ml1 );
                    vs = vs_push_ml( vs, ml1 );
                    vs = vs_push_e( vs, e1 );

                    cs = cs_push( cs, EvMEET );
                    cs = cs_push( cs, CHANFLE );
                    cs = cs_push( cs, BELONGS );
                }
                unref_ml( ml1 );
                unref_e( e1 );
                unref_ml( ml2 );
                unref_ml( ml3 );        // 21-04-06 nueva implementacion    
                break;


            case EvCONCAT:
                e1 = vs_pop_e( &vs );
                e2 = vs_pop_e( &vs );
                e3 = ( expr )malloc( SIZE_OF_EXPR_IDENT );
                e3->tag = STRING;
                e3->refs = 0;
                if( e1->tag == STRING ) {
                    index1 = strlen( e1->e.name );
                    if( e2->tag == STRING ) {
                        index2 = strlen( e2->e.name );
                        e3->e.name = ( identifier )malloc( index1 + index2 + 1 );
                        sprintf( e3->e.name, "%s%s", e1->e.name, e2->e.name );
                        //strcpy(e3->e.name, e1->e.name);
                        //strcpy(e3->e.name+index1, e2->e.name);
                        //e3->e.name[index1+index2] = '\0';
                    }
                    else {
		      char *s = expr2str(e2);
		      int len = index1 + strlen(s) + 1;
		      e3->e.name = ( identifier )malloc( len );
		      memset( e3->e.name, ( int )'\0', len );
		      sprintf(e3->e.name, "%s%s", e1->e.name, s);
		      free(s);		      
		    }
                }
                else {
		  char *s = expr2str(e1);
		  int len = index1 + strlen(s) + 1;
		  e3->e.name = ( identifier )malloc( len );
		  memset( e3->e.name, ( int )'\0', len );
		  sprintf(e3->e.name, "%s%s", e2->e.name, s);
		  free(s);		      
                }
                vs = vs_push_e( vs, e3 );
                unref_e( e1 );
                unref_e( e2 );
                break;

            case EvINPUT:
                e1 = vs_pop_e( &vs );
                e2 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e2->refs = 0;
                if( _input( e1->e.name ) ) {
                    e2->tag = TRUE;
                }
                else {
                    e2->tag = FALSE;
                }
                vs = vs_push_e( vs, e2 );
                unref_e( e1 );
                break;

            case EvNUMINPUT:
                e1 = vs_pop_e( &vs );
                e2 = ( expr )malloc( sizeof( struct expr ) );
                e2->refs = 0;
                e2->tag = NUM;
                e2->e.num = numinput( e1->e.name );
                vs = vs_push_e( vs, e2 );
                unref_e( e1 );
                break;

            case EvCOMPLEMENT:
                e1 = vs_pop_e( &vs );
                switch( e1->tag ) {
                    case NUM:
                        e3 = ( expr )malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        e3->tag = NUM;
                        e3->e.num = -e1->e.num;
                        vs = vs_push_e( vs, e3 );
                        break;
                    case TRUE:
                        e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e3->refs = 0;
                        e3->tag = FALSE;
                        vs = vs_push_e( vs, e3 );
                        break;
                    case FALSE:
                        e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e3->refs = 0;
                        e3->tag = TRUE;
                        vs = vs_push_e( vs, e3 );
                        break;
                    case AMSET:
                    case UNION:
                    case INTERSECTION:
                    case DEPARTMENT:
                    case CLASS:
                    case ALL:
                        e3 = ( expr )malloc( SIZE_OF_EXPR_UNOP );
                        e3->refs = 0;
                        e3->tag = COMPLEMENT;
                        e3->e.unop = ref_e( e1 );
                        vs = vs_push_e( vs, e3 );
                        break;
                    case COMPLEMENT:
                        vs = vs_push_e( vs, e1->e.unop );
                        break;
                    default:
                        unref_e( e1 );
                        p_error( ST( S_EVCOMPLEMENT__CASO_NO_CONTEMPLADO ) );
                        return ABNORM;
                }
                unref_e( e1 );
                break;

            case EvROUND:
                e1 = vs_pop_e( &vs );
                e3 = ( expr )malloc( sizeof( struct expr ) );
                e3->refs = 0;
                e3->tag = NUM;
                e3->e.num = round2( e1->e.num, TOLERANCE );
                vs = vs_push_e( vs, e3 );
                unref_e( e1 );
                break;

            case EvTRUNC:
                e1 = vs_pop_e( &vs );
                if( e1->tag == NUM ) {
                    e3 = ( expr )malloc( sizeof( struct expr ) );
                    e3->refs = 0;
                    e3->tag = NUM;
                    //e3->e.num = round2(e1->e.num,TOLERANCE);
                    e3->e.num = ( double )( int )e1->e.num;
                }
                else if( e1->tag == ARTICLE ) {
                    // se trata de un articulo
                    e3 = ( expr )malloc( sizeof( struct expr ) );
                    e3->refs = 0;
                    e3->tag = ARTICLE;

                    if( e1->e.name[0] == 'j' ) {
                        int h;
                        e3->e.name = ( identifier )malloc( strlen( e1->e.name ) + 1 );
                        e3->e.name[0] = 'i';
                        for( h = 1;h < (int)strlen( e1->e.name );h++ ) {
                            if( e1->e.name[h] != '$' ) {
                                e3->e.name[h] = e1->e.name[h];
                            }
                            else {
                                e3->e.name[h] = '\0';
                                break;
                            }
                        }
                    }
                    else {
                        e3->e.name = ( identifier )malloc( strlen( e1->e.name ) + 1 );
                        strcpy( e3->e.name, e1->e.name );
                        e3->e.name[strlen( e1->e.name )] = '\0';
                    }
                }
                else {
                    glog(
                    "EVALUADOR DE PROMOCIONES: ERROR: TRUNC de una expresion que no es un ARTICLE" ,LOG_MOTOR_PROMO,3);
                    exit( 1 );
                }
                vs = vs_push_e( vs, e3 );
                unref_e( e1 );
                break;

            case EvAMOUNT:
                e1 = vs_pop_e( &vs );
                e3 = ( expr )malloc( sizeof( struct expr ) );
                e3->refs = 0;
                e3->tag = NUM;
                e3->e.num = 0;
                ml1 = e1->e.amset;
                while( ml1 ) {
                    e3->e.num += amount( ml1->article->e.name, ml1->freq->e.num );
                    ml1 = ml1->tail;
                }
                vs = vs_push_e( vs, e3 );
                unref_e( e1 );
                break;

            case EvSIZE:
                e1 = vs_pop_e( &vs );
                switch( e1->tag ) {
                    case AMSET:
                        e3 = ( expr )malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        e3->tag = NUM;
                        e3->e.num = 0;
                        ml1 = e1->e.amset;
                        while( ml1 ) {
                            e3->e.num++;
                            ml1 = ml1->tail;
                        }
                        vs = vs_push_e( vs, e3 );
                        break;

                    default:
                        unref_e( e1 );
                        p_error( ST( S_EVSIZE__CASO_NO_CONTEMPLADO ) );
                        return ABNORM;
                }
                unref_e( e1 );
                break;

            case EvLENGTH:
                e1 = vs_pop_e( &vs );
                switch( e1->tag ) {
                    case AMSET:
                        e3 = ( expr )malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        e3->tag = NUM;
                        e3->e.num = 0;
                        ml1 = e1->e.amset;
                        while( ml1 ) {
                            e3->e.num += ml1->freq->e.num;
                            ml1 = ml1->tail;
                        }
                        vs = vs_push_e( vs, e3 );
                        break;


                    default:
                        unref_e( e1 );
                        p_error( ST( S_EVLENGTH__CASO_NO_CONTEMPLADO ) );
                        return ABNORM;
                }
                unref_e( e1 );
                break;

            case EvCOUNTABLE:
                e1 = vs_pop_e( &vs );
                e2 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e2->refs = 0;
                e2->tag = ( countable( e1->e.name ) == 0 ? FALSE : TRUE );
                vs = vs_push_e( vs, e2 );
                unref_e( e1 );
                break;

            case EvDOT:
                e1 = vs_pop_e( &vs );
                e2 = vs_pop_e( &vs );
                switch( e2->tag ) {
                    case AMSET:
                        e3 = ( expr )malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        e3->tag = NUM;
                        e3->e.num = 0;
                        ml1 = e2->e.amset;

                        if( e1->tag == CASH ) {
                            while( ml1 ) {
                                if( ml1->article->tag == CASH ) {
                                    e3->e.num += ml1->freq->e.num;
                                    break;
                                }
                                ml1 = ml1->tail;
                            }
                        }
                        else if( e1->tag == CTACTE ) {
                            while( ml1 ) {
                                if( ml1->article->tag == CTACTE ) {
                                    e3->e.num += ml1->freq->e.num;
                                    break;
                                }
                                ml1 = ml1->tail;
                            }
                        }
                        else if( e1->tag == CARD ) {
                        	int cName = strcmp(e1->e.card.name, "null");
                        	int cPlan = strcmp(e1->e.card.plan, "null");
                            while( ml1 ) {
                                if( ( ml1->article->tag == CARD )
                                 && ( ( !cName ) || ( idcmp( e1->e.card.name, ml1->article->e.card.name ) == 0 ) )
                                 && ( ( !cPlan ) || ( idcmp( e1->e.card.plan, ml1->article->e.card.plan ) == 0 ) ) ) {
                                    e3->e.num += ml1->freq->e.num;
                                    if ( cName && cPlan) {
                                    	break;
                                    }
                                }
                                ml1 = ml1->tail;
                            }
                            /*
                            } else if (e1->tag==CARD){
                              while (ml1) {
                                if (ml1->article->tag != CASH && idcmp(e1->e.name,ml1->article->e.name)==0) {
                                  e3->e.num += ml1->freq->e.num;
                                  break;
                                }
                                ml1 = ml1->tail;
                              }
                               */
                        }
                        else if( e1->tag == MUTUAL ) {
                        	int cName = strcmp(e1->e.name, "null");
                            while( ml1 ) {
                                if( ( ml1->article->tag == MUTUAL )
                                 && ( ( !cName ) || ( idcmp( e1->e.name, ml1->article->e.name ) == 0 ) ) ) {
                                 //&& idcmp( e1->e.name, ml1->article->e.name ) == 0 ) {
                                    e3->e.num += ml1->freq->e.num;
                                    if ( cName ) {
                                    	break;
                                    }
                                }
                                ml1 = ml1->tail;
                            }
                        }
                        else if( e1->tag == TICKET ) {
                        	int cName = strcmp(e1->e.name, "null");
                            while( ml1 ) {
                                if( ( ml1->article->tag == TICKET )
                                 && ( ( !cName ) || ( idcmp( e1->e.name, ml1->article->e.name ) == 0 ) ) ) {
                                 //&& idcmp( e1->e.name, ml1->article->e.name ) == 0 ) {
                                    e3->e.num += ml1->freq->e.num;
                                    if ( cName ) {
                                    	break;
                                    }
                                }
                                ml1 = ml1->tail;
                            }
                        }
                        else if( e1->tag == CHEQUE ) {
                        	int cName = strcmp(e1->e.name, "null");
                            while( ml1 ) {
                                if( ( ml1->article->tag == CHEQUE )
                                 && ( ( !cName ) || ( idcmp( e1->e.name, ml1->article->e.name ) == 0 ) ) ) {
                                 //&& idcmp( e1->e.name, ml1->article->e.name ) == 0 ) {
                                    e3->e.num += ml1->freq->e.num;
                                    if ( cName ) {
                                    	break;
                                    }
                                }
                                ml1 = ml1->tail;
                            }
                        }
                        else if( e1->tag == CURRENCY ) {
                        	int cName = strcmp(e1->e.name, "null");
                            while( ml1 ) {
                                if( ml1->article->tag == CURRENCY
                                 && ( ( !cName ) || ( idcmp( e1->e.name, ml1->article->e.name ) == 0 ) ) ) {
                                 //&& idcmp( e1->e.name, ml1->article->e.name ) == 0 ) {
                                    e3->e.num += ml1->freq->e.num;
                                    if ( cName ) {
                                    	break;
                                    }
                                }
                                ml1 = ml1->tail;
                            }
                        }
                        else if( e1->tag == POINT ) {
                            while( ml1 ) {
                                if( ml1->article->tag == POINT
                                 && idcmp( e1->e.name, ml1->article->e.name ) == 0 ) {
                                    e3->e.num += ml1->freq->e.num;
                                    break;
                                }
                                ml1 = ml1->tail;
                            }
                        }
                        else {
                            while( ml1 ) {
                                if( idcmp( e1->e.name, ml1->article->e.name ) == 0 ) {
                                    e3->e.num += ml1->freq->e.num;
                                    break;
                                }
                                ml1 = ml1->tail;
                            }
                        }
                        vs = vs_push_e( vs, e3 );

                        break;

                    case DATEV:
                        e3 = ( expr )malloc( sizeof( struct expr ) );
                        e3->refs = 0;
                        e3->tag = NUM;
                        e3->e.num = 0;
                        if( e1->tag == DAYSELECTOR ) {
                            char buf[11];
                            int y,m,d;
                            if( strdate( buf, 11, e2->e.date, 0 ) == 0 ) {
                                sscanf( buf, "%d-%d-%d", &y, &m, &d );
                                e3->e.num = d;
                            }
                            else {
                                e3->e.num = 0;
                            }
                        }
                        else if( e1->tag == MONTHSELECTOR ) {
                            char buf[11];
                            int y,m,d;
                            if( strdate( buf, 11, e2->e.date, 0 ) == 0 ) {
                                sscanf( buf, "%d-%d-%d", &y, &m, &d );
                                e3->e.num = m;
                            }
                            else {
                                e3->e.num = 0;
                            }
                        }
                        else if( e1->tag == YEARSELECTOR ) {
                            char buf[11];
                            int y,m,d;
                            if( strdate( buf, 11, e2->e.date, 0 ) == 0 ) {
                                sscanf( buf, "%d-%d-%d", &y, &m, &d );
                                e3->e.num = y;
                            }
                            else {
                                e3->e.num = 0;
                            }
                        }
                        else {
                            unref_e( e1 );
                            unref_e( e2 );
                            p_error( ST( S_EVBYMEANS___ESTO_NO_DEBERIA_PASAR_ ) );
                            return ABNORM;
                        }
                        vs = vs_push_e( vs, e3 );
                        break;

                    case PART:
                        {
                            int fin = 0;
                            ps1 = e2->e.part;
                            while( ( ps1 != NULL ) && ( !fin ) ) {
                                l1 = ps1->clase->e.set;
                                while( ( l1 != NULL ) && ( !fin ) ) {
                                    if( idcmp( l1->head->e.name, e1->e.name ) == 0 ) {
                                        vs = vs_push_e( vs, ps1->rep );
                                        fin = 1;
                                    }
                                    l1 = l1->tail;
                                }
                                ps1 = ps1->tail;
                            }
                            if( !fin ) {
                                vs = vs_push_e( vs, e1 );
                            }
                        }
                        break;

                    default:
                        unref_e( e1 );
                        unref_e( e2 );
                        p_error( ST( S_EVBYMEANS___ESTO_NO_DEBERIA_PASAR_ ) );
                        return ABNORM;
                }
                unref_e( e1 );
                unref_e( e2 );
                break;


            case BELONGS:
                e1 = vs_pop_e( &vs );
                ml1 = vs_pop_ml( &vs );
                switch( e1->tag ) {
                    case ALL:
                        e2 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e2->refs = 0;
                        e2->tag = ( ml1->freq->e.num >= 1 ? TRUE : FALSE );
                        vs = vs_push_e( vs, e2 );
                        break;

                    case UNION:
                        vs = vs_push_ml( vs, ml1 );
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_ml( vs, ml1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );

                        cs = cs_push( cs, IF_BELONG_UNION );
                        cs = cs_push( cs, BELONGS );

                        break;

                    case INTERSECTION:
                        vs = vs_push_ml( vs, ml1 );
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        vs = vs_push_ml( vs, ml1 );
                        vs = vs_push_e( vs, e1->e.binop.e0 );

                        cs = cs_push( cs, IF_BELONG_INTERSECTION );
                        cs = cs_push( cs, BELONGS );
                        break;

                    case COMPLEMENT:
                        vs = vs_push_ml( vs, ml1 );
                        vs = vs_push_e( vs, e1->e.unop );
                        cs = cs_push( cs, EvCOMPLEMENT );
                        cs = cs_push( cs, BELONGS );
                        break;

                    case DEPARTMENT:
                        e2 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e2->refs = 0;
                        e2->tag = ( inDepartment( ml1->article->e.name, e1->e.name )
                                 && ( ml1->freq->e.num >= 1 ) ? TRUE : FALSE );
                        vs = vs_push_e( vs, e2 );
                        break;

                    case CLASS:
                        e2 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                        e2->refs = 0;
                        e2->tag = ( inClass( ml1->article->e.name, e1->e._class.nivel,
                                             e1->e._class.concept ) && ( ml1->freq->e.num >= 1 )
                                  ? TRUE : FALSE );
                        vs = vs_push_e( vs, e2 );
                        break;

                    case AMSET:
                        ml2 = e1->e.amset;
                        while( ml2 ) {
                            if( ( ml1->article->e.name ) && ( ml1->article->e.name[0] == 'j' ) ) {
                                mlist ml5 = NULL;
                                double n = 0;
                                ml3 = vs_pop_ml( &vs );
                                e2 = vs_pop_e( &vs );
                                ml4 = vs_pop_ml( &vs ); // el que se construye (zs)
                                ml5 = vs_pop_ml( &vs );     // 21-04-06 nueva implementacion - el original del lado izquierdo del meet
                                // contar la cantidad total de articulos jxxx en ml3, y si es >= a 
                                // la cantidad de ixxx en e1, porner true, sino NADA.
                                {
                                    mlist ml = ml5;
                                    while( ml != NULL ) {
                                        //          if (idcmp(ml->article->e.name,ml1->article->e.name) == 2) {
                                        if( ( idcmp( ml->article->e.name, ml1->article->e.name )
                                           == 2 )
                                         || ( idcmp( ml->article->e.name, ml1->article->e.name )
                                           == 0 ) ) {
                                            n += ( ml->freq->e.num );
                                        }
                                        ml = ml->tail;
                                    }
                                }
                                vs = vs_push_ml( vs, ml5 );
                                vs = vs_push_ml( vs, ml4 );
                                vs = vs_push_e( vs, e2 );
                                vs = vs_push_ml( vs, ml3 );
                                unref_ml( ml5 );
                                unref_ml( ml4 );
                                unref_e( e2 );
                                unref_ml( ml3 );

                                if( ( ( ml2->article->e.name[0] == 'j' )
                                   && ( idcmp( ml2->article->e.name, ml1->article->e.name ) == 0 ) )
                                 && ( ml2->freq->e.num <= ml1->freq->e.num ) ) {
                                    e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                                    e3->refs = 0;
                                    e3->tag = TRUE;
                                    vs = vs_push_e( vs, e3 );
                                    break;
                                }
                                else if( ( idcmp( ml2->article->e.name, ml1->article->e.name )
                                        == 3 ) && ( ml2->freq->e.num <= n ) ) {
                                    e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                                    e3->refs = 0;
                                    e3->tag = TRUE;
                                    vs = vs_push_e( vs, e3 );
                                    break;
                                }
                            }
                            else if( ( ml1->article->tag != CASH ) && ( ml2->article->tag != CASH )
                                  && ( idcmp( ml2->article->e.name, ml1->article->e.name ) == 0 )
                                  && ( ml2->freq->e.num <= ml1->freq->e.num ) ) {
                                e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                                e3->refs = 0;
                                e3->tag = TRUE;
                                vs = vs_push_e( vs, e3 );
                                break;
                            }
                            else if( ( ml1->article->tag == CASH ) && ( ml2->article->tag == CASH )
                                  && ( ml2->freq->e.num <= ml1->freq->e.num ) ) {
                                e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                                e3->refs = 0;
                                e3->tag = TRUE;
                                vs = vs_push_e( vs, e3 );
                                break;
                            }
                            /////////////////// FALTA CONSIDERAR LOS OTROS MEDIOS DE PAGO
                            ml2 = ml2->tail;
                        };
                        if( !ml2 ) {
                            e3 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                            e3->refs = 0;
                            e3->tag = FALSE;
                            vs = vs_push_e( vs, e3 );
                        }
                        break;


                    case MULT:
                        e2 = ( expr )malloc( sizeof( struct expr ) );
                        e2->refs = 0;
                        e2->tag = NUM;
                        e2->e.num = ( ml1->freq->e.num ) / ( e1->e.binop.e0->e.num ) ;
                        ml2 = ( mlist )malloc( sizeof( struct mnode ) );
                        ml2->refs = 0;
                        ml2->article = ref_e( ml1->article );
                        ml2->freq = ref_e( e2 );
                        ml2->tail = NULL;
                        vs = vs_push_ml( vs, ml2 );
                        vs = vs_push_e( vs, e1->e.binop.e1 );
                        cs = cs_push( cs, BELONGS );
                        break;

                    default:
                        unref_e( e1 );
                        unref_ml( ml1 );
                        p_error( ST( S_BELONGS__CASO_NO_CONTEMPLADO__ ) );
                        return ABNORM;
                }

                unref_e( e1 );
                unref_ml( ml1 );
                break;


            case IF_BELONG_UNION:
                e1 = vs_pop_e( &vs );
                if( e1->tag == FALSE ) {
                    cs = cs_push( cs, BELONGS );
                }
                else {
                    e2 = vs_pop_e( &vs );
                    ml1 = vs_pop_ml( &vs );
                    vs = vs_push_e( vs, e1 );
                    unref_e( e2 );
                    unref_ml( ml1 );
                }
                unref_e( e1 );
                break;

            case IF_BELONG_INTERSECTION:
                e1 = vs_pop_e( &vs );
                if( e1->tag == TRUE ) {
                    cs = cs_push( cs, BELONGS );
                }
                else {
                    e2 = vs_pop_e( &vs );
                    ml1 = vs_pop_ml( &vs );
                    vs = vs_push_e( vs, e1 );
                    unref_e( e2 );
                    unref_ml( ml1 );
                }
                unref_e( e1 );
                break;

            case RESTE:
                e2 = vs_pop_e( &vs );
                ml1 = vs_pop_ml( &vs );
                if( ml1 == NULL ) {
                    ml3 = vs_pop_ml( &vs );
                    e3 = ( expr )malloc( SIZE_OF_EXPR_AMSET );
                    e3->refs = 0;
                    e3->tag = AMSET;
                    e3->e.amset = ref_ml( ml3 );
                    vs = vs_push_e( vs, e3 );
                    unref_ml( ml3 );
                }
                else {
                    vs = vs_push_ml( vs, ml1->tail );
                    vs = vs_push_e( vs, e2 );
                    vs = vs_push_ml( vs, ml1 );
                    vs = vs_push_e( vs, e2 );
                    cs = cs_push( cs, RESTE );  
                    cs = cs_push( cs, CHANFLE2 );
                    cs = cs_push( cs, RESTAR );
                }
                unref_e( e2 );
                unref_ml( ml1 );
                break;

            case RESTAR:
                e2 = vs_pop_e( &vs );
                ml1 = vs_pop_ml( &vs );

                e3 = ( expr )malloc( sizeof( struct expr ) );
                e3->refs = 0;
                e3->tag = NUM;
                e3->e.num = ml1->freq->e.num;
                ml4 = ( mlist )malloc( sizeof( struct mnode ) );
                ml4->refs = 0;
                ml4->article = ref_e( ml1->article );
                ml4->freq = ref_e( e3 );
                ml4->tail = NULL;

                switch( e2->tag ) {
                    case AMSET:
                        ml2 = e2->e.amset;
                        while( ml2 ) {
                            if( idcmp( ml1->article->e.name, ml2->article->e.name ) == 0 ) {
                                if( ml1->freq->e.num > ml2->freq->e.num ) {
                                    e3->e.num = ( ml1->freq->e.num - ml2->freq->e.num );
                                }
                                else {
                                    e3->e.num = 0;
                                }
                                break;
                            }
                            ml2 = ml2->tail;
                        }   
                        vs = vs_push_ml( vs, ml4 );
                        break;
                    case ALL:
                        e3->e.num = 0;
                        vs = vs_push_ml( vs, ml4 );
                        break;
                    case UNION:
                        vs = vs_push_ml( vs, ml1 );
                        vs = vs_push_e( vs, e2->e.binop.e0 );
                        vs = vs_push_ml( vs, ml1 );
                        vs = vs_push_e( vs, e2->e.binop.e1 );
                        cs = cs_push( cs, TOMAR_MIN );
                        cs = cs_push( cs, RESTAR );
                        cs = cs_push( cs, SWAP_2 );
                        cs = cs_push( cs, RESTAR );
                        unref_ml( ref_ml( ml4 ) ); /////////////////////////
                        break;
                    case INTERSECTION:
                        vs = vs_push_ml( vs, ml1 );
                        vs = vs_push_e( vs, e2->e.binop.e0 );
                        vs = vs_push_ml( vs, ml1 );
                        vs = vs_push_e( vs, e2->e.binop.e1 );
                        cs = cs_push( cs, TOMAR_MAX );
                        cs = cs_push( cs, RESTAR );
                        cs = cs_push( cs, SWAP_2 );
                        cs = cs_push( cs, RESTAR );
                        unref_ml( ref_ml( ml4 ) ); /////////////////////////
                        break;
                    case COMPLEMENT:
                        vs = vs_push_ml( vs, ml1 );
                        vs = vs_push_ml( vs, ml1 );
                        vs = vs_push_e( vs, e2->e.unop );
                        cs = cs_push( cs, COMPLEMENTAR );
                        cs = cs_push( cs, RESTAR );
                        unref_ml( ref_ml( ml4 ) ); /////////////////////////
                        break;
                    case DEPARTMENT:
                        if( inDepartment( ml1->article->e.name, e2->e.name ) 
                           //&& ml1->freq->e.num >= 1  // analizar bien la semantica de la resta
                          ) {
                            e3->e.num = 0;
                        }
                        vs = vs_push_ml( vs, ml4 );
                        break;
                    case CLASS:
                        if( inClass(
                          ml1->article->e.name, e2->e._class.nivel, e2->e._class.concept ) 
                          // && (ml1->freq->e.num >= 1) // analizar bien la semantica de la resta
                          ) {
                            e3->e.num = 0;
                        }
                        vs = vs_push_ml( vs, ml4 );
                        break;
                    default:
                        p_error( "RESTAR: Resta mal tipada." );
                        unref_e( e2 );
                        unref_ml( ml1 );
                        unref_ml( ref_ml( ml4 ) );
                        return( ABNORM );
                        break;
                }
                unref_e( e2 );
                unref_ml( ml1 );
                break;

            case CHANFLE:
                e1 = vs_pop_e( &vs );
                if( e1->tag == TRUE ) {
                    ml1 = vs_pop_ml( &vs );
                    e2 = vs_pop_e( &vs );
                    ml2 = vs_pop_ml( &vs );

                    ml3 = ( mlist )malloc( sizeof( struct mnode ) );
                    ml3->refs = 0;
                    ml3->tail = ref_ml( ml2 );
                    ml3->article = ref_e( ml1->article );
                    ml3->freq = ref_e( ml1->freq );

                    vs = vs_push_ml( vs, ml3 );
                    vs = vs_push_e( vs, e2 );
                    vs = vs_push_ml( vs, ml1->tail );

                    unref_ml( ml1 );
                    unref_e( e2 );
                    unref_ml( ml2 );
                }
                else {
                    ml1 = vs_pop_ml( &vs );
                    vs = vs_push_ml( vs, ml1->tail );
                    unref_ml( ml1 );
                }
                unref_e( e1 );
                break;

            case CHANFLE2:
                ml4 = vs_pop_ml( &vs );
                e2 = vs_pop_e( &vs );
                ml2 = vs_pop_ml( &vs );
                ml3 = vs_pop_ml( &vs );
                if( ml4->freq->e.num > 0 ) {
                    ml4->tail = ref_ml( ml3 );
                    vs = vs_push_ml( vs, ml4 );
                }
                else {
                    vs = vs_push_ml( vs, ml3 );
                }
                vs = vs_push_ml( vs, ml2 );
                vs = vs_push_e( vs, e2 );
                unref_ml( ml2 );
                unref_ml( ml3 );
                unref_ml( ml4 );
                unref_e( e2 );
                break;

            case TOMAR_MIN:
                ml1 = vs_pop_ml( &vs );
                ml2 = vs_pop_ml( &vs );
                vs = vs_push_ml( vs, ( ml1->freq->e.num < ml2->freq->e.num ? ml1 : ml2 ) );      
                unref_ml( ml1 );
                unref_ml( ml2 );
                break;

            case TOMAR_MAX:
                ml1 = vs_pop_ml( &vs );
                ml2 = vs_pop_ml( &vs );
                vs = vs_push_ml( vs, ( ml1->freq->e.num > ml2->freq->e.num ? ml1 : ml2 ) );      
                unref_ml( ml1 );
                unref_ml( ml2 );
                break;

            case COMPLEMENTAR:
                ml2 = vs_pop_ml( &vs );
                ml1 = vs_pop_ml( &vs );

                e3 = ( expr )malloc( sizeof( struct expr ) );
                e3->refs = 0;
                e3->tag = NUM;
                e3->e.num = ml1->freq->e.num;
                ml4 = ( mlist )malloc( sizeof( struct mnode ) );
                ml4->refs = 0;
                ml4->article = ref_e( ml1->article );
                ml4->freq = ref_e( e3 );
                ml4->tail = NULL;

                if( ml2->freq->e.num < ml1->freq->e.num ) {
                    // borrar el articulo
                    e3->e.num = 0;
                } // else dejar el articulo con la cantidad original

                vs = vs_push_ml( vs, ml4 );      

                unref_ml( ml1 );
                unref_ml( ml2 );
                break;

            case SACAR_ML:
                // 21-04-06 nueva implementacion
                e1 = vs_pop_e( &vs );
                e2 = vs_pop_e( &vs );
                e3 = vs_pop_e( &vs );
                vs = vs_push_ml( vs, e1->e.amset );
                vs = vs_push_e( vs, e3 );
                vs = vs_push_e( vs, e2 );
                vs = vs_push_ml( vs, e1->e.amset );
                unref_e( e1 );
                unref_e( e2 );
                unref_e( e3 );
                break;
                //    case SACAR_ML: // version anterior al 21-04-06
                //      e1 = vs_pop_e(&vs);
                //      vs = vs_push_ml(vs, e1->e.amset);
                //      unref_e(e1);
                //      break;

            case SWAP:
                vs = vs_swap( vs );
                break;

            case SWAP_2:
                vs = vs_swap_2( vs );
                break;

            case SWAP_3:
                vs = vs_swap_3( vs );
                break;

            case FREE:
                s = free_state( s );
                break;

            case POP_EXTERN_VARS:
                l1 = vs_pop_l( &vs );
                l2 = l1;
                while( l2 ) {
                    e1 = value( ( *ss )->s, l2->head->e.name );
                    s = update( s, l2->head->e.name, e1 );
                    l2 = l2->tail;
                }
                unref_l( l1 );
                break;

            case PUSH_GLOBAL_VARS:
                l1 = vs_pop_l( &vs );  // variables estaticas
                l2 = l1;
                while( l2 ) {
                    e1 = value( s, l2->head->e.name );
                    ( *ss )->st = static_update( ( *ss )->st, promo_name, l2->head->e.name, e1 );
                    l2 = l2->tail;
                }
                unref_l( l1 );
                l1 = vs_pop_l( &vs );  // variables externas
                l2 = l1;
                while( l2 ) {
                    e1 = value( s, l2->head->e.name );
                    ( *ss )->s = update( ( *ss )->s, l2->head->e.name, e1 );
                    l2 = l2->tail;
                }
                unref_l( l1 );
                l1 = vs_pop_l( &vs );  // variables globales
                l2 = l1;
                while( l2 ) {
                    e1 = value( s, l2->head->e.name );
                    ( *ss )->s = update( ( *ss )->s, l2->head->e.name, e1 );
                    l2 = l2->tail;
                }
                unref_l( l1 );
                break;

            default:
                p_error( ST( S_SWITCH_COMMANDO___CASO_NO_CONTEM ) ); //27/01/05
                return ABNORM;
        }
    }
    //  unref_e(purch);
    unref_e( added );
    added = NULL;
    unref_e( means_set );
    means_set = NULL;

    return NORM;
}

/** @} */

void freeall()
{
    cs = cs_free( cs );
    vs = vs_free( vs );
    s = free_state( s );
    unref_e( purch );
    purch = NULL;
    unref_e( wpurch );
    wpurch = NULL;
    unref_e( added );
    added = NULL;
    unref_e( means_set );
    means_set = NULL;
    if (promo_id_extern) {
	free (promo_id_extern); 
	promo_id_extern = NULL;
    }
}

void p_error( char *s )
{
	char buff[256];
	sprintf(buff,"PROMO: %s",s);
    //fprintf( stderr, ST( S_ERROR___S ), s );
    glog( buff ,LOG_MOTOR_PROMO,3);
    freeall();
}

double round2( double x, int n )
{
    double i;
    if( modf( x * pow( 10, n ), &i ) < .5 ) {
        return i / pow( 10, n );
    }
    else {
        return ( i + 1 ) / pow( 10, n );
    }
}

states free_states( states ss )
{
  //assert( ss );
  if ( ss ) {
    ss->s = free_state( ss->s );
    ss->st = free_static_state( ss->st );
    free( ss );
  }
    return NULL;
}

states states_clone( states ss )
{
    states s = NULL;
	if ( ss ) {
    s = ( states )malloc( sizeof( struct states ) );
    s->s = state_clone( ss->s );
    s->st = staticState_clone( ss->st );
	}
    return s;
}



/**
 * @ingroup groupEval 
 * Agrega articulos a la lista de articulos comprados, y
 * evalua las promos online.
 */
evalres add_articles( long article, identifier barcod, long codPresentacion, double cant, double precio, long depart, int promocionable, int por_importe, promo ps, states *ss )
{
    evalres r;
    expr e; 
    //  char buf[STRLEN_LONG];
    char buf[256];

    /*
    if( precio > 0 ) {
        //_snprintf(buf, 256, "j%li$%.8lf",article, (float)ITEM_PRECIO/(float)RAM_CANTIDAD );
        //_snprintf(buf, 256, "j%li$%.8lf",article, (float)RAM_IMPORTE/(float)RAM_CANTIDAD );
        _snprintf( buf, 256, "j%li$%.8lf", article, precio );
    }
    else {
        sprintf( buf, "i%li", article );
    }
    */
    if (barcod) {
		if ((!PRESENTACIONES_HABILITADAS) || (PRESENTACIONES_HABILITADAS && codPresentacion <= 1)) {
			if( por_importe ) {
				_snprintf( buf, 256, "y%li_%s$%.8lf", article, barcod, precio );
			} else {
				sprintf( buf, "x%li_%s", article, barcod );
			}
		} else {
			if( por_importe ) {
				_snprintf( buf, 256, "z%li_%s_%li$%.8lf", article, barcod, codPresentacion, precio );
			} else {
				sprintf( buf, "z%li_%s_%li", article, barcod, codPresentacion );
			}
		}
    } else {
		if( por_importe > 0 ) {
			_snprintf( buf, 256, "i%li$%.8lf", article, precio );
    } else {
      sprintf( buf, "i%li", article );
    }
    }

	glog("PROMO: add_articles: newAmset",LOG_MOTOR_PROMO,5);
    e = new_amset(); 
    ins_amset( e, buf, cant );
    unref_e( added );
    added = ref_e( e );

	glog("PROMO: add_articles: add_article_count",LOG_MOTOR_PROMO,5);
    add_article_count( article, cant );
	glog("PROMO: add_articles: add_department_count",LOG_MOTOR_PROMO,5);
    add_department_count( depart, cant );
	if ((!PRESENTACIONES_HABILITADAS) || (PRESENTACIONES_HABILITADAS && codPresentacion <= 1)) {
		#ifdef WIN32
			glog("PROMO: add_articles: put_art_cache",LOG_MOTOR_PROMO,5);
			put_art_cache ( article, ( barcod ? _atoi64(barcod) : 0 ), depart, precio, (char) promocionable);
		#else
			put_art_cache ( article, ( barcod ? atoll(barcod) : 0 ), depart, precio, (char) promocionable);
		#endif
	}
	glog("PROMO: add_articles: eval(ONLINE)",LOG_MOTOR_PROMO,5);
    r = eval( ONLINE, ps, ss );

    #ifdef PROMO_FAULT_TOLERANCE
    {
        //char ident[256];
        //sprintf(ident,"i%li",article);
        //logAddArticle(ident,cant);
		glog("PROMO: add_articles: logAddArticle",LOG_MOTOR_PROMO,5);
        logAddArticle( buf, cant, depart, precio, (char) promocionable, (char) por_importe );
    }
    #endif //PROMO_FAULT_TOLERANCE

    //  return eval(ONLINE, ps, ss);
	glog("PROMO: add_articles: end.",LOG_MOTOR_PROMO,5);
    return r;
}

/**
 * @ingroup groupEval 
 * Quita articulos a la lista de articulos comprados, y
 * evalua las cancelaciones de las promos online.
 */
evalres remove_articles( long article, identifier barcod, long codPresentacion, double cant, double precio, long depart, int promocionable, int por_importe, promo ps, states *ss )
{
    evalres r;
    expr e; 
    //char buf[STRLEN_LONG];
    char buf[256];

    /*
	if (barcod) {
      sprintf( buf, "x%li_%s", article, barcod );
    } else {
        sprintf( buf, "i%li", article );
    }
    */
    if (barcod) {
		if ((!PRESENTACIONES_HABILITADAS) || (PRESENTACIONES_HABILITADAS && codPresentacion <= 1)) {
			if( por_importe ) {
				_snprintf( buf, 256, "y%li_%s$%.8lf", article, barcod, precio );
			} else {
				_snprintf( buf, 256, "x%li_%s", article, barcod );
			}
		} else {
			if( por_importe ) {
				_snprintf( buf, 256, "z%li_%s_%li$%.8lf", article, barcod, codPresentacion, precio );
			} else {
				_snprintf( buf, 256, "z%li_%s_%li", article, barcod, codPresentacion );
			}
		}
    } else {
		if( por_importe > 0 ) {
			_snprintf( buf, 256, "i%li$%.8lf", article, precio );
    } else {
      sprintf( buf, "i%li", article );
    }
    }

    e = new_amset(); 
    ins_amset( e, buf, cant );
    unref_e( added );
    added = ref_e( e );

    add_article_count( article, -cant );
    add_department_count( depart, -cant );

    r = eval( CANCELLATION, ps, ss );

    #ifdef PROMO_FAULT_TOLERANCE
    {
        //char ident[256];
        //sprintf(ident,"i%li",article);
        //logRemoveArticle(ident,cant);
        logRemoveArticle( buf, cant, depart, precio, (char) promocionable, (char) por_importe );
    }
    #endif //PROMO_FAULT_TOLERANCE

    //  return eval(CANCELLATION, ps, ss);
    return r;
}


/**
 * @ingroup groupEval 
 * Actualiza los datos en el cache de articulos previamente comprados.
 * Por el momento no se están considerando las promos online, por lo que
 * no estan siendo reevaluadas.
 */
evalres update_articles( long article, identifier barcod, long codPresentacion, double cant, double precio, long depart, int promocionable, promo ps, states *ss )
{
    evalres r = NORM;
    //expr e; 
    //  char buf[STRLEN_LONG];
    char buf[256];

    if (barcod) {
		if ((!PRESENTACIONES_HABILITADAS) || (PRESENTACIONES_HABILITADAS && codPresentacion <= 1)) {
			_snprintf( buf, 256, "x%li_%s", article, barcod );
		} else {
			_snprintf( buf, 256, "z%li_%s_%li", article, barcod, codPresentacion );
		}
    } else {
      sprintf( buf, "i%li", article );
    }

	if ((!PRESENTACIONES_HABILITADAS) || (PRESENTACIONES_HABILITADAS && codPresentacion <= 1)) {
		#ifdef WIN32
			change_art_cache ( article, ( barcod ? _atoi64(barcod) : 0 ), depart, precio, (char) promocionable);
		#else //WIN32
			change_art_cache ( article, ( barcod ? atoll(barcod) : 0 ), depart, precio, (char) promocionable);
		#endif //WIN32
	}
    //r = eval( ONLINE, ps, ss );

    #ifdef PROMO_FAULT_TOLERANCE
    {
        //char ident[256];
        //sprintf(ident,"i%li",article);
        //logAddArticle(ident,cant);
        logUpdateArticle( buf, cant, depart, precio, (char) promocionable, (char) 0 );
    }
    #endif //PROMO_FAULT_TOLERANCE

    //  return eval(ONLINE, ps, ss);
    return r;
}







/** 
 * Agrega al motor de promociones un 'articulo ficticio' que refleje
 * en descuento externo al motor de promociones, otorgado por ejemplo
 * por el POS.
 * Se agrega inicialmente para manejar 'cambio de articulo' o 'cambio de mercaderia'
 */
evalres add_external_discount( double monto, promo ps, states *ss )
{
	expr e;
    char buf[256];
	char ident[] = "i0";

    #ifdef PROMO_FAULT_TOLERANCE
    {
//        logAddExternalDiscount( monto );
//        logUpdateArticle( buf, cant, depart, precio, (char) promocionable );
        logAddExternalDiscount( ident, 1, 0, monto, (char) 0, (char) 0 );
    }
    #endif //PROMO_FAULT_TOLERANCE


	// actualizar la variable global discounts para agregar el descuento
	e = clon_amset( value( ( *ss )->s, "discounts" ) );

    _snprintf( buf,256, "%s$%lf", ident, monto );
	buf[255] = '\0';

	ins_amset( e, buf, 1 );

	( *ss )->s = update( ( *ss )->s, "discounts", e );
    // el nombre de esta var (discounts) debe coincidir con el
    // nombre utilizado en la promo global
		
    return NORM;
}



/*
 * Devuelve el programa que resulta de concatenar p1 ++ p2
 * Se modifica p1.
 */
promo concat( promo p1, promo p2 )
{
    promo p = p1;
    while( p != NULL ) {
        if( p->nextPromo == NULL ) {
            p->nextPromo = p2;
            break;
        }
        else {
            p = p->nextPromo;
        }
    }
    return p1;
}


recover_info actual_info( states ss )
{
    recover_info r;
    r = ( recover_info )malloc( sizeof( struct recover_info ) );
    r->promo_state = states_clone( ss );
    r->cashdesk_state = actual_state();
    return r;
}

states recover( recover_info r, states ss )
{
    ss = free_states( ss );
    recover_state( r->cashdesk_state );
    return states_clone( r->promo_state );
}

recover_info free_recover_info( recover_info s )
{
    if( s != NULL ) {
        s->cashdesk_state = free_cashdesk_state( s->cashdesk_state );
        s->promo_state = free_states( s->promo_state );
        free( s );
    }
    return NULL;
}



//#endif

