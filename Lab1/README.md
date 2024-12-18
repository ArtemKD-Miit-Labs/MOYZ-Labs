# ������������ ������ �1. ��������, ������ � ������� ��������� ������ (������� �25)

**���� ������:** �������� ��������� ������ ���������� (���) � ������ ��������� ������.

## **���������� ������**
1.	������� ����� �� ������� ������ 1� ������� 1 � ������, ��������������� ��������� ��������, � ����� ���;
2.	������� ����� �� ������� ������ 2� ������� 1 � ������, ��������������� ��������� ��������, � ��������� ���;
3.	������� ����� �� ������� ������ 3� ������� 1 � ������, ��������������� ��������� ��������, � ��������� ���;
4.	������� ����� �� ������� ������ 4� ������� 1 � ������, ��������������� ��������� ��������, � ��������� ���;
5.	������� ����� �� ������� ������ 5� ������� 1 � ������, ��������������� ��������� ��������, � ��������� ���;
6.	������� ����� �� ������� ������ 6� ������� 1 � ������, ��������������� ��������� ��������, � ��������� ���;
7.	�������� �����, ���������� � ��� ����� ���������� ������� 1 � 2, ����� �����, 4-�� ������� ���������, �� ������� ���������� � ������� ���;
8.	�������� �����, ���������� � ��� ����� ���������� ������� 3 � 4, ����� �����, 4-�� ������� ���������, �� ������� ���������� � ������� ���;
9.	�������� �����, ���������� � ��� ����� ���������� ������� 5 � 6, ����� �����, 4-�� ������� ���������, �� ������� ���������� � ������� ���;
10.	 ��������� �����, ����������� � ��� ����� ���������� ������ � �������, ������ �� ������� ������ 1� ������� 1 � ������, ��������������� ��������� ��������, � ����� 16-��������� ��� ��� ������ �����;
11.	 ��������� �����, ����������� � ��� ����� ���������� ������ � �������, ������ �� ������� ������ 2� ������� 1 � ������, ��������������� ��������� ��������, � ����� 32-��������� ��� ����������� �����;

**<u>����������: ������� ESP � EBP ������������ ������ !!!</u>**

|����� ��������|����� 1|����� 2|����� 3|����� 4|����� 5|����� 6|����� 1|����� 2|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|25|2516094474|1472212419|51469|16564|145|86|8|8|

## **��� ���������**
```c
#include <iomanip>

int main()
{
	int32_t temp_data;
	int32_t num1 = 2516094474; // 0x95F88E0A
	int32_t num2 = 1472212419; // 0x57C02DC3
	int16_t num3 = 51469; // 0xC90D
	int16_t num4 = 16564; // 0x40B4
	int8_t num5 = 145; // 0x91
	int8_t num6 = 86; // 0x56

	__asm {
		PUSHAD

		//1-6
		MOV EAX, num1
		MOV EBX, num2
		MOVZX SI, num3
		MOVZX DI, num4
		MOVZX CL, num5
		MOV CH, num6

		// EDX free

		//7.1
		MOV EDX, EAX
		MOV EAX, EBX
		MOV EBX, EDX
		//7.2
		PUSH EAX
		PUSH EBX
		POP EAX
		POP EBX
		//7.3
		XCHG EAX, EBX
		//7.4
		LEA EDX, temp_data
		MOV [EDX], EAX
		MOV EAX, EBX
		MOV EBX, [EDX]

		//8.1
		MOVZX DX, SI
		MOV SI, DI
		MOV DI, DX
		//8.2
		PUSH SI
		PUSH DI
		POP SI
		POP DI
		//8.3
		XCHG SI, DI
		//8.4
		LEA EDX, temp_data
		MOV [EDX], SI
		MOV SI, DI
		MOV DI, [EDX]

		//9.1
		MOV DL, CH
		MOV CH, CL
		MOV CL, DL
		//9.2 *
		BSWAP ECX
		PUSH ECX
		POP CX
		POP CX
		//9.3
		XCHG CH, CL
		//9.4
		LEA EDX, temp_data
		MOV [EDX], CH
		MOV CH, CL
		MOV CL, [EDX]

		//10
		PUSHAD
		MOVSX AX, SI
		MOVSX BX, DI
		POPAD

		//11
		PUSHAD
		MOVZX EAX, SI
		MOVZX EBX, DI
		POPAD

		POPAD
	}
}

```

## **�����**
1.	� ���� ���������� ������������ ������ ���� ������� ������� � ����������� ������������� ��������� ������ ���������� (���).
2.	���� �������������������, ��� � ������� ������ MOV, PUSH, POP, XCHG, BSWAP � LEA ����� ���������� �������������� ����� ������� ����� ����������, ������ � �������.
3.	����� ��������, ��� ��� ������ � ���������� ����� ��������� �� ����������� (8-bit, 16-bit, 32-bit) � ��� ��� ������� ������� �� ������� � ������� ����� (��������, EAX, AX, AH, AL).
4.	���� ������� ������� ��������� (MOVSX) �  ������������ (MOVZX) ����������, ������� ��������� ��������� ���������� ������ ����� ���������� ������ �����������.
5.	���� ������� ������ �������� ��� ������ � ��������� PUSH � POP, ��� ��� ������ ��� �� ������������� ����� �������� � ����� � ������ ��� ���������.

## **����������**
� ���������� ������ ���� ������� ������� ������� ��������� ������ � ������ � ������� ����� ����������.
���������� ������ ���� ������������� �� ����������� �������������� ���������� � ������� � ����������, ��� �������������� ������ ������ �����������. ��� ��� ����� ��� ��������� ��������� ������ �������������� ������ �� ������ ������.
������ ������������ ������ �������� ��������� ��� ����������� �������� ����� ����������, ��� ������� ������� � ����������� ������ ������� ��� ���������� ����� ������� ���������� � ��������.