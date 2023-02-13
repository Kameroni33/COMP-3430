#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define COUNT_UP_TO 100000000

pthread_mutex_t lock;    // our pthread_mutex lock
static int counter = 0;  // the object we wish to lock (update atomically)

void *count_up( void *args )
{
    (void) args;

    for ( int i = 0; i < COUNT_UP_TO; i++ )
    {
        pthread_mutex_lock(&lock);
        counter++;  // increment counter until we reach COUNT_UP_TO
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main( void )
{
    pthread_t p1, p2;

    thread_mutex_init(&lock, NULL);

    pthread_create( &p1, NULL, count_up, NULL );
    pthread_create( &p2, NULL, count_up, NULL );

    pthread_join( p1, NULL );
    pthread_join( p2, NULL );
    printf( "Counter is %d\n", counter );
    return EXIT_SUCCESS;
}
