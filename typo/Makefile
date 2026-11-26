CC = cc
PKG_CONFIG ?= pkg-config

CFLAGS ?= -I/usr/local/include 
LDFLAGS = -L/usr/local/lib

CMARK_CFLAGS = $(shell $(PKG_CONFIG) --cflags libcmark)
CMARK_LIBS = $(shell $(PKG_CONFIG) --libs libcmark)

BIN = bin

.PHONY: all clean

all: $(BIN)/typo

$(BIN)/typo: main.c $(BIN)
	$(CC) $(CMARK_CFLAGS) $(CFLAGS) \
		-std=c89 -Wall -Wextra -Wpedantic \
		$(LDFLAGS) \
		-o $@ main.c \
		$(CMARK_LIBS) $(LDLIBS)

$(BIN):
	mkdir -p $@

clean:
	rm -rf $(BIN)
