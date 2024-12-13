#include<iostream>
#include<conio.h>
#include<iomanip>
#include<sstream>
#include<ctime>

using namespace std;
const int DataSize = 512;

int main()
{
    srand(time(NULL));

    unsigned short data[DataSize] = { NULL };

    // Инициализация необходимых счетчиков элементов по условиям
    int32_t even_counter = 0;
    int32_t odd_counter = 0;
    int32_t more_50000_counter = 0;
    int32_t less_10000_counter = 0;

    _asm {
        PUSHAD

        main_loop:
            CALL rand // получаем случайное число в EAX
            MOV ESI, EAX // переносим число в ESI для сохранения между операциями
            MOV EBX, 2 // записываем делитель в регистр EBX 
            xor EDX, EDX // очизаем регистр страшрей части делимого
            DIV EBX // проверка числа на четность
            CMP EDX, 0 // сравнение остатка от деления с нулем
            JE even_num_proc // переход к процедуре обработки ЧЕТНОГО числа по метке "even_num_proc"
            JNE odd_num_proc // переход к процедуре обработки НЕЧЕТНОГО числа по метке "odd_num_proc"
            JMP main_loop_exit // если ни одно условие не срабатывает выходим из цикла

        even_num_proc:
            MOV ECX, 0 // устанавливаем начальный индекс группы условия
            LEA EBX, even_counter // получаем адресс счетчика группы
            MOV EDX, DWord Ptr [EBX] // перемещаем счетчик группы в регистр EDX
            CALL add_num_in_data // записываем число в массив
            CALL inc_counter_proc // инкрементируем счетчик группы
            JMP more_or_less_proc // переходим к проверке следующей группы условий

        odd_num_proc:
            MOV ECX, 128 // устанавливаем начальный индекс группы условия
            LEA EBX, odd_counter // получаем адресс счетчика группы
            MOV EDX, DWord Ptr [EBX] // перемещаем счетчик группы в регистр EDX
            CALL add_num_in_data // записываем число в массив
            CALL inc_counter_proc // инкрементируем счетчик группы
            JMP more_or_less_proc // переходим к проверке следующей группы условий

        more_or_less_proc:
            CMP EAX, 11000 // проверка условия => 50000
            JAE more_50000_proc 
            CMP EAX, 10000 // проверка условия <= 10000
            JBE less_10000_proc
            JMP couters_reach_127_proc // проверка глваного условия выполнения цикла

        more_50000_proc:
            MOV ECX, 256 // устанавливаем начальный индекс группы условия
            LEA EBX, more_50000_counter // получаем адресс счетчика группы
            MOV EDX, DWord Ptr [EBX] // перемещаем счетчик группы в регистр EDX
            CALL add_num_in_data // записываем число в массив
            CALL inc_counter_proc // инкрементируем счетчик группы
            JMP couters_reach_127_proc // проверка глваного условия выполнения цикла

        less_10000_proc:
            MOV ECX, 384 // устанавливаем начальный индекс группы условия
            LEA EBX, less_10000_counter // получаем адресс счетчика группы
            MOV EDX, DWord Ptr [EBX] // перемещаем счетчик группы в регистр EDX
            CALL add_num_in_data // записываем число в массив
            CALL inc_counter_proc // инкрементируем счетчик группы
            JMP couters_reach_127_proc // проверка глваного условия выполнения цикла

        add_num_in_data: // в ECX хранится начальный индекс самой группы, в EDX хранится индекс элемента текущей группы, в ESI хранится полученое случайное число, 
            ADD ECX, EDX // получаем индекс для записи нового элемента, как сумму стартового индекса группы (ECX) + индекс элемента группы (EDX)
            LEA EDI, data // получаем адрес нулевого элемента выделенного массива data
            MOV Word Ptr [EDI + ECX * 2], SI // вычисляем из полученных ранее индексов место записи числа в массиве
            RET

        inc_counter_proc: // в EBX хранится адрес переменной счетчика, в EDX хранится индекс элемента текущей группы
            INC EDX // инкрементируем счетчик
            MOV DWord Ptr [EBX], EDX // заносим его обратно в память
            RET

        couters_reach_127_proc: // EBX - адрес переменных, EDX - значания этих перменных
            LEA EBX, even_counter // проверка счетчика четных чисел
            CALL current_counter_reach_127_proc
            JAE main_loop_exit

            LEA EBX, odd_counter // проверка счетчика нечетных чисел
            CALL current_counter_reach_127_proc
            JAE main_loop_exit

            LEA EBX, more_50000_counter // проверка счетчика чисел => 5000
            CALL current_counter_reach_127_proc
            JAE main_loop_exit
            
            LEA EBX, less_10000_counter // проверка счетчика чисел <= 10000
            CALL current_counter_reach_127_proc
            JAE main_loop_exit
           
            JMP main_loop


        current_counter_reach_127_proc:
            MOV EDX, DWord Ptr [EBX] // проверяем достиг ли счетчик по адресу в EBX 127
            CMP EDX, 127
            RET


        main_loop_exit:

        POPAD
    }

    cout << "Numbers [0 : 127] (count:" << even_counter <<"): ";
    for (int i = 0; i < even_counter; i++) {
        cout << data[i] << " ";
    }
    cout << "\n" << endl;

    cout << "Numbers [128 : 255] (count:" << odd_counter << "): ";
    for (int i = 128; i < odd_counter + 128; i++) {
        cout << data[i] << " ";
    }
    cout << "\n" << endl;

    cout << "Numbers [256 : 383] (count:" << more_50000_counter << "): ";
    for (int i = 256; i < more_50000_counter + 256; i++) {
        cout << data[i] << " ";
    }
    cout << "\n" << endl;

    cout << "Numbers [384 : 511] (count:" << less_10000_counter << "): ";
    for (int i = 384; i < less_10000_counter + 384; i++) {
        cout << data[i] << " ";
    }
    cout << "\n" << endl;

    return 0;
}
