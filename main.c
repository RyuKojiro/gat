#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#include "acpi.h"

#define ICON_NAME_PREFIX	"battery"
#define ICON_NAME_FULL		"full"
#define ICON_NAME_GOOD		"good"
#define ICON_NAME_LOW		"low"
#define ICON_NAME_CAUTION	"caution"
#define ICON_NAME_CHARGING	"charging"

#define ICON_NAME_ERROR		"battery-missing"

#define ICON_NAME_LEN		25
#define TRAY_TEXT_LEN		28

#define TRAY_TEXT_CHARGING	"Charging"
#define TRAY_TEXT_NOTFOUND	"Device not found"

#define REFRESH_INTERVAL	2000 /* milliseconds */

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
	else if(stats->percentage > 60)
		strncat(result, ICON_NAME_GOOD, ICON_NAME_LEN - strlen(result));
	else if(stats->percentage > 30)
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
		
		/* This will halt the timer loop, as well */
		return EXIT_FAILURE;
	}

	const char *iconName = iconNameForBatteryStatus(&stats);
	const char *trayText = trayTextForBatteryStatus(mydevname, &stats);
	
	gtk_status_icon_set_from_icon_name(trayIcon, iconName);
	gtk_status_icon_set_tooltip_text(trayIcon, trayText);
	
	free((void *)iconName);
	free((void *)trayText);
	
	g_timeout_add(REFRESH_INTERVAL, (GSourceFunc) updateTray, trayIcon);
	return EXIT_SUCCESS;
}

static gboolean trayIconClicked(GtkStatusIcon *status_icon, GdkEvent *event, gpointer user_data) {
	/* Create the contextual menu (which for now only allows quitting) */
	GtkWidget *menu = gtk_menu_new();

	GtkWidget *quitItem = gtk_menu_item_new_with_label("Quit");
	g_signal_connect(quitItem, "activate", G_CALLBACK(gtk_main_quit), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), quitItem);
	g_object_unref(G_OBJECT(quitItem));
	
	gtk_widget_show_all(menu);
	gtk_menu_popup(GTK_MENU(menu), NULL, NULL, gtk_status_icon_position_menu, status_icon, ((GdkEventButton *)event)->button, ((GdkEventButton*)event)->time);
	g_object_unref(G_OBJECT(menu));
	
	return TRUE;
}

int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);
	
	if(argc <= 1) {
		warnx("No device specified");
		return EXIT_FAILURE;
	}
	
	/* Use the last argument as the device name */
	mydevname = argv[argc - 1];

	GtkStatusIcon *trayIcon = gtk_status_icon_new();
	g_signal_connect(trayIcon, "button-release-event", G_CALLBACK(trayIconClicked), NULL);

	/* Only call this once, as it starts a timer loop */
	updateTray(trayIcon);
	gtk_main();
	
	g_object_unref(G_OBJECT(trayIcon));
	
	return EXIT_SUCCESS;
}


