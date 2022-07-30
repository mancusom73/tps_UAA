#pragma pack (push,8) 
#ifndef __INI_ENTITY_H
#define __INI_ENTITY_H

#include <string>
#include <vector>
using namespace std;

enum INI_TYPE { IT_INTEGER, IT_DOUBLE, IT_STRING };

union IniValue
{
    int Integer;
    double Double;
    char String[1024];
};

class IniEntity
{
  public:
    IniValue value;
    string attribute;
    string group;
    INI_TYPE type;
    IniEntity();
    IniEntity( const string &g, int i );
    IniEntity( const string &g, double d );
    IniEntity( const string &g, const string &s );

    void setValue( int i );
    void setValue( double d );
    void setValue( const string &s );
};

class IniFile
{
  private:
    vector<IniEntity> attributes;
  public:
    const IniEntity &get( const string group, const string &index );
    const IniEntity &get( const string &index );
    const IniEntity &get( int index );
    int getIndex( const string &group, const string &index );
    void add( const IniEntity &e );
    void save( const string &filename );
    void setValue( int index, int s );
    void setValue( int index, double s );
    void setValue( int index, const string s );
};
#endif

#pragma pack (pop) 
