#Makefile for project1Greg.c
CC	=	gcc

CFLAGS = -Wall -g

TARGET = project1Greg

SRCS = project1Greg.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)
$(TARGET):	$(OBJS)
		$(CC)	$(CFLAGS)	-o	$(TARGET)	$(OBJS)

%.o: %.c
	$(CC)	$(CFLAGS)	-c	$<	-o	$@

clean:
	rm	-f	$(TARGET)	$(OBJS)
