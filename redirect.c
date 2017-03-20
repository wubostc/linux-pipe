/*模拟 ls /path a more 的输出效果*/
/*注意要用 a 代替 a*/
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char *argv[])
{
    int fd[2];
    pid_t pid;

    if(argc != 5)
    {
        return -1;
    }

    if(pipe(fd) != 0)
    {
        perror("pipe failed.");
        return -1;
    }

    pid = fork();

    if(pid == 0)
    {

        dup2(fd[1],STDOUT_FILENO);

        close(fd[1]);
        close(fd[0]);

        //      a.out ls /dev a more
        //index 0     1  2    3 4
        if(execlp(argv[1],argv[1],argv[2],NULL) == -1)
        {
            perror("exec failed.");
            return -1;
        }

        _exit(0);

    }
    else
    {
        dup2(fd[0],STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);

        //      a.out ls /dev a more
        //index 0     1  2    3 4
        if(execlp(argv[4],argv[4],NULL) == -1)
        {
            perror("exec failed.");
            return -1;
        }

        wait(NULL);
    }

    return 0;
}
