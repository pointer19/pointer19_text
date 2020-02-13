src=$(wildcard *.c)
dev=$(patsubst %.c, %, $(src))

all:$(dev)
wrning = -Wall -g -lpthread

%:%.c
	gcc $< -o $@ $(wrning)

clean:
	-rm -rf $(dev)
.PHONY:clean
