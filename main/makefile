include ../makefile.include

EXE_NAME=dnsServer
OBJS=main.o

#
# Targets
#

all: $(EXE_NAME) $(OBJS)

$(EXE_NAME): $(OBJS)
	$(CC) $(CCFLAGS) $(LIBRARY_PATH) -o $(BIN_DIR)/$(EXE_NAME) main$(OBJ_EXTENSION) -ldnsLoader -ldnsResolver -lsocketServer -ldnsUtils

main.o: main.cpp
	$(CC) $(CCFLAGS) $(INCLUDE_PATH) -c main.cpp -o main$(OBJ_EXTENSION)

clean:
	$(DELETE_CMD) $(OBJS) $(BIN_DIR)/$(EXE_NAME) main$(OBJ_EXTENSION)
