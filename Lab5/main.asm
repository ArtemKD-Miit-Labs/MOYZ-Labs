section .data
    msg1 db "Enter first string: ", 0
    msg1_len equ $ - msg1

    msg2 db "Enter second string: ", 0
    msg2_len equ $ - msg2

    newline db 10   ; Символ новой строки
    newline_len equ $ - newline

section .bss
    buffer1 resb 100
    buffer2 resb 100
    concat_buffer resb 200  ; Буфер для объединённой строки

section .text
    global _start

_start:
    ; Выводим приглашение к вводу первой строки
    mov eax, 4          ; sys_write
    mov ebx, 1          ; stdout
    mov ecx, msg1
    mov edx, msg1_len
    int 0x80

    ; Читаем первую строку
    mov eax, 3          ; sys_read
    mov ebx, 0          ; stdin
    mov ecx, buffer1
    mov edx, 100        ; Максимальная длина
    int 0x80
    mov esi, eax        ; Сохраняем длину первой строки в esi

    ; Убираем символ новой строки, заменяя его на 0
    dec esi
    mov byte [buffer1 + esi], 0

    ; Выводим приглашение ко второй строке
    mov eax, 4
    mov ebx, 1
    mov ecx, msg2
    mov edx, msg2_len
    int 0x80

    ; Читаем вторую строку
    mov eax, 3
    mov ebx, 0
    mov ecx, buffer2
    mov edx, 100
    int 0x80
    mov edi, eax        ; Сохраняем длину второй строки в edi

    ; Убираем символ новой строки, заменяя его на 0
    dec edi
    mov byte [buffer2 + edi], 0

    ; Копируем первую строку в concat_buffer
    mov esi, buffer1
    mov edi, concat_buffer

copy_first:
    lodsb
    test al, al
    jz copy_second
    stosb
    jmp copy_first

copy_second:
    mov esi, buffer2

copy_loop:
    lodsb
    test al, al
    jz done_concat
    stosb
    jmp copy_loop

done_concat:
    ; Добавляем символ новой строки
    mov al, 10
    stosb

    ; Вывод объединённой строки
    mov eax, 4
    mov ebx, 1
    mov ecx, concat_buffer
    mov edx, 200
    int 0x80

    ; Завершаем программу
    mov eax, 1          ; sys_exit
    xor ebx, ebx
    int 0x80