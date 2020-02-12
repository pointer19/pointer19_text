src = $(wildcard *.c)  
obj = $(patsubst %.c, %, $(src))

all:$(obj)

wrning = -Wall -g -lpthread

%:%.c
	gcc $< -o $@ $(wrning)

.PHONY:clean
clean:
	-rm -rf $(obj)


