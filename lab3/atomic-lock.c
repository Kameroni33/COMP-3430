#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdatomic.h>

#define COUNT_UP_TO 100000000

void mutex_lock( atomic_int * );
void mutex_unlock( atomic_int * );

static atomic_int lock = 0;  // our flag to signal if the lock is held
static int counter = 0;      // the object we wish to lock (update atomically)

void *count_up( void *args )
{
    (void) args;

    for ( int i = 0; i < COUNT_UP_TO; i++ )
    {
        mutex_lock( &lock );
        counter++;  // increment counter until we reach COUNT_UP_TO
        mutex_unlock( &lock );
    }

    return NULL;
}

int main( void )
{
    pthread_t p1, p2;

    pthread_create( &p1, NULL, count_up, NULL );
    pthread_create( &p2, NULL, count_up, NULL );

    pthread_join( p1, NULL );
    pthread_join( p2, NULL );
    printf( "Counter is %d\n", counter );
    return EXIT_SUCCESS;
}

void mutex_lock( atomic_int *lock )
{
    while (*lock == 1) ;  // "spin" and test flag
    *lock = 1;            // set flag
}

void mutex_unlock( atomic_int *lock )
{
    *lock = 0;            // un-set flag
}
