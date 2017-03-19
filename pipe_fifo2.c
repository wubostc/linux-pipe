#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>


int main()
{
    char buf[1024];
    int fda[2];
    int fdb[2];
#define R_A fda[0]
#define W_A fda[1]
#define R_B fdb[0]
#define W_B fdb[1]

    pid_t pid;

    if(pipe(fda) != 0)
    {
        close(R_A);
        close(W_A);
        perror("pipe failed.");
        return -1;
    }
    if(pipe(fdb) != 0)
    {
        close(R_B);
        close(W_B);
        perror("pipe failed.");
        return -1;
    }

    pid = fork();
    /* child 小写转大写 */
    if(pid == 0)
    {
        printf("[%d] child pid\n",getpid());
        close(W_A);
        close(R_B);

        while(1)
        {
            memset(buf,0,1024);
            read(R_A,buf,1024);

            if(strcmp(buf,"exit\n") == 0)
            {
                break;
            }

            size_t i;
            for(i = 0; i < strlen(buf); ++i)
            {
                buf[i] = toupper(buf[i]);
            }

            //sent to parent
            write(W_B,buf,strlen(buf));

        }

        close(W_B);
        close(R_A);
        _exit(0);
    }
    else
    {
        printf("[%d] parent pid\n",getpid());
        close(R_A);
        close(W_B);

        while(1)
        {
            if(fgets(buf,1024,stdin) != NULL)
            {
                if(strcmp(buf,"exit\n") == 0)
                {
                    break;
                }

                printf("lower:%s",buf);

                write(W_A,buf,strlen(buf));
                read(R_B,buf,1024);


                printf("upper:%s",buf);
            }
        }

        close(R_B);
        close(W_A);
        wait(NULL);
    }

    return 0;
}
