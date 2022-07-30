#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <global.h>
#include <aarch.h>
#include <btrv.h> 
#include <calcular.h> 
#include <cmemory.h> 
#include <cr.h>  
#include <dbrouter.h> 
#include <dd.h> 
#include <formulc.h> 
#include <log.h> 
#include <mensaje.h> 
#include <trim.h> 
#include <path.h>
#include <db.h>
#include <deftables.h>
/*
 * ******************************************************************************
 * Sistema : TPV 1000 II
 * M¢dulo  : Funciones para c lculo de expresiones matem ticas con variables RNV.
 * Programador : LUIS HORACIO DIAZ STOFFEL
 * Fecha   : 18/09/2002
 * ******************************************************************************
 */
extern void ERROR_SISTEMA_ERRNO(void);
extern int RESOLVER_VAR_NOMBRES( char *cadena );
extern int GET_POSICION_DEL_FOR( int codigo_subdindice );
int VALOR_VARIABLE( char *valor, int cod_variable, int tipo_variable, int subindice );
/*-------------------------------------------------------------------------*/
int RETORNAR_FORMULA( char *formula, int tipo_calculo, int cod_iva_cliente )
/*-------------------------------------------------------------------------
Retorna la formula para la base imponible de acuerdo al codigo de iva del
cliente y al tipo de calculo (correspondiente a la provincia).
-------------------------------------------------------------------------*/
{
    int rta = 0;
	int  area_ant, tipo_ant;

    struct _formula_dgr form_dgr_sic;
    /*struct _indice
    {
        BINARY tipo_calculo;
        BINARY cod_iva_cliente;
    } indice;*/
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

    /*indice.tipo_calculo = (char)tipo_calculo - '0';
    indice.cod_iva_cliente = (char)cod_iva_cliente;*/
  //  if( !USED( AREA_AUX7 ) ) {
      //  if( USE_DB( AREA_AUX7, _FORM_DGR_SIC, ( char* )&form_dgr_sic, sizeof( form_dgr_sic ), NULL,
       //             0, 0 ) == 0 ) {
		if( OPEN_TABLE( T_FORMULA_DGR, TT_ORIG, ( char* )&form_dgr_sic, sizeof( struct _formula_dgr ) ) == 0 ) {
           // GET_EQUAL( ( char* )&indice );
			//SELECT_TABLE( T_FORM_DGR, TT_ORIG );
			SET_WHERE("TIPO_CALCULO = '%d' and COD_IVA_CLIENTE = '%d'", /*indice.*/tipo_calculo, /*indice.*/cod_iva_cliente );
			RUN_QUERY(NO);
            if( FOUND2() ) {
                memset( formula, 0, sizeof( form_dgr_sic.formula_base_imp ) );
                strncpy( formula, form_dgr_sic.formula_base_imp,
                         sizeof( form_dgr_sic.formula_base_imp ) - 1 );
                TRIM( formula );
                rta = 1;
            }
            else {
                char *msg = "ERROR: NO SE ENCONTRO FORMULA";
                MENSAJE( msg );
                GRABAR_LOG_SISTEMA( msg ,LOG_ERRORES,2);
                strcpy( formula, "" );
            }
           // CLOSE_DATABASE( AREA_AUX7 );
			CLOSE_TABLE( T_FORMULA_DGR, TT_ORIG );
        }
    //}
   // else {
   //     char *msg = "ERROR: AREA AUX7 USADA";
   //     MENSAJE( msg );
   //     GRABAR_LOG_SISTEMA( msg );
   // }
    //SELECT( area );
	SELECT_TABLE( area_ant, tipo_ant );
    return rta;
}
/*-------------------------------------------------------------------------*/
int OBTENER_VALOR( char *variable, char *valor, struct _valores_calculo valores_calculo )
/*-------------------------------------------------------------------------
Retorna el valor de una determinada variable.
-------------------------------------------------------------------------*/
{
    int rta = 1, i, i_var, i_sub, ln;
    int area_ant, buscar_por_nombre, tipo_ant;
    int error = 0;
    char nombre[50],subindice_aux[10],cad_aux[30];
    int subindice = 0, etapa = 0;
    double total;
    unsigned char offset;

    //Se separa el nombre de la variable y el subindice.
    ln = strlen( variable );
    for( i = 0, i_var = 0, i_sub = 0;( i < ln ) && ( etapa != 2 );i++ ) {
        if( variable[i] == '(' ) {
            etapa = 1;
        }
        else if( variable[i] == ')' ) {
            etapa = 2;
        }
        else {
            switch( etapa ) {
                case 0:
                    //Variable
                    nombre[i_var] = variable[i];
                    i_var++;
                    break;
                case 1:
                    //Subindice
                    subindice_aux[i_sub] = variable[i];
                    i_sub++;
                    break;
            }
        }
    }
    nombre[i_var] = 0;
    subindice_aux[i_sub] = 0;
    if( i_sub == 0 ) {
        subindice = 0;
    }
    else {
		if( subindice_aux[0]=='I' && GET_POSICION_DEL_FOR( 254 )){ 
			subindice = GET_POSICION_DEL_FOR( 254 );
		}else
        subindice = atoi( subindice_aux );
    }
    if( etapa == 1 ) {
        error = 4;
    }
    //Se verifican los valores "ESTATICOS" pasados por parametro.
    if( strcmp( nombre, "<IMPORTE_NETO>" ) == 0 ) {
        sprintf( valor, "%.5lf", valores_calculo.importe_neto );
    }
    else if( strcmp( nombre, "<IVA>" ) == 0 ) {
        sprintf( valor, "%.5lf", valores_calculo.iva );
    }
    else if( strcmp( nombre, "<EXENTO>" ) == 0 ) {
        sprintf( valor, "%.5lf", valores_calculo.exento );
    }else if( strcmp( nombre, "<TKT_INI_EST>" ) == 0 ) {
        sprintf( valor, "%i", valores_calculo.tkt_ini_est );
    }
    else {
        if( strlen( nombre ) < 35 ) {
            for( i = strlen( nombre );i < 35;i++ ) {
                nombre[i] = ' ';
            }
            nombre[35] = 0;
        }
        buscar_por_nombre = ( nombre[0] == '#' ) ? NO : SI;
        //Se busca la variable en DD_VAR.SIC por su nombre.
        if( buscar_por_nombre ) {
            area_ant = SELECTED2();
			tipo_ant = SELECTED_TIPO();
            if( /*!USED( AREA_AUX7 )*/ !OPENED(DD_VAR, TT_ORIG) ) {
                if(  /*USE_DATABASE( AREA_AUX7, DD_VAR_SIC, ( char* )&dd_var, sizeof( dd_var ), NULL, 0, 0 )*/ 
					 OPEN_TABLE(DD_VAR, TT_ORIG ,(char*)&dd_var,sizeof( struct _dd_var ))== 0 )  {
					 //SET_ORDER( 4 );
					 SET_ORDER2("NOM_VARIABLE");
					 //GET_EQUAL( ( char* )&nombre );
					 SET_WHERE("NOM_VARIABLE = '%s'",nombre);
					 RUN_QUERY(NO);
					 if( !FOUND2() ) {
                        error = 2;
					 }
					 //CLOSE_DATABASE( AREA_AUX7 );
					 CLOSE_TABLE(DD_VAR, TT_ORIG);
                }
                else {
                    error = 3;
                }
            }
            else {
                error = 4;
            }
            //SELECT( area );
			SELECT_TABLE(area_ant, tipo_ant);
        }
        else {
            //Se determina si la busqueda es con offset
            offset = ( strncmp( nombre + 1, "OFF", 3 ) == 0 );
            if( offset ) {
                dd_var.cod_variable = ( INT16 )EST_VAR( valores_calculo.tkt_ini_est
                    + ( INT16 )atoi( &nombre[4] ) );
            } else {
                dd_var.cod_variable = atoi( &nombre[1] );
            }
        }
        if( !error ) {
            if( subindice == 9999 ) {
                error = 0;
                total = 0.0;
                subindice = DD_ELEMENTOS( dd_var.cod_variable );
                for( i = 0 ;i < subindice && !error ;i++ ) {
                    error = VALOR_VARIABLE( cad_aux, dd_var.cod_variable,
                                            DD_TIPO( dd_var.cod_variable ), i );
                    total += atof( cad_aux );
                }
                if( !error ) {
                    sprintf( valor, "%.5lf", total );
                }
            }
            else {
                error = VALOR_VARIABLE( valor, dd_var.cod_variable, DD_TIPO( dd_var.cod_variable ),
                                        subindice );
            }
        }
    }
    if( error ) {
        //De presentarse un error se indica el codigo del mismo y se retorna
        //una cadena vacia.
        char msg_error[20];
        _snprintf(msg_error, sizeof(msg_error)-1, "CALC ERR %d", error );
        MENSAJE( msg_error );
        strcpy( valor, "" );
    }
    return rta;
}
/*-------------------------------------------------------------------------*/
int VALOR_VARIABLE( char *valor, int cod_variable, int tipo_variable, int subindice )
/*-------------------------------------------------------------------------*/
{
    int error = 0;
    //Se retorna el valor de la variable segun el tipo de dato y el
    //subindice.
    switch( tipo_variable ) {
        case _SHORT_INT:
            sprintf( valor, "%d", _GET_MEMORY_CHAR( cod_variable, subindice ) );
            break;
        case _INT:
        case _UNSIGNED:
        case _HORA:
        case _DATE:
            sprintf( valor, "%d", _GET_MEMORY_INT( cod_variable, subindice ) );
            break;
        case _LONG:
            sprintf( valor, "%li", _GET_MEMORY_LONG( cod_variable, subindice ) );
            break;
        case _FLOAT:
            sprintf( valor, "%.5f", _GET_MEMORY_FLOAT( cod_variable, subindice ) );
            break;
        case _DOUBLE:
            if( subindice == -3 ) {
                double temporal = 0;
                int i,max_elem;
                max_elem = DD_ELEMENTOS( cod_variable );
                for( i = 0 ;i < max_elem ;i++ ) {
                    temporal += _GET_MEMORY_DOUBLE( cod_variable, i );
                }
                sprintf( valor, "%.5lf", temporal );
            }
            else {
				int rango ;
				char valor1[100];
                _snprintf( valor1,100,"%.5lf", _GET_MEMORY_DOUBLE( cod_variable, subindice ) );//controlar que no se desborde?¿?¿
				rango = _CONTROLAR_RANGO_VARIABLES( valor1, _DOUBLE );
				if( rango == 0) {
					glog(ST(ERROR_DE_OVERFLOW_EN_LA_MULTIPLICACION),LOG_INCONDICIONAL,1 );
					_SET_MEMORY_DOUBLE( cod_variable, subindice, 0 );
					ERROR_SISTEMA_ERRNO();
				}
				sprintf( valor,"%.5lf", _GET_MEMORY_DOUBLE( cod_variable, subindice ) );
            }
            break;
        case _STRING:
            sprintf( valor, "%s", _GET_MEMORY_STR( cod_variable, subindice ) );
            break;
        default:
            strcpy( valor, "" );
            error = 1;
            break;
    }
    return( error );
}
/*-------------------------------------------------------------------------*/
int REEMPLAZAR_VARIABLE( char *formula, char *formula_final, int indice, int longitud,
                         struct _valores_calculo valores_calculo )
/*-------------------------------------------------------------------------
Determina la variable a valuar para obtener su valor ejecutando la funcion
OBTENER_VALOR.
-------------------------------------------------------------------------*/
{
    int i, ind = 0, rta = -1, fin_variable = 0;
    char variable[50];
    char valor[100];
    //En base al "indice" indicado se busca el caracter "}" que indica el
    //fin de la varaible, para asi obtener su valor.
    strcpy( variable, "" );
	memset(valor,0, 100);
	memset(variable,0, 50);
    for( i = indice + 1;( i < longitud ) && ( !fin_variable );i++ ) {
        if( formula[i] == '}' ) {
            fin_variable = 1;
        }
        else {
            variable[ind] = formula[i];
            ind++;
            variable[ind] = 0;
        }
    }
    if( fin_variable ) {
        //Si se encontro el fin de la variable se llama a la funcion
        //OBTENER_VALOR para concatenar el valor correspondiente a la
        //"formula_final".
        if( OBTENER_VALOR( variable, valor, valores_calculo ) ) {
            char valor_parentesis[102];
            _snprintf(valor_parentesis, sizeof(valor_parentesis)-1, "(%s)", valor );
			memset(valor,0,sizeof(valor));
			valor[0]='\0';
            strcat( formula_final, valor_parentesis );

        }
    }
    rta = i - 1;
    return rta;
}
/*-------------------------------------------------------------------------*/
int ANALIZAR_FORMULA( char *formula, struct _valores_calculo valores_calculo )
/*-------------------------------------------------------------------------
Recorre la formula buscando variables (delimitadas por "{}") invocando
a la funcion REEMPLAZAR_VARIABLE de ser necesario.
-------------------------------------------------------------------------*/
{
    int ln,i,ind;
    char formula_final[500];
    int rta = 1;
    //Se recorre la formula inicial buscando el caracter "{" que indica
    //el comienzo de una variable, y se va creando la "formula_final".
    memset(formula_final,0, sizeof(formula_final));
    ln = strlen( formula );
    strcpy( formula_final, "" );
    ind = 0;
    for( i = 0;( i < ln ) && ( i >= 0 );i++ ) {
        if( formula[i] == '{' ) {
            //Si se detecta una variable se reemplaza por su respectivo valor
            //en la "formula_final".
            i = REEMPLAZAR_VARIABLE( formula, formula_final, i, ln, valores_calculo );
            ind = strlen( formula_final );
        }
        else {
            formula_final[ind] = formula[i];
            ind++;
            formula_final[ind] = 0;
        }
    }
    if( i < 0 ) {
        rta = 0;
    }
    else {
        strcpy( formula, formula_final );
    }
    return rta;
}
/*-------------------------------------------------------------------------*/
double RESOLVER( char *formula, struct _valores_calculo *valores_calculo )
/*-------------------------------------------------------------------------
Analiza la formula reemplazando las variables por sus valores correspondientes
y se resuelve la expresi¢n.
-------------------------------------------------------------------------*/
{
    int length, error = 0, err, errno_anterior;
    formu code;
    double result = 0.0;
    char source[500],mens[60],error_message[30];
    struct _valores_calculo __valores_calculo;

	errno_anterior = errno; //guardamos el errno anterior
    //Valores auxiliares necesarios para calculo de formulas.
	ERROR_SISTEMA_ERRNO();
	errno=0;
	memset(&__valores_calculo,0,sizeof(struct _valores_calculo));
	memset(source,0,sizeof(source));

	if( valores_calculo ) {
        memcpy( &__valores_calculo, valores_calculo, sizeof( struct _valores_calculo ) );
    }
    else {
        memset( &__valores_calculo, 0, sizeof( struct _valores_calculo ) );
    }
	/*AL NO INICIALIZAR LAS VARIABLES OCASIONA ERROR POR LO TANTO SE ASEGURA ESTE LIMPIA */
	if( errno > 0 ) {
		memset( &__valores_calculo, 0, sizeof( struct _valores_calculo ) );
		errno=0;
	}
	strcpy( source, formula );
	RESOLVER_VAR_NOMBRES( source);
    //printf("%s %s",source,formula);
    memset( mens, 0, sizeof( mens ) );
    memset( error_message, 0, sizeof( error_message ) );
    //Se analiza la formula para reemplazar las variables por sus respectivos
    //valores y luego se utiliza el algoritmo "formulc" para obtener el valor
    //de la expresion matematica.
    if( ANALIZAR_FORMULA( source, __valores_calculo ) ) {
        result = 0;
        code = translate( source, "", &length, &err );
        if( fget_error() != NULL )
                                         //se guarda el error porque "fnot_empty" lo borra
        {
            strcpy( error_message, fget_error() );
        } 
        if( !fnot_empty( code ) ) {
            error = 5;
            strcpy( mens, "Error en Formula" );
        }
        else {
            result = fval_at( code );
            if( errno == ERANGE ) {
                error = 6;
                strcpy( mens, "Error de rango (interprete de formula)." );
            }
            else if( errno == EDOM ) {
                error = 7;
                strcpy( mens, "Error de Dominio (interprete de formula)." );
            }
        }
    }
    else {
        error = 8;
        strcpy( mens, "Error de parseo." );
    }
    if( error ) {
        char msg_error[70];
        MENSAJE( mens );
        _snprintf(msg_error, sizeof(msg_error)-1, "ERROR %d: %s ", error, mens );
        GRABAR_LOG_SISTEMA( msg_error ,LOG_ERRORES,2);
        //si el error es de la formula
        if( error_message != NULL ) {
            _snprintf(msg_error, sizeof(msg_error)-1, "%s en caracter #%d en la sig formula ", error_message, err );
            GRABAR_LOG_SISTEMA( msg_error ,LOG_ERRORES,2);
            GRABAR_LOG_SISTEMA( source ,LOG_ERRORES,2);
            GRABAR_LOG_SISTEMA( formula ,LOG_ERRORES,2);
        }
    }
    errno = errno_anterior; //guardamos el errno anterior
	return( result );
}

/*-------------------------------------------------------------------------*/
double _RESOLVER( char *formula, struct _valores_calculo *valores_calculo, char *cadena )
/*-------------------------------------------------------------------------
 * sobrecarga de la funcion RESOLVER(..)
 * Es para permitir que las variables cargadas con las funciones del tipo var(n), puedan ser
 * tratadas como indices cuando se intente usar por ejemplo la siguietne línea en el tkt-caja.cfg
 * [CONSULTA TABLA]      [47] [1] [(strstr)var(4);] [string] [18] [default(noEncontrado) largo_str(10)]
 * (en este ejemplo se intenta acceder a la tabla articulo.sic, con un código de barras cargado en var(4)
 */

{
	double rta = 0.0;
	
	rta = RESOLVER( formula, valores_calculo );
	sprintf( cadena, "%013.0lf", rta );
	return( rta );
}

