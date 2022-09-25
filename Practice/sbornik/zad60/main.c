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

int main(int argc, char* argv[]) {
	
	if (argc != 3) {
		errx(1, "Enter 2 arguments\n");
	}
	
	uint16_t num;
	uint16_t arr[65536] = {0};

	/*for(int k=0; k<65355; k++) {
		printf("%d ", arr[k]);
	}
	printf("kur");*/

	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1) {
		err(2, "Could not open file\n");
	}

	int fd2 = open(argv[2], O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd2 == -1) {
		err(3, "Could not create file\n");
	}

	struct stat nums;
	if (fstat(fd1, &nums) == -1) {
		err(4, "Problem with stat func\n");
	}
	
	//printf("%ld ", nums.st_size);
	if (nums.st_size % 2 != 0) {
		errx(5, "The size of file is inapropriate\n");
	}

	while (read(fd1, &num, sizeof(num))) {
		arr[num]++;
		//printf("%d\n", num);
	}

	for (uint16_t i=0; i<65535; i++) {
		for (uint16_t j=0; j<arr[i]; j++) {
			printf("%d - %d\n", i, arr[i]);
			dprintf(fd2, "%d ", i);
		}
	}

	exit(0);
}
