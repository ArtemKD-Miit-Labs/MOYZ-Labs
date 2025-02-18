global _start

.data
    A db 100, -43, 33
    B db 67, 10, -50
    C db 10, 10, 10

.text
_start:
    mov eax, 10

_exit:
    mov eax, 1
    mov ebx, 10
    int 80h

; Вычисление sum( |Ai| * |Bi| )

