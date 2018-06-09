/*
以前不太明白，非阻塞就得不断轮询，直到数据准备好
*/

#include <unistd.h>

#include <fcntl.h>

#include <stdio.h>

#include <errno.h>

#include <string.h>

#include <stdlib.h>

#define MSG_TRY "test again\n"

int main(void)

{
    char buf[10];
    int fd, n;
    fd = open("/dev/tty", O_RDONLY|O_NONBLOCK);//用只读和非阻塞的方式打开tty
    if(fd<0)

    {

        printf("open error\n");
        exit(1);
    }

    tryagain:
        n = read(fd, buf, 10);
        if (n < 0)

        {

    /*

    如果在open一个设备时指定了O_NONBLOCK标志，read/write就不会阻塞。以read为例，
     如果设备暂时没有数据可读就返回-1，同时置errno为EWOULDBLOCK（或者EAGAIN，这两个宏定义的值相同）

    */


            if (errno == EAGAIN)

            {

                sleep(1);
                write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY));
                goto tryagain;
            }
            perror("read /dev/tty");   //其他错误，还会有其他错误吗？？


            exit(1);
        }
    write(STDOUT_FILENO, buf, n);
    close(fd);
    return 0;

}
