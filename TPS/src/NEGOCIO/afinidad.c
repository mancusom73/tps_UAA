#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <afinidad.h> 
#include <btrv.h> 
#include <cadafech.h> 
#include <clientes.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <getch.h> 
#include <get_fech.h> 
#include <log.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <pago_tar.h> 
#include <pant.h> 
#include <tar_mag.h> 
#include <_cr1.h> 
#include <scanner.h>
#include <db.h>
#include <deftables.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <actions.h>
void LEER_PUNTOS();
/*****************************************************************************/
void PEDIR_CLIENTE_AFINIDAD( int documento )
/*****************************************************************************/
{
    if( ( !UTILIZAR_PRECIO_AFINIDAD || ( UTILIZAR_PRECIO_AFINIDAD && !RAM_P_TICKET ) )
     && !RAM_PUNTOS_USADOS ) {
        if( !documento ) {
            SET_MEMORY_CHAR( __ram_pedir_cliente_afinidad, 1 );
            LIMPIAR_RECUADRO_PETICION( S__CLIENTE_AFINIDAD_ );
        }
        else {
            SET_MEMORY_CHAR( __ram_pedir_cliente_afinidad, 2 );
            LIMPIAR_RECUADRO_PETICION( S__NRO_DOCUMENTO_CLIENTE__ );
        }
    }
    if( UTILIZAR_PRECIO_AFINIDAD && RAM_P_TICKET ) {
        MENSAJE_STRING( S_DEBE_SELECCIONAR_EL_CLIENTE_AL_COMIENZO );
    }
}
/*****************************************************************************/
void TOMAR_CLIENTE_AFINIDAD( int recupero)
/*****************************************************************************/
{
    int l,la_fecha,mensaje, salir = 0, k;
    char cadena[51],cad_afi[10], buffer2[51],buffer1[81],*ptr;
    char vto[9], c_despl[3], adic_habil = 'N';
	int ok = 1;
	double n_corto_temp = 0.0, n_corto_temp2 = 0.0, n1,n2, n_corto;
	char aux_numero[21], marca = 0;
	int  prefijo_ok=0, rta, mallargo = 0;
	long divisor;
	float aux_limite;
	char marca_especial = 0;



    memset( buffer2, 0, sizeof( buffer2 ) );
    memset( buffer1, 0, sizeof( buffer1 ) );
	memset( cadena,0, sizeof(cadena));
	memset( cad_afi,0, sizeof(cad_afi));
	memset( vto,0, sizeof(vto));
	memset( c_despl,0, sizeof(c_despl));

    mensaje = 0;
	if( RAM_PEDIR_CLIENTE_AFINIDAD == 2) {
		if ( recupero == NO ) {
			LIMPIA_BUFFER_TARJETA( SI ); //agregado
			 _PERMITIR_BANDA = SI;
			  SET_ESPERA_GETCH( 2 );
			 MENSAJE_STRING( S_PASE_LA_TARJETA );
		 } else {
         salir = 4;
		}
		 while( !salir ) {
         if( LECTOR_TARJETA_LEIDA(  )/* || RAM_PEDIR_CLIENTE_AFINIDAD == 2 */) {
            salir = 1;
         }
         if( !salir ) {
            k = GETCH(  );
            if( k == 27 ) {
               salir = 3;
            } else if( k == -65 ) {
               salir = 2;
            }
         }
      }
	}
	if ( salir == 2){
		int dd;
		GRABAR_LOG_SISTEMA( "PIDE DATOS TARJETA",LOG_VENTAS,4 );
		ok = PEDIR_DATOS_TARJETA( buffer1, ( char )( ( marca_especial ) ? marca_especial - 1 : 0 ) );
		if( ok != 0) {
			dd= strlen(buffer1);
			buffer1[dd-1]= '\0';
		}
    }                                                                        
	if ( salir == 3){
		ok = 0;
	}

	if( ok && (salir == 2 || LECTOR_TARJETA_LEIDA() )) {
        if( salir == 2||  LEER_TARJETA_BANDA( (unsigned char*) buffer2, (unsigned char*) buffer1 ) ) {
            l = strlen( ID_AFINIDAD );
			if( RAM_PEDIR_CLIENTE_AFINIDAD == 2){ //lareina
				if( salir == 2) {
					strncpy( cadena, buffer1,50 ); //depende que track tiene habilitado
				} else {
					if( buffer2 != NULL && strlen( buffer2) > 12 )
						strncpy( cadena, buffer2,50 );
					else
						strncpy( cadena, &buffer1[1],50 );
				}
				buffer1[49]= 0;
				buffer2[49]= 0;
			}else
            strcpy( cadena, buffer2 );
						//afinidad comun
			if( RAM_PEDIR_CLIENTE_AFINIDAD != 2){ //lareina
            if( memcmp( cadena, ID_AFINIDAD, l ) == 0 ) {
                ptr = strchr( cadena, '=' );
                if( ptr ) {
                    ptr++;
                    strcpy( vto, "01/xx/xx" );
                    vto[3] = ptr[0];
                    vto[4] = ptr[1];
                    vto[6] = ptr[2];
                    vto[7] = ptr[3];
                    la_fecha = CADENA_A_FECHA( vto );
                    if( la_fecha > GET_FECHA_DOS() ) {
                        cadena[(
                        LONGITUD_NRO_CLIENTE_AFINIDAD == 0 ? l + 8
                      : l + LONGITUD_NRO_CLIENTE_AFINIDAD ) + DESPLAZAM_DESDE_ID_CLIENTE_AFINI] = 0;
                        SET_MEMORY_LONG( __ram_cliente_afinidad, atol( &cadena[l] ) );
                        if( CONSULTA_AFINIDAD_ON_LINE ) {
                            LEER_PUNTOS();
                        }
                    }
                    else {
                        char mensaje_char[206]; //Max tamanio mensaje * 2
                        strcpy( mensaje_char, ST( S_TARJETA_VENCIDA ) );
                        strcat( mensaje_char, ".    " );
                        strcat( mensaje_char, ST ( S_PRESIONE_ENTER_PARA_CONTINUAR ) );
                        MENSAJE_CON_TECLA( mensaje_char, 13 );
                        mensaje = 2;
                    }
                }
                else {
                    MENSAJE_STRING( S_NRO_TARJ_INVALIDO );
                }
            }
            else {
                mensaje = 1;
            }
			}else { //afinidad la reina
				if( ok ){
					NRO_TARJETA( cadena, &n1, &n2,  0 );
					for( k = ( 10 - DIGITOS_BIN_INTERNACIONAL ), divisor = 1;k > 0;k--, divisor *= 10 ) {
						;
					}
					n_corto_temp = n1 / divisor;
					n_corto_temp2 = modf( n_corto_temp, &n_corto );
      				rta = BUSCA_MARCA_TARJETA( n_corto, &marca, &aux_limite );
					if( rta ) {
						rta = EXISTE_TARJETA( marca );
					}
					if( rta == 1 && ok  ){
      				prefijo_ok = 1;
					CADENA_NRO_TARJETA( cadena, aux_numero, marca );
					if(LONGITUD_TARJETA_AFINIDAD > 12)
					   cadena[LONGITUD_TARJETA_AFINIDAD]= '\0';
					if( ok && prefijo_ok && !VALIDA_LARGO_NUMERO( marca, strlen( cadena ) ) ){
		  				prefijo_ok = 0;
      					mallargo = 1;
      				}

      				l = strlen( ID_AFINIDAD );	  //PREFIJO TARJETA AFINIDAD
					if( memcmp( cadena, ID_AFINIDAD, l ) == 0 ) {
   						prefijo_ok = 1;
   					} else {
   						prefijo_ok = 0;
					}
   				if( prefijo_ok == 1) {
         				SET_MEMORY_CHAR( __var_tarjetas_status, 1 );//agregado
         				SET_MEMORY_CHAR( __var_tarjetas_tarjeta, marca );
   					SET_MEMORY_CHAR( __var_tarjetas_confirmar_numero, TARJ[marca].nodo ); //reusamos esta variable.
				}
					}else {
						mensaje= 1;
						CEREAR_MEMORY( __ram_string_cliente_afinidad );
						SET_MEMORY_LONG( __ram_cliente_afinidad, 0L );
					}

		   } //ok
		} //ELSE AFINIDAD la reina
			if( prefijo_ok && rta == 1 ) {
			//LONGITUD CLIENTE AFINIDAD = cuantos nros tengo que tomar de la banda como cliente
			//INDICE COMIENZA CLIENTE Y INDICE FIN NRO CLIENTE BANDA= desde que posicion hasta que posicion tomo el cliente
			//
			int desde, hasta;
			desde= INDICE_COMIENZO_CLIENTE_EN_BANDA - 1;
			hasta = INDICE_COMIENZO_CLIENTE_EN_BANDA - 1 + LONGITUD_NRO_CLIENTE_AFINIDAD ;

			SET_MEMORY( __ram_string_cliente_afinidad, cadena );
			
			cadena[hasta] = '\0';
            
			SET_MEMORY_LONG( __ram_cliente_afinidad,
                                     atol( &cadena[desde] ) );
			if( CONSULTA_AFINIDAD_ON_LINE ) {
				if( recupero == NO ) {
					LEER_PUNTOS(  );
					if ( RAM_PUNTOS_AFINIDAD == -1) {
						rta = 0;
						mensaje = 1;
					}
				} else
					rta = 1;
				if( rta == 0) {
				   CEREAR_MEMORY( __ram_string_cliente_afinidad );
					SET_MEMORY_LONG( __ram_cliente_afinidad, 0L );
							   //limpiemos la afinidad
				}
			}
						//---- Se habilita solo para pruebas !!!!
						//SET_MEMORY_DOUBLE( __ram_puntos_afinidad, 400.0 );
			}else {
				mensaje =1;
						CEREAR_MEMORY( __ram_string_cliente_afinidad );
						SET_MEMORY_LONG( __ram_cliente_afinidad, 0L );
			}
        }
	}else {
		if( ok ) {
        l = strlen( ID_AFINIDAD );
		//Se toma el cliente de RAM_COD_BARRA porque se ingresa como el Codigo de Barra de un art.
		strncpy( cadena, RAM_COD_BARRA, sizeof( cadena ) );
		LTRIM(cadena);
		strncpy( cad_afi, ID_AFINIDAD, sizeof( cad_afi ) );
		LTRIM(cad_afi);
        if( RAM_PEDIR_CLIENTE_AFINIDAD == 1 ) {
            if( memcmp( cadena,cad_afi, l ) == 0 ) {
                if( !CRC_SCANNER( cadena, 13, 1 ) ) {
                    char grabarlog[25],cadaux[17];
                    mensaje = 1;
                    strncpy( cadaux, cadena ,sizeof(cadaux));
                    _snprintf( grabarlog, sizeof(grabarlog)-1, "ERROR DE CRC [%d]", cadaux[12] - '0' );
                    GRABAR_LOG_SISTEMA( grabarlog ,LOG_ERRORES,2);
                }
                if( !mensaje ) {
                    cadena[( LONGITUD_NRO_CLIENTE_AFINIDAD ? l + LONGITUD_NRO_CLIENTE_AFINIDAD
                           : 12 ) + DESPLAZAM_DESDE_ID_CLIENTE_AFINI] = 0;
                    SET_MEMORY_LONG( __ram_cliente_afinidad,
                                     atol( &cadena[l + DESPLAZAM_DESDE_ID_CLIENTE_AFINI] ) );
                }
                if( CONSULTA_AFINIDAD_ON_LINE ) {
                    if( config.pais == COLOMBIA
                     || ( ( config.pais == CHILE || config.pais == BRASIL )
                       && USAR_CLI_AFI_PARA_CLIENTES_AFINI ) ) {
                        OBTENER_DATOS_CLIENTE_AFINIDAD( NO );
                    }
                    else {
                        LEER_PUNTOS();
                    }
                }
            }
            else {
                mensaje = 1;
            }
        }
        else if( CONSULTA_AFINIDAD_ON_LINE && ( config.pais == COLOMBIA ) ) {
            OBTENER_DATOS_CLIENTE_AFINIDAD( SI );
        }
    }
	}
    if( mensaje == 1 ) {
        SET_MEMORY_LONG( __ram_cliente_afinidad, 0L );
        CEREAR_MEMORY( __ram_string_cliente_afinidad );
        MENSAJE_STRING( S_NO_ES_CLIENTE_AFINIDAD );
	} else if( ok == 0) {
		glog("PRESION ESCAfini", LOG_VARIOS,5);
		SET_MEMORY_LONG( __ram_cliente_afinidad, 0L );
        CEREAR_MEMORY( __ram_string_cliente_afinidad );
	}else {
		if( VAR_TARJETAS_NRO_CUENTA[0] != 0 ) {
			long cliente_a = 0;
			char cadena1[20];

			memset(cadena1,0, sizeof(cadena1));

			memcpy(cadena1, VAR_TARJETAS_ANT_NRO_CUENTA, sizeof(cadena1));
			cadena1[19]= 0;
			cliente_a = atol( cadena1 );
			if( cliente_a != RAM_CLIENTE_AFINIDAD )
				SET_MEMORY_LONG( __ram_cliente_afinidad, cliente_a );


		}
        if( mensaje != 2 && CONTROLAR_CLIENTE_AFINIDAD && !EXISTE_CLIENTE( RAM_CLIENTE_AFINIDAD ) ) {
            MENSAJE_STRING( S_EL_CLIENTE_NO_EXISTE );
            SET_MEMORY_LONG( __ram_cliente_afinidad, 0L );
        }
        else {
            if( RAM_PEDIR_CLIENTE_AFINIDAD != 2 ) {
				strncpy( c_despl, &cadena[l], DESPLAZAM_DESDE_ID_CLIENTE_AFINI<4? DESPLAZAM_DESDE_ID_CLIENTE_AFINI:3 );
            atoi( c_despl );
				SET_MEMORY( __ram_string_cliente_afinidad, cadena );
				if ( l < 7)
						l=7;
				SET_MEMORY_LONG( __ram_cliente_afinidad, atol( &cadena[l] ) );
			}

            if( clientes.cod_cliente_titular ) {
                // Caso Cliente Afinidad Adicional
                SET_MEMORY_INT( __ram_fecha_cumple_afi_adicional, clientes.fecha_nac );
                SET_MEMORY( __ram_nom_cli_afinidad_adicional, clientes.nombre );
                SET_MEMORY_LONG( __ram_cliente_afinidad_adicional, RAM_CLIENTE_AFINIDAD );
                if( clientes.porc_desc < 100 ) {
                    SET_MEMORY_FLOAT( __ram_descuento_afinidad_adicional, clientes.porc_desc );
                }
                else {
                    MENSAJE_STRING( S_PORC_DE_DESCUENTO_INVALIDO );
                    SET_MEMORY_FLOAT( __ram_descuento_afinidad_adicional, 0.0 );
                }
                SET_MEMORY_INT( __ram_cod_mutual_afinidad_adicional, clientes.cod_mutual );
                adic_habil = clientes.hab_cliente_frecuente;
                if( EXISTE_CLIENTE( clientes.cod_cliente_titular ) ) {
                    SET_MEMORY_LONG( __ram_cliente_afinidad, clientes.cod_cliente );
                    SET_MEMORY( __ram_nombre_cliente_afinidad, clientes.nombre );
                }
                else {
                    MENSAJE_STRING( S_EL_CLIENTE_NO_EXISTE );
                    SET_MEMORY_LONG( __ram_cliente_afinidad, 0L );    
                    SET_MEMORY_LONG( __ram_cliente_afinidad_adicional, 0L );
                    SET_MEMORY_INT( __ram_fecha_cumple_afi_adicional, 0 );
                    SET_MEMORY_FLOAT( __ram_descuento_afinidad_adicional, 0.0 );
                    CEREAR_MEMORY( __ram_nombre_cliente_afinidad );
                    CEREAR_MEMORY( __ram_nom_cli_afinidad_adicional );
                    SET_MEMORY_INT( __ram_cod_mutual_afinidad_adicional, 0 );
                }
            }
            else {
                //---caso CLIENTE AFINIDAD TITULAR
                SET_MEMORY( __ram_nombre_cliente_afinidad, clientes.nombre );
                SET_MEMORY_LONG( __ram_cliente_afinidad_adicional, 0L );
                SET_MEMORY_INT( __ram_fecha_cumple_afi_adicional, 0 );
                SET_MEMORY_FLOAT( __ram_descuento_afinidad_adicional, 0.0 );
                CEREAR_MEMORY( __ram_nom_cli_afinidad_adicional );
                SET_MEMORY_INT( __ram_cod_mutual_afinidad_adicional, 0 );
            }
            if( !VERIFICAR_CLIENTE_HABILITADO( adic_habil ) ) {
                SET_MEMORY_LONG( __ram_cliente_afinidad, 0L );      
                SET_MEMORY_LONG( __ram_cliente_afinidad_adicional, 0L );
            }
        }
    }
    SET_MEMORY_CHAR( __ram_pedir_cliente_afinidad, 0 );
	_PERMITIR_BANDA = NO;
	LIMPIA_BUFFER_TARJETA( SI );
	VACIAR_KEY();
	if( RAM_CLIENTE_AFINIDAD > 0 && clientes.cod_perfil > 0 
		//Para el caso de secuencia TOTAL - ESC, no cargue perfil si se lo saco
		&& !( recupero && !RAM_PERFIL ) )
		CARGAR_PERFIL_CLIENTE( clientes.cod_perfil );
	else 
		if( RAM_CLIENTE_AFINIDAD == 0 ) {
			if( RAM_PERFIL > 0){
				CEREAR_PERFIL_CLIENTE();
				MUESTRA_PERFIL();
			}
		}


    MOSTRAR_CLIENTE_AFINIDAD();
    LIMPIAR_RECUADRO_PETICION( 0 );
	if( RAM_CLIENTE_AFINIDAD > 0 && RAM_NRO_CLIENTE > 0 && RAM_CLIENTE_AFINIDAD != RAM_NRO_CLIENTE ) {
		//VOLVAMOS AL CLIENTE DE FACTURACION
		long cli_auxi = RAM_NRO_CLIENTE;
		EXISTE_CLIENTE( cli_auxi );
	}


}
/*****************************************************************************/
void LEER_PUNTOS()
/*****************************************************************************/
{
    /*------------------------- Envia el paquete -------------------------*/
    #ifdef COMPILAR_CLIENTE_AFINIDAD_ON_LINE
    ENVIAR_PAQUETE_AFINIDAD( _ON_LINE_SALDO, RAM_CLIENTE_AFINIDAD, 0 );
    #endif
}
/*****************************************************************************/
int OBTENER_FECHA_PUNTOS( char *buffer )
/*****************************************************************************/
{
    char _fecha[9];
    int fecha = 0, i, encontro;
    for( encontro = 0, i = 0;buffer[i] && !encontro;i++ ) {
        if( buffer[i] == '/' && buffer[i + 3] == '/' ) {
            encontro = 1;
        }
    }
    if( encontro ) {
        memcpy( _fecha, &buffer[i - 3], 8 );
        _fecha[8] = 0;
        fecha = CADENA_A_FECHA( _fecha );
    }
    return ( fecha );
}
/*****************************************************************************/
int OBTENER_DATOS_CLIENTE_AFINIDAD( int documento )
/*****************************************************************************/
{
    char where[80],nombre[41],auxi[50],cod_barra[17];
    int ok = 0, i, cont;
    int arriba = 15, izquierda = 10, derecha = 70, abajo;
    int l;

	struct _clientes_afinidad
    {
        BINARY status;
        char cod_cliente[14];
        char nombre[21];
        char apellido[21];
        char direccion[51];
        char telefono[21];
        char ciudad[41];
        char documento[21];
        int categoria;
        int genero;
        int fecha_nac;
        double puntos_acumulados;
        int fecha_puntos;
        char cant_promociones[5];
        char desc_categoria[21];
        BINARY fin;
    } cli_afi;
    #ifdef INVEL_L
    #define CLI_AFI "cli_afi.ctr"
    #else
    #define CLI_AFI "cli_afi.btr"
    #endif
    l = strlen( ID_AFINIDAD );
    memset( nombre, 0, sizeof( nombre ) );
    SET_MEMORY( __ram_nombre_cliente_afinidad, nombre );
    SET_MEMORY_DOUBLE( __ram_puntos_afinidad, ( double )0.0 );
    SET_MEMORY_INT( __ram_fecha_puntos_afinidad, 0 );
    SET_MEMORY_INT( __ram_afinidad_fecha_nac, 0 );
    SET_MEMORY_INT( __ram_codigo_tarjeta_afinidad, 0 );
    for( i = 0;i < 5;i++ ) {
        _SET_MEMORY_CHAR( __ram_promociones_afinidad, i, 0 );
    }
    if( documento ) {
        strcpy( cod_barra, RAM_COD_BARRA );
        for( i = 0, cont = 0;i < 17;i++ ) {
            if( cod_barra[i] == ' ' ) {
                cont++;
            }
        }
        _snprintf( where, sizeof(where)-1, " WHERE documento = '%s'", &cod_barra[cont] );
    }
    else {
        _snprintf( where, sizeof(where)-1, " WHERE cod_cliente = '%s'", RAM_STRING_CLIENTE_AFINIDAD );
    }
	/*if( DBROUTER_OBTENER_TABLA( CLI_AFI, "vista_clientes_afinidad", where, SI ) ) {
        if(OPEN_TABLE(T_CLI_AFI, TT_ORIG ,(char*)&cli_afi,sizeof( struct _clientes_afinidad ))== 0) {
            if( RECCOUNT2(NO,"") ){
                ok = 1;
                if( documento ) {
                    SET_MEMORY( __ram_string_cliente_afinidad, cli_afi.cod_cliente );
                    strcpy( auxi1, cli_afi.cod_cliente );
                    auxi1[12] = 0;
                    SET_MEMORY_LONG( __ram_cliente_afinidad, atol( &auxi1[l] ) );
                }
                strcpy( nombre, cli_afi.nombre );
                for( i = strlen( nombre ) - 1;i && nombre[i] == ' ';i-- ) {
                    nombre[i] = 0;
                }
                strcat( nombre, " " );
                strcat( nombre, cli_afi.apellido );
                nombre[30] = 0;
                SET_MEMORY( __ram_nombre_cliente_afinidad, nombre );
                SET_MEMORY_DOUBLE( __ram_puntos_afinidad, cli_afi.puntos_acumulados );
                SET_MEMORY_INT( __ram_fecha_puntos_afinidad, cli_afi.fecha_puntos );
                SET_MEMORY_INT( __ram_afinidad_fecha_nac, cli_afi.fecha_nac );
                SET_MEMORY_INT( __ram_codigo_tarjeta_afinidad, cli_afi.categoria );
                for( i = 0;i < 5;i++ ) {
                    _SET_MEMORY_CHAR( __ram_promociones_afinidad, i, cli_afi.cant_promociones[i] );
                }
            }
            CLOSE_TABLE( T_CLI_AFI, TT_ORIG );
        }
    }*/
    if( MOSTRAR_DATOS_CLIENTE_AFINIDAD ) {
        if( ok ) {
            abajo = arriba + 8;
            _snprintf( auxi, sizeof(auxi)-1, ST( S_NUMERO____S ), RAM_STRING_CLIENTE_AFINIDAD );
            MOSTRAR_CADENA( izquierda + 2 - ancho_panel / 2, arriba + 1, auxi );
            _snprintf( auxi, sizeof(auxi)-1, ST( S_NOMBRE____S ), RAM_NOMBRE_CLIENTE_AFINIDAD );
            MOSTRAR_CADENA( izquierda + 2 - ancho_panel / 2, arriba + 3, auxi );
            _snprintf( auxi, sizeof(auxi)-1, "Puntos : %*.*f", ENTEROS + 2, DECIMALES, RAM_PUNTOS_AFINIDAD );
            MOSTRAR_CADENA( izquierda + 2 - ancho_panel / 2, arriba + 5, auxi );
            _snprintf( auxi, sizeof(auxi)-1, ST( S_CATEG_____S ), cli_afi.desc_categoria );
            MOSTRAR_CADENA( izquierda + 2 - ancho_panel / 2, arriba + 7, auxi );
            MENSAJE_CON_TECLA(ST( S_PRESIONE_ENTER_PARA_CONTINUAR ),13);
            /*MENSAJE_SIN_SONIDO_STRING( S_PRESIONE_ENTER_PARA_CONTINUAR );
            while( GETCH() != 13 );*/
        }
    }
    return ( ok );
}
/*****************************************************************************/
int AFINIDAD_DESCUENTO_PUNTOS( double puntos, double descuento )
/*****************************************************************************/
{
    int rta = 0;
    char temp[100];
    int salir = 0;
    int espera;
    if( RAM_CLIENTE_AFINIDAD ) {
        if( ( RAM_PUNTOS_AFINIDAD - RAM_PUNTOS_USADOS - puntos ) > -0.005 ) {
            _snprintf( temp, sizeof(temp)-1, ST( S_ACEPTA_DESCONTAR__5_2F_PUNTOS__DESCUENTO___5_2F__ENTER_ESC_ ),
                     puntos / 100.00, descuento );
            //MENSAJE_SIN_SONIDO( temp,NO );
            espera = GET_ESPERA_GETCH();
            SET_ESPERA_GETCH( 1 );
            if(MENSAJE_CON_TECLA_S_N(temp, 13, 27) == 13){
                rta = 1;
            }
            /*while( !salir ) {
                k = GETCH();
                if( k == 27 ) {
                    salir = 1;
                }
                if( k == 13 ) {
                    salir = 1;
                    rta = 1;
                }
            }*/
            SET_ESPERA_GETCH( espera );
        }
    }
    return rta;
}
/*****************************************************************************/
int VERIFICAR_CLIENTE_HABILITADO( char adicional )
/*****************************************************************************/
{
    int ok = 1;
    if( RAM_CLIENTE_AFINIDAD && ( clientes.hab_cliente_frecuente == 'N' ) ) {
        ok = 0;
        if( clientes.puntos ) {
            MENSAJE_STRING( S_CLIENTE_BLOQUEADO );
        }
        else {
            MENSAJE_STRING( S_CLIENTE_NO_HABILITADO );
        }
    }
    if( ok && RAM_CLIENTE_AFINIDAD_ADICIONAL && ( adicional == 'N' ) ) {
        MENSAJE_STRING( S_CLIENTE_ADICIONAL_BLOQUEADO );
        ok = 0;
    }
    return ( ok );
}

/***************************************************************************/
void RECUPERAR_CLIENTE_AFINIDAD( void ){
/***************************************************************************/
	char      cadena1[50];

	if( _RAM_STRING_CLIENTE_AFINIDAD(0) ){
		strcpy( cadena1, RAM_STRING_CLIENTE_AFINIDAD );
		TRIM( cadena1 );
		//SET_MEMORY( __ram_cod_barras_leido, cadena1 );
   		SET_MEMORY_CHAR( __ram_pedir_cliente_afinidad, 2 );
   		TOMAR_CLIENTE_AFINIDAD( SI );//PONER SI SI SE DESEA QUE NO CONSULTA NUEVAMENTE
   }
}