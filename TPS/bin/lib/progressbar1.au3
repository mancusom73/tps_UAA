#include <GUIConstantsEx.au3>
#include <ProgressConstants.au3>
#include <WindowsConstants.au3>


$count=1
$x=0
$increment=0
$Form1 = GUICreate("Form1", 321, 239, 193, 125)
$Progress1 = GUICtrlCreateProgress(48, 48, 209, 33)
;$Progress2 = GUICtrlCreateProgress(48, 96, 209, 41)
GUISetState(@SW_SHOW)

while $increment<100
    if $count=1 then $x += 1
    if $x>99 then $count=2
    if $count=2 then $x -= 1
    if $x<1 then $count=1
    GUICtrlSetData($Progress1,$x)
 ;   GUICtrlSetData($Progress2,$increment/10)
    sleep(50)
    $increment += 1
;need to increment second loop with your file operations
WEnd   

GUIDelete($form1)
msgbox(0,"","Done")

