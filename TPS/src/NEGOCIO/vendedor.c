#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cmemory.h> 
#include <cr.h> 
#include <cr_pant.h> 
#include <mstring.h> 
#include <stringt.h> 
#include <vendedor.h> 
#include <_cr1.h> 
#include <l_valid.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/*****************************************************************************/
void PEDIR_VENDEDOR()
/*****************************************************************************/
{
    long numero;
    char cnumero[10];
    numero = PEDIR_ENTERO_EN_RECUADRO( ST( S__NRO_VENDEDOR_ ), SI, 5 );
    if( numero < 0 ) {
        numero = 0;
    }
    else {
        _itoa( numero, cnumero, 10 );
        if( !EJECUTAR_L_VALID( cnumero, VALIDACION_DEL_NRO_DE_VENDEDOR ) ) {
            MENSAJE_STRING( S_NUMERO_DE_VENDEDOR_INVALIDO );
            numero = 0;
        }
    }
    //SET_MEMORY_INT(__nro_vendedor, numero);
    SET_MEMORY_LONG( __nro_vendedor_long, numero );
    MOSTRAR_VENDEDOR();
}
#ifdef COMPILAR_MOZO
/*****************************************************************************/
void PEDIR_MOZO()
/*****************************************************************************/
{
    int numero;
    numero = ( int )PEDIR_ENTERO_EN_RECUADRO( ST( S__MOZO_ ), SI, 1 );
    if( numero < 0 ) {
        numero = 0;
    }
    SET_MEMORY_INT( __nro_mozo, numero );
    MOSTRAR_MOZO();
}
#endif
/*****************************************************************************/
void PEDIR_VENDEDOR_SI_ES_NECESARIO()
/*****************************************************************************/
{
    if( !NRO_VENDEDOR_LONG && VENDEDOR_OBLIGATORIO ) {
        PEDIR_VENDEDOR();
    }
}

