#include <sys/types.h>
#include <sys/envsys.h>
#include <prop/proplib.h>
#include <stdio.h>
#include <fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, const char *argv[]) {
	int fd, error;
	prop_dictionary_t battery_dict = prop_dictionary_create();

        if ((fd = open("/dev/sysmon", O_RDWR)) == -1)
		err(EXIT_FAILURE, "open");
	
	error = prop_dictionary_send_ioctl(battery_dict, fd, ENVSYS_GETDICTIONARY);
	close(fd);

	printf("%s\n", prop_dictionary_externalize(battery_dict));
	prop_object_release(battery_dict);
	return EXIT_SUCCESS;
}

