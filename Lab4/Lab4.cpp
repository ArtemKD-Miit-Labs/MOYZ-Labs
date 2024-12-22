#include <iostream>
#include <sstream>
#include <bitset>

using namespace std;
const int DataSize = 8;

string IntToHex(int32_t n)
{
	stringstream ss;
	ss << hex << n;
	return ss.str();
}


int main()
{
	setlocale(LC_ALL, "ru");
	int32_t data[DataSize] = { NULL };

	int32_t num = 0x58697EFF;
	int8_t unit_counter = 0;
	int8_t zero_counter = 0;

	__asm {
		PUSHAD
		LEA ESI, num
		LEA EDI, data
		MOV ECX, 0
		CALL counter_with_ROL_proc
		CALL counter_with_TEST_proc
		CALL pair_counter_proc
		CALL reverse_lower_byte_proc

		JMP exit_mark

		// 1-1
		counter_with_ROL_proc:
			PUSH ECX // сохраняем глобальный счетчик для сохранения в память в стеке
			MOV EAX, DWord Ptr [ESI]
			MOV ECX, 32
			xor EBX, EBX // счетчик единиц
			xor EDX, EDX // счетчик нулей
			
			counter_with_ROL_proc_loop:
				ROL EAX, 1
				JC ROL_unit_bit_condition
				INC EDX
				JMP end_ROL_loop

				ROL_unit_bit_condition:
				INC EBX
				JMP end_ROL_loop

			end_ROL_loop:
			LOOP counter_with_ROL_proc_loop
			POP ECX // Получаем счетчик из стека и записываем с его помощью результат в память
			MOV DWord Ptr [EDI + ECX * 4], EBX
			INC ECX
			MOV DWord Ptr [EDI + ECX * 4], EDX
			INC ECX
			RET

		// 1-2
		counter_with_TEST_proc:
			PUSH ECX // сохраняем глобальный счетчик для сохранения в память в стеке
			MOV EAX, DWord Ptr [ESI]
			MOV ECX, 32
			xor EBX, EBX // счетчик единиц
			xor EDX, EDX // счетчик нулей
			
			counter_with_TEST_proc_loop:
				TEST EAX, 1
				JZ TEST_unit_bit_condition
				INC EBX
				JMP end_TEST_loop

				TEST_unit_bit_condition:
				INC EDX
				JMP end_TEST_loop

			end_TEST_loop:
			SHR EAX, 1
			LOOP counter_with_TEST_proc_loop
			POP ECX // Получаем счетчик из стека и записываем с его помощью результат в память
			MOV DWord Ptr [EDI + ECX * 4], EBX
			INC ECX
			MOV DWord Ptr [EDI + ECX * 4], EDX
			INC ECX
			RET

		// 2
		pair_counter_proc:
			PUSH ECX
			MOV EAX, DWord Ptr [ESI]
			MOV ECX, 31
			MOV EBX, 0x3 // 0011

			pair_counter_loop:
				PUSH EAX
				and EAX, EBX
				CMP EAX, EBX
				JZ unit_pair_condition
				POP EAX
				PUSH EAX
				not EAX
				and EAX, EBX
				CMP EAX, EBX
				JZ zero_pair_condition
				JMP end_pair_counter_loop

				unit_pair_condition:
				MOV DL, Byte Ptr [unit_counter]
				INC DL
				MOV Byte Ptr [unit_counter], DL
				JMP end_pair_counter_loop

				zero_pair_condition:
				MOV DL, Byte Ptr [zero_counter]
				INC DL
				MOV Byte Ptr [zero_counter], DL
				JMP end_pair_counter_loop

			end_pair_counter_loop:
			POP EAX
			SHL EBX, 1
			LOOP pair_counter_loop
			POP ECX // Получаем счетчик из стека и записываем с его помощью результат в память
			MOVZX EDX, Byte Ptr [unit_counter]
			MOV DWord Ptr [EDI + ECX * 4], EDX
			INC ECX
			MOVZX EDX, Byte Ptr [zero_counter]
			MOV DWord Ptr [EDI + ECX * 4], EDX
			INC ECX
			RET

		// 3
		reverse_lower_byte_proc:
			PUSH ECX
			MOV AL, Byte Ptr [ESI]
			MOV DH, 0x80
			MOV DL, 0x01
			MOV CL, 4

			reverse_loop:
				CALL get_reverse_bits_by_mask_proc // получаем число с отображенными битами по текущей маске
				CALL clear_mask_bits_proc // зануляем в исходном числе биты по маске
				or AL, BL // проводим битовое сложение, чтобы положить отображенные биты в исходное число
				SHR DH, 1 // сдвигаем биты маски на одну позицию
				SHL DL, 1
			LOOP reverse_loop
			POP ECX
			MOV EDX, num
			MOV DWord Ptr [EDI + ECX * 4], EDX
			INC ECX
			MOV DL, AL
			MOV DWord Ptr [EDI + ECX * 4], EDX
			INC ECX
			RET

			get_reverse_bits_by_mask_proc:
				MOV BH, AL
				MOV BL, AL
				and BH, DH
				and BL, DL
				CALL shift_mask_bits_value
				or BL, BH // в регистре BL хранится отображенная маска текущей пары битов
				RET

			shift_mask_bits_value:
				PUSH CX // сохраняем регистр CX т.к. он используется в команде LOOP, а нам нудно использовать CL в команде SHR
				SHL CL, 1
				DEC CL
				SHR BH, CL
				SHL BL, CL
				POP CX
				RET

			clear_mask_bits_proc:
				MOV BH, DH // регистр BH можем использовать т.к. значение отраженных битов находится только в регистре BL
				or BH, DL
				not BH
				and AL, BH
				RET
		
		exit_mark:
		POPAD
	}

	int i = 0;
	bitset<32> bin_num(num);
	cout << "Посчет кол-ва единиц и нулей №1 (команда сдвига ROL)" << endl;
	cout << "Число: \t" << num << " \thex: " << IntToHex(num) << " \tbin: " << bin_num << endl;
	cout << "Единиц: " << data[i++] << endl;
	cout << "Нулей: " << data[i++] << "\n" <<  endl;

	cout << "Посчет кол-ва единиц и нулей №2 (команда TEST)" << endl;
	cout << "Число: \t" << num << " \thex: " << IntToHex(num) << " \tbin: " << bin_num << endl;
	cout << "Единиц: " << data[i++] << endl;
	cout << "Нулей: " << data[i++] << "\n" << endl;

	cout << "Посчет кол-ва пар единиц и нулей" << endl;
	cout << "Число: \t" << num << " \thex: " << IntToHex(num) << " \tbin: " << bin_num << endl;
	cout << "Пар единиц: " << data[i++] << endl;
	cout << "Пар нулей: " << data[i++] << "\n" << endl;

	cout << "Отображение последнего байта числа" << endl;
	bitset<32> source_bin_num(data[i]);
	cout << "Исходное число: \t" << data[i] << " \thex: " << IntToHex(data[i]) << " \tbin: " << source_bin_num << endl;
	i++;
	bitset<32> target_bin_num(data[i]);
	cout << "Полученное число: \t" << data[i] << " \thex: " << IntToHex(data[i]) << " \tbin: " << target_bin_num << endl;
}