#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<unistd.h>
#include <string.h>
typedef void* (*fun)(void*);
int g_Flag=0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
void* thread1(void*);
void* thread2(void*);
 /*
 *  when program is started, a single thread is created, called the initial thread or main thread.
 *  Additional threads are created by pthread_create.
 *  So we just need to create two thread in main().
 */
 int main(int argc, char** argv)
{
    printf("enter main\n");
    pthread_mutex_lock(&mutex);   //阻塞，输出enter main
    pthread_mutex_lock(&mutex);


    printf("leave main\n");
    exit(0);
}



int main(int argc, char** argv)
{
    printf("enter main\n");
    pthread_mutex_unlock(&mutex);  //非阻塞，输出完，说明unlock执行几次都是一次效果，unlock不会阻塞自己
    pthread_mutex_unlock(&mutex);


    printf("leave main\n");
    exit(0);
}
