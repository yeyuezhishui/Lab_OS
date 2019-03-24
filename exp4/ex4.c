#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>

void printdir(char*dir,int depth)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if((dp=opendir(dir))==NULL)
	{
		printf("Can not open the dir\n");
		return;
	}
	chdir(dir);//dir设为当前目录
	while((entry = readdir(dp))!=NULL)//读取dp目录项的相关信息
	{
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode))//如果是目录
		{
			//跳过“..”和‘.’
			if(strcmp(entry->d_name,"..")==0||strcmp(entry->d_name,".")==0)
				continue;
				
				
			//打印权限
			if(statbuf.st_mode&S_IRUSR)
				printf("r");
			else
				printf("-");
			if(statbuf.st_mode&S_IWUSR)
				printf("w");
			else
				printf("-");
			if(statbuf.st_mode&S_IXUSR)
				printf("x");
			else
				printf("-");
			if(statbuf.st_mode&S_IRGRP)
				printf("r");
			else
				printf("-");
			if(statbuf.st_mode&S_IWGRP)
				printf("w");
			else
				printf("-");
			if(statbuf.st_mode&S_IXGRP)
				printf("x");
			else
				printf("-");
			if(statbuf.st_mode&S_IROTH)
				printf("r");
			else
				printf("-");
			if(statbuf.st_mode&S_IWOTH)
				printf("w");
			else
				printf("-");
			if(statbuf.st_mode&S_IXOTH)
				printf("x\t");
			else
				printf("-\t");
			
			printf("%d\t",depth);//打印深度
			
			struct passwd * pwd;
			pwd = getpwuid(statbuf.st_uid);//由拥有者ID num读取 ID name
			printf("%s\t",pwd->pw_name);//打印文件所有者ID
			
			if((pwd = getpwuid(statbuf.st_gid))!=NULL)//存在组用户
			{
				printf("%s\t",pwd->pw_name);//打印文件组所有者ID
			}
			
			printf("%ld\t",statbuf.st_size);//打印文件大小
			
			printf("%s\t",strtok(ctime(&statbuf.st_mtime)+4,"\n"));//打印最后修改时间
			
			printf("%s\n",entry->d_name);//打印name
			
			//递归
			printdir(entry->d_name,depth+4);
		}
		else
		{	
			//打印权限
			if(statbuf.st_mode&S_IRUSR)
				printf("r");
			else
				printf("-");
			if(statbuf.st_mode&S_IWUSR)
				printf("w");
			else
				printf("-");
			if(statbuf.st_mode&S_IXUSR)
				printf("x");
			else
				printf("-");
			if(statbuf.st_mode&S_IRGRP)
				printf("r");
			else
				printf("-");
			if(statbuf.st_mode&S_IWGRP)
				printf("w");
			else
				printf("-");
			if(statbuf.st_mode&S_IXGRP)
				printf("x");
			else
				printf("-");
			if(statbuf.st_mode&S_IROTH)
				printf("r");
			else
				printf("-");
			if(statbuf.st_mode&S_IWOTH)
				printf("w");
			else
				printf("-");
			if(statbuf.st_mode&S_IXOTH)
				printf("x\t");
			else
				printf("-\t");
			
			printf("%d\t",depth);//打印深度
			
			struct passwd * pwd;
			pwd = getpwuid(statbuf.st_uid);//由拥有者ID num读取 ID name
			printf("%s\t",pwd->pw_name);//打印文件所有者ID
			
			if((pwd = getpwuid(statbuf.st_gid))!=NULL)//存在组用户
			{
				printf("%s\t",pwd->pw_name);//打印文件组所有者ID
			}
			
			printf("%ld\t",statbuf.st_size);//打印文件大小
			
			printf("%s\t",strtok(ctime(&statbuf.st_mtime)+4,"\n"));//打印最后修改时间
			
			printf("%s\n",entry->d_name);//打印name
		}		
	}
	chdir("..");//返回上级目录
	closedir(dp);
}

int main()
{
	char op[100];
	scanf("%s",op);
	char arg[100];
	scanf("%s",arg);
	char dirname[1024];
	getcwd(dirname,1024);
	if(strcmp(op,"ls")==0&&strcmp(arg,"-lR")==0)
	{
	
		printdir(dirname,1);
	}
	
	return 0;
}
