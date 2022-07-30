#ifndef DBMEM_H
#define DBMEM_H
void DBMEM_RECORRER( int cod, int ( *funcion )(), char *datos );
void _DBMEM_RECORRER( int area, int ( *funcion )(), char *datos );
int DBMEM_LOAD( int cod );
void DBMEM_GO( int registro );
int DBMEM_LOAD_SQL( int cod , int pTabla, int pTipo, char *indice);
int _DBMEM_LOAD_SQL( int area, char *nombre, int pTabla, int pTipo, char *indice);

#endif
