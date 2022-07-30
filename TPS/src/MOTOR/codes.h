#pragma pack (push,1) 
#pragma pack (1)

#ifndef _CODES_
#define _CODES_

typedef enum { MkNUM, /* Operaciones Binarias Aritmeticas */
               MkSUM, MkSUB, MkMULT, MkFDIV, MkMOD, MkDIV, MkMAX, MkMIN, MkLEQ, MkLESS, MkEQUAL,
               MkGREATER, MkGEQ, MkNEQ, MkCOND,  MkPRECOND, /* Operaciones Binarias Booleanas */
               MkAND, MkOR, MkLOG, /* Operaciones Binarias de Multiconjuntos */
               MkUNION, MkINTERSECTION, MkIN, MkMEET, /* Operaciones Binarias de Multiconjuntos */
               MkCONCAT, /* Operaciones Unarias */
               MkAMOUNT, MkCOMPLEMENT, MkSIZE, MkLENGTH, MkNUM_RNV, MkBOOL_RNV, MkSTRING_RNV,
               MkTIME_RNV, MkDATE_RNV, MkCOUNTABLE, MkROUND, MkTRUNC, /* */
               MkDOT, MkINTERVAL, /* Sets, AMSets*/
               MkSET, MkAMSET, MkPART, /* Constructores IDENT */
               MkIDENT, MkDEPARTMENT, MkPOINT, MkBONUS, MkARTICLE, MkSTRING, MkMUTUAL, MkTICKET,
               MkCHEQUE, MkCURRENCY, /* Constructores dos IDENT */
               MkCLASS, MkCARD, /* Valor de Fecha y Hora */
               MkDATEV, MkTIMEV, /* Valores Booleanos*/
               MkTRUE, MkFALSE, MkINPUT, MkNUMINPUT, /* */
               MkHOLIDAY, MkAFFILIATED, MkALL, /*MkPURCHASE, MkWPURCHASE, */
               MkBUYED, MkDELIVERED, MkDISCOUNTS, MkADDED, MkTIME, MkDATE, MkDATEOFBIRTH, MkDAY,
               MkCASH, MkCTACTE, MkMEANS, MkCHANGE, MkRETURNED, MkPOINTS, MkEBONUS, MkCASHDESK,
               MkDAYSELECTOR, MkMONTHSELECTOR, MkYEARSELECTOR, /* DAYS*/
               MkMONDAY, MkTUESDAY, MkWEDNESDAY, MkTHURSDAY, MkFRIDAY, MkSATURDAY, MkSUNDAY,
               /* REQUERIMIENTOS EXCEPCIONALES */
               MkREQ_ANY, MkREQ_ALL, MkREQ_DEP, /* BENEFS */
               MkASSIGN, MkIF, MkFOR, MkPRINT, MkCREDIT, MkDISTRIBUTE, MkISSUE_BONUS, MkGIVE_POINTS,
               MkACTIVATE, MkEPROC, MkSKIP, MkCOMP, MkREC, MkNUM_RNV_ASSIGN, MkBOOL_RNV_ASSIGN,
	       MkREQ_MEANS, MkREQ_MEANS_CANT,
               MkSTRING_RNV_ASSIGN, MkDATE_RNV_ASSIGN, MkTIME_RNV_ASSIGN, /* PARAMS */
               MkPARAM, MkPARAMGLOBAL, MkPARAMEXTERN, MkPARAMSTATIC, /* PROMOS */
               MkPROMO, MkCANCELLATION, /* PROG */
               MkPROG } code;


#endif

#pragma pack (pop) 
