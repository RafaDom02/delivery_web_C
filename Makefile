CC = gcc
CFLAGS = -Wall -Wextra -Isrc/include

SRCDIR = src
INCDIR = src/include

SOURCES = $(SRCDIR)/main.c $(SRCDIR)/srclib/conf.c
HEADERS = $(INCDIR)/conf.h

SRC_KILLER = $(SRCDIR)/killDemonized.c

TARGET = web-service
KILLER = web-killer

all: $(TARGET) $(KILLER)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

$(KILLER): $(SRC_KILLER)
	$(CC) $(CFLAGS) -o $(KILLER) $(SRC_KILLER)

clean:
	rm -f $(TARGET) $(KILLER)
