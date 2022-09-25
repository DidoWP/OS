#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <string.h>

void readFromStdin(void);

int main(int argc, char* argv[]) {
	
	if ( argc == 1 ) {
		readFromStdin();
	}
	else {
		for (int i=1; i<argc; i++) {
			if (strcmp(argv[i], "-") == 0) {
				readFromStdin();
			}
			else {
				char bf;
				int fd = open(argv[i], O_RDONLY);
				if (fd == -1) {
					errx(1, "Not a file\n");
				}

				while( read(fd, &bf, sizeof(bf)) > 0) {
					printf("%c", bf);
				}
			}
		}
	}

	exit(0);
}

void readFromStdin(void) {
	char buff;
	while( read(0, &buff, sizeof(buff)) > 0 ) {
		printf("%c", buff);
	}
}

