//
// test for pthread_mutex
//

#include <pthread.h>
#include <stdio.h>
#include <propeller.h>
#include <unistd.h>

pthread_t t1, t2;
pthread_mutex_t M;

void *
threadfunc(void *arg)
{
  int id = (int)arg;

  printf("thread %d on cog %d: getting mutex\n", id, cogid());
  pthread_yield();
  pthread_mutex_lock(&M);
  printf("thread %d got mutex\n", id);
  printf("thread %d freeing mutex\n", id);
  pthread_mutex_unlock(&M);
  printf("thread %d done\n", id);
  pthread_yield();
  return NULL;
}

int
main()
{
  printf("creating mutex\n");
  pthread_mutex_init(&M, NULL);
  pthread_mutex_lock(&M);
  printf("main: starting threads\n");
  pthread_create(&t1, NULL, threadfunc, (void *)1);
  pthread_create(&t2, NULL, threadfunc, (void *)2);
  pthread_yield();
  printf("main sleeping...\n");
  sleep(2);
  printf("main releasing mutex\n");
  pthread_mutex_unlock(&M);
  printf("main done\n");
  sleep(4);
  return 0;
}
