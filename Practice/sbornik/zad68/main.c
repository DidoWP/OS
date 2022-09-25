#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <unistd.h>
#include <err.h>
#include <string.h>

int main (int argc, char* argv[]) {

	int a[2];

	if( pipe(a) == -1 ) {
		err(1, "Could not create pipe\n");	
	}

	const pid_t p1 = fork();
	if( p1 == -1 ) {
		err(1, "Procces can not be forked\n");
	}

	if( p1 == 0 ) {
		close(a[0]);
		dup2(a[1],1);
		close(a[1]);
		if (execlp("cat", "cat", argv[1], 0) == -1) {
			err(2, "exec error\n");
		}
	}

	close(a[1]);
	dup2(a[0],0);
	execlp("sort","sort",0);
	//while (read(a[0], &buff, 1)) {
	//	printf("%c", buff);
	//}
	close(a[0]);

    exit(0);
}

