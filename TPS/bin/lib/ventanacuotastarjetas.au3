;#include <GUIConstantsEx.au3>
;#include <WindowsConstants.au3>
;#include <ListviewConstants.au3>
;#include <EditConstants.au3>
;#include <FontConstants.au3>
; init application data
; Font
;#include <Misc.au3>

Opt("GuiCloseOnEsc", 0)
 Opt("GUIOnEventMode", 1)

Global Const $GUI_FONTNORMAL = 0
Global Const $GUI_FONTITALIC = 2
Global Const $GUI_FONTUNDER = 4
Global Const $GUI_FONTSTRIKE = 8
Global Const $GUI_EVENT_CLOSE = -3
Global Const $GUI_DEFBUTTON = 512
Global Const $FW_NORMAL = 400
Global Const $GUI_NOFOCUS= 8192
Global Const $GUI_DISABLE=128
Global Const $BS_ICON = 0x0040
Global Const $BS_BITMAP = 0x0080
Global Const $BS_DEFPUSHBUTTON = 0x0001
Global Const $MB_SYSTEMMODAL = 4096
Dim $aServers[4][4] ; columns: server name | LVitem ControlID | drive letter
$CmdLine[0] ; Contains the total number of items in the array.
$aServers[0][0]=3
Local $sFolder = @ScriptDir & "\"

$contador= 2
For $i = 1 To $CmdLine[1] Step 1
	 For $j = 0 To 2 Step 1
			$aServers[$i][$j] = $CmdLine[$contador]
		$contador = $contador + 1
	Next
Next

;$aServers[0][0]=3
;$aServers[1][1]='NOVIEMBRE'
;$aServers[2][1]='DICIEMBRE'
;$aServers[3][1]='ENERO'
;$aServers[1][0]='2018'
;$aServers[2][0]='2018'
;$aServers[3][0]='2018'
;$aServers[1][2]='$20.20'
;$aServers[2][2]='$20.20'
;$aServers[3][2]='$30.20'

Global $sUserName=''
Global $sPassword=''

; init GUI-related data
Global Const $GUI_W=350
Global Const $GUI_H=250
Global $msg=0



; buid GUI
GUICreate("CUOTAS TARJETAS",$GUI_W,$GUI_H, 650,400)
GUISetBkColor(0x444444)
$gServers=GUICtrlCreateListView(' CUOTAS  |    MONTOxCUOTA |   RECARGO%  ',10,10,$GUI_W-20,$GUI_H-120,Default,Default)
;GUICtrlCreateLabel('User Name:',50,$GUI_H-100+2)
;GUICtrlSetColor(-1,0xFFFFFF)
;GUICtrlCreateLabel('Password:',50,$GUI_H-70-2)
;GUICtrlSetColor(-1,0xFFFFFF)
;$gUserName=GUICtrlCreateInput('',110,$GUI_H-103+2,$GUI_W-220,20)
;$gPassword=GUICtrlCreateInput('',110,$GUI_H-73-2,$GUI_W-220,20,BitOR($GUI_SS_DEFAULT_INPUT,$ES_PASSWORD))
GUISetBkColor(0x00F04545)
;Local Const $sFont = "Comic Sans Ms"
Local Const $sFont = "Arial"
HotKeySet("{ENTER}", "Cerrar")
HotKeySet("{2}", "Cerrar")
For $i=1 To $aServers[0][0]
    GUICtrlSetFont(-1, 10, $FW_NORMAL,  1, $sFont)
	$aServers[$i][1]=GUICtrlCreateListViewItem($aServers[$i][0]&'|   '&$aServers[$i][1]&'|'&$aServers[$i][2]&'%',$gServers)
	GUICtrlSetColor (-1, 0x4545F0)
	GUICtrlSetState( $aServers[$i][1], $GUI_DISABLE )
	; GUICtrlSetBkColor(-1, 0xAA0000)
Next

GUICtrlSetState( $gServers, $GUI_DISABLE )
GUISetState()
    While 1
        $idMsg = GUIGetMsg()
        Select
            Case $idMsg = $GUI_EVENT_CLOSE
                MsgBox($MB_SYSTEMMODAL, "", "Dialog was closed")
                ExitLoop
        EndSelect
    WEnd

Func Cerrar()
	Exit
EndFunc   ;==>TogglePause