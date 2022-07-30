#include "IniParser.h"

IniParser::IniParser( const string &filename )
{
    this->filename = filename;
    memset( delimitador, 0x00, sizeof( delimitador ) );

    delimitador[0] = ' ';

    comentario = ';';
}


string IniParser::parsString( char stop_char )
{
    char buffer[1024];
    int i = 0;
    while( *current_char && *current_char != stop_char && *current_char != comentario ) {
        buffer[i] = *current_char;
        i++;
        current_char++;
    }
    buffer[i] = 0x00;
    return buffer;
}


string IniParser::parsToken( char stop_char )
{
    char buffer[1024];
    string s = parsString( stop_char );
    int j = 0;
    buffer[0] = 0;
    for( int i = 0;i < (int)s.length();i++ ) {
        if( !isDelimitador( s.c_str()[i] ) ) {
            buffer[j++] = s.c_str()[i];
        }
    }
    buffer[j] = 0x00;
    return buffer;
}


int IniParser::parsInteger( char stop_char )
{
    //char buffer[1024];
    string s = parsString( stop_char );
    return atoi( s.c_str() );
}


double IniParser::parsDouble( char stop_char )
{
    //char buffer[1024];
    string s = parsString( stop_char );
    return atof( s.c_str() );
}


bool IniParser::parsChar( char stop_char )
{
    while( *current_char && *current_char != stop_char && *current_char != comentario ) {
        current_char++;
    }
    return *current_char == stop_char;
}


void IniParser::parsSpaces()
{
    int i = 0;
    while( *current_char && *current_char == ' ' ) {
        current_char++;
    }
}


bool IniParser::isDelimitador( char d )
{
    int i = 0;
    while( delimitador[i] != 0x00 ) {
        if( delimitador[i] == d ) {
            return true;
        }
        i++;
    }
    return false;
}


bool IniParser::pars( IniFile *ini )
{
    char buffer[1024];
    bool rta = false;
    string group;
    file_stream.open( filename.c_str() );
    if( file_stream.is_open() ) {
        rta = true;
        while( !file_stream.eof() && rta == true ) {
            memset( buffer, 0x00, 1024 );
            file_stream.getline( buffer, 1024 );
            //printf("%s\n", buffer);
            current_char = buffer;
            if( parsChar( '[' ) ) {
                current_char++;
                group = parsToken( ']' );
            }
            else {
                current_char = buffer;
                string attribute = parsToken( '=' );
                current_char++;
                parsSpaces();
                string value;
                if( attribute.length() > 0 ) {
                    int index = ini->getIndex( group, attribute );
                    if( index >= 0 ) {
                        switch( ini->get( index ).type ) {
                            case IT_INTEGER:
                                ini->setValue( index, parsInteger( 0x00 ) );
                                break;
                            case IT_DOUBLE:
                                ini->setValue( index, parsDouble( 0x00 ) );
                                break;
                            case IT_STRING:
                                value = parsString( 0x00 );
                                if( value.length() > 0 ) {
                                    ini->setValue( index, value );
                                }
                                break;
                        }
                    }
                    else {
                        char str[1024];
                        _snprintf(str, sizeof(str)-1, "%s", attribute.c_str() );
                        printf( "ATTRIBUTE NOT FOUND: '%s'\n", attribute.c_str() );

                        rta = false;
                    }
                }
            }
        }
    }
    return rta;
}
