#include <stdio.h>
#include <unistd.h>

void main(int argc, char **argv){
	
	int i;
	int m = fork();
	if( m == 0){
		//Child
		sleep(4);
		printf("Valeur de i = %d\n", i);
	}
	else{
		//Father
		printf("Entrer une valeur pour i = ");
		scanf("%d",&i);
		wait();
	}
}

