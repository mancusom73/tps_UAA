/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <stdlib.h>
#include <string.h>
#include <memory.h>
int L_VALID_01( char *num_tarjeta );
int L_VALID_02( char *num_tarjeta );
int L_VALID_03( char *num_tarjeta );
int L_VALID_04( char *num_tarjeta );
int L_VALID_05( char *num_tarjeta );
int L_VALID_06( char *num_tarjeta );
int L_VALID_07( char *num_tarjeta );
int L_VALID_08( char *num_tarjeta );
int L_VALID_09( char *num_tarjeta );
int L_VALID_10( char *num_tarjeta );
int L_VALID_11( char *num_tarjeta );
/*****************************************************************************/
int EJECUTAR_L_VALID( char *cadena, int f_valid )
/*****************************************************************************/
{
    int rta = 0;
    switch( f_valid ) {
        case 0:
            /*--- Sin validacion ---*/
            rta = 1;
            break;
        case 1:
            /*--- Norma ISO 7812-1985(E) ---*/
            rta = L_VALID_01( cadena );
            break;
        case 2:
            /*--- MODULO 9713 ---*/
            rta = L_VALID_02( cadena );
            break;
        case 3:
            /*--- MODULO 1212 ---*/
            rta = L_VALID_03( cadena );
            break;
        case 4:
            /*--- MODULO 4326 ---*/
            rta = L_VALID_04( cadena );
            break;
        case 5:
            /*--- MODULO 11 ---*/
            rta = L_VALID_05( cadena );
            break;
        case 6:
            /*--- MODULO 3971 ---*/
            rta = L_VALID_06( cadena );
            break;
        case 7:
            /*--- MODULO 11 (N§ cuenta socio Argencard) ---*/
            rta = L_VALID_07( cadena );
            break;
        case 8:
            /*--- MODULO 11 (N§ vendedor Falabella) ---*/
            rta = L_VALID_08( cadena );
            break;
        case 9:
            /*--- MODULO 11 (N§ tarjeta CMR) ---*/
            rta = L_VALID_09( cadena );
            break;
        case 10:
            /*--- COBRO CFE ---*/
            rta = L_VALID_10( cadena );
            break;
		case 11:
            /*--- COBRO TELMEX ---*/
            rta = L_VALID_11( cadena );
            break;
	}
    return ( rta );
}
/***************************************************************************/
/*                         Norma ISO 7812-1985(E)                          */
/***************************************************************************/
int L_VALID_01( char *num_tarjeta )
/***************************************************************************/
{
    int i,cant,rta;
    char array1[15],d_verif,suma,suma_mayor;
    cant = strlen( num_tarjeta );
    if( cant <= 16 ) {
        /*--- Cargo los digitos de la tarjeta en binario en el array 1 ---*/
        /*--- 1 de por medio se multiplica el digito por 2             ---*/
        for( i = 0;i < cant - 1;i++ ) {
            array1[i] = num_tarjeta[cant - i - 2] - '0';
            if( !( i % 2 ) ) {
                array1[i] *= 2;
            }
        }
        d_verif = num_tarjeta[cant - 1] - '0';
        /*--- Sumo los digitos descomponiendo los numeros > 9 en dos digitos ---*/
        for( i = 0, suma = 0;i < cant - 1;i++ ) {
            if( array1[i] < 10 ) {
                suma += array1[i];
            }
            else {
                suma += array1[i] / 10;
                suma += array1[i] % 10;
            }
        }
        /*--- Obtengo el primer n£mero multiplo de 10 > o = a suma ----*/
        suma_mayor = ( suma % 10 ) ? suma + ( 10 - ( suma % 10 ) ) : suma;
        /*--- Comparo la diferencia entre suma_mayor y suma con el digito verificador ---*/
        if( ( suma_mayor - suma ) != d_verif ) {
            rta = 0;
        }
        else {
            rta = 1;
        }
    }
    else {
        rta = 0;
    }
    return ( rta );
}
/***************************************************************************/
/*                              MODULO 9713                                */
/***************************************************************************/
int L_VALID_02( char *num_tarjeta )
/***************************************************************************/
{
    int i,cant,rta;
    char d_verif,suma,suma_mayor,digito;
    char _9713[4] = {9,7,1,3};
    cant = 13;
    suma = 0;
    if( cant <= 16 ) {
        /*--- Se multiplican los digitos por [9713] y se suman ---*/
        for( i = 0;i < cant - 1;i++ ) {
            digito = num_tarjeta[i] - '0';
            digito = ( digito * _9713[i % 4] ) % 10;
            suma += digito;
        }
        d_verif = num_tarjeta[cant - 1] - '0';
        /*--- Obtengo el primer n£mero multiplo de 10 > o = a suma ----*/
        suma_mayor = ( suma % 10 ) ? suma + ( 10 - ( suma % 10 ) ) : suma;
        /*--- Comparo la diferencia entre suma_mayor y suma con el digito verificador ---*/
        if( ( suma_mayor - suma ) != d_verif ) {
            rta = 0;
        }
        else {
            rta = 1;
        }
    }
    else {
        rta = 0;
    }
    return ( rta );
}
/***************************************************************************/
/*                              MODULO 1212                                */
/***************************************************************************/
int L_VALID_03( char *num_tarjeta )
/***************************************************************************/
{
    int i,cant,rta,suma;
    char d_verif,suma_mayor,digito,digito1;
    char _1212[4] = {1,2,1,2};
    cant = strlen( num_tarjeta );
    suma = 0;
    if( cant <= 16 ) {
        /*--- Se multiplican los digitos por [2121] y se suman ---*/
        for( i = 0;i < cant - 1;i++ ) {
            digito = num_tarjeta[i] - '0';
            digito1 = ( digito * _1212[i % 4] ) / 10;
            suma += digito1;
            digito1 = ( digito * _1212[i % 4] ) % 10;
            suma += digito1;
        }
        d_verif = num_tarjeta[cant - 1] - '0';
        /*--- Obtengo el primer n£mero multiplo de 10 > o = a suma ----*/
        suma_mayor = ( suma % 10 ) ? suma + ( 10 - ( suma % 10 ) ) : suma;
        /*--- Comparo la diferencia entre suma_mayor y suma con el digito verificador ---*/
        if( ( suma_mayor - suma ) != d_verif ) {
            rta = 0;
        }
        else {
            rta = 1;
        }
    }
    else {
        rta = 0;
    }
    return ( rta );
}
/***************************************************************************/
/*                              MODULO 4326                                */
/***************************************************************************/
int L_VALID_04( char *num_tarjeta )
/***************************************************************************/
{
    int i,cant,rta,suma;
    char d_verif,digito,_d_verif;
    char _4326[8] = {4,3,2,6,5,4,3,2};
    suma = 0;
    cant = 9;
    if( cant <= 16 ) {
        /*--- Se multiplican los digitos por [2121] y se suman ---*/
        for( i = 0;i < cant - 1;i++ ) {
            digito = num_tarjeta[i + 6] - '0';
            suma += ( digito * _4326[i] );
        }
        d_verif = num_tarjeta[cant - 1 + 6] - '0';
        /*--- Obtengo el primer n£mero multiplo de 10 > o = a suma ----*/
        if( ( suma % 11 ) == 10 ) {
            _d_verif = ( suma / 11 ) % 10;
        }
        else {
            _d_verif = ( suma % 11 );
        }
        /*--- Comparo la diferencia entre suma_mayor y suma con el digito verificador ---*/
        if( _d_verif != d_verif ) {
            rta = 0;
        }
        else {
            rta = 1;
        }
    }
    else {
        rta = 0;
    }
    return ( rta );
}
/***************************************************************************/
/*                              MODULO 11                                  */
/***************************************************************************/
int L_VALID_05( char *num_tarjeta )
/***************************************************************************/
{
    long suma,coef;
    int i,cant,rta,resto;
    char d_verif,digito;
    suma = 0;
    cant = strlen( num_tarjeta );
    if( cant <= 16 ) {
        /*--- Obtenemos la potencia de 2 ----*/
        coef = 1;
        coef = ( coef << ( cant - 1 ) );
        /*--- Se multiplican los digitos por potencias de 2 y se suman ---*/
        for( i = 0;i < cant - 1;i++ ) {
            digito = num_tarjeta[i] - '0';
            suma += digito * coef;
            coef = coef >> 1;
        }
        d_verif = num_tarjeta[cant - 1] - '0';
        /*--- Obtengo el resto de suma / 11 ----*/
        resto = ( int )( suma % 11 );
        if( resto > 9 || resto != d_verif ) {
            rta = 0;
        }
        else {
            rta = 1;
        }
    }
    else {
        rta = 0;
    }
    return ( rta );
}
/***************************************************************************/
/*                              MODULO 1397                                */
/***************************************************************************/
int L_VALID_06( char *num_tarjeta )
/***************************************************************************/
{
    int i,cant,rta;
    char d_verif,suma,digito,_num[20];
    char _1397[4] = {1,3,9,7};
    cant = 14;
    memcpy( _num, &num_tarjeta[3], 12 );
    memcpy( &_num[12], "11", 2 );
    suma = 0;
    if( cant <= 16 ) {
        /*--- Se multiplican los digitos por [9713] y se suman ---*/
        for( i = 0;i < cant;i++ ) {
            digito = _num[i] - '0';
            digito = ( digito * _1397[i % 4] ) % 10;
            suma += digito;
        }
        d_verif = num_tarjeta[cant - 2 + 3] - '0';
        suma = suma % 10;
        /*--- Comparo la diferencia entre suma_mayor y suma con el digito verificador ---*/
        if( ( ( 10 - suma ) % 10 ) != d_verif ) {
            rta = 0;
        }
        else {
            rta = 1;
        }
    }
    else {
        rta = 0;
    }
    return ( rta );
}
/***************************************************************************/
/*               MODULO 11  (N§ cuenta socio Argencard)                    */
/***************************************************************************/
int L_VALID_07( char *num_tarjeta )
/***************************************************************************/
{
    long suma;
    int i,cant,rta,resto;
    char d_verif,digito;
    suma = 0;
    cant = strlen( num_tarjeta );
    if( cant <= 16 ) {
        /*--- Se multiplican los digitos por la posicion y se suman ---*/
        for( i = 0;i < cant - 1;i++ ) {
            digito = num_tarjeta[i] - '0';
            suma += digito * ( i + 1 );
        }
        d_verif = num_tarjeta[cant - 1] - '0';
        /*--- Obtengo el resto de suma / 11 ----*/
        resto = ( int )( suma % 11 );
        if( resto == 10 ) {
            resto = 1;
        }
        rta = ( resto != d_verif ) ? 0 : 1;
    }
    else {
        rta = 0;
    }
    return ( rta );
}
/***************************************************************************/
/*               MODULO 11  (N§ vendedor FALABELLA)                        */
/***************************************************************************/
int L_VALID_08( char *nro )
/***************************************************************************/
{
    long suma = 0;
    int h,l,resto,dv;
    l = strlen( nro );
    /*--- Se multiplican los digitos por la posicion y se suman ---*/
    for( h = 0;h < l - 1;h++ ) {
        suma += ( nro[h] - '0' ) * ( l - h );
    }
    dv = nro[l - 1] - '0';
    /*--- Obtengo el resto de suma / 11 ----*/
    resto = 11 - ( suma % 11 );
    if( resto > 9 ) {
        resto -= 10;
    }
    return ( resto == dv );
}
/***************************************************************************/
/*                MODULO 11  (N§ tarjeta CMR)                              */
/***************************************************************************/
int L_VALID_09( char *nro )
/***************************************************************************/
{
    long suma = 0;
    int h,l,resto,dv;
    int factor[] = {2,3,4,5,6,7,2,3,4,5,6,7};
    l = strlen( nro );
    /*--- Se multiplican los digitos por un factor y se suman ---*/
    for( h = 0;h < l - 2;h++ ) {
        suma += ( nro[l - h - 3] - '0' ) * factor[h];
    }
    dv = atoi( &nro[l - 2] );
    /*--- Obtengo el resto de suma / 11 ----*/
    resto = 11 - ( suma % 11 );
    if( resto == 11 ) {
        resto = 0;
    }
    else if( resto == 10 ) {
        resto = 11;
    }
    return ( resto == dv );
}
/***************************************************************************/
/*                              COBRO CFE                                  */
/***************************************************************************/
int L_VALID_10( char *nro )
/***************************************************************************/
{
    int     suma = 0;
    int           h, l, j, resto, dv, l_factor;
    int           factor[] = {6,5,4,3,2,7};
    l = strlen( nro );
    l_factor = sizeof( factor ) / sizeof( int );
    /*--- Se multiplican los digitos por un factor y se suman ---*/
    for( h = 0; h < l - 1; h++ ) {
            j = ( h < l_factor ) ? h : h % l_factor;
        suma += ( int )( nro[h] - '0' ) * factor[j];
    }
    dv = atoi( &nro[l - 1] );
    /*--- Obtengo el resto de suma / 11 ----*/
    resto = ( suma % 11 );
	if( resto > 9 ) {
        resto -= 10;
    }
    return ( resto == dv );
}
/***************************************************************************/
/*                           COBRO TELMEX                                  */
/***************************************************************************/
int L_VALID_11( char *nro )
/***************************************************************************/
{
    int  	suma = 0;
    int		h, l, j, resto, dv, l_factor;
    int		factor[] = {1,3,7};
    l = strlen( nro );
	l_factor = sizeof( factor ) / sizeof( int );
    /*--- Se multiplican los digitos por un factor y se suman ---*/
    for( h = 0; h < l - 2; h++ ) {
		j = ( h < l_factor ) ? h : h % l_factor;
        suma += ( int )( nro[h] - '0' ) * factor[j];
    }
    dv = atoi( &nro[l - 2] );
    /*--- Obtengo el resto de suma / 9 y le sumo 1 ----*/
    resto = ( suma % 9 ) + 1;
    return ( resto == dv );
}

