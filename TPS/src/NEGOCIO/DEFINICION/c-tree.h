#ifndef C_TREE_H
#define C_TREE_H
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

#ifdef __cplusplus
extern    "C"
{
    #endif

    int Ctree_SET_STATUS( char orden, char valor );
    int Ctree_GET_STATUS( char orden );
    int Ctree_GET_PREVIOUS();
    int Ctree_GET_NEXT();
    int Ctree_USE_DATABASE( int area, char *nombre, char *estructura, int tam_estructura,
                            char *clave_acceso, int largo, int modo );
    int Ctree_GO( int registro );
    int Ctree_CLOSE_DATABASE( int area );
    void Ctree_SELECT( int num_area );
    int Ctree_SELECTED();
    int Ctree_UPDATE();
    int Ctree_SKIP( int salto );
    int Ctree__DELETE();
    int Ctree_TESTEAR( char orden );
    void Ctree_CLOSE_DB( int cod );
    void Ctree_CLOSE_ALL();
    void Ctree_ZAP();
    void Ctree_SET_ORDER( int number_key );
    void Ctree_DELETE();
    void Ctree_RECALL();
    void Ctree_DELETE_ALL();
    void Ctree_RECALL_ALL();
    void Ctree_SETEAR( char orden, char bit );
    void Ctree_PACK();
    int Ctree_INSERT();
    int Ctree_EOF();
    int Ctree_Init();
    int Ctree_GET_EQUAL( char *clave );
    int Ctree_FOUND();
    int Ctree_GET_GREATER( char *clave );
    int Ctree_GET_GREATER_E( char *clave );
    int Ctree_GET_LESS( char *clave );
    int Ctree_GET_LESS_E( char *clave );
    int Ctree_BOF();
    int Ctree_GET_DIRECT( unsigned long posicion );
    long Ctree_GET_POSITION();
    int Ctree_GET_FIRST();
    int Ctree_INSERT_NO_MODIF_STATUS();
    int Ctree_DELETED();
    void Ctree_GET_LOCKS( char *lock, char *wait, char *multiple );
    void Ctree_SET_LOCKS( char lock, char wait, char multiple );
    int Ctree_GET_ORDER( void );
    int Ctree_EMPTY();
    unsigned long Ctree_RECCOUNT( void );
    void Ctree_RECALL();
    int Ctree_DELETED();
    void Ctree_DELETE_ALL();
    void Ctree_RECALL_ALL();
    int Ctree_END_TRANSACTION();
    int Ctree_INSERT_VARIABLE( int long_reg );
    int Ctree_BEGIN_TRANSACTION();
    int Ctree_BEGIN_TRANSACTION_CONCURRENTE();
    int Ctree_UNLOCK();
    int Ctree_UNLOCK_SINGLE( long registro );
    int Ctree_UNLOCK_ALL();
    int Ctree_GET_LAST();
    int Ctree_CLOSE_DATABASE( int area );
    int Ctree_ABORT_TRANSACTION();
    int Ctree_SIZEOF_REC_DB( int area );
    char *Ctree_BUFFER_REC_DB( int area );


    #ifdef __cplusplus
}
#endif
#endif
