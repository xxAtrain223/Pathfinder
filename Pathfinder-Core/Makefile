CC=gcc
# CC=arm-frc-linux-gnueabi-gcc  # RoboRIO

TARGET=pathfinder

C=src
HEAD=include

CFLAGS=-shared -I$(HEAD) -g -lm
LDFLAGS=-shared

SRCS=$(C)/fit/hermite.c $(C)/mathutil.c $(C)/spline.c $(C)/trajectory.c $(C)/generator.c $(C)/modifiers/tank.c $(C)/modifiers/swerve.c
OBJS=$(SRCS:.c=.o)

ifeq ($(OS),Windows_NT)
    EXTTXT := dll
else
    EXTTXT := so
    PRETXT := lib
endif

ifeq ($(OS),Darwin)
    EXTTXT := dylib
endif

TARGET_NAME=$(PRETXT)$(TARGET).$(EXTTXT)

-include $(OBJS:.o=.d)

all: $(TARGET_NAME)

$(TARGET_NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET_NAME) $(OBJS)

clean: $(OBJS)
	rm -rf $(OBJS) $(TARGET_NAME)
