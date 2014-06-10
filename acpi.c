#include <sys/types.h>
#include <sys/envsys.h>
#include <prop/proplib.h>
#include <stdio.h>
#include <fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <paths.h>

#include "acpi.h"

#define PROP_KEY_MAXIMUM				"max-value"
#define PROP_KEY_CRITICAL				"critical-capacity"
#define PROP_KEY_WARNING				"warning-capacity"
#define PROP_KEY_CURRENT				"cur-value"
#define PROP_KEY_DESCRIPTION			"description"
#define PROP_VALUE_CHARGE_DESCRIPTION	"charge"

struct batteryStats getStatsForDevice(const char *mydevname) {
	int fd, error;
	prop_dictionary_t battery_dict = prop_dictionary_create();

	// Get the ACPI PLIST
	if ((fd = open(_PATH_SYSMON, O_RDONLY)) == -1)
		err(EXIT_FAILURE, "open");
	
	error = prop_dictionary_recv_ioctl(fd, ENVSYS_GETDICTIONARY, &battery_dict);
	close(fd);

	// Find the device in the PLIST
	prop_object_t obj = prop_dictionary_get(battery_dict, mydevname);
   	if (prop_object_type(obj) != PROP_TYPE_ARRAY) {
   		warnx("unknown device `%s'", mydevname);
		prop_object_release(battery_dict);
		return EXIT_FAILURE;
	}
	
	// Iterate over PLIST dictionaries looking for charge dictionary
	for(int i = 0; i < prop_array_count(obj); i++) {
		prop_object_t dict = prop_array_get(obj, i);
		if(prop_object_type(dict) != PROP_TYPE_DICTIONARY) {
			warnx("unexpected data");
			prop_object_release(battery_dict);
			return EXIT_FAILURE;
		}

		prop_object_t desc = prop_dictionary_get(dict, PROP_KEY_DESCRIPTION);
		if(prop_string_equals_cstring(desc, PROP_VALUE_CHARGE_DESCRIPTION))  {
			// This dictionary is a match
			// Get charge info, and figure out the state, from key-value pairs
			
		}
	}

	char *buf = prop_array_externalize(obj);
	printf("%s\n", buf);
	free(buf);

	prop_object_release(battery_dict);
	return EXIT_SUCCESS;
}

