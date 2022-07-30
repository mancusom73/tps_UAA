#pragma pack(1)
#include <deftables.h>
#include <db.h>
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <archivos.h> 
#include <btrv.h> 
#include <cmemory.h>
#include <cio.h> 
#include <cr.h> 
#include <day.h> 
#include <dbrouter.h> 
#include <get_dow.h> 
#include <log.h> 
#include <malloc.h> 
#include <tkt.h> 
#include <trim.h> 
#include <path.h>
#include <str.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
struct
{
    BINARY status;
    short int cod_archivo;
    char alias[21];
    char nom_fisico[30];
    short int cod_carpeta;
    char tipo;                               /* 0 -> btr  1 -> mem  2 -> dat */
    //char reserva[90];
} archivos_sic;                                   // 130
/*
 * INDICES
 * -------
 * 0 -> cod_carpeta + cod_archivo
 * 1 -> cod_carpeta
 * 2 -> cod_archivo
 * 3 -> alias
 */
struct
{
    BINARY status;
    short int cod_carpeta;
    char nom_carpeta[16];
    char path[51];
    char local;                              /* 0 -> NO  1 -> SI */
    //char reserva[99];
} carpetas_sic;                                   // 170
/*
 * INDICES
 * -------
 * 0 -> cod_carpeta
 * 1 -> nom_carpeta
 */
struct _archivos
{
    char nom_fisico[30];
    short int cod_carpeta;
    char tipo;
    char area;
};
struct _carpetas
{
    char path[51];
    char local;
};
static struct _archivos *archivos;
static struct _carpetas *carpetas;
static int cant_carpetas,cant_archivos;
extern void *MALLOC_CLEAR( unsigned memoria );
/******************************************************************************/
int INIT_ARCHIVOS()
/******************************************************************************/
{
    int ok = 0, cod;
    char *s;
    /*if( USE_DATABASE( AREA_AUX, CARPETAS_SIC, ( char* )&carpetas_sic, sizeof( carpetas_sic ), NULL,
                      0, 0 ) == 0 ) {*/
    if( OPEN_TABLE( T_CARPETAS, TT_ORIG, ( char* )&carpetas_sic, sizeof( carpetas_sic ) ) == 0 ){
        /*
        if( USE_DATABASE( AREA_AUX2, ARCHIVOS_SIC, ( char* )&archivos_sic, sizeof( archivos_sic ),
                          NULL, 0, 0 ) == 0 ) {*/
        if( OPEN_TABLE( T_ARCHIVOS, TT_ORIG, ( char* )&archivos_sic, sizeof( archivos_sic ) ) == 0 ){
            //SET_ORDER( 3 );
            /*--------------- Carga carpetas ----------------*/
            SELECT_TABLE( T_CARPETAS, TT_ORIG );
			SET_WHERE("");
			RUN_QUERY(NO);
            GO2( BOTTOM );
            cant_carpetas = carpetas_sic.cod_carpeta + 1;
            carpetas = (struct _carpetas *)MALLOC_CLEAR( cant_carpetas * sizeof( struct _carpetas ) );
            if( carpetas ) {
                GO2( TOP );
                while( !dbEOF() ) {
                    cod = carpetas_sic.cod_carpeta;
                    strncpy( carpetas[cod].path, carpetas_sic.path, sizeof( carpetas[cod].path ) );
                    carpetas[cod].local = carpetas_sic.local;
                    TRIM( carpetas[cod].path );
                    SKIP2( 1 );
                }
            }
            /*--------------- Carga archivos ----------------*/
            SELECT_TABLE( T_ARCHIVOS, TT_ORIG );
			SET_WHERE("");
			RUN_QUERY(NO);
            GO2( BOTTOM );
            cant_archivos = archivos_sic.cod_archivo + 1;
            archivos = (struct _archivos *)MALLOC_CLEAR( cant_archivos * sizeof( struct _archivos ) );
            if( archivos ) {
                GO2( TOP );
                while( !dbEOF() ) {
                    cod = archivos_sic.cod_archivo;
                    strncpy( archivos[cod].nom_fisico, archivos_sic.nom_fisico, sizeof( archivos[cod].nom_fisico ) );
                    archivos[cod].cod_carpeta = archivos_sic.cod_carpeta;
                    archivos[cod].tipo = archivos_sic.tipo;
                    TRIM( archivos[cod].nom_fisico );
                    /*----------------- Agrega nro de caja ---------------------*/
                    s = strchr( archivos[cod].nom_fisico, '?' );
                    if( s ) {
                        memmove( s, &s[2], strlen( s ) + 1 );
                        memcpy( s, NRO_NOM_CAJA, 3 );
                    }
                    SKIP2( 1 );
                }
            }
            if( carpetas && archivos ) {
                ok = 1;
            }
        }
    }
    //CLOSE_DATABASE( AREA_AUX2 );
    //CLOSE_DATABASE( AREA_AUX );
    CLOSE_TABLE( T_CARPETAS, TT_ORIG );
    CLOSE_TABLE( T_ARCHIVOS, TT_ORIG );
    return ( ok );
}
/******************************************************************************/
void NOM_ARCHIVO( int cod, char *destino )
/******************************************************************************/
{
    char nom[20],*s,aux[10];
	memset(nom,0,20);
	memset(aux,0,10);
    if( cod < cant_archivos ) {
        strcpy( destino, carpetas[archivos[cod].cod_carpeta].path );
        /*------------------- Procesa nombres especiales ---------------*/
        strcpy( nom, archivos[cod].nom_fisico );
        s = strchr( nom, '!' );
        if( s ) {
            CDOW( _FECHA_ARCHIVO, aux );
            *s = aux[0];
        }
        s = strchr( nom, '~' );
        if( s ) {
            CDAY( _FECHA_ARCHIVO, aux );
            memmove( &s[1], s, strlen( s ) + 1 );
            memcpy( s, aux, 2 );
        }
        #ifdef INVEL_L
        strcat( destino, nom );
        _strlwr( destino );
        #else
        strcat( destino, nom );
        #endif
    }
}

/******************************************************************************/
void NOM_CARPETA_ARCHIVO( int cod, char *destino )
/******************************************************************************/
{
    strcpy( destino, carpetas[archivos[cod].cod_carpeta].path );
}


/******************************************************************************/
int COPY( int origen, int destino )
/******************************************************************************/
{
    char nom_origen[80],nom_destino[80];
    NOM_ARCHIVO( origen, nom_origen );
    NOM_ARCHIVO( destino, nom_destino );
    return ( COPIAR_ARCHIVO( nom_origen, nom_destino, __LINE__, __FILE__ ) );
}
/******************************************************************************/
int APPEND( int origen, int destino )
/******************************************************************************/
{
    char nom_origen[80],nom_destino[80];
    NOM_ARCHIVO( origen, nom_origen );
    NOM_ARCHIVO( destino, nom_destino );
    return ( APPEND_ARCHIVO( nom_origen, nom_destino, __LINE__, __FILE__ ) );
}
/******************************************************************************/
int OPEN( int cod, int modo )
/******************************************************************************/
{
    char nombre[80];
    NRO_ARCHIVO = cod;
    NOM_ARCHIVO( cod, nombre );
    return ( _OPEN( nombre, modo, __LINE__, __FILE__ ) );
}
/******************************************************************************/
int OPEN_O_CREAT( int cod, int modo )
/******************************************************************************/
{
    char nombre[80];
    NRO_ARCHIVO = cod;
    NOM_ARCHIVO( cod, nombre );
    return ( _OPEN_O_CREAT( nombre, modo, __LINE__, __FILE__ ) );
}
//(Nota Unific MySQL-Linux): CREAT_ ya estaba renombrada (en 01.002 era CREAT) por algun problema en Linux?
/******************************************************************************/
int CREAT_( int cod, int modo )
/******************************************************************************/
{
    char nombre[80];
    NRO_ARCHIVO = cod;
    NOM_ARCHIVO( cod, nombre );
    return ( _CREAT( nombre, modo, __LINE__, __FILE__ ) );
}
/***************************************************************************/
int SETFTIME( int cod, unsigned fecha )
/***************************************************************************/
{
    char nombre[80];
    NOM_ARCHIVO( cod, nombre );
    return ( SET_FECHA_ARCHIVO( nombre, fecha ) );
}
/***************************************************************************/
int ARCHIVO_LOCAL( int cod )
/***************************************************************************/
{
    return ( carpetas[archivos[cod].cod_carpeta].local );
}
/***************************************************************************/
int COD_CARPETA( int cod )
/***************************************************************************/
{
    return ( archivos[cod].cod_carpeta );
}
/***************************************************************************/
int CARPETA_LOCAL( int cod )
/***************************************************************************/
{
    return ( carpetas[archivos[cod].cod_carpeta].local );
}

/******************************************************************************/
void LOG_EXISTE_ARCHIVO_COD( int cod )
/******************************************************************************/
{
    char nombre[80];
    NOM_ARCHIVO( cod, nombre );
    _LOG_EXISTE_ARCHIVO( nombre );
}
/******************************************************************************/
int AREA_OF( int cod )
/******************************************************************************/
{
    return ( archivos[cod].area );
}
/******************************************************************************/
int SET_AREA( int cod, int area )
/******************************************************************************/
{
    archivos[cod].area = area;
    return 0;
}

#ifdef COMPILAR_CARGAUTOMATICAITEMS
/******************************************************************************/
int LEVANTAR_ITEMS_DE_ARCHIVO( void )
/******************************************************************************/
{
   struct _item_dat{
      char  inicio;
      char  cant[6];
      char  separador;
      char  cod_barra[13];
      char  final;
	  char  final2;
	  char  final3;
   }items;
   int       handle,lugar = 0,opcion = 99;

   items.inicio='¿';
   items.separador='|';
  items.final='?';
  //items.final='\n';
   memset(items.cod_barra,0,13);

      	 /*----------- Lee el archivo -------------*/
//   if( RAM_P_TICKET )
  //    opcion =  1;//graba
   //else
      opcion =  2;//lee

//   MENSAJE_SIN_SONIDO( ST( S_POR_FAVOR_ESPERE_UN_MOMENTO_Procesando_datos ) );
   switch( opcion /*BOX_PEDIR_ENTERO( 0, 21, "0.Sale- 1.Graba-2.Lee-3.Del ( TICKET ) ", 9, SI,   54 )*/ ){
      case 1://graba ticket
      {
         unlink( "items.dat" );
         handle = _OPEN_O_CREAT( "items.dat", O_RDWR, __LINE__, __FILE__ );
         if( handle && RAM_P_TICKET ){
            for( lugar = 1 ;lugar <= ( RAM_P_TICKET ) ;lugar++ ){
               POSICIONAR_REGISTRO( handle, lugar, sizeof( struct _item_dat) );
               sprintf(items.cant,"%8.3lf", _TICKET_CANTIDAD( lugar - 1 ) );
               strcpy(items.cod_barra,  _TICKET_COD_BARRA( lugar - 1 ));
               items.cant[8]=0;
               items.cod_barra[17]=0;
               _WRITE( handle, ( char * ) &items, sizeof( struct _item_dat ), __LINE__, __FILE__ );
            }
         }/*else
            glog("Err.Itms.dt");*/

         //_CLOSE( handle, __LINE__, __FILE__ );
      }
      break;
      case 2://lee ticket
      {
         int i = 0;
		 char* barras[17];
		 memset(barras,0,17);
		// strncpy(barras[0],"7797470000045",13);
		 /*barras[0]
		 barras[1]
		 barras[2]
		 barras[3]
		 barras[4]
		 barras[5]
		 barras[6]
		 barras[7]
		 barras[8]
		 barras[9]
		 barras[10]
		 barras[11]
		 barras[12]
		 barras[13]
		 barras[14]
		 barras[15]
		 barras[16]*/


		  CEREAR_MEMORY( __ram_cod_barra );
         SET_MEMORY_DOUBLE( __ram_cantidad, 0.00);
         handle = _OPEN_O_CREAT( "items.dat", O_RDWR, __LINE__, __FILE__ );
         if( handle ){
            do{

            	_READ( handle, ( char * ) &items, sizeof( struct _item_dat ),
            	__LINE__, __FILE__ );
			   items.cod_barra[13]='\0';
               SET_MEMORY( __ram_cod_barra, items.cod_barra);
		//	   SET_MEMORY( __ram_cod_barra, barras[0]);
			   items.cant[6]='\0';
               SET_MEMORY_DOUBLE( __ram_cantidad,atof(items.cant));
               {char mensa[32];sprintf(mensa,"%s %lf",items.cod_barra, atof(items.cant));
			   glog(mensa, LOG_VARIOS, 1);}
               if(!( RAM_CANTIDAD &&  CR1_BUSCA_CODIGO( ) &&  CR1_AGREGA_ITEM( NO,_ORIGEN_TICKET, 0 ))){
                 // mmmt( "error toma datos" );
                  break;
               }
               CEREAR_MEMORY( __ram_cod_barra );
               SET_MEMORY_DOUBLE( __ram_cantidad, 0.00);
			   i++;
            }while(i< 16 /*!eof(handle)*/);

            _CLOSE( handle, __LINE__, __FILE__ );
         }else
            glog("S/dat.Itms.dt", LOG_VARIOS,1);
      }
      break;
      case 3:
         unlink( "items.dat" );
      break;
   }
//   MENSAJE_SIN_SONIDO( ST( S_PROCESO_FINALIZADO ) );
   return 1;
}
#endif