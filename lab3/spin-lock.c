#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define COUNT_UP_TO 100000000

void mutex_lock( int * );
void mutex_unlock( int * );

static int lock = 0;
static int counter = 0;

void *count_up( void *args )
{
    (void) args;

    for ( int i = 0; i < COUNT_UP_TO; i++ )
    {
        mutex_lock( &lock );
        counter++;
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

void mutex_lock( int *lock )
{
    while (lock == 1) ;  // "spin" and test flag
    lock = 1;            // set flag
}

void mutex_unlock( int *lock )
{
    lock = 0;            // un-set flag
}
