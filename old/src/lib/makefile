CC=gcc

TARGET=libhotpi.a
OBJS=utils.o config.o logging.o
DEPS=*.h
LIBS=

# Release, use -DRELEASE to eliminate debug logging
# CFLAGS=-Wall -O2 -finline-functions -fno-strict-aliasing -fno-stack-protector
# LFLAGS=

# Debug
CFLAGS=-Wall -g  -fno-stack-protector -pg
LFLAGS=-g  -pg

all: $(TARGET)

live:

$(TARGET): $(OBJS) $(DEPS)
	ar rcs $(TARGET) $(OBJS)

clean:
	rm -f *.o $(TARGET)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
