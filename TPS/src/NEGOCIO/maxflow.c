/* Executor of MF code (for Push-Relabel) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "values.h"
#define MAXLONG 2147483647

#pragma pack(1)

/* statistic variables */
long n_push  = 0;         /* number of pushes */
long n_rel   = 0;         /* number of relabels */
long n_up    = 0;         /* number of updates */
long n_gap   = 0;         /* number of gaps */
long n_gnode = 0;         /* number of nodes after gap */  
//float t, t2;

/* definitions of types: node & arc */

//#include "types_pr.h"
/* defs.h */

typedef  /* arc */
   struct arc_st
{
   long             r_cap;           /* residual capasity */
   struct node_st   *head;           /* head */
   struct arc_st    *sister;         /* opposite arc */
   struct arc_st    *next;           /* next arc with the same tail */
}
  arc;

typedef  /* node */
   struct node_st
{
   arc              *first;           /* first outgoing arc */
   arc              *current;         /* current incident arc */
   long             excess;           /* excess of the node */
   long             rank;             /* distance from the sink */
   struct node_st   *nl_next;         /* next node in layer-list */
   struct node_st   *nl_prev;         /* previous node in layer-list */
} node;


typedef /* layer */
   struct layer_st
{
   node             *push_first;      /* 1st node with pisitive excess */
   node             *trans_first;     /* 1st node with zero excess */
} layer;


/* parser for getting extended DIMACS format input and transforming the
   data to the internal representation */

//#include "parser_fl.c"
/* ----------------------------------------------------------------- */

int parse( grafo, size, n_ad, m_ad, nodes_ad, arcs_ad, cap_ad,
           source_ad, sink_ad, node_min_ad )

/* all parameters are output */
char	*grafo;				   /* buffer que contiene el grafo */
int 	size;				   /* tamaño del buffer */
long    *n_ad;                 /* address of the number of nodes */
long    *m_ad;                 /* address of the number of arcs */
node    **nodes_ad;            /* address of the array of nodes */
arc     **arcs_ad;             /* address of the array of arcs */
long    **cap_ad;              /* address of the array of capasities */
node    **source_ad;           /* address of the pointer to the source */
node    **sink_ad;             /* address of the pointer to the source */
long    *node_min_ad;          /* address of the minimal node */

{

#define MAXLINE       100	/* max line length in the input file */
#define ARC_FIELDS      3	/* no of fields in arc line  */
#define NODE_FIELDS     2	/* no of fields in node line  */
#define P_FIELDS        3       /* no of fields in problem line */
#define PROBLEM_TYPE "max"      /* name of problem type*/


long    n,                      /* internal number of nodes */
        node_min = 0,               /* minimal no of node  */
        node_max = 0,               /* maximal no of nodes */
       *arc_first = 0,              /* internal array for holding
                                     - node degree
                                     - position of the first outgoing arc */
       *arc_tail = NULL,               /* internal array: tails of the arcs */
        source = 0,                 /* no of the source */
        sink = 0,                   /* no of the sink */
        /* temporary variables carrying no of nodes */
        head, tail, i;

long    m,                      /* internal number of arcs */
        /* temporary variables carrying no of arcs */
        last, arc_num, arc_new_num;

node    *nodes = NULL,                 /* pointer to the node structure */
        *head_p,
        *ndp;

arc     *arcs = NULL,                  /* pointer to the arc structure */
        *arc_current = NULL,
        *arc_new,
        *arc_tmp;

long    *acap = NULL,                  /* array of capasities */
        cap;                    /* capasity of the current arc */

long    no_lines=0,             /* no of current input line */
        no_plines=0,            /* no of problem-lines */
        no_nslines=0,           /* no of node-source-lines */
        no_nklines=0,           /* no of node-source-lines */
        no_alines=0,            /* no of arc-lines */
        pos_current=0;          /* 2*no_alines */

//char    in_line[MAXLINE],       /* for reading input line */
char    *in_line,		        /* for reading input line */
        pr_type[4],             /* for reading type of the problem */
        nd;                     /* source (s) or sink (t) */

int     k,                      /* temporary */
        err_no;                 /* no of detected error */

/* -------------- error numbers & error messages ---------------- */
#define EN1   0
#define EN2   1
#define EN3   2
#define EN4   3
#define EN6   4
#define EN10  5
#define EN7   6
#define EN8   7
#define EN9   8
#define EN11  9
#define EN12 10
#define EN13 11
#define EN14 12
#define EN16 13
#define EN15 14
#define EN17 15
#define EN18 16
#define EN21 17
#define EN19 18
#define EN20 19
#define EN22 20

static char *err_message[] = 
  { 
/* 0*/    "more than one problem line.",
/* 1*/    "wrong number of parameters in the problem line.",
/* 2*/    "it is not a Max Flow problem line.",
/* 3*/    "bad value of a parameter in the problem line.",
/* 4*/    "can't obtain enough memory to solve this problem.",
/* 5*/    "more than one line with the problem name.",
/* 6*/    "can't read problem name.",
/* 7*/    "problem description must be before node description.",
/* 8*/    "this parser doesn't support multiply sources and sinks.",
/* 9*/    "wrong number of parameters in the node line.",
/*10*/    "wrong value of parameters in the node line.",
/*11*/    " ",
/*12*/    "source and sink descriptions must be before arc descriptions.",
/*13*/    "too many arcs in the input.",
/*14*/    "wrong number of parameters in the arc line.",
/*15*/    "wrong value of parameters in the arc line.",
/*16*/    "unknown line type in the input.",
/*17*/    "reading error.",
/*18*/    "not enough arcs in the input.",
/*19*/    "source or sink doesn't have incident arcs.",
/*20*/    "can't read anything from the input file."
  };
/* --------------------------------------------------------------- */

/* The main loop:
        -  reads the line of the input,
        -  analises its type,
        -  checks correctness of parameters,
        -  puts data to the arrays,
        -  does service functions
*/

//FILE * arch;
char* sep ="\n";
char *buf = NULL, *bufOrig = NULL;
memset(pr_type,'\0',4);
//while ( gets ( in_line ) != NULL )
//arch = fopen ("in2", "r");
buf = (char*)malloc(size + 1);
bufOrig = buf;
strncpy( buf, grafo, size + 1 );
in_line = strtok( buf, sep);
//while ( fgets ( in_line, MAXLINE, arch ) != NULL )
//while ( ( in_line = strsep( &buf, sep) ) != NULL )
while ( in_line != NULL )
{
  no_lines ++;


  switch (in_line[0])
    {
      case 'c':                  /* skip lines with comments */
      case '\n':                 /* skip empty lines   */
      case '\0':                 /* skip empty lines at the end of file */
                break;

      case 'p':                  /* problem description      */
                if ( no_plines > 0 )
                   /* more than one problem line */
                   { err_no = EN1 ; goto error; }

                no_plines = 1;
   
                if (
        /* reading problem line: type of problem, no of nodes, no of arcs */
                    sscanf ( in_line, "%*c %3s %ld %ld", pr_type, &n, &m )
                != P_FIELDS
                   )
		    /*wrong number of parameters in the problem line*/
		    { err_no = EN2; goto error; }

                if ( strcmp ( pr_type, PROBLEM_TYPE ) )
		    /*wrong problem type*/
		    { err_no = EN3; goto error; }

                if ( n <= 0  || m <= 0 )
		    /*wrong value of no of arcs or nodes*/
		    { err_no = EN4; goto error; }

        /* allocating memory for  'nodes', 'arcs'  and internal arrays */
                nodes    = (node*) calloc ( n+2, sizeof(node) );
		arcs     = (arc*)  calloc ( 2*m+1, sizeof(arc) );
	        arc_tail = (long*) calloc ( 2*m,   sizeof(long) ); 
		arc_first= (long*) calloc ( n+2, sizeof(long) );
                acap     = (long*) calloc ( 2*m, sizeof(long) );
                /* arc_first [ 0 .. n+1 ] = 0 - initialized by calloc */

                if ( nodes == NULL || arcs == NULL || 
                     arc_first == NULL || arc_tail == NULL )
                    /* memory is not allocated */
		    { err_no = EN6; goto error; }
		     
		/* setting pointer to the first arc */
		arc_current = arcs;

                break;

      case 'n':		         /* source(s) description */
		if ( no_plines == 0 )
                  /* there was not problem line above */
                  { err_no = EN8; goto error; }

                /* reading source  or sink */
		k = sscanf ( in_line,"%*c %ld %c", &i, &nd );
 
		if ( k < NODE_FIELDS )
                  /* node line is incorrect */
                  { err_no = EN11; goto error; }

		if ( i < 0 || i > n )
                  /* wrong value of node */
                  { err_no = EN12; goto error; }

		switch ( nd )
		  {
		  case 's':  /* source line */
		    
		    if ( no_nslines != 0)
		      /* more than one source line */ 
		      { err_no = EN9; goto error; }

		    no_nslines = 1;
		    source = i;
		    break;

		  case 't':  /* sink line */

		    if ( no_nklines != 0)
		      /* more than one sink line */
		      { err_no = EN9; goto error; }

		    no_nklines = 1;
		    sink = i;
		    break;

		  default:
		    /* wrong type of node-line */
                    err_no = EN12; goto error; 
		    break;
		  }

                node_max = 0;
                node_min = n;
		break;

      case 'a':                    /* arc description */
		if ( no_nslines == 0 || no_nklines == 0 ) 
                  /* there was not source and sink description above */
                  { err_no = EN14; goto error; }

		if ( no_alines >= m )
                  /*too many arcs on input*/
                  { err_no = EN16; goto error; }
		
		if (
                    /* reading an arc description */
                    sscanf ( in_line,"%*c %ld %ld %ld",
                                      &tail, &head, &cap )
                    != ARC_FIELDS
                   ) 
                    /* arc description is not correct */
                    { err_no = EN15; goto error; }

		if ( tail < 0  ||  tail > n  ||
                     head < 0  ||  head > n  
		   )
                    /* wrong value of nodes */
		    { err_no = EN17; goto error; }

               /* no of arcs incident to node i is stored in arc_first[i+1] */
		arc_first[tail + 1] ++; 
		arc_first[head + 1] ++;

                /* storing information about the arc */
		arc_tail[pos_current]        = tail;
		arc_tail[pos_current+1]      = head;
		arc_current       -> head    = nodes + head;
		arc_current       -> r_cap    = cap;
		arc_current       -> sister  = arc_current + 1;
	      ( arc_current + 1 ) -> head    = nodes + tail;
	      ( arc_current + 1 ) -> r_cap    = 0;
	      ( arc_current + 1 ) -> sister  = arc_current;

		/* searching minimumu and maximum node */
                if ( head < node_min ) node_min = head;
                if ( tail < node_min ) node_min = tail;
                if ( head > node_max ) node_max = head;
                if ( tail > node_max ) node_max = tail;

		no_alines   ++;
		arc_current += 2;
		pos_current += 2;
		break;

	default:
		/* unknown type of line */
		err_no = EN18; goto error;
		break;

    } /* end of switch */
  in_line = strtok( NULL, sep);
}     /* end of input loop */

/* ----- all is red  or  error while reading ----- */ 

if ( bufOrig != NULL ) {
	free( bufOrig );
}
//if ( feof (arch) == 0 ) /* reading error */
//  { err_no=EN21; goto error; } 

if ( no_lines == 0 ) /* empty input */
  { err_no = EN22; goto error; } 

if ( no_alines < m ) /* not enough arcs */
  { err_no = EN19; goto error; } 

//fclose(arch);
/********** ordering arcs - linear time algorithm ***********/

/* first arc from the first node */
( nodes + node_min ) -> first = arcs;

/* before below loop arc_first[i+1] is the number of arcs outgoing from i;
   after this loop arc_first[i] is the position of the first 
   outgoing from node i arcs after they would be ordered;
   this value is transformed to pointer and written to node.first[i]
   */
 
for ( i = node_min + 1; i <= node_max + 1; i ++ ) 
  {
    arc_first[i]          += arc_first[i-1];
    ( nodes + i ) -> first = arcs + arc_first[i];
  }


for ( i = node_min; i < node_max; i ++ ) /* scanning all the nodes  
                                            exept the last*/
  {

    last = ( ( nodes + i + 1 ) -> first ) - arcs;
                             /* arcs outgoing from i must be cited    
                              from position arc_first[i] to the position
                              equal to initial value of arc_first[i+1]-1  */

    for ( arc_num = arc_first[i]; arc_num < last; arc_num ++ )
      { tail = arc_tail[arc_num];

	while ( tail != i )
          /* the arc no  arc_num  is not in place because arc cited here
             must go out from i;
             we'll put it to its place and continue this process
             until an arc in this position would go out from i */

	  { arc_new_num  = arc_first[tail];
	    arc_current  = arcs + arc_num;
	    arc_new      = arcs + arc_new_num;
	    
	    /* arc_current must be cited in the position arc_new    
	       swapping these arcs:                                 */

	    head_p               = arc_new -> head;
	    arc_new -> head      = arc_current -> head;
	    arc_current -> head  = head_p;

	    cap                 = arc_new -> r_cap;
	    arc_new -> r_cap     = arc_current -> r_cap;
	    arc_current -> r_cap = cap;

	    if ( arc_new != arc_current -> sister )
	      {
	        arc_tmp                = arc_new -> sister;
	        arc_new  -> sister     = arc_current -> sister;
	        arc_current -> sister  = arc_tmp;

                ( arc_current -> sister ) -> sister = arc_current;
		( arc_new     -> sister ) -> sister = arc_new;
	      }

	    arc_tail[arc_num] = arc_tail[arc_new_num];
	    arc_tail[arc_new_num] = tail;

	    /* we increase arc_first[tail]  */
	    arc_first[tail] ++ ;

            tail = arc_tail[arc_num];
	  }
      }
    /* all arcs outgoing from  i  are in place */
  }       

/* -----------------------  arcs are ordered  ------------------------- */

/*----------- constructing lists ---------------*/


  for ( ndp = nodes + node_min; ndp <= nodes + node_max;  ndp ++ )
      ndp -> first = (arc*) NULL;

  for ( arc_current = arcs + (2*m-1); arc_current >= arcs; arc_current -- )
    {
      arc_num = arc_current - arcs;
      tail = arc_tail [arc_num];
      ndp = nodes + tail;
      arc_current -> next = ndp -> first;
      ndp -> first = arc_current;
    }


/* ----------- assigning output values ------------*/
*m_ad = m;
*n_ad = node_max - node_min + 1;
*source_ad = nodes + source;
*sink_ad   = nodes + sink;
*node_min_ad = node_min;
*nodes_ad = nodes + node_min;
*arcs_ad = arcs;
*cap_ad = acap;

for ( arc_current = arcs, arc_num = 0; 
      arc_num < 2*m;
      arc_current ++, arc_num ++
    )
     acap [ arc_num ] = arc_current -> r_cap; 

if ( source < node_min || source > node_max )
  /* bad value of the source */
  { err_no = EN20; goto error; }
  
if ( (*source_ad) -> first == (arc*) NULL ||
     (*sink_ad  ) -> first == (arc*) NULL    ) 
  /* no arc goes out of the source */
  { err_no = EN20; goto error; }

/* free internal memory */
free ( arc_first ); free ( arc_tail );

/* Thanks God! all is done */
return (0);

/* ---------------------------------- */
 error:  /* error found reading input */

printf ( "\nline %ld of input - %s\n", 
         no_lines, err_message[err_no] );

return (1);

}
/* --------------------   end of parser  -------------------*/


/* function 'timer()' for mesuring processor time */

//#include "timer.c"

/* function for constructing maximal flow */

//#include "m_prf.c"
/* Maximal flow - Push-Relabel algorithm */
/* Highest level */
/* 01/09/94 - Stanford Computer Science Department */
/* Boris Cherkassky - cher@theory.stanford.edu, on.cher@zib-berlin.de */
/* Andrew V. Goldberg - goldberg@cs.stanford.edu */

#define BIGGEST_FLOW MAXLONG
#define MIN( a, b ) ( ( (a) < (b) ) ? a : b )
#define GLOB_UPDT_FREQ 1.0
#define WHITE 0
#define GREY 1
#define BLACK 2

#define FLOW(a) ( cap[a - arcs] - a -> r_cap )

/* global variables */

long   n;                    /* number of nodes */
node   *nodes;               /* array of nodes */
arc    *arcs;                /* array of arcs */
layer  *layers = NULL;              /* array of layers */
long   *cap;                 /* array of capasities */
node   *source;              /* origin */
node   *sink;                /* destination */
node   **queue = NULL;       /* queue for storing nodes */
node   **q_read, **q_write;  /* queue pointers */
long   lmax;                 /* maximal layer */
long   lmax_push;            /* maximal layer with excess node */
long   lmin_push;            /* minimal layer with excess node */


/*--- initialization */

int pr_init ( n_p, nodes_p, arcs_p, cap_p, source_p, sink_p )

long    n_p;       /* number of nodes */
node    *nodes_p;  /* array of nodes */
arc     *arcs_p;   /* array of arcs */
long    *cap_p;    /* array of capasitiies */
node    *source_p; /* origin          */
node    *sink_p;   /* destination     */

{
node  *i;        /* current node */

n      = n_p;
nodes  = nodes_p;
arcs   = arcs_p;
cap    = cap_p;
source = source_p;
sink   = sink_p;

queue = (node**) calloc ( n, sizeof (node*) );
if ( queue == NULL ) return ( 1 );

layers = (layer*) calloc ( n+2, sizeof (layer) );
if ( layers == NULL ) return ( 1 );

for ( i = nodes; i < nodes + n; i++ )
  i -> excess = 0;

source -> excess = BIGGEST_FLOW;

lmax = n-1;

return ( 0 );

} /* end of initialization */


/*--- global rank update - breadth first search */

void def_ranks ()

{

node  *i, *j, *jn;  /* current nodes */
arc   *a;           /* current arc   */
layer *l;           /* current layer */
long  j_rank;       /* rank of node j */

n_up ++; /* statistics */

/* initialization */

for ( i = nodes; i < nodes + n; i ++ )
  i    -> rank = n;

  sink -> rank = 0;

*queue = sink;

for ( l = layers; l <= layers + lmax; l++ )
  {
    l -> push_first   = NULL;
    l -> trans_first  = NULL;
  }

lmax = lmax_push = 0;
       lmin_push = n;

/* breadth first search */

for ( q_read = queue, q_write = queue + 1; 
      q_read != q_write;
      q_read ++
    )
   { /* scanning arcs incident to node i */

    i = *q_read;
    j_rank = ( i -> rank ) + 1;

    for ( a = i -> first; a != NULL; a = a -> next )
      {
        j = a -> head;

        if ( j -> rank == n )
          /* j is not labelled */

          if ( ( ( a -> sister ) -> r_cap ) > 0 )
    	{ /* arc (j, i) is not saturated */

    	  j -> rank    = j_rank;
    	  j -> current = j -> first;

	  l = layers + j_rank;
	  if ( j_rank > lmax ) lmax = j_rank;

	  if ( ( j -> excess ) > 0 )
	    {
	      j -> nl_next     = l -> push_first;
	      l -> push_first  = j;
	      if ( j_rank > lmax_push ) lmax_push = j_rank;
	      if ( j_rank < lmin_push ) lmin_push = j_rank;
	    }
	  else /* j -> excess == 0 */
	    {
	      jn = l -> trans_first;
	      j -> nl_next     = jn;
	      if ( jn != NULL )
		jn -> nl_prev = j;
	      l -> trans_first  = j; 
	    }

    	  *q_write = j; q_write ++; /* put j  to scanning queue */
    	}
      } /* node "i" is scanned */ 
  } /* end of scanning queue */

} /* end of global update */

/*--- removing excessive flow - second phase of PR-algorithm */
//#include "phase2.c"
/*--- removing excessive flow - second phase of PR-algorithm */
void prefl_to_flow ( )
/*
   do dsf in the reverse flow graph from nodes with excess
   cancel cycles if found
   return excess flow in topological order
*/

/*
   rank is used for dfs labels
   nl_prev is used for DSF tree
   nl_next is used for topological order list
*/
{
  node *i, *j, *tos = NULL, *bos, *restart, *r;
  arc *a;
  long delta;

  /* deal with self-loops */
  for ( i = nodes; i < nodes + n; i++ ) {
    for ( a = i -> first; a != NULL; a = a -> next )
      if ( a -> head == i ) {
	a -> r_cap = cap[a - arcs];
      }
  }

  /* initialize */
  bos = NULL;
  for ( i = nodes; i < nodes + n; i++ ) {
    i -> rank = WHITE;
    i -> nl_prev = NULL;
    i -> current = i -> first;
  }

  for ( i = nodes; i < nodes + n; i++ )
    if (( i -> rank == WHITE ) && ( i -> excess > 0 ) &&
	( i != source ) && ( i != sink )) {
      r = i;
      r -> rank = GREY;
      do {
	for ( ; i -> current != NULL; i -> current = i -> current -> next ) {
	  a = i -> current;
	  if (( cap[a - arcs] == 0 ) && ( a -> r_cap > 0 ) &&
	      ( a -> head != source) && ( a -> head != sink )) {
	    j = a -> head;
	    if ( j -> rank == WHITE ) {
	      /* start scanning j */
	      j -> rank = GREY;
	      j -> nl_prev = i;
	      i = j;
	      break;
	    }
	    else
	      if ( j -> rank == GREY ) {
		/* find minimum flow on the cycle */
		delta = a -> r_cap;
		while ( 1 ) {
		  delta = MIN ( delta, j -> current -> r_cap );
		  if ( j == i )
		    break;
		  else
		    j = j -> current -> head;
		}

		/* remove delta flow units */
		j = i;
		while ( 1 ) {
		  a = j -> current;
		  a -> r_cap -= delta;
		  a -> sister -> r_cap += delta;
		  j = a -> head;
		  if ( j == i )
		    break;
		}
	
		/* back DFS to the first zerod arc */
		restart = i;
		for ( j = i -> current -> head; j != i; j = a -> head ) {
		  a = j -> current;
		  if (( j -> rank == WHITE ) || ( a -> r_cap == 0 )) {
		    j -> current -> head -> rank = WHITE;
		    if ( j -> rank != WHITE )
		      restart = j;
		  }
		}
	
		if ( restart != i ) {
		  i = restart;
		  i -> current = i -> current -> next;
		  break;
		}
	      }
	  }
	}

	if ( i -> current == NULL ) {
	  /* scan of i complete */
	  i -> rank = BLACK;
	  if ( i != source ) {
	    if ( bos == NULL ) {
	      bos = i;
	      tos = i;
	    }
	    else {
	      i -> nl_next = tos;
	      tos = i;
	    }
	  }

	  if ( i != r ) {
	    i = i -> nl_prev;
	    i -> current = i -> current -> next;
	  }
	  else
	    break;
	}
      } while ( 1 );
    }


  /* return excesses */
  /* note that sink is not on the stack */
  if ( bos != NULL ) {
    i = tos;
    do {
      a = i -> first;
      while ( i -> excess > 0 ) {
	if (( cap[a - arcs] == 0 ) && ( a -> r_cap > 0 )) {
	  delta = MIN ( i -> excess, a -> r_cap );
	  a -> r_cap -= delta;
	  a -> sister -> r_cap += delta;
	  i -> excess -= delta;
	  a -> head -> excess += delta;
	}
	a = a -> next;
      }
      if ( i == bos )
	break;
      else
	i = i -> nl_next;
    } while ( 1 );
  }
}



/*--- pushing flow from node  i  */

int push ( i )

node  *i;      /* outpushing node */

{

node  *j;                /* sucsessor of i */
node  *j_next, *j_prev;  /* j's sucsessor and predecessor in layer list */
long  j_rank;            /* rank of the next layer */
layer *lj;               /* j's layer */
arc   *a;                /* current arc (i,j) */
long  fl;                /* flow to push through the arc */

j_rank = (i -> rank) - 1;

/* scanning arcs outgoing from  i  */

for ( a = i -> current; a != NULL; a = a -> next )
  {
    if ( a -> r_cap > 0 ) /* "a" is not saturated */
      {
	j = a -> head;

	if ( j -> rank == j_rank ) 
	  { /* j belongs to the next layer */

	    fl = MIN ( i -> excess, a -> r_cap );

	    a             -> r_cap -= fl;
	    (a -> sister) -> r_cap += fl;
n_push ++; /* statistics */

	    if ( j_rank > 0 )
	      {
		lj = layers + j_rank;

		if ( j -> excess == 0 )
		  { /* before current push  j  had zero excess */
		
		    /* remove  j  from the list of transit nodes */
		    j_next = j -> nl_next;
		
		    if ( lj -> trans_first == j )
		      /* j  starts the list */
		      lj -> trans_first = j_next;
		    else
		      { /* j  is not the first */
			j_prev = j -> nl_prev;
			j_prev -> nl_next = j_next;
			if ( j_next != NULL )
			  j_next -> nl_prev = j_prev;
		      }

		    /* put  j  to the push-list */
		    j -> nl_next = lj -> push_first;
		    lj -> push_first = j;

		    if ( j_rank < lmin_push )
		      lmin_push = j_rank;

		  } /* j -> excess == 0 */

	      } /* j -> rank > 0 */

	    j -> excess += fl;
	    i -> excess -= fl;

	    if ( i -> excess == 0 ) break;

	  } /* j belongs to the next layer */
      } /* a  is not saturated */
  } /* end of scanning arcs from  i */

i -> current = a;

return ( ( a == NULL ) ? 1 : 0 );

} /* end of push */

/*--- relabelling node i */

long relabel ( i )

node *i;   /* node to relabel */

{

node  *j;        /* sucsessor of i */
long  j_rank;    /* minimal rank of a node available from j */
arc   *a;        /* current arc */
arc   *a_j = NULL;      /* an arc which leads to the node with minimal rank */
layer *l;        /* layer for node i */

n_rel ++; /* statistics */

i -> rank = j_rank = n;

/* looking for a node with minimal rank available from i */

for ( a = i -> first; a != NULL; a = a -> next )
  {
    if ( a -> r_cap > 0 )
      {
	j = a -> head;

	if ( j -> rank < j_rank )
	  {
	    j_rank = j -> rank;
	    a_j    = a;
	  }
      }
  }
      
j_rank++;
if ( j_rank < n )
  {
    /* siting  i  into the manual */

    i -> rank    = j_rank;
    i -> current = a_j;

    l = layers + j_rank;

    if ( i -> excess > 0 ) 
      {
        i -> nl_next = l -> push_first;
        l -> push_first = i;
        if ( j_rank > lmax_push ) lmax_push = j_rank;
        if ( j_rank < lmin_push ) lmin_push = j_rank;
      }
    else
      {
        j = l -> trans_first;
	i -> nl_next = j;
	if ( j != 0 ) j -> nl_prev = i;
	l -> trans_first = i;
      }

    if ( j_rank > lmax ) lmax = j_rank;

  } /* end of j_rank < n */
      
return ( j_rank );

} /* end of relabel */


/*--- head program */

int prflow ( n_p, nodes_p, arcs_p, cap_p, source_p, sink_p, fl )

long   n_p;         /* number of nodes */
node   *nodes_p;    /* array of nodes */
arc    *arcs_p;     /* array of arcs  */
long   *cap_p;      /* capasity */
node   *source_p;   /* origin */
node   *sink_p;     /* destination */
double *fl;         /* flow amount*/

{

node   *i;          /* current node */
node   *j;          /* i-sucsessor in layer list */
layer  *l;          /* current layer */
long   n_r;         /* the number of relabels */
int    cc;          /* condition code */


cc = pr_init ( n_p, nodes_p, arcs_p, cap_p, source_p, sink_p );

if ( cc ) return ( cc );

def_ranks ();

n_r = 0;

/* highest level method */

while ( lmax_push >= lmin_push ) /* main loop */
  { 
    l = layers + lmax_push;

    i = l -> push_first;


    if ( i == NULL )
      { /* nothing to push from this level */ 

	lmax_push --;
      }
    else
      {
	l -> push_first = i -> nl_next;

	cc = push ( i );

	if ( cc )
	  { /* i must be relabeled */

	    relabel ( i );
	    n_r ++;

	    /* checking the necessity of global update */
	    if ( n_r > GLOB_UPDT_FREQ * (float) n ) 
	      { /* it is time for global update */
		def_ranks ();
		n_r = 0;
	      }
	    else if ( i -> rank == n )
	      {
		j = l -> trans_first;
		i -> nl_next = j;
		l -> trans_first = i;
		if ( j != NULL ) j -> nl_prev = i;
	      }
	  }
	else
	  { /* excess is pushed out of  i  */

	    j = l -> trans_first;
	    i -> nl_next = j;
	    l -> trans_first = i;
	    if ( j != NULL ) j -> nl_prev = i;
	  }
      }
    
  } /* end of the main loop */
    
*fl += sink -> excess;

//t2 = timer() - t2;

prefl_to_flow ( );

return ( 0 );

} /* end of constructing flow */



double maxFlow ( grafo, size )

char *grafo;
int size;

{

arc *arp = NULL;
long *cap = NULL;
node *ndp = NULL, *source = NULL, *sink = NULL;
long n, m, nmin; 
long ni, na;
double flow = 0;
int  prt;
int cc;

//prt     = ( argc > 1 ) ? atoi( argv[1] ): 1;
prt     = 1;

#define N_NODE( i ) ( (i) - ndp + nmin )
#define N_ARC( a )  ( ( a == NULL )? -1 : (a) - arp )

//printf("c\nc maxflow - push-relabel (highest level, no gaps)\nc\n");

if ( parse( grafo, size, &n, &m, &ndp, &arp, &cap, &source, &sink, &nmin ) ) {
	// el grafo contiene error, no es conexo, o ...
	return 0;
}

//printf("c nodes:       %10ld\nc arcs:        %10ld\nc\n", n, m);

cc = prflow ( n, ndp, arp, cap, source, sink, &flow );

if ( cc ) { 
	fprintf ( stderr, "Allocating error\n"); 
	return 0; 
}


//printf ("c flow:        %10.0lf\nc\n", flow );

/*
if ( prt )
  {
    printf ("c pushes:      %10ld\n", n_push);
    printf ("c relabels:    %10ld\n", n_rel);
    printf ("c updates:     %10ld\n", n_up);
    printf ("c\n");

  }

if ( prt > 1 )
  {
    printf ("s %.0lf\n", flow);

    for ( i = ndp; i < ndp + n; i ++ )
      {
	ni = N_NODE(i);
	for ( a = i -> first; a != NULL; a = a -> next )
	  {
	    na = N_ARC(a);
	    if ( cap[na] > 0 )
	      printf ( "f %7ld %7ld %12ld\n",
		      ni, N_NODE( a -> head ), cap[na] - ( a -> r_cap )
		      );
	  }
      }
    printf("c\n");
  }
*/

/*
 * ReInicializacion variables locales.
 */
n = 0;
m = 0;
if (ndp) { free(ndp-nmin); ndp = NULL; }
if (arp) { free(arp); arp = NULL; }
if (cap) { free(cap); cap = NULL; }
source = NULL;
sink = NULL;
nmin = 0;
//flow = 0;
ni = na = 0;

/* global variables */

if ( layers ) { free(layers); layers = NULL; }  /* array of layers */
if ( queue ) { free(queue); queue = NULL; }   /* queue for storing nodes */

/* statistic variables */
n_push  = 0;         /* number of pushes */
n_rel   = 0;         /* number of relabels */
n_up    = 0;         /* number of updates */
n_gap   = 0;         /* number of gaps */
n_gnode = 0;         /* number of nodes after gap */  

/*
 * Fin ReInicializacion
 */

return flow;
}

