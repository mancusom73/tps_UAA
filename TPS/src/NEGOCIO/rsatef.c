#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <dbrouter.h> 
#include <malloc.h> 
#include <aarch.h>
#include <rsaref.h>
/******************************************************************************
Sistema : TPV 1000 II
Mdulo  : Manejo de encriptacion RSA
******************************************************************************/
#pragma warning (disable:4103)  //aviso de pragma pack en cr.h
#ifdef COMPILAR_ENCRIPTACION
static R_RANDOM_STRUCT *randomStruct;
static R_RSA_PUBLIC_KEY *PUBLIC_KEY;
static R_RSA_PRIVATE_KEY *PRIVATE_KEY;
static int cant_claves;
static struct _clave_tef *claves_d;

/****************************************************************************/
int RSA_INIT( void )
/****************************************************************************/
{
    int ok = 0;
    //struct _bloque_rsa *bloque_rsa;
    static unsigned char seedByte = 0;
    unsigned int bytesNeeded;
    randomStruct = MALLOC( sizeof( R_RANDOM_STRUCT ) );
    PUBLIC_KEY = MALLOC( sizeof( R_RSA_PUBLIC_KEY ) );
    if( randomStruct && PUBLIC_KEY ) {
        R_RandomInit( randomStruct );
        while( 1 ) {
            R_GetRandomBytesNeeded( &bytesNeeded, randomStruct );
            if( bytesNeeded == 0 ) {
                break;
            }
            R_RandomUpdate( randomStruct, &seedByte, 1 );
        }
        memcpy( PUBLIC_KEY, clave_tef->bloque, sizeof( R_RSA_PUBLIC_KEY ) );
        ok = 1;
    }
    return( ok );
}
/****************************************************************************/
int RSA_ENCRIPTAR( char *buffer_in, int len_in, char *buffer_out, int *len_out )
/****************************************************************************/
{
    int ok = 0, cant_bloques, i, _len_in, _len_out, off_in, off_out, st = 0;
    int bytes_por_bloque,len_out_orig;
	if( PUBLIC_KEY ){
	    bytes_por_bloque = ( ( ( PUBLIC_KEY->bits ) / 8 ) - 20 );
	    cant_bloques = len_in / bytes_por_bloque;
	    if( len_in % bytes_por_bloque ) {
	        cant_bloques++;
	    }
	    off_in = off_out = 0;
	    _len_out = *len_out;
	    len_out_orig = *len_out;
	    *len_out = 0;
	    for( i = 0 ;i < cant_bloques && st == 0;i++ ) {
	        if( i == cant_bloques - 1 ) {
	            _len_in = len_in - ( i * bytes_por_bloque );
	        }
	        else {
	            _len_in = bytes_por_bloque;
	        } 
	        st = RSAPublicEncrypt( &buffer_out[off_out], &_len_out, &buffer_in[off_in], _len_in,
	                               PUBLIC_KEY, randomStruct );
	        off_in += _len_in;
	        ( *len_out ) += _len_out;
	        off_out += _len_out;
	        _len_out = len_out_orig - off_out;
	    }
	    if( st == 0 ) {
	        ok = 1;
	    }
	}
    return( ok );
}
/****************************************************************************/
int RSA_DESENCRIPTAR( char  *bloque_in, int len_in , char *bloque_out, int *len_out )
/****************************************************************************/
{
	int ok = 0, ln_in = 0,ln_out = 0;


   if( PRIVATE_KEY ){
      ln_in = len_in;
      ln_out = *len_out;
	  ok = _RSAPrivateDecrypt( &bloque_out[0], &ln_out, bloque_in, ln_in, PRIVATE_KEY);

   }

   return( ok );

}
/****************************************************************************/
int DESACTIVAR_PUBLIC_KEY( void ){
/****************************************************************************/
   free(randomStruct );
   free(PUBLIC_KEY   );
   glog("FREE.pbl.k", LOG_VARIOS,4);
   return 0;
}
/****************************************************************************/
int DESACTIVAR_PRIVATE_KEY( void ){
/****************************************************************************/
   glog("des.pr.k", LOG_VARIOS,4);
   //FREE( PRIVATE_KEY );
   if( PRIVATE_KEY ){
      glog("FREE.pr.1", LOG_VARIOS,4);
		free( PRIVATE_KEY );
	}
	PRIVATE_KEY = NULL;
   return 0;
}
/****************************************************************************/
int ACTIVAR_PRIVATE_KEY( void )
/****************************************************************************/
{
	int   ok = 0, handle;
   long  len;

   //--- Lee el archivo de claves
   //posiblemente se pueda implementar que lea al mismo
   //tiempo del archivo ambas claves en una sola funcion,
   //se siguio el  proceso que se encontraba establecido
   //para no modificar lo existente

   PRIVATE_KEY   = MALLOC( sizeof( R_RSA_PRIVATE_KEY ) );

   if( PRIVATE_KEY ){
      handle = OPEN( _KEY_POS_DAT, O_RDONLY );
      if(handle > 0 ){
         len = filelength( handle );
         cant_claves = len / sizeof( struct _clave_tef );
         claves_d = ( struct  _clave_tef *)MALLOC( (unsigned)len );

         if( claves_d ) {
             read( handle, claves_d, (unsigned)len );
	          memcpy( PRIVATE_KEY, claves_d->bloque, sizeof( R_RSA_PRIVATE_KEY) );
             //memcpy( &PRIVATE_KEY, claves_d->bloque, sizeof( R_RSA_PRIVATE_KEY) );
             if( PRIVATE_KEY && PRIVATE_KEY->bits != 0 ){//puede dar 0 y genera error al usar la llave
               glog("act.pr.1", LOG_VARIOS,4);
			   ok = 1;
			 }else{
				 glog("Nact.pr.1", LOG_VARIOS,4);
				 ok = 0;
			 }
            glog("prv.3", LOG_VARIOS,4);

             
          }else
            DESACTIVAR_PRIVATE_KEY( );
		  _CLOSE( handle, __LINE__, __FILE__ );
//          close( handle );
          free(claves_d);
      }else
         DESACTIVAR_PRIVATE_KEY( );
   }
   return( ok );
}
#endif

