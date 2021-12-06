#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg) {
    int i , res;
    /*settiamo stato di cancellazione PTHREAD_CANCEL_ENABLE*/
    res=pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    /* controllo su pthread_setcancelstate */
    if (res != 0) {
        perror("pthread_setcancelstate failed");
        exit (EXIT_FAILURE);
    }
    /*setto tipo di cancellazione PTHREAD_CANCEL_DEFERRED - Il thread sarà cancellato al 1° cancellation point utile*/
    res=pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
    /* controllo su pthread_setcanceltype */
    if (res != 0) {
        perror("Thread pthread_setcanceltype failed");
        exit (EXIT_FAILURE);
    }
    printf("thread_function is running\n");
    for (i=0;i<10;i++) {
        printf("Thread is still running (%d)...\n",i);
        sleep(1); /* punto di cancellazione */
    }
    pthread_exit(0);
}

int main(void) {
    int res;
    pthread_t a_thread;
    void *thread_result;
    /* creazione thread */
    res=pthread_create(&a_thread,NULL,thread_function,NULL);
    if (res != 0) { /* controllo pthread_create */
        perror("thread creation failed");
        exit (EXIT_FAILURE);
    }
    sleep(3); /* così il main thread lascia spazio all’esecuzione del nuovo thread */
    printf("Cancelling thread...\n");
    /* richiesta di cancellazione del thread a_thread */
    res=pthread_cancel(a_thread);
    /* controllo pthread_cancel() */
    if (res != 0) {
        perror("thread cancellation failed");
        exit (EXIT_FAILURE);
    }
    printf("waiting for thread to finish...\n");
    res=pthread_join(a_thread,&thread_result); /*si sincronizza sulla fine di a_thread*/
    /* controllo su pthread_join */
    if (res!=0) {
        perror ("Thread join failed");
        exit (EXIT_FAILURE);
    }
    printf("Done\n");
    exit(EXIT_SUCCESS);
}
