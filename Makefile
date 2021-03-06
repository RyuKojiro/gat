CFLAGS=		`pkg-config --cflags gtk+-3.0` -std=c99 -Wno-deprecated-declarations
LIBS=		`pkg-config --libs gtk+-3.0` -lprop
SRCS=		main.c acpi.c
PROG=		gat
MANPAGE=	gat.1
PREFIX?=	/usr/local
DESTDIR?=	/

all: $(PROG)

$(PROG): $(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)

install: $(PROG)
	install -m 0755 $(PROG) $(DESTDIR)/$(PREFIX)/bin
	install -m 0644 $(MANPAGE) $(DESTDIR)/$(PREFIX)/man/man1

