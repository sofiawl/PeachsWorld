TARGET = peach

SRC = \
    main.c \
    peach.c \
    menu.c \
    background.c \
    enemy.c \
    boss.c

HEADERS = \
    peach.h \
    menu.h \
    background.h \
    enemy.h \
    boss.h

CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lm

ALLEGRO_FLAGS = $(shell pkg-config --libs --cflags allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 allegro_ttf-5)

all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(ALLEGRO_FLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
