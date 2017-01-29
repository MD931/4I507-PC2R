#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int i;

void* fonction(void* arg){
	printf("FILS : PID = %d - PPID %d\n", getpid(), getppid());
	printf("Je suis le thread fils, valeur de i = %d\n",i);
}

int main()
{
	pthread_t thread;
	printf("Entrer une valeur pour i\n");
	scanf("%d",&i);
	printf("PERE : PID = %d\n", getpid());
	pthread_create(&thread, NULL, fonction, NULL);
	printf("Je suis le thread père, valeur de i = %d\n",i);
	pthread_join(thread, NULL);
	
}
