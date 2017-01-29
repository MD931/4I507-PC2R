#include <stdio.h>
#include <unistd.h>

void main(int argc, char **argv){
	
	int m;
	if((m = fork()) == 0)
		printf("Fils : Valeur de retour du fork = %d\n", m);
	else
		printf("Père : Valeur de retour du fork = %d\n", m);
}

