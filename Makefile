CC	:= gcc
LFLAGS	:= -lSDL2 -lSDL2_ttf -lm
CFLAGS	:= -Wall -Werror -fPIC  $(LFLAGS)
CTFLAGS	:= -Wall $(LFLAGS) -L/home/mz37/programming/SGK # my own path, this should be edited later
LDFLAGS = -shared

RM = rm -f   # rm command
TARGET_LIB = libSGK.so  # target lib

TARGET_TEST = test/test-container.out test/system-test.out

SRCS = src/SGK.c src/widgets.c src/container.c
OBJS = $(SRCS:.c=.o)



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

clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d) ${TARGET_TEST}
