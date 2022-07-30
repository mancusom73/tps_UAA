#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <abort.h> 
#include <archivos.h>
#include <btrv.h> 
#include <cio.h> 
#include <cr.h> 
#include <dbmem.h> 
#include <dbrouter.h> 
#include <dd.h> 
#include <get_fech.h> 
#include <lista.h> 
#include <log.h> 
#include <l_valid.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <tkt.h> 
#include <trim.h> 
#include <_cr1.h> 
#include <cmemory.h>
#include <aarch.h>
#include <path.h>
#ifdef INVEL_W
#include <io.h>
#else
#endif
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W) || defined(INVEL_L)
#include <search.h>
#include <actions.h>
#endif
#ifndef INVEL_L
#include <dos.h>
#else
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/io.h>
#endif
int f_cmp_busca_tarjeta_cab1( const void *_p1, const void *_p2 );
int f_cmp_busca_tarjeta_cab2( const void *_p1, const void *_p2 );
int f_cmp_busca_tarjeta_lista( const void *_p1, const void *_p2 );
#define INTENTOS 3
#define SENAL_RANGO_INICIAL 0x80
#define SENAL_RANGO_FINAL   0x40
#define TAMANO_BUFFER_NUM_TARJ 10000
struct _cab
{
    unsigned long codigo;
};
struct ind_lista
{
    unsigned long codigo;
    unsigned long offset;
};
struct num_lista
{
    unsigned long numero;
    unsigned char status;
};
struct _datos_lista
{
    char *cabecera;
    short int handle_cab;
    short int cantidad_reg_cab;
    short int handle_lista;
};
struct _tarj_datos
{
    unsigned short nro_boletin;
    unsigned short fecha_vto;
};

struct _datos_recorrer
{
    short int ok;
//(Nota Unific MySQL-Linux): se deja "cod" con tipo long. En 01.002 esta con tipo double
    double cod;
    short int tarjeta;
    float coef;
    //marca de tarjeta ingresada por el cajero para swiss medical 
    short int tarj_ingresada;
};

static struct _tarj_datos TARJ_DATOS[CANT_TARJETAS];
static struct _datos_lista DATOS_LISTA[CANT_TARJETAS];
static char boletines_inicializados;
int RECORRER_RANGOS( struct _rangos *rangos, struct _datos_recorrer *datos );
void LISTA_CARGA_INDICE( char nro );
/*****************************************************************************/
void INICIALIZAR_TARJETAS()
/*****************************************************************************/
{
    if( !DBMEM_LOAD_SQL( _RANGOS_SIC ,T_RANGOS, TT_ORIG , "COD_DESDE") ) {
        ABORT( ST( S_ERROR_AL_CARGAR_RANGOS ) );
    }
    else {
        if( !CARGAR_TARJETAS() ) {
            ABORT( ST( S_ERROR_AL_CARGAR_TARJETAS ) );
        }
    }
}
/*****************************************************************************/
void INICIALIZAR_BUSCA_TARJETA()
/*****************************************************************************/
{
    int i;
    for( i = 0;i < CANT_TARJETAS;i++ ) {
        if( NUMERO_ARCHIVO( i ) ) {
            if( DATOS_LISTA[NUMERO_ARCHIVO( i ) - 1].handle_cab == 0 ) {
                LISTA_CARGA_INDICE( ( char )NUMERO_ARCHIVO( i ) );
            }
        }
    }
    boletines_inicializados = 1;
}
/*****************************************************************************/
void FIN_BUSCA_TARJETA()
/*****************************************************************************/
{
    int i;
    for( i = 0;i < CANT_TARJETAS;i++ ) {
        if( DATOS_LISTA[i].handle_cab ) {
            _CLOSE( DATOS_LISTA[i].handle_cab, __LINE__, __FILE__ );
            DATOS_LISTA[i].handle_cab = 0;
        }
        if( DATOS_LISTA[i].handle_lista ) {
            _CLOSE( DATOS_LISTA[i].handle_lista, __LINE__, __FILE__ );
        }
        if( DATOS_LISTA[i].cabecera ) {
            free( DATOS_LISTA[i].cabecera );
        }
    }
}
/*****************************************************************************/
void LISTA_CARGA_INDICE( char nro )
/*****************************************************************************/
{
    char nombre[] = {"LISTA1.000"};
    int registros;
    int i,error;
    long tamano;
    /*--- Construyo el nombre del archivo de indices a cargar y lo abro ----*/
    nombre[8] = '0' + ( nro / 10 );
    nombre[9] = '0' + ( nro % 10 );
    nombre[5] = '1';
    nro--;
    NRO_ARCHIVO = ARCH_LISTA1_001;
    #ifdef INVEL_L
    DATOS_LISTA[nro].handle_cab = _OPEN( nombre, O_RDWR, __LINE__, __FILE__ );
    #else
    DATOS_LISTA[nro].handle_cab = _OPEN( nombre, O_BINARY, __LINE__, __FILE__ );
    #endif
    if( DATOS_LISTA[nro].handle_cab > 0 ) {
        /*--- lee la fecha de vto y el N del boletin ----*/
        error = _READ( DATOS_LISTA[nro].handle_cab, ( char* )( &TARJ_DATOS[nro] ),
                       sizeof( struct _tarj_datos ), __LINE__, __FILE__ );
        if( !error ) {
            /*--- Obtengo la cantidad de registros de indices a guardar en memoria ---*/
            tamano = filelength( DATOS_LISTA[nro].handle_cab ) - sizeof( struct _tarj_datos );
            registros = ( int )( tamano / 512 );
            if( tamano % 512 ) {
                registros++;
            }
            /*--- Se pide memoria para la cantidad de registros de indices ---*/
            DATOS_LISTA[nro].cantidad_reg_cab = registros;
            DATOS_LISTA[nro].cabecera = calloc( registros + 1, sizeof( struct _cab ) );//malloc( ( registros + 1 ) * sizeof( struct _cab ) );
            if( !DATOS_LISTA[nro].cabecera ) {
                ABORT( ST( S_NO_HAY_MEMORIA_PARA_CARGAR_BOLETINES_PROTECTIVOS ) );
            }
            else {
                /*--- Se carga 1 registro de indice cada 512 bytes ---*/
                NRO_ARCHIVO = ARCH_LISTA1_001;
                for( i = 0, error = 0;i < registros && !error;i++ ) {
                    error = LSEEK( DATOS_LISTA[nro].handle_cab,
                                   ( long )i * 512 + sizeof( struct _tarj_datos ), SEEK_SET,
                                   __LINE__, __FILE__ );
                    if( !error ) {
                        error = _READ( DATOS_LISTA[nro].handle_cab,
                                       &DATOS_LISTA[nro].cabecera[i * 4], 4, __LINE__, __FILE__ );
                    }
                }
                if( error ) {
                    ABORT( ST( S_ERROR_AL_CARGAR_LISTA1 ) );
                }
                else {
                    /*--- Marca de final de registros de indices ---*/
                    memset( &DATOS_LISTA[nro].cabecera[registros * 4], 0xff, 4 );
                    /*--- Abrimos el archivo de los nmeros para dicha tarjeta ---*/
                    nombre[5] = '2';
                    NRO_ARCHIVO = ARCH_LISTA2_001;
                    #ifdef INVEL_L
                    if( ( DATOS_LISTA[nro].handle_lista
                        = _OPEN( nombre, O_RDWR, __LINE__, __FILE__ ) ) < 0 ) {
                        ABORT( ST( S_ERROR_AL_CARGAR_LISTA2 ) );
                    }
                    #else
                    if( ( DATOS_LISTA[nro].handle_lista
                        = _OPEN( nombre, O_BINARY, __LINE__, __FILE__ ) ) < 0 ) {
                        ABORT( ST( S_ERROR_AL_CARGAR_LISTA2 ) );
                    }
                    #endif
                }
            }
        }
    }
}
/*****************************************************************************/
int VALIDA_NUMERO_TARJETA( char *num_tarjeta, char marca )
/*****************************************************************************/
{
    int rta,f_valid;
    f_valid = TARJ[marca].nro_funcion_valid;
    // Esto debiera ser configurable
    /* if( marca == _CREDENCIAL_VIEJA && num_tarjeta[6] == '7' ) { f_valid =
     * 1; } */
    rta = EJECUTAR_L_VALID( num_tarjeta, f_valid );
    return ( rta );
}
/*****************************************************************************/
int VALIDA_BOLETIN( char nro )
/*****************************************************************************/
{
    int rta = 1;
    if( MODO_NEGOCIO != SERVICIOS ) {
        if((nro - 1)>=0 && TARJ_DATOS[nro - 1].fecha_vto < GET_FECHA_DOS() ) {
            rta = 0;
        }
    }
    return ( rta );
}
/*****************************************************************************/
int BUSCA_TARJETA( char *nro_tarjeta, char cod_marca )
/*****************************************************************************/
{
    char *p,nro,_codigo[17],salir,seguir;
    struct num_lista *p_num_lista,*p_num_lista_sig,primer;
    int rta = 0, registros, offset_nro, error = 0, cant_bloques, bloque, i, cant_reg;
    unsigned bytes,tam_buffer;
    long tamano,offset,bytes_bloque;
    unsigned long rango_cod_alto,rango_cod_bajo,cod_alto,cod_bajo;
    struct ind_lista *p_cab_lista,*p_cab_lista_sig;
    char buf_cab[520],*buffer;
    /*---------- Obtengo los codigos alto y bajo y N de archivo ----------*/
    nro = TARJ[cod_marca].nro_archivo - 1;
    offset_nro = TARJ[cod_marca].offset_1_bloque - 1;
    memcpy( _codigo, &nro_tarjeta[offset_nro], TARJ[cod_marca].digitos_1_bloque );
    _codigo[TARJ[cod_marca].digitos_1_bloque] = 0;
    cod_alto = atol( _codigo );
    strncpy( _codigo, &nro_tarjeta[offset_nro + TARJ[cod_marca].digitos_1_bloque], sizeof(_codigo) );
    cod_bajo = atol( _codigo );
    /*--- Busco los 8 primeros digitos de la tarjeta en el resumen del indice ---*/
    /*--- Se busca por <= a cod_alto ---*/
    /* Sort using Quicksort algorithm: */
    qsort( ( void* )DATOS_LISTA[nro].cabecera, ( size_t )DATOS_LISTA[nro].cantidad_reg_cab,
           sizeof( unsigned long ), ( int ( * )( const 
           void*, const void* ) )f_cmp_busca_tarjeta_cab1 );
    //  p = bsearch( &cod_alto, DATOS_LISTA[nro].cabecera, DATOS_LISTA[nro].cantidad_reg_cab,
    //      4, f_cmp_busca_tarjeta_cab1 );
    p = bsearch( &cod_alto, DATOS_LISTA[nro].cabecera, DATOS_LISTA[nro].cantidad_reg_cab,
                 sizeof( unsigned long ), f_cmp_busca_tarjeta_cab1 );
    if( p ) {
        /*------------------- Cargo el bloque de indices --------------------*/
        offset = ( p - DATOS_LISTA[nro].cabecera ) * 128 + sizeof( struct _tarj_datos );
        tamano = filelength( DATOS_LISTA[nro].handle_cab );
        NRO_ARCHIVO = ARCH_LISTA1_001;
        LSEEK( DATOS_LISTA[nro].handle_cab, offset, SEEK_SET, __LINE__, __FILE__ );
        /*--- Si hay quedan menos de 9 registros de indices se pone marca de final ---*/
        /*--- y si no leemos los 9 siguientes registros ----*/
        if( tamano - 520 >= offset ) {
            error = _READ( DATOS_LISTA[nro].handle_cab, buf_cab, 520, __LINE__, __FILE__ );
            registros = 64;
        }
        else {
            error = _READ( DATOS_LISTA[nro].handle_cab, buf_cab, ( int )( tamano - offset ),
                           __LINE__, __FILE__ );
            registros = ( int )( tamano - offset ) / 8;
            memset( &buf_cab[registros * 8], 0xff, 4 );
        }
        /*--- Busco 8 primeros digitos de la tarjeta en el indice       ---*/
        /*--- Se busca por <= a cod_alto por si hay inhabilitacin por rango de N ---*/
        p_cab_lista = bsearch( &cod_alto, buf_cab, registros, sizeof( struct ind_lista ),
                               f_cmp_busca_tarjeta_cab2 );
        if( p_cab_lista && !error ) {
            /*---- leemos el primer nmero (por si es limite superior ) ----*/
            if( p_cab_lista->codigo == cod_alto ) {
                LSEEK( DATOS_LISTA[nro].handle_lista, p_cab_lista->offset, SEEK_SET, __LINE__,
                       __FILE__ );
                error = _READ( DATOS_LISTA[nro].handle_lista, ( char* )&primer, sizeof( primer ),
                               __LINE__, __FILE__ );
                if( primer.numero >= cod_bajo && ( primer.status & SENAL_RANGO_FINAL ) ) {
                    p_cab_lista--;
                }
            }
            /*--- Obtenemos el tamao del bloque del lista2 ---*/
            p_cab_lista_sig = p_cab_lista + 1;
            if( p_cab_lista_sig->codigo != 0xffffffff ) {
                bytes_bloque = p_cab_lista_sig->offset - p_cab_lista->offset;
            }
            else {
                bytes_bloque = filelength( DATOS_LISTA[nro].handle_lista ) - p_cab_lista->offset;
            }
            /*--------------- Pedimos memoria ------------------*/
            tam_buffer = BUFFER_LISTA - ( BUFFER_LISTA % sizeof( struct num_lista ) );
            buffer = malloc( tam_buffer + 5 );
            if( !buffer ) {
                MENSAJE_TECLA_STRING( S_NO_HAY_MEMORIA_PARA_TARJETAS_PRESIONE_ESC );
                return ( 2 );
            }
            /*----- Calculamos la cantidad de bloques que ocupan los N ----*/
            cant_bloques = ( int )( bytes_bloque / tam_buffer );
            if( bytes_bloque % tam_buffer ) {
                cant_bloques++;
            }
            for( bloque = 0, seguir = 1;bloque < cant_bloques && !error && seguir;bloque++ ) {
                /*------------------ Leemos un bloque ----------------*/
                NRO_ARCHIVO = ARCH_LISTA2_001;
                LSEEK( DATOS_LISTA[nro].handle_lista,
                       p_cab_lista->offset + ( ( long )bloque * tam_buffer ), SEEK_SET, __LINE__,
                       __FILE__ );
                bytes = ( bytes_bloque - ( long )bloque * ( long )tam_buffer < ( long )tam_buffer )
                      ? ( unsigned )( bytes_bloque - ( long )bloque * ( long )tam_buffer )
                      : tam_buffer;
                error = _READ( DATOS_LISTA[nro].handle_lista, buffer, bytes, __LINE__, __FILE__ );
                memset( &buffer[bytes], 0xff, 4 );
                p_num_lista = ( struct num_lista * )buffer;
                cant_reg = bytes / sizeof( struct num_lista );
                for( i = 0, seguir = 1;i < cant_reg && seguir;i++ ) {
                    if( p_num_lista->numero == cod_bajo ) {
                        seguir = 0;
                    }
                    else {
                        if( p_num_lista->numero < cod_bajo
                         && ( p_num_lista->status & SENAL_RANGO_INICIAL ) ) {
                            p_num_lista_sig = p_num_lista + 1;
                            if( p_num_lista_sig->numero >= cod_bajo ) {
                                seguir = 0;
                            }
                            else {
                                p_num_lista++;
                            }
                        }
                        else {
                            p_num_lista++;
                        }
                    }
                }
            }
            if( seguir ) {
                p_num_lista = NULL;
            }
            if( p_num_lista && !error ) {
                /*--- Verificamos si est dentro del rango o si est el nmero completo ----*/
                if( p_num_lista->status & SENAL_RANGO_INICIAL ) {
                    p_num_lista_sig = p_num_lista + 1;
                    salir = 0;
                    if( p_num_lista_sig->numero == 0xffffffff ) {
                        /*--- Buscamos el prximo codigo ---*/
                        if( p_cab_lista_sig->codigo != 0xffffffff ) {
                            rango_cod_alto = p_cab_lista_sig->codigo;
                            LSEEK( DATOS_LISTA[nro].handle_lista, p_cab_lista_sig->offset, SEEK_SET,
                                   __LINE__, __FILE__ );
                            _READ( DATOS_LISTA[nro].handle_lista, ( char* )p_num_lista_sig,
                                   sizeof( struct num_lista ), __LINE__, __FILE__ );
                        }
                        else {
                            salir = 1;
                        }
                    }
                    else {
                        rango_cod_alto = p_cab_lista->codigo;
                    }
                    rango_cod_bajo = p_num_lista_sig->numero;
                    if( p_num_lista_sig->status & SENAL_RANGO_FINAL && !salir ) {
                        if( ( cod_alto > p_cab_lista->codigo )
                         || ( cod_alto == p_cab_lista->codigo && cod_bajo >= p_num_lista->numero ) ) {
                            if( ( cod_alto < p_cab_lista_sig->codigo )
                             || ( cod_alto == rango_cod_alto && cod_bajo <= rango_cod_bajo ) ) {
                                rta = 1;
                            }
                        }
                    }
                }
                else {
                    if( cod_alto == p_cab_lista->codigo && cod_bajo == p_num_lista->numero ) {
                        rta = 1;
                    }
                }
            }
            if( buffer)
            free( buffer );
        }
    }
    if( error ) {
        rta = 2;
    }
    return ( rta );
}
/*****************************************************************************/
int f_cmp_busca_tarjeta_cab1( const void *_p1, const void *_p2 )
/*****************************************************************************/
{
    int rta;
    unsigned long *p1,*p2,*p3;
    p1 = ( unsigned long* )_p1;
    p2 = ( unsigned long* )_p2;
    if( *p1 == *p2 ) {
        rta = 0;
    }
    else {
        if( *p1 < *p2 ) {
            rta = -1;
        }
        else {
            rta = 1;
        }
    }
    if( rta > 0 ) {
        p3 = p2 + 1;
        if( *p3 > *p1 ) {
            rta = 0;
        }
    }
    return ( rta );
}
/*****************************************************************************/
int f_cmp_busca_tarjeta_cab2( const void *_p1, const void *_p2 )
/*****************************************************************************/
{
    int rta;
    struct ind_lista *p1,*p2,*p3;
    p1 = ( struct ind_lista * )_p1;
    p2 = ( struct ind_lista * )_p2;
    if( p1->codigo == p2->codigo ) {
        rta = 0;
    }
    else {
        if( p1->codigo < p2->codigo ) {
            rta = -1;
        }
        else {
            rta = 1;
        }
        if( rta > 0 ) {
            p3 = p2 + 1;
            if( p1->codigo < p3->codigo ) {
                rta = 0;
            }
        }
    }
    return ( rta );
}
/*****************************************************************************/
int f_cmp_busca_tarjeta_lista( const void *_p1, const void *_p2 )
/*****************************************************************************/
{
    int rta;
    struct num_lista *p2,*p3;
    unsigned long *p1;
    p1 = ( unsigned long* )_p1;
    p2 = ( struct num_lista * )_p2;
    if( *p1 == p2->numero ) {
        rta = 0;
    }
    else {
        if( *p1 < p2->numero ) {
            rta = -1;
        }
        else {
            rta = 1;
        }
        if( rta > 0 ) {
            if( p2->status & SENAL_RANGO_INICIAL ) {
                p3 = p2 + 1;
                if( *p1 < p3->numero ) {
                    rta = 0;
                }
            }
        }
        else {
            if( rta < 0 ) {
                if( p2->status & SENAL_RANGO_FINAL ) {
                    p3 = p2 - 1;
                    if( *p1 > p3->numero && !( p3->status & SENAL_RANGO_INICIAL ) ) {
                        rta = 0;
                    }
                }
            }
        }
    }
    return ( rta );
}
/*****************************************************************************/
int DEBE_PEDIR_CUPON( int marca, int cuotas, int financia_tarjeta )
/*****************************************************************************/
{
    int rta = 0;
    if( cuotas == 1 ) {
        rta = TARJ[marca].pide_numero & 0x01;
    }
    else {
        if( !financia_tarjeta ) {
            rta = TARJ[marca].pide_numero & 0x02;
        }
        else {
            rta = TARJ[marca].pide_numero & 0x04;
        }
    }
    return ( rta );
}
/*****************************************************************************/
int CARGAR_TARJETAS()
/*****************************************************************************/
{
    int h, rta = 0;
	int reg;
    struct _tarjetas tarjeta_sic;
    //  memset(TARJ, 0, sizeof(TARJ));
    TARJ = ( struct _tarjetas * )calloc( CANT_TARJETAS, sizeof( struct _tarjetas ) );//malloc( sizeof( struct _tarjetas ) * CANT_TARJETAS );
    if( TARJ ) {
        memset( TARJ, 0, sizeof( struct _tarjetas ) * CANT_TARJETAS );
        //  _tarjetas TARJ[CANT_TARJETAS];
        if( /*USE_DB( AREA_AUX9, _tarjetas, ( char* )&tarjeta_sic, sizeof( struct _tarjetas ),
                    NULL, 0, 0 )*/
			OPEN_TABLE(T_TARJETAS, TT_ORIG,( char* )&tarjeta_sic,sizeof( struct _tarjetas )) == 0 ) {
			SELECT_TABLE( T_TARJETAS, TT_ORIG );
            reg = RECCOUNT2(NULL,NULL);
            if( reg ) {
				SET_WHERE("");
				RUN_QUERY(NO);
                for( h = 0;!dbEOF();h++ ) {
                    if( tarjeta_sic.cod_tarjeta < CANT_TARJETAS ) {
                        memcpy( &TARJ[tarjeta_sic.cod_tarjeta - 1], &tarjeta_sic,
                                sizeof( struct _tarjetas ) );
                    }
                    SKIP2( 1 );
                }
                CLOSE_TABLE( T_TARJETAS, TT_ORIG );
                rta = 1;
            }
        }
    }
    return ( rta );
}
/*****************************************************************************/
int NUMERO_BOLETIN( char nro )
/*****************************************************************************/
{
    return ( TARJ_DATOS[nro - 1].nro_boletin );
}
/*****************************************************************************/
int NUMERO_ARCHIVO( int marca )
/*****************************************************************************/
{
    return ( TARJ ? TARJ[marca].nro_archivo : 0 );
}
/*****************************************************************************/
char * NOMBRE_TARJETA( int marca )
/*****************************************************************************/
{
    return ( TARJ[marca].nombre );
}
/*****************************************************************************/
float LIMITE2_TARJETA( int marca )
/*****************************************************************************/
{
    return ( TARJ[marca].limite2 );
}
/*****************************************************************************/
int TARJETA_ON_LINE( int marca )
/*****************************************************************************/
{
    int rta;
    rta = TARJ[marca].on_line;
    if( SISTEMA_MANUAL_DE_TARJETA_DIFERI && VAR_TARJETAS_PRIMER_VTO ) {
        rta = 0;
    }
    if( RAM_NOTA_CR
        && AUTORIZACION_TARJ_EN_NOTAS_CREDI == _OFFLINE ) {
        if( VAR_TARJETAS_CELDA_PINPAD == 0)  //las de debito no la podemos hacer offline
        SET_MEMORY_CHAR( __var_tarjetas_unicamente_ol, 0 ); 
    }

    return ( rta );
}
/*****************************************************************************/
int IMPRIME_MENSAJE_TARJETA( int marca )
/*****************************************************************************/
{
    return ( ( TARJ[marca].imprime_mensaje == 'S' ) ? 1 : 0 );
}
/*****************************************************************************/
int DEBE_PEDIR_COD_VIGENCIA( int marca )
/*****************************************************************************/
{
    return ( ( TARJ[marca].pide_cod_vigencia == 'S' ) ? 1 : 0 );
}
/*****************************************************************************/
int NODO_TARJETA( int marca )
/*****************************************************************************/
{
    return ( TARJ[marca].nodo );
}
/*****************************************************************************/
int COD_VALIDACION_VOUCHER( int marca )
/*****************************************************************************/
{
    int rta;
    if( VAR_TARJETAS_MONEDA == PESOS ) {
        rta = TARJ[marca].val_voucher_pesos;
    }
    else {
        rta = TARJ[marca].val_voucher_dolares;
    }
    return ( rta );
}
/****************************************************************************/
int RECORRER_RANGOS( struct _rangos *rangos, struct _datos_recorrer *datos )
/****************************************************************************/
{
    if( datos->cod >= rangos->cod_desde ) {
        if( datos->cod <= rangos->cod_hasta ) {
            datos->ok = 1;
            datos->tarjeta = rangos->cod_tarjeta - 1;
            datos->coef = rangos->coef;
            DBMEM_GO( BOTTOM );
        }
    }
    /*else {
        DBMEM_GO( BOTTOM );
    }*/
    return 0;
}
/**
/* Verifica que el numero dado este en el rango dado, y que el codigo de
/* tarjeta dado corresponda con el codigo de tarjeta asociado al rango.
/* @param rangos contiene el rango y el codigo de tarj asociado
/* @param datos contiene el numero, el codigo de tarj y campos para guardar los
/*              datos obtenidos del parametro rangos
/* @return siempre cero
/****************************************************************************/
int RECORRER_RANGOS2( struct _rangos *rangos, struct _datos_recorrer *datos )
/****************************************************************************/
{
    if( ( datos->cod >= rangos->cod_desde ) && ( datos->cod <= rangos->cod_hasta ) ){
        if ( datos->tarj_ingresada == rangos->cod_tarjeta ){
            datos->ok = 1;
            datos->tarjeta = rangos->cod_tarjeta;
            datos->coef = rangos->coef;
        }
    }
    return 0;
}
/****************************************************************************/
int BUSCA_MARCA_TARJETA( double numero, char *cod_tarjeta, float *limite )
/****************************************************************************/
{
    struct _datos_recorrer datos;
    datos.ok = 0;
    datos.cod = numero;
    DBMEM_RECORRER( _RANGOS_SIC, RECORRER_RANGOS, ( char* )&datos );
// controlemos que venga bien porque puede haber error en lectura de la rangos
	if( datos.tarjeta > 200 ) { //una marca de tarjeta deberia ser menos de 200
		GRABAR_LOG_SISTEMA( "ERROR RECORRIENDO RANGOS -SE REINTENTA" ,LOG_VENTAS,1);	
		DBMEM_RECORRER( _RANGOS_SIC, RECORRER_RANGOS, ( char* )&datos );
	}

    if( datos.ok ) {
        *cod_tarjeta = ( char )datos.tarjeta;
        /*
            02/12/2004-- fabiana -- se comenta para poder igualar la base de datos
            if( TARJ[*cod_tarjeta].tarjetas_esp_track2 == 'S'
                || TARJ[*cod_tarjeta].tarjetas_esp_track2 == 's' )
                BUSCA_MARCA_TARJETA_ESPECIAL( cod_tarjeta );
            */
        //*limite = TARJ[datos.tarjeta].limite * datos.coef;
        *limite = TARJ[*cod_tarjeta].limite * datos.coef;
    }
    return ( datos.ok );
}
/**
/* Obtiene los datos (cod tarjeta y limite) de la tarjeta determinada por 
/* cod_tarjeta y por numero.
/* @param numero comienzo del numero de tarjeta a buscar en los rangos
/* @param cod_tar_selected codigo de tarjeta a buscar
/* @param limite se devuelve el limite de la tarjeta
/* @return 1 si se encontro la tarjeta 
/****************************************************************************/
int BUSCA_MARCA_TARJETA_SERVICIOS( double numero, char *cod_tarjeta, float *limite )
/****************************************************************************/
{
    struct _datos_recorrer datos;
    datos.ok = 0;
    datos.cod = numero;
    datos.tarj_ingresada = *cod_tarjeta; /*num tarjeta dado por el cajero*/
    DBMEM_RECORRER( _RANGOS_SIC, RECORRER_RANGOS2, ( char* )&datos );
    if( datos.ok ) {
        *limite = TARJ[*cod_tarjeta].limite * datos.coef;
    }
    return ( datos.ok );
}
/****************************************************************************/
int EXISTE_TARJETA( int marca )
/****************************************************************************/
{
    int rta = 0;
    struct _tarjetas tarjeta_sic;
    char men[] = "ERROR: SE ENCONTRO RANGOS,PERO NO LA TARJETA";
    if( /*USE_DB( AREA_AUX, _tarjetas, ( char* )&tarjeta_sic, sizeof( tarjeta_sic ), NULL, 0, 0 )*/
     OPEN_TABLE(T_TARJETAS, TT_ORIG,( char* )&tarjeta_sic,sizeof( tarjeta_sic )) == 0 ) {
		 //SELECT_TABLE( T_TARJETAS, TT_ORIG );
         //SET_ORDER( 1 );
        marca++;
        //GET_EQUAL( ( char* )&marca );
		SET_WHERE("COD_TARJETA = '%d'",marca);
		RUN_QUERY(NO);
        if( FOUND2() ) {
            rta = 1;
        }
        else {
            MENSAJE_TECLA( men );
            rta = 0;
        }
        CLOSE_TABLE( T_TARJETAS, TT_ORIG );
    }
    return ( rta );
}
/*****************************************************************************/
int BOLETINES_INICIALIZADOS()
/*****************************************************************************/
{
    return ( ( int )boletines_inicializados );
}
/****************************************************************************/
void BUSCA_MARCA_TARJETA_ESPECIAL( char *cod_tarjeta )
/****************************************************************************/
{
    char aux_track2[50],aux_codigo[20];
    struct  _tar_adicional tar_track2_sic;
    int aux;
    /*if( USE_DB( AREA_AUX, _tar_adicional, ( char* )&tar_track2_sic, 0, NULL, 0,
                sizeof( tar_track2_sic ) ) == 0 ) {*/
	if( OPEN_TABLE( T_TAR_TAR2, TT_ORIG,( char* )&tar_track2_sic,
			sizeof( struct _tar_adicional) )== 0 ) {
        //SET_ORDER( 1 );
		SET_ORDER2("COD_TARJETA");
        //aux = *cod_tarjeta;
        aux = ( *cod_tarjeta ) + 1;
        /*GET_EQUAL( ( char* )&aux );*/
		SET_WHERE("COD_TARJETA = '%i' ",  aux );
		RUN_QUERY(NO);
        strcpy( aux_track2, GET_TRACK_2_PRIVADO( ) );
        while( /*!BtrvEOF()*/ !dbEOF() && ( ( int )tar_track2_sic.cod_tarjeta == aux ) ) {
            TRIM( tar_track2_sic.nro_especial );
            strncpy( aux_codigo, &aux_track2[tar_track2_sic.pto_inicio],
                     strlen( tar_track2_sic.nro_especial ) );
            aux_codigo[strlen( tar_track2_sic.nro_especial )] = 0x00;
            if( !strcmp( aux_codigo, tar_track2_sic.nro_especial ) ) {
                *cod_tarjeta = tar_track2_sic.cod_tarj_especial - 1;
                break;
            }
            //SKIP( 1 );
			SKIP2( 1 );
        }
        //CLOSE_DATABASE( AREA_AUX );
		CLOSE_TABLE( T_TAR_TAR2, TT_ORIG );
    }
}
/****************************************************************************/
int EXISTE_PREFIJO_BARRA_TARJETA( char *bin, long prefijo, int *l_tarj )
/****************************************************************************/
{
	//prefijo, es el prefijo de codigo de barra.
	//en bin, devolvemos el prefijo del bin
    int rta = 0;
    struct _tarjetas tarjeta_sic;
    if( /*USE_DB( AREA_AUX, _tarjetas, ( char* )&tarjeta_sic, sizeof( tarjeta_sic ), NULL, 0, 0 )*/
     OPEN_TABLE(T_TARJETAS, TT_ORIG,( char* )&tarjeta_sic,sizeof( tarjeta_sic )) == 0 ) {
		 SELECT_TABLE( T_TARJETAS, TT_ORIG );
        //SET_ORDER( 3 );
        //GET_EQUAL( (char *) &prefijo );
		 SET_WHERE("PREFIJO_COD_BARRA = '%ld'",prefijo);
        if( FOUND2( ) ) {
            rta = 1;
			_ltoa( tarjeta_sic.prefijo_bin, bin, 10 );
			*l_tarj = tarjeta_sic.largo1;
			//memcpy( bin, ltoa(tarjeta_sic.prefijo_bin, sizeof( tarjeta_sic.prefijo_bin ) );
		} else {
			GRABAR_LOG_SISTEMA( "No se encontro prefijo barra" , LOG_VENTAS,2 );
		}      
        CLOSE_TABLE( T_TARJETAS, TT_ORIG );
    }
    return ( rta );	
    return 0;
}

