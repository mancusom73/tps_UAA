/*
 *   Sistema: TPV 1000 - IV
 *
 *   Interface C para el resto de la aplicacion.
 *
 *   @author: Nicolas Zandarin (2007)
 */

#include "confInterface.h"
#ifdef __cplusplus
#include "confManager.h"
#include "confContainer.h"
#endif

confContainer* confData;

extern "C" {

    int CARGAR_CONFTPV( void ) {
        
        bool loadedOk;
        confManager* configurador;

        //Se crea el objeto que carga la configuracion en memoria
        configurador = new confManager();
        loadedOk = configurador->load();
        if( loadedOk ) { 
            confData = configurador->getData();
        }
        delete( configurador );
        
        return loadedOk? 1:0;
    }
    
    CTYPE_INT GET_CONFIG_INT( int lugar ) {
        return confData->getInt( lugar );
    }
    CTYPE_FLOAT GET_CONFIG_FLOAT( int lugar ) {
        return confData->getFloat( lugar );
    }
    CTYPE_DOUBLE GET_CONFIG_DOUBLE( int lugar ) {
        return confData->getDouble( lugar );
    }
    CTYPE_UNSIGNED GET_CONFIG_UNSIGNED( int lugar ) {
        return confData->getUnsigned( lugar );
    }
    CTYPE_LONG GET_CONFIG_LONG( int lugar ) {
        return confData->getLong( lugar );
    }
    CTYPE_ULONG GET_CONFIG_ULONG( int lugar ) {
        return confData->getUnsignedLong( lugar );
    }
    CTYPE_STRING GET_CONFIG_STR( int lugar ) {
        return confData->getString( lugar );
    }
    CTYPE_CHAR GET_CONFIG_CHAR( int lugar ) {
        return (CTYPE_CHAR)(confData->getString( lugar ))[0];
    }
    int GET_CONFIG_STR_LENGHT( int lugar ) {
        return confData->getStringLen( lugar );
    }
}