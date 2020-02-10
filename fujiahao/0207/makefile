src = $(wildcard *.c)
obj = $(patsubst %.c,%,$(src))

all:$(obj)

%:%.c
	gcc $< -o $@ 

clean:
	-rm -rf $(obj)
.PHONY:clean
