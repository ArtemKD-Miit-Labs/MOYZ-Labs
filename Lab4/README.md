# Ëàáîðàòîðíàÿ ðàáîòà ¹4. Ëîãè÷åñêèå êîìàíäû è êîìàíäû ìàíèïóëèðîâàíèÿ áèòàìè (Âàðèàíò ¹25)

**Öåëü ðàáîòû:** èçó÷åíèå ëîãè÷åñêèõ êîìàíä è êîìàíä ìàíèïóëèðîâàíèÿ áèòàìè, ïîëó÷åíèå íàâûêà ðàáîòû ñ íèìè.

## **Ïîñòàíîâêà çàäà÷è**
1.	Çàíåñòè â ïàìÿòü 32 ðàçðÿäíîå øåñòíàäöàòåðè÷íîå ÷èñëî â ñîîòâåòñòâèè ñ âàðèàíòîì èç òàáëèöû 6;
2.	Ïîäñ÷èòàòü êîëè÷åñòâî íóëåé è åäèíèö â äàííîì ÷èñëå äâóìÿ ðàçíûìè ñïîñîáàìè;
3.	Ïîäñ÷èòàòü êîëè÷åñòâî ïàðíûõ íóëåé è ïàðíûõ åäèíèö â äàííîì ÷èñëå;
4.	Â ìëàäøåì áàéòå ÷èñëà îáìåíÿòü ìåæäó ñîáîé áèòû 0-7, 1-6, 2-5, 3-4.

Ðåçóëüòàòû êàæäîãî ïóíêòà ñîõðàíèòü â ïàìÿòè è ðàñïå÷àòàòü.

|Íîìåð âàðèàíòà|×èñëî(HEX)|
|:-:|:-:|
|25|**58697EFF**|

## **Êîä ïðîãðàììû**
```c
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

		counter_with_ROL_proc:
			PUSH ECX
			MOV EAX, DWord Ptr [ESI]
			MOV ECX, 32
			xor EBX, EBX
			xor EDX, EDX
			
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
			POP ECX
			MOV DWord Ptr [EDI + ECX * 4], EBX
			INC ECX
			MOV DWord Ptr [EDI + ECX * 4], EDX
			INC ECX
			RET

		// 1-2
		counter_with_TEST_proc:
			PUSH ECX
			MOV EAX, DWord Ptr [ESI]
			MOV ECX, 32
			xor EBX, EBX
			xor EDX, EDX
			
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
			POP ECX
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
			MOV EBX, 0x3

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
			POP ECX
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
				CALL get_reverse_bits_by_mask_proc
				CALL clear_mask_bits_proc
				or AL, BL
				SHR DH, 1
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
				or BL, BH
				RET

			shift_mask_bits_value:
				PUSH CX
				SHL CL, 1
				DEC CL
				SHR BH, CL
				SHL BL, CL
				POP CX
				RET

			clear_mask_bits_proc:
				MOV BH, DH
				or BH, DL
				not BH
				and AL, BH
				RET
		
		exit_mark:
		POPAD
	}

	int i = 0;
	bitset<32> bin_num(num);
	cout << "Ïîñ÷åò êîë-âà åäèíèö è íóëåé ¹1 (êîìàíäà ñäâèãà ROL)" << endl;
	cout << "×èñëî: \t" << num << " \thex: " << IntToHex(num) << " \tbin: " << bin_num << endl;
	cout << "Åäèíèö: " << data[i++] << endl;
	cout << "Íóëåé: " << data[i++] << "\n" <<  endl;

	cout << "Ïîñ÷åò êîë-âà åäèíèö è íóëåé ¹2 (êîìàíäà TEST)" << endl;
	cout << "×èñëî: \t" << num << " \thex: " << IntToHex(num) << " \tbin: " << bin_num << endl;
	cout << "Åäèíèö: " << data[i++] << endl;
	cout << "Íóëåé: " << data[i++] << "\n" << endl;

	cout << "Ïîñ÷åò êîë-âà ïàð åäèíèö è íóëåé" << endl;
	cout << "×èñëî: \t" << num << " \thex: " << IntToHex(num) << " \tbin: " << bin_num << endl;
	cout << "Ïàð åäèíèö: " << data[i++] << endl;
	cout << "Ïàð íóëåé: " << data[i++] << "\n" << endl;

	cout << "Îòîáðàæåíèå ïîñëåäíåãî áàéòà ÷èñëà" << endl;
	bitset<32> source_bin_num(data[i]);
	cout << "Èñõîäíîå ÷èñëî: \t" << data[i] << " \thex: " << IntToHex(data[i]) << " \tbin: " << source_bin_num << endl;
	i++;
	bitset<32> target_bin_num(data[i]);
	cout << "Ïîëó÷åííîå ÷èñëî: \t" << data[i] << " \thex: " << IntToHex(data[i]) << " \tbin: " << target_bin_num << endl;
}
```

## **Âûâîä**
1.	Â õîäå âûïîëíåíèÿ ëàáîðàòîðíîé ðàáîòû áûëè èçó÷åíû ëîãè÷åñêèå êîìàíäû è êîìàíäû ìàíèïóëèðîâàíèÿ áèòàìè.
2.	Áûëè èçó÷åíû êîìàíäû áèòîâûõ ñäâèãîâ òàêèå, êàê SHR, ROL è äðóãèå.
3.	Òàêæå áûëà èçó÷åíà ãðóïïà êîìàíä LOOP, êîòîðûå ïîçâîëÿþò ñîçäàâàòü öèêëû ñî ñ÷åò÷èêîì.
4.	Áûë ïðîäåìîíñòðèðîâàí ïîäõîä â îïåðèðîâàíèè áèòàìè ÷èñëà ñ ïîìîùüþ ìàñîê â ñâÿçêå ñ ëîãè÷åñêèìè êîìàíäàìè.

## **Çàêëþ÷åíèå**
Âûïîëíåíèå ëàáîðàòîðíîé ðàáîòû ðàñøèðèëî çíàíèå êîìàíä ÿçûêà àññåìáëåðà. Ðàçíûìè ïîäõîäàìè íàó÷èëèñü ðàáîòå ñ áèòàìè ÷èñëà. Ïîëó÷åííûå íàâûêè áóäóò íåîáõîäèìû ïðè òî÷å÷íîé ðàáîòå ñ áèòàìè ÷èñëà, áèòàìè ïðàâ è íàñòðîåê, à òàêæå ìîæåò óñêîðèòü óìíîæåíèå ÷èñåë íà 2 ïóòåì çàìåíû îáû÷íîé îïåðàöèè óìíîæåíèÿ íà áèòîâûé ñäâèã ÷èñëà.
