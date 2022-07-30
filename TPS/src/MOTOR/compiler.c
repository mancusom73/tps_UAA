/*! \file compiler.c
    \brief Escritura/Lectura de un árbol sintáctico de promociones a/de un archivo.

  Este fuente implementa las funciones de escritura de un árbol sintáctico de
  promociones a un archivo. 
  También implementa la lectura de dicho archivo, devolviéndo el árbol sintáctico
  en memoria equivalente al original que fué escrito.

  Los Casos de Uso realacionados son:
  <A HREF="../../../2-A&D/Use%20Case/(011-0015)_Controlar_Version_del_Codigo_Binario_de_Promocion.xml">
    (011-0015) Controlar Version del Codigo Binario de Promocion</A>,
  <A HREF="../../../2-A&D/Use%20Case/(011-0016)_Cargar_Promociones_Globales.xml">
    (011-0016) Cargar Promociones Globales</A>,
  <A HREF="../../../2-A&D/Use%20Case/(011-0017)_Cargar_Programa_de_Promociones.xml">
    (011-0017) Cargar Programa de Promociones</A>.
  
  */



#pragma pack (1) 
//#ifdef MODULO_PROMOCIONES
//#define PROMO_STR
#if defined(INVEL_W) || defined(INVEL_L)
#include "cr.h"
#endif
#include <stdio.h>
#include <string.h>
#include "etpv.h"
#include "compiler.h"
#include "types2.h"
#include "utils.h"
#include "time2.h"
#include <fcntl.h>
#include <ini.h>
/**/
#include "uprinter.h"
/**/
#include "lib_req.h"
#include "lib_dreq.h"


#pragma pack (1)

#define SIZEOF_INT 2

int fputint(int c, FILE *flujo) {
  int i;
  int r = 0;
  for (i=0;i<SIZEOF_INT;i++) {
    r += fputc( ((char*)(&c))[i], flujo  );
  }
  return r;
}

int fgetint(FILE *flujo) {
  int i;
  int r = 0;
  for (i=0;i<SIZEOF_INT;i++) {
    ((char*)(&r))[i] += fgetc( flujo );
  }
  return r;
}


/*
 * codifico todo con bytes (unsigned char)
 */
void compile_expr( expr e, FILE *cs )
{
    unsigned char *doubleing;
    unsigned char *valor;
    int set_size;
    list ls,ls_tmp;
    mlist mls,mls_tmp;
    part ps,ps_tmp;
    char *ident;
    int index;

    switch( e->tag ) {
        case NUM:
            fputc( MkNUM, cs ); //cuidado con estos... estoy suponiendo tExpr <= 255
            doubleing = ( unsigned char* )( &e->e.num );
            for( index = 0;index < sizeof( double );index++ ) {
                fputc( doubleing[index], cs );
            }
            break;
        case SUM:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkSUM, cs );
            break;
        case SUB:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkSUB, cs );
            break;
        case MULT:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkMULT, cs );
            break;
        case FDIV:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkFDIV, cs );
            break;
        case MOD:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkMOD, cs );
            break;
        case DIV:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkDIV, cs );
            break;
        case MAX:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkMAX, cs );
            break;
        case MIN:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkMIN, cs );
            break;
        case LEQ:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkLEQ, cs );
            break;
        case LESS:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkLESS, cs );
            break;
        case EQUAL:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkEQUAL, cs );
            break;
        case GREATER:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkGREATER, cs );
            break;
        case GEQ:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkGEQ, cs );
            break;
        case NEQ:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkNEQ, cs );
            break;
        case COND:
            compile_expr( e->e.cond->e2, cs );
            compile_expr( e->e.cond->e1, cs );
            compile_expr( e->e.cond->e0, cs );
            fputc( MkCOND, cs );
            free( e->e.cond );
            break;
        case AND:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkAND, cs );
            break;
        case OR:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkOR, cs );
            break;

        case LOG:
            compile_expr( e->e.log.e0, cs );
            fputc( MkLOG, cs );

            /* target */
            ident = e->e.log.target;
            index = strlen( ident );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );

            break;

        case UNION:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkUNION, cs );
            break;
        case INTERSECTION:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkINTERSECTION, cs );
            break;
        case IN:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkIN, cs );
            break;
        case MEET:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkMEET, cs );
            break;
        case CONCAT:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkCONCAT, cs );
            break;
        case _ROUND:
            compile_expr( e->e.unop, cs );
            fputc( MkROUND, cs );
            break;
        case TRUNC:
            compile_expr( e->e.unop, cs );
            fputc( MkTRUNC, cs );
            break;
        case AMOUNT:
            compile_expr( e->e.unop, cs );
            fputc( MkAMOUNT, cs );
            break;
        case COMPLEMENT:
            compile_expr( e->e.unop, cs );
            fputc( MkCOMPLEMENT, cs );
            break;
        case INPUT:
            compile_expr( e->e.unop, cs );
            fputc( MkINPUT, cs );
            break;
        case NUMINPUT:
            compile_expr( e->e.unop, cs );
            fputc( MkNUMINPUT, cs );
            break;
        case SIZE:
            compile_expr( e->e.unop, cs );
            fputc( MkSIZE, cs );
            break;
        case LENGTH:
            compile_expr( e->e.unop, cs );
            fputc( MkLENGTH, cs );
            break;
        case COUNTABLE:
            compile_expr( e->e.unop, cs );
            fputc( MkCOUNTABLE, cs );
            break;
        case NUM_RNV:
            ident = e->e.name;
            index = strlen( ident );
            fputc( MkNUM_RNV, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            break;
        case BOOL_RNV:
            ident = e->e.name;
            index = strlen( ident );
            fputc( MkBOOL_RNV, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            break;
        case STRING_RNV:
            ident = e->e.name;
            index = strlen( ident );
            fputc( MkSTRING_RNV, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            break;
        case DATE_RNV:
            ident = e->e.name;
            index = strlen( ident );
            fputc( MkDATE_RNV, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            break;
        case TIME_RNV:
            ident = e->e.name;
            index = strlen( ident );
            fputc( MkTIME_RNV, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            break;

        case DOT:
            compile_expr( e->e.binop.e1, cs );
            compile_expr( e->e.binop.e0, cs );
            fputc( MkDOT, cs );
            break;
        case INTERVAL:
            compile_expr( e->e.interval.e1, cs );
            compile_expr( e->e.interval.e0, cs );
            fputc( MkINTERVAL, cs );
            break;

        case SET:
            ls = e->e.set;
            set_size = 0;
            while( ls ) {
                compile_expr( ls->head, cs );
                set_size++;
                ls_tmp = ls;
                ls = ls->tail;
                free( ls_tmp );
            }
            fputc( MkSET, cs );
            fputint( set_size, cs );
            //fputc( set_size, cs ); //cuidado con esto. Estoy suponiendo (length lista <= 256)
            break;

        case AMSET:
            mls = e->e.amset;
            set_size = 0;
            while( mls ) {
                compile_expr( mls->freq, cs );
                compile_expr( mls->article, cs );
                set_size++;
                mls_tmp = mls;
                mls = mls->tail;
                free( mls_tmp );
            }
            fputc( MkAMSET, cs );
            fputint( set_size, cs );
            //fputc( set_size, cs ); //cuidado con esto. Estoy suponiendo (length lista <= 256)
            break;

        case PART:
            ps = e->e.part;
            set_size = 0;
            while( ps ) {
                compile_expr( ps->clase, cs );
                compile_expr( ps->rep, cs );
                set_size++;
                ps_tmp = ps;
                ps = ps->tail;
                free( ps_tmp );
            }
            fputc( MkPART, cs );
            fputint( set_size, cs );
            //fputc( set_size, cs ); //cuidado con esto. Estoy suponiendo (length lista <= 256)
            break;


        case DATEV:
            fputc( MkDATEV, cs ); //cuidado con estos... estoy suponiendo tExpr <= 255
            valor = ( unsigned char* )( &e->e.date );
            for( index = 0;index < sizeof( idate );index++ ) {
                fputc( valor[index], cs );
            }
            break;
        case TIMEV:
            fputc( MkTIMEV, cs ); //cuidado con estos... estoy suponiendo tExpr <= 255
            valor = ( unsigned char* )( &e->e.time );
            for( index = 0;index < sizeof( itime );index++ ) {
                fputc( valor[index], cs );
            }
            break;

            /* ident s */
        case STRING:
            ident = e->e.name;
            index = strlen( ident );
            fputc( MkSTRING, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            break;
        case IDENT:
            ident = e->e.name;
            index = strlen( ident );
            fputc( MkIDENT, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            break;
        case DEPARTMENT:
            ident = e->e.name;
            index = strlen( ident );
            fputc( MkDEPARTMENT, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            break;
        case CLASS:
            fputc( MkCLASS, cs );
            ident = e->e._class.nivel;
            index = strlen( ident );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            ident = e->e._class.concept;
            index = strlen( ident );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            break;
        case POINT:
            ident = e->e.name;
            index = strlen( ident );
            fputc( MkPOINT, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            break;
        case BONUS:
            /* primero la expresion del valor del bono */
            compile_expr( e->e.bonus->e0, cs ); // expresion que expresa el valor del bono

            /* fecha de vencimiento */
            compile_expr( e->e.bonus->maturity, cs );

            /* cantidad de dias de validez del bono */
            compile_expr( e->e.bonus->validity, cs ); 

            fputc( MkBONUS, cs );

            /* target */
            ident = e->e.bonus->target;
            index = strlen( ident );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );

            /* type */
            ident = e->e.bonus->type;
            index = strlen( ident );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );

            /* leyenda */
            ident = e->e.bonus->leyenda;
            index = strlen( ident );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            free( e->e.bonus );

            break;

        case ARTICLE:
            ident = e->e.name;
            index = strlen( ident );
            fputc( MkARTICLE, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            break;
        case CARD:
            fputc( MkCARD, cs );
            ident = e->e.card.name;
            index = strlen( ident );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            ident = e->e.card.plan;
            index = strlen( ident );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            break;
        case MUTUAL:
            ident = e->e.name;
            index = strlen( ident );
            fputc( MkMUTUAL, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            break;
        case TICKET:
            ident = e->e.name;
            index = strlen( ident );
            fputc( MkTICKET, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            break;
        case CHEQUE:
            ident = e->e.name;
            index = strlen( ident );
            fputc( MkCHEQUE, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            break;
        case CURRENCY:
            ident = e->e.name;
            index = strlen( ident );
            fputc( MkCURRENCY, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            break;

            /* CTES */
        case TRUE:
            fputc( MkTRUE, cs );
            break;
        case FALSE:
            fputc( MkFALSE, cs );
            break;
        case HOLIDAY:
            fputc( MkHOLIDAY, cs );
            break;
        case AFFILIATED:
            fputc( MkAFFILIATED, cs );
            break;
        case ALL:
            fputc( MkALL, cs );
            break;
            /*
            case PURCHASE:
            fputc(MkPURCHASE, cs);
            break;
            case WPURCHASE:
            fputc(MkWPURCHASE, cs);
            break;
             */
        case BUYED:
            fputc( MkBUYED, cs );
            break;
        case DELIVERED:
            fputc( MkDELIVERED, cs );
            break;
        case DISCOUNTS:
            fputc( MkDISCOUNTS, cs );
            break;
        case ADDED:
            fputc( MkADDED, cs );
            break;
        case TIME:
            fputc( MkTIME, cs );
            break;
        case DATE:
            fputc( MkDATE, cs );
            break;
        case DATEOFBIRTH:
            fputc( MkDATEOFBIRTH, cs );
            break;
        case _DAY:
            fputc( MkDAY, cs );
            break;
        case CASH:
            fputc( MkCASH, cs );
            break;
        case CTACTE:
            fputc( MkCTACTE, cs );
            break;
        case MEANS:
            fputc( MkMEANS, cs );
            break;
        case CHANGE:
            fputc( MkCHANGE, cs );
            break;
        case RETURNED:
            fputc( MkRETURNED, cs );
            break;
        case POINTS:
            fputc( MkPOINTS, cs );
            break;
        case EBONUS:
            fputc( MkEBONUS, cs );
            break;
        case CASHDESK:
            fputc( MkCASHDESK, cs );
            break;
        case DAYSELECTOR:
            fputc( MkDAYSELECTOR, cs );
            break;
        case MONTHSELECTOR:
            fputc( MkMONTHSELECTOR, cs );
            break;
        case YEARSELECTOR:
            fputc( MkYEARSELECTOR, cs );
            break;
        case MONDAY:
            fputc( MkMONDAY, cs );
            break;
        case TUESDAY:
            fputc( MkTUESDAY, cs );
            break;
        case WEDNESDAY:
            fputc( MkWEDNESDAY, cs );
            break;
        case THURSDAY:
            fputc( MkTHURSDAY, cs );
            break;
        case FRIDAY:
            fputc( MkFRIDAY, cs );
            break;
        case SATURDAY:
            fputc( MkSATURDAY, cs );
            break;
        case SUNDAY:
            fputc( MkSUNDAY, cs );
            break;

        case REQ_ANY:
            mls = e->e.amset;
            set_size = 0;
            while( mls ) {
                compile_expr( mls->freq, cs );
                compile_expr( mls->article, cs );
                set_size++;
                mls_tmp = mls;
                mls = mls->tail;
                free( mls_tmp );
            }
            fputc( MkREQ_ANY, cs );
            fputint( set_size, cs );
            //fputc( set_size, cs ); //cuidado con esto. Estoy suponiendo (length lista <= 256)
            break;

        case REQ_ALL:
            mls = e->e.amset;
            set_size = 0;
            while( mls ) {
                compile_expr( mls->freq, cs );
                compile_expr( mls->article, cs );
                set_size++;
                mls_tmp = mls;
                mls = mls->tail;
                free( mls_tmp );
            }
            fputc( MkREQ_ALL, cs );
            fputint( set_size, cs );
            //fputc( set_size, cs ); //cuidado con esto. Estoy suponiendo (length lista <= 256)
            break;

        case REQ_DEP:
            compile_expr( e->e.binop.e1, cs );
            mls = (mlist) e->e.binop.e0;
            set_size = 0;
            while( mls ) {
                compile_expr( mls->freq, cs );
                compile_expr( mls->article, cs );
                set_size++;
                mls_tmp = mls;
                mls = mls->tail;
                free( mls_tmp );
            }
            fputc( MkREQ_DEP, cs );
            fputint( set_size, cs );
            //fputc( set_size, cs ); //cuidado con esto. Estoy suponiendo (length lista <= 256)
            break;

        default:
		    //fprintf(stderr,ST( S_COMPILE_EXPR__CASO__I_NO_CONTEMPLADON ),e->tag);
		    glog(ST( S_COMPILE_EXPR__CASO__I_NO_CONTEMPLADON ),LOG_MOTOR_PROMO,3);
            break;
    }

    free( e );
}


void compile_benef( benef b, FILE *cs )
{
    char *ident;
    int index;
    switch( b->tag ) {
        case ASSIGN:
            compile_expr( b->b.assign.e, cs );
            ident = b->b.assign.var;
            index = strlen( ident );
            fputc( MkASSIGN, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            free( b );
            break;
        case IF:
            compile_benef( b->b.cond.c2, cs );
            compile_benef( b->b.cond.c1, cs );
            compile_expr( b->b.cond.b, cs );
            fputc( MkIF, cs );
            free( b );
            break;

        case FOR:
            compile_benef( b->b.foreach.c, cs );
            compile_expr( b->b.foreach.e, cs );
            fputc( MkFOR, cs );
            ident = b->b.foreach.k;
            index = strlen( ident );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            ident = b->b.foreach.a;
            index = strlen( ident );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            free( b );
            break;
        case BPRINT:
            // 27/01/05 cambiado por conflicto de nombres (antes era PRINT)
            compile_expr( b->b.print, cs );
            fputc( MkPRINT, cs );
            free( b );
            break;
        case CREDIT:
            compile_expr( b->b.credit.e4, cs );
            compile_expr( b->b.credit.e3, cs );
            compile_expr( b->b.credit.e2, cs );
            compile_expr( b->b.credit.e1, cs );
            fputc( MkCREDIT, cs );
	    //free( b->b.credit.e3 ); //<--no debería ir algo como esto???
            free( b );
            break;
        case DISTRIBUTE:
            compile_expr( b->b.distribute.e3, cs );
            compile_expr( b->b.distribute.e2, cs );
            compile_expr( b->b.distribute.e1, cs );
            fputc( MkDISTRIBUTE, cs );
            free( b );
            break;
        case ISSUE_BONUS:
            compile_expr( b->b.issueBonus.e2, cs );
            compile_expr( b->b.issueBonus.e1, cs );
            fputc( MkISSUE_BONUS, cs );
            free( b );
            break;
        case GIVE_POINTS:
            compile_expr( b->b.givePoints.e2, cs );
            compile_expr( b->b.givePoints.e1, cs );
            fputc( MkGIVE_POINTS, cs );
            free( b );
            break;
        case ACTIVATE:
            compile_expr( b->b.activate.e2, cs );
            compile_expr( b->b.activate.e1, cs );
            fputc( MkACTIVATE, cs );
            free( b );
            break;
        case EPROC:
            compile_expr( b->b.eproc.params, cs );
            ident = b->b.eproc.procName;
            index = strlen( ident );
            fputc( MkEPROC, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            free( b );
            break;
        case REQ_MEANS:
	    if ( b->b.reqMeans.e2 ) compile_expr( b->b.reqMeans.e2, cs );
            compile_expr( b->b.reqMeans.e1, cs );
            if ( b->b.reqMeans.e2 ) {
	      fputc( MkREQ_MEANS_CANT, cs );
	    } else {
	      fputc( MkREQ_MEANS, cs );
	    }
            free( b );
            break;
        case REC:
            switch( b->b.rec.type ) {
                case REC_PROMOID_OLD:
                case REC_ARTS:
                case REC_DEP:
                case REC_MEAN:
                case REC_CLASS:
                case REC_PERFIL:
                case REC_CREDIT:
                    compile_expr( b->b.rec.e1, cs );
                    break;
                case REC_BONUS:
                case REC_POINTS:
                case REC_ACTIVATE_MEAN:
                case REC_STRING:
                case REC_PROMOID:
                    compile_expr( b->b.rec.e1, cs );
                    compile_expr( b->b.rec.e2, cs );
                    break;
                case REC_ALL:
                    break;
                default:
                    break;
            }
            fputc( MkREC, cs );
            fputc( b->b.rec.type, cs );  // CUIDADO: Estoy asumiendo que tBenef < 256 ---------
            free( b );
            break;
        case __SKIP:
            fputc( MkSKIP, cs );
            free( b );
            break;
        case COMP:
            compile_benef( b->b.comp.c2, cs );
            compile_benef( b->b.comp.c1, cs );
            fputc( MkCOMP, cs );
            free( b );
            break;

        case NUM_RNV_ASSIGN:
            compile_expr( b->b.assign.e, cs );
            ident = b->b.assign.var;
            index = strlen( ident );
            fputc( MkNUM_RNV_ASSIGN, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            free( b );
            break;
        case BOOL_RNV_ASSIGN:
            compile_expr( b->b.assign.e, cs );
            ident = b->b.assign.var;
            index = strlen( ident );
            fputc( MkBOOL_RNV_ASSIGN, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            free( b );
            break;
        case STRING_RNV_ASSIGN:
            compile_expr( b->b.assign.e, cs );
            ident = b->b.assign.var;
            index = strlen( ident );
            fputc( MkSTRING_RNV_ASSIGN, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            free( b );
            break;
        case DATE_RNV_ASSIGN:
            compile_expr( b->b.assign.e, cs );
            ident = b->b.assign.var;
            index = strlen( ident );
            fputc( MkDATE_RNV_ASSIGN, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            free( b );
            break;
        case TIME_RNV_ASSIGN:
            compile_expr( b->b.assign.e, cs );
            ident = b->b.assign.var;
            index = strlen( ident );
            fputc( MkTIME_RNV_ASSIGN, cs );
            fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
            index = 0;
            while( ident[index] != '\0' ) {
                fputc( ident[index], cs );
                index++;
            }
            free( ident );
            free( b );
            break;


        default:
		    //fprintf(stderr,ST( S_CASO_DE_BENEF_NO_CONTEMPLADO ));
		    glog(ST( S_CASO_DE_BENEF_NO_CONTEMPLADO ),LOG_MOTOR_PROMO,3);
			break;
    }
}


void compile_param( param p, FILE *cs )
{
    int index;
    char *ident;

    if( p->tag == LOCAL ) {
        compile_expr( p->e, cs );
        fputc( MkPARAM, cs );
    }
    else if( p->tag == GLOBAL ) {
        compile_expr( p->e, cs );
        fputc( MkPARAMGLOBAL, cs );
    }
    else if( p->tag == EXTERN ) {
        fputc( MkPARAMEXTERN, cs );
    }
    else if( p->tag == STATIC ) {
        compile_expr( p->e, cs );
        fputc( MkPARAMSTATIC, cs );
    }
    else {
        perror( "compile_param: caso no contemplado." );
        exit( 1 );
    }
    ident = p->var;
    index = strlen( ident );
    fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
    index = 0;
    while( ident[index] != '\0' ) {
        fputc( ident[index], cs );
        index++;
    }
    free( ident );
    free( p );
    //fprintf(stderr,":}");
}


void compile_promo( promo p, FILE *cs )
{
  list conds,conds_tmp, preconds;
    param params,params_tmp;
    int condsize, precondsize; //# de conditions
    int paramsize; //# de parameters
    conds = 0;
    preconds = 0;
    params = 0;

    compile_benef( p->benefits, cs );

    conds = p->conditions;
    condsize = 0;
    while( conds ) {
        compile_expr( conds->head, cs );
        conds_tmp = conds;
        conds = conds->tail;
        free( conds_tmp );
        condsize++;
    }

    params = p->parameters;
    paramsize = 0;
    while( params ) {
        params_tmp = params->nextParams;
        compile_param( params, cs );
        params = params_tmp;
        paramsize++;
    }

    preconds = p->preconditions;
    precondsize = 0;
    while( preconds ) {
        compile_expr( preconds->head, cs );
        conds_tmp = preconds;
        preconds = preconds->tail;
        free( conds_tmp );
        precondsize++;
    }

    fputc( MkPROMO, cs );
    fputint( precondsize, cs );
    fputint( paramsize, cs );
    fputint( condsize, cs ); 
    //fputc( precondsize, cs ); //cuidado!!!! estoy suponiendo no mas de 255 elementos en la lista!!!!!
    //fputc( paramsize, cs ); //cuidado!!!! estoy suponiendo no mas de 255 elementos en la lista!!!!!
    //fputc( condsize, cs ); //cuidado!!!! estoy suponiendo no mas de 255 elementos en la lista!!!!!
    fputc( p->evalTime, cs );
    {
        char *ident;
        int index;
        ident = p->name;
        index = strlen( ident );
        fputc( index, cs ); // CUIDADO! ESTOY SUPONIENDO index <= 255!!!!!!!
        index = 0;
        while( ident[index] != '\0' ) {
            fputc( ident[index], cs );
            index++;
        }
        free( ident );
    }

    if( p->cancellation != NULL ) {
        compile_promo( p->cancellation, cs );
        fputc( MkCANCELLATION, cs );
    }

    free( p );

    //fprintf(stderr,":]");
}


void compile_prog( promo prog, FILE *cs )
{
    int progsize = 0;
    promo prog_tmp;

    //fprintf(stderr,"<:");

    while( prog ) {
        prog_tmp = prog->nextPromo;
        compile_promo( prog, cs );
        prog = prog_tmp;
        progsize++;
    }
    fputc( MkPROG, cs );
    fputint( progsize, cs );
    //fputc( progsize, cs );

    //fprintf(stderr,":>");
}







/*
**********************************************************************
**********************************************************************
**********************************************************************
*/




promo build_prog( FILE *cs )
{
    expr e1;
    benef b1;
    param pa1,pa2;
    list glob1,glob2,ext1,ext2,stat1,stat2;
    int set_size;
    list ls1,ls2,ls3;
    mlist mls1,mls2;
    part ps1,ps2;
    int str_len,index;
    int paramsize,condsize,precondsize;
    promo pro1,pro2;
    int progsize;


    double doubleing;
    itime timeval;
    idate dateval;
    expr_stack exprStack = NULL;
    benef_stack benefStack = NULL;
    param_stack paramStack = NULL;
    promo_stack promoStack = NULL;
    code inst;


    inst = ( code )( unsigned char* )fgetc( cs );
    while( inst != MkPROG ) {
        switch( inst ) {
                /* CASOS DE CONSTRUCCION DE EXPRESIONES */
            case MkNUM:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = NUM;
                for( index = 0;index < sizeof( double );index++ ) {
                    ( ( unsigned char* )( &doubleing ) )[index] = ( unsigned char )fgetc( cs );
                }
                e1->e.num = doubleing;
                expr_stack_push( e1, &exprStack );
                break;
            case MkSUM:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = SUM;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkSUB:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = SUB;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkMULT:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = MULT;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;

            case MkFDIV:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = FDIV;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkMOD:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = MOD;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkDIV:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = DIV;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkMAX:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = MAX;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;

            case MkMIN:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = MIN;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkLEQ:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = LEQ;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkLESS:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = LESS;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkEQUAL:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = EQUAL;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkGREATER:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = GREATER;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkGEQ:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = GEQ;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkNEQ:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = NEQ;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkCOND:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = COND;
                e1->e.cond = ( econd )malloc( sizeof( struct econd ) );
                e1->e.cond->e0 = expr_stack_pop( &exprStack );
                e1->e.cond->e1 = expr_stack_pop( &exprStack );
                e1->e.cond->e2 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkAND:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = AND;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;

            case MkOR:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = OR;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;

            case MkLOG:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = LOG;

                /* target */
                str_len = fgetc( cs );
                e1->e.log.target = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.log.target[index] = fgetc( cs );
                }
                e1->e.log.target[str_len] = '\0';

                e1->e.log.e0 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;


            case MkUNION:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = UNION;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkINTERSECTION:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = INTERSECTION;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkIN:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = IN;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkMEET:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = MEET;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;

            case MkCONCAT:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = CONCAT;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;

            case MkROUND:
                e1 = ( expr )malloc( SIZE_OF_EXPR_UNOP );
                e1->refs = 1;
                e1->tag = _ROUND;
                e1->e.unop = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkTRUNC:
                e1 = ( expr )malloc( SIZE_OF_EXPR_UNOP );
                e1->refs = 1;
                e1->tag = TRUNC;
                e1->e.unop = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkAMOUNT:
                e1 = ( expr )malloc( SIZE_OF_EXPR_UNOP );
                e1->refs = 1;
                e1->tag = AMOUNT;
                e1->e.unop = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkCOMPLEMENT:
                e1 = ( expr )malloc( SIZE_OF_EXPR_UNOP );
                e1->refs = 1;
                e1->tag = COMPLEMENT;
                e1->e.unop = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkSIZE:
                e1 = ( expr )malloc( SIZE_OF_EXPR_UNOP );
                e1->refs = 1;
                e1->tag = SIZE;
                e1->e.unop = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkLENGTH:
                e1 = ( expr )malloc( SIZE_OF_EXPR_UNOP );
                e1->refs = 1;
                e1->tag = LENGTH;
                e1->e.unop = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkCOUNTABLE:
                e1 = ( expr )malloc( SIZE_OF_EXPR_UNOP );
                e1->refs = 1;
                e1->tag = COUNTABLE;
                e1->e.unop = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkNUM_RNV:
                str_len = fgetc( cs );
                e1 = ( expr )malloc( SIZE_OF_EXPR_IDENT );
                e1->refs = 1;
                e1->tag = NUM_RNV;
                e1->e.name = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.name[index] = fgetc( cs );
                }
                e1->e.name[str_len] = '\0';
                expr_stack_push( e1, &exprStack );
                break;
            case MkBOOL_RNV:
                str_len = fgetc( cs );
                e1 = ( expr )malloc( SIZE_OF_EXPR_IDENT );
                e1->refs = 1;
                e1->tag = BOOL_RNV;
                e1->e.name = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.name[index] = fgetc( cs );
                }
                e1->e.name[str_len] = '\0';
                expr_stack_push( e1, &exprStack );
                break;
            case MkSTRING_RNV:
                str_len = fgetc( cs );
                e1 = ( expr )malloc( SIZE_OF_EXPR_IDENT );
                e1->refs = 1;
                e1->tag = STRING_RNV;
                e1->e.name = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.name[index] = fgetc( cs );
                }
                e1->e.name[str_len] = '\0';
                expr_stack_push( e1, &exprStack );
                break;
            case MkDATE_RNV:
                str_len = fgetc( cs );
                e1 = ( expr )malloc( SIZE_OF_EXPR_IDENT );
                e1->refs = 1;
                e1->tag = DATE_RNV;
                e1->e.name = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.name[index] = fgetc( cs );
                }
                e1->e.name[str_len] = '\0';
                expr_stack_push( e1, &exprStack );
                break;
            case MkTIME_RNV:
                str_len = fgetc( cs );
                e1 = ( expr )malloc( SIZE_OF_EXPR_IDENT );
                e1->refs = 1;
                e1->tag = TIME_RNV;
                e1->e.name = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.name[index] = fgetc( cs );
                }
                e1->e.name[str_len] = '\0';
                expr_stack_push( e1, &exprStack );
                break;

            case MkDOT:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = DOT;
                e1->e.binop.e0 = expr_stack_pop( &exprStack );
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;

            case MkINTERVAL:
                e1 = ( expr )malloc( sizeof( struct expr ) );
                e1->refs = 1;
                e1->tag = INTERVAL;
                e1->e.interval.e0 = expr_stack_pop( &exprStack );
                e1->e.interval.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;


            case MkSET:
                set_size = fgetint( cs );
                ls1 = NULL;
                while( set_size ) {
                    ls2 = ( list )malloc( sizeof( struct node ) );
                    ls2->refs = 1;
                    ls2->head = expr_stack_pop( &exprStack );
                    ls2->tail = ls1;
                    ls1 = ls2;
                    set_size--;
                }
                e1 = ( expr )malloc( SIZE_OF_EXPR_SET );
                e1->refs = 1;
                e1->tag = SET;
                e1->e.set = ls1;
                expr_stack_push( e1, &exprStack );
                break;

            case MkAMSET:
                set_size = fgetint( cs );
                mls1 = NULL;
                while( set_size ) {
                    mls2 = ( mlist )malloc( sizeof( struct mnode ) );
                    mls2->refs = 1;
                    mls2->article = expr_stack_pop( &exprStack );
                    mls2->freq = expr_stack_pop( &exprStack );
                    mls2->tail = mls1;
                    mls1 = mls2;
                    set_size--;
                }
                e1 = ( expr )malloc( SIZE_OF_EXPR_AMSET );
                e1->refs = 1;
                e1->tag = AMSET;
                e1->e.amset = mls1;
                expr_stack_push( e1, &exprStack );
                break;

            case MkPART:
                set_size = fgetint( cs );
                ps1 = NULL;
                while( set_size ) {
                    ps2 = ( part )malloc( sizeof( struct pnode ) );
                    ps2->refs = 1;
                    ps2->rep = expr_stack_pop( &exprStack );
                    ps2->clase = expr_stack_pop( &exprStack );
                    ps2->tail = ps1;
                    ps1 = ps2;
                    set_size--;
                }
                e1 = ( expr )malloc( SIZE_OF_EXPR_PART );
                e1->refs = 1;
                e1->tag = PART;
                e1->e.part = ps1;
                expr_stack_push( e1, &exprStack );
                break;

            case MkSTRING:
                str_len = fgetc( cs );
                e1 = ( expr )malloc( SIZE_OF_EXPR_IDENT );
                e1->refs = 1;
                e1->tag = STRING;
                e1->e.name = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.name[index] = fgetc( cs );
                }
                e1->e.name[str_len] = '\0';
                expr_stack_push( e1, &exprStack );
                break;
            case MkIDENT:
                str_len = fgetc( cs );
                e1 = ( expr )malloc( SIZE_OF_EXPR_IDENT );
                e1->refs = 1;
                e1->tag = IDENT;
                e1->e.name = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.name[index] = fgetc( cs );
                }
                e1->e.name[str_len] = '\0';
                expr_stack_push( e1, &exprStack );
                break;
            case MkDEPARTMENT:
                str_len = fgetc( cs );
                e1 = ( expr )malloc( SIZE_OF_EXPR_IDENT );
                e1->refs = 1;
                e1->tag = DEPARTMENT;
                e1->e.name = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.name[index] = fgetc( cs );
                }
                e1->e.name[str_len] = '\0';
                expr_stack_push( e1, &exprStack );
                break;
            case MkCLASS:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CLASS );
                e1->refs = 1;
                e1->tag = CLASS;
                str_len = fgetc( cs );
                e1->e._class.nivel = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e._class.nivel[index] = fgetc( cs );
                }
                e1->e._class.nivel[str_len] = '\0';
                str_len = fgetc( cs );
                e1->e._class.concept = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e._class.concept[index] = fgetc( cs );
                }
                e1->e._class.concept[str_len] = '\0';
                expr_stack_push( e1, &exprStack );
                break;
            case MkPOINT:
                str_len = fgetc( cs );
                e1 = ( expr )malloc( SIZE_OF_EXPR_IDENT );
                e1->refs = 1;
                e1->tag = POINT;
                e1->e.name = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.name[index] = fgetc( cs );
                }
                e1->e.name[str_len] = '\0';
                expr_stack_push( e1, &exprStack );
                break;
            case MkBONUS:
                e1 = ( expr )malloc( SIZE_OF_EXPR_BONUS );
                e1->refs = 1;
                e1->tag = BONUS;
                e1->e.bonus = ( struct bonus * )malloc( sizeof( struct bonus ) );
                /* target */
                str_len = fgetc( cs );
                e1->e.bonus->target = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.bonus->target[index] = fgetc( cs );
                }
                e1->e.bonus->target[str_len] = '\0';
                /* type */
                str_len = fgetc( cs );
                e1->e.bonus->type = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.bonus->type[index] = fgetc( cs );
                }
                e1->e.bonus->type[str_len] = '\0';
                /* leyenda */
                str_len = fgetc( cs );
                e1->e.bonus->leyenda = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.bonus->leyenda[index] = fgetc( cs );
                }
                e1->e.bonus->leyenda[str_len] = '\0';
                /* cantidad de dias de validez */
                e1->e.bonus->validity = expr_stack_pop( &exprStack );
                /* fecha de vencimiento */
                e1->e.bonus->maturity = expr_stack_pop( &exprStack );
                /* valor */
                e1->e.bonus->e0 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkARTICLE:
                str_len = fgetc( cs );
                e1 = ( expr )malloc( SIZE_OF_EXPR_IDENT );
                e1->refs = 1;
                e1->tag = ARTICLE;
                e1->e.name = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.name[index] = fgetc( cs );
                }
                e1->e.name[str_len] = '\0';
                expr_stack_push( e1, &exprStack );
                break;
            case MkCARD:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CARD );
                e1->refs = 1;
                e1->tag = CARD;
                str_len = fgetc( cs );
                e1->e.card.name = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.card.name[index] = fgetc( cs );
                }
                e1->e.card.name[str_len] = '\0';
                str_len = fgetc( cs );
                e1->e.card.plan = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.card.plan[index] = fgetc( cs );
                }
                e1->e.card.plan[str_len] = '\0';
                expr_stack_push( e1, &exprStack );
                break;
            case MkMUTUAL:
                str_len = fgetc( cs );
                e1 = ( expr )malloc( SIZE_OF_EXPR_IDENT );
                e1->refs = 1;
                e1->tag = MUTUAL;
                e1->e.name = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.name[index] = fgetc( cs );
                }
                e1->e.name[str_len] = '\0';
                expr_stack_push( e1, &exprStack );
                break;
            case MkTICKET:
                str_len = fgetc( cs );
                e1 = ( expr )malloc( SIZE_OF_EXPR_IDENT );
                e1->refs = 1;
                e1->tag = TICKET;
                e1->e.name = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.name[index] = fgetc( cs );
                }
                e1->e.name[str_len] = '\0';
                expr_stack_push( e1, &exprStack );
                break;
            case MkCHEQUE:
                str_len = fgetc( cs );
                e1 = ( expr )malloc( SIZE_OF_EXPR_IDENT );
                e1->refs = 1;
                e1->tag = CHEQUE;
                e1->e.name = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.name[index] = fgetc( cs );
                }
                e1->e.name[str_len] = '\0';
                expr_stack_push( e1, &exprStack );
                break;
            case MkCURRENCY:
                str_len = fgetc( cs );
                e1 = ( expr )malloc( SIZE_OF_EXPR_IDENT );
                e1->refs = 1;
                e1->tag = CURRENCY;
                e1->e.name = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    e1->e.name[index] = fgetc( cs );
                }
                e1->e.name[str_len] = '\0';
                expr_stack_push( e1, &exprStack );
                break;

            case MkTRUE:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = TRUE;
                expr_stack_push( e1, &exprStack );
                break;
            case MkFALSE:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = FALSE;
                expr_stack_push( e1, &exprStack );
                break;
            case MkINPUT:
                e1 = ( expr )malloc( SIZE_OF_EXPR_UNOP );
                e1->refs = 1;
                e1->tag = INPUT;
                e1->e.unop = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkNUMINPUT:
                e1 = ( expr )malloc( SIZE_OF_EXPR_UNOP );
                e1->refs = 1;
                e1->tag = NUMINPUT;
                e1->e.unop = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;
            case MkHOLIDAY:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = HOLIDAY;
                expr_stack_push( e1, &exprStack );
                break;
            case MkAFFILIATED:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = AFFILIATED;
                expr_stack_push( e1, &exprStack );
                break;
            case MkALL:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = ALL;
                expr_stack_push( e1, &exprStack );
                break;
                /*
                case MkPURCHASE:
                e1 = (expr)malloc(SIZE_OF_EXPR_CTE);
                e1->refs = 1;
                e1->tag = PURCHASE;
                expr_stack_push(e1, &exprStack);
                break;
                case MkWPURCHASE:
                e1 = (expr)malloc(SIZE_OF_EXPR_CTE);
                e1->refs = 1;
                e1->tag = WPURCHASE;
                expr_stack_push(e1, &exprStack);
                break;
                 */
            case MkBUYED:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = BUYED;
                expr_stack_push( e1, &exprStack );
                break;
            case MkDELIVERED:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = DELIVERED;
                expr_stack_push( e1, &exprStack );
                break;
            case MkDISCOUNTS:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = DISCOUNTS;
                expr_stack_push( e1, &exprStack );
                break;
            case MkADDED:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = ADDED;
                expr_stack_push( e1, &exprStack );
                break;
            case MkDATEV:
                e1 = ( expr )malloc( SIZE_OF_EXPR_DATEV );
                e1->tag = DATEV;
                e1->refs = 1;
                for( index = 0;index < sizeof( idate );index++ ) {
                    ( ( unsigned char* )( &dateval ) )[index] = ( unsigned char )fgetc( cs );
                }
                e1->e.date = dateval;
                expr_stack_push( e1, &exprStack );
                break;
            case MkTIMEV:
                e1 = ( expr )malloc( SIZE_OF_EXPR_TIMEV );
                e1->refs = 1;
                e1->tag = TIMEV;
                for( index = 0;index < sizeof( itime );index++ ) {
                    ( ( unsigned char* )( &timeval ) )[index] = ( unsigned char )fgetc( cs );
                }
                e1->e.time = timeval;
                expr_stack_push( e1, &exprStack );
                break;
            case MkTIME:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = TIME;
                expr_stack_push( e1, &exprStack );
                break;
            case MkDATE:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = DATE;
                expr_stack_push( e1, &exprStack );
                break;
            case MkDATEOFBIRTH:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = DATEOFBIRTH;
                expr_stack_push( e1, &exprStack );
                break;
            case MkDAY:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = _DAY;
                expr_stack_push( e1, &exprStack );
                break;
            case MkCASH:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = CASH;
                expr_stack_push( e1, &exprStack );
                break;
            case MkCTACTE:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = CTACTE;
                expr_stack_push( e1, &exprStack );
                break;
            case MkMEANS:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = MEANS;
                expr_stack_push( e1, &exprStack );
                break;
            case MkCHANGE:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = CHANGE;
                expr_stack_push( e1, &exprStack );
                break;
            case MkRETURNED:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = RETURNED;
                expr_stack_push( e1, &exprStack );
                break;
            case MkPOINTS:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = POINTS;
                expr_stack_push( e1, &exprStack );
                break;
            case MkEBONUS:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = EBONUS;
                expr_stack_push( e1, &exprStack );
                break;
            case MkCASHDESK:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = CASHDESK;
                expr_stack_push( e1, &exprStack );
                break;
            case MkDAYSELECTOR:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = DAYSELECTOR;
                expr_stack_push( e1, &exprStack );
                break;
            case MkMONTHSELECTOR:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = MONTHSELECTOR;
                expr_stack_push( e1, &exprStack );
                break;
            case MkYEARSELECTOR:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = YEARSELECTOR;
                expr_stack_push( e1, &exprStack );
                break;
            case MkMONDAY:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = MONDAY;
                expr_stack_push( e1, &exprStack );
                break;
            case MkTUESDAY:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = TUESDAY;
                expr_stack_push( e1, &exprStack );
                break;
            case MkWEDNESDAY:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = WEDNESDAY;
                expr_stack_push( e1, &exprStack );
                break;
            case MkTHURSDAY:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = THURSDAY;
                expr_stack_push( e1, &exprStack );
                break;
            case MkFRIDAY:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = FRIDAY;
                expr_stack_push( e1, &exprStack );
                break;
            case MkSATURDAY:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = SATURDAY;
                expr_stack_push( e1, &exprStack );
                break;
            case MkSUNDAY:
                e1 = ( expr )malloc( SIZE_OF_EXPR_CTE );
                e1->refs = 1;
                e1->tag = SUNDAY;
                expr_stack_push( e1, &exprStack );
                break;



            case MkREQ_ANY:
                set_size = fgetint( cs );
                mls1 = NULL;
                while( set_size ) {
                    mls2 = ( mlist )malloc( sizeof( struct mnode ) );
                    mls2->refs = 1;
                    {
                        expr e3 = expr_stack_pop( &exprStack );
                        //mls2->article = expr_stack_pop(&exprStack);
                        mls2->article = ( expr )article_count_ref( atol( &( e3->e.name )[1] ) ); // NOTAR QUE ESTO ES UN DESASTRE! MUCHO CUIDADO AL DESALOCAR LA MEMORIA y AL ACCEDER. Ahora es un puntero a un LONG.
                        unref_e( e3 );
                    }
                    mls2->freq = expr_stack_pop( &exprStack );
                    mls2->tail = mls1;
                    mls1 = mls2;
                    set_size--;
                }
                e1 = ( expr )malloc( SIZE_OF_EXPR_AMSET );
                e1->refs = 1;
                e1->tag = REQ_ANY;
                e1->e.amset = mls1;
                expr_stack_push( e1, &exprStack );
                break;

            case MkREQ_ALL:
                set_size = fgetint( cs );
                mls1 = NULL;
                while( set_size ) {
                    mls2 = ( mlist )malloc( sizeof( struct mnode ) );
                    mls2->refs = 1;
                    {
                        expr e3 = expr_stack_pop( &exprStack );
                        //mls2->article = expr_stack_pop(&exprStack);
                        mls2->article = ( expr )article_count_ref( atol( &( e3->e.name )[1] ) ); // NOTAR QUE ESTO ES UN DESASTRE! MUCHO CUIDADO AL DESALOCAR LA MEMORIA y AL ACCEDER. Ahora es un puntero a un LONG.
                        unref_e( e3 );
                    }
                    mls2->freq = expr_stack_pop( &exprStack );
                    mls2->tail = mls1;
                    mls1 = mls2;
                    set_size--;
                }
                e1 = ( expr )malloc( SIZE_OF_EXPR_AMSET );
                e1->refs = 1;
                e1->tag = REQ_ALL;
                e1->e.amset = mls1;
                expr_stack_push( e1, &exprStack );
                break;

            case MkREQ_DEP:
                e1 = ( expr )malloc( SIZE_OF_EXPR_AMSET );
                e1->refs = 1;
                e1->tag = REQ_DEP;
                set_size = fgetint( cs );
                mls1 = NULL;
                while( set_size ) {
                    mls2 = ( mlist )malloc( sizeof( struct mnode ) );
                    mls2->refs = 1;
                    {
                    		// Algo de esta pinta!!!
                        expr e3 = expr_stack_pop( &exprStack );
                        mls2->article = ( expr )department_count_ref( atol( &( e3->e.name )[1] ) ); // NOTAR QUE ESTO ES UN DESASTRE! MUCHO CUIDADO AL DESALOCAR LA MEMORIA y AL ACCEDER. Ahora es un puntero a un LONG.
                        unref_e( e3 );
                        //----------------------
                        //mls2->article = expr_stack_pop(&exprStack);
                    }
                    mls2->freq = expr_stack_pop( &exprStack );
                    mls2->tail = mls1;
                    mls1 = mls2;
                    set_size--;
                }
                e1->e.binop.e0 = (expr) mls1;
                e1->e.binop.e1 = expr_stack_pop( &exprStack );
                expr_stack_push( e1, &exprStack );
                break;


                /* CASOS DE CONSTRUCCION DE BENEFICIOS */
            case MkASSIGN:
                str_len = fgetc( cs );
                b1 = ( benef )malloc( SIZE_OF_ASSIGN );
                b1->refs = 1;
                b1->tag = ASSIGN;
                b1->b.assign.var = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    b1->b.assign.var[index] = fgetc( cs );
                }
                b1->b.assign.var[str_len] = '\0';
                b1->b.assign.e = expr_stack_pop( &exprStack );
                benef_stack_push( b1, &benefStack );
                break;
            case MkIF:
                b1 = ( benef )malloc( SIZE_OF_COND );
                b1->refs = 1;
                b1->tag = IF;
                b1->b.cond.b = expr_stack_pop( &exprStack );
                b1->b.cond.c1 = benef_stack_pop( &benefStack );
                b1->b.cond.c2 = benef_stack_pop( &benefStack );
                benef_stack_push( b1, &benefStack );
                break;

                //////////////////////////////////
            case MkFOR:
                b1 = ( benef )malloc( SIZE_OF_FOR );
                b1->refs = 1;
                b1->tag = FOR;
                str_len = fgetc( cs );
                b1->b.foreach.k = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    b1->b.foreach.k[index] = fgetc( cs );
                }
                b1->b.foreach.k[str_len] = '\0';
                str_len = fgetc( cs );
                b1->b.foreach.a = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    b1->b.foreach.a[index] = fgetc( cs );
                }
                b1->b.foreach.a[str_len] = '\0';
                b1->b.foreach.e = expr_stack_pop( &exprStack );
                b1->b.foreach.c = benef_stack_pop( &benefStack );
                benef_stack_push( b1, &benefStack );
                break;
            case MkPRINT:
                b1 = ( benef )malloc( SIZE_OF_PRINT );
                b1->refs = 1;
                b1->tag = BPRINT;
                b1->b.print = expr_stack_pop( &exprStack );
                benef_stack_push( b1, &benefStack );
                break;
            case MkCREDIT:
                b1 = ( benef )malloc( SIZE_OF_CREDIT );
                b1->refs = 1;
                b1->tag = CREDIT;
                b1->b.credit.e1 = expr_stack_pop( &exprStack );
                b1->b.credit.e2 = expr_stack_pop( &exprStack );
                b1->b.credit.e3 = expr_stack_pop( &exprStack );
                b1->b.credit.e4 = expr_stack_pop( &exprStack );
                benef_stack_push( b1, &benefStack );
                break;
            case MkDISTRIBUTE:
                b1 = ( benef )malloc( SIZE_OF_DISTRIBUTE );
                b1->refs = 1;
                b1->tag = DISTRIBUTE;
                b1->b.distribute.e1 = expr_stack_pop( &exprStack );
                b1->b.distribute.e2 = expr_stack_pop( &exprStack );
                b1->b.distribute.e3 = expr_stack_pop( &exprStack );
                benef_stack_push( b1, &benefStack );
                break;
            case MkISSUE_BONUS:
                b1 = ( benef )malloc( SIZE_OF_ISSUEBONUS );
                b1->refs = 1;
                b1->tag = ISSUE_BONUS;
                b1->b.issueBonus.e1 = expr_stack_pop( &exprStack );
                b1->b.issueBonus.e2 = expr_stack_pop( &exprStack );
                benef_stack_push( b1, &benefStack );
                break;
            case MkGIVE_POINTS:
                b1 = ( benef )malloc( SIZE_OF_GIVEPOINTS );
                b1->refs = 1;
                b1->tag = GIVE_POINTS;
                b1->b.givePoints.e1 = expr_stack_pop( &exprStack );
                b1->b.givePoints.e2 = expr_stack_pop( &exprStack );
                benef_stack_push( b1, &benefStack );
                break;
            case MkACTIVATE:
                b1 = ( benef )malloc( SIZE_OF_ACTIVATE );
                b1->refs = 1;
                b1->tag = ACTIVATE;
                b1->b.activate.e1 = expr_stack_pop( &exprStack );
                b1->b.activate.e2 = expr_stack_pop( &exprStack );
                benef_stack_push( b1, &benefStack );
                break;
            case MkEPROC:
                str_len = fgetc( cs );
                b1 = ( benef )malloc( SIZE_OF_EPROC );
                b1->refs = 1;
                b1->tag = EPROC;
                b1->b.eproc.procName = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    b1->b.eproc.procName[index] = fgetc( cs );
                }
                b1->b.eproc.procName[str_len] = '\0';
                b1->b.eproc.params = expr_stack_pop( &exprStack );
                benef_stack_push( b1, &benefStack );
                break;
            case MkREQ_MEANS:
                b1 = ( benef )malloc( SIZE_OF_REQ_MEANS );
                b1->refs = 1;
                b1->tag = REQ_MEANS;
                b1->b.reqMeans.e1 = expr_stack_pop( &exprStack );
                b1->b.reqMeans.e2 = NULL;
                benef_stack_push( b1, &benefStack );
                break;
            case MkREQ_MEANS_CANT:
                b1 = ( benef )malloc( SIZE_OF_REQ_MEANS );
                b1->refs = 1;
                b1->tag = REQ_MEANS;
                b1->b.reqMeans.e1 = expr_stack_pop( &exprStack );
                b1->b.reqMeans.e2 = expr_stack_pop( &exprStack );
                benef_stack_push( b1, &benefStack );
                break;
            case MkREC:
                b1 = ( benef )malloc( SIZE_OF_REC );
                b1->refs = 1;
                b1->tag = REC;
                b1->b.rec.type = fgetc( cs );
                switch( b1->b.rec.type ) {
                    case REC_PROMOID_OLD:
                    case REC_ARTS:
                    case REC_DEP:
                    case REC_MEAN:
                    case REC_CLASS:
                    case REC_PERFIL:
                    case REC_CREDIT:
                        b1->b.rec.e1 = expr_stack_pop( &exprStack );
                        b1->b.rec.e2 = NULL;
                        break;
                    case REC_BONUS:
                    case REC_POINTS:
                    case REC_ACTIVATE_MEAN:
                    case REC_STRING:
                        b1->b.rec.e1 = expr_stack_pop( &exprStack );
                        b1->b.rec.e2 = expr_stack_pop( &exprStack );
                        break;
                    case REC_PROMOID:
                        b1->b.rec.e2 = expr_stack_pop( &exprStack );/*ANTES ESTABAN AL REVES*/
                        b1->b.rec.e1 = expr_stack_pop( &exprStack );/*FR 12-07-07*/
                        break;
                    case REC_ALL:
                        b1->b.rec.e1 = NULL;
                        b1->b.rec.e2 = NULL;
                        break;
                    default:
                        break;
                }
                benef_stack_push( b1, &benefStack );
                break;
            case MkSKIP:
                b1 = ( benef )malloc( SIZE_OF_SKIP );
                b1->refs = 1;
                b1->tag = __SKIP;
                benef_stack_push( b1, &benefStack );
                break;
            case MkCOMP:
                b1 = ( benef )malloc( SIZE_OF_COMP );
                b1->refs = 1;
                b1->tag = COMP;
                b1->b.comp.c1 = benef_stack_pop( &benefStack );
                b1->b.comp.c2 = benef_stack_pop( &benefStack );
                benef_stack_push( b1, &benefStack );
                break;

            case MkNUM_RNV_ASSIGN:
                str_len = fgetc( cs );
                b1 = ( benef )malloc( SIZE_OF_ASSIGN );
                b1->refs = 1;
                b1->tag = NUM_RNV_ASSIGN;
                b1->b.assign.var = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    b1->b.assign.var[index] = fgetc( cs );
                }
                b1->b.assign.var[str_len] = '\0';
                b1->b.assign.e = expr_stack_pop( &exprStack );
                benef_stack_push( b1, &benefStack );
                break;
            case MkBOOL_RNV_ASSIGN:
                str_len = fgetc( cs );
                b1 = ( benef )malloc( SIZE_OF_ASSIGN );
                b1->refs = 1;
                b1->tag = BOOL_RNV_ASSIGN;
                b1->b.assign.var = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    b1->b.assign.var[index] = fgetc( cs );
                }
                b1->b.assign.var[str_len] = '\0';
                b1->b.assign.e = expr_stack_pop( &exprStack );
                benef_stack_push( b1, &benefStack );
                break;
            case MkSTRING_RNV_ASSIGN:
                str_len = fgetc( cs );
                b1 = ( benef )malloc( SIZE_OF_ASSIGN );
                b1->refs = 1;
                b1->tag = STRING_RNV_ASSIGN;
                b1->b.assign.var = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    b1->b.assign.var[index] = fgetc( cs );
                }
                b1->b.assign.var[str_len] = '\0';
                b1->b.assign.e = expr_stack_pop( &exprStack );
                benef_stack_push( b1, &benefStack );
                break;
            case MkDATE_RNV_ASSIGN:
                str_len = fgetc( cs );
                b1 = ( benef )malloc( SIZE_OF_ASSIGN );
                b1->refs = 1;
                b1->tag = DATE_RNV_ASSIGN;
                b1->b.assign.var = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    b1->b.assign.var[index] = fgetc( cs );
                }
                b1->b.assign.var[str_len] = '\0';
                b1->b.assign.e = expr_stack_pop( &exprStack );
                benef_stack_push( b1, &benefStack );
                break;
            case MkTIME_RNV_ASSIGN:
                str_len = fgetc( cs );
                b1 = ( benef )malloc( SIZE_OF_ASSIGN );
                b1->refs = 1;
                b1->tag = TIME_RNV_ASSIGN;
                b1->b.assign.var = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    b1->b.assign.var[index] = fgetc( cs );
                }
                b1->b.assign.var[str_len] = '\0';
                b1->b.assign.e = expr_stack_pop( &exprStack );
                benef_stack_push( b1, &benefStack );
                break;


                /*  *******************************   */
            case MkPARAM:
                str_len = fgetc( cs );
                pa1 = ( param )malloc( sizeof( struct param ) );
                pa1->refs = 1;
                pa1->tag = LOCAL;
                pa1->var = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    pa1->var[index] = fgetc( cs );
                }
                pa1->var[str_len] = '\0';
                pa1->e = expr_stack_pop( &exprStack );
                pa1->nextParams = NULL;
                param_stack_push( pa1, &paramStack );
                break;
            case MkPARAMGLOBAL:
                str_len = fgetc( cs );
                pa1 = ( param )malloc( sizeof( struct param ) );
                pa1->refs = 1;
                pa1->tag = GLOBAL;
                pa1->var = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    pa1->var[index] = fgetc( cs );
                }
                pa1->var[str_len] = '\0';
                pa1->e = expr_stack_pop( &exprStack );
                pa1->nextParams = NULL;
                param_stack_push( pa1, &paramStack );
                break;
            case MkPARAMSTATIC:
                str_len = fgetc( cs );
                pa1 = ( param )malloc( sizeof( struct param ) );
                pa1->refs = 1;
                pa1->tag = STATIC;
                pa1->var = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    pa1->var[index] = fgetc( cs );
                }
                pa1->var[str_len] = '\0';
                pa1->e = expr_stack_pop( &exprStack );
                pa1->nextParams = NULL;
                param_stack_push( pa1, &paramStack );
                break;
            case MkPARAMEXTERN:
                str_len = fgetc( cs );
                pa1 = ( param )malloc( sizeof( struct param ) );
                pa1->refs = 1;
                pa1->tag = EXTERN;
                pa1->var = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    pa1->var[index] = fgetc( cs );
                }
                pa1->var[str_len] = '\0';
                pa1->e = NULL;
                pa1->nextParams = NULL;
                param_stack_push( pa1, &paramStack );
                break;
            case MkPROMO:
                precondsize = fgetint( cs );
                paramsize = fgetint( cs );
                condsize = fgetint( cs );
                //precondsize = fgetc( cs );
                //paramsize = fgetc( cs );
                //condsize = fgetc( cs );

                /* generacion de la lista de precondiciones */
                ls3 = NULL;
                while( precondsize-- ) {
                    ls2 = ( list )malloc( sizeof( struct node ) );
                    ls2->refs = 1;
                    ls2->head = expr_stack_pop( &exprStack );
                    ls2->tail = ls3;
                    ls3 = ls2;
                }

                /* generacion de la lista de declaraciones de parametros */
                pa1 = NULL;
                glob1 = NULL;
                stat1 = NULL;
                ext1 = NULL;
                while( paramsize-- ) {
                    pa2 = param_stack_pop( &paramStack );
                    if( pa2->tag == LOCAL ) {
                        pa2->nextParams = pa1;
                        pa1 = pa2;
                    }
                    else if( pa2->tag == GLOBAL ) {
                        pa2->nextParams = pa1;    
                        pa1 = pa2;

                        // fprintf(stderr,"Una var global: %s.\n",pa2->var);

                        e1 = ( expr )malloc( sizeof( struct expr ) );
                        e1->refs = 1;
                        e1->tag = IDENT;
                        {
                            char *ident;
                            int index = strlen( pa2->var );
                            ident = ( char* )malloc( index + 1 );
                            strcpy( ident, pa2->var );
                            ident[index] = '\0';
                            e1->e.name = ident;
                        }

                        if( ( glob2 = ( list )malloc( sizeof( struct node ) ) ) == NULL ) {
                            perror( "falta de memoria" );
                            exit( 1 );
                        }
                        // fprintf(stderr,"La misma var: %s.\n",e1->e.name);
                        glob2->refs = 1;
                        glob2->head = e1;
                        glob2->tail = glob1;
                        glob1 = glob2;
                        // fprintf(stderr,"La misma var: %s.\n",glob1->head->e.name);
                    }
                    else if( pa2->tag == STATIC ) {
                        pa2->nextParams = pa1;    
                        pa1 = pa2;

                        e1 = ( expr )malloc( sizeof( struct expr ) );
                        e1->refs = 1;
                        e1->tag = IDENT;
                        {
                            char *ident;
                            int index = strlen( pa2->var );
                            ident = ( char* )malloc( index + 1 );
                            strcpy( ident, pa2->var );
                            ident[index] = '\0';
                            e1->e.name = ident;
                        }

                        if( ( stat2 = ( list )malloc( sizeof( struct node ) ) ) == NULL ) {
                            perror( "falta de memoria" );
                            exit( 1 );
                        }
                        stat2->refs = 1;
                        stat2->head = e1;
                        stat2->tail = stat1;
                        stat1 = stat2;
                    }
                    else if( pa2->tag == EXTERN ) {
                        //e1 = (expr)malloc(SIZE_OF_EXPR_IDENT);
                        e1 = ( expr )malloc( sizeof( struct expr ) );
                        e1->refs = 1;
                        e1->tag = IDENT;
                        {
                            char *ident;
                            int index = strlen( pa2->var );
                            ident = ( char* )malloc( index + 1 );
                            strcpy( ident, pa2->var );
                            ident[index] = '\0';
                            e1->e.name = ident;
                        }
                        ext2 = ext1;
                        ext1 = ( list )malloc( sizeof( struct node ) );
                        ext1->refs = 1;
                        ext1->head = e1;
                        ext1->tail = ext2;

                        unref_pa( pa2 );
                    }
                    else {
                        perror( "generacion de lista de parametros: caso no contemplado" );
                        exit( 1 );
                    }
                }

                /* generacion de la lista de condiciones */
                ls1 = NULL;
                while( condsize-- ) {
                    ls2 = ( list )malloc( sizeof( struct node ) );
                    ls2->refs = 1;
                    ls2->head = expr_stack_pop( &exprStack );
                    ls2->tail = ls1;
                    ls1 = ls2;
                }


                /* construccion. de la promo */
                pro1 = ( promo )malloc( sizeof( struct promo ) );
                pro1->refs = 1;

                pro1->globals = glob1;
                pro1->externs = ext1;
                pro1->statics = stat1;

                pro1->preconditions = ls3;
                pro1->parameters = pa1;
                pro1->conditions = ls1;
                pro1->benefits = benef_stack_pop( &benefStack );

                pro1->evalTime = (evalTime) fgetc( cs );

                /* nombre de la promo */
                str_len = fgetc( cs );
                pro1->name = ( identifier )malloc( str_len + 1 );
                for( index = 0;index < str_len;index++ ) {
                    pro1->name[index] = fgetc( cs );
                }
                pro1->name[str_len] = '\0';

                pro1->cancellation = NULL;
                pro1->nextPromo = NULL;
                promo_stack_push( pro1, &promoStack );

                break;

            case MkCANCELLATION:
                pro2 = promo_stack_pop( &promoStack );
                pro1 = promo_stack_pop( &promoStack );
                pro1->cancellation = pro2;
                promo_stack_push( pro1, &promoStack );
                break;

            default:
		      //fprintf(stderr,ST( S_CASO_NO_CONTEMPLADO_ ));
				glog(ST( S_CASO_NO_CONTEMPLADO_ ),LOG_MOTOR_PROMO,3);
				// Error irrecuperable. Se liberan las estructuras
				// y se devuelve la promocion NULL.
				glog("PROMO: ERROR: build_prog: Abortando carga de promocion. Liberando estructuras...",LOG_MOTOR_PROMO,3);
				while (exprStack) {
					expr e = ref_e (expr_stack_pop( &exprStack ));
					unref_e (e);
				}
				while (benefStack) {
					benef b = ref_b (benef_stack_pop( &benefStack ));
					unref_b (b);
				}
				while (paramStack) {
					param pa = ref_pa (param_stack_pop( &paramStack ));
					unref_pa (pa);
				}
				while (promoStack) {
					promo pr = ref_pro (promo_stack_pop( &promoStack ));
					unref_pr (pr);
				}
				return NULL;
        }

        inst = ( code )( unsigned char* )fgetc( cs );
    }



    /* CONSTRUCCION DEL PROGRAMA (LISTA DE PROGS)*/
    pro1 = NULL;
    progsize = fgetint( cs );
    //progsize = fgetc( cs );

    while( progsize-- ) {
        pro2 = promo_stack_pop( &promoStack );
        pro2->nextPromo = pro1;
        pro1 = pro2;
    }

    return pro1;
}





void to_disk( promo p, char *file, pc_version *ver )
{
    FILE *cs;
    #ifdef WIN32
    _fmode = O_BINARY;
    #endif
    #ifdef INVEL_W
    _fmode = O_BINARY;
    #endif
    #ifdef __MSDOS__
    _fmode = O_BINARY;
    #endif
    cs = fopen( file, "w" );
    if( !cs ) {
	    //fprintf(stderr,ST( S_TO_DISK__CANNOT_OPEN_FILE__S_ ),file);
	    glog(ST( S_TO_DISK__CANNOT_OPEN_FILE__S_ ),LOG_MOTOR_PROMO,3);
        exit( 1 );
    }

    fputint(ver->macro, cs);
    fputint(ver->version, cs);
    fputint(ver->subversion, cs);
    fputint(ver->build, cs);
    
    compile_prog( p, cs );

    if( fclose( cs ) != 0 ) {
	    //fprintf(stderr,ST( S_TO_DISK__CANNOT_CLOSE_FILE__S_ ),file);
	    glog(ST( S_TO_DISK__CANNOT_CLOSE_FILE__S_ ),LOG_MOTOR_PROMO,3);
        exit( 1 );
    }
}




promo from_disk( char *file )
{
    FILE *cs;
    promo pro;
    pc_version ver;
    #ifdef WIN32
    _fmode = O_BINARY;
    #endif
    #ifdef INVEL_W
    _fmode = O_BINARY;
    #endif
    #ifdef __MSDOS__
    _fmode = O_BINARY;
    #endif
    
    //if ( !compatible_version(file,ver) ) {
    //	fprintf( stderr, "El archivo %s se genero con una version incompatible del compilador de promociones.\n", file );
    //	return NULL;
    //}
    
    cs = fopen( file, "r" );
    if( !cs ) {
	    //fprintf(stderr,ST( S_FROM_DISK__CANNOT_OPEN_FILE__S_ ),file);
	    glog(ST( S_FROM_DISK__CANNOT_OPEN_FILE__S_ ),LOG_MOTOR_PROMO,3);
        return NULL;
    }

    ver.macro = fgetint( cs );
    ver.version = fgetint( cs );
    ver.subversion = fgetint( cs );
    ver.build = fgetint( cs );

    pro = build_prog( cs );

    if( fclose( cs ) != 0 ) {
	    //fprintf(stderr,ST( S_FROM_DISK__CANNOT_CLOSE_FILE__S_ ),file);
	    glog(ST( S_FROM_DISK__CANNOT_CLOSE_FILE__S_ ),LOG_MOTOR_PROMO,3);
		return NULL;
    }
    return pro;
}


int compatible_version( char *file, pc_version * comp_ver )
{
    FILE *cs;
    //promo pro;
    pc_version ver = {0,0,0,0};
    #ifdef WIN32
    _fmode = O_BINARY;
    #endif
    #ifdef INVEL_W
    _fmode = O_BINARY;
    #endif
    #ifdef __MSDOS__
    _fmode = O_BINARY;
    #endif
    cs = fopen( file, "r" );
    if( !cs ) {
        fprintf( stderr, ST( S_FROM_DISK__CANNOT_OPEN_FILE__S_ ), file );
        return 0;
    }

    ver.macro = fgetint( cs );
    ver.version = fgetint( cs );
    ver.subversion = fgetint( cs );
    ver.build = fgetint( cs );

		if ( (ver.macro != comp_ver->macro) || (ver.version != comp_ver->version) ) {
			return 0;
		}

    if( fclose( cs ) != 0 ) {
        fprintf( stderr, ST( S_FROM_DISK__CANNOT_CLOSE_FILE__S_ ), file );
				return 0;
    }
    return 1;
}




//#endif

