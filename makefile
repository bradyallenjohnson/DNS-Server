include makefile.include

all:
	$(CD_CMD) dnsUtils;      $(MAKE) all
	$(CD_CMD) socketServer;  $(MAKE) all
	$(CD_CMD) dnsLoader;     $(MAKE) all
	$(CD_CMD) dnsResolver;   $(MAKE) all
	$(CD_CMD) main;          $(MAKE) all

clean:
	$(CD_CMD) dnsUtils;      $(MAKE) clean
	$(CD_CMD) socketServer;  $(MAKE) clean
	$(CD_CMD) dnsLoader;     $(MAKE) clean
	$(CD_CMD) dnsResolver;   $(MAKE) clean
	$(CD_CMD) main;          $(MAKE) clean

