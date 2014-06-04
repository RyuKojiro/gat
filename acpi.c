#include <sys/types.h>
#include <sys/envsys.h>
#include <prop/proplib.h>
#include <stdio.h>
#include <fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <paths.h>

int main(int argc, const char *argv[]) {
	int fd, error;
	const char *mydevname = "acpibat0";
	prop_dictionary_t battery_dict = prop_dictionary_create();

        if ((fd = open(_PATH_SYSMON, O_RDONLY)) == -1)
		err(EXIT_FAILURE, "open");
	
	error = prop_dictionary_recv_ioctl(fd, ENVSYS_GETDICTIONARY, &battery_dict);
	close(fd);

       	prop_object_t obj = prop_dictionary_get(battery_dict, mydevname);
       	if (prop_object_type(obj) != PROP_TYPE_ARRAY) {
       		warnx("unknown device `%s'", mydevname);
		prop_object_release(battery_dict);
		return EXIT_FAILURE;
        }

	char *buf = prop_array_externalize(obj);
	printf("%s\n", buf);
	free(buf);

	prop_object_release(battery_dict);
	return EXIT_SUCCESS;
}

