TARGET = jnl_ascii_dump

CC = gcc
CFLAGS += -Wall
CFLAGS += -g3
CFLAGS += -O0

$(TARGET) : jnl_ascii_dump.o asciiDump.o
	$(CC) $(CFLAGS) $^ -o $@

.PHONY : clean
clean:
	rm -f a.out $(TARGET) *.o gmon.out
