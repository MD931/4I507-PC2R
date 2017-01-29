#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUF_SIZE 10
#define NB_TURN 20

typedef struct _Buffer {   int nbr;   int elem[BUF_SIZE]; } Buffer;

Buffer* CreerBuffer(){
	Buffer* buf = (Buffer*) malloc(sizeof(Buffer));
	if(buf == NULL){
		fprintf(stderr, "Impossible d'allouer le buffer\n");
		exit(EXIT_FAILURE);
	}
	buf->nbr = 0;
	return buf;
}

void EcrireBuffer(Buffer* buf, int val){
	while(1){
		if(buf->nbr == BUF_SIZE)
			fprintf(stderr,"P\n");
		else
			break;
	}
	buf->elem[buf->nbr] = val;
	sched_yield();
	buf->nbr++;
}

int LireBuffer(Buffer* buf){
	while(1){
		if(buf->nbr == 0)
			fprintf(stderr,"V\n");
		else
			break;
	}
	int val = buf->elem[buf->nbr];
	sched_yield();
	for(int i=1;i<buf->nbr;i++){
		buf->elem[i-1] = buf->elem[i];
	}
	sched_yield();
	buf->nbr--;
	return val;
}

void* THREAD_consommateur(void* arg){
	Buffer* buf = (Buffer*) arg;
	for(int i=0;i<NB_TURN;i++)
		printf("Valeur de consommée = %d\n",LireBuffer(buf));
	return NULL;
}

void* THREAD_producteur(void* arg){
	Buffer* buf = (Buffer*) arg;
	for(int i=0;i<NB_TURN;i++)
		EcrireBuffer(buf, i);
	return NULL;
}

int main()
{	
	pthread_t thread[2];
	Buffer* buf = CreerBuffer();
	pthread_create(&thread[0], NULL, THREAD_producteur, (void*) buf);
	pthread_create(&thread[1], NULL, THREAD_consommateur, (void*) buf);
	pthread_join(thread[0], NULL);
	pthread_join(thread[1], NULL);	
}
