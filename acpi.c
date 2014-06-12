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
#define PROP_VALUE_CHARGING_DESCRIPTION	"charging"

#define ERR_TEXT_BAD_PLIST				"Unexpected property list data"

int getStatsForDevice(const char *mydevname, struct batteryStats *out) {
	int fd, error;
	prop_dictionary_t battery_dict = prop_dictionary_create();

	/* Get the ACPI PLIST */
	if ((fd = open(_PATH_SYSMON, O_RDONLY)) == -1)
		err(EXIT_FAILURE, "open");
	
	error = prop_dictionary_recv_ioctl(fd, ENVSYS_GETDICTIONARY, &battery_dict);
	close(fd);

	/* Find the device in the PLIST */
	prop_object_t obj = prop_dictionary_get(battery_dict, mydevname);
   	if (prop_object_type(obj) != PROP_TYPE_ARRAY) {
   		warnx("Unknown device `%s'", mydevname);
		prop_object_release(battery_dict);
		return EXIT_FAILURE;
	}
	
	/* Iterate over PLIST dictionaries looking for charge dictionary */
	for(int i = 0; i < prop_array_count(obj); i++) {
		prop_object_t dict = prop_array_get(obj, i);
		if(prop_object_type(dict) != PROP_TYPE_DICTIONARY) {
			warnx(ERR_TEXT_BAD_PLIST);
			prop_object_release(battery_dict);
			return EXIT_FAILURE;
		}

		prop_object_t desc = prop_dictionary_get(dict, PROP_KEY_DESCRIPTION);
		
		/* Get charge info, and figure out the state, from key-value pairs */
		if(prop_string_equals_cstring(desc, PROP_VALUE_CHARGE_DESCRIPTION))  {
			prop_object_t current_o = prop_dictionary_get(dict, PROP_KEY_CURRENT);
			prop_object_t max_o = prop_dictionary_get(dict, PROP_KEY_MAXIMUM);
		
			if(prop_object_type(current_o) != PROP_TYPE_NUMBER || prop_object_type(max_o) != PROP_TYPE_NUMBER) {
				warnx(ERR_TEXT_BAD_PLIST);
				prop_object_release(battery_dict);
				return EXIT_FAILURE;
			}

			int64_t current = prop_number_unsigned_integer_value(current_o);
			int64_t max = prop_number_unsigned_integer_value(max_o);
			
			out->percentage = (current * 100 / max);
		}

		/* Get plugged in or not info */
		if(prop_string_equals_cstring(desc, PROP_VALUE_CHARGING_DESCRIPTION))  {
			prop_object_t charging_o = prop_dictionary_get(dict, PROP_KEY_CURRENT);
			if(prop_object_type(charging_o) != PROP_TYPE_NUMBER) {
				warnx(ERR_TEXT_BAD_PLIST);
				prop_object_release(battery_dict);
				return EXIT_FAILURE;
			}
			
			out->pluggedIn = prop_number_unsigned_integer_value(charging_o);
		}
	}

	prop_object_release(battery_dict);
	return EXIT_SUCCESS;
}

