#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/fcntl.h>

int main()
{
    int fds[2];
    pid_t pid;
    char szBuf[1024];

    if(pipe(fds) != 0)
    {
        perror("pipe fail.");
        return -1;
    }

    pid = fork();
    if(pid == 0)
    {
        //child
        printf("[%d] child pid\n",getpid());
        //重定向fds[0]到STDIN
        dup2(fds[0],STDIN_FILENO);

        close(fds[1]);//close write
        close(fds[0]);//重定向之后就可以关了

        while(1)
        {
            memset(szBuf,0,1024);
            /*printf("11111");*/
            int len = read(STDIN_FILENO,szBuf,1024);//blocked
            if(len == -1)
            {
                perror("read fail.\n");
                break;
            }
            else
            {
                printf("receive:%s",szBuf);
                if(strcmp(szBuf,"exit\n") == 0)
                {
                    break;
                }
            }
        }
        _exit(0);
    }
    else
    {
        //parent
        printf("[%d] parent pid.\n",getpid());

        //重定向到STDOUT
        dup2(fds[1],STDOUT_FILENO);
        close(fds[0]);
        close(fds[1]);//重定向后关了

        while(1)
        {
            fgets(szBuf,1024,stdin);
            write(STDOUT_FILENO,szBuf,strlen(szBuf));
            /*printf("send: %s",szBuf);*/
            if(strcmp(szBuf,"exit\n") == 0)
            {
                wait(NULL);
                break;
            }
            sleep(1);
        }
    }

    return 0;
}
