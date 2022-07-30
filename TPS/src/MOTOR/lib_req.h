#pragma pack (push,1) 
#ifndef LIB_REQS
#define LIB_REQS

//#include "../types2.h"

#pragma pack (1)

#define cod_art long
#define cod_bar long long

void init_articles_counts ( void );

double * article_count_ref( cod_art cod_int );
  /* Inicializa el código interno de articulo 'cod_int' en el AVL
   * y devuelve un puntero al contador que indica la cantidad en que
   * dicho artículo ha sido incluido en el ticket.
   */

double * add_article_count( cod_art cod_int, double count );


void restart_articles_counts ( void );

void destroy_articles_counts ( void );


#endif //LIB_REQS

#pragma pack (pop) 
