#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <clave.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <driver.h> 
#include <modo_pro.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifndef INVEL_L
#endif
#if !defined(INVEL_W) && !defined(INVEL_L)
static unsigned char _modo;
SEL selector_memoria_driver_invel;
char *puntero_memoria_real_driver_invel;
char *puntero_memoria_driver_invel;
USHORT paragraph_memoria_driver_invel;
/*
 * SEL         selector_memoria_driver_btrieve;
 * USHORT      paragraph_memoria_driver_btrieve;
 * char *puntero_memoria_driver_btrieve;
 */
/*SEL       selector_memoria2_driver_btrieve;
 * USHORT    paragraph_memoria2_driver_btrieve;
 * char     *puntero_memoria2_real_driver_btrieve;
 * char     *puntero_memoria2_driver_btrieve; */
/***************************************************************************/
INIT_MODO_PROTEGIDO()
/***************************************************************************/
{
    USHORT paragraph;
    #ifdef BLINKER
    if( DosGetMachineMode( &_modo ) != 0 ) {
        printf( "No se puede dererminar el modo de ejecucin" );
        exit( 1 );
    }
    if( DosAllocRealSeg( LONGITUD_MEMORIA_DRIVER_INVEL, &paragraph, &selector_memoria_driver_invel )
     != 0 ) {
        printf( "DosAllocRealSeg() failed\n" );
        exit( 255 );
    }
    /* cuando se termina el programa se libera auomaticamente no hace falta
     * llamar a DosFreeRealSeg(selector_memoria_driver_invel) Las  pruebas
     * fueron hechas */
    /* apunta directamente a memoria real Es la ubicacion real de memoria
     * compartida  entre el programa en modo protegido CR.EXE y el driver
     * DRV_INV.EXE
     *
     */
    paragraph_memoria_driver_invel = paragraph;
    puntero_memoria_real_driver_invel = ( char* )( ( ( long )paragraph ) << 16 );
    puntero_memoria_driver_invel = MK_FP( selector_memoria_driver_invel, 0 );
    /* puntero_memoria_driver_invel es un selector que permite acceder a
     * memoria real. No se puede acceder directamente con
     * puntero_memoria_real_driver_invel, sino que se debe hacerlo a traves
     * de selector para accederlo hay que crear el puntero.
     *
     * Ejemplo:
     *
     * Si selector_memoria_driver_invel es igual a 0x1177
     *
     * printf("%p",puntero_memoria_driver_invel);
     *
     * puntero_memoria_driver_invel va a ser igual a 1177:0000
     *
     * y la ubicacion real en memoria que puede ser cualquira
     *
     * paragraph es 14C7
     *
     * y a puntero_memoria_real_driver_invel se lo rota 16 bits para obtener la
     * direccion en modo real 14C7:0000 que como ya digimos  no puede ser
     * usada desde cr.exe sino que se le tiene que pasar al driver para que
     * sepa de donde es que tiene que poner o sacar datos. (Estos datos
     * (0x1177  ;  0x14c7) fueron sacados de la ejecucion de una prueba)
     *
     *
     *
     * se trabaja con dos areas de memoria compartidas en memoria baja con el
     * driver
     *
     * area1
     *
     * en la funcion DRIVER se aloja memoria baja y se copia una estructura para
     * que el driver pueda devolver valores y acceder al area2 La  direccion
     * del area1 se le pasa en el registro ds
     *
     * area2
     *
     * dentro de la estructura qque se pasa en el area1 es comun pasar un
     * puntero a un buffer ,este puntero debe contener la direccion de
     * memoria real del  area2, o  sea tiene que tener
     *
     * puntero_memoria_real_driver_invel
     *
     * Devuelta:  NO USAR ESTE PUNTERO DESDE CR.EXE (sino se va  a producir un
     * fault)  solo pasarlo .Para poner y sacar valores del area2 usar el
     * selector mediante el puntero.
     *
     * puntero_memoria_driver_invel
     *
     */
    /*   if (DosAllocRealSeg(LONGITUD_MEMORIA_DRIVER_BTRIEVE, &paragraph, &selector_memoria_driver_btrieve) != 0)
     * {
     * printf("DosAllocRealSeg() failed\n");
     * exit(255);
     * }
     * // espera el paragraph
     * paragraph_memoria_driver_btrieve =paragraph;
     * puntero_memoria_driver_btrieve=MK_FP(selector_memoria_driver_btrieve,0);
     */
    /* if (DosAllocRealSeg(LONGITUD_MEMORIA2_DRIVER_BTRIEVE, &paragraph,
     * &selector_memoria2_driver_btrieve) != 0) { printf("DosAllocRealSeg()
     * failed\n"); exit(255); } paragraph_memoria2_driver_btrieve =paragraph;
     * puntero_memoria2_real_driver_btrieve =(char*)(((long)paragraph) <<16);
     * puntero_memoria2_driver_btrieve=MK_FP(selector_memoria2_driver_btrieve,
     * 0); */
    /* operaciones que no se deben hacer
     *
     * memcpy( , , -1);
     *
     * la longitud debe ser mayor o igual que 0
     *
     * puntero
     *
     * y puntero es nulo o esta apuntando a un area de memoria no alojada o
     * memoria convencional.
     *
     * memcpy(p2,p1,255);
     *
     * y para p1 se alojo menos que 255 este es el caso de cuando se trabaja con
     * indices se pasa un puntero (char *) &estructura_indice, pero resulta
     * que el sizeof de estructura_indice es < que 255 entonces va a dar un
     * protection fault
     *
     */
    // printf(" selector %04X
    // %4X\n",puntero_memoria2_driver_btrieve,paragraph);
    // getch();
    #else
    _modo = 0;
    #endif
    return ( _modo );
}
/***************************************************************************/
char * MK_FP_PROTEGIDO( unsigned seg, unsigned off, unsigned long l )
/***************************************************************************/
{
    char *p = NULL;
    SEL selector;
    switch( _modo ) {
        case 0:
            /*------------- modo real ---------*/
            p = MK_FP( seg, off );
            break;
        case 1:
            /*------------- modo protegido ---------*/
            #ifdef BLINKER
            if( DosMapRealSeg( seg, l, &selector ) == 0 ) {
                p = MK_FP( selector, 0 );
            }
            #endif
            break;
    }
    return ( p );
}
/***************************************************************************/
char * MK_FP_BIOS_PROTEGIDO( unsigned off )
/***************************************************************************/
{
    char *p = NULL;
    SEL selector;
    switch( _modo ) {
        case 0:
            /*------------- modo real ---------*/
            p = MK_FP( 0x0040, off );
            break;
        case 1:
            /*------------- modo protegido ---------*/
            #ifdef BLINKER
            if( DosGetBIOSSeg( &selector ) == 0 ) {
                p = MK_FP( selector, off );
            }
            #endif
            break;
    }
    return ( p );
}
/***************************************************************************/
void GENERA_PROTECTION_FAULT()
/***************************************************************************/
{
    unsigned long *t = ( unsigned long * )0x46c, t_inicial;
    t_inicial = *t;
}
/***************************************************************************/
LIBERA_MEMORIA_CONVENCIONAL()
/***************************************************************************/
{
    if( DosFreeRealSeg( selector_memoria_driver_invel ) != 0 ) {
        // error al liberar
    }
}
#endif

