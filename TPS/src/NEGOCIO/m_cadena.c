#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <config.h>
#include <cr.h> 
#include <m_cadena.h> 
#include <pant.h> 
#include <print.h> 
#include <trim.h> 
#include <cmemory.h>
#include <bridge.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#endif
extern int calcHojas;

extern void GRABAR_VOUCHER_PRN( char *cadena );
extern char _COPIA_VOUCHER_PRN;
extern void IMPRIMIR_TEXTO_NO_FISCAL(int station,int tipo, char *texto, int salto_fila );

#ifdef DEBUG_IMPRESION	
extern void IMPRIME_AUX(int x,char *cadena);
#endif
/******************************************************************************/
void MOSTRAR_CADENA( int x, int y, char *cadena )
/******************************************************************************/
{
    if( y == 100 && *cadena != NULL ) {
        if( MODO_NEGOCIO != SERVICIOS ) {
            PRINT( ( char )x, ( char )y, cadena );
        }else{
            if( !calcHojas ) {
                PRINT( ( char )x, ( char )y, cadena );
            }
		}
#ifdef DEBUG_IMPRESION	
		IMPRIME_AUX( x,cadena);
#endif		
	}
}
/******************************************************************************/
void FORMAT_CADENA( char *picture, char *cadena_in, char *cadena_out )
/******************************************************************************/
{
    int i = 0, j = 0, l_pic, l_in;
    l_in = strlen( cadena_in );
    l_pic = strlen( picture );
    if( l_pic > l_in ) {
        i = l_pic - l_in;
        memset( cadena_out, ' ', i );
    }
    if( l_pic < l_in ) {
        j = l_in - l_pic;
        memcpy( cadena_out, cadena_in, j );
    }
    for( ;i < ( int )strlen( picture ) ;i++, j++ ) {
        switch( picture[i] ) {
            case 'X':
                cadena_out[j] = cadena_in[j];
                break;
            default:
                cadena_out[j] = picture[i];
                break;
        }
    }
    cadena_out[j] = 0;
}
/** @largo es el largo de la cadena a mostrar. Incluye la cadena original 
  * mas (largo - cadena) espacios blancos a la izq.
  */
/******************************************************************************/
void MOSTRAR_CADENA_ALD( int x, int y, char *cadena, int largo ) 
/******************************************************************************/
{
    char strAlin[100] = {""};
    int espacios = 0;
    LTRIM( cadena );
    espacios = largo - strlen( cadena );
    if( ( largo <= 100 ) && ( espacios >= 0 ) ) {
        memset( strAlin, ' ', espacios );
        strcpy( strAlin + espacios, cadena );
    }
    MOSTRAR_CADENA( x, y, strAlin );
}
//PARA LA VERSION MYSQL ==>>funcion agrupa la impresion fiscal y no fiscal
/**************************************************************************************/
void _MOSTRAR_CADENA( int x, int y, char *cadena ){
/**************************************************************************************/
int i = 0;

	if ( ( y >= 50 )/* && IMPRESORA_FISCAL == FISCAL*/ ){
		char *posiciones = NULL;
		
		if( x < 0 && x > 450)
			x = 1;

		posiciones = ( char* )MALLOC( sizeof( char ) *( x + 1 ));

		if( posiciones != NULL ){
			memset( posiciones, 0x00, x+1);
			memset(posiciones,' ',x );
			
			strcat(posiciones, cadena);//corri la cadena como indica x 
		}else
			posiciones = &cadena[0];//sin espacios, no habia memoria
		
		if( TIPO_DE_LETRA < LETRA_NORMAL || TIPO_DE_LETRA > LETRA_SUBRAYADO_NEGRITA )
			TIPO_DE_LETRA = LETRA_NORMAL ;
		
		GRABAR_LOG_BTRIEVE(posiciones);
		//IMPRIMIR_TEXTO_NO_FISCAL( 1 , TIPO_DE_LETRA , posiciones);
		//salvo indicacion de lo contrario, queda seteada la letra normal. si se desea otra setearla cada vez.
		TIPO_DE_LETRA = LETRA_NORMAL ;

		if( posiciones != NULL )
			free(( char * )posiciones);

	}else{
		MOSTRAR_CADENA( x, y, cadena );
	}
}

