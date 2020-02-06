nclude <stdio.h>
#include <stdlib.h>
#include <unistd.h>
   int main(void)
  {
          pid_t pid;
          int fd[2];
          pipe(fd);
          pid = fork();
  
          if(pid < 0)
          {
                  perror("fork error");
                  exit(1);
          } 
          else if(pid == 0) //子进程
          {
                  dup2(fd[0],0);
                  close(fd[1]);    
                  execlp("wc","wc","-l",NULL);
          }
         else 
         {
                 dup2(fd[1],1); 
                 close(fd[0]); 
                 execlp("ls","ls","-l",NULL);
         }
         return 0;

 }
