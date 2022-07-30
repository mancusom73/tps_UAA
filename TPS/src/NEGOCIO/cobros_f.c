#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <aarch.h>
#include <btrv.h> 
#include <b_mens.h> 
#include <cobros.h> 
#include <cr.h>  
#include <dbrouter.h> 
#include <fecha.h> 
#include <get_fech.h> 
#include <mstring.h> 
#include <tkt.h> 
#include <trim.h> 
#include <deftables.h>
#include <db.h>
#pragma warning (disable:4103)  //aviso de pragma pack en cr.h
struct _obtenido_codigo_barra
{
    // Datos comprobante.
    char C[11];               // NRO CUENTA
    char N[11];               // NRO COMPROBANTE
    char R[2];              // MONEDA IMPORTE (0-PESO 1-DOLAR)
    char U[3];              // CODIGO DE COBRO
    char Z[2];              // LETRA DE COMPROBANTE
    char Y[5];              // PUNTO DE VENTA
    char B[4];              // AÑO DE EMISION
    // Importe primer Vencimiento
    char E[11];               // PARTE ENTERA
    char K[6];                // PARTE DECIMAL
    // Fecha Primer Vencimiento
    char G[5];                // DIF. EN DIAS DESDE 1/1/1998
    char L[5];              // DIF. EN DIAS DESDE AÑO DE EMISION
    char A[5];                // AO
    char M[3];                // MES
    char D[3];                // DIA
    // Fecha Segundo Vencimiento
    char S[4];                // DIAS HASTA EL 2 VTO DESDE EL PRIMERO
    // Importe Segundo VTO
    char Q[11];               // PARTE ENTERA
    char P[6];                // PARTE DECIMAL
    char H[3];                // COEFICIENTE RECARGO ENTERO
    char J[3];                // COEFICIENTE RECARGO DECIMAL
    // Recargo 1 a sumar al importe
    char I[6];              // PARTE ENTERA
    char F[3];              // PARTE DECIMAL
    // Fecha Tercer Vencimiento
    char T[4];                // DIAS HASTA EL 3 VTO DESDE EL SEGUNDO
    char O[4];              // DIAS HASTA EL 3 VTO DESDE EL PRIMERO
    // Recargo 2 a sumar al importe
    char V[6];              // PARTE ENTERA
    char W[3];              // PARTE DECIMAL
}obtenido_cod_barra;
int ANALIZAR_CODIGO_BARRA( char *scanner_cod_barra, struct _DatosCodBarra *datos, char *mascara );
int ANALIZAR_CODIGO_BARRA_PARA_SERVICIOS_ESP( int codCobro, char *scanner_cod_barra,
                                              struct _DatosCodBarra *datos )
{
    struct   _mascara_scanner masc_scanner;
    int rta = 1;
    if( OPEN_TABLE( T_MASCARA_SCANNER, TT_ORIG, ( char* )&masc_scanner, sizeof( masc_scanner ) ) == 0 ) {
        SELECT_TABLE( T_MASCARA_SCANNER, TT_ORIG );
        SET_ORDER2( "COD_COBRO" );
        SET_WHERE( "COD_COBRO = '%i'", codCobro );
		RUN_QUERY( NO );
        while( masc_scanner.cod_cobro == codCobro && rta == 1 && !BtrvEOF() && !BOF() ) {
            rta = ANALIZAR_CODIGO_BARRA( scanner_cod_barra, datos, masc_scanner.mascara );
            datos->cod_cobro = codCobro;
            SKIP2( 1 );
        }
        CLOSE_TABLE( T_MASCARA_SCANNER, TT_ORIG );
    }
    return rta;
}
int ANALIZAR_CODIGO_BARRA( char *scanner_cod_barra, struct _DatosCodBarra *datos, char *mascara )
{
    char *letra_mascara = mascara; //= cobros.mascara_scanner;  // NEW  cobros.patron_codigo_barra
    char *letra_scanner = scanner_cod_barra;
    double primer_importe;
    double recargo1 = 0;
    double recargo2 = 0;
    int dif_dias1 = 0;
    int dif_dias2 = 0;
    int iError = 0;
    int i,seguir;
    int vencido = 0;
    datos->nro_comprobante = 0;
    datos->importe = 0;
    datos->anio_emision = 0;
    datos->cod_cobro = 0;
    datos->moneda = 0; // por defecto en pesos 0
    datos->nro_cuenta = 0;
    datos->punto_venta = 0;
    datos->letra_comp = -1;
    memset( &obtenido_cod_barra, 0, sizeof( obtenido_cod_barra ) );
    for( i = 0, seguir = 1;scanner_cod_barra[i] && seguir;i++ ) {
        if( scanner_cod_barra[i] < '0' || scanner_cod_barra[i] > '9' ) {
            scanner_cod_barra[i] = 0;
            seguir = 0;
        }
    }
    TRIM( letra_mascara );
    TRIM( letra_scanner );
    if( strlen( scanner_cod_barra ) != strlen( letra_mascara ) ) {
        iError = 1;
    }
    /* 
          si la cantidad de caracteres del codigo de barra no es igual 
          a la cantidad de caracteres de la mascara retorna error
    */
    if( strlen( letra_scanner ) != strlen( letra_mascara ) ) {
        iError = 1;
    }
    while( *letra_mascara && !iError ) {
        if( *letra_mascara >= '0' && *letra_mascara <= '9' ) {
            if( *letra_mascara != *letra_scanner ) {
                iError = 1;
                break;
            }
        }
        else {
            switch( *letra_mascara ) {
                    // Importe primer Vencimiento
                case 'E':
                    // PARTE ENTERA
                    if( strlen( obtenido_cod_barra.E ) + 1 < sizeof( obtenido_cod_barra.E ) ) {
                        obtenido_cod_barra.E[strlen( obtenido_cod_barra.E )] = *letra_scanner;
                    }
                    break;
                case 'K':
                    // PARTE DECIMAL
                    if( strlen( obtenido_cod_barra.K ) + 1 < sizeof( obtenido_cod_barra.K ) ) {
                        obtenido_cod_barra.K[strlen( obtenido_cod_barra.K )] = *letra_scanner;
                    }
                    break;
                case 'C':
                    // NRO CUENTA
                    if( strlen( obtenido_cod_barra.C ) + 1 < sizeof( obtenido_cod_barra.C ) ) {
                        obtenido_cod_barra.C[strlen( obtenido_cod_barra.C )] = *letra_scanner;
                    }
                    break;
                case 'N':
                    // NRO COMPROBANTE
                    if( strlen( obtenido_cod_barra.N ) + 1 < sizeof( obtenido_cod_barra.N ) ) {
                        obtenido_cod_barra.N[strlen( obtenido_cod_barra.N )] = *letra_scanner;
                    }
                    break;
                case 'R':
                    // MONEDA 0-PESO 1-DOLAR
                    if( strlen( obtenido_cod_barra.R ) + 1 < sizeof( obtenido_cod_barra.R ) ) {
                        obtenido_cod_barra.R[strlen( obtenido_cod_barra.R )] = *letra_scanner;
                    }
                    break;
                case 'U':
                    // CODIGO DE COBRO
                    if( strlen( obtenido_cod_barra.U ) + 1 < sizeof( obtenido_cod_barra.U ) ) {
                        obtenido_cod_barra.U[strlen( obtenido_cod_barra.U )] = *letra_scanner;
                    }
                    break;
                case 'Z':
                    // LETRA DE COMPROBANTE
                    if( strlen( obtenido_cod_barra.Z ) + 1 < sizeof( obtenido_cod_barra.Z ) ) {
                        obtenido_cod_barra.Z[strlen( obtenido_cod_barra.Z )] = *letra_scanner;
                    }
                    break;
                case 'Y':
                    // PUNTO DE VENTA
                    if( strlen( obtenido_cod_barra.Y ) + 1 < sizeof( obtenido_cod_barra.Y ) ) {
                        obtenido_cod_barra.Y[strlen( obtenido_cod_barra.Y )] = *letra_scanner;
                    }
                    break;
                case 'B':
                    // AÑO DE EMISION
                    if( strlen( obtenido_cod_barra.B ) + 1 < sizeof( obtenido_cod_barra.B ) ) {
                        obtenido_cod_barra.B[strlen( obtenido_cod_barra.B )] = *letra_scanner;
                    }
                    break;
                    // Fecha Primer Vencimiento
                case 'G':
                    // DIF. EN DIAS DESDE 1/1/1998
                    if( strlen( obtenido_cod_barra.G ) + 1 < sizeof( obtenido_cod_barra.G ) ) {
                        obtenido_cod_barra.G[strlen( obtenido_cod_barra.G )] = *letra_scanner;
                    }
                    break;
                case 'L':
                    // DIF. EN DIAS DESDE AÑO DE EMISION
                    if( strlen( obtenido_cod_barra.L ) + 1 < sizeof( obtenido_cod_barra.L ) ) {
                        obtenido_cod_barra.L[strlen( obtenido_cod_barra.L )] = *letra_scanner;
                    }
                    break;
                case 'A':
                    // AO
                    if( strlen( obtenido_cod_barra.A ) + 1 < sizeof( obtenido_cod_barra.A ) ) {
                        obtenido_cod_barra.A[strlen( obtenido_cod_barra.A )] = *letra_scanner;
                    }
                    break;
                case 'M':
                    // MES
                    if( strlen( obtenido_cod_barra.M ) + 1 < sizeof( obtenido_cod_barra.M ) ) {
                        obtenido_cod_barra.M[strlen( obtenido_cod_barra.M )] = *letra_scanner;
                    }
                    break;
                case 'D':
                    // DIA
                    if( strlen( obtenido_cod_barra.D ) + 1 < sizeof( obtenido_cod_barra.D ) ) {
                        obtenido_cod_barra.D[strlen( obtenido_cod_barra.D )] = *letra_scanner;
                    }
                    break;
                    // Fecha Segundo Vencimiento
                case 'S':
                    // DIAS
                    if( strlen( obtenido_cod_barra.S ) + 1 < sizeof( obtenido_cod_barra.S ) ) {
                        obtenido_cod_barra.S[strlen( obtenido_cod_barra.S )] = *letra_scanner;
                    }
                    break;
                    // Importe Segundo VTO
                case 'Q':
                    // PARTE ENTERA
                    if( strlen( obtenido_cod_barra.Q ) + 1 < sizeof( obtenido_cod_barra.Q ) ) {
                        obtenido_cod_barra.Q[strlen( obtenido_cod_barra.Q )] = *letra_scanner;
                    }
                    break;
                case 'P':
                    // PARTE DECIMAL
                    if( strlen( obtenido_cod_barra.P ) + 1 < sizeof( obtenido_cod_barra.P ) ) {
                        obtenido_cod_barra.P[strlen( obtenido_cod_barra.P )] = *letra_scanner;
                    }
                    break;
                case 'H':
                    // COEFICIENTE RECARGO ENTERO
                    if( strlen( obtenido_cod_barra.H ) + 1 < sizeof( obtenido_cod_barra.H ) ) {
                        obtenido_cod_barra.H[strlen( obtenido_cod_barra.H )] = *letra_scanner;
                    }
                    break;
                case 'J':
                    // COEFICIENTE RECARGO DECIMAL
                    if( strlen( obtenido_cod_barra.J ) + 1 < sizeof( obtenido_cod_barra.J ) ) {
                        obtenido_cod_barra.J[strlen( obtenido_cod_barra.J )] = *letra_scanner;
                    }
                    break;
                    //Recargo 1 a sumar al importe
                case 'I':
                    // PARTE ENTERA
                    if( strlen( obtenido_cod_barra.I ) + 1 < sizeof( obtenido_cod_barra.I ) ) {
                        obtenido_cod_barra.I[strlen( obtenido_cod_barra.I )] = *letra_scanner;
                    }
                    break;
                case 'F':
                    // PARTE DECIMAL
                    if( strlen( obtenido_cod_barra.F ) + 1 < sizeof( obtenido_cod_barra.F ) ) {
                        obtenido_cod_barra.F[strlen( obtenido_cod_barra.F )] = *letra_scanner;
                    }
                    break;
                    // Fecha Tercer Vencimiento
                case 'T':
                    // DIAS HASTA EL 3 VTO DESDE EL 2 VTO
                    if( strlen( obtenido_cod_barra.T ) + 1 < sizeof( obtenido_cod_barra.T ) ) {
                        obtenido_cod_barra.T[strlen( obtenido_cod_barra.T )] = *letra_scanner;
                    }
                    break;
                case 'O':
                    // DIAS HASTA EL 3 VTO DESDE EL 1 VTO
                    if( strlen( obtenido_cod_barra.O ) + 1 < sizeof( obtenido_cod_barra.O ) ) {
                        obtenido_cod_barra.O[strlen( obtenido_cod_barra.O )] = *letra_scanner;
                    }
                    break;
                    //Recargo 2 a sumar al importe
                case 'V':
                    // PARTE ENTERA
                    if( strlen( obtenido_cod_barra.V ) + 1 < sizeof( obtenido_cod_barra.V ) ) {
                        obtenido_cod_barra.V[strlen( obtenido_cod_barra.V )] = *letra_scanner;
                    }
                    break;
                case 'W':
                    // PARTE DECIMAL
                    if( strlen( obtenido_cod_barra.W ) + 1 < sizeof( obtenido_cod_barra.W ) ) {
                        obtenido_cod_barra.W[strlen( obtenido_cod_barra.W )] = *letra_scanner;
                    }
                    break;
            }
        };
        letra_mascara++;
        letra_scanner++;
    }
    if( !iError ) {
        // Analizar lo Obtenido
        int fecha_1_venc = 0;
        // -----------------------------------------------------
        // Primero trata de Obtener datos del comprobante
        // -----------------------------------------------------
        if( strlen( obtenido_cod_barra.N ) > 0 ) {
            datos->nro_comprobante = atol( obtenido_cod_barra.N );
        }
        if( strlen( obtenido_cod_barra.C ) > 0 ) {
            datos->nro_cuenta = atol( obtenido_cod_barra.C );
        }
        /*if(strlen(obtenido_cod_barra.U)>0)
           datos->cod_cobro = atoi(obtenido_cod_barra.U);*/
        if( strlen( obtenido_cod_barra.B ) > 0 ) {
            datos->anio_emision = atoi( obtenido_cod_barra.B );
        }
        if( strlen( obtenido_cod_barra.R ) > 0 ) {
            datos->moneda = atoi( obtenido_cod_barra.R );
        }
        if( strlen( obtenido_cod_barra.Y ) > 0 ) {
            datos->punto_venta = atoi( obtenido_cod_barra.Y );
        }
        if( strlen( obtenido_cod_barra.Z ) > 0 ) {
            datos->letra_comp = atoi( obtenido_cod_barra.Z );
        }
        // -----------------------------------------------------
        // Primero trata de Obtener el primer vto en formato 'G'
        // -----------------------------------------------------
        if( strlen( obtenido_cod_barra.G ) > 0 ) {
            fecha_1_venc = INC_DIA( FECHA( 1, 1, 98 ), atoi( obtenido_cod_barra.G ) );
        }
        else if( strlen( obtenido_cod_barra.L ) > 0 ) {
            if( strlen( obtenido_cod_barra.B ) > 0 ) {
                fecha_1_venc = INC_DIA( FECHA( 1, 1, datos->anio_emision ),
                                        atoi( obtenido_cod_barra.L ) );
                fecha_1_venc--;
            }
            else {
                return 1;
            }
        }
        else {
            int dia,mes,anio;
            if( strlen( obtenido_cod_barra.D ) || strlen( obtenido_cod_barra.M )
             || strlen( obtenido_cod_barra.A ) ) {
                dia = atoi( obtenido_cod_barra.D );
                mes = atoi( obtenido_cod_barra.M );
                if( strlen( obtenido_cod_barra.A ) == 4 ) {
                    anio = atoi( &obtenido_cod_barra.A[2] );
                }   // Ao 4 digitos
                else {
                    anio = atoi( obtenido_cod_barra.A );
                }
                fecha_1_venc = FECHA( ( char )dia, ( char )mes, anio );
            }
        }
        primer_importe = atoi( obtenido_cod_barra.E ) + ( atoi( obtenido_cod_barra.K ) / 100.0 );
        if( fecha_1_venc >= ( int )_GET_FECHA_DOS() || !fecha_1_venc ) {
            datos->importe = primer_importe;
        }
        else {
            // -----------------------------------------------------
            // Vencido controlar si existe fecha 2
            // -----------------------------------------------------
            // -----------------------------------------------------------------
            // Segundo Vencimiento por recargo
            // -----------------------------------------------------------------
            if( strlen( obtenido_cod_barra.H ) > 0 ||   // COEFICIENTE RECARGO ENTERO
               strlen( obtenido_cod_barra.J ) > 0 ) {
                // COEFICIENTE RECARGO DECIMAL
                double recargo;
                recargo = atoi( obtenido_cod_barra.H ) + atoi( obtenido_cod_barra.J ) / 100.0;
                dif_dias1 = atoi( obtenido_cod_barra.S );
                datos->importe = primer_importe + ( primer_importe * ( recargo )*dif_dias1 )
                               / 36500.0;
            }
            else if( strlen( obtenido_cod_barra.Q )
                 ||  // case 'Q': // Importe Segundo VTO// PARTE ENTERA
                     strlen( obtenido_cod_barra.P ) ) {
                // case 'P': // Importe Segundo VTO // PARTE DECIMAL
                datos->importe = atoi( obtenido_cod_barra.Q ) + atoi( obtenido_cod_barra.P )
                               / 100.0;
            }
            else if( strlen( obtenido_cod_barra.I ) > 0 || strlen( obtenido_cod_barra.F ) > 0 ) {
                recargo1 = atoi( obtenido_cod_barra.I ) + atoi( obtenido_cod_barra.F ) / 100.0;
                datos->importe = primer_importe + recargo1;
            }
            else {
                datos->importe = primer_importe;
            }
            // -----------------------------------------------------------------
            // controla que este entre fecha vencimiento 1 y fecha vencimiento 2
            // -----------------------------------------------------------------
            if( strlen( obtenido_cod_barra.S ) > 0 ) {
                dif_dias1 = atoi( obtenido_cod_barra.S );
            }
            else {
                dif_dias1 = 0;
            }
            if( INC_DIA( fecha_1_venc, dif_dias1 ) < ( int )_GET_FECHA_DOS() ) {
                // -----------------------------------------------------
                // Vencido controlar si existe fecha 3
                // -----------------------------------------------------
                if( strlen( obtenido_cod_barra.T ) > 0 ) {
                    dif_dias2 = atoi( obtenido_cod_barra.T ) + dif_dias1;
                }
                else if( strlen( obtenido_cod_barra.O ) > 0 ) {
                    dif_dias2 = atoi( obtenido_cod_barra.O );
                }
                else {
                    dif_dias2 = 0;
                }
                if( INC_DIA( fecha_1_venc, dif_dias2 ) < ( int )_GET_FECHA_DOS() ) {
                    vencido = 1;
                }
                else if( strlen( obtenido_cod_barra.V ) > 0 || strlen( obtenido_cod_barra.W ) > 0 ) {
                    recargo1 = atoi( obtenido_cod_barra.V ) + atoi( obtenido_cod_barra.W ) / 100.0;
                    datos->importe = primer_importe + recargo1;
                }
                else {
                    return( 1 );
                }
            }
            if( vencido ) {
                //MENSAJE_STRING( S_COMPROBANTE_VENCIDO );
                //BORRAR_MENSAJE(  );
                iError = 2;
                /*if( cobros.permite_pago_venci != 'S' ) 
                        {
                               datos->nro_cuenta = -1;
                               datos->nro_comprobante = -1;
                               datos->importe = 0;
                               iError = 2;
                           }*/
            }
        }
    }
    /*if( iError == 1 ) {
        MENSAJE_STRING( S_CODIGO_INVALIDO );
        BORRAR_MENSAJE(  );
    }*/
    return ( iError );
}

