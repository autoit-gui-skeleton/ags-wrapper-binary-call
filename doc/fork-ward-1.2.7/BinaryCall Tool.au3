; =============================================================================
;  AutoIt BinaryCall Tool 1.2 (2015.1.21)
;  Purpose: Convert GAS Assembly File To Binary Machine Code Script
;  Author: Ward
; =============================================================================

#Include <Array.au3>
#Include <File.au3>
#Include <GUIConstantsEx.au3>
#include <WindowsConstants.au3>

#Include "BinaryCall.au3"

Global $Title = "BinaryCall Tool 1.2"
Global $IniFile = @ScriptDir & "\BinaryCall Tool.ini"

Main()

Func Main()
	If @AutoItX64 Then Exit MsgBox(16, $Title, "AutoIt x86 Version Only !")
	Local $MainWin = GUICreate($Title, 450, 250, -1, -1, -1, $WS_EX_ACCEPTFILES)

	Local $Copyright = "Copyright © 2015 Ward"

	Local $Gas2Fasm = GUICtrlCreateButton("1: GAS2FASM Converter", 0, 0, 450, 50)
	GUICtrlSetFont(-1, 10, 900)
	GUICtrlSetState(-1, $GUI_DROPACCEPTED)
	Local $Fasm2Au3 = GUICtrlCreateButton("2: FASM2AU3 Converter", 0, 50, 450, 50)
	GUICtrlSetFont(-1, 10, 900)
	GUICtrlSetState(-1, $GUI_DROPACCEPTED)
	Local $Gas2Au3 = GUICtrlCreateButton("1+2: GAS2AU3 Converter", 0, 100, 450, 50, 1)
	GUICtrlSetFont(-1, 10, 900)
	GUICtrlSetState(-1, $GUI_FOCUS)
	GUICtrlSetState(-1, $GUI_DROPACCEPTED)
	Local $Bin2Au3 = GUICtrlCreateButton("XX: BIN2AU3 Converter", 0, 150, 450, 50)
	GUICtrlSetFont(-1, 10, 900)
	GUICtrlSetState(-1, $GUI_DROPACCEPTED)
	Local $Logo = GUICtrlCreateButton($Copyright, 0, 200, 450, 50)
	GUICtrlSetFont(-1, 10, 900)
	GUICtrlSetState(-1, $GUI_DISABLE)
	GUICtrlSetState(-1, $GUI_DROPACCEPTED)

	GUISetState(@SW_SHOW)

	Local $DragFile = Null
	While 1
		Switch GUIGetMsg()
		Case $Gas2Fasm
			Local $Filename = $DragFile ? $DragFile : FileOpenDialog("GAS2FASM Converter", @ScriptDir, "GASM Files (*.s)|All Files (*.*)", 1+2, "", $MainWin)
			If FileExists($Filename) Then
				GUICtrlSetData($Logo, "Processing ...")
				Local $Gas = FileRead($Filename)
				Local $Fasm = GasToFasm($Gas)
				Local $DefaultName = $Filename & ".asm"
				If StringRight($Filename, 2) = ".s" Then $DefaultName = StringTrimRight($Filename, 2) & ".asm"

				Local $Output = FileSaveDialog("GAS2FASM Converter", @ScriptDir, "FASM Files (*.asm)", 16, $DefaultName, $MainWin)
				If Not @Error Then
					TextFileWrite($Output, $Fasm)
					MsgBox(64, $Title, "GAS2FASM Done")
				EndIf
				GUICtrlSetData($Logo, $Copyright)
			EndIf
			$DragFile = Null

		Case $Fasm2Au3
			Local $Filename = $DragFile ? $DragFile : FileOpenDialog("FASM2AU3 Converter", @ScriptDir, "Assembly Files (*.asm)", 1+2, "", $MainWin)
			If FileExists($Filename) Then
				GUICtrlSetData($Logo, "Processing ...")
				Local $DllList = DllListGenerate($Filename)
				Local $Fasm = FileRead($Filename)
				Local $Clip = Fasm2Au3($Fasm, $DllList, True)
				If @Error Then MsgBox(16, $Title, "FASM2AU3 Fail")
				If $Clip Then
					ConsoleWrite($Clip)
					ClipPut($Clip)
					MsgBox(64, $Title, "FASM2AU3 Done")
				EndIf
				GUICtrlSetData($Logo, $Copyright)
			EndIf
			$DragFile = Null

		Case $Gas2Au3
			Local $Filename = $DragFile ? $DragFile : FileOpenDialog("GAS2AU3 Converter", @ScriptDir, "GASM Files (*.s)|All Files (*.*)", 1+2, "", $MainWin)
			If FileExists($Filename) Then
				GUICtrlSetData($Logo, "Processing ...")
				Local $DllList = DllListGenerate($Filename)
				Local $Gas = FileRead($Filename)
				Local $Fasm = GasToFasm($Gas)
				Local $Clip = Fasm2Au3($Fasm, $DllList, True)
				If @Error Then MsgBox(16, $Title, "FASM2AU3 Fail")
				If $Clip Then
					ConsoleWrite($Clip)
					ClipPut($Clip)
					MsgBox(64, $Title, "GAS2FASM Done")
				EndIf
				GUICtrlSetData($Logo, $Copyright)
			EndIf
			$DragFile = Null

		Case $Bin2Au3
			Local $Filename = $DragFile ? $DragFile : FileOpenDialog("BIN2AU3 Converter", @ScriptDir, "Binary Files (*.*)", 1+2, "", $MainWin)
			If FileExists($Filename) Then
				GUICtrlSetData($Logo, "Processing ...")
				Local $Binary = BinaryFileRead($Filename)
				Local $Compressed = LZMACompress($Binary, 9)
				Local $Clip = StringToVar(_BinaryCall_Base64Encode($Compressed), "Code")
				If $Clip Then
					$Clip &= 'Local $Binary = _BinaryCall_LzmaDecompress(_BinaryCall_Base64Decode($Code))' & @CRLF
					ConsoleWrite($Clip)
					ClipPut($Clip)
					MsgBox(64, $Title, "BIN2AU3 Done")
				EndIf
				GUICtrlSetData($Logo, $Copyright)
			EndIf
			$DragFile = Null

		Case $GUI_EVENT_DROPPED
			$DragFile = @GUI_DragFile

			Local $Drive = "", $Dir = "", $Filename = "", $Ext = ""
			_PathSplit($DragFile, $Drive, $Dir, $Filename, $Ext)

			Local $DoEvent = True
			Select
				Case @GUI_DropId = $Gas2Fasm And $Ext = ".s"
				Case @GUI_DropId = $Gas2Au3 And $Ext = ".s"
				Case @GUI_DropId = $Fasm2Au3 And $Ext = ".asm"
				Case @GUI_DropId = $Bin2Au3
				Case Else
					$DoEvent = False
			EndSelect
			If $DoEvent Then ControlClick($MainWin, "", GUICtrlGetHandle(@GUI_DropId))

		Case $GUI_EVENT_CLOSE
			ExitLoop

		EndSwitch
	WEnd
	GUIDelete()
	Exit
EndFunc

Func GasToFasm($Text)
	; Convert PC/UNIX format
	$Text = StringReplace($Text, @CRLF, @LF)

	Local $Match, $IsUse64 = StringRegExp($Text, "\s(r[a-d]x|r[sd]i|r[sb]p|r[89][dwb]?|r1[0-5][dwb]?)[\s,]") ? True : False

	; Convert .ascii string to serial number (To avoid be modified)
	Local $AsciiList = StringRegExp($Text, '(.ascii\s+"(.*)")', 3)
	For $i = 0 To UBound($AsciiList) - 1 Step 2
		$Text = StringReplace($Text, $AsciiList[$i], StringFormat(".ascii%06d", $i), 1, 1)
	Next

	; Create extern symbol list
	Local $ExternList[1] = [0]
	$Match = StringRegExp($Text, "\s+\.def\s*([^;]*);|___chkstk_ms|___chkstk", 3)
	For $i = 0 To UBound($Match) - 1
		Local $Symbol = $Match[$i]
		Local $API = StringRegExp($Symbol, "^_(.*)@\d+", 3)
		If IsArray($API) Then
			$Symbol = $API[0]
			$Text = StringReplace($Text, $Match[$i], $Symbol, 0, 1)
		EndIf

		If Not StringInStr($Text, @LF & $Symbol & ":", 1) Then
			If _ArraySearch($ExternList, $Symbol, 1, 0, 1) < 0 Then
				_ArrayAdd($ExternList, $Symbol)
				$ExternList[0] += 1
			EndIf
		EndIf
	Next

	; Create global symbol list
	Local $GlobalList = StringRegExp($Text, "\.globl[\s+](.*)", 3)
	If IsArray($GlobalList) Then
		_ArrayInsert($GlobalList, 0, UBound($GlobalList))
	Else
		Local $GlobalList[1] = [0]
	EndIf


	; Replace the directives that FASM not supported
	$Text = StringRegExpReplace($Text, "\t*\.(def.*\.endef|globl|intel_syntax|section|text|data|bss|p2align|ident|linkonce|seh.*|cfi.*).*[\r\n]+", @LF)

	; Replace the directives that FASM supported
	$Text = StringReplace($Text, ".align", "align", 0, 1)

	; Replace the labels that FASM not supported (.L1, .LC1, etc.)
	$Text = StringRegExpReplace($Text, "\.(LC?\d+)", "?\1")

	; Replace data definitions
	$Text = StringReplace($Text, ".byte", "db", 0, 1)
	$Text = StringReplace($Text, ".word", "dw", 0, 1)
	$Text = StringReplace($Text, ".long", "dd", 0, 1)
	$Text = StringReplace($Text, ".quad", "dq", 0, 1)
	$Text = StringReplace($Text, ".space", "rb", 0, 1)

	; Replace inline ASM
	$Text = StringRegExpReplace($Text, "\s#\s+\d+.*", "")
	$Text = StringReplace($Text, "/APP", @TAB & "; WARNING: inline ASM begin", 0, 1)
	$Text = StringReplace($Text, "/NO_APP", @TAB & "; WARNING: inline ASM end", 0, 1)

	; Replace static variables
	$Text = StringRegExpReplace($Text, "\.l?comm\s+([^,\s]+)\s*,\s*(\d+).*", "\1 rb \2")

	; Replace assembly syntax
	$Text = StringReplace($Text, "OFFSET FLAT:", "", 0, 1)
	$Text = StringReplace($Text, "movabs", "mov", 0, 1)
	$Text = StringReplace($Text, "bswapl", "bswap", 0, 1)

	; Replace extra square brackets (For example: call [DWORD PTR 16[edi]])
	$Text = StringRegExpReplace($Text, "(\s)(jmp|call)(\s+)\[(.*)\]", "\1\2\3\4")

	; Set shifts and rotate 1 bit by default
	$Text = StringRegExpReplace($Text, "(\s)(shl|shr|sal|sar|rol|ror\s+)([^,\r\n]+)[\r\n]", "\1\2\3, 1" & @LF)

	; Change constants sequence in square brackets
	$Text = StringRegExpReplace($Text, "(\s)([^\s]+)\[", "\1[\2+")

	; Replace extra rip for 64 bit ASM
	$Text = StringRegExpReplace($Text, "(\[.*)\+rip(.*])", "\1\2")

	; Replace PTR syntax
	$Text = StringRegExpReplace($Text, "(PTR\s+)([^\[][^\s,]*)", "\1[\2]")
	$Text = StringReplace($Text, "XMMWORD PTR", "xword", 0, 1)
	$Text = StringReplace($Text, "TBYTE PTR", "tbyte", 0, 1)
	$Text = StringReplace($Text, "QWORD PTR", "qword", 0, 1)
	$Text = StringReplace($Text, "DWORD PTR", "dword", 0, 1)
	$Text = StringReplace($Text, "WORD PTR", "word", 0, 1)
	$Text = StringReplace($Text, "BYTE PTR", "byte", 0, 1)

	; Replace movsx to movsxd
	$Text = StringRegExpReplace($Text, "(\s)movsx(\s+r[^,]+,\s*e[^\r\n]+)", "\1movsxd\2")
	$Text = StringRegExpReplace($Text, "(\s)movsx(\s+r[^,]+,\s*r\d+d)", "\1movsxd\2")
	$Text = StringRegExpReplace($Text, "(\s)movsx(\s+r[^,]+,\s*dword\s*\[)", "\1movsxd\2")

	; Replace st(0) to st0
	$Text = StringRegExpReplace($Text, "([\r\n]\s*f.*st)\(([0-7])\)", "\1\2")
	$Text = StringRegExpReplace($Text, "([\r\n]\s*f.*st)\(([0-7])\)", "\1\2")

	; Replace unused prefix
	$Text = StringRegExpReplace($Text, "rex\.W ", "")

	; Remove discard .refptr. label
	$Text = StringRegExpReplace($Text, "\.refptr\.([\w@]+):[\r\n]+\td.\t\1", "")

	; Add windows api to extern list
	$Match = StringRegExp($Text, "[^\.](__imp__?[\w@]+)", 3) ; add [^\.] to avoid .refptr.
	For $i = 0 To UBound($Match) - 1
		Local $API = StringRegExpReplace($Match[$i], "__imp__?|@.*", "")
		If StringRegExp($API, "^(__argc|__argv|__badioinfo|__initenv|__pioinfo|__wargv|__winitenv|_acmdln|_aexit_rtn|_commode|_daylight|_dstbias|_environ|_fileinfo|_fmode|_fpreset|_iob|_mbcasemap|_mbctype|_osplatform|_osver|_pctype|_pgmptr|_pwctype|_sys_errlist|_sys_nerr|_timezone|_tzname|_wcmdln|_wctype|_wenviron|_winmajor|_winminor|_winver|_wpgmptr)$") Then
			$Text = StringRegExpReplace($Text, "([dq]word)\s+\[" & $Match[$i] & "\]", "\1 [" & $API & "]")
		Else
			$Text = StringRegExpReplace($Text, "([dq]word)\s+\[" & $Match[$i] & "\]", "\1 " & $API)
		EndIf

		If Not StringInStr($Text, @LF & $API & ":", 1) Then
			If _ArraySearch($ExternList, $API, 1, 0, 1) < 0 Then
				_ArrayAdd($ExternList, $API)
				$ExternList[0] += 1
			EndIf
		EndIf
	Next

	; Add extern list to tail
	If $ExternList[0] Then $Text &= @LF & @LF & @TAB & "; extrn symbol list" & @LF
	For $i = 1 To $ExternList[0]
		$Text &= @TAB & $ExternList[$i] & ":" & @LF
	Next

	Local $Header = ""
	If $IsUse64 Then
		$Header = "use64"
	Else
		$Header = "use32"
	EndIf

	; Add global list to head
	If $GlobalList[0] Then $Header &= @LF & @LF & @TAB & "; global symbol list" & @LF & @LF
	For $i = 1 To $GlobalList[0]
		Local $Symbol = $GlobalList[$i]
		If Not $IsUse64 Then $Symbol = StringRegExpReplace($Symbol, "^_", "")

		If StringRegExp($Symbol, "^\.refptr\.") Then
			Local $NewSymbol = StringRegExpReplace($Symbol, "^\.refptr\.(__imp_)?", "")
			$Text = StringReplace($Text, $Symbol, $NewSymbol, 0, 1)
			$Text &= @TAB & $NewSymbol & ":" & @LF
		Else
			$Header &= StringFormat('\tdb "%s"\n\tjmp %s\n\n', $Symbol, $GlobalList[$i])
		EndIf
	Next

	; Add ZERO_PADDING macro
	$Header &= @LF &  "ZERO_PADDING:" & @LF

	$Text = StringRegExpReplace($Text, "\.file.*", $Header)

	; Replace '.ascii' string to 'db' after all done
	For $i = 0 To UBound($AsciiList) - 1 Step 2
		Local $DB = "db" & @TAB & AsciiToDB($AsciiList[$i + 1])
		$Text = StringReplace($Text, StringFormat(".ascii%06d", $i), $DB, 1, 1)
	Next
	Return $Text
EndFunc

Func AsciiToDB($String)
	Static $SymbolList
	If Not IsDllStruct($SymbolList) Then
		Local $Code = 'AwAAAARiAwAAAAAAAABcQfD555tIPPb/a31LbK6GqgcXE2KwIwwKdZXWItl+E+kw0EqKNp4VwFrBtWHq5Xf+bNgzIvK35Hw9w3H/V5Nck/mzqLEtsyLYbga6XklRACXP/lTCtfb0OA0/WA3w8VtnBh0yCZcXU+3I/v2txZPA5bHoW58XigCPaikstN/X12NKFlMsFmtY7Or/U7A8VJzpOHWL46T3RicR9jy5stnZBjmeMiHDb4r/XXBzi/EO4PqDg3cb/bUYZlmzBOhdwgaRGw/GBZJ/6KOPaqI5zb5vY9MFOXcuVUKxC9Z2nqFVYB+4dhrz0R12tcwOFcMAU41SuLExC+JIvn4JaRScuETY4x7CGcgftIS/g93yywcUukbYaAeUCQJxzBPiRFggDCZ5yDpG7CUMzvkW1jpiqP0/IUuR20s/01aDDn8CK73tf9s2OGMGswV4GS6WaJS1S4V6CcJrDvSMsXNvTAn5ye7AqQA9XEBC0x9ZcHo6hwx9HBN6vdkSdhUuz9s5s4J/3w+oSTjW7+ejxzE3IqiR/aLWs3C+4LFFKQA/GJlenp1IFGB1PvKDhLCoKzpG7tsLNsL23tBOm9WDQ2hw/YZZLhdHrl08S3VZ333y+XISJu6alLV748ZicTPnm8OmPJcUVKMmxK2sq1gVQfytr/QvzqChmAlh/8dV+J8yg8zl3d2JqqJk0SC79GKtslokhZRW5QiamEjyJ/hG3aN9YdhC5Gcyw5SxfPOboSwnQEXJYGgjxAGHNcFTzDHS/7MR6MX2FNu8ZU21X4OKWcCiGI6+tb+pizqmSI4ahIouuOGBMIA='
		Local $Symbol[] = ["AsciiToDB"]

		Local $CodeBase = _BinaryCall_Create($Code)
		If @Error Then Return SetError(1, 0, "")
		$SymbolList = _BinaryCall_SymbolList($CodeBase, $Symbol)
		If @Error Then Return SetError(1, 0, "")
	EndIf

	Local $Ret = DllCallAddress("ptr:cdecl", DllStructGetData($SymbolList, "AsciiToDB"), "str", $String)
	If @Error Or $Ret[0] = 0 Then Return SetError(1, 0, "")

	Local $Length = _BinaryCall_lstrlenA($Ret[0])
	Local $DB = DllStructGetData(DllStructCreate("char[" & $Length & "]", $Ret[0]), 1)
	DllCall($__BinaryCall_Msvcrtdll, "none:cdecl", "free", "ptr", $Ret[0])

	Return $DB
EndFunc

Func Fasm2Au3($Text, $DllList = Default, $IsCompress = True, $Inc = FileRead(@ScriptDir & '\BinaryCall.inc'))
	; FASM reserved word
	Static $RegExp = ",(aaa|aad|aam|aas|adc|adcx|add|addpd|addps|addsd|addss|addsubpd|addsubps|adox|aesdec|aesdeclast|aesenc|aesenclast|aesimc|aeskeygenassist|ah|al|align|and|andn|andnpd|andnps|andpd|andps|arpl|as|assert|at|ax|bextr|bh|binary|bl|blcfill|blci|blcic|blcmsk|blcs|blendpd|blendps|blendvpd|blendvps|blsfill|blsi|blsic|blsmsk|blsr|bound|bp|bpl|break|bsf|bsr|bswap|bt|btc|btr|bts|bx|byte|bzhi|call|cbw|cdq|cdqe|ch|cl|clac|clc|cld|clflush|clgi|cli|clts|cmc|cmova|cmovae|cmovb|cmovbe|cmovc|cmove|cmovg|cmovge|cmovl|cmovle|cmovna|cmovnae|cmovnb|cmovnbe|cmovnc|cmovne|cmovng|cmovnge|cmovnl|cmovnle|cmovno|cmovnp|cmovns|cmovnz|cmovo|cmovp|cmovpe|cmovpo|cmovs|cmovz|cmp|cmpeqpd|cmpeqps|cmpeqsd|cmpeqss|cmplepd|cmpleps|cmplesd|cmpless|cmpltpd|cmpltps|cmpltsd|cmpltss|cmpneqpd|cmpneqps|cmpneqsd|cmpneqss|cmpnlepd|cmpnleps|cmpnlesd|cmpnless|cmpnltpd|cmpnltps|cmpnltsd|cmpnltss|cmpordpd|cmpordps|cmpordsd|cmpordss|cmppd|cmpps|cmps|cmpsb|cmpsd|cmpsq|cmpss|cmpsw|cmpunordpd|cmpunordps|cmpunordsd|cmpunordss|cmpxchg|cmpxchg16b|cmpxchg8b|code|coff|comisd|comiss|common|console|cpuid|cqo|cr0|cr1|cr10|cr11|cr12|cr13|cr14|cr15|cr2|cr3|cr4|cr5|cr6|cr7|cr8|cr9|crc32|cs|cvtdq2pd|cvtdq2ps|cvtpd2dq|cvtpd2pi|cvtpd2ps|cvtpi2pd|cvtpi2ps|cvtps2dq|cvtps2pd|cvtps2pi|cvtsd2si|cvtsd2ss|cvtsi2sd|cvtsi2ss|cvtss2sd|cvtss2si|cvttpd2dq|cvttpd2pi|cvttps2dq|cvttps2pi|cvttsd2si|cvttss2si|cwd|cwde|cx|daa|das|data|db|dd|dec|define|defined|df|dh|di|dil|discardable|display|div|divpd|divps|divsd|divss|dl|dll|dp|dppd|dpps|dq|dqword|dr0|dr1|dr10|dr11|dr12|dr13|dr14|dr15|dr2|dr3|dr4|dr5|dr6|dr7|dr8|dr9|ds|dt|du|dup|dw|dword|dx|dynamic|eax|ebp|ebx|ecx|edi|edx|efi|efiboot|efiruntime|eip|elf|elf64|else|emms|end|enter|entry|eq|eqtype|err|es|esi|esp|executable|export|extractps|extrn|extrq|f2xm1|fabs|fadd|faddp|far|fbld|fbstp|fchs|fclex|fcmovb|fcmovbe|fcmove|fcmovnb|fcmovnbe|fcmovne|fcmovnu|fcmovu|fcom|fcomi|fcomip|fcomp|fcompp|fcos|fdecstp|fdisi|fdiv|fdivp|fdivr|fdivrp|femms|feni|ffree|ffreep|fiadd|ficom|ficomp|fidiv|fidivr|fild|file|fimul|fincstp|finit|fist|fistp|fisttp|fisub|fisubr|fixups|" & _
			"fld|fld1|fldcw|fldenv|fldenvd|fldenvw|fldl2e|fldl2t|fldlg2|fldln2|fldpi|fldz|fmul|fmulp|fnclex|fndisi|fneni|fninit|fnop|fnsave|fnsaved|fnsavew|fnstcw|fnstenv|fnstenvd|fnstenvw|fnstsw|format|forward|fpatan|fprem|fprem1|fptan|frndint|from|frstor|frstord|frstorw|frstpm|fs|fsave|fsaved|fsavew|fscale|fsetpm|fsin|fsincos|fsqrt|fst|fstcw|fstenv|fstenvd|fstenvw|fstp|fstsw|fsub|fsubp|fsubr|fsubrp|ftst|fucom|fucomi|fucomip|fucomp|fucompp|fwait|fword|fxam|fxch|fxrstor|fxrstor64|fxsave|fxsave64|fxtract|fyl2x|fyl2xp1|getsec|gs|gui|haddpd|haddps|heap|hlt|hsubpd|hsubps|icebp|idiv|if|import|imul|in|inc|include|ins|insb|insd|insertps|insertq|insw|int|int1|int3|interpreter|into|invd|invept|invlpg|invlpga|invpcid|invvpid|iret|iretd|iretq|iretw|irp|irps|irpv|ja|jae|jb|jbe|jc|jcxz|je|jecxz|jg|jge|jl|jle|jmp|jna|jnae|jnb|jnbe|jnc|jne|jng|jnge|jnl|jnle|jno|jnp|jns|jnz|jo|jp|jpe|jpo|jrcxz|js|jz|label|lahf|lar|large|lddqu|ldmxcsr|lds|lea|leave|les|lfence|lfs|lgdt|lgs|lidt|linkinfo|linkremove|lldt|llwpcb|lmsw|load|loadall286|loadall386|local|lock|lods|lodsb|lodsd|lodsq|lodsw|loop|loopd|loope|looped|loopeq|loopew|loopne|loopned|loopneq|loopnew|loopnz|loopnzd|loopnzq|loopnzw|loopq|loopw|loopz|loopzd|loopzq|loopzw|lsl|lss|ltr|lwpins|lwpval|lzcnt|macro|maskmovdqu|maskmovq|match|maxpd|maxps|maxsd|maxss|mfence|minpd|minps|minsd|minss|mm0|mm1|mm2|mm3|mm4|mm5|mm6|mm7|mod|monitor|mov|movapd|movaps|movbe|movd|movddup|movdq2q|movdqa|movdqu|movhlps|movhpd|movhps|movlhps|movlpd|movlps|movmskpd|movmskps|movntdq|movntdqa|movnti|movntpd|movntps|movntq|movntsd|movntss|movq|movq2dq|movs|movsb|movsd|movshdup|movsldup|movsq|movss|movsw|movsx|movsxd|movupd|movups|movzx|mpsadbw|ms|ms64|mul|mulpd|mulps|mulsd|mulss|mulx|mwait|mz|native|near|neg|nop|not|note|notpageable|nx|on|or|org|orpd|orps|out|outs|outsb|outsd|outsw|pabsb|pabsd|pabsw|packssdw|packsswb|packusdw|packuswb|paddb|paddd|paddq|paddsb|paddsw|paddusb|paddusw|paddw|palignr|pand|pandn|pause|pavgb|pavgusb|pavgw|pblendvb|pblendw|pclmulhqhdq|pclmulhqhqdq|pclmulhqlqdq|pclmullqhdq|pclmullqhqdq|pclmullqlqdq|" & _
			"pclmulqdq|pcmpeqb|pcmpeqd|pcmpeqq|pcmpeqw|pcmpestri|pcmpestrm|pcmpgtb|pcmpgtd|pcmpgtq|pcmpgtw|pcmpistri|pcmpistrm|pdep|pe|pe64|pext|pextrb|pextrd|pextrq|pextrw|pf2id|pf2iw|pfacc|pfadd|pfcmpeq|pfcmpge|pfcmpgt|pfmax|pfmin|pfmul|pfnacc|pfpnacc|pfrcp|pfrcpit1|pfrcpit2|pfrsqit1|pfrsqrt|pfsub|pfsubr|phaddd|phaddsw|phaddw|phminposuw|phsubd|phsubsw|phsubw|pi2fd|pi2fw|pinsrb|pinsrd|pinsrq|pinsrw|plt|pmaddubsw|pmaddwd|pmaxsb|pmaxsd|pmaxsw|pmaxub|pmaxud|pmaxuw|pminsb|pminsd|pminsw|pminub|pminud|pminuw|pmovmskb|pmovsxbd|pmovsxbq|pmovsxbw|pmovsxdq|pmovsxwd|pmovsxwq|pmovzxbd|pmovzxbq|pmovzxbw|pmovzxdq|pmovzxwd|pmovzxwq|pmuldq|pmulhrsw|pmulhrw|pmulhuw|pmulhw|pmulld|pmullw|pmuludq|pop|popa|popad|popaw|popcnt|popd|popf|popfd|popfq|popfw|popq|popw|por|postpone|prefetch|prefetchnta|prefetcht0|prefetcht1|prefetcht2|prefetchw|psadbw|pshufb|pshufd|pshufhw|pshuflw|pshufw|psignb|psignd|psignw|pslld|pslldq|psllq|psllw|psrad|psraw|psrld|psrldq|psrlq|psrlw|psubb|psubd|psubq|psubsb|psubsw|psubusb|psubusw|psubw|pswapd|ptest|ptr|public|punpckhbw|punpckhdq|punpckhqdq|punpckhwd|punpcklbw|punpckldq|punpcklqdq|punpcklwd|purge|push|pusha|pushad|pushaw|pushd|pushf|pushfd|pushfq|pushfw|pushq|pushw|pword|pxor|qqword|qword|r10|r10b|r10d|r10l|r10w|r11|r11b|r11d|r11l|r11w|r12|r12b|r12d|r12l|r12w|r13|r13b|r13d|r13l|r13w|r14|r14b|r14d|r14l|r14w|r15|r15b|r15d|r15l|r15w|r8|r8b|r8d|r8l|r8w|r9|r9b|r9d|r9l|r9w|rax|rb|rbp|rbx|rcl|rcpps|rcpss|rcr|rcx|rd|rdfsbase|rdgsbase|rdi|rdmsr|rdmsrq|rdpmc|rdrand|rdseed|rdtsc|rdtscp|rdx|readable|relativeto|rep|repe|repeat|repne|repnz|rept|repz|resource|restore|restruc|ret|retd|retf|retfd|retfq|retfw|retn|retnd|retnq|retnw|retq|retw|reverse|rf|rip|rol|ror|rorx|roundpd|roundps|roundsd|roundss|rp|rq|rsi|rsm|rsp|rsqrtps|rsqrtss|rt|rva|rw|sahf|sal|salc|sar|sarx|sbb|scas|scasb|scasd|scasq|scasw|section|segment|seta|setae|setalc|setb|setbe|setc|sete|setg|setge|setl|setle|setna|setnae|setnb|setnbe|setnc|setne|setng|setnge|setnl|setnle|setno|setnp|setns|setnz|seto|setp|setpe|setpo|sets|setz|sfence|sgdt|shareable|shl|shld|shlx|short|" & _
			"shr|shrd|shrx|shufpd|shufps|si|sidt|sil|skinit|sldt|slwpcb|smsw|sp|spl|sqrtpd|sqrtps|sqrtsd|sqrtss|ss|st|st0|st1|st2|st3|st4|st5|st6|st7|stac|stack|static|stc|std|stgi|sti|stmxcsr|store|stos|stosb|stosd|stosq|stosw|str|struc|sub|subpd|subps|subsd|subss|swapgs|syscall|sysenter|sysexit|sysexitq|sysret|sysretq|t1mskc|tbyte|test|times|tr0|tr1|tr2|tr3|tr4|tr5|tr6|tr7|tword|tzcnt|tzmsk|ucomisd|ucomiss|ud2|unpckhpd|unpckhps|unpcklpd|unpcklps|use16|use32|use64|used|vaddpd|vaddps|vaddsd|vaddss|vaddsubpd|vaddsubps|vaesdec|vaesdeclast|vaesenc|vaesenclast|vaesimc|vaeskeygenassist|vandnpd|vandnps|vandpd|vandps|vblendpd|vblendps|vblendvpd|vblendvps|vbroadcastf128|vbroadcasti128|vbroadcastsd|vbroadcastss|vcmpeq_ospd|vcmpeq_osps|vcmpeq_ossd|vcmpeq_osss|vcmpeq_uqpd|vcmpeq_uqps|vcmpeq_uqsd|vcmpeq_uqss|vcmpeq_uspd|vcmpeq_usps|vcmpeq_ussd|vcmpeq_usss|vcmpeqpd|vcmpeqps|vcmpeqsd|vcmpeqss|vcmpfalse_ospd|vcmpfalse_osps|vcmpfalse_ossd|vcmpfalse_osss|vcmpfalsepd|vcmpfalseps|vcmpfalsesd|vcmpfalsess|vcmpge_oqpd|vcmpge_oqps|vcmpge_oqsd|vcmpge_oqss|vcmpgepd|vcmpgeps|vcmpgesd|vcmpgess|vcmpgt_oqpd|vcmpgt_oqps|vcmpgt_oqsd|vcmpgt_oqss|vcmpgtpd|vcmpgtps|vcmpgtsd|vcmpgtss|vcmple_oqpd|vcmple_oqps|vcmple_oqsd|vcmple_oqss|vcmplepd|vcmpleps|vcmplesd|vcmpless|vcmplt_oqpd|vcmplt_oqps|vcmplt_oqsd|vcmplt_oqss|vcmpltpd|vcmpltps|vcmpltsd|vcmpltss|vcmpneq_oqpd|vcmpneq_oqps|vcmpneq_oqsd|vcmpneq_oqss|vcmpneq_ospd|vcmpneq_osps|vcmpneq_ossd|vcmpneq_osss|vcmpneq_uspd|vcmpneq_usps|vcmpneq_ussd|vcmpneq_usss|vcmpneqpd|vcmpneqps|vcmpneqsd|vcmpneqss|vcmpnge_uqpd|vcmpnge_uqps|vcmpnge_uqsd|vcmpnge_uqss|vcmpngepd|vcmpngeps|vcmpngesd|vcmpngess|vcmpngt_uqpd|vcmpngt_uqps|vcmpngt_uqsd|vcmpngt_uqss|vcmpngtpd|vcmpngtps|vcmpngtsd|vcmpngtss|vcmpnle_uqpd|vcmpnle_uqps|vcmpnle_uqsd|vcmpnle_uqss|vcmpnlepd|vcmpnleps|vcmpnlesd|vcmpnless|vcmpnlt_uqpd|vcmpnlt_uqps|vcmpnlt_uqsd|vcmpnlt_uqss|vcmpnltpd|vcmpnltps|vcmpnltsd|vcmpnltss|vcmpord_spd|vcmpord_sps|vcmpord_ssd|vcmpord_sss|vcmpordpd|vcmpordps|vcmpordsd|vcmpordss|vcmppd|vcmpps|vcmpsd|vcmpss|vcmptrue_uspd|vcmptrue_usps|vcmptrue_ussd|" & _
			"vcmptrue_usss|vcmptruepd|vcmptrueps|vcmptruesd|vcmptruess|vcmpunord_spd|vcmpunord_sps|vcmpunord_ssd|vcmpunord_sss|vcmpunordpd|vcmpunordps|vcmpunordsd|vcmpunordss|vcomisd|vcomiss|vcvtdq2pd|vcvtdq2ps|vcvtpd2dq|vcvtpd2ps|vcvtph2ps|vcvtps2dq|vcvtps2pd|vcvtps2ph|vcvtsd2si|vcvtsd2ss|vcvtsi2sd|vcvtsi2ss|vcvtss2sd|vcvtss2si|vcvttpd2dq|vcvttps2dq|vcvttsd2si|vcvttss2si|vdivpd|vdivps|vdivsd|vdivss|vdppd|vdpps|verr|verw|vextractf128|vextracti128|vextractps|vfmadd132pd|vfmadd132ps|vfmadd132sd|vfmadd132ss|vfmadd213pd|vfmadd213ps|vfmadd213sd|vfmadd213ss|vfmadd231pd|vfmadd231ps|vfmadd231sd|vfmadd231ss|vfmaddpd|vfmaddps|vfmaddsd|vfmaddss|vfmaddsub132pd|vfmaddsub132ps|vfmaddsub213pd|vfmaddsub213ps|vfmaddsub231pd|vfmaddsub231ps|vfmaddsubpd|vfmaddsubps|vfmsub132pd|vfmsub132ps|vfmsub132sd|vfmsub132ss|vfmsub213pd|vfmsub213ps|vfmsub213sd|vfmsub213ss|vfmsub231pd|vfmsub231ps|vfmsub231sd|vfmsub231ss|vfmsubadd132pd|vfmsubadd132ps|vfmsubadd213pd|vfmsubadd213ps|vfmsubadd231pd|vfmsubadd231ps|vfmsubaddpd|vfmsubaddps|vfmsubpd|vfmsubps|vfmsubsd|vfmsubss|vfnmadd132pd|vfnmadd132ps|vfnmadd132sd|vfnmadd132ss|vfnmadd213pd|vfnmadd213ps|vfnmadd213sd|vfnmadd213ss|vfnmadd231pd|vfnmadd231ps|vfnmadd231sd|vfnmadd231ss|vfnmaddpd|vfnmaddps|vfnmaddsd|vfnmaddss|vfnmsub132pd|vfnmsub132ps|vfnmsub132sd|vfnmsub132ss|vfnmsub213pd|vfnmsub213ps|vfnmsub213sd|vfnmsub213ss|vfnmsub231pd|vfnmsub231ps|vfnmsub231sd|vfnmsub231ss|vfnmsubpd|vfnmsubps|vfnmsubsd|vfnmsubss|vfrczpd|vfrczps|vfrczsd|vfrczss|vgatherdpd|vgatherdps|vgatherqpd|vgatherqps|vhaddpd|vhaddps|vhsubpd|vhsubps|vinsertf128|vinserti128|vinsertps|virtual|vlddqu|vldmxcsr|vmaskmovdqu|vmaskmovpd|vmaskmovps|vmaxpd|vmaxps|vmaxsd|vmaxss|vmcall|vmclear|vminpd|vminps|vminsd|vminss|vmlaunch|vmload|vmmcall|vmovapd|vmovaps|vmovd|vmovddup|vmovdqa|vmovdqu|vmovhlps|vmovhpd|vmovhps|vmovlhps|vmovlpd|vmovlps|vmovmskpd|vmovmskps|vmovntdq|vmovntdqa|vmovntpd|vmovntps|vmovq|vmovsd|vmovshdup|vmovsldup|vmovss|vmovupd|vmovups|vmpsadbw|vmptrld|vmptrst|vmread|vmresume|vmrun|vmsave|vmulpd|vmulps|vmulsd|vmulss|vmwrite|vmxoff|vmxon|vorpd|vorps|" & _
			"vpabsb|vpabsd|vpabsw|vpackssdw|vpacksswb|vpackusdw|vpackuswb|vpaddb|vpaddd|vpaddq|vpaddsb|vpaddsw|vpaddusb|vpaddusw|vpaddw|vpalignr|vpand|vpandn|vpavgb|vpavgw|vpblendd|vpblendvb|vpblendw|vpbroadcastb|vpbroadcastd|vpbroadcastq|vpbroadcastw|vpclmulhqhdq|vpclmulhqlqdq|vpclmullqhdq|vpclmullqlqdq|vpclmulqdq|vpcmov|vpcmpeqb|vpcmpeqd|vpcmpeqq|vpcmpeqw|vpcmpestri|vpcmpestrm|vpcmpgtb|vpcmpgtd|vpcmpgtq|vpcmpgtw|vpcmpistri|vpcmpistrm|vpcomb|vpcomd|vpcomeqb|vpcomeqd|vpcomeqq|vpcomequb|vpcomequd|vpcomequq|vpcomequw|vpcomeqw|vpcomfalseb|vpcomfalsed|vpcomfalseq|vpcomfalseub|vpcomfalseud|vpcomfalseuq|vpcomfalseuw|vpcomfalsew|vpcomgeb|vpcomged|vpcomgeq|vpcomgeub|vpcomgeud|vpcomgeuq|vpcomgeuw|vpcomgew|vpcomgtb|vpcomgtd|vpcomgtq|vpcomgtub|vpcomgtud|vpcomgtuq|vpcomgtuw|vpcomgtw|vpcomleb|vpcomled|vpcomleq|vpcomleub|vpcomleud|vpcomleuq|vpcomleuw|vpcomlew|vpcomltb|vpcomltd|vpcomltq|vpcomltub|vpcomltud|vpcomltuq|vpcomltuw|vpcomltw|vpcomneqb|vpcomneqd|vpcomneqq|vpcomnequb|vpcomnequd|vpcomnequq|vpcomnequw|vpcomneqw|vpcomq|vpcomtrueb|vpcomtrued|vpcomtrueq|vpcomtrueub|vpcomtrueud|vpcomtrueuq|vpcomtrueuw|vpcomtruew|vpcomub|vpcomud|vpcomuq|vpcomuw|vpcomw|vperm2f128|vperm2i128|vpermd|vpermil2pd|vpermil2ps|vpermilmo2pd|vpermilmo2ps|vpermilmz2pd|vpermilmz2ps|vpermilpd|vpermilps|vpermiltd2pd|vpermiltd2ps|vpermpd|vpermps|vpermq|vpextrb|vpextrd|vpextrq|vpextrw|vpgatherdd|vpgatherdq|vpgatherqd|vpgatherqq|vphaddbd|vphaddbq|vphaddbw|vphaddd|vphadddq|vphaddsw|vphaddubd|vphaddubq|vphaddubw|vphaddudq|vphadduwd|vphadduwq|vphaddw|vphaddwd|vphaddwq|vphminposuw|vphsubbw|vphsubd|vphsubdq|vphsubsw|vphsubw|vphsubwd|vpinsrb|vpinsrd|vpinsrq|vpinsrw|vpmacsdd|vpmacsdqh|vpmacsdql|vpmacssdd|vpmacssdqh|vpmacssdql|vpmacsswd|vpmacssww|vpmacswd|vpmacsww|vpmadcsswd|vpmadcswd|vpmaddubsw|vpmaddwd|vpmaskmovd|vpmaskmovq|vpmaxsb|vpmaxsd|vpmaxsw|vpmaxub|vpmaxud|vpmaxuw|vpminsb|vpminsd|vpminsw|vpminub|vpminud|vpminuw|vpmovmskb|vpmovsxbd|vpmovsxbq|vpmovsxbw|vpmovsxdq|vpmovsxwd|vpmovsxwq|vpmovzxbd|vpmovzxbq|vpmovzxbw|vpmovzxdq|vpmovzxwd|vpmovzxwq|vpmuldq|vpmulhrsw|vpmulhuw|vpmulhw|" & _
			"vpmulld|vpmullw|vpmuludq|vpor|vpperm|vprotb|vprotd|vprotq|vprotw|vpsadbw|vpshab|vpshad|vpshaq|vpshaw|vpshlb|vpshld|vpshlq|vpshlw|vpshufb|vpshufd|vpshufhw|vpshuflw|vpsignb|vpsignd|vpsignw|vpslld|vpslldq|vpsllq|vpsllvd|vpsllvq|vpsllw|vpsrad|vpsravd|vpsraw|vpsrld|vpsrldq|vpsrlq|vpsrlvd|vpsrlvq|vpsrlw|vpsubb|vpsubd|vpsubq|vpsubsb|vpsubsw|vpsubusb|vpsubusw|vpsubw|vptest|vpunpckhbw|vpunpckhdq|vpunpckhqdq|vpunpckhwd|vpunpcklbw|vpunpckldq|vpunpcklqdq|vpunpcklwd|vpxor|vrcpps|vrcpss|vroundpd|vroundps|vroundsd|vroundss|vrsqrtps|vrsqrtss|vshufpd|vshufps|vsqrtpd|vsqrtps|vsqrtsd|vsqrtss|vstmxcsr|vsubpd|vsubps|vsubsd|vsubss|vtestpd|vtestps|vucomisd|vucomiss|vunpckhpd|vunpckhps|vunpcklpd|vunpcklps|vxorpd|vxorps|vzeroall|vzeroupper|wait|wbinvd|wdm|while|word|wrfsbase|wrgsbase|writable|writeable|wrmsr|wrmsrq|xabort|xacquire|xadd|xbegin|xchg|xend|xgetbv|xlat|xlatb|xmm0|xmm1|xmm10|xmm11|xmm12|xmm13|xmm14|xmm15|xmm2|xmm3|xmm4|xmm5|xmm6|xmm7|xmm8|xmm9|xor|xorpd|xorps|xrelease|xrstor|xrstor64|xsave|xsave64|xsaveopt|xsaveopt64|xsetbv|xtest|xword|ymm0|ymm1|ymm10|ymm11|ymm12|ymm13|ymm14|ymm15|ymm2|ymm3|ymm4|ymm5|ymm6|ymm7|ymm8|ymm9|yword)([,\r\n])"
	Static $SystemDllCache = ObjCreate('Scripting.Dictionary')

	Local $Use64 = StringInStr($Text, "use64") ? True : False
	Local $Import = ""
	If IsArray($DllList) Then
		Local $SystemDir = (StringInStr(@OSArch, "64") And $Use64 ? @WindowsDir & "\sysnative" : @SystemDir)
		Local $Index = 10

		For $i = 0 To UBound($DllList) - 1
			Local $DllFile = $DllList[$i], $IsSystem = False
			If Not FileExists($DllFile) Then
				$DllFile = $SystemDir & "\" & $DllFile
				$IsSystem = True
			EndIf
			If Not FileExists($DllFile) Then ContinueLoop

			If $IsSystem And $SystemDllCache.Exists($DllFile) Then
				$Import &= $SystemDllCache.Item($DllFile)
				ContinueLoop
			EndIf

			Local $Drive = "", $Dir = "", $Filename = "", $Ext = ""
			_PathSplit($DllFile, $Drive, $Dir, $Filename, $Ext)
			Local $DllName = $Filename & $Ext
			Local $Export = ExportList(BinaryFileRead($DllFile))
			If @Error Then ContinueLoop

			If (@Extended And $Use64) Or (Not @Extended And Not $Use64) Then
				Local $Line = StringFormat("def_api %s,%d,%s\n", $DllName, $Index, $Export)
				Do
					$Line = StringRegExpReplace($Line, $RegExp, "\2")
				Until @Extended = 0

				If $IsSystem Then $SystemDllCache.Add($DllFile, $Line)
				$Import &= $Line
			EndIf
			$Index += 1
		Next
	EndIf

	Local $Source = ($Use64 ? "use64" : "use32") & @LF & $Inc & @LF & $Import & @LF & $Text
	Local $Clip = ($Use64 ? "If @AutoItX64 Then" : "If Not @AutoItX64 Then") & @CRLF
	Local $Symbol = ""

	Local $Match = StringRegExp($Text, '(?s)db\s+"([^"]+)"[\n\r]+\s+jmp\s+_?\1[\r\n]', 3)
	If IsArray($Match) Then $Symbol = ArrayToVar($Match, "Symbol", 2048)

	Local $Bin1 = Fasm($Source)
	If @Error Then Return SetError(1, FasmError($Bin1), "")
	Local $MemoryUsage = @Extended

	Local $Bin2 = Fasm("ORG 0x8080" & @LF & $Source, $MemoryUsage)
	If @Error Then Return SetError(1, FasmError($Bin2), "")
	If Not $Bin1 Or Not $Bin2 Then Return SetError(1, 0, "")

	If StringInStr($Text, "extrn symbol list") Then
		Local $ExternList = StringRegExpReplace($Text, "(?s).* extrn symbol list", "")
		Local $MacroList = StringRegExpReplace($ExternList, "([^\s]+):", "\1")

		Local $Bin3 = Fasm(StringReplace($Source, $ExternList, $MacroList), $MemoryUsage)
		If @Error Then
			If UBound($Bin3) = 4 Then
				Local $ErrorMsg = $Bin3[1]
				Local $ErrorLine = StringStripWS($Bin3[3], 3)

				If $ErrorMsg = "ILLEGAL_INSTRUCTION" Then
					MsgBox(16, $Title, StringFormat('Unresolved external symbol: "%s"', $ErrorLine))
				EndIf
			EndIf
			Return SetError(1, 0, "")
		EndIf
	EndIf

	Local $Reloc = ""
	If $Bin1 <> $Bin2 Then
		$Reloc = RelocationGenerate($Bin1, $Bin2)
		If @Error Then Return SetError(2, 0, "")
	EndIf

	If $IsCompress Then
		Local $Compressed = LZMACompress($Bin1, 9)
		$Clip &= @TAB & StringToVar(_BinaryCall_Base64Encode($Compressed), "Code")

		If $Reloc Then
			$Compressed = LZMACompress($Reloc, 9)
			$Clip &= @TAB & StringToVar(_BinaryCall_Base64Encode($Compressed), "Reloc")
		EndIf
	Else
		$Clip &= @TAB & StringToVar($Bin1, "Code")
		If $Reloc Then $Clip &= @TAB & StringToVar($Reloc, "Reloc")
	EndIf

	If $Symbol Then $Clip &= @TAB & $Symbol & @CRLF
	$Clip &= @CRLF & @TAB & 'Local $CodeBase = _BinaryCall_Create($Code' & ($Reloc ? ', $Reloc)' : ')')
	$Clip &= @CRLF & @TAB & 'If @Error Then Exit MsgBox(16, "_BinaryCall_Create Error", _BinaryCall_LastError())' & @CRLF

	If $Symbol Then
		$Clip &= @TAB & 'Local $SymbolList = _BinaryCall_SymbolList($CodeBase, $Symbol)'
		$Clip &= @CRLF & @TAB & 'If @Error Then Exit MsgBox(16, "_BinaryCall_SymbolList Error", _BinaryCall_LastError())' & @CRLF
	EndIf

	$Clip &= "EndIf" & @CRLF
	Return $Clip
EndFunc

Func Fasm($Source, $MaxStructSize = 65536, $PassesLimit = 100)
	Static $SymbolList

	If Not IsDllStruct($SymbolList) Then
		Local $Code = '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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'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' & _
			'dcnwBFm37fWWbh6hXYJDGkwndLIbOruBZPZAMqVbWZcRx1SRhwiVzBoyzVdD0thLlXSjtURcuJX/QrvpEDDI6wnsM2J45F8poie03+H8B7b9sjetbmXvBsG/VCT54zAZ57QoGkQZC1b6cFGEou4IsFhnjQN5BXyVapk44im1Ze9MWH8MPTPYuJy1H5QEIdrL10yVNT8uv5T1EaKjlcvs6Js5SdaxzSa+TwXN/8mhgck+sTDcMAglBsLX+EK3ypy3q2SBtpMUEv6AnG+QoEUUgM1glNEhZIZtrVHbAe9Z2VSyXgMJY/fpUSniaa12vt1N75cRHqqT/x6OCWpJ9d1qLeTPM5p8YvRL9J6KS31lYyw52EHlF5SSmTsvys5NToPHfkhNQ1nqecvX6DyX41HKLcmAgCcwy6y5hULcbOx8NuXWmaWkafCHOOLpWqBtNHleACYYR8PCNbraXPHDNnLeg8M+2QdY4OK0QDYB2siXs33DdFUNmBSigkpjKkAVUHjKFQh86mgv+PRskJ0ikbx9b1vfZySaPqY5zVft5IHNq64y39oaaQBihruBqKe7PIztwRpj4MSrO+HA1uK0CxNjWgRNqmas34TnsUwVMddiCGYNFmzJl1qLh6LCmxnUH3b/qrai9dshCdnlcm2Qb7wGslO6aR7mhS8Ei++hfzbomr1QtD2HrIVXTlt9d239IJYkEi7DmxQ9XtM5jI7Wp1tFzzm0Sa81itYPOaa5H8f34sHdvbuHwtd08SAFD4rtdBYX638iCL7YRaYIiI3ny3rD1p/Sas1NK7WQ4iLjuWxmNmqC4IOnJIeFE8f/AblHz7MslhZs6+5lxRwpDBPGDB5lvBUkx1svQAVPLYlQ9W4scFZOMezuJ9PXF9gz2/SlEcqvxGYTfAhz7CUO1NrTC7y5jZCdoL3/43cT7Kcpgz6B0lSkdygVnshg6Vk89PT6bLab/mEn1ExHyf0sXg7ILgIuSRgWljkfKuaPPUOKwf4tMA34qR7Vyn9ez59h9ptlmskCfe6HrFS2A9yyjMA0upimoWkGPnpjkfyYRz1MQYkZvUZUuczWq/fNJDyDuV8MRbXA8Tb3Z3dL3lQ0SsKRPqENrnNUkQfrBOXKIkz2HOxGWjh4aqyNiP6W4hi0f5OT1Gfx2rZO5htr+ORaFKJEinfagM5vetCfyH6Qlhr7SovByj7/QwiOsqsuWj6cpxjqgBfR3tiHLrudmyLFhswyGovX5e+V/UJq/YL9gueakX1jffatTsdExMu5kONk3AquBXOIT5sh2O+PpikHkvFKBTNXaNWp7zRhw3IFaiwmtXSZv7LilzgGTMwhB10pT8EoLBhfcPw3pFOxwasKvUw+OvthlI3xRv2AlroDcCohPokAKy89rcgcoUrAIG3+LUZYDyINIcADabIlkZ/YcEMv14ZbeLvhy+VpZE9pLeNga0P9cLGFsYtrEijP2AC4D4Wl8/gAe8QQxqcTjddPPXH5LMUy0WPWClYK7XAStQWRY8inzgcmv/aXKbF6c09UDjKMPvXztfgfxuEMvk1XiDAtPIE0JDuJgq6oQTn5IFSs5uDXeMZPjaM3bt6D1IhcXGHKAE2KZeePVXMzcNGnaO4bN2GGPyQQ+BDmUjq8+FUNaVD4Yqm2IU+BZhbRmgppVyvsNNQYw/AAjvOMOWFjGLQjMCadeWw7gUVhay3nrmO+uS3PB8Md7s2qnl48vJRNJJlO3oBfwe0hPV/+0tytbsXahH+pKLeca+1ed+ptn0+l999ydErzx024nXooKZ56dgkgHZ+/lgdFHri5OCRGsnp1tfDODocRPxwAUqBbjdXdOKELBESkL0+gfLQ8skBsW+riEHOmt7EXb4ebfa1yFtlnLqxrB3pUxk+lQDQjp4FqD2jFX5Q+QeTV1Nt6srJZFSofYcLFZVsoRAUA'
		Local $Reloc = 'AwAAAATGMQAAAAAAAAACABbRSPWxkDbpt1lcdhoEdpYa74OYnq/W5xDegDIpg4kQxas/l7wA9hyyy2j3IQqJQXbWALkAFirq/B/xCT9O1JvTNtpuKK/KeKLEyhgii9030iskDKiLID3GeDiUcL7ZY98V0vRPCwZp9DJ+7TduVXYfc64C+/RiL3LojrEbshy3SRpQrrVJVuiPI9kHoHbL9L/9+VNqD2AdUEM/TUVQxs+oopW+2WKbYVeZmCWdrn7IQQiTtJZ3sY6m8032X+QBuUKBy66+FIuERWQqXrF7ZMF8hNXEPxPx4x08vd9OaPYOejI7QzazpMJZvhH2P9ILVAzhhOobYhWBoXmPVBmBb1PZWfqrI3Tdeh9qv+7tb9wphSDi3xn4wCusmXiWWJ7Lb3Dbfg8klksb/j9nP8vqyinlgZtDMWhE7omZAQcQNoqC4DWE6YwaRX/1U15eIFir7H2/27/JTvUUNjxp3jcv/O3YAD5In2P91tFcB+1rKK635uCU0vD1vQYL/1V+gz+teWLsFFeo/3TkooLMR6/t5auGL4+DDO9AZMFP6g3RPXQeL9X7r1FRc91CMpCtLQEHNVe1jb9IyU6q3WcwChnoEXR+A0/R9cVQHKc5VeNH+Hax5MsSKvJvjakZ3Imi/Pwk9OY0XAJn7gCZ+FKBFcp1CxZi5tss3ucjTuUPys2GrTqnp/JvvCHKURFiRAvCEPGMgGDdVoItwyWc8kOrvilpRLzbxvnoaurFvC7LexYaU+S5lKm015xwiGF5FlkCooTHcUQUiyv8IwNWFg5VfSq49MMTR4cQ8yOtD+ZguWbWgqphAgfdQopzf2wJTs48Ax+s62qvlhWW0tiQ6or5stmnSvOQLTNgBIbyXuNUGnSCm+Y3M7JsLfIvms0DV7/4o1zv9JYUJpU5rxyOXWO0HGRi1d51UMrWiLnMyGcn3xhZmI2dRNsZy/eG+5ivbYRRboX2EZjcRB+lnUUDIen40sW3eqe47qVdQLg0frF1yEwcnqOWYT9JZ5IFYQoWS8CY2EmJEL2co0i6oxmTWcz9OLLe9g63kcM8ZKGrfpCIZFr375WAQAFKOn7Gtv5PEcZB6k5810GHCL7sziLMF1OcEl66ER3t1b2P7IstG0R+oYAFn7YEqp/K/+r42yWkF4B88o67sdYBItCweVMtEAnT0/gDMuiAPt+g4vZmixlRI5HorqE4WZzydb7zpswEIUHX8ru9TH+EFEAQBB9cQv+T1bOuvxHuahMnBveYQrNLBXvis8WZ5birA0WHlhWvjT2CdUuK7B0gRonU701m7VQ/zOIUqA1Ct9cbo5ofvwgRLDhsQO6l+Y8ycj4yMLUGeQ/ESPRA9nuBsuGS/RGzKX5oVoIavDSFEfr7456K8Q39B2zmUkBSekjxdT4ldYxCzUGvVI6xrYM/GpySTudQGHrfHHSYeP3fLmkO0RXu1c1eie40D5zI3jwujPjbHJWYdC1JfklaBcjuO3LG/bUZUrGj0No176WsyslsB+/FNjWn5oceIbIMz4POOGqN1dSOS0GERAXkO9D9wcxBzbs9PgtSOwn6ggeJ0hqrsALtSB0GovNL+5SpLJVK1EOo298TJ1mVVNTd+d1kGxH2EAjeQe6GMopvxL3mKohGECVs7v9NonTBX+p+Moyu7vy6Nxc05TDD1VFLyvKiAvVdM9WXwnY842imBfmHO6AhgfwrFGhE3v/zr1T+6WUMyBnT6J56tCsW/CFuGXMuaMnXAiWiLAx15FFOWElB59NtGU+N995GpYjo0zm0K1nz1UTUjIMGjXRFasPZnE1825l3vnnNgf7BDnG1e4hDuuncaJHKtPtVVFI2bWNHa/ddvfnH/KgjdO90xKRhm0/Wc/J8l4ArYuUOor1gNZUeoIh6KnIGlxUBfB54qv+2gwZH07H9kP4zId+B7bd98nUtFlb2UPu0mhzTA/Dcg0Crg+hmgbG11T1SJzp67vJ47ND4xlkWQJsOsYZmDtBii+1Yx4g/s4RpuDbo9EsH0WJ/IIOIM3ux2n4w5NIJvXyg' & _
			'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' & _
			'FABG86gHz4VZqk8e5lAqglWHUKiP2SlpRDO4n+YyQuvcu4ESRGyiz7qEcS4QpMDRg36ZeSIJ5tV5VEbO2WT+sF7E265YVO+IBJCHDeTcqN0D5bFL1uPxRfDrXxgdLj+b8x5+DL2/N2I+VxrqJ318DTfxVhiTRliAJeUsqsmx5r6JZyry/2za3XZMKIBeMlKe0MV6pNKBso3ICz8/rBLhZhNxoRcOgTPrI9ESVEkekklWVb83qKushN1xOrEuFUNoSnUxenKdyVpuZYCRL2gk2M9NuUdzdv3ObfHnamViMFfMJC9JFbpLFcyev6RG18XG1metxMIfs234X8UdUnhWj9XSJC2o8UcX60P+kuJ0syEsyHhaNHGmzFfoAaedZLvRFKhsg9HwnQ1awLiAP9KnmoRiXRVgarmBwVbgFvtDcl8D4ttEYSKRG8ErNHClGwew8pu2AA=='
		Local $Symbol[] = ["fasm_GetVersion","fasm_Assemble","fasm_AssembleFile"]

		Local $CodeBase = _BinaryCall_Create($Code, $Reloc)
		If @Error Then Return SetError(1, 0, Binary(""))
		$SymbolList = _BinaryCall_SymbolList($CodeBase, $Symbol)
		If @Error Then Return SetError(1, 0, Binary(""))
	EndIf

	Local $FasmState = DllStructCreate("int state;int length;ptr output;byte binary[" & $MaxStructSize & "]")
	Local $StructPtr = DllStructGetPtr($FasmState)
	Local $StructSize = DllStructGetSize($FasmState)

	Local $Ret = DllCallAddress("int", DllStructGetData($SymbolList, "fasm_Assemble"), "str", $Source, "ptr", $StructPtr, "uint", $StructSize, "uint", $PassesLimit, "ptr", 0)
	If @Error Then Return SetError(1, 0, Binary(""))

	If $Ret[0] = 0 Then
		Local $CodePtr = DllStructGetData($FasmState, "output")
		Local $CodeLen = DllStructGetData($FasmState, "length")
		Local $Offset = Number($CodePtr - $StructPtr) - 12
		Local $Binary = BinaryMid(DllStructGetData($FasmState, "binary"), $Offset + 1, $CodeLen)
		Return SetError(0, $MaxStructSize, $Binary)

	ElseIf $Ret[0] = -2 Then
		$FasmState = 0
		Local $Ret = Fasm($Source, ($MaxStructSize + 10) * 2, $PassesLimit)
		Return SetError(@Error, @Extended, $Ret)

	Else
		Local $ErrorCode = $Ret[0], $ErrorMsg, $ErrorLineNumber = -1, $ErrorLine = ""
		Local $ErrorList = [-1,"INVALID_PARAMETER",-2,"OUT_OF_MEMORY",-3,"STACK_OVERFLOW",-4,"SOURCE_NOT_FOUND",-5,"UNEXPECTED_END_OF_SOURCE",-6,"CANNOT_GENERATE_CODE",-7,"FORMAT_LIMITATIONS_EXCEDDED",-8,"WRITE_FAILED",-101,"FILE_NOT_FOUND",-102,"ERROR_READING_FILE",-103,"INVALID_FILE_FORMAT",-104,"INVALID_MACRO_ARGUMENTS",-105,"INCOMPLETE_MACRO",-106,"UNEXPECTED_CHARACTERS",-107,"INVALID_ARGUMENT",-108,"ILLEGAL_INSTRUCTION",-109,"INVALID_OPERAND",-110,"INVALID_OPERAND_SIZE",-111,"OPERAND_SIZE_NOT_SPECIFIED",-112,"OPERAND_SIZES_DO_NOT_MATCH",-113,"INVALID_ADDRESS_SIZE",-114,"ADDRESS_SIZES_DO_NOT_AGREE",-115,"DISALLOWED_COMBINATION_OF_REGISTERS",-116,"LONG_IMMEDIATE_NOT_ENCODABLE",-117,"RELATIVE_JUMP_OUT_OF_RANGE",-118,"INVALID_EXPRESSION",-119,"INVALID_ADDRESS",-120,"INVALID_VALUE",-121,"VALUE_OUT_OF_RANGE",-122,"UNDEFINED_SYMBOL",-123,"INVALID_USE_OF_SYMBOL",-124,"NAME_TOO_LONG",-125,"INVALID_NAME",-126,"RESERVED_WORD_USED_AS_SYMBOL",-127,"SYMBOL_ALREADY_DEFINED",-128,"MISSING_END_QUOTE",-129,"MISSING_END_DIRECTIVE",-130,"UNEXPECTED_INSTRUCTION",-131,"EXTRA_CHARACTERS_ON_LINE",-132,"SECTION_NOT_ALIGNED_ENOUGH",-133,"SETTING_ALREADY_SPECIFIED",-134,"DATA_ALREADY_DEFINED",-135,"TOO_MANY_REPEATS",-136,"SYMBOL_OUT_OF_SCOPE",-140,"USER_ERROR",-141,"ASSERTION_FAILED"]

		If $ErrorCode = 2 Then
			$ErrorCode = DllStructGetData($FasmState, "length")

			Local $LineHeader = DllStructCreate("ptr;int;uint;uint", Ptr(DllStructGetData($FasmState, "output")))
			If DllStructGetData($LineHeader, 2) < 0 Then
				Local $MacroLineHeader = DllStructCreate("ptr;int;uint;uint", Ptr(DllStructGetData($LineHeader, 4)))
				$LineHeader = $MacroLineHeader
			EndIf
			$ErrorLineNumber = DllStructGetData($LineHeader, 2)

			Local $Lines = StringSplit($Source, @LF)
			If $ErrorLineNumber > 0 And $ErrorLineNumber <= $Lines[0] Then
				$ErrorLine = $Lines[$ErrorLineNumber]
			EndIf
		EndIf

		For $i = 0 To UBound($ErrorList) - 1 Step 2
			If $ErrorCode = $ErrorList[$i] Then
				$ErrorMsg = $ErrorList[$i + 1]
				ExitLoop
			EndIf
		Next

		Local $Error[4] = [$ErrorCode, $ErrorMsg, $ErrorLineNumber, $ErrorLine]
		Return SetError($ErrorCode, $MaxStructSize, $Error)
	EndIf
EndFunc

Func FasmError($Error)
	If IsArray($Error) And UBound($Error) = 4 Then
		Local $ErrorCode = $Error[0], $ErrorMsg = $Error[1], $ErrorLineNumber = $Error[2], $ErrorLine = $Error[3]
		If $ErrorLine >= 0 Then $ErrorMsg &= @CRLF & "LINE NUMBER: " & $ErrorLineNumber & @CRLF & 'ERROR LINE: "' & $ErrorLine & '"'

		MsgBox(16, "Flat Assembler Error: " & $ErrorCode, $ErrorMsg)
	EndIf
EndFunc

Func RelocationGenerate($Binary1, $Binary2)
	If BinaryLen($Binary1) <> BinaryLen($Binary2) Then Return SetError(1, 0, Binary(""))

	Static $SymbolList
	If Not IsDllStruct($SymbolList) Then
		Local $Code = 'AwAAAASHAAAAAAAAAAApGhTPx+iIs8F6O5HXfzWUsoyq8W0AcEYpif6kUNxxoBoEvbdFLMDiDmCFO9oPQqEhU8Aqy9gd7+XUyB0cUPMaht0tjXIpFLNjwZR//XG+fsQqrWuLee9vJITpOlNZ743XxTW6sIRwZC6OrdbEkDDc6cxCTfLKWYfV3nuxuVkClLY7ovgIjAA='
		Local $Symbol[] = ["RelocationGenerate"]

		Local $CodeBase = _BinaryCall_Create($Code)
		If @Error Then Return SetError(1, 0, Binary(""))
		$SymbolList = _BinaryCall_SymbolList($CodeBase, $Symbol)
		If @Error Then Return SetError(1, 0, Binary(""))
	EndIf

	Local $Buffer1 = _BinaryCall_Alloc($Binary1)
	Local $Buffer2 = _BinaryCall_Alloc($Binary2)
	Local $Output = _BinaryCall_Alloc("", BinaryLen($Binary1) * 4 + 4)
	If $Buffer1 = 0 Or $Buffer2 = 0 Or $Output = 0 Then Return SetError(1, 0, Binary(""))

	Local $Reloc = Binary("")
	Local $Ret = DllCallAddress("uint:cdecl", DllStructGetData($SymbolList, "RelocationGenerate"), "ptr", $Buffer1, "uint", BinaryLen($Binary1), "ptr", $Buffer2, "uint", BinaryLen($Binary2), "ptr", $Output, "uint", BinaryLen($Binary1) * 4)
	If Not @Error And $Ret[0] Then
		Local $OutputBuffer = DllStructCreate("byte[" & $Ret[0] & "]", $Output)
		$Reloc = DllStructGetData($OutputBuffer, 1)
	EndIf

	_BinaryCall_Free($Buffer1)
	_BinaryCall_Free($Buffer2)
	_BinaryCall_Free($Output)

	Return $Reloc
EndFunc

Func LZMACompress($Data, $Level = 5)
	Static $SymbolList
	If Not IsDllStruct($SymbolList) Then
		Local $Code = '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' & _
			'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' & _
			'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' & _
			'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' & _
			'L88Kp16D5LyIRPq/pLiT0KEAMnTmksxjYEO0gOYqoSvJB8Ds7R/+5CsVCIWp05/LaQKETExdn4qBP5rlxVu5IE1VRy3bsLEIjMD+d1l0/ZjrsaQx5AQqEwhxv6PuOn+8tVznQ321G7G+LFFVQR8A5f3UP3/SJ24viwP0w+zpYqm/lcaVF0B8/f0d6X3G+P8WklNyF5A/a3qE1jObkEgTPdRsxmae8S3Ua32AAADGyAGwXs+U960jjvrMdkMjj5Y8zUNf+bfPxbNe46dS53CTFdjxTEoH+FqebkT8Y+V/RuhDjHevfGOb3Cpk4MMjN10ZJCsHU7dO8TtQzKEK4CgJcFPz/1qa7bQszHPAeUNZe6i834N0mfqnxjdU5Pj7+/8lohbYhTu8FPvLh+mBoFTDfV0YovXwcTNog/Bwce1djuAYeINdtJVhHp+th+Ks5RYDLEMov5o721CWWQPhYuWDp06vkSvCSM3g8oY6SS3iZVelS/zf5r5nuyluKBLuYKlc7ETzsGf0k31x7Nip12V7D98lZHFo6sbq3jwYkEsM0tJuesZ1iBfZzuJGzsPh7JzRNoBcimJ7vtwihlLvPcfK4qEUlQCFZIMn0ARtRQgyHJJILmuuzDYWWEK+StkLkAXl2iJtTyWskDj7VCuXnGmaGnU2fQDJKt/zT8ai96Gy9CNoaq4i+onxCaRx9kBG4KuKG5PlFsYErG54NvOHSSt2eBypHNvRfrrpsf/FP784j5sAEDDBfBThAs/XfdXZNUEzQnejhZv5XVyradMTeOsNFcnq3RVnt3Rnibv/gMRo6h9yMryIbABqrhYgMVBwksCl17+fiCyfjFFzO9MDiPfVf4flhmZlycXZLdrAj6Ist1EzUKxKi0PS7DBS9ZybdEzok3UuQfcOOhZXBFM5bNZ0sSCQTR9DN0K5yypNE9IukLTolxUW5Jdgr3BljztRKRMAXDryVvU4ESbfj7V6lJHLVV5sosyHofRHw0Ds0r0k7DqebvEnvCENg3m0fAD3nt0OYsVJvQ4MS8QfQQagPC2VrPr2bhCN1WZegstoJj1bAsQGWssYmHPH5bsbb38NJAAYMs+IqQIcjtoe7sd5E9d0lJqgeDJS3X3veIG1DE6ztj/lfHcXMl7jsPd2FGtMh9MjUBn6LVNh9iJpKS18arrn6rB5jc/enqrQ+Oy1m6W+nHYw5HHl85x9i1+NdlL8N+gxw6GFOluqmJWYF8nso2XWO5cg/XtqtQFRDHXZD0v62+gRFfWRpDby8cboI0CR53/+JnWQyyw6LOdz1rU4QUk1af2xA4Ur845IRFYGB0AGrdKCZ+2aHFeZiSG9ekQJvmnuiOTn7X0H7ELMRMzx3g6W5grDv2KgIpG//1Md9YtqrX5F0ICjFbV4dVmUYVmrHUn1paOy4brNBqKtCdlqnjFkzL97G6BBDnyVBIor/lazIbRocSW6FExuFpk7IFPeIzelm3myNGtm5LAlb9L6Bd2nZCvxV/lZLTnogpuLcvlQIOkyzo0wt3PcIZlhaLaRaneQpxYvyT8f52lmdewjPHtZIbrWnZ0nR4vOtoZOpyN4ORtezBCn+1yaa7nv/czd1EsLbIBIGIto0yc+pT3FRI/eR67vOQON/x/CmTK//O0E8Yq6HM9H+0O7HWWLK+WmBVEM4Vp2zX4L4pUSDTOPcC8hRaFahrntJkJ/vfl6SC8VnFeoEcvDA1WNOL7hTPh7Rv3HKkTAqvm/KFywQ9B+lnAOv6OFamuMNDoVMc7iK4Ao3qpTOqWec2O1vNYqrqwK6NtfLOZ9/5yEAEg+ke0KEnPwWaPVjdjkfkWh3o+zcYXQS2dIVkmlUueQ1bGPcKPuVdCmzcee5FnX37tvOKiup/hX7TqtPUXuEwWldU/S17IXpVSPZEMtwWOLoDTGkgT9EzBx8zuRDaXpy10iLJGjHsoJsd0Y9Kp5wSNpimZ6AKLuaaeUXXoT4hhbnthjpUpoYnhFHkYN/vAd8JHnwhdo2FzwWepQ5d0DQu4nnTiyzyIsuS/H5g6EuuhN0bNZ6MeHzQ3m' & _
			'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' & _
			'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' & _
			'bIC259si4FmjqEHY/4k1lOL05Tof6qxeRrAPf1MXpfvQuKjyerJrl+EyyEMwNX5+CE1lNHa6rjtWCaBQn+lXuSNr1vpaIeqb+nRB7FJX0WcE1H3feHXCkh6N9fX8gEuDcowxSeibRmjQ2DdqrYOJBgTKh7hm8rigBwex9uQgVGI9KILdbzoFqlgzrdy4/AhjgsLAEee2ik9hmYwQ9JTZzhd0ZR7V8i0orBG608L4dJpNH4G+dGP73uY5mp4raH0BxLPyRhFCtDxgfsMk/hgfE4WJWfFmxu9Tw/QlspxfEA9MxzXavEy9/Sf3cg8WAM1p/UaKRqzqVIHYOXvyqiQ0Dh5JxDdUSO2FaGq14v1US682tCPb'
		Local $Reloc = 'AwAAAARQAAAAAAAAAAABACKScWZYwJmYCZmPE1QkN6mx3PrTXYj9qObkEF/bkqfbmUMe/ZKVDGiDJlQfLU7fsbiZgCZCSAWmZ66IjREkPrwDglavL23uGQ+CXS3bSmjK4sVP'
		Local $Symbol[] = ["LzmaCompress"]

		Local $CodeBase = _BinaryCall_Create($Code, $Reloc)
		If @Error Then Return SetError(1, 0, Binary(''))
		$SymbolList = _BinaryCall_SymbolList($CodeBase, $Symbol)
		If @Error Then Return SetError(1, 0, Binary(''))
	EndIf

	$Data = Binary($Data)
	Local $InputLen = BinaryLen($Data)
	Local $Input = DllStructCreate("byte[" & $InputLen & "]")
	DllStructSetData($Input, 1, $Data)

	Local $OutputLen = $InputLen + 1024
	Local $Output = DllStructCreate("byte[" & $OutputLen & "]")

	Local $Prop = DllStructCreate("int level;uint dictSize;int lc;int lp;int pb;int algo;int fb;int btMode;int numHashBytes;uint mc;int writeEndMark;int numThreads")
	DllStructSetData($Prop, "level", $Level)
	DllStructSetData($Prop, "dictSize", 0)
	DllStructSetData($Prop, "lc", -1)
	DllStructSetData($Prop, "lp", -1)
	DllStructSetData($Prop, "algo", -1)
	DllStructSetData($Prop, "fb", -1)
	DllStructSetData($Prop, "btMode", -1)
	DllStructSetData($Prop, "numHashBytes", -1)
	DllStructSetData($Prop, "mc", -1)
	DllStructSetData($Prop, "writeEndMark", -1)
	DllStructSetData($Prop, "numThreads", 1)

	Local $PropSize = DllStructCreate("uint")
	DllStructSetData($PropSize, 1, 5)

	Local $Ret = DllCallAddress("uint:cdecl", DllStructGetData($SymbolList, "LzmaCompress"), _
					"ptr", DllStructGetPtr($Output) + 13, _
					"uint*", $OutputLen - 13, _
					"ptr", DllStructGetPtr($Input), _
					"uint", $InputLen, _
					"ptr", DllStructGetPtr($Prop), _
					"ptr", DllStructGetPtr($Output), _
					"uint*", DllStructGetPtr($PropSize))

	If @Error Or $Ret[0] <> 0 Then Return SetError(2, 0, Binary(''))

	DllStructSetData(DllStructCreate("uint64", DllStructGetPtr($Output) + 5), 1, $InputLen)
	Return BinaryMid(DllStructGetData($Output, 1), 1, $Ret[2] + 13)
EndFunc

Func ExportList($Binary, $Split = ",")
	Local Const $tagIMAGE_DOS_HEADER =  "WORD e_magic;WORD e_cblp;WORD e_cp;WORD e_crlc;WORD e_cparhdr;WORD e_minalloc;WORD e_maxalloc;WORD e_ss;WORD e_sp;WORD e_csum;WORD e_ip;WORD e_cs;WORD e_lfarlc;WORD e_ovno;WORD e_res[4];WORD e_oemid;WORD e_oeminfo;WORD e_res2[10];LONG e_lfanew;"
	Local Const $tagIMAGE_FILE_HEADER = "WORD Machine;WORD NumberOfSections;DWORD TimeDateStamp;DWORD PointerToSymbolTable;DWORD NumberOfSymbols;WORD SizeOfOptionalHeader;WORD Characteristics;"
	Local $tagIMAGE_OPTIONAL_HEADER32 = "WORD Magic;BYTE MajorLinkerVersion;BYTE MinorLinkerVersion;DWORD SizeOfCode;DWORD SizeOfInitializedData;DWORD SizeOfUninitializedData;DWORD AddressOfEntryPoint;DWORD BaseOfCode;DWORD BaseOfData;uint ImageBase;DWORD SectionAlignment;DWORD FileAlignment;WORD MajorOperatingSystemVersion;WORD MinorOperatingSystemVersion;WORD MajorImageVersion;WORD MinorImageVersion;WORD MajorSubsystemVersion;WORD MinorSubsystemVersion;DWORD Win32VersionValue;DWORD SizeOfImage;DWORD SizeOfHeaders;DWORD CheckSum;WORD Subsystem;WORD DllCharacteristics;uint SizeOfStackReserve;uint SizeOfStackCommit;uint SizeOfHeapReserve;uint SizeOfHeapCommit;DWORD LoaderFlags;DWORD NumberOfRvaAndSizes;"
	Local $tagIMAGE_OPTIONAL_HEADER64 = "WORD Magic;BYTE MajorLinkerVersion;BYTE MinorLinkerVersion;DWORD SizeOfCode;DWORD SizeOfInitializedData;DWORD SizeOfUninitializedData;DWORD AddressOfEntryPoint;DWORD BaseOfCode;uint64 ImageBase;DWORD SectionAlignment;DWORD FileAlignment;WORD MajorOperatingSystemVersion;WORD MinorOperatingSystemVersion;WORD MajorImageVersion;WORD MinorImageVersion;WORD MajorSubsystemVersion;WORD MinorSubsystemVersion;DWORD Win32VersionValue;DWORD SizeOfImage;DWORD SizeOfHeaders;DWORD CheckSum;WORD Subsystem;WORD DllCharacteristics;uint64 SizeOfStackReserve;uint64 SizeOfStackCommit;uint64 SizeOfHeapReserve;uint64 SizeOfHeapCommit;DWORD LoaderFlags;DWORD NumberOfRvaAndSizes;"
	Local Const $tagIMAGE_NT_HEADER32 = "DWORD Signature;" & $tagIMAGE_FILE_HEADER & $tagIMAGE_OPTIONAL_HEADER32
	Local Const $tagIMAGE_NT_HEADER64 = "DWORD Signature;" & $tagIMAGE_FILE_HEADER & $tagIMAGE_OPTIONAL_HEADER64
	Local Const $tagIMAGE_SECTION_HEADER = "CHAR Name[8];DWORD VirtualSize;DWORD VirtualAddress;DWORD SizeOfRawData;DWORD PointerToRawData;DWORD PointerToRelocations;DWORD PointerToLinenumbers;WORD NumberOfRelocations;WORD NumberOfLinenumbers;DWORD Characteristics;"
	Local Const $tagIMAGE_DATA_DIRECTORY = "DWORD VirtualAddress;DWORD Size;"
	Local Const $tagIMAGE_EXPORT_DIRECTORY = "DWORD Characteristics;DWORD TimeDateStamp;WORD MajorVersion;WORD MinorVersion;DWORD Name;DWORD Base;DWORD NumberOfFunctions;DWORD NumberOfNames;DWORD AddressOfFunctions;DWORD AddressOfNames;DWORD AddressOfNameOrdinals;"
	Local Const $IMAGE_DIRECTORY_ENTRY_EXPORT = 0

	Local $Buffer = DllStructCreate("byte[" & BinaryLen($Binary) & "]")
	DllStructSetData($Buffer, 1, $Binary)
	Local $Base = DllStructGetPtr($Buffer)

	Local $IMAGE_DOS_HEADER = DllStructCreate($tagIMAGE_DOS_HEADER, $Base)
	If DllStructGetData($IMAGE_DOS_HEADER, "e_magic") <> 0x5A4D Then Return SetError(1, 0, "")

	Local $PEHeader = $Base + DllStructGetData($IMAGE_DOS_HEADER, "e_lfanew")
	Local $IMAGE_NT_HEADER = DllStructCreate($tagIMAGE_NT_HEADER32, $PEHeader)
	If DllStructGetData($IMAGE_NT_HEADER, "Signature") <> 0x4550 Then Return SetError(1, 0, "")

	Local $IsX64
	Switch DllStructGetData($IMAGE_NT_HEADER, "Magic")
		Case 0x10B ; IMAGE_NT_OPTIONAL_HDR32_MAGIC
			$IsX64 = False
		Case 0x20B ; IMAGE_NT_OPTIONAL_HDR64_MAGIC
			$IsX64 = True
			$IMAGE_NT_HEADER = DllStructCreate($tagIMAGE_NT_HEADER64, $PEHeader)
		Case Else
			Return SetError(1, 0, "")
	EndSwitch

	Local $SizeOfDataDirectory = DllStructGetSize(DllStructCreate($tagIMAGE_DATA_DIRECTORY))
	Local $ExportDirectoryPtr = $PEHeader + DllStructGetSize($IMAGE_NT_HEADER) + $IMAGE_DIRECTORY_ENTRY_EXPORT * $SizeOfDataDirectory
	Local $ExportDirectory = DllStructCreate($tagIMAGE_DATA_DIRECTORY, $ExportDirectoryPtr)
	Local $ExportVirtualAddress = DllStructGetData($ExportDirectory, "VirtualAddress")
	Local $ExportSize = DllStructGetData($ExportDirectory, "Size")
	If $ExportSize = 0 Then Return SetError(0, $IsX64, "")

	Local $SizeOfFileHeader = DllStructGetPtr($IMAGE_NT_HEADER, "Magic") - $PEHeader
	Local $SizeOfOptionalHeader = DllStructGetData($IMAGE_NT_HEADER, "SizeOfOptionalHeader")
	Local $NumberOfSections = DllStructGetData($IMAGE_NT_HEADER, "NumberOfSections")

	Local $ExportSectionVirtualAddress = 0, $ExportSectionRawData
	Local $SectionPtr = $PEHeader + $SizeOfFileHeader + $SizeOfOptionalHeader
	For $i = 1 To $NumberOfSections
		Local $Section = DllStructCreate($tagIMAGE_SECTION_HEADER, $SectionPtr)
		Local $VirtualAddress = DllStructGetData($Section, "VirtualAddress")
		Local $SizeOfRawData = DllStructGetData($Section, "SizeOfRawData")
		Local $PointerToRawData = DllStructGetData($Section, "PointerToRawData")

		If $VirtualAddress <= $ExportVirtualAddress And $VirtualAddress + $SizeOfRawData >= $ExportVirtualAddress Then
			$ExportSectionVirtualAddress = $VirtualAddress
			$ExportSectionRawData = $PointerToRawData
			ExitLoop
		EndIf

		$SectionPtr += DllStructGetSize($Section)
	Next

	If $ExportSectionVirtualAddress Then
		Local $VirtualDiff = $ExportSectionVirtualAddress - $ExportSectionRawData
		Local $IMAGE_EXPORT_DIRECTORY = DllStructCreate($tagIMAGE_EXPORT_DIRECTORY, $Base + $ExportVirtualAddress - $VirtualDiff)
		Local $NumberOfNames = DllStructGetData($IMAGE_EXPORT_DIRECTORY, "NumberOfNames")
		Local $NameRef = $Base + DllStructGetData($IMAGE_EXPORT_DIRECTORY, "AddressOfNames") - $VirtualDiff

		Local $List
		For $i = 1 To $NumberOfNames
			Local $Ptr = $Base + DllStructGetData(DllStructCreate("dword", $NameRef), 1) - $VirtualDiff
			Local $Len = _BinaryCall_lstrlenA($Ptr)
			Local $FuncName = DllStructGetData(DllStructCreate("char[" & $Len & "]", $Ptr), 1)
			$List &= ($i = 1 ? $FuncName : $Split & $FuncName)
			$NameRef += 4
		Next
		Return SetError(0, $IsX64, $List)

	EndIf
	Return SetError(1, $IsX64, "")
EndFunc

Func DllListGenerate($SourceFile = "")
	Local $DllList = StringSplit(IniRead($IniFile, "Setting", "WindowsAPI", ""), ",", 2)
	Local $SearchSourceDir = Int(IniRead($IniFile, "Setting", "AutoSearchAndImportDllInSourceDir", "0"))
	Local $SearchScriptDir = Int(IniRead($IniFile, "Setting", "AutoSearchAndImportDllInScriptDir", "0"))

	If $SearchScriptDir Then
		DllSearch(@ScriptDir, $DllList)
	EndIf

	If $SearchSourceDir And $SourceFile Then
		Local $Drive = "", $Dir = "", $Filename = "", $Ext = ""
		_PathSplit(_PathFull($SourceFile), $Drive, $Dir, $Filename, $Ext)
		DllSearch($Drive & $Dir, $DllList)
	EndIf

	Return $DllList
EndFunc

Func DllSearch($Dir, ByRef $Array)
	If StringRight($Dir, 1) = "\" Then $Dir = StringTrimRight($Dir, 1)
	Local $Search = FileFindFirstFile($Dir & "\*.dll")
	If $Search = -1 Then Return
	While 1
		Local $Filename = FileFindNextFile($Search)
		If @Error Then ExitLoop
		_ArrayAdd($Array, _PathFull($Dir & "\" & $Filename))
	WEnd
	FileClose($Search)
EndFunc

Func StringToVar($String, $VarName = "Var", $BreakLine = 2048)
	Local $Text
	$string = String($String)
	If $String Then
		$Text = StringFormat("Local $%s = '%s'", $VarName, StringLeft($String, $BreakLine))
		$String = StringTrimLeft($String, $BreakLine)

		While $String
			$Text &= StringFormat(" & _\r\n\t\t'%s'", StringLeft($String, $BreakLine))
			$String = StringTrimLeft($String, $BreakLine)
		WEnd

		$Text &= @CRLF
	EndIf
	Return $Text
EndFunc

Func ArrayToVar(ByRef $Array, $VarName = "Var", $BreakLine = 2048)
	Local $Text, $Count = 0

	For $i = 0 To UBound($Array) - 1
		If $i = 0 Then
			$Text = StringFormat("Local $%s[] = [", $VarName)
		EndIf

		Local $Clip = StringFormat('"%s",', StringReplace($Array[$i], '"', '""'))
		$Count += StringLen($Clip)
		$Text &= $Clip

		If $i = UBound($Array) - 1 Then
			$Text = StringTrimRight($Text, 1)
			$Text &= "]"
		ElseIf $Count > $BreakLine Then
			$Text &= " _" & @CRLF & @TAB & @TAB
			$Count = 0
		EndIf
	Next
	Return $Text
EndFunc

Func BinaryFileRead($Filename)
	Local $File = FileOpen($Filename, 16)
	Local $Ret = FileRead($File)
	FileClose($File)
	Return $Ret
EndFunc

Func BinaryFileWrite($Filename, $Data)
	Local $File = FileOpen($Filename, 16+2)
	FileWrite($File, $Data)
	FileClose($File)
EndFunc

Func TextFileWrite($Filename, $Text)
	Local $File = FileOpen($Filename, 2)
	FileWrite($File, $Text)
	FileClose($File)
EndFunc
