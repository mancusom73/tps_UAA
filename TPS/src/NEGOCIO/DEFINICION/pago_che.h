#ifndef PAGO_CHE_H
#define PAGO_CHE_H
int CHEQUE_EN_BOLETIN( char *cuenta, long nro_cheque );
int BUSCAR_CLIENTE_EN_BOLETIN( char *banco, char *cuenta, char *cuit, char *telefono,
                               int *habilitado, long nro_cheque, int lector, char *serie,
                               int plaza );
int PIDE_CUENTA_CHEQUE( char *cta );
int CHEQUEAR_DIGITO_VERIFICADOR_EN_RUT( char *cuit );
long PIDE_NRO_CHEQUE( void );
unsigned PIDE_FECHA_VTO( void );
int PIDE_COD_POST( void );
int PIDE_COD_HORA( void );
int PIDE_CUIT( char *cuit, int otro_cuit, char *titulo, unsigned pos );
int PIDE_TELEFONO( char *telefono, int otro_tel, char *titulo, unsigned pos );
void REPROCESAR_CHEQUES( char tipo_pago );
void IMPRIME_DATOS_CHEQUE( long nro, char *nom_banco, unsigned fecha, char *picture );
int PIDE_DATOS_CHEQUE( int al_dia, long *numero, int *banco, unsigned * fecha_vto,
                       int *cod_post_cheq, int *cod_hora_acre, char *cuit, char *telefono,
                       char *cuenta_banco, int *cod_valida_cheque, int *plaza,
                       int pedir_fecha_vto );
void VERIFICAR_CHEQUE_EN_BOLETIN( void );
int PIDE_PLAZA( int *numero );
int BUSCAR_DATOS_CHEQUE_EN_ARCHIVO_CLIENTES( char *banco, char *cuenta, int plaza, char *cuit,
                                             char *telefono );
int MOD11( int suma );
int PIDE_DATOS_COMPRADOR_EN_CHEQUE( char *cuit, char *telefono, int al_dia, unsigned * fecha_vto,
                                    int lector );
int PIDE_DATOS_CHEQUE_FALTANTES( int solicitar_supervisor, int al_dia, long *numero, char *banco,
                                 unsigned * fecha_vto, int *cod_post_cheq, int *cod_hora_acre,
                                 char *cuit, char *telefono, int pedir_fecha_vto );
void CEREAR_DATOS_CHEQUE( long *numero, char *banco, unsigned * fecha_vto, int *cod_post_cheq,
                          int *cod_hora_acre, char *cuit, char *telefono );
int VALIDAR_DIGITO_VERIFICADOR_EN_RUT( char *rut );
int PIDE_CADENA_NRO_CHEQUE( char *numero );
#endif
