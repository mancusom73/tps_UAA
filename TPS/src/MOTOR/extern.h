#pragma pack (push,1) 
#include"types2.h"
#include"time2.h"

#pragma pack (1)


// Nota: identifier = char*

//void credit(expr e1, expr e2);
void credit( double cant, identifier article, double amount, expr e );

//void distribute( identifier article, double cant, double val );
void distribute( long promo_id, char* promoIdExtern, long grupo, identifier article, double cantidad, double precio );

//void issueBonus(expr e1, expr e2);
void issueBonus( identifier target, identifier type, double valor, double cantidad, char *leyenda, idate vencimiento, double validez );

//void givePoints(expr e1, expr e2);
void givePoints( identifier point, double num );

void activateCard( long promoId, identifier submedio, identifier plan, double num );
void activateCurrency( identifier ident, double num );
void activateMutual( long promoId, identifier submedio, double num );
void activateCheque( long promoId, identifier submedio, double num );
void activateTicket( long promoId, identifier submedio, double num );
void activateCash( double num );

void extProc( identifier procName, char* params );

// MODO 0 -> Se debe pagar todo el ticket con alguno de los medios del grupo.
// MODO 1 -> El monto pagado con los medios del mismo grupo debe cubrir el total del ticket.
// MODO 2 -> Se debe pagar al menos $'num' con alguno de los medios del grupo.
// MODO 3 -> El monto pagado con los medios del mismo grupo debe cubrir al menos $'num'.
void reqCard( int group, int modo, identifier submedio, identifier plan, double num );
void reqCurrency( int group, int modo, identifier ident, double num );
void reqMutual( int group, int modo, identifier ident, double num );
void reqCheque( int group, int modo, identifier ident, double num );
void reqTicket( int group, int modo, identifier ident, double num );
void reqCash( int group, int modo, double num );
void reqCtacte( int group, int modo, double num );

void recArt( long promoId, char* promoIdExt, identifier art, double cant, long grupo );

void recCash( long promoId, char* promoIdExt, long grupo );
void recCtacte( long promoId, char* promoIdExt, long grupo );
void recMutual( long promoId, char* promoIdExt, identifier name, long grupo );
void recTicket( long promoId, char* promoIdExt, identifier name, long grupo );
void recCheque( long promoId, char* promoIdExt, identifier name, long grupo );
void recCurrency( long promoId, char* promoIdExt, identifier name, long grupo );
void recCard( long promoId, char* promoIdExt, identifier name, identifier plan, long grupo );
void recAll( long promoId, char* promoIdExt, long grupo );
void recPerfil( long promoId, char* promoIdExt, long perfil, long grupo );
void recCredit( long promoId, char* promoIdExt, double cant, long grupo );
void recDistribute( long promoId, char* promoIdExt, double cant, long grupo );
void recBonus( long promoId, char* promoIdExt, identifier type, double value, double cant, long grupo );
void recPoints( long promoId, char* promoIdExt, identifier point, double cant, long grupo );
void recActivateCard( long promoId, char* promoIdExt, identifier name, identifier plan, double monto, long grupo );
void recActivateTicket( long promoId, char* promoIdExt, identifier name, double monto, long grupo );
void recActivateMutual( long promoId, char* promoIdExt, identifier name, double monto, long grupo );
void recLog( identifier target, char *leyenda);

char* expr2str(expr e);

int inDepartment( identifier art, identifier dep );

int inClass( identifier art, identifier nivel, identifier concept );

//double amount(expr e1, double n);
double amount( identifier article, double n );

int affiliated( void );

idate dateofbirth( void );

int holiday( void );

//expr purchase(void);
expr buyed_articles( void );
expr delivered_articles( void );
expr discounts_credits( void );

expr means( void );
expr change( void );
expr returned( void );

double num_rnv( identifier ident );

int bool_rnv( identifier ident );

char *string_rnv( identifier ident );

long date_rnv( identifier ident );

long time_rnv( identifier ident );

void num_rnv_update( identifier ident, double n );

void bool_rnv_update( identifier ident, int b );

void string_rnv_update( identifier ident, char *b );

void date_rnv_update( identifier ident, long b );

void time_rnv_update( identifier ident, long b );

int countable( identifier ident );

void print( identifier string );

double cashdesk( void );

expr points( void );

expr ebonus( void );

int _input( char *str );

double numinput( char *str );

idate get_date();
itime get_time();


typedef struct cashdesk_state *cashdesk_state;

// devuelve una estructura que representa al estado actual de la caja
cashdesk_state actual_state( void );

// modifica el estado actual para dejarlo en un estado
// igual a s.
void recover_state( cashdesk_state s );

// imprime todos los elementos del ticket producidos 
// desde el estado s hasta el estado actual.
void redo_actions( cashdesk_state s );

cashdesk_state free_cashdesk_state( cashdesk_state s );

cashdesk_state cashdesk_state_set_str( cashdesk_state s, int n, char *str );

#pragma pack (pop) 
