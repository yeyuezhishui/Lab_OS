#include "ex3_header.h"
#include <unistd.h>
#include <sys/file.h>
#include <sys/wait.h>


int main(int argc, char* argv[])
{
	//创建信号灯
	int semID;
	semID = semget(semKey,2,IPC_CREAT|0666);
	union semun arg;
	arg.val = 10;//10个缓冲区初始都为可写状态
	semctl(semID,0,SETVAL,arg);//初始化可写信号灯
	arg.val = 0;//没有一个缓冲区初始可读
	semctl(semID,1,SETVAL,arg);//初始化可读信号灯
	
	//创建共享内存
	int shID;
	shID = shmget(shKey,sizeof(shbuf),IPC_CREAT|0666);
	shbuf * Loop_BUF = (shbuf *)shmat(shID,NULL,0);
	Loop_BUF->last_block = 0;//初始标记最后一个缓冲区不存在
	int pid1 = -1,pid2 = -1;
	pid1 = fork();
	if(pid1 != 0 && pid2 != 0)
	{
		pid2 = fork();
	}
	if(pid1==0)//子进程1
	{
		execv("./ex3_get",argv);
	}
	else if(pid2==0)//子进程2
	{
		execv("./ex3_put",argv);
	}
	else//主进程
	{
		wait(&pid1);
		wait(&pid2);
		printf("\n succeed!\n");
		//删除信号灯
		
		semctl(semID,1,IPC_RMID,0);
		//释放共享内存
		shmctl(shID,IPC_RMID,0);
	}
	return 0;
}
