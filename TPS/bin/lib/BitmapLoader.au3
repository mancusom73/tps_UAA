#include-once

Func Load_BMP_From_Mem($mem_image) ;coded by UEZ - thanks to progandy for the MemGlobalAlloc and tVARIANT lines
	Local $memBitmap, $len, $tMem, $hData, $pData, $aBitmap, $aStream, $tVARIANT, $aCall
	$memBitmap = Binary($mem_image) ;load image  saved in variable (memory) and convert it to binary
	$len = BinaryLen($memBitmap) ;get length of image
	$hData = _MemGlobalAlloc($len, $GMEM_MOVEABLE) ;allocates movable memory  ($GMEM_MOVEABLE = 0x0002)
	$pData = _MemGlobalLock($hData) ;translate the handle into a pointer
	$tMem = DllStructCreate("byte[" & $len & "]", $pData) ;create struct
	DllStructSetData($tMem, 1, $memBitmap) ;fill struct with image data
	_MemGlobalUnlock($hData) ;decrements the lock count  associated with a memory object that was allocated with GMEM_MOVEABLE
	$aStream = DllCall($DLL_OLE32, "int", "CreateStreamOnHGlobal", "handle", $pData, "int", True, "ptr*", 0)
	If @error Then Return SetError(@error, @extended, 0)
	$aBitmap = DllCall($ghGDIPDll, "uint", "GdipCreateBitmapFromStream", "ptr", $aStream[3], "int*", 0) ;Creates a Bitmap object based on an IStream COM interface
	If @error Then Return SetError(@error, @extended, 0)
	$tVARIANT = DllStructCreate("word vt;word r1;word r2;word r3;ptr data; ptr")
	$aCall = DllCall($DLL_OLEAUT32, "long", "DispCallFunc", "ptr", $aStream[3], "dword", 8 + 8 * @AutoItX64, _
			"dword", 4, "dword", 23, "dword", 0, "ptr", 0, "ptr", 0, "ptr", DllStructGetPtr($tVARIANT)) ;release memory from $hStream to avoid memory leak
	If @error Then Return SetError(@error, @extended, 0)
	$tMem = 0
	$tVARIANT = 0
	Return $aBitmap[2]
EndFunc   ;==>Load_BMP_From_Mem