//#pragma pack(8)
#include "FileVerif.h"


extern CDesktop *properties;

FileVerif::~FileVerif()
{
}

/* Verifica que el checksum definido en FileVerif.h corresponda
 * al calculado sobre LogoInvel.gif y que el Logo se ubique
 * en la parte superior de la pantalla.
 */
bool FileVerif::VerifLogo( bool mostrar, bool cerrar, const string &skin, const string &pantalla )
{
    CWindow *ventana;
    CComponentRef *logo;
    CComponent *logoComponent;
    CImageRef *logoImageRef;
    CImage *logoImage;

    string filename;
    bool result = true;
    int tam_x = 0, x = 0, y = 0, x_icon = 0, y_icon = 0;
    char mensaje[200];
    static bool comprobado = false;

    ventana = properties->GetWindows()->GetWindowByName( pantalla );
    if( ventana ) {
        tam_x = ventana->GetSize()->GetWidth();
        logo = ventana->GetComponents()->GetComponentRefByName( "LogoInvel" );
    }
    if( logo ) {
        x = logo->GetPosx();
        y = logo->GetPosy();
        logoComponent = properties->GetRepository()->GetComponentByName( logo->GetID() );
    }
    if( logoComponent ) {
        logoImageRef = logoComponent->GetIconImageRef();
    }
    if( logoImageRef ) {
        x_icon = logoImageRef->GetPosx();
        y_icon = logoImageRef->GetPosy();
        logoImage = properties->GetImages(
        )->GetSkinByName( skin )->GetImageByName( logoImageRef->GetID() );
    }
    if( logoImage ) {
        filename = logoImage->GetFilename();
    }

    if( x_icon || y_icon ) {
        sprintf( mensaje, "Configuracion de pantalla:\nEn \"%s\", X=%d e Y=%d deben valer cero",
                 logoImageRef->GetID().c_str(), x_icon, y_icon );
        result = false; //estas coordenadas deben ser cero
    }
    else if( ( y <= LOGO_LIM_Y_MIN ) || ( y >= LOGO_LIM_Y_MAX ) || ( x <= LOGO_LIM_X_MIN )
          || ( x >= ( tam_x - LOGO_INVEL_WIDTH ) ) ) {
        sprintf(
        mensaje,
        "Configuracion de pantalla:\nLos valores de X=%d e Y=%d en \"%s\" de la pantalla \"%s\" son incorrectos.\nDeben cumplir (%d < X < %d),(%d < Y < %d)",
        x,
        y,
        logo->GetID().c_str(),
        pantalla.c_str(),
        LOGO_LIM_X_MIN, tam_x - LOGO_INVEL_WIDTH, LOGO_LIM_Y_MIN, LOGO_LIM_Y_MAX );
        result = false;
    }
    else {
        //calculamos el hash una sola vez en la primer pantalla que se cargue
        if( !comprobado ) {
            if( !( result = Verif( filename, LOGO_INVEL_SHA1 ) ) ) {
                sprintf( mensaje, "El archivo de imagen \"%s\" no existe o ha sido alterado",
                         filename.c_str() );
            }
            comprobado = true;
        }
        else {
            result = true;
        }
    }

    if( !result ) {
        if( mostrar ) {
            //ShowCursor( true );
            //sprintf(mensaje, "\nPresione Aceptar para %s",filename.c_str(),pantalla.c_str(), LOGO_LIM_Y_MIN, LOGO_LIM_Y_MAX, (cerrar)?"cerrar la aplicaci?n":"continuar");
            //MessageBox( NULL, mensaje, "Aviso", MB_OK );
        }
        if( cerrar ) {
            exit( 0 );
        }

        return( false );
    }
    return( true );
}

/* Funcionamiento: Primero se calcula el hash SHA1 sobre el archivo
 * cuyo nombre es pasado por parametro. El resultado es un array de 20 
 * bytes. Luego se calcula el SHA1 de este array, y el resultado
 * del calculo es tomado en un string en representacion Hexadecimal.
 * Este ultimo string es el que se compara con el pasado en el 
 * parametro sha.
 *
 * Para averiguar el "sha" que se le debe pasar de parametro, por ejemplo
 * se puede utilizar el prograna "HashCalc". Calcular el SHA1 del archivo y
 * luego calcularle SHA1 al resultado, tomandolo como un numero Hexadecimal.
 */
bool FileVerif::Verif( const string &archivo, const string &sha )
{
    uByte shaResult1[20];
    string shaResult2;

    if( ComputeInputFile( archivo.c_str() ) ) {
        GetByteResult( shaResult1 );
        Start();
        ComputeInputBuffer( shaResult1, 20 );
        Stop();
        shaResult2 = GetHexResult();

        if( strcmp( shaResult2.c_str(), sha.c_str() ) != 0 ) {
            return( false );
        }
    }
    else {
        return( false );
    }
    return ( true );
}
