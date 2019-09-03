PHONY : clean
CFLAGS = -g

src = $(wildcard *.c)
target = $(patsubst %.c, %.out, ${src})

%.out:%.c
	$(CC) $(CFLAGS) $^ -o $@

all:${target}


clean:
	rm -f ${target}
