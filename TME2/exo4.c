/******************************* td2_2.canevas *******************************/

#include "fthread.h"
#include "stdio.h"
#include "unistd.h"
#define NB_TURN 100

ft_event_t  evt;

void awaiter (void *args)
{
	int i, res;

	for (i = 0; i< 5; i++) {
		fprintf(stdout, "debut awater\n");
		ft_thread_await(evt);
		fprintf(stdout, "Evenement reçu\n");
		ft_thread_cooperate_n(3);
	}

	fprintf(stdout, "stop generator!\n");
	ft_scheduler_stop((ft_thread_t) args);
}

void generator (void *args)
{
	int i;

	for (i=0;; ++i) {
		ft_thread_cooperate_n(7);
		fprintf(stdout,"Evenement numéro %d envoyé\n", i);
		ft_thread_generate(evt);
	}
}

void trace(void* args){
	int i;
	for(i=0;i<NB_TURN;i++){
		fprintf(stdout,"==== INSTANT %d ====\n",i);
		ft_thread_cooperate();
	}
	ft_exit();
}



int main (void)
{
	ft_thread_t ft_trace, ft_awaiter, ft_generator;
	ft_scheduler_t sched = ft_scheduler_create ();
	
	evt = ft_event_create(sched);
	ft_trace = ft_thread_create(sched, trace, NULL, NULL);
	ft_generator = ft_thread_create(sched, generator, NULL, NULL);
	ft_awaiter = ft_thread_create(sched, awaiter, NULL, (void*) ft_generator);
	

	ft_scheduler_start(sched);
  

	ft_exit ();
	return 0;
}
