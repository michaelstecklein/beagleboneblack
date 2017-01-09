#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(void) {
	int count = 0;
	system("mkdir allfiles");
	while (1) {
		sleep(1);
		count++;
		printf ("count : %i\n", count);
		char command[100];
		sprintf(command, "echo 'test' > allfiles/file_%i.txt", count);
		system(command);
	}
}
