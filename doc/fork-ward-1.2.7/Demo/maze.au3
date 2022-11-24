; =============================================================================
;  AutoIt BinaryCall UDF Demo
;  Author: Ward
; =============================================================================

#Include <GUIConstantsEx.au3>
#Include "..\BinaryCall.au3"

Main()

Func CreateMaze()
	Static $SymbolList
	If Not IsDllStruct($SymbolList) Then
		Local $Code, $Reloc
		If @AutoItX64 Then
			$Code = 'AwAAAATYbAMAAAAAAAAkL48ClECNsbXAnsP7Sx9oG5ArcpgWXuFCLumIf5QvLztB2RAYOreKA1KDba6PEpJblkd9lFZ41AbiWXBK5UrUzmvQP4GrZxqK1Def1nEQi1ZjbzPzwYcyAsXo0n5RIAu6vORzVbmM78enNryDv72ENlDls6xmEEWUdvT598qNPZ03sgtyt5Dgj2iAHfjIU5CypNVM6zd0jeB3XM11BsBOih/bp0Zo7wLc3KBJxcU94nw0Q1r60UjL4cQGU0EwLZTNjEbUHAlFs56M5JQnpFphpJ8iAaPAdIn1tRbhBokNH4jXQqJAlU6kko5PTwgmoMopTzd3FWvB065kT1a6+VNKA5YE5XzbDv4okyaeehIT/lxQ90F79nWyFyK6OTsO3z5E9WwHk++nZnmjDx/3oM3MbmEdno+YV4rDxHji24UK0fsRE8EtufSAi7qPMsCgaiyJgWG9WT1I+AzWn5Dbc5ey/vzyclxkD+4RGJ1QwlVa3IKA4uDVQIxxp2TIyxHairKpQn752WgeCK6OktbDZK+wiGaE85QV3wxTGkDJlwEcZQzvD6l8p0QfiiqYBWRSaIdQaIZoFxRR49J6F3ucarmcB7raSTBzX4Cv8uT5CbeifXLuKrkYH5ob2i2G6/Cg12m+H9r1HHksAdsb1V0zRkw98M8/R/0zfqSbTjwCCGCwtdnlH+lItWoNqK3Zk333PaUKIktEEfU+f6E1KDbms7MLRx3hs5t3oVgNfo71jdGYY+pV9QRqjMCrMCrAFnb0L3tqwIB4mIZZ6uPhM0rZDhX6tubwekw1PSdVHYW9ZQZ/8Idrq6h4P4sYkR8eCY0W49+pXiG/8r3V+EGWJ8NYTOsMg6Ft2rdFRqGY252fzQOQpFiKr8wtkIkMyV1Mdg3x5/0/RjxpqQiGGjWcqH2wCS3cJVh/iqrkKSb90HhWRJrRMBMY8oSrU/j2iXCLFBMhLJs89/ruToaZD+N7nm+LPhEig7V4zTFl7Ue9PclyB2PQtPG7uHC9jBjNRlVIYWBa23tH+7pibER0PGeJu1PRqL64vCCWM+j/XvQa4+eAfaXL5C3htpgIRlQx1TplG4I+jXp8XjEmQGbdT0LR6CM17vVg/ERI2x7krXuGRe2iBKxnqKV10OjN/2AKIMuC+dVZ+bowXk2dl9jgAeaBv6/YAsv8vamW4cAXcs6Ke0MlkmwkZfK28m3ziXgbZwIbtXyq1Tc8kOpFRmda3C1HQQNpy6FMHkLgv9TkJ1bUPudqAX/MwSkEC1giPpXEQYLEjm25293u4ym/vELM/gphWx4RT45UrU+WJEb2RSsZn3maK0uavixRg4HkWX9aVuGaZEbYBDxW9U/iZlpQrbcWRP10tyK5BJ3HlSAQeTcQIU4SNRdH5qzf03d1kRgbGvy+EC06tqjEKl9xCvJ87AJ9dj1goSfPQNfYgUq8vHJDV+topow3VNc6Ew7HPqmlpts3MRqWn5UG+1y12pTd5dKGE4ClFJHcB3xRD0JGrMmJdEQUW3iWWdJHVD2qaIjkPbRtpMJ/mxgqzTXYZHmtPgTQC6/l+scZGULIm9cuDqdRRn6o+D6DF6X6u217aCH/2Ujq6+blxHMWMXvlzllAAI+1GVpVOeG9oj5WZX5I9MqMaoF19Pc4uQ=='
		Else
			$Code = 'AwAAAARcbAMAAAAAAABcQfD555tIPPb/a31LbK6GqgzaHILktPrFEyGW6MGK0YK7pu6qCw+cDt8Ej/1Pi7ekqwWYVpUg7B8+LFNqlDxb4zMvxINaHIXooSPCvGmLu5VcSNzkbsbgQCdjgooATD4xxp5FUx85DsByixyVLoDVfTsJVvhoYwGrDUUB7dSGyV2XTqBOvUOeeqojhT6NWvQeAJu9XxVGihZ/qKYo+oI+TtiVVz7OX0Vcn/DCiuX+gf8vXnoZxgpsVCr5IQHXYGh7NRw3UjRyE3a0E+7gfpobiYVKnZDZXl/yUgWvl3Gb0c3OROT5c15Gs7iAmQoESmXA3rYu8ac79ovXljeI8Oq+oSeU4kGS5vto5fh/Bs+GgVc7HCqvN9jghLtqJLQVmirbKsLaYYOIy2+MNTZSzC+8vVJOi3v33h+OflGv9vOB+F/F5NESpDcae84tahMiFbtl05YQs2TNT0XxJAcwBplOsE2MPHp8JIJzp5yqpNKgvwi84H1zN3WYn0jCkEGC5sqoVBOpdjkjkm3XTiDDyRG5nDeadAqXNDaYzZFoNATq31T1FLcvzkzerzuO67XKNikkEkwc4qGaZF4eVTaBTtuM9Hy48QLl0BxVIeyeag/b/7VmPAgrk56Is+FZFV74BWb+Vi9X9aHAGfVO55qvzFa/LwiEr6IK5JnLxOzpJ54CMkMwiM4OJy7aE6nhJn16HYu2NOzY5RzPYWy2vVkTce7Zo/AO3lvtaqyMmvFneQGbYS+r8K5ocPJugQl0ADhXtIpib+/HASqjH74qfY9C9ZUK5E2CHplku5JMNo4we4wBYhi6acgTwivlZl5k4C2YWfnGQ/OrM1skSpL1nNc/mHKifm9E5yBjipZgw1Z35TCRTO/A3nrMzh71slXibAiDTljngH9iWWljhdUPHzcIkU2P1aFthihov3MzoDcFmCKEwAzxyaCnSmKX3CG7qT0XYZmBlOASvIwUHglRmv0wnLHaWoC7Uoz4odLYO8MJBbuVeREm58w3ztb6cqeUBA7v0CciOMq8hfwVxXTK1hHea/Q2ptkdjjfValN60bVR3b5QrEUu/Xf1kmHTanBpoYTRZcdvmNjbigrPUiBKEEuA86U0CDMhxCbVljrR+urJNU9mG5JDWm+hg5MuH9e5H8+avXw7zc/HaUBFXrKcpEFnf0ltc2QKAJGWoZAh6Q+PZ3vJZS3smJX+i5yrfmhjUyGKpJo99MbLo6edSKNPFw3jFAX6Qi57caZbQ98mwo5KCLFVFeEiaWx7F6cx3G7rwiuX6cyLBAF/QgdsphTDbkI6LrwfMy+o2ffG7ByUveCBCGLadB282xp4LNH0lIM0uSa1OsGbFelFQYhoeL3fxaJRF+962YleScfBOe4C27EKMNqtvEUvO6+Elh+T+pTeHmVPyrLty7dsdnhKnzAMfLrn/LOUnGUJBF+qDz5LZkKQEw7/EJP4cXhZ+AvN/1lIfvu5eKclgZA='
			$Reloc = 'AwAAAASqAAAAAAAAAAACABjXoik0wlrPXygG9yU9a3OnpB0jIvSrXupQgiGbYdb52W16oBMa8m1TWSRTG9gNuH2p1QY31R5M9tGOlCWhFj0YEliSIzkYmXNsYQ9fFDZQXaw='
		EndIf
		Local $CodeBase = _BinaryCall_Create($Code, $Reloc)
		If @Error Then Exit MsgBox(16, "_BinaryCall_Create Error", _BinaryCall_LastError())

		Local $Symbol[] = ["maze"]
		$SymbolList = _BinaryCall_SymbolList($CodeBase, $Symbol)
		If @Error Then Exit MsgBox(16, "_BinaryCall_SymbolList Error", _BinaryCall_LastError())
	EndIf

	Local $Ret = DllCallAddress("str:cdecl", DllStructGetData($SymbolList, "maze"))
	Return $Ret[0]
EndFunc

Func Main()
    GUICreate("Maze Generator Demo", 602, 482, -1, -1)
	Local $Filename = CreateMaze()
    Local $Pic = GUICtrlCreatePic($Filename, 0, 0, 602, 482)
	FileDelete($Filename)

    GUISetState(@SW_SHOW)
    While 1
		Switch GUIGetMsg()
			Case $Pic
				Local $Filename = CreateMaze()
				GUICtrlSetImage($Pic, $Filename)
				FileDelete($Filename)

			Case $GUI_EVENT_CLOSE
				ExitLoop
		EndSwitch
    WEnd
    GUIDelete()
EndFunc
