.globl _main
#.bb0:
_main:
    pushq		%rbp
    movq		%rsp, %rbp
    xorl		%eax, %eax
    subq		$32, %rsp
    movl		$5, %eax
    movl		%eax,-4(%rbp)
    movl		$0, %eax
    movl		%eax,-8(%rbp)
    movl		$0, %eax
    movl		%eax,-4(%rbp)
_blockEnd7:
    movl		$10, %eax
    movl		%eax,-4(%rbp)
_blockEnd9:
    movl		$10, %eax
    movl		%eax,-4(%rbp)
_blockEnd13:
    movl		$10, %eax
    movl		%eax,-12(%rbp)
    movl		$5, %eax
    movl		%eax,-16(%rbp)
    movl		-16(%rbp), %eax
    movq		%rbp, %rsp
    popq		%rbp
    ret
