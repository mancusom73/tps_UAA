#ifndef BOLETIN_H
#define BOLETIN_H
int BUSCAR_MEDIO_EN_BOLETIN( char tipo_boletin, int nro_archivo, char *nro_bono, double *importe,
                             int concatenar_importe, int verificar_importe, int local );

int PIDE_NRO_BONO( char *nro );
int EXISTE_BOLETIN_TCOMPRA_MUTUALES( int nro_archivo, char *nro_bono, double importe,
                                     int concatenar );
int ACTUALIZA_BOLETINES_TCOMPRA_MUTUALES( int *registros );
void CONTRUYE_MASCARA( char *nro_bono, double importe, int concatenar, char *dest, int size );
int OBTENER_BOLETIN_REMOTO( int nro_archivo, char *nro_bono, double importe, int concatenar );
void INICIALIZA_BOLETIN_TICKET();
#endif
