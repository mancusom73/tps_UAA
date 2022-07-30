#pragma pack (push,8) 
/*******************************************************************************
*   Sistema: TPV 1000 - IV
*
*   Clases      :   -
*   Descripcion :   Definicion de las funciones que van a ser utilizadas en 'C'
*
*   Autor
*   -----               
*   Forray Juan Pablo
********************************************************************************/


#ifndef __DB_H
#define __DB_H



#ifdef __cplusplus
extern    "C" {
#endif
	
	// ODBC Error codes (derived from ODBC SqlState codes)
	#define DB_FAILURE                                         0
    #define DB_SUCCESS                                         1
    #define DB_ERR_NOT_IN_USE                                  2
    #define DB_ERR_GENERAL_WARNING                             3  // SqlState = '01000'
    #define DB_ERR_DISCONNECT_ERROR                            4  // SqlState = '01002'
    #define DB_ERR_DATA_TRUNCATED                              5  // SqlState = '01004'
    #define DB_ERR_PRIV_NOT_REVOKED                            6  // SqlState = '01006'
    #define DB_ERR_INVALID_CONN_STR_ATTR                       7  // SqlState = '01S00'
    #define DB_ERR_ERROR_IN_ROW                                8  // SqlState = '01S01'
    #define DB_ERR_OPTION_VALUE_CHANGED                        9  // SqlState = '01S02'
    #define DB_ERR_NO_ROWS_UPD_OR_DEL                         10  // SqlState = '01S03'
    #define DB_ERR_MULTI_ROWS_UPD_OR_DEL                      11  // SqlState = '01S04'
    #define DB_ERR_WRONG_NO_OF_PARAMS                         12  // SqlState = '07001'
    #define DB_ERR_DATA_TYPE_ATTR_VIOL                        13  // SqlState = '07006'
    #define DB_ERR_UNABLE_TO_CONNECT                          14  // SqlState = '08001'
    #define DB_ERR_CONNECTION_IN_USE                          15  // SqlState = '08002'
    #define DB_ERR_CONNECTION_NOT_OPEN                        16  // SqlState = '08003'
    #define DB_ERR_REJECTED_CONNECTION                        17  // SqlState = '08004'
    #define DB_ERR_CONN_FAIL_IN_TRANS                         18  // SqlState = '08007'
    #define DB_ERR_COMM_LINK_FAILURE                          19  // SqlState = '08S01'
    #define DB_ERR_INSERT_VALUE_LIST_MISMATCH                 20  // SqlState = '21S01'
    #define DB_ERR_DERIVED_TABLE_MISMATCH                     21  // SqlState = '21S02'
    #define DB_ERR_STRING_RIGHT_TRUNC                         22  // SqlState = '22001'
    #define DB_ERR_NUMERIC_VALUE_OUT_OF_RNG                   23  // SqlState = '22003'
    #define DB_ERR_ERROR_IN_ASSIGNMENT                        24  // SqlState = '22005'
    #define DB_ERR_DATETIME_FLD_OVERFLOW                      25  // SqlState = '22008'
    #define DB_ERR_DIVIDE_BY_ZERO                             26  // SqlState = '22012'
    #define DB_ERR_STR_DATA_LENGTH_MISMATCH                   27  // SqlState = '22026'
    #define DB_ERR_INTEGRITY_CONSTRAINT_VIOL                  28  // SqlState = '23000'
    #define DB_ERR_INVALID_CURSOR_STATE                       29  // SqlState = '24000'
    #define DB_ERR_INVALID_TRANS_STATE                        30  // SqlState = '25000'
    #define DB_ERR_INVALID_AUTH_SPEC                          31  // SqlState = '28000'
    #define DB_ERR_INVALID_CURSOR_NAME                        32  // SqlState = '34000'
    #define DB_ERR_SYNTAX_ERROR_OR_ACCESS_VIOL                33  // SqlState = '37000'
    #define DB_ERR_DUPLICATE_CURSOR_NAME                      34  // SqlState = '3C000'
    #define DB_ERR_SERIALIZATION_FAILURE                      35  // SqlState = '40001'
    #define DB_ERR_SYNTAX_ERROR_OR_ACCESS_VIOL2               36  // SqlState = '42000'
    #define DB_ERR_OPERATION_ABORTED                          37  // SqlState = '70100'
    #define DB_ERR_UNSUPPORTED_FUNCTION                       38  // SqlState = 'IM001'
    #define DB_ERR_NO_DATA_SOURCE                             39  // SqlState = 'IM002'
    #define DB_ERR_DRIVER_LOAD_ERROR                          40  // SqlState = 'IM003'
    #define DB_ERR_SQLALLOCENV_FAILED                         41  // SqlState = 'IM004'
    #define DB_ERR_SQLALLOCCONNECT_FAILED                     42  // SqlState = 'IM005'
    #define DB_ERR_SQLSETCONNECTOPTION_FAILED                 43  // SqlState = 'IM006'
    #define DB_ERR_NO_DATA_SOURCE_DLG_PROHIB                  44  // SqlState = 'IM007'
    #define DB_ERR_DIALOG_FAILED                              45  // SqlState = 'IM008'
    #define DB_ERR_UNABLE_TO_LOAD_TRANSLATION_DLL             46  // SqlState = 'IM009'
    #define DB_ERR_DATA_SOURCE_NAME_TOO_LONG                  47  // SqlState = 'IM010'
    #define DB_ERR_DRIVER_NAME_TOO_LONG                       48  // SqlState = 'IM011'
    #define DB_ERR_DRIVER_KEYWORD_SYNTAX_ERROR                49  // SqlState = 'IM012'
    #define DB_ERR_TRACE_FILE_ERROR                           50  // SqlState = 'IM013'
    #define DB_ERR_TABLE_OR_VIEW_ALREADY_EXISTS               51  // SqlState = 'S0001'
    #define DB_ERR_TABLE_NOT_FOUND                            52  // SqlState = 'S0002'
    #define DB_ERR_INDEX_ALREADY_EXISTS                       53  // SqlState = 'S0011'
    #define DB_ERR_INDEX_NOT_FOUND                            54  // SqlState = 'S0012'
    #define DB_ERR_COLUMN_ALREADY_EXISTS                      55  // SqlState = 'S0021'
    #define DB_ERR_COLUMN_NOT_FOUND                           56  // SqlState = 'S0022'
    #define DB_ERR_NO_DEFAULT_FOR_COLUMN                      57  // SqlState = 'S0023'
    #define DB_ERR_GENERAL_ERROR                              58  // SqlState = 'S1000'
    #define DB_ERR_MEMORY_ALLOCATION_FAILURE                  59  // SqlState = 'S1001'
    #define DB_ERR_INVALID_COLUMN_NUMBER                      60  // SqlState = 'S1002'
    #define DB_ERR_PROGRAM_TYPE_OUT_OF_RANGE                  61  // SqlState = 'S1003'
    #define DB_ERR_SQL_DATA_TYPE_OUT_OF_RANGE                 62  // SqlState = 'S1004'
    #define DB_ERR_OPERATION_CANCELLED                        63  // SqlState = 'S1008'
    #define DB_ERR_INVALID_ARGUMENT_VALUE                     64  // SqlState = 'S1009'
    #define DB_ERR_FUNCTION_SEQUENCE_ERROR                    65  // SqlState = 'S1010'
    #define DB_ERR_OPERATION_INVALID_AT_THIS_TIME             66  // SqlState = 'S1011'
    #define DB_ERR_INVALID_TRANS_OPERATION_CODE               67  // SqlState = 'S1012'
    #define DB_ERR_NO_CURSOR_NAME_AVAIL                       68  // SqlState = 'S1015'
    #define DB_ERR_INVALID_STR_OR_BUF_LEN                     69  // SqlState = 'S1090'
    #define DB_ERR_DESCRIPTOR_TYPE_OUT_OF_RANGE               70  // SqlState = 'S1091'
    #define DB_ERR_OPTION_TYPE_OUT_OF_RANGE                   71  // SqlState = 'S1092'
    #define DB_ERR_INVALID_PARAM_NO                           72  // SqlState = 'S1093'
    #define DB_ERR_INVALID_SCALE_VALUE                        73  // SqlState = 'S1094'
    #define DB_ERR_FUNCTION_TYPE_OUT_OF_RANGE                 74  // SqlState = 'S1095'
    #define DB_ERR_INF_TYPE_OUT_OF_RANGE                      75  // SqlState = 'S1096'
    #define DB_ERR_COLUMN_TYPE_OUT_OF_RANGE                   76  // SqlState = 'S1097'
    #define DB_ERR_SCOPE_TYPE_OUT_OF_RANGE                    77  // SqlState = 'S1098'
    #define DB_ERR_NULLABLE_TYPE_OUT_OF_RANGE                 78  // SqlState = 'S1099'
    #define DB_ERR_UNIQUENESS_OPTION_TYPE_OUT_OF_RANGE        79  // SqlState = 'S1100'
    #define DB_ERR_ACCURACY_OPTION_TYPE_OUT_OF_RANGE          80  // SqlState = 'S1101'
    #define DB_ERR_DIRECTION_OPTION_OUT_OF_RANGE              81  // SqlState = 'S1103'
    #define DB_ERR_INVALID_PRECISION_VALUE                    82  // SqlState = 'S1104'
    #define DB_ERR_INVALID_PARAM_TYPE                         83  // SqlState = 'S1105'
    #define DB_ERR_FETCH_TYPE_OUT_OF_RANGE                    84  // SqlState = 'S1106'
    #define DB_ERR_ROW_VALUE_OUT_OF_RANGE                     85  // SqlState = 'S1107'
    #define DB_ERR_CONCURRENCY_OPTION_OUT_OF_RANGE            86  // SqlState = 'S1108'
    #define DB_ERR_INVALID_CURSOR_POSITION                    87  // SqlState = 'S1109'
    #define DB_ERR_INVALID_DRIVER_COMPLETION                  88  // SqlState = 'S1110'
    #define DB_ERR_INVALID_BOOKMARK_VALUE                     89  // SqlState = 'S1111'
    #define DB_ERR_DRIVER_NOT_CAPABLE                         90  // SqlState = 'S1C00'
    #define DB_ERR_TIMEOUT_EXPIRED                            91  // SqlState = 'S1T00'

	// Codigo de Error Nativo ( Native Error Code ) dado por el MYSQL
	#define	ERROR_NATIVO_126					126	// Index file is crashed
	#define	ERROR_NATIVO_127					127	// Record-file is crashed
	#define	ERROR_NATIVO_130					130	// Incorrect file format
	#define	ERROR_NATIVO_132					132	// Old database file
	#define	ERROR_NATIVO_134					134	// Record was already deleted (or record file crashed)
	#define	ERROR_NATIVO_135					135	// No more room in record file
	#define	ERROR_NATIVO_136					136	// No more room in index file
	#define	ERROR_NATIVO_141					141	// Duplicate unique key or constraint on write or update
	#define	ERROR_NATIVO_144					144	// Table is crashed and last repair failed
	#define	ERROR_NATIVO_145					145	// Table was marked as crashed and should be repaired
	#define	ERROR_NATIVO_1034				   1034 // Clave de archivo erronea para la tabla: '%s'; intente repararlo
	#define	ERROR_NATIVO_1035				   1035 // Clave de archivo antigua para la tabla '%s'; reparelo!
	#define	ERROR_NATIVO_1036				   1036 // '%s' es de solo lectura
	#define	ERROR_NATIVO_1054				   1054 // La columna '%s' en %s es desconocida
	#define	ERROR_NATIVO_1062				   1062 // Entrada duplicada para la clave - (ER_DUP_ENTRY)
	#define	ERROR_NATIVO_1194				   1194	// Tabla '%s' esta marcada como crashed y debe ser reparada (servidor)
	#define	ERROR_NATIVO_1195				   1195	// Tabla '%s' esta marcada como crashed y la última reparación (automactica?) falló
	#define	ER_SPECIFIC_ACCESS_DENIED_ERROR	   1227 // Acceso negado. Usted necesita el privilegio %s para esta operacion.
    #define	ERROR_NATIVO_1459				   1459	// Table upgrade required. Please do "REPAIR TABLE `%s`" to fix it!

	//Client Error Codes and Messages
	#define	CR_SERVER_GONE_ERROR			   2006	// El servidor MySQL no está disponible. 



    #define TOP          -1
    #define BOTTOM       -2

	/*Script pos*/
	#define SPOS_CREATE_TABLE		1
	#define SPOS_UPDATE_TABLE		2
	#define SPOS_INSERT_CLIENTE 	3
	#define SPOS_UPDATE_CLIENTE 	4
	#define SPOS_OFFLINE       	    5
	#define SPOS_PAUSA_JSINCRO 	    6

    long GET_POSITION_REG(  );
    int GET_PAGINA_REG( );
    int GET_DIRECT_REG( unsigned long pPosicion, int pPagina );
    int initOdbc(char *pDsn,char *pUser, char *pPasswd, int pNroConnect);
    void endOdbc( int pNroConnect );
    int OPEN_TABLE( int pTabla, int pTipo, char *pEstruct, long pSizeof );
    void CLOSE_TABLE( int pTabla, int pTipo);
    int SELECT_TABLE( int pTabla, int pTipo );
    int SELECTED2( );
    int SELECTED_TIPO( );
    int GO2( int pRegistro );
    int SET_ORDER2( char *pCampos );
    int SET_WHERE( char *pCondicion, ... );
    int SET_FROM( char *pTablas );
    int SET_LIMIT( int pLimite, int pPaginado );
    int RUN_QUERY(int pDistinct);
    int SKIP2( int pCant );
    int SKIP_PAGE( int pCant );
    int dbEOF();
    int dbBOF();
    int FOUND2();
    int OPENED(int pTabla, int pTipo );
    long RECCOUNT2( char *pDistinct, char *pwhere, ... );
    int DIRECT_DB_QUERY(char *pQuery, ...);
    int GET_DATO(int pColumna, int pType, char *pData, long pSizeof);
    int DIRECT_DB_SKIP( int pCant );
    int OPEN_TABLE_TMP( int pTabla, char *pNombre, char *pEstruct );
    int INSERT2(int pModifStatus);
    int UPDATE2( );
    int DELETE2( );
    int VERIFICAR_SCRIPT();
    int EJECUTAR_SCRIPT( char* pNombre, char* pPath );
	int EJECUTAR_SCRIPT_TOUCH(void);
	long SQL_ROW_SIZE(int pTabla);
	int ZAP2(  );
    int GET_TABLE_NAME( int pTabla, int pTipo, char *pName, int pSizeMaxName );
    int GET_TABLE_NUMBER( char *pName );
    int DROP_TABLE(int pTabla, int pTipo );
	char * GET_WHERE(  );
	int SET_EMPTY_TABLE(int pTabla, int pTipo);
	int BACKUP_TABLE( int pTabla );
    int BACKUP_TABLE_TYPE( int pTabla, int pTipo );
    int RESTORE_TABLE_TYPE( int pTabla, int pTipo );
    int TRANSFER_TABLE( int pTabla, int pTipoS, int pTipoD );
	int INSERT_SCRIPT_POS(int pCodComando, char* pParametro );
	void VERIFICAR_CONEXION( );
	int VERIFICAR_ESTADO_TABLA( char *sentencia, int repair )  ;
#ifdef __cplusplus
}
#endif

#endif
#pragma pack (pop) 
