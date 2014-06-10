#include <gtk/gtk.h>
#include <sys/envsys.h>
#include <stdlib.h>
#include <string.h>

#include "acpi.h"

#define ICON_NAME_PREFIX	"battery"
#define ICON_NAME_FULL		"full"
#define ICON_NAME_GOOD		"good"
#define ICON_NAME_LOW		"low"
#define ICON_NAME_CAUTION	"caution"
#define ICON_NAME_CHARGING	"charging"

#define ICON_NAME_LEN		25

static const char *iconNameForBatteryStatus(struct batteryStats *stats) {
	char *result = malloc(ICON_NAME_LEN);
	snprintf(result, ICON_NAME_LEN, "%s-", ICON_NAME_PREFIX);

	if(stats->percentage > 90)
		strncat(result, ICON_NAME_FULL, ICON_NAME_LEN - strlen(result));
	else if(stats->percentage > 70)
		strncat(result, ICON_NAME_GOOD, ICON_NAME_LEN - strlen(result));
	else if(stats->percentage > 40)
		strncat(result, ICON_NAME_LOW, ICON_NAME_LEN - strlen(result));
	else
		strncat(result, ICON_NAME_CAUTION, ICON_NAME_LEN - strlen(result));

	if(stats->pluggedIn) {
		strncat(result, "-", 1);
		strncat(result, ICON_NAME_CHARGING, ICON_NAME_LEN - strlen(result));
	}

	return result;
}

int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);

	GtkStatusIcon *trayIcon = gtk_status_icon_new();

	struct batteryStats stats;
	int rc = getStatsForDevice("acpibat0", &stats);
	
	if(rc) {
		printf("getStatsForDevice returned failure\n");
		return 1;
	}

	printf("plugged = %lu\npercentage = %lu\n", stats.pluggedIn, stats.percentage);

	const char *iconName = iconNameForBatteryStatus(&stats);
	
	printf("%s\n", iconName);
		
	gtk_status_icon_set_from_icon_name(trayIcon, iconName);
	gtk_main();

	free((void *)iconName);
	return 0;
}


