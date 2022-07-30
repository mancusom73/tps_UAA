;#include <GUIConstantsEx.au3>
;#include <WindowsConstants.au3>
;#include <ListviewConstants.au3>
;#include <EditConstants.au3>
;#include <FontConstants.au3>
; init application data
; Font
;#include <Misc.au3>

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
Global Const $MB_SYSTEMMODAL = 4096 ; System modal
Dim $aServers[4][4] ; columns: server name | LVitem ControlID | drive letter
$CmdLine[0] ; Contains the total number of items in the array.
$aServers[0][0]=3
Local $sFolder = @ScriptDir & "\"
Local $mes[12] = ['ENERO','Febrero','MARZO','ABRIL','MAYO','JUNIO','JULIO','AGOSTO','SEPTIEMBRE','OCTUBRE','NOVIEMBRE','DICIEMBRE']
Local $posicion =1
Local $isprimera =1

$contador= 2
For $i = 1 To $CmdLine[1] Step 1
	 For $j = 0 To 2 Step 1
			If $j = 0 Then
				$meses = Number($CmdLine[$contador])
				if($meses > 12) Then
					$aServers[$i][$j] = "DATO INCORR"
				else
					$aServers[$i][$j] = $mes[$meses -1]
				endif
			else
					$aServers[$i][$j] = $CmdLine[$contador]
			EndIf
		$contador = $contador + 1
	Next
Next


Global $sUserName=''
Global $sPassword=''

; init GUI-related data
Global Const $GUI_W=350
Global Const $GUI_H=250
Global $msg=0



; buid GUI
Local $hGUI =GUICreate("RESUMEN DE DEUDA",$GUI_W,$GUI_H)
GUISetBkColor(0x444444)
$gServers=GUICtrlCreateListView('         MES                |    Año       |   Deuda        ',10,10,$GUI_W-20,$GUI_H-120,Default,Default)
;GUICtrlCreateLabel('User Name:',50,$GUI_H-100+2)
;GUICtrlSetColor(-1,0xFFFFFF)
;GUICtrlCreateLabel('Password:',50,$GUI_H-70-2)
;GUICtrlSetColor(-1,0xFFFFFF)
;$gUserName=GUICtrlCreateInput('',110,$GUI_H-103+2,$GUI_W-220,20)
;$gPassword=GUICtrlCreateInput('',110,$GUI_H-73-2,$GUI_W-220,20,BitOR($GUI_SS_DEFAULT_INPUT,$ES_PASSWORD))
GUISetBkColor(0x00F04545)
;Local Const $sFont = "Comic Sans Ms"
Local Const $sFont = "Arial"

For $i=1 To $aServers[0][0]
    GUICtrlSetFont(-1, 12, $FW_NORMAL,  1, $sFont)
	if $aServers[$i][1] > 0 then
		$aServers[$i][1]=GUICtrlCreateListViewItem($aServers[$i][0]&'|   '&$aServers[$i][1]&'|$'&$aServers[$i][2],$gServers)
	endif	
	GUICtrlSetColor (-1, 0x4545F0)
	GUICtrlSetState( $aServers[$i][1], $GUI_DISABLE )
	; GUICtrlSetBkColor(-1, 0xAA0000)
Next

GUICtrlSetState( $gServers, $GUI_DISABLE )
GUISetState()
$gButtonOK=GUICtrlCreateButton(' Cargar la Deuda en ticket  ',90,$GUI_H-80,$GUI_W-180,40, $BS_DEFPUSHBUTTON)
GUICtrlSetImage($gButtonOK, "shell32.dll", 279,2)
$gButtonNOOK=GUICtrlCreateButton('Cancelar Comienzo de ticket ',90,$GUI_H-40,$GUI_W-180,40)
GUICtrlSetImage(-1, "shell32.dll", 28,0)
GUICtrlSetState($gButtonOK,$GUI_DEFBUTTON)
;GUISetState()
GUISetState(@SW_SHOW)
ControlFocus($gButtonOK, "", "RESUMEN DE DEUDA")

HotKeySet("{Esc}", "captureEsc")

; main function
;ToolTip("<= ENTER Carga la deuda",760,422,'MENSAJE', 1,4)

ControlFocus($gButtonOK, "", "Edit1")
ToolTip("<= ENTER Carga la deuda",760,422,'MENSAJE', 1,4)
Do
    $msg=GUIGetMsg()
    If $msg=$gButtonOK and $isprimera > 1 Then
	    ToolTip("SE CARGA DEUDA DE CLIENTE",750,400 +( 30 * $i),'CAJA MENSAJE"', 1,4)
		Sleep(1000)
		ToolTip("")
        Exit(1)
	else
		If $msg=$gButtonNOOK and $isprimera >1 Then
			ToolTip("SE CANCELA TICKET",750,400 +( 30 * $i),'CAJA MENSAJE"', 3,4)
			Sleep(1000)
			ToolTip("")
			Exit(0)
		ENDIF
		If $msg=$gButtonOK and $isprimera = 1 Then
			$isprimera =2
		endif
    EndIf
	If _IsPressed("26") Then
	   ; cursor para arriba
	   $isprimera =2
	   ToolTip("")
	   Sleep(100)
	   Local $sControl = ControlGetFocus($hGUI)
	   if $sControl = "Button1" Then
			ToolTip("<= ENTER Carga la deuda",760,422,'MENSAJE', 1,4)
		else 	
			ToolTip("<= ENTER NO CARGA LA DEUDA",760,462,'MENSAJE', 3,4)			
		endif	
	EndIf

	If _IsPressed("28") Then
		; cursor para abajo
		$isprimera =2
		ToolTip("")
		Sleep(100)
		Local $sControl = ControlGetFocus($hGUI)
		if $sControl = "Button1" Then
			ToolTip("<= ENTER Carga la deuda",760,422,'MENSAJE', 1,4)	
		else
			ToolTip("<= ENTER NO CARGA LA DEUDA",760,462,'MENSAJE', 3,4)				
		Endif	
	EndIf
Until $msg=$GUI_EVENT_CLOSE
Exit(1)

Func _IsPressed($sHexKey, $vDLL = 'user32.dll')
	; $hexKey must be the value of one of the keys.
	; _Is_Key_Pressed will return 0 if the key is not pressed, 1 if it is.
	Local $a_R = DllCall($vDLL, "short", "GetAsyncKeyState", "int", '0x' & $sHexKey)
	If @error Then Return SetError(@error, @extended, False)
	Return BitAND($a_R[0], 0x8000) <> 0
EndFunc   ;==>_IsPressed

Func captureEsc()
    ; ... can do stuff here
    ;HotKeySet("{Esc}")
    ;Send("{Esc}")
    ;HotKeySet("{Esc}", "captureEsc")
EndFunc