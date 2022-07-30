#ifndef F_FIJO_H
#define F_FIJO_H
void CR1_FONDO_FIJO();
int CR1_FONDO_FIJO_MULT_MEDIOS();
void FONDO_FIJO_MULTIPLE_CONFECCIONA_PANTALLA();
void FONDO_FIJO_AGREGAR_MEDIO( int pos, char modo, double importe, unsigned cuenta, int id_registro,
                               double importe_me, int banco, long nro_cheque, long autorizacion, int mostrar );
void FONDO_FIJO_INGRESA_MEDIO( int nro );
int FONDO_FIJO_PIDE_MEDIO();
int MOSTRAR_FONDO_FIJO( int pos );
#endif
