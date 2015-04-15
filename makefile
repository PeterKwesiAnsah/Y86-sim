CC = gcc -g -std=c99
OBJ = main.o decodeStage.o executeStage.o fetchStage.o memoryStage.o writebackStage.o memory.o registers.o dump.o tools.o loader.o
yess: $(OBJ)
	$(CC) -o yess $(OBJ)
main.o: main.c tools.h memory.h dump.h
decodeStage.o: decodeStage.c forwarding.h decodeStage.h registers.h instructions.h
executeStage.o: executeStage.c forwarding.h executeStage.h memoryStage.h registers.h instructions.h
fetchStage.o: fetchStage.c forwarding.h fetchStage.h registers.h instructions.h
memoryStage.o: memoryStage.c forwarding.h memoryStage.h registers.h instructions.h
writebackStage.o: writebackStage.c forwarding.h writebackStage.h registers.h instructions.h
memory.o: memory.c memory.h tools.h
registers.o: registers.c registers.h tools.h
dump.o: dump.c dump.h tools.h fetchStage.h decodeStage.h executeStage.h memoryStage.h writebackStage.h registers.h memory.h
loader.o: loader.c loader.h tools.h memory.h


clean:
	rm -rf ./*.o ./yess
