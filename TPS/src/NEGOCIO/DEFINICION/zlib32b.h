///////////////////////////////////
//  cabecera del interface C/C++
//  de la Dll   ZLib32b
///////////////////////////////////
#ifdef INVEL_W
#ifndef _H_B3ZIP_DLL_
#define  _H_B3ZIP_DLL_  //impide reentrada

#ifndef PUBLIC
#ifdef __cplusplus
#define PUBLIC(tipo)  extern "C" tipo WINAPI 
#else   //_cplusplus
#define PUBLIC(tipo)  tipo WINAPI 
#endif  //_cplusplus
#endif //PUBLIC

////////////////////////////////////////////////////////////
// FUNCIONES DE USO GENERAL
////////////////////////////////////////////////////////////
// calculo del CRC de 32 bits
PUBLIC( LONG ) GetCrc32( LONG crc, LPVOID buffer, LONG len );
// obtener/liberar un bloque de memoria
PUBLIC( void * ) AllocBuffer( long size );
PUBLIC( void ) FreeBuffer( void*dir );

////////////////////////////////////////////////////////////
//   FUNCIONES PARA COMPRIMIR/DESCOMPRIMIR FICHEROS
////////////////////////////////////////////////////////////
// zip/unzip ficheros (compatible gzip)
PUBLIC( char * ) Gzip( char*filename );  //devuelve txterror (o NULL)
PUBLIC( char * ) Gunzip( char*filename );//devuelve txterror (o NULL)

////////////////////////////////////////////////////////////
//  COMPRIMIR/DESCOMPRIMIR FICHEROS en un TRHEAD aparte
////////////////////////////////////////////////////////////
// zip/unzip ficheros (compatible gzip)
PUBLIC( HANDLE ) ThreadGzip( char*filename );   //devuelve handle del thread
PUBLIC( HANDLE ) ThreadGunzip( char*filename ); //devuelve handle del thread  

////////////////////////////////////////////////////////////
// COMPRIMIR/DESCOMPRIMIR GRANDES BLOQUES DE MEMORIA 
////////////////////////////////////////////////////////////
//la apliacion reserva buffers de entrada/salida
struct GZIP2BUFFERS
{
    char *buffer_in;
    long size_in;
    char *buffer_out;
    long size_out;
    long numerror;
};

PUBLIC( int ) BufferGzip( struct GZIP2BUFFERS * lp );  //comprime
PUBLIC( int ) BufferGunzip( struct GZIP2BUFFERS * lp );//descomprime


////////////////////////////////////////////////////////////
//     CANALES DE COMPRESION/DESCOMPRESION
////////////////////////////////////////////////////////////
#ifndef ZHANDLE
#define ZHANDLE  HANDLE
#endif  //ZHANDLE
//////////////////////////////////////////////////////////////////////////////////////////////////////
// typedef de la funcion callback para recibir notificaciones de la DLL
typedef void (WINAPI *ZipEventProc)( void *usuario, ZHANDLE handle, UINT msg, void *buffer,
                                     long size );
// notificaciones  ==> switch(msg) ...
#define ZEVENT_ZIP        0x1001
#define ZEVENT_UNZIP      0x1002
#define ZEVENT_ENDUNZIP   0x1003

////////// funciones para comprimir/descomprimir datos en memoria
PUBLIC( ZHANDLE ) ZipInit( ZipEventProc lpfn, void*usuario );
PUBLIC( BOOL )    Zip( ZHANDLE, void*buffer, long num );
PUBLIC( BOOL )    ZipEnd( ZHANDLE );
PUBLIC( ZHANDLE ) UnzipInit( ZipEventProc lpfn, void*usuario );
PUBLIC( BOOL )    Unzip( ZHANDLE, void*buffer, long num );
PUBLIC( BOOL )    UnzipEnd( ZHANDLE );


// valores devueltos por GetStatus
#define ZSTD_NOOP  0
#define ZSTD_ZIP   1
#define ZSTD_UNZIP 2

// valores devueltos por GetZError

#define ZERR_OK            0
#define ZERR_STREAM_END    1
#define ZERR_NEED_DICT     2
#define ZERR_ERRNO         (-1)     //error stdlib
#define ZERR_STREAM_ERROR  (-2)    //datos suministrados ilegales
#define ZERR_DATA_ERROR    (-3)     //idem 
#define ZERR_MEM_ERROR     (-4)    //falta memoria
#define ZERR_BUF_ERROR     (-5)
#define ZERR_VERSION_ERROR (-6)    //errror interno version
#define ZERR_INIT_ERROR    (-7)    //se ha llamado a init a destiempo   

///  aplicables a compresion y descompresion
PUBLIC( long ) GetZError( ZHANDLE p );
PUBLIC( long ) GetZStatus( ZHANDLE p );
PUBLIC( long ) GetTotalIn( ZHANDLE p );
PUBLIC( long ) GetTotalOut( ZHANDLE p );


////////////////////////////////////////////////////////////

#endif // _H_B3ZIP_DLL_
#endif

