#include <gtk/gtk.h>
#include <sys/envsys.h>

int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);

	GtkStatusIcon *trayIcon = gtk_status_icon_new();

	gtk_status_icon_set_from_icon_name(trayIcon, "battery-full");
	gtk_main();

	return 0;
}
