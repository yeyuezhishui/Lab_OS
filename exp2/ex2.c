#include<sys/types.h>
#include<pthread.h>
#include<stdio.h>
#include<sys/sem.h>


int SUM = 0;//累加数
int SemID; //信号灯集合

union semun
{
	int val;
	struct semid_ds *buf;
 	unsigned short int *array;
 	struct seminfo *__buf;
};
  
//定义P操作
void P(int semid,int index)
{
	struct sembuf sem;
	sem.sem_num = index;//要操作的信号量
	sem.sem_op = -1;//P操作
	sem.sem_flg = 0;
	semop(semid,&sem,1);//执行一个操作
	return;
}


//定义V操作
void V(int semid,int index)
{
	struct sembuf sem;
	sem.sem_num = index;
	sem.sem_op = 1;//V操作
	sem.sem_flg = 0;
	semop(semid,&sem,1);
	return;
}

void *subp1()//写线程
{
	int i = 1;
	for(i = 1;i<=100;i++)
	{
		P(SemID,1);//P写操作
		printf("In write_thread: P(Sem_Write)\n");
		SUM+=i;
		printf("In write_thread: SUM has added %d\n",i);
		printf("In write_thread: V(Sem_Read)\n");
		V(SemID,0);//V读操作
	}
}

void *subp2()//读线程
{
	int i;
	for(i = 1;i<=100;i++)
	{
		P(SemID,0);//P读操作
		printf("In read_thread: P(Sem_Read)\n");
		printf("In read_thread: SUM has been read\nSUM = %d\n",SUM);
		printf("In read_thread: V(Sem_Write)\n");
		V(SemID,1);//V写操作
	}
}

int main(void)
{
	key_t key = 1;
	//创建信号量
	SemID = semget(key,2,IPC_CREAT|0666);//0号为缓冲区可读信号灯
										//1号为缓冲区可写信号灯
	//初始化信号灯值
	union semun arg;
	int ctl_re;
		//初始化可读信号灯
	arg.val = 0;
	ctl_re = semctl(SemID,0,SETVAL,arg);
		//初始化可写信号灯
	arg.val = 1;
	ctl_re = semctl(SemID,1,SETVAL,arg);
	
	//创建两个线程
	pthread_t th1,th2;
	int re;
	
	re = pthread_create(&th1,NULL,(void *)subp1,NULL);
	re = pthread_create(&th2,NULL,(void *)subp2,NULL);
	
	//等待两个子线程终止
	re = pthread_join(th1,NULL);
	re = pthread_join(th2,NULL);
	
	//删除信号量集
	ctl_re = semctl(SemID,1,IPC_RMID,arg);
	
	return 0;
}
