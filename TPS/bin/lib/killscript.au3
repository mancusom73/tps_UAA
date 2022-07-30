;#include <MsgBoxConstants.au3>
Global Const $GUI_EVENT_CLOSE = -3
Example()

Func Example()
    ; Retrieve a list of window handles.
	Local $ventanaliquidar
	If $CmdLine[0] = 1 Then
		$ventanaliquidar = $CmdLine[1]
		Local $aList = WinList()

		; Loop through the array displaying only visable windows with a title.
		For $i = 1 To $aList[0][0]
			If $aList[$i][0] <> "" And BitAND(WinGetState($aList[$i][1]), 2) and $aList[$i][0] = $ventanaliquidar  Then
				;MsgBox($MB_SYSTEMMODAL, "", "Dialog was closed")
				WinActivate($aList[$i][0])
				Send("{BACKSPACE}")
			EndIf
		Next
	else	
		Run(@ComSpec & " /c pskill AutoIt3.exe", @SystemDir, @SW_HIDE)
	endif	
EndFunc   ;==>Example
