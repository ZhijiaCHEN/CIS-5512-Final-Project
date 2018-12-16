SYS_LINK=-L/usr/lib
INCS = -I../include
OBJS = ../obj 
FLAGS = -g -pthread
cc = gcc

all : tsh tshtest launch

tsh : tsh.c 
	$(cc) $(EXTRA) $(INCS) $(FLAGS) -o tsh tsh.c -L$(OBJS) -lsng

tshtest : tshtest.c 
	$(cc) $(EXTRA) $(INCS) $(FLAGS) -o tshtest tshtest.c -L$(OBJS) -lsng

launch : launch.c 
	$(cc) $(EXTRA) $(INCS) $(FLAGS) -o launch launch.c -L$(OBJS) -lsng

.PHONY: clean

clean:
	rm tsh tshtest launch test
