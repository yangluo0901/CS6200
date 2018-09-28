#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int File = 10;
int reader_count = 0;
int writer_count = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t rconds = PTHREAD_COND_INITIALIZER; //reader condition
pthread_cond_t wconds = PTHREAD_COND_INITIALIZER; //writer conditioin

void *reader(void *param){
    pthread_mutex_lock(&m);//enter

    while(writer_count > 0){
        pthread_cond_wait(rconds,&m);
    }
    reader_count ++;

    pthread_mutex_unlock(&m);//exit

    printf("total %d readers are reading the shared number : %d,\n ",reader_count,File);

    pthread_mutex_lock(&m);//enter

    reader_count--;

    pthread_mutex_unlock(&m);//exit

    if(reader_count == 0){
        pthread_cond_signal(wconds);
        
    }
    

}


void *writer(void *param){
    pthread_mutex_lock(&m);//enter
    while(reader_count != 0){
        pthread_cond_wait(wconds,&m);
    }
    writer_count ++;
    pthread_mutex_unlock(&m);//exit
    File ++;
    pthread_mutex_lock(&m);//enter
    writer_count --;
    pthread_mutex_unlock(&m);//exit
    pthread_cond_broadcast(rconds);
    if(reader_count = 5){
        pthread_cond_signal(wconds);
    }

}




int main(int argc, char const *argv[])
{
    pthread_t rpid[5];
    pthread_t wpid[5];
    for(int i=0; i<5; i++){
        if ( pthread_create(&rpid[i],NULL,reader,NULL) != 0){
            fprintf(stderr, "Unable to create reader thread\n");
        }
        if ( pthread_create(&wpid[i],NULL,reader,NULL) != 0){
            fprintf(stderr, "Unable to create writer thread\n");
        }
    }
    for(int i = 0; i < 5; i++) {
		pthread_join(rpid[i], NULL);
        pthread_join(wpid[i], NULL);
	}

	
    return 0;
}
