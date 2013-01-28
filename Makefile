BUILD_DIR := obj

CPP := g++
CC  := gcc
LD := g++

CFLAGS :=
LDFLAGS := -lcurses

OMEGA_CPP_SRC:= $(wildcard src/OmegaRPG/src/*.cpp)
OMEGA_CPP_OBJS := $(patsubst src/OmegaRPG/src/%.cpp, $(BUILD_DIR)/%.opp, $(OMEGA_CPP_SRC))

OMEGA_C_SRC := $(wildcard src/OmegaRPG/src/*.c)
OMEGA_C_OBJS := $(patsubst src/OmegaRPG/src/%.c, $(BUILD_DIR)/%.o, $(OMEGA_C_SRC))

all : omega

omega : build_dir $(OMEGA_CPP_OBJS) $(OMEGA_C_OBJS)
	$(LD) -o $@ $(OMEGA_CPP_OBJS) $(OMEGA_C_OBJS) $(LDFLAGS)

$(BUILD_DIR)/%.o : src/OmegaRPG/src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.opp : src/OmegaRPG/src/%.cpp
	$(CPP) $(CFLAGS) -o $@ -c $<

build_dir : 
	mkdir -p $(BUILD_DIR)

clean : 
	rm -rf $(BUILD_DIR)
