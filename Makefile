CC = gcc
CFLAGS = -g -Wall
SRCMODULES = colors.c doors.c enemies.c game.c menupage.c quentin.c renderer.c results.c
OBJMODULES = $(SRCMODULES:.c=.o)

%.o: %.c %.h
			$(CC) $(CFLAGS) -c $< -o $@

strangecave: main.c const.h $(OBJMODULES)
				$(CC) $(CFLAGS) $^ -o $@ -lncurses

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCMODULES)
		$(CC) -MM $^ > $@

run: strangecave
				./strangecave

clean: rm -f *.o strangecave

