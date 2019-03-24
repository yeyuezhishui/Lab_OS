#include "ex3_header.h"

int main()
{
	//获取信号灯
	int semID;
	semID = semget(semKey,2,IPC_CREAT|0666);
	
	//获取环形缓冲
	int shID;
	shID = shmget(shKey,sizeof(shbuf),IPC_CREAT|0666);
	shbuf * Loop_BUF = (shbuf *)shmat(shID,NULL,0);
	
	//打开要写入的文件
	FILE *fp2;
	if((fp2 = fopen("ex3_test_put.mp4","wb"))==NULL)
	{
		printf("Can not open the file\n");
	}
	
	int Buf_Num_2 = 0;
	while(Loop_BUF->last_block!=1 || Loop_BUF->len_of_block[Buf_Num_2]==100)
	{
		P(semID,1);//P读
		fwrite(Loop_BUF->buf_block[Buf_Num_2],sizeof(char),Loop_BUF->len_of_block[Buf_Num_2],fp2);//每次读取一个缓冲区的内容
		Buf_Num_2 = (Buf_Num_2+1)%10;//循环累加
		V(semID,0);//V写
	}
	if(Loop_BUF->len_of_block[Buf_Num_2]!=0)//当标记为最后一块的缓冲区内还有可读内容
	{
		fwrite(Loop_BUF->buf_block[Buf_Num_2],sizeof(char),Loop_BUF->len_of_block[Buf_Num_2],fp2);//读取最后一块缓冲区
	}
	fclose(fp2);
	return 0;
}
