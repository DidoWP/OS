#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>

struct patch {
    uint16_t off;
    uint8_t orgn;
	uint8_t new;
}__attribute__((packed));

int main(int argc, char* argv[]) {
        int fd1;
        int fd2;
		int fd3;
		struct stat st1;
		struct stat st2;
		uint8_t rdfd3;
		char buff;

		if ( argc != 4 ){
			errx(1, "Enter 3 arguments!");
		}
        if ((fd1 = (open(argv[1], O_RDONLY))) == -1) {
            err(2, "Open the first file failed!\n");
        }
        if ((fd2 = (open(argv[2], O_RDONLY))) == -1) {
            err(2, "Open the second file failed!\n");
        }

		fstat(fd1, &st1);
		fstat(fd2, &st2);

		if (st2.st_size % sizeof(struct patch) != 0) {
			err(3, "Patch file can not be read properly\n");
		}
		if ( st1.st_size > 65536 ) {
			warnx("The offcet may not be enough.\n");
		}

        fd3 = open(argv[3], O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
		if (fd3 == -1) {
			err(4, "Failed to create file");
		}
		
		while( read(fd1, &buff, sizeof(buff) > 0)) {

			int write_size = write(fd3, &buff, sizeof(buff));
			if (write_size != sizeof(buff)) {
				err(5, "Failed to write to file\n");
			}
		}
		struct patch rpatch;
		int read_size;
		while( (read_size = read(fd2, &rpatch, sizeof(rpatch))) ) {

			if (read_size != sizeof(rpatch) ) {
				err(6, "Failed to read patch file\n");
			}
			
			lseek(fd3, rpatch.off, SEEK_SET);

			if ( read(fd3, &rdfd3, sizeof(rdfd3)) != sizeof(rdfd3) ) {
				err(7, "Failed to read from file\n");
			}
			
			if ( rdfd3 != rpatch.orgn ) {
				errx(8, "Not propriate patch file\n");
			}

			lseek(fd3, rpatch.off, SEEK_SET);

			write(fd3, &rpatch.new, sizeof(rpatch.new));
		}
    
        close(fd1);
        close(fd2);
		close(fd3);
        exit(0);
}
