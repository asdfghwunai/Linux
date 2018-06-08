//忽略，屏蔽信号
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

int main(int arg, char *args[])
{
    pid_t pid=fork();
    if(pid==-1)
    {
        printf("fork() failed! error message:%s\n",strerror(errno));
        return -1;
    }
    //注册信号，屏蔽SIGCHLD信号，子进程退出，将不会给父进程发送信号，因此也不会出现僵尸进程
    signal(SIGCHLD,SIG_IGN);
    if(pid>0)
    {
        printf("father is runing !\n");
        sleep(10);
    }
    if(pid==0)
    {
        printf("i am child!\n");
        exit(0);
    }
    printf("game over!\n");
    return 0;
}
//================================================


//恢复信号
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

void catch_signal(int sign)
{
    switch (sign)
    {
    case SIGINT:
        printf("ctrl + C 被执行了!\n");    //经试验ctrl + C跳不出来了
        //exit(0);
        break;
    }
}

int main(int arg, char *args[])
{
    //注册终端中断信号
    signal(SIGINT, catch_signal);
    char tempc = 0;
    while ((tempc = getchar()) != 'a')
    {
        printf("tempc=%d\n", tempc);
        //sleep()
    }
    //恢复信号
    signal(SIGINT, SIG_DFL);     //回复后可以中断了
    while (1)
    {
        pause();
    }
    printf("game over!\n");
    return 0;
}

//=============================
//signal()函数的返回值
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

void catch_signal(int sign)
{
    switch (sign)
    {
    case SIGINT:
        printf("ctrl + C 被执行了!\n");
        //exit(0);
        break;
    }
}

int main(int arg, char *args[])
{
    /*
     * signal()函数的返回值是signal()函数上一次的行为
     * */
    typedef void (*sighandler_t)(int);
    //因为第一次注册信号SIGINT,所以上一次的行为就是默认行为
    sighandler_t old=signal(SIGINT, catch_signal);
    if(old==SIG_ERR)
    {
        //注册信号失败
        perror("signal error");
    }
    /*正规写法*/
    if(signal(SIGQUIT,catch_signal)==SIG_ERR)
    {
        //注册新号失败
        perror("signal error");
    }
    char tempc = 0;
    while ((tempc = getchar()) != 'a')
    {
        printf("tempc=%d\n", tempc);
        //sleep()
    }
    //把默认行为重新注册，不就是恢复默认信号了
    signal(SIGINT, old);
    while (1)
    {
        pause();
    }
    printf("game over!\n");
    return 0;
}
