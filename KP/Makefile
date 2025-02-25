BUILD_DIR = Build

all: ${BUILD_DIR} main main-gdb

${BUILD_DIR}:
	mkdir ${BUILD_DIR}

main: main.o
	ld -m elf_i386 -o ${BUILD_DIR}/main ${BUILD_DIR}/main.o

main.o: main.asm
	nasm -f elf32 -o ${BUILD_DIR}/main.o main.asm

main-gdb: main-gdb.o
	ld -m elf_i386 -o ${BUILD_DIR}/main-gdb ${BUILD_DIR}/main-gdb.o

main-gdb.o: main.asm
	nasm -f elf32 -g -F dwarf -o ${BUILD_DIR}/main-gdb.o main.asm

clean:
	rm -rf ${BUILD_DIR}

	