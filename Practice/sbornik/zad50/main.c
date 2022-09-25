#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

struct pair{
	uint32_t x;
	uint32_t y;
};

int main(int argc, char* argv[]) {
	
	int fd1;
	int fd2;
	int fd3;

	if ( fd1 = (open(argv[1], O_RDONLY)) == -1 ) {
		fprintf(stderr, "Open fd1 failed\n");
		exit(1);		
	}
	if ( fd2 = open(argv[2], O_RDONLY) == -1 ) {
		fprintf(stderr, "Open fd2 failed\n");
		exit(1);
	}
	if( fd3 = open(argv[3], O_RDWR | O_APPEND) == -1 ) {
		fprintf(stderr, "Open fd3 failed\n");
		exit(1);
	}

	if ( fd1 % 8 != 0 ) {
		fprintf(stderr, "Bad file\n");
		exit(1);
	}
	if ( fd2 % 4 != 0 ) {
		fprintf(stderr, "Bad file\n");
		exit(1);
	}

	struct pair p;

	while( read(fd1, &p, sizeof(p)) == sizeof(p)) {
		if ( lseek (fd2, p.x*sizeof(uint32_t), SEEK_SET) == -1 ){
			exit(2);
		}
		
		for ( int i=0; i<p.y; i++ ) {
			read(fd2, &p, sizeof(p));
			write(fd3, &p, sizeof(p));
		}
	}

	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}
