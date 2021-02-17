#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NTHREADS 6

pthread_key_t tkey = 0;

void destrutor(void *key)
{
	printf("destroying\n");
	free(key);
	pthread_setspecific(tkey, NULL);
}

void printkey(void)
{
	void *global;
	pthread_t tid;
	tid = pthread_self();
	global = pthread_getspecific(tkey);
	printf("stored data for thread %d\n", tid);
}


void* initThread(void *param)
{
	pthread_t tid = pthread_self();
	int* threads;

	threads = malloc(sizeof(pthread_t) + sizeof(int) * 10);
	memcpy(threads, &tid, sizeof(pthread_t));
	pthread_setspecific(tid, threads);
	
	printkey();

	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t threads[NTHREADS];
	int i = 0;
	int rc = 0;

	rc = pthread_key_create(&tkey, destrutor);

	for (i = 0; i < NTHREADS; ++i)
		rc = pthread_create(&threads[i], NULL, initThread, NULL);

	for (i = 0; i < NTHREADS; ++i)
		rc = pthread_join(threads[i], NULL);

	rc = pthread_key_delete(tkey);

	return 0;
}
