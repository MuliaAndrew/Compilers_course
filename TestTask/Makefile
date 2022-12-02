.Phony: all

FLAGS = -lelf

all: MAIN CLEAN

./PhysMem/physMem.o:
			g++ -c Physmem/physmem.cpp -o physmem.o

./Inst/instruction.o:
			g++ -c Instructions/instructions.cpp -o instruction.o

./Hardware/hardware.o:
			g++ -c Executor/executor.cpp -o executor.o
		
./ElfParser/elf_parser.o:
			g++ -c ElfParser/elf_parser.cpp -o elf_parser.o $(FLAGS)

./ErrorHandler/error.o:
			g++ -c ErrorHandler/error.cpp -o error.o

./Cache/cache.o:
			g++ -c Cache/cache.cpp -o cache.o

main.o:	main.cpp
			g++ -c main.cpp -o main.o

MAIN: ./PhysMem/physMem.o ./Hardware/hardware.o ./Inst/instruction.o ./ElfParser/elf_parser.o ./ErrorHandler/error.o ./Cache/cache.o main.o 
			g++ main.o physmem.o cache.o error.o executor.o instruction.o elf_parser.o -o out $(FLAGS)

CLEAN:
			rm -rf physmem.o
			rm -rf executor.o
			rm -rf main.o
			rm -rf instruction.o
			rm -rf elf_parser.o
			rm -rf error.o
			rm -rf cache.o
			