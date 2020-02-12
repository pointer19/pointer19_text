rc=$(wildcard *.c)
obj=$(patsubst %.c, %, $(src))
ALL:$(obj)
wr=-Wall -g -lpthread
%:%.c
	gcc $< -o $@ $(wr)
clean:
	-rm -rf $(obj)
.PHONY:clean
