#pragma pack (push,8) 
/*******************************************************************************
*   Sistema: TPV 1000 - IV
*
*   Clases      :   -
*   Descripcion :   Definicion de todas la tablas
*
*   Autor
*   -----               
*   Forray Juan Pablo
********************************************************************************/


#ifndef __DEFTABLES_H
#define __DEFTABLES_H

/*Conexiones*/
#define ODBC_LOCAL    0
#define ODBC_SERVIDOR 1

#define C_CHAR        1
#define C_INT         4
#define C_DOUBLE      8

/* Para tipos extra del Conftpv */
#define C_SHORT       2
#define C_FLOAT       4
#define C_DATE        2
#define C_TIME        2

/* Tipos de tablas */
#define TT_ORIG          0
#define TT_BAK           1
#define TT_TEMP          2
#define TT_ENTR          3
#define TT_ALTA          4
#define TT_MODIF         5
#define TT_BAK_ULT       6
#define TT_HIST			 7
#define TT_IMPORTACION   8
#define TT_OLD		     9	//utilizada para hacer copia de la original al iniciar la caja controla las old por si se corto la luz en una operacion
#define TT_TEMP1         10

#define CANT_TIPOS       11

//#define T_TEMP  	        0
#define T_CLI_COBRO	        1
#define T_PAGOS2	        2
#define T_CLI_COBRO_NOV     3
#define T_CAJEROS			4
#define T_CLIENTES			5
#define T_BANCOS			6
#define T_CUO_TAR			7
#define T_DPTO				8
#define T_COMPRA			9
#define T_MUTUALES			10
#define T_COBROS			11
#define T_ARTICULO			12
#define T_TARJETAS			13
#define T_MEDIOS			14
#define T_IMPUESTO			15
#define T_MOV_CTA			16
//#define T_ALTAS				17
//#define T_ALTAS_PR			18
//#define T_MODIF				19
#define T_MODIF_PR			20
#define T_PRESENT			21
//#define T_PENDIENTE1		22
//#define T_PENDIENTE2		23
#define T_REND_CAJ			24
#define T_TER_CAJA			25
#define T_CUO_CTA			26
#define T_CUO_MUT			27
#define T_SCRIPT			28
#define T_ACU_VTA       	29
#define T_CAJAS				30
#define T_RANGOS			31
#define T_PLU				32
#define T_PERFILES			33
#define T_ART_PERF			34
#define T_PERF_DES			35
#define T_FUNCION			36
#define T_CLASIF1			37
#define T_CLAS_PER			38
#define T_VIGENCIA			39
#define T_NODOS				40
#define T_EVENTOS			41
//#define T_TICKET_ENTR		    42
//#define T_TICKET_BAK_ULT	    43
#define T_EV_CONT			44
//#define T_E_TICKET_ENTR		45
//#define T_E_TICKET_BAK_ULT	46
#define T_PAGOS				47
//#define T_PAGO_ENTR			48
//#define T_PAGO_BAK_ULT		49
#define T_EV_CIERRE			50
#define T_VARIOS            51
#define T_EV_SUPER          52
//#define T_VARIOS_ENTR         53
//#define T_VARIOS_BAK_ULT      54
//#define T_EV_SUPER_ENTR       55
#define T_EV_DMEDIOS        56      // ex T_EV_DPAGO 
//#define T_EV_DPAGO_ENTR       57
#define T_EV_DESPRO         58
//#define T_EV_DESPRO_ENTR      59
#define T_EV_FACTU          60
//#define T_EV_FACTU_ENTR       61
#define T_EV_B_PROMO        62
//#define T_EV_B_PROMO_ENTR     63
#define T_GRUPO_CLI         64
#define T_ARTICULO_IVA      65
#define T_GARANTIA			66
#define T_MOTIVOS			67
#define T_MOTIVOS2			68
#define T_CLASIF2			69
#define T_BAND_ART			70
#define T_BANDAS			71
#define T_BANDAS_CLAS		72
#define T_TAR_MED			73
#define T_VUELTOS			74
#define T_CAJA_SUC			75
#define T_COND_IVA			76
#define T_MASCARA_SCANNER	77
//#define T_PROMO_FID			78
#define T_INSTITUT			79
//#define T_PRO_CLI			80
#define T_TEMPLATES			81
#define T_CUO_GRU			82
#define T_TENVASE    		83
#define T_COB_MED			84
//#define T_PRO_DEP			85
#define T_D_ENTRY			86
#define T_COB_TAR			87
#define T_CTRL_NC			88
//#define T_PRO_MED			89
//#define T_PRO_PLU         90
//#define T_PRO_PER			91
//#define T_PRO_CLAS		 92
#define T_CUO_MED			93
#define T_ART_DAT			94
//#define T_ART_BEN			95
#define T_COPIAS_T			96
#define T_TAR_ESP			97
#define T_COMTAR			98
#define T_EVC_TMP			99
#define T_MONEDAS      	    100
#define T_EV_TMP			101
#define T_EVCB_TMP			102
#define T_EVD_TMP			103
#define T_EVM_TMP			104
#define T_EVP_TMP			105
//#define T_PRO_ART			106
#define T_CLI_AFI			107
#define T_CLIENTES_NOV		108
#define T_SUCURSALES		109
#define T_ESTADO_CAJEROS    110
#define T_FORMULA_DGR			111
//#define T_MENSAJES	    	112 
//#define T_DET_OPER_CLI		113	
#define T_HABIL_DIFER_TARJ		114
#define T_CLI_CHEQ			115
//#define T_INFORME_SUP		116
#define T_CAMB_VAL_HAB		117
#define T_D_MEDIOS   		118
#define T_TIP_MOV_CAJA		119
#define T_TIP_MOV			120
//#define T_BOL_TIC			121 //TODO: eliminar la correcta es la de abajo
#define T_BOLETIN			122
#define T_R_SCRIPT			123
//#define T_COTIZA_TMP		124
#define T_TRANS_BAL_CAB		125
#define T_TRANS_BAL_DET		126 
#define T_PAGOS1			127
//#define T_DCOT_TMP			128
#define T_PANEL_VGA			129
#define T_BOTON_VGA			130
#define T_MEDIOS_REINT		131
//#define T_ALTA_DEU			132
//#define T_MODI_DEU			133
#define T_OPE_CLIENTE       134
#define T_CIERRE_LOTE		135
#define T_TKT_TMP			136
//#define T_UNINEG			137
//#define T_FUNC_USO			138
//#define T_STRING			139
//#define T_TAB_PRO			140
#define T_BOLETIN_NOV		141
//#define T_CLI_COBRO			142
//#define T_PREPAGA			143
#define T_COTICKET			144
#define T_TRANSAC2			145
#define T_ARCHIVOS			146
#define T_CARPETAS			147
#define T_STRINGT			148
#define T_EV_COD_BARRA		149
#define DD_EST2				150
#define DD_VAR				151
#define T_DETALLE_FONDOFIJO 152
#define T_GRUPO_ART         153
#define T_FACTU_MEM         154
#define T_NRO_CLI           155
#define T_INC_EVENTO        156
#define T_EVBP_TMP          157
#define T_TICK_TMP			158
#define T_DIF_CAJERO		159
#define T_LIN_MENS_TMP		160
#define T_TAR_TAR2			161
#define T_OLMEDIOS			162
#define T_DIF_TARJ		    163
#define T_VTA_XYZ		    164
#define T_VTA_CLAS		    165
#define T_VTA_MED		    166
#define T_AUX_ENVASE	    167
#define T_DEV_ENVASE	    168
#define T_RTA_SWITCH_BTR	169
//#define T_CHD_TCK_COMPARATIVO		170
#define T_D_DESCR					171
#define T_MED_DENOMINACION			172
#define T_EVCI_TMP				    173
#define T_TIPOS_COMPROB       	    174
#define T_TRAN_CRP       	        175
#define T_TPVCONFIG      	        176
#define T_ALICUOTA      	        177
#define T_PORCPERC      	        178
#define T_TOTAL_X      	            179
#define T_RESPUESTA_NAPSE           180


/*Suma total de tablas*/
#define CANT_TABLAS			        181



#endif
#pragma pack (pop) 
