#include<iostream>
#include<conio.h>
#include<iomanip>
#include<sstream>

using namespace std;
const int DataSize = 144;

string IntToHex(int n)
{
	stringstream ss;
	ss << hex << n;
	return ss.str();
}

// 1)
//	b + c*b - (a/4) = num1
//	  3  1  4   2

// 2)
//	a*b - 1 = num2
//	 1  2

// 3)
// num1/num2 = num3

int main()
{
	int32_t a = 5678901; // 0x56A735
	int32_t b = 6789012; // 0x679794
	int32_t c = 7890123; // 0x7864CB
	int32_t d = 8901234; // 0x87D272
	int32_t e = 9012345; // 0x898479
	int32_t f = 10123456; // 0x9A78C0

	unsigned char Memo[DataSize];
	for (int i = 0; i++; i < DataSize) {
		Memo[i] = NULL;
	}

	__asm {
		PUSHAD

		LEA EDI, Memo // Храним в регистре EDI адресс первого байта Memo

		// 1)-1
		MOV EAX, c
		MUL b
		MOV DWord Ptr [EDI], EAX // заносим в Memo младшую часть произведения
		MOV DWord Ptr [EDI + 4], EDX // заносим в Memo старшую часть произведения

		// 1)-2
		MOV EAX, a
		xor EDX, EDX
		MOV EBX, 4
		DIV EBX
		MOV DWord Ptr [EDI + 8], EAX

		// 1)-3
		MOV EAX, b // младшая часть b
		MOV EDX, 0 // старшая чать b
		MOV ECX, DWord Ptr [EDI] // младшая часть пунтка 1)-1
		MOV EBX, DWord Ptr [EDI + 4] // старшая часть пункта 1)-1

		ADD EAX, ECX
		ADC EDX, EBX		

		// 1)-4
		MOV ECX, DWord Ptr [EDI + 8] // младшая часть пунтка 1)-2
		MOV EBX, 0 // старшая часть пунтка 1)-2

		SUB EAX, ECX
		SBB EDX, EBX

		MOV DWord Ptr [EDI], EAX
		MOV DWord Ptr [EDI + 4], EDX

		// 2)-1
		MOV EAX, a
		MUL b // младшая часть EAX, старшая EDX

		// 2)-2
		SUB EAX, 1
		SBB EDX, 0

		MOV EBX, EAX
		MOV ECX, EDX

		// 3)
		MOV EAX, DWord Ptr [EDI]
		MOV EDX, DWord Ptr [EDI + 4]

		DIV EBX
		MOV DWord Ptr [EDI], EAX

		POPAD
	}

	for (int i = 0; i < DataSize; i++) {
		if ((i % 16) == 0) {
			cout << "\n" << setw(2) << i / 16 << ":";
		}
		else {
			cout << " " << setw(2) << IntToHex(Memo[i - 1]);
		}
	}
	_getch();

	return 0;
}
