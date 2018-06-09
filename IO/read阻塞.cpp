//输入啥就输出啥

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)

{
    char buf[10];
    int n;
    n = read(STDIN_FILENO, buf, 10);//从标准输入读取字符到buf里面
    if (n < 0)

    {
        printf("read error");
        exit(1);
    }
    write(STDOUT_FILENO, buf, n);//把buf的数据写到标准输出上（终端）


    return 0;

}
