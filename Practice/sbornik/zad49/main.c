#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv[]) {
	int fd1;
	int8_t b;
	int8_t* array[256];
	
	if ( argc != 2 ) {
		fprintf(stderr, "err\n");
		exit(1);
	}

	if ( fd1 = open(argv[1], O_RDONLY) == -1 ) {
		fprintf(stderr, "Open failed\n");
		exit(1);
	}

	while ( read(fd1, &b, 1) > 0 ) {
		fprintf(stdout, b);
	}

	close(fd1);
	exit(0);
}
