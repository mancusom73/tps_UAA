#include <./lib/ButtonConstants.au3>
#include <./lib/EditConstants.au3>
#include <./lib/GUIConstantsEx.au3>
#include <./lib/GUIListBox.au3>
#include <./lib/StaticConstants.au3>
#include <./lib/WindowsConstants.au3>
#include <./lib/WinHttp.au3>
#include <./lib/WinHttpConstants.au3>
#include <./lib/Json.au3>
#include  <./lib/String.au3>
#include  <./lib/Date.au3>
#include  <./lib/GuiListView.au3>
#include  <./lib/Date.au3>
#include <./lib/File.au3>
;~  Este script solo redirecciona al script servivio QR que sea necesario  ****************

Global $Version = "Qr Gateway 1.00"

Global CONST $MercadoPago       = 1
Global CONST $Pluspago          = 2

Global CONST $MercadoPago_file       = "codigoqr.au3"
Global CONST $Pluspago_file          = "codigoqrpluspago.au3"

;sprintf(parametros,"%i %i %li %li %i %.2lf %i %i %i",sucursal,nro_caja ,caja_z, ram_id_evento, posicion,_PAGO_IMPORTE( i ), ram_modo_devolucion,reintento,id_pago);
Global  $CodSucursal, $NroCaja,$CajaZ, $IdEvento, $PosicionPago, $Importe, $EsDevolucion, $Reintento, $nro_servicio

if($CmdLine[0] = 9 ) Then ; ojo aca no olvidarse de modo produccion.
   $esproduccion = 1
   $CodSucursal = $CmdLine[1]
   $NroCaja = $CmdLine[2]
   $CajaZ = $CmdLine[3]
   $IdEvento = $CmdLine[4]
   $PosicionPago = $CmdLine[5]
   $Importe = $CmdLine[6]
   ;$NroOrden =GenerarOrdenAleatoria()
   $EsDevolucion =$CmdLine[7]
    $Reintento =$CmdLine[8]
	$nro_servicio =$CmdLine[9]
    ConsoleWrite(" sucursal " & $CodSucursal & " caja_z"&  $CajaZ & " importe " & $Importe & " Devolucion "&  $EsDevolucion & " Reintento "& $Reintento & " Servicio "& $nro_servicio &@LF)

EndIf

Local $sParamet=$CodSucursal & " " &$NroCaja & " " &  $CajaZ& " " &  $IdEvento &" " &$PosicionPago  & " " & $Importe & " " & $EsDevolucion&  " " &$Reintento
Local $sWorkingDir = ""
Local $archivo_script = ""

Switch $nro_servicio
  Case $MercadoPago
	 $archivo_script =@ScriptDir &"\codigoqr.au3"
	Case $Pluspago
	  $archivo_script = @ScriptDir &"\codigoqrpluspago.au3"
 EndSwitch

Local $iPID = RunWait ('"' & @AutoItExe & '" /AutoIt3ExecuteScript "' & $archivo_script  & '" ' & $sParamet , $sWorkingDir, @SW_HIDE, $STDIN_CHILD)

ConsoleWrite("me devolvio codigo " & $iPID )
Exit( $iPID);