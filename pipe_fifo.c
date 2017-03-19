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

        close(fds[1]);//close write

        while(1)
        {
            memset(szBuf,0,1024);
            /*printf("11111");*/
            int len = read(fds[0],szBuf,1024);//blocked
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
        close(fds[0]);
        _exit(0);
    }
    else
    {
        //parent
        printf("[%d] parent pid.\n",getpid());
        close(fds[0]);

        while(1)
        {
            fgets(szBuf,1024,stdin);
            /*szBuf[strlen(szBuf)] = '\n';*/
            write(fds[1],szBuf,strlen(szBuf));
            printf("send: %s",szBuf);
            if(strcmp(szBuf,"exit\n") == 0)
            {
                wait(NULL);
                break;

            }
            sleep(1);
        }

        close(fds[1]);

        return 0;
    }

    return 0;
}
