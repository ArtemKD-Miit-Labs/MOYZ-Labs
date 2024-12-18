# ������������ ������ �3. ������� �������� ���������� (������� �25)

**���� ������:** �������� ������ �������� ���������� � ��������� ������ ������ � ����.

## **���������� ������**
1.	� ������� ��������� Random ������������ ��������� ����� ����� � ��������� �� 0 �� 65535;
2.	������ ����� (������ � ����� ������ ����������) �������� � �� (������ ������), ������� � �������� �������� ������� data. �������� ����� (�������� � ����� ������ ����������) �������� � ��, ������� � 128 �������� ������� data. ��� �����, ������� ������ ��� ����� 50000 �������� � ��, ������� � 256 �������� ������� data. ��� �����, ������� ������ 10000 �������� � ��, ������� � 384 �������� ������� data;
3.	��������� �.1-�.2, ���� ����� �������������� ������ �� ������ �� ��������� �� �������� 127.

## **��� ���������**
```c
#include<iostream>
#include<ctime>

using namespace std;
const int DataSize = 512;

int main()
{
    srand(time(NULL));

    unsigned short data[DataSize] = { NULL };

    int32_t even_counter = 0;
    int32_t odd_counter = 0;
    int32_t more_50000_counter = 0;
    int32_t less_10000_counter = 0;

    _asm {
        PUSHAD

        main_loop :
            CALL rand
            MOV ESI, EAX
            MOV EBX, 2 
            xor EDX, EDX
            DIV EBX
            CMP EDX, 0
            JE even_num_proc
            JNE odd_num_proc
            JMP main_loop_exit

        even_num_proc :
            MOV ECX, 0
            LEA EBX, even_counter
            MOV EDX, DWord Ptr[EBX]
            CALL add_num_in_data
            CALL inc_counter_proc
            JMP more_or_less_proc

        odd_num_proc :
            MOV ECX, 128
            LEA EBX, odd_counter
            MOV EDX, DWord Ptr[EBX]
            CALL add_num_in_data
            CALL inc_counter_proc
            JMP more_or_less_proc

        more_or_less_proc :
            CMP EAX, 50000
            JAE more_50000_proc
            CMP EAX, 10000
            JBE less_10000_proc
            JMP couters_reach_127_proc

        more_50000_proc :
            MOV ECX, 256
            LEA EBX, more_50000_counter
            MOV EDX, DWord Ptr[EBX]
            CALL add_num_in_data
            CALL inc_counter_proc
            JMP couters_reach_127_proc


        less_10000_proc :
            MOV ECX, 384
            LEA EBX, less_10000_counter
            MOV EDX, DWord Ptr[EBX]
            CALL add_num_in_data
            CALL inc_counter_proc
            JMP couters_reach_127_proc

        add_num_in_data
            ADD ECX, EDX
            LEA EDI, data
            MOV Word Ptr[EDI + ECX * 2], SI
            RET

        inc_counter_proc :
            INC EDX
            MOV DWord Ptr[EBX], EDX
            RET

        couters_reach_127_proc :
            LEA EBX, even_counter
            CALL current_counter_reach_127_proc
            JAE main_loop_exit

            LEA EBX, odd_counter
            CALL current_counter_reach_127_proc
            JAE main_loop_exit

            LEA EBX, more_50000_counter
            CALL current_counter_reach_127_proc
            JAE main_loop_exit

            LEA EBX, less_10000_counter
            CALL current_counter_reach_127_proc
            JAE main_loop_exit

            JMP main_loop


        current_counter_reach_127_proc :
            MOV EDX, DWord Ptr[EBX]
            CMP EDX, 127
            RET


        main_loop_exit :

        POPAD
    }

    int i = 0, j = i + even_counter;
    cout << "Numbers [0 : 127] (count:" << even_counter << "): ";
    for (i; i < j; i++) {
        cout << data[i] << " ";
    }
    cout << "\n" << endl;

    i = 128, j = i + odd_counter;
    cout << "Numbers [128 : 255] (count:" << odd_counter << "): ";
    for (i; i < j; i++) {
        cout << data[i] << " ";
    }
    cout << "\n" << endl;


    i = 256, j = i + more_50000_counter;
    cout << "Numbers [256 : 383] (count:" << more_50000_counter << "): ";
    for (i; i < j; i++) {
        cout << data[i] << " ";
    }
    cout << "\n" << endl;

    i = 384, j = i + less_10000_counter;
    cout << "Numbers [384 : 511] (count:" << less_10000_counter << "): ";
    for (i; i < j; i++) {
        cout << data[i] << " ";
    }
    cout << "\n" << endl;

    return 0;
}
```

![��������� ������](�����.jpg)

## **�����**
1.	� ���� ���������� ������������ ������ ���� ������� ������� �������� ���������� ����� ����������, ����� ��� JMP, JCC, CALL, RET.
2.	���� ������� ������� CMP, CMPXCHG � CMPXCHG8B ��� ��������� ������ ����� �����.
3.	����� ���� ������� ������ ������ LOOP, ������� ��������� ��������� ����� �� ���������.
4.	���� ������������������� ��� � ������� ������ �������� ���������� ����� ����������� �����, � � ������� ������ ������ CMP � JCC ����������� ����������, ������� �� �������� ��������� � ������ ����� �������� ������.
5.	��������, ��� ��� ������ �������� CALL ��������� ����������� ������������ � ����� ������� RET, ��� ��� ��� �� ���������� � �������� EPI ����� ��������� ���������� ������.

## **����������**
���������� ������������ ������ ��������� ������ ������ ����� ����������. �� ����������� ����������� �����, ��� ����� � �������� ��������, ����������� ��� ���������� ����� ������� ��������. ����� ��������� �������� ������������� ����� ���� � ��������� ��������� � ���������������� ��. ���������� ������ ����� ���������� ��� ���������� �������� �� ������� ������� � ����������.