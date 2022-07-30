#ifndef BANCOS_H
#define BANCOS_H
int ABRIR_CHEQUES_CLIENTES( void );
void CLOSE_CHEQUE_CLIENTES( void );
int PIDE_BANCO( char *cuenta_banco );
int PIDE_BANCO_CLIENTE( char *cuenta_banco );
int PIDE_NRO_BANCO();
int EXISTE_BANCO( int nro );
int EXISTE_BANCO__( int nro );
int PIDE_CUENTA_BANCO_CLIENTE( int banco, char *cuenta_banco );
int GRABA_CHEQUE_CLIENTE( int h, int pos, int borrar );
int EXISTEN_CHEQUES_CLIENTE();
int BORRA_CHEQUE_CLIENTE( int h );
void BUSCA_NOMBRE_BANCO( int codigo_banco );
int GRABAR_CHEQUES_EN_DBROUTER();
void BUSCAR_BANCO( int codigo_banco, char *aux );
int ELIMINA_CHEQUE( int cheque, int banco, char *cheque_str );
int CANCELA_CHEQUES( void );
#endif
