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
#define TRAY_TEXT_LEN		24

#define TRAY_TEXT_CHARGING		"Charging"

static const char *trayTextForBatteryStatus(const char *devname, struct batteryStats *stats) {
	char *result = malloc(TRAY_TEXT_LEN);
	snprintf(result, TRAY_TEXT_LEN, "%s: ", devname);
	snprintf(result + strlen(result), TRAY_TEXT_LEN - strlen(result), "%2lu%%", stats->percentage);

	if(stats->pluggedIn) {
		strncat(result, TRAY_TEXT_CHARGING, TRAY_TEXT_LEN - strlen(result));
	}

	return result;
}

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

	const char *devname = "acpibat0";

	struct batteryStats stats;
	int rc = getStatsForDevice(devname, &stats);
	
	if(rc) {
		printf("getStatsForDevice returned failure\n");
		return 1;
	}

	const char *iconName = iconNameForBatteryStatus(&stats);
	const char *trayText = trayTextForBatteryStatus(devname, &stats);
	
	gtk_status_icon_set_from_icon_name(trayIcon, iconName);
	gtk_status_icon_set_tooltip_text(trayIcon, trayText);
	gtk_main();

	free((void *)iconName);
	return 0;
}


