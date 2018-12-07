; =============================================================================
;  AutoIt BinaryCall UDF Demo
;  Author: Ward
; =============================================================================

#Include <WinAPISys.au3>
#Include "..\BinaryCall.au3"

xxHash_Test()

Func xxHash_Test()
	Local $File = FileOpen(@AutoItExe, 16)
	Local $Binary = FileRead($File)
	FileClose($File)
	xxHash(0, 0) ; For startup

	Local $Buffer = _BinaryCall_Alloc($Binary)
	Local $Msg = StringFormat('Compare CRC32/xxHash speed for file "%s"\n\n', @AutoItExe)

	Local $Timer = TimerInit()
	Local $CRC32 = _WinAPI_ComputeCrc32($Buffer, BinaryLen($Binary))
	$Msg &= StringFormat("CRC32: %08X, Spent Time: %f\r\n", $CRC32, TimerDiff($Timer))

	$Timer = TimerInit()
	Local $xxHash = xxHash($Buffer, BinaryLen($Binary))
	$Msg &= StringFormat("xxHash: %08X, Spent Time: %f\r\n", $xxHash, TimerDiff($Timer))

	_BinaryCall_Free($Buffer)

	MsgBox(0, "xxHash Demo", $Msg)
EndFunc

Func xxHash($Ptr, $Length, $Seed = 0)
	Static $SymbolList
	If Not IsDllStruct($SymbolList) Then
		Local $Code, $Reloc, $CodeBase
		If @AutoItX64 Then
			$Code = 'AwAAAAS4BwAAAAAAAAAkL48ClECNsbXAnsP7Sx9oG5A5RfeGYgCbAVc0jS0NWQYvNEoJWKLy7ax96mZt9+5jLqepiRhcESauDRtAfQpYUo1Iz3YuYTVO2DsDaDxQvPFpd696wC3BTDYi0AEAgnCgrUTt3c/HOnMrxdPCkIVPmauRteybJDfsm2F2LgbJLxPmHDuEoTbYXWM0Vt7vXPOK84AiGkxu3gkC3OlBzXvV1N99Y4xjakkGLz0W06S9kV8xOB/XJEBmmV5Ud+k8GRe5BIqdUzyg6dFP4yLsJiPaRVVNCYbvNbnZo9EJWP7QRmWiVo+qPwx+clXUf1Icazutw28HnEZE3fjMbrMet1zeaeB1ebLksIg5uZaBc/G4bcKMzT1wiju74OYEidIRUhkqGmiRXo5nosgzJRXrs1jEoSyR8FfQdAuA2AuLfY5aqoi8eJl0axnGQXjakGH/vxnfcyBsk1judAhSOwjLe2zfJWj3xw3HLuZE5HK1FmcFLjDbDTsjZNda4SVTRHavKHX5Tic4pMtzLou0H7AYtlsSFqwb06ApAED4ODfkeRQMWFqzm5FDroV9uL9mpXNpKjjY2yKP9JF1eUIj7CarFKIQBWMsEO61aiRD9SWYQDnTpdrQcUl4sCr4zx6wwbEFbND+/vfn0fhN5rdrthQcB8uKUdV/f/kKBKrOy5xWovzKPBdLe/BpYZh5OQUsSXmzLGpYH9oedDNKLlJMjQFYYuh42SDAWAlLk99P7qm0ycgMhHw1BgRyqmECbr5xmHcuwYZ6NWW3QxsJrm/tBxOpgsWRoQUuZJ7Bmbs/9R2bnNO62zCPJ746+7I/6AXVtdRoqzEugP//6Mb7SeO5PGPXbsBVJyafwHce7AVhHGRV/nx2o/cf2RWaN4MUBVfJq6bAAcazts+049gSn9EP1pbntakj3KIUPxiJfJmNtWmlZHlBHj9Dsy5QwuJ3r96+024Ew9bH+16xLvKJ7DHvecQIqm5lWpk4TVluNofSlekn51ha63f068zuZH+ec8lwFUIwuE+IegxGt+byzg6fu8UvZVdQOtD0d6tv4yJpLazoVaEoltn10zMt7Dn6mlwt9s9u2mSgjNenjrKZpIYIxTUGWmzWg/S8jB6n6pqppdYsSWEAYieXWYFvs8N1bfNSz4rRPr4ujUJuGRX6eG092ie8GF5PCLmUtnGAbmZ1YHmg29cG5ApLaY1uHozW3hOdyxPR8Ls8gi+i+m1p50b7Haf9Z541vmPdnyNZmR5/8Z/hee46SB+DJayTPOUi3sgPHEgHP9GwyEA+Ad8fypNACwen7cSBYKMHODkrm02mXcaP6X36MZjDaiNKR6xY6i5vAPh1isDaIvFGNLLqofCHzJQZxbqiPPrV77KXSzxq260Nm17BpUjP7ODnm0C9AnFSPcqEBvQYkbbBu7xbaui9C3K9lMUz/tGSnuM++z2kc2ZliCVaHUuwqzxJB/rnEuGroQbqkSrCHIQcOxnAEXY5eXoJr4BlIy9qPOHbaLBFMrOo4MAdnbLFRhDbE+tfU4fx7lC0rqocvimdYielXDmwim1sva1Cj37lS+HxaICytRwf1cgvGCY9m9jetJWOw0NKew=='
		Else
			$Code = 'AwAAAARABwAAAAAAAABcQfD555tIPPb/a31LbK6Gt2ZxV/hGYSGAizT2eplfNsEudMw2jVUE7N53wSTIpZj5zVfBe9qrpU+LuR1iwx1BYusUhiLV0/kzZC9P5bzTa1Z0GSVyRS7ykXopvuYIyU9iuwdguHCfWezDOGl+/R1V4346RP7aj07Qrwj5zcLONMDP1VKHPt7CLtvfEh+if+GX3/549GNaSMuvCIqqKk1F/3iRO14vLlWQbt9kcXpOZM0MsD34Ax1MPEUR49Fpwlv3mTT9lFNfJK2+Fue9iOt92dxrjXB+e2z+FLdl5tJQgvbcXG1V14/RCwsy5qenNEmHN43UKGUwnLepq71HHPcWATCsRQAIDMhP+TEY//MohTJKnYJ+Srrf85/YX2XXVAQ4E7fVMC6cnfuStLdWYn77XhNwk1ujPQBam8noCNSXCwvzHEHQBQWKjV3GM2kBDMN1vrnkbaPr/+sqipDGWo3pIMLCNQZcQ0gstblXAGEiuOJ//d1A4Ulei+vNQqqn9JuxkP69CLZ3UoZQoqbH7Dv/g0rrzoe+pgyC8eDUtIbrh2Jsvg+lABxJ+9iyThkdgSn9hfiADB4p6G5/XxTqHVngvCiJq7fVLheRZYE0vqcJkTLWBzy+POc/fzHznVkmmk/u9Dvu/oZu4NQMflIh2SlXRAnUscvcO3zrzTf+mQJyEjDuPsZuCogRSeNEAUC136dFiWbHJuQftFGGxzWtnftBNxCgrEFCm9mH+wZd8NHUtaeKC3e38bJuwfKweB4yFtlh7+JWPdlwsPmcCyAa4o5O9fwha2ZN1OtBgIaa5M19deVCPeXb2ErB/4oWmqUfHZi9b9+oZHtu0WGejOPClxVxKYBlzrajB+2WKU043OkZr56HoFNNBiUczJ0bc6o/4bbCNgimDiOhtrWV5u/VYOfIci8uMHbiSvLXSBDTUEFv+XySJ6N1h6VdbKIwgU7DUYXLmQzEMQQz0rgpQZdUq2NdPJvt0Wk6cPFAYgtPxDGdZpN+BqfvR3Y/TTIgiA+ZbNFwM9mTadmecfivqoaIGqRWluTHKesEev8DQFQQf0Z8d7Ga8LeenlyRmMthuCKKJoDD4qzQXiH7T42W1Szmjou7IHjwPLk0EORvsyUiIHzCVBi6NIE+1kEayqYaGlcg2eeXoU8TQzpqIgucPrperOkJBBCSTHVIgtRCyeRJtUpE9lsxv39xo8QWyuvYVIpmZEvrEIgv0+Mhctzbu+JxJXLsg3+R1qjp+MgaqO4FnhCcqffV5Of9ntJxTG+DEPCZEImuDQ6GLO5OCaueQdVNhBE4EcE5JlBrmoCyUJq9HOSq76bgSLptTHRcQozZqswQtK9oRgjUHhx3iTjzVx8VMe6akitMrRlLPEC4FPuVh9Z/TyXIQmp7lElcckW5nhGerhkCTwcu9tjmeiNQNkpf4DgA'
			$Reloc = 'AwAAAAQIAAAAAAAAAAABABm9t9YOieZskAA='
		EndIf
		$CodeBase = _BinaryCall_Create($Code, $Reloc)
		If @Error Then Return SetError(1, 0, 0)

		Local $Symbol[] = ["XXH32","XXH32_sizeofState","XXH32_resetState","XXH32_init","XXH32_update","XXH32_intermediateDigest","XXH32_digest"]
		$SymbolList = _BinaryCall_SymbolList($CodeBase, $Symbol)
		If @Error Then Return SetError(1, 0, 0)
	EndIf

	If $Length = 0 Then Return 0

	Local $Ret = DllCallAddress("uint:cdecl", DllStructGetData($SymbolList, "XXH32"), "ptr", $Ptr, "uint", $Length, "uint", $Seed)
	If @Error Then Return 0
	Return $Ret[0]
EndFunc
