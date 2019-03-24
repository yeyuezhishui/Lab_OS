#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <string.h>

pid_t pid1;
pid_t pid2;
int MyPipe[2];
int pid1_status;
int pid2_statis;
char Input_Pipe[25];
char Output_Pipe[25];

void P1Over()
{
		close(MyPipe[1]);
    printf("Child Process 1 is Killed by Parent!\n");
    exit(0);
}
void P2Over()
{
		close(MyPipe[0]);
    printf("Child Process 2 is Killed by Parent!\n");
    exit(0);
}

void Kill_All()
{
    kill(pid1,SIGUSR1);
    kill(pid2,SIGUSR1);
}

int main(void)
{
    //创建管道
    pipe(MyPipe);
		int len;
    //创建子进程1，2
		pid1 = fork();
		if(pid1 != 0)
		{
			pid2 = fork();
		}

    //子进程1的操作
    if (pid1 == 0)
    {
        signal(SIGINT, SIG_IGN);
        signal(SIGUSR1, P1Over);
        int i = 0;
        close(MyPipe[0]);//关读
        while (1)
        {
            lockf(MyPipe[1], F_LOCK, 0); //写区上锁
            sprintf(Input_Pipe, "I send you %d times\n", ++i);

            //往管道里写
            len = sizeof(Input_Pipe);
            write(MyPipe[1], Input_Pipe, len+1);
            sleep(1);
            lockf(MyPipe[1], F_ULOCK, 0); //写区解锁
        }
    }
    else if (pid2 == 0)
    {
        signal(SIGINT, SIG_IGN);
        signal(SIGUSR1, P2Over);
        close(MyPipe[1]);//关写
        int j = 0;
        while (1)
        {
            lockf(MyPipe[0], F_LOCK, 0); //读区上锁
            if((read(MyPipe[0],Output_Pipe,25)!=0))
            {
                printf("%s",Output_Pipe);
            }
            lockf(MyPipe[0], F_ULOCK, 0); //读区解锁
        }
    }
    else
    {
    	signal(SIGINT,Kill_All);
    	waitpid(pid1,&pid1_status,0);
    	waitpid(pid2,&pid2_statis,0);
    	close(MyPipe[1]);
    	close(MyPipe[0]);
    	printf("parent process is killed!\n");
    	return 0;
    }
}
