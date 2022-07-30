; #FUNCTION# =========================================================================================================
; Name...........: _Toast_Show
; Description ...: Shows a slice message from the systray
; Syntax.........: _Toast_Show($nIcon, $sTitle, $sMessage, [$iDelay [, $fWait]])
; Parameters ....: $nIcon    - The icon to show. Default (0) is none. Can be on of the following:
;                            |1 - App window
;                            |2 - Warning triangle
;                            |3 - Question mark
;                            |4 - Stop sign
;                            |5 - Info sign
;                            |6 - Security sign (Vista+)
;                  $sTitle   - Text to display on Title bar
;                  $sMessage - Text to display in Toast body
;                  $iDelay   - The delay in seconds before the Toast retracts or script continues (Default = 0)
;                              If negative, an [X] is added to the title bar. Clicking [X] retracts/continues immediately
;                  $fWait    - True  - Script waits for delay time before continuing and Toast remains visible
;                              False - Script continues and Toast retracts automatically after delay time
; Requirement(s).: v3.3.1.5 or higher - AdlibRegister/Unregister used in _Toast_Show
; Return values .: Success: Returns 2-element array: [Toast width, Toast height]
;                  Failure: Returns -1 and sets @error as follows:
;                           1 = Toast GUI creation failed
;                           2 = Taskbar not found
; Author ........: Melba23, based on some original code by GioVit for the Toast, Prog@ndy and Zedna for the [X]
; Notes .........; Any visible Toast is retracted by a subsequent _Toast_Hide or _Toast_Show, or clicking a visible [X]
; Example........; Yes
;=====================================================================================================================

_Toast_Show(6, "test", "this is a test" & @CRLF & @CRLF & "And yet another test!")
Sleep(10000)

Func _Toast_Show($nIcon, $sTitle, $sMessage, $iDelay = 0, $fWait = True)

    ; Store current GUI mode and set Message mode
    Local $nOldOpt = Opt('GUIOnEventMode', 0)

    ; Retract any Toast already in place
    If $hToast_Handle <> 0 Then _Toast_Hide()

    ; Reset non-reacting Close [X] ControlID
    $hToast_Close_X = 9999

    ; Set default auto-sizing Toast widths
    Local $iToast_Width_max = 500
    Local $iToast_Width_min = 150

    ; Get message label size
    Local $aLabel_Pos = _StringSize($sMessage, $iToast_Font_Size, Default, Default, $sToast_Font_Name, $iToast_Width_max - 20)

    ; Reset text to match rectangle
    $sMessage = $aLabel_Pos[0]

    ;Set line height for this font
    Local $iLine_Height = $aLabel_Pos[1]

    ; Set label size
    Local $iLabelwidth  = $aLabel_Pos[2]
    Local $iLabelheight = $aLabel_Pos[3]

    ; Set Toast size
    Local $iToast_Width = $iLabelwidth + 20
    If $iToast_Width < $iToast_Width_min Then
        ; Increase if below min size
        $iToast_Width = $iToast_Width_min
        $iLabelwidth  = $iToast_Width_min - 20
    EndIf

    ; Set title bar height - with minimum for [X]
    Local $iTitle_Height = 0
    If $sTitle = "" Then
        If $iDelay < 0 Then $iTitle_Height = 5
    Else
        $iTitle_Height = $iLine_Height + 2
        If $iDelay < 0 Then
            If $iTitle_Height < 17 Then $iTitle_Height = 17
        EndIf
    EndIf

    ; Set Toast height as label height + title bar + bottom margin
    Local $iToast_Height = $iLabelheight + $iTitle_Height + 20

    If $nIcon <> 0 Then $iToast_Width += 34

    ; Get Toast starting position and direction
    Local $aToast_Data = _Toast_Locate($iToast_Width, $iToast_Height)

    ; Create Toast slice with $WS_POPUPWINDOW, $WS_EX_TOOLWINDOW style and $WS_EX_TOPMOST extended style
    $hToast_Handle = GUICreate("", $iToast_Width, $iToast_Height, $aToast_Data[0], $aToast_Data[1], 0x80880000, BitOr(0x00000080, 0x00000008))
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
            $hToast_Close_X = GUICtrlCreateLabel("T", $iToast_Width - 18, $iX_YCoord, 17, 17)
                GUICtrlSetFont(-1, 14, Default, Default, "Wingdings 2")
                GUICtrlSetBkColor(-1, -2) ; $GUI_BKCOLOR_TRANSPARENT
                GUICtrlSetColor(-1, $iToast_Header_Col)
            ; Reduce title bar width to allow [X] to activate
            $iTitle_Width -= 18
        EndIf

        ; Create Title label with bold text, centred vertically in case bar is higher than line
        GUICtrlCreateLabel($sTitle, 10, 0, $iTitle_Width, $iTitle_Height, 0x0200) ; $SS_CENTERIMAGE
            GUICtrlSetBkColor(-1,$iToast_Header_BkCol)
            GUICtrlSetColor(-1, $iToast_Header_Col)
            If BitAND($iToast_Style, 4) = 4 Then GUICtrlSetFont(-1, $iToast_Font_Size, 600)

    Else

        If $iDelay < 0 Then
            ; Only need [X]
            $hToast_Close_X = GUICtrlCreateLabel("T", $iToast_Width - 18, 0, 17, 17)
                GUICtrlSetFont(-1, 14, Default, Default, "Wingdings 2")
                GUICtrlSetBkColor(-1, -2) ; $GUI_BKCOLOR_TRANSPARENT
                GUICtrlSetColor(-1, $iToast_Message_Col)
        EndIf

    EndIf

    ; Create Message label
    GUICtrlCreateLabel($sMessage, 10 + (($nIcon <> 0) * 34), 10 + $iTitle_Height, $iLabelwidth, $iLabelheight)
        GUICtrlSetStyle(-1, $iLabel_Style)
        If $iToast_Message_Col <> Default Then GUICtrlSetColor(-1, $iToast_Message_Col)

    If $nIcon >= 6 Then $nIcon = 7
    If $nIcon <> 0 Then GUICtrlCreateIcon("user32.dll", -$nIcon, 2, ($iToast_Height - 32) / 2)

    ; Slide Toast Slice into view from behind systray and activate
    DllCall("user32.dll", "int", "AnimateWindow", "hwnd", $hToast_Handle, "int", 1000, "long", $aToast_Data[2])

    ; Activate Toast without stealing focus
    GUISetState(@SW_SHOWNOACTIVATE, $hToast_Handle)

    ; If script is to pause
    If $fWait = True Then

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

EndFunc ; => _Toast_Show