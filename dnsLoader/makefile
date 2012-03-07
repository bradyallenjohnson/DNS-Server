include ../makefile.include
  
LIBRARY_NAME=dnsLoader
LIBRARY=$(LIBRARY_PREFIX)$(LIBRARY_NAME)$(LIBRARY_SUFFIX)
OBJS=DnsLoader.o DnsLoaderFormat.o DnsLoaderMedium.o DnsLoaderFormatTextImpl.o DnsLoaderMediumFileImpl.o

#
# Targets
#

all: $(OBJS) $(LIBRARY)

$(LIBRARY): $(OBJS)
	$(AR) r $(LIB_DIR)/$(LIBRARY) $(OBJS)
	$(COPY_CMD) *.h $(INCLUDE_DIR)/dnsLoader

DnsLoader.o: DnsLoader.cpp DnsLoader.h DnsLoaderMedium.h DnsLoaderFormat.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATH) -c DnsLoader.cpp -o DnsLoader$(OBJ_EXTENSION)

DnsLoaderFormat.o: DnsLoaderFormat.cpp DnsLoaderFormat.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATH) -c DnsLoaderFormat.cpp -o DnsLoaderFormat$(OBJ_EXTENSION)

DnsLoaderMedium.o: DnsLoaderMedium.cpp DnsLoaderMedium.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATH) -c DnsLoaderMedium.cpp -o DnsLoaderMedium$(OBJ_EXTENSION)

DnsLoaderFormatTextImpl.o: DnsLoaderFormatTextImpl.cpp DnsLoaderFormatTextImpl.h DnsLoaderFormat.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATH) -c DnsLoaderFormatTextImpl.cpp -o DnsLoaderFormatTextImpl$(OBJ_EXTENSION)

DnsLoaderMediumFileImpl.o: DnsLoaderMediumFileImpl.cpp DnsLoaderMediumFileImpl.h DnsLoaderMedium.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATH) -c DnsLoaderMediumFileImpl.cpp -o DnsLoaderMediumFileImpl$(OBJ_EXTENSION)

clean:
	$(DELETE_CMD) $(OBJS) $(LIB_DIR)/$(LIBRARY) $(INCLUDE_DIR)/dnsLoader/*.h
