#include <sys/types.h>
#include <sys/envsys.h>
#include <prop/proplib.h>
#include <stdio.h>
#include <fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <paths.h>

#define PROP_KEY_MAXIMUM				"max-value"
#define PROP_KEY_CRITICAL				"critical-capacity"
#define PROP_KEY_WARNING				"warning-capacity"
#define PROP_KEY_CURRENT				"cur-value"
#define PROP_KEY_DESCRIPTION			"description"
#define PROP_VALUE_CHARGE_DESCRIPTION	"charge"

int main(int argc, const char *argv[]) {
	int fd, error;
	const char *mydevname = "acpibat0";
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

	// Get charge info, and figure out the state
	

	char *buf = prop_array_externalize(obj);
	printf("%s\n", buf);
	free(buf);

	prop_object_release(battery_dict);
	return EXIT_SUCCESS;
}

