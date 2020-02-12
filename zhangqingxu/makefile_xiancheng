src = $(wildcard *.c)                                                       
obj = $(patsubst %.c,%.o,$(src))
ALL:main
main:$(obj)
	gcc $^ -o $@
%.o:%.c
	gcc -c $< -o $@
.PHONY:clean
clean:
	rm -rf $(obj) main
