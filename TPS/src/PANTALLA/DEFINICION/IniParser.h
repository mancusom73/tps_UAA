#pragma pack (push,8) 
#ifndef __INI_PARSER_H
#define __INI_PARSER_H

#include <fstream>
#include "IniEntity.h"

class IniParser
{
  private:
    ifstream file_stream;
    string filename;
    char comentario;
    char delimitador[100];
    char *current_char;

    void parsSpaces();
    string parsString( char stop_char );
    string parsToken( char stop_char );
    bool parsChar( char stop_char );
    int parsInteger( char stop_char );
    double parsDouble( char stop_char );
    bool isDelimitador( char d );
  public:
    IniParser( const string &filename );
    bool pars( IniFile *ini );
};
#endif

#pragma pack (pop) 
