  #include<stdio.h>
  #include<stdlib.h>
  #include<string.h>
  #include<unistd.h>
  #include<errno.h>
  #include<fcntl.h>
  
int main(int argc, char* argv[])
  {
  
     int fd[2];
     char buf[10240];
     int pr=pipe(fd);
     pid_t pid=fork();
     if(pr<0)
     {
       perror("pipe error");
       exit(1);
     }
     if(pid==0)
     {
       close(fd[0]);
       dup2(fd[1],STDOUT_FILENO);
       execlp("ls","ls","-l",NULL);
    }
   if(pid>0)
    {
      close(fd[1]);
      dup2(fd[0],STDIN_FILENO);
      wait(NULL);
      execlp("wc","wc","-l",NULL);
      return 0;
   }
  }



