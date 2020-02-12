src = $(wildcard *.c)
obj = $(patsubst %.c, %, $(src))

all:$(obj)

wrning = -Wall -g -lpthread

%:%.c
	gcc $< -o $@ $(wrning)

clean:
	-rm -rf $(obj)
.PHONY:clean


