CFLAGS=	`pkg-config --cflags gtk+-2.0`
LIBS=	`pkg-config --libs gtk+-2.0`
SRCS=	main.c acpi.c
PROG=	gat

all:
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)

