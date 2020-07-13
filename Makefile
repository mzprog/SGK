CC	:= gcc
LFLAGS	:= -lSDL2 -lSDL2_ttf -lm
CFLAGS	:= -Wall -Werror -fPIC  $(LFLAGS)
CTFLAGS	:= -Wall $(LFLAGS) 
LDFLAGS = -shared

RM = rm -f   # rm command
TARGET_LIB = libSGK.so  # target lib

TARGET_TEST = test/test-container.out test/system-test.out

SRCS = src/SGK.c src/widgets.c src/container.c
HDRS = $(SRCS:.c=.h) #we will edit it later

OBJS = $(SRCS:.c=.o)

DISTLIB = /usr/lib
DISTHDR = /usr/include/SGK


all: ${TARGET_LIB}

test/test-container.out: ${TARGET_LIB} test/container-test.c
	$(CC)  -o $@ test/container-test.c -lSGK  $(CTFLAGS)
	
test/system-test.out: ${TARGET_LIB} test/system-test.c
	$(CC)  -o $@ test/system-test.c -lSGK  $(CTFLAGS)

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.c=.d)

install: ${TARGET_LIB} 
	cp $< $(DISTLIB)
	chmod 0755  $(DISTLIB)/$(TARGET_LIB)
	ldconfig
	mkdir $(DISTHDR)
	cp $(HDRS) $(DISTHDR)

clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d) ${TARGET_TEST}
