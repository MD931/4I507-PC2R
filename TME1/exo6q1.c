#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int SHARED_compteur;
#define NP_THREAD 10

void* routine(void* arg){
	int temp = SHARED_compteur;
	sched_yield();
	temp++;
	sched_yield();
	SHARED_compteur = temp;
	printf("Compteur = %d\n", SHARED_compteur); 
}

int main()
{	
	SHARED_compteur = 0;
	pthread_t thread[NP_THREAD];
	for(int i = 0;i<NP_THREAD;i++)
		pthread_create(&thread[i], NULL, routine, NULL);
	while(1)
		if(NP_THREAD == SHARED_compteur){
			printf("TERMINE\n");
			break;
		}
	
}
