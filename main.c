#include <gtk/gtk.h>
#include <sys/envsys.h>

#include "acpi.h"

int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);

	GtkStatusIcon *trayIcon = gtk_status_icon_new();

	struct batteryStats stats = getStatsForDevice("acpibat0");

	printf("plugged = %d\npercentage = %d\n", stats.pluggedIn, stats.percentage);
	
	gtk_status_icon_set_from_icon_name(trayIcon, "battery-full");
	gtk_main();

	return 0;
}
