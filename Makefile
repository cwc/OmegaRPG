
CPP := g++
CC  := gcc
LD := g++

CFLAGS :=
LDFLAGS := -lcurses

CXX_SRC_FILES := $(wildcard src/OmegaRPG/src/*.cpp)
CXX_OBJS := $(patsubst src/OmegaRPG/src/%.cpp, obj/%.op, $(CXX_SRC_FILES))

C_SRC_FILES := $(wildcard src/OmegaRPG/src/*.c)
C_OBJS := $(patsubst src/OmegaRPG/src/%.c, obj/%.o, $(C_SRC_FILES))

omega : dir cppobj cobj
	$(LD) -o $@ $(CXX_OBJS) $(C_OBJS) $(LDFLAGS)

cppobj : $(CXX_OBJS)

cobj : $(C_OBJS)

obj/%.o : src/OmegaRPG/src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

obj/%.op : src/OmegaRPG/src/%.cpp
	$(CPP) $(CFLAGS) -o $@ -c $<

dir : 
	mkdir -p obj

clean : 
	rm -rf obj/
