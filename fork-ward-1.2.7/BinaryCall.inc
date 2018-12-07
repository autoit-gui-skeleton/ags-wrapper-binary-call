; =============================================================================
;  AutoIt BinaryCall FASM Header (2015.1.8)
;  Purpose: Macros To Generate Winodws API Jump Table
;  Author: Ward
; =============================================================================

def_local fix local L1, L2, L3, L4, L5, L6, L7, L8, L9, L10, L11, L12, L13, L14, L15, L16, L17, L18, L19, L20, L21, L22, L23, L24, L25, L26, L27, L28, L29, L30

macro	ZERO_PADDING nul
{
	if $ - $$ < 128
	    db (128 - ($ - $$)) dup 0
	end if	
}

macro	setbits
{
	virtual at 0
		mov eax, [0]
		bits = 1 shl $
	end virtual
}

macro	def_label newname, oldname
{
	macro	newname nul
	\{
		if ~ defined newname | defined @f
	@@:
		oldname
		label newname at oldname
		end if
	\}
}

macro	def_start n, label
{
	setbits
	if bits = n
		if ~ defined label | defined @f
	@@:
	label:
		display	`label # " (inline)",0ah, 0dh
}

macro	def_end
{
		end if
	end if
}

macro	__main nul
{
__main:
	ret
}

macro	___chkstk	nul
{
	def_local

	def_start 64, ___chkstk
		add     rax, 7
		and     rax, -8
		pop     r11
		mov     r10, rsp
		cmp     rax, 4096
		jc      L2
	L1:
		sub     r10, 4096
		or      dword [r10], 00H
		sub     rax, 4096
		cmp     rax, 4096
		ja      L1
	L2:
		sub     r10, rax
		or      dword [r10], 00H
		mov     rsp, r10
		push    r11
		ret
	def_end

	def_start 32, ___chkstk
		push    ecx
		mov     ecx, esp
		add     ecx, 8
	L1:
		cmp     eax, 4096
		jc      L2
		sub     ecx, 4096
		or      dword [ecx], 00H
		sub     eax, 4096
		jmp     L1
	L2:
		sub     ecx, eax
		or      dword [ecx], 00H
		mov     eax, esp
		mov     esp, ecx
		mov     ecx, dword [eax]
		mov     eax, dword [eax+4H]
		jmp     eax
	def_end
}

macro	___chkstk_ms	nul
{
	def_local

	def_start 64, ___chkstk_ms
		push    rcx
		push    rax
		cmp     rax, 4096
		lea     rcx, [rsp+18H]
		jc      L2
	L1:
		sub     rcx, 4096
		or      qword [rcx], 00H
		sub     rax, 4096
		cmp     rax, 4096
		ja      L1
	L2:
		sub     rcx, rax
		or      qword [rcx], 00H
		pop     rax
		pop     rcx
		ret
	def_end

	def_start 32, ___chkstk_ms
		push    ecx
		push    eax
		cmp     eax, 4096
		lea     ecx, [esp+0CH]
		jc      L2
	L1:
		sub     ecx, 4096
		or      dword [ecx], 00H
		sub     eax, 4096
		cmp     eax, 4096
		ja      L1
	L2:
		sub     ecx, eax
		or      dword [ecx], 00H
		pop     eax
		pop     ecx
		ret
	def_end
}

macro	___udivmoddi4	nul
{
	def_local

	def_start 64, ___udivmoddi4
	def_end

	def_start 32, ___udivmoddi4
		push	ebp
		mov	ecx, edx
		mov	ebp, esp
		push	edi
		push	esi
		push	ebx
		mov	ebx, edx
		sub	esp, 36
		mov	dword [ebp-24], eax
		mov	eax, dword [ebp+8]
		mov	edi, dword [ebp+16]
		mov	edx, dword [ebp-24]
		mov	dword [ebp-36], eax
		mov	eax, dword [ebp+12]
		mov	esi, dword [ebp-36]
		mov	dword [ebp-40], eax
		mov	eax, dword [ebp-24]
		mov	dword [ebp-44], eax
		mov	eax, dword [ebp-36]
		mov	dword [ebp-28], eax
		mov	eax, dword [ebp-40]
		mov	dword [ebp-32], eax
		cmp	dword [ebp-32], 0
		jne	L4
		cmp	esi, ecx
		jbe	L1
		mov	ebx, edx
		mov	edx, ecx
		xor	ecx, ecx
		mov	eax, ebx
		div	esi
		mov	ebx, eax
		jmp	L3
	L1:
		test	esi, esi
		jne	L2
		mov	eax, 1
		xor	edx, edx
		div	esi
		mov	esi, eax
	L2:
		mov	ebx, dword [ebp-24]
		mov	eax, ecx
		xor	edx, edx
		div	esi
		mov	ecx, eax
		mov	eax, ebx
		div	esi
		mov	ebx, eax
	L3:
		test	edi, edi
		je	L12
		mov	dword [edi], edx
		mov	dword [edi+4], 0
		jmp	L12
	L4:
		cmp	dword [ebp-32], ecx
		jbe	L5
		test	edi, edi
		je	L11
		mov	eax, dword [ebp-24]
		mov	dword [edi+4], ecx
		mov	dword [edi], eax
		jmp	L11
	L5:
		bsr	esi, dword [ebp-40]
		xor	esi, 31
		mov	dword [ebp-20], esi
		jne	L8
		cmp	edx, dword [ebp-28]
		jae	L6
		xor	ebx, ebx
		cmp	ecx, dword [ebp-32]
		mov	eax, ecx
		jbe	L7
	L6:
		mov	ebx, 1
		mov	eax, ecx
		mov	edx, dword [ebp-24]
		sub	edx, dword [ebp-36]
		sbb	eax, dword [ebp-40]
	L7:
		xor	ecx, ecx
		test	edi, edi
		je	L12
		mov	dword [edi], edx
		mov	dword [edi+4], eax
		jmp	L12
	L8:
		mov	edx, 32
		sub	edx, dword [ebp-20]
		mov	eax, dword [ebp-32]
		mov	cl, byte [ebp-20]
		mov	esi, dword [ebp-44]
		sal	eax, cl
		mov	dword [ebp-32], edx
		mov	edx, dword [ebp-28]
		mov	cl, byte [ebp-32]
		shr	edx, cl
		mov	cl, byte [ebp-20]
		or	edx, eax
		mov	eax, dword [ebp-28]
		mov	dword [ebp-36], edx
		sal	eax, cl
		mov	cl, byte [ebp-32]
		mov	dword [ebp-40], eax
		mov	eax, ebx
		shr	eax, cl
		mov	cl, byte [ebp-20]
		mov	dword [ebp-24], eax
		mov	edx, dword [ebp-24]
		sal	ebx, cl
		mov	cl, byte [ebp-32]
		shr	esi, cl
		mov	cl, byte [ebp-20]
		or	ebx, esi
		mov	esi, dword [ebp-44]
		mov	eax, ebx
		div	 dword [ebp-36]
		sal	esi, cl
		mov	dword [ebp-24], edx
		mov	ecx, eax
		mov	ebx, eax
		mov	dword [ebp-44], esi
		mul	dword [ebp-40]
		cmp	edx, dword [ebp-24]
		mov	dword [ebp-28], eax
		mov	esi, edx
		ja	L9
		cmp	eax, dword [ebp-44]
		jbe	L10
		cmp	edx, dword [ebp-24]
		jne	L10
	L9:
		lea	ebx, [ecx-1]
		mov	esi, edx
		sub	eax, dword [ebp-40]
		sbb	esi, dword [ebp-36]
		mov	dword [ebp-28], eax
	L10:
		xor	ecx, ecx
		test	edi, edi
		je	L12
		mov	cl, byte [ebp-20]
		mov	eax, dword [ebp-24]
		mov	edx, dword [ebp-44]
		sub	edx, dword [ebp-28]
		sbb	eax, esi
		shr	edx, cl
		mov	cl, byte [ebp-32]
		mov	esi, eax
		sal	esi, cl
		mov	cl, byte [ebp-20]
		or	edx, esi
		mov	dword [edi], edx
		shr	eax, cl
		xor	ecx, ecx
		mov	dword [edi+4], eax
		jmp	L12
	L11:
		xor	ecx, ecx
		xor	ebx, ebx
	L12:
		add	esp, 36
		mov	eax, ebx
		mov	edx, ecx
		pop	ebx
		pop	esi
		pop	edi
		pop	ebp
		ret
	def_end
}

macro	___udivdi3	nul
{
	def_local
		___udivmoddi4

	def_start 64, ___udivdi3
	def_end

	def_start 32, ___udivdi3
		push	ebp
		mov	ebp, esp
		push	ebx
		mov	ecx, dword [ebp+16]
		mov	ebx, dword [ebp+20]
		mov	eax, dword [ebp+8]
		mov	edx, dword [ebp+12]
		mov	dword [ebp+16], 0
		mov	dword [ebp+12], ebx
		mov	dword [ebp+8], ecx
		pop	ebx
		pop	ebp
		jmp	___udivmoddi4
	def_end
}

macro	___umoddi3	nul
{
	def_local
		___udivmoddi4

	def_start 64, ___umoddi3
	def_end

	def_start 32, ___umoddi3
		push	ebp
		mov	ebp, esp
		sub	esp, 32
		lea	eax, [ebp-8]
		mov	edx, dword [ebp+20]
		mov	dword [esp+8], eax
		mov	eax, dword [ebp+16]
		mov	dword [esp+4], edx
		mov	edx, dword [ebp+12]
		mov	dword [esp], eax
		mov	eax, dword [ebp+8]
		call	___udivmoddi4
		mov	eax, dword [ebp-8]
		mov	edx, dword [ebp-4]
		leave
		ret
	def_end
}

macro	___divdi3	nul
{
	def_local
		___udivmoddi4

	def_start 64, ___divdi3
	def_end

	def_start 32, ___divdi3
		push	ebp
		mov	ebp, esp
		push	edi
		push	esi
		push	ebx
		xor	ebx, ebx
		sub	esp, 36
		mov	edx, dword [ebp+12]
		mov	eax, dword [ebp+8]
		mov	esi, dword [ebp+16]
		mov	edi, dword [ebp+20]
		mov	dword [ebp-20], edx
		mov	edx, dword [ebp-20]
		mov	dword [ebp-24], eax
		mov	ecx, esi
		mov	eax, dword [ebp-24]
		mov	dword [ebp-28], edi
		test	edx, edx
		jns	L1
		mov	eax, dword [ebp-24]
		mov	edx, dword [ebp-20]
		neg	eax
		adc	edx, 0
		or	ebx, -1
		neg	edx
	L1:
		cmp	dword [ebp-28], 0
		jns	L2
		neg	esi
		not	ebx
		adc	edi, 0
		mov	ecx, esi
		neg	edi
		mov	dword [ebp-28], edi
	L2:
		mov	edi, dword [ebp-28]
		mov	dword [esp+8], 0
		mov	dword [esp], ecx
		mov	dword [esp+4], edi
		call	___udivmoddi4
		test	ebx, ebx
		je	L3
		neg	eax
		adc	edx, 0
		neg	edx
	L3:
		add	esp, 36
		pop	ebx
		pop	esi
		pop	edi
		pop	ebp
		ret
	def_end
}

macro	___moddi3	nul
{
	def_local
		___udivmoddi4

	def_start 64, ___moddi3
	def_end

	def_start 32, ___moddi3
		push	ebp
		mov	ebp, esp
		push	edi
		push	esi
		push	ebx
		sub	esp, 52
		mov	ebx, dword [ebp+12]
		mov	eax, dword [ebp+16]
		mov	edx, dword [ebp+20]
		mov	ecx, dword [ebp+8]
		mov	dword [ebp-44], 0
		test	ebx, ebx
		mov	dword [ebp-40], eax
		mov	edi, dword [ebp-40]
		mov	dword [ebp-36], edx
		mov	eax, ecx
		mov	edx, ebx
		mov	esi, dword [ebp-36]
		jns	L1
		neg	ecx
		mov	dword [ebp-44], -1
		adc	ebx, 0
		mov	eax, ecx
		neg	ebx
		mov	edx, ebx
	L1:
		test	esi, esi
		jns	L2
		mov	ecx, dword [ebp-40]
		mov	ebx, dword [ebp-36]
		neg	ecx
		adc	ebx, 0
		mov	edi, ecx
		neg	ebx
		mov	esi, ebx
	L2:
		lea	ecx, [ebp-24]
		mov	dword [esp], edi
		mov	dword [esp+4], esi
		mov	dword [esp+8], ecx
		call	___udivmoddi4
		cmp	dword [ebp-44], 0
		je	L3
		neg	dword [ebp-24]
		adc	dword [ebp-20], 0
		neg	dword [ebp-20]
	L3:
		mov	eax, dword [ebp-24]
		mov	edx, dword [ebp-20]
		add	esp, 52
		pop	ebx
		pop	esi
		pop	edi
		pop	ebp
		ret
	def_end
}

macro	___ashrdi3	nul
{
	def_local

	def_start 64, ___ashrdi3
	def_end

	def_start 32, ___ashrdi3
		push	ebp
		mov	ebp, esp
		mov	ecx, dword [ebp+16]
		mov	edx, dword [ebp+12]
		mov	eax, dword [ebp+8]
		shrd	eax, edx, cl
		sar	edx, cl
		test	cl, 32
		je	L1
		mov	eax, edx
		sar	edx, 31
	L1:
		pop	ebp
		ret
	def_end
}

macro	___lshrdi3	nul
{
	def_local

	def_start 64, ___lshrdi3
	def_end

	def_start 32, ___lshrdi3
		push	ebp
		mov	ebp, esp
		mov	ecx, dword [ebp+16]
		mov	edx, dword [ebp+12]
		mov	eax, dword [ebp+8]
		shrd	eax, edx, cl
		shr	edx, cl
		test	cl, 32
		je	L1
		mov	eax, edx
		xor	edx, edx
	L1:
		pop	ebp
		ret
	def_end
}

macro	___ashldi3	nul
{
	def_local

	def_start 64, ___ashldi3
	def_end

	def_start 32, ___ashldi3
		push	ebp
		mov	ebp, esp
		mov	ecx, dword [ebp+16]
		mov	eax, dword [ebp+8]
		mov	edx, dword [ebp+12]
		shld	edx, eax, cl
		sal	eax, cl
		test	cl, 32
		je	L1
		mov	edx, eax
		xor	eax, eax
	L1:
		pop	ebp
		ret
	def_end
}

macro	___floatundisf	nul
{
	def_local

	def_start 64, ___floatundisf
		sub	rsp, 24
		mov	rax, rcx
		sar	rax, 32
		test	eax, eax
		js	L1
		cvtsi2ss	xmm0, rcx
		jmp	L2
	L1:
		mov	qword [rsp], rcx
		fild	qword [rsp]
		fadd	dword [L3]
		fstp	dword [12+rsp]
		movss	xmm0, dword [12+rsp]
	L2:
		add	rsp, 24
		ret

	align 4
	L3:
		dd	1602224128
	def_end

	def_start 32, ___floatundisf
		push	ebp
		mov	ebp, esp
		sub	esp, 8
		mov	eax, dword [ebp+12]
		fild	qword [ebp+8]
		test	eax, eax
		jns	L1
		fadd	dword [L2]
	L1:
		fstp	dword [ebp-4]
		fld	dword [ebp-4]
		leave
		ret
	align 4
	L2:
		dd	1602224128
	def_end
}

macro	___floatundidf	nul
{
	def_local

	def_start 64, ___floatundidf
		sub	rsp, 24
		mov	rax, rcx
		sar	rax, 32
		test	eax, eax
		js	L1
		cvtsi2sd	xmm0, rcx
		jmp	L2
	L1:
		mov	qword [rsp], rcx
		fild	qword [rsp]
		fadd	dword [L3]
		fstp	qword [8+rsp]
		movsd	xmm0, qword [8+rsp]
	L2:
		add	rsp, 24
		ret
	align 4
	L3:
		dd	1602224128
	def_end

	def_start 32, ___floatundidf
		push	ebp
		mov	ebp, esp
		sub	esp, 8
		mov	eax, dword [ebp+12]
		fild	qword [ebp+8]
		test	eax, eax
		jns	L1
		fadd	dword [L2]
	L1:
		fstp	qword [ebp-8]
		fld	qword [ebp-8]
		leave
		ret
	align 4
	L2:
		dd	1602224128
	def_end
}

macro	___floatundixf	nul
{
	def_local

	def_start 64, ___floatundixf
		sub	rsp, 24
		mov	rax, rcx
		mov	rcx, rdx
		mov	qword [8+rsp], rdx
		sar	rcx, 32
		test	ecx, ecx
		fild	qword [8+rsp]
		jns	L1
		fadd	dword [L2]
	L1:
		fstp	Tbyte [rax]
		add	rsp, 24
		ret
	align 4
	L2:
		dd	1602224128
	def_end

	def_start 32, ___floatundixf
		push	ebp
		mov	ebp, esp
		mov	eax, dword [ebp+12]
		fild	qword [ebp+8]
		test	eax, eax
		jns	L1
		fadd	dword [L2]
	L1:
		pop	ebp
		ret
	align 4
	L2:
		dd	1602224128
	def_end
}

macro	___fixunssfdi	nul
{
	def_local

	def_start 64, ___fixunssfdi
		xor	eax, eax
		movd	edx, xmm0
		test	edx, edx
		je	L1
		movd	ecx, xmm0
		or	rax, -1
		sar	ecx, 23
		movzx	ecx, cl
		sub	ecx, 150
		cmp	ecx, 40
		jg	L1
		and	edx, 8388607
		or	edx, 8388608
		test	ecx, ecx
		js	L2
		movsxd	rdx, edx
		mov	rax, rdx
		sal	rax, cl
		ret
	L2:
		xor	eax, eax
		cmp	ecx, -23
		jl	L1
		neg	ecx
		shr	edx, cl
		mov	eax, edx
	L1:
		ret
	def_end

	def_start 32, ___fixunssfdi
		push	ebp
		xor	eax, eax
		xor	edx, edx
		mov	ebp, esp
		push	ebx
		mov	ebx, dword [ebp+8]
		test	ebx, ebx
		je	L2
		mov	ecx, ebx
		or	eax, -1
		sar	ecx, 23
		mov	edx, eax
		movzx	ecx, cl
		sub	ecx, 150
		cmp	ecx, 40
		jg	L2
		and	ebx, 8388607
		or	ebx, 8388608
		test	ecx, ecx
		js	L1
		mov	eax, ebx
		cdq
		sal	eax, cl
		shld	edx, ebx, cl
		xor	ebx, ebx
		and	cl, 32
		cmovne	edx, eax
		cmovne	eax, ebx
		jmp	L2
	L1:
		xor	eax, eax
		xor	edx, edx
		cmp	ecx, -23
		jl	L2
		neg	ecx
		mov	eax, ebx
		xor	edx, edx
		shr	eax, cl
	L2:
		pop	ebx
		pop	ebp
		ret
	def_end
}

macro	___fixunsdfdi	nul
{
	def_local

	def_start 64, ___fixunsdfdi
		xor	eax, eax
		movq	rdx, xmm0
		test	rdx, rdx
		je	L1
		movq	rax, xmm0
		sar	rax, 52
		mov	rcx, rax
		or	rax, -1
		and	ecx, 2047
		sub	ecx, 1075
		cmp	ecx, 11
		jg	L1
		mov	rax, 4503599627370495
		and	rdx, rax
		mov	rax, 4503599627370496
		or	rdx, rax
		test	ecx, ecx
		js	L2
		mov	rax, rdx
		sal	rax, cl
		ret
	L2:
		xor	eax, eax
		cmp	ecx, -52
		jl	L1
		neg	ecx
		mov	rax, rdx
		shr	rax, cl
	L1:
		ret
	def_end

	def_start 32, ___fixunsdfdi
		push	ebp
		xor	eax, eax
		xor	edx, edx
		mov	ebp, esp
		push	edi
		push	esi
		mov	esi, dword [ebp+12]
		push	ebx
		mov	ebx, dword [ebp+8]
		mov	edi, esi
		or	edi, ebx
		je	L2
		mov	ecx, esi
		or	eax, -1
		sar	ecx, 20
		mov	edx, eax
		and	ecx, 2047
		sub	ecx, 1075
		cmp	ecx, 11
		jg	L2
		mov	eax, esi
		and	eax, 1048575
		or	eax, 1048576
		test	ecx, ecx
		mov	edi, eax
		js	L1
		mov	edx, edi
		mov	eax, ebx
		xor	esi, esi
		shld	edx, ebx, cl
		sal	eax, cl
		and	cl, 32
		cmovne	edx, eax
		cmovne	eax, esi
		jmp	L2
	L1:
		xor	eax, eax
		xor	edx, edx
		cmp	ecx, -52
		jl	L2
		neg	ecx
		mov	eax, ebx
		mov	edx, edi
		shrd	eax, edi, cl
		shr	edx, cl
		xor	ebx, ebx
		and	cl, 32
		cmovne	eax, edx
		cmovne	edx, ebx
	L2:
		pop	ebx
		pop	esi
		pop	edi
		pop	ebp
		ret
	def_end
}

macro	___fixunsxfdi	nul
{
	def_local

	def_start 64, ___fixunsxfdi
		sub	rsp, 24
		fld	tbyte [rcx]
		fstp	tbyte [rsp]
		mov	rax, qword [rsp]
		test	rax, rax
		jne	L1
		cmp	word [8+rsp], 0
		je	L4
	L1:
		mov	ecx, dword [8+rsp]
		and	ecx, 32767
		sub	ecx, 16446
		test	ecx, ecx
		jg	L2
		cmp	ecx, -63
		jl	L3
		neg	ecx
		shr	rax, cl
		jmp	L4
	L2:
		or	rax, -1
		jmp	L4
	L3:
		xor	eax, eax
	L4:
		add	rsp, 24
		ret
	def_end

	def_start 32, ___fixunsxfdi
		push	ebp
		mov	ebp, esp
		sub	esp, 24
		fld	Tbyte [ebp+8]
		fstp	Tbyte [ebp-24]
		mov	edx, dword [ebp-20]
		mov	eax, dword [ebp-24]
		mov	ecx, edx
		or	ecx, eax
		jne	L1
		cmp	word [ebp-16], 0
		je	L5
	L1:
		mov	ecx, dword [ebp-16]
		and	ecx, 32767
		sub	ecx, 16446
		test	ecx, ecx
		jg	L3
		cmp	ecx, -63
		jl	L4
		neg	ecx
		shrd	eax, edx, cl
		shr	edx, cl
		test	cl, 32
		je	L2
		mov	eax, edx
		xor	edx, edx
	L2:
		jmp	L5
	L3:
		or	eax, -1
		mov	edx, eax
		jmp	L5
	L4:
		xor	eax, eax
		xor	edx, edx
	L5:
		leave
		ret
	def_end
}

macro	_strtoll	nul
{
	def_local
		___moddi3
		___divdi3
		isspace
		isalpha
		isupper
		errno
		_errno

	def_start 64, _strtoll
		push	r15
		push	r14
		push	r13
		push	r12
		push	rbp
		push	rdi
		push	rsi
		push	rbx
		sub	rsp, 56
		mov	rdi, qword isspace
		mov	qword [128+rsp], rcx
		mov	qword [136+rsp], rdx
		mov	r14d, r8d
		mov	rbx, rcx
	L1:
		lea	r15, [1+rbx]
		movzx	esi, byte [-1+r15]
		mov	ecx, esi
		call	rdi
		test	eax, eax
		je	L2
		mov	rbx, r15
		jmp	L1
	L2:
		cmp	esi, 45
		mov	r12d, esi
		jne	?L4
		lea	r15, [2+rbx]
		movsx	r12d, byte [1+rbx]
		mov	edi, 1
		jmp	L4
	?L4:
		xor	edi, edi
		cmp	esi, 43
		jne	L4
		movsx	r12d, byte [1+rbx]
		lea	r15, [2+rbx]
	L4:
		test	r14d, -17
		jne	L7
		test	r14d, r14d
		sete	dl
		cmp	r12d, 48
		jne	L5
		mov	al, byte [r15]
		and	eax, -33
		cmp	al, 88
		jne	L21
		movsx	r12d, byte [1+r15]
		mov	r14d, 16
		add	r15, 2
		jmp	L7
	L5:
		test	dl, dl
		mov	eax, 10
	L6:
		cmovne	r14d, eax
	L7:
		test	edi, edi
		mov	rdx, 9223372036854775807
		mov	rax, -9223372036854775808
		cmove	rax, rdx
		movsxd	rcx, r14d
		cqo
		mov	qword [40+rsp], rcx
		idiv	rcx
		test	edi, edi
		mov	esi, edx
		mov	rbx, rax
		je	L9
		test	edx, edx
		jle	L8
		sub	esi, r14d
		inc	rbx
	L8:
		neg	esi
	L9:
		xor	ebp, ebp
		xor	r13d, r13d
	L10:
		lea	eax, [-48+r12]
		cmp	eax, 9
		mov	r8d, eax
		jbe	L11
		mov	ecx, r12d
		call	qword isalpha
		test	eax, eax
		je	L20
		mov	ecx, r12d
		call	qword isupper
		cmp	eax, 1
		mov	r8d, r12d
		sbb	eax, eax
		and	eax, 32
		add	eax, 55
		sub	r8d, eax
	L11:
		cmp	r8d, r14d
		jge	L20
		cmp	r13d, -1
		je	L19
		test	edi, edi
		je	L14
		cmp	rbp, rbx
		jl	L12
		cmp	r8d, esi
		jle	L13
		cmp	rbp, rbx
		jne	L13
	L12:
		call	qword errno
		mov	rbp, -9223372036854775808
		mov	dword [rax], 34
		jmp	L16
	L13:
		mov	rdx, rbp
		movsxd	r8, r8d
		imul	rdx, qword [40+rsp]
		sub	rdx, r8
		mov	rbp, rdx
		jmp	L18
	L14:
		cmp	rbp, rbx
		jg	L15
		cmp	r8d, esi
		jle	L17
		cmp	rbp, rbx
		jne	L17
	L15:
		call	qword errno
		mov	rbp, 9223372036854775807
		mov	dword [rax], 34
	L16:
		or	r13d, -1
		jmp	L19
	L17:
		mov	rdx, rbp
		movsxd	r8, r8d
		imul	rdx, qword [40+rsp]
		lea	rbp, [rdx+r8]
	L18:
		mov	r13d, 1
	L19:
		inc	r15
		movzx	r12d, byte [-1+r15]
		jmp	L10
	L20:
		cmp	qword [136+rsp], 0
		je	L22
		dec	r15
		test	r13d, r13d
		cmove	r15, qword [128+rsp]
		mov	rax, qword [136+rsp]
		mov	qword [rax], r15
		jmp	L22
	L21:
		test	dl, dl
		mov	eax, 8
		jmp	L6
	L22:
		mov	rax, rbp
		add	rsp, 56
		pop	rbx
		pop	rsi
		pop	rdi
		pop	rbp
		pop	r12
		pop	r13
		pop	r14
		pop	r15
		ret
	def_end

	def_start 32, _strtoll
		push	ebp
		mov	ebp, esp
		push	edi
		push	esi
		push	ebx
		sub	esp, 60
		mov	edi, dword [ebp+8]
		mov	edx, dword isspace
	L1:
		lea	ebx, [edi+1]
		movzx	esi, byte [ebx-1]
		mov	dword [ebp-32], edx
		mov	dword [esp], esi
		call	edx
		test	eax, eax
		mov	edx, dword [ebp-32]
		je	L2
		mov	edi, ebx
		jmp	L1
	L2:
		cmp	esi, 45
		jne	L3
		lea	ebx, [edi+2]
		movsx	esi, byte [edi+1]
		mov	dword [ebp-56], 1
		jmp	L4
	L3:
		cmp	esi, 43
		mov	dword [ebp-56], 0
		jne	L4
		movsx	esi, byte [edi+1]
		lea	ebx, [edi+2]
	L4:
		test	dword [ebp+16], -17
		jne	L7
		cmp	dword [ebp+16], 0
		sete	dl
		cmp	esi, 48
		jne	L5
		mov	al, byte [ebx]
		and	eax, -33
		cmp	al, 88
		jne	L25
		movsx	esi, byte [ebx+1]
		mov	dword [ebp+16], 16
		add	ebx, 2
		jmp	L7
	L5:
		test	dl, dl
		mov	eax, 10
	L6:
		cmove	eax, dword [ebp+16]
		mov	dword [ebp+16], eax
	L7:
		cmp	dword [ebp-56], 0
		mov	dword [ebp-32], 0
		mov	dword [ebp-28], -2147483648
		jne	L8
		mov	dword [ebp-32], -1
		mov	dword [ebp-28], 2147483647
	L8:
		mov	eax, dword [ebp+16]
		mov	dword [ebp-48], eax
		sar	eax, 31
		mov	dword [ebp-44], eax
		mov	eax, dword [ebp-48]
		mov	edx, dword [ebp-44]
		mov	dword [esp+8], eax
		mov	eax, dword [ebp-32]
		mov	dword [esp+12], edx
		mov	edx, dword [ebp-28]
		mov	dword [esp], eax
		mov	dword [esp+4], edx
		call	___moddi3
		mov	edx, dword [ebp-44]
		mov	dword [ebp-52], eax
		mov	eax, dword [ebp-48]
		mov	dword [esp+12], edx
		mov	edx, dword [ebp-28]
		mov	dword [esp+8], eax
		mov	eax, dword [ebp-32]
		mov	dword [esp+4], edx
		mov	dword [esp], eax
		call	___divdi3
		cmp	dword [ebp-56], 0
		mov	dword [ebp-40], eax
		mov	dword [ebp-36], edx
		je	L10
		cmp	dword [ebp-52], 0
		jle	L9
		mov	ecx, dword [ebp+16]
		sub	dword [ebp-52], ecx
		add	dword [ebp-40], 1
		adc	dword [ebp-36], 0
	L9:
		neg	dword [ebp-52]
	L10:
		mov	dword [ebp-32], 0
		mov	dword [ebp-28], 0
		xor	edi, edi
	L11:
		lea	edx, [esi-48]
		cmp	edx, 9
		ja	L12
		mov	esi, edx
		jmp	L13
	L12:
		mov	dword [esp], esi
		call	dword isalpha
		test	eax, eax
		je	L24
		mov	dword [esp], esi
		call	dword isupper
		cmp	eax, 1
		sbb	eax, eax
		and	eax, 32
		add	eax, 55
		sub	esi, eax
	L13:
		cmp	esi, dword [ebp+16]
		jge	L24
		cmp	edi, -1
		je	L23
		cmp	dword [ebp-56], 0
		mov	eax, dword [ebp-36]
		je	L17
		cmp	dword [ebp-28], eax
		jl	L15
		jg	L14
		mov	eax, dword [ebp-40]
		cmp	dword [ebp-32], eax
		jb	L15
	L14:
		cmp	esi, dword [ebp-52]
		jle	L16
		mov	edx, dword [ebp-32]
		mov	eax, dword [ebp-28]
		xor	edx, dword [ebp-40]
		xor	eax, dword [ebp-36]
		or	edx, eax
		jne	L16
	L15:
		call	dword _errno
		mov	dword [ebp-32], 0
		mov	dword [eax], 34
		mov	dword [ebp-28], -2147483648
		jmp	L20
	L16:
		mov	edx, dword [ebp-28]
		mov	eax, dword [ebp-44]
		mov	edi, esi
		imul	edx, dword [ebp-48]
		imul	eax, dword [ebp-32]
		sar	edi, 31
		lea	ecx, [edx+eax]
		mov	eax, dword [ebp-32]
		mul	dword [ebp-48]
		add	edx, ecx
		sub	eax, esi
		sbb	edx, edi
		jmp	L22
	L17:
		cmp	dword [ebp-28], eax
		jg	L19
		jl	L18
		mov	eax, dword [ebp-40]
		cmp	dword [ebp-32], eax
		ja	L19
	L18:
		cmp	esi, dword [ebp-52]
		jle	L21
		mov	edx, dword [ebp-32]
		mov	eax, dword [ebp-28]
		xor	edx, dword [ebp-40]
		xor	eax, dword [ebp-36]
		or	edx, eax
		jne	L21
	L19:
		call	dword _errno
		mov	dword [ebp-32], -1
		mov	dword [eax], 34
		mov	dword [ebp-28], 2147483647
	L20:
		or	edi, -1
		jmp	L23
	L21:
		mov	edx, dword [ebp-28]
		mov	eax, dword [ebp-44]
		mov	edi, esi
		imul	edx, dword [ebp-48]
		imul	eax, dword [ebp-32]
		sar	edi, 31
		lea	ecx, [edx+eax]
		mov	eax, dword [ebp-32]
		mul	dword [ebp-48]
		add	edx, ecx
		add	eax, esi
		adc	edx, edi
	L22:
		mov	dword [ebp-32], eax
		mov	dword [ebp-28], edx
		mov	edi, 1
	L23:
		inc	ebx
		movzx	esi, byte [ebx-1]
		jmp	L11
	L24:
		cmp	dword [ebp+12], 0
		je	L26
		dec	ebx
		test	edi, edi
		cmove	ebx, dword [ebp+8]
		mov	eax, dword [ebp+12]
		mov	dword [eax], ebx
		jmp	L26
	L25:
		test	dl, dl
		mov	eax, 8
		jmp	L6
	L26:
		mov	eax, dword [ebp-32]
		mov	edx, dword [ebp-28]
		add	esp, 60
		pop	ebx
		pop	esi
		pop	edi
		pop	ebp
		ret
	def_end
}

macro	_strtoull	nul
{
	def_local
		___udivdi3
		___umoddi3
		isspace
		isalpha
		isupper
		errno
		_errno

	def_start 64, _strtoull
		push	r15
		push	r14
		push	r13
		push	r12
		push	rbp
		push	rdi
		push	rsi
		push	rbx
		sub	rsp, 72
		mov	rdi, qword isspace
		mov	rbp, rcx
		mov	r12, rdx
		mov	r14d, r8d
		mov	rbx, rcx
	L1:
		lea	r15, [1+rbx]
		movzx	esi, byte [-1+r15]
		mov	ecx, esi
		call	rdi
		test	eax, eax
		je	L2
		mov	rbx, r15
		jmp	L1
	L2:
		cmp	esi, 45
		mov	r13d, esi
		jne	L3
		lea	r15, [2+rbx]
		movsx	r13d, byte [1+rbx]
		mov	edi, 1
		jmp	L4
	L3:
		xor	edi, edi
		cmp	esi, 43
		jne	L4
		movsx	r13d, byte [1+rbx]
		lea	r15, [2+rbx]
	L4:
		test	r14d, -17
		jne	L7
		test	r14d, r14d
		sete	dl
		cmp	r13d, 48
		jne	L5
		mov	al, byte [r15]
		and	eax, -33
		cmp	al, 88
		jne	L15
		movsx	r13d, byte [1+r15]
		mov	r14d, 16
		add	r15, 2
		jmp	L7
	L5:
		test	dl, dl
		mov	eax, 10
	L6:
		cmovne	r14d, eax
	L7:
		movsxd	rax, r14d
		xor	edx, edx
		xor	ebx, ebx
		mov	qword [40+rsp], rax
		or	rax, -1
		xor	esi, esi
		div	qword [40+rsp]
		mov	qword [48+rsp], rax
		mov	dword [60+rsp], edx
	L8:
		lea	eax, [-48+r13]
		cmp	eax, 9
		mov	r8d, eax
		jbe	L9
		mov	ecx, r13d
		call	qword isalpha
		test	eax, eax
		je	L13
		mov	ecx, r13d
		call	qword isupper
		cmp	eax, 1
		mov	r8d, r13d
		sbb	eax, eax
		and	eax, 32
		add	eax, 55
		sub	r8d, eax
	L9:
		cmp	r8d, r14d
		jge	L13
		cmp	ebx, -1
		je	L12
		cmp	rsi, qword [48+rsp]
		ja	L10
		cmp	r8d, dword [60+rsp]
		jle	L11
		cmp	rsi, qword [48+rsp]
		jne	L11
	L10:
		call	qword errno
		or	ebx, -1
		mov	dword [rax], 34
		or	rsi, -1
		jmp	L12
	L11:
		mov	rdx, rsi
		movsxd	r8, r8d
		mov	ebx, 1
		imul	rdx, qword [40+rsp]
		lea	rsi, [r8+rdx]
	L12:
		inc	r15
		movzx	r13d, byte [-1+r15]
		jmp	L8
	L13:
		test	ebx, ebx
		mov	rax, rsi
		jle	L14
		mov	rax, rsi
		neg	rax
		test	edi, edi
		cmove	rax, rsi
	L14:
		test	r12, r12
		je	L16
		dec	r15
		test	ebx, ebx
		cmovne	rbp, r15
		mov	qword [r12], rbp
		jmp	L16
	L15:
		test	dl, dl
		mov	eax, 8
		jmp	L6
	L16:
		add	rsp, 72
		pop	rbx
		pop	rsi
		pop	rdi
		pop	rbp
		pop	r12
		pop	r13
		pop	r14
		pop	r15
		ret
	def_end

	def_start 32, _strtoull
		push	ebp
		mov	ebp, esp
		push	edi
		push	esi
		push	ebx
		sub	esp, 60
		mov	edi, dword [ebp+8]
		mov	edx, dword isspace
	L1:
		lea	ebx, [edi+1]
		movzx	esi, byte [ebx-1]
		mov	dword [ebp-32], edx
		mov	dword [esp], esi
		call	edx
		test	eax, eax
		mov	edx, dword [ebp-32]
		je	L2
		mov	edi, ebx
		jmp	L1
	L2:
		cmp	esi, 45
		jne	L3
		lea	ebx, [edi+2]
		movsx	esi, byte [edi+1]
		mov	dword [ebp-44], 1
		jmp	L4
	L3:
		cmp	esi, 43
		mov	dword [ebp-44], 0
		jne	L4
		movsx	esi, byte [edi+1]
		lea	ebx, [edi+2]
	L4:
		test	dword [ebp+16], -17
		jne	L7
		cmp	dword [ebp+16], 0
		sete	dl
		cmp	esi, 48
		jne	L5
		mov	al, byte [ebx]
		and	eax, -33
		cmp	al, 88
		jne	L17
		movsx	esi, byte [ebx+1]
		mov	dword [ebp+16], 16
		add	ebx, 2
		jmp	L7
	L5:
		test	dl, dl
		mov	eax, 10
	L6:
		cmove	eax, dword [ebp+16]
		mov	dword [ebp+16], eax
	L7:
		mov	eax, dword [ebp+16]
		mov	dword [esp], -1
		xor	edi, edi
		mov	dword [esp+4], -1
		mov	dword [ebp-40], eax
		sar	eax, 31
		mov	dword [ebp-36], eax
		mov	eax, dword [ebp-40]
		mov	edx, dword [ebp-36]
		mov	dword [esp+8], eax
		mov	dword [esp+12], edx
		call	___udivdi3
		mov	dword [ebp-48], eax
		mov	dword [ebp-52], edx
		mov	eax, dword [ebp-40]
		mov	edx, dword [ebp-36]
		mov	dword [esp], -1
		mov	dword [esp+4], -1
		mov	dword [esp+8], eax
		mov	dword [esp+12], edx
		call	___umoddi3
		mov	dword [ebp-32], 0
		mov	dword [ebp-56], eax
		mov	dword [ebp-28], 0
	L8:
		lea	edx, [esi-48]
		cmp	edx, 9
		ja	L9
		mov	esi, edx
		jmp	L10
	L9:
		mov	dword [esp], esi
		call	dword isalpha
		test	eax, eax
		je	L15
		mov	dword [esp], esi
		call	dword isupper
		cmp	eax, 1
		sbb	eax, eax
		and	eax, 32
		add	eax, 55
		sub	esi, eax
	L10:
		cmp	esi, dword [ebp+16]
		jge	L15
		cmp	edi, -1
		je	L14
		mov	eax, dword [ebp-52]
		cmp	dword [ebp-28], eax
		ja	L12
		jb	L11
		mov	eax, dword [ebp-48]
		cmp	dword [ebp-32], eax
		ja	L12
	L11:
		cmp	esi, dword [ebp-56]
		jle	L13
		mov	edx, dword [ebp-32]
		mov	eax, dword [ebp-28]
		xor	edx, dword [ebp-48]
		xor	eax, dword [ebp-52]
		or	edx, eax
		jne	L13
	L12:
		call	dword _errno
		or	edi, -1
		mov	dword [eax], 34
		mov	dword [ebp-32], -1
		mov	dword [ebp-28], -1
		jmp	L14
	L13:
		mov	edx, dword [ebp-28]
		mov	eax, dword [ebp-36]
		mov	edi, esi
		imul	eax, dword [ebp-32]
		imul	edx, dword [ebp-40]
		sar	edi, 31
		lea	ecx, [edx+eax]
		mov	eax, dword [ebp-32]
		mul	dword [ebp-40]
		add	edx, ecx
		add	esi, eax
		adc	edi, edx
		mov	dword [ebp-32], esi
		mov	dword [ebp-28], edi
		mov	edi, 1
	L14:
		inc	ebx
		movzx	esi, byte [ebx-1]
		jmp	L8
	L15:
		test	edi, edi
		mov	eax, dword [ebp-32]
		mov	edx, dword [ebp-28]
		jle	L16
		cmp	dword [ebp-44], 0
		je	L16
		mov	eax, dword [ebp-32]
		mov	edx, dword [ebp-28]
		neg	eax
		adc	edx, 0
		neg	edx
	L16:
		cmp	dword [ebp+12], 0
		je	L18
		lea	ecx, [ebx-1]
		test	edi, edi
		cmove	ecx, dword [ebp+8]
		mov	ebx, dword [ebp+12]
		mov	dword [ebx], ecx
		jmp	L18
	L17:
		test	dl, dl
		mov	eax, 8
		jmp	L6
	L18:
		add	esp, 60
		pop	ebx
		pop	esi
		pop	edi
		pop	ebp
		ret
	def_end
}

macro	_strtof	nul
{
	def_local
		__strtod
		___strtod

	def_start 64, _strtof
		sub	rsp, 40
		call	__strtod
		cvtsd2ss	xmm0, xmm0
		add	rsp, 40
		ret
	def_end

	def_start 32, _strtof
		push	ebp
		mov	ebp, esp
		sub	esp, 40
		mov	eax, dword [ebp+12]
		mov	dword [esp+4], eax
		mov	eax, dword [ebp+8]
		mov	dword [esp], eax
		call	strtod
		fstp	dword [ebp-12]
		fld	dword [ebp-12]
		leave
		ret
	def_end
}

macro	__flush	nul
{
	def_local
		fileno
		write
		_fileno
		_write

	def_start 64, __flush
		push	rdi
		push	rsi
		push	rbx
		sub	rsp, 32
		mov	eax, dword [24+rcx]
		mov	rbx, rcx
		mov	edx, eax
		and	edx, 3
		cmp	edx, 2
		jne	L1
		test	eax, 264
		je	L1
		mov	rdi, qword [16+rcx]
		mov	rsi, qword [rcx]
		sub	rsi, rdi
		test	rsi, rsi
		jle	L1
		call	qword fileno
		mov	r8d, esi
		mov	rdx, rdi
		mov	ecx, eax
		call	qword write
		cdqe
		cmp	rax, rsi
		jne	L2
		mov	eax, dword [24+rbx]
		test	al, -128
		je	L1
		and	eax, -3
		mov	dword [24+rbx], eax
	L1:
		xor	eax, eax
		jmp	L3
	L2:
		or	dword [24+rbx], 32
		or	eax, -1
	L3:
		mov	rdx, qword [16+rbx]
		mov	dword [8+rbx], 0
		mov	qword [rbx], rdx
		add	rsp, 32
		pop	rbx
		pop	rsi
		pop	rdi
		ret
	def_end

	def_start 32, __flush
		push	ebp
		mov	ebp, esp
		push	edi
		push	esi
		push	ebx
		sub	esp, 44
		mov	ebx, dword [ebp+8]
		mov	eax, dword [ebx+12]
		mov	edx, eax
		and	edx, 3
		cmp	edx, 2
		jne	L2
		test	eax, 264
		je	L2
		mov	edi, dword [ebx+8]
		mov	esi, dword [ebx]
		sub	esi, edi
		mov	eax, esi
		mov	dword [ebp-32], esi
		sar	eax, 31
		mov	dword [ebp-28], eax
		cmp	dword [ebp-28], 0
		jl	L2
		jg	L1
		cmp	esi, 0
		jbe	L2
	L1:
		mov	dword [esp], ebx
		call	dword _fileno
		mov	dword [esp+8], esi
		mov	dword [esp+4], edi
		mov	dword [esp], eax
		call	dword _write
		mov	ecx, eax
		sar	ecx, 31
		cmp	ecx, dword [ebp-28]
		jne	L3
		cmp	eax, esi
		jne	L3
		mov	eax, dword [ebx+12]
		test	al, -128
		je	L2
		and	eax, -3
		mov	dword [ebx+12], eax
	L2:
		xor	eax, eax
		jmp	L4
	L3:
		or	dword [ebx+12], 32
		or	eax, -1
	L4:
		mov	edx, dword [ebx+8]
		mov	dword [ebx+4], 0
		mov	dword [ebx], edx
		add	esp, 44
		pop	ebx
		pop	esi
		pop	edi
		pop	ebp
		ret
	def_end
}

macro	_fseeko64	nul
{
	def_local
		fgetpos
		_fgetpos
		fflush
		_fflush
		fileno
		_fileno
		_filelengthi64
		filelengthi64
		_errno
		errno
		_fsetpos
		fsetpos

	def_start 64, _fseeko64
		push	rsi
		push	rbx
		sub	rsp, 56
		cmp	r8d, 1
		mov	rsi, rcx
		mov	rbx, rdx
		jne	L1
		lea	rdx, [40+rsp]
		call	fgetpos
		mov	edx, eax
		or	eax, -1
		test	edx, edx
		jne	L6
		add	qword [40+rsp], rbx
		jmp	L5
	L1:
		cmp	r8d, 2
		jne	L2
		call	fflush
		mov	rcx, rsi
		call	qword fileno
		mov	ecx, eax
		call	qword filelengthi64
		add	rbx, rax
		jmp	L3
	L2:
		test	r8d, r8d
		jne	L4
	L3:
		mov	qword [40+rsp], rbx
		jmp	L5
	L4:
		call	qword errno
		mov	dword [rax], 22
		or	eax, -1
		jmp	L6
	L5:
		lea	rdx, [40+rsp]
		mov	rcx, rsi
		call	fsetpos
	L6:
		add	rsp, 56
		pop	rbx
		pop	rsi
		ret
	def_end

	def_start 32, _fseeko64
		push	ebp
		mov	ebp, esp
		push	edi
		push	esi
		push	ebx
		sub	esp, 44
		mov	eax, dword [ebp+20]
		mov	ebx, dword [ebp+8]
		mov	esi, dword [ebp+12]
		mov	edi, dword [ebp+16]
		cmp	eax, 1
		jne	L1
		lea	eax, [ebp-32]
		mov	dword [esp], ebx
		mov	dword [esp+4], eax
		call	_fgetpos
		mov	edx, eax
		or	eax, -1
		test	edx, edx
		jne	L5
		add	dword [ebp-32], esi
		adc	dword [ebp-28], edi
		jmp	L4
	L1:
		cmp	eax, 2
		jne	L2
		mov	dword [esp], ebx
		call	_fflush
		mov	dword [esp], ebx
		call	dword _fileno
		mov	dword [esp], eax
		call	dword _filelengthi64
		add	eax, esi
		adc	edx, edi
		mov	dword [ebp-32], eax
		mov	dword [ebp-28], edx
		jmp	L4
	L2:
		test	eax, eax
		jne	L3
		mov	dword [ebp-32], esi
		mov	dword [ebp-28], edi
		jmp	L4
	L3:
		call	dword _errno
		mov	dword [eax], 22
		or	eax, -1
		jmp	L5
	L4:
		lea	eax, [ebp-32]
		mov	dword [esp], ebx
		mov	dword [esp+4], eax
		call	_fsetpos
	L5:
		add	esp, 44
		pop	ebx
		pop	esi
		pop	edi
		pop	ebp
		ret
	def_end
}

macro	_ftello64	nul
{
	def_local
		fgetpos
		_fgetpos

	def_start 64, _ftello64
		sub	rsp, 56
		lea	rdx, [40+rsp]
		call	fgetpos
		or	rdx, -1
		test	eax, eax
		jne	L1
		mov	rdx, qword [40+rsp]
	L1:
		mov	rax, rdx
		add	rsp, 56
		ret
	def_end

	def_start 32, _ftello64
		push	ebp
		mov	ebp, esp
		sub	esp, 40
		lea	eax, [ebp-16]
		mov	dword [esp+4], eax
		mov	eax, dword [ebp+8]
		mov	dword [esp], eax
		call	_fgetpos
		mov	ecx, eax
		or	eax, -1
		test	ecx, ecx
		mov	edx, eax
		jne	L1
		mov	eax, dword [ebp-16]
		mov	edx, dword [ebp-12]
	L1:
		leave
		ret
	def_end
}

macro	__fseeki64	nul
{
	def_local
		errno
		_errno
		_ftelli64
		__ftelli64
		_flush
		__flush
		fileno
		_fileno
		lseeki64
		_lseeki64

	def_start 64, __fseeki64
		push	rbp
		push	rdi
		push	rsi
		push	rbx
		sub	rsp, 40
		mov	rdi, qword errno
		mov	rbx, rcx
		mov	rbp, rdx
		mov	esi, r8d
		call	rdi
		test	rbx, rbx
		mov	dword [rax], 0
		je	L1
		cmp	esi, 2
		jbe	L2
	L1:
		call	rdi
		mov	dword [rax], 22
		or	eax, -1
		jmp	L6
	L2:
		and	dword [24+rbx], -17
		cmp	esi, 1
		jne	L3
		mov	rcx, rbx
		xor	sil, sil
		call	_ftelli64
		add	rbp, rax
	L3:
		mov	rcx, rbx
		call	_flush
		mov	eax, dword [24+rbx]
		test	al, -128
		je	L4
		and	eax, -4
		mov	dword [24+rbx], eax
		jmp	L5
	L4:
		and	eax, 1033
		cmp	eax, 9
		jne	L5
		mov	dword [36+rbx], 512
	L5:
		mov	rcx, rbx
		call	qword fileno
		mov	r8d, esi
		mov	rdx, rbp
		mov	ecx, eax
		call	qword lseeki64
		inc	rax
		sete	al
		movzx	eax, al
		neg	eax
	L6:
		add	rsp, 40
		pop	rbx
		pop	rsi
		pop	rdi
		pop	rbp
		ret
	def_end

	def_start 32, __fseeki64
		push	ebp
		mov	ebp, esp
		push	edi
		push	esi
		push	ebx
		sub	esp, 44
		mov	edx, dword _errno
		mov	ebx, dword [ebp+8]
		mov	esi, dword [ebp+12]
		mov	edi, dword [ebp+16]
		mov	dword [ebp-28], edx
		call	edx
		test	ebx, ebx
		mov	dword [eax], 0
		mov	edx, dword [ebp-28]
		je	L1
		cmp	dword [ebp+20], 2
		jbe	L2
	L1:
		call	edx
		mov	dword [eax], 22
		or	eax, -1
		jmp	L6
	L2:
		and	dword [ebx+12], -17
		cmp	dword [ebp+20], 1
		jne	L3
		mov	dword [esp], ebx
		call	__ftelli64
		add	esi, eax
		mov	dword [ebp+20], 0
		adc	edi, edx
	L3:
		mov	dword [esp], ebx
		call	__flush
		mov	eax, dword [ebx+12]
		test	al, -128
		je	L4
		and	eax, -4
		mov	dword [ebx+12], eax
		jmp	L5
	L4:
		and	eax, 1033
		cmp	eax, 9
		jne	L5
		mov	dword [ebx+24], 512
	L5:
		mov	dword [esp], ebx
		call	dword _fileno
		mov	ecx, dword [ebp+20]
		mov	dword [esp+4], esi
		mov	dword [esp+8], edi
		mov	dword [esp], eax
		mov	dword [esp+12], ecx
		call	dword _lseeki64
		and	eax, edx
		inc	eax
		sete	al
		movzx	eax, al
		neg	eax
	L6:
		add	esp, 44
		pop	ebx
		pop	esi
		pop	edi
		pop	ebp
		ret
	def_end
}

macro	__ftelli64	nul
{
	def_local
		errno
		_errno
		fileno
		_fileno
		lseeki64
		_lseeki64

	def_start 64, __ftelli64
		push	rsi
		push	rbx
		sub	rsp, 40
		mov	rsi, qword errno
		mov	rbx, rcx
		call	rsi
		mov	rcx, rbx
		mov	dword [rax], 0
		call	qword fileno
		cmp	dword [8+rbx], 0
		jns	L1
		mov	dword [8+rbx], 0
	L1:
		xor	edx, edx
		mov	r8d, 1
		mov	ecx, eax
		call	qword lseeki64
		or	rdx, -1
		test	rax, rax
		js	L7
		mov	ecx, dword [24+rbx]
		test	ecx, 264
		jne	L2
		movsxd	rdx, dword [8+rbx]
		sub	rax, rdx
		mov	rdx, rax
		jmp	L7
	L2:
		mov	rdx, qword [rbx]
		sub	rdx, qword [16+rbx]
		test	cl, -125
		jne	L3
		call	rsi
		or	rdx, -1
		mov	dword [rax], 22
		jmp	L7
	L3:
		test	rax, rax
		je	L7
		and	cl, 1
		je	L6
		movsxd	rcx, dword [8+rbx]
		test	ecx, ecx
		je	L4
		add	rcx, rdx
		jmp	L5
	L4:
		xor	ecx, ecx
		xor	edx, edx
	L5:
		sub	rax, rcx
	L6:
		add	rdx, rax
	L7:
		mov	rax, rdx
		add	rsp, 40
		pop	rbx
		pop	rsi
		ret
	def_end

	def_start 32, __ftelli64
		push	ebp
		mov	ebp, esp
		push	edi
		push	esi
		push	ebx
		sub	esp, 44
		mov	ebx, dword [ebp+8]
		mov	esi, dword _errno
		call	esi
		mov	dword [eax], 0
		mov	dword [esp], ebx
		mov	edi, esi
		call	dword _fileno
		cmp	dword [ebx+4], 0
		jns	L1
		mov	dword [ebx+4], 0
	L1:
		mov	dword [esp+12], 1
		mov	dword [esp+4], 0
		mov	dword [esp+8], 0
		mov	dword [esp], eax
		call	dword _lseeki64
		test	edx, edx
		mov	dword [ebp-32], -1
		mov	dword [ebp-28], -1
		js	L9
		mov	esi, dword [ebx+12]
		test	esi, 264
		jne	L2
		mov	ecx, dword [ebx+4]
		mov	ebx, ecx
		sar	ebx, 31
		sub	eax, ecx
		sbb	edx, ebx
		jmp	L8
	L2:
		mov	ecx, dword [ebx]
		sub	ecx, dword [ebx+8]
		test	esi, 131
		jne	L3
		call	edi
		mov	dword [ebp-32], -1
		mov	dword [eax], 22
		mov	dword [ebp-28], -1
		jmp	L9
	L3:
		mov	edi, edx
		or	edi, eax
		jne	L4
		mov	dword [ebp-32], ecx
		mov	dword [ebp-28], 0
		jmp	L9
	L4:
		and	esi, 1
		je	L7
		mov	esi, dword [ebx+4]
		test	esi, esi
		je	L5
		add	esi, ecx
		jmp	L6
	L5:
		xor	esi, esi
		xor	ecx, ecx
	L6:
		xor	edi, edi
		sub	eax, esi
		sbb	edx, edi
	L7:
		xor	ebx, ebx
		add	eax, ecx
		adc	edx, ebx
	L8:
		mov	dword [ebp-32], eax
		mov	dword [ebp-28], edx
	L9:
		mov	eax, dword [ebp-32]
		mov	edx, dword [ebp-28]
		add	esp, 44
		pop	ebx
		pop	esi
		pop	edi
		pop	ebp
		ret
	def_end
}

macro	___ms_vfscanf	nul
{
	def_local
		memcpy
		fscanf
		_memcpy
		_fscanf

	def_start 64, ___ms_vfscanf
		push	rdi
		push	rsi
		push	rbx
		sub	rsp, 16
		lea	rax, [12+rsp]
		mov	rsi, r8
		mov	rbx, rsp
		lea	rsp, [rsp+rax-0x28]
		sub	rsp, rsi
		mov	[rsp+0x18], rcx
		mov	[rsp+0x20], rdx
		lea	rdi, [rsp+0x28]
		mov	[rsp], rdi
		mov	[rsp+0x8], rsi
		mov	[rsp+0x10], rsi
		sub	[rsp+0x10], rax
		mov	r8, [rsp+0x10]
		mov	rdx, [rsp+0x8]
		mov	rcx, [rsp]
		call	memcpy
		add	rsp, 24
		mov	r9, [rsp+0x18]
		mov	r8, [rsp+0x10]
		mov	rdx, [rsp+0x8]
		mov	rcx, [rsp]
		call	fscanf
		mov	rsp, rbx
		add	rsp, 16
		pop	rbx
		pop	rsi
		pop	rdi
		ret
	def_end

	def_start 32, ___ms_vfscanf
		push	ebp
		mov	ebp, esp
		push	edi
		push	esi
		push	ebx
		lea	eax, [ebp-16]
		sub	esp, 16
		mov	ecx, dword [ebp+8]
		mov	edx, dword [ebp+12]
		mov	esi, dword [ebp+16]
		mov	ebx, esp
		lea	esp, [esp+eax-0x14]
		sub	esp, esi
		mov	[esp+0xC], ecx
		mov	[esp+0x10], edx
		lea	edi, [esp+0x14]
		mov	[esp], edi
		mov	[esp+0x4], esi
		mov	[esp+0x8], esi
		sub	[esp+0x8], eax
		call	_memcpy
		add	esp, 12
		call	_fscanf
		mov	esp, ebx
		add	esp, 16
		pop	ebx
		pop	esi
		pop	edi
		pop	ebp
		ret
	def_end
}

macro	___ms_vfwscanf	nul
{
	def_local
		memcpy
		fwscanf
		_memcpy
		_fwscanf

	def_start 64, ___ms_vfwscanf
		push	rdi
		push	rsi
		push	rbx
		sub	rsp, 16
		lea	rax, [12+rsp]
		mov	rsi, r8
		mov	rbx, rsp
		lea	rsp, [rsp+rax-0x28]
		sub	rsp, rsi
		mov	[rsp+0x18], rcx
		mov	[rsp+0x20], rdx
		lea	rdi, [rsp+0x28]
		mov	[rsp], rdi
		mov	[rsp+0x8], rsi
		mov	[rsp+0x10], rsi
		sub	[rsp+0x10], rax
		mov	r8, [rsp+0x10]
		mov	rdx, [rsp+0x8]
		mov	rcx, [rsp]
		call	memcpy
		add	rsp, 24
		mov	r9, [rsp+0x18]
		mov	r8, [rsp+0x10]
		mov	rdx, [rsp+0x8]
		mov	rcx, [rsp]
		call	fwscanf
		mov	rsp, rbx
		add	rsp, 16
		pop	rbx
		pop	rsi
		pop	rdi
		ret
	def_end

	def_start 32, ___ms_vfwscanf
		push	ebp
		mov	ebp, esp
		push	edi
		push	esi
		push	ebx
		lea	eax, [ebp-16]
		sub	esp, 16
		mov	ecx, dword [ebp+8]
		mov	edx, dword [ebp+12]
		mov	esi, dword [ebp+16]
		mov	ebx, esp
		lea	esp, [esp+eax-0x14]
		sub	esp, esi
		mov	[esp+0xC], ecx
		mov	[esp+0x10], edx
		lea	edi, [esp+0x14]
		mov	[esp], edi
		mov	[esp+0x4], esi
		mov	[esp+0x8], esi
		sub	[esp+0x8], eax
		call	_memcpy
		add	esp, 12
		call	_fwscanf
		mov	esp, ebx
		add	esp, 16
		pop	ebx
		pop	esi
		pop	edi
		pop	ebp
		ret
	def_end
}

macro	___ms_vscanf	nul
{
	def_local
		_iob_func
		__ms_vfscanf
		___iob_func
		___ms_vfscanf

	def_start 64, ___ms_vscanf
		push	rbx
		sub	rsp, 48
		mov	rbx, rcx
		mov	qword [40+rsp], rdx
		call	qword _iob_func
		mov	r8, qword [40+rsp]
		mov	rdx, rbx
		mov	rcx, rax
		add	rsp, 48
		pop	rbx
		jmp	__ms_vfscanf
	def_end

	def_start 32, ___ms_vscanf
		push	ebp
		mov	ebp, esp
		sub	esp, 24
		call	___iob_func
		mov	edx, dword [ebp+12]
		mov	dword [esp], eax
		mov	dword [esp+8], edx
		mov	edx, dword [ebp+8]
		mov	dword [esp+4], edx
		call	___ms_vfscanf
		leave
		ret
	def_end
}

macro	___ms_vsscanf	nul
{
	def_local
		memcpy
		sscanf
		_memcpy
		_sscanf

	def_start 64, ___ms_vsscanf
		push	rdi
		push	rsi
		push	rbx
		sub	rsp, 16
		lea	rax, [12+rsp]
		mov	rsi, r8
		mov	rbx, rsp
		lea	rsp, [rsp+rax-0x28]
		sub	rsp, rsi
		mov	[rsp+0x18], rcx
		mov	[rsp+0x20], rdx
		lea	rdi, [rsp+0x28]
		mov	[rsp], rdi
		mov	[rsp+0x8], rsi
		mov	[rsp+0x10], rsi
		sub	[rsp+0x10], rax
		mov	r8, [rsp+0x10]
		mov	rdx, [rsp+0x8]
		mov	rcx, [rsp]
		call	memcpy
		add	rsp, 24
		mov	r9, [rsp+0x18]
		mov	r8, [rsp+0x10]
		mov	rdx, [rsp+0x8]
		mov	rcx, [rsp]
		call	sscanf
		mov	rsp, rbx
		add	rsp, 16
		pop	rbx
		pop	rsi
		pop	rdi
		ret
	def_end

	def_start 32, ___ms_vsscanf
		push	ebp
		mov	ebp, esp
		push	edi
		push	esi
		push	ebx
		lea	eax, [ebp-16]
		sub	esp, 16
		mov	ecx, dword [ebp+8]
		mov	edx, dword [ebp+12]
		mov	esi, dword [ebp+16]
		mov	ebx, esp
		lea	esp, [esp+eax-0x14]
		sub	esp, esi
		mov	[esp+0xC], ecx
		mov	[esp+0x10], edx
		lea	edi, [esp+0x14]
		mov	[esp], edi
		mov	[esp+0x4], esi
		mov	[esp+0x8], esi
		sub	[esp+0x8], eax
		call	_memcpy
		add	esp, 12
		call	_sscanf
		mov	esp, ebx
		add	esp, 16
		pop	ebx
		pop	esi
		pop	edi
		pop	ebp
		ret
	def_end
}

macro	___ms_vswscanf	nul
{
	def_local
		memcpy
		swscanf
		_memcpy
		_swscanf

	def_start 64, ___ms_vswscanf
		push	rdi
		push	rsi
		push	rbx
		sub	rsp, 16
		lea	rax, [12+rsp]
		mov	rsi, r8
		mov	rbx, rsp
		lea	rsp, [rsp+rax-0x28]
		sub	rsp, rsi
		mov	[rsp+0x18], rcx
		mov	[rsp+0x20], rdx
		lea	rdi, [rsp+0x28]
		mov	[rsp], rdi
		mov	[rsp+0x8], rsi
		mov	[rsp+0x10], rsi
		sub	[rsp+0x10], rax
		mov	r8, [rsp+0x10]
		mov	rdx, [rsp+0x8]
		mov	rcx, [rsp]
		call	memcpy
		add	rsp, 24
		mov	r9, [rsp+0x18]
		mov	r8, [rsp+0x10]
		mov	rdx, [rsp+0x8]
		mov	rcx, [rsp]
		call	swscanf
		mov	rsp, rbx
		add	rsp, 16
		pop	rbx
		pop	rsi
		pop	rdi
		ret
	def_end

	def_start 32, ___ms_vswscanf
		push	ebp
		mov	ebp, esp
		push	edi
		push	esi
		push	ebx
		lea	eax, [ebp-16]
		sub	esp, 16
		mov	ecx, dword [ebp+8]
		mov	edx, dword [ebp+12]
		mov	esi, dword [ebp+16]
		mov	ebx, esp
		lea	esp, [esp+eax-0x14]
		sub	esp, esi
		mov	[esp+0xC], ecx
		mov	[esp+0x10], edx
		lea	edi, [esp+0x14]
		mov	[esp], edi
		mov	[esp+0x4], esi
		mov	[esp+0x8], esi
		sub	[esp+0x8], eax
		call	_memcpy
		add	esp, 12
		call	_swscanf
		mov	esp, ebx
		add	esp, 16
		pop	ebx
		pop	esi
		pop	edi
		pop	ebp
		ret
	def_end
}

macro	___ms_vwscanf	nul
{
	def_local
		_iob_func
		__ms_vfwscanf
		___iob_func
		___ms_vfwscanf

	def_start 64, ___ms_vwscanf
		push	rbx
		sub	rsp, 48
		mov	rbx, rcx
		mov	qword [40+rsp], rdx
		call	qword _iob_func
		mov	r8, qword [40+rsp]
		mov	rdx, rbx
		mov	rcx, rax
		add	rsp, 48
		pop	rbx
		jmp	__ms_vfwscanf
	def_end

	def_start 32, ___ms_vwscanf
		push	ebp
		mov	ebp, esp
		sub	esp, 24
		call	___iob_func
		mov	edx, dword [ebp+12]
		mov	dword [esp], eax
		mov	dword [esp+8], edx
		mov	edx, dword [ebp+8]
		mov	dword [esp+4], edx
		call	___ms_vfwscanf
		leave
		ret
	def_end
}

macro	import	api
{
	setbits
	if bits = 64
		db	0x48, 0xB8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0
		db	api, 0
	else if bits = 32
		db	0xB8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0
		db	api, 0
	end if
}

macro def_api dll, number, [api]
{
	local	api_address
	common
		setbits
	forward
		api_address:
		if used api | used _#api
			macro api n
			\{
				if ~ defined api | defined @f
			@@:
				label api at api_address
				end if
			\}
			macro _#api n
			\{
				if ~ defined _#api | defined @f
			@@:
				label _#api at api_address
				end if
			\}
			if bits = 64
				db	0x48, 0xB8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0
			else if bits = 32
				db	0xB8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0
			end if

			if currentdll <> number
				display `dll, 0Dh, 0Ah
				display `api, 0Dh, 0Ah
				db	`dll#"|"#`api, 0
				currentdll = number
			else
				display `api, 0Dh, 0Ah
				db	`api, 0
			end if
		end if
}

macro def_api_ dll, number, [api]
{
	local	api_address
	common
		setbits
		setdllname = 1
	forward
		api_address:
		if used api
			macro api n
			\{
				if ~ defined api | defined @f
			@@:
				label api at api_address
				end if
			\}
			if bits = 64
				db	0x48, 0xB8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0
			else if bits = 32
				db	0xB8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0
			end if

			if currentdll <> number
				display `dll, 0Dh, 0Ah
				display "_"#`api, 0Dh, 0Ah
				db	`dll#"|_"#`api, 0
				currentdll = number
			else
				display "_"#`api, 0Dh, 0Ah
				db	"_"#`api, 0
			end if
		end if
}

macro def_adr dll, number, [api]
{
	local	api_address
	common
		setbits
	forward
		api_address:
		if used api | used _#api
			macro api n
			\{
				if ~ defined api | defined @f
			@@:
				setbits
				label api at api_address + bits/32
				end if
			\}
			macro _#api n
			\{
				if ~ defined _#api | defined @f
			@@:
				setbits
				label _#api at api_address + bits/32
				end if
			\}
			if bits = 64
				db	0x48, 0xB8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0
			else if bits = 32
				db	0xB8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0
			end if

			if currentdll <> number
				display `dll, 0Dh, 0Ah
				display `api # " (address)", 0Dh, 0Ah
				db	`dll#"|"#`api, 0
				currentdll = number
			else
				display `api # " (address)", 0Dh, 0Ah
				db	`api, 0
			end if
		end if
}

currentdll = 0
def_api msvcrt.dll,1,__crtCompareStringA,__crtCompareStringW,__crtGetLocaleInfoW,__crtGetStringTypeW,__crtLCMapStringA,__crtLCMapStringW,__dllonexit,__doserrno,__fpecode,__getmainargs,__iob_func,__isascii,__iscsym,__iscsymf,__lc_codepage,__lc_collate_cp,__lc_handle,__lconv_init,__libm_sse2_acos,__libm_sse2_acosf,__libm_sse2_asin,__libm_sse2_asinf,__libm_sse2_atan,__libm_sse2_atan2,__libm_sse2_atanf,__libm_sse2_cos,__libm_sse2_cosf,__libm_sse2_exp,__libm_sse2_expf,__libm_sse2_log,__libm_sse2_log10,__libm_sse2_log10f,__libm_sse2_logf,__libm_sse2_pow,__libm_sse2_powf,__libm_sse2_sin,__libm_sse2_sinf,__libm_sse2_tan,__libm_sse2_tanf,__mb_cur_max,__p___argc,__p___argv,__p___initenv,__p___mb_cur_max,__p___wargv,__p___winitenv,__p__acmdln,__p__amblksiz,__p__commode,__p__daylight,__p__dstbias,__p__environ,__p__fileinfo,__p__fmode,__p__iob,__p__mbcasemap,__p__mbctype,__p__osver,__p__pctype,__p__pgmptr,__p__pwctype,__p__timezone,__p__tzname,__p__wcmdln,__p__wenviron,__p__winmajor,__p__winminor,__p__winver,__p__wpgmptr,__pctype_func,__pwctype_func,__pxcptinfoptrs,__set_app_type,__setlc_active,__setusermatherr,__strncnt,__threadhandle,__threadid,__toascii,__unDName,__unDNameEx,__uncaught_exception,__unguarded_readlc_active,__wargv,__wcserror,__wcserror_s,__wcsncnt,__wgetmainargs,_abnormal_termination,_abs64,_access,_access_s,_adj_fdiv_m16i,_adj_fdiv_m32,_adj_fdiv_m32i,_adj_fdiv_m64,_adj_fdiv_r,_adj_fdivr_m16i,_adj_fdivr_m32,_adj_fdivr_m32i,_adj_fdivr_m64,_adj_fpatan,_adj_fprem,_adj_fprem1,_adj_fptan,_adjust_fdiv,_aligned_free,_aligned_free_dbg,_aligned_malloc,_aligned_malloc_dbg,_aligned_offset_malloc,_aligned_offset_malloc_dbg,_aligned_offset_realloc,_aligned_offset_realloc_dbg,_aligned_realloc,_aligned_realloc_dbg,_amsg_exit,_atodbl,_atodbl_l,_atof_l,_atoflt_l,_atoi64,_atoi64_l,_atoi_l,_atol_l,_atoldbl,_atoldbl_l,_assert,_beep,_beginthread,_beginthreadex,_c_exit,_cabs,_callnewh,_calloc_dbg,_cexit,_cgets,_cgets_s,_cgetws,_cgetws_s,_chdir,_chdrive,_chgsign,_chkesp,_chmod,_chsize,_chsize_s,_chvalidator,_chvalidator_l,_clearfp,_close,_commit,_control87,_controlfp,_controlfp_s,_copysign,_cprintf,_cprintf_l,_cprintf_p,_cprintf_p_l,_cprintf_s,_cprintf_s_l,_cputs,_cputws,_creat,_create_locale,_crtAssertBusy,_crtBreakAlloc,_crtDbgFlag,_cscanf,_cscanf_l,_cscanf_s,_cscanf_s_l,_ctime32,_ctime32_s,_ctime64,_ctime64_s,_ctype,_cwait,_cwprintf,_cwprintf_l,_cwprintf_p,_cwprintf_p_l,_cwprintf_s,_cwprintf_s_l,_cwscanf,_cwscanf_l,_cwscanf_s,_cwscanf_s_l,_difftime32,_difftime64,_dup,_dup2,_ecvt,_ecvt_s,_endthread,_endthreadex,_eof,_errno,_except_handler2,_except_handler3,_except_handler4_common,_execl,_execle,_execlp,_execlpe,_execv,_execve,_execvp,_execvpe,_exit,_expand,_expand_dbg,_fcloseall,_fcvt,_fcvt_s,_fdopen,_fgetchar,_fgetwchar,_filbuf,_filelength,_filelengthi64,_fileno,_findclose,_findfirst,_findfirst64,_findfirsti64,_findnext,_findnext64,_findnexti64,_finite,_flsbuf,_flushall,_fpclass,_fpieee_flt,_fprintf_l,_fprintf_p,_fprintf_p_l,_fprintf_s_l,_fputchar,_fputwchar,_free_dbg,_free_locale,_freea,_freea_s,_fscanf_l,_fscanf_s_l,_fseeki64,_fsopen,_fstat,_fstat64,_fstati64,_ftime,_ftime32,_ftime32_s,_ftime64,_ftime64_s,_ftol,_ftol2,_ftol2_sse,_ftol2_sse_excpt,_fullpath,_fullpath_dbg,_futime,_futime32,_futime64,_fwprintf_l,_fwprintf_p,_fwprintf_p_l,_fwprintf_s_l,_fwscanf_l,_fwscanf_s_l,_gcvt,_gcvt_s,_get_current_locale,_get_doserrno,_get_environ,_get_errno,_get_fileinfo,_get_fmode,_get_heap_handle,_get_osfhandle,_get_osplatform,_get_osver,_get_output_format,_get_pgmptr,_get_sbh_threshold,_get_wenviron,_get_winmajor,_get_winminor,_get_winver,_get_wpgmptr,_getch,_getche,_getcwd,_getdcwd,_getdiskfree,_getdllprocaddr,_getdrive,_getdrives,_getmaxstdio,_getmbcp,_getpid,_getsystime,_getw,_getwch,_getwche,_getws,_global_unwind2,_gmtime32,_gmtime32_s,_gmtime64,_gmtime64_s,_heapadd,_heapchk,_heapmin,_heapset,_heapused,_heapwalk,_hypot,_i64toa,_i64toa_s,_i64tow,_i64tow_s,_initterm,_initterm_e,_inp,_inpd,_inpw,_invalid_parameter,_isalnum_l,_isalpha_l,_isatty,_iscntrl_l,_isctype,_isctype_l,_isdigit_l,_isgraph_l,_isleadbyte_l,_islower_l,_ismbbalnum,_ismbbalnum_l,_ismbbalpha,_ismbbalpha_l,_ismbbgraph,_ismbbgraph_l,_ismbbkalnum,_ismbbkalnum_l,_ismbbkana,_ismbbkana_l,_ismbbkprint,_ismbbkprint_l,_ismbbkpunct,_ismbbkpunct_l,_ismbblead,_ismbblead_l,_ismbbprint,_ismbbprint_l,_ismbbpunct,_ismbbpunct_l,_ismbbtrail,_ismbbtrail_l,_ismbcalnum,_ismbcalnum_l,_ismbcalpha,_ismbcalpha_l,_ismbcdigit,_ismbcdigit_l,_ismbcgraph,_ismbcgraph_l,_ismbchira,_ismbchira_l,_ismbckata,_ismbckata_l,_ismbcl0,_ismbcl0_l,_ismbcl1,_ismbcl1_l,_ismbcl2,_ismbcl2_l,_ismbclegal,_ismbclegal_l,_ismbclower,_ismbclower_l,_ismbcprint,_ismbcprint_l,_ismbcpunct,_ismbcpunct_l,_ismbcspace,_ismbcspace_l,_ismbcsymbol,_ismbcsymbol_l,_ismbcupper,_ismbcupper_l,_ismbslead,_ismbslead_l,_ismbstrail,_ismbstrail_l,_isnan,_isprint_l,_isspace_l,_isupper_l,_iswalnum_l,_iswalpha_l,_iswcntrl_l,_iswctype_l,_iswdigit_l,_iswgraph_l,_iswlower_l,_iswprint_l,_iswpunct_l,_iswspace_l,_iswupper_l,_iswxdigit_l,_isxdigit_l,_itoa,_itoa_s,_itow,_itow_s,_j0,_j1,_jn,_kbhit,_lfind,_lfind_s,_libm_sse2_acos_precise,_libm_sse2_asin_precise,_libm_sse2_atan_precise,_libm_sse2_cos_precise,_libm_sse2_exp_precise,_libm_sse2_log10_precise,_libm_sse2_log_precise,_libm_sse2_pow_precise,_libm_sse2_sin_precise,_libm_sse2_sqrt_precise,_libm_sse2_tan_precise,_loaddll,_local_unwind2,_local_unwind4,_localtime32,_localtime32_s,_localtime64,_localtime64_s,_lock,_locking,_logb,_longjmpex,_lrotl,_lrotr,_lsearch,_lsearch_s,_lseek,_lseeki64,_ltoa,_ltoa_s,_ltow,_ltow_s,_makepath,_makepath_s,_malloc_dbg,_mbbtombc,_mbbtombc_l,_mbbtype,_mbccpy,_mbccpy_l,_mbccpy_s,_mbccpy_s_l,_mbcjistojms,_mbcjistojms_l,_mbcjmstojis,_mbcjmstojis_l,_mbclen,_mbclen_l,_mbctohira,_mbctohira_l,_mbctokata,_mbctokata_l,_mbctolower,_mbctolower_l,_mbctombb,_mbctombb_l,_mbctoupper,_mbctoupper_l,_mblen_l,_mbsbtype,_mbsbtype_l,_mbscat,_mbscat_s,_mbscat_s_l,_mbschr,_mbschr_l,_mbscmp,_mbscmp_l,_mbscoll,_mbscoll_l,_mbscpy,_mbscpy_s,_mbscpy_s_l,_mbscspn,_mbscspn_l,_mbsdec,_mbsdec_l,_mbsdup,_mbsicmp,_mbsicmp_l,_mbsicoll,_mbsicoll_l,_mbsinc,_mbsinc_l,_mbslen,_mbslen_l,_mbslwr,_mbslwr_l,_mbslwr_s,_mbslwr_s_l,_mbsnbcat,_mbsnbcat_l,_mbsnbcat_s,_mbsnbcat_s_l,_mbsnbcmp,_mbsnbcmp_l,_mbsnbcnt,_mbsnbcnt_l,_mbsnbcoll,_mbsnbcoll_l,_mbsnbcpy,_mbsnbcpy_l,_mbsnbcpy_s,_mbsnbcpy_s_l,_mbsnbicmp,_mbsnbicmp_l,_mbsnbicoll,_mbsnbicoll_l,_mbsnbset,_mbsnbset_l,_mbsnbset_s,_mbsnbset_s_l,_mbsncat,_mbsncat_l,_mbsncat_s,_mbsncat_s_l,_mbsnccnt,_mbsnccnt_l,_mbsncmp,_mbsncmp_l,_mbsncoll,_mbsncoll_l,_mbsncpy,_mbsncpy_l,_mbsncpy_s,_mbsncpy_s_l,_mbsnextc,_mbsnextc_l,_mbsnicmp,_mbsnicmp_l,_mbsnicoll,_mbsnicoll_l,_mbsninc,_mbsninc_l,_mbsnlen,_mbsnlen_l,_mbsnset,_mbsnset_l,_mbsnset_s,_mbsnset_s_l,_mbspbrk,_mbspbrk_l,_mbsrchr,_mbsrchr_l,_mbsrev,_mbsrev_l,_mbsset,_mbsset_l,_mbsset_s,_mbsset_s_l,_mbsspn,_mbsspn_l,_mbsspnp,_mbsspnp_l,_mbsstr,_mbsstr_l,_mbstok,_mbstok_l,_mbstok_s,_mbstok_s_l,_mbstowcs_l,_mbstowcs_s_l,_mbstrlen,_mbstrlen_l,_mbstrnlen,_mbstrnlen_l,_mbsupr,_mbsupr_l,_mbsupr_s,_mbsupr_s_l,_mbtowc_l,_memccpy,_memicmp,_memicmp_l,_mkdir,_mkgmtime,_mkgmtime32,_mkgmtime64,_mktemp,_mktemp_s,_mktime32,_mktime64,_msize,_msize_debug,_nextafter,_onexit,_open,_open_osfhandle,_outp,_outpd,_outpw,_pclose,_pipe,_popen,_printf_l,_printf_p,_printf_p_l,_printf_s_l,_purecall,_putch,_putenv,_putenv_s,_putw,_putwch,_putws,_read,_realloc_dbg,_resetstkoflw,_rmdir,_rmtmp,_rotl,_rotl64,_rotr,_rotr64,_safe_fdiv,_safe_fdivr,_safe_fprem,_safe_fprem1,_scalb,_scanf_l,_scanf_s_l,_scprintf,_scprintf_l,_scprintf_p_l,_scwprintf,_scwprintf_l,_scwprintf_p_l,_searchenv,_searchenv_s,_seh_longjmp_unwind,_seh_longjmp_unwind4,_set_SSE2_enable,_set_controlfp,_set_doserrno,_set_errno,_set_error_mode,_set_fileinfo,_set_fmode,_set_output_format,_set_sbh_threshold,_seterrormode,_setjmp,_setjmp3,_setmaxstdio,_setmbcp,_setmode,_setsystime,_sleep,_snprintf,_snprintf_c,_snprintf_c_l,_snprintf_l,_snprintf_s,_snprintf_s_l,_snscanf,_snscanf_l,_snscanf_s,_snscanf_s_l,_snwprintf,_snwprintf_l,_snwprintf_s,_snwprintf_s_l,_snwscanf,_snwscanf_l,_snwscanf_s,_snwscanf_s_l,_sopen,_sopen_s,_spawnl,_spawnle,_spawnlp,_spawnlpe,_spawnv,_spawnve,_spawnvp,_spawnvpe,_splitpath,_splitpath_s,_sprintf_l,_sprintf_p_l,_sprintf_s_l,_sscanf_l,_sscanf_s_l,_stat,_stat64,_stati64,_statusfp,_strcmpi,_strcoll_l,_strdate,_strdate_s,_strdup,_strdup_dbg,_strerror,_strerror_s,_stricmp,_stricmp_l,_stricoll,_stricoll_l,_strlwr,_strlwr_l,_strlwr_s,_strlwr_s_l,_strncoll,_strncoll_l,_strnicmp,_strnicmp_l,_strnicoll,_strnicoll_l,_strnset,_strnset_s,_strrev,_strset,_strset_s,_strtime,_strtime_s,_strtod_l,_strtoi64,_strtoi64_l,_strtol_l,_strtoui64,_strtoui64_l,_strtoul_l,_strupr,_strupr_l,_strupr_s,_strupr_s_l,_strxfrm_l,_swab,_swprintf,_swprintf_c,_swprintf_c_l,_swprintf_p_l,_swprintf_s_l,_swscanf_l,_swscanf_s_l,_tell,_telli64,_tempnam,_tempnam_dbg,_time32,_time64,_tolower,_tolower_l,_toupper,_toupper_l,_towlower_l,_towupper_l,_tzset,_ui64toa,_ui64toa_s,_ui64tow,_ui64tow_s,_ultoa,_ultoa_s,_ultow,_ultow_s,_umask,_umask_s,_ungetch,_ungetwch,_unlink,_unloaddll,_unlock,_utime,_utime32,_utime64,_vcprintf,_vcprintf_l,_vcprintf_p,_vcprintf_p_l,_vcprintf_s,_vcprintf_s_l,_vcwprintf,_vcwprintf_l,_vcwprintf_p,_vcwprintf_p_l,_vcwprintf_s,_vcwprintf_s_l,_vfprintf_l,_vfprintf_p,_vfprintf_p_l,_vfprintf_s_l,_vfwprintf_l,_vfwprintf_p,_vfwprintf_p_l,_vfwprintf_s_l,_vprintf_l,_vprintf_p,_vprintf_p_l,_vprintf_s_l,_vscprintf,_vscprintf_l,_vscprintf_p_l,_vscwprintf,_vscwprintf_l,_vscwprintf_p_l,_vsnprintf,_vsnprintf_c,_vsnprintf_c_l,_vsnprintf_l,_vsnprintf_s,_vsnprintf_s_l,_vsnwprintf,_vsnwprintf_l,_vsnwprintf_s,_vsnwprintf_s_l,_vsprintf_l,_vsprintf_p,_vsprintf_p_l,_vsprintf_s_l,_vswprintf,_vswprintf_c,_vswprintf_c_l,_vswprintf_l,_vswprintf_p_l,_vswprintf_s_l,_vwprintf_l,_vwprintf_p,_vwprintf_p_l,_vwprintf_s_l,_waccess,_waccess_s,_wasctime,_wasctime_s,_wassert,_wchdir,_wchmod,_wcreat,_wcscoll_l,_wcsdup,_wcsdup_dbg,_wcserror,_wcserror_s,_wcsftime_l,_wcsicmp,_wcsicmp_l,_wcsicoll,_wcsicoll_l,_wcslwr,_wcslwr_l,_wcslwr_s,_wcslwr_s_l,_wcsncoll,_wcsncoll_l,_wcsnicmp,_wcsnicmp_l,_wcsnicoll,_wcsnicoll_l,_wcsnset,_wcsnset_s,_wcsrev,_wcsset,_wcsset_s,_wcstod_l,_wcstoi64,_wcstoi64_l,_wcstol_l,_wcstombs_l,_wcstombs_s_l,_wcstoui64,_wcstoui64_l,_wcstoul_l,_wcsupr,_wcsupr_l,_wcsupr_s,_wcsupr_s_l,_wcsxfrm_l,_wctime,_wctime32,_wctime32_s,_wctime64,_wctime64_s,_wctomb_l,_wctomb_s_l,_wexecl,_wexecle,_wexeclp,_wexeclpe,_wexecv,_wexecve,_wexecvp,_wexecvpe,_wfdopen,_wfindfirst,_wfindfirst64,_wfindfirsti64,_wfindnext,_wfindnext64,_wfindnexti64,_wfopen,_wfopen_s,_wfreopen,_wfreopen_s,_wfsopen,_wfullpath,_wfullpath_dbg,_wgetcwd,_wgetdcwd,_wgetenv,_wgetenv_s,_winput_s,_wmakepath,_wmakepath_s,_wmkdir,_wmktemp,_wmktemp_s,_wopen,_woutput_s,_wperror,_wpopen,_wprintf_l,_wprintf_p,_wprintf_p_l,_wprintf_s_l,_wputenv,_wputenv_s,_wremove,_wrename,_write,_wrmdir,_wscanf_l,_wscanf_s_l,_wsearchenv,_wsearchenv_s,_wsetlocale,_wsopen,_wsopen_s,_wspawnl,_wspawnle,_wspawnlp,_wspawnlpe,_wspawnv,_wspawnve,_wspawnvp,_wspawnvpe,_wsplitpath,_wsplitpath_s,_wstat,_wstat64,_wstati64,_wstrdate,_wstrdate_s,_wstrtime,_wstrtime_s,_wsystem,_wtempnam,_wtempnam_dbg,_wtmpnam,_wtmpnam_s,_wtof,_wtof_l,_wtoi,_wtoi64,_wtoi64_l,_wtoi_l,_wtol,_wtol_l,_wunlink,_wutime,_wutime32,_wutime64,_y0,_y1,_yn,abort,abs,acos,asctime,asctime_s,asin,atan,atan2,atexit,atof,atoi,atol,bsearch,bsearch_s,btowc,calloc,ceil,clearerr,clearerr_s,clock,cos,cosh,ctime,difftime,exit,exp,fclose,feof,ferror,fflush,fgetc,fgetpos,fgets,fgetwc,fgetws,floor,fmod,fopen,fopen_s,fprintf,fprintf_s,fputc,fputs,fputwc,fputws,fread,free,freopen,freopen_s,frexp,fscanf,fscanf_s,fseek,fsetpos,ftell,fwprintf,fwprintf_s,fwrite,fwscanf,fwscanf_s,getc,getchar,getenv,getenv_s,gets,getwc,getwchar,gmtime,is_wctype,isalnum,isalpha,iscntrl,isdigit,isgraph,isleadbyte,islower,isprint,ispunct,isspace,isupper,iswalnum,iswalpha,iswascii,iswcntrl,iswctype,iswdigit,iswgraph,iswlower,iswprint,iswpunct,iswspace,iswupper,iswxdigit,isxdigit,labs,ldexp,ldiv,localeconv,localtime,log,log10,longjmp,malloc,mblen,mbrlen,mbrtowc,mbsdup_dbg,mbsrtowcs,mbsrtowcs_s,mbstowcs,mbstowcs_s,mbtowc,memchr,memcmp,memcpy,memcpy_s,memmove,memmove_s,memset,mktime,modf,perror,pow,printf,printf_s,putc,putchar,puts,putwc,putwchar,qsort,qsort_s,raise,rand,rand_s,realloc,remove,rename,rewind,scanf,scanf_s,setbuf,setlocale,setvbuf,signal,sin,sinh,sprintf,sprintf_s,sqrt,srand,sscanf,sscanf_s,strcat,strcat_s,strchr,strcmp,strcoll,strcpy,strcpy_s,strcspn,strerror,strerror_s,strftime,strlen,strncat,strncat_s,strncmp,strncpy,strncpy_s,strnlen,strpbrk,strrchr,strspn,strstr,strtod,strtok,strtok_s,strtol,strtoul,strxfrm,swprintf,swprintf_s,swscanf,swscanf_s,system,tan,tanh,time,tmpfile,tmpfile_s,tmpnam,tmpnam_s,tolower,toupper,towlower,towupper,ungetc,ungetwc,utime,vfprintf,vfprintf_s,vfwprintf,vfwprintf_s,vprintf,vprintf_s,vsnprintf,vsprintf,vsprintf_s,vswprintf,vswprintf_s,vwprintf,vwprintf_s,wcrtomb,wcrtomb_s,wcscat,wcscat_s,wcschr,wcscmp,wcscoll,wcscpy,wcscpy_s,wcscspn,wcsftime,wcslen,wcsncat,wcsncat_s,wcsncmp,wcsncpy,wcsncpy_s,wcsnlen,wcspbrk,wcsrchr,wcsrtombs,wcsrtombs_s,wcsspn,wcsstr,wcstod,wcstok,wcstok_s,wcstol,wcstombs,wcstombs_s,wcstoul,wcsxfrm,wctob,wctomb,wctomb_s,wprintf,wprintf_s,wscanf,wscanf_s ; unsupported api: div,fabs
def_api_ msvcrt.dll,1,_crtCompareStringA,_crtCompareStringW,_crtGetLocaleInfoW,_crtGetStringTypeW,_crtLCMapStringA,_crtLCMapStringW,_dllonexit,_doserrno,_fpecode,_getmainargs,_iob_func,_isascii,_iscsym,_iscsymf,_lc_codepage,_lc_collate_cp,_lc_handle,_lconv_init,_libm_sse2_acos,_libm_sse2_acosf,_libm_sse2_asin,_libm_sse2_asinf,_libm_sse2_atan,_libm_sse2_atan2,_libm_sse2_atanf,_libm_sse2_cos,_libm_sse2_cosf,_libm_sse2_exp,_libm_sse2_expf,_libm_sse2_log,_libm_sse2_log10,_libm_sse2_log10f,_libm_sse2_logf,_libm_sse2_pow,_libm_sse2_powf,_libm_sse2_sin,_libm_sse2_sinf,_libm_sse2_tan,_libm_sse2_tanf,_mb_cur_max,_p___argc,_p___argv,_p___initenv,_p___mb_cur_max,_p___wargv,_p___winitenv,_p__acmdln,_p__amblksiz,_p__commode,_p__daylight,_p__dstbias,_p__environ,_p__fileinfo,_p__fmode,_p__iob,_p__mbcasemap,_p__mbctype,_p__osver,_p__pctype,_p__pgmptr,_p__pwctype,_p__timezone,_p__tzname,_p__wcmdln,_p__wenviron,_p__winmajor,_p__winminor,_p__winver,_p__wpgmptr,_pctype_func,_pwctype_func,_pxcptinfoptrs,_set_app_type,_setlc_active,_setusermatherr,_strncnt,_threadhandle,_threadid,_toascii,_unDName,_unDNameEx,_uncaught_exception,_unguarded_readlc_active,_wcserror,_wcserror_s,_wcsncnt,_wgetmainargs,abnormal_termination,abs64,access,access_s,adj_fdiv_m16i,adj_fdiv_m32,adj_fdiv_m32i,adj_fdiv_m64,adj_fdiv_r,adj_fdivr_m16i,adj_fdivr_m32,adj_fdivr_m32i,adj_fdivr_m64,adj_fpatan,adj_fprem,adj_fprem1,adj_fptan,adjust_fdiv,aligned_free,aligned_free_dbg,aligned_malloc,aligned_malloc_dbg,aligned_offset_malloc,aligned_offset_malloc_dbg,aligned_offset_realloc,aligned_offset_realloc_dbg,aligned_realloc,aligned_realloc_dbg,amsg_exit,atodbl,atodbl_l,atof_l,atoflt_l,atoi64,atoi64_l,atoi_l,atol_l,atoldbl,atoldbl_l,beep,beginthread,beginthreadex,c_exit,cabs,callnewh,calloc_dbg,cexit,cgets,cgets_s,cgetws,cgetws_s,chdir,chdrive,chgsign,chkesp,chmod,chsize,chsize_s,chvalidator,chvalidator_l,clearfp,close,commit,control87,controlfp,controlfp_s,copysign,cprintf,cprintf_l,cprintf_p,cprintf_p_l,cprintf_s,cprintf_s_l,cputs,cputws,creat,create_locale,crtAssertBusy,crtBreakAlloc,crtDbgFlag,cscanf,cscanf_l,cscanf_s,cscanf_s_l,ctime32,ctime32_s,ctime64,ctime64_s,ctype,cwait,cwprintf,cwprintf_l,cwprintf_p,cwprintf_p_l,cwprintf_s,cwprintf_s_l,cwscanf,cwscanf_l,cwscanf_s,cwscanf_s_l,difftime32,difftime64,dup2,ecvt,ecvt_s,endthread,endthreadex,eof,errno,except_handler2,except_handler3,except_handler4_common,execl,execle,execlp,execlpe,execv,execve,execvp,execvpe,exit,expand,expand_dbg,fcloseall,fcvt,fcvt_s,fdopen,fgetchar,fgetwchar,filbuf,filelength,filelengthi64,fileno,findclose,findfirst,findfirst64,findfirsti64,findnext,findnext64,findnexti64,finite,flsbuf,flushall,fpclass,fpieee_flt,fprintf_l,fprintf_p,fprintf_p_l,fprintf_s_l,fputchar,fputwchar,free_dbg,free_locale,freea,freea_s,fscanf_l,fscanf_s_l,fseeki64,fsopen,fstat,fstat64,fstati64,ftime,ftime32,ftime32_s,ftime64,ftime64_s,ftol,ftol2,ftol2_sse,ftol2_sse_excpt,fullpath,fullpath_dbg,futime,futime32,futime64,fwprintf_l,fwprintf_p,fwprintf_p_l,fwprintf_s_l,fwscanf_l,fwscanf_s_l,gcvt,gcvt_s,get_current_locale,get_doserrno,get_environ,get_errno,get_fileinfo,get_fmode,get_heap_handle,get_osfhandle,get_osplatform,get_osver,get_output_format,get_pgmptr,get_sbh_threshold,get_wenviron,get_winmajor,get_winminor,get_winver,get_wpgmptr,getch,getche,getcwd,getdcwd,getdiskfree,getdllprocaddr,getdrive,getdrives,getmaxstdio,getmbcp,getpid,getsystime,getw,getwch,getwche,getws,global_unwind2,gmtime32,gmtime32_s,gmtime64,gmtime64_s,heapadd,heapchk,heapmin,heapset,heapused,heapwalk,hypot,i64toa,i64toa_s,i64tow,i64tow_s,initterm,initterm_e,inp,inpd,inpw,invalid_parameter,isalnum_l,isalpha_l,isatty,iscntrl_l,isctype,isctype_l,isdigit_l,isgraph_l,isleadbyte_l,islower_l,ismbbalnum,ismbbalnum_l,ismbbalpha,ismbbalpha_l,ismbbgraph,ismbbgraph_l,ismbbkalnum,ismbbkalnum_l,ismbbkana,ismbbkana_l,ismbbkprint,ismbbkprint_l,ismbbkpunct,ismbbkpunct_l,ismbblead,ismbblead_l,ismbbprint,ismbbprint_l,ismbbpunct,ismbbpunct_l,ismbbtrail,ismbbtrail_l,ismbcalnum,ismbcalnum_l,ismbcalpha,ismbcalpha_l,ismbcdigit,ismbcdigit_l,ismbcgraph,ismbcgraph_l,ismbchira,ismbchira_l,ismbckata,ismbckata_l,ismbcl0,ismbcl0_l,ismbcl1,ismbcl1_l,ismbcl2,ismbcl2_l,ismbclegal,ismbclegal_l,ismbclower,ismbclower_l,ismbcprint,ismbcprint_l,ismbcpunct,ismbcpunct_l,ismbcspace,ismbcspace_l,ismbcsymbol,ismbcsymbol_l,ismbcupper,ismbcupper_l,ismbslead,ismbslead_l,ismbstrail,ismbstrail_l,isnan,isprint_l,isspace_l,isupper_l,iswalnum_l,iswalpha_l,iswcntrl_l,iswctype_l,iswdigit_l,iswgraph_l,iswlower_l,iswprint_l,iswpunct_l,iswspace_l,iswupper_l,iswxdigit_l,isxdigit_l,itoa,itoa_s,itow,itow_s,j0,j1,jn,kbhit,lfind,lfind_s,libm_sse2_acos_precise,libm_sse2_asin_precise,libm_sse2_atan_precise,libm_sse2_cos_precise,libm_sse2_exp_precise,libm_sse2_log10_precise,libm_sse2_log_precise,libm_sse2_pow_precise,libm_sse2_sin_precise,libm_sse2_sqrt_precise,libm_sse2_tan_precise,loaddll,local_unwind2,local_unwind4,localtime32,localtime32_s,localtime64,localtime64_s,locking,logb,longjmpex,lrotl,lrotr,lsearch,lsearch_s,lseek,lseeki64,ltoa,ltoa_s,ltow,ltow_s,makepath,makepath_s,malloc_dbg,mbbtombc,mbbtombc_l,mbbtype,mbccpy,mbccpy_l,mbccpy_s,mbccpy_s_l,mbcjistojms,mbcjistojms_l,mbcjmstojis,mbcjmstojis_l,mbclen,mbclen_l,mbctohira,mbctohira_l,mbctokata,mbctokata_l,mbctolower,mbctolower_l,mbctombb,mbctombb_l,mbctoupper,mbctoupper_l,mblen_l,mbsbtype,mbsbtype_l,mbscat,mbscat_s,mbscat_s_l,mbschr,mbschr_l,mbscmp,mbscmp_l,mbscoll,mbscoll_l,mbscpy,mbscpy_s,mbscpy_s_l,mbscspn,mbscspn_l,mbsdec,mbsdec_l,mbsdup,mbsicmp,mbsicmp_l,mbsicoll,mbsicoll_l,mbsinc,mbsinc_l,mbslen,mbslen_l,mbslwr,mbslwr_l,mbslwr_s,mbslwr_s_l,mbsnbcat,mbsnbcat_l,mbsnbcat_s,mbsnbcat_s_l,mbsnbcmp,mbsnbcmp_l,mbsnbcnt,mbsnbcnt_l,mbsnbcoll,mbsnbcoll_l,mbsnbcpy,mbsnbcpy_l,mbsnbcpy_s,mbsnbcpy_s_l,mbsnbicmp,mbsnbicmp_l,mbsnbicoll,mbsnbicoll_l,mbsnbset,mbsnbset_l,mbsnbset_s,mbsnbset_s_l,mbsncat,mbsncat_l,mbsncat_s,mbsncat_s_l,mbsnccnt,mbsnccnt_l,mbsncmp,mbsncmp_l,mbsncoll,mbsncoll_l,mbsncpy,mbsncpy_l,mbsncpy_s,mbsncpy_s_l,mbsnextc,mbsnextc_l,mbsnicmp,mbsnicmp_l,mbsnicoll,mbsnicoll_l,mbsninc,mbsninc_l,mbsnlen,mbsnlen_l,mbsnset,mbsnset_l,mbsnset_s,mbsnset_s_l,mbspbrk,mbspbrk_l,mbsrchr,mbsrchr_l,mbsrev,mbsrev_l,mbsset,mbsset_l,mbsset_s,mbsset_s_l,mbsspn,mbsspn_l,mbsspnp,mbsspnp_l,mbsstr,mbsstr_l,mbstok,mbstok_l,mbstok_s,mbstok_s_l,mbstowcs_l,mbstowcs_s_l,mbstrlen,mbstrlen_l,mbstrnlen,mbstrnlen_l,mbsupr,mbsupr_l,mbsupr_s,mbsupr_s_l,mbtowc_l,memccpy,memicmp,memicmp_l,mkdir,mkgmtime,mkgmtime32,mkgmtime64,mktemp,mktemp_s,mktime32,mktime64,msize,msize_debug,nextafter,onexit,open,open_osfhandle,outp,outpd,outpw,pclose,pipe,popen,printf_l,printf_p,printf_p_l,printf_s_l,purecall,putch,putenv,putenv_s,putw,putwch,putws,read,realloc_dbg,resetstkoflw,rmdir,rmtmp,rotl,rotl64,rotr,rotr64,safe_fdiv,safe_fdivr,safe_fprem,safe_fprem1,scalb,scanf_l,scanf_s_l,scprintf,scprintf_l,scprintf_p_l,scwprintf,scwprintf_l,scwprintf_p_l,searchenv,searchenv_s,seh_longjmp_unwind,seh_longjmp_unwind4,set_SSE2_enable,set_controlfp,set_doserrno,set_errno,set_error_mode,set_fileinfo,set_fmode,set_output_format,set_sbh_threshold,seterrormode,setjmp,setjmp3,setmaxstdio,setmbcp,setmode,setsystime,sleep,snprintf,snprintf_c,snprintf_c_l,snprintf_l,snprintf_s,snprintf_s_l,snscanf,snscanf_l,snscanf_s,snscanf_s_l,snwprintf,snwprintf_l,snwprintf_s,snwprintf_s_l,snwscanf,snwscanf_l,snwscanf_s,snwscanf_s_l,sopen,sopen_s,spawnl,spawnle,spawnlp,spawnlpe,spawnv,spawnve,spawnvp,spawnvpe,splitpath,splitpath_s,sprintf_l,sprintf_p_l,sprintf_s_l,sscanf_l,sscanf_s_l,stat,stat64,stati64,statusfp,strcmpi,strcoll_l,strdate,strdate_s,strdup,strdup_dbg,strerror,strerror_s,stricmp,stricmp_l,stricoll,stricoll_l,strlwr,strlwr_l,strlwr_s,strlwr_s_l,strncoll,strncoll_l,strnicmp,strnicmp_l,strnicoll,strnicoll_l,strnset,strnset_s,strrev,strset,strset_s,strtime,strtime_s,strtod_l,strtoi64,strtoi64_l,strtol_l,strtoui64,strtoui64_l,strtoul_l,strupr,strupr_l,strupr_s,strupr_s_l,strxfrm_l,swab,swprintf,swprintf_c,swprintf_c_l,swprintf_p_l,swprintf_s_l,swscanf_l,swscanf_s_l,tell,telli64,tempnam,tempnam_dbg,time32,time64,tolower,tolower_l,toupper,toupper_l,towlower_l,towupper_l,tzset,ui64toa,ui64toa_s,ui64tow,ui64tow_s,ultoa,ultoa_s,ultow,ultow_s,umask,umask_s,ungetch,ungetwch,unlink,unloaddll,unlock,utime,utime32,utime64,vcprintf,vcprintf_l,vcprintf_p,vcprintf_p_l,vcprintf_s,vcprintf_s_l,vcwprintf,vcwprintf_l,vcwprintf_p,vcwprintf_p_l,vcwprintf_s,vcwprintf_s_l,vfprintf_l,vfprintf_p,vfprintf_p_l,vfprintf_s_l,vfwprintf_l,vfwprintf_p,vfwprintf_p_l,vfwprintf_s_l,vprintf_l,vprintf_p,vprintf_p_l,vprintf_s_l,vscprintf,vscprintf_l,vscprintf_p_l,vscwprintf,vscwprintf_l,vscwprintf_p_l,vsnprintf,vsnprintf_c,vsnprintf_c_l,vsnprintf_l,vsnprintf_s,vsnprintf_s_l,vsnwprintf,vsnwprintf_l,vsnwprintf_s,vsnwprintf_s_l,vsprintf_l,vsprintf_p,vsprintf_p_l,vsprintf_s_l,vswprintf,vswprintf_c,vswprintf_c_l,vswprintf_l,vswprintf_p_l,vswprintf_s_l,vwprintf_l,vwprintf_p,vwprintf_p_l,vwprintf_s_l,waccess,waccess_s,wasctime,wasctime_s,wassert,wchdir,wchmod,wcreat,wcscoll_l,wcsdup,wcsdup_dbg,wcserror,wcserror_s,wcsftime_l,wcsicmp,wcsicmp_l,wcsicoll,wcsicoll_l,wcslwr,wcslwr_l,wcslwr_s,wcslwr_s_l,wcsncoll,wcsncoll_l,wcsnicmp,wcsnicmp_l,wcsnicoll,wcsnicoll_l,wcsnset,wcsnset_s,wcsrev,wcsset,wcsset_s,wcstod_l,wcstoi64,wcstoi64_l,wcstol_l,wcstombs_l,wcstombs_s_l,wcstoui64,wcstoui64_l,wcstoul_l,wcsupr,wcsupr_l,wcsupr_s,wcsupr_s_l,wcsxfrm_l,wctime,wctime32,wctime32_s,wctime64,wctime64_s,wctomb_l,wctomb_s_l,wexecl,wexecle,wexeclp,wexeclpe,wexecv,wexecve,wexecvp,wexecvpe,wfdopen,wfindfirst,wfindfirst64,wfindfirsti64,wfindnext,wfindnext64,wfindnexti64,wfopen,wfopen_s,wfreopen,wfreopen_s,wfsopen,wfullpath,wfullpath_dbg,wgetcwd,wgetdcwd,wgetenv,wgetenv_s,winput_s,wmakepath,wmakepath_s,wmkdir,wmktemp,wmktemp_s,wopen,woutput_s,wperror,wpopen,wprintf_l,wprintf_p,wprintf_p_l,wprintf_s_l,wputenv,wputenv_s,wremove,wrename,write,wrmdir,wscanf_l,wscanf_s_l,wsearchenv,wsearchenv_s,wsetlocale,wsopen,wsopen_s,wspawnl,wspawnle,wspawnlp,wspawnlpe,wspawnv,wspawnve,wspawnvp,wspawnvpe,wsplitpath,wsplitpath_s,wstat,wstat64,wstati64,wstrdate,wstrdate_s,wstrtime,wstrtime_s,wsystem,wtempnam,wtempnam_dbg,wtmpnam,wtmpnam_s,wtof,wtof_l,wtoi,wtoi64,wtoi64_l,wtoi_l,wtol,wtol_l,wunlink,wutime,wutime32,wutime64,y0,y1,yn ; unsupported api: dup,lock
def_adr msvcrt.dll,1,__argc,__argv,__badioinfo,__initenv,__pioinfo,__wargv,__winitenv,_acmdln,_aexit_rtn,_commode,_daylight,_dstbias,_environ,_fileinfo,_fmode,_fpreset,_iob,_mbcasemap,_mbctype,_osplatform,_osver,_pctype,_pgmptr,_pwctype,_sys_errlist,_sys_nerr,_timezone,_tzname,_wcmdln,_wctype,_wenviron,_winmajor,_winminor,_winver,_wpgmptr

def_label ___main, __main
def_label __floatundisf, ___floatundisf
def_label __floatundidf, ___floatundidf
def_label __floatundixf, ___floatundixf
def_label __fixunssfdi, ___fixunssfdi
def_label __fixunsdfdi, ___fixunsdfdi
def_label __fixunsxfdi, ___fixunsxfdi
def_label ___strtod, strtod
def_label __strtod, strtod
def_label strtof, _strtof
def_label strtoull, _strtoull
def_label strtoll, _strtoll
def_label _flush, __flush
def_label fseeko64, _fseeko64
def_label ftello64, _ftello64
def_label _fseeki64, __fseeki64
def_label _ftelli64, __ftelli64
def_label __ms_vsnprintf, _vsnprintf
def_label ___ms_vsnprintf, vsnprintf
def_label __ms_vsnwprintf, _vsnwprintf
def_label ___ms_vsnwprintf, vsnwprintf
def_label ___ms_snprintf, _snprintf
def_label __ms_snprintf, snprintf
def_label ___ms_snwprintf, _snwprintf
def_label __ms_snwprintf, snwprintf
def_label __ms_vfscanf, ___ms_vfscanf
def_label __ms_vfwscanf, ___ms_vfwscanf
def_label __ms_vscanf, ___ms_vscanf
def_label __ms_vsscanf, ___ms_vsscanf
def_label __ms_vswscanf, ___ms_vswscanf
def_label __ms_vwscanf, ___ms_vwscanf
