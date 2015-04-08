CC = gcc -g -std=c99
OBJ = main.o decodeStage.o executeStage.o fetchStage.o memoryStage.o writebackStage.o memory.o registers.o dump.o tools.o loader.o
yess: $(OBJ)
	$(CC) -o yess $(OBJ)
main.o: main.c tools.h memory.h dump.h
decodeStage.o: decodeStage.c decodeStage.h tools.h forwarding.h
executeStage.o: executeStage.c executeStage.h tools.h memoryStage.h writebackStage.h
fetchStage.o: fetchStage.c fetchStage.h tools.h memory.h instructions.h registers.h memoryStage.h
memoryStage.o: memoryStage.c memoryStage.h tools.h writebackStage.h
writebackStage.o: writebackStage.c writebackStage.h tools.h instructions.h
memory.o: memory.c memory.h tools.h
registers.o: registers.c registers.h tools.h
dump.o: dump.c dump.h tools.h fetchStage.h decodeStage.h executeStage.h memoryStage.h writebackStage.h registers.h memory.h
loader.o: loader.c loader.h tools.h memory.h


clean:
	rm -rf ./*.o ./yess
