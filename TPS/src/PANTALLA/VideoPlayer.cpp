
#include "VideoPlayer.h"
#include <wx/string.h>
#include <wx/file.h>
#include <wx/utils.h>
                
//#include "actions.h"
//#include "CR.H"

/** La unica lista de videos. */
static VideoList lista_videos;

int defY, defX, defAlto, defAncho, defVentaY, defVentaX, defVentaAlto, defVentaAncho;
char defVidDir[100], defRepDir[100];
char defVidVentasDir[100], defVidSsaverDir[100];

/*--------------------- Definiciones de class VideoPlayer -------------------*/

void VideoPlayer::play( char *repro, char *path, char *nombre, int posx, int posy, int ancho, int alto ) {

    wxProcess iniciador;
    wxString comando;

    comando.Printf("%smplayer2.exe -ontop -noborder -slave -quiet -geometry %dx%d+%d+%d %s%s ", repro, ancho, alto, posx, posy, path, nombre );
    proc = iniciador.Open( comando.c_str(), wxEXEC_ASYNC );

    if( proc ) {
        proc->Redirect();
    }
	
    /* No se pudo crear el proceso... */
}

int VideoPlayer::ExisteReproductor( ) {
    wxString mLogFile;
    //wxTextFile mTextFile(pPathScript);
    int existe = 0;
    wxFile archivo;
    mLogFile.Printf("%smplayer.exe", defRepDir);
    /*if(wxFile::Exists(mLogFile)){
        if(archivo.Open(mLogFile, archivo.write_append)){
            log = 1;
        }
    }*/
	if(wxFile::Exists(mLogFile)){
		/*CREAR ARCHIVO*/
		existe = 1;
	}
	else {
		existe =0;
	}

	return existe; //0 no 1 si
}

int VideoPlayer::ExisteVideo( char *video ) {
	  wxString mLogFile;
    //wxTextFile mTextFile(pPathScript);
    int existe = 0;
    wxFile archivo;
    mLogFile.Printf("%s%s", defVidDir, video);
    /*if(wxFile::Exists(mLogFile)){
        if(archivo.Open(mLogFile, archivo.write_append)){
            log = 1;
        }
    }*/
	if(wxFile::Exists(mLogFile)){
		/*CREAR ARCHIVO*/
		existe = 1;
	}
	else {
		existe =0;
	}

	return existe; //0 no 1 si
}

bool VideoPlayer::HayVideos( ) {
	bool hay = false; 
	if(strcmp(defVidDir,"no") != 0 ||
		   strcmp(defVidVentasDir,"no") != 0 ||
		   strcmp(defVidSsaverDir,"no") != 0 ) {
		
			   hay = true;
	}
	return hay;
}



void *VideoPlayer::Entry() {

    char leido[256];
    wxInputStream *salida; 
    videoInfo *video;
    int cota, salir = 0;

    while( true && salir == 0) {

        //Mejorar el mecanismo de espera
        while ( !lista_videos.hayVideo() ) {
            Sleep( 800 );
        }
        video = lista_videos.getVideo();
			if(  strlen(video->nombre) > 1 ) {
			if( ExisteReproductor( ) && ExisteVideo(video->nombre))  {
				play( defRepDir,
					defVidDir, video->nombre,
					video->posx, video->posy, 
					video->ancho, video->alto );
        
        if( proc ) {
            salida = proc->GetInputStream();
            cota = 0;
            memset( leido, NULL, sizeof(leido) );
            /* Nos detenemos cuando haya finalizado el video */
            while( strncmp( leido, "\r\nExiting...", 12 ) != 0 && cota < 55 ) {
                Sleep( 300 );
                memset( leido, NULL, sizeof(leido) );   
                salida->Read( leido, 256 );
                cota++;
            }
        }
			} else {
				salir = 1; 
			}
			} else 
				salir = 1;
        /* Se borra el video ya que no se elimina cuando se desencola del array */
        delete( video );
			
				
    }
return  0;
}

/*------------------------ Definiciones de class VideoList ------------------*/

void VideoList::addVideo( videoInfo *video ) {
    
    mutexProtecting.Lock();
    videos.Add( video );
    mutexProtecting.Unlock();
}

videoInfo * VideoList::getVideo( ) {

    videoInfo *v_temp;
    
    mutexProtecting.Lock();
    v_temp = videos[0];
    videos.RemoveAt(0);
    mutexProtecting.Unlock();
    return v_temp;
}

bool VideoList::hayVideo() {
    bool respuesta;
    
    mutexProtecting.Lock();
    if( videos.GetCount() > 0 ) {
        respuesta = true;
    } else {
        respuesta = false;
    }
    mutexProtecting.Unlock();
    return respuesta;

}

/*---------------------------- Acceso desde C -------------------------------*/

extern "C" {
	#include <cr.h>

    void SET_VIDEO_DEFAULTS( char* repDir, char* vidDirPromos, char* vidDirVentas, char* vidDirSsaver, int ancho, int alto, int x, int y, int anchoV, int altoV, int xV, int yV ) {
        
        strncpy( defRepDir, repDir, sizeof( defRepDir ) );
        strncpy( defVidDir, vidDirPromos, sizeof( defVidDir ) );
		strncpy( defVidVentasDir, vidDirVentas, sizeof( defVidVentasDir ) );
		strncpy( defVidSsaverDir, vidDirSsaver, sizeof( defVidSsaverDir ) );
        defAncho = ancho;
        defAlto = alto;
        defX = x;
        defY = y;
        defVentaAncho = anchoV;
        defVentaAlto = altoV;
        defVentaX = xV;
        defVentaY = yV;
    }


    
    /**
     * Anexa videos a la lista
     */
    void REP_VIDEO( char* nombre, int ancho, int alto, int posx, int posy, int videoType ) {

        videoInfo *video = new videoInfo();
        strncpy( video->nombre, nombre, sizeof( video->nombre ) );
        video->nombre[ sizeof( video->nombre ) -1 ] ='\0';
        video->ancho = ( ancho < 0 ) ? defAncho : ancho;
        video->alto = ( alto < 0 ) ? defAlto : alto;
        video->posx = ( posx < 0 ) ? defX : posx;
        video->posy = ( posy < 0 ) ? defY : posy;
        lista_videos.addVideo( video );
    }


	void DIRECT_VIDEO_PLAY_DEFAULT(char *directorio){
		DIRECT_VIDEO_PLAY(directorio,defVentaAncho,defVentaAlto,defVentaX,defVentaY);
	}


	void DIRECT_VIDEO_PLAY(char *directorio, int ancho, int alto, int posx, int posy){

		
		if(strcmp(directorio,"no") != 0){
			wxProcess iniciador;
			wxProcess *procTmp;
			wxString comando;
			comando.Printf("%smplayer.exe -ontop -noborder -slave -geometry %dx%d+%d+%d -loop 0 -really-quiet %s%s ", defRepDir, ancho, alto, posx, posy, directorio, "*" );
			procTmp = iniciador.Open( comando.c_str(), wxEXEC_MAKE_GROUP_LEADER );
			

			SLEEP(1000);
			#ifdef WXDEBUG
				comando.Printf("%snircmd win activate process TPSVd.exe",defRepDir);
			#else
				comando.Printf("%snircmd win activate process TPSV.exe",defRepDir);
			#endif
			procTmp = iniciador.Open( comando.c_str(), wxEXEC_ASYNC );
		}
		
	}

	void KILL_ALL_VIDEOS(){
		if(strcmp(defVidDir,"no") != 0 ||
		   strcmp(defVidVentasDir,"no") != 0 ||
		   strcmp(defVidSsaverDir,"no") != 0 )
		   {
				wxProcess iniciador;
				wxProcess *procTmp;
				procTmp = iniciador.Open( "taskkill /IM Mplayer.exe", wxEXEC_ASYNC );
				SLEEP(500);
			}
	}

	void SETEA_FOCUS_TPS( )
	{
	#ifdef INVEL_W
		wxProcess iniciador;
		wxProcess *procTmp;
		wxString comando;
		//SLEEP(1000);
		#ifdef WXDEBUG
			comando.Printf("%snircmd win activate process TPSVd.exe",defRepDir);
		#else
			comando.Printf("%snircmd win activate process TPSV.exe",defRepDir);
		#endif
		procTmp = iniciador.Open( comando.c_str(), wxEXEC_ASYNC );
	 #endif INVEL_W
		
	}

}
