#include <DbFileUtils.h>
#include <DB.h> 


DbFileUtils::DbFileUtils( int tNumber, int tType, char* pathDatabase ) {

    char tmp[255];

    tableNumber = tNumber;
    tableType = tType;
    GET_TABLE_NAME( tNumber, tType, tmp, sizeof( tmp ) );
    tableName = tmp;
    tableName.LowerCase();
    backName = tableName;
    backName.Append("_backr");
    pathMysql = pathDatabase;
}

int DbFileUtils::Rename( wxString newFileName, wxString oldFileName, wxString backFileName ) {

    bool replaced = false, backuped = false;
    wxString tempFile;

    if( wxFileExists( newFileName ) ) {
        //La backup, no nos sirve
        wxRemoveFile( backFileName );
        //Se crea backup de la tabla a reemplazar
        if( wxCopyFile( newFileName, backFileName, true ) ) {
            backuped = true;
            //Se borra la tabla a reemplazar
            if( wxRemoveFile( newFileName ) ) {
                //Se reemplaza por la importada
                replaced = wxRenameFile( oldFileName, newFileName );
            //No se pudo borrar el destino
            } else {
                wxRemoveFile( backFileName );
                msgErr.Printf( "Error: borrando %s", newFileName );
            }
        //No se puedo crear el backup
        } else {
            msgErr.Printf("Error: creando backup %s -> %s", newFileName, backFileName );
        }
    //Si no existe la tabla a reemplazar.
    } else {
        //No hay backup porque no hay nada q deshacer
        backuped = false;
        replaced = wxRenameFile( oldFileName, newFileName );
    }

    if( !replaced ) {
        if( backuped && !wxFileExists( newFileName ) ) {
            wxRenameFile( backFileName, newFileName );
        }
        return 1;
    } else  {
        return 0;
    }
}

int DbFileUtils::Replace( int tableDest, int tableDestType ) {

    wxString tableDestMyd, tableDestMyi, tableDestFrm, tableMyd, tableMyi, tableFrm,
        tableBackMyd, tableBackMyi, tableBackFrm, destName;
    char tmpStr[60];
    int err = 0;

    CLOSE_TABLE( tableNumber, tableType );
    CLOSE_TABLE( tableDest, tableDestType );

    GET_TABLE_NAME( tableDest, tableDestType, tmpStr, sizeof( tmpStr ) );
    destName = tmpStr;
    destName.LowerCase();
    tableDestMyd = pathMysql+destName+".MYD";
    tableDestMyi = pathMysql+destName+".MYI";
    tableDestFrm = pathMysql+destName+".frm";

    tableBackMyd = pathMysql+backName+".MYD";
    tableBackMyi = pathMysql+backName+".MYI";
    tableBackFrm = pathMysql+backName+".frm";

    tableMyd = pathMysql+tableName+".MYD";
    tableMyi = pathMysql+tableName+".MYI";
    tableFrm = pathMysql+tableName+".frm";

    msgErr.Printf("Reemplazo de tabla: %s -> %s OK", tableName, destName );        
    
    //Si las tablas de origen existen
    if( wxFileExists( tableMyd ) && wxFileExists( tableMyi ) ) {
        if( ( err = Rename( tableDestMyd, tableMyd, tableBackMyd ) ) == 0 ) {
            if( ( err = Rename( tableDestMyi, tableMyi, tableBackMyi ) ) == 0 ) {
                Rename( tableDestFrm, tableFrm, tableBackFrm );
            }
        }
    //Si no existen las tablas import
    } else {
        msgErr.Printf("Error: No se encontro la tabla importada %s, %s", tableMyd, tableMyi );
        err = 1;
    }
    //Si como resultado no esta la tabla requerida
    if( !wxFileExists( tableDestMyd ) || !wxFileExists( tableDestMyi ) || !wxFileExists( tableDestFrm )) {
        msgErr.Printf("Error: importando la tabla %d, no estan disponibles todos los archivos que la componen", tableDest );
        err = 1;
    }
    
    //GRABAR_LOG_SISTEMA( msgErr );

    if( err == 0 ) {
        wxRemoveFile( tableBackMyd );
        wxRemoveFile( tableBackMyi );
        wxRemoveFile( tableBackFrm );
        return err;
    }
    return err;
}

wxString DbFileUtils::getError() {
    return msgErr;
}

