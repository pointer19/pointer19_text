src = $(wildcard *.c)
obj =$(patsubst %.c, %, $(src))
aLL:$(obj)
wrning = -Wall -g
	
%:%.c
	gcc $< -o $@ $(wrning)
clean:
	-rm -rf $(obj)
.PHONY:clean
