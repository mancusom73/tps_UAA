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
#Region ### START Koda GUI section ### Form=C:\work_svn\TPS-LAREINA\bin\bakau3\propios\lib\codigoqrprueba.kxf
;~  los parametros son cod_sucursal, nro_caja ,caja_z , id_evento , codposicion de pago  ****************
;$CmdLine[0] ; Contains the total number of items in the array.
;$CmdLine[1] ; cod_sucursal
;$CmdLine[2] ; nro_caja
;$CmdLine[3] ; caja_z
;$CmdLine[4] ; id_evento
;$CmdLine[5] ; posicion_pago
;$CmdLine[6] ; importe
;$CmdLine[7] ; devolucon  0 no es 1 si es
;$CmdLine[8] ; reintento de mismo evento


#Region ### Variable de respuesta ###
Global $Aprobada = 2

;~ HotKeySet("{ESC}", "_Terminate")
;HotKeySet("{UP}", "_Arriba")
;HotKeySet("{DOWN}", "_Abajo")
HotKeySet("{Enter}", "_PressButton")
;HotKeySet("{Enter}", "_PressButton")
;HotKeySet("{C}", "_PressButton3")
;HotKeySet("{c}", "_PressButton3")
;HotKeySet("{B}", "_PressButton2")
;HotKeySet("{b}", "_PressButton2")
;HotKeySet("{A}", "_PressButton1")
;HotKeySet("{a}", "_PressButton1")
;HotKeySet("{LEFT}", "_PressButtonLeft")
;HotKeySet("{RIGHT}", "_PressButtonRight")



Global Enum $WinHttpRequestOption_UserAgentString, _
            $WinHttpRequestOption_URL, _
            $WinHttpRequestOption_URLCodePage, _
            $WinHttpRequestOption_EscapePercentInURL, _
            $WinHttpRequestOption_SslErrorIgnoreFlags, _
            $WinHttpRequestOption_SelectCertificate, _
            $WinHttpRequestOption_EnableRedirects, _
            $WinHttpRequestOption_UrlEscapeDisable, _
            $WinHttpRequestOption_UrlEscapeDisableQuery, _
            $WinHttpRequestOption_SecureProtocols, _
            $WinHttpRequestOption_EnableTracing, _
            $WinHttpRequestOption_RevertImpersonationOverSsl, _
            $WinHttpRequestOption_EnableHttpsToHttpRedirects, _
            $WinHttpRequestOption_EnablePassportAuthentication, _
            $WinHttpRequestOption_MaxAutomaticRedirects, _
            $WinHttpRequestOption_MaxResponseHeaderSize, _
            $WinHttpRequestOption_MaxResponseDrainSize, _
            $WinHttpRequestOption_EnableHttp1_1, _
            $WinHttpRequestOption_EnableCertificateRevocationCheck

Global CONST $WinHttpRequestOption_SslErrorIgnoreFlags_UnknownCA       = 0x0100
Global CONST $WinHttpRequestOption_SslErrorIgnoreFlags_CertWrongUsage  = 0x0200
Global CONST $WinHttpRequestOption_SslErrorIgnoreFlags_CertCNInvalid   = 0x1000
Global CONST $WinHttpRequestOption_SslErrorIgnoreFlags_CertDateInvalid = 0x2000
Global CONST $WinHttpRequestOption_SslErrorIgnoreFlags_IgnoreAll       = 0x3300  ;IGNORE ALL OF THE ABOVE


Global CONST $MaxIntentos = 40
Global $Titulo = "MercadoPago POS 3.50"
Global $blinkear = 0
Global $reintentos =0
Global $errorformato = 0
Local $huboerror = 0

GlobaL $ModoProduccion = True
$RED = 1
Global  $hour, $min, $sec
Global  $IpServidor, $PortServidor, $CodSucursal, $NroCaja,$CajaZ, $IdEvento, $PosicionPago, $Importe, $EsDevolucion, $Reintento
Global $sQrIni= @ScriptDir & "\qrmercadopago.ini"

Local $NroOrden
Local $esproduccion = 0

$IpServidor = "10.9.0.1"
$PortServidor = "8091"

#Region ### Codigos de repuesta de la aplicacion ###
; 2 no se autorizo  3 ok,  0 cerro  el script

if($CmdLine[0] = 8 ) Then ; ojo aca no olvidarse de modo produccion.
   $esproduccion = 1
   $CodSucursal = $CmdLine[1]
   $NroCaja = $CmdLine[2]
   $CajaZ = $CmdLine[3]
   $IdEvento = $CmdLine[4]
   $PosicionPago = $CmdLine[5]
   $Importe = $CmdLine[6]
   $NroOrden =GenerarOrdenAleatoria()
   $EsDevolucion =$CmdLine[7]
    $Reintento =$CmdLine[8]
    ConsoleWrite2(" sucursal " & $CodSucursal & " caja_z"&  $CajaZ & " importe " & $Importe & " orden "& $NroOrden &  " Devolucion "&  $EsDevolucion & " Reintento "&      $Reintento & @LF)
 Else
	$CodSucursal = 1
   $NroCaja = 1
   $EsDevolucion = 0
	if( $CmdLine[0] = 1) Then
	   $ModoProduccion = False
	EndIf

EndIf



LeerIni()
Global $begin = TimerInit()

$Form1 = GUICreate($Titulo, 700, 400, 50, 100)
GUISetBkColor(0xA6CAF0)
$lstbody = GUICtrlCreateList("", 256, 32, 281, 81, BitOR($LBS_STANDARD, $LBS_EXTENDEDSEL))



;~ $lstjson = GUICtrlCreateList("", 520, 421, 281, 110, BitOR($ES_WANTRETURN,$WS_HSCROLL,$WS_VSCROLL))
$Label1 = GUICtrlCreateLabel("Respu  Token", 256, 8, 81, 17)
$Label2 = GUICtrlCreateLabel("Respuesta json", 528, 300, 77, 17)
;$btnloguin = GUICtrlCreateButton("Obtener Merchar orden", 48, 40, 185, 49)

;$btngenerarorden = GUICtrlCreateButton("A -Generar Orden", 64, 30, 169, 81,0)
;GUICtrlSetImage($btngenerarorden, "shell32.dll", 241,1)
;la parte de atras del boton simulando borde
;$atras1 = GUICtrlCreateLabel("", 62, 28, 173, 85, $WS_CLIPSIBLINGS)
;GUICtrlSetBkColor(-1, 0x0000FF)
;GUICtrlSetState(-1, $GUI_DISABLE)


$monto = GUICtrlCreateInput(StringFormat("%#.2f", 0), 256, 160, 121, 21)

GUICtrlSetState(-1, $GUI_DISABLE)
$Label3 = GUICtrlCreateLabel("Monto orden", 256, 136, 64, 17)
$btnobtener = GUICtrlCreateButton("Obtener Orden Publicada", 288, 256, 169, 81)
;$btnsalir = GUICtrlCreateButton("B -Salir", 64, 120, 169, 81, $WS_BORDER)
;GUICtrlSetImage($btnsalir , "shell32.dll", 28,1)

;la parte de atras del boton simulando borde
;$atras2 = GUICtrlCreateLabel("", 62,118, 173, 85, $WS_CLIPSIBLINGS)
;GUICtrlSetBkColor(-1, 0x0000FF)
;GUICtrlSetState(-1, $GUI_DISABLE)
;GUICtrlSetState( -1, $GUI_HIDE)

$btnaborrarorden = GUICtrlCreateButton("C -Borrar Orden Pendiente", 64, 376, 169, 65, $WS_BORDER)
GUICtrlSetImage($btnaborrarorden , "shell32.dll", 200,1)

;la parte de atras del boton simulando borde
;$atras3 = GUICtrlCreateLabel("", 62,374, 173, 69, $WS_CLIPSIBLINGS)
;GUICtrlSetBkColor(-1, 0x0000FF)
;GUICtrlSetState(-1, $GUI_DISABLE)
;GUICtrlSetState( -1, $GUI_HIDE)

$txtcodigorespuesta = GUICtrlCreateInput("", 544, 72, 73, 21)
GUICtrlSetFont(-1, 8, 800, 0, "MS Sans Serif")
GUICtrlSetColor(-1, 0x008000)
GUICtrlSetState(-1, $GUI_DISABLE)
$Label4 = GUICtrlCreateLabel("Codigo de respuesta", 408, 72, 130, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")

$txtnroorden = GUICtrlCreateInput("", 544, 32, 105, 21)
GUICtrlSetFont(-1, 8, 800, 0, "MS Sans Serif")
GUICtrlSetColor(-1, 0x008000)
GUICtrlSetState(-1, $GUI_DISABLE)
$Label5 = GUICtrlCreateLabel("Nro de Orden", 408, 32, 85, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")

$txtstatus = GUICtrlCreateInput("", 544, 112, 73, 21)
GUICtrlSetFont(-1, 8, 800, 0, "MS Sans Serif")
GUICtrlSetColor(-1, 0x008000)
GUICtrlSetState(-1, $GUI_DISABLE)
$Label6 = GUICtrlCreateLabel("Status Orden", 408, 112, 81, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")

$editmensajes = GUICtrlCreateEdit("", 503, 304, 191, 90,Bitor($ES_WANTRETURN, $WS_VSCROLL, $ES_AUTOVSCROLL))
GUICtrlSetState(-1, $GUI_DISABLE)

$txtimporteorden = GUICtrlCreateInput("txtimporteorden", 544, 152, 73, 21)
GUICtrlSetFont(-1, 8, 800, 0, "MS Sans Serif")
GUICtrlSetColor(-1, 0x008000)
GUICtrlSetState(-1, $GUI_DISABLE)
GUICtrlCreateLabel("Importe Orden", 408, 152, 89, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")

;~ $lblmensajes = GUICtrlCreateLabel("_", 16, 496, 487, 40)
$lblmensajes = GUICtrlCreateLabel("_", 48, 40, 350, 312)
GUICtrlSetFont(-1, 14, 800, 0, "MS Sans Serif")
GUICtrlSetColor(-1, 0xFF0000)
$btncambiarmonto = GUICtrlCreateButton("Camb Monto de Devolucion", 256, 184, 157, 25)
$btncambiarorden = GUICtrlCreateButton("Camb Nro de orden", 256, 210, 137, 25)
$Pic1 = GUICtrlCreatePic(@ScriptDir & "/mercadopago.gif", 20, 208, 272, 128)
$Group1 = GUICtrlCreateGroup("Envio / Recepcion MercadoPago", 400, 0, 289, 297)
GUICtrlSetFont(-1, 10, 800, 0, "MS Sans Serif")
GUICtrlCreateLabel("Nro Sucursal", 408, 197, 81, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")
GUICtrlCreateLabel("Nro Caja", 408, 237, 57, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")
GUICtrlCreateLabel("Nro Sucursal", 408, 197, 81, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")
GUICtrlCreateLabel("Nro Caja", 408, 237, 57, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")
$txtsucursal = GUICtrlCreateInput("", 544, 197, 73, 21)
GUICtrlSetFont(-1, 8, 800, 0, "MS Sans Serif")
GUICtrlSetColor(-1, 0x008000)
GUICtrlSetState(-1, $GUI_DISABLE)
$txtnrocaja = GUICtrlCreateInput("", 544, 237, 73, 21)
GUICtrlSetFont(-1, 8, 800, 0, "MS Sans Serif")
GUICtrlSetColor(-1, 0x008000)
GUICtrlSetState(-1, $GUI_DISABLE)
GUICtrlCreateGroup("", -99, -99, 1, 1)
$chkdevolucion = GUICtrlCreateCheckbox("Modo Devolucion", 256, 224, 145, 25)
GUICtrlSetFont(-1, 10, 800, 0, "MS Sans Serif")
GUICtrlSetColor(-1, 0x000000)
GUICtrlSetState(-1, $GUI_CHECKED)
GUICtrlSetState(-1, $GUI_DISABLE)
GUICtrlCreateLabel("Id Pago", 408, 272, 51, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")
$txtidpago = GUICtrlCreateInput("", 544, 272, 115, 21)
GUICtrlSetFont(-1, 8, 800, 0, "MS Sans Serif")
GUICtrlSetColor(-1, 0x008000)
GUICtrlSetState(-1, $GUI_DISABLE)
$txtcambiarmonto = GUICtrlCreateCheckbox("Cambiar Importe", 416, 160, 105, 25)
Desactivar()

GUISetState(@SW_SHOW)

Global $aKeys[7] = ["code","referencia_orden","status","importe_pagado", "importe","accessToken","message" ] ;<== Add an array of controls
;~ "mp_http_status",
Global $aControles[7] = [$txtcodigorespuesta,$txtnroorden,$txtstatus, $monto,$txtimporteorden,$lstbody,$editmensajes] ;<== Add an array of controls
Log2("Iniciando Aplicacion QR " & $Titulo)


;~  no funciona Local
$levantado = LevantadoQR2()
if $levantado = 0 Then
   Log2("NO HAY CONEXION CON EL SERVICIO DE MERCADO PAGO ")
   MsgBox(48 + $MB_SYSTEMMODAL, "ERROR GRAVE", "NO HAY CONEXION CON EL SERVICIO DE MERCADO PAGO " & @CRLF &  "POR FAVOR AVISE AL SUPERVISOR - Sale en 3 Seg", 3  )
     FocusTPS()
	 Exit(2)
EndIf

if( AccesTokendeHoy() = -1) Then
   Log2("NO PUDO PEDIR TOKEN SERVIDOR ")
   MsgBox(48 + $MB_SYSTEMMODAL, "ERROR GRAVE", "NO SE PUDO OBTENER AUTORIZACION DEL SERVIDOR " & @CRLF &  "POR FAVOR AVISE AL SUPERVISOR - Sale en 3 Seg",3  )
     FocusTPS()
	 Exit(2)
EndIf
CargarToken()
#EndRegion ### END Koda GUI section ###
;~ WinSetOnTop($Titulo, "", 1)



Local $sec = @SEC


if( $esproduccion = 1 ) Then
   SetearTxt($Importe, $txtimporteorden)
   SetearTxt($CodSucursal, $txtsucursal)
   SetearTxt($NroCaja, $txtnrocaja)
   SetearTxt($NroOrden, $txtnroorden)
EndIf

LeerNroOrden()
ActivarMensaje()
SetearTxt("PREPARANDO ENVIO", $lblmensajes)
Sleep(1000)
;~ DesactivarMensaje()
;~ mandamos directamente la orden
Log2("Click en generar orden ")
DesactivarMensaje()
$reintentos = 0
;GUICtrlSetState($btngenerarorden ,$GUI_DISABLE)
HotKeySet("{Enter}")
if( $EsDevolucion = 0) Then
   TratarOrden()
   Salida()
Else
   Devolucion( GUICtrlRead($txtnroorden ))
   Salida()
EndIf

While 1
	$nMsg = GUIGetMsg()
	Switch $nMsg
	Case $GUI_EVENT_CLOSE
		DesactivarMensaje()
;~ 			Exit
		 Case $btnobtener
			DesactivarMensaje()
			MsgBox($MB_SYSTEMMODAL, "", "Trayendo ordenes pendientes")
;~ 			ObtenerOrden( false )
			EnviarRequest(2)
;		 Case $btnloguin
;		 DesactivarMensaje()
;~ 			merchar order
;~ 			EnviarRequest(4)
;~ aca pruebo leer
;			LeerJsonCompuesto(4)
;		 Case $btngenerarorden
;			Log2("Click en generar orden ")
;			 DesactivarMensaje()
;~ 			GenerarOrden()
;			$reintentos = 0
;			GUICtrlSetState($btngenerarorden ,$GUI_DISABLE)
;			HotKeySet("{Enter}")
;			if( $EsDevolucion = 0) Then
;			   TratarOrden()
;			Else
;~ 			   validemos que sea un numero lo ingresado
;
			   Devolucion( GUICtrlRead($txtnroorden ))
;			 EndIf

		 Case $btnaborrarorden
			 DesactivarMensaje()
;~ 			probando
;~ 			LeerJsonCompuesto()
			EnviarRequest(3)
;~ 			SetearTxt("Mi prueba", $txtcodigorespuesta)
		 Case $btncambiarmonto
;~ 			   la uso por si tengo que cambiar el monto de la orden para la homologacion
			   SetearTxt(StringFormat("%#.2f", GUICtrlRead($monto )), $txtimporteorden)
			   GrabarNroOrden(0)
;~  			   MsgBox($MB_SYSTEMMODAL, '', StringFormat("%#.2f", GUICtrlRead($monto ))
;~ 			   MsgBox($MB_SYSTEMMODAL, '',StringFormat("$"& "%#.2f", GUICtrlRead($monto )) )
		 case $btncambiarorden:
			LeerNroOrden()
;		 case $btnsalir
;			ConsoleWrite2("Boton salir Salgo con codigo " & $Aprobada & @LF)
 ;			Exit $Aprobada
		 case $txtcambiarmonto
			If GUICtrlRead($txtcambiarmonto) = $GUI_CHECKED Then
;~                 GUICtrlSetState($monto, $GUI_ENABLE)
				Local $sValue = InputBox("Ingrese Monto", "Ingrese Valor Mayor a 0 y menor a 1.55", "1.55", " M8")
				ConsoleWrite2("Igreso  " & $sValue & @LF)
				if( $sValue > 0) Then
				   if (ValidarNumero($sValue) = False ) Then
					  ActivarMensaje()
					 GUICtrlSetData($lblmensajes,"IMPORTE INGRESADO INCORRECTO ")
				  Else
					  SetearTxt($sValue, $monto)
					  SetearTxt($sValue, $txtimporteorden)
					  ActivarMensaje()
					 SetearTxt("IMPORTE DEVOLUCION MODIFICADO", $lblmensajes)
					 GrabarNroOrden(1)
				   EndIf

				EndIf

            Else
                GUICtrlSetState($monto, $GUI_DISABLE)
            EndIf
	EndSwitch
WEnd

Func Desactivar()
   GUICtrlSetState( $Label1 , $GUI_HIDE)
   GUICtrlSetState( $Label2, $GUI_HIDE)
   ;GUICtrlSetState($btnloguin, $GUI_HIDE)


   GUICtrlSetState($btnaborrarorden, $GUI_HIDE)


   GUICtrlSetState($btncambiarorden, $GUI_HIDE)
   GUICtrlSetState($lstbody, $GUI_HIDE)
;~    GUICtrlSetState($monto , $GUI_HIDE)
   GUICtrlSetState($Label3 , $GUI_HIDE)
   GUICtrlSetState($Label3 , $GUI_HIDE)

   GUICtrlSetData($txtsucursal,$CodSucursal )
   GUICtrlSetData($txtnrocaja,$NroCaja )
   GUICtrlSetState( $btnobtener, $GUI_HIDE)

   GUICtrlSetState($btncambiarmonto, $GUI_DISABLE)

   if( $ModoProduccion = False) Then
	  ;GUICtrlSetData($btngenerarorden, "NO DISPONIBLE")
	  ;GUICtrlSetState( $btngenerarorden, $GUI_DISABLE)
	   ActivarMensaje()
	   SetearTxt("MODO PRUEBA - NO OPERATIVO ", $lblmensajes)

   EndIf
   if( $EsDevolucion = 0 )Then
	  GUICtrlSetState(  $chkdevolucion, $GUI_HIDE)
	  GUICtrlSetState(  $monto, $GUI_HIDE)
	  GUICtrlSetState($btncambiarmonto, $GUI_HIDE)
	  GUICtrlSetState($txtcambiarmonto,$GUI_HIDE)
   Else
	  ;GUICtrlSetData($btngenerarorden, "A -Devolucion Orden")
	  GUICtrlSetState(  $monto, $GUI_DISABLE)

   EndIf



EndFunc

Func TratarOrden()
;~ 2reintentar 0 ok 1 error
Local $accion = 2
$accion =  EnviarRequest(1)
 ConsoleWrite2("Respuesta orden  " & $accion  & @LF)
if($accion == 1 and $errorformato = 0 ) Then ; reenviamos porque el token se pidio
   $accion =  EnviarRequest(1)
EndIf

;~ si la accion me dice que hay que pedir token oJO
Local $respu= 1
While $reintentos <= $MaxIntentos And $accion =  2
;~     MsgBox($MB_SYSTEMMODAL, "", "Value of $i is: " & $i)

	; Delete el archivo
	Local $iDelete = FileDelete(@ScriptDir & "\trans_id.txt")
	If $iDelete Then
		ConsoleWrite2("Borrado archivo trans_id ok "  & @LF)
	Else
		ConsoleWrite2("Borrado archivo trans_id FALLO O NO EXISTE"  & @LF)
	EndIf
    ConsoleWrite2("Mando consulta mercharorder " & @LF)
	$accion =  EnviarRequest(4)
	 ConsoleWrite2("Respuesta merchar " & $accion  & @LF)
    $reintentos = $reintentos + 1
	ActivarMensaje()
    GUICtrlSetData($lblmensajes,"ESPERANDO LECTURA QR" & @LF & @LF & "****ESPERE POR FAVOR****")
    ;if(IsPar($reintentos) = True ) Then
	if( Mod($reintentos, 3) = 0 ) Then  ;cada 3 preguntanmos
	  $respu = MsgBox($MB_YESNO + $MB_ICONQUESTION + $MB_SYSTEMMODAL +$MB_DEFBUTTON2, "MENSAJE POS","DESEA CANCELAR? " & @CRLF & "PRESIONE S o Y ...SINO CONTINUA SOLO EN 5 SEG", 8  )
	  if( $respu = $IDYES ) Then
		 $reintentos = $MaxIntentos + 1 ; para salir
		 GUICtrlSetData($lblmensajes,"CANCELANDO ORDEN ENVIADA")
		 ConsoleWrite2("Cancelo el cajero seguir esperando " & @LF)
	  EndIf
	  ;~ 	espero 5 segundos
   EndIf
   if( $respu <> $IDNO and $accion <> 0 ) Then
	  Sleep(5000)
	EndIf
    $respu= 1
	ConsoleWrite2("Reitentos: " & $reintentos & "accion "& $accion & @LF)
WEnd
if( $reintentos > $MaxIntentos ) Then
;~ aca salio por timeout borrar la orden
		 MsgBox($MB_OK + $MB_ICONINFORMATION + $MB_SYSTEMMODAL, "MENSAJE MERCADOPAGO","SE VENCIO EL TIEMPO DE ESPERA Sale en 3 Seg", 3  )
		  ConsoleWrite2("SE VENCIO EL TIEMPO DE ESPERA Sale en 3 Seg " & @LF)
		  ActivarMensaje()
		 GUICtrlSetData($lblmensajes,"ORDEN ANULADA ")
		  ConsoleWrite2("ORDEN ANULADA" & @LF)
		 EnviarRequest(3)
Else
	  if( $accion = 0 and $huboerror = 0 ) Then
;~    aca salio porque encontro la merchar order
		  MsgBox($MB_OK + $MB_ICONINFORMATION + $MB_SYSTEMMODAL, "MENSAJE MERCADOPAGO","SE REGISTRO EL PAGO EN MERCADOPAGO MUCHAS GRACIAS Sale en 3 Seg", 3 )
;~ 		  ConsoleWrite("1 - se registro"& @LF)
		  ActivarMensaje()
		  $Aprobada = 3
		 GUICtrlSetData($lblmensajes,"ORDEN PAGADA REGISTRADA")
		 GuardarOrden()
	  Else
		  MsgBox($MB_OK + $MB_ICONINFORMATION + $MB_SYSTEMMODAL, "MENSAJE MERCADOPAGO","ERROR EN LA GENERACION DE LA ORDEN - Sale en 3 Seg" , 3 )
;~ 		  ConsoleWrite("1 - se registro"& @LF)
		  ActivarMensaje()
		 GUICtrlSetData($lblmensajes,"ERROR EN LA GENERACION DE LA ORDEN")

	  EndIf
EndIf

EndFunc

Func GuardarOrden()
;~    guardamos un historico de pagos por las dudas? hace falta?
   Local $hFileOpen = FileOpen(@ScriptDir & "\ordenhis.txt", $FO_APPEND)
;~    guardar nro de orden, fecha, monto
 Local $orden, $fecha, $monto
   $orden =  GUICtrlRead($txtnroorden )
   $fecha = _DateToDayValue(@YEAR,@MON,@MDAY)
   Dim $Y, $M, $D
   $fecha=_DayValueToDate($fecha, $Y, $M, $D)
   $fecha= StringFormat("%02i-%02i-%04i", $D,  $M,  $Y)
   $monto = GUICtrlRead($txtimporteorden)
   if($EsDevolucion = 0 ) Then
	  Local $tipo ="COMPRA"
   Else
	  Local $tipo ="DEVOLUCION"
   EndIf
   ConsoleWrite2($fecha & @CRLF)
   Local $cadena = $orden &"," & $fecha &","& $monto & ","& $tipo
   Local $aArray2[1] = [$cadena]
;~    _ArrayDisplay($aArray2,"Original", Default, 8)
;~    ConsoleWrite($cadena & @CRLF)
   _FileWriteFromArray($hFileOpen, $aArray2, Default, Default, @CRLF)
   FileClose($hFileOpen)
EndFunc



Func LeerNroOrden()
Local $ordenaleatoria

Local $Json1 = FileRead(@ScriptDir & "\orden.json")
FileClose(@ScriptDir & "\orden.json")

Local $Data1 = Json_Decode($Json1)
Local $Json2 = Json_Encode($Data1)

Local $Data2 = Json_Decode($Json2)
Local $Json3 = Json_Encode($Data2)
Local $Obj = Json_ObjCreate()

if( $EsDevolucion = 1 ) Then
   if( $esproduccion = 1) Then
	  $ordenaleatoria = GenerarOrdenAleatoria()
	  SetearTxt($ordenaleatoria, $txtnroorden)
	  SetearTxt($Importe, $monto)
   Else
;~ 	  en la devolucion de pruebas el referencia_orden lo saco del json hardcodeado
	  Local $valorkey = Json_ObjGet($Data2, "referencia_orden")
	  $ordenaleatoria = $valorkey
	  SetearTxt($ordenaleatoria, $txtnroorden)
	  $valorkey = Json_ObjGet($Data2, "importe")
	  SetearTxt($valorkey, $txtimporteorden)
	  SetearTxt($valorkey, $monto)
   EndIf

;~    Devolucion( $ordenaleatoria)
   Return 1
EndIf

$ordenaleatoria = GenerarOrdenAleatoria()

;busco por si quedo de pluspago el tag para borrarlo
Local $valorkey = Json_ObjExists($Data2, "id_transaccion_interno")
if( $valorkey ) Then
	Json_ObjDelete($Data2,"id_transaccion_interno")
	ConsoleWrite2("Borro referencia_orden de pluspago que no va "& @LF)
	Json_ObjPut($Data2, "referencia_orden", $ordenaleatoria)
endif

Local $valorkey = Json_ObjExists($Data2, "referencia_orden")

if( $valorkey ) Then

   $valorkey = Json_ObjGet($Data2,  "cod_sucursal")
   if( $esproduccion = 1 ) Then
	  Json_ObjPut($Data2, "cod_sucursal", Number($CodSucursal))
	  SetearTxt($CodSucursal, $txtsucursal)
   Else
	  SetearTxt($valorkey, $txtsucursal)
	EndIf


	$valorkey = Json_ObjGet($Data2,  "nro_caja")
	if( $esproduccion = 1 ) Then
	  Json_ObjPut($Data2, "nro_caja", Number($NroCaja))
	  SetearTxt($NroCaja, $txtnrocaja)
   Else
	  SetearTxt($valorkey, $txtnrocaja)
    EndIf


   $valorkey = Json_ObjGet($Data2,  "referencia_orden")
   Json_ObjPut($Data2, "referencia_orden", $ordenaleatoria)
   SetearTxt($ordenaleatoria, $txtnroorden)

   $valorkey = Json_ObjGet($Data2,  "importe")
   if( $esproduccion = 1 ) Then
	  Json_ObjPut($Data2, "importe", Number($Importe))
	  SetearTxt($Importe, $txtimporteorden)
   Else
	  Json_ObjPut($Data2, "importe", Number($valorkey ))
	  ConsoleWrite("Grabo json Importe " &Number($valorkey )& @CRLF)
	  SetearTxt($valorkey, $txtimporteorden)
	EndIf


	Local $hFileOpen = FileOpen(@ScriptDir & "\orden.json", $FO_OVERWRITE)
	$Json3 = Json_Encode($Data2)
	ConsoleWrite2("AAsi mando al json : " & $Json3 & @LF)
 	FileWrite($hFileOpen, $Json3)
EndIf
FileClose(@ScriptDir & "\orden.json")
EndFunc

Func GrabarNroOrden($devo)
;modifica el nro de orden y el importe de la orden del archivo
if( $devo = 0 ) Then
   Local $Json1 = FileRead(@ScriptDir & "\orden.json")
   FileClose(@ScriptDir & "\orden.json")
Else
   Local $Json1 = FileRead(@ScriptDir & "\devolucion.json")
   FileClose(@ScriptDir & "\devolucion.json")
EndIf

	Local $Data1 = Json_Decode($Json1)
	Local $Json2 = Json_Encode($Data1)

	Local $Data2 = Json_Decode($Json2)
	Local $Json3 = Json_Encode($Data2)

 	Local $Obj = Json_ObjCreate()
 	Local $valorkey = Json_ObjExists($Data2, "importe")

   Local $hFileOpen

   if( $valorkey ) Then

		   if( $devo = 0 ) Then
			Json_ObjPut($Data2, "importe", Number(GUICtrlRead($txtimporteorden )))
		 Else
			Json_ObjPut($Data2, "importe", Number(GUICtrlRead($monto )))
			EndIf

 		   $Json3 = Json_Encode($Data2)
;~ 		   GenerarOrdenAleatoria()
		   ConsoleWrite2("Test2 Result: " & $Json3 & @LF)
		   if( $devo = 0 ) Then
			   $hFileOpen = FileOpen(@ScriptDir & "\orden.json", $FO_OVERWRITE)
			Else
			   $hFileOpen = FileOpen(@ScriptDir & "\devolucion.json", $FO_OVERWRITE)
			EndIf
			FileWrite($hFileOpen, $Json3)
			FileClose($hFileOpen)
    EndIf


SetearTxt("SIN ENVIAR", $txtstatus)

EndFunc

Func GenerarOrdenAleatoria()
   Local $ordenaleatoria
   if($esproduccion = 1 )Then
	  $ordenaleatoria = $CodSucursal & $CajaZ & $IdEvento & $PosicionPago & $Reintento
   else
	  $ordenaleatoria = @Hour & @MIN & @SEC
    EndIf



   Return $ordenaleatoria
;~    pensaba horaminutossegundos


EndFunc

Func CargarToken()
Local $hFile=FileOpen("AccessToken.html",0)
 If $hFile = -1 Then
        ConsoleWrite2(@CRLF & "No encontre archivo token" & @CRLF)
        Return
    EndIf
Local $sFileRead = FileRead($hFile)
FileClose($hFile)
CargarDatos( true)
EndFunc

Func EnviarRequest($tipo_mensaje)
   ;1 crear orden, 2 consultar orden instore 3 eliminar orden instore 4 obtener mercharorder 5 pedir pagos 6 crear devolucion 7 pedir token
; The data to be sent seria la autorizacion el access token que necesitamos para las operaciones
$Bearer = GUICtrlRead($lstbody )
Local $micodigoerror = 0
 Switch $tipo_mensaje
 Case 1 ; crear orden
	;Leemos el json
    Local $sFileName="orden.json"
    Local $sFilePath=@ScriptDir & "\" & $sFileName
    Local $hFile=FileOpen($sFilePath,0);16=$FO_BINARY
    Local $sFileData=FileRead($hFile)
    FileClose($hFile)
	ConsoleWrite2("Lectura de json " &$sFileData & @CRLF)
 Case 5 ; devolucion
    Local $sFileName="\devolucion.json"
    Local $sFilePath=@ScriptDir & "\" & $sFileName
    Local $hFile=FileOpen($sFilePath,0);16=$FO_BINARY
    Local $sFileData=FileRead($hFile)
    FileClose($hFile)
	ConsoleWrite2("Lectura de json devolucion " &$sFileData & @CRLF)
 EndSwitch

 Local $oHttp   = Null, _
   $oComErr = Null
 Local $iHttpStatus = 0
 Local $sResponse = "", _
          $sPostData = ""

ConsoleWrite2(@CRLF & "Executing API TIPO MENSAJE " &$tipo_mensaje & @CRLF)
;Set COM error handler
    $oComErr = ObjEvent("AutoIT.Error", "com_error_handler")

 $oHttp = ObjCreate("winhttp.winhttprequest.5.1")
    If @error Then
        ConsoleWrite2("Unable to create http request object." & @CRLF)
        FocusTPS()
		Exit -1
    EndIf
 ConsoleWrite2("WinHttpRequest object created." & @CRLF)
 $oHttp.Option($WinHttpRequestOption_SslErrorIgnoreFlags) = $WinHttpRequestOption_SslErrorIgnoreFlags_IgnoreAll

  With $oHttp
	   Switch $tipo_mensaje
	   Case 1 ; crear orden
		  	 .Open("POST", "https://" & $IpServidor &":" & $PortServidor &"/instore_orders/", False)
			 ;subir a 6
			 .SetTimeouts("10000", "60000", "60000", "60000")
			 .SetRequestHeader("Authorization", "Bearer " & $Bearer)
			 .SetRequestHeader("Content-Type", "application/json")
			  Local $sData = ''
			$sPostData &= $sFileData
			GUICtrlSetColor($lblmensajes,0xff0000)
;~ 			SetearTxt("POR FAVOR ESCANEE EL CODIGO QR PARA TOMAR EL PAGO", $lblmensajes)
			SetearTxt("ENVIANDO ORDEN ....REVISE SU DISPOSITIVO", $lblmensajes)
			ActivarMensaje()
;~ 			ConsoleWrite("Mandando la orden json " &$sPostData & @CRLF)
		   .Send($sPostData)
;~ 		   $micodigoerror = @error
;~ 		   ConsoleWrite("Salgo del send con el siguiente codigo de error " & @error & " " & $micodigoerror & @CRLF)

		 Case 2 ; consultar orden
;~ 			.Open("GET", "https://" & $IpServidor &":" & $PortServidor &"/instore_orders/1?cod_sucursal=1' ", False)
			.Open("GET", "https://" & $IpServidor &":" & $PortServidor &"/instore_orders/"& $NroCaja & "?cod_sucursal=" & $CodSucursal & "' ", False)
			.SetTimeouts("600000", "600000", "600000", "600000")
			.SetRequestHeader("Authorization", "Bearer " & $Bearer)
			.SetRequestHeader("Content-Type", "application/json")
			.Send()
;~ 			$micodigoerror = @error
		 Case 3 ; borrar orden
;~ 			.Open("DELETE", "https://" & $IpServidor &":" & $PortServidor &"/instore_orders/1?cod_sucursal=1' ", False)
			.Open("DELETE", "https://" & $IpServidor &":" & $PortServidor &"/instore_orders/"& $NroCaja & "?cod_sucursal=" & $CodSucursal & "' ", False)
			.SetTimeouts("600000", "600000", "600000", "600000")
			.SetRequestHeader("Authorization", "Bearer " & $Bearer)
			.SetRequestHeader("Content-Type", "application/json")
			.Send()
;~ 			$micodigoerror = @error
		 Case 4 ; merchar order
;~ 			http://127.0.0.1:8091/merchant_orders/2
			Local $mercharorder = "https://" & $IpServidor &":" & $PortServidor &"/merchant_orders/" & GUICtrlRead($txtnroorden )
;~ 			ConsoleWrite("el merchar order es  " & $mercharorder &@CRLF)
			.Open("GET", $mercharorder, False)
 			.SetTimeouts("50000", "50000", "50000", "50000")
 			.SetRequestHeader("Authorization", "Bearer " & $Bearer)
 			.SetRequestHeader("Content-Type", "application/json")
 			.Send()
;~ 			MsgBox($MB_OK + $MB_ICONINFORMATION + $MB_SYSTEMMODAL, "","el nro de orden a buscar es " &  GUICtrlRead($txtnroorden )   )
		  Case 5 ; devoluciones
			Local $devolucionorden = "https://" & $IpServidor &":" & $PortServidor &"/refunds/"
;~ 			ConsoleWrite("el merchar order es  " & $mercharorder &@CRLF)
			.Open("POST", $devolucionorden, False)
 			 .SetTimeouts("10000", "30000", "30000", "30000")
 			.SetRequestHeader("Authorization", "Bearer " & $Bearer)
 			.SetRequestHeader("Content-Type", "application/json")
			Local $sData = ''
			$sPostData &= $sFileData
			GUICtrlSetColor($lblmensajes,0xff0000)
			SetearTxt("ENVIANDO DEVOLUCION ....", $lblmensajes)
			ActivarMensaje()
 			.Send($sPostData)
		Case 6 ; consultas de pago
			Local $consultapago = "https://" & $IpServidor &":" & $PortServidor &"/payments/"& GUICtrlRead($txtidpago )
;~ 			ConsoleWrite("el merchar order es  " & $mercharorder &@CRLF)
			.Open("GET", $consultapago, False)
 			 .SetTimeouts("10000", "30000", "30000", "30000")
 			.SetRequestHeader("Authorization", "Bearer " & $Bearer)
 			.SetRequestHeader("Content-Type", "application/json")
			.Send()
	  EndSwitch

if( @error = -2147352567) Then
;~    salida por timeout
	  ConsoleWrite2("Salida por timeout " & @error &@CRLF)
	  if($tipo_mensaje = 1) Then
;~ 		 si salio por timeout y es mensaje 1 debemos seguir esperando
		 Return Quehacemo(-1, "", $tipo_mensaje)
;~ 		 Return 2
	  EndIf

	  ActivarMensaje()
	  GUICtrlSetData($lblmensajes,"ERROR DE COMUNICACION - REINTENTE DE NUEVO")
	  ;GUICtrlSetState($btngenerarorden ,$GUI_ENABLE)
	  if($tipo_mensaje = 4) Then
		 Return 1
	  EndIf
	  Return 0
else
   $iHttpStatus = .Status
   $sResponse   = .ResponseText
   GUICtrlSetData($lblmensajes,"")
   $blinkear = 0
   AdlibUnRegister("SetTime")
   AdlibUnRegister("ControlarTime")
EndIf
EndWith
If $iHttpStatus <> 200 and  $iHttpStatus <> 201 then
		 ConsoleWrite2("1-HTTP Status  : " & String($iHttpStatus) & @CRLF)
         ConsoleWrite2("HTTP Response: " & @CRLF & $sResponse & @CRLF)


Else
	   ConsoleWrite2("2-HTTP Status  : " & String($iHttpStatus) & @CRLF)
         ConsoleWrite2("2- HTTP Response: " & @CRLF & $sResponse & @CRLF)
;~ 	   if $tipo_mensaje = 1 Then
;~ 		  ActivarMensaje()
;~ 		 GUICtrlSetData($lblmensajes,"ORDEN PAGADA REGISTRADA")
;~ 		 $Aprobada =3
;~ 	   EndIf
EndIf

;~ recibi un 200 o similar por ende tenemos respuesta
 $file = FileOpen("Received.html", 2) ; The value of 2 overwrites the file if it already exists
 FileWrite($file, $sResponse)
 FileClose($file)

 $seguir = Quehacemo(String($iHttpStatus), $sResponse,$tipo_mensaje)
 ConsoleWrite2('$seguir es' & $seguir &  @LF)
 if( $seguir <> 1 and  $seguir <> 0) Then
	Return $seguir
 EndIf
 if( $seguir = 1 and  $tipo_mensaje = 1) Then
	Return $seguir
 EndIf
 if( $seguir = 1 and  ($tipo_mensaje = 4  or $tipo_mensaje = 6) and $Esdevolucion = 1 ) Then
	Return $seguir
 EndIf
 if( $seguir =  0 and  ($tipo_mensaje = 5 or $tipo_mensaje = 6) and $Esdevolucion = 1 ) Then
	Return $seguir
 EndIf
 CargarDatos(False)
 LeerJsonCompuesto($tipo_mensaje)
EndFunc


Func Quehacemo($iHttpStatus, $sResponse, $tipo_mensaje)
;~ 0 ok  1 error  2 reintentar
 ConsoleWrite2('Quehacemo httpStatus' & $iHttpStatus &  @LF)
   Switch $iHttpStatus
   Case 401
	  ConsoleWrite2('FALLO AUTENTICACION PEDIMOS TOKEN DE SEGURIDAD' & @CRLF)
 	  post_authentication_test()
	  ActivarMensaje()
	  GUICtrlSetData($lblmensajes,"POR FAVOR REINTENTE DE NUEVO")
	  Return 1
   Case 405
      if($tipo_mensaje = 4) Then
		 ActivarMensaje()
		 GUICtrlSetData($lblmensajes,"ERROR ORDEN o NO HAY ORDENES")
		 Return 2
	  EndIf
	  if($tipo_mensaje = 5) Then
		 ActivarMensaje()
		 GUICtrlSetData($lblmensajes,"ERROR EN PROCESO DE DEVOLUCION")
		 SetearTxt("Error",$txtstatus)
		 Return 1
	  EndIf
   Case 400 ; bad request
      if($tipo_mensaje = 1) Then
		 ActivarMensaje()
		 GUICtrlSetData($lblmensajes,"ERROR ORDEN GENERADA REVISE LA MISMA")
		 Return 1
	  EndIf
   Case 500 ; erro aplicacion
		 ActivarMensaje()
		 GUICtrlSetData($lblmensajes,"ERROR EN CONFIGURACION POS")
		 ConsoleWrite2("ERROR EN CONFIGURACION POS" & @CRLF)
	    Return 1
   Case -1
;~ 	  timeout
	  ConsoleWrite2("Fue que hacemos por timeout  : " & @CRLF)
	  Return 2
   Case 200
	  if($tipo_mensaje = 4 OR $tipo_mensaje = 6) Then
;~ 		 este tipo de mensaje necesito saber si esta la orden o no
		 ConsoleWrite2("Chequear respuesta json merchar o id_pago" & @CRLF)
	    Return  LeerJsonCompuesto($tipo_mensaje)
;~ 	    Return 2
	 EndIf
  Case  201
	  if($tipo_mensaje = 5 OR $tipo_mensaje = 1 ) Then
;~ 		 devolvio algo la devolucion
		 ConsoleWrite2("Chequear respuesta devolucion / O compra" & @CRLF)
	    Return  LeerJsonCompuesto($tipo_mensaje)
	   EndIf
   case Else
	  ;aca van los get que ahi debemos er el mp_http_status

   EndSwitch
Return 0
EndFunc

Func CargarDatos( $istoken)
    if( $istoken) Then
	  GUICtrlSetData($lstbody, "")
	  Local $Json1 = FileRead(@ScriptDir & "\AccessToken.html")
   Else
	  Local $Json1 = FileRead(@ScriptDir & "\Received.html")
   EndIf
	Local $Data1 = Json_Decode($Json1)
	Local $Json2 = Json_Encode($Data1)

	Local $Data2 = Json_Decode($Json2)
	Local $Json3 = Json_Encode($Data2)
	ConsoleWrite2("Test1 Result: " & $Json3 & @LF)

	For $i = 0 to UBound($aKeys) -1
;~         ConsoleWrite('Busco = ' & $aKeys[$i] & @LF)
		$valorkey = Json_ObjExists($Data2, $aKeys[$i])
		if( $valorkey ) Then
		   $valorkey = Json_ObjGet($Data2,  $aKeys[$i])
		   SetearTxt($valorkey,$aControles[$i])
 		    ConsoleWrite2('si!!! ENCONTRE ' & $aKeys[$i] & " SU VALOR ES " & $valorkey &  @LF)
		Else
;~  		   ConsoleWrite2('NO ENCONTRE ' & $aKeys[$i] & @LF)
		 EndIf

	  Next


;~ 	$valorkey = Json_ObjExists($Data2, $key)
;~ 	if( $valorkey ) Then
;~  	   $valorkey = Json_ObjGet($Data2, $key)
;~ 	   GUICtrlSetData( $lstjson, $Json3)
;~ 	   if( $istoken) Then
;~ 		  GUICtrlSetData( $lstbody,  $valorkey )
;~ 		 EndIf

;~ 	Else
;~ 	   GUICtrlSetData( $lstjson, $Json3)
;~ 	   MsgBox(262144+64, 'All Tests Passed.','NONENS ENCOTNRO!!')
;~ 	 EndIf
;~ 	Return ($Json2 = $Json3)
 EndFunc   ;==>Test1

 Func LeerJsonCompuesto($tipo_mensaje)
  ConsoleWrite2("Leer Json  Received.html" & @CRLF)
 Local $huborespuesta = 0
 $huboerror = 0
 $file = FileOpen("Received.html", $FO_READ)
 Local $Json1 = FileRead($file)
 FileClose($file)
 Local $Data1 = Json_Decode($Json1)
 Local $Json2 = Json_Encode($Data1)

 Local $Data2 = Json_Decode($Json2)
 Local $Json3 = Json_Encode($Data2)
 ConsoleWrite2("2Test1 Result: " & $Json3 & @LF)

   $jsonString = $Json3
   $Obj = Json_Decode($jsonString)

   $code = Json_ObjExists($Data2, "code")

   SetearTxt($code,$txtcodigorespuesta)
   if($tipo_mensaje <> 4) Then
	  	  Local $mp_status = functionOfMyDreams($Obj, "respuesta", "mp_http_status")
		  Local $errorcito = functionOfMyDreams($Obj, "respuesta", "error")
		  if StringCompare ( $errorcito, "true",0 ) = 0 Then
			 ConsoleWrite2("hay error reportado por servidor" & @LF)
			 $huboerror = 1
		  EndIf
		  Local $newcode = Json_ObjGet($Data2, "code")
		;  ConsoleWrite2("Nuevo code " & $newcode & @TAB &  $tipo_mensaje & @LF)
		  Local $newcode = Json_ObjGet($Data2, "code")
		  if StringCompare ( $newcode, "404",0 ) = 0 Then
			 ConsoleWrite2("hay error reportado por servidor mal formato" & @LF)
			 $huboerror = 1
		  EndIf


	   Else
			$mp_status = "200"
	EndIf
    ConsoleWrite2("mp_http_status " & $mp_status & @TAB &  $tipo_mensaje & @LF)

  if( $mp_status = 404 and $tipo_mensaje = 2) Then ; no hay ordenes
	 $blinkear =1
	  AdlibRegister("SetTime", 1000)
	  AdlibRegister("ControlarTime", 30000)
	 GUICtrlSetData($lblmensajes,"NO HAY ORDENES PENDIENTES PARA ESTE POS")
;~ 	 SetearTxt("",$txtnroorden)
	 SetearTxt(0,$txtimporteorden)
	 Return
  EndIf
  if( $mp_status = 400 and $tipo_mensaje = 3) Then ; no hay ordenes para borrar
	 $blinkear =1
	  AdlibRegister("SetTime", 1000)
	  AdlibRegister("ControlarTime", 30000)
	 GUICtrlSetData($lblmensajes,"NO HAY ORDENES DISPONIBLES PARA BORRAR")
	 SetearTxt("",$txtnroorden)
	 SetearTxt(0,$txtimporteorden)
	 Return
  EndIf
    if( $mp_status = 204 and $tipo_mensaje = 3) Then ; borrar orden
  	 $blinkear =1
	  AdlibRegister("SetTime", 1000)
	  AdlibRegister("ControlarTime", 30000)
	 GUICtrlSetData($lblmensajes,"SE HA BORRADO LA ORDEN PENDIENTE")
	 SetearTxt("",$txtnroorden)
	 SetearTxt(0,$txtimporteorden)
	 Return
  EndIf
   if( ($mp_status = 400 or $huboerror = 1) and $tipo_mensaje = 1 ) Then ; error en la formacion del mensaje
	 $blinkear =1
	  AdlibRegister("SetTime", 1000)
	  AdlibRegister("ControlarTime", 30000)
	  ConsoleWrite2("ERROR INTERNO MENSAJERIA" & @LF)
	 GUICtrlSetData($lblmensajes,"ERROR INTERNO EN LA MENSAJERIA")
	 SetearTxt("",$txtnroorden)
	 SetearTxt(0,$txtimporteorden)
	 $Aprobada = 2
     $errorformato = 1
	 $huboerror = 1
	 Return 1
  EndIf

;~ veamo si es un instore order o merchar order  importe_pagado es respuesta de la orden creada
; importe es respuesta de instore_order y importe_transaccion es el get del merchan order
;~   Primera prueba
$str = $Json3
$str = StringReplace($str, '"', "")
if ($tipo_mensaje = 4) Then ;veamos si hay o Not
   ConsoleWrite2("busco importe_transaccion MERCHAR ORDEN" & @LF)
		 Local $importe_pagado = Json_Get($Obj, '[respuesta][ordenes][0][pagos][0][importe_transaccion]')
		 Local $posicionjson = 0;
		 While $importe_pagado > 0
			if ($importe_pagado > 0) Then

				  SetearTxt($mp_status,$txtcodigorespuesta)
				  SetearTxt("pagada",$txtstatus)

				  if( Number( GUICtrlRead( $txtimporteorden) ) < $importe_pagado) Then
					 ConsoleWrite2('Devolucion parcial quiero pagar ' &  Number( GUICtrlRead( $txtimporteorden) ) & " de total de   " &  $importe_pagado & @LF)
				  Else
					 SetearTxt($importe_pagado,$txtimporteorden)
				  EndIf

				  Local $id_pago = Json_Get($Obj, '[respuesta][ordenes][0][pagos]['& $posicionjson & '][id]')
				  SetearTxt( $id_pago,$txtidpago)
				  ConsoleWrite2("El id_pago es " & $id_pago & @LF)
;~ 				  veamos si esta aprobada
				  ConsoleWrite2("Envio ese id para ver si esta aprobado " & $id_pago & @LF)
				  Local $accion =  EnviarRequest(6)
				  ConsoleWrite("accion 6 consulta pago : " & $accion & @LF)
				  if( $accion =0 ) Then
					   Return $accion
					EndIf
;~ si no estuvo aprobado pruebo con el siguiente
				  $posicionjson= $posicionjson + 1
				  $importe_pagado = Json_Get($Obj, '[respuesta][ordenes][0][pagos]['& $posicionjson & '][importe_transaccion]')

			else ; aca no hay pagado seguir

				   if( $EsDevolucion = 1) Then
					  ConsoleWrite2("No hay pago merchar  No devolucion" & @LF)
					  Return 1
				   EndIf
				  Return 2
			 EndIf
		 WEnd
	ConsoleWrite2("No hay pago merchar order sigo esperando" & @LF)
	Return 2
EndIf


if ($tipo_mensaje = 3) Then ;veamos si hay o Not
   Local $importe = functionOfMyDreams($Obj, "respuesta", "importe")
   if( $importe > 0 ) Then
;~ 	  busquedmos los datos de instore order
      ConsoleWrite2('Hay importe ' & $importe &  @LF)
	  $res = StringRegExp($str, '(?:code|referencia_orden|importe|mp_http_status):([^\{,}]+)', 3)
	   if( @error = 0 ) Then
		 SetearTxt($res[0],$txtcodigorespuesta)
		 SetearTxt($res[1],$txtnroorden)
		 SetearTxt($res[2],$txtimporteorden)
		 ActivarMensaje()
		 SetearTxt("EXISTE UNA ORDEN DE PAGO PENDIENTE EN ESTE POS", $lblmensajes)
		 SetearTxt($jsonString,$editmensajes )
		 $huborespuesta =1
	  Else
		  ConsoleWrite2('Salio solito ' &  @LF)
		  Return
	  EndIf
   EndIf

EndIf

if ($tipo_mensaje = 5) Then ;veamos si hay o Not

   Local $status = functionOfMyDreams($Obj, "respuesta", "status")
   ConsoleWrite2('Status devo ' & $status &  @LF)
   if( $status = "approved" ) Then
;~ 	  busquemos los datos de instore order
		 SetearTxt("approved",$txtcodigorespuesta)
		 ActivarMensaje()
		 SetearTxt("YA FUE REALIZADA LA DEVOLUCION ", $lblmensajes)
		 $huborespuesta =1
		  GuardarOrden()
		  $Aprobada = 3
		 Return 0
	  Else  ; no se pudo devolver
		  ActivarMensaje()
		 SetearTxt("ERROR EN LA DEVOLUCION DE LA ORDEN", $lblmensajes)
		 SetearTxt($jsonString,$editmensajes )
		 SetearTxt("ERROR ",$txtstatus)
;~ 		 SetearTxt($importe_pagado,$txtimporteorden)
		 Return 1
   EndIf
EndIf


if ($tipo_mensaje = 6) Then ;veamos si hay o Not

   Local $status = functionOfMyDreams($Obj, "respuesta", "status")
   ConsoleWrite2('Status id_pago ' & $status &  @LF)
   if( $status = "approved" ) Then
   ;aca lo aprobo grabemos el archivo
		Local $hFileOpen = FileOpen(@ScriptDir & "\trans_id.txt", $FO_OVERWRITE)
		FileWrite($hFileOpen, 111111)
		FileFlush($hFileOpen)
		FileClose($hFileOpen)
		Return 0
   Else  ; no se pudo devolver
		 Return 1
   EndIf
EndIf

if( $huborespuesta =1) Then

   SetearTxt($res[0],$txtcodigorespuesta)
   SetearTxt($res[1],$txtnroorden)
   SetearTxt($res[2],$txtstatus)
   SetearTxt($res[3],$txtimporteorden)
   if($importe_pagado > 0 and $tipo_mensaje = 4)  Then
	   Return 0
    EndIf

Else
   if( $huborespuesta =2) Then
	  SetearTxt($res[0],$txtcodigorespuesta)
	  SetearTxt($res[1],$txtnroorden)
 	  SetearTxt($res[2],$txtstatus)
;~ 	  SetearTxt("",$txtimporteorden)
	  if( $tipo_mensaje = 4) Then
		 Return 2
	  EndIf
	EndIf

EndIf

;~   Seguna prueba ver si falla la primera en leer multiples objetos
;~ $jsonString = $Json3
;~ $Obj = Json_Decode($jsonString)

;~ $age = functionOfMyDreams($Obj, "respuesta", "referencia_orden")

;~ ConsoleWrite("referencia : " & $age & @CRLF)

EndFunc

Func functionOfMyDreams($decodedObject, $targetObject, $targetKeyInObject)
    $value = Json_Get($decodedObject, '["' & $targetObject & '"]["' & $targetKeyInObject & '"]')
    return $value
EndFunc   ;==>functionOfMyDreams


Func post_authentication_test()
    Local $oHttp   = Null, _
          $oComErr = Null

    Local $iHttpStatus = 0

    Local $sResponse = "", _
          $sPostData = ""


    ConsoleWrite2(@CRLF & "Executing API pedido de token" & @CRLF)

    ;Set COM error handler
    $oComErr = ObjEvent("AutoIT.Error", "com_error_handler")

    ;Create a HTTP COM object
    $oHttp = ObjCreate("winhttp.winhttprequest.5.1")
    If @error Then
        ConsoleWrite2("Unable to create http request object." & @CRLF)
        FocusTPS()
		Exit -1
    EndIf
    ConsoleWrite2("WinHttpRequest object created." & @CRLF)

    $oHttp.Option($WinHttpRequestOption_SslErrorIgnoreFlags) = $WinHttpRequestOption_SslErrorIgnoreFlags_IgnoreAll

    With $oHttp
        ;Open POST request

;~ 		  60 segundos de timeout
   		.SetTimeouts("0", "30000", "15000", "30000")
  		.Open("POST", "https://" & $IpServidor &":" & $PortServidor &"/authentication/", False)
;~   		.Open("POST", "https://run.mocky.io/v3/d8c8de02-fd37-4ee0-9a64-2e20d8bc2c1f---caulq", False)

        .SetRequestHeader("Content-Type", "application/json")
        .Send('{"strategy": "local","email": "admin","password": "TheQueeN.20"}')
 		.WaitForResponse()
;~ en teoria si el open tiene false no hace falta esto

        If @error Then
             ConsoleWrite2(StringFormat("SEND ERROR: (0x%X) %s", $oComErr.Number, $oComErr.Description) & @CRLF)
            Return -1
        EndIf

        ;Get status code and response
        $iHttpStatus = .Status
        $sResponse   = .ResponseText

        ;If status code isn't okay
        If $iHttpStatus <> 200 And $iHttpStatus <> 201 Then
            ConsoleWrite2("3-HTTP Status  : " & String($iHttpStatus) & @CRLF)
            ConsoleWrite2("3-HTTP Response: " & @CRLF & $sResponse & @CRLF)
			If $iHttpStatus = 302 Then
			   ConsoleWrite2(( "Server Attempted Redirect to: ", .getResponseHeader("Location"))
			EndIf
			SetearTxt(String($iHttpStatus), $txtcodigorespuesta )
            Return -1
		 Else
			ConsoleWrite2("4-HTTP Status  : " & String($iHttpStatus) & @CRLF)
            ConsoleWrite2("4-HTTP Response: " & @CRLF & $sResponse & @CRLF)
        EndIf
    EndWith

    ConsoleWrite2("API Response:" & @CRLF & $sResponse & @CRLF)
    $file = FileOpen(@ScriptDir & "\AccessToken.html", 2) ; The value of 2 overwrites the file if it already exists
     FileWrite($file, $sResponse)
	 FileFlush($file)
     FileClose($file)
    CargarDatos( True)
EndFunc

Func com_error_handler($oError)
    ConsoleWrite2("Error Desc: " & $oError.Description & @CRLF)
    ConsoleWrite2(@ScriptName & " (" & $oError.scriptline & ") : ==> COM Error intercepted !" & @CRLF & _
            @TAB & "err.number is: " & @TAB & @TAB & "0x" & Hex($oError.number) &  @TAB & Dec(Hex($oError.number)) & @CRLF & _
            @TAB & "err.windescription:" & @TAB & $oError.windescription & @CRLF & _
            @TAB & "err.description is: " & @TAB & $oError.description & @CRLF & _
            @TAB & "err.source is: " & @TAB & @TAB & $oError.source & @CRLF & _
            @TAB & "err.scriptline is: " & @TAB & $oError.scriptline & @CRLF & _
            @TAB & "err.retcode is: " & @TAB & "0x" & Hex($oError.retcode) & @CRLF & @CRLF)
			SetError(2)
			SetearTxt($oError.description,$editmensajes)
			SetearTxt($oError.number,$txtcodigorespuesta)

;~ 	     Return
 EndFunc

Func SetTime()

    Local $time = _TicksToTime(Int(TimerDiff($begin)), $hour, $min, $sec)
;~ 	ConsoleWrite("Timessss blinquear " & $blinkear & " sec vs sec " & @Sec & " " & $sec &@CRLF )
	If @SEC <> $sec and $blinkear = 1 Then
	  $sec = @SEC
		 If $RED Then
			 GUICtrlSetColor($lblmensajes,0xA6CAF0)
		 Else
  		   GUICtrlSetColor($lblmensajes,0xff0000)
		 EndIf
  	   $RED = Not $RED
	EndIf
;~     ControlSetText($Form1 , "", $tiempoinsumido, StringFormat("%02i:%02i:%02i", $hour, $min, $sec))
 EndFunc

 Func SetearTxt($txtValor, $controlID)
GUICtrlSetData( $controlID,  $txtValor )
EndFunc

Func DesactivarMensaje()
   	 AdlibUnRegister("SetTime")
		 AdlibUnRegister("ControlarTime")
		 $blinkear = 0
		SetearTxt("",$lblmensajes)
	 EndFunc

Func ActivarMensaje()
	   AdlibRegister("SetTime", 1000)
	   AdlibRegister("ControlarTime", 30000)
		 $blinkear = 1

EndFunc

Func _Terminate()
    FocusTPS()
	Exit
 EndFunc   ;==>_Terminate

 Func IsPar($numner)
    If ($numner/2)=Round($numner/2) Then
        Return True
    Else
        Return False
    EndIf
 EndFunc

 Func _Arriba()
;~     MsgBox($MB_SYSTEMMODAL, "", "This is a message.")
;ControlFocus($Form1 ,$Titulo,$btngenerarorden)
;GUICtrlSetState( $atras3, $GUI_HIDE)
;GUICtrlSetState( $atras2, $GUI_HIDE)
;GUICtrlSetState( $atras1, $GUI_SHOW)
EndFunc   ;==>ShowMessage

Func _Abajo()
;~     MsgBox($MB_SYSTEMMODAL, "", "This is a message.")
;ControlFocus($Form1 ,$Titulo,$btnsalir)

;GUICtrlSetState( $atras2, $GUI_SHOW)
;GUICtrlSetState( $atras1, $GUI_HIDE)
;GUICtrlSetState( $atras3, $GUI_HIDE)

EndFunc   ;==>ShowMessage

Func _PressButton()
;~     MsgBox($MB_SYSTEMMODAL, "", "This is a message.")
;Send("{SPACE}")
EndFunc   ;==>ShowMessage

Func _PressButton1()
;ControlFocus($Form1 ,$Titulo,$btngenerarorden)
;GUICtrlSetState( $atras3, $GUI_HIDE)
;GUICtrlSetState( $atras2, $GUI_HIDE)
;GUICtrlSetState( $atras1, $GUI_SHOW)
EndFunc   ;==>ShowMessage

Func _PressButton2()
;ControlFocus($Form1 ,$Titulo,$btnsalir)
;GUICtrlSetState( $atras2, $GUI_SHOW)
;GUICtrlSetState( $atras1, $GUI_HIDE)
;GUICtrlSetState( $atras3, $GUI_HIDE)
EndFunc   ;==>ShowMessage

Func _PressButton3()
ControlFocus($Form1 ,$Titulo,$btnaborrarorden)
;GUICtrlSetState( $atras2, $GUI_HIDE)
;GUICtrlSetState( $atras3, $GUI_SHOW)
;GUICtrlSetState( $atras1, $GUI_HIDE)
EndFunc   ;==>ShowMessage

Func _PressButtonLeft()
ControlFocus($Form1 ,$Titulo,$monto)

EndFunc   ;==>ShowMessage

Func _PressButtonRight()
ControlFocus($Form1 ,$Titulo,$txtcambiarmonto)

EndFunc   ;==>ShowMessage

Func LevantadoQR()

Local $HostI =$IpServidor
Local $PortI =$PortServidor

Local $iPing = Ping($HostI, 3000)
If $iPing Then ; If a value greater than 0 was returned then display the following message.
   $Respuesta=1
Else
    ConsoleWrite2("Error " & @error &@LF)
   $Respuesta=0
EndIf
Return   $Respuesta

EndFunc   ;

Func LevantadoQR2()
 TCPStartup()

;~ $HostI ="10.9.0.1"
;~ $PortI ="8091"

Local $HostI =$IpServidor
Local $PortI =$PortServidor
;~ $PortI ="8080"
;~ ControlSend ( "Telnet Client","", ".", "{TAB}")
$Host = TCPNameToIP ( $HostI )
;~ $Conection = TCPConnect ( $Host, $PortI )
Log2("Conectando a " & $Host & " " & $PortI )
$Conection = _TCPConnect ( $HostI, $PortI,1000 )

If $Conection = -1 Then
   $Respuesta=0
Else
   $Respuesta=1
EndIf

TCPShutdown()
Return $Respuesta
EndFunc   ;


; #FUNCTION# ====================================================================================================================
; Name...........: _TCPConnect
; Description ...: Triess to establishes a TCP-connection in a specified time limit
; Syntax.........: _TCPConnect($sIPAddr, $iPort, $iTimeOut = -1)
; Parameters ....: $sIpAddr - IP address to connect to (IPv4)
; $iPort - Port to use
; $iTimeOut - Timeout for connection in milliseconds (default: -1)
; |Values < 0: default timeout
; |Values 0, Keyword Default: use time from Opt("TCPTimeout")
; |Values > 0: timeout in milliseconds
; Return values .: Success - Socket to use with TCP-functions
; Failure - -1, sets @error
; |1 - $sIpAddr incorrect
; |2 - could not get port
; |3 - could not create socket
; |4 - could not connect
; |5 - could not get WSAError
; |and errors from WSAGetLastError
; Author ........: ProgAndy
; Modified.......: JScript
; Remarks .......:
; Related .......: TCPConnect, TCPCloseSocket, TCPSend, TCPRecv
; Link ..........:
; Example .......:
; ===============================================================================================================================
Func _TCPConnect($sIPAddr, $iPort, $iTimeOut = -1)
    Local $hWs2 = DllOpen("Ws2_32.dll")
    Local $iDllErr, $fError = False, $aRes
    Local $hSock = DllCall($hWs2, "uint", "socket", "int", 2, "int", 1, "int", 6)
    If @error Then
        $iDllErr = 3
    ElseIf $hSock[0] = 4294967295 Or $hSock[0] = -1 Then
        $fError = True
    Else
        $hSock = $hSock[0]
        $aRes = DllCall($hWs2, "ulong", "inet_addr", "str", $sIPAddr)
        If @error Or $aRes[0] = -1 Or $aRes[0] = 4294967295 Then
            $iDllErr = 1
        Else
            $iPort = DllCall($hWs2, "ushort", "htons", "ushort", $iPort)
            If @error Then
                $iDllErr = 2
            Else
                $iPort = $iPort[0]
            EndIf
        EndIf
        If 0 = $iDllErr Then
            Local $tSockAddr = DllStructCreate("short sin_family;ushort sin_port; ulong sin_addr;char sin_zero[8];")
            DllStructSetData($tSockAddr, 1, 2)
            DllStructSetData($tSockAddr, 2, $iPort)
            DllStructSetData($tSockAddr, 3, $aRes[0])

            If IsKeyword($iTimeOut) Or $iTimeOut = 0 Then $iTimeOut = Opt("TCPTimeout")

            If $iTimeOut > -1 Then DllCall($hWs2, "int", "ioctlsocket", "int", $hSock, "long", 0x8004667e, "uint*", 1)
            $aRes = DllCall($hWs2, "int", "connect", "int", $hSock, "ptr", DllStructGetPtr($tSockAddr), "int", DllStructGetSize($tSockAddr))

            Select
                Case @error
                    $iDllErr = 4
                Case $aRes[0] <> 0
                    $aRes = DllCall($hWs2, "int", "WSAGetLastError")
                    If Not @error And $aRes[0] = 10035 Then ContinueCase
                    $fError = True
                Case $iTimeOut > -1
                    If IsKeyword($iTimeOut) Or $iTimeOut = 0 Then $iTimeOut = Opt("TCPTimeout")
                    Local $t = DllStructCreate("uint;int")
                    DllStructSetData($t, 1, 1)
                    DllStructSetData($t, 2, $hSock)
                    Local $to = DllStructCreate("long;long")
                    DllStructSetData($to, 1, Floor($iTimeOut / 1000))
                    DllStructSetData($to, 2, Mod($iTimeOut, 1000))
                    $aRes = DllCall($hWs2, "int", "select", "int", $hSock, "ptr", DllStructGetPtr($t), "ptr", DllStructGetPtr($t), "ptr", 0, "ptr", DllStructGetPtr($to))
                    If Not @error And $aRes[0] = 0 Then
                        $aRes = DllCall($hWs2, "int", "WSAGetLastError")
                        If Not @error And $aRes[0] = 0 Then
                            $iDllErr = 10060
                        Else
                            $fError = True
                        EndIf
                    Else
                        DllCall($hWs2, "int", "ioctlsocket", "int", $hSock, "long", 0x8004667e, "uint*", 0)
                    EndIf
            EndSelect
        EndIf
    EndIf
    If $iDllErr Then
        TCPCloseSocket($hSock)
        $hSock = -1
    ElseIf $fError Then
        $iDllErr = DllCall($hWs2, "int", "WSAGetLastError")
        If Not @error Then $iDllErr = $iDllErr[0]
        If $iDllErr = 0 Then $iDllErr = 5
        TCPCloseSocket($hSock)
        $hSock = -1
    EndIf
    DllClose($hWs2)
    Return SetError($iDllErr, 0, $hSock)
 EndFunc   ;==>_TCPConnect

 Func ConsoleWrite2 ($mensaje)
	if( $esproduccion = 0 ) Then
	   ConsoleWrite($mensaje)
	Else
	  Log2($mensaje)
    EndIf
 EndFunc

 Func Log2($mensaje)
;~ 	una operacion simple son 10 Kb de log. cuantas operaciones por dia? 2 como mucho, por  25 dias total = 10 * 2 * 25 = 500kn es decir medio mega
   Local $sFileNamesistemalog =@ScriptDir & "/mercado.log"
   Local $iFileExists = FileExists($sFileNamesistemalog)
   If $iFileExists = 0 Then
	    Local $hFileOpen = FileOpen($sFileNamesistemalog, $FO_OVERWRITE)
	    FileWrite($hFileOpen, "Comienzo logs"& @LF)
	   FileClose($hFileOpen)
   EndIf
   Local $armado = _DateTimeFormat(_NowCalc(), 2) & " " & _DateTimeFormat(_NowCalc(), 5)  & " " & $mensaje
$iFileExists = FileExists($sFileNamesistemalog)
If $iFileExists = 1 Then
   Local $hFileOpen = FileOpen($sFileNamesistemalog, $FO_APPEND)
   FileWriteLine($hFileOpen, $armado )
   FileClose($hFileOpen)
   ;MsgBox($MB_SYSTEMMODAL, "ERROR", "encontrado")
   ;Exit(3)
   TrimLogFile($sFileNamesistemalog)
EndIf
EndFunc


Func Devolucion( $OrdenBuscar)
;~ primero que todo con el nro de orden debemos saber el id_pago llamando al mercharorden

 Local $accion =  EnviarRequest(4)
;~ 0 existe 1 no existe
ConsoleWrite2("La Orden a buscar es : " & $OrdenBuscar & "Accion devuelta " & $accion  & @LF)
;crearmos el archivo de devolucion.json para enviar
if( $accion = 1) Then
   ActivarMensaje()
   SetearTxt("NO EXISTE UN PAGO PARA HACER DEVOLUCION", $lblmensajes)
   Return
EndIf

Local $sFileNamesistemalog = @ScriptDir & "\devolucion.json"
;~ Local $iFileExists = FileExists($sFileNamesistemalog)
Local $cadena = '{"id_pago": 10274347133,"importe": 1.01}'
Local $hFileOpen = FileOpen($sFileNamesistemalog, $FO_OVERWRITE)

If Not FileWrite($hFileOpen, $cadena ) Then
   ConsoleWrite( "An error occurred whilst writing the temporary file.")
EndIf

$close = FileClose($hFileOpen)

Local $hFileOpen = FileOpen($sFileNamesistemalog, $FO_READ)
Local $Json1 = FileRead($hFileOpen)
FileClose($hFileOpen)

Local $Data1 = Json_Decode($Json1)
Local $Json2 = Json_Encode($Data1)

Local $Data2 = Json_Decode($Json2)
Local $Json3 = Json_Encode($Data2)

Local $Obj = Json_ObjCreate()
Local $valorkey = Json_ObjExists($Data2, "id_pago")

if( $valorkey ) Then
   ConsoleWrite( "Grabando el json.")
   $valorkey = Json_ObjGet($Data2,  "id_pago")
   Json_ObjPut($Data2, "id_pago", Number(GUICtrlRead($txtidpago )))

   $valorkey = Json_ObjGet($Data2,  "importe")
   Json_ObjPut($Data2, "importe", Number(GUICtrlRead($txtimporteorden )))

	Local $hFileOpen2 = FileOpen(@ScriptDir & "\devolucion.json", $FO_OVERWRITE)
	$Json3 = Json_Encode($Data2)
	ConsoleWrite2("AAsi mando al json : " & $Json3 & @LF)
 	FileWrite($hFileOpen2, $Json3)
 EndIf
FileClose($hFileOpen)
Local $accion =  EnviarRequest(5)
ConsoleWrite("accion 5 devolucion : " & $accion & @LF)

EndFunc

Func ValidarNumero($numero2)
Local $numero = Number($numero2);
ConsoleWrite("Numero : " & $numero & @LF)
   If IsNumber($numero) Then
	  ConsoleWrite("Si es numero vs: " &  Number( GUICtrlRead( $txtimporteorden) ) & @LF)
;~ 	  validar que no sea superio a la orden
	   if( $numero >=  Number( GUICtrlRead( $txtimporteorden) )) Then
		  Return False
	   Else
		  DesactivarMensaje()
		  Return True
	   EndIf
   else
	  Return False
   EndIf
EndFunc


Func LeerIni()
;~    leamos la configuracion
; Read the INI file for the value of 'Title' in the section labelled 'General'.
; contemos las seccciones
$sarray =IniReadSectionNames ( $sQrIni )
Local $cantidadserver= $sarray[0]


Local $ipserver
For $iI = 0 To $sarray[0]
   If StringInStr("10.9.0.1",BuscarServer($sarray[$iI]), $STR_NOCASESENSEBASIC) = 0 Then
	  $ipserver = BuscarServer($sarray[$iI])
	  $IpServidor = $ipserver
	EndIf
 Next

;~ MsgBox(48 + $MB_SYSTEMMODAL, "ERROR GRAVE", "server " & $ipserver)

Local $puerto
For $iI = 0 To $sarray[0]
   If StringInStr("8091",BuscarPuerto($sarray[$iI]), $STR_NOCASESENSEBASIC) = 0 Then
	  $puerto = BuscarPuerto($sarray[$iI])
	  $PortServidor = $puerto
	EndIf
 Next

;~ MsgBox(48 + $MB_SYSTEMMODAL, "ERROR GRAVE", "puerto " & $puerto)

Local $prefijocaja
; ConsoleWrite(" caja " & $sarray[2] &@LF )
For $iI = 1 To $sarray[0]
   If StringInStr("",BuscarCaja($sarray[$iI]), $STR_NOCASESENSEBASIC) = 0 Then
	  $prefijocaja = BuscarCaja($sarray[$iI])
	EndIf
 Next
;~ MsgBox(48 + $MB_SYSTEMMODAL, "ERROR GRAVE", "prefijo1 " & $prefijocaja)

Local $prefijosucursal
For $iI = 1 To $sarray[0]
   If StringInStr("",BuscarSucursal($sarray[$iI]), $STR_NOCASESENSEBASIC) = 0 Then
	  $prefijosucursal = BuscarSucursal($sarray[$iI])
	EndIf
 Next
;~ MsgBox(48 + $MB_SYSTEMMODAL, "ERROR GRAVE", "prefijo2  " & $prefijosucursal)


Return 1


EndFunc

Func BuscarPuerto( $Seccion)
;~     ConsoleWrite("port " & $Seccion &@LF )
      $sRead = IniRead($sQrIni, $Seccion, "port", "8091")
	  Return $sRead
   EndFunc

 Func BuscarServer( $Seccion)
      $sRead = IniRead($sQrIni, $Seccion, "server", "10.9.0.1")
	  Return $sRead
   EndFunc

Func BuscarCaja( $Seccion)
;~    ConsoleWrite("seccion " & $Seccion &@LF )
      $sRead = IniRead($sQrIni, $Seccion, "caja", "")
	  Return $sRead
   EndFunc

Func BuscarSucursal( $Seccion)
      $sRead = IniRead($sQrIni, $Seccion, "sucursal", "")
	  Return $sRead
   EndFunc

Func TrimLogFile($sFilename, $iMaxSize = 2048000)
;~     filegetsize en bytes , 1000 kn serian 2000 *1024 = 1024000
	Local $iSize = FileGetSize($sFilename)
	ConsoleWrite("tama log  " & $iSize &@LF )
    Local $hFile = -1
    Local $sData = ''

    If $iSize > $iMaxSize Then
        $hFile = FileOpen($sFilename,2)
	    FileWrite($hFile , "Comienzo nuevo log" & @LF )
        FileClose($hFile)
    EndIf
;~     Return FileGetSize($sFilename)

EndFunc   ;==>TrimLogFile

Func AccesTokendeHoy()
$file = @ScriptDir & "\AccessToken.html"
Local $diatoken = FileGetTime($file, $FT_MODIFIED,  $FT_ARRAY)
Local $diadeltoken = $diatoken[1]  &$diatoken[2]
Local $diahoy = @MON &@MDAY
ConsoleWrite("fecha del token  " & $diadeltoken  & "vs " &$diahoy & @LF )
if( $diadeltoken <> $diahoy) Then
;~    hay que pedir token distinta fecha
   ConsoleWrite2("mandar a pedir token por fecha " &@LF )
   Return post_authentication_test()
EndIf
EndFunc

Func Salida()
   ConsoleWrite2("Boton salir Salgo con codigo " & $Aprobada & @LF)
   FocusTPS()
   Exit $Aprobada
EndFunc

Func FocusTPS()
   Local $aList = WinList()
	Local $Titulo = "TPS - V"
	Local $Titulo2 = "TPV 1000 - IV"

	Local $activado = 0

	 if WinExists ( $Titulo ) Then
		$activado = 1
	  EndIf

	  if WinExists ( $Titulo2  ) Then
		 $activado = 1
	   EndIf


    If $activado = 1 and WinActivate( $Titulo2 , "") = 0  Then
		WinActivate( $Titulo , "")
	EndIf
EndFunc