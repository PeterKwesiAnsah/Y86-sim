CC = gcc
OBJ = decodeStage.o executeStage.o fetchStage.o memoryStage.o writebackStage.o memory.o registers.o dump.o tools.o
yess: $(OBJ)
	$(CC) -o yess -std=c99 $(OBJ)
decodeStage.o: decodeStage.c decodeStage.h tools.h
executeStage.o: executeStage.c executeStage.h tools.h
fetchStage.o: fetchStage.c fetchStage.h tools.h
memoryStage.o: memoryStage.c memoryStage.h tools.h
writebackStage.o: writebackStage.c writebackStage.h tools.h
memory.o: memory.c memory.h tools.h
	$(CC) -std=c99 memory.c memory.h tools.h
registers.o: registers.c registers.h tools.h
dump.o: dump.c dump.h tools.h
tools.o: tools.c tools.h memory.h

clean:
	rm -rf ./*.o
