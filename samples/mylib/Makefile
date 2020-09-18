include makefile.conf

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(WOPTION) -c $(SRCS)
	$(CC) -o $@ $^

clean:
	$(RM) *.o
	$(RM) $(TARGET)

