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

int main (void) {

	int a[2];
	int b[2];
	pipe(a);
    pid_t p1 = fork(); 
	
	if(p1 == 0){
        close(a[0]);
        dup2(a[1],1);
        execlp("/bin/cut","cut","-d",":","-f","7","/etc/passwd",0);
    }

    close(a[1]);
    pipe(b);
    pid_t p2 = fork();
    
	if(p2 == 0){
        dup2(a[0],0);
        close(b[0]);
        dup2(b[1],1);
        execlp("/bin/sort","sort",0);
    }

    close(a[0]);
    close(b[1]);
    dup2(b[0],0);
    execlp("/bin/uniq","uniq","-c",0);
    close(a[1]);
    close(b[0]);
   
   	exit(0);
} 

