#ifndef IMP_COMP_H
#define IMP_COMP_H
void IMPRIME_COMPROBANTE( char *titulo, double importe, int modo, int comp, int comp_no_fiscal );
void IMPRIME_CABECERA_COMPROBANTE( char *titulo, int modo, int comp, int comp_no_fiscal );
void IMPRIME_PIE_COMPROBANTE( int modo, double total );
#endif
