include makefile.conf

all: $(TARGET)

$(TARGET): $(TTLIB_OBJS)
	$(AR) $@ $^

clean:
	$(RM) $(TTLIB_OBJS)
	$(RM) $(TARGET)

