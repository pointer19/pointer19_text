#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
int main(int argc,char* argv[])
{
int i = 0;
        int pid;
		        for(; i < 5; i++)
					        {
								                pid = fork();     //父进程跟子进程同时返回for判断
												                if(pid == 0)
																	                {
																						                        break;    //子进程结束，不返回创建
																												                }
																																                else if(pid < 0)
																																					                {
																																										                        perror("fork error");
																																																                        exit(1);
																																																						                }
																																																										        }
																																																												        if(i == 5)
																																																															        {
																																																																		                printf("I'm parent pid = %d,ppid = %d\n",getpid(),getppid());
																																																																						                return 0;
																																																																										        }
																																																																												
																																																																												        printf("I'm child  pid = %d,ppid = %d\n",getpid(),getppid());	
	
	return 0;	
}

