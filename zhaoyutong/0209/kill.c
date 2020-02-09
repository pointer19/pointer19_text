  #include<stdio.h>
  #include<stdlib.h>
  #include<string.h>
  #include<unistd.h>
  #include<signal.h>
  #include<errno.h>
  #include<fcntl.h>
int main(int argc, char* argv[])
  {     
        pid_t pid;
        pid = fork();
        if(pid > 0)
        {
                fork();
                while(1);
        }
        if(pid == 0)
        {
                sleep(20);
                kill(0, SIGSEGV);
        }
  }
  


