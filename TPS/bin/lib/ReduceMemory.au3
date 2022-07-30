#include-once

Func ReduceMemory($ProcID = 0)
    ; Original version : w_Outer
    ; modified by Rajesh V R to include process ID, Prog@ndy, Merrik
    Local $ai_Return, $ai_Handle
    
	If $ProcID <= 0 Then ; No process id specified - use current process instead.
        $ai_Return = DllCall($DLL_PSAPI, 'int', 'EmptyWorkingSet', 'ptr', -1)
        If @error Then Return SetError(1, 0, 0)
        Return $ai_Return[0]
    EndIf

    $ProcID = ProcessExists($ProcID)
    If $ProcID = 0 Then Return SetError(2, 0, 0)
    $ai_Handle = DllCall($DLL_KERNEL32, 'ptr', 'OpenProcess', 'dword', 0x1f0fff, 'int', False, 'dword', $ProcID)
    If @error Or $ai_Handle[0] = 0 Then Return SetError(3, 0, 0)
    $ai_Return = DllCall($DLL_PSAPI, 'int', 'EmptyWorkingSet', 'ptr', $ai_Handle[0])
    If @error Then
        DllCall($DLL_KERNEL32, 'int', 'CloseHandle', 'ptr', $ai_Handle[0])
        Return SetError(4, 0, 0)
    EndIf
    DllCall($DLL_KERNEL32, 'int', 'CloseHandle', 'ptr', $ai_Handle[0])
    Return $ai_Return[0]
EndFunc   ;==>_ReduceMemory

Func ReduceMemoryAll()
	Local $list = ProcessList()
	If Not @error Then
		For $i = 1 To $list[0][0]
			ReduceMemory($list[$i][1])
		Next
		EndIf
EndFunc