#ifndef TAR_MAG_H
#define TAR_MAG_H
int LEER_TARJETA_MAGNETICA( unsigned char *track2, unsigned char *track1 );
int INIT_TARJETA_MAGNETICA();
void LEER_TARJETA_MAGNETICA_OMRON( unsigned char *track2, unsigned char *track1 );
int LECTOR_BANDA( int tecla );
int LECTOR_TARJETA_LEIDA();
void LIMPIA_BUFFER_TARJETA( int cerear );
int LEER_TARJETA_BANDA( unsigned char *buffer, unsigned char *buffer2 );
void CARGA_TRACKS_EN_RNV();
int STRLEN( char *men, int cant );
#endif
