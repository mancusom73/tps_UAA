#include-once
#include-once

; #INDEX# ============================================================================================================
; Title .........: Toast
; AutoIt Version : 3.3.8.0
; Language ......: English
; Description ...: Show and hides slice messages from the systray in user defined colours and fonts
; Author(s) .....: Melba23 - using some code from guinness and MilesAhead in _Toast_Locate
; ====================================================================================================================

;#AutoIt3Wrapper_Au3Check_Parameters=-d -w 1 -w 2 -w 3 -w- 4 -w 5 -w 6 -w- 7

; #INCLUDES# =========================================================================================================
#include <StringSize.au3>
#include <GDIPlus.au3>

; #GLOBAL VARIABLES# =================================================================================================
Global Const $AW_SLIDE          = 0x00040000 ; Animation (type) constants
Global Const $AW_CENTER         = 0x00000010 ;   ""
Global Const $AW_BLEND          = 0x00080000 ;   ""
Global Const $AW_ROLL           = 0x00000000 ;   ""
Global Const $SLICEUP_TIME      = 1000       ; milliseconds, time for slice to appear
Global Const $SLICEDOWN_TIME    =  500       ; milliseconds, time for slice to disappear
Global Const $END_OF_TASKBAR    =    0       ; Toast will appear at the end of the taskbar (default)
Global Const $CENTER_OF_TASKBAR =    1       ; Toast will appear in the center of the taskbar

Global $iDef_Toast_Font_Size = _Toast_GetDefFont(0)
Global $sDef_Toast_Font_Name = _Toast_GetDefFont(1)

Global $hToast_Handle = 0
Global $hToast_Close_X = 9999
Global $iToast_Move = 0
Global $iToast_Style = 1 ; $SS_CENTER
Global $aRet = DllCall("User32.dll", "int", "GetSysColor", "int", 8) ; $COLOR_WINDOWTEXT = 8
Global $iToast_Header_BkCol = $aRet[0]
$aRet = DllCall("User32.dll", "int", "GetSysColor", "int", 5) ; $COLOR_WINDOW = 5
Global $iToast_Header_Col = $aRet[0]
Global $iToast_Header_Bold = 0
Global $iToast_Message_BkCol = $iToast_Header_Col
Global $iToast_Message_Col = $iToast_Header_BkCol
Global $iToast_Font_Size = $iDef_Toast_Font_Size
Global $sToast_Font_Name = $sDef_Toast_Font_Name
Global $iToast_Taskbar = 0
Global $iToast_Timer = 0
Global $iToast_Start = 0
Global $fToast_Close = False
Global $iToast_Taskbar_Pos = $END_OF_TASKBAR ; Set default Toast on Taskbar position to end of taskbar.if Animation is $AW_SLIDE or $AW_ROLL, center Toast on screen if Animation is $AW_CENTER or $AW_BLEND
Global $bToast_Animation = $AW_SLIDE ; Animation (type)
Global $iSliceUp_Time = $SLICEUP_TIME ; milliseconds, time for slice to appear
Global $iSliceDown_Time = $SLICEDOWN_TIME ; milliseconds, time for slice to disappear

; #CURRENT# ==========================================================================================================
; _Toast_Set:  Sets text justification and optionally colours and font, for _Toast_Show function calls
; _Toast_Set_Timers_Animation: Sets the Toast "up" and "down" speed, animation type, and Toast position relative to taskbar
; _Toast_Show: Shows a slice message from the systray
; _Toast_Hide: Hides a slice message from the systray
; ====================================================================================================================

; #INTERNAL_USE_ONLY#=================================================================================================
; _Toast_Locate:          Find Systray and determine Toast start position and movement direction
; _Toast_Timer_Check:     Checks whether Toast has timed out or closure [X] clicked
; _Toast_WM_EVENTS:       Message handler to check if closure [X] clicked
; _Toast_GetDefFont:      Determine system default MsgBox font and size
; _Toast_ShowPNG:         Set PNG as image
; _Toast_BitmapCreateDIB: Create bitmap
; ====================================================================================================================

; #FUNCTION# =========================================================================================================
; Name...........: _Toast_Set
; Description ...: Sets text justification and optionally colours and font, for _Toast_Show function calls
; Syntax.........: _Toast_Set($vJust, [$iHdr_BkCol, [$iHdr_Col, [$iMsg_BkCol, [$iMsg_Col, [$sFont_Size, [$iFont_Name, [$iTaskbar]]]]]]])
; Parameters ....: $vJust     - 0 = Left justified, 1 = Centered (Default), 2 = Right justified
;                                Can use $SS_LEFT, $SS_CENTER, $SS_RIGHT
;                                + 4 = Header text in bold
;                       >>>>>    Setting this parameter to' Default' will reset ALL parameters to default values     <<<<<
;                       >>>>>    All optional parameters default to system MsgBox default values                     <<<<<
;                  $iHdr_BkCol - [Optional] The colour for the title bar background
;                  $iHdr_Col   - [Optional] The colour for the title bar text
;                  $iMsg_BkCol - [Optional] The colour for the message background
;                  $iMsg_Col   - [Optional] The colour for the message text
;                                Omitting a colour parameter or setting it to -1 leaves it unchanged
;                                Setting a colour parameter to Default resets the system colour
;                  $iFont_Size - [Optional] The font size in points to use for the Toast
;                  $sFont_Name - [Optional] The font to use for the Toast
;                       >>>>>    Omitting a font parameter, setting size to -1 or name to "" leaves it unchanged     <<<<<
;                       >>>>>    Setting a font parameter to Default resets the system message box font or size      <<<<<
;                  $iTaskbar   - [Optional] 0 - Taskbar on main monitor (default)
;                                           1 - Taskbar on secondary monitor to right
;                                           2 - Taskbar on secondary monitor below
;                                           3 - Taskbar on secondary monitor to left
;                                           4 - Taskbar on secondary monitor above
;                                          +8 - Toolbar docked to taskbar
; Requirement(s).: v3.3.2.0 or higher - AdlibRegister/Unregister used in _Toast_Show
; Return values .: Success - Returns 1
;                  Failure - Returns 0 and sets @error to 1 with @extended set to parameter index number
; Author ........: Melba23
; Example........; Yes
;=====================================================================================================================

Func _Toast_Set($vJust, $iHdr_BkCol = -1, $iHdr_Col = -1, $iMsg_BkCol = -1, $iMsg_Col = -1, $iFont_Size = -1, $sFont_Name = "", $iTaskbar = 0)

    ; Set parameters
    Switch $vJust
        Case Default
            $iToast_Style = 1; $SS_CENTER
            $aRet = DllCall("User32.dll", "int", "GetSysColor", "int", 8) ; $COLOR_WINDOWTEXT = 8
            $iToast_Header_BkCol = $aRet[0]
            $aRet = DllCall("User32.dll", "int", "GetSysColor", "int", 5) ; $COLOR_WINDOW = 5
            $iToast_Header_Col = $aRet[0]
            $iToast_Message_BkCol = $iToast_Header_Col
            $iToast_Message_Col = $iToast_Header_BkCol
            $sToast_Font_Name = $sDef_Toast_Font_Name
            $iToast_Font_Size = $iDef_Toast_Font_Size
            $iToast_Taskbar = 0
            Return
        Case 0, 1, 2, 4, 5, 6
            $iToast_Style = $vJust
        Case -1
            ; Do nothing
        Case Else
            Return SetError(1, 1, 0)
    EndSwitch

    Switch $iHdr_BkCol
        Case Default
            $aRet = DllCall("User32.dll", "int", "GetSysColor", "int", 8) ; $COLOR_WINDOWTEXT = 8
            $iToast_Header_BkCol = $aRet[0]
        Case 0 To 0xFFFFFF
            $iToast_Header_BkCol = Int($iHdr_BkCol)
        Case -1
            ; Do nothing
        Case Else
            Return SetError(1, 2, 0)
    EndSwitch

    Switch $iHdr_Col
        Case Default
            $aRet = DllCall("User32.dll", "int", "GetSysColor", "int", 5) ; $COLOR_WINDOW = 5
            $iToast_Header_Col = $aRet[0]
        Case 0 To 0xFFFFFF
            $iToast_Header_Col = Int($iHdr_Col)
        Case -1
            ; Do nothing
        Case Else
            Return SetError(1, 3, 0)
    EndSwitch

    Switch $iMsg_BkCol
        Case Default
            $aRet = DllCall("User32.dll", "int", "GetSysColor", "int", 5) ; $COLOR_WINDOW = 5
            $iToast_Message_BkCol = $aRet[0]
        Case 0 To 0xFFFFFF
            $iToast_Message_BkCol = Int($iMsg_BkCol)
        Case -1
            ; Do nothing
        Case Else
            Return SetError(1, 4, 0)
    EndSwitch

    Switch $iMsg_Col
        Case Default
            $aRet = DllCall("User32.dll", "int", "GetSysColor", "int", 8) ; $COLOR_WINDOWTEXT = 8
            $iToast_Message_Col = $aRet[0]
        Case 0 To 0xFFFFFF
            $iToast_Message_Col = Int($iMsg_Col)
        Case -1
            ; Do nothing
        Case Else
            Return SetError(1, 5, 0)
    EndSwitch

    Switch $iFont_Size
        Case Default
            $iToast_Font_Size = $iDef_Toast_Font_Size
        Case 8 To 72
            $iToast_Font_Size = Int($iFont_Size)
        Case -1
            ; Do nothing
        Case Else
            Return SetError(1, 6, 0)
    EndSwitch

    Switch $sFont_Name
        Case Default
            $sToast_Font_Name = $sDef_Toast_Font_Name
        Case ""
            ; Do nothing
        Case Else
            If IsString($sFont_Name) Then
                $sToast_Font_Name = $sFont_Name
            Else
                Return SetError(1, 7, 0)
            EndIf
    EndSwitch

    Switch $iTaskbar
        Case Default
            $iToast_Taskbar = 0
        Case 0 To 4, 8 To 12
            If IsInt($iTaskbar) Then
                $iToast_Taskbar = $iTaskbar
            Else
                $iToast_Taskbar = 0
            EndIf
    EndSwitch

    Return 1

EndFunc   ;==>_Toast_Set

; #FUNCTION# =========================================================================================================
; Name...........: _Toast_Set_Timers_Animation
; Description ...: Set the Slice time "up" and "down"; sets animation type.
;                  Center the Toast and set Animation type.
;                  change whether killing the slice timer in
; Syntax.........: _Toast_Set_Timers_Animation($iSliceUp_TimeMs, [$iSliceDown_TimeMs, [$iTaskbar_Pos, [$bAnimation]]])
; Parameters ....: $iSliceUp_TimeMs   - integer, time in milliseconds to pop the slice "up".
;                              >>>>>    Setting this parameter to 'Default' will reset ALL parameters to default values           <<<<<
;                              >>>>>    however, unlike _Toast_Set(), the remaining parameters may be specified                   <<<<<
;                              >>>>>    even though this parameter is set to 'Default'.                                           <<<<<
;                              >>>>>    For example, _Toast_Set_Timers_Animation(Default, -1, -1, $AW_CENTER) will reset          <<<<<
;                              >>>>>    the first 3 parameters to their defaults and change the animation style to $AW_CENTER.    <<<<<
;                  $iSliceDown_TimeMs - [Optional] integer, time in milliseconds to pop the slice "down".
;                  $iTaskbar_Pos      - [Optional] integer, Toast position on Taskbar if Animation is $AW_SLIDE or $AW_ROLL,
;                                       ignored if Animation is $AW_CENTER or $AW_BLEND.
;                                       0 = $END_OF_TASKBAR (default) = Toast will appear at the end of the taskbar
;                                       1 = $CENTER_OF_TASKBAR        = Toast will appear in the center of taskbar
;                  $bAnimation        - [Optional] integer, animation type: $AW_SLIDE (Default), $AW_CENTER, $AW_BLEND, or $AW_ROLL
; Requirement(s).: v3.3.2.0 or higher
; Return values .: Success - Returns 1
;                  Failure - Returns 0 and sets @error to 1 with @extended set to parameter index number
; Author ........: jrs
; Remarks .......: A parameter set to -1 retains it's current value.
; Example........; Yes
;=====================================================================================================================

Func _Toast_Set_Timers_Animation($iSliceUp_TimeMs, $iSliceDown_TimeMs = -1, $iTaskbar_Pos = -1, $bAnimation = -1)

	; Set parameters

	; If 1st param = Default and the rest are omitted, set everything to defaults.
	If $iSliceUp_TimeMs = Default Then
		$iSliceUp_Time = $SLICEUP_TIME
		$iSliceDown_Time = $SLICEDOWN_TIME
		$iToast_Taskbar_Pos = $END_OF_TASKBAR
		$bToast_Animation = $AW_SLIDE
		; function does NOT return after 1st parameter = 'Default',
	EndIf

	; Slice Up Time
	Switch $iSliceUp_TimeMs
		Case Default
			$iSliceUp_Time = $SLICEUP_TIME
		Case -1
			; Do nothing
		Case Else
			If IsInt($iSliceUp_TimeMs) Then
				$iSliceUp_Time = $iSliceUp_TimeMs
			Else
				Return SetError(1, 1, 0)
			EndIf
	EndSwitch

	; Slice Down Time
	Switch $iSliceDown_TimeMs
		Case Default
			$iSliceDown_Time = $SLICEDOWN_TIME
		Case -1
			; Do nothing
		Case Else
			If IsInt($iSliceDown_TimeMs) Then
				$iSliceDown_Time = $iSliceDown_TimeMs
			Else
				Return SetError(1, 2, 0)
			EndIf
	EndSwitch

	; Toast position on Taskbar
	Switch $iTaskbar_Pos
		Case Default
			$iToast_Taskbar_Pos = $END_OF_TASKBAR
		Case -1
			; Do nothing
		Case $END_OF_TASKBAR, $CENTER_OF_TASKBAR
			$iToast_Taskbar_Pos = $iTaskbar_Pos
		Case Else
			Return SetError(1, 3, 0)
	EndSwitch

	; Toast animation style
	Switch $bAnimation
		Case Default
			$bToast_Animation = $AW_SLIDE
		Case $AW_SLIDE, $AW_CENTER, $AW_BLEND, $AW_ROLL
			$bToast_Animation = $bAnimation
		Case -1
			; Do nothing
		Case Else
			Return SetError(1, 4, 0)
	EndSwitch

EndFunc   ;==>_Toast_Set_Timers_Animation

; #FUNCTION# =========================================================================================================
; Name...........: _Toast_Show
; Description ...: Shows a slice message from the systray
; Syntax.........: _Toast_Show($vIcon, $sTitle, $sMessage, [$iDelay [, $fWait [, $fRaw]]])
; Parameters ....: $vIcon    - 0 - No icon, 8 - UAC, 16 - Stop, 32 - Query, 48 - Exclamation, 64 - Information
;                              The $MB_ICON constant can also be used for the last 4 above
;                              If set to the name of an exe, the main icon of that exe will be displayed
;                              If set to the name of an image file, that image will be displayed
;                              Any other value returns -1, error 1
;                  $sTitle   - Text to display on Title bar
;                  $sMessage - Text to display in Toast body
;                  $iDelay   - The delay in seconds before the Toast retracts or script continues (Default = 0)
;                              If negative, an [X] is added to the title bar. Clicking [X] retracts/continues immediately
;                  $fWait    - True  - Script waits for delay time before continuing and Toast remains visible
;                              False - Script continues and Toast retracts automatically after delay time
;                  $fRaw     - True  - Message is not wrapped and Toast expands to show full width
;                            - False - Message is wrapped if over max preset Toast width
; Requirement(s).: v3.3.1.5 or higher - AdlibRegister/Unregister used in _Toast_Show
; Return values .: Success: Returns 2-element array: [Toast width, Toast height]
;                  Failure: Returns -1 and sets @error as follows:
;                           1 = Toast GUI creation failed
;                           2 = Taskbar not found
;                           4 = When using Raw, the Toast is too wide for the display
;                           3 = StringSize error
; Author ........: Melba23, based on some original code by GioVit for the Toast
; Notes .........; Any visible Toast is retracted by a subsequent _Toast_Hide or _Toast_Show, or clicking a visible [X]
; Example........; Yes
;=====================================================================================================================

Func _Toast_Show($vIcon, $sTitle, $sMessage, $iDelay = 0, $fWait = True, $fRaw = False, $posicion = 1)

    ; Store current GUI mode and set Message mode
    Local $nOldOpt = Opt('GUIOnEventMode', 0)

    ; Retract any Toast already in place
    If $hToast_Handle <> 0 Then _Toast_Hide()

    ; Reset non-reacting Close [X] ControlID
    $hToast_Close_X = 9999

    ; Set default auto-sizing Toast widths
    Local $iToast_Width_max = 500
    Local $iToast_Width_min = 150

    ; Check for icon
    Local $iIcon_Style = 0
    Local $iIcon_Reduction = 36
    Local $sDLL = "user32.dll"
    Local $sImg = ""
    If StringIsDigit($vIcon) Then
        Switch $vIcon
            Case 0
                $iIcon_Reduction = 0
            Case 8
                $sDLL = "imageres.dll"
                $iIcon_Style = 78
            Case 16 ; Stop
                $iIcon_Style = -4
            Case 32 ; Query
                $iIcon_Style = -3
            Case 48 ; Exclam
                $iIcon_Style = -2
            Case 64 ; Info
                $iIcon_Style = -5
            Case Else
                Return SetError(1, 0, -1)
        EndSwitch
    Else
        Switch StringLower(StringRight($vIcon, 3))
            Case "exe", "ico"
                $sDLL = $vIcon
                $iIcon_Style = 0
            Case "bmp", "jpg", "gif", "png"
                $sImg = $vIcon
        EndSwitch
    EndIf

    ; Determine max message width
    Local $iMax_Label_Width = $iToast_Width_max - 20 - $iIcon_Reduction
    If $fRaw = True Then $iMax_Label_Width = 0

    ; Get message label size
    Local $aLabel_Pos = _StringSize($sMessage, $iToast_Font_Size, Default, Default, $sToast_Font_Name, $iMax_Label_Width)
    If @error Then
        $nOldOpt = Opt('GUIOnEventMode', $nOldOpt)
        Return SetError(3, 0, -1)
    EndIf

    ; Reset text to match rectangle
    $sMessage = $aLabel_Pos[0]

    ;Set line height for this font
    Local $iLine_Height = $aLabel_Pos[1]

    ; Set label size
    Local $iLabelwidth = $aLabel_Pos[2]
    Local $iLabelheight = $aLabel_Pos[3]

    ; Set Toast size
    Local $iToast_Width = $iLabelwidth + 20 + $iIcon_Reduction
    ; Check if Toast will fit on screen
    If $iToast_Width > @DesktopWidth - 20 Then
        $nOldOpt = Opt('GUIOnEventMode', $nOldOpt)
        Return SetError(4, 0, -1)
    EndIf
    ; Increase if below min size
    If $iToast_Width < $iToast_Width_min + $iIcon_Reduction Then
        $iToast_Width = $iToast_Width_min + $iIcon_Reduction
        $iLabelwidth = $iToast_Width_min - 20
    EndIf

    ; Set title bar height - with minimum for [X]
    Local $iTitle_Height = 0
    If $sTitle = "" Then
        If $iDelay < 0 Then $iTitle_Height = 6
    Else
        $iTitle_Height = $iLine_Height + 2
        If $iDelay < 0 Then
            If $iTitle_Height < 17 Then $iTitle_Height = 17
        EndIf
    EndIf

    ; Set Toast height as label height + title bar + bottom margin
    Local $iToast_Height = $iLabelheight + $iTitle_Height + 20
    ; Ensure enough room for icon if displayed
    If $iIcon_Reduction Then
        If $iToast_Height < $iTitle_Height + 42 Then $iToast_Height = $iTitle_Height + 47
    EndIf

    ; Get Toast starting position and direction
    Local $aToast_Data = _Toast_Locate($iToast_Width, $iToast_Height)

    ; Create Toast slice with $WS_POPUPWINDOW, $WS_EX_TOOLWINDOW style and $WS_EX_TOPMOST extended style
    ;hmm$hToast_Handle = GUICreate("", $iToast_Width, $iToast_Height, $aToast_Data[0], $aToast_Data[1], 0x80880000, BitOR(0x00000080, 0x00000008))
	
	$hToast_Handle = GUICreate("POPUPPROMO", $iToast_Width, $iToast_Height, $aToast_Data[0], $aToast_Data[1] - ($iToast_Height * $posicion), 0x80880000, BitOR(0x00000080, 0x00000008))
    If @error Then
        $nOldOpt = Opt('GUIOnEventMode', $nOldOpt)
        Return SetError(1, 0, -1)
    EndIf
    GUISetFont($iToast_Font_Size, Default, Default, $sToast_Font_Name)
    GUISetBkColor($iToast_Message_BkCol)

    ; Set centring parameter
    Local $iLabel_Style = 0 ; $SS_LEFT
    If BitAND($iToast_Style, 1) = 1 Then
        $iLabel_Style = 1 ; $SS_CENTER
    ElseIf BitAND($iToast_Style, 2) = 2 Then
        $iLabel_Style = 2 ; $SS_RIGHT
    EndIf

    ; Check installed fonts
    Local $sX_Font = "WingDings"
    Local $sX_Char = "x"
    Local $i = 1
    While 1
        Local $sInstalled_Font = RegEnumVal("HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts", $i)
        If @error Then ExitLoop
        If StringInStr($sInstalled_Font, "WingDings 2") Then
            $sX_Font = "WingDings 2"
            $sX_Char = "T"
        EndIf
        $i += 1
    WEnd

    ; Create title bar if required
    If $sTitle <> "" Then

        ; Create disabled background strip
        GUICtrlCreateLabel("", 0, 0, $iToast_Width, $iTitle_Height)
        GUICtrlSetBkColor(-1, $iToast_Header_BkCol)
        GUICtrlSetState(-1, 128) ; $GUI_DISABLE

        ; Set title bar width to offset text
        Local $iTitle_Width = $iToast_Width - 10

        ; Create closure [X] if needed
        If $iDelay < 0 Then
            ; Create [X]
            Local $iX_YCoord = Int(($iTitle_Height - 17) / 2)
            $hToast_Close_X = GUICtrlCreateLabel($sX_Char, $iToast_Width - 18, $iX_YCoord, 17, 17)
            GUICtrlSetFont(-1, 14, Default, Default, $sX_Font)
            GUICtrlSetBkColor(-1, -2) ; $GUI_BKCOLOR_TRANSPARENT
            GUICtrlSetColor(-1, $iToast_Header_Col)
            ; Reduce title bar width to allow [X] to activate
            $iTitle_Width -= 18
        EndIf

        ; Create Title label with bold text, centered vertically in case bar is higher than line
        GUICtrlCreateLabel($sTitle, 10, 0, $iTitle_Width, $iTitle_Height, 0x0200) ; $SS_CENTERIMAGE
        GUICtrlSetBkColor(-1, $iToast_Header_BkCol)
        GUICtrlSetColor(-1, $iToast_Header_Col)
        If BitAND($iToast_Style, 4) = 4 Then GUICtrlSetFont(-1, $iToast_Font_Size, 600)

    Else

        If $iDelay < 0 Then
            ; Only need [X]
            $hToast_Close_X = GUICtrlCreateLabel($sX_Char, $iToast_Width - 18, 0, 17, 17)
            GUICtrlSetFont(-1, 14, Default, Default, $sX_Font)
            GUICtrlSetBkColor(-1, -2) ; $GUI_BKCOLOR_TRANSPARENT
            GUICtrlSetColor(-1, $iToast_Message_Col)
        EndIf

    EndIf

    ; Create icon
    If $iIcon_Reduction Then
        Switch StringLower(StringRight($sImg, 3))
            Case "bmp", "jpg", "gif"
                ;GUICtrlCreatePic($sImg, 10, 10 + $iTitle_Height, 32, 32) hmmancuso
				GUICtrlCreatePic($sImg, 1, 10 + $iTitle_Height , 82, 82)
            Case "png"
                _Toast_ShowPNG($sImg, $iTitle_Height)
            Case Else
                GUICtrlCreateIcon($sDLL, $iIcon_Style, 10, 10 + $iTitle_Height)
        EndSwitch
    EndIf

    ; Create Message label
    ; hmmancusoGUICtrlCreateLabel($sMessage, 10 + $iIcon_Reduction, 10 + $iTitle_Height, $iLabelwidth, $iLabelheight)
	GUICtrlCreateLabel($sMessage, 45 + $iIcon_Reduction, 10 + $iTitle_Height, $iLabelwidth, $iLabelheight)
    GUICtrlSetStyle(-1, $iLabel_Style)
    If $iToast_Message_Col <> Default Then GUICtrlSetColor(-1, $iToast_Message_Col)

    ; Slide Toast Slice into view from behind systray and activate hmm
    DllCall("user32.dll", "int", "AnimateWindow", "hwnd", $hToast_Handle, "int", $iSliceUp_Time, "long", $aToast_Data[2])

    ; Activate Toast without stealing focus
    GUISetState(@SW_SHOWNOACTIVATE, $hToast_Handle)

    ; If script is to pause
    If $fWait = True Then

        ; Clear message queue
        Do
        Until GUIGetMsg() = 0

        ; Begin timeout counter
        Local $iTimeout_Begin = TimerInit()

        ; Wait for timeout or closure
        While 1
            If GUIGetMsg() = $hToast_Close_X Or TimerDiff($iTimeout_Begin) / 1000 >= Abs($iDelay) Then ExitLoop
        WEnd

        ; If script is to continue and delay has been set
    ElseIf Abs($iDelay) > 0 Then

        ; Store timer info
        $iToast_Timer = Abs($iDelay * 1000)
        $iToast_Start = TimerInit()

        ; Register Adlib function to run timer
        AdlibRegister("_Toast_Timer_Check", 100)
        ; Register message handler to check for [X] click
        GUIRegisterMsg(0x0021, "_Toast_WM_EVENTS") ; $WM_MOUSEACTIVATE

    EndIf

    ; Reset original mode
    $nOldOpt = Opt('GUIOnEventMode', $nOldOpt)

    ; Create array to return Toast dimensions
    Local $aToast_Data[3] = [$iToast_Width, $iToast_Height, $iLine_Height]

    Return $aToast_Data

EndFunc   ;==>_Toast_Show

; #FUNCTION# ========================================================================================================
; Name...........: _Toast_Hide
; Description ...: Hides a slice message from the systray
; Syntax.........: _Toast_Hide()
; Requirement(s).: v3.3.1.5 or higher - AdlibRegister used in _Toast_Show
; Return values .: Success: Returns 0
;                  Failure: If Toast does not exist returns -1 and sets @error to 1
; Author ........: Melba23
; Example........; Yes
;=====================================================================================================================

Func _Toast_Hide()

    ; If no Toast to hide, return
    If $hToast_Handle = 0 Then Return SetError(1, 0, -1)

    ; Slide Toast back behind systray
    DllCall("user32.dll", "int", "AnimateWindow", "hwnd", $hToast_Handle, "int", $iSliceDown_Time, "long", $iToast_Move)
    ; Delete Toast slice
    GUIDelete($hToast_Handle)
    ; Set flag for "no Toast"
    $hToast_Handle = 0
    ; Unregister a possible message handler
    GUIRegisterMsg(0x0021, "") ; $WM_MOUSEACTIVATE
    ; Unregister a possible Adlib function
    AdlibUnRegister("_Toast_Timer_Check")
    ; Clear possible flag
    $fToast_Close = False

EndFunc   ;==>_Toast_Hide

; #INTERNAL_USE_ONLY#============================================================================================================
; Name...........: _Toast_Locate
; Description ...: Find Systray and determine Toast start position and movement direction
; Syntax ........: _Toast_Locate($iToast_Width, $iToast_Height)
; Parameters ....: $iToast_Width  - required width of slice
;                  $iToast_Height - required height of slice
; Author ........: Melba23 - using some code from guinness and MilesAhead
; Modified.......:
; Remarks .......: This function is used internally by _Toast_Show
; ===============================================================================================================================
Func _Toast_Locate($iToast_Width, $iToast_Height)

	Local Const $AW_SLIDE_OUT_BOTTOM = 0x00000004 ; animation direction constants
	Local Const $AW_SLIDE_IN_BOTTOM  = 0x00000008
	Local Const $AW_SLIDE_OUT_RIGHT  = 0x00000001
	Local Const $AW_SLIDE_IN_RIGHT   = 0x00000002
	Local Const $AW_SLIDE_OUT_TOP    = 0x00000008
	Local Const $AW_SLIDE_IN_TOP     = 0x00000004
	Local Const $AW_SLIDE_OUT_LEFT   = 0x00000002
	Local Const $AW_SLIDE_IN_LEFT    = 0x00000001
	Local Const $AW_HIDE             = 0x00010000 ; combine with animation constants

	; Don't try to center the Toast on the Taskbar
	; if the animation type is $AW_CENTER or $AW_BLEND.
    ; These animation types will always be centered
    ; on the desktop/work area.at the end of this function.
	Local $iToast_Taskbar_Pos_Temp = $iToast_Taskbar_Pos
	If $bToast_Animation = $AW_CENTER _
	Or $bToast_Animation = $AW_BLEND Then
		$iToast_Taskbar_Pos_Temp = $END_OF_TASKBAR
	EndIf

    ; Define return array
    Local $aToast_Data[3]
    ; Create struct
    ;Local $tWorkArea = DllStructCreate("struct;long Left;long Top;long Right;long Bottom;endstruct")

	; Set animation type and 'hide' bit - common to all animation types.
	$iToast_Move = $bToast_Animation + $AW_HIDE
	$aToast_Data[2] = $bToast_Animation

    If BitAND($iToast_Taskbar, 8) Then
        ; Docked toolbar
        $iToast_Taskbar -= 8

        #cs
        ; Get monitor info
        Local $aMonitors = _WinAPI_EnumDisplayMonitors()

        $aData = _WinAPI_GetMonitorInfo($aMonitors[$iToast_Taskbar][0])
        Local $aScreenSize =
        Local $aWorkArea[4] = [DllStructGetData($aData[1], 1), _
                            DllStructGetData($aData[1], 2), _
                            DllStructGetData($aData[1], 3), _
                            DllStructGetData($aData[1], 4)]
        ; Determine direction of Toast motion and starting position
        If $aWorkArea[3] <> @DesktopHeight Then
			$iToast_Move    += $AW_SLIDE_OUT_BOTTOM
            $aToast_Data[0]  = @DesktopWidth - $iToast_Width - 10
            $aToast_Data[1]  = $aWorkArea[3] - $iToast_Height - 2
			$aToast_Data[2] += $AW_SLIDE_IN_BOTTOM
			If $iToast_Taskbar_Pos_Temp = $CENTER_OF_TASKBAR Then $aToast_Data[0] /= 2
        ElseIf $aWorkArea[2] <> @DesktopWidth Then
			$iToast_Move    += $AW_SLIDE_OUT_RIGHT
            $aToast_Data[0]  = $aWorkArea[2] - $iToast_Width - 2
            $aToast_Data[1]  = @DesktopHeight - $iToast_Height - 10
			$aToast_Data[2] += $AW_SLIDE_IN_RIGHT
			If $iToast_Taskbar_Pos_Temp = $CENTER_OF_TASKBAR Then $aToast_Data[1] /= 2
        ElseIf $aWorkArea[1] <> 0 Then
			$iToast_Move    += $AW_SLIDE_OUT_TOP
            $aToast_Data[0]  = @DesktopWidth - $iToast_Width - 10
            $aToast_Data[1]  = $aWorkArea[1]
			$aToast_Data[2] += $AW_SLIDE_IN_TOP
			If $iToast_Taskbar_Pos_Temp = $CENTER_OF_TASKBAR Then $aToast_Data[0] = /= 2
        ElseIf $aWorkArea[0] <> 0 Then
			$iToast_Move    += $AW_SLIDE_OUT_LEFT
            $aToast_Data[0]  = $aWorkArea[0]
            $aToast_Data[1]  = @DesktopHeight - $iToast_Height - 10
			$aToast_Data[2] += $AW_SLIDE_IN_LEFT
			If $iToast_Taskbar_Pos_Temp = $CENTER_OF_TASKBAR Then $aToast_Data[1] /= 2
        EndIf
        #ce

    Else
        ; Find taskbar
        Local $iPrevMode = Opt("WinTitleMatchMode", 4)
        Local $aTray_Pos = WinGetPos("[CLASS:Shell_TrayWnd]")
        Opt("WinTitleMatchMode", $iPrevMode)
        ; If error in finding systray
        If Not IsArray($aTray_Pos) Then Return SetError(2, 0, -1)

        ConsoleWrite("Taskbar: " & $aTray_Pos[0] & " - " & $aTray_Pos[1] & " - " & $aTray_Pos[2] & " - " & $aTray_Pos[3] & @CRLF)

        ; Get virtual desktop size
        Local $aRet = DllCall("user32.dll", "int", "GetSystemMetrics", "int", 78) ; $SM_VIRTUALWIDTH
        Local $iVirtualDesktopWidth = $aRet[0]
        $aRet = DllCall("user32.dll", "int", "GetSystemMetrics", "int", 79) ; $SM_VIRTUALHEIGHT
        Local $iVirtualDesktopHeight = $aRet[0]

        ConsoleWrite("Virtual Destop: " & $iVirtualDesktopWidth & " - " & $iVirtualDesktopHeight & @CRLF)

        ; Default values for single monitor
        Local $iLeft = 0, $iRight = @DesktopWidth,  $iTop = 0, $iBottom = @DesktopHeight
        Switch $iToast_Taskbar
            Case 1 ; Secondary monitor to right
                $iLeft = @DesktopWidth
                $iRight = $iVirtualDesktopWidth
            Case 2 ; Secondary monitor below
                $iTop = @DesktopHeight
                $iBottom = $iVirtualDesktopHeight
            Case 3 ; Secondary monitor to left
                $iLeft = @DesktopWidth - $iVirtualDesktopWidth
                $iRight = 0

                ConsoleWrite("L/R: " & $iLeft & " - " & $iRight & @CRLF)

            Case 4 ; Secondary monitor above
                $iTop = @DesktopHeight - $iVirtualDesktopHeight
                $iBottom = 0
        EndSwitch

        ; Determine direction of Toast motion and starting position
        If $aTray_Pos[1] > $iTop Then
			$iToast_Move    += $AW_SLIDE_OUT_BOTTOM
            $aToast_Data[0]  = $iRight - $iToast_Width - 10
            $aToast_Data[1]  = $aTray_Pos[1] - $iToast_Height - 2
			$aToast_Data[2] += $AW_SLIDE_IN_BOTTOM
			If $iToast_Taskbar_Pos_Temp = $CENTER_OF_TASKBAR Then $aToast_Data[0] /= 2
        ElseIf $aTray_Pos[0] > $iLeft Then
			$iToast_Move    += $AW_SLIDE_OUT_RIGHT
            $aToast_Data[0]  = $aTray_Pos[0] - $iToast_Width - 2
            $aToast_Data[1]  = $iBottom - $iToast_Height - 10
			$aToast_Data[2] += $AW_SLIDE_IN_RIGHT
			If $iToast_Taskbar_Pos_Temp = $CENTER_OF_TASKBAR Then $aToast_Data[1] /= 2
        ElseIf $aTray_Pos[2] > $iRight - 70 Then
			$iToast_Move    += $AW_SLIDE_OUT_TOP
            $aToast_Data[0]  = $iRight - $iToast_Width - 10
            $aToast_Data[1]  = $aTray_Pos[1] + $aTray_Pos[3]
			$aToast_Data[2] += $AW_SLIDE_IN_TOP
			If $iToast_Taskbar_Pos_Temp = $CENTER_OF_TASKBAR Then $aToast_Data[0] /= 2
        ElseIf $aTray_Pos[3] >= $iBottom Then
			$iToast_Move    += $AW_SLIDE_OUT_LEFT
            $aToast_Data[0]  = $aTray_Pos[0] + $aTray_Pos[2]
            $aToast_Data[1]  = $iBottom - $iToast_Height - 10
			$aToast_Data[2] += $AW_SLIDE_IN_LEFT
			If $iToast_Taskbar_Pos_Temp = $CENTER_OF_TASKBAR Then $aToast_Data[1] /= 2
        EndIf

        ConsoleWrite("Toast: " & $aToast_Data[0] & " - " & $aToast_Data[1] & " - " & $iToast_Width & " - " & $iToast_Height & @CRLF)

    EndIf

	; Center Toast on screen if animation type is $AW_CENTER or $AW_BLEND
	; The direction flags $AW_SLIDE_OUT... and $AW_SLIDE_IN...
	; are ignored for these animation types.
	If $bToast_Animation = $AW_CENTER _
	Or $bToast_Animation = $AW_BLEND Then
		$aToast_Data[0] /= 2
		$aToast_Data[1] /= 2
	EndIf

    Return $aToast_Data

EndFunc   ;==>_Toast_Locate

; #INTERNAL_USE_ONLY#============================================================================================================
; Name...........: _Toast_Timer_Check
; Description ...: Checks whether Toast has timed out or closure [X] clicked
; Syntax ........:  _Toast_Timer_Check()
; Author ........: Melba23
; Modified.......:
; Remarks .......: This function is used internally by _Toast_Show if the Wait parameter is set to False
; ===============================================================================================================================
Func _Toast_Timer_Check()

    ; Return if timeout not elapsed and [X] not clicked
    If TimerDiff($iToast_Start) < $iToast_Timer And $fToast_Close = False Then Return

    ; Retract slice
    _Toast_Hide()

EndFunc   ;==>_Toast_Timer_Check

; #INTERNAL_USE_ONLY#============================================================================================================
; Name...........: _Toast_WM_EVENTS
; Description ...: Message handler to check if closure [X] clicked
; Author ........: Melba23
; Modified.......:
; Remarks .......: This function is used internally by _Toast_Show if the Wait parameter is set to False
; ===============================================================================================================================
Func _Toast_WM_EVENTS($hWnd, $Msg, $wParam, $lParam)

    #forceref $wParam, $lParam
    If $hWnd = $hToast_Handle Then
        If $Msg = 0x0021 Then ; $WM_MOUSEACTIVATE
            ; Check mouse position
            Local $aPos = GUIGetCursorInfo($hToast_Handle)
            If $aPos[4] = $hToast_Close_X Then $fToast_Close = True
        EndIf
    EndIf
    Return 'GUI_RUNDEFMSG'

EndFunc   ;==>_Toast_WM_EVENTS

; #INTERNAL_USE_ONLY#============================================================================================================
; Name...........: _Toast_GetDefFont
; Description ...: Determine system default MsgBox font and size
; Syntax ........: _Toast_GetDefFont($iData)
; Parameters ....: $iData - 0 = Font point size, 1 = Font name
; Author ........: Melba23
; Modified.......:
; Remarks .......: This function is used internally by _Toast functions
; ===============================================================================================================================
Func _Toast_GetDefFont($iData)

    ; Get default system font data
    Local $tNONCLIENTMETRICS = DllStructCreate("uint;int;int;int;int;int;byte[60];int;int;byte[60];int;int;byte[60];byte[60];byte[60]")
    DllStructSetData($tNONCLIENTMETRICS, 1, DllStructGetSize($tNONCLIENTMETRICS))
    DllCall("user32.dll", "int", "SystemParametersInfo", "int", 41, "int", DllStructGetSize($tNONCLIENTMETRICS), "ptr", DllStructGetPtr($tNONCLIENTMETRICS), "int", 0)
    ; Read font data for MsgBox font
    Local $tLOGFONT = DllStructCreate("long;long;long;long;long;byte;byte;byte;byte;byte;byte;byte;byte;char[32]", DllStructGetPtr($tNONCLIENTMETRICS, 15))
    Switch $iData
        Case 0
            ; Font size as integer
            Return Int((Abs(DllStructGetData($tLOGFONT, 1)) + 1) * .75)
        Case 1
            ; Font name
            Return DllStructGetData($tLOGFONT, 14)
    EndSwitch

EndFunc   ;==>_Toast_GetDefFont

; #INTERNAL_USE_ONLY#============================================================================================================
; Name...........: _Toast_ShowPNG
; Description ...: Set PNG as image
; Syntax ........: _Toast_ShowPNG($sImg)
; Parameters ....: $sImg - Path of image file
; Author ........: UEZ
; Modified.......: Melba23
; Remarks .......:
; ===============================================================================================================================
Func _Toast_ShowPNG($sImg, $iTitle_Height)

    _GDIPlus_Startup()
    Local $hPic = GUICtrlCreatePic("", 10, 10 + $iTitle_Height, 32, 32)
    Local $hBitmap = _GDIPlus_BitmapCreateFromFile($sImg)
    Local $aResult = DllCall($ghGDIPDll, "uint", "GdipCreateBitmapFromScan0", "int", 32, "int", 32, "int", 0, "int", 0x0026200A, "ptr", 0, "int*", 0)
    Local $hBitmap_Resized = $aResult[6]
    Local $hBMP_Ctxt = _GDIPlus_ImageGetGraphicsContext($hBitmap_Resized)
    DllCall($ghGDIPDll, "uint", "GdipSetInterpolationMode", "handle", $hBMP_Ctxt, "int", 7)
    _GDIPlus_GraphicsDrawImageRect($hBMP_Ctxt, $hBitmap, 0, 0, 32, 32)
    Local $hHBitmap = _Toast_BitmapCreateDIB($hBitmap_Resized)
    _WinAPI_DeleteObject(GUICtrlSendMsg($hPic, 0x0172, 0, $hHBitmap))
    _GDIPlus_BitmapDispose($hBitmap)
    _GDIPlus_BitmapDispose($hBitmap_Resized)
    _GDIPlus_GraphicsDispose($hBMP_Ctxt)
    _WinAPI_DeleteObject($hHBitmap)
    _GDIPlus_Shutdown()

EndFunc   ;==>_Toast_ShowPNG

; #INTERNAL_USE_ONLY#============================================================================================================
; Name...........: _Toast_BitmapCreateDIB
; Description ...: Create bitmap
; Syntax ........: _Toast_BitmapCreateDIB($hBitmap)
; Parameters ....: $hBitmap - Handle of bitmap
; Author ........: UEZ
; Modified.......:
; Remarks .......:
; ===============================================================================================================================
Func _Toast_BitmapCreateDIB($hBitmap)

    Local $hRet = 0

    Local $aRet1 = DllCall($ghGDIPDll, "uint", "GdipGetImageDimension", "ptr", $hBitmap, "float*", 0, "float*", 0)
    If (@error) Or ($aRet1[0]) Then Return 0
    Local $tData = _GDIPlus_BitmapLockBits($hBitmap, 0, 0, $aRet1[2], $aRet1[3], $GDIP_ILMREAD, $GDIP_PXF32ARGB)
    Local $pBits = DllStructGetData($tData, "Scan0")
    If Not $pBits Then Return 0
    Local $tBIHDR = DllStructCreate("dword;long;long;ushort;ushort;dword;dword;long;long;dword;dword")
    DllStructSetData($tBIHDR, 1, DllStructGetSize($tBIHDR))
    DllStructSetData($tBIHDR, 2, $aRet1[2])
    DllStructSetData($tBIHDR, 3, $aRet1[3])
    DllStructSetData($tBIHDR, 4, 1)
    DllStructSetData($tBIHDR, 5, 32)
    DllStructSetData($tBIHDR, 6, 0)
    Local $aRet2 = DllCall("gdi32.dll", "ptr", "CreateDIBSection", "hwnd", 0, "ptr", DllStructGetPtr($tBIHDR), "uint", 0, "ptr*", 0, "ptr", 0, "dword", 0)
    If (Not @error) And ($aRet2[0]) Then
        DllCall("gdi32.dll", "dword", "SetBitmapBits", "ptr", $aRet2[0], "dword", $aRet1[2] * $aRet1[3] * 4, "ptr", DllStructGetData($tData, "Scan0"))
        $hRet = $aRet2[0]
    EndIf
    _GDIPlus_BitmapUnlockBits($hBitmap, $tData)
    Return $hRet
EndFunc   ;==>_Toast_BitmapCreateDIB