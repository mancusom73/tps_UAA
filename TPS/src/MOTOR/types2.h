#pragma pack (push,1) 
#ifndef _TYPES_
#define _TYPES_

#include <stdlib.h>
#include <stdio.h>
#include "time2.h"

#pragma pack (1)

#define nil NULL

#define SALIDA stdout


typedef enum { NUM,

               /* Operaciones Binarias Aritmeticas */
               SUM, SUB, MULT, FDIV, MOD, DIV, MAX, MIN,
               LEQ, LESS, EQUAL, GREATER, GEQ, NEQ,

	       /* Expresion Condicional */
               COND,

	       /* Operaciones Binarias Booleanas */
               AND, OR, LOG,

 	       /* Operaciones Binarias de Multiconjuntos */
               UNION, INTERSECTION, IN, MEET,

	       /* Operaciones Binarias de Strings*/
	       CONCAT,

	       /* Operaciones Unarias */
               AMOUNT, COMPLEMENT, SIZE, LENGTH,
	       NUM_RNV, BOOL_RNV, STRING_RNV, DATE_RNV, TIME_RNV,
	       COUNTABLE,
	       _ROUND, TRUNC, INPUT, NUMINPUT, 

	       /* */
	       DOT, INTERVAL,

	       /* Sets, AMSets*/
	       SET, AMSET, 

	       /* Particion de Articulos con Articulo representante */
	       PART, 

               /* Constructores IDENT */
               IDENT, DEPARTMENT, POINT, BONUS, ARTICLE, STRING, 
	       MUTUAL, TICKET, CHEQUE, CURRENCY,

	       /* Constructores dos IDENT */
	       CLASS, CARD,

	       /* Valor Fecha y Hora */
	       TIMEV, DATEV,

	       /* Valores Booleanos*/
	       TRUE, FALSE,

	       /* */
	       HOLIDAY, AFFILIATED, ALL, /*PURCHASE, WPURCHASE,*/ TIME, DATE,
	       BUYED, DELIVERED, DISCOUNTS, ADDED,
	       DATEOFBIRTH, _DAY, CASH, CTACTE, MEANS, CHANGE, RETURNED, POINTS, EBONUS, CASHDESK,
	       DAYSELECTOR, MONTHSELECTOR, YEARSELECTOR,

	       /* DAYS*/
	       MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY,

	       /* REQUERIMIENTOS EXCEPCIONALES */
	       REQ_ANY, REQ_ALL, REQ_DEP
               } tExpr;

typedef char * identifier;


typedef int tRefs; //el tipo del contador de referencias
typedef unsigned char tExpr_; //el tipo de el identificador de expresiones


typedef struct expr {
    tRefs refs;
    tExpr_ tag;
    union {

      /* Operaciones Binarias */
      struct {struct expr *e0; struct expr *e1;} binop;

      /* Operaciones Unarias */
      struct expr *unop;

      /* */
      struct {struct expr *e0; struct expr *e1;} frequency;

      /* LOG */
      struct {identifier target; struct expr *e0;} log;

      /* Conjunto de Expresiones */
      struct node {tRefs refs;
	           struct expr *head;
	           struct node * tail; } * set;

      /* Multiconjuntos Arithmeticos*/
      struct mnode {tRefs refs;
		    struct expr *article;
	            struct expr *freq;
	            struct mnode * tail;} * amset;

      /* Particion de articulos con articulo representate */
      struct pnode {tRefs refs;
		    struct expr *rep;
	            struct expr *clase;
	            struct pnode * tail;} * part;

      struct {struct expr *e0; struct expr *e1;} interval;

      /* bonus */
      struct bonus {
	identifier target; // CHIP,SHOW, PRINT_IN_TICKET, PRINT_OUTSIDE_TICKET
	identifier type;   //tipo/formado del bono/cupon
	struct expr *e0;   //valor del bono
	char* leyenda;
	struct expr *maturity;   //Fecha de vencimiento
	struct expr *validity;   //cantidad de días de validez
      } * bonus;

      /* expresion condicional */
      struct econd {
	struct expr *e0; // condicion
	struct expr *e1; // expresion para el caso  e0
	struct expr *e2; // expresion para el caso !e0
      } * cond;

      /* identificador */
      identifier name;

      /* dos identificadores */
      struct _class {
	identifier nivel;
	identifier concept;
      } _class;

      struct _card {
	identifier name;
	identifier plan;
      } card;

      /* expresion entera */
      double num;

      /* time */
      itime time;

      /* date */
      idate date;

    } e;

  } *expr;


//#define SIZE_OF_EXPR_CTE sizeof(tExpr_)+sizeof(tRefs)
//// al anterior no le falta +sizeof(expr) ??????????????????????????????????
//#define SIZE_OF_EXPR_UNOP SIZE_OF_EXPR_CTE+sizeof(expr)
//#define SIZE_OF_EXPR_IDENT SIZE_OF_EXPR_CTE+sizeof(identifier)
//#define SIZE_OF_EXPR_SET SIZE_OF_EXPR_CTE+sizeof(struct node*)
//#define SIZE_OF_EXPR_AMSET SIZE_OF_EXPR_CTE+sizeof(struct mnode*)
//#define SIZE_OF_EXPR_TIMEV SIZE_OF_EXPR_CTE+sizeof(itime)
//#define SIZE_OF_EXPR_DATEV SIZE_OF_EXPR_CTE+sizeof(idate)
//#define SIZE_OF_EXPR_BONUS SIZE_OF_EXPR_CTE+sizeof(struct bonus*)
//#define SIZE_OF_EXPR_CLASS SIZE_OF_EXPR_CTE+sizeof(struct _class)
//#define SIZE_OF_EXPR_CARD SIZE_OF_EXPR_CTE+sizeof(struct _card)



#define SIZE_OF_EXPR_CTE sizeof(struct expr)
// al anterior no le falta +sizeof(expr) ??????????????????????????????????
#define SIZE_OF_EXPR_UNOP sizeof(struct expr)
#define SIZE_OF_EXPR_IDENT sizeof(struct expr)
#define SIZE_OF_EXPR_SET sizeof(struct expr)
#define SIZE_OF_EXPR_AMSET sizeof(struct expr)
#define SIZE_OF_EXPR_PART sizeof(struct expr)
#define SIZE_OF_EXPR_TIMEV sizeof(struct expr)
#define SIZE_OF_EXPR_DATEV sizeof(struct expr)
#define SIZE_OF_EXPR_BONUS sizeof(struct expr)
#define SIZE_OF_EXPR_CLASS sizeof(struct expr)
#define SIZE_OF_EXPR_CARD sizeof(struct expr)
//#define SIZE_OF_BONUS sizeof(struct bonus)
//esto parece no optimizar mucho!!!!!!!!! que pasa? a donde se va todo?



/* Expr Set */
typedef struct node *list;


/* MSet Arithmetico*/
typedef struct mnode *mlist;

/* Particion de Articulos con Articulo Representante */
typedef struct pnode *part;

/* Expresion Condicional */
typedef struct econd * econd;


/**************************************************/

typedef enum { ASSIGN,
               IF,
               FOR,
               BPRINT,  // 27/01/05 cambiado por conflicto de nombres. antes era PRINT
               CREDIT,
               DISTRIBUTE,
               ISSUE_BONUS,
               GIVE_POINTS,
	       ACTIVATE,
	       EPROC, // External Procedure (llamada a procedimientos externos)
               __SKIP,
               COMP,
    	         NUM_RNV_ASSIGN,
    	         BOOL_RNV_ASSIGN,
    	         STRING_RNV_ASSIGN,
    	         DATE_RNV_ASSIGN,
	       TIME_RNV_ASSIGN ,

	       /* REGISTRO DE EVENTOS */
	       REC,
	       REC_PROMOID, REC_PROMOID_OLD,
	       REC_ARTS, REC_DEP, REC_MEAN, REC_CLASS, REC_ALL, REC_PERFIL,
	       REC_CREDIT, REC_BONUS, REC_POINTS, REC_ACTIVATE_MEAN,
	       REC_STRING,

	       /* REQUERIMIENTOS MEDIOS DE PAGO */
	       REQ_MEANS
} tBenef;


typedef unsigned char tBenef_;

/* Tipo Beneficios */
typedef struct benef {
    tRefs refs;
    tBenef_ tag;
    union {

      /* Operaciones Binarias */
      struct assign{identifier var; expr e;} assign;
      struct cond{expr b; struct benef *c1; struct benef *c2; } cond;
      struct foreach{identifier a,k; expr e; struct benef *c;} foreach;
      expr print;
      struct credit{expr e1; expr e2; expr e3; expr e4;} credit; // e1:descarga; e2:monto; e3:articulos_beneficiados; e4:cantidad_de_descuentos
      struct distribute{expr e1; expr e2; expr e3;} distribute;
      struct issueBonus{expr e1; expr e2;} issueBonus;
      struct givePoints{expr e1; expr e2;} givePoints;
      struct activate{expr e1; expr e2;} activate;
      struct eproc{identifier procName; expr params; } eproc;
      struct reqMeans{expr e1; expr e2;} reqMeans;
      struct comp{struct benef *c1; struct benef *c2; } comp;
      struct rec{char type; expr e1; expr e2;} rec;
    } b;
} * benef;


//#define SIZE_OF_SKIP sizeof(tRefs)+sizeof(tBenef_)
//#define SIZE_OF_ASSIGN sizeof(tRefs)+sizeof(tBenef_)+sizeof(struct assign)
//#define SIZE_OF_COND sizeof(tRefs)+sizeof(tBenef_)+sizeof(struct cond)
//#define SIZE_OF_FOR sizeof(tRefs)+sizeof(tBenef_)+sizeof(struct foreach)
//#define SIZE_OF_PRINT sizeof(tRefs)+sizeof(tBenef_)+sizeof(expr)
//#define SIZE_OF_CREDIT sizeof(tRefs)+sizeof(tBenef_)+sizeof(struct credit)
//#define SIZE_OF_DISTRIBUTE sizeof(tRefs)+sizeof(tBenef_)+sizeof(struct distribute)
//#define SIZE_OF_ISSUEBONUS sizeof(tRefs)+sizeof(tBenef_)+sizeof(struct issueBonus)
//#define SIZE_OF_GIVEPOINTS sizeof(tRefs)+sizeof(tBenef_)+sizeof(struct givePoints)
//#define SIZE_OF_COMP sizeof(tRefs)+sizeof(tBenef_)+sizeof(struct comp)

#define SIZE_OF_SKIP sizeof(struct benef)
#define SIZE_OF_ASSIGN sizeof(struct benef)
#define SIZE_OF_COND sizeof(struct benef)
#define SIZE_OF_FOR sizeof(struct benef)
#define SIZE_OF_PRINT sizeof(struct benef)
#define SIZE_OF_CREDIT sizeof(struct benef)
#define SIZE_OF_DISTRIBUTE sizeof(struct benef)
#define SIZE_OF_ISSUEBONUS sizeof(struct benef)
#define SIZE_OF_GIVEPOINTS sizeof(struct benef)
#define SIZE_OF_ACTIVATE sizeof(struct benef)
#define SIZE_OF_EPROC sizeof(struct benef)
#define SIZE_OF_REQ_MEANS sizeof(struct benef)
#define SIZE_OF_REC sizeof(struct benef)
#define SIZE_OF_COMP sizeof(struct benef)



/***************************************************/

typedef enum {LOCAL, GLOBAL, EXTERN, STATIC} aParam; //alcance del parametro

typedef struct param {
  tRefs refs;
  aParam tag;
  identifier var;
  expr e;
  struct param *nextParams;
} * param;


typedef enum {ONLINE, PREPAGO, POSTPAGO, CANCELLATION, EVER, INIT_PARAMS} evalTime;

typedef struct promo {
  tRefs refs;
  evalTime evalTime;
  identifier name;
  list globals; // variables globales
  list externs; // variables externas
  list statics; // variables estaticas
  list  preconditions;
  param parameters;
  list  conditions;
  benef benefits;
  struct promo * cancellation;
  struct promo * nextPromo;
} *promo;


expr ref_e(expr e);
list ref_l(list e);
mlist ref_ml(mlist e);
part ref_pl(part e);
benef ref_b(benef e);
param ref_pa(param e);
promo ref_pro(promo e);

void unref_e(expr e);
void unref_l(list e);
void unref_ml(mlist e);
void unref_pl(part e);
void unref_req(mlist e);
void unref_b(benef e);
void unref_pa(param e);
void unref_pr(promo e);


typedef struct artIdent {
  long codInt;
  char codBar[21];
  long codPresentacion;
  double cant;
} artIdent;

expr new_amset(void);
void ins_amset(expr e, identifier a, double k);
void del_amset(expr e, identifier a, double k);
expr clon_amset(expr e);

// Devuelve un articulo y su cantidad en rta, y lo elimina del article-amset.
// devuelve 1 en caso de exito y 0 en caso de error o amset vacio.
int get_amset(expr e, artIdent *rta);

expr new_means(void);
void ins_cash(expr e, double k);
void ins_ctacte(expr e, double k);
void ins_card(expr e, identifier a, identifier p, double k);
void ins_mutual(expr e, identifier a, double k);
void ins_ticket(expr e, identifier a, double k);
void ins_cheque(expr e, identifier a, double k);
void ins_currency(expr e, identifier a, double k);
expr clon_means(expr e);

expr new_points(void); //crea un multiconjunto de puntos vacio.
void ins_point(expr e, identifier a, double k); //inserta puntos en el amset_points

expr new_ebonus(void); 
void ins_ebonus(expr e, identifier a, double k);

expr new_part(void);
void ins_part(expr e, identifier rep, identifier a); 
//agrega el articulo 'a' a la clase de la particion 'e' cuyo representante es 'rep'.
//pre: 'a' no pertenece a ninguna clase de la particion 'e'

int idcmp(identifier, identifier);

typedef enum {COD_INTERNO, COD_BARRA, COD_INTERNO_BARRA, 
			  COD_INTERNO_PRECIO, COD_BARRA_PRECIO, COD_INTERNO_BARRA_PRECIO,
			  COD_INTERNO_BARRA_PRESENTACION, COD_INTERNO_BARRA_PRESENTACION_PRECIO,
			  COD_ERROR} TIPO_COD_ARTICULO;

//TIPO_COD_ARTICULO getCodsArt( identifier ident, long* cod_int, _int64 * cod_bar, double * precio);
//TIPO_COD_ARTICULO getCodsArt( identifier ident, long* cod_int, char * cod_bar, double * precio);
TIPO_COD_ARTICULO getCodsArt( identifier ident, long* cod_int, char * cod_bar, double * precio, long* cod_presentacion);

#endif

#pragma pack (pop) 
