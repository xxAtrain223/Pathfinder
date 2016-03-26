CC=gcc
# CC=arm-frc-linux-gnueabi-gcc  # RoboRIO

DESTDIR?=/usr
PREFIX?=/local

TARGET=pathfinderjava

C=src/native/c
HEAD=src/native/include
JNI_HEAD=src/native/jni_include

CFLAGS=-shared -I$(HEAD) -I$(JNI_HEAD) -I"$(JAVA_HOME)/include" -g -lm -lpathfinder -L../Pathfinder-Core -I../Pathfinder-Core/include 
LDFLAGS=-shared

JNI_HEADERS = jaci.pathfinder.PathfinderJNI

SRCS=$(C)/pathfinder_java.c
HEADERS=
OBJS=$(SRCS:.c=.o)

GRADLE=./gradlew

ifeq ($(OS),Windows_NT)
    EXTTXT := dll
    GRADLE = gradlew
    CFLAGS += -I"$(JAVA_HOME)/include/win32"
    OSSTR := Win
else
    EXTTXT := so
    PRETXT := lib
    OSSTR := Linux
endif

ifeq ($(shell uname -s),Darwin)
    EXTTXT := dylib
    OSSTR := Mac
endif

TARGET_NAME=$(PRETXT)$(TARGET).$(EXTTXT)

-include $(OBJS:.o=.d)

all: generate prepare $(TARGET)

generate:
	javah -cp build/classes/main -d $(JNI_HEAD) $(JNI_HEADERS)

prepare:
	mkdir -p build/c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o build/c/$(TARGET_NAME) $(OBJS)

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
