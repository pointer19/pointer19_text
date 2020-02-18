#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/select.h>
#define SER_PORT 8000
#define SER_ADDR "127.0.0.1"
#define BUF_SIZE 1024
void sys_err(char* str)
{
  perror(str);
  exit(1);
}
int main(int argc, char* argv[])
{     int i,rr;
      int lfd,cfd;
      int maxfd;
      char buf[BUF_SIZE];
      fd_set rset,aset;
      FD_ZERO(&aset);
      struct sockaddr_in ser_addr,cli_addr;
      bzero(&ser_addr,sizeof(ser_addr));
      bzero(&cli_addr,sizeof(cli_addr));
      ser_addr.sin_family=AF_INET;
      ser_addr.sin_port=htons(SER_PORT);
      ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);      
      lfd=socket(AF_INET,SOCK_STREAM,0);
      int opt=1;
      setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));
      maxfd=lfd;
      FD_SET(lfd,&aset);
      bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
      listen(lfd,128);
      int cli_len=sizeof(cli_addr);
      int sr;
      while(1)
     { rset=aset;
      sr=select(maxfd+1,&rset,NULL,NULL,NULL);
        if(sr<0)
        { sys_err("select error");
        }
      else if(sr>0)
      {   if(FD_ISSET(lfd,&rset))                      
         {cfd=accept(lfd,(struct sockaddr*)&cli_addr,&cli_len);
          char dst[256];
          printf("client is ok IP:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));
           FD_SET(cfd,&aset);         
           if(cfd>maxfd)
           {   
             maxfd=cfd;
           }   
           if(--sr==0)
           {   
              continue;
           }
         }                
        for(i=lfd+1;i<maxfd+1;i++)
        {
           if(FD_ISSET(i,&rset))
            { rr=read(i,buf,sizeof(buf));
              if(rr<0)
              {sys_err("read error");
              }
              else if(rr==0)
              { printf("client lost\n");
                close(i);
                FD_CLR(i,&aset);
              }
              else
              { 
                 write(STDOUT_FILENO,buf,rr);
                 write(i,buf,rr);
              }
              if(--sr==0)
              { break;
              }
            }
         }
       }
    }
       return 0;
}
