#pragma pack (push,8) 
#ifndef __FILE_VERIF_H
#define __FILE_VERIF_H

#include "CSHADigest.h"
#include "CScreencfg.h"
//todas las letras mayusculas!
#define LOGO_INVEL_SHA1   "ABEEC663DB8916BD04A99BCA9E2C61AE0FB6332C"
#define LOGO_LIM_X_MIN    0
#define LOGO_LIM_Y_MIN    0
#define LOGO_LIM_Y_MAX    15
#define LOGO_INVEL_WIDTH  110


class FileVerif : public CSHADigest
{
  public:

    FileVerif()
    {
    };
    ~FileVerif();
    bool VerifLogo( bool mostrar, bool cerrar, const string &skin, const string &pantalla );
    bool Verif( const string &archivo, const string &sha );
};

#endif
#pragma pack (pop) 
