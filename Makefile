INCLUDE=	-I/usr/pkg/include/gtk-3.0 -I/usr/pkg/include/glib/glib-2.0 -I/usr/pkg/lib/glib-2.0/include
LDFLAGS=	-L/usr/pkg/lib/gtk-3.0
SRCS=		main.c

all:
	$(CC) $(INCLUDE) $(LDFLAGS) $(SRCS)
