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
#include <stdbool.h>

int main (int argc, char* argv[]) {

	printf("%d\n", argc);	
	if (argc != 3) {
		err(1, "Bad arguments\n");
	}
	
	int fd1 = open(argv[1], O_RDWR);
	if( fd1 == -1 ) {
		err(2, "Failed to open file\n");
	}
	
	struct stat stf1;
	fstat(fd1, &stf1);

	printf("%ld\n", stf1.st_size);

	if (stf1.st_size % 4 != 0) {
		err(3, "Bad file\n");
	}

	int fd2 = open(argv[2], O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd2 == -1) {
		err(4, "Failed to create file\n");
	}

	int size = stf1.st_size / 4;

	printf("%d", size);
   
    exit(0);
}

