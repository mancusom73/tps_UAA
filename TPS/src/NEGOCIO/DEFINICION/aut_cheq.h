#ifndef AUT_CHEQ_H
#define AUT_CHEQ_H
int VALIDAR_CHEQUE( int ind_medio, int sucursal, int nro_caja, long cajero, long nro_boleta,
                    char *nro_rut_comprador, char *nro_rut_girador, int nro_banco, char *nro_cuenta,
                    long nro_cheque, int nro_plaza, int tipo_credito, double monto,
                    unsigned fecha_vencimiento, int habilitado );
#endif
