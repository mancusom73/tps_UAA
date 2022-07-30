#include <zettoControl.au3>

Local $hgui, $nmsg, $zmsg, $btn1, $btn2, $btn3, $btn4, $btn5
$hgui = GUICreate('Progressbar Example', 520, 400, -1, -1)
;GUISetBkColor(0x00FF00)

Global $PICX = GUICtrlGetHandle(GUICtrlCreatePic(@ScriptDir & '\multimedia.jpg',0,90,576,221))
GUICtrlSetState(-1,$GUI_DISABLE)

;zettoCtrlSetWindow($hgui)
;zettoCtrlLoadProgress('Apple Tiger')

Global $pgbar = zettoCtrlCreateProgress(0,0,59,'%int%',175,125,150,17,'SC Google Chrome')
;zettoCtrlSetGlass(-1,'0x8F',0xBF)
;zettoCtrlSetState(-1, $GUI_DISABLE)
;zettoCtrlSetGlassMode(-1,False)

;zettoCtrlSetData($pgbar,True)

zettoCtrlLoadButton('SA Adobe Flash CS4 SWF #Single',@ScriptDir & '\Button\SA Adobe Flash CS4 SWF #Single')

$btn1 = zettoCtrlCreateButton('% Integer', 210, 155, 85, 25 ,'button1')
$btn2 = zettoCtrlCreateButton('% Decimal', 210, 183, 85, 25 ,'button2')
$btn3 = zettoCtrlCreateButton('% Int & Dec', 210, 211, 85, 25 ,'button3')
$btn4 = zettoCtrlCreateButton('Value', 210, 239, 85, 25 ,'button4')
$btn5 = zettoCtrlCreateButton('Apple Tiger', 210, 266, 85, 25 ,'button5',-1,-1)
;zettoCtrlSetState(-1, $GUI_DISABLE)
zettoCtrlSetGlass(-1,'0x8F',0xAF,0xCF,0x7f)
;zettoCtrlSetGlassMode(-1,False)

GUISetState(@SW_SHOW)
AdlibRegister('updatevalue')

	;#cs
	For $loop = 0 To 59
		zettoCtrlSetData($pgbar,Int($loop))
		Sleep(1)
		Next
	Sleep(100)
	;#ce

;zettoCtrlCreateTimer('updatevalue',90,True)

Do
	$zmsg = zettoEventHandler()
	Switch $zmsg
		Case 'button1_MouseUp'
			zettoCtrlSetData($pgbar,'%int%',1)
		Case 'button2_MouseUp'
			zettoCtrlSetData($pgbar,'%dec%',1)
		Case 'button3_MouseUp'
			zettoCtrlSetData($pgbar,'%intdec%',1)
		Case 'button4_MouseUp'
			zettoCtrlSetData($pgbar,'/val',1)
		Case 'button5_MouseUp'
			;zettoCtrlSetSkin($pgbar,'Apple Tiger')
		EndSwitch
	$nmsg = GUIGetMsg()
	Switch $nmsg
		Case $GUI_EVENT_CLOSE
			Exit
		EndSwitch
Until False
	
Func updatevalue()
	zettoCtrlSetData($pgbar,Int(@SEC))
EndFunc