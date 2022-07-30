#pragma pack (1) 

//#ifdef MODULO_PROMOCIONES

//#define PROMO_STR

#include "etpv.h"
#include"types2.h"
#include <string.h>

//char* PC_PATH = NULL;

#pragma pack (1)

expr ref_e( expr e )
{
    if( e ) {
        e->refs++;
    }
    return e;
}
list ref_l( list e )
{
    if( e ) {
        e->refs++;
    }
    return e;
}
mlist ref_ml( mlist e )
{
    if( e ) {
        e->refs++;
    }
    return e;
}
part ref_pl( part e )
{
    if( e ) {
        e->refs++;
    }
    return e;
}
benef ref_b( benef e )
{
    if( e ) {
        ( e->refs )++;
    }
    return e;
}
param ref_pa( param e )
{
    if( e ) {
        e->refs++;
    }
    return e;
}
promo ref_pro( promo e )
{
    if( e ) {
        e->refs++;
    }
    return e;
}



///////////////

void unref_e( expr e )
{
    if( !e ) {
        return;
    }
    if( !--e->refs ) {
        switch( e->tag ) {
            case NUM:
                free( e );
                break;
            case SUM:
            case SUB:
            case MULT:
            case FDIV:
            case MOD:
            case DIV:
            case MAX:
            case MIN:
            case LEQ :
            case LESS:
            case EQUAL:
            case GREATER:
            case GEQ:
            case NEQ:
            case AND:
            case OR:
            case UNION:
            case INTERSECTION:
            case IN:
            case MEET:
            case CONCAT:
            case DOT:
                unref_e( e->e.binop.e0 );
                unref_e( e->e.binop.e1 );
                free( e );
                break;
            case LOG:
                free( e->e.log.target );
                unref_e( e->e.log.e0 );
                free( e );
                break;
            case COND:
                unref_e( e->e.cond->e0 );
                unref_e( e->e.cond->e1 );
                unref_e( e->e.cond->e2 );
                free( e->e.cond );
                free( e );
                break;
            case AMOUNT:
            case COMPLEMENT:
            case SIZE:
            case LENGTH:
            case COUNTABLE:
            case _ROUND:
            case TRUNC:
            case INPUT:
            case NUMINPUT:
                unref_e( e->e.unop );
                free( e );
                break;
            case INTERVAL:
                unref_e( e->e.interval.e0 );
                unref_e( e->e.interval.e1 );
                free( e );
                break;
            case SET:
                unref_l( e->e.set );
                free( e );
                break;
            case AMSET:
                unref_ml( e->e.amset );
                free( e );
                break;
            case PART:
                unref_pl( e->e.part );
                free( e );
                break;
            case IDENT:
            case DEPARTMENT:
            case POINT:
            case ARTICLE:
            case STRING:
            case MUTUAL:
            case TICKET:
            case CHEQUE:
            case CURRENCY:
            case NUM_RNV:
            case BOOL_RNV:
            case STRING_RNV:
            case DATE_RNV:
            case TIME_RNV:
                free( e->e.name );
                free( e );
                break;
            case CARD:
                free( e->e.card.name );
                free( e->e.card.plan );
                free( e );
                break;
            case CLASS:
                free( e->e._class.nivel );
                free( e->e._class.concept );
                free( e );
                break;
            case BONUS:
                free( e->e.bonus->target );
                free( e->e.bonus->type );
                unref_e( e->e.bonus->e0 );
                free( e->e.bonus->leyenda );      
                unref_e( e->e.bonus->maturity );
                unref_e( e->e.bonus->validity );
                free( e->e.bonus );
                free( e );
                break;
            case TIMEV:
            case DATEV:
            case TRUE:
            case FALSE:
            case HOLIDAY:
            case AFFILIATED:
            case ALL:
                /*case PURCHASE: case WPURCHASE: */
            case BUYED:
            case DELIVERED:
            case DISCOUNTS:
            case ADDED:
            case TIME:
            case DATE:
            case _DAY:
            case CASH:
            case CTACTE:
            case MEANS:
            case CHANGE:
            case RETURNED:
            case POINTS:
            case EBONUS:
            case CASHDESK:
            case MONDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:
            case SATURDAY:
            case SUNDAY:
            case DAYSELECTOR:
            case MONTHSELECTOR:
            case YEARSELECTOR:
            case DATEOFBIRTH:
                free( e );
                break;
            case REQ_ANY:
            case REQ_ALL:
                unref_req( e->e.amset );
                free( e );
                break;
            case REQ_DEP:
                unref_req( (mlist) e->e.binop.e0 );
                unref_e( e->e.binop.e1 );                
                free( e );
                break;
            default:
                fprintf( stderr, ST( S_UNREF_E__CASO_NO_CONTEMPLADO ) );
                break;
        }
    }
}

void unref_l( list l )
{
    list l2;
    //fprintf(stderr,"l<");
    while( l ) {
        //if (l->head->tag == IDENT) /**/fprintf(stderr,"+%s+",l->head->e.name);
        //fprintf(stderr,"refs:%i.",l->refs);
        --l->refs;
        //fprintf(stderr,"refs:%i.",l->refs);
        if( l->refs != 0 ) {
            break;
        }
        //fprintf(stderr,"(%s)",l->head->e.name);
        unref_e( l->head );
        //fprintf(stderr,"-");
        l2 = l;
        l = l->tail;
        //fprintf(stderr,",");
        free( l2 );
    }
    //fprintf(stderr,">\n");
}

void unref_ml( mlist ml )
{
    mlist ml2;
    while( ml ) {
        --ml->refs;
        if( ml->refs != 0 ) {
            break;
        }
        unref_e( ml->article );
        unref_e( ml->freq );
        ml2 = ml;
        ml = ml->tail;
        free( ml2 );
    }
}

void unref_pl( part ml )
{
    part ml2;
    while( ml ) {
        --ml->refs;
        if( ml->refs != 0 ) {
            break;
        }
        unref_e( ml->rep );
        unref_e( ml->clase );
        ml2 = ml;
        ml = ml->tail;
        free( ml2 );
    }
}

void unref_req( mlist ml )
{
    mlist ml2;
    while( ml ) {
        --ml->refs;
        if( ml->refs != 0 ) {
            break;
        }
        //unref_e(ml->article);
        unref_e( ml->freq );
        ml2 = ml;
        ml = ml->tail;
        free( ml2 );
    }
}

void unref_b( benef b )
{
    if( !b ) {
        return;
    }
    if( !--b->refs ) {
        switch( b->tag ) {
            case ASSIGN:
            case NUM_RNV_ASSIGN:
            case BOOL_RNV_ASSIGN:
            case STRING_RNV_ASSIGN:
            case DATE_RNV_ASSIGN:
            case TIME_RNV_ASSIGN:
                free( b->b.assign.var );
                unref_e( b->b.assign.e );
                free( b );                      // fa: revisar aca <<<<<<<<<<
                break;
            case IF:
                unref_e( b->b.cond.b );
                unref_b( b->b.cond.c1 );
                unref_b( b->b.cond.c2 );
                free( b );
                break;
            case FOR:
                free( b->b.foreach.a );
                free( b->b.foreach.k );
                unref_e( b->b.foreach.e );
                unref_b( b->b.foreach.c );
                free( b );
                break;
            case BPRINT:
                unref_e( b->b.print );
                free( b );
                break;
            case CREDIT:
                unref_e( b->b.credit.e1 );
                unref_e( b->b.credit.e2 );
                unref_e( b->b.credit.e3 );
                unref_e( b->b.credit.e4 );
                free( b );
                break;
            case DISTRIBUTE:
                unref_e( b->b.distribute.e1 );
                unref_e( b->b.distribute.e2 );
                unref_e( b->b.distribute.e3 );
                free( b );
                break;
            case ISSUE_BONUS:
                unref_e( b->b.issueBonus.e1 );
                unref_e( b->b.issueBonus.e2 );
                free( b );
                break;
            case GIVE_POINTS:
                unref_e( b->b.givePoints.e1 );
                unref_e( b->b.givePoints.e2 );
                free( b );
                break;
            case ACTIVATE:
                unref_e( b->b.activate.e1 );
                unref_e( b->b.activate.e2 );
                free( b );
                break;
            case EPROC:
                free( b->b.eproc.procName );
                unref_e( b->b.eproc.params );
                free( b );
                break;
            case REQ_MEANS:
                unref_e( b->b.activate.e1 );
                unref_e( b->b.activate.e2 );
                free( b );
                break;
            case REC:
                unref_e( b->b.rec.e1 );
                unref_e( b->b.rec.e2 );
                free( b );
                break;
            case __SKIP:
                free( b );
                break;
            case COMP:
                unref_b( b->b.comp.c1 );
                unref_b( b->b.comp.c2 );
                free( b );
                break;
            default:
                fprintf( stderr, ST( S_UNREF_E__CASO_NO_CONTEMPLADO ) );
                break;
        }
    }
}


void unref_pa( param p )
{
    param p1 = p;
    param p2;
    //fprintf(stderr,"pa[(%s)",p->var);

    if( !p ) {
        return;
    }
    while( p1 ) {
        if( !--p1->refs ) {
            p2 = p1->nextParams;
            free( p1->var );
            if( p1->tag != EXTERN ) {
                unref_e( p1->e );
            }
            free( p1 );
            p1 = p2;
        }
        else {
            break;
        }
    }
    //fprintf(stderr,"]\n");
}

void unref_pr( promo p )
{
    promo p1 = p;
    promo p2;

    if( !p ) {
        return;
    }
    while( p1 ) {
        //fprintf(stderr,"[%i]",p1->refs);
        if( !--p1->refs ) {
            //fprintf(stderr,"pr{(%i)",p1->refs);
            p2 = p1->nextPromo;
            //fprintf(stderr,".");
            unref_l( p1->statics );
            //fprintf(stderr,".");
            unref_l( p1->externs );
            //fprintf(stderr,".");
            unref_l( p1->globals );
            //fprintf(stderr,".");
            unref_l( p1->preconditions );
            unref_pa( p1->parameters );
            //fprintf(stderr,".");
            unref_l( p1->conditions );
            unref_b( p1->benefits );
            unref_pr( p1->cancellation );

            free( p1->name );
            free( p1 );
            p1 = p2;
            //fprintf(stderr,",");
        }
        else {
            break;
        }
    }
    //fprintf(stderr,"}\n");
}

expr new_amset()
{
    expr e;
    e = ( expr )malloc( sizeof( struct expr ) );
    e->refs = 0;
    e->tag = AMSET;
    e->e.amset = NULL;
    return e;
}

expr clon_amset( expr e )
{
    expr e1 = NULL;
    struct mnode *m1;
    if( e != NULL ) {
        e1 = new_amset();
        m1 = e->e.amset;
        while( m1 != NULL ) {
            ins_amset( e1, m1->article->e.name, m1->freq->e.num );
            m1 = m1->tail;
        }
    }
    return e1;
}

/*
 * Agrega normalizadamente un articulo 'a' con cantidad 'k'
 * en el AMSET 'e'.
 */
void ins_amset( expr e, identifier a, double k )
{
    //int i;
    char *ident;
    mlist ml;
    expr e1,e2;

    // Si la cantidad k es negativa, se debe borrar en vez de agregar,
    // lo cual esta bien implementado en del_amset
    if( k < 0 ) {
        /**/fprintf( stderr, "type2.c: ins_amset: Agregando cantidad negativa\n" );
        del_amset( e, a, -k );
        return;
    }


    // se asume que e fue creado con las primitivas, por lo cual
    // es una expr de tipo AMSET normalizada.
    ml = e->e.amset;
    while( ml ) {
        if( idcmp( a, ml->article->e.name ) == 0 ) {
            ml->freq->e.num += k;
            return;
        }
        ml = ml->tail;
    }

    ident = ( char* )malloc( strlen( a ) + 1 );
    strcpy( ident, a );
    e1 = ( expr )malloc( sizeof( struct expr ) );
    e1->refs = 0;
    e1->tag = ARTICLE;
    e1->e.name = ident;
    e2 = ( expr )malloc( sizeof( struct expr ) );
    e2->refs = 0;
    e2->tag = NUM;
    e2->e.num = k;
    ml = ( mlist )malloc( sizeof( struct mnode ) );
    ml->refs = 0;
    ml->article = ref_e( e1 );
    ml->freq = ref_e( e2 );
    ml->tail = e->e.amset;
    e->e.amset = ref_ml( ml );
    return;
}


/*
 * Quita normalizadamente un articulo 'a' con cantidad 'k'
 * en el AMSET 'e'.
 */
void del_amset( expr e, identifier a, double k )
{
    //int i;
    mlist ml,ml2;

    // se asume que e fue creado con las primitivas, por lo cual
    // es una expr de tipo AMSET normalizada.
    ml = e->e.amset;
    ml2 = ml;
    while( ml ) {
        if( idcmp( a, ml->article->e.name ) == 0 ) {
            ml->freq->e.num -= k;
            if( ml->freq->e.num <= 0 ) {
                if( ml == ml2 ) {
                    // Borrar el primer elemento de la lista
                    e->e.amset = ref_ml( ml->tail );
                }
                else {
                    ml2->tail = ref_ml( ml->tail );
                }
                unref_ml( ml );
            }
            return;
        }
        ml2 = ml;
        ml = ml->tail;
    }
    return;
}



expr new_means()
{
    expr e;
    e = ( expr )malloc( sizeof( struct expr ) );
    e->refs = 0;
    e->tag = AMSET;
    e->e.amset = NULL;
    return e;
}


/*
 * Agrega normalizadamente un card 'a' con cantidad 'k' en plan de pago 'p'
 * en el AMSET_MEANS 'e'.
 */
//void ins_card(expr e, identifier a, double k) {
void ins_card( expr e, identifier a, identifier p, double k )
{
    //int i;
    char *ident;
    char *plan;
    mlist ml;
    expr e1,e2;

    // se asume que e fue creado con las primitivas, por lo cual
    // es una expr de tipo AMSET normalizada.
    ml = e->e.amset;
    while( ml ) {
		if ( ml->article->tag == CARD ) {
			if( ( idcmp( a, ml->article->e.card.name ) == 0 )
			 && ( idcmp( p, ml->article->e.card.plan ) == 0 ) ) {
				ml->freq->e.num += k;
				return;
			}
		}
        ml = ml->tail;
    }

    ident = ( char* )malloc( strlen( a ) + 1 );
    strcpy( ident, a );
    ident[strlen( a )] = '\0';
    plan = ( char* )malloc( strlen( p ) + 1 );
    strcpy( plan, p );
    plan[strlen( p )] = '\0';
    e1 = ( expr )malloc( sizeof( struct expr ) );
    e1->refs = 0;
    e1->tag = CARD;
    e1->e.card.name = ident;
    e1->e.card.plan = plan;
    e2 = ( expr )malloc( sizeof( struct expr ) );
    e2->refs = 0;
    e2->tag = NUM;
    e2->e.num = k;
    ml = ( mlist )malloc( sizeof( struct mnode ) );
    ml->refs = 0;
    ml->article = ref_e( e1 );
    ml->freq = ref_e( e2 );
    ml->tail = e->e.amset;
    e->e.amset = ref_ml( ml );
    return;
}


/*
 * Agrega normalizadamente una mutual 'a' con cantidad 'k'
 * en el AMSET_MEANS 'e'.
 */
void ins_mutual( expr e, identifier a, double k )
{
    char *ident;
    mlist ml;
    expr e1,e2;

    // se asume que e fue creado con las primitivas, por lo cual
    // es una expr de tipo AMSET normalizada.
    ml = e->e.amset;
    while( ml ) {
        if( ( ml->article->tag == MUTUAL ) && ( idcmp( a, ml->article->e.name ) == 0 ) ) {
            ml->freq->e.num += k;
            return;
        }
        ml = ml->tail;
    }

    ident = ( char* )malloc( strlen( a ) + 1 );
    strcpy( ident, a );
    e1 = ( expr )malloc( sizeof( struct expr ) );
    e1->refs = 0;
    e1->tag = MUTUAL;
    e1->e.name = ident;
    e2 = ( expr )malloc( sizeof( struct expr ) );
    e2->refs = 0;
    e2->tag = NUM;
    e2->e.num = k;
    ml = ( mlist )malloc( sizeof( struct mnode ) );
    ml->refs = 0;
    ml->article = ref_e( e1 );
    ml->freq = ref_e( e2 );
    ml->tail = e->e.amset;
    e->e.amset = ref_ml( ml );
    return;
}


void ins_ticket( expr e, identifier a, double k )
{
    char *ident;
    mlist ml;
    expr e1,e2;

    ml = e->e.amset;
    while( ml ) {
        if( ( ml->article->tag == TICKET ) && ( idcmp( a, ml->article->e.name ) == 0 ) ) {
            ml->freq->e.num += k;
            return;
        }
        ml = ml->tail;
    }

    ident = ( char* )malloc( strlen( a ) + 1 );
    strcpy( ident, a );
    e1 = ( expr )malloc( sizeof( struct expr ) );
    e1->refs = 0;
    e1->tag = TICKET;
    e1->e.name = ident;
    e2 = ( expr )malloc( sizeof( struct expr ) );
    e2->refs = 0;
    e2->tag = NUM;
    e2->e.num = k;
    ml = ( mlist )malloc( sizeof( struct mnode ) );
    ml->refs = 0;
    ml->article = ref_e( e1 );
    ml->freq = ref_e( e2 );
    ml->tail = e->e.amset;
    e->e.amset = ref_ml( ml );
    return;
}

void ins_cheque( expr e, identifier a, double k )
{
    char *ident;
    mlist ml;
    expr e1,e2;

    ml = e->e.amset;
    while( ml ) {
        if( ( ml->article->tag == CHEQUE ) && ( idcmp( a, ml->article->e.name ) == 0 ) ) {
            ml->freq->e.num += k;
            return;
        }
        ml = ml->tail;
    }

    ident = ( char* )malloc( strlen( a ) + 1 );
    strcpy( ident, a );
    e1 = ( expr )malloc( sizeof( struct expr ) );
    e1->refs = 0;
    e1->tag = CHEQUE;
    e1->e.name = ident;
    e2 = ( expr )malloc( sizeof( struct expr ) );
    e2->refs = 0;
    e2->tag = NUM;
    e2->e.num = k;
    ml = ( mlist )malloc( sizeof( struct mnode ) );
    ml->refs = 0;
    ml->article = ref_e( e1 );
    ml->freq = ref_e( e2 );
    ml->tail = e->e.amset;
    e->e.amset = ref_ml( ml );
    return;
}

void ins_currency( expr e, identifier a, double k )
{
    char *ident;
    mlist ml;
    expr e1,e2;

    ml = e->e.amset;
    while( ml ) {
        if( ( ml->article->tag == CURRENCY ) && ( idcmp( a, ml->article->e.name ) == 0 ) ) {
            ml->freq->e.num += k;
            return;
        }
        ml = ml->tail;
    }

    ident = ( char* )malloc( strlen( a ) + 1 );
    strcpy( ident, a );
    e1 = ( expr )malloc( sizeof( struct expr ) );
    e1->refs = 0;
    e1->tag = CURRENCY;
    e1->e.name = ident;
    e2 = ( expr )malloc( sizeof( struct expr ) );
    e2->refs = 0;
    e2->tag = NUM;
    e2->e.num = k;
    ml = ( mlist )malloc( sizeof( struct mnode ) );
    ml->refs = 0;
    ml->article = ref_e( e1 );
    ml->freq = ref_e( e2 );
    ml->tail = e->e.amset;
    e->e.amset = ref_ml( ml );
    return;
}



void ins_cash( expr e, double k )
{
    //int i;
    //char* ident;
    mlist ml;
    expr e1,e2;

    // se asume que e fue creado con las primitivas, por lo cual
    // es una expr de tipo AMSET normalizada.
    ml = e->e.amset;
    while( ml ) {
        if( ml->article->tag == CASH ) {
            ml->freq->e.num += k;
            return;
        }
        ml = ml->tail;
    }

    e1 = ( expr )malloc( sizeof( struct expr ) );
    e1->refs = 0;
    e1->tag = CASH;
    e2 = ( expr )malloc( sizeof( struct expr ) );
    e2->refs = 0;
    e2->tag = NUM;
    e2->e.num = k;
    ml = ( mlist )malloc( sizeof( struct mnode ) );
    ml->refs = 0;
    ml->article = ref_e( e1 );
    ml->freq = ref_e( e2 );
    ml->tail = e->e.amset;
    e->e.amset = ref_ml( ml );
    return;
}

void ins_ctacte( expr e, double k )
{
    //int i;
    //char* ident;
    mlist ml;
    expr e1,e2;

    // se asume que e fue creado con las primitivas, por lo cual
    // es una expr de tipo AMSET normalizada.
    ml = e->e.amset;
    while( ml ) {
        if( ml->article->tag == CTACTE ) {
            ml->freq->e.num += k;
            return;
        }
        ml = ml->tail;
    }

    e1 = ( expr )malloc( sizeof( struct expr ) );
    e1->refs = 0;
    e1->tag = CTACTE;
    e2 = ( expr )malloc( sizeof( struct expr ) );
    e2->refs = 0;
    e2->tag = NUM;
    e2->e.num = k;
    ml = ( mlist )malloc( sizeof( struct mnode ) );
    ml->refs = 0;
    ml->article = ref_e( e1 );
    ml->freq = ref_e( e2 );
    ml->tail = e->e.amset;
    e->e.amset = ref_ml( ml );
    return;
}


expr clon_means( expr e )
{
    expr e1 = NULL;
    struct mnode *m1;
    if( e != NULL ) {
        e1 = new_means();
        m1 = e->e.amset;
        while( m1 != NULL ) {
            switch( m1->article->tag ) {
                case CASH:
                    ins_cash( e1, m1->freq->e.num );
                    break;
                case CARD:
                    ins_card( e1, m1->article->e.card.name, m1->article->e.card.plan,
                              m1->freq->e.num );    
                    break;
                case MUTUAL:
                    ins_mutual( e1, m1->article->e.name, m1->freq->e.num );
                    break;
                case TICKET:
                    ins_ticket( e1, m1->article->e.name, m1->freq->e.num );
                    break;
                case CHEQUE:
                    ins_cheque( e1, m1->article->e.name, m1->freq->e.num );
                    break;
                case CURRENCY:
                    ins_currency( e1, m1->article->e.name, m1->freq->e.num );
                    break;
                default:
                    break;
            }
            m1 = m1->tail;
        }
    }
    return e1;
}




/***********************************************************/

expr new_points( void )
{
    //crea un multiconjunto de puntos vacio.
    expr e;
    e = ( expr )malloc( sizeof( struct expr ) );
    e->refs = 0;
    e->tag = AMSET;
    e->e.amset = NULL;
    return e;
}

/*
 * Agrega normalizadamente un punto de tipo 'a' con cantidad 'k'
 * en el AMSET_POINTS 'e'.
 */
void ins_point( expr e, identifier a, double k )
{
    //inserta puntos en el amset_points
    //int i;
    char *ident;
    mlist ml;
    expr e1,e2;

    // se asume que e fue creado con las primitivas, por lo cual
    // es una expr de tipo AMSET normalizada.
    ml = e->e.amset;
    while( ml ) {
        if( idcmp( a, ml->article->e.name ) == 0 ) {
            ml->freq->e.num += k;
            return;
        }
        ml = ml->tail;
    }

    ident = ( char* )malloc( strlen( a ) + 1 );
    strcpy( ident, a );
    e1 = ( expr )malloc( sizeof( struct expr ) );
    e1->refs = 0;
    e1->tag = POINT;
    e1->e.name = ident;
    e2 = ( expr )malloc( sizeof( struct expr ) );
    e2->refs = 0;
    e2->tag = NUM;
    e2->e.num = k;
    ml = ( mlist )malloc( sizeof( struct mnode ) );
    ml->refs = 0;
    ml->article = ref_e( e1 );
    ml->freq = ref_e( e2 );
    ml->tail = e->e.amset;
    e->e.amset = ref_ml( ml );
    return;
}



expr new_ebonus( void )
{
    //crea un multiconjunto de bonos electronicos vacio.
    expr e;
    e = ( expr )malloc( sizeof( struct expr ) );
    e->refs = 0;
    e->tag = AMSET;
    e->e.amset = NULL;
    return e;
}

/*
 * Agrega normalizadamente un bono electronico de tipo 'a' con cantidad 'k'
 * en el AMSET_EBONUS 'e'.
 */
void ins_ebonus( expr e, identifier a, double k )
{
    char *ident;
    mlist ml;
    expr e1,e2;

    // se asume que e fue creado con las primitivas, por lo cual
    // es una expr de tipo AMSET normalizada.
    ml = e->e.amset;
    while( ml ) {
        if( idcmp( a, ml->article->e.name ) == 0 ) {
            ml->freq->e.num += k;
            return;
        }
        ml = ml->tail;
    }

    ident = ( char* )malloc( strlen( a ) + 1 );
    strcpy( ident, a );
    e1 = ( expr )malloc( sizeof( struct expr ) );
    e1->refs = 0;
    e1->tag = IDENT;
    e1->e.name = ident;
    e2 = ( expr )malloc( sizeof( struct expr ) );
    e2->refs = 0;
    e2->tag = NUM;
    e2->e.num = k;
    ml = ( mlist )malloc( sizeof( struct mnode ) );
    ml->refs = 0;
    ml->article = ref_e( e1 );
    ml->freq = ref_e( e2 );
    ml->tail = e->e.amset;
    e->e.amset = ref_ml( ml );
    return;
}






expr new_part( void )
{
    expr e;
    e = ( expr )malloc( sizeof( struct expr ) );
    e->refs = 0;
    e->tag = PART;
    e->e.part = NULL;
    return e;
}

//agrega el articulo 'a' a la clase de la particion 'e' cuyo representante es 'rep'.
//pre: 'a' no pertenece a ninguna clase de la particion 'e'
void ins_part( expr e, identifier rep, identifier a )
{
    part p;

    p = e->e.part;
    while( p != NULL ) {
        if( idcmp( p->rep->e.name, rep ) == 0 ) {
            list s;
            s = ( list )malloc( sizeof( struct node ) );
            s->refs = 1;
            s->head = ( expr )malloc( SIZE_OF_EXPR_IDENT );
            s->head->refs = 1;
            s->head->tag = ARTICLE;
            s->head->e.name = ( identifier )malloc( strlen( a ) + 1 );
            strcpy( s->head->e.name, a );
            s->head->e.name[strlen( a )] = '\0';
            s->tail = p->clase->e.set;
            p->clase->e.set = s;
            break;
        }
        p = p->tail;
    }
    if( p == NULL ) {
        // crear una nueva clase con representante 'rep'
        list s;
        p = ( part )malloc( sizeof( struct pnode ) );
        p->refs = 1;
        p->rep = ( expr )malloc( SIZE_OF_EXPR_IDENT );
        p->rep->refs = 1;
        p->rep->tag = ARTICLE;
        p->rep->e.name = ( identifier )malloc( strlen( rep ) + 1 );
        strcpy( p->rep->e.name, rep );
        p->rep->e.name[strlen( rep )] = '\0';
        s = ( list )malloc( sizeof( struct node ) );
        s->refs = 1;
        s->head = ( expr )malloc( SIZE_OF_EXPR_IDENT );
        s->head->refs = 1;
        s->head->tag = ARTICLE;
        s->head->e.name = ( identifier )malloc( strlen( a ) + 1 );
        strcpy( s->head->e.name, a );
        s->head->e.name[strlen( a )] = '\0';
        s->tail = NULL;
        p->clase = ( expr )malloc( sizeof( struct expr ) );
        p->clase->refs = 1;
        p->clase->tag = SET;
        p->clase->e.set = s;
        p->tail = e->e.part;
        e->e.part = p;
    }
    return;
}





// idcmp(i,j) devuelve 0 cuando los identificadores i y j son iguales 
// ( modulo cambio de precio ) i.e. uno no tiene precio y el otro si, o ambos tienen el mismo precio
// devuelve 1 cuando son distintos.
// devuelve 2 cuando i y j son de la pinta "jxxx$nnn" y "jxxx$mmm" respectivamente
// devuelve 3 cuando i y j son de la pinta "jxxx$nnn" y "ixxx" respectivamente y vice

//#define MONEDA '7'
#define MONEDA '$'

int idcmp( identifier i, identifier j )
{
    int n = 0;
    int rta = 1; 
    if( ( !i ) || ( !j ) ) {
        return -1;
    }

    if( ( i[0] == 'j' ) || ( j[0] == 'j' ) ) {
        n = 1;
        // Tratamiento especial para articulos "por importe"
        while( 1 ) {
            if( i[n] != j[n] ) {
                if( ( ( i[n] == '\0' ) && ( j[n] == MONEDA ) )
                 || ( ( i[n] == MONEDA ) && ( j[n] == '\0' ) ) ) {
                    //fprintf(stderr,"*");
                    return 3;
                }
                else {
                    return ( rta == 2 ? 2 : 1 );
                }
            }
            else if( i[n] == MONEDA ) {
                rta = 2;
                //return 2;
            }

            if( ( i[n] != '\0' ) && ( j[n] != '\0' ) ) {
                n++;
            }
            else {
                return i[n] == j[n] ? 0 : ( rta == 2 ? 2 : 1 );
            }
        }
    }

    //  if ( ( (i[0] == 'j') && (j[0] == 'i') ) || ( (i[0] == 'i') && (j[0] == 'j') ) ) {
    //    n = 1;
    //  }

    // Tratamiento "NORMAL"

    while( 1 ) {
        if( i[n] != j[n] ) {
            if( ( ( i[n] == '\0' ) && ( j[n] == MONEDA ) )
             || ( ( i[n] == MONEDA ) && ( j[n] == '\0' ) ) ) {
                return 0;
            }
            else {
                return 1;
            }
        }

        if( i[n] != '\0' ) {
            n++;
        }
        else {
            return 0;
        }
    }
}


//TIPO_COD_ARTICULO getCodsArt( identifier ident, long* cod_int, _int64 * cod_bar, double * precio) {
TIPO_COD_ARTICULO getCodsArt( identifier ident, long* cod_int, char * cod_bar, double * precio, long* cod_presentacion) {
	
	if (!ident || !cod_int || !cod_bar || !precio || !cod_presentacion) return COD_ERROR;

	switch ( ident[0] ) {
	case 'i':
		switch ( sscanf( ident, "i%li$%lf", cod_int, precio ) ) {
		case 1:
			return COD_INTERNO; 
			break;
		case 2:
			return COD_INTERNO_PRECIO; 
			break;
		default:
			return COD_ERROR;
			break;
		}
		break;
	case 'b':
		//switch ( sscanf( ident, "b%I64i$%lf", cod_bar, precio ) ) {
		switch ( sscanf( ident, "b%s$%lf", cod_bar, precio ) ) {
		case 1:
			return COD_BARRA; 
			break;
		case 2:
			return COD_BARRA_PRECIO; 
			break;
		default:
			return COD_ERROR;
			break;
		}
		break;
	case 'x':
		//switch ( sscanf( ident, "x%li_%I64i$%lf", cod_int, cod_bar, precio ) ) {
		switch ( sscanf( ident, "x%li_%13s$%lf", cod_int, cod_bar, precio ) ) {
		case 2:
			return COD_INTERNO_BARRA; 
			break;
		case 3:
			return COD_INTERNO_BARRA_PRECIO; 
			break;
		default:
			return COD_ERROR;
			break;
		}
		break;
	case 'y':
		//switch ( sscanf( ident, "x%li_%I64i$%lf", cod_int, cod_bar, precio ) ) {
		switch ( sscanf( ident, "y%li_%13s$%lf", cod_int, cod_bar, precio ) ) {
		case 2:
			return COD_INTERNO_BARRA; 
			break;
		case 3:
			return COD_INTERNO_BARRA_PRECIO; 
			break;
		default:
			return COD_ERROR;
			break;
		}
		break;
	case 'z':
		// Artículos con presentaciones
		// codigoInterno_codigoBarra_codigoPresentacion&precio
		//switch ( sscanf( ident, "x%li_%I64i$%lf", cod_int, cod_bar, precio ) ) {
		switch ( sscanf( ident, "z%li_%13s_%li$%lf", cod_int, cod_bar, cod_presentacion, precio ) ) {
		case 1:
			return COD_INTERNO; 
			break;
		case 2:
			return COD_INTERNO_BARRA; 
			break;
		case 3:
			return COD_INTERNO_BARRA_PRESENTACION;
			break;
		case 4:
			return COD_INTERNO_BARRA_PRESENTACION_PRECIO; 
			break;
		default:
			return COD_ERROR;
			break;
		}
		break;
	case 'j':
		switch ( sscanf( ident, "j%li$%lf", cod_int, precio ) ) {
		case 1:
			return COD_INTERNO; 
			break;
		case 2:
			return COD_INTERNO_PRECIO; 
			break;
		default:
			return COD_ERROR;
			break;
		}
		break;
	default:
		return COD_ERROR;
	}

}


#ifndef WIN32
#define min(A,B) (A>B?B:A)
#endif //WIN32

// Devuelve un articulo y su cantidad en rta, y lo elimina del article-amset.
// devuelve 1 en caso de exito y 0 en caso de error o amset vacio.
int get_amset(expr e, artIdent *rta) {
  long cod_int = 0;
  char cod_bar[256];
  double precio = 0;
  long cod_presentacion = 0;
  mlist ml = NULL;

  if (!e) return 0;
  if (e->tag != AMSET) return 0;
  if (!e->e.amset) return 0;

  memset(cod_bar, 0, sizeof(cod_bar));
  memset(rta, 0, sizeof(struct artIdent));
  //fprintf( stdout, "types2: get_amset: memoria seteada\n" );
  //fprintf( stdout, "types2: get_amset: identifier: %s\n",e->e.amset->article->e.name );

  switch( getCodsArt (e->e.amset->article->e.name, &cod_int, &cod_bar[0], &precio, &cod_presentacion)  ) {
    
  case COD_INTERNO:
  case COD_BARRA:
  case COD_INTERNO_BARRA:
  case COD_INTERNO_PRECIO:
  case COD_BARRA_PRECIO:
  case COD_INTERNO_BARRA_PRECIO:
  //fprintf( stdout, "types2: get_amset: switch\n" );
    rta->codInt = cod_int;

    //fprintf( stdout, "types2: get_amset: codInt (%li) listo.\n",rta->codInt );
    if (cod_bar) strncpy( rta->codBar, cod_bar, min( strlen(cod_bar), sizeof(rta->codBar)-1 ) );

  //fprintf( stdout, "types2: get_amset: codBar (%s) listo.\n",rta->codBar );

    rta->cant = e->e.amset->freq->e.num;
  //fprintf( stdout, "types2: get_amset: cant (%.2lf) listo.\n",rta->cant );

    ml = ref_ml( e->e.amset->tail );
    unref_ml ( e->e.amset );
    e->e.amset = ml;
    break;
  case COD_INTERNO_BARRA_PRESENTACION:
  case COD_INTERNO_BARRA_PRESENTACION_PRECIO:
    rta->codInt = cod_int;
    if (cod_bar) strncpy( rta->codBar, cod_bar, min( strlen(cod_bar), sizeof(rta->codBar)-1 ) );
	rta->codPresentacion = cod_presentacion;
    rta->cant = e->e.amset->freq->e.num;
    ml = ref_ml( e->e.amset->tail );
    unref_ml ( e->e.amset );
    e->e.amset = ml;
    break;
  case COD_ERROR:
  default:
    fprintf(stderr,"types2.c: get_amset: caso de identificador de articulo no reconocido\n");
    return 0;
    break;

  }

  //fprintf( stdout, "types2: get_amset: liberando codBar\n" );
  //free(cod_bar);
  return 1;

	 
}


//#endif


