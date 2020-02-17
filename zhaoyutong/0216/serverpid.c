#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<signal.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#define SER_PORT 8000
#define SER_ADDR "192.168.109.1"
#define BUF_SIZE 1024
void catch_child(int signum)
{
  pid_t wpid;
  while((wpid=waitpid(-1,NULL,WNOHANG))>0)
  {
    printf("wait pid =%d\n",wpid);
  }
}
void sys_err(char* str)
{
  perror(str);
  exit(1);
}
int main(int argc, char* argv[])
{
      int lfd,cfd;
      char buf[BUF_SIZE];
      struct sockaddr_in ser_addr,cli_addr;

      struct sigaction act;
      act.sa_handler=catch_child;
      sigemptyset(&act.sa_mask);
      act.sa_flags=SA_RESTART;
      sigaction(SIGCHLD,&act,NULL);

      bzero(&ser_addr,sizeof(ser_addr));
      bzero(&cli_addr,sizeof(cli_addr));

      ser_addr.sin_family=AF_INET;
      ser_addr.sin_port=htons(SER_PORT);
      ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
      lfd=socket(AF_INET,SOCK_STREAM,0);
      int opt=1;
      setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));
      int br=bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
      if(br!=0) 
      {perror("bind error");
       exit(1);
      }
      listen(lfd,128);
      int cli_len=sizeof(cli_addr);
      while(1)
 {    
      cfd=accept(lfd,(struct sockaddr*)&cli_addr,&cli_len);
      char dst[256];
      printf("client is ok IP:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));
      int pid=fork();
      if(pid<0)
      {
       perror("fork error");
       exit(1);
      }
      else if(pid==0)
      {
        close(lfd);  
        while(1)
        {
        int rr=read(cfd,buf,sizeof(buf));
        if(rr==0)
        {
         exit(1);
        }
        write(STDOUT_FILENO,buf,rr);
        write(cfd,buf,rr);
        }
         close(cfd);
      }
      else
      {
        close(cfd);
      }
 }
       close(lfd);
       return 0;
}
