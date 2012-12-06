TARGET  := fmain
SRCS    := main.c tools.c object/bool.c object/vm.c object/nil.c object/object.c object/string.c object/symbol.c object/type.c object/map.c object/function.c object/thread.c parse/parser.c parse/scanner.c parse/ast.c object/integer.c
OBJS    := ${SRCS:.c=.o} 

CCFLAGS = -I. -std=gnu99 -Wall -ggdb 
#CCFLAGS = -I. -std=gnu99 -O2 -Wall -ggdb 
LDFLAGS = 
LIBS    = 

.PHONY: all clean distclean 

all:: ${TARGET} 

${TARGET}: ${OBJS} 
	${CC} ${LDFLAGS} -o $@ $^ ${LIBS} 

${OBJS}: %.o: %.c
	${CC} ${CCFLAGS} -o $@ -c $< 

clean:: 
	-rm -f *~ ${OBJS} ${TARGET} 

distclean:: clean
