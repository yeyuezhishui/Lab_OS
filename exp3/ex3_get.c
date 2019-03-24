#include "ex3_header.h"
#include <stdlib.h>


//读取文件大小函数
int file_size(char * filename)
{
	struct stat statbuf;
	stat(filename,&statbuf);//读取文件状态信息
	int size = statbuf.st_size;
	return size;
}

int main()
{
	//get信号灯 
	int semID;
	semID = semget(semKey,2,IPC_CREAT|0666);//0666为权限
	
	//get共享内存
	int shID;
	shID = shmget(shKey,sizeof(shbuf),IPC_CREAT|0666);
	shbuf * Loop_BUF = (shbuf *)shmat(shID,NULL,0);
	
	
	//定义文件指针
	FILE * fp;
	if((fp = fopen("ex3_test.mp4","rb"))==NULL)
	{
		printf("Can not open file\n");
	}
	

	int total_size = file_size("ex3_test.mp4");//文件总大小
	int Buf_Num = 0;//当前缓冲区编号
	double current_size = 0;//当前已读大小
	
	//准备进度条
	char buf[102];//进度条主体
	memset(buf,'\0',sizeof(buf));
	const char* lable="|/-\\";
	int i = 0;
	
	
	
	
	
	while(Loop_BUF->last_block!=1)
	{
		P(semID,0);//P写操作
		Loop_BUF->len_of_block[Buf_Num] = fread(Loop_BUF->buf_block[Buf_Num],sizeof(char),100,fp);//往一个缓冲区里写
		if(Loop_BUF->len_of_block[Buf_Num]<100)//当一个缓冲区没有被写满
		{
			
			Loop_BUF->last_block = 1;//标记遇为最后一个缓冲区出现
		}
		current_size += Loop_BUF->len_of_block[Buf_Num];
		printf("\033[?25l");
		
		//来做一个进度条
		printf("[%-100s][%.0f%%][%c]\r",buf,current_size/total_size*100,lable[i%4]);
		i++;
		int j = 0;
		for(j = 0;j<=(current_size*100/total_size);j++)
		{
			buf[j] = '=';
		}
		Buf_Num = (Buf_Num+1)%10;//环形读取下一个缓冲区
		V(semID,1);//V读操作
	}
	fclose(fp);
	return 0;
}
