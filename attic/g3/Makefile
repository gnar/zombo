SRC += obj/Object.cpp 
SRC += obj/Instance.cpp
SRC += obj/Class.cpp 
SRC += obj/String.cpp 
SRC += obj/Integer.cpp
SRC += obj/Native.cpp
SRC += obj/True.cpp

SRC += gc/Collector.cpp

SRC += Context.cpp MessageMap.cpp Value.cpp 
SRC += main.cpp

####################################

CC = g++ -std=gnu++0x -g
EXE = g3
INCLUDE_FLAGS += -I.
LDFLAGS = -lmp -lm
DEFINES = 

#####################################

OBJ = ${SRC:.cpp=.o}

all: ${OBJ}
	${CC} ${OBJ} ${LDFLAGS} -o ${EXE}

%.o: %.cpp
	${CC} ${DEFINES} ${INCLUDE_FLAGS} -c $< -o $@

clean:
	-rm -f ${OBJ} ${EXE}
