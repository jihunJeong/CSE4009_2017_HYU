#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>


#define NUM_THREAD 100

enum { STATE_A, STATE_B } state = STATE_A;

pthread_cond_t  condA = PTHREAD_COND_INITIALIZER;
pthread_cond_t  condB = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int arr[40];
int max_num = 6;
int min_num = 1;
int input_num = 1;

void *threadA() {
	while (1) {
		/*Wait for stat A */
		pthread_mutex_lock(&mutex);
		while(state != STATE_A)
			pthread_cond_wait(&condA, &mutex);
		pthread_mutex_unlock(&mutex);

		for (int i = 1; i <= max_num; i++) {
			arr[i - 1] = input_num;
			input_num++;
			
			for (int k = 0; k < i; k++) {
				printf("%d,", arr[k]);
			}

			printf("\n");
		}

		pthread_mutex_lock(&mutex);
		state = STATE_B;
		pthread_cond_signal(&condB);
		pthread_mutex_unlock(&mutex);
	}

	return 0;
}	

void *threadB() {
	while(1) {
		pthread_mutex_lock(&mutex);
		while (state != STATE_B)
			pthread_cond_wait(&condB, &mutex);
		pthread_mutex_unlock(&mutex);

		int i = 0;
		while(i < max_num) {
			printf("popped %d\n", arr[i]);
			i++;
		}

		if (max_num == 6) {
			min_num = 7;
			max_num = 17;
		} else if (max_num == 17) {
			min_num = 24;
			max_num = 40;
		} else {
			min_num += 40;
		}

		pthread_mutex_lock(&mutex);
		state = STATE_A;
		pthread_cond_signal(&condA);
		pthread_mutex_unlock(&mutex);
	}

	return 0;
}

int main () 
{
	pthread_t a_thread, b_thread;
	
	pthread_create(&a_thread, NULL, threadA, NULL);
	pthread_create(&b_thread, NULL, threadB, NULL);

	pthread_join(a_thread, NULL);
	pthread_join(b_thread, NULL);
}