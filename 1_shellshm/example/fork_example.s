	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 11, 0	sdk_version 11, 1
	.globl	_main                   ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32             ; =32
	stp	x29, x30, [sp, #16]     ; 16-byte Folded Spill
	add	x29, sp, #16            ; =16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	stur	wzr, [x29, #-4]
	bl	_fork
	str	w0, [sp, #8]
	ldr	w8, [sp, #8]
	cmp	w8, #0                  ; =0
	cset	w8, ge
	tbnz	w8, #0, LBB0_2
; %bb.1:
	adrp	x8, ___stderrp@GOTPAGE
	ldr	x8, [x8, ___stderrp@GOTPAGEOFF]
	ldr	x0, [x8]
	adrp	x1, l_.str@PAGE
	add	x1, x1, l_.str@PAGEOFF
	bl	_fprintf
	mov	w9, #-1
	stur	w9, [x29, #-4]
	b	LBB0_7
LBB0_2:
	ldr	w8, [sp, #8]
	cbnz	w8, LBB0_4
; %bb.3:
	bl	_getpid
                                        ; implicit-def: $x1
	mov	x1, x0
	sxtw	x8, w1
	adrp	x0, l_.str.1@PAGE
	add	x0, x0, l_.str.1@PAGEOFF
	mov	x1, x8
	bl	_fprintf
	b	LBB0_5
LBB0_4:
	bl	_getpid
                                        ; implicit-def: $x1
	mov	x1, x0
	sxtw	x8, w1
	adrp	x0, l_.str.2@PAGE
	add	x0, x0, l_.str.2@PAGEOFF
	mov	x1, x8
	bl	_fprintf
	mov	x8, #0
	mov	x0, x8
	bl	_wait
LBB0_5:
; %bb.6:
	stur	wzr, [x29, #-4]
LBB0_7:
	ldur	w0, [x29, #-4]
	ldp	x29, x30, [sp, #16]     ; 16-byte Folded Reload
	add	sp, sp, #32             ; =32
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"fort failed!"

l_.str.1:                               ; @.str.1
	.asciz	"Child Process (# %i)"

l_.str.2:                               ; @.str.2
	.asciz	"Parent Process (# %i)"

.subsections_via_symbols
