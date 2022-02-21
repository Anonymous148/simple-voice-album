LIBS = $(wildcard lib/*)
CC = arm-none-linux-gnueabi-gcc

TARGET:=photo
SRCS:=$(wildcard ./src/*.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))

CPPFLAGS += -I ./inc
CPPFLAGS += -I ./inc/libxml2
LDFLAGS  += -L ./lib

LDFLAGS += -lxml2
LDFLAGS += -lasound
LDFLAGS += -lz
LDFLAGS += -lvnet 
LDFLAGS += -lpthread

LDFLAGS += -Wl,-rpath=. -Wl,-rpath=./lib


LDFLAGS += -Wl,-rpath=. -Wl,-rpath=./lib

$(TARGET):$(OBJS)
	$(CC) $^ -o $@ $(CPPFLAGS) $(LDFLAGS) -lsqlite3 -L/usr/local/sqlite/lib/ -I/usr/local/sqlite/include  -Wall
%.o:%.c
	$(CC) -c $^ -o $@ $(CPPFLAGS) $(LDFLAGS) -lsqlite3 -L/usr/local/sqlite/lib/ -I/usr/local/sqlite/include -Wall

clean:
	rm $(OBJS) $(TARGET) -rf
