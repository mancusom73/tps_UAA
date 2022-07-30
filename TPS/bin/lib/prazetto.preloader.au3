; 13 July 2011

#include-once

Func _loadvar(ByRef $data,$var, $default='')
	Local $ret=$default, $table, $line
	$table = StringSplit($data,@CRLF,2)
	If IsArray($table) Then
		For $loop = 0 To UBound($table)-1
			$line = StringSplit($table[$loop],'=',2)
			If UBound($line) == 2 Then
				If $var = $line[0] Then
					$ret = $line[1]
					ExitLoop
					EndIf
				EndIf
			Next
		EndIf
	Return $ret
EndFunc

Func _preload(ByRef $data,$var, $default='',$folder='')
	Local $file, $array
	If StringInStr($folder,@CR,1) <> 0 Then
		$array = StringSplit($folder,@CR,2)
		Return _ResourceGetAsRaw($array[1],$var,1033,$array[0],1)
		Else
		$file = $folder & _loadvar($data,$var,$default)
		If FileExists($file) Then
			Return FileRead($file)
			Else
			ConsoleWrite('File '& $file & ' for ' & $var & ' skin are not found! Please check again template definition or file is exist?'&@CR)
			EndIf
		EndIf
EndFunc