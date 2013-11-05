CC=gcc

TARGET=honeybridge
OBJS=main.o utils.o logging.o serial.o enviracom.o
DEPS=*.h
LIBS=

# Release, use -DRELEASE to eliminate debug logging
CFLAGS=-Wall -O2 -DLINUX -I../lib -finline-functions -fno-strict-aliasing -fno-stack-protector
LFLAGS=-L../lib

# Debug
#CFLAGS=-Wall -g -DLINUX -I../lib -fno-stack-protector -pg
#LFLAGS=-g -L../lib -pg

all: $(TARGET)

install: $(TARGET)
	install -s honeybridge /usr/local/bin

clean:
	rm -f *.o $(TARGET) pid *.out

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS) $(DEPS)
	$(CC) $(LFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
