#ifndef JPOSCONS_H
#define JPOSCONS_H
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

/*
 * // se saco de los fuentes de jpos
 * public interface JposConst
 * {
 * //###################################################################
 * //#### General JavaPOS Constants
 * //###################################################################
 * 
 * /////////////////////////////////////////////////////////////////////
 * // "State" Property Constants
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int JPOS_S_CLOSED        = 1;
 * public static final int JPOS_S_IDLE          = 2;
 * public static final int JPOS_S_BUSY          = 3;
 * public static final int JPOS_S_ERROR         = 4;
 * 
 * /////////////////////////////////////////////////////////////////////
 * // "ResultCode" Property Constants
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int JPOSERR    = 100;
 * public static final int JPOSERREXT = 200;
 * 
 * public static final int JPOS_SUCCESS         =  0;
 * public static final int JPOS_E_CLOSED        =  1 + JPOSERR;
 * public static final int JPOS_E_CLAIMED       =  2 + JPOSERR;
 * public static final int JPOS_E_NOTCLAIMED    =  3 + JPOSERR;
 * public static final int JPOS_E_NOSERVICE     =  4 + JPOSERR;
 * public static final int JPOS_E_DISABLED      =  5 + JPOSERR;
 * public static final int JPOS_E_ILLEGAL       =  6 + JPOSERR;
 * public static final int JPOS_E_NOHARDWARE    =  7 + JPOSERR;
 * public static final int JPOS_E_OFFLINE       =  8 + JPOSERR;
 * public static final int JPOS_E_NOEXIST       =  9 + JPOSERR;
 * public static final int JPOS_E_EXISTS        = 10 + JPOSERR;
 * public static final int JPOS_E_FAILURE       = 11 + JPOSERR;
 * public static final int JPOS_E_TIMEOUT       = 12 + JPOSERR;
 * public static final int JPOS_E_BUSY          = 13 + JPOSERR;
 * public static final int JPOS_E_EXTENDED      = 14 + JPOSERR;
 * 
 * /////////////////////////////////////////////////////////////////////
 * // OPOS "BinaryConversion" Property Constants
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int JPOS_BC_NONE         = 0;
 * public static final int JPOS_BC_NIBBLE       = 1;
 * public static final int JPOS_BC_DECIMAL      = 2;
 * 
 * 
 * /////////////////////////////////////////////////////////////////////
 * // "CheckHealth" Method: "Level" Parameter Constants
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int JPOS_CH_INTERNAL     = 1;
 * public static final int JPOS_CH_EXTERNAL     = 2;
 * public static final int JPOS_CH_INTERACTIVE  = 3;
 * 
 * 
 * /////////////////////////////////////////////////////////////////////
 * // "CapPowerReporting", "PowerState", "PowerNotify" Property 
 * //   Constants
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int JPOS_PR_NONE         = 0;
 * public static final int JPOS_PR_STANDARD     = 1;
 * public static final int JPOS_PR_ADVANCED     = 2;
 * 
 * public static final int JPOS_PN_DISABLED     = 0;
 * public static final int JPOS_PN_ENABLED      = 1;
 * 
 * public static final int JPOS_PS_UNKNOWN      = 2000;
 * public static final int JPOS_PS_ONLINE       = 2001;
 * public static final int JPOS_PS_OFF          = 2002;
 * public static final int JPOS_PS_OFFLINE      = 2003;
 * public static final int JPOS_PS_OFF_OFFLINE  = 2004;
 * 
 * 
 * /////////////////////////////////////////////////////////////////////
 * // "ErrorEvent" Event: "ErrorLocus" Parameter Constants
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int JPOS_EL_OUTPUT       = 1;
 * public static final int JPOS_EL_INPUT        = 2;
 * public static final int JPOS_EL_INPUT_DATA   = 3;
 * 
 * 
 * /////////////////////////////////////////////////////////////////////
 * // "ErrorEvent" Event: "ErrorResponse" Constants
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int JPOS_ER_RETRY        = 11;
 * public static final int JPOS_ER_CLEAR        = 12;
 * public static final int JPOS_ER_CONTINUEINPUT= 13;
 * 
 * 
 * /////////////////////////////////////////////////////////////////////
 * // "StatusUpdateEvent" Event: Common "Status" Constants
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int JPOS_SUE_POWER_ONLINE      = 2001;
 * public static final int JPOS_SUE_POWER_OFF         = 2002;
 * public static final int JPOS_SUE_POWER_OFFLINE     = 2003;
 * public static final int JPOS_SUE_POWER_OFF_OFFLINE = 2004;
 * 
 * 
 * /////////////////////////////////////////////////////////////////////
 * // General Constants
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int JPOS_FOREVER         = -1;
 * }
 * 
 * 
 * 
 * /////////////////////////////////////////////////////////////////////
 * //
 * // THIS SOFTWARE IS PROVIDED AS IS. THE JAVAPOS WORKING GROUP MAKES NO 
 * // REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE SOFTWARE, 
 * // EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
 * // WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR 
 * // NON-INFRINGEMENT.  INDIVIDUAL OR CORPORATE MEMBERS OF THE JAVAPOS 
 * // WORKING GROUP SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED AS A RESULT
 * // OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
 * // 
 * // FiscalPrinterConst
 * //
 * //   Fiscal Printer constants for JavaPOS Applications.
 * //
 * // Modification history
 * // ------------------------------------------------------------------
 * // 98-04-20 JavaPOS Release 1.3                                   BS
 * //
 * /////////////////////////////////////////////////////////////////////
 * 
 * package jpos;
 * 
 * public interface FiscalPrinterConst
 * {
 * /////////////////////////////////////////////////////////////////////
 * // Fiscal Printer Station Constants
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int FPTR_S_JOURNAL                   = 1;
 * public static final int FPTR_S_RECEIPT                   = 2;
 * public static final int FPTR_S_SLIP                      = 4;
 * 
 * public static final int FPTR_S_JOURNAL_RECEIPT = FPTR_S_JOURNAL | FPTR_S_RECEIPT;
 * 
 * 
 * /////////////////////////////////////////////////////////////////////
 * // "CountryCode" Property Constants
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int FPTR_CC_BRAZIL                   =  1;
 * public static final int FPTR_CC_GREECE                   =  2;
 * public static final int FPTR_CC_HUNGARY                  =  3;
 * public static final int FPTR_CC_ITALY                    =  4;
 * public static final int FPTR_CC_POLAND                   =  5;
 * public static final int FPTR_CC_TURKEY                   =  6;
 * 
 * 
 * /////////////////////////////////////////////////////////////////////
 * // "ErrorLevel" Property Constants
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int FPTR_EL_NONE                     =  1;
 * public static final int FPTR_EL_RECOVERABLE              =  2;
 * public static final int FPTR_EL_FATAL                    =  3;
 * public static final int FPTR_EL_BLOCKED                  =  4;
 * 
 * 
 * /////////////////////////////////////////////////////////////////////
 * // "ErrorState", "PrinterState" Property Constants
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int FPTR_PS_MONITOR                  =  1;
 * public static final int FPTR_PS_FISCAL_RECEIPT           =  2;
 * public static final int FPTR_PS_FISCAL_RECEIPT_TOTAL     =  3;
 * public static final int FPTR_PS_FISCAL_RECEIPT_ENDING    =  4;
 * public static final int FPTR_PS_FISCAL_DOCUMENT          =  5;
 * public static final int FPTR_PS_FIXED_OUTPUT             =  6;
 * public static final int FPTR_PS_ITEM_LIST                =  7;
 * public static final int FPTR_PS_LOCKED                   =  8;
 * public static final int FPTR_PS_NONFISCAL                =  9;
 * public static final int FPTR_PS_REPORT                   = 10;
 * 
 * 
 * /////////////////////////////////////////////////////////////////////
 * // "SlipSelection" Property Constants
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int FPTR_SS_FULL_LENGTH              =  1;
 * public static final int FPTR_SS_VALIDATION               =  2;
 * 
 * 
 * /////////////////////////////////////////////////////////////////////
 * // "GetData" Method Constants
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int FPTR_GD_CURRENT_TOTAL            =  1;
 * public static final int FPTR_GD_DAILY_TOTAL              =  2;
 * public static final int FPTR_GD_RECEIPT_NUMBER           =  3;
 * public static final int FPTR_GD_REFUND                   =  4;
 * public static final int FPTR_GD_NOT_PAID                 =  5;
 * public static final int FPTR_GD_MID_VOID                 =  6;
 * public static final int FPTR_GD_Z_REPORT                 =  7;
 * public static final int FPTR_GD_GRAND_TOTAL              =  8;
 * public static final int FPTR_GD_PRINTER_ID               =  9;
 * public static final int FPTR_GD_FIRMWARE                 = 10;
 * public static final int FPTR_GD_RESTART                  = 11;
 * 
 * 
 * /////////////////////////////////////////////////////////////////////
 * // "AdjustmentType" arguments in diverse methods
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int FPTR_AT_AMOUNT_DISCOUNT          =  1;
 * public static final int FPTR_AT_AMOUNT_SURCHARGE         =  2;
 * public static final int FPTR_AT_PERCENTAGE_DISCOUNT      =  3;
 * public static final int FPTR_AT_PERCENTAGE_SURCHARGE     =  4;
 * 
 * 
 * /////////////////////////////////////////////////////////////////////
 * // "ReportType" argument in "PrintReport" method
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int FPTR_RT_ORDINAL                  =  1;
 * public static final int FPTR_RT_DATE                     =  2;
 * 
 * 
 * /////////////////////////////////////////////////////////////////////
 * // "StatusUpdateEvent" Event: "Data" Parameter Constants
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int FPTR_SUE_COVER_OPEN              =  11;
 * public static final int FPTR_SUE_COVER_OK                =  12;
 * 
 * public static final int FPTR_SUE_JRN_EMPTY               =  21;
 * public static final int FPTR_SUE_JRN_NEAREMPTY           =  22;
 * public static final int FPTR_SUE_JRN_PAPEROK             =  23;
 * 
 * public static final int FPTR_SUE_REC_EMPTY               =  24;
 * public static final int FPTR_SUE_REC_NEAREMPTY           =  25;
 * public static final int FPTR_SUE_REC_PAPEROK             =  26;
 * 
 * public static final int FPTR_SUE_SLP_EMPTY               =  27;
 * public static final int FPTR_SUE_SLP_NEAREMPTY           =  28;
 * public static final int FPTR_SUE_SLP_PAPEROK             =  29;
 * 
 * public static final int FPTR_SUE_IDLE                    =1001;
 * 
 * 
 * /////////////////////////////////////////////////////////////////////
 * // "ErrorCodeExtended" Property Constants for Fiscal Printer
 * /////////////////////////////////////////////////////////////////////
 * 
 * public static final int JPOS_EFPTR_COVER_OPEN = 1 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_JRN_EMPTY  = 2 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_REC_EMPTY  = 3 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_SLP_EMPTY  = 4 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_SLP_FORM   = 5 + JposConst.JPOSERREXT; // EndRemoval
 * public static final int JPOS_EFPTR_MISSING_DEVICES            =
 * 6 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_WRONG_STATE                =
 * 7 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_TECHNICAL_ASSISTANCE       =
 * 8 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_CLOCK_ERROR                =
 * 9 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_FISCAL_MEMORY_FULL         =
 * 10 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_FISCAL_MEMORY_DISCONNECTED =
 * 11 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_FISCAL_TOTALS_ERROR        =
 * 12 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_BAD_ITEM_QUANTITY          =
 * 13 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_BAD_ITEM_AMOUNT            =
 * 14 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_BAD_ITEM_DESCRIPTION       =
 * 15 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_RECEIPT_TOTAL_OVERFLOW     =
 * 16 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_BAD_VAT                    =
 * 17 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_BAD_PRICE                  =
 * 18 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_BAD_DATE                   =
 * 19 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_NEGATIVE_TOTAL             =
 * 20 + JposConst.JPOSERREXT; // (Several)
 * public static final int JPOS_EFPTR_WORD_NOT_ALLOWED           =
 * 21 + JposConst.JPOSERREXT; // (Several)
 * }
 */

// conversion a C++
#define  JPOS_S_CLOSED          1
#define  JPOS_S_IDLE            2
#define  JPOS_S_BUSY            3
#define  JPOS_S_ERROR           4

/////////////////////////////////////////////////////////////////////
// "ResultCode" Property Constants
/////////////////////////////////////////////////////////////////////

#define  JPOSERR                100
#define  JPOSERREXT             200
#define  JPOS_SUCCESS           0
#define  JPOS_E_CLOSED          (1 + JPOSERR)
#define  JPOS_E_CLAIMED         (2 + JPOSERR)
#define  JPOS_E_NOTCLAIMED      (3 + JPOSERR)
#define  JPOS_E_NOSERVICE       (4 + JPOSERR)
#define  JPOS_E_DISABLED        (5 + JPOSERR)
#define  JPOS_E_ILLEGAL         (6 + JPOSERR)
#define  JPOS_E_NOHARDWARE      (7 + JPOSERR)
#define  JPOS_E_OFFLINE         (8 + JPOSERR)
#define  JPOS_E_NOEXIST         (9 + JPOSERR)
#define  JPOS_E_EXISTS          (10 + JPOSERR)
#define  JPOS_E_FAILURE         (11 + JPOSERR)
#define  JPOS_E_TIMEOUT         (12 + JPOSERR)
#define  JPOS_E_BUSY            (13 + JPOSERR)
#define  JPOS_E_EXTENDED        (14 + JPOSERR)

/////////////////////////////////////////////////////////////////////
// OPOS "BinaryConversion" Property Constants
/////////////////////////////////////////////////////////////////////

#define  JPOS_BC_NONE           0
#define  JPOS_BC_NIBBLE         1
#define  JPOS_BC_DECIMAL        2


/////////////////////////////////////////////////////////////////////
// "CheckHealth" Method: "Level" Parameter Constants
/////////////////////////////////////////////////////////////////////

#define  JPOS_CH_INTERNAL       1
#define  JPOS_CH_EXTERNAL       2
#define  JPOS_CH_INTERACTIVE    3


/////////////////////////////////////////////////////////////////////
// "CapPowerReporting", "PowerState", "PowerNotify" Property 
//   Constants
/////////////////////////////////////////////////////////////////////

#define      JPOS_PR_NONE         0
#define      JPOS_PR_STANDARD     1
#define      JPOS_PR_ADVANCED     2

#define      JPOS_PN_DISABLED     0
#define      JPOS_PN_ENABLED      1

#define      JPOS_PS_UNKNOWN      2000
#define      JPOS_PS_ONLINE       2001
#define      JPOS_PS_OFF          2002
#define      JPOS_PS_OFFLINE      2003
#define      JPOS_PS_OFF_OFFLINE  2004


/////////////////////////////////////////////////////////////////////
// "ErrorEvent" Event: "ErrorLocus" Parameter Constants
/////////////////////////////////////////////////////////////////////

#define  JPOS_EL_OUTPUT         1
#define  JPOS_EL_INPUT          2
#define  JPOS_EL_INPUT_DATA     3


/////////////////////////////////////////////////////////////////////
// "ErrorEvent" Event: "ErrorResponse" Constants
/////////////////////////////////////////////////////////////////////

#define  JPOS_ER_RETRY          11
#define  JPOS_ER_CLEAR          12
#define  JPOS_ER_CONTINUEINPUT  13


/////////////////////////////////////////////////////////////////////
// "StatusUpdateEvent" Event: Common "Status" Constants
/////////////////////////////////////////////////////////////////////

#define  JPOS_SUE_POWER_ONLINE          2001
#define  JPOS_SUE_POWER_OFF             2002
#define  JPOS_SUE_POWER_OFFLINE         2003
#define  JPOS_SUE_POWER_OFF_OFFLINE     2004


/////////////////////////////////////////////////////////////////////
// General Constants
/////////////////////////////////////////////////////////////////////

#define  JPOS_FOREVER         (-1);

/////////////////////////////////////////////////////////////////////
// Fiscal Printer Station Constants
/////////////////////////////////////////////////////////////////////

#define  FPTR_S_JOURNAL                   1
#define  FPTR_S_RECEIPT                   2
#define  FPTR_S_SLIP                      4

#define  FPTR_S_JOURNAL_RECEIPT         (FPTR_S_JOURNAL | FPTR_S_RECEIPT)


/////////////////////////////////////////////////////////////////////
// "CountryCode" Property Constants
/////////////////////////////////////////////////////////////////////

#define  FPTR_CC_BRAZIL                   1
#define  FPTR_CC_GREECE                   2
#define  FPTR_CC_HUNGARY                  3
#define  PTR_CC_ITALY                     4
#define  FPTR_CC_POLAND                   5
#define  FPTR_CC_TURKEY                   6


/////////////////////////////////////////////////////////////////////
// "ErrorLevel" Property Constants
/////////////////////////////////////////////////////////////////////

#define  FPTR_EL_NONE                     1
#define  FPTR_EL_RECOVERABLE              2
#define  FPTR_EL_FATAL                    3
#define  FPTR_EL_BLOCKED                  4


/////////////////////////////////////////////////////////////////////
// "ErrorState", "PrinterState" Property Constants
/////////////////////////////////////////////////////////////////////

#define  FPTR_PS_MONITOR                  1
#define  FPTR_PS_FISCAL_RECEIPT           2
#define  FPTR_PS_FISCAL_RECEIPT_TOTAL     3
#define  FPTR_PS_FISCAL_RECEIPT_ENDING    4
#define  FPTR_PS_FISCAL_DOCUMENT          5
#define  FPTR_PS_FIXED_OUTPUT             6
#define  FPTR_PS_ITEM_LIST                7
#define  FPTR_PS_LOCKED                   8
#define  FPTR_PS_NONFISCAL                9
#define  FPTR_PS_REPORT                   10


/////////////////////////////////////////////////////////////////////
// "SlipSelection" Property Constants
/////////////////////////////////////////////////////////////////////

#define  FPTR_SS_FULL_LENGTH              1
#define  FPTR_SS_VALIDATION               2


/////////////////////////////////////////////////////////////////////
// "GetData" Method Constants
/////////////////////////////////////////////////////////////////////

#define  FPTR_GD_CURRENT_TOTAL            1
#define  FPTR_GD_DAILY_TOTAL              2
#define  FPTR_GD_RECEIPT_NUMBER           3
#define  FPTR_GD_REFUND                   4
#define  FPTR_GD_NOT_PAID                 5
#define  FPTR_GD_MID_VOID                 6
#define  FPTR_GD_Z_REPORT                 7
#define  FPTR_GD_GRAND_TOTAL              8
#define  FPTR_GD_PRINTER_ID               9
#define  FPTR_GD_FIRMWARE                 10
#define  FPTR_GD_RESTART                  11


/////////////////////////////////////////////////////////////////////
// "AdjustmentType" arguments in diverse methods
/////////////////////////////////////////////////////////////////////

#define  FPTR_AT_AMOUNT_DISCOUNT          1
#define  FPTR_AT_AMOUNT_SURCHARGE         2
#define  FPTR_AT_PERCENTAGE_DISCOUNT      3
#define  FPTR_AT_PERCENTAGE_SURCHARGE     4


/////////////////////////////////////////////////////////////////////
// "ReportType" argument in "PrintReport" method
/////////////////////////////////////////////////////////////////////

#define  FPTR_RT_ORDINAL                  1
#define  FPTR_RT_DATE                     2


/////////////////////////////////////////////////////////////////////
// "StatusUpdateEvent" Event: "Data" Parameter Constants
/////////////////////////////////////////////////////////////////////

#define  FPTR_SUE_COVER_OPEN              11
#define  FPTR_SUE_COVER_OK                12

#define  FPTR_SUE_JRN_EMPTY               21
#define  FPTR_SUE_JRN_NEAREMPTY           22
#define  FPTR_SUE_JRN_PAPEROK             23

#define  FPTR_SUE_REC_EMPTY               24
#define  FPTR_SUE_REC_NEAREMPTY           25
#define  FPTR_SUE_REC_PAPEROK             26

#define  FPTR_SUE_SLP_EMPTY               27
#define  FPTR_SUE_SLP_NEAREMPTY           28
#define  FPTR_SUE_SLP_PAPEROK             29

#define  FPTR_SUE_IDLE                    1001


/////////////////////////////////////////////////////////////////////
// "ErrorCodeExtended" Property Constants for Fiscal Printer
/////////////////////////////////////////////////////////////////////

#define  JPOS_EFPTR_COVER_OPEN      (1 + JPOSERREXT)
#define  JPOS_EFPTR_JRN_EMPTY       (2 + JPOSERREXT)
#define  JPOS_EFPTR_REC_EMPTY       (3 + JPOSERREXT)
#define  JPOS_EFPTR_SLP_EMPTY       (4 + JPOSERREXT)
#define  JPOS_EFPTR_SLP_FORM        (5 + JPOSERREXT)
#define  JPOS_EFPTR_MISSING_DEVICES (6 + JPOSERREXT)
#define  JPOS_EFPTR_WRONG_STATE     (7 + JPOSERREXT)
#define  JPOS_EFPTR_TECHNICAL_ASSISTANCE       (8 + JPOSERREXT)
#define  JPOS_EFPTR_CLOCK_ERROR     (9 + JPOSERREXT)
#define  JPOS_EFPTR_FISCAL_MEMORY_FULL     ( 10 + JPOSERREXT)
#define  JPOS_EFPTR_FISCAL_MEMORY_DISCONNECTED (11 + JPOSERREXT)
#define  JPOS_EFPTR_FISCAL_TOTALS_ERROR        (12 + JPOSERREXT)
#define  JPOS_EFPTR_BAD_ITEM_QUANTITY          (13 + JPOSERREXT)
#define  JPOS_EFPTR_BAD_ITEM_AMOUNT            (14 + JPOSERREXT)
#define  JPOS_EFPTR_BAD_ITEM_DESCRIPTION       (15 + JPOSERREXT)
#define  JPOS_EFPTR_RECEIPT_TOTAL_OVERFLOW     (16 + JPOSERREXT)
#define  JPOS_EFPTR_BAD_VAT                    (17 + JPOSERREXT)
#define  JPOS_EFPTR_BAD_PRICE                  (18 + JPOSERREXT)
#define  JPOS_EFPTR_BAD_DATE                   (19 + JPOSERREXT)
#define  JPOS_EFPTR_NEGATIVE_TOTAL             (20 + JPOSERREXT)
#define  JPOS_EFPTR_WORD_NOT_ALLOWED           (21 + JPOSERREXT)
#endif