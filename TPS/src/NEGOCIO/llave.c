#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <b_mens.h> 
#include <cr.h> 
#include <llave.h> 
#include <mensaje.h> 
#include <tkt.h> 
#include <_cr1.h> 
#include <cmemory.h>
#include <actions.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
static char cmodo[6][11] = {"P","OFF","CAJERO","SUPERVISOR","X","Z"};
static char cmodo3[6][4] = { "L", "L", "R", "S", "S", "S" };
static char cmodo4[6][4] = { "OFF", "1", "1", "2", "3", "4" }; //kb78
/******************************************************************************/
int ESPERAR_LLAVE( int modo )
/******************************************************************************/
{
    int n = 0,k = 0, pos = -1, positionCount = 0;
    char mens[50];
    if( TIPO_LLAVE == LLAVE_DRIVER ) {
        switch( modo ) {
            case 'P':
                n = 0;
                break;
            case 'O':
                n = 1;
                break;
            case 'R':
                n = 2;
                break;
            case 'S':
                n = 3;
                break;
            case 'X':
                n = 4;
                break;
            case 'Z':
                n = 5;
                break;
        }
		positionCount = getData( ACTION_KEYLOCK_POSITION_COUNT, ( char* )&n, 0 );
		if( n > positionCount + 1 )
			n = 3; //si la posicion no la dispone la llave, entonces se va a esperar la posicion supervisor
		pos = getData( ACTION_KEYLOCK_GET_POSITION, ( char* )&n, 0 );
		if( pos != n ) {
			_snprintf(mens, sizeof(mens)-1, ST( S_COLOQUE_LA_LLAVE_EN__S ), cmodo[n] );
			MENSAJE( mens );
        }
        while( pos != n ) {
            SLEEP( 500 );
			//getData( ACTION_KEYLOCK_WAIT_POSITION, ( char* )&n, 0 );
            pos = getData( ACTION_KEYLOCK_GET_POSITION, ( char* )&n, 0 );
		}
        BORRAR_MENSAJE(  );
	}else{
	
		 if( TIPO_LLAVE == LLAVE_KB78 ) {
		if( _LLAVE != modo ) {
			switch ( modo ) {
			case 'O':
				n = 0;
				break;
			case 'P':
				n = 1;
				break;
			case 'R':
				n = 2;
				break;
			case 'S':
				n = 3;
				break;
			case 'X':
				n = 4;
				modo = 'X';
				break;
			case 'Z':
				n = 5;
				modo = 'Z';
				break;
			}
			sprintf( mens, ST( S_COLOQUE_LA_LLAVE_EN__S ), cmodo4[n]/*cmodo3[n]*/ );

			MENSAJE( mens );

			while ( k != 27 && _LLAVE != modo ) {
				k = GETCH(  );
				switch( k ) {
               case 1:
                  _LLAVE = 'L';
                  break;
               case 2:
                  _LLAVE = 'R' ;
                  break;
               case 3:
                  _LLAVE = 'S';
                  break;
			   case 4:
                  _LLAVE = 'X';
                  break;
			   case 5:
                  _LLAVE = 'Z';
                  break;
			   }
         } 

			if( _LLAVE != modo ){
				pos = 0;
         }
         _LLAVE = 'O';
			BORRAR_MENSAJE(  );
		}
	}

	}
	if(  TIPO_LLAVE == LLAVE_KB78 && pos == 0 && k == 27 ) //si presiono escape no se autoriza
		return (0);
	return( 1 );
}
