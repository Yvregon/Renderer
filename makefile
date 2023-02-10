CC = g++
CFLAGS = -std=c++17 -O2

LFLAGS = tgaimage.h figures.h Obj.h Vector3f.h Matrix.h geometry.h
SRCS = tgaimage.cpp Obj.cpp Vector3f.cpp Matrix.cpp geometry.cpp main.cpp
OBJS = $(SRCS: .cpp=.o)

main: $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^

clean:
	rm -rf *.o *~ 
