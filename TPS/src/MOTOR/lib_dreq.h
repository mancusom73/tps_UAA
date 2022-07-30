#pragma pack (push,1) 
#ifndef LIB_DREQ
#define LIB_DREQ

#pragma pack (1)

#define cod_dep long

void init_department_counts ( void );

double * department_count_ref( cod_dep dep );
  /* Inicializa el código de departamento 'cod_dep' en el AVL
   * y devuelve un puntero al contador que indica la cantidad de 
   * artículos de dicho departamento que han sido incluidos en el ticket.
   */

double * add_department_count( cod_dep dep, double count );

void restart_department_counts ( void );

void destroy_department_counts ( void );


#endif //LIB_DREQ

#pragma pack (pop) 
