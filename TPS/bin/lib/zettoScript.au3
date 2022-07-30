; 1 Nov 2011

#include-once

Opt("MustDeclareVars", 1)

; Get properties table of subclass
Func _zettoscript_GetParamChild(byref $param, $keyword, $cs=0)
	If Not IsArray($param) Then Return
	Local $nkeyword, $key
	$nkeyword = StringLen($keyword)
	For $loop = 0 To UBound($param) - 1
		$key = StringLower(StringLeft($param[$loop], $nkeyword))
		If $key = $keyword Then
			Return _zettoscript_GetParam($param[$loop],$keyword)
			ExitLoop
			EndIf
		Next
EndFunc

Func _zettoscript_Optimize($script)
	Local $nextChar
	Local $codeComments	= 0		; Signature for comments
	Local $codeString			; Signature for string as single or double quote
	Local $currentChar			; Current reading char
	Local $currentCharA			; Ascii Current reading char
	Local $finalChar			; Final Script
	Local $lastChar				; Latest reading char
	Local $statComments	= 0		; 1 Current reading inside comments
	Local $statRemember = 1		; 1 Remember lastest reading char in $lastChar
	Local $statSave
	Local $statString	= 0		; 1 current reading char inside string
								; 4747 for Single line  // and or 4742 Multi line /* comments */
	$script = ReplaceEOL($script,@CR)

	For $loop = 1 To StringLen($script)
		$statSave = 1
		$statRemember = 1
		$currentChar = StringMid($script,$loop,1)
		$currentCharA = Asc($currentChar)
		
		; if parser not inside string and not inside comments
		If Not $statString And Not $statComments Then
			; replace tab with space
			If $currentCharA == 9 Then
				$currentChar	= ' '
				$currentCharA	= 32
				EndIf

			; single space
			If $currentCharA == 32 And $lastChar == 32 Then
				$currentChar	= ''
				$lastChar		= 32
				$statSave		= 0
				$statRemember	= 0
				EndIf

			; single cr
			If $currentCharA == 13 And $lastChar == 13 Then
				$currentChar	= ''
				$currentCharA	= 13
				$lastChar		= 13
				$statSave		= 0
				$statRemember	= 0
				EndIf

			; if found this char, discard previous space
			If $lastChar == 32 Then
				Switch $currentCharA
					; " ' : ; { }
					Case 34, 39, 58, 59, 123, 125
						$finalChar = StringTrimRight($finalChar,1)
					; cr 'blank pharagraph' contains tab or space become single line
					Case 13
						$finalChar = StringTrimRight($finalChar,1)
						If Asc(StringRight($finalChar,1)) == 13 Then
							$currentChar	= ''
							$statSave		= 0
							$statRemember	= 0
							$currentCharA	= 0
							EndIf
					EndSwitch
				EndIf
			
			; if last char cr
			If $lastChar == 13 Then
				Switch $currentCharA
					; { }
					Case 123, 125
						$finalChar = StringTrimRight($finalChar,1)
					EndSwitch
				EndIf

			; if found this char, discard current space
			If $currentCharA == 32 Then
				Switch $lastChar
					; " ' : ; { }
					Case 34, 39, 58, 59, 123, 125
						$currentChar = ''
						$statSave		= 0
						$statRemember	= 0
					; cr 'blank pharagraph' contains tab or space become single line
					Case 13
						$currentChar	= ''
						$lastChar = 13
						$statSave		= 0
						$statRemember	= 0
					EndSwitch
				EndIf
			EndIf

			Switch $currentCharA
			; cr
			Case  13
				; Comments Single Line End
				If Not $statString And $statComments And $codeComments == 4747 Then
					$statComments = 0
					$codeComments = 0
					; if latest saved are cr
					If Asc(StringRight($finalChar,1)) == 13 Then
						$finalChar = StringTrimRight($finalChar,1)
						EndIf
					EndIf
			; " Double Quote String
			Case 34
				If Not $statComments Then
					If $statString Then
						If $codeString = 34 Then
							$statString = 0
							EndIf
						Else
						$statString = 1
						$codeString = 34
						EndIf
					EndIf
			; ' Single Quote String
			Case 39
				If Not $statComments Then
					If $statString And Not $statComments Then
						If $codeString = 39 Then
							$statString = 0
							EndIf
						Else
						$statString = 1
						$codeString = 39
						EndIf
					EndIf
		; /* Comment Start
		Case 47
			If Not $statString And Not $statComments Then
				$nextChar = Asc(StringMid($script,$loop+1,1))
				Switch $nextChar
					; // Multi Line Comments
					Case 42
						$statComments = 1
						$statRemember = 0
						$currentChar = ''
						$codeComments = 4742
					; // Single Line Comments
					Case 47
						$statComments = 1
						$statRemember = 0
						$currentChar = ''
						$codeComments = 4747
					EndSwitch
				EndIf
		EndSwitch

	; */ Comments Multi Line End
	If Not $statString And $statComments And $currentCharA == 42 Then
		If Asc(StringMid($script,$loop+1,1)) == 47 And $codeComments = 4742 Then
			; if next are cr
			If Asc(StringMid($script,$loop+2,1)) == 13 Then
			; if latest saved are cr
				If Asc(StringRight($finalChar,1)) == 13 Then
					$finalChar = StringTrimRight($finalChar,1)
					EndIf
				EndIf
			$statComments = 0
			$codeComments = 0
			$currentChar = ''
			$loop +=1
			EndIf
		EndIf

	If $statSave And Not $statComments Then $finalChar &= $currentChar
	If $statRemember And Not $statComments And Not $statString Then $lastChar = Asc($currentChar)
	Next

	If Asc(StringLeft($finalChar,1)) == 13 Then $finalChar = StringTrimLeft($finalChar,1)
	If Asc(StringRight($finalChar,1)) == 13 Then $finalChar = StringTrimRight($finalChar,1)
	$finalChar = ReplaceEOL($finalChar,'')
	
	Return $finalChar
EndFunc

; Get properties table of class
Func _zettoscript_GetParam($cscript,$keyword, $cs=0)
	Local $currentChar
	Local $codeString
	Local $currentCharAsc
	Local $paramArray
	Local $paramString
	Local $statOpen
	Local $statSave = 1
	Local $statString
	Local $statFound
	
	Local $nkeyword = StringLen($keyword)
	
	For $loop = 0 To StringLen($cscript)
		$currentChar = StringMid($cscript,$loop,1)
		$currentCharAsc = Asc($currentChar)
		
		; String parsing status
		Switch $currentCharAsc
			; " Double Quote String
			Case 34
			If $statString Then
				If $codeString = 34 Then $statString = 0
				Else
				$statString = 1
				$codeString = 34
				EndIf
			; ' Single Quote String
			Case 39
			If $statString Then
				If $codeString = 39 Then $statString = 0
				Else
				$statString = 1
				$codeString = 39
				EndIf
			EndSwitch
	
		; Escaping subclass parsing status
		If Not $statString Then
			Switch $currentCharAsc
				; } close
				Case 125
					If $statOpen = 1 Then
						; Reset variable for next line
						$currentChar = ''
						$paramString = ''
						EndIf
					$statOpen -= 1
				; { open
				Case 123
					$statOpen += 1
				EndSwitch
			EndIf
		
		If $statSave Then $paramString &= $currentChar
		
		If Not $statString And $statOpen = 0 Then
			If $nkeyword = StringLen($paramString) Then
				If $cs Then
					$statFound = $paramString == $keyword
					Else
					$statFound = $paramString = $keyword
					EndIf
				If $statFound Then
			
					Return _zettoscript_GetTable(StringTrimLeft($cscript,$loop))
					EndIf
				EndIf
			EndIf
	$statSave = 1
	Next
EndFunc

; Get properties of class and or subclass
Func _zettoscript_GetTable($cclass)
	Local $currentChar
	Local $codeString
	Local $currentCharAsc
	Local $paramArray
	Local $paramString
	Local $statOpen
	Local $statSave
	Local $statString
	
	For $loop = 0 To StringLen($cclass)
		$currentChar = StringMid($cclass,$loop,1)
		$currentCharAsc = Asc($currentChar)
		
		; String parsing status
		Switch $currentCharAsc
			; " Double Quote String
			Case 34
			If $statString Then
				If $codeString = 34 Then $statString = 0
				Else
				$statString = 1
				$codeString = 34
				EndIf
			; ' Single Quote String
			Case 39
			If $statString Then
				If $codeString = 39 Then $statString = 0
				Else
				$statString = 1
				$codeString = 39
				EndIf
			EndSwitch
	
		; Properties of class parsing status
		If Not $statString Then
			Switch $currentCharAsc
				; } close
				Case 125
					If $statOpen > 1 Then
						$currentCharAsc = 59
						EndIf
					$statOpen -= 1
					If $statOpen = 0 Then ExitLoop
				; { open
				Case 123
					$statSave = 1
					$statOpen += 1
					If $statOpen = 1 Then $statSave = 0
				EndSwitch
			EndIf
		
		If $statSave Then $paramString &=$currentChar
		
		; End of line parsing status ;
		If Not $statString And $statOpen = 1 And $currentCharAsc = 59 Then
			__ArrayEx_AddArray($paramArray, $paramString)
			; Reset variable for next line
			$paramString = ''
			EndIf
		
		; Reset variable
		$statSave = 1
		Next
	Return $paramArray
EndFunc

Func ReplaceEOL($data, $EOL=' ')
	$data = StringReplace($data,@CRLF,$EOL)
	$data = StringReplace($data,@CR,$EOL)
	$data = StringReplace($data,@LF,$EOL)
	Return $data
EndFunc

; add to library
; by zetto
Func __ArrayEx_AddArray(ByRef $library, ByRef $value)
	Local $n = UBound($library)
	If IsArray($library) Then
		ReDim $library[$n+1]
		$library[$n] = $value
		Else
		Local $arr[1]
		$arr[0] = $value
		$library = $arr
		EndIf
EndFunc


; 1 : Variable
; 2 : String
; 3 : Hexadecimal
Func _zettoscript_GetValueEx($param)
	Local $det, $ret
	$det = Asc(StringLeft($param,1))
	Select
		Case $det = 34 Or $det = 39; " or '
			$ret = _zettoscript_ParseString($param)	
		Case StringLeft($param,2) = '0x' And StringLen($param) <= 11
			$ret = StringTrimRight($param,1)
		Case StringRegExp(StringTrimRight($param,1),'\A[[:digit:]]*\Z')
			$ret = Execute(StringTrimRight($param,1))
		Case Else
			$ret = StringTrimRight($param,1)
		EndSelect
	Return $ret
EndFunc

Func _zettoscript_GetValue(byref $param, $keyword, $cs=0)
	$keyword = StringLower($keyword)
	If Not IsArray($param) Then Return
	Local $nkeyword, $key, $value, $ret
	$nkeyword = StringLen($keyword)
	For $loop = 0 To UBound($param) - 1
		$key = StringLower(StringLeft($param[$loop], $nkeyword))
		If $key == $keyword Then
			$value = StringTrimLeft($param[$loop],StringInStr($param[$loop],':'))
			Return _zettoscript_GetValueEx($value)
			ExitLoop
			EndIf
		Next
	Return $ret
EndFunc

Func _zettoscript_Exist(byref $param, $keyword, $cs=0)
	If Not IsArray($param) Then Return
	Local $nkeyword, $key, $value
	$nkeyword = StringLen($keyword)
	For $loop = 0 To UBound($param) - 1
		$key = StringLower(StringLeft($param[$loop], $nkeyword))
		If $key == $keyword Then
			Return 1
			ExitLoop
			EndIf
		Next
	Return
EndFunc

Func _zettoscript_ParseString(byref $param)
	Local $currentChar
	Local $codeString
	Local $currentCharAsc
	Local $paramString
	Local $statSave = 1
	Local $statString
	
	For $loop = 0 To StringLen($param)
		$currentChar = StringMid($param,$loop,1)
		$currentCharAsc = Asc($currentChar)
		
		; String parsing status
		Switch $currentCharAsc
			; " Double Quote String
			Case 34
			If $statString Then
				If $codeString = 34 Then $statString = 0
				Else
				$statString = 1
				$codeString = 34
				$statSave	= 0
				EndIf
			; ' Single Quote String
			Case 39
			If $statString Then
				If $codeString = 39 Then $statString = 0
				Else
				$statString = 1
				$codeString = 39
				$statSave	= 0
				EndIf
			EndSwitch
	
		If $statSave And $statString Then $paramString &= $currentChar
			
	$statSave = 1
	Next
	Return $paramString
EndFunc

Func isNullVar(ByRef $param)
	Local $null
	If $param = $null Then Return 1
EndFunc
	
Func zetto_AlterFont(ByRef $font,ByRef $size, ByRef $unit, ByRef $family)
	Local $ffont, $aTable = StringSplit($family,',',2)
	For $loop = 0 To UBound($aTable) - 1
		$ffont = StringLeft($aTable[$loop],StringInStr($aTable[$loop],':')-1)
		If isFontExist($ffont) Then
			$font = $ffont
			$size = StringTrimLeft($aTable[$loop],StringInStr($aTable[$loop],':'))
			Return 1
			EndIf
		Next	
	$font = 'Arial'
	$size = 11
	$unit = 3
	Return 1
EndFunc

Func isFontExist($font)
	Local $ptr = _GDIPlus_FontFamilyCreate ($font)
	If Not isNullPtr($ptr) Then
		_GDIPlus_FontFamilyDispose($ptr)
		Return 1
		EndIf
EndFunc

Func __zetto_Format2Val(ByRef $var)
	Switch $var
		Case 'PXF01INDEXED'
		$var = 0x00030101
		Case 'PXF04INDEXED'
		$var = 0x00030402
		Case 'PXF08INDEXED'
		$var = 0x00030803
		Case 'PXF16GRAYSCALE'
		$var = 0x00101004
		Case 'PXF16RGB555'
		$var = 0x00021005
		Case 'PXF16RGB565'
		$var = 0x00021006
		Case 'PXF16ARGB1555'
		$var = 0x00061007
		Case 'PXF24RGB'
		$var = 0x00021808
		Case 'PXF32RGB'
		$var = 0x00022009
		Case 'PXF32ARGB'
		$var = 0x0026200A
		Case 'PXF32PARGB'
		$var = 0x000D200B
		Case 'PXF48RGB'
		$var = 0x0010300C
		Case 'PXF64ARGB'
		$var = 0x0034400D 
		Case 'PXF64PARGB'
		$var = 0x001C400E
		EndSwitch
EndFunc