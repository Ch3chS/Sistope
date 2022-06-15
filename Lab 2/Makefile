TARGET = lab2
OBJS = lab2.c

CFLAGS = -Wall

$(TARGET) : $(OBJS)
	@gcc $(CFLAGS) $(OBJS) -o $(TARGET) -lm

%.o : %.c
	gcc -c $(CFLAGS) $< -o $@

.PHONY: clean
clean :
	@rm -f *.o *.out $(TARGET)
