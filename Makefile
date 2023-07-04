CC = gcc
CFLAGS = -Wall -Wextra -Isrc/include

SRCDIR = src/srclib
INCDIR = src/include

SOURCES = $(SRCDIR)/conf.c
HEADERS = $(INCDIR)/conf.h

TARGET = web-service

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
