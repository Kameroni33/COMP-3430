#include <sys/time.h>
#include <stdio.h>

// Yeah I took this from here
// https://stackoverflow.com/a/44896326/1457012
// But there's not a lot of other ways to do it!
long long timeInMilliseconds() {
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

int main(){
    printf("Time in milliseconds is %lld\n", timeInMilliseconds());
    return 0;
}
