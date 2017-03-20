#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#define FIFO_NAME "pnod"

void StartRead()
{
    char buf[1024];
    int fd;

    fd = open(FIFO_NAME,O_RDONLY);

    if(fd < 0)
    {
        perror("open read fail.");
        return;
    }

    while(1)
    {
        memset(buf,0,1024);
        if(read(fd,buf,1024) == -1)
        {
            perror("Read fail.");
            return;
        }

        printf("Recv:%s",buf);

        if(strcmp(buf,"exit\n") == 0)
        {
            break;
        }
    }
}

void StartWrite()
{
    char buf[1024];
    int fd;

    fd = open(FIFO_NAME,O_WRONLY);
    if(fd < 0)
    {
        perror("open write fail.");
        return;
    }

    while(1)
    {
        fprintf(stdout,"Send:",NULL);
        fflush(stdout);
        fgets(buf,1024,stdin);

        write(fd,buf,strlen(buf));

        if(strcmp(buf,"exit\n") == 0)
        {
            break;
        }
    }
}

int main(int argc,char *argv[])
{
    struct stat info;

    if(argc != 2 || (strcmp(argv[1],"r") && strcmp(argv[1],"w")))
    {
        printf("usage:%s [r | w]",argv[0]);
        printf("\t r:read fifo\n\t w:write fifo\n");
        return -1;
    }

    //get info by filename
    if(stat(FIFO_NAME, &info) == -1)
    {
        mkfifo(FIFO_NAME,0600); // create fifo if not exist
        stat(FIFO_NAME, &info);
    }
//#define	__S_IFIFO	0010000	/* FIFO.  */

    if(info.st_mode == (0010000 | 0600))
    {
        if(argv[1][0] == 'r')
        {
            StartRead();
        }
        if(argv[1][0] == 'w')
        {
            StartWrite();
            popen("rm pnod","w");
        }
    }
    else
    {
        perror("fatal error:");
    }

    return 0;
}
