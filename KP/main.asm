section .data
    A dw -3, 4, -5      ; массив A
    B dw 2, -6, 7       ; массив B
    C dw -8, 9, -10     ; массив C
    X dw 3              ; коэффициент X
    Y dw 2              ; коэффициент Y
    sum1 dd 0           ; сумма (|A[i]| * |B[i]|)
    sum2 dd 0           ; сумма |C[i]|
    result dd 0         ; итоговый результат
    overflow_msg db "Overflow detected!", 0xA, 0

section .bss
    temp resw 1

section .text
    global _start

_start:
    xor eax, eax
    xor edx, edx
    mov ecx, 3          ; количество элементов в массивах
    lea esi, [A]        ; указатель на массив A
    lea edi, [B]        ; указатель на массив B
    lea ebx, [C]        ; указатель на массив C

loop_calculation:
    ; Загружаем |A[i]|
    mov ax, [esi]
    test ax, ax
    jns no_abs_A
    neg ax
no_abs_A:

    ; Загружаем |B[i]|
    mov dx, [edi]
    test dx, dx
    jns no_abs_B
    neg dx
no_abs_B:

    ; Умножаем |A[i]| * |B[i]|
    imul dx
    jo overflow         ; Проверка переполнения

    ; Складываем сумму sum1 += |A[i]| * |B[i]|
    add [sum1], eax
    jo overflow         ; Проверка переполнения

    ; Обрабатываем |C[i]|
    mov ax, [ebx]
    test ax, ax
    jns no_abs_C
    neg ax
no_abs_C:

    ; Складываем сумму sum2 += |C[i]|
    add [sum2], eax
    jo overflow         ; Проверка переполнения

    ; Переход к следующему элементу
    add esi, 2
    add edi, 2
    add ebx, 2
    loop loop_calculation

    ; Умножаем sum1 на X
    mov eax, [sum1]
    imul word [X]
    jo overflow
    mov [sum1], eax

    ; Умножаем sum2 на Y
    mov eax, [sum2]
    imul word [Y]
    jo overflow
    mov [sum2], eax

    ; Вычисляем итоговый результат result = sum1 - sum2
    mov eax, [sum1]
    sub eax, [sum2]
    jo overflow
    mov [result], eax

    ; Завершаем программу
    mov eax, 1
    mov ebx, [result]
    int 0x80

overflow:
    ; Обработка переполнения
    mov eax, 4
    mov ebx, 1
    mov ecx, overflow_msg
    mov edx, 20
    int 0x80

    ; Завершаем программу с кодом ошибки
    mov eax, 1
    mov ebx, 1
    int 0x80
