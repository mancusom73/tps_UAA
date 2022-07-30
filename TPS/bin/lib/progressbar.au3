;#include <Math.au3>

Global Const $DLG_NOTONTOP = 2
Global Const $GUI_EVENT_CLOSE2 = -3

Local $mensaje

If $CmdLine[0] = 1 Then
	$mensaje = $CmdLine[1]
else
	$mensaje ="Conectandose al Servidor"
Endif 

HotKeySet("{BACKSPACE}", "Cerrar")
;HotKeySet("{ESCAPE}", "Cerrar")
; simulation of the freeware FairStars CD Ripper Installation ( http://www.fairstars.com/download/fscdripper_setup.exe )
; dirgetsize of @ProgramFilesDir & '\FairStars CD Ripper' give 6331609 bits after install.
$count=1
$x=0
$increment=0
ProgressOn ( "Conectando", $mensaje, "Aguarde..." ,@DesktopWidth - 350,@DesktopHeight - 200,$DLG_NOTONTOP)
while $increment<100
    if $count=1 then $x += 1
    if $x>99 then $count=2
    if $count=2 then $x -= 1
    if $x<1 then $count=1
    ;GUICtrlSetData($Progress1,$x)
    ProgressSet ( $increment )
	sleep(50)
    $increment += 1
	if $increment=100 then $increment = 1
WEnd

ProgressSet ( 100, "Finalizado !" )
Sleep ( 1000 )
ProgressOff ( )

Func Cerrar()
	Exit
EndFunc   ;==>TogglePause