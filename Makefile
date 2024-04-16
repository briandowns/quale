CC = cc

VERSION := 0.1.0
BINDIR = bin
BINARY = quale

LDFLAGS := -lsqlite3 -lulfius -ljansson -lyder -lc
CFLAGS  := -Dapp_name=$(BINARY) -Dgit_sha=$(shell git rev-parse HEAD)

$(BINDIR)/$(BINARY): clean $(BINDIR)
	$(CC) main.c database.c models.c api.c $(CFLAGS) $(LDFLAGS) -o $@
	install_name_tool -change @rpath/libulfius.2.6.dylib /usr/local/lib/libulfius.2.6.3.dylib bin/quale
	install_name_tool -change @rpath/libyder.1.4.dylib /usr/local/lib/libyder.1.4.7.dylib bin/quale 

$(BINDIR):
	mkdir -p $@

clean:
	rm -rf $(BINDIR)
