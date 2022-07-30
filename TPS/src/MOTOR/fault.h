#pragma pack (push,1) 
/*
 * MODULO de TOLERACIA A FALLAS
 */

#include "loglib.h"

#pragma pack (1)


typedef enum { normalState, recoveredState, recoveringState } rState;
extern rState recState;


void updateMode( void );
int startFaultToleranceEngine();
int recoverEngine();


int stopFaultToleranceEngine();

int restartFaultToleranceEngine();

//void* readData(id v, char ind, void * r, size_t size);

//void writeData(id v, char ind, void *data, size_t size);

//number readNumRnv(id v, char ind);
int readNumRnv( void *r, id v, char ind );

void writeNumRnv( id v, char ind, number e );

int readBoolInput( void );
int readNumInput( double * );
int readTime( long * );
int readDate( long * );
int readDay( long * );
//int readPurchase(void);

int logAddArticle( identifier id, number count, long department, double precio, char promocionable, char porImporte );
int logRemoveArticle( identifier id, number count, long department, double precio, char promocionable, char porImporte );
int logUpdateArticle( identifier id, number count, long department, double precio, char promocionable, char porImporte );
int logAddExternalDiscount( identifier id, number count, long department, double precio, char promocionable, char porImporte );

int logCredit( identifier id, number count );
int logDistribute( identifier id, number count, number value );
int logIssueBonus( identifier target, identifier type, number count, number value,
                   identifier lable );
int logGivePoints( identifier id, number count );
int logEProc( identifier id, char* params );

int logPanelVentas( void );
int logPanelPagos( void );
int logPanelPosPago( void );

int logBoolInput( int value );
int logNumInput( double value );
int logTime( long value );
int logDate( long value );
int logDay( long value );
int logPurchase( int value );
int logCashdeskState( void *s, int size, char *str0, char *str1, char *str2, char *str3,
                      char *str4 );

#pragma pack (pop) 
