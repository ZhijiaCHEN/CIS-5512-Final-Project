CFLAGS = -O3
OBJS =-L ../../obj -lsng
INC =-I ../../include
#SIZE = $(MAT_SIZE)

all: nxdr matrix

nxdr :  mtclnt1 mtwrkr1

mtclnt1 : mmaster.c
	cc $(CFLAGS) $(INC) -DN=$(SIZE) -o mmaster mmaster.c $(OBJS)

mtwrkr1 : mworker.c
	cc $(CFLAGS) $(INC) -DN=$(SIZE) -o mworker mworker.c $(OBJS)

matrix : matrix.c
	cc $(CFLAGS) $(INC) -DN=$(SIZE) -o matrix matrix.c $(OBJS)
