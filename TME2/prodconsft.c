/******************************** prodconsft.canevas *************************/

#include "fthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "prodconsft.h"

/*************************************/

// A CONTINUER .........


typedef struct cell
{
   int          value;
   struct cell *next;
} *cell;

typedef struct file
{
   int  length;
   cell first;
   cell last;
} *file;

file add (int v, file l)
{
   cell c   = (cell)malloc(sizeof(struct cell));
   c->value = v;
   c->next  = NULL;

   if (l == NULL) {
      l         = (file)malloc(sizeof(struct file));
      l->length = 0;
      l->first  = c;
   } else {
      l->last->next = c;
   }
   l->length++;
   l->last = c;
   return l;
}

void put (int v, file *l)
{
  (*l) = add(v,*l);
}

int get (file *l)
{
  int res;
  file f = *l;
  if (l == NULL) {
     fprintf (stdout, "get error!\n");
     return 0;
  }
  res = f->first->value;
  f->length--;
  if (f->last == f->first) {
     *l = NULL;			//     a la place de f = NULL;
  } else {
    f->first = f->first->next;
  }
  return res;
}

int size (file l)
{
   if (l == NULL) return 0;
   return l->length;
}


/*************************************/

file in = NULL, out = NULL;
ft_scheduler_t in_sched, out_sched;
ft_event_t new_input, new_output;

/*************************************/


void process_value (int v, int n)
{
 
  fprintf(stdout,"PROCESS Value = %d | N = %d\n",v,n);
  ft_thread_unlink();
  //usleep(50000);
  ft_thread_link(out_sched);
  put(v, &out);
  ft_thread_generate(new_output);
  ft_thread_unlink();
  ft_thread_link(in_sched);

}


void trace(void* args){
	
	int nb_turn;
	nb_turn = (int) args;
	int i;
	for(i=0;i<nb_turn;i++){
		fprintf(stdout,"==== INSTANT %d ====\n",i);
		ft_thread_cooperate();
	}
	ft_exit();
}


void process (void *args)
{
  while (1) {
    if (size(in) > 0) {
       process_value(get(&in), (int)args);
    } else {
       ft_thread_await (new_input);
       if (size(in) == 0) ft_thread_cooperate();
    }
  }
}

void produce (void *args)
{
  int v = 0;
  while (v < PRODUCED) {
    fprintf(stdout,"PRODUCE %d\n",v);
    put(v++, &in);
    ft_thread_generate(new_input);
    ft_thread_cooperate();
  }
}

void consume (void *args)
{
  int v = 0;
  while (v < PRODUCED) {

    fprintf(stdout,"CONSUME %d\n",v);
    int tmp;
    if(out != NULL){
       tmp = get(&out);
       v++;
    }
    else{
       ft_thread_await(new_output);
    }

    ft_thread_cooperate();

  }
  exit(0);
}

int main (void)
{
   int i;
   ft_thread_t thread_array[MAX_THREADS];

   in_sched   = ft_scheduler_create();
   out_sched  = ft_scheduler_create();
   
   new_input  = ft_event_create(in_sched);
   new_output = ft_event_create(out_sched);

   ft_thread_create(in_sched, trace, NULL, (void*) 50);

   for (i=0; i<MAX_THREADS; i++) {
       thread_array[i] = ft_thread_create(in_sched, process, NULL, (void *) i);
   }
  
   ft_thread_create(in_sched, produce, NULL, NULL);
   ft_thread_create(out_sched, consume, NULL, NULL);

   ft_scheduler_start(in_sched);
   ft_scheduler_start(out_sched);  

   ft_exit();
   return 0;
}

