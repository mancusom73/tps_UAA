/***************************************************************************/
/* Clases para adminmistrar listas de videos y reproducirlos utilizando un */
/* reproductor externo                                                     */
/***************************************************************************/
#pragma once

#include <wx/process.h>
#include <wx/app.h>
#include <wx/event.h>
#include <wx/thread.h>
#pragma pack (push, 8)


/**
 * Almacena la informacion de un video 
 */
class videoInfo {
public:
    char nombre[50];
    int alto;
    int ancho;
    int posx;
    int posy;
};

/**
 * Declaracion un vector de punteros a objetos del tipo videoInfo
 */
WX_DEFINE_ARRAY( videoInfo *, ArrayDeVideos );

/**
 * Minima clase para administrar el acceso concurrente a la lista de videos 
 */
class VideoList {
public:
    void addVideo( videoInfo *video );
    videoInfo *getVideo();
    bool hayVideo();
private:
    ArrayDeVideos videos;
    wxMutex mutexProtecting;
};

/**
 * Thread que ejecuta en un proceso un reproductor de video, de manera de reproducir
 * los videos de la lista, en serie.
 */ 
class VideoPlayer : public wxThread {
public:
    VideoPlayer() : wxThread( wxTHREAD_DETACHED ) {};
    ~VideoPlayer(){};
    void play( char *repro, char *path, char *nombre, int posx, int posy, int ancho, int alto );
    void *Entry();
	int ExisteReproductor( );
	int VideoPlayer::ExisteVideo( char *video );
	bool VideoPlayer::HayVideos( );
/*	void killAll();
	void directPlay(char *directorio);*/
private:
    wxProcess *proc;
	
};

extern "C"
{
	void DIRECT_VIDEO_PLAY(char *directorio, int ancho, int alto, int posx, int posy);
	void DIRECT_VIDEO_PLAY_DEFAULT(char *directorio);
	void KILL_ALL_VIDEOS();
	void SETEA_FOCUS_TPS( );
}

#pragma pack (pop)
