CC	:= gcc
LFLAGS	:= -lSDL2 -lSDL2_ttf -lm
CFLAGS	:= -Wall -Werror -fPIC  $(LFLAGS)
CTFLAGS	:= -Wall $(LFLAGS) -L/home/mz37/programming/MZSDL
LDFLAGS = -shared

RM = rm -f   # rm command
TARGET_LIB = libSGK.so  # target lib

TARGET_TEST = test/test-container.out

SRCS = src/mzsdl.c src/container.c
OBJS = $(SRCS:.c=.o)



all: ${TARGET_LIB}

test/test-container.out: ${TARGET_LIB} test/container-test.c
	$(CC)  -o $@ test/container-test.c -lSGK  $(LFLAGS)

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.c=.d)

clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d) ${TARGET_TEST}
