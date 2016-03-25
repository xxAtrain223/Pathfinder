CC=gcc
# CC=arm-frc-linux-gnueabi-gcc  # RoboRIO

DESTDIR?=/usr
PREFIX?=/local

TARGET=pathfinder

C=src
HEAD=include

CFLAGS=-shared -I$(HEAD) -g -lm
LDFLAGS=-shared

SRCS=$(C)/fit/hermite.c $(C)/mathutil.c $(C)/spline.c $(C)/trajectory.c $(C)/generator.c $(C)/modifiers/tank.c $(C)/modifiers/swerve.c
HEADERS=$(HEAD)/pathfinder/fit.h $(HEAD)/pathfinder/mathutil.h $(HEAD)/pathfinder/spline.h \
	$(HEAD)/pathfinder/structs.h $(HEAD)/pathfinder/trajectory.h $(HEAD)/pathfinder/modifiers/swerve.h $(HEAD)/pathfinder/modifiers/tank.h \
    $(HEAD)/pathfinder.h    
OBJS=$(SRCS:.c=.o)

ifeq ($(OS),Windows_NT)
    EXTTXT := dll
else
    EXTTXT := so
    PRETXT := lib
endif

ifeq ($(shell uname -s),Darwin)
    EXTTXT := dylib
endif

TARGET_NAME=$(PRETXT)$(TARGET).$(EXTTXT)

-include $(OBJS:.o=.d)

all: $(TARGET_NAME)

$(TARGET_NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET_NAME) $(OBJS)

clean: $(OBJS)
	rm -rf $(OBJS) $(TARGET_NAME)

install: $(all)
	install -m 0755 -d $(DESTDIR)$(PREFIX)/include
	install -m 0644 $(HEADERS) $(DESTDIR)$(PREFIX)/include
	install -m 0755 -d $(DESTDIR)$(PREFIX)/lib
	install -m 0755 $(TARGET_NAME) $(DESTDIR)$(PREFIX)/lib

uninstall:
	cd $(DESTDIR)$(PREFIX)/include/ && rm -f $(HEADERS)
	cd $(DESTDIR)$(PREFIX)/lib/ && rm -f $(TARGET_NAME)