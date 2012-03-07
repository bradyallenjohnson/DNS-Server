include ../makefile.include

LIBRARY_NAME=dnsUtils
LIBRARY=$(LIBRARY_PREFIX)$(LIBRARY_NAME)$(LIBRARY_SUFFIX)
OBJS=ConfigFileParser.o ConfigOptions.o Logger.o

#
# Targets
#

all: $(OBJS) $(LIBRARY)

$(LIBRARY): $(OBJS)
	$(AR) r $(LIB_DIR)/$(LIBRARY) $(OBJS)
	$(COPY_CMD) *.h $(INCLUDE_DIR)/dnsUtils

ConfigFileParser.o: ConfigFileParser.cpp ConfigFileParser.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATH) -c ConfigFileParser.cpp -o ConfigFileParser$(OBJ_EXTENSION)

ConfigOptions.o: ConfigOptions.cpp ConfigOptions.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATH) -c ConfigOptions.cpp -o ConfigOptions$(OBJ_EXTENSION)

Logger.o: Logger.cpp Logger.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATH) -c Logger.cpp -o Logger$(OBJ_EXTENSION)

clean:
	$(DELETE_CMD) $(OBJS) $(LIB_DIR)/$(LIBRARY) $(INCLUDE_DIR)/dnsUtils/*.h
