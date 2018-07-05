# Template of makefile

# Write below after "TARGET =" the name of your program without ".c".
# (As the name of the sample program is "gap.c", the following
# line is "gap".)

TARGET = gap

# You can change the compiler, but note that the competition will
# be done by using "gcc -O2".

CC= gcc
CFLAGS= -Wall -O2

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o -lm

$(TARGET).o: $(TARGET).c cpu_time.c
	$(CC) $(CFLAGS) -c $(TARGET).c

clean:
	rm *.o
