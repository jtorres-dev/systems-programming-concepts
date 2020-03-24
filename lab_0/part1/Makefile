CC = gcc

PROG = part1
HDRS = student.h
SRCS = student.c part1.c

OBJS = $(SRCS:.c=.o)
$(PROG) : $(OBJS)
	$(CC) $(OBJS) -o $(PROG)
	
part1.o = part1.c student.h
student.o = student.c student.h
