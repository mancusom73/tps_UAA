#include-once

Opt("MustDeclareVars", 1)

Func isNullPtr(ByRef $param)
	Local $null = '0x00000000'
	If @AutoItX64 Then $null = '0x0000000000000000'
	Local $ret = 0
	If IsArray($param) Then
		If $param[0] == $null Then $ret = 1
		Else
		If $param == $null Then $ret = 1
		EndIf
	Return $ret
EndFunc

Func isNullInt(ByRef $param)
	Local $ret = 0
	If IsArray($param) Then
		If $param[0] == 0 Then $ret = 1
		Else
		If $param == 0 Then $ret = 1
		EndIf
	Return $ret
EndFunc

;by trancexx
Func _ResourceGetAsRaw($iResType, $iResName, $iResLang, $sModule, $iMode = 0, $iSize = 0)

	Local $iLoaded
	Local $a_hCall = DllCall($DLL_KERNEL32, "hwnd", "GetModuleHandleW", "wstr", $sModule)

	If @error Then
		Return SetError(1, 0, "")
	EndIf

	If Not $a_hCall[0] Then
		$a_hCall = DllCall($DLL_KERNEL32, "hwnd", "LoadLibraryExW", "wstr", $sModule, "hwnd", 0, "int", 1 + 2 + 8) ; 1 + 2, to be able x64 application to load x32 winimage and 3 to use search startegy

		If @error Or Not $a_hCall[0] Then
			Return SetError(2, 0, "")
		EndIf
		$iLoaded = 1
	EndIf

	Local $hModule = $a_hCall[0]

	Switch IsNumber($iResType) + 2 * IsNumber($iResName)
		Case 0
			$a_hCall = DllCall($DLL_KERNEL32, "hwnd", "FindResourceExW", _
					"hwnd", $hModule, _
					"wstr", $iResType, _
					"wstr", $iResName, _
					"int", $iResLang)
		Case 1
			$a_hCall = DllCall($DLL_KERNEL32, "hwnd", "FindResourceExW", _
					"hwnd", $hModule, _
					"int", $iResType, _
					"wstr", $iResName, _
					"int", $iResLang)
		Case 2
			$a_hCall = DllCall($DLL_KERNEL32, "hwnd", "FindResourceExW", _
					"hwnd", $hModule, _
					"wstr", $iResType, _
					"int", $iResName, _
					"int", $iResLang)
		Case 3
			$a_hCall = DllCall($DLL_KERNEL32, "hwnd", "FindResourceExW", _
					"hwnd", $hModule, _
					"int", $iResType, _
					"int", $iResName, _
					"int", $iResLang)
	EndSwitch

	If @error Or Not $a_hCall[0] Then
		If $iLoaded Then
			Local $a_iCall = DllCall($DLL_KERNEL32, "int", "FreeLibrary", "hwnd", $hModule)
			If @error Or Not $a_iCall[0] Then
				Return SetError(7, 0, "")
			EndIf
		EndIf
		Return SetError(3, 0, "")
	EndIf

	Local $hResource = $a_hCall[0]

	Local $a_iCall = DllCall($DLL_KERNEL32, "int", "SizeofResource", "hwnd", $hModule, "hwnd", $hResource)

	If @error Or Not $a_iCall[0] Then
		If $iLoaded Then
			Local $a_iCall = DllCall($DLL_KERNEL32, "int", "FreeLibrary", "hwnd", $hModule)
			If @error Or Not $a_iCall[0] Then
				Return SetError(7, 0, "")
			EndIf
		EndIf
		Return SetError(4, 0, "")
	EndIf

	Local $iSizeOfResource = $a_iCall[0]

	$a_hCall = DllCall($DLL_KERNEL32, "hwnd", "LoadResource", "hwnd", $hModule, "hwnd", $hResource)

	If @error Or Not $a_hCall[0] Then
		If $iLoaded Then
			Local $a_iCall = DllCall($DLL_KERNEL32, "int", "FreeLibrary", "hwnd", $hModule)
			If @error Or Not $a_iCall[0] Then
				Return SetError(7, 0, "")
			EndIf
		EndIf
		Return SetError(5, 0, "")
	EndIf

	Local $a_pCall = DllCall($DLL_KERNEL32, "ptr", "LockResource", "hwnd", $a_hCall[0])

	If @error Or Not $a_pCall[0] Then
		If $iLoaded Then
			Local $a_iCall = DllCall($DLL_KERNEL32, "int", "FreeLibrary", "hwnd", $hModule)
			If @error Or Not $a_iCall[0] Then
				Return SetError(7, 0, "")
			EndIf
		EndIf
		Return SetError(6, 0, "")
	EndIf

	Local $tOut
	Switch $iMode
		Case 0
			$tOut = DllStructCreate("char[" & $iSizeOfResource + 1 & "]", $a_pCall[0])
		Case 1
			$tOut = DllStructCreate("byte[" & $iSizeOfResource & "]", $a_pCall[0])
	EndSwitch

	Local $sReturnData = DllStructGetData($tOut, 1)

	If $iLoaded Then
		Local $a_iCall = DllCall($DLL_KERNEL32, "int", "FreeLibrary", "hwnd", $hModule)
		If @error Or Not $a_iCall[0] Then
			Return SetError(7, 0, "")
		EndIf
	EndIf

	Switch $iSize
		Case 0
			Return SetError(0, 0, $sReturnData)
		Case Else
			Switch $iMode
				Case 0
					Return SetError(0, 0, StringLeft($sReturnData, $iSize))
				Case 1
					Return SetError(0, 0, BinaryMid($sReturnData, 1, $iSize))
			EndSwitch
	EndSwitch

EndFunc   ;==>_ResourceGetAsRaw