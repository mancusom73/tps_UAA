/*******************************************************************************
*   Sistema: TPV 1000 - IV
*
*   Clases      :   DbScript
*   Descripcion :   Manejador de script de modificacion de base de datos
*
*   Autor
*   -----               
*   Forray Juan Pablo
********************************************************************************/

#include <dbscript.h>
#include <dbtables.h>
#include <db.h>


int DbScript :: VerifScripts(ODBC *poODBCConect)
{
    wxString mConsulta;
    wxString mPathFile;
    wxString mFileName;
    int rta = 0;

    wxDir dir(PATH_DBSCRIPT);
    if ( !dir.IsOpened() )
    {
        /*si no exite el path PATH_DBSCRIPT*/
        mStrError.Printf("No existe el directorio: %s",PATH_DBSCRIPT);
        return 1;
    }
    //bool cont = dir.GetFirst(&mFileName,"",wxDIR_FILES);
    wxArrayString mList;
    int cont = 0;
    int cantTotal = 0;
    wxString mfile;
    cantTotal = (int) dir.GetAllFiles(PATH_DBSCRIPT,&mList,_T(""),wxDIR_FILES);
    mList.Sort(false);
    /*while( cont < cantTotal){
    //bool cont = dir.GetFirst(&mFileName,_T("*"),wxDIR_FILES | wxDIR_DOTDOT);
    	if(mList.Item(cont).Len() > 11){
    		
    		mfile.sprintf("%s",mList.Item(cont).substr(mList.Item(cont).Len() - 11,11).c_str());
    	}
    	printf("%s\n", mfile.c_str());
    	cont ++;
    //cont = dir.GetFirst(&mFileName,_T("*"),wxDIR_FILES | wxDIR_DOTDOT);
    }*/
    
    while ( cont < cantTotal) //recorre todos los archivos
    {
    	if(mList.Item(cont).Len() > 11){
    		
    		mFileName.sprintf("%s",mList.Item(cont).substr(mList.Item(cont).Len() - 11,11).c_str());
    	}
        /*Controla que el nombre del archivo tenga 11 caracteres formato: aaaammdd_xx*/
        if(mFileName.Len() == 11){ 
            mConsulta.Printf("select * from script_aplicados where script = '%s'",mFileName.c_str());
            poODBCConect->getDb(ODBC_LOCAL)->ExecSql(mConsulta.c_str());
            if(poODBCConect->getDb(ODBC_LOCAL)->GetNext()){
                /*si el script ya fue ejecutado lo borra*/
                //wxRemoveFile(mPathFile);
            }else{
                /*Ejecuta el script*/
                mPathFile.Printf("%s%s",PATH_DBSCRIPT,mFileName.c_str());
                rta = RunScripts(poODBCConect,mPathFile,PATH_LOGS);
                if(rta){ // si hay errores
                    mStrError.Printf("Se registraron %i errores en el script: %s",rta,mFileName.c_str());
                }
                mConsulta.Printf("insert into script_aplicados values ('%s',NOW(),%i)",mFileName.c_str(),rta);
                poODBCConect->getDb(ODBC_LOCAL)->ExecSql(mConsulta.c_str());

            }
        }

        //cont = dir.GetNext(&mFileName);
        cont++;
    }

    return rta;
}



int DbScript :: RunScripts(ODBC *poODBCConect, wxString pPathScript,wxString pPathLog)
{
    wxString str;
    wxString script;
	wxString scriptTmp;
    wxString mLogFile;
    wxTextFile mTextFile(pPathScript);
    int cntError = 0, indErr = 0, log = 0;
    bool rta = true;
    wxFile archivo;
    mLogFile.Printf("%sdb_script.log",pPathLog.c_str()/*PATH_LOGS*/);

	//aqui el nuevo desarrollo para ejecutar sp. El archivo sp es SP_AAAAMMDD OJO hasta 11puede tener
	if( strstr( pPathScript,"SP_2") != NULL ){
		 cntError = RunScriptsSP(poODBCConect,pPathScript,pPathLog);
		 return cntError;
	}
    /*if(wxFile::Exists(mLogFile)){
        if(archivo.Open(mLogFile, archivo.write_append)){
            log = 1;
        }
    }*/
	if(!wxFile::Exists(mLogFile)){
		/*CREAR ARCHIVO*/
		archivo.Create(mLogFile,true,wxS_DEFAULT);
	}
    if(archivo.Open(mLogFile, archivo.write_append)){
        log = 1;
    }  
    if(mTextFile.Open()){
		for ( (!mTextFile.Eof()) ? str = mTextFile.GetFirstLine() : str = "" ; !mTextFile.Eof() || str.Len() > 0; (!mTextFile.Eof()) ? str = mTextFile.GetNextLine():str = "" )
        {
            if(str.Len() > 0 && str.GetChar(0) != '#'){             //si no es un comentario ni una linea en blanco
                script += str;                                      //concatena lineas
                if(str.Contains(";")){                              //Si tiene un punto y coma, ejecuta y pasa al siguiente
					script.Replace((wxChar*)"\n",(wxChar*)" ");
					script.Replace((wxChar*)"\t",(wxChar*)" ");
					script.Replace((wxChar*)"\r",(wxChar*)" ");
					script.Trim();
					script.Trim(FALSE);
                    rta = poODBCConect->getDb(ODBC_LOCAL)->ExecSql(script.c_str()); //ejecuta el script para la tabla original
					scriptTmp.Printf("%s",script.Lower().c_str()); //hace una copia en minusculas
					if(rta && (scriptTmp.Find("alter table") >= 0 || scriptTmp.Find("create table") >= 0)){ //si la sentencia es una modificación de estructura a una tabla, aplica el mismo script para las tablas auxiliares
						int pos=0;
						bool alter=false;
						alter = (scriptTmp.Find("alter table") >= 0) ? true : false;
						//obtiene el nombre de la tabla trabajando con una copia del script temporal
						scriptTmp.Replace((wxChar*)"alter table",(wxChar*)" ");
						scriptTmp.Replace((wxChar*)"create table",(wxChar*)" ");
						scriptTmp.Replace((wxChar*)"'",(wxChar*)" ");
						scriptTmp.Replace((wxChar*)"`",(wxChar*)" ");
						scriptTmp.Replace((wxChar*)"\"",(wxChar*)" ");
						scriptTmp.Trim(false);
						pos = scriptTmp.Find(" ");
						scriptTmp.Remove(pos); // queda solamente el nombre de la tabla
						if(alter == true){
							rta = EjecutarPorTipo(script,scriptTmp,poODBCConect);
							//informa al server que modifico una tabla
							INSERT_SCRIPT_POS(SPOS_UPDATE_TABLE, (char *)scriptTmp.c_str() );
						}else{
							//informa al server que creo una nueva tabla
							if(scriptTmp != "pos_dbmap" && scriptTmp != "pos_dbmap_datatype")
								INSERT_SCRIPT_POS(SPOS_CREATE_TABLE, (char *)scriptTmp.c_str() );
						}
					}
                    if(!rta){
                        if(log){
                            if(!cntError){
                                archivo.Write(wxDateTime::Now().Format("\n\n\n%d/%m/%y %H:%M.%S "));
                                archivo.Write("Error en el script: ");
                                archivo.Write(pPathScript);
                                archivo.Write("\n");
                            }
                            archivo.Write("\n");
                            archivo.Write(script);
                            archivo.Write("\n");
                            archivo.Write(poODBCConect->getDb(ODBC_LOCAL)->errorList[indErr]);
                            indErr = (indErr == (DB_MAX_ERROR_HISTORY - 1)) ? indErr = 0 : (indErr + 1);
                        } 
                        cntError++;
                    }
                    script.Clear();                                 //Borra la cadena
                }
            }
        }
    }else{          //no se pudo abrir el archivo
        mStrError.Printf("No se pudo abrir el script: %s",pPathScript.c_str());
        return 1;
    }
    archivo.Close();
    mTextFile.Close();
    /*Retorna la cantidad de errores en el script*/
    return cntError;
}

int DbScript :: RunScriptsSP(ODBC *poODBCConect, wxString pPathScript,wxString pPathLog)
{
    wxString str;
    wxString script;
	wxString scriptTmp;
    wxString mLogFile;
    wxTextFile mTextFile(pPathScript);
    int cntError = 0, indErr = 0, log = 0;
    bool rta = true;
    wxFile archivo;
    mLogFile.Printf("%sdb_script.log",pPathLog.c_str()/*PATH_LOGS*/);
    /*if(wxFile::Exists(mLogFile)){
        if(archivo.Open(mLogFile, archivo.write_append)){
            log = 1;
        }
    }*/
	if(!wxFile::Exists(mLogFile)){
		/*CREAR ARCHIVO*/
		archivo.Create(mLogFile,true,wxS_DEFAULT);
	}
    if(archivo.Open(mLogFile, archivo.write_append)){
        log = 1;
    }  
    if(mTextFile.Open()){
		string linea, archivo2;
		while(!mTextFile.Eof())
		{
			linea = mTextFile.GetNextLine();
			if(linea[0] != '#'){
				//los sp deben tener como primera linea el borrado si ya existe
				if( strstr( linea.c_str(),"DROP PROCEDURE IF EXISTS") != NULL ){
					rta = poODBCConect->getDb(ODBC_LOCAL)->ExecSql(linea.c_str()); //ejecuta el script para la tabla original
				} else				
					archivo2 += linea + "\n";
			}
		}

		if(archivo2.length() > 0){            //si no es un comentario ni una linea en blanco
			rta = poODBCConect->getDb(ODBC_LOCAL)->ExecSql(archivo2.c_str()); //ejecuta el script para la tabla original
			scriptTmp.Printf("%s",script.Lower().c_str()); //hace una copia en minusculas
		     if(!rta){
                 if(log){
                     if(!cntError){
                          archivo.Write(wxDateTime::Now().Format("\n\n\n%d/%m/%y %H:%M.%S "));
                          archivo.Write("Error en el script: ");
                          archivo.Write(pPathScript);
                          archivo.Write("\n");
                      }
				      archivo.Write("\n");
                      archivo.Write(script);
                      archivo.Write("\n");
                      archivo.Write(poODBCConect->getDb(ODBC_LOCAL)->errorList[indErr]);
                      indErr = (indErr == (DB_MAX_ERROR_HISTORY - 1)) ? indErr = 0 : (indErr + 1);
				 } 
                 cntError++;
               }
		}
               //     script.Clear();                                 //Borra la cadena
    }else{          //no se pudo abrir el archivo
        mStrError.Printf("No se pudo abrir el script: %s",pPathScript.c_str());
        return 1;
    }
    archivo.Close();
    mTextFile.Close();
    /*Retorna la cantidad de errores en el script*/
    return cntError;
}


char *DbScript :: getError()
{
    return (char *) mStrError.c_str();
}

bool DbScript :: EjecutarPorTipo(wxString pScript, wxString pNomTabla, ODBC *poODBCConect){
	wxString mScript_tipo;
	wxString mConsulta;
	char mSubTipo[15], mNomTableTipo[50];
	bool rta = true; 
    for( int i = 1; rta && i < CANT_TIPOS; i++ ) {
		mScript_tipo.Printf("%s",pScript.c_str());							//copia de la sentencia original
		DbTables::getStrTipo(i,mSubTipo,sizeof(mSubTipo) );					//obtiene el tipo
		strncpy( mNomTableTipo, pNomTabla.c_str(), sizeof(mNomTableTipo) );	//arma el nuevo nombre de la tabla segun tipo
		strncat( mNomTableTipo, mSubTipo, sizeof(mNomTableTipo) );
        mConsulta.Printf("SHOW FIELDS FROM %s",mNomTableTipo);				//controla si existe la tabla
        poODBCConect->getDb(ODBC_LOCAL)->ExecSql(mConsulta.c_str());

        if(poODBCConect->getDb(ODBC_LOCAL)->GetNext()){								//SI EXISTE LA TABLA
            /*si la tabla existe, ejecuta la sentencia para dicha tabla*/
            //reemplaza el nombre de la tabla original por el nombre tipo en el script
			mScript_tipo.Replace((wxChar*)pNomTabla.c_str(),(wxChar*)mNomTableTipo);
			rta = poODBCConect->getDb(ODBC_LOCAL)->ExecSql(mScript_tipo.c_str());				//Ejecuta la sentencia para la tabla tipo
        }
    }
	return rta;
}