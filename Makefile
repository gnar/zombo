SRC += obj/object.c
SRC += obj/instance.c
SRC += obj/class.c
SRC += obj/nil.c
SRC += context.c
SRC += parse/scanner.c
SRC += parse/parser.c
SRC += parse/ast.c
SRC += main.c

####################################

CC = gcc -Wall -g 
EXE = zombo
INCLUDE_FLAGS += -I.
LDFLAGS = #-lmp -lm
DEFINES = 

#####################################

OBJ = ${SRC:.c=.o}

all: ${OBJ}
	${CC} ${OBJ} ${LDFLAGS} -o ${EXE}

%.o: %.c
	${CC} ${DEFINES} ${INCLUDE_FLAGS} -c $< -o $@

clean:
	-rm -f ${OBJ} ${EXE}
