#pragma pack (1) 

//#ifdef MODULO_PROMOCIONES

#include "uprinter.h"
#include "types2.h"
#include "time2.h"

#pragma pack (1)

void up_expr( FILE *out, expr e, int prec )
{
    list l;
    mlist ml;
    part ps;
    if( e == NULL ) {
        return;
    }
    switch( e->tag ) {
        case NUM:
            /* Aca a los flotantes no le muestro los decimales */
            if( ( ( double )( long )e->e.num ) == e->e.num ) {
                fprintf( out, "%li", ( long )e->e.num );
            }
            else {
                fprintf( out, "%.8lf", e->e.num );
            }
            break;
        case SUM:
            if( prec > 6 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 6 );
            fprintf( out, "+" );
            up_expr( out, e->e.binop.e1, 6 );
            if( prec > 6 ) {
                fprintf( out, ")" );
            }
            break;
        case SUB:
            if( prec > 6 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 6 );
            fprintf( out, "-" );
            up_expr( out, e->e.binop.e1, 6 );
            if( prec > 6 ) {
                fprintf( out, ")" );
            }
            break;
        case MULT:
            if( prec > 9 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 9 );
            fprintf( out, "*" );
            up_expr( out, e->e.binop.e1, 9 );
            if( prec > 9 ) {
                fprintf( out, ")" );
            }
            break;
        case FDIV:
            if( prec > 9 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 9 );
            fprintf( out, "/" );
            up_expr( out, e->e.binop.e1, 9 );
            if( prec > 9 ) {
                fprintf( out, ")" );
            }
            break;
        case MOD:
            if( prec > 8 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 8 );
            fprintf( out, " mod " );
            up_expr( out, e->e.binop.e1, 8 );
            if( prec > 8 ) {
                fprintf( out, ")" );
            }
            break;
        case DIV:
            if( prec > 8 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 8 );
            fprintf( out, " div " );
            up_expr( out, e->e.binop.e1, 8 );
            if( prec > 8 ) {
                fprintf( out, ")" );
            }
            break;
        case MAX:
            if( prec > 7 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 7 );
            fprintf( out, " max " );
            up_expr( out, e->e.binop.e1, 7 );
            if( prec > 7 ) {
                fprintf( out, ")" );
            }
            break;
        case MIN:
            if( prec > 7 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 7 );
            fprintf( out, " min " );
            up_expr( out, e->e.binop.e1, 7 );
            if( prec > 7 ) {
                fprintf( out, ")" );
            }
            break;
        case LEQ:
            if( prec > 5 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 5 );
            fprintf( out, "<=" );
            up_expr( out, e->e.binop.e1, 5 );
            if( prec > 5 ) {
                fprintf( out, ")" );
            }
            break;
        case LESS:
            if( prec > 5 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 5 );
            fprintf( out, "<" );
            up_expr( out, e->e.binop.e1, 5 );
            if( prec > 5 ) {
                fprintf( out, ")" );
            }
            break;
        case EQUAL:
            if( prec > 5 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 5 );
            fprintf( out, "==" );
            up_expr( out, e->e.binop.e1, 5 );
            if( prec > 5 ) {
                fprintf( out, ")" );
            }
            break;
        case GREATER:
            if( prec > 5 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 5 );
            fprintf( out, ">" );
            up_expr( out, e->e.binop.e1, 5 );
            if( prec > 5 ) {
                fprintf( out, ")" );
            }
            break;
        case GEQ:
            if( prec > 5 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 5 );
            fprintf( out, ">=" );
            up_expr( out, e->e.binop.e1, 5 );
            if( prec > 5 ) {
                fprintf( out, ")" );
            }
            break;
        case NEQ:
            if( prec > 5 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 5 );
            fprintf( out, "!=" );
            up_expr( out, e->e.binop.e1, 5 );
            if( prec > 5 ) {
                fprintf( out, ")" );
            }
            break;
        case COND:
            fprintf( out, "( " );
            up_expr( out, e->e.cond->e0, 0 );
            fprintf( out, " ? " );
            up_expr( out, e->e.cond->e1, 0 );
            fprintf( out, " : " );
            up_expr( out, e->e.cond->e2, 0 );
            fprintf( out, " )" );
            break;
        case AND:
            if( prec > 2 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 2 );
            fprintf( out, "&&" );
            up_expr( out, e->e.binop.e1, 2 );
            if( prec > 2 ) {
                fprintf( out, ")" );
            }
            break;
        case OR:
            if( prec > 1 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 1 );
            fprintf( out, "||" );
            up_expr( out, e->e.binop.e1, 1 );
            if( prec > 1 ) {
                fprintf( out, ")" );
            }
            break;
        case LOG:
	  fprintf( out, "log( %s, ", e->e.log.target);
            up_expr( out, e->e.log.e0, 1 );
	    fprintf( out, ")" );
            break;
        case UNION:
            if( prec > 4 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 4 );
            fprintf( out, " union " );
            up_expr( out, e->e.binop.e1, 4 );
            if( prec > 4 ) {
                fprintf( out, ")" );
            }
            break;
        case INTERSECTION:
            if( prec > 4 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 4 );
            fprintf( out, " intersection " );
            up_expr( out, e->e.binop.e1, 4 );
            if( prec > 4 ) {
                fprintf( out, ")" );
            }
            break;
        case IN:
            if( prec > 3 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 3 );
            fprintf( out, " in " );
            up_expr( out, e->e.binop.e1, 3 );
            if( prec > 3 ) {
                fprintf( out, ")" );
            }
            break;
        case MEET:
            if( prec > 3 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 3 );
            fprintf( out, " meet " );
            up_expr( out, e->e.binop.e1, 3 );
            if( prec > 3 ) {
                fprintf( out, ")" );
            }
            break;
        case CONCAT:
            if( prec > 11 ) {
                fprintf( out, "(" );
            }
            up_expr( out, e->e.binop.e0, 3 );
            fprintf( out, "++" );
            up_expr( out, e->e.binop.e1, 3 );
            if( prec > 11 ) {
                fprintf( out, ")" );
            }
            break;
        case _ROUND:
            fprintf( out, "%%" );
            up_expr( out, e->e.unop, 10 );
            break;
        case TRUNC:
            fprintf( out, "&" );
            up_expr( out, e->e.unop, 10 );
            break;
        case AMOUNT:
            fprintf( out, "$" );
            up_expr( out, e->e.unop, 10 );
            break;
        case COMPLEMENT:
            fprintf( out, "!" );
            up_expr( out, e->e.unop, 10 );
            break;
        case SIZE:
            fprintf( out, "|" );
            up_expr( out, e->e.unop, 0 );
            fprintf( out, "|" );
            break;
        case NUM_RNV:
            fprintf( out, "numRNV(%s)", e->e.name );
            break;
        case BOOL_RNV:
            fprintf( out, "boolRNV(%s)", e->e.name );
            break;
        case STRING_RNV:
            fprintf( out, "stringRNV(%s)", e->e.name );
            break;
        case DATE_RNV:
            fprintf( out, "dateRNV(%s)", e->e.name );
            break;
        case TIME_RNV:
            fprintf( out, "timeRNV(%s)", e->e.name );
            break;
        case COUNTABLE:
            fprintf( out, "countable(" );
            up_expr( out, e->e.unop, 10 );
            fprintf( out, ")" );
            break;
        case LENGTH:
            fprintf( out, "\\" );
            up_expr( out, e->e.unop, 0 );
            fprintf( out, "\\" );
            break;
        case DOT:
            up_expr( out, e->e.binop.e0, 0 );
            fprintf( out, "." );
            up_expr( out, e->e.binop.e1, 0 );
            break;
        case INTERVAL:
            fprintf( out, "[" );
            up_expr( out, e->e.interval.e0, 0 );
            fprintf( out, "," );
            up_expr( out, e->e.interval.e1, 0 );
            fprintf( out, "]" );
            break;
        case SET:
            fprintf( out, "{" );
            l = e->e.set;
            while( l ) {
                up_expr( out, l->head, 0 );
                l = l->tail;
                if( l ) {
                    fprintf( out, "," );
                }
            }
            fprintf( out, "}" );
            break;
        case AMSET:
            fprintf( out, "{" );
            ml = e->e.amset;
            while( ml ) {
                fprintf( out, "(" );
                up_expr( out, ml->article, 0 );
                fprintf( out, "," );
                up_expr( out, ml->freq, 0 );
                fprintf( out, ")" );
                ml = ml -> tail;
                if( ml ) {
                    fprintf( out, "," );
                }
            }
            fprintf( out, "}" );
            break;
        case PART:
            fprintf( out, "{|" );
            ps = e->e.part;
            while( ps ) {
                fprintf( out, "[" );
                up_expr( out, ps->rep, 0 );
                fprintf( out, "," );
                up_expr( out, ps->clase, 0 );
                fprintf( out, "]" );
                ps = ps -> tail;
                if( ps ) {
                    fprintf( out, "," );
                }
            }
            fprintf( out, "|}" );
            break;
        case STRING:
            fprintf( out, "\"%s\"", e->e.name );
            break;
        case IDENT:
            fprintf( out, "%s", e->e.name );
            break;
        case DEPARTMENT:
            fprintf( out, "department " );
            fprintf( out, "%s", e->e.name );
            //fprintf(out,"");
            break;
        case CLASS:
            fprintf( out, "class(%s,%s)", e->e._class.nivel, e->e._class.concept );
            //fprintf(out,"");
            break;
        case POINT:
            fprintf( out, "point " );
            fprintf( out, "%s", e->e.name );
            //fprintf(out,"");
            break;
        case BONUS:
            fprintf( out, "bonus(%s,%s,", e->e.bonus->target, e->e.bonus->type );
            up_expr( out, e->e.bonus->e0, 0 );
            fprintf( out, ",\"%s\",", e->e.bonus->leyenda );
            up_expr( out, e->e.bonus->maturity, 0 );
            fprintf( out, "," );
            up_expr( out, e->e.bonus->validity, 0 );
            fprintf( out, ")" );
            //fprintf(out,"");
            break;
        case ARTICLE:
            fprintf( out, "article " );
            fprintf( out, "%s", e->e.name );
            //fprintf(out,"");
            break;
        case CARD:
            fprintf( out, "card(%s,%s)", e->e.card.name, e->e.card.plan );
            //fprintf(out,"");
            break;
        case MUTUAL:
            fprintf( out, "mutual " );
            fprintf( out, "%s", e->e.name );
            //fprintf(out,"");
            break;
        case TICKET:
            fprintf( out, "ticket " );
            fprintf( out, "%s", e->e.name );
            //fprintf(out,"");
            break;
        case CHEQUE:
            fprintf( out, "cheque " );
            fprintf( out, "%s", e->e.name );
            //fprintf(out,"");
            break;
        case CURRENCY:
            fprintf( out, "currency " );
            fprintf( out, "%s", e->e.name );
            //fprintf(out,"");
            break;
        case TRUE:
            fprintf( out, "true" );
            break;
        case FALSE:
            fprintf( out, "false" );
            break;
        case INPUT:
            fprintf( out, "input(" );
            up_expr( out, e->e.unop, 10 );
            fprintf( out, ")" );
            break;
        case NUMINPUT:
            fprintf( out, "numinput(" );
            up_expr( out, e->e.unop, 10 );
            fprintf( out, ")" );
            break;
        case HOLIDAY:
            fprintf( out, "holiday" );
            break;
        case AFFILIATED:
            fprintf( out, "affiliated" );
            break;
        case ALL:
            fprintf( out, "all" );
            break;
            /*
            case PURCHASE:
            fprintf(out,"purchase");
            break;
            case WPURCHASE:
            fprintf(out,"wholepurchase");
            break;
            */
        case BUYED:
            fprintf( out, "buyed_articles" );
            break;
        case DELIVERED:
            fprintf( out, "delivered_articles" );
            break;
        case DISCOUNTS:
            fprintf( out, "discount_credits" );
            break;
        case ADDED:
            fprintf( out, "added_articles" );
            break;

        case TIMEV:
            {
                char d[7];
                strtime( d, 6, e->e.time );
                fprintf( out, "%s", d );
            }
            break;
        case DATEV:
            {
                char d[15];
                strdate( d, 14, e->e.date, 0 );
                fprintf( out, "%s", d );
            }
            break;
        case TIME:
            {
                char d[7];
                strtime( d, 6, gettime_() );
                fprintf( out, "time(%s)", d );
            }
            break;
        case DATE:
            {
                char d[15];
                strdate( d, 14, getdate_(), 0 );
                fprintf( out, "date(%s)", d );
            }
            break;
        case DATEOFBIRTH:
            fprintf( out, "dateofbirth" );
            break;
        case _DAY:
            fprintf( out, "day" );
            break;
        case CASH:
            fprintf( out, "cash" );
            break;
        case CTACTE:
            fprintf( out, "ctacte" );
            break;
        case MEANS:
            fprintf( out, "means" );
            break;
        case CHANGE:
            fprintf( out, "change" );
            break;
        case RETURNED:
            fprintf( out, "returned" );
            break;
        case POINTS:
            fprintf( out, "points" );
            break;
        case CASHDESK:
            fprintf( out, "cashdesk" );
            break;
        case MONDAY:
            fprintf( out, "monday" );
            break;
        case TUESDAY:
            fprintf( out, "tuesday" );
            break;
        case WEDNESDAY:
            fprintf( out, "wednesday" );
            break;
        case THURSDAY:
            fprintf( out, "thursday" );
            break;
        case FRIDAY:
            fprintf( out, "friday" );
            break;
        case SATURDAY:
            fprintf( out, "saturday" );
            break;
        case SUNDAY:
            fprintf( out, "sunday" );
            break;
        case DAYSELECTOR:
            fprintf( out, "day" );
            break;
        case MONTHSELECTOR:
            fprintf( out, "month" );
            break;
        case YEARSELECTOR:
            fprintf( out, "year" );
            break;

        case REQ_ANY:
            fprintf( out, "requireAny {" );
            ml = e->e.amset;
            while( ml ) {
                fprintf( out, "(" );
                fprintf( out, "%.2lf", *((double*)(ml->article)) );
                //up_expr( out, ml->article, 0 );
                fprintf( out, "," );
                up_expr( out, ml->freq, 0 );
                fprintf( out, ")" );
                ml = ml -> tail;
                if( ml ) {
                    fprintf( out, "," );
                }
            }
            fprintf( out, "}" );
            break;

        case REQ_ALL:
            fprintf( out, "requireAll {" );
            ml = e->e.amset;
            while( ml ) {
                fprintf( out, "(" );
                fprintf( out, "%.2lf", *((double*)(ml->article)) );
                //up_expr( out, ml->article, 0 );
                fprintf( out, "," );
                up_expr( out, ml->freq, 0 );
                fprintf( out, ")" );
                ml = ml -> tail;
                if( ml ) {
                    fprintf( out, "," );
                }
            }
            fprintf( out, "}" );
            break;

        case REQ_DEP:
            fprintf( out, "require ( [" );
            ml = (mlist) e->e.binop.e0;
            while( ml ) {
                up_expr( out, ml->article, 0 );
                ml = ml -> tail;
                if( ml ) {
                    fprintf( out, "] ," );
                }
            }
            fprintf( out, ", " );
            up_expr( out, e->e.binop.e1, 0 );
            fprintf( out, ")" );
            break;

        default :
            fprintf( out, "?" );
    }
}


void up_benef( FILE *out, benef b, int offset )
{
    switch( b->tag ) {
        case ASSIGN:
            fprintf( out, "%*s", offset, "" );
            fprintf( out, "%s = ", b->b.assign.var );
            up_expr( out, b->b.assign.e, 0 );
            break;
        case IF:
            fprintf( out, "%*s", offset, "" );
            fprintf( out, "if " );
            up_expr( out, b->b.cond.b, 0 );
            fprintf( out, " then\n" );
            up_benef( out, b->b.cond.c1, offset + 4 );
            fprintf( out, "\n%*s", offset, "" );
            fprintf( out, "else\n" );
            up_benef( out, b->b.cond.c2, offset + 4 );
            fprintf( out, "\n%*s", offset, "" );
            fprintf( out, "fi" );
            break;
        case FOR:
            fprintf( out, "%*s", offset, "" );
            fprintf( out, "for (%s,%s) in ", b->b.foreach.a, b->b.foreach.k );
            up_expr( out, b->b.foreach.e, 0 );
            fprintf( out, " do\n" );
            up_benef( out, b->b.foreach.c, offset + 4 );
            fprintf( out, "\n%*s", offset, "" );
            fprintf( out, "od" );
            break;
        case BPRINT:
            fprintf( out, "%*s", offset, "" );
            fprintf( out, "print(" );
            up_expr( out, b->b.print, 0 );
            fprintf( out, ")" );
            break;
        case CREDIT:
            fprintf( out, "%*s", offset, "" );
            fprintf( out, "credit(" );
            up_expr( out, b->b.credit.e4, 0 );
            fprintf( out, "," );
            up_expr( out, b->b.credit.e1, 0 );
            fprintf( out, "," );
            up_expr( out, b->b.credit.e2, 0 );
            fprintf( out, "," );
            up_expr( out, b->b.credit.e3, 0 );
            fprintf( out, ")" );
            break;
        case DISTRIBUTE:
            fprintf( out, "%*s", offset, "" );
            fprintf( out, "distribute(" );
            up_expr( out, b->b.distribute.e1, 0 );
            fprintf( out, "," );
            up_expr( out, b->b.distribute.e2, 0 );
            fprintf( out, "," );
            up_expr( out, b->b.distribute.e3, 0 );
            fprintf( out, ")" );
            break;
        case ISSUE_BONUS:
            fprintf( out, "%*s", offset, "" );
            fprintf( out, "issueBonus(" );
            up_expr( out, b->b.issueBonus.e1, 0 );
            fprintf( out, "," );
            up_expr( out, b->b.issueBonus.e2, 0 );
            fprintf( out, ")" );
            break;
        case GIVE_POINTS:
            fprintf( out, "%*s", offset, "" );
            fprintf( out, "givePoints(" );
            up_expr( out, b->b.givePoints.e1, 0 );
            fprintf( out, "," );
            up_expr( out, b->b.givePoints.e2, 0 );
            fprintf( out, ")" );
            break;
        case ACTIVATE:
            fprintf( out, "%*s", offset, "" );
            fprintf( out, "activateMean(" );
            up_expr( out, b->b.activate.e1, 0 );
            fprintf( out, "," );
            up_expr( out, b->b.activate.e2, 0 );
            fprintf( out, ")" );
            break;
        case EPROC:
            fprintf( out, "%*s", offset, "" );
            fprintf( out, "eproc(%s,", b->b.eproc.procName );
            up_expr( out, b->b.eproc.params, 0 );
            fprintf( out, ")" );
            break;
        case REQ_MEANS:
	    if ( b->b.reqMeans.e2 ) {
	      fprintf( out, "%*s", offset, "" );
	      fprintf( out, "requireMeans(" );
	      up_expr( out, b->b.reqMeans.e1, 0 );
	      fprintf( out, "," );
	      up_expr( out, b->b.reqMeans.e2, 0 );
	      fprintf( out, ")" );
	    } else {
	      fprintf( out, "%*s", offset, "" );
	      fprintf( out, "requireMeans " );
	      up_expr( out, b->b.reqMeans.e1, 0 );
	    }
            break;



	       /* REGISTRO DE EVENTOS 
	       REC,
	       REC_PROMOID, REC_PROMOID_OLD,
	       REC_ARTS, REC_DEP, REC_MEAN, REC_CLASS, REC_ALL, REC_PERFIL,
	       REC_CREDIT, REC_BONUS, REC_POINTS, REC_ACTIVATE_MEAN,
	       REC_STRING,
		*/
	case REC:
	    switch (b->b.rec.type) {
	        case REC_PROMOID:
	            fprintf( out, "%*s", offset, "" );
		    fprintf( out, "rec(promoId, " );
		    up_expr( out, b->b.rec.e1, 0 );
		    fprintf( out, ", " );
		    up_expr( out, b->b.rec.e2, 0 );
		    fprintf( out, ")" );
	            break;
	        case REC_PROMOID_OLD:
	            fprintf( out, "%*s", offset, "" );
		    fprintf( out, "rec(promoId, " );
		    up_expr( out, b->b.rec.e1, 0 );
		    fprintf( out, ")" );
	            break;
		case REC_ARTS:
	            fprintf( out, "%*s", offset, "" );
		    fprintf( out, "rec(artsBeneficiary, " );
		    fprintf( out, ", " );
		    up_expr( out, b->b.rec.e1, 0 );
		    fprintf( out, ")" );
	            break;
		case REC_DEP:
	            fprintf( out, "%*s", offset, "" );
		    fprintf( out, "rec(depBeneficiary, " );
		    fprintf( out, ", " );
		    up_expr( out, b->b.rec.e1, 0 );
		    fprintf( out, ")" );
	            break;
		case REC_MEAN:
	            fprintf( out, "%*s", offset, "" );
		    fprintf( out, "rec(meanBeneficiary, " );
		    fprintf( out, ", " );
		    up_expr( out, b->b.rec.e1, 0 );
		    fprintf( out, ")" );
	            break;
		case REC_CLASS:
	            fprintf( out, "%*s", offset, "" );
		    fprintf( out, "rec(classBeneficiary, " );
		    fprintf( out, ", " );
		    up_expr( out, b->b.rec.e1, 0 );
		    fprintf( out, ")" );
	            break;
		case REC_ALL:
	            fprintf( out, "%*s", offset, "" );
		    fprintf( out, "rec(allBeneficiary)" );
	            break;
		case REC_PERFIL:
	            fprintf( out, "%*s", offset, "" );
		    fprintf( out, "rec(perfil, " );
		    fprintf( out, ", " );
		    up_expr( out, b->b.rec.e1, 0 );
		    fprintf( out, ")" );
	            break;
		case REC_CREDIT:
	            fprintf( out, "%*s", offset, "" );
		    fprintf( out, "rec(creditBenefit, " );
		    fprintf( out, ", " );
		    up_expr( out, b->b.rec.e1, 0 );
		    fprintf( out, ")" );
	            break;
		case REC_BONUS:
	            fprintf( out, "%*s", offset, "" );
		    fprintf( out, "rec(bonusBenefit, " );
		    fprintf( out, ", " );
		    up_expr( out, b->b.rec.e1, 0 );
		    fprintf( out, ", " );
		    up_expr( out, b->b.rec.e2, 0 );
		    fprintf( out, ")" );
	            break;
		case REC_POINTS:
	            fprintf( out, "%*s", offset, "" );
		    fprintf( out, "rec(pointsBenefit, " );
		    fprintf( out, ", " );
		    up_expr( out, b->b.rec.e1, 0 );
		    fprintf( out, ", " );
		    up_expr( out, b->b.rec.e2, 0 );
		    fprintf( out, ")" );
	            break;
		case REC_ACTIVATE_MEAN:
	            fprintf( out, "%*s", offset, "" );
		    fprintf( out, "rec(meanBenefit, " );
		    fprintf( out, ", " );
		    up_expr( out, b->b.rec.e1, 0 );
		    fprintf( out, ", " );
		    up_expr( out, b->b.rec.e2, 0 );
		    fprintf( out, ")" );
	            break;
		case REC_STRING:
	            fprintf( out, "%*s", offset, "" );
		    fprintf( out, "rec(" );
		    up_expr( out, b->b.rec.e1, 0 );
		    fprintf( out, ", " );
		    up_expr( out, b->b.rec.e2, 0 );
		    fprintf( out, ")" );
		    break;
	    	default:
		    fprintf( out, "tipo de comando rec no reconocido\n" );
		    break;
	    }
	    break;

        case __SKIP:
            fprintf( out, "%*s", offset, "" );
            fprintf( out, "skip" );
            break;
        case COMP:
            up_benef( out, b->b.comp.c1, offset );
            fprintf( out, ";\n" );
            up_benef( out, b->b.comp.c2, offset );
            break;
        case NUM_RNV_ASSIGN:
            fprintf( out, "%*s", offset, "" );
            fprintf( out, "numRnv(%s) = ", b->b.assign.var );
            up_expr( out, b->b.assign.e, 0 );
            break;
        case BOOL_RNV_ASSIGN:
            fprintf( out, "%*s", offset, "" );
            fprintf( out, "boolRnv(%s) = ", b->b.assign.var );
            up_expr( out, b->b.assign.e, 0 );
            break;
        case STRING_RNV_ASSIGN:
            fprintf( out, "%*s", offset, "" );
            fprintf( out, "stringRNV(%s) = ", b->b.assign.var );
            up_expr( out, b->b.assign.e, 0 );
            break;
        case DATE_RNV_ASSIGN:
            fprintf( out, "%*s", offset, "" );
            fprintf( out, "dateRNV(%s) = ", b->b.assign.var );
            up_expr( out, b->b.assign.e, 0 );
            break;
        case TIME_RNV_ASSIGN:
            fprintf( out, "%*s", offset, "" );
            fprintf( out, "timeRNV(%s) = ", b->b.assign.var );
            up_expr( out, b->b.assign.e, 0 );
            break;
        default:
            fprintf( out, "Commando irreconocible\n" );
    }
}




/* Funcion principal que imprime una promocion en pantalla */

void up_promo( FILE *out, promo p )
{
    list es = p->externs;
    list pcs = p->preconditions;
    param ps = p->parameters;
    list cs = p->conditions;
    benef bs = p->benefits;

    //fprintf(out,"Promotion %s\n",p->name);
    fprintf( out, "Promotion " );

    switch( p->evalTime ) {
        case ONLINE:
            fprintf( out, "online " );
            break;
        case PREPAGO:
            fprintf( out, "prepago " );
            break;
        case POSTPAGO:
            fprintf( out, "postpago " );
            break;
        case EVER:
            fprintf( out, "ever " );
            break;
        default:
            fprintf( out, "ERROR " );
            break;
    }

    fprintf( out, "%s\n", p->name );
    fprintf( out, "  PreConditions\n" );
    while( pcs ) {
        fprintf( out, "%*s", 4, "" );
        up_expr( out, pcs->head, 0 );
        pcs = pcs->tail;
        if( pcs ) {
            fprintf( out, ";\n" );
        }
    }
    fprintf( out, "\n" );
    fprintf( out, "  Parameters\n" );
    while( es ) {
        fprintf( out, "%*s", 4, "" );
        fprintf( out, "extern " );
        fprintf( out, es->head->e.name );
        es = es->tail;
        if( es ) {
            fprintf( out, ";\n" );
        }
    }
    fprintf( out, "\n" );
    while( ps ) {
        fprintf( out, "%*s", 4, "" );
        if( ps->tag == GLOBAL ) {
            fprintf( out, "global " );
        }
        else if( ps->tag == STATIC ) {
            fprintf( out, "static " );
        }
        else if( ps->tag == EXTERN ) {
            fprintf( out, "extern " );
        }
        fprintf( out, ps->var );
        if( ps->tag != EXTERN ) {
            fprintf( out, " = " );
            up_expr( out, ps->e, 0 );
        }
        ps = ps->nextParams;
        if( ps ) {
            fprintf( out, ";\n" );
        }
    }
    fprintf( out, "\n" );
    fprintf( out, "  Conditions\n" );
    while( cs ) {
        fprintf( out, "%*s", 4, "" );
        up_expr( out, cs->head, 0 );
        cs = cs->tail;
        if( cs ) {
            fprintf( out, ";\n" );
        }
    }
    fprintf( out, "\n" );
    fprintf( out, "  Benefits\n" );
    up_benef( out, bs, 4 );
    fprintf( out, "\n" );
    if( p->cancellation != NULL ) {
        fprintf( out, "Cancellation " );
        up_promo( out, p->cancellation );
    }
}

void up_promos( FILE *out, promo p )
{
    promo ps = p;
    if( ps == NULL ) {
        return;
    }
    do {
        up_promo( out, ps );
        if( ps->nextPromo != NULL ) {
            fprintf( out, "//-------------------------//\n" );
        }
        ps = ps->nextPromo;
    }
    while( ps );
}


//#endif


