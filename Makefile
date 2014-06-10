CFLAGS=		`pkg-config --cflags gtk+-3.0` -std=c99
LIBS=		`pkg-config --libs gtk+-3.0` -lprop
SRCS=		main.c acpi.c
PROG=		lgat
MANPAGE=	gat.1
BINPREFIX=	/usr/pkg
MANPREFIX=	/usr/pkg

$(PROG): $(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)

install: $(PROG)
	install -m 0755 $(PROG) $(BINPREFIX)/bin
	install -m 0644 $(MANPAGE) $(MANPREFIX)/man/man1

