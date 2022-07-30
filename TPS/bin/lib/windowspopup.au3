
;Example Toast

#include "Toast.au3"
#include <Misc.au3>

Local $sMsg, $hProgress, $aRet[2]

Local $sAutoIt_Path = StringRegExpReplace(@AutoItExe, "(^.*\\)(.*)", "\1")
Local $yaestacorriendo =0
; -----------------------------------------
; Default Toast (The Title text goes here).
; -----------------------------------------
; con esto se que esta corriendo 
If _Singleton(@ScriptName,1)=0 Then 
	
	;veamos cuantas veces esta corriendo , el popup es POPUPPROMO
	Local $aList = WinList()
    For $i = 1 To $aList[0][0]
        If $aList[$i][0] <> "" And BitAND(WinGetState($aList[$i][1]), 2) and $aList[$i][0] = "POPUPPROMO" Then
            ;MsgBox($MB_SYSTEMMODAL, "", "Title: " & $aList[$i][0] & @CRLF & "Handle: " & $aList[$i][1])
			$yaestacorriendo = $yaestacorriendo + 1 
        EndIf
    Next

endif
;Exit MsgBox(262144+16,"Error!",@ScriptName&" is already running!");

$sMsg  = "Promo 2x1 en gaseosas cola 1234567" & "        "& @CRLF 
$sMsg &= "exitos y seguimos sumando         "& @CRLF & @CRLF 
;Local $vIcon= @ScriptDir & "promo.png"

;_Toast_Set(Default)
;_Toast_Set(5, 0xFF00FF, 0xFFFF00, 0x0000FF, 0xFFFFFF, 10, "Arial")
_Toast_Set(-1, 0xCC00CC, 0xD5D5FF, 0xEADDF8, -1, 12)
_Toast_Set_Timers_Animation(Default)
;$aRet = _Toast_Show(64, "PROMOCION ACTIVADA", $sMsg, 3, true, false)
 $aRet = _Toast_Show("c:\work_svn\TPS-LAREINA\bin\bakau3\promo.gif", "PROMOCION ACTIVADA", $sMsg, 3,true, false,$yaestacorriendo)
; Parameters ....: $vIcon    - 0 - No icon, 8 - UAC, 16 - Stop, 32 - Query, 48 - Exclamation, 64 - Information
;                              The $MB_ICON constant can also be used for the last 4 above
;                              If set to the name of an exe, the main icon of that exe will be displayed
;                              If set to the name of an image file, that image will be displayed
;                              Any other value returns -1, error 1
;                  $sTitle   - Text to display on Title bar
;                  $sMessage - Text to display in Toast body
;                  $iDelay   - The delay in seconds before the Toast retracts or script continues (Default = 0)
;                              If negative, an [X] is added to the title bar. Clicking [X] retracts/continues immediately
;                  $fWait    - True  - Script waits for delay time before continuing and Toast remains visible
;                              False - Script continues and Toast retracts automatically after delay time
;                  $fRaw     - True  - Message is not wrapped and Toast expands to show full width
;                            - False - Message is wrapped if over max preset Toast width

ConsoleWrite("Toast size: " & $aRet[0] & " x " & $aRet[1] & @CRLF)
_Toast_Hide()
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
exit
