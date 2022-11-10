#pragma pack (push,1) 
#ifndef __PATH_H
#define __PATH_H

#define DD_VAR_SIC            "../DD/dd_var.sic"
#define DD_EST2_SIC           "../DD/dd_est2.sic"
#define ARCHIVOS_SIC          "../AARCH/archivos.sic"
#define CARPETAS_SIC          "../AARCH/carpetas.sic"

#define RNV_DAT               "../DD/rnv.dat"
#define ENTRENAM_DAT          "../DD/entrenam.dat"
#define SCRIPT_SIC            "script.sic"
#define SCRIPT_CTR            "script.ctr"
#ifdef INVEL_L
#define NRO_CLI_SIC           "nro_cli.ctr"
#else
#define NRO_CLI_SIC           "c:nro_cli.sic"
#endif 

#ifdef INVEL_W
#define TKT_CAJA_INI          "../CFG/TKT-CAJA.INI"
#define TKT_CAJA_CFG          "../CFG/TKT-CAJA.CFG"
#define REM_CAJA_CFG          "../CFG/REM-CAJA.CFG"
#define FAC_CAJA_CFG          "../CFG/FAC-CAJA.CFG"
#define VAL_CAJA_CFG          "../CFG/VAL-CAJA.CFG"
#define CLIENTES_CFG          "../CFG/CLIENTES.CFG"
#else
#define TKT_CAJA_INI          "../CFG/tkt-caja.ini"
#define TKT_CAJA_CFG          "../CFG/tkt-caja.cfg"
#define REM_CAJA_CFG          "../CFG/rem-caja.cfg"
#define FAC_CAJA_CFG          "../CFG/fac-caja.cfg"
#define VAL_CAJA_CFG          "../CFG/val-caja.cfg"
#endif
#define FISCAL_DEF            "../CFG/FISCAL.DEF"
#define FISCAL_F_DEF          "../CFG/FISCAL_F.DEF"
#define FISCALEP_DEF          "../CFG/FISCALEP.DEF"
#define PRN_ON_DEF            "../CFG/PRN_ON.DEF"
#define PRINTER_DEF           "../CFG/PRINTER.DEF"
#define TM930_DEF             "../CFG/TM930.DEF"
#define FISCALE2_DEF          "../CFG/FISCALE2.DEF"
#define IBM_4614_DEF          "../CFG/IBM_4614.DEF"
#define TEC_OM_DEF            "../CFG/TEC_OM.DEF"
#define TEC_INV_DEF           "../CFG/TEC_INV.DEF"
#define TEC_MIX_DEF           "../CFG/TEC_MIX.DEF"
#define TEC_KB78_DEF          "../CFG/TEC_KB78.DEF"
#define PKBST_50_DEF          "../CFG/PKBST-50.DEF"
#define TEC_3500_DEF          "../CFG/TEC_3500.DEF"
#define TEC_CARY_DEF          "../CFG/TEC_CARY.DEF"
#define TEC_PC_DEF            "../CFG/tec_pc.def"
#define TEC_JS_DEF            "../CFG/TEC_JS.DEF"
#define TPVIV_INI             "../CFG/TPVIV.INI"

/*definicion de nombres de archivos para el recupero*/
#define ARCH_REC_COBROS_ESP             "../REC/cobros_esp.rec"
#define ARCH_REC_SUBMEDIOS              "../REC/submedios.rec"
#define ARCH_REC_SUPERVISOR             "../REC/super.rec"
#define ARCH_REC_COBRO_INC              "../REC/cobro_inc.rec"
#define ARCH_REC_CIERRE                 "../REC/cierre.rec"
#define ARCH_REC_DIFERENCIAS            "../REC/diferencias.rec"
#define ARCH_REC_DATOS_COMP             "../REC/datos_comp.rec"
#define ARCH_REC_IMPORT                 "../REC/import.rec"
#define ARCH_REC_RETIRO                 "../REC/submedios_retiros.rec"
#define ARCH_REC_RET_TARJ_CUP           "../REC/ret_tar_cup.rec"
#define ARCH_REC_IMPORTE_DEVOLUCION     "../REC/importe_devolucion.rec"
#define ARCH_REC_CONTADOR_ARTICULO      "../REC/contador_art.rec"
#define ARCH_REC_DEVOL_AM               "../REC/devol_am.rec"
#define ARCH_REC_TARJETAS_FISC		    "../REC/tarjetas_fisc.rec"
#define ARCH_REC_INFO_IMPORT            "../REC/info_import.rec"
#define ARCH_REC_IMPORT_EN_PANEL        "../REC/import_en_panel.rec"

/*Definicion de nombres de archivo comunicacion motor - caja*/
#define ARCH_REC_ACT_MEDIO_MOTOR        "../REC/act_medio.rec"    //Contiene la comunicacion Motor - Caja sobre medios de pago
#define ARCH_REC_PAGOS_PROMO            "../REC/pagos_promo.rec"  //Contiene los pagos realizados con medios activados por promocion, solo caja
#define ARCH_REC_ASIG_BENEFICIO         "../REC/asig_benef.rec"   //Contiene los beneficios otorgados en un ticket para imprimir o asignar en una cuenta
#define ARCH_REC_EVENT_PROMO            "../REC/event_promo.rec"  //Contiene los beneficios eventos de promoci?n que se deben informar para calcular el Costo Financiero
#define ARCH_REC_REQUERIR_MEDIO         "../REC/requerir_med.rec" //Contine los medios requeridos por promocion


#define ARCH_REC_COBROS					"../REC/cobros.rec"
#define ARCH_REC_DATOS_PAGATODO         "../REC/datos_PT.rec"
#define ARCH_REC_COBROS_VENTA				"../REC/cobros_venta.rec"
#define ARCH_REC_CIERRE_ANT				"../REC/ev_cierre_ant.rec"
#define ARCH_REC_COBROS_VENTA_GARANTIA		"../REC/cobros_venta_garantia.rec"
#define ARCH_REC_COBROS_VENTA_PAGATODO		"../REC/cobros_venta_PT.rec"
#define PATH_ART								"../../IMG/VISTA/ART/"
#define ARCH_REC_COBROS_KINACU				"../REC/cobros_kinacu.rec"
#define _PINPAD_DAT							"../TABLAS/pinpad.dat"
#define _ARCH_PWK							"../REC/pwkey.dat"  //archivo donde guardo temporal el pin del pinpad
#define _NAPSE_DAT							"../TABLAS/napse.dat"

/*archivos en comun*/
#define PATH_FOTO             "../../IMG/COMUN/USR/"
#define FOTODEFECTO           "../../IMG/COMUN/USR/fotodefault.gif"
#define IMAGE_INICIO          "../../IMG/COMUN/inicio.gif"
#define IMAGE_EMERGENTE       "../../IMG/COMUN/emergente.gif"
#define IMAGE_DETALLE_FTP     "../../IMG/COMUN/det_ftp.gif"
#define PATH_PROMOS           "../PRO/"
#define PATH_LOGS             "../LOG/"
#define PATH_BACKUPS          "../BAK/"
#define PATH_TMP_DADIC        "../TMP/"
#define PATH_SERVIDOR         "../M/"
#define PATH_TOUCH            "../TOUCH/"

#endif

/* archivos de importaciones */
#ifdef INVEL_L
#define EV_TMP "ev_tmp.ctr"
#define EVC_TMP "evc_tmp.ctr"
#define EVM_TMP "evm_tmp.ctr"
#define EVD_TMP "evd_tmp.ctr"
#define EVP_TMP "evp_tmp.ctr"
#define EVCB_TMP "evcb_tmp.ctr"
    #define EVBP_TMP "evbp_tmp.ctr"
    #define EV_TMP2 "ev_tmp2.ctr"
	#define EVCIERRE_TMP "evci_tmp.ctr"
#else
#define EV_TMP "ev_tmp.btr"
#define EVC_TMP "evc_tmp.btr"
#define EVM_TMP "evm_tmp.btr"
#define EVD_TMP "evd_tmp.btr"
#define EVP_TMP "evp_tmp.btr"
#define EVCB_TMP "evcb_tmp.btr"
    #define EVBP_TMP "evbp_tmp.btr"
    #define EV_TMP2 "ev_tmp2.btr"
	#define EVCIERRE_TMP "evci_tmp.btr"
#endif

/*path de archivos de mysql*/
#define PATH_MYSQL "../DATABASE/DBTPVIV/"
/*path de script de actualizacion de la base*/
#define PATH_DBSCRIPT "../../DBSCRIPT/"


/*path de archivos en el bin*/
#define PATH_BIN "..\\..\\Bin\\"

/* archivos de promociones */
#define PROMO_COD "promo.cod"
#define PROMO_WRK "promo.wrk"
#define PROMO_SQL "promo.sql"

#pragma pack (pop) 
