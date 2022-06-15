TARGET = lab1
OBJS = lab1.c

TARGET2 = Vis
OBJS2 = Vis.c

CFLAGS = -Wall

$(TARGET) : $(OBJS)
	@gcc $(CFLAGS) $(OBJS) -o $(TARGET) -lm
	@gcc $(CFLAGS) $(OBJS2) -o $(TARGET2) -lm

%.o : %.c
	gcc -c $(CFLAGS) $< -o $@

.PHONY: clean
clean :
	@rm -f *.o *.out $(TARGET) $(TARGET2)