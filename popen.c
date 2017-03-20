#include <stdio.h>
#include <unistd.h>


int main()
{
    // fork 一个子进程，shell 运行命令
    // sort 连接到标准输入
    FILE *pf = popen("sort -r","w");

    fprintf(pf,"AAAAAAA\n");
    fprintf(pf,"DDDDDDD\n",NULL);
    fprintf(pf,"CCCCCCC\n",NULL);
    fprintf(pf,"BBBBBBB\n",NULL);

    sleep(3);
    fprintf(pf,"ZZZZZ\n",NULL);

    pclose(pf);

    return 0;
}
