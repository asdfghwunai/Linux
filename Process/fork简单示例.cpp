/*
我感觉进程这东西比线程大点，局部变量都是自己的，但线程局部变量是大家的
*/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int glob=10;
static void my_exit1(void)  //进程退出时调用函数
{
    printf("pid=%d first exit handler\n",getpid());
}

static void my_exit2(void)
{
    printf("pid=%d second exit handler\n",getpid());
}

int main()
{
    int local;
    pid_t pid;
    local=8;
    if(atexit(my_exit1)!=0)  //为进程注册的退出时调用函数也会被子进程共享，先注册的后调用
    {
        perror("atexit");
    }

    if(atexit(my_exit2)!=0)  //先调用
    {
        perror("atexit");
    }

    if((pid=fork())==0)
    {
        printf("child pid is %d\n",getpid());   //子进程执行某个任务完后尽量使用exit退出，不然，若父进程中创建的子进程位于循环中，可能会引起未知的行为
    }
    else if(pid>0)
    {
        sleep(10);   //父进程休息10s，就不存在竞争资源了
        glob++;   //global是共享的，local是自己的
        local--;
        printf("father pid is %d\n",getpid());
    }
    else
    {
        perror("fork");
    }
    printf("pid=%d,glob=%d,localar=%d\n",getpid(),glob,local);//这段代码父子进程共享
    return 0;//也可以使用exit(0)
}
