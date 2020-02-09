  #include<stdio.h>
  #include<stdlib.h>
  #include<string.h>
  #include<unistd.h>
  #include<errno.h>
  #include<fcntl.h>
  #include<signal.h>
  #include<sys/wait.h>
void sys_err(char *str)
{
 perror(str);
 exit(1);
}
void catch_child(int signum)
{
  pid_t wpid;
  while((wpid =waitpid(-1,NULL,WNOHANG))>0)
  {
    printf("wait pid =%d\n",wpid);
  }
}
int main(int argc, char* argv[])
  {
    pid_t pid;
    int i;
    sigset_t myset;
    sigemptyset(&myset);
    sigaddset(&myset,SIGCHLD);
    sigprocmask(SIG_BLOCK,&myset,NULL);
    for(i=0;i<10;i++)
    {
      pid=fork();
      if(pid<0)
      {
       sys_err("fork error");
      }
      else if(pid==0)
      {
        break;
      }
   }
if(i==10)
{
  struct sigaction act;
  act.sa_handler=catch_child;
  sigemptyset(&act.sa_mask);
  act.sa_flags=0;
  sigaction(SIGCHLD,&act,NULL);
  sigprocmask(SIG_UNBLOCK,&myset,NULL);
  printf("I am parent,pid=%d\n",getpid());
  while(1)
  { printf("--------\n");
    sleep(1);
  }
  
} 
else
 {
 printf("i am %d child,pid=%d\n",i,getpid());
 sleep(i);
}      
  
      return 0;
  }

 
