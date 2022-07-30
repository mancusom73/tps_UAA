#AutoIt3Wrapper_AU3Check_Parameters=-w 1 -w 2 -w 3 -w 4 -w 5 -w 6 -w 7

;#POINT 5786578856 Button Focus
;#POINT 3487878843 Button Focus <_freshbutton>

#include-once

;normal
;if focus then focus-img

; 1, 2, 10 - 25, 27 Oct 2011
; 7-11 Nov 2011
; Feb 13-14 2012
; Mar 8,12 2012
; May 14-17 2012
; glass definition!! on button from progress
; mask script on progress from button 

; zettoCtrlAddLicense( $key, $name, opt $company )
; Add license for propertiary or commercial edition of components.
; Visual Control Studio  $10

; #INDEX# ======================================================================
; Title .........: zetto Graphical Control (on going)
; AutoIt Version : 3.3.6++                                 Graphical ,--^,
; File Version...: 1.0.1                                       _ ___/ /|/
; Written.@......: 5-11, 14 Aug 2011                       ,;'( )__, ) '
;                  22 Aug 2011                            ;;  //   L__.
; Language ......: English                                '   \    /  ' Control
; Description ...: Provide graphical control functions.       ^   ^  AutoIt3
; Author(s) .....: Prasetyo Priadi (prazetto)
; ==============================================================================

; #CONTROLS# ====================================================================================================================
; Button            Class Static
; Progress          Class Static
; Timer             Not Visible
; ===============================================================================================================================

; #NOTE# ========================================================================================================================
; zettoControl transparency not supported on Windows 2000
; ===============================================================================================================================

; #CURRENT# =====================================================================================================================
; Export function of UDF!
; The explanation is on the function itself.
; Please scroll down and i'm sure your brain is enough to understand!
; ===============================================================================================================================

; >> Control Creation
;    zettoCtrlCreateButton
;    zettoCtrlCreateProgress
;    zettoCtrlCreateTimer

; >> Control Common Action
;    zettoCtrlRead
;    zettoCtrlDelete
;    zettoCtrlReduceResource
;    zettoCtrlWindowClear

; >> Control Set Properties
;    zettoCtrlSetColor
;    zettoCtrlSetCursor
;    zettoCtrlSetData
;    zettoCtrlSetGlass
;    zettoCtrlSetGlassMode
;    zettoCtrlSetImage
;    zettoCtrlSetSkin
;    zettoCtrlSetState
;    zettoCtrlSetStyle
;    zettoCtrlSetWindow

; >> Control Get Properties
;    zettoCtrlGetID
;    zettoCtrlGetHandle
;    zettoCtrlGetType

; >> Control Events
;    zettoEventCaller
;    zettoEventHandler

; >> Control Load Skin
;    zettoCtrlLoadButton
;    zettoCtrlLoadProgress

; ===============================================================================================================================

; #INTERNAL_USE_ONLY# ===========================================================================================================
; Use only if you understand
; I willn't accept the question for this. Study by yourself! Okay...
; ===============================================================================================================================

; >> Control Core Graphical Rendering
;    __freshbutton
;    __freshprogress

; >> Control Button State
;    __zettobtn_hover
;    __zettobtn_down
;    __zettobtn_up
;    __zettobtn_leave
;    __zettobtn_combo

; >> Finalization
;    __zetto_FreeResource

; >> Control Indexing
;    __zetto_ButtonNum
;    __zetto_ProgressNum
;    __zetto_TimerNum

; >> Control Skin Indexing
;    __zetto_SkinButtonNum
;    __zetto_SkinProgressNum

; >> Graphical Core
;    __zetto_BitmapMasking
;    __zetto_clonehImage
;    __zetto_FrameworkPNG
;    __zetto_ImageFilter
;    __zetto_SetImage
;    __zettoCreatePNG
;    _drawimage
;    _resizeimage
;    _zetto_FlipHorizontal
;    _zetto_FlipVertical
;    _zetto_SmartZoom

; >> Library Database / Bank Data
;    __zetto_addtolibrary_arr
;    __zetto_addtolibrary_str
;    __zetto_deltolibrary_str

; >> Parser -Number
;    _parser_default
;    _parser_int1var
;    _parser_int4var
;    _parser_rgb2int

; >> Parser -RGB
;    _parser_rgb
;    _zscript_crgb
;    _zscript_cbgr
;    _zscript_cargb
;    _zscript_crgba
;    _zscript_cr
;    _zscript_cg
;    _zscript_cb

; >> Control Script Preloader
;    __zetto_CtrlPreload
;    __zetto_CtrlScriptOpen
;    __zetto_CtrlScriptExist

; >> Misc
;    __zetto_ButtonXWND
;    __zetto_disable
;    __zetto_isinside
;    _GDIPlus_BitmapGetPixel
;    _GDIPlus_MatrixCreate2
;    _glass_effect
;    _isdirectory
;    _progressbar_Timer
;    _zetto_GetParent
;    _zetto_GetTabBkColor
;    SendMessage

; ===============================================================================================================================

; #ALGO# ========================================================================================================================
; zettoCtrlLoad* ~Button    :  Have similar sequence algorithm and only different on loading tag name and data.
;                ~Progress     
; ===============================================================================================================================

; #IMPORTS_UDF# =================================================================================================================
; Resource.au3       --> Resource UDF by Trancexx
; ZLIB.au3           --> ZLIB Compression UDF by Ward
; ReduceMemory.au3   --> Reduce Memory UDF by w_Outer
; BitmapLoader.au3   --> Load Image from Stream by UEZ
; GUICtrlOnHover.au3 --> Control Hovering UDF by G Sandler
; ===============================================================================================================================

; Initialize DLL
Global $DLL_GDI32			= DllOpen('gdi32.dll')
Global $DLL_KERNEL32		= DllOpen('kernel32.dll')
Global $DLL_OLE32			= DllOpen('ole32.dll')
Global $DLL_OLEAUT32		= DllOpen('oleaut32.dll')
Global $DLL_PSAPI			= DllOpen('psapi.dll')
Global $DLL_USER32			= DllOpen('user32.dll')
Global $DLL_UXTHEME			= DllOpen('uxtheme.dll')

Global $NULL

#include <GUIConstantsEx.au3>
#include <StaticConstants.au3>
#include <GDIPlus.au3>
#include <WinAPI.au3>
#Include <Memory.au3>
#include <Array.au3>
#Include <WindowsConstants.au3>

#Include <ZLIB.au3>
#include <GUICtrlOnHover.au3>
#include <ReduceMemory.au3>
#include <prazetto.preloader.au3>
#include <Resource.au3>
#include <BitmapLoader.au3>
#include <zettoScript.au3>

_GDIPlus_Startup()

Opt('MustDeclareVars', 0)

; Text Alignment Constante for Button
Global Const $SS_TEXT_TOPLEFT			= 1
Global Const $SS_TEXT_TOPCENTER			= 2
Global Const $SS_TEXT_TOPRIGHT			= 4
Global Const $SS_TEXT_MIDDLELEFT		= 8
Global Const $SS_TEXT_MIDDLECENTER		= 16
Global Const $SS_TEXT_MIDDLERIGHT		= 32
Global Const $SS_TEXT_BOTTOMLEFT		= 64
Global Const $SS_TEXT_BOTTOMCENTER		= 128
Global Const $SS_TEXT_BOTTOMRIGHT		= 256

; Icon Alignment Constante for button
Global Const $SS_ICON_TOPLEFT			= 512
Global Const $SS_ICON_TOPCENTER			= 1024
Global Const $SS_ICON_TOPRIGHT			= 2048
Global Const $SS_ICON_MIDDLELEFT		= 4096
Global Const $SS_ICON_MIDDLECENTER		= 8192
Global Const $SS_ICON_MIDDLERIGHT		= 16384
Global Const $SS_ICON_BOTTOMLEFT		= 32768
Global Const $SS_ICON_BOTTOMCENTER		= 65536
Global Const $SS_ICON_BOTTOMRIGHT		= 131072

; Control Mode Constante for Button
Global Const $SS_MODE_TOGGLE			= 262144
;new
Global Const $SS_MODE_BGSTATIC			= 0
Global Const $SS_MODE_BGDYNAMIC			= 0
Global Const $SS_MODE_BGTRANSPARENT		= 0

; Control Mode Constante for Progress
Global Const $SS_MODE_BUSY				= 262144

; Control Bar Style for Progress on Busy mode
Global Const $SS_BUSY_LEAD				= 1
Global Const $SS_BUSY_LAG				= 2
Global Const $SS_BUSY_MIDDLE			= 4
Global Const $SS_BUSY_SIDES				= 8
Global Const $SS_BUSY_SIMPLE			= 16

; Control Bar Directional for Progress on Busy mode
Global Const $SS_GOTO_FORWARD			= 32	; --> Bar will be move from left to right
Global Const $SS_GOTO_BACKWARD			= 64	; <--                       right to left
Global Const $SS_GOTO_BIDIRECT			= 128	; <->                       left to right and right to left

; Error return constante
Global Const $ERROR_INVALID = 1
Global Const $ERROR_UNKNOWN = 2
Global Const $ERROR_CNEXIST = 3
Global Const $ERROR_WKSPACE = 4
Global Const $ERROR_FNEXIST = 5
Global Const $ERROR_WNEXIST = 6
Global Const $ERROR_FUNCNOEXIST	= 7
Global Const $ERROR_USER32TIMER	= 8

Global $WIN_2000 = 0
If @OSVersion == 'WIN_2000' Then $WIN_2000 = 1
Global $WIN_FORM = _WinAPI_GetSysColor($COLOR_BTNFACE)

Global $REG_ZETTOCONTROL = RegRead('HKCU\Software\Seagea\Autoit3++\zettoControl','Install Path')
Global $_ZETTO_FRAMEWORK_PNG800X600 = __zetto_FrameworkPNG()
Global $_ZETTO_FRAMEWORK_PNG1024X768

Global $_ZETTO_CTRL_HWND						; Latest Window Handle
Global $_ZETTO_CTRL_LAST						; Latest Created Control
Global $_ZETTO_CTRL_RDLL						; Latest EXE/DLL Resource
Global $_ZETTO_CTRL_EVENT						; Control Event

;Global $_ZETTO_BUTTON_hImage
;Global $_ZETTO_BUTTON_hGraphics

ReduceMemory()
Global $_ZETTO_CTRL_RAM = ProcessGetStats()
If IsArray($_ZETTO_CTRL_RAM) Then $_ZETTO_CTRL_RAM = $_ZETTO_CTRL_RAM[0] + 5242880
;AdlibRegister('zettoCtrlReduceResource')

; Register function on exit
OnAutoItExitRegister('__zetto_FreeResource')

; Button global variable
Global $_ZETTO_BUTTON_DOWN = 1					; Press button status
Global $_ZETTO_BUTTON_LIB		; [array]		; Created Button Database
Global $_ZETTO_BUTTON_SKN		; [array]		; Loaded Skin Button Database
Global $_ZETTO_BUTTON_SKIN		; [array]		; Latest Loaded Skin Button
Global $_ZETTO_BUTTON = 0						; Button handle

Global $COMBO_BUTTON_STATE		                ; Combo Button state
Global $COMBO_BUTTON_INDEX      ; [zctrl]       ; Current applied combo button

; ProgressBar global variable
Global $_ZETTO_PROGRESS_LIB		; [array]		; Created ProgressBar Database
Global $_ZETTO_PROGRESS_SKN		; [array]		; Loaded Skin ProgressBar Database
Global $_ZETTO_PROGRESS_SKIN	; [array]		; Latest Loaded Skin ProgressBar
Global $_ZETTO_PROGRESS_BUSY	; [string]		; ProgressBar busy [mode:on] Database
Global $_ZETTO_PROGRESS_TIME = 90				; Timer value for ProgressBar busy [mode:on]
Global $_ZETTO_PROGRESS_HTIM					; Timer zctrl handle for ProgressBar busy [mode:on]

; Timer global variable
Global $_ZETTO_TIMER_LIB						; Created Timer Database

; This below global constante for human reading purposes only,
; where for remembering placement of value on development, debugging
; and understanding zettoControl UDF. The AutoIt3 lexer needn't this
; constante, so for optimizing speed of this UDF, precompile with
; precompiler.au3 to produced zettoControl.precompiled.au3 file.
; Please don't distribute zettoControl.precompiled.au3 file without
; zettoControl.au3 UDF. So other people will know how to learning.

; Properties description aren't using correct english spelling
; like 'Button Margin of Left' or 'Left Button Margin' instead
; of 'Button Margin Left', lets say read it as properties style
; Button.Margin.Left

#open-precompiler

; Apply to all control ret properties
Global Const $__ZETTO_CTRL_HANDLE__					= 0
Global Const $__ZETTO_CTRL_IDENTIFIER__				= 1

; Button Skin Properties										   Format	Variable		Descriptions
Global Const $__ZETTO_SKIN_BUTTON_PROPERTIES__			= 26
Global Const $__ZETTO_SKIN_BUTTON_NAME__				= 0		; [ Str ]	{ Global }		Button Skin Identifier
Global Const $__ZETTO_SKIN_BUTTON_TEXT_FONT__			= 1		; [ Str ]	{ Derivatable } Button Caption Text Font
Global Const $__ZETTO_SKIN_BUTTON_TEXT_FAMILY__			= 21	; [ Int ]	{ Global } 		Button Caption Text Family				!freshbutton
Global Const $__ZETTO_SKIN_BUTTON_TEXT_SIZE__			= 2		; [ Str ]	{ Derivatable } Button Caption Text Size
Global Const $__ZETTO_SKIN_BUTTON_TEXT_STYLE__			= 3		; [ Int ]	{ Derivatable } Button Caption Text Style
Global Const $__ZETTO_SKIN_BUTTON_TEXT_UNIT__			= 22	; [ Int ]	{ Global } 		Button Caption Text Unit				!freshbutton
Global Const $__ZETTO_SKIN_BUTTON_TEXT_FORMAT__			= 23	; [ Hex ]	{ Global } 		Button Caption Text Format				!freshbutton
Global Const $__ZETTO_SKIN_BUTTON_TEXT_LANGUAGE__		= 24	; [ Hex ]	{ Global } 		Button Caption Text Language			!freshbutton
Global Const $__ZETTO_SKIN_BUTTON_TEXT_COLOR_NORMAL__	= 4		; [ ARGB ]	{ Derivatable } Button Caption Text Color Normal 
Global Const $__ZETTO_SKIN_BUTTON_TEXT_COLOR_HOVER__	= 5		; [ ARGB ]	{ Derivatable } Button Caption Text Color Hover
Global Const $__ZETTO_SKIN_BUTTON_TEXT_COLOR_DOWN__		= 6		; [ ARGB ]	{ Derivatable } Button Caption Text Color Down
Global Const $__ZETTO_SKIN_BUTTON_TEXT_COLOR_FOCUS__	= 19	; [ ARGB ]	{ Derivatable } Button Caption Text Color Focus			!freshbutton
Global Const $__ZETTO_SKIN_BUTTON_TEXT_COLOR_DISABLE__	= 20	; [ ARGB ]	{ Derivatable } Button Caption Text Color Disable		!freshbutton
Global Const $__ZETTO_SKIN_BUTTON_MARGIN_LEFT__			= 7		; [ Int ]	{ Global } 		Button Margin Left
Global Const $__ZETTO_SKIN_BUTTON_MARGIN_RIGHT__		= 8		; [ Int ]	{ Global } 		Button Margin Right
Global Const $__ZETTO_SKIN_BUTTON_MARGIN_TOP__			= 9		; [ Int ]	{ Global } 		Button Margin Top
Global Const $__ZETTO_SKIN_BUTTON_MARGIN_BOTTOM__		= 10	; [ Int ]	{ Global } 		Button Margin Bottom
Global Const $__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__		= 11	; [ Image ]	{ Global } 		Button Image Normal 
Global Const $__ZETTO_SKIN_BUTTON_IMAGE_HOVER__			= 12	; [ Image ]	{ Global } 		Button Image Hover
Global Const $__ZETTO_SKIN_BUTTON_IMAGE_DOWN__			= 13	; [ Image ]	{ Global } 		Button Image Down
Global Const $__ZETTO_SKIN_BUTTON_IMAGE_FOCUS__			= 14	; [ Image ]	{ Global } 		Button Image Focus						!freshbutton
Global Const $__ZETTO_SKIN_BUTTON_IMAGE_DISABLE__		= 15	; [ Image ]	{ Global } 		Button Image Disable					!freshbutton
Global Const $__ZETTO_SKIN_BUTTON_SHOW_FOCUS_TAB__		= 17	; [ Bool ]	{ Derivatable } Show Focus Tab							!freshbutton
Global Const $__ZETTO_SKIN_BUTTON_SHOW_FOCUS_CUR__		= 18	; [ Bool ]	{ Derivatable } Show Focus Cursor						!freshbutton
Global Const $__ZETTO_SKIN_BUTTON_GDIP_FORMAT__			= 16	; [ Int ]	{ Global } 		GDI+ Image Format
Global Const $__ZETTO_SKIN_BUTTON_GDIP_GLASS__			= 25	; [ Bool ]	{ Global } 		Glass state								!freshbutton


; new {

; combo

Global Const $__ZETTO_SKIN_BUTTON_COMBO_SHAPE__					= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_ARROW__					= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_ROTATE__				= 14

Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_LEFT_NORMAL__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_LEFT_HOVER__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_LEFT_DOWNA__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_LEFT_DOWNB__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_LEFT_FOCUS__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_LEFT_DISABLE__	= 14

Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_RIGHT_NORMAL__	= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_RIGHT_HOVER__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_RIGHT_DOWNA__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_RIGHT_DOWNB__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_RIGHT_FOCUS__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_RIGHT_DISABLE__	= 14

Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_TOP_NORMAL__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_TOP_HOVER__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_TOP_DOWNA__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_TOP_DOWNB__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_TOP_FOCUS__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_TOP_DISABLE__		= 14

Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_BOTTOM_NORMAL__	= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_BOTTOM_HOVER__	= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_BOTTOM_DOWNA__	= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_BOTTOM_DOWNB__	= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_BOTTOM_FOCUS__	= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_BOTTOM_DISABLE__	= 14

Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_ARROW_LEFT__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_ARROW_TOP__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_ARROW_BOTTOM__	= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_IMAGE_ARROW_RIGHT__		= 14

Global Const $__ZETTO_SKIN_BUTTON_COMBO_MARGIN_LEFT__			= 14	; { RectWall }	5|5|5|5
Global Const $__ZETTO_SKIN_BUTTON_COMBO_MARGIN_RIGHT__			= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_MARGIN_TOP__			= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_MARGIN_BOTTOM__			= 14

Global Const $__ZETTO_SKIN_BUTTON_COMBO_POS_LEFT__				= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_POS_TOP__				= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_POS_BOTTOM__			= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_POS_RIGHT__				= 14

Global Const $__ZETTO_SKIN_BUTTON_COMBO_ARROW_POS_LEFT__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_ARROW_POS_TOP__			= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_ARROW_POS_BOTTOM__		= 14
Global Const $__ZETTO_SKIN_BUTTON_COMBO_ARROW_POS_RIGHT__		= 14

; }

; Button Properties
Global Const $__ZETTO_BUTTON_PROPERTIES__				= 32
Global Const $__ZETTO_BUTTON_HANDLE__					= 0
Global Const $__ZETTO_BUTTON_WHANDLE__					= 1
Global Const $__ZETTO_BUTTON_CAPTION__					= 2
Global Const $__ZETTO_BUTTON_CAPTION_FONT__				= 3
Global Const $__ZETTO_BUTTON_CAPTION_SIZE__				= 4
Global Const $__ZETTO_BUTTON_CAPTION_STYLE__			= 5
Global Const $__ZETTO_BUTTON_COLOR_NORMAL__				= 6
Global Const $__ZETTO_BUTTON_COLOR_HOVER__				= 7
Global Const $__ZETTO_BUTTON_COLOR_DOWN__				= 8
Global Const $__ZETTO_BUTTON_COLOR_DISABLE__			= 9
Global Const $__ZETTO_BUTTON_NAME_SKIN__				= 10
Global Const $__ZETTO_BUTTON_STATE_DISABLE__			= 11
Global Const $__ZETTO_BUTTON_NAMEID__					= 12
Global Const $__ZETTO_BUTTON_ICON_NORMAL__				= 13
Global Const $__ZETTO_BUTTON_ICON_HOVER__				= 14
Global Const $__ZETTO_BUTTON_ICON_DOWN__				= 15
Global Const $__ZETTO_BUTTON_ICON_DISABLE__				= 16
Global Const $__ZETTO_BUTTON_ICON_MASK__				= 17
Global Const $__ZETTO_BUTTON_ICON_SIZE__				= 18
Global Const $__ZETTO_BUTTON_ICON_BORDER__				= 19
Global Const $__ZETTO_BUTTON_CAPTION_BORDER__			= 20
Global Const $__ZETTO_BUTTON_STYLE__					= 21
Global Const $__ZETTO_BUTTON_MODE__						= 22
Global Const $__ZETTO_BUTTON_TOGGLE__					= 23
Global Const $__ZETTO_BUTTON_SOUND__					= 24
Global Const $__ZETTO_BUTTON_GLASS__					= 25
Global Const $__ZETTO_BUTTON_GLASS_NORMAL__				= 26
Global Const $__ZETTO_BUTTON_GLASS_HOVER__				= 27
Global Const $__ZETTO_BUTTON_GLASS_DISABLE__			= 28
Global Const $__ZETTO_BUTTON_GLASS_DOWN__				= 29
Global Const $__ZETTO_BUTTON_FOCUS__					= 30	; Show tab focus/selected state, Default Inherited from Skin
Global Const $__ZETTO_BUTTON_XWND__						= 31

; Progress Skin Properties
Global Const $__ZETTO_SKIN_PROGRESS_PROPERTIES__	= 38
Global Const $__ZETTO_SKIN_PROGRESS_NAME__			= 0
Global Const $__ZETTO_SKIN_PROGRESS_BACKGROUND__	= 1		; Image{
Global Const $__ZETTO_SKIN_PROGRESS_FOREGROUND__	= 2		
Global Const $__ZETTO_SKIN_PROGRESS_LEAD__			= 3
Global Const $__ZETTO_SKIN_PROGRESS_LAG__			= 4
Global Const $__ZETTO_SKIN_PROGRESS_MIDDLE__		= 5
Global Const $__ZETTO_SKIN_PROGRESS_SIDES__			= 6
Global Const $__ZETTO_SKIN_PROGRESS_SIMPLE__		= 7		; }
Global Const $__ZETTO_SKIN_PROGRESS_FONT__			= 8		; Font {
Global Const $__ZETTO_SKIN_PROGRESS_FAMILY__		= 9
Global Const $__ZETTO_SKIN_PROGRESS_SIZE__			= 10
Global Const $__ZETTO_SKIN_PROGRESS_STYLE__			= 11
Global Const $__ZETTO_SKIN_PROGRESS_UNIT__			= 12	; }
Global Const $__ZETTO_SKIN_PROGRESS_FORM__			= 13	; }
Global Const $__ZETTO_SKIN_PROGRESS_LANG__			= 14	; }
Global Const $__ZETTO_SKIN_PROGRESS_COLOR_ENABLE__	= 15	; Font Color {
Global Const $__ZETTO_SKIN_PROGRESS_COLOR_DISABLE__	= 16
Global Const $__ZETTO_SKIN_PROGRESS_COLOR_MASK__	= 17	; }
Global Const $__ZETTO_SKIN_PROGRESS_BLEFT__			= 18	; Background Relative Zooming {
Global Const $__ZETTO_SKIN_PROGRESS_BRIGHT__		= 19
Global Const $__ZETTO_SKIN_PROGRESS_BUP__			= 20
Global Const $__ZETTO_SKIN_PROGRESS_BBOTTOM__		= 21	; }
Global Const $__ZETTO_SKIN_PROGRESS_FLEFT__			= 22	; Foreground Relative Zooming {
Global Const $__ZETTO_SKIN_PROGRESS_FRIGHT__		= 23
Global Const $__ZETTO_SKIN_PROGRESS_FUP__			= 24
Global Const $__ZETTO_SKIN_PROGRESS_FBOTTOM__		= 25	; }
Global Const $__ZETTO_SKIN_PROGRESS_FOREW__			= 26	; Relative Foreground Width
Global Const $__ZETTO_SKIN_PROGRESS_FOREH__			= 27	;                     Height
Global Const $__ZETTO_SKIN_PROGRESS_SPACE__			= 28	; Space Between Foreground
Global Const $__ZETTO_SKIN_PROGRESS_POSLEFT__		= 29	; Foreground Position {
Global Const $__ZETTO_SKIN_PROGRESS_POSRIGHT__		= 30
Global Const $__ZETTO_SKIN_PROGRESS_POSUP__			= 31
Global Const $__ZETTO_SKIN_PROGRESS_POSBOTTOM__		= 32	; }
Global Const $__ZETTO_SKIN_PROGRESS_ZOOM__			= 33
Global Const $__ZETTO_SKIN_PROGRESS_METHOD__		= 34
Global Const $__ZETTO_SKIN_PROGRESS_FORMAT__		= 35
Global Const $__ZETTO_SKIN_PROGRESS_GLASS__			= 36
Global Const $__ZETTO_SKIN_PROGRESS_GRAYED__		= 37

; Progress Skin Const Value
Global Const $__ZETTO_ZOOM_SIMPLE__					= 1
Global Const $__ZETTO_ZOOM_SMART__					= 2
Global Const $__ZETTO_METHOD_BLOCK__				= 1
Global Const $__ZETTO_METHOD_LINEAR__				= 2


; Progress Properties
Global Const $__ZETTO_PROGRESS_PROPERTIES__				= 15
Global Const $__ZETTO_PROGRESS_HANDLE__					= 0
Global Const $__ZETTO_PROGRESS_WHANDLE__				= 1
Global Const $__ZETTO_PROGRESS_VALUE__					= 2
Global Const $__ZETTO_PROGRESS_MIN__					= 3
Global Const $__ZETTO_PROGRESS_MAX__					= 4
Global Const $__ZETTO_PROGRESS_DISABLE__				= 5
Global Const $__ZETTO_PROGRESS_CAPTION__				= 6
Global Const $__ZETTO_PROGRESS_NAME_SKIN__				= 7
Global Const $__ZETTO_PROGRESS_STYLE__					= 8
Global Const $__ZETTO_PROGRESS_BUSY__					= 9		; Busy run or not
Global Const $__ZETTO_PROGRESS_BUSYPOS__				= 10	; Busy position value
Global Const $__ZETTO_PROGRESS_BIDIRECT__				= 11	; Busy biderectional status
Global Const $__ZETTO_PROGRESS_GLASS__					= 12
Global Const $__ZETTO_PROGRESS_GLASS_NORMAL__			= 13
Global Const $__ZETTO_PROGRESS_GLASS_DISABLE__			= 14

; Timer Properties
Global Const $__ZETTO_TIMER_PROPERTIES	= 5
Global Const $__ZETTO_TIMER_NAME		= 0
Global Const $__ZETTO_TIMER_ENABLE		= 1
Global Const $__ZETTO_TIMER_INTERVAL	= 2
Global Const $__ZETTO_TIMER_CALLBACK	= 3
Global Const $__ZETTO_TIMER_TIMER		= 4

#close-precompiler

Func zettoCtrlReduceResource()
	Local $ram = ProcessGetStats()
	If Not IsArray($ram) Then Return
	If $ram[0] > $_ZETTO_CTRL_RAM Then ReduceMemory()
EndFunc

; $nametime	: Timer Name
; $interval : Interval of timer
; $enable   : Timer Status
; @error	$ERROR_FUNCNOEXIST
Func zettoCtrlCreateTimer($nametime,$interval=250,$enable=True)
	Local $callback=-1, $timer=-1, $prop[$__ZETTO_TIMER_PROPERTIES]
	If $enable Then
		$callback = DllCallbackRegister($nametime & '_Timer', 'int', '')
		If $callback == 0 Then Return SetError($ERROR_FUNCNOEXIST, 0, 0)
		EndIf
	If $enable And $interval <> 0 Then
		$timer = DllCall($DLL_USER32, 'uint', 'SetTimer', 'hwnd', 0, 'uint', 0, 'int', $interval, 'ptr', DllCallbackGetPtr($callback))
		If Not IsArray($timer) Then Return SetError($ERROR_USER32TIMER, 0, 0)
		$timer = $timer[0]
		EndIf
	
	$prop[$__ZETTO_TIMER_INTERVAL]		= $interval
	$prop[$__ZETTO_TIMER_CALLBACK]		= $callback
	$prop[$__ZETTO_TIMER_ENABLE]		= $enable
	$prop[$__ZETTO_TIMER_TIMER]			= $timer
	$prop[$__ZETTO_TIMER_NAME]			= $nametime
	
	__zetto_addtolibrary_arr($_ZETTO_TIMER_LIB, $prop)
	
	Local $ret[2]
	
	$ret[$__ZETTO_CTRL_HANDLE__] = $nametime
	$ret[$__ZETTO_CTRL_IDENTIFIER__] = 'ZTMR'
	
	$_ZETTO_CTRL_LAST = $ret
	
	Return $ret
EndFunc

; get index number of created zetto timer from zctrl
Func __zetto_TimerNum(ByRef $zctrl)
	Local $prop, $ret = -1
	For $loop = 0 To UBound($_ZETTO_TIMER_LIB)-1
		$prop = $_ZETTO_TIMER_LIB[$loop]
		If $zctrl[$__ZETTO_CTRL_HANDLE__] == $prop[$__ZETTO_TIMER_NAME] Then
			$ret = $loop
			ExitLoop
			EndIf
		Next
	Return $ret
EndFunc

; Free Resource used by zettoControl UDF
Func __zetto_FreeResource()
	Local $prop, $zctrl[2]
	$zctrl[$__ZETTO_CTRL_IDENTIFIER__] = 'ZTMR'
	For $loop = 0 To UBound($_ZETTO_TIMER_LIB)-1
		$prop = $_ZETTO_TIMER_LIB[$loop]
		If $prop[$__ZETTO_TIMER_ENABLE] == True Then
			$zctrl[$__ZETTO_CTRL_HANDLE__] = $prop[$__ZETTO_TIMER_NAME]
			zettoCtrlSetData($zctrl, False)
			EndIf
		Next
	_GDIPlus_ImageDispose($_ZETTO_FRAMEWORK_PNG800X600)
	_GDIPlus_ImageDispose($_ZETTO_FRAMEWORK_PNG1024X768)
	
	;Restore pFunc point
	For $loop = 0 To UBound($_ZETTO_BUTTON_LIB)-1
		$prop = $_ZETTO_BUTTON_LIB[$loop]
		; $GWL_WNDPROC = 0xFFFFFFFC
		;_WinAPI_SetWindowLong($prop[$__ZETTO_BUTTON_XWND__], 0xFFFFFFFC, $_ZETTO_BTN_PROCWNL)
		Next

	;_GDIPlus_Shutdown()
EndFunc

Func zettoCtrlLoadProgress($nameskin,$resource=0)
	Local $script, $file, $folder, $skin[$__ZETTO_SKIN_PROGRESS_PROPERTIES__]
	Local $sequendir, $sequenres

	; Select resource
	Switch StringUpper($resource)
		Case '0'
			If @Compiled Then
				$sequenres = 1
				$resource = @ScriptFullPath
				Else
				$sequendir = 1
				$resource = @ScriptDir
				EndIf
		Case '{EXTERNAL}'
			$sequendir = 1
			$resource = @ScriptDir
		Case Else
			If FileExists($resource) Then
			 If _isdirectory($resource) Then
				$sequendir = 1
				Else
				$sequenres = 1
				EndIf
			EndIf
		EndSwitch
	
	; Sequence search startegy
	If $sequendir Then
		If __zetto_CtrlScriptExist($resource &  '\Progress\' & $nameskin) Then
			$script = __zetto_CtrlScriptOpen($resource &  '\Progress\' & $nameskin)
			$folder = $resource &  '\Progress\' & $nameskin
		ElseIf __zetto_CtrlScriptExist($resource) Then
			$script = __zetto_CtrlScriptOpen($resource)
			$folder = $resource
		ElseIf __zetto_CtrlScriptExist($resource &  '\' & $nameskin) Then
			$script = __zetto_CtrlScriptOpen($resource &  '\' & $nameskin)
			$folder = $resource &  '\' & $nameskin
		ElseIf StringLen($script) == 0 Then
			If __zetto_CtrlScriptExist($REG_ZETTOCONTROL & '\Progress\' & $nameskin) Then
				$script = __zetto_CtrlScriptOpen($REG_ZETTOCONTROL & '\Progress\' & $nameskin)
				$folder = $REG_ZETTOCONTROL & '\Progress\' & $nameskin
				EndIf
			EndIf
			If StringLen($script) <> 0 Then
				$folder &= '\'
				EndIf
		EndIf

	; Open resource from windows image file (exe, dll, ocx and etc)
	If $sequenres Then
		Local $nameres = 'ZBAR_' & StringUpper(StringReplace($nameskin,' ','_',0))
		$folder = $resource & @CR & $nameres
		$script = _ResourceGetAsRaw($nameres,'ZINIT',1033,$resource)
		If StringLen($script) <> 0 Then $_ZETTO_CTRL_RDLL = $resource
		EndIf

	If StringLen($script) == 0 Then Return SetError(1, 0, 0)
	$script = _zettoscript_Optimize($script)
	
	; Refresh on skin reloading
	Local $numskin = __zetto_SkinProgressNum($nameskin)
	If $numskin <> -1 Then _ArrayDelete($_ZETTO_PROGRESS_SKN,$numskin)

	; Skin preloader
	Local $propImage, $propCaption, $propCaptionColor, $propBackground, $propForeground, $propSize, $propPosition, $propSettings, $propBusy

	$propImage 			= _zettoscript_GetParam($script,'.Progress::Image')
	$propCaption		= _zettoscript_GetParam($script,'.Progress::Caption')
	$propCaptionColor	= _zettoscript_GetParamChild($propCaption,'.color')
	$propBackground		= _zettoscript_GetParam($script,'.Progress::Background')
	$propForeground		= _zettoscript_GetParam($script,'.Progress::Foreground')
	$propSize			= _zettoscript_GetParamChild($propForeground,'.size')
	$propPosition		= _zettoscript_GetParam($script,'.Progress::Position')
	$propSettings		= _zettoscript_GetParam($script,'.Progress::Settings')
		
	$skin[$__ZETTO_SKIN_PROGRESS_NAME__]			= $nameskin
	
	$skin[$__ZETTO_SKIN_PROGRESS_BACKGROUND__]	 = Load_BMP_From_Mem(__zetto_CtrlPreload($propImage, '.background',$folder))
	$skin[$__ZETTO_SKIN_PROGRESS_FOREGROUND__]	 = Load_BMP_From_Mem(__zetto_CtrlPreload($propImage, '.foreground',$folder))
	If _zettoscript_Exist($propImage, '.busy') Then
		$propBusy = _zettoscript_GetParamChild($propImage, '.busy')
		$skin[$__ZETTO_SKIN_PROGRESS_LEAD__]	= Load_BMP_From_Mem(__zetto_CtrlPreload($propBusy, '.lead',$folder))
		$skin[$__ZETTO_SKIN_PROGRESS_LAG__]		= Load_BMP_From_Mem(__zetto_CtrlPreload($propBusy, '.lag',$folder))
		$skin[$__ZETTO_SKIN_PROGRESS_MIDDLE__]	= Load_BMP_From_Mem(__zetto_CtrlPreload($propBusy, '.middle',$folder))
		$skin[$__ZETTO_SKIN_PROGRESS_SIDES__]	= Load_BMP_From_Mem(__zetto_CtrlPreload($propBusy, '.sides',$folder))
		$skin[$__ZETTO_SKIN_PROGRESS_SIMPLE__]	= Load_BMP_From_Mem(__zetto_CtrlPreload($propBusy, '.simple',$folder))
		EndIf

	$skin[$__ZETTO_SKIN_PROGRESS_FONT__] 		= _zettoscript_GetValue($propCaption, '.font')
	$skin[$__ZETTO_SKIN_PROGRESS_FAMILY__] 		= _zettoscript_GetValue($propCaption, '.family')
	$skin[$__ZETTO_SKIN_PROGRESS_SIZE__] 		= _zettoscript_GetValue($propCaption, '.size')
	$skin[$__ZETTO_SKIN_PROGRESS_STYLE__] 		= _zettoscript_GetValue($propCaption, '.style')
	$skin[$__ZETTO_SKIN_PROGRESS_UNIT__] 		= _zettoscript_GetValue($propCaption, '.unit')
	$skin[$__ZETTO_SKIN_PROGRESS_FORM__] 		= _zettoscript_GetValue($propCaption, '.format')
	$skin[$__ZETTO_SKIN_PROGRESS_LANG__] 		= _zettoscript_GetValue($propCaption, '.language')

	$skin[$__ZETTO_SKIN_PROGRESS_COLOR_ENABLE__]	= _zettoscript_GetValue($propCaptionColor, '.enable')
	$skin[$__ZETTO_SKIN_PROGRESS_COLOR_DISABLE__]	= _zettoscript_GetValue($propCaptionColor, '.disable')
	$skin[$__ZETTO_SKIN_PROGRESS_COLOR_MASK__]	= _zettoscript_GetValue($propCaptionColor, '.mask')	; }

	$skin[$__ZETTO_SKIN_PROGRESS_BLEFT__]		= _zettoscript_GetValue($propBackground, '.left')
	$skin[$__ZETTO_SKIN_PROGRESS_BRIGHT__]		= _zettoscript_GetValue($propBackground, '.right')
	$skin[$__ZETTO_SKIN_PROGRESS_BUP__]			= _zettoscript_GetValue($propBackground, '.up')
	$skin[$__ZETTO_SKIN_PROGRESS_BBOTTOM__]		= _zettoscript_GetValue($propBackground, '.bottom')
	
	$skin[$__ZETTO_SKIN_PROGRESS_FLEFT__]		= _zettoscript_GetValue($propForeground, '.left')
	$skin[$__ZETTO_SKIN_PROGRESS_FRIGHT__]		= _zettoscript_GetValue($propForeground, '.right')
	$skin[$__ZETTO_SKIN_PROGRESS_FUP__]			= _zettoscript_GetValue($propForeground, '.up')
	$skin[$__ZETTO_SKIN_PROGRESS_FBOTTOM__]		= _zettoscript_GetValue($propForeground, '.bottom')

	$skin[$__ZETTO_SKIN_PROGRESS_FOREW__]		= _zettoscript_GetValue($propSize, '.width')
	$skin[$__ZETTO_SKIN_PROGRESS_FOREH__]		= _zettoscript_GetValue($propSize, '.height')
	
	$skin[$__ZETTO_SKIN_PROGRESS_SPACE__]		= _zettoscript_GetValue($propForeground, '.space')

	$skin[$__ZETTO_SKIN_PROGRESS_POSLEFT__]		= _zettoscript_GetValue($propPosition, '.left')
	$skin[$__ZETTO_SKIN_PROGRESS_POSRIGHT__]	= _zettoscript_GetValue($propPosition, '.right')
	$skin[$__ZETTO_SKIN_PROGRESS_POSUP__]		= _zettoscript_GetValue($propPosition, '.up')
	$skin[$__ZETTO_SKIN_PROGRESS_POSBOTTOM__]	= _zettoscript_GetValue($propPosition, '.bottom')

	$skin[$__ZETTO_SKIN_PROGRESS_ZOOM__]		= _zettoscript_GetValue($propSettings, '.zoom')
	$skin[$__ZETTO_SKIN_PROGRESS_METHOD__]		= _zettoscript_GetValue($propSettings, '.method')
	$skin[$__ZETTO_SKIN_PROGRESS_FORMAT__]		= _zettoscript_GetValue($propSettings, '.format')

	$skin[$__ZETTO_SKIN_PROGRESS_GLASS__]		= _zettoscript_GetValue($propSettings, '.glass')
	$skin[$__ZETTO_SKIN_PROGRESS_GRAYED__]		= _zettoscript_GetValue($propSettings, '.grayed')

	; Set default value
	If isNullVar($skin[$__ZETTO_SKIN_PROGRESS_UNIT__]) Then $skin[$__ZETTO_SKIN_PROGRESS_UNIT__] = 3
	If Not isFontExist($skin[$__ZETTO_SKIN_PROGRESS_FONT__]) Then
		zetto_AlterFont($skin[$__ZETTO_SKIN_PROGRESS_FONT__],$skin[$__ZETTO_SKIN_PROGRESS_SIZE__],$skin[$__ZETTO_SKIN_PROGRESS_UNIT__],$skin[$__ZETTO_SKIN_PROGRESS_FAMILY__])
		EndIf
	
	; Constante set
	Switch $skin[$__ZETTO_SKIN_PROGRESS_ZOOM__]
		Case 'SIMPLE'
		$skin[$__ZETTO_SKIN_PROGRESS_ZOOM__] = $__ZETTO_ZOOM_SIMPLE__
		Case 'SMART'
		$skin[$__ZETTO_SKIN_PROGRESS_ZOOM__] = $__ZETTO_ZOOM_SMART__
		EndSwitch
	Switch $skin[$__ZETTO_SKIN_PROGRESS_METHOD__]
		Case 'LINEAR'
		$skin[$__ZETTO_SKIN_PROGRESS_METHOD__] = $__ZETTO_METHOD_LINEAR__
		Case 'BLOCK'
		$skin[$__ZETTO_SKIN_PROGRESS_METHOD__] = $__ZETTO_METHOD_BLOCK__
		EndSwitch

	If isNullVar($skin[$__ZETTO_SKIN_PROGRESS_FORMAT__]) Then $skin[$__ZETTO_SKIN_PROGRESS_FORMAT__] = 0x0026200A
	__zetto_Format2Val($skin[$__ZETTO_SKIN_PROGRESS_FORMAT__])
	If isNullVar($skin[$__ZETTO_SKIN_PROGRESS_GRAYED__]) Then $skin[$__ZETTO_SKIN_PROGRESS_GRAYED__] = 'NaN'

	__zetto_addtolibrary_arr($_ZETTO_PROGRESS_SKN, $skin)
	$_ZETTO_PROGRESS_SKIN = $nameskin
	Return 1
EndFunc

Func __zetto_CtrlScriptOpen($folder)
	Local $search, $file, $data, $wdir = @WorkingDir
	FileChangeDir($folder)
	$search = FileFindFirstFile("*.def")
	If $search = -1 Then
		FileChangeDir($wdir)
		Return SetError(1, 0, 0)
		EndIf
	$file = FileFindNextFile($search)
	If @error Then
		FileChangeDir($wdir)
		Return SetError(2, 0, 0)
		EndIf
	$data = FileRead($file)
	If @error Then
		FileChangeDir($wdir)
		Return SetError(2, 0, 0)
		EndIf
	FileClose($search)
	FileChangeDir($wdir)
	Return $data
EndFunc

Func __zetto_CtrlScriptExist($folder)
	Local $search, $file, $data, $wdir = @WorkingDir
	FileChangeDir($folder)
	$search = FileFindFirstFile("*.def")
	If $search = -1 Then
		FileChangeDir($wdir)
		Return SetError(1, 0, 0)
		EndIf
	FileClose($search)
	FileChangeDir($wdir)
	Return 1
EndFunc

Func __zetto_CtrlPreload(ByRef $data, $var, $folder='')
	Local $file, $array
	If StringInStr($folder,@CR,1) <> 0 Then
		$array = StringSplit($folder,@CR,2)
		Return _ResourceGetAsRaw($array[1],$var,1033,$array[0],1)
		Else
		$file = $folder & _zettoscript_GetValue($data, $var)
		If FileExists($file) Then
			Return FileRead($file)
			Else
			ConsoleWrite('File '& $file & ' for ' & $var & ' skin are not found! Please check again template definition or file is exist?'&@CR)
			EndIf
		EndIf
EndFunc

Func zettoCtrlCreateProgress($value,$min,$max,$text,$left,$top,$width=150,$height=17,$nameskin=-1,$sstyle=16,$glass=-1)
	Local $prop[$__ZETTO_PROGRESS_PROPERTIES__], $skin
	
	If $sstyle=-1 Or $sstyle=0 Then $sstyle=16
	
	If $nameskin == -1 Or $nameskin == '' Then
		$nameskin = $_ZETTO_PROGRESS_SKIN
		Else
		If __zetto_SkinProgressNum($nameskin) == -1 Then
			If Not zettoCtrlLoadProgress($nameskin) Then
				zettoCtrlLoadProgress($nameskin,$_ZETTO_CTRL_RDLL)
				EndIf
			Else
			$_ZETTO_PROGRESS_SKIN = $nameskin
			EndIf
		EndIf
	
	Local $chnd = GUICtrlCreatePic('',$left,$top,$width,$height,$SS_NOTIFY)
	
	$skin = __zetto_SkinProgressNum($nameskin)
	If $skin == -1 Then Return
	$skin = $_ZETTO_PROGRESS_SKN[$skin]
	
	If $glass = -1 Then $glass = $skin[$__ZETTO_SKIN_PROGRESS_GLASS__]
	
	$prop[$__ZETTO_PROGRESS_HANDLE__]			= $chnd
	$prop[$__ZETTO_PROGRESS_WHANDLE__]			= _zetto_GetParent(GUICtrlGetHandle($chnd))
	$prop[$__ZETTO_PROGRESS_VALUE__]			= $value
	$prop[$__ZETTO_PROGRESS_MIN__]				= $min
	$prop[$__ZETTO_PROGRESS_MAX__]				= $max
	$prop[$__ZETTO_PROGRESS_DISABLE__]			= 0
	$prop[$__ZETTO_PROGRESS_CAPTION__]			= $text
	$prop[$__ZETTO_PROGRESS_NAME_SKIN__]		= $nameskin
	$prop[$__ZETTO_PROGRESS_STYLE__]			= $sstyle
	$prop[$__ZETTO_PROGRESS_BUSY__]				= 0
	$prop[$__ZETTO_PROGRESS_BUSYPOS__]			= 'NaN'	; Not an Number
	$prop[$__ZETTO_PROGRESS_BIDIRECT__]			= 1
	$prop[$__ZETTO_PROGRESS_GLASS__]			= $glass
	$prop[$__ZETTO_PROGRESS_GLASS_NORMAL__]		= 'NaN'
	$prop[$__ZETTO_PROGRESS_GLASS_DISABLE__	]	= 'NaN'
	
	__zetto_addtolibrary_arr($_ZETTO_PROGRESS_LIB, $prop)
	__freshprogress($prop)
	
	If BitAND($sstyle,$SS_MODE_BUSY) Then
		If $_ZETTO_PROGRESS_BUSY == '' Then
			If IsArray($_ZETTO_PROGRESS_HTIM) Then
				zettoCtrlSetData($_ZETTO_PROGRESS_HTIM, True)
				Else
				$_ZETTO_PROGRESS_HTIM = zettoCtrlCreateTimer('_progressbar',$_ZETTO_PROGRESS_TIME,True)
				EndIf
			EndIf
		;__zetto_addtolibrary_str($_ZETTO_PROGRESS_BUSY, $chnd)
		EndIf
	
	Local $ret[2]

	$ret[$__ZETTO_CTRL_HANDLE__] = $chnd
	$ret[$__ZETTO_CTRL_IDENTIFIER__] = 'ZBAR'
	
	$_ZETTO_CTRL_LAST = $ret
	$_ZETTO_CTRL_HWND = $prop[$__ZETTO_PROGRESS_WHANDLE__]
	
	Return $ret
EndFunc

Func _progressbar_Timer()
	If $_ZETTO_PROGRESS_BUSY == '' Then
		zettoCtrlSetData($_ZETTO_PROGRESS_HTIM, False)
		Return
		EndIf
	Local $busy, $num
	$busy = StringSplit($_ZETTO_PROGRESS_BUSY,'|',2)
	For $loop = 0 To UBound($busy)-1
		$num = __zetto_ProgressNum($busy[$loop])
		If $num <> -1 Then __freshprogress($_ZETTO_PROGRESS_LIB[$num])
		Next
EndFunc

; add to library for string
Func __zetto_addtolibrary_str(ByRef $library, ByRef $value)
	
	If $library == '' Then
		$library = $value
		Return 1
		EndIf
	
	Local $arr, $fnd, $val = String($value)
	
	$arr = StringSplit($library,'|',2)
	For $loop = 0 To UBound($arr)-1
		If $arr[$loop] == $val Then
			$fnd = 1
			ExitLoop
			EndIf
		Next
	
	If Not $fnd Then $library &= '|' & $value
EndFunc

; del in library for string
Func __zetto_deltolibrary_str(ByRef $library, ByRef $value)
	If $library == '' Then Return
	Local $arr, $num, $val = String($value)
	$arr = StringSplit($library,'|',2)
	$library = ''
	For $loop = 0 To UBound($arr)-1
		If $arr[$loop] <> $val Then
			If $library == '' Then
				$library = $arr[$loop]
				Else
				$library &= '|' & $arr[$loop]
				EndIf
			EndIf
		Next
EndFunc


; get loaded skin progress index from nameskin
Func __zetto_SkinProgressNum($nameskin)
	Local $ret = -1
	If IsArray($_ZETTO_PROGRESS_SKN) Then
		Local $array
		For $loop = 0 To UBound($_ZETTO_PROGRESS_SKN) -1
			$array = $_ZETTO_PROGRESS_SKN[$loop]
			If $array[$__ZETTO_SKIN_PROGRESS_NAME__] = $nameskin Then
				$ret = $loop
				ExitLoop
				EndIf
			Next
		EndIf
	Return $ret
EndFunc

; get number of created zetto button from zctrl
Func __zetto_ProgressNum(ByRef $handle)
	Local $array, $ret = -1
	For $loop = 0 To UBound($_ZETTO_PROGRESS_LIB)-1
		$array = $_ZETTO_PROGRESS_LIB[$loop]
		If $array[$__ZETTO_PROGRESS_HANDLE__] == $handle Then
			$ret = $loop
			ExitLoop
			EndIf
		Next
	Return $ret
EndFunc

Func __zetto_clonehImage(ByRef $hImage)
	Local $ret = DllCall($ghGDIPDll,"int","GdipCloneImage", "int", $hImage, "int*",0)
    If @error Or $ret[0] Then Return SetError(1,0,0)
	Return $ret[2]
EndFunc   ;==>_WinAPI_CopyImage

Func __freshprogress($prop)
	
	; Inspect control
	Local $chnd, $size
	
	$chnd = GUICtrlGetHandle($prop[$__ZETTO_PROGRESS_HANDLE__])
	$size = ControlGetPos($chnd,'','')
	If Not IsArray($size) Then Return

	; Inspect Skin
	Local $skin = __zetto_SkinProgressNum($prop[$__ZETTO_PROGRESS_NAME_SKIN__])
	If $skin == -1 Then Return
	$skin = $_ZETTO_PROGRESS_SKN[$skin]

	Local $tRect, $format, $fcolor

	$tRect = DllStructCreate($tagRECT)

	DllStructSetData($tRect, 1, $size[0])
	DllStructSetData($tRect, 2, $size[1])
	DllStructSetData($tRect, 3, $size[0] + $size[2])
	DllStructSetData($tRect, 4, $size[1] + $size[3])

	$format = $skin[$__ZETTO_SKIN_PROGRESS_FORMAT__]

	Local $hBitmap, $hImageProgress, $hImageBlock, $hGraphic

	$hImageProgress = __zetto_clonehImage($skin[$__ZETTO_SKIN_PROGRESS_BACKGROUND__])
	If @error Then Return

	_zetto_SmartZoom($hImageProgress, $size[2], $size[3], $skin[$__ZETTO_SKIN_PROGRESS_BLEFT__], $skin[$__ZETTO_SKIN_PROGRESS_BRIGHT__],$skin[$__ZETTO_SKIN_PROGRESS_BUP__],$skin[$__ZETTO_SKIN_PROGRESS_BBOTTOM__],$format,1)
	$hGraphic = _GDIPlus_ImageGetGraphicsContext($hImageProgress)

	; progressBar
	Local $bar_kiri = $skin[$__ZETTO_SKIN_PROGRESS_POSLEFT__]
	Local $bar_kanan = $skin[$__ZETTO_SKIN_PROGRESS_POSRIGHT__]
	Local $bar_atas = $skin[$__ZETTO_SKIN_PROGRESS_POSUP__]
	Local $bar_bawah = $skin[$__ZETTO_SKIN_PROGRESS_POSBOTTOM__]
	
	Local $PG_BUSY = BitAND($prop[$__ZETTO_PROGRESS_STYLE__],$SS_MODE_BUSY)
	
	If $PG_BUSY Then
		Local $hImageBusy, $s
		Local $s_busy_lead, $s_busy_lag, $s_busy_middle, $s_busy_sides, $s_busy_simple
	
		$s_busy_lead = BitAND($prop[$__ZETTO_PROGRESS_STYLE__],$SS_BUSY_LEAD)
		$s_busy_lag = BitAND($prop[$__ZETTO_PROGRESS_STYLE__],$SS_BUSY_LAG)
		$s_busy_middle = BitAND($prop[$__ZETTO_PROGRESS_STYLE__],$SS_BUSY_MIDDLE)
		$s_busy_sides = BitAND($prop[$__ZETTO_PROGRESS_STYLE__],$SS_BUSY_SIDES)
		$s_busy_simple = BitAND($prop[$__ZETTO_PROGRESS_STYLE__],$SS_BUSY_SIMPLE)
	
		Local $s_goto_forward, $s_goto_backward, $s_goto_bidirect

		$s_goto_forward = BitAND($prop[$__ZETTO_PROGRESS_STYLE__],$SS_GOTO_FORWARD)
		$s_goto_backward = BitAND($prop[$__ZETTO_PROGRESS_STYLE__],$SS_GOTO_BACKWARD)
		$s_goto_bidirect = BitAND($prop[$__ZETTO_PROGRESS_STYLE__],$SS_GOTO_BIDIRECT)
		
		; If directional not present set predefault
		If Not BitOR($s_goto_forward,$s_goto_backward,$s_goto_bidirect) Then
			Select
				Case $s_busy_lead
					$s_goto_forward = 1
				Case $s_busy_lag
					$s_goto_backward = 1
				Case $s_busy_middle
					$s_goto_bidirect = 1
				Case $s_busy_sides
					$s_goto_bidirect = 1
				Case $s_busy_simple
					$s_goto_bidirect = 1
				Case Else
					$s_goto_forward = 1
				EndSelect
			EndIf

		; If style not present set predefault
		If Not BitOR($s_busy_lead,$s_busy_lag,$s_busy_middle,$s_busy_sides,$s_busy_simple) Then
			Select
				Case $s_goto_forward
					$s_busy_lead = 1
				Case $s_goto_backward
					$s_busy_lag = 1
				Case $s_goto_bidirect
					$s_busy_sides = 1
				EndSelect
			EndIf
			
		Select
			Case $s_busy_lead
				$s = $__ZETTO_SKIN_PROGRESS_LEAD__
			Case $s_busy_lag
				$s = $__ZETTO_SKIN_PROGRESS_LAG__
			Case $s_busy_middle
				$s = $__ZETTO_SKIN_PROGRESS_MIDDLE__
			Case $s_busy_sides
				$s = $__ZETTO_SKIN_PROGRESS_SIDES__
			Case $s_busy_simple
				$s = $__ZETTO_SKIN_PROGRESS_SIMPLE__
			EndSelect

		Local $begin

		$hImageBusy = __zetto_clonehImage($skin[$s])
		Local $twidth = _GDIPlus_ImageGetWidth($hImageBusy)

		
		Switch $skin[$__ZETTO_SKIN_PROGRESS_ZOOM__]
			Case $__ZETTO_ZOOM_SIMPLE__
				_resizeimage($hImageBusy,$twidth,$size[3]-($bar_atas+$bar_bawah))
			Case $__ZETTO_ZOOM_SMART__
				_zetto_SmartZoom($hImageBusy,$twidth,$size[3]-($bar_atas+$bar_bawah),$skin[$__ZETTO_SKIN_PROGRESS_FLEFT__],$skin[$__ZETTO_SKIN_PROGRESS_FRIGHT__],$skin[$__ZETTO_SKIN_PROGRESS_FUP__],$skin[$__ZETTO_SKIN_PROGRESS_FBOTTOM__],$format)
			EndSwitch

		Local $busypos, $startpoint, $endpoint, $currentpoint
		
		$busypos = $prop[$__ZETTO_PROGRESS_BUSYPOS__]
		$startpoint	= $bar_kiri
		$endpoint = $size[2]

		Select
			; Roll bar to forward
			Case $s_goto_forward
				If $busypos == 'NaN' Then $busypos = '-' & $twidth
				$currentpoint = $bar_kiri + $busypos
				$busypos += 4
				If $currentpoint > $endpoint Then $busypos = '-' & $twidth
			; Roll bar to backward
			Case $s_goto_backward
				If $busypos == 'NaN' Then $busypos = $size[2]
				$currentpoint = $bar_kiri+$busypos
				$busypos -= 4
				If $currentpoint < '-' & $twidth Then $busypos = $size[2]
			; Roll bar to forward and backward
			Case $s_goto_bidirect
				If $prop[$__ZETTO_PROGRESS_BIDIRECT__] Then
					If $busypos == 'NaN' Then $busypos = '-' & $twidth
					$currentpoint = $bar_kiri+$busypos
					$busypos += 4
					If $currentpoint > $endpoint Then
						$busypos = 'NaN'
						$prop[$__ZETTO_PROGRESS_BIDIRECT__] = 0
						EndIf
					Else
					If $busypos == 'NaN' Then $busypos = $size[2]
					$currentpoint = $bar_kiri+$busypos
					$busypos -= 4
					If $currentpoint < '-' & $twidth Then
						$busypos = 'NaN'
						$prop[$__ZETTO_PROGRESS_BIDIRECT__] = 1
						EndIf
					EndIf
			EndSelect
		
		;Create start border for left frame
		Local $startdraw, $enddraw, $endpos, $startimage, $endimage
		If $twidth > $currentpoint Then
			$startdraw = 1
			$startimage = _GDIPlus_BitmapCloneArea($hImageProgress,0,0,$bar_kiri,$size[3],$format)
			EndIf
		;Create end border for right frame
		If $currentpoint > ($endpoint - ($twidth+$bar_kiri)) Then
			$enddraw = 1
			$endpos = $size[2]-$bar_kanan
			$endimage = _GDIPlus_BitmapCloneArea($hImageProgress,$endpos,0,$bar_kanan,$size[3],$format)
			EndIf

		_drawimage($hGraphic,$hImageBusy,$currentpoint,$bar_atas)
		
		If $startdraw Then _drawimage($hGraphic,$startimage,0,0)
		If $enddraw Then _drawimage($hGraphic,$endimage,$endpos,0)
		
		; update busy position
		$prop[$__ZETTO_PROGRESS_BUSYPOS__] = $busypos
		
		Local $add
		$add = __zetto_ProgressNum($prop[$__ZETTO_PROGRESS_HANDLE__])
		$_ZETTO_PROGRESS_LIB[$add] = $prop
		
		EndIf
	
	
	If Not $PG_BUSY Then
	$hImageBlock = __zetto_clonehImage($skin[$__ZETTO_SKIN_PROGRESS_FOREGROUND__])
	
	Local $min = $prop[$__ZETTO_PROGRESS_MIN__]
	Local $max = $prop[$__ZETTO_PROGRESS_MAX__]
	Local $pos = $prop[$__ZETTO_PROGRESS_VALUE__]
	Local $procent = $pos * 100 / $max
	Local $procenx = $procent
	If Int($procent) == 0 And StringInStr($procent,'.',1) <> 0 Then $procenx = 1

	If $procenx <> 0 And Not $PG_BUSY Then
		Switch $skin[$__ZETTO_SKIN_PROGRESS_METHOD__]
			Case $__ZETTO_METHOD_LINEAR__
				$pos = Int(($size[2]-($bar_kiri+$bar_kanan)) * (($procenx)/100))
				Switch $skin[$__ZETTO_SKIN_PROGRESS_ZOOM__]
					Case $__ZETTO_ZOOM_SIMPLE__
						_resizeimage($hImageBlock,$pos,$size[3]-($bar_atas+$bar_bawah))
					Case $__ZETTO_ZOOM_SMART__
						_zetto_SmartZoom($hImageBlock,$pos,$size[3]-($bar_atas+$bar_bawah),$skin[$__ZETTO_SKIN_PROGRESS_FLEFT__],$skin[$__ZETTO_SKIN_PROGRESS_FRIGHT__],$skin[$__ZETTO_SKIN_PROGRESS_FUP__],$skin[$__ZETTO_SKIN_PROGRESS_FBOTTOM__],$format)
					EndSwitch
				
				_drawimage($hGraphic,$hImageBlock,$bar_kiri,$bar_atas)
			Case $__ZETTO_METHOD_BLOCK__
				
				Local $max_block = ($size[2] - $bar_kiri - $bar_kanan) / ($skin[$__ZETTO_SKIN_PROGRESS_FOREW__] + $skin[$__ZETTO_SKIN_PROGRESS_SPACE__])
				Local $total_block
				$total_block = Int(($max_block * $procenx) / 100)
				If $total_block == 0 Then $total_block = 1
				
				Switch $skin[$__ZETTO_SKIN_PROGRESS_ZOOM__]
					Case $__ZETTO_ZOOM_SIMPLE__
						_resizeimage($hImageBlock,$skin[$__ZETTO_SKIN_PROGRESS_FOREW__],$size[3]-($bar_atas+$bar_bawah))
					Case $__ZETTO_ZOOM_SMART__
						_zetto_SmartZoom($hImageBlock,$skin[$__ZETTO_SKIN_PROGRESS_FOREW__],$size[3]-($bar_atas+$bar_bawah),$skin[$__ZETTO_SKIN_PROGRESS_FLEFT__],$skin[$__ZETTO_SKIN_PROGRESS_FRIGHT__],$skin[$__ZETTO_SKIN_PROGRESS_FUP__],$skin[$__ZETTO_SKIN_PROGRESS_FBOTTOM__],$format)
					EndSwitch

				; Draw block
				For $loop = 0 To $total_block-1
					_drawimage($hGraphic,$hImageBlock,$bar_kiri+($loop * $skin[$__ZETTO_SKIN_PROGRESS_FOREW__])+($loop * $skin[$__ZETTO_SKIN_PROGRESS_SPACE__]),$bar_atas,0)
					Next
				
				; Fill in the hole
				If $procenx == 100 Then
					Local $sam = ($total_block * $skin[$__ZETTO_SKIN_PROGRESS_FOREW__])+($total_block * $skin[$__ZETTO_SKIN_PROGRESS_SPACE__])
					Local $fill = ($size[2] - $bar_kiri - $bar_kanan) - $sam
					If $fill > 0 Then
						Local $wdth = _GDIPlus_ImageGetWidth($hImageBlock)
						If $fill > $wdth Then $fill = $wdth
						Local $crop = _GDIPlus_BitmapCloneArea($hImageBlock,0,0,$fill,$size[3]-($bar_atas+$bar_bawah),$format)
						_drawimage($hGraphic,$crop,$bar_kiri+$sam,$bar_atas)
						EndIf
					EndIf

				_GDIPlus_ImageDispose($hImageBlock)
			EndSwitch
		EndIf
	
	EndIf
	
	; @SW_DISABLE
	If $prop[$__ZETTO_PROGRESS_DISABLE__] Then
		Local $cmask, $parent
		If $skin[$__ZETTO_SKIN_PROGRESS_GRAYED__] == 'NaN' Then		
			$cmask = _WinAPI_GetSysColor($COLOR_3DFACE)
			$parent = _zetto_GetParent($chnd)
			; Is zettoControl inside Tab
			If StringInStr(WinGetClassList($parent,''),'SysTabControl32') <> 0 Then
				Local $syscol, $systab = ControlGetPos($parent,'[CLASS:SysTabControl32; INSTANCE:1]','')
				If IsArray($systab) Then
					If $size[0] >= $systab[0] And $size[1] >= $systab[1] And $size[2] <= $systab[2] And $size[3] <= $systab[3] Then
						$syscol = _zetto_GetTabBkColor($parent)
						If Not @error Then $cmask = $syscol
						EndIf
					EndIf
				EndIf
			Else
			$cmask = $skin[$__ZETTO_SKIN_PROGRESS_GRAYED__]
			EndIf
		__zetto_disable($hImageProgress, $hGraphic,$cmask)
		EndIf
	
	;_zetto_FlipHorizontal($hImageProgress, $hGraphic)
	;_zetto_FlipVertical($hImageProgress, $hGraphic)
	
	Local $caption = $prop[$__ZETTO_PROGRESS_CAPTION__]
	Local $decimal
	
	If $prop[$__ZETTO_PROGRESS_CAPTION__] <> '' And Not $PG_BUSY Then
		If StringInStr($caption,'%dec',1) <> 0 Or StringInStr($caption,'%intdec',1) <> 0 Then
			If StringInStr($procent,'.',1) <> 0 Then
				$decimal = StringSplit($procent,'.',2)
				$decimal = $decimal[0] & '.' & StringLeft($decimal[1],2)
				$caption = StringReplace($caption,'%dec',$decimal,0,1)
				If StringInStr($caption,'%intdec',1) <> 0 Then $caption = StringReplace($caption,'%intdec',$decimal,0,1)
				Else
				$caption = StringReplace($caption,'%dec',$procent & '.00',0,1)
				If StringInStr($caption,'%intdec',1) <> 0 Then $caption = StringReplace($caption,'%intdec',$procent,0,1)
				EndIf
			EndIf
		$caption = StringReplace($caption,'%int',Int($procent),0,1)
		$caption = StringReplace($caption,'/val',$prop[$__ZETTO_PROGRESS_VALUE__],0,1)

		Local $hBrush, $hFormat, $hFamily, $hFont, $tLayout
		Local $aInfo
		Local $font = $skin[$__ZETTO_SKIN_PROGRESS_FONT__]
		Local $fsize = $skin[$__ZETTO_SKIN_PROGRESS_SIZE__]
		Local $style = $skin[$__ZETTO_SKIN_PROGRESS_STYLE__]
		If $prop[$__ZETTO_PROGRESS_DISABLE__] Then
			$fcolor = $skin[$__ZETTO_SKIN_PROGRESS_COLOR_DISABLE__]
			Else
			$fcolor = $skin[$__ZETTO_SKIN_PROGRESS_COLOR_ENABLE__]
			EndIf

		$hBrush = _GDIPlus_BrushCreateSolid($fcolor)
		$hFormat = _GDIPlus_StringFormatCreate()
		$hFamily = _GDIPlus_FontFamilyCreate($font)
		If isNullPtr($hFamily) Then $hFamily = _GDIPlus_FontFamilyCreate('Arial')
		$hFont = _GDIPlus_FontCreate($hFamily,$fsize,$style)
		$aInfo = _GDIPlus_GraphicsMeasureString($hGraphic,$caption, $hFont,_GDIPlus_RectFCreate(0,0,0,0), $hFormat)
	
		Local $CaptionSpaceLeft	= 0
		Local $CaptionSpaceTop	= 0
	
		Select
			Case BitAND($prop[$__ZETTO_PROGRESS_STYLE__],1)	; Text Top Left
				$tLayout = _GDIPlus_RectFCreate($CaptionSpaceLeft,$CaptionSpaceTop, 0, 0)
			Case BitAND($prop[$__ZETTO_PROGRESS_STYLE__],2)	; Text Top Center
				$tLayout = _GDIPlus_RectFCreate(($size[2]-DllStructGetData($aInfo[0],3))/2,$CaptionSpaceTop, 0, 0)
			Case BitAND($prop[$__ZETTO_PROGRESS_STYLE__],4)	; Text Top Right
				$tLayout = _GDIPlus_RectFCreate(($size[2]-DllStructGetData($aInfo[0],3))-$CaptionSpaceLeft,$CaptionSpaceTop, 0, 0)
			Case BitAND($prop[$__ZETTO_PROGRESS_STYLE__],8)	; Text Middle Left
				$tLayout = _GDIPlus_RectFCreate($CaptionSpaceLeft,($size[3]-DllStructGetData($aInfo[0],4))/2, 0, 0)
			Case BitAND($prop[$__ZETTO_PROGRESS_STYLE__],16)	; Text Middle Center as Default
				$tLayout = _GDIPlus_RectFCreate(-2+($size[2]-DllStructGetData($aInfo[0],3))/2,($size[3]-DllStructGetData($aInfo[0],4))/2,0,0)
			Case BitAND($prop[$__ZETTO_PROGRESS_STYLE__],32)	; Text Middle Right
				$tLayout = _GDIPlus_RectFCreate(($size[2]-DllStructGetData($aInfo[0],3))-$CaptionSpaceLeft,($size[3]-DllStructGetData($aInfo[0],4))/2, 0, 0)
			Case BitAND($prop[$__ZETTO_PROGRESS_STYLE__],64)	; Text Bottom Left
				$tLayout = _GDIPlus_RectFCreate($CaptionSpaceLeft,($size[3]-DllStructGetData($aInfo[0],4))-$CaptionSpaceTop, 0, 0)
			Case BitAND($prop[$__ZETTO_PROGRESS_STYLE__],128)	; Text Bottom Center
				$tLayout = _GDIPlus_RectFCreate(-2+($size[2]-DllStructGetData($aInfo[0],3))/2,($size[3]-DllStructGetData($aInfo[0],4))-$CaptionSpaceTop, 0, 0)
			Case BitAND($prop[$__ZETTO_PROGRESS_STYLE__],256)	; Text Bottom Right
				$tLayout = _GDIPlus_RectFCreate(($size[2]-DllStructGetData($aInfo[0],3))-$CaptionSpaceLeft,($size[3]-DllStructGetData($aInfo[0],4))-$CaptionSpaceTop, 0, 0)
			EndSelect
	
		_GDIPlus_GraphicsDrawStringEx($hGraphic,$caption, $hFont, $tLayout, $hFormat, $hBrush)

		_GDIPlus_BrushDispose($hBrush)
		_GDIPlus_StringFormatDispose($hFormat)
		_GDIPlus_FontFamilyDispose($hFamily)
		_GDIPlus_FontDispose($hFont)
		EndIf

	Local $glass
	
	If $prop[$__ZETTO_PROGRESS_GLASS__] Then
		$glass = 1
		If $prop[$__ZETTO_PROGRESS_DISABLE__] And $prop[$__ZETTO_PROGRESS_GLASS_DISABLE__] <> 'NaN' Then
			_glass_effect($hImageProgress,$hGraphic,$prop[$__ZETTO_PROGRESS_GLASS_DISABLE__])
			Else
			If $prop[$__ZETTO_PROGRESS_GLASS_NORMAL__] <>'NaN' Then _glass_effect($hImageProgress,$hGraphic,$prop[$__ZETTO_PROGRESS_GLASS_NORMAL__])
			EndIf
		EndIf

	__zetto_SetImage($prop[$__ZETTO_PROGRESS_HANDLE__],$hImageProgress,$tRect,$glass)

	_GDIPlus_ImageDispose($hImageProgress)
	_GDIPlus_GraphicsDispose($hGraphic)
EndFunc


Func __zetto_disable(ByRef $hImage, ByRef $hGraphic, $mask)
	If $WIN_2000 Then Return	
	Local $lock, $width, $height, $stride, $scan, $buffer, $value
	Local $hImageGrayed, $loc2, $strid2, $sca2, $buffe2
	Local $alpha = '0x80'
	
	If IsString($mask) Then
		$alpha = StringLeft($mask,4)
		$mask = Execute('0x' & StringRight($mask, 6))
		EndIf

	$width = _GDIPlus_ImageGetWidth($hImage)
	$height = _GDIPlus_ImageGetHeight($hImage)

	$hImageGrayed = __zettoCreatePNG($width, $height)

	$lock = _GDIPlus_BitmapLockBits($hImage, 0, 0, $width, $height, $GDIP_ILMREAD, $GDIP_PXF32ARGB)
	$stride = DllStructGetData($lock, "stride")
	$scan = DllStructGetData($lock, "Scan0")
	
	$loc2 = _GDIPlus_BitmapLockBits($hImageGrayed, 0, 0, $width, $height, $GDIP_ILMWRITE, $GDIP_PXF32ARGB)
	$strid2 = DllStructGetData($loc2, "stride")
	$sca2 = DllStructGetData($loc2, "Scan0")
	
	Local $hex = Hex(0,6)
	
	For $loopw = 0 To $width - 1
		For $looph = 0 To $height - 1
			$buffer = DllStructCreate("dword", $scan + ($looph * $stride) + ($loopw * 4))
			$value = DllStructGetData($buffer, 1)
			If Hex($value, 6) <> $hex Then
				$buffe2 = DllStructCreate("dword", $sca2 + ($looph * $strid2) + ($loopw * 4))
				DllStructSetData($buffe2, 1, $alpha & Hex($mask, 6))
				EndIf
			Next
		Next
	
	_GDIPlus_BitmapUnlockBits($hImage, $lock)
	_GDIPlus_BitmapUnlockBits($hImageGrayed, $loc2)
	
	_drawimage($hGraphic,$hImageGrayed,0,0)
EndFunc

Func __zettobtn_hover($ctrl)
	Local $prop
	For $loop = 0 To UBound($_ZETTO_BUTTON_LIB)-1
		$prop = $_ZETTO_BUTTON_LIB[$loop]
		If $prop[$__ZETTO_BUTTON_HANDLE__] == $ctrl Then
			__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_HOVER__,$prop)
			If $prop[$__ZETTO_BUTTON_NAMEID__] <> -1 Then $_ZETTO_CTRL_EVENT = $prop[$__ZETTO_BUTTON_NAMEID__] & '_MouseOver'
			ExitLoop
			EndIf
		Next
	
	If Not $COMBO_BUTTON_STATE Then AdlibRegister('__zettobtn_combo',90)
	
	$COMBO_BUTTON_STATE = 1
	$COMBO_BUTTON_INDEX = $loop

EndFunc

; Combo button events checker
Func __zettobtn_combo()
	
	; Suicide
	If Not($COMBO_BUTTON_STATE) Then
		AdlibUnRegister('__zettobtn_combo')
		Return
		EndIf
	
	Local $prop, $chnd, $hwnd, $mpos
	
	$prop = $_ZETTO_BUTTON_LIB[$COMBO_BUTTON_INDEX]
	$ctrl = $prop[$__ZETTO_BUTTON_HANDLE__]
	
	If Not IsHWnd($chnd) Then $chnd = GUICtrlGetHandle($ctrl)
	$hwnd = _zetto_GetParent($chnd)
	If Not IsHWnd($hwnd) Then Return SetError(1,0,0)
	
	; Left
	; Right
	; Up
	; Bottom
	
	$chnd = GUICtrlGetHandle($prop[$__ZETTO_BUTTON_HANDLE__])
	$mpos = GUIGetCursorInfo($hwnd)
	$size = ControlGetPos($chnd,'','')
	If  Not IsArray($mpos) Or Not IsArray($size) Then Return
	
	Local $T = 0 ; rect  [default]
	Local $P = 2 ; right [default]
	Local $X = 20
	Local $Y = 15
	
	Switch $T
		Case 0
			Switch $P
				Case 1	; Left
				;ConsoleWrite(($mpos[0]-($size[0]-1) <= $X )&@cr)
				Case 2	; Right [default]
				;ConsoleWrite(($mpos[0]-($size[0]-1) >= $size[2] - $X )&@cr)
				Case 3	; Top
				;ConsoleWrite(($mpos[1]-($size[1]-1) <= $Y)&@cr)
				Case 4	; Bottom
				;ConsoleWrite(($mpos[1]-($size[1]-1) >= $size[3] - $Y )&@cr)
				EndSwitch
		Case 1
			Switch $P
				Case 1	; Left
				Case 2	; Right [default]
				Case 3	; Top
				Case 4	; Bottom
				EndSwitch
		EndSwitch
	
	;ConsoleWrite('Inside Button..!'&@cr)
	
	If IsArray($mpos) Then
		;ConsoleWrite($COMBO_BUTTON_INDEX & ' - ' &$mpos[0]-($size[0]-1) & ' x ' & $mpos[1]-($size[1]-1)&@cr)
		EndIf
EndFunc

Func __zettobtn_down($ctrl)
	;ConsoleWrite('TTT'&@CR)
	If $_ZETTO_BUTTON_DOWN Then
		Local $prop
		For $loop = 0 To UBound($_ZETTO_BUTTON_LIB)-1
			$prop = $_ZETTO_BUTTON_LIB[$loop]
			If $prop[$__ZETTO_BUTTON_HANDLE__] == $ctrl Then
				
				; Release focus state image representation
				Local $lfoc = __zetto_ButtonXWND(_WinAPI_GetFocus())
				If $lfoc <> -1 Then
					If $prop[$__ZETTO_BUTTON_FOCUS__] Then __freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__,$_ZETTO_BUTTON_LIB[$lfoc])
					EndIf
				_WinAPI_SetFocus($prop[$__ZETTO_BUTTON_XWND__])
				
				__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_DOWN__,$prop)
				$_ZETTO_BUTTON_DOWN = 0
				If $prop[$__ZETTO_BUTTON_NAMEID__] <> -1 Then $_ZETTO_CTRL_EVENT = $prop[$__ZETTO_BUTTON_NAMEID__] & '_MouseDown'
				ExitLoop
				EndIf
			Next
		EndIf
EndFunc

Func __zettobtn_up($ctrl)
	Local $prop
	For $loop = 0 To UBound($_ZETTO_BUTTON_LIB)-1
		$prop = $_ZETTO_BUTTON_LIB[$loop]
		If $prop[$__ZETTO_BUTTON_HANDLE__] == $ctrl Then
			If BitAND($prop[$__ZETTO_BUTTON_STYLE__],$SS_MODE_TOGGLE) Then
				__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_HOVER__,$prop)
				If $prop[$__ZETTO_BUTTON_TOGGLE__] Then
					$prop[$__ZETTO_BUTTON_TOGGLE__] = 0
					$_ZETTO_BUTTON_LIB[$loop] = $prop
					If $prop[$__ZETTO_BUTTON_NAMEID__] <> -1 Then $_ZETTO_CTRL_EVENT = $prop[$__ZETTO_BUTTON_NAMEID__] & '_ToogleNo'
					Else
					$prop[$__ZETTO_BUTTON_TOGGLE__] = 1
					$_ZETTO_BUTTON_LIB[$loop] = $prop
					If $prop[$__ZETTO_BUTTON_NAMEID__] <> -1 Then $_ZETTO_CTRL_EVENT = $prop[$__ZETTO_BUTTON_NAMEID__] & '_ToogleOk'
					EndIf
				Else
				__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_HOVER__,$prop)
				If $prop[$__ZETTO_BUTTON_NAMEID__] <> -1 Then $_ZETTO_CTRL_EVENT = $prop[$__ZETTO_BUTTON_NAMEID__] & '_MouseUp'
				EndIf
			ExitLoop
			EndIf
		Next
	If $_ZETTO_BUTTON_DOWN = 0 Then $_ZETTO_BUTTON_DOWN = 1
EndFunc

Func __zettobtn_leave($ctrl)
	Local $prop
	For $loop = 0 To UBound($_ZETTO_BUTTON_LIB)-1
		$prop = $_ZETTO_BUTTON_LIB[$loop]
		If $prop[$__ZETTO_BUTTON_HANDLE__] == $ctrl Then
			If BitAND($prop[$__ZETTO_BUTTON_STYLE__],$SS_MODE_TOGGLE) Then
				If $prop[$__ZETTO_BUTTON_TOGGLE__] Then
					__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_DOWN__,$prop)
					Else
					;Show image focus if control are focused in toogle button
					If $prop[$__ZETTO_BUTTON_FOCUS__] And $prop[$__ZETTO_BUTTON_XWND__] = _WinAPI_GetFocus() Then
						__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_FOCUS__,$prop)
						Else
						__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__,$prop)
						EndIf
					EndIf
				Else
				;Show image focus if control are focused in normal button
				If $prop[$__ZETTO_BUTTON_FOCUS__] And $prop[$__ZETTO_BUTTON_XWND__] = _WinAPI_GetFocus() Then
					;#POINT 5786578856 Button Focus
					__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_FOCUS__,$prop)
					;__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__,$prop)
					Else
					__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__,$prop)
					EndIf
				EndIf
			If $prop[$__ZETTO_BUTTON_NAMEID__] <> -1 Then $_ZETTO_CTRL_EVENT = $prop[$__ZETTO_BUTTON_NAMEID__] & '_MouseLeave'
			
			; Terminate combo button events checker
			; See __zettobtn_combo()
			$COMBO_BUTTON_STATE = 0
			ExitLoop
			EndIf
		Next
	If $_ZETTO_BUTTON_DOWN = 0 Then $_ZETTO_BUTTON_DOWN = 1
EndFunc

; get skin button index from nameskin
Func __zetto_SkinButtonNum($nameskin)
	Local $ret = -1
	If IsArray($_ZETTO_BUTTON_SKN) Then
		Local $array
		For $loop = 0 To UBound($_ZETTO_BUTTON_SKN) - 1
			$array = $_ZETTO_BUTTON_SKN[$loop]
			If Not IsArray($array) Then ExitLoop
			If $array[$__ZETTO_SKIN_BUTTON_NAME__] = $nameskin Then
				$ret = $loop
				ExitLoop
				EndIf
			Next
		EndIf
	Return $ret
EndFunc

Func _isdirectory($path)
	Local $ret = False
	If StringInStr(FileGetAttrib($path),'D',1) <> 0 Then $ret = True
	Return $ret
EndFunc

; by prazetto
Func zettoCtrlLoadButton($nameskin,$resource=0)
	Local $script, $file, $folder, $skin[$__ZETTO_SKIN_BUTTON_PROPERTIES__]
	Local $sequendir, $sequenres

	; Select resource
	Switch StringUpper($resource)
		Case '0'
			If @Compiled Then
				$sequenres = 1
				$resource = @ScriptFullPath
				Else
				$sequendir = 1
				$resource = @ScriptDir
				EndIf
		Case '{EXTERNAL}'
			$sequendir = 1
			$resource = @ScriptDir
		Case Else
			If FileExists($resource) Then
			 If _isdirectory($resource) Then
				$sequendir = 1
				Else
				$sequenres = 1
				EndIf
			EndIf
		EndSwitch

	; Sequence search startegy
	If $sequendir Then
		If __zetto_CtrlScriptExist($resource &  '\Button\' & $nameskin) Then
			$script = __zetto_CtrlScriptOpen($resource &  '\Button\' & $nameskin)
			$folder = $resource &  '\Button\' & $nameskin
		ElseIf __zetto_CtrlScriptExist($resource) Then
			$script = __zetto_CtrlScriptOpen($resource)
			$folder = $resource
		ElseIf __zetto_CtrlScriptExist($resource &  '\' & $nameskin) Then
			$script = __zetto_CtrlScriptOpen($resource &  '\' & $nameskin)
			$folder = $resource &  '\' & $nameskin
		ElseIf StringLen($script) == 0 Then
			If __zetto_CtrlScriptExist($REG_ZETTOCONTROL & '\Button\' & $nameskin) Then
				$script = __zetto_CtrlScriptOpen($REG_ZETTOCONTROL & '\Button\' & $nameskin)
				$folder = $REG_ZETTOCONTROL & '\Button\' & $nameskin
				EndIf
			EndIf
			If StringLen($script) <> 0 Then
				$folder &= '\'
				EndIf
		EndIf

	; Open resource from windows image file (exe, dll, ocx and etc)
	If $sequenres Then
		Local $nameres = 'ZBTN_' & StringUpper(StringReplace($nameskin,' ','_',0))
		$folder = $resource & @CR & $nameres
		$define = _ResourceGetAsRaw($nameres,'ZINIT',1033,$resource)
		If StringLen($define) <> 0 Then $_ZETTO_CTRL_RDLL = $resource
		EndIf

	If StringLen($script) == 0 Then Return SetError(1, 0, 0)
	$script = _zettoscript_Optimize($script)
	
	; Refresh on skin reloading
	Local $numskin = __zetto_SkinButtonNum($nameskin)
	If $numskin <> -1 Then _ArrayDelete($_ZETTO_BUTTON_SKN,$numskin)
	
	
	$skin[$__ZETTO_SKIN_BUTTON_NAME__]			= $nameskin
	
	; Skin preloader

	Local $button = _zettoscript_GetParam($script,'.button::normal')
	Local $varstr

	Local $dummy

	Local $margin_left=0,$margin_right=0,$margin_top=0,$margin_bottom=0
	Local $size_width
	Local $size_height
	Local $pos_x=-1
	Local $pos_y=-1
	
	Local $lmage		; universal $image::normal
	Local $image		; universal str image path
	Local $jmage		; universal mem image binary
	Local $vmage		; current str image path
	Local $xmage		; current mem image binary
	
	Local $image_normal, $in
	Local $image_hover, $ih
	Local $image_down, $id
	Local $image_focus, $if
	Local $image_disable, $is
	
	Local $mask
	
	Local $cap_caption, $col_color

	;zss.button constante
	Local Const $zss_margin 			= 'margin'
	Local Const $zss_margin_left		= 'margin-left'
	Local Const $zss_margin_right		= 'margin-right'
	Local Const $zss_margin_top			= 'margin-top'
	Local Const $zss_margin_bottom		= 'margin-bottom'
	Local Const $zss_size				= 'size'
	Local Const $zss_width				= 'width'
	Local Const $zss_height				= 'height'
	Local Const $zss_position			= 'position'
	Local Const $zss_image				= 'image'
	Local Const $zss_image_normal		= 'image-normal'
	Local Const $zss_image_hover		= 'image-hover'
	Local Const $zss_image_down			= 'image-down'
	Local Const $zss_image_focus		= 'image-focus'
	Local Const $zss_image_disable		= 'image-disable'
	Local Const $zss_mask				= 'mask'
	Local Const $zss_format				= 'format'
	Local Const $zss_glass				= 'glass'
	Local Const $zss_focus_tab			= 'focus-tab'
	Local Const $zss_focus_cursor		= 'focus-cursor'

	;zss.button.caption constante
	Local Const $zss_caption			= '.caption'
	Local Const $zss_cap_format			= 'format'
	Local Const $zss_cap_language		= 'language'
	Local Const $zss_cap_font			= 'font'
	Local Const $zss_cap_family			= 'family'
	Local Const $zss_cap_size			= 'size'
	Local Const $zss_cap_style			= 'style'
	Local Const $zss_cap_unit			= 'unit'
	
	;zss.button.caption.color constante
	Local Const $zss_color				= '.color'
	Local Const $zss_col_normal			= 'normal'
	Local Const $zss_col_hover			= 'hover'
	Local Const $zss_col_down			= 'down'
	Local Const $zss_col_focus			= 'focus'
	Local Const $zss_col_disable		= 'disable'
	
	; parsing definition with sequence
	
	Local $value
	Local $const
	
	For $event = 0 To 4
		Switch $event
			Case 0
				$const = '.button::normal'
			Case 1
				$const = '.button::hover'
			Case 2
				$const = '.button::down'
			Case 3
				$const = '.button::focus'
			Case 4
				$const = '.button::disable'
			EndSwitch

		$button = _zettoscript_GetParam($script,$const)
		;_ArrayDisplay($button)
		
		For $loop=0 To UBound($button)-1
			$pos = StringInStr($button[$loop],':',1);
			If $pos<>0 Then
				;param
				$value = _zettoscript_GetValueEx(StringTrimLeft($button[$loop],StringInStr($button[$loop],':')))
				Switch StringLower(StringLeft($button[$loop],$pos-1))
					Case $zss_margin
						_parser_int4var($value, $margin_left, $margin_right, $margin_top, $margin_bottom)
					Case $zss_margin_left
						_parser_int1var($value,$margin_left)
					Case $zss_margin_right
						_parser_int1var($value,$margin_right)
					Case $zss_margin_top
						_parser_int1var($value,$margin_top)
					Case $zss_margin_bottom
						_parser_int1var($value,$margin_bottom)
					Case $zss_width
						_parser_int1var($value,$size_width)
					Case $zss_height
						_parser_int1var($value,$size_height)
					Case $zss_position
						_parser_int4var($value, $pos_x, $pos_y, $dummy, $dummy)
					Case $zss_size
						_parser_int4var($value, $size_width, $size_height, $dummy, $dummy)
					Case $zss_image
						$image = $value
					Case $zss_image_normal
						$image_normal = $value
						If $in = $NULL Then $in = 1
					Case $zss_image_hover
						$image_hover = $value
						If $ih = $NULL Then $ih = 1
					Case $zss_image_down
						$image_down = $value
						If $id = $NULL Then $id = 1
					Case $zss_image_disable
						$image_disable = $value
						If $is = $NULL Then $is = 1
					Case $zss_image_focus
						$image_focus = $value
						If $if = $NULL Then $if = 1
					Case $zss_mask
						_parser_rgb($value,$mask)
					Case $zss_format
						$skin[$__ZETTO_SKIN_BUTTON_GDIP_FORMAT__] = $value
					Case $zss_glass
						$skin[$__ZETTO_SKIN_BUTTON_GDIP_GLASS__] = $value
					Case $zss_focus_tab
						$skin[$__ZETTO_SKIN_BUTTON_SHOW_FOCUS_TAB__] = $value
					Case $zss_focus_cursor
						$skin[$__ZETTO_SKIN_BUTTON_SHOW_FOCUS_CUR__] = $value
					EndSwitch
				EndIf
			;class
			$pos = StringInStr($button[$loop],'{',1);
			If $pos<>0 Then
				Switch StringLower(StringLeft($button[$loop],$pos-1))
					Case $zss_caption
						 $cap_caption = $button[$loop]
					EndSwitch
				EndIf
			Next;$loop
		If $image == $NULL Then Sleep(1)
	
		Switch $event
			;.button::normal
			Case 0
				If $image_normal == $NULL Then $image_normal = $image
				$vmage = $image_normal
				$lmage = $image
			;.button::hover
			Case 1;
				If $image_hover == $NULL Then $image_hover = $image
				$vmage = $image_hover
				;.button::down
			Case 2
				If $image_down == $NULL Then $image_down = $image
				$vmage = $image_down
			;button::focus
			Case 3
				If $image_focus == $NULL Then $image_focus = $image
				$vmage = $image_focus
			;.button::disable
			Case 4
				If $image_disable == $NULL Then $image_disable = $image
				$vmage = $image_disable
			EndSwitch
	
	;ConsoleWrite($folder &$vmage&@CR)
	
	; size definition doesn't exist
	; position definition doesn't exist -> single image represent single event
	;If $size_width = $NULL And $size_height = $NULL And $pos_x==-1 And $pos_y==-1 Then
		If $event == 0 Then
			$jmage = Load_BMP_From_Mem(FileRead($folder & $vmage))
			$xmage = $jmage
		Else
		If $vmage==$lmage Then
			$jmage = $xmage
			Else
			$jmage = Load_BMP_From_Mem(FileRead($folder & $vmage))
			EndIf
		EndIf


		If $jmage <> 0 Then
				
			;ConsoleWrite($jmage&@CR)


		Local $imgW = _GDIPlus_ImageGetWidth($jmage)
		Local  $imgH = _GDIPlus_ImageGetHeight($jmage)
		;ConsoleWrite($imgW & '  '& $imgH & @cr)
		
		; Set if image size larger than definition means multiple image
		If ($size_width <> $NULL Or $size_height <> $NULL) And ($size_height < $imgH Or $size_width < $imgW) Then
			If $pos_x = -1 Then $pos_x = 0
			If $pos_y = -1 Then $pos_y = 0
			EndIf
		
		If ($size_width <> $NULL Or $size_height <> $NULL) And ($pos_x<>-1 Or $pos_y<>-1) Then
			If $size_width = $NULL Then $size_width = $imgW			; Default are get size from image
			If $size_height = $NULL Then $size_height = $imgH
			
			Local $cam_x = $pos_x + $size_width
			Local $cam_y = $pos_y + $size_height
			Local $zmage
			
			;add $jmage to removal queue -->
			
			; camera inside and position: + +
			If $cam_x >= 0 And $cam_y >= 0 And $cam_x <= $imgW And $cam_y <= $imgH Then
				$zmage = _GDIPlus_BitmapCloneArea($jmage, $pos_x, $pos_y, $size_width, $size_height, $GDIP_PXF32ARGB)
				$jmage = $zmage
				EndIf
			
			; add cache
			
			; camera outside and position + +
			If $cam_x >= 0 And $cam_y >= 0 And ($cam_x > $imgW Or $cam_y > $imgH) Then
				$hImageD = __zettoCreatePNG($imgW+400,$imgH+400)
				$hGraphicD = _GDIPlus_ImageGetGraphicsContext($hImageD)
				_drawimage($hGraphicD,$jmage,0,0,0)
				
				$zmage = _GDIPlus_BitmapCloneArea($hImageD, $pos_x, $pos_y, $size_width, $size_height, $GDIP_PXF32ARGB)
				$jmage = $zmage

				_GDIPlus_ImageDispose($hImageD)
				_GDIPlus_GraphicsDispose($hGraphicD)
				EndIf
			
			EndIf
		
		
		EndIf
		
		; Store image skin button and if there mask are declared then will be filtered
		; On every event can have different mask like red, green, blue or other.

		
		Switch $event
			Case 0
				If $mask <> '' Then
					$lmage = __zettoCreatePNG($size_width, $size_height)
					$hGraphics = _GDIPlus_ImageGetGraphicsContext($lmage)
					;_GDIPlus_GraphicsDrawImage($hGraphics,$jmage,0,0)                     <-- can't do what i want
					_GDIPlus_GraphicsDrawImageRect($hGraphics, $jmage, 0, 0, $size_width, $size_height)
					$jmage = $lmage
					__zetto_BitmapMasking($jmage, Dec($mask),'0xFF')
					EndIf
				$skin[$__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__] = $jmage
			; 1 to 4 block script is based to 0			
			Case 1
				If $mask <> '' Then
					$lmage = __zettoCreatePNG($size_width, $size_height)
					$hGraphics = _GDIPlus_ImageGetGraphicsContext($lmage)
					_GDIPlus_GraphicsDrawImageRect($hGraphics, $jmage, 0, 0, $size_width, $size_height)
					$jmage = $lmage
					__zetto_BitmapMasking($jmage, Dec($mask),'0xFF')
					EndIf
				$skin[$__ZETTO_SKIN_BUTTON_IMAGE_HOVER__] = $jmage
			Case 2
				If $mask <> '' Then
					$lmage = __zettoCreatePNG($size_width, $size_height)
					$hGraphics = _GDIPlus_ImageGetGraphicsContext($lmage)
					_GDIPlus_GraphicsDrawImageRect($hGraphics, $jmage, 0, 0, $size_width, $size_height)
					$jmage = $lmage
					__zetto_BitmapMasking($jmage, Dec($mask),'0xFF')
					EndIf
				$skin[$__ZETTO_SKIN_BUTTON_IMAGE_DOWN__] = $jmage
			Case 3
				

				If $mask <> '' Then
					$lmage = __zettoCreatePNG($size_width, $size_height)
					$hGraphics = _GDIPlus_ImageGetGraphicsContext($lmage)
					_GDIPlus_GraphicsDrawImageRect($hGraphics, $jmage, 0, 0, $size_width, $size_height)
					$jmage = $lmage
					__zetto_BitmapMasking($jmage, Dec($mask),'0xFF')
					EndIf
				$skin[$__ZETTO_SKIN_BUTTON_IMAGE_FOCUS__] = $jmage
				
				
				$skin[$__ZETTO_SKIN_BUTTON_IMAGE_FOCUS__] = $skin[$__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__]

				
			Case 4
				If $mask <> '' Then
					$lmage = __zettoCreatePNG($size_width, $size_height)
					$hGraphics = _GDIPlus_ImageGetGraphicsContext($lmage)
					_GDIPlus_GraphicsDrawImageRect($hGraphics, $jmage, 0, 0, $size_width, $size_height)
					$jmage = $lmage
					__zetto_BitmapMasking($jmage, Dec($mask),'0xFF')
					EndIf
				$skin[$__ZETTO_SKIN_BUTTON_IMAGE_DISABLE__] = $jmage
			EndSwitch
		;EndIf

	; size definition are exist
	; single image represent all event

	Next;$event

	; margin
	$skin[$__ZETTO_SKIN_BUTTON_MARGIN_LEFT__]		= $margin_left
	$skin[$__ZETTO_SKIN_BUTTON_MARGIN_RIGHT__]		= $margin_right
	$skin[$__ZETTO_SKIN_BUTTON_MARGIN_TOP__]		= $margin_top
	$skin[$__ZETTO_SKIN_BUTTON_MARGIN_BOTTOM__]		= $margin_bottom

	; parse zss.button.caption
	$caption = _zettoscript_GetParam($cap_caption,$zss_caption)
	For $loop=0 To UBound($caption)-1
			$pos = StringInStr($caption[$loop],':',1);
			If $pos<>0 Then
				$value = _zettoscript_GetValueEx(StringTrimLeft($caption[$loop],StringInStr($caption[$loop],':')))
				Switch StringLower(StringLeft($caption[$loop],$pos-1))
					Case $zss_cap_format
						$skin[$__ZETTO_SKIN_BUTTON_TEXT_FORMAT__] = $value
					Case $zss_cap_language
						$skin[$__ZETTO_SKIN_BUTTON_TEXT_LANGUAGE__] = $value
					Case $zss_cap_font
						$skin[$__ZETTO_SKIN_BUTTON_TEXT_FONT__] = $value
					Case $zss_cap_family
						$skin[$__ZETTO_SKIN_BUTTON_TEXT_FAMILY__] = $value
					Case $zss_cap_size
						$skin[$__ZETTO_SKIN_BUTTON_TEXT_SIZE__] = $value
					Case $zss_cap_style
						$skin[$__ZETTO_SKIN_BUTTON_TEXT_STYLE__] = $value
					Case $zss_cap_unit
						$skin[$__ZETTO_SKIN_BUTTON_TEXT_UNIT__] = $value
					EndSwitch
				EndIf
			;class
			$pos = StringInStr($caption[$loop],'{',1);
			If $pos<>0 Then
				Switch StringLower(StringLeft($caption[$loop],$pos-1))
					Case $zss_color
						 $col_color = $caption[$loop]
					EndSwitch
				EndIf
		Next

	; parse zss.button.caption.color
	$color = _zettoscript_GetParam($col_color,$zss_color)
	For $loop=0 To UBound($color)-1
			$pos = StringInStr($color[$loop],':',1);
			If $pos<>0 Then
				$value = _zettoscript_GetValueEx(StringTrimLeft($color[$loop],StringInStr($color[$loop],':')))
				Switch StringLower(StringLeft($color[$loop],$pos-1))
					Case $zss_col_normal
						$skin[$__ZETTO_SKIN_BUTTON_TEXT_COLOR_NORMAL__] = $value
					Case $zss_col_hover
						$skin[$__ZETTO_SKIN_BUTTON_TEXT_COLOR_HOVER__] = $value
					Case $zss_col_down
						$skin[$__ZETTO_SKIN_BUTTON_TEXT_COLOR_DOWN__] = $value
					Case $zss_col_focus
						$skin[$__ZETTO_SKIN_BUTTON_TEXT_COLOR_FOCUS__] = $value
					Case $zss_col_disable
						$skin[$__ZETTO_SKIN_BUTTON_TEXT_COLOR_DISABLE__] = $value
					EndSwitch
				EndIf
		Next

	_parser_default($skin[$__ZETTO_SKIN_BUTTON_TEXT_FORMAT__],0x0000)
	_parser_default($skin[$__ZETTO_SKIN_BUTTON_TEXT_LANGUAGE__],0x0000)
	_parser_default($skin[$__ZETTO_SKIN_BUTTON_TEXT_FONT__],'')
	_parser_default($skin[$__ZETTO_SKIN_BUTTON_TEXT_FAMILY__],'')
	_parser_default($skin[$__ZETTO_SKIN_BUTTON_TEXT_SIZE__],11.5)
	_parser_default($skin[$__ZETTO_SKIN_BUTTON_TEXT_STYLE__],0)
	_parser_default($skin[$__ZETTO_SKIN_BUTTON_TEXT_UNIT__],3)
	_parser_default($skin[$__ZETTO_SKIN_BUTTON_TEXT_COLOR_NORMAL__],0x0)

	If isNullVar($skin[$__ZETTO_SKIN_BUTTON_GDIP_FORMAT__]) Then $skin[$__ZETTO_SKIN_BUTTON_GDIP_FORMAT__] = 0x0026200A
	__zetto_Format2Val($skin[$__ZETTO_SKIN_BUTTON_GDIP_FORMAT__])

	__zetto_addtolibrary_arr($_ZETTO_BUTTON_SKN, $skin)
	$_ZETTO_BUTTON_SKIN = $nameskin
	Return 1
EndFunc


Func _parser_default(ByRef $v,$n)
	If $v == 0 Or $v=='' Then $v = $n
EndFunc

; decimal x parser, 1 variable
; store digit with single variable
; Example data type: ^width, ^height, ^margin-right, ^border-left and etc
Func _parser_int1var($varstr, ByRef $x)
	$varstr = StringReplace($varstr,'px','')
	$varstr = StringReplace($varstr,' ','')
	If StringRegExp($varstr,'\A[[:digit:]]*\Z') Then $x = Int($varstr)
EndFunc

; decimal vxyz parser, 4 variable
; store digit with four variable
; Example data type: ^border, ^rectangle, ^margin and etc
Func _parser_int4var($varstr, ByRef $v,ByRef $x,ByRef $y,ByRef $z)
	Local $d, $q
	
	$varstr = StringReplace($varstr,'px',' ')
	$varstr = StringReplace($varstr,'  ',' ')
	$q = StringSplit($varstr,' ',2)
	
	For $l=0 To UBound($q)-1
		$d = StringRegExp($q[$l],'\A[[:digit:]]*\Z')
		Switch $l
			Case 0
				If $d Then $v = Int($q[0])
			Case 2
				If $d Then $x = Int($q[1])
			Case 3
				If $d Then $y = Int($q[2])
			Case 4
				If $d Then $z = Int($q[3])
				ExitLoop
			EndSwitch
		Next
EndFunc

Func _parser_rgb2int($v)
	Return Dec($v)
EndFunc

; rgb parser in multiformat
Func _parser_rgb($varstr,ByRef $d)
	Local $s, $r, $i, $null, $n
	$varstr = StringLower($varstr)
	$varstr = StringReplace($varstr,' ','')
	$varstr = StringReplace($varstr,'[','(')
	$varstr = StringReplace($varstr,']',')')
	$s = StringInStr($varstr,'#',1)
	If $s<>0 Then $r = StringTrimLeft($varstr,$s)
	$s = StringInStr($varstr,'x',1)
	If $s<>0 Then $r = StringTrimLeft($varstr,$s)
	$s = StringInStr($varstr,'h',1)
	If $s<>0 Then $r = StringLeft($varstr,$s-1)
	$s = StringInStr($varstr,'rgb',1); argb & rgba also will be executed
	If $s<>0 Then $r = Execute('_zscript_c' & $varstr)
	$s = StringInStr($varstr,'bgr',1)
	If $s<>0 Then $r = Execute('_zscript_c' & $varstr)
	If $r == $null Then
		If StringRegExp($varstr,'\A[[:digit:]]*\Z') Then
			$r = Hex($varstr,6)
			Else
			If StringInStr($varstr,'r',1)<>0 Or StringInStr($varstr,'g',1)<>0 Or StringInStr($varstr,'b',1)<>0 Then $r = Execute('_zscript_c' & $varstr)
			EndIf
		EndIf
	$n = StringLen($r)
	If $n > 6 Then
		$r = StringRight($r,6)
		Else
		Switch $n
			Case 1
				$r &= '00000'
			Case 2
				$r &= '0000'
			Case 3
				$r &= '000'
			Case 4
				$r &= '00'
			Case 5
				$r &= '0'
			EndSwitch
		EndIf
	$d = $r
EndFunc

Func _zscript_crgb($r=0,$g=0,$b=0)
	Return Hex($r,2) & Hex($g,2) & Hex($b,2)
EndFunc

Func _zscript_cbgr($b=0,$g=0,$r=0)
	Return Hex($r,2) & Hex($g,2) & Hex($b,2)
EndFunc

Func _zscript_cargb($a=0,$r=0,$g=0,$b=0)
	Return Hex($a,2) & Hex($r,2) & Hex($g,2) & Hex($b,2)
EndFunc

Func _zscript_crgba($r=0,$g=0,$b=0,$a=0)
	Return Hex($a,2) & Hex($r,2) & Hex($g,2) & Hex($b,2)
EndFunc

Func _zscript_cr($r)
	Return Hex($r,2) & '0000'
EndFunc

Func _zscript_cg($g)
	Return '00' & Hex($g,2) & '00'
EndFunc

Func _zscript_cb($b)
	Return '0000' & Hex($b,2)
EndFunc





; add to library for array
Func __zetto_addtolibrary_arr(ByRef $library, ByRef $value)
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

; by prazetto
Func zettoCtrlCreateButton($text,$left,$top,$width=125,$height=25,$nameid=-1,$nameskin=-1,$sstyle=0,$glass=0)
	Local $prop[$__ZETTO_BUTTON_PROPERTIES__], $skin
	
	If $sstyle=-1 Then $sstyle=0
	
	If $nameskin == -1 Then
		$nameskin = $_ZETTO_BUTTON_SKIN
		Else
		If __zetto_SkinButtonNum($nameskin) == -1 Then
			If Not zettoCtrlLoadButton($nameskin) Then
				zettoCtrlLoadButton($nameskin,$_ZETTO_CTRL_RDLL)
				EndIf
			Else
			$_ZETTO_BUTTON_SKIN = $nameskin
			EndIf
		EndIf
	
	If $nameid <> -1 Then $nameid = StringReplace($nameid,' ','_')
	If $nameid == '' Then $nameid = -1
	
	;If Icon Alignment not present, default as Middle Left  
	If Not BitOR(BitAND($sstyle,512),BitAND($sstyle,1024),BitAND($sstyle,2048),BitAND($sstyle,4096),BitAND($sstyle,8192),BitAND($sstyle,16384),BitAND($sstyle,32768),BitAND($sstyle,65536),BitAND($sstyle,131072)) Then $sstyle += 4096
	
	Local $chnd = GUICtrlCreatePic('',$left,$top,$width,$height,BitOR($GUI_SS_DEFAULT_PIC,$SS_NOTIFY,$WS_TABSTOP))
	;GUICtrlSetState(-1,$GUI_FOCUS)
	
	_GUICtrl_OnHoverRegister($chnd, '__zettobtn_hover', '__zettobtn_leave', '__zettobtn_down', '__zettobtn_up')
	
	$skin = __zetto_SkinButtonNum($nameskin)
	If $skin == -1 Then Return
	$skin = $_ZETTO_BUTTON_SKN[$skin]
	
	$prop[$__ZETTO_BUTTON_HANDLE__]			= $chnd
	$prop[$__ZETTO_BUTTON_WHANDLE__]		= _zetto_GetParent(GUICtrlGetHandle($chnd))
	$prop[$__ZETTO_BUTTON_XWND__]			= GUICtrlGetHandle($chnd)
	$prop[$__ZETTO_BUTTON_CAPTION__]		= $text
	$prop[$__ZETTO_BUTTON_CAPTION_FONT__]	= $skin[$__ZETTO_SKIN_BUTTON_TEXT_FONT__]
	$prop[$__ZETTO_BUTTON_CAPTION_SIZE__]	= $skin[$__ZETTO_SKIN_BUTTON_TEXT_SIZE__]
	$prop[$__ZETTO_BUTTON_CAPTION_STYLE__]	= $skin[$__ZETTO_SKIN_BUTTON_TEXT_STYLE__]
	$prop[$__ZETTO_BUTTON_COLOR_NORMAL__]	= $skin[$__ZETTO_SKIN_BUTTON_TEXT_COLOR_NORMAL__]
	$prop[$__ZETTO_BUTTON_COLOR_HOVER__]	= $skin[$__ZETTO_SKIN_BUTTON_TEXT_COLOR_HOVER__]
	$prop[$__ZETTO_BUTTON_COLOR_DOWN__]		= $skin[$__ZETTO_SKIN_BUTTON_TEXT_COLOR_DOWN__]
	$prop[$__ZETTO_BUTTON_COLOR_DISABLE__]	= 'NaN'
	$prop[$__ZETTO_BUTTON_NAME_SKIN__]		= $nameskin
	$prop[$__ZETTO_BUTTON_STATE_DISABLE__]		= 0
	$prop[$__ZETTO_BUTTON_NAMEID__]			= $nameid
	$prop[$__ZETTO_BUTTON_ICON_NORMAL__]	= 0
	$prop[$__ZETTO_BUTTON_ICON_HOVER__]		= 0
	$prop[$__ZETTO_BUTTON_ICON_DOWN__]		= 0
	$prop[$__ZETTO_BUTTON_ICON_DISABLE__]	= 0
	$prop[$__ZETTO_BUTTON_ICON_MASK__]		= 0xDEAD
	$prop[$__ZETTO_BUTTON_ICON_SIZE__]		= -1
	$prop[$__ZETTO_BUTTON_STYLE__]			= $sstyle
	$prop[$__ZETTO_BUTTON_TOGGLE__]			= 0
	$prop[$__ZETTO_BUTTON_GLASS__]			= $glass
	$prop[$__ZETTO_BUTTON_GLASS_NORMAL__]	= 'NaN'
	$prop[$__ZETTO_BUTTON_GLASS_HOVER__]	= 'NaN'
	$prop[$__ZETTO_BUTTON_GLASS_DOWN__]		= 'NaN'
	$prop[$__ZETTO_BUTTON_GLASS_DISABLE__]	= 'NaN'
	$prop[$__ZETTO_BUTTON_FOCUS__]			= 1			; Inherit from Button::Skin

	__zetto_addtolibrary_arr($_ZETTO_BUTTON_LIB, $prop)
	__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__,$prop)
	
	Local $ret[2]
	
	$ret[$__ZETTO_CTRL_HANDLE__] = $chnd
	$ret[$__ZETTO_CTRL_IDENTIFIER__] = 'ZBTN'
	
	$_ZETTO_CTRL_LAST = $ret
	$_ZETTO_CTRL_HWND = $prop[$__ZETTO_BUTTON_WHANDLE__]
	
	Return $ret
EndFunc

; Get index number of zetto button on data bank from zctrl
Func __zetto_ButtonNum(ByRef $zctrl)
	;~ Local $array, $ret = -1
	Local $ret = -1
	For $loop = 0 To UBound($_ZETTO_BUTTON_LIB)-1
		;$array = $_ZETTO_BUTTON_LIB[$loop]
		;If $array[$__ZETTO_BUTTON_HANDLE__] == $zctrl[$__ZETTO_BUTTON_HANDLE__] Then
		;	$ret = $loop
		;	ExitLoop
		;	EndIf
		If __zetto_isinside($_ZETTO_BUTTON_LIB[$loop],$__ZETTO_BUTTON_HANDLE__, $zctrl[$__ZETTO_BUTTON_HANDLE__]) Then
			$ret = $loop
			ExitLoop
			EndIf
		Next
	Return $ret
EndFunc

; Get index number of zetto button on data bank from hwnd
Func __zetto_ButtonXWND($xwnd)
	;~ Local $array, $ret = -1
	Local $ret = -1
	For $loop = 0 To UBound($_ZETTO_BUTTON_LIB)-1
		; Base Code version:
		; Lib access by copied to local variable
		;~ $array = $_ZETTO_BUTTON_LIB[$loop]
		;~ If $array[$__ZETTO_BUTTON_XWND__] == $xwnd Then
		;~ 	$ret = $loop
		;~ 	ExitLoop
		;~ 	EndIf
		; Optimized Code version:
		; Lib access by passing as byref
		If __zetto_isinside($_ZETTO_BUTTON_LIB[$loop],$__ZETTO_BUTTON_XWND__, $xwnd) Then
			$ret = $loop
			ExitLoop
			EndIf
		Next
	Return $ret
EndFunc

; Internal Function
; Fast array value comparator function
Func __zetto_isinside(ByRef $lib, $idx,ByRef $val)
	Return $lib[$idx] == $val
EndFunc

; by prazetto
Func __freshbutton($state,ByRef $prop)
	; Inspect control
	Local $chnd, $size
	
	$chnd = GUICtrlGetHandle($prop[$__ZETTO_BUTTON_HANDLE__])
	$size = ControlGetPos($chnd,'','')
	If Not IsArray($size) Then Return

	; Inspect Skin
	Local $skin = __zetto_SkinButtonNum($prop[$__ZETTO_BUTTON_NAME_SKIN__])
	If $skin == -1 Then Return
	$skin = $_ZETTO_BUTTON_SKN[$skin]

	Local $tRect, $format, $fcolor

	$tRect = DllStructCreate($tagRECT)

	DllStructSetData($tRect, 1, $size[0])
	DllStructSetData($tRect, 2, $size[1])
	DllStructSetData($tRect, 3, $size[0] + $size[2])
	DllStructSetData($tRect, 4, $size[1] + $size[3])

	Switch StringUpper($skin[$__ZETTO_SKIN_BUTTON_GDIP_FORMAT__])
		Case 'PXF32ARGB'
			$format = $GDIP_PXF32ARGB
		Case Else
			;$format = $GDIP_PXF24RGB
		EndSwitch

	Switch $state
		;Case $__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__
		;#POINT 3487878843 Button Focus <_freshbutton>
		Case $__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__
			If $prop[$__ZETTO_BUTTON_STATE_DISABLE__] And $prop[$__ZETTO_BUTTON_COLOR_DISABLE__] <> 'NaN' Then
				$fcolor = $prop[$__ZETTO_BUTTON_COLOR_DISABLE__]
				Else
				$fcolor = $prop[$__ZETTO_BUTTON_COLOR_NORMAL__]
				EndIf
		Case $__ZETTO_SKIN_BUTTON_IMAGE_HOVER__
			$fcolor = $prop[$__ZETTO_BUTTON_COLOR_HOVER__]
		Case $__ZETTO_SKIN_BUTTON_IMAGE_DOWN__
			$fcolor = $prop[$__ZETTO_BUTTON_COLOR_DOWN__]
		Case $__ZETTO_SKIN_BUTTON_IMAGE_FOCUS__ 							; <<-- FOCUS
			$fcolor = $prop[$__ZETTO_BUTTON_COLOR_NORMAL__]
		EndSwitch

	Local $hImageButton, $hGraphic

	$hImageButton = __zetto_clonehImage($skin[$state])
	_zetto_SmartZoom($hImageButton, $size[2], $size[3], $skin[$__ZETTO_SKIN_BUTTON_MARGIN_LEFT__], $skin[$__ZETTO_SKIN_BUTTON_MARGIN_RIGHT__],$skin[$__ZETTO_SKIN_BUTTON_MARGIN_TOP__],$skin[$__ZETTO_SKIN_BUTTON_MARGIN_BOTTOM__],$format,1)

	$hGraphic = _GDIPlus_ImageGetGraphicsContext($hImageButton)

	Local $plusicon = 0
	Local $hImageIcon
	Switch $state
			Case $__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__, $__ZETTO_SKIN_BUTTON_IMAGE_FOCUS__ 							; <<-- FOCUS
				$hImageIcon = $__ZETTO_BUTTON_ICON_NORMAL__
			Case $__ZETTO_SKIN_BUTTON_IMAGE_HOVER__
				$hImageIcon = $__ZETTO_BUTTON_ICON_HOVER__
			Case $__ZETTO_SKIN_BUTTON_IMAGE_DOWN__
				$hImageIcon = $__ZETTO_BUTTON_ICON_DOWN__
			EndSwitch
	If $prop[$__ZETTO_BUTTON_STATE_DISABLE__] Then $hImageIcon = $__ZETTO_BUTTON_ICON_DISABLE__
	; IMAGICON
	If $prop[$hImageIcon] <> '0' Then
		$hImageIcon = Load_BMP_From_Mem($prop[$hImageIcon])
		If $prop[$__ZETTO_BUTTON_ICON_MASK__] <> 0xDEAD Then
			__zetto_ImageFilter($hImageIcon)
			__zetto_BitmapMasking($hImageIcon, $prop[$__ZETTO_BUTTON_ICON_MASK__], 0xFF)
			EndIf
		Local $w, $h, $arr
		If $prop[$__ZETTO_BUTTON_ICON_SIZE__] == -1 Then		
			$w = $size[3] - 9
			$h = $size[3] - 9
			Else
			$arr = StringSplit($prop[$__ZETTO_BUTTON_ICON_SIZE__],'x',2)
			If UBound($arr) == 2 Then
				$w = $arr[0]
				$h = $arr[1]
				Else
				$w = $arr[0]
				$h = $arr[0]
				EndIf
			EndIf
		
		_resizeimage($hImageIcon,$w,$h)
		
		; Icon Alignment
		Local $IconSpaceLeft		= 4		; 5
		Local $IconSpaceTop			= 2		; 4

		; Auto Text Alignment Positioning on iconized button if text alignment setting not present
		If $prop[$__ZETTO_BUTTON_CAPTION__] <> '' And Not BitOR(BitAND($prop[$__ZETTO_BUTTON_STYLE__],1),BitAND($prop[$__ZETTO_BUTTON_STYLE__],2),BitAND($prop[$__ZETTO_BUTTON_STYLE__],4),BitAND($prop[$__ZETTO_BUTTON_STYLE__],8),BitAND($prop[$__ZETTO_BUTTON_STYLE__],16),BitAND($prop[$__ZETTO_BUTTON_STYLE__],32),BitAND($prop[$__ZETTO_BUTTON_STYLE__],64),BitAND($prop[$__ZETTO_BUTTON_STYLE__],128),BitAND($prop[$__ZETTO_BUTTON_STYLE__],256)) Then
			Select
				Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],1024)	; Top Center
					$prop[$__ZETTO_BUTTON_STYLE__] += 128	; Text Bottom Center
				Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],65536)	; Bottom Center
					$prop[$__ZETTO_BUTTON_STYLE__] += 2		; Text Top Center
				Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],4096) Or BitAND($prop[$__ZETTO_BUTTON_STYLE__],16384) ; Middle Left or Right
					$prop[$__ZETTO_BUTTON_STYLE__] += 16	; Text Middle Center
				Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],512) Or BitAND($prop[$__ZETTO_BUTTON_STYLE__],2048)	; Top Left or Right
					$prop[$__ZETTO_BUTTON_STYLE__] += 2		; Text Top Center
				Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],32768) Or BitAND($prop[$__ZETTO_BUTTON_STYLE__],131072)	; Bottom Left or Right
					$prop[$__ZETTO_BUTTON_STYLE__] += 128	; Text Bottom Center
				EndSelect
			EndIf

		Select
			Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],512)		; Icon Top Left
				_drawimage($hGraphic,$hImageIcon,$IconSpaceLeft,$IconSpaceTop)
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],1) Then $plusicon = $w + $IconSpaceLeft		; Text Top Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],2) Then $plusicon = $w/2 + $IconSpaceLeft		; Text Top Center
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],4) Then $plusicon = 0							; Text Top Right
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],8) Then $plusicon = $w + $IconSpaceLeft		; Text Middle Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],16) Then $plusicon = $w/2 + $IconSpaceLeft		; Text Middle Center
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],32) Then $plusicon = 0						; Text Middle Right
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],64) Then $plusicon = $w + $IconSpaceLeft		; Text Bottom Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],128) Then $plusicon = $w/2 + $IconSpaceLeft	; Text Bottom Center
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],256) Then $plusicon = 0						; Text Bottom Right
			Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],1024)	; Icon Top Center
				_drawimage($hGraphic,$hImageIcon,($size[2]-$w)/2,$IconSpaceTop)
			Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],2048)	; Icon Top Right
				_drawimage($hGraphic,$hImageIcon,($size[2]-$w)-$IconSpaceLeft,$IconSpaceTop)
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],1) Then $plusicon = 0							; Text Top Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],2) Then $plusicon -= $w/2 + $IconSpaceLeft		; Text Top Center
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],4) Then $plusicon -= $w + $IconSpaceLeft		; Text Top Right
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],8) Then $plusicon = 0							; Text Middle Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],16) Then $plusicon -= $w/2 + $IconSpaceLeft	; Text Middle Center
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],32) Then $plusicon -= $w + $IconSpaceLeft		; Text Middle Right
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],64) Then $plusicon = 0						; Text Bottom Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],128) Then $plusicon -= $w/2 + $IconSpaceLeft	; Text Bottom Center
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],256) Then $plusicon -= $w + $IconSpaceLeft		; Text Bottom Right
			Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],4096)	; Icon Middle Left as Default
				_drawimage($hGraphic,$hImageIcon,$IconSpaceLeft,($size[3]-$h)/2)
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],1) Then $plusicon = $w + $IconSpaceLeft		; Text Top Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],2) Then $plusicon = $w/2 + $IconSpaceLeft		; Text Top Center
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],4) Then $plusicon = 0							; Text Top Right
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],8) Then $plusicon = $w + $IconSpaceLeft		; Text Middle Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],16) Then $plusicon = $w/2 + $IconSpaceLeft		; Text Middle Center
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],32) Then $plusicon = 0						; Text Middle Right
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],64) Then $plusicon = $w + $IconSpaceLeft		; Text Bottom Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],128) Then $plusicon = $w/2 + $IconSpaceLeft	; Text Bottom Center
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],256) Then $plusicon = 0						; Text Bottom Right
			Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],8192)	; Icon Middle Center
				_drawimage($hGraphic,$hImageIcon,($size[2]-$w)/2,($size[3]-$h)/2)
			Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],16384)	; Icon Middle Right
				_drawimage($hGraphic,$hImageIcon,($size[2]-$w)-$IconSpaceLeft,($size[3]-$h)/2)
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],1) Then $plusicon = 0							; Text Top Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],2) Then $plusicon -= $w/2 + $IconSpaceLeft		; Text Top Center
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],4) Then $plusicon -= $w + $IconSpaceLeft		; Text Top Right
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],8) Then $plusicon = 0							; Text Middle Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],16) Then $plusicon -= $w/2 + $IconSpaceLeft	; Text Middle Center
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],32) Then $plusicon -= $w + $IconSpaceLeft		; Text Middle Right
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],64) Then $plusicon = 0						; Text Bottom Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],128) Then $plusicon -= $w/2 + $IconSpaceLeft	; Text Bottom Center
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],256) Then $plusicon -= $w + $IconSpaceLeft		; Text Bottom Right
			Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],32768)	; Icon Bottom Left
				_drawimage($hGraphic,$hImageIcon,$IconSpaceLeft,($size[3]-$h)-$IconSpaceTop)
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],1) Then $plusicon = $w + $IconSpaceLeft		; Text Top Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],2) Then $plusicon = $w/2 + $IconSpaceLeft		; Text Top Center
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],4) Then $plusicon = 0							; Text Top Right
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],8) Then $plusicon = $w + $IconSpaceLeft		; Text Middle Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],16) Then $plusicon = $w/2 + $IconSpaceLeft		; Text Middle Center
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],32) Then $plusicon = 0						; Text Middle Right
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],64) Then $plusicon = $w + $IconSpaceLeft		; Text Bottom Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],128) Then $plusicon = $w/2 + $IconSpaceLeft	; Text Bottom Center
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],256) Then $plusicon = 0						; Text Bottom Right
			Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],65536)	; Icon Bottom Center
				_drawimage($hGraphic,$hImageIcon,($size[2]-$w)/2,($size[3]-$h)-$IconSpaceTop)
			Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],131072)	; Icon Bottom Right
				_drawimage($hGraphic,$hImageIcon,($size[2]-$w)-$IconSpaceLeft,($size[3]-$h)-$IconSpaceTop)
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],1) Then $plusicon = 0							; Text Top Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],2) Then $plusicon -= $w/2 + $IconSpaceLeft		; Text Top Center
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],4) Then $plusicon -= $w + $IconSpaceLeft		; Text Top Right
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],8) Then $plusicon = 0							; Text Middle Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],16) Then $plusicon -= $w/2 + $IconSpaceLeft	; Text Middle Center
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],32) Then $plusicon -= $w + $IconSpaceLeft		; Text Middle Right
				;If BitAND($prop[$__ZETTO_BUTTON_STYLE__],64) Then $plusicon = 0						; Text Bottom Left
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],128) Then $plusicon -= $w/2 + $IconSpaceLeft	; Text Bottom Center
				If BitAND($prop[$__ZETTO_BUTTON_STYLE__],256) Then $plusicon -= $w + $IconSpaceLeft		; Text Bottom Right
			EndSelect
		EndIf
	; End Imagicon
	
	Local $glass
	
		If $prop[$__ZETTO_BUTTON_GLASS__] Then
		$glass = 1
		Switch $state
			Case $__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__
				If $prop[$__ZETTO_BUTTON_STATE_DISABLE__] And $prop[$__ZETTO_BUTTON_GLASS_DISABLE__] <> 'NaN' Then
					_glass_effect($hImageButton,$hGraphic,$prop[$__ZETTO_BUTTON_GLASS_DISABLE__])
					Else
					If $prop[$__ZETTO_BUTTON_GLASS_NORMAL__] <> 'NaN' Then _glass_effect($hImageButton,$hGraphic,$prop[$__ZETTO_BUTTON_GLASS_NORMAL__])
					EndIf
			Case $__ZETTO_SKIN_BUTTON_IMAGE_HOVER__
				If $prop[$__ZETTO_BUTTON_GLASS_HOVER__] <> 'NaN' Then _glass_effect($hImageButton,$hGraphic,$prop[$__ZETTO_BUTTON_GLASS_HOVER__])
			Case $__ZETTO_SKIN_BUTTON_IMAGE_DOWN__
				If $prop[$__ZETTO_BUTTON_GLASS_DOWN__] <> 'NaN' Then _glass_effect($hImageButton,$hGraphic,$prop[$__ZETTO_BUTTON_GLASS_DOWN__])
			EndSwitch
		EndIf
	
	; @SW_DISABLE
	Local $hBrushMask
	If $prop[$__ZETTO_BUTTON_STATE_DISABLE__] Then
		Local $cmask, $parent
		$cmask = _WinAPI_GetSysColor($COLOR_3DFACE)
		$parent = _zetto_GetParent($chnd)
		; Is zettoControl inside Tab
		If StringInStr(WinGetClassList($parent,''),'SysTabControl32') <> 0 Then
			Local $syscol, $systab = ControlGetPos($parent,'[CLASS:SysTabControl32; INSTANCE:1]','')
			If IsArray($systab) Then
				If $size[0] >= $systab[0] And $size[1] >= $systab[1] And $size[2] <= $systab[2] And $size[3] <= $systab[3] Then
					 $syscol = _zetto_GetTabBkColor($parent)
					 If Not @error Then $cmask = $syscol
					EndIf
				EndIf
			EndIf
		$hBrushMask = _GDIPlus_BrushCreateSolid (0x80000000+$cmask)
		If $prop[$__ZETTO_BUTTON_COLOR_DISABLE__] <> 'NaN' And Not $prop[$__ZETTO_BUTTON_GLASS__] Then
			_GDIPlus_GraphicsFillRect($hGraphic,0,0,$size[2],$size[3],$hBrushMask)
			_GDIPlus_BrushDispose($hBrushMask)
			EndIf
		EndIf
	; End @SW_DISABLE
	
	; Auto Text Alignment, Text Middle Center
	If $plusicon == 0 And $prop[$__ZETTO_BUTTON_CAPTION__] <> '' And Not BitOR(BitAND($prop[$__ZETTO_BUTTON_STYLE__],1),BitAND($prop[$__ZETTO_BUTTON_STYLE__],2),BitAND($prop[$__ZETTO_BUTTON_STYLE__],4),BitAND($prop[$__ZETTO_BUTTON_STYLE__],8),BitAND($prop[$__ZETTO_BUTTON_STYLE__],16),BitAND($prop[$__ZETTO_BUTTON_STYLE__],32),BitAND($prop[$__ZETTO_BUTTON_STYLE__],64),BitAND($prop[$__ZETTO_BUTTON_STYLE__],128),BitAND($prop[$__ZETTO_BUTTON_STYLE__],256)) Then $prop[$__ZETTO_BUTTON_STYLE__] += 16
	
	If $prop[$__ZETTO_BUTTON_CAPTION__] <> '' Then
	Local $hBrush, $hFormat, $hFamily, $hFont, $tLayout
	Local $aInfo
	Local $font = $prop[$__ZETTO_BUTTON_CAPTION_FONT__]
	Local $fsize = $prop[$__ZETTO_BUTTON_CAPTION_SIZE__]
	Local $style = $prop[$__ZETTO_BUTTON_CAPTION_STYLE__]

    $hBrush = _GDIPlus_BrushCreateSolid($fcolor)
    $hFormat = _GDIPlus_StringFormatCreate()
    $hFamily = _GDIPlus_FontFamilyCreate($font)
	If isNullPtr($hFamily) Then $hFamily = _GDIPlus_FontFamilyCreate("Arial")
    $hFont = _GDIPlus_FontCreate($hFamily,$fsize,$style)
	$aInfo = _GDIPlus_GraphicsMeasureString($hGraphic,$prop[$__ZETTO_BUTTON_CAPTION__], $hFont,_GDIPlus_RectFCreate(1,1,0,0), $hFormat)

	; Caption Alignment
	Local $CaptionSpaceLeft		= 1
	Local $CaptionSpaceTop		= 1

	Select
		Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],1)	; Text Top Left
			$tLayout = _GDIPlus_RectFCreate($CaptionSpaceLeft+$plusicon,$CaptionSpaceTop, 0, 0)
		Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],2)	; Text Top Center
			$tLayout = _GDIPlus_RectFCreate(($size[2]+$plusicon-DllStructGetData($aInfo[0],3))/2,$CaptionSpaceTop, 0, 0)
		Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],4)	; Text Top Right
			$tLayout = _GDIPlus_RectFCreate((($size[2]+$plusicon)-DllStructGetData($aInfo[0],3))-$CaptionSpaceLeft,$CaptionSpaceTop, 0, 0)
		Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],8)	; Text Middle Left
			$tLayout = _GDIPlus_RectFCreate($CaptionSpaceLeft+$plusicon,2+($size[3]-DllStructGetData($aInfo[0],4))/2, 0, 0)
		Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],16)	; Text Middle Center as Default
			If $size[3] >= 25 Then			
				$tLayout = _GDIPlus_RectFCreate(-1+(($size[2]-DllStructGetData($aInfo[0],3))+$plusicon)/2,1+($size[3]-DllStructGetData($aInfo[0],4))/2, 0, 0)
				Else
				$tLayout = _GDIPlus_RectFCreate(-1+(($size[2]-DllStructGetData($aInfo[0],3))+$plusicon)/2,($size[3]-DllStructGetData($aInfo[0],4))/2, 0, 0)			
				EndIf
		Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],32)	; Text Middle Right
			$tLayout = _GDIPlus_RectFCreate((($size[2]-DllStructGetData($aInfo[0],3))-$CaptionSpaceLeft)+$plusicon,2+($size[3]-DllStructGetData($aInfo[0],4))/2, 0, 0)
		Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],64)	; Text Bottom Left
			$tLayout = _GDIPlus_RectFCreate($CaptionSpaceLeft+$plusicon,($size[3]-DllStructGetData($aInfo[0],4))-$CaptionSpaceTop, 0, 0)
		Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],128)	; Text Bottom Center
			$tLayout = _GDIPlus_RectFCreate(-2+($size[2]+$plusicon-DllStructGetData($aInfo[0],3))/2,($size[3]-DllStructGetData($aInfo[0],4))-$CaptionSpaceTop, 0, 0)
		Case BitAND($prop[$__ZETTO_BUTTON_STYLE__],256)	; Text Bottom Right
			$tLayout = _GDIPlus_RectFCreate((($size[2]+$plusicon)-DllStructGetData($aInfo[0],3))-$CaptionSpaceLeft,($size[3]-DllStructGetData($aInfo[0],4))-$CaptionSpaceTop, 0, 0)
		EndSelect

	_GDIPlus_GraphicsDrawStringEx($hGraphic,$prop[$__ZETTO_BUTTON_CAPTION__], $hFont, $tLayout, $hFormat, $hBrush)
	
	_GDIPlus_BrushDispose($hBrush)
	_GDIPlus_StringFormatDispose($hFormat)
	_GDIPlus_FontFamilyDispose($hFamily)
	_GDIPlus_FontDispose($hFont)
	
	EndIf	
	
	; @SW_DISABLE
	If $prop[$__ZETTO_BUTTON_STATE_DISABLE__] And $prop[$__ZETTO_BUTTON_COLOR_DISABLE__] == 'NaN' And Not $prop[$__ZETTO_BUTTON_GLASS__] Then
		_GDIPlus_GraphicsFillRect($hGraphic,0,0,$size[2],$size[3],$hBrushMask)
		_GDIPlus_BrushDispose($hBrushMask)
		EndIf
	; End @SW_DISABLE
	
	__zetto_SetImage($prop[$__ZETTO_BUTTON_HANDLE__],$hImageButton, $tRect,$glass)

	_GDIPlus_ImageDispose($hImageButton)
	_GDIPlus_GraphicsDispose($hGraphic)

EndFunc

; Author : Your Name
Func _glass_effect(ByRef $hImage, ByRef $hGraphic, $alpha='0xff')
	If $WIN_2000 Then Return	
	Local $hImageClone, $lock, $width, $height, $stride, $scan, $buffer, $value
	Local $hImageFilter, $hGraphicFilter

	$width = _GDIPlus_ImageGetWidth($hImage)
	$height = _GDIPlus_ImageGetHeight($hImage)
	
	$hImageClone = _GDIPlus_BitmapCloneArea($hImage, 0, 0, $width, $height, $GDIP_PXF32ARGB)

	$lock = _GDIPlus_BitmapLockBits($hImageClone, 0, 0, $width, $height, BitOR($GDIP_ILMREAD, $GDIP_ILMWRITE), $GDIP_PXF32ARGB)
	$stride = DllStructGetData($lock, "stride")
	$scan = DllStructGetData($lock, "Scan0")
	
	Local $hex = Hex(0, 6)
	
	For $loopw = 0 To $width - 1
		For $looph = 0 To $height - 1
			$buffer = DllStructCreate("dword", $scan + ($looph * $stride) + ($loopw * 4))
			$value = DllStructGetData($buffer, 1)
			If Hex($value, 6) <> $hex Then DllStructSetData($buffer, 1, $alpha & Hex($value, 6))
			Next
		Next
	
	_GDIPlus_BitmapUnlockBits($hImageClone, $lock)
	$hImageFilter = __zettoCreatePNG($width,$height)
	$hGraphicFilter = _GDIPlus_ImageGetGraphicsContext($hImageFilter)
	_drawimage($hGraphicFilter,$hImageClone,0,0)
	_GDIPlus_ImageDispose($hImage)
	_GDIPlus_ImageDispose($hImageClone)
	_GDIPlus_GraphicsDispose($hGraphic)
	$hImage = $hImageFilter
	$hGraphic = $hGraphicFilter
EndFunc

Func _zetto_GetTabBkColor($hwnd)
	If $DLL_UXTHEME = -1 Then SetError(1, 0, 0)
	Local $hTheme, $cTheme, $PartId = 11

	$hTheme = DllCall($DLL_UXTHEME, 'ptr', 'OpenThemeData', 'hwnd', $hwnd, 'wstr', 'Tab')
	If @error Then Return SetError(2, 0, 0)
	If @OSVersion == 'WIN_XP' Or @OSVersion == 'WIN_2003' Then $PartId = 10
	$cTheme = DllCall($DLL_UXTHEME, 'uint', 'GetThemeColor', 'ptr', $hTheme[0], 'int', $PartId, 'int', 1, 'int', 3821, 'dword*', 0)
	If @error Then Return SetError(3, 0, 0)
	DllCall($DLL_UXTHEME, 'uint', 'CloseThemeData', 'ptr', $hTheme[0])

	Return $cTheme[5]
EndFunc

; zettoCtrlCreateMenu(title0, menuid0, title1, menuid1, titleN..., menuidN...)
; zettoCtrlCreateToolbar

;zettoCtrlSetStyleActive  0 set for button
;                         1 set for progress
;zettoCtrlCreateSound
;zettoCtrlCreateFilter
;zettoCtrlAddFilter
;zettoCtrlGetWindow
;zettoCtrlGetStyle

Func zettoCtrlSetStyle($zctrl=-1,$style=0)
	If $zctrl == -1 Then $zctrl = $_ZETTO_CTRL_LAST
	If Not IsArray($zctrl) Then Return
	Local $num, $prop, $ret = 0
	Switch $zctrl[$__ZETTO_CTRL_IDENTIFIER__]
		Case 'ZBTN'
			$num = __zetto_ButtonNum($zctrl)
			If $num <> -1 Then
				$prop = $_ZETTO_BUTTON_LIB[$num]
				If $prop[$__ZETTO_BUTTON_STYLE__] <> $style Then
					$prop[$__ZETTO_BUTTON_STYLE__] = $style
					$_ZETTO_BUTTON_LIB[$num] = $prop
					__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__,$prop)
					EndIf
				$ret = 1
				EndIf
			Case 'ZBAR'
			$num = __zetto_ProgressNum($zctrl[$__ZETTO_CTRL_HANDLE__])
			If $num <> -1 Then
				$prop = $_ZETTO_PROGRESS_LIB[$num]
				If $prop[$__ZETTO_PROGRESS_STYLE__] <> $style Then
					$prop[$__ZETTO_PROGRESS_STYLE__] = $style
					$_ZETTO_PROGRESS_LIB[$num] = $prop
					__freshprogress($prop)
					EndIf
				$ret=1
				EndIf
		EndSwitch
	Return $ret
EndFunc

; by prazetto
Func zettoCtrlSetSkin($zctrl=-1,$nameskin='')
	If $zctrl == -1 Then $zctrl = $_ZETTO_CTRL_LAST
	If Not IsArray($zctrl) And $nameskin <> '' Then Return
	Local $num, $prop, $ret = 0
	Switch $zctrl[$__ZETTO_CTRL_IDENTIFIER__]
		Case 'ZBTN'
			$num = __zetto_ButtonNum($zctrl)
			If $num <> -1 Then
				$prop = $_ZETTO_BUTTON_LIB[$num]
				If __zetto_SkinButtonNum($nameskin) == -1 Then
					zettoCtrlLoadButton($nameskin)
					EndIf
					Local $skin

					$skin = __zetto_SkinButtonNum($nameskin)
					If $skin == -1 Then Return
					$skin = $_ZETTO_BUTTON_SKN[$skin]
				
					$prop[$__ZETTO_BUTTON_CAPTION_FONT__]	= $skin[$__ZETTO_SKIN_BUTTON_TEXT_FONT__]
					$prop[$__ZETTO_BUTTON_CAPTION_SIZE__]	= $skin[$__ZETTO_SKIN_BUTTON_TEXT_SIZE__]
					$prop[$__ZETTO_BUTTON_CAPTION_STYLE__]	= $skin[$__ZETTO_SKIN_BUTTON_TEXT_STYLE__]
					$prop[$__ZETTO_BUTTON_COLOR_NORMAL__]	= $skin[$__ZETTO_SKIN_BUTTON_TEXT_COLOR_NORMAL__]
					$prop[$__ZETTO_BUTTON_COLOR_HOVER__]	= $skin[$__ZETTO_SKIN_BUTTON_TEXT_COLOR_HOVER__]
					$prop[$__ZETTO_BUTTON_COLOR_DOWN__]		= $skin[$__ZETTO_SKIN_BUTTON_TEXT_COLOR_DOWN__]
					$prop[$__ZETTO_BUTTON_NAME_SKIN__]		= $nameskin
				
				__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__,$prop)
				$_ZETTO_BUTTON_LIB[$num] = $prop
				$ret = 1
				EndIf
		Case 'ZBAR'
			$num = __zetto_ProgressNum($zctrl[$__ZETTO_CTRL_HANDLE__])
			If $num <> -1 Then
				$prop = $_ZETTO_PROGRESS_LIB[$num]
				If $prop[$__ZETTO_PROGRESS_NAME_SKIN__] <> $nameskin Then
					$prop[$__ZETTO_PROGRESS_NAME_SKIN__] = $nameskin
					__freshprogress($prop)
					EndIf
				$_ZETTO_PROGRESS_LIB[$num] = $prop
				$ret=1
				EndIf
		EndSwitch
	Return $ret
EndFunc

; #FUNCTION# ===================================================================
; Name...........: zettoCtrlSetImage
; Description ...: Sets the image to use for a zettoControl.
; Syntax.........: zettoCtrlSetImage ( opt $zctrl, opt $normal, opt $hover,
;                                      opt $down, opt $disable, opt $size,
;                                      opt $mask )
; Parameters ....: $zctrl  - The zettoControl identifier as returned by a
;                            zettoCtrlCreate... function.
;                  $normal  - image on normal      -+  File or image data string.
;                  $hover   - image on mouse over   |  BMP, JPG, PNG and etc..
;                  $down    - image on mouse down   |    0 - Clear recent image
;                  $disable - image on disable     -+    1 - Keep recent image
;                  $size    - set image size
;                             -1        --> Default
;                             24        --> 24x24 pixel
;                             '18x16'   --> 18x16 pixel
;                  $mask    - Format $RGB, 0x000000, usefull on bmp tranparency
;                             0xDEAD = no masking [default]
;                             0xDEFA = Mask color are pixel color on coord 1,1
; Return values .: Success - Returns 1.
;                  Failure - Returns 0.
; Author ........: Prasetyo Priadi
; Modified ......:
; Related .......:
; Link ..........:
; Example .......: Yes
; ==============================================================================
Func zettoCtrlSetImage($zctrl=-1,$normal=0,$hover=0,$down=0,$disable=0,$size=-1,$mask=0xDEAD)
	If $zctrl == -1 Then $zctrl = $_ZETTO_CTRL_LAST
	If Not IsArray($zctrl) Then Return
	Local $num, $prop, $ret = 0
	Switch $zctrl[$__ZETTO_CTRL_IDENTIFIER__]
		Case 'ZBTN'
			$num = __zetto_ButtonNum($zctrl)
			If $num <> -1 Then
				$prop = $_ZETTO_BUTTON_LIB[$num]
				
				Local $first = StringLen($prop[$__ZETTO_BUTTON_ICON_NORMAL__]) + StringLen($prop[$__ZETTO_BUTTON_ICON_HOVER__]) + StringLen($prop[$__ZETTO_BUTTON_ICON_DOWN__]) + StringLen($prop[$__ZETTO_BUTTON_ICON_DISABLE__])
				
				If BitOR(IsString($normal),IsBinary($normal)) And StringLen($normal) > 255 Then
					If IsBinary($normal) Then $normal = BinaryToString($normal)
					$prop[$__ZETTO_BUTTON_ICON_NORMAL__] = $normal
					EndIf
				If BitOR(IsString($hover),IsBinary($hover)) And StringLen($hover) > 255 Then
					If IsBinary($hover) Then $hover = BinaryToString($hover)
					$prop[$__ZETTO_BUTTON_ICON_HOVER__] = $hover
					EndIf
				If BitOR(IsString($down),IsBinary($down)) And StringLen($down) > 255 Then
					If IsBinary($down) Then $down = BinaryToString($down)
					$prop[$__ZETTO_BUTTON_ICON_DOWN__] = $down
					EndIf
				If BitOR(IsString($disable),IsBinary($disable)) And StringLen($disable) > 255 Then
					If IsBinary($disable) Then $disable = BinaryToString($disable)
					$prop[$__ZETTO_BUTTON_ICON_DISABLE__] = $disable
					EndIf
				
				If IsString($normal) And String($normal) <> '0' And StringLen($normal) < 255 Then
					If FileExists($normal) Then $prop[$__ZETTO_BUTTON_ICON_NORMAL__] = FileRead($normal)
					EndIf
				If IsString($hover) And String($hover) <> '0' And StringLen($hover) < 255 Then
					If FileExists($hover) Then $prop[$__ZETTO_BUTTON_ICON_HOVER__] = FileRead($hover)
					EndIf
				If IsString($down) And String($down) <> '0' And StringLen($down) < 255 Then
					If FileExists($down) Then $prop[$__ZETTO_BUTTON_ICON_DOWN__] = FileRead($down)
					EndIf
				If IsString($disable) And String($disable) <> '0' And StringLen($disable) < 255 Then
					If FileExists($disable) Then $prop[$__ZETTO_BUTTON_ICON_DISABLE__] = FileRead($disable)
					EndIf
				
				If String($normal) == '0' Then $prop[$__ZETTO_BUTTON_ICON_NORMAL__] = '0'
				If String($hover) == '0' Then $prop[$__ZETTO_BUTTON_ICON_HOVER__] = '0'
				If String($down) =='0' Then $prop[$__ZETTO_BUTTON_ICON_DOWN__] = '0'
				If String($disable) =='0' Then $prop[$__ZETTO_BUTTON_ICON_DISABLE__] = '0'

				If $first == 4 Then
					If $prop[$__ZETTO_BUTTON_ICON_HOVER__] = '0' Then $prop[$__ZETTO_BUTTON_ICON_HOVER__] = $prop[$__ZETTO_BUTTON_ICON_NORMAL__]
					If $prop[$__ZETTO_BUTTON_ICON_DOWN__] = '0' Then $prop[$__ZETTO_BUTTON_ICON_DOWN__] = $prop[$__ZETTO_BUTTON_ICON_NORMAL__]
					If $prop[$__ZETTO_BUTTON_ICON_DISABLE__] = '0' Then $prop[$__ZETTO_BUTTON_ICON_DISABLE__] = $prop[$__ZETTO_BUTTON_ICON_NORMAL__]
					EndIf
				
				If $mask <> 0xDEAD Then $prop[$__ZETTO_BUTTON_ICON_MASK__] = $mask
				
				$prop[$__ZETTO_BUTTON_ICON_SIZE__]	= $size
				__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__,$prop)
				$_ZETTO_BUTTON_LIB[$num] = $prop
				$ret = 1
				EndIf
		EndSwitch
	Return $ret
EndFunc

; #FUNCTION# ===================================================================
; Name...........: zettoCtrlSetColor
; Description ...: Sets the text color of a zettoControl.
; Syntax.........: Button
;                  zettoCtrlSetColor ( opt $zctrl, opt $normal, opt $hover, opt $down, opt $disable )
; Parameters ....: $zctrl - The zettoControl identifier as returned by a
;                           zettoCtrlCreate... function.
;                  $normal  - color on normal
;                  $hover   - color on mouse over
;                  $down    - color on mouse down
;                  $disable - color on disable state
;                  Format color on $ARGB, 0xFF000000
; Return values .: Success - Returns 1.
;                  Failure - Returns 0.
; Author ........: Prasetyo Priadi
; Modified ......:
; Related .......:
; Link ..........:
; Example .......: Yes
; ==============================================================================
Func zettoCtrlSetColor($zctrl=-1,$normal=-1,$hover=-1,$down=-1,$disable=-1)
	If $zctrl == -1 Then $zctrl = $_ZETTO_CTRL_LAST
	If Not IsArray($zctrl) Then Return
	Local $num, $prop, $ret = 0
	Switch $zctrl[$__ZETTO_CTRL_IDENTIFIER__]
		Case 'ZBTN'
			$num = __zetto_ButtonNum($zctrl)
			If $num <> -1 Then
				$prop = $_ZETTO_BUTTON_LIB[$num]
				If $normal <> -1 Then $prop[$__ZETTO_BUTTON_COLOR_NORMAL__] = $normal
				If $hover <> -1 Then $prop[$__ZETTO_BUTTON_COLOR_HOVER__] = $hover
				If $down <> -1 Then $prop[$__ZETTO_BUTTON_COLOR_DOWN__] = $down
				If $disable <> -1 Then $prop[$__ZETTO_BUTTON_COLOR_DISABLE__] = $disable
				$_ZETTO_BUTTON_LIB[$num] = $prop
				If $normal <> -1 Or $hover <> -1 Or $down <> -1 Or $disable <> -1 Then __freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__,$prop)
				$ret = 1
				EndIf
		EndSwitch
	Return $ret
EndFunc

; Sets the glass transparency of a zettoControl.

;        button   - zettoCtrlGlass ( zctrl, normal, hover,  down, disable )
;        progress - zettoCtrlGlass ( zctrl, normal, disable )

; <- focus state
Func zettoCtrlSetGlass($zctrl=-1,$normal=-1,$hover=-1,$down=-1,$disable=-1)
	If $zctrl == -1 Then $zctrl = $_ZETTO_CTRL_LAST
	If Not IsArray($zctrl) Then Return
	Local $num, $prop, $ret = 0
	
	If Not IsString($normal) Then $normal = '0x' & Hex($normal,2)
	If Not IsString($hover) Then $hover = '0x' & Hex($hover,2)
	If Not IsString($down) Then $down = '0x' & Hex($down,2)
	If Not IsString($disable) Then $disable = '0x' & Hex($disable,2)

	Switch $zctrl[$__ZETTO_CTRL_IDENTIFIER__]
		Case 'ZBTN'
			$num = __zetto_ButtonNum($zctrl)
			If $num <> -1 Then
				$prop = $_ZETTO_BUTTON_LIB[$num]
				If $normal <> -1 Then $prop[$__ZETTO_BUTTON_GLASS_NORMAL__] = $normal
				If $hover <> -1 Then $prop[$__ZETTO_BUTTON_GLASS_HOVER__] = $hover
				If $down <> -1 Then $prop[$__ZETTO_BUTTON_GLASS_DOWN__] = $down
				If $disable <> -1 Then $prop[$__ZETTO_BUTTON_GLASS_DISABLE__] = $disable
				If $normal <> -1 Or $hover <> -1 Or $down <> -1 Or $disable <> -1 Then
					$prop[$__ZETTO_BUTTON_GLASS__] = 1
					$_ZETTO_BUTTON_LIB[$num] = $prop
					__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__,$prop)
					EndIf
				$ret = 1
				EndIf
		Case 'ZBAR'
			$num = __zetto_ProgressNum($zctrl[$__ZETTO_CTRL_HANDLE__])
			If $num <> -1 Then
				$prop = $_ZETTO_PROGRESS_LIB[$num]
				If $normal <> -1 Then $prop[$__ZETTO_PROGRESS_GLASS_NORMAL__] = $normal
				If $hover <> -1 Then $prop[$__ZETTO_PROGRESS_GLASS_DISABLE__] = $hover
				If $normal <> -1 Or $hover <> -1 Then
					$prop[$__ZETTO_PROGRESS_GLASS__] = 1
					__freshprogress($prop)
					$_ZETTO_PROGRESS_LIB[$num] = $prop
					EndIf
				$ret = 1
			EndIf
		EndSwitch
	Return $ret
EndFunc

; Sets the glass transparency mode of a zettoControl.
Func zettoCtrlSetGlassMode($zctrl=-1,$glass=1)
	If $zctrl == -1 Then $zctrl = $_ZETTO_CTRL_LAST
	If Not IsArray($zctrl) Then Return
	Local $num, $prop, $ret = 0
	Switch $zctrl[$__ZETTO_CTRL_IDENTIFIER__]
		Case 'ZBTN'
			$num = __zetto_ButtonNum($zctrl)
			If $num <> -1 Then
				$prop = $_ZETTO_BUTTON_LIB[$num]
				If $glass <> $prop[$__ZETTO_BUTTON_GLASS__] Then
					If $glass Then
						$prop[$__ZETTO_BUTTON_GLASS__] = 1
						Else
						$prop[$__ZETTO_BUTTON_GLASS__] = 0
						EndIf
					$_ZETTO_BUTTON_LIB[$num] = $prop
					__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__,$prop)
					EndIf
				$ret = 1
				EndIf
		Case 'ZBAR'
			$num = __zetto_ProgressNum($zctrl[$__ZETTO_CTRL_HANDLE__])
			If $num <> -1 Then
				$prop = $_ZETTO_PROGRESS_LIB[$num]
				If $glass <> $prop[$__ZETTO_PROGRESS_GLASS__] Then
					If $glass Then
						$prop[$__ZETTO_PROGRESS_GLASS__] = 1
						Else
						$prop[$__ZETTO_PROGRESS_GLASS__] = 0
						EndIf
					$_ZETTO_PROGRESS_LIB[$num] = $prop
					__freshprogress($prop)
					EndIf
				$ret = 1
			EndIf
		EndSwitch
	Return $ret
EndFunc

;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

; zettoCtrlSetData
; zettoCtrlSetState
; zettoCtrlRead
; zettoCtrlDelete
; zettoCtrlGetID
; zettoCtrlGetHandle
; zettoCtrlGetType
; zettoCtrlSetCursor

; #FUNCTION# ===================================================================
; Name...........: zettoCtrlSetData
; Description ...: Modifies the data for a zettoControl.
; Syntax.........: zettoCtrlSetData( opt $zctrl, opt $data, opt $advanced )
; Parameters ....: $zctrl - The zettoControl identifier as returned by a
;                           zettoCtrlCreate... function.
;                  $data  - Data
;                  $advanced - Set extended information of a zettoControl.
;                              Button   0 Caption [default]
;                                       1 Toggle Status
;                              Progress 0 Value [default]
;                                       1 Caption & Preprocessor
;                                       2 Min Value
;                                       3 Max Value
;                              Timer    0 Status [default]
;                                       1 Interval
;                                       2 Name
;                                       note : Setting interval and timer name
;                                              not affected untill restarting
;                                              timer with called line..
;                                              zettoCtrlSetData($timer, False)
;                                              zettoCtrlSetData($timer, True)
; Return values .: Success - Returns 1.
;                  Failure - Returns 0 and generate error code.
; Author ........: Prasetyo Priadi
; Modified ......:
; Related .......:
; Link ..........:
; Example .......: Yes
; ==============================================================================
Func zettoCtrlSetData($zctrl=-1,$data='',$advanced=0)
	If $zctrl == -1 Then $zctrl = $_ZETTO_CTRL_LAST
	If Not IsArray($zctrl) Then Return SetError($ERROR_INVALID, 0, 0)
	Local $num, $opt, $prop, $ret = 0
	Switch $zctrl[$__ZETTO_CTRL_IDENTIFIER__]
		Case 'ZBTN'
			$num = __zetto_ButtonNum($zctrl)
			If $num <> -1 Then
				Switch $advanced
					Case 0
						$opt = $__ZETTO_BUTTON_CAPTION__
					Case 1
						$opt = $__ZETTO_BUTTON_TOGGLE__
					EndSwitch
				$prop = $_ZETTO_BUTTON_LIB[$num]
				If $prop[$opt] <> $data Then
					$prop[$opt] = $data
					If BitAND($prop[$__ZETTO_BUTTON_STYLE__],$SS_MODE_TOGGLE) And $advanced == 1 Then
						If $data == 0 Then __freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__,$prop)
						If $data == 1 Then __freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_DOWN__,$prop)
						Else
						If BitAND($prop[$__ZETTO_BUTTON_STYLE__],$SS_MODE_TOGGLE) And $prop[$__ZETTO_BUTTON_TOGGLE__] Then
							__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_DOWN__,$prop)
							Else
							__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__,$prop)
							EndIf
						EndIf
					$_ZETTO_BUTTON_LIB[$num] = $prop
					EndIf
				$ret = 1
				EndIf
		Case 'ZBAR'
			$num = __zetto_ProgressNum($zctrl[$__ZETTO_CTRL_HANDLE__])
			If $num <> -1 Then
				$prop = $_ZETTO_PROGRESS_LIB[$num]
				Switch $advanced
					Case 0
						If BitAND($prop[$__ZETTO_PROGRESS_STYLE__],$SS_MODE_BUSY) Then
							If $data Then
								If $_ZETTO_PROGRESS_BUSY == '' Then zettoCtrlSetData($_ZETTO_PROGRESS_HTIM, True)
								__zetto_addtolibrary_str($_ZETTO_PROGRESS_BUSY, $zctrl[$__ZETTO_CTRL_HANDLE__])
								Else
								__zetto_deltolibrary_str($_ZETTO_PROGRESS_BUSY, $zctrl[$__ZETTO_CTRL_HANDLE__])
								EndIf
							Return 1
							Else
							$opt = $__ZETTO_PROGRESS_VALUE__
							EndIf
					Case 1
						$opt = $__ZETTO_PROGRESS_CAPTION__
					Case 2
						$opt = $__ZETTO_PROGRESS_MIN__
					Case 3
						$opt = $__ZETTO_PROGRESS_MAX__
					EndSwitch
				If $prop[$opt] <> $data Then
					$prop[$opt] = $data
					__freshprogress($prop)
					$_ZETTO_PROGRESS_LIB[$num] = $prop
					EndIf
				$ret=1
				EndIf
		Case 'ZTMR'
			$num = __zetto_TimerNum($zctrl)
			If $num <> -1 Then
				$prop = $_ZETTO_TIMER_LIB[$num]
				Switch $advanced
					Case 0
						$opt = $__ZETTO_TIMER_ENABLE
					Case 1
						$opt = $__ZETTO_TIMER_INTERVAL
					Case 2
						$opt = $__ZETTO_TIMER_NAME
					EndSwitch
				If $advanced == 0 Then
					If $prop[$opt] <> $data Then
						$prop[$__ZETTO_TIMER_ENABLE] = $data
						If $data = True Then
							Local $callback = 0, $timer
							If $prop[$__ZETTO_TIMER_CALLBACK] == -1 Then
								$callback = DllCallbackRegister($prop[$__ZETTO_TIMER_NAME] & '_Timer', 'int', '')
								If $callback == 0 Then Return SetError($ERROR_FUNCNOEXIST, 0, 0)
								EndIf
							If $prop[$__ZETTO_TIMER_TIMER] == -1 And $callback <> 0 Then
								$timer = DllCall($DLL_USER32, 'uint', 'SetTimer', 'hwnd', 0, 'uint', 0, 'int', $prop[$__ZETTO_TIMER_INTERVAL], 'ptr', DllCallbackGetPtr($callback))
								If Not IsArray($timer) Then Return SetError($ERROR_USER32TIMER, 0, 0)
								$prop[$__ZETTO_TIMER_CALLBACK]	= $callback
								$prop[$__ZETTO_TIMER_TIMER]		= $timer[0]
								EndIf
							EndIf
						If $data = False Then
							If $prop[$__ZETTO_TIMER_TIMER] <> -1 Then
								DllCall($DLL_USER32, 'int', 'KillTimer', 'hwnd', 0 , 'uint', $prop[$__ZETTO_TIMER_TIMER])
								$prop[$__ZETTO_TIMER_TIMER] = -1
								EndIf
							If $prop[$__ZETTO_TIMER_CALLBACK] <> -1 Then
								DllCallbackFree($prop[$__ZETTO_TIMER_CALLBACK])
								$prop[$__ZETTO_TIMER_CALLBACK] = -1
								EndIf
							EndIf
						$_ZETTO_TIMER_LIB[$num] = $prop
						EndIf
					$ret = 1
					Else
					If $prop[$opt] <> $data Then
						$prop[$opt] = $data
						$_ZETTO_TIMER_LIB[$num] = $prop
						EndIf
					$ret = 1
					EndIf
				EndIf
		Case Else
			Return SetError($ERROR_UNKNOWN, 0, 0)
		EndSwitch
	If $num == -1 Then Return SetError($ERROR_CNEXIST, 0, 0)
	Return $ret
EndFunc

; #FUNCTION# ===================================================================
; Name...........: zettoCtrlSetState
; Description ...: Changes the state of a zettoControl.
; Syntax.........: zettoCtrlSetState ( opt $zctrl, $state )
; Parameters ....: $zctrl - The zettoControl identifier as returned by a
;                           zettoCtrlCreate... function.
;                  state  - Using state identical with GUICtrlSetState.
; Return values .: Success - Returns 1.
;                  Failure - Returns 0 and generate error code.
; Author ........: Prasetyo Priadi
; Modified ......:
; Related .......:
; Link ..........:
; Example .......: Yes
; ==============================================================================
Func zettoCtrlSetState($zctrl=-1,$state=-1)
	If $zctrl == -1 Then $zctrl = $_ZETTO_CTRL_LAST
	If Not IsArray($zctrl) Then Return SetError($ERROR_INVALID, 0, 0)
	Local $num, $prop, $ret = 0
	Switch $zctrl[$__ZETTO_CTRL_IDENTIFIER__]
		Case 'ZBTN'
			$num = __zetto_ButtonNum($zctrl)
			If $num <> -1 Then
				$prop = $_ZETTO_BUTTON_LIB[$num]
				If $state == $GUI_ENABLE Or $state == $GUI_DISABLE Then
					Local $boolean = 0
					If $state == $GUI_DISABLE Then $boolean = 1
					$prop[$__ZETTO_BUTTON_STATE_DISABLE__] = $boolean
					$_ZETTO_BUTTON_LIB[$num] = $prop
					$ret = GUICtrlSetState($zctrl[$__ZETTO_CTRL_HANDLE__],$state)
					__freshbutton($__ZETTO_SKIN_BUTTON_IMAGE_NORMAL__,$prop)
					Else
					$ret = GUICtrlSetState($zctrl[$__ZETTO_CTRL_HANDLE__],$state)
					EndIf
				EndIf
		Case 'ZBAR'
			$num = __zetto_ProgressNum($zctrl[$__ZETTO_CTRL_HANDLE__])
			If $num <> -1 Then
				$prop = $_ZETTO_PROGRESS_LIB[$num]
				If $state == $GUI_ENABLE Or $state == $GUI_DISABLE Then
					Local $boolean = 0
					If $state == $GUI_DISABLE Then $boolean = 1
					$prop[$__ZETTO_PROGRESS_DISABLE__]  = $boolean
					$_ZETTO_PROGRESS_LIB[$num] = $prop
					__freshprogress($prop)
					EndIf
				$ret = GUICtrlSetState($zctrl[$__ZETTO_CTRL_HANDLE__],$state)
				EndIf
		Case Else
			Return SetError($ERROR_UNKNOWN, 0, 0)
		EndSwitch
	If $num == -1 Then Return SetError($ERROR_CNEXIST, 0, 0)
	Return $ret
EndFunc

; #FUNCTION# ===================================================================
; Name...........: zettoCtrlRead
; Description ...: Read state or data of a zettoControl.
; Syntax.........: zettoCtrlRead ( opt $zctrl, opt $advanced )
; Parameters ....: $zctrl    - The zettoControl identifier as returned by a
;                              zettoCtrlCreate... function.
;                  $advanced - Read extended information of a zettoControl.
;                              Button   0 Caption [default]
;                                       1 Toggle Status
;                              Progress 0 Value [default]
;                                       1 Caption & Preprocessor
;                                       2 Min Value
;                                       3 Max Value
;                              Timer    0 Status [default]
;                                       1 Interval
;                                       2 Name
; Return values .: Success: Returns depending the zettoControl. 
;                  Failure: Returns 0 and generate error code.
; Author ........: Prasetyo Priadi
; Modified ......:
; Related .......:
; Link ..........:
; Example .......: Yes
; ==============================================================================
Func zettoCtrlRead($zctrl=-1,$advanced=0)
	If $zctrl == -1 Then $zctrl = $_ZETTO_CTRL_LAST
	If Not IsArray($zctrl) Then Return SetError($ERROR_INVALID, 0, 0)
	Local $num, $prop, $ret = 0, $opt
	Switch $zctrl[$__ZETTO_CTRL_IDENTIFIER__]
		Case 'ZBTN'
			$num = __zetto_ButtonNum($zctrl)
			If $num <> -1 Then
				Switch $advanced
					Case 0
						$opt = $__ZETTO_BUTTON_CAPTION__
					Case 1
						$opt = $__ZETTO_BUTTON_TOGGLE__
					EndSwitch
				$prop = $_ZETTO_BUTTON_LIB[$num]
				$ret = $prop[$opt]
				EndIf
		Case 'ZBAR'
			$num = __zetto_ProgressNum($zctrl[$__ZETTO_CTRL_HANDLE__])
			If $num <> -1 Then
				Switch $advanced
					Case 0
						$opt = $__ZETTO_PROGRESS_VALUE__
					Case 1
						$opt = $__ZETTO_PROGRESS_CAPTION__
					Case 2
						$opt = $__ZETTO_PROGRESS_MIN__
					Case 3
						$opt = $__ZETTO_PROGRESS_MAX__
					EndSwitch
				$prop = $_ZETTO_PROGRESS_LIB[$num]
				$ret = $prop[$opt]
				EndIf
		Case 'ZTMR'
			$num = __zetto_TimerNum($zctrl)
			If $num <> -1 Then
				Switch $advanced
					Case 0
						$opt = $__ZETTO_TIMER_ENABLE
					Case 1
						$opt = $__ZETTO_TIMER_INTERVAL
					Case 2
						$opt = $__ZETTO_TIMER_NAME
					EndSwitch
				$prop = $_ZETTO_TIMER_LIB[$num]
				$ret = $prop[$opt]
				EndIf
		Case Else
			Return SetError($ERROR_UNKNOWN, 0, 0)
		EndSwitch
	If $num == -1 Then Return SetError($ERROR_CNEXIST, 0, 0)
	Return $ret
EndFunc

; #FUNCTION# ===================================================================
; Name...........: zettoCtrlDelete
; Description ...: Deletes a zettoControl.
; Syntax.........: zettoCtrlDelete ( opt $zctrl )
; Parameters ....: $zctrl - The zettoControl identifier as returned by a
;                           zettoCtrlCreate... function.
; Return values .: Success - Returns 1.
;                  Failure - Returns 0 and generate error code.
; Author ........: Prasetyo Priadi
; Modified ......:
; Related .......:
; Link ..........:
; Example .......: Yes
; ==============================================================================
Func zettoCtrlDelete(ByRef $zctrl)
	If $zctrl == -1 Then $zctrl = $_ZETTO_CTRL_LAST
	If Not IsArray($zctrl) Then Return SetError($ERROR_INVALID, 0, 0)
	Local $num, $prop, $ret = 0
	Switch $zctrl[$__ZETTO_CTRL_IDENTIFIER__]
		Case 'ZBTN'
			$num = __zetto_ButtonNum($zctrl)
			If $num <> -1 Then
				$prop = $_ZETTO_BUTTON_LIB[$num]
				_ArrayDelete($_ZETTO_BUTTON_LIB,$num)
				If $_ZETTO_CTRL_HWND <> $prop[$__ZETTO_BUTTON_WHANDLE__] Then GUISwitch($prop[$__ZETTO_BUTTON_WHANDLE__])
				$ret = GUICtrlDelete($prop[$__ZETTO_BUTTON_HANDLE__])
				_GUICtrl_OnHoverRegister($prop[$__ZETTO_BUTTON_HANDLE__])
				If $_ZETTO_CTRL_HWND <> $prop[$__ZETTO_BUTTON_WHANDLE__] Then GUISwitch($_ZETTO_CTRL_HWND)
				EndIf
		Case 'ZBAR'
			$num = __zetto_ProgressNum($zctrl[$__ZETTO_CTRL_HANDLE__])
			If $num <> -1 Then
				$prop = $_ZETTO_PROGRESS_LIB[$num]
				_ArrayDelete($_ZETTO_PROGRESS_LIB,$num)
				If $_ZETTO_CTRL_HWND <> $prop[$__ZETTO_PROGRESS_WHANDLE__] Then GUISwitch($prop[$__ZETTO_PROGRESS_WHANDLE__])
				$ret = GUICtrlDelete($prop[$__ZETTO_PROGRESS_HANDLE__])
				If BitAND($prop[$__ZETTO_PROGRESS_STYLE__],$SS_MODE_BUSY) Then __zetto_deltolibrary_str($_ZETTO_PROGRESS_BUSY, $zctrl[$__ZETTO_CTRL_HANDLE__])
				If $_ZETTO_CTRL_HWND <> $prop[$__ZETTO_PROGRESS_WHANDLE__] Then GUISwitch($_ZETTO_CTRL_HWND)
				EndIf
		Case 'ZTMR'
			$num = __zetto_TimerNum($zctrl)
			If $num <> -1 Then
				$prop = $_ZETTO_TIMER_LIB[$num]
				If $prop[$__ZETTO_TIMER_ENABLE] == True Then zettoCtrlSetData($zctrl, False)
				_ArrayDelete($_ZETTO_TIMER_LIB,$num)
				$ret = 1
				EndIf
		Case Else
			Return SetError($ERROR_UNKNOWN, 0, 0)
		EndSwitch
	If $num == -1 Then
		Return SetError($ERROR_CNEXIST, 0, 0)
		Else
		$zctrl[$__ZETTO_CTRL_IDENTIFIER__] = 'NaN'
		$zctrl[$__ZETTO_CTRL_HANDLE__] = 'NaN'
		EndIf
	Return $ret
EndFunc

; #FUNCTION# ===================================================================
; Name...........: zettoCtrlGetID
; Description ...: Returns the ID for a zettoControl.
; Syntax.........: zettoCtrlGetID ( opt $zctrl )
; Parameters ....: $zctrl - The zettoControl identifier as returned by a
;                           zettoCtrlCreate... function.
; Return values .: Success - Returns the ID of the given zettoControl.
;                  Failure - Returns -1 and generate error code.
; Author ........: Prasetyo Priadi
; Modified ......:
; Related .......:
; Link ..........:
; Example .......: Yes
; ==============================================================================
Func zettoCtrlGetID($zctrl=-1)
	If $zctrl == -1 Then $zctrl = $_ZETTO_CTRL_LAST
	If Not IsArray($zctrl) Then Return SetError($ERROR_INVALID, 0, -1)
	Return $zctrl[$__ZETTO_CTRL_HANDLE__]
EndFunc

; #FUNCTION# ===================================================================
; Name...........: zettoCtrlGetHandle
; Description ...: Returns the handle for a zettoControl.
; Syntax.........: zettoCtrlGetHandle ( opt $zctrl )
; Parameters ....: $zctrl - The zettoControl identifier as returned by a
;                           zettoCtrlCreate... function.
; Return values .: Success - Returns the handle of the given zettoControl.
;                  Failure - Returns 0 and generate error code.
; Author ........: Prasetyo Priadi
; Modified ......:
; Related .......:
; Link ..........:
; Example .......: Yes
; ==============================================================================
Func zettoCtrlGetHandle($zctrl=-1)
	If $zctrl == -1 Then $zctrl = $_ZETTO_CTRL_LAST
	If Not IsArray($zctrl) Then Return SetError($ERROR_INVALID, 0, 0)
	Return GUICtrlGetHandle($zctrl[$__ZETTO_CTRL_HANDLE__])
EndFunc

; #FUNCTION# ===================================================================
; Name...........: zettoCtrlGetType
; Description ...: Get zettoControl type.
; Syntax.........: zettoCtrlGetType ( opt $zctrl )
; Parameters ....: $zctrl - The zettoControl identifier as returned by a
;                           zettoCtrlCreate... function.
; Return values .: Success - Returns a string representing the type.
;                  Failure - Returns 0 and generate error code.
; Author ........: Prasetyo Priadi
; Modified ......:
; Related .......:
; Link ..........:
; Example .......: Yes
; ==============================================================================
Func zettoCtrlGetType($zctrl=-1)
	If $zctrl == -1 Then $zctrl = $_ZETTO_CTRL_LAST
	If Not IsArray($zctrl) Then Return SetError($ERROR_INVALID, 0, 0)
	Local $ret = 0
	Switch $zctrl[$__ZETTO_CTRL_IDENTIFIER__]
		Case 'ZBTN'
			$ret = 'Button'
		Case 'ZBAR'
			$ret = 'Progress'
		Case 'ZTMR'
			$ret = 'Timer'
		Case Else
			$ret = 'Unknown'
		EndSwitch
	Return $ret
EndFunc

; #FUNCTION# ===================================================================
; Name...........: zettoCtrlSetCursor
; Description ...: Sets the mouse cursor icon for a particular zettoControl.
; Syntax.........: zettoCtrlSetCursor ( opt $zctrl, opt $cursorID )
; Parameters ....: $zctrl    - The zettoControl identifier as returned by a
;                              zettoCtrlCreate... function.
;                  $cursorID - cursor ID as used by Windows SetCursor API.
; Return values .: Success - Returns 1.
;                  Failure - Returns 0 and generate error code.
; Author ........: Prasetyo Priadi
; Modified ......:
; Related .......:
; Link ..........:
; Example .......: Yes
; ==============================================================================
Func zettoCtrlSetCursor($zctrl=-1, $cursorID=-1)
	If $zctrl == -1 Then $zctrl = $_ZETTO_CTRL_LAST
	If Not IsArray($zctrl) Then Return SetError($ERROR_INVALID, 0, 0)
	Return GUICtrlSetCursor($zctrl[$__ZETTO_CTRL_HANDLE__], $cursorID)
EndFunc

;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

; #FUNCTION# ===================================================================
; Name...........: zettoCtrlSetWindow
; Description ...: Initialize where zettoControl will be created.
; Syntax.........: zettoCtrlSetWindow ( byref $winhandle , opt $tabitemID )
; Parameters ....: $winhandle - The handle of the window.
;                  $tabitemID - controlID of the tabitem control to be selected.
; Return values .: Success - Returns the handle of the previously current.
;                  Failure - Returns 0 if there is not valid winhandle.
; Author ........: Prasetyo Priadi
; Modified ......:
; Related .......:
; Link ..........:
; Example .......: Yes
; ==============================================================================
Func zettoCtrlSetWindow(ByRef $winhandle, $tabitemID = -1)
	Local $ret = 0
	If IsHWnd($winhandle) Then
		If $tabitemID == -1 Then
			$ret = GUISwitch($winhandle)
			Else
			$ret = GUISwitch($winhandle, $tabitemID)
			EndIf
		$_ZETTO_CTRL_HWND = $winhandle
		EndIf
	Return $ret
EndFunc

; #FUNCTION# ===================================================================
; Name...........: zettoCtrlWindowClear
; Description ...: Clear zettoControl in current window.
; Syntax.........: zettoCtrlWindowClear ( $winhandle , opt $option, opt $delete )
; Parameters ....: $winhandle - The handle of the window.
;                  $option    - 0 : Clear All [default]
;                               1 : Clear All Button
;                               2 : Clear All Progress
;                  $delete    - 0 : Keep control on window
;                               1 : Delete control on window [default]
; Return values .: Success - Returns 1.
;                  Failure - Returns 0 if there is not valid winhandle.
; Author ........: Prasetyo Priadi
; Modified ......:
; Related .......:
; Link ..........:
; Example .......: Yes
; ==============================================================================
Func zettoCtrlWindowClear($winhandle=-1, $option=0, $delete=1)
	Local $prop, $b = 0, $p = 0
	Local $button, $progress
	
	If $winhandle == -1 Then $winhandle = $_ZETTO_CTRL_HWND
	If Not IsHWnd($winhandle) Then Return

	Switch $option
		Case 0
			$button = 1
			$progress = 1
		Case 1
			$button = 1
		Case 2
			$progress = 1
		EndSwitch
	
	If $button Then
		Do
			For $loop = 0 To UBound($_ZETTO_BUTTON_LIB)-1
				$prop = $_ZETTO_BUTTON_LIB[$loop]
				If $winhandle == $prop[$__ZETTO_BUTTON_WHANDLE__] Then
					_ArrayDelete($_ZETTO_BUTTON_LIB,$loop)
					If $delete Then
						If $winhandle <> $prop[$__ZETTO_BUTTON_WHANDLE__] Then GUISwitch($prop[$__ZETTO_BUTTON_WHANDLE__])
						GUICtrlDelete($prop[$__ZETTO_BUTTON_HANDLE__])
						If $winhandle <> $prop[$__ZETTO_BUTTON_WHANDLE__] Then GUISwitch($_ZETTO_CTRL_HWND)
						EndIf
					_GUICtrl_OnHoverRegister($prop[$__ZETTO_BUTTON_HANDLE__])
					ExitLoop
					EndIf
				Next
			$b += 0.1
		Until $b >= UBound($_ZETTO_BUTTON_LIB)
		EndIf
	
	If $progress Then
		Do
			For $loop = 0 To UBound($_ZETTO_PROGRESS_LIB)-1
				$prop = $_ZETTO_PROGRESS_LIB[$loop]
				If $winhandle == $prop[$__ZETTO_PROGRESS_WHANDLE__] Then
					_ArrayDelete($_ZETTO_PROGRESS_LIB,$loop)
					If $delete Then
						If $winhandle <> $prop[$__ZETTO_PROGRESS_WHANDLE__] Then GUISwitch($prop[$__ZETTO_PROGRESS_WHANDLE__])
						GUICtrlDelete($prop[$__ZETTO_PROGRESS_HANDLE__])
						If $winhandle <> $prop[$__ZETTO_PROGRESS_WHANDLE__] Then GUISwitch($_ZETTO_CTRL_HWND)
						EndIf
					ExitLoop
					EndIf
				Next
			$p += 0.1
		Until $p >= UBound($_ZETTO_PROGRESS_LIB)
		EndIf
	Return 1
EndFunc

; #FUNCTION# ===================================================================
; Name...........: zettoEventHandler
; Description ...: Listen event of zettoControl and retrieve string.
; Syntax.........: zettoEventHandler()
; Parameters ....:
; Return values .: Return string representing nameID of zettoControl plus suffix
;                  of event.
;                  nameid_MouseDown         - on Mouse Down
;                  nameid_MouseUp           - on Mouse Up
;                  nameid_MouseLeave        - on Mouse Leave
;                  nameid_MouseOver         - on Mouse Over
; Author ........: Prasetyo Priadi
; Modified ......:
; Related .......:
; Link ..........:
; Example .......: Yes
; ==============================================================================
Func zettoEventHandler()
	Local $ret = $_ZETTO_CTRL_EVENT
	$_ZETTO_CTRL_EVENT = ''
	Return $ret
EndFunc

; #FUNCTION# ===================================================================
; Name...........: zettoEventCaller
; Description ...: Listen event of zettoControl, retrieve string and call.
; Syntax.........: zettoEventCaller()
; Parameters ....: 
; Return values .: Return string representing nameID of zettoControl plus suffix
;                  of event. Details.. see zettoEventHandler.
; Author ........: Prasetyo Priadi
; Modified ......:
; Related .......:
; Link ..........:
; Example .......: Yes
; ==============================================================================
Func zettoEventCaller()
	Local $ret = $_ZETTO_CTRL_EVENT
	If $_ZETTO_CTRL_EVENT <> '' Then
		Call($ret)
		EndIf
	$_ZETTO_CTRL_EVENT = ''
	Return $ret
EndFunc

; by prazetto, replaced _GDIPlus_GraphicsDrawImage and don't ask why
Func _drawimage(ByRef $hGraphic,ByRef $hImage,$left,$top,$dispose=1)
	_GDIPlus_DrawImagePoints($hGraphic,$hImage,$left,$top,_GDIPlus_ImageGetWidth($hImage)+$left,$top,$left,_GDIPlus_ImageGetHeight($hImage)+$top)
	If $dispose Then _GDIPlus_ImageDispose($hImage)
EndFunc

; based to Yashield
Func _resizeimage(ByRef $hImage,$width,$height)
	Local $ret = DllCall($ghGDIPDll, 'int', 'GdipGetImageThumbnail', 'ptr', $hImage, 'int',$width, 'int',$height, 'ptr*', 0, 'ptr', 0, 'ptr', 0)
	If (Not @error) And ($Ret[0] = 0) Then
		_GDIPlus_ImageDispose($hImage)
		$hImage = $ret[4]
		EndIf
EndFunc

; Error    0 - Invalid control
;          1 - Failure create hBitmap from hImage
;          3 - Set image to controlfailed
Func __zetto_SetImage($chnd, $hImage, $tRect=0, $glass=0)
	If Not IsHWnd($chnd) Then $chnd = GUICtrlGetHandle($chnd)
	If Not IsHWnd($chnd) Then Return SetError(1,0,0)
	Local $hBitmap, $hBitmapL
	$hBitmap = _GDIPlus_BitmapCreateHBITMAPFromBitmap($hImage,0x00000000)
	If Not $hBitmap Then Return SetError(2,0,0)

	; ~~ $hBitmapL = _SendMessage($chnd, 0x0172, 0, $hBitmap)
	$hBitmapL = DllCall($DLL_USER32, 'lresult', "SendMessageW", "hwnd", $chnd, "uint", 0x0173, 'wparam', 0, 'lparam', 0)
	If Not @error Then
		; Save up RAM memory resource!!!
		; This line will delete latest object and give after image like if we write direct on window.
		If $hBitmapL[0] <> 0 Then DllCall($DLL_GDI32, "bool", "DeleteObject", "handle", $hBitmapL[0])
		EndIf

	; ~~ _SendMessage($chnd, 0x0172, 0, $hBitmap)
	DllCall($DLL_USER32, 'lresult', "SendMessageW", "hwnd", $chnd, "uint", 0x0172, 'wparam', 0, 'lparam', $hBitmap)
	If @error Then Return SetError(3,0,0)

	; Obviously Windows 2000 willn't show the image if object are deleted
	; ~~ If Not $WIN_2000 Then _WinAPI_DeleteObject($hBitmap)
	If Not $WIN_2000 Then DllCall($DLL_GDI32, "bool", "DeleteObject", "handle", $hBitmap)
	
	; Apply glass effect
	; Don't apply on normal, because will make blink rate
	If $glass Then
		; ~~ _WinAPI_InvalidateRect(_WinAPI_GetParent($chnd), $tRect)
		Local $parent = _zetto_GetParent($chnd)
		If @error Then Return SetError(4,0,0)
		DllCall($DLL_USER32, "bool", "InvalidateRect", "hwnd", $parent, "ptr", DllStructGetPtr($tRect), "bool", True)
		;DllCall($DLL_USER32, "bool", "InvalidateRgn", "hwnd", $parent, "ptr", DllStructGetPtr($tRect), "bool", True)
		;DllCall($DLL_USER32, "bool", "UpdateWindow", "hwnd", $parent)
		
		;Local $hDC = _WinAPI_GetDC($chnd)
		;SendMessage($chnd, 0x0014,  $hDC, 0)
		;_WinAPI_ReleaseDC($chnd, $hDC)
			
			
		If @error Then Return SetError(5,0,0)
		EndIf
EndFunc



Func SendMessage($hwnd, $msg, $wp, $lp)
	Local $ret
	$ret = DllCall($DLL_USER32, "long", "SendMessageA", "long", $hwnd, "int", $msg, "int", $wp, "int", $lp)
	If @error Then
		SetError(1)
		Return 0
	Else
		SetError(0)
		Return $ret[0]
	EndIf
	
EndFunc   ;==>SendMessage



Func _zetto_GetParent($hwnd)
	Local $ret = DllCall($DLL_USER32, 'hwnd', 'GetParent', 'hwnd', $hwnd)
	If @error Then Return SetError(1,0,0)
	Return $ret[0]
EndFunc

; #FUNCTION# ===================================================================
; Name...........: __zetto_FrameworkPNG
; Description ...: Load Transparent Blank PNG Image (PXF32ARGB).
; Syntax.........: __zetto_FrameworkPNG ( opt $image )
; Parameters ....: $image - Option of image retrieve.
; Return values .: Returns a handle to a new Bitmap object (hImage).
; Author ........: Prasetyo Priadi
; Modified ......:
; Related .......:
; Remarks .......: Don't ask why because this is the best solution than usage
;                  _WinAPI_CreateBitmap --> _GDIPlus_BitmapCreateFromHBITMAP -->
;                  _GDIPlus_BitmapCloneArea --> _GDIPlus_BitmapLockBits -->
;                  _GDIPlus_BitmapUnlockBits.
; Link ..........:
; Example .......: Yes
; ==============================================================================
Func __zetto_FrameworkPNG($image=0)
	Local $hImage
	Local $PNG800X600 = '0x78DAEB0CF073E7E592E2626060E0F5F47009626060566060608AE060038ACC2A6B2A00529C051E91C50C0CDCC220CCC8306B8E04505021D923C89781A14A9581A1A19981E1E77F20FD9281A1D48081E155020383D50C0606F1FC49F147DD1818B8967BBA388654DC7A73DD50908181E388C181FDCF8FB3324FF77871CD0F682183B81B1303C38106208B8D051BC340011BA38191111B8387031B23C181011B8399092F4342001B03EC2E4C0615DC4E4D4F90E67697D118188D81D118188D81D118188D81D118188D819114030FBA591918FEFF6766D0DDCBBEF66BF8BD24A03883A7AB9FCB3AA7842600C0CB0F15'
	Local $PNG1024X768 = '0x789CEB0CF073E7E592E2626060E0F5F470096260600132991938D880D4AE5DA2BC408AB3C023B29881815B1884191966CD91000A2A247B04F9323054A9323034343330FCFC0FA45F3230941A3030BC4A6060B09AC1C0209E3F29FEA81B038340AFA78B6348C5AD37371C051918380E1B1CD8FFFC61F2748F179B67A7004D6AC8536704522043191A402CC24C90E318121888611269A204F126D2C17052C282A68693101634359C94B0184D22A34964200D27252C686A384961D1C0424BD3691318A3D1389AD307D27052C282A6868F261134E6904C22120C4234359D2681311A8DA3397D200D1F4D22A349840013A2614B1590F8FF9F99E1420AC7DFC4875F1341829EAE7E2EEB9C129A00D03AEBFC'
	Switch $image
		Case 0
			 $PNG800X600 = _ZLIB_Uncompress($PNG800X600)
			$hImage = Load_BMP_From_Mem( $PNG800X600)
		Case 1
			$PNG1024X768 = _ZLIB_Uncompress($PNG1024X768)
			$hImage = Load_BMP_From_Mem($PNG1024X768)
		EndSwitch
	Return $hImage
EndFunc

; #FUNCTION# ===================================================================
; Name...........: __zettoCreatePNG
; Description ...: Create Transparent Blank PNG Image.
; Syntax.........: __zettoCreatePNG ( $width, $height, $format )
; Parameters ....: $width  - Specifies the png width in pixels 
;                  $height - Specifies the png height in pixels 
;                  $format - GDIP Pixel format Constant like $GDIP_PXF32ARGB
; Return values .: Returns a handle to a new Bitmap object (hImage).
; Author ........: Prasetyo Priadi
; Modified ......:
; Related .......:
; Link ..........:
; Example .......: Yes
; ==============================================================================
Func __zettoCreatePNG($width,$height,$format=0x0026200A) ; $GDIP_PXF32ARGB
	If $width > 800 Or $height > 600 Then
		If Not IsInt($_ZETTO_FRAMEWORK_PNG1024X768) Then $_ZETTO_FRAMEWORK_PNG1024X768 = __zetto_FrameworkPNG(1)
		Return _GDIPlus_BitmapCloneArea($_ZETTO_FRAMEWORK_PNG1024X768,0,0,$width,$height,$format)
		Else
		Return _GDIPlus_BitmapCloneArea($_ZETTO_FRAMEWORK_PNG800X600,0,0,$width,$height,$format)
		EndIf
EndFunc

; Preprocessing or image filter.
; For compatibilities with:
; - GlyFX Delphi 8.0 .Net Toolbar
;   which designed at 256 Color Bitmap, 8 Bpp Indexed
; - Windows 95 Toolbar which designed at 16 Color Bitmap, 4 Bpp Indexed
; - Visual Basic 6 Toolbar and etc.
Func __zetto_ImageFilter(ByRef $hImage)
	Local $format = _GDIPlus_ImageGetPixelFormat($hImage), $filter
	Local $width, $height, $hImageFilter, $hGraphicFilter, $hImageClone
	If IsArray($format) Then
		Switch $format[0]
			Case 196865 ; 1 Bpp Indexed | $GDIP_PXF01INDEXED
				$filter = 1
			Case 197634 ; 4 Bpp Indexed | $GDIP_PXF04INDEXED 
				$filter = 1
			Case 198659 ; 8 Bpp Indexed | $GDIP_PXF08INDEXED
				$filter = 1
			EndSwitch
		EndIf
	If $filter Then
		$width=_GDIPlus_ImageGetWidth($hImage)
		$height=_GDIPlus_ImageGetHeight($hImage)
		$hImageFilter = __zettoCreatePNG($width,$height)
		$hGraphicFilter = _GDIPlus_ImageGetGraphicsContext($hImageFilter)
		$hImageClone = _GDIPlus_BitmapCloneArea($hImage,0,0,$width,$height,$GDIP_PXF32ARGB)
		_drawimage($hGraphicFilter,$hImageClone,0,0)
		_GDIPlus_ImageDispose($hImage)
		_GDIPlus_ImageDispose($hImageClone)
		_GDIPlus_GraphicsDispose($hGraphicFilter)
		$hImage = $hImageFilter
		EndIf
EndFunc

; 0xDEAD = no masking [default]
; 0xDEFA = Mask color are pixel color on 1,1
; Original ImageColorToTransparent() by picea892
; $mask - Colour to be made transparent. Format $RGB
Func __zetto_BitmapMasking(ByRef $hImage, $mask = 0xDEAD, $alpha=0xFF)
    If Not IsString($alpha) Then $alpha = '0x' & StringRight(Hex($alpha,2),2)
	Local $hBitmap, $Reslt, $width, $height, $stride, $format, $Scan0, $buffer, $value, $iIW, $iIH
	$width = _GDIPlus_ImageGetWidth($hImage)
	$height = _GDIPlus_ImageGetHeight($hImage)
    $hBitmap = _GDIPlus_BitmapCloneArea($hImage, 0, 0, $width, $height, $GDIP_PXF32ARGB)
	_GDIPlus_ImageDispose($hImage)
    If $mask = 0xDEFA Then $mask = _GDIPlus_BitmapGetPixel($hBitmap, 1, 1)
    $Reslt = _GDIPlus_BitmapLockBits($hBitmap, 0, 0, $width, $height, BitOR($GDIP_ILMREAD, $GDIP_ILMWRITE), $GDIP_PXF32ARGB)
    $stride = DllStructGetData($Reslt, "stride")
    $Scan0 = DllStructGetData($Reslt, "Scan0")
    If $mask <> 0xDEAD Then
		For $i = 0 To $width - 1
			For $j = 0 To $height - 1
				$buffer = DllStructCreate("dword", $Scan0 + ($j * $stride) + ($i * 4))
				$value = DllStructGetData($buffer, 1)
				If Hex($value, 6) = Hex($mask, 6) Then
					DllStructSetData($buffer, 1, Hex($mask, 6))
					Else
					DllStructSetData($buffer, 1, $alpha & Hex($value, 6))
					EndIf
				Next
			Next
		Else
		For $i = 0 To $width - 1
			For $j = 0 To $height - 1
				$buffer = DllStructCreate("dword", $Scan0 + ($j * $stride) + ($i * 4))
				$value = DllStructGetData($buffer, 1)
				If Hex($value, 6) <> Hex(0, 6) Then
					DllStructSetData($buffer, 1, $alpha & Hex($value, 6))
					EndIf
				Next
			Next
		EndIf
	_GDIPlus_BitmapUnlockBits($hBitmap, $Reslt)
   $hImage = $hBitmap
EndFunc

;The GetPixel method gets the color of a specified pixel in this bitmap.
Func _GDIPlus_BitmapGetPixel($hBitmap, $iX, $iY)
    Local $tArgb, $pArgb, $aRet
    $tArgb = DllStructCreate("dword Argb")
    $pArgb = DllStructGetPtr($tArgb)
    $aRet = DllCall($ghGDIPDll, "int", "GdipBitmapGetPixel", "hwnd", $hBitmap, "int", $iX, "int", $iY, "ptr", $pArgb)
    Return "0x" & Hex(DllStructGetData($tArgb, "Argb"))
EndFunc  ;==>GDIPlus_BitmapGetPixel

Func _zetto_FlipHorizontal(ByRef $hImage, ByRef $hGraphic)
	Local $width, $height, $hMatrix, $hImageHoriz, $hGraphicHoriz
	$width = _GDIPlus_ImageGetWidth($hImage)
	$height = _GDIPlus_ImageGetHeight($hImage)
	$hMatrix = _GDIPlus_MatrixCreate2(-1.0,0.0,0.0,1.0, $width, 0)
	$hImageHoriz = __zettoCreatePNG($width, $height)
	$hGraphicHoriz = _GDIPlus_ImageGetGraphicsContext($hImageHoriz)
	_GDIPlus_GraphicsSetTransform($hGraphicHoriz, $hMatrix)
	_GDIPlus_GraphicsDrawImage($hGraphicHoriz, $hImage, 0, 0)
	_drawimage($hGraphic,$hImageHoriz,0,0)
	_GDIPlus_MatrixDispose($hMatrix)
	_GDIPlus_GraphicsDispose($hGraphicHoriz)
EndFunc

Func _zetto_FlipVertical(ByRef $hImage, ByRef $hGraphic)
	Local $width, $height, $hMatrix, $hImageVertic, $hGraphicVertic
	$width = _GDIPlus_ImageGetWidth($hImage)
	$height = _GDIPlus_ImageGetHeight($hImage)
	$hMatrix = _GDIPlus_MatrixCreate2(1.0,0.0,0.0,-1.0, 0, $height)
	$hImageVertic = __zettoCreatePNG($width, $height)
	$hGraphicVertic = _GDIPlus_ImageGetGraphicsContext($hImageVertic)
	_GDIPlus_GraphicsSetTransform($hGraphicVertic, $hMatrix)
	_GDIPlus_GraphicsDrawImage($hGraphicVertic, $hImage, 0, 0)
	_drawimage($hGraphic,$hImageVertic,0,0)
	_GDIPlus_MatrixDispose($hMatrix)
	_GDIPlus_GraphicsDispose($hGraphicVertic)
EndFunc

; zooming with piece per piece where the wonderfull beggining
; where art there is logic
; Format default $GDIP_PXF32ARGB = 0x0026200A ; 32 bpp; 8 bits for each RGB and alpha
	; $left     kiri_tengah
	; $right    kanan_tengah
	; $up       atas_tengah
	; $bottom   bawah_tengah
Func _zetto_SmartZoom(ByRef $hImage, $width, $height, $left=1, $right=1,$up=1,$bottom=1,$format=0x0026200A,$background=0,$dispose=1)
	Local $hImageSmart, $hGraphicSmart, $hImagePiece, $iWidth, $iHeight, $pass
	
	$iWidth = _GDIPlus_ImageGetWidth($hImage)
	$iHeight = _GDIPlus_ImageGetHeight($hImage)
	
	; size are identic then return
	If $width == $iWidth And $height == $iHeight Then Return
	
	$hImageSmart = __zettoCreatePNG($width, $height, $format)
	$hGraphicSmart = _GDIPlus_ImageGetGraphicsContext($hImageSmart)
	
	If $WIN_2000 Then
		If $background Then
			Local $hBrush = _GDIPlus_BrushCreateSolid(0xFF000000 + $WIN_FORM)
			_GDIPlus_GraphicsFillRect($hGraphicSmart, 0, 0, $width, $height,$hBrush)
			_GDIPlus_BrushDispose($hBrush)
			EndIf
		EndIf
	
	;kiri-atas
	$hImagePiece = _GDIPlus_BitmapCloneArea($hImage,0,0,$left,$up,$format)
	_drawimage($hGraphicSmart,$hImagePiece,0,0)
	
	;kiri-bawah
	$hImagePiece = _GDIPlus_BitmapCloneArea($hImage,0,$iHeight-$bottom,$left,$bottom,$format)
	_drawimage($hGraphicSmart,$hImagePiece,0,$height-$bottom)
	
	;kiri-tengah
	$hImagePiece = _GDIPlus_BitmapCloneArea($hImage,0,$up,$left,$iHeight-($up+$bottom),$format)
	_resizeimage($hImagePiece,_GDIPlus_ImageGetWidth($hImagePiece),$height-($up+$bottom))
	_drawimage($hGraphicSmart,$hImagePiece,0,$up)
	
	; width target more lower than border
	If ($left+$right) > $width Then
		$left /= 2
		$right /= 2
		$pass = 1
		EndIf
	
	;kanan-atas
	$hImagePiece = _GDIPlus_BitmapCloneArea($hImage,$iWidth-$right,0,$right,$up,$format)
	_drawimage($hGraphicSmart,$hImagePiece,$width-$right,0)
	
	;kanan-bawah
	$hImagePiece = _GDIPlus_BitmapCloneArea($hImage,$iWidth-$right,$iHeight-$bottom,$right,$bottom,$format)
	_drawimage($hGraphicSmart,$hImagePiece,$width-$right,$height-$bottom)

	;kanan-tengah
	$hImagePiece = _GDIPlus_BitmapCloneArea($hImage,$iWidth-$right,$bottom,$right,$iHeight-($up+$bottom+1),$format)
	_resizeimage($hImagePiece,_GDIPlus_ImageGetWidth($hImagePiece),$height-($up+$bottom))
	_drawimage($hGraphicSmart,$hImagePiece,$width-$right,$up)
	
	
	If ($left+$right) == $width Or $pass Then
		_GDIPlus_ImageDispose($hImage)
		_GDIPlus_GraphicsDispose($hGraphicSmart)
		$hImage = $hImageSmart
		Return
		EndIf
	
	;atas-tengah
	$hImagePiece = _GDIPlus_BitmapCloneArea($hImage,$left,0,$iWidth-($left+$right),$up,$format)
	_resizeimage($hImagePiece,$width-($left+$right),_GDIPlus_ImageGetHeight($hImagePiece))
	_drawimage($hGraphicSmart,$hImagePiece,$left,0)

	;bawah-tengah
	$hImagePiece = _GDIPlus_BitmapCloneArea($hImage,$left,$iHeight-$bottom,$iWidth-($left+$right),$bottom,$format)
	_resizeimage($hImagePiece,$width-($left+$right),_GDIPlus_ImageGetHeight($hImagePiece))
	_drawimage($hGraphicSmart,$hImagePiece,$left,$height-$bottom)
	
	;tengah
	$hImagePiece = _GDIPlus_BitmapCloneArea($hImage,$left,$up,$iWidth-($left+$right),$iHeight-($bottom+$up),$format)
	_resizeimage($hImagePiece,$width-($left+$right),$height-($bottom+$up))
	_drawimage($hGraphicSmart,$hImagePiece,$left,$up)
	
	If $dispose Then _GDIPlus_ImageDispose($hImage)
	_GDIPlus_GraphicsDispose($hGraphicSmart)
	$hImage = $hImageSmart

EndFunc

; by Matrix2, modify by prazetto
Func _GDIPlus_MatrixCreate2($nM11, $nM12, $nM21, $nM22, $nDX, $nDY)
    Local $ret, $iret
	$iret = DllCall($ghGDIPDll, "uint", "GdipCreateMatrix2", "float", $nM11, "float", $nM12, "float", $nM21, "float", $nM22, "float", $nDX, "float", $nDY, "int*", 0)
	If IsArray($iret) Then
		If Not $iret[0] Then $ret = $iret[7]
		EndIf
    Return $ret
EndFunc