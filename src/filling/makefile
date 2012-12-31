CC=gcc

TARGET=hotpi-filling
OBJS=filling.o
DEPS=*.h ../lib/libhotpi.a
LIBS=-lhotpi

# Debug
CFLAGS=-Wall -g -DLINUX -I../lib -fno-stack-protector -pg
LFLAGS=-g -L../lib -pg

# Release, use -DRELEASE to eliminate debug logging
# CFLAGS=-Wall -O2 -DLINUX -I../lib -finline-functions -fno-strict-aliasing -fno-stack-protector
# LFLAGS=-L../lib

all: $(TARGET)

clean:
	rm -f *.o $(TARGET) *.out

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS) $(DEPS)
	$(CC) $(LFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
