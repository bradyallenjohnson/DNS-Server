include ../makefile.include

LIBRARY_NAME=dnsResolver
LIBRARY=$(LIBRARY_PREFIX)$(LIBRARY_NAME)$(LIBRARY_SUFFIX)
OBJS=DnsManager.o DnsMessage.o DnsRequest.o DnsResponse.o DnsResolver.o DnsResolverLocalImpl.o

#
# Targets
#

all: $(OBJS) $(LIBRARY)

$(LIBRARY): $(OBJS)
	$(AR) r $(LIB_DIR)/$(LIBRARY) $(OBJS)
	$(COPY_CMD) *.h $(INCLUDE_DIR)/dnsResolver

DnsManager.o: DnsManager.cpp DnsManager.h DnsResolver.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATH) -c DnsManager.cpp -o DnsManager$(OBJ_EXTENSION)

DnsMessage.o: DnsMessage.cpp DnsMessage.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATH) -c DnsMessage.cpp -o DnsMessage$(OBJ_EXTENSION)

DnsRequest.o: DnsRequest.cpp DnsRequest.h DnsMessage.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATH) -c DnsRequest.cpp -o DnsRequest$(OBJ_EXTENSION)

DnsResponse.o: DnsResponse.cpp DnsResponse.h DnsRequest.h DnsMessage.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATH) -c DnsResponse.cpp -o DnsResponse$(OBJ_EXTENSION)

DnsResolver.o: DnsResolver.cpp DnsResolver.h DnsRequest.h DnsResponse.h DnsMessage.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATH) -c DnsResolver.cpp -o DnsResolver$(OBJ_EXTENSION)

DnsResolverLocalImpl.o: DnsResolverLocalImpl.cpp DnsResolverLocalImpl.h DnsResolver.h DnsRequest.h DnsResponse.h DnsMessage.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATH) -c DnsResolverLocalImpl.cpp -o DnsResolverLocalImpl$(OBJ_EXTENSION)

clean:
	$(DELETE_CMD) $(OBJS) $(LIB_DIR)/$(LIBRARY) $(INCLUDE_DIR)/dnsResolver/*.h
