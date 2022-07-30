;#include <GUIConstantsEx.au3>
;#include <WindowsConstants.au3>
;#include <ListviewConstants.au3>
;#include <EditConstants.au3>
;#include <FontConstants.au3>
; init application data
; Font
Global Const $GUI_FONTNORMAL = 0
Global Const $GUI_FONTITALIC = 2
Global Const $GUI_FONTUNDER = 4
Global Const $GUI_FONTSTRIKE = 8
Global Const $GUI_EVENT_CLOSE = -3
Global Const $GUI_DEFBUTTON = 512
Global Const $FW_NORMAL = 400
Dim $aServers[4][4] ; columns: server name | LVitem ControlID | drive letter
$CmdLine[0] ; Contains the total number of items in the array.
$aServers[0][0]=3
Global $contador= 2
For $i = 1 To $CmdLine[1] Step 1
	 For $j = 0 To 3 Step 1
		ConsoleWrite("Missing " & $contador & " "  & @CRLF)
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
GUICreate("RESUMEN DE DEUDA DE ASOCIADO",$GUI_W,$GUI_H)
GUISetBkColor(0x444444)
$gServers=GUICtrlCreateListView('  MES      |  Año       |   Deuda      ',10,10,$GUI_W-20,$GUI_H-120,Default,Default)
;GUICtrlCreateLabel('User Name:',50,$GUI_H-100+2)
;GUICtrlSetColor(-1,0xFFFFFF)
;GUICtrlCreateLabel('Password:',50,$GUI_H-70-2)
;GUICtrlSetColor(-1,0xFFFFFF)
;$gUserName=GUICtrlCreateInput('',110,$GUI_H-103+2,$GUI_W-220,20)
;$gPassword=GUICtrlCreateInput('',110,$GUI_H-73-2,$GUI_W-220,20,BitOR($GUI_SS_DEFAULT_INPUT,$ES_PASSWORD))
GUISetBkColor(0x00F04545) 
Local Const $sFont = "Comic Sans Ms"
For $i=1 To $aServers[0][0]
    GUICtrlSetFont(-1, 12, $FW_NORMAL,  $GUI_FONTITALIC, $sFont) 
	$aServers[$i][1]=GUICtrlCreateListViewItem($aServers[$i][0]&'|'&$aServers[$i][1]&'|   '&$aServers[$i][2],$gServers)
	GUICtrlSetColor (-1, 0x4545F0)
	
	; GUICtrlSetBkColor(-1, 0xAA0000)
Next

GUISetState()
$gButtonOK=GUICtrlCreateButton('Confirmar',110,$GUI_H-40,$GUI_W-220,30)
GUICtrlSetState($gButtonOK,$GUI_DEFBUTTON)
GUISetState()

; main function
Do
    $msg=GUIGetMsg()
    If $msg=$gButtonOK Then
;        $sUserName='EXTERNALDOMAIN\'&GUICtrlRead($gUserName)
 ;       $sPassword=GUICtrlRead($gPassword)
  ;      For $i=1 To $aServers[0][0]
   ;         If BitAND(GUICtrlRead($aServers[$i][1],1),$GUI_CHECKED) Then DriveMapAdd($aServers[$i][2],$aServers[$i][0],Default,$sUserName,$sPassword)
    ;    Next
        Exit
    EndIf
Until $msg=$GUI_EVENT_CLOSE