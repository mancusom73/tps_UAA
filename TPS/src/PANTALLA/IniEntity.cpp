
#include "IniEntity.h"
#include <fstream>

IniEntity::IniEntity()
{
    group = "";
    attribute = "";
    memset( &value, 0x00, sizeof( value ) );
}


IniEntity::IniEntity( const string &g, int i )
{
    attribute = g;
    value.Integer = i;
}


IniEntity::IniEntity( const string &g, double d )
{
    attribute = g;
    value.Double = d;
}


IniEntity::IniEntity( const string &g, const string &s )
{
    attribute = g;
    strncpy( value.String, s.c_str(), sizeof( value.String ) );
}


void IniEntity::setValue( int i )
{
    value.Integer = i;
}


void IniEntity::setValue( double d )
{
    value.Double = d;
}


void IniEntity::setValue( const string &s )
{
    strncpy( value.String, s.c_str(), sizeof( value.String ) );
}


const IniEntity & IniFile::get( const string group, const string &index )
{
    for( unsigned int i = 0;i < attributes.size();i++ ) {
        string s = attributes[i].attribute;
        if( group == attributes[i].group ) {
            if( attributes[i].attribute == index ) {
                return attributes[i];
            }
        }
        s = "";
    }
    return attributes[0];
}


const IniEntity & IniFile::get( const string &index )
{
    for( unsigned int i = 0;i < attributes.size();i++ ) {
        if( attributes[i].attribute == index ) {
            return attributes[i];
        }
    }
    return attributes[0];
}


const IniEntity & IniFile::get( int index )
{
    return attributes[index];
}


void IniFile::add( const IniEntity &e )
{
    attributes.push_back( e );
    /*
    int i = attributes.size();
    char buffer[1000];
    sprintf(buffer,"%s",attributes[i-1].attribute);
    */
}


int IniFile::getIndex( const string &group, const string &index )
{
    for( unsigned int i = 0;i < attributes.size();i++ ) {
        if( group == attributes[i].group ) {
            if( attributes[i].attribute == index ) {
                return i;
            }
        }
    }
    return -1;
}


void IniFile::save( const string &filename )
{
    string group = "";
    fstream stream;
    stream.open( filename.c_str(), ios::app | ios::out );

    if( stream.is_open() ) {
        for( unsigned int i = 0;i < attributes.size();i++ ) {
            if( group != attributes[i].group ) {
                stream << endl;
                stream << "[" << attributes[i].group << "]";
                stream << endl;
                group = attributes[i].group;
            }
            stream << attributes[i].attribute.c_str();
            stream << " = ";
            switch( attributes[i].type ) {
                case IT_INTEGER:
                    stream << attributes[i].value.Integer;
                    break;
                case IT_DOUBLE:
                    stream << attributes[i].value.Double;
                    break;
                case IT_STRING:
                    stream << attributes[i].value.String;
                    break;
            }
            stream << endl;
        }
        stream.flush();
        stream.close();
    }
}


void IniFile::setValue( int index, const string s )
{
    if( index >= 0 ) {
        attributes[index].setValue( s );
    }
}


void IniFile::setValue( int index, int s )
{
    if( index >= 0 ) {
        attributes[index].setValue( s );
    }
}


void IniFile::setValue( int index, double s )
{
    if( index >= 0 ) {
        attributes[index].setValue( s );
    }
}
