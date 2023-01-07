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
#include <./lib/Misc.au3>
#include <./lib/ButtonConstants.au3>
#include <./lib/EditConstants.au3>
#include <./lib/GUIConstantsEx.au3>
#include <./lib/StaticConstants.au3>
#include "mysql.au3"
#include "mysql2.au3"



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
Global CONST $_ON_LINE_COMPRA = 0
Global CONST  $_ON_LINE_DEVOLUCION = 1
Global CONST $_ON_LINE_ANULACION = 2
Global CONST $_ON_LINE_CASHPLUS =22;
Global CONST  $_ON_LINE_ANULACION_CASHPLUS =23


Global  $IpServidor, $PortServidor,$NroCaja,$CajaZ, $IdEvento, $idTransaccion

Local $version ="1.15"
$Titulo = "Script Napse " & $version

If _Singleton(@ScriptName,1)=0 Then

	;veamos cuantas veces esta corriendo , el popup es POPUPPROMO
	Local $aList = WinList()
    For $i = 1 To $aList[0][0]
        If $aList[$i][0] <> "" And BitAND(WinGetState($aList[$i][1]), 2) and $aList[$i][0] = $Titulo Then
            ;MsgBox($MB_SYSTEMMODAL, "", "Title: " & $aList[$i][0] & @CRLF & "Handle: " & $aList[$i][1])
			$yaestacorriendo = $yaestacorriendo + 1
		 EndIf
		 If $aList[$i][0] <> "" And BitAND(WinGetState($aList[$i][1]), 2) and $aList[$i][0] = $Titulo2 Then
            ;MsgBox($MB_SYSTEMMODAL, "", "Title: " & $aList[$i][0] & @CRLF & "Handle: " & $aList[$i][1])
			$yaestacorriendo = $yaestacorriendo + 1
		 EndIf

    Next

endif

ConsoleWrite2("Iniciando " & $Titulo)

Local $oHttp   = Null, _
$oComErr = Null
Local $iHttpStatus = 0
Local $sResponse = "", _
$sPostData = ""

$IpServidor = "127.0.0.1"
$PortServidor = "8080" ;

Global $servidormysql="localhost"
Global $usuariomysql ="root"
Global $clavemysql ="root"
Global $basemysql ="DBTPVIV"
Global $portmysql =3306

ConsoleWrite2("Cantidad de argumentos " & $CmdLine[0] )

if($CmdLine[0] = 2 ) Then ; ojo aca no olvidarse de modo produccion.
   $NroCaja = $CmdLine[1]
   $idTransaccion = $CmdLine[2]
   ;$CajaZ = $CmdLine[2]
   ;$IdEvento = $CmdLine[3]

    ConsoleWrite2(" Nro Caja "&  $NroCaja & " id transaccion " & $idTransaccion& @LF)
EndIf

#Region ### START Koda GUI section ### Form=C:\work_svn\TPS-LAREINA\bin\bakau3\propios\lib\FormNapse.kxf
$Form1 = GUICreate("Simulacion Napse", 788, 698, 192, 124)
$Input1 = GUICtrlCreateInput("", 120, 40, 121, 21)
$Input2 = GUICtrlCreateInput("", 112, 72, 121, 21)
$Input3 = GUICtrlCreateInput("", 112, 112, 121, 21)
$Input4 = GUICtrlCreateInput("", 344, 40, 121, 21)
$Input5 = GUICtrlCreateInput("", 344, 72, 121, 21)
$Input6 = GUICtrlCreateInput("", 352, 120, 121, 21)
$Input7 = GUICtrlCreateInput("", 608, 40, 121, 21)
$Input8 = GUICtrlCreateInput("", 616, 80, 121, 21)
$Label1 = GUICtrlCreateLabel("FH_INCORPORACION", 0, 40, 114, 17)
$Label2 = GUICtrlCreateLabel("X_OPERACION", 0, 80, 80, 17)
$Label3 = GUICtrlCreateLabel("Importe", 8, 112, 39, 17)
$Label4 = GUICtrlCreateLabel("CAJA_z", 248, 40, 62, 17)
$Label5 = GUICtrlCreateLabel("N_CUOTAS", 248, 72, 62, 17)
$Label6 = GUICtrlCreateLabel("C_IDDOCUMENTO", 248, 120, 98, 17)
$Label7 = GUICtrlCreateLabel("X_NOMBRETABLA", 496, 40, 98, 17)
$Label8 = GUICtrlCreateLabel("C_PTOVTAORIGEN", 496, 80, 102, 17)
$Group1 = GUICtrlCreateGroup("Respuesta", -8, 152, 761, 281)
GUICtrlSetFont(-1, 14, 400, 0, "MS Sans Serif")
$Label9 = GUICtrlCreateLabel("N_NROAUTORIZACION", 8, 200, 151, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")
$txtnroaturoizadion = GUICtrlCreateInput("", 160, 200, 121, 32)
$nro_cupon = GUICtrlCreateInput("", 160, 248, 121, 32)
$nrocomercio = GUICtrlCreateInput("", 160, 304, 121, 32)
$nroterminal = GUICtrlCreateInput("", 160, 344, 121, 32)
$nrolote = GUICtrlCreateInput("", 560, 200, 121, 32)
$tarjeta = GUICtrlCreateInput("", 560, 248, 121, 32)
$tipotarjeta = GUICtrlCreateInput("", 560, 296, 121, 32)
$titular = GUICtrlCreateInput("", 552, 344, 121, 32)
$Label10 = GUICtrlCreateLabel("N_NROCUPON", 6, 256, 99, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")
$Label11 = GUICtrlCreateLabel("N_NROCOMERCIO", 5, 313, 122, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")
$Label12 = GUICtrlCreateLabel("N_NROTERMINAL", 6, 357, 119, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")
$Label13 = GUICtrlCreateLabel("N_NROLOTE", 356, 202, 86, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")
$Label14 = GUICtrlCreateLabel("X_TARJETA", 360, 257, 81, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")
$Label15 = GUICtrlCreateLabel("N_TIPOTARJETA", 364, 303, 114, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")
$Label16 = GUICtrlCreateLabel("X_NOMBRETITULAR", 363, 346, 135, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")
$Label17 = GUICtrlCreateLabel("N_NROCUENTA", 3, 402, 107, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")
$nrocuenta = GUICtrlCreateInput("", 160, 392, 121, 32)
$Label18 = GUICtrlCreateLabel("N_TIPOCTA", 357, 394, 79, 20)
GUICtrlSetFont(-1, 10, 400, 0, "MS Sans Serif")
$tipocta = GUICtrlCreateInput("", 552, 384, 121, 32)
GUICtrlCreateGroup("", -99, -99, 1, 1)
$Group2 = GUICtrlCreateGroup("Procesamiento", 24, 456, 761, 177)
GUICtrlSetFont(-1, 14, 400, 0, "MS Sans Serif")
GUICtrlSetColor(-1, 0x000000)
$Button1 = GUICtrlCreateButton("Procesar", 80, 560, 139, 25)
$Group3 = GUICtrlCreateGroup("Resultado", 600, 480, 137, 137)
$Radio1 = GUICtrlCreateRadio("Rechazada", 608, 520, 113, 17)
$Radio2 = GUICtrlCreateRadio("Aceptada", 608, 560, 113, 17)
GUICtrlSetState(-1, $GUI_CHECKED)
GUICtrlCreateGroup("", -99, -99, 1, 1)
$Button2 = GUICtrlCreateButton("Cancelar", 256, 560, 99, 25)
GUICtrlCreateGroup("", -99, -99, 1, 1)
GUISetState(@SW_SHOW)
#EndRegion ### END Koda GUI section ###
$Label19 = GUICtrlCreateLabel("IDEVENTO", 496, 120, 59, 17)
$Input9 = GUICtrlCreateInput("", 616, 120, 121, 21)
CargarOperacion($idTransaccion )
While 1
	$nMsg = GUIGetMsg()
	Switch $nMsg
		Case $GUI_EVENT_CLOSE
			Exit
		 Case $Button1
			ProcesarOperacion($idTransaccion)
	EndSwitch
WEnd
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Func ProcesarOperacion($idTransaccion)

;Set COM error handler
$oComErr = ObjEvent("AutoIT.Error", "com_error_handler")

 $oHttp = ObjCreate("winhttp.winhttprequest.5.1")
    If @error Then
        ConsoleWrite2("Unable to create http request object." & @CRLF)
        Exit -1
    EndIf
 ConsoleWrite2("WinHttpRequest object created." & @CRLF)
 $oHttp.Option($WinHttpRequestOption_SslErrorIgnoreFlags) = $WinHttpRequestOption_SslErrorIgnoreFlags_IgnoreAll

 Local $url = "http://localhost:8080/card/transaccion/" &$idTransaccion
 ConsoleWrite2("URL." & $url & @CRLF)

  With $oHttp
	;.Open("GET", "https://" & $IpServidor &":" & $PortServidor &"/pay_orders/"& $NroCaja & "?criterio_de_consulta=por_nro_caja&cod_sucursal=" & $CodSucursal & "' ", False)
	.Open("POST", "http://localhost:8080/card/transaccion/" & $idTransaccion, False)

	.SetTimeouts("600000", "600000", "600000", "600000")
;	.SetRequestHeader("Authorization", "Bearer " & $Bearer)
;	.SetRequestHeader("Content-Type", "application/json")
	.Send()

 $iHttpStatus = .Status
 $sResponse   = .ResponseText
 If $iHttpStatus = 200 or  $iHttpStatus = 201 then
	ConsoleWrite2("1-HTTP Status  : " & String($iHttpStatus) & @CRLF)
    ConsoleWrite2("HTTP Response: " & @CRLF & $sResponse & @CRLF)
	 ConsoleWrite2("Salgo con codigo 3 OK " & @CRLF )
    Exit (3)

Else
   ConsoleWrite2("2-HTTP Status  : " & String($iHttpStatus) & @CRLF)
   ConsoleWrite2("2- HTTP Response: " & @CRLF & $sResponse & @CRLF)
   ConsoleWrite2("Salgo con codigo 2 ERROR " & @CRLF )
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;simulo abajo
   CargarRespuesta(1)
   Exit (2)
EndIf
EndWith
EndFunc
 Func ConsoleWrite2 ($mensaje)

	  Log2($mensaje)
 EndFunc

 Func Log2($mensaje)
;~ 	una operacion simple son 10 Kb de log. cuantas operaciones por dia? 2 como mucho, por  25 dias total = 10 * 2 * 25 = 500kn es decir medio mega
   Local $sFileNamesistemalog =@ScriptDir & "/napseCaja.log"
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


Func CargarOperacion($idTransaccion)
$sql = _MySQLConnect($usuariomysql,$clavemysql,$basemysql ,$servidormysql)

		if  $sql = 0 Then
			MsgBox(48, "Error!", "IMPOSIBLE CONECTARSE A LA BASE DE DATOS" )
			Return 0
		endif
 $consulta = _Query($sql,"SELECT * FROM transac2 t where id_transaccion =  " &$idTransaccion)

; Local $PuntosAcumulados = $consulta.Fields("TIPO_DE_MENSAJE").value

GUICtrlSetData($Input1,_NowDate ( ) )
Local $operacion = $consulta.Fields("TIPO_DE_MENSAJE").value
if ($operacion = $_ON_LINE_COMPRA OR $operacion = $_ON_LINE_CASHPLUS ) Then
           GUICtrlSetData($Input2,"SA")
else
           if ($operacion =$_ON_LINE_DEVOLUCION ) Then
			  GUICtrlSetData($Input2,"RF")
		   else ;//anulaciones
			  GUICtrlSetData($Input2,"VS")
           EndIf
EndIf
GUICtrlSetData($Input3,$consulta.Fields("TIPO_DE_MENSAJE").value)
GUICtrlSetData($Input4,$consulta.Fields("caja_z").value)
GUICtrlSetData($Input6,$consulta.Fields("nro_comp_transaccion").value  )
GUICtrlSetData($Input7,"DOC_FACUTA_NOTA" )
GUICtrlSetData($Input8,$NroCaja )
GUICtrlSetData($Input5,$consulta.Fields("CUOTAS").value )
GUICtrlSetData($Input9,$consulta.Fields("ID_EVENTO").value )
$CajaZ=$consulta.Fields("caja_z").value
$IdEvento= $consulta.Fields("ID_EVENTO").value
_MySQLEnd($sql)


EndFunc

Func CargarRespuesta($idTransaccion)
$sql = _MySQLConnect($usuariomysql,$clavemysql,$basemysql ,$servidormysql)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;TRUCHAMOS LA BUSQUEDA
Local $IdEvento  = 77
Local $CajaZ =100152
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		if  $sql = 0 Then
			MsgBox(48, "Error!", "IMPOSIBLE CONECTARSE A LA BASE DE DATOS" )
			Return 0
		endif
 $consulta = _Query($sql,"SELECT * FROM respuesta_naspse r where id_evento =  " &$IdEvento & " and caja_z = " & $CajaZ)


if ($consulta = 0) Then
           MsgBox(48, "Error!", "NO PUDE TRAER LA OPERACION" )

EndIf
GUICtrlSetData($nrocuenta,$consulta.Fields("X_NROCUENTA").value) 
GUICtrlSetData($tipocta ,$consulta.Fields("N_TIPOCTA").value)
GUICtrlSetData($txtnroaturoizadion,$consulta.Fields("N_NROAUTORIZACION").value) 
GUICtrlSetData($nro_cupon ,$consulta.Fields("N_NROCUPON").value)
GUICtrlSetData($nrocomercio ,$consulta.Fields("N_NROCOMERCIO").value)
GUICtrlSetData($nroterminal  ,$consulta.Fields("N_NROTERMINAL").value)
GUICtrlSetData($nrolote ,$consulta.Fields("N_NROLOTE").value)
GUICtrlSetData($tarjeta ,$consulta.Fields("X_TARJETA").value)
GUICtrlSetData($tipotarjeta ,$consulta.Fields("N_TIPOTARJETA").value)
GUICtrlSetData($titular,$consulta.Fields("X_NOMBRETITULAR").value)
;;;;
MsgBox($MB_SYSTEMMODAL, "", "Continuar  ")



EndFunc


