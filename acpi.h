
#ifndef _ACPI_H
#define _ACPI_H

struct batteryStats {
	uint64_t pluggedIn;
	uint64_t percentage;
};

int getStatsForDevice(const char *mydevname, struct batteryStats *out);

#endif

