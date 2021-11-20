	.file	"ex2.c"
	.text
	.p2align 4
	.type	even, @function
	.global even
even:	# even function. - %edi has num, %esi has i  
	movl	%edi, %eax	# Save the number in the return value reg.
	movl	%esi, %ecx	# Move %esi (i) to index reg %ecx.
	sal	%ecx, %eax # num << i.
	ret


	.type	go, @function
	.global go
go:		# go function. - %edi has address of A[10]
	xorl	%ecx, %ecx	# initialise %ecx to 0 => i = 0 // more efficient then movl
	xorl	%eax, %eax	# initialise %eax to 0 => sum = 0. // more efficient then movl

.L2:	# if statment.
	movl	(%edi, %ecx, 4), %esi	# Acess array in index %ecx (= i) to %esi => %esi = A[i]
	movl	%esi, %edx	# Move esi to temp register for calculations => %edx = A[i]
	sal		%ecx, %edx	# Avoid calling even function and implement logic by shifiting %edx(A[i]) %ecx(i) times => A[i] << i // preperation for if statment.
	add 	%eax, %edx	# Add %eax (sum) to %edx (shifted value) // preperation for if statment.
	add		%esi, %eax	# Add %esi to %eax - meaning add A[i] to sum // preperation for else statment.
	testb	%al,  0		# check if LSB at %sil is 1 - meaning if A[i] % 2 == 0
	cmove	%edx, %eax	# if zero flag is on - meaning A[i] % 2 == 0, move %edx (sum + (A[i] << i)) inside %eax
    
.L1:	# while loop. - while(i < 10)
	add		$1, %ecx	# i++
	cmpl	$10, %ecx	# check if i < 10
	jne .L2		# if zero flag is off meaning i != 10 go to .L2 and check if statment.

    ret
