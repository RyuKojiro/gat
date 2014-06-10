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

#define ICON_NAME_ERROR		"dialog-error"

#define ICON_NAME_LEN		25
#define TRAY_TEXT_LEN		28

#define TRAY_TEXT_CHARGING	"Charging"
#define TRAY_TEXT_NOTFOUND	"Device not found"

static const char *mydevname;

static const char *trayTextForBatteryStatus(const char *devname, struct batteryStats *stats) {
	char *result = malloc(TRAY_TEXT_LEN);
	snprintf(result, TRAY_TEXT_LEN, "%s: ", devname);
	snprintf(result + strlen(result), TRAY_TEXT_LEN - strlen(result), "%2lu%%", stats->percentage);

	if(stats->pluggedIn) {
		strncat(result, ", ", TRAY_TEXT_LEN - strlen(result));
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

static gboolean updateTray(GtkStatusIcon *trayIcon) {
	struct batteryStats stats;
	int rc = getStatsForDevice(mydevname, &stats);
	
	if(rc) {
		char text[TRAY_TEXT_LEN];
		snprintf(text, TRAY_TEXT_LEN, "%s: %s", mydevname, TRAY_TEXT_NOTFOUND);
		gtk_status_icon_set_from_icon_name(trayIcon, ICON_NAME_ERROR);
		gtk_status_icon_set_tooltip_text(trayIcon, text);
		
		// This will halt the timer loop, as well
		return EXIT_FAILURE;
	}

	const char *iconName = iconNameForBatteryStatus(&stats);
	const char *trayText = trayTextForBatteryStatus(mydevname, &stats);
	
	gtk_status_icon_set_from_icon_name(trayIcon, iconName);
	gtk_status_icon_set_tooltip_text(trayIcon, trayText);
	
	free((void *)iconName);
	free((void *)trayText);
	
	g_timeout_add(2000, (GSourceFunc) updateTray, trayIcon);
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);
	
	if(argc <= 1) {
		warnx("No device specified");
		return EXIT_FAILURE;
	}

	mydevname = argv[argc - 1];

	GtkStatusIcon *trayIcon = gtk_status_icon_new();
	
	// Only call this once, as it starts a timer loop
	updateTray(trayIcon);
	gtk_main();

	return EXIT_SUCCESS;
}


