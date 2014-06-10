CFLAGS=	`pkg-config --cflags gtk+-2.0` -std=c99
LIBS=	`pkg-config --libs gtk+-2.0` -lprop
SRCS=	main.c acpi.c
PROG=	gat
PREFIX=	/usr

$(PROG): $(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)

install: $(PROG)
	install -m 0755 $(PROG) $(PREFIX)/bin

