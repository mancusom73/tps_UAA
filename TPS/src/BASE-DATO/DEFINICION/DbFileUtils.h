#pragma pack (push,8) 
#pragma once
#include <wx/string.h>
#include <wx/filefn.h>

class DbFileUtils {

private:
    wxString tableName;
    wxString backName;
    wxString msgErr;
    wxString pathMysql;
    int tableNumber;
    int tableType;

public:    
    DbFileUtils( int tNumber, int tType, char *pathDatabase );
    virtual ~DbFileUtils(){};
    int Rename( wxString newFileName, wxString oldFileName, wxString backFileName );
    int Replace( int tableDest, int tableDestType );
    wxString getError();

};
#pragma pack (pop)