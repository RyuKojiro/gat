clang -o batt main.c -D_REENTRANT -pthread -I/usr/pkg/include/gtk-3.0 -I/usr/pkg/include/at-spi2-atk/2.0 -I/usr/pkg/include/gtk-3.0 -I/usr/pkg/include/glib/gio-unix-2.0/ -I/usr/pkg/include/cairo -I/usr/pkg/include/pango-1.0 -I/usr/pkg/include/harfbuzz -I/usr/pkg/include/pango-1.0 -I/usr/pkg/include/atk-1.0 -I/usr/pkg/include/cairo -I/usr/pkg/include/pixman-1 -I/usr/pkg/include -I/usr/X11R7/include/freetype2 -I/usr/X11R7/include -I/usr/pkg/include/freetype2 -I/usr/pkg/include -I/usr/pkg/include/libpng16 -I/usr/pkg/include/gdk-pixbuf-2.0 -I/usr/pkg/include/libpng16 -I/usr/pkg/include/glib/glib-2.0 -I/usr/pkg/lib/glib-2.0/include -I/usr/pkg/include -I/usr/X11R7/include -L/usr/pkg/lib -Wl,-R/usr/pkg/lib -lgtk-3 -Wl,-R/usr/pkg/lib -lgdk-3 -Wl,-R/usr/pkg/lib -lpangocairo-1.0 -Wl,-R/usr/pkg/lib -lpangoft2-1.0 -Wl,-R/usr/pkg/lib -lpango-1.0 -lm -Wl,-R/usr/pkg/lib -lfontconfig -Wl,-R/usr/pkg/lib -lfreetype -Wl,-R/usr/pkg/lib -latk-1.0 -lcairo-gobject -Wl,-R/usr/pkg/lib -lcairo -Wl,-R/usr/pkg/lib -lgdk_pixbuf-2.0 -Wl,-R/usr/pkg/lib -lgio-2.0 -Wl,-R/usr/pkg/lib -lgobject-2.0 -Wl,-R/usr/pkg/lib -lglib-2.0 -lintl 