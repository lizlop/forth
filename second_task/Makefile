ASM			= nasm
ASMFLAGS	= -felf64 -g

LINKER 		= ld
LINKERFLAGS = 
LIBS        = 

all: bin/forth
	
bin/forth: obj/forth.o
	mkdir -p bin 
	$(LINKER) -o bin/forth $(LINKERFLAGS) -o bin/forth obj/forth.o $(LIBS)

obj/forth.o: forth.asm macro.inc basic_words.inc initial_process.inc
	mkdir -p obj
	$(ASM) $(ASMFLAGS) forth.asm -o obj/forth.o

clean: 
	rm -rf build obj

.PHONY: clean

