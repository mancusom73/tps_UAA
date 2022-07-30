#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <ini.h>
#include <ssaver.h>
#include <actions.h>

static unsigned long _clock_ultima_tecla;
int ssaverOn = 0;
char ssaver_activo = 0;
int win_act_ontop = 0;
extern int WIN_ACTIVA_ONTOP( void );


/*****************************************************************************/
void INIT_SCREEN_SAVER()
/*****************************************************************************/
{
    if( !_clock_ultima_tecla ) {
        SET_TIME_SCREEN_SAVER();
    }
}
/*****************************************************************************/
void SET_TIME_SCREEN_SAVER()
/*****************************************************************************/
{
    _clock_ultima_tecla = GET_TIME();
}
/*****************************************************************************/
void VERIFICAR_SCREEN_SAVER(int *tecla)
/*****************************************************************************/
{
    if( ConfIni.timerScreenSaver &&  ssaverOn == 0 && *tecla == -999 ) {
        if( TIEMPO_PASADO_DESDE( &_clock_ultima_tecla )
          > ( unsigned long ) ( ConfIni.timerScreenSaver * 18.2 ) ) {
			ssaverOn = 1;
			KILL_ALL_VIDEOS();
			
			if( WIN_ACTIVA_ONTOP() ) {
				win_act_ontop = 1;
			}
			ExecuteAction( A_DES_TECLADO_ALFABETICO );
			ExecuteAction( A_DES_WIN_ON_TOP );
			SetCurrPanel( 0 );

			ScreenSaverOn();
			DIRECT_VIDEO_PLAY(ConfIni.dirVideosSsaver, ConfIni.vAnchoS,ConfIni.vAltoS, ConfIni.vPosxS, ConfIni.vPosyS);
        }
	}else{
		if(ConfIni.timerScreenSaver && -999 != *tecla){
			if(ssaverOn){
				KILL_ALL_VIDEOS();
				//SLEEP(300);
				ScreenSaverOff();
				SetCurrPanel( 1 );
				ssaverOn = 0;
				if( win_act_ontop ) {
					*tecla = 27;//ESC
				} else {
					*tecla = 199;//Ç
				}
				win_act_ontop = 0;
			}
			
			SET_TIME_SCREEN_SAVER();
		}
		if(ssaverOn){
			ScreenSaverOn();
		}
	}
}



