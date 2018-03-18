#include "i8042.h"
#include "i8254.h"

.global _readOutBufASM
.global _scancodeASM

.data
    _scancodeASM: 
        .long 0

.text

_readOutBufASM:

xor %eax, %eax
xor %ebx, %ebx
inb $STAT_REG
movb %al, %bl
shr %eax
jnc error
inb $OUT_BUF
and $(PAR_ERR | TO_ERR), %ebx
jnz error
movl %eax, _scancodeASM
movl $0, %eax
jmp end

error:
movl $1, %eax

end: 
ret
