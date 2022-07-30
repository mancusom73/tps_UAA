#ifndef RAM_H
#define RAM_H
void SET_RAM( int estado );
int VALIDAR_RAM();
int VERIFICAR_E_NRO();
int VERIFICAR_E_ACU( int est );
int _VAL_IMPORTE( int cod, int h );
int _VAL_CANTIDAD( int cod, int h );
int _VAL_IMPORTE_Y_CANTIDAD( int cod_imp, int cod_can, int h );
int __VAL_IMPORTE( int cod, int h, double desde, double hasta );
#endif
