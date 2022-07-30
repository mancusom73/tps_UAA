#include-once

Global $lastError = False, $lastErrorNo = -1
Global $objConn

Global $oMyError = ObjEvent("AutoIt.Error", "__obj_error")

Global Const $ODBCDriver = "HKEY_LOCAL_MACHINE\SOFTWARE\ODBC\ODBCINST.INI\ODBC Drivers"
Global $MYSQL_REG = __mysql_odbc($ODBCDriver)
Global Const $MYSQL_VERSION = $MYSQL_REG[1]

Func mysql_connect($sHost, $sUserName, $sPassword, $sDatabase, $iPort = 3306, $sDriver = Default)
    If Not IsString($sHost) Then Return __mysql_seterr("$sHost parameter is not a string in mysql_connect!")
    If Not IsString($sUserName) Then Return __mysql_seterr("$sUserName parameter is not a string in mysql_connect!")
    If Not IsString($sPassword) Then Return __mysql_seterr("$sPassword parameter is not a string in mysql_connect!")
    If Not IsString($sDatabase) Then Return __mysql_seterr("$sDatabase parameter is not a string in mysql_connect!")
    If Not IsNumber($iPort) Then Return __mysql_seterr("$iPort parameter is not a string in mysql_connect!")
    If $sDriver = Default Then $sDriver = "{MySQL ODBC " & $MYSQL_VERSION & " Driver}"
    If Not IsString($sDriver) Then Return __mysql_seterr("$sDriver parameter is not a string in mysql_connect!")

    Local $strDriverConn

    $objConn = ObjCreate("ADODB.Connection")

    $strDriverConn = "DRIVER=" & $sDriver & ";SERVER=" & $sHost & ";DATABASE=" & $sDatabase & ";UID=" & $sUserName & ";PWD=" & $sPassword & ";PORT=" & $iPort

    $objConn_ect = $objConn.Open($strDriverConn)
    If @error Then Return False

    Return $objConn
EndFunc   ;==>mysql_connect

Func mysql_query($sQuery, $sConn = Default)
    If Not IsString($sQuery) Then Return __mysql_seterr("[" & $MYSQL_VERSION & "] $sQuery must be a string in mysql_query")
    If Not $sConn = Default Then $objConn = $sConn

    If IsObj($objConn) Then
        Local $arSQLRet
        $objConn_quer = $objConn.Execute($sQuery)
        If Not $objConn_quer Then Return False

        Return $objConn_quer
    Else
        Return False
    EndIf
EndFunc   ;==>mysql_query

Func mysql_fetch_array($arrObj, $sConn = Default)
    If Not $sConn = Default Then $objConn = $sConn

    If IsObj($objConn) Then
        Local $iCol = 0, $iFields = 0
        Local $__mysql_temp = $objConn.Execute($arrObj)
        If @error Then Return False

        Local $__mysql_field_count = $__mysql_temp.Fields.Count
        Local $__mysql_arr[1][$__mysql_field_count][2]

        Do
            For $i In $__mysql_temp.Fields
                ReDim $__mysql_arr[$iCol + 1][$iFields + 1][2]

                $__mysql_arr[$iCol][$iFields][0] = $i.Name
                $__mysql_arr[$iCol][$iFields][1] = $i.Value

                $iFields += 1
            Next

            $iCol +=1
            $iFields = 0
            $__mysql_temp.MoveNext
        Until $__mysql_temp.EOF

        Return $__mysql_arr
    Else
        Return False
    EndIf
EndFunc   ;==>mysql_fetch_array

Func mysql_drop_db($sDatabaseName, $sConn = Default)
    If Not $sConn = Default Then $objConn = $sConn

    If IsObj($objConn) Then
        ; This function is deprecated, it is preferable to use mysql_query() to issue an sql DROP DATABASE statement instead.
        Local $sQuery
        $sQuery = "DROP DATABASE " & $sDatabaseName & ";"

        Local $__mysql_temp = $objConn.Execute($sQuery)
        If @error Then Return False
    Else
        Return False
    EndIf
EndFunc

Func mysql_error()
    Return $lastError
EndFunc   ;==>mysql_error

Func mysql_errno()
    Return $lastErrorNo
EndFunc

Func mysql_close($sConn = Default)
    If Not $sConn = Default Then $objConn = $sConn
    $objConn.Close
EndFunc   ;==>mysql_close

;;--------------------------------------------------------------------------
;; Internal Functions
;; __mysql_odbc()   - Matches registry string for version
;; __mysql_seterr() - Sets the last error string
;; __mysql_getver() - Returns the installed ODBC connector version
;; __obj_error()    - Returns any ODBC errors thrown by the connector
;;--------------------------------------------------------------------------

Func __mysql_odbc($strKey)
    Local $i, $arODBC
    While 1
        $i += 1
        $regVal = RegEnumVal($strKey, $i)
        $arODBC = StringRegExp($regVal, "MySQL ODBC (.*?) Driver", 2)
        If IsArray($arODBC) Then Return $arODBC
    WEnd
EndFunc   ;==>__mysql_odbc

Func __mysql_seterr($__sString)
    $lastError = $__sString
    Return $lastError
EndFunc   ;==>__mysql_seterr

Func __mysql_getver()
    If __mysql_odbc($ODBCDriver) Then Return __mysql_odbc($ODBCDriver)
EndFunc   ;==>__mysql_getver

Func __obj_error()
    $lastErrorNo = Hex($oMyError.Number, 8)
    __mysql_seterr($oMyError.description)
    SetError(Hex($oMyError.number, 8), $oMyError.lastdllerror)
EndFunc   ;==>__obj_error