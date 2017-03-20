#include <stdio.h>
#include <unistd.h>


int main()
{
    // fork 一个子进程，shell 运行命令
    // sort 连接到标准输入
    FILE *pRead,*pWrite;
    char buf[1024] = { 0 };

    //对于这个程序来说是 r
    if((pRead = popen("ps -ef","r")) == NULL)
    {
        perror("popen ps failed.");
        return -1;
    }

    //对于这个程序来说是 w 
    if((pWrite = popen("grep pts","w")) == NULL)
    {
        perror("popen grep failed.");
        return -1;
    }


    // read the result of [ps -ef]
    while(fgets(buf,1024,pRead) > 0)
    {
        fprintf(pWrite,"%s",buf);
    }

    pclose(pRead);
    pclose(pWrite);

    getchar();//pause

    return 0;
}
