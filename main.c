/*
 * Copyright (c) 2014 Daniel Loffgren
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#include "acpi.h"

#define ICON_NAME_PREFIX       "battery"
#define ICON_NAME_FULL         "full"
#define ICON_NAME_GOOD         "good"
#define ICON_NAME_LOW          "low"
#define ICON_NAME_CAUTION      "caution"
#define ICON_NAME_CHARGING     "charging"
#define ICON_NAME_ERROR        "missing"
#define ICON_NAME_SEPARATOR    "-"

#define TRAY_TEXT_LEN          28

#define TRAY_TEXT_CHARGING     "Charging"
#define TRAY_TEXT_NOTFOUND     "Device not found"

#define BATTERY_THRESHOLD_FULL 90 /* percent */
#define BATTERY_THRESHOLD_GOOD 60
#define BATTERY_THRESHOLD_LOW  30

#define REFRESH_INTERVAL       2000 /* milliseconds */

typedef struct {
	const char *devName;
	GtkStatusIcon *trayIcon;
} TrayItem;

static const char *trayTextForBatteryStatus(const char *devname, struct batteryStats *stats) {
	char *result = malloc(TRAY_TEXT_LEN);
	snprintf(result, TRAY_TEXT_LEN, "%s: %2lu%%", devname, stats->percentage);

	if(stats->pluggedIn) {
		strncat(result, ", " TRAY_TEXT_CHARGING, TRAY_TEXT_LEN - strlen(result));
	}

	return result;
}

static const char *iconNameForBatteryStatus(struct batteryStats *stats) {
	if(stats->pluggedIn) {
		if(stats->percentage > BATTERY_THRESHOLD_FULL)
			return ICON_NAME_PREFIX ICON_NAME_SEPARATOR ICON_NAME_FULL ICON_NAME_SEPARATOR ICON_NAME_CHARGING;
		else if(stats->percentage > BATTERY_THRESHOLD_GOOD)
			return ICON_NAME_PREFIX ICON_NAME_SEPARATOR ICON_NAME_GOOD ICON_NAME_SEPARATOR ICON_NAME_CHARGING;
		else if(stats->percentage > BATTERY_THRESHOLD_LOW)
			return ICON_NAME_PREFIX ICON_NAME_SEPARATOR ICON_NAME_LOW ICON_NAME_SEPARATOR ICON_NAME_CHARGING;
		else
			return ICON_NAME_PREFIX ICON_NAME_SEPARATOR ICON_NAME_CAUTION ICON_NAME_SEPARATOR ICON_NAME_CHARGING;
	}
	else {
		if(stats->percentage > BATTERY_THRESHOLD_FULL)
			return ICON_NAME_PREFIX ICON_NAME_SEPARATOR ICON_NAME_FULL;
		else if(stats->percentage > BATTERY_THRESHOLD_GOOD)
			return ICON_NAME_PREFIX ICON_NAME_SEPARATOR ICON_NAME_GOOD;
		else if(stats->percentage > BATTERY_THRESHOLD_LOW)
			return ICON_NAME_PREFIX ICON_NAME_SEPARATOR ICON_NAME_LOW;
		else
			return ICON_NAME_PREFIX ICON_NAME_SEPARATOR ICON_NAME_CAUTION;
	}
}

static gboolean updateTray(TrayItem *item) {
	struct batteryStats stats;
	int rc = getStatsForDevice(item->devName, &stats);

	if(rc == EXIT_SUCCESS) {
		const char *iconName = iconNameForBatteryStatus(&stats);
		const char *trayText = trayTextForBatteryStatus(item->devName, &stats);

		gtk_status_icon_set_from_icon_name(item->trayIcon, iconName);
		gtk_status_icon_set_tooltip_text(item->trayIcon, trayText);

		g_timeout_add(REFRESH_INTERVAL, (GSourceFunc) updateTray, item);
	}
	else {
		char text[TRAY_TEXT_LEN];
		snprintf(text, TRAY_TEXT_LEN, "%s: %s", item->devName, TRAY_TEXT_NOTFOUND);
		gtk_status_icon_set_from_icon_name(item->trayIcon, ICON_NAME_PREFIX
		                                                   ICON_NAME_SEPARATOR
		                                                   ICON_NAME_ERROR);
		gtk_status_icon_set_tooltip_text(item->trayIcon, text);
	}
	/* This function returning EXIT_FAILURE will halt the timer loop, as well */
	return rc;
}

static gboolean trayIconClicked(GtkStatusIcon *status_icon, GdkEvent *event, gpointer user_data) {
	(void)user_data;

	/* Create the contextual menu (which for now only allows quitting) */
	GtkWidget *menu = gtk_menu_new();

	GtkWidget *quitItem = gtk_menu_item_new_with_label("Quit");
	g_signal_connect(quitItem, "activate", G_CALLBACK(gtk_main_quit), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), quitItem);
	g_object_ref_sink(G_OBJECT(quitItem));
	
	gtk_widget_show_all(menu);
	gtk_menu_popup(GTK_MENU(menu), NULL, NULL, gtk_status_icon_position_menu, status_icon, ((GdkEventButton *)event)->button, ((GdkEventButton*)event)->time);
	g_object_ref_sink(G_OBJECT(menu));
	
	return TRUE;
}

int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);
	
	if(argc <= 1) {
		warnx("No device specified");
		return EXIT_FAILURE;
	}
	
	TrayItem **pool = calloc((size_t)argc - 1, sizeof(TrayItem));
	for (int i = 1; i < argc; i++) {
		/* Use the last argument as the device name */
		TrayItem *item = malloc(sizeof(TrayItem));
		item->devName = argv[i];
		item->trayIcon = gtk_status_icon_new();

		g_signal_connect(item->trayIcon, "button-release-event", G_CALLBACK(trayIconClicked), item);

		updateTray(item);
		pool[i - 1] = item;
	}

	/* Only call this once, as it starts a timer loop */
	gtk_main();
	
	for (int i = 1; i < argc - 1; i++)
		g_object_ref_sink(G_OBJECT(pool[i]->trayIcon));

	free(pool);

	return EXIT_SUCCESS;
}


