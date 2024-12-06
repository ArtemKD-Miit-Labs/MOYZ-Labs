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

	int32_t num1;
	int32_t num2;
	int32_t num3;

	int32_t temp_num;

	unsigned char Memo[DataSize];
	for (int i = 0; i++; i < DataSize) {
		Memo[i] = NULL;
	}

	__asm {
		PUSHAD

		LEA EDI, Memo

		xor ECX, ECX

		// 1)-1
		MOV EAX, c
		MUL b
		MOV DWord Ptr [EDI + ECX * 4], EAX
		INC ECX
		MOV DWord Ptr [EDI + ECX * 4], EDX
		INC ECX

		// 1)-2
		MOV EAX, a
		xor EDX, EDX
		MOV EBX, 4
		DIV EBX
		MOV DWord Ptr [EDI + ECX * 4], EAX
		INC ECX
		MOV DWord Ptr [EDI + ECX * 4], EDX
		INC ECX

		// 1)-3
		// младшая часть b
		MOV EAX, b
		// старшая чать b
		MOV EDX, 0
		// младшая часть пунтка 1)-1
		MOV ESI, DWord Ptr [EDI]
		// старшая часть пункта 1)-1
		MOV EBX, DWord Ptr [EDI + 4]

		ADD EAX, ESI
		ADC EDX, EBX		

		// 1)-4
		MOV ESI, DWord Ptr [EDI + 8]
		MOV EBX, 0

		SUB EAX, ESI
		SBB EDX, EBX

		MOV DWord Ptr [EDI], EAX
		MOV DWord Ptr [EDI + 4], EDX

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
