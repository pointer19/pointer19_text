  #include<stdio.h>
  #include<stdlib.h>
  #include<string.h>
  #include<unistd.h>
  #include<errno.h>
  #include<fcntl.h> 
int main(int argc, char* argv[])
  {
        int ar1;
        ar1 = alarm(5);
        int i = 0;
        while(1)
        {
          printf("i = %d\n", i++);
        }
        return 0;
  }


